// Stubs for N64 OS functions that are in N64Recomp's ignored_funcs list.
// The recompiler doesn't generate code for these, but other recompiled OS
// internal functions still reference them via _recomp suffix.
// These are all handled natively by ultramodern, so empty stubs are safe.

#include <cstdint>
#include <cstdio>
#include <cstring>
#if defined(__APPLE__) || defined(__linux__)
#include <dlfcn.h>
#endif
#include "ultramodern/ultramodern.hpp"
#include "ultramodern/ultra64.h"

#include "recomp.h"
#include "librecomp/overlays.hpp"

#ifndef LOD_ENABLE_SAVE_AUTO_INPUT
#define LOD_ENABLE_SAVE_AUTO_INPUT 0
#endif

#ifndef LOD_ENABLE_SAVE_HANDLER_TRACE
#define LOD_ENABLE_SAVE_HANDLER_TRACE 0
#endif

#ifndef LOD_ENABLE_SAVE_EXIT_TRACE
#define LOD_ENABLE_SAVE_EXIT_TRACE 0
#endif

#ifndef LOD_ENABLE_SAVE_STATE45_TRACE
#define LOD_ENABLE_SAVE_STATE45_TRACE 0
#endif

#ifndef LOD_ENABLE_BOOT_GS_SKIP
#define LOD_ENABLE_BOOT_GS_SKIP 0
#endif

// ── PFS/audio override gates ────────────────────────────────────────
// Each override masks a real runtime/PFS-internal issue. They default
// to 1 (override active) to preserve current behavior. Set any to 0
// at build time to drop the override; the linker will then resolve
// the symbol to the weak RECOMP_FUNC version in RecompiledFuncs/.
//
// Cleanup priority (try disabling first — thinnest justification):
//   * LOD_OVERRIDE_FUNC_8009E480       — audio-chain hard no-op
//   * LOD_OVERRIDE_FUNC_8001D398       — contpak recheck force-return-2
#ifndef LOD_OVERRIDE_FUNC_8009E480
#define LOD_OVERRIDE_FUNC_8009E480 1
#endif
#ifndef LOD_OVERRIDE_FUNC_8001D398
#define LOD_OVERRIDE_FUNC_8001D398 1
#endif
#ifndef LOD_OVERRIDE_CONTPAK_INSERTED_STATUS
#define LOD_OVERRIDE_CONTPAK_INSERTED_STATUS 1
#endif
#ifndef LOD_OVERRIDE_FUNC_8001C93C
#define LOD_OVERRIDE_FUNC_8001C93C 1
#endif
#ifndef LOD_OVERRIDE_FUNC_8009F400
#define LOD_OVERRIDE_FUNC_8009F400 1
#endif

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
// Keep the generated weak symbol callable when LOD_OVERRIDE_FUNC_8001D398=1
// but LOD_OVERRIDE_CONTPAK_INSERTED_STATUS=0.
extern "C" void contpak_get_inserted_status(uint8_t* rdram, recomp_context* ctx);

#if LOD_ENABLE_SAVE_HANDLER_TRACE
static void lod_install_save_trace_wrappers(const char* reason);
#endif
#if LOD_ENABLE_SAVE_EXIT_TRACE
static void lod_install_save_exit_trace_wrappers(const char* reason);
#endif
#if LOD_ENABLE_SAVE_STATE45_TRACE
static void lod_install_save_state45_trace_wrappers(const char* reason);
#endif

// ── Map overlay table: ROM start → {full_size, overlay_id} ──────────
// Built from game's overlay table at ROM 0xB39E8 + section_table .index values.
// overlay_id = section .index + 3 (overlay_sections_by_index offset).
static const struct { uint32_t rom_start; uint32_t full_size; uint32_t overlay_id; } map_ovl_table[] = {
    { 0x0076CD00, 0x0C810,   7 }, { 0x00779510, 0x04CF0,   8 }, { 0x0077E200, 0x04F40,   9 },
    { 0x00783140, 0x02EF0,  10 }, { 0x00786030, 0x03FF0,  11 }, { 0x0078A020, 0x092B0,  12 },
    { 0x007932D0, 0x07E40,  13 }, { 0x0079B110, 0x07C60,  14 }, { 0x007A2D70, 0x03380,  15 },
    { 0x007A60F0, 0x04FA0,  16 }, { 0x007AB090, 0x033A0,  17 }, { 0x007AE430, 0x03B30,  18 },
    { 0x007B1F60, 0x02CD0,  19 }, { 0x007B4C30, 0x01DF0,  20 }, { 0x007B6A20, 0x02880,  21 },
    { 0x007B92A0, 0x00F30,  22 }, { 0x007BA1D0, 0x04700,  23 }, { 0x007BE8D0, 0x043E0,  24 },
    { 0x007C2CB0, 0x04050,  25 }, { 0x007C6D00, 0x01CA0,  26 }, { 0x007C89A0, 0x05440,  27 },
    { 0x007CDDE0, 0x01AB0,  28 }, { 0x007CF890, 0x000D0,  29 }, { 0x007CF960, 0x02EA0,  30 },
    { 0x007D2800, 0x01490,  31 }, { 0x007D3C90, 0x00790,  32 }, { 0x007D4420, 0x02700,  33 },
    { 0x007D6B20, 0x01070,  34 }, { 0x007D7B90, 0x01C00,  35 }, { 0x007D9790, 0x070C0,  36 },
    { 0x007E0850, 0x01DC0,  37 }, { 0x007E2610, 0x03040,  38 }, { 0x007E5650, 0x02340,  39 },
    { 0x007E7990, 0x07C50,  40 }, { 0x007EF5E0, 0x16690,  41 }, { 0x00805C70, 0x04880,  42 },
    { 0x0080A4F0, 0x0A900,  43 }, { 0x00814DF0, 0x06220,  44 }, { 0x0081B010, 0x03030,  45 },
    { 0x0081E040, 0x0B1E0,  46 }, { 0x00829220, 0x01470,  47 }, { 0x0082A690, 0x03CA0,  48 },
    { 0x0082E330, 0x079E0,  49 }, { 0x00835D10, 0x006A0,  50 }, { 0x008363B0, 0x00590,  51 },
    { 0x00836940, 0x00EC0,  52 }, { 0x00837800, 0x009C0,  53 },
};
static constexpr int MAP_OVL_TABLE_SIZE = 47;

extern "C" {

// ── Map overlay loader override ─────────────────────────────────────
// func_80012ED0 is the game's overlay DMA function. The game calls it to
// load map overlays from ROM to VRAM 0x802E3B70. In the recomp, the DMA
// doesn't populate RDRAM (the game's copy mechanism isn't intercepted).
// Override: copy overlay data from ROM with byte-swap + register functions.
//
// Signature: func_80012ED0(queue, ctrl_struct, rom_start, vram_dest, size, flags)
void func_80012ED0(uint8_t* rdram, recomp_context* ctx) {
    uint32_t ctrl = (uint32_t)ctx->r5;
    uint32_t rom_start = (uint32_t)ctx->r6;
    uint32_t vram_dest = (uint32_t)ctx->r7;
    uint32_t size = MEM_W(0x10, ctx->r29);  // 5th arg on caller's stack

    if (vram_dest == 0x802E3B70) {
        // Map overlay load — find entry in our table
        uint32_t rdram_dst = vram_dest - 0x80000000;
        for (int i = 0; i < MAP_OVL_TABLE_SIZE; i++) {
            if (map_ovl_table[i].rom_start == rom_start) {
                uint32_t full_size = map_ovl_table[i].full_size;
                lod_copy_overlay_data(rdram, rom_start, rdram_dst, full_size);
                load_overlay_by_id(map_ovl_table[i].overlay_id, vram_dest);
#if LOD_ENABLE_SAVE_HANDLER_TRACE
                if (rom_start == 0x007EF5E0) {
                    lod_install_save_trace_wrappers("map_ovl_34-load");
                }
#endif
#if LOD_ENABLE_SAVE_STATE45_TRACE
                if (rom_start == 0x007EF5E0) {
                    lod_install_save_state45_trace_wrappers("map_ovl_34-load");
                }
#endif

                static int load_n = 0; load_n++;
                if (load_n <= 20 || (load_n % 100) == 0)
                    fprintf(stderr, "[MAP_OVL] #%d loaded ovl_id=%d rom=0x%X size=0x%X (game requested 0x%X)\n",
                            load_n, map_ovl_table[i].overlay_id, rom_start, full_size, size);
                ctx->r2 = 0;
                return;
            }
        }
        fprintf(stderr, "[MAP_OVL] WARNING: unknown overlay rom=0x%X size=0x%X\n", rom_start, size);
    }

    // Non-map-overlay call: do the DMA via lod_copy_overlay_data if
    // the destination is in valid RDRAM range, otherwise skip.
    if (rom_start != 0 && size > 0 && size < 0x100000) {
        uint32_t rdram_dst = vram_dest & 0x1FFFFFFF;
        if (rdram_dst + size <= 0x800000) {
            lod_copy_overlay_data(rdram, rom_start, rdram_dst, size);
            // Store end address in ctrl struct if provided (matches original behavior)
            if (ctrl != 0) {
                uint32_t ctrl_phys = ctrl & 0x1FFFFFFF;
                if (ctrl_phys + 4 <= 0x800000)
                    MEM_W(0x0, ctrl) = vram_dest + size;
            }
        }
    }
    ctx->r2 = 0;
}

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

#if LOD_OVERRIDE_FUNC_8009F400
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
#endif // LOD_OVERRIDE_FUNC_8009F400

#if LOD_OVERRIDE_CONTPAK_INSERTED_STATUS
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
#endif // LOD_OVERRIDE_CONTPAK_INSERTED_STATUS

#if LOD_OVERRIDE_FUNC_8001D398
static void contpak_check_inserted_err_override_impl(uint8_t* rdram, recomp_context* ctx) {
    // Still run our contpak_get_inserted_status to update pak[] array
    contpak_get_inserted_status(rdram, ctx);
    ctx->r2 = 2;
}

// func_8001D398 (contpak recheck): override to return 2 (found).
// The recompiled version calls contpak_get_inserted_status which needs
// full PIF flow. Returning 2 directly is simpler and matches emulator behavior.
void func_8001D398(uint8_t* rdram, recomp_context* ctx) {
    contpak_check_inserted_err_override_impl(rdram, ctx);
}

// Future-regeneration alias for the same function after symbol sync.
void contpak_check_inserted_err(uint8_t* rdram, recomp_context* ctx) {
    contpak_check_inserted_err_override_impl(rdram, ctx);
}
#endif // LOD_OVERRIDE_FUNC_8001D398

#if LOD_OVERRIDE_FUNC_8001C93C
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
#endif // LOD_OVERRIDE_FUNC_8001C93C

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

#if LOD_OVERRIDE_FUNC_8009E480
// func_8009E480: on the audio task chain, NOT a PFS save function.
// Crashes without this stub because it calls PFS internals that need full setup.
void func_8009E480(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}
#endif // LOD_OVERRIDE_FUNC_8009E480

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

static inline bool lod_rdram_range_ok(uint32_t phys, uint32_t size) {
    return size <= 0x800000 && phys <= (0x800000 - size);
}

static inline uint8_t lod_rdram_u8(uint8_t* rdram, uint32_t phys) {
    return rdram[phys ^ 3];
}

static inline int16_t lod_rdram_s16(uint8_t* rdram, uint32_t phys) {
    uint16_t value = ((uint16_t)lod_rdram_u8(rdram, phys) << 8) |
                     (uint16_t)lod_rdram_u8(rdram, phys + 1);
    return (int16_t)value;
}

static inline uint32_t lod_rdram_u32(uint8_t* rdram, uint32_t phys) {
    return *(uint32_t*)(rdram + phys);
}

static int32_t lod_current_gamestate(uint8_t* rdram) {
    constexpr uint32_t GSM_PTR_PHYS = 0x0C1520;
    uint32_t gsm_addr = lod_rdram_u32(rdram, GSM_PTR_PHYS);
    if (gsm_addr == 0) {
        return 0;
    }

    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (!lod_rdram_range_ok(gsm_phys, 0x2C)) {
        return 0;
    }

    return *(int32_t*)(rdram + gsm_phys + 0x24);
}

static uint8_t lod_object_dispatch_state(uint8_t* rdram, uint32_t obj_phys) {
    if (!lod_rdram_range_ok(obj_phys, 0x10)) {
        return 255;
    }

    int16_t function_info_id = lod_rdram_s16(rdram, obj_phys + 0x0E);
    int dispatch_index = function_info_id + 1;
    if (dispatch_index < 0 || dispatch_index >= 3) {
        return 255;
    }

    return lod_rdram_u8(rdram, obj_phys + 0x09 + dispatch_index * 2);
}

static void lod_dump_save_state_context(uint8_t* rdram, int si_dma_count,
                                        const char* label, uint32_t obj_phys,
                                        const char* reason) {
    constexpr uint32_t SAVE_OUTER_TABLE_PHYS = 0x2F7170;
    constexpr uint32_t SAVE_INNER_TABLE_PHYS = 0x2F71A8;
    constexpr uint32_t GSM_PTR_PHYS = 0x0C1520;
    constexpr uint32_t PAK_UNINSERTED_PHYS = 0x0F2260;
    constexpr uint32_t PFS_STATUS_PHYS = 0x0F1F20;
    constexpr uint32_t SAVE_DISPATCH_FLAGS_PHYS = 0x1CABC8; // 0x801D0000 - 0x5438
    constexpr uint32_t STATE3_FLAGS_PHYS = 0x1CAB18; // 0x801D0000 - 0x54E8
    constexpr uint32_t STATE3_CTX_PTR_PHYS = 0x1CAC20; // 0x801D0000 - 0x53E0
    constexpr uint32_t MAP_OVL_34_FLAG_6FE4_PHYS = 0x2F6FE4; // 0x802F6FE4

    if (!lod_rdram_range_ok(obj_phys, 0x74)) {
        fprintf(stderr, "[G4-001] %s %s #%d obj=0x%08X(invalid)\n",
            label, reason, si_dma_count, obj_phys | 0x80000000);
        return;
    }

    uint32_t gsm_addr = lod_rdram_u32(rdram, GSM_PTR_PHYS);
    int32_t cur_gs = 0;
    if (gsm_addr != 0) {
        uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
        if (lod_rdram_range_ok(gsm_phys, 0x28)) {
            cur_gs = *(int32_t*)(rdram + gsm_phys + 0x24);
        }
    }

    uint8_t obj_state = lod_rdram_u8(rdram, obj_phys + 0x09);
    int16_t function_info_id = lod_rdram_s16(rdram, obj_phys + 0x0E);
    uint8_t sched_08 = lod_rdram_u8(rdram, obj_phys + 0x08);
    uint8_t sched_09 = lod_rdram_u8(rdram, obj_phys + 0x09);
    uint8_t sched_0a = lod_rdram_u8(rdram, obj_phys + 0x0A);
    uint8_t sched_0b = lod_rdram_u8(rdram, obj_phys + 0x0B);
    uint8_t sched_0c = lod_rdram_u8(rdram, obj_phys + 0x0C);
    uint8_t sched_0d = lod_rdram_u8(rdram, obj_phys + 0x0D);
    uint8_t dispatch_state = lod_object_dispatch_state(rdram, obj_phys);
    uint32_t outer_handler = 0;
    uint32_t inner_handler = 0;
    if (dispatch_state < 16) {
        if (lod_rdram_range_ok(SAVE_OUTER_TABLE_PHYS + dispatch_state * 4, 4)) {
            outer_handler = lod_rdram_u32(rdram, SAVE_OUTER_TABLE_PHYS + dispatch_state * 4);
        }
        if (lod_rdram_range_ok(SAVE_INNER_TABLE_PHYS + dispatch_state * 4, 4)) {
            inner_handler = lod_rdram_u32(rdram, SAVE_INNER_TABLE_PHYS + dispatch_state * 4);
        }
    }

    uint32_t fig0 = lod_rdram_u32(rdram, obj_phys + 0x24);
    uint32_t alloc_base = lod_rdram_u32(rdram, obj_phys + 0x34);
    uint32_t alloc_base_phys = alloc_base & 0x1FFFFFFF;
    bool alloc_ok = alloc_base != 0 && lod_rdram_range_ok(alloc_base_phys, 0x54);
    int16_t base_40 = 0;
    int16_t base_48 = 0;
    int16_t work_3e = 0; // after handler's unconditional s0 += 8, this is base + 0x46
    int16_t work_40 = 0; // after s0 += 8, this is base + 0x48
    uint32_t work_48_ptr = 0; // after s0 += 8, this is base + 0x50
    if (alloc_ok) {
        base_40 = lod_rdram_s16(rdram, alloc_base_phys + 0x40);
        base_48 = lod_rdram_s16(rdram, alloc_base_phys + 0x48);
        work_3e = lod_rdram_s16(rdram, alloc_base_phys + 0x46);
        work_40 = lod_rdram_s16(rdram, alloc_base_phys + 0x48);
        work_48_ptr = lod_rdram_u32(rdram, alloc_base_phys + 0x50);
    }

    uint32_t flags = lod_rdram_u32(rdram, STATE3_FLAGS_PHYS);
    uint32_t state3_ctx = lod_rdram_u32(rdram, STATE3_CTX_PTR_PHYS);
    uint32_t ctx_14 = 0;
    uint32_t ctx_14_14 = 0;
    uint32_t ctx_14_14_14 = 0;
    uint32_t ctx_phys = state3_ctx & 0x1FFFFFFF;
    if (state3_ctx != 0 && lod_rdram_range_ok(ctx_phys, 0x18)) {
        ctx_14 = lod_rdram_u32(rdram, ctx_phys + 0x14);
        uint32_t ctx_14_phys = ctx_14 & 0x1FFFFFFF;
        if (ctx_14 != 0 && lod_rdram_range_ok(ctx_14_phys, 0x18)) {
            ctx_14_14 = lod_rdram_u32(rdram, ctx_14_phys + 0x14);
            uint32_t ctx_14_14_phys = ctx_14_14 & 0x1FFFFFFF;
            if (ctx_14_14 != 0 && lod_rdram_range_ok(ctx_14_14_phys, 0x18)) {
                ctx_14_14_14 = lod_rdram_u32(rdram, ctx_14_14_phys + 0x14);
            }
        }
    }

    uint32_t child = lod_rdram_u32(rdram, obj_phys + 0x1C);
    uint32_t dispatch = lod_rdram_u32(rdram, obj_phys + 0x10);
    uint32_t alloc15 = lod_rdram_u32(rdram, obj_phys + 0x70);
    uint8_t pak0 = lod_rdram_u8(rdram, PAK_UNINSERTED_PHYS);
    uint32_t pfs_status = lod_rdram_u32(rdram, PFS_STATUS_PHYS);
    uint32_t dispatch_flags = lod_rdram_u32(rdram, SAVE_DISPATCH_FLAGS_PHYS);
    int16_t map_flag_6fe4 = lod_rdram_s16(rdram, MAP_OVL_34_FLAG_6FE4_PHYS);

    const char* helper_path = alloc_ok && work_40 == 2 ? "802F5584" : "802F0014";
    const char* source_path = alloc_ok && base_48 == 2 ? "nested_ctx" : "direct_ctx";
    fprintf(stderr,
        "[G4-001] %s %s #%d gs=%d obj=0x%08X state09=%u func_id=%d sched=%02X,%02X,%02X,%02X,%02X,%02X dispatch_state=%u outer=0x%08X inner=0x%08X "
        "dispatch=0x%08X child=0x%08X fig0=0x%08X alloc15=0x%08X "
        "alloc_base=0x%08X%s base40=%d base48=%d work3E=%d work40=%d work48ptr=0x%08X helper=%s source=%s "
        "disp_flags=0x%08X map6FE4=%d flags=0x%08X bit10=%d bit40=%d "
        "ctx=0x%08X ctx14=0x%08X ctx1414=0x%08X ctx141414=0x%08X "
        "pak0=%u pfs_status=0x%08X\n",
        label, reason, si_dma_count, cur_gs, obj_phys | 0x80000000, obj_state,
        function_info_id, sched_08, sched_09, sched_0a, sched_0b, sched_0c, sched_0d,
        dispatch_state, outer_handler, inner_handler,
        dispatch, child, fig0, alloc15,
        alloc_base, alloc_ok ? "" : "(invalid)", base_40, base_48, work_3e, work_40, work_48_ptr,
        helper_path, source_path,
        dispatch_flags, map_flag_6fe4,
        flags, (flags & 0x10) != 0, (flags & 0x40) != 0,
        state3_ctx, ctx_14, ctx_14_14, ctx_14_14_14,
        pak0, pfs_status);
}

static void lod_dump_save_object_window(uint8_t* rdram, int si_dma_count, const char* reason) {
    constexpr uint32_t SAVE_OUTER_TABLE_PHYS = 0x2F7170;
    constexpr uint32_t START_PHYS = 0x31AFA4;
    constexpr uint32_t OBJECT_SIZE = 0x74;

    fprintf(stderr, "[G4-001] object-window %s #%d\n", reason, si_dma_count);
    for (int i = 0; i < 14; i++) {
        uint32_t obj_phys = START_PHYS + i * OBJECT_SIZE;
        if (!lod_rdram_range_ok(obj_phys, OBJECT_SIZE)) {
            continue;
        }

        uint8_t state09 = lod_rdram_u8(rdram, obj_phys + 0x09);
        int16_t function_info_id = lod_rdram_s16(rdram, obj_phys + 0x0E);
        uint8_t dispatch_state = lod_object_dispatch_state(rdram, obj_phys);
        uint32_t outer_handler = 0;
        if (dispatch_state < 16 && lod_rdram_range_ok(SAVE_OUTER_TABLE_PHYS + dispatch_state * 4, 4)) {
            outer_handler = lod_rdram_u32(rdram, SAVE_OUTER_TABLE_PHYS + dispatch_state * 4);
        }

        uint32_t destroy = lod_rdram_u32(rdram, obj_phys + 0x10);
        uint32_t parent = lod_rdram_u32(rdram, obj_phys + 0x14);
        uint32_t child = lod_rdram_u32(rdram, obj_phys + 0x1C);
        uint32_t fig0 = lod_rdram_u32(rdram, obj_phys + 0x24);
        uint32_t alloc0 = lod_rdram_u32(rdram, obj_phys + 0x34);
        uint32_t alloc1 = lod_rdram_u32(rdram, obj_phys + 0x38);
        uint32_t alloc15 = lod_rdram_u32(rdram, obj_phys + 0x70);

        if (state09 != 0 || dispatch_state != 255 || destroy || parent || child || fig0 || alloc0 || alloc1 || alloc15) {
            fprintf(stderr,
                "[G4-001]   obj[%02d]=0x%08X state09=%u func_id=%d dispatch_state=%u outer=0x%08X "
                "destroy=0x%08X parent=0x%08X child=0x%08X fig0=0x%08X alloc0=0x%08X alloc1=0x%08X alloc15=0x%08X\n",
                i, obj_phys | 0x80000000, state09, function_info_id, dispatch_state, outer_handler,
                destroy, parent, child, fig0, alloc0, alloc1, alloc15);
        }
    }
}

#if LOD_ENABLE_SAVE_HANDLER_TRACE
static recomp_func_t* lod_orig_save_outer_dispatch = nullptr;
static recomp_func_t* lod_orig_save_schedule_dispatch = nullptr;
static recomp_func_t* lod_orig_save_outer_state3_update = nullptr;
static recomp_func_t* lod_orig_save_inner_state3_update = nullptr;

static void lod_trace_save_handler_entry(uint8_t* rdram, recomp_context* ctx,
                                         const char* label, uint32_t vram, int* counter) {
    (*counter)++;
    uint32_t obj_addr = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj_addr & 0x1FFFFFFF;
    if (*counter <= 24 || (*counter % 120) == 0) {
        fprintf(stderr, "[G4-001] handler-entry %s #%d vram=0x%08X a0=0x%08X ra=0x%08X sp=0x%08X\n",
            label, *counter, vram, obj_addr, (uint32_t)ctx->r31, (uint32_t)ctx->r29);
        lod_dump_save_state_context(rdram, *counter, label, obj_phys, "handler-entry");
    }
}

static void lod_trace_save_outer_dispatch(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_handler_entry(rdram, ctx, "outer_dispatch", 0x802EBAF0, &count);
    if (lod_orig_save_outer_dispatch != nullptr) {
        lod_orig_save_outer_dispatch(rdram, ctx);
    }
}

static void lod_trace_save_schedule_dispatch(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_handler_entry(rdram, ctx, "schedule_dispatch", 0x802ECA84, &count);
    if (lod_orig_save_schedule_dispatch != nullptr) {
        lod_orig_save_schedule_dispatch(rdram, ctx);
    }
}

static void lod_trace_save_outer_state3_update(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_handler_entry(rdram, ctx, "outer_state3", 0x802ECF4C, &count);
    if (lod_orig_save_outer_state3_update != nullptr) {
        lod_orig_save_outer_state3_update(rdram, ctx);
    }
}

static void lod_trace_save_inner_state3_update(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_handler_entry(rdram, ctx, "inner_state3", 0x802ED804, &count);
    if (lod_orig_save_inner_state3_update != nullptr) {
        lod_orig_save_inner_state3_update(rdram, ctx);
    }
}

static void lod_install_save_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                           recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

static void lod_install_save_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    lod_install_save_trace_wrapper(0x802EBAF0, lod_trace_save_outer_dispatch,
        &lod_orig_save_outer_dispatch);
    lod_install_save_trace_wrapper(0x802ECA84, lod_trace_save_schedule_dispatch,
        &lod_orig_save_schedule_dispatch);
    lod_install_save_trace_wrapper(0x802ECF4C, lod_trace_save_outer_state3_update,
        &lod_orig_save_outer_state3_update);
    lod_install_save_trace_wrapper(0x802ED804, lod_trace_save_inner_state3_update,
        &lod_orig_save_inner_state3_update);

    if (install_count <= 4) {
        fprintf(stderr,
            "[G4-001] installed save trace wrappers #%d reason=%s originals outer=%p schedule=%p outer3=%p inner3=%p\n",
            install_count, reason,
            (void*)lod_orig_save_outer_dispatch,
            (void*)lod_orig_save_schedule_dispatch,
            (void*)lod_orig_save_outer_state3_update,
            (void*)lod_orig_save_inner_state3_update);
    }
}

#endif

#if LOD_ENABLE_SAVE_STATE45_TRACE
static recomp_func_t* lod_orig_state45_save_outer_dispatch = nullptr;
static recomp_func_t* lod_orig_state45_save_schedule_dispatch = nullptr;
static recomp_func_t* lod_orig_save_outer_state4_bridge = nullptr;
static recomp_func_t* lod_orig_save_inner_state4_init = nullptr;
static recomp_func_t* lod_orig_save_inner_state5_update = nullptr;
static recomp_func_t* lod_orig_state45_curLevel_goToFunc = nullptr;
static recomp_func_t* lod_orig_state45_curLevel_goToNextFuncAndClearTimer = nullptr;

struct LodSaveState45Snapshot {
    int32_t gs = 0;
    uint32_t obj_phys = 0;
    bool obj_ok = false;
    uint8_t state09 = 255;
    int16_t func_id = 0;
    uint8_t dispatch_state = 255;
    uint8_t sched[6] = {};
    uint32_t obj_38 = 0;
    uint32_t obj_3c = 0;
    uint32_t obj_40 = 0;
    uint32_t obj_44 = 0;
    uint32_t obj_48 = 0;
    uint32_t data = 0;
    bool data_ok = false;
    int16_t data_3e = 0;
    int16_t data_40 = 0;
    int16_t data_46 = 0;
    int16_t data_48_s16 = 0;
    uint32_t data_1c = 0;
    uint32_t data_20 = 0;
    uint32_t data_24 = 0;
    uint32_t data_44 = 0;
    uint32_t data_48 = 0;
    uint32_t data_50 = 0;
    uint32_t data_54 = 0;
    uint32_t flags_cab18 = 0;
    uint32_t flags_cabc8 = 0;
    int16_t map_6fe4 = 0;
    uint8_t pak0 = 0;
    uint32_t pfs_status = 0;
};

static bool lod_save_state45_object_interesting(uint32_t obj_phys) {
    return obj_phys >= 0x31A000 && obj_phys < 0x31D000;
}

static LodSaveState45Snapshot lod_capture_save_state45_snapshot(uint8_t* rdram, uint32_t obj_phys) {
    constexpr uint32_t FLAGS_CAB18_PHYS = 0x1CAB18; // 0x801CAB18
    constexpr uint32_t FLAGS_CABC8_PHYS = 0x1CABC8; // 0x801CABC8
    constexpr uint32_t MAP_OVL_34_FLAG_6FE4_PHYS = 0x2F6FE4; // 0x802F6FE4
    constexpr uint32_t PAK_UNINSERTED_PHYS = 0x0F2260;
    constexpr uint32_t PFS_STATUS_PHYS = 0x0F1F20;

    LodSaveState45Snapshot s;
    s.gs = lod_current_gamestate(rdram);
    s.obj_phys = obj_phys;
    s.obj_ok = lod_rdram_range_ok(obj_phys, 0x74);
    s.flags_cab18 = lod_rdram_u32(rdram, FLAGS_CAB18_PHYS);
    s.flags_cabc8 = lod_rdram_u32(rdram, FLAGS_CABC8_PHYS);
    s.map_6fe4 = lod_rdram_s16(rdram, MAP_OVL_34_FLAG_6FE4_PHYS);
    s.pak0 = lod_rdram_u8(rdram, PAK_UNINSERTED_PHYS);
    s.pfs_status = lod_rdram_u32(rdram, PFS_STATUS_PHYS);

    if (!s.obj_ok) {
        return s;
    }

    s.state09 = lod_rdram_u8(rdram, obj_phys + 0x09);
    s.func_id = lod_rdram_s16(rdram, obj_phys + 0x0E);
    s.dispatch_state = lod_object_dispatch_state(rdram, obj_phys);
    for (int i = 0; i < 6; i++) {
        s.sched[i] = lod_rdram_u8(rdram, obj_phys + 0x08 + i);
    }

    s.data = lod_rdram_u32(rdram, obj_phys + 0x34);
    s.obj_38 = lod_rdram_u32(rdram, obj_phys + 0x38);
    s.obj_3c = lod_rdram_u32(rdram, obj_phys + 0x3C);
    s.obj_40 = lod_rdram_u32(rdram, obj_phys + 0x40);
    s.obj_44 = lod_rdram_u32(rdram, obj_phys + 0x44);
    s.obj_48 = lod_rdram_u32(rdram, obj_phys + 0x48);
    uint32_t data_phys = s.data & 0x1FFFFFFF;
    s.data_ok = s.data != 0 && lod_rdram_range_ok(data_phys, 0x58);
    if (s.data_ok) {
        s.data_1c = lod_rdram_u32(rdram, data_phys + 0x1C);
        s.data_20 = lod_rdram_u32(rdram, data_phys + 0x20);
        s.data_24 = lod_rdram_u32(rdram, data_phys + 0x24);
        s.data_3e = lod_rdram_s16(rdram, data_phys + 0x3E);
        s.data_40 = lod_rdram_s16(rdram, data_phys + 0x40);
        s.data_44 = lod_rdram_u32(rdram, data_phys + 0x44);
        s.data_46 = lod_rdram_s16(rdram, data_phys + 0x46);
        s.data_48_s16 = lod_rdram_s16(rdram, data_phys + 0x48);
        s.data_48 = lod_rdram_u32(rdram, data_phys + 0x48);
        s.data_50 = lod_rdram_u32(rdram, data_phys + 0x50);
        s.data_54 = lod_rdram_u32(rdram, data_phys + 0x54);
    }

    return s;
}

static void lod_print_save_state45_snapshot(const char* label, const char* phase, int count,
                                            uint32_t vram, const LodSaveState45Snapshot& s,
                                            uint32_t a0, uint32_t a1, int32_t a2,
                                            uint32_t ra) {
    if (!s.obj_ok) {
        fprintf(stderr,
            "[SAVE45] %s-%s #%d vram=0x%08X gs=%d obj=0x%08X(invalid) "
            "a0=0x%08X a1=0x%08X a2=%d ra=0x%08X flagsCAB18=0x%08X flagsCABC8=0x%08X map6FE4=%d\n",
            label, phase, count, vram, s.gs, s.obj_phys | 0x80000000,
            a0, a1, a2, ra, s.flags_cab18, s.flags_cabc8, s.map_6fe4);
        return;
    }

    fprintf(stderr,
        "[SAVE45] %s-%s #%d vram=0x%08X gs=%d obj=0x%08X state09=%u func_id=%d "
        "dispatch_state=%u sched=%02X,%02X,%02X,%02X,%02X,%02X "
        "data=0x%08X%s obj38=0x%08X obj3C=0x%08X obj40=0x%08X obj44=0x%08X obj48=0x%08X "
        "d1C=0x%08X d20=0x%08X d24=0x%08X d3E=%d d40=%d "
        "d44=0x%08X d46=%d d48h=%d d48=0x%08X d50=0x%08X d54=0x%08X "
        "flagsCAB18=0x%08X flagsCABC8=0x%08X bit10=%d bit40=%d map6FE4=%d "
        "pak0=%u pfs=0x%08X a0=0x%08X a1=0x%08X a2=%d ra=0x%08X\n",
        label, phase, count, vram, s.gs, s.obj_phys | 0x80000000,
        s.state09, s.func_id, s.dispatch_state,
        s.sched[0], s.sched[1], s.sched[2], s.sched[3], s.sched[4], s.sched[5],
        s.data, s.data_ok ? "" : "(invalid)",
        s.obj_38, s.obj_3c, s.obj_40, s.obj_44, s.obj_48,
        s.data_1c, s.data_20, s.data_24, s.data_3e, s.data_40,
        s.data_44, s.data_46, s.data_48_s16, s.data_48, s.data_50, s.data_54,
        s.flags_cab18, s.flags_cabc8, (s.flags_cabc8 & 0x10) != 0, (s.flags_cabc8 & 0x40) != 0,
        s.map_6fe4, s.pak0, s.pfs_status, a0, a1, a2, ra);
}

static void lod_trace_save_state45_handler(uint8_t* rdram, recomp_context* ctx,
                                           const char* label, uint32_t vram,
                                           int* counter, recomp_func_t* original) {
    (*counter)++;
    uint32_t obj_addr = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj_addr & 0x1FFFFFFF;
    LodSaveState45Snapshot before = lod_capture_save_state45_snapshot(rdram, obj_phys);
    bool should_log = *counter <= 16 ||
        (before.gs == 4 && before.obj_ok && lod_save_state45_object_interesting(obj_phys));

    if (should_log) {
        lod_print_save_state45_snapshot(label, "enter", *counter, vram, before,
            obj_addr, (uint32_t)ctx->r5, (int32_t)ctx->r6, (uint32_t)ctx->r31);
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }

    if (should_log) {
        LodSaveState45Snapshot after = lod_capture_save_state45_snapshot(rdram, obj_phys);
        lod_print_save_state45_snapshot(label, "exit", *counter, vram, after,
            obj_addr, (uint32_t)ctx->r5, (int32_t)ctx->r6, (uint32_t)ctx->r31);
    }
}

static void lod_print_save_state45_host_caller(const char* label, const char* phase,
                                               int counter, void* host_return) {
    const char* symbol = "(unknown)";
    const char* image = "(unknown)";
    uintptr_t symbol_base = 0;
#if defined(__APPLE__) || defined(__linux__)
    Dl_info info = {};
    if (host_return != nullptr && dladdr(host_return, &info) != 0) {
        if (info.dli_sname != nullptr) {
            symbol = info.dli_sname;
        }
        if (info.dli_fname != nullptr) {
            image = info.dli_fname;
        }
        symbol_base = (uintptr_t)info.dli_saddr;
    }
#endif
    uintptr_t addr = (uintptr_t)host_return;
    uintptr_t offset = (symbol_base != 0 && addr >= symbol_base) ? (addr - symbol_base) : 0;
    fprintf(stderr,
        "[SAVE45-HOST] %s-%s #%d host_return=%p symbol=%s+0x%zX image=%s\n",
        label, phase, counter, host_return, symbol, (size_t)offset, image);
}

static void lod_trace_save_state45_schedule(uint8_t* rdram, recomp_context* ctx,
                                            const char* label, uint32_t vram,
                                            int* counter, recomp_func_t* original,
                                            bool has_target, void* host_return) {
    (*counter)++;
    uint32_t schedule_addr = (uint32_t)ctx->r4;
    uint32_t obj_phys = (schedule_addr - 0x08) & 0x1FFFFFFF;
    int32_t target = has_target ? (int32_t)(int16_t)ctx->r6 : -1;
    LodSaveState45Snapshot before = lod_capture_save_state45_snapshot(rdram, obj_phys);
    bool should_log = *counter <= 24 ||
        (before.gs == 4 && before.obj_ok && lod_save_state45_object_interesting(obj_phys));

    if (should_log) {
        lod_print_save_state45_snapshot(label, "enter", *counter, vram, before,
            schedule_addr, (uint32_t)ctx->r5, target, (uint32_t)ctx->r31);
        lod_print_save_state45_host_caller(label, "enter", *counter, host_return);
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }

    if (should_log) {
        LodSaveState45Snapshot after = lod_capture_save_state45_snapshot(rdram, obj_phys);
        lod_print_save_state45_snapshot(label, "exit", *counter, vram, after,
            schedule_addr, (uint32_t)ctx->r5, target, (uint32_t)ctx->r31);
        lod_print_save_state45_host_caller(label, "exit", *counter, host_return);
    }
}

static void lod_trace_state45_save_outer_dispatch(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_state45_handler(rdram, ctx, "save_screen_outer_dispatch",
        0x802EBAF0, &count, lod_orig_state45_save_outer_dispatch);
}

static void lod_trace_state45_save_schedule_dispatch(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_state45_handler(rdram, ctx, "save_screen_schedule_dispatch",
        0x802ECA84, &count, lod_orig_state45_save_schedule_dispatch);
}

static void lod_trace_save_outer_state4_bridge(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_state45_handler(rdram, ctx, "outer_state4_bridge",
        0x802EDBD8, &count, lod_orig_save_outer_state4_bridge);
}

static void lod_trace_save_inner_state4_init(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_state45_handler(rdram, ctx, "inner_state4_init",
        0x802ED630, &count, lod_orig_save_inner_state4_init);
}

static void lod_trace_save_inner_state5_update(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_state45_handler(rdram, ctx, "inner_state5_update",
        0x802ED6C4, &count, lod_orig_save_inner_state5_update);
}

static void lod_trace_state45_curLevel_goToFunc(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_state45_schedule(rdram, ctx, "object_curLevel_goToFunc",
        0x80001E30, &count, lod_orig_state45_curLevel_goToFunc, true,
        __builtin_return_address(0));
}

static void lod_trace_state45_curLevel_goToNextFuncAndClearTimer(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_state45_schedule(rdram, ctx, "object_curLevel_goToNextFuncAndClearTimer",
        0x80001CE8, &count, lod_orig_state45_curLevel_goToNextFuncAndClearTimer, false,
        __builtin_return_address(0));
}

static void lod_install_save_state45_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                                   recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

static void lod_install_save_state45_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    lod_install_save_state45_trace_wrapper(0x802EBAF0, lod_trace_state45_save_outer_dispatch,
        &lod_orig_state45_save_outer_dispatch);
    lod_install_save_state45_trace_wrapper(0x802ECA84, lod_trace_state45_save_schedule_dispatch,
        &lod_orig_state45_save_schedule_dispatch);
    lod_install_save_state45_trace_wrapper(0x802EDBD8, lod_trace_save_outer_state4_bridge,
        &lod_orig_save_outer_state4_bridge);
    lod_install_save_state45_trace_wrapper(0x802ED630, lod_trace_save_inner_state4_init,
        &lod_orig_save_inner_state4_init);
    lod_install_save_state45_trace_wrapper(0x802ED6C4, lod_trace_save_inner_state5_update,
        &lod_orig_save_inner_state5_update);
    lod_install_save_state45_trace_wrapper(0x80001E30, lod_trace_state45_curLevel_goToFunc,
        &lod_orig_state45_curLevel_goToFunc);
    lod_install_save_state45_trace_wrapper(0x80001CE8, lod_trace_state45_curLevel_goToNextFuncAndClearTimer,
        &lod_orig_state45_curLevel_goToNextFuncAndClearTimer);

    if (install_count <= 4) {
        fprintf(stderr,
            "[SAVE45] installed state4/5 trace wrappers #%d reason=%s originals "
            "outer=%p schedule=%p outer4=%p inner4=%p inner5=%p goToFunc=%p nextClear=%p\n",
            install_count, reason,
            (void*)lod_orig_state45_save_outer_dispatch,
            (void*)lod_orig_state45_save_schedule_dispatch,
            (void*)lod_orig_save_outer_state4_bridge,
            (void*)lod_orig_save_inner_state4_init,
            (void*)lod_orig_save_inner_state5_update,
            (void*)lod_orig_state45_curLevel_goToFunc,
            (void*)lod_orig_state45_curLevel_goToNextFuncAndClearTimer);
    }
}

#endif

#if LOD_ENABLE_SAVE_EXIT_TRACE
static recomp_func_t* lod_orig_object_curLevel_goToNextFunc = nullptr;
static recomp_func_t* lod_orig_object_curLevel_goToNextFuncAndClearTimer = nullptr;
static recomp_func_t* lod_orig_func_8001ACB0 = nullptr;
static recomp_func_t* lod_orig_func_8001AF54 = nullptr;
static recomp_func_t* lod_orig_func_8001AF90 = nullptr;
static recomp_func_t* lod_orig_func_8001AFF0 = nullptr;
static recomp_func_t* lod_orig_func_8001B0B4 = nullptr;

static bool lod_save_trace_object_interesting(uint32_t obj_phys) {
    return obj_phys >= 0x31A000 && obj_phys < 0x31D000;
}

static void lod_trace_save_schedule_advance(uint8_t* rdram, recomp_context* ctx,
                                            const char* label, uint32_t vram,
                                            int* counter, recomp_func_t* original) {
    uint32_t schedule_addr = (uint32_t)ctx->r4;
    uint32_t function_id_addr = (uint32_t)ctx->r5;
    uint32_t obj_phys = (schedule_addr - 0x08) & 0x1FFFFFFF;
    int32_t gs = lod_current_gamestate(rdram);
    bool obj_ok = lod_rdram_range_ok(obj_phys, 0x74);
    bool should_log = gs == 4 && obj_ok && lod_save_trace_object_interesting(obj_phys);

    uint8_t before_dispatch = 255;
    int16_t before_func_id = 0;
    uint8_t before_sched[6] = {};
    if (should_log) {
        (*counter)++;
        before_dispatch = lod_object_dispatch_state(rdram, obj_phys);
        before_func_id = lod_rdram_s16(rdram, obj_phys + 0x0E);
        for (int i = 0; i < 6; i++) {
            before_sched[i] = lod_rdram_u8(rdram, obj_phys + 0x08 + i);
        }

        if (*counter <= 80 || (*counter % 240) == 0) {
            fprintf(stderr,
                "[SAVE-EXIT] state-advance-enter %s #%d vram=0x%08X gs=%d obj=0x%08X "
                "a0=0x%08X a1=0x%08X func_id=%d dispatch_state=%u "
                "sched=%02X,%02X,%02X,%02X,%02X,%02X ra=0x%08X\n",
                label, *counter, vram, gs, obj_phys | 0x80000000,
                schedule_addr, function_id_addr, before_func_id, before_dispatch,
                before_sched[0], before_sched[1], before_sched[2],
                before_sched[3], before_sched[4], before_sched[5],
                (uint32_t)ctx->r31);
        }
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }

    if (should_log && (*counter <= 80 || (*counter % 240) == 0)) {
        uint8_t after_dispatch = lod_object_dispatch_state(rdram, obj_phys);
        int16_t after_func_id = lod_rdram_s16(rdram, obj_phys + 0x0E);
        uint8_t after_sched[6] = {};
        for (int i = 0; i < 6; i++) {
            after_sched[i] = lod_rdram_u8(rdram, obj_phys + 0x08 + i);
        }

        fprintf(stderr,
            "[SAVE-EXIT] state-advance-exit  %s #%d gs=%d obj=0x%08X "
            "func_id=%d->%d dispatch_state=%u->%u "
            "sched=%02X,%02X,%02X,%02X,%02X,%02X -> %02X,%02X,%02X,%02X,%02X,%02X\n",
            label, *counter, lod_current_gamestate(rdram), obj_phys | 0x80000000,
            before_func_id, after_func_id, before_dispatch, after_dispatch,
            before_sched[0], before_sched[1], before_sched[2],
            before_sched[3], before_sched[4], before_sched[5],
            after_sched[0], after_sched[1], after_sched[2],
            after_sched[3], after_sched[4], after_sched[5]);
    }
}

static void lod_trace_save_table_handler(uint8_t* rdram, recomp_context* ctx,
                                         const char* label, uint32_t vram,
                                         int* counter, recomp_func_t* original) {
    uint32_t obj_addr = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj_addr & 0x1FFFFFFF;
    int32_t gs = lod_current_gamestate(rdram);
    bool obj_ok = lod_rdram_range_ok(obj_phys, 0x74);
    (*counter)++;
    bool should_log = *counter <= 24 ||
        (gs == 4 && obj_ok && lod_save_trace_object_interesting(obj_phys));

    if (should_log) {
        if (*counter <= 80 || (*counter % 240) == 0) {
            fprintf(stderr,
                "[SAVE-EXIT] table-handler-enter %s #%d vram=0x%08X gs=%d obj=0x%08X "
                "%sdispatch_state=%u func_id=%d a0=0x%08X ra=0x%08X\n",
                label, *counter, vram, gs, obj_phys | 0x80000000,
                obj_ok ? "" : "(invalid) ",
                obj_ok ? lod_object_dispatch_state(rdram, obj_phys) : 255,
                obj_ok ? lod_rdram_s16(rdram, obj_phys + 0x0E) : 0,
                obj_addr, (uint32_t)ctx->r31);
            if (obj_ok) {
                lod_dump_save_state_context(rdram, *counter, label, obj_phys, "table-handler-enter");
            }
        }
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }
}

static void lod_trace_save_exit_to_gs3_candidate(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    uint32_t obj_addr = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj_addr & 0x1FFFFFFF;
    int32_t before_gs = lod_current_gamestate(rdram);
    bool obj_ok = lod_rdram_range_ok(obj_phys, 0x74);
    count++;
    bool should_log = count <= 12 || (before_gs == 4 && obj_ok);

    if (should_log) {
        fprintf(stderr,
            "[SAVE-EXIT] exit-candidate-enter save_exit_to_gs3 #%d vram=0x8001AF54 gs=%d "
            "obj=0x%08X %sdispatch_state=%u func_id=%d a0=0x%08X ra=0x%08X\n",
            count, before_gs, obj_phys | 0x80000000,
            obj_ok ? "" : "(invalid) ",
            obj_ok ? lod_object_dispatch_state(rdram, obj_phys) : 255,
            obj_ok ? lod_rdram_s16(rdram, obj_phys + 0x0E) : 0,
            obj_addr, (uint32_t)ctx->r31);
        if (obj_ok) {
            lod_dump_save_state_context(rdram, count, "save_exit_to_gs3", obj_phys, "before");
        }
    }

    if (lod_orig_func_8001AF54 != nullptr) {
        lod_orig_func_8001AF54(rdram, ctx);
    }

    if (should_log) {
        fprintf(stderr,
            "[SAVE-EXIT] exit-candidate-exit  save_exit_to_gs3 #%d gs=%d->%d obj=0x%08X "
            "%sdispatch_state=%u func_id=%d\n",
            count, before_gs, lod_current_gamestate(rdram), obj_phys | 0x80000000,
            obj_ok ? "" : "(invalid) ",
            obj_ok ? lod_object_dispatch_state(rdram, obj_phys) : 255,
            obj_ok ? lod_rdram_s16(rdram, obj_phys + 0x0E) : 0);
        if (obj_ok) {
            lod_dump_save_state_context(rdram, count, "save_exit_to_gs3", obj_phys, "after");
        }
    }
}

static void lod_trace_object_curLevel_goToNextFunc(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_schedule_advance(rdram, ctx, "object_curLevel_goToNextFunc",
        0x80001C20, &count, lod_orig_object_curLevel_goToNextFunc);
}

static void lod_trace_object_curLevel_goToNextFuncAndClearTimer(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_schedule_advance(rdram, ctx, "object_curLevel_goToNextFuncAndClearTimer",
        0x80001CE8, &count, lod_orig_object_curLevel_goToNextFuncAndClearTimer);
}

static void lod_trace_func_8001ACB0(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_table_handler(rdram, ctx, "save_state_table_8001ACB0",
        0x8001ACB0, &count, lod_orig_func_8001ACB0);
}

static void lod_trace_func_8001AF90(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_table_handler(rdram, ctx, "save_state_table_8001AF90",
        0x8001AF90, &count, lod_orig_func_8001AF90);
}

static void lod_trace_func_8001AFF0(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_table_handler(rdram, ctx, "save_state_table_8001AFF0",
        0x8001AFF0, &count, lod_orig_func_8001AFF0);
}

static void lod_trace_func_8001B0B4(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_save_table_handler(rdram, ctx, "save_state_table_8001B0B4",
        0x8001B0B4, &count, lod_orig_func_8001B0B4);
}

static void lod_install_save_exit_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                                recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

static void lod_install_save_exit_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    lod_install_save_exit_trace_wrapper(0x80001C20, lod_trace_object_curLevel_goToNextFunc,
        &lod_orig_object_curLevel_goToNextFunc);
    lod_install_save_exit_trace_wrapper(0x80001CE8, lod_trace_object_curLevel_goToNextFuncAndClearTimer,
        &lod_orig_object_curLevel_goToNextFuncAndClearTimer);
    lod_install_save_exit_trace_wrapper(0x8001ACB0, lod_trace_func_8001ACB0,
        &lod_orig_func_8001ACB0);
    lod_install_save_exit_trace_wrapper(0x8001AF54, lod_trace_save_exit_to_gs3_candidate,
        &lod_orig_func_8001AF54);
    lod_install_save_exit_trace_wrapper(0x8001AF90, lod_trace_func_8001AF90,
        &lod_orig_func_8001AF90);
    lod_install_save_exit_trace_wrapper(0x8001AFF0, lod_trace_func_8001AFF0,
        &lod_orig_func_8001AFF0);
    lod_install_save_exit_trace_wrapper(0x8001B0B4, lod_trace_func_8001B0B4,
        &lod_orig_func_8001B0B4);

    if (install_count <= 4) {
        fprintf(stderr,
            "[SAVE-EXIT] installed save exit trace wrappers #%d reason=%s originals "
            "cur_next=%p cur_next_clear=%p acb0=%p af54=%p af90=%p aff0=%p b0b4=%p\n",
            install_count, reason,
            (void*)lod_orig_object_curLevel_goToNextFunc,
            (void*)lod_orig_object_curLevel_goToNextFuncAndClearTimer,
            (void*)lod_orig_func_8001ACB0,
            (void*)lod_orig_func_8001AF54,
            (void*)lod_orig_func_8001AF90,
            (void*)lod_orig_func_8001AFF0,
            (void*)lod_orig_func_8001B0B4);
    }
}

#endif

void __osSiRawStartDma_recomp(uint8_t* rdram, recomp_context* ctx) {
    int32_t direction = (int32_t)ctx->r4;
    uint32_t dram_addr = (uint32_t)ctx->r5;

    // Diagnostic logging
    static int si_dma_count = 0; si_dma_count++;
#if LOD_ENABLE_SAVE_HANDLER_TRACE
    static bool save_trace_wrappers_installed = false;
    if (!save_trace_wrappers_installed) {
        lod_install_save_trace_wrappers("si-lazy-init");
        save_trace_wrappers_installed = true;
    }
#endif
#if LOD_ENABLE_SAVE_EXIT_TRACE
    static bool save_exit_trace_wrappers_installed = false;
    if (!save_exit_trace_wrappers_installed) {
        lod_install_save_exit_trace_wrappers("si-lazy-init");
        save_exit_trace_wrappers_installed = true;
    }
#endif
#if LOD_ENABLE_SAVE_STATE45_TRACE
    static bool save_state45_trace_wrappers_installed = false;
    if (!save_state45_trace_wrappers_installed) {
        lod_install_save_state45_trace_wrappers("si-lazy-init");
        save_state45_trace_wrappers_installed = true;
    }
#endif
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

#if LOD_ENABLE_SAVE_AUTO_INPUT
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
#endif

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

#if LOD_ENABLE_BOOT_GS_SKIP
    // Boot screen skip: force gamestate transitions past screens that
    // require Controller Pak or are irrelevant for PC port.
    // GameStateMgr at 0x8031AC78: +0x24 = current_game_state, +0x28 = exitingGameState
    if (direction == 0) {
        uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
        if (gsm_addr != 0) {
            uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
            int32_t cur_gs = *(int32_t*)(rdram + gsm_phys + 0x24);
            static int skip_delay = 0;
            // gs=4 (save screen): skip to gs=1 (Konami logo, needed for graphics init)
            // gs=12 (expansion pak): skip to gs=6 (title screen)
            // gs=5 (intro cutscene): skip to gs=6 (title screen)
            int32_t target_gs = -1;
            if (cur_gs == 4) target_gs = 1;
            else if (cur_gs == 12) target_gs = 6;
            else if (cur_gs == 5) target_gs = 6;

            if (target_gs >= 0) {
                skip_delay++;
                if (skip_delay == 60) { // ~1 second delay
                    // Force gamestate transition: set current=-1 (exit flag)
                    // and exitingGameState=target (next state).
                    *(int32_t*)(rdram + gsm_phys + 0x24) = -1;
                    *(int32_t*)(rdram + gsm_phys + 0x28) = target_gs;
                    fprintf(stderr, "[SKIP] gs=%d → gs=%d (SI_DMA #%d)\n",
                            cur_gs, target_gs, si_dma_count);
                    // NOTE: don't swap overlays here — race condition with game
                    // thread executing overlay code. The func_80012ED0 override
                    // handles overlay swaps when the game's own loader fires.
                    skip_delay = 0;
                }
            } else {
                skip_delay = 0;
            }
        }
    }
#endif

    // Watchpoint: monitor save object state byte at 0x8031AFA4+0x09
    if (direction == 0) {
        static uint8_t prev_state = 255;
        static uint8_t prev_parent_dispatch_state = 255;
        static uint8_t prev_child_state = 255;
        static uint8_t prev_child_dispatch_state = 255;
        static bool dumped_dispatch3_window = false;
        uint8_t cur_state = rdram[(0x31AFA4 + 0x09) ^ 3];
        uint8_t parent_dispatch_state = lod_object_dispatch_state(rdram, 0x31AFA4);
        uint32_t child_addr = *(uint32_t*)(rdram + 0x31AFA4 + 0x1C);
        uint32_t child_phys = child_addr & 0x1FFFFFFF;
        bool child_ok = child_addr != 0 && lod_rdram_range_ok(child_phys, 0x74);
        uint8_t child_state = child_ok ? lod_rdram_u8(rdram, child_phys + 0x09) : 255;
        uint8_t child_dispatch_state = child_ok ? lod_object_dispatch_state(rdram, child_phys) : 255;
        if (parent_dispatch_state == 3 &&
                (parent_dispatch_state != prev_parent_dispatch_state || (si_dma_count % 500) == 0)) {
            lod_dump_save_state_context(rdram, si_dma_count, "parent", 0x31AFA4,
                parent_dispatch_state != prev_parent_dispatch_state ? "enter-dispatch3" : "poll-dispatch3");
            if (!dumped_dispatch3_window) {
                lod_dump_save_object_window(rdram, si_dma_count, "parent-enter-dispatch3");
                dumped_dispatch3_window = true;
            }
        }
        if (child_dispatch_state == 3 &&
                (child_dispatch_state != prev_child_dispatch_state || (si_dma_count % 500) == 0)) {
            lod_dump_save_state_context(rdram, si_dma_count, "child", child_phys,
                child_dispatch_state != prev_child_dispatch_state ? "enter-dispatch3" : "poll-dispatch3");
        }
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
        if (parent_dispatch_state != prev_parent_dispatch_state && parent_dispatch_state != 255) {
            fprintf(stderr, "[WATCH] Save parent dispatch_state: %d → %d (SI_DMA #%d)\n",
                    prev_parent_dispatch_state, parent_dispatch_state, si_dma_count);
            prev_parent_dispatch_state = parent_dispatch_state;
        }
        if (child_ok && child_state != prev_child_state && child_state != 0) {
            fprintf(stderr, "[WATCH] Save child state: %d → %d (SI_DMA #%d child=0x%08X)\n",
                    prev_child_state, child_state, si_dma_count, child_addr);
            prev_child_state = child_state;
        }
        if (child_ok && child_dispatch_state != prev_child_dispatch_state && child_dispatch_state != 255) {
            fprintf(stderr, "[WATCH] Save child dispatch_state: %d → %d (SI_DMA #%d child=0x%08X)\n",
                    prev_child_dispatch_state, child_dispatch_state, si_dma_count, child_addr);
            prev_child_dispatch_state = child_dispatch_state;
        } else if (!child_ok) {
            prev_child_state = 255;
            prev_child_dispatch_state = 255;
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
