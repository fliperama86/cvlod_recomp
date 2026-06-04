// NI Overlay Runtime Loader
//
// The game uses two TLB-mapped overlay regions:
//   0x0F000000: game object overlays (enemies, UI, etc.) — swap in/out
//   0x0E000000: cutscene/textbox overlays — swap independently
//
// When osMapTLB maps either vaddr, we match the physical address to an NI
// file index, then load the correct recompiled overlay section.

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#if defined(__APPLE__) || defined(__linux__)
#include <dlfcn.h>
#endif

#include "recomp.h"
#include "librecomp/overlays.hpp"

#ifndef LOD_ENABLE_SAVE_PAIR120_TRACE
#define LOD_ENABLE_SAVE_PAIR120_TRACE 0
#endif

#ifndef LOD_FIX_NI_PAIR120_RESULT_LABELS
#define LOD_FIX_NI_PAIR120_RESULT_LABELS 1
#endif

extern "C" void load_overlay_by_id(uint32_t id, uint32_t ram_addr);
extern "C" void unload_overlay_by_id(uint32_t id);

// Full overlay data table (rom offsets + sizes in extended ROM)
#include "ni_ovl_data.h"

// NI pairs: text_ni_index 0x1C5 (453) → pair 0 → overlay index 48 (first NI in overlays.txt)
// overlay_id = 48 + pair_index
// pair_index = (ni_text_index - 0x1C5) / 2
static constexpr int NI_TEXT_INDEX_START = 0x1C5;
static constexpr int NI_OVERLAY_ID_START = 54;
static constexpr int NI_PAIR_COUNT = 245;

// Track separately for 0x0E and 0x0F (they're independent)
static int loaded_0f_pair = -1;
static int loaded_0e_pair = -1;

#if LOD_FIX_NI_PAIR120_RESULT_LABELS
static const char* lod_ni_pair120_internal_label_name(uint32_t target) {
    switch (target) {
        case 0x0F00086C: return "pair120_label_clear";
        case 0x0F0008D4: return "pair120_label_10b";
        case 0x0F0008E4: return "pair120_label_10a";
        case 0x0F000958: return "pair120_label_obj3c";
        case 0x0F000964: return "pair120_label_111";
        case 0x0F000974: return "pair120_label_112";
        default: return nullptr;
    }
}

static bool lod_ni_pair120_call_internal_result_label(uint32_t target, uint8_t* rdram,
                                                       recomp_context* ctx) {
    switch (target) {
        case 0x0F00086C:
            // Original internal label 0x0F00086C inside ni_ovl_120_func_0F000860:
            //   sw a0, 0(sp); sh -1, 0x801CAE1C; v0 = 0; jr ra
            ctx->r14 = ADD32(0, -1);
            ctx->r1 = S32(0x801D << 16);
            MEM_W(0x0, ctx->r29) = ctx->r4;
            MEM_H(-0x51E4, ctx->r1) = (int16_t)-1;
            ctx->r2 = 0;
            return true;
        case 0x0F0008D4:
            ctx->r14 = ADD32(0, 0x10B);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x10B);
            return true;
        case 0x0F0008E4:
            ctx->r14 = ADD32(0, 0x10A);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x10A);
            return true;
        case 0x0F000958:
            ctx->r2 = MEM_W(0x3C, ctx->r4);
            MEM_W(0x38, ctx->r4) = ctx->r2;
            return true;
        case 0x0F000964:
            ctx->r14 = ADD32(0, 0x111);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x111);
            return true;
        case 0x0F000974:
            ctx->r14 = ADD32(0, 0x112);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x112);
            return true;
        default:
            return false;
    }
}

static void lod_call_ni_pair120_result_callback(uint8_t* rdram, recomp_context* ctx,
                                                 uint32_t target) {
    if (!lod_ni_pair120_call_internal_result_label(target, rdram, ctx)) {
        LOOKUP_FUNC((int32_t)target)(rdram, ctx);
    }
}

static void lod_fixed_ni_pair120_result_schedule_dispatch(uint8_t* rdram, recomp_context* ctx) {
    // Scoped compatibility shim for ni_ovl_120_result_schedule_dispatch.
    // The pair-120 result table at 0x0F0016BC intentionally contains a mix of
    // function starts and internal labels. N64Recomp's range fallback resolves
    // internal-label targets to their containing function starts, which changes
    // callback return values and can reset the save/Controller Pak state machine.
    ctx->r29 = ADD32(ctx->r29, -0x18);
    MEM_W(0x14, ctx->r29) = ctx->r31;
    MEM_W(0x18, ctx->r29) = ctx->r4;

    ctx->r14 = MEM_W(ctx->r4, 0x48);
    ctx->r25 = S32(0x0F00 << 16);
    ctx->r15 = S32(ctx->r14 << 2);
    ctx->r25 = ADD32(ctx->r25, ctx->r15);
    ctx->r25 = MEM_W(ctx->r25, 0x16BC);

    lod_call_ni_pair120_result_callback(rdram, ctx, (uint32_t)ctx->r25);

    ctx->r7 = MEM_W(ctx->r29, 0x18);
    ctx->r24 = ADD32(0, 1);
    ctx->r25 = S32(0x8000 << 16);
    ctx->r3 = ADD32(ctx->r7, 0x34);
    MEM_W(0x4, ctx->r3) = ctx->r2;

    if (ctx->r2 != 0) {
        ctx->r25 = ADD32(S32(0x8000 << 16), 0x1E30);
        ctx->r4 = ADD32(ctx->r7, 0x8);
        ctx->r5 = ADD32(ctx->r7, 0xE);
        ctx->r6 = 0;
        LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        ctx->r31 = MEM_W(ctx->r29, 0x14);
    } else {
        ctx->r25 = ADD32(ctx->r25, 0x1CE8);
        MEM_W(0x10, ctx->r3) = ctx->r24;
        ctx->r4 = ADD32(ctx->r7, 0x8);
        ctx->r5 = ADD32(ctx->r7, 0xE);
        LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        ctx->r31 = MEM_W(ctx->r29, 0x14);
    }

    ctx->r29 = ADD32(ctx->r29, 0x18);
}

static void lod_install_ni_pair120_dispatch_fix(const char* reason) {
    recomp::overlays::add_loaded_function((int32_t)0x0F000484,
        lod_fixed_ni_pair120_result_schedule_dispatch);

#if LOD_ENABLE_SAVE_PAIR120_TRACE
    static int install_count = 0;
    install_count++;
    if (install_count <= 8 || (install_count % 100) == 0) {
        fprintf(stderr,
            "[PAIR120-FIX] installed scoped result dispatcher fix #%d reason=%s\n",
            install_count, reason);
    }
#else
    (void)reason;
#endif
}
#endif

#if LOD_ENABLE_SAVE_PAIR120_TRACE
static recomp_func_t* lod_orig_ni_pair120_result_schedule_dispatch = nullptr;

static bool lod_pair120_rdram_range_ok(uint32_t phys, uint32_t size) {
    return phys < 0x00800000 && size <= 0x00800000 && phys <= 0x00800000 - size;
}

static uint32_t lod_pair120_rdram_u32(uint8_t* rdram, uint32_t phys) {
    if (!lod_pair120_rdram_range_ok(phys, 4)) {
        return 0;
    }
    return *(uint32_t*)(rdram + phys);
}

static uint8_t lod_pair120_rdram_u8(uint8_t* rdram, uint32_t phys) {
    if (!lod_pair120_rdram_range_ok(phys, 1)) {
        return 0;
    }
    return rdram[phys ^ 3];
}

static int16_t lod_pair120_rdram_s16(uint8_t* rdram, uint32_t phys) {
    if (!lod_pair120_rdram_range_ok(phys, 2)) {
        return 0;
    }
    return *(int16_t*)(rdram + (phys ^ 2));
}

static int32_t lod_pair120_current_gamestate(uint8_t* rdram) {
    constexpr uint32_t GSM_PTR_PHYS = 0x0C1520; // 0x800C1520
    uint32_t gsm_addr = lod_pair120_rdram_u32(rdram, GSM_PTR_PHYS);
    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (gsm_addr == 0 || !lod_pair120_rdram_range_ok(gsm_phys + 0x24, 4)) {
        return -1;
    }
    return (int32_t)lod_pair120_rdram_u32(rdram, gsm_phys + 0x24);
}

static void lod_pair120_host_symbol(recomp_func_t* func, const char** symbol_out,
                                    uintptr_t* offset_out) {
    const char* symbol = "(unknown)";
    uintptr_t offset = 0;
#if defined(__APPLE__) || defined(__linux__)
    Dl_info info = {};
    if (func != nullptr && dladdr((void*)func, &info) != 0) {
        if (info.dli_sname != nullptr) {
            symbol = info.dli_sname;
        }
        uintptr_t sym_base = (uintptr_t)info.dli_saddr;
        uintptr_t addr = (uintptr_t)func;
        if (sym_base != 0 && addr >= sym_base) {
            offset = addr - sym_base;
        }
    }
#endif
    *symbol_out = symbol;
    *offset_out = offset;
}

static void lod_trace_ni_pair120_result_schedule_dispatch(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    count++;

    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj & 0x1FFFFFFF;
    bool obj_ok = lod_pair120_rdram_range_ok(obj_phys, 0x74);
    int32_t gs = lod_pair120_current_gamestate(rdram);

    uint8_t before_state = obj_ok ? lod_pair120_rdram_u8(rdram, obj_phys + 0x09) : 255;
    int16_t before_func_id = obj_ok ? lod_pair120_rdram_s16(rdram, obj_phys + 0x0E) : 0;
    uint32_t before_38 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x38) : 0;
    uint32_t before_44 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x44) : 0;
    uint32_t before_48 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x48) : 0;
    uint32_t callback_target = 0;
    recomp_func_t* callback_host = nullptr;
    if (before_48 < 64) {
        callback_target = (uint32_t)MEM_W(0x16BC + before_48 * 4, (gpr)(int32_t)0x0F000000);
        if (callback_target >= 0x0F000000 && callback_target < 0x0F010000) {
            callback_host = get_function((int32_t)callback_target);
        }
    }

    const char* callback_symbol = "(unknown)";
    uintptr_t callback_offset = 0;
#if LOD_FIX_NI_PAIR120_RESULT_LABELS
    if (const char* label_name = lod_ni_pair120_internal_label_name(callback_target)) {
        callback_symbol = label_name;
    } else
#endif
    {
        lod_pair120_host_symbol(callback_host, &callback_symbol, &callback_offset);
    }

    bool interesting = obj_ok && obj_phys >= 0x31A000 && obj_phys < 0x31D000;
    bool should_log = count <= 32 || (gs == 4 && interesting);
    if (should_log) {
        fprintf(stderr,
            "[PAIR120] enter #%d gs=%d obj=0x%08X state09=%u func_id=%d "
            "idx_pre=%u cb=0x%08X cb_host=%p cb_symbol=%s+0x%zX "
            "obj38_pre=0x%08X obj44_pre=0x%08X obj48_pre=0x%08X ra=0x%08X sp=0x%08X\n",
            count, gs, obj, before_state, before_func_id, before_48,
            callback_target, (void*)callback_host, callback_symbol, (size_t)callback_offset,
            before_38, before_44, before_48, (uint32_t)ctx->r31, (uint32_t)ctx->r29);
    }

    if (lod_orig_ni_pair120_result_schedule_dispatch != nullptr) {
        lod_orig_ni_pair120_result_schedule_dispatch(rdram, ctx);
    }

    if (should_log) {
        uint8_t after_state = obj_ok ? lod_pair120_rdram_u8(rdram, obj_phys + 0x09) : 255;
        int16_t after_func_id = obj_ok ? lod_pair120_rdram_s16(rdram, obj_phys + 0x0E) : 0;
        uint32_t after_38 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x38) : 0;
        uint32_t after_44 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x44) : 0;
        uint32_t after_48 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x48) : 0;
        const char* branch = after_38 != 0 ? "reset-nonzero" : "advance-zero";
        fprintf(stderr,
            "[PAIR120] exit #%d gs=%d obj=0x%08X state09=%u->%u func_id=%d->%d "
            "idx_pre=%u cb=0x%08X result_obj38=0x%08X obj44=0x%08X obj48=0x%08X branch=%s v0_after=0x%08X ra=0x%08X\n",
            count, lod_pair120_current_gamestate(rdram), obj, before_state, after_state,
            before_func_id, after_func_id, before_48, callback_target,
            after_38, after_44, after_48, branch, (uint32_t)ctx->r2, (uint32_t)ctx->r31);
    }
}

static void lod_install_ni_pair120_trace_wrapper(const char* reason) {
#if LOD_FIX_NI_PAIR120_RESULT_LABELS
    lod_orig_ni_pair120_result_schedule_dispatch =
        lod_fixed_ni_pair120_result_schedule_dispatch;
#else
    recomp_func_t* current = get_function((int32_t)0x0F000484);
    if (current != lod_trace_ni_pair120_result_schedule_dispatch) {
        lod_orig_ni_pair120_result_schedule_dispatch = current;
    }
#endif
    recomp::overlays::add_loaded_function((int32_t)0x0F000484,
        lod_trace_ni_pair120_result_schedule_dispatch);

    static int install_count = 0;
    install_count++;
    if (install_count <= 8 || (install_count % 100) == 0) {
        fprintf(stderr,
            "[PAIR120] installed result dispatcher trace #%d reason=%s original=%p\n",
            install_count, reason, (void*)lod_orig_ni_pair120_result_schedule_dispatch);
    }
}
#endif



// 0x0E overlay pairs (cutscene/textbox) — classified by internal jal targets
// These are compiled for vram 0x0E000000
static const int pairs_0e[] = {
    135, 137, 138, 149, 150, 151, 152, 153, 154, 157,
    159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
    169, 170, 171, 172, 173, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 205,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
    218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
    228, 229, 230, 237, 238, 239, 240, 241, 244,
};
static constexpr int NUM_0E_PAIRS = sizeof(pairs_0e) / sizeof(pairs_0e[0]);

static bool is_0e_pair(int pair) {
    for (int i = 0; i < NUM_0E_PAIRS; i++) {
        if (pairs_0e[i] == pair) return true;
    }
    return false;
}

static int ni_index_to_pair(int ni_index) {
    if (ni_index < NI_TEXT_INDEX_START) return -1;
    int offset = ni_index - NI_TEXT_INDEX_START;
    if (offset % 2 != 0) return -1;
    int pair = offset / 2;
    if (pair >= NI_PAIR_COUNT) return -1;
    return pair;
}

// Copy the full overlay data (text+data) from the extended ROM to the
// segment region in RDRAM. The extended ROM is mapped at rdram+0x10000000
// and contains each overlay's text+data at the original compiler layout.
// The segment region (rdram+0x8F000000 for 0x0F, rdram+0x8E000000 for 0x0E)
// is what MEM_W(0x0F00XXXX) resolves to in recompiled code.
static void copy_overlay_data_to_segment(uint8_t* rdram, int pair_index, uint32_t vram) {
    if (pair_index < 0 || pair_index >= NI_OVL_COUNT) return;

    const NiOvlData& data = ni_ovl_data[pair_index];
    // Extended ROM is mapped at rdram + 0x10000000 + rom_offset
    uint8_t* src = rdram + 0x10000000 + data.rom_offset;
    // Segment region: MEM_W(vram) → rdram + vram + 0x80000000
    uint8_t* dst = rdram + (uint64_t)vram + 0x80000000ULL;
    uint32_t size = data.full_size;

    // Ensure the segment region is writable (may extend beyond TLB-mapped pages)
    uintptr_t page_mask = sysconf(_SC_PAGESIZE) - 1;
    uint8_t* aligned = (uint8_t*)((uintptr_t)dst & ~page_mask);
    size_t aligned_size = ((dst + size) - aligned + page_mask) & ~page_mask;
    mprotect(aligned, aligned_size, PROT_READ | PROT_WRITE);

    memcpy(dst, src, size);
}

static void load_ni_overlay(uint8_t* rdram, int pair_index) {
    int overlay_id = NI_OVERLAY_ID_START + pair_index;
    bool is_0e = is_0e_pair(pair_index);
    uint32_t vram = is_0e ? 0x0E000000 : 0x0F000000;
    int& loaded_pair = is_0e ? loaded_0e_pair : loaded_0f_pair;

    if (pair_index == loaded_pair) return;

    if (loaded_pair >= 0) {
        unload_overlay_by_id(NI_OVERLAY_ID_START + loaded_pair);
    }

    load_overlay_by_id(overlay_id, vram);
    // Copy full overlay data (text+data) from extended ROM to segment region
    copy_overlay_data_to_segment(rdram, pair_index, vram);
    loaded_pair = pair_index;

#if LOD_FIX_NI_PAIR120_RESULT_LABELS
    if (pair_index == 120) {
        lod_install_ni_pair120_dispatch_fix("pair120-load");
    }
#endif
#if LOD_ENABLE_SAVE_PAIR120_TRACE
    if (pair_index == 120) {
        lod_install_ni_pair120_trace_wrapper("pair120-load");
    }
#endif

    static int load_count = 0;
    load_count++;
    if (load_count <= 30 || (load_count % 100) == 0) {
        fprintf(stderr, "[ni_ovl] #%d pair %d → 0x%08X (overlay_id=%d rom=0x%08X data=0x%X)\n",
                load_count, pair_index, vram, overlay_id,
                ni_ovl_data[pair_index].rom_offset, ni_ovl_data[pair_index].full_size);
    }
}

// Fingerprint table for overlay identification
#include "ni_ovl_fingerprints.h"

// Match decompressed data at paddr against known overlay fingerprints.
// The game decompresses NI files to its own heap in low RDRAM, so we
// read the first 128 bytes and compare against precomputed signatures.
static int match_overlay_fingerprint(uint8_t* rdram, uint32_t paddr) {
    if (paddr + 128 > 0x00800000) return -1;  // sanity: within 8MB RDRAM

    // Read 32 big-endian words from the decompressed data.
    // The data in RDRAM is byte-swapped (LE), so we read as LE uint32
    // and the fingerprints are stored as BE. We need to match what the
    // game sees, which is the byte-swapped version in RDRAM.
    // Actually: RDRAM stores data in the host's native byte order after
    // the byte-swap in lod_init. The fingerprints are from the raw BE
    // decompressed data. We need to byte-swap when comparing.
    const uint32_t* data = (const uint32_t*)(rdram + paddr);

    for (int i = 0; i < NI_FP_COUNT; i++) {
        bool match = true;
        for (int j = 0; j < NI_FP_WORDS; j++) {
            // RDRAM stores decompressed data as-is (BE words read as uint32)
            uint32_t rdram_word = data[j];
            if (rdram_word != ni_fingerprints[i].words[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return ni_fingerprints[i].pair_index;
        }
    }
    return -1;
}

// RT64 segment overrides for TLB-mapped segments
extern "C" uint32_t g_tlb_segment_0e;
extern "C" uint32_t g_tlb_segment_0f;

// Called from osMapTLB hook when vaddr is 0x0E000000 or 0x0F000000
extern "C" void ni_overlay_on_tlb_map(uint8_t* rdram, uint32_t vaddr,
                                       uint32_t even_paddr, uint32_t odd_paddr) {
    if (vaddr != 0x0F000000 && vaddr != 0x0E000000) return;

    // Set RT64 segment override to the MEM_W region (0x8F/0x8E000000).
    // osVirtualToPhysical passes 0x0F/0x0E addresses through raw, so the DL
    // has 0x0F-style addresses. RT64 resolves via this override.
    if (vaddr == 0x0F000000) g_tlb_segment_0f = 0x8F000000;
    else                     g_tlb_segment_0e = 0x8E000000;

    int pair = match_overlay_fingerprint(rdram, even_paddr);
    if (pair >= 0) {
        load_ni_overlay(rdram, pair);
        return;
    }

    static int miss_count = 0;
    miss_count++;
    if (miss_count <= 10) {
        fprintf(stderr, "[ni_ovl] WARNING: TLB map 0x%08X paddr=0x%08X — no matching overlay\n",
                vaddr, even_paddr);
        // Debug: dump first 32 bytes at paddr in both byte orders
        if (even_paddr + 32 <= 0x00800000) {
            const uint32_t* p = (const uint32_t*)(rdram + even_paddr);
            fprintf(stderr, "[ni_ovl]   LE words: %08X %08X %08X %08X\n", p[0], p[1], p[2], p[3]);
            fprintf(stderr, "[ni_ovl]   BE words: %08X %08X %08X %08X\n",
                    __builtin_bswap32(p[0]), __builtin_bswap32(p[1]),
                    __builtin_bswap32(p[2]), __builtin_bswap32(p[3]));
            // Show expected first words from overlay 0
            fprintf(stderr, "[ni_ovl]   Expected (ovl 0): %08X %08X %08X %08X\n",
                    ni_fingerprints[0].words[0], ni_fingerprints[0].words[1],
                    ni_fingerprints[0].words[2], ni_fingerprints[0].words[3]);
        }
    }
}
