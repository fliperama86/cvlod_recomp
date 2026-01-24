/**
 * CVLoD Recomp - Memory Access Wrapper
 *
 * Provides memory access functions that handle both RDRAM and hardware registers.
 * This wraps the standard recomp.h memory access to add hardware emulation.
 */

#ifndef CVLOD_MEMORY_H
#define CVLOD_MEMORY_H

#include <cstdint>
#include "hardware.h"

// RDRAM base address (sign-extended)
constexpr uint64_t RDRAM_BASE = 0xFFFFFFFF80000000ULL;
constexpr uint64_t RDRAM_SIZE_MASK = 0x7FFFFF; // 8MB

// External RDRAM pointer and ROM data
extern uint8_t* g_rdram;
extern uint8_t* g_rom;
extern size_t g_rom_size;

// Safe memory read - handles hardware addresses
inline int32_t safe_mem_w(uint8_t* rdram, uint64_t addr) {
    if (is_hw_address(addr)) {
        return (int32_t)hw_read32(addr);
    }
    // Handle uncached RDRAM (KSEG1: 0xA0000000-0xA3FFFFFF)
    if (is_uncached_rdram(addr)) {
        uint64_t offset = (addr - 0xFFFFFFFFA0000000ULL) & RDRAM_SIZE_MASK;
        return *(int32_t*)(rdram + offset);
    }
    // Standard RDRAM access (KSEG0: 0x80000000-0x83FFFFFF)
    uint64_t offset = (addr - RDRAM_BASE) & RDRAM_SIZE_MASK;
    return *(int32_t*)(rdram + offset);
}

// Safe memory write - handles hardware addresses
inline void safe_mem_w_write(uint8_t* rdram, uint64_t addr, int32_t value) {
    if (is_hw_address(addr)) {
        hw_write32(addr, (uint32_t)value);
        return;
    }
    // Handle uncached RDRAM
    if (is_uncached_rdram(addr)) {
        uint64_t offset = (addr - 0xFFFFFFFFA0000000ULL) & RDRAM_SIZE_MASK;
        *(int32_t*)(rdram + offset) = value;
        return;
    }
    // Standard RDRAM access
    uint64_t offset = (addr - RDRAM_BASE) & RDRAM_SIZE_MASK;
    *(int32_t*)(rdram + offset) = value;
}

// Helper to check if address will cause issues
inline bool is_valid_rdram_addr(uint64_t addr) {
    // Check KSEG0 range
    if (addr >= RDRAM_BASE && addr < RDRAM_BASE + 0x800000) return true;
    // Check KSEG1 range
    if (addr >= 0xFFFFFFFFA0000000ULL && addr < 0xFFFFFFFFA0800000ULL) return true;
    return false;
}

#endif // CVLOD_MEMORY_H
