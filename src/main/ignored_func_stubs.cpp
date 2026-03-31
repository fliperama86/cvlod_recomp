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

// Input callback — defined in main.cpp (C++ linkage)
bool get_n64_input(int controller_num, uint16_t* buttons, float* x, float* y);

// Virtual Controller Pak — defined in pak.cpp
extern "C" void pak_read(uint16_t addr, uint8_t* out, int count);
extern "C" void pak_write(uint16_t addr, const uint8_t* in, int count);
extern "C" uint8_t pak_data_crc(const uint8_t* data, int size);

extern "C" {

// ── OS thread/interrupt stubs (ignored by N64Recomp, handled by ultramodern) ──

void __osDequeueThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osEnqueueThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osDispatchThread_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osViSwapContext_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSpDeviceBusy_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSetCompare_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __ll_rshift_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSiGetAccess_recomp(uint8_t* rdram, recomp_context* ctx) {}
void __osSiRelAccess_recomp(uint8_t* rdram, recomp_context* ctx) {}

// ── PFS stubs ────────────────────────────────────────────────────────

// RAM area test — no real hardware to probe, always succeed.
void __osPfsCheckRamArea_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// osPfsInitPak is in N64Recomp's ignored list — forward to game's own copy.
// The game's recompiled osPfsInitPak at 0x800A4040 reads the pak image through
// __osContRamRead (PIF handler) and initializes the OSPfs struct correctly.
// osPfsInitPak: the recompiled version reads the pak via PIF and validates.
// Override BOTH entry points (SDK symbol + game's own copy) with a manual
// init that sets the OSPfs fields correctly based on our formatted pak.
// This avoids whatever validation issue the recompiled version hits.
static void do_pfs_init(uint8_t* rdram, recomp_context* ctx) {
    // osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, s32 channel)
    gpr queue_addr = (gpr)(int32_t)(uint32_t)ctx->r4;
    gpr pfs_addr = (gpr)(int32_t)(uint32_t)ctx->r5;
    int channel = (int)ctx->r6;

    static int c = 0; if (++c <= 5)
        fprintf(stderr, "[PAK] osPfsInitPak #%d queue=0x%08X pfs=0x%08X ch=%d\n",
                c, (uint32_t)(queue_addr & 0xFFFFFFFF), (uint32_t)(pfs_addr & 0xFFFFFFFF), channel);

    // Fill OSPfs struct to match what the emulator produces:
    //   +0x00: status (0x05 = initialized)
    //   +0x04: queue
    //   +0x08: channel
    //   +0x0C: id[32] (copy of pak ID block)
    //   +0x2C: label[32] (all zeros)
    //   +0x4C: version (0)
    //   +0x50: dir_size (16 = DEF_DIR_PAGES * PFS_ONE_PAGE)
    //   +0x54: inode_table (8 = 1 * PFS_ONE_PAGE)
    //   +0x58: minode_table (16 = 2 * PFS_ONE_PAGE)
    //   +0x5C: dir_table (24 = minode + banks * PFS_ONE_PAGE = 16 + 1*8)
    //   +0x60: inodeStartPage (5 = 1 + DEF_DIR_PAGES + 2*banks = 1+2+2)
    //   +0x64: banks (1)
    //   +0x65: activebank (0)
    MEM_W(0x00, pfs_addr) = 0x01;       // status: PFS_INITIALIZED (matches emulator)
    MEM_W(0x04, pfs_addr) = (uint32_t)(queue_addr & 0xFFFFFFFF);
    MEM_W(0x08, pfs_addr) = channel;

    // Copy ID block from pak (block 1 = 32 bytes at byte offset 0x20)
    uint8_t id_block[32];
    pak_read(0x0020, id_block, 32);
    for (int i = 0; i < 32; i++)
        MEM_B(0x0C + i, pfs_addr) = id_block[i];

    // Label (block 7 = byte offset 0xE0) — all zeros for empty pak
    for (int i = 0; i < 32; i++)
        MEM_B(0x2C + i, pfs_addr) = 0;

    // banks=1 (from ID block byte 0x1A)
    int banks = id_block[0x1A]; // should be 1
    MEM_W(0x4C, pfs_addr) = 0;                              // version
    MEM_W(0x50, pfs_addr) = 2 * 8;                          // dir_size = DEF_DIR_PAGES * PFS_ONE_PAGE
    MEM_W(0x54, pfs_addr) = 1 * 8;                          // inode_table = 1 * PFS_ONE_PAGE
    MEM_W(0x58, pfs_addr) = (1 + banks) * 8;                // minode_table
    MEM_W(0x5C, pfs_addr) = (1 + banks) * 8 + banks * 8;   // dir_table
    MEM_W(0x60, pfs_addr) = 1 + 2 + 2 * banks;             // inodeStartPage
    MEM_B(0x64, pfs_addr) = (uint8_t)banks;                 // banks
    MEM_B(0x65, pfs_addr) = 0;                              // activebank

    if (c <= 5)
        fprintf(stderr, "[PAK] osPfsInitPak #%d → OK (banks=%d inodeStart=%d)\n",
                c, banks, 1 + 2 + 2*banks);
    ctx->r2 = 0; // success
}

void osPfsInitPak_recomp(uint8_t* rdram, recomp_context* ctx) {
    do_pfs_init(rdram, ctx);
}
void func_800A4040(uint8_t* rdram, recomp_context* ctx) {
    do_pfs_init(rdram, ctx);
}

// func_8009E480: on the audio task chain, NOT a PFS save function.
// Crashes without this stub because it calls PFS internals that need full setup.
void func_8009E480(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// Bank select — single 32KB bank, no-op.
void func_800A4D00(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

// SI access semaphores — no real hardware contention.
void func_80098500(uint8_t* rdram, recomp_context* ctx) {}

// Raw PI I/O — not used for Controller Pak.
void __osPiRawReadIo_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = (uint64_t)(int64_t)-1;
}
void __osPiRawWriteIo_recomp(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = (uint64_t)(int64_t)-1;
}

// Misc
void __f_to_ull_recomp(uint8_t* rdram, recomp_context* ctx) {}
void osPiGetCmdQueue_recomp(uint8_t* rdram, recomp_context* ctx) {}

// ── SI DMA handler (PIF command processing) ──────────────────────────

// Byte access macros for PIF buffer (XOR-3 for big-endian byte order)
#define RD_MEM_B(base, offset) (base)[(offset) ^ 3]
#define WR_MEM_B(base, offset, val) (base)[(offset) ^ 3] = (uint8_t)(val)

void __osSiRawStartDma_recomp(uint8_t* rdram, recomp_context* ctx) {
    int32_t direction = (int32_t)ctx->r4;
    uint32_t dram_addr = (uint32_t)ctx->r5;

    // Diagnostic logging
    static int si_dma_count = 0; si_dma_count++;
    if (si_dma_count <= 10 || si_dma_count % 500 == 0) {
        uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
        int32_t cur_gs = 0;
        if (gsm_addr != 0) cur_gs = *(int32_t*)(rdram + (gsm_addr & 0x1FFFFFFF) + 0x24);
        fprintf(stderr, "[SI_DMA] #%d dir=%d gs=%d\n", si_dma_count, direction, cur_gs);
    }

    // On WRITE: if the PIF buffer is empty, format a basic button read command.
    if (direction == 1 && dram_addr != 0) {
        uint8_t* pif = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];
        if (RD_MEM_B(pif, 0) == 0x00 || RD_MEM_B(pif, 0) == 0xFE) {
            WR_MEM_B(pif, 0, 0xFF);
            WR_MEM_B(pif, 1, 0x01);
            WR_MEM_B(pif, 2, 0x04);
            WR_MEM_B(pif, 3, 0x01);
            WR_MEM_B(pif, 4, 0x00);
            WR_MEM_B(pif, 5, 0x00);
            WR_MEM_B(pif, 6, 0x00);
            WR_MEM_B(pif, 7, 0xFE);
        }
    }

    // Seed OSContStatus on first SI DMA so the game sees CONT_CARD_ON
    // before contpak_get_inserted_status runs.
    {
        static bool seeded = false;
        if (!seeded) {
            seeded = true;
            // Match emulator exactly: all 4 controllers type=0x0005, ctrl 0-2 pak present
            uint32_t cs = 0x000EFB90;
            for (int i = 0; i < 4; i++) {
                rdram[(cs+i*4+0) ^ 3] = 0x00;
                rdram[(cs+i*4+1) ^ 3] = 0x05; // type = CONT_TYPE_NORMAL
                rdram[(cs+i*4+2) ^ 3] = (i < 3) ? 0x01 : 0x00; // status: pak present on 0-2
                rdram[(cs+i*4+3) ^ 3] = 0x00; // err = 0 (no error)
            }
            // pak insertion: 0 = inserted
            rdram[0x000F2260 ^ 3] = 0;
            rdram[0x000F2261 ^ 3] = 0;
            rdram[0x000F2262 ^ 3] = 0;
            rdram[0x000F2263 ^ 3] = 0;
            fprintf(stderr, "[PAK] Seeded OSContStatus (4 controllers, pak on 0-2)\n");
        }
    }

    // On READ: parse PIF command buffer and fill responses.
    if (direction == 0 && dram_addr != 0) {
        uint8_t* pif = &rdram[(uint64_t)(dram_addr & 0x3FFFFFFF)];

        // Ensure max_controllers is set (4, matching emulator)
        rdram[0x13EDB2] = 4;

        int pos = 0;
        int channel = 0;
        while (pos < 63) {
            uint8_t tx = RD_MEM_B(pif, pos);
            if (tx == 0xFE) break;
            if (tx == 0x00) { pos++; channel++; continue; }
            if (tx == 0xFF) { pos++; continue; }

            uint8_t rx = RD_MEM_B(pif, pos + 1);
            if (tx == 0 || rx == 0) break;
            if (pos + 2 + tx + rx > 64) break;

            uint8_t cmd = RD_MEM_B(pif, pos + 2);
            int response_start = pos + 2 + tx;

            // Only controller 0 is connected
            if (channel > 0) {
                WR_MEM_B(pif, pos + 1, rx | 0x80);
                pos += 2 + tx + rx;
                channel++;
                continue;
            }

            if (cmd == 0x00) {
                // Controller status: type=0x0005, status=0x01 (CONT_CARD_ON)
                WR_MEM_B(pif, response_start + 0, 0x05);
                WR_MEM_B(pif, response_start + 1, 0x00);
                WR_MEM_B(pif, response_start + 2, 0x01);
            } else if (cmd == 0xFF) {
                // Reset / identify — same as status
                WR_MEM_B(pif, response_start + 0, 0x05);
                WR_MEM_B(pif, response_start + 1, 0x00);
                WR_MEM_B(pif, response_start + 2, 0x01);
            } else if (cmd == 0x01) {
                // Read buttons
                uint16_t buttons = 0;
                float x = 0.0f, y = 0.0f;
                get_n64_input(0, &buttons, &x, &y);
                uint8_t sx = (uint8_t)(int8_t)(x * 127);
                uint8_t sy = (uint8_t)(int8_t)(y * 127);
                WR_MEM_B(pif, response_start + 0, buttons >> 8);
                WR_MEM_B(pif, response_start + 1, buttons & 0xFF);
                WR_MEM_B(pif, response_start + 2, sx);
                WR_MEM_B(pif, response_start + 3, sy);
            } else if (cmd == 0x02) {
                // Controller Pak READ
                uint8_t addr_hi = RD_MEM_B(pif, pos + 3);
                uint8_t addr_lo = RD_MEM_B(pif, pos + 4);
                uint16_t pak_addr = ((addr_hi << 8) | addr_lo) & 0xFFE0;

                uint8_t pak_data[32];
                pak_read(pak_addr, pak_data, 32);
                for (int i = 0; i < 32; i++)
                    WR_MEM_B(pif, response_start + i, pak_data[i]);
                uint8_t crc = pak_data_crc(pak_data, 32);
                WR_MEM_B(pif, response_start + 32, crc);
                {
                    static int pr = 0; pr++;
                    if (pr <= 30 || pr % 100 == 0)
                        fprintf(stderr, "[PIF] PAK_READ #%d addr=0x%04X crc=0x%02X data=[%02X %02X %02X %02X...]\n",
                                pr, pak_addr, crc, pak_data[0], pak_data[1], pak_data[2], pak_data[3]);
                }
            } else if (cmd == 0x03) {
                // Controller Pak WRITE
                uint8_t addr_hi = RD_MEM_B(pif, pos + 3);
                uint8_t addr_lo = RD_MEM_B(pif, pos + 4);
                uint16_t pak_addr = ((addr_hi << 8) | addr_lo) & 0xFFE0;

                uint8_t pak_data[32];
                for (int i = 0; i < 32; i++)
                    pak_data[i] = RD_MEM_B(pif, pos + 5 + i);
                pak_write(pak_addr, pak_data, 32);
                uint8_t crc = pak_data_crc(pak_data, 32);
                WR_MEM_B(pif, response_start, crc);
                {
                    static int pw = 0; pw++;
                    if (pw <= 30 || pw % 100 == 0)
                        fprintf(stderr, "[PIF] PAK_WRITE #%d addr=0x%04X crc=0x%02X data=[%02X %02X %02X %02X...]\n",
                                pw, pak_addr, crc, pak_data[0], pak_data[1], pak_data[2], pak_data[3]);
                }
            } else {
                // Unknown PIF command — log it
                static int unk = 0; unk++;
                if (unk <= 20)
                    fprintf(stderr, "[PIF] UNKNOWN cmd=0x%02X ch=%d tx=%d rx=%d pos=%d\n",
                            cmd, channel, tx, rx, pos);
            }
            pos += 2 + tx + rx;
            channel++;
        }
    }

    // Send SI completion message
    if (ultramodern::is_game_started()) {
        ultramodern::send_si_message();
    }
    ctx->r2 = 0;
}

// ── Controller detection ─────────────────────────────────────────────

extern "C" s32 osContSetCh(uint8_t* rdram, uint8_t ch);

void __osContGetInitData_recomp(uint8_t* rdram, recomp_context* ctx) {
    osContSetCh(rdram, 1);

    uint32_t pattern_addr = (uint32_t)ctx->r4 & 0x3FFFFF;
    uint32_t data_addr = (uint32_t)ctx->r5 & 0x3FFFFF;

    #define WR_B(addr, val) rdram[(addr) ^ 3] = (uint8_t)(val)
    if (pattern_addr < 0x800000) {
        WR_B(pattern_addr, 0x01);
    }
    if (data_addr < 0x800000) {
        // Match emulator: all 4 controllers present, type=0x0005, pak on 0-2
        for (int c = 0; c < 4; c++) {
            WR_B(data_addr + c*4 + 0, 0x00);
            WR_B(data_addr + c*4 + 1, 0x05); // CONT_TYPE_NORMAL
            WR_B(data_addr + c*4 + 2, (c < 3) ? 0x01 : 0x00); // pak present on 0-2
            WR_B(data_addr + c*4 + 3, 0x00); // no error
        }
    }
    #undef WR_B

    rdram[0x0013EDB1 ^ 3] = 4; // max_controllers = 4 (matches emulator)
}

// ── PIF buffer formatting ────────────────────────────────────────────

void __osPackRequestData_recomp(uint8_t* rdram, recomp_context* ctx) {
    uint8_t cmd = (uint8_t)ctx->r4;
    uint8_t* pif = &rdram[0x13ED70];
    if (cmd == 0x00) {
        WR_MEM_B(pif, 0, 0xFF); WR_MEM_B(pif, 1, 0x01);
        WR_MEM_B(pif, 2, 0x03); WR_MEM_B(pif, 3, 0x00);
        WR_MEM_B(pif, 4, 0x00); WR_MEM_B(pif, 5, 0x00);
        WR_MEM_B(pif, 6, 0x00); WR_MEM_B(pif, 7, 0xFE);
    } else if (cmd == 0x01) {
        WR_MEM_B(pif, 0, 0xFF); WR_MEM_B(pif, 1, 0x01);
        WR_MEM_B(pif, 2, 0x04); WR_MEM_B(pif, 3, 0x01);
        WR_MEM_B(pif, 4, 0x00); WR_MEM_B(pif, 5, 0x00);
        WR_MEM_B(pif, 6, 0x00); WR_MEM_B(pif, 7, 0x00);
    }
    ctx->r2 = 0;
}

#undef RD_MEM_B
#undef WR_MEM_B

} // end extern "C"
