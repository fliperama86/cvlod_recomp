// NI Overlay Runtime Loader
//
// When the game loads an NI code overlay to vaddr 0x0F000000 via osMapTLB,
// this hooks into the TLB handler to load the correct recompiled section.
//
// The game DMAs the NI overlay data to a physical address, then calls
// osMapTLB(idx, mask, 0x0F000000, paddr, ...). We use the NI file index
// (derived from the game's overlay_system state) to identify which
// recompiled section to load.

#include <cstdio>
#include <cstdint>
#include <unordered_map>

#include "recomp.h"
#include "librecomp/overlays.hpp"

extern "C" void load_overlay_by_id(uint32_t id, uint32_t ram_addr);
extern "C" void unload_overlay_by_id(uint32_t id);

// NI text index → overlay_sections_by_index ID mapping
// Generated from step7: NI text_ni_index maps to overlay index (position in overlays.txt)
// The overlay_sections_by_index table maps overlay index → section_table index
// We need: NI text index → overlay ID (position in overlays.txt)

// NI pairs: text_ni_index 0x1C5 (453) → pair 0 → overlay index 48 (first NI in overlays.txt)
// So overlay_id = 48 + pair_index
// And NI text indices go: 0x1C5, 0x1C7, 0x1C9, ... (every other index, text entries only)
// pair_index = (ni_text_index - 0x1C5) / 2

static constexpr uint32_t NI_VRAM = 0x0F000000;
static constexpr int NI_TEXT_INDEX_START = 0x1C5;  // First NI text index
static constexpr int NI_OVERLAY_ID_START = 48;      // First NI entry in overlays.txt
static constexpr int NI_PAIR_COUNT = 245;

static int currently_loaded_ni_pair = -1;

// Convert NI file index to pair index
static int ni_index_to_pair(int ni_index) {
    if (ni_index < NI_TEXT_INDEX_START) return -1;
    int offset = ni_index - NI_TEXT_INDEX_START;
    if (offset % 2 != 0) return -1;  // Data entries are odd
    int pair = offset / 2;
    if (pair >= NI_PAIR_COUNT) return -1;
    return pair;
}

// Load an NI overlay by pair index
static void load_ni_overlay(int pair_index) {
    if (pair_index == currently_loaded_ni_pair) return;

    int overlay_id = NI_OVERLAY_ID_START + pair_index;

    // Unload previous NI overlay if any
    if (currently_loaded_ni_pair >= 0) {
        int prev_id = NI_OVERLAY_ID_START + currently_loaded_ni_pair;
        unload_overlay_by_id(prev_id);
    }

    // Load new overlay at 0x0F000000
    load_overlay_by_id(overlay_id, NI_VRAM);
    currently_loaded_ni_pair = pair_index;

    static int load_count = 0;
    load_count++;
    if (load_count <= 20 || (load_count % 100) == 0) {
        fprintf(stderr, "[ni_ovl] #%d loaded pair %d (overlay_id=%d) at 0x%08X\n",
                load_count, pair_index, overlay_id, NI_VRAM);
    }
}

// Called from osMapTLB hook when vaddr == 0x0F000000
// The NI file index must be determined from the game's overlay_system state.
// The overlay_system tracks which NI file is currently being loaded in its
// internal state. We read this from RDRAM.
extern "C" void ni_overlay_on_tlb_map(uint8_t* rdram, uint32_t vaddr,
                                       uint32_t even_paddr, uint32_t odd_paddr) {
    if (vaddr != NI_VRAM) return;

    // The overlay_system stores the current NI file index at specific offsets.
    // Read the current NI file index from the game's overlay_system object.
    // overlay_system base = 0x801D1880 (data section start)
    // The NI index being loaded is tracked in the overlay dispatch pipeline.
    //
    // Strategy: scan the NI decompressed address table to find which NI entry
    // has data at even_paddr. The physical address uniquely identifies the overlay.
    //
    // NI decompressed data lives at rdram + 0x14000000+. When osMapTLB is called
    // with even_paddr, that paddr points into this region.
    //
    // We match paddr to the NI file that was decompressed there during init.

    // Import the decompressed address table from lod_init.cpp
    extern uint32_t ni_decompressed_addrs[];
    extern int ni_decompressed_count;

    // The even_paddr from osMapTLB is a physical N64 address.
    // Our NI data is at rdram + 0x14000000, so paddr = even_paddr.
    // ni_decompressed_addrs[i] = 0x80000000 | rdram_offset
    // So paddr should match (ni_decompressed_addrs[i] & 0x7FFFFFFF)

    uint32_t target_rdram_offset = even_paddr;

    for (int i = 0; i < ni_decompressed_count; i++) {
        if (ni_decompressed_addrs[i] == 0) continue;
        uint32_t entry_rdram_offset = ni_decompressed_addrs[i] & 0x7FFFFFFF;
        if (entry_rdram_offset == target_rdram_offset) {
            int pair = ni_index_to_pair(i);
            if (pair >= 0) {
                load_ni_overlay(pair);
                return;
            }
        }
    }

    // Fallback: couldn't match paddr to NI entry
    static int miss_count = 0;
    miss_count++;
    if (miss_count <= 10) {
        fprintf(stderr, "[ni_ovl] WARNING: TLB map 0x0F000000 paddr=0x%08X - no matching NI entry\n",
                even_paddr);
    }
}
