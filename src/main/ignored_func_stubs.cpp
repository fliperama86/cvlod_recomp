// Stubs for N64 OS functions that are in N64Recomp's ignored_funcs list.
// The recompiler doesn't generate code for these, but other recompiled OS
// internal functions still reference them via _recomp suffix.
// These are all handled natively by ultramodern, so empty stubs are safe.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include "ultramodern/ultramodern.hpp"
#include "ultramodern/ultra64.h"

#include "recomp.h"

extern "C" {

void __osDequeueThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osEnqueueThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osDispatchThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osViSwapContext_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSpDeviceBusy_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSetCompare_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __ll_rshift_recomp(uint8_t* rdram, recomp_context* ctx) {}

// Batch 2: additional ignored functions called by recompiled code
void __osSiGetAccess_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSiRelAccess_recomp(uint8_t* rdram, recomp_context* ctx) {}
} // end extern "C"

// Input callback — defined in main.cpp (C++ linkage)
bool get_n64_input(int controller_num, uint16_t* buttons, float* x, float* y);

extern "C" {

void __osSiRawStartDma_recomp(uint8_t* rdram, recomp_context* ctx) {
    // s32 __osSiRawStartDma(s32 direction, void *dramAddr)
    // direction: 0 = OS_READ (PIF→DRAM), 1 = OS_WRITE (DRAM→PIF)
    int32_t direction = (int32_t)ctx->r4;
    uint32_t dram_addr = (uint32_t)ctx->r5;

    static int si_dma_count = 0; si_dma_count++;
    if (si_dma_count <= 10 || si_dma_count % 500 == 0) {
        fprintf(stderr, "[SI_DMA] #%d dir=%d addr=0x%08X\n", si_dma_count, direction, dram_addr);
    }

    // On WRITE: if the PIF buffer is empty (no controller commands), format it
    // ourselves with a standard button read command for controller 0.
    // MEM_B-compatible byte access: rdram[(addr + offset) ^ 3]
    #define RD_MEM_B(base, offset) (base)[(offset) ^ 3]
    #define WR_MEM_B(base, offset, val) (base)[(offset) ^ 3] = (uint8_t)(val)

    if (direction == 1 && dram_addr != 0) {
        uint8_t* pif = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];
        if (RD_MEM_B(pif, 0) == 0x00 || RD_MEM_B(pif, 3) == 0xFE) {
            // Format a PIF button read command for controller 0
            WR_MEM_B(pif, 0, 0x01); // TX count
            WR_MEM_B(pif, 1, 0x04); // RX count
            WR_MEM_B(pif, 2, 0x01); // command: read buttons
            WR_MEM_B(pif, 3, 0x00); // buttons_hi (filled on READ)
            WR_MEM_B(pif, 4, 0x00); // buttons_lo
            WR_MEM_B(pif, 5, 0x00); // stick_x
            WR_MEM_B(pif, 6, 0x00); // stick_y
            WR_MEM_B(pif, 7, 0xFE); // end
        }
    }

    if (direction == 0 && dram_addr != 0) {
        // PIF READ: populate response buffer with controller data.
        // contpak_parsePIF reads 8 bytes per controller via lwl/lwr (MEM_W),
        // then extracts buttons from bytes 4-5, stick from 6-7.
        uint16_t buttons = 0;
        float x = 0.0f, y = 0.0f;
        get_n64_input(0, &buttons, &x, &y);

        // Ensure max_controllers is set (game reads from RDRAM, not C++ variable)
        if (rdram[0x13EDB1 ^ 3] == 0) {
            rdram[0x13EDB1 ^ 3] = 1;
        }

        // Write PIF response using MEM_B-compatible byte order (XOR 3).
        uint8_t* pif = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];
        WR_MEM_B(pif, 0, 0x01);  // TX count
        WR_MEM_B(pif, 1, 0x04);  // RX count
        WR_MEM_B(pif, 2, 0x00);  // status (no error)
        WR_MEM_B(pif, 3, 0x01);  // command
        WR_MEM_B(pif, 4, (buttons >> 8) & 0xFF);
        WR_MEM_B(pif, 5, buttons & 0xFF);
        WR_MEM_B(pif, 6, (int8_t)(x * 127));
        WR_MEM_B(pif, 7, (int8_t)(y * 127));
    }

    // Send SI completion message so the game doesn't hang waiting for SI DMA.
    // Guard: only send if the SI event is registered (mq != 0).
    // Early boot calls __osSiRawStartDma before osSetEventMesg(SI).
    if (ultramodern::is_game_started()) {
        ultramodern::send_si_message();
    }
    ctx->r2 = 0; // return success
}
void __osContGetInitData_recomp(uint8_t* rdram, recomp_context* ctx) {
    // void __osContGetInitData(u8* bitpattern, OSContStatus* data)
    // bitpattern = $a0, data = $a1
    // Populate controller detection data: report controller 0 as connected.
    uint32_t pattern_addr = (uint32_t)ctx->r4 & 0x3FFFFF;
    uint32_t data_addr = (uint32_t)ctx->r5 & 0x3FFFFF;

    if (pattern_addr < 0x800000) {
        rdram[pattern_addr] = 0x01; // bit 0 = controller 0 connected
    }
    if (data_addr < 0x800000) {
        // OSContStatus: { u16 type, u8 status, u8 err_no }
        *(uint16_t*)(rdram + data_addr) = 0x0005; // CONT_TYPE_NORMAL
        rdram[data_addr + 2] = 0x00;
        rdram[data_addr + 3] = 0x00; // success
        for (int c = 1; c < 4; c++) {
            *(uint16_t*)(rdram + data_addr + c * 4) = 0;
            rdram[data_addr + c * 4 + 2] = 0;
            rdram[data_addr + c * 4 + 3] = 0x08; // no response
        }
    }

    // CRITICAL: Also set the game's internal max_controllers at 0x8013EDB1.
    // The PIF command formatter (func_80098C60) and response parser (func_80098BD4)
    // both read this byte. Without it, they skip all controller data.
    // 0x80140000 - 0x124F = 0x8013EDB1
    rdram[0x0013EDB1] = 1; // 1 controller connected

    fprintf(stderr, "[CONT] __osContGetInitData: pattern=0x%02X, max_controllers=1 at 0x0013EDB1\n",
            pattern_addr < 0x800000 ? rdram[pattern_addr] : 0);
}
void __osPackRequestData_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPiRawReadIo_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPiRawWriteIo_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPfsCheckRamArea_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __f_to_ull_recomp(uint8_t* rdram, recomp_context* ctx) {}
void osPiGetCmdQueue_recomp(uint8_t* rdram, recomp_context* ctx) {}

}
