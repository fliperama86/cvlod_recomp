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
#include <unistd.h>
#include <sys/mman.h>

#include "recomp.h"

// Track TLB entries so we can re-copy when mappings change
struct TlbEntry {
    uint32_t vaddr;
    uint32_t even_paddr;
    uint32_t odd_paddr;
    uint32_t page_size;
    bool valid;
};

static TlbEntry tlb_table[32] = {};

// Override the weak RECOMP_FUNC stub in funcs_40.c
// osMapTLB(index, page_mask, vaddr, even_paddr, odd_paddr, asid)
extern "C" void func_80097730(uint8_t* rdram, recomp_context* ctx) {
    int32_t  index      = (int32_t)ctx->r4;
    uint32_t page_mask  = (uint32_t)ctx->r5;
    uint32_t vaddr      = (uint32_t)ctx->r6;
    uint32_t even_paddr = (uint32_t)ctx->r7;
    uint32_t odd_paddr  = MEM_W(0x10, ctx->r29);

    // Handle osUnmapTLB (index = -1 or vaddr = 0xFFFFFFFF)
    if (index < 0 || index >= 32) {
        return;
    }

    uint32_t mask_field = (page_mask >> 13) & 0xFFF;
    uint32_t page_size  = (mask_field + 1) * 4096;
    uint32_t entry_size = page_size * 2;

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
    if (even_paddr != 0xFFFFFFFF && even_paddr < 0x00800000) {
        memcpy(target, rdram + even_paddr, page_size);
    }
    if (odd_paddr != 0xFFFFFFFF && odd_paddr < 0x00800000) {
        memcpy(target + page_size, rdram + odd_paddr, page_size);
    }

    // Save the mapping for debugging
    tlb_table[index] = { vaddr, even_paddr, odd_paddr, page_size, true };

    static int log_count = 0;
    log_count++;
    if (log_count <= 30 || (log_count % 200) == 0) {
        fprintf(stderr, "[osMapTLB] #%d idx=%d vaddr=0x%08X even=0x%08X odd=0x%08X page=0x%X\n",
                log_count, index, vaddr, even_paddr, odd_paddr, page_size);
    }
}

// osUnmapTLB
extern "C" void func_800977D0(uint8_t* rdram, recomp_context* ctx) {
    int32_t index = (int32_t)ctx->r4;
    if (index >= 0 && index < 32) {
        tlb_table[index].valid = false;
    }
}
