// Stubs/shims for N64 OS functions that are in N64Recomp's ignored_funcs list.
// The recompiler doesn't generate code for these, but other recompiled OS
// internal functions still reference them via _recomp suffix. Most are handled
// natively by ultramodern, but verify each symbol before leaving it empty:
// some LoD functions have misleading libultra names and still mutate game data.

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

#ifndef LOD_SKIP_EXPANSION_PAK_SCREEN
#define LOD_SKIP_EXPANSION_PAK_SCREEN 0
#endif

#ifndef LOD_AUTO_ADVANCE_EXPANSION_PAK_SCREEN
#define LOD_AUTO_ADVANCE_EXPANSION_PAK_SCREEN 0
#endif

#ifndef LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT
#define LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT 0
#endif

#ifndef LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT
#define LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT 0
#endif

#ifndef LOD_ITEM_MENU_SAVE_SLOT
#define LOD_ITEM_MENU_SAVE_SLOT 1
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

#ifndef LOD_ENABLE_B2_ASSET_TRACE
#define LOD_ENABLE_B2_ASSET_TRACE 0
#endif

#ifndef LOD_MAP76_TRACE_SI_INTERVAL
#define LOD_MAP76_TRACE_SI_INTERVAL 6
#endif

#ifndef LOD_MAP76_BOSS_FIG_INTERVAL
#define LOD_MAP76_BOSS_FIG_INTERVAL 10
#endif

#ifndef LOD_ENABLE_ITEM_KSEG0_TRACE
#define LOD_ENABLE_ITEM_KSEG0_TRACE 0
#endif

#ifndef LOD_FIX_NULL_OBJECT_DISPATCH_CHILD
#define LOD_FIX_NULL_OBJECT_DISPATCH_CHILD 1
#endif

#ifndef LOD_ENABLE_INPUT_TRACE
#define LOD_ENABLE_INPUT_TRACE 0
#endif

#ifndef LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE
#define LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE 0
#endif

#ifndef LOD_ENABLE_NI24_WRITER_TRACE
#define LOD_ENABLE_NI24_WRITER_TRACE 0
#endif

#ifndef LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
#define LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS 0
#endif

#ifndef LOD_ENABLE_AUDIO_PULL_TRACE
#define LOD_ENABLE_AUDIO_PULL_TRACE 0
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
#if LOD_ENABLE_KSEG0_FAULT_TRACE || LOD_ENABLE_B2_ASSET_TRACE
static void lod_install_kseg0_fault_trace_wrappers(const char* reason);
#endif
#if LOD_ENABLE_ITEM_KSEG0_TRACE
extern "C" void lod_install_item_kseg0_trace_wrappers_early();
#endif
#if LOD_FIX_NULL_OBJECT_DISPATCH_CHILD
extern "C" void lod_install_null_object_dispatch_child_guard_early();
#endif
#if LOD_ENABLE_INPUT_TRACE
extern "C" void lod_install_input_action_trace_wrappers_early();
#endif
#if LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE
static void lod_install_fog_lake_trigger_trace_wrappers(const char* reason);
#endif
#if LOD_ENABLE_B2_ASSET_TRACE
static void lod_install_map76_boss_lifecycle_trace_wrappers(const char* reason);
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
#if LOD_ENABLE_B2_ASSET_TRACE
static constexpr uint32_t LOD_B2_TRACE_MAP_ROM = 0x0076CD00;
static constexpr uint32_t LOD_B2_TRACE_ROM = 0x00C3FE4E;
static constexpr uint32_t LOD_B2_TRACE_RAM = 0x802A3B70;
static constexpr uint32_t LOD_B2_TRACE_SIZE = 0x0000F3B2;
static constexpr uint32_t LOD_B2_TRACE_FILE = 0x000000B2;
#endif
static uint32_t lod_current_map_ovl_rom = 0;
static uint32_t lod_current_map_ovl_size = 0;
static int lod_map_ovl_load_count = 0;

extern "C" uint32_t lod_current_map_overlay_rom() {
    return lod_current_map_ovl_rom;
}

extern "C" uint32_t lod_current_map_overlay_size() {
    return lod_current_map_ovl_size;
}

extern "C" int lod_current_map_overlay_load_count() {
    return lod_map_ovl_load_count;
}

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
#if LOD_ENABLE_B2_ASSET_TRACE
                if (rom_start == LOD_B2_TRACE_MAP_ROM) {
                    const LodKseg0FaultTraceSnapshot& s = lod_kseg0_fault_trace_snapshot;
                    fprintf(stderr,
                            "[B2_ASSET] map76-loaded map#%d rom=0x%08X size=0x%X requested=0x%X "
                            "last_dma={count=%u gs=%u rom=0x%08X ram=0x%08X size=0x%08X phys=0x%08X ok=%u}\n",
                            lod_map_ovl_load_count, rom_start, full_size, size,
                            s.romcopy_count, s.romcopy_gs, s.romcopy_rom, s.romcopy_ram,
                            s.romcopy_size, s.romcopy_ram_phys, s.romcopy_ram_ok);
                    lod_install_map76_boss_lifecycle_trace_wrappers("map76-load");
                }
#endif
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
#if LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE
                if (rom_start == 0x007AE430) {
                    lod_install_fog_lake_trigger_trace_wrappers("map_ovl_11-load");
                }
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

// ── OS thread/interrupt stubs and verified ignored-function shims ────────────

static bool lod_recomp_list_addr_ok(uint32_t addr) {
    uint32_t phys = 0;
    return lod_decode_rdram_phys_addr(addr, 8, &phys);
}

void __osDequeueThread_recomp(uint8_t* rdram, recomp_context* ctx) {
    // LoD's 0x80096EC0 implementation is the libultra doubly-linked-list
    // unlink helper used by the audio synth (ALLink/PVoice queues), despite
    // the symbol name. Leaving this empty prevents alSynNew from ever
    // maintaining synth voice lists, so alSynAllocVoice always fails.
    gpr node = ctx->r4;
    uint32_t node_addr = (uint32_t)node;
    if (node_addr == 0 || !lod_recomp_list_addr_ok(node_addr)) {
#if LOD_ENABLE_AUDIO_PULL_TRACE
        static unsigned bad_dequeue_count = 0;
        if (++bad_dequeue_count <= 16) {
            fprintf(stderr, "[OS_LIST] dequeue skipped invalid node=0x%08X\n", node_addr);
        }
#endif
        return;
    }

    gpr next = (gpr)(int32_t)MEM_W(0x0, node);
    gpr prev = (gpr)(int32_t)MEM_W(0x4, node);

    if ((uint32_t)next != 0 && lod_recomp_list_addr_ok((uint32_t)next)) {
        MEM_W(0x4, next) = (int32_t)prev;
    }
    if ((uint32_t)prev != 0 && lod_recomp_list_addr_ok((uint32_t)prev)) {
        MEM_W(0x0, prev) = (int32_t)next;
    }

#if LOD_ENABLE_AUDIO_PULL_TRACE
    static unsigned dequeue_count = 0;
    if (++dequeue_count <= 40 || (dequeue_count % 500) == 0) {
        fprintf(stderr, "[OS_LIST] dequeue#%u node=0x%08X next=0x%08X prev=0x%08X\n",
                dequeue_count, node_addr, (uint32_t)next, (uint32_t)prev);
    }
#endif
}

void __osEnqueueThread_recomp(uint8_t* rdram, recomp_context* ctx) {
    // Original 0x80096EF0:
    //   old = *head; node->prev = head; node->next = old;
    //   if (old) old->prev = node; *head = node;
    gpr node = ctx->r4;
    gpr head = ctx->r5;
    uint32_t node_addr = (uint32_t)node;
    uint32_t head_addr = (uint32_t)head;
    if (node_addr == 0 || head_addr == 0 ||
        !lod_recomp_list_addr_ok(node_addr) ||
        !lod_recomp_list_addr_ok(head_addr)) {
#if LOD_ENABLE_AUDIO_PULL_TRACE
        static unsigned bad_enqueue_count = 0;
        if (++bad_enqueue_count <= 16) {
            fprintf(stderr, "[OS_LIST] enqueue skipped invalid node=0x%08X head=0x%08X\n",
                    node_addr, head_addr);
        }
#endif
        return;
    }

    gpr old = (gpr)(int32_t)MEM_W(0x0, head);
    MEM_W(0x4, node) = (int32_t)head;
    MEM_W(0x0, node) = (int32_t)old;
    if ((uint32_t)old != 0 && lod_recomp_list_addr_ok((uint32_t)old)) {
        MEM_W(0x4, old) = (int32_t)node;
    }
    MEM_W(0x0, head) = (int32_t)node;

#if LOD_ENABLE_AUDIO_PULL_TRACE
    static unsigned enqueue_count = 0;
    if (++enqueue_count <= 40 || (enqueue_count % 500) == 0) {
        fprintf(stderr, "[OS_LIST] enqueue#%u node=0x%08X head=0x%08X old=0x%08X newHead=0x%08X\n",
                enqueue_count, node_addr, head_addr, (uint32_t)old, (uint32_t)MEM_W(0x0, head));
    }
#endif
}
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

#if LOD_ENABLE_AUDIO_PULL_TRACE
static inline bool lod_rdram_range_ok(uint32_t phys, uint32_t size);
static inline uint8_t lod_rdram_u8(uint8_t* rdram, uint32_t phys);
static inline int16_t lod_rdram_s16(uint8_t* rdram, uint32_t phys);
static inline uint32_t lod_rdram_u32(uint8_t* rdram, uint32_t phys);

static recomp_func_t* lod_orig_audio_al_syn_set_vol = nullptr;            // 0x800973A0
static recomp_func_t* lod_orig_audio_al_syn_alloc_voice = nullptr;        // 0x80099278
static recomp_func_t* lod_orig_audio_al_syn_start_voice = nullptr;        // 0x800999B0
static recomp_func_t* lod_orig_audio_al_syn_set_pitch = nullptr;          // 0x8009B1A0
static recomp_func_t* lod_orig_audio_al_syn_set_pan = nullptr;            // 0x8009B940
static recomp_func_t* lod_orig_audio_dma_callback = nullptr;              // 0x8009092C
static recomp_func_t* lod_orig_audio_main_bus_pull = nullptr;   // 0x8009D5F0
static recomp_func_t* lod_orig_audio_fx_pull = nullptr;         // 0x8009D710
static recomp_func_t* lod_orig_audio_aux_bus_pull = nullptr;    // 0x8009E3A0
static recomp_func_t* lod_orig_audio_envmixer_pull = nullptr;   // 0x8009E480
static recomp_func_t* lod_orig_audio_envmixer_param = nullptr;  // 0x8009E988
static recomp_func_t* lod_orig_audio_resample_pull = nullptr;   // 0x8009F1EC
static recomp_func_t* lod_orig_audio_raw16_pull = nullptr;      // 0x8009F7A4
static recomp_func_t* lod_orig_audio_adpcm_pull = nullptr;      // 0x8009FC7C
static recomp_func_t* lod_orig_audio_al_syn_set_fx_mix = nullptr;         // 0x800A1540
static recomp_func_t* lod_orig_audio_al_syn_start_voice_params = nullptr; // 0x800A4DF0
static recomp_func_t* lod_orig_audio_al_syn_stop_voice = nullptr;         // 0x800A71B0

static inline uint32_t lod_audio_phys(uint32_t addr) {
    return addr & 0x1FFFFFFF;
}

static inline bool lod_audio_addr_ok(uint32_t addr, uint32_t size) {
    return addr != 0 && lod_rdram_range_ok(lod_audio_phys(addr), size);
}

static uint32_t lod_audio_u32(uint8_t* rdram, uint32_t addr, uint32_t off) {
    const uint32_t phys = lod_audio_phys(addr) + off;
    return (addr != 0 && lod_rdram_range_ok(phys, 4)) ? lod_rdram_u32(rdram, phys) : 0;
}

static int16_t lod_audio_s16(uint8_t* rdram, uint32_t addr, uint32_t off) {
    const uint32_t phys = lod_audio_phys(addr) + off;
    return (addr != 0 && lod_rdram_range_ok(phys, 2)) ? lod_rdram_s16(rdram, phys) : 0;
}

static uint16_t lod_audio_u16(uint8_t* rdram, uint32_t addr, uint32_t off) {
    return (uint16_t)lod_audio_s16(rdram, addr, off);
}

static uint8_t lod_audio_u8(uint8_t* rdram, uint32_t addr, uint32_t off) {
    const uint32_t phys = lod_audio_phys(addr) + off;
    return (addr != 0 && lod_rdram_range_ok(phys, 1)) ? lod_rdram_u8(rdram, phys) : 0;
}

static uint32_t lod_audio_min_u32(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}

static uint32_t lod_audio_count_nonzero_phys(uint8_t* rdram, uint32_t phys, uint32_t size) {
    if (!lod_rdram_range_ok(phys, size)) {
        return 0;
    }

    uint32_t nonzero = 0;
    for (uint32_t i = 0; i < size; i++) {
        if (lod_rdram_u8(rdram, phys + i) != 0) {
            nonzero++;
        }
    }
    return nonzero;
}

static void lod_audio_dump_head_phys(uint8_t* rdram, uint32_t phys, uint32_t size) {
    const uint32_t n = lod_audio_min_u32(size, 16);
    if (!lod_rdram_range_ok(phys, n)) {
        fprintf(stderr, " <bad>");
        return;
    }
    for (uint32_t i = 0; i < n; i++) {
        fprintf(stderr, " %02X", lod_rdram_u8(rdram, phys + i));
    }
}

struct LodAudioDmaCacheEntry {
    bool ok = false;
    uint32_t entry = 0;
    uint32_t next = 0;
    uint32_t prev = 0;
    uint32_t base = 0;
    uint32_t age = 0;
    uint32_t buffer = 0;
};

static LodAudioDmaCacheEntry lod_audio_dma_find_cache_entry(uint8_t* rdram,
                                                            uint32_t head,
                                                            uint32_t addr,
                                                            uint32_t size) {
    LodAudioDmaCacheEntry found{};
    uint32_t node = head;
    const uint32_t end = addr + size;
    const bool wrapped = end < addr;
    for (uint32_t guard = 0; node != 0 && guard < 64; guard++) {
        if (!lod_audio_addr_ok(node, 0x14)) {
            break;
        }

        LodAudioDmaCacheEntry entry{};
        entry.ok = true;
        entry.entry = node;
        entry.next = lod_audio_u32(rdram, node, 0x00);
        entry.prev = lod_audio_u32(rdram, node, 0x04);
        entry.base = lod_audio_u32(rdram, node, 0x08);
        entry.age = lod_audio_u32(rdram, node, 0x0C);
        entry.buffer = lod_audio_u32(rdram, node, 0x10);

        if (!wrapped && addr >= entry.base && end <= entry.base + 0xA00) {
            return entry;
        }
        node = entry.next;
    }
    return found;
}

struct LodAudioDmaMessage {
    bool ok = false;
    uint16_t type = 0;
    uint8_t subtype = 0;
    uint32_t ret_queue = 0;
    uint32_t dst = 0;
    uint32_t src = 0;
    uint32_t size = 0;
    uint32_t unk14 = 0;
};

static LodAudioDmaMessage lod_audio_dma_message(uint8_t* rdram, uint32_t msg) {
    LodAudioDmaMessage out{};
    if (!lod_audio_addr_ok(msg, 0x18)) {
        return out;
    }
    out.ok = true;
    out.type = lod_audio_u16(rdram, msg, 0x00);
    out.subtype = lod_audio_u8(rdram, msg, 0x02);
    out.ret_queue = lod_audio_u32(rdram, msg, 0x04);
    out.dst = lod_audio_u32(rdram, msg, 0x08);
    out.src = lod_audio_u32(rdram, msg, 0x0C);
    out.size = lod_audio_u32(rdram, msg, 0x10);
    out.unk14 = lod_audio_u32(rdram, msg, 0x14);
    return out;
}

struct LodAudioVoiceState {
    uint32_t pvoice = 0;
    uint32_t source = 0;
    uint32_t filter = 0;
    uint32_t handler = 0;
    uint32_t pos = 0;
    int16_t state = 0;
    int16_t priority = 0;
    int16_t pan = 0;
    int16_t fxmix = 0;
};

static LodAudioVoiceState lod_audio_voice_state(uint8_t* rdram, uint32_t voice) {
    LodAudioVoiceState state{};
    if (!lod_audio_addr_ok(voice, 0x1C)) {
        return state;
    }

    state.pvoice = lod_audio_u32(rdram, voice, 0x08);
    state.state = lod_audio_s16(rdram, voice, 0x14);
    state.priority = lod_audio_s16(rdram, voice, 0x16);
    state.pan = lod_audio_s16(rdram, voice, 0x18);
    state.fxmix = lod_audio_s16(rdram, voice, 0x1A);

    if (lod_audio_addr_ok(state.pvoice, 0xDC)) {
        state.source = lod_audio_u32(rdram, state.pvoice, 0x08);
        state.filter = lod_audio_u32(rdram, state.pvoice, 0x0C);
        state.pos = lod_audio_u32(rdram, state.pvoice, 0xD8);
        if (lod_audio_addr_ok(state.filter, 0x0C)) {
            state.handler = lod_audio_u32(rdram, state.filter, 0x08);
        }
    }

    return state;
}

struct LodAudioEventState {
    bool ok = false;
    uint32_t next = 0;
    uint32_t time = 0;
    uint16_t type = 0;
    uint32_t d0c = 0;
    uint32_t d10 = 0;
    uint32_t d14 = 0;
    uint32_t d18 = 0;
};

static LodAudioEventState lod_audio_event_state(uint8_t* rdram, uint32_t event) {
    LodAudioEventState state{};
    if (!lod_audio_addr_ok(event, 0x1C)) {
        return state;
    }
    state.ok = true;
    state.next = lod_audio_u32(rdram, event, 0x00);
    state.time = lod_audio_u32(rdram, event, 0x04);
    state.type = (uint16_t)lod_audio_s16(rdram, event, 0x08);
    state.d0c = lod_audio_u32(rdram, event, 0x0C);
    state.d10 = lod_audio_u32(rdram, event, 0x10);
    state.d14 = lod_audio_u32(rdram, event, 0x14);
    state.d18 = lod_audio_u32(rdram, event, 0x18);
    return state;
}

struct LodAudioEnvmixerState {
    uint32_t source = 0;
    uint32_t handler = 0;
    uint32_t state_ptr = 0;
    uint32_t state_phys = 0;
    uint32_t event = 0;
    uint32_t event_tail = 0;
    uint32_t dirty = 0;
    uint32_t aux_active = 0;
    uint32_t pos = 0;
    uint32_t target = 0;
    int16_t vol_idx = 0;
    int16_t env_gain = 0;
    int16_t left = 0;
    int16_t right = 0;
    int16_t left_next = 0;
    int16_t right_next = 0;
    uint16_t left_rate = 0;
    uint16_t right_rate = 0;
    int16_t left_step = 0;
    int16_t right_step = 0;
    uint16_t state20[8] = {};
    uint16_t state40[8] = {};
    LodAudioEventState event_state{};
};

static LodAudioEnvmixerState lod_audio_envmixer_state(uint8_t* rdram, uint32_t node) {
    LodAudioEnvmixerState state{};
    if (!lod_audio_addr_ok(node, 0x4C)) {
        return state;
    }

    state.source = lod_audio_u32(rdram, node, 0x00);
    state.handler = lod_audio_u32(rdram, node, 0x08);
    state.state_ptr = lod_audio_u32(rdram, node, 0x14);
    state.state_phys = lod_audio_phys(state.state_ptr);
    state.vol_idx = lod_audio_s16(rdram, node, 0x18);
    state.env_gain = lod_audio_s16(rdram, node, 0x1A);
    state.left = lod_audio_s16(rdram, node, 0x1C);
    state.right = lod_audio_s16(rdram, node, 0x1E);
    state.left_next = lod_audio_s16(rdram, node, 0x28);
    state.right_next = lod_audio_s16(rdram, node, 0x2E);
    state.left_rate = lod_audio_u16(rdram, node, 0x24);
    state.left_step = lod_audio_s16(rdram, node, 0x26);
    state.right_rate = lod_audio_u16(rdram, node, 0x2A);
    state.right_step = lod_audio_s16(rdram, node, 0x2C);
    state.pos = lod_audio_u32(rdram, node, 0x30);
    state.target = lod_audio_u32(rdram, node, 0x34);
    state.dirty = lod_audio_u32(rdram, node, 0x38);
    state.event = lod_audio_u32(rdram, node, 0x3C);
    state.event_tail = lod_audio_u32(rdram, node, 0x40);
    state.aux_active = lod_audio_u32(rdram, node, 0x48);
    state.event_state = lod_audio_event_state(rdram, state.event);

    if (lod_audio_addr_ok(state.state_ptr, 0x50)) {
        for (uint32_t i = 0; i < 8; i++) {
            state.state20[i] = lod_audio_u16(rdram, state.state_ptr, 0x20 + i * 2);
            state.state40[i] = lod_audio_u16(rdram, state.state_ptr, 0x40 + i * 2);
        }
    }

    return state;
}

static void lod_audio_log_voice(const char* tag, uint32_t count,
                                uint32_t synth, uint32_t voice,
                                const LodAudioVoiceState& before,
                                const LodAudioVoiceState& after,
                                const char* extra) {
    fprintf(stderr,
            "[AUDIO_VOICE] %s#%u synth=0x%08X voice=0x%08X"
            " pvoice 0x%08X->0x%08X state %d->%d pri %d->%d"
            " pan %d->%d fx %d->%d source 0x%08X->0x%08X"
            " filter 0x%08X->0x%08X handler 0x%08X->0x%08X"
            " pos 0x%08X->0x%08X%s%s\n",
            tag, count, synth, voice,
            before.pvoice, after.pvoice,
            before.state, after.state,
            before.priority, after.priority,
            before.pan, after.pan,
            before.fxmix, after.fxmix,
            before.source, after.source,
            before.filter, after.filter,
            before.handler, after.handler,
            before.pos, after.pos,
            (extra != nullptr && extra[0] != '\0') ? " " : "",
            extra != nullptr ? extra : "");
}

static void lod_audio_log_event(const char* tag, uint32_t count,
                                const LodAudioEventState& event) {
    if (!event.ok) {
        return;
    }
    fprintf(stderr,
            "[AUDIO_EVENT] %s#%u event{next=0x%08X time=%u type=0x%X"
            " d0c=0x%08X d10=0x%08X d14=0x%08X d18=0x%08X}\n",
            tag, count, event.next, event.time, event.type,
            event.d0c, event.d10, event.d14, event.d18);
}

static void lod_audio_log_envmixer_state_piece(const char* label,
                                               const LodAudioEnvmixerState& state) {
    fprintf(stderr,
            "%s{src=0x%08X handler=0x%08X state=0x%08X phys=0x%08X"
            " ev=0x%08X tail=0x%08X evtype=0x%X evtime=%u"
            " dirty=%u aux=%u pos=%u target=%u"
            " volidx=%d gain=%d lr=%d/%d next=%d/%d"
            " rate=%u/%u step=%d/%d st20:",
            label, state.source, state.handler, state.state_ptr, state.state_phys,
            state.event, state.event_tail, state.event_state.type,
            state.event_state.time, state.dirty, state.aux_active,
            state.pos, state.target, state.vol_idx, state.env_gain,
            state.left, state.right, state.left_next, state.right_next,
            state.left_rate, state.right_rate, state.left_step, state.right_step);
    for (uint16_t value : state.state20) {
        fprintf(stderr, " %04X", value);
    }
    fprintf(stderr, " st40:");
    for (uint16_t value : state.state40) {
        fprintf(stderr, " %04X", value);
    }
    fprintf(stderr, "}");
}

static bool lod_audio_envmixer_state_tiny(const LodAudioEnvmixerState& state) {
    return state.state20[0] <= 0x0140 || state.state20[1] <= 0x0140 ||
           state.left == 0 || state.right == 0 || state.left_next == 0 ||
           state.right_next == 0;
}

static bool lod_audio_envmixer_state_changed(const LodAudioEnvmixerState& before,
                                             const LodAudioEnvmixerState& after) {
    return before.event != after.event || before.event_tail != after.event_tail ||
           before.dirty != after.dirty || before.aux_active != after.aux_active ||
           before.pos != after.pos || before.target != after.target ||
           before.vol_idx != after.vol_idx || before.env_gain != after.env_gain ||
           before.left != after.left || before.right != after.right ||
           before.left_next != after.left_next || before.right_next != after.right_next ||
           before.left_step != after.left_step || before.right_step != after.right_step;
}

struct LodAudioEnvmixerTraceCounter {
    uint32_t state_phys = 0;
    uint32_t count = 0;
};

static uint32_t lod_audio_envmixer_state_pull_count(uint32_t state_phys) {
    if (state_phys == 0) {
        return 0;
    }

    static LodAudioEnvmixerTraceCounter counters[64];
    for (LodAudioEnvmixerTraceCounter& counter : counters) {
        if (counter.state_phys == state_phys) {
            return ++counter.count;
        }
    }
    for (LodAudioEnvmixerTraceCounter& counter : counters) {
        if (counter.state_phys == 0) {
            counter.state_phys = state_phys;
            counter.count = 1;
            return counter.count;
        }
    }

    return 0;
}

static bool lod_audio_envmixer_hot_state(uint32_t state_phys) {
    switch (state_phys) {
        // States observed near sparse-output/collapse traces. Keep this as
        // telemetry-only; it only changes trace density under AUDIO_PULL_TRACE.
        case 0x00106180:
        case 0x001062E0:
        case 0x00106390:
        case 0x00106700:
            return true;
        default:
            return false;
    }
}

static void lod_trace_audio_al_syn_set_vol(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t synth = (uint32_t)ctx->r4;
    const uint32_t voice = (uint32_t)ctx->r5;
    const int16_t vol = (int16_t)(uint16_t)ctx->r6;
    const uint32_t delta = (uint32_t)ctx->r7;
    const LodAudioVoiceState before = lod_audio_voice_state(rdram, voice);

    if (lod_orig_audio_al_syn_set_vol != nullptr) {
        lod_orig_audio_al_syn_set_vol(rdram, ctx);
    }

    const LodAudioVoiceState after = lod_audio_voice_state(rdram, voice);
    const bool changed = before.pvoice != after.pvoice || before.state != after.state ||
                         before.pos != after.pos || before.source != after.source;
    if (count <= 120 || (vol != 0 && count <= 2500) || changed ||
        (count % 1000) == 0) {
        char extra[96];
        snprintf(extra, sizeof(extra), "vol=%d delta=%u v0=0x%08X",
                 vol, delta, (uint32_t)ctx->r2);
        lod_audio_log_voice("alSynSetVol", count, synth, voice, before, after, extra);
    }
}

static void lod_trace_audio_al_syn_alloc_voice(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t synth = (uint32_t)ctx->r4;
    const uint32_t voice = (uint32_t)ctx->r5;
    const uint32_t config = (uint32_t)ctx->r6;
    const int16_t cfg_priority = lod_audio_s16(rdram, config, 0x00);
    const int16_t cfg_pan = lod_audio_s16(rdram, config, 0x02);
    const uint8_t cfg_fx = lod_audio_u8(rdram, config, 0x04);
    const LodAudioVoiceState before = lod_audio_voice_state(rdram, voice);

    if (lod_orig_audio_al_syn_alloc_voice != nullptr) {
        lod_orig_audio_al_syn_alloc_voice(rdram, ctx);
    }

    const LodAudioVoiceState after = lod_audio_voice_state(rdram, voice);
    if (count <= 120 || (uint32_t)ctx->r2 == 0 || (count % 500) == 0) {
        char extra[128];
        snprintf(extra, sizeof(extra),
                 "config=0x%08X cfg{pri=%d pan=%d fx=%u} result=%u",
                 config, cfg_priority, cfg_pan, cfg_fx, (uint32_t)ctx->r2);
        lod_audio_log_voice("alSynAllocVoice", count, synth, voice, before, after, extra);
    }
}

static void lod_trace_audio_al_syn_start_voice(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t synth = (uint32_t)ctx->r4;
    const uint32_t voice = (uint32_t)ctx->r5;
    const uint32_t wave = (uint32_t)ctx->r6;
    const LodAudioVoiceState before = lod_audio_voice_state(rdram, voice);

    if (lod_orig_audio_al_syn_start_voice != nullptr) {
        lod_orig_audio_al_syn_start_voice(rdram, ctx);
    }

    const LodAudioVoiceState after = lod_audio_voice_state(rdram, voice);
    if (count <= 120 || before.pvoice != after.pvoice || before.pos != after.pos ||
        (count % 500) == 0) {
        char extra[96];
        snprintf(extra, sizeof(extra), "wave=0x%08X v0=0x%08X", wave, (uint32_t)ctx->r2);
        lod_audio_log_voice("alSynStartVoice", count, synth, voice, before, after, extra);
    }
}

static void lod_trace_audio_al_syn_start_voice_params(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t synth = (uint32_t)ctx->r4;
    const uint32_t voice = (uint32_t)ctx->r5;
    const uint32_t wave = (uint32_t)ctx->r6;
    const uint32_t pitch_bits = (uint32_t)ctx->r7;
    const uint32_t sp = (uint32_t)ctx->r29;
    const int16_t vol = (int16_t)MEM_H(0x12, (gpr)(int32_t)sp);
    const uint8_t pan = (uint8_t)MEM_BU(0x17, (gpr)(int32_t)sp);
    const uint8_t fxmix = (uint8_t)MEM_BU(0x1B, (gpr)(int32_t)sp);
    const uint32_t delta = (uint32_t)MEM_W(0x1C, (gpr)(int32_t)sp);
    const LodAudioVoiceState before = lod_audio_voice_state(rdram, voice);

    if (lod_orig_audio_al_syn_start_voice_params != nullptr) {
        lod_orig_audio_al_syn_start_voice_params(rdram, ctx);
    }

    const LodAudioVoiceState after = lod_audio_voice_state(rdram, voice);
    if (count <= 120 || vol != 0 || before.pvoice != after.pvoice ||
        before.pos != after.pos || (count % 500) == 0) {
        char extra[160];
        snprintf(extra, sizeof(extra),
                 "wave=0x%08X pitch=0x%08X vol=%d pan=%u fx=%u delta=%u v0=0x%08X",
                 wave, pitch_bits, vol, pan, fxmix, delta, (uint32_t)ctx->r2);
        lod_audio_log_voice("alSynStartVoiceParams", count, synth, voice,
                            before, after, extra);
    }
}

static void lod_trace_audio_al_syn_stop_voice(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t synth = (uint32_t)ctx->r4;
    const uint32_t voice = (uint32_t)ctx->r5;
    const LodAudioVoiceState before = lod_audio_voice_state(rdram, voice);

    if (lod_orig_audio_al_syn_stop_voice != nullptr) {
        lod_orig_audio_al_syn_stop_voice(rdram, ctx);
    }

    const LodAudioVoiceState after = lod_audio_voice_state(rdram, voice);
    if (count <= 120 || before.pvoice != after.pvoice || before.state != after.state ||
        (count % 500) == 0) {
        char extra[64];
        snprintf(extra, sizeof(extra), "v0=0x%08X", (uint32_t)ctx->r2);
        lod_audio_log_voice("alSynStopVoice", count, synth, voice, before, after, extra);
    }
}

static void lod_trace_audio_al_syn_simple_param(uint8_t* rdram, recomp_context* ctx,
                                                const char* tag, recomp_func_t* original,
                                                uint32_t* counter) {
    (*counter)++;
    const uint32_t count = *counter;
    const uint32_t synth = (uint32_t)ctx->r4;
    const uint32_t voice = (uint32_t)ctx->r5;
    const uint32_t value = (uint32_t)ctx->r6;
    const LodAudioVoiceState before = lod_audio_voice_state(rdram, voice);

    if (original != nullptr) {
        original(rdram, ctx);
    }

    const LodAudioVoiceState after = lod_audio_voice_state(rdram, voice);
    if (count <= 80 || value != 0 || before.pan != after.pan ||
        before.fxmix != after.fxmix || before.pos != after.pos ||
        (count % 1000) == 0) {
        char extra[96];
        snprintf(extra, sizeof(extra), "value=0x%08X v0=0x%08X", value, (uint32_t)ctx->r2);
        lod_audio_log_voice(tag, count, synth, voice, before, after, extra);
    }
}

static void lod_trace_audio_al_syn_set_pitch(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    lod_trace_audio_al_syn_simple_param(rdram, ctx, "alSynSetPitch",
                                        lod_orig_audio_al_syn_set_pitch, &count);
}

static void lod_trace_audio_al_syn_set_pan(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    lod_trace_audio_al_syn_simple_param(rdram, ctx, "alSynSetPan",
                                        lod_orig_audio_al_syn_set_pan, &count);
}

static void lod_trace_audio_al_syn_set_fx_mix(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    lod_trace_audio_al_syn_simple_param(rdram, ctx, "alSynSetFXMix",
                                        lod_orig_audio_al_syn_set_fx_mix, &count);
}

static void lod_trace_audio_dma_callback(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;

    constexpr uint32_t DMA_STATE = 0x800FDDE0;
    constexpr uint32_t DMA_MSG_BASE = 0x800FE1E8;
    constexpr uint32_t DMA_MSG_INDEX = 0x800B86E4;
    constexpr uint32_t DMA_AGE_COUNTER = 0x800B86E0;
    constexpr uint32_t DMA_ASYNC_ENABLED = 0x800BB500;
    constexpr uint32_t DMA_ASYNC_QUEUE = 0x800BB508;

    const uint32_t req_addr = (uint32_t)ctx->r4;
    const uint32_t req_len = (uint32_t)ctx->r5;
    const uint32_t callback_state = (uint32_t)ctx->r6;
    const uint32_t active_before = lod_audio_u32(rdram, DMA_STATE, 0x04);
    const uint32_t free_before = lod_audio_u32(rdram, DMA_STATE, 0x08);
    const uint32_t msg_index_before = lod_audio_u32(rdram, DMA_MSG_INDEX, 0x00);
    const uint32_t age_before = lod_audio_u32(rdram, DMA_AGE_COUNTER, 0x00);
    const uint32_t async_enabled_before = lod_audio_u32(rdram, DMA_ASYNC_ENABLED, 0x00);
    const uint32_t async_queue_before = lod_audio_u32(rdram, DMA_ASYNC_QUEUE, 0x00);
    const LodAudioDmaCacheEntry hit_before =
        lod_audio_dma_find_cache_entry(rdram, active_before, req_addr, req_len);

    if (lod_orig_audio_dma_callback != nullptr) {
        lod_orig_audio_dma_callback(rdram, ctx);
    }

    const uint32_t ret = (uint32_t)ctx->r2;
    const uint32_t ret_phys = lod_audio_phys(ret);
    const uint32_t rsp_load_phys = ret_phys & ~UINT32_C(7);
    const uint32_t active_after = lod_audio_u32(rdram, DMA_STATE, 0x04);
    const uint32_t free_after = lod_audio_u32(rdram, DMA_STATE, 0x08);
    const uint32_t msg_index_after = lod_audio_u32(rdram, DMA_MSG_INDEX, 0x00);
    const uint32_t age_after = lod_audio_u32(rdram, DMA_AGE_COUNTER, 0x00);
    const uint32_t async_enabled_after = lod_audio_u32(rdram, DMA_ASYNC_ENABLED, 0x00);
    const uint32_t async_queue_after = lod_audio_u32(rdram, DMA_ASYNC_QUEUE, 0x00);
    const LodAudioDmaCacheEntry hit_after =
        lod_audio_dma_find_cache_entry(rdram, active_after, req_addr, req_len);

    uint32_t msg_addr = 0;
    LodAudioDmaMessage msg{};
    if (!hit_before.ok && msg_index_after != msg_index_before && msg_index_before < 128) {
        msg_addr = DMA_MSG_BASE + msg_index_before * 0x18;
        msg = lod_audio_dma_message(rdram, msg_addr);
    }

    const uint32_t ret_probe_len = lod_audio_min_u32(req_len + 8, 0x100);
    const uint32_t rsp_probe_len = lod_audio_min_u32(req_len + 16, 0x140);
    const uint32_t ret_nonzero =
        lod_audio_count_nonzero_phys(rdram, ret_phys, ret_probe_len);
    const uint32_t rsp_nonzero =
        lod_audio_count_nonzero_phys(rdram, rsp_load_phys, rsp_probe_len);
    const uint32_t msg_dst_phys = lod_audio_phys(msg.dst);
    const uint32_t msg_probe_len = lod_audio_min_u32(msg.size, 0x100);
    const uint32_t msg_dst_nonzero =
        msg.ok ? lod_audio_count_nonzero_phys(rdram, msg_dst_phys, msg_probe_len) : 0;

    const bool miss = !hit_before.ok;
    const bool suspicious =
        ret == 0 || (miss && !msg.ok) ||
        (msg.ok && (msg.src != (req_addr & ~UINT32_C(1)))) ||
        (msg.ok && (lod_audio_phys(msg.dst) != lod_audio_phys(hit_after.buffer)));
    const bool empty = ret_nonzero == 0 && rsp_nonzero == 0 && msg_dst_nonzero == 0;
    const bool miss_sample =
        miss && (count <= 500 || ret_nonzero != 0 || rsp_nonzero != 0 ||
                 msg_dst_nonzero != 0 || (count % 100) == 0);
    const bool should_log =
        count <= 240 || miss_sample || ret_nonzero != 0 || rsp_nonzero != 0 ||
        msg_dst_nonzero != 0 || suspicious ||
        (empty && count <= 1000 && (count % 100) == 0) || (count % 1000) == 0;

    if (should_log) {
        fprintf(stderr,
                "[AUDIO_DMA_CB] #%u req=0x%08X len=0x%X cb_state=0x%08X"
                " hit_before=%u hit_after=%u ret=0x%08X ret_phys=0x%08X"
                " rsp_phys=0x%08X nonzero{ret=%u/%u rsp=%u/%u msgdst=%u/%u}"
                " lists{active=0x%08X->0x%08X free=0x%08X->0x%08X}"
                " counters{msg=%u->%u age=%u->%u}"
                " async{enabled=%u->%u queue=0x%08X->0x%08X}",
                count, req_addr, req_len, callback_state,
                hit_before.ok ? 1u : 0u, hit_after.ok ? 1u : 0u,
                ret, ret_phys, rsp_load_phys,
                ret_nonzero, ret_probe_len, rsp_nonzero, rsp_probe_len,
                msg_dst_nonzero, msg_probe_len,
                active_before, active_after, free_before, free_after,
                msg_index_before, msg_index_after, age_before, age_after,
                async_enabled_before, async_enabled_after,
                async_queue_before, async_queue_after);

        if (hit_before.ok) {
            fprintf(stderr,
                    " before{entry=0x%08X base=0x%08X age=%u buf=0x%08X}",
                    hit_before.entry, hit_before.base, hit_before.age,
                    hit_before.buffer);
        }
        if (hit_after.ok) {
            fprintf(stderr,
                    " after{entry=0x%08X base=0x%08X age=%u buf=0x%08X}",
                    hit_after.entry, hit_after.base, hit_after.age,
                    hit_after.buffer);
        }
        if (msg.ok) {
            fprintf(stderr,
                    " msg@0x%08X{type=0x%X sub=%u queue=0x%08X"
                    " dst=0x%08X src=0x%08X size=0x%X unk14=0x%08X}",
                    msg_addr, msg.type, msg.subtype, msg.ret_queue, msg.dst,
                    msg.src, msg.size, msg.unk14);
        }
        fprintf(stderr, " head:");
        lod_audio_dump_head_phys(rdram, rsp_load_phys, rsp_probe_len);
        fprintf(stderr, "\n");
    }
}

static void lod_audio_pull_trace_common(uint8_t* rdram, recomp_context* ctx,
                                        const char* name, uint32_t vram,
                                        recomp_func_t* original, uint32_t* counter) {
    (*counter)++;
    uint32_t count = *counter;
    uint32_t in_a0 = (uint32_t)ctx->r4;
    uint32_t in_a1 = (uint32_t)ctx->r5;
    uint32_t in_a2 = (uint32_t)ctx->r6;
    uint32_t in_a3 = (uint32_t)ctx->r7;
    uint32_t node_func = 0;
    uint32_t node_phys = in_a0 & 0x1FFFFFFF;
    if (in_a0 != 0 && lod_rdram_range_ok(node_phys, 0x10)) {
        node_func = lod_rdram_u32(rdram, node_phys + 0x08);
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }

    uint32_t out_v0 = (uint32_t)ctx->r2;
    int32_t cmd_delta = (int32_t)(out_v0 - in_a2);
    bool high_volume = std::strcmp(name, "envmixerPull") == 0;
    uint32_t sample_period = high_volume ? 5000 : 250;
    if (count <= 40 || (count % sample_period) == 0) {
        fprintf(stderr,
                "[AUDIO_PULL] %s#%u vram=0x%08X a0=0x%08X node_fn=0x%08X "
                "a1=0x%08X a2=0x%08X a3=0x%08X -> v0=0x%08X delta=%d\n",
                name, count, vram, in_a0, node_func, in_a1, in_a2, in_a3,
                out_v0, cmd_delta);
    }
}

#define LOD_AUDIO_PULL_WRAPPER(wrapper_name, label, vram_value, original_var) \
    static void wrapper_name(uint8_t* rdram, recomp_context* ctx) { \
        static uint32_t count = 0; \
        lod_audio_pull_trace_common(rdram, ctx, label, vram_value, original_var, &count); \
    }

LOD_AUDIO_PULL_WRAPPER(lod_trace_audio_main_bus_pull, "mainBusPull", 0x8009D5F0, lod_orig_audio_main_bus_pull)
LOD_AUDIO_PULL_WRAPPER(lod_trace_audio_fx_pull, "fxPull", 0x8009D710, lod_orig_audio_fx_pull)
LOD_AUDIO_PULL_WRAPPER(lod_trace_audio_aux_bus_pull, "auxBusPull", 0x8009E3A0, lod_orig_audio_aux_bus_pull)
LOD_AUDIO_PULL_WRAPPER(lod_trace_audio_resample_pull, "resamplePull", 0x8009F1EC, lod_orig_audio_resample_pull)
LOD_AUDIO_PULL_WRAPPER(lod_trace_audio_raw16_pull, "raw16Pull", 0x8009F7A4, lod_orig_audio_raw16_pull)
LOD_AUDIO_PULL_WRAPPER(lod_trace_audio_adpcm_pull, "adpcmPull", 0x8009FC7C, lod_orig_audio_adpcm_pull)

#undef LOD_AUDIO_PULL_WRAPPER

static void lod_trace_audio_envmixer_pull(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;

    const uint32_t in_a0 = (uint32_t)ctx->r4;
    const uint32_t in_a1 = (uint32_t)ctx->r5;
    const uint32_t in_a2 = (uint32_t)ctx->r6;
    const uint32_t in_a3 = (uint32_t)ctx->r7;
    uint32_t node_func = 0;
    const uint32_t node_phys = in_a0 & 0x1FFFFFFF;
    if (in_a0 != 0 && lod_rdram_range_ok(node_phys, 0x10)) {
        node_func = lod_rdram_u32(rdram, node_phys + 0x08);
    }
    const LodAudioEnvmixerState before = lod_audio_envmixer_state(rdram, in_a0);

    if (lod_orig_audio_envmixer_pull != nullptr) {
        lod_orig_audio_envmixer_pull(rdram, ctx);
    }

    const uint32_t out_v0 = (uint32_t)ctx->r2;
    const int32_t cmd_delta = (int32_t)(out_v0 - in_a2);
    const LodAudioEnvmixerState after = lod_audio_envmixer_state(rdram, in_a0);
    if (count <= 40 || (count % 5000) == 0) {
        fprintf(stderr,
                "[AUDIO_PULL] envmixerPull#%u vram=0x8009E480 a0=0x%08X"
                " node_fn=0x%08X a1=0x%08X a2=0x%08X a3=0x%08X"
                " -> v0=0x%08X delta=%d\n",
                count, in_a0, node_func, in_a1, in_a2, in_a3, out_v0, cmd_delta);
    }

    const bool tiny = lod_audio_envmixer_state_tiny(before) ||
                      lod_audio_envmixer_state_tiny(after);
    const bool changed = lod_audio_envmixer_state_changed(before, after);
    const uint32_t state_phys = before.state_phys != 0 ? before.state_phys : after.state_phys;
    const uint32_t state_count = lod_audio_envmixer_state_pull_count(state_phys);
    const bool hot = lod_audio_envmixer_hot_state(before.state_phys) ||
                     lod_audio_envmixer_hot_state(after.state_phys);
    const bool state_start = state_count != 0 && state_count <= 8;
    const bool hot_sample = hot && state_count != 0 && ((state_count % 16) == 0);
    const bool tiny_sample = tiny && state_count != 0 && ((state_count % 250) == 0);
    if (count <= 80 || state_start || (changed && count <= 2000) ||
        hot_sample || tiny_sample || (count % 5000) == 0) {
        fprintf(stderr, "[AUDIO_ENVMIXER_CPU] envmixerPull#%u statePull#%u"
                        " hot=%u tiny=%u changed=%u node=0x%08X "
                        "a1=0x%08X a2=0x%08X a3=0x%08X -> v0=0x%08X ",
                count, state_count, hot ? 1u : 0u, tiny ? 1u : 0u,
                changed ? 1u : 0u, in_a0, in_a1, in_a2, in_a3, out_v0);
        lod_audio_log_envmixer_state_piece("before", before);
        fprintf(stderr, " ");
        lod_audio_log_envmixer_state_piece("after", after);
        fprintf(stderr, "\n");
    }
}

static void lod_trace_audio_envmixer_param(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;

    uint32_t node = (uint32_t)ctx->r4;
    uint32_t param_id = (uint32_t)ctx->r5;
    uint32_t param_value = (uint32_t)ctx->r6;
    uint32_t node_phys = node & 0x1FFFFFFF;
    bool node_ok = node != 0 && lod_rdram_range_ok(node_phys, 0x4C);
    uint32_t source_before = node_ok ? lod_rdram_u32(rdram, node_phys + 0x00) : 0;
    uint32_t state38_before = node_ok ? lod_rdram_u32(rdram, node_phys + 0x38) : 0;
    uint32_t state48_before = node_ok ? lod_rdram_u32(rdram, node_phys + 0x48) : 0;
    LodAudioEventState event_before = lod_audio_event_state(rdram, param_value);

    if (lod_orig_audio_envmixer_param != nullptr) {
        lod_orig_audio_envmixer_param(rdram, ctx);
    }

    uint32_t source_after = node_ok ? lod_rdram_u32(rdram, node_phys + 0x00) : 0;
    uint32_t state38_after = node_ok ? lod_rdram_u32(rdram, node_phys + 0x38) : 0;
    uint32_t state48_after = node_ok ? lod_rdram_u32(rdram, node_phys + 0x48) : 0;
    LodAudioEventState event_after = lod_audio_event_state(rdram, param_value);
    bool important_param = param_id == 1 || param_id == 3 || param_id == 4 ||
                           param_id == 5 || param_id == 9;
    bool changed = state38_before != state38_after ||
                   state48_before != state48_after ||
                   source_before != source_after;
    if (count <= 80 || important_param || changed || (count % 1000) == 0) {
        fprintf(stderr,
                "[AUDIO_PARAM] envmixerParam#%u a0=0x%08X id=%u value=0x%08X "
                "source 0x%08X->0x%08X state38 0x%08X->0x%08X "
                "state48 0x%08X->0x%08X"
                " event_before{ok=%u next=0x%08X time=%u type=0x%X"
                " d0c=0x%08X d10=0x%08X d14=0x%08X d18=0x%08X}"
                " event_after{ok=%u next=0x%08X time=%u type=0x%X"
                " d0c=0x%08X d10=0x%08X d14=0x%08X d18=0x%08X}"
                " v0=0x%08X\n",
                count, node, param_id, param_value,
                source_before, source_after, state38_before, state38_after,
                state48_before, state48_after,
                event_before.ok ? 1U : 0U, event_before.next, event_before.time,
                event_before.type, event_before.d0c, event_before.d10,
                event_before.d14, event_before.d18,
                event_after.ok ? 1U : 0U, event_after.next, event_after.time,
                event_after.type, event_after.d0c, event_after.d10,
                event_after.d14, event_after.d18, (uint32_t)ctx->r2);
        lod_audio_log_event("envmixerParam.before", count, event_before);
        lod_audio_log_event("envmixerParam.after", count, event_after);
    }
}

static void lod_install_audio_pull_trace_wrapper(uint32_t vram, const char* name,
                                                 recomp_func_t* wrapper,
                                                 recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current == wrapper) {
        return;
    }
    *original_out = current;
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
    fprintf(stderr, "[AUDIO_PULL] installed %s wrapper vram=0x%08X original=%p wrapper=%p\n",
            name, vram, (void*)current, (void*)wrapper);
}

extern "C" void lod_install_audio_pull_trace_wrappers_early() {
    lod_install_audio_pull_trace_wrapper(0x800973A0, "alSynSetVol",
                                         lod_trace_audio_al_syn_set_vol,
                                         &lod_orig_audio_al_syn_set_vol);
    lod_install_audio_pull_trace_wrapper(0x80099278, "alSynAllocVoice",
                                         lod_trace_audio_al_syn_alloc_voice,
                                         &lod_orig_audio_al_syn_alloc_voice);
    lod_install_audio_pull_trace_wrapper(0x800999B0, "alSynStartVoice",
                                         lod_trace_audio_al_syn_start_voice,
                                         &lod_orig_audio_al_syn_start_voice);
    lod_install_audio_pull_trace_wrapper(0x8009B1A0, "alSynSetPitch",
                                         lod_trace_audio_al_syn_set_pitch,
                                         &lod_orig_audio_al_syn_set_pitch);
    lod_install_audio_pull_trace_wrapper(0x8009B940, "alSynSetPan",
                                         lod_trace_audio_al_syn_set_pan,
                                         &lod_orig_audio_al_syn_set_pan);
    lod_install_audio_pull_trace_wrapper(0x8009092C, "audioDmaCallback",
                                         lod_trace_audio_dma_callback,
                                         &lod_orig_audio_dma_callback);
    lod_install_audio_pull_trace_wrapper(0x8009D5F0, "mainBusPull",
                                         lod_trace_audio_main_bus_pull,
                                         &lod_orig_audio_main_bus_pull);
    lod_install_audio_pull_trace_wrapper(0x8009D710, "fxPull",
                                         lod_trace_audio_fx_pull,
                                         &lod_orig_audio_fx_pull);
    lod_install_audio_pull_trace_wrapper(0x8009E3A0, "auxBusPull",
                                         lod_trace_audio_aux_bus_pull,
                                         &lod_orig_audio_aux_bus_pull);
    lod_install_audio_pull_trace_wrapper(0x8009E480, "envmixerPull",
                                         lod_trace_audio_envmixer_pull,
                                         &lod_orig_audio_envmixer_pull);
    lod_install_audio_pull_trace_wrapper(0x8009E988, "envmixerParam",
                                         lod_trace_audio_envmixer_param,
                                         &lod_orig_audio_envmixer_param);
    lod_install_audio_pull_trace_wrapper(0x8009F1EC, "resamplePull",
                                         lod_trace_audio_resample_pull,
                                         &lod_orig_audio_resample_pull);
    lod_install_audio_pull_trace_wrapper(0x8009F7A4, "raw16Pull",
                                         lod_trace_audio_raw16_pull,
                                         &lod_orig_audio_raw16_pull);
    lod_install_audio_pull_trace_wrapper(0x8009FC7C, "adpcmPull",
                                         lod_trace_audio_adpcm_pull,
                                         &lod_orig_audio_adpcm_pull);
    lod_install_audio_pull_trace_wrapper(0x800A1540, "alSynSetFXMix",
                                         lod_trace_audio_al_syn_set_fx_mix,
                                         &lod_orig_audio_al_syn_set_fx_mix);
    lod_install_audio_pull_trace_wrapper(0x800A4DF0, "alSynStartVoiceParams",
                                         lod_trace_audio_al_syn_start_voice_params,
                                         &lod_orig_audio_al_syn_start_voice_params);
    lod_install_audio_pull_trace_wrapper(0x800A71B0, "alSynStopVoice",
                                         lod_trace_audio_al_syn_stop_voice,
                                         &lod_orig_audio_al_syn_stop_voice);
}
#endif // LOD_ENABLE_AUDIO_PULL_TRACE

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

static inline bool lod_ai_buffer_phys(uint32_t addr, uint32_t size, uint32_t* phys_out) {
    uint32_t phys = addr;
    const uint32_t region = addr & 0xE0000000u;
    if (region == 0x80000000u || region == 0xA0000000u) {
        phys = addr & 0x1FFFFFFFu;
    }

    if (!lod_rdram_range_ok(phys, size)) {
        return false;
    }

    *phys_out = phys;
    return true;
}

// LoD can pass a low/physical RDRAM address to osAiSetNextBuffer. The
// ultramodern runtime helper expects a KSEG0/KSEG1-style pointer and otherwise
// maps low 0x00xxxxxx addresses to rdram+0x80xxxxxx, which faults past RDRAM.
// Keep this local wrapper out of the N64ModernRuntime submodule and patch the
// generated audioTask_build call site to use it after regeneration.
void lod_osAiSetNextBuffer_recomp(uint8_t* rdram, recomp_context* ctx) {
    const uint32_t raw = (uint32_t)ctx->r4;
    const uint32_t bytes = (uint32_t)ctx->r5;
    uint32_t phys = 0;

    if (bytes == 0) {
        ctx->r2 = 0;
        return;
    }

    if (!lod_ai_buffer_phys(raw, bytes, &phys)) {
#if LOD_ENABLE_AUDIO_TRACE
        fprintf(stderr,
                "[AUDIO_AI] skip invalid osAiSetNextBuffer raw=0x%08X bytes=0x%X\n",
                raw, bytes);
#endif
        ctx->r2 = 0;
        return;
    }

    const uint32_t kseg0 = 0x80000000u | phys;
#if LOD_ENABLE_AUDIO_TRACE
    if (raw != kseg0) {
        static uint32_t normalize_count = 0;
        normalize_count++;
        if (normalize_count <= 20 || (normalize_count % 200) == 0) {
            fprintf(stderr,
                    "[AUDIO_AI] normalize osAiSetNextBuffer#%u raw=0x%08X"
                    " -> kseg0=0x%08X phys=0x%06X bytes=0x%X\n",
                    normalize_count, raw, kseg0, phys, bytes);
        }
    }
#endif
    ultramodern::queue_audio_buffer(rdram, (PTR(int16_t))kseg0, bytes);
    ctx->r2 = 0;
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

#if LOD_ENABLE_NI24_WRITER_TRACE || LOD_ENABLE_B2_ASSET_TRACE
extern "C" int lod_ni_overlay_loaded_0f_pair();
extern "C" int lod_ni_overlay_loaded_0e_pair();

static recomp_func_t* lod_orig_object_curlevel_goto_next_clear_timer = nullptr; // 0x80001CE8
static recomp_func_t* lod_orig_object_curlevel_goto_func = nullptr;             // 0x80001E30

static bool lod_writer_trace_is_map76_boss_obj_id(uint16_t obj_id) {
    switch (obj_id) {
        case 0x08B:
        case 0x0EF:
        case 0x0FE:
        case 0x0FF:
        case 0x1E1:
        case 0x1E2:
        case 0x1E3:
            return true;
        default:
            return false;
    }
}

static void lod_ni24_writer_host_symbol(const char** symbol_out, uintptr_t* offset_out) {
    const char* symbol = "(unknown)";
    uintptr_t offset = 0;
#if defined(__APPLE__) || defined(__linux__)
    Dl_info info = {};
    void* caller = __builtin_return_address(1);
    if (caller != nullptr && dladdr(caller, &info) != 0 && info.dli_sname != nullptr) {
        symbol = info.dli_sname;
        uintptr_t sym_base = (uintptr_t)info.dli_saddr;
        uintptr_t addr = (uintptr_t)caller;
        if (sym_base != 0 && addr >= sym_base) {
            offset = addr - sym_base;
        }
    }
#endif
    *symbol_out = symbol;
    *offset_out = offset;
}

static void lod_ni24_writer_snapshot(uint8_t* rdram, uint32_t base_phys,
                                     uint8_t out[8]) {
    for (uint32_t i = 0; i < 8; i++) {
        out[i] = lod_rdram_range_ok(base_phys + i, 1) ?
            lod_rdram_u8(rdram, base_phys + i) : 0xFF;
    }
}

static void lod_log_ni24_writer(uint8_t* rdram, recomp_context* ctx, const char* op,
                                uint32_t schedule_base, uint32_t level_ptr,
                                int32_t target_func, const uint8_t before_sched[8]) {
    const uint32_t base_phys = schedule_base & 0x1FFFFFFF;
    const uint32_t level_phys = level_ptr & 0x1FFFFFFF;
    const uint32_t obj = schedule_base - 0x08;
    const bool shape_ok = (level_ptr == schedule_base + 0x06);
    const bool base_ok = lod_rdram_range_ok(base_phys, 8);
    const bool level_ok = lod_rdram_range_ok(level_phys, 2);
    const int16_t level = level_ok ? lod_rdram_s16(rdram, level_phys) : -1;
    const uint32_t slot_phys = base_phys + (uint32_t)((int32_t)level * 2);
    const bool slot_ok = level >= 0 && level < 4 && lod_rdram_range_ok(slot_phys, 2);
    const uint8_t before_timer = slot_ok ? before_sched[(uint32_t)level * 2] : 0xFF;
    const uint8_t before_state = slot_ok ? before_sched[(uint32_t)level * 2 + 1] : 0xFF;
    const uint8_t after_timer = slot_ok ? lod_rdram_u8(rdram, slot_phys + 0) : 0xFF;
    const uint8_t after_state = slot_ok ? lod_rdram_u8(rdram, slot_phys + 1) : 0xFF;
    const int32_t gamestate = lod_current_gamestate(rdram);
    const int loaded_0f = lod_ni_overlay_loaded_0f_pair();
    const int loaded_0e = lod_ni_overlay_loaded_0e_pair();
    uint32_t obj_phys = 0;
    const bool obj_ok = lod_decode_rdram_phys_addr(obj, 0x74, &obj_phys);
    uint16_t obj_raw = 0;
    uint16_t obj_flags = 0;
    uint16_t obj_id = 0;
    if (obj_ok) {
        obj_raw = (uint16_t)lod_rdram_s16(rdram, obj_phys + 0x00);
        obj_flags = (uint16_t)lod_rdram_s16(rdram, obj_phys + 0x02);
        obj_id = obj_raw & 0x07FF;
    }

    const bool target_obj = obj == 0x80324A5C;
    const bool suspicious = before_state >= 4 || after_state >= 4 ||
                            (target_func >= 4 && target_func < 0x100);
    const bool pair24_gameplay = gamestate == 3 && loaded_0f == 24;
#if LOD_ENABLE_B2_ASSET_TRACE
    const bool map76_boss = gamestate == 3 &&
                            lod_current_map_ovl_rom == LOD_B2_TRACE_MAP_ROM &&
                            obj_ok &&
                            lod_writer_trace_is_map76_boss_obj_id(obj_id);
#else
    const bool map76_boss = false;
#endif
    if (!shape_ok || !base_ok || !level_ok ||
        ((!pair24_gameplay || (!target_obj && !suspicious)) && !map76_boss)) {
        return;
    }

    static uint32_t trace_count = 0;
    trace_count++;
    if (!map76_boss && !target_obj && trace_count > 120 && (trace_count % 500) != 0) {
        return;
    }

    uint8_t after_sched[8] = {};
    lod_ni24_writer_snapshot(rdram, base_phys, after_sched);

    const char* symbol = "(unknown)";
    uintptr_t offset = 0;
    lod_ni24_writer_host_symbol(&symbol, &offset);

    const char* tag = map76_boss ? "[MAP76_WRITE]" : "[NI24_WRITE]";

    fprintf(stderr,
        "%s #%u op=%s gs=%d map#%d map_rom=0x%08X loaded0f=%d loaded0e=%d "
        "obj=0x%08X obj_id=0x%03X raw=0x%04X flags=0x%04X level=%d target=%d "
        "slot=0x%08X state=%u->%u timer=%u->%u "
        "sched_before=%02X%02X,%02X%02X,%02X%02X,%02X%02X "
        "sched_after=%02X%02X,%02X%02X,%02X%02X,%02X%02X "
        "a0=0x%08X a1=0x%08X a2=0x%08X caller=%s+0x%zX\n",
        tag, trace_count, op, gamestate, lod_map_ovl_load_count, lod_current_map_ovl_rom,
        loaded_0f, loaded_0e, obj, obj_id, obj_raw, obj_flags, level, target_func,
        slot_ok ? (schedule_base + (uint32_t)((int32_t)level * 2)) : 0,
        before_state, after_state, before_timer, after_timer,
        before_sched[0], before_sched[1], before_sched[2], before_sched[3],
        before_sched[4], before_sched[5], before_sched[6], before_sched[7],
        after_sched[0], after_sched[1], after_sched[2], after_sched[3],
        after_sched[4], after_sched[5], after_sched[6], after_sched[7],
        schedule_base, level_ptr, (uint32_t)ctx->r6, symbol, (size_t)offset);
}

static void lod_trace_object_curlevel_goto_next_clear_timer(uint8_t* rdram,
                                                            recomp_context* ctx) {
    const uint32_t schedule_base = (uint32_t)ctx->r4;
    const uint32_t level_ptr = (uint32_t)ctx->r5;
    const uint32_t base_phys = schedule_base & 0x1FFFFFFF;
    uint8_t before_sched[8] = {};
    if (lod_rdram_range_ok(base_phys, sizeof(before_sched))) {
        lod_ni24_writer_snapshot(rdram, base_phys, before_sched);
    }

    if (lod_orig_object_curlevel_goto_next_clear_timer != nullptr) {
        lod_orig_object_curlevel_goto_next_clear_timer(rdram, ctx);
    }

    lod_log_ni24_writer(rdram, ctx, "next_clear_timer", schedule_base, level_ptr,
                        -1, before_sched);
}

static void lod_trace_object_curlevel_goto_func(uint8_t* rdram, recomp_context* ctx) {
    const uint32_t schedule_base = (uint32_t)ctx->r4;
    const uint32_t level_ptr = (uint32_t)ctx->r5;
    const int32_t target_func = (int32_t)ctx->r6;
    const uint32_t base_phys = schedule_base & 0x1FFFFFFF;
    uint8_t before_sched[8] = {};
    if (lod_rdram_range_ok(base_phys, sizeof(before_sched))) {
        lod_ni24_writer_snapshot(rdram, base_phys, before_sched);
    }

    if (lod_orig_object_curlevel_goto_func != nullptr) {
        lod_orig_object_curlevel_goto_func(rdram, ctx);
    }

    lod_log_ni24_writer(rdram, ctx, "goto_func", schedule_base, level_ptr,
                        target_func, before_sched);
}

static void lod_install_ni24_writer_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                                  recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

extern "C" void lod_install_ni24_writer_trace_wrappers_early() {
    lod_install_ni24_writer_trace_wrapper(0x80001CE8,
        lod_trace_object_curlevel_goto_next_clear_timer,
        &lod_orig_object_curlevel_goto_next_clear_timer);
    lod_install_ni24_writer_trace_wrapper(0x80001E30,
        lod_trace_object_curlevel_goto_func,
        &lod_orig_object_curlevel_goto_func);
    fprintf(stderr,
        "[NI24_WRITE] installed object schedule writer trace originals next=%p goto=%p\n",
        (void*)lod_orig_object_curlevel_goto_next_clear_timer,
        (void*)lod_orig_object_curlevel_goto_func);
}
#endif

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

#if LOD_ENABLE_KSEG0_FAULT_TRACE || LOD_ENABLE_B2_ASSET_TRACE
static recomp_func_t* lod_orig_kseg0_dmamgr_update = nullptr; // 0x80011E48
static recomp_func_t* lod_orig_kseg0_dmamgr_romcopy = nullptr; // 0x8001A42C

#if LOD_ENABLE_B2_ASSET_TRACE
static bool lod_b2_trace_matches_asset(uint32_t rom, uint32_t ram, uint32_t size, uint32_t file_id) {
    return rom == LOD_B2_TRACE_ROM ||
           ram == LOD_B2_TRACE_RAM ||
           size == LOD_B2_TRACE_SIZE ||
           file_id == LOD_B2_TRACE_FILE;
}

static uint32_t lod_b2_trace_hash_rdram(uint8_t* rdram, uint32_t phys, uint32_t size) {
    uint32_t n = size;
    if (n > 0x100) {
        n = 0x100;
    }
    if (n == 0 || !lod_rdram_range_ok(phys, n)) {
        return 0;
    }

    uint32_t h = 2166136261u;
    for (uint32_t i = 0; i < n; i++) {
        h ^= rdram[phys + i];
        h *= 16777619u;
    }
    return h;
}
#endif

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
#if LOD_ENABLE_KSEG0_FAULT_TRACE
    bool active_bad = s.pending != 0 && s.entry_ram_ok == 0;
    bool current_bad = s.cur_ram_ok == 0;
#endif
#if LOD_ENABLE_B2_ASSET_TRACE
    bool active_b2 = lod_b2_trace_matches_asset(s.entry_rom, s.entry_ram, s.entry_size, s.entry_file_id);
    bool current_b2 = lod_b2_trace_matches_asset(s.cur_rom, s.cur_ram, s.cur_size, s.cur_file_id);
    uint32_t b2_pre_count = s.count;
    uint32_t b2_pre_gs = s.gs;
    uint32_t b2_pre_map_count = s.map_load_count;
    uint32_t b2_pre_map_rom = s.map_rom;
    uint32_t b2_pre_chunk = s.chunk_addr;
    uint32_t b2_pre_slot = s.slot;
    uint32_t b2_pre_pending = s.pending;
    uint32_t b2_pre_entry_rom = s.entry_rom;
    uint32_t b2_pre_entry_ram = s.entry_ram;
    uint32_t b2_pre_entry_size = s.entry_size;
    uint32_t b2_pre_entry_file = s.entry_file_id;
    uint32_t b2_pre_entry_user = s.entry_user_ptr;
    uint32_t b2_pre_cur_ram = s.cur_ram;
    uint32_t b2_pre_cur_size = s.cur_size;
    bool b2_pre_interesting = active_b2 || current_b2;
    uint32_t b2_pre_target_phys = 0;
    bool b2_pre_target_ok = (b2_pre_entry_ram == LOD_B2_TRACE_RAM) &&
        lod_decode_rdram_phys_addr(b2_pre_entry_ram, 0x100, &b2_pre_target_phys);
    uint32_t b2_pre_hash = b2_pre_target_ok
        ? lod_b2_trace_hash_rdram(rdram, b2_pre_target_phys, 0x100)
        : 0;
    if (active_b2 || current_b2) {
        static int b2_update_count = 0;
        static uint32_t last_map_rom = 0;
        static uint32_t last_chunk = 0;
        static uint32_t last_slot = 0xFFFFFFFF;
        static uint32_t last_pending = 0xFFFFFFFF;
        static uint32_t last_entry_rom = 0;
        static uint32_t last_entry_ram = 0;
        static uint32_t last_entry_size = 0;
        static uint32_t last_entry_file = 0;
        static uint32_t last_cur_rom = 0;
        static uint32_t last_cur_ram = 0;
        static uint32_t last_cur_size = 0;
        static uint32_t last_cur_file = 0;

        b2_update_count++;
        bool changed =
            s.map_rom != last_map_rom ||
            s.chunk_addr != last_chunk ||
            s.slot != last_slot ||
            s.pending != last_pending ||
            s.entry_rom != last_entry_rom ||
            s.entry_ram != last_entry_ram ||
            s.entry_size != last_entry_size ||
            s.entry_file_id != last_entry_file ||
            s.cur_rom != last_cur_rom ||
            s.cur_ram != last_cur_ram ||
            s.cur_size != last_cur_size ||
            s.cur_file_id != last_cur_file;

        if (changed || b2_update_count <= 24 || (b2_update_count % 120) == 0) {
            fprintf(stderr,
                    "[B2_ASSET] update#%d call#%u gs=%u map#%u map_rom=0x%08X "
                    "chunk=0x%08X slot=%u pending=%u active_match=%d current_match=%d "
                    "active={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X user=0x%08X ram_ok=%u} "
                    "cur={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X ram_ok=%u} "
                    "ra=0x%08X sp=0x%08X a={0x%08X,0x%08X,0x%08X,0x%08X}\n",
                    b2_update_count, s.count, s.gs, s.map_load_count, s.map_rom,
                    s.chunk_addr, s.slot, s.pending, active_b2 ? 1 : 0, current_b2 ? 1 : 0,
                    s.entry_rom, s.entry_ram, s.entry_size, s.entry_file_id, s.entry_user_ptr, s.entry_ram_ok,
                    s.cur_rom, s.cur_ram, s.cur_size, s.cur_file_id, s.cur_ram_ok,
                    s.ra, s.sp, s.a0, s.a1, s.a2, s.a3);
        }

        last_map_rom = s.map_rom;
        last_chunk = s.chunk_addr;
        last_slot = s.slot;
        last_pending = s.pending;
        last_entry_rom = s.entry_rom;
        last_entry_ram = s.entry_ram;
        last_entry_size = s.entry_size;
        last_entry_file = s.entry_file_id;
        last_cur_rom = s.cur_rom;
        last_cur_ram = s.cur_ram;
        last_cur_size = s.cur_size;
        last_cur_file = s.cur_file_id;
    }
#endif
#if LOD_ENABLE_KSEG0_FAULT_TRACE
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
#endif
    if (lod_orig_kseg0_dmamgr_update != nullptr) {
        lod_orig_kseg0_dmamgr_update(rdram, ctx);
    }
#if LOD_ENABLE_B2_ASSET_TRACE
    if (b2_pre_interesting && b2_pre_target_ok) {
        lod_kseg0_record_snapshot(rdram, ctx, 0x80011E48);
        const LodKseg0FaultTraceSnapshot& post = lod_kseg0_fault_trace_snapshot;
        uint32_t decomp_size_guess = 0;
        if (post.cur_ram > b2_pre_entry_ram && post.cur_ram < 0x80800000) {
            decomp_size_guess = post.cur_ram - b2_pre_entry_ram;
        }
        uint32_t hash_size = decomp_size_guess != 0 ? decomp_size_guess : 0x100;
        uint32_t b2_post_hash = lod_b2_trace_hash_rdram(rdram, b2_pre_target_phys, hash_size);
        uint32_t b2_w0 = lod_rdram_range_ok(b2_pre_target_phys, 4) ? lod_rdram_u32(rdram, b2_pre_target_phys + 0x00) : 0;
        uint32_t b2_w4 = lod_rdram_range_ok(b2_pre_target_phys + 0x04, 4) ? lod_rdram_u32(rdram, b2_pre_target_phys + 0x04) : 0;
        uint32_t b2_w18 = lod_rdram_range_ok(b2_pre_target_phys + 0x18, 4) ? lod_rdram_u32(rdram, b2_pre_target_phys + 0x18) : 0;
        static int b2_after_count = 0;
        b2_after_count++;
        bool changed = b2_pre_hash != b2_post_hash || decomp_size_guess == 0x3E720;
        if (changed || b2_after_count <= 24 || (b2_after_count % 120) == 0) {
            fprintf(stderr,
                    "[B2_ASSET] after_update#%d call#%u gs=%u map#%u map_rom=0x%08X "
                    "chunk=0x%08X slot=%u pending=%u "
                    "entry={rom=0x%08X ram=0x%08X size=0x%08X file=0x%08X user=0x%08X} "
                    "pre_cur={ram=0x%08X size=0x%08X} post_cur={ram=0x%08X size=0x%08X file=0x%08X} "
                    "decomp_guess=0x%X target_phys=0x%06X hash=0x%08X->0x%08X "
                    "w={0x%08X,0x%08X,0x%08X}\n",
                    b2_after_count, b2_pre_count, b2_pre_gs, b2_pre_map_count, b2_pre_map_rom,
                    b2_pre_chunk, b2_pre_slot, b2_pre_pending,
                    b2_pre_entry_rom, b2_pre_entry_ram, b2_pre_entry_size, b2_pre_entry_file,
                    b2_pre_entry_user, b2_pre_cur_ram, b2_pre_cur_size,
                    post.cur_ram, post.cur_size, post.cur_file_id,
                    decomp_size_guess, b2_pre_target_phys, b2_pre_hash, b2_post_hash,
                    b2_w0, b2_w4, b2_w18);
        }
    }
#endif
}

static void lod_kseg0_trace_dmamgr_romcopy(uint8_t* rdram, recomp_context* ctx) {
    lod_kseg0_record_dmamgr_romcopy(rdram, ctx);
    const LodKseg0FaultTraceSnapshot& s = lod_kseg0_fault_trace_snapshot;
#if LOD_ENABLE_B2_ASSET_TRACE
    bool b2_romcopy = lod_b2_trace_matches_asset(s.romcopy_rom, s.romcopy_ram, s.romcopy_size, 0);
    uint32_t b2_before_hash = (b2_romcopy && s.romcopy_ram_ok != 0)
        ? lod_b2_trace_hash_rdram(rdram, s.romcopy_ram_phys, s.romcopy_size)
        : 0;
#endif
#if LOD_ENABLE_KSEG0_FAULT_TRACE
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
#endif
    if (lod_orig_kseg0_dmamgr_romcopy != nullptr) {
        lod_orig_kseg0_dmamgr_romcopy(rdram, ctx);
    }
#if LOD_ENABLE_B2_ASSET_TRACE
    if (b2_romcopy) {
        static int b2_romcopy_count = 0;
        b2_romcopy_count++;
        uint32_t b2_after_hash = s.romcopy_ram_ok != 0
            ? lod_b2_trace_hash_rdram(rdram, s.romcopy_ram_phys, s.romcopy_size)
            : 0;
        if (b2_romcopy_count <= 24 || (b2_romcopy_count % 120) == 0) {
            fprintf(stderr,
                    "[B2_ASSET] romcopy#%d call#%u gs=%u map#%d map_rom=0x%08X "
                    "rom=0x%08X ram=0x%08X size=0x%08X phys=0x%08X "
                    "before_hash=0x%08X after_hash=0x%08X ra=0x%08X ram_ok=%u\n",
                    b2_romcopy_count, s.romcopy_count, s.romcopy_gs,
                    lod_map_ovl_load_count, lod_current_map_ovl_rom,
                    s.romcopy_rom, s.romcopy_ram, s.romcopy_size, s.romcopy_ram_phys,
                    b2_before_hash, b2_after_hash, s.romcopy_ra, s.romcopy_ram_ok);
        }
    }
#endif
}

#if LOD_ENABLE_B2_ASSET_TRACE
static constexpr int LOD_MAP76_MAX_OBJECTS = 384;

static uint32_t lod_map76_object_signature(uint8_t* rdram, uint32_t obj_phys) {
    if (!lod_rdram_range_ok(obj_phys, 0x74)) {
        return 0;
    }

    uint32_t h = 2166136261u;
    auto mix = [&](uint32_t v) {
        h ^= v;
        h *= 16777619u;
    };

    mix((uint16_t)lod_rdram_s16(rdram, obj_phys + 0x00));
    mix((uint16_t)lod_rdram_s16(rdram, obj_phys + 0x02));
    mix((uint16_t)lod_rdram_s16(rdram, obj_phys + 0x04));
    mix((uint16_t)lod_rdram_s16(rdram, obj_phys + 0x06));
    mix(lod_rdram_u32(rdram, obj_phys + 0x08));
    mix(lod_rdram_u32(rdram, obj_phys + 0x0C));
    mix(lod_rdram_u32(rdram, obj_phys + 0x10));
    mix(lod_rdram_u32(rdram, obj_phys + 0x14));
    mix(lod_rdram_u32(rdram, obj_phys + 0x1C));
    mix((uint16_t)lod_rdram_s16(rdram, obj_phys + 0x20));
    mix((uint16_t)lod_rdram_s16(rdram, obj_phys + 0x22));
    for (uint32_t off = 0x24; off <= 0x70; off += 4) {
        mix(lod_rdram_u32(rdram, obj_phys + off));
    }
    return h;
}

static bool lod_map76_is_boss_trace_obj(uint16_t obj_id) {
    switch (obj_id) {
        case 0x08B:
        case 0x0EF:
        case 0x0FE:
        case 0x0FF:
        case 0x1E1:
        case 0x1E2:
        case 0x1E3:
            return true;
        default:
            return false;
    }
}

static bool lod_map76_is_boss_chain_obj(uint16_t obj_id) {
    switch (obj_id) {
        case 0x0EF: // werewolf intro/appearance model that vanishes
        case 0x1E1:
        case 0x1E2:
        case 0x1E3:
            return true;
        default:
            return false;
    }
}

static bool lod_map76_is_combat_boss_obj(uint16_t obj_id) {
    switch (obj_id) {
        case 0x1E1:
        case 0x1E2:
        case 0x1E3:
            return true;
        default:
            return false;
    }
}

extern uint32_t trace_ring[];
extern int trace_ring_pos;
extern int trace_total;

static void lod_map76_print_trace_ring(const char* tag) {
    int count = trace_total < 32 ? trace_total : 32;
    if (count <= 0) {
        return;
    }

    fprintf(stderr, "%s last_func_lookups:", tag);
    for (int i = 0; i < count; i++) {
        int idx = (trace_ring_pos - count + i + 32) % 32;
        fprintf(stderr, " 0x%08X", trace_ring[idx]);
    }
    fprintf(stderr, "\n");
}

static float lod_map76_f32(uint32_t bits) {
    float out = 0.0f;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

static bool lod_map76_find_fig_owner(uint8_t* rdram, uint32_t first_phys, uint32_t end_phys,
                                     uint32_t fig_addr, int* owner_slot,
                                     uint16_t* owner_id, int* owner_fig_index) {
    if (owner_slot) {
        *owner_slot = -1;
    }
    if (owner_id) {
        *owner_id = 0;
    }
    if (owner_fig_index) {
        *owner_fig_index = -1;
    }

    if (fig_addr == 0) {
        return false;
    }

    int slot = 0;
    for (uint32_t phys = first_phys; phys < end_phys && slot < LOD_MAP76_MAX_OBJECTS; phys += 0x74, slot++) {
        if (!lod_rdram_range_ok(phys, 0x74)) {
            break;
        }

        uint16_t raw_id = (uint16_t)lod_rdram_s16(rdram, phys + 0x00);
        uint16_t obj_id = raw_id & 0x07FF;
        if (obj_id == 0) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            if (lod_rdram_u32(rdram, phys + 0x24 + (uint32_t)i * 4) == fig_addr) {
                if (owner_slot) {
                    *owner_slot = slot;
                }
                if (owner_id) {
                    *owner_id = obj_id;
                }
                if (owner_fig_index) {
                    *owner_fig_index = i;
                }
                return true;
            }
        }
    }

    return false;
}

static uint32_t lod_map76_figure_chain_hash(uint8_t* rdram, uint32_t fig_addr) {
    uint32_t h = 2166136261u;
    auto mix = [&](uint32_t v) {
        h ^= v;
        h *= 16777619u;
    };

    uint32_t seen[8] = {};
    uint32_t cur = fig_addr;
    for (int depth = 0; depth < 8 && cur != 0; depth++) {
        for (int i = 0; i < depth; i++) {
            if (seen[i] == cur) {
                mix(0xC1C1C1C1u);
                return h;
            }
        }
        seen[depth] = cur;

        uint32_t phys = 0;
        if (!lod_decode_rdram_phys_addr(cur, 0x6C, &phys)) {
            mix(0xBAD00000u ^ cur);
            return h;
        }

        mix(cur);
        mix((uint16_t)lod_rdram_s16(rdram, phys + 0x00)); // type/hidden
        mix((uint16_t)lod_rdram_s16(rdram, phys + 0x02)); // flags
        mix(lod_rdram_u32(rdram, phys + 0x04));           // prev
        mix(lod_rdram_u32(rdram, phys + 0x08));           // sibling
        mix(lod_rdram_u32(rdram, phys + 0x0C));           // next
        mix(lod_rdram_u32(rdram, phys + 0x10));           // parent
        mix(lod_rdram_u32(rdram, phys + 0x14));           // child/model child
        mix(lod_rdram_u32(rdram, phys + 0x38));           // asset/material-ish
        mix(lod_rdram_u32(rdram, phys + 0x3C));           // asset/dlist-ish

        cur = lod_rdram_u32(rdram, phys + 0x10);
    }

    return h;
}

static uint32_t lod_map76_figure_signature(uint8_t* rdram, uint32_t fig_phys) {
    uint32_t h = 2166136261u;
    auto mix = [&](uint32_t v) {
        h ^= v;
        h *= 16777619u;
    };

    // Keep this structural. Position/animation words change every frame and
    // drown the useful lifecycle transitions (hidden/visible, link, asset).
    mix((uint16_t)lod_rdram_s16(rdram, fig_phys + 0x00)); // type/hidden
    mix((uint16_t)lod_rdram_s16(rdram, fig_phys + 0x02)); // flags
    mix(lod_rdram_u32(rdram, fig_phys + 0x04));           // prev
    mix(lod_rdram_u32(rdram, fig_phys + 0x08));           // sibling
    mix(lod_rdram_u32(rdram, fig_phys + 0x0C));           // next
    mix(lod_rdram_u32(rdram, fig_phys + 0x10));           // parent
    mix(lod_rdram_u32(rdram, fig_phys + 0x14));           // child/model child
    mix(lod_rdram_u32(rdram, fig_phys + 0x30));           // material-ish
    mix(lod_rdram_u32(rdram, fig_phys + 0x34));           // display-list-ish
    mix(lod_rdram_u32(rdram, fig_phys + 0x38));           // asset/material-ish
    mix(lod_rdram_u32(rdram, fig_phys + 0x3C));           // asset/dlist-ish
    mix(lod_rdram_u32(rdram, fig_phys + 0x64));           // map/extra ptr-ish
    return h;
}

static bool lod_map76_force_chain_dump(int dump_count) {
    (void)dump_count;
    return false;
}

static bool lod_map76_focus_window(bool trace_armed) {
    return trace_armed;
}

static void lod_map76_active_object_dump(uint8_t* rdram, uint32_t first_phys, uint32_t end_phys,
                                         int dump_count, int si_dma_count, const char* reason,
                                         int trigger_slot, uint16_t trigger_id) {
    fprintf(stderr,
            "[MAP76_ACTIVE] dump#%d si=%d reason=%s trigger_slot=%d trigger_id=0x%03X "
            "base=0x%08X end=0x%08X\n",
            dump_count, si_dma_count, reason ? reason : "unknown", trigger_slot, trigger_id,
            first_phys | 0x80000000, end_phys | 0x80000000);

    int slot = 0;
    int lines = 0;
    int active = 0;
    for (uint32_t phys = first_phys; phys < end_phys && slot < LOD_MAP76_MAX_OBJECTS; phys += 0x74, slot++) {
        if (!lod_rdram_range_ok(phys, 0x74)) {
            break;
        }

        uint16_t raw_id = (uint16_t)lod_rdram_s16(rdram, phys + 0x00);
        uint16_t obj_id = raw_id & 0x07FF;
        if (obj_id == 0) {
            continue;
        }

        active++;
        if (lines >= 180) {
            continue;
        }

        uint16_t flags = (uint16_t)lod_rdram_s16(rdram, phys + 0x02);
        uint8_t s08 = lod_rdram_u8(rdram, phys + 0x08);
        uint8_t s09 = lod_rdram_u8(rdram, phys + 0x09);
        uint8_t s0a = lod_rdram_u8(rdram, phys + 0x0A);
        uint8_t s0b = lod_rdram_u8(rdram, phys + 0x0B);
        uint8_t s0c = lod_rdram_u8(rdram, phys + 0x0C);
        uint8_t s0d = lod_rdram_u8(rdram, phys + 0x0D);
        int16_t func_id = lod_rdram_s16(rdram, phys + 0x0E);
        uint8_t dispatch = lod_object_dispatch_state(rdram, phys);
        uint32_t destroy = lod_rdram_u32(rdram, phys + 0x10);
        uint32_t parent = lod_rdram_u32(rdram, phys + 0x14);
        uint32_t child = lod_rdram_u32(rdram, phys + 0x1C);
        uint16_t alloc = (uint16_t)lod_rdram_s16(rdram, phys + 0x20);
        uint16_t gfx = (uint16_t)lod_rdram_s16(rdram, phys + 0x22);
        uint32_t fig0 = lod_rdram_u32(rdram, phys + 0x24);
        uint32_t fig1 = lod_rdram_u32(rdram, phys + 0x28);
        uint32_t fig2 = lod_rdram_u32(rdram, phys + 0x2C);
        uint32_t fig3 = lod_rdram_u32(rdram, phys + 0x30);
        uint32_t data0 = lod_rdram_u32(rdram, phys + 0x34);
        uint32_t data1 = lod_rdram_u32(rdram, phys + 0x38);
        uint32_t data2 = lod_rdram_u32(rdram, phys + 0x3C);
        uint32_t data3 = lod_rdram_u32(rdram, phys + 0x40);
        bool boss_related = lod_map76_is_boss_trace_obj(obj_id) ||
                            parent == ((uint32_t)(first_phys + (uint32_t)trigger_slot * 0x74) | 0x80000000) ||
                            child == ((uint32_t)(first_phys + (uint32_t)trigger_slot * 0x74) | 0x80000000);

        fprintf(stderr,
                "[MAP76_ACTIVE] slot=%d addr=0x%08X id=0x%03X raw=0x%04X flags=0x%04X boss_related=%d "
                "sched=%02X/%02X %02X/%02X %02X/%02X func=%d disp=%u "
                "destroy=0x%08X parent=0x%08X child=0x%08X bits={alloc=0x%04X gfx=0x%04X} "
                "fig={0x%08X,0x%08X,0x%08X,0x%08X} data={0x%08X,0x%08X,0x%08X,0x%08X}\n",
                slot, phys | 0x80000000, obj_id, raw_id, flags, boss_related ? 1 : 0,
                s08, s09, s0a, s0b, s0c, s0d, func_id, dispatch,
                destroy, parent, child, alloc, gfx,
                fig0, fig1, fig2, fig3, data0, data1, data2, data3);
        lines++;
    }

    fprintf(stderr,
            "[MAP76_ACTIVE] summary dump#%d si=%d active=%d printed=%d total_slots=%d\n",
            dump_count, si_dma_count, active, lines, slot);
}

static void lod_map76_figure_chain_trace(uint8_t* rdram, uint32_t first_phys, uint32_t end_phys,
                                         int dump_count, int si_dma_count,
                                         int slot, uint16_t obj_id, int fig_index,
                                         uint32_t fig_addr) {
    if (slot < 0 || slot >= LOD_MAP76_MAX_OBJECTS || fig_index < 0 || fig_index >= 4 || fig_addr == 0) {
        return;
    }

    static uint32_t last_ptr[LOD_MAP76_MAX_OBJECTS][4] = {};
    static uint32_t last_hash[LOD_MAP76_MAX_OBJECTS][4] = {};

    uint32_t chain_hash = lod_map76_figure_chain_hash(rdram, fig_addr);
    const bool force_dump = lod_map76_force_chain_dump(dump_count);
    bool changed = fig_addr != last_ptr[slot][fig_index] ||
                   (force_dump && chain_hash != last_hash[slot][fig_index]);
    if (!changed) {
        return;
    }

    last_ptr[slot][fig_index] = fig_addr;
    last_hash[slot][fig_index] = chain_hash;

    fprintf(stderr,
            "[MAP76_CHAIN] dump#%d si=%d slot=%d id=0x%03X fig%d root=0x%08X hash=0x%08X changed=%d\n",
            dump_count, si_dma_count, slot, obj_id, fig_index, fig_addr,
            chain_hash, changed ? 1 : 0);

    uint32_t seen[8] = {};
    uint32_t cur = fig_addr;
    for (int depth = 0; depth < 8 && cur != 0; depth++) {
        bool cycle = false;
        for (int i = 0; i < depth; i++) {
            if (seen[i] == cur) {
                cycle = true;
                break;
            }
        }
        if (cycle) {
            fprintf(stderr,
                    "[MAP76_CHAIN] dump#%d slot=%d id=0x%03X fig%d depth=%d ptr=0x%08X CYCLE\n",
                    dump_count, slot, obj_id, fig_index, depth, cur);
            return;
        }
        seen[depth] = cur;

        uint32_t phys = 0;
        if (!lod_decode_rdram_phys_addr(cur, 0x6C, &phys)) {
            fprintf(stderr,
                    "[MAP76_CHAIN] dump#%d slot=%d id=0x%03X fig%d depth=%d ptr=0x%08X BAD_PTR\n",
                    dump_count, slot, obj_id, fig_index, depth, cur);
            return;
        }

        int owner_slot = -1;
        uint16_t owner_id = 0;
        int owner_fig = -1;
        lod_map76_find_fig_owner(rdram, first_phys, end_phys, cur,
                                 &owner_slot, &owner_id, &owner_fig);

        int16_t type = lod_rdram_s16(rdram, phys + 0x00);
        uint16_t flags = (uint16_t)lod_rdram_s16(rdram, phys + 0x02);
        uint32_t prev = lod_rdram_u32(rdram, phys + 0x04);
        uint32_t sibling = lod_rdram_u32(rdram, phys + 0x08);
        uint32_t next = lod_rdram_u32(rdram, phys + 0x0C);
        uint32_t parent = lod_rdram_u32(rdram, phys + 0x10);
        uint32_t child = lod_rdram_u32(rdram, phys + 0x14);
        uint32_t w38 = lod_rdram_u32(rdram, phys + 0x38);
        uint32_t w3c = lod_rdram_u32(rdram, phys + 0x3C);
        uint32_t w40 = lod_rdram_u32(rdram, phys + 0x40);
        uint32_t w44 = lod_rdram_u32(rdram, phys + 0x44);
        uint32_t w48 = lod_rdram_u32(rdram, phys + 0x48);

        fprintf(stderr,
                "[MAP76_CHAIN] dump#%d slot=%d id=0x%03X fig%d depth=%d ptr=0x%08X phys=0x%06X "
                "owner=%d/0x%03X/fig%d type=0x%04X(%d) hidden=%d flags=0x%04X "
                "links={prev=0x%08X sib=0x%08X next=0x%08X parent=0x%08X child=0x%08X} "
                "asset={w38=0x%08X w3c=0x%08X} pos={%.2f,%.2f,%.2f}\n",
                dump_count, slot, obj_id, fig_index, depth, cur, phys,
                owner_slot, owner_id, owner_fig,
                (uint16_t)type, type, type < 0 ? 1 : 0, flags,
                prev, sibling, next, parent, child,
                w38, w3c,
                lod_map76_f32(w40), lod_map76_f32(w44), lod_map76_f32(w48));

        cur = parent;
    }
}

static void lod_map76_figure_trace(uint8_t* rdram, int dump_count, int si_dma_count,
                                   int slot, uint16_t obj_id, int fig_index,
                                   uint32_t fig_addr) {
    if (slot < 0 || slot >= LOD_MAP76_MAX_OBJECTS || fig_index < 0 || fig_index >= 4) {
        return;
    }

    static uint32_t last_ptr[LOD_MAP76_MAX_OBJECTS][4] = {};
    static uint32_t last_hash[LOD_MAP76_MAX_OBJECTS][4] = {};

    uint32_t fig_phys = 0;
    bool fig_ok = fig_addr != 0 && lod_decode_rdram_phys_addr(fig_addr, 0xA8, &fig_phys);
    uint32_t fig_hash = fig_ok ? lod_map76_figure_signature(rdram, fig_phys) : 0;
    bool changed = fig_addr != last_ptr[slot][fig_index] ||
                   fig_hash != last_hash[slot][fig_index];

    if (fig_addr == 0 && !changed) {
        return;
    }

    if (!changed) {
        return;
    }

    last_ptr[slot][fig_index] = fig_addr;
    last_hash[slot][fig_index] = fig_hash;

    if (fig_addr == 0 || !fig_ok) {
        fprintf(stderr,
                "[MAP76_FIG] dump#%d si=%d slot=%d id=0x%03X fig%d ptr=0x%08X ok=%d hash=0x%08X changed=%d\n",
                dump_count, si_dma_count, slot, obj_id, fig_index, fig_addr,
                fig_ok ? 1 : 0, fig_hash, changed ? 1 : 0);
        return;
    }

    int16_t type = lod_rdram_s16(rdram, fig_phys + 0x00);
    uint16_t flags = (uint16_t)lod_rdram_s16(rdram, fig_phys + 0x02);
    uint32_t prev = lod_rdram_u32(rdram, fig_phys + 0x04);
    uint32_t sibling = lod_rdram_u32(rdram, fig_phys + 0x08);
    uint32_t next = lod_rdram_u32(rdram, fig_phys + 0x0C);
    uint32_t parent = lod_rdram_u32(rdram, fig_phys + 0x10);
    uint32_t w14 = lod_rdram_u32(rdram, fig_phys + 0x14);
    uint32_t w18 = lod_rdram_u32(rdram, fig_phys + 0x18);
    uint32_t w1c = lod_rdram_u32(rdram, fig_phys + 0x1C);
    uint32_t w20 = lod_rdram_u32(rdram, fig_phys + 0x20);
    uint32_t w24 = lod_rdram_u32(rdram, fig_phys + 0x24);
    uint32_t w28 = lod_rdram_u32(rdram, fig_phys + 0x28);
    uint32_t w2c = lod_rdram_u32(rdram, fig_phys + 0x2C);
    uint32_t w30 = lod_rdram_u32(rdram, fig_phys + 0x30);
    uint32_t w34 = lod_rdram_u32(rdram, fig_phys + 0x34);
    uint32_t w38 = lod_rdram_u32(rdram, fig_phys + 0x38);
    uint32_t w3c = lod_rdram_u32(rdram, fig_phys + 0x3C);
    uint32_t w40 = lod_rdram_u32(rdram, fig_phys + 0x40);
    uint32_t w44 = lod_rdram_u32(rdram, fig_phys + 0x44);
    uint32_t w48 = lod_rdram_u32(rdram, fig_phys + 0x48);
    uint32_t w4c = lod_rdram_u32(rdram, fig_phys + 0x4C);
    uint32_t w50 = lod_rdram_u32(rdram, fig_phys + 0x50);
    uint32_t w54 = lod_rdram_u32(rdram, fig_phys + 0x54);
    uint32_t w58 = lod_rdram_u32(rdram, fig_phys + 0x58);
    uint32_t w5c = lod_rdram_u32(rdram, fig_phys + 0x5C);
    uint32_t w60 = lod_rdram_u32(rdram, fig_phys + 0x60);
    uint32_t w64 = lod_rdram_u32(rdram, fig_phys + 0x64);
    uint32_t w68 = lod_rdram_u32(rdram, fig_phys + 0x68);

    fprintf(stderr,
            "[MAP76_FIG] dump#%d si=%d slot=%d id=0x%03X fig%d ptr=0x%08X phys=0x%06X "
            "type=0x%04X(%d) hidden=%d flags=0x%04X hash=0x%08X changed=%d "
            "links={prev=0x%08X sib=0x%08X next=0x%08X parent=0x%08X} "
            "model={mat=0x%08X dl=0x%08X f38=0x%08X f3c=0x%08X map=0x%08X} "
            "pos={%.2f,%.2f,%.2f} scale={%.3f,%.3f,%.3f} "
            "w14-68={0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,"
            "0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,"
            "0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}\n",
            dump_count, si_dma_count, slot, obj_id, fig_index, fig_addr, fig_phys,
            (uint16_t)type, type, type < 0 ? 1 : 0, flags, fig_hash, changed ? 1 : 0,
            prev, sibling, next, parent,
            w30, w34, w38, w3c, w64,
            lod_map76_f32(w40), lod_map76_f32(w44), lod_map76_f32(w48),
            lod_map76_f32(w58), lod_map76_f32(w5c), lod_map76_f32(w60),
            w14, w18, w1c, w20, w24, w28, w2c, w30, w34, w38, w3c,
            w40, w44, w48, w4c, w50, w54, w58, w5c, w60, w64, w68);
}

static void lod_map76_boss_figure_periodic_trace(uint8_t* rdram, int dump_count, int si_dma_count,
                                                 int slot, uint16_t obj_id, int fig_index,
                                                 uint32_t fig_addr) {
    if (!lod_map76_is_combat_boss_obj(obj_id) ||
        slot < 0 || slot >= LOD_MAP76_MAX_OBJECTS ||
        fig_index < 0 || fig_index >= 4) {
        return;
    }

    static bool printed_once[LOD_MAP76_MAX_OBJECTS][4] = {};
    bool due = !printed_once[slot][fig_index] ||
               (LOD_MAP76_BOSS_FIG_INTERVAL > 0 &&
                (dump_count % LOD_MAP76_BOSS_FIG_INTERVAL) == 0);
    if (!due) {
        return;
    }

    printed_once[slot][fig_index] = true;

    if (fig_addr == 0) {
        fprintf(stderr,
                "[MAP76_BOSS_FIG] dump#%d si=%d slot=%d id=0x%03X fig%d ptr=0x00000000 ok=0\n",
                dump_count, si_dma_count, slot, obj_id, fig_index);
        return;
    }

    uint32_t fig_phys = 0;
    if (!lod_decode_rdram_phys_addr(fig_addr, 0xA8, &fig_phys)) {
        fprintf(stderr,
                "[MAP76_BOSS_FIG] dump#%d si=%d slot=%d id=0x%03X fig%d ptr=0x%08X ok=0\n",
                dump_count, si_dma_count, slot, obj_id, fig_index, fig_addr);
        return;
    }

    int16_t type = lod_rdram_s16(rdram, fig_phys + 0x00);
    uint16_t flags = (uint16_t)lod_rdram_s16(rdram, fig_phys + 0x02);
    uint32_t prev = lod_rdram_u32(rdram, fig_phys + 0x04);
    uint32_t sibling = lod_rdram_u32(rdram, fig_phys + 0x08);
    uint32_t next = lod_rdram_u32(rdram, fig_phys + 0x0C);
    uint32_t parent = lod_rdram_u32(rdram, fig_phys + 0x10);
    uint32_t w14 = lod_rdram_u32(rdram, fig_phys + 0x14);
    uint32_t w18 = lod_rdram_u32(rdram, fig_phys + 0x18);
    uint32_t w1c = lod_rdram_u32(rdram, fig_phys + 0x1C);
    uint32_t w20 = lod_rdram_u32(rdram, fig_phys + 0x20);
    uint32_t w24 = lod_rdram_u32(rdram, fig_phys + 0x24);
    uint32_t w28 = lod_rdram_u32(rdram, fig_phys + 0x28);
    uint32_t w2c = lod_rdram_u32(rdram, fig_phys + 0x2C);
    uint32_t w30 = lod_rdram_u32(rdram, fig_phys + 0x30);
    uint32_t w34 = lod_rdram_u32(rdram, fig_phys + 0x34);
    uint32_t w38 = lod_rdram_u32(rdram, fig_phys + 0x38);
    uint32_t w3c = lod_rdram_u32(rdram, fig_phys + 0x3C);
    uint32_t w40 = lod_rdram_u32(rdram, fig_phys + 0x40);
    uint32_t w44 = lod_rdram_u32(rdram, fig_phys + 0x44);
    uint32_t w48 = lod_rdram_u32(rdram, fig_phys + 0x48);
    uint32_t w4c = lod_rdram_u32(rdram, fig_phys + 0x4C);
    uint32_t w50 = lod_rdram_u32(rdram, fig_phys + 0x50);
    uint32_t w54 = lod_rdram_u32(rdram, fig_phys + 0x54);
    uint32_t w58 = lod_rdram_u32(rdram, fig_phys + 0x58);
    uint32_t w5c = lod_rdram_u32(rdram, fig_phys + 0x5C);
    uint32_t w60 = lod_rdram_u32(rdram, fig_phys + 0x60);
    uint32_t w64 = lod_rdram_u32(rdram, fig_phys + 0x64);
    uint32_t w68 = lod_rdram_u32(rdram, fig_phys + 0x68);
    uint32_t chain_hash = lod_map76_figure_chain_hash(rdram, fig_addr);

    fprintf(stderr,
            "[MAP76_BOSS_FIG] dump#%d si=%d slot=%d id=0x%03X fig%d ptr=0x%08X phys=0x%06X "
            "type=0x%04X(%d) hidden=%d flags=0x%04X chain=0x%08X "
            "links={prev=0x%08X sib=0x%08X next=0x%08X parent=0x%08X} "
            "model={mat=0x%08X dl=0x%08X f38=0x%08X f3c=0x%08X map=0x%08X} "
            "pos={%.2f,%.2f,%.2f} scale={%.3f,%.3f,%.3f} "
            "w14-68={0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,"
            "0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,"
            "0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X}\n",
            dump_count, si_dma_count, slot, obj_id, fig_index, fig_addr, fig_phys,
            (uint16_t)type, type, type < 0 ? 1 : 0, flags, chain_hash,
            prev, sibling, next, parent,
            w30, w34, w38, w3c, w64,
            lod_map76_f32(w40), lod_map76_f32(w44), lod_map76_f32(w48),
            lod_map76_f32(w58), lod_map76_f32(w5c), lod_map76_f32(w60),
            w14, w18, w1c, w20, w24, w28, w2c, w30, w34, w38, w3c,
            w40, w44, w48, w4c, w50, w54, w58, w5c, w60, w64, w68);
}

static void lod_map76_object_trace(uint8_t* rdram, int si_dma_count) {
    if (lod_current_map_ovl_rom != LOD_B2_TRACE_MAP_ROM) {
        return;
    }

    int32_t gs = lod_current_gamestate(rdram);
    if (gs != 3) {
        return;
    }

    uint32_t list_base = lod_rdram_range_ok(0x0C1530, 4) ? lod_rdram_u32(rdram, 0x0C1530) : 0;
    uint32_t list_end = lod_rdram_range_ok(0x0C153C, 4) ? lod_rdram_u32(rdram, 0x0C153C) : 0;
    uint32_t first = list_base + 0x74;
    uint32_t first_phys = first & 0x1FFFFFFF;
    uint32_t end_phys = list_end & 0x1FFFFFFF;
    if (list_base == 0 || list_end == 0 ||
        first_phys >= end_phys ||
        !lod_rdram_range_ok(first_phys, 0x74) ||
        !lod_rdram_range_ok(end_phys - 0x74, 0x74)) {
        static int bad_count = 0;
        if (++bad_count <= 8) {
            fprintf(stderr,
                    "[MAP76_OBJ] bad-list #%d si=%d gs=%d base=0x%08X end=0x%08X first_phys=0x%06X end_phys=0x%06X\n",
                    bad_count, si_dma_count, gs, list_base, list_end, first_phys, end_phys);
        }
        return;
    }

    static uint32_t last_sig[LOD_MAP76_MAX_OBJECTS] = {};
    static uint16_t last_id[LOD_MAP76_MAX_OBJECTS] = {};
    static uint16_t last_raw_id[LOD_MAP76_MAX_OBJECTS] = {};
    static uint16_t last_flags[LOD_MAP76_MAX_OBJECTS] = {};
    static uint32_t last_figs[LOD_MAP76_MAX_OBJECTS][4] = {};
    static int dump_count = 0;
    static int last_si = -100000;
    static int trace_armed_until = 0;
    static bool trace_armed_logged = false;
    static bool handoff_dumped = false;

    bool periodic = (si_dma_count - last_si) >= LOD_MAP76_TRACE_SI_INTERVAL;
    if (!periodic && dump_count != 0) {
        return;
    }

    last_si = si_dma_count;
    dump_count++;

    int total = 0;
    int active = 0;
    int with_fig = 0;
    uint32_t table_hash = 2166136261u;
    int lines = 0;

    for (uint32_t phys = first_phys; phys < end_phys && total < LOD_MAP76_MAX_OBJECTS; phys += 0x74, total++) {
        if (!lod_rdram_range_ok(phys, 0x74)) {
            break;
        }

        uint16_t raw_id = (uint16_t)lod_rdram_s16(rdram, phys + 0x00);
        uint16_t obj_id = raw_id & 0x07FF;
        uint16_t flags = (uint16_t)lod_rdram_s16(rdram, phys + 0x02);
        if (obj_id == 0) {
            bool trace_armed_now = trace_armed_until > 0 && dump_count <= trace_armed_until;
            bool interesting_removed =
                last_id[total] != 0 &&
                (trace_armed_now || last_id[total] == 0x0EF) &&
                lod_map76_is_boss_trace_obj(last_id[total]);
            if (interesting_removed) {
                uint32_t sig = lod_map76_object_signature(rdram, phys);
                fprintf(stderr,
                        "[MAP76_OBJ] removed dump#%d si=%d slot=%d addr=0x%08X "
                        "prev_id=0x%03X prev_raw=0x%04X prev_flags=0x%04X prev_sig=0x%08X "
                        "cur_raw=0x%04X cur_flags=0x%04X cur_sig=0x%08X "
                        "prev_fig={0x%08X,0x%08X,0x%08X,0x%08X}\n",
                        dump_count, si_dma_count, total, phys | 0x80000000,
                        last_id[total], last_raw_id[total], last_flags[total], last_sig[total],
                        raw_id, flags, sig,
                        last_figs[total][0], last_figs[total][1],
                        last_figs[total][2], last_figs[total][3]);
                lod_map76_print_trace_ring("[MAP76_OBJ] removed");
                if (last_id[total] == 0x0EF && !handoff_dumped) {
                    lod_map76_active_object_dump(rdram, first_phys, end_phys, dump_count, si_dma_count,
                                                 "0EF removed-slot", total, last_id[total]);
                    handoff_dumped = true;
                }
            }
            last_sig[total] = 0;
            last_id[total] = 0;
            last_raw_id[total] = raw_id;
            last_flags[total] = flags;
            last_figs[total][0] = 0;
            last_figs[total][1] = 0;
            last_figs[total][2] = 0;
            last_figs[total][3] = 0;
            continue;
        }

        if (obj_id == 0x0EF) {
            // The werewolf intro/appearance object only exists at boss entry.
            // Use it as the trigger so pre-boss Fog Lake gameplay stays quiet.
            if (!trace_armed_logged) {
                trace_armed_until = dump_count + 70;
                fprintf(stderr,
                        "[MAP76_TRACE] armed dump#%d si=%d slot=%d addr=0x%08X raw=0x%04X flags=0x%04X until_dump#%d\n",
                        dump_count, si_dma_count, total, phys | 0x80000000,
                        raw_id, flags, trace_armed_until);
                trace_armed_logged = true;
            }
        }

        bool trace_armed = trace_armed_until > 0 && dump_count <= trace_armed_until;

        active++;
        uint32_t fig0 = lod_rdram_u32(rdram, phys + 0x24);
        uint32_t fig1 = lod_rdram_u32(rdram, phys + 0x28);
        uint32_t fig2 = lod_rdram_u32(rdram, phys + 0x2C);
        uint32_t fig3 = lod_rdram_u32(rdram, phys + 0x30);
        if (fig0 || fig1 || fig2 || fig3) {
            with_fig++;
        }

        uint32_t sig = lod_map76_object_signature(rdram, phys);
        table_hash ^= sig;
        table_hash *= 16777619u;

        bool changed = sig != last_sig[total] || obj_id != last_id[total];
        bool raw_changed = raw_id != last_raw_id[total];
        bool flags_changed = flags != last_flags[total];
        bool raw_retired = ((int16_t)raw_id < 0) && raw_changed;
        bool fig_changed = fig0 != last_figs[total][0] ||
                           fig1 != last_figs[total][1] ||
                           fig2 != last_figs[total][2] ||
                           fig3 != last_figs[total][3];
        bool suspicious = (fig0 == 0 && fig1 == 0 && fig2 == 0 && fig3 == 0) ||
                          (flags & 0x8000) != 0 ||
                          (obj_id >= 0x080 && obj_id <= 0x1FF);
        bool boss_trace = lod_map76_is_boss_trace_obj(obj_id);
        bool chain_trace = lod_map76_is_boss_chain_obj(obj_id);
        bool lifecycle_changed = obj_id != last_id[total] ||
                                 raw_changed ||
                                 flags_changed ||
                                 fig_changed;
        bool appearance_retiring =
            obj_id == 0x0EF &&
            !handoff_dumped &&
            (raw_retired ||
             ((flags & 0x8000) != 0 && flags_changed) ||
             (fig_changed && fig0 == 0 && fig1 == 0 && fig2 == 0 && fig3 == 0));
        if (chain_trace && lod_map76_focus_window(trace_armed)) {
            lod_map76_figure_trace(rdram, dump_count, si_dma_count, total, obj_id, 0, fig0);
            lod_map76_figure_trace(rdram, dump_count, si_dma_count, total, obj_id, 1, fig1);
            lod_map76_figure_trace(rdram, dump_count, si_dma_count, total, obj_id, 2, fig2);
            lod_map76_figure_trace(rdram, dump_count, si_dma_count, total, obj_id, 3, fig3);
            lod_map76_boss_figure_periodic_trace(rdram, dump_count, si_dma_count, total, obj_id, 0, fig0);
            lod_map76_boss_figure_periodic_trace(rdram, dump_count, si_dma_count, total, obj_id, 1, fig1);
            lod_map76_boss_figure_periodic_trace(rdram, dump_count, si_dma_count, total, obj_id, 2, fig2);
            lod_map76_boss_figure_periodic_trace(rdram, dump_count, si_dma_count, total, obj_id, 3, fig3);
            lod_map76_figure_chain_trace(rdram, first_phys, end_phys, dump_count, si_dma_count, total, obj_id, 0, fig0);
            lod_map76_figure_chain_trace(rdram, first_phys, end_phys, dump_count, si_dma_count, total, obj_id, 1, fig1);
            lod_map76_figure_chain_trace(rdram, first_phys, end_phys, dump_count, si_dma_count, total, obj_id, 2, fig2);
            lod_map76_figure_chain_trace(rdram, first_phys, end_phys, dump_count, si_dma_count, total, obj_id, 3, fig3);
        }
        if (appearance_retiring) {
            lod_map76_active_object_dump(rdram, first_phys, end_phys, dump_count, si_dma_count,
                                         "0EF retiring", total, obj_id);
            handoff_dumped = true;
        }

        bool log_object_detail =
            boss_trace &&
            (trace_armed || obj_id == 0x0EF) &&
            lifecycle_changed;
        if (log_object_detail) {
            if (lines < 80) {
                uint8_t s08 = lod_rdram_u8(rdram, phys + 0x08);
                uint8_t s09 = lod_rdram_u8(rdram, phys + 0x09);
                uint8_t s0a = lod_rdram_u8(rdram, phys + 0x0A);
                uint8_t s0b = lod_rdram_u8(rdram, phys + 0x0B);
                uint8_t s0c = lod_rdram_u8(rdram, phys + 0x0C);
                uint8_t s0d = lod_rdram_u8(rdram, phys + 0x0D);
                int16_t func_id = lod_rdram_s16(rdram, phys + 0x0E);
                uint8_t dispatch = lod_object_dispatch_state(rdram, phys);
                uint16_t alloc = (uint16_t)lod_rdram_s16(rdram, phys + 0x20);
                uint16_t gfx = (uint16_t)lod_rdram_s16(rdram, phys + 0x22);
                uint32_t destroy = lod_rdram_u32(rdram, phys + 0x10);
                uint32_t parent = lod_rdram_u32(rdram, phys + 0x14);
                uint32_t child = lod_rdram_u32(rdram, phys + 0x1C);
                uint32_t data0 = lod_rdram_u32(rdram, phys + 0x34);
                uint32_t data1 = lod_rdram_u32(rdram, phys + 0x38);
                uint32_t data2 = lod_rdram_u32(rdram, phys + 0x3C);
                uint32_t data3 = lod_rdram_u32(rdram, phys + 0x40);
                fprintf(stderr,
                        "[MAP76_OBJ] obj dump#%d si=%d slot=%d addr=0x%08X id=0x%03X raw=0x%04X flags=0x%04X "
                        "sched=%02X/%02X %02X/%02X %02X/%02X func=%d disp=%u destroy=0x%08X parent=0x%08X child=0x%08X "
                        "bits={alloc=0x%04X gfx=0x%04X} fig={0x%08X,0x%08X,0x%08X,0x%08X} "
                        "data={0x%08X,0x%08X,0x%08X,0x%08X} sig=0x%08X changed=%d fig_changed=%d susp=%d\n",
                        dump_count, si_dma_count, total, phys | 0x80000000, obj_id, raw_id, flags,
                        s08, s09, s0a, s0b, s0c, s0d, func_id, dispatch, destroy, parent, child,
                        alloc, gfx, fig0, fig1, fig2, fig3, data0, data1, data2, data3,
                        sig, changed ? 1 : 0, fig_changed ? 1 : 0, suspicious ? 1 : 0);
                lines++;
            }
        }
        if (boss_trace && raw_retired) {
            lod_map76_print_trace_ring("[MAP76_OBJ] retired");
        }

        last_sig[total] = sig;
        last_id[total] = obj_id;
        last_raw_id[total] = raw_id;
        last_flags[total] = flags;
        last_figs[total][0] = fig0;
        last_figs[total][1] = fig1;
        last_figs[total][2] = fig2;
        last_figs[total][3] = fig3;
    }

    if (trace_armed_until > 0 && dump_count <= trace_armed_until && (lines > 0 || (dump_count % 20) == 0)) {
        fprintf(stderr,
                "[MAP76_OBJ] summary dump#%d si=%d gs=%d map#%d active=%d with_fig=%d total_slots=%d hash=0x%08X lines=%d base=0x%08X end=0x%08X\n",
                dump_count, si_dma_count, gs, lod_map_ovl_load_count, active, with_fig, total,
                table_hash, lines, list_base, list_end);
    }
}

// Generated main-code helpers used by object 0x0EF's update wrapper
// (declared manually to avoid including generated funcs.h here).
void func_8002D548(uint8_t* rdram, recomp_context* ctx);
void func_8002D1F4(uint8_t* rdram, recomp_context* ctx);
void object_destroyChildrenAndModelInfo(uint8_t* rdram, recomp_context* ctx);

struct LodMap76LifecycleObject {
    bool ok = false;
    int slot = -1;
    uint32_t addr = 0;
    uint32_t phys = 0;
    uint16_t raw = 0;
    uint16_t id = 0;
    uint16_t flags = 0;
    uint8_t sched[6] = {};
    int16_t func = 0;
    uint8_t disp = 0;
    uint32_t destroy = 0;
    uint32_t parent = 0;
    uint32_t child = 0;
    uint16_t alloc = 0;
    uint16_t gfx = 0;
    uint32_t figs[4] = {};
    uint32_t data[4] = {};
    uint32_t sig = 0;
    uint16_t fig0_type = 0;
    uint16_t fig0_flags = 0;
    uint32_t fig0_dl = 0;
};

static int lod_map76_lifecycle_slot_for_phys(uint8_t* rdram, uint32_t phys) {
    uint32_t list_base = lod_rdram_range_ok(0x0C1530, 4) ? lod_rdram_u32(rdram, 0x0C1530) : 0;
    uint32_t list_end = lod_rdram_range_ok(0x0C153C, 4) ? lod_rdram_u32(rdram, 0x0C153C) : 0;
    uint32_t first_phys = (list_base + 0x74) & 0x1FFFFFFF;
    uint32_t end_phys = list_end & 0x1FFFFFFF;
    if (list_base == 0 || list_end == 0 ||
        phys < first_phys || phys >= end_phys ||
        (phys - first_phys) % 0x74 != 0) {
        return -1;
    }
    return (int)((phys - first_phys) / 0x74);
}

static LodMap76LifecycleObject lod_map76_lifecycle_capture(uint8_t* rdram, uint32_t obj_addr) {
    LodMap76LifecycleObject out = {};
    out.addr = obj_addr;

    uint32_t phys = 0;
    if (!lod_decode_rdram_phys_addr(obj_addr, 0x74, &phys) ||
        !lod_rdram_range_ok(phys, 0x74)) {
        return out;
    }

    out.ok = true;
    out.phys = phys;
    out.slot = lod_map76_lifecycle_slot_for_phys(rdram, phys);
    out.raw = (uint16_t)lod_rdram_s16(rdram, phys + 0x00);
    out.id = out.raw & 0x07FF;
    out.flags = (uint16_t)lod_rdram_s16(rdram, phys + 0x02);
    for (uint32_t i = 0; i < 6; i++) {
        out.sched[i] = lod_rdram_u8(rdram, phys + 0x08 + i);
    }
    out.func = lod_rdram_s16(rdram, phys + 0x0E);
    out.disp = lod_object_dispatch_state(rdram, phys);
    out.destroy = lod_rdram_u32(rdram, phys + 0x10);
    out.parent = lod_rdram_u32(rdram, phys + 0x14);
    out.child = lod_rdram_u32(rdram, phys + 0x1C);
    out.alloc = (uint16_t)lod_rdram_s16(rdram, phys + 0x20);
    out.gfx = (uint16_t)lod_rdram_s16(rdram, phys + 0x22);
    for (uint32_t i = 0; i < 4; i++) {
        out.figs[i] = lod_rdram_u32(rdram, phys + 0x24 + i * 4);
        out.data[i] = lod_rdram_u32(rdram, phys + 0x34 + i * 4);
    }
    out.sig = lod_map76_object_signature(rdram, phys);

    uint32_t fig0_phys = 0;
    if (out.figs[0] != 0 && lod_decode_rdram_phys_addr(out.figs[0], 0x6C, &fig0_phys)) {
        out.fig0_type = (uint16_t)lod_rdram_s16(rdram, fig0_phys + 0x00);
        out.fig0_flags = (uint16_t)lod_rdram_s16(rdram, fig0_phys + 0x02);
        out.fig0_dl = lod_rdram_u32(rdram, fig0_phys + 0x3C);
    }
    return out;
}

static bool lod_map76_lifecycle_interesting(const LodMap76LifecycleObject& obj) {
    return obj.ok && lod_map76_is_boss_trace_obj(obj.id);
}

static bool lod_map76_lifecycle_changed(const LodMap76LifecycleObject& a,
                                        const LodMap76LifecycleObject& b) {
    if (a.ok != b.ok || a.raw != b.raw || a.flags != b.flags ||
        a.destroy != b.destroy || a.parent != b.parent || a.child != b.child ||
        a.alloc != b.alloc || a.gfx != b.gfx || a.sig != b.sig) {
        return true;
    }
    for (int i = 0; i < 6; i++) {
        if (a.sched[i] != b.sched[i]) {
            return true;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (a.figs[i] != b.figs[i] || a.data[i] != b.data[i]) {
            return true;
        }
    }
    return false;
}

static bool lod_map76_lifecycle_bad_retire(const LodMap76LifecycleObject& obj) {
    return obj.ok && obj.id == 0x0EF &&
           (((int16_t)obj.raw < 0) ||
            (obj.flags & 0x8000) != 0 ||
            (obj.figs[0] == 0 && obj.figs[1] == 0 &&
             obj.figs[2] == 0 && obj.figs[3] == 0) ||
            obj.child == 0);
}

static void lod_map76_lifecycle_log_transition(uint8_t* rdram,
                                               const char* hook, uint32_t call,
                                               const LodMap76LifecycleObject& before,
                                               const LodMap76LifecycleObject& after) {
    const bool interesting = lod_map76_lifecycle_interesting(before) ||
                             lod_map76_lifecycle_interesting(after);
    if (!interesting) {
        return;
    }

    const bool changed = lod_map76_lifecycle_changed(before, after);
    const bool bad = lod_map76_lifecycle_bad_retire(before) ||
                     lod_map76_lifecycle_bad_retire(after);
    static uint32_t printed = 0;
    if (!changed && !bad && call > 12) {
        return;
    }
    if (printed >= 260 && !changed) {
        return;
    }
    printed++;

    fprintf(stderr,
            "[MAP76_LIFE] %s#%u changed=%d bad=%d gs=%d map#%d map_rom=0x%08X "
            "before={ok=%d slot=%d addr=0x%08X id=0x%03X raw=0x%04X flags=0x%04X "
            "sched=%02X/%02X %02X/%02X %02X/%02X func=%d disp=%u destroy=0x%08X "
            "parent=0x%08X child=0x%08X bits=%04X/%04X "
            "fig={0x%08X,0x%08X,0x%08X,0x%08X} data={0x%08X,0x%08X,0x%08X,0x%08X} "
            "fig0={type=0x%04X flags=0x%04X dl=0x%08X} sig=0x%08X} "
            "after={ok=%d slot=%d addr=0x%08X id=0x%03X raw=0x%04X flags=0x%04X "
            "sched=%02X/%02X %02X/%02X %02X/%02X func=%d disp=%u destroy=0x%08X "
            "parent=0x%08X child=0x%08X bits=%04X/%04X "
            "fig={0x%08X,0x%08X,0x%08X,0x%08X} data={0x%08X,0x%08X,0x%08X,0x%08X} "
            "fig0={type=0x%04X flags=0x%04X dl=0x%08X} sig=0x%08X}\n",
            hook, call, changed ? 1 : 0, bad ? 1 : 0,
            lod_current_gamestate(rdram), lod_map_ovl_load_count, lod_current_map_ovl_rom,
            before.ok ? 1 : 0, before.slot, before.addr, before.id, before.raw, before.flags,
            before.sched[0], before.sched[1], before.sched[2],
            before.sched[3], before.sched[4], before.sched[5],
            before.func, before.disp, before.destroy,
            before.parent, before.child, before.alloc, before.gfx,
            before.figs[0], before.figs[1], before.figs[2], before.figs[3],
            before.data[0], before.data[1], before.data[2], before.data[3],
            before.fig0_type, before.fig0_flags, before.fig0_dl, before.sig,
            after.ok ? 1 : 0, after.slot, after.addr, after.id, after.raw, after.flags,
            after.sched[0], after.sched[1], after.sched[2],
            after.sched[3], after.sched[4], after.sched[5],
            after.func, after.disp, after.destroy,
            after.parent, after.child, after.alloc, after.gfx,
            after.figs[0], after.figs[1], after.figs[2], after.figs[3],
            after.data[0], after.data[1], after.data[2], after.data[3],
            after.fig0_type, after.fig0_flags, after.fig0_dl, after.sig);
}

struct LodMap76Ni44StateSnapshot {
    bool ok = false;
    bool trace = false;
    uint32_t obj_addr = 0;
    uint32_t obj_phys = 0;
    uint16_t raw = 0;
    uint16_t id = 0;
    uint16_t flags = 0;
    uint8_t sched[8] = {};
    int16_t cur_level = 0;
    uint32_t fig0 = 0;
    uint32_t data0 = 0;
    uint16_t fig0_type = 0;
    uint16_t fig0_flags = 0;
    uint16_t fig0_anim = 0;
    uint32_t sub24 = 0;
    uint32_t sub28 = 0;
    uint32_t sub2c = 0;
    uint32_t sub30 = 0;
    uint32_t sub44 = 0;
    uint32_t sub48 = 0;
    uint32_t sub4c = 0;
    uint32_t sub50 = 0;
    uint16_t sub54 = 0;
    uint16_t sub56 = 0;
    uint32_t sub58 = 0;
    uint32_t sub68 = 0;
    uint32_t sub6c = 0;
    uint16_t sub78 = 0;
    uint16_t sub7a = 0;
    uint32_t sub90 = 0;
};

static LodMap76Ni44StateSnapshot lod_map76_ni44_state_capture(uint8_t* rdram,
                                                              uint32_t obj_addr) {
    LodMap76Ni44StateSnapshot out = {};
    out.obj_addr = obj_addr;

    uint32_t obj_phys = 0;
    if (!lod_decode_rdram_phys_addr(obj_addr, 0x74, &obj_phys) ||
        !lod_rdram_range_ok(obj_phys, 0x74)) {
        return out;
    }

    out.ok = true;
    out.obj_phys = obj_phys;
    out.raw = (uint16_t)lod_rdram_s16(rdram, obj_phys + 0x00);
    out.id = out.raw & 0x07FF;
    out.flags = (uint16_t)lod_rdram_s16(rdram, obj_phys + 0x02);
    out.cur_level = lod_rdram_s16(rdram, obj_phys + 0x0E);
    for (uint32_t i = 0; i < 8; i++) {
        out.sched[i] = lod_rdram_u8(rdram, obj_phys + 0x08 + i);
    }
    out.fig0 = lod_rdram_u32(rdram, obj_phys + 0x24);
    out.data0 = lod_rdram_u32(rdram, obj_phys + 0x34);
    out.trace = lod_current_gamestate(rdram) == 3 &&
                lod_current_map_ovl_rom == LOD_B2_TRACE_MAP_ROM &&
                out.id == 0x0EF;

    uint32_t fig_phys = 0;
    if (out.fig0 != 0 &&
        lod_decode_rdram_phys_addr(out.fig0, 0x64, &fig_phys) &&
        lod_rdram_range_ok(fig_phys, 0x64)) {
        out.fig0_type = (uint16_t)lod_rdram_s16(rdram, fig_phys + 0x00);
        out.fig0_flags = (uint16_t)lod_rdram_s16(rdram, fig_phys + 0x02);
        out.fig0_anim = (uint16_t)lod_rdram_s16(rdram, fig_phys + 0x5E);
    }

    uint32_t data_phys = 0;
    if (out.data0 != 0 &&
        lod_decode_rdram_phys_addr(out.data0, 0x170, &data_phys) &&
        lod_rdram_range_ok(data_phys + 0xDC, 0x94)) {
        uint32_t sub = data_phys + 0xDC;
        out.sub24 = lod_rdram_u32(rdram, sub + 0x24);
        out.sub28 = lod_rdram_u32(rdram, sub + 0x28);
        out.sub2c = lod_rdram_u32(rdram, sub + 0x2C);
        out.sub30 = lod_rdram_u32(rdram, sub + 0x30);
        out.sub44 = lod_rdram_u32(rdram, sub + 0x44);
        out.sub48 = lod_rdram_u32(rdram, sub + 0x48);
        out.sub4c = lod_rdram_u32(rdram, sub + 0x4C);
        out.sub50 = lod_rdram_u32(rdram, sub + 0x50);
        out.sub54 = (uint16_t)lod_rdram_s16(rdram, sub + 0x54);
        out.sub56 = (uint16_t)lod_rdram_s16(rdram, sub + 0x56);
        out.sub58 = lod_rdram_u32(rdram, sub + 0x58);
        out.sub68 = lod_rdram_u32(rdram, sub + 0x68);
        out.sub6c = lod_rdram_u32(rdram, sub + 0x6C);
        out.sub78 = (uint16_t)lod_rdram_s16(rdram, sub + 0x78);
        out.sub7a = (uint16_t)lod_rdram_s16(rdram, sub + 0x7A);
        out.sub90 = lod_rdram_u32(rdram, sub + 0x90);
    }

    return out;
}

static bool lod_map76_ni44_state_changed(const LodMap76Ni44StateSnapshot& a,
                                         const LodMap76Ni44StateSnapshot& b) {
    if (a.ok != b.ok || a.raw != b.raw || a.flags != b.flags ||
        a.cur_level != b.cur_level || a.fig0 != b.fig0 || a.data0 != b.data0 ||
        a.fig0_type != b.fig0_type || a.fig0_flags != b.fig0_flags ||
        a.fig0_anim != b.fig0_anim || a.sub24 != b.sub24 ||
        a.sub44 != b.sub44 || a.sub48 != b.sub48 ||
        a.sub50 != b.sub50 || a.sub54 != b.sub54 ||
        a.sub56 != b.sub56 || a.sub58 != b.sub58 ||
        a.sub68 != b.sub68 || a.sub6c != b.sub6c ||
        a.sub78 != b.sub78 || a.sub7a != b.sub7a) {
        return true;
    }
    // Ignore timer-byte churn for the primary filter, but keep state-byte
    // changes. Timers are still printed when a line is selected.
    for (uint32_t i = 1; i < 8; i += 2) {
        if (a.sched[i] != b.sched[i]) {
            return true;
        }
    }
    return false;
}

static bool lod_map76_ni44_state_near_handoff(const LodMap76Ni44StateSnapshot& s) {
    if (!s.trace) {
        return false;
    }
    const uint8_t root_state = s.sched[1];
    const uint8_t sub_state = s.sched[3];
    return (root_state >= 3 && sub_state >= 2) || s.sub56 != 0;
}

static void lod_log_map76_ni44_state_transition(
    uint8_t* rdram, const char* hook, uint32_t call,
    const LodMap76Ni44StateSnapshot& before,
    const LodMap76Ni44StateSnapshot& after) {
    static uint32_t printed_count = 0;

    if (!before.trace && !after.trace) {
        return;
    }

    const bool changed = lod_map76_ni44_state_changed(before, after);
    const bool near_handoff = lod_map76_ni44_state_near_handoff(before) ||
                              lod_map76_ni44_state_near_handoff(after);
    if (!changed && (!near_handoff || (call % 4) != 0)) {
        return;
    }
    // Keep this trace usable during manual boss-area testing by capping the
    // verbose state snapshots.
    if (printed_count >= 120) {
        return;
    }
    printed_count++;

    fprintf(stderr,
            "[MAP76_NI44] %s#%u changed=%d near=%d gs=%d map#%d map_rom=0x%08X "
            "obj=0x%08X raw=0x%04X->0x%04X flags=0x%04X->0x%04X cur=%d->%d "
            "sched=%02X/%02X %02X/%02X %02X/%02X %02X/%02X -> "
                  "%02X/%02X %02X/%02X %02X/%02X %02X/%02X "
            "fig0=0x%08X->0x%08X fig={type=0x%04X->0x%04X flags=0x%04X->0x%04X anim5e=0x%04X->0x%04X} "
            "data0=0x%08X->0x%08X "
            "sub24=0x%08X->0x%08X sub28=0x%08X->0x%08X "
            "sub2c=0x%08X->0x%08X sub30=0x%08X->0x%08X "
            "sub44=0x%08X->0x%08X sub48=0x%08X->0x%08X "
            "sub4c=0x%08X->0x%08X sub50=0x%08X->0x%08X "
            "sub54=0x%04X->0x%04X sub56=0x%04X->0x%04X "
            "sub58=0x%08X->0x%08X sub68=0x%08X->0x%08X "
            "sub6c=0x%08X->0x%08X sub78=0x%04X->0x%04X "
            "sub7a=0x%04X->0x%04X sub90=0x%08X->0x%08X\n",
            hook, call, changed ? 1 : 0, near_handoff ? 1 : 0,
            lod_current_gamestate(rdram), lod_map_ovl_load_count, lod_current_map_ovl_rom,
            before.obj_addr, before.raw, after.raw, before.flags, after.flags,
            before.cur_level, after.cur_level,
            before.sched[0], before.sched[1], before.sched[2], before.sched[3],
            before.sched[4], before.sched[5], before.sched[6], before.sched[7],
            after.sched[0], after.sched[1], after.sched[2], after.sched[3],
            after.sched[4], after.sched[5], after.sched[6], after.sched[7],
            before.fig0, after.fig0,
            before.fig0_type, after.fig0_type, before.fig0_flags, after.fig0_flags,
            before.fig0_anim, after.fig0_anim,
            before.data0, after.data0,
            before.sub24, after.sub24, before.sub28, after.sub28,
            before.sub2c, after.sub2c, before.sub30, after.sub30,
            before.sub44, after.sub44, before.sub48, after.sub48,
            before.sub4c, after.sub4c, before.sub50, after.sub50,
            before.sub54, after.sub54, before.sub56, after.sub56,
            before.sub58, after.sub58, before.sub68, after.sub68,
            before.sub6c, after.sub6c, before.sub78, after.sub78,
            before.sub7a, after.sub7a, before.sub90, after.sub90);
}

static void lod_map76_ni44_trace_call(uint8_t* rdram, recomp_context* ctx,
                                      const char* hook, uint32_t* call_counter,
                                      recomp_func_t* original) {
    (*call_counter)++;
    const uint32_t call = *call_counter;
    const uint32_t obj_addr = (uint32_t)ctx->r4;
    const LodMap76Ni44StateSnapshot before =
        lod_map76_ni44_state_capture(rdram, obj_addr);

    if (original != nullptr) {
        original(rdram, ctx);
    }

    const LodMap76Ni44StateSnapshot after =
        lod_map76_ni44_state_capture(rdram, obj_addr);
    lod_log_map76_ni44_state_transition(rdram, hook, call, before, after);
}

static void lod_trace_map76_obj0ef_update_8002C518(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    call++;

    const uint32_t obj_addr = (uint32_t)ctx->r4;
    const LodMap76LifecycleObject before = lod_map76_lifecycle_capture(rdram, obj_addr);

    // Reimplement the tiny original wrapper so we can place probes between its
    // two direct calls without touching generated code:
    //   8002C518: func_8002D548(a0); func_8002D1F4(a0);
    ctx->r29 = ADD32(ctx->r29, -0x18);
    MEM_W(0x14, ctx->r29) = ctx->r31;
    MEM_W(0x18, ctx->r29) = ctx->r4;

    func_8002D548(rdram, ctx);
    const uint32_t saved_obj = (uint32_t)MEM_W(0x18, ctx->r29);
    const LodMap76LifecycleObject after_d548 =
        lod_map76_lifecycle_capture(rdram, saved_obj);
    lod_map76_lifecycle_log_transition(rdram, "8002C518.after_8002D548",
                                       call, before, after_d548);

    ctx->r4 = MEM_W(0x18, ctx->r29);
    func_8002D1F4(rdram, ctx);
    const LodMap76LifecycleObject after_d1f4 =
        lod_map76_lifecycle_capture(rdram, saved_obj);
    lod_map76_lifecycle_log_transition(rdram, "8002C518.after_8002D1F4",
                                       call, after_d548, after_d1f4);

    ctx->r31 = MEM_W(0x14, ctx->r29);
    ctx->r29 = ADD32(ctx->r29, 0x18);
}

static recomp_func_t* lod_orig_map76_default_destroy_80002CE0 = nullptr;

static void lod_trace_map76_default_destroy_80002CE0(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    call++;
    const uint32_t obj_addr = (uint32_t)ctx->r4;
    const LodMap76LifecycleObject before = lod_map76_lifecycle_capture(rdram, obj_addr);

    if (lod_orig_map76_default_destroy_80002CE0 != nullptr) {
        lod_orig_map76_default_destroy_80002CE0(rdram, ctx);
    } else {
        object_destroyChildrenAndModelInfo(rdram, ctx);
    }

    const LodMap76LifecycleObject after = lod_map76_lifecycle_capture(rdram, obj_addr);
    lod_map76_lifecycle_log_transition(rdram, "80002CE0.default_destroy",
                                       call, before, after);
}

static recomp_func_t* lod_orig_map76_ni44_destroy_0F003FCC = nullptr;
static recomp_func_t* lod_orig_map76_ni44_dispatch_0F000D78 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_sub0_0F000EF0 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_sub1_0F001034 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_sub2_0F0011A8 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_sub3_0F0013B4 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_sub4_0F0014A0 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_dispatch_0F001970 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub0_0F001BF8 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub1_0F001CFC = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub2_dispatch_0F001F20 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub2_0F001FA0 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub2_0F002010 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub2_0F002300 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub2_0F002380 = nullptr;
static recomp_func_t* lod_orig_map76_ni44_root4_sub2_0F002488 = nullptr;

static void lod_trace_map76_ni44_destroy_0F003FCC(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    call++;

    const uint32_t obj_addr = (uint32_t)ctx->r4;
    const LodMap76LifecycleObject before = lod_map76_lifecycle_capture(rdram, obj_addr);

    if (lod_orig_map76_ni44_destroy_0F003FCC != nullptr) {
        lod_orig_map76_ni44_destroy_0F003FCC(rdram, ctx);
    }

    const LodMap76LifecycleObject after = lod_map76_lifecycle_capture(rdram, obj_addr);
    lod_map76_lifecycle_log_transition(rdram, "0F003FCC.ni44_destroy",
                                       call, before, after);
}

static void lod_trace_map76_ni44_dispatch_0F000D78(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F000D78.dispatch", &call,
                              lod_orig_map76_ni44_dispatch_0F000D78);
}

static void lod_trace_map76_ni44_sub0_0F000EF0(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F000EF0.sub0", &call,
                              lod_orig_map76_ni44_sub0_0F000EF0);
}

static void lod_trace_map76_ni44_sub1_0F001034(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F001034.sub1", &call,
                              lod_orig_map76_ni44_sub1_0F001034);
}

static void lod_trace_map76_ni44_sub2_0F0011A8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F0011A8.sub2", &call,
                              lod_orig_map76_ni44_sub2_0F0011A8);
}

static void lod_trace_map76_ni44_sub3_0F0013B4(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F0013B4.sub3", &call,
                              lod_orig_map76_ni44_sub3_0F0013B4);
}

static void lod_trace_map76_ni44_sub4_0F0014A0(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F0014A0.sub4", &call,
                              lod_orig_map76_ni44_sub4_0F0014A0);
}

static void lod_trace_map76_ni44_root4_dispatch_0F001970(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F001970.root4", &call,
                              lod_orig_map76_ni44_root4_dispatch_0F001970);
}

static void lod_trace_map76_ni44_root4_sub0_0F001BF8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F001BF8.root4_sub0", &call,
                              lod_orig_map76_ni44_root4_sub0_0F001BF8);
}

static void lod_trace_map76_ni44_root4_sub1_0F001CFC(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F001CFC.root4_sub1", &call,
                              lod_orig_map76_ni44_root4_sub1_0F001CFC);
}

static void lod_trace_map76_ni44_root4_sub2_dispatch_0F001F20(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F001F20.root4_sub2_dispatch", &call,
                              lod_orig_map76_ni44_root4_sub2_dispatch_0F001F20);
}

static void lod_trace_map76_ni44_root4_sub2_0F001FA0(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F001FA0.root4_sub2_0", &call,
                              lod_orig_map76_ni44_root4_sub2_0F001FA0);
}

static void lod_trace_map76_ni44_root4_sub2_0F002010(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F002010.root4_sub2_0", &call,
                              lod_orig_map76_ni44_root4_sub2_0F002010);
}

static void lod_trace_map76_ni44_root4_sub2_0F002300(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F002300.root4_sub2_1", &call,
                              lod_orig_map76_ni44_root4_sub2_0F002300);
}

static void lod_trace_map76_ni44_root4_sub2_0F002380(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F002380.root4_sub2_2", &call,
                              lod_orig_map76_ni44_root4_sub2_0F002380);
}

static void lod_trace_map76_ni44_root4_sub2_0F002488(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t call = 0;
    lod_map76_ni44_trace_call(rdram, ctx, "0F002488.root4_sub2_3", &call,
                              lod_orig_map76_ni44_root4_sub2_0F002488);
}

static void lod_install_map76_ni44_state_trace_wrapper(uint32_t vram,
                                                       recomp_func_t* wrapper,
                                                       recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
        recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
    }
}

extern "C" void lod_install_map76_boss_ni44_destroy_trace_wrapper(const char* reason) {
    recomp_func_t* current = get_function((int32_t)0x0F003FCC);
    if (current != lod_trace_map76_ni44_destroy_0F003FCC) {
        lod_orig_map76_ni44_destroy_0F003FCC = current;
        recomp::overlays::add_loaded_function((int32_t)0x0F003FCC,
                                              lod_trace_map76_ni44_destroy_0F003FCC);
    }

    lod_install_map76_ni44_state_trace_wrapper(
        0x0F000D78, lod_trace_map76_ni44_dispatch_0F000D78,
        &lod_orig_map76_ni44_dispatch_0F000D78);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F000EF0, lod_trace_map76_ni44_sub0_0F000EF0,
        &lod_orig_map76_ni44_sub0_0F000EF0);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F001034, lod_trace_map76_ni44_sub1_0F001034,
        &lod_orig_map76_ni44_sub1_0F001034);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F0011A8, lod_trace_map76_ni44_sub2_0F0011A8,
        &lod_orig_map76_ni44_sub2_0F0011A8);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F0013B4, lod_trace_map76_ni44_sub3_0F0013B4,
        &lod_orig_map76_ni44_sub3_0F0013B4);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F0014A0, lod_trace_map76_ni44_sub4_0F0014A0,
        &lod_orig_map76_ni44_sub4_0F0014A0);

    lod_install_map76_ni44_state_trace_wrapper(
        0x0F001970, lod_trace_map76_ni44_root4_dispatch_0F001970,
        &lod_orig_map76_ni44_root4_dispatch_0F001970);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F001BF8, lod_trace_map76_ni44_root4_sub0_0F001BF8,
        &lod_orig_map76_ni44_root4_sub0_0F001BF8);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F001CFC, lod_trace_map76_ni44_root4_sub1_0F001CFC,
        &lod_orig_map76_ni44_root4_sub1_0F001CFC);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F001F20, lod_trace_map76_ni44_root4_sub2_dispatch_0F001F20,
        &lod_orig_map76_ni44_root4_sub2_dispatch_0F001F20);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F001FA0, lod_trace_map76_ni44_root4_sub2_0F001FA0,
        &lod_orig_map76_ni44_root4_sub2_0F001FA0);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F002010, lod_trace_map76_ni44_root4_sub2_0F002010,
        &lod_orig_map76_ni44_root4_sub2_0F002010);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F002300, lod_trace_map76_ni44_root4_sub2_0F002300,
        &lod_orig_map76_ni44_root4_sub2_0F002300);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F002380, lod_trace_map76_ni44_root4_sub2_0F002380,
        &lod_orig_map76_ni44_root4_sub2_0F002380);
    lod_install_map76_ni44_state_trace_wrapper(
        0x0F002488, lod_trace_map76_ni44_root4_sub2_0F002488,
        &lod_orig_map76_ni44_root4_sub2_0F002488);

    static int install_count = 0;
    install_count++;
    if (install_count <= 8) {
        fprintf(stderr,
                "[MAP76_LIFE] installed ni44 destroy/state wrappers #%d reason=%s "
                "destroy=%p dispatch=%p sub0=%p sub1=%p sub2=%p sub3=%p sub4=%p\n",
                install_count, reason ? reason : "unknown",
                (void*)lod_orig_map76_ni44_destroy_0F003FCC,
                (void*)lod_orig_map76_ni44_dispatch_0F000D78,
                (void*)lod_orig_map76_ni44_sub0_0F000EF0,
                (void*)lod_orig_map76_ni44_sub1_0F001034,
                (void*)lod_orig_map76_ni44_sub2_0F0011A8,
                (void*)lod_orig_map76_ni44_sub3_0F0013B4,
                (void*)lod_orig_map76_ni44_sub4_0F0014A0);
    }
}

static void lod_install_map76_boss_lifecycle_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    // The werewolf object is being retired through a schedule/state transition
    // before its NI44 destroy callback is reached. Reuse the existing object
    // schedule-writer trace, but filter it to Map76 boss-family objects.
    lod_install_ni24_writer_trace_wrappers_early();

    recomp_func_t* update_current = get_function((int32_t)0x8002C518);
    if (update_current != lod_trace_map76_obj0ef_update_8002C518) {
        recomp::overlays::add_loaded_function((int32_t)0x8002C518,
                                              lod_trace_map76_obj0ef_update_8002C518);
    }

    recomp_func_t* destroy_current = get_function((int32_t)0x80002CE0);
    if (destroy_current != lod_trace_map76_default_destroy_80002CE0) {
        lod_orig_map76_default_destroy_80002CE0 = destroy_current;
        recomp::overlays::add_loaded_function((int32_t)0x80002CE0,
                                              lod_trace_map76_default_destroy_80002CE0);
    }

    if (install_count <= 4) {
        fprintf(stderr,
                "[MAP76_LIFE] installed lifecycle wrappers #%d reason=%s "
                "update_orig=%p default_destroy_orig=%p\n",
                install_count, reason ? reason : "unknown",
                (void*)update_current,
                (void*)lod_orig_map76_default_destroy_80002CE0);
    }
}

#endif

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

#if LOD_FIX_NULL_OBJECT_DISPATCH_CHILD
static recomp_func_t* lod_orig_null_guard_object_dispatch_child = nullptr; // 0x8000516C

static void lod_null_guard_object_dispatch_child(uint8_t* rdram, recomp_context* ctx) {
    if ((uint32_t)ctx->r4 == 0) {
        static uint32_t skip_count = 0;
        skip_count++;
        if (skip_count <= 5) {
            fprintf(stderr,
                    "[OBJECT_FIX] object_dispatchChild(NULL,0x%08X) no-op #%u "
                    "gs=%d map#%d map_rom=0x%08X\n",
                    (uint32_t)ctx->r5, skip_count, lod_current_gamestate(rdram),
                    lod_map_ovl_load_count, lod_current_map_ovl_rom);
            fflush(stderr);
        }
        return;
    }

    if (lod_orig_null_guard_object_dispatch_child != nullptr) {
        lod_orig_null_guard_object_dispatch_child(rdram, ctx);
    }
}

extern "C" void lod_install_null_object_dispatch_child_guard_early() {
    constexpr uint32_t OBJECT_DISPATCH_CHILD_VRAM = 0x8000516C;
    recomp_func_t* current = get_function((int32_t)OBJECT_DISPATCH_CHILD_VRAM);
    if (current != lod_null_guard_object_dispatch_child) {
        lod_orig_null_guard_object_dispatch_child = current;
    }
    recomp::overlays::add_loaded_function((int32_t)OBJECT_DISPATCH_CHILD_VRAM,
                                          lod_null_guard_object_dispatch_child);
    fprintf(stderr,
            "[OBJECT_FIX] installed object_dispatchChild(NULL) guard original=%p\n",
            (void*)lod_orig_null_guard_object_dispatch_child);
}
#endif

#if LOD_ENABLE_ITEM_KSEG0_TRACE
extern "C" int lod_ni_overlay_loaded_0f_pair();
extern "C" int lod_ni_overlay_loaded_0e_pair();

static recomp_func_t* lod_orig_item_object_dispatch_child = nullptr; // 0x8000516C
static recomp_func_t* lod_orig_item_func_8007D9F0 = nullptr;
static recomp_func_t* lod_orig_item_func_8007E8F4 = nullptr;
static recomp_func_t* lod_orig_item_func_80085280 = nullptr;
static recomp_func_t* lod_orig_item_func_800856B8 = nullptr;
static recomp_func_t* lod_orig_item_func_80086120 = nullptr;
static recomp_func_t* lod_orig_item_func_80086528 = nullptr;
static recomp_func_t* lod_orig_item_func_8008AA90 = nullptr;
static recomp_func_t* lod_orig_item_func_8008B3A4 = nullptr;
static recomp_func_t* lod_orig_item_func_8008F590 = nullptr;

struct LodItemTraceInterest {
    bool segmented = false; // normal segmented display-list/model data, e.g. 0x06xxxxxx
    bool bad = false;       // low segment / post-RDRAM KSEG mirror candidate, e.g. 0x08/0x88xxxxxx
};

static bool lod_item_trace_is_segmented_value(uint32_t value) {
    const uint32_t high = value & 0xFF000000u;
    return high == 0x06000000u || high == 0x08000000u || high == 0x86000000u ||
           high == 0x88000000u;
}

static bool lod_item_trace_is_bad_value(uint32_t value) {
    const uint32_t high = value & 0xFF000000u;
    return high == 0x08000000u || high == 0x88000000u ||
           (value >= 0x80800000u && value < 0x81000000u);
}

static void lod_item_trace_consider(LodItemTraceInterest& interest, uint32_t value) {
    if (lod_item_trace_is_segmented_value(value)) {
        interest.segmented = true;
    }
    if (lod_item_trace_is_bad_value(value)) {
        interest.bad = true;
    }
}

static uint32_t lod_item_trace_u32_at(uint8_t* rdram, bool base_ok,
                                      uint32_t base_phys, uint32_t off) {
    return base_ok && lod_rdram_range_ok(base_phys + off, 4)
        ? lod_rdram_u32(rdram, base_phys + off)
        : 0;
}

static uint8_t lod_item_trace_u8_at(uint8_t* rdram, bool base_ok,
                                    uint32_t base_phys, uint32_t off) {
    return base_ok && lod_rdram_range_ok(base_phys + off, 1)
        ? lod_rdram_u8(rdram, base_phys + off)
        : 0;
}

static int16_t lod_item_trace_s16_at(uint8_t* rdram, bool base_ok,
                                     uint32_t base_phys, uint32_t off) {
    return base_ok && lod_rdram_range_ok(base_phys + off, 2)
        ? lod_rdram_s16(rdram, base_phys + off)
        : 0;
}

static void lod_item_trace_scan_block(uint8_t* rdram, LodItemTraceInterest& interest,
                                      uint32_t addr, uint32_t bytes) {
    uint32_t phys = 0;
    if (!lod_decode_rdram_phys_addr(addr, bytes, &phys)) {
        lod_item_trace_consider(interest, addr);
        return;
    }
    for (uint32_t off = 0; off + 4 <= bytes; off += 4) {
        lod_item_trace_consider(interest, lod_rdram_u32(rdram, phys + off));
    }
}

static LodItemTraceInterest lod_item_trace_collect(uint8_t* rdram,
                                                   const recomp_context* ctx,
                                                   uint32_t obj_addr) {
    LodItemTraceInterest interest;
    lod_item_trace_consider(interest, (uint32_t)ctx->r2);
    lod_item_trace_consider(interest, (uint32_t)ctx->r3);
    lod_item_trace_consider(interest, (uint32_t)ctx->r4);
    lod_item_trace_consider(interest, (uint32_t)ctx->r5);
    lod_item_trace_consider(interest, (uint32_t)ctx->r6);
    lod_item_trace_consider(interest, (uint32_t)ctx->r7);
    lod_item_trace_consider(interest, (uint32_t)ctx->r31);

    uint32_t obj_phys = 0;
    const bool obj_ok = lod_decode_rdram_phys_addr(obj_addr, 0x74, &obj_phys);
    if (!obj_ok) {
        lod_item_trace_consider(interest, obj_addr);
        return interest;
    }

    constexpr uint32_t object_fields[] = {
        0x10, 0x14, 0x1C, 0x24, 0x30, 0x34, 0x38, 0x3C,
        0x40, 0x58, 0x5C, 0x60, 0x64, 0x6C, 0x70,
    };
    for (uint32_t off : object_fields) {
        uint32_t value = lod_rdram_u32(rdram, obj_phys + off);
        lod_item_trace_consider(interest, value);
    }

    // Follow the most likely model/work pointers one level deep.  The item
    // crash looks like a segmented display-list pointer (0x08xxxxxx) escaping
    // into a CPU dereference, and model records commonly keep that value near
    // +0x3C/+0x40.
    constexpr uint32_t pointer_fields[] = {0x24, 0x30, 0x34, 0x38, 0x58, 0x5C, 0x60, 0x70};
    for (uint32_t off : pointer_fields) {
        uint32_t ptr = lod_rdram_u32(rdram, obj_phys + off);
        if (ptr != 0) {
            lod_item_trace_scan_block(rdram, interest, ptr, 0x80);
        }
    }

    return interest;
}

static bool lod_item_trace_nested_has_segment(uint8_t* rdram, uint32_t ptr_addr) {
    uint32_t ptr_phys = 0;
    if (!lod_decode_rdram_phys_addr(ptr_addr, 0x80, &ptr_phys)) {
        return lod_item_trace_is_segmented_value(ptr_addr);
    }
    for (uint32_t off = 0; off + 4 <= 0x80; off += 4) {
        uint32_t value = lod_rdram_u32(rdram, ptr_phys + off);
        if (lod_item_trace_is_segmented_value(value) || lod_item_trace_is_bad_value(value)) {
            return true;
        }
    }
    return false;
}

static void lod_item_trace_print_nested(uint8_t* rdram, const char* phase, const char* label,
                                        uint32_t count, const char* ptr_label,
                                        uint32_t ptr_addr) {
    uint32_t ptr_phys = 0;
    if (!lod_decode_rdram_phys_addr(ptr_addr, 0x80, &ptr_phys)) {
        if (lod_item_trace_is_segmented_value(ptr_addr)) {
            fprintf(stderr,
                    "[ITEM_KSEG0] %s #%u %s %s=0x%08X non-rdram-segptr\n",
                    phase, count, label, ptr_label, ptr_addr);
        }
        return;
    }

    const uint32_t f00 = lod_rdram_u32(rdram, ptr_phys + 0x00);
    const uint32_t f18 = lod_rdram_u32(rdram, ptr_phys + 0x18);
    const uint32_t f20 = lod_rdram_u32(rdram, ptr_phys + 0x20);
    const uint32_t f24 = lod_rdram_u32(rdram, ptr_phys + 0x24);
    const uint32_t f38 = lod_rdram_u32(rdram, ptr_phys + 0x38);
    const uint32_t f3c = lod_rdram_u32(rdram, ptr_phys + 0x3C);
    const uint32_t f40 = lod_rdram_u32(rdram, ptr_phys + 0x40);
    const uint32_t f58 = lod_rdram_u32(rdram, ptr_phys + 0x58);
    const uint32_t f5c = lod_rdram_u32(rdram, ptr_phys + 0x5C);
    const uint32_t f60 = lod_rdram_u32(rdram, ptr_phys + 0x60);
    const uint32_t f6c = lod_rdram_u32(rdram, ptr_phys + 0x6C);
    const uint32_t f70 = lod_rdram_u32(rdram, ptr_phys + 0x70);

    fprintf(stderr,
            "[ITEM_KSEG0] %s #%u %s %s=0x%08X phys=0x%06X "
            "{00=0x%08X 18=0x%08X 20=0x%08X 24=0x%08X "
            "38=0x%08X 3C=0x%08X 40=0x%08X 58=0x%08X "
            "5C=0x%08X 60=0x%08X 6C=0x%08X 70=0x%08X}\n",
            phase, count, label, ptr_label, ptr_addr, ptr_phys,
            f00, f18, f20, f24, f38, f3c, f40, f58, f5c, f60, f6c, f70);
}

static void lod_item_child_trace_print_obj(uint8_t* rdram, const char* tag, uint32_t addr) {
    uint32_t phys = 0;
    if (!lod_decode_rdram_phys_addr(addr, 0x74, &phys)) {
        fprintf(stderr, "[ITEM_CHILD]   %s=0x%08X ok=0 bad=%d\n",
                tag, addr, lod_item_trace_is_bad_value(addr) ? 1 : 0);
        return;
    }

    fprintf(stderr,
            "[ITEM_CHILD]   %s=0x%08X phys=0x%06X "
            "{flags=0x%04X step=%u state=%u level=%d "
            "08=0x%08X 10=0x%08X 14=0x%08X 18=0x%08X "
            "1C=0x%08X 24=0x%08X 30=0x%08X 34=0x%08X "
            "38=0x%08X 3C=0x%08X 40=0x%08X 5C=0x%08X 60=0x%08X}\n",
            tag, addr, phys,
            lod_item_trace_u32_at(rdram, true, phys, 0x00) >> 16,
            lod_item_trace_u8_at(rdram, true, phys, 0x08),
            lod_item_trace_u8_at(rdram, true, phys, 0x09),
            lod_item_trace_s16_at(rdram, true, phys, 0x0E),
            lod_item_trace_u32_at(rdram, true, phys, 0x08),
            lod_item_trace_u32_at(rdram, true, phys, 0x10),
            lod_item_trace_u32_at(rdram, true, phys, 0x14),
            lod_item_trace_u32_at(rdram, true, phys, 0x18),
            lod_item_trace_u32_at(rdram, true, phys, 0x1C),
            lod_item_trace_u32_at(rdram, true, phys, 0x24),
            lod_item_trace_u32_at(rdram, true, phys, 0x30),
            lod_item_trace_u32_at(rdram, true, phys, 0x34),
            lod_item_trace_u32_at(rdram, true, phys, 0x38),
            lod_item_trace_u32_at(rdram, true, phys, 0x3C),
            lod_item_trace_u32_at(rdram, true, phys, 0x40),
            lod_item_trace_u32_at(rdram, true, phys, 0x5C),
            lod_item_trace_u32_at(rdram, true, phys, 0x60));
}

static bool lod_item_child_trace_has_bad_chain(uint8_t* rdram, uint32_t root_addr) {
    if (lod_item_trace_is_bad_value(root_addr)) {
        return true;
    }

    uint32_t root_phys = 0;
    if (!lod_decode_rdram_phys_addr(root_addr, 0x74, &root_phys)) {
        return root_addr != 0;
    }

    const uint32_t root_child = lod_rdram_u32(rdram, root_phys + 0x08);
    const uint32_t root_next = lod_rdram_u32(rdram, root_phys + 0x10);
    const uint32_t root_parent = lod_rdram_u32(rdram, root_phys + 0x14);
    if (lod_item_trace_is_bad_value(root_child) ||
        lod_item_trace_is_bad_value(root_next) ||
        lod_item_trace_is_bad_value(root_parent)) {
        return true;
    }

    uint32_t child_phys = 0;
    if (root_child == 0) {
        return false;
    }
    if (!lod_decode_rdram_phys_addr(root_child, 0x74, &child_phys)) {
        return true;
    }

    const uint32_t child_next = lod_rdram_u32(rdram, child_phys + 0x10);
    const uint32_t child_parent = lod_rdram_u32(rdram, child_phys + 0x14);
    if (lod_item_trace_is_bad_value(child_next) ||
        lod_item_trace_is_bad_value(child_parent)) {
        return true;
    }

    uint32_t child_parent_phys = 0;
    if (child_parent != 0 &&
        !lod_decode_rdram_phys_addr(child_parent, 0x74, &child_parent_phys)) {
        return true;
    }
    if (child_parent_phys != 0) {
        const uint32_t parent_next = lod_rdram_u32(rdram, child_parent_phys + 0x10);
        const uint32_t parent_parent = lod_rdram_u32(rdram, child_parent_phys + 0x14);
        if (lod_item_trace_is_bad_value(parent_next) ||
            lod_item_trace_is_bad_value(parent_parent)) {
            return true;
        }
    }

    return false;
}

static void lod_item_child_trace_log(uint8_t* rdram, const recomp_context* ctx,
                                     uint32_t count) {
    const uint32_t root_addr = (uint32_t)ctx->r4;
    const uint32_t mask = (uint32_t)ctx->r5;
    const int loaded_0f = lod_ni_overlay_loaded_0f_pair();
    const int loaded_0e = lod_ni_overlay_loaded_0e_pair();
    const int32_t gs = lod_current_gamestate(rdram);
    const bool pair99_cleanup = gs == 3 && loaded_0f == 99 && mask == 0;
    const bool bad_chain = lod_item_child_trace_has_bad_chain(rdram, root_addr);

    if (!pair99_cleanup && !bad_chain && count > 6) {
        return;
    }

    fprintf(stderr,
            "[ITEM_CHILD] pre #%u object_dispatchChild gs=%d map#%d map_rom=0x%08X "
            "ni={0f=%d 0e=%d} root=0x%08X mask=0x%08X bad_chain=%d "
            "args={a0=0x%08X a1=0x%08X a2=0x%08X a3=0x%08X ra=0x%08X v0=0x%08X}\n",
            count, gs, lod_map_ovl_load_count, lod_current_map_ovl_rom,
            loaded_0f, loaded_0e, root_addr, mask, bad_chain ? 1 : 0,
            (uint32_t)ctx->r4, (uint32_t)ctx->r5, (uint32_t)ctx->r6,
            (uint32_t)ctx->r7, (uint32_t)ctx->r31, (uint32_t)ctx->r2);

    lod_item_child_trace_print_obj(rdram, "root", root_addr);

    uint32_t root_phys = 0;
    if (lod_decode_rdram_phys_addr(root_addr, 0x74, &root_phys)) {
        const uint32_t root_child = lod_rdram_u32(rdram, root_phys + 0x08);
        const uint32_t root_next = lod_rdram_u32(rdram, root_phys + 0x10);
        const uint32_t root_parent = lod_rdram_u32(rdram, root_phys + 0x14);
        fprintf(stderr,
                "[ITEM_CHILD]   root_links child=0x%08X next=0x%08X parent=0x%08X\n",
                root_child, root_next, root_parent);
        if (root_child != 0) {
            lod_item_child_trace_print_obj(rdram, "root.child", root_child);
            uint32_t child_phys = 0;
            if (lod_decode_rdram_phys_addr(root_child, 0x74, &child_phys)) {
                const uint32_t child_child = lod_rdram_u32(rdram, child_phys + 0x08);
                const uint32_t child_next = lod_rdram_u32(rdram, child_phys + 0x10);
                const uint32_t child_parent = lod_rdram_u32(rdram, child_phys + 0x14);
                fprintf(stderr,
                        "[ITEM_CHILD]   child_links child=0x%08X next=0x%08X parent=0x%08X\n",
                        child_child, child_next, child_parent);
                if (child_parent != 0) {
                    lod_item_child_trace_print_obj(rdram, "root.child.parent", child_parent);
                }
                if (child_next != 0 && child_next != root_child) {
                    lod_item_child_trace_print_obj(rdram, "root.child.next", child_next);
                }
            }
        }
        if (root_parent != 0) {
            lod_item_child_trace_print_obj(rdram, "root.parent", root_parent);
        }
    }
    fflush(stderr);
}

static void lod_item_trace_log(uint8_t* rdram, const recomp_context* ctx,
                               const char* phase, const char* label,
                               uint32_t vram, uint32_t count,
                               uint32_t obj_addr, const LodItemTraceInterest& interest,
                               bool force) {
    static uint32_t total_logs = 0;
    if (!force && !interest.bad) {
        return;
    }
    if (!interest.bad && total_logs >= 180) {
        return;
    }
    total_logs++;

    uint32_t obj_phys = 0;
    const bool obj_ok = lod_decode_rdram_phys_addr(obj_addr, 0x74, &obj_phys);
    const uint8_t obj_step = lod_item_trace_u8_at(rdram, obj_ok, obj_phys, 0x08);
    const uint8_t obj_state = lod_item_trace_u8_at(rdram, obj_ok, obj_phys, 0x09);
    const int16_t obj_level = lod_item_trace_s16_at(rdram, obj_ok, obj_phys, 0x0E);
    const uint32_t obj_10 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x10);
    const uint32_t obj_14 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x14);
    const uint32_t obj_1c = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x1C);
    const uint32_t obj_24 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x24);
    const uint32_t obj_30 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x30);
    const uint32_t obj_34 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x34);
    const uint32_t obj_38 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x38);
    const uint32_t obj_3c = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x3C);
    const uint32_t obj_40 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x40);
    const uint32_t obj_58 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x58);
    const uint32_t obj_5c = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x5C);
    const uint32_t obj_60 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x60);
    const uint32_t obj_64 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x64);
    const uint32_t obj_6c = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x6C);
    const uint32_t obj_70 = lod_item_trace_u32_at(rdram, obj_ok, obj_phys, 0x70);

    fprintf(stderr,
            "[ITEM_KSEG0] %s #%u %s vram=0x%08X gs=%d map#%d map_rom=0x%08X "
            "ni={0f=%d 0e=%d} bad=%d seg=%d "
            "args={a0=0x%08X a1=0x%08X a2=0x%08X a3=0x%08X ra=0x%08X v0=0x%08X} "
            "obj_ok=%d obj_phys=0x%06X obj={step=%u state=%u level=%d "
            "10=0x%08X 14=0x%08X 1C=0x%08X 24=0x%08X 30=0x%08X 34=0x%08X "
            "38=0x%08X 3C=0x%08X 40=0x%08X 58=0x%08X 5C=0x%08X 60=0x%08X "
            "64=0x%08X 6C=0x%08X 70=0x%08X}\n",
            phase, count, label, vram, lod_current_gamestate(rdram),
            lod_map_ovl_load_count, lod_current_map_ovl_rom,
            lod_ni_overlay_loaded_0f_pair(), lod_ni_overlay_loaded_0e_pair(),
            interest.bad ? 1 : 0, interest.segmented ? 1 : 0,
            (uint32_t)ctx->r4, (uint32_t)ctx->r5, (uint32_t)ctx->r6,
            (uint32_t)ctx->r7, (uint32_t)ctx->r31, (uint32_t)ctx->r2,
            obj_ok ? 1 : 0, obj_ok ? obj_phys : 0,
            obj_step, obj_state, obj_level,
            obj_10, obj_14, obj_1c, obj_24, obj_30, obj_34,
            obj_38, obj_3c, obj_40, obj_58, obj_5c, obj_60,
            obj_64, obj_6c, obj_70);

    const struct {
        const char* label;
        uint32_t addr;
    } nested[] = {
        {"obj24", obj_24}, {"obj30", obj_30}, {"obj34", obj_34}, {"obj38", obj_38},
        {"obj58", obj_58}, {"obj5C", obj_5c}, {"obj60", obj_60}, {"obj70", obj_70},
    };
    for (const auto& ptr : nested) {
        if (ptr.addr != 0 && lod_item_trace_nested_has_segment(rdram, ptr.addr)) {
            lod_item_trace_print_nested(rdram, phase, label, count, ptr.label, ptr.addr);
        }
    }
}

static void lod_item_trace_call(uint8_t* rdram, recomp_context* ctx,
                                const char* label, uint32_t vram,
                                recomp_func_t* original, uint32_t* counter) {
    (*counter)++;
    const uint32_t count = *counter;
    const uint32_t obj_addr = (uint32_t)ctx->r4;
    LodItemTraceInterest pre = lod_item_trace_collect(rdram, ctx, obj_addr);
    const bool force_pre = pre.bad || count <= 3 ||
                           (pre.segmented && count <= 12) ||
                           (pre.segmented && (count % 600) == 0);
    lod_item_trace_log(rdram, ctx, "pre", label, vram, count, obj_addr, pre, force_pre);

    if (original != nullptr) {
        original(rdram, ctx);
    }

    LodItemTraceInterest post = lod_item_trace_collect(rdram, ctx, obj_addr);
    if (post.bad) {
        lod_item_trace_log(rdram, ctx, "post", label, vram, count, obj_addr, post, true);
    }
}

#define LOD_ITEM_TRACE_WRAPPER(wrapper_name, label, vram_value, original_var) \
static void wrapper_name(uint8_t* rdram, recomp_context* ctx) { \
    static uint32_t count = 0; \
    lod_item_trace_call(rdram, ctx, label, vram_value, original_var, &count); \
}

static void lod_item_trace_object_dispatch_child(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    lod_item_child_trace_log(rdram, ctx, count);
    if (lod_orig_item_object_dispatch_child != nullptr) {
        lod_orig_item_object_dispatch_child(rdram, ctx);
    }
}

LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_8007D9F0, "func_8007D9F0", 0x8007D9F0, lod_orig_item_func_8007D9F0)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_8007E8F4, "func_8007E8F4", 0x8007E8F4, lod_orig_item_func_8007E8F4)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_80085280, "func_80085280", 0x80085280, lod_orig_item_func_80085280)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_800856B8, "func_800856B8", 0x800856B8, lod_orig_item_func_800856B8)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_80086120, "func_80086120", 0x80086120, lod_orig_item_func_80086120)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_80086528, "func_80086528", 0x80086528, lod_orig_item_func_80086528)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_8008AA90, "func_8008AA90", 0x8008AA90, lod_orig_item_func_8008AA90)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_8008B3A4, "func_8008B3A4", 0x8008B3A4, lod_orig_item_func_8008B3A4)
LOD_ITEM_TRACE_WRAPPER(lod_item_trace_func_8008F590, "func_8008F590", 0x8008F590, lod_orig_item_func_8008F590)

#undef LOD_ITEM_TRACE_WRAPPER

static void lod_install_item_kseg0_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                                 recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

extern "C" void lod_install_item_kseg0_trace_wrappers_early() {
    static int install_count = 0;
    install_count++;

    lod_install_item_kseg0_trace_wrapper(0x8000516C,
        lod_item_trace_object_dispatch_child, &lod_orig_item_object_dispatch_child);
    lod_install_item_kseg0_trace_wrapper(0x8007D9F0,
        lod_item_trace_func_8007D9F0, &lod_orig_item_func_8007D9F0);
    lod_install_item_kseg0_trace_wrapper(0x8007E8F4,
        lod_item_trace_func_8007E8F4, &lod_orig_item_func_8007E8F4);
    lod_install_item_kseg0_trace_wrapper(0x80085280,
        lod_item_trace_func_80085280, &lod_orig_item_func_80085280);
    lod_install_item_kseg0_trace_wrapper(0x800856B8,
        lod_item_trace_func_800856B8, &lod_orig_item_func_800856B8);
    lod_install_item_kseg0_trace_wrapper(0x80086120,
        lod_item_trace_func_80086120, &lod_orig_item_func_80086120);
    lod_install_item_kseg0_trace_wrapper(0x80086528,
        lod_item_trace_func_80086528, &lod_orig_item_func_80086528);
    lod_install_item_kseg0_trace_wrapper(0x8008AA90,
        lod_item_trace_func_8008AA90, &lod_orig_item_func_8008AA90);
    lod_install_item_kseg0_trace_wrapper(0x8008B3A4,
        lod_item_trace_func_8008B3A4, &lod_orig_item_func_8008B3A4);
    lod_install_item_kseg0_trace_wrapper(0x8008F590,
        lod_item_trace_func_8008F590, &lod_orig_item_func_8008F590);

    if (install_count <= 4) {
        fprintf(stderr,
                "[ITEM_KSEG0] installed item/model trace wrappers #%d originals "
                "child=%p 7d9f0=%p 7e8f4=%p 85280=%p 856b8=%p 86120=%p "
                "86528=%p 8aa90=%p 8b3a4=%p 8f590=%p\n",
                install_count,
                (void*)lod_orig_item_object_dispatch_child,
                (void*)lod_orig_item_func_8007D9F0,
                (void*)lod_orig_item_func_8007E8F4,
                (void*)lod_orig_item_func_80085280,
                (void*)lod_orig_item_func_800856B8,
                (void*)lod_orig_item_func_80086120,
                (void*)lod_orig_item_func_80086528,
                (void*)lod_orig_item_func_8008AA90,
                (void*)lod_orig_item_func_8008B3A4,
                (void*)lod_orig_item_func_8008F590);
    }
}
#endif

#if LOD_ENABLE_FOG_LAKE_TRIGGER_TRACE
static recomp_func_t* lod_orig_fog_gate_init_802E5328 = nullptr;
static recomp_func_t* lod_orig_fog_gate_check_802E5458 = nullptr;
static recomp_func_t* lod_orig_fog_platform_wait_802E55E8 = nullptr;
static recomp_func_t* lod_orig_fog_enemy_slot_802E56A8 = nullptr;

static constexpr uint32_t LOD_FOG_MAP_ROM = 0x007AE430;
static constexpr uint32_t LOD_FOG_SYS_PHYS = 0x001C82C0;        // 0x801C82C0
static constexpr uint32_t LOD_FOG_SYS_FLAGS_PHYS = LOD_FOG_SYS_PHYS + 0x2858;
static constexpr uint32_t LOD_FOG_KILL_COUNT_PHYS = LOD_FOG_SYS_PHYS + 0x2B38;
static constexpr uint32_t LOD_FOG_OVL_GLOBAL_SLOT0_PHYS = 0x002E5AFC; // 0x802E5AFC
static constexpr uint32_t LOD_FOG_ENEMY_LIST_PHYS = 0x000F9120;       // 0x800F9120
static constexpr uint32_t LOD_FOG_ENEMY_LIST_ENTRY_SIZE = 0x1C;
static constexpr uint32_t LOD_FOG_ENEMY_DONE_FLAG = 0x10;

struct LodFogEnemyState {
    uint32_t actor = 0;
    uint32_t entry = 0;
    uint32_t flags = 0;
    uint32_t aux_flags = 0;
    bool found = false;
};

struct LodFogState {
    uint32_t obj = 0;
    bool obj_ok = false;
    int16_t obj_func = 0;
    uint32_t obj_timer = 0;
    uint32_t sys_flags = 0;
    int16_t kill_count = 0;
    uint32_t slot[2] = {0, 0};
    uint32_t global_slot[2] = {0, 0};
    LodFogEnemyState enemy[2];
};

static LodFogEnemyState lod_fog_enemy_state_for_actor(uint8_t* rdram, uint32_t actor) {
    LodFogEnemyState state;
    state.actor = actor;

    if (actor == 0 || !lod_rdram_range_ok(LOD_FOG_ENEMY_LIST_PHYS, 4)) {
        return state;
    }

    int32_t count = (int32_t)lod_rdram_u32(rdram, LOD_FOG_ENEMY_LIST_PHYS);
    if (count < 0) {
        count = 0;
    } else if (count > 0x18) {
        count = 0x18;
    }

    for (int32_t i = 0; i < count; i++) {
        const uint32_t entry_phys = LOD_FOG_ENEMY_LIST_PHYS + 4 + (uint32_t)i * LOD_FOG_ENEMY_LIST_ENTRY_SIZE;
        if (!lod_rdram_range_ok(entry_phys, LOD_FOG_ENEMY_LIST_ENTRY_SIZE)) {
            break;
        }

        const uint32_t entry_actor = lod_rdram_u32(rdram, entry_phys + 0x18);
        if (entry_actor == actor) {
            state.entry = 0x80000000u | entry_phys;
            state.flags = lod_rdram_u32(rdram, entry_phys + 0x00);
            state.aux_flags = lod_rdram_u32(rdram, entry_phys + 0x04);
            state.found = true;
            break;
        }
    }

    return state;
}

static LodFogState lod_fog_capture_state(uint8_t* rdram, uint32_t obj) {
    LodFogState state;
    state.obj = obj;

    if (lod_rdram_range_ok(LOD_FOG_SYS_FLAGS_PHYS, 4)) {
        state.sys_flags = lod_rdram_u32(rdram, LOD_FOG_SYS_FLAGS_PHYS);
    }
    if (lod_rdram_range_ok(LOD_FOG_KILL_COUNT_PHYS, 2)) {
        state.kill_count = lod_rdram_s16(rdram, LOD_FOG_KILL_COUNT_PHYS);
    }
    if (lod_rdram_range_ok(LOD_FOG_OVL_GLOBAL_SLOT0_PHYS, 8)) {
        state.global_slot[0] = lod_rdram_u32(rdram, LOD_FOG_OVL_GLOBAL_SLOT0_PHYS + 0);
        state.global_slot[1] = lod_rdram_u32(rdram, LOD_FOG_OVL_GLOBAL_SLOT0_PHYS + 4);
    }

    const uint32_t obj_phys = obj & 0x1FFFFFFF;
    state.obj_ok = obj != 0 && lod_rdram_range_ok(obj_phys, 0x4C);
    if (state.obj_ok) {
        state.obj_func = lod_rdram_s16(rdram, obj_phys + 0x0E);
        state.slot[0] = lod_rdram_u32(rdram, obj_phys + 0x40);
        state.slot[1] = lod_rdram_u32(rdram, obj_phys + 0x44);
        state.obj_timer = lod_rdram_u32(rdram, obj_phys + 0x48);
    }

    for (int i = 0; i < 2; i++) {
        state.enemy[i] = lod_fog_enemy_state_for_actor(rdram, state.slot[i]);
    }

    return state;
}

static bool lod_fog_state_slots_changed(const LodFogState& before, const LodFogState& after) {
    return before.slot[0] != after.slot[0] ||
           before.slot[1] != after.slot[1] ||
           before.global_slot[0] != after.global_slot[0] ||
           before.global_slot[1] != after.global_slot[1];
}

static bool lod_fog_state_enemy_flags_changed(const LodFogState& before, const LodFogState& after) {
    return before.enemy[0].found != after.enemy[0].found ||
           before.enemy[1].found != after.enemy[1].found ||
           before.enemy[0].flags != after.enemy[0].flags ||
           before.enemy[1].flags != after.enemy[1].flags ||
           before.enemy[0].aux_flags != after.enemy[0].aux_flags ||
           before.enemy[1].aux_flags != after.enemy[1].aux_flags;
}

static bool lod_fog_state_has_done_flag(const LodFogState& state) {
    return (state.enemy[0].found && ((state.enemy[0].flags & LOD_FOG_ENEMY_DONE_FLAG) != 0)) ||
           (state.enemy[1].found && ((state.enemy[1].flags & LOD_FOG_ENEMY_DONE_FLAG) != 0));
}

static void lod_fog_print_state_delta(uint8_t* rdram, const char* label, uint32_t count,
                                      const LodFogState& before,
                                      const LodFogState& after) {
    fprintf(stderr,
            "[FOG_LAKE] %s#%u gs=%d map_rom=0x%08X obj=0x%08X ok=%d "
            "func=%d->%d timer=%u->%u sys_flags=0x%08X kill=%d->%d "
            "slots=[0x%08X,0x%08X]->[0x%08X,0x%08X] "
            "globals=[0x%08X,0x%08X]->[0x%08X,0x%08X] "
            "enemy0={found=%d->%d flags=0x%08X->0x%08X aux=0x%08X->0x%08X entry=0x%08X->0x%08X} "
            "enemy1={found=%d->%d flags=0x%08X->0x%08X aux=0x%08X->0x%08X entry=0x%08X->0x%08X}\n",
            label, count, lod_current_gamestate(rdram), lod_current_map_ovl_rom,
            before.obj, before.obj_ok ? 1 : 0,
            before.obj_func, after.obj_func, before.obj_timer, after.obj_timer,
            after.sys_flags, before.kill_count, after.kill_count,
            before.slot[0], before.slot[1], after.slot[0], after.slot[1],
            before.global_slot[0], before.global_slot[1], after.global_slot[0], after.global_slot[1],
            before.enemy[0].found ? 1 : 0, after.enemy[0].found ? 1 : 0,
            before.enemy[0].flags, after.enemy[0].flags,
            before.enemy[0].aux_flags, after.enemy[0].aux_flags,
            before.enemy[0].entry, after.enemy[0].entry,
            before.enemy[1].found ? 1 : 0, after.enemy[1].found ? 1 : 0,
            before.enemy[1].flags, after.enemy[1].flags,
            before.enemy[1].aux_flags, after.enemy[1].aux_flags,
            before.enemy[1].entry, after.enemy[1].entry);
}

static void lod_trace_fog_gate_init_802E5328(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t obj = (uint32_t)ctx->r4;
    const LodFogState before = lod_fog_capture_state(rdram, obj);
    if (lod_orig_fog_gate_init_802E5328 != nullptr) {
        lod_orig_fog_gate_init_802E5328(rdram, ctx);
    }
    const LodFogState after = lod_fog_capture_state(rdram, obj);

    const bool interesting = count <= 6 ||
        before.kill_count != after.kill_count ||
        lod_fog_state_slots_changed(before, after) ||
        lod_fog_state_enemy_flags_changed(before, after);
    if (interesting) {
        lod_fog_print_state_delta(rdram, "gate_init", count, before, after);
    }
}

static void lod_trace_fog_gate_check_802E5458(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t obj = (uint32_t)ctx->r4;
    const LodFogState before = lod_fog_capture_state(rdram, obj);
    if (lod_orig_fog_gate_check_802E5458 != nullptr) {
        lod_orig_fog_gate_check_802E5458(rdram, ctx);
    }
    const LodFogState after = lod_fog_capture_state(rdram, obj);

    const bool has_tracked_enemy = before.slot[0] != 0 || before.slot[1] != 0 ||
                                   after.slot[0] != 0 || after.slot[1] != 0;
    const bool interesting = (count <= 8 && has_tracked_enemy) ||
        before.kill_count != after.kill_count ||
        before.obj_func != after.obj_func ||
        lod_fog_state_slots_changed(before, after) ||
        lod_fog_state_enemy_flags_changed(before, after) ||
        lod_fog_state_has_done_flag(before) ||
        lod_fog_state_has_done_flag(after);
    if (interesting) {
        lod_fog_print_state_delta(rdram, "gate_check", count, before, after);
    }
}

static void lod_trace_fog_platform_wait_802E55E8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    static int16_t last_logged_func = -32768;
    static int16_t last_logged_kill_count = -32768;
    count++;
    const uint32_t obj = (uint32_t)ctx->r4;
    const LodFogState before = lod_fog_capture_state(rdram, obj);
    if (lod_orig_fog_platform_wait_802E55E8 != nullptr) {
        lod_orig_fog_platform_wait_802E55E8(rdram, ctx);
    }
    const LodFogState after = lod_fog_capture_state(rdram, obj);

    const bool interesting = count <= 4 ||
        before.obj_func != after.obj_func ||
        after.obj_func != last_logged_func ||
        after.kill_count != last_logged_kill_count ||
        (before.kill_count > 0 && after.kill_count <= 0);
    if (interesting) {
        last_logged_func = after.obj_func;
        last_logged_kill_count = after.kill_count;
        lod_fog_print_state_delta(rdram, "platform_wait", count, before, after);
    }
}

static void lod_trace_fog_enemy_slot_802E56A8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    count++;
    const uint32_t obj = (uint32_t)ctx->r4;
    const LodFogState before = lod_fog_capture_state(rdram, obj);
    if (lod_orig_fog_enemy_slot_802E56A8 != nullptr) {
        lod_orig_fog_enemy_slot_802E56A8(rdram, ctx);
    }
    const LodFogState after = lod_fog_capture_state(rdram, obj);

    const bool interesting = count <= 6 ||
        lod_fog_state_slots_changed(before, after) ||
        lod_fog_state_enemy_flags_changed(before, after);
    if (interesting) {
        lod_fog_print_state_delta(rdram, "enemy_slot", count, before, after);
    }
}

static void lod_install_fog_lake_trigger_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                                       recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

static void lod_install_fog_lake_trigger_trace_wrappers(const char* reason) {
    static int install_count = 0;
    install_count++;

    lod_install_fog_lake_trigger_trace_wrapper(0x802E5328, lod_trace_fog_gate_init_802E5328,
        &lod_orig_fog_gate_init_802E5328);
    lod_install_fog_lake_trigger_trace_wrapper(0x802E5458, lod_trace_fog_gate_check_802E5458,
        &lod_orig_fog_gate_check_802E5458);
    lod_install_fog_lake_trigger_trace_wrapper(0x802E55E8, lod_trace_fog_platform_wait_802E55E8,
        &lod_orig_fog_platform_wait_802E55E8);
    lod_install_fog_lake_trigger_trace_wrapper(0x802E56A8, lod_trace_fog_enemy_slot_802E56A8,
        &lod_orig_fog_enemy_slot_802E56A8);

    fprintf(stderr,
            "[FOG_LAKE] installed trigger trace wrappers #%d reason=%s map_rom=0x%08X "
            "orig={5328=%p 5458=%p 55E8=%p 56A8=%p}\n",
            install_count, reason, lod_current_map_ovl_rom,
            (void*)lod_orig_fog_gate_init_802E5328,
            (void*)lod_orig_fog_gate_check_802E5458,
            (void*)lod_orig_fog_platform_wait_802E55E8,
            (void*)lod_orig_fog_enemy_slot_802E56A8);
}
#endif

#if LOD_ENABLE_INPUT_TRACE
static recomp_func_t* lod_orig_input_action_8002D6E8 = nullptr;
static recomp_func_t* lod_orig_input_action_80031470 = nullptr;
static recomp_func_t* lod_orig_input_action_800332C4 = nullptr;

static void lod_trace_input_action_candidate(uint8_t* rdram, recomp_context* ctx,
                                             const char* label, uint32_t vram,
                                             recomp_func_t* original,
                                             uint32_t* counter) {
    (*counter)++;

    constexpr uint32_t PLAYER_CONT_DATA_PHYS = 0x000F35F0; // 0x800F35F0
    constexpr uint32_t PLAYER_CONT_CUR = PLAYER_CONT_DATA_PHYS + 0x04;
    const uint16_t held = lod_rdram_range_ok(PLAYER_CONT_CUR + 0x02, 0x02)
        ? (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x02) : 0;
    const uint16_t pressed = lod_rdram_range_ok(PLAYER_CONT_CUR + 0x04, 0x02)
        ? (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x04) : 0;
    const bool action_active = ((held | pressed) & 0xC000) != 0;
    const int32_t gs = lod_current_gamestate(rdram);
    const bool log_this = (*counter <= 4) || (gs == 3 && action_active);

    const uint32_t obj_addr = (uint32_t)ctx->r4;
    const uint32_t obj_phys = obj_addr & 0x1FFFFFFF;
    const bool obj_ok = lod_rdram_range_ok(obj_phys, 0x38);
    const uint8_t obj_state09_before = obj_ok ? lod_rdram_u8(rdram, obj_phys + 0x09) : 255;
    const int16_t obj_func_before = obj_ok ? lod_rdram_s16(rdram, obj_phys + 0x0E) : 0;
    const uint32_t actor_addr = obj_ok ? lod_rdram_u32(rdram, obj_phys + 0x34) : 0;
    const uint32_t actor_phys = actor_addr & 0x1FFFFFFF;
    const bool actor_ok = actor_addr != 0 && lod_rdram_range_ok(actor_phys, 0x100);
    const uint32_t actor_flags0_before = actor_ok ? lod_rdram_u32(rdram, actor_phys + 0x00) : 0;
    const uint32_t actor_flags8_before = actor_ok ? lod_rdram_u32(rdram, actor_phys + 0x08) : 0;
    const int16_t actor_gate_f8_before = actor_ok ? lod_rdram_s16(rdram, actor_phys + 0xF8) : 0;

    if (log_this) {
        fprintf(stderr,
                "[INPUT_ACTION] enter %s#%u vram=0x%08X gs=%d a0=0x%08X "
                "ctrl={held=0x%04X pressed=0x%04X} "
                "obj={ok=%d state09=%u func=%d} "
                "actor=0x%08X {ok=%d flags0=0x%08X flags8=0x%08X gateF8=%d} ra=0x%08X orig=%p\n",
                label, *counter, vram, gs, obj_addr,
                held, pressed,
                obj_ok ? 1 : 0, obj_state09_before, obj_func_before,
                actor_addr, actor_ok ? 1 : 0,
                actor_flags0_before, actor_flags8_before, actor_gate_f8_before,
                (uint32_t)ctx->r31, (void*)original);
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }

    if (log_this) {
        const uint16_t held_after = lod_rdram_range_ok(PLAYER_CONT_CUR + 0x02, 0x02)
            ? (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x02) : 0;
        const uint16_t pressed_after = lod_rdram_range_ok(PLAYER_CONT_CUR + 0x04, 0x02)
            ? (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x04) : 0;
        const uint8_t obj_state09_after = obj_ok ? lod_rdram_u8(rdram, obj_phys + 0x09) : 255;
        const int16_t obj_func_after = obj_ok ? lod_rdram_s16(rdram, obj_phys + 0x0E) : 0;
        const uint32_t actor_flags0_after = actor_ok ? lod_rdram_u32(rdram, actor_phys + 0x00) : 0;
        const uint32_t actor_flags8_after = actor_ok ? lod_rdram_u32(rdram, actor_phys + 0x08) : 0;
        const int16_t actor_gate_f8_after = actor_ok ? lod_rdram_s16(rdram, actor_phys + 0xF8) : 0;
        fprintf(stderr,
                "[INPUT_ACTION] exit  %s#%u vram=0x%08X gs=%d v0=0x%08X "
                "ctrl={held=0x%04X pressed=0x%04X -> held=0x%04X pressed=0x%04X} "
                "obj={state09=%u->%u func=%d->%d} "
                "actor={flags0=0x%08X->0x%08X flags8=0x%08X->0x%08X gateF8=%d->%d}\n",
                label, *counter, vram, lod_current_gamestate(rdram), (uint32_t)ctx->r2,
                held, pressed, held_after, pressed_after,
                obj_state09_before, obj_state09_after, obj_func_before, obj_func_after,
                actor_flags0_before, actor_flags0_after,
                actor_flags8_before, actor_flags8_after,
                actor_gate_f8_before, actor_gate_f8_after);
    }
}

static void lod_trace_input_action_8002D6E8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    lod_trace_input_action_candidate(rdram, ctx, "act_8002D6E8", 0x8002D6E8,
                                     lod_orig_input_action_8002D6E8, &count);
}

static void lod_trace_input_action_80031470(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    lod_trace_input_action_candidate(rdram, ctx, "act_80031470", 0x80031470,
                                     lod_orig_input_action_80031470, &count);
}

static void lod_trace_input_action_800332C4(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t count = 0;
    lod_trace_input_action_candidate(rdram, ctx, "act_800332C4", 0x800332C4,
                                     lod_orig_input_action_800332C4, &count);
}

static void lod_install_input_action_trace_wrapper(uint32_t vram, recomp_func_t* wrapper,
                                                   recomp_func_t** original_out) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original_out = current;
    }
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
}

extern "C" void lod_install_input_action_trace_wrappers_early() {
    static int install_count = 0;
    install_count++;

    lod_install_input_action_trace_wrapper(0x8002D6E8, lod_trace_input_action_8002D6E8,
        &lod_orig_input_action_8002D6E8);
    lod_install_input_action_trace_wrapper(0x80031470, lod_trace_input_action_80031470,
        &lod_orig_input_action_80031470);
    lod_install_input_action_trace_wrapper(0x800332C4, lod_trace_input_action_800332C4,
        &lod_orig_input_action_800332C4);

    if (install_count <= 4) {
        fprintf(stderr,
                "[INPUT_ACTION] installed wrappers #%d originals "
                "8002D6E8=%p 80031470=%p 800332C4=%p\n",
                install_count,
                (void*)lod_orig_input_action_8002D6E8,
                (void*)lod_orig_input_action_80031470,
                (void*)lod_orig_input_action_800332C4);
    }
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
#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
    if (si_dma_count <= 10 || si_dma_count % 500 == 0) {
        uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
        int32_t cur_gs = 0;
        if (gsm_addr != 0) cur_gs = *(int32_t*)(rdram + (gsm_addr & 0x1FFFFFFF) + 0x24);
        fprintf(stderr, "[SI_DMA] #%d dir=%d gs=%d\n", si_dma_count, direction, cur_gs);
    }
#endif
#if LOD_ENABLE_B2_ASSET_TRACE
    if (direction == 0) {
        lod_map76_object_trace(rdram, si_dma_count);
    }
#endif

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

#if LOD_AUTO_ADVANCE_EXPANSION_PAK_SCREEN && !LOD_ENABLE_BOOT_INPUT_SCRIPT && !LOD_ENABLE_FAST_GAMEPLAY_INPUT_SCRIPT
                // Default baseline: do not expose the unstable native high-res
                // selector to normal testers. Advance the Expansion Pak screen
                // through the normal controller/PIF path, without forcing
                // gamestate or enabling the broader debug boot script.
                {
                    static int32_t ep_last_gs = INT32_MIN;
                    static int ep_gs_frame = 0;
                    int32_t ep_gs = lod_current_gamestate(rdram);
                    if (ep_gs != ep_last_gs) {
                        ep_last_gs = ep_gs;
                        ep_gs_frame = 0;
                    }
                    ep_gs_frame++;

                    if (ep_gs == 12) {
                        bool pulse = (ep_gs_frame >= 30 && ep_gs_frame < 36) ||
                                     (ep_gs_frame >= 90 && ep_gs_frame < 96) ||
                                     (ep_gs_frame >= 150 && ep_gs_frame < 156);
                        if (pulse) {
                            buttons |= 0x9000; // A + Start
                            if (ep_gs_frame == 30 ||
                                ep_gs_frame == 90 ||
                                ep_gs_frame == 150) {
                                fprintf(stderr, "[AUTO] gs=12 frame %d: A+Start (native high-res selector auto-advance)\n",
                                        ep_gs_frame);
                            }
                        }
                    }
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
                    int file_select_a_frame = (LOD_ITEM_MENU_SAVE_SLOT > 1) ? 1320 : 1200;
                    bool data_menu_confirm_pulse =
                        (boot_script_gs_frame >= 720 && boot_script_gs_frame < 726 &&
                         LOD_ITEM_MENU_SAVE_SLOT <= 1);
                    bool pulse = (boot_script_gs_frame >= 60 && boot_script_gs_frame < 66) ||
                                 (boot_script_gs_frame >= 180 && boot_script_gs_frame < 186) ||
                                 (boot_script_gs_frame >= 360 && boot_script_gs_frame < 366) ||
                                 data_menu_confirm_pulse ||
                                 (boot_script_gs_frame >= file_select_a_frame &&
                                  boot_script_gs_frame < file_select_a_frame + 6);
                    if (pulse) {
                        buttons |= 0x8000; // A
                        if (boot_script_gs_frame == 60 ||
                            boot_script_gs_frame == 180 ||
                            boot_script_gs_frame == 360 ||
                            (boot_script_gs_frame == 720 && LOD_ITEM_MENU_SAVE_SLOT <= 1) ||
                            boot_script_gs_frame == file_select_a_frame) {
                            fprintf(stderr, "[AUTO] gs=8 frame %d: A\n", boot_script_gs_frame);
                        }
                    }

                    if (boot_script_gs == 8) {
                        // The regular boot script reaches the file-select
                        // screen around gs=8 frame ~1100 and presses A at
                        // frame 1200. Pick save #2/#3 with ordinary D-pad
                        // input before that existing A pulse lands.
                        int desired_slot = LOD_ITEM_MENU_SAVE_SLOT;
                        if (desired_slot < 1) desired_slot = 1;
                        if (desired_slot > 3) desired_slot = 3;
                        for (int slot = 2; slot <= desired_slot; slot++) {
                            int pulse_start = 1140 + (slot - 2) * 40;
                            bool slot_down_pulse =
                                (boot_script_gs_frame >= pulse_start &&
                                 boot_script_gs_frame < pulse_start + 24);
                            if (slot_down_pulse) {
                                y = -1.0f;          // analog-only down; D-pad opens the file submenu here
                                if (boot_script_gs_frame == pulse_start) {
                                    fprintf(stderr,
                                            "[AUTO] item-menu gs=8 frame %d: AnalogDown (save slot %d)\n",
                                            boot_script_gs_frame, slot);
                                }
                            }
                        }
                    }
                }
#endif
#endif

#if LOD_ENABLE_ITEM_MENU_INPUT_SCRIPT
                // Debug-only pause/menu repro harness. This deliberately uses
                // normal PIF/controller input, after the natural boot/load
                // route reaches gameplay, so screenshots/logs reflect the
                // real menu path rather than a forced gamestate.
                {
                    static int32_t menu_script_last_gs = INT32_MIN;
                    static int menu_script_gs_frame = 0;
                    int32_t menu_script_gs = lod_current_gamestate(rdram);
                    if (menu_script_gs != menu_script_last_gs) {
                        menu_script_last_gs = menu_script_gs;
                        menu_script_gs_frame = 0;
                    }
                    menu_script_gs_frame++;

                    if (menu_script_gs == 3) {
                        // Wait until the loaded save/gameplay frame is stable,
                        // then pulse Start to open the pause menu. The menu
                        // defaults to "Item", so a later A pulse enters the
                        // item screen without cursor movement.
                        bool start_pulse =
                            (menu_script_gs_frame >= 240 && menu_script_gs_frame < 246);
                        bool item_select_pulse =
                            (menu_script_gs_frame >= 330 && menu_script_gs_frame < 332);
                        if (start_pulse) {
                            buttons |= 0x1000; // Start
                            if (menu_script_gs_frame == 240) {
                                fprintf(stderr,
                                        "[AUTO] item-menu gs=3 frame %d: Start\n",
                                        menu_script_gs_frame);
                            }
                        }
                        if (item_select_pulse) {
                            buttons |= 0x8000; // A
                            if (menu_script_gs_frame == 330) {
                                fprintf(stderr,
                                        "[AUTO] item-menu gs=3 frame %d: A (select Item)\n",
                                        menu_script_gs_frame);
                            }
                        }
                    }
                }
#endif

                uint8_t sx = (uint8_t)(int8_t)(x * 127);
                uint8_t sy = (uint8_t)(int8_t)(y * 127);
                WR_MEM_B(pif, response_start + 0, buttons >> 8);
                WR_MEM_B(pif, response_start + 1, buttons & 0xFF);
                WR_MEM_B(pif, response_start + 2, sx);
                WR_MEM_B(pif, response_start + 3, sy);

#if LOD_ENABLE_INPUT_TRACE
                {
                    const int32_t input_trace_gs = lod_current_gamestate(rdram);
                    const bool action_pressed = (buttons & 0xC000) != 0; // A/B
                    if (input_trace_gs == 3 && action_pressed) {
                        static uint32_t input_trace_count = 0;
                        input_trace_count++;
                        if (input_trace_count <= 80 || (input_trace_count % 30) == 0) {
                            // func_80020A0C copies sys.controllers[controller]
                            // from 0x801C87F4 + controller*0xE into the
                            // gameplay/player-control path. The previous
                            // trace accidentally started at +4, so `pressed`
                            // was mislabeled as `conn`.
                            constexpr uint32_t SYS_CONT0_PHYS = 0x001C87F4;
                            constexpr uint32_t SYS_CONTROL_MODE_PHYS = 0x001CAB2C; // 0x801C82C0 + 0x286C
                            constexpr uint32_t PLAYER_CONT_DATA_PHYS = 0x000F35F0; // 0x800F35F0
                            constexpr uint32_t PLAYER_CONT_CUR = PLAYER_CONT_DATA_PHYS + 0x04;
                            constexpr uint32_t PLAYER_CONT_PREV = PLAYER_CONT_DATA_PHYS + 0x12;
                            int16_t sys_control_mode = 0;
                            uint16_t sys_connected = 0;
                            uint16_t sys_held = 0;
                            uint16_t sys_pressed = 0;
                            int16_t sys_joy_x = 0;
                            int16_t sys_joy_y = 0;
                            int16_t sys_joy_ang = 0;
                            int16_t sys_joy_held = 0;
                            if (lod_rdram_range_ok(SYS_CONTROL_MODE_PHYS, 0x02)) {
                                sys_control_mode = lod_rdram_s16(rdram, SYS_CONTROL_MODE_PHYS);
                            }
                            if (lod_rdram_range_ok(SYS_CONT0_PHYS, 0x0E)) {
                                sys_connected = (uint16_t)lod_rdram_s16(rdram, SYS_CONT0_PHYS + 0x00);
                                sys_held = (uint16_t)lod_rdram_s16(rdram, SYS_CONT0_PHYS + 0x02);
                                sys_pressed = (uint16_t)lod_rdram_s16(rdram, SYS_CONT0_PHYS + 0x04);
                                sys_joy_x = lod_rdram_s16(rdram, SYS_CONT0_PHYS + 0x06);
                                sys_joy_y = lod_rdram_s16(rdram, SYS_CONT0_PHYS + 0x08);
                                sys_joy_ang = lod_rdram_s16(rdram, SYS_CONT0_PHYS + 0x0A);
                                sys_joy_held = lod_rdram_s16(rdram, SYS_CONT0_PHYS + 0x0C);
                            }
                            uint16_t player_connected = 0;
                            uint16_t player_held = 0;
                            uint16_t player_pressed = 0;
                            int16_t player_joy_x = 0;
                            int16_t player_joy_y = 0;
                            uint16_t player_prev_held = 0;
                            uint16_t player_prev_pressed = 0;
                            if (lod_rdram_range_ok(PLAYER_CONT_CUR, 0x0E) &&
                                lod_rdram_range_ok(PLAYER_CONT_PREV, 0x0E)) {
                                player_connected = (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x00);
                                player_held = (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x02);
                                player_pressed = (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x04);
                                player_joy_x = lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x06);
                                player_joy_y = lod_rdram_s16(rdram, PLAYER_CONT_CUR + 0x08);
                                player_prev_held = (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_PREV + 0x02);
                                player_prev_pressed = (uint16_t)lod_rdram_s16(rdram, PLAYER_CONT_PREV + 0x04);
                            }
                            fprintf(stderr,
                                    "[INPUT_TRACE] #%u gs=%d pif_btn=0x%04X sx=%d sy=%d "
                                    "mode=%d "
                                    "sys@1C87F4={conn=0x%04X held=0x%04X pressed=0x%04X "
                                    "joy=(%d,%d) ang=%d joyheld=%d} "
                                    "player@0F35F0={conn=0x%04X held=0x%04X pressed=0x%04X "
                                    "joy=(%d,%d) prev_held=0x%04X prev_pressed=0x%04X}\n",
                                    input_trace_count, input_trace_gs, buttons,
                                    (int)(int8_t)sx, (int)(int8_t)sy,
                                    (int)sys_control_mode,
                                    sys_connected, sys_held, sys_pressed,
                                    (int)sys_joy_x, (int)sys_joy_y,
                                    (int)sys_joy_ang, (int)sys_joy_held,
                                    player_connected, player_held, player_pressed,
                                    (int)player_joy_x, (int)player_joy_y,
                                    player_prev_held, player_prev_pressed);
                        }
                    }
                }
#endif
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
#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
                {
                    static int pr = 0; pr++;
                    if (pr <= 30 || pr % 100 == 0)
                        fprintf(stderr, "[PIF] PAK_READ #%d addr=0x%04X crc=0x%02X data=[%02X %02X %02X %02X...]\n",
                                pr, pak_addr, crc, pak_data[0], pak_data[1], pak_data[2], pak_data[3]);
                }
#endif
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
#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
                {
                    static int pw = 0; pw++;
                    if (pw <= 30 || pw % 100 == 0)
                        fprintf(stderr, "[PIF] PAK_WRITE #%d addr=0x%04X crc=0x%02X data=[%02X %02X %02X %02X...]\n",
                                pw, pak_addr, crc, pak_data[0], pak_data[1], pak_data[2], pak_data[3]);
                }
#endif
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
#if LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
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
#endif

    // Map overlay loading is now done in lod_on_init (before game threads start).
    // See lod_init.cpp for map_ovl_34 loading.

#if LOD_SKIP_EXPANSION_PAK_SCREEN
    // Baseline compatibility skip: native Expansion Pak/high-res selector is
    // unstable in the recomp (presentation bug + high-res-only crash evidence).
    // Skip only this screen, and leave the rest of the boot/gameplay flow to the
    // game. Natural routes continue through gs=5 before reaching the title
    // path; jumping directly to gs=6 is too early and can loop/crash.
    if (direction == 0) {
        uint32_t gsm_addr = *(uint32_t*)(rdram + 0x0C1520);
        if (gsm_addr != 0) {
            uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
            int32_t cur_gs = *(int32_t*)(rdram + gsm_phys + 0x24);
            static int ep_skip_delay = 0;
            static bool ep_skip_done = false;
            if (cur_gs == 12 && !ep_skip_done) {
                ep_skip_delay++;
                if (ep_skip_delay == 60) {
                    // Mirror gamestate_change(5): current_game_state = -5,
                    // delay = 1. Do not touch exitingGameState; bypassing the
                    // intermediate negative state skips loader/overlay setup and
                    // loops back to the Konami/logo path.
                    *(int32_t*)(rdram + gsm_phys + 0x24) = -5;
                    *(uint16_t*)(rdram + gsm_phys + (0x06 ^ 2)) = 1;
                    fprintf(stderr, "[SKIP] gs=12 → -5 (Expansion Pak/native high-res selector)\n");
                    ep_skip_done = true;
                    ep_skip_delay = 0;
                }
            } else if (cur_gs != 12) {
                ep_skip_delay = 0;
            }
        }
    }
#endif

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

#if LOD_ENABLE_SAVE_STATE45_TRACE || LOD_ENABLE_RUNTIME_HEARTBEAT_LOGS
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
#endif

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
