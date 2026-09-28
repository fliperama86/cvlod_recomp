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
#include <cmath>
#include <array>
#include <vector>
#include "lod/lod_mem_compat.h"
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

#ifndef LOD_ENABLE_PAUSE_ITEM_TRACE
#define LOD_ENABLE_PAUSE_ITEM_TRACE 0
#endif

#ifndef LOD_ENABLE_B2_ASSET_TRACE
#define LOD_ENABLE_B2_ASSET_TRACE 0
#endif

#ifndef LOD_ENABLE_ISSUE23_TRACE
#define LOD_ENABLE_ISSUE23_TRACE 0
#endif

#ifndef LOD_ENABLE_ISSUE23_FORCE_ROLLOVER
#define LOD_ENABLE_ISSUE23_FORCE_ROLLOVER 0
#endif

#ifndef LOD_ENABLE_ISSUE23_FORCE_DIRECT_285E
#define LOD_ENABLE_ISSUE23_FORCE_DIRECT_285E 0
#endif
#ifndef LOD_ENABLE_ISSUE23_FORCE_CHILD_FLAGS
#define LOD_ENABLE_ISSUE23_FORCE_CHILD_FLAGS 0
#endif

#ifndef LOD_ENABLE_ISSUE27_FORCE_CHILD_FLAGS
#define LOD_ENABLE_ISSUE27_FORCE_CHILD_FLAGS 0
#endif

#ifndef LOD_ENABLE_ISSUE27_FORCE_DIRECT_285E
#define LOD_ENABLE_ISSUE27_FORCE_DIRECT_285E 0
#endif

#ifndef LOD_ENABLE_ISSUE27_NI129_TRACE
#define LOD_ENABLE_ISSUE27_NI129_TRACE 0
#endif

#ifndef LOD_ENABLE_ISSUE27_NI_LOAD_TRACE
#define LOD_ENABLE_ISSUE27_NI_LOAD_TRACE 0
#endif

#ifndef LOD_ENABLE_ISSUE27_PAIR51_TRACE
#define LOD_ENABLE_ISSUE27_PAIR51_TRACE 0
#endif

#ifndef LOD_ENABLE_ISSUE27_PAUSE_GATE_TRACE
#define LOD_ENABLE_ISSUE27_PAUSE_GATE_TRACE 0
#endif

#ifndef LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN
#define LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN 0
#endif

#define LOD_ENABLE_NI129_STATE_TRACE \
    (LOD_ENABLE_ISSUE23_TRACE || LOD_ENABLE_ISSUE27_NI129_TRACE)

#define LOD_ENABLE_NI129_STATE_WRAPPERS \
    (LOD_ENABLE_NI129_STATE_TRACE || LOD_ENABLE_ISSUE23_FORCE_DIRECT_285E || \
     LOD_ENABLE_ISSUE23_FORCE_CHILD_FLAGS || LOD_ENABLE_ISSUE27_FORCE_CHILD_FLAGS || \
     LOD_ENABLE_ISSUE27_FORCE_DIRECT_285E)

#ifndef LOD_ENABLE_TOWER_NI126_TRACE
#define LOD_ENABLE_TOWER_NI126_TRACE 0
#endif

#ifndef LOD_FIX_PAIR126_INPUT_RELEASE
#define LOD_FIX_PAIR126_INPUT_RELEASE 0
#endif

#ifndef LOD_FIX_HENRY_LOAD_HANDOFF
#define LOD_FIX_HENRY_LOAD_HANDOFF 0
#endif

#ifndef LOD_ENABLE_NI99_MAP76_TRACE
#define LOD_ENABLE_NI99_MAP76_TRACE 1
#endif

#ifndef LOD_FIX_NI_PAIR120_RESULT_LABELS
#define LOD_FIX_NI_PAIR120_RESULT_LABELS 1
#endif

#ifndef LOD_FIX_NI_PAIR24_INTERNAL_LABELS
#define LOD_FIX_NI_PAIR24_INTERNAL_LABELS 1
#endif

#ifndef LOD_FIX_NI_PAIR23_INTERNAL_LABELS
#define LOD_FIX_NI_PAIR23_INTERNAL_LABELS 1
#endif

#ifndef LOD_FIX_NI_PRESERVE_SAME_PAIR_DATA
#define LOD_FIX_NI_PRESERVE_SAME_PAIR_DATA 1
#endif

#ifndef LOD_FIX_NI_PAIR99_ITEM_LIST_PERSIST
#define LOD_FIX_NI_PAIR99_ITEM_LIST_PERSIST 1
#endif

#ifndef LOD_FIX_NI_PERSIST_OVERLAY_DATA
#define LOD_FIX_NI_PERSIST_OVERLAY_DATA 0
#endif

#ifndef LOD_FIX_NI_SEG6_CPU_ALIAS
#define LOD_FIX_NI_SEG6_CPU_ALIAS 0
#endif

#ifndef LOD_FIX_RUN_DL_STALE_NI_FALLBACK
#define LOD_FIX_RUN_DL_STALE_NI_FALLBACK 0
#endif

#ifndef LOD_ENABLE_NI0E_TRACE
#define LOD_ENABLE_NI0E_TRACE 0
#endif

extern "C" void load_overlays(uint32_t rom, int32_t ram_addr, uint32_t size);
extern "C" void unload_overlays(int32_t ram_addr, uint32_t size);
extern "C" uint32_t lod_current_map_overlay_rom();
extern "C" uint32_t lod_current_map_overlay_size();
extern "C" int lod_current_map_overlay_load_count();
#if LOD_ENABLE_B2_ASSET_TRACE
extern "C" void lod_install_map76_boss_ni44_destroy_trace_wrapper(const char* reason);
#endif

// Full overlay data table (rom offsets + sizes in extended ROM)
#include "ni_ovl_data.h"

// NI pairs: text_ni_index 0x1C5 (453) → pair 0.
// pair_index = (ni_text_index - 0x1C5) / 2
static constexpr int NI_TEXT_INDEX_START = 0x1C5;
static constexpr int NI_PAIR_COUNT = 245;
static constexpr uint32_t NI_OVERLAY_UNLOAD_SIZE = 0x00100000;
static constexpr uint32_t LOD_ISSUE27_PRE_HANDOFF_MAP_ROM = 0x0082E330;
static constexpr uint32_t LOD_ISSUE27_POST_HARPY_MAP_ROM = 0x007D3C90;

// Track separately for 0x0E and 0x0F (they're independent)
static int loaded_0f_pair = -1;
static int loaded_0e_pair = -1;

// Recompiled MEM_* helpers expect canonical sign-extended MIPS 32-bit
// addresses in GPRs. A zero-extended 0x80xxxxxx pointer maps 4GB past the
// intended RDRAM mirror and can fault even when the low 32-bit address is
// otherwise valid.
static gpr lod_ni_canonical_gpr(gpr value) {
    return (gpr)(int32_t)(uint32_t)value;
}

struct LodNiSavedCalleeRegs {
    gpr r16;
    gpr r17;
    gpr r18;
    gpr r19;
    gpr r20;
    gpr r21;
    gpr r22;
    gpr r23;
    gpr r30;
};

static LodNiSavedCalleeRegs lod_ni_save_callee_regs(const recomp_context* ctx) {
    return {
        lod_ni_canonical_gpr(ctx->r16),
        lod_ni_canonical_gpr(ctx->r17),
        lod_ni_canonical_gpr(ctx->r18),
        lod_ni_canonical_gpr(ctx->r19),
        lod_ni_canonical_gpr(ctx->r20),
        lod_ni_canonical_gpr(ctx->r21),
        lod_ni_canonical_gpr(ctx->r22),
        lod_ni_canonical_gpr(ctx->r23),
        lod_ni_canonical_gpr(ctx->r30),
    };
}

static void lod_ni_restore_callee_regs(recomp_context* ctx, const LodNiSavedCalleeRegs& saved) {
    ctx->r16 = saved.r16;
    ctx->r17 = saved.r17;
    ctx->r18 = saved.r18;
    ctx->r19 = saved.r19;
    ctx->r20 = saved.r20;
    ctx->r21 = saved.r21;
    ctx->r22 = saved.r22;
    ctx->r23 = saved.r23;
    ctx->r30 = saved.r30;
}

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

#if LOD_ENABLE_NI0E_TRACE
// Issue #27/#31 unified 0x0E hypothesis: unsampled event log for anything that
// touches the 0x0E TLB window's residency bookkeeping. These events are rare
// (a handful per session per the reporter logs), so every one is logged.
// See docs/issue27-31-ni0e-findings.md for the research this is based on.
static void lod_ni0e_log_slot_event(uint8_t* rdram, const char* kind, int pair_index,
                                     int prev_pair, uint32_t even_paddr, uint32_t odd_paddr) {
    fprintf(stderr,
            "[NI0E_TRACE] slot-event kind=%s pair=%d prev0e=%d even=0x%08X odd=0x%08X "
            "map_rom=0x%08X gs=%d\n",
            kind, pair_index, prev_pair, even_paddr, odd_paddr,
            lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram));
}

// Round 8, task 4: prove whether the days-banner text struct's field wipe
// (docs/issue27-31-ni0e-findings.md Round 7) is caused by *this* host-side
// path re-copying an NI pair's pristine ROM data over the 0x0E/0x0F TLB
// window. Reads the struct pointer this session has registered at RDRAM
// 0x8019EECC (func_80145FD4's global, see Round 7) and checks whether it
// falls inside [dst, dst+size) of a segment copy about to happen. Always
// logs on a hit (rare: a handful of pair swaps per session).
static void lod_ni0e_check_textbuf_reload(uint8_t* rdram, uint8_t* dst, uint32_t size,
                                           const char* path_tag) {
    constexpr uint32_t TEXTREG_GLOBAL_PHYS = 0x0019EECC; // RDRAM 0x8019EECC
    const uint32_t struct_ptr = lod_ni_telemetry_u32(rdram, TEXTREG_GLOBAL_PHYS);
    if (struct_ptr == 0) {
        return;
    }
    // Round 8 fix: resolve struct_ptr the same way the recompiled MEM_W macro
    // would (lib/N64ModernRuntime/N64Recomp/include/recomp.h: sign-extend as
    // a 32-bit value, then subtract the kseg0 base 0xFFFFFFFF80000000). A
    // plain `& 0x1FFFFFFF` mask (the original version of this check) only
    // resolves ordinary kseg0 heap pointers (0x80xxxxxx) correctly; it silently
    // stays below 0x20000000 for a raw 0x0E/0x0F TLB-window address and can
    // never match this function's own `dst` (which is always in the
    // 0x8E000000/0x8F000000 mirror range for such addresses), even though
    // that is the single most likely case given func_80145FD4's struct_ptr is
    // built from `file_ptr_array[seg_id]`, which for a code-classified NI
    // pair's seg_id holds exactly such a raw 0x0E/0x0F value. This formula
    // handles both conventions uniformly.
    const uint64_t signed_ptr = (uint64_t)(int64_t)(int32_t)struct_ptr;
    const uint64_t struct_off = signed_ptr - 0xFFFFFFFF80000000ULL;
    const uint64_t dst_off = (uint64_t)(dst - rdram);
    if (struct_off >= dst_off && struct_off < dst_off + size) {
        fprintf(stderr,
                "[NI0E_TRACE] textbuf-reload dst=0x%08X size=0x%X struct=0x%08X map_rom=0x%08X "
                "path=%s\n",
                (uint32_t)dst_off, size, struct_ptr, lod_current_map_overlay_rom(), path_tag);
    }
}

// Divergence detector: called from object_dispatch1's two LOOKUP_FUNC call
// sites (RecompiledFuncs/funcs_1.c) whenever the dispatch target falls in the
// 0x0E window. Dumps the recomp's own loaded_0e_pair alongside a small window
// of the game's own NI-system object (sys+0x295C, RDRAM 0x801CAC1C) so a human
// can correlate the two later. The exact field that records "which pair the
// game believes is resident at 0x0E" was not conclusively identified during
// research (see docs/issue27-31-ni0e-findings.md section b), so this dumps a
// window of candidate words rather than doing a direct MISMATCH compare.
extern "C" void lod_ni0e_check_dispatch(uint8_t* rdram, uint32_t target, uint32_t obj) {
    static uint32_t call_count = 0;
    call_count++;
    const bool should_log = call_count <= 40 || (call_count % 600) == 0;
    if (!should_log) {
        return;
    }

    const uint32_t ni_sys_ptr = lod_ni_telemetry_ni_sys_ptr(rdram);
    const bool ni_sys_ptr_ok = ni_sys_ptr >= 0x80000000u && ni_sys_ptr < 0x80800000u;
    uint32_t window[5] = {};
    bool window_ok = false;
    if (ni_sys_ptr_ok) {
        const uint32_t ni_sys_phys = ni_sys_ptr & 0x1FFFFFFFu;
        window_ok = lod_ni_telemetry_range_ok(ni_sys_phys + 0x140, 4);
        if (window_ok) {
            window[0] = lod_ni_telemetry_u32(rdram, ni_sys_phys + 0x130);
            window[1] = lod_ni_telemetry_u32(rdram, ni_sys_phys + 0x134);
            window[2] = lod_ni_telemetry_u32(rdram, ni_sys_phys + 0x138);
            window[3] = lod_ni_telemetry_u32(rdram, ni_sys_phys + 0x13C);
            window[4] = lod_ni_telemetry_u32(rdram, ni_sys_phys + 0x140);
        }
    }

    fprintf(stderr,
            "[NI0E_TRACE] dispatch #%u target=0x%08X obj=0x%08X loaded0e=%d "
            "ni_sys_ptr=0x%08X map_rom=0x%08X gs=%d window[0x130..0x140]=%s"
            "%08X,%08X,%08X,%08X,%08X\n",
            call_count, target, obj, loaded_0e_pair, ni_sys_ptr,
            lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram),
            window_ok ? "" : "(invalid ptr) ",
            window[0], window[1], window[2], window[3], window[4]);
}

// Round 22: GameStateMgr command-queue node-pool probes.
//
// Task: prove or refute "the GSM command-queue node pool has only 4 slots at
// RDRAM 0x801B1D60, leaks nodes across loaded-Henry map transitions, and
// silently drops the fatal transition's scene-build command" (this round's
// task brief; docs/issue27-31-fix-design.md section 11).
//
// Static reading this round of cmdNodeTable_clear (func_80001940,
// RecompiledFuncs/funcs_0.c) and cmdNodeTable_alloc (func_80001968, same
// file) already REFUTES the "4 slots" premise: both functions compute their
// table bounds as the literal immediates 0x801B1D60 (base) and 0x801B3D60
// (end) -- an 0x2000-byte region of 8-byte slots, i.e. **1024 slots**, not
// 4. This is independently corroborated by the pre-existing round-5
// object-destroy-probe comment above (lod_ni0e_manager_destroy_probe,
// RecompiledFuncs/funcs_1.c): it documents 0x801B3D60 as the START of a
// *different*, adjacent table (a per-id live-instance counter) -- exactly
// the address this round's read finds as the node pool's END. Two
// independent investigations landing on the same boundary is strong
// confirmation the geometry above is correct.
//
// A slot (8 bytes: +0 "next" link, +4 payload/occupancy word) is free iff
// the word at slot+4 == 0. cmdNodeTable_alloc does a linear first-fit scan
// of all 1024 slots and returns NULL with **zero signal of any kind** on
// exhaustion (no return code the caller has to ignore -- there simply is no
// side channel at all). Every caller of the pool (func_800019B0/
// func_800019E8/GameStateMgr_enqueue, all in funcs_0.c/funcs_1.c) treats a
// NULL return by silently skipping the link step -- confirmed exhaustively
// silent, no retry, no error path, anywhere in the call chain reachable from
// static reading.
//
// Also found this round: the pool is NOT GSM-exclusive. Besides
// GameStateMgr_enqueue (funcs_1.c), func_80010EFC (funcs_7.c, in the same
// object-execution family as object_execute/object_activateChildren) is an
// independent, unrelated caller of the same push-helpers
// (func_800019B0/func_800019E8) and therefore an independent consumer of
// pool capacity. And GameStateMgr_dispatch (func_80002E3C, the pool's own
// consumer/freer) has, like every function in round 18/19/21's DMA-manager
// chain, **zero static jal callers anywhere in RecompiledFuncs/*.c** -- it
// is reached only through a runtime function pointer, so whether it runs on
// every frame of a loaded-Henry transition is not resolvable from static
// reading alone (the same ceiling round 21 section 10.4 already documented
// for the DMA-manager chain).
//
// Separately, GameStateMgr_dispatch's own three possible actions on a
// dequeued command -- func_800020E8 (frees an object's alloc_data/figure
// pointers; independently confirmed by the pre-existing round-5 probe
// comment as "the object-destroy entry point"), sceneDataPool_free (frees a
// scene-data-pool entry), or func_80001920 (a small default pointer-clear)
// -- are all TEARDOWN actions. None of them create objects. Every static
// caller of GameStateMgr_enqueue found this round (object_executeChildren,
// func_80004F44's recursive children-destroy walk, object_dispatchChild's
// per-field release block) is itself object/resource-teardown code, not
// scene-build code. So: this queue is a deferred DESTROY/RELEASE dispatcher,
// not a scene-build command bus -- no evidence was found that the 0x1AB
// framework family's creation is routed through it at all (that family's
// own bgState state machine, table 0x8018D3B0, is driven by the ordinary
// per-object dispatch loop, a completely separate mechanism). The literal
// "scene-build command silently dropped" claim is therefore REFUTED by
// direct reading; the "silent failure on exhaustion" mechanism is real, but
// what it would silently drop is a deferred object/resource release, not a
// scene build.
//
// The probes below still ship as directed (occupancy/alloc/free/enqueue
// visibility is useful regardless of which hypothesis turns out to be
// right), so a future capture can show empirically whether occupancy ever
// climbs meaningfully across loaded-Henry transitions or stays near-idle
// (expected, given the geometry finding above).
static constexpr uint32_t LOD_NI0E_GSM_POOL_BASE_PHYS = 0x001B1D60u;
static constexpr uint32_t LOD_NI0E_GSM_POOL_END_PHYS = 0x001B3D60u;
static constexpr uint32_t LOD_NI0E_GSM_POOL_SLOT_STRIDE = 8u;
static constexpr uint32_t LOD_NI0E_GSM_POOL_SLOT_COUNT =
    (LOD_NI0E_GSM_POOL_END_PHYS - LOD_NI0E_GSM_POOL_BASE_PHYS) / LOD_NI0E_GSM_POOL_SLOT_STRIDE;

static uint32_t lod_ni0e_gsm_pool_slot_index(uint32_t addr) {
    const uint32_t phys = addr & 0x1FFFFFFFu;
    if (phys < LOD_NI0E_GSM_POOL_BASE_PHYS || phys >= LOD_NI0E_GSM_POOL_END_PHYS) {
        return 0xFFFFFFFFu;
    }
    return (phys - LOD_NI0E_GSM_POOL_BASE_PHYS) / LOD_NI0E_GSM_POOL_SLOT_STRIDE;
}

extern "C" uint32_t lod_ni0e_gsm_pool_occupied_count(uint8_t* rdram) {
    uint32_t occupied = 0;
    for (uint32_t phys = LOD_NI0E_GSM_POOL_BASE_PHYS; phys < LOD_NI0E_GSM_POOL_END_PHYS;
         phys += LOD_NI0E_GSM_POOL_SLOT_STRIDE) {
        if (lod_ni_telemetry_u32(rdram, phys + 4u) != 0u) {
            occupied++;
        }
    }
    return occupied;
}

// Sticky per-enqueue-call failure flag: set by lod_ni0e_gsm_alloc_probe
// whenever cmdNodeTable_alloc returns NULL, consumed by
// lod_ni0e_gsm_enq_probe at GameStateMgr_enqueue's single exit point. A
// logical enqueue always performs at least one underlying pool alloc (either
// directly, or via func_800019B0/func_800019E8), so "any NULL during this
// call" is the correct definition of "this enqueue's command was dropped."
static bool g_lod_ni0e_gsm_enq_alloc_failed = false;

// gsm-alloc: fires from cmdNodeTable_alloc (funcs_0.c) on every call.
// Task-specified: always for a pool-exhaustion return (slot_addr==0 -- the
// previously-invisible silent-drop moment), rate-limited (first 100 + every
// 500th) for ordinary successes.
extern "C" void lod_ni0e_gsm_alloc_probe(uint8_t* rdram, uint32_t slot_addr, uint32_t payload) {
    static uint32_t call_count = 0;
    call_count++;
    const bool exhausted = (slot_addr == 0u);
    if (exhausted) {
        g_lod_ni0e_gsm_enq_alloc_failed = true;
    }
    const bool should_log = exhausted || call_count <= 100u || (call_count % 500u) == 0u;
    if (!should_log) {
        return;
    }
    const uint32_t occupied = lod_ni0e_gsm_pool_occupied_count(rdram);
    if (exhausted) {
        fprintf(stderr,
                "[NI0E_TRACE] gsm-alloc #%u DROPPED payload=0x%08X occupied=%u/%u map_rom=0x%08X "
                "gs=%d\n",
                call_count, payload, occupied, LOD_NI0E_GSM_POOL_SLOT_COUNT,
                lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram));
    } else {
        fprintf(stderr,
                "[NI0E_TRACE] gsm-alloc #%u slot=0x%08X idx=%u payload=0x%08X occupied=%u/%u "
                "map_rom=0x%08X\n",
                call_count, slot_addr, lod_ni0e_gsm_pool_slot_index(slot_addr), payload, occupied,
                LOD_NI0E_GSM_POOL_SLOT_COUNT, lod_current_map_overlay_rom());
    }
}

// gsm-free: fires at each of the 4 sites that clear a slot's +4 payload word
// back to 0 (GameStateMgr_dispatch's 3 inline sub-list-node frees, plus
// func_80001B00's 1 master-entry free). Same rate limit as gsm-alloc; frees
// are the mirror image and equally bursty (a full-room teardown at a map
// transition can free dozens of nodes in one GameStateMgr_dispatch call).
extern "C" void lod_ni0e_gsm_free_probe(uint8_t* rdram, uint32_t slot_addr, uint32_t old_payload) {
    static uint32_t call_count = 0;
    call_count++;
    const bool should_log = call_count <= 100u || (call_count % 500u) == 0u;
    if (!should_log) {
        return;
    }
    const uint32_t occupied = lod_ni0e_gsm_pool_occupied_count(rdram);
    fprintf(stderr,
            "[NI0E_TRACE] gsm-free #%u slot=0x%08X idx=%u old_payload=0x%08X occupied=%u/%u "
            "map_rom=0x%08X\n",
            call_count, slot_addr, lod_ni0e_gsm_pool_slot_index(slot_addr), old_payload, occupied,
            LOD_NI0E_GSM_POOL_SLOT_COUNT, lod_current_map_overlay_rom());
}

// gsm-enq: fires once per GameStateMgr_enqueue (func_80002D5C) call, at its
// single exit point. Task-specified: always for drops, rate-limited for
// successes.
extern "C" void lod_ni0e_gsm_enq_begin() {
    g_lod_ni0e_gsm_enq_alloc_failed = false;
}

extern "C" void lod_ni0e_gsm_enq_probe(uint8_t* rdram, uint32_t obj, uint32_t cmd,
                                        uint32_t target) {
    static uint32_t call_count = 0;
    call_count++;
    const bool dropped = g_lod_ni0e_gsm_enq_alloc_failed;
    const bool should_log = dropped || call_count <= 100u || (call_count % 500u) == 0u;
    if (!should_log) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] gsm-enq #%u obj=0x%08X cmd=0x%08X target=0x%08X %s map_rom=0x%08X "
            "gs=%d\n",
            call_count, obj, cmd, target, dropped ? "DROPPED" : "ok",
            lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram));
}

// Round 24: NI-system object (id 0x009) state-machine probes, tag
// "nisys-state". Task: map ni_system_handler (0x8001B9A0) and its dispatch
// parent func_8001B718 (0x8001B718, id-0x009 driver, table 0x800B3834) end
// to end; find which state calls func_80011D80 (DMA-pump re-init), which
// waits on load completion, which calls overlay_system_create (0x8001BA78).
// See docs/issue27-31-fix-design.md section 12 for the full writeup.
//
// Full read this round found: func_8001B718 and func_80011D80 are BOTH
// generic recursion-guarded per-object state dispatchers of the identical
// shape (obj+0xE depth counter; obj+depth*2+8/+9 = per-depth-level
// visit-count/state-index byte pair; state advance via
// object_curLevel_goToNextFuncAndClearTimer, func_80001CE8, which ALWAYS
// increments the state index by exactly 1 with no data-driven condition of
// its own) but they dispatch through TWO DIFFERENT tables on TWO DIFFERENT
// objects: func_8001B718 uses table 0x800B3834 on the id-0x009 "NI-system"
// object; func_80011D80 uses table D_800AF560 (round 21 section 10.4's
// already-named DMA-manager table) on the separate "DMA-pump" object rounds
// 18/19/21/22 already investigated. The two are linked only through the
// global active-pump pointer at RDRAM 0x800C1600, which ni_system_handler
// reads and hands to func_80012ED0 below -- NOT through any shared
// state-index/table. func_80011D80 does NOT touch the NI-system object at
// all, so "does a transition skip func_80011D80" (this round's original
// framing) is the wrong question -- direct log evidence (repro22.log) shows
// a desc-life REINIT precedes every single MAP_OVL load, including the
// fatal one; see section 12 for the corrected divergence.
//
// ni_system_handler's own tail ALWAYS calls func_80001CE8 (unconditional
// advance) regardless of which internal branch it took, then immediately
// re-dispatches to table[new index] in the SAME call (a tail-chain, not a
// return-and-reinvoke) -- so ni_system_handler runs for exactly one pass
// ever per id-0x009 object lifetime, then permanently hands off to
// overlay_system_create (going by code adjacency and the pre-existing
// bgstate-trace wrapper naming), which does NOT participate in that
// tail-chain -- it re-invokes itself via the object's own obj+0x10 "current
// dispatch pointer" field instead, so once state reaches its slot it is
// what func_8001B718 calls every subsequent frame, gated solely by
// sys+0x2B24 (RDRAM 0x801CADE4): create the parentless 0x1AB root iff
// sys+0x2B24 != 0.
//
// The only writer of sys+0x2B24 in this whole chain is func_80012ED0,
// called from ni_system_handler's a2!=0 branch (a2 = a per-scene table
// row's field0, table at 0x800B2DE8 + sys.scene*8) with a1=&sys+0x2B24.
// func_80012ED0 has a 3-way branch: alignment-fail (returns 0, no side
// effect); "delta==0 and a2 looks like a valid ROM address" -> issues an
// IMMEDIATE synchronous DMA_ROMCopy and writes sys+0x2B24=a3+a2 in the SAME
// call (healthy, same-frame case, site=immediate-dma); otherwise -> creates
// a NEW child object (id 5) under the CURRENT active pump
// (*(0x800C1600)), stores &sys+0x2B24 into that child's own +0x34 field,
// and EXPLICITLY WRITES ZERO into sys+0x2B24 (site=defer-child) --
// deferring the completion signal entirely to that new child object's own
// future per-frame dispatch (its own separate obj+0x10 entry, never itself
// found this round -- same "no static jal caller, function-pointer-only"
// ceiling already hit for the DMA-manager chain, round 21 sec 10.4, and
// GameStateMgr_dispatch, round 22 sec 11.3).
//
// This is the identified divergence candidate for Task 2: since
// ni_system_handler's a2!=0 branch (and therefore this whole func_80012ED0
// call) executes AT MOST ONCE per id-0x009 object lifetime, if site
// "defer-child" is taken and the resulting child object is never dispatched
// again (the same "driving object stopped running" mechanism already
// suspected for the DMA-manager chain and GameStateMgr_dispatch), sys+0x2B24
// never becomes nonzero, and overlay_system_create loops forever finding its
// guard false -- exactly the observed hang shape. site=defer-child is always
// logged below (task-specified, not rate-limited); everything else is
// rate-limited (first 60 + every 300).
extern "C" void lod_ni0e_nisys_handler_probe(uint8_t* rdram, uint32_t obj,
                                              uint32_t table_entry_addr, uint32_t a2) {
    static uint32_t call_count = 0;
    call_count++;
    const bool should_log = call_count <= 60u || (call_count % 300u) == 0u;
    if (!should_log) {
        return;
    }
    constexpr uint32_t SYS_PHYS = 0x001C82C0u;
    const uint32_t table_phys = table_entry_addr & 0x1FFFFFFFu;
    const uint32_t field0 = lod_ni_telemetry_u32(rdram, table_phys + 0u);
    const uint32_t field1 = lod_ni_telemetry_u32(rdram, table_phys + 4u);
    const uint32_t sys2b24 = lod_ni_telemetry_u32(rdram, SYS_PHYS + 0x2B24u);
    const uint32_t obj_phys = obj & 0x1FFFFFFFu;
    const uint32_t state_word = lod_ni_telemetry_u32(rdram, obj_phys + 0x8u);
    fprintf(stderr,
            "[NI0E_TRACE] nisys-state #%u site=ni_system_handler obj=0x%08X state8B=0x%08X "
            "a2(field0)=0x%08X field1=0x%08X branch=%s sys2B24=0x%08X map_rom=0x%08X gs=%d\n",
            call_count, obj, state_word, a2, field1, a2 == 0u ? "idle-bump" : "scene-active",
            sys2b24, lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram));
}

extern "C" void lod_ni0e_ovlsys_create_probe(uint8_t* rdram, uint32_t obj, uint32_t sys2b24) {
    static uint32_t call_count = 0;
    call_count++;
    const bool will_create = sys2b24 != 0u;
    const bool should_log = will_create || call_count <= 60u || (call_count % 300u) == 0u;
    if (!should_log) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] nisys-state #%u site=overlay_system_create obj=0x%08X sys2B24=0x%08X "
            "guard=%s map_rom=0x%08X gs=%d\n",
            call_count, obj, sys2b24, will_create ? "CREATE" : "skip",
            lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram));
}

// func_80012ED0's 3-way branch. site=defer-child is Task 2's identified
// divergent branch -- always logged (always_log!=0), not rate-limited, per
// instructions. site=align-fail and site=immediate-dma are rate-limited.
extern "C" void lod_ni0e_nisys_future_probe(uint8_t* rdram, const char* site, uint32_t always_log,
                                             uint32_t pump_obj, uint32_t completion_ptr,
                                             uint32_t a2, uint32_t a3, int32_t delta) {
    static uint32_t call_count = 0;
    call_count++;
    const bool should_log = always_log != 0u || call_count <= 60u || (call_count % 300u) == 0u;
    if (!should_log) {
        return;
    }
    constexpr uint32_t SYS_PHYS = 0x001C82C0u;
    const uint32_t sys2b24_before = lod_ni_telemetry_u32(rdram, SYS_PHYS + 0x2B24u);
    fprintf(stderr,
            "[NI0E_TRACE] nisys-state #%u site=%s pump_obj=0x%08X completion_ptr=0x%08X "
            "a2=0x%08X a3=0x%08X delta=%d sys2B24_before=0x%08X map_rom=0x%08X gs=%d\n",
            call_count, site, pump_obj, completion_ptr, a2, a3, delta, sys2b24_before,
            lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram));
}
#endif  // LOD_ENABLE_NI0E_TRACE

#if LOD_ENABLE_NI129_STATE_WRAPPERS
static constexpr uint32_t LOD_ISSUE23_DEST_MAP_ROM = 0x0082E330;
static constexpr int LOD_ISSUE23_NI_PAIR = 129;

static bool lod_issue23_ni_focus(int pair_index, uint32_t vram) {
    if (pair_index != LOD_ISSUE23_NI_PAIR || vram != 0x0F000000) {
        return false;
    }

    const uint32_t map_rom = lod_current_map_overlay_rom();
#if LOD_ENABLE_ISSUE23_TRACE || LOD_ENABLE_ISSUE23_FORCE_DIRECT_285E || LOD_ENABLE_ISSUE23_FORCE_CHILD_FLAGS
    if (map_rom == LOD_ISSUE23_DEST_MAP_ROM) {
        return true;
    }
#endif
#if LOD_ENABLE_ISSUE27_NI129_TRACE || LOD_ENABLE_ISSUE27_FORCE_CHILD_FLAGS || \
    LOD_ENABLE_ISSUE27_FORCE_DIRECT_285E
    if (map_rom == LOD_ISSUE27_POST_HARPY_MAP_ROM) {
        return true;
    }
#endif
    return false;
}

static bool lod_issue23_ni_trace_map_active(uint32_t map_rom) {
#if LOD_ENABLE_ISSUE23_TRACE
    if (map_rom == LOD_ISSUE23_DEST_MAP_ROM) {
        return true;
    }
#endif
#if LOD_ENABLE_ISSUE27_NI129_TRACE
    if (map_rom == LOD_ISSUE27_POST_HARPY_MAP_ROM) {
        return true;
    }
#endif
    return false;
}

static const char* lod_issue23_ni_trace_prefix(uint32_t map_rom) {
#if LOD_ENABLE_ISSUE27_NI129_TRACE
    if (map_rom == LOD_ISSUE27_POST_HARPY_MAP_ROM) {
        return "ISSUE27_NI129";
    }
#endif
    (void)map_rom;
    return "ISSUE23_NI129";
}

static void lod_issue23_log_ni(uint8_t* rdram, const char* phase,
                               int pair_index, uint32_t vram, uint32_t mapped_vaddr) {
#if LOD_ENABLE_NI129_STATE_TRACE
    if (!lod_issue23_ni_focus(pair_index, vram)) {
        return;
    }

    static int log_count = 0;
    log_count++;
    if (log_count > 80 && (log_count % 30) != 0) {
        return;
    }

    const uint32_t map_rom = lod_current_map_overlay_rom();
    const NiOvlData& data = ni_ovl_data[pair_index];
    fprintf(stderr,
            "[%s] #%d phase=%s gs=%d exec=0x%08X ni=0x%08X "
            "map#%d map=0x%08X pair=%d vram=0x%08X mapped=0x%08X "
            "loaded0f=%d loaded0e=%d rom=0x%08X size=0x%X\n",
            lod_issue23_ni_trace_prefix(map_rom), log_count, phase,
            lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram), lod_ni_telemetry_ni_sys_ptr(rdram),
            lod_current_map_overlay_load_count(), map_rom,
            pair_index, vram, mapped_vaddr, loaded_0f_pair, loaded_0e_pair,
            data.rom_offset, data.full_size);
#else
    (void)rdram;
    (void)phase;
    (void)pair_index;
    (void)vram;
    (void)mapped_vaddr;
#endif
}

static recomp_func_t* lod_orig_issue23_ni129_entry = nullptr;
static recomp_func_t* lod_orig_issue23_ni129_0070 = nullptr;
static recomp_func_t* lod_orig_issue23_ni129_00C8 = nullptr;
static recomp_func_t* lod_orig_issue23_ni129_0244 = nullptr;
static recomp_func_t* lod_orig_issue23_ni129_04A8 = nullptr;
static recomp_func_t* lod_orig_issue23_ni129_068C = nullptr;
static recomp_func_t* lod_orig_issue23_ni129_07FC = nullptr;
static recomp_func_t* lod_orig_issue23_ni129_0938 = nullptr;

static uint32_t lod_issue23_ni_u32(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return (uint32_t)MEM_W(off, lod_ni_canonical_gpr(base));
}

static int16_t lod_issue23_ni_s16(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return (int16_t)MEM_H(off, lod_ni_canonical_gpr(base));
}

static uint8_t lod_issue23_ni_u8(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return (uint8_t)MEM_BU(off, lod_ni_canonical_gpr(base));
}

static float lod_issue23_ni_float_bits(uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint32_t lod_issue23_ni_dispatch_target(uint8_t* rdram, uint8_t index) {
    return lod_issue23_ni_u32(rdram, 0x0F000D88, (int32_t)index * 4);
}

static bool lod_issue23_ni_should_log(uint32_t count) {
    return count <= 160 || (count % 120) == 0;
}

static bool lod_issue23_ni_should_log_for_map(uint32_t map_rom, uint32_t count) {
#if LOD_ENABLE_ISSUE27_NI129_TRACE
    if (map_rom == LOD_ISSUE27_POST_HARPY_MAP_ROM) {
        return count <= 12 || count == 60 || count == 120 || (count % 60) == 0;
    }
#else
    (void)map_rom;
#endif
    return lod_issue23_ni_should_log(count);
}

static uint32_t lod_issue23_ni_child_flag_word(uint8_t* rdram) {
    constexpr int32_t child_flag_word_off = (0x2A0 >> 5) * 4;
    return lod_issue23_ni_u32(rdram, 0x801CAA60, child_flag_word_off);
}

static uint32_t lod_issue23_ni_child_flag_pack(uint32_t word) {
    uint32_t pack = 0;
    for (uint32_t flag = 0x2A4; flag <= 0x2A9; flag++) {
        const uint32_t mask = 0x80000000u >> (flag & 0x1Fu);
        if ((word & mask) != 0) {
            pack |= 1u << (flag - 0x2A4);
        }
    }
    return pack;
}

static bool lod_issue23_ni_guest_ram_range_ok(uint32_t addr, uint32_t size) {
    if (addr == 0) {
        return false;
    }
    const uint32_t phys = addr & 0x1FFFFFFF;
    return lod_ni_telemetry_range_ok(phys, size);
}

static bool lod_issue23_ni_guest_overlay_range_ok(uint32_t addr, uint32_t size) {
    const uint32_t seg = addr & 0xFF000000u;
    if (seg != 0x0F000000u && seg != 0x0E000000u) {
        return false;
    }

    const int pair = seg == 0x0F000000u ? loaded_0f_pair : loaded_0e_pair;
    if (pair < 0 || pair >= NI_PAIR_COUNT) {
        return false;
    }

    const uint32_t off = addr & 0x00FFFFFFu;
    const uint32_t size_avail = ni_ovl_data[pair].full_size;
    return size <= size_avail && off <= (size_avail - size);
}

static bool lod_issue23_ni_guest_mem_range_ok(uint32_t addr, uint32_t size) {
    return lod_issue23_ni_guest_ram_range_ok(addr, size) ||
           lod_issue23_ni_guest_overlay_range_ok(addr, size);
}

static uint32_t lod_issue23_ni_u32_guest(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    const uint32_t addr = (uint32_t)(base + off);
    return lod_issue23_ni_guest_mem_range_ok(addr, 4)
        ? (uint32_t)MEM_W(off, lod_ni_canonical_gpr(base))
        : 0;
}

static uint16_t lod_issue23_ni_u16_ram(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    const uint32_t addr = (uint32_t)(base + off);
    return lod_issue23_ni_guest_ram_range_ok(addr, 2)
        ? (uint16_t)MEM_HU(off, lod_ni_canonical_gpr(base))
        : 0;
}

static uint32_t lod_issue23_ni_u32_ram(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    const uint32_t addr = (uint32_t)(base + off);
    return lod_issue23_ni_guest_ram_range_ok(addr, 4)
        ? (uint32_t)MEM_W(off, lod_ni_canonical_gpr(base))
        : 0;
}

static float lod_issue23_ni_float_ram(uint8_t* rdram, uint32_t base, int32_t off) {
    return lod_issue23_ni_float_bits(lod_issue23_ni_u32_ram(rdram, base, off));
}

static void lod_issue27_log_portal_node(uint8_t* rdram, const char* tag,
                                        uint32_t count, const char* func,
                                        uint32_t obj, const char* label,
                                        uint32_t node) {
#if LOD_ENABLE_ISSUE27_NI129_TRACE
    if (lod_current_map_overlay_rom() != LOD_ISSUE27_POST_HARPY_MAP_ROM ||
        node == 0) {
        return;
    }

    if (!lod_issue23_ni_guest_ram_range_ok(node, 0xB8)) {
        fprintf(stderr,
                "[ISSUE27_PORTAL_NODE] %s#%u func=%s obj=0x%08X %s=0x%08X invalid-ram\n",
                tag, count, func, obj, label, node);
        return;
    }

    const uint16_t kind = lod_issue23_ni_u16_ram(rdram, node, 0x00);
    const uint16_t flags = lod_issue23_ni_u16_ram(rdram, node, 0x02);
    const uint32_t parent = lod_issue23_ni_u32_ram(rdram, node, 0x08);
    const uint32_t sibling_prev = lod_issue23_ni_u32_ram(rdram, node, 0x0C);
    const uint32_t sibling_next = lod_issue23_ni_u32_ram(rdram, node, 0x10);
    const uint32_t first_child = lod_issue23_ni_u32_ram(rdram, node, 0x14);
    const uint32_t word18 = lod_issue23_ni_u32_ram(rdram, node, 0x18);
    const uint32_t word1c = lod_issue23_ni_u32_ram(rdram, node, 0x1C);
    const uint32_t word20 = lod_issue23_ni_u32_ram(rdram, node, 0x20);
    const uint32_t word24 = lod_issue23_ni_u32_ram(rdram, node, 0x24);
    const uint32_t word28 = lod_issue23_ni_u32_ram(rdram, node, 0x28);
    const uint32_t word2c = lod_issue23_ni_u32_ram(rdram, node, 0x2C);
    const uint32_t word30 = lod_issue23_ni_u32_ram(rdram, node, 0x30);
    const uint32_t word34 = lod_issue23_ni_u32_ram(rdram, node, 0x34);
    const uint32_t ptr38 = lod_issue23_ni_u32_ram(rdram, node, 0x38);
    const uint32_t ptr3c = lod_issue23_ni_u32_ram(rdram, node, 0x3C);
    const uint32_t ptr40 = lod_issue23_ni_u32_ram(rdram, node, 0x40);
    const uint32_t pool44 = lod_issue23_ni_u32_ram(rdram, node, 0x44);
    const uint32_t pool48 = lod_issue23_ni_u32_ram(rdram, node, 0x48);
    const uint32_t pool4c = lod_issue23_ni_u32_ram(rdram, node, 0x4C);

    const uint32_t dl0 = lod_issue23_ni_u32_guest(rdram, ptr3c, 0x00);
    const uint32_t dl4 = lod_issue23_ni_u32_guest(rdram, ptr3c, 0x04);
    const uint32_t dl8 = lod_issue23_ni_u32_guest(rdram, ptr3c, 0x08);
    const uint32_t dlc = lod_issue23_ni_u32_guest(rdram, ptr3c, 0x0C);
    const uint32_t portal_c58_0 = lod_issue23_ni_u32_guest(rdram, 0x0F000C58, 0x00);
    const uint32_t portal_c58_4 = lod_issue23_ni_u32_guest(rdram, 0x0F000C58, 0x04);
    const uint32_t portal_c68_0 = lod_issue23_ni_u32_guest(rdram, 0x0F000C68, 0x00);
    const uint32_t portal_c68_4 = lod_issue23_ni_u32_guest(rdram, 0x0F000C68, 0x04);
    const uint32_t portal_c70_0 = lod_issue23_ni_u32_guest(rdram, 0x0F000C70, 0x00);
    const uint32_t portal_c70_4 = lod_issue23_ni_u32_guest(rdram, 0x0F000C70, 0x04);

    fprintf(stderr,
            "[ISSUE27_PORTAL_NODE] %s#%u func=%s obj=0x%08X %s=0x%08X "
            "kind=0x%04X flags=0x%04X links={parent=0x%08X prev=0x%08X "
            "next=0x%08X child=0x%08X} words={18=%08X 1c=%08X 20=%08X "
            "24=%08X 28=%08X 2c=%08X 30=%08X 34=%08X} "
            "ptrs={38=0x%08X 3c=0x%08X 40=0x%08X 44=0x%08X 48=0x%08X 4c=0x%08X} "
            "floats={50=%.3f 54=%.3f 58=%.3f 5c=%.3f 60=%.3f 64=%.3f "
            "68=%.3f 6c=%.3f 70=%.3f} dl3c={%08X,%08X,%08X,%08X} "
            "local_dl={c58:%08X,%08X c68:%08X,%08X c70:%08X,%08X}\n",
            tag, count, func, obj, label, node,
            kind, flags, parent, sibling_prev, sibling_next, first_child,
            word18, word1c, word20, word24, word28, word2c, word30, word34,
            ptr38, ptr3c, ptr40, pool44, pool48, pool4c,
            lod_issue23_ni_float_ram(rdram, node, 0x50),
            lod_issue23_ni_float_ram(rdram, node, 0x54),
            lod_issue23_ni_float_ram(rdram, node, 0x58),
            lod_issue23_ni_float_ram(rdram, node, 0x5C),
            lod_issue23_ni_float_ram(rdram, node, 0x60),
            lod_issue23_ni_float_ram(rdram, node, 0x64),
            lod_issue23_ni_float_ram(rdram, node, 0x68),
            lod_issue23_ni_float_ram(rdram, node, 0x6C),
            lod_issue23_ni_float_ram(rdram, node, 0x70),
            dl0, dl4, dl8, dlc,
            portal_c58_0, portal_c58_4, portal_c68_0, portal_c68_4,
            portal_c70_0, portal_c70_4);
#else
    (void)rdram;
    (void)tag;
    (void)count;
    (void)func;
    (void)obj;
    (void)label;
    (void)node;
#endif
}

static void lod_issue23_log_ni_state(uint8_t* rdram, const char* tag, uint32_t count,
                                     const char* func, uint32_t obj, uint32_t ra) {
#if LOD_ENABLE_NI129_STATE_TRACE
    const uint32_t map_rom = lod_current_map_overlay_rom();
    if (!lod_issue23_ni_trace_map_active(map_rom) ||
        !lod_issue23_ni_should_log_for_map(map_rom, count)) {
        return;
    }

    const int16_t depth = lod_issue23_ni_s16(rdram, obj, 0x0E);
    const int next_depth = (int)depth + 1;
    const uint32_t next_off = next_depth >= 0 && next_depth < 8 ? (uint32_t)(next_depth * 2) : 0;
    const uint8_t next_count = next_depth >= 0 && next_depth < 8
        ? lod_issue23_ni_u8(rdram, obj, 0x08 + next_off)
        : 0;
    const uint8_t next_index = next_depth >= 0 && next_depth < 8
        ? lod_issue23_ni_u8(rdram, obj, 0x09 + next_off)
        : 0;
    const uint32_t target = lod_issue23_ni_dispatch_target(rdram, next_index);

    const uint32_t data = lod_issue23_ni_u32(rdram, obj, 0x38);
    const uint32_t data0 = data != 0 ? lod_issue23_ni_u32(rdram, data, 0x00) : 0;
    const uint32_t data4 = data != 0 ? lod_issue23_ni_u32(rdram, data, 0x04) : 0;
    const uint32_t data8 = data != 0 ? lod_issue23_ni_u32(rdram, data, 0x08) : 0;
    const uint16_t data_c = data != 0 ? (uint16_t)lod_issue23_ni_s16(rdram, data, 0x0C) : 0;
    const uint32_t data28_node = data != 0 ? data + 0x28 : 0;
    const uint32_t data30_node = data != 0 ? data + 0x30 : 0;
    const uint32_t obj24 = lod_issue23_ni_u32(rdram, obj, 0x24);

    const uint32_t focus = lod_ni_telemetry_u32(rdram, 0x001CAC20);
    const uint32_t focus24 = lod_ni_telemetry_u32(rdram, 0x001CAC24);
    const uint32_t focus28 = lod_ni_telemetry_u32(rdram, 0x001CAC28);
    const int16_t global_285c = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x285C);
    const int16_t global_285e = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x285E);
    const int16_t global_2860 = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x2860);
    const int16_t global_2862 = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x2862);
    const int16_t global_2864 = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x2864);
    const int16_t global_2866 = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x2866);
    const int16_t global_28d0 = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x28D0);
    const int16_t global_2b4e = (int16_t)lod_issue23_ni_s16(rdram, 0x801C82C0, 0x2B4E);
    const uint32_t global_2bc8 = lod_issue23_ni_u32(rdram, 0x801C82C0, 0x2BC8);
    const uint32_t global_2bd0 = lod_issue23_ni_u32(rdram, 0x801C82C0, 0x2BD0);
    const uint32_t flags_word = lod_issue23_ni_child_flag_word(rdram);
    const uint32_t flags_pack = lod_issue23_ni_child_flag_pack(flags_word);

    fprintf(stderr,
            "[%s_STATE] %s#%u func=%s gs=%d exec=0x%08X ni=0x%08X "
            "map#%d obj=0x%08X ra=0x%08X depth=%d next={count=%u idx=%u target=0x%08X} "
            "levels=%u/%u,%u/%u,%u/%u,%u/%u fields={child=0x%08X data=0x%08X "
            "entry=%u func=0x%08X aux44=0x%08X} data={%08X,%08X,%08X,c=%04X} "
            "portalNodes={data28=0x%08X data30=0x%08X} "
            "globals={285c=%d 285e=%d 2860=%d 2862=%d 2864=%d 2866=%d "
            "28d0=%d 2b4e=%d 2bc8=0x%08X 2bd0=0x%08X "
            "flagsWord=0x%08X flagsPack=0x%02X focus=0x%08X "
            "focus24=0x%08X focus28=0x%08X loaded0f=%d loaded0e=%d}\n",
            lod_issue23_ni_trace_prefix(map_rom), tag, count, func,
            lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram), lod_ni_telemetry_ni_sys_ptr(rdram),
            lod_current_map_overlay_load_count(), obj, ra, (int)depth,
            next_count, next_index, target,
            lod_issue23_ni_u8(rdram, obj, 0x08), lod_issue23_ni_u8(rdram, obj, 0x09),
            lod_issue23_ni_u8(rdram, obj, 0x0A), lod_issue23_ni_u8(rdram, obj, 0x0B),
            lod_issue23_ni_u8(rdram, obj, 0x0C), lod_issue23_ni_u8(rdram, obj, 0x0D),
            lod_issue23_ni_u8(rdram, obj, 0x10), lod_issue23_ni_u8(rdram, obj, 0x11),
            obj24, data,
            lod_issue23_ni_u32(rdram, obj, 0x3C),
            lod_issue23_ni_u32(rdram, obj, 0x40),
            lod_issue23_ni_u32(rdram, obj, 0x44),
            data0, data4, data8, data_c,
            data28_node, data30_node,
            global_285c, global_285e, global_2860, global_2862,
            global_2864, global_2866, global_28d0, global_2b4e, global_2bc8,
            global_2bd0, flags_word, flags_pack, focus, focus24, focus28,
            loaded_0f_pair, loaded_0e_pair);

#if LOD_ENABLE_ISSUE27_NI129_TRACE
    if (map_rom == LOD_ISSUE27_POST_HARPY_MAP_ROM) {
        lod_issue27_log_portal_node(rdram, tag, count, func, obj, "obj24", obj24);
        lod_issue27_log_portal_node(rdram, tag, count, func, obj, "data+28", data28_node);
        lod_issue27_log_portal_node(rdram, tag, count, func, obj, "data+30", data30_node);
        if (focus24 != 0 && focus24 != data28_node) {
            lod_issue27_log_portal_node(rdram, tag, count, func, obj, "focus24", focus24);
        }
        if (focus28 != 0 && focus28 != data30_node && focus28 != focus24) {
            lod_issue27_log_portal_node(rdram, tag, count, func, obj, "focus28", focus28);
        }
        if (data0 != 0 && data0 != obj24) {
            lod_issue27_log_portal_node(rdram, tag, count, func, obj, "data0", data0);
        }
        if (data8 != 0 && data8 != obj24 && data8 != data0) {
            lod_issue27_log_portal_node(rdram, tag, count, func, obj, "data8", data8);
        }
    }
#endif
#else
    (void)rdram;
    (void)tag;
    (void)count;
    (void)func;
    (void)obj;
    (void)ra;
#endif
}

static void lod_issue23_force_rollover_if_enabled(uint8_t* rdram, const char* func,
                                                  uint32_t count, uint32_t obj,
                                                  uint32_t ra) {
#if LOD_ENABLE_ISSUE23_FORCE_DIRECT_285E
    static bool forced = false;
    if (forced ||
        lod_current_map_overlay_rom() != LOD_ISSUE23_DEST_MAP_ROM ||
        std::strcmp(func, "state_0F0000C8") != 0 ||
        count < 120) {
        return;
    }

    const int16_t global_285c = lod_issue23_ni_s16(rdram, 0x801C82C0, 0x285C);
    const int16_t global_285e = lod_issue23_ni_s16(rdram, 0x801C82C0, 0x285E);
    const uint32_t obj_3c = lod_issue23_ni_u32(rdram, obj, 0x3C);
    if (global_285c != 0 || (uint32_t)(int32_t)global_285e != obj_3c) {
        return;
    }

    const int16_t forced_285e = (int16_t)(global_285e + 1);
    MEM_H(0x285E, lod_ni_canonical_gpr(0x801C82C0)) = forced_285e;
    forced = true;
    fprintf(stderr,
            "[ISSUE23_FORCE] forced 285E %d->%d at stuck gate func=%s "
            "count=%u map#%d map=0x%08X obj=0x%08X obj3c=0x%08X ra=0x%08X "
            "gs=%d exec=0x%08X ni=0x%08X loaded0f=%d loaded0e=%d\n",
            global_285e, forced_285e, func, count,
            lod_current_map_overlay_load_count(), lod_current_map_overlay_rom(),
            obj, obj_3c, ra, lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram), lod_ni_telemetry_ni_sys_ptr(rdram),
            loaded_0f_pair, loaded_0e_pair);
#else
    (void)rdram;
    (void)func;
    (void)count;
    (void)obj;
    (void)ra;
#endif
}

static void lod_issue27_force_285e_if_enabled(uint8_t* rdram, const char* func,
                                              uint32_t count, uint32_t obj,
                                              uint32_t ra) {
#if LOD_ENABLE_ISSUE27_FORCE_DIRECT_285E
    static bool forced = false;
    if (forced ||
        lod_current_map_overlay_rom() != LOD_ISSUE27_POST_HARPY_MAP_ROM ||
        std::strcmp(func, "state_0F0000C8") != 0 ||
        count < 120 ||
        lod_ni_overlay_loaded_0f_pair() != 129 ||
        lod_ni_overlay_loaded_0e_pair() != 205 ||
        lod_ni_telemetry_gamestate(rdram) != 3 ||
        (lod_ni_telemetry_exec_flags(rdram) & 0x38000000) != 0x38000000) {
        return;
    }

    const int16_t global_285c = lod_issue23_ni_s16(rdram, 0x801C82C0, 0x285C);
    const int16_t global_285e = lod_issue23_ni_s16(rdram, 0x801C82C0, 0x285E);
    const uint32_t obj_3c = lod_issue23_ni_u32(rdram, obj, 0x3C);
    if (global_285c != 0 || (uint32_t)(int32_t)global_285e != obj_3c) {
        return;
    }

    const uint32_t flags_word = lod_issue23_ni_child_flag_word(rdram);
    const uint32_t flags_pack = lod_issue23_ni_child_flag_pack(flags_word);
    if (flags_pack == 0x3F) {
        return;
    }

    const int16_t forced_285e = (int16_t)(global_285e + 1);
    MEM_H(0x285E, lod_ni_canonical_gpr(0x801C82C0)) = forced_285e;
    forced = true;
    fprintf(stderr,
            "[ISSUE27_FORCE] forced 285E %d->%d at stuck NI129 gate func=%s "
            "count=%u map#%d map=0x%08X obj=0x%08X obj3c=0x%08X ra=0x%08X "
            "flagsWord=0x%08X flagsPack=0x%02X gs=%d exec=0x%08X ni=0x%08X "
            "loaded0f=%d loaded0e=%d\n",
            global_285e, forced_285e, func, count,
            lod_current_map_overlay_load_count(), lod_current_map_overlay_rom(),
            obj, obj_3c, ra, flags_word, flags_pack,
            lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram),
            lod_ni_telemetry_ni_sys_ptr(rdram),
            loaded_0f_pair, loaded_0e_pair);
#else
    (void)rdram;
    (void)func;
    (void)count;
    (void)obj;
    (void)ra;
#endif
}

static void lod_issue23_force_child_flags_if_enabled(uint8_t* rdram, const char* func,
                                                     uint32_t count, uint32_t obj,
                                                     uint32_t ra) {
#if LOD_ENABLE_ISSUE23_FORCE_CHILD_FLAGS
    static bool forced = false;
    if (forced ||
        lod_current_map_overlay_rom() != LOD_ISSUE23_DEST_MAP_ROM ||
        std::strcmp(func, "state_0F0000C8") != 0) {
        return;
    }

    constexpr gpr flag_base = (gpr)(int32_t)0x801CAA60;
    constexpr int32_t child_flag_word_off = (0x2A0 >> 5) * 4;
    constexpr uint32_t child_flags_mask =
        (0x80000000u >> (0x2A4 & 0x1F)) |
        (0x80000000u >> (0x2A5 & 0x1F)) |
        (0x80000000u >> (0x2A6 & 0x1F)) |
        (0x80000000u >> (0x2A7 & 0x1F)) |
        (0x80000000u >> (0x2A8 & 0x1F)) |
        (0x80000000u >> (0x2A9 & 0x1F));
    const uint32_t before_word = (uint32_t)MEM_W(child_flag_word_off, flag_base);
    const uint32_t after_word = before_word | child_flags_mask;
    MEM_W(child_flag_word_off, flag_base) = (int32_t)after_word;
    forced = true;

    fprintf(stderr,
            "[ISSUE23_FORCE] forced child flags 0x2A4..0x2A9 at %s "
            "count=%u map#%d map=0x%08X obj=0x%08X ra=0x%08X "
            "wordOff=0x%X mask=0x%08X before=0x%08X after=0x%08X "
            "gs=%d exec=0x%08X ni=0x%08X loaded0f=%d loaded0e=%d\n",
            func, count, lod_current_map_overlay_load_count(),
            lod_current_map_overlay_rom(), obj, ra, child_flag_word_off,
            child_flags_mask, before_word, after_word,
            lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram),
            lod_ni_telemetry_ni_sys_ptr(rdram),
            loaded_0f_pair, loaded_0e_pair);
#else
    (void)rdram;
    (void)func;
    (void)count;
    (void)obj;
    (void)ra;
#endif
}

static void lod_issue27_force_child_flags_if_enabled(uint8_t* rdram, const char* func,
                                                     uint32_t count, uint32_t obj,
                                                     uint32_t ra) {
#if LOD_ENABLE_ISSUE27_FORCE_CHILD_FLAGS
    static bool forced = false;
    if (forced ||
        lod_current_map_overlay_rom() != LOD_ISSUE27_POST_HARPY_MAP_ROM ||
        std::strcmp(func, "state_0F0000C8") != 0) {
        return;
    }

    constexpr gpr flag_base = (gpr)(int32_t)0x801CAA60;
    constexpr int32_t child_flag_word_off = (0x2A0 >> 5) * 4;
    constexpr uint32_t missing_child_flags_mask =
        (0x80000000u >> (0x2A5 & 0x1F)) |
        (0x80000000u >> (0x2A8 & 0x1F));
    const uint32_t before_word = (uint32_t)MEM_W(child_flag_word_off, flag_base);
    const uint32_t after_word = before_word | missing_child_flags_mask;
    MEM_W(child_flag_word_off, flag_base) = (int32_t)after_word;
    forced = true;

    fprintf(stderr,
            "[ISSUE27_FORCE] forced missing child flags 0x2A5/0x2A8 at %s "
            "count=%u map#%d map=0x%08X obj=0x%08X ra=0x%08X "
            "wordOff=0x%X mask=0x%08X before=0x%08X after=0x%08X "
            "gs=%d exec=0x%08X ni=0x%08X loaded0f=%d loaded0e=%d\n",
            func, count, lod_current_map_overlay_load_count(),
            lod_current_map_overlay_rom(), obj, ra, child_flag_word_off,
            missing_child_flags_mask, before_word, after_word,
            lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram),
            lod_ni_telemetry_ni_sys_ptr(rdram),
            loaded_0f_pair, loaded_0e_pair);
#else
    (void)rdram;
    (void)func;
    (void)count;
    (void)obj;
    (void)ra;
#endif
}

static void lod_issue23_ni_trace_call(uint8_t* rdram, recomp_context* ctx,
                                      const char* func, recomp_func_t* original,
                                      uint32_t* counter) {
    (*counter)++;
    const uint32_t count = *counter;
    const uint32_t obj = (uint32_t)ctx->r4;
    const uint32_t ra = (uint32_t)ctx->r31;
    lod_issue23_log_ni_state(rdram, "pre", count, func, obj, ra);
    lod_issue23_force_rollover_if_enabled(rdram, func, count, obj, ra);
    lod_issue27_force_285e_if_enabled(rdram, func, count, obj, ra);
    lod_issue23_force_child_flags_if_enabled(rdram, func, count, obj, ra);
    lod_issue27_force_child_flags_if_enabled(rdram, func, count, obj, ra);
    if (original != nullptr) {
        original(rdram, ctx);
    }
    lod_issue23_log_ni_state(rdram, "post", count, func, obj, ra);
}

static void lod_trace_issue23_ni129_entry(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "entry_0F000000",
        lod_orig_issue23_ni129_entry, &counter);
}

static void lod_trace_issue23_ni129_0070(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "state_0F000070",
        lod_orig_issue23_ni129_0070, &counter);
}

static void lod_trace_issue23_ni129_00C8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "state_0F0000C8",
        lod_orig_issue23_ni129_00C8, &counter);
}

static void lod_trace_issue23_ni129_0244(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "state_0F000244",
        lod_orig_issue23_ni129_0244, &counter);
}

static void lod_trace_issue23_ni129_04A8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "state_0F0004A8",
        lod_orig_issue23_ni129_04A8, &counter);
}

static void lod_trace_issue23_ni129_068C(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "state_0F00068C",
        lod_orig_issue23_ni129_068C, &counter);
}

static void lod_trace_issue23_ni129_07FC(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "state_0F0007FC",
        lod_orig_issue23_ni129_07FC, &counter);
}

static void lod_trace_issue23_ni129_0938(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue23_ni_trace_call(rdram, ctx, "helper_0F000938",
        lod_orig_issue23_ni129_0938, &counter);
}

static void lod_install_issue23_ni129_trace_wrapper(uint32_t func_vram,
                                                    recomp_func_t* wrapper,
                                                    recomp_func_t** original_out,
                                                    const char* name,
                                                    const char* reason) {
    recomp_func_t* current = get_function((int32_t)func_vram);
    if (current == wrapper) {
        return;
    }

    *original_out = current;
    recomp::overlays::add_loaded_function((int32_t)func_vram, wrapper);
    const uint32_t map_rom = lod_current_map_overlay_rom();
    fprintf(stderr,
            "[%s_STATE] installed %s wrapper vram=0x%08X reason=%s original=%p wrapper=%p\n",
            lod_issue23_ni_trace_prefix(map_rom), name, func_vram, reason,
            (void*)current, (void*)wrapper);
}

static void lod_install_issue23_ni129_trace_wrappers(int pair_index, uint32_t vram,
                                                     const char* reason) {
    if (!lod_issue23_ni_focus(pair_index, vram)) {
        return;
    }

    lod_install_issue23_ni129_trace_wrapper(0x0F000000,
        lod_trace_issue23_ni129_entry, &lod_orig_issue23_ni129_entry,
        "entry", reason);
    lod_install_issue23_ni129_trace_wrapper(0x0F000070,
        lod_trace_issue23_ni129_0070, &lod_orig_issue23_ni129_0070,
        "0F000070", reason);
    lod_install_issue23_ni129_trace_wrapper(0x0F0000C8,
        lod_trace_issue23_ni129_00C8, &lod_orig_issue23_ni129_00C8,
        "0F0000C8", reason);
    lod_install_issue23_ni129_trace_wrapper(0x0F000244,
        lod_trace_issue23_ni129_0244, &lod_orig_issue23_ni129_0244,
        "0F000244", reason);
    lod_install_issue23_ni129_trace_wrapper(0x0F0004A8,
        lod_trace_issue23_ni129_04A8, &lod_orig_issue23_ni129_04A8,
        "0F0004A8", reason);
    lod_install_issue23_ni129_trace_wrapper(0x0F00068C,
        lod_trace_issue23_ni129_068C, &lod_orig_issue23_ni129_068C,
        "0F00068C", reason);
    lod_install_issue23_ni129_trace_wrapper(0x0F0007FC,
        lod_trace_issue23_ni129_07FC, &lod_orig_issue23_ni129_07FC,
        "0F0007FC", reason);
    lod_install_issue23_ni129_trace_wrapper(0x0F000938,
        lod_trace_issue23_ni129_0938, &lod_orig_issue23_ni129_0938,
        "0F000938", reason);
}
#endif

#if LOD_ENABLE_ISSUE27_PAIR51_TRACE
static constexpr uint32_t LOD_ISSUE27_PAIR51_MAP_ROM = 0x007D7B90;
static constexpr int LOD_ISSUE27_PAIR51_NI_PAIR = 51;

static recomp_func_t* lod_orig_issue27_pair51_entry = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0070 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0194 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0868 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0928 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0AF4 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0C60 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0D48 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_0F40 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_1074 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_10D8 = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_119C = nullptr;
static recomp_func_t* lod_orig_issue27_pair51_1344 = nullptr;

static bool lod_issue27_pair51_focus(int pair_index, uint32_t vram) {
    return pair_index == LOD_ISSUE27_PAIR51_NI_PAIR &&
           vram == 0x0F000000 &&
           lod_current_map_overlay_rom() == LOD_ISSUE27_PAIR51_MAP_ROM;
}

static bool lod_issue27_pair51_should_log(uint32_t count) {
    return count <= 80 || (count % 120) == 0;
}

static bool lod_issue27_pair51_addr_ok(uint32_t base, int32_t off, uint32_t size) {
    const uint32_t addr = (uint32_t)((int64_t)(int32_t)base + off);
    const uint32_t phys = addr & 0x1FFFFFFF;
    return lod_ni_telemetry_range_ok(phys, size);
}

static uint32_t lod_issue27_pair51_u32(uint8_t* rdram, uint32_t base, int32_t off) {
    if (base == 0 || !lod_issue27_pair51_addr_ok(base, off, 4)) {
        return 0;
    }
    return (uint32_t)MEM_W(off, lod_ni_canonical_gpr(base));
}

static uint16_t lod_issue27_pair51_u16(uint8_t* rdram, uint32_t base, int32_t off) {
    if (base == 0 || !lod_issue27_pair51_addr_ok(base, off, 2)) {
        return 0;
    }
    return (uint16_t)MEM_HU(off, lod_ni_canonical_gpr(base));
}

static int16_t lod_issue27_pair51_s16(uint8_t* rdram, uint32_t base, int32_t off) {
    if (base == 0 || !lod_issue27_pair51_addr_ok(base, off, 2)) {
        return 0;
    }
    return (int16_t)MEM_H(off, lod_ni_canonical_gpr(base));
}

static uint8_t lod_issue27_pair51_u8(uint8_t* rdram, uint32_t base, int32_t off) {
    if (base == 0 || !lod_issue27_pair51_addr_ok(base, off, 1)) {
        return 0;
    }
    return (uint8_t)MEM_BU(off, lod_ni_canonical_gpr(base));
}

static uint32_t lod_issue27_pair51_dispatch_target(uint8_t* rdram, uint8_t index) {
    return (uint32_t)MEM_W((int32_t)index * 4, lod_ni_canonical_gpr(0x0F003C98));
}

static uint32_t lod_issue27_pair51_flag_word(uint8_t* rdram) {
    constexpr gpr flag_base = (gpr)(int32_t)0x801CAA60;
    constexpr int32_t child_flag_word_off = (0x2A0 >> 5) * 4;
    return (uint32_t)MEM_W(child_flag_word_off, flag_base);
}

static uint32_t lod_issue27_pair51_flag_pack(uint32_t word) {
    uint32_t pack = 0;
    for (uint32_t flag = 0x2A4; flag <= 0x2A9; flag++) {
        const uint32_t bit = 0x80000000u >> (flag & 0x1F);
        if ((word & bit) != 0) {
            pack |= 1u << (flag - 0x2A4);
        }
    }
    return pack;
}

static void lod_issue27_pair51_log_state(uint8_t* rdram, const char* tag,
                                         uint32_t count, const char* func,
                                         uint32_t obj, uint32_t ra) {
    if (!lod_issue27_pair51_should_log(count)) {
        return;
    }

    const int16_t depth = lod_issue27_pair51_s16(rdram, obj, 0x0E);
    const int next_depth = (int)depth + 1;
    const uint32_t next_off = next_depth >= 0 && next_depth < 8
        ? (uint32_t)(next_depth * 2)
        : 0;
    const uint8_t next_count = next_depth >= 0 && next_depth < 8
        ? lod_issue27_pair51_u8(rdram, obj, 0x08 + (int32_t)next_off)
        : 0;
    const uint8_t next_index = next_depth >= 0 && next_depth < 8
        ? lod_issue27_pair51_u8(rdram, obj, 0x09 + (int32_t)next_off)
        : 0;
    const uint32_t target = lod_issue27_pair51_dispatch_target(rdram, next_index);

    const uint32_t data34 = lod_issue27_pair51_u32(rdram, obj, 0x34);
    const uint32_t data38 = lod_issue27_pair51_u32(rdram, obj, 0x38);
    const uint32_t child = lod_issue27_pair51_u32(rdram, obj, 0x24);
    const uint32_t aux4c = lod_issue27_pair51_u32(rdram, obj, 0x4C);
    const uint32_t aux54 = lod_issue27_pair51_u32(rdram, obj, 0x54);
    const uint32_t actor64 = lod_issue27_pair51_u32(rdram, obj, 0x64);
    const uint32_t data70 = lod_issue27_pair51_u32(rdram, obj, 0x70);
    const uint16_t data70_flag = lod_issue27_pair51_u16(rdram, data70, 0x14);
    const uint16_t data70_state = lod_issue27_pair51_u16(rdram, data70, 0x18);
    const uint32_t global_base = 0x801C82C0;
    const int16_t global_28d0 = lod_issue27_pair51_s16(rdram, global_base, 0x28D0);
    const int16_t global_2b4e = lod_issue27_pair51_s16(rdram, global_base, 0x2B4E);
    const uint32_t global_2bc8 = lod_issue27_pair51_u32(rdram, global_base, 0x2BC8);
    const uint32_t global_2bd0 = lod_issue27_pair51_u32(rdram, global_base, 0x2BD0);
    const uint32_t flags_word = lod_issue27_pair51_flag_word(rdram);
    const uint32_t flags_pack = lod_issue27_pair51_flag_pack(flags_word);

    fprintf(stderr,
            "[ISSUE27_PAIR51] %s#%u func=%s gs=%d exec=0x%08X ni=0x%08X "
            "map#%d map=0x%08X obj=0x%08X ra=0x%08X depth=%d "
            "next={count=%u idx=%u target=0x%08X} levels=%u/%u,%u/%u,%u/%u,%u/%u "
            "fields={obj2=0x%04X child=0x%08X data34=0x%08X data38=0x%08X "
            "obj3c=0x%08X aux4c=0x%08X aux54=0x%08X actor64=0x%08X data70=0x%08X "
            "d70flag=0x%04X d70state=0x%04X} globals={28d0=%d 2b4e=%d "
            "2bc8=0x%08X 2bd0=0x%08X flagsWord=0x%08X flagsPack=0x%02X "
            "loaded0f=%d loaded0e=%d}\n",
            tag, count, func, lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram), lod_ni_telemetry_ni_sys_ptr(rdram),
            lod_current_map_overlay_load_count(), lod_current_map_overlay_rom(),
            obj, ra, (int)depth, next_count, next_index, target,
            lod_issue27_pair51_u8(rdram, obj, 0x08), lod_issue27_pair51_u8(rdram, obj, 0x09),
            lod_issue27_pair51_u8(rdram, obj, 0x0A), lod_issue27_pair51_u8(rdram, obj, 0x0B),
            lod_issue27_pair51_u8(rdram, obj, 0x0C), lod_issue27_pair51_u8(rdram, obj, 0x0D),
            lod_issue27_pair51_u8(rdram, obj, 0x10), lod_issue27_pair51_u8(rdram, obj, 0x11),
            lod_issue27_pair51_u16(rdram, obj, 0x02), child, data34, data38,
            lod_issue27_pair51_u32(rdram, obj, 0x3C), aux4c, aux54, actor64,
            data70, data70_flag, data70_state,
            global_28d0, global_2b4e, global_2bc8, global_2bd0,
            flags_word, flags_pack, loaded_0f_pair, loaded_0e_pair);
}

static void lod_issue27_pair51_trace_call(uint8_t* rdram, recomp_context* ctx,
                                          const char* func, recomp_func_t* original,
                                          uint32_t* counter) {
    (*counter)++;
    const uint32_t count = *counter;
    const uint32_t obj = (uint32_t)ctx->r4;
    const uint32_t ra = (uint32_t)ctx->r31;

    lod_issue27_pair51_log_state(rdram, "pre", count, func, obj, ra);
    if (original != nullptr) {
        original(rdram, ctx);
    }
    lod_issue27_pair51_log_state(rdram, "post", count, func, obj, ra);
}

static void lod_trace_issue27_pair51_entry(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "entry_0F000000",
        lod_orig_issue27_pair51_entry, &counter);
}

static void lod_trace_issue27_pair51_0070(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000070",
        lod_orig_issue27_pair51_0070, &counter);
}

static void lod_trace_issue27_pair51_0194(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000194",
        lod_orig_issue27_pair51_0194, &counter);
}

static void lod_trace_issue27_pair51_0868(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000868",
        lod_orig_issue27_pair51_0868, &counter);
}

static void lod_trace_issue27_pair51_0928(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000928",
        lod_orig_issue27_pair51_0928, &counter);
}

static void lod_trace_issue27_pair51_0AF4(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000AF4",
        lod_orig_issue27_pair51_0AF4, &counter);
}

static void lod_trace_issue27_pair51_0C60(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000C60",
        lod_orig_issue27_pair51_0C60, &counter);
}

static void lod_trace_issue27_pair51_0D48(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000D48",
        lod_orig_issue27_pair51_0D48, &counter);
}

static void lod_trace_issue27_pair51_0F40(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F000F40",
        lod_orig_issue27_pair51_0F40, &counter);
}

static void lod_trace_issue27_pair51_1074(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F001074",
        lod_orig_issue27_pair51_1074, &counter);
}

static void lod_trace_issue27_pair51_10D8(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F0010D8",
        lod_orig_issue27_pair51_10D8, &counter);
}

static void lod_trace_issue27_pair51_119C(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F00119C",
        lod_orig_issue27_pair51_119C, &counter);
}

static void lod_trace_issue27_pair51_1344(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_issue27_pair51_trace_call(rdram, ctx, "state_0F001344",
        lod_orig_issue27_pair51_1344, &counter);
}

static void lod_install_issue27_pair51_trace_wrapper(uint32_t func_vram,
                                                     recomp_func_t* wrapper,
                                                     recomp_func_t** original_out,
                                                     const char* name,
                                                     const char* reason) {
    recomp_func_t* current = get_function((int32_t)func_vram);
    if (current == wrapper) {
        return;
    }

    *original_out = current;
    recomp::overlays::add_loaded_function((int32_t)func_vram, wrapper);
    fprintf(stderr,
            "[ISSUE27_PAIR51] installed %s wrapper vram=0x%08X reason=%s original=%p wrapper=%p\n",
            name, func_vram, reason, (void*)current, (void*)wrapper);
}

static void lod_install_issue27_pair51_trace_wrappers(int pair_index, uint32_t vram,
                                                      const char* reason) {
    if (!lod_issue27_pair51_focus(pair_index, vram)) {
        return;
    }

    lod_install_issue27_pair51_trace_wrapper(0x0F000000,
        lod_trace_issue27_pair51_entry, &lod_orig_issue27_pair51_entry,
        "entry", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000070,
        lod_trace_issue27_pair51_0070, &lod_orig_issue27_pair51_0070,
        "0F000070", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000194,
        lod_trace_issue27_pair51_0194, &lod_orig_issue27_pair51_0194,
        "0F000194", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000868,
        lod_trace_issue27_pair51_0868, &lod_orig_issue27_pair51_0868,
        "0F000868", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000928,
        lod_trace_issue27_pair51_0928, &lod_orig_issue27_pair51_0928,
        "0F000928", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000AF4,
        lod_trace_issue27_pair51_0AF4, &lod_orig_issue27_pair51_0AF4,
        "0F000AF4", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000C60,
        lod_trace_issue27_pair51_0C60, &lod_orig_issue27_pair51_0C60,
        "0F000C60", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000D48,
        lod_trace_issue27_pair51_0D48, &lod_orig_issue27_pair51_0D48,
        "0F000D48", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F000F40,
        lod_trace_issue27_pair51_0F40, &lod_orig_issue27_pair51_0F40,
        "0F000F40", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F001074,
        lod_trace_issue27_pair51_1074, &lod_orig_issue27_pair51_1074,
        "0F001074", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F0010D8,
        lod_trace_issue27_pair51_10D8, &lod_orig_issue27_pair51_10D8,
        "0F0010D8", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F00119C,
        lod_trace_issue27_pair51_119C, &lod_orig_issue27_pair51_119C,
        "0F00119C", reason);
    lod_install_issue27_pair51_trace_wrapper(0x0F001344,
        lod_trace_issue27_pair51_1344, &lod_orig_issue27_pair51_1344,
        "0F001344", reason);
}
#endif

#if LOD_ENABLE_ISSUE27_PAUSE_GATE_TRACE
static recomp_func_t* lod_orig_issue27_pause_pair106_0468 = nullptr;
static recomp_func_t* lod_orig_issue27_pause_pair109_0390 = nullptr;
static recomp_func_t* lod_orig_issue27_pause_pair120_0298 = nullptr;

static constexpr uint32_t LOD_ISSUE27_PAUSE_SYS_BASE = 0x801C82C0;
static constexpr int32_t LOD_ISSUE27_PAUSE_SYS_INPUT_OFF = 0x0538;
static constexpr uint16_t LOD_ISSUE27_PAUSE_MASK = 0x1080;

static uint16_t lod_issue27_pause_u16(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return (uint16_t)MEM_HU(off, lod_ni_canonical_gpr(base));
}

static uint32_t lod_issue27_pause_u32(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return (uint32_t)MEM_W(off, lod_ni_canonical_gpr(base));
}

static bool lod_issue27_pause_focus_map(uint8_t* rdram) {
    return lod_current_map_overlay_rom() == LOD_ISSUE27_POST_HARPY_MAP_ROM &&
           lod_ni_telemetry_gamestate(rdram) == 3;
}

static bool lod_issue27_pause_pair_focus(int pair_index, uint32_t vram) {
    return vram == 0x0F000000 &&
           (pair_index == 106 || pair_index == 109 || pair_index == 120) &&
           lod_current_map_overlay_rom() == LOD_ISSUE27_POST_HARPY_MAP_ROM;
}

static bool lod_issue27_pause_should_log(uint32_t count,
                                         uint16_t input_pre,
                                         uint16_t input_post) {
    return count <= 12 ||
           ((input_pre | input_post) & LOD_ISSUE27_PAUSE_MASK) != 0 ||
           (count % 300) == 0;
}

static bool lod_issue27_pause_ram_ok(uint32_t base, int32_t off, uint32_t size) {
    if (base == 0) {
        return false;
    }
    const uint32_t addr = (uint32_t)((int64_t)(int32_t)base + off);
    const uint32_t phys = addr & 0x1FFFFFFF;
    return lod_ni_telemetry_range_ok(phys, size);
}

static uint8_t lod_issue27_pause_u8_ram(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return lod_issue27_pause_ram_ok(base, off, 1)
        ? (uint8_t)MEM_BU(off, lod_ni_canonical_gpr(base))
        : 0;
}

static uint16_t lod_issue27_pause_u16_ram(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return lod_issue27_pause_ram_ok(base, off, 2)
        ? (uint16_t)MEM_HU(off, lod_ni_canonical_gpr(base))
        : 0;
}

static uint32_t lod_issue27_pause_u32_ram(uint8_t* rdram, uint32_t base, int32_t off) {
    (void)rdram;
    return lod_issue27_pause_ram_ok(base, off, 4)
        ? (uint32_t)MEM_W(off, lod_ni_canonical_gpr(base))
        : 0;
}

static void lod_issue27_pause_log_obj(uint8_t* rdram, uint32_t obj) {
    if (!lod_issue27_pause_ram_ok(obj, 0, 0x74)) {
        fprintf(stderr, " obj={addr=0x%08X ok=0}", obj);
        return;
    }

    fprintf(stderr,
            " obj={addr=0x%08X ok=1 flags=0x%04X id=0x%04X depth=%d "
            "state=%u/%u,%u/%u,%u/%u,%u/%u links={child=0x%08X parent=0x%08X "
            "next=0x%08X} fields={24=0x%08X 34=0x%08X 38=0x%08X 3c=0x%08X "
            "40=0x%08X 44=0x%08X 48=0x%08X 4c=0x%08X 50=0x%08X 54=0x%08X "
            "64=0x%08X 70=0x%08X}}",
            obj,
            (unsigned)lod_issue27_pause_u16_ram(rdram, obj, 0x00),
            (unsigned)lod_issue27_pause_u16_ram(rdram, obj, 0x0C),
            (int)(int16_t)lod_issue27_pause_u16_ram(rdram, obj, 0x0E),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x08),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x09),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x0A),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x0B),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x0C),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x0D),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x10),
            (unsigned)lod_issue27_pause_u8_ram(rdram, obj, 0x11),
            lod_issue27_pause_u32_ram(rdram, obj, 0x24),
            lod_issue27_pause_u32_ram(rdram, obj, 0x28),
            lod_issue27_pause_u32_ram(rdram, obj, 0x2C),
            lod_issue27_pause_u32_ram(rdram, obj, 0x24),
            lod_issue27_pause_u32_ram(rdram, obj, 0x34),
            lod_issue27_pause_u32_ram(rdram, obj, 0x38),
            lod_issue27_pause_u32_ram(rdram, obj, 0x3C),
            lod_issue27_pause_u32_ram(rdram, obj, 0x40),
            lod_issue27_pause_u32_ram(rdram, obj, 0x44),
            lod_issue27_pause_u32_ram(rdram, obj, 0x48),
            lod_issue27_pause_u32_ram(rdram, obj, 0x4C),
            lod_issue27_pause_u32_ram(rdram, obj, 0x50),
            lod_issue27_pause_u32_ram(rdram, obj, 0x54),
            lod_issue27_pause_u32_ram(rdram, obj, 0x64),
            lod_issue27_pause_u32_ram(rdram, obj, 0x70));
}

static void lod_issue27_pause_log_gate(uint8_t* rdram,
                                       const char* phase,
                                       const char* func,
                                       uint32_t count,
                                       uint32_t obj,
                                       uint32_t ra,
                                       uint16_t input_pre,
                                       uint16_t input_post,
                                       uint32_t r2_post) {
    if (!lod_issue27_pause_focus_map(rdram) ||
        !lod_issue27_pause_should_log(count, input_pre, input_post)) {
        return;
    }

    fprintf(stderr,
            "[ISSUE27_PAUSE_NI] %s#%u func=%s map#%d map=0x%08X "
            "gs=%d exec=0x%08X ni=0x%08X loaded0f=%d loaded0e=%d "
            "ra=0x%08X r2post=0x%08X input={pre=0x%04X post=0x%04X "
            "held536=0x%04X h53a=0x%04X h53c=0x%04X} "
            "globals={2908=0x%08X 2bc4=0x%08X 2bc8=0x%08X "
            "2bcc=0x%08X 2bd0=0x%08X}",
            phase, count, func, lod_current_map_overlay_load_count(),
            lod_current_map_overlay_rom(), lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram), lod_ni_telemetry_ni_sys_ptr(rdram),
            loaded_0f_pair, loaded_0e_pair, ra, r2_post, input_pre, input_post,
            lod_issue27_pause_u16(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x0536),
            lod_issue27_pause_u16(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x053A),
            lod_issue27_pause_u16(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x053C),
            lod_issue27_pause_u32(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x2908),
            lod_issue27_pause_u32(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x2BC4),
            lod_issue27_pause_u32(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x2BC8),
            lod_issue27_pause_u32(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x2BCC),
            lod_issue27_pause_u32(rdram, LOD_ISSUE27_PAUSE_SYS_BASE, 0x2BD0));
    lod_issue27_pause_log_obj(rdram, obj);
    fprintf(stderr, "\n");
}

static void lod_issue27_pause_trace_call(uint8_t* rdram,
                                         recomp_context* ctx,
                                         const char* func,
                                         recomp_func_t* original,
                                         uint32_t* counter) {
    (*counter)++;
    const uint32_t count = *counter;
    const uint32_t obj = (uint32_t)ctx->r4;
    const uint32_t ra = (uint32_t)ctx->r31;
    const uint16_t input_pre = lod_issue27_pause_u16(
        rdram, LOD_ISSUE27_PAUSE_SYS_BASE, LOD_ISSUE27_PAUSE_SYS_INPUT_OFF);

    if (original != nullptr) {
        original(rdram, ctx);
    }

    const uint16_t input_post = lod_issue27_pause_u16(
        rdram, LOD_ISSUE27_PAUSE_SYS_BASE, LOD_ISSUE27_PAUSE_SYS_INPUT_OFF);
    lod_issue27_pause_log_gate(rdram, "call", func, count, obj, ra,
                               input_pre, input_post, (uint32_t)ctx->r2);
}

static void lod_trace_issue27_pause_pair106_0468(uint8_t* rdram,
                                                 recomp_context* ctx) {
    static uint32_t count = 0;
    lod_issue27_pause_trace_call(rdram, ctx, "pair106.0F000468",
        lod_orig_issue27_pause_pair106_0468, &count);
}

static void lod_trace_issue27_pause_pair109_0390(uint8_t* rdram,
                                                 recomp_context* ctx) {
    static uint32_t count = 0;
    lod_issue27_pause_trace_call(rdram, ctx, "pair109.0F000390",
        lod_orig_issue27_pause_pair109_0390, &count);
}

static void lod_trace_issue27_pause_pair120_0298(uint8_t* rdram,
                                                 recomp_context* ctx) {
    static uint32_t count = 0;
    lod_issue27_pause_trace_call(rdram, ctx, "pair120.0F000298",
        lod_orig_issue27_pause_pair120_0298, &count);
}

static void lod_install_issue27_pause_trace_wrapper(uint32_t func_vram,
                                                    recomp_func_t* wrapper,
                                                    recomp_func_t** original_out,
                                                    const char* name,
                                                    const char* reason) {
    recomp_func_t* current = get_function((int32_t)func_vram);
    if (current == wrapper) {
        return;
    }

    *original_out = current;
    recomp::overlays::add_loaded_function((int32_t)func_vram, wrapper);
    fprintf(stderr,
            "[ISSUE27_PAUSE_NI] installed %s wrapper vram=0x%08X "
            "reason=%s original=%p wrapper=%p map#%d map=0x%08X\n",
            name, func_vram, reason, (void*)current, (void*)wrapper,
            lod_current_map_overlay_load_count(), lod_current_map_overlay_rom());
}

static void lod_install_issue27_pause_trace_wrappers(int pair_index,
                                                     uint32_t vram,
                                                     const char* reason) {
    if (!lod_issue27_pause_pair_focus(pair_index, vram)) {
        return;
    }

    if (pair_index == 106) {
        lod_install_issue27_pause_trace_wrapper(0x0F000468,
            lod_trace_issue27_pause_pair106_0468,
            &lod_orig_issue27_pause_pair106_0468,
            "pair106.0F000468", reason);
    }
    else if (pair_index == 109) {
        lod_install_issue27_pause_trace_wrapper(0x0F000390,
            lod_trace_issue27_pause_pair109_0390,
            &lod_orig_issue27_pause_pair109_0390,
            "pair109.0F000390", reason);
    }
    else if (pair_index == 120) {
        lod_install_issue27_pause_trace_wrapper(0x0F000298,
            lod_trace_issue27_pause_pair120_0298,
            &lod_orig_issue27_pause_pair120_0298,
            "pair120.0F000298", reason);
    }
}
#endif

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

static void unload_ni_overlay_segment(uint8_t* rdram, uint32_t vram) {
    int* loaded_pair = ni_overlay_loaded_pair_for_vram(vram);
    if (loaded_pair == nullptr || *loaded_pair < 0) {
        return;
    }

#if LOD_ENABLE_NI0E_TRACE
    if (vram == 0x0E000000) {
        lod_ni0e_log_slot_event(rdram, "unload", *loaded_pair, *loaded_pair, 0, 0);
    }
#endif

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

#if LOD_FIX_NI_PAIR23_INTERNAL_LABELS
static float lod_ni_u32_as_f32(uint32_t value) {
    float ret = 0.0f;
    memcpy(&ret, &value, sizeof(ret));
    return ret;
}

static uint32_t lod_ni_f32_as_u32(float value) {
    uint32_t ret = 0;
    memcpy(&ret, &value, sizeof(ret));
    return ret;
}

static int32_t lod_ni_trunc_nonnegative_f32_to_i32(float value) {
    if (!(value >= 0.0f) || !std::isfinite(value) || value >= 2147483648.0f) {
        return -1;
    }
    return (int32_t)value;
}

static void lod_ni_pair23_internal_label_0F004A24(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;

    // Original internal label 0x0F004A24 inside ni_ovl_023_func_0F004744.
    // The pair-23 sub-dispatch table at 0x0F006FC8 uses this label as state 1.
    // N64Recomp's range fallback resolves it to function start 0x0F004744,
    // re-running state-0 initialization instead of this update path.
    const gpr obj = lod_ni_canonical_gpr(ctx->r4);
    const gpr data0 = lod_ni_canonical_gpr(MEM_W(0x34, obj));
    const gpr work = lod_ni_canonical_gpr(MEM_W(0x24, obj));

    float timer = lod_ni_u32_as_f32((uint32_t)MEM_W(0x1B0, data0));
    timer += 25.5f;
    MEM_W(0x1B0, data0) = lod_ni_f32_as_u32(timer);

    const gpr child = lod_ni_canonical_gpr(MEM_W(0x14, work));
    const int32_t timer_byte = lod_ni_trunc_nonnegative_f32_to_i32(timer);
    MEM_B(0x1B, child) = timer_byte;
    MEM_B(0x19, child) = timer_byte;
    MEM_B(0x18, child) = lod_ni_trunc_nonnegative_f32_to_i32(timer * 0.2f);

    if (timer >= 254.0f) {
        MEM_B(0x9, obj) = 2;
    }
}

static void lod_call_ni_pair23_spawn_callback(uint8_t* rdram, recomp_context* ctx,
                                               uint32_t state, uint32_t target) {
    switch (target) {
        case 0x0F004744:
            LOOKUP_FUNC((int32_t)target)(rdram, ctx);
            return;
        case 0x0F004A24:
            lod_ni_pair23_internal_label_0F004A24(rdram, ctx);
            return;
        default:
            static int warn_count = 0;
            warn_count++;
            if (warn_count <= 8 || (warn_count % 100) == 0) {
                fprintf(stderr,
                    "[PAIR23-FIX] skipped invalid spawn callback state=%u target=0x%08X (#%d)\n",
                    state, target, warn_count);
            }
            return;
    }
}

static void lod_fixed_ni_pair23_spawn_dispatch(uint8_t* rdram, recomp_context* ctx) {
    // Scoped compatibility shim for ni_ovl_023_func_0F00469C.
    // Its table at 0x0F006FC8 has two valid entries. Entry 1 is an internal
    // label (0x0F004A24) inside 0x0F004744; if it falls through the generic
    // range fallback, state 1 re-enters state-0 initialization and can advance
    // to state 2, whose table word is adjacent string data ("No memory...").
    ctx->r29 = ADD32(ctx->r29, -0x18);
    MEM_W(0x14, ctx->r29) = ctx->r31;

    ctx->r2 = MEM_W(ctx->r4, 0x34);
    ctx->f0.u32l = 0;
    ctx->r1 = S32(0x4000 << 16);
    ctx->r15 = MEM_BU(ctx->r2, 0xAC);
    MEM_W(0x128, ctx->r2) = ctx->f0.u32l;
    MEM_W(0x12C, ctx->r2) = ctx->f0.u32l;
    ctx->r24 = ctx->r15 | 0x8;
    MEM_B(0xAC, ctx->r2) = ctx->r24;
    MEM_W(0xA4, ctx->r2) = ctx->f0.u32l;
    MEM_W(0x9C, ctx->r2) = ctx->f0.u32l;

    ctx->r5 = MEM_W(ctx->r4, 0x3C);
    ctx->r25 = MEM_W(ctx->r5, 0x60);
    ctx->r8 = ctx->r25 | ctx->r1;
    MEM_W(0x60, ctx->r5) = ctx->r8;

    ctx->r6 = MEM_H(ctx->r4, 0xE);
    ctx->r25 = S32(0x0F00 << 16);
    ctx->r6 = ADD32(ctx->r6, 0x1);
    ctx->r6 = S32(ctx->r6 << 16);
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    ctx->r9 = S32(ctx->r6 << 1);
    ctx->r3 = ADD32(ctx->r4, ctx->r9);
    MEM_H(0xE, ctx->r4) = ctx->r6;
    ctx->r10 = MEM_BU(ctx->r3, 0x8);
    ctx->r11 = ADD32(ctx->r10, 0x1);
    MEM_B(0x8, ctx->r3) = ctx->r11;
    MEM_W(0x18, ctx->r29) = ctx->r4;

    ctx->r12 = MEM_BU(ctx->r3, 0x9);
    ctx->r13 = S32(ctx->r12 << 2);
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    ctx->r25 = MEM_W(ctx->r25, 0x6FC8);

    const LodNiSavedCalleeRegs saved_callee = lod_ni_save_callee_regs(ctx);
    lod_call_ni_pair23_spawn_callback(rdram, ctx, (uint32_t)ctx->r12, (uint32_t)ctx->r25);
    lod_ni_restore_callee_regs(ctx, saved_callee);

    ctx->r4 = MEM_W(ctx->r29, 0x18);
    ctx->r14 = MEM_H(ctx->r4, 0xE);
    ctx->r15 = ADD32(ctx->r14, -0x1);
    MEM_H(0xE, ctx->r4) = ctx->r15;
    ctx->r31 = MEM_W(ctx->r29, 0x14);
    ctx->r29 = ADD32(ctx->r29, 0x18);
}

static void lod_install_ni_pair23_internal_label_fix(const char* reason) {
    recomp::overlays::add_loaded_function((int32_t)0x0F00469C,
        lod_fixed_ni_pair23_spawn_dispatch);

    static int install_count = 0;
    install_count++;
    if (install_count <= 8 || (install_count % 100) == 0) {
        fprintf(stderr,
            "[PAIR23-FIX] installed scoped spawn dispatcher fix #%d reason=%s\n",
            install_count, reason);
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

#if LOD_ENABLE_PAUSE_ITEM_TRACE
static recomp_func_t* lod_orig_pause_item_entry = nullptr;
static recomp_func_t* lod_orig_pause_item_sub_entry = nullptr;

static bool lod_pause_item_addr_phys(uint32_t addr, uint32_t size, uint32_t* phys_out) {
    if (addr == 0) {
        return false;
    }

    uint32_t phys = 0xFFFFFFFF;
    if (addr < 0x00800000) {
        phys = addr;
    } else if (addr >= 0x80000000 && addr < 0x80800000) {
        phys = addr - 0x80000000;
    } else if (addr >= 0xA0000000 && addr < 0xA0800000) {
        phys = addr - 0xA0000000;
    } else {
        return false;
    }

    if (!lod_ni_telemetry_range_ok(phys, size)) {
        return false;
    }
    if (phys_out != nullptr) {
        *phys_out = phys;
    }
    return true;
}

static uint8_t lod_pause_item_u8(uint8_t* rdram, uint32_t phys) {
    return lod_ni_telemetry_range_ok(phys, 1) ? rdram[phys ^ 3] : 0;
}

static uint16_t lod_pause_item_u16(uint8_t* rdram, uint32_t phys) {
    return lod_ni_telemetry_range_ok(phys, 2) ? *(uint16_t*)(rdram + (phys ^ 2)) : 0;
}

static int16_t lod_pause_item_s16(uint8_t* rdram, uint32_t phys) {
    return (int16_t)lod_pause_item_u16(rdram, phys);
}

static uint32_t lod_pause_item_u32(uint8_t* rdram, uint32_t phys) {
    return lod_ni_telemetry_range_ok(phys, 4) ? *(uint32_t*)(rdram + phys) : 0;
}

static float lod_pause_item_f32(uint8_t* rdram, uint32_t phys) {
    union { uint32_t u; float f; } v{};
    v.u = lod_pause_item_u32(rdram, phys);
    return v.f;
}

static uint32_t lod_pause_item_hash_step(uint32_t hash, uint32_t value) {
    hash ^= value;
    hash *= 16777619u;
    return hash;
}

static uint32_t lod_pause_item_table_target(uint8_t* rdram, uint32_t table_offset,
                                            uint32_t state_index, uint32_t max_entries) {
    // MEM_W(offset, base) expects an offset relative to the 0x0F segment base.
    if (state_index >= max_entries) {
        return 0;
    }
    return (uint32_t)MEM_W(table_offset + state_index * 4, (gpr)(int32_t)0x0F000000);
}

static uint32_t lod_pause_item_top_target(uint8_t* rdram, uint32_t state_index) {
    return lod_pause_item_table_target(rdram, 0x5BAC, state_index, 32);
}

static uint32_t lod_pause_item_sub_a_target(uint8_t* rdram, uint32_t state_index) {
    return lod_pause_item_table_target(rdram, 0x5BDC, state_index, 8);
}

static uint32_t lod_pause_item_sub_b_target(uint8_t* rdram, uint32_t state_index) {
    return lod_pause_item_table_target(rdram, 0x5BF4, state_index, 8);
}

static uint32_t lod_pause_item_pair99_item_target(uint8_t* rdram, uint32_t state_index) {
    return lod_pause_item_table_target(rdram, 0x4F6C, state_index, 8);
}

static bool lod_pause_item_is_focused_pair(int pair) {
    return pair == 45 || pair == 99 || pair == 101;
}

static uint32_t lod_pause_item_pair_target(uint8_t* rdram, int pair, uint32_t state_index) {
    switch (pair) {
        case 45:  return lod_pause_item_table_target(rdram, 0x5BAC, state_index, 32);
        case 99:  return lod_pause_item_table_target(rdram, 0x4CE8, state_index, 16);
        case 101: return lod_pause_item_table_target(rdram, 0x1488, state_index, 8);
        default:  return 0;
    }
}

static const char* lod_pause_item_state_name(uint32_t target) {
    switch (target) {
        case 0x0F000070: return "vol-down";
        case 0x0F0000E8: return "init";
        case 0x0F0005F0: return "create-main";
        case 0x0F000B18: return "calc-main";
        case 0x0F002F94: return "top04";
        case 0x0F003118: return "top05";
        case 0x0F003250: return "top06";
        case 0x0F003364: return "top07";
        case 0x0F0034B8: return "top08";
        case 0x0F003544: return "callback";
        case 0x0F003628: return "top10";
        case 0x0F00371C: return "top11";
        case 0x0F001540: return "subA0";
        case 0x0F002198: return "subA1";
        case 0x0F00236C: return "subA2";
        case 0x0F0025D4: return "subA3";
        case 0x0F0027AC: return "subA4-noop";
        case 0x0F0027B4: return "subA5";
        case 0x0F002ACC: return "subB0";
        case 0x0F002BFC: return "subB1";
        case 0x0F002DB0: return "subB2-label";
        case 0x0F002DD4: return "subB3";
        case 0x0F002F8C: return "subB4-noop";
        case 0x0F0001F8: return "p99-state01";
        case 0x0F000648: return "p99-state02";
        case 0x0F0009F8: return "p99-state03";
        case 0x0F000A40: return "p99-state04";
        case 0x0F00103C: return "p99-state05";
        case 0x0F0010A0: return "p99-state06";
        case 0x0F0010E8: return "p99-state07";
        case 0x0F001270: return "p99-state08";
        case 0x0F001350: return "p99-state09";
        case 0x0F0016BC: return "p99-state10";
        case 0x0F00192C: return "p99-state11";
        case 0x0F0019FC: return "p99-state12";
        case 0x0F001A9C: return "p99-state13";
        case 0x0F001B08: return "p99-state14";
        case 0x0F001FE8: return "p99-state15";
        case 0x0F002830: return "p99-item-create";
        case 0x0F0029BC: return "p99-item-calc";
        case 0x0F002E2C: return "p99-item-close";
        case 0x0F002FAC: return "p99-item-destroy";
        case 0x0F003008: return "p99-item-noop";
        case 0x0F00302C: return "p99-item-selected";
        case 0x0F00364C: return "p99-item-selected-close";
        case 0x0F0003A8: return "p101-state01";
        case 0x0F0004DC: return "p101-state02";
        case 0x0F00052C: return "p101-state03";
        case 0x0F000690: return "p101-state04";
        default: return "unknown";
    }
}

static uint8_t lod_pause_item_current_state_index(uint8_t* rdram, uint32_t obj) {
    uint32_t obj_phys = 0;
    if (!lod_pause_item_addr_phys(obj, 0x74, &obj_phys)) {
        return 0xFF;
    }

    int16_t active_level = lod_pause_item_s16(rdram, obj_phys + 0x0E);
    int32_t dispatch_level = active_level + 1;
    if (dispatch_level < 0) {
        dispatch_level = 0;
    }
    uint32_t state_byte_phys = obj_phys + 0x09 + (uint32_t)dispatch_level * 2;
    if (!lod_ni_telemetry_range_ok(state_byte_phys, 1)) {
        return 0xFF;
    }
    return lod_pause_item_u8(rdram, state_byte_phys);
}

struct LodPauseItemTraceSlot {
    uint32_t obj = 0;
    int pair = -999;
    uint32_t sig = 0;
};

static bool lod_pause_item_seen_signature(int pair, uint32_t obj, uint32_t sig) {
    static LodPauseItemTraceSlot slots[32] = {};
    int free_slot = -1;
    for (int i = 0; i < 32; i++) {
        if (slots[i].obj == obj && slots[i].pair == pair) {
            if (slots[i].sig == sig) {
                return true;
            }
            slots[i].sig = sig;
            return false;
        }
        if (slots[i].obj == 0 && free_slot < 0) {
            free_slot = i;
        }
    }
    if (free_slot < 0) {
        free_slot = (obj >> 4) & 31;
    }
    slots[free_slot].obj = obj;
    slots[free_slot].pair = pair;
    slots[free_slot].sig = sig;
    return false;
}

static void lod_pause_item_dump_save_items(uint8_t* rdram, const char* site, int count) {
    // Pair 99 builds its item list from sys.SaveStruct_gameplay item bytes at
    // SaveStruct base 0x801C82C0 + 0x2883 + item_id.
    static constexpr uint32_t kSaveStructPhys = 0x001C82C0;
    static constexpr uint32_t kItemAmountOff = 0x2883;

    char nonzero[512];
    size_t pos = 0;
    nonzero[0] = '\0';
    int nonzero_count = 0;
    for (int item_id = 0; item_id <= 0x2E; item_id++) {
        uint32_t phys = kSaveStructPhys + kItemAmountOff + (uint32_t)item_id;
        uint8_t amount = lod_pause_item_u8(rdram, phys);
        if (amount != 0) {
            int wrote = snprintf(nonzero + pos, sizeof(nonzero) - pos,
                                 "%s%02X:%u", nonzero_count == 0 ? "" : ",",
                                 item_id, amount);
            if (wrote > 0) {
                pos += (size_t)wrote;
                if (pos >= sizeof(nonzero)) {
                    pos = sizeof(nonzero) - 1;
                    nonzero[pos] = '\0';
                    break;
                }
            }
            nonzero_count++;
        }
    }
    if (nonzero_count == 0) {
        snprintf(nonzero, sizeof(nonzero), "none");
    }

    fprintf(stderr,
            "[ITEM_MENU_SAVE] %s #%d nonzero=%d items={%s} "
            "raw04-2D=[",
            site, count, nonzero_count, nonzero);
    for (int item_id = 4; item_id <= 0x2D; item_id++) {
        uint32_t phys = kSaveStructPhys + kItemAmountOff + (uint32_t)item_id;
        fprintf(stderr, "%s%u", item_id == 4 ? "" : ",",
                lod_pause_item_u8(rdram, phys));
    }
    fprintf(stderr, "]\n");
}

static void lod_pause_item_dump_child(uint8_t* rdram, const char* site, int count,
                                      const char* label, uint32_t obj) {
    uint32_t phys = 0;
    if (!lod_pause_item_addr_phys(obj, 0x74, &phys)) {
        fprintf(stderr,
                "[ITEM_MENU_CHILD] %s #%d %s=0x%08X invalid\n",
                site, count, label, obj);
        return;
    }

    int16_t level = lod_pause_item_s16(rdram, phys + 0x0E);
    uint8_t state0 = lod_pause_item_u8(rdram, phys + 0x09);
    uint8_t state1 = lod_pause_item_u8(rdram, phys + 0x0B);
    uint8_t state2 = lod_pause_item_u8(rdram, phys + 0x0D);
    uint16_t type10 = lod_pause_item_u16(rdram, phys + 0x10);
    uint16_t flags12 = lod_pause_item_u16(rdram, phys + 0x12);
    uint32_t f20 = lod_pause_item_u32(rdram, phys + 0x20);
    uint32_t f24 = lod_pause_item_u32(rdram, phys + 0x24);
    uint32_t f28 = lod_pause_item_u32(rdram, phys + 0x28);
    uint32_t f2c = lod_pause_item_u32(rdram, phys + 0x2C);
    uint32_t f30 = lod_pause_item_u32(rdram, phys + 0x30);
    uint32_t f34 = lod_pause_item_u32(rdram, phys + 0x34);
    uint32_t f38 = lod_pause_item_u32(rdram, phys + 0x38);
    uint32_t f3c = lod_pause_item_u32(rdram, phys + 0x3C);
    uint32_t f40 = lod_pause_item_u32(rdram, phys + 0x40);
    uint32_t f44 = lod_pause_item_u32(rdram, phys + 0x44);
    uint32_t f48 = lod_pause_item_u32(rdram, phys + 0x48);
    uint32_t f4c = lod_pause_item_u32(rdram, phys + 0x4C);
    uint32_t f50 = lod_pause_item_u32(rdram, phys + 0x50);
    uint32_t f54 = lod_pause_item_u32(rdram, phys + 0x54);
    uint32_t f58 = lod_pause_item_u32(rdram, phys + 0x58);
    uint32_t f5c = lod_pause_item_u32(rdram, phys + 0x5C);
    uint32_t f60 = lod_pause_item_u32(rdram, phys + 0x60);
    uint32_t f64 = lod_pause_item_u32(rdram, phys + 0x64);
    uint32_t f68 = lod_pause_item_u32(rdram, phys + 0x68);
    uint32_t f6c = lod_pause_item_u32(rdram, phys + 0x6C);
    uint32_t f70 = lod_pause_item_u32(rdram, phys + 0x70);

    uint32_t state_phys = 0;
    bool state_ok = lod_pause_item_addr_phys(f70, 0x60, &state_phys);
    uint32_t st_flags = state_ok ? lod_pause_item_u32(rdram, state_phys + 0x00) : 0;
    uint32_t st_cam = state_ok ? lod_pause_item_u32(rdram, state_phys + 0x04) : 0;
    uint32_t st_text = state_ok ? lod_pause_item_u32(rdram, state_phys + 0x08) : 0;
    uint32_t st_amount = state_ok ? lod_pause_item_u32(rdram, state_phys + 0x0C) : 0;
    uint32_t st_work = state_ok ? lod_pause_item_u32(rdram, state_phys + 0x10) : 0;
    uint16_t st_x = state_ok ? lod_pause_item_u16(rdram, state_phys + 0x14) : 0;
    uint16_t st_y = state_ok ? lod_pause_item_u16(rdram, state_phys + 0x16) : 0;
    float st_z = state_ok ? lod_pause_item_f32(rdram, state_phys + 0x18) : 0.0f;
    float st_sx = state_ok ? lod_pause_item_f32(rdram, state_phys + 0x1C) : 0.0f;
    float st_sy = state_ok ? lod_pause_item_f32(rdram, state_phys + 0x20) : 0.0f;
    uint32_t st_number = state_ok ? lod_pause_item_u32(rdram, state_phys + 0x24) : 0;
    uint16_t st_width = state_ok ? lod_pause_item_u16(rdram, state_phys + 0x2C) : 0;
    uint8_t st_opt = state_ok ? lod_pause_item_u8(rdram, state_phys + 0x2F) : 0;
    uint8_t st_line = state_ok ? lod_pause_item_u8(rdram, state_phys + 0x30) : 0;
    uint8_t st_pal = state_ok ? lod_pause_item_u8(rdram, state_phys + 0x34) : 0;
    uint8_t st_misc0 = state_ok ? lod_pause_item_u8(rdram, state_phys + 0x35) : 0;
    uint8_t st_misc1 = state_ok ? lod_pause_item_u8(rdram, state_phys + 0x36) : 0;
    uint32_t st_window_flags = state_ok ? lod_pause_item_u32(rdram, state_phys + 0x54) : 0;

    fprintf(stderr,
            "[ITEM_MENU_CHILD] %s #%d %s=0x%08X lvl=%d states=%u/%u/%u "
            "type=0x%04X flags=0x%04X "
            "obj={20=0x%08X 24=0x%08X 28=0x%08X 2C=0x%08X 30=0x%08X 34=0x%08X 38=0x%08X 3C=0x%08X "
            "40=0x%08X 44=0x%08X 48=0x%08X 4C=0x%08X 50=0x%08X 54=0x%08X 58=0x%08X 5C=0x%08X "
            "60=0x%08X 64=0x%08X 68=0x%08X 6C=0x%08X 70=0x%08X} "
            "mfds_state={ok=%d flags=0x%08X cam=0x%08X text=0x%08X amount=0x%08X work=0x%08X "
            "pos=(%u,%u,%+.1f) scale=(%.2f,%.2f) number=%u width=%u opt=%u line=%u pal=%u misc=%u/%u win=0x%08X}\n",
            site, count, label, obj, level, state0, state1, state2,
            type10, flags12,
            f20, f24, f28, f2c, f30, f34, f38, f3c,
            f40, f44, f48, f4c, f50, f54, f58, f5c,
            f60, f64, f68, f6c, f70,
            state_ok ? 1 : 0, st_flags, st_cam, st_text, st_amount, st_work,
            st_x, st_y, st_z, st_sx, st_sy, st_number, st_width, st_opt,
            st_line, st_pal, st_misc0, st_misc1, st_window_flags);
}

static void lod_pause_item_format_text_head(uint8_t* rdram, uint32_t text_addr,
                                            char* out, size_t out_size) {
    if (out_size == 0) {
        return;
    }
    out[0] = '\0';

    uint32_t phys = 0;
    if (!lod_pause_item_addr_phys(text_addr, 2, &phys)) {
        snprintf(out, out_size, "invalid");
        return;
    }

    size_t pos = 0;
    for (int i = 0; i < 16; i++) {
        if (!lod_ni_telemetry_range_ok(phys + (uint32_t)i * 2, 2)) {
            break;
        }
        uint16_t code = lod_pause_item_u16(rdram, phys + (uint32_t)i * 2);
        int wrote = snprintf(out + pos, out_size - pos,
                             "%s%04X", i == 0 ? "" : " ", code);
        if (wrote <= 0) {
            break;
        }
        pos += (size_t)wrote;
        if (pos >= out_size) {
            out[out_size - 1] = '\0';
            break;
        }
        if (code == 0xB500 || code == 0xFFFF || code == 0x0000) {
            break;
        }
    }
}

static void lod_pause_item_dump_mfds_state(uint8_t* rdram, const char* site, int count,
                                           const char* label, uint32_t state_addr) {
    uint32_t phys = 0;
    if (!lod_pause_item_addr_phys(state_addr, 0x60, &phys)) {
        fprintf(stderr,
                "[ITEM_MENU_MFDS] %s #%d %s=0x%08X invalid\n",
                site, count, label, state_addr);
        return;
    }

    uint32_t flags = lod_pause_item_u32(rdram, phys + 0x00);
    uint32_t cam = lod_pause_item_u32(rdram, phys + 0x04);
    uint32_t text = lod_pause_item_u32(rdram, phys + 0x08);
    uint32_t amount_text = lod_pause_item_u32(rdram, phys + 0x0C);
    uint32_t work = lod_pause_item_u32(rdram, phys + 0x10);
    uint16_t pos_x = lod_pause_item_u16(rdram, phys + 0x14);
    uint16_t pos_y = lod_pause_item_u16(rdram, phys + 0x16);
    float pos_z = lod_pause_item_f32(rdram, phys + 0x18);
    float scale_x = lod_pause_item_f32(rdram, phys + 0x1C);
    float scale_y = lod_pause_item_f32(rdram, phys + 0x20);
    int32_t number = (int32_t)lod_pause_item_u32(rdram, phys + 0x24);
    int32_t prev_number = (int32_t)lod_pause_item_u32(rdram, phys + 0x28);
    uint16_t width = lod_pause_item_u16(rdram, phys + 0x2C);
    uint8_t prev_opt = lod_pause_item_u8(rdram, phys + 0x2E);
    uint8_t opt = lod_pause_item_u8(rdram, phys + 0x2F);
    uint8_t line = lod_pause_item_u8(rdram, phys + 0x30);
    uint8_t spacing = lod_pause_item_u8(rdram, phys + 0x32);
    uint8_t left_margin = lod_pause_item_u8(rdram, phys + 0x33);
    uint8_t palette = lod_pause_item_u8(rdram, phys + 0x34);
    uint32_t window_flags = lod_pause_item_u32(rdram, phys + 0x54);
    float close_speed = lod_pause_item_f32(rdram, phys + 0x58);
    uint32_t lens = lod_pause_item_u32(rdram, phys + 0x5C);

    uint32_t work_phys = 0;
    bool work_ok = lod_pause_item_addr_phys(work, 0x60, &work_phys);
    uint32_t work_flags = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x00) : 0;
    uint16_t work_w04 = work_ok ? lod_pause_item_u16(rdram, work_phys + 0x04) : 0;
    uint16_t work_w06 = work_ok ? lod_pause_item_u16(rdram, work_phys + 0x06) : 0;
    uint32_t work_08 = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x08) : 0;
    uint32_t work_0c = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x0C) : 0;
    uint32_t work_10 = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x10) : 0;
    uint8_t work_text_speed = work_ok ? lod_pause_item_u8(rdram, work_phys + 0x3D) : 0;
    uint8_t work_tex_idx = work_ok ? lod_pause_item_u8(rdram, work_phys + 0x3C) : 0;
    uint32_t work_texbuf = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x48) : 0;
    uint32_t work_ltexbuf = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x4C) : 0;

    char text_head[128];
    char amount_head[128];
    lod_pause_item_format_text_head(rdram, text, text_head, sizeof(text_head));
    lod_pause_item_format_text_head(rdram, amount_text, amount_head, sizeof(amount_head));

    fprintf(stderr,
            "[ITEM_MENU_MFDS] %s #%d %s=0x%08X "
            "flags=0x%08X cam=0x%08X text=0x%08X amount=0x%08X work=0x%08X "
            "pos=(%u,%u,%+.1f) scale=(%.2f,%.2f) num=%d prev=%d "
            "dims={line=%u width=%u spacing=%u margin=%u opt=%u/%u pal=%u} "
            "win=0x%08X close=%.2f lens=0x%08X "
            "text_head=[%s] amount_head=[%s] "
            "work={ok=%d flags=0x%08X wh=%u/%u ptrs=0x%08X/0x%08X/0x%08X "
            "tex=%u speed=%u texbuf=0x%08X ltex=0x%08X}\n",
            site, count, label, state_addr,
            flags, cam, text, amount_text, work,
            pos_x, pos_y, pos_z, scale_x, scale_y, number, prev_number,
            line, width, spacing, left_margin, prev_opt, opt, palette,
            window_flags, close_speed, lens,
            text_head, amount_head,
            work_ok ? 1 : 0, work_flags, work_w04, work_w06, work_08, work_0c, work_10,
            work_tex_idx, work_text_speed, work_texbuf, work_ltexbuf);
}

static void lod_pause_item_dump_textbox_handle(uint8_t* rdram, const char* site, int count,
                                               const char* label, uint32_t handle_addr) {
    uint32_t phys = 0;
    if (!lod_pause_item_addr_phys(handle_addr, 0x74, &phys)) {
        fprintf(stderr,
                "[ITEM_MENU_TBOX] %s #%d %s=0x%08X invalid\n",
                site, count, label, handle_addr);
        return;
    }

    uint32_t core_addr = lod_pause_item_u32(rdram, phys + 0x38);
    uint32_t core_phys = 0;
    bool core_ok = lod_pause_item_addr_phys(core_addr, 0xA4, &core_phys);

    uint32_t h20 = lod_pause_item_u32(rdram, phys + 0x20);
    uint32_t h24 = lod_pause_item_u32(rdram, phys + 0x24);
    uint32_t h34 = lod_pause_item_u32(rdram, phys + 0x34);
    uint32_t h3c = lod_pause_item_u32(rdram, phys + 0x3C);
    uint32_t h40 = lod_pause_item_u32(rdram, phys + 0x40);
    uint32_t h4c = lod_pause_item_u32(rdram, phys + 0x4C);
    uint32_t h50 = lod_pause_item_u32(rdram, phys + 0x50);
    uint32_t h54 = lod_pause_item_u32(rdram, phys + 0x54);
    uint32_t h60 = lod_pause_item_u32(rdram, phys + 0x60);
    uint32_t h6c = lod_pause_item_u32(rdram, phys + 0x6C);

    uint32_t c20 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x20) : 0;
    uint8_t c24 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x24) : 0;
    uint8_t c25 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x25) : 0;
    uint16_t c30 = core_ok ? lod_pause_item_u16(rdram, core_phys + 0x30) : 0;
    uint32_t c34 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x34) : 0;
    uint32_t c38 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x38) : 0;
    uint8_t c3c = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x3C) : 0;
    uint8_t c3d = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x3D) : 0;
    uint32_t c54 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x54) : 0;
    uint32_t c58 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x58) : 0;
    uint32_t c5c = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x5C) : 0;
    uint8_t c60 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x60) : 0;
    uint8_t c61 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x61) : 0;
    uint8_t c62 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x62) : 0;
    uint32_t c98 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x98) : 0;
    uint32_t c9c = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x9C) : 0;
    uint32_t ca0 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0xA0) : 0;

    char head34[128], head54[128], head58[128], head5c[128];
    lod_pause_item_format_text_head(rdram, c34, head34, sizeof(head34));
    lod_pause_item_format_text_head(rdram, c54, head54, sizeof(head54));
    lod_pause_item_format_text_head(rdram, c58, head58, sizeof(head58));
    lod_pause_item_format_text_head(rdram, c5c, head5c, sizeof(head5c));

    fprintf(stderr,
            "[ITEM_MENU_TBOX] %s #%d %s=0x%08X "
            "handle={20=0x%08X 24=0x%08X 34=0x%08X 38(core)=0x%08X 3C=0x%08X 40=0x%08X "
            "4C=0x%08X 50=0x%08X 54=0x%08X 60=0x%08X 6C=0x%08X} "
            "core={ok=%d 20flags=0x%08X pal24=%u line25=%u w30=0x%04X "
            "text34=0x%08X amount38=0x%08X idx3C=%u speed3D=%u "
            "lineptrs=0x%08X/0x%08X/0x%08X states=%u/%u/%u "
            "scale98/9C/A0=0x%08X/0x%08X/0x%08X} "
            "heads={34=[%s] 54=[%s] 58=[%s] 5C=[%s]}\n",
            site, count, label, handle_addr,
            h20, h24, h34, core_addr, h3c, h40, h4c, h50, h54, h60, h6c,
            core_ok ? 1 : 0, c20, c24, c25, c30, c34, c38, c3c, c3d,
            c54, c58, c5c, c60, c61, c62, c98, c9c, ca0,
            head34, head54, head58, head5c);
}

static recomp_func_t* lod_orig_pause_item_func_80083008 = nullptr;
static recomp_func_t* lod_orig_pause_item_func_800835D0 = nullptr;
static recomp_func_t* lod_orig_pause_item_func_8008485C = nullptr;
static recomp_func_t* lod_orig_pause_item_func_80083FA0 = nullptr;

static uint32_t lod_pause_item_stack_u32(uint8_t* rdram, uint32_t sp, uint32_t off) {
    uint32_t phys = 0;
    if (!lod_pause_item_addr_phys(ADD32(sp, off), 4, &phys)) {
        return 0;
    }
    return lod_pause_item_u32(rdram, phys);
}

static bool lod_pause_item_text_trace_active(uint8_t* rdram) {
    int32_t gs = lod_ni_telemetry_gamestate(rdram);
    return gs == 3 && (loaded_0f_pair == 99 || loaded_0f_pair == 101);
}

static bool lod_pause_item_text_trace_log_this(uint8_t* rdram, int* counter) {
    if (!lod_pause_item_text_trace_active(rdram)) {
        return false;
    }
    (*counter)++;
    return *counter <= 160 || (*counter % 200) == 0;
}

static void lod_pause_item_log_text_core(uint8_t* rdram, const char* site, int count,
                                         uint32_t handle_addr) {
    uint32_t handle_phys = 0;
    if (!lod_pause_item_addr_phys(handle_addr, 0x74, &handle_phys)) {
        fprintf(stderr,
                "[ITEM_TEXT] %s #%d handle=0x%08X invalid\n",
                site, count, handle_addr);
        return;
    }

    uint32_t core_addr = lod_pause_item_u32(rdram, handle_phys + 0x38);
    uint32_t core_phys = 0;
    bool core_ok = lod_pause_item_addr_phys(core_addr, 0xA4, &core_phys);
    uint32_t flags = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x20) : 0;
    uint8_t pal = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x24) : 0;
    uint8_t line = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x25) : 0;
    uint32_t text = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x34) : 0;
    uint32_t amount = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x38) : 0;
    uint8_t idx = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x3C) : 0;
    uint8_t speed = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x3D) : 0;
    uint32_t l0 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x54) : 0;
    uint32_t l1 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x58) : 0;
    uint32_t l2 = core_ok ? lod_pause_item_u32(rdram, core_phys + 0x5C) : 0;
    uint8_t s0 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x60) : 0;
    uint8_t s1 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x61) : 0;
    uint8_t s2 = core_ok ? lod_pause_item_u8(rdram, core_phys + 0x62) : 0;

    char text_head[128], amount_head[128], l0_head[128], l1_head[128], l2_head[128];
    lod_pause_item_format_text_head(rdram, text, text_head, sizeof(text_head));
    lod_pause_item_format_text_head(rdram, amount, amount_head, sizeof(amount_head));
    lod_pause_item_format_text_head(rdram, l0, l0_head, sizeof(l0_head));
    lod_pause_item_format_text_head(rdram, l1, l1_head, sizeof(l1_head));
    lod_pause_item_format_text_head(rdram, l2, l2_head, sizeof(l2_head));

    fprintf(stderr,
            "[ITEM_TEXT] %s #%d pair=%d handle=0x%08X core=0x%08X ok=%d "
            "flags=0x%08X pal=%u line=%u text=0x%08X amount=0x%08X idx=%u speed=%u "
            "lineptrs=0x%08X/0x%08X/0x%08X states=%u/%u/%u "
            "heads={text=[%s] amount=[%s] l0=[%s] l1=[%s] l2=[%s]}\n",
            site, count, loaded_0f_pair, handle_addr, core_addr, core_ok ? 1 : 0,
            flags, pal, line, text, amount, idx, speed, l0, l1, l2, s0, s1, s2,
            text_head, amount_head, l0_head, l1_head, l2_head);
}

static void lod_trace_pause_item_func_80083008(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    bool log_this = lod_pause_item_text_trace_log_this(rdram, &count);
    uint32_t handle = (uint32_t)ctx->r4;
    uint32_t text = (uint32_t)ctx->r5;
    uint32_t flags_arg = (uint32_t)ctx->r6;
    uint32_t a3 = (uint32_t)ctx->r7;
    uint32_t sp = (uint32_t)ctx->r29;
    uint32_t stk10 = lod_pause_item_stack_u32(rdram, sp, 0x10);
    uint32_t stk14 = lod_pause_item_stack_u32(rdram, sp, 0x14);
    uint32_t stk18 = lod_pause_item_stack_u32(rdram, sp, 0x18);
    if (log_this) {
        char head[128];
        lod_pause_item_format_text_head(rdram, text, head, sizeof(head));
        fprintf(stderr,
                "[ITEM_TEXT] 80083008-pre #%d pair=%d a={handle=0x%08X text=0x%08X flags=0x%08X a3=0x%08X} "
                "stk={10=0x%08X 14=0x%08X 18=0x%08X} text_head=[%s]\n",
                count, loaded_0f_pair, handle, text, flags_arg, a3, stk10, stk14, stk18, head);
    }
    if (lod_orig_pause_item_func_80083008 != nullptr) {
        lod_orig_pause_item_func_80083008(rdram, ctx);
    }
    if (log_this) {
        lod_pause_item_log_text_core(rdram, "80083008-post", count, handle);
    }
}

static void lod_trace_pause_item_func_800835D0(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    bool log_this = lod_pause_item_text_trace_log_this(rdram, &count);
    uint32_t handle = (uint32_t)ctx->r4;
    uint32_t a1 = (uint32_t)ctx->r5;
    uint32_t a2 = (uint32_t)ctx->r6;
    uint32_t a3 = (uint32_t)ctx->r7;
    uint32_t sp = (uint32_t)ctx->r29;
    uint32_t stk10 = lod_pause_item_stack_u32(rdram, sp, 0x10);
    uint32_t stk14 = lod_pause_item_stack_u32(rdram, sp, 0x14);
    uint32_t stk18 = lod_pause_item_stack_u32(rdram, sp, 0x18);
    if (log_this) {
        fprintf(stderr,
                "[ITEM_TEXT] 800835D0-pre #%d pair=%d a={handle=0x%08X a1=0x%08X a2=0x%08X a3=0x%08X} "
                "stk={10=0x%08X 14=0x%08X 18=0x%08X}\n",
                count, loaded_0f_pair, handle, a1, a2, a3, stk10, stk14, stk18);
    }
    if (lod_orig_pause_item_func_800835D0 != nullptr) {
        lod_orig_pause_item_func_800835D0(rdram, ctx);
    }
    if (log_this) {
        lod_pause_item_log_text_core(rdram, "800835D0-post", count, handle);
    }
}

static void lod_trace_pause_item_func_8008485C(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    bool log_this = lod_pause_item_text_trace_log_this(rdram, &count);
    uint32_t handle = (uint32_t)ctx->r4;
    uint32_t line_text = (uint32_t)ctx->r5;
    uint32_t line_index = (uint32_t)ctx->r6;
    uint32_t line_state = (uint32_t)ctx->r7;
    if (log_this) {
        char head[128];
        lod_pause_item_format_text_head(rdram, line_text, head, sizeof(head));
        fprintf(stderr,
                "[ITEM_TEXT] 8008485C-pre #%d pair=%d a={handle=0x%08X line=0x%08X idx=%u state=%u} "
                "line_head=[%s]\n",
                count, loaded_0f_pair, handle, line_text, line_index, line_state, head);
    }
    if (lod_orig_pause_item_func_8008485C != nullptr) {
        lod_orig_pause_item_func_8008485C(rdram, ctx);
    }
    if (log_this) {
        lod_pause_item_log_text_core(rdram, "8008485C-post", count, handle);
    }
}

static void lod_trace_pause_item_func_80083FA0(uint8_t* rdram, recomp_context* ctx) {
    static int count = 0;
    bool log_this = lod_pause_item_text_trace_log_this(rdram, &count);
    uint32_t pool = (uint32_t)ctx->r4;
    uint32_t index = (uint32_t)ctx->r5;
    if (log_this) {
        char head[128];
        lod_pause_item_format_text_head(rdram, pool, head, sizeof(head));
        fprintf(stderr,
                "[ITEM_TEXT] 80083FA0-pre #%d pair=%d pool=0x%08X index=%u pool_head=[%s]\n",
                count, loaded_0f_pair, pool, index, head);
    }
    if (lod_orig_pause_item_func_80083FA0 != nullptr) {
        lod_orig_pause_item_func_80083FA0(rdram, ctx);
    }
    if (log_this) {
        uint32_t result = (uint32_t)ctx->r2;
        char head[128];
        lod_pause_item_format_text_head(rdram, result, head, sizeof(head));
        fprintf(stderr,
                "[ITEM_TEXT] 80083FA0-post #%d pair=%d result=0x%08X result_head=[%s]\n",
                count, loaded_0f_pair, result, head);
    }
}

static void lod_install_pause_item_text_trace_wrapper(uint32_t vram,
                                                      recomp_func_t* wrapper,
                                                      recomp_func_t** original,
                                                      const char* name) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current != wrapper) {
        *original = current;
        recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
        fprintf(stderr,
                "[ITEM_TEXT] installed %s wrapper vram=0x%08X original=%p wrapper=%p\n",
                name, vram, (void*)current, (void*)wrapper);
    }
}

static void lod_install_pause_item_text_trace_wrappers(const char* reason) {
    (void)reason;
    lod_install_pause_item_text_trace_wrapper(0x80083008,
        lod_trace_pause_item_func_80083008, &lod_orig_pause_item_func_80083008, "80083008");
    lod_install_pause_item_text_trace_wrapper(0x800835D0,
        lod_trace_pause_item_func_800835D0, &lod_orig_pause_item_func_800835D0, "800835D0");
    lod_install_pause_item_text_trace_wrapper(0x8008485C,
        lod_trace_pause_item_func_8008485C, &lod_orig_pause_item_func_8008485C, "8008485C");
    lod_install_pause_item_text_trace_wrapper(0x80083FA0,
        lod_trace_pause_item_func_80083FA0, &lod_orig_pause_item_func_80083FA0, "80083FA0");
}

static void lod_pause_item_trace_log_generic(uint8_t* rdram, recomp_context* ctx,
                                             const char* site) {
    static uint16_t last_sys_held = 0;
    static uint16_t last_sys_pressed = 0;
    static int pair_counts[NI_OVL_COUNT] = {};
    static int generic_log_count = 0;

    int pair = loaded_0f_pair;
    if (pair < 0 || pair >= NI_OVL_COUNT) {
        pair = -1;
    }
    if (pair >= 0) {
        pair_counts[pair]++;
    }

    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = 0;
    bool obj_ok = lod_pause_item_addr_phys(obj, 0x74, &obj_phys);
    int16_t active_level_before = obj_ok ? lod_pause_item_s16(rdram, obj_phys + 0x0E) : -99;
    int32_t dispatch_level = active_level_before + 1;
    if (dispatch_level < 0) {
        dispatch_level = 0;
    }

    uint32_t state_byte_phys = obj_phys + 0x09 + (uint32_t)dispatch_level * 2;
    uint8_t state_index = (obj_ok && lod_ni_telemetry_range_ok(state_byte_phys, 1)) ?
        lod_pause_item_u8(rdram, state_byte_phys) : 0xFF;
    uint8_t state_timer = (obj_ok && state_byte_phys > 0) ?
        lod_pause_item_u8(rdram, state_byte_phys - 1) : 0;

    uint32_t model24 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x24) : 0;
    uint32_t ctx34 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x34) : 0;
    uint32_t child38 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x38) : 0;
    uint32_t model54 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x54) : 0;

    constexpr uint32_t SYS_INPUT_PHYS = 0x001C87F4;
    constexpr uint32_t PLAYER_INPUT_PHYS = 0x000F35F0;
    uint16_t sys_conn = lod_pause_item_u16(rdram, SYS_INPUT_PHYS + 0x00);
    uint16_t sys_held = lod_pause_item_u16(rdram, SYS_INPUT_PHYS + 0x02);
    uint16_t sys_pressed = lod_pause_item_u16(rdram, SYS_INPUT_PHYS + 0x04);
    uint16_t player_conn = lod_pause_item_u16(rdram, PLAYER_INPUT_PHYS + 0x00);
    uint16_t player_held = lod_pause_item_u16(rdram, PLAYER_INPUT_PHYS + 0x02);
    uint16_t player_pressed = lod_pause_item_u16(rdram, PLAYER_INPUT_PHYS + 0x04);

    bool input_event = sys_pressed != 0 || sys_held != last_sys_held || sys_pressed != last_sys_pressed;

    if (lod_pause_item_is_focused_pair(pair)) {
        uint32_t top_target = state_index != 0xFF ? lod_pause_item_pair_target(rdram, pair, state_index) : 0;

        uint32_t ctx34_phys = 0;
        bool ctx34_ok = lod_pause_item_addr_phys(ctx34, 0x170, &ctx34_phys);
        uint32_t work_phys = ctx34_phys + 0xDC;
        uint16_t c128 = ctx34_ok ? lod_pause_item_u16(rdram, ctx34_phys + 0x128) : 0;
        uint16_t c144 = ctx34_ok ? lod_pause_item_u16(rdram, ctx34_phys + 0x144) : 0;
        uint8_t work3a = ctx34_ok ? lod_pause_item_u8(rdram, work_phys + 0x3A) : 0;
        uint8_t work3b = ctx34_ok ? lod_pause_item_u8(rdram, work_phys + 0x3B) : 0;
        uint8_t work3c = ctx34_ok ? lod_pause_item_u8(rdram, work_phys + 0x3C) : 0;
        uint16_t work4c = ctx34_ok ? lod_pause_item_u16(rdram, work_phys + 0x4C) : 0;
        uint16_t work58 = ctx34_ok ? lod_pause_item_u16(rdram, work_phys + 0x58) : 0;
        uint16_t work68 = ctx34_ok ? lod_pause_item_u16(rdram, work_phys + 0x68) : 0;
        uint32_t work3c_ptr = ctx34_ok ? lod_pause_item_u32(rdram, work_phys + 0x3C) : 0;
        uint32_t work64_ptr = ctx34_ok ? lod_pause_item_u32(rdram, work_phys + 0x64) : 0;
        uint32_t work7c_ptr = ctx34_ok ? lod_pause_item_u32(rdram, work_phys + 0x7C) : 0;
        uint32_t work84_ptr = ctx34_ok ? lod_pause_item_u32(rdram, work_phys + 0x84) : 0;
        float work30 = ctx34_ok ? lod_pause_item_f32(rdram, work_phys + 0x30) : 0.0f;
        // The sub-dispatch tables at 0x5BDC/0x5BF4 only exist in pair 45.
        // Pair 99/101 are the pause/menu shell and children; reading those
        // offsets there can fault outside the loaded 0x0F segment.
        uint32_t sub_a_target = (pair == 45) ? lod_pause_item_sub_a_target(rdram, work3a) : 0;
        uint32_t sub_b_target = (pair == 45) ? lod_pause_item_sub_b_target(rdram, work3b) : 0;

        uint32_t sig = 2166136261u;
        sig = lod_pause_item_hash_step(sig, (uint32_t)pair);
        sig = lod_pause_item_hash_step(sig, obj);
        sig = lod_pause_item_hash_step(sig, (uint32_t)(uint16_t)active_level_before);
        sig = lod_pause_item_hash_step(sig, (uint32_t)dispatch_level);
        sig = lod_pause_item_hash_step(sig, state_index);
        sig = lod_pause_item_hash_step(sig, top_target);
        sig = lod_pause_item_hash_step(sig, model24);
        sig = lod_pause_item_hash_step(sig, ctx34);
        sig = lod_pause_item_hash_step(sig, child38);
        sig = lod_pause_item_hash_step(sig, model54);
        sig = lod_pause_item_hash_step(sig, work3a | ((uint32_t)work3b << 8) |
                                          ((uint32_t)work3c << 16) | ((uint32_t)work4c << 24));
        sig = lod_pause_item_hash_step(sig, work58);
        sig = lod_pause_item_hash_step(sig, work3c_ptr);
        sig = lod_pause_item_hash_step(sig, work64_ptr);
        sig = lod_pause_item_hash_step(sig, work7c_ptr);
        sig = lod_pause_item_hash_step(sig, work84_ptr);
        sig = lod_pause_item_hash_step(sig, sys_held | ((uint32_t)sys_pressed << 16));

        bool already_seen = lod_pause_item_seen_signature(pair, obj, sig);
        bool initial = pair_counts[pair] <= 18;
        bool interesting_state = (pair == 45) &&
            (state_index >= 4 || work3a != 0 || work3b != 0 || work3c != 0);
        bool pair99_item_state = (pair == 99) && (state_index >= 3 && state_index <= 7);
        bool should_log = initial || input_event || !already_seen || interesting_state;
        if (should_log) {
            fprintf(stderr,
                "[ITEM_MENU_STATE] %s #%d gs=%d pair0f=%d obj=0x%08X ok=%d "
                "lvl=%d dispatch=%d timer=%u state=%u->0x%08X(%s) "
                "subA=%u->0x%08X(%s) subB=%u->0x%08X(%s) sig=0x%08X "
                "input={sys:%04X/%04X/%04X player:%04X/%04X/%04X} "
                "obj={model24=0x%08X ctx34=0x%08X child38=0x%08X model54=0x%08X} "
                "ctx={ok=%d flags128=0x%04X flags144=0x%04X w30=%+.2f w3C=%u "
                "w4C=0x%04X w58=0x%04X w68=0x%04X ptr3C=0x%08X ptr64=0x%08X ptr7C=0x%08X ptr84=0x%08X} "
                "regs={v0=0x%08X a0=0x%08X a1=0x%08X a2=0x%08X a3=0x%08X sp=0x%08X ra=0x%08X}\n",
                site, pair_counts[pair], lod_ni_telemetry_gamestate(rdram), pair,
                obj, obj_ok, active_level_before, dispatch_level, state_timer,
                state_index, top_target, lod_pause_item_state_name(top_target),
                work3a, sub_a_target, lod_pause_item_state_name(sub_a_target),
                work3b, sub_b_target, lod_pause_item_state_name(sub_b_target), sig,
                sys_conn, sys_held, sys_pressed, player_conn, player_held, player_pressed,
                model24, ctx34, child38, model54,
                ctx34_ok, c128, c144, work30, work3c, work4c, work58, work68,
                work3c_ptr, work64_ptr, work7c_ptr, work84_ptr,
                (uint32_t)ctx->r2, (uint32_t)ctx->r4, (uint32_t)ctx->r5,
                (uint32_t)ctx->r6, (uint32_t)ctx->r7,
                (uint32_t)ctx->r29, (uint32_t)ctx->r31);
        }

        if (pair99_item_state && should_log) {
            uint32_t obj28 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x28) : 0;
            uint32_t obj2c = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x2C) : 0;
            uint32_t obj30 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x30) : 0;
            uint32_t obj3c = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x3C) : 0;
            uint32_t obj40 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x40) : 0;
            uint32_t obj44 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x44) : 0;
            uint32_t obj48 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x48) : 0;
            uint32_t obj4c = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x4C) : 0;
            uint32_t obj50 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x50) : 0;
            uint32_t obj58 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x58) : 0;
            uint32_t obj5c = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x5C) : 0;
            uint32_t obj60 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x60) : 0;
            uint32_t obj64 = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x64) : 0;

            uint32_t item_work_phys = 0;
            bool item_work_ok = lod_pause_item_addr_phys(ctx34, 0x70, &item_work_phys);
            int16_t work1c = item_work_ok ? lod_pause_item_s16(rdram, item_work_phys + 0x1C) : 0;
            uint8_t work1d = item_work_ok ? lod_pause_item_u8(rdram, item_work_phys + 0x1D) : 0;
            int16_t work28_count = item_work_ok ? lod_pause_item_s16(rdram, item_work_phys + 0x28) : 0;
            uint32_t work00 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x00) : 0;
            uint32_t work04 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x04) : 0;
            uint32_t work08 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x08) : 0;
            uint32_t work0c = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x0C) : 0;
            uint32_t work10 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x10) : 0;
            uint32_t work14 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x14) : 0;
            uint32_t work18 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x18) : 0;
            uint32_t work20 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x20) : 0;
            uint32_t work24 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x24) : 0;
            uint32_t work2c = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x2C) : 0;
            uint32_t work30w = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x30) : 0;
            uint32_t work34 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x34) : 0;
            uint32_t work38 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x38) : 0;
            uint32_t work3c_w = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x3C) : 0;
            uint32_t work40 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x40) : 0;
            uint16_t work44 = item_work_ok ? lod_pause_item_u16(rdram, item_work_phys + 0x44) : 0;
            uint16_t work46 = item_work_ok ? lod_pause_item_u16(rdram, item_work_phys + 0x46) : 0;
            uint32_t work48 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x48) : 0;
            uint32_t work4c_w = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x4C) : 0;
            uint32_t work50 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x50) : 0;
            uint32_t work54 = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x54) : 0;
            uint32_t work58_w = item_work_ok ? lod_pause_item_u32(rdram, item_work_phys + 0x58) : 0;

            int16_t list_id[8] = {};
            int16_t list_amt[8] = {};
            for (int i = 0; i < 8; i++) {
                list_id[i] = MEM_H(0x4EA0 + i * 4, (gpr)(int32_t)0x0F000000);
                list_amt[i] = MEM_H(0x4EA2 + i * 4, (gpr)(int32_t)0x0F000000);
            }

            fprintf(stderr,
                "[ITEM_MENU_WORK] %s #%d gs=%d obj=0x%08X state=%u "
                "obj={24=0x%08X 28=0x%08X 2C=0x%08X 30=0x%08X 34=0x%08X 38=0x%08X 3C=0x%08X "
                "40=0x%08X 44=0x%08X 48=0x%08X 4C=0x%08X 50=0x%08X 54=0x%08X 58=0x%08X 5C=0x%08X 60=0x%08X 64=0x%08X} "
                "work={ok=%d 00=0x%08X 04=0x%08X 08=0x%08X 0C=0x%08X 10=0x%08X 14=0x%08X 18=0x%08X "
                "1C=%d 1D=%u 20=0x%08X 24=0x%08X 28_count=%d 2C=0x%08X 30=0x%08X 34=0x%08X 38=0x%08X "
                "3C=0x%08X 40=0x%08X 44=0x%04X 46=0x%04X 48=0x%08X 4C=0x%08X 50=0x%08X 54=0x%08X 58=0x%08X} "
                "list=[%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d]\n",
                site, pair_counts[pair], lod_ni_telemetry_gamestate(rdram), obj, state_index,
                model24, obj28, obj2c, obj30, ctx34, child38, obj3c,
                obj40, obj44, obj48, obj4c, obj50, model54, obj58, obj5c, obj60, obj64,
                item_work_ok, work00, work04, work08, work0c, work10, work14, work18,
                work1c, work1d, work20, work24, work28_count, work2c, work30w, work34, work38,
                work3c_w, work40, work44, work46, work48, work4c_w, work50, work54, work58_w,
                list_id[0], list_amt[0], list_id[1], list_amt[1],
                list_id[2], list_amt[2], list_id[3], list_amt[3],
                list_id[4], list_amt[4], list_id[5], list_amt[5],
                list_id[6], list_amt[6], list_id[7], list_amt[7]);

            lod_pause_item_dump_save_items(rdram, site, pair_counts[pair]);
            lod_pause_item_dump_child(rdram, site, pair_counts[pair], "work00_parent", work00);
            lod_pause_item_dump_child(rdram, site, pair_counts[pair], "work0C_scroll", work0c);
            lod_pause_item_dump_textbox_handle(rdram, site, pair_counts[pair], "work10_item_name0", work10);
            lod_pause_item_dump_textbox_handle(rdram, site, pair_counts[pair], "work14_item_name1", work14);
            lod_pause_item_dump_textbox_handle(rdram, site, pair_counts[pair], "work18_item_name2", work18);
            if (work20 != 0 && work20 != work10 && work20 != work14 && work20 != work18) {
                lod_pause_item_dump_textbox_handle(rdram, site, pair_counts[pair], "work20_active", work20);
            }
        }
    } else {
        uint32_t sig = 2166136261u;
        sig = lod_pause_item_hash_step(sig, (uint32_t)pair);
        sig = lod_pause_item_hash_step(sig, obj);
        sig = lod_pause_item_hash_step(sig, (uint32_t)(uint16_t)active_level_before);
        sig = lod_pause_item_hash_step(sig, state_index);
        sig = lod_pause_item_hash_step(sig, model24);
        sig = lod_pause_item_hash_step(sig, ctx34);
        sig = lod_pause_item_hash_step(sig, child38);
        sig = lod_pause_item_hash_step(sig, sys_held | ((uint32_t)sys_pressed << 16));
        (void)lod_pause_item_seen_signature(pair, obj, sig);
        bool initial = pair >= 0 && pair_counts[pair] <= 2;
        bool should_log = input_event || initial;
        if (should_log && generic_log_count < 160) {
            generic_log_count++;
            fprintf(stderr,
                "[NI0F_ENTRY] %s #%d gs=%d pair=%d obj=0x%08X ok=%d "
                "lvl=%d dispatch=%d timer=%u state=%u sig=0x%08X "
                "input={sys:%04X/%04X/%04X player:%04X/%04X/%04X} "
                "obj={model24=0x%08X ctx34=0x%08X child38=0x%08X model54=0x%08X} "
                "regs={a1=0x%08X a2=0x%08X a3=0x%08X}\n",
                site, pair >= 0 ? pair_counts[pair] : 0, lod_ni_telemetry_gamestate(rdram), pair,
                obj, obj_ok, active_level_before, dispatch_level, state_timer, state_index, sig,
                sys_conn, sys_held, sys_pressed, player_conn, player_held, player_pressed,
                model24, ctx34, child38, model54,
                (uint32_t)ctx->r5, (uint32_t)ctx->r6, (uint32_t)ctx->r7);
        }
    }

    last_sys_held = sys_held;
    last_sys_pressed = sys_pressed;
}

static void lod_pause_item_trace_log_pair99_item(uint8_t* rdram, recomp_context* ctx,
                                                 const char* site) {
    uint32_t obj = (uint32_t)ctx->r4;
    uint32_t obj_phys = 0;
    bool obj_ok = lod_pause_item_addr_phys(obj, 0x74, &obj_phys);
    uint8_t state_index = lod_pause_item_current_state_index(rdram, obj);
    uint32_t target = state_index != 0xFF
        ? lod_pause_item_pair99_item_target(rdram, state_index) : 0;

    uint16_t sys_held = lod_pause_item_u16(rdram, 0x001C87F4 + 2);
    uint16_t sys_pressed = lod_pause_item_u16(rdram, 0x001C87F4 + 4);
    bool input_event = sys_held != 0 || sys_pressed != 0;

    uint32_t work = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x34) : 0;
    uint32_t work_phys = 0;
    bool work_ok = lod_pause_item_addr_phys(work, 0x70, &work_phys);
    int16_t active_level = obj_ok ? lod_pause_item_s16(rdram, obj_phys + 0x0E) : 0;
    uint16_t timer = obj_ok ? lod_pause_item_u16(rdram, obj_phys + 0x04) : 0;
    uint32_t parent = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x14) : 0;
    uint32_t child = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x1C) : 0;
    uint32_t camera = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x38) : 0;
    uint32_t state_flags = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x3C) : 0;
    uint32_t scroll = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x44) : 0;
    int32_t top = obj_ok ? (int32_t)lod_pause_item_u32(rdram, obj_phys + 0x48) : 0;
    int32_t page = obj_ok ? (int32_t)lod_pause_item_u32(rdram, obj_phys + 0x4C) : 0;
    int32_t selected = obj_ok ? (int32_t)lod_pause_item_u32(rdram, obj_phys + 0x54) : 0;
    uint32_t empty = obj_ok ? lod_pause_item_u32(rdram, obj_phys + 0x58) : 0;

    uint32_t rows[6] = {};
    for (int i = 0; i < 6; i++) {
        rows[i] = work_ok ? lod_pause_item_u32(rdram, work_phys + (uint32_t)i * 4) : 0;
    }
    uint32_t cursor = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x18) : 0;
    uint32_t cursor_text = work_ok ? lod_pause_item_u32(rdram, work_phys + 0x1C) : 0;
    int16_t list_count = work_ok ? lod_pause_item_s16(rdram, work_phys + 0x28) : 0;

    int16_t list_id[8] = {};
    int16_t list_amt[8] = {};
    for (int i = 0; i < 8; i++) {
        list_id[i] = MEM_H(0x4EA0 + i * 4, (gpr)(int32_t)0x0F000000);
        list_amt[i] = MEM_H(0x4EA2 + i * 4, (gpr)(int32_t)0x0F000000);
    }

    uint32_t sig = 2166136261u;
    sig = lod_pause_item_hash_step(sig, obj);
    sig = lod_pause_item_hash_step(sig, state_index);
    sig = lod_pause_item_hash_step(sig, target);
    sig = lod_pause_item_hash_step(sig, work);
    sig = lod_pause_item_hash_step(sig, (uint32_t)(uint16_t)list_count);
    sig = lod_pause_item_hash_step(sig, state_flags);
    sig = lod_pause_item_hash_step(sig, (uint32_t)top);
    sig = lod_pause_item_hash_step(sig, (uint32_t)page);
    sig = lod_pause_item_hash_step(sig, (uint32_t)selected);
    sig = lod_pause_item_hash_step(sig, empty);
    sig = lod_pause_item_hash_step(sig, rows[0]);
    sig = lod_pause_item_hash_step(sig, rows[1]);
    sig = lod_pause_item_hash_step(sig, rows[2]);
    sig = lod_pause_item_hash_step(sig, rows[3]);
    sig = lod_pause_item_hash_step(sig, rows[4]);
    sig = lod_pause_item_hash_step(sig, rows[5]);
    sig = lod_pause_item_hash_step(sig, (uint32_t)list_id[0] | ((uint32_t)list_amt[0] << 16));
    sig = lod_pause_item_hash_step(sig, sys_held | ((uint32_t)sys_pressed << 16));

    static int log_count = 0;
    bool changed = !lod_pause_item_seen_signature(9900, obj, sig);
    bool should_log = input_event || changed || log_count < 24;
    if (!should_log || log_count >= 220) {
        return;
    }
    log_count++;

    fprintf(stderr,
            "[ITEM_LIST] %s #%d gs=%d obj=0x%08X ok=%d lvl=%d timer=%u "
            "state=%u->0x%08X(%s) sig=0x%08X input=%04X/%04X "
            "parent=0x%08X child=0x%08X work=0x%08X ok=%d "
            "obj={cam=0x%08X flags=0x%08X scroll=0x%08X top=%d page=%d selected=%d empty=%u} "
            "work={rows=[0x%08X,0x%08X,0x%08X,0x%08X,0x%08X,0x%08X] cursor=0x%08X text=0x%08X count=%d} "
            "list=[%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d,%d:%d] "
            "regs={a0=0x%08X a1=0x%08X a2=0x%08X a3=0x%08X sp=0x%08X ra=0x%08X}\n",
            site, log_count, lod_ni_telemetry_gamestate(rdram),
            obj, obj_ok ? 1 : 0, active_level, timer,
            state_index, target, lod_pause_item_state_name(target), sig,
            sys_held, sys_pressed,
            parent, child, work, work_ok ? 1 : 0,
            camera, state_flags, scroll, top, page, selected, empty,
            rows[0], rows[1], rows[2], rows[3], rows[4], rows[5],
            cursor, cursor_text, list_count,
            list_id[0], list_amt[0], list_id[1], list_amt[1],
            list_id[2], list_amt[2], list_id[3], list_amt[3],
            list_id[4], list_amt[4], list_id[5], list_amt[5],
            list_id[6], list_amt[6], list_id[7], list_amt[7],
            (uint32_t)ctx->r4, (uint32_t)ctx->r5, (uint32_t)ctx->r6,
            (uint32_t)ctx->r7, (uint32_t)ctx->r29, (uint32_t)ctx->r31);

    if ((site[0] == 's' || log_count <= 12) && work_ok) {
        for (int i = 0; i < 6; i++) {
            if (rows[i] != 0) {
                char label[32];
                snprintf(label, sizeof(label), "row%d", i);
                lod_pause_item_dump_textbox_handle(rdram, site, log_count, label, rows[i]);
            }
        }
        if (cursor != 0) {
            lod_pause_item_dump_child(rdram, site, log_count, "cursor", cursor);
        }
    }
}

static void lod_trace_pause_item_entry(uint8_t* rdram, recomp_context* ctx) {
    int pre_pair = loaded_0f_pair;
    uint32_t pre_obj = (uint32_t)ctx->r4;
    uint8_t pre_state = lod_pause_item_current_state_index(rdram, pre_obj);
    bool post_log = (pre_pair == 99 && pre_state >= 3 && pre_state <= 7) ||
                    (pre_pair == 101 && pre_state >= 1 && pre_state <= 4);

    lod_pause_item_trace_log_generic(rdram, ctx, "entry-pre");
    if (lod_orig_pause_item_entry != nullptr) {
        lod_orig_pause_item_entry(rdram, ctx);
    }
    if (post_log) {
        lod_pause_item_trace_log_generic(rdram, ctx, "entry-post");
    }
}

static void lod_trace_pause_item_sub_entry(uint8_t* rdram, recomp_context* ctx) {
    bool trace_pair = loaded_0f_pair == 99;
    if (trace_pair) {
        lod_pause_item_trace_log_pair99_item(rdram, ctx, "sub-pre");
    }
    if (lod_orig_pause_item_sub_entry != nullptr) {
        lod_orig_pause_item_sub_entry(rdram, ctx);
    }
    if (trace_pair) {
        lod_pause_item_trace_log_pair99_item(rdram, ctx, "sub-post");
    }
}

static void lod_install_pause_item_trace_wrappers(int pair_index, uint32_t vram,
                                                  const char* reason) {
    if (vram != 0x0F000000) {
        return;
    }

    lod_install_pause_item_text_trace_wrappers(reason);

    recomp_func_t* entry_current = get_function((int32_t)0x0F000000);
    if (entry_current != lod_trace_pause_item_entry) {
        lod_orig_pause_item_entry = entry_current;
    }
    recomp::overlays::add_loaded_function((int32_t)0x0F000000,
        lod_trace_pause_item_entry);

    if (pair_index == 99) {
        recomp_func_t* sub_current = get_function((int32_t)0x0F0027C0);
        if (sub_current != lod_trace_pause_item_sub_entry) {
            lod_orig_pause_item_sub_entry = sub_current;
        }
        recomp::overlays::add_loaded_function((int32_t)0x0F0027C0,
            lod_trace_pause_item_sub_entry);
    }

    static int install_count = 0;
    static int pair_install_counts[NI_OVL_COUNT] = {};
    install_count++;
    int pair_install_count = 0;
    if (pair_index >= 0 && pair_index < NI_OVL_COUNT) {
        pair_install_count = ++pair_install_counts[pair_index];
    }
    bool focused_pair = lod_pause_item_is_focused_pair(pair_index);
    if (install_count <= 12 || pair_install_count <= 3 ||
        (focused_pair && (pair_install_count % 100) == 0) ||
        (install_count % 500) == 0) {
        fprintf(stderr,
            "[ITEM_MENU] installed NI0F entry trace #%d pair_count=%d reason=%s pair=%d "
            "vram=0x%08X entry_orig=%p sub_orig=%p\n",
            install_count, pair_install_count, reason, pair_index, vram,
            (void*)lod_orig_pause_item_entry,
            pair_index == 99 ? (void*)lod_orig_pause_item_sub_entry : nullptr);
    }
}
#endif

#if LOD_ENABLE_TOWER_NI126_TRACE
static recomp_func_t* lod_orig_tower_ni126_entry = nullptr;
static recomp_func_t* lod_orig_tower_ni126_state_init = nullptr;
static recomp_func_t* lod_orig_tower_ni126_state_fade_in = nullptr;
static recomp_func_t* lod_orig_tower_ni126_state_wait = nullptr;
static recomp_func_t* lod_orig_tower_ni126_state_fade_out = nullptr;
static recomp_func_t* lod_orig_tower_ni126_state_destroy = nullptr;
static recomp_func_t* lod_orig_tower_ni18_entry = nullptr;

static bool lod_tower_ni_trace_enabled() {
    const uint32_t map_rom = lod_current_map_overlay_rom();
    return map_rom == 0x007D9790 || map_rom == 0x0082E330;
}

static uint32_t lod_tower_ni_u32(uint8_t* rdram, uint32_t base, int32_t off) {
    return (uint32_t)MEM_W(off, lod_ni_canonical_gpr(base));
}

static int16_t lod_tower_ni_s16(uint8_t* rdram, uint32_t base, int32_t off) {
    return (int16_t)MEM_H(off, lod_ni_canonical_gpr(base));
}

static uint16_t lod_tower_ni_u16(uint8_t* rdram, uint32_t base, int32_t off) {
    return (uint16_t)MEM_HU(off, lod_ni_canonical_gpr(base));
}

static uint8_t lod_tower_ni_u8(uint8_t* rdram, uint32_t base, int32_t off) {
    return (uint8_t)MEM_BU(off, lod_ni_canonical_gpr(base));
}

static int lod_tower_ni_log_this(uint32_t* counter, bool force = false) {
    uint32_t count = ++(*counter);
    if (force || count <= 80 || (count % 120) == 0) {
        return (int)count;
    }
    return 0;
}

static void lod_tower_ni_log_state(uint8_t* rdram, const char* tag, int call,
                                   int pair, const char* func, uint32_t obj) {
    if (!lod_tower_ni_trace_enabled()) {
        return;
    }

    uint32_t depth = (uint16_t)lod_tower_ni_s16(rdram, obj, 0x0E);
    uint8_t state0_count = lod_tower_ni_u8(rdram, obj, 0x08);
    uint8_t state0_index = lod_tower_ni_u8(rdram, obj, 0x09);
    uint32_t cb = lod_tower_ni_u32(rdram, obj, 0x10);
    uint32_t child = lod_tower_ni_u32(rdram, obj, 0x24);
    uint32_t entry_index = lod_tower_ni_u32(rdram, obj, 0x34);
    uint32_t timer = lod_tower_ni_u32(rdram, obj, 0x38);
    uint32_t limit = lod_tower_ni_u32(rdram, obj, 0x48);
    uint32_t frame = lod_tower_ni_u32(rdram, obj, 0x4C);
    uint32_t done = lod_tower_ni_u32(rdram, obj, 0x50);
    uint32_t linked = lod_tower_ni_u32(rdram, obj, 0x54);
    uint32_t flag58 = lod_tower_ni_u32(rdram, obj, 0x58);
    uint32_t unk5c = lod_tower_ni_u32(rdram, obj, 0x5C);
    uint32_t unk60 = lod_tower_ni_u32(rdram, obj, 0x60);

    uint32_t table_off = 0x844 + (entry_index * 12);
    uint16_t table_stage = 0;
    uint16_t table_entry = 0;
    uint8_t table_match_a = 0;
    uint8_t table_match_b = 0;
    uint8_t table_duration = 0;
    uint8_t table_color0 = 0;
    uint8_t table_color1 = 0;
    uint8_t table_color2 = 0;
    if (entry_index < 23) {
        table_stage = lod_tower_ni_u16(rdram, 0x0F000000, (int32_t)table_off + 0);
        table_entry = lod_tower_ni_u16(rdram, 0x0F000000, (int32_t)table_off + 2);
        table_match_a = lod_tower_ni_u8(rdram, 0x0F000000, (int32_t)table_off + 4);
        table_match_b = lod_tower_ni_u8(rdram, 0x0F000000, (int32_t)table_off + 5);
        table_duration = lod_tower_ni_u8(rdram, 0x0F000000, (int32_t)table_off + 6);
        table_color0 = lod_tower_ni_u8(rdram, 0x0F000000, (int32_t)table_off + 7);
        table_color1 = lod_tower_ni_u8(rdram, 0x0F000000, (int32_t)table_off + 8);
        table_color2 = lod_tower_ni_u8(rdram, 0x0F000000, (int32_t)table_off + 9);
    }

    int16_t stage_global = (int16_t)MEM_H(-0x546E, (gpr)(int32_t)0x801D0000);
    int16_t sys_gate = (int16_t)MEM_H(0x2B4E, (gpr)(int32_t)0x801C82C0);
    uint32_t exec_flags = lod_ni_telemetry_exec_flags(rdram);
    int gs = lod_ni_telemetry_gamestate(rdram);

    fprintf(stderr,
            "[TOWER_NI] %s #%d pair=%d func=%s obj=0x%08X gs=%d map#%d "
            "stage=0x%04X sys_gate=%d exec=0x%08X loaded0f=%d loaded0e=%d "
            "state={depth=%u count=%u index=%u cb=0x%08X child=0x%08X entry=%u "
            "timer=%d limit=%d frame=%d done=%d linked=0x%08X flag58=%d unk5c=0x%08X unk60=0x%08X} "
            "table={off=0x%03X stage=0x%04X entry=%u match=%u/%u duration=%u color=%02X/%02X/%02X}\n",
            tag, call, pair, func, obj, gs, lod_current_map_overlay_load_count(),
            (uint16_t)stage_global, sys_gate, exec_flags, loaded_0f_pair, loaded_0e_pair,
            depth, state0_count, state0_index, cb, child, entry_index,
            (int32_t)timer, (int32_t)limit, (int32_t)frame, (int32_t)done,
            linked, (int32_t)flag58, unk5c, unk60,
            table_off, table_stage, table_entry, table_match_a, table_match_b,
            table_duration, table_color0, table_color1, table_color2);
}

static void lod_tower_ni_trace_call(uint8_t* rdram, recomp_context* ctx,
                                    int pair, const char* func,
                                    recomp_func_t* original,
                                    uint32_t* counter,
                                    bool force_post = false) {
    uint32_t obj = (uint32_t)ctx->r4;
    int call = lod_tower_ni_log_this(counter);
    if (call != 0) {
        lod_tower_ni_log_state(rdram, "pre", call, pair, func, obj);
    }
    if (original != nullptr) {
        original(rdram, ctx);
    }
    if (call != 0 || force_post) {
        int post_call = call != 0 ? call : (int)*counter;
        lod_tower_ni_log_state(rdram, "post", post_call, pair, func, obj);
    }
}

static void lod_trace_tower_ni126_entry(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_tower_ni_trace_call(rdram, ctx, 126, "entry",
                            lod_orig_tower_ni126_entry, &counter, true);
}

static void lod_trace_tower_ni126_state_init(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_tower_ni_trace_call(rdram, ctx, 126, "state_init_0F000070",
                            lod_orig_tower_ni126_state_init, &counter, true);
}

static void lod_trace_tower_ni126_state_fade_in(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_tower_ni_trace_call(rdram, ctx, 126, "state_fade_in_0F000318",
                            lod_orig_tower_ni126_state_fade_in, &counter);
}

static void lod_trace_tower_ni126_state_wait(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_tower_ni_trace_call(rdram, ctx, 126, "state_wait_0F000490",
                            lod_orig_tower_ni126_state_wait, &counter, true);
}

static void lod_trace_tower_ni126_state_fade_out(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_tower_ni_trace_call(rdram, ctx, 126, "state_fade_out_0F000590",
                            lod_orig_tower_ni126_state_fade_out, &counter);
}

static void lod_trace_tower_ni126_state_destroy(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_tower_ni_trace_call(rdram, ctx, 126, "state_destroy_0F00064C",
                            lod_orig_tower_ni126_state_destroy, &counter, true);
}

static void lod_trace_tower_ni18_entry(uint8_t* rdram, recomp_context* ctx) {
    static uint32_t counter = 0;
    lod_tower_ni_trace_call(rdram, ctx, 18, "entry",
                            lod_orig_tower_ni18_entry, &counter, true);
}

static void lod_install_tower_ni_trace_wrapper(uint32_t vram,
                                               recomp_func_t* wrapper,
                                               recomp_func_t** original_out,
                                               const char* name) {
    recomp_func_t* current = get_function((int32_t)vram);
    if (current == wrapper) {
        return;
    }
    *original_out = current;
    recomp::overlays::add_loaded_function((int32_t)vram, wrapper);
    fprintf(stderr,
            "[TOWER_NI] installed %s wrapper vram=0x%08X original=%p wrapper=%p\n",
            name, vram, (void*)current, (void*)wrapper);
}

static void lod_install_tower_ni_trace_wrappers(int pair_index, uint32_t vram,
                                                const char* reason) {
    if (vram != 0x0F000000 || !lod_tower_ni_trace_enabled()) {
        return;
    }

    if (pair_index == 126) {
        lod_install_tower_ni_trace_wrapper(0x0F000000, lod_trace_tower_ni126_entry,
            &lod_orig_tower_ni126_entry, "pair126.entry");
        lod_install_tower_ni_trace_wrapper(0x0F000070, lod_trace_tower_ni126_state_init,
            &lod_orig_tower_ni126_state_init, "pair126.0F000070");
        lod_install_tower_ni_trace_wrapper(0x0F000318, lod_trace_tower_ni126_state_fade_in,
            &lod_orig_tower_ni126_state_fade_in, "pair126.0F000318");
        lod_install_tower_ni_trace_wrapper(0x0F000490, lod_trace_tower_ni126_state_wait,
            &lod_orig_tower_ni126_state_wait, "pair126.0F000490");
        lod_install_tower_ni_trace_wrapper(0x0F000590, lod_trace_tower_ni126_state_fade_out,
            &lod_orig_tower_ni126_state_fade_out, "pair126.0F000590");
        lod_install_tower_ni_trace_wrapper(0x0F00064C, lod_trace_tower_ni126_state_destroy,
            &lod_orig_tower_ni126_state_destroy, "pair126.0F00064C");
    }
    else if (pair_index == 18) {
        lod_install_tower_ni_trace_wrapper(0x0F000000, lod_trace_tower_ni18_entry,
            &lod_orig_tower_ni18_entry, "pair18.entry");
    }

    fprintf(stderr,
            "[TOWER_NI] trace wrappers considered pair=%d vram=0x%08X reason=%s map#%d map=0x%08X\n",
            pair_index, vram, reason, lod_current_map_overlay_load_count(),
            lod_current_map_overlay_rom());
}
#endif

#if LOD_FIX_PAIR126_INPUT_RELEASE
static uint32_t lod_pair126_input_release_generation_value = 0;
static uint32_t lod_pair126_input_release_map_count_value = 0;
#endif

extern "C" uint32_t lod_pair126_input_release_generation() {
#if LOD_FIX_PAIR126_INPUT_RELEASE
    return lod_pair126_input_release_generation_value;
#else
    return 0;
#endif
}

extern "C" uint32_t lod_pair126_input_release_map_count() {
#if LOD_FIX_PAIR126_INPUT_RELEASE
    return lod_pair126_input_release_map_count_value;
#else
    return 0;
#endif
}

#if LOD_FIX_PAIR126_INPUT_RELEASE
static recomp_func_t* lod_orig_pair126_input_release_state_init = nullptr;
static recomp_func_t* lod_orig_pair126_input_release_state_destroy = nullptr;

static constexpr uint32_t LOD_PAIR126_EXEC_FLAGS_PHYS = 0x001CABC8;
static constexpr uint32_t LOD_PAIR126_TRANSITION_LOCKED_FLAGS = 0x20000000;
static constexpr uint32_t LOD_PAIR126_CONTROLLABLE_FLAGS = 0x38000000;
static constexpr uint32_t LOD_PAIR126_MISSING_GAMEPLAY_FLAGS =
    LOD_PAIR126_CONTROLLABLE_FLAGS & ~LOD_PAIR126_TRANSITION_LOCKED_FLAGS;

static uint32_t lod_pair126_input_phys(uint32_t base, int32_t off) {
    return ((uint32_t)((int32_t)base + off)) & 0x1FFFFFFF;
}

static uint32_t lod_pair126_input_u32(uint8_t* rdram, uint32_t base, int32_t off) {
    uint32_t phys = lod_pair126_input_phys(base, off);
    return lod_ni_telemetry_range_ok(phys, 4) ? *(uint32_t*)(rdram + phys) : 0;
}

static int16_t lod_pair126_input_s16(uint8_t* rdram, uint32_t base, int32_t off) {
    uint32_t phys = lod_pair126_input_phys(base, off);
    return lod_ni_telemetry_range_ok(phys, 2) ? *(int16_t*)(rdram + (phys ^ 2)) : 0;
}

static uint16_t lod_pair126_input_u16(uint8_t* rdram, uint32_t base, int32_t off) {
    uint32_t phys = lod_pair126_input_phys(base, off);
    return lod_ni_telemetry_range_ok(phys, 2) ? *(uint16_t*)(rdram + (phys ^ 2)) : 0;
}

static uint8_t lod_pair126_input_u8(uint8_t* rdram, uint32_t base, int32_t off) {
    uint32_t phys = lod_pair126_input_phys(base, off);
    return lod_ni_telemetry_range_ok(phys, 1) ? rdram[phys ^ 3] : 0;
}

static uint8_t lod_pair126_overlay_u8(uint8_t* rdram, uint32_t base, int32_t off) {
    return (uint8_t)MEM_BU(off, lod_ni_canonical_gpr(base));
}

static uint16_t lod_pair126_overlay_u16(uint8_t* rdram, uint32_t base, int32_t off) {
    return (uint16_t)MEM_HU(off, lod_ni_canonical_gpr(base));
}

static bool lod_pair126_exec_flags_transition_locked(uint8_t* rdram) {
    if (lod_ni_telemetry_gamestate(rdram) != 3 ||
        !lod_ni_telemetry_range_ok(LOD_PAIR126_EXEC_FLAGS_PHYS, 4)) {
        return false;
    }

    uint32_t* exec_flags = (uint32_t*)(rdram + LOD_PAIR126_EXEC_FLAGS_PHYS);
    const uint32_t before = *exec_flags;
    return (before & LOD_PAIR126_CONTROLLABLE_FLAGS) ==
        LOD_PAIR126_TRANSITION_LOCKED_FLAGS;
}

static void lod_pair126_release_input_flags_if_stuck(uint8_t* rdram, const char* reason) {
    if (!lod_pair126_exec_flags_transition_locked(rdram)) {
        return;
    }

    uint32_t* exec_flags = (uint32_t*)(rdram + LOD_PAIR126_EXEC_FLAGS_PHYS);
    const uint32_t before = *exec_flags;
    const uint32_t after = before | LOD_PAIR126_MISSING_GAMEPLAY_FLAGS;
    *exec_flags = after;
    lod_pair126_input_release_generation_value++;
    lod_pair126_input_release_map_count_value =
        (uint32_t)lod_current_map_overlay_load_count();

    static int release_log_count = 0;
    release_log_count++;
    if (release_log_count <= 8 || (release_log_count % 120) == 0) {
        fprintf(stderr,
                "[PAIR126_INPUT_RELEASE_FIX] %s #%d exec 0x%08X -> 0x%08X map#%d map=0x%08X\n",
                reason, release_log_count, before, after,
                lod_current_map_overlay_load_count(),
                lod_current_map_overlay_rom());
    }
}

static bool lod_issue27_pair126_realign_stage_if_needed(uint8_t* rdram,
                                                        uint32_t obj,
                                                        const char* reason) {
#if LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN
    if (obj == 0 ||
        loaded_0f_pair != 126 ||
        lod_current_map_overlay_rom() != LOD_ISSUE27_PRE_HANDOFF_MAP_ROM ||
        lod_ni_telemetry_gamestate(rdram) != 3) {
        return false;
    }

    constexpr uint32_t stage_global_base = 0x801D0000;
    constexpr int32_t stage_global_off = -0x546E;
    const uint32_t stage_global_phys =
        lod_pair126_input_phys(stage_global_base, stage_global_off);
    if (!lod_ni_telemetry_range_ok(stage_global_phys, 2)) {
        return false;
    }

    const uint32_t entry_source = lod_pair126_input_u32(rdram, obj, 0x70);
    uint32_t entry_index = 0;
    if (entry_source != 0) {
        entry_index = lod_pair126_input_u16(rdram, entry_source, 0x18);
    }
    else {
        entry_index = lod_pair126_input_u32(rdram, obj, 0x34);
    }

    if (entry_index >= 23) {
        return false;
    }

    const int32_t table_off = 0x844 + ((int32_t)entry_index * 12);
    const uint16_t table_stage = lod_pair126_overlay_u16(rdram, 0x0F000000, table_off + 0);
    const uint16_t table_entry = lod_pair126_overlay_u16(rdram, 0x0F000000, table_off + 2);
    const uint8_t match_a = lod_pair126_overlay_u8(rdram, 0x0F000000, table_off + 4);
    const uint8_t match_b = lod_pair126_overlay_u8(rdram, 0x0F000000, table_off + 5);
    const int16_t stage_global =
        lod_pair126_input_s16(rdram, stage_global_base, stage_global_off);

    if (stage_global == (int16_t)match_a || stage_global == (int16_t)match_b) {
        return false;
    }

    const int16_t target_stage = match_b != 0 ? (int16_t)match_b : (int16_t)match_a;
    if (target_stage == stage_global) {
        return false;
    }

    *(int16_t*)(rdram + (stage_global_phys ^ 2)) = target_stage;

    static int realign_log_count = 0;
    realign_log_count++;
    fprintf(stderr,
            "[ISSUE27_PAIR126_STAGE_REALIGN] #%d reason=%s obj=0x%08X "
            "map#%d map=0x%08X exec=0x%08X entry_source=0x%08X entry=%u "
            "stage %d->%d table={off=0x%03X stage=0x%04X entry=%u match=%u/%u} "
            "loaded0f=%d loaded0e=%d\n",
            realign_log_count, reason, obj, lod_current_map_overlay_load_count(),
            lod_current_map_overlay_rom(), lod_ni_telemetry_exec_flags(rdram),
            entry_source, entry_index, stage_global, target_stage, table_off,
            table_stage, table_entry, match_a, match_b, loaded_0f_pair,
            loaded_0e_pair);
    return true;
#else
    (void)rdram;
    (void)obj;
    (void)reason;
    return false;
#endif
}

static bool lod_pair126_init_returned_without_fade(uint8_t* rdram, uint32_t obj) {
    if (obj == 0) {
        return false;
    }

    // Pair126 normally creates a fade child/link during init and later reaches
    // state_destroy, which restores the normal gameplay/control flags. Some
    // restart or transition routes return from init with only the root state
    // left, so no later pair126 cleanup can run while exec flags stay locked.
    if (loaded_0f_pair != 126 ||
        !lod_pair126_exec_flags_transition_locked(rdram)) {
        return false;
    }

    int16_t stage_global = lod_pair126_input_s16(rdram, 0x801D0000, -0x546E);
    uint8_t state0_count = lod_pair126_input_u8(rdram, obj, 0x08);
    uint8_t state0_index = lod_pair126_input_u8(rdram, obj, 0x09);
    uint32_t child = lod_pair126_input_u32(rdram, obj, 0x24);
    uint32_t entry_index = lod_pair126_input_u32(rdram, obj, 0x34);
    uint32_t timer = lod_pair126_input_u32(rdram, obj, 0x38);
    uint32_t limit = lod_pair126_input_u32(rdram, obj, 0x48);
    uint32_t frame = lod_pair126_input_u32(rdram, obj, 0x4C);
    uint32_t done = lod_pair126_input_u32(rdram, obj, 0x50);
    uint32_t linked = lod_pair126_input_u32(rdram, obj, 0x54);

    return stage_global != 0 &&
        state0_count == 1 &&
        state0_index == 0 &&
        entry_index > 0 &&
        entry_index < 23 &&
        child == 0 &&
        linked == 0 &&
        timer == 0 &&
        limit == 0 &&
        frame == 0 &&
        done == 0;
}

static void lod_fix_pair126_input_release_state_init(uint8_t* rdram, recomp_context* ctx) {
    uint32_t obj = (uint32_t)ctx->r4;
    lod_issue27_pair126_realign_stage_if_needed(rdram, obj, "pre-init");
    if (lod_orig_pair126_input_release_state_init != nullptr) {
        lod_orig_pair126_input_release_state_init(rdram, ctx);
    }
    if (lod_pair126_init_returned_without_fade(rdram, obj)) {
        lod_pair126_release_input_flags_if_stuck(rdram, "pair126.init-no-fade");
    }
}

static void lod_fix_pair126_input_release_state_destroy(uint8_t* rdram, recomp_context* ctx) {
    if (lod_orig_pair126_input_release_state_destroy != nullptr) {
        lod_orig_pair126_input_release_state_destroy(rdram, ctx);
    }
    lod_pair126_release_input_flags_if_stuck(rdram, "pair126.destroy");
}

static void lod_install_pair126_input_release_single(uint32_t func_vram,
                                                   recomp_func_t* wrapper,
                                                   recomp_func_t** original_out,
                                                   const char* name,
                                                   const char* reason) {
    recomp_func_t* current = get_function((int32_t)func_vram);
    if (current == wrapper) {
        return;
    }

    *original_out = current;
    recomp::overlays::add_loaded_function((int32_t)func_vram, wrapper);

    static int install_count = 0;
    install_count++;
    if (install_count <= 6 || (install_count % 120) == 0) {
        fprintf(stderr,
                "[PAIR126_INPUT_RELEASE_FIX] installed %s wrapper #%d "
                "reason=%s original=%p map=0x%08X\n",
                name, install_count, reason, (void*)current,
                lod_current_map_overlay_rom());
    }
}

static void lod_install_pair126_input_release_wrapper(int pair_index, uint32_t vram,
                                                    const char* reason) {
    if (pair_index != 126 || vram != 0x0F000000) {
        return;
    }

    lod_install_pair126_input_release_single(0x0F000070,
        lod_fix_pair126_input_release_state_init,
        &lod_orig_pair126_input_release_state_init,
        "pair126.init", reason);
    lod_install_pair126_input_release_single(0x0F00064C,
        lod_fix_pair126_input_release_state_destroy,
        &lod_orig_pair126_input_release_state_destroy,
        "pair126.destroy", reason);
}
#endif

#if LOD_FIX_HENRY_LOAD_HANDOFF
// Root-cause fix for issue #27/#31's scenario-controller gap.
//
// Proven chain (docs/issue27-31-fix-design.md Round 12, docs/issue27-31-ni0e-findings.md):
// object 0x016's cutscene launcher (func_80189164) runs every frame and
// searches a 93-entry table at 0x80197170 (10 bytes/record, halfword +0x2 =
// cutscene id) for a record matching sys+0x2BC8 ("requested cutscene").
// Cutscene 0x61 (Henry's opening/briefing) is the record that spawns the
// scenario controllers (ids 0x083/0x084/0x062/0x196) the day-banner, pause
// manager, and Edward spawn logic all depend on. On the fresh new-game path,
// NI pair 104's object 0x00F menu-flow state machine queues a record index
// into sys+0x2BCC via the shared library helper at NI-window offset
// 0x0F00082C (canonical definition RecompiledFuncs/funcs_243.c:726,
// identical bytes duplicated into RecompiledFuncs/funcs_212.c:7582+ since
// pair 104 embeds the same static routine), and the common "enter gameplay"
// state handler func_8001A8C4 (RecompiledFuncs/funcs_13.c:5) unconditionally
// commits sys+0x2BCC into sys+0x2BC8. On a Henry save-load, nothing ever
// writes sys+0x2BCC/0x2BC8: the new-game menu flow never runs (the player
// skipped straight to gameplay), and this round's exhaustive read of NI pair
// 126's entire state machine (RecompiledFuncs/funcs_218.c:4451-5542: entry,
// state_init, fade_in, wait, fade_out, state_destroy) found zero references
// to sys+0x2BC8/0x2BCC/0x2BD0 anywhere in its code -- pair 126's early-bail
// branch (the dormant LOD_ENABLE_ISSUE27_PAIR126_STAGE_REALIGN target) feeds
// a *different* mechanism (the bgState_activate exec-flags gate) and is not
// the cutscene-request producer, refuting the round-11 hypothesis that
// widening that shim's map gate would restore controller creation.
//
// Root fix (this flag): correct the divergent *input* (sys+0x2BC8/0x2BCC),
// the same shape as the existing stage-realign shim correcting sys+0x28D2,
// so object 0x016's own native search-and-spawn logic does all the actual
// work (table lookup, scenario-controller creation, cutscene state machine,
// day-banner/pause/Edward follow-on). Nothing about object creation, exec
// flags, or day-system state is forced directly; only the one request field
// pair a legitimate producer would have written is corrected, mirroring
// exactly what NI static_158_0F00082C's own "immediate commit" branch does
// for every other in-game cutscene request (queue + commit + sys+0x2BD0 bit
// 0x10), so the write shape matches a real, already-existing game mechanism.
//
// Guard (checked once per VI from lod_fix_henry_load_handoff_tick, called
// from main.cpp's per-frame debug-cheats callback so it does not depend on
// pair 126 being involved in any particular room's transition):
//   1. gamestate == 3 (gameplay running).
//   2. A short grace window (LOD_HENRY_HANDOFF_GRACE_FRAMES) after gameplay
//      is first observed running, during which sys+0x2BC8/0x2BCC are
//      sampled every frame; if either is ever seen nonzero, the native path
//      already made its own request (fresh new-game, or any other legitimate
//      producer) and the fix stands down for this gameplay session.
//   3. If the grace window elapses with sys+0x2BC8 and sys+0x2BCC having
//      NEVER been observed nonzero, confirm this is genuinely a Henry
//      session (not Reinhardt/normal, whose sessions never reach this state
//      with a live Henry root object) by reading the per-id live-instance
//      counter table at 0x801B3D60 (index (id-1)*2, confirmed by reading
//      both object_createAndSetChild's increment and func_800020E8's
//      decrement in RecompiledFuncs/funcs_1.c) for id 0x1AB, the Henry root
//      family object round-6's differential capture showed is created on
//      every loaded-Henry session and never on non-Henry sessions.
//   4. One-shot per gameplay-entry window: re-arms only if gamestate leaves
//      3 and returns (covers retry/game-over/reload-a-different-save within
//      one process), so it cannot re-fire every frame or fight a later,
//      legitimate zero-request idle moment (sys+0x2BC8 legitimately returns
//      to 0 between ordinary in-game cutscenes).
static constexpr uint32_t LOD_HENRY_HANDOFF_SYS_PHYS = 0x001C82C0;
static constexpr uint32_t LOD_HENRY_HANDOFF_CUTSCENE_REQ_PHYS =
    LOD_HENRY_HANDOFF_SYS_PHYS + 0x2BC8;
static constexpr uint32_t LOD_HENRY_HANDOFF_CUTSCENE_QUEUE_PHYS =
    LOD_HENRY_HANDOFF_SYS_PHYS + 0x2BCC;
static constexpr uint32_t LOD_HENRY_HANDOFF_COMMIT_FLAGS_PHYS =
    LOD_HENRY_HANDOFF_SYS_PHYS + 0x2BD0;
static constexpr uint32_t LOD_HENRY_HANDOFF_CUTSCENE_ID = 0x61;
static constexpr uint32_t LOD_HENRY_HANDOFF_COMMIT_BIT = 0x10;
static constexpr uint32_t LOD_HENRY_HANDOFF_INSTANCE_TABLE_PHYS = 0x001B3D60;
static constexpr uint32_t LOD_HENRY_HANDOFF_ROOT_OBJECT_ID = 0x1AB;
static constexpr uint32_t LOD_HENRY_HANDOFF_GRACE_FRAMES = 90;
static constexpr uint32_t LOD_HENRY_HANDOFF_GIVEUP_FRAMES = 300;

static bool lod_henry_handoff_root_family_present(uint8_t* rdram) {
    const uint32_t entry_off = (LOD_HENRY_HANDOFF_ROOT_OBJECT_ID - 1) * 2;
    const uint32_t phys = (LOD_HENRY_HANDOFF_INSTANCE_TABLE_PHYS + entry_off) ^ 2;
    if (!lod_ni_telemetry_range_ok(phys, 2)) {
        return false;
    }
    return *(uint16_t*)(rdram + phys) != 0;
}

extern "C" void lod_fix_henry_load_handoff_tick(uint8_t* rdram) {
    static bool fired = false;
    static bool ever_saw_request = false;
    static uint32_t frames_in_gameplay = 0;
    static int32_t last_gamestate = -1;

    if (fired || rdram == nullptr) {
        return;
    }

    const int32_t gamestate = lod_ni_telemetry_gamestate(rdram);
    if (gamestate != 3) {
        if (last_gamestate == 3) {
            // Left gameplay: re-arm so a later, distinct gameplay entry
            // (retry, game-over-continue, load a different save) gets its
            // own fresh grace window instead of inheriting stale state.
            ever_saw_request = false;
            frames_in_gameplay = 0;
        }
        last_gamestate = gamestate;
        return;
    }
    last_gamestate = 3;
    frames_in_gameplay++;

    if (!lod_ni_telemetry_range_ok(LOD_HENRY_HANDOFF_CUTSCENE_REQ_PHYS, 8)) {
        return;
    }
    const uint32_t cutscene_req =
        lod_ni_telemetry_u32(rdram, LOD_HENRY_HANDOFF_CUTSCENE_REQ_PHYS);
    const uint32_t cutscene_queue =
        lod_ni_telemetry_u32(rdram, LOD_HENRY_HANDOFF_CUTSCENE_QUEUE_PHYS);
    if (cutscene_req != 0 || cutscene_queue != 0) {
        // Some legitimate producer requested a cutscene at some point during
        // this gameplay session (fresh new-game, or an ordinary in-map
        // cutscene). Never override the game's own request stream.
        ever_saw_request = true;
    }

    if (frames_in_gameplay < LOD_HENRY_HANDOFF_GRACE_FRAMES) {
        return;
    }
    if (ever_saw_request) {
        return;
    }

    if (!lod_henry_handoff_root_family_present(rdram)) {
        // Not (yet, or ever) a Henry session. Keep sampling until the
        // give-up deadline in case the root family is created slightly late;
        // after that, stand down permanently for this gameplay window.
        if (frames_in_gameplay >= LOD_HENRY_HANDOFF_GIVEUP_FRAMES) {
            fired = true;
        }
        return;
    }

    // Confirmed: Henry session, gameplay running for LOD_HENRY_HANDOFF_GRACE_FRAMES
    // frames, and sys+0x2BC8/0x2BCC have never once been observed nonzero.
    // This is exactly the differential signature of the load-path bug (fresh
    // Henry sessions commit 0x61 within a handful of frames of gameplay
    // start). Correct the request fields the same way NI's own
    // static_158_0F00082C "immediate commit" branch does for every other
    // in-game cutscene request, then stand down permanently.
    uint32_t* cutscene_req_ptr =
        (uint32_t*)(rdram + LOD_HENRY_HANDOFF_CUTSCENE_REQ_PHYS);
    uint32_t* cutscene_queue_ptr =
        (uint32_t*)(rdram + LOD_HENRY_HANDOFF_CUTSCENE_QUEUE_PHYS);
    *cutscene_queue_ptr = LOD_HENRY_HANDOFF_CUTSCENE_ID;
    *cutscene_req_ptr = LOD_HENRY_HANDOFF_CUTSCENE_ID;
    if (lod_ni_telemetry_range_ok(LOD_HENRY_HANDOFF_COMMIT_FLAGS_PHYS, 4)) {
        uint32_t* commit_flags_ptr =
            (uint32_t*)(rdram + LOD_HENRY_HANDOFF_COMMIT_FLAGS_PHYS);
        *commit_flags_ptr |= LOD_HENRY_HANDOFF_COMMIT_BIT;
    }

    fired = true;
    fprintf(stderr,
            "[HENRY_LOAD_HANDOFF] requested cutscene 0x%02X after %u gameplay "
            "frames with no native request seen (map=0x%08X map#%d gs=%d)\n",
            LOD_HENRY_HANDOFF_CUTSCENE_ID, frames_in_gameplay,
            lod_current_map_overlay_rom(), lod_current_map_overlay_load_count(),
            gamestate);
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
static uint8_t* ni_segment_base(uint8_t* rdram, uint32_t vram) {
    return rdram + (uint64_t)vram + 0x80000000ULL;
}

#if LOD_FIX_NI_SEG6_CPU_ALIAS
// Some LoD model/node records carry segment-6 pointers (0x06xxxxxx) into data
// that lives in the reusable NI segment mirror. Recompiled CPU MEM_* accesses do
// not perform RSP-style segment lookup, so 0x06xxxxxx maps to rdram+0x86xxxxxx.
// Keep that alias populated with the active NI segment image. This fixes
// CPU-side segmented-pointer dereferences such as 0x0600635C, which otherwise
// become host accesses through the unmapped 0x86000000 KSEG0 mirror.
static constexpr uint32_t NI_SEG6_CPU_ALIAS_VADDR = 0x06000000;
static constexpr uint32_t NI_SEG6_CPU_ALIAS_MAX_SIZE = 0x00100000;

static void ni_mirror_segment_to_seg6_cpu_alias(uint8_t* rdram,
                                                const uint8_t* segment_base,
                                                int pair_index,
                                                uint32_t vram,
                                                uint32_t size,
                                                const char* reason) {
    if (size == 0 || size > NI_SEG6_CPU_ALIAS_MAX_SIZE) {
        return;
    }

    uint8_t* alias = rdram + (uint64_t)NI_SEG6_CPU_ALIAS_VADDR + 0x80000000ULL;
    uintptr_t page_mask = sysconf(_SC_PAGESIZE) - 1;
    uint8_t* aligned = (uint8_t*)((uintptr_t)alias & ~page_mask);
    size_t aligned_size = ((alias + size) - aligned + page_mask) & ~page_mask;
    if (mprotect(aligned, aligned_size, PROT_READ | PROT_WRITE) != 0) {
        static int fail_count = 0;
        fail_count++;
        if (fail_count <= 8) {
            fprintf(stderr,
                    "[NI_SEG6_ALIAS] mprotect failed #%d pair=%d vram=0x%08X "
                    "size=0x%X reason=%s\n",
                    fail_count, pair_index, vram, size, reason);
        }
        return;
    }

    memcpy(alias, segment_base, size);

#if LOD_ENABLE_TLB_SEG6_TRACE
    static int copy_count = 0;
    copy_count++;
    if (copy_count <= 24 || (copy_count % 200) == 0) {
        const uint32_t w0 = size >= 4 ? *(const uint32_t*)(segment_base + 0x00) : 0;
        const uint32_t w4 = size >= 8 ? *(const uint32_t*)(segment_base + 0x04) : 0;
        const uint32_t w73dc = size >= 0x73E0 ? *(const uint32_t*)(segment_base + 0x73DC) : 0;
        fprintf(stderr,
                "[NI_SEG6_ALIAS] copy#%d pair=%d vram=0x%08X size=0x%X "
                "reason=%s alias=0x%08X words={0x%08X,0x%08X,+73DC=0x%08X}\n",
                copy_count, pair_index, vram, size, reason,
                NI_SEG6_CPU_ALIAS_VADDR, w0, w4, w73dc);
    }
#endif
}
#endif

#if LOD_FIX_NI_PAIR99_ITEM_LIST_PERSIST
// Pair 99's pause item-list code keeps the built item table in its NI data
// segment at 0x0F004EA0. In-game this survives the rapid pair99/pair101 TLB
// alternation used by the scrolling menu child. Our single 0x8F segment mirror
// would otherwise replace that mutable table with pristine ROM bytes whenever
// another pair is copied into the mirror.
static constexpr uint32_t NI_PAIR99_ITEM_LIST_OFF = 0x4EA0;
static constexpr uint32_t NI_PAIR99_ITEM_LIST_SIZE = 0x00CC; // up to dispatch data at 0x4F6C

static uint8_t ni_pair99_saved_item_list[NI_PAIR99_ITEM_LIST_SIZE] = {};
static bool ni_pair99_saved_item_list_valid = false;

static uint16_t ni_segment_u16(uint8_t* segment_base, uint32_t offset) {
    return *(uint16_t*)(segment_base + (offset ^ 2));
}

static bool ni_pair99_item_list_looks_live(uint8_t* segment_base) {
    int populated = 0;
    for (int i = 0; i < 16; i++) {
        uint32_t offset = NI_PAIR99_ITEM_LIST_OFF + (uint32_t)i * 4;
        uint16_t id = ni_segment_u16(segment_base, offset + 0);
        uint16_t amount = ni_segment_u16(segment_base, offset + 2);
        if (id == 0 && amount == 0) {
            continue;
        }
        if (id < 4 || id > 0x2D || amount == 0 || amount > 999) {
            return false;
        }
        populated++;
    }
    return populated > 0;
}

static void ni_pair99_save_item_list_if_live(uint8_t* rdram, uint32_t vram,
                                             uint32_t old_size,
                                             const char* reason) {
    if (vram != 0x0F000000 ||
        old_size < NI_PAIR99_ITEM_LIST_OFF + NI_PAIR99_ITEM_LIST_SIZE) {
        return;
    }

    uint8_t* segment_base = ni_segment_base(rdram, vram);
    if (!ni_pair99_item_list_looks_live(segment_base)) {
        return;
    }

    memcpy(ni_pair99_saved_item_list,
           segment_base + NI_PAIR99_ITEM_LIST_OFF,
           sizeof(ni_pair99_saved_item_list));
    ni_pair99_saved_item_list_valid = true;

#if LOD_ENABLE_PAUSE_ITEM_TRACE
    static int save_count = 0;
    save_count++;
    if (save_count <= 8 || (save_count % 100) == 0) {
        fprintf(stderr,
            "[ITEM_LIST_FIX] saved pair99 list #%d reason=%s first=%u:%u\n",
            save_count, reason,
            ni_segment_u16(segment_base, NI_PAIR99_ITEM_LIST_OFF + 0),
            ni_segment_u16(segment_base, NI_PAIR99_ITEM_LIST_OFF + 2));
    }
#else
    (void)reason;
#endif
}

static void ni_pair99_restore_item_list_if_saved(uint8_t* segment_base,
                                                 int pair_index,
                                                 uint32_t vram,
                                                 uint32_t size) {
    if (pair_index != 99 || vram != 0x0F000000 ||
        !ni_pair99_saved_item_list_valid ||
        size < NI_PAIR99_ITEM_LIST_OFF + NI_PAIR99_ITEM_LIST_SIZE) {
        return;
    }

    memcpy(segment_base + NI_PAIR99_ITEM_LIST_OFF,
           ni_pair99_saved_item_list,
           sizeof(ni_pair99_saved_item_list));

#if LOD_ENABLE_PAUSE_ITEM_TRACE
    static int restore_count = 0;
    restore_count++;
    if (restore_count <= 8 || (restore_count % 100) == 0) {
        fprintf(stderr,
            "[ITEM_LIST_FIX] restored pair99 list #%d first=%u:%u\n",
            restore_count,
            ni_segment_u16(segment_base, NI_PAIR99_ITEM_LIST_OFF + 0),
            ni_segment_u16(segment_base, NI_PAIR99_ITEM_LIST_OFF + 2));
    }
#endif
}
#endif


#if LOD_FIX_RUN_DL_STALE_NI_FALLBACK
struct LodNiStaleOverlaySnapshot {
    std::vector<uint8_t> bytes;
    bool valid = false;
};

static std::array<LodNiStaleOverlaySnapshot, NI_PAIR_COUNT> ni_stale_0f_data;
static std::array<LodNiStaleOverlaySnapshot, NI_PAIR_COUNT> ni_stale_0e_data;
static std::array<int, NI_PAIR_COUNT> ni_stale_0f_recent = {};
static std::array<int, NI_PAIR_COUNT> ni_stale_0e_recent = {};
static uint32_t ni_stale_0f_recent_count = 0;
static uint32_t ni_stale_0e_recent_count = 0;

static std::array<int, NI_PAIR_COUNT>* ni_stale_recent_for_vram(uint32_t vram, uint32_t** count_out) {
    if (vram == 0x0F000000) {
        *count_out = &ni_stale_0f_recent_count;
        return &ni_stale_0f_recent;
    }
    if (vram == 0x0E000000) {
        *count_out = &ni_stale_0e_recent_count;
        return &ni_stale_0e_recent;
    }
    *count_out = nullptr;
    return nullptr;
}

static LodNiStaleOverlaySnapshot* ni_stale_snapshot_for_vram(uint32_t vram, int pair_index) {
    if (pair_index < 0 || pair_index >= NI_OVL_COUNT) {
        return nullptr;
    }
    if (vram == 0x0F000000) {
        return &ni_stale_0f_data[(size_t)pair_index];
    }
    if (vram == 0x0E000000) {
        return &ni_stale_0e_data[(size_t)pair_index];
    }
    return nullptr;
}

static void ni_stale_mark_recent(uint32_t vram, int pair_index) {
    uint32_t* count = nullptr;
    std::array<int, NI_PAIR_COUNT>* recent = ni_stale_recent_for_vram(vram, &count);
    if (recent == nullptr || count == nullptr || pair_index < 0 || pair_index >= NI_OVL_COUNT) {
        return;
    }

    uint32_t found = NI_PAIR_COUNT;
    for (uint32_t i = 0; i < *count; i++) {
        if ((*recent)[i] == pair_index) {
            found = i;
            break;
        }
    }
    if (found != NI_PAIR_COUNT) {
        for (uint32_t i = found; i > 0; i--) {
            (*recent)[i] = (*recent)[i - 1];
        }
        (*recent)[0] = pair_index;
        return;
    }

    const uint32_t max_count = (uint32_t)recent->size();
    if (*count < max_count) {
        (*count)++;
    }
    for (uint32_t i = *count - 1; i > 0; i--) {
        (*recent)[i] = (*recent)[i - 1];
    }
    (*recent)[0] = pair_index;
}

static void ni_stale_save_overlay_data(uint8_t* rdram, uint32_t vram, int pair_index) {
    LodNiStaleOverlaySnapshot* slot = ni_stale_snapshot_for_vram(vram, pair_index);
    if (slot == nullptr) {
        return;
    }

    const uint32_t size = ni_ovl_data[pair_index].full_size;
    uint8_t* segment_base = ni_segment_base(rdram, vram);
    slot->bytes.resize(size);
    memcpy(slot->bytes.data(), segment_base, size);
    slot->valid = true;
    ni_stale_mark_recent(vram, pair_index);
}

static bool ni_stale_base_and_offset(uint32_t segmented_address, uint32_t* vram_out, uint32_t* offset_out) {
    const uint32_t hi = segmented_address & 0xFF000000u;
    if (hi == 0x0F000000u || hi == 0x8F000000u) {
        *vram_out = 0x0F000000u;
        *offset_out = segmented_address & 0x00FFFFFFu;
        return true;
    }
    if (hi == 0x0E000000u || hi == 0x8E000000u) {
        *vram_out = 0x0E000000u;
        *offset_out = segmented_address & 0x00FFFFFFu;
        return true;
    }
    return false;
}

extern "C" const void* lod_ni_stale_dl_candidate(uint8_t* rdram, uint32_t segmented_address,
                                                  uint32_t min_size, uint32_t attempt,
                                                  int* pair_out, uint32_t* source_out) {
    uint32_t vram = 0;
    uint32_t offset = 0;
    if (!ni_stale_base_and_offset(segmented_address, &vram, &offset)) {
        return nullptr;
    }

    uint32_t* count = nullptr;
    std::array<int, NI_PAIR_COUNT>* recent = ni_stale_recent_for_vram(vram, &count);
    if (recent == nullptr || count == nullptr) {
        return nullptr;
    }

    int current_pair = -1;
    if (vram == 0x0F000000u) {
        current_pair = loaded_0f_pair;
    }
    else if (vram == 0x0E000000u) {
        current_pair = loaded_0e_pair;
    }

    uint32_t visible_index = 0;
    for (uint32_t i = 0; i < *count; i++) {
        const int pair = (*recent)[i];
        if (pair < 0 || pair >= NI_OVL_COUNT || pair == current_pair) {
            continue;
        }

        const NiOvlData& data = ni_ovl_data[pair];
        if (min_size > data.full_size || offset > data.full_size - min_size) {
            continue;
        }
        if (visible_index++ != attempt) {
            continue;
        }

        const LodNiStaleOverlaySnapshot* snapshot = ni_stale_snapshot_for_vram(vram, pair);
        if (snapshot != nullptr && snapshot->valid && snapshot->bytes.size() == data.full_size) {
            if (pair_out != nullptr) {
                *pair_out = pair;
            }
            if (source_out != nullptr) {
                *source_out = 1; // outgoing-pair snapshot
            }
            return snapshot->bytes.data() + offset;
        }

        if (pair_out != nullptr) {
            *pair_out = pair;
        }
        if (source_out != nullptr) {
            *source_out = 2; // pristine extended-ROM data
        }
        return rdram + 0x10000000u + data.rom_offset + offset;
    }

    return nullptr;
}
#endif

#if LOD_FIX_NI_PERSIST_OVERLAY_DATA
struct LodNiPersistedOverlayData {
    std::vector<uint8_t> bytes;
    bool valid = false;
};

static std::array<LodNiPersistedOverlayData, NI_PAIR_COUNT> ni_persist_0f_data;
static std::array<LodNiPersistedOverlayData, NI_PAIR_COUNT> ni_persist_0e_data;

static LodNiPersistedOverlayData* ni_persist_slot_for_vram(uint32_t vram, int pair_index) {
    if (pair_index < 0 || pair_index >= NI_OVL_COUNT) {
        return nullptr;
    }
    if (vram == 0x0F000000) {
        return &ni_persist_0f_data[(size_t)pair_index];
    }
    if (vram == 0x0E000000) {
        return &ni_persist_0e_data[(size_t)pair_index];
    }
    return nullptr;
}

static void ni_persist_save_overlay_data(uint8_t* rdram, uint32_t vram,
                                         int pair_index, const char* reason) {
    LodNiPersistedOverlayData* slot = ni_persist_slot_for_vram(vram, pair_index);
    if (slot == nullptr) {
        return;
    }

    const uint32_t size = ni_ovl_data[pair_index].full_size;
    uint8_t* segment_base = ni_segment_base(rdram, vram);
    slot->bytes.resize(size);
    memcpy(slot->bytes.data(), segment_base, size);
    slot->valid = true;

    (void)reason;
}

static void ni_persist_restore_overlay_data_if_saved(uint8_t* segment_base,
                                                     int pair_index,
                                                     uint32_t vram,
                                                     uint32_t size) {
    LodNiPersistedOverlayData* slot = ni_persist_slot_for_vram(vram, pair_index);
    if (slot == nullptr || !slot->valid || slot->bytes.size() != size) {
        return;
    }

    memcpy(segment_base, slot->bytes.data(), size);
}
#endif

static void copy_overlay_data_to_segment(uint8_t* rdram, int pair_index, uint32_t vram,
                                          const char* path_tag = "unspecified") {
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

#if LOD_ENABLE_NI0E_TRACE
    // Round 8, task 4: check BEFORE the copy whether this recopy is about to
    // overwrite the currently-registered days-banner text struct pointer.
    lod_ni0e_check_textbuf_reload(rdram, dst, size, path_tag);
#endif

    memcpy(dst, src, size);

#if LOD_FIX_NI_PERSIST_OVERLAY_DATA
    ni_persist_restore_overlay_data_if_saved(dst, pair_index, vram, size);
#endif

#if LOD_FIX_NI_PAIR99_ITEM_LIST_PERSIST
    ni_pair99_restore_item_list_if_saved(dst, pair_index, vram, size);
#endif

#if LOD_FIX_NI_SEG6_CPU_ALIAS
    ni_mirror_segment_to_seg6_cpu_alias(rdram, dst, pair_index, vram, size, "copy");
#endif
}

static void load_ni_overlay(uint8_t* rdram, int pair_index, uint32_t mapped_vaddr,
                             uint32_t even_paddr, uint32_t odd_paddr) {
    uint32_t vram = mapped_vaddr;
    int& loaded_pair = (vram == 0x0E000000) ? loaded_0e_pair : loaded_0f_pair;
#if LOD_ENABLE_NI0E_TRACE
    const int ni0e_prev_pair_for_log = loaded_pair;
#endif

#if LOD_FIX_NI_PRESERVE_SAME_PAIR_DATA
    // Repeated osMapTLB calls for the already-resident NI pair are page remaps,
    // not a fresh overlay load. Re-copying the full ROM image here erases
    // mutable overlay data that the game keeps in the 0x0E/0x0F segment (for
    // example pause-menu item-list entries at pair 99's 0x0F004EA0), causing
    // rows/icons to disappear after creation. Different pairs still unload and
    // reload below, which resets overlay data when the game actually swaps NI
    // overlays.
    if (pair_index == loaded_pair) {
#if LOD_FIX_NI_SEG6_CPU_ALIAS
        ni_mirror_segment_to_seg6_cpu_alias(rdram, ni_segment_base(rdram, vram),
            pair_index, vram, ni_ovl_data[pair_index].full_size, "same-pair");
#endif
#if LOD_FIX_RUN_DL_STALE_NI_FALLBACK
        ni_stale_mark_recent(vram, pair_index);
#endif
#if LOD_ENABLE_NI129_STATE_WRAPPERS
        lod_issue23_log_ni(rdram, "same-pair", pair_index, vram, mapped_vaddr);
        lod_install_issue23_ni129_trace_wrappers(pair_index, vram, "same-pair");
#endif
#if LOD_ENABLE_ISSUE27_PAIR51_TRACE
        lod_install_issue27_pair51_trace_wrappers(pair_index, vram, "same-pair");
#endif
#if LOD_ENABLE_ISSUE27_PAUSE_GATE_TRACE
        lod_install_issue27_pause_trace_wrappers(pair_index, vram, "same-pair");
#endif
#if LOD_ENABLE_NI0E_TRACE
        if (vram == 0x0E000000) {
            lod_ni0e_log_slot_event(rdram, "map-same", pair_index, ni0e_prev_pair_for_log,
                                     even_paddr, odd_paddr);
        }
#endif
        return;
    }
#endif

    // Save mutable data that belongs to the outgoing NI pair before the shared
    // 0x8F/0x8E mirror is overwritten by the incoming pair's pristine ROM data.
#if LOD_FIX_RUN_DL_STALE_NI_FALLBACK
    if (loaded_pair >= 0 && pair_index != loaded_pair) {
        ni_stale_save_overlay_data(rdram, vram, loaded_pair);
    }
#endif
#if LOD_FIX_NI_PERSIST_OVERLAY_DATA
    if (loaded_pair >= 0 && pair_index != loaded_pair) {
        ni_persist_save_overlay_data(rdram, vram, loaded_pair, "pair-swap");
    }
#endif
#if LOD_FIX_NI_PAIR99_ITEM_LIST_PERSIST
    if (vram == 0x0F000000 && loaded_pair == 99 && pair_index != loaded_pair) {
        ni_pair99_save_item_list_if_live(rdram, vram,
            ni_ovl_data[loaded_pair].full_size, "pair-swap");
    }
#endif

    // Always keep the actual TLB-mapped segment populated. Use the TLB vaddr as
    // the source of truth: some overlays generated with 0x0F comments are
    // executed through 0x0E in-game, and their dispatch tables contain 0x0E
    // function pointers (for example pair 233).
    copy_overlay_data_to_segment(rdram, pair_index, mapped_vaddr, "load_ni_overlay-pre-swap");

#if !LOD_FIX_NI_PRESERVE_SAME_PAIR_DATA
    if (pair_index == loaded_pair) return;
#endif

    if (loaded_pair >= 0) {
        unload_ni_overlay_segment(rdram, vram);
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
    copy_overlay_data_to_segment(rdram, pair_index, vram, "load_ni_overlay-post-load_overlays");
    loaded_pair = pair_index;
#if LOD_ENABLE_NI0E_TRACE
    if (vram == 0x0E000000) {
        lod_ni0e_log_slot_event(rdram, "map-swap", pair_index, ni0e_prev_pair_for_log,
                                 even_paddr, odd_paddr);
    }
#endif
#if LOD_FIX_RUN_DL_STALE_NI_FALLBACK
    ni_stale_mark_recent(vram, pair_index);
#endif
#if LOD_ENABLE_NI129_STATE_WRAPPERS
    lod_issue23_log_ni(rdram, "load", pair_index, vram, mapped_vaddr);
    lod_install_issue23_ni129_trace_wrappers(pair_index, vram, "load");
#endif
#if LOD_ENABLE_ISSUE27_PAIR51_TRACE
    lod_install_issue27_pair51_trace_wrappers(pair_index, vram, "load");
#endif
#if LOD_ENABLE_ISSUE27_PAUSE_GATE_TRACE
    lod_install_issue27_pause_trace_wrappers(pair_index, vram, "load");
#endif

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
#if LOD_FIX_NI_PAIR23_INTERNAL_LABELS
    if (pair_index == 23 && vram == 0x0F000000) {
        lod_install_ni_pair23_internal_label_fix("pair23-load");
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
#if LOD_ENABLE_PAUSE_ITEM_TRACE
    lod_install_pause_item_trace_wrappers(pair_index, vram, "0f-load");
#endif
#if LOD_ENABLE_B2_ASSET_TRACE
    if (pair_index == 44 && vram == 0x0F000000 &&
        lod_current_map_overlay_rom() == 0x0076CD00) {
        lod_install_map76_boss_ni44_destroy_trace_wrapper("pair44-load");
    }
#endif
#if LOD_ENABLE_TOWER_NI126_TRACE
    if ((pair_index == 126 || pair_index == 18) && vram == 0x0F000000) {
        lod_install_tower_ni_trace_wrappers(pair_index, vram, "tower-pair-load");
    }
#endif
#if LOD_FIX_PAIR126_INPUT_RELEASE
    if (pair_index == 126 && vram == 0x0F000000) {
        lod_install_pair126_input_release_wrapper(pair_index, vram, "pair126-load");
    }
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

#if LOD_ENABLE_ISSUE27_NI_LOAD_TRACE
static void lod_issue27_log_ni_load_trace(uint8_t* rdram, const char* phase,
                                          uint32_t vaddr, uint32_t even_paddr,
                                          uint32_t odd_paddr, int pair) {
    const uint32_t map_rom = lod_current_map_overlay_rom();
    const int map_count = lod_current_map_overlay_load_count();
    const bool focus_map = map_rom == LOD_ISSUE27_PRE_HANDOFF_MAP_ROM ||
                           map_rom == LOD_ISSUE27_POST_HARPY_MAP_ROM;
    const bool focus_transition_0e = vaddr == 0x0E000000 && map_count >= 2;
    const bool focus_pair129_0f = vaddr == 0x0F000000 && pair == 129 && map_count >= 3;
    if (!focus_map && !focus_transition_0e && !focus_pair129_0f) {
        return;
    }

    static int trace_count = 0;
    static uint32_t last_sig[2] = {0xFFFFFFFF, 0xFFFFFFFF};
    const int slot = (vaddr == 0x0E000000) ? 0 : 1;
    const int loaded_pair = (vaddr == 0x0E000000) ? loaded_0e_pair : loaded_0f_pair;
    const uint32_t sig = ((uint32_t)slot << 28) ^
                         ((uint32_t)(pair + 1) << 16) ^
                         ((uint32_t)(loaded_pair + 1) << 8) ^
                         (map_rom & 0xFF) ^
                         ((uint32_t)map_count << 20);
    const bool changed = sig != last_sig[slot];
    trace_count++;
    if (!changed && trace_count > 120 && (trace_count % 200) != 0) {
        return;
    }
    last_sig[slot] = sig;

    uint32_t words[4] = {};
    if (lod_ni_telemetry_range_ok(even_paddr, sizeof(words))) {
        const uint32_t* p = (const uint32_t*)(rdram + even_paddr);
        words[0] = p[0];
        words[1] = p[1];
        words[2] = p[2];
        words[3] = p[3];
    }

    uint32_t ni_rom = 0;
    uint32_t ni_size = 0;
    if (pair >= 0 && pair < NI_OVL_COUNT) {
        ni_rom = ni_ovl_data[pair].rom_offset;
        ni_size = ni_ovl_data[pair].full_size;
    }

    const char* action = "miss";
    if (pair >= 0) {
        action = (pair == loaded_pair) ? "same" : "swap";
    }

    fprintf(stderr,
            "[ISSUE27_NI_LOAD] #%d phase=%s action=%s map#%d map=0x%08X "
            "vaddr=0x%08X even=0x%08X odd=0x%08X pair=%d prev=%d "
            "loaded0f=%d loaded0e=%d rom=0x%08X size=0x%X "
            "gs=%d exec=0x%08X ni=0x%08X words=%08X,%08X,%08X,%08X\n",
            trace_count, phase, action, map_count, map_rom,
            vaddr, even_paddr, odd_paddr, pair, loaded_pair,
            loaded_0f_pair, loaded_0e_pair, ni_rom, ni_size,
            lod_ni_telemetry_gamestate(rdram),
            lod_ni_telemetry_exec_flags(rdram),
            lod_ni_telemetry_ni_sys_ptr(rdram),
            words[0], words[1], words[2], words[3]);
}
#endif

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
#if LOD_ENABLE_ISSUE27_NI_LOAD_TRACE
        lod_issue27_log_ni_load_trace(rdram, "match", vaddr, even_paddr, odd_paddr, pair);
#endif
#if LOD_ENABLE_NI99_MAP76_TRACE
        if (pair == 99 && vaddr == 0x0F000000 && lod_current_map_overlay_rom() == 0x0076CD00) {
            static int trace_count = 0;
            trace_count++;
            if (trace_count <= 40 || (trace_count % 500) == 0) {
                const int prev_0f = loaded_0f_pair;
                const NiOvlData& data = ni_ovl_data[pair];
                fprintf(stderr,
                        "[NI99_MAP76] #%d map#%d map_rom=0x%08X map_size=0x%X "
                        "vaddr=0x%08X even=0x%08X odd=0x%08X prev0f=%d prev0e=%d "
                        "ni_rom=0x%08X ni_size=0x%X gs=%d exec=0x%08X ni=0x%08X\n",
                        trace_count,
                        lod_current_map_overlay_load_count(),
                        lod_current_map_overlay_rom(),
                        lod_current_map_overlay_size(),
                        vaddr, even_paddr, odd_paddr, prev_0f, loaded_0e_pair,
                        data.rom_offset, data.full_size,
                        lod_ni_telemetry_gamestate(rdram),
                        lod_ni_telemetry_exec_flags(rdram),
                        lod_ni_telemetry_ni_sys_ptr(rdram));
            }
        }
#endif
        load_ni_overlay(rdram, pair, vaddr, even_paddr, odd_paddr);
        return true;
    }

    static int miss_count = 0;
    miss_count++;
#if LOD_ENABLE_ISSUE27_NI_LOAD_TRACE
    lod_issue27_log_ni_load_trace(rdram, "miss", vaddr, even_paddr, odd_paddr, -1);
#endif
#if LOD_ENABLE_NI0E_TRACE
    if (vaddr == 0x0E000000) {
        lod_ni0e_log_slot_event(rdram, "miss", -1, loaded_0e_pair, even_paddr, odd_paddr);
    }
#endif
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
