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

// Boot flow: gs=4 (save) → gs=1 (Konami) → gs=12 (expansion pak) → gs=5 (intro) → gs=8 (title)
// All gamestates now run naturally — Controller Pak is implemented.
static void try_skip_boot_screens(uint8_t* rdram) {
    // No skips — the save screen works with virtual Controller Pak.
    return;
    // Keep the infrastructure below in case we need to skip screens later.
    uint32_t gsm_ptr_phys = 0x0C1520;
    int32_t gsm_addr = *(int32_t*)(rdram + gsm_ptr_phys);
    if (gsm_addr == 0) return;

    uint32_t gsm_phys = (uint32_t)gsm_addr & 0x1FFFFFFF;
    int32_t cur_state = *(int32_t*)(rdram + gsm_phys + 0x24);

    int target_gs = 0;
    int wait_frames = 0;

    if (target_gs == 0) return;

    static int skip_count = 0;
    skip_count++;
    if (skip_count < wait_frames) return;

    fprintf(stderr, "[SKIP_BOOT] gsm=0x%08X cur_state=%d → forcing transition to gs=%d\n",
            gsm_addr, cur_state, target_gs);

    // gamestate_change(target): current_game_state = -target, exitingGameState = 1
    *(int32_t*)(rdram + gsm_phys + 0x24) = -target_gs;
    *(int16_t*)(rdram + ((gsm_phys + 0x06) ^ 2)) = 1;
    skip_count = 0; // reset for next state
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

// Virtual Controller Pak read/write — defined in pak.cpp
extern "C" void pak_read(uint16_t addr, uint8_t* out, int count);
extern "C" void pak_write(uint16_t addr, const uint8_t* in, int count);

extern "C" {

void __osSiRawStartDma_recomp(uint8_t* rdram, recomp_context* ctx) {
    // s32 __osSiRawStartDma(s32 direction, void *dramAddr)
    // direction: 0 = OS_READ (PIF→DRAM), 1 = OS_WRITE (DRAM→PIF)
    int32_t direction = (int32_t)ctx->r4;
    uint32_t dram_addr = (uint32_t)ctx->r5;

    static int si_dma_count = 0; si_dma_count++;
    if (si_dma_count <= 10 || si_dma_count % 500 == 0) {
        // Also log gamestate for heartbeat during periods with no DLs
        uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
        int32_t cur_gs = 0;
        if (gsm_addr != 0) cur_gs = *(int32_t*)(rdram + (gsm_addr & 0x1FFFFFFF) + 0x24);
        fprintf(stderr, "[SI_DMA] #%d dir=%d addr=0x%08X gs=%d\n", si_dma_count, direction, dram_addr, cur_gs);
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

    // On first SI DMA, pre-seed OSContStatus so the game's pak init function
    // sees CONT_CARD_ON before contpak_get_inserted_status has a chance to run.
    // Without this, the init function reads cont_status=0 → pak "not present" →
    // then later cont_status=1 → pak "present" → "Controller Pak has been changed".
    {
        static bool seeded = false;
        if (!seeded) {
            seeded = true;
            uint32_t cs = 0x000EFB90;
            rdram[(cs+0) ^ 3] = 0x00; rdram[(cs+1) ^ 3] = 0x05;
            rdram[(cs+2) ^ 3] = 0x01; rdram[(cs+3) ^ 3] = 0x00;
            for (int i = 1; i < 4; i++) {
                rdram[(cs+i*4+0) ^ 3] = 0x00; rdram[(cs+i*4+1) ^ 3] = 0x00;
                rdram[(cs+i*4+2) ^ 3] = 0x00; rdram[(cs+i*4+3) ^ 3] = 0x08;
            }
            rdram[0x000F2260 ^ 3] = 0; // pak inserted on controller 0
            rdram[0x000F2261 ^ 3] = 1;
            rdram[0x000F2262 ^ 3] = 1;
            rdram[0x000F2263 ^ 3] = 1;
            fprintf(stderr, "[PAK] Seeded OSContStatus + pak status on first SI DMA\n");
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
        // Each command block corresponds to a controller channel (0-3).
        int pos = 0;
        int channel = 0;
        while (pos < 63) {  // PIF buffer is 64 bytes; byte 63 is status, not a command
            uint8_t tx = RD_MEM_B(pif, pos);
            if (tx == 0xFE) break;    // end marker
            if (tx == 0x00) { pos++; channel++; continue; } // channel separator
            if (tx == 0xFF) { pos++; continue; } // skip padding

            uint8_t rx = RD_MEM_B(pif, pos + 1);
            if (tx == 0 || rx == 0) break;
            // Bounds check: ensure response fits within PIF buffer
            if (pos + 2 + tx + rx > 64) break;

            uint8_t cmd = RD_MEM_B(pif, pos + 2);
            int response_start = pos + 2 + tx;

            {
                static int pif_cmd_count = 0; pif_cmd_count++;
                if (pif_cmd_count <= 40 || pif_cmd_count % 500 == 0)
                    fprintf(stderr, "[PIF] #%d ch=%d cmd=0x%02X tx=%d rx=%d pos=%d buf=0x%08X\n",
                            pif_cmd_count, channel, cmd, tx, rx, pos, dram_addr);
            }

            // Only controller 0 is connected
            if (channel > 0) {
                // Mark as no-response: set bit 7 of rx byte
                WR_MEM_B(pif, pos + 1, rx | 0x80);
                pos += 2 + tx + rx;
                channel++;
                continue;
            }

            if (cmd == 0x00) {
                // Controller status: type=0x0005, status=0x01 (CONT_CARD_ON = pak present)
                WR_MEM_B(pif, response_start + 0, 0x05); // type high
                WR_MEM_B(pif, response_start + 1, 0x00); // type low
                WR_MEM_B(pif, response_start + 2, 0x01); // status: pak present
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
            } else if (cmd == 0x02) {
                // Controller Pak READ: tx=3 (addr_hi, addr_lo, crc8), rx=33 (32 data + status)
                uint8_t addr_hi = RD_MEM_B(pif, pos + 3);
                uint8_t addr_lo = RD_MEM_B(pif, pos + 4);
                uint16_t pak_addr = ((addr_hi << 8) | addr_lo) & 0xFFE0;

                uint8_t pak_data[32];
                pak_read(pak_addr, pak_data, 32);
                for (int i = 0; i < 32; i++) {
                    WR_MEM_B(pif, response_start + i, pak_data[i]);
                }
                WR_MEM_B(pif, response_start + 32, 0x00); // status OK
                {
                    static int pr = 0; pr++;
                    if (pr <= 20)
                        fprintf(stderr, "[PIF] PAK_READ addr=0x%04X data=[%02X %02X %02X %02X %02X %02X %02X %02X...] tx=%d rx=%d\n",
                                pak_addr, pak_data[0], pak_data[1], pak_data[2], pak_data[3],
                                pak_data[4], pak_data[5], pak_data[6], pak_data[7], tx, rx);
                }
            } else if (cmd == 0x03) {
                // Controller Pak WRITE: tx=35 (addr_hi, addr_lo, 32 data, crc8), rx=1 (status)
                uint8_t addr_hi = RD_MEM_B(pif, pos + 3);
                uint8_t addr_lo = RD_MEM_B(pif, pos + 4);
                uint16_t pak_addr = ((addr_hi << 8) | addr_lo) & 0xFFE0;

                uint8_t pak_data[32];
                for (int i = 0; i < 32; i++) {
                    pak_data[i] = RD_MEM_B(pif, pos + 5 + i);
                }
                pak_write(pak_addr, pak_data, 32);
                WR_MEM_B(pif, response_start, 0x00); // status OK
                {
                    static int pw = 0; pw++;
                    if (pw <= 20)
                        fprintf(stderr, "[PIF] PAK_WRITE addr=0x%04X data=[%02X %02X %02X %02X...]\n",
                                pak_addr, pak_data[0], pak_data[1], pak_data[2], pak_data[3]);
                }
            }
            pos += 2 + tx + rx;
            channel++;
        }
    }

    if (direction == 0) {
        try_skip_boot_screens(rdram);

        // Monitor pak status — log whenever pak[0] changes
        {
            static uint8_t prev_pak0 = 255;
            uint8_t pak0 = rdram[0x000F2260 ^ 3];
            static int mon_count = 0; mon_count++;
            if (pak0 != prev_pak0) {
                fprintf(stderr, "[MON#%d] pak[0] CHANGED: %d → %d\n", mon_count, prev_pak0, pak0);
                prev_pak0 = pak0;
            }
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
        // Controller 0: type=0x0005, status=0x01 (pak present), err_no=0x00
        WR_B(data_addr + 0, 0x00); // type high byte
        WR_B(data_addr + 1, 0x05); // type low byte (CONT_TYPE_NORMAL)
        WR_B(data_addr + 2, 0x01); // status: CONT_CARD_ON (pak present)
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
// contpak_mainLoop: Let the recompiled version run — it properly handles PIF/SI
// message flow (readPIF → osRecvMesg → parsePIF). Overriding it breaks the SI
// message queue and prevents pak operations from working.

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
    ctx->r2 = 0; // success — pak RAM is functional
}

// contpak_is_plug, contpak_get_inserted_status, contpak_check_rumble_pak:
// Let recompiled versions run — they go through PIF/SI and rely on
// __osContGetInitData_recomp + PIF handler for correct responses.

// Forward declarations for pak.cpp functions
extern "C" void osPfsChecker_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsFreeBlocks_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsFindFile_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsReadWriteFile_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsInitPak_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsFileState_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsDeleteFile_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsAllocateFile_recomp(uint8_t* rdram, recomp_context* ctx);
extern "C" void osPfsNumFiles_recomp(uint8_t* rdram, recomp_context* ctx);

// Log func_8001D398 (contpak re-check) to see if wrappers are failing here
void func_8001D398(uint8_t* rdram, recomp_context* ctx) {
    // This function re-queries PIF for pak status and returns 1 (error) or 2 (found)
    uint8_t pak0 = rdram[0x000F2260 ^ 3];
    static int c = 0; if (++c <= 10) fprintf(stderr, "[PAK] func_8001D398 (recheck) pak[0]=%d\n", pak0);
    ctx->r2 = 2; // pak found — always report success
}

// ========================================================================
// BM64 APPROACH: Let recompiled contpak wrappers run natively.
// They check pak[0] (which is 0 from our seeding), reformat parameters,
// and call the game's SDK functions — which we override below.
// ========================================================================

// Override the game's built-in osPfsInitPak (at 0x800A4040).
// The game has its own copy of the N64 SDK osPfsInitPak. Our osPfsInitPak_recomp
// only overrides the SDK's copy at 0x800A3010. The save screen calls the game's
// copy which goes through PIF and fails on our virtual pak. Redirect to our
// implementation in pak.cpp which works with our save buffer directly.
void func_800A4040(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_800A4040 (game osPfsInitPak) → redirecting to our impl\n");
    osPfsInitPak_recomp(rdram, ctx);
}
// Override the game's built-in copies of osPfs SDK functions.
// The game statically linked the N64 SDK, so it has its own copies at different
// addresses. These go through PIF/SI which doesn't work with our virtual pak.
// Redirect them to our implementations in pak.cpp that access the save buffer directly.
// === BM64 APPROACH: Stub ALL internal osPfs PIF-level functions ===
// These are called from deep inside the game's compiled osPfs SDK code.
// Stubbing them prevents any PIF-level pak I/O, which would fail because
// our save buffer format doesn't match what the game's code expects.

// func_8009A100 = __osPfsRequestData (pak type detection via PIF)
void func_8009A100(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0; // success — pak is memory pak type
}

// func_800A4248 = __osPfsGetInitData (pak setup/label read)
void func_800A4248(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_800A5130 = __osPfsRWInode or __osPfsGetLabel (reads pak pages via PIF)
void func_800A5130(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_8009C1B0 = internal osPfs helper (inode chain traversal)
void func_8009C1B0(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_8009C284 = internal osPfs helper
void func_8009C284(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_8009BA54 = inode checksum validation
void func_8009BA54(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_8009BEA0 = inode repair
void func_8009BEA0(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_800A1394 = internal pak page read/write
void func_800A1394(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_80098500 = __osSiGetAccess (SI semaphore — no-op)
void func_80098500(uint8_t* rdram, recomp_context* ctx) {}

// func_80099D84 = game's osPfsReadWriteFile (size=0x378, flag in $a2: 0=read, 1=write)
void func_80099D84(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_80099D84 (osPfsReadWriteFile) flag=%d\n", (int)ctx->r6);
    osPfsReadWriteFile_recomp(rdram, ctx);
}

// func_8009A884 = osPfsFileState
extern "C" void osPfsFileState_recomp(uint8_t* rdram, recomp_context* ctx);
void func_8009A884(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_8009A884 called (osPfsFileState?)\n");
    osPfsFileState_recomp(rdram, ctx);
}

// func_8009A9E0 = osPfsDeleteFile (size=0x334)
// Always return success — even if the file doesn't exist, the game just wants
// to ensure space is available. Don't call our delete (which returns error for missing files).
void func_8009A9E0(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_8009A9E0 (osPfsDeleteFile) → success\n");
    ctx->r2 = 0; // always succeed
}

// func_800A16A0 = osPfsRepairId or similar wrapper
extern "C" void osPfsRepairId_recomp(uint8_t* rdram, recomp_context* ctx);
void func_800A16A0(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_800A16A0 called\n");
    ctx->r2 = 0; // success
}

// func_8009B000 / func_8009B0D0 = internal osPfs helpers (called from osPfsNumFiles)
void func_8009B000(uint8_t* rdram, recomp_context* ctx) { ctx->r2 = 0; }
void func_8009B0D0(uint8_t* rdram, recomp_context* ctx) { ctx->r2 = 0; }

// === CRITICAL: __osContRamRead / __osContRamWrite equivalents ===
// These format PIF commands for 32-byte pak block transfers and call __osSiRawStartDma.
// BM64 stubs these as no-ops. Without stubbing, the game's recompiled osPfs code
// does PIF-level pak I/O that conflicts with our save buffer format.

// func_8009A610 = __osContRamWrite (size=0x168, writes 32 bytes to pak via PIF)
void func_8009A610(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0; // success
}

// func_800A4EE0 = __osContRamRead (reads 32 bytes from pak via PIF)
void func_800A4EE0(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0; // success
}

// func_8009CC60 = __osContAddressCrc (computes CRC for pak address)
void func_8009CC60(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_800A5360 = another __osContRamRead variant (size=0xD0)
void func_800A5360(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_8009E480 = osPfs internal (size=0x508, called via function pointer, does PIF reads)
// Crashes because __osContRamRead is stubbed. Stub this too.
void func_8009E480(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// === Override the game's built-in copies of N64 SDK osPfs functions ===
// The contpak wrapper functions reformat parameters and call these.

// func_8009F400 = game's osPfsFindFile
void func_8009F400(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 10) fprintf(stderr, "[PAK] func_8009F400 (osPfsFindFile) called\n");
    osPfsFindFile_recomp(rdram, ctx);
    if (c <= 10) fprintf(stderr, "[PAK]   → returned %d\n", (int)(int32_t)ctx->r2);
}

// func_800A3E60 = game's osPfsChecker
void func_800A3E60(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_800A3E60 (osPfsChecker) called\n");
    osPfsChecker_recomp(rdram, ctx);
}

// func_800A09A0 = game's high-level "allocate + write save file" handler (size=0x648)
// Params: $a0=save_info_struct, $a1=controller, $a2=data/length, $a3=file_no_output
// The recompiled version does inode allocation via PIF (func_800A1394) which we stub.
// Override to call our osPfsAllocateFile + osPfsReadWriteFile.
void func_800A09A0(uint8_t* rdram, recomp_context* ctx) {
    gpr info = (gpr)(int32_t)(uint32_t)ctx->r4;  // save info struct
    int controller = (int)ctx->r5;
    uint32_t length = (uint32_t)ctx->r6;          // save data length
    gpr file_no_out = (gpr)(int32_t)(uint32_t)ctx->r7; // output

    // Extract save metadata from info struct
    uint16_t company = (uint16_t)MEM_HU(0x08, info);
    uint32_t game_code = (uint32_t)MEM_W(0x04, info);
    gpr name_addr = info + 0x0A;  // game_name in struct
    gpr ext_addr = info + 0x0E;   // ext_name in struct

    fprintf(stderr, "[PAK] func_800A09A0 (allocate+write): company=0x%04X game=0x%08X len=%d ctrl=%d\n",
            company, game_code, length, controller);

    // First try to find existing file
    gpr pfs_addr = (gpr)(int32_t)(0x800F1F20 + controller * 0x68);
    ctx->r4 = pfs_addr;
    ctx->r5 = (gpr)company;
    ctx->r6 = (gpr)game_code;
    ctx->r7 = ext_addr;
    MEM_W(0x10, ctx->r29) = (uint32_t)name_addr;
    MEM_W(0x14, ctx->r29) = (uint32_t)file_no_out;
    osPfsFindFile_recomp(rdram, ctx);

    if (ctx->r2 != 0) {
        // File not found — allocate it
        fprintf(stderr, "[PAK]   Allocating new file, length=%d\n", length);
        ctx->r4 = pfs_addr;
        ctx->r5 = (gpr)company;
        ctx->r6 = (gpr)game_code;
        ctx->r7 = name_addr;
        MEM_W(0x10, ctx->r29) = (uint32_t)ext_addr;
        MEM_W(0x14, ctx->r29) = length;
        MEM_W(0x18, ctx->r29) = (uint32_t)file_no_out;
        osPfsAllocateFile_recomp(rdram, ctx);
        if (ctx->r2 != 0) {
            fprintf(stderr, "[PAK]   Allocate FAILED: %d\n", (int)(int32_t)ctx->r2);
            return;
        }
    }
    fprintf(stderr, "[PAK]   File ready, file_no=%d\n",
            (int)(int32_t)MEM_W(0, file_no_out));
    ctx->r2 = 0; // success
}

// func_800A4D00 = game's __osPfsSelectBank (no-op for 32KB pak)
void func_800A4D00(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// func_800A3B40 = game's osPfsFreeBlocks
void func_800A3B40(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_800A3B40 (osPfsFreeBlocks) called\n");
    osPfsFreeBlocks_recomp(rdram, ctx);
}

// func_8009AE60 = game's osPfsNumFiles (called from contpak_get_free_blks)
extern "C" void osPfsNumFiles_recomp(uint8_t* rdram, recomp_context* ctx);
void func_8009AE60(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_8009AE60 (osPfsNumFiles) called\n");
    osPfsNumFiles_recomp(rdram, ctx);
}

// func_800968B0 = game's osPfsFreeBlocks
void func_800968B0(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5) fprintf(stderr, "[PAK] func_800968B0 (osPfsFreeBlocks) called\n");
    osPfsFreeBlocks_recomp(rdram, ctx);
}

void __f_to_ull_recomp(uint8_t* rdram, recomp_context* ctx) {}
void osPiGetCmdQueue_recomp(uint8_t* rdram, recomp_context* ctx) {}

}
