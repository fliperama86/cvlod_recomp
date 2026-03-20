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

#include "recomp.h"
#include "librecomp/overlays.hpp"

extern "C" void load_overlay_by_id(uint32_t id, uint32_t ram_addr);
extern "C" void unload_overlay_by_id(uint32_t id);

// NI pairs: text_ni_index 0x1C5 (453) → pair 0 → overlay index 48 (first NI in overlays.txt)
// overlay_id = 48 + pair_index
// pair_index = (ni_text_index - 0x1C5) / 2
static constexpr int NI_TEXT_INDEX_START = 0x1C5;
static constexpr int NI_OVERLAY_ID_START = 48;
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

static void load_ni_overlay(int pair_index) {
    int overlay_id = NI_OVERLAY_ID_START + pair_index;
    bool is_0e = is_0e_pair(pair_index);
    uint32_t vram = is_0e ? 0x0E000000 : 0x0F000000;
    int& loaded_pair = is_0e ? loaded_0e_pair : loaded_0f_pair;

    if (pair_index == loaded_pair) return;

    if (loaded_pair >= 0) {
        unload_overlay_by_id(NI_OVERLAY_ID_START + loaded_pair);
    }

    load_overlay_by_id(overlay_id, vram);
    loaded_pair = pair_index;

    static int load_count = 0;
    load_count++;
    if (load_count <= 30 || (load_count % 100) == 0) {
        fprintf(stderr, "[ni_ovl] #%d pair %d → 0x%08X (overlay_id=%d)\n",
                load_count, pair_index, vram, overlay_id);
    }
}

// Called from osMapTLB hook when vaddr is 0x0E000000 or 0x0F000000
extern "C" void ni_overlay_on_tlb_map(uint8_t* rdram, uint32_t vaddr,
                                       uint32_t even_paddr, uint32_t odd_paddr) {
    if (vaddr != 0x0F000000 && vaddr != 0x0E000000) return;

    extern uint32_t ni_decompressed_addrs[];
    extern int ni_decompressed_count;

    uint32_t target = even_paddr;

    for (int i = 0; i < ni_decompressed_count; i++) {
        if (ni_decompressed_addrs[i] == 0) continue;
        uint32_t entry_offset = ni_decompressed_addrs[i] & 0x7FFFFFFF;
        if (entry_offset == target) {
            int pair = ni_index_to_pair(i);
            if (pair >= 0) {
                load_ni_overlay(pair);
                return;
            }
        }
    }

    static int miss_count = 0;
    miss_count++;
    if (miss_count <= 10) {
        fprintf(stderr, "[ni_ovl] WARNING: TLB map 0x%08X paddr=0x%08X — no matching NI entry\n",
                vaddr, even_paddr);
    }
}
