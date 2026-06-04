/**
 * CVLoD Recomp - Hardware Register Emulation
 *
 * Stub implementations for N64 hardware registers.
 */

#include "hardware.h"
#include <cstdio>
#include <cstring>

HardwareState g_hw;

void hw_init() {
  memset(&g_hw, 0, sizeof(g_hw));

  // RSP starts halted
  g_hw.rsp_status = 0x1; // SP_STATUS_HALT

  // MI version (RCP 2.0)
  g_hw.mi_mode = 0;
  g_hw.mi_intr = 0;
  g_hw.mi_intr_mask = 0;

  // VI defaults
  g_hw.vi_status = 0;
  g_hw.vi_v_current = 0;
  g_hw.vi_v_intr = 0x3FF; // Default VBlank
  g_hw.vi_width = 320;
  g_hw.vi_v_sync = 525;

  // DPC defaults
  g_hw.dpc_status = 0x80; // PIPE_READY

  // PI idle
  g_hw.pi_status = 0;

  g_hw.frame_count = 0;
  g_hw.trace_hw_access = true; // Enable tracing for debugging
}

static const char *get_hw_region_name(uint64_t addr) {
  uint64_t base = addr & 0xFFFFFFFFFFFF0000ULL;
  if (base == 0xFFFFFFFFA4000000ULL)
    return "RSP_DMEM";
  if (base == 0xFFFFFFFFA4001000ULL)
    return "RSP_IMEM";
  if ((addr & 0xFFFFFFFFFFF80000ULL) == 0xFFFFFFFFA4040000ULL)
    return "RSP_REGS";
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4100000ULL)
    return "RDP_REGS";
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4300000ULL)
    return "MI_REGS";
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4400000ULL)
    return "VI_REGS";
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4500000ULL)
    return "AI_REGS";
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4600000ULL)
    return "PI_REGS";
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4700000ULL)
    return "RI_REGS";
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4800000ULL)
    return "SI_REGS";
  if ((addr & 0xFFFFFFFFF0000000ULL) == 0xFFFFFFFFB0000000ULL)
    return "CART_ROM";
  return "UNKNOWN";
}

// RDP Registers (offset from 0xA4100000)
constexpr uint32_t DPC_START = 0x00;
constexpr uint32_t DPC_END = 0x04;
constexpr uint32_t DPC_CURRENT = 0x08;
constexpr uint32_t DPC_STATUS = 0x0C;
constexpr uint32_t DPC_CLOCK = 0x10;
constexpr uint32_t DPC_BUFBUSY = 0x14;
constexpr uint32_t DPC_PIPEBUSY = 0x18;
constexpr uint32_t DPC_TMEM = 0x1C;

uint32_t hw_read32(uint64_t addr) {
  uint32_t value = 0;
  uint32_t offset = addr & 0xFFFF;

  // Identify region and handle read
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4300000ULL) {
    // MI registers
    switch (offset) {
    case MI_MODE:
      value = g_hw.mi_mode;
      break;
    case MI_VERSION:
      value = 0x02020102;
      break; // RCP 2.0
    case MI_INTR:
      value = g_hw.mi_intr;
      break;
    case MI_INTR_MASK:
      value = g_hw.mi_intr_mask;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4400000ULL) {
    // VI registers
    switch (offset) {
    case VI_STATUS:
      value = g_hw.vi_status;
      break;
    case VI_ORIGIN:
      value = g_hw.vi_origin;
      break;
    case VI_WIDTH:
      value = g_hw.vi_width;
      break;
    case VI_V_INTR:
      value = g_hw.vi_v_intr;
      break;
    case VI_V_CURRENT:
      // Simulate scanline progression
      g_hw.vi_v_current = (g_hw.vi_v_current + 1) % 525;
      value = g_hw.vi_v_current;
      break;
    case VI_BURST:
      value = g_hw.vi_burst;
      break;
    case VI_V_SYNC:
      value = g_hw.vi_v_sync;
      break;
    case VI_H_SYNC:
      value = g_hw.vi_h_sync;
      break;
    case VI_LEAP:
      value = g_hw.vi_leap;
      break;
    case VI_H_START:
      value = g_hw.vi_h_start;
      break;
    case VI_V_START:
      value = g_hw.vi_v_start;
      break;
    case VI_V_BURST:
      value = g_hw.vi_v_burst;
      break;
    case VI_X_SCALE:
      value = g_hw.vi_x_scale;
      break;
    case VI_Y_SCALE:
      value = g_hw.vi_y_scale;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4100000ULL) {
    // RDP (DPC) registers
    switch (offset) {
    case DPC_START:
      value = g_hw.dpc_start;
      break;
    case DPC_END:
      value = g_hw.dpc_end;
      break;
    case DPC_CURRENT:
      value = g_hw.dpc_current;
      break;
    case DPC_STATUS:
      value = g_hw.dpc_status;
      break;
    case DPC_CLOCK:
      value = g_hw.dpc_clock;
      break;
    case DPC_BUFBUSY:
      value = g_hw.dpc_bufbusy;
      break;
    case DPC_PIPEBUSY:
      value = g_hw.dpc_pipebusy;
      break;
    case DPC_TMEM:
      value = g_hw.dpc_tmem;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF80000ULL) == 0xFFFFFFFFA4040000ULL) {
    // RSP registers
    switch (offset) {
    case RSP_SP_STATUS:
      value = g_hw.rsp_status;
      break;
    case RSP_SP_DMA_BUSY:
      value = 0;
      break; // Not busy
    case RSP_SP_DMA_FULL:
      value = 0;
      break; // Not full
    case RSP_SP_SEMAPHORE:
      value = g_hw.rsp_semaphore;
      g_hw.rsp_semaphore = 1;
      break;
    case RSP_SP_PC:
      value = g_hw.rsp_pc;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4600000ULL) {
    // PI registers
    switch (offset) {
    case PI_STATUS:
      // Return not busy, not error
      value = 0;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4100000ULL) {
    // RDP registers - return ready status
    value = 0;
  }

  if (g_hw.trace_hw_access) {
    printf("[HW READ ] %s: 0x%08X -> 0x%08X\n", get_hw_region_name(addr),
           (uint32_t)(addr & 0xFFFFFFFF), value);
  }

  return value;
}

void hw_write32(uint64_t addr, uint32_t value) {
  uint32_t offset = addr & 0xFFFF;

  if (g_hw.trace_hw_access) {
    printf("[HW WRITE] %s: 0x%08X <- 0x%08X\n", get_hw_region_name(addr),
           (uint32_t)(addr & 0xFFFFFFFF), value);
  }

  // Identify region and handle write
  if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4300000ULL) {
    // MI registers
    switch (offset) {
    case MI_MODE:
      g_hw.mi_mode = value;
      break;
    case MI_INTR_MASK:
      g_hw.mi_intr_mask = value;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4400000ULL) {
    // VI registers
    switch (offset) {
    case VI_STATUS:
      g_hw.vi_status = value;
      break;
    case VI_ORIGIN:
      g_hw.vi_origin = value;
      break;
    case VI_WIDTH:
      g_hw.vi_width = value;
      break;
    case VI_V_INTR:
      g_hw.vi_v_intr = value;
      break;
    case VI_V_CURRENT:
      g_hw.vi_v_current = 0;
      break; // Writing clears
    case VI_BURST:
      g_hw.vi_burst = value;
      break;
    case VI_V_SYNC:
      g_hw.vi_v_sync = value;
      break;
    case VI_H_SYNC:
      g_hw.vi_h_sync = value;
      break;
    case VI_LEAP:
      g_hw.vi_leap = value;
      break;
    case VI_H_START:
      g_hw.vi_h_start = value;
      break;
    case VI_V_START:
      g_hw.vi_v_start = value;
      break;
    case VI_V_BURST:
      g_hw.vi_v_burst = value;
      break;
    case VI_X_SCALE:
      g_hw.vi_x_scale = value;
      break;
    case VI_Y_SCALE:
      g_hw.vi_y_scale = value;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4100000ULL) {
    // RDP (DPC) registers
    switch (offset) {
    case DPC_START:
      g_hw.dpc_start = value;
      break;
    case DPC_END:
      g_hw.dpc_end = value;
      break;
    case DPC_CURRENT:
      g_hw.dpc_current = value;
      break;
    case DPC_STATUS:
      g_hw.dpc_status = value;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF80000ULL) == 0xFFFFFFFFA4040000ULL) {
    // RSP registers
    switch (offset) {
    case RSP_SP_STATUS:
      // Handle status write bits
      if (value & 0x01)
        g_hw.rsp_status &= ~0x01; // Clear halt
      if (value & 0x02)
        g_hw.rsp_status |= 0x01; // Set halt
      if (value & 0x04)
        g_hw.rsp_status &= ~0x02; // Clear broke
      // ... more bits could be handled
      break;
    case RSP_SP_SEMAPHORE:
      g_hw.rsp_semaphore = 0;
      break;
    default:
      break;
    }
  } else if ((addr & 0xFFFFFFFFFFF00000ULL) == 0xFFFFFFFFA4600000ULL) {
    // PI registers
    switch (offset) {
    case PI_DRAM_ADDR:
      g_hw.pi_dram_addr = value;
      break;
    case PI_CART_ADDR:
      g_hw.pi_cart_addr = value;
      break;
    case PI_STATUS:
      // Writing to status clears interrupt
      g_hw.pi_status = 0;
      break;
    case PI_WR_LEN:
      // Initiate DMA from cart to RDRAM
      // This would trigger actual ROM->RDRAM copy
      printf("[PI DMA] Cart 0x%08X -> RDRAM 0x%08X, len %u\n",
             g_hw.pi_cart_addr, g_hw.pi_dram_addr, value + 1);
      break;
    case PI_RD_LEN:
      // Initiate DMA from RDRAM to cart (save)
      printf("[PI DMA] RDRAM 0x%08X -> Cart 0x%08X, len %u\n",
             g_hw.pi_dram_addr, g_hw.pi_cart_addr, value + 1);
      break;
    default:
      break;
    }
  }
}
