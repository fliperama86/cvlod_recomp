// NI Overlay Runtime Loader
//
// The game uses two TLB-mapped overlay regions:
//   0x0F000000: game object overlays (enemies, UI, etc.) — swap in/out
//   0x0E000000: cutscene/textbox overlays — swap independently
//
// When osMapTLB maps either vaddr, we match the physical address to an NI
// file index, then load the correct recompiled overlay section.

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#if defined(__APPLE__) || defined(__linux__)
#include <dlfcn.h>
#endif

#include "recomp.h"
#include "librecomp/overlays.hpp"

#ifndef LOD_ENABLE_SAVE_PAIR120_TRACE
#define LOD_ENABLE_SAVE_PAIR120_TRACE 0
#endif

#ifndef LOD_ENABLE_GS5_NI_TRACE
#define LOD_ENABLE_GS5_NI_TRACE 0
#endif

#ifndef LOD_ENABLE_NI24_NULL_DISPATCH_TRACE
#define LOD_ENABLE_NI24_NULL_DISPATCH_TRACE 0
#endif

#ifndef LOD_FIX_NI_PAIR120_RESULT_LABELS
#define LOD_FIX_NI_PAIR120_RESULT_LABELS 1
#endif

#ifndef LOD_FIX_NI_PAIR24_INTERNAL_LABELS
#define LOD_FIX_NI_PAIR24_INTERNAL_LABELS 1
#endif

extern "C" void load_overlays(uint32_t rom, int32_t ram_addr, uint32_t size);
extern "C" void unload_overlays(int32_t ram_addr, uint32_t size);

// Full overlay data table (rom offsets + sizes in extended ROM)
#include "ni_ovl_data.h"

// NI pairs: text_ni_index 0x1C5 (453) → pair 0.
// pair_index = (ni_text_index - 0x1C5) / 2
static constexpr int NI_TEXT_INDEX_START = 0x1C5;
static constexpr int NI_PAIR_COUNT = 245;
static constexpr uint32_t NI_OVERLAY_UNLOAD_SIZE = 0x00100000;

// Track separately for 0x0E and 0x0F (they're independent)
static int loaded_0f_pair = -1;
static int loaded_0e_pair = -1;

extern "C" int lod_ni_overlay_loaded_0f_pair() {
    return loaded_0f_pair;
}

extern "C" int lod_ni_overlay_loaded_0e_pair() {
    return loaded_0e_pair;
}

static bool lod_ni_telemetry_range_ok(uint32_t phys, uint32_t size) {
    return size <= 0x800000 && phys <= (0x800000 - size);
}

static uint32_t lod_ni_telemetry_u32(uint8_t* rdram, uint32_t phys) {
    return lod_ni_telemetry_range_ok(phys, 4) ? *(uint32_t*)(rdram + phys) : 0;
}

static int32_t lod_ni_telemetry_gamestate(uint8_t* rdram) {
    constexpr uint32_t GSM_PTR_PHYS = 0x0C1520;
    uint32_t gsm_addr = lod_ni_telemetry_u32(rdram, GSM_PTR_PHYS);
    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (gsm_addr == 0 || !lod_ni_telemetry_range_ok(gsm_phys + 0x24, 4)) {
        return 0;
    }
    return (int32_t)lod_ni_telemetry_u32(rdram, gsm_phys + 0x24);
}

static uint32_t lod_ni_telemetry_exec_flags(uint8_t* rdram) {
    constexpr uint32_t EXEC_FLAGS_PHYS = 0x001CABC8;
    return lod_ni_telemetry_u32(rdram, EXEC_FLAGS_PHYS);
}

static uint32_t lod_ni_telemetry_ni_sys_ptr(uint8_t* rdram) {
    constexpr uint32_t NI_SYS_PTR_PHYS = 0x001CAC1C;
    return lod_ni_telemetry_u32(rdram, NI_SYS_PTR_PHYS);
}

static uint32_t ni_overlay_cpu_mirror(uint32_t vram) {
    if (vram == 0x0F000000) return 0x8F000000;
    if (vram == 0x0E000000) return 0x8E000000;
    return 0;
}

static int* ni_overlay_loaded_pair_for_vram(uint32_t vram) {
    if (vram == 0x0F000000) return &loaded_0f_pair;
    if (vram == 0x0E000000) return &loaded_0e_pair;
    return nullptr;
}

static void unload_ni_overlay_segment(uint32_t vram) {
    int* loaded_pair = ni_overlay_loaded_pair_for_vram(vram);
    if (loaded_pair == nullptr || *loaded_pair < 0) {
        return;
    }

    unload_overlays((int32_t)vram, NI_OVERLAY_UNLOAD_SIZE);
    if (uint32_t mirror_vram = ni_overlay_cpu_mirror(vram)) {
        unload_overlays((int32_t)mirror_vram, NI_OVERLAY_UNLOAD_SIZE);
    }
    *loaded_pair = -1;
}

extern "C" uint32_t ni_overlay_loaded_span(uint32_t vram) {
    int* loaded_pair = ni_overlay_loaded_pair_for_vram(vram);
    if (loaded_pair == nullptr) {
        return 0;
    }

    if (*loaded_pair < 0 || *loaded_pair >= NI_OVL_COUNT) {
        return 0;
    }
    return ni_ovl_data[*loaded_pair].full_size;
}

#if LOD_FIX_NI_PAIR120_RESULT_LABELS
static const char* lod_ni_pair120_internal_label_name(uint32_t target) {
    switch (target) {
        case 0x0F00086C: return "pair120_label_clear";
        case 0x0F0008D4: return "pair120_label_10b";
        case 0x0F0008E4: return "pair120_label_10a";
        case 0x0F000958: return "pair120_label_obj3c";
        case 0x0F000964: return "pair120_label_111";
        case 0x0F000974: return "pair120_label_112";
        default: return nullptr;
    }
}

static bool lod_ni_pair120_call_internal_result_label(uint32_t target, uint8_t* rdram,
                                                       recomp_context* ctx) {
    switch (target) {
        case 0x0F00086C:
            // Original internal label 0x0F00086C inside ni_ovl_120_func_0F000860:
            //   sw a0, 0(sp); sh -1, 0x801CAE1C; v0 = 0; jr ra
            ctx->r14 = ADD32(0, -1);
            ctx->r1 = S32(0x801D << 16);
            MEM_W(0x0, ctx->r29) = ctx->r4;
            MEM_H(-0x51E4, ctx->r1) = (int16_t)-1;
            ctx->r2 = 0;
            return true;
        case 0x0F0008D4:
            ctx->r14 = ADD32(0, 0x10B);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x10B);
            return true;
        case 0x0F0008E4:
            ctx->r14 = ADD32(0, 0x10A);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x10A);
            return true;
        case 0x0F000958:
            ctx->r2 = MEM_W(0x3C, ctx->r4);
            MEM_W(0x38, ctx->r4) = ctx->r2;
            return true;
        case 0x0F000964:
            ctx->r14 = ADD32(0, 0x111);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x111);
            return true;
        case 0x0F000974:
            ctx->r14 = ADD32(0, 0x112);
            MEM_W(0x38, ctx->r4) = ctx->r14;
            ctx->r2 = ADD32(0, 0x112);
            return true;
        default:
            return false;
    }
}

static void lod_call_ni_pair120_result_callback(uint8_t* rdram, recomp_context* ctx,
                                                 uint32_t target) {
    if (!lod_ni_pair120_call_internal_result_label(target, rdram, ctx)) {
        LOOKUP_FUNC((int32_t)target)(rdram, ctx);
    }
}

static void lod_fixed_ni_pair120_result_schedule_dispatch(uint8_t* rdram, recomp_context* ctx) {
    // Scoped compatibility shim for ni_ovl_120_result_schedule_dispatch.
    // The pair-120 result table at 0x0F0016BC intentionally contains a mix of
    // function starts and internal labels. N64Recomp's range fallback resolves
    // internal-label targets to their containing function starts, which changes
    // callback return values and can reset the save/Controller Pak state machine.
    ctx->r29 = ADD32(ctx->r29, -0x18);
    MEM_W(0x14, ctx->r29) = ctx->r31;
    MEM_W(0x18, ctx->r29) = ctx->r4;

    ctx->r14 = MEM_W(ctx->r4, 0x48);
    ctx->r25 = S32(0x0F00 << 16);
    ctx->r15 = S32(ctx->r14 << 2);
    ctx->r25 = ADD32(ctx->r25, ctx->r15);
    ctx->r25 = MEM_W(ctx->r25, 0x16BC);

    lod_call_ni_pair120_result_callback(rdram, ctx, (uint32_t)ctx->r25);

    ctx->r7 = MEM_W(ctx->r29, 0x18);
    ctx->r24 = ADD32(0, 1);
    ctx->r25 = S32(0x8000 << 16);
    ctx->r3 = ADD32(ctx->r7, 0x34);
    MEM_W(0x4, ctx->r3) = ctx->r2;

    if (ctx->r2 != 0) {
        ctx->r25 = ADD32(S32(0x8000 << 16), 0x1E30);
        ctx->r4 = ADD32(ctx->r7, 0x8);
        ctx->r5 = ADD32(ctx->r7, 0xE);
        ctx->r6 = 0;
        LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        ctx->r31 = MEM_W(ctx->r29, 0x14);
    } else {
        ctx->r25 = ADD32(ctx->r25, 0x1CE8);
        MEM_W(0x10, ctx->r3) = ctx->r24;
        ctx->r4 = ADD32(ctx->r7, 0x8);
        ctx->r5 = ADD32(ctx->r7, 0xE);
        LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        ctx->r31 = MEM_W(ctx->r29, 0x14);
    }

    ctx->r29 = ADD32(ctx->r29, 0x18);
}

static void lod_install_ni_pair120_dispatch_fix(const char* reason) {
    recomp::overlays::add_loaded_function((int32_t)0x0F000484,
        lod_fixed_ni_pair120_result_schedule_dispatch);

#if LOD_ENABLE_SAVE_PAIR120_TRACE
    static int install_count = 0;
    install_count++;
    if (install_count <= 8 || (install_count % 100) == 0) {
        fprintf(stderr,
            "[PAIR120-FIX] installed scoped result dispatcher fix #%d reason=%s\n",
            install_count, reason);
    }
#else
    (void)reason;
#endif
}
#endif

#if LOD_FIX_NI_PAIR24_INTERNAL_LABELS
static void lod_ni_pair24_internal_label_0F00ADEC(uint8_t* rdram, recomp_context* ctx) {
    // Original internal label 0x0F00ADEC inside ni_ovl_024_func_0F00ACE4.
    // Pair-24's dispatch table intentionally targets this label for state 4.
    // N64Recomp's range fallback otherwise resolves 0x0F00ADEC to the containing
    // function start (0x0F00ACE4), which advances the object from state 4 to the
    // nonexistent state 5 and prevents the Fog Lake platform event from firing.
    ctx->r2 = MEM_W(ctx->r4, 0x24);
    ctx->r25 = ADD32(0, 0x2);
    ctx->r14 = MEM_BU(ctx->r2, 0x1B);
    ctx->r15 = ADD32(ctx->r14, -0xF);
    ctx->r24 = ctx->r15 & 0xFF;
    ctx->r1 = SIGNED(ctx->r24) < 0x14 ? 1 : 0;

    // Delay slot of the branch at 0x0F00AE04 executes regardless of branch.
    MEM_B(0x1B, ctx->r2) = ctx->r15;

    if (ctx->r1 != 0) {
        MEM_B(0x1B, ctx->r2) = 0;
        MEM_B(0x9, ctx->r4) = ctx->r25;
    }
}

static void lod_install_ni_pair24_internal_label_fix(const char* reason) {
    recomp::overlays::add_loaded_function((int32_t)0x0F00ADEC,
        lod_ni_pair24_internal_label_0F00ADEC);

    static int install_count = 0;
    install_count++;
    if (install_count == 1) {
        fprintf(stderr,
            "[PAIR24-FIX] installed internal label alias 0x0F00ADEC reason=%s\n",
            reason);
    }
}
#endif

#if LOD_ENABLE_SAVE_PAIR120_TRACE
static recomp_func_t* lod_orig_ni_pair120_result_schedule_dispatch = nullptr;

static bool lod_pair120_rdram_range_ok(uint32_t phys, uint32_t size) {
    return phys < 0x00800000 && size <= 0x00800000 && phys <= 0x00800000 - size;
}

static uint32_t lod_pair120_rdram_u32(uint8_t* rdram, uint32_t phys) {
    if (!lod_pair120_rdram_range_ok(phys, 4)) {
        return 0;
    }
    return *(uint32_t*)(rdram + phys);
}

static uint8_t lod_pair120_rdram_u8(uint8_t* rdram, uint32_t phys) {
    if (!lod_pair120_rdram_range_ok(phys, 1)) {
        return 0;
    }
    return rdram[phys ^ 3];
}

static int16_t lod_pair120_rdram_s16(uint8_t* rdram, uint32_t phys) {
    if (!lod_pair120_rdram_range_ok(phys, 2)) {
        return 0;
    }
    return *(int16_t*)(rdram + (phys ^ 2));
}

static int32_t lod_pair120_current_gamestate(uint8_t* rdram) {
    constexpr uint32_t GSM_PTR_PHYS = 0x0C1520; // 0x800C1520
    uint32_t gsm_addr = lod_pair120_rdram_u32(rdram, GSM_PTR_PHYS);
    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (gsm_addr == 0 || !lod_pair120_rdram_range_ok(gsm_phys + 0x24, 4)) {
        return -1;
    }
    return (int32_t)lod_pair120_rdram_u32(rdram, gsm_phys + 0x24);
}

static void lod_pair120_host_symbol(recomp_func_t* func, const char** symbol_out,
                                    uintptr_t* offset_out) {
    const char* symbol = "(unknown)";
    uintptr_t offset = 0;
#if defined(__APPLE__) || defined(__linux__)
    Dl_info info = {};
    if (func != nullptr && dladdr((void*)func, &info) != 0) {
        if (info.dli_sname != nullptr) {
            symbol = info.dli_sname;
        }
        uintptr_t sym_base = (uintptr_t)info.dli_saddr;
        uintptr_t addr = (uintptr_t)func;
        if (sym_base != 0 && addr >= sym_base) {
            offset = addr - sym_base;
        }
    }
#endif
    *symbol_out = symbol;
    *offset_out = offset;
}

static void lod_trace_ni_pair120_result_schedule_dispatch(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    count++;

    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj & 0x1FFFFFFF;
    bool obj_ok = lod_pair120_rdram_range_ok(obj_phys, 0x74);
    int32_t gs = lod_pair120_current_gamestate(rdram);

    uint8_t before_state = obj_ok ? lod_pair120_rdram_u8(rdram, obj_phys + 0x09) : 255;
    int16_t before_func_id = obj_ok ? lod_pair120_rdram_s16(rdram, obj_phys + 0x0E) : 0;
    uint32_t before_38 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x38) : 0;
    uint32_t before_44 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x44) : 0;
    uint32_t before_48 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x48) : 0;
    uint32_t callback_target = 0;
    recomp_func_t* callback_host = nullptr;
    if (before_48 < 64) {
        callback_target = (uint32_t)MEM_W(0x16BC + before_48 * 4, (gpr)(int32_t)0x0F000000);
        if (callback_target >= 0x0F000000 && callback_target < 0x0F010000) {
            callback_host = get_function((int32_t)callback_target);
        }
    }

    const char* callback_symbol = "(unknown)";
    uintptr_t callback_offset = 0;
#if LOD_FIX_NI_PAIR120_RESULT_LABELS
    if (const char* label_name = lod_ni_pair120_internal_label_name(callback_target)) {
        callback_symbol = label_name;
    } else
#endif
    {
        lod_pair120_host_symbol(callback_host, &callback_symbol, &callback_offset);
    }

    bool interesting = obj_ok && obj_phys >= 0x31A000 && obj_phys < 0x31D000;
    bool should_log = count <= 32 || (gs == 4 && interesting);
    if (should_log) {
        fprintf(stderr,
            "[PAIR120] enter #%d gs=%d obj=0x%08X state09=%u func_id=%d "
            "idx_pre=%u cb=0x%08X cb_host=%p cb_symbol=%s+0x%zX "
            "obj38_pre=0x%08X obj44_pre=0x%08X obj48_pre=0x%08X ra=0x%08X sp=0x%08X\n",
            count, gs, obj, before_state, before_func_id, before_48,
            callback_target, (void*)callback_host, callback_symbol, (size_t)callback_offset,
            before_38, before_44, before_48, (uint32_t)ctx->r31, (uint32_t)ctx->r29);
    }

    if (lod_orig_ni_pair120_result_schedule_dispatch != nullptr) {
        lod_orig_ni_pair120_result_schedule_dispatch(rdram, ctx);
    }

    if (should_log) {
        uint8_t after_state = obj_ok ? lod_pair120_rdram_u8(rdram, obj_phys + 0x09) : 255;
        int16_t after_func_id = obj_ok ? lod_pair120_rdram_s16(rdram, obj_phys + 0x0E) : 0;
        uint32_t after_38 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x38) : 0;
        uint32_t after_44 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x44) : 0;
        uint32_t after_48 = obj_ok ? lod_pair120_rdram_u32(rdram, obj_phys + 0x48) : 0;
        const char* branch = after_38 != 0 ? "reset-nonzero" : "advance-zero";
        fprintf(stderr,
            "[PAIR120] exit #%d gs=%d obj=0x%08X state09=%u->%u func_id=%d->%d "
            "idx_pre=%u cb=0x%08X result_obj38=0x%08X obj44=0x%08X obj48=0x%08X branch=%s v0_after=0x%08X ra=0x%08X\n",
            count, lod_pair120_current_gamestate(rdram), obj, before_state, after_state,
            before_func_id, after_func_id, before_48, callback_target,
            after_38, after_44, after_48, branch, (uint32_t)ctx->r2, (uint32_t)ctx->r31);
    }
}

static void lod_install_ni_pair120_trace_wrapper(const char* reason) {
#if LOD_FIX_NI_PAIR120_RESULT_LABELS
    lod_orig_ni_pair120_result_schedule_dispatch =
        lod_fixed_ni_pair120_result_schedule_dispatch;
#else
    recomp_func_t* current = get_function((int32_t)0x0F000484);
    if (current != lod_trace_ni_pair120_result_schedule_dispatch) {
        lod_orig_ni_pair120_result_schedule_dispatch = current;
    }
#endif
    recomp::overlays::add_loaded_function((int32_t)0x0F000484,
        lod_trace_ni_pair120_result_schedule_dispatch);

    static int install_count = 0;
    install_count++;
    if (install_count <= 8 || (install_count % 100) == 0) {
        fprintf(stderr,
            "[PAIR120] installed result dispatcher trace #%d reason=%s original=%p\n",
            install_count, reason, (void*)lod_orig_ni_pair120_result_schedule_dispatch);
    }
}
#endif

#if LOD_ENABLE_GS5_NI_TRACE
static recomp_func_t* lod_orig_gs5_0f_dispatch = nullptr;
static recomp_func_t* lod_orig_gs5_0e_dispatch = nullptr;
static int lod_trace_0f_pair = -1;
static int lod_trace_0e_pair = -1;

static bool lod_gs5_trace_rdram_ok(uint32_t phys, uint32_t size) {
    return phys < 0x00800000 && size <= 0x00800000 && phys <= 0x00800000 - size;
}

static uint8_t lod_gs5_trace_u8(uint8_t* rdram, uint32_t phys) {
    if (!lod_gs5_trace_rdram_ok(phys, 1)) {
        return 0;
    }
    return rdram[phys ^ 3];
}

static int16_t lod_gs5_trace_s16(uint8_t* rdram, uint32_t phys) {
    if (!lod_gs5_trace_rdram_ok(phys, 2)) {
        return 0;
    }
    return *(int16_t*)(rdram + (phys ^ 2));
}

static uint32_t lod_gs5_trace_u32(uint8_t* rdram, uint32_t phys) {
    if (!lod_gs5_trace_rdram_ok(phys, 4)) {
        return 0;
    }
    return *(uint32_t*)(rdram + phys);
}

static int32_t lod_gs5_trace_current_gamestate(uint8_t* rdram) {
    uint32_t gsm_addr = lod_gs5_trace_u32(rdram, 0x0C1520);
    uint32_t gsm_phys = gsm_addr & 0x1FFFFFFF;
    if (gsm_addr == 0 || !lod_gs5_trace_rdram_ok(gsm_phys + 0x24, 4)) {
        return -1;
    }
    return (int32_t)lod_gs5_trace_u32(rdram, gsm_phys + 0x24);
}

static uint32_t lod_gs5_trace_dispatch_table_offset(int pair) {
    switch (pair) {
        case 53:  return 0x2120;
        case 104: return 0x26F8;
        case 149: return 0x1944;
        default:  return 0;
    }
}

static const char* lod_gs5_trace_pair_label(int pair) {
    switch (pair) {
        case 53:  return "pair53";
        case 104: return "pair104";
        case 149: return "pair149";
        default:  return "pair?";
    }
}

static uint32_t lod_gs5_trace_table_target(uint8_t* rdram, int pair, uint32_t vram, uint8_t index) {
    uint32_t table = lod_gs5_trace_dispatch_table_offset(pair);
    if (table == 0 || index >= 64) {
        return 0;
    }
    return (uint32_t)MEM_W(table + index * 4, (gpr)(int32_t)vram);
}

static void lod_trace_gs5_ni_dispatch_common(uint8_t* rdram, recomp_context* ctx,
                                             int pair, uint32_t vram,
                                             recomp_func_t* original,
                                             const char* region_label) {
    static int trace_count = 0;
    trace_count++;

    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = obj & 0x1FFFFFFF;
    bool obj_ok = lod_gs5_trace_rdram_ok(obj_phys, 0x74);
    int32_t gs = lod_gs5_trace_current_gamestate(rdram);
    int16_t before_func = obj_ok ? lod_gs5_trace_s16(rdram, obj_phys + 0x0E) : 0;
    int before_level = before_func + 1;
    uint32_t before_slot = (before_level >= 0 && before_level < 3) ? obj_phys + 0x08 + before_level * 2 : 0;
    uint8_t before_timer = before_slot ? lod_gs5_trace_u8(rdram, before_slot + 0) : 0xFF;
    uint8_t before_state = before_slot ? lod_gs5_trace_u8(rdram, before_slot + 1) : 0xFF;
    uint32_t before_target = lod_gs5_trace_table_target(rdram, pair, vram, before_state);
    uint32_t obj24 = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x24) : 0;
    uint32_t obj34 = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x34) : 0;
    uint32_t obj38 = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x38) : 0;
    uint32_t obj40 = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x40) : 0;
    uint32_t obj44 = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x44) : 0;
    uint32_t obj48 = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x48) : 0;
    int16_t obj4c = obj_ok ? lod_gs5_trace_s16(rdram, obj_phys + 0x4C) : 0;
    uint32_t obj50 = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x50) : 0;
    uint32_t obj5c = obj_ok ? lod_gs5_trace_u32(rdram, obj_phys + 0x5C) : 0;
    uint32_t sys_2968 = lod_gs5_trace_u32(rdram, 0x1CAC28);
    uint32_t sys_2b10 = lod_gs5_trace_u32(rdram, 0x1CADD0);
    uint32_t sys_2bd0 = lod_gs5_trace_u32(rdram, 0x1CAE90);
    uint32_t sys_input = lod_gs5_trace_u32(rdram, 0x1C87F8);
    uint32_t data0 = 0;
    uint32_t data0_24 = 0;
    uint32_t data0_34 = 0;
    uint32_t obj34_phys = obj34 & 0x1FFFFFFF;
    if (lod_gs5_trace_rdram_ok(obj34_phys, 0x10)) {
        data0 = lod_gs5_trace_u32(rdram, obj34_phys + 0x00);
        uint32_t data0_phys = data0 & 0x1FFFFFFF;
        if (lod_gs5_trace_rdram_ok(data0_phys, 0x38)) {
            data0_24 = lod_gs5_trace_u32(rdram, data0_phys + 0x24);
            data0_34 = lod_gs5_trace_u32(rdram, data0_phys + 0x34);
        }
    }

    bool should_log = (gs == 5 && (trace_count <= 80 || (trace_count % 500) == 0)) ||
                      trace_count <= 16;
    if (should_log) {
        fprintf(stderr,
            "[GS5-NI] enter #%d %s/%s gs=%d obj=0x%08X ok=%d func=%d level=%d timer=%u state=%u target=0x%08X "
            "obj24=0x%08X obj34=0x%08X obj38=0x%08X obj40=0x%08X obj44=0x%08X obj48=0x%08X obj4C=%d obj50=0x%08X obj5C=0x%08X "
            "sys2968=0x%08X sys2B10=0x%08X sys2BD0=0x%08X input=0x%08X data0=0x%08X data0+24=0x%08X data0+34=0x%08X "
            "ra=0x%08X sp=0x%08X\n",
            trace_count, region_label, lod_gs5_trace_pair_label(pair), gs, obj, obj_ok,
            before_func, before_level, before_timer, before_state, before_target,
            obj24, obj34, obj38, obj40, obj44, obj48, obj4c, obj50, obj5c,
            sys_2968, sys_2b10, sys_2bd0, sys_input, data0, data0_24, data0_34,
            (uint32_t)ctx->r31, (uint32_t)ctx->r29);
    }

    if (original != nullptr) {
        original(rdram, ctx);
    }

    if (should_log) {
        int16_t after_func = obj_ok ? lod_gs5_trace_s16(rdram, obj_phys + 0x0E) : 0;
        int after_level = after_func + 1;
        uint32_t after_slot = (after_level >= 0 && after_level < 3) ? obj_phys + 0x08 + after_level * 2 : 0;
        uint8_t after_timer = after_slot ? lod_gs5_trace_u8(rdram, after_slot + 0) : 0xFF;
        uint8_t after_state = after_slot ? lod_gs5_trace_u8(rdram, after_slot + 1) : 0xFF;
        fprintf(stderr,
            "[GS5-NI] exit  #%d %s/%s gs=%d obj=0x%08X func=%d->%d level=%d->%d timer=%u->%u state=%u->%u v0=0x%08X ra=0x%08X\n",
            trace_count, region_label, lod_gs5_trace_pair_label(pair),
            lod_gs5_trace_current_gamestate(rdram), obj,
            before_func, after_func, before_level, after_level,
            before_timer, after_timer, before_state, after_state,
            (uint32_t)ctx->r2, (uint32_t)ctx->r31);
    }
}

static void lod_trace_gs5_0f_dispatch(uint8_t* rdram, recomp_context* ctx) {
    lod_trace_gs5_ni_dispatch_common(rdram, ctx, lod_trace_0f_pair, 0x0F000000,
                                     lod_orig_gs5_0f_dispatch, "0F");
}

static void lod_trace_gs5_0e_dispatch(uint8_t* rdram, recomp_context* ctx) {
    lod_trace_gs5_ni_dispatch_common(rdram, ctx, lod_trace_0e_pair, 0x0E000000,
                                     lod_orig_gs5_0e_dispatch, "0E");
}

static void lod_install_gs5_ni_trace_wrapper(int pair_index, uint32_t vram) {
    if (pair_index != 53 && pair_index != 104 && pair_index != 149) {
        return;
    }

    recomp_func_t* current = get_function((int32_t)vram);
    if (vram == 0x0F000000) {
        if (current != lod_trace_gs5_0f_dispatch) {
            lod_orig_gs5_0f_dispatch = current;
        }
        lod_trace_0f_pair = pair_index;
        recomp::overlays::add_loaded_function((int32_t)vram, lod_trace_gs5_0f_dispatch);
    } else {
        if (current != lod_trace_gs5_0e_dispatch) {
            lod_orig_gs5_0e_dispatch = current;
        }
        lod_trace_0e_pair = pair_index;
        recomp::overlays::add_loaded_function((int32_t)vram, lod_trace_gs5_0e_dispatch);
    }

    static int install_count = 0;
    install_count++;
    if (install_count <= 32 || (install_count % 500) == 0) {
        fprintf(stderr,
            "[GS5-NI] installed trace #%d %s pair=%d vram=0x%08X original=%p\n",
            install_count, vram == 0x0F000000 ? "0F" : "0E",
            pair_index, vram, (void*)current);
    }
}
#endif

#if LOD_ENABLE_NI24_NULL_DISPATCH_TRACE
static recomp_func_t* lod_orig_ni24_func_0F00A8DC = nullptr;

static bool lod_ni24_rdram_ok(uint32_t phys, uint32_t size) {
    return phys < 0x00800000 && size <= 0x00800000 && phys <= 0x00800000 - size;
}

static uint32_t lod_ni24_rdram_u32(uint8_t* rdram, uint32_t phys) {
    return lod_ni24_rdram_ok(phys, 4) ? *(uint32_t*)(rdram + phys) : 0;
}

static uint8_t lod_ni24_rdram_u8(uint8_t* rdram, uint32_t phys) {
    return lod_ni24_rdram_ok(phys, 1) ? rdram[phys ^ 3] : 0;
}

static int16_t lod_ni24_rdram_s16(uint8_t* rdram, uint32_t phys) {
    return lod_ni24_rdram_ok(phys, 2) ? *(int16_t*)(rdram + (phys ^ 2)) : 0;
}

static uint32_t lod_ni24_table_entry(uint8_t* rdram, uint8_t index) {
    return (uint32_t)MEM_W(0x2E74 + (uint32_t)index * 4,
                           (gpr)(int32_t)0x0F010000);
}

static void lod_log_ni24_dispatch(uint8_t* rdram, recomp_context* ctx,
                                  gpr obj, int16_t func_before,
                                  int16_t func_next, gpr slot_vaddr,
                                  uint8_t timer_before, uint8_t timer_after,
                                  uint8_t dispatch_index, uint32_t target) {
    static int dispatch_count = 0;
    static int null_count = 0;
    dispatch_count++;
    if (target == 0) {
        null_count++;
    }

    bool should_log = dispatch_count <= 8 ||
                      (target == 0 && (null_count <= 3 || (null_count % 2000) == 0));
    if (!should_log) {
        return;
    }

    uint32_t obj_u32 = (uint32_t)obj;
    uint32_t obj_phys = obj_u32 & 0x1FFFFFFF;
    bool obj_ok = lod_ni24_rdram_ok(obj_phys, 0x74);
    uint32_t sp_phys = (uint32_t)ctx->r29 & 0x1FFFFFFF;
    uint32_t obj34 = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x34) : 0;
    uint32_t obj38 = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x38) : 0;
    uint32_t obj3c = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x3C) : 0;
    uint32_t obj40 = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x40) : 0;
    uint32_t obj44 = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x44) : 0;
    uint32_t obj48 = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x48) : 0;
    uint32_t obj5c = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x5C) : 0;
    uint32_t obj60 = obj_ok ? lod_ni24_rdram_u32(rdram, obj_phys + 0x60) : 0;
    uint32_t saved_v1 = lod_ni24_rdram_ok(sp_phys + 0x1C, 4) ?
        lod_ni24_rdram_u32(rdram, sp_phys + 0x1C) : 0;

    fprintf(stderr,
        "[NI24_DISPATCH] #%d null#%d gs=%d loaded0f=%d loaded0e=%d "
        "obj=0x%08X ok=%d func=%d->%d slot=0x%08X timer=%u->%u idx=%u target=0x%08X "
        "obj34=0x%08X obj38=0x%08X obj3C=0x%08X obj40=0x%08X obj44=0x%08X "
        "obj48=0x%08X obj5C=0x%08X obj60=0x%08X saved_v1=0x%08X "
        "ra=0x%08X sp=0x%08X "
        "table[0..15]=%08X,%08X,%08X,%08X,%08X,%08X,%08X,%08X,"
        "%08X,%08X,%08X,%08X,%08X,%08X,%08X,%08X\n",
        dispatch_count, null_count, lod_ni_telemetry_gamestate(rdram),
        loaded_0f_pair, loaded_0e_pair,
        obj_u32, obj_ok, func_before, func_next, (uint32_t)slot_vaddr, timer_before, timer_after,
        dispatch_index, target,
        obj34, obj38, obj3c, obj40, obj44, obj48, obj5c, obj60, saved_v1,
        (uint32_t)ctx->r31, (uint32_t)ctx->r29,
        lod_ni24_table_entry(rdram, 0), lod_ni24_table_entry(rdram, 1),
        lod_ni24_table_entry(rdram, 2), lod_ni24_table_entry(rdram, 3),
        lod_ni24_table_entry(rdram, 4), lod_ni24_table_entry(rdram, 5),
        lod_ni24_table_entry(rdram, 6), lod_ni24_table_entry(rdram, 7),
        lod_ni24_table_entry(rdram, 8), lod_ni24_table_entry(rdram, 9),
        lod_ni24_table_entry(rdram, 10), lod_ni24_table_entry(rdram, 11),
        lod_ni24_table_entry(rdram, 12), lod_ni24_table_entry(rdram, 13),
        lod_ni24_table_entry(rdram, 14), lod_ni24_table_entry(rdram, 15));
}

static void lod_trace_ni24_func_0F00A8DC(uint8_t* rdram, recomp_context* ctx) {
    // Precompute the table dispatch that the original function reaches after
    // calling func_80045668. Do not hand-copy the generated function here: the
    // original must run unchanged so this trace cannot perturb stack/register
    // sign-extension semantics.
    gpr obj = ctx->r4;
    uint32_t obj_phys = (uint32_t)obj & 0x1FFFFFFF;
    bool obj_ok = lod_ni24_rdram_ok(obj_phys, 0x74);
    int16_t func_before = obj_ok ? lod_ni24_rdram_s16(rdram, obj_phys + 0x0E) : 0;
    int16_t func_next = (int16_t)(func_before + 1);
    gpr slot_vaddr = ADD32(obj, (int32_t)func_next * 2);
    uint32_t slot_phys = (uint32_t)slot_vaddr & 0x1FFFFFFF;
    uint8_t dispatch_index = lod_ni24_rdram_ok(slot_phys + 0x09, 1) ?
        lod_ni24_rdram_u8(rdram, slot_phys + 0x09) : 0xFF;
    uint8_t timer_before = lod_ni24_rdram_ok(slot_phys + 0x08, 1) ?
        lod_ni24_rdram_u8(rdram, slot_phys + 0x08) : 0xFF;
    uint32_t target = dispatch_index != 0xFF ?
        lod_ni24_table_entry(rdram, dispatch_index) : 0;
    lod_log_ni24_dispatch(rdram, ctx, obj, func_before, func_next, slot_vaddr,
                          timer_before, (uint8_t)(timer_before + 1),
                          dispatch_index, target);

    if (lod_orig_ni24_func_0F00A8DC != nullptr) {
        lod_orig_ni24_func_0F00A8DC(rdram, ctx);
    }
}

static void lod_install_ni24_null_dispatch_trace_wrapper(int pair_index, uint32_t vram,
                                                         const char* reason) {
    if (pair_index != 24 || vram != 0x0F000000) {
        return;
    }

    recomp_func_t* current = get_function((int32_t)0x0F00A8DC);
    if (current != lod_trace_ni24_func_0F00A8DC) {
        lod_orig_ni24_func_0F00A8DC = current;
    }

    recomp::overlays::add_loaded_function((int32_t)0x0F00A8DC,
        lod_trace_ni24_func_0F00A8DC);

    static int install_count = 0;
    install_count++;
    if (install_count == 1) {
        fprintf(stderr,
            "[NI24_DISPATCH] installed null-dispatch trace #%d reason=%s pair=%d "
            "vram=0x%08X original=%p\n",
            install_count, reason, pair_index, vram,
            (void*)lod_orig_ni24_func_0F00A8DC);
    }
}
#endif

static int ni_index_to_pair(int ni_index) {
    if (ni_index < NI_TEXT_INDEX_START) return -1;
    int offset = ni_index - NI_TEXT_INDEX_START;
    if (offset % 2 != 0) return -1;
    int pair = offset / 2;
    if (pair >= NI_PAIR_COUNT) return -1;
    return pair;
}

// Copy the full overlay data (text+data) from the extended ROM to the
// segment region in RDRAM. The extended ROM is mapped at rdram+0x10000000
// and contains each overlay's text+data at the original compiler layout.
// The segment region (rdram+0x8F000000 for 0x0F, rdram+0x8E000000 for 0x0E)
// is what MEM_W(0x0F00XXXX) resolves to in recompiled code.
static void copy_overlay_data_to_segment(uint8_t* rdram, int pair_index, uint32_t vram) {
    if (pair_index < 0 || pair_index >= NI_OVL_COUNT) return;

    const NiOvlData& data = ni_ovl_data[pair_index];
    // Extended ROM is mapped at rdram + 0x10000000 + rom_offset
    uint8_t* src = rdram + 0x10000000 + data.rom_offset;
    // Segment region: MEM_W(vram) → rdram + vram + 0x80000000
    uint8_t* dst = rdram + (uint64_t)vram + 0x80000000ULL;
    uint32_t size = data.full_size;

    // Ensure the segment region is writable (may extend beyond TLB-mapped pages)
    uintptr_t page_mask = sysconf(_SC_PAGESIZE) - 1;
    uint8_t* aligned = (uint8_t*)((uintptr_t)dst & ~page_mask);
    size_t aligned_size = ((dst + size) - aligned + page_mask) & ~page_mask;
    mprotect(aligned, aligned_size, PROT_READ | PROT_WRITE);

    memcpy(dst, src, size);
}

static void load_ni_overlay(uint8_t* rdram, int pair_index, uint32_t mapped_vaddr) {
    uint32_t vram = mapped_vaddr;
    int& loaded_pair = (vram == 0x0E000000) ? loaded_0e_pair : loaded_0f_pair;

    // Always keep the actual TLB-mapped segment populated. Use the TLB vaddr as
    // the source of truth: some overlays generated with 0x0F comments are
    // executed through 0x0E in-game, and their dispatch tables contain 0x0E
    // function pointers (for example pair 233).
    copy_overlay_data_to_segment(rdram, pair_index, mapped_vaddr);

    if (pair_index == loaded_pair) return;

    if (loaded_pair >= 0) {
        unload_ni_overlay_segment(vram);
    }

    const NiOvlData& data = ni_ovl_data[pair_index];
    load_overlays(data.rom_offset, (int32_t)vram, data.full_size);
    if (uint32_t mirror_vram = ni_overlay_cpu_mirror(vram)) {
        // Some computed dispatches use the MEM_W/CPU segment mirror
        // (0x8Fxxxxxx or 0x8Exxxxxx) after osMapTLB populates the segment
        // region. Register mirror aliases for the currently loaded NI section
        // so those targets resolve without changing the runtime lookup code.
        load_overlays(data.rom_offset, (int32_t)mirror_vram, data.full_size);
    }
    // Copy full overlay data (text+data) from extended ROM to the executable
    // segment region. This is intentionally after load_overlays as well as
    // before the early-return path so remaps keep the segment bytes fresh.
    copy_overlay_data_to_segment(rdram, pair_index, vram);
    loaded_pair = pair_index;

#if LOD_FIX_NI_PAIR120_RESULT_LABELS
    if (pair_index == 120) {
        lod_install_ni_pair120_dispatch_fix("pair120-load");
    }
#endif
#if LOD_FIX_NI_PAIR24_INTERNAL_LABELS
    if (pair_index == 24 && vram == 0x0F000000) {
        lod_install_ni_pair24_internal_label_fix("pair24-load");
    }
#endif
#if LOD_ENABLE_SAVE_PAIR120_TRACE
    if (pair_index == 120) {
        lod_install_ni_pair120_trace_wrapper("pair120-load");
    }
#endif
#if LOD_ENABLE_GS5_NI_TRACE
    lod_install_gs5_ni_trace_wrapper(pair_index, vram);
#endif
#if LOD_ENABLE_NI24_NULL_DISPATCH_TRACE
    lod_install_ni24_null_dispatch_trace_wrapper(pair_index, vram, "pair24-load");
#endif

    static int load_count = 0;
    load_count++;
    if (load_count <= 30 || (load_count % 100) == 0) {
        fprintf(stderr, "[ni_ovl] #%d pair %d → 0x%08X mapped=0x%08X (rom=0x%08X data=0x%X)\n",
                load_count, pair_index, vram, mapped_vaddr, data.rom_offset, data.full_size);
    }
}

// Fingerprint table for overlay identification
#include "ni_ovl_fingerprints.h"

// Match decompressed data at paddr against known overlay fingerprints.
// The game decompresses NI files to its own heap in low RDRAM, so we
// read the first 128 bytes and compare against precomputed signatures.
static int match_overlay_fingerprint(uint8_t* rdram, uint32_t paddr) {
    if (paddr + 128 > 0x00800000) return -1;  // sanity: within 8MB RDRAM

    // Read 32 big-endian words from the decompressed data.
    // The data in RDRAM is byte-swapped (LE), so we read as LE uint32
    // and the fingerprints are stored as BE. We need to match what the
    // game sees, which is the byte-swapped version in RDRAM.
    // Actually: RDRAM stores data in the host's native byte order after
    // the byte-swap in lod_init. The fingerprints are from the raw BE
    // decompressed data. We need to byte-swap when comparing.
    const uint32_t* data = (const uint32_t*)(rdram + paddr);

    for (int i = 0; i < NI_FP_COUNT; i++) {
        bool match = true;
        for (int j = 0; j < NI_FP_WORDS; j++) {
            // RDRAM stores decompressed data as-is (BE words read as uint32)
            uint32_t rdram_word = data[j];
            if (rdram_word != ni_fingerprints[i].words[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return ni_fingerprints[i].pair_index;
        }
    }
    return -1;
}

// RT64 segment overrides for TLB-mapped segments
extern "C" uint32_t g_tlb_segment_0e;
extern "C" uint32_t g_tlb_segment_0f;

// Called from osMapTLB hook when vaddr is 0x0E000000 or 0x0F000000
extern "C" bool ni_overlay_on_tlb_map(uint8_t* rdram, uint32_t vaddr,
                                       uint32_t even_paddr, uint32_t odd_paddr) {
    if (vaddr != 0x0F000000 && vaddr != 0x0E000000) return false;

    // Set RT64 segment override to the MEM_W region (0x8F/0x8E000000).
    // osVirtualToPhysical passes 0x0F/0x0E addresses through raw, so the DL
    // has 0x0F-style addresses. RT64 resolves via this override.
    if (vaddr == 0x0F000000) g_tlb_segment_0f = 0x8F000000;
    else                     g_tlb_segment_0e = 0x8E000000;

    int pair = match_overlay_fingerprint(rdram, even_paddr);
    if (pair >= 0) {
        load_ni_overlay(rdram, pair, vaddr);
        return true;
    }

    static int miss_count = 0;
    miss_count++;
    if (miss_count <= 10) {
        int loaded_pair = (vaddr == 0x0F000000) ? loaded_0f_pair : loaded_0e_pair;
        uint32_t loaded_rom = 0;
        uint32_t loaded_size = 0;
        if (loaded_pair >= 0 && loaded_pair < NI_OVL_COUNT) {
            loaded_rom = ni_ovl_data[loaded_pair].rom_offset;
            loaded_size = ni_ovl_data[loaded_pair].full_size;
        }
        fprintf(stderr,
                "[ni_ovl] WARNING: miss#%d TLB map 0x%08X even=0x%08X odd=0x%08X — no matching overlay; "
                "loaded_pair=%d loaded_rom=0x%08X loaded_size=0x%X loaded0f=%d loaded0e=%d "
                "gs=%d exec=0x%08X ni=0x%08X\n",
                miss_count, vaddr, even_paddr, odd_paddr,
                loaded_pair, loaded_rom, loaded_size, loaded_0f_pair, loaded_0e_pair,
                lod_ni_telemetry_gamestate(rdram),
                lod_ni_telemetry_exec_flags(rdram),
                lod_ni_telemetry_ni_sys_ptr(rdram));
        // Debug: dump first 32 bytes at paddr in both byte orders
        if (even_paddr + 32 <= 0x00800000) {
            const uint32_t* p = (const uint32_t*)(rdram + even_paddr);
            fprintf(stderr, "[ni_ovl]   LE words: %08X %08X %08X %08X\n", p[0], p[1], p[2], p[3]);
            fprintf(stderr, "[ni_ovl]   BE words: %08X %08X %08X %08X\n",
                    __builtin_bswap32(p[0]), __builtin_bswap32(p[1]),
                    __builtin_bswap32(p[2]), __builtin_bswap32(p[3]));
            // Show expected first words from overlay 0
            fprintf(stderr, "[ni_ovl]   Expected (ovl 0): %08X %08X %08X %08X\n",
                    ni_fingerprints[0].words[0], ni_fingerprints[0].words[1],
                    ni_fingerprints[0].words[2], ni_fingerprints[0].words[3]);
        }
    }
    return false;
}
