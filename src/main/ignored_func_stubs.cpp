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
#include "lod/lod_fault_trace.hpp"
#include "librecomp/overlays.hpp"

#ifndef LOD_ENABLE_SAVE_AUTO_INPUT
#define LOD_ENABLE_SAVE_AUTO_INPUT 0
#endif

#ifndef LOD_ENABLE_BOOT_INPUT_SCRIPT
#define LOD_ENABLE_BOOT_INPUT_SCRIPT 0
#endif

#ifndef LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT
#define LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT 0
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

#ifndef LOD_ENABLE_BGSTATE_TRACE
#define LOD_ENABLE_BGSTATE_TRACE 0
#endif

#ifndef LOD_ENABLE_GS3_ANIM_TRACE
#define LOD_ENABLE_GS3_ANIM_TRACE 0
#endif

#ifndef LOD_ENABLE_KSEG0_FAULT_TRACE
#define LOD_ENABLE_KSEG0_FAULT_TRACE 0
#endif

#ifndef LOD_ENABLE_BOOT_GS_SKIP
#define LOD_ENABLE_BOOT_GS_SKIP 0
#endif

// ── PFS/audio override gates ────────────────────────────────────────
// Each override masks a real runtime/PFS-internal issue. Most old bring-up
// overrides still default to 1 until individually burned down. Set any to 0
// at build time to drop the override; the linker will then resolve
// the symbol to the weak RECOMP_FUNC version in RecompiledFuncs/.
//
// Cleanup priority (try disabling first — thinnest justification):
//   * LOD_OVERRIDE_ALENVMIXERPULL      — libultra envmixer hard no-op
//     (LOD_OVERRIDE_FUNC_8009E480 remains accepted as a compatibility alias)
//   * LOD_OVERRIDE_FUNC_8001D398       — contpak recheck force-return-2
//
// LOD_OVERRIDE_FUNC_8009F400 is intentionally default-off: the direct
// osPfsFindFile shim can return stale NOT FOUND after the game creates a
// Controller Pak note, while the recompiled/native PFS path advances.
#ifndef LOD_OVERRIDE_ALENVMIXERPULL
#  ifdef LOD_OVERRIDE_FUNC_8009E480
#    define LOD_OVERRIDE_ALENVMIXERPULL LOD_OVERRIDE_FUNC_8009E480
#  else
#    define LOD_OVERRIDE_ALENVMIXERPULL 1
#  endif
#endif
#ifndef LOD_OVERRIDE_FUNC_8009E480
#define LOD_OVERRIDE_FUNC_8009E480 LOD_OVERRIDE_ALENVMIXERPULL
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
#define LOD_OVERRIDE_FUNC_8009F400 0
#endif

// Input callback — defined in main.cpp (C++ linkage)
bool get_n64_input(int controller_num, uint16_t* buttons, float* x, float* y);

// Virtual Controller Pak — defined in pak.cpp
extern "C" void pak_read(uint16_t addr, uint8_t* out, int count);
extern "C" void pak_write(uint16_t addr, const uint8_t* in, int count);
extern "C" uint8_t pak_data_crc(const uint8_t* data, int size);

// Overlay management — defined in librecomp
extern "C" void load_overlays(uint32_t rom, int32_t ram_addr, uint32_t size);
extern "C" void unload_overlays(int32_t ram_addr, uint32_t size);
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
#if LOD_ENABLE_BGSTATE_TRACE
static void lod_install_bgstate_trace_wrappers(const char* reason);
#endif
#if LOD_ENABLE_GS3_ANIM_TRACE
static void lod_install_gs3_anim_trace_wrappers(const char* reason);
#endif
#if LOD_ENABLE_KSEG0_FAULT_TRACE
static void lod_install_kseg0_fault_trace_wrappers(const char* reason);
#endif

// ── Map overlay table: ROM start → full DMA size ────────────────────
// Built from the game's overlay table at ROM 0xB39E8. Do not hard-code
// recomp overlay IDs here: adding/removing sections changes generated IDs.
static const struct { uint32_t rom_start; uint32_t full_size; } map_ovl_table[] = {
    { 0x0076CD00, 0x0C810 }, { 0x00779510, 0x04CF0 }, { 0x0077E200, 0x04F40 },
    { 0x00783140, 0x02EF0 }, { 0x00786030, 0x03FF0 }, { 0x0078A020, 0x092B0 },
    { 0x007932D0, 0x07E40 }, { 0x0079B110, 0x07C60 }, { 0x007A2D70, 0x03380 },
    { 0x007A60F0, 0x04FA0 }, { 0x007AB090, 0x033A0 }, { 0x007AE430, 0x03B30 },
    { 0x007B1F60, 0x02CD0 }, { 0x007B4C30, 0x01DF0 }, { 0x007B6A20, 0x02880 },
    { 0x007B92A0, 0x00F30 }, { 0x007BA1D0, 0x04700 }, { 0x007BE8D0, 0x043E0 },
    { 0x007C2CB0, 0x04050 }, { 0x007C6D00, 0x01CA0 }, { 0x007C89A0, 0x05440 },
    { 0x007CDDE0, 0x01AB0 }, { 0x007CF890, 0x000D0 }, { 0x007CF960, 0x02EA0 },
    { 0x007D2800, 0x01490 }, { 0x007D3C90, 0x00790 }, { 0x007D4420, 0x02700 },
    { 0x007D6B20, 0x01070 }, { 0x007D7B90, 0x01C00 }, { 0x007D9790, 0x070C0 },
    { 0x007E0850, 0x01DC0 }, { 0x007E2610, 0x03040 }, { 0x007E5650, 0x02340 },
    { 0x007E7990, 0x07C50 }, { 0x007EF5E0, 0x16690 }, { 0x00805C70, 0x04880 },
    { 0x0080A4F0, 0x0A900 }, { 0x00814DF0, 0x06220 }, { 0x0081B010, 0x03030 },
    { 0x0081E040, 0x0B1E0 }, { 0x00829220, 0x01470 }, { 0x0082A690, 0x03CA0 },
    { 0x0082E330, 0x079E0 }, { 0x00835D10, 0x006A0 }, { 0x008363B0, 0x00590 },
    { 0x00836940, 0x00EC0 }, { 0x00837800, 0x009C0 },
};
static constexpr int MAP_OVL_TABLE_SIZE = 47;
static constexpr uint32_t LOD_MAP_OVL_RAM = 0x802E3B70;
static constexpr uint32_t LOD_MAP_OVL_UNLOAD_SIZE = 0x00200000;
static constexpr uint32_t LOD_OVLSYS_RAM = 0x801CAEA0;
static constexpr uint32_t LOD_OVLSYS_UNLOAD_SIZE = 0x00020000;
static uint32_t lod_current_map_ovl_rom = 0;
static uint32_t lod_current_map_ovl_size = 0;
static int lod_map_ovl_load_count = 0;

static bool lod_decode_rdram_phys_addr(uint32_t addr, uint32_t size, uint32_t* phys_out) {
    if (size > 0x800000) {
        return false;
    }

    uint32_t phys = 0;
    if (addr < 0x00800000) {
        // Some DMA helpers pass physical RDRAM offsets directly.
        phys = addr;
    } else if (addr >= 0x80000000 && addr < 0x80800000) {
        // KSEG0 cached RDRAM.
        phys = addr - 0x80000000;
    } else if (addr >= 0xA0000000 && addr < 0xA0800000) {
        // KSEG1 uncached RDRAM.
        phys = addr - 0xA0000000;
    } else {
        return false;
    }

    if (phys > 0x800000 - size) {
        return false;
    }

    *phys_out = phys;
    return true;
}

static bool lod_is_overlay_system_dma(uint32_t rom_start, uint32_t vram_dest) {
    if (vram_dest != LOD_OVLSYS_RAM) {
        return false;
    }

    switch (rom_start) {
        case 0x00745230: // overlay_system variant 0
        case 0x0074CD10: // overlay_system variant 1
        case 0x0075A570: // overlay_system variant 2
        case 0x007639A0: // overlay_system variant 3
            return true;
        default:
            return false;
    }
}

#if LOD_ENABLE_BGSTATE_TRACE
static bool lod_range_overlaps(uint32_t start, uint32_t size, uint32_t range_start, uint32_t range_end) {
    if (size == 0) {
        return false;
    }
    uint64_t end = (uint64_t)start + size;
    return (uint64_t)start < range_end && end > range_start;
}
#endif

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
                unload_overlays((int32_t)vram_dest, LOD_MAP_OVL_UNLOAD_SIZE);
                load_overlays(rom_start, (int32_t)vram_dest, full_size);
                // Store end address in ctrl struct if provided (matches original DMA behavior).
                // ni_system_handler passes sys+0x2B24 here; leaving it at zero prevents
                // the overlay-system object (0x1AB) from being created after map load.
                if (ctrl != 0) {
                    uint32_t ctrl_phys = 0;
                    if (lod_decode_rdram_phys_addr(ctrl, 4, &ctrl_phys)) {
                        *(uint32_t*)(rdram + ctrl_phys) = vram_dest + size;
                    } else {
                        static int bad_map_ctrl_count = 0;
                        if (++bad_map_ctrl_count <= 8) {
                            fprintf(stderr,
                                    "[MAP_OVL] WARNING: skipped map DMA ctrl write ctrl=0x%08X rom=0x%08X dst=0x%08X size=0x%X\n",
                                    ctrl, rom_start, vram_dest, size);
                        }
                    }
                }
                lod_map_ovl_load_count++;
                lod_current_map_ovl_rom = rom_start;
                lod_current_map_ovl_size = full_size;
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
#if LOD_ENABLE_BGSTATE_TRACE
                lod_install_bgstate_trace_wrappers("map-overlay-load");
#endif

                if (lod_map_ovl_load_count <= 20 || (lod_map_ovl_load_count % 100) == 0)
                    fprintf(stderr, "[MAP_OVL] #%d loaded rom=0x%X size=0x%X (game requested 0x%X)\n",
                            lod_map_ovl_load_count, rom_start, full_size, size);
                ctx->r2 = 0;
                return;
            }
        }
        fprintf(stderr, "[MAP_OVL] WARNING: unknown overlay rom=0x%X size=0x%X\n", rom_start, size);
    }

    // Non-map-overlay call: do the DMA via lod_copy_overlay_data if
    // the destination is in valid RDRAM range, otherwise skip.
    //
    // Important: do not feed uint32_t addresses back through MEM_W here.
    // MEM_* expects sign-extended N64 KSEG addresses; after a uint32_t cast,
    // a valid 0x80xxxxxx pointer is treated as +0x100xxxxxx and can write
    // outside mapped host memory. Normalize to a physical RDRAM offset first.
    if (rom_start != 0 && size > 0 && size < 0x100000) {
        uint32_t rdram_dst = 0;
        if (lod_decode_rdram_phys_addr(vram_dest, size, &rdram_dst)) {
#if LOD_ENABLE_BGSTATE_TRACE
            bool touches_ovlsys_ram = lod_range_overlaps(vram_dest, size, 0x801CAEA0, 0x801ED010);
            bool touches_ovlsys_rom = lod_range_overlaps(rom_start, size, 0x00745230, 0x0076CD00);
            uint32_t ovlsys_top0_before = *(uint32_t*)(rdram + 0x001D1880);
            uint32_t ovlsys_sub0_before = *(uint32_t*)(rdram + 0x001D1ACC);
            if (touches_ovlsys_ram || touches_ovlsys_rom) {
                static int ovlsys_dma_trace_count = 0;
                if (++ovlsys_dma_trace_count <= 40) {
                    fprintf(stderr,
                        "[OVLSYS-DMA] before #%d ctrl=0x%08X rom=0x%08X dst=0x%08X phys=0x%06X size=0x%X "
                        "touch_ram=%d touch_rom=%d table0=0x%08X sub0=0x%08X\n",
                        ovlsys_dma_trace_count, ctrl, rom_start, vram_dest, rdram_dst, size,
                        touches_ovlsys_ram ? 1 : 0, touches_ovlsys_rom ? 1 : 0,
                        ovlsys_top0_before, ovlsys_sub0_before);
                }
            }
#endif
            lod_copy_overlay_data(rdram, rom_start, rdram_dst, size);
            if (lod_is_overlay_system_dma(rom_start, vram_dest)) {
                unload_overlays((int32_t)LOD_OVLSYS_RAM, LOD_OVLSYS_UNLOAD_SIZE);
                load_overlays(rom_start, (int32_t)vram_dest, size);
                static int ovlsys_load_count = 0;
                if (++ovlsys_load_count <= 16 || (ovlsys_load_count % 100) == 0) {
                    fprintf(stderr,
                        "[OVLSYS-DMA] registered variant #%d rom=0x%08X dst=0x%08X size=0x%X\n",
                        ovlsys_load_count, rom_start, vram_dest, size);
                }
            }
#if LOD_ENABLE_BGSTATE_TRACE
            if (touches_ovlsys_ram || touches_ovlsys_rom) {
                static int ovlsys_dma_after_trace_count = 0;
                if (++ovlsys_dma_after_trace_count <= 40) {
                    fprintf(stderr,
                        "[OVLSYS-DMA] after  #%d ctrl=0x%08X rom=0x%08X dst=0x%08X phys=0x%06X size=0x%X "
                        "table0=0x%08X->0x%08X sub0=0x%08X->0x%08X\n",
                        ovlsys_dma_after_trace_count, ctrl, rom_start, vram_dest, rdram_dst, size,
                        ovlsys_top0_before, *(uint32_t*)(rdram + 0x001D1880),
                        ovlsys_sub0_before, *(uint32_t*)(rdram + 0x001D1ACC));
                }
            }
            lod_install_bgstate_trace_wrappers("non-map-dma");
#endif
            // Store end address in ctrl struct if provided (matches original behavior)
            if (ctrl != 0) {
                uint32_t ctrl_phys = 0;
                if (lod_decode_rdram_phys_addr(ctrl, 4, &ctrl_phys)) {
                    *(uint32_t*)(rdram + ctrl_phys) = vram_dest + size;
                } else {
                    static int bad_ctrl_count = 0;
                    if (++bad_ctrl_count <= 8) {
                        fprintf(stderr,
                            "[MAP_OVL] WARNING: skipped non-map DMA ctrl write ctrl=0x%08X rom=0x%08X dst=0x%08X size=0x%X\n",
                            ctrl, rom_start, vram_dest, size);
                    }
                }
            }
        } else {
            static int bad_dst_count = 0;
            if (++bad_dst_count <= 8) {
                fprintf(stderr,
                    "[MAP_OVL] WARNING: skipped non-map DMA copy with invalid dst=0x%08X rom=0x%08X size=0x%X\n",
                    vram_dest, rom_start, size);
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
void __ll_rshift_recomp(uint8_t* /*rdram*/, recomp_context* ctx) {
    // IDO/libultra helper for signed 64-bit right shifts. The N64 o32 calling
    // convention passes 64-bit values as register pairs: a0/a1 for the value,
    // a2/a3 for the shift count. Return is v0/v1 (high/low).
    //
    // This is not an OS stub: asset/decompression state code calls it while
    // deciding whether an async load has finished. Leaving it empty preserves
    // stale v0/v1 and can strand the boot save-screen scheduler on a null
    // callback.
    uint64_t value_u64 = (uint64_t)(uint32_t)ctx->r4 << 32 |
                         (uint64_t)(uint32_t)ctx->r5;
    int64_t value = (int64_t)value_u64;
    uint32_t shift = (uint32_t)ctx->r7;
    int64_t result = 0;

    if (shift >= 64) {
        result = value < 0 ? -1 : 0;
    } else {
        result = value >> shift;
    }

    ctx->r2 = (int32_t)(result >> 32);
    ctx->r3 = (int32_t)(result & 0xFFFFFFFF);
}
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
// pfs_find_file / func_8009F400: legacy bring-up shim that scans the
// directory directly from the pak image. Keep default-off; use only for
// controlled A/B regression checks against the native/recompiled PFS path.
static void lod_osPfsFindFile_direct_shim(uint8_t* rdram, recomp_context* ctx) {
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

// Keep both names so the A/B-only override works before and after symbol
// regeneration renamed 0x8009F400 from func_8009F400 to pfs_find_file.
void func_8009F400(uint8_t* rdram, recomp_context* ctx) {
    lod_osPfsFindFile_direct_shim(rdram, ctx);
}

void pfs_find_file(uint8_t* rdram, recomp_context* ctx) {
    lod_osPfsFindFile_direct_shim(rdram, ctx);
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

#if LOD_OVERRIDE_ALENVMIXERPULL
// alEnvmixerPull (formerly func_8009E480): libultra envmixer pull on the audio
// task chain. This hard no-op keeps bring-up stable but suppresses generated
// audio command output; disable with LOD_OVERRIDE_ALENVMIXERPULL=0 or the
// compatibility alias LOD_OVERRIDE_FUNC_8009E480=0 while debugging audio.
static void alEnvmixerPull_noop(recomp_context* ctx) {
    ctx->r2 = 0;
}

void alEnvmixerPull(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    alEnvmixerPull_noop(ctx);
}

// Compatibility for stale local RecompiledFuncs generated before the symbol
// rename. Once regenerated, the weak original is named alEnvmixerPull instead.
void func_8009E480(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    alEnvmixerPull_noop(ctx);
}
#endif // LOD_OVERRIDE_ALENVMIXERPULL

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

static uint32_t lod_current_exec_flags(uint8_t* rdram) {
    constexpr uint32_t EXEC_FLAGS_PHYS = 0x001CABC8;
    return lod_rdram_range_ok(EXEC_FLAGS_PHYS, 4) ? lod_rdram_u32(rdram, EXEC_FLAGS_PHYS) : 0;
}

static uint32_t lod_current_ni_sys_ptr(uint8_t* rdram) {
    constexpr uint32_t NI_SYS_PTR_PHYS = 0x001CAC1C;
    return lod_rdram_range_ok(NI_SYS_PTR_PHYS, 4) ? lod_rdram_u32(rdram, NI_SYS_PTR_PHYS) : 0;
}

static bool lod_fast_gameplay_idle_target(uint8_t* rdram, int32_t gs) {
    return gs == 3 &&
           lod_current_exec_flags(rdram) == 0x38000000 &&
           lod_current_ni_sys_ptr(rdram) == 0x803241C0;
}

static uint8_t lod_object_dispatch_state(uint8_t* rdram, uint32_t obj_phys);

#if LOD_ENABLE_GS3_ANIM_TRACE
static recomp_func_t* lod_orig_dmamgr_update_pending_file = nullptr; // 0x80011E48
static recomp_func_t* lod_orig_dmamgr_romcopy = nullptr;             // 0x8001A42C

static bool lod_trace_dense_dmamgr(int32_t gs) {
    // The current late-crash reproduction reaches this map overlay before
    // Signal 10 at 0x380800002. Another repro can happen earlier in gs=8.
    return gs == 8 || lod_current_map_ovl_rom == 0x007C89A0;
}

static uint32_t lod_trace_u32_or_zero(uint8_t* rdram, uint32_t phys, bool ok) {
    return ok && lod_rdram_range_ok(phys, 4) ? lod_rdram_u32(rdram, phys) : 0;
}

static uint16_t lod_trace_u16_or_zero(uint8_t* rdram, uint32_t phys, bool ok) {
    return ok && lod_rdram_range_ok(phys, 2) ? (uint16_t)lod_rdram_s16(rdram, phys) : 0;
}

static void lod_trace_dmamgr_update_pending_file(uint8_t* rdram, recomp_context* ctx) {
    static int count_all = 0;
    static int count_traced = 0;
    count_all++;

    int32_t gs = lod_current_gamestate(rdram);
    uint32_t obj_addr = (uint32_t)ctx->r4;
    uint32_t obj_phys = 0;
    bool obj_ok = lod_decode_rdram_phys_addr(obj_addr, 0x38, &obj_phys);

    uint32_t chunk_addr = obj_ok ? lod_rdram_u32(rdram, obj_phys + 0x34) : 0;
    uint32_t chunk_phys = 0;
    bool chunk_ok = chunk_addr != 0 && lod_decode_rdram_phys_addr(chunk_addr, 0x988, &chunk_phys);

    uint16_t slot = lod_trace_u16_or_zero(rdram, chunk_phys + 0x844, chunk_ok);
    uint16_t pending = lod_trace_u16_or_zero(rdram, chunk_phys + 0x846, chunk_ok);
    bool slot_ok = chunk_ok && slot < 16;
    uint32_t entry_phys = slot_ok ? (chunk_phys + 0x848 + (uint32_t)slot * 0x14) : 0;
    uint32_t entry_rom = lod_trace_u32_or_zero(rdram, entry_phys + 0x00, slot_ok);
    uint32_t entry_ram = lod_trace_u32_or_zero(rdram, entry_phys + 0x04, slot_ok);
    uint32_t entry_size = lod_trace_u32_or_zero(rdram, entry_phys + 0x08, slot_ok);
    uint32_t entry_file_id = lod_trace_u32_or_zero(rdram, entry_phys + 0x0C, slot_ok);
    uint32_t entry_user_ptr = lod_trace_u32_or_zero(rdram, entry_phys + 0x10, slot_ok);

    bool suspicious =
        !obj_ok || !chunk_ok || !slot_ok || pending == 0 ||
        (entry_rom >= 0x30000000 && entry_rom < 0x40000000) ||
        (entry_ram >= 0x30000000 && entry_ram < 0x40000000) ||
        (entry_user_ptr >= 0x30000000 && entry_user_ptr < 0x40000000);

    if (gs != 0) {
        count_traced++;
        bool dense = lod_trace_dense_dmamgr(gs);
        bool should_log = suspicious || count_traced <= 120 || (count_traced % 240) == 0 || dense;
        if (should_log) {
            uint32_t stream = lod_trace_u32_or_zero(rdram, chunk_phys + 0x818, chunk_ok);
            uint32_t out_rom = lod_trace_u32_or_zero(rdram, chunk_phys + 0x828, chunk_ok);
            uint32_t out_ram = lod_trace_u32_or_zero(rdram, chunk_phys + 0x82C, chunk_ok);
            uint32_t out_size = lod_trace_u32_or_zero(rdram, chunk_phys + 0x830, chunk_ok);
            uint32_t align = lod_trace_u32_or_zero(rdram, chunk_phys + 0x838, chunk_ok);
            uint32_t file_id = lod_trace_u32_or_zero(rdram, chunk_phys + 0x840, chunk_ok);
            uint32_t rom_chunk = lod_trace_u32_or_zero(rdram, chunk_phys + 0x800, chunk_ok);
            uint32_t rom_cur = lod_trace_u32_or_zero(rdram, chunk_phys + 0x804, chunk_ok);
            uint32_t read_limit = lod_trace_u32_or_zero(rdram, chunk_phys + 0x808, chunk_ok);
            uint32_t bytes_left = lod_trace_u32_or_zero(rdram, chunk_phys + 0x80C, chunk_ok);

            fprintf(stderr,
                    "[GS3-DMAMGR] 11E48 all#%d trace#%d gs=%d map#%d rom=0x%08X map_size=0x%X "
                    "a0=0x%08X obj_ok=%d ra=0x%08X sp=0x%08X chunk=0x%08X chunk_ok=%d "
                    "slot=%u pending=%u entry={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X user=0x%08X} "
                    "cur={stream=0x%08X rom=0x%08X ram=0x%08X size=0x%08X align=0x%08X file=0x%08X "
                    "rom_chunk=0x%08X rom_cur=0x%08X read_limit=0x%08X bytes_left=0x%08X} suspicious=%d\n",
                    count_all, count_traced, gs, lod_map_ovl_load_count, lod_current_map_ovl_rom,
                    lod_current_map_ovl_size, obj_addr, obj_ok ? 1 : 0, (uint32_t)ctx->r31,
                    (uint32_t)ctx->r29, chunk_addr, chunk_ok ? 1 : 0, slot, pending,
                    entry_rom, entry_ram, entry_size, entry_file_id, entry_user_ptr,
                    stream, out_rom, out_ram, out_size, align, file_id,
                    rom_chunk, rom_cur, read_limit, bytes_left, suspicious ? 1 : 0);
        }
    }

    if (lod_orig_dmamgr_update_pending_file != nullptr) {
        lod_orig_dmamgr_update_pending_file(rdram, ctx);
    }
}

static void lod_trace_dmamgr_romcopy(uint8_t* rdram, recomp_context* ctx) {
    static int count_trace_size8 = 0;

    int32_t gs = lod_current_gamestate(rdram);
    uint32_t rom = (uint32_t)ctx->r4;
    uint32_t dest = (uint32_t)ctx->r5;
    uint32_t size = (uint32_t)ctx->r6;
    uint32_t ra = (uint32_t)ctx->r31;

    if (lod_orig_dmamgr_romcopy != nullptr) {
        lod_orig_dmamgr_romcopy(rdram, ctx);
    }

    if (gs != 0 && size == 8) {
        count_trace_size8++;
        uint32_t dest_phys = 0;
        bool dest_ok = lod_decode_rdram_phys_addr(dest, 8, &dest_phys);
        uint32_t word0 = dest_ok ? lod_rdram_u32(rdram, dest_phys) : 0;
        uint32_t word1 = dest_ok ? lod_rdram_u32(rdram, dest_phys + 4) : 0;
        bool suspicious =
            !dest_ok ||
            (rom >= 0x30000000 && rom < 0x40000000) ||
            (word0 >= 0x30000000 && word0 < 0x40000000) ||
            (word1 >= 0x30000000 && word1 < 0x40000000);
        bool dense = lod_trace_dense_dmamgr(gs);
        bool should_log = suspicious || count_trace_size8 <= 120 || (count_trace_size8 % 240) == 0 || dense;
        if (should_log) {
            fprintf(stderr,
                    "[GS3-DMAMGR] 1A42C size8#%d gs=%d map#%d rom=0x%08X src=0x%08X dest=0x%08X "
                    "dest_ok=%d out={0x%08X,0x%08X} ra=0x%08X suspicious=%d\n",
                    count_trace_size8, gs, lod_map_ovl_load_count, lod_current_map_ovl_rom,
                    rom, dest, dest_ok ? 1 : 0, word0, word1, ra, suspicious ? 1 : 0);
        }
    }
}

static void lod_install_gs3_anim_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                               recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

static void lod_install_gs3_anim_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    lod_install_gs3_anim_trace_wrapper(0x80011E48, lod_trace_dmamgr_update_pending_file,
        &lod_orig_dmamgr_update_pending_file);
    lod_install_gs3_anim_trace_wrapper(0x8001A42C, lod_trace_dmamgr_romcopy,
        &lod_orig_dmamgr_romcopy);

    if (install_count <= 4) {
        fprintf(stderr,
                "[GS3-DMAMGR] installed wrappers #%d reason=%s update=%p romcopy=%p\n",
                install_count, reason,
                (void*)lod_orig_dmamgr_update_pending_file,
                (void*)lod_orig_dmamgr_romcopy);
    }
}

extern "C" void lod_install_gs3_anim_trace_wrappers_early() {
    lod_install_gs3_anim_trace_wrappers("lod-on-init");
}
#endif

#if LOD_ENABLE_KSEG0_FAULT_TRACE
static recomp_func_t* lod_orig_kseg0_dmamgr_update = nullptr; // 0x80011E48
static recomp_func_t* lod_orig_kseg0_dmamgr_romcopy = nullptr; // 0x8001A42C

static uint32_t lod_kseg0_trace_u32_or_zero(uint8_t* rdram, uint32_t phys, bool ok) {
    return ok && lod_rdram_range_ok(phys, 4) ? lod_rdram_u32(rdram, phys) : 0;
}

static uint16_t lod_kseg0_trace_u16_or_zero(uint8_t* rdram, uint32_t phys, bool ok) {
    return ok && lod_rdram_range_ok(phys, 2) ? (uint16_t)lod_rdram_s16(rdram, phys) : 0;
}

static bool lod_kseg0_trace_ram_span_ok(uint32_t addr, uint32_t size) {
    uint32_t phys = 0;
    return lod_decode_rdram_phys_addr(addr, size != 0 ? size : 1, &phys);
}

static void lod_kseg0_record_dmamgr_romcopy(uint8_t* rdram, recomp_context* ctx) {
    LodKseg0FaultTraceSnapshot& s = lod_kseg0_fault_trace_snapshot;
    s.magic = 0x4B534730; // "KSG0"
    s.romcopy_count++;
    s.romcopy_gs = (uint32_t)lod_current_gamestate(rdram);
    s.romcopy_rom = (uint32_t)ctx->r4;
    s.romcopy_ram = (uint32_t)ctx->r5;
    s.romcopy_size = (uint32_t)ctx->r6;
    s.romcopy_ra = (uint32_t)ctx->r31;
    s.romcopy_ram_phys = 0;
    uint32_t phys = 0;
    s.romcopy_ram_ok = lod_decode_rdram_phys_addr(
        s.romcopy_ram, s.romcopy_size != 0 ? s.romcopy_size : 1, &phys) ? 1 : 0;
    if (s.romcopy_ram_ok != 0) {
        s.romcopy_ram_phys = phys;
    }
}

static void lod_kseg0_record_snapshot(uint8_t* rdram, recomp_context* ctx, uint32_t func_vram) {
    LodKseg0FaultTraceSnapshot& s = lod_kseg0_fault_trace_snapshot;
    s.magic = 0x4B534730; // "KSG0"
    s.count++;
    s.func_vram = func_vram;
    s.gs = (uint32_t)lod_current_gamestate(rdram);
    s.map_load_count = (uint32_t)lod_map_ovl_load_count;
    s.map_rom = lod_current_map_ovl_rom;
    s.map_size = lod_current_map_ovl_size;
    s.ra = (uint32_t)ctx->r31;
    s.sp = (uint32_t)ctx->r29;
    s.a0 = (uint32_t)ctx->r4;
    s.a1 = (uint32_t)ctx->r5;
    s.a2 = (uint32_t)ctx->r6;
    s.a3 = (uint32_t)ctx->r7;
    s.v0 = (uint32_t)ctx->r2;
    s.v1 = (uint32_t)ctx->r3;
    s.s0 = (uint32_t)ctx->r16;
    s.t0 = (uint32_t)ctx->r8;
    s.t1 = (uint32_t)ctx->r9;
    s.t2 = (uint32_t)ctx->r10;
    s.t3 = (uint32_t)ctx->r11;
    s.t4 = (uint32_t)ctx->r12;
    s.t5 = (uint32_t)ctx->r13;
    s.t6 = (uint32_t)ctx->r14;
    s.t7 = (uint32_t)ctx->r15;
    s.t8 = (uint32_t)ctx->r24;
    s.t9 = (uint32_t)ctx->r25;

    s.chunk_addr = 0;
    s.chunk_phys = 0;
    s.slot = 0xFFFFFFFF;
    s.pending = 0xFFFFFFFF;
    s.entry_rom = 0;
    s.entry_ram = 0;
    s.entry_size = 0;
    s.entry_file_id = 0;
    s.entry_user_ptr = 0;
    s.cur_rom = 0;
    s.cur_ram = 0;
    s.cur_size = 0;
    s.cur_file_id = 0;
    s.entry_ram_ok = 1;
    s.cur_ram_ok = 1;

    if (func_vram == 0x80011E48) {
        uint32_t obj_addr = (uint32_t)ctx->r4;
        uint32_t obj_phys = 0;
        bool obj_ok = lod_decode_rdram_phys_addr(obj_addr, 0x38, &obj_phys);
        s.chunk_addr = obj_ok ? lod_rdram_u32(rdram, obj_phys + 0x34) : 0;
        bool chunk_ok = s.chunk_addr != 0 && lod_decode_rdram_phys_addr(s.chunk_addr, 0x988, &s.chunk_phys);
        s.slot = lod_kseg0_trace_u16_or_zero(rdram, s.chunk_phys + 0x844, chunk_ok);
        s.pending = lod_kseg0_trace_u16_or_zero(rdram, s.chunk_phys + 0x846, chunk_ok);
        bool slot_ok = chunk_ok && s.slot < 16;
        uint32_t entry_phys = slot_ok ? (s.chunk_phys + 0x848 + s.slot * 0x14) : 0;
        s.entry_rom = lod_kseg0_trace_u32_or_zero(rdram, entry_phys + 0x00, slot_ok);
        s.entry_ram = lod_kseg0_trace_u32_or_zero(rdram, entry_phys + 0x04, slot_ok);
        s.entry_size = lod_kseg0_trace_u32_or_zero(rdram, entry_phys + 0x08, slot_ok);
        s.entry_file_id = lod_kseg0_trace_u32_or_zero(rdram, entry_phys + 0x0C, slot_ok);
        s.entry_user_ptr = lod_kseg0_trace_u32_or_zero(rdram, entry_phys + 0x10, slot_ok);

        s.cur_rom = lod_kseg0_trace_u32_or_zero(rdram, s.chunk_phys + 0x828, chunk_ok);
        s.cur_ram = lod_kseg0_trace_u32_or_zero(rdram, s.chunk_phys + 0x82C, chunk_ok);
        s.cur_size = lod_kseg0_trace_u32_or_zero(rdram, s.chunk_phys + 0x830, chunk_ok);
        s.cur_file_id = lod_kseg0_trace_u32_or_zero(rdram, s.chunk_phys + 0x840, chunk_ok);
        s.entry_ram_ok = (!slot_ok || (s.entry_ram == 0 && s.entry_size == 0) ||
                          lod_kseg0_trace_ram_span_ok(s.entry_ram, s.entry_size)) ? 1 : 0;
        s.cur_ram_ok = (!chunk_ok || (s.cur_ram == 0 && s.cur_size == 0) ||
                        lod_kseg0_trace_ram_span_ok(s.cur_ram, s.cur_size)) ? 1 : 0;
    }
}

static void lod_kseg0_trace_dmamgr_update(uint8_t* rdram, recomp_context* ctx) {
    lod_kseg0_record_snapshot(rdram, ctx, 0x80011E48);
    const LodKseg0FaultTraceSnapshot& s = lod_kseg0_fault_trace_snapshot;
    bool active_bad = s.pending != 0 && s.entry_ram_ok == 0;
    bool current_bad = s.cur_ram_ok == 0;
    if (active_bad || current_bad) {
        static int suspicious_count = 0;
        suspicious_count++;
        if (suspicious_count <= 80 || (suspicious_count % 240) == 0) {
            fprintf(stderr,
                    "[KSEG0-DMAMGR] suspicious#%d call#%u gs=%u map#%u rom=0x%08X "
                    "chunk=0x%08X slot=%u pending=%u active={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X} "
                    "cur={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X} "
                    "active_bad=%d current_bad=%d\n",
                    suspicious_count, s.count, s.gs, s.map_load_count, s.map_rom,
                    s.chunk_addr, s.slot, s.pending,
                    s.entry_rom, s.entry_ram, s.entry_size, s.entry_file_id,
                    s.cur_rom, s.cur_ram, s.cur_size, s.cur_file_id,
                    active_bad ? 1 : 0, current_bad ? 1 : 0);
        }
    }
    if (lod_orig_kseg0_dmamgr_update != nullptr) {
        lod_orig_kseg0_dmamgr_update(rdram, ctx);
    }
}

static void lod_kseg0_trace_dmamgr_romcopy(uint8_t* rdram, recomp_context* ctx) {
    lod_kseg0_record_dmamgr_romcopy(rdram, ctx);
    const LodKseg0FaultTraceSnapshot& s = lod_kseg0_fault_trace_snapshot;
    bool suspicious =
        s.romcopy_ram_ok == 0 ||
        s.romcopy_ram >= 0x80800000 ||
        (s.romcopy_size != 0 && s.romcopy_size > 0x100000);
    if (suspicious) {
        static int suspicious_count = 0;
        suspicious_count++;
        if (suspicious_count <= 80 || (suspicious_count % 240) == 0) {
            fprintf(stderr,
                    "[KSEG0-DMAMGR] romcopy-suspicious#%d call#%u gs=%u map#%d map_rom=0x%08X "
                    "rom=0x%08X ram=0x%08X size=0x%08X phys=0x%08X ra=0x%08X ram_ok=%u\n",
                    suspicious_count, s.romcopy_count, s.romcopy_gs,
                    lod_map_ovl_load_count, lod_current_map_ovl_rom,
                    s.romcopy_rom, s.romcopy_ram, s.romcopy_size,
                    s.romcopy_ram_phys, s.romcopy_ra, s.romcopy_ram_ok);
        }
    }
    if (lod_orig_kseg0_dmamgr_romcopy != nullptr) {
        lod_orig_kseg0_dmamgr_romcopy(rdram, ctx);
    }
}

static void lod_install_kseg0_fault_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                                  recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

static void lod_install_kseg0_fault_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    lod_install_kseg0_fault_trace_wrapper(0x80011E48, lod_kseg0_trace_dmamgr_update,
        &lod_orig_kseg0_dmamgr_update);
    lod_install_kseg0_fault_trace_wrapper(0x8001A42C, lod_kseg0_trace_dmamgr_romcopy,
        &lod_orig_kseg0_dmamgr_romcopy);

    if (install_count <= 4) {
        fprintf(stderr,
                "[KSEG0-TRACE] installed wrappers #%d reason=%s dmamgr=%p romcopy=%p\n",
                install_count, reason,
                (void*)lod_orig_kseg0_dmamgr_update,
                (void*)lod_orig_kseg0_dmamgr_romcopy);
    }
}

extern "C" void lod_install_kseg0_fault_trace_wrappers_early() {
    lod_install_kseg0_fault_trace_wrappers("lod-on-init");
}
#endif

#if LOD_ENABLE_BGSTATE_TRACE
static recomp_func_t* lod_orig_bgstate_driver = nullptr;     // 0x80145AF0
static recomp_func_t* lod_orig_bgstate_bootstrap = nullptr;  // 0x80145B60
static recomp_func_t* lod_orig_bgstate_init = nullptr;       // 0x80145BD0
static recomp_func_t* lod_orig_bgstate_load = nullptr;       // 0x80145DA8
static recomp_func_t* lod_orig_bgstate_update_a = nullptr;   // 0x80145FD4
static recomp_func_t* lod_orig_bgstate_update_b = nullptr;   // 0x801460D4
static recomp_func_t* lod_orig_bgstate_create = nullptr;     // 0x8014615C
static recomp_func_t* lod_orig_bgstate_activate = nullptr;   // 0x80146240
static recomp_func_t* lod_orig_scene5_obj7_driver = nullptr; // 0x8001A5BC
static recomp_func_t* lod_orig_scene5_aux_driver = nullptr;  // 0x8001AA48
static recomp_func_t* lod_orig_scene5_sys_dispatch = nullptr;// 0x8001AB28
static recomp_func_t* lod_orig_scene5_obj8_driver = nullptr; // 0x8001B4C0
static recomp_func_t* lod_orig_scene5_obj8_load = nullptr;   // 0x8001B530
static recomp_func_t* lod_orig_overlay_system_create = nullptr; // 0x8001BA78
static recomp_func_t* lod_orig_sys_obj009_driver = nullptr;  // 0x8001B718
static recomp_func_t* lod_orig_sys_obj00a_driver = nullptr;  // 0x8001BC5C
static recomp_func_t* lod_orig_sys_obj0a2_driver = nullptr;  // 0x80037730
static recomp_func_t* lod_orig_sys_obj106_driver = nullptr;  // 0x80058890
static recomp_func_t* lod_orig_sys_obj185_driver = nullptr;  // 0x80086120
static recomp_func_t* lod_orig_sys_obj089_driver = nullptr;  // 0x8017E00C
static recomp_func_t* lod_orig_sys_obj016_driver = nullptr;  // 0x80189050
static recomp_func_t* lod_orig_sys_obj093_driver = nullptr;  // 0x801CAEA0
static recomp_func_t* lod_orig_ovlsys_script_outer = nullptr; // 0x801CB180
static recomp_func_t* lod_orig_ovlsys_script_alloc = nullptr;  // 0x801CB404
static recomp_func_t* lod_orig_ovlsys_script_wait_a = nullptr; // 0x801CB4CC
static recomp_func_t* lod_orig_ovlsys_script_wait_b = nullptr; // 0x801CB550
static recomp_func_t* lod_orig_ovlsys_ni_init = nullptr;       // 0x801CB5CC
static recomp_func_t* lod_orig_ovlsys_script_done = nullptr;   // 0x801CBC78

static uint32_t lod_bgstate_u32(uint8_t* rdram, uint32_t phys) {
    if (!lod_rdram_range_ok(phys, 4)) {
        return 0;
    }
    return lod_rdram_u32(rdram, phys);
}

static int16_t lod_bgstate_s16(uint8_t* rdram, uint32_t phys) {
    if (!lod_rdram_range_ok(phys, 2)) {
        return 0;
    }
    return lod_rdram_s16(rdram, phys);
}

static uint8_t lod_bgstate_u8(uint8_t* rdram, uint32_t phys) {
    if (!lod_rdram_range_ok(phys, 1)) {
        return 0;
    }
    return lod_rdram_u8(rdram, phys);
}

static void lod_bgstate_trace_snapshot(uint8_t* rdram, const char* label,
                                       uint32_t vram, recomp_context* ctx,
                                       const char* phase, int count) {
    constexpr uint32_t SYS_PHYS = 0x1C82C0;      // 0x801C82C0
    constexpr uint32_t BG_WORK_PHYS = 0x19D180;  // 0x8019D180
    constexpr uint32_t DISPATCH_TABLE_PHYS = 0x18D3B0; // 0x8018D3B0

    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj & 0x1FFFFFFF;
    bool obj_ok = obj != 0 && lod_rdram_range_ok(obj_phys, 0x74);
    uint8_t state = obj_ok ? lod_bgstate_u8(rdram, obj_phys + 0x09) : 0xFF;
    int16_t func_id = obj_ok ? lod_bgstate_s16(rdram, obj_phys + 0x0E) : 0;
    uint8_t dispatch_state = obj_ok ? lod_object_dispatch_state(rdram, obj_phys) : 0xFF;
    uint32_t dispatch_target = 0;
    if (dispatch_state < 64) {
        dispatch_target = lod_bgstate_u32(rdram, DISPATCH_TABLE_PHYS + dispatch_state * 4);
    }

    uint32_t sys2908 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2908);
    uint32_t sys2924 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2924);
    uint32_t sys295c = lod_bgstate_u32(rdram, SYS_PHYS + 0x295C);
    uint32_t sys2960 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2960);
    uint32_t sys2968 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2968);
    uint32_t sys2b10 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B10);
    uint32_t sys2b24 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B24);
    uint32_t sys2b28 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B28);
    uint32_t sys2b2c = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B2C);
    uint32_t sys2bd0 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2BD0);
    int16_t scene = lod_bgstate_s16(rdram, SYS_PHYS + 0x28D0);
    int16_t scene2 = lod_bgstate_s16(rdram, SYS_PHYS + 0x28D2);

    uint32_t bg00 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x00);
    uint32_t bg04 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x04);
    uint32_t bg08 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x08);
    uint32_t bg0c = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x0C);
    uint32_t bg10 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x10);
    uint32_t bg14 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x14);
    uint32_t bg18 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x18);
    uint32_t bg1c = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x1C);
    uint32_t bg20 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x20);
    uint32_t bg24 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x24);

    fprintf(stderr,
        "[BGSTATE] %s %-10s #%d vram=0x%08X gs=%d scene=%d/%d a0=0x%08X "
        "obj_ok=%d state09=%u func_id=%d dispatch=%u target=0x%08X ra=0x%08X "
        "sys2908=0x%08X sys2924=0x%08X sys295c=0x%08X sys2960=0x%08X "
        "sys2968=0x%08X sys2B10=0x%08X sys2B24=0x%08X sys2B28=0x%08X "
        "sys2B2C=0x%08X sys2BD0=0x%08X "
        "bg[00..24]=%08X,%08X,%08X,%08X,%08X,%08X,%08X,%08X,%08X,%08X\n",
        phase, label, count, vram, lod_current_gamestate(rdram), scene, scene2, obj,
        obj_ok ? 1 : 0, state, func_id, dispatch_state, dispatch_target,
        (uint32_t)ctx->r31, sys2908, sys2924, sys295c, sys2960, sys2968,
        sys2b10, sys2b24, sys2b28, sys2b2c, sys2bd0, bg00, bg04, bg08, bg0c,
        bg10, bg14, bg18, bg1c, bg20, bg24);
}

static void lod_trace_bgstate_common(uint8_t* rdram, recomp_context* ctx,
                                     const char* label, uint32_t vram,
                                     recomp_func_t* original, int* count) {
    (*count)++;
    int32_t gs = lod_current_gamestate(rdram);
    bool verbose = *count <= 80 || (*count % 500) == 0 ||
                   (gs == 5 && (*count <= 120 || (*count % 30) == 0));
    if (verbose) {
        lod_bgstate_trace_snapshot(rdram, label, vram, ctx, "enter", *count);
    }
    if (original != nullptr) {
        original(rdram, ctx);
    }
    if (verbose) {
        lod_bgstate_trace_snapshot(rdram, label, vram, ctx, "exit ", *count);
    }
}

static void lod_trace_overlay_script_snapshot(uint8_t* rdram, recomp_context* ctx,
                                              const char* label, uint32_t vram,
                                              const char* phase, int count) {
    constexpr uint32_t SYS_PHYS = 0x1C82C0;      // 0x801C82C0
    constexpr uint32_t TOP_TABLE_PHYS = 0x1D1880; // 0x801D1880
    constexpr uint32_t SUB_TABLE_PHYS = 0x1D1ACC; // 0x801D1ACC

    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj & 0x1FFFFFFF;
    bool obj_ok = obj != 0 && lod_rdram_range_ok(obj_phys, 0x80);

    uint16_t obj_id = obj_ok ? ((uint16_t)lod_bgstate_s16(rdram, obj_phys + 0x00) & 0x07FF) : 0;
    uint16_t flags = obj_ok ? (uint16_t)lod_bgstate_s16(rdram, obj_phys + 0x02) : 0;
    uint8_t state09 = obj_ok ? lod_bgstate_u8(rdram, obj_phys + 0x09) : 0xFF;
    int16_t script_idx = obj_ok ? lod_bgstate_s16(rdram, obj_phys + 0x0E) : 0;
    uint32_t slot24 = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x24) : 0;
    uint32_t slot34 = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x34) : 0;

    auto read_pair = [&](int idx, uint8_t* count_out, uint8_t* cmd_out) {
        *count_out = 0;
        *cmd_out = 0;
        if (!obj_ok || idx < 0) {
            return;
        }
        uint32_t pair_phys = obj_phys + 0x08 + (uint32_t)idx * 2;
        if (!lod_rdram_range_ok(pair_phys, 2)) {
            return;
        }
        *count_out = lod_bgstate_u8(rdram, pair_phys + 0);
        *cmd_out = lod_bgstate_u8(rdram, pair_phys + 1);
    };

    uint8_t cur_count = 0, cur_cmd = 0, next_count = 0, next_cmd = 0;
    uint8_t next2_count = 0, next2_cmd = 0;
    read_pair(script_idx, &cur_count, &cur_cmd);
    read_pair(script_idx + 1, &next_count, &next_cmd);
    read_pair(script_idx + 2, &next2_count, &next2_cmd);

    uint32_t top_target = next_cmd < 16 ? lod_bgstate_u32(rdram, TOP_TABLE_PHYS + next_cmd * 4) : 0;
    uint32_t sub_target = next_cmd < 16 ? lod_bgstate_u32(rdram, SUB_TABLE_PHYS + next_cmd * 4) : 0;
    uint32_t sub_cur_target = cur_cmd < 16 ? lod_bgstate_u32(rdram, SUB_TABLE_PHYS + cur_cmd * 4) : 0;

    uint32_t sys2908 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2908);
    uint32_t sys295c = lod_bgstate_u32(rdram, SYS_PHYS + 0x295C);
    uint32_t sys2960 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2960);
    uint32_t sys2964 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2964);
    uint32_t sys2b10 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B10);
    uint32_t sys2b24 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B24);
    uint32_t ovlsys_18b4 = lod_bgstate_u32(rdram, 0x1D18B4); // 0x801D18B4
    uint32_t ovlsys_ada0 = lod_bgstate_u32(rdram, 0x1CADA0); // 0x801CADA0
    uint32_t ovlsys_ae88 = lod_bgstate_u32(rdram, 0x1DAE88); // 0x801DAE88

    uint8_t script_bytes[24] = {};
    if (obj_ok && lod_rdram_range_ok(obj_phys + 0x08, sizeof(script_bytes))) {
        for (size_t i = 0; i < sizeof(script_bytes); i++) {
            script_bytes[i] = lod_bgstate_u8(rdram, obj_phys + 0x08 + (uint32_t)i);
        }
    }

    fprintf(stderr,
        "[OVLSYS-SCRIPT] %s %-10s #%d vram=0x%08X gs=%d a0=0x%08X "
        "obj_ok=%d id=0x%03X flags=0x%04X state09=%u script_idx=%d "
        "cur=%02X/%02X sub_cur=0x%08X next=%02X/%02X top=0x%08X sub=0x%08X next2=%02X/%02X "
        "slot24=0x%08X slot34=0x%08X "
        "sys2908=0x%08X sys295C=0x%08X sys2960=0x%08X sys2964=0x%08X "
        "sys2B10=0x%08X sys2B24=0x%08X ovl18B4=0x%08X ovlADA0=0x%08X ovlAE88=0x%08X "
        "bytes08=%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X "
        "%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X "
        "v0=0x%08X ra=0x%08X\n",
        phase, label, count, vram, lod_current_gamestate(rdram), obj,
        obj_ok ? 1 : 0, obj_id, flags, state09, script_idx,
        cur_count, cur_cmd, sub_cur_target, next_count, next_cmd, top_target, sub_target,
        next2_count, next2_cmd, slot24, slot34, sys2908, sys295c, sys2960, sys2964,
        sys2b10, sys2b24, ovlsys_18b4, ovlsys_ada0, ovlsys_ae88,
        script_bytes[0], script_bytes[1], script_bytes[2], script_bytes[3],
        script_bytes[4], script_bytes[5], script_bytes[6], script_bytes[7],
        script_bytes[8], script_bytes[9], script_bytes[10], script_bytes[11],
        script_bytes[12], script_bytes[13], script_bytes[14], script_bytes[15],
        script_bytes[16], script_bytes[17], script_bytes[18], script_bytes[19],
        script_bytes[20], script_bytes[21], script_bytes[22], script_bytes[23],
        (uint32_t)ctx->r2, (uint32_t)ctx->r31);
}

static void lod_trace_overlay_script_common(uint8_t* rdram, recomp_context* ctx,
                                            const char* label, uint32_t vram,
                                            recomp_func_t* original, int* count) {
    (*count)++;
    int32_t gs = lod_current_gamestate(rdram);
    bool verbose = (*count <= 80) || (gs == 5 && (*count <= 200 || (*count % 120) == 0));
    if (verbose) {
        lod_trace_overlay_script_snapshot(rdram, ctx, label, vram, "enter", *count);
    }
    if (original != nullptr) {
        original(rdram, ctx);
    }
    if (verbose) {
        lod_trace_overlay_script_snapshot(rdram, ctx, label, vram, "exit ", *count);
    }
}

static uint32_t lod_dispatch_table_for_obj_id(uint16_t obj_id) {
    switch (obj_id) {
        case 0x007: return 0x0B37C4;
        case 0x008: return 0x0B3824;
        case 0x009: return 0x0B3834;
        case 0x00A: return 0x0B3848;
        case 0x016: return 0x197130;
        case 0x089: return 0x196110;
        case 0x0A2: return 0x0B4B70;
        case 0x106: return 0x0B5C90;
        case 0x185: return 0x0B7790;
        case 0x1AB: return 0x18D3B0;
        default: return 0;
    }
}

static void lod_trace_obj_ref(uint8_t* rdram, uint32_t obj_addr,
                              uint16_t* id_out, uint8_t* state_out,
                              int16_t* func_id_out, uint8_t* dispatch_out,
                              uint32_t* scheduler_out, uint32_t* target_out) {
    constexpr uint32_t OBJ_FUNC_TABLE_PHYS = 0x0AD640; // 0x800AD640
    uint32_t obj_phys = obj_addr & 0x1FFFFFFF;
    if (obj_addr == 0 || !lod_rdram_range_ok(obj_phys, 0x74)) {
        *id_out = 0;
        *state_out = 0xFF;
        *func_id_out = 0;
        *dispatch_out = 0xFF;
        *scheduler_out = 0;
        *target_out = 0;
        return;
    }

    *id_out = (uint16_t)lod_bgstate_s16(rdram, obj_phys + 0x00) & 0x07FF;
    *state_out = lod_bgstate_u8(rdram, obj_phys + 0x09);
    *func_id_out = lod_bgstate_s16(rdram, obj_phys + 0x0E);
    *dispatch_out = lod_object_dispatch_state(rdram, obj_phys);
    *scheduler_out = lod_bgstate_u32(rdram, OBJ_FUNC_TABLE_PHYS + *id_out * 4);
    *target_out = 0;
    uint32_t dispatch_table = lod_dispatch_table_for_obj_id(*id_out);
    if (dispatch_table != 0 && *dispatch_out < 64) {
        *target_out = lod_bgstate_u32(rdram, dispatch_table + *dispatch_out * 4);
    }
}

static void lod_trace_schedule_common(uint8_t* rdram, recomp_context* ctx,
                                      const char* label, uint32_t vram,
                                      uint32_t dispatch_table_phys,
                                      recomp_func_t* original, int* count) {
    (*count)++;

    constexpr uint32_t SYS_PHYS = 0x1C82C0;      // 0x801C82C0
    constexpr uint32_t BG_WORK_PHYS = 0x19D180;  // 0x8019D180
    constexpr uint32_t OBJ_FUNC_TABLE_PHYS = 0x0AD640; // 0x800AD640

    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj & 0x1FFFFFFF;
    bool obj_ok = obj != 0 && lod_rdram_range_ok(obj_phys, 0x74);
    uint16_t obj_id = obj_ok ? ((uint16_t)lod_bgstate_s16(rdram, obj_phys + 0x00) & 0x07FF) : 0;
    uint16_t obj_flags = obj_ok ? (uint16_t)lod_bgstate_s16(rdram, obj_phys + 0x02) : 0;
    uint8_t state = obj_ok ? lod_bgstate_u8(rdram, obj_phys + 0x09) : 0xFF;
    int16_t func_id = obj_ok ? lod_bgstate_s16(rdram, obj_phys + 0x0E) : 0;
    uint8_t dispatch_state = obj_ok ? lod_object_dispatch_state(rdram, obj_phys) : 0xFF;
    uint32_t dispatch_target = 0;
    if (dispatch_table_phys != 0 && dispatch_state < 64) {
        dispatch_target = lod_bgstate_u32(rdram, dispatch_table_phys + dispatch_state * 4);
    }
    uint32_t obj_func_target = lod_bgstate_u32(rdram, OBJ_FUNC_TABLE_PHYS + obj_id * 4);

    uint32_t child = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x1C) : 0;
    uint32_t slot34 = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x34) : 0;
    uint32_t slot38 = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x38) : 0;
    uint32_t slot3c = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x3C) : 0;
    uint32_t slot40 = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x40) : 0;
    uint32_t slot44 = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x44) : 0;
    uint32_t slot48 = obj_ok ? lod_bgstate_u32(rdram, obj_phys + 0x48) : 0;

    uint32_t sys2908 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2908);
    uint32_t sys290c = lod_bgstate_u32(rdram, SYS_PHYS + 0x290C);
    uint32_t sys2914 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2914);
    uint32_t sys291c = lod_bgstate_u32(rdram, SYS_PHYS + 0x291C);
    uint32_t sys2924 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2924);
    uint32_t sys292c = lod_bgstate_u32(rdram, SYS_PHYS + 0x292C);
    uint32_t sys2934 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2934);
    uint32_t sys293c = lod_bgstate_u32(rdram, SYS_PHYS + 0x293C);
    uint32_t sys2944 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2944);
    uint32_t sys294c = lod_bgstate_u32(rdram, SYS_PHYS + 0x294C);
    uint32_t sys295c = lod_bgstate_u32(rdram, SYS_PHYS + 0x295C);
    uint32_t sys2960 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2960);
    uint32_t sys2adc = lod_bgstate_u32(rdram, SYS_PHYS + 0x2ADC);
    uint32_t sys2b10 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B10);
    uint32_t sys2b20 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B20);
    uint32_t sys2b24 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B24);
    uint32_t sys2b70 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B70);
    uint32_t sys2bd0 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2BD0);
    int32_t gs = lod_current_gamestate(rdram);
    uint32_t bg14 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x14);
    uint32_t bg18 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x18);

    uint16_t s290c_id, s2914_id, s291c_id, s2924_id, s292c_id;
    uint16_t s2934_id, s293c_id, s2944_id, s294c_id, s2b70_id;
    uint8_t s290c_state, s2914_state, s291c_state, s2924_state, s292c_state;
    uint8_t s2934_state, s293c_state, s2944_state, s294c_state, s2b70_state;
    int16_t s290c_func, s2914_func, s291c_func, s2924_func, s292c_func;
    int16_t s2934_func, s293c_func, s2944_func, s294c_func, s2b70_func;
    uint8_t s290c_disp, s2914_disp, s291c_disp, s2924_disp, s292c_disp;
    uint8_t s2934_disp, s293c_disp, s2944_disp, s294c_disp, s2b70_disp;
    uint32_t s290c_sched, s2914_sched, s291c_sched, s2924_sched, s292c_sched;
    uint32_t s2934_sched, s293c_sched, s2944_sched, s294c_sched, s2b70_sched;
    uint32_t s290c_target, s2914_target, s291c_target, s2924_target, s292c_target;
    uint32_t s2934_target, s293c_target, s2944_target, s294c_target, s2b70_target;
    lod_trace_obj_ref(rdram, sys290c, &s290c_id, &s290c_state, &s290c_func, &s290c_disp, &s290c_sched, &s290c_target);
    lod_trace_obj_ref(rdram, sys2914, &s2914_id, &s2914_state, &s2914_func, &s2914_disp, &s2914_sched, &s2914_target);
    lod_trace_obj_ref(rdram, sys291c, &s291c_id, &s291c_state, &s291c_func, &s291c_disp, &s291c_sched, &s291c_target);
    lod_trace_obj_ref(rdram, sys2924, &s2924_id, &s2924_state, &s2924_func, &s2924_disp, &s2924_sched, &s2924_target);
    lod_trace_obj_ref(rdram, sys292c, &s292c_id, &s292c_state, &s292c_func, &s292c_disp, &s292c_sched, &s292c_target);
    lod_trace_obj_ref(rdram, sys2934, &s2934_id, &s2934_state, &s2934_func, &s2934_disp, &s2934_sched, &s2934_target);
    lod_trace_obj_ref(rdram, sys293c, &s293c_id, &s293c_state, &s293c_func, &s293c_disp, &s293c_sched, &s293c_target);
    lod_trace_obj_ref(rdram, sys2944, &s2944_id, &s2944_state, &s2944_func, &s2944_disp, &s2944_sched, &s2944_target);
    lod_trace_obj_ref(rdram, sys294c, &s294c_id, &s294c_state, &s294c_func, &s294c_disp, &s294c_sched, &s294c_target);
    lod_trace_obj_ref(rdram, sys2b70, &s2b70_id, &s2b70_state, &s2b70_func, &s2b70_disp, &s2b70_sched, &s2b70_target);

    bool interesting_obj =
        obj_id == 0x002 || obj_id == 0x003 || obj_id == 0x004 ||
        obj_id == 0x006 || obj_id == 0x009 || obj_id == 0x00A ||
        obj_id == 0x016 || obj_id == 0x089 || obj_id == 0x093 ||
        obj_id == 0x0A2 || obj_id == 0x106 || obj_id == 0x185 ||
        obj_id == 0x007 || obj_id == 0x008 || obj_id == 0x00E ||
        obj_id == 0x088 || obj_id == 0x08A || obj_id == 0x1AB ||
        obj_id == 0x1AC || obj_id == 0x1AD || obj_id == 0x1AE ||
        obj_id == 0x1B1 || obj_id == 0x1B2 || obj_id == 0x1D0;
    bool verbose = (*count <= 80) ||
                   (gs == 5 && (interesting_obj || *count <= 160 || (*count % 120) == 0)) ||
                   (*count % 1000) == 0;

    if (verbose) {
        fprintf(stderr,
            "[G5-OBJ] enter %-14s #%d vram=0x%08X gs=%d a0=0x%08X obj_ok=%d "
            "id=0x%03X flags=0x%04X state09=%u func_id=%d dispatch=%u "
            "obj_func=0x%08X target=0x%08X child=0x%08X "
            "slots34..48=%08X,%08X,%08X,%08X,%08X,%08X "
            "sys2908=0x%08X sys290C=0x%08X sys2914=0x%08X sys2924=0x%08X "
            "sys295C=0x%08X sys2960=0x%08X sys2ADC=0x%08X sys2B10=0x%08X "
            "sys2B20=0x%08X sys2B24=0x%08X sys2BD0=0x%08X bg14=0x%08X bg18=0x%08X "
            "sysObjs{290C=%03X/%u/%d/%u 2914=%03X/%u/%d/%u 291C=%03X/%u/%d/%u "
            "2924=%03X/%u/%d/%u 292C=%03X/%u/%d/%u 2934=%03X/%u/%d/%u "
            "293C=%03X/%u/%d/%u 2944=%03X/%u/%d/%u 294C=%03X/%u/%d/%u 2B70=%03X/%u/%d/%u} "
            "ra=0x%08X\n",
            label, *count, vram, gs, obj, obj_ok ? 1 : 0,
            obj_id, obj_flags, state, func_id, dispatch_state, obj_func_target,
            dispatch_target, child, slot34, slot38, slot3c, slot40, slot44, slot48,
            sys2908, sys290c, sys2914, sys2924, sys295c, sys2960, sys2adc, sys2b10,
            sys2b20, sys2b24, sys2bd0, bg14, bg18,
            s290c_id, s290c_state, s290c_func, s290c_disp,
            s2914_id, s2914_state, s2914_func, s2914_disp,
            s291c_id, s291c_state, s291c_func, s291c_disp,
            s2924_id, s2924_state, s2924_func, s2924_disp,
            s292c_id, s292c_state, s292c_func, s292c_disp,
            s2934_id, s2934_state, s2934_func, s2934_disp,
            s293c_id, s293c_state, s293c_func, s293c_disp,
            s2944_id, s2944_state, s2944_func, s2944_disp,
            s294c_id, s294c_state, s294c_func, s294c_disp,
            s2b70_id, s2b70_state, s2b70_func, s2b70_disp,
            (uint32_t)ctx->r31);

        if (gs == 5 && (strcmp(label, "sys_dispatch") == 0 || *count <= 40 || (*count % 120) == 0)) {
            fprintf(stderr,
                "[G5-SYSOBJS] #%d "
                "290C=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "2914=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "291C=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "2924=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "292C=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "2934=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "293C=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "2944=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "294C=%03X/%u/%d/%u sched=0x%08X target=0x%08X "
                "2B70=%03X/%u/%d/%u sched=0x%08X target=0x%08X\n",
                *count,
                s290c_id, s290c_state, s290c_func, s290c_disp, s290c_sched, s290c_target,
                s2914_id, s2914_state, s2914_func, s2914_disp, s2914_sched, s2914_target,
                s291c_id, s291c_state, s291c_func, s291c_disp, s291c_sched, s291c_target,
                s2924_id, s2924_state, s2924_func, s2924_disp, s2924_sched, s2924_target,
                s292c_id, s292c_state, s292c_func, s292c_disp, s292c_sched, s292c_target,
                s2934_id, s2934_state, s2934_func, s2934_disp, s2934_sched, s2934_target,
                s293c_id, s293c_state, s293c_func, s293c_disp, s293c_sched, s293c_target,
                s2944_id, s2944_state, s2944_func, s2944_disp, s2944_sched, s2944_target,
                s294c_id, s294c_state, s294c_func, s294c_disp, s294c_sched, s294c_target,
                s2b70_id, s2b70_state, s2b70_func, s2b70_disp, s2b70_sched, s2b70_target);
        }
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }

    if (verbose) {
        uint8_t after_dispatch = obj_ok ? lod_object_dispatch_state(rdram, obj_phys) : 0xFF;
        uint32_t after_sys2b10 = lod_bgstate_u32(rdram, SYS_PHYS + 0x2B10);
        uint32_t after_bg14 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x14);
        uint32_t after_bg18 = lod_bgstate_u32(rdram, BG_WORK_PHYS + 0x18);
        fprintf(stderr,
            "[G5-OBJ] exit  %-14s #%d gs=%d a0=0x%08X dispatch=%u->%u "
            "v0=0x%08X sys2B10=0x%08X->0x%08X bg14=0x%08X->0x%08X bg18=0x%08X->0x%08X ra=0x%08X\n",
            label, *count, lod_current_gamestate(rdram), obj, dispatch_state, after_dispatch,
            (uint32_t)ctx->r2, sys2b10, after_sys2b10, bg14, after_bg14, bg18, after_bg18,
            (uint32_t)ctx->r31);
    }
}

static void lod_trace_bgstate_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "bg_driver", 0x80145AF0, 0x18D3B0,
                              lod_orig_bgstate_driver, &count);
}

static void lod_trace_bgstate_bootstrap(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "bg_bootstrap", 0x80145B60, 0x18D3B0,
                              lod_orig_bgstate_bootstrap, &count);
}

static void lod_trace_bgstate_init(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_bgstate_common(rdram, ctx, "init", 0x80145BD0, lod_orig_bgstate_init, &count);
}

static void lod_trace_bgstate_load(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_bgstate_common(rdram, ctx, "load", 0x80145DA8, lod_orig_bgstate_load, &count);
}

static void lod_trace_bgstate_update_a(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_bgstate_common(rdram, ctx, "update_a", 0x80145FD4, lod_orig_bgstate_update_a, &count);
}

static void lod_trace_bgstate_update_b(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_bgstate_common(rdram, ctx, "update_b", 0x801460D4, lod_orig_bgstate_update_b, &count);
}

static void lod_trace_bgstate_create(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_bgstate_common(rdram, ctx, "create", 0x8014615C, lod_orig_bgstate_create, &count);
}

static void lod_trace_bgstate_activate(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_bgstate_common(rdram, ctx, "activate", 0x80146240, lod_orig_bgstate_activate, &count);
}

static void lod_trace_scene5_obj7_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "scene5_obj7", 0x8001A5BC, 0x0B37C4,
                              lod_orig_scene5_obj7_driver, &count);
}

static void lod_trace_scene5_aux_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "scene5_aux", 0x8001AA48, 0x0B37DC,
                              lod_orig_scene5_aux_driver, &count);
}

static void lod_trace_scene5_sys_dispatch(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_dispatch", 0x8001AB28, 0,
                              lod_orig_scene5_sys_dispatch, &count);
}

static void lod_trace_scene5_obj8_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "scene5_obj8", 0x8001B4C0, 0x0B3824,
                              lod_orig_scene5_obj8_driver, &count);
}

static void lod_trace_scene5_obj8_load(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "scene5_load", 0x8001B530, 0x0B3824,
                              lod_orig_scene5_obj8_load, &count);
}

static void lod_trace_overlay_system_create(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "ovl_sys_create", 0x8001BA78, 0,
                              lod_orig_overlay_system_create, &count);
}

static void lod_trace_sys_obj009_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_obj009", 0x8001B718, 0x0B3834,
                              lod_orig_sys_obj009_driver, &count);
}

static void lod_trace_sys_obj00a_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_obj00A", 0x8001BC5C, 0x0B3848,
                              lod_orig_sys_obj00a_driver, &count);
}

static void lod_trace_sys_obj0a2_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_obj0A2", 0x80037730, 0x0B4B70,
                              lod_orig_sys_obj0a2_driver, &count);
}

static void lod_trace_sys_obj106_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_obj106", 0x80058890, 0x0B5C90,
                              lod_orig_sys_obj106_driver, &count);
}

static void lod_trace_sys_obj185_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_obj185", 0x80086120, 0x0B7790,
                              lod_orig_sys_obj185_driver, &count);
}

static void lod_trace_sys_obj089_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_obj089", 0x8017E00C, 0x196110,
                              lod_orig_sys_obj089_driver, &count);
}

static void lod_trace_sys_obj016_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_schedule_common(rdram, ctx, "sys_obj016", 0x80189050, 0x197130,
                              lod_orig_sys_obj016_driver, &count);
}

static void lod_trace_sys_obj093_driver(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    count++;
    int32_t gs = lod_current_gamestate(rdram);
    bool verbose = count <= 80 || (gs == 5 && (count <= 200 || (count % 120) == 0));
    if (verbose) {
        lod_trace_overlay_script_snapshot(rdram, ctx, "sys_obj093", 0x801CAEA0, "enter", count);
    }
    count--;
    lod_trace_schedule_common(rdram, ctx, "sys_obj093", 0x801CAEA0, 0,
                              lod_orig_sys_obj093_driver, &count);
    if (verbose) {
        lod_trace_overlay_script_snapshot(rdram, ctx, "sys_obj093", 0x801CAEA0, "exit ", count);
    }
}

static void lod_trace_ovlsys_script_outer(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_overlay_script_common(rdram, ctx, "script0", 0x801CB180,
                                    lod_orig_ovlsys_script_outer, &count);
}

static void lod_trace_ovlsys_script_alloc(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_overlay_script_common(rdram, ctx, "alloc", 0x801CB404,
                                    lod_orig_ovlsys_script_alloc, &count);
}

static void lod_trace_ovlsys_script_wait_a(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_overlay_script_common(rdram, ctx, "wait_a", 0x801CB4CC,
                                    lod_orig_ovlsys_script_wait_a, &count);
}

static void lod_trace_ovlsys_script_wait_b(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_overlay_script_common(rdram, ctx, "wait_b", 0x801CB550,
                                    lod_orig_ovlsys_script_wait_b, &count);
}

static void lod_trace_ovlsys_ni_init(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_overlay_script_common(rdram, ctx, "ni_init", 0x801CB5CC,
                                    lod_orig_ovlsys_ni_init, &count);
}

static void lod_trace_ovlsys_script_done(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    lod_trace_overlay_script_common(rdram, ctx, "done", 0x801CBC78,
                                    lod_orig_ovlsys_script_done, &count);
}

static void lod_install_bgstate_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                              recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

static void lod_install_bgstate_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    lod_install_bgstate_trace_wrapper(0x80145AF0, lod_trace_bgstate_driver, &lod_orig_bgstate_driver);
    lod_install_bgstate_trace_wrapper(0x80145B60, lod_trace_bgstate_bootstrap, &lod_orig_bgstate_bootstrap);
    lod_install_bgstate_trace_wrapper(0x80145BD0, lod_trace_bgstate_init, &lod_orig_bgstate_init);
    lod_install_bgstate_trace_wrapper(0x80145DA8, lod_trace_bgstate_load, &lod_orig_bgstate_load);
    lod_install_bgstate_trace_wrapper(0x80145FD4, lod_trace_bgstate_update_a, &lod_orig_bgstate_update_a);
    lod_install_bgstate_trace_wrapper(0x801460D4, lod_trace_bgstate_update_b, &lod_orig_bgstate_update_b);
    lod_install_bgstate_trace_wrapper(0x8014615C, lod_trace_bgstate_create, &lod_orig_bgstate_create);
    lod_install_bgstate_trace_wrapper(0x80146240, lod_trace_bgstate_activate, &lod_orig_bgstate_activate);
    lod_install_bgstate_trace_wrapper(0x8001A5BC, lod_trace_scene5_obj7_driver, &lod_orig_scene5_obj7_driver);
    lod_install_bgstate_trace_wrapper(0x8001AA48, lod_trace_scene5_aux_driver, &lod_orig_scene5_aux_driver);
    lod_install_bgstate_trace_wrapper(0x8001AB28, lod_trace_scene5_sys_dispatch, &lod_orig_scene5_sys_dispatch);
    lod_install_bgstate_trace_wrapper(0x8001B4C0, lod_trace_scene5_obj8_driver, &lod_orig_scene5_obj8_driver);
    lod_install_bgstate_trace_wrapper(0x8001B530, lod_trace_scene5_obj8_load, &lod_orig_scene5_obj8_load);
    lod_install_bgstate_trace_wrapper(0x8001BA78, lod_trace_overlay_system_create, &lod_orig_overlay_system_create);
    lod_install_bgstate_trace_wrapper(0x8001B718, lod_trace_sys_obj009_driver, &lod_orig_sys_obj009_driver);
    lod_install_bgstate_trace_wrapper(0x8001BC5C, lod_trace_sys_obj00a_driver, &lod_orig_sys_obj00a_driver);
    lod_install_bgstate_trace_wrapper(0x80037730, lod_trace_sys_obj0a2_driver, &lod_orig_sys_obj0a2_driver);
    lod_install_bgstate_trace_wrapper(0x80058890, lod_trace_sys_obj106_driver, &lod_orig_sys_obj106_driver);
    lod_install_bgstate_trace_wrapper(0x80086120, lod_trace_sys_obj185_driver, &lod_orig_sys_obj185_driver);
    lod_install_bgstate_trace_wrapper(0x8017E00C, lod_trace_sys_obj089_driver, &lod_orig_sys_obj089_driver);
    lod_install_bgstate_trace_wrapper(0x80189050, lod_trace_sys_obj016_driver, &lod_orig_sys_obj016_driver);
    lod_install_bgstate_trace_wrapper(0x801CAEA0, lod_trace_sys_obj093_driver, &lod_orig_sys_obj093_driver);
    lod_install_bgstate_trace_wrapper(0x801CB180, lod_trace_ovlsys_script_outer, &lod_orig_ovlsys_script_outer);
    lod_install_bgstate_trace_wrapper(0x801CB404, lod_trace_ovlsys_script_alloc, &lod_orig_ovlsys_script_alloc);
    lod_install_bgstate_trace_wrapper(0x801CB4CC, lod_trace_ovlsys_script_wait_a, &lod_orig_ovlsys_script_wait_a);
    lod_install_bgstate_trace_wrapper(0x801CB550, lod_trace_ovlsys_script_wait_b, &lod_orig_ovlsys_script_wait_b);
    lod_install_bgstate_trace_wrapper(0x801CB5CC, lod_trace_ovlsys_ni_init, &lod_orig_ovlsys_ni_init);
    lod_install_bgstate_trace_wrapper(0x801CBC78, lod_trace_ovlsys_script_done, &lod_orig_ovlsys_script_done);

    if (install_count <= 4) {
        fprintf(stderr,
            "[BGSTATE] installed trace wrappers #%d reason=%s originals "
            "driver=%p bootstrap=%p init=%p load=%p update_a=%p update_b=%p create=%p activate=%p "
            "obj7=%p aux=%p sysdisp=%p obj8=%p load=%p ovlcreate=%p\n",
            install_count, reason,
            (void*)lod_orig_bgstate_driver,
            (void*)lod_orig_bgstate_bootstrap,
            (void*)lod_orig_bgstate_init,
            (void*)lod_orig_bgstate_load,
            (void*)lod_orig_bgstate_update_a,
            (void*)lod_orig_bgstate_update_b,
            (void*)lod_orig_bgstate_create,
            (void*)lod_orig_bgstate_activate,
            (void*)lod_orig_scene5_obj7_driver,
            (void*)lod_orig_scene5_aux_driver,
            (void*)lod_orig_scene5_sys_dispatch,
            (void*)lod_orig_scene5_obj8_driver,
            (void*)lod_orig_scene5_obj8_load,
            (void*)lod_orig_overlay_system_create);
    }
}

extern "C" void lod_install_bgstate_trace_wrappers_early() {
    lod_install_bgstate_trace_wrappers("lod-on-init");
}
#endif

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
#if LOD_ENABLE_BGSTATE_TRACE
    if (si_dma_count <= 20 || (si_dma_count % 60) == 0) {
        lod_install_bgstate_trace_wrappers("si-refresh");
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

#if LOD_ENABLE_BOOT_INPUT_SCRIPT || LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT
                // Debug-only boot automation. This is not a gamestate skip:
                // it exercises the same controller/PIF path as real input.
                static int32_t boot_script_last_gs = INT32_MIN;
                static int boot_script_gs_frame = 0;
                static bool fast_script_reached_idle_target = false;
                int32_t boot_script_gs = lod_current_gamestate(rdram);
                if (boot_script_gs != boot_script_last_gs) {
                    boot_script_last_gs = boot_script_gs;
                    boot_script_gs_frame = 0;
                    fprintf(stderr, "[AUTO] Enter gs=%d input script\n", boot_script_gs);
                }
                boot_script_gs_frame++;

#if LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT
                // Fast debug-only controller script for KSEG0/idle repros.
                // It aggressively presses Start/A through skippable menus and
                // cutscenes, then stops once the known low-res gameplay-idle
                // signature is reached so the actual idle test is hands-off.
                bool fast_at_idle_target = lod_fast_gameplay_idle_target(rdram, boot_script_gs);
                if (fast_at_idle_target) {
                    if (!fast_script_reached_idle_target) {
                        fast_script_reached_idle_target = true;
                        fprintf(stderr,
                                "[AUTO] fast script reached idle target gs=3 exec=0x%08X ni=0x%08X; releasing input\n",
                                lod_current_exec_flags(rdram), lod_current_ni_sys_ptr(rdram));
                    }
                } else {
                    bool pulse = false;
                    uint16_t fast_buttons = 0x9000; // A + Start

                    if (boot_script_gs == 12) {
                        // Expansion Pak acknowledgement: quick repeated A/Start.
                        pulse = (boot_script_gs_frame % 24) >= 6 &&
                                (boot_script_gs_frame % 24) < 12;
                    } else if (boot_script_gs == 5 || boot_script_gs == 6 ||
                               boot_script_gs == 8 || boot_script_gs == -3 ||
                               boot_script_gs == 3) {
                        // Menus, title, character select, book/prologue, and
                        // early scene transitions: skip/advance aggressively.
                        pulse = (boot_script_gs_frame % 18) >= 3 &&
                                (boot_script_gs_frame % 18) < 8;
                    }

                    if (pulse) {
                        buttons |= fast_buttons;
                        if (boot_script_gs_frame <= 120 &&
                            (boot_script_gs_frame % 18) == 3) {
                            fprintf(stderr,
                                    "[AUTO] fast gs=%d frame %d: A+Start\n",
                                    boot_script_gs, boot_script_gs_frame);
                        }
                    }
                }
#else
                if (boot_script_gs == 12) {
                    // Expansion Pak acknowledgement screen. Pulse both A and
                    // Start so either accepted prompt advances, then release.
                    bool pulse = (boot_script_gs_frame >= 30 && boot_script_gs_frame < 36) ||
                                 (boot_script_gs_frame >= 90 && boot_script_gs_frame < 96) ||
                                 (boot_script_gs_frame >= 150 && boot_script_gs_frame < 156);
                    if (pulse) {
                        buttons |= 0x9000; // A + Start
                        if (boot_script_gs_frame == 30 ||
                            boot_script_gs_frame == 90 ||
                            boot_script_gs_frame == 150) {
                            fprintf(stderr, "[AUTO] gs=12 frame %d: A+Start\n", boot_script_gs_frame);
                        }
                    }
                } else if (boot_script_gs == 5) {
                    // Intro/cutscene/title-attract candidate. Pulse Start/A at
                    // intervals to test whether this path is merely skippable.
                    bool pulse = (boot_script_gs_frame >= 60 && boot_script_gs_frame < 66) ||
                                 (boot_script_gs_frame >= 180 && boot_script_gs_frame < 186) ||
                                 (boot_script_gs_frame >= 360 && boot_script_gs_frame < 366) ||
                                 (boot_script_gs_frame >= 720 && boot_script_gs_frame < 726);
                    if (pulse) {
                        buttons |= 0x9000; // A + Start
                        if (boot_script_gs_frame == 60 ||
                            boot_script_gs_frame == 180 ||
                            boot_script_gs_frame == 360 ||
                            boot_script_gs_frame == 720) {
                            fprintf(stderr, "[AUTO] gs=5 frame %d: A+Start\n", boot_script_gs_frame);
                        }
                    }
                } else if (boot_script_gs == 6) {
                    // Title / main-menu path. Pulse Start+A without changing
                    // gamestate directly; if the menu is already open, A
                    // should select the default highlighted item.
                    bool pulse = (boot_script_gs_frame >= 60 && boot_script_gs_frame < 66) ||
                                 (boot_script_gs_frame >= 150 && boot_script_gs_frame < 156) ||
                                 (boot_script_gs_frame >= 300 && boot_script_gs_frame < 306) ||
                                 (boot_script_gs_frame >= 600 && boot_script_gs_frame < 606) ||
                                 (boot_script_gs_frame >= 900 && boot_script_gs_frame < 906);
                    if (pulse) {
                        buttons |= 0x9000; // A + Start
                        if (boot_script_gs_frame == 60 ||
                            boot_script_gs_frame == 150 ||
                            boot_script_gs_frame == 300 ||
                            boot_script_gs_frame == 600 ||
                            boot_script_gs_frame == 900) {
                            fprintf(stderr, "[AUTO] gs=6 frame %d: A+Start\n", boot_script_gs_frame);
                        }
                    }
                } else if (boot_script_gs == 8) {
                    // New-game/character-select/book sequence. Use sparse A
                    // pulses so this remains regular controller interaction.
                    bool pulse = (boot_script_gs_frame >= 60 && boot_script_gs_frame < 66) ||
                                 (boot_script_gs_frame >= 180 && boot_script_gs_frame < 186) ||
                                 (boot_script_gs_frame >= 360 && boot_script_gs_frame < 366) ||
                                 (boot_script_gs_frame >= 720 && boot_script_gs_frame < 726) ||
                                 (boot_script_gs_frame >= 1200 && boot_script_gs_frame < 1206);
                    if (pulse) {
                        buttons |= 0x8000; // A
                        if (boot_script_gs_frame == 60 ||
                            boot_script_gs_frame == 180 ||
                            boot_script_gs_frame == 360 ||
                            boot_script_gs_frame == 720 ||
                            boot_script_gs_frame == 1200) {
                            fprintf(stderr, "[AUTO] gs=8 frame %d: A\n", boot_script_gs_frame);
                        }
                    }
                }
#endif
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
