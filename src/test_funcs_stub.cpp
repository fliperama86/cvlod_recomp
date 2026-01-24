/**
 * CVLoD Recomp - Stub Function Table for Testing
 *
 * Provides a minimal function table implementation that
 * doesn't require linking with the full recompiled library.
 */

#include <cstdint>
#include <unordered_map>
#include "recomp.h"

// Stub function that does nothing
static void stub_func(uint8_t* rdram, recomp_context* ctx) {
    // Empty stub
}

// Function lookup table - just count entries, don't actually call
static std::unordered_map<int32_t, recomp_func_t*> g_function_table;

// List of known function addresses (parsed from funcs.h)
static const uint32_t known_addresses[] = {
    // Sample of addresses from the actual function table
    0x80000460, 0x8000053C, 0x80000578, 0x8000067C,
    0x80000770, 0x800007B0, 0x80000864, 0x80000A30,
    0x80000A98, 0x80000BD0, 0x80000F30, 0x80001090,
    0x800013E8, 0x80001438, 0x80001454, 0x800014B4,
    0x800014FC, 0x80001508, 0x80001514, 0x8000163C,
    0x80001680, 0x80001804, 0x80001858, 0x80001918,
    // ... many more would be here in full version
    0x80005000, 0x80010000, 0x80020000, 0x80030000,
    0x80040000, 0x80050000, 0x80060000, 0x80070000,
    0x80080000, 0x80090000, 0x800A0000, 0x800B0000,
    // Common section samples
    0x80128900, 0x80130000, 0x80140000, 0x80150000,
    0x80160000, 0x80170000,
};

void init_function_table() {
    g_function_table.clear();

    // Register all known addresses with a stub
    for (uint32_t addr : known_addresses) {
        g_function_table[(int32_t)addr] = stub_func;
    }

    // Also read the actual count from funcs.h if we want accurate numbers
    // For now, report what we have
    printf("  [Stub table] Registered %zu test addresses\n", g_function_table.size());
}

recomp_func_t* cvlod_get_function(int32_t vram) {
    auto it = g_function_table.find(vram);
    if (it != g_function_table.end()) {
        return it->second;
    }
    return nullptr;
}
