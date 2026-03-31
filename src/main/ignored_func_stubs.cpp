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

// Overlay management — defined in librecomp
extern "C" void load_overlay_by_id(uint32_t id, uint32_t ram_addr);
// Copy overlay data from ROM to RDRAM (byte-swapped) — defined in lod_init.cpp
extern "C" void lod_copy_overlay_data(uint8_t* rdram, uint32_t rom_offset,
                                       uint32_t rdram_dst, uint32_t size);

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

// osPfsInitPak is in N64Recomp's ignored list, so we must provide a stub.
// Forward to the game's own recompiled copy at 0x800A4040 (in funcs_42.c).
// The recompiled version reads the pak via PIF (__osContRamRead), validates
// checksums, and — critically — calls func_800A4350 which builds the inode
// cache that the rest of the PFS stack depends on.
void osPfsInitPak_recomp(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; if (++c <= 5)
        fprintf(stderr, "[PAK] osPfsInitPak_recomp #%d → forwarding to native func_800A4040\n", c);
    // func_800A4040 is the game's recompiled osPfsInitPak (RECOMP_FUNC in funcs_42.c).
    // Declare and call it — our extern "C" linkage is compatible with the C-linkage
    // recomp calling convention.
    void func_800A4040(uint8_t* rdram, recomp_context* ctx);
    func_800A4040(rdram, ctx);
    if (c <= 5)
        fprintf(stderr, "[PAK] osPfsInitPak_recomp #%d → result=%d\n", c, (int)(int32_t)ctx->r2);
}

// Trace game-level PFS wrappers to see which ones get called during save screen
// func_8009F400 (osPfsFindFile): scan directory directly from pak image.
// The recompiled version goes through PIF but returned a wrong error code.
void func_8009F400(uint8_t* rdram, recomp_context* ctx) {
    // s32 osPfsFindFile(OSPfs *pfs, u16 company, u32 game_code, u8 *name, u8 *ext, s32 *file_no)
    gpr pfs_addr = (gpr)(int32_t)(uint32_t)ctx->r4;
    uint16_t company = (uint16_t)ctx->r5;
    uint32_t game_code = (uint32_t)ctx->r6;
    gpr name_addr = (gpr)(int32_t)(uint32_t)ctx->r7;
    gpr ext_addr = (gpr)(int32_t)(uint32_t)MEM_W(0x10, ctx->r29);
    gpr file_no_ptr = (gpr)(int32_t)(uint32_t)MEM_W(0x14, ctx->r29);

    // Read name and ext from RDRAM
    uint8_t name[16], ext[4];
    for (int i = 0; i < 16; i++) name[i] = MEM_BU(name_addr, i);
    for (int i = 0; i < 4; i++) ext[i] = MEM_BU(ext_addr, i);

    // Scan directory directly from pak image (16 entries at page 3-4)
    // Directory entry format: game_code(4) + company(2) + start_page(2) + status(1) + ...
    int found = -1;
    for (int slot = 0; slot < 16; slot++) {
        uint8_t entry[32];
        pak_read(0x0300 + slot * 32, entry, 32);
        uint32_t e_gc = (entry[0]<<24)|(entry[1]<<16)|(entry[2]<<8)|entry[3];
        uint16_t e_cc = (entry[4]<<8)|entry[5];
        if (e_gc == 0 && e_cc == 0) continue; // empty slot
        if (e_gc == game_code && e_cc == company &&
            memcmp(entry + 0x0C, ext, 4) == 0 &&
            memcmp(entry + 0x10, name, 16) == 0) {
            found = slot;
            break;
        }
    }

    static int c = 0; if (++c <= 10)
        fprintf(stderr, "[PFS] FindFile #%d company=0x%04X game=0x%08X → %s (slot=%d)\n",
                c, company, game_code, found >= 0 ? "FOUND" : "NOT FOUND", found);

    // Dump save object state on every FindFile call
    // Object at 0x8031AFA4 (state=3 on emulator, save screen controller)
    if (c <= 5) {
        uint32_t obj = 0x31AFA4;
        fprintf(stderr, "[OBJ] Save object @ 0x8031AFA4 during Create:\n");
        fprintf(stderr, "[OBJ]   +0x08(flags)=0x%08X +0x09(state)=%d\n",
            *(uint32_t*)(rdram + obj + 0x08), rdram[(obj + 0x09) ^ 3]);
        fprintf(stderr, "[OBJ]   +0x38(alloc1)=0x%08X +0x40(alloc3)=0x%08X +0x48(alloc5)=0x%08X\n",
            *(uint32_t*)(rdram + obj + 0x38), *(uint32_t*)(rdram + obj + 0x40),
            *(uint32_t*)(rdram + obj + 0x48));
        fprintf(stderr, "[OBJ]   +0x10(dispatch)=0x%08X +0x24(fig0)=0x%08X\n",
            *(uint32_t*)(rdram + obj + 0x10), *(uint32_t*)(rdram + obj + 0x24));
    }

    if (found >= 0) {
        if (file_no_ptr) MEM_W(0, file_no_ptr) = found;
        ctx->r2 = 0;
    } else {
        ctx->r2 = 5; // PFS_ERR_INVALID = file not found
    }
}
// contpak_get_inserted_status (0x8001CCC0): calls osPfsIsPlug via PIF,
// then writes pak insertion status to 0x800F2260. Let recompiled run,
// but trace its result.
void contpak_get_inserted_status(uint8_t* rdram, recomp_context* ctx) {
    // The recompiled version calls func_800A0210 (osPfsIsPlug) which sends
    // PIF status queries. Our PIF handler responds correctly. But the function
    // also calls osRecvMesg to wait for SI completion, and func_800A0294 to
    // parse the response. These should all work.
    //
    // Instead of running the recompiled version (which we can't call as weak),
    // just set the pak insertion status directly:
    for (int i = 0; i < 4; i++) {
        // Read status byte from OSContStatus
        uint8_t status = rdram[(0x0EFB92 + i*4) ^ 3]; // OSContStatus[i].status
        uint8_t inserted = (status & 0x01) ? 0 : 1; // CONT_CARD_ON → 0=inserted
        rdram[(0x0F2260 + i) ^ 3] = inserted;
    }
    static int c = 0; if (++c <= 5)
        fprintf(stderr, "[PFS] contpak_get_inserted_status #%d pak=[%d,%d,%d,%d]\n", c,
            rdram[0x0F2260^3], rdram[0x0F2261^3], rdram[0x0F2262^3], rdram[0x0F2263^3]);
}

// func_8001D398 (contpak recheck): override to return 2 (found).
// The recompiled version calls contpak_get_inserted_status which needs
// full PIF flow. Returning 2 directly is simpler and matches emulator behavior.
void func_8001D398(uint8_t* rdram, recomp_context* ctx) {
    // Still run our contpak_get_inserted_status to update pak[] array
    contpak_get_inserted_status(rdram, ctx);
    ctx->r2 = 2;
}

// func_8001C93C (pak detection/validation): trace return value.
// This is called during save screen boot and gates whether the object
// enters state 3 (normal) or state 5 (abnormality).
void func_8001C93C(uint8_t* rdram, recomp_context* ctx) {
    uint8_t ctrl = (uint8_t)ctx->r4;
    // Check pak insertion status
    uint8_t pak_status = rdram[(0x0F2260 + ctrl) ^ 3];
    static int c = 0; if (++c <= 10)
        fprintf(stderr, "[PFS] func_8001C93C (pak_detect) #%d ctrl=%d pak[%d]=%d\n",
                c, ctrl, ctrl, pak_status);
    // If pak is inserted (0), return success (0). Otherwise return error.
    if (pak_status != 0) {
        ctx->r2 = 0x2000; // pak not present
        if (c <= 10) fprintf(stderr, "[PFS]   → 0x2000 (not present)\n");
    } else {
        ctx->r2 = 0; // success
        if (c <= 10) fprintf(stderr, "[PFS]   → 0 (present)\n");
    }
}

// func_800A16A0 (PFS status gate): logging wrapper that replicates the
// recompiled logic but adds diagnostics to trace the 4→5 transition.
void func_800A16A0(uint8_t* rdram, recomp_context* ctx) {
    gpr pfs_addr = (gpr)(int32_t)(uint32_t)ctx->r4;
    uint32_t status = MEM_W(pfs_addr, 0X0);
    static int c = 0; c++;

    if ((status & 0x5) == 0) {
        if (c <= 20) fprintf(stderr, "[GATE] func_800A16A0 #%d pfs=0x%08X status=0x%08X → skip (bits clear)\n",
                c, (uint32_t)(pfs_addr & 0xFFFFFFFF), status);
        ctx->r2 = 5; // default return (PFS_ERR_INVALID) — matches recompiled code
        return;
    }

    if (c <= 20) fprintf(stderr, "[GATE] func_800A16A0 #%d pfs=0x%08X status=0x%08X → calling func_8009C004\n",
            c, (uint32_t)(pfs_addr & 0xFFFFFFFF), status);

    // Call the recompiled inode validator (func_8009C004 in funcs_41.c)
    void func_8009C004(uint8_t* rdram, recomp_context* ctx);
    func_8009C004(rdram, ctx);
    int32_t result = (int32_t)ctx->r2;

    if (c <= 20) fprintf(stderr, "[GATE] func_800A16A0 #%d → func_8009C004 returned %d\n", c, result);

    if (result == 0) {
        // Success: clear bits 0 and 2 in status (matches recompiled logic: AND ~0x5)
        uint32_t new_status = status & ~(uint32_t)0x5;
        MEM_W(0X0, pfs_addr) = new_status;
        if (c <= 20) fprintf(stderr, "[GATE] func_800A16A0 #%d → cleared status 0x%08X → 0x%08X\n",
                c, status, new_status);
    }
    ctx->r2 = (uint64_t)(int64_t)result;
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

                // Auto-press: inject button presses at specific frame counts
                // A=0x8000, B=0x4000, Start=0x1000, DPad-Up=0x0800, DPad-Down=0x0400
                static int auto_frame = 0;
                auto_frame++;

                // At ~3s press DPad-Down to move cursor to "Create"
                if (auto_frame == 180) {
                    buttons |= 0x0400; // DPad-Down
                    fprintf(stderr, "[AUTO] Frame %d: DPad-Down (move to Create)\n", auto_frame);
                }
                // At ~3.5s press A to select "Create"
                if (auto_frame == 210) {
                    buttons |= 0x8000; // A button
                    fprintf(stderr, "[AUTO] Frame %d: A (select Create)\n", auto_frame);
                }
                // At ~5s press A again (confirm if needed)
                if (auto_frame == 300) {
                    buttons |= 0x8000;
                    fprintf(stderr, "[AUTO] Frame %d: A again\n", auto_frame);
                }

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

    // Trace scheduler messages — dump the message slots and queue state
    if (direction == 0) {
        static int dc = 0;
        if (++dc == 100) {
            // Main message queue at 0x800C5D38
            uint32_t mq_valid = *(uint32_t*)(rdram + 0x0C5D38 + 0x08);
            uint32_t mq_first = *(uint32_t*)(rdram + 0x0C5D38 + 0x0C);
            uint32_t mq_count = *(uint32_t*)(rdram + 0x0C5D38 + 0x10);
            uint32_t mq_msgs  = *(uint32_t*)(rdram + 0x0C5D38 + 0x14);
            fprintf(stderr, "[MSG] queue@5D38: valid=%d first=%d count=%d msgs=0x%08X\n",
                mq_valid, mq_first, mq_count, mq_msgs);
            // Dump message slots at 0x800C5D70 (known from emulator)
            fprintf(stderr, "[MSG] msg_slots:");
            for (int i = 0; i < 8; i++) {
                uint32_t v = *(uint32_t*)(rdram + 0x0C5D50 + i*4);
                fprintf(stderr, " %08X", v);
            }
            fprintf(stderr, "\n");
            // Dump scheduler event structs at 0x800C5E80 area
            fprintf(stderr, "[MSG] sched@5E80:");
            for (int i = 0; i < 8; i++) {
                uint32_t v = *(uint32_t*)(rdram + 0x0C5E80 + i*4);
                fprintf(stderr, " %08X", v);
            }
            fprintf(stderr, "\n");
        }
    }

    // Map overlay loading is now done in lod_on_init (before game threads start).
    // See lod_init.cpp for map_ovl_34 loading.

    // Watchpoint: monitor save object state byte at 0x8031AFA4+0x09
    if (direction == 0) {
        static uint8_t prev_state = 255;
        uint8_t cur_state = rdram[(0x31AFA4 + 0x09) ^ 3];
        // Log alloc_data when state changes
        if (cur_state != prev_state && cur_state >= 3 && cur_state <= 5) {
            fprintf(stderr, "[ALLOC] state=%d alloc1=0x%08X alloc2=0x%08X alloc3=0x%08X alloc5=0x%08X\n",
                cur_state, *(uint32_t*)(rdram + 0x31AFA4 + 0x38), *(uint32_t*)(rdram + 0x31AFA4 + 0x3C),
                *(uint32_t*)(rdram + 0x31AFA4 + 0x40), *(uint32_t*)(rdram + 0x31AFA4 + 0x48));
        }
        if (cur_state != prev_state && cur_state != 0) {
            fprintf(stderr, "[WATCH] Save obj state: %d → %d (SI_DMA #%d)\n",
                    prev_state, cur_state, si_dma_count);
            // When it transitions to 4 or 5, dump context + child object tree
            if (cur_state == 4 || cur_state == 5) {
                uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
                int32_t cur_gs = 0;
                if (gsm_addr) cur_gs = *(int32_t*)(rdram + (gsm_addr & 0x1FFFFFFF) + 0x24);
                fprintf(stderr, "[WATCH]   gs=%d dispatch=0x%08X parent_state=%d\n",
                    cur_gs, *(uint32_t*)(rdram + 0x31AFA4 + 0x10),
                    rdram[(0x31AF30 + 0x09) ^ 3]);
                fprintf(stderr, "[WATCH]   pak[0]=%d pfs_status=0x%08X\n",
                    rdram[0x0F2260 ^ 3], *(uint32_t*)(rdram + 0x0F1F20));
                // State machine jump table is at 0x802F71A8 (map_ovl_34 data).
                // State dispatch: handler = MEM_W(0x802F71A8 + state*4)
                // Dump table entries for states 0-7.
                uint32_t jt_base = 0x002F71A8; // physical RDRAM offset
                if (jt_base + 32 < 0x800000) {
                    fprintf(stderr, "[WATCH]   jump_table @0x802F71A8:");
                    for (int s = 0; s < 8; s++)
                        fprintf(stderr, " [%d]=0x%08X", s, *(uint32_t*)(rdram + jt_base + s*4));
                    fprintf(stderr, "\n");
                }
                // Dump save object and child
                uint32_t obj = 0x31AFA4;
                uint32_t child_phys = (*(uint32_t*)(rdram + obj + 0x1C)) & 0x1FFFFFFF;
                if (child_phys + 0x30 < 0x800000) {
                    fprintf(stderr, "[WATCH]   child @0x%08X +0x24=0x%08X state=%d +0x0E=%d\n",
                        *(uint32_t*)(rdram + obj + 0x1C),
                        *(uint32_t*)(rdram + child_phys + 0x24),
                        rdram[(child_phys + 0x09) ^ 3],
                        (int16_t)((rdram[(child_phys + 0x0E) ^ 3] << 8) | rdram[(child_phys + 0x0F) ^ 3]));
                }
            }
            prev_state = cur_state;
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
