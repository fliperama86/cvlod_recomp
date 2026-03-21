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

// Force gamestate transition from save screen (4) to Konami logo (1).
static void try_skip_save_screen(uint8_t* rdram) {
    static int skip_counter = 0;
    skip_counter++;
    if (skip_counter != 30) return;

    // GameStateMgr* at 0x800C1520
    uint32_t gsm_ptr_phys = 0x0C1520;
    int32_t gsm_addr = *(int32_t*)(rdram + gsm_ptr_phys);
    if (gsm_addr == 0) return;

    uint32_t gsm_phys = (uint32_t)gsm_addr & 0x1FFFFFFF;
    int32_t cur_state = *(int32_t*)(rdram + gsm_phys + 0x24);

    fprintf(stderr, "[SKIP_SAVE] gsm=0x%08X cur_state=%d, forcing transition to gamestate 1\n",
            gsm_addr, cur_state);

    // gamestate_change(1): current_game_state = -1, exitingGameState = 1
    *(int32_t*)(rdram + gsm_phys + 0x24) = -1;
    *(int16_t*)(rdram + ((gsm_phys + 0x06) ^ 2)) = 1;
}

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
        // PIF WRITE: game sends command buffer. If empty, format a button read.
        uint8_t* pif = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];
        if (RD_MEM_B(pif, 0) == 0x00 || RD_MEM_B(pif, 0) == 0xFE) {
            WR_MEM_B(pif, 0, 0xFF); // channel separator
            WR_MEM_B(pif, 1, 0x01); // TX count
            WR_MEM_B(pif, 2, 0x04); // RX count
            WR_MEM_B(pif, 3, 0x01); // command: read buttons
            WR_MEM_B(pif, 4, 0x00); // buttons_hi (filled on READ)
            WR_MEM_B(pif, 5, 0x00); // buttons_lo
            WR_MEM_B(pif, 6, 0x00); // stick_x
            WR_MEM_B(pif, 7, 0xFE); // end marker
        }
    }

    if (direction == 0 && dram_addr != 0) {
        // PIF READ: populate response buffer with controller data.
        uint8_t* pif = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];

        // Ensure max_controllers is set (game reads from RDRAM at 0x8013EDB1).
        // MEM_BU(0x8013EDB1) → rdram[0x13EDB1 ^ 3] = rdram[0x13EDB2]
        rdram[0x13EDB2] = 1;

        // Parse the PIF command buffer to determine what the game is asking for.
        // The game uses swl/swr (raw big-endian) to write the PIF buffer, and
        // lwl/lwr to read it. These bypass XOR-3. So we use DIRECT byte access.
        // Parse PIF command buffer using XOR-3 byte access (consistent with
        // game's swl/swr writes and lwl/lwr reads which go through MEM_W).
        int pos = 0;
        while (pos < 63) {  // PIF buffer is 64 bytes; byte 63 is status, not a command
            uint8_t tx = RD_MEM_B(pif, pos);
            if (tx == 0xFE) break;    // end marker
            if (tx == 0x00) { pos++; continue; } // skip padding
            if (tx == 0xFF) { pos++; continue; } // skip separator

            uint8_t rx = RD_MEM_B(pif, pos + 1);
            if (tx == 0 || rx == 0) break;
            // Bounds check: ensure response fits within PIF buffer
            if (pos + 2 + tx + rx > 64) break;

            uint8_t cmd = RD_MEM_B(pif, pos + 2);
            int response_start = pos + 2 + tx;

            if (cmd == 0x00) {
                // Controller status query: report controller, no pak
                uint32_t off = (dram_addr & 0x3FFFFF) + response_start;
                *(int32_t*)(rdram + off) =
                    (int32_t)((0x05 << 24) | (0x00 << 16) | (0x00 << 8) | 0x00);
            } else if (cmd == 0x01) {
                // Read buttons — write as native-endian word (MEM_W format)
                uint16_t buttons = 0;
                float x = 0.0f, y = 0.0f;
                get_n64_input(0, &buttons, &x, &y);
                uint8_t sx = (uint8_t)(int8_t)(x * 127);
                uint8_t sy = (uint8_t)(int8_t)(y * 127);
                uint32_t off = (dram_addr & 0x3FFFFF) + response_start;
                *(int32_t*)(rdram + off) =
                    (int32_t)(((buttons >> 8) << 24) | ((buttons & 0xFF) << 16) |
                              (sy << 8) | sx);
            } else if (cmd == 0x02 || cmd == 0x03) {
                // Controller Pak read/write: set error flag
                WR_MEM_B(pif, pos + 1, rx | 0x80);
            }
            pos += 2 + tx + rx;
        }
    }

    // Try to skip past the save screen after it's been running a while
    if (direction == 0) {
        try_skip_save_screen(rdram);
    }

    // Send SI completion message so the game doesn't hang waiting for SI DMA.
    // Guard: only send if the SI event is registered (mq != 0).
    // Early boot calls __osSiRawStartDma before osSetEventMesg(SI).
    if (ultramodern::is_game_started()) {
        ultramodern::send_si_message();
    }
    ctx->r2 = 0; // return success
}
extern "C" s32 osContSetCh(uint8_t* rdram, uint8_t ch);

void __osContGetInitData_recomp(uint8_t* rdram, recomp_context* ctx) {
    // void __osContGetInitData(u8* bitpattern, OSContStatus* data)
    // bitpattern = $a0, data = $a1
    // Populate controller detection data: report controller 0 as connected.

    // Set the runtime's max_controllers so osContGetReadData works.
    // LoD bypasses osContInit, so the runtime never learns about controllers.
    osContSetCh(rdram, 1);

    uint32_t pattern_addr = (uint32_t)ctx->r4 & 0x3FFFFF;
    uint32_t data_addr = (uint32_t)ctx->r5 & 0x3FFFFF;

    // Use byte-swapped writes (XOR 3) to match how recompiled code reads via MEM_B/MEM_H.
    #define WR_B(addr, val) rdram[(addr) ^ 3] = (uint8_t)(val)
    if (pattern_addr < 0x800000) {
        WR_B(pattern_addr, 0x01); // bit 0 = controller 0 connected
    }
    if (data_addr < 0x800000) {
        // OSContStatus per controller: { u16 type, u8 status, u8 err_no } = 4 bytes
        // Controller 0: type=0x0005, status=0x00 (no pak), err_no=0x00
        WR_B(data_addr + 0, 0x00); // type high byte
        WR_B(data_addr + 1, 0x05); // type low byte (CONT_TYPE_NORMAL)
        WR_B(data_addr + 2, 0x00); // status: 0 = no pak
        WR_B(data_addr + 3, 0x00); // err_no: success
        // Controllers 1-3: no response
        for (int c = 1; c < 4; c++) {
            WR_B(data_addr + c * 4 + 0, 0x00);
            WR_B(data_addr + c * 4 + 1, 0x00);
            WR_B(data_addr + c * 4 + 2, 0x00);
            WR_B(data_addr + c * 4 + 3, 0x08); // CONT_NO_RESPONSE_ERROR
        }
    }
    #undef WR_B

    // CRITICAL: Also set the game's internal max_controllers at 0x8013EDB1.
    // The PIF command formatter (func_80098C60) and response parser (func_80098BD4)
    // both read this byte. Without it, they skip all controller data.
    // 0x80140000 - 0x124F = 0x8013EDB1
    rdram[0x0013EDB1 ^ 3] = 1; // 1 controller connected (byte-swapped)

}
// Override the game's custom PIF-based input loop with direct runtime callbacks.
// This is how every other N64 recomp project handles input — bypass PIF emulation
// and use the runtime's poll_input() / get_input() callbacks directly.
//
// Game controller struct at 0x801C87F4, 14 (0xE) bytes per controller:
//   +0x00: u16 (connection/misc)
//   +0x02: u16 btns_held
//   +0x04: u16 btns_pressed (accumulated, cleared by game)
//   +0x06: s16 joy_x
//   +0x08: s16 joy_y
//   +0x0A: u16 btns_pressed2 (accumulated, cleared by game)
//   +0x0C: s16 (angle/duration)
void contpak_mainLoop_recomp(uint8_t* rdram, recomp_context* ctx) {
    constexpr uint32_t CTRL_BASE = 0x801C87F4;
    constexpr int CTRL_STRIDE = 0x0E;
    constexpr int NUM_CONTROLLERS = 4;

    for (int c = 0; c < NUM_CONTROLLERS; c++) {
        uint16_t buttons = 0;
        float x = 0.0f, y = 0.0f;

        bool connected = get_n64_input(c, &buttons, &x, &y);

        // KSEG0 address of this controller's struct
        gpr base = (gpr)(int32_t)(CTRL_BASE + c * CTRL_STRIDE);

        // Read previous held state
        uint16_t prev_held = (uint16_t)MEM_HU(0x02, base);
        uint16_t prev_pressed = (uint16_t)MEM_HU(0x04, base);

        // Newly pressed = buttons that weren't held last frame
        uint16_t newly_pressed = (prev_held ^ buttons) & buttons;

        // Write held buttons
        MEM_H(0x02, base) = (int16_t)buttons;
        // Accumulate pressed (game clears this when it processes)
        MEM_H(0x04, base) = (int16_t)(prev_pressed | newly_pressed);

        // Stick values: convert float -1..1 to int8 -80..80, then to s16
        int8_t sx = (int8_t)(x * 80.0f);
        int8_t sy = (int8_t)(y * 80.0f);
        MEM_H(0x06, base) = (int16_t)sx;
        MEM_H(0x08, base) = (int16_t)sy;

        // Generate D-pad-like flags from stick position (dead zone = ±40)
        uint16_t stick_flags = 0;
        if (sx < -40) stick_flags |= 0x0200; // left
        if (sx > 40)  stick_flags |= 0x0100; // right
        if (sy > 40)  stick_flags |= 0x0800; // up
        if (sy < -40) stick_flags |= 0x0400; // down

        uint16_t prev_pressed2 = (uint16_t)MEM_HU(0x0A, base);
        uint16_t prev_stick_flags = (prev_pressed2 & 0x0F00);
        uint16_t new_stick = (prev_stick_flags ^ stick_flags) & stick_flags;
        MEM_H(0x0A, base) = (int16_t)(prev_pressed2 | new_stick);
    }

    // Send SI completion message so osRecvMesg in the game's input path doesn't block
    if (ultramodern::is_game_started()) {
        ultramodern::send_si_message();
    }
}

void __osPackRequestData_recomp(uint8_t* rdram, recomp_context* ctx) {
    // __osPackRequestData(u8 cmd) — formats __osContPifRam for a PIF command.
    // cmd = $a0 (0x00 = status query, 0x01 = read buttons, etc.)
    // Format the PIF buffer at 0x8013ED70 so __osSiRawStartDma READ can parse it.
    uint8_t cmd = (uint8_t)ctx->r4;

    // PIF buffer uses XOR-3 byte access (consistent with MEM_W/swl/swr).
    uint8_t* pif = &rdram[0x13ED70];
    if (cmd == 0x00) {
        // Status query: [sep][TX=1][RX=3][cmd=0x00][type_hi][type_lo][status][end]
        WR_MEM_B(pif, 0, 0xFF);
        WR_MEM_B(pif, 1, 0x01);
        WR_MEM_B(pif, 2, 0x03);
        WR_MEM_B(pif, 3, 0x00);
        WR_MEM_B(pif, 4, 0x00);
        WR_MEM_B(pif, 5, 0x00);
        WR_MEM_B(pif, 6, 0x00);
        WR_MEM_B(pif, 7, 0xFE);
    } else if (cmd == 0x01) {
        // Read buttons: [sep][TX=1][RX=4][cmd=0x01][btn_hi][btn_lo][stick_x][stick_y]
        WR_MEM_B(pif, 0, 0xFF);
        WR_MEM_B(pif, 1, 0x01);
        WR_MEM_B(pif, 2, 0x04);
        WR_MEM_B(pif, 3, 0x01);
        WR_MEM_B(pif, 4, 0x00);
        WR_MEM_B(pif, 5, 0x00);
        WR_MEM_B(pif, 6, 0x00);
        WR_MEM_B(pif, 7, 0x00);
    }
    ctx->r2 = 0;
}
void __osPiRawReadIo_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = (uint64_t)(int64_t)-1;
}
void __osPiRawWriteIo_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = (uint64_t)(int64_t)-1;
}
void __osPfsCheckRamArea_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 1; // PFS_ERR_NOPACK
}

// Override game's contpak_is_plug to bypass PIF protocol entirely.
// Returns 0 (success) and writes bitpattern=0 (no paks) via $a0 pointer.
void contpak_is_plug_recomp(uint8_t* rdram, recomp_context* ctx) {
    // a0 = bitpattern output pointer (u8*)
    uint32_t bp_addr = (uint32_t)ctx->r5;
    if (bp_addr != 0) {
        uint32_t phys = bp_addr & 0x1FFFFFFF;
        rdram[phys ^ 3] = 0x00; // no paks on any controller
    }
    ctx->r2 = 0; // success
}

// Override contpak_get_inserted_status: set all controllers as "not inserted"
// without going through PIF/SI DMA.
void contpak_get_inserted_status_recomp(uint8_t* rdram, recomp_context* ctx) {
    // contpak_uninserted[] at 0x800F0000 + 0x2260 = 0x800F2260
    // Each entry is 1 byte, indexed by controller number
    for (int c = 0; c < 4; c++) {
        uint32_t addr = 0x800F2260 + c;
        uint32_t phys = addr & 0x1FFFFFFF;
        rdram[phys ^ 3] = 1; // not inserted
    }
}

// Override contpak_check_rumble_pak: no pak present
void contpak_check_rumble_pak_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 1; // PFS_ERR_NOPACK
}
void __f_to_ull_recomp(uint8_t* rdram, recomp_context* ctx) {}
void osPiGetCmdQueue_recomp(uint8_t* rdram, recomp_context* ctx) {}

}
