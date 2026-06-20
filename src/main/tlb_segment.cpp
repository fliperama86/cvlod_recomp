// TLB Segment Address Translation for Castlevania: Legacy of Darkness
//
// On real N64, osMapTLB maps virtual addresses (kuseg) to physical RDRAM.
// In the recomp, MEM_W(seg_addr) computes: rdram + seg_addr + 0x80000000
// For seg_addr 0x0F000000 → rdram + 0x8F000000 (in PROT_NONE region).
//
// Strategy: mprotect the target region so segment accesses don't SIGBUS,
// and memcpy the data there FROM the physical address. When the game
// remaps the TLB entry, we update our copy.
//
// The key insight: the game DMAs overlay data to a physical RDRAM address
// first, THEN calls osMapTLB to make it accessible via segment addressing.
// So when osMapTLB is called, the data is already in rdram at even_paddr/odd_paddr.

#include <cstdio>
#include <cstdint>
#include <cstring>
#include "lod/lod_mem_compat.h"

#include "recomp.h"

#ifndef LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
#define LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS 0
#endif

#ifndef LOD_ENABLE_TLB_SEG6_TRACE
#define LOD_ENABLE_TLB_SEG6_TRACE 0
#endif

#ifndef LOD_FIX_NI_TLB_SKIP_ACTUAL_SPAN
#define LOD_FIX_NI_TLB_SKIP_ACTUAL_SPAN 0
#endif

// Track TLB entries so we can re-copy when mappings change
struct TlbEntry {
    uint32_t vaddr;
    uint32_t even_paddr;
    uint32_t odd_paddr;
    uint32_t page_size;
    bool valid;
};

static TlbEntry tlb_table[32] = {};

// NI overlay loader hook
extern "C" bool ni_overlay_on_tlb_map(uint8_t* rdram, uint32_t vaddr,
                                       uint32_t even_paddr, uint32_t odd_paddr);
extern "C" uint32_t ni_overlay_loaded_span(uint32_t vaddr);

static constexpr uint32_t NI_SEG_0F_BASE = 0x0F000000;
static constexpr uint32_t NI_SEG_0E_BASE = 0x0E000000;
static constexpr uint32_t NI_TLB_LEGACY_SKIP_SPAN = 0x00010000;
static constexpr uint32_t NI_TLB_MAX_SEGMENT_SPAN = 0x00100000;

#if LOD_ENABLE_TLB_SEG6_TRACE
static constexpr uint32_t SEG6_TRACE_BASE = 0x06000000;
static constexpr uint32_t SEG6_TRACE_SPAN = 0x01000000;
static constexpr uint32_t SEG6_TRACE_FOCUS = 0x060073DC;

static bool range_overlaps_u32(uint32_t lhs_start, uint32_t lhs_size,
                               uint32_t rhs_start, uint32_t rhs_size) {
    const uint64_t lhs_end = (uint64_t)lhs_start + lhs_size;
    const uint64_t rhs_end = (uint64_t)rhs_start + rhs_size;
    return (uint64_t)lhs_start < rhs_end && (uint64_t)rhs_start < lhs_end;
}

static bool range_contains_u32(uint32_t start, uint32_t size, uint32_t value) {
    return (uint64_t)value >= start && (uint64_t)value < (uint64_t)start + size;
}

static uint32_t trace_read_paddr_word(uint8_t* rdram, uint32_t paddr) {
    return (uint32_t)MEM_W(paddr, (gpr)(int32_t)0x80000000);
}

static uint32_t trace_read_vaddr_word(uint8_t* rdram, uint32_t vaddr) {
    return (uint32_t)MEM_W(0, (gpr)(int32_t)vaddr);
}
#endif

static uint32_t align_up_u32(uint32_t value, uint32_t alignment) {
    if (alignment == 0) {
        return value;
    }
    return ((value + alignment - 1) / alignment) * alignment;
}

static bool ni_segment_base_for_vaddr(uint32_t vaddr, uint32_t* base_out) {
    if (vaddr >= NI_SEG_0F_BASE && vaddr < NI_SEG_0F_BASE + NI_TLB_MAX_SEGMENT_SPAN) {
        *base_out = NI_SEG_0F_BASE;
        return true;
    }
    if (vaddr >= NI_SEG_0E_BASE && vaddr < NI_SEG_0E_BASE + NI_TLB_MAX_SEGMENT_SPAN) {
        *base_out = NI_SEG_0E_BASE;
        return true;
    }
    return false;
}

static bool tlb_paddr_copyable(uint32_t paddr, uint32_t size) {
    return paddr != 0xFFFFFFFF && (uint64_t)paddr + size <= 0x20000000ULL;
}

static bool tlb_range_contains_u64(uint32_t start, uint32_t size,
                                   uint64_t query_start, uint64_t query_end) {
    const uint64_t end = (uint64_t)start + size;
    return query_start >= (uint64_t)start && query_end <= end;
}

extern "C" bool lod_tlb_translate_virtual_to_physical(uint32_t vaddr, uint32_t size, uint32_t* paddr_out) {
    if (paddr_out == nullptr || size == 0) {
        return false;
    }
    const uint64_t query_start = vaddr;
    const uint64_t query_end = query_start + size;
    if (query_end < query_start) {
        return false;
    }

    for (const TlbEntry& entry : tlb_table) {
        if (!entry.valid) {
            continue;
        }

        const uint32_t even_start = entry.vaddr;
        const uint32_t odd_start = entry.vaddr + entry.page_size;
        if (tlb_range_contains_u64(even_start, entry.page_size, query_start, query_end) &&
            tlb_paddr_copyable(entry.even_paddr, (uint32_t)(vaddr - even_start) + size)) {
            *paddr_out = entry.even_paddr + (vaddr - even_start);
            return true;
        }
        if (tlb_range_contains_u64(odd_start, entry.page_size, query_start, query_end) &&
            tlb_paddr_copyable(entry.odd_paddr, (uint32_t)(vaddr - odd_start) + size)) {
            *paddr_out = entry.odd_paddr + (vaddr - odd_start);
            return true;
        }
    }
    return false;
}

extern "C" uint32_t lod_tlb_translate_virtual_to_physical_or_zero(uint32_t vaddr, uint32_t size) {
    uint32_t paddr = 0;
    return lod_tlb_translate_virtual_to_physical(vaddr, size, &paddr) ? paddr : 0;
}

// Override the weak RECOMP_FUNC stub in funcs_40.c
// osMapTLB(index, page_mask, vaddr, even_paddr, odd_paddr, asid)
extern "C" void func_80097730(uint8_t* rdram, recomp_context* ctx) {
    int32_t  index      = (int32_t)ctx->r4;
    uint32_t page_mask  = (uint32_t)ctx->r5;
    uint32_t vaddr      = (uint32_t)ctx->r6;
    uint32_t even_paddr = (uint32_t)ctx->r7;
    uint32_t odd_paddr  = MEM_W(0x10, ctx->r29);


    uint32_t mask_field = (page_mask >> 13) & 0xFFF;
    uint32_t page_size  = (mask_field + 1) * 4096;
    uint32_t entry_size = page_size * 2;

#if LOD_ENABLE_TLB_SEG6_TRACE
    const bool seg6_trace_map = range_overlaps_u32(vaddr, entry_size, SEG6_TRACE_BASE, SEG6_TRACE_SPAN);
    const bool seg6_focus_map = range_contains_u32(vaddr, entry_size, SEG6_TRACE_FOCUS);
    static uint64_t seg6_trace_count = 0;
    bool seg6_should_log = false;
    if (seg6_trace_map) {
        seg6_trace_count++;
        seg6_should_log = seg6_trace_count <= 256 || seg6_focus_map || (seg6_trace_count % 1000) == 0;
        if (seg6_should_log) {
            fprintf(stderr,
                    "[osMapTLB_SEG6] map#%llu idx=%d vaddr=0x%08X size=0x%X page=0x%X mask=0x%08X even=0x%08X odd=0x%08X focus=%d\n",
                    (unsigned long long)seg6_trace_count, index, vaddr, entry_size, page_size, page_mask,
                    even_paddr, odd_paddr, seg6_focus_map ? 1 : 0);
        }
    }
#endif

    // Hook: load NI overlay when mapping 0x0F000000 or 0x0E000000.
    // This copies full overlay data from extended ROM to the segment region.
    // We must skip the normal TLB copy below to avoid overwriting with the
    // small decompressed blob.
    //
    // Multi-page overlays generate MULTIPLE TLB entries (e.g., 0x0F000000
    // and 0x0F002000). Only the base entry triggers the overlay loader.
    // Subsequent entries within the same region must also skip the normal
    // TLB copy, since copy_overlay_data_to_segment already wrote the full
    // overlay data. Without this, the normal handler overwrites the data
    // section with raw game-decompressed bytes (potentially wrong format).
    //
    // Preserve the old first-64KB skip window for compatibility by default, but
    // allow experiments to use the actual loaded NI overlay span. Several real
    // NI overlays are larger than 64KB, and normal-copying later TLB entries
    // such as 0x0F012000 can corrupt the segment bytes the NI loader already
    // populated. Conversely, using a 64KB minimum for smaller overlays can leave
    // stale bytes past the actual overlay image in the segment mirror.
    uint32_t ni_base = 0;
    if (ni_segment_base_for_vaddr(vaddr, &ni_base)) {
        bool is_base_map = (vaddr == ni_base);
        bool base_map_matched_overlay = false;
        if (is_base_map) {
            base_map_matched_overlay = ni_overlay_on_tlb_map(rdram, vaddr, even_paddr, odd_paddr);
        }

        uint32_t skip_span = ni_overlay_loaded_span(ni_base);
        if (is_base_map && !base_map_matched_overlay) {
            // This base mapping did not identify a new NI executable overlay.
            // Keep any previous recompiled functions registered, but let the
            // normal TLB copy update the mapped segment bytes for this page.
        } else if (skip_span == 0) {
            // No known NI overlay is resident for this segment. Fall through to
            // the normal TLB copy path.
        } else {
            if (!LOD_FIX_NI_TLB_SKIP_ACTUAL_SPAN && skip_span < NI_TLB_LEGACY_SKIP_SPAN) {
                skip_span = NI_TLB_LEGACY_SKIP_SPAN;
            } else if (skip_span > NI_TLB_MAX_SEGMENT_SPAN) {
                skip_span = NI_TLB_MAX_SEGMENT_SPAN;
            }
            skip_span = align_up_u32(skip_span, entry_size);

            if (vaddr < ni_base + skip_span) {
                // Save mapping info but skip data copy — overlay loader already populated
                if (index >= 0 && index < 32) {
                    tlb_table[index] = { vaddr, even_paddr, odd_paddr, page_size, true };
                }
#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
                static int log_count_ni = 0;
                log_count_ni++;
                if (log_count_ni <= 10 || (log_count_ni % 5000) == 0) {
                    fprintf(stderr, "[osMapTLB] #%d idx=%d vaddr=0x%08X even=0x%08X odd=0x%08X page=0x%X ni_span=0x%X (NI skip copy)\n",
                            log_count_ni, index, vaddr, even_paddr, odd_paddr, page_size, skip_span);
                }
#endif
                return;
            }
        }
    }

    // Handle osUnmapTLB (index = -1 or vaddr = 0xFFFFFFFF)
    if (index < 0 || index >= 32) {
        return;
    }

    // Compute rdram offset for this virtual address
    uint64_t rdram_seg_offset = (uint64_t)vaddr + 0x80000000ULL;

    if (rdram_seg_offset + entry_size > 0x100000000ULL) {
        fprintf(stderr, "[osMapTLB] WARNING: vaddr=0x%08X maps beyond 4GB\n", vaddr);
        return;
    }

    // Make the target region accessible.
    // Round down to OS page boundary (macOS ARM uses 16KB pages).
    uint8_t* target = rdram + rdram_seg_offset;
    uintptr_t page_mask_os = sysconf(_SC_PAGESIZE) - 1;
    uint8_t* aligned_target = (uint8_t*)((uintptr_t)target & ~page_mask_os);
    size_t aligned_size = (target + entry_size - aligned_target + page_mask_os) & ~page_mask_os;
    if (mprotect(aligned_target, aligned_size, PROT_READ | PROT_WRITE) != 0) {
        perror("[osMapTLB] mprotect failed");
        fprintf(stderr, "[osMapTLB]   idx=%d vaddr=0x%08X target=%p offset=0x%llX size=0x%X page=0x%X\n",
                index, vaddr, target, (unsigned long long)rdram_seg_offset, entry_size, page_size);
        fprintf(stderr, "[osMapTLB]   even=0x%08X odd=0x%08X page_mask=0x%08X\n",
                even_paddr, odd_paddr, page_mask);
        return;
    }

    // Copy data from physical addresses to the virtual address region.
    // This is needed because MEM_W(seg_addr) goes to rdram+seg_addr+0x80000000,
    // not to rdram+paddr. We must keep the virtual region in sync.
    //
    // Skip copy when paddr points to the very start of RDRAM (0x0) or to
    // addresses that overlap with ROM-initialized overlay data, as the game
    // may be remapping segments that our init code already populated correctly.
    // On real N64, TLB just redirects reads — no copying needed. The game's
    // DMA already placed the right data at paddr. Our segment region was
    // pre-filled during init, so only update if paddr looks like a real
    // overlay DMA destination (typically in the 0x1000-0x7FFFFF range).
    // Copy data from physical address to the virtual segment region.
    // The game DMAs overlay/asset data to physical RDRAM, then maps it via TLB.
    // We replicate this by copying from rdram+paddr to the segment region.
    // Copy from physical address to the segment region in RDRAM.
    // Handles both low RDRAM addresses (0x1000-0x7FFFFF, DMA'd overlay data)
    // and ROM-mapped addresses (0x10000000+, NI file system data).
    auto do_tlb_copy = [&](uint32_t paddr, uint8_t* dst, uint32_t size) {
        // Check bounds: must be within the usable RDRAM+ROM region.
        if (tlb_paddr_copyable(paddr, size)) {
            memcpy(dst, rdram + paddr, size);
        }
    };
    do_tlb_copy(even_paddr, target, page_size);
    do_tlb_copy(odd_paddr, target + page_size, page_size);

#if LOD_ENABLE_TLB_SEG6_TRACE
    if (seg6_should_log) {
        auto trace_half = [&](const char* name, uint32_t half_vaddr, uint32_t paddr) {
            const bool present = paddr != 0xFFFFFFFF;
            const bool copyable = present && (uint64_t)paddr + page_size <= 0x20000000ULL;
            const bool has_focus = range_contains_u32(half_vaddr, page_size, SEG6_TRACE_FOCUS);
            const uint32_t first_src = copyable ? trace_read_paddr_word(rdram, paddr) : 0;
            const uint32_t first_dst = trace_read_vaddr_word(rdram, half_vaddr);
            uint32_t focus_src = 0;
            uint32_t focus_dst = 0;
            uint32_t focus_off = 0;
            if (has_focus) {
                focus_off = SEG6_TRACE_FOCUS - half_vaddr;
                if (copyable && (uint64_t)focus_off + 4 <= page_size) {
                    focus_src = trace_read_paddr_word(rdram, paddr + focus_off);
                }
                focus_dst = trace_read_vaddr_word(rdram, SEG6_TRACE_FOCUS);
            }
            fprintf(stderr,
                    "[osMapTLB_SEG6]   %s half_vaddr=0x%08X paddr=0x%08X copied=%d first_src=0x%08X first_dst=0x%08X",
                    name, half_vaddr, paddr, copyable ? 1 : 0, first_src, first_dst);
            if (has_focus) {
                fprintf(stderr, " focus_off=0x%X focus_src=0x%08X focus_dst=0x%08X",
                        focus_off, focus_src, focus_dst);
            }
            fputc('\n', stderr);
        };
        trace_half("even", vaddr, even_paddr);
        trace_half("odd ", vaddr + page_size, odd_paddr);
    }
#endif

    // Save the mapping for debugging
    tlb_table[index] = { vaddr, even_paddr, odd_paddr, page_size, true };

#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
    static int log_count = 0;
    log_count++;
    if (log_count <= 10 || (log_count % 5000) == 0) {
        fprintf(stderr, "[osMapTLB] #%d idx=%d vaddr=0x%08X even=0x%08X odd=0x%08X page=0x%X\n",
                log_count, index, vaddr, even_paddr, odd_paddr, page_size);
    }
#endif
}

// osUnmapTLB
extern "C" void func_800977D0(uint8_t* rdram, recomp_context* ctx) {
    int32_t index = (int32_t)ctx->r4;
    if (index >= 0 && index < 32) {
#if LOD_ENABLE_TLB_SEG6_TRACE
        if (tlb_table[index].valid &&
            range_overlaps_u32(tlb_table[index].vaddr, tlb_table[index].page_size * 2,
                               SEG6_TRACE_BASE, SEG6_TRACE_SPAN)) {
            fprintf(stderr, "[osUnmapTLB_SEG6] idx=%d vaddr=0x%08X size=0x%X even=0x%08X odd=0x%08X\n",
                    index, tlb_table[index].vaddr, tlb_table[index].page_size * 2,
                    tlb_table[index].even_paddr, tlb_table[index].odd_paddr);
        }
#endif
        tlb_table[index].valid = false;
    }
}
