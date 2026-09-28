#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

#if LOD_ENABLE_NI0E_TRACE
// Round 7: writer/builder probes for the Henry room-entry days-banner text
// struct global at RDRAM 0x8019EECC. func_80145FD4 (below) is the only
// writer of this global found anywhere in RecompiledFuncs: it always clears
// it first, then, when the current map's per-map record (table at
// 0x80192160, 16 bytes/entry, indexed by the map id at sys+0x28D0) has a
// nonzero flag word at its own +0x0, rebuilds it to
// file_ptr_array[record+0xC] + (record+0x0 & 0xFFFFFF), where
// file_ptr_array is sys+0x570 (the NI file pointer table, see
// game-engine-architecture memory notes). See docs/issue27-31-ni0e-findings.md
// Round 7.
#include <stdbool.h>
#include <stdio.h>

extern uint32_t lod_current_map_overlay_rom(void);

static void lod_ni0e_textreg_store_probe(uint32_t value, const char* writer) {
    const uint32_t map_rom = lod_current_map_overlay_rom();
    fprintf(stderr, "[NI0E_TRACE] textreg store=0x%08X writer=%s map_rom=0x%08X\n", value,
            writer, map_rom);
}

static inline bool lod_ni0e_textreg_addr_ok(uint32_t addr) {
    if (addr == 0) {
        return false;
    }
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return phys <= 0x800000u - 0x3Cu;
}

static uint32_t lod_ni0e_textreg_fields_logged = 0;

// Dumps the six fields func_801682D0/func_8016890C consume from the struct
// this global points at: +0x10/+0x14/+0x18 (object pointers), +0x20 (array
// count), +0x24 (object array base), +0x38 (string array base). Capped at
// 40 total logs across the session.
static void lod_ni0e_textreg_fields_probe(uint8_t* rdram, uint32_t ptr, const char* site) {
    if (lod_ni0e_textreg_fields_logged >= 40u) {
        return;
    }
    lod_ni0e_textreg_fields_logged++;
    if (!lod_ni0e_textreg_addr_ok(ptr)) {
        fprintf(stderr, "[NI0E_TRACE] textreg-fields site=%s ptr=0x%08X (invalid, skipped)\n",
                site, ptr);
        return;
    }
    const gpr ptr_gpr = (gpr)(int32_t)ptr;
    const uint32_t f10 = (uint32_t)MEM_W(0x10, ptr_gpr);
    const uint32_t f14 = (uint32_t)MEM_W(0x14, ptr_gpr);
    const uint32_t f18 = (uint32_t)MEM_W(0x18, ptr_gpr);
    const uint32_t f20 = (uint32_t)MEM_W(0x20, ptr_gpr);
    const uint32_t f24 = (uint32_t)MEM_W(0x24, ptr_gpr);
    const uint32_t f38 = (uint32_t)MEM_W(0x38, ptr_gpr);
    fprintf(stderr,
            "[NI0E_TRACE] textreg-fields site=%s ptr=0x%08X f10=0x%08X f14=0x%08X "
            "f18=0x%08X f20=0x%08X f24=0x%08X f38=0x%08X\n",
            site, ptr, f10, f14, f18, f20, f24, f38);
}

// Round 14 (Task A): textreg-resolve probe. Companion to the fileptr-write
// probe in funcs_7.c (func_80011754, tag fileptr-write). Hooked into the
// builder path below (record_flag != 0), right where func_80145FD4 has just
// resolved fileid/slot_value/offset and is about to store struct_ptr to the
// 0x8019EECC global. Logs the current map id (sys+0x28D0), the per-map
// record's fileid (record+0xC), and file_ptr_array[fileid]'s value *at this
// exact moment* -- if the ordering-race hypothesis in
// docs/issue27-31-fix-design.md Round 14 is correct, a Henry-load map
// transition should show slot_value==0 (or a value belonging to a different,
// stale fileid) here while a working session always shows the new map's file
// already resident. Capped at first 40 + every 300th (builder runs are rare
// relative to fileptr writes, but this matches the task brief's cap exactly).
static uint32_t lod_ni0e_textreg_resolve_calls = 0;

static void lod_ni0e_textreg_resolve_probe(uint32_t map_id, uint32_t fileid, uint32_t slot_value,
                                            uint32_t offset, uint32_t struct_ptr) {
    lod_ni0e_textreg_resolve_calls++;
    if (lod_ni0e_textreg_resolve_calls > 40u && (lod_ni0e_textreg_resolve_calls % 300u) != 0u) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] textreg-resolve map_id=0x%04X fileid=0x%02X slot_value=0x%08X "
            "offset=0x%06X struct_ptr=0x%08X call=%u map_rom=0x%08X\n",
            map_id, fileid, slot_value, offset, struct_ptr, lod_ni0e_textreg_resolve_calls,
            lod_current_map_overlay_rom());
}

// Round 8 (continued): no static writer of the days-banner text struct's
// +0x10/+0x14/+0x18 fields was found anywhere in RecompiledFuncs, including
// pair 129/186 and ni_ovl_129_func_0F000938 (see
// docs/issue27-31-ni0e-findings.md Round 8, task 3). `func_8014615C` (below;
// part of the same "bgState" driver/bootstrap/init/load/update_a/update_b/
// create/activate state family as `func_80145FD4` above -- confirmed by the
// identical dispatch-table tail pattern in both functions, and by
// LOD_ENABLE_BGSTATE_TRACE's own "bgState" naming for this exact address
// range in ignored_func_stubs.cpp) creates six specific child objects (ids
// 0x1AC/0x1AD/0x1B2/0x1B1/0x1D0/0x1AE, all `object_createAndSetChild(parent
// = sys+0x2924, id)`) immediately before calling the same func_80001CE8
// finalize helper seen on ni_ovl_129_func_0F0000C8's BUILD path
// (funcs_218.c) -- strongly suggesting these are steps of the same
// activation sequence. None of the six creation results (v0/ctx->r2) are
// stored anywhere in the static code, so this is the strongest lead found
// outside pair 129/186 for whatever eventually lands in the struct's
// object-pointer fields, but it is UNCONFIRMED (object_createAndSetChild's
// own internal linking was not traced this round -- it may simply link into
// the generic obj+0x34..+0x74 child-slot array, unrelated to this global).
// Probe captures the six creation results plus a before/after dump of the
// struct's f10/f14/f18 fields so a live capture can correlate them
// empirically. Capped at 5 calls (~8 log lines/call).
static uint32_t lod_ni0e_bgcreate_calls = 0;

static inline bool lod_ni0e_bgcreate_active(void) {
    return lod_ni0e_bgcreate_calls >= 1 && lod_ni0e_bgcreate_calls <= 5;
}

static void lod_ni0e_bgcreate_dump(uint8_t* rdram, const char* phase, uint32_t obj) {
    if (!lod_ni0e_bgcreate_active()) {
        return;
    }
    const gpr sys_gpr = (gpr)(int32_t)0x801C82C0;
    const uint32_t parent2924 = (uint32_t)MEM_W(0X2924, sys_gpr);
    const gpr global_gpr = (gpr)(int32_t)0x8019EECCu;
    const uint32_t struct_ptr = (uint32_t)MEM_W(0x0, global_gpr);
    uint32_t f10 = 0, f14 = 0, f18 = 0;
    if (lod_ni0e_textreg_addr_ok(struct_ptr)) {
        const gpr ptr_gpr = (gpr)(int32_t)struct_ptr;
        f10 = (uint32_t)MEM_W(0x10, ptr_gpr);
        f14 = (uint32_t)MEM_W(0x14, ptr_gpr);
        f18 = (uint32_t)MEM_W(0x18, ptr_gpr);
    }
    fprintf(stderr,
            "[NI0E_TRACE] daypop bgcreate-%s #%u obj=0x%08X parent2924=0x%08X "
            "struct_ptr=0x%08X f10=0x%08X f14=0x%08X f18=0x%08X\n",
            phase, lod_ni0e_bgcreate_calls, obj, parent2924, struct_ptr, f10, f14, f18);
}

static void lod_ni0e_bgcreate_enter(uint8_t* rdram, uint32_t obj) {
    lod_ni0e_bgcreate_calls++;
    lod_ni0e_bgcreate_dump(rdram, "enter", obj);
}

static void lod_ni0e_bgcreate_child(uint32_t id, uint32_t result) {
    if (!lod_ni0e_bgcreate_active()) {
        return;
    }
    fprintf(stderr, "[NI0E_TRACE] daypop bgcreate-child #%u id=0x%03X result=0x%08X\n",
            lod_ni0e_bgcreate_calls, id, result);
}

static void lod_ni0e_bgcreate_exit(uint8_t* rdram, uint32_t obj) {
    lod_ni0e_bgcreate_dump(rdram, "exit", obj);
}

// Round 13: bgState_activate's AND-gate entry probe (see
// docs/issue27-31-fix-design.md section 4 and
// docs/issue27-31-ni0e-findings.md Round 13). bgState_activate (below, vaddr
// 0x80146240) is the single consumer of the "activation control block" at
// 0x8019D180: it sets sys+0x2908 |= 0x10000000 only when BOTH
// *(0x8019D194) (+0x14) and *(0x8019D198) (+0x18) are nonzero. Reading the
// function in full confirms there is no third comparand -- it is exactly
// this two-word AND-gate, nothing else guards the exec-flags write. Logs
// both gate words, the current exec flags (sys+0x2908), map rom, and
// whether the gate fires this call (predicted the same way the function's
// own two branches decide it). Rate: first 40 calls unconditionally, then
// only when the (gate1, gate2, fired) tuple changes from the last logged
// value.
static uint32_t lod_ni0e_actgate_calls = 0;
static uint32_t lod_ni0e_actgate_last_gate1 = 0xFFFFFFFFu;
static uint32_t lod_ni0e_actgate_last_gate2 = 0xFFFFFFFFu;
static int lod_ni0e_actgate_last_fired = -1;
static bool lod_ni0e_actgate_have_last = false;

static void lod_ni0e_activation_gate_probe(uint8_t* rdram, uint32_t obj) {
    lod_ni0e_actgate_calls++;
    const gpr block_gpr = (gpr)(int32_t)0x8019D180u;
    const uint32_t gate1 = (uint32_t)MEM_W(0x14, block_gpr);  // 0x8019D194
    const uint32_t gate2 = (uint32_t)MEM_W(0x18, block_gpr);  // 0x8019D198
    const gpr sys_gpr = (gpr)(int32_t)0x801C82C0u;
    const uint32_t exec_flags = (uint32_t)MEM_W(0x2908, sys_gpr);
    const int fired = (gate1 != 0 && gate2 != 0) ? 1 : 0;
    const uint32_t map_rom = lod_current_map_overlay_rom();

    const bool changed = !lod_ni0e_actgate_have_last || gate1 != lod_ni0e_actgate_last_gate1 ||
                          gate2 != lod_ni0e_actgate_last_gate2 || fired != lod_ni0e_actgate_last_fired;
    if (lod_ni0e_actgate_calls > 40u && !changed) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] activation-gate #%u obj=0x%08X gate1=0x%08X gate2=0x%08X "
            "exec=0x%08X fired=%d map_rom=0x%08X\n",
            lod_ni0e_actgate_calls, obj, gate1, gate2, exec_flags, fired, map_rom);
    lod_ni0e_actgate_last_gate1 = gate1;
    lod_ni0e_actgate_last_gate2 = gate2;
    lod_ni0e_actgate_last_fired = fired;
    lod_ni0e_actgate_have_last = true;
}

// Round 15 (Task B): bgState dispatch probe. Locates the function-pointer
// table that dispatches func_80145FD4 (the days-banner text builder,
// "bgState update_a" per LOD_ENABLE_BGSTATE_TRACE's own naming). Confirmed
// by a direct ROM read (resources/castlevania2_decompressed.z64, using the
// "common" segment's file<->vram mapping from castlevania2.yaml: file 0xC2120
// = vram 0x80141870): the shared table at RDRAM 0x8018D3B0 -- already named
// DISPATCH_TABLE_PHYS in ignored_func_stubs.cpp's pre-existing
// LOD_ENABLE_BGSTATE_TRACE infra, and mapped to owning object id 0x1AB by
// that same file's lod_dispatch_table_for_obj_id -- holds, in order:
//   [0]=0x80145B60 bootstrap  [1]=0x80145BD0 init     [2]=0x80145DA8 load
//   [3]=0x80145FD4 update_a  <-- the text builder, state index 3
//   [4]=0x801460D4 update_b  [5]=0x8014615C create    [6]=0x80146240 activate
//   [7]=0x801462A8 (unnamed, next state after activate)
//   [8..11] then jump to unrelated fixed addresses (0x80198xxx) and
//   [12..] to per-map-overlay-relative addresses (0x802Exxxx/0x802Fxxxx,
//   inside the "map_ovl_*" vram window every map overlay shares) -- states
//   8+ belong to a different, map-specific part of the same machine and are
//   not traced by this probe.
// The dispatch itself is inlined identically at the tail of six of the eight
// named functions (driver/bootstrap/init/update_a/update_b/create -- load
// and activate do not re-dispatch, confirmed by grepping this file for the
// table's `-0x2C50` load, which appears in exactly six functions): each
// reads the object's re-entrancy depth (obj+0xE, a signed halfword --
// func_80145AF0/driver is the only one of the six that increments it before
// reading; the other five re-dispatch at the SAME depth their caller is
// already at, i.e. they chain directly into the next state within one
// driver invocation), indexes a per-depth (count, state) byte pair stored at
// obj + depth*2 + (8, 9), bumps the count byte, and jalr's through
// table[state]. Hooked at all six call sites below, immediately before each
// site's own `-0x2C50` table read resolves the final jalr target (so it
// never touches ctx/obj/control flow), with a `site` tag identifying which
// of the six functions is dispatching.
//
// (a) tag `bgstate`: every dispatch's (obj, depth, state, count), rate-
// limited to the first 60 + every 300th (shared counter across all six
// sites), per the task brief.
// (b) tag `bgstate-trans`: unconditional (uncapped -- transitions are rare
// by construction, since most dispatches simply re-select the same state
// they were already in) log of every time the *state* byte actually changes
// from the last dispatch seen for the same object, tracked as a single
// (obj, state) pair since object id 0x1AB is the singleton root of this
// family (docs/issue27-31-fix-design.md section 4.1) -- a change of obj
// itself (first sighting, or a different bgState-family instance) is also
// logged as a transition from "none".
static uint32_t lod_ni0e_bgstate_dispatch_calls = 0;
static uint32_t lod_ni0e_bgstate_last_obj = 0;
static int lod_ni0e_bgstate_last_state = -1;
static bool lod_ni0e_bgstate_have_last = false;

// obj itself is range-checked by the caller (lod_ni0e_textreg_addr_ok, fixed
// small size); depth is a signed 16-bit field so the pair offset is bounded
// to roughly +-64K, but obj could still sit close to the top of the
// emulated 8MB span, so the actual pair-byte address is checked separately
// here before it is dereferenced.
static inline bool lod_ni0e_bgstate_pair_ok(uint32_t obj, int32_t pair_off) {
    const uint32_t phys = obj & 0x1FFFFFFFu;
    const int64_t end = (int64_t)phys + pair_off + 0x9;
    return end >= 0 && end < 0x800000;
}

static void lod_ni0e_bgstate_dispatch_probe(uint8_t* rdram, uint32_t obj, const char* site) {
    lod_ni0e_bgstate_dispatch_calls++;
    const bool verbose = lod_ni0e_bgstate_dispatch_calls <= 60u ||
                          (lod_ni0e_bgstate_dispatch_calls % 300u) == 0u;

    if (!lod_ni0e_textreg_addr_ok(obj)) {
        if (verbose) {
            fprintf(stderr,
                    "[NI0E_TRACE] bgstate #%u site=%s obj=0x%08X (invalid, skipped)\n",
                    lod_ni0e_bgstate_dispatch_calls, site, obj);
        }
        return;
    }
    const gpr obj_gpr = (gpr)(int32_t)obj;
    const int16_t depth = (int16_t)MEM_H(0xE, obj_gpr);
    const int32_t pair_off = (int32_t)depth * 2;
    if (!lod_ni0e_bgstate_pair_ok(obj, pair_off)) {
        if (verbose) {
            fprintf(stderr,
                    "[NI0E_TRACE] bgstate #%u site=%s obj=0x%08X depth=%d (pair out of "
                    "range, skipped)\n",
                    lod_ni0e_bgstate_dispatch_calls, site, obj, (int)depth);
        }
        return;
    }
    const uint8_t state = (uint8_t)MEM_BU(pair_off + 0x9, obj_gpr);
    const uint8_t count = (uint8_t)MEM_BU(pair_off + 0x8, obj_gpr);

    if (verbose) {
        fprintf(stderr,
                "[NI0E_TRACE] bgstate #%u site=%s obj=0x%08X depth=%d state=%u count=%u "
                "map_rom=0x%08X\n",
                lod_ni0e_bgstate_dispatch_calls, site, obj, (int)depth, state, count,
                lod_current_map_overlay_rom());
    }

    const bool transitioned = !lod_ni0e_bgstate_have_last || obj != lod_ni0e_bgstate_last_obj ||
                               (int)state != lod_ni0e_bgstate_last_state;
    if (transitioned) {
        fprintf(stderr,
                "[NI0E_TRACE] bgstate-trans #%u site=%s obj=0x%08X depth=%d %d -> %u "
                "map_rom=0x%08X\n",
                lod_ni0e_bgstate_dispatch_calls, site, obj, (int)depth,
                lod_ni0e_bgstate_have_last ? lod_ni0e_bgstate_last_state : -1, state,
                lod_current_map_overlay_rom());
    }
    lod_ni0e_bgstate_last_obj = obj;
    lod_ni0e_bgstate_last_state = (int)state;
    lod_ni0e_bgstate_have_last = true;
}

// Round 16 (Task A): update_a (func_80145FD4) branch-decision probe. A full
// static read of the function (see docs/issue27-31-ni0e-findings.md Round
// 16) found exactly two early-outs between entry and (a) the textreg-resolve
// probe site and (b) the unconditional state-advance call:
//   gate1, vaddr 0x80145FE8 (`beql $t6,$zero,L_801460C8`): sys+0x2B28 (RDRAM
//   0x801CADE8 -- no static writer found anywhere in RecompiledFuncs/*.c, a
//   field not previously documented in this investigation) == 0 -> FULL
//   early return. Nothing else in the function runs at all: no map-id/
//   record lookup, no textreg store/resolve, and -- critically -- no call to
//   object_curLevel_goToNextFuncAndClearTimer (func_80001CE8,
//   funcs_0.c:4589, confirmed by full read to have no internal gating
//   whatsoever: it unconditionally increments the combined (count<<8|state)
//   halfword at obj+depth*2+8 by 1, which is the actual 3->4 state-advance
//   mechanism -- the dispatch tail's own `table[state]` read at the very end
//   of func_80145FD4 only re-selects whichever state that increment just
//   produced, it does not itself decide the transition). Gate1 firing is
//   therefore sufficient by itself to explain BOTH "no textreg-resolve" AND
//   "state never advances 3->4" in the same capture.
//   gate2, vaddr 0x80146028 (`beq $v1,$zero,L_80146070`): the per-map
//   record's flag word (0x80192160 + map_id*16, +0x0) == 0 -> skips ONLY the
//   struct_ptr builder store and the textreg-resolve probe, falling through
//   to the shared tail at L_80146070 -- which still runs the state-advance
//   call unconditionally. Gate2 firing alone would NOT explain a stalled
//   3->4 transition.
// Since the round-16 capture shows update_a reaching NEITHER the
// textreg-resolve probe NOR ever advancing 3->4 on the new map, gate1 is the
// prime suspect. Logged at three points so the next capture can confirm:
// gate1's decision (tag "gate1"), gate2's decision (tag "gate2", only
// reached when gate1 passed), and immediately before the state-advance call
// itself (tag "advance", reached whenever gate1 passed, regardless of
// gate2), dumping the (count,state) pair BEFORE the increment so a capture
// can see exactly which state is (or is not) being left behind. Rate-limited
// per tag: first 20 + every 600th, per the task brief.
static uint32_t lod_ni0e_updatea_gate1_calls = 0;
static uint32_t lod_ni0e_updatea_gate2_calls = 0;
static uint32_t lod_ni0e_updatea_advance_calls = 0;

static inline bool lod_ni0e_updatea_rate_ok(uint32_t n) {
    return n <= 20u || (n % 600u) == 0u;
}

static void lod_ni0e_updatea_gate1_probe(uint32_t obj, uint32_t sys2b28, int skipped) {
    lod_ni0e_updatea_gate1_calls++;
    if (!lod_ni0e_updatea_rate_ok(lod_ni0e_updatea_gate1_calls)) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] update_a-branch #%u tag=gate1 obj=0x%08X sys2B28=0x%08X %s "
            "map_rom=0x%08X\n",
            lod_ni0e_updatea_gate1_calls, obj, sys2b28,
            skipped ? "SKIP(busy, full-return)" : "pass", lod_current_map_overlay_rom());
}

static void lod_ni0e_updatea_gate2_probe(uint32_t obj, uint32_t map_id, uint32_t record_ptr,
                                          uint32_t record_flag, int skipped) {
    lod_ni0e_updatea_gate2_calls++;
    if (!lod_ni0e_updatea_rate_ok(lod_ni0e_updatea_gate2_calls)) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] update_a-branch #%u tag=gate2 obj=0x%08X map_id=0x%04X "
            "record_ptr=0x%08X record_flag=0x%08X %s map_rom=0x%08X\n",
            lod_ni0e_updatea_gate2_calls, obj, map_id, record_ptr, record_flag,
            skipped ? "SKIP(record-empty, no textreg-resolve)" : "pass",
            lod_current_map_overlay_rom());
}

static void lod_ni0e_updatea_advance_probe(uint8_t* rdram, uint32_t obj) {
    lod_ni0e_updatea_advance_calls++;
    if (!lod_ni0e_updatea_rate_ok(lod_ni0e_updatea_advance_calls)) {
        return;
    }
    if (!lod_ni0e_textreg_addr_ok(obj)) {
        fprintf(stderr, "[NI0E_TRACE] update_a-branch #%u tag=advance obj=0x%08X (invalid, skipped)\n",
                lod_ni0e_updatea_advance_calls, obj);
        return;
    }
    const gpr obj_gpr = (gpr)(int32_t)obj;
    const int16_t depth = (int16_t)MEM_H(0xE, obj_gpr);
    const int32_t pair_off = (int32_t)depth * 2;
    if (!lod_ni0e_bgstate_pair_ok(obj, pair_off)) {
        fprintf(stderr,
                "[NI0E_TRACE] update_a-branch #%u tag=advance obj=0x%08X depth=%d (pair out "
                "of range)\n",
                lod_ni0e_updatea_advance_calls, obj, (int)depth);
        return;
    }
    const uint8_t state_before = (uint8_t)MEM_BU(pair_off + 0x9, obj_gpr);
    const uint8_t count_before = (uint8_t)MEM_BU(pair_off + 0x8, obj_gpr);
    fprintf(stderr,
            "[NI0E_TRACE] update_a-branch #%u tag=advance obj=0x%08X depth=%d "
            "state_before=%u count_before=%u map_rom=0x%08X\n",
            lod_ni0e_updatea_advance_calls, obj, (int)depth, state_before, count_before,
            lod_current_map_overlay_rom());
}

// Round 17 (Task A): branch map of func_80145DA8, the bgState "load" state
// (dispatch index 2). Read in full: unlike update_a's gate1, NEITHER of this
// function's two early-out branches is a full early return -- both only
// skip part of the per-id/per-map alloc work, and the function always falls
// through to an UNCONDITIONAL call into func_800119CC (funcs_7.c) with
// a3 = &(sys+0x2B28) (RDRAM 0x801CADE8, the exact field update_a's gate1
// blocks on) and a1 = the fixed id-array scratch at RDRAM 0x8019D1A8. Tracing
// func_800119CC -> func_80011754 -> func_800116BC (funcs_7.c) shows THAT
// chain is the real (computed-pointer) writer: func_800116BC zeroes
// *(sys+0x2B28) synchronously when it queues an async decompress/DMA
// request, then some not-yet-traced completion handler is expected to write
// the real heap pointer back into it later -- never a literal "0x2B28"
// immediate anywhere, matching round 16's grep blind spot exactly. Three
// sites, tag "load_state-branch", same first-20 + every-600 rate limit as
// update_a-branch (own per-site counters).
static uint32_t lod_ni0e_loadstate_identry_calls = 0;
static uint32_t lod_ni0e_loadstate_record2_calls = 0;
static uint32_t lod_ni0e_loadstate_queue_calls = 0;

static inline bool lod_ni0e_loadstate_rate_ok(uint32_t n) {
    return n <= 20u || (n % 600u) == 0u;
}

// Site "id-entry" (0x80145E34): the per-map record's byteA/byteB fields
// (0x8018E6A8 + map_id*16, +0xA/+0xB) are only queued for allocation
// (func_80001514 alloc, id-array push, sys+id*4+0x570 clear) when nonzero;
// a zero entry skips straight to the loop-continue. s1_ptr is the loop
// cursor (sp+0x74/0x78/0x7C) so a capture can tell which of the 3 slots this
// is; the first slot (map_id itself) never reaches this branch at all (it is
// unconditionally processed), so only byteA/byteB ever log "skipped".
static void lod_ni0e_loadstate_identry_probe(uint32_t s1_ptr, uint32_t entry_value, int skipped) {
    lod_ni0e_loadstate_identry_calls++;
    if (!lod_ni0e_loadstate_rate_ok(lod_ni0e_loadstate_identry_calls)) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] load_state-branch #%u tag=id-entry cursor=0x%08X entry=0x%08X %s "
            "map_rom=0x%08X\n",
            lod_ni0e_loadstate_identry_calls, s1_ptr, entry_value,
            skipped ? "SKIP(zero, no alloc/queue)" : "pass", lod_current_map_overlay_rom());
}

// Site "record2-gate" (0x80145EEC): the per-map record2 (0x80192160 +
// map_id*16 -- the SAME table update_a's own gate2 reads, round 16) gates
// the "main map" allocation block (a second func_80001514 alloc stored at
// the fixed global 0x8019D1A4, plus another id-array push/0x570 clear) on
// its own +0x0 word being nonzero.
static void lod_ni0e_loadstate_record2_probe(uint32_t map_id, uint32_t record2_ptr,
                                              uint32_t record2_flag, int skipped) {
    lod_ni0e_loadstate_record2_calls++;
    if (!lod_ni0e_loadstate_rate_ok(lod_ni0e_loadstate_record2_calls)) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] load_state-branch #%u tag=record2-gate map_id=0x%04X "
            "record2_ptr=0x%08X record2_flag=0x%08X %s map_rom=0x%08X\n",
            lod_ni0e_loadstate_record2_calls, map_id, record2_ptr, record2_flag,
            skipped ? "SKIP(record2-empty, no main-map alloc)" : "pass",
            lod_current_map_overlay_rom());
}

// Site "queue-call" (0x80145F84, immediately before the unconditional call
// into func_800119CC): this call always happens regardless of the two
// branches above -- what varies is how many valid entries the id-array
// (RDRAM 0x8019D1A8, a fixed scratch buffer NEVER reset per-call) holds for
// func_800119CC's internal scan to find. id_count is the number of entries
// this call actually pushed (read from $s5+1 at the call site); 0 means
// neither byteA/byteB nor record2 contributed anything this call (only the
// always-processed map_id slot, if it too was somehow a duplicate/no-op).
static void lod_ni0e_loadstate_queue_probe(uint32_t obj, uint32_t id_count) {
    lod_ni0e_loadstate_queue_calls++;
    if (!lod_ni0e_loadstate_rate_ok(lod_ni0e_loadstate_queue_calls)) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] load_state-branch #%u tag=queue-call obj=0x%08X id_count=%u "
            "map_rom=0x%08X\n",
            lod_ni0e_loadstate_queue_calls, obj, id_count, lod_current_map_overlay_rom());
}
#endif  // LOD_ENABLE_NI0E_TRACE

RECOMP_FUNC void func_80143E84(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143E84: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143E88: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143E8C: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80143E90: lhu         $t6, 0x18($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X18);
    // 0x80143E94: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x80143E98: bnel        $t7, $zero, L_80143ED4
    if (ctx->r15 != 0) {
        // 0x80143E9C: lh          $v1, 0xE($a0)
        ctx->r3 = MEM_H(ctx->r4, 0XE);
            goto L_80143ED4;
    }
    goto skip_0;
    // 0x80143E9C: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    skip_0:
    // 0x80143EA0: lw          $a1, 0x4($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X4);
    // 0x80143EA4: lw          $a2, 0x8($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X8);
    // 0x80143EA8: lw          $a3, 0xC($v0)
    ctx->r7 = MEM_W(ctx->r2, 0XC);
    // 0x80143EAC: jal         0x80145198
    // 0x80143EB0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80145198(rdram, ctx);
        goto after_0;
    // 0x80143EB0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80143EB4: beq         $v0, $zero, L_80143ED0
    if (ctx->r2 == 0) {
        // 0x80143EB8: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_80143ED0;
    }
    // 0x80143EB8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80143EBC: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80143EC0: jalr        $t9
    // 0x80143EC4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80143EC4: nop

    after_1:
    // 0x80143EC8: b           L_80143F2C
    // 0x80143ECC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80143F2C;
    // 0x80143ECC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80143ED0:
    // 0x80143ED0: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
L_80143ED4:
    // 0x80143ED4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80143ED8: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80143EDC: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80143EE0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80143EE4: sll         $t8, $v1, 1
    ctx->r24 = S32(ctx->r3 << 1);
    // 0x80143EE8: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x80143EEC: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80143EF0: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80143EF4: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80143EF8: sb          $t1, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r9;
    // 0x80143EFC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80143F00: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80143F04: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80143F08: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80143F0C: lw          $t9, -0x2F2C($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2F2C);
    // 0x80143F10: jalr        $t9
    // 0x80143F14: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80143F14: nop

    after_2:
    // 0x80143F18: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80143F1C: lh          $t4, 0xE($a0)
    ctx->r12 = MEM_H(ctx->r4, 0XE);
    // 0x80143F20: addiu       $t5, $t4, -0x1
    ctx->r13 = ADD32(ctx->r12, -0X1);
    // 0x80143F24: sh          $t5, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r13;
    // 0x80143F28: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80143F2C:
    // 0x80143F2C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143F30: jr          $ra
    // 0x80143F34: nop

    return;
    // 0x80143F34: nop

;}
RECOMP_FUNC void func_80143F38(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143F38: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x80143F3C: lw          $t7, -0x5170($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X5170);
    // 0x80143F40: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80143F44: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80143F48: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80143F4C: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x80143F50: andi        $t8, $t7, 0x1
    ctx->r24 = ctx->r15 & 0X1;
    // 0x80143F54: lw          $s0, 0x40($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X40);
    // 0x80143F58: lw          $a1, 0x70($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X70);
    // 0x80143F5C: bne         $t8, $zero, L_801440A8
    if (ctx->r24 != 0) {
        // 0x80143F60: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_801440A8;
    }
    // 0x80143F60: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80143F64: lhu         $t9, 0x0($a1)
    ctx->r25 = MEM_HU(ctx->r5, 0X0);
    // 0x80143F68: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80143F6C: andi        $t0, $t9, 0x4
    ctx->r8 = ctx->r25 & 0X4;
    // 0x80143F70: bnel        $t0, $zero, L_80143F94
    if (ctx->r8 != 0) {
        // 0x80143F74: lui         $at, 0x3F80
        ctx->r1 = S32(0X3F80 << 16);
            goto L_80143F94;
    }
    goto skip_0;
    // 0x80143F74: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    skip_0:
    // 0x80143F78: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x80143F7C: jal         0x801454DC
    // 0x80143F80: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    func_801454DC(rdram, ctx);
        goto after_0;
    // 0x80143F80: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    after_0:
    // 0x80143F84: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x80143F88: beq         $v0, $zero, L_801440A8
    if (ctx->r2 == 0) {
        // 0x80143F8C: lw          $a1, 0x3C($sp)
        ctx->r5 = MEM_W(ctx->r29, 0X3C);
            goto L_801440A8;
    }
    // 0x80143F8C: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    // 0x80143F90: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
L_80143F94:
    // 0x80143F94: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x80143F98: lwc1        $f4, 0x4($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80143F9C: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80143FA0: lh          $t1, 0xE($s0)
    ctx->r9 = MEM_H(ctx->r16, 0XE);
    // 0x80143FA4: mul.s       $f12, $f4, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80143FA8: lwc1        $f8, 0xC($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0XC);
    // 0x80143FAC: mtc1        $t1, $f10
    ctx->f10.u32l = ctx->r9;
    // 0x80143FB0: mul.s       $f14, $f6, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x80143FB4: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x80143FB8: mul.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x80143FBC: cvt.s.w     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80143FC0: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80143FC4: mfc1        $a3, $f10
    ctx->r7 = (int32_t)ctx->f10.u32l;
    // 0x80143FC8: jal         0x80145358
    // 0x80143FCC: nop

    func_80145358(rdram, ctx);
        goto after_1;
    // 0x80143FCC: nop

    after_1:
    // 0x80143FD0: beq         $v0, $zero, L_801440A8
    if (ctx->r2 == 0) {
        // 0x80143FD4: lw          $v1, 0x34($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X34);
            goto L_801440A8;
    }
    // 0x80143FD4: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x80143FD8: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80143FDC: bnel        $v0, $zero, L_80143FF0
    if (ctx->r2 != 0) {
        // 0x80143FE0: lw          $a0, 0x4($s0)
        ctx->r4 = MEM_W(ctx->r16, 0X4);
            goto L_80143FF0;
    }
    goto skip_1;
    // 0x80143FE0: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
    skip_1:
    // 0x80143FE4: b           L_80144004
    // 0x80143FE8: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_80144004;
    // 0x80143FE8: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80143FEC: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
L_80143FF0:
    // 0x80143FF0: jalr        $v0
    // 0x80143FF4: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_2;
    // 0x80143FF4: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    after_2:
    // 0x80143FF8: beq         $v0, $zero, L_80144004
    if (ctx->r2 == 0) {
        // 0x80143FFC: lw          $v1, 0x34($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X34);
            goto L_80144004;
    }
    // 0x80143FFC: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x80144000: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_80144004:
    // 0x80144004: beq         $v1, $zero, L_801440A8
    if (ctx->r3 == 0) {
        // 0x80144008: lw          $a2, 0x48($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X48);
            goto L_801440A8;
    }
    // 0x80144008: lw          $a2, 0x48($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X48);
    // 0x8014400C: lh          $t2, 0x12($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X12);
    // 0x80144010: addiu       $v1, $a2, 0x34
    ctx->r3 = ADD32(ctx->r6, 0X34);
    // 0x80144014: sw          $zero, 0x18($v1)
    MEM_W(0X18, ctx->r3) = 0;
    // 0x80144018: sw          $t2, 0x1C($v1)
    MEM_W(0X1C, ctx->r3) = ctx->r10;
    // 0x8014401C: lh          $t3, 0x10($s0)
    ctx->r11 = MEM_H(ctx->r16, 0X10);
    // 0x80144020: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80144024: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144028: sh          $t3, 0x20($v1)
    MEM_H(0X20, ctx->r3) = ctx->r11;
    // 0x8014402C: lh          $v0, 0xC($s0)
    ctx->r2 = MEM_H(ctx->r16, 0XC);
    // 0x80144030: addiu       $t9, $t9, 0x47C0
    ctx->r25 = ADD32(ctx->r25, 0X47C0);
    // 0x80144034: bne         $v0, $at, L_80144044
    if (ctx->r2 != ctx->r1) {
        // 0x80144038: sll         $a0, $v0, 2
        ctx->r4 = S32(ctx->r2 << 2);
            goto L_80144044;
    }
    // 0x80144038: sll         $a0, $v0, 2
    ctx->r4 = S32(ctx->r2 << 2);
    // 0x8014403C: b           L_80144094
    // 0x80144040: sh          $zero, 0x16($v1)
    MEM_H(0X16, ctx->r3) = 0;
        goto L_80144094;
    // 0x80144040: sh          $zero, 0x16($v1)
    MEM_H(0X16, ctx->r3) = 0;
L_80144044:
    // 0x80144044: addu        $a0, $a0, $v0
    ctx->r4 = ADD32(ctx->r4, ctx->r2);
    // 0x80144048: sll         $a0, $a0, 1
    ctx->r4 = S32(ctx->r4 << 1);
    // 0x8014404C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x80144050: jalr        $t9
    // 0x80144054: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80144054: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    after_3:
    // 0x80144058: lh          $t4, 0xC($s0)
    ctx->r12 = MEM_H(ctx->r16, 0XC);
    // 0x8014405C: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x80144060: div         $zero, $v0, $t4
    lo = S32(S64(S32(ctx->r2)) / S64(S32(ctx->r12))); hi = S32(S64(S32(ctx->r2)) % S64(S32(ctx->r12)));
    // 0x80144064: mfhi        $t5
    ctx->r13 = hi;
    // 0x80144068: sh          $t5, 0x16($v1)
    MEM_H(0X16, ctx->r3) = ctx->r13;
    // 0x8014406C: lw          $a2, 0x48($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X48);
    // 0x80144070: bne         $t4, $zero, L_8014407C
    if (ctx->r12 != 0) {
        // 0x80144074: nop
    
            goto L_8014407C;
    }
    // 0x80144074: nop

    // 0x80144078: break       7
    do_break(2148810872);
L_8014407C:
    // 0x8014407C: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80144080: bne         $t4, $at, L_80144094
    if (ctx->r12 != ctx->r1) {
        // 0x80144084: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80144094;
    }
    // 0x80144084: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80144088: bne         $v0, $at, L_80144094
    if (ctx->r2 != ctx->r1) {
        // 0x8014408C: nop
    
            goto L_80144094;
    }
    // 0x8014408C: nop

    // 0x80144090: break       6
    do_break(2148810896);
L_80144094:
    // 0x80144094: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144098: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8014409C: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801440A0: jalr        $t9
    // 0x801440A4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801440A4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_4:
L_801440A8:
    // 0x801440A8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801440AC: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801440B0: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x801440B4: jr          $ra
    // 0x801440B8: nop

    return;
    // 0x801440B8: nop

;}
RECOMP_FUNC void func_801440BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801440BC: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x801440C0: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801440C4: lw          $t6, -0x5170($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5170);
    // 0x801440C8: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x801440CC: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x801440D0: sw          $s1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r17;
    // 0x801440D4: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x801440D8: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x801440DC: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x801440E0: lw          $s2, 0x40($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X40);
    // 0x801440E4: beq         $t7, $zero, L_8014410C
    if (ctx->r15 == 0) {
        // 0x801440E8: lw          $s0, 0x70($a0)
        ctx->r16 = MEM_W(ctx->r4, 0X70);
            goto L_8014410C;
    }
    // 0x801440E8: lw          $s0, 0x70($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X70);
    // 0x801440EC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801440F0: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801440F4: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801440F8: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x801440FC: jalr        $t9
    // 0x80144100: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80144100: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80144104: b           L_80144394
    // 0x80144108: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
        goto L_80144394;
    // 0x80144108: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
L_8014410C:
    // 0x8014410C: addiu       $s1, $a3, 0x34
    ctx->r17 = ADD32(ctx->r7, 0X34);
    // 0x80144110: lw          $t8, 0x1C($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X1C);
    // 0x80144114: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80144118: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x8014411C: addiu       $t0, $t8, -0x1
    ctx->r8 = ADD32(ctx->r24, -0X1);
    // 0x80144120: bgez        $t0, L_80144140
    if (SIGNED(ctx->r8) >= 0) {
        // 0x80144124: sw          $t0, 0x1C($s1)
        MEM_W(0X1C, ctx->r17) = ctx->r8;
            goto L_80144140;
    }
    // 0x80144124: sw          $t0, 0x1C($s1)
    MEM_W(0X1C, ctx->r17) = ctx->r8;
    // 0x80144128: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8014412C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80144130: jalr        $t9
    // 0x80144134: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80144134: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_1:
    // 0x80144138: b           L_80144394
    // 0x8014413C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
        goto L_80144394;
    // 0x8014413C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
L_80144140:
    // 0x80144140: lw          $t2, 0x18($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X18);
    // 0x80144144: lh          $t4, 0x20($s1)
    ctx->r12 = MEM_H(ctx->r17, 0X20);
    // 0x80144148: lui         $t6, 0x8010
    ctx->r14 = S32(0X8010 << 16);
    // 0x8014414C: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80144150: slt         $at, $t4, $t3
    ctx->r1 = SIGNED(ctx->r12) < SIGNED(ctx->r11) ? 1 : 0;
    // 0x80144154: beq         $at, $zero, L_80144390
    if (ctx->r1 == 0) {
        // 0x80144158: sw          $t3, 0x18($s1)
        MEM_W(0X18, ctx->r17) = ctx->r11;
            goto L_80144390;
    }
    // 0x80144158: sw          $t3, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->r11;
    // 0x8014415C: sw          $zero, 0x18($s1)
    MEM_W(0X18, ctx->r17) = 0;
    // 0x80144160: lw          $t6, -0x6EE0($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X6EE0);
    // 0x80144164: lh          $t7, 0x24($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X24);
    // 0x80144168: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x8014416C: slt         $at, $t6, $t7
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x80144170: beql        $at, $zero, L_80144394
    if (ctx->r1 == 0) {
        // 0x80144174: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80144394;
    }
    goto skip_0;
    // 0x80144174: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_0:
    // 0x80144178: jal         0x80145420
    // 0x8014417C: sw          $a3, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r7;
    func_80145420(rdram, ctx);
        goto after_2;
    // 0x8014417C: sw          $a3, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r7;
    after_2:
    // 0x80144180: lh          $t8, 0x22($s1)
    ctx->r24 = MEM_H(ctx->r17, 0X22);
    // 0x80144184: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80144188: slt         $at, $v0, $t8
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r24) ? 1 : 0;
    // 0x8014418C: beql        $at, $zero, L_80144394
    if (ctx->r1 == 0) {
        // 0x80144190: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80144394;
    }
    goto skip_1;
    // 0x80144190: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_1:
    // 0x80144194: lhu         $t0, 0x0($s0)
    ctx->r8 = MEM_HU(ctx->r16, 0X0);
    // 0x80144198: andi        $t1, $t0, 0x4
    ctx->r9 = ctx->r8 & 0X4;
    // 0x8014419C: beq         $t1, $zero, L_801441BC
    if (ctx->r9 == 0) {
        // 0x801441A0: nop
    
            goto L_801441BC;
    }
    // 0x801441A0: nop

    // 0x801441A4: jal         0x8014555C
    // 0x801441A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8014555C(rdram, ctx);
        goto after_3;
    // 0x801441A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x801441AC: bnel        $v0, $zero, L_801441D0
    if (ctx->r2 != 0) {
        // 0x801441B0: lhu         $t2, 0x16($s1)
        ctx->r10 = MEM_HU(ctx->r17, 0X16);
            goto L_801441D0;
    }
    goto skip_2;
    // 0x801441B0: lhu         $t2, 0x16($s1)
    ctx->r10 = MEM_HU(ctx->r17, 0X16);
    skip_2:
    // 0x801441B4: b           L_80144394
    // 0x801441B8: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
        goto L_80144394;
    // 0x801441B8: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
L_801441BC:
    // 0x801441BC: jal         0x801454DC
    // 0x801441C0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_801454DC(rdram, ctx);
        goto after_4;
    // 0x801441C0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_4:
    // 0x801441C4: beql        $v0, $zero, L_80144394
    if (ctx->r2 == 0) {
        // 0x801441C8: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80144394;
    }
    goto skip_3;
    // 0x801441C8: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_3:
    // 0x801441CC: lhu         $t2, 0x16($s1)
    ctx->r10 = MEM_HU(ctx->r17, 0X16);
L_801441D0:
    // 0x801441D0: lw          $t9, 0x8($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X8);
    // 0x801441D4: lui         $at, 0x3FFF
    ctx->r1 = S32(0X3FFF << 16);
    // 0x801441D8: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801441DC: subu        $t3, $t3, $t2
    ctx->r11 = SUB32(ctx->r11, ctx->r10);
    // 0x801441E0: sll         $t3, $t3, 3
    ctx->r11 = S32(ctx->r11 << 3);
    // 0x801441E4: addu        $s0, $t9, $t3
    ctx->r16 = ADD32(ctx->r25, ctx->r11);
    // 0x801441E8: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x801441EC: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801441F0: sll         $t4, $v0, 0
    ctx->r12 = S32(ctx->r2 << 0);
    // 0x801441F4: bgez        $t4, L_8014422C
    if (SIGNED(ctx->r12) >= 0) {
        // 0x801441F8: and         $v1, $v0, $at
        ctx->r3 = ctx->r2 & ctx->r1;
            goto L_8014422C;
    }
    // 0x801441F8: and         $v1, $v0, $at
    ctx->r3 = ctx->r2 & ctx->r1;
    // 0x801441FC: beq         $v1, $zero, L_80144220
    if (ctx->r3 == 0) {
        // 0x80144200: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_80144220;
    }
    // 0x80144200: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80144204: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144208: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x8014420C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80144210: jalr        $t9
    // 0x80144214: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80144214: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_5:
    // 0x80144218: b           L_80144224
    // 0x8014421C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80144224;
    // 0x8014421C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80144220:
    // 0x80144220: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80144224:
    // 0x80144224: beql        $v1, $zero, L_80144394
    if (ctx->r3 == 0) {
        // 0x80144228: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80144394;
    }
    goto skip_4;
    // 0x80144228: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_4:
L_8014422C:
    // 0x8014422C: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80144230: lui         $at, 0x3FFF
    ctx->r1 = S32(0X3FFF << 16);
    // 0x80144234: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80144238: sll         $t5, $v0, 1
    ctx->r13 = S32(ctx->r2 << 1);
    // 0x8014423C: bgez        $t5, L_80144274
    if (SIGNED(ctx->r13) >= 0) {
        // 0x80144240: and         $v1, $v0, $at
        ctx->r3 = ctx->r2 & ctx->r1;
            goto L_80144274;
    }
    // 0x80144240: and         $v1, $v0, $at
    ctx->r3 = ctx->r2 & ctx->r1;
    // 0x80144244: beq         $v1, $zero, L_80144268
    if (ctx->r3 == 0) {
        // 0x80144248: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_80144268;
    }
    // 0x80144248: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8014424C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144250: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80144254: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80144258: jalr        $t9
    // 0x8014425C: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x8014425C: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_6:
    // 0x80144260: b           L_8014426C
    // 0x80144264: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_8014426C;
    // 0x80144264: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80144268:
    // 0x80144268: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8014426C:
    // 0x8014426C: bnel        $v1, $zero, L_80144394
    if (ctx->r3 != 0) {
        // 0x80144270: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_80144394;
    }
    goto skip_5;
    // 0x80144270: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_5:
L_80144274:
    // 0x80144274: lhu         $a0, 0xE($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0XE);
    // 0x80144278: beql        $a0, $zero, L_80144298
    if (ctx->r4 == 0) {
        // 0x8014427C: lhu         $t7, 0xC($s0)
        ctx->r15 = MEM_HU(ctx->r16, 0XC);
            goto L_80144298;
    }
    goto skip_6;
    // 0x8014427C: lhu         $t7, 0xC($s0)
    ctx->r15 = MEM_HU(ctx->r16, 0XC);
    skip_6:
    // 0x80144280: lw          $t6, 0x10($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X10);
    // 0x80144284: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144288: addiu       $t9, $t9, 0x47C0
    ctx->r25 = ADD32(ctx->r25, 0X47C0);
    // 0x8014428C: bne         $t6, $zero, L_801442D0
    if (ctx->r14 != 0) {
        // 0x80144290: nop
    
            goto L_801442D0;
    }
    // 0x80144290: nop

    // 0x80144294: lhu         $t7, 0xC($s0)
    ctx->r15 = MEM_HU(ctx->r16, 0XC);
L_80144298:
    // 0x80144298: lhu         $a1, 0x6($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X6);
    // 0x8014429C: lhu         $a2, 0x8($s0)
    ctx->r6 = MEM_HU(ctx->r16, 0X8);
    // 0x801442A0: lhu         $a3, 0xA($s0)
    ctx->r7 = MEM_HU(ctx->r16, 0XA);
    // 0x801442A4: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    // 0x801442A8: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x801442AC: lw          $a0, 0x68($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X68);
    // 0x801442B0: sw          $t8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r24;
    // 0x801442B4: lh          $t0, 0x2($s0)
    ctx->r8 = MEM_H(ctx->r16, 0X2);
    // 0x801442B8: sw          $t0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r8;
    // 0x801442BC: lh          $t1, 0x4($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X4);
    // 0x801442C0: jal         0x80144EE4
    // 0x801442C4: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    func_80144EE4(rdram, ctx);
        goto after_7;
    // 0x801442C4: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    after_7:
    // 0x801442C8: b           L_8014431C
    // 0x801442CC: lh          $s0, 0xC($s2)
    ctx->r16 = MEM_H(ctx->r18, 0XC);
        goto L_8014431C;
    // 0x801442CC: lh          $s0, 0xC($s2)
    ctx->r16 = MEM_H(ctx->r18, 0XC);
L_801442D0:
    // 0x801442D0: jalr        $t9
    // 0x801442D4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x801442D4: nop

    after_8:
    // 0x801442D8: lw          $t3, 0x10($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X10);
    // 0x801442DC: sll         $t2, $v0, 3
    ctx->r10 = S32(ctx->r2 << 3);
    // 0x801442E0: lw          $a0, 0x68($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X68);
    // 0x801442E4: addu        $v1, $t2, $t3
    ctx->r3 = ADD32(ctx->r10, ctx->r11);
    // 0x801442E8: lhu         $t4, 0x6($v1)
    ctx->r12 = MEM_HU(ctx->r3, 0X6);
    // 0x801442EC: lhu         $a1, 0x0($v1)
    ctx->r5 = MEM_HU(ctx->r3, 0X0);
    // 0x801442F0: lhu         $a2, 0x2($v1)
    ctx->r6 = MEM_HU(ctx->r3, 0X2);
    // 0x801442F4: lhu         $a3, 0x4($v1)
    ctx->r7 = MEM_HU(ctx->r3, 0X4);
    // 0x801442F8: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    // 0x801442FC: lh          $t5, 0x0($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X0);
    // 0x80144300: sw          $t5, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r13;
    // 0x80144304: lh          $t6, 0x2($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X2);
    // 0x80144308: sw          $t6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r14;
    // 0x8014430C: lh          $t7, 0x4($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X4);
    // 0x80144310: jal         0x80144EE4
    // 0x80144314: sw          $t7, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r15;
    func_80144EE4(rdram, ctx);
        goto after_9;
    // 0x80144314: sw          $t7, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r15;
    after_9:
    // 0x80144318: lh          $s0, 0xC($s2)
    ctx->r16 = MEM_H(ctx->r18, 0XC);
L_8014431C:
    // 0x8014431C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80144320: lui         $v0, 0x8000
    ctx->r2 = S32(0X8000 << 16);
    // 0x80144324: beq         $s0, $at, L_80144390
    if (ctx->r16 == ctx->r1) {
        // 0x80144328: addiu       $v0, $v0, 0x47C0
        ctx->r2 = ADD32(ctx->r2, 0X47C0);
            goto L_80144390;
    }
    // 0x80144328: addiu       $v0, $v0, 0x47C0
    ctx->r2 = ADD32(ctx->r2, 0X47C0);
    // 0x8014432C: sw          $v0, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r2;
    // 0x80144330: sll         $a0, $s0, 2
    ctx->r4 = S32(ctx->r16 << 2);
L_80144334:
    // 0x80144334: lw          $v0, 0x3C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X3C);
    // 0x80144338: addu        $a0, $a0, $s0
    ctx->r4 = ADD32(ctx->r4, ctx->r16);
    // 0x8014433C: sll         $a0, $a0, 1
    ctx->r4 = S32(ctx->r4 << 1);
    // 0x80144340: addiu       $a0, $a0, -0xA
    ctx->r4 = ADD32(ctx->r4, -0XA);
    // 0x80144344: jalr        $v0
    // 0x80144348: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_10;
    // 0x80144348: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_10:
    // 0x8014434C: lhu         $v1, 0x16($s1)
    ctx->r3 = MEM_HU(ctx->r17, 0X16);
    // 0x80144350: lh          $s0, 0xC($s2)
    ctx->r16 = MEM_H(ctx->r18, 0XC);
    // 0x80144354: addu        $t8, $v0, $v1
    ctx->r24 = ADD32(ctx->r2, ctx->r3);
    // 0x80144358: div         $zero, $t8, $s0
    lo = S32(S64(S32(ctx->r24)) / S64(S32(ctx->r16))); hi = S32(S64(S32(ctx->r24)) % S64(S32(ctx->r16)));
    // 0x8014435C: mfhi        $a0
    ctx->r4 = hi;
    // 0x80144360: bne         $s0, $zero, L_8014436C
    if (ctx->r16 != 0) {
        // 0x80144364: nop
    
            goto L_8014436C;
    }
    // 0x80144364: nop

    // 0x80144368: break       7
    do_break(2148811624);
L_8014436C:
    // 0x8014436C: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80144370: bne         $s0, $at, L_80144384
    if (ctx->r16 != ctx->r1) {
        // 0x80144374: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80144384;
    }
    // 0x80144374: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80144378: bne         $t8, $at, L_80144384
    if (ctx->r24 != ctx->r1) {
        // 0x8014437C: nop
    
            goto L_80144384;
    }
    // 0x8014437C: nop

    // 0x80144380: break       6
    do_break(2148811648);
L_80144384:
    // 0x80144384: beql        $a0, $v1, L_80144334
    if (ctx->r4 == ctx->r3) {
        // 0x80144388: sll         $a0, $s0, 2
        ctx->r4 = S32(ctx->r16 << 2);
            goto L_80144334;
    }
    goto skip_7;
    // 0x80144388: sll         $a0, $s0, 2
    ctx->r4 = S32(ctx->r16 << 2);
    skip_7:
    // 0x8014438C: sh          $a0, 0x16($s1)
    MEM_H(0X16, ctx->r17) = ctx->r4;
L_80144390:
    // 0x80144390: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
L_80144394:
    // 0x80144394: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x80144398: lw          $s1, 0x2C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X2C);
    // 0x8014439C: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x801443A0: jr          $ra
    // 0x801443A4: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x801443A4: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void func_801443A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801443A8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801443AC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801443B0: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x801443B4: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801443B8: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801443BC: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801443C0: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x801443C4: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x801443C8: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801443CC: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801443D0: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801443D4: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801443D8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801443DC: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801443E0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801443E4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801443E8: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801443EC: lw          $t9, -0x2F24($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2F24);
    // 0x801443F0: jalr        $t9
    // 0x801443F4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801443F4: nop

    after_0:
    // 0x801443F8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801443FC: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80144400: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80144404: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80144408: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8014440C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80144410: jr          $ra
    // 0x80144414: nop

    return;
    // 0x80144414: nop

;}
RECOMP_FUNC void func_80144418(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144418: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8014441C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144420: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80144424: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80144428: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x8014442C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80144430: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80144434: addiu       $a2, $zero, 0x34
    ctx->r6 = ADD32(0, 0X34);
    // 0x80144438: jalr        $t9
    // 0x8014443C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8014443C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x80144440: bne         $v0, $zero, L_80144460
    if (ctx->r2 != 0) {
        // 0x80144444: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_80144460;
    }
    // 0x80144444: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80144448: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x8014444C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80144450: jalr        $t9
    // 0x80144454: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80144454: nop

    after_1:
    // 0x80144458: b           L_80144474
    // 0x8014445C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80144474;
    // 0x8014445C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80144460:
    // 0x80144460: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144464: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80144468: jalr        $t9
    // 0x8014446C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8014446C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x80144470: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80144474:
    // 0x80144474: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80144478: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8014447C: jr          $ra
    // 0x80144480: nop

    return;
    // 0x80144480: nop

;}
RECOMP_FUNC void func_80144484(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144484: addiu       $sp, $sp, -0x90
    ctx->r29 = ADD32(ctx->r29, -0X90);
    // 0x80144488: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8014448C: lui         $s1, 0x801D
    ctx->r17 = S32(0X801D << 16);
    // 0x80144490: addiu       $s1, $s1, -0x7D40
    ctx->r17 = ADD32(ctx->r17, -0X7D40);
    // 0x80144494: lh          $t6, 0x28D0($s1)
    ctx->r14 = MEM_H(ctx->r17, 0X28D0);
    // 0x80144498: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8014449C: lui         $s0, 0x8019
    ctx->r16 = S32(0X8019 << 16);
    // 0x801444A0: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x801444A4: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x801444A8: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x801444AC: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x801444B0: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x801444B4: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x801444B8: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x801444BC: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x801444C0: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x801444C4: addu        $s0, $s0, $t7
    ctx->r16 = ADD32(ctx->r16, ctx->r15);
    // 0x801444C8: lw          $s3, 0x34($a0)
    ctx->r19 = MEM_W(ctx->r4, 0X34);
    // 0x801444CC: lw          $s0, -0x2CFC($s0)
    ctx->r16 = MEM_W(ctx->r16, -0X2CFC);
    // 0x801444D0: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x801444D4: or          $fp, $a0, $zero
    ctx->r30 = ctx->r4 | 0;
    // 0x801444D8: sw          $s0, 0x14($s3)
    MEM_W(0X14, ctx->r19) = ctx->r16;
    // 0x801444DC: lhu         $t8, 0x18($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X18);
    // 0x801444E0: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x801444E4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801444E8: sll         $t9, $t8, 4
    ctx->r25 = S32(ctx->r24 << 4);
    // 0x801444EC: addu        $s0, $s0, $t9
    ctx->r16 = ADD32(ctx->r16, ctx->r25);
    // 0x801444F0: sw          $s0, 0x14($s3)
    MEM_W(0X14, ctx->r19) = ctx->r16;
    // 0x801444F4: lhu         $t0, 0x2($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X2);
    // 0x801444F8: ori         $t1, $t0, 0x4
    ctx->r9 = ctx->r8 | 0X4;
    // 0x801444FC: sh          $t1, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r9;
    // 0x80144500: jal         0x80005A30
    // 0x80144504: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    sceneLookup(rdram, ctx);
        goto after_0;
    // 0x80144504: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    after_0:
    // 0x80144508: bne         $v0, $zero, L_80144528
    if (ctx->r2 != 0) {
        // 0x8014450C: sw          $v0, 0x24($fp)
        MEM_W(0X24, ctx->r30) = ctx->r2;
            goto L_80144528;
    }
    // 0x8014450C: sw          $v0, 0x24($fp)
    MEM_W(0X24, ctx->r30) = ctx->r2;
    // 0x80144510: lw          $t9, 0x10($fp)
    ctx->r25 = MEM_W(ctx->r30, 0X10);
    // 0x80144514: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80144518: jalr        $t9
    // 0x8014451C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8014451C: nop

    after_1:
    // 0x80144520: b           L_80144760
    // 0x80144524: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
        goto L_80144760;
    // 0x80144524: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_80144528:
    // 0x80144528: lw          $s5, 0x24($fp)
    ctx->r21 = MEM_W(ctx->r30, 0X24);
    // 0x8014452C: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80144530: jal         0x801431CC
    // 0x80144534: or          $a1, $s5, $zero
    ctx->r5 = ctx->r21 | 0;
    sceneStateCopy(rdram, ctx);
        goto after_2;
    // 0x80144534: or          $a1, $s5, $zero
    ctx->r5 = ctx->r21 | 0;
    after_2:
    // 0x80144538: lh          $t2, 0x6($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X6);
    // 0x8014453C: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80144540: addiu       $t4, $t4, -0x3044
    ctx->r12 = ADD32(ctx->r12, -0X3044);
    // 0x80144544: sll         $t3, $t2, 5
    ctx->r11 = S32(ctx->r10 << 5);
    // 0x80144548: addu        $s7, $t3, $t4
    ctx->r23 = ADD32(ctx->r11, ctx->r12);
    // 0x8014454C: lh          $t5, 0xC($s7)
    ctx->r13 = MEM_H(ctx->r23, 0XC);
    // 0x80144550: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x80144554: addiu       $t9, $t9, -0x6C40
    ctx->r25 = ADD32(ctx->r25, -0X6C40);
    // 0x80144558: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x8014455C: addiu       $a0, $a0, -0x19F0
    ctx->r4 = ADD32(ctx->r4, -0X19F0);
    // 0x80144560: jalr        $t9
    // 0x80144564: sw          $t5, 0x40($s5)
    MEM_W(0X40, ctx->r21) = ctx->r13;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80144564: sw          $t5, 0x40($s5)
    MEM_W(0X40, ctx->r21) = ctx->r13;
    after_3:
    // 0x80144568: lhu         $t7, 0x2($s5)
    ctx->r15 = MEM_HU(ctx->r21, 0X2);
    // 0x8014456C: sw          $v0, 0x38($s5)
    MEM_W(0X38, ctx->r21) = ctx->r2;
    // 0x80144570: lw          $t6, 0x8($s7)
    ctx->r14 = MEM_W(ctx->r23, 0X8);
    // 0x80144574: ori         $t8, $t7, 0x900
    ctx->r24 = ctx->r15 | 0X900;
    // 0x80144578: sh          $t8, 0x2($s5)
    MEM_H(0X2, ctx->r21) = ctx->r24;
    // 0x8014457C: sw          $t6, 0x3C($s5)
    MEM_W(0X3C, ctx->r21) = ctx->r14;
    // 0x80144580: lhu         $t0, 0x14($s7)
    ctx->r8 = MEM_HU(ctx->r23, 0X14);
    // 0x80144584: ori         $t3, $t8, 0x40
    ctx->r11 = ctx->r24 | 0X40;
    // 0x80144588: andi        $t1, $t0, 0x1
    ctx->r9 = ctx->r8 & 0X1;
    // 0x8014458C: beql        $t1, $zero, L_801445A0
    if (ctx->r9 == 0) {
        // 0x80144590: lw          $t4, 0x8($s0)
        ctx->r12 = MEM_W(ctx->r16, 0X8);
            goto L_801445A0;
    }
    goto skip_0;
    // 0x80144590: lw          $t4, 0x8($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X8);
    skip_0:
    // 0x80144594: b           L_801445A4
    // 0x80144598: sh          $t3, 0x2($s5)
    MEM_H(0X2, ctx->r21) = ctx->r11;
        goto L_801445A4;
    // 0x80144598: sh          $t3, 0x2($s5)
    MEM_H(0X2, ctx->r21) = ctx->r11;
    // 0x8014459C: lw          $t4, 0x8($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X8);
L_801445A0:
    // 0x801445A0: sh          $t4, 0x5E($s5)
    MEM_H(0X5E, ctx->r21) = ctx->r12;
L_801445A4:
    // 0x801445A4: lw          $t5, 0x2B0C($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X2B0C);
    // 0x801445A8: addiu       $t6, $zero, 0xFF
    ctx->r14 = ADD32(0, 0XFF);
    // 0x801445AC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x801445B0: sw          $t5, 0x18($s5)
    MEM_W(0X18, ctx->r21) = ctx->r13;
    // 0x801445B4: lw          $t9, 0x7C($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X7C);
    // 0x801445B8: sb          $t6, 0x1B($s5)
    MEM_B(0X1B, ctx->r21) = ctx->r14;
    // 0x801445BC: sw          $t9, 0x24($s5)
    MEM_W(0X24, ctx->r21) = ctx->r25;
    // 0x801445C0: lh          $t7, 0x0($fp)
    ctx->r15 = MEM_H(ctx->r30, 0X0);
    // 0x801445C4: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x801445C8: addiu       $t9, $t9, 0x6504
    ctx->r25 = ADD32(ctx->r25, 0X6504);
    // 0x801445CC: ori         $t8, $t7, 0x1000
    ctx->r24 = ctx->r15 | 0X1000;
    // 0x801445D0: sh          $t8, 0x0($fp)
    MEM_H(0X0, ctx->r30) = ctx->r24;
    // 0x801445D4: lw          $a1, 0x40($s5)
    ctx->r5 = MEM_W(ctx->r21, 0X40);
    // 0x801445D8: jalr        $t9
    // 0x801445DC: lw          $a0, 0x3C($s5)
    ctx->r4 = MEM_W(ctx->r21, 0X3C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801445DC: lw          $a0, 0x3C($s5)
    ctx->r4 = MEM_W(ctx->r21, 0X3C);
    after_4:
    // 0x801445E0: sw          $v0, 0x74($s5)
    MEM_W(0X74, ctx->r21) = ctx->r2;
    // 0x801445E4: sw          $v0, 0x68($fp)
    MEM_W(0X68, ctx->r30) = ctx->r2;
    // 0x801445E8: jal         0x80146900
    // 0x801445EC: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    func_80146900(rdram, ctx);
        goto after_5;
    // 0x801445EC: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    after_5:
    // 0x801445F0: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x801445F4: addiu       $t9, $t9, 0x524
    ctx->r25 = ADD32(ctx->r25, 0X524);
    // 0x801445F8: jalr        $t9
    // 0x801445FC: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x801445FC: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    after_6:
    // 0x80144600: bne         $v0, $zero, L_80144628
    if (ctx->r2 != 0) {
        // 0x80144604: or          $s6, $v0, $zero
        ctx->r22 = ctx->r2 | 0;
            goto L_80144628;
    }
    // 0x80144604: or          $s6, $v0, $zero
    ctx->r22 = ctx->r2 | 0;
    // 0x80144608: bnel        $s3, $zero, L_8014462C
    if (ctx->r19 != 0) {
        // 0x8014460C: addiu       $t0, $s3, 0x8
        ctx->r8 = ADD32(ctx->r19, 0X8);
            goto L_8014462C;
    }
    goto skip_1;
    // 0x8014460C: addiu       $t0, $s3, 0x8
    ctx->r8 = ADD32(ctx->r19, 0X8);
    skip_1:
    // 0x80144610: lw          $t9, 0x10($fp)
    ctx->r25 = MEM_W(ctx->r30, 0X10);
    // 0x80144614: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80144618: jalr        $t9
    // 0x8014461C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x8014461C: nop

    after_7:
    // 0x80144620: b           L_80144760
    // 0x80144624: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
        goto L_80144760;
    // 0x80144624: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_80144628:
    // 0x80144628: addiu       $t0, $s3, 0x8
    ctx->r8 = ADD32(ctx->r19, 0X8);
L_8014462C:
    // 0x8014462C: sw          $t0, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r8;
    // 0x80144630: lw          $t1, 0x10($fp)
    ctx->r9 = MEM_W(ctx->r30, 0X10);
    // 0x80144634: lui         $t3, 0x8014
    ctx->r11 = S32(0X8014 << 16);
    // 0x80144638: addiu       $t3, $t3, 0x4C78
    ctx->r11 = ADD32(ctx->r11, 0X4C78);
    // 0x8014463C: sw          $t1, 0x8($t0)
    MEM_W(0X8, ctx->r8) = ctx->r9;
    // 0x80144640: sw          $t3, 0x10($fp)
    MEM_W(0X10, ctx->r30) = ctx->r11;
    // 0x80144644: sw          $v0, 0x4($s3)
    MEM_W(0X4, ctx->r19) = ctx->r2;
    // 0x80144648: lhu         $t4, 0xA($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0XA);
    // 0x8014464C: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80144650: addiu       $t2, $zero, 0x3E8
    ctx->r10 = ADD32(0, 0X3E8);
    // 0x80144654: andi        $t6, $t4, 0xF9FF
    ctx->r14 = ctx->r12 & 0XF9FF;
    // 0x80144658: ori         $t8, $t6, 0x200
    ctx->r24 = ctx->r14 | 0X200;
    // 0x8014465C: sh          $t6, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r14;
    // 0x80144660: andi        $t0, $t8, 0xFFFC
    ctx->r8 = ctx->r24 & 0XFFFC;
    // 0x80144664: sh          $t8, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r24;
    // 0x80144668: sh          $t0, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r8;
    // 0x8014466C: ori         $t1, $t0, 0x3
    ctx->r9 = ctx->r8 | 0X3;
    // 0x80144670: sb          $v1, 0xD($v0)
    MEM_B(0XD, ctx->r2) = ctx->r3;
    // 0x80144674: sh          $t1, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r9;
    // 0x80144678: sh          $v1, 0x4($v0)
    MEM_H(0X4, ctx->r2) = ctx->r3;
    // 0x8014467C: sh          $t2, 0x8($v0)
    MEM_H(0X8, ctx->r2) = ctx->r10;
    // 0x80144680: lh          $t3, 0xE($s7)
    ctx->r11 = MEM_H(ctx->r23, 0XE);
    // 0x80144684: lw          $s1, 0x10($s7)
    ctx->r17 = MEM_W(ctx->r23, 0X10);
    // 0x80144688: lui         $s3, 0x8004
    ctx->r19 = S32(0X8004 << 16);
    // 0x8014468C: blez        $t3, L_80144734
    if (SIGNED(ctx->r11) <= 0) {
        // 0x80144690: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_80144734;
    }
    // 0x80144690: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80144694: lui         $s4, 0x8004
    ctx->r20 = S32(0X8004 << 16);
    // 0x80144698: addiu       $s4, $s4, 0x844
    ctx->r20 = ADD32(ctx->r20, 0X844);
    // 0x8014469C: addiu       $s3, $s3, 0x628
    ctx->r19 = ADD32(ctx->r19, 0X628);
    // 0x801446A0: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
L_801446A4:
    // 0x801446A4: jalr        $s3
    // 0x801446A8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_8;
    // 0x801446A8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_8:
    // 0x801446AC: bne         $v0, $zero, L_801446CC
    if (ctx->r2 != 0) {
        // 0x801446B0: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_801446CC;
    }
    // 0x801446B0: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x801446B4: lw          $t9, 0x10($fp)
    ctx->r25 = MEM_W(ctx->r30, 0X10);
    // 0x801446B8: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x801446BC: jalr        $t9
    // 0x801446C0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x801446C0: nop

    after_9:
    // 0x801446C4: b           L_80144760
    // 0x801446C8: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
        goto L_80144760;
    // 0x801446C8: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_801446CC:
    // 0x801446CC: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x801446D0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801446D4: jalr        $s4
    // 0x801446D8: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_10;
    // 0x801446D8: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_10:
    // 0x801446DC: lhu         $t4, 0x0($s1)
    ctx->r12 = MEM_HU(ctx->r17, 0X0);
    // 0x801446E0: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x801446E4: addiu       $s1, $s1, 0x1C
    ctx->r17 = ADD32(ctx->r17, 0X1C);
    // 0x801446E8: sh          $t4, 0x12($s0)
    MEM_H(0X12, ctx->r16) = ctx->r12;
    // 0x801446EC: lhu         $t5, -0x1A($s1)
    ctx->r13 = MEM_HU(ctx->r17, -0X1A);
    // 0x801446F0: sh          $t5, 0x14($s0)
    MEM_H(0X14, ctx->r16) = ctx->r13;
    // 0x801446F4: lhu         $t6, -0x18($s1)
    ctx->r14 = MEM_HU(ctx->r17, -0X18);
    // 0x801446F8: sh          $t6, 0x16($s0)
    MEM_H(0X16, ctx->r16) = ctx->r14;
    // 0x801446FC: lwc1        $f4, -0x14($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, -0X14);
    // 0x80144700: swc1        $f4, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f4.u32l;
    // 0x80144704: lwc1        $f6, -0x10($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, -0X10);
    // 0x80144708: swc1        $f6, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f6.u32l;
    // 0x8014470C: lwc1        $f8, -0xC($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, -0XC);
    // 0x80144710: swc1        $f8, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f8.u32l;
    // 0x80144714: lwc1        $f10, -0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, -0X8);
    // 0x80144718: swc1        $f10, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f10.u32l;
    // 0x8014471C: lwc1        $f16, -0x4($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, -0X4);
    // 0x80144720: swc1        $f16, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f16.u32l;
    // 0x80144724: lh          $t7, 0xE($s7)
    ctx->r15 = MEM_H(ctx->r23, 0XE);
    // 0x80144728: slt         $at, $s2, $t7
    ctx->r1 = SIGNED(ctx->r18) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x8014472C: bnel        $at, $zero, L_801446A4
    if (ctx->r1 != 0) {
        // 0x80144730: or          $a0, $s5, $zero
        ctx->r4 = ctx->r21 | 0;
            goto L_801446A4;
    }
    goto skip_2;
    // 0x80144730: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    skip_2:
L_80144734:
    // 0x80144734: lw          $t0, 0x44($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X44);
    // 0x80144738: addiu       $t8, $zero, 0x3
    ctx->r24 = ADD32(0, 0X3);
    // 0x8014473C: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80144740: jal         0x80143234
    // 0x80144744: sh          $t8, 0x26($t0)
    MEM_H(0X26, ctx->r8) = ctx->r24;
    func_80143234(rdram, ctx);
        goto after_11;
    // 0x80144744: sh          $t8, 0x26($t0)
    MEM_H(0X26, ctx->r8) = ctx->r24;
    after_11:
    // 0x80144748: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8014474C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80144750: addiu       $a0, $fp, 0x8
    ctx->r4 = ADD32(ctx->r30, 0X8);
    // 0x80144754: jalr        $t9
    // 0x80144758: addiu       $a1, $fp, 0xE
    ctx->r5 = ADD32(ctx->r30, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x80144758: addiu       $a1, $fp, 0xE
    ctx->r5 = ADD32(ctx->r30, 0XE);
    after_12:
    // 0x8014475C: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_80144760:
    // 0x80144760: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80144764: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80144768: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8014476C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80144770: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80144774: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80144778: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x8014477C: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80144780: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80144784: jr          $ra
    // 0x80144788: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
    return;
    // 0x80144788: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
;}
RECOMP_FUNC void func_8014478C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014478C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80144790: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80144794: lw          $a2, 0x24($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X24);
    // 0x80144798: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8014479C: addiu       $a1, $a2, 0x50
    ctx->r5 = ADD32(ctx->r6, 0X50);
    // 0x801447A0: jal         0x8014314C
    // 0x801447A4: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    func_8014314C(rdram, ctx);
        goto after_0;
    // 0x801447A4: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    after_0:
    // 0x801447A8: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x801447AC: beq         $v0, $zero, L_801447C8
    if (ctx->r2 == 0) {
        // 0x801447B0: lw          $a2, 0x1C($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X1C);
            goto L_801447C8;
    }
    // 0x801447B0: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x801447B4: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x801447B8: jalr        $t9
    // 0x801447BC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801447BC: nop

    after_1:
    // 0x801447C0: b           L_8014483C
    // 0x801447C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8014483C;
    // 0x801447C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801447C8:
    // 0x801447C8: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801447CC: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x801447D0: lw          $t6, 0x2B0C($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X2B0C);
    // 0x801447D4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801447D8: sw          $t6, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r14;
    // 0x801447DC: lw          $t7, 0x7C($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X7C);
    // 0x801447E0: sw          $t7, 0x24($a2)
    MEM_W(0X24, ctx->r6) = ctx->r15;
    // 0x801447E4: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x801447E8: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801447EC: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801447F0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801447F4: sll         $t8, $v1, 1
    ctx->r24 = S32(ctx->r3 << 1);
    // 0x801447F8: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x801447FC: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80144800: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80144804: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80144808: sb          $t1, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r9;
    // 0x8014480C: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80144810: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80144814: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80144818: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x8014481C: lw          $t9, -0x2F18($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2F18);
    // 0x80144820: jalr        $t9
    // 0x80144824: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80144824: nop

    after_2:
    // 0x80144828: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x8014482C: lh          $t4, 0xE($a0)
    ctx->r12 = MEM_H(ctx->r4, 0XE);
    // 0x80144830: addiu       $t5, $t4, -0x1
    ctx->r13 = ADD32(ctx->r12, -0X1);
    // 0x80144834: sh          $t5, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r13;
    // 0x80144838: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8014483C:
    // 0x8014483C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80144840: jr          $ra
    // 0x80144844: nop

    return;
    // 0x80144844: nop

;}
RECOMP_FUNC void func_80144848(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144848: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8014484C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80144850: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80144854: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80144858: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8014485C: lw          $a3, 0x4($v1)
    ctx->r7 = MEM_W(ctx->r3, 0X4);
    // 0x80144860: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x80144864: jal         0x8014558C
    // 0x80144868: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    func_8014558C(rdram, ctx);
        goto after_0;
    // 0x80144868: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    after_0:
    // 0x8014486C: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x80144870: beq         $v0, $zero, L_801448AC
    if (ctx->r2 == 0) {
        // 0x80144874: lw          $a3, 0x20($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X20);
            goto L_801448AC;
    }
    // 0x80144874: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x80144878: lh          $t6, 0x0($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X0);
    // 0x8014487C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144880: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80144884: sw          $t6, 0x30($v1)
    MEM_W(0X30, ctx->r3) = ctx->r14;
    // 0x80144888: lh          $t7, 0x0($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X0);
    // 0x8014488C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80144890: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80144894: andi        $t8, $t7, 0xF5FF
    ctx->r24 = ctx->r15 & 0XF5FF;
    // 0x80144898: sh          $t8, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r24;
    // 0x8014489C: jalr        $t9
    // 0x801448A0: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801448A0: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_1:
    // 0x801448A4: b           L_80144904
    // 0x801448A8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80144904;
    // 0x801448A8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801448AC:
    // 0x801448AC: lw          $t0, 0x4C($a3)
    ctx->r8 = MEM_W(ctx->r7, 0X4C);
    // 0x801448B0: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x801448B4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801448B8: beq         $t0, $zero, L_80144900
    if (ctx->r8 == 0) {
        // 0x801448BC: lui         $t9, 0x8004
        ctx->r25 = S32(0X8004 << 16);
            goto L_80144900;
    }
    // 0x801448BC: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x801448C0: addiu       $t9, $t9, 0x4780
    ctx->r25 = ADD32(ctx->r25, 0X4780);
    // 0x801448C4: jalr        $t9
    // 0x801448C8: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801448C8: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_2:
    // 0x801448CC: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x801448D0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801448D4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801448D8: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x801448DC: lh          $t1, 0x26($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X26);
    // 0x801448E0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801448E4: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801448E8: sh          $t2, 0x26($v0)
    MEM_H(0X26, ctx->r2) = ctx->r10;
    // 0x801448EC: lh          $t3, 0x26($v0)
    ctx->r11 = MEM_H(ctx->r2, 0X26);
    // 0x801448F0: bgtzl       $t3, L_80144904
    if (SIGNED(ctx->r11) > 0) {
        // 0x801448F4: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80144904;
    }
    goto skip_0;
    // 0x801448F4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x801448F8: jalr        $t9
    // 0x801448FC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801448FC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
L_80144900:
    // 0x80144900: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80144904:
    // 0x80144904: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80144908: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8014490C: jr          $ra
    // 0x80144910: nop

    return;
    // 0x80144910: nop

;}
RECOMP_FUNC void func_80144914(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144914: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x80144918: sw          $ra, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r31;
    // 0x8014491C: sw          $fp, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r30;
    // 0x80144920: sw          $s7, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r23;
    // 0x80144924: sw          $s6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r22;
    // 0x80144928: sw          $s5, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r21;
    // 0x8014492C: sw          $s4, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r20;
    // 0x80144930: sw          $s3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r19;
    // 0x80144934: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x80144938: sw          $s1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r17;
    // 0x8014493C: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x80144940: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x80144944: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80144948: or          $s7, $a0, $zero
    ctx->r23 = ctx->r4 | 0;
    // 0x8014494C: sw          $t6, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r14;
    // 0x80144950: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x80144954: lw          $s1, 0x68($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X68);
    // 0x80144958: addiu       $t9, $t9, -0x3044
    ctx->r25 = ADD32(ctx->r25, -0X3044);
    // 0x8014495C: lw          $s5, 0x14($s0)
    ctx->r21 = MEM_W(ctx->r16, 0X14);
    // 0x80144960: lui         $s6, 0x801D
    ctx->r22 = S32(0X801D << 16);
    // 0x80144964: addiu       $a0, $s6, -0x55A0
    ctx->r4 = ADD32(ctx->r22, -0X55A0);
    // 0x80144968: lh          $t7, 0x6($s5)
    ctx->r15 = MEM_H(ctx->r21, 0X6);
    // 0x8014496C: lw          $v0, 0xC($s5)
    ctx->r2 = MEM_W(ctx->r21, 0XC);
    // 0x80144970: lw          $s2, 0x4($s0)
    ctx->r18 = MEM_W(ctx->r16, 0X4);
    // 0x80144974: sll         $t8, $t7, 5
    ctx->r24 = S32(ctx->r15 << 5);
    // 0x80144978: beq         $v0, $zero, L_80144990
    if (ctx->r2 == 0) {
        // 0x8014497C: addu        $s4, $t8, $t9
        ctx->r20 = ADD32(ctx->r24, ctx->r25);
            goto L_80144990;
    }
    // 0x8014497C: addu        $s4, $t8, $t9
    ctx->r20 = ADD32(ctx->r24, ctx->r25);
    // 0x80144980: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144984: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x80144988: jalr        $t9
    // 0x8014498C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8014498C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_0:
L_80144990:
    // 0x80144990: lw          $a1, 0x50($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X50);
    // 0x80144994: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80144998: lui         $s6, 0x801D
    ctx->r22 = S32(0X801D << 16);
    // 0x8014499C: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x801449A0: addiu       $s6, $s6, -0x55A0
    ctx->r22 = ADD32(ctx->r22, -0X55A0);
    // 0x801449A4: lhu         $a0, 0x1E($s4)
    ctx->r4 = MEM_HU(ctx->r20, 0X1E);
    // 0x801449A8: jalr        $t9
    // 0x801449AC: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801449AC: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_1:
    // 0x801449B0: addiu       $s3, $s0, 0x8
    ctx->r19 = ADD32(ctx->r16, 0X8);
    // 0x801449B4: ori         $t0, $zero, 0xFFFF
    ctx->r8 = 0 | 0XFFFF;
    // 0x801449B8: sw          $t0, 0x18($s3)
    MEM_W(0X18, ctx->r19) = ctx->r8;
    // 0x801449BC: lh          $t1, 0x0($s2)
    ctx->r9 = MEM_H(ctx->r18, 0X0);
    // 0x801449C0: lui         $at, 0x3FA0
    ctx->r1 = S32(0X3FA0 << 16);
    // 0x801449C4: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x801449C8: andi        $t2, $t1, 0xFDFF
    ctx->r10 = ctx->r9 & 0XFDFF;
    // 0x801449CC: sh          $t2, 0x0($s2)
    MEM_H(0X0, ctx->r18) = ctx->r10;
    // 0x801449D0: lh          $t3, 0x1C($s1)
    ctx->r11 = MEM_H(ctx->r17, 0X1C);
    // 0x801449D4: lh          $t5, 0x18($s1)
    ctx->r13 = MEM_H(ctx->r17, 0X18);
    // 0x801449D8: lh          $t4, 0x1A($s1)
    ctx->r12 = MEM_H(ctx->r17, 0X1A);
    // 0x801449DC: mtc1        $t3, $f4
    ctx->f4.u32l = ctx->r11;
    // 0x801449E0: lw          $a3, 0x18($s4)
    ctx->r7 = MEM_W(ctx->r20, 0X18);
    // 0x801449E4: subu        $t6, $t4, $t5
    ctx->r14 = SUB32(ctx->r12, ctx->r13);
    // 0x801449E8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801449EC: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x801449F0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x801449F4: lw          $a0, 0x50($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X50);
    // 0x801449F8: cvt.s.w     $f12, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    ctx->f12.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801449FC: cvt.d.s     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f0.d = CVT_D_S(ctx->f6.fl);
    // 0x80144A00: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x80144A04: add.d       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = ctx->f0.d + ctx->f0.d;
    // 0x80144A08: cvt.s.d     $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f2.fl = CVT_S_D(ctx->f8.d);
    // 0x80144A0C: mul.s       $f16, $f2, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = MUL_S(ctx->f2.fl, ctx->f12.fl);
    // 0x80144A10: mfc1        $a1, $f2
    ctx->r5 = (int32_t)ctx->f2.u32l;
    // 0x80144A14: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x80144A18: mul.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x80144A1C: trunc.w.d   $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.u32l = TRUNC_W_D(ctx->f6.d);
    // 0x80144A20: jal         0x801456C8
    // 0x80144A24: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    func_801456C8(rdram, ctx);
        goto after_2;
    // 0x80144A24: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    after_2:
    // 0x80144A28: lh          $t8, 0x4($s5)
    ctx->r24 = MEM_H(ctx->r21, 0X4);
    // 0x80144A2C: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x80144A30: jal         0x801469F4
    // 0x80144A34: sh          $t8, 0x22($s3)
    MEM_H(0X22, ctx->r19) = ctx->r24;
    func_801469F4(rdram, ctx);
        goto after_3;
    // 0x80144A34: sh          $t8, 0x22($s3)
    MEM_H(0X22, ctx->r19) = ctx->r24;
    after_3:
    // 0x80144A38: lh          $t9, 0x4($s5)
    ctx->r25 = MEM_H(ctx->r21, 0X4);
    // 0x80144A3C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80144A40: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80144A44: blez        $t9, L_80144B18
    if (SIGNED(ctx->r25) <= 0) {
        // 0x80144A48: lui         $s4, 0x8000
        ctx->r20 = S32(0X8000 << 16);
            goto L_80144B18;
    }
    // 0x80144A48: lui         $s4, 0x8000
    ctx->r20 = S32(0X8000 << 16);
    // 0x80144A4C: lui         $s3, 0x3FFF
    ctx->r19 = S32(0X3FFF << 16);
    // 0x80144A50: ori         $s3, $s3, 0xFFFF
    ctx->r19 = ctx->r19 | 0XFFFF;
    // 0x80144A54: lui         $fp, 0x8000
    ctx->r30 = S32(0X8000 << 16);
    // 0x80144A58: addiu       $s4, $s4, 0x48A0
    ctx->r20 = ADD32(ctx->r20, 0X48A0);
    // 0x80144A5C: lw          $t0, 0x0($s5)
    ctx->r8 = MEM_W(ctx->r21, 0X0);
L_80144A60:
    // 0x80144A60: addu        $s0, $t0, $s2
    ctx->r16 = ADD32(ctx->r8, ctx->r18);
    // 0x80144A64: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80144A68: and         $t1, $v0, $fp
    ctx->r9 = ctx->r2 & ctx->r30;
    // 0x80144A6C: beq         $t1, $zero, L_80144A98
    if (ctx->r9 == 0) {
        // 0x80144A70: and         $v1, $v0, $s3
        ctx->r3 = ctx->r2 & ctx->r19;
            goto L_80144A98;
    }
    // 0x80144A70: and         $v1, $v0, $s3
    ctx->r3 = ctx->r2 & ctx->r19;
    // 0x80144A74: beq         $v1, $zero, L_80144A8C
    if (ctx->r3 == 0) {
        // 0x80144A78: or          $a0, $s6, $zero
        ctx->r4 = ctx->r22 | 0;
            goto L_80144A8C;
    }
    // 0x80144A78: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80144A7C: jalr        $s4
    // 0x80144A80: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_4;
    // 0x80144A80: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_4:
    // 0x80144A84: b           L_80144A90
    // 0x80144A88: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80144A90;
    // 0x80144A88: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80144A8C:
    // 0x80144A8C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80144A90:
    // 0x80144A90: beql        $v1, $zero, L_80144B4C
    if (ctx->r3 == 0) {
        // 0x80144A94: lw          $ra, 0x4C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X4C);
            goto L_80144B4C;
    }
    goto skip_0;
    // 0x80144A94: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
    skip_0:
L_80144A98:
    // 0x80144A98: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    // 0x80144A9C: sll         $t2, $v0, 1
    ctx->r10 = S32(ctx->r2 << 1);
    // 0x80144AA0: bgez        $t2, L_80144ACC
    if (SIGNED(ctx->r10) >= 0) {
        // 0x80144AA4: and         $v1, $v0, $s3
        ctx->r3 = ctx->r2 & ctx->r19;
            goto L_80144ACC;
    }
    // 0x80144AA4: and         $v1, $v0, $s3
    ctx->r3 = ctx->r2 & ctx->r19;
    // 0x80144AA8: beq         $v1, $zero, L_80144AC0
    if (ctx->r3 == 0) {
        // 0x80144AAC: or          $a0, $s6, $zero
        ctx->r4 = ctx->r22 | 0;
            goto L_80144AC0;
    }
    // 0x80144AAC: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80144AB0: jalr        $s4
    // 0x80144AB4: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_5;
    // 0x80144AB4: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_5:
    // 0x80144AB8: b           L_80144AC4
    // 0x80144ABC: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80144AC4;
    // 0x80144ABC: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80144AC0:
    // 0x80144AC0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80144AC4:
    // 0x80144AC4: bnel        $v1, $zero, L_80144B4C
    if (ctx->r3 != 0) {
        // 0x80144AC8: lw          $ra, 0x4C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X4C);
            goto L_80144B4C;
    }
    goto skip_1;
    // 0x80144AC8: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
    skip_1:
L_80144ACC:
    // 0x80144ACC: lhu         $t3, 0xC($s0)
    ctx->r11 = MEM_HU(ctx->r16, 0XC);
    // 0x80144AD0: lhu         $a1, 0x6($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X6);
    // 0x80144AD4: lhu         $a2, 0x8($s0)
    ctx->r6 = MEM_HU(ctx->r16, 0X8);
    // 0x80144AD8: lhu         $a3, 0xA($s0)
    ctx->r7 = MEM_HU(ctx->r16, 0XA);
    // 0x80144ADC: sw          $t3, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r11;
    // 0x80144AE0: lh          $t4, 0x0($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X0);
    // 0x80144AE4: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x80144AE8: sw          $t4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r12;
    // 0x80144AEC: lh          $t5, 0x2($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X2);
    // 0x80144AF0: sw          $t5, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r13;
    // 0x80144AF4: lh          $t6, 0x4($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X4);
    // 0x80144AF8: jal         0x80145030
    // 0x80144AFC: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    func_80145030(rdram, ctx);
        goto after_6;
    // 0x80144AFC: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    after_6:
    // 0x80144B00: lh          $t7, 0x4($s5)
    ctx->r15 = MEM_H(ctx->r21, 0X4);
    // 0x80144B04: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80144B08: addiu       $s2, $s2, 0x18
    ctx->r18 = ADD32(ctx->r18, 0X18);
    // 0x80144B0C: slt         $at, $s1, $t7
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x80144B10: bnel        $at, $zero, L_80144A60
    if (ctx->r1 != 0) {
        // 0x80144B14: lw          $t0, 0x0($s5)
        ctx->r8 = MEM_W(ctx->r21, 0X0);
            goto L_80144A60;
    }
    goto skip_2;
    // 0x80144B14: lw          $t0, 0x0($s5)
    ctx->r8 = MEM_W(ctx->r21, 0X0);
    skip_2:
L_80144B18:
    // 0x80144B18: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x80144B1C: addiu       $t9, $t9, -0x6C40
    ctx->r25 = ADD32(ctx->r25, -0X6C40);
    // 0x80144B20: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80144B24: jalr        $t9
    // 0x80144B28: addiu       $a0, $a0, -0x19B8
    ctx->r4 = ADD32(ctx->r4, -0X19B8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80144B28: addiu       $a0, $a0, -0x19B8
    ctx->r4 = ADD32(ctx->r4, -0X19B8);
    after_7:
    // 0x80144B2C: lw          $t8, 0x50($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X50);
    // 0x80144B30: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144B34: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80144B38: addiu       $a0, $s7, 0x8
    ctx->r4 = ADD32(ctx->r23, 0X8);
    // 0x80144B3C: addiu       $a1, $s7, 0xE
    ctx->r5 = ADD32(ctx->r23, 0XE);
    // 0x80144B40: jalr        $t9
    // 0x80144B44: sw          $v0, 0x38($t8)
    MEM_W(0X38, ctx->r24) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x80144B44: sw          $v0, 0x38($t8)
    MEM_W(0X38, ctx->r24) = ctx->r2;
    after_8:
    // 0x80144B48: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
L_80144B4C:
    // 0x80144B4C: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x80144B50: lw          $s1, 0x2C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X2C);
    // 0x80144B54: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x80144B58: lw          $s3, 0x34($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X34);
    // 0x80144B5C: lw          $s4, 0x38($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X38);
    // 0x80144B60: lw          $s5, 0x3C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X3C);
    // 0x80144B64: lw          $s6, 0x40($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X40);
    // 0x80144B68: lw          $s7, 0x44($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X44);
    // 0x80144B6C: lw          $fp, 0x48($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X48);
    // 0x80144B70: jr          $ra
    // 0x80144B74: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x80144B74: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_80144B78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144B78: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80144B7C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80144B80: lw          $a1, 0x34($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X34);
    // 0x80144B84: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80144B88: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x80144B8C: lw          $t6, 0x20($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X20);
    // 0x80144B90: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x80144B94: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80144B98: bne         $t6, $zero, L_80144BC0
    if (ctx->r14 != 0) {
        // 0x80144B9C: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80144BC0;
    }
    // 0x80144B9C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144BA0: lh          $t7, 0x0($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X0);
    // 0x80144BA4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80144BA8: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80144BAC: or          $t8, $t7, $at
    ctx->r24 = ctx->r15 | ctx->r1;
    // 0x80144BB0: jalr        $t9
    // 0x80144BB4: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80144BB4: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
    after_0:
    // 0x80144BB8: b           L_80144C04
    // 0x80144BBC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80144C04;
    // 0x80144BBC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80144BC0:
    // 0x80144BC0: lwc1        $f4, 0x54($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X54);
    // 0x80144BC4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80144BC8: ldc1        $f8, -0x7EE8($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X7EE8);
    // 0x80144BCC: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x80144BD0: addiu       $v0, $a1, 0x8
    ctx->r2 = ADD32(ctx->r5, 0X8);
    // 0x80144BD4: sub.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d - ctx->f8.d;
    // 0x80144BD8: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x80144BDC: swc1        $f16, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f16.u32l;
    // 0x80144BE0: lw          $t0, 0x18($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X18);
    // 0x80144BE4: sra         $t1, $t0, 8
    ctx->r9 = S32(SIGNED(ctx->r8) >> 8);
    // 0x80144BE8: sb          $t1, 0x1B($v1)
    MEM_B(0X1B, ctx->r3) = ctx->r9;
    // 0x80144BEC: lw          $t2, 0x18($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X18);
    // 0x80144BF0: addiu       $t3, $t2, -0x888
    ctx->r11 = ADD32(ctx->r10, -0X888);
    // 0x80144BF4: bgez        $t3, L_80144C00
    if (SIGNED(ctx->r11) >= 0) {
        // 0x80144BF8: sw          $t3, 0x18($v0)
        MEM_W(0X18, ctx->r2) = ctx->r11;
            goto L_80144C00;
    }
    // 0x80144BF8: sw          $t3, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r11;
    // 0x80144BFC: sw          $zero, 0x18($v0)
    MEM_W(0X18, ctx->r2) = 0;
L_80144C00:
    // 0x80144C00: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80144C04:
    // 0x80144C04: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80144C08: jr          $ra
    // 0x80144C0C: nop

    return;
    // 0x80144C0C: nop

;}
RECOMP_FUNC void func_80144C10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144C10: jr          $ra
    // 0x80144C14: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x80144C14: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_80144C18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144C18: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80144C1C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80144C20: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80144C24: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
    // 0x80144C28: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80144C2C: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x80144C30: jal         0x8014558C
    // 0x80144C34: sw          $t6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r14;
    func_8014558C(rdram, ctx);
        goto after_0;
    // 0x80144C34: sw          $t6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r14;
    after_0:
    // 0x80144C38: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80144C3C: bne         $v0, $zero, L_80144C68
    if (ctx->r2 != 0) {
        // 0x80144C40: lw          $a3, 0x20($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X20);
            goto L_80144C68;
    }
    // 0x80144C40: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x80144C44: lw          $t7, 0x30($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X30);
    // 0x80144C48: lw          $t8, 0x18($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X18);
    // 0x80144C4C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144C50: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80144C54: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x80144C58: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x80144C5C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80144C60: jalr        $t9
    // 0x80144C64: sh          $t7, 0x0($t8)
    MEM_H(0X0, ctx->r24) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80144C64: sh          $t7, 0x0($t8)
    MEM_H(0X0, ctx->r24) = ctx->r15;
    after_1:
L_80144C68:
    // 0x80144C68: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80144C6C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80144C70: jr          $ra
    // 0x80144C74: nop

    return;
    // 0x80144C74: nop

;}
RECOMP_FUNC void func_80144C78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144C78: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80144C7C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80144C80: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x80144C84: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80144C88: jal         0x801469F4
    // 0x80144C8C: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    func_801469F4(rdram, ctx);
        goto after_0;
    // 0x80144C8C: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    after_0:
    // 0x80144C90: lw          $t7, 0x1C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X1C);
    // 0x80144C94: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x80144C98: addiu       $t9, $t9, 0x938
    ctx->r25 = ADD32(ctx->r25, 0X938);
    // 0x80144C9C: lw          $a0, 0x4($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X4);
    // 0x80144CA0: beql        $a0, $zero, L_80144CB4
    if (ctx->r4 == 0) {
        // 0x80144CA4: lw          $t8, 0x1C($sp)
        ctx->r24 = MEM_W(ctx->r29, 0X1C);
            goto L_80144CB4;
    }
    goto skip_0;
    // 0x80144CA4: lw          $t8, 0x1C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80144CA8: jalr        $t9
    // 0x80144CAC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80144CAC: nop

    after_1:
    // 0x80144CB0: lw          $t8, 0x1C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X1C);
L_80144CB4:
    // 0x80144CB4: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80144CB8: lw          $t9, 0x10($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X10);
    // 0x80144CBC: jalr        $t9
    // 0x80144CC0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80144CC0: nop

    after_2:
    // 0x80144CC4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80144CC8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80144CCC: jr          $ra
    // 0x80144CD0: nop

    return;
    // 0x80144CD0: nop

;}
RECOMP_FUNC void func_80144CD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144CD4: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80144CD8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80144CDC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80144CE0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80144CE4: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80144CE8: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x80144CEC: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80144CF0: sw          $v0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r2;
    // 0x80144CF4: jal         0x801454CC
    // 0x80144CF8: sw          $v1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r3;
    func_801454CC(rdram, ctx);
        goto after_0;
    // 0x80144CF8: sw          $v1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r3;
    after_0:
    // 0x80144CFC: lw          $v0, 0x4C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X4C);
    // 0x80144D00: lw          $v1, 0x44($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X44);
    // 0x80144D04: ori         $t7, $zero, 0xFFFF
    ctx->r15 = 0 | 0XFFFF;
    // 0x80144D08: lwc1        $f6, 0x4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80144D0C: lwc1        $f4, 0x50($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X50);
    // 0x80144D10: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80144D14: sub.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x80144D18: swc1        $f10, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f10.u32l;
    // 0x80144D1C: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80144D20: lwc1        $f6, 0x54($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X54);
    // 0x80144D24: mul.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80144D28: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80144D2C: swc1        $f10, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f10.u32l;
    // 0x80144D30: lwc1        $f6, 0xC($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80144D34: lwc1        $f4, 0x58($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80144D38: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80144D3C: sub.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x80144D40: swc1        $f10, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f10.u32l;
    // 0x80144D44: lw          $t6, 0xC($s0)
    ctx->r14 = MEM_W(ctx->r16, 0XC);
    // 0x80144D48: subu        $a0, $t7, $t6
    ctx->r4 = SUB32(ctx->r15, ctx->r14);
    // 0x80144D4C: jal         0x80097330
    // 0x80144D50: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_1;
    // 0x80144D50: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_1:
    // 0x80144D54: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x80144D58: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x80144D5C: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x80144D60: cvt.s.w     $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80144D64: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80144D68: lw          $t8, 0xC($s0)
    ctx->r24 = MEM_W(ctx->r16, 0XC);
    // 0x80144D6C: ori         $t9, $zero, 0xFFFF
    ctx->r25 = 0 | 0XFFFF;
    // 0x80144D70: subu        $a0, $t9, $t8
    ctx->r4 = SUB32(ctx->r25, ctx->r24);
    // 0x80144D74: cvt.d.s     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.d = CVT_D_S(ctx->f4.fl);
    // 0x80144D78: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x80144D7C: div.d       $f6, $f8, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f6.d = DIV_D(ctx->f8.d, ctx->f10.d);
    // 0x80144D80: cvt.s.d     $f16, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f16.fl = CVT_S_D(ctx->f6.d);
    // 0x80144D84: jal         0x800A3A50
    // 0x80144D88: swc1        $f16, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f16.u32l;
    func_800A3A50(rdram, ctx);
        goto after_2;
    // 0x80144D88: swc1        $f16, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f16.u32l;
    after_2:
    // 0x80144D8C: lwc1        $f0, 0x4($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80144D90: lwc1        $f2, 0x38($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80144D94: lwc1        $f16, 0x24($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X24);
    // 0x80144D98: neg.s       $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = -ctx->f0.fl;
    // 0x80144D9C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80144DA0: c.lt.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl < ctx->f4.fl;
    // 0x80144DA4: nop

    // 0x80144DA8: bc1t        L_80144DC0
    if (c1cs) {
        // 0x80144DAC: nop
    
            goto L_80144DC0;
    }
    // 0x80144DAC: nop

    // 0x80144DB0: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x80144DB4: nop

    // 0x80144DB8: bc1fl       L_80144DCC
    if (!c1cs) {
        // 0x80144DBC: mtc1        $v0, $f8
        ctx->f8.u32l = ctx->r2;
            goto L_80144DCC;
    }
    goto skip_0;
    // 0x80144DBC: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    skip_0:
L_80144DC0:
    // 0x80144DC0: b           L_80144E9C
    // 0x80144DC4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_80144E9C;
    // 0x80144DC4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80144DC8: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
L_80144DCC:
    // 0x80144DCC: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x80144DD0: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x80144DD4: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80144DD8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80144DDC: lwc1        $f18, 0x3C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x80144DE0: lwc1        $f12, 0x0($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80144DE4: cvt.d.s     $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f6.d = CVT_D_S(ctx->f10.fl);
    // 0x80144DE8: div.d       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f4.d); 
    ctx->f8.d = DIV_D(ctx->f6.d, ctx->f4.d);
    // 0x80144DEC: lwc1        $f6, 0x34($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80144DF0: cvt.s.d     $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f2.fl = CVT_S_D(ctx->f8.d);
    // 0x80144DF4: neg.s       $f8, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = -ctx->f12.fl;
    // 0x80144DF8: mul.s       $f10, $f18, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f2.fl);
    // 0x80144DFC: nop

    // 0x80144E00: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80144E04: add.s       $f0, $f10, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80144E08: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x80144E0C: nop

    // 0x80144E10: bc1t        L_80144E28
    if (c1cs) {
        // 0x80144E14: nop
    
            goto L_80144E28;
    }
    // 0x80144E14: nop

    // 0x80144E18: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x80144E1C: nop

    // 0x80144E20: bc1fl       L_80144E34
    if (!c1cs) {
        // 0x80144E24: neg.s       $f6, $f18
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = -ctx->f18.fl;
            goto L_80144E34;
    }
    goto skip_1;
    // 0x80144E24: neg.s       $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = -ctx->f18.fl;
    skip_1:
L_80144E28:
    // 0x80144E28: b           L_80144E9C
    // 0x80144E2C: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_80144E9C;
    // 0x80144E2C: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80144E30: neg.s       $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = -ctx->f18.fl;
L_80144E34:
    // 0x80144E34: lwc1        $f4, 0x34($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80144E38: mul.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80144E3C: lwc1        $f14, 0x8($s0)
    ctx->f14.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80144E40: mul.s       $f8, $f4, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80144E44: neg.s       $f6, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = -ctx->f14.fl;
    // 0x80144E48: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80144E4C: c.lt.s      $f12, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f12.fl < ctx->f6.fl;
    // 0x80144E50: nop

    // 0x80144E54: bc1t        L_80144E6C
    if (c1cs) {
        // 0x80144E58: nop
    
            goto L_80144E6C;
    }
    // 0x80144E58: nop

    // 0x80144E5C: c.lt.s      $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f14.fl < ctx->f12.fl;
    // 0x80144E60: nop

    // 0x80144E64: bc1fl       L_80144E78
    if (!c1cs) {
        // 0x80144E68: mtc1        $zero, $f11
        ctx->f_odd[(11 - 1) * 2] = 0;
            goto L_80144E78;
    }
    goto skip_2;
    // 0x80144E68: mtc1        $zero, $f11
    ctx->f_odd[(11 - 1) * 2] = 0;
    skip_2:
L_80144E6C:
    // 0x80144E6C: b           L_80144E9C
    // 0x80144E70: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_80144E9C;
    // 0x80144E70: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80144E74: mtc1        $zero, $f11
    ctx->f_odd[(11 - 1) * 2] = 0;
L_80144E78:
    // 0x80144E78: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80144E7C: cvt.d.s     $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.d = CVT_D_S(ctx->f0.fl);
    // 0x80144E80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80144E84: c.lt.d      $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f4.d < ctx->f10.d;
    // 0x80144E88: nop

    // 0x80144E8C: bc1f        L_80144E9C
    if (!c1cs) {
        // 0x80144E90: nop
    
            goto L_80144E9C;
    }
    // 0x80144E90: nop

    // 0x80144E94: b           L_80144E9C
    // 0x80144E98: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80144E9C;
    // 0x80144E98: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80144E9C:
    // 0x80144E9C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80144EA0: jr          $ra
    // 0x80144EA4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x80144EA4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_80144EA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144EA8: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80144EAC: lw          $t6, -0x53DC($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53DC);
    // 0x80144EB0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80144EB4: lw          $v0, 0x0($t6)
    ctx->r2 = MEM_W(ctx->r14, 0X0);
    // 0x80144EB8: jr          $ra
    // 0x80144EBC: andi        $v0, $v0, 0x8
    ctx->r2 = ctx->r2 & 0X8;
    return;
    // 0x80144EBC: andi        $v0, $v0, 0x8
    ctx->r2 = ctx->r2 & 0X8;
;}
RECOMP_FUNC void func_80144EC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144EC0: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80144EC4: lw          $t6, -0x53DC($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53DC);
    // 0x80144EC8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80144ECC: lui         $at, 0x20
    ctx->r1 = S32(0X20 << 16);
    // 0x80144ED0: lw          $v0, 0x0($t6)
    ctx->r2 = MEM_W(ctx->r14, 0X0);
    // 0x80144ED4: jr          $ra
    // 0x80144ED8: and         $v0, $v0, $at
    ctx->r2 = ctx->r2 & ctx->r1;
    return;
    // 0x80144ED8: and         $v0, $v0, $at
    ctx->r2 = ctx->r2 & ctx->r1;
;}
RECOMP_FUNC void func_80144EDC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144EDC: jr          $ra
    // 0x80144EE0: nop

    return;
    // 0x80144EE0: nop

;}
RECOMP_FUNC void func_80144EE4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80144EE4: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80144EE8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80144EEC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80144EF0: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    // 0x80144EF4: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x80144EF8: sw          $a3, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r7;
    // 0x80144EFC: lw          $t6, 0x70($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X70);
    // 0x80144F00: lhu         $a1, 0x46($sp)
    ctx->r5 = MEM_HU(ctx->r29, 0X46);
    // 0x80144F04: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80144F08: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80144F0C: lw          $a0, -0x540C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X540C);
    // 0x80144F10: sw          $t6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r14;
    // 0x80144F14: jal         0x80002410
    // 0x80144F18: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    object_createAndSetChild(rdram, ctx);
        goto after_0;
    // 0x80144F18: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    after_0:
    // 0x80144F1C: beq         $v0, $zero, L_8014501C
    if (ctx->r2 == 0) {
        // 0x80144F20: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8014501C;
    }
    // 0x80144F20: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80144F24: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80144F28: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x80144F2C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80144F30: addiu       $a2, $zero, 0x20
    ctx->r6 = ADD32(0, 0X20);
    // 0x80144F34: addiu       $a3, $zero, 0xF
    ctx->r7 = ADD32(0, 0XF);
    // 0x80144F38: jalr        $t9
    // 0x80144F3C: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80144F3C: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    after_1:
    // 0x80144F40: bne         $v0, $zero, L_80144F5C
    if (ctx->r2 != 0) {
        // 0x80144F44: lw          $a0, 0x28($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X28);
            goto L_80144F5C;
    }
    // 0x80144F44: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x80144F48: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80144F4C: jalr        $t9
    // 0x80144F50: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80144F50: nop

    after_2:
    // 0x80144F54: b           L_80145020
    // 0x80144F58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80145020;
    // 0x80144F58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80144F5C:
    // 0x80144F5C: lw          $t7, 0x20($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X20);
    // 0x80144F60: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80144F64: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80144F68: sh          $t7, 0x10($v0)
    MEM_H(0X10, ctx->r2) = ctx->r15;
    // 0x80144F6C: lhu         $t8, 0x4A($sp)
    ctx->r24 = MEM_HU(ctx->r29, 0X4A);
    // 0x80144F70: sh          $t8, 0x18($v0)
    MEM_H(0X18, ctx->r2) = ctx->r24;
    // 0x80144F74: lhu         $t0, 0x4E($sp)
    ctx->r8 = MEM_HU(ctx->r29, 0X4E);
    // 0x80144F78: sh          $t0, 0x14($v0)
    MEM_H(0X14, ctx->r2) = ctx->r8;
    // 0x80144F7C: lhu         $t1, 0x52($sp)
    ctx->r9 = MEM_HU(ctx->r29, 0X52);
    // 0x80144F80: sh          $t1, 0x16($v0)
    MEM_H(0X16, ctx->r2) = ctx->r9;
    // 0x80144F84: lh          $t2, 0x56($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X56);
    // 0x80144F88: mtc1        $t2, $f4
    ctx->f4.u32l = ctx->r10;
    // 0x80144F8C: nop

    // 0x80144F90: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80144F94: swc1        $f6, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f6.u32l;
    // 0x80144F98: lh          $t3, 0x5A($sp)
    ctx->r11 = MEM_H(ctx->r29, 0X5A);
    // 0x80144F9C: mtc1        $t3, $f8
    ctx->f8.u32l = ctx->r11;
    // 0x80144FA0: nop

    // 0x80144FA4: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80144FA8: swc1        $f10, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f10.u32l;
    // 0x80144FAC: lh          $t4, 0x5E($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X5E);
    // 0x80144FB0: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x80144FB4: nop

    // 0x80144FB8: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80144FBC: swc1        $f18, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f18.u32l;
    // 0x80144FC0: lw          $t5, 0x6C($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X6C);
    // 0x80144FC4: beql        $t5, $zero, L_80144FE4
    if (ctx->r13 == 0) {
        // 0x80144FC8: lw          $t8, 0x38($sp)
        ctx->r24 = MEM_W(ctx->r29, 0X38);
            goto L_80144FE4;
    }
    goto skip_0;
    // 0x80144FC8: lw          $t8, 0x38($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X38);
    skip_0:
    // 0x80144FCC: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x80144FD0: ori         $t9, $t6, 0x2000
    ctx->r25 = ctx->r14 | 0X2000;
    // 0x80144FD4: sh          $t9, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r25;
    // 0x80144FD8: lw          $t7, 0x6C($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X6C);
    // 0x80144FDC: sw          $t7, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->r15;
    // 0x80144FE0: lw          $t8, 0x38($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X38);
L_80144FE4:
    // 0x80144FE4: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x80144FE8: addiu       $t9, $t9, -0x12E8
    ctx->r25 = ADD32(ctx->r25, -0X12E8);
    // 0x80144FEC: lhu         $t0, 0x0($t8)
    ctx->r8 = MEM_HU(ctx->r24, 0X0);
    // 0x80144FF0: andi        $t1, $t0, 0x4
    ctx->r9 = ctx->r8 & 0X4;
    // 0x80144FF4: beq         $t1, $zero, L_80145014
    if (ctx->r9 == 0) {
        // 0x80144FF8: nop
    
            goto L_80145014;
    }
    // 0x80144FF8: nop

    // 0x80144FFC: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x80145000: addiu       $t9, $t9, -0x12E8
    ctx->r25 = ADD32(ctx->r25, -0X12E8);
    // 0x80145004: jalr        $t9
    // 0x80145008: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80145008: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_3:
    // 0x8014500C: b           L_80145020
    // 0x80145010: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80145020;
    // 0x80145010: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80145014:
    // 0x80145014: jalr        $t9
    // 0x80145018: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80145018: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_4:
L_8014501C:
    // 0x8014501C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80145020:
    // 0x80145020: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80145024: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x80145028: jr          $ra
    // 0x8014502C: nop

    return;
    // 0x8014502C: nop

;}
RECOMP_FUNC void func_80145030(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145030: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80145034: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    // 0x80145038: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x8014503C: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80145040: andi        $v0, $v0, 0x7FF
    ctx->r2 = ctx->r2 & 0X7FF;
    // 0x80145044: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80145048: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8014504C: slti        $at, $v0, 0xA9
    ctx->r1 = SIGNED(ctx->r2) < 0XA9 ? 1 : 0;
    // 0x80145050: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80145054: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x80145058: sw          $a3, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r7;
    // 0x8014505C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80145060: bne         $at, $zero, L_80145084
    if (ctx->r1 != 0) {
        // 0x80145064: or          $s0, $a1, $zero
        ctx->r16 = ctx->r5 | 0;
            goto L_80145084;
    }
    // 0x80145064: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80145068: slti        $at, $v0, 0xFD
    ctx->r1 = SIGNED(ctx->r2) < 0XFD ? 1 : 0;
    // 0x8014506C: beql        $at, $zero, L_80145088
    if (ctx->r1 == 0) {
        // 0x80145070: addiu       $at, $zero, 0x1D9
        ctx->r1 = ADD32(0, 0X1D9);
            goto L_80145088;
    }
    goto skip_0;
    // 0x80145070: addiu       $at, $zero, 0x1D9
    ctx->r1 = ADD32(0, 0X1D9);
    skip_0:
    // 0x80145074: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80145078: jal         0x80002410
    // 0x8014507C: lw          $a0, -0x540C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X540C);
    object_createAndSetChild(rdram, ctx);
        goto after_0;
    // 0x8014507C: lw          $a0, -0x540C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X540C);
    after_0:
    // 0x80145080: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
L_80145084:
    // 0x80145084: addiu       $at, $zero, 0x1D9
    ctx->r1 = ADD32(0, 0X1D9);
L_80145088:
    // 0x80145088: beq         $s0, $at, L_8014509C
    if (ctx->r16 == ctx->r1) {
        // 0x8014508C: or          $a1, $s0, $zero
        ctx->r5 = ctx->r16 | 0;
            goto L_8014509C;
    }
    // 0x8014508C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80145090: addiu       $at, $zero, 0x1DA
    ctx->r1 = ADD32(0, 0X1DA);
    // 0x80145094: bne         $s0, $at, L_801450AC
    if (ctx->r16 != ctx->r1) {
        // 0x80145098: nop
    
            goto L_801450AC;
    }
    // 0x80145098: nop

L_8014509C:
    // 0x8014509C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801450A0: jal         0x80002410
    // 0x801450A4: lw          $a0, -0x5434($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5434);
    object_createAndSetChild(rdram, ctx);
        goto after_1;
    // 0x801450A4: lw          $a0, -0x5434($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5434);
    after_1:
    // 0x801450A8: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
L_801450AC:
    // 0x801450AC: beq         $a0, $zero, L_80145140
    if (ctx->r4 == 0) {
        // 0x801450B0: or          $a1, $zero, $zero
        ctx->r5 = 0 | 0;
            goto L_80145140;
    }
    // 0x801450B0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801450B4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801450B8: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x801450BC: addiu       $a2, $zero, 0x20
    ctx->r6 = ADD32(0, 0X20);
    // 0x801450C0: addiu       $a3, $zero, 0xF
    ctx->r7 = ADD32(0, 0XF);
    // 0x801450C4: jalr        $t9
    // 0x801450C8: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801450C8: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    after_2:
    // 0x801450CC: bne         $v0, $zero, L_801450E8
    if (ctx->r2 != 0) {
        // 0x801450D0: lw          $a0, 0x20($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X20);
            goto L_801450E8;
    }
    // 0x801450D0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x801450D4: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x801450D8: jalr        $t9
    // 0x801450DC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801450DC: nop

    after_3:
    // 0x801450E0: b           L_80145144
    // 0x801450E4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80145144;
    // 0x801450E4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801450E8:
    // 0x801450E8: sh          $s0, 0x10($v0)
    MEM_H(0X10, ctx->r2) = ctx->r16;
    // 0x801450EC: lhu         $t6, 0x4A($sp)
    ctx->r14 = MEM_HU(ctx->r29, 0X4A);
    // 0x801450F0: sh          $t6, 0x18($v0)
    MEM_H(0X18, ctx->r2) = ctx->r14;
    // 0x801450F4: lhu         $t7, 0x4E($sp)
    ctx->r15 = MEM_HU(ctx->r29, 0X4E);
    // 0x801450F8: sh          $t7, 0x14($v0)
    MEM_H(0X14, ctx->r2) = ctx->r15;
    // 0x801450FC: lhu         $t8, 0x52($sp)
    ctx->r24 = MEM_HU(ctx->r29, 0X52);
    // 0x80145100: sh          $t8, 0x16($v0)
    MEM_H(0X16, ctx->r2) = ctx->r24;
    // 0x80145104: lh          $t0, 0x56($sp)
    ctx->r8 = MEM_H(ctx->r29, 0X56);
    // 0x80145108: mtc1        $t0, $f4
    ctx->f4.u32l = ctx->r8;
    // 0x8014510C: nop

    // 0x80145110: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80145114: swc1        $f6, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f6.u32l;
    // 0x80145118: lh          $t1, 0x5A($sp)
    ctx->r9 = MEM_H(ctx->r29, 0X5A);
    // 0x8014511C: mtc1        $t1, $f8
    ctx->f8.u32l = ctx->r9;
    // 0x80145120: nop

    // 0x80145124: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80145128: swc1        $f10, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f10.u32l;
    // 0x8014512C: lh          $t2, 0x5E($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X5E);
    // 0x80145130: mtc1        $t2, $f16
    ctx->f16.u32l = ctx->r10;
    // 0x80145134: nop

    // 0x80145138: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8014513C: swc1        $f18, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f18.u32l;
L_80145140:
    // 0x80145140: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80145144:
    // 0x80145144: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80145148: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8014514C: jr          $ra
    // 0x80145150: nop

    return;
    // 0x80145150: nop

    // 0x80145154: jr          $ra
    // 0x80145158: nop

    return;
    // 0x80145158: nop

;}
RECOMP_FUNC void func_8014515C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014515C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80145160: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80145164: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80145168: lw          $v0, 0x14($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X14);
    // 0x8014516C: lw          $a0, 0x70($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X70);
    // 0x80145170: sw          $zero, 0x4C($v0)
    MEM_W(0X4C, ctx->r2) = 0;
    // 0x80145174: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    // 0x80145178: jal         0x80145418
    // 0x8014517C: addiu       $v0, $v0, 0x34
    ctx->r2 = ADD32(ctx->r2, 0X34);
    func_80145418(rdram, ctx);
        goto after_0;
    // 0x8014517C: addiu       $v0, $v0, 0x34
    ctx->r2 = ADD32(ctx->r2, 0X34);
    after_0:
    // 0x80145180: jal         0x80002CE0
    // 0x80145184: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    object_destroyChildrenAndModelInfo(rdram, ctx);
        goto after_1;
    // 0x80145184: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    after_1:
    // 0x80145188: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8014518C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80145190: jr          $ra
    // 0x80145194: nop

    return;
    // 0x80145194: nop

;}
RECOMP_FUNC void func_80145198(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145198: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8014519C: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    // 0x801451A0: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x801451A4: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x801451A8: lwc1        $f4, 0x34($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X34);
    // 0x801451AC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801451B0: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x801451B4: addiu       $a1, $sp, 0x18
    ctx->r5 = ADD32(ctx->r29, 0X18);
    // 0x801451B8: swc1        $f12, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f12.u32l;
    // 0x801451BC: swc1        $f14, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f14.u32l;
    // 0x801451C0: jal         0x8014314C
    // 0x801451C4: swc1        $f4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f4.u32l;
    func_8014314C(rdram, ctx);
        goto after_0;
    // 0x801451C4: swc1        $f4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x801451C8: beql        $v0, $zero, L_801451EC
    if (ctx->r2 == 0) {
        // 0x801451CC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_801451EC;
    }
    goto skip_0;
    // 0x801451CC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x801451D0: jal         0x80145420
    // 0x801451D4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    func_80145420(rdram, ctx);
        goto after_1;
    // 0x801451D4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    after_1:
    // 0x801451D8: bnel        $v0, $zero, L_801451EC
    if (ctx->r2 != 0) {
        // 0x801451DC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_801451EC;
    }
    goto skip_1;
    // 0x801451DC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x801451E0: b           L_801451EC
    // 0x801451E4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_801451EC;
    // 0x801451E4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801451E8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801451EC:
    // 0x801451EC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801451F0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x801451F4: jr          $ra
    // 0x801451F8: nop

    return;
    // 0x801451F8: nop

;}
RECOMP_FUNC void func_801451FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801451FC: addiu       $sp, $sp, -0x1E0
    ctx->r29 = ADD32(ctx->r29, -0X1E0);
    // 0x80145200: sw          $a2, 0x1E8($sp)
    MEM_W(0X1E8, ctx->r29) = ctx->r6;
    // 0x80145204: lwc1        $f4, 0x1E8($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1E8);
    // 0x80145208: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8014520C: sw          $a3, 0x1EC($sp)
    MEM_W(0X1EC, ctx->r29) = ctx->r7;
    // 0x80145210: swc1        $f12, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f12.u32l;
    // 0x80145214: swc1        $f14, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f14.u32l;
    // 0x80145218: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x8014521C: addiu       $a1, $sp, 0x3C
    ctx->r5 = ADD32(ctx->r29, 0X3C);
    // 0x80145220: jal         0x8003E5D8
    // 0x80145224: swc1        $f4, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f4.u32l;
    func_8003E5D8(rdram, ctx);
        goto after_0;
    // 0x80145224: swc1        $f4, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x80145228: lw          $v0, 0x3C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X3C);
    // 0x8014522C: lui         $at, 0xC0
    ctx->r1 = S32(0XC0 << 16);
    // 0x80145230: srl         $t6, $v0, 30
    ctx->r14 = S32(U32(ctx->r2) >> 30);
    // 0x80145234: beq         $t6, $zero, L_80145244
    if (ctx->r14 == 0) {
        // 0x80145238: and         $t7, $v0, $at
        ctx->r15 = ctx->r2 & ctx->r1;
            goto L_80145244;
    }
    // 0x80145238: and         $t7, $v0, $at
    ctx->r15 = ctx->r2 & ctx->r1;
    // 0x8014523C: b           L_801452D4
    // 0x80145240: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801452D4;
    // 0x80145240: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80145244:
    // 0x80145244: bne         $t7, $zero, L_80145254
    if (ctx->r15 != 0) {
        // 0x80145248: andi        $t8, $v0, 0x8000
        ctx->r24 = ctx->r2 & 0X8000;
            goto L_80145254;
    }
    // 0x80145248: andi        $t8, $v0, 0x8000
    ctx->r24 = ctx->r2 & 0X8000;
    // 0x8014524C: b           L_801452D4
    // 0x80145250: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801452D4;
    // 0x80145250: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80145254:
    // 0x80145254: beq         $t8, $zero, L_80145264
    if (ctx->r24 == 0) {
        // 0x80145258: lui         $t9, 0x801D
        ctx->r25 = S32(0X801D << 16);
            goto L_80145264;
    }
    // 0x80145258: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x8014525C: b           L_80145268
    // 0x80145260: lwc1        $f12, 0x80($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X80);
        goto L_80145268;
    // 0x80145260: lwc1        $f12, 0x80($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X80);
L_80145264:
    // 0x80145264: lwc1        $f12, 0xE0($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0XE0);
L_80145268:
    // 0x80145268: lw          $t9, -0x53E0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X53E0);
    // 0x8014526C: lui         $at, 0x402E
    ctx->r1 = S32(0X402E << 16);
    // 0x80145270: mtc1        $at, $f15
    ctx->f_odd[(15 - 1) * 2] = ctx->r1;
    // 0x80145274: lwc1        $f6, 0x54($t9)
    ctx->f6.u32l = MEM_W(ctx->r25, 0X54);
    // 0x80145278: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x8014527C: cvt.d.s     $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.d = CVT_D_S(ctx->f12.fl);
    // 0x80145280: cvt.d.s     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f0.d = CVT_D_S(ctx->f6.fl);
    // 0x80145284: add.d       $f8, $f0, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f14.d); 
    ctx->f8.d = ctx->f0.d + ctx->f14.d;
    // 0x80145288: c.lt.d      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.d < ctx->f2.d;
    // 0x8014528C: nop

    // 0x80145290: bc1fl       L_801452A4
    if (!c1cs) {
        // 0x80145294: sub.d       $f10, $f0, $f14
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f14.d); 
    ctx->f10.d = ctx->f0.d - ctx->f14.d;
            goto L_801452A4;
    }
    goto skip_0;
    // 0x80145294: sub.d       $f10, $f0, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f14.d); 
    ctx->f10.d = ctx->f0.d - ctx->f14.d;
    skip_0:
    // 0x80145298: b           L_801452D4
    // 0x8014529C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801452D4;
    // 0x8014529C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801452A0: sub.d       $f10, $f0, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f14.d); 
    ctx->f10.d = ctx->f0.d - ctx->f14.d;
L_801452A4:
    // 0x801452A4: c.lt.d      $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f2.d < ctx->f10.d;
    // 0x801452A8: nop

    // 0x801452AC: bc1fl       L_801452C0
    if (!c1cs) {
        // 0x801452B0: trunc.w.s   $f16, $f12
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    ctx->f16.u32l = TRUNC_W_S(ctx->f12.fl);
            goto L_801452C0;
    }
    goto skip_1;
    // 0x801452B0: trunc.w.s   $f16, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    ctx->f16.u32l = TRUNC_W_S(ctx->f12.fl);
    skip_1:
    // 0x801452B4: b           L_801452D4
    // 0x801452B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801452D4;
    // 0x801452B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801452BC: trunc.w.s   $f16, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    ctx->f16.u32l = TRUNC_W_S(ctx->f12.fl);
L_801452C0:
    // 0x801452C0: lw          $t2, 0x1EC($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X1EC);
    // 0x801452C4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801452C8: mfc1        $t1, $f16
    ctx->r9 = (int32_t)ctx->f16.u32l;
    // 0x801452CC: nop

    // 0x801452D0: sh          $t1, 0x0($t2)
    MEM_H(0X0, ctx->r10) = ctx->r9;
L_801452D4:
    // 0x801452D4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801452D8: addiu       $sp, $sp, 0x1E0
    ctx->r29 = ADD32(ctx->r29, 0X1E0);
    // 0x801452DC: jr          $ra
    // 0x801452E0: nop

    return;
    // 0x801452E0: nop

;}
RECOMP_FUNC void func_801452E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801452E4: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801452E8: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x801452EC: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x801452F0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801452F4: swc1        $f12, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f12.u32l;
    // 0x801452F8: swc1        $f14, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f14.u32l;
    // 0x801452FC: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x80145300: addiu       $a1, $sp, 0x24
    ctx->r5 = ADD32(ctx->r29, 0X24);
    // 0x80145304: jal         0x8004007C
    // 0x80145308: swc1        $f4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f4.u32l;
    func_8004007C(rdram, ctx);
        goto after_0;
    // 0x80145308: swc1        $f4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x8014530C: bgez        $v0, L_80145320
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80145310: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80145320;
    }
    // 0x80145310: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80145314: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80145318: b           L_80145350
    // 0x8014531C: nop

        goto L_80145350;
    // 0x8014531C: nop

L_80145320:
    // 0x80145320: lw          $t6, 0x34($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X34);
    // 0x80145324: lw          $t7, 0x28($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X28);
    // 0x80145328: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8014532C: bnel        $t6, $zero, L_8014534C
    if (ctx->r14 != 0) {
        // 0x80145330: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_8014534C;
    }
    goto skip_0;
    // 0x80145330: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_0:
    // 0x80145334: bnel        $t7, $zero, L_8014534C
    if (ctx->r15 != 0) {
        // 0x80145338: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_8014534C;
    }
    goto skip_1;
    // 0x80145338: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_1:
    // 0x8014533C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80145340: b           L_80145350
    // 0x80145344: nop

        goto L_80145350;
    // 0x80145344: nop

    // 0x80145348: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
L_8014534C:
    // 0x8014534C: nop

L_80145350:
    // 0x80145350: jr          $ra
    // 0x80145354: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x80145354: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_80145358(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145358: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8014535C: lw          $v0, -0x53E0($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53E0);
    // 0x80145360: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80145364: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    // 0x80145368: lwc1        $f4, 0x50($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X50);
    // 0x8014536C: lwc1        $f6, 0x54($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X54);
    // 0x80145370: lwc1        $f8, 0x8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X8);
    // 0x80145374: sub.s       $f0, $f12, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f12.fl - ctx->f4.fl;
    // 0x80145378: lwc1        $f10, 0x58($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X58);
    // 0x8014537C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80145380: sub.s       $f2, $f14, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f14.fl - ctx->f6.fl;
    // 0x80145384: mul.s       $f18, $f0, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80145388: sub.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x8014538C: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80145390: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x80145394: mul.s       $f8, $f16, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x80145398: lwc1        $f18, 0xC($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XC);
    // 0x8014539C: mul.s       $f4, $f18, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x801453A0: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x801453A4: c.lt.s      $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f10.fl < ctx->f4.fl;
    // 0x801453A8: nop

    // 0x801453AC: bc1f        L_801453BC
    if (!c1cs) {
        // 0x801453B0: nop
    
            goto L_801453BC;
    }
    // 0x801453B0: nop

    // 0x801453B4: jr          $ra
    // 0x801453B8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x801453B8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801453BC:
    // 0x801453BC: jr          $ra
    // 0x801453C0: nop

    return;
    // 0x801453C0: nop

    // 0x801453C4: jr          $ra
    // 0x801453C8: nop

    return;
    // 0x801453C8: nop

;}
RECOMP_FUNC void func_801453CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801453CC: sll         $t6, $a1, 5
    ctx->r14 = S32(ctx->r5 << 5);
    // 0x801453D0: addu        $v0, $t6, $a0
    ctx->r2 = ADD32(ctx->r14, ctx->r4);
    // 0x801453D4: sltu        $at, $a0, $v0
    ctx->r1 = ctx->r4 < ctx->r2 ? 1 : 0;
    // 0x801453D8: beq         $at, $zero, L_8014540C
    if (ctx->r1 == 0) {
        // 0x801453DC: or          $v1, $a0, $zero
        ctx->r3 = ctx->r4 | 0;
            goto L_8014540C;
    }
    // 0x801453DC: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x801453E0: lhu         $a0, 0x2($v1)
    ctx->r4 = MEM_HU(ctx->r3, 0X2);
L_801453E4:
    // 0x801453E4: andi        $t7, $a0, 0x8000
    ctx->r15 = ctx->r4 & 0X8000;
    // 0x801453E8: bne         $t7, $zero, L_801453FC
    if (ctx->r15 != 0) {
        // 0x801453EC: ori         $t8, $a0, 0x8000
        ctx->r24 = ctx->r4 | 0X8000;
            goto L_801453FC;
    }
    // 0x801453EC: ori         $t8, $a0, 0x8000
    ctx->r24 = ctx->r4 | 0X8000;
    // 0x801453F0: sh          $t8, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r24;
    // 0x801453F4: jr          $ra
    // 0x801453F8: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x801453F8: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_801453FC:
    // 0x801453FC: addiu       $v1, $v1, 0x20
    ctx->r3 = ADD32(ctx->r3, 0X20);
    // 0x80145400: sltu        $at, $v1, $v0
    ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
    // 0x80145404: bnel        $at, $zero, L_801453E4
    if (ctx->r1 != 0) {
        // 0x80145408: lhu         $a0, 0x2($v1)
        ctx->r4 = MEM_HU(ctx->r3, 0X2);
            goto L_801453E4;
    }
    goto skip_0;
    // 0x80145408: lhu         $a0, 0x2($v1)
    ctx->r4 = MEM_HU(ctx->r3, 0X2);
    skip_0:
L_8014540C:
    // 0x8014540C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80145410: jr          $ra
    // 0x80145414: nop

    return;
    // 0x80145414: nop

;}
RECOMP_FUNC void func_80145418(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145418: jr          $ra
    // 0x8014541C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x8014541C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_80145420(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145420: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80145424: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x80145428: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8014542C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80145430: addiu       $t9, $t9, -0x12E8
    ctx->r25 = ADD32(ctx->r25, -0X12E8);
    // 0x80145434: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80145438: jalr        $t9
    // 0x8014543C: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8014543C: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_0:
    // 0x80145440: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80145444: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80145448: jr          $ra
    // 0x8014544C: nop

    return;
    // 0x8014544C: nop

;}
RECOMP_FUNC void func_80145450(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145450: bne         $a0, $zero, L_80145460
    if (ctx->r4 != 0) {
        // 0x80145454: andi        $t6, $a0, 0x1
        ctx->r14 = ctx->r4 & 0X1;
            goto L_80145460;
    }
    // 0x80145454: andi        $t6, $a0, 0x1
    ctx->r14 = ctx->r4 & 0X1;
    // 0x80145458: jr          $ra
    // 0x8014545C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x8014545C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80145460:
    // 0x80145460: beq         $t6, $zero, L_80145498
    if (ctx->r14 == 0) {
        // 0x80145464: addiu       $v0, $a0, 0x1
        ctx->r2 = ADD32(ctx->r4, 0X1);
            goto L_80145498;
    }
    // 0x80145464: addiu       $v0, $a0, 0x1
    ctx->r2 = ADD32(ctx->r4, 0X1);
    // 0x80145468: addiu       $v0, $a0, 0x1
    ctx->r2 = ADD32(ctx->r4, 0X1);
    // 0x8014546C: bgez        $v0, L_80145478
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80145470: addu        $at, $v0, $zero
        ctx->r1 = ADD32(ctx->r2, 0);
            goto L_80145478;
    }
    // 0x80145470: addu        $at, $v0, $zero
    ctx->r1 = ADD32(ctx->r2, 0);
    // 0x80145474: addiu       $at, $v0, 0x1
    ctx->r1 = ADD32(ctx->r2, 0X1);
L_80145478:
    // 0x80145478: sra         $v0, $at, 1
    ctx->r2 = S32(SIGNED(ctx->r1) >> 1);
    // 0x8014547C: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80145480: bgez        $v0, L_8014548C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80145484: addu        $at, $v0, $zero
        ctx->r1 = ADD32(ctx->r2, 0);
            goto L_8014548C;
    }
    // 0x80145484: addu        $at, $v0, $zero
    ctx->r1 = ADD32(ctx->r2, 0);
    // 0x80145488: addiu       $at, $v0, 0xFF
    ctx->r1 = ADD32(ctx->r2, 0XFF);
L_8014548C:
    // 0x8014548C: sra         $v0, $at, 8
    ctx->r2 = S32(SIGNED(ctx->r1) >> 8);
    // 0x80145490: jr          $ra
    // 0x80145494: nop

    return;
    // 0x80145494: nop

L_80145498:
    // 0x80145498: bgez        $v0, L_801454A4
    if (SIGNED(ctx->r2) >= 0) {
        // 0x8014549C: addu        $at, $v0, $zero
        ctx->r1 = ADD32(ctx->r2, 0);
            goto L_801454A4;
    }
    // 0x8014549C: addu        $at, $v0, $zero
    ctx->r1 = ADD32(ctx->r2, 0);
    // 0x801454A0: addiu       $at, $v0, 0x1
    ctx->r1 = ADD32(ctx->r2, 0X1);
L_801454A4:
    // 0x801454A4: sra         $v0, $at, 1
    ctx->r2 = S32(SIGNED(ctx->r1) >> 1);
    // 0x801454A8: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x801454AC: negu        $v0, $v0
    ctx->r2 = SUB32(0, ctx->r2);
    // 0x801454B0: bgez        $v0, L_801454BC
    if (SIGNED(ctx->r2) >= 0) {
        // 0x801454B4: addu        $at, $v0, $zero
        ctx->r1 = ADD32(ctx->r2, 0);
            goto L_801454BC;
    }
    // 0x801454B4: addu        $at, $v0, $zero
    ctx->r1 = ADD32(ctx->r2, 0);
    // 0x801454B8: addiu       $at, $v0, 0xFF
    ctx->r1 = ADD32(ctx->r2, 0XFF);
L_801454BC:
    // 0x801454BC: sra         $v0, $at, 8
    ctx->r2 = S32(SIGNED(ctx->r1) >> 8);
    // 0x801454C0: andi        $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 & 0XFFFF;
    // 0x801454C4: jr          $ra
    // 0x801454C8: nop

    return;
    // 0x801454C8: nop

;}
RECOMP_FUNC void func_801454CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801454CC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801454D0: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x801454D4: jr          $ra
    // 0x801454D8: nop

    return;
    // 0x801454D8: nop

;}
RECOMP_FUNC void func_801454DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801454DC: lhu         $v0, 0x14($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X14);
    // 0x801454E0: lhu         $v1, 0x16($a0)
    ctx->r3 = MEM_HU(ctx->r4, 0X16);
    // 0x801454E4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801454E8: bne         $v0, $zero, L_80145500
    if (ctx->r2 != 0) {
        // 0x801454EC: slt         $at, $v1, $v0
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r2) ? 1 : 0;
            goto L_80145500;
    }
    // 0x801454EC: slt         $at, $v1, $v0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x801454F0: bne         $v1, $zero, L_80145500
    if (ctx->r3 != 0) {
        // 0x801454F4: nop
    
            goto L_80145500;
    }
    // 0x801454F4: nop

    // 0x801454F8: jr          $ra
    // 0x801454FC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x801454FC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80145500:
    // 0x80145500: bne         $at, $zero, L_8014552C
    if (ctx->r1 != 0) {
        // 0x80145504: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_8014552C;
    }
    // 0x80145504: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80145508: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8014550C: lh          $a0, -0x54E0($a0)
    ctx->r4 = MEM_H(ctx->r4, -0X54E0);
    // 0x80145510: slt         $at, $a0, $v0
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80145514: bne         $at, $zero, L_80145554
    if (ctx->r1 != 0) {
        // 0x80145518: slt         $at, $a0, $v1
        ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r3) ? 1 : 0;
            goto L_80145554;
    }
    // 0x80145518: slt         $at, $a0, $v1
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x8014551C: beq         $at, $zero, L_80145554
    if (ctx->r1 == 0) {
        // 0x80145520: nop
    
            goto L_80145554;
    }
    // 0x80145520: nop

    // 0x80145524: jr          $ra
    // 0x80145528: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80145528: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8014552C:
    // 0x8014552C: lh          $a0, -0x54E0($a0)
    ctx->r4 = MEM_H(ctx->r4, -0X54E0);
    // 0x80145530: slt         $at, $a0, $v0
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80145534: bnel        $at, $zero, L_80145548
    if (ctx->r1 != 0) {
        // 0x80145538: slt         $at, $v1, $a0
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r4) ? 1 : 0;
            goto L_80145548;
    }
    goto skip_0;
    // 0x80145538: slt         $at, $v1, $a0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r4) ? 1 : 0;
    skip_0:
    // 0x8014553C: jr          $ra
    // 0x80145540: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80145540: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80145544: slt         $at, $v1, $a0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r4) ? 1 : 0;
L_80145548:
    // 0x80145548: bne         $at, $zero, L_80145554
    if (ctx->r1 != 0) {
        // 0x8014554C: nop
    
            goto L_80145554;
    }
    // 0x8014554C: nop

    // 0x80145550: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
L_80145554:
    // 0x80145554: jr          $ra
    // 0x80145558: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    return;
    // 0x80145558: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
;}
RECOMP_FUNC void func_8014555C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014555C: lhu         $t6, 0x16($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X16);
    // 0x80145560: lhu         $t8, 0x14($a0)
    ctx->r24 = MEM_HU(ctx->r4, 0X14);
    // 0x80145564: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80145568: addu        $t7, $t6, $a1
    ctx->r15 = ADD32(ctx->r14, ctx->r5);
    // 0x8014556C: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x80145570: slt         $at, $t7, $t9
    ctx->r1 = SIGNED(ctx->r15) < SIGNED(ctx->r25) ? 1 : 0;
    // 0x80145574: bne         $at, $zero, L_80145584
    if (ctx->r1 != 0) {
        // 0x80145578: nop
    
            goto L_80145584;
    }
    // 0x80145578: nop

    // 0x8014557C: jr          $ra
    // 0x80145580: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80145580: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80145584:
    // 0x80145584: jr          $ra
    // 0x80145588: nop

    return;
    // 0x80145588: nop

;}
RECOMP_FUNC void func_8014558C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014558C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80145590: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80145594: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80145598: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x8014559C: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
    // 0x801455A0: lw          $a2, 0xC($v1)
    ctx->r6 = MEM_W(ctx->r3, 0XC);
    // 0x801455A4: beq         $a2, $zero, L_801455EC
    if (ctx->r6 == 0) {
        // 0x801455A8: nop
    
            goto L_801455EC;
    }
    // 0x801455A8: nop

    // 0x801455AC: beq         $a2, $zero, L_801455DC
    if (ctx->r6 == 0) {
        // 0x801455B0: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_801455DC;
    }
    // 0x801455B0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x801455B4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801455B8: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x801455BC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801455C0: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x801455C4: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x801455C8: jalr        $t9
    // 0x801455CC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801455CC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_0:
    // 0x801455D0: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x801455D4: b           L_801455DC
    // 0x801455D8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_801455DC;
    // 0x801455D8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_801455DC:
    // 0x801455DC: beq         $v1, $zero, L_801455EC
    if (ctx->r3 == 0) {
        // 0x801455E0: nop
    
            goto L_801455EC;
    }
    // 0x801455E0: nop

    // 0x801455E4: b           L_801456B8
    // 0x801455E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_801456B8;
    // 0x801455E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801455EC:
    // 0x801455EC: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801455F0: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801455F4: lh          $t6, 0x28D0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X28D0);
    // 0x801455F8: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x801455FC: addiu       $v0, $zero, 0x32
    ctx->r2 = ADD32(0, 0X32);
    // 0x80145600: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80145604: lbu         $t7, -0x13E0($t7)
    ctx->r15 = MEM_BU(ctx->r15, -0X13E0);
    // 0x80145608: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x8014560C: sltiu       $at, $t8, 0x6
    ctx->r1 = ctx->r24 < 0X6 ? 1 : 0;
    // 0x80145610: beq         $at, $zero, L_8014565C
    if (ctx->r1 == 0) {
        // 0x80145614: sll         $t8, $t8, 2
        ctx->r24 = S32(ctx->r24 << 2);
            goto L_8014565C;
    }
    // 0x80145614: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80145618: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8014561C: addu        $at, $at, $t8
    gpr jr_addend_80145624 = ctx->r24;
    ctx->r1 = ADD32(ctx->r1, ctx->r24);
    // 0x80145620: lw          $t8, -0x7EE0($at)
    ctx->r24 = ADD32(ctx->r1, -0X7EE0);
    // 0x80145624: jr          $t8
    // 0x80145628: nop

    switch (jr_addend_80145624 >> 2) {
        case 0: goto L_8014562C; break;
        case 1: goto L_80145634; break;
        case 2: goto L_8014563C; break;
        case 3: goto L_80145644; break;
        case 4: goto L_8014564C; break;
        case 5: goto L_80145654; break;
        default: switch_error(__func__, 0x80145624, 0x80198120);
    }
    // 0x80145628: nop

L_8014562C:
    // 0x8014562C: b           L_8014565C
    // 0x80145630: addiu       $v0, $zero, 0x1E
    ctx->r2 = ADD32(0, 0X1E);
        goto L_8014565C;
    // 0x80145630: addiu       $v0, $zero, 0x1E
    ctx->r2 = ADD32(0, 0X1E);
L_80145634:
    // 0x80145634: b           L_8014565C
    // 0x80145638: addiu       $v0, $zero, 0x28
    ctx->r2 = ADD32(0, 0X28);
        goto L_8014565C;
    // 0x80145638: addiu       $v0, $zero, 0x28
    ctx->r2 = ADD32(0, 0X28);
L_8014563C:
    // 0x8014563C: b           L_8014565C
    // 0x80145640: addiu       $v0, $zero, 0x32
    ctx->r2 = ADD32(0, 0X32);
        goto L_8014565C;
    // 0x80145640: addiu       $v0, $zero, 0x32
    ctx->r2 = ADD32(0, 0X32);
L_80145644:
    // 0x80145644: b           L_8014565C
    // 0x80145648: addiu       $v0, $zero, 0x64
    ctx->r2 = ADD32(0, 0X64);
        goto L_8014565C;
    // 0x80145648: addiu       $v0, $zero, 0x64
    ctx->r2 = ADD32(0, 0X64);
L_8014564C:
    // 0x8014564C: b           L_8014565C
    // 0x80145650: addiu       $v0, $zero, 0xC8
    ctx->r2 = ADD32(0, 0XC8);
        goto L_8014565C;
    // 0x80145650: addiu       $v0, $zero, 0xC8
    ctx->r2 = ADD32(0, 0XC8);
L_80145654:
    // 0x80145654: b           L_8014565C
    // 0x80145658: addiu       $v0, $zero, 0x12C
    ctx->r2 = ADD32(0, 0X12C);
        goto L_8014565C;
    // 0x80145658: addiu       $v0, $zero, 0x12C
    ctx->r2 = ADD32(0, 0X12C);
L_8014565C:
    // 0x8014565C: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80145660: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x80145664: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x80145668: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8014566C: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80145670: lui         $at, 0x3FE8
    ctx->r1 = S32(0X3FE8 << 16);
    // 0x80145674: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x80145678: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x8014567C: lw          $a0, 0x2960($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X2960);
    // 0x80145680: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x80145684: lw          $a1, 0x24($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X24);
    // 0x80145688: mul.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = MUL_D(ctx->f8.d, ctx->f10.d);
    // 0x8014568C: nop

    // 0x80145690: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x80145694: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x80145698: mfc1        $a2, $f6
    ctx->r6 = (int32_t)ctx->f6.u32l;
    // 0x8014569C: jal         0x80145A1C
    // 0x801456A0: nop

    func_80145A1C(rdram, ctx);
        goto after_1;
    // 0x801456A0: nop

    after_1:
    // 0x801456A4: beql        $v0, $zero, L_801456B8
    if (ctx->r2 == 0) {
        // 0x801456A8: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_801456B8;
    }
    goto skip_0;
    // 0x801456A8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_0:
    // 0x801456AC: b           L_801456B8
    // 0x801456B0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801456B8;
    // 0x801456B0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801456B4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801456B8:
    // 0x801456B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801456BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801456C0: jr          $ra
    // 0x801456C4: nop

    return;
    // 0x801456C4: nop

;}
RECOMP_FUNC void func_801456C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801456C8: addiu       $sp, $sp, -0xD0
    ctx->r29 = ADD32(ctx->r29, -0XD0);
    // 0x801456CC: lw          $t6, 0xE0($sp)
    ctx->r14 = MEM_W(ctx->r29, 0XE0);
    // 0x801456D0: sw          $fp, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r30;
    // 0x801456D4: sw          $s6, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r22;
    // 0x801456D8: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x801456DC: slti        $at, $t6, 0xF
    ctx->r1 = SIGNED(ctx->r14) < 0XF ? 1 : 0;
    // 0x801456E0: or          $s6, $a0, $zero
    ctx->r22 = ctx->r4 | 0;
    // 0x801456E4: sw          $ra, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r31;
    // 0x801456E8: sw          $s7, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r23;
    // 0x801456EC: sw          $s5, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r21;
    // 0x801456F0: sw          $s4, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r20;
    // 0x801456F4: sw          $s3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r19;
    // 0x801456F8: sw          $s2, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r18;
    // 0x801456FC: sw          $s1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r17;
    // 0x80145700: sw          $s0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r16;
    // 0x80145704: sdc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X40, ctx->r29);
    // 0x80145708: sdc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X38, ctx->r29);
    // 0x8014570C: sdc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X30, ctx->r29);
    // 0x80145710: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x80145714: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80145718: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x8014571C: sw          $a2, 0xD8($sp)
    MEM_W(0XD8, ctx->r29) = ctx->r6;
    // 0x80145720: sw          $a3, 0xDC($sp)
    MEM_W(0XDC, ctx->r29) = ctx->r7;
    // 0x80145724: bne         $at, $zero, L_80145738
    if (ctx->r1 != 0) {
        // 0x80145728: or          $fp, $zero, $zero
        ctx->r30 = 0 | 0;
            goto L_80145738;
    }
    // 0x80145728: or          $fp, $zero, $zero
    ctx->r30 = 0 | 0;
    // 0x8014572C: addiu       $t7, $zero, 0xF
    ctx->r15 = ADD32(0, 0XF);
    // 0x80145730: sw          $t7, 0xE0($sp)
    MEM_W(0XE0, ctx->r29) = ctx->r15;
    // 0x80145734: addiu       $fp, $zero, 0x1
    ctx->r30 = ADD32(0, 0X1);
L_80145738:
    // 0x80145738: lw          $t8, 0xE0($sp)
    ctx->r24 = MEM_W(ctx->r29, 0XE0);
    // 0x8014573C: lui         $s5, 0x8000
    ctx->r21 = S32(0X8000 << 16);
    // 0x80145740: addiu       $s5, $s5, 0x47C0
    ctx->r21 = ADD32(ctx->r21, 0X47C0);
    // 0x80145744: blez        $t8, L_80145930
    if (SIGNED(ctx->r24) <= 0) {
        // 0x80145748: or          $s4, $zero, $zero
        ctx->r20 = 0 | 0;
            goto L_80145930;
    }
    // 0x80145748: or          $s4, $zero, $zero
    ctx->r20 = 0 | 0;
    // 0x8014574C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80145750: ldc1        $f30, -0x7EC8($at)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r1, -0X7EC8);
    // 0x80145754: lui         $at, 0x40F0
    ctx->r1 = S32(0X40F0 << 16);
    // 0x80145758: mtc1        $at, $f29
    ctx->f_odd[(29 - 1) * 2] = ctx->r1;
    // 0x8014575C: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80145760: mtc1        $at, $f27
    ctx->f_odd[(27 - 1) * 2] = ctx->r1;
    // 0x80145764: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80145768: lui         $s7, 0x8006
    ctx->r23 = S32(0X8006 << 16);
    // 0x8014576C: mtc1        $zero, $f28
    ctx->f28.u32l = 0;
    // 0x80145770: mtc1        $zero, $f26
    ctx->f26.u32l = 0;
    // 0x80145774: addiu       $s7, $s7, -0x6B48
    ctx->r23 = ADD32(ctx->r23, -0X6B48);
    // 0x80145778: ldc1        $f24, -0x7EC0($at)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r1, -0X7EC0);
    // 0x8014577C: cvt.d.s     $f22, $f12
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f22.d = CVT_D_S(ctx->f12.fl);
L_80145780:
    // 0x80145780: jalr        $s5
    // 0x80145784: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_0;
    // 0x80145784: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    after_0:
    // 0x80145788: addiu       $t9, $v0, -0x2710
    ctx->r25 = ADD32(ctx->r2, -0X2710);
    // 0x8014578C: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x80145790: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x80145794: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80145798: lwc1        $f4, 0x50($s6)
    ctx->f4.u32l = MEM_W(ctx->r22, 0X50);
    // 0x8014579C: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x801457A0: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801457A4: mul.d       $f10, $f8, $f24
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f24.d); 
    ctx->f10.d = MUL_D(ctx->f8.d, ctx->f24.d);
    // 0x801457A8: nop

    // 0x801457AC: mul.d       $f16, $f10, $f22
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f22.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f22.d);
    // 0x801457B0: div.d       $f18, $f16, $f26
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f26.d); 
    ctx->f18.d = DIV_D(ctx->f16.d, ctx->f26.d);
    // 0x801457B4: mtc1        $s4, $f16
    ctx->f16.u32l = ctx->r20;
    // 0x801457B8: nop

    // 0x801457BC: cvt.s.w     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    ctx->f4.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801457C0: cvt.d.s     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f0.d = CVT_D_S(ctx->f4.fl);
    // 0x801457C4: add.d       $f8, $f18, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f18.d + ctx->f6.d;
    // 0x801457C8: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x801457CC: add.d       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = ctx->f0.d + ctx->f0.d;
    // 0x801457D0: swc1        $f10, 0xC0($sp)
    MEM_W(0XC0, ctx->r29) = ctx->f10.u32l;
    // 0x801457D4: lwc1        $f18, 0x54($s6)
    ctx->f18.u32l = MEM_W(ctx->r22, 0X54);
    // 0x801457D8: cvt.d.s     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f6.d = CVT_D_S(ctx->f18.fl);
    // 0x801457DC: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x801457E0: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x801457E4: jalr        $s5
    // 0x801457E8: swc1        $f16, 0xC4($sp)
    MEM_W(0XC4, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_1;
    // 0x801457E8: swc1        $f16, 0xC4($sp)
    MEM_W(0XC4, ctx->r29) = ctx->f16.u32l;
    after_1:
    // 0x801457EC: addiu       $t0, $v0, -0x2710
    ctx->r8 = ADD32(ctx->r2, -0X2710);
    // 0x801457F0: mtc1        $t0, $f4
    ctx->f4.u32l = ctx->r8;
    // 0x801457F4: addiu       $a0, $zero, 0x1E
    ctx->r4 = ADD32(0, 0X1E);
    // 0x801457F8: addiu       $a1, $zero, 0x1E
    ctx->r5 = ADD32(0, 0X1E);
    // 0x801457FC: cvt.s.w     $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    ctx->f18.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80145800: lwc1        $f4, 0x58($s6)
    ctx->f4.u32l = MEM_W(ctx->r22, 0X58);
    // 0x80145804: addiu       $a2, $sp, 0xC0
    ctx->r6 = ADD32(ctx->r29, 0XC0);
    // 0x80145808: lui         $a3, 0x2080
    ctx->r7 = S32(0X2080 << 16);
    // 0x8014580C: cvt.d.s     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f6.d = CVT_D_S(ctx->f18.fl);
    // 0x80145810: cvt.d.s     $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f18.d = CVT_D_S(ctx->f4.fl);
    // 0x80145814: mul.d       $f8, $f6, $f24
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f24.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f24.d);
    // 0x80145818: nop

    // 0x8014581C: mul.d       $f10, $f8, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f22.d); 
    ctx->f10.d = MUL_D(ctx->f8.d, ctx->f22.d);
    // 0x80145820: div.d       $f16, $f10, $f26
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f26.d); 
    ctx->f16.d = DIV_D(ctx->f10.d, ctx->f26.d);
    // 0x80145824: add.d       $f6, $f16, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f6.d = ctx->f16.d + ctx->f18.d;
    // 0x80145828: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x8014582C: jalr        $s7
    // 0x80145830: swc1        $f8, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_2;
    // 0x80145830: swc1        $f8, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f8.u32l;
    after_2:
    // 0x80145834: beq         $v0, $zero, L_80145920
    if (ctx->r2 == 0) {
        // 0x80145838: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_80145920;
    }
    // 0x80145838: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x8014583C: mtc1        $fp, $f10
    ctx->f10.u32l = ctx->r30;
    // 0x80145840: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x80145844: lui         $s2, 0x8006
    ctx->r18 = S32(0X8006 << 16);
    // 0x80145848: cvt.d.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.d = CVT_D_W(ctx->f10.u32l);
    // 0x8014584C: lui         $s3, 0x8006
    ctx->r19 = S32(0X8006 << 16);
    // 0x80145850: addiu       $s3, $s3, -0x2C7C
    ctx->r19 = ADD32(ctx->r19, -0X2C7C);
    // 0x80145854: addiu       $s2, $s2, -0x337C
    ctx->r18 = ADD32(ctx->r18, -0X337C);
    // 0x80145858: addiu       $s1, $s1, -0x3768
    ctx->r17 = ADD32(ctx->r17, -0X3768);
    // 0x8014585C: mul.d       $f20, $f30, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f30.d); NAN_CHECK(ctx->f4.d); 
    ctx->f20.d = MUL_D(ctx->f30.d, ctx->f4.d);
    // 0x80145860: jalr        $s5
    // 0x80145864: addiu       $a0, $zero, 0x7FFF
    ctx->r4 = ADD32(0, 0X7FFF);
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_3;
    // 0x80145864: addiu       $a0, $zero, 0x7FFF
    ctx->r4 = ADD32(0, 0X7FFF);
    after_3:
    // 0x80145868: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    // 0x8014586C: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x80145870: bgez        $v0, L_80145888
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80145874: cvt.d.w     $f18, $f16
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
            goto L_80145888;
    }
    // 0x80145874: cvt.d.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
    // 0x80145878: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x8014587C: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80145880: nop

    // 0x80145884: add.d       $f18, $f18, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f6.d); 
    ctx->f18.d = ctx->f18.d + ctx->f6.d;
L_80145888:
    // 0x80145888: div.d       $f8, $f18, $f28
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f28.d); 
    ctx->f8.d = DIV_D(ctx->f18.d, ctx->f28.d);
    // 0x8014588C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80145890: ldc1        $f6, -0x7EB8($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, -0X7EB8);
    // 0x80145894: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80145898: addiu       $a1, $sp, 0xB4
    ctx->r5 = ADD32(ctx->r29, 0XB4);
    // 0x8014589C: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    // 0x801458A0: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x801458A4: cvt.d.s     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f4.d = CVT_D_S(ctx->f10.fl);
    // 0x801458A8: mul.d       $f16, $f4, $f30
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f30.d); 
    ctx->f16.d = MUL_D(ctx->f4.d, ctx->f30.d);
    // 0x801458AC: add.d       $f18, $f16, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d); 
    ctx->f18.d = ctx->f16.d + ctx->f6.d;
    // 0x801458B0: add.d       $f8, $f18, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f20.d); 
    ctx->f8.d = ctx->f18.d + ctx->f20.d;
    // 0x801458B4: cvt.s.d     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f0.fl = CVT_S_D(ctx->f8.d);
    // 0x801458B8: swc1        $f0, 0xB8($sp)
    MEM_W(0XB8, ctx->r29) = ctx->f0.u32l;
    // 0x801458BC: swc1        $f0, 0xB4($sp)
    MEM_W(0XB4, ctx->r29) = ctx->f0.u32l;
    // 0x801458C0: jalr        $s1
    // 0x801458C4: swc1        $f0, 0xBC($sp)
    MEM_W(0XBC, ctx->r29) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_4;
    // 0x801458C4: swc1        $f0, 0xBC($sp)
    MEM_W(0XBC, ctx->r29) = ctx->f0.u32l;
    after_4:
    // 0x801458C8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801458CC: lw          $a1, 0xDC($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XDC);
    // 0x801458D0: jalr        $s2
    // 0x801458D4: addiu       $a2, $zero, 0x800
    ctx->r6 = ADD32(0, 0X800);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_5;
    // 0x801458D4: addiu       $a2, $zero, 0x800
    ctx->r6 = ADD32(0, 0X800);
    after_5:
    // 0x801458D8: jalr        $s3
    // 0x801458DC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_6;
    // 0x801458DC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_6:
    // 0x801458E0: beq         $v0, $zero, L_80145914
    if (ctx->r2 == 0) {
        // 0x801458E4: lui         $t9, 0x8006
        ctx->r25 = S32(0X8006 << 16);
            goto L_80145914;
    }
    // 0x801458E4: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801458E8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801458EC: lwc1        $f10, -0x7EB0($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X7EB0);
    // 0x801458F0: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801458F4: addiu       $t9, $t9, -0x2F4C
    ctx->r25 = ADD32(ctx->r25, -0X2F4C);
    // 0x801458F8: lui         $a1, 0x8014
    ctx->r5 = S32(0X8014 << 16);
    // 0x801458FC: addiu       $a1, $a1, 0x5978
    ctx->r5 = ADD32(ctx->r5, 0X5978);
    // 0x80145900: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80145904: jalr        $t9
    // 0x80145908: swc1        $f10, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80145908: swc1        $f10, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f10.u32l;
    after_7:
    // 0x8014590C: b           L_80145924
    // 0x80145910: lw          $t1, 0xE0($sp)
    ctx->r9 = MEM_W(ctx->r29, 0XE0);
        goto L_80145924;
    // 0x80145910: lw          $t1, 0xE0($sp)
    ctx->r9 = MEM_W(ctx->r29, 0XE0);
L_80145914:
    // 0x80145914: addiu       $t9, $t9, -0x2EC4
    ctx->r25 = ADD32(ctx->r25, -0X2EC4);
    // 0x80145918: jalr        $t9
    // 0x8014591C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x8014591C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_8:
L_80145920:
    // 0x80145920: lw          $t1, 0xE0($sp)
    ctx->r9 = MEM_W(ctx->r29, 0XE0);
L_80145924:
    // 0x80145924: addiu       $s4, $s4, 0x1
    ctx->r20 = ADD32(ctx->r20, 0X1);
    // 0x80145928: bne         $s4, $t1, L_80145780
    if (ctx->r20 != ctx->r9) {
        // 0x8014592C: nop
    
            goto L_80145780;
    }
    // 0x8014592C: nop

L_80145930:
    // 0x80145930: lw          $ra, 0x6C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X6C);
    // 0x80145934: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80145938: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x8014593C: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x80145940: ldc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X30);
    // 0x80145944: ldc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X38);
    // 0x80145948: ldc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X40);
    // 0x8014594C: lw          $s0, 0x48($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X48);
    // 0x80145950: lw          $s1, 0x4C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X4C);
    // 0x80145954: lw          $s2, 0x50($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X50);
    // 0x80145958: lw          $s3, 0x54($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X54);
    // 0x8014595C: lw          $s4, 0x58($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X58);
    // 0x80145960: lw          $s5, 0x5C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X5C);
    // 0x80145964: lw          $s6, 0x60($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X60);
    // 0x80145968: lw          $s7, 0x64($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X64);
    // 0x8014596C: lw          $fp, 0x68($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X68);
    // 0x80145970: jr          $ra
    // 0x80145974: addiu       $sp, $sp, 0xD0
    ctx->r29 = ADD32(ctx->r29, 0XD0);
    return;
    // 0x80145974: addiu       $sp, $sp, 0xD0
    ctx->r29 = ADD32(ctx->r29, 0XD0);
;}
RECOMP_FUNC void func_80145978(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145978: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8014597C: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x80145980: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80145984: addiu       $t9, $t9, -0x2C7C
    ctx->r25 = ADD32(ctx->r25, -0X2C7C);
    // 0x80145988: jalr        $t9
    // 0x8014598C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8014598C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    after_0:
    // 0x80145990: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80145994: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80145998: lwc1        $f0, -0x7EAC($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X7EAC);
    // 0x8014599C: sw          $v0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r2;
    // 0x801459A0: swc1        $f4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f4.u32l;
    // 0x801459A4: swc1        $f4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f4.u32l;
    // 0x801459A8: lwc1        $f6, 0x4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801459AC: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801459B0: addiu       $t9, $t9, -0x39C8
    ctx->r25 = ADD32(ctx->r25, -0X39C8);
    // 0x801459B4: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x801459B8: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    // 0x801459BC: addiu       $a2, $zero, 0x4000
    ctx->r6 = ADD32(0, 0X4000);
    // 0x801459C0: swc1        $f0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f0.u32l;
    // 0x801459C4: swc1        $f0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f0.u32l;
    // 0x801459C8: swc1        $f0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f0.u32l;
    // 0x801459CC: jalr        $t9
    // 0x801459D0: swc1        $f6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801459D0: swc1        $f6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f6.u32l;
    after_1:
    // 0x801459D4: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801459D8: addiu       $t9, $t9, -0x3768
    ctx->r25 = ADD32(ctx->r25, -0X3768);
    // 0x801459DC: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x801459E0: addiu       $a1, $sp, 0x1C
    ctx->r5 = ADD32(ctx->r29, 0X1C);
    // 0x801459E4: jalr        $t9
    // 0x801459E8: addiu       $a2, $zero, 0x2000
    ctx->r6 = ADD32(0, 0X2000);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801459E8: addiu       $a2, $zero, 0x2000
    ctx->r6 = ADD32(0, 0X2000);
    after_2:
    // 0x801459EC: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x801459F0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801459F4: ldc1        $f18, -0x7EA8($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, -0X7EA8);
    // 0x801459F8: lwc1        $f10, 0x4($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X4);
    // 0x801459FC: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x80145A00: add.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f16.d + ctx->f18.d;
    // 0x80145A04: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x80145A08: swc1        $f6, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f6.u32l;
    // 0x80145A0C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80145A10: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80145A14: jr          $ra
    // 0x80145A18: nop

    return;
    // 0x80145A18: nop

;}
RECOMP_FUNC void func_80145A1C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145A1C: lwc1        $f4, 0x50($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X50);
    // 0x80145A20: lwc1        $f6, 0x50($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X50);
    // 0x80145A24: mtc1        $zero, $f3
    ctx->f_odd[(3 - 1) * 2] = 0;
    // 0x80145A28: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80145A2C: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80145A30: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80145A34: cvt.d.s     $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f8.d = CVT_D_S(ctx->f0.fl);
    // 0x80145A38: c.lt.d      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.d < ctx->f2.d;
    // 0x80145A3C: nop

    // 0x80145A40: bc1fl       L_80145A50
    if (!c1cs) {
        // 0x80145A44: c.lt.s      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
            goto L_80145A50;
    }
    goto skip_0;
    // 0x80145A44: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    skip_0:
    // 0x80145A48: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x80145A4C: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
L_80145A50:
    // 0x80145A50: nop

    // 0x80145A54: bc1fl       L_80145A68
    if (!c1cs) {
        // 0x80145A58: lwc1        $f10, 0x54($a1)
        ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
            goto L_80145A68;
    }
    goto skip_1;
    // 0x80145A58: lwc1        $f10, 0x54($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
    skip_1:
    // 0x80145A5C: jr          $ra
    // 0x80145A60: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80145A60: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80145A64: lwc1        $f10, 0x54($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X54);
L_80145A68:
    // 0x80145A68: lwc1        $f16, 0x54($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X54);
    // 0x80145A6C: sub.s       $f0, $f10, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x80145A70: cvt.d.s     $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f18.d = CVT_D_S(ctx->f0.fl);
    // 0x80145A74: c.lt.d      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.d < ctx->f2.d;
    // 0x80145A78: nop

    // 0x80145A7C: bc1fl       L_80145A8C
    if (!c1cs) {
        // 0x80145A80: c.lt.s      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
            goto L_80145A8C;
    }
    goto skip_2;
    // 0x80145A80: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    skip_2:
    // 0x80145A84: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x80145A88: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
L_80145A8C:
    // 0x80145A8C: nop

    // 0x80145A90: bc1fl       L_80145AA4
    if (!c1cs) {
        // 0x80145A94: lwc1        $f4, 0x58($a1)
        ctx->f4.u32l = MEM_W(ctx->r5, 0X58);
            goto L_80145AA4;
    }
    goto skip_3;
    // 0x80145A94: lwc1        $f4, 0x58($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X58);
    skip_3:
    // 0x80145A98: jr          $ra
    // 0x80145A9C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80145A9C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80145AA0: lwc1        $f4, 0x58($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X58);
L_80145AA4:
    // 0x80145AA4: lwc1        $f6, 0x58($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X58);
    // 0x80145AA8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80145AAC: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80145AB0: cvt.d.s     $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f8.d = CVT_D_S(ctx->f0.fl);
    // 0x80145AB4: c.lt.d      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.d < ctx->f2.d;
    // 0x80145AB8: nop

    // 0x80145ABC: bc1fl       L_80145ACC
    if (!c1cs) {
        // 0x80145AC0: c.lt.s      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
            goto L_80145ACC;
    }
    goto skip_4;
    // 0x80145AC0: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    skip_4:
    // 0x80145AC4: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x80145AC8: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
L_80145ACC:
    // 0x80145ACC: nop

    // 0x80145AD0: bc1f        L_80145AE0
    if (!c1cs) {
        // 0x80145AD4: nop
    
            goto L_80145AE0;
    }
    // 0x80145AD4: nop

    // 0x80145AD8: jr          $ra
    // 0x80145ADC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80145ADC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80145AE0:
    // 0x80145AE0: jr          $ra
    // 0x80145AE4: nop

    return;
    // 0x80145AE4: nop

;}
RECOMP_FUNC void func_80145AF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145AF0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80145AF4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80145AF8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80145AFC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80145B00: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80145B04: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80145B08: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80145B0C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80145B10: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80145B14: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80145B18: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80145B1C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80145B20: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80145B24: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80145B28: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80145B2C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80145B30: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgstate_dispatch_probe(rdram, (uint32_t)ctx->r4, "driver");
#endif
    // 0x80145B34: lw          $t9, -0x2C50($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2C50);
    // 0x80145B38: jalr        $t9
    // 0x80145B3C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80145B3C: nop

    after_0:
    // 0x80145B40: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80145B44: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80145B48: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80145B4C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80145B50: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80145B54: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80145B58: jr          $ra
    // 0x80145B5C: nop

    return;
    // 0x80145B5C: nop

;}
RECOMP_FUNC void func_80145B60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145B60: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80145B64: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80145B68: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80145B6C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80145B70: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80145B74: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80145B78: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x80145B7C: jalr        $t9
    // 0x80145B80: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80145B80: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    after_0:
    // 0x80145B84: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80145B88: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80145B8C: lh          $t6, 0xE($a0)
    ctx->r14 = MEM_H(ctx->r4, 0XE);
    // 0x80145B90: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80145B94: addu        $v0, $a0, $t7
    ctx->r2 = ADD32(ctx->r4, ctx->r15);
    // 0x80145B98: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x80145B9C: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x80145BA0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80145BA4: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80145BA8: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x80145BAC: sb          $t0, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r8;
    // 0x80145BB0: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgstate_dispatch_probe(rdram, (uint32_t)ctx->r4, "bootstrap");
#endif
    // 0x80145BB4: lw          $t9, -0x2C50($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2C50);
    // 0x80145BB8: jalr        $t9
    // 0x80145BBC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80145BBC: nop

    after_1:
    // 0x80145BC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80145BC4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80145BC8: jr          $ra
    // 0x80145BCC: nop

    return;
    // 0x80145BCC: nop

;}
RECOMP_FUNC void func_80145BD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145BD0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80145BD4: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80145BD8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80145BDC: lh          $t6, 0x28D0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X28D0);
    // 0x80145BE0: lw          $t8, 0x2908($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2908);
    // 0x80145BE4: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80145BE8: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80145BEC: lui         $at, 0xEFFF
    ctx->r1 = S32(0XEFFF << 16);
    // 0x80145BF0: lui         $s1, 0x8019
    ctx->r17 = S32(0X8019 << 16);
    // 0x80145BF4: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80145BF8: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80145BFC: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80145C00: or          $s4, $a0, $zero
    ctx->r20 = ctx->r4 | 0;
    // 0x80145C04: addiu       $s2, $s2, -0x2E80
    ctx->r18 = ADD32(ctx->r18, -0X2E80);
    // 0x80145C08: addiu       $s1, $s1, -0x1958
    ctx->r17 = ADD32(ctx->r17, -0X1958);
    // 0x80145C0C: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80145C10: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80145C14: sll         $t7, $t6, 4
    ctx->r15 = S32(ctx->r14 << 4);
    // 0x80145C18: and         $t9, $t8, $at
    ctx->r25 = ctx->r24 & ctx->r1;
    // 0x80145C1C: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80145C20: addu        $s0, $s1, $t7
    ctx->r16 = ADD32(ctx->r17, ctx->r15);
    // 0x80145C24: sw          $t9, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r25;
    // 0x80145C28: sw          $zero, 0x2B10($v0)
    MEM_W(0X2B10, ctx->r2) = 0;
    // 0x80145C2C: sw          $zero, 0x2B2C($v0)
    MEM_W(0X2B2C, ctx->r2) = 0;
    // 0x80145C30: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80145C34: jal         0x80000F30
    // 0x80145C38: addiu       $a1, $zero, 0x24
    ctx->r5 = ADD32(0, 0X24);
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x80145C38: addiu       $a1, $zero, 0x24
    ctx->r5 = ADD32(0, 0X24);
    after_0:
    // 0x80145C3C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80145C40: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80145C44: sw          $zero, -0x2E5C($at)
    MEM_W(-0X2E5C, ctx->r1) = 0;
    // 0x80145C48: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x80145C4C: swc1        $f4, -0xA30($at)
    MEM_W(-0XA30, ctx->r1) = ctx->f4.u32l;
    // 0x80145C50: lw          $t1, 0x0($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X0);
    // 0x80145C54: lw          $t0, 0x4($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X4);
    // 0x80145C58: subu        $a1, $t0, $t1
    ctx->r5 = SUB32(ctx->r8, ctx->r9);
    // 0x80145C5C: jal         0x80147A50
    // 0x80145C60: addiu       $a0, $a1, 0x18
    ctx->r4 = ADD32(ctx->r5, 0X18);
    func_80147A50(rdram, ctx);
        goto after_1;
    // 0x80145C60: addiu       $a0, $a1, 0x18
    ctx->r4 = ADD32(ctx->r5, 0X18);
    after_1:
    // 0x80145C64: sw          $v0, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r2;
    // 0x80145C68: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80145C6C: lh          $a0, -0x5470($a0)
    ctx->r4 = MEM_H(ctx->r4, -0X5470);
    // 0x80145C70: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x80145C74: addiu       $s3, $zero, 0x3
    ctx->r19 = ADD32(0, 0X3);
    // 0x80145C78: sll         $t2, $a0, 4
    ctx->r10 = S32(ctx->r4 << 4);
L_80145C7C:
    // 0x80145C7C: addu        $t3, $s1, $t2
    ctx->r11 = ADD32(ctx->r17, ctx->r10);
    // 0x80145C80: addu        $t4, $t3, $s0
    ctx->r12 = ADD32(ctx->r11, ctx->r16);
    // 0x80145C84: lbu         $v1, 0x9($t4)
    ctx->r3 = MEM_BU(ctx->r12, 0X9);
    // 0x80145C88: beq         $v1, $zero, L_80145CBC
    if (ctx->r3 == 0) {
        // 0x80145C8C: sll         $t5, $v1, 4
        ctx->r13 = S32(ctx->r3 << 4);
            goto L_80145CBC;
    }
    // 0x80145C8C: sll         $t5, $v1, 4
    ctx->r13 = S32(ctx->r3 << 4);
    // 0x80145C90: addu        $v0, $s1, $t5
    ctx->r2 = ADD32(ctx->r17, ctx->r13);
    // 0x80145C94: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    // 0x80145C98: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x80145C9C: subu        $a1, $t6, $t7
    ctx->r5 = SUB32(ctx->r14, ctx->r15);
    // 0x80145CA0: jal         0x80147A50
    // 0x80145CA4: addiu       $a0, $a1, 0x18
    ctx->r4 = ADD32(ctx->r5, 0X18);
    func_80147A50(rdram, ctx);
        goto after_2;
    // 0x80145CA4: addiu       $a0, $a1, 0x18
    ctx->r4 = ADD32(ctx->r5, 0X18);
    after_2:
    // 0x80145CA8: lw          $t8, 0x4($s2)
    ctx->r24 = MEM_W(ctx->r18, 0X4);
    // 0x80145CAC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80145CB0: addu        $t9, $t8, $v0
    ctx->r25 = ADD32(ctx->r24, ctx->r2);
    // 0x80145CB4: sw          $t9, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r25;
    // 0x80145CB8: lh          $a0, -0x5470($a0)
    ctx->r4 = MEM_H(ctx->r4, -0X5470);
L_80145CBC:
    // 0x80145CBC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80145CC0: bnel        $s0, $s3, L_80145C7C
    if (ctx->r16 != ctx->r19) {
        // 0x80145CC4: sll         $t2, $a0, 4
        ctx->r10 = S32(ctx->r4 << 4);
            goto L_80145C7C;
    }
    goto skip_0;
    // 0x80145CC4: sll         $t2, $a0, 4
    ctx->r10 = S32(ctx->r4 << 4);
    skip_0:
    // 0x80145CC8: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80145CCC: addiu       $t1, $t1, 0x2160
    ctx->r9 = ADD32(ctx->r9, 0X2160);
    // 0x80145CD0: sll         $t0, $a0, 4
    ctx->r8 = S32(ctx->r4 << 4);
    // 0x80145CD4: addu        $v0, $t0, $t1
    ctx->r2 = ADD32(ctx->r8, ctx->r9);
    // 0x80145CD8: lw          $t2, 0x0($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X0);
    // 0x80145CDC: beq         $t2, $zero, L_80145D04
    if (ctx->r10 == 0) {
        // 0x80145CE0: nop
    
            goto L_80145D04;
    }
    // 0x80145CE0: nop

    // 0x80145CE4: lw          $t3, 0x8($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X8);
    // 0x80145CE8: lw          $t4, 0x4($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X4);
    // 0x80145CEC: subu        $v1, $t3, $t4
    ctx->r3 = SUB32(ctx->r11, ctx->r12);
    // 0x80145CF0: jal         0x80147A50
    // 0x80145CF4: addiu       $a0, $v1, 0x18
    ctx->r4 = ADD32(ctx->r3, 0X18);
    func_80147A50(rdram, ctx);
        goto after_3;
    // 0x80145CF4: addiu       $a0, $v1, 0x18
    ctx->r4 = ADD32(ctx->r3, 0X18);
    after_3:
    // 0x80145CF8: lw          $t5, 0x4($s2)
    ctx->r13 = MEM_W(ctx->r18, 0X4);
    // 0x80145CFC: addu        $t6, $t5, $v0
    ctx->r14 = ADD32(ctx->r13, ctx->r2);
    // 0x80145D00: sw          $t6, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r14;
L_80145D04:
    // 0x80145D04: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80145D08: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x80145D0C: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80145D10: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80145D14: lw          $a2, 0x4($s2)
    ctx->r6 = MEM_W(ctx->r18, 0X4);
    // 0x80145D18: jalr        $t9
    // 0x80145D1C: addiu       $a3, $zero, 0x8
    ctx->r7 = ADD32(0, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80145D1C: addiu       $a3, $zero, 0x8
    ctx->r7 = ADD32(0, 0X8);
    after_4:
    // 0x80145D20: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
    // 0x80145D24: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80145D28: addiu       $a0, $zero, 0x7
    ctx->r4 = ADD32(0, 0X7);
    // 0x80145D2C: lw          $a2, 0x4($s2)
    ctx->r6 = MEM_W(ctx->r18, 0X4);
    // 0x80145D30: jal         0x800013E8
    // 0x80145D34: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    heap_init(rdram, ctx);
        goto after_5;
    // 0x80145D34: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_5:
    // 0x80145D38: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80145D3C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80145D40: addiu       $a0, $s4, 0x8
    ctx->r4 = ADD32(ctx->r20, 0X8);
    // 0x80145D44: jalr        $t9
    // 0x80145D48: addiu       $a1, $s4, 0xE
    ctx->r5 = ADD32(ctx->r20, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x80145D48: addiu       $a1, $s4, 0xE
    ctx->r5 = ADD32(ctx->r20, 0XE);
    after_6:
    // 0x80145D4C: lh          $t7, 0xE($s4)
    ctx->r15 = MEM_H(ctx->r20, 0XE);
    // 0x80145D50: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80145D54: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80145D58: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x80145D5C: addu        $v0, $s4, $t8
    ctx->r2 = ADD32(ctx->r20, ctx->r24);
    // 0x80145D60: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80145D64: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80145D68: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80145D6C: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80145D70: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80145D74: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80145D78: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgstate_dispatch_probe(rdram, (uint32_t)ctx->r20, "init");
#endif
    // 0x80145D7C: lw          $t9, -0x2C50($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2C50);
    // 0x80145D80: jalr        $t9
    // 0x80145D84: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80145D84: nop

    after_7:
    // 0x80145D88: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80145D8C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80145D90: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80145D94: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80145D98: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80145D9C: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80145DA0: jr          $ra
    // 0x80145DA4: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80145DA4: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80145DA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145DA8: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x80145DAC: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80145DB0: lui         $s6, 0x801D
    ctx->r22 = S32(0X801D << 16);
    // 0x80145DB4: addiu       $s6, $s6, -0x7D40
    ctx->r22 = ADD32(ctx->r22, -0X7D40);
    // 0x80145DB8: lh          $t6, 0x28D0($s6)
    ctx->r14 = MEM_H(ctx->r22, 0X28D0);
    // 0x80145DBC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80145DC0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80145DC4: addiu       $t9, $t9, -0x1958
    ctx->r25 = ADD32(ctx->r25, -0X1958);
    // 0x80145DC8: sll         $t8, $t6, 4
    ctx->r24 = S32(ctx->r14 << 4);
    // 0x80145DCC: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80145DD0: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x80145DD4: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80145DD8: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80145DDC: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80145DE0: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80145DE4: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80145DE8: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80145DEC: sw          $a0, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r4;
    // 0x80145DF0: addu        $s0, $t8, $t9
    ctx->r16 = ADD32(ctx->r24, ctx->r25);
    // 0x80145DF4: sw          $t6, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r14;
    // 0x80145DF8: lbu         $t0, 0xA($s0)
    ctx->r8 = MEM_BU(ctx->r16, 0XA);
    // 0x80145DFC: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x80145E00: addiu       $s4, $s4, -0x2E60
    ctx->r20 = ADD32(ctx->r20, -0X2E60);
    // 0x80145E04: sw          $t0, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r8;
    // 0x80145E08: lbu         $t1, 0xB($s0)
    ctx->r9 = MEM_BU(ctx->r16, 0XB);
    // 0x80145E0C: addiu       $s1, $sp, 0x74
    ctx->r17 = ADD32(ctx->r29, 0X74);
    // 0x80145E10: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x80145E14: addiu       $s5, $zero, -0x1
    ctx->r21 = ADD32(0, -0X1);
    // 0x80145E18: addiu       $s7, $sp, 0x78
    ctx->r23 = ADD32(ctx->r29, 0X78);
    // 0x80145E1C: addiu       $fp, $sp, 0x80
    ctx->r30 = ADD32(ctx->r29, 0X80);
    // 0x80145E20: sw          $t1, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r9;
L_80145E24:
    // 0x80145E24: sltu        $at, $s1, $s7
    ctx->r1 = ctx->r17 < ctx->r23 ? 1 : 0;
    // 0x80145E28: bnel        $at, $zero, L_80145E40
    if (ctx->r1 != 0) {
        // 0x80145E2C: lw          $t3, 0x0($s1)
        ctx->r11 = MEM_W(ctx->r17, 0X0);
            goto L_80145E40;
    }
    goto skip_0;
    // 0x80145E2C: lw          $t3, 0x0($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X0);
    skip_0:
    // 0x80145E30: lw          $t2, 0x0($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X0);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_loadstate_identry_probe((uint32_t)ctx->r17, (uint32_t)ctx->r10,
                                      ctx->r10 == 0 ? 1 : 0);
#endif
    // 0x80145E34: beql        $t2, $zero, L_80145ECC
    if (ctx->r10 == 0) {
        // 0x80145E38: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_80145ECC;
    }
    goto skip_1;
    // 0x80145E38: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    skip_1:
    // 0x80145E3C: lw          $t3, 0x0($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X0);
L_80145E40:
    // 0x80145E40: lui         $t5, 0x8019
    ctx->r13 = S32(0X8019 << 16);
    // 0x80145E44: addiu       $t5, $t5, -0x1958
    ctx->r13 = ADD32(ctx->r13, -0X1958);
    // 0x80145E48: sll         $t4, $t3, 4
    ctx->r12 = S32(ctx->r11 << 4);
    // 0x80145E4C: addu        $s0, $t4, $t5
    ctx->r16 = ADD32(ctx->r12, ctx->r13);
    // 0x80145E50: lhu         $t6, 0x8($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X8);
    // 0x80145E54: addu        $t7, $s6, $s3
    ctx->r15 = ADD32(ctx->r22, ctx->r19);
    // 0x80145E58: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x80145E5C: sw          $t6, 0x2B14($t7)
    MEM_W(0X2B14, ctx->r15) = ctx->r14;
    // 0x80145E60: lw          $t9, 0x0($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X0);
    // 0x80145E64: lw          $t8, 0x4($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4);
    // 0x80145E68: addiu       $t0, $t0, -0x2E80
    ctx->r8 = ADD32(ctx->r8, -0X2E80);
    // 0x80145E6C: addu        $s2, $s3, $t0
    ctx->r18 = ADD32(ctx->r19, ctx->r8);
    // 0x80145E70: subu        $a1, $t8, $t9
    ctx->r5 = SUB32(ctx->r24, ctx->r25);
    // 0x80145E74: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80145E78: addiu       $t9, $t9, 0x1514
    ctx->r25 = ADD32(ctx->r25, 0X1514);
    // 0x80145E7C: jalr        $t9
    // 0x80145E80: addiu       $a0, $zero, 0x7
    ctx->r4 = ADD32(0, 0X7);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80145E80: addiu       $a0, $zero, 0x7
    ctx->r4 = ADD32(0, 0X7);
    after_0:
    // 0x80145E84: sw          $v0, 0x8($s2)
    MEM_W(0X8, ctx->r18) = ctx->r2;
    // 0x80145E88: lhu         $t1, 0x8($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X8);
    // 0x80145E8C: addiu       $v0, $s5, 0x1
    ctx->r2 = ADD32(ctx->r21, 0X1);
    // 0x80145E90: sll         $t2, $v0, 3
    ctx->r10 = S32(ctx->r2 << 3);
    // 0x80145E94: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80145E98: addu        $at, $at, $t2
    ctx->r1 = ADD32(ctx->r1, ctx->r10);
    // 0x80145E9C: sw          $t1, -0x2E58($at)
    MEM_W(-0X2E58, ctx->r1) = ctx->r9;
    // 0x80145EA0: lw          $t3, 0x8($s2)
    ctx->r11 = MEM_W(ctx->r18, 0X8);
    // 0x80145EA4: or          $s5, $v0, $zero
    ctx->r21 = ctx->r2 | 0;
    // 0x80145EA8: addiu       $s4, $s4, 0x8
    ctx->r20 = ADD32(ctx->r20, 0X8);
    // 0x80145EAC: sw          $t3, 0x4($s4)
    MEM_W(0X4, ctx->r20) = ctx->r11;
    // 0x80145EB0: jal         0x80011364
    // 0x80145EB4: lhu         $a0, 0x8($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0X8);
    func_80011364(rdram, ctx);
        goto after_1;
    // 0x80145EB4: lhu         $a0, 0x8($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0X8);
    after_1:
    // 0x80145EB8: lhu         $t4, 0x8($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0X8);
    // 0x80145EBC: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x80145EC0: addu        $t6, $s6, $t5
    ctx->r14 = ADD32(ctx->r22, ctx->r13);
    // 0x80145EC4: sw          $zero, 0x570($t6)
    MEM_W(0X570, ctx->r14) = 0;
    // 0x80145EC8: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
L_80145ECC:
    // 0x80145ECC: bne         $s1, $fp, L_80145E24
    if (ctx->r17 != ctx->r30) {
        // 0x80145ED0: addiu       $s3, $s3, 0x4
        ctx->r19 = ADD32(ctx->r19, 0X4);
            goto L_80145E24;
    }
    // 0x80145ED0: addiu       $s3, $s3, 0x4
    ctx->r19 = ADD32(ctx->r19, 0X4);
    // 0x80145ED4: lh          $v0, 0x28D0($s6)
    ctx->r2 = MEM_H(ctx->r22, 0X28D0);
    // 0x80145ED8: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80145EDC: addiu       $t8, $t8, 0x2160
    ctx->r24 = ADD32(ctx->r24, 0X2160);
    // 0x80145EE0: sll         $t7, $v0, 4
    ctx->r15 = S32(ctx->r2 << 4);
    // 0x80145EE4: addu        $s0, $t7, $t8
    ctx->r16 = ADD32(ctx->r15, ctx->r24);
    // 0x80145EE8: lw          $t0, 0x0($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X0);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_loadstate_record2_probe((uint32_t)ctx->r2, (uint32_t)ctx->r16, (uint32_t)ctx->r8,
                                      ctx->r8 == 0 ? 1 : 0);
#endif
    // 0x80145EEC: beql        $t0, $zero, L_80145F60
    if (ctx->r8 == 0) {
        // 0x80145EF0: lw          $t7, 0x0($s4)
        ctx->r15 = MEM_W(ctx->r20, 0X0);
            goto L_80145F60;
    }
    goto skip_2;
    // 0x80145EF0: lw          $t7, 0x0($s4)
    ctx->r15 = MEM_W(ctx->r20, 0X0);
    skip_2:
    // 0x80145EF4: lw          $t9, 0x8($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X8);
    // 0x80145EF8: lw          $t1, 0x4($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X4);
    // 0x80145EFC: addiu       $a0, $zero, 0x7
    ctx->r4 = ADD32(0, 0X7);
    // 0x80145F00: subu        $a1, $t9, $t1
    ctx->r5 = SUB32(ctx->r25, ctx->r9);
    // 0x80145F04: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80145F08: addiu       $t9, $t9, 0x1514
    ctx->r25 = ADD32(ctx->r25, 0X1514);
    // 0x80145F0C: jalr        $t9
    // 0x80145F10: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80145F10: nop

    after_2:
    // 0x80145F14: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80145F18: addiu       $v1, $v1, -0x2E5C
    ctx->r3 = ADD32(ctx->r3, -0X2E5C);
    // 0x80145F1C: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x80145F20: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80145F24: lhu         $t2, 0xC($s0)
    ctx->r10 = MEM_HU(ctx->r16, 0XC);
    // 0x80145F28: addiu       $v0, $s5, 0x1
    ctx->r2 = ADD32(ctx->r21, 0X1);
    // 0x80145F2C: sll         $t3, $v0, 3
    ctx->r11 = S32(ctx->r2 << 3);
    // 0x80145F30: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80145F34: addu        $at, $at, $t3
    ctx->r1 = ADD32(ctx->r1, ctx->r11);
    // 0x80145F38: sw          $t2, -0x2E58($at)
    MEM_W(-0X2E58, ctx->r1) = ctx->r10;
    // 0x80145F3C: sw          $a1, 0xC($s4)
    MEM_W(0XC, ctx->r20) = ctx->r5;
    // 0x80145F40: addiu       $s4, $s4, 0x8
    ctx->r20 = ADD32(ctx->r20, 0X8);
    // 0x80145F44: jal         0x80011364
    // 0x80145F48: lhu         $a0, 0xC($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0XC);
    func_80011364(rdram, ctx);
        goto after_3;
    // 0x80145F48: lhu         $a0, 0xC($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0XC);
    after_3:
    // 0x80145F4C: lhu         $t4, 0xC($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0XC);
    // 0x80145F50: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x80145F54: addu        $t6, $s6, $t5
    ctx->r14 = ADD32(ctx->r22, ctx->r13);
    // 0x80145F58: sw          $zero, 0x570($t6)
    MEM_W(0X570, ctx->r14) = 0;
    // 0x80145F5C: lw          $t7, 0x0($s4)
    ctx->r15 = MEM_W(ctx->r20, 0X0);
L_80145F60:
    // 0x80145F60: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80145F64: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80145F68: or          $t8, $t7, $at
    ctx->r24 = ctx->r15 | ctx->r1;
    // 0x80145F6C: sw          $t8, 0x0($s4)
    MEM_W(0X0, ctx->r20) = ctx->r24;
    // 0x80145F70: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80145F74: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x80145F78: addiu       $a3, $a3, -0x5218
    ctx->r7 = ADD32(ctx->r7, -0X5218);
    // 0x80145F7C: addiu       $a1, $a1, -0x2E58
    ctx->r5 = ADD32(ctx->r5, -0X2E58);
    // 0x80145F80: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    // 0x80145F84: jal         0x800119CC
    // 0x80145F88: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_loadstate_queue_probe((uint32_t)MEM_W(ctx->r29, 0X80), (uint32_t)(ctx->r21 + 1));
#endif
    func_800119CC(rdram, ctx);
        goto after_4;
    // 0x80145F88: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_4:
    // 0x80145F8C: lw          $t0, 0x80($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X80);
    // 0x80145F90: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80145F94: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80145F98: addiu       $a0, $t0, 0x8
    ctx->r4 = ADD32(ctx->r8, 0X8);
    // 0x80145F9C: jalr        $t9
    // 0x80145FA0: addiu       $a1, $t0, 0xE
    ctx->r5 = ADD32(ctx->r8, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80145FA0: addiu       $a1, $t0, 0xE
    ctx->r5 = ADD32(ctx->r8, 0XE);
    after_5:
    // 0x80145FA4: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80145FA8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80145FAC: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80145FB0: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80145FB4: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80145FB8: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80145FBC: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80145FC0: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80145FC4: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80145FC8: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80145FCC: jr          $ra
    // 0x80145FD0: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    return;
    // 0x80145FD0: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
;}
RECOMP_FUNC void func_80145FD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80145FD4: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x80145FD8: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x80145FDC: lw          $t6, 0x2B28($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X2B28);
    // 0x80145FE0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80145FE4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80145FE8: beql        $t6, $zero, L_801460C8
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_updatea_gate1_probe((uint32_t)ctx->r4, (uint32_t)ctx->r14, ctx->r14 == 0 ? 1 : 0);
#endif
    if (ctx->r14 == 0) {
        // 0x80145FEC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801460C8;
    }
    goto skip_0;
    // 0x80145FEC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80145FF0: jal         0x801468D4
    // 0x80145FF4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_801468D4(rdram, ctx);
        goto after_0;
    // 0x80145FF4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80145FF8: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x80145FFC: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x80146000: lh          $t7, 0x28D0($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X28D0);
    // 0x80146004: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x80146008: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x8014600C: addiu       $t0, $t0, -0x1134
    ctx->r8 = ADD32(ctx->r8, -0X1134);
    // 0x80146010: addiu       $t9, $t9, 0x2160
    ctx->r25 = ADD32(ctx->r25, 0X2160);
    // 0x80146014: sll         $t8, $t7, 4
    ctx->r24 = S32(ctx->r15 << 4);
    // 0x80146018: addu        $v0, $t8, $t9
    ctx->r2 = ADD32(ctx->r24, ctx->r25);
    // 0x8014601C: sw          $zero, 0x0($t0)
    MEM_W(0X0, ctx->r8) = 0;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_textreg_store_probe(0, "func_80145FD4");
#endif
    // 0x80146020: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x80146024: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80146028: beq         $v1, $zero, L_80146070
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_updatea_gate2_probe((uint32_t)ctx->r6, (uint32_t)ctx->r15, (uint32_t)ctx->r2,
                                  (uint32_t)ctx->r3, ctx->r3 == 0 ? 1 : 0);
#endif
    if (ctx->r3 == 0) {
        // 0x8014602C: nop
    
            goto L_80146070;
    }
    // 0x8014602C: nop

    // 0x80146030: lhu         $t1, 0xC($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0XC);
    // 0x80146034: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x80146038: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x8014603C: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80146040: addu        $t3, $a3, $t2
    ctx->r11 = ADD32(ctx->r7, ctx->r10);
    // 0x80146044: lw          $t4, 0x570($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X570);
    // 0x80146048: and         $t5, $v1, $at
    ctx->r13 = ctx->r3 & ctx->r1;
    // 0x8014604C: addu        $a1, $t4, $t5
    ctx->r5 = ADD32(ctx->r12, ctx->r13);
    // 0x80146050: sw          $a1, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r5;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_textreg_store_probe((uint32_t)ctx->r5, "func_80145FD4");
    lod_ni0e_textreg_resolve_probe((uint32_t)ctx->r15, (uint32_t)ctx->r9, (uint32_t)ctx->r12,
                                    (uint32_t)ctx->r13, (uint32_t)ctx->r5);
#endif
    // 0x80146054: lhu         $t7, 0xC($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0XC);
    // 0x80146058: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8014605C: addu        $t9, $a3, $t8
    ctx->r25 = ADD32(ctx->r7, ctx->r24);
    // 0x80146060: lw          $a0, 0x570($t9)
    ctx->r4 = MEM_W(ctx->r25, 0X570);
    // 0x80146064: jal         0x80172430
    // 0x80146068: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_80172430(rdram, ctx);
        goto after_1;
    // 0x80146068: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_1:
    // 0x8014606C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
L_80146070:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_updatea_advance_probe(rdram, (uint32_t)ctx->r6);
#endif
    // 0x80146070: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80146074: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80146078: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x8014607C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80146080: jalr        $t9
    // 0x80146084: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80146084: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_2:
    // 0x80146088: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8014608C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80146090: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80146094: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x80146098: addu        $v0, $a0, $t2
    ctx->r2 = ADD32(ctx->r4, ctx->r10);
    // 0x8014609C: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801460A0: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801460A4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801460A8: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801460AC: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801460B0: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x801460B4: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgstate_dispatch_probe(rdram, (uint32_t)ctx->r4, "update_a");
#endif
    // 0x801460B8: lw          $t9, -0x2C50($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2C50);
    // 0x801460BC: jalr        $t9
    // 0x801460C0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801460C0: nop

    after_3:
    // 0x801460C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801460C8:
    // 0x801460C8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801460CC: jr          $ra
    // 0x801460D0: nop

#if LOD_ENABLE_NI0E_TRACE
    {
        const gpr lod_ni0e_textreg_gpr = (gpr)(int32_t)0x8019EECCu;
        const uint32_t lod_ni0e_textreg_ptr = (uint32_t)MEM_W(0x0, lod_ni0e_textreg_gpr);
        lod_ni0e_textreg_fields_probe(rdram, lod_ni0e_textreg_ptr, "func_80145FD4-exit");
    }
#endif
    return;
    // 0x801460D0: nop

;}
RECOMP_FUNC void func_801460D4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801460D4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801460D8: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x801460DC: lw          $t6, 0x295C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X295C);
    // 0x801460E0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801460E4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801460E8: beq         $t6, $zero, L_8014614C
    if (ctx->r14 == 0) {
        // 0x801460EC: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_8014614C;
    }
    // 0x801460EC: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801460F0: lw          $t7, 0x2960($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X2960);
    // 0x801460F4: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801460F8: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x801460FC: beq         $t7, $zero, L_8014614C
    if (ctx->r15 == 0) {
        // 0x80146100: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_8014614C;
    }
    // 0x80146100: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80146104: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80146108: jalr        $t9
    // 0x8014610C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8014610C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x80146110: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80146114: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80146118: lh          $t8, 0xE($a0)
    ctx->r24 = MEM_H(ctx->r4, 0XE);
    // 0x8014611C: sll         $t0, $t8, 1
    ctx->r8 = S32(ctx->r24 << 1);
    // 0x80146120: addu        $v0, $a0, $t0
    ctx->r2 = ADD32(ctx->r4, ctx->r8);
    // 0x80146124: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x80146128: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x8014612C: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80146130: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80146134: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80146138: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x8014613C: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgstate_dispatch_probe(rdram, (uint32_t)ctx->r4, "update_b");
#endif
    // 0x80146140: lw          $t9, -0x2C50($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2C50);
    // 0x80146144: jalr        $t9
    // 0x80146148: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80146148: nop

    after_1:
L_8014614C:
    // 0x8014614C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80146150: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80146154: jr          $ra
    // 0x80146158: nop

    return;
    // 0x80146158: nop

;}
RECOMP_FUNC void func_8014615C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014615C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80146160: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80146164: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80146168: lui         $t6, 0x9999
    ctx->r14 = S32(0X9999 << 16);
    // 0x8014616C: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80146170: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80146174: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_enter(rdram, (uint32_t)ctx->r4);
#endif
    // 0x80146178: ori         $t6, $t6, 0x99FF
    ctx->r14 = ctx->r14 | 0X99FF;
    // 0x8014617C: sw          $t6, 0x2B0C($s0)
    MEM_W(0X2B0C, ctx->r16) = ctx->r14;
    // 0x80146180: jal         0x801476D0
    // 0x80146184: lw          $a0, 0x2960($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2960);
    func_801476D0(rdram, ctx);
        goto after_0;
    // 0x80146184: lw          $a0, 0x2960($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2960);
    after_0:
    // 0x80146188: jal         0x801462B0
    // 0x8014618C: nop

    func_801462B0(rdram, ctx);
        goto after_1;
    // 0x8014618C: nop

    after_1:
    // 0x80146190: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x80146194: jal         0x80002410
    // 0x80146198: addiu       $a1, $zero, 0x1AC
    ctx->r5 = ADD32(0, 0X1AC);
    object_createAndSetChild(rdram, ctx);
        goto after_2;
    // 0x80146198: addiu       $a1, $zero, 0x1AC
    ctx->r5 = ADD32(0, 0X1AC);
    after_2:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_child(0x1AC, (uint32_t)ctx->r2);
#endif
    // 0x8014619C: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x801461A0: jal         0x80002410
    // 0x801461A4: addiu       $a1, $zero, 0x1AD
    ctx->r5 = ADD32(0, 0X1AD);
    object_createAndSetChild(rdram, ctx);
        goto after_3;
    // 0x801461A4: addiu       $a1, $zero, 0x1AD
    ctx->r5 = ADD32(0, 0X1AD);
    after_3:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_child(0x1AD, (uint32_t)ctx->r2);
#endif
    // 0x801461A8: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x801461AC: jal         0x80002410
    // 0x801461B0: addiu       $a1, $zero, 0x1B2
    ctx->r5 = ADD32(0, 0X1B2);
    object_createAndSetChild(rdram, ctx);
        goto after_4;
    // 0x801461B0: addiu       $a1, $zero, 0x1B2
    ctx->r5 = ADD32(0, 0X1B2);
    after_4:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_child(0x1B2, (uint32_t)ctx->r2);
#endif
    // 0x801461B4: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x801461B8: jal         0x80002410
    // 0x801461BC: addiu       $a1, $zero, 0x1B1
    ctx->r5 = ADD32(0, 0X1B1);
    object_createAndSetChild(rdram, ctx);
        goto after_5;
    // 0x801461BC: addiu       $a1, $zero, 0x1B1
    ctx->r5 = ADD32(0, 0X1B1);
    after_5:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_child(0x1B1, (uint32_t)ctx->r2);
#endif
    // 0x801461C0: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x801461C4: jal         0x80002410
    // 0x801461C8: addiu       $a1, $zero, 0x1D0
    ctx->r5 = ADD32(0, 0X1D0);
    object_createAndSetChild(rdram, ctx);
        goto after_6;
    // 0x801461C8: addiu       $a1, $zero, 0x1D0
    ctx->r5 = ADD32(0, 0X1D0);
    after_6:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_child(0x1D0, (uint32_t)ctx->r2);
#endif
    // 0x801461CC: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x801461D0: jal         0x80002410
    // 0x801461D4: addiu       $a1, $zero, 0x1AE
    ctx->r5 = ADD32(0, 0X1AE);
    object_createAndSetChild(rdram, ctx);
        goto after_7;
    // 0x801461D4: addiu       $a1, $zero, 0x1AE
    ctx->r5 = ADD32(0, 0X1AE);
    after_7:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_child(0x1AE, (uint32_t)ctx->r2);
#endif
    // 0x801461D8: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x801461DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801461E0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801461E4: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x801461E8: jalr        $t9
    // 0x801461EC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x801461EC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_8:
    // 0x801461F0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x801461F4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801461F8: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x801461FC: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x80146200: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x80146204: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80146208: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x8014620C: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80146210: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80146214: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80146218: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x8014621C: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgstate_dispatch_probe(rdram, (uint32_t)ctx->r4, "create");
#endif
    // 0x80146220: lw          $t9, -0x2C50($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2C50);
    // 0x80146224: jalr        $t9
    // 0x80146228: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x80146228: nop

    after_9:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_bgcreate_exit(rdram, (uint32_t)ctx->r4);
#endif
    // 0x8014622C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80146230: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80146234: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80146238: jr          $ra
    // 0x8014623C: nop

    return;
    // 0x8014623C: nop

;}
RECOMP_FUNC void bgState_activate(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_activation_gate_probe(rdram, (uint32_t)ctx->r4);
#endif
    // 0x80146240: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80146244: addiu       $v0, $v0, -0x2E80
    ctx->r2 = ADD32(ctx->r2, -0X2E80);
    // 0x80146248: lw          $t6, 0x14($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X14);
    // 0x8014624C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80146250: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80146254: beq         $t6, $zero, L_80146298
    if (ctx->r14 == 0) {
        // 0x80146258: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_80146298;
    }
    // 0x80146258: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8014625C: lw          $t7, 0x18($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X18);
    // 0x80146260: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80146264: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80146268: beq         $t7, $zero, L_80146298
    if (ctx->r15 == 0) {
        // 0x8014626C: lui         $at, 0x1000
        ctx->r1 = S32(0X1000 << 16);
            goto L_80146298;
    }
    // 0x8014626C: lui         $at, 0x1000
    ctx->r1 = S32(0X1000 << 16);
    // 0x80146270: lw          $t8, 0x2908($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2908);
    // 0x80146274: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x80146278: sw          $t0, 0x2B10($v0)
    MEM_W(0X2B10, ctx->r2) = ctx->r8;
    // 0x8014627C: or          $t9, $t8, $at
    ctx->r25 = ctx->r24 | ctx->r1;
    // 0x80146280: sw          $t9, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r25;
    // 0x80146284: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80146288: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8014628C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80146290: jalr        $t9
    // 0x80146294: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80146294: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_80146298:
    // 0x80146298: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8014629C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801462A0: jr          $ra
    // 0x801462A4: nop

    return;
    // 0x801462A4: nop

;}
RECOMP_FUNC void func_801462A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801462A8: jr          $ra
    // 0x801462AC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x801462AC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_801462B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801462B0: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x801462B4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801462B8: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x801462BC: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x801462C0: lh          $t6, 0x28D0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X28D0);
    // 0x801462C4: lh          $t9, 0x28D2($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X28D2);
    // 0x801462C8: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x801462CC: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x801462D0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801462D4: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x801462D8: subu        $t0, $t0, $t9
    ctx->r8 = SUB32(ctx->r8, ctx->r25);
    // 0x801462DC: lw          $t8, -0xB8($t8)
    ctx->r24 = MEM_W(ctx->r24, -0XB8);
    // 0x801462E0: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x801462E4: subu        $t0, $t0, $t9
    ctx->r8 = SUB32(ctx->r8, ctx->r25);
    // 0x801462E8: sll         $t0, $t0, 1
    ctx->r8 = S32(ctx->r8 << 1);
    // 0x801462EC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801462F0: addu        $v0, $t8, $t0
    ctx->r2 = ADD32(ctx->r24, ctx->r8);
    // 0x801462F4: lh          $t1, 0x2($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X2);
    // 0x801462F8: lw          $t2, 0x2B2C($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X2B2C);
    // 0x801462FC: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    // 0x80146300: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x80146304: nop

    // 0x80146308: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8014630C: swc1        $f0, 0x50($t2)
    MEM_W(0X50, ctx->r10) = ctx->f0.u32l;
    // 0x80146310: lw          $t3, 0x2968($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X2968);
    // 0x80146314: swc1        $f0, 0x14($t3)
    MEM_W(0X14, ctx->r11) = ctx->f0.u32l;
    // 0x80146318: lh          $t4, 0x4($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X4);
    // 0x8014631C: lw          $t5, 0x2B2C($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X2B2C);
    // 0x80146320: mtc1        $t4, $f6
    ctx->f6.u32l = ctx->r12;
    // 0x80146324: nop

    // 0x80146328: cvt.s.w     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    ctx->f0.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8014632C: swc1        $f0, 0x54($t5)
    MEM_W(0X54, ctx->r13) = ctx->f0.u32l;
    // 0x80146330: lw          $t6, 0x2968($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X2968);
    // 0x80146334: swc1        $f0, 0x18($t6)
    MEM_W(0X18, ctx->r14) = ctx->f0.u32l;
    // 0x80146338: lh          $t7, 0x6($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X6);
    // 0x8014633C: lw          $t9, 0x2B2C($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X2B2C);
    // 0x80146340: mtc1        $t7, $f8
    ctx->f8.u32l = ctx->r15;
    // 0x80146344: nop

    // 0x80146348: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8014634C: swc1        $f0, 0x58($t9)
    MEM_W(0X58, ctx->r25) = ctx->f0.u32l;
    // 0x80146350: lw          $t8, 0x2968($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X2968);
    // 0x80146354: swc1        $f0, 0x1C($t8)
    MEM_W(0X1C, ctx->r24) = ctx->f0.u32l;
    // 0x80146358: lw          $t1, 0x2B2C($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X2B2C);
    // 0x8014635C: lhu         $t0, 0x8($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X8);
    // 0x80146360: sh          $t0, 0x5E($t1)
    MEM_H(0X5E, ctx->r9) = ctx->r8;
    // 0x80146364: lh          $t2, 0xA($v0)
    ctx->r10 = MEM_H(ctx->r2, 0XA);
    // 0x80146368: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    // 0x8014636C: mtc1        $t2, $f10
    ctx->f10.u32l = ctx->r10;
    // 0x80146370: nop

    // 0x80146374: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80146378: swc1        $f16, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f16.u32l;
    // 0x8014637C: lh          $t3, 0xC($v0)
    ctx->r11 = MEM_H(ctx->r2, 0XC);
    // 0x80146380: mtc1        $t3, $f18
    ctx->f18.u32l = ctx->r11;
    // 0x80146384: nop

    // 0x80146388: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8014638C: swc1        $f4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f4.u32l;
    // 0x80146390: lh          $t4, 0xE($v0)
    ctx->r12 = MEM_H(ctx->r2, 0XE);
    // 0x80146394: sw          $v0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r2;
    // 0x80146398: mtc1        $t4, $f6
    ctx->f6.u32l = ctx->r12;
    // 0x8014639C: nop

    // 0x801463A0: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801463A4: jal         0x8017994C
    // 0x801463A8: swc1        $f8, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f8.u32l;
    func_8017994C(rdram, ctx);
        goto after_0;
    // 0x801463A8: swc1        $f8, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f8.u32l;
    after_0:
    // 0x801463AC: lw          $v0, 0x34($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X34);
    // 0x801463B0: lw          $a0, 0x2B70($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B70);
    // 0x801463B4: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    // 0x801463B8: lh          $t5, 0x10($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X10);
    // 0x801463BC: mtc1        $t5, $f10
    ctx->f10.u32l = ctx->r13;
    // 0x801463C0: nop

    // 0x801463C4: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801463C8: swc1        $f16, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f16.u32l;
    // 0x801463CC: lh          $t6, 0x12($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X12);
    // 0x801463D0: mtc1        $t6, $f18
    ctx->f18.u32l = ctx->r14;
    // 0x801463D4: nop

    // 0x801463D8: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x801463DC: swc1        $f4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f4.u32l;
    // 0x801463E0: lh          $t7, 0x14($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X14);
    // 0x801463E4: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x801463E8: nop

    // 0x801463EC: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801463F0: jal         0x8017DF5C
    // 0x801463F4: swc1        $f8, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f8.u32l;
    func_8017DF5C(rdram, ctx);
        goto after_1;
    // 0x801463F4: swc1        $f8, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f8.u32l;
    after_1:
    // 0x801463F8: jal         0x80148138
    // 0x801463FC: nop

    func_80148138(rdram, ctx);
        goto after_2;
    // 0x801463FC: nop

    after_2:
    // 0x80146400: jal         0x80179D38
    // 0x80146404: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    func_80179D38(rdram, ctx);
        goto after_3;
    // 0x80146404: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    after_3:
    // 0x80146408: jal         0x80180250
    // 0x8014640C: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    func_80180250(rdram, ctx);
        goto after_4;
    // 0x8014640C: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    after_4:
    // 0x80146410: lh          $t9, 0x28D0($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X28D0);
    // 0x80146414: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x80146418: addiu       $t0, $t0, 0x6470
    ctx->r8 = ADD32(ctx->r8, 0X6470);
    // 0x8014641C: sll         $t8, $t9, 2
    ctx->r24 = S32(ctx->r25 << 2);
    // 0x80146420: addu        $a1, $t8, $t0
    ctx->r5 = ADD32(ctx->r24, ctx->r8);
    // 0x80146424: jal         0x80182A20
    // 0x80146428: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    func_80182A20(rdram, ctx);
        goto after_5;
    // 0x80146428: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    after_5:
    // 0x8014642C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80146430: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80146434: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80146438: jr          $ra
    // 0x8014643C: nop

    return;
    // 0x8014643C: nop

;}
RECOMP_FUNC void func_80146440(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80146440: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80146444: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80146448: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x8014644C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80146450: addiu       $a0, $zero, 0x2F4
    ctx->r4 = ADD32(0, 0X2F4);
    // 0x80146454: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
L_80146458:
    // 0x80146458: sw          $zero, 0x28($v1)
    MEM_W(0X28, ctx->r3) = 0;
    // 0x8014645C: addiu       $a1, $a1, 0x54
    ctx->r5 = ADD32(ctx->r5, 0X54);
    // 0x80146460: addiu       $v1, $v1, 0x54
    ctx->r3 = ADD32(ctx->r3, 0X54);
    // 0x80146464: addiu       $v0, $v0, 0x54
    ctx->r2 = ADD32(ctx->r2, 0X54);
    // 0x80146468: sw          $zero, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = 0;
    // 0x8014646C: sw          $zero, -0x14($v0)
    MEM_W(-0X14, ctx->r2) = 0;
    // 0x80146470: sw          $zero, -0x8($v0)
    MEM_W(-0X8, ctx->r2) = 0;
    // 0x80146474: sw          $zero, -0x18($v0)
    MEM_W(-0X18, ctx->r2) = 0;
    // 0x80146478: sw          $zero, -0xC($v0)
    MEM_W(-0XC, ctx->r2) = 0;
    // 0x8014647C: sw          $zero, -0x1C($v0)
    MEM_W(-0X1C, ctx->r2) = 0;
    // 0x80146480: sw          $zero, -0x20($v0)
    MEM_W(-0X20, ctx->r2) = 0;
    // 0x80146484: bne         $a1, $a0, L_80146458
    if (ctx->r5 != ctx->r4) {
        // 0x80146488: sw          $zero, -0x10($v0)
        MEM_W(-0X10, ctx->r2) = 0;
            goto L_80146458;
    }
    // 0x80146488: sw          $zero, -0x10($v0)
    MEM_W(-0X10, ctx->r2) = 0;
    // 0x8014648C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80146490: addiu       $a0, $a0, -0x2E30
    ctx->r4 = ADD32(ctx->r4, -0X2E30);
    // 0x80146494: jal         0x80000F30
    // 0x80146498: addiu       $a1, $zero, 0x1200
    ctx->r5 = ADD32(0, 0X1200);
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x80146498: addiu       $a1, $zero, 0x1200
    ctx->r5 = ADD32(0, 0X1200);
    after_0:
    // 0x8014649C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801464A0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801464A4: sh          $zero, -0x1C30($at)
    MEM_H(-0X1C30, ctx->r1) = 0;
    // 0x801464A8: jr          $ra
    // 0x801464AC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x801464AC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_801464B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801464B0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801464B4: bne         $a0, $zero, L_801464C4
    if (ctx->r4 != 0) {
        // 0x801464B8: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_801464C4;
    }
    // 0x801464B8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801464BC: b           L_801464F4
    // 0x801464C0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801464F4;
    // 0x801464C0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801464C4:
    // 0x801464C4: jal         0x8014659C
    // 0x801464C8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    func_8014659C(rdram, ctx);
        goto after_0;
    // 0x801464C8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x801464CC: bgez        $v0, L_801464DC
    if (SIGNED(ctx->r2) >= 0) {
        // 0x801464D0: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_801464DC;
    }
    // 0x801464D0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x801464D4: b           L_801464F4
    // 0x801464D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801464F4;
    // 0x801464D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801464DC:
    // 0x801464DC: sll         $t6, $v1, 3
    ctx->r14 = S32(ctx->r3 << 3);
    // 0x801464E0: addu        $t6, $t6, $v1
    ctx->r14 = ADD32(ctx->r14, ctx->r3);
    // 0x801464E4: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x801464E8: addiu       $t7, $t7, -0x2E30
    ctx->r15 = ADD32(ctx->r15, -0X2E30);
    // 0x801464EC: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x801464F0: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
L_801464F4:
    // 0x801464F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801464F8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801464FC: jr          $ra
    // 0x80146500: nop

    return;
    // 0x80146500: nop

;}
RECOMP_FUNC void func_80146504(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80146504: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80146508: bne         $a0, $zero, L_80146518
    if (ctx->r4 != 0) {
        // 0x8014650C: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_80146518;
    }
    // 0x8014650C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80146510: b           L_80146548
    // 0x80146514: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80146548;
    // 0x80146514: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80146518:
    // 0x80146518: jal         0x8014659C
    // 0x8014651C: nop

    func_8014659C(rdram, ctx);
        goto after_0;
    // 0x8014651C: nop

    after_0:
    // 0x80146520: bgez        $v0, L_80146530
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80146524: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80146530;
    }
    // 0x80146524: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80146528: b           L_80146548
    // 0x8014652C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80146548;
    // 0x8014652C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80146530:
    // 0x80146530: sll         $t6, $v1, 3
    ctx->r14 = S32(ctx->r3 << 3);
    // 0x80146534: addu        $t6, $t6, $v1
    ctx->r14 = ADD32(ctx->r14, ctx->r3);
    // 0x80146538: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8014653C: addiu       $t7, $t7, -0x2E30
    ctx->r15 = ADD32(ctx->r15, -0X2E30);
    // 0x80146540: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80146544: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
L_80146548:
    // 0x80146548: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8014654C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80146550: jr          $ra
    // 0x80146554: nop

    return;
    // 0x80146554: nop

;}
RECOMP_FUNC void func_80146558(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80146558: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8014655C: lh          $t6, -0x1C30($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X1C30);
    // 0x80146560: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80146564: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80146568: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8014656C: slt         $at, $t6, $a0
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x80146570: beq         $at, $zero, L_80146580
    if (ctx->r1 == 0) {
        // 0x80146574: sll         $t7, $a0, 3
        ctx->r15 = S32(ctx->r4 << 3);
            goto L_80146580;
    }
    // 0x80146574: sll         $t7, $a0, 3
    ctx->r15 = S32(ctx->r4 << 3);
    // 0x80146578: jr          $ra
    // 0x8014657C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x8014657C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80146580:
    // 0x80146580: addu        $t7, $t7, $a0
    ctx->r15 = ADD32(ctx->r15, ctx->r4);
    // 0x80146584: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x80146588: addiu       $t8, $t8, -0x2E30
    ctx->r24 = ADD32(ctx->r24, -0X2E30);
    // 0x8014658C: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80146590: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x80146594: jr          $ra
    // 0x80146598: nop

    return;
    // 0x80146598: nop

;}
RECOMP_FUNC void func_8014659C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014659C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801465A0: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x801465A4: addiu       $v0, $v0, -0x2E30
    ctx->r2 = ADD32(ctx->r2, -0X2E30);
    // 0x801465A8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x801465AC: lh          $a3, -0x1C30($a3)
    ctx->r7 = MEM_H(ctx->r7, -0X1C30);
L_801465B0:
    // 0x801465B0: lw          $t6, 0x10($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X10);
    // 0x801465B4: bnel        $a0, $t6, L_801465E0
    if (ctx->r4 != ctx->r14) {
        // 0x801465B8: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_801465E0;
    }
    goto skip_0;
    // 0x801465B8: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_0:
    // 0x801465BC: lw          $t7, 0x14($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X14);
    // 0x801465C0: bnel        $a1, $t7, L_801465E0
    if (ctx->r5 != ctx->r15) {
        // 0x801465C4: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_801465E0;
    }
    goto skip_1;
    // 0x801465C4: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_1:
    // 0x801465C8: beql        $a2, $zero, L_801465F0
    if (ctx->r6 == 0) {
        // 0x801465CC: slt         $at, $v1, $a3
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r7) ? 1 : 0;
            goto L_801465F0;
    }
    goto skip_2;
    // 0x801465CC: slt         $at, $v1, $a3
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r7) ? 1 : 0;
    skip_2:
    // 0x801465D0: lhu         $t8, 0x0($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X0);
    // 0x801465D4: beql        $a2, $t8, L_801465F0
    if (ctx->r6 == ctx->r24) {
        // 0x801465D8: slt         $at, $v1, $a3
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r7) ? 1 : 0;
            goto L_801465F0;
    }
    goto skip_3;
    // 0x801465D8: slt         $at, $v1, $a3
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r7) ? 1 : 0;
    skip_3:
    // 0x801465DC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
L_801465E0:
    // 0x801465E0: slt         $at, $v1, $a3
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r7) ? 1 : 0;
    // 0x801465E4: bne         $at, $zero, L_801465B0
    if (ctx->r1 != 0) {
        // 0x801465E8: addiu       $v0, $v0, 0x24
        ctx->r2 = ADD32(ctx->r2, 0X24);
            goto L_801465B0;
    }
    // 0x801465E8: addiu       $v0, $v0, 0x24
    ctx->r2 = ADD32(ctx->r2, 0X24);
    // 0x801465EC: slt         $at, $v1, $a3
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r7) ? 1 : 0;
L_801465F0:
    // 0x801465F0: bne         $at, $zero, L_80146600
    if (ctx->r1 != 0) {
        // 0x801465F4: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80146600;
    }
    // 0x801465F4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x801465F8: jr          $ra
    // 0x801465FC: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    return;
    // 0x801465FC: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80146600:
    // 0x80146600: jr          $ra
    // 0x80146604: nop

    return;
    // 0x80146604: nop

;}
