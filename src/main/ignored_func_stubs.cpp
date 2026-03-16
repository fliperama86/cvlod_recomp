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

    if (direction == 0 && dram_addr != 0) {
        // PIF READ: game wants controller data. Populate the DRAM buffer with
        // a PIF response containing our keyboard input.
        //
        // PIF response format for osContStartReadData (4 controllers):
        // Each channel: [TX_LEN | RX_LEN | CMD_BYTE | buttons_hi | buttons_lo | stick_x | stick_y]
        // Followed by padding, total 64 bytes.
        //
        // We only populate controller 0. Others get CONT_NO_RESPONSE_ERROR.
        uint8_t* buf = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];

        // Format a minimal PIF response for a controller read command (0x01)
        // The buffer was previously written with the command by the game (WRITE direction).
        // We just overwrite the response bytes in-place.
        //
        // Standard N64 PIF controller read response layout per channel:
        //   byte 0: TX count (0x01)
        //   byte 1: RX count (0x04)
        //   byte 2: command (0x01 = read buttons)
        //   byte 3: buttons high byte
        //   byte 4: buttons low byte
        //   byte 5: analog stick X (-128 to 127)
        //   byte 6: analog stick Y (-128 to 127)

        uint16_t buttons = 0;
        float x = 0.0f, y = 0.0f;
        get_n64_input(0, &buttons, &x, &y);

        // Find the first controller channel in the PIF buffer.
        // The game's osContStartReadData formats the command buffer with
        // TX=1, RX=4 for each active controller channel.
        for (int offset = 0; offset < 64 - 6; offset++) {
            if (buf[offset] == 0x01 && buf[offset + 1] == 0x04) {
                // Found a controller read channel — inject our data
                buf[offset + 2] = 0x01; // command: read buttons
                buf[offset + 3] = (buttons >> 8) & 0xFF; // buttons high
                buf[offset + 4] = buttons & 0xFF;        // buttons low
                buf[offset + 5] = (int8_t)(x * 127);     // stick X
                buf[offset + 6] = (int8_t)(y * 127);     // stick Y
                break; // only controller 0
            }
        }
    }

    // Send SI completion message so the game doesn't hang waiting for SI DMA.
    ultramodern::send_si_message();
    ctx->r2 = 0; // return success
}
void __osContGetInitData_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPackRequestData_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPiRawReadIo_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPiRawWriteIo_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osPfsCheckRamArea_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __f_to_ull_recomp(uint8_t* rdram, recomp_context* ctx) {}
void osPiGetCmdQueue_recomp(uint8_t* rdram, recomp_context* ctx) {}

}
