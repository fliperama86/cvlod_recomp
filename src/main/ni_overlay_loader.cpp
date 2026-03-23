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

#include "recomp.h"
#include "librecomp/overlays.hpp"

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

    static int load_count = 0;
    load_count++;
    if (load_count <= 30 || (load_count % 100) == 0) {
        fprintf(stderr, "[ni_ovl] #%d pair %d → 0x%08X (overlay_id=%d, data=0x%X bytes)\n",
                load_count, pair_index, vram, overlay_id, ni_ovl_data[pair_index].full_size);
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

// Called from osMapTLB hook when vaddr is 0x0E000000 or 0x0F000000
extern "C" void ni_overlay_on_tlb_map(uint8_t* rdram, uint32_t vaddr,
                                       uint32_t even_paddr, uint32_t odd_paddr) {
    if (vaddr != 0x0F000000 && vaddr != 0x0E000000) return;

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
