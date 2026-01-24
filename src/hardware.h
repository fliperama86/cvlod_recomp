/**
 * CVLoD Recomp - Hardware Register Emulation
 *
 * Provides stub implementations for N64 hardware registers.
 * These are memory-mapped I/O addresses that the game accesses.
 */

#ifndef CVLOD_HARDWARE_H
#define CVLOD_HARDWARE_H

#include <cstdint>
#include <cstdio>

// N64 Memory Map (relevant regions)
// 0x00000000 - 0x03FFFFFF: RDRAM (physical)
// 0x80000000 - 0x803FFFFF: RDRAM via KSEG0 (cached)
// 0xA0000000 - 0xA03FFFFF: RDRAM via KSEG1 (uncached)
// 0xA4000000 - 0xA400FFFF: RSP DMEM/IMEM
// 0xA4040000 - 0xA404FFFF: RSP registers
// 0xA4100000 - 0xA41FFFFF: RDP command registers
// 0xA4300000 - 0xA43FFFFF: MI (MIPS Interface)
// 0xA4400000 - 0xA44FFFFF: VI (Video Interface)
// 0xA4500000 - 0xA45FFFFF: AI (Audio Interface)
// 0xA4600000 - 0xA46FFFFF: PI (Peripheral Interface)
// 0xA4700000 - 0xA47FFFFF: RI (RDRAM Interface)
// 0xA4800000 - 0xA48FFFFF: SI (Serial Interface)
// 0xB0000000 - 0xBFFFFFFF: Cartridge ROM

// Hardware register base addresses (sign-extended for 64-bit)
constexpr uint64_t HW_RSP_DMEM    = 0xFFFFFFFFA4000000ULL;
constexpr uint64_t HW_RSP_IMEM    = 0xFFFFFFFFA4001000ULL;
constexpr uint64_t HW_RSP_REGS    = 0xFFFFFFFFA4040000ULL;
constexpr uint64_t HW_RDP_REGS    = 0xFFFFFFFFA4100000ULL;
constexpr uint64_t HW_MI_REGS     = 0xFFFFFFFFA4300000ULL;
constexpr uint64_t HW_VI_REGS     = 0xFFFFFFFFA4400000ULL;
constexpr uint64_t HW_AI_REGS     = 0xFFFFFFFFA4500000ULL;
constexpr uint64_t HW_PI_REGS     = 0xFFFFFFFFA4600000ULL;
constexpr uint64_t HW_RI_REGS     = 0xFFFFFFFFA4700000ULL;
constexpr uint64_t HW_SI_REGS     = 0xFFFFFFFFA4800000ULL;
constexpr uint64_t HW_CART_ROM    = 0xFFFFFFFFB0000000ULL;

// Hardware register space allocation
constexpr size_t HW_REGS_SIZE = 0x100000; // 1MB for all HW registers

// RSP Registers (offset from 0xA4040000)
constexpr uint32_t RSP_SP_MEM_ADDR    = 0x00;
constexpr uint32_t RSP_SP_DRAM_ADDR   = 0x04;
constexpr uint32_t RSP_SP_RD_LEN      = 0x08;
constexpr uint32_t RSP_SP_WR_LEN      = 0x0C;
constexpr uint32_t RSP_SP_STATUS      = 0x10;
constexpr uint32_t RSP_SP_DMA_FULL    = 0x14;
constexpr uint32_t RSP_SP_DMA_BUSY    = 0x18;
constexpr uint32_t RSP_SP_SEMAPHORE   = 0x1C;
constexpr uint32_t RSP_SP_PC          = 0x80; // +0x80000

// MI Registers (offset from 0xA4300000)
constexpr uint32_t MI_MODE         = 0x00;
constexpr uint32_t MI_VERSION      = 0x04;
constexpr uint32_t MI_INTR         = 0x08;
constexpr uint32_t MI_INTR_MASK    = 0x0C;

// VI Registers (offset from 0xA4400000)
constexpr uint32_t VI_STATUS       = 0x00;
constexpr uint32_t VI_ORIGIN       = 0x04;
constexpr uint32_t VI_WIDTH        = 0x08;
constexpr uint32_t VI_V_INTR       = 0x0C;
constexpr uint32_t VI_V_CURRENT    = 0x10;
constexpr uint32_t VI_BURST        = 0x14;
constexpr uint32_t VI_V_SYNC       = 0x18;
constexpr uint32_t VI_H_SYNC       = 0x1C;
constexpr uint32_t VI_LEAP         = 0x20;
constexpr uint32_t VI_H_START      = 0x24;
constexpr uint32_t VI_V_START      = 0x28;
constexpr uint32_t VI_V_BURST      = 0x2C;
constexpr uint32_t VI_X_SCALE      = 0x30;
constexpr uint32_t VI_Y_SCALE      = 0x34;

// PI Registers (offset from 0xA4600000)
constexpr uint32_t PI_DRAM_ADDR    = 0x00;
constexpr uint32_t PI_CART_ADDR    = 0x04;
constexpr uint32_t PI_RD_LEN       = 0x08;
constexpr uint32_t PI_WR_LEN       = 0x0C;
constexpr uint32_t PI_STATUS       = 0x10;

// Hardware state
struct HardwareState {
    // RSP
    uint32_t rsp_status;
    uint32_t rsp_pc;
    uint32_t rsp_semaphore;

    // MI
    uint32_t mi_mode;
    uint32_t mi_intr;
    uint32_t mi_intr_mask;

    // VI
    uint32_t vi_status;
    uint32_t vi_origin;
    uint32_t vi_width;
    uint32_t vi_v_current;
    uint32_t vi_v_intr;

    // PI
    uint32_t pi_status;
    uint32_t pi_dram_addr;
    uint32_t pi_cart_addr;

    // Counters for simulation
    uint32_t frame_count;

    // Debug
    bool trace_hw_access;
};

extern HardwareState g_hw;

// Initialize hardware state
void hw_init();

// Hardware register access functions
uint32_t hw_read32(uint64_t addr);
void hw_write32(uint64_t addr, uint32_t value);

// Check if address is in hardware register range
inline bool is_hw_address(uint64_t addr) {
    // Check for 0xA4xxxxxx range (sign-extended)
    uint64_t masked = addr & 0xFFFFFFFFFF000000ULL;
    return (masked >= 0xFFFFFFFFA4000000ULL && masked <= 0xFFFFFFFFA4800000ULL) ||
           (masked >= 0xFFFFFFFFB0000000ULL); // Cartridge ROM
}

// Check if address is uncached RDRAM (KSEG1)
inline bool is_uncached_rdram(uint64_t addr) {
    uint64_t masked = addr & 0xFFFFFFFFFF000000ULL;
    return (masked >= 0xFFFFFFFFA0000000ULL && masked < 0xFFFFFFFFA4000000ULL);
}

#endif // CVLOD_HARDWARE_H
