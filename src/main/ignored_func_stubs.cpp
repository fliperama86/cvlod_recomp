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
    if (direction == 1 && dram_addr != 0) {
        uint8_t* buf = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];
        // Check if buffer has no valid commands.
        // NOTE: N64 is big-endian, recomp host is little-endian.
        // MEM_B(i, addr) reads buf[(i & ~3) | (3 - (i & 3))].
        // To write logical byte N: buf[N ^ 3] for first word, buf[4 + ((N-4) ^ 3)] for second.
        #define PIF_BYTE(buf, n) (buf)[((n) & ~3) | (3 - ((n) & 3))]
        if (PIF_BYTE(buf, 0) == 0x00 || PIF_BYTE(buf, 3) == 0xFE) {
            // Format a PIF button read for controller 0 (byte-swapped):
            PIF_BYTE(buf, 0) = 0x01; // TX count
            PIF_BYTE(buf, 1) = 0x04; // RX count
            PIF_BYTE(buf, 2) = 0x01; // command: read buttons
            PIF_BYTE(buf, 3) = 0x00; // buttons_hi (filled on READ)
            PIF_BYTE(buf, 4) = 0x00; // buttons_lo
            PIF_BYTE(buf, 5) = 0x00; // stick_x
            PIF_BYTE(buf, 6) = 0x00; // stick_y
            PIF_BYTE(buf, 7) = 0xFE; // end
            if (si_dma_count <= 5) {
                fprintf(stderr, "[SI_DMA] #%d WRITE: formatted PIF read command for 1 controller\n", si_dma_count);
            }
        }
    }

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

        // Inject controller data into PIF response buffer.
        // The game may format the buffer with TX=1,RX=4 commands, OR the buffer
        // may be empty (just 0xFE end marker). Either way, we write a standard
        // PIF controller read response for channel 0 at the expected offset.
        //
        // Try pattern match first; fall back to writing at offset 0 if empty.
        // Inject controller data using byte-swapped addressing (N64 big-endian).
        // MEM_B(n) reads buf[(n&~3)|(3-(n&3))], so we write the same way.
        #ifndef PIF_BYTE
        #define PIF_BYTE(buf, n) (buf)[((n) & ~3) | (3 - ((n) & 3))]
        #endif
        bool injected = false;
        for (int offset = 0; offset < 64 - 6; offset++) {
            if (PIF_BYTE(buf, offset) == 0x01 && PIF_BYTE(buf, offset + 1) == 0x04) {
                PIF_BYTE(buf, offset + 3) = (buttons >> 8) & 0xFF;
                PIF_BYTE(buf, offset + 4) = buttons & 0xFF;
                PIF_BYTE(buf, offset + 5) = (int8_t)(x * 127);
                PIF_BYTE(buf, offset + 6) = (int8_t)(y * 127);
                injected = true;
                break;
            }
        }
        if (!injected) {
            // Fallback: write complete PIF response at offset 0
            PIF_BYTE(buf, 0) = 0x01;
            PIF_BYTE(buf, 1) = 0x04;
            PIF_BYTE(buf, 2) = 0x01;
            PIF_BYTE(buf, 3) = (buttons >> 8) & 0xFF;
            PIF_BYTE(buf, 4) = buttons & 0xFF;
            PIF_BYTE(buf, 5) = (int8_t)(x * 127);
            PIF_BYTE(buf, 6) = (int8_t)(y * 127);
            PIF_BYTE(buf, 7) = 0xFE;
            injected = true;
        }
        if (injected && (si_dma_count <= 5)) {
            fprintf(stderr, "[SI_DMA] #%d PIF inject: buttons=0x%04X raw:", si_dma_count, buttons);
            for (int b = 0; b < 8; b++) fprintf(stderr, " %02X", buf[b]);
            fprintf(stderr, " logical:");
            for (int b = 0; b < 8; b++) fprintf(stderr, " %02X", PIF_BYTE(buf, b));
            fprintf(stderr, "\n");
        }
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
