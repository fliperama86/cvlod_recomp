// NI overlay host hooks for standalone RT64 test harnesses.
//
// RT64's LoD display-list resolver (lib/rt64/src/hle/lod_ni_dl_resolver.cpp)
// calls back into the game's NI overlay loader. The harnesses link rt64
// without the game, so they provide these hooks reporting that no NI overlay
// is loaded.

#include <cstdint>

extern "C" uint32_t ni_overlay_loaded_span(uint32_t vram) {
    (void)vram;
    return 0;
}

extern "C" int lod_ni_overlay_loaded_0e_pair() {
    return -1;
}

extern "C" int lod_ni_overlay_loaded_0f_pair() {
    return -1;
}

extern "C" const void* lod_ni_stale_dl_candidate(uint8_t* rdram, uint32_t segmented_address,
                                                  uint32_t min_size, uint32_t attempt,
                                                  int* pair_out, uint32_t* source_out) {
    (void)rdram;
    (void)segmented_address;
    (void)min_size;
    (void)attempt;
    (void)pair_out;
    (void)source_out;
    return nullptr;
}
