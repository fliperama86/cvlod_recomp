#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

#if LOD_ENABLE_NI0E_TRACE
#include <stdbool.h>
#include <stdio.h>

// Round 2: retry-crash context probe for func_8016890C (below), the
// innermost frame of the death/retry SIGBUS (fault at rdram+0xBF800008;
// 0xBF800000 is the IEEE-754 bit pattern for -1.0f). func_8016890C(a0=obj,
// a1=str) is a text-run helper called repeatedly from func_801682D0 (one of
// the "Henry overlay-system" dialog draw paths). At entry (once a0 != 0) it
// unconditionally does `bzero_recomp(fixed_buf, *(uint32_t*)(obj+0))`
// (funcs_57.c, the `lw $a1,0x0($s5)` feeding the `jal bzero_recomp` right
// after the null check) -- obj+0x0 is read as a byte COUNT for a zero-fill.
// If that field holds 0xBF800000 (a corrupted/uninitialized -1.0f sentinel
// instead of a small integer), bzero_recomp would try to zero an enormous
// span starting from a fixed buffer, which is a plausible root cause for a
// SIGBUS landing near rdram+0xBF800000ish. This probe only logs; it never
// alters obj, ctx, or control flow, so if this is the real cause the crash
// still happens right after (with the bad value now on record).
static int lod_ni0e_figwalk_depth = 0;
static uint32_t lod_ni0e_figwalk_bad_logged = 0;
static uint32_t lod_ni0e_figwalk_deep_logged = 0;

static inline bool lod_ni0e_figwalk_addr_ok(uint32_t addr, uint32_t size) {
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return addr != 0 && phys <= 0x800000u && size <= 0x800000u - phys;
}

static inline gpr lod_ni0e_figwalk_addr_gpr(uint32_t addr) {
    return (gpr)(int32_t)addr;
}

// Entry probe: bumps + checks the recursion-depth counter (runaway-recursion
// evidence), then validates obj and its first dereferenced field (obj+0x0,
// the bzero_recomp byte count described above).
static void lod_ni0e_figwalk_enter(uint8_t* rdram, uint32_t obj) {
    lod_ni0e_figwalk_depth++;
    if (lod_ni0e_figwalk_depth > 64 && lod_ni0e_figwalk_deep_logged < 40u) {
        lod_ni0e_figwalk_deep_logged++;
        fprintf(stderr, "[NI0E_TRACE] figwalk-deep depth=%d obj=0x%08X\n",
                lod_ni0e_figwalk_depth, obj);
    }

    if (obj == 0 || lod_ni0e_figwalk_bad_logged >= 40u) {
        return;
    }

    const bool obj_ok = lod_ni0e_figwalk_addr_ok(obj, 4);
    uint32_t field0 = 0;
    if (obj_ok) {
        field0 = (uint32_t)MEM_W(0x0, lod_ni0e_figwalk_addr_gpr(obj));
    }
    const bool field_bad = obj_ok && ((field0 >> 16) == 0xBF80u);
    if (!obj_ok || field_bad) {
        lod_ni0e_figwalk_bad_logged++;
        fprintf(stderr,
                "[NI0E_TRACE] figwalk-bad ptr=0x%08X obj_ok=%d field=0x%08X depth=%d\n",
                obj, obj_ok ? 1 : 0, field0, lod_ni0e_figwalk_depth);
    }
}

static void lod_ni0e_figwalk_leave(void) {
    if (lod_ni0e_figwalk_depth > 0) {
        lod_ni0e_figwalk_depth--;
    }
}

// Round 3: crash-producer probe for func_801682D0, the caller identified in
// the round-2 native backtrace as the direct source of the bad a0 values fed
// into func_8016890C. func_801682D0 calls func_8016890C exactly 4 times:
// three single fixed-offset calls reading obj/str pairs out of a struct
// pointed to by $s2 (fields +0x10/+0x28, +0x14/+0x2C, +0x18/+0x30), then a
// loop (index $s1, byte cursor $s0) over two parallel arrays whose bases are
// $s2+0x24 (obj array) and $s2+0x38 (str array), bounded by a count at
// $s2+0x20. This probe never touches ctx/obj/control flow, only logs.
static uint32_t lod_ni0e_figsrc_entry_calls = 0;

static void lod_ni0e_figsrc_entry_probe(uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r7) {
    lod_ni0e_figsrc_entry_calls++;
    const bool periodic = (lod_ni0e_figsrc_entry_calls % 600u) == 0u;
    if (lod_ni0e_figsrc_entry_calls > 20u && !periodic) {
        return;
    }
    fprintf(stderr,
            "[NI0E_TRACE] figsrc-entry r4=0x%08X r5=0x%08X r6=0x%08X r7=0x%08X call=%u\n",
            r4, r5, r6, r7, lod_ni0e_figsrc_entry_calls);
}

static uint32_t lod_ni0e_figsrc_bad_logged = 0;

static inline bool lod_ni0e_figsrc_addr_ok(uint32_t addr) {
    if (addr == 0) {
        return false;
    }
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return phys <= 0x800000u - 4u;
}

// Round 7: companion to the entry probe above, called right after $s2 is
// loaded from the RDRAM global at 0x8019EECC (0x80168300, `lw $s2,
// -0x1134($s2)` below). Dumps the same six fields the three fixed-offset
// calls and the array loop later in this function consume: +0x10/+0x14/+0x18
// (object pointers), +0x20 (array count), +0x24 (object array base), +0x38
// (string array base). Reuses lod_ni0e_figsrc_entry_calls (already bumped by
// the entry probe for this same invocation) so the two log lines share the
// same rate-limit decision and correlate 1:1 by call number.
static void lod_ni0e_figsrc_entry_fields_probe(uint8_t* rdram, uint32_t s2) {
    const bool periodic = (lod_ni0e_figsrc_entry_calls % 600u) == 0u;
    if (lod_ni0e_figsrc_entry_calls > 20u && !periodic) {
        return;
    }
    if (!lod_ni0e_figsrc_addr_ok(s2)) {
        fprintf(stderr,
                "[NI0E_TRACE] figsrc-entry-fields call=%u s2=0x%08X (invalid, skipped)\n",
                lod_ni0e_figsrc_entry_calls, s2);
        return;
    }
    const gpr s2_gpr = (gpr)(int32_t)s2;
    const uint32_t f10 = (uint32_t)MEM_W(0x10, s2_gpr);
    const uint32_t f14 = (uint32_t)MEM_W(0x14, s2_gpr);
    const uint32_t f18 = (uint32_t)MEM_W(0x18, s2_gpr);
    const uint32_t f20 = (uint32_t)MEM_W(0x20, s2_gpr);
    const uint32_t f24 = (uint32_t)MEM_W(0x24, s2_gpr);
    const uint32_t f38 = (uint32_t)MEM_W(0x38, s2_gpr);
    fprintf(stderr,
            "[NI0E_TRACE] figsrc-entry-fields call=%u s2=0x%08X f10=0x%08X f14=0x%08X "
            "f18=0x%08X f20=0x%08X f24=0x%08X f38=0x%08X\n",
            lod_ni0e_figsrc_entry_calls, s2, f10, f14, f18, f20, f24, f38);
}

// Checks the outgoing a0 (the obj argument about to be passed to
// func_8016890C) for the two known-bad shapes: fails the object-pointer
// range check, or its high halfword is 0x3F80/0xBF80 (the +1.0f/-1.0f float
// bit patterns seen in the round-2 capture). `container`/`cursor`/`index`
// describe where a0 was read from: for the three fixed-offset calls,
// container is $s2 and cursor is the field offset used (index -1, not a
// loop); for the loop call, container is the obj-array base ($s2+0x24's
// value), cursor is the byte offset ($s0), and index is the loop counter
// ($s1).
static void lod_ni0e_figsrc_check(uint32_t a0, uint32_t container, uint32_t cursor,
                                   int32_t index, const char* site) {
    // Round 4 fix: a0==0 is benign (func_8016890C null-checks a0 itself
    // before touching it), but round 3's cap was being consumed by these
    // zero calls before the real bad pointer (0x3F800000) ever got logged.
    // Only count/log nonzero a0 that is out of range or carries one of the
    // known float-bit-pattern sentinels.
    if (a0 == 0 || lod_ni0e_figsrc_bad_logged >= 20u) {
        return;
    }
    const uint32_t hi = a0 >> 16;
    const bool bad = !lod_ni0e_figsrc_addr_ok(a0) || hi == 0x3F80u || hi == 0xBF80u;
    if (!bad) {
        return;
    }
    lod_ni0e_figsrc_bad_logged++;
    fprintf(stderr,
            "[NI0E_TRACE] figsrc-bad site=%s a0=0x%08X container=0x%08X cursor=0x%08X "
            "index-ish=%d\n",
            site, a0, container, cursor, index);
}
#endif  // LOD_ENABLE_NI0E_TRACE

// Round 14: LOD_FIX_TEXT_MEASURE_GUARD, defense-in-depth. Independent of
// LOD_ENABLE_NI0E_TRACE (must compile/run standalone). Guards the two ends of
// the days-banner text-measure recursion identified in
// docs/issue27-31-fix-design.md / docs/issue27-31-ni0e-findings.md: the
// global at RDRAM 0x8019EECC (built by func_80145FD4 in funcs_45.c) can hold
// a stale/freed struct pointer on a Henry map transition; func_801682D0
// reads it into $s2 and feeds obj/str pairs to the self-recursive
// func_8016B878 (below), which walks garbage until stack overflow (native
// SIGBUS, see the round 2/3 figwalk probes above). This does not fix the
// stale-pointer root cause -- it only stops the runaway recursion / bad
// pointer walk from crashing the process, and is a strict no-op whenever the
// incoming data is valid.
#ifndef LOD_FIX_TEXT_MEASURE_GUARD
#define LOD_FIX_TEXT_MEASURE_GUARD 0
#endif

#if LOD_FIX_TEXT_MEASURE_GUARD
#include <stdio.h>

// func_8016B878's own recursion-depth counter. Single-threaded (game loop),
// reset to 0 naturally once every top-level call fully unwinds (each entry
// increments, each exit decrements -- see the two hook points below).
static int lod_text_guard_measure_depth = 0;
static int lod_text_guard_measure_logged = 0;

// func_8016890C's incoming-pointer range check ("standard range check" per
// the task brief: nonzero and within the emulated 8MB RDRAM span, matching
// the same convention as the pre-existing figwalk/figsrc NI0E_TRACE probes
// above). Kept independent of LOD_ENABLE_NI0E_TRACE's own copy since this
// flag must work standalone.
static int lod_text_guard_field_logged = 0;

static inline int lod_text_guard_addr_ok(uint32_t addr) {
    if (addr == 0) {
        return 0;
    }
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return phys <= 0x800000u - 4u;
}

// Round 15: func_80168AA0's own recursion-depth counter. func_80168AA0 is
// the draw-side sibling of func_8016B878 below -- same table walk at RDRAM
// global 0x8019EF08 (self-recursive via two internal `jal 0x80168AA0`
// sites), reached from func_8016890C instead of func_8016B62C. Round 14
// only capped func_8016B878's (measure-side) recursion; func_80168AA0 was
// left completely unguarded, so the same stale/circular table data that
// used to runaway-recurse in func_8016B878 can still runaway-recurse here
// -- this is the leading suspect for round 14's "guard fired but the game
// still froze" result (see the workspace guard below for the actual fix
// layered on top). Kept as its own independent static counter/flag (not
// shared with lod_text_guard_measure_depth) since the two functions are
// reached via unrelated call chains and, in principle, could nest relative
// to each other across different in-flight draw/measure calls.
static int lod_text_guard_draw_depth = 0;
static int lod_text_guard_draw_logged = 0;

// Round 15 (Task A): workspace-validity check shared by func_801682D0 (the
// draw dispatcher) and func_8016AE10 (its measure-mode twin). Both load the
// same possibly-stale struct pointer from RDRAM global 0x8019EECC into $s2
// with no check at all, then unconditionally feed its +0x10/+0x14/+0x18
// (and, for the measure twin, +0x1C) fields plus the +0x20-count/+0x24-
// array/+0x38-array loop straight into the func_8016890C/func_8016B62C
// dispatch chain -- exactly the data that ends up walked by the
// func_80168AA0/func_8016B878 recursive walkers above. Catches the three
// signatures the round 15 task calls out: s2 itself zero, s2 out of the
// emulated RDRAM span, or s2's own +0x10 field (the first fixed-offset
// obj pointer the caller reads) nonzero-and-out-of-range -- the stale-
// pointer signature the round 2-4 figsrc/figwalk captures identified.
static inline int lod_text_guard_workspace_invalid(uint8_t* rdram, uint32_t s2) {
    if (s2 == 0 || !lod_text_guard_addr_ok(s2)) {
        return 1;
    }
    const uint32_t field10 = (uint32_t)MEM_W(0X10, (gpr)(int32_t)s2);
    if (field10 != 0 && !lod_text_guard_addr_ok(field10)) {
        return 1;
    }
    return 0;
}

static int lod_text_guard_workspace_draw_logged = 0;
static int lod_text_guard_workspace_measure_logged = 0;

// Round 18 (Task B): array-loop guard. Repro18's live capture caught
// func_801682D0 crashing at native offset +2872 (frame #2 in the native
// backtrace -- the fault is inside func_801682D0 itself, not a callee),
// fault address rdram+0x8707F800 (phys 0x707F800, past the 8MB RDRAM span),
// immediately after `[NI0E_TRACE] figsrc-bad site=field14 a0=0xD7000002
// container=0x80380F04 ...` and `[TEXT_GUARD] func_8016890C rejected
// out-of-range obj=0xD7000002` -- i.e. the round-14 per-call guard on
// func_8016890C correctly caught and neutralized that one bad field, and
// the crash happened *after* that rejection, with no further figsrc-bad/
// TEXT_GUARD line before it. That rules out the crash being inside another
// func_8016890C call (its own guard would have applied there too, silently
// after the first fprintf since lod_text_guard_field_logged is a one-shot
// flag -- but the fault PC is in func_801682D0, not func_8016890C, so the
// fault did not happen inside that callee at all).
//
// Reading the remaining span: func_801682D0/func_8016AE10's
// variable-length-run loop (count at s2+0x20, parallel obj/str array BASE
// pointers at s2+0x24/s2+0x38) computes each iteration's a0/a1 with a
// direct, unguarded `MEM_W(objBase+cursor, 0)` / `MEM_W(strBase+cursor, 0)`
// read *in the caller*, before func_8016890C/func_8016B62C is ever called --
// so even though those callees have their own per-call obj range-check
// (round 14 guard 2 and its func_8016B62C analog), a corrupt array BASE
// pointer (s2+0x24 or s2+0x38 itself holding stale/garbage data, the same
// class of corruption the field14 rejection just demonstrated elsewhere in
// the very same struct) faults right here, before any guard downstream ever
// runs. This is the concrete mechanism for the repro18 crash.
//
// This is intentionally a *separate*, narrower check from
// lod_text_guard_workspace_invalid above rather than widening that one:
// the three/four fixed-offset text-draw calls (independently protected by
// the callee's own guard already) and the variable-length array run are
// independently populated parts of the same shared struct (round 3/7's
// finding -- three fixed slots plus a separately-maintained parallel-array
// "run"), so unconditionally requiring s2+0x24/+0x38 to be valid even when
// the loop count is <=0 (never dereferenced) or when only the fixed slots
// are actually stale would silently drop legitimate, currently-active
// banner draws that have nothing to do with the array run. Instead this is
// checked only when the loop is actually about to execute (count > 0), and
// only gates the loop itself (same safe skip target the existing
// count<=0 case already uses), leaving the fixed-offset calls untouched.
static inline int lod_text_guard_array_invalid(uint8_t* rdram, uint32_t s2) {
    const uint32_t obj_base = (uint32_t)MEM_W(0X24, (gpr)(int32_t)s2);
    const uint32_t str_base = (uint32_t)MEM_W(0X38, (gpr)(int32_t)s2);
    if (obj_base != 0 && !lod_text_guard_addr_ok(obj_base)) {
        return 1;
    }
    if (str_base != 0 && !lod_text_guard_addr_ok(str_base)) {
        return 1;
    }
    return 0;
}

static int lod_text_guard_array_draw_logged = 0;
static int lod_text_guard_array_measure_logged = 0;

// Round 17 (Task B): systematic sweep of every self-recursive function in
// the text/figure family (0x80160000-0x80180000 vaddr range), found with a
// small script that greps each RECOMP_FUNC body for a call to its own name
// (see docs/issue27-31-ni0e-findings.md, round 17). func_80168AA0 and
// func_8016B878 above were already guarded (rounds 14/15); these nine more
// self-recursive walkers in this file were not. Same depth-128-cap pattern:
// each gets its own independent static counter/one-shot log flag since they
// are reached via unrelated call chains and, in principle, could nest
// relative to each other and to the two guards above.
static int lod_text_guard_depth_801685A8 = 0;
static int lod_text_guard_logged_801685A8 = 0;
static int lod_text_guard_depth_80168620 = 0;
static int lod_text_guard_logged_80168620 = 0;
static int lod_text_guard_depth_80169070 = 0;
static int lod_text_guard_logged_80169070 = 0;
static int lod_text_guard_depth_8016B218 = 0;
static int lod_text_guard_logged_8016B218 = 0;
static int lod_text_guard_depth_8016B290 = 0;
static int lod_text_guard_logged_8016B290 = 0;
static int lod_text_guard_depth_8016BC44 = 0;
static int lod_text_guard_logged_8016BC44 = 0;
static int lod_text_guard_depth_8016D200 = 0;
static int lod_text_guard_logged_8016D200 = 0;
static int lod_text_guard_depth_8016D278 = 0;
static int lod_text_guard_logged_8016D278 = 0;
static int lod_text_guard_depth_8016D704 = 0;
static int lod_text_guard_logged_8016D704 = 0;
#endif  // LOD_FIX_TEXT_MEASURE_GUARD

RECOMP_FUNC void func_80167F18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167F18: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80167F1C: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80167F20: jr          $ra
    // 0x80167F24: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    return;
    // 0x80167F24: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
;}
RECOMP_FUNC void func_80167F28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167F28: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80167F2C: jr          $ra
    // 0x80167F30: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    return;
    // 0x80167F30: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
;}
RECOMP_FUNC void func_80167F34(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167F34: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80167F38: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80167F3C: jr          $ra
    // 0x80167F40: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    return;
    // 0x80167F40: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
;}
RECOMP_FUNC void func_80167F44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167F44: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80167F48: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80167F4C: jr          $ra
    // 0x80167F50: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    return;
    // 0x80167F50: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
;}
RECOMP_FUNC void func_80167F54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80167F54: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x80167F58: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80167F5C: or          $s1, $a2, $zero
    ctx->r17 = ctx->r6 | 0;
    // 0x80167F60: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80167F64: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80167F68: sw          $a1, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r5;
    // 0x80167F6C: or          $s0, $a3, $zero
    ctx->r16 = ctx->r7 | 0;
    // 0x80167F70: addiu       $a1, $sp, 0x24
    ctx->r5 = ADD32(ctx->r29, 0X24);
    // 0x80167F74: jal         0x80001090
    // 0x80167F78: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    memory_copy(rdram, ctx);
        goto after_0;
    // 0x80167F78: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_0:
    // 0x80167F7C: lw          $v0, 0x74($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X74);
    // 0x80167F80: lwc1        $f4, 0x54($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80167F84: swc1        $f4, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f4.u32l;
    // 0x80167F88: lwc1        $f6, 0x58($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80167F8C: swc1        $f6, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f6.u32l;
    // 0x80167F90: lwc1        $f8, 0x5C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80167F94: swc1        $f8, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f8.u32l;
    // 0x80167F98: lwc1        $f10, 0x24($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X24);
    // 0x80167F9C: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80167FA0: mul.s       $f18, $f10, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f10.fl);
    // 0x80167FA4: lwc1        $f10, 0x2C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80167FA8: mul.s       $f6, $f4, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f4.fl);
    // 0x80167FAC: add.s       $f8, $f18, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f18.fl + ctx->f6.fl;
    // 0x80167FB0: mul.s       $f4, $f10, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f10.fl);
    // 0x80167FB4: jal         0x800A01E0
    // 0x80167FB8: add.s       $f12, $f4, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80167FB8: add.s       $f12, $f4, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f8.fl;
    after_1:
    // 0x80167FBC: swc1        $f0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f0.u32l;
    // 0x80167FC0: lwc1        $f18, 0x34($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80167FC4: lwc1        $f10, 0x38($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80167FC8: mul.s       $f6, $f18, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x80167FCC: lwc1        $f18, 0x3C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x80167FD0: mul.s       $f4, $f10, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f10.fl);
    // 0x80167FD4: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x80167FD8: mul.s       $f10, $f18, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x80167FDC: jal         0x800A01E0
    // 0x80167FE0: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_2;
    // 0x80167FE0: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
    after_2:
    // 0x80167FE4: swc1        $f0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f0.u32l;
    // 0x80167FE8: lwc1        $f6, 0x44($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80167FEC: lwc1        $f18, 0x48($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80167FF0: mul.s       $f4, $f6, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f6.fl);
    // 0x80167FF4: lwc1        $f6, 0x4C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80167FF8: mul.s       $f10, $f18, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x80167FFC: add.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x80168000: mul.s       $f18, $f6, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f18.fl = MUL_S(ctx->f6.fl, ctx->f6.fl);
    // 0x80168004: jal         0x800A01E0
    // 0x80168008: add.s       $f12, $f18, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_3;
    // 0x80168008: add.s       $f12, $f18, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f8.fl;
    after_3:
    // 0x8016800C: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80168010: lwc1        $f2, 0x0($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80168014: swc1        $f0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f0.u32l;
    // 0x80168018: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016801C: c.eq.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl == ctx->f2.fl;
    // 0x80168020: nop

    // 0x80168024: bc1tl       L_80168064
    if (c1cs) {
        // 0x80168028: lwc1        $f2, 0x4($s0)
        ctx->f2.u32l = MEM_W(ctx->r16, 0X4);
            goto L_80168064;
    }
    goto skip_0;
    // 0x80168028: lwc1        $f2, 0x4($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X4);
    skip_0:
    // 0x8016802C: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x80168030: lwc1        $f4, 0x24($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X24);
    // 0x80168034: lwc1        $f6, 0x34($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80168038: div.s       $f0, $f14, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f14.fl, ctx->f2.fl);
    // 0x8016803C: lwc1        $f8, 0x44($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80168040: mul.s       $f10, $f4, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80168044: nop

    // 0x80168048: mul.s       $f18, $f6, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8016804C: nop

    // 0x80168050: mul.s       $f4, $f8, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x80168054: swc1        $f10, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f10.u32l;
    // 0x80168058: swc1        $f18, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f18.u32l;
    // 0x8016805C: swc1        $f4, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f4.u32l;
    // 0x80168060: lwc1        $f2, 0x4($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X4);
L_80168064:
    // 0x80168064: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80168068: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x8016806C: c.eq.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl == ctx->f2.fl;
    // 0x80168070: nop

    // 0x80168074: bc1tl       L_801680B0
    if (c1cs) {
        // 0x80168078: lwc1        $f2, 0x8($s0)
        ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
            goto L_801680B0;
    }
    goto skip_1;
    // 0x80168078: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
    skip_1:
    // 0x8016807C: div.s       $f0, $f14, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f14.fl, ctx->f2.fl);
    // 0x80168080: lwc1        $f10, 0x28($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80168084: lwc1        $f18, 0x38($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80168088: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016808C: mul.s       $f6, $f10, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80168090: nop

    // 0x80168094: mul.s       $f8, $f18, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80168098: nop

    // 0x8016809C: mul.s       $f10, $f4, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x801680A0: swc1        $f6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f6.u32l;
    // 0x801680A4: swc1        $f8, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f8.u32l;
    // 0x801680A8: swc1        $f10, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f10.u32l;
    // 0x801680AC: lwc1        $f2, 0x8($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X8);
L_801680B0:
    // 0x801680B0: c.eq.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl == ctx->f2.fl;
    // 0x801680B4: nop

    // 0x801680B8: bc1tl       L_801680F4
    if (c1cs) {
        // 0x801680BC: lwc1        $f2, 0x34($sp)
        ctx->f2.u32l = MEM_W(ctx->r29, 0X34);
            goto L_801680F4;
    }
    goto skip_2;
    // 0x801680BC: lwc1        $f2, 0x34($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X34);
    skip_2:
    // 0x801680C0: div.s       $f0, $f14, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f14.fl, ctx->f2.fl);
    // 0x801680C4: lwc1        $f6, 0x2C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x801680C8: lwc1        $f8, 0x3C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x801680CC: lwc1        $f10, 0x4C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x801680D0: mul.s       $f18, $f6, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x801680D4: nop

    // 0x801680D8: mul.s       $f4, $f8, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x801680DC: nop

    // 0x801680E0: mul.s       $f6, $f10, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x801680E4: swc1        $f18, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f18.u32l;
    // 0x801680E8: swc1        $f4, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f4.u32l;
    // 0x801680EC: swc1        $f6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f6.u32l;
    // 0x801680F0: lwc1        $f2, 0x34($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X34);
L_801680F4:
    // 0x801680F4: lwc1        $f0, 0x24($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X24);
    // 0x801680F8: mul.s       $f18, $f2, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801680FC: nop

    // 0x80168100: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80168104: jal         0x800A01E0
    // 0x80168108: add.s       $f12, $f18, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_4;
    // 0x80168108: add.s       $f12, $f18, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f8.fl;
    after_4:
    // 0x8016810C: lwc1        $f12, 0x44($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80168110: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80168114: swc1        $f0, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f0.u32l;
    // 0x80168118: jal         0x801681C8
    // 0x8016811C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_801681C8(rdram, ctx);
        goto after_5;
    // 0x8016811C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_5:
    // 0x80168120: lwc1        $f2, 0x68($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80168124: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80168128: negu        $t6, $v0
    ctx->r14 = SUB32(0, ctx->r2);
    // 0x8016812C: sh          $t6, 0x2($s1)
    MEM_H(0X2, ctx->r17) = ctx->r14;
    // 0x80168130: c.eq.s      $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f2.fl == ctx->f16.fl;
    // 0x80168134: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80168138: bc1fl       L_80168164
    if (!c1cs) {
        // 0x8016813C: mtc1        $at, $f4
        ctx->f4.u32l = ctx->r1;
            goto L_80168164;
    }
    goto skip_3;
    // 0x8016813C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    skip_3:
    // 0x80168140: sh          $zero, 0x0($s1)
    MEM_H(0X0, ctx->r17) = 0;
    // 0x80168144: lwc1        $f12, 0x28($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80168148: lwc1        $f14, 0x38($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016814C: jal         0x801681C8
    // 0x80168150: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_801681C8(rdram, ctx);
        goto after_6;
    // 0x80168150: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_6:
    // 0x80168154: negu        $t7, $v0
    ctx->r15 = SUB32(0, ctx->r2);
    // 0x80168158: b           L_801681B4
    // 0x8016815C: sh          $t7, 0x4($s1)
    MEM_H(0X4, ctx->r17) = ctx->r15;
        goto L_801681B4;
    // 0x8016815C: sh          $t7, 0x4($s1)
    MEM_H(0X4, ctx->r17) = ctx->r15;
    // 0x80168160: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
L_80168164:
    // 0x80168164: lwc1        $f10, 0x48($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80168168: lwc1        $f6, 0x4C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016816C: div.s       $f2, $f4, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = DIV_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80168170: mul.s       $f12, $f10, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80168174: swc1        $f2, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f2.u32l;
    // 0x80168178: mul.s       $f14, $f6, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016817C: jal         0x801681C8
    // 0x80168180: nop

    func_801681C8(rdram, ctx);
        goto after_7;
    // 0x80168180: nop

    after_7:
    // 0x80168184: lwc1        $f2, 0x68($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80168188: negu        $t8, $v0
    ctx->r24 = SUB32(0, ctx->r2);
    // 0x8016818C: sh          $t8, 0x0($s1)
    MEM_H(0X0, ctx->r17) = ctx->r24;
    // 0x80168190: lwc1        $f18, 0x34($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80168194: lwc1        $f8, 0x24($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X24);
    // 0x80168198: mul.s       $f12, $f18, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f18.fl, ctx->f2.fl);
    // 0x8016819C: nop

    // 0x801681A0: mul.s       $f14, $f8, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x801681A4: jal         0x801681C8
    // 0x801681A8: nop

    func_801681C8(rdram, ctx);
        goto after_8;
    // 0x801681A8: nop

    after_8:
    // 0x801681AC: negu        $t9, $v0
    ctx->r25 = SUB32(0, ctx->r2);
    // 0x801681B0: sh          $t9, 0x4($s1)
    MEM_H(0X4, ctx->r17) = ctx->r25;
L_801681B4:
    // 0x801681B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801681B8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x801681BC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801681C0: jr          $ra
    // 0x801681C4: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x801681C4: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void func_801681C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801681C8: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x801681CC: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x801681D0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801681D4: c.lt.s      $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f14.fl < ctx->f16.fl;
    // 0x801681D8: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801681DC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801681E0: bc1fl       L_801681F4
    if (!c1cs) {
        // 0x801681E4: mov.s       $f0, $f14
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
            goto L_801681F4;
    }
    goto skip_0;
    // 0x801681E4: mov.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
    skip_0:
    // 0x801681E8: b           L_801681F4
    // 0x801681EC: neg.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = -ctx->f14.fl;
        goto L_801681F4;
    // 0x801681EC: neg.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = -ctx->f14.fl;
    // 0x801681F0: mov.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
L_801681F4:
    // 0x801681F4: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x801681F8: nop

    // 0x801681FC: bc1t        L_8016822C
    if (c1cs) {
        // 0x80168200: nop
    
            goto L_8016822C;
    }
    // 0x80168200: nop

    // 0x80168204: c.lt.s      $f12, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f12.fl < ctx->f16.fl;
    // 0x80168208: nop

    // 0x8016820C: bc1fl       L_80168220
    if (!c1cs) {
        // 0x80168210: mov.s       $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
            goto L_80168220;
    }
    goto skip_1;
    // 0x80168210: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    skip_1:
    // 0x80168214: b           L_80168220
    // 0x80168218: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
        goto L_80168220;
    // 0x80168218: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
    // 0x8016821C: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_80168220:
    // 0x80168220: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x80168224: nop

    // 0x80168228: bc1f        L_80168290
    if (!c1cs) {
        // 0x8016822C: lui         $at, 0x801A
        ctx->r1 = S32(0X801A << 16);
            goto L_80168290;
    }
L_8016822C:
    // 0x8016822C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80168230: lwc1        $f2, -0x51AC($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X51AC);
    // 0x80168234: mul.s       $f14, $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x80168238: c.lt.s      $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f14.fl < ctx->f16.fl;
    // 0x8016823C: mul.s       $f12, $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x80168240: bc1fl       L_80168254
    if (!c1cs) {
        // 0x80168244: mov.s       $f0, $f14
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
            goto L_80168254;
    }
    goto skip_2;
    // 0x80168244: mov.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
    skip_2:
    // 0x80168248: b           L_80168254
    // 0x8016824C: neg.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = -ctx->f14.fl;
        goto L_80168254;
    // 0x8016824C: neg.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = -ctx->f14.fl;
    // 0x80168250: mov.s       $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    ctx->f0.fl = ctx->f14.fl;
L_80168254:
    // 0x80168254: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x80168258: nop

    // 0x8016825C: bc1t        L_8016822C
    if (c1cs) {
        // 0x80168260: nop
    
            goto L_8016822C;
    }
    // 0x80168260: nop

    // 0x80168264: c.lt.s      $f12, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f12.fl < ctx->f16.fl;
    // 0x80168268: nop

    // 0x8016826C: bc1fl       L_80168280
    if (!c1cs) {
        // 0x80168270: mov.s       $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
            goto L_80168280;
    }
    goto skip_3;
    // 0x80168270: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    skip_3:
    // 0x80168274: b           L_80168280
    // 0x80168278: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
        goto L_80168280;
    // 0x80168278: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
    // 0x8016827C: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_80168280:
    // 0x80168280: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x80168284: nop

    // 0x80168288: bc1t        L_8016822C
    if (c1cs) {
        // 0x8016828C: nop
    
            goto L_8016822C;
    }
    // 0x8016828C: nop

L_80168290:
    // 0x80168290: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80168294: lwc1        $f0, -0x51A8($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X51A8);
    // 0x80168298: mul.s       $f4, $f12, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x8016829C: nop

    // 0x801682A0: mul.s       $f8, $f14, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x801682A4: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x801682A8: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x801682AC: mfc1        $a0, $f6
    ctx->r4 = (int32_t)ctx->f6.u32l;
    // 0x801682B0: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
    // 0x801682B4: jal         0x80004AB0
    // 0x801682B8: nop

    func_80004AB0(rdram, ctx);
        goto after_0;
    // 0x801682B8: nop

    after_0:
    // 0x801682BC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801682C0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801682C4: jr          $ra
    // 0x801682C8: nop

    return;
    // 0x801682C8: nop

;}
RECOMP_FUNC void func_801682D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figsrc_entry_probe((uint32_t)ctx->r4, (uint32_t)ctx->r5, (uint32_t)ctx->r6,
                                 (uint32_t)ctx->r7);
#endif
    // 0x801682D0: addiu       $sp, $sp, -0x90
    ctx->r29 = ADD32(ctx->r29, -0X90);
    // 0x801682D4: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x801682D8: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x801682DC: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x801682E0: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x801682E4: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x801682E8: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x801682EC: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x801682F0: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x801682F4: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x801682F8: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x801682FC: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80168300: lw          $s2, -0x1134($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X1134);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figsrc_entry_fields_probe(rdram, (uint32_t)ctx->r18);
#endif
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: workspace guard (LOD_FIX_TEXT_MEASURE_GUARD, round 15 Task A) ---
    // Moved up from the individual func_8016890C call sites: if $s2 (the
    // struct read from global 0x8019EECC above) is invalid, skip the ENTIRE
    // s2-dependent text-draw span below (the three fixed func_8016890C
    // calls, the func_80169884/func_80169934 calls, and the func_8016890C
    // array loop) in one shot by jumping straight to L_80168404, instead of
    // relying on each individual call's own inner guard to reject it one at
    // a time (round 14's guards 1/2 only stopped the crash; leaving the
    // per-call draw commands half-issued is what turned it into a freeze).
    // L_80168404 is a safe landing spot: it immediately overwrites $s2 with
    // an unrelated global's list head for the function's second, independent
    // half, so nothing here depends on $s2 having been valid. That second
    // half (and the sqrt-normalize tail after it) still runs normally,
    // which matters: this function's callers use its return value ($v0) --
    // one caller (funcs_21.c) dereferences it unconditionally with no null
    // check -- and that value is produced entirely by the tail, independent
    // of $s2, so preserving it (rather than bailing out via a synthetic
    // early return before it runs) is required for those callers to stay
    // safe. See docs/issue27-31-ni0e-findings.md Round 15 for the caller
    // audit that ruled out a full early-return here.
    if (lod_text_guard_workspace_invalid(rdram, (uint32_t)ctx->r18)) {
        if (!lod_text_guard_workspace_draw_logged) {
            lod_text_guard_workspace_draw_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_801682D0 rejected invalid workspace s2=0x%08X "
                    "(stale text-struct pointer), skipping text draw\n",
                    (uint32_t)ctx->r18);
        }
        goto L_80168404;
    }
    // --- END PATCH ---
#endif
    // 0x80168304: addiu       $a0, $sp, 0x7C
    ctx->r4 = ADD32(ctx->r29, 0X7C);
    // 0x80168308: addiu       $v1, $sp, 0x7C
    ctx->r3 = ADD32(ctx->r29, 0X7C);
    // 0x8016830C: addiu       $v0, $sp, 0x70
    ctx->r2 = ADD32(ctx->r29, 0X70);
    // 0x80168310: or          $t0, $a1, $zero
    ctx->r8 = ctx->r5 | 0;
    // 0x80168314: lwc1        $f18, 0x0($a3)
    ctx->f18.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80168318: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016831C: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x80168320: mul.s       $f20, $f18, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80168324: beql        $at, $zero, L_80168364
    if (ctx->r1 == 0) {
        // 0x80168328: swc1        $f20, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->f20.u32l;
            goto L_80168364;
    }
    goto skip_0;
    // 0x80168328: swc1        $f20, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f20.u32l;
    skip_0:
    // 0x8016832C: swc1        $f20, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f20.u32l;
L_80168330:
    // 0x80168330: lwc1        $f18, 0x0($t0)
    ctx->f18.u32l = MEM_W(ctx->r8, 0X0);
    // 0x80168334: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80168338: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016833C: mul.s       $f18, $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80168340: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80168344: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x80168348: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016834C: swc1        $f18, -0x8($v0)
    MEM_W(-0X8, ctx->r2) = ctx->f18.u32l;
    // 0x80168350: lwc1        $f18, 0x0($a3)
    ctx->f18.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80168354: mul.s       $f20, $f18, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80168358: bnel        $at, $zero, L_80168330
    if (ctx->r1 != 0) {
        // 0x8016835C: swc1        $f20, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->f20.u32l;
            goto L_80168330;
    }
    goto skip_1;
    // 0x8016835C: swc1        $f20, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f20.u32l;
    skip_1:
    // 0x80168360: swc1        $f20, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f20.u32l;
L_80168364:
    // 0x80168364: lwc1        $f18, 0x0($t0)
    ctx->f18.u32l = MEM_W(ctx->r8, 0X0);
    // 0x80168368: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016836C: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x80168370: mul.s       $f18, $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80168374: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x80168378: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016837C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80168380: addiu       $a0, $a0, -0x10F0
    ctx->r4 = ADD32(ctx->r4, -0X10F0);
    // 0x80168384: jal         0x80169884
    // 0x80168388: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    func_80169884(rdram, ctx);
        goto after_0;
    // 0x80168388: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    after_0:
    // 0x8016838C: addiu       $a0, $sp, 0x7C
    ctx->r4 = ADD32(ctx->r29, 0X7C);
    // 0x80168390: jal         0x80169934
    // 0x80168394: addiu       $a1, $sp, 0x70
    ctx->r5 = ADD32(ctx->r29, 0X70);
    func_80169934(rdram, ctx);
        goto after_1;
    // 0x80168394: addiu       $a1, $sp, 0x70
    ctx->r5 = ADD32(ctx->r29, 0X70);
    after_1:
    // 0x80168398: lw          $a0, 0x10($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X10);
    // 0x8016839C: jal         0x8016890C
    // 0x801683A0: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figsrc_check((uint32_t)ctx->r4, (uint32_t)ctx->r18, 0x10u, -1, "field10");
#endif
    func_8016890C(rdram, ctx);
        goto after_2;
    // 0x801683A0: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    after_2:
    // 0x801683A4: lw          $a0, 0x14($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X14);
    // 0x801683A8: jal         0x8016890C
    // 0x801683AC: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figsrc_check((uint32_t)ctx->r4, (uint32_t)ctx->r18, 0x14u, -1, "field14");
#endif
    func_8016890C(rdram, ctx);
        goto after_3;
    // 0x801683AC: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    after_3:
    // 0x801683B0: lw          $a0, 0x18($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X18);
    // 0x801683B4: jal         0x8016890C
    // 0x801683B8: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figsrc_check((uint32_t)ctx->r4, (uint32_t)ctx->r18, 0x18u, -1, "field18");
#endif
    func_8016890C(rdram, ctx);
        goto after_4;
    // 0x801683B8: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    after_4:
    // 0x801683BC: lw          $t6, 0x20($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X20);
    // 0x801683C0: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801683C4: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: array-loop guard (LOD_FIX_TEXT_MEASURE_GUARD, round 18 Task B) ---
    // See lod_text_guard_array_invalid above for the full repro18 crash
    // analysis. Only checked when the loop is about to run at all (count>0,
    // matching the blez below); $s1/$s0 are already 0 at this point (just
    // set above), so jumping straight to the existing L_80168404 skip target
    // is safe with no further register fixups needed.
    if (SIGNED(ctx->r14) > 0 && lod_text_guard_array_invalid(rdram, (uint32_t)ctx->r18)) {
        if (!lod_text_guard_array_draw_logged) {
            lod_text_guard_array_draw_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_801682D0 rejected invalid array bases "
                    "obj_base=0x%08X str_base=0x%08X count=%d (stale text-struct array), "
                    "skipping array loop\n",
                    (uint32_t)MEM_W(ctx->r18, 0X24), (uint32_t)MEM_W(ctx->r18, 0X38),
                    (int32_t)ctx->r14);
        }
        goto L_80168404;
    }
    // --- END PATCH ---
#endif
    // 0x801683C8: blez        $t6, L_80168404
    if (SIGNED(ctx->r14) <= 0) {
        // 0x801683CC: nop
    
            goto L_80168404;
    }
    // 0x801683CC: nop

    // 0x801683D0: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
L_801683D4:
    // 0x801683D4: lw          $t9, 0x38($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X38);
    // 0x801683D8: addu        $t8, $t7, $s0
    ctx->r24 = ADD32(ctx->r15, ctx->r16);
    // 0x801683DC: addu        $t1, $t9, $s0
    ctx->r9 = ADD32(ctx->r25, ctx->r16);
    // 0x801683E0: lw          $a1, 0x0($t1)
    ctx->r5 = MEM_W(ctx->r9, 0X0);
    // 0x801683E4: jal         0x8016890C
    // 0x801683E8: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figsrc_check((uint32_t)ctx->r4, (uint32_t)ctx->r15, (uint32_t)ctx->r16,
                           (int32_t)ctx->r17, "loop-array");
#endif
    func_8016890C(rdram, ctx);
        goto after_5;
    // 0x801683E8: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
    after_5:
    // 0x801683EC: lw          $t2, 0x20($s2)
    ctx->r10 = MEM_W(ctx->r18, 0X20);
    // 0x801683F0: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x801683F4: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x801683F8: slt         $at, $s1, $t2
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r10) ? 1 : 0;
    // 0x801683FC: bnel        $at, $zero, L_801683D4
    if (ctx->r1 != 0) {
        // 0x80168400: lw          $t7, 0x24($s2)
        ctx->r15 = MEM_W(ctx->r18, 0X24);
            goto L_801683D4;
    }
    goto skip_2;
    // 0x80168400: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
    skip_2:
L_80168404:
    // 0x80168404: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80168408: lw          $s2, -0x1A20($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X1A20);
    // 0x8016840C: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x80168410: addiu       $s4, $s4, -0x9FC
    ctx->r20 = ADD32(ctx->r20, -0X9FC);
    // 0x80168414: beq         $s2, $zero, L_801684BC
    if (ctx->r18 == 0) {
        // 0x80168418: or          $s3, $zero, $zero
        ctx->r19 = 0 | 0;
            goto L_801684BC;
    }
    // 0x80168418: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x8016841C: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
L_80168420:
    // 0x80168420: beql        $v0, $zero, L_801684B0
    if (ctx->r2 == 0) {
        // 0x80168424: lw          $s2, 0x0($s2)
        ctx->r18 = MEM_W(ctx->r18, 0X0);
            goto L_801684B0;
    }
    goto skip_3;
    // 0x80168424: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    skip_3:
    // 0x80168428: lw          $s0, 0x24($v0)
    ctx->r16 = MEM_W(ctx->r2, 0X24);
    // 0x8016842C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80168430: beq         $s0, $zero, L_801684AC
    if (ctx->r16 == 0) {
        // 0x80168434: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_801684AC;
    }
    // 0x80168434: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80168438: lw          $t3, 0x0($s4)
    ctx->r11 = MEM_W(ctx->r20, 0X0);
    // 0x8016843C: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
    // 0x80168440: sw          $s3, 0x10($t3)
    MEM_W(0X10, ctx->r11) = ctx->r19;
    // 0x80168444: lw          $t4, 0x0($s4)
    ctx->r12 = MEM_W(ctx->r20, 0X0);
    // 0x80168448: sw          $v0, 0xC($t4)
    MEM_W(0XC, ctx->r12) = ctx->r2;
    // 0x8016844C: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
    // 0x80168450: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x80168454: slti        $at, $v1, 0x282
    ctx->r1 = SIGNED(ctx->r3) < 0X282 ? 1 : 0;
    // 0x80168458: bne         $at, $zero, L_80168470
    if (ctx->r1 != 0) {
        // 0x8016845C: slti        $at, $v1, 0x285
        ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
            goto L_80168470;
    }
    // 0x8016845C: slti        $at, $v1, 0x285
    ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
    // 0x80168460: beq         $at, $zero, L_80168470
    if (ctx->r1 == 0) {
        // 0x80168464: nop
    
            goto L_80168470;
    }
    // 0x80168464: nop

    // 0x80168468: b           L_80168470
    // 0x8016846C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_80168470;
    // 0x8016846C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80168470:
    // 0x80168470: jal         0x80168698
    // 0x80168474: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_80168698(rdram, ctx);
        goto after_6;
    // 0x80168474: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_6:
    // 0x80168478: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016847C: beql        $a0, $zero, L_801684B0
    if (ctx->r4 == 0) {
        // 0x80168480: lw          $s2, 0x0($s2)
        ctx->r18 = MEM_W(ctx->r18, 0X0);
            goto L_801684B0;
    }
    goto skip_4;
    // 0x80168480: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    skip_4:
    // 0x80168484: lhu         $t5, 0x2($s0)
    ctx->r13 = MEM_HU(ctx->r16, 0X2);
    // 0x80168488: andi        $t6, $t5, 0x8000
    ctx->r14 = ctx->r13 & 0X8000;
    // 0x8016848C: beq         $t6, $zero, L_801684A4
    if (ctx->r14 == 0) {
        // 0x80168490: nop
    
            goto L_801684A4;
    }
    // 0x80168490: nop

    // 0x80168494: jal         0x801685A8
    // 0x80168498: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_801685A8(rdram, ctx);
        goto after_7;
    // 0x80168498: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_7:
    // 0x8016849C: b           L_801684B0
    // 0x801684A0: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
        goto L_801684B0;
    // 0x801684A0: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
L_801684A4:
    // 0x801684A4: jal         0x80168620
    // 0x801684A8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80168620(rdram, ctx);
        goto after_8;
    // 0x801684A8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_8:
L_801684AC:
    // 0x801684AC: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
L_801684B0:
    // 0x801684B0: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x801684B4: bnel        $s2, $zero, L_80168420
    if (ctx->r18 != 0) {
        // 0x801684B8: lw          $v0, 0x4($s2)
        ctx->r2 = MEM_W(ctx->r18, 0X4);
            goto L_80168420;
    }
    goto skip_5;
    // 0x801684B8: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
    skip_5:
L_801684BC:
    // 0x801684BC: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801684C0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801684C4: lwc1        $f20, -0x51A0($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0X51A0);
    // 0x801684C8: lw          $v1, -0x9FC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X9FC);
    // 0x801684CC: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801684D0: addiu       $s2, $zero, 0x258
    ctx->r18 = ADD32(0, 0X258);
    // 0x801684D4: addu        $v0, $v1, $s1
    ctx->r2 = ADD32(ctx->r3, ctx->r17);
L_801684D8:
    // 0x801684D8: addiu       $s0, $v0, 0x230
    ctx->r16 = ADD32(ctx->r2, 0X230);
    // 0x801684DC: lh          $t7, 0x60($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X60);
    // 0x801684E0: beql        $t7, $zero, L_8016852C
    if (ctx->r15 == 0) {
        // 0x801684E4: addiu       $s0, $v0, 0x48C
        ctx->r16 = ADD32(ctx->r2, 0X48C);
            goto L_8016852C;
    }
    goto skip_6;
    // 0x801684E4: addiu       $s0, $v0, 0x48C
    ctx->r16 = ADD32(ctx->r2, 0X48C);
    skip_6:
    // 0x801684E8: jal         0x800A01E0
    // 0x801684EC: lwc1        $f12, 0x1C($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X1C);
    sqrtf_recomp(rdram, ctx);
        goto after_9;
    // 0x801684EC: lwc1        $f12, 0x1C($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X1C);
    after_9:
    // 0x801684F0: mul.s       $f16, $f0, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x801684F4: lwc1        $f18, 0x0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X0);
    // 0x801684F8: lwc1        $f6, 0x4($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X4);
    // 0x801684FC: lwc1        $f10, 0x8($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80168500: mul.s       $f4, $f18, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f20.fl);
    // 0x80168504: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80168508: mul.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f20.fl);
    // 0x8016850C: swc1        $f16, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f16.u32l;
    // 0x80168510: mul.s       $f16, $f10, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f20.fl);
    // 0x80168514: swc1        $f4, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f4.u32l;
    // 0x80168518: swc1        $f8, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f8.u32l;
    // 0x8016851C: swc1        $f16, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f16.u32l;
    // 0x80168520: lw          $v1, -0x9FC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X9FC);
    // 0x80168524: addu        $v0, $v1, $s1
    ctx->r2 = ADD32(ctx->r3, ctx->r17);
    // 0x80168528: addiu       $s0, $v0, 0x48C
    ctx->r16 = ADD32(ctx->r2, 0X48C);
L_8016852C:
    // 0x8016852C: lh          $t8, 0x60($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X60);
    // 0x80168530: beql        $t8, $zero, L_80168578
    if (ctx->r24 == 0) {
        // 0x80168534: addiu       $s1, $s1, 0xC8
        ctx->r17 = ADD32(ctx->r17, 0XC8);
            goto L_80168578;
    }
    goto skip_7;
    // 0x80168534: addiu       $s1, $s1, 0xC8
    ctx->r17 = ADD32(ctx->r17, 0XC8);
    skip_7:
    // 0x80168538: jal         0x800A01E0
    // 0x8016853C: lwc1        $f12, 0x1C($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X1C);
    sqrtf_recomp(rdram, ctx);
        goto after_10;
    // 0x8016853C: lwc1        $f12, 0x1C($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X1C);
    after_10:
    // 0x80168540: mul.s       $f18, $f0, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f20.fl);
    // 0x80168544: lwc1        $f4, 0x0($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80168548: lwc1        $f8, 0x4($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X4);
    // 0x8016854C: lwc1        $f16, 0x8($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80168550: mul.s       $f6, $f4, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f20.fl);
    // 0x80168554: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80168558: mul.s       $f10, $f8, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f20.fl);
    // 0x8016855C: swc1        $f18, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f18.u32l;
    // 0x80168560: mul.s       $f18, $f16, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x80168564: swc1        $f6, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f6.u32l;
    // 0x80168568: swc1        $f10, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f10.u32l;
    // 0x8016856C: swc1        $f18, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f18.u32l;
    // 0x80168570: lw          $v1, -0x9FC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X9FC);
    // 0x80168574: addiu       $s1, $s1, 0xC8
    ctx->r17 = ADD32(ctx->r17, 0XC8);
L_80168578:
    // 0x80168578: bnel        $s1, $s2, L_801684D8
    if (ctx->r17 != ctx->r18) {
        // 0x8016857C: addu        $v0, $v1, $s1
        ctx->r2 = ADD32(ctx->r3, ctx->r17);
            goto L_801684D8;
    }
    goto skip_8;
    // 0x8016857C: addu        $v0, $v1, $s1
    ctx->r2 = ADD32(ctx->r3, ctx->r17);
    skip_8:
    // 0x80168580: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80168584: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80168588: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8016858C: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80168590: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x80168594: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x80168598: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x8016859C: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
    // 0x801685A0: jr          $ra
    // 0x801685A4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x801685A4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_801685A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_801685A8 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_801685A8++;
    if (lod_text_guard_depth_801685A8 > 128) {
        if (!lod_text_guard_logged_801685A8) {
            lod_text_guard_logged_801685A8 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_801685A8 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_801685A8--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x801685A8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801685AC: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801685B0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801685B4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801685B8: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x801685BC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801685C0: beq         $s0, $zero, L_8016860C
    if (ctx->r16 == 0) {
        // 0x801685C4: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016860C;
    }
L_801685C4:
    // 0x801685C4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801685C8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801685CC: jal         0x80168698
    // 0x801685D0: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_80168698(rdram, ctx);
        goto after_0;
    // 0x801685D0: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x801685D4: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x801685D8: beql        $a0, $zero, L_801685EC
    if (ctx->r4 == 0) {
        // 0x801685DC: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_801685EC;
    }
    goto skip_0;
    // 0x801685DC: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x801685E0: jal         0x801685A8
    // 0x801685E4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_801685A8(rdram, ctx);
        goto after_1;
    // 0x801685E4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x801685E8: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_801685EC:
    // 0x801685EC: beql        $v0, $zero, L_80168610
    if (ctx->r2 == 0) {
        // 0x801685F0: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80168610;
    }
    goto skip_1;
    // 0x801685F0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x801685F4: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x801685F8: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x801685FC: bnel        $t7, $zero, L_80168610
    if (ctx->r15 != 0) {
        // 0x80168600: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80168610;
    }
    goto skip_2;
    // 0x80168600: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x80168604: bne         $v0, $zero, L_801685C4
    if (ctx->r2 != 0) {
        // 0x80168608: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_801685C4;
    }
    // 0x80168608: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016860C:
    // 0x8016860C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80168610:
    // 0x80168610: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80168614: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80168618: jr          $ra
    // 0x8016861C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_801685A8--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016861C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80168620(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_80168620 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_80168620++;
    if (lod_text_guard_depth_80168620 > 128) {
        if (!lod_text_guard_logged_80168620) {
            lod_text_guard_logged_80168620 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_80168620 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_80168620--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x80168620: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80168624: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80168628: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016862C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80168630: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x80168634: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80168638: beq         $s0, $zero, L_80168684
    if (ctx->r16 == 0) {
        // 0x8016863C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80168684;
    }
L_8016863C:
    // 0x8016863C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80168640: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80168644: jal         0x80168698
    // 0x80168648: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_80168698(rdram, ctx);
        goto after_0;
    // 0x80168648: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x8016864C: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x80168650: beql        $a0, $zero, L_80168664
    if (ctx->r4 == 0) {
        // 0x80168654: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_80168664;
    }
    goto skip_0;
    // 0x80168654: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x80168658: jal         0x80168620
    // 0x8016865C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80168620(rdram, ctx);
        goto after_1;
    // 0x8016865C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x80168660: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_80168664:
    // 0x80168664: beql        $v0, $zero, L_80168688
    if (ctx->r2 == 0) {
        // 0x80168668: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80168688;
    }
    goto skip_1;
    // 0x80168668: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016866C: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x80168670: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x80168674: bnel        $t7, $zero, L_80168688
    if (ctx->r15 != 0) {
        // 0x80168678: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80168688;
    }
    goto skip_2;
    // 0x80168678: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016867C: bne         $v0, $zero, L_8016863C
    if (ctx->r2 != 0) {
        // 0x80168680: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016863C;
    }
    // 0x80168680: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80168684:
    // 0x80168684: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80168688:
    // 0x80168688: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016868C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80168690: jr          $ra
    // 0x80168694: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_80168620--;
    // --- END PATCH ---
#endif
    return;
    // 0x80168694: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80168698(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80168698: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x8016869C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801686A0: sw          $a0, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r4;
    // 0x801686A4: sw          $a2, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r6;
    // 0x801686A8: lw          $v0, 0x74($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X74);
    // 0x801686AC: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x801686B0: lw          $a3, -0x1134($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X1134);
    // 0x801686B4: beql        $v0, $zero, L_801688F8
    if (ctx->r2 == 0) {
        // 0x801686B8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801688F8;
    }
    goto skip_0;
    // 0x801686B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801686BC: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
    // 0x801686C0: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x801686C4: beql        $t9, $zero, L_801688F8
    if (ctx->r25 == 0) {
        // 0x801686C8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801688F8;
    }
    goto skip_1;
    // 0x801686C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x801686CC: lhu         $v1, 0x20($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X20);
    // 0x801686D0: beql        $v1, $zero, L_801688F8
    if (ctx->r3 == 0) {
        // 0x801686D4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801688F8;
    }
    goto skip_2;
    // 0x801686D4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x801686D8: lw          $t2, 0xC($a3)
    ctx->r10 = MEM_W(ctx->r7, 0XC);
    // 0x801686DC: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x801686E0: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x801686E4: addu        $t5, $t2, $t4
    ctx->r13 = ADD32(ctx->r10, ctx->r12);
    // 0x801686E8: lhu         $v0, -0x2($t5)
    ctx->r2 = MEM_HU(ctx->r13, -0X2);
    // 0x801686EC: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x801686F0: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x801686F4: beq         $v0, $at, L_801688F4
    if (ctx->r2 == ctx->r1) {
        // 0x801686F8: or          $t0, $v0, $zero
        ctx->r8 = ctx->r2 | 0;
            goto L_801688F4;
    }
    // 0x801686F8: or          $t0, $v0, $zero
    ctx->r8 = ctx->r2 | 0;
    // 0x801686FC: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x80168700: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x80168704: subu        $t8, $t8, $v0
    ctx->r24 = SUB32(ctx->r24, ctx->r2);
    // 0x80168708: lw          $t7, 0x4($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X4);
    // 0x8016870C: sll         $t8, $t8, 4
    ctx->r24 = S32(ctx->r24 << 4);
    // 0x80168710: addiu       $t3, $sp, 0x30
    ctx->r11 = ADD32(ctx->r29, 0X30);
    // 0x80168714: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x80168718: sw          $t9, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r25;
    // 0x8016871C: bne         $a1, $zero, L_80168758
    if (ctx->r5 != 0) {
        // 0x80168720: sw          $t3, 0x58($sp)
        MEM_W(0X58, ctx->r29) = ctx->r11;
            goto L_80168758;
    }
    // 0x80168720: sw          $t3, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r11;
    // 0x80168724: lwc1        $f4, 0x50($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X50);
    // 0x80168728: swc1        $f4, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->f4.u32l;
    // 0x8016872C: lwc1        $f6, 0x54($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X54);
    // 0x80168730: swc1        $f6, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->f6.u32l;
    // 0x80168734: lwc1        $f8, 0x58($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X58);
    // 0x80168738: swc1        $f8, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->f8.u32l;
    // 0x8016873C: lh          $t4, 0x5C($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X5C);
    // 0x80168740: sh          $t4, 0x36($sp)
    MEM_H(0X36, ctx->r29) = ctx->r12;
    // 0x80168744: lh          $t5, 0x5E($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X5E);
    // 0x80168748: sh          $t5, 0x38($sp)
    MEM_H(0X38, ctx->r29) = ctx->r13;
    // 0x8016874C: lh          $t6, 0x60($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X60);
    // 0x80168750: b           L_80168780
    // 0x80168754: sh          $t6, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = ctx->r14;
        goto L_80168780;
    // 0x80168754: sh          $t6, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = ctx->r14;
L_80168758:
    // 0x80168758: lw          $a0, 0x80($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X80);
    // 0x8016875C: addiu       $a1, $sp, 0x70
    ctx->r5 = ADD32(ctx->r29, 0X70);
    // 0x80168760: addiu       $a2, $sp, 0x36
    ctx->r6 = ADD32(ctx->r29, 0X36);
    // 0x80168764: addiu       $a3, $sp, 0x64
    ctx->r7 = ADD32(ctx->r29, 0X64);
    // 0x80168768: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    // 0x8016876C: jal         0x80167F54
    // 0x80168770: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    func_80167F54(rdram, ctx);
        goto after_0;
    // 0x80168770: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    after_0:
    // 0x80168774: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80168778: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016877C: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
L_80168780:
    // 0x80168780: lw          $t7, 0x88($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X88);
    // 0x80168784: addiu       $v1, $sp, 0x70
    ctx->r3 = ADD32(ctx->r29, 0X70);
    // 0x80168788: addiu       $v0, $sp, 0x30
    ctx->r2 = ADD32(ctx->r29, 0X30);
    // 0x8016878C: bne         $t7, $zero, L_8016879C
    if (ctx->r15 != 0) {
        // 0x80168790: lui         $at, 0x4120
        ctx->r1 = S32(0X4120 << 16);
            goto L_8016879C;
    }
    // 0x80168790: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x80168794: sh          $zero, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = 0;
    // 0x80168798: sh          $zero, 0x36($sp)
    MEM_H(0X36, ctx->r29) = 0;
L_8016879C:
    // 0x8016879C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x801687A0: addiu       $a0, $sp, 0x36
    ctx->r4 = ADD32(ctx->r29, 0X36);
    // 0x801687A4: lwc1        $f14, 0x0($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X0);
    // 0x801687A8: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x801687AC: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x801687B0: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x801687B4: beql        $at, $zero, L_801687E4
    if (ctx->r1 == 0) {
        // 0x801687B8: trunc.w.s   $f16, $f18
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
            goto L_801687E4;
    }
    goto skip_3;
    // 0x801687B8: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    skip_3:
L_801687BC:
    // 0x801687BC: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x801687C0: lwc1        $f14, 0x4($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X4);
    // 0x801687C4: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x801687C8: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x801687CC: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x801687D0: mfc1        $t9, $f16
    ctx->r25 = (int32_t)ctx->f16.u32l;
    // 0x801687D4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801687D8: bne         $at, $zero, L_801687BC
    if (ctx->r1 != 0) {
        // 0x801687DC: sh          $t9, -0x4($v0)
        MEM_H(-0X4, ctx->r2) = ctx->r25;
            goto L_801687BC;
    }
    // 0x801687DC: sh          $t9, -0x4($v0)
    MEM_H(-0X4, ctx->r2) = ctx->r25;
    // 0x801687E0: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
L_801687E4:
    // 0x801687E4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801687E8: mfc1        $t9, $f16
    ctx->r25 = (int32_t)ctx->f16.u32l;
    // 0x801687EC: nop

    // 0x801687F0: sh          $t9, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r25;
    // 0x801687F4: lh          $t4, 0x36($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X36);
    // 0x801687F8: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x801687FC: sh          $t0, 0x54($sp)
    MEM_H(0X54, ctx->r29) = ctx->r8;
    // 0x80168800: bne         $t4, $zero, L_80168858
    if (ctx->r12 != 0) {
        // 0x80168804: sh          $t3, 0x56($sp)
        MEM_H(0X56, ctx->r29) = ctx->r11;
            goto L_80168858;
    }
    // 0x80168804: sh          $t3, 0x56($sp)
    MEM_H(0X56, ctx->r29) = ctx->r11;
    // 0x80168808: lh          $t5, 0x3A($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X3A);
    // 0x8016880C: bne         $t5, $zero, L_80168858
    if (ctx->r13 != 0) {
        // 0x80168810: nop
    
            goto L_80168858;
    }
    // 0x80168810: nop

    // 0x80168814: lw          $t2, 0x0($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X0);
    // 0x80168818: addiu       $a0, $sp, 0x30
    ctx->r4 = ADD32(ctx->r29, 0X30);
    // 0x8016881C: jal         0x801699EC
    // 0x80168820: lw          $a1, 0xC($t2)
    ctx->r5 = MEM_W(ctx->r10, 0XC);
    func_801699EC(rdram, ctx);
        goto after_1;
    // 0x80168820: lw          $a1, 0xC($t2)
    ctx->r5 = MEM_W(ctx->r10, 0XC);
    after_1:
    // 0x80168824: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80168828: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016882C: lw          $a0, 0x5C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X5C);
    // 0x80168830: addiu       $a1, $v0, 0x38
    ctx->r5 = ADD32(ctx->r2, 0X38);
    // 0x80168834: addiu       $a2, $v0, 0x44
    ctx->r6 = ADD32(ctx->r2, 0X44);
    // 0x80168838: jal         0x80169BD8
    // 0x8016883C: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    func_80169BD8(rdram, ctx);
        goto after_2;
    // 0x8016883C: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    after_2:
    // 0x80168840: beq         $v0, $zero, L_801688F4
    if (ctx->r2 == 0) {
        // 0x80168844: addiu       $a0, $sp, 0x30
        ctx->r4 = ADD32(ctx->r29, 0X30);
            goto L_801688F4;
    }
    // 0x80168844: addiu       $a0, $sp, 0x30
    ctx->r4 = ADD32(ctx->r29, 0X30);
    // 0x80168848: jal         0x80168DD0
    // 0x8016884C: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    func_80168DD0(rdram, ctx);
        goto after_3;
    // 0x8016884C: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    after_3:
    // 0x80168850: b           L_801688F8
    // 0x80168854: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801688F8;
    // 0x80168854: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80168858:
    // 0x80168858: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016885C: lw          $a1, -0x1130($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1130);
    // 0x80168860: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80168864: blez        $a1, L_801688B0
    if (SIGNED(ctx->r5) <= 0) {
        // 0x80168868: nop
    
            goto L_801688B0;
    }
    // 0x80168868: nop

    // 0x8016886C: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x80168870: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80168874: lw          $v0, -0x1128($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X1128);
    // 0x80168878: lw          $v1, 0x10($t6)
    ctx->r3 = MEM_W(ctx->r14, 0X10);
L_8016887C:
    // 0x8016887C: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x80168880: bnel        $v1, $t7, L_801688A4
    if (ctx->r3 != ctx->r15) {
        // 0x80168884: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_801688A4;
    }
    goto skip_4;
    // 0x80168884: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    skip_4:
    // 0x80168888: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    // 0x8016888C: addiu       $t8, $v1, 0x6C
    ctx->r24 = ADD32(ctx->r3, 0X6C);
    // 0x80168890: addiu       $t9, $v1, 0x9C
    ctx->r25 = ADD32(ctx->r3, 0X9C);
    // 0x80168894: sw          $t8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r24;
    // 0x80168898: b           L_801688B0
    // 0x8016889C: sw          $t9, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r25;
        goto L_801688B0;
    // 0x8016889C: sw          $t9, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r25;
    // 0x801688A0: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_801688A4:
    // 0x801688A4: slt         $at, $a0, $a1
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x801688A8: bne         $at, $zero, L_8016887C
    if (ctx->r1 != 0) {
        // 0x801688AC: addiu       $v0, $v0, 0x8
        ctx->r2 = ADD32(ctx->r2, 0X8);
            goto L_8016887C;
    }
    // 0x801688AC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
L_801688B0:
    // 0x801688B0: beql        $a0, $a1, L_801688F8
    if (ctx->r4 == ctx->r5) {
        // 0x801688B4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801688F8;
    }
    goto skip_5;
    // 0x801688B4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_5:
    // 0x801688B8: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x801688BC: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x801688C0: jal         0x801699EC
    // 0x801688C4: lw          $a1, 0xC($t3)
    ctx->r5 = MEM_W(ctx->r11, 0XC);
    func_801699EC(rdram, ctx);
        goto after_4;
    // 0x801688C4: lw          $a1, 0xC($t3)
    ctx->r5 = MEM_W(ctx->r11, 0XC);
    after_4:
    // 0x801688C8: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801688CC: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x801688D0: lw          $a0, 0x5C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X5C);
    // 0x801688D4: addiu       $a1, $v0, 0x38
    ctx->r5 = ADD32(ctx->r2, 0X38);
    // 0x801688D8: addiu       $a2, $v0, 0x44
    ctx->r6 = ADD32(ctx->r2, 0X44);
    // 0x801688DC: jal         0x80169BD8
    // 0x801688E0: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    func_80169BD8(rdram, ctx);
        goto after_5;
    // 0x801688E0: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    after_5:
    // 0x801688E4: beq         $v0, $zero, L_801688F4
    if (ctx->r2 == 0) {
        // 0x801688E8: lw          $a0, 0x58($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X58);
            goto L_801688F4;
    }
    // 0x801688E8: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x801688EC: jal         0x80168F38
    // 0x801688F0: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    func_80168F38(rdram, ctx);
        goto after_6;
    // 0x801688F0: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    after_6:
L_801688F4:
    // 0x801688F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801688F8:
    // 0x801688F8: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    // 0x801688FC: jr          $ra
    // 0x80168900: nop

    return;
    // 0x80168900: nop

;}
RECOMP_FUNC void func_80168904(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80168904: jr          $ra
    // 0x80168908: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x80168908: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8016890C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figwalk_enter(rdram, (uint32_t)ctx->r4);
#endif
    // 0x8016890C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80168910: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x80168914: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80168918: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016891C: or          $s5, $a0, $zero
    ctx->r21 = ctx->r4 | 0;
    // 0x80168920: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80168924: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x80168928: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x8016892C: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x80168930: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80168934: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: pointer-range guard (LOD_FIX_TEXT_MEASURE_GUARD) ---
    // The original code only null-checks a0 (the `beq $a0,$zero` just below).
    // On a stale text-struct (see the comment above the flag's helpers),
    // func_801682D0 can hand this function a nonzero-but-garbage a0 (e.g. a
    // freed-buffer leftover or a float bit pattern, per the round 2/3
    // NI0E_TRACE figwalk/figsrc findings). Extend the null check to the same
    // "standard range check" (nonzero + within the emulated RDRAM span) used
    // elsewhere in this file, and take the exact same early-return path the
    // game's own null check uses when it fails. No effect when a0 is 0 or a
    // genuinely valid pointer -- the original `beq` below still runs as-is.
    if (ctx->r4 != 0 && !lod_text_guard_addr_ok((uint32_t)ctx->r4)) {
        if (!lod_text_guard_field_logged) {
            lod_text_guard_field_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016890C rejected out-of-range obj=0x%08X "
                    "(stale text-struct pointer)\n",
                    (uint32_t)ctx->r4);
        }
        MEM_W(0X14, ctx->r29) = ctx->r16;
        goto L_80168A74;
    }
    // --- END PATCH ---
#endif
    // 0x80168938: beq         $a0, $zero, L_80168A74
    if (ctx->r4 == 0) {
        // 0x8016893C: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_80168A74;
    }
    // 0x8016893C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80168940: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x80168944: addiu       $s7, $s7, -0x9FC
    ctx->r23 = ADD32(ctx->r23, -0X9FC);
    // 0x80168948: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x8016894C: addiu       $s0, $a0, 0x8
    ctx->r16 = ADD32(ctx->r4, 0X8);
    // 0x80168950: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80168954: addiu       $a1, $v0, 0x14
    ctx->r5 = ADD32(ctx->r2, 0X14);
    // 0x80168958: jal         0x80169BD8
    // 0x8016895C: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    func_80169BD8(rdram, ctx);
        goto after_0;
    // 0x8016895C: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    after_0:
    // 0x80168960: beq         $v0, $zero, L_80168A74
    if (ctx->r2 == 0) {
        // 0x80168964: lui         $a0, 0x801A
        ctx->r4 = S32(0X801A << 16);
            goto L_80168A74;
    }
    // 0x80168964: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80168968: addiu       $a0, $a0, -0x1110
    ctx->r4 = ADD32(ctx->r4, -0X1110);
    // 0x8016896C: jal         0x80169B88
    // 0x80168970: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80169B88(rdram, ctx);
        goto after_1;
    // 0x80168970: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x80168974: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x80168978: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x8016897C: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x80168980: addiu       $s6, $s6, 0x2490
    ctx->r22 = ADD32(ctx->r22, 0X2490);
    // 0x80168984: addiu       $s0, $s0, -0x111C
    ctx->r16 = ADD32(ctx->r16, -0X111C);
    // 0x80168988: addiu       $t6, $t6, -0x1680
    ctx->r14 = ADD32(ctx->r14, -0X1680);
    // 0x8016898C: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x80168990: sw          $t6, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r14;
    // 0x80168994: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80168998: addiu       $a0, $a0, -0x9F8
    ctx->r4 = ADD32(ctx->r4, -0X9F8);
    // 0x8016899C: jal         0x80000F30
    // 0x801689A0: lw          $a1, 0x0($s5)
    ctx->r5 = MEM_W(ctx->r21, 0X0);
    bzero_recomp(rdram, ctx);
        goto after_2;
    // 0x801689A0: lw          $a1, 0x0($s5)
    ctx->r5 = MEM_W(ctx->r21, 0X0);
    after_2:
    // 0x801689A4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801689A8: sw          $s1, -0x10F8($at)
    MEM_W(-0X10F8, ctx->r1) = ctx->r17;
    // 0x801689AC: lw          $a1, 0x0($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X0);
    // 0x801689B0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801689B4: jal         0x80168AA0
    // 0x801689B8: addiu       $a1, $a1, 0x14
    ctx->r5 = ADD32(ctx->r5, 0X14);
    func_80168AA0(rdram, ctx);
        goto after_3;
    // 0x801689B8: addiu       $a1, $a1, 0x14
    ctx->r5 = ADD32(ctx->r5, 0X14);
    after_3:
    // 0x801689BC: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x801689C0: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801689C4: addiu       $s4, $zero, 0x30
    ctx->r20 = ADD32(0, 0X30);
    // 0x801689C8: blez        $v1, L_80168A74
    if (SIGNED(ctx->r3) <= 0) {
        // 0x801689CC: addiu       $s3, $zero, 0x28
        ctx->r19 = ADD32(0, 0X28);
            goto L_80168A74;
    }
    // 0x801689CC: addiu       $s3, $zero, 0x28
    ctx->r19 = ADD32(0, 0X28);
    // 0x801689D0: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
L_801689D4:
    // 0x801689D4: lw          $t1, 0x4($s5)
    ctx->r9 = MEM_W(ctx->r21, 0X4);
    // 0x801689D8: addu        $t8, $t7, $s1
    ctx->r24 = ADD32(ctx->r15, ctx->r17);
    // 0x801689DC: lh          $t9, 0x0($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X0);
    // 0x801689E0: multu       $t9, $s3
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r19)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801689E4: mflo        $t0
    ctx->r8 = lo;
    // 0x801689E8: addu        $s0, $t0, $t1
    ctx->r16 = ADD32(ctx->r8, ctx->r9);
    // 0x801689EC: lh          $t2, 0x26($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X26);
    // 0x801689F0: beql        $t2, $zero, L_80168A04
    if (ctx->r10 == 0) {
        // 0x801689F4: lh          $t4, 0x24($s0)
        ctx->r12 = MEM_H(ctx->r16, 0X24);
            goto L_80168A04;
    }
    goto skip_0;
    // 0x801689F4: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
    skip_0:
    // 0x801689F8: b           L_80168A64
    // 0x801689FC: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80168A64;
    // 0x801689FC: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x80168A00: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
L_80168A04:
    // 0x80168A04: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x80168A08: addiu       $a0, $s0, 0xC
    ctx->r4 = ADD32(ctx->r16, 0XC);
    // 0x80168A0C: multu       $t4, $s4
    result = U64(U32(ctx->r12)) * U64(U32(ctx->r20)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80168A10: lw          $t3, 0x4($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X4);
    // 0x80168A14: addiu       $a1, $v0, 0x14
    ctx->r5 = ADD32(ctx->r2, 0X14);
    // 0x80168A18: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    // 0x80168A1C: mflo        $t5
    ctx->r13 = lo;
    // 0x80168A20: addu        $s2, $t3, $t5
    ctx->r18 = ADD32(ctx->r11, ctx->r13);
    // 0x80168A24: jal         0x80169BD8
    // 0x80168A28: nop

    func_80169BD8(rdram, ctx);
        goto after_4;
    // 0x80168A28: nop

    after_4:
    // 0x80168A2C: bne         $v0, $zero, L_80168A44
    if (ctx->r2 != 0) {
        // 0x80168A30: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80168A44;
    }
    // 0x80168A30: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80168A34: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80168A38: lw          $v1, -0x111C($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X111C);
    // 0x80168A3C: b           L_80168A64
    // 0x80168A40: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80168A64;
    // 0x80168A40: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80168A44:
    // 0x80168A44: jal         0x801699EC
    // 0x80168A48: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_801699EC(rdram, ctx);
        goto after_5;
    // 0x80168A48: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_5:
    // 0x80168A4C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80168A50: jal         0x80168DD0
    // 0x80168A54: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_80168DD0(rdram, ctx);
        goto after_6;
    // 0x80168A54: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_6:
    // 0x80168A58: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80168A5C: lw          $v1, -0x111C($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X111C);
    // 0x80168A60: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80168A64:
    // 0x80168A64: addiu       $s1, $s1, 0x2
    ctx->r17 = ADD32(ctx->r17, 0X2);
    // 0x80168A68: slt         $at, $s1, $v0
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80168A6C: bnel        $at, $zero, L_801689D4
    if (ctx->r1 != 0) {
        // 0x80168A70: lw          $t7, 0x0($s6)
        ctx->r15 = MEM_W(ctx->r22, 0X0);
            goto L_801689D4;
    }
    goto skip_1;
    // 0x80168A70: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
    skip_1:
L_80168A74:
    // 0x80168A74: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80168A78: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80168A7C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80168A80: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80168A84: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80168A88: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x80168A8C: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x80168A90: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x80168A94: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x80168A98: jr          $ra
    // 0x80168A9C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_figwalk_leave();
#endif
    return;
    // 0x80168A9C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_80168AA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 15) ---
    // func_80168AA0 is the draw-side sibling of func_8016B878 (see that
    // function's own copy of this guard, funcs_57.c, for the full
    // description): self-recursive (two `jal 0x80168AA0` sites in its own
    // body), walks the same RDRAM-global-0x8019EF08 table, and is reached
    // from func_8016890C the same way func_8016B878 is reached from
    // func_8016B62C. Confirmed single natural return point (one `return;`
    // in the whole function body, at its shared epilogue) by full-body grep,
    // same convention as func_8016B878. Round 14 only guarded func_8016B878;
    // this sibling was left unguarded, which is the leading explanation for
    // round 14's guarded-crash-became-a-freeze result -- the workspace
    // guard in func_801682D0/func_8016AE10 above should prevent this from
    // being reached at all on the known stale-pointer path, but this stays
    // as its own independent backstop in case some other caller reaches
    // func_8016890C -> func_80168AA0 with corrupted table data that the
    // workspace guard's own s2/+0x10 check does not happen to catch.
    lod_text_guard_draw_depth++;
    if (lod_text_guard_draw_depth > 128) {
        if (!lod_text_guard_draw_logged) {
            lod_text_guard_draw_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_80168AA0 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_draw_depth--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x80168AA0: addiu       $sp, $sp, -0x78
    ctx->r29 = ADD32(ctx->r29, -0X78);
    // 0x80168AA4: sw          $a0, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r4;
    // 0x80168AA8: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x80168AAC: lw          $t7, -0x10F8($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X10F8);
    // 0x80168AB0: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80168AB4: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80168AB8: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x80168ABC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80168AC0: sw          $a1, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r5;
    // 0x80168AC4: addu        $t2, $t6, $t7
    ctx->r10 = ADD32(ctx->r14, ctx->r15);
    // 0x80168AC8: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    // 0x80168ACC: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80168AD0: lui         $t4, 0x801A
    ctx->r12 = S32(0X801A << 16);
    // 0x80168AD4: bne         $v1, $at, L_80168B60
    if (ctx->r3 != ctx->r1) {
        // 0x80168AD8: nop
    
            goto L_80168B60;
    }
    // 0x80168AD8: nop

    // 0x80168ADC: lh          $a2, 0x2($t2)
    ctx->r6 = MEM_H(ctx->r10, 0X2);
    // 0x80168AE0: lw          $v0, 0x4($t2)
    ctx->r2 = MEM_W(ctx->r10, 0X4);
    // 0x80168AE4: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80168AE8: blez        $a2, L_80168DC0
    if (SIGNED(ctx->r6) <= 0) {
        // 0x80168AEC: lui         $t1, 0x8019
        ctx->r9 = S32(0X8019 << 16);
            goto L_80168DC0;
    }
    // 0x80168AEC: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80168AF0: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x80168AF4: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80168AF8: addiu       $a1, $a1, -0x111C
    ctx->r5 = ADD32(ctx->r5, -0X111C);
    // 0x80168AFC: addiu       $a3, $a3, -0x9F8
    ctx->r7 = ADD32(ctx->r7, -0X9F8);
    // 0x80168B00: addiu       $t1, $t1, 0x2490
    ctx->r9 = ADD32(ctx->r9, 0X2490);
    // 0x80168B04: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
L_80168B08:
    // 0x80168B08: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x80168B0C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80168B10: addu        $a0, $a3, $t8
    ctx->r4 = ADD32(ctx->r7, ctx->r24);
    // 0x80168B14: lbu         $t9, 0x0($a0)
    ctx->r25 = MEM_BU(ctx->r4, 0X0);
    // 0x80168B18: bnel        $t9, $zero, L_80168B50
    if (ctx->r25 != 0) {
        // 0x80168B1C: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_80168B50;
    }
    goto skip_0;
    // 0x80168B1C: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_0:
    // 0x80168B20: lw          $t8, 0x0($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X0);
    // 0x80168B24: lw          $t7, 0x0($t1)
    ctx->r15 = MEM_W(ctx->r9, 0X0);
    // 0x80168B28: sb          $t0, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r8;
    // 0x80168B2C: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x80168B30: sll         $t9, $t8, 1
    ctx->r25 = S32(ctx->r24 << 1);
    // 0x80168B34: addu        $t8, $t7, $t9
    ctx->r24 = ADD32(ctx->r15, ctx->r25);
    // 0x80168B38: sh          $t6, 0x0($t8)
    MEM_H(0X0, ctx->r24) = ctx->r14;
    // 0x80168B3C: lw          $t7, 0x0($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X0);
    // 0x80168B40: addiu       $t9, $t7, 0x1
    ctx->r25 = ADD32(ctx->r15, 0X1);
    // 0x80168B44: sw          $t9, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r25;
    // 0x80168B48: lh          $a2, 0x2($t2)
    ctx->r6 = MEM_H(ctx->r10, 0X2);
    // 0x80168B4C: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
L_80168B50:
    // 0x80168B50: bne         $at, $zero, L_80168B08
    if (ctx->r1 != 0) {
        // 0x80168B54: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
            goto L_80168B08;
    }
    // 0x80168B54: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80168B58: b           L_80168DC4
    // 0x80168B5C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80168DC4;
    // 0x80168B5C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80168B60:
    // 0x80168B60: lw          $t4, -0x9FC($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X9FC);
    // 0x80168B64: sll         $t5, $v1, 2
    ctx->r13 = S32(ctx->r3 << 2);
    // 0x80168B68: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80168B6C: addu        $v0, $t4, $t5
    ctx->r2 = ADD32(ctx->r12, ctx->r13);
    // 0x80168B70: lwc1        $f0, 0x20($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X20);
    // 0x80168B74: lh          $t6, 0x2($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X2);
    // 0x80168B78: addiu       $ra, $sp, 0x40
    ctx->r31 = ADD32(ctx->r29, 0X40);
    // 0x80168B7C: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x80168B80: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x80168B84: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80168B88: addu        $v1, $ra, $t5
    ctx->r3 = ADD32(ctx->r31, ctx->r13);
    // 0x80168B8C: bc1f        L_80168B9C
    if (!c1cs) {
        // 0x80168B90: cvt.s.w     $f12, $f4
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    ctx->f12.fl = CVT_S_W(ctx->f4.u32l);
            goto L_80168B9C;
    }
    // 0x80168B90: cvt.s.w     $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    ctx->f12.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80168B94: b           L_80168BA0
    // 0x80168B98: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_80168BA0;
    // 0x80168B98: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_80168B9C:
    // 0x80168B9C: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_80168BA0:
    // 0x80168BA0: lwc1        $f8, -0x519C($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X519C);
    // 0x80168BA4: c.lt.s      $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f2.fl < ctx->f8.fl;
    // 0x80168BA8: nop

    // 0x80168BAC: bc1fl       L_80168BC4
    if (!c1cs) {
        // 0x80168BB0: swc1        $f12, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->f12.u32l;
            goto L_80168BC4;
    }
    goto skip_1;
    // 0x80168BB0: swc1        $f12, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f12.u32l;
    skip_1:
    // 0x80168BB4: addiu       $v1, $zero, 0x3
    ctx->r3 = ADD32(0, 0X3);
    // 0x80168BB8: b           L_80168CEC
    // 0x80168BBC: addiu       $ra, $sp, 0x40
    ctx->r31 = ADD32(ctx->r29, 0X40);
        goto L_80168CEC;
    // 0x80168BBC: addiu       $ra, $sp, 0x40
    ctx->r31 = ADD32(ctx->r29, 0X40);
    // 0x80168BC0: swc1        $f12, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f12.u32l;
L_80168BC4:
    // 0x80168BC4: lwc1        $f10, 0x14($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80168BC8: lwc1        $f18, 0x20($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X20);
    // 0x80168BCC: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80168BD0: sub.s       $f16, $f12, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f12.fl - ctx->f10.fl;
    // 0x80168BD4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80168BD8: div.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = DIV_S(ctx->f16.fl, ctx->f18.fl);
    // 0x80168BDC: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x80168BE0: nop

    // 0x80168BE4: bc1t        L_80168C04
    if (c1cs) {
        // 0x80168BE8: nop
    
            goto L_80168C04;
    }
    // 0x80168BE8: nop

    // 0x80168BEC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80168BF0: addiu       $t8, $sp, 0x40
    ctx->r24 = ADD32(ctx->r29, 0X40);
    // 0x80168BF4: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x80168BF8: nop

    // 0x80168BFC: bc1f        L_80168C0C
    if (!c1cs) {
        // 0x80168C00: nop
    
            goto L_80168C0C;
    }
    // 0x80168C00: nop

L_80168C04:
    // 0x80168C04: b           L_80168CEC
    // 0x80168C08: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
        goto L_80168CEC;
    // 0x80168C08: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
L_80168C0C:
    // 0x80168C0C: bne         $v1, $t8, L_80168C20
    if (ctx->r3 != ctx->r24) {
        // 0x80168C10: addiu       $t7, $sp, 0x44
        ctx->r15 = ADD32(ctx->r29, 0X44);
            goto L_80168C20;
    }
    // 0x80168C10: addiu       $t7, $sp, 0x44
    ctx->r15 = ADD32(ctx->r29, 0X44);
    // 0x80168C14: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x80168C18: b           L_80168C38
    // 0x80168C1C: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
        goto L_80168C38;
    // 0x80168C1C: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
L_80168C20:
    // 0x80168C20: bne         $v1, $t7, L_80168C34
    if (ctx->r3 != ctx->r15) {
        // 0x80168C24: or          $a2, $zero, $zero
        ctx->r6 = 0 | 0;
            goto L_80168C34;
    }
    // 0x80168C24: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80168C28: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80168C2C: b           L_80168C38
    // 0x80168C30: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
        goto L_80168C38;
    // 0x80168C30: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
L_80168C34:
    // 0x80168C34: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
L_80168C38:
    // 0x80168C38: sll         $v0, $a2, 2
    ctx->r2 = S32(ctx->r6 << 2);
    // 0x80168C3C: addu        $a0, $t4, $v0
    ctx->r4 = ADD32(ctx->r12, ctx->r2);
    // 0x80168C40: lwc1        $f8, 0x20($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X20);
    // 0x80168C44: lwc1        $f16, 0x14($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X14);
    // 0x80168C48: addu        $v1, $ra, $v0
    ctx->r3 = ADD32(ctx->r31, ctx->r2);
    // 0x80168C4C: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x80168C50: sll         $t0, $a3, 2
    ctx->r8 = S32(ctx->r7 << 2);
    // 0x80168C54: addu        $a1, $t4, $t0
    ctx->r5 = ADD32(ctx->r12, ctx->r8);
    // 0x80168C58: lui         $t9, 0x801A
    ctx->r25 = S32(0X801A << 16);
    // 0x80168C5C: addu        $t1, $ra, $t0
    ctx->r9 = ADD32(ctx->r31, ctx->r8);
    // 0x80168C60: addiu       $t9, $t9, -0x1110
    ctx->r25 = ADD32(ctx->r25, -0X1110);
    // 0x80168C64: addu        $t3, $v0, $t9
    ctx->r11 = ADD32(ctx->r2, ctx->r25);
    // 0x80168C68: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x80168C6C: lwc1        $f16, 0x0($t3)
    ctx->f16.u32l = MEM_W(ctx->r11, 0X0);
    // 0x80168C70: swc1        $f18, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
    // 0x80168C74: lwc1        $f4, 0x20($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X20);
    // 0x80168C78: lwc1        $f8, 0x14($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X14);
    // 0x80168C7C: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80168C80: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80168C84: swc1        $f10, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->f10.u32l;
    // 0x80168C88: lwc1        $f2, 0x0($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X0);
    // 0x80168C8C: c.lt.s      $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f2.fl < ctx->f16.fl;
    // 0x80168C90: nop

    // 0x80168C94: bc1t        L_80168CE4
    if (c1cs) {
        // 0x80168C98: nop
    
            goto L_80168CE4;
    }
    // 0x80168C98: nop

    // 0x80168C9C: lwc1        $f18, 0xC($t3)
    ctx->f18.u32l = MEM_W(ctx->r11, 0XC);
    // 0x80168CA0: addu        $v0, $t0, $t9
    ctx->r2 = ADD32(ctx->r8, ctx->r25);
    // 0x80168CA4: c.lt.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl < ctx->f2.fl;
    // 0x80168CA8: nop

    // 0x80168CAC: bc1t        L_80168CE4
    if (c1cs) {
        // 0x80168CB0: nop
    
            goto L_80168CE4;
    }
    // 0x80168CB0: nop

    // 0x80168CB4: lwc1        $f0, 0x0($t1)
    ctx->f0.u32l = MEM_W(ctx->r9, 0X0);
    // 0x80168CB8: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80168CBC: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x80168CC0: nop

    // 0x80168CC4: bc1t        L_80168CE4
    if (c1cs) {
        // 0x80168CC8: nop
    
            goto L_80168CE4;
    }
    // 0x80168CC8: nop

    // 0x80168CCC: lwc1        $f6, 0xC($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80168CD0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80168CD4: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x80168CD8: nop

    // 0x80168CDC: bc1f        L_80168CEC
    if (!c1cs) {
        // 0x80168CE0: nop
    
            goto L_80168CEC;
    }
    // 0x80168CE0: nop

L_80168CE4:
    // 0x80168CE4: b           L_80168CEC
    // 0x80168CE8: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_80168CEC;
    // 0x80168CE8: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_80168CEC:
    // 0x80168CEC: bnel        $v1, $zero, L_80168D00
    if (ctx->r3 != 0) {
        // 0x80168CF0: lw          $a1, 0x7C($sp)
        ctx->r5 = MEM_W(ctx->r29, 0X7C);
            goto L_80168D00;
    }
    goto skip_2;
    // 0x80168CF0: lw          $a1, 0x7C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X7C);
    skip_2:
    // 0x80168CF4: b           L_80168D00
    // 0x80168CF8: or          $a1, $ra, $zero
    ctx->r5 = ctx->r31 | 0;
        goto L_80168D00;
    // 0x80168CF8: or          $a1, $ra, $zero
    ctx->r5 = ctx->r31 | 0;
    // 0x80168CFC: lw          $a1, 0x7C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X7C);
L_80168D00:
    // 0x80168D00: beq         $v1, $zero, L_80168D1C
    if (ctx->r3 == 0) {
        // 0x80168D04: lw          $t6, 0x7C($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X7C);
            goto L_80168D1C;
    }
    // 0x80168D04: lw          $t6, 0x7C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X7C);
    // 0x80168D08: addu        $t8, $t6, $t5
    ctx->r24 = ADD32(ctx->r14, ctx->r13);
    // 0x80168D0C: lwc1        $f8, 0x0($t8)
    ctx->f8.u32l = MEM_W(ctx->r24, 0X0);
    // 0x80168D10: c.le.s      $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f12.fl <= ctx->f8.fl;
    // 0x80168D14: nop

    // 0x80168D18: bc1f        L_80168D74
    if (!c1cs) {
        // 0x80168D1C: lui         $t7, 0x801A
        ctx->r15 = S32(0X801A << 16);
            goto L_80168D74;
    }
L_80168D1C:
    // 0x80168D1C: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x80168D20: addiu       $t7, $t7, -0x1110
    ctx->r15 = ADD32(ctx->r15, -0X1110);
    // 0x80168D24: addu        $v0, $t5, $t7
    ctx->r2 = ADD32(ctx->r13, ctx->r15);
    // 0x80168D28: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80168D2C: swc1        $f12, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f12.u32l;
    // 0x80168D30: swc1        $f10, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f10.u32l;
    // 0x80168D34: lh          $a0, 0x4($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X4);
    // 0x80168D38: swc1        $f12, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f12.u32l;
    // 0x80168D3C: sw          $t5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r13;
    // 0x80168D40: sw          $t2, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r10;
    // 0x80168D44: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x80168D48: sw          $v1, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r3;
    // 0x80168D4C: jal         0x80168AA0
    // 0x80168D50: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    func_80168AA0(rdram, ctx);
        goto after_0;
    // 0x80168D50: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    after_0:
    // 0x80168D54: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x80168D58: lwc1        $f16, 0x68($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80168D5C: lw          $v1, 0x60($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X60);
    // 0x80168D60: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    // 0x80168D64: lw          $t2, 0x64($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X64);
    // 0x80168D68: lw          $t5, 0x28($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X28);
    // 0x80168D6C: lwc1        $f12, 0x4C($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80168D70: swc1        $f16, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f16.u32l;
L_80168D74:
    // 0x80168D74: beq         $v1, $zero, L_80168D90
    if (ctx->r3 == 0) {
        // 0x80168D78: lw          $t9, 0x7C($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X7C);
            goto L_80168D90;
    }
    // 0x80168D78: lw          $t9, 0x7C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X7C);
    // 0x80168D7C: addu        $t6, $t9, $t5
    ctx->r14 = ADD32(ctx->r25, ctx->r13);
    // 0x80168D80: lwc1        $f18, 0x0($t6)
    ctx->f18.u32l = MEM_W(ctx->r14, 0X0);
    // 0x80168D84: c.le.s      $f18, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f18.fl <= ctx->f12.fl;
    // 0x80168D88: nop

    // 0x80168D8C: bc1f        L_80168DC0
    if (!c1cs) {
        // 0x80168D90: lui         $t8, 0x801A
        ctx->r24 = S32(0X801A << 16);
            goto L_80168DC0;
    }
L_80168D90:
    // 0x80168D90: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x80168D94: addiu       $t8, $t8, -0x1110
    ctx->r24 = ADD32(ctx->r24, -0X1110);
    // 0x80168D98: addu        $v0, $t5, $t8
    ctx->r2 = ADD32(ctx->r13, ctx->r24);
    // 0x80168D9C: lwc1        $f4, 0xC($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80168DA0: swc1        $f12, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f12.u32l;
    // 0x80168DA4: swc1        $f4, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f4.u32l;
    // 0x80168DA8: lh          $a0, 0x6($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X6);
    // 0x80168DAC: jal         0x80168AA0
    // 0x80168DB0: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    func_80168AA0(rdram, ctx);
        goto after_1;
    // 0x80168DB0: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    after_1:
    // 0x80168DB4: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x80168DB8: lwc1        $f6, 0x68($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80168DBC: swc1        $f6, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f6.u32l;
L_80168DC0:
    // 0x80168DC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80168DC4:
    // 0x80168DC4: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
    // 0x80168DC8: jr          $ra
    // 0x80168DCC: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_draw_depth--;
    // --- END PATCH ---
#endif
    return;
    // 0x80168DCC: nop

;}
RECOMP_FUNC void func_80168DD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80168DD0: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80168DD4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80168DD8: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80168DDC: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x80168DE0: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x80168DE4: or          $s3, $a1, $zero
    ctx->r19 = ctx->r5 | 0;
    // 0x80168DE8: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80168DEC: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x80168DF0: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80168DF4: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80168DF8: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80168DFC: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80168E00: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80168E04: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80168E08: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x80168E0C: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    // 0x80168E10: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80168E14: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
L_80168E18:
    // 0x80168E18: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80168E1C: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
L_80168E20:
    // 0x80168E20: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x80168E24: sll         $t7, $a2, 2
    ctx->r15 = S32(ctx->r6 << 2);
    // 0x80168E28: subu        $t7, $t7, $a2
    ctx->r15 = SUB32(ctx->r15, ctx->r6);
    // 0x80168E2C: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80168E30: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x80168E34: lwc1        $f4, 0x18($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X18);
    // 0x80168E38: addu        $t9, $t8, $v0
    ctx->r25 = ADD32(ctx->r24, ctx->r2);
    // 0x80168E3C: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80168E40: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80168E44: bne         $v0, $a0, L_80168E20
    if (ctx->r2 != ctx->r4) {
        // 0x80168E48: swc1        $f4, 0x5C($t9)
        MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
            goto L_80168E20;
    }
    // 0x80168E48: swc1        $f4, 0x5C($t9)
    MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
    // 0x80168E4C: addiu       $a2, $a2, 0x1
    ctx->r6 = ADD32(ctx->r6, 0X1);
    // 0x80168E50: slti        $at, $a2, 0x2
    ctx->r1 = SIGNED(ctx->r6) < 0X2 ? 1 : 0;
    // 0x80168E54: bne         $at, $zero, L_80168E18
    if (ctx->r1 != 0) {
        // 0x80168E58: addiu       $a3, $a3, 0xC
        ctx->r7 = ADD32(ctx->r7, 0XC);
            goto L_80168E18;
    }
    // 0x80168E58: addiu       $a3, $a3, 0xC
    ctx->r7 = ADD32(ctx->r7, 0XC);
    // 0x80168E5C: or          $s1, $s3, $zero
    ctx->r17 = ctx->r19 | 0;
    // 0x80168E60: lui         $fp, 0x801A
    ctx->r30 = S32(0X801A << 16);
    // 0x80168E64: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x80168E68: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x80168E6C: lui         $s5, 0x801A
    ctx->r21 = S32(0X801A << 16);
    // 0x80168E70: lui         $s4, 0x8019
    ctx->r20 = S32(0X8019 << 16);
    // 0x80168E74: addiu       $s4, $s4, 0x2520
    ctx->r20 = ADD32(ctx->r20, 0X2520);
    // 0x80168E78: addiu       $s5, $s5, -0x1118
    ctx->r21 = ADD32(ctx->r21, -0X1118);
    // 0x80168E7C: addiu       $s6, $s6, 0x2494
    ctx->r22 = ADD32(ctx->r22, 0X2494);
    // 0x80168E80: addiu       $s7, $s7, -0x1580
    ctx->r23 = ADD32(ctx->r23, -0X1580);
    // 0x80168E84: addiu       $fp, $fp, -0x978
    ctx->r30 = ADD32(ctx->r30, -0X978);
    // 0x80168E88: addiu       $s3, $zero, 0xC
    ctx->r19 = ADD32(0, 0XC);
    // 0x80168E8C: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
L_80168E90:
    // 0x80168E90: lw          $v1, 0x0($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X0);
    // 0x80168E94: beql        $v1, $zero, L_80168F00
    if (ctx->r3 == 0) {
        // 0x80168E98: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_80168F00;
    }
    goto skip_0;
    // 0x80168E98: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    skip_0:
    // 0x80168E9C: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80168EA0: addu        $t0, $s4, $s2
    ctx->r8 = ADD32(ctx->r20, ctx->r18);
    // 0x80168EA4: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x80168EA8: lw          $t2, 0x6E4($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X6E4);
    // 0x80168EAC: and         $t3, $t1, $t2
    ctx->r11 = ctx->r9 & ctx->r10;
    // 0x80168EB0: beql        $t3, $zero, L_80168F00
    if (ctx->r11 == 0) {
        // 0x80168EB4: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_80168F00;
    }
    goto skip_1;
    // 0x80168EB4: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    skip_1:
    // 0x80168EB8: lw          $t4, 0x18($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X18);
    // 0x80168EBC: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80168EC0: sw          $t4, 0x168($v0)
    MEM_W(0X168, ctx->r2) = ctx->r12;
    // 0x80168EC4: sw          $zero, 0x0($s5)
    MEM_W(0X0, ctx->r21) = 0;
    // 0x80168EC8: sw          $s7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r23;
    // 0x80168ECC: lw          $t5, 0x0($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X0);
    // 0x80168ED0: jal         0x80000F30
    // 0x80168ED4: lh          $a1, 0x1C($t5)
    ctx->r5 = MEM_H(ctx->r13, 0X1C);
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x80168ED4: lh          $a1, 0x1C($t5)
    ctx->r5 = MEM_H(ctx->r13, 0X1C);
    after_0:
    // 0x80168ED8: lw          $t6, 0xC($s1)
    ctx->r14 = MEM_W(ctx->r17, 0XC);
    // 0x80168EDC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80168EE0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80168EE4: sw          $t6, -0x10F4($at)
    MEM_W(-0X10F4, ctx->r1) = ctx->r14;
    // 0x80168EE8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x80168EEC: jal         0x80169070
    // 0x80168EF0: addiu       $a1, $a1, 0x38
    ctx->r5 = ADD32(ctx->r5, 0X38);
    func_80169070(rdram, ctx);
        goto after_1;
    // 0x80168EF0: addiu       $a1, $a1, 0x38
    ctx->r5 = ADD32(ctx->r5, 0X38);
    after_1:
    // 0x80168EF4: jal         0x801695C0
    // 0x80168EF8: nop

    func_801695C0(rdram, ctx);
        goto after_2;
    // 0x80168EF8: nop

    after_2:
    // 0x80168EFC: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
L_80168F00:
    // 0x80168F00: bne         $s2, $s3, L_80168E90
    if (ctx->r18 != ctx->r19) {
        // 0x80168F04: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_80168E90;
    }
    // 0x80168F04: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x80168F08: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80168F0C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80168F10: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80168F14: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80168F18: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80168F1C: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80168F20: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80168F24: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80168F28: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80168F2C: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80168F30: jr          $ra
    // 0x80168F34: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x80168F34: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_80168F38(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80168F38: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80168F3C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80168F40: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80168F44: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x80168F48: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x80168F4C: or          $s3, $a1, $zero
    ctx->r19 = ctx->r5 | 0;
    // 0x80168F50: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80168F54: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x80168F58: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x80168F5C: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x80168F60: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x80168F64: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80168F68: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80168F6C: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x80168F70: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    // 0x80168F74: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80168F78: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
L_80168F7C:
    // 0x80168F7C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80168F80: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
L_80168F84:
    // 0x80168F84: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x80168F88: sll         $t7, $a2, 2
    ctx->r15 = S32(ctx->r6 << 2);
    // 0x80168F8C: subu        $t7, $t7, $a2
    ctx->r15 = SUB32(ctx->r15, ctx->r6);
    // 0x80168F90: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80168F94: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x80168F98: lwc1        $f4, 0x18($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X18);
    // 0x80168F9C: addu        $t9, $t8, $v0
    ctx->r25 = ADD32(ctx->r24, ctx->r2);
    // 0x80168FA0: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80168FA4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80168FA8: bne         $v0, $a0, L_80168F84
    if (ctx->r2 != ctx->r4) {
        // 0x80168FAC: swc1        $f4, 0x5C($t9)
        MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
            goto L_80168F84;
    }
    // 0x80168FAC: swc1        $f4, 0x5C($t9)
    MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
    // 0x80168FB0: addiu       $a2, $a2, 0x1
    ctx->r6 = ADD32(ctx->r6, 0X1);
    // 0x80168FB4: slti        $at, $a2, 0x2
    ctx->r1 = SIGNED(ctx->r6) < 0X2 ? 1 : 0;
    // 0x80168FB8: bne         $at, $zero, L_80168F7C
    if (ctx->r1 != 0) {
        // 0x80168FBC: addiu       $a3, $a3, 0xC
        ctx->r7 = ADD32(ctx->r7, 0XC);
            goto L_80168F7C;
    }
    // 0x80168FBC: addiu       $a3, $a3, 0xC
    ctx->r7 = ADD32(ctx->r7, 0XC);
    // 0x80168FC0: or          $s2, $s3, $zero
    ctx->r18 = ctx->r19 | 0;
    // 0x80168FC4: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x80168FC8: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x80168FCC: lui         $s5, 0x801A
    ctx->r21 = S32(0X801A << 16);
    // 0x80168FD0: lui         $s4, 0x8019
    ctx->r20 = S32(0X8019 << 16);
    // 0x80168FD4: addiu       $s4, $s4, 0x2520
    ctx->r20 = ADD32(ctx->r20, 0X2520);
    // 0x80168FD8: addiu       $s5, $s5, -0x1118
    ctx->r21 = ADD32(ctx->r21, -0X1118);
    // 0x80168FDC: addiu       $s6, $s6, 0x2494
    ctx->r22 = ADD32(ctx->r22, 0X2494);
    // 0x80168FE0: addiu       $s7, $s7, -0x750
    ctx->r23 = ADD32(ctx->r23, -0X750);
    // 0x80168FE4: addiu       $s3, $zero, 0xC
    ctx->r19 = ADD32(0, 0XC);
    // 0x80168FE8: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80168FEC:
    // 0x80168FEC: lw          $v1, 0x0($s2)
    ctx->r3 = MEM_W(ctx->r18, 0X0);
    // 0x80168FF0: beql        $v1, $zero, L_8016903C
    if (ctx->r3 == 0) {
        // 0x80168FF4: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_8016903C;
    }
    goto skip_0;
    // 0x80168FF4: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    skip_0:
    // 0x80168FF8: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80168FFC: addu        $t0, $s4, $s1
    ctx->r8 = ADD32(ctx->r20, ctx->r17);
    // 0x80169000: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x80169004: lw          $t2, 0x6E4($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X6E4);
    // 0x80169008: and         $t3, $t1, $t2
    ctx->r11 = ctx->r9 & ctx->r10;
    // 0x8016900C: beql        $t3, $zero, L_8016903C
    if (ctx->r11 == 0) {
        // 0x80169010: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_8016903C;
    }
    goto skip_1;
    // 0x80169010: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    skip_1:
    // 0x80169014: lw          $t4, 0x18($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X18);
    // 0x80169018: sw          $t4, 0x168($v0)
    MEM_W(0X168, ctx->r2) = ctx->r12;
    // 0x8016901C: lw          $t5, 0x0($s2)
    ctx->r13 = MEM_W(ctx->r18, 0X0);
    // 0x80169020: lh          $a0, 0x1C($t5)
    ctx->r4 = MEM_H(ctx->r13, 0X1C);
    // 0x80169024: beql        $a0, $zero, L_8016903C
    if (ctx->r4 == 0) {
        // 0x80169028: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_8016903C;
    }
    goto skip_2;
    // 0x80169028: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    skip_2:
    // 0x8016902C: sw          $a0, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r4;
    // 0x80169030: jal         0x801695C0
    // 0x80169034: sw          $s7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r23;
    func_801695C0(rdram, ctx);
        goto after_0;
    // 0x80169034: sw          $s7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r23;
    after_0:
    // 0x80169038: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
L_8016903C:
    // 0x8016903C: bne         $s1, $s3, L_80168FEC
    if (ctx->r17 != ctx->r19) {
        // 0x80169040: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_80168FEC;
    }
    // 0x80169040: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x80169044: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80169048: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016904C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80169050: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80169054: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80169058: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x8016905C: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x80169060: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x80169064: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x80169068: jr          $ra
    // 0x8016906C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x8016906C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_80169070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_80169070 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_80169070++;
    if (lod_text_guard_depth_80169070 > 128) {
        if (!lod_text_guard_logged_80169070) {
            lod_text_guard_logged_80169070 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_80169070 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_80169070--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x80169070: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x80169074: sw          $a0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r4;
    // 0x80169078: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016907C: lw          $t7, -0x10F4($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X10F4);
    // 0x80169080: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80169084: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80169088: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x8016908C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80169090: sw          $a1, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r5;
    // 0x80169094: addu        $t2, $t6, $t7
    ctx->r10 = ADD32(ctx->r14, ctx->r15);
    // 0x80169098: lh          $v0, 0x0($t2)
    ctx->r2 = MEM_H(ctx->r10, 0X0);
    // 0x8016909C: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x801690A0: lui         $ra, 0x801A
    ctx->r31 = S32(0X801A << 16);
    // 0x801690A4: bne         $v0, $at, L_80169220
    if (ctx->r2 != ctx->r1) {
        // 0x801690A8: addiu       $ra, $ra, -0x9FC
        ctx->r31 = ADD32(ctx->r31, -0X9FC);
            goto L_80169220;
    }
    // 0x801690A8: addiu       $ra, $ra, -0x9FC
    ctx->r31 = ADD32(ctx->r31, -0X9FC);
    // 0x801690AC: lh          $t1, 0x2($t2)
    ctx->r9 = MEM_H(ctx->r10, 0X2);
    // 0x801690B0: lw          $a1, 0x4($t2)
    ctx->r5 = MEM_W(ctx->r10, 0X4);
    // 0x801690B4: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x801690B8: blez        $t1, L_801695B0
    if (SIGNED(ctx->r9) <= 0) {
        // 0x801690BC: lui         $ra, 0x801A
        ctx->r31 = S32(0X801A << 16);
            goto L_801695B0;
    }
    // 0x801690BC: lui         $ra, 0x801A
    ctx->r31 = S32(0X801A << 16);
    // 0x801690C0: lui         $t5, 0x8019
    ctx->r13 = S32(0X8019 << 16);
    // 0x801690C4: lui         $t3, 0x801A
    ctx->r11 = S32(0X801A << 16);
    // 0x801690C8: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x801690CC: addiu       $a3, $a3, -0x1118
    ctx->r7 = ADD32(ctx->r7, -0X1118);
    // 0x801690D0: addiu       $t3, $t3, -0x978
    ctx->r11 = ADD32(ctx->r11, -0X978);
    // 0x801690D4: addiu       $t5, $t5, 0x2494
    ctx->r13 = ADD32(ctx->r13, 0X2494);
    // 0x801690D8: addiu       $ra, $ra, -0x9FC
    ctx->r31 = ADD32(ctx->r31, -0X9FC);
    // 0x801690DC: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
L_801690E0:
    // 0x801690E0: lh          $v1, 0x0($a1)
    ctx->r3 = MEM_H(ctx->r5, 0X0);
    // 0x801690E4: addiu       $t0, $t0, 0x1
    ctx->r8 = ADD32(ctx->r8, 0X1);
    // 0x801690E8: addu        $a0, $t3, $v1
    ctx->r4 = ADD32(ctx->r11, ctx->r3);
    // 0x801690EC: lbu         $t8, 0x0($a0)
    ctx->r24 = MEM_BU(ctx->r4, 0X0);
    // 0x801690F0: bnel        $t8, $zero, L_80169210
    if (ctx->r24 != 0) {
        // 0x801690F4: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80169210;
    }
    goto skip_0;
    // 0x801690F4: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_0:
    // 0x801690F8: lw          $a2, 0x0($ra)
    ctx->r6 = MEM_W(ctx->r31, 0X0);
    // 0x801690FC: sll         $t9, $v1, 6
    ctx->r25 = S32(ctx->r3 << 6);
    // 0x80169100: lw          $t6, 0x168($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X168);
    // 0x80169104: lwc1        $f4, 0x160($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X160);
    // 0x80169108: addu        $v0, $t9, $t6
    ctx->r2 = ADD32(ctx->r25, ctx->r14);
    // 0x8016910C: lh          $t7, 0x2C($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X2C);
    // 0x80169110: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x80169114: nop

    // 0x80169118: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016911C: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x80169120: nop

    // 0x80169124: bc1tl       L_80169210
    if (c1cs) {
        // 0x80169128: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80169210;
    }
    goto skip_1;
    // 0x80169128: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_1:
    // 0x8016912C: lh          $t8, 0x2E($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X2E);
    // 0x80169130: lwc1        $f18, 0x154($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X154);
    // 0x80169134: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x80169138: nop

    // 0x8016913C: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80169140: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x80169144: nop

    // 0x80169148: bc1tl       L_80169210
    if (c1cs) {
        // 0x8016914C: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80169210;
    }
    goto skip_2;
    // 0x8016914C: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_2:
    // 0x80169150: lh          $t9, 0x30($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X30);
    // 0x80169154: lwc1        $f6, 0x15C($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X15C);
    // 0x80169158: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x8016915C: nop

    // 0x80169160: cvt.s.w     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80169164: c.lt.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl < ctx->f8.fl;
    // 0x80169168: nop

    // 0x8016916C: bc1tl       L_80169210
    if (c1cs) {
        // 0x80169170: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80169210;
    }
    goto skip_3;
    // 0x80169170: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_3:
    // 0x80169174: lh          $t6, 0x32($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X32);
    // 0x80169178: lwc1        $f18, 0x150($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X150);
    // 0x8016917C: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x80169180: nop

    // 0x80169184: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80169188: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016918C: nop

    // 0x80169190: bc1tl       L_80169210
    if (c1cs) {
        // 0x80169194: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80169210;
    }
    goto skip_4;
    // 0x80169194: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_4:
    // 0x80169198: lh          $t7, 0x34($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X34);
    // 0x8016919C: lwc1        $f4, 0x164($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X164);
    // 0x801691A0: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x801691A4: nop

    // 0x801691A8: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801691AC: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x801691B0: nop

    // 0x801691B4: bc1tl       L_80169210
    if (c1cs) {
        // 0x801691B8: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80169210;
    }
    goto skip_5;
    // 0x801691B8: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_5:
    // 0x801691BC: lh          $t8, 0x36($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X36);
    // 0x801691C0: lwc1        $f18, 0x158($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X158);
    // 0x801691C4: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x801691C8: nop

    // 0x801691CC: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801691D0: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x801691D4: nop

    // 0x801691D8: bc1tl       L_80169210
    if (c1cs) {
        // 0x801691DC: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_80169210;
    }
    goto skip_6;
    // 0x801691DC: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_6:
    // 0x801691E0: lw          $t7, 0x0($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X0);
    // 0x801691E4: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x801691E8: sb          $t4, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r12;
    // 0x801691EC: lh          $t9, 0x0($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X0);
    // 0x801691F0: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x801691F4: addu        $t7, $t6, $t8
    ctx->r15 = ADD32(ctx->r14, ctx->r24);
    // 0x801691F8: sh          $t9, 0x0($t7)
    MEM_H(0X0, ctx->r15) = ctx->r25;
    // 0x801691FC: lw          $t6, 0x0($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X0);
    // 0x80169200: addiu       $t8, $t6, 0x1
    ctx->r24 = ADD32(ctx->r14, 0X1);
    // 0x80169204: sw          $t8, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r24;
    // 0x80169208: lh          $t1, 0x2($t2)
    ctx->r9 = MEM_H(ctx->r10, 0X2);
    // 0x8016920C: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
L_80169210:
    // 0x80169210: bne         $at, $zero, L_801690E0
    if (ctx->r1 != 0) {
        // 0x80169214: addiu       $a1, $a1, 0x2
        ctx->r5 = ADD32(ctx->r5, 0X2);
            goto L_801690E0;
    }
    // 0x80169214: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x80169218: b           L_801695B4
    // 0x8016921C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801695B4;
    // 0x8016921C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80169220:
    // 0x80169220: lw          $a2, 0x0($ra)
    ctx->r6 = MEM_W(ctx->r31, 0X0);
    // 0x80169224: sll         $v1, $v0, 2
    ctx->r3 = S32(ctx->r2 << 2);
    // 0x80169228: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8016922C: addu        $t9, $a2, $v1
    ctx->r25 = ADD32(ctx->r6, ctx->r3);
    // 0x80169230: lwc1        $f0, 0x44($t9)
    ctx->f0.u32l = MEM_W(ctx->r25, 0X44);
    // 0x80169234: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80169238: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x8016923C: nop

    // 0x80169240: bc1fl       L_80169254
    if (!c1cs) {
        // 0x80169244: mov.s       $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
            goto L_80169254;
    }
    goto skip_7;
    // 0x80169244: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    skip_7:
    // 0x80169248: b           L_80169254
    // 0x8016924C: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_80169254;
    // 0x8016924C: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
    // 0x80169250: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_80169254:
    // 0x80169254: lwc1        $f4, -0x5198($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X5198);
    // 0x80169258: c.lt.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl < ctx->f4.fl;
    // 0x8016925C: nop

    // 0x80169260: bc1fl       L_80169274
    if (!c1cs) {
        // 0x80169264: lh          $t7, 0x2($t2)
        ctx->r15 = MEM_H(ctx->r10, 0X2);
            goto L_80169274;
    }
    goto skip_8;
    // 0x80169264: lh          $t7, 0x2($t2)
    ctx->r15 = MEM_H(ctx->r10, 0X2);
    skip_8:
    // 0x80169268: b           L_80169400
    // 0x8016926C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_80169400;
    // 0x8016926C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x80169270: lh          $t7, 0x2($t2)
    ctx->r15 = MEM_H(ctx->r10, 0X2);
L_80169274:
    // 0x80169274: addiu       $t4, $sp, 0x50
    ctx->r12 = ADD32(ctx->r29, 0X50);
    // 0x80169278: addu        $t6, $t4, $v1
    ctx->r14 = ADD32(ctx->r12, ctx->r3);
    // 0x8016927C: mtc1        $t7, $f8
    ctx->f8.u32l = ctx->r15;
    // 0x80169280: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80169284: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80169288: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8016928C: swc1        $f10, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f10.u32l;
    // 0x80169290: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    // 0x80169294: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x80169298: addu        $a0, $t4, $v1
    ctx->r4 = ADD32(ctx->r12, ctx->r3);
    // 0x8016929C: addu        $v0, $a2, $v1
    ctx->r2 = ADD32(ctx->r6, ctx->r3);
    // 0x801692A0: lwc1        $f18, 0x38($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X38);
    // 0x801692A4: lwc1        $f16, 0x0($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801692A8: lwc1        $f4, 0x44($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X44);
    // 0x801692AC: sub.s       $f6, $f16, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x801692B0: div.s       $f2, $f6, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f4.fl);
    // 0x801692B4: c.lt.s      $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f2.fl < ctx->f8.fl;
    // 0x801692B8: swc1        $f2, -0x6AC($at)
    MEM_W(-0X6AC, ctx->r1) = ctx->f2.u32l;
    // 0x801692BC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801692C0: bc1t        L_801692E4
    if (c1cs) {
        // 0x801692C4: nop
    
            goto L_801692E4;
    }
    // 0x801692C4: nop

    // 0x801692C8: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801692CC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801692D0: c.lt.s      $f10, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f10.fl < ctx->f2.fl;
    // 0x801692D4: nop

    // 0x801692D8: bc1fl       L_801692F0
    if (!c1cs) {
        // 0x801692DC: lh          $t8, 0x2($t2)
        ctx->r24 = MEM_H(ctx->r10, 0X2);
            goto L_801692F0;
    }
    goto skip_9;
    // 0x801692DC: lh          $t8, 0x2($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X2);
    skip_9:
    // 0x801692E0: swc1        $f2, -0x6AC($at)
    MEM_W(-0X6AC, ctx->r1) = ctx->f2.u32l;
L_801692E4:
    // 0x801692E4: b           L_80169400
    // 0x801692E8: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
        goto L_80169400;
    // 0x801692E8: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x801692EC: lh          $t8, 0x2($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X2);
L_801692F0:
    // 0x801692F0: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x801692F4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801692F8: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x801692FC: nop

    // 0x80169300: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80169304: swc1        $f18, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f18.u32l;
    // 0x80169308: lh          $v0, 0x0($t2)
    ctx->r2 = MEM_H(ctx->r10, 0X0);
    // 0x8016930C: bne         $v0, $zero, L_8016931C
    if (ctx->r2 != 0) {
        // 0x80169310: nop
    
            goto L_8016931C;
    }
    // 0x80169310: nop

    // 0x80169314: b           L_80169334
    // 0x80169318: addiu       $t1, $zero, 0x2
    ctx->r9 = ADD32(0, 0X2);
        goto L_80169334;
    // 0x80169318: addiu       $t1, $zero, 0x2
    ctx->r9 = ADD32(0, 0X2);
L_8016931C:
    // 0x8016931C: bne         $v0, $at, L_80169330
    if (ctx->r2 != ctx->r1) {
        // 0x80169320: or          $t0, $zero, $zero
        ctx->r8 = 0 | 0;
            goto L_80169330;
    }
    // 0x80169320: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x80169324: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x80169328: b           L_80169334
    // 0x8016932C: addiu       $t1, $zero, 0x2
    ctx->r9 = ADD32(0, 0X2);
        goto L_80169334;
    // 0x8016932C: addiu       $t1, $zero, 0x2
    ctx->r9 = ADD32(0, 0X2);
L_80169330:
    // 0x80169330: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
L_80169334:
    // 0x80169334: sll         $v0, $t0, 2
    ctx->r2 = S32(ctx->r8 << 2);
    // 0x80169338: addu        $a1, $a2, $v0
    ctx->r5 = ADD32(ctx->r6, ctx->r2);
    // 0x8016933C: lwc1        $f6, 0x44($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X44);
    // 0x80169340: lwc1        $f8, 0x38($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X38);
    // 0x80169344: addu        $v1, $t4, $v0
    ctx->r3 = ADD32(ctx->r12, ctx->r2);
    // 0x80169348: mul.s       $f4, $f6, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016934C: sll         $a0, $t1, 2
    ctx->r4 = S32(ctx->r9 << 2);
    // 0x80169350: addu        $a3, $a2, $a0
    ctx->r7 = ADD32(ctx->r6, ctx->r4);
    // 0x80169354: addu        $t3, $t4, $a0
    ctx->r11 = ADD32(ctx->r12, ctx->r4);
    // 0x80169358: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016935C: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80169360: swc1        $f10, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f10.u32l;
    // 0x80169364: lwc1        $f16, 0x44($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X44);
    // 0x80169368: lwc1        $f6, 0x38($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X38);
    // 0x8016936C: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x80169370: add.s       $f4, $f18, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f6.fl;
    // 0x80169374: swc1        $f4, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->f4.u32l;
    // 0x80169378: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016937C: sw          $t0, -0x6B4($at)
    MEM_W(-0X6B4, ctx->r1) = ctx->r8;
    // 0x80169380: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80169384: sw          $t1, -0x6B0($at)
    MEM_W(-0X6B0, ctx->r1) = ctx->r9;
    // 0x80169388: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016938C: swc1        $f2, -0x6AC($at)
    MEM_W(-0X6AC, ctx->r1) = ctx->f2.u32l;
    // 0x80169390: lwc1        $f8, 0x5C($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X5C);
    // 0x80169394: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x80169398: nop

    // 0x8016939C: bc1tl       L_801693E8
    if (c1cs) {
        // 0x801693A0: lh          $v1, 0x0($t2)
        ctx->r3 = MEM_H(ctx->r10, 0X0);
            goto L_801693E8;
    }
    goto skip_10;
    // 0x801693A0: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    skip_10:
    // 0x801693A4: lwc1        $f10, 0x68($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X68);
    // 0x801693A8: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x801693AC: nop

    // 0x801693B0: bc1tl       L_801693E8
    if (c1cs) {
        // 0x801693B4: lh          $v1, 0x0($t2)
        ctx->r3 = MEM_H(ctx->r10, 0X0);
            goto L_801693E8;
    }
    goto skip_11;
    // 0x801693B4: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    skip_11:
    // 0x801693B8: lwc1        $f0, 0x0($t3)
    ctx->f0.u32l = MEM_W(ctx->r11, 0X0);
    // 0x801693BC: lwc1        $f16, 0x5C($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X5C);
    // 0x801693C0: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x801693C4: nop

    // 0x801693C8: bc1tl       L_801693E8
    if (c1cs) {
        // 0x801693CC: lh          $v1, 0x0($t2)
        ctx->r3 = MEM_H(ctx->r10, 0X0);
            goto L_801693E8;
    }
    goto skip_12;
    // 0x801693CC: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    skip_12:
    // 0x801693D0: lwc1        $f18, 0x68($a3)
    ctx->f18.u32l = MEM_W(ctx->r7, 0X68);
    // 0x801693D4: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x801693D8: nop

    // 0x801693DC: bc1fl       L_801693F8
    if (!c1cs) {
        // 0x801693E0: lh          $v1, 0x0($t2)
        ctx->r3 = MEM_H(ctx->r10, 0X0);
            goto L_801693F8;
    }
    goto skip_13;
    // 0x801693E0: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    skip_13:
    // 0x801693E4: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
L_801693E8:
    // 0x801693E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801693EC: b           L_80169400
    // 0x801693F0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
        goto L_80169400;
    // 0x801693F0: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x801693F4: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
L_801693F8:
    // 0x801693F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801693FC: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
L_80169400:
    // 0x80169400: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80169404: bne         $v0, $zero, L_801694CC
    if (ctx->r2 != 0) {
        // 0x80169408: sw          $v0, -0x6B8($at)
        MEM_W(-0X6B8, ctx->r1) = ctx->r2;
            goto L_801694CC;
    }
    // 0x80169408: sw          $v0, -0x6B8($at)
    MEM_W(-0X6B8, ctx->r1) = ctx->r2;
    // 0x8016940C: addu        $v0, $a2, $v1
    ctx->r2 = ADD32(ctx->r6, ctx->r3);
    // 0x80169410: lwc1        $f6, 0x5C($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X5C);
    // 0x80169414: addiu       $a3, $t2, 0x4
    ctx->r7 = ADD32(ctx->r10, 0X4);
    // 0x80169418: addiu       $a1, $sp, 0x50
    ctx->r5 = ADD32(ctx->r29, 0X50);
    // 0x8016941C: swc1        $f6, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f6.u32l;
    // 0x80169420: lh          $t9, 0x2($t2)
    ctx->r25 = MEM_H(ctx->r10, 0X2);
    // 0x80169424: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x80169428: nop

    // 0x8016942C: cvt.s.w     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80169430: swc1        $f8, 0x5C($v0)
    MEM_W(0X5C, ctx->r2) = ctx->f8.u32l;
    // 0x80169434: lh          $a0, 0x0($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X0);
    // 0x80169438: sw          $t2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r10;
    // 0x8016943C: jal         0x80169070
    // 0x80169440: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    func_80169070(rdram, ctx);
        goto after_0;
    // 0x80169440: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    after_0:
    // 0x80169444: lw          $t2, 0x48($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X48);
    // 0x80169448: lui         $ra, 0x801A
    ctx->r31 = S32(0X801A << 16);
    // 0x8016944C: addiu       $ra, $ra, -0x9FC
    ctx->r31 = ADD32(ctx->r31, -0X9FC);
    // 0x80169450: lh          $t6, 0x0($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X0);
    // 0x80169454: lw          $t7, 0x0($ra)
    ctx->r15 = MEM_W(ctx->r31, 0X0);
    // 0x80169458: lwc1        $f10, 0x5C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8016945C: sll         $t8, $t6, 2
    ctx->r24 = S32(ctx->r14 << 2);
    // 0x80169460: lw          $a3, 0x34($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X34);
    // 0x80169464: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x80169468: swc1        $f10, 0x5C($t9)
    MEM_W(0X5C, ctx->r25) = ctx->f10.u32l;
    // 0x8016946C: lh          $t7, 0x0($t2)
    ctx->r15 = MEM_H(ctx->r10, 0X0);
    // 0x80169470: lw          $t6, 0x0($ra)
    ctx->r14 = MEM_W(ctx->r31, 0X0);
    // 0x80169474: addiu       $a1, $sp, 0x50
    ctx->r5 = ADD32(ctx->r29, 0X50);
    // 0x80169478: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8016947C: addu        $v0, $t6, $t8
    ctx->r2 = ADD32(ctx->r14, ctx->r24);
    // 0x80169480: lwc1        $f16, 0x68($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X68);
    // 0x80169484: swc1        $f16, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f16.u32l;
    // 0x80169488: lh          $t9, 0x2($t2)
    ctx->r25 = MEM_H(ctx->r10, 0X2);
    // 0x8016948C: mtc1        $t9, $f18
    ctx->f18.u32l = ctx->r25;
    // 0x80169490: nop

    // 0x80169494: cvt.s.w     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    ctx->f6.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80169498: swc1        $f6, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f6.u32l;
    // 0x8016949C: jal         0x80169070
    // 0x801694A0: lh          $a0, 0x2($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X2);
    func_80169070(rdram, ctx);
        goto after_1;
    // 0x801694A0: lh          $a0, 0x2($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X2);
    after_1:
    // 0x801694A4: lw          $t2, 0x48($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X48);
    // 0x801694A8: lui         $ra, 0x801A
    ctx->r31 = S32(0X801A << 16);
    // 0x801694AC: addiu       $ra, $ra, -0x9FC
    ctx->r31 = ADD32(ctx->r31, -0X9FC);
    // 0x801694B0: lh          $t6, 0x0($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X0);
    // 0x801694B4: lw          $t7, 0x0($ra)
    ctx->r15 = MEM_W(ctx->r31, 0X0);
    // 0x801694B8: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x801694BC: sll         $t8, $t6, 2
    ctx->r24 = S32(ctx->r14 << 2);
    // 0x801694C0: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x801694C4: b           L_801695B0
    // 0x801694C8: swc1        $f4, 0x68($t9)
    MEM_W(0X68, ctx->r25) = ctx->f4.u32l;
        goto L_801695B0;
    // 0x801694C8: swc1        $f4, 0x68($t9)
    MEM_W(0X68, ctx->r25) = ctx->f4.u32l;
L_801694CC:
    // 0x801694CC: lh          $t8, 0x2($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X2);
    // 0x801694D0: lw          $t6, 0x64($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X64);
    // 0x801694D4: addu        $v0, $a2, $v1
    ctx->r2 = ADD32(ctx->r6, ctx->r3);
    // 0x801694D8: mtc1        $t8, $f8
    ctx->f8.u32l = ctx->r24;
    // 0x801694DC: addu        $t7, $t6, $v1
    ctx->r15 = ADD32(ctx->r14, ctx->r3);
    // 0x801694E0: lwc1        $f2, 0x0($t7)
    ctx->f2.u32l = MEM_W(ctx->r15, 0X0);
    // 0x801694E4: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801694E8: c.le.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl <= ctx->f2.fl;
    // 0x801694EC: nop

    // 0x801694F0: bc1fl       L_8016955C
    if (!c1cs) {
        // 0x801694F4: c.le.s      $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
            goto L_8016955C;
    }
    goto skip_14;
    // 0x801694F4: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    skip_14:
    // 0x801694F8: lwc1        $f10, 0x5C($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X5C);
    // 0x801694FC: swc1        $f10, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f10.u32l;
    // 0x80169500: swc1        $f0, 0x5C($v0)
    MEM_W(0X5C, ctx->r2) = ctx->f0.u32l;
    // 0x80169504: lh          $a0, 0x4($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X4);
    // 0x80169508: sw          $t2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r10;
    // 0x8016950C: jal         0x80169070
    // 0x80169510: lw          $a1, 0x64($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X64);
    func_80169070(rdram, ctx);
        goto after_2;
    // 0x80169510: lw          $a1, 0x64($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X64);
    after_2:
    // 0x80169514: lw          $t2, 0x48($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X48);
    // 0x80169518: lui         $ra, 0x801A
    ctx->r31 = S32(0X801A << 16);
    // 0x8016951C: addiu       $ra, $ra, -0x9FC
    ctx->r31 = ADD32(ctx->r31, -0X9FC);
    // 0x80169520: lh          $t6, 0x0($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X0);
    // 0x80169524: lw          $t9, 0x0($ra)
    ctx->r25 = MEM_W(ctx->r31, 0X0);
    // 0x80169528: lwc1        $f16, 0x5C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8016952C: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80169530: addu        $t8, $t9, $t7
    ctx->r24 = ADD32(ctx->r25, ctx->r15);
    // 0x80169534: swc1        $f16, 0x5C($t8)
    MEM_W(0X5C, ctx->r24) = ctx->f16.u32l;
    // 0x80169538: lh          $t6, 0x2($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X2);
    // 0x8016953C: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    // 0x80169540: lw          $t9, 0x64($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X64);
    // 0x80169544: mtc1        $t6, $f18
    ctx->f18.u32l = ctx->r14;
    // 0x80169548: sll         $v1, $v1, 2
    ctx->r3 = S32(ctx->r3 << 2);
    // 0x8016954C: addu        $t7, $t9, $v1
    ctx->r15 = ADD32(ctx->r25, ctx->r3);
    // 0x80169550: lwc1        $f2, 0x0($t7)
    ctx->f2.u32l = MEM_W(ctx->r15, 0X0);
    // 0x80169554: cvt.s.w     $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    ctx->f0.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80169558: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
L_8016955C:
    // 0x8016955C: nop

    // 0x80169560: bc1fl       L_801695B4
    if (!c1cs) {
        // 0x80169564: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801695B4;
    }
    goto skip_15;
    // 0x80169564: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_15:
    // 0x80169568: lw          $t8, 0x0($ra)
    ctx->r24 = MEM_W(ctx->r31, 0X0);
    // 0x8016956C: addu        $v0, $t8, $v1
    ctx->r2 = ADD32(ctx->r24, ctx->r3);
    // 0x80169570: lwc1        $f6, 0x68($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X68);
    // 0x80169574: swc1        $f6, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f6.u32l;
    // 0x80169578: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x8016957C: lh          $a0, 0x6($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X6);
    // 0x80169580: sw          $t2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r10;
    // 0x80169584: jal         0x80169070
    // 0x80169588: lw          $a1, 0x64($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X64);
    func_80169070(rdram, ctx);
        goto after_3;
    // 0x80169588: lw          $a1, 0x64($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X64);
    after_3:
    // 0x8016958C: lw          $t2, 0x48($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X48);
    // 0x80169590: lui         $ra, 0x801A
    ctx->r31 = S32(0X801A << 16);
    // 0x80169594: addiu       $ra, $ra, -0x9FC
    ctx->r31 = ADD32(ctx->r31, -0X9FC);
    // 0x80169598: lh          $t9, 0x0($t2)
    ctx->r25 = MEM_H(ctx->r10, 0X0);
    // 0x8016959C: lw          $t6, 0x0($ra)
    ctx->r14 = MEM_W(ctx->r31, 0X0);
    // 0x801695A0: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x801695A4: sll         $t7, $t9, 2
    ctx->r15 = S32(ctx->r25 << 2);
    // 0x801695A8: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x801695AC: swc1        $f4, 0x68($t8)
    MEM_W(0X68, ctx->r24) = ctx->f4.u32l;
L_801695B0:
    // 0x801695B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801695B4:
    // 0x801695B4: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    // 0x801695B8: jr          $ra
    // 0x801695BC: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_80169070--;
    // --- END PATCH ---
#endif
    return;
    // 0x801695BC: nop

;}
RECOMP_FUNC void func_801695C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801695C0: addiu       $sp, $sp, -0xC0
    ctx->r29 = ADD32(ctx->r29, -0XC0);
    // 0x801695C4: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x801695C8: lw          $t6, -0x1118($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X1118);
    // 0x801695CC: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x801695D0: sw          $ra, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r31;
    // 0x801695D4: sw          $fp, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r30;
    // 0x801695D8: sw          $s7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r23;
    // 0x801695DC: sw          $s6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r22;
    // 0x801695E0: sw          $s5, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r21;
    // 0x801695E4: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x801695E8: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x801695EC: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x801695F0: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x801695F4: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x801695F8: sw          $zero, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = 0;
    // 0x801695FC: blez        $t6, L_8016984C
    if (SIGNED(ctx->r14) <= 0) {
        // 0x80169600: or          $s4, $zero, $zero
        ctx->r20 = 0 | 0;
            goto L_8016984C;
    }
    // 0x80169600: or          $s4, $zero, $zero
    ctx->r20 = 0 | 0;
    // 0x80169604: addiu       $s1, $sp, 0x80
    ctx->r17 = ADD32(ctx->r29, 0X80);
    // 0x80169608: lui         $s3, 0x801A
    ctx->r19 = S32(0X801A << 16);
    // 0x8016960C: addiu       $s3, $s3, -0x9FC
    ctx->r19 = ADD32(ctx->r19, -0X9FC);
    // 0x80169610: or          $s7, $s1, $zero
    ctx->r23 = ctx->r17 | 0;
    // 0x80169614: or          $s5, $zero, $zero
    ctx->r21 = 0 | 0;
    // 0x80169618: addiu       $fp, $zero, 0x1
    ctx->r30 = ADD32(0, 0X1);
    // 0x8016961C: addiu       $s6, $sp, 0x98
    ctx->r22 = ADD32(ctx->r29, 0X98);
L_80169620:
    // 0x80169620: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80169624: lw          $t8, 0x2494($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X2494);
    // 0x80169628: lw          $s0, 0x0($s3)
    ctx->r16 = MEM_W(ctx->r19, 0X0);
    // 0x8016962C: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x80169630: addu        $t9, $t8, $s5
    ctx->r25 = ADD32(ctx->r24, ctx->r21);
    // 0x80169634: lh          $t0, 0x0($t9)
    ctx->r8 = MEM_H(ctx->r25, 0X0);
    // 0x80169638: lw          $t2, 0x168($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X168);
    // 0x8016963C: or          $a3, $s6, $zero
    ctx->r7 = ctx->r22 | 0;
    // 0x80169640: sll         $t1, $t0, 6
    ctx->r9 = S32(ctx->r8 << 6);
    // 0x80169644: addu        $s2, $t1, $t2
    ctx->r18 = ADD32(ctx->r9, ctx->r10);
    // 0x80169648: lh          $t3, 0x28($s2)
    ctx->r11 = MEM_H(ctx->r18, 0X28);
    // 0x8016964C: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80169650: addiu       $a1, $s0, 0x38
    ctx->r5 = ADD32(ctx->r16, 0X38);
    // 0x80169654: bne         $t3, $at, L_80169680
    if (ctx->r11 != ctx->r1) {
        // 0x80169658: addiu       $a2, $s0, 0x44
        ctx->r6 = ADD32(ctx->r16, 0X44);
            goto L_80169680;
    }
    // 0x80169658: addiu       $a2, $s0, 0x44
    ctx->r6 = ADD32(ctx->r16, 0X44);
    // 0x8016965C: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80169660: addiu       $a1, $s0, 0x38
    ctx->r5 = ADD32(ctx->r16, 0X38);
    // 0x80169664: jal         0x8016A15C
    // 0x80169668: addiu       $a2, $s0, 0x44
    ctx->r6 = ADD32(ctx->r16, 0X44);
    func_8016A15C(rdram, ctx);
        goto after_0;
    // 0x80169668: addiu       $a2, $s0, 0x44
    ctx->r6 = ADD32(ctx->r16, 0X44);
    after_0:
    // 0x8016966C: lw          $s0, 0x0($s3)
    ctx->r16 = MEM_W(ctx->r19, 0X0);
    // 0x80169670: lw          $t4, 0x6EC($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X6EC);
    // 0x80169674: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x80169678: b           L_80169698
    // 0x8016967C: sw          $t5, 0x6EC($s0)
    MEM_W(0X6EC, ctx->r16) = ctx->r13;
        goto L_80169698;
    // 0x8016967C: sw          $t5, 0x6EC($s0)
    MEM_W(0X6EC, ctx->r16) = ctx->r13;
L_80169680:
    // 0x80169680: jal         0x8016A5B8
    // 0x80169684: or          $a3, $s6, $zero
    ctx->r7 = ctx->r22 | 0;
    func_8016A5B8(rdram, ctx);
        goto after_1;
    // 0x80169684: or          $a3, $s6, $zero
    ctx->r7 = ctx->r22 | 0;
    after_1:
    // 0x80169688: lw          $s0, 0x0($s3)
    ctx->r16 = MEM_W(ctx->r19, 0X0);
    // 0x8016968C: lw          $t6, 0x6F0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X6F0);
    // 0x80169690: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80169694: sw          $t7, 0x6F0($s0)
    MEM_W(0X6F0, ctx->r16) = ctx->r15;
L_80169698:
    // 0x80169698: lw          $s0, 0x0($s3)
    ctx->r16 = MEM_W(ctx->r19, 0X0);
    // 0x8016969C: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    // 0x801696A0: lw          $t8, 0x6E8($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X6E8);
    // 0x801696A4: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x801696A8: bne         $v0, $zero, L_80169830
    if (ctx->r2 != 0) {
        // 0x801696AC: sw          $t9, 0x6E8($s0)
        MEM_W(0X6E8, ctx->r16) = ctx->r25;
            goto L_80169830;
    }
    // 0x801696AC: sw          $t9, 0x6E8($s0)
    MEM_W(0X6E8, ctx->r16) = ctx->r25;
    // 0x801696B0: lw          $a0, 0x0($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X0);
    // 0x801696B4: or          $a2, $s7, $zero
    ctx->r6 = ctx->r23 | 0;
    // 0x801696B8: jal         0x80173524
    // 0x801696BC: addiu       $a0, $a0, 0x1E4
    ctx->r4 = ADD32(ctx->r4, 0X1E4);
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x801696BC: addiu       $a0, $a0, 0x1E4
    ctx->r4 = ADD32(ctx->r4, 0X1E4);
    after_2:
    // 0x801696C0: lw          $s0, 0x0($s3)
    ctx->r16 = MEM_W(ctx->r19, 0X0);
    // 0x801696C4: addiu       $v0, $sp, 0x80
    ctx->r2 = ADD32(ctx->r29, 0X80);
    // 0x801696C8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801696CC: addiu       $v1, $sp, 0x74
    ctx->r3 = ADD32(ctx->r29, 0X74);
    // 0x801696D0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801696D4: lw          $t0, 0x0($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X0);
    // 0x801696D8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801696DC: addu        $t1, $t0, $a0
    ctx->r9 = ADD32(ctx->r8, ctx->r4);
    // 0x801696E0: beq         $v1, $s1, L_80169728
    if (ctx->r3 == ctx->r17) {
        // 0x801696E4: lh          $t2, 0x0($t1)
        ctx->r10 = MEM_H(ctx->r9, 0X0);
            goto L_80169728;
    }
    // 0x801696E4: lh          $t2, 0x0($t1)
    ctx->r10 = MEM_H(ctx->r9, 0X0);
L_801696E8:
    // 0x801696E8: mtc1        $t2, $f20
    ctx->f20.u32l = ctx->r10;
    // 0x801696EC: lwc1        $f18, 0x0($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801696F0: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x801696F4: cvt.s.w     $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    ctx->f20.fl = CVT_S_W(ctx->f20.u32l);
    // 0x801696F8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801696FC: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80169700: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x80169704: add.s       $f20, $f18, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = ctx->f18.fl + ctx->f20.fl;
    // 0x80169708: swc1        $f20, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f20.u32l;
    // 0x8016970C: lwc1        $f18, 0x10($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X10);
    // 0x80169710: sub.s       $f18, $f20, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f20.fl - ctx->f18.fl;
    // 0x80169714: swc1        $f18, -0x8($v1)
    MEM_W(-0X8, ctx->r3) = ctx->f18.u32l;
    // 0x80169718: lw          $t0, 0x0($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X0);
    // 0x8016971C: addu        $t1, $t0, $a0
    ctx->r9 = ADD32(ctx->r8, ctx->r4);
    // 0x80169720: bne         $v1, $s1, L_801696E8
    if (ctx->r3 != ctx->r17) {
        // 0x80169724: lh          $t2, 0x0($t1)
        ctx->r10 = MEM_H(ctx->r9, 0X0);
            goto L_801696E8;
    }
    // 0x80169724: lh          $t2, 0x0($t1)
    ctx->r10 = MEM_H(ctx->r9, 0X0);
L_80169728:
    // 0x80169728: mtc1        $t2, $f20
    ctx->f20.u32l = ctx->r10;
    // 0x8016972C: lwc1        $f18, 0x0($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80169730: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x80169734: cvt.s.w     $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    ctx->f20.fl = CVT_S_W(ctx->f20.u32l);
    // 0x80169738: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016973C: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x80169740: add.s       $f20, $f18, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = ctx->f18.fl + ctx->f20.fl;
    // 0x80169744: swc1        $f20, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f20.u32l;
    // 0x80169748: lwc1        $f18, 0x10($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X10);
    // 0x8016974C: sub.s       $f18, $f20, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f20.fl - ctx->f18.fl;
    // 0x80169750: swc1        $f18, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f18.u32l;
    // 0x80169754: lwc1        $f18, 0x74($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X74);
    // 0x80169758: lwc1        $f4, 0x78($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X78);
    // 0x8016975C: lwc1        $f16, 0x7C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X7C);
    // 0x80169760: mul.s       $f6, $f18, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x80169764: lbu         $a0, 0x2B($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X2B);
    // 0x80169768: mul.s       $f8, $f4, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f4.fl);
    // 0x8016976C: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80169770: mul.s       $f18, $f16, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x80169774: jal         0x80166F40
    // 0x80169778: add.s       $f20, $f18, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f20.fl = ctx->f18.fl + ctx->f10.fl;
    func_80166F40(rdram, ctx);
        goto after_3;
    // 0x80169778: add.s       $f20, $f18, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f20.fl = ctx->f18.fl + ctx->f10.fl;
    after_3:
    // 0x8016977C: bne         $v0, $zero, L_80169790
    if (ctx->r2 != 0) {
        // 0x80169780: or          $a1, $s7, $zero
        ctx->r5 = ctx->r23 | 0;
            goto L_80169790;
    }
    // 0x80169780: or          $a1, $s7, $zero
    ctx->r5 = ctx->r23 | 0;
    // 0x80169784: lw          $v0, 0x0($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X0);
    // 0x80169788: b           L_80169798
    // 0x8016978C: addiu       $v0, $v0, 0x230
    ctx->r2 = ADD32(ctx->r2, 0X230);
        goto L_80169798;
    // 0x8016978C: addiu       $v0, $v0, 0x230
    ctx->r2 = ADD32(ctx->r2, 0X230);
L_80169790:
    // 0x80169790: lw          $v0, 0x0($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X0);
    // 0x80169794: addiu       $v0, $v0, 0x48C
    ctx->r2 = ADD32(ctx->r2, 0X48C);
L_80169798:
    // 0x80169798: lbu         $v1, 0x2A($s2)
    ctx->r3 = MEM_BU(ctx->r18, 0X2A);
    // 0x8016979C: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x801697A0: bne         $v1, $zero, L_801697B0
    if (ctx->r3 != 0) {
        // 0x801697A4: nop
    
            goto L_801697B0;
    }
    // 0x801697A4: nop

    // 0x801697A8: b           L_801697C0
    // 0x801697AC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
        goto L_801697C0;
    // 0x801697AC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_801697B0:
    // 0x801697B0: bne         $fp, $v1, L_801697C0
    if (ctx->r30 != ctx->r3) {
        // 0x801697B4: addiu       $s0, $v0, 0x190
        ctx->r16 = ADD32(ctx->r2, 0X190);
            goto L_801697C0;
    }
    // 0x801697B4: addiu       $s0, $v0, 0x190
    ctx->r16 = ADD32(ctx->r2, 0X190);
    // 0x801697B8: b           L_801697C0
    // 0x801697BC: addiu       $s0, $v0, 0xC8
    ctx->r16 = ADD32(ctx->r2, 0XC8);
        goto L_801697C0;
    // 0x801697BC: addiu       $s0, $v0, 0xC8
    ctx->r16 = ADD32(ctx->r2, 0XC8);
L_801697C0:
    // 0x801697C0: lh          $t3, 0x60($s0)
    ctx->r11 = MEM_H(ctx->r16, 0X60);
    // 0x801697C4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801697C8: beq         $t3, $zero, L_801697E4
    if (ctx->r11 == 0) {
        // 0x801697CC: nop
    
            goto L_801697E4;
    }
    // 0x801697CC: nop

    // 0x801697D0: lwc1        $f4, 0x1C($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x801697D4: c.lt.s      $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f20.fl < ctx->f4.fl;
    // 0x801697D8: nop

    // 0x801697DC: bc1f        L_80169830
    if (!c1cs) {
        // 0x801697E0: nop
    
            goto L_80169830;
    }
    // 0x801697E0: nop

L_801697E4:
    // 0x801697E4: jal         0x80172E2C
    // 0x801697E8: sw          $t4, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = ctx->r12;
    func_80172E2C(rdram, ctx);
        goto after_4;
    // 0x801697E8: sw          $t4, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = ctx->r12;
    after_4:
    // 0x801697EC: addiu       $a0, $s0, 0xC
    ctx->r4 = ADD32(ctx->r16, 0XC);
    // 0x801697F0: jal         0x80172E2C
    // 0x801697F4: addiu       $a1, $sp, 0x74
    ctx->r5 = ADD32(ctx->r29, 0X74);
    func_80172E2C(rdram, ctx);
        goto after_5;
    // 0x801697F4: addiu       $a1, $sp, 0x74
    ctx->r5 = ADD32(ctx->r29, 0X74);
    after_5:
    // 0x801697F8: swc1        $f20, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f20.u32l;
    // 0x801697FC: sw          $s2, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r18;
    // 0x80169800: lw          $t5, 0x0($s3)
    ctx->r13 = MEM_W(ctx->r19, 0X0);
    // 0x80169804: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80169808: addiu       $a1, $s0, 0x48
    ctx->r5 = ADD32(ctx->r16, 0X48);
    // 0x8016980C: lw          $t6, 0x8($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X8);
    // 0x80169810: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x80169814: sw          $t6, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r14;
    // 0x80169818: lw          $t7, 0x0($s3)
    ctx->r15 = MEM_W(ctx->r19, 0X0);
    // 0x8016981C: lw          $t8, 0x0($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X0);
    // 0x80169820: lh          $t9, 0x8($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X8);
    // 0x80169824: jal         0x80001090
    // 0x80169828: sh          $t9, 0x5C($s0)
    MEM_H(0X5C, ctx->r16) = ctx->r25;
    memory_copy(rdram, ctx);
        goto after_6;
    // 0x80169828: sh          $t9, 0x5C($s0)
    MEM_H(0X5C, ctx->r16) = ctx->r25;
    after_6:
    // 0x8016982C: sh          $fp, 0x60($s0)
    MEM_H(0X60, ctx->r16) = ctx->r30;
L_80169830:
    // 0x80169830: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x80169834: lw          $t0, -0x1118($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X1118);
    // 0x80169838: addiu       $s4, $s4, 0x1
    ctx->r20 = ADD32(ctx->r20, 0X1);
    // 0x8016983C: addiu       $s5, $s5, 0x2
    ctx->r21 = ADD32(ctx->r21, 0X2);
    // 0x80169840: slt         $at, $s4, $t0
    ctx->r1 = SIGNED(ctx->r20) < SIGNED(ctx->r8) ? 1 : 0;
    // 0x80169844: bne         $at, $zero, L_80169620
    if (ctx->r1 != 0) {
        // 0x80169848: nop
    
            goto L_80169620;
    }
    // 0x80169848: nop

L_8016984C:
    // 0x8016984C: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    // 0x80169850: lw          $v0, 0xA8($sp)
    ctx->r2 = MEM_W(ctx->r29, 0XA8);
    // 0x80169854: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80169858: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8016985C: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80169860: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x80169864: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x80169868: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x8016986C: lw          $s5, 0x34($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X34);
    // 0x80169870: lw          $s6, 0x38($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X38);
    // 0x80169874: lw          $s7, 0x3C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X3C);
    // 0x80169878: lw          $fp, 0x40($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X40);
    // 0x8016987C: jr          $ra
    // 0x80169880: addiu       $sp, $sp, 0xC0
    ctx->r29 = ADD32(ctx->r29, 0XC0);
    return;
    // 0x80169880: addiu       $sp, $sp, 0xC0
    ctx->r29 = ADD32(ctx->r29, 0XC0);
;}
RECOMP_FUNC void func_80169884(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80169884: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x80169888: addiu       $t0, $t0, -0x9FC
    ctx->r8 = ADD32(ctx->r8, -0X9FC);
    // 0x8016988C: sw          $a0, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r4;
    // 0x80169890: sw          $zero, 0x22C($a0)
    MEM_W(0X22C, ctx->r4) = 0;
    // 0x80169894: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x80169898: sw          $zero, 0x488($t7)
    MEM_W(0X488, ctx->r15) = 0;
    // 0x8016989C: lw          $t8, 0x0($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X0);
    // 0x801698A0: sw          $zero, 0x6F0($t8)
    MEM_W(0X6F0, ctx->r24) = 0;
    // 0x801698A4: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x801698A8: lw          $v1, 0x6F0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X6F0);
    // 0x801698AC: sw          $v1, 0x6EC($v0)
    MEM_W(0X6EC, ctx->r2) = ctx->r3;
    // 0x801698B0: lw          $t9, 0x0($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X0);
    // 0x801698B4: sw          $v1, 0x6E8($t9)
    MEM_W(0X6E8, ctx->r25) = ctx->r3;
    // 0x801698B8: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x801698BC: sw          $a1, 0x4($t1)
    MEM_W(0X4, ctx->r9) = ctx->r5;
    // 0x801698C0: lw          $t2, 0x0($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X0);
    // 0x801698C4: sh          $zero, 0x484($t2)
    MEM_H(0X484, ctx->r10) = 0;
    // 0x801698C8: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x801698CC: lh          $a3, 0x484($v0)
    ctx->r7 = MEM_H(ctx->r2, 0X484);
    // 0x801698D0: sh          $a3, 0x420($v0)
    MEM_H(0X420, ctx->r2) = ctx->r7;
    // 0x801698D4: lw          $t3, 0x0($t0)
    ctx->r11 = MEM_W(ctx->r8, 0X0);
    // 0x801698D8: sh          $a3, 0x3BC($t3)
    MEM_H(0X3BC, ctx->r11) = ctx->r7;
    // 0x801698DC: lw          $t4, 0x0($t0)
    ctx->r12 = MEM_W(ctx->r8, 0X0);
    // 0x801698E0: sh          $a3, 0x358($t4)
    MEM_H(0X358, ctx->r12) = ctx->r7;
    // 0x801698E4: lw          $t5, 0x0($t0)
    ctx->r13 = MEM_W(ctx->r8, 0X0);
    // 0x801698E8: sh          $a3, 0x2F4($t5)
    MEM_H(0X2F4, ctx->r13) = ctx->r7;
    // 0x801698EC: lw          $t6, 0x0($t0)
    ctx->r14 = MEM_W(ctx->r8, 0X0);
    // 0x801698F0: sh          $a3, 0x290($t6)
    MEM_H(0X290, ctx->r14) = ctx->r7;
    // 0x801698F4: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x801698F8: sh          $zero, 0x6E0($t7)
    MEM_H(0X6E0, ctx->r15) = 0;
    // 0x801698FC: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x80169900: lh          $a3, 0x6E0($v0)
    ctx->r7 = MEM_H(ctx->r2, 0X6E0);
    // 0x80169904: sh          $a3, 0x67C($v0)
    MEM_H(0X67C, ctx->r2) = ctx->r7;
    // 0x80169908: lw          $t8, 0x0($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X0);
    // 0x8016990C: sh          $a3, 0x618($t8)
    MEM_H(0X618, ctx->r24) = ctx->r7;
    // 0x80169910: lw          $t9, 0x0($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X0);
    // 0x80169914: sh          $a3, 0x5B4($t9)
    MEM_H(0X5B4, ctx->r25) = ctx->r7;
    // 0x80169918: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x8016991C: sh          $a3, 0x550($t1)
    MEM_H(0X550, ctx->r9) = ctx->r7;
    // 0x80169920: lw          $t2, 0x0($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X0);
    // 0x80169924: sh          $a3, 0x4EC($t2)
    MEM_H(0X4EC, ctx->r10) = ctx->r7;
    // 0x80169928: lw          $t3, 0x0($t0)
    ctx->r11 = MEM_W(ctx->r8, 0X0);
    // 0x8016992C: jr          $ra
    // 0x80169930: sw          $a2, 0x6E4($t3)
    MEM_W(0X6E4, ctx->r11) = ctx->r6;
    return;
    // 0x80169930: sw          $a2, 0x6E4($t3)
    MEM_W(0X6E4, ctx->r11) = ctx->r6;
;}
RECOMP_FUNC void func_80169934(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80169934: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80169938: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016993C: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x80169940: addiu       $a1, $zero, 0xC
    ctx->r5 = ADD32(0, 0XC);
    // 0x80169944: addiu       $a0, $a0, -0x9FC
    ctx->r4 = ADD32(ctx->r4, -0X9FC);
    // 0x80169948: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8016994C:
    // 0x8016994C: lw          $t6, 0x0($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X0);
    // 0x80169950: lwc1        $f0, 0x0($a2)
    ctx->f0.u32l = MEM_W(ctx->r6, 0X0);
    // 0x80169954: addiu       $a2, $a2, 0x4
    ctx->r6 = ADD32(ctx->r6, 0X4);
    // 0x80169958: addu        $t7, $t6, $v1
    ctx->r15 = ADD32(ctx->r14, ctx->r3);
    // 0x8016995C: swc1        $f0, 0x14($t7)
    MEM_W(0X14, ctx->r15) = ctx->f0.u32l;
    // 0x80169960: lw          $t8, 0x0($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X0);
    // 0x80169964: addu        $t9, $t8, $v1
    ctx->r25 = ADD32(ctx->r24, ctx->r3);
    // 0x80169968: swc1        $f0, 0xA4($t9)
    MEM_W(0XA4, ctx->r25) = ctx->f0.u32l;
    // 0x8016996C: lw          $t0, 0x0($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X0);
    // 0x80169970: lwc1        $f2, 0x0($a3)
    ctx->f2.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80169974: addu        $t1, $t0, $v1
    ctx->r9 = ADD32(ctx->r8, ctx->r3);
    // 0x80169978: swc1        $f2, 0x20($t1)
    MEM_W(0X20, ctx->r9) = ctx->f2.u32l;
    // 0x8016997C: add.s       $f4, $f2, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f2.fl + ctx->f0.fl;
    // 0x80169980: lw          $t2, 0x0($a0)
    ctx->r10 = MEM_W(ctx->r4, 0X0);
    // 0x80169984: addu        $t3, $t2, $v1
    ctx->r11 = ADD32(ctx->r10, ctx->r3);
    // 0x80169988: swc1        $f4, 0x2C($t3)
    MEM_W(0X2C, ctx->r11) = ctx->f4.u32l;
    // 0x8016998C: lw          $t4, 0x0($a0)
    ctx->r12 = MEM_W(ctx->r4, 0X0);
    // 0x80169990: addu        $v0, $t4, $v1
    ctx->r2 = ADD32(ctx->r12, ctx->r3);
    // 0x80169994: lwc1        $f12, 0x14($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80169998: lwc1        $f14, 0x2C($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X2C);
    // 0x8016999C: c.lt.s      $f12, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f12.fl < ctx->f14.fl;
    // 0x801699A0: nop

    // 0x801699A4: bc1fl       L_801699C8
    if (!c1cs) {
        // 0x801699A8: swc1        $f14, 0x138($v0)
        MEM_W(0X138, ctx->r2) = ctx->f14.u32l;
            goto L_801699C8;
    }
    goto skip_0;
    // 0x801699A8: swc1        $f14, 0x138($v0)
    MEM_W(0X138, ctx->r2) = ctx->f14.u32l;
    skip_0:
    // 0x801699AC: swc1        $f12, 0x138($v0)
    MEM_W(0X138, ctx->r2) = ctx->f12.u32l;
    // 0x801699B0: lw          $t5, 0x0($a0)
    ctx->r13 = MEM_W(ctx->r4, 0X0);
    // 0x801699B4: addu        $v0, $t5, $v1
    ctx->r2 = ADD32(ctx->r13, ctx->r3);
    // 0x801699B8: lwc1        $f6, 0x2C($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X2C);
    // 0x801699BC: b           L_801699D8
    // 0x801699C0: swc1        $f6, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->f6.u32l;
        goto L_801699D8;
    // 0x801699C0: swc1        $f6, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->f6.u32l;
    // 0x801699C4: swc1        $f14, 0x138($v0)
    MEM_W(0X138, ctx->r2) = ctx->f14.u32l;
L_801699C8:
    // 0x801699C8: lw          $t6, 0x0($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X0);
    // 0x801699CC: addu        $v0, $t6, $v1
    ctx->r2 = ADD32(ctx->r14, ctx->r3);
    // 0x801699D0: lwc1        $f8, 0x14($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X14);
    // 0x801699D4: swc1        $f8, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->f8.u32l;
L_801699D8:
    // 0x801699D8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801699DC: bne         $v1, $a1, L_8016994C
    if (ctx->r3 != ctx->r5) {
        // 0x801699E0: addiu       $a3, $a3, 0x4
        ctx->r7 = ADD32(ctx->r7, 0X4);
            goto L_8016994C;
    }
    // 0x801699E0: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x801699E4: jr          $ra
    // 0x801699E8: nop

    return;
    // 0x801699E8: nop

;}
RECOMP_FUNC void func_801699EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801699EC: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x801699F0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801699F4: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x801699F8: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x801699FC: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x80169A00: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80169A04: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80169A08: sw          $a0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r4;
    // 0x80169A0C: sw          $a0, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->r4;
    // 0x80169A10: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
    // 0x80169A14: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    // 0x80169A18: addiu       $a3, $sp, 0x44
    ctx->r7 = ADD32(ctx->r29, 0X44);
    // 0x80169A1C: sw          $a1, 0x8($t8)
    MEM_W(0X8, ctx->r24) = ctx->r5;
    // 0x80169A20: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x80169A24: lw          $a2, 0x60($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X60);
    // 0x80169A28: addiu       $v1, $sp, 0x38
    ctx->r3 = ADD32(ctx->r29, 0X38);
    // 0x80169A2C: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x80169A30: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80169A34: sltu        $at, $v1, $a0
    ctx->r1 = ctx->r3 < ctx->r4 ? 1 : 0;
    // 0x80169A38: beq         $at, $zero, L_80169A7C
    if (ctx->r1 == 0) {
        // 0x80169A3C: lh          $t9, 0x0($a2)
        ctx->r25 = MEM_H(ctx->r6, 0X0);
            goto L_80169A7C;
    }
    // 0x80169A3C: lh          $t9, 0x0($a2)
    ctx->r25 = MEM_H(ctx->r6, 0X0);
L_80169A40:
    // 0x80169A40: mtc1        $t9, $f18
    ctx->f18.u32l = ctx->r25;
    // 0x80169A44: lwc1        $f16, 0x14($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80169A48: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80169A4C: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80169A50: sltu        $at, $v1, $a0
    ctx->r1 = ctx->r3 < ctx->r4 ? 1 : 0;
    // 0x80169A54: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80169A58: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x80169A5C: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x80169A60: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x80169A64: swc1        $f18, -0x4($a3)
    MEM_W(-0X4, ctx->r7) = ctx->f18.u32l;
    // 0x80169A68: lwc1        $f16, 0x1C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X1C);
    // 0x80169A6C: add.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x80169A70: swc1        $f18, -0x8($v1)
    MEM_W(-0X8, ctx->r3) = ctx->f18.u32l;
    // 0x80169A74: bne         $at, $zero, L_80169A40
    if (ctx->r1 != 0) {
        // 0x80169A78: lh          $t9, 0x0($a2)
        ctx->r25 = MEM_H(ctx->r6, 0X0);
            goto L_80169A40;
    }
    // 0x80169A78: lh          $t9, 0x0($a2)
    ctx->r25 = MEM_H(ctx->r6, 0X0);
L_80169A7C:
    // 0x80169A7C: mtc1        $t9, $f18
    ctx->f18.u32l = ctx->r25;
    // 0x80169A80: lwc1        $f16, 0x14($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80169A84: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80169A88: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80169A8C: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x80169A90: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x80169A94: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x80169A98: swc1        $f18, -0x4($a3)
    MEM_W(-0X4, ctx->r7) = ctx->f18.u32l;
    // 0x80169A9C: lwc1        $f16, 0x1C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X1C);
    // 0x80169AA0: add.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x80169AA4: swc1        $f18, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f18.u32l;
    // 0x80169AA8: lw          $t1, 0x60($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X60);
    // 0x80169AAC: addiu       $a0, $s1, 0x1E4
    ctx->r4 = ADD32(ctx->r17, 0X1E4);
    // 0x80169AB0: jal         0x801739B8
    // 0x80169AB4: lh          $a1, 0x8($t1)
    ctx->r5 = MEM_H(ctx->r9, 0X8);
    func_801739B8(rdram, ctx);
        goto after_0;
    // 0x80169AB4: lh          $a1, 0x8($t1)
    ctx->r5 = MEM_H(ctx->r9, 0X8);
    after_0:
    // 0x80169AB8: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x80169ABC: addiu       $a0, $s1, 0x208
    ctx->r4 = ADD32(ctx->r17, 0X208);
    // 0x80169AC0: jal         0x80173BC0
    // 0x80169AC4: addiu       $a1, $s1, 0x1E4
    ctx->r5 = ADD32(ctx->r17, 0X1E4);
    func_80173BC0(rdram, ctx);
        goto after_1;
    // 0x80169AC4: addiu       $a1, $s1, 0x1E4
    ctx->r5 = ADD32(ctx->r17, 0X1E4);
    after_1:
    // 0x80169AC8: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x80169ACC: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    // 0x80169AD0: addiu       $a0, $s1, 0x208
    ctx->r4 = ADD32(ctx->r17, 0X208);
    // 0x80169AD4: jal         0x80173524
    // 0x80169AD8: addiu       $a2, $s1, 0x38
    ctx->r6 = ADD32(ctx->r17, 0X38);
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x80169AD8: addiu       $a2, $s1, 0x38
    ctx->r6 = ADD32(ctx->r17, 0X38);
    after_2:
    // 0x80169ADC: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x80169AE0: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    // 0x80169AE4: addiu       $a0, $s1, 0x208
    ctx->r4 = ADD32(ctx->r17, 0X208);
    // 0x80169AE8: jal         0x80173524
    // 0x80169AEC: addiu       $a2, $s1, 0x50
    ctx->r6 = ADD32(ctx->r17, 0X50);
    func_80173524(rdram, ctx);
        goto after_3;
    // 0x80169AEC: addiu       $a2, $s1, 0x50
    ctx->r6 = ADD32(ctx->r17, 0X50);
    after_3:
    // 0x80169AF0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80169AF4: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    // 0x80169AF8: lw          $t2, 0x0($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X0);
L_80169AFC:
    // 0x80169AFC: addu        $v0, $t2, $v1
    ctx->r2 = ADD32(ctx->r10, ctx->r3);
    // 0x80169B00: lwc1        $f0, 0x38($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X38);
    // 0x80169B04: swc1        $f0, 0xB0($v0)
    MEM_W(0XB0, ctx->r2) = ctx->f0.u32l;
    // 0x80169B08: lw          $t3, 0x0($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X0);
    // 0x80169B0C: addu        $v0, $t3, $v1
    ctx->r2 = ADD32(ctx->r11, ctx->r3);
    // 0x80169B10: lwc1        $f18, 0x50($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80169B14: sub.s       $f6, $f18, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f18.fl - ctx->f0.fl;
    // 0x80169B18: swc1        $f6, 0x44($v0)
    MEM_W(0X44, ctx->r2) = ctx->f6.u32l;
    // 0x80169B1C: lw          $t4, 0x0($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X0);
    // 0x80169B20: addu        $v0, $t4, $v1
    ctx->r2 = ADD32(ctx->r12, ctx->r3);
    // 0x80169B24: lwc1        $f2, 0x38($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X38);
    // 0x80169B28: lwc1        $f12, 0x50($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80169B2C: c.lt.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl < ctx->f12.fl;
    // 0x80169B30: nop

    // 0x80169B34: bc1fl       L_80169B58
    if (!c1cs) {
        // 0x80169B38: swc1        $f12, 0x150($v0)
        MEM_W(0X150, ctx->r2) = ctx->f12.u32l;
            goto L_80169B58;
    }
    goto skip_0;
    // 0x80169B38: swc1        $f12, 0x150($v0)
    MEM_W(0X150, ctx->r2) = ctx->f12.u32l;
    skip_0:
    // 0x80169B3C: swc1        $f2, 0x150($v0)
    MEM_W(0X150, ctx->r2) = ctx->f2.u32l;
    // 0x80169B40: lw          $t5, 0x0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X0);
    // 0x80169B44: addu        $v0, $t5, $v1
    ctx->r2 = ADD32(ctx->r13, ctx->r3);
    // 0x80169B48: lwc1        $f4, 0x50($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80169B4C: b           L_80169B68
    // 0x80169B50: swc1        $f4, 0x15C($v0)
    MEM_W(0X15C, ctx->r2) = ctx->f4.u32l;
        goto L_80169B68;
    // 0x80169B50: swc1        $f4, 0x15C($v0)
    MEM_W(0X15C, ctx->r2) = ctx->f4.u32l;
    // 0x80169B54: swc1        $f12, 0x150($v0)
    MEM_W(0X150, ctx->r2) = ctx->f12.u32l;
L_80169B58:
    // 0x80169B58: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x80169B5C: addu        $v0, $t6, $v1
    ctx->r2 = ADD32(ctx->r14, ctx->r3);
    // 0x80169B60: lwc1        $f8, 0x38($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X38);
    // 0x80169B64: swc1        $f8, 0x15C($v0)
    MEM_W(0X15C, ctx->r2) = ctx->f8.u32l;
L_80169B68:
    // 0x80169B68: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80169B6C: bnel        $v1, $a0, L_80169AFC
    if (ctx->r3 != ctx->r4) {
        // 0x80169B70: lw          $t2, 0x0($s0)
        ctx->r10 = MEM_W(ctx->r16, 0X0);
            goto L_80169AFC;
    }
    goto skip_1;
    // 0x80169B70: lw          $t2, 0x0($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X0);
    skip_1:
    // 0x80169B74: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80169B78: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80169B7C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80169B80: jr          $ra
    // 0x80169B84: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    return;
    // 0x80169B84: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
;}
RECOMP_FUNC void func_80169B88(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80169B88: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80169B8C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80169B90: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x80169B94: addiu       $t1, $zero, 0x2
    ctx->r9 = ADD32(0, 0X2);
    // 0x80169B98: addiu       $t0, $zero, 0x3
    ctx->r8 = ADD32(0, 0X3);
L_80169B9C:
    // 0x80169B9C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80169BA0: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80169BA4: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
L_80169BA8:
    // 0x80169BA8: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80169BAC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80169BB0: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80169BB4: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x80169BB8: bne         $v1, $t0, L_80169BA8
    if (ctx->r3 != ctx->r8) {
        // 0x80169BBC: swc1        $f4, -0x4($a0)
        MEM_W(-0X4, ctx->r4) = ctx->f4.u32l;
            goto L_80169BA8;
    }
    // 0x80169BBC: swc1        $f4, -0x4($a0)
    MEM_W(-0X4, ctx->r4) = ctx->f4.u32l;
    // 0x80169BC0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80169BC4: addiu       $a2, $a2, 0xC
    ctx->r6 = ADD32(ctx->r6, 0XC);
    // 0x80169BC8: bne         $v0, $t1, L_80169B9C
    if (ctx->r2 != ctx->r9) {
        // 0x80169BCC: addiu       $a3, $a3, 0xC
        ctx->r7 = ADD32(ctx->r7, 0XC);
            goto L_80169B9C;
    }
    // 0x80169BCC: addiu       $a3, $a3, 0xC
    ctx->r7 = ADD32(ctx->r7, 0XC);
    // 0x80169BD0: jr          $ra
    // 0x80169BD4: nop

    return;
    // 0x80169BD4: nop

;}
RECOMP_FUNC void func_80169BD8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80169BD8: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x80169BDC: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80169BE0: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x80169BE4: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x80169BE8: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x80169BEC: sw          $a2, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r6;
    // 0x80169BF0: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80169BF4: lwc1        $f0, 0x0($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80169BF8: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80169BFC: or          $s2, $a1, $zero
    ctx->r18 = ctx->r5 | 0;
    // 0x80169C00: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x80169C04: nop

    // 0x80169C08: bc1fl       L_80169C1C
    if (!c1cs) {
        // 0x80169C0C: lwc1        $f6, 0xC($s1)
        ctx->f6.u32l = MEM_W(ctx->r17, 0XC);
            goto L_80169C1C;
    }
    goto skip_0;
    // 0x80169C0C: lwc1        $f6, 0xC($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0XC);
    skip_0:
    // 0x80169C10: b           L_80169C3C
    // 0x80169C14: sh          $zero, 0x48($sp)
    MEM_H(0X48, ctx->r29) = 0;
        goto L_80169C3C;
    // 0x80169C14: sh          $zero, 0x48($sp)
    MEM_H(0X48, ctx->r29) = 0;
    // 0x80169C18: lwc1        $f6, 0xC($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0XC);
L_80169C1C:
    // 0x80169C1C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80169C20: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x80169C24: nop

    // 0x80169C28: bc1f        L_80169C38
    if (!c1cs) {
        // 0x80169C2C: nop
    
            goto L_80169C38;
    }
    // 0x80169C2C: nop

    // 0x80169C30: b           L_80169C38
    // 0x80169C34: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169C38;
    // 0x80169C34: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169C38:
    // 0x80169C38: sh          $v0, 0x48($sp)
    MEM_H(0X48, ctx->r29) = ctx->r2;
L_80169C3C:
    // 0x80169C3C: lwc1        $f0, 0x4($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X4);
    // 0x80169C40: lwc1        $f8, 0x4($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80169C44: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x80169C48: nop

    // 0x80169C4C: bc1fl       L_80169C60
    if (!c1cs) {
        // 0x80169C50: lwc1        $f10, 0x10($s1)
        ctx->f10.u32l = MEM_W(ctx->r17, 0X10);
            goto L_80169C60;
    }
    goto skip_1;
    // 0x80169C50: lwc1        $f10, 0x10($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X10);
    skip_1:
    // 0x80169C54: b           L_80169C80
    // 0x80169C58: sh          $zero, 0x4A($sp)
    MEM_H(0X4A, ctx->r29) = 0;
        goto L_80169C80;
    // 0x80169C58: sh          $zero, 0x4A($sp)
    MEM_H(0X4A, ctx->r29) = 0;
    // 0x80169C5C: lwc1        $f10, 0x10($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X10);
L_80169C60:
    // 0x80169C60: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80169C64: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x80169C68: nop

    // 0x80169C6C: bc1f        L_80169C7C
    if (!c1cs) {
        // 0x80169C70: nop
    
            goto L_80169C7C;
    }
    // 0x80169C70: nop

    // 0x80169C74: b           L_80169C7C
    // 0x80169C78: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169C7C;
    // 0x80169C78: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169C7C:
    // 0x80169C7C: sh          $v0, 0x4A($sp)
    MEM_H(0X4A, ctx->r29) = ctx->r2;
L_80169C80:
    // 0x80169C80: lwc1        $f0, 0x8($s2)
    ctx->f0.u32l = MEM_W(ctx->r18, 0X8);
    // 0x80169C84: lwc1        $f16, 0x8($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X8);
    // 0x80169C88: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x80169C8C: nop

    // 0x80169C90: bc1fl       L_80169CA4
    if (!c1cs) {
        // 0x80169C94: lwc1        $f18, 0x14($s1)
        ctx->f18.u32l = MEM_W(ctx->r17, 0X14);
            goto L_80169CA4;
    }
    goto skip_2;
    // 0x80169C94: lwc1        $f18, 0x14($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X14);
    skip_2:
    // 0x80169C98: b           L_80169CC4
    // 0x80169C9C: sh          $zero, 0x4C($sp)
    MEM_H(0X4C, ctx->r29) = 0;
        goto L_80169CC4;
    // 0x80169C9C: sh          $zero, 0x4C($sp)
    MEM_H(0X4C, ctx->r29) = 0;
    // 0x80169CA0: lwc1        $f18, 0x14($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X14);
L_80169CA4:
    // 0x80169CA4: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x80169CA8: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x80169CAC: nop

    // 0x80169CB0: bc1f        L_80169CC0
    if (!c1cs) {
        // 0x80169CB4: nop
    
            goto L_80169CC0;
    }
    // 0x80169CB4: nop

    // 0x80169CB8: b           L_80169CC0
    // 0x80169CBC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169CC0;
    // 0x80169CBC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169CC0:
    // 0x80169CC0: sh          $v0, 0x4C($sp)
    MEM_H(0X4C, ctx->r29) = ctx->r2;
L_80169CC4:
    // 0x80169CC4: lh          $t6, 0x48($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X48);
    // 0x80169CC8: addiu       $t0, $zero, 0x2
    ctx->r8 = ADD32(0, 0X2);
    // 0x80169CCC: lh          $t7, 0x4A($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X4A);
    // 0x80169CD0: bne         $t0, $t6, L_80169CF0
    if (ctx->r8 != ctx->r14) {
        // 0x80169CD4: lw          $t9, 0x78($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X78);
            goto L_80169CF0;
    }
    // 0x80169CD4: lw          $t9, 0x78($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X78);
    // 0x80169CD8: bne         $t0, $t7, L_80169CF0
    if (ctx->r8 != ctx->r15) {
        // 0x80169CDC: lh          $t8, 0x4C($sp)
        ctx->r24 = MEM_H(ctx->r29, 0X4C);
            goto L_80169CF0;
    }
    // 0x80169CDC: lh          $t8, 0x4C($sp)
    ctx->r24 = MEM_H(ctx->r29, 0X4C);
    // 0x80169CE0: bnel        $t0, $t8, L_80169CF4
    if (ctx->r8 != ctx->r24) {
        // 0x80169CE4: lwc1        $f4, 0x0($t9)
        ctx->f4.u32l = MEM_W(ctx->r25, 0X0);
            goto L_80169CF4;
    }
    goto skip_3;
    // 0x80169CE4: lwc1        $f4, 0x0($t9)
    ctx->f4.u32l = MEM_W(ctx->r25, 0X0);
    skip_3:
    // 0x80169CE8: b           L_80169EB4
    // 0x80169CEC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169EB4;
    // 0x80169CEC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169CF0:
    // 0x80169CF0: lwc1        $f4, 0x0($t9)
    ctx->f4.u32l = MEM_W(ctx->r25, 0X0);
L_80169CF4:
    // 0x80169CF4: lwc1        $f6, 0x0($s2)
    ctx->f6.u32l = MEM_W(ctx->r18, 0X0);
    // 0x80169CF8: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80169CFC: swc1        $f8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f8.u32l;
    // 0x80169D00: lwc1        $f16, 0x4($s2)
    ctx->f16.u32l = MEM_W(ctx->r18, 0X4);
    // 0x80169D04: lwc1        $f10, 0x4($t9)
    ctx->f10.u32l = MEM_W(ctx->r25, 0X4);
    // 0x80169D08: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x80169D0C: swc1        $f18, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f18.u32l;
    // 0x80169D10: lwc1        $f6, 0x8($s2)
    ctx->f6.u32l = MEM_W(ctx->r18, 0X8);
    // 0x80169D14: lwc1        $f4, 0x8($t9)
    ctx->f4.u32l = MEM_W(ctx->r25, 0X8);
    // 0x80169D18: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80169D1C: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80169D20: swc1        $f10, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f10.u32l;
    // 0x80169D24: lwc1        $f16, 0x0($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X0);
    // 0x80169D28: c.lt.s      $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f8.fl < ctx->f16.fl;
    // 0x80169D2C: nop

    // 0x80169D30: bc1fl       L_80169D44
    if (!c1cs) {
        // 0x80169D34: lwc1        $f18, 0xC($s1)
        ctx->f18.u32l = MEM_W(ctx->r17, 0XC);
            goto L_80169D44;
    }
    goto skip_4;
    // 0x80169D34: lwc1        $f18, 0xC($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0XC);
    skip_4:
    // 0x80169D38: b           L_80169D64
    // 0x80169D3C: sh          $zero, 0x40($sp)
    MEM_H(0X40, ctx->r29) = 0;
        goto L_80169D64;
    // 0x80169D3C: sh          $zero, 0x40($sp)
    MEM_H(0X40, ctx->r29) = 0;
    // 0x80169D40: lwc1        $f18, 0xC($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0XC);
L_80169D44:
    // 0x80169D44: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80169D48: c.lt.s      $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f18.fl < ctx->f4.fl;
    // 0x80169D4C: nop

    // 0x80169D50: bc1f        L_80169D60
    if (!c1cs) {
        // 0x80169D54: nop
    
            goto L_80169D60;
    }
    // 0x80169D54: nop

    // 0x80169D58: b           L_80169D60
    // 0x80169D5C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169D60;
    // 0x80169D5C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169D60:
    // 0x80169D60: sh          $v0, 0x40($sp)
    MEM_H(0X40, ctx->r29) = ctx->r2;
L_80169D64:
    // 0x80169D64: lwc1        $f6, 0x60($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X60);
    // 0x80169D68: lwc1        $f10, 0x4($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80169D6C: lwc1        $f16, 0x60($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X60);
    // 0x80169D70: addiu       $v1, $sp, 0x48
    ctx->r3 = ADD32(ctx->r29, 0X48);
    // 0x80169D74: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x80169D78: addiu       $a0, $sp, 0x46
    ctx->r4 = ADD32(ctx->r29, 0X46);
    // 0x80169D7C: bc1fl       L_80169D90
    if (!c1cs) {
        // 0x80169D80: lwc1        $f8, 0x10($s1)
        ctx->f8.u32l = MEM_W(ctx->r17, 0X10);
            goto L_80169D90;
    }
    goto skip_5;
    // 0x80169D80: lwc1        $f8, 0x10($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X10);
    skip_5:
    // 0x80169D84: b           L_80169DB0
    // 0x80169D88: sh          $zero, 0x42($sp)
    MEM_H(0X42, ctx->r29) = 0;
        goto L_80169DB0;
    // 0x80169D88: sh          $zero, 0x42($sp)
    MEM_H(0X42, ctx->r29) = 0;
    // 0x80169D8C: lwc1        $f8, 0x10($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X10);
L_80169D90:
    // 0x80169D90: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80169D94: c.lt.s      $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f8.fl < ctx->f16.fl;
    // 0x80169D98: nop

    // 0x80169D9C: bc1f        L_80169DAC
    if (!c1cs) {
        // 0x80169DA0: nop
    
            goto L_80169DAC;
    }
    // 0x80169DA0: nop

    // 0x80169DA4: b           L_80169DAC
    // 0x80169DA8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169DAC;
    // 0x80169DA8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169DAC:
    // 0x80169DAC: sh          $v0, 0x42($sp)
    MEM_H(0X42, ctx->r29) = ctx->r2;
L_80169DB0:
    // 0x80169DB0: lwc1        $f18, 0x64($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X64);
    // 0x80169DB4: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x80169DB8: lwc1        $f10, 0x64($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X64);
    // 0x80169DBC: c.lt.s      $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f18.fl < ctx->f4.fl;
    // 0x80169DC0: nop

    // 0x80169DC4: bc1fl       L_80169DD8
    if (!c1cs) {
        // 0x80169DC8: lwc1        $f6, 0x14($s1)
        ctx->f6.u32l = MEM_W(ctx->r17, 0X14);
            goto L_80169DD8;
    }
    goto skip_6;
    // 0x80169DC8: lwc1        $f6, 0x14($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X14);
    skip_6:
    // 0x80169DCC: b           L_80169DF8
    // 0x80169DD0: sh          $zero, 0x44($sp)
    MEM_H(0X44, ctx->r29) = 0;
        goto L_80169DF8;
    // 0x80169DD0: sh          $zero, 0x44($sp)
    MEM_H(0X44, ctx->r29) = 0;
    // 0x80169DD4: lwc1        $f6, 0x14($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X14);
L_80169DD8:
    // 0x80169DD8: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80169DDC: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x80169DE0: nop

    // 0x80169DE4: bc1f        L_80169DF4
    if (!c1cs) {
        // 0x80169DE8: nop
    
            goto L_80169DF4;
    }
    // 0x80169DE8: nop

    // 0x80169DEC: b           L_80169DF4
    // 0x80169DF0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169DF4;
    // 0x80169DF0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169DF4:
    // 0x80169DF4: sh          $v0, 0x44($sp)
    MEM_H(0X44, ctx->r29) = ctx->r2;
L_80169DF8:
    // 0x80169DF8: lh          $t1, 0x40($sp)
    ctx->r9 = MEM_H(ctx->r29, 0X40);
    // 0x80169DFC: lh          $t2, 0x42($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X42);
    // 0x80169E00: addiu       $v0, $sp, 0x40
    ctx->r2 = ADD32(ctx->r29, 0X40);
    // 0x80169E04: bne         $t0, $t1, L_80169E24
    if (ctx->r8 != ctx->r9) {
        // 0x80169E08: nop
    
            goto L_80169E24;
    }
    // 0x80169E08: nop

    // 0x80169E0C: bne         $t0, $t2, L_80169E24
    if (ctx->r8 != ctx->r10) {
        // 0x80169E10: lh          $t3, 0x44($sp)
        ctx->r11 = MEM_H(ctx->r29, 0X44);
            goto L_80169E24;
    }
    // 0x80169E10: lh          $t3, 0x44($sp)
    ctx->r11 = MEM_H(ctx->r29, 0X44);
    // 0x80169E14: bne         $t0, $t3, L_80169E24
    if (ctx->r8 != ctx->r11) {
        // 0x80169E18: nop
    
            goto L_80169E24;
    }
    // 0x80169E18: nop

    // 0x80169E1C: b           L_80169EB4
    // 0x80169E20: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169EB4;
    // 0x80169E20: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169E24:
    // 0x80169E24: lh          $a1, 0x0($v1)
    ctx->r5 = MEM_H(ctx->r3, 0X0);
    // 0x80169E28: lh          $t4, 0x0($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X0);
    // 0x80169E2C: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80169E30: slti        $at, $a1, 0x2
    ctx->r1 = SIGNED(ctx->r5) < 0X2 ? 1 : 0;
    // 0x80169E34: bnel        $a1, $t4, L_80169E50
    if (ctx->r5 != ctx->r12) {
        // 0x80169E38: sltu        $at, $v0, $a0
        ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
            goto L_80169E50;
    }
    goto skip_7;
    // 0x80169E38: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    skip_7:
    // 0x80169E3C: beql        $at, $zero, L_80169E50
    if (ctx->r1 == 0) {
        // 0x80169E40: sltu        $at, $v0, $a0
        ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
            goto L_80169E50;
    }
    goto skip_8;
    // 0x80169E40: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    skip_8:
    // 0x80169E44: b           L_80169EB4
    // 0x80169E48: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80169EB4;
    // 0x80169E48: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80169E4C: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
L_80169E50:
    // 0x80169E50: bne         $at, $zero, L_80169E24
    if (ctx->r1 != 0) {
        // 0x80169E54: addiu       $v1, $v1, 0x2
        ctx->r3 = ADD32(ctx->r3, 0X2);
            goto L_80169E24;
    }
    // 0x80169E54: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80169E58: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80169E5C: addiu       $v1, $sp, 0x48
    ctx->r3 = ADD32(ctx->r29, 0X48);
L_80169E60:
    // 0x80169E60: lh          $a1, 0x0($v1)
    ctx->r5 = MEM_H(ctx->r3, 0X0);
    // 0x80169E64: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80169E68: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80169E6C: beq         $t0, $a1, L_80169EA0
    if (ctx->r8 == ctx->r5) {
        // 0x80169E70: or          $a3, $s2, $zero
        ctx->r7 = ctx->r18 | 0;
            goto L_80169EA0;
    }
    // 0x80169E70: or          $a3, $s2, $zero
    ctx->r7 = ctx->r18 | 0;
    // 0x80169E74: lw          $t5, 0x78($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X78);
    // 0x80169E78: addiu       $t6, $sp, 0x50
    ctx->r14 = ADD32(ctx->r29, 0X50);
    // 0x80169E7C: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x80169E80: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x80169E84: jal         0x80169ECC
    // 0x80169E88: sw          $t5, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r13;
    func_80169ECC(rdram, ctx);
        goto after_0;
    // 0x80169E88: sw          $t5, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r13;
    after_0:
    // 0x80169E8C: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x80169E90: bne         $v0, $zero, L_80169EA0
    if (ctx->r2 != 0) {
        // 0x80169E94: addiu       $t0, $zero, 0x2
        ctx->r8 = ADD32(0, 0X2);
            goto L_80169EA0;
    }
    // 0x80169E94: addiu       $t0, $zero, 0x2
    ctx->r8 = ADD32(0, 0X2);
    // 0x80169E98: b           L_80169EB4
    // 0x80169E9C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80169EB4;
    // 0x80169E9C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80169EA0:
    // 0x80169EA0: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80169EA4: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x80169EA8: bne         $s0, $at, L_80169E60
    if (ctx->r16 != ctx->r1) {
        // 0x80169EAC: addiu       $v1, $v1, 0x2
        ctx->r3 = ADD32(ctx->r3, 0X2);
            goto L_80169E60;
    }
    // 0x80169EAC: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80169EB0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80169EB4:
    // 0x80169EB4: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80169EB8: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80169EBC: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80169EC0: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x80169EC4: jr          $ra
    // 0x80169EC8: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x80169EC8: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void func_80169ECC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80169ECC: lw          $t6, 0x10($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X10);
    // 0x80169ED0: sll         $v0, $a0, 2
    ctx->r2 = S32(ctx->r4 << 2);
    // 0x80169ED4: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80169ED8: addu        $t7, $t6, $v0
    ctx->r15 = ADD32(ctx->r14, ctx->r2);
    // 0x80169EDC: lwc1        $f0, 0x0($t7)
    ctx->f0.u32l = MEM_W(ctx->r15, 0X0);
    // 0x80169EE0: sll         $t8, $a1, 2
    ctx->r24 = S32(ctx->r5 << 2);
    // 0x80169EE4: subu        $t8, $t8, $a1
    ctx->r24 = SUB32(ctx->r24, ctx->r5);
    // 0x80169EE8: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x80169EEC: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80169EF0: addu        $t9, $a2, $t8
    ctx->r25 = ADD32(ctx->r6, ctx->r24);
    // 0x80169EF4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80169EF8: bc1f        L_80169F08
    if (!c1cs) {
        // 0x80169EFC: addu        $t0, $t9, $v0
        ctx->r8 = ADD32(ctx->r25, ctx->r2);
            goto L_80169F08;
    }
    // 0x80169EFC: addu        $t0, $t9, $v0
    ctx->r8 = ADD32(ctx->r25, ctx->r2);
    // 0x80169F00: b           L_80169F0C
    // 0x80169F04: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_80169F0C;
    // 0x80169F04: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_80169F08:
    // 0x80169F08: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_80169F0C:
    // 0x80169F0C: lwc1        $f6, -0x5194($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X5194);
    // 0x80169F10: addu        $t1, $a3, $v0
    ctx->r9 = ADD32(ctx->r7, ctx->r2);
    // 0x80169F14: c.lt.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl < ctx->f6.fl;
    // 0x80169F18: nop

    // 0x80169F1C: bc1fl       L_80169F30
    if (!c1cs) {
        // 0x80169F20: lwc1        $f12, 0x0($t0)
        ctx->f12.u32l = MEM_W(ctx->r8, 0X0);
            goto L_80169F30;
    }
    goto skip_0;
    // 0x80169F20: lwc1        $f12, 0x0($t0)
    ctx->f12.u32l = MEM_W(ctx->r8, 0X0);
    skip_0:
    // 0x80169F24: jr          $ra
    // 0x80169F28: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    return;
    // 0x80169F28: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x80169F2C: lwc1        $f12, 0x0($t0)
    ctx->f12.u32l = MEM_W(ctx->r8, 0X0);
L_80169F30:
    // 0x80169F30: lwc1        $f8, 0x0($t1)
    ctx->f8.u32l = MEM_W(ctx->r9, 0X0);
    // 0x80169F34: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80169F38: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80169F3C: sub.s       $f10, $f12, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f12.fl - ctx->f8.fl;
    // 0x80169F40: div.s       $f2, $f10, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80169F44: c.lt.s      $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f2.fl < ctx->f16.fl;
    // 0x80169F48: nop

    // 0x80169F4C: bc1t        L_80169F6C
    if (c1cs) {
        // 0x80169F50: nop
    
            goto L_80169F6C;
    }
    // 0x80169F50: nop

    // 0x80169F54: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x80169F58: lw          $v1, 0x14($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X14);
    // 0x80169F5C: c.lt.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl < ctx->f2.fl;
    // 0x80169F60: addu        $t2, $v1, $v0
    ctx->r10 = ADD32(ctx->r3, ctx->r2);
    // 0x80169F64: bc1f        L_80169F74
    if (!c1cs) {
        // 0x80169F68: nop
    
            goto L_80169F74;
    }
    // 0x80169F68: nop

L_80169F6C:
    // 0x80169F6C: jr          $ra
    // 0x80169F70: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    return;
    // 0x80169F70: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
L_80169F74:
    // 0x80169F74: bne         $v0, $zero, L_8016A018
    if (ctx->r2 != 0) {
        // 0x80169F78: swc1        $f12, 0x0($t2)
        MEM_W(0X0, ctx->r10) = ctx->f12.u32l;
            goto L_8016A018;
    }
    // 0x80169F78: swc1        $f12, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f12.u32l;
    // 0x80169F7C: lw          $t3, 0x10($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X10);
    // 0x80169F80: lwc1        $f8, 0x4($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X4);
    // 0x80169F84: lwc1        $f4, 0x4($t3)
    ctx->f4.u32l = MEM_W(ctx->r11, 0X4);
    // 0x80169F88: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80169F8C: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80169F90: swc1        $f10, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f10.u32l;
    // 0x80169F94: lw          $t4, 0x10($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X10);
    // 0x80169F98: lwc1        $f4, 0x8($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X8);
    // 0x80169F9C: lwc1        $f0, 0x4($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X4);
    // 0x80169FA0: lwc1        $f16, 0x8($t4)
    ctx->f16.u32l = MEM_W(ctx->r12, 0X8);
    // 0x80169FA4: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x80169FA8: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x80169FAC: swc1        $f6, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->f6.u32l;
    // 0x80169FB0: lwc1        $f8, 0x4($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X4);
    // 0x80169FB4: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x80169FB8: nop

    // 0x80169FBC: bc1t        L_8016A008
    if (c1cs) {
        // 0x80169FC0: nop
    
            goto L_8016A008;
    }
    // 0x80169FC0: nop

    // 0x80169FC4: lwc1        $f10, 0x10($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X10);
    // 0x80169FC8: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x80169FCC: nop

    // 0x80169FD0: bc1t        L_8016A008
    if (c1cs) {
        // 0x80169FD4: nop
    
            goto L_8016A008;
    }
    // 0x80169FD4: nop

    // 0x80169FD8: lwc1        $f0, 0x8($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X8);
    // 0x80169FDC: lwc1        $f16, 0x8($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X8);
    // 0x80169FE0: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x80169FE4: nop

    // 0x80169FE8: bc1t        L_8016A008
    if (c1cs) {
        // 0x80169FEC: nop
    
            goto L_8016A008;
    }
    // 0x80169FEC: nop

    // 0x80169FF0: lwc1        $f18, 0x14($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X14);
    // 0x80169FF4: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80169FF8: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x80169FFC: nop

    // 0x8016A000: bc1f        L_8016A010
    if (!c1cs) {
        // 0x8016A004: nop
    
            goto L_8016A010;
    }
    // 0x8016A004: nop

L_8016A008:
    // 0x8016A008: jr          $ra
    // 0x8016A00C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x8016A00C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8016A010:
    // 0x8016A010: jr          $ra
    // 0x8016A014: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x8016A014: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8016A018:
    // 0x8016A018: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x8016A01C: bne         $v0, $at, L_8016A0C0
    if (ctx->r2 != ctx->r1) {
        // 0x8016A020: lw          $t7, 0x10($sp)
        ctx->r15 = MEM_W(ctx->r29, 0X10);
            goto L_8016A0C0;
    }
    // 0x8016A020: lw          $t7, 0x10($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X10);
    // 0x8016A024: lw          $t5, 0x10($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X10);
    // 0x8016A028: lwc1        $f8, 0x0($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016A02C: lwc1        $f4, 0x0($t5)
    ctx->f4.u32l = MEM_W(ctx->r13, 0X0);
    // 0x8016A030: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016A034: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8016A038: swc1        $f10, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f10.u32l;
    // 0x8016A03C: lw          $t6, 0x10($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X10);
    // 0x8016A040: lwc1        $f4, 0x8($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X8);
    // 0x8016A044: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016A048: lwc1        $f16, 0x8($t6)
    ctx->f16.u32l = MEM_W(ctx->r14, 0X8);
    // 0x8016A04C: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x8016A050: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x8016A054: swc1        $f6, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->f6.u32l;
    // 0x8016A058: lwc1        $f8, 0x0($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X0);
    // 0x8016A05C: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x8016A060: nop

    // 0x8016A064: bc1t        L_8016A0B0
    if (c1cs) {
        // 0x8016A068: nop
    
            goto L_8016A0B0;
    }
    // 0x8016A068: nop

    // 0x8016A06C: lwc1        $f10, 0xC($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0XC);
    // 0x8016A070: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x8016A074: nop

    // 0x8016A078: bc1t        L_8016A0B0
    if (c1cs) {
        // 0x8016A07C: nop
    
            goto L_8016A0B0;
    }
    // 0x8016A07C: nop

    // 0x8016A080: lwc1        $f0, 0x8($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X8);
    // 0x8016A084: lwc1        $f16, 0x8($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X8);
    // 0x8016A088: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x8016A08C: nop

    // 0x8016A090: bc1t        L_8016A0B0
    if (c1cs) {
        // 0x8016A094: nop
    
            goto L_8016A0B0;
    }
    // 0x8016A094: nop

    // 0x8016A098: lwc1        $f18, 0x14($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X14);
    // 0x8016A09C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016A0A0: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x8016A0A4: nop

    // 0x8016A0A8: bc1f        L_8016A0B8
    if (!c1cs) {
        // 0x8016A0AC: nop
    
            goto L_8016A0B8;
    }
    // 0x8016A0AC: nop

L_8016A0B0:
    // 0x8016A0B0: jr          $ra
    // 0x8016A0B4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x8016A0B4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8016A0B8:
    // 0x8016A0B8: jr          $ra
    // 0x8016A0BC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x8016A0BC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8016A0C0:
    // 0x8016A0C0: lwc1        $f4, 0x0($t7)
    ctx->f4.u32l = MEM_W(ctx->r15, 0X0);
    // 0x8016A0C4: lwc1        $f8, 0x0($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016A0C8: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016A0CC: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8016A0D0: swc1        $f10, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f10.u32l;
    // 0x8016A0D4: lw          $t8, 0x10($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X10);
    // 0x8016A0D8: lwc1        $f4, 0x4($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X4);
    // 0x8016A0DC: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016A0E0: lwc1        $f16, 0x4($t8)
    ctx->f16.u32l = MEM_W(ctx->r24, 0X4);
    // 0x8016A0E4: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x8016A0E8: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x8016A0EC: swc1        $f6, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f6.u32l;
    // 0x8016A0F0: lwc1        $f8, 0x0($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X0);
    // 0x8016A0F4: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x8016A0F8: nop

    // 0x8016A0FC: bc1t        L_8016A148
    if (c1cs) {
        // 0x8016A100: nop
    
            goto L_8016A148;
    }
    // 0x8016A100: nop

    // 0x8016A104: lwc1        $f10, 0xC($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0XC);
    // 0x8016A108: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x8016A10C: nop

    // 0x8016A110: bc1t        L_8016A148
    if (c1cs) {
        // 0x8016A114: nop
    
            goto L_8016A148;
    }
    // 0x8016A114: nop

    // 0x8016A118: lwc1        $f0, 0x4($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8016A11C: lwc1        $f16, 0x4($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8016A120: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x8016A124: nop

    // 0x8016A128: bc1t        L_8016A148
    if (c1cs) {
        // 0x8016A12C: nop
    
            goto L_8016A148;
    }
    // 0x8016A12C: nop

    // 0x8016A130: lwc1        $f18, 0x10($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X10);
    // 0x8016A134: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016A138: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x8016A13C: nop

    // 0x8016A140: bc1f        L_8016A150
    if (!c1cs) {
        // 0x8016A144: nop
    
            goto L_8016A150;
    }
    // 0x8016A144: nop

L_8016A148:
    // 0x8016A148: b           L_8016A150
    // 0x8016A14C: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016A150;
    // 0x8016A14C: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016A150:
    // 0x8016A150: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x8016A154: jr          $ra
    // 0x8016A158: nop

    return;
    // 0x8016A158: nop

;}
RECOMP_FUNC void func_8016A15C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016A15C: addiu       $sp, $sp, -0x78
    ctx->r29 = ADD32(ctx->r29, -0X78);
    // 0x8016A160: sw          $a3, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r7;
    // 0x8016A164: lwc1        $f4, 0x0($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X0);
    // 0x8016A168: lwc1        $f14, 0x0($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016A16C: lwc1        $f8, 0x4($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8016A170: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016A174: mul.s       $f6, $f14, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f4.fl);
    // 0x8016A178: lwc1        $f12, 0x8($a0)
    ctx->f12.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016A17C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016A180: mul.s       $f10, $f16, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x8016A184: lwc1        $f8, 0x8($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X8);
    // 0x8016A188: add.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016A18C: mul.s       $f6, $f8, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f12.fl);
    // 0x8016A190: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8016A194: add.s       $f2, $f6, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x8016A198: c.lt.s      $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f2.fl < ctx->f10.fl;
    // 0x8016A19C: nop

    // 0x8016A1A0: bc1fl       L_8016A1B4
    if (!c1cs) {
        // 0x8016A1A4: mov.s       $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    ctx->f18.fl = ctx->f2.fl;
            goto L_8016A1B4;
    }
    goto skip_0;
    // 0x8016A1A4: mov.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    ctx->f18.fl = ctx->f2.fl;
    skip_0:
    // 0x8016A1A8: b           L_8016A1B4
    // 0x8016A1AC: neg.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = -ctx->f2.fl;
        goto L_8016A1B4;
    // 0x8016A1AC: neg.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = -ctx->f2.fl;
    // 0x8016A1B0: mov.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    ctx->f18.fl = ctx->f2.fl;
L_8016A1B4:
    // 0x8016A1B4: lwc1        $f8, -0x5190($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X5190);
    // 0x8016A1B8: c.lt.s      $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f18.fl < ctx->f8.fl;
    // 0x8016A1BC: nop

    // 0x8016A1C0: bc1fl       L_8016A1D4
    if (!c1cs) {
        // 0x8016A1C4: lwc1        $f6, 0x0($a1)
        ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
            goto L_8016A1D4;
    }
    goto skip_1;
    // 0x8016A1C4: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    skip_1:
    // 0x8016A1C8: b           L_8016A5B0
    // 0x8016A1CC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016A5B0;
    // 0x8016A1CC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016A1D0: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
L_8016A1D4:
    // 0x8016A1D4: lwc1        $f10, 0x4($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8016A1D8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A1DC: mul.s       $f4, $f14, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x8016A1E0: nop

    // 0x8016A1E4: mul.s       $f8, $f16, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f10.fl);
    // 0x8016A1E8: lwc1        $f10, 0x8($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016A1EC: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x8016A1F0: mul.s       $f4, $f12, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f10.fl);
    // 0x8016A1F4: lwc1        $f10, 0xC($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8016A1F8: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x8016A1FC: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x8016A200: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8016A204: neg.s       $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = -ctx->f6.fl;
    // 0x8016A208: div.s       $f0, $f4, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016A20C: c.lt.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl < ctx->f10.fl;
    // 0x8016A210: nop

    // 0x8016A214: bc1t        L_8016A240
    if (c1cs) {
        // 0x8016A218: nop
    
            goto L_8016A240;
    }
    // 0x8016A218: nop

    // 0x8016A21C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016A220: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x8016A224: or          $a3, $a2, $zero
    ctx->r7 = ctx->r6 | 0;
    // 0x8016A228: c.lt.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl < ctx->f0.fl;
    // 0x8016A22C: lw          $t0, 0x84($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X84);
    // 0x8016A230: or          $t1, $a0, $zero
    ctx->r9 = ctx->r4 | 0;
    // 0x8016A234: addiu       $t2, $sp, 0x4C
    ctx->r10 = ADD32(ctx->r29, 0X4C);
    // 0x8016A238: bc1f        L_8016A248
    if (!c1cs) {
        // 0x8016A23C: addiu       $t3, $sp, 0x58
        ctx->r11 = ADD32(ctx->r29, 0X58);
            goto L_8016A248;
    }
    // 0x8016A23C: addiu       $t3, $sp, 0x58
    ctx->r11 = ADD32(ctx->r29, 0X58);
L_8016A240:
    // 0x8016A240: b           L_8016A5B0
    // 0x8016A244: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
        goto L_8016A5B0;
    // 0x8016A244: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
L_8016A248:
    // 0x8016A248: addiu       $t4, $sp, 0x40
    ctx->r12 = ADD32(ctx->r29, 0X40);
    // 0x8016A24C: addiu       $t5, $sp, 0x34
    ctx->r13 = ADD32(ctx->r29, 0X34);
    // 0x8016A250: addiu       $v0, $sp, 0x40
    ctx->r2 = ADD32(ctx->r29, 0X40);
    // 0x8016A254: lwc1        $f12, 0x0($a3)
    ctx->f12.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016A258: addiu       $t5, $t5, 0x4
    ctx->r13 = ADD32(ctx->r13, 0X4);
    // 0x8016A25C: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016A260: mul.s       $f16, $f0, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x8016A264: beql        $t5, $v0, L_8016A2F4
    if (ctx->r13 == ctx->r2) {
        // 0x8016A268: add.s       $f14, $f18, $f16
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_8016A2F4;
    }
    goto skip_2;
    // 0x8016A268: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
    skip_2:
    // 0x8016A26C: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
L_8016A270:
    // 0x8016A270: addiu       $t5, $t5, 0x4
    ctx->r13 = ADD32(ctx->r13, 0X4);
    // 0x8016A274: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016A278: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016A27C: swc1        $f14, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->f14.u32l;
    // 0x8016A280: lh          $t8, 0x10($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X10);
    // 0x8016A284: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016A288: addiu       $t1, $t1, 0x2
    ctx->r9 = ADD32(ctx->r9, 0X2);
    // 0x8016A28C: mtc1        $t8, $f12
    ctx->f12.u32l = ctx->r24;
    // 0x8016A290: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016A294: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016A298: cvt.s.w     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    ctx->f12.fl = CVT_S_W(ctx->f12.u32l);
    // 0x8016A29C: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016A2A0: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A2A4: swc1        $f12, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f12.u32l;
    // 0x8016A2A8: swc1        $f14, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f14.u32l;
    // 0x8016A2AC: lh          $t9, 0x14($t1)
    ctx->r25 = MEM_H(ctx->r9, 0X14);
    // 0x8016A2B0: mtc1        $t9, $f14
    ctx->f14.u32l = ctx->r25;
    // 0x8016A2B4: nop

    // 0x8016A2B8: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A2BC: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A2C0: swc1        $f14, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f14.u32l;
    // 0x8016A2C4: lh          $t6, 0x1A($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X1A);
    // 0x8016A2C8: mtc1        $t6, $f14
    ctx->f14.u32l = ctx->r14;
    // 0x8016A2CC: nop

    // 0x8016A2D0: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A2D4: sub.s       $f12, $f14, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A2D8: swc1        $f12, -0x8($t5)
    MEM_W(-0X8, ctx->r13) = ctx->f12.u32l;
    // 0x8016A2DC: lwc1        $f12, 0x0($a3)
    ctx->f12.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016A2E0: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016A2E4: mul.s       $f16, $f0, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x8016A2E8: bnel        $t5, $v0, L_8016A270
    if (ctx->r13 != ctx->r2) {
        // 0x8016A2EC: add.s       $f14, $f18, $f16
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_8016A270;
    }
    goto skip_3;
    // 0x8016A2EC: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
    skip_3:
    // 0x8016A2F0: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
L_8016A2F4:
    // 0x8016A2F4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016A2F8: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016A2FC: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016A300: swc1        $f14, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->f14.u32l;
    // 0x8016A304: lh          $t8, 0x10($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X10);
    // 0x8016A308: addiu       $t1, $t1, 0x2
    ctx->r9 = ADD32(ctx->r9, 0X2);
    // 0x8016A30C: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016A310: mtc1        $t8, $f12
    ctx->f12.u32l = ctx->r24;
    // 0x8016A314: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016A318: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016A31C: cvt.s.w     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    ctx->f12.fl = CVT_S_W(ctx->f12.u32l);
    // 0x8016A320: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A324: swc1        $f12, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f12.u32l;
    // 0x8016A328: swc1        $f14, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f14.u32l;
    // 0x8016A32C: lh          $t9, 0x14($t1)
    ctx->r25 = MEM_H(ctx->r9, 0X14);
    // 0x8016A330: mtc1        $t9, $f14
    ctx->f14.u32l = ctx->r25;
    // 0x8016A334: nop

    // 0x8016A338: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A33C: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A340: swc1        $f14, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f14.u32l;
    // 0x8016A344: lh          $t6, 0x1A($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X1A);
    // 0x8016A348: mtc1        $t6, $f14
    ctx->f14.u32l = ctx->r14;
    // 0x8016A34C: nop

    // 0x8016A350: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A354: sub.s       $f12, $f14, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A358: swc1        $f12, -0x4($t5)
    MEM_W(-0X4, ctx->r13) = ctx->f12.u32l;
    // 0x8016A35C: lwc1        $f12, 0x58($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X58);
    // 0x8016A360: lwc1        $f14, 0x5C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8016A364: lwc1        $f2, 0x60($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X60);
    // 0x8016A368: mul.s       $f8, $f12, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x8016A36C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016A370: mul.s       $f4, $f14, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8016A374: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x8016A378: mul.s       $f10, $f2, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x8016A37C: lwc1        $f4, -0x518C($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X518C);
    // 0x8016A380: add.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8016A384: lwc1        $f10, 0x40($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016A388: c.lt.s      $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f8.fl < ctx->f4.fl;
    // 0x8016A38C: nop

    // 0x8016A390: bc1fl       L_8016A3A4
    if (!c1cs) {
        // 0x8016A394: trunc.w.s   $f6, $f10
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
            goto L_8016A3A4;
    }
    goto skip_4;
    // 0x8016A394: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
    skip_4:
    // 0x8016A398: b           L_8016A5B0
    // 0x8016A39C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016A5B0;
    // 0x8016A39C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016A3A0: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
L_8016A3A4:
    // 0x8016A3A4: lwc1        $f4, 0x34($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016A3A8: lwc1        $f18, 0x3C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016A3AC: swc1        $f10, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f10.u32l;
    // 0x8016A3B0: mfc1        $t8, $f6
    ctx->r24 = (int32_t)ctx->f6.u32l;
    // 0x8016A3B4: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016A3B8: mtc1        $t8, $f8
    ctx->f8.u32l = ctx->r24;
    // 0x8016A3BC: mfc1        $t6, $f6
    ctx->r14 = (int32_t)ctx->f6.u32l;
    // 0x8016A3C0: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016A3C4: mtc1        $t6, $f8
    ctx->f8.u32l = ctx->r14;
    // 0x8016A3C8: nop

    // 0x8016A3CC: cvt.s.w     $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    ctx->f16.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016A3D0: mul.s       $f6, $f0, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f18.fl);
    // 0x8016A3D4: lwc1        $f8, 0x48($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016A3D8: mul.s       $f10, $f8, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f16.fl);
    // 0x8016A3DC: sub.s       $f6, $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016A3E0: trunc.w.s   $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016A3E4: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x8016A3E8: nop

    // 0x8016A3EC: beql        $v1, $zero, L_8016A444
    if (ctx->r3 == 0) {
        // 0x8016A3F0: lwc1        $f4, 0x48($sp)
        ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
            goto L_8016A444;
    }
    goto skip_5;
    // 0x8016A3F0: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    skip_5:
    // 0x8016A3F4: mtc1        $v1, $f10
    ctx->f10.u32l = ctx->r3;
    // 0x8016A3F8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A3FC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8016A400: cvt.s.w     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016A404: div.s       $f14, $f6, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8016A408: mul.s       $f6, $f18, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f12.fl);
    // 0x8016A40C: nop

    // 0x8016A410: mul.s       $f10, $f2, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f4.fl);
    // 0x8016A414: sub.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016A418: lwc1        $f6, 0x0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016A41C: mul.s       $f16, $f4, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f16.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x8016A420: neg.s       $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = -ctx->f8.fl;
    // 0x8016A424: mul.s       $f10, $f2, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f6.fl);
    // 0x8016A428: nop

    // 0x8016A42C: mul.s       $f6, $f4, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x8016A430: add.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8016A434: mul.s       $f0, $f8, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = MUL_S(ctx->f8.fl, ctx->f14.fl);
    // 0x8016A438: b           L_8016A564
    // 0x8016A43C: nop

        goto L_8016A564;
    // 0x8016A43C: nop

    // 0x8016A440: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
L_8016A444:
    // 0x8016A444: trunc.w.s   $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016A448: swc1        $f4, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f4.u32l;
    // 0x8016A44C: trunc.w.s   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x8016A450: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    // 0x8016A454: lwc1        $f10, 0x38($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016A458: mtc1        $t9, $f6
    ctx->f6.u32l = ctx->r25;
    // 0x8016A45C: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
    // 0x8016A460: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016A464: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x8016A468: nop

    // 0x8016A46C: cvt.s.w     $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    ctx->f4.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016A470: mul.s       $f6, $f8, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x8016A474: lwc1        $f8, 0x44($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016A478: mul.s       $f4, $f8, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x8016A47C: sub.s       $f6, $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x8016A480: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016A484: mfc1        $v1, $f4
    ctx->r3 = (int32_t)ctx->f4.u32l;
    // 0x8016A488: nop

    // 0x8016A48C: beql        $v1, $zero, L_8016A4E4
    if (ctx->r3 == 0) {
        // 0x8016A490: lwc1        $f10, 0x38($sp)
        ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
            goto L_8016A4E4;
    }
    goto skip_6;
    // 0x8016A490: lwc1        $f10, 0x38($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
    skip_6:
    // 0x8016A494: mtc1        $v1, $f4
    ctx->f4.u32l = ctx->r3;
    // 0x8016A498: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A49C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8016A4A0: cvt.s.w     $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    ctx->f4.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016A4A4: div.s       $f12, $f6, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = DIV_S(ctx->f6.fl, ctx->f4.fl);
    // 0x8016A4A8: mul.s       $f6, $f10, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8016A4AC: nop

    // 0x8016A4B0: mul.s       $f4, $f14, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f18.fl);
    // 0x8016A4B4: sub.s       $f10, $f6, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x8016A4B8: lwc1        $f6, 0x0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016A4BC: mul.s       $f16, $f10, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f12.fl);
    // 0x8016A4C0: neg.s       $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = -ctx->f8.fl;
    // 0x8016A4C4: mul.s       $f4, $f14, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x8016A4C8: nop

    // 0x8016A4CC: mul.s       $f6, $f10, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8016A4D0: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016A4D4: mul.s       $f0, $f8, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = MUL_S(ctx->f8.fl, ctx->f12.fl);
    // 0x8016A4D8: b           L_8016A564
    // 0x8016A4DC: nop

        goto L_8016A564;
    // 0x8016A4DC: nop

    // 0x8016A4E0: lwc1        $f10, 0x38($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
L_8016A4E4:
    // 0x8016A4E4: lwc1        $f6, 0x44($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016A4E8: mul.s       $f4, $f0, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x8016A4EC: nop

    // 0x8016A4F0: mul.s       $f8, $f6, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x8016A4F4: sub.s       $f4, $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x8016A4F8: trunc.w.s   $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016A4FC: mfc1        $v1, $f8
    ctx->r3 = (int32_t)ctx->f8.u32l;
    // 0x8016A500: nop

    // 0x8016A504: beq         $v1, $zero, L_8016A55C
    if (ctx->r3 == 0) {
        // 0x8016A508: nop
    
            goto L_8016A55C;
    }
    // 0x8016A508: nop

    // 0x8016A50C: mtc1        $v1, $f8
    ctx->f8.u32l = ctx->r3;
    // 0x8016A510: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A514: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8016A518: cvt.s.w     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016A51C: div.s       $f2, $f4, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f4.fl, ctx->f8.fl);
    // 0x8016A520: lwc1        $f8, 0x34($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016A524: mul.s       $f4, $f10, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f12.fl);
    // 0x8016A528: nop

    // 0x8016A52C: mul.s       $f10, $f14, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x8016A530: sub.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f10.fl;
    // 0x8016A534: lwc1        $f4, 0x40($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016A538: mul.s       $f16, $f8, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016A53C: neg.s       $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = -ctx->f6.fl;
    // 0x8016A540: mul.s       $f10, $f14, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f4.fl);
    // 0x8016A544: nop

    // 0x8016A548: mul.s       $f4, $f8, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f12.fl);
    // 0x8016A54C: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8016A550: mul.s       $f0, $f6, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016A554: b           L_8016A564
    // 0x8016A558: nop

        goto L_8016A564;
    // 0x8016A558: nop

L_8016A55C:
    // 0x8016A55C: b           L_8016A5B0
    // 0x8016A560: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016A5B0;
    // 0x8016A560: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016A564:
    // 0x8016A564: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8016A568: nop

    // 0x8016A56C: c.lt.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl < ctx->f2.fl;
    // 0x8016A570: nop

    // 0x8016A574: bc1t        L_8016A5A8
    if (c1cs) {
        // 0x8016A578: nop
    
            goto L_8016A5A8;
    }
    // 0x8016A578: nop

    // 0x8016A57C: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x8016A580: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A584: bc1t        L_8016A5A8
    if (c1cs) {
        // 0x8016A588: nop
    
            goto L_8016A5A8;
    }
    // 0x8016A588: nop

    // 0x8016A58C: add.s       $f10, $f16, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f16.fl + ctx->f0.fl;
    // 0x8016A590: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016A594: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016A598: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016A59C: nop

    // 0x8016A5A0: bc1f        L_8016A5B0
    if (!c1cs) {
        // 0x8016A5A4: nop
    
            goto L_8016A5B0;
    }
    // 0x8016A5A4: nop

L_8016A5A8:
    // 0x8016A5A8: b           L_8016A5B0
    // 0x8016A5AC: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016A5B0;
    // 0x8016A5AC: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016A5B0:
    // 0x8016A5B0: jr          $ra
    // 0x8016A5B4: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
    return;
    // 0x8016A5B4: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
;}
RECOMP_FUNC void func_8016A5B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016A5B8: addiu       $sp, $sp, -0x78
    ctx->r29 = ADD32(ctx->r29, -0X78);
    // 0x8016A5BC: sw          $a3, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r7;
    // 0x8016A5C0: lwc1        $f6, 0x0($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X0);
    // 0x8016A5C4: lwc1        $f14, 0x0($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016A5C8: lwc1        $f10, 0x4($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8016A5CC: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016A5D0: mul.s       $f4, $f14, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x8016A5D4: lwc1        $f12, 0x8($a0)
    ctx->f12.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016A5D8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016A5DC: mul.s       $f8, $f16, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f10.fl);
    // 0x8016A5E0: lwc1        $f10, 0x8($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X8);
    // 0x8016A5E4: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x8016A5E8: mul.s       $f4, $f10, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f12.fl);
    // 0x8016A5EC: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8016A5F0: add.s       $f2, $f4, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016A5F4: c.lt.s      $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f2.fl < ctx->f8.fl;
    // 0x8016A5F8: nop

    // 0x8016A5FC: bc1fl       L_8016A610
    if (!c1cs) {
        // 0x8016A600: mov.s       $f18, $f2
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    ctx->f18.fl = ctx->f2.fl;
            goto L_8016A610;
    }
    goto skip_0;
    // 0x8016A600: mov.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    ctx->f18.fl = ctx->f2.fl;
    skip_0:
    // 0x8016A604: b           L_8016A610
    // 0x8016A608: neg.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = -ctx->f2.fl;
        goto L_8016A610;
    // 0x8016A608: neg.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = -ctx->f2.fl;
    // 0x8016A60C: mov.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    ctx->f18.fl = ctx->f2.fl;
L_8016A610:
    // 0x8016A610: lwc1        $f10, -0x5188($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X5188);
    // 0x8016A614: c.lt.s      $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f18.fl < ctx->f10.fl;
    // 0x8016A618: nop

    // 0x8016A61C: bc1fl       L_8016A630
    if (!c1cs) {
        // 0x8016A620: lwc1        $f4, 0x0($a1)
        ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
            goto L_8016A630;
    }
    goto skip_1;
    // 0x8016A620: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    skip_1:
    // 0x8016A624: b           L_8016AD84
    // 0x8016A628: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016AD84;
    // 0x8016A628: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016A62C: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
L_8016A630:
    // 0x8016A630: lwc1        $f8, 0x4($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8016A634: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A638: mul.s       $f6, $f14, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f4.fl);
    // 0x8016A63C: nop

    // 0x8016A640: mul.s       $f10, $f16, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x8016A644: lwc1        $f8, 0x8($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016A648: add.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016A64C: mul.s       $f6, $f12, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f8.fl);
    // 0x8016A650: lwc1        $f8, 0xC($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8016A654: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016A658: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x8016A65C: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8016A660: neg.s       $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = -ctx->f4.fl;
    // 0x8016A664: div.s       $f0, $f6, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016A668: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x8016A66C: nop

    // 0x8016A670: bc1t        L_8016A69C
    if (c1cs) {
        // 0x8016A674: nop
    
            goto L_8016A69C;
    }
    // 0x8016A674: nop

    // 0x8016A678: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016A67C: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x8016A680: or          $a3, $a2, $zero
    ctx->r7 = ctx->r6 | 0;
    // 0x8016A684: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x8016A688: lw          $t0, 0x84($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X84);
    // 0x8016A68C: or          $t1, $a0, $zero
    ctx->r9 = ctx->r4 | 0;
    // 0x8016A690: addiu       $t2, $sp, 0x4C
    ctx->r10 = ADD32(ctx->r29, 0X4C);
    // 0x8016A694: bc1f        L_8016A6A4
    if (!c1cs) {
        // 0x8016A698: addiu       $t3, $sp, 0x58
        ctx->r11 = ADD32(ctx->r29, 0X58);
            goto L_8016A6A4;
    }
    // 0x8016A698: addiu       $t3, $sp, 0x58
    ctx->r11 = ADD32(ctx->r29, 0X58);
L_8016A69C:
    // 0x8016A69C: b           L_8016AD84
    // 0x8016A6A0: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
        goto L_8016AD84;
    // 0x8016A6A0: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
L_8016A6A4:
    // 0x8016A6A4: addiu       $t4, $sp, 0x40
    ctx->r12 = ADD32(ctx->r29, 0X40);
    // 0x8016A6A8: addiu       $t5, $sp, 0x34
    ctx->r13 = ADD32(ctx->r29, 0X34);
    // 0x8016A6AC: addiu       $v0, $sp, 0x40
    ctx->r2 = ADD32(ctx->r29, 0X40);
    // 0x8016A6B0: lwc1        $f12, 0x0($a3)
    ctx->f12.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016A6B4: addiu       $t5, $t5, 0x4
    ctx->r13 = ADD32(ctx->r13, 0X4);
    // 0x8016A6B8: sltu        $at, $t5, $v0
    ctx->r1 = ctx->r13 < ctx->r2 ? 1 : 0;
    // 0x8016A6BC: mul.s       $f16, $f0, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x8016A6C0: beq         $at, $zero, L_8016A750
    if (ctx->r1 == 0) {
        // 0x8016A6C4: lwc1        $f18, 0x0($v1)
        ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
            goto L_8016A750;
    }
    // 0x8016A6C4: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016A6C8: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
L_8016A6CC:
    // 0x8016A6CC: addiu       $t5, $t5, 0x4
    ctx->r13 = ADD32(ctx->r13, 0X4);
    // 0x8016A6D0: sltu        $at, $t5, $v0
    ctx->r1 = ctx->r13 < ctx->r2 ? 1 : 0;
    // 0x8016A6D4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016A6D8: swc1        $f14, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->f14.u32l;
    // 0x8016A6DC: lh          $t6, 0x10($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X10);
    // 0x8016A6E0: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016A6E4: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016A6E8: mtc1        $t6, $f12
    ctx->f12.u32l = ctx->r14;
    // 0x8016A6EC: addiu       $t1, $t1, 0x2
    ctx->r9 = ADD32(ctx->r9, 0X2);
    // 0x8016A6F0: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016A6F4: cvt.s.w     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    ctx->f12.fl = CVT_S_W(ctx->f12.u32l);
    // 0x8016A6F8: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016A6FC: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016A700: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A704: swc1        $f12, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f12.u32l;
    // 0x8016A708: swc1        $f14, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f14.u32l;
    // 0x8016A70C: lh          $t7, 0x14($t1)
    ctx->r15 = MEM_H(ctx->r9, 0X14);
    // 0x8016A710: mtc1        $t7, $f14
    ctx->f14.u32l = ctx->r15;
    // 0x8016A714: nop

    // 0x8016A718: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A71C: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A720: swc1        $f14, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f14.u32l;
    // 0x8016A724: lh          $t8, 0x20($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X20);
    // 0x8016A728: mtc1        $t8, $f14
    ctx->f14.u32l = ctx->r24;
    // 0x8016A72C: nop

    // 0x8016A730: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A734: sub.s       $f12, $f14, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A738: swc1        $f12, -0x8($t5)
    MEM_W(-0X8, ctx->r13) = ctx->f12.u32l;
    // 0x8016A73C: lwc1        $f12, 0x0($a3)
    ctx->f12.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016A740: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016A744: mul.s       $f16, $f0, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x8016A748: bnel        $at, $zero, L_8016A6CC
    if (ctx->r1 != 0) {
        // 0x8016A74C: add.s       $f14, $f18, $f16
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_8016A6CC;
    }
    goto skip_2;
    // 0x8016A74C: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
    skip_2:
L_8016A750:
    // 0x8016A750: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
    // 0x8016A754: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016A758: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016A75C: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016A760: swc1        $f14, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->f14.u32l;
    // 0x8016A764: lh          $t6, 0x10($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X10);
    // 0x8016A768: addiu       $t1, $t1, 0x2
    ctx->r9 = ADD32(ctx->r9, 0X2);
    // 0x8016A76C: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016A770: mtc1        $t6, $f12
    ctx->f12.u32l = ctx->r14;
    // 0x8016A774: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016A778: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016A77C: cvt.s.w     $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    ctx->f12.fl = CVT_S_W(ctx->f12.u32l);
    // 0x8016A780: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A784: swc1        $f12, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f12.u32l;
    // 0x8016A788: swc1        $f14, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f14.u32l;
    // 0x8016A78C: lh          $t7, 0x14($t1)
    ctx->r15 = MEM_H(ctx->r9, 0X14);
    // 0x8016A790: mtc1        $t7, $f14
    ctx->f14.u32l = ctx->r15;
    // 0x8016A794: nop

    // 0x8016A798: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A79C: sub.s       $f14, $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A7A0: swc1        $f14, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f14.u32l;
    // 0x8016A7A4: lh          $t8, 0x20($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X20);
    // 0x8016A7A8: mtc1        $t8, $f14
    ctx->f14.u32l = ctx->r24;
    // 0x8016A7AC: nop

    // 0x8016A7B0: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016A7B4: sub.s       $f12, $f14, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = ctx->f14.fl - ctx->f12.fl;
    // 0x8016A7B8: swc1        $f12, -0x4($t5)
    MEM_W(-0X4, ctx->r13) = ctx->f12.u32l;
    // 0x8016A7BC: lwc1        $f16, 0x58($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X58);
    // 0x8016A7C0: lwc1        $f18, 0x5C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8016A7C4: lwc1        $f14, 0x60($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X60);
    // 0x8016A7C8: mul.s       $f10, $f16, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x8016A7CC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016A7D0: mul.s       $f6, $f18, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x8016A7D4: add.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8016A7D8: mul.s       $f8, $f14, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8016A7DC: lwc1        $f6, -0x5184($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X5184);
    // 0x8016A7E0: add.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x8016A7E4: lwc1        $f8, 0x40($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016A7E8: c.lt.s      $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f10.fl < ctx->f6.fl;
    // 0x8016A7EC: nop

    // 0x8016A7F0: bc1fl       L_8016A804
    if (!c1cs) {
        // 0x8016A7F4: trunc.w.s   $f4, $f8
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.u32l = TRUNC_W_S(ctx->f8.fl);
            goto L_8016A804;
    }
    goto skip_3;
    // 0x8016A7F4: trunc.w.s   $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.u32l = TRUNC_W_S(ctx->f8.fl);
    skip_3:
    // 0x8016A7F8: b           L_8016AD84
    // 0x8016A7FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016AD84;
    // 0x8016A7FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016A800: trunc.w.s   $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.u32l = TRUNC_W_S(ctx->f8.fl);
L_8016A804:
    // 0x8016A804: lwc1        $f6, 0x34($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016A808: swc1        $f8, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f8.u32l;
    // 0x8016A80C: lwc1        $f8, 0x48($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016A810: mfc1        $t6, $f4
    ctx->r14 = (int32_t)ctx->f4.u32l;
    // 0x8016A814: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016A818: swc1        $f6, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f6.u32l;
    // 0x8016A81C: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x8016A820: or          $t1, $a0, $zero
    ctx->r9 = ctx->r4 | 0;
    // 0x8016A824: mfc1        $t8, $f4
    ctx->r24 = (int32_t)ctx->f4.u32l;
    // 0x8016A828: cvt.s.w     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    ctx->f2.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016A82C: lwc1        $f4, 0x3C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016A830: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x8016A834: addiu       $t2, $sp, 0x4C
    ctx->r10 = ADD32(ctx->r29, 0X4C);
    // 0x8016A838: addiu       $t3, $sp, 0x58
    ctx->r11 = ADD32(ctx->r29, 0X58);
    // 0x8016A83C: cvt.s.w     $f12, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    ctx->f12.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016A840: mul.s       $f10, $f2, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f4.fl);
    // 0x8016A844: addiu       $t4, $sp, 0x40
    ctx->r12 = ADD32(ctx->r29, 0X40);
    // 0x8016A848: addiu       $t5, $sp, 0x34
    ctx->r13 = ADD32(ctx->r29, 0X34);
    // 0x8016A84C: addiu       $v0, $sp, 0x40
    ctx->r2 = ADD32(ctx->r29, 0X40);
    // 0x8016A850: mul.s       $f6, $f8, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f12.fl);
    // 0x8016A854: sub.s       $f10, $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f10.fl - ctx->f6.fl;
    // 0x8016A858: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8016A85C: mfc1        $v1, $f6
    ctx->r3 = (int32_t)ctx->f6.u32l;
    // 0x8016A860: lwc1        $f6, 0x48($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016A864: beql        $v1, $zero, L_8016A8C0
    if (ctx->r3 == 0) {
        // 0x8016A868: trunc.w.s   $f4, $f6
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
            goto L_8016A8C0;
    }
    goto skip_4;
    // 0x8016A868: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
    skip_4:
    // 0x8016A86C: mtc1        $v1, $f6
    ctx->f6.u32l = ctx->r3;
    // 0x8016A870: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A874: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016A878: cvt.s.w     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016A87C: div.s       $f2, $f10, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f6.fl);
    // 0x8016A880: lwc1        $f6, 0x4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X4);
    // 0x8016A884: mul.s       $f10, $f4, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8016A888: nop

    // 0x8016A88C: mul.s       $f4, $f14, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x8016A890: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x8016A894: lwc1        $f10, 0x0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016A898: mul.s       $f12, $f6, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016A89C: neg.s       $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = -ctx->f8.fl;
    // 0x8016A8A0: mul.s       $f4, $f14, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f10.fl);
    // 0x8016A8A4: nop

    // 0x8016A8A8: mul.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x8016A8AC: add.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x8016A8B0: mul.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016A8B4: b           L_8016A9F0
    // 0x8016A8B8: nop

        goto L_8016A9F0;
    // 0x8016A8B8: nop

    // 0x8016A8BC: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
L_8016A8C0:
    // 0x8016A8C0: swc1        $f6, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f6.u32l;
    // 0x8016A8C4: lwc1        $f6, 0x3C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016A8C8: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
    // 0x8016A8CC: lwc1        $f4, 0x38($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016A8D0: mtc1        $t7, $f10
    ctx->f10.u32l = ctx->r15;
    // 0x8016A8D4: swc1        $f4, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f4.u32l;
    // 0x8016A8D8: cvt.s.w     $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016A8DC: mul.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x8016A8E0: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016A8E4: lwc1        $f8, 0x44($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016A8E8: mfc1        $t9, $f4
    ctx->r25 = (int32_t)ctx->f4.u32l;
    // 0x8016A8EC: nop

    // 0x8016A8F0: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x8016A8F4: nop

    // 0x8016A8F8: cvt.s.w     $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    ctx->f4.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016A8FC: mul.s       $f4, $f8, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x8016A900: sub.s       $f10, $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x8016A904: trunc.w.s   $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8016A908: mfc1        $v1, $f4
    ctx->r3 = (int32_t)ctx->f4.u32l;
    // 0x8016A90C: nop

    // 0x8016A910: beql        $v1, $zero, L_8016A96C
    if (ctx->r3 == 0) {
        // 0x8016A914: lwc1        $f6, 0x38($sp)
        ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
            goto L_8016A96C;
    }
    goto skip_5;
    // 0x8016A914: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
    skip_5:
    // 0x8016A918: mtc1        $v1, $f4
    ctx->f4.u32l = ctx->r3;
    // 0x8016A91C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A920: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016A924: cvt.s.w     $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    ctx->f4.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016A928: div.s       $f2, $f10, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8016A92C: lwc1        $f10, 0x0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016A930: mul.s       $f4, $f10, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f14.fl);
    // 0x8016A934: nop

    // 0x8016A938: mul.s       $f10, $f18, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f6.fl);
    // 0x8016A93C: sub.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f10.fl;
    // 0x8016A940: lwc1        $f4, 0x4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X4);
    // 0x8016A944: mul.s       $f12, $f6, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016A948: neg.s       $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = -ctx->f8.fl;
    // 0x8016A94C: mul.s       $f10, $f18, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x8016A950: nop

    // 0x8016A954: mul.s       $f4, $f6, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x8016A958: add.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8016A95C: mul.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016A960: b           L_8016A9F0
    // 0x8016A964: nop

        goto L_8016A9F0;
    // 0x8016A964: nop

    // 0x8016A968: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
L_8016A96C:
    // 0x8016A96C: lwc1        $f4, 0x44($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016A970: lui         $at, 0xBF80
    ctx->r1 = S32(0XBF80 << 16);
    // 0x8016A974: mul.s       $f10, $f2, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f6.fl);
    // 0x8016A978: nop

    // 0x8016A97C: mul.s       $f8, $f4, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x8016A980: sub.s       $f10, $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x8016A984: trunc.w.s   $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8016A988: mfc1        $v1, $f8
    ctx->r3 = (int32_t)ctx->f8.u32l;
    // 0x8016A98C: nop

    // 0x8016A990: beq         $v1, $zero, L_8016A9E8
    if (ctx->r3 == 0) {
        // 0x8016A994: nop
    
            goto L_8016A9E8;
    }
    // 0x8016A994: nop

    // 0x8016A998: mtc1        $v1, $f8
    ctx->f8.u32l = ctx->r3;
    // 0x8016A99C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016A9A0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016A9A4: cvt.s.w     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016A9A8: div.s       $f2, $f10, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f8.fl);
    // 0x8016A9AC: lwc1        $f8, 0x34($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016A9B0: mul.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x8016A9B4: nop

    // 0x8016A9B8: mul.s       $f6, $f18, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f8.fl);
    // 0x8016A9BC: sub.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f10.fl - ctx->f6.fl;
    // 0x8016A9C0: lwc1        $f10, 0x40($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016A9C4: mul.s       $f12, $f8, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016A9C8: neg.s       $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = -ctx->f4.fl;
    // 0x8016A9CC: mul.s       $f6, $f18, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f10.fl);
    // 0x8016A9D0: nop

    // 0x8016A9D4: mul.s       $f10, $f8, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f16.fl);
    // 0x8016A9D8: add.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016A9DC: mul.s       $f0, $f4, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016A9E0: b           L_8016A9F0
    // 0x8016A9E4: nop

        goto L_8016A9F0;
    // 0x8016A9E4: nop

L_8016A9E8:
    // 0x8016A9E8: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x8016A9EC: nop

L_8016A9F0:
    // 0x8016A9F0: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x8016A9F4: lw          $t0, 0x84($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X84);
    // 0x8016A9F8: c.le.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl <= ctx->f12.fl;
    // 0x8016A9FC: nop

    // 0x8016AA00: bc1f        L_8016AA3C
    if (!c1cs) {
        // 0x8016AA04: nop
    
            goto L_8016AA3C;
    }
    // 0x8016AA04: nop

    // 0x8016AA08: c.le.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl <= ctx->f0.fl;
    // 0x8016AA0C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016AA10: bc1f        L_8016AA3C
    if (!c1cs) {
        // 0x8016AA14: nop
    
            goto L_8016AA3C;
    }
    // 0x8016AA14: nop

    // 0x8016AA18: add.s       $f6, $f12, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f12.fl + ctx->f0.fl;
    // 0x8016AA1C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016AA20: nop

    // 0x8016AA24: c.le.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl <= ctx->f8.fl;
    // 0x8016AA28: nop

    // 0x8016AA2C: bc1f        L_8016AA3C
    if (!c1cs) {
        // 0x8016AA30: nop
    
            goto L_8016AA3C;
    }
    // 0x8016AA30: nop

    // 0x8016AA34: b           L_8016AD84
    // 0x8016AA38: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016AD84;
    // 0x8016AA38: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016AA3C:
    // 0x8016AA3C: addiu       $t5, $t5, 0x4
    ctx->r13 = ADD32(ctx->r13, 0X4);
    // 0x8016AA40: beq         $t5, $v0, L_8016AAB0
    if (ctx->r13 == ctx->r2) {
        // 0x8016AA44: lh          $t8, 0x1C($t1)
        ctx->r24 = MEM_H(ctx->r9, 0X1C);
            goto L_8016AAB0;
    }
    // 0x8016AA44: lh          $t8, 0x1C($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X1C);
L_8016AA48:
    // 0x8016AA48: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016AA4C: addiu       $t5, $t5, 0x4
    ctx->r13 = ADD32(ctx->r13, 0X4);
    // 0x8016AA50: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016AA54: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016AA58: addiu       $t1, $t1, 0x2
    ctx->r9 = ADD32(ctx->r9, 0X2);
    // 0x8016AA5C: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016AA60: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016AA64: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016AA68: swc1        $f18, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f18.u32l;
    // 0x8016AA6C: lwc1        $f16, -0x4($t0)
    ctx->f16.u32l = MEM_W(ctx->r8, -0X4);
    // 0x8016AA70: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016AA74: swc1        $f16, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f16.u32l;
    // 0x8016AA78: lh          $t9, 0x20($t1)
    ctx->r25 = MEM_H(ctx->r9, 0X20);
    // 0x8016AA7C: mtc1        $t9, $f16
    ctx->f16.u32l = ctx->r25;
    // 0x8016AA80: nop

    // 0x8016AA84: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016AA88: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016AA8C: swc1        $f16, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f16.u32l;
    // 0x8016AA90: lh          $t6, 0x14($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X14);
    // 0x8016AA94: mtc1        $t6, $f16
    ctx->f16.u32l = ctx->r14;
    // 0x8016AA98: nop

    // 0x8016AA9C: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016AAA0: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016AAA4: swc1        $f18, -0x8($t5)
    MEM_W(-0X8, ctx->r13) = ctx->f18.u32l;
    // 0x8016AAA8: bne         $t5, $v0, L_8016AA48
    if (ctx->r13 != ctx->r2) {
        // 0x8016AAAC: lh          $t8, 0x1C($t1)
        ctx->r24 = MEM_H(ctx->r9, 0X1C);
            goto L_8016AA48;
    }
    // 0x8016AAAC: lh          $t8, 0x1C($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X1C);
L_8016AAB0:
    // 0x8016AAB0: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016AAB4: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016AAB8: addiu       $t1, $t1, 0x2
    ctx->r9 = ADD32(ctx->r9, 0X2);
    // 0x8016AABC: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016AAC0: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016AAC4: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016AAC8: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016AACC: swc1        $f18, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f18.u32l;
    // 0x8016AAD0: lwc1        $f16, -0x4($t0)
    ctx->f16.u32l = MEM_W(ctx->r8, -0X4);
    // 0x8016AAD4: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016AAD8: swc1        $f16, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f16.u32l;
    // 0x8016AADC: lh          $t9, 0x20($t1)
    ctx->r25 = MEM_H(ctx->r9, 0X20);
    // 0x8016AAE0: mtc1        $t9, $f16
    ctx->f16.u32l = ctx->r25;
    // 0x8016AAE4: nop

    // 0x8016AAE8: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016AAEC: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016AAF0: swc1        $f16, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f16.u32l;
    // 0x8016AAF4: lh          $t6, 0x14($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X14);
    // 0x8016AAF8: mtc1        $t6, $f16
    ctx->f16.u32l = ctx->r14;
    // 0x8016AAFC: nop

    // 0x8016AB00: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016AB04: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016AB08: swc1        $f18, -0x4($t5)
    MEM_W(-0X4, ctx->r13) = ctx->f18.u32l;
    // 0x8016AB0C: lwc1        $f16, 0x58($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X58);
    // 0x8016AB10: lwc1        $f18, 0x5C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8016AB14: lwc1        $f14, 0x60($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X60);
    // 0x8016AB18: mul.s       $f4, $f16, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x8016AB1C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016AB20: mul.s       $f8, $f18, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x8016AB24: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x8016AB28: mul.s       $f10, $f14, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8016AB2C: lwc1        $f8, -0x5180($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X5180);
    // 0x8016AB30: add.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8016AB34: lwc1        $f10, 0x40($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016AB38: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x8016AB3C: nop

    // 0x8016AB40: bc1fl       L_8016AB54
    if (!c1cs) {
        // 0x8016AB44: trunc.w.s   $f6, $f10
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
            goto L_8016AB54;
    }
    goto skip_6;
    // 0x8016AB44: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
    skip_6:
    // 0x8016AB48: b           L_8016AD84
    // 0x8016AB4C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016AD84;
    // 0x8016AB4C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016AB50: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
L_8016AB54:
    // 0x8016AB54: lwc1        $f8, 0x34($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016AB58: swc1        $f10, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f10.u32l;
    // 0x8016AB5C: lwc1        $f10, 0x48($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016AB60: mfc1        $t8, $f6
    ctx->r24 = (int32_t)ctx->f6.u32l;
    // 0x8016AB64: trunc.w.s   $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8016AB68: swc1        $f8, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f8.u32l;
    // 0x8016AB6C: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x8016AB70: mfc1        $t6, $f6
    ctx->r14 = (int32_t)ctx->f6.u32l;
    // 0x8016AB74: cvt.s.w     $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    ctx->f2.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016AB78: lwc1        $f6, 0x3C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016AB7C: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x8016AB80: nop

    // 0x8016AB84: cvt.s.w     $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    ctx->f12.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016AB88: mul.s       $f4, $f2, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f6.fl);
    // 0x8016AB8C: nop

    // 0x8016AB90: mul.s       $f8, $f10, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f12.fl);
    // 0x8016AB94: sub.s       $f4, $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x8016AB98: trunc.w.s   $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016AB9C: lwc1        $f4, 0x3C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016ABA0: mfc1        $v1, $f8
    ctx->r3 = (int32_t)ctx->f8.u32l;
    // 0x8016ABA4: nop

    // 0x8016ABA8: beq         $v1, $zero, L_8016AC00
    if (ctx->r3 == 0) {
        // 0x8016ABAC: nop
    
            goto L_8016AC00;
    }
    // 0x8016ABAC: nop

    // 0x8016ABB0: mtc1        $v1, $f8
    ctx->f8.u32l = ctx->r3;
    // 0x8016ABB4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016ABB8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8016ABBC: cvt.s.w     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016ABC0: div.s       $f2, $f4, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f4.fl, ctx->f8.fl);
    // 0x8016ABC4: lwc1        $f8, 0x0($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016ABC8: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x8016ABCC: nop

    // 0x8016ABD0: mul.s       $f6, $f14, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x8016ABD4: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8016ABD8: lwc1        $f4, 0x4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X4);
    // 0x8016ABDC: mul.s       $f12, $f8, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016ABE0: neg.s       $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = -ctx->f10.fl;
    // 0x8016ABE4: mul.s       $f6, $f14, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f4.fl);
    // 0x8016ABE8: nop

    // 0x8016ABEC: mul.s       $f4, $f8, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f16.fl);
    // 0x8016ABF0: add.s       $f10, $f6, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x8016ABF4: mul.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8016ABF8: b           L_8016AD30
    // 0x8016ABFC: nop

        goto L_8016AD30;
    // 0x8016ABFC: nop

L_8016AC00:
    // 0x8016AC00: trunc.w.s   $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016AC04: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016AC08: lwc1        $f6, 0x44($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016AC0C: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    // 0x8016AC10: mov.s       $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = ctx->f8.fl;
    // 0x8016AC14: swc1        $f8, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f8.u32l;
    // 0x8016AC18: mtc1        $t9, $f10
    ctx->f10.u32l = ctx->r25;
    // 0x8016AC1C: lwc1        $f8, 0x48($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016AC20: swc1        $f6, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f6.u32l;
    // 0x8016AC24: cvt.s.w     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016AC28: mul.s       $f10, $f6, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8016AC2C: trunc.w.s   $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8016AC30: mfc1        $t7, $f6
    ctx->r15 = (int32_t)ctx->f6.u32l;
    // 0x8016AC34: nop

    // 0x8016AC38: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x8016AC3C: nop

    // 0x8016AC40: cvt.s.w     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016AC44: mul.s       $f6, $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8016AC48: sub.s       $f6, $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016AC4C: trunc.w.s   $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016AC50: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x8016AC54: lwc1        $f10, 0x44($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016AC58: beq         $v1, $zero, L_8016ACB0
    if (ctx->r3 == 0) {
        // 0x8016AC5C: nop
    
            goto L_8016ACB0;
    }
    // 0x8016AC5C: nop

    // 0x8016AC60: mtc1        $v1, $f10
    ctx->f10.u32l = ctx->r3;
    // 0x8016AC64: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016AC68: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8016AC6C: cvt.s.w     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016AC70: div.s       $f2, $f6, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8016AC74: lwc1        $f6, 0x4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X4);
    // 0x8016AC78: mul.s       $f10, $f6, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x8016AC7C: nop

    // 0x8016AC80: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x8016AC84: sub.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f6.fl;
    // 0x8016AC88: lwc1        $f6, 0x0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016AC8C: mul.s       $f12, $f4, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016AC90: neg.s       $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = -ctx->f6.fl;
    // 0x8016AC94: mul.s       $f10, $f18, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f8.fl);
    // 0x8016AC98: nop

    // 0x8016AC9C: mul.s       $f8, $f4, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x8016ACA0: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x8016ACA4: mul.s       $f0, $f6, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016ACA8: b           L_8016AD30
    // 0x8016ACAC: nop

        goto L_8016AD30;
    // 0x8016ACAC: nop

L_8016ACB0:
    // 0x8016ACB0: mul.s       $f4, $f2, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f0.fl);
    // 0x8016ACB4: nop

    // 0x8016ACB8: mul.s       $f8, $f10, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f12.fl);
    // 0x8016ACBC: sub.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x8016ACC0: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016ACC4: mfc1        $v1, $f4
    ctx->r3 = (int32_t)ctx->f4.u32l;
    // 0x8016ACC8: nop

    // 0x8016ACCC: beq         $v1, $zero, L_8016AD28
    if (ctx->r3 == 0) {
        // 0x8016ACD0: nop
    
            goto L_8016AD28;
    }
    // 0x8016ACD0: nop

    // 0x8016ACD4: mtc1        $v1, $f6
    ctx->f6.u32l = ctx->r3;
    // 0x8016ACD8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016ACDC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016ACE0: cvt.s.w     $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016ACE4: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016ACE8: div.s       $f2, $f8, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = DIV_S(ctx->f8.fl, ctx->f4.fl);
    // 0x8016ACEC: lwc1        $f4, 0x34($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016ACF0: mul.s       $f8, $f6, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x8016ACF4: nop

    // 0x8016ACF8: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x8016ACFC: sub.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x8016AD00: lwc1        $f8, 0x40($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016AD04: mul.s       $f12, $f4, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016AD08: neg.s       $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = -ctx->f10.fl;
    // 0x8016AD0C: mul.s       $f6, $f18, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f8.fl);
    // 0x8016AD10: nop

    // 0x8016AD14: mul.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8016AD18: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8016AD1C: mul.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8016AD20: b           L_8016AD30
    // 0x8016AD24: nop

        goto L_8016AD30;
    // 0x8016AD24: nop

L_8016AD28:
    // 0x8016AD28: b           L_8016AD84
    // 0x8016AD2C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016AD84;
    // 0x8016AD2C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016AD30:
    // 0x8016AD30: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8016AD34: nop

    // 0x8016AD38: c.lt.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl < ctx->f4.fl;
    // 0x8016AD3C: nop

    // 0x8016AD40: bc1t        L_8016AD7C
    if (c1cs) {
        // 0x8016AD44: nop
    
            goto L_8016AD7C;
    }
    // 0x8016AD44: nop

    // 0x8016AD48: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8016AD4C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016AD50: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x8016AD54: nop

    // 0x8016AD58: bc1t        L_8016AD7C
    if (c1cs) {
        // 0x8016AD5C: nop
    
            goto L_8016AD7C;
    }
    // 0x8016AD5C: nop

    // 0x8016AD60: add.s       $f10, $f12, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f12.fl + ctx->f0.fl;
    // 0x8016AD64: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016AD68: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016AD6C: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016AD70: nop

    // 0x8016AD74: bc1f        L_8016AD84
    if (!c1cs) {
        // 0x8016AD78: nop
    
            goto L_8016AD84;
    }
    // 0x8016AD78: nop

L_8016AD7C:
    // 0x8016AD7C: b           L_8016AD84
    // 0x8016AD80: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016AD84;
    // 0x8016AD80: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016AD84:
    // 0x8016AD84: jr          $ra
    // 0x8016AD88: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
    return;
    // 0x8016AD88: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
;}
RECOMP_FUNC void func_8016AD8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016AD8C: lwc1        $f0, 0x0($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016AD90: swc1        $f0, 0xC($a1)
    MEM_W(0XC, ctx->r5) = ctx->f0.u32l;
    // 0x8016AD94: swc1        $f0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f0.u32l;
    // 0x8016AD98: swc1        $f0, 0x30($a1)
    MEM_W(0X30, ctx->r5) = ctx->f0.u32l;
    // 0x8016AD9C: swc1        $f0, 0x3C($a1)
    MEM_W(0X3C, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADA0: lwc1        $f0, 0xC($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8016ADA4: swc1        $f0, 0x24($a1)
    MEM_W(0X24, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADA8: swc1        $f0, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADAC: swc1        $f0, 0x48($a1)
    MEM_W(0X48, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADB0: swc1        $f0, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADB4: lwc1        $f2, 0x4($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016ADB8: swc1        $f2, 0x10($a1)
    MEM_W(0X10, ctx->r5) = ctx->f2.u32l;
    // 0x8016ADBC: swc1        $f2, 0x1C($a1)
    MEM_W(0X1C, ctx->r5) = ctx->f2.u32l;
    // 0x8016ADC0: swc1        $f2, 0x28($a1)
    MEM_W(0X28, ctx->r5) = ctx->f2.u32l;
    // 0x8016ADC4: swc1        $f2, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f2.u32l;
    // 0x8016ADC8: lwc1        $f0, 0x10($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X10);
    // 0x8016ADCC: swc1        $f0, 0x58($a1)
    MEM_W(0X58, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADD0: swc1        $f0, 0x4C($a1)
    MEM_W(0X4C, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADD4: swc1        $f0, 0x40($a1)
    MEM_W(0X40, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADD8: swc1        $f0, 0x34($a1)
    MEM_W(0X34, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADDC: lwc1        $f0, 0x8($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016ADE0: swc1        $f0, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADE4: swc1        $f0, 0x2C($a1)
    MEM_W(0X2C, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADE8: swc1        $f0, 0x5C($a1)
    MEM_W(0X5C, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADEC: swc1        $f0, 0x38($a1)
    MEM_W(0X38, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADF0: lwc1        $f0, 0x14($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X14);
    // 0x8016ADF4: swc1        $f0, 0x20($a1)
    MEM_W(0X20, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADF8: swc1        $f0, 0x14($a1)
    MEM_W(0X14, ctx->r5) = ctx->f0.u32l;
    // 0x8016ADFC: swc1        $f0, 0x44($a1)
    MEM_W(0X44, ctx->r5) = ctx->f0.u32l;
    // 0x8016AE00: jr          $ra
    // 0x8016AE04: swc1        $f0, 0x50($a1)
    MEM_W(0X50, ctx->r5) = ctx->f0.u32l;
    return;
    // 0x8016AE04: swc1        $f0, 0x50($a1)
    MEM_W(0X50, ctx->r5) = ctx->f0.u32l;
;}
RECOMP_FUNC void func_8016AE10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016AE10: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x8016AE14: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x8016AE18: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x8016AE1C: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x8016AE20: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x8016AE24: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8016AE28: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8016AE2C: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x8016AE30: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8016AE34: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x8016AE38: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016AE3C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016AE40: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    // 0x8016AE44: lw          $s2, -0x1134($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X1134);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: workspace guard (LOD_FIX_TEXT_MEASURE_GUARD, round 15 Task A;
    // round 16 Task B fixes a fallback-crash regression this guard introduced) ---
    // func_8016AE10 is the measure-mode twin of func_801682D0 above: same
    // global 0x8019EECC load into $s2, same unchecked +0x10/+0x14/+0x18/
    // +0x1C fixed-offset fields feeding func_8016B62C (which bottoms out in
    // the self-recursive func_8016B878), same +0x20/+0x24/+0x38 array loop.
    // Same fix, same reasoning: skip straight to L_8016AF20 (this function's
    // own analog of L_80168404 -- it likewise immediately overwrites $s2
    // with an unrelated global before using it again), leaving the
    // independent second half and tail intact for this function's own
    // return-value consumers.
    //
    // Round 16 fix: that "independent second half" is NOT fully independent
    // of the skipped code after all. $s1 (ctx->r17) is used from
    // L_8016AFD8/L_8016AFFC onward as a persistent, cross-iteration
    // array-index multiplier (the `sll $s1,2 / sub / sll 3 / add $s1 / sll 2`
    // stride computation feeding the interpolation loop's memory reads at
    // +0x230/+0x23C/+0x248/+0x48C/+0x498/+0x4A4/+0x4EC off a table based at
    // the unrelated global 0x8019F604) that MUST start at 0 for that loop to
    // stay in bounds. In the original, unpatched control flow $s1 is
    // guaranteed 0 by the time L_8016AF20 is reached -- either the plain
    // `or $s1,$zero,$zero` right before the array-loop's blez check
    // (vaddr 0x8016AED8), or the loop's own trailing reset once it finishes
    // (vaddr 0x8016AF1C) -- but BOTH of those live in the span this guard's
    // early jump skips over, so on the guarded path $s1 is left holding
    // whatever this callee-saved register last held in the CALLER's frame,
    // not 0. That stale value feeds directly into the stride math, producing
    // a wild out-of-range pointer -- this is the round-16 capture's crash
    // (Signal 10 inside the code this same guard jumps to, native offset
    // +1144, landing right at/after L_8016AF20 per objdump cross-check).
    // Confirmed by contrast: func_801682D0's structurally-identical tail
    // (L_801684BC, its own analog of L_8016AFD8) explicitly re-zeroes $s1
    // (`or $s1,$zero,$zero`, vaddr 0x801684CC) BEFORE using it the same way
    // in its own sqrt-normalize loop -- that explicit reset is why
    // func_801682D0's guard fires cleanly with no crash while
    // func_8016AE10's does not; func_801682D0 needs no equivalent fix (see
    // docs/issue27-31-ni0e-findings.md Round 16 for the full read of both
    // tails). Both callers of func_8016AE10 (funcs_19.c func_8003E5D8 and
    // func_8003E958) null-check $v0 before using it, so no caller-facing
    // return-value change is needed here -- restoring the one broken
    // invariant (which the original code always upheld for every path
    // reaching L_8016AF20) is both minimal and sufficient.
    if (lod_text_guard_workspace_invalid(rdram, (uint32_t)ctx->r18)) {
        if (!lod_text_guard_workspace_measure_logged) {
            lod_text_guard_workspace_measure_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016AE10 rejected invalid workspace s2=0x%08X "
                    "(stale text-struct pointer), skipping text measure\n",
                    (uint32_t)ctx->r18);
        }
        ctx->r17 = 0;  // $s1: restore the "always 0 entering L_8016AF20" invariant
        goto L_8016AF20;
    }
    // --- END PATCH ---
#endif
    // 0x8016AE48: addiu       $a0, $sp, 0x68
    ctx->r4 = ADD32(ctx->r29, 0X68);
    // 0x8016AE4C: addiu       $v0, $sp, 0x5C
    ctx->r2 = ADD32(ctx->r29, 0X5C);
    // 0x8016AE50: lwc1        $f16, 0x0($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016AE54: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016AE58: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016AE5C: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016AE60: beql        $at, $zero, L_8016AE88
    if (ctx->r1 == 0) {
        // 0x8016AE64: swc1        $f18, -0x4($v0)
        MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
            goto L_8016AE88;
    }
    goto skip_0;
    // 0x8016AE64: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    skip_0:
L_8016AE68:
    // 0x8016AE68: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016AE6C: lwc1        $f16, 0x4($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8016AE70: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016AE74: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016AE78: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016AE7C: bne         $at, $zero, L_8016AE68
    if (ctx->r1 != 0) {
        // 0x8016AE80: addiu       $v1, $v1, 0x4
        ctx->r3 = ADD32(ctx->r3, 0X4);
            goto L_8016AE68;
    }
    // 0x8016AE80: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016AE84: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
L_8016AE88:
    // 0x8016AE88: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016AE8C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016AE90: addiu       $a0, $a0, -0x10F0
    ctx->r4 = ADD32(ctx->r4, -0X10F0);
    // 0x8016AE94: jal         0x8016C198
    // 0x8016AE98: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    func_8016C198(rdram, ctx);
        goto after_0;
    // 0x8016AE98: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    after_0:
    // 0x8016AE9C: jal         0x8016C228
    // 0x8016AEA0: addiu       $a0, $sp, 0x5C
    ctx->r4 = ADD32(ctx->r29, 0X5C);
    func_8016C228(rdram, ctx);
        goto after_1;
    // 0x8016AEA0: addiu       $a0, $sp, 0x5C
    ctx->r4 = ADD32(ctx->r29, 0X5C);
    after_1:
    // 0x8016AEA4: lw          $a0, 0x10($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X10);
    // 0x8016AEA8: jal         0x8016B62C
    // 0x8016AEAC: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    func_8016B62C(rdram, ctx);
        goto after_2;
    // 0x8016AEAC: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    after_2:
    // 0x8016AEB0: lw          $a0, 0x14($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X14);
    // 0x8016AEB4: jal         0x8016B62C
    // 0x8016AEB8: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    func_8016B62C(rdram, ctx);
        goto after_3;
    // 0x8016AEB8: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    after_3:
    // 0x8016AEBC: lw          $a0, 0x18($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X18);
    // 0x8016AEC0: jal         0x8016B62C
    // 0x8016AEC4: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    func_8016B62C(rdram, ctx);
        goto after_4;
    // 0x8016AEC4: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    after_4:
    // 0x8016AEC8: lw          $a0, 0x1C($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X1C);
    // 0x8016AECC: jal         0x8016B62C
    // 0x8016AED0: lw          $a1, 0x34($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X34);
    func_8016B62C(rdram, ctx);
        goto after_5;
    // 0x8016AED0: lw          $a1, 0x34($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X34);
    after_5:
    // 0x8016AED4: lw          $t6, 0x20($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X20);
    // 0x8016AED8: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x8016AEDC: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: array-loop guard (LOD_FIX_TEXT_MEASURE_GUARD, round 18 Task B) ---
    // Measure-mode twin of func_801682D0's array-loop guard above; see
    // lod_text_guard_array_invalid for the full repro18 crash analysis
    // (func_801682D0 crashed on this exact unguarded pattern -- this
    // function shares the identical s2+0x20/+0x24/+0x38 loop feeding
    // func_8016B62C instead of func_8016890C). $s1/$s0 are already 0 at
    // this point (just set above), matching round 16's own "$s1 must be 0
    // entering L_8016AF20" invariant, so no extra register fixup is needed
    // for this jump.
    if (SIGNED(ctx->r14) > 0 && lod_text_guard_array_invalid(rdram, (uint32_t)ctx->r18)) {
        if (!lod_text_guard_array_measure_logged) {
            lod_text_guard_array_measure_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016AE10 rejected invalid array bases "
                    "obj_base=0x%08X str_base=0x%08X count=%d (stale text-struct array), "
                    "skipping array loop\n",
                    (uint32_t)MEM_W(ctx->r18, 0X24), (uint32_t)MEM_W(ctx->r18, 0X38),
                    (int32_t)ctx->r14);
        }
        goto L_8016AF20;
    }
    // --- END PATCH ---
#endif
    // 0x8016AEE0: blez        $t6, L_8016AF20
    if (SIGNED(ctx->r14) <= 0) {
        // 0x8016AEE4: nop
    
            goto L_8016AF20;
    }
    // 0x8016AEE4: nop

    // 0x8016AEE8: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
L_8016AEEC:
    // 0x8016AEEC: lw          $t9, 0x38($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X38);
    // 0x8016AEF0: addu        $t8, $t7, $s0
    ctx->r24 = ADD32(ctx->r15, ctx->r16);
    // 0x8016AEF4: addu        $t1, $t9, $s0
    ctx->r9 = ADD32(ctx->r25, ctx->r16);
    // 0x8016AEF8: lw          $a1, 0x0($t1)
    ctx->r5 = MEM_W(ctx->r9, 0X0);
    // 0x8016AEFC: jal         0x8016B62C
    // 0x8016AF00: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
    func_8016B62C(rdram, ctx);
        goto after_6;
    // 0x8016AF00: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
    after_6:
    // 0x8016AF04: lw          $t2, 0x20($s2)
    ctx->r10 = MEM_W(ctx->r18, 0X20);
    // 0x8016AF08: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x8016AF0C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x8016AF10: slt         $at, $s1, $t2
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r10) ? 1 : 0;
    // 0x8016AF14: bnel        $at, $zero, L_8016AEEC
    if (ctx->r1 != 0) {
        // 0x8016AF18: lw          $t7, 0x24($s2)
        ctx->r15 = MEM_W(ctx->r18, 0X24);
            goto L_8016AEEC;
    }
    goto skip_1;
    // 0x8016AF18: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
    skip_1:
    // 0x8016AF1C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_8016AF20:
    // 0x8016AF20: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016AF24: lw          $s4, -0x1A20($s4)
    ctx->r20 = MEM_W(ctx->r20, -0X1A20);
    // 0x8016AF28: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016AF2C: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x8016AF30: beq         $s4, $zero, L_8016AFD8
    if (ctx->r20 == 0) {
        // 0x8016AF34: or          $s5, $zero, $zero
        ctx->r21 = 0 | 0;
            goto L_8016AFD8;
    }
    // 0x8016AF34: or          $s5, $zero, $zero
    ctx->r21 = 0 | 0;
    // 0x8016AF38: lw          $v0, 0x4($s4)
    ctx->r2 = MEM_W(ctx->r20, 0X4);
L_8016AF3C:
    // 0x8016AF3C: beql        $v0, $zero, L_8016AFCC
    if (ctx->r2 == 0) {
        // 0x8016AF40: lw          $s4, 0x0($s4)
        ctx->r20 = MEM_W(ctx->r20, 0X0);
            goto L_8016AFCC;
    }
    goto skip_2;
    // 0x8016AF40: lw          $s4, 0x0($s4)
    ctx->r20 = MEM_W(ctx->r20, 0X0);
    skip_2:
    // 0x8016AF44: lw          $s2, 0x24($v0)
    ctx->r18 = MEM_W(ctx->r2, 0X24);
    // 0x8016AF48: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8016AF4C: beq         $s2, $zero, L_8016AFC8
    if (ctx->r18 == 0) {
        // 0x8016AF50: or          $a0, $s2, $zero
        ctx->r4 = ctx->r18 | 0;
            goto L_8016AFC8;
    }
    // 0x8016AF50: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8016AF54: lw          $t3, 0x0($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X0);
    // 0x8016AF58: addiu       $s3, $zero, 0x1
    ctx->r19 = ADD32(0, 0X1);
    // 0x8016AF5C: sw          $s5, 0x10($t3)
    MEM_W(0X10, ctx->r11) = ctx->r21;
    // 0x8016AF60: lw          $t4, 0x0($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X0);
    // 0x8016AF64: sw          $v0, 0xC($t4)
    MEM_W(0XC, ctx->r12) = ctx->r2;
    // 0x8016AF68: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
    // 0x8016AF6C: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x8016AF70: slti        $at, $v1, 0x282
    ctx->r1 = SIGNED(ctx->r3) < 0X282 ? 1 : 0;
    // 0x8016AF74: bne         $at, $zero, L_8016AF8C
    if (ctx->r1 != 0) {
        // 0x8016AF78: slti        $at, $v1, 0x285
        ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
            goto L_8016AF8C;
    }
    // 0x8016AF78: slti        $at, $v1, 0x285
    ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
    // 0x8016AF7C: beq         $at, $zero, L_8016AF8C
    if (ctx->r1 == 0) {
        // 0x8016AF80: nop
    
            goto L_8016AF8C;
    }
    // 0x8016AF80: nop

    // 0x8016AF84: b           L_8016AF8C
    // 0x8016AF88: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
        goto L_8016AF8C;
    // 0x8016AF88: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
L_8016AF8C:
    // 0x8016AF8C: jal         0x8016B308
    // 0x8016AF90: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    func_8016B308(rdram, ctx);
        goto after_7;
    // 0x8016AF90: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    after_7:
    // 0x8016AF94: lw          $a0, 0x14($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X14);
    // 0x8016AF98: beql        $a0, $zero, L_8016AFCC
    if (ctx->r4 == 0) {
        // 0x8016AF9C: lw          $s4, 0x0($s4)
        ctx->r20 = MEM_W(ctx->r20, 0X0);
            goto L_8016AFCC;
    }
    goto skip_3;
    // 0x8016AF9C: lw          $s4, 0x0($s4)
    ctx->r20 = MEM_W(ctx->r20, 0X0);
    skip_3:
    // 0x8016AFA0: lhu         $t5, 0x2($s2)
    ctx->r13 = MEM_HU(ctx->r18, 0X2);
    // 0x8016AFA4: andi        $t6, $t5, 0x8000
    ctx->r14 = ctx->r13 & 0X8000;
    // 0x8016AFA8: beq         $t6, $zero, L_8016AFC0
    if (ctx->r14 == 0) {
        // 0x8016AFAC: nop
    
            goto L_8016AFC0;
    }
    // 0x8016AFAC: nop

    // 0x8016AFB0: jal         0x8016B218
    // 0x8016AFB4: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    func_8016B218(rdram, ctx);
        goto after_8;
    // 0x8016AFB4: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    after_8:
    // 0x8016AFB8: b           L_8016AFCC
    // 0x8016AFBC: lw          $s4, 0x0($s4)
    ctx->r20 = MEM_W(ctx->r20, 0X0);
        goto L_8016AFCC;
    // 0x8016AFBC: lw          $s4, 0x0($s4)
    ctx->r20 = MEM_W(ctx->r20, 0X0);
L_8016AFC0:
    // 0x8016AFC0: jal         0x8016B290
    // 0x8016AFC4: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    func_8016B290(rdram, ctx);
        goto after_9;
    // 0x8016AFC4: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    after_9:
L_8016AFC8:
    // 0x8016AFC8: lw          $s4, 0x0($s4)
    ctx->r20 = MEM_W(ctx->r20, 0X0);
L_8016AFCC:
    // 0x8016AFCC: addiu       $s5, $s5, 0x1
    ctx->r21 = ADD32(ctx->r21, 0X1);
    // 0x8016AFD0: bnel        $s4, $zero, L_8016AF3C
    if (ctx->r20 != 0) {
        // 0x8016AFD4: lw          $v0, 0x4($s4)
        ctx->r2 = MEM_W(ctx->r20, 0X4);
            goto L_8016AF3C;
    }
    goto skip_4;
    // 0x8016AFD4: lw          $v0, 0x4($s4)
    ctx->r2 = MEM_W(ctx->r20, 0X4);
    skip_4:
L_8016AFD8:
    // 0x8016AFD8: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016AFDC: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x8016AFE0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016AFE4: lwc1        $f0, -0x5148($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X5148);
    // 0x8016AFE8: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x8016AFEC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8016AFF0: addiu       $t0, $zero, 0x4
    ctx->r8 = ADD32(0, 0X4);
    // 0x8016AFF4: addiu       $a1, $zero, 0xC
    ctx->r5 = ADD32(0, 0XC);
    // 0x8016AFF8: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
L_8016AFFC:
    // 0x8016AFFC: addu        $v0, $v1, $a2
    ctx->r2 = ADD32(ctx->r3, ctx->r6);
    // 0x8016B000: lh          $t7, 0x290($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X290);
    // 0x8016B004: beql        $t7, $zero, L_8016B0F8
    if (ctx->r15 == 0) {
        // 0x8016B008: lh          $t5, 0x4EC($v0)
        ctx->r13 = MEM_H(ctx->r2, 0X4EC);
            goto L_8016B0F8;
    }
    goto skip_5;
    // 0x8016B008: lh          $t5, 0x4EC($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X4EC);
    skip_5:
    // 0x8016B00C: lwc1        $f8, 0x248($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X248);
    // 0x8016B010: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016B014: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016B018: swc1        $f10, 0x248($v0)
    MEM_W(0X248, ctx->r2) = ctx->f10.u32l;
    // 0x8016B01C: sll         $t9, $s1, 2
    ctx->r25 = S32(ctx->r17 << 2);
    // 0x8016B020: subu        $t9, $t9, $s1
    ctx->r25 = SUB32(ctx->r25, ctx->r17);
    // 0x8016B024: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
    // 0x8016B028: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x8016B02C: addu        $t9, $t9, $s1
    ctx->r25 = ADD32(ctx->r25, ctx->r17);
    // 0x8016B030: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x8016B034: addu        $t1, $t8, $t9
    ctx->r9 = ADD32(ctx->r24, ctx->r25);
    // 0x8016B038: addu        $v0, $t1, $v1
    ctx->r2 = ADD32(ctx->r9, ctx->r3);
    // 0x8016B03C: lwc1        $f16, 0x230($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X230);
    // 0x8016B040: addiu       $a0, $a0, -0x4
    ctx->r4 = ADD32(ctx->r4, -0X4);
    // 0x8016B044: sll         $t3, $s1, 2
    ctx->r11 = S32(ctx->r17 << 2);
    // 0x8016B048: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B04C: beql        $v1, $a0, L_8016B0BC
    if (ctx->r3 == ctx->r4) {
        // 0x8016B050: swc1        $f18, 0x230($v0)
        MEM_W(0X230, ctx->r2) = ctx->f18.u32l;
            goto L_8016B0BC;
    }
    goto skip_6;
    // 0x8016B050: swc1        $f18, 0x230($v0)
    MEM_W(0X230, ctx->r2) = ctx->f18.u32l;
    skip_6:
    // 0x8016B054: swc1        $f18, 0x230($v0)
    MEM_W(0X230, ctx->r2) = ctx->f18.u32l;
L_8016B058:
    // 0x8016B058: lw          $t2, 0x0($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X0);
    // 0x8016B05C: subu        $t3, $t3, $s1
    ctx->r11 = SUB32(ctx->r11, ctx->r17);
    // 0x8016B060: sll         $t3, $t3, 3
    ctx->r11 = S32(ctx->r11 << 3);
    // 0x8016B064: addu        $t3, $t3, $s1
    ctx->r11 = ADD32(ctx->r11, ctx->r17);
    // 0x8016B068: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x8016B06C: addu        $t4, $t2, $t3
    ctx->r12 = ADD32(ctx->r10, ctx->r11);
    // 0x8016B070: addu        $v0, $t4, $v1
    ctx->r2 = ADD32(ctx->r12, ctx->r3);
    // 0x8016B074: lwc1        $f16, 0x23C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X23C);
    // 0x8016B078: sll         $t9, $s1, 2
    ctx->r25 = S32(ctx->r17 << 2);
    // 0x8016B07C: subu        $t9, $t9, $s1
    ctx->r25 = SUB32(ctx->r25, ctx->r17);
    // 0x8016B080: mul.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B084: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x8016B088: addu        $t9, $t9, $s1
    ctx->r25 = ADD32(ctx->r25, ctx->r17);
    // 0x8016B08C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016B090: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x8016B094: sll         $t3, $s1, 2
    ctx->r11 = S32(ctx->r17 << 2);
    // 0x8016B098: swc1        $f16, 0x23C($v0)
    MEM_W(0X23C, ctx->r2) = ctx->f16.u32l;
    // 0x8016B09C: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
    // 0x8016B0A0: addu        $t1, $t8, $t9
    ctx->r9 = ADD32(ctx->r24, ctx->r25);
    // 0x8016B0A4: addu        $v0, $t1, $v1
    ctx->r2 = ADD32(ctx->r9, ctx->r3);
    // 0x8016B0A8: lwc1        $f16, 0x230($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X230);
    // 0x8016B0AC: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B0B0: bnel        $v1, $a0, L_8016B058
    if (ctx->r3 != ctx->r4) {
        // 0x8016B0B4: swc1        $f18, 0x230($v0)
        MEM_W(0X230, ctx->r2) = ctx->f18.u32l;
            goto L_8016B058;
    }
    goto skip_7;
    // 0x8016B0B4: swc1        $f18, 0x230($v0)
    MEM_W(0X230, ctx->r2) = ctx->f18.u32l;
    skip_7:
    // 0x8016B0B8: swc1        $f18, 0x230($v0)
    MEM_W(0X230, ctx->r2) = ctx->f18.u32l;
L_8016B0BC:
    // 0x8016B0BC: subu        $t3, $t3, $s1
    ctx->r11 = SUB32(ctx->r11, ctx->r17);
    // 0x8016B0C0: lw          $t2, 0x0($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X0);
    // 0x8016B0C4: sll         $t3, $t3, 3
    ctx->r11 = S32(ctx->r11 << 3);
    // 0x8016B0C8: addu        $t3, $t3, $s1
    ctx->r11 = ADD32(ctx->r11, ctx->r17);
    // 0x8016B0CC: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x8016B0D0: addu        $t4, $t2, $t3
    ctx->r12 = ADD32(ctx->r10, ctx->r11);
    // 0x8016B0D4: addu        $v0, $t4, $v1
    ctx->r2 = ADD32(ctx->r12, ctx->r3);
    // 0x8016B0D8: lwc1        $f16, 0x23C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X23C);
    // 0x8016B0DC: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x8016B0E0: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016B0E4: mul.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B0E8: swc1        $f16, 0x23C($v0)
    MEM_W(0X23C, ctx->r2) = ctx->f16.u32l;
    // 0x8016B0EC: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x8016B0F0: addu        $v0, $v1, $a2
    ctx->r2 = ADD32(ctx->r3, ctx->r6);
    // 0x8016B0F4: lh          $t5, 0x4EC($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X4EC);
L_8016B0F8:
    // 0x8016B0F8: beql        $t5, $zero, L_8016B1E8
    if (ctx->r13 == 0) {
        // 0x8016B0FC: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_8016B1E8;
    }
    goto skip_8;
    // 0x8016B0FC: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    skip_8:
    // 0x8016B100: lwc1        $f8, 0x4A4($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X4A4);
    // 0x8016B104: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016B108: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016B10C: swc1        $f10, 0x4A4($v0)
    MEM_W(0X4A4, ctx->r2) = ctx->f10.u32l;
    // 0x8016B110: sll         $t7, $s1, 2
    ctx->r15 = S32(ctx->r17 << 2);
    // 0x8016B114: subu        $t7, $t7, $s1
    ctx->r15 = SUB32(ctx->r15, ctx->r17);
    // 0x8016B118: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016B11C: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x8016B120: addu        $t7, $t7, $s1
    ctx->r15 = ADD32(ctx->r15, ctx->r17);
    // 0x8016B124: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x8016B128: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x8016B12C: addu        $v0, $t8, $v1
    ctx->r2 = ADD32(ctx->r24, ctx->r3);
    // 0x8016B130: lwc1        $f16, 0x48C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X48C);
    // 0x8016B134: addiu       $a1, $a1, -0x4
    ctx->r5 = ADD32(ctx->r5, -0X4);
    // 0x8016B138: sll         $t1, $s1, 2
    ctx->r9 = S32(ctx->r17 << 2);
    // 0x8016B13C: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B140: beql        $v1, $a1, L_8016B1B0
    if (ctx->r3 == ctx->r5) {
        // 0x8016B144: swc1        $f18, 0x48C($v0)
        MEM_W(0X48C, ctx->r2) = ctx->f18.u32l;
            goto L_8016B1B0;
    }
    goto skip_9;
    // 0x8016B144: swc1        $f18, 0x48C($v0)
    MEM_W(0X48C, ctx->r2) = ctx->f18.u32l;
    skip_9:
    // 0x8016B148: swc1        $f18, 0x48C($v0)
    MEM_W(0X48C, ctx->r2) = ctx->f18.u32l;
L_8016B14C:
    // 0x8016B14C: lw          $t9, 0x0($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X0);
    // 0x8016B150: subu        $t1, $t1, $s1
    ctx->r9 = SUB32(ctx->r9, ctx->r17);
    // 0x8016B154: sll         $t1, $t1, 3
    ctx->r9 = S32(ctx->r9 << 3);
    // 0x8016B158: addu        $t1, $t1, $s1
    ctx->r9 = ADD32(ctx->r9, ctx->r17);
    // 0x8016B15C: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x8016B160: addu        $t2, $t9, $t1
    ctx->r10 = ADD32(ctx->r25, ctx->r9);
    // 0x8016B164: addu        $v0, $t2, $v1
    ctx->r2 = ADD32(ctx->r10, ctx->r3);
    // 0x8016B168: lwc1        $f16, 0x498($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X498);
    // 0x8016B16C: sll         $t7, $s1, 2
    ctx->r15 = S32(ctx->r17 << 2);
    // 0x8016B170: subu        $t7, $t7, $s1
    ctx->r15 = SUB32(ctx->r15, ctx->r17);
    // 0x8016B174: mul.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B178: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x8016B17C: addu        $t7, $t7, $s1
    ctx->r15 = ADD32(ctx->r15, ctx->r17);
    // 0x8016B180: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016B184: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x8016B188: sll         $t1, $s1, 2
    ctx->r9 = S32(ctx->r17 << 2);
    // 0x8016B18C: swc1        $f16, 0x498($v0)
    MEM_W(0X498, ctx->r2) = ctx->f16.u32l;
    // 0x8016B190: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016B194: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x8016B198: addu        $v0, $t8, $v1
    ctx->r2 = ADD32(ctx->r24, ctx->r3);
    // 0x8016B19C: lwc1        $f16, 0x48C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X48C);
    // 0x8016B1A0: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B1A4: bnel        $v1, $a1, L_8016B14C
    if (ctx->r3 != ctx->r5) {
        // 0x8016B1A8: swc1        $f18, 0x48C($v0)
        MEM_W(0X48C, ctx->r2) = ctx->f18.u32l;
            goto L_8016B14C;
    }
    goto skip_10;
    // 0x8016B1A8: swc1        $f18, 0x48C($v0)
    MEM_W(0X48C, ctx->r2) = ctx->f18.u32l;
    skip_10:
    // 0x8016B1AC: swc1        $f18, 0x48C($v0)
    MEM_W(0X48C, ctx->r2) = ctx->f18.u32l;
L_8016B1B0:
    // 0x8016B1B0: subu        $t1, $t1, $s1
    ctx->r9 = SUB32(ctx->r9, ctx->r17);
    // 0x8016B1B4: lw          $t9, 0x0($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X0);
    // 0x8016B1B8: sll         $t1, $t1, 3
    ctx->r9 = S32(ctx->r9 << 3);
    // 0x8016B1BC: addu        $t1, $t1, $s1
    ctx->r9 = ADD32(ctx->r9, ctx->r17);
    // 0x8016B1C0: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x8016B1C4: addu        $t2, $t9, $t1
    ctx->r10 = ADD32(ctx->r25, ctx->r9);
    // 0x8016B1C8: addu        $v0, $t2, $v1
    ctx->r2 = ADD32(ctx->r10, ctx->r3);
    // 0x8016B1CC: lwc1        $f16, 0x498($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X498);
    // 0x8016B1D0: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016B1D4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016B1D8: mul.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016B1DC: swc1        $f16, 0x498($v0)
    MEM_W(0X498, ctx->r2) = ctx->f16.u32l;
    // 0x8016B1E0: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x8016B1E4: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_8016B1E8:
    // 0x8016B1E8: bne         $s1, $t0, L_8016AFFC
    if (ctx->r17 != ctx->r8) {
        // 0x8016B1EC: addiu       $a2, $a2, 0x64
        ctx->r6 = ADD32(ctx->r6, 0X64);
            goto L_8016AFFC;
    }
    // 0x8016B1EC: addiu       $a2, $a2, 0x64
    ctx->r6 = ADD32(ctx->r6, 0X64);
    // 0x8016B1F0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x8016B1F4: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016B1F8: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016B1FC: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8016B200: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x8016B204: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x8016B208: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x8016B20C: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    // 0x8016B210: jr          $ra
    // 0x8016B214: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x8016B214: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_8016B218(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016B218 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016B218++;
    if (lod_text_guard_depth_8016B218 > 128) {
        if (!lod_text_guard_logged_8016B218) {
            lod_text_guard_logged_8016B218 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016B218 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016B218--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016B218: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016B21C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016B220: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016B224: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016B228: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016B22C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016B230: beq         $s0, $zero, L_8016B27C
    if (ctx->r16 == 0) {
        // 0x8016B234: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016B27C;
    }
L_8016B234:
    // 0x8016B234: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016B238: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x8016B23C: jal         0x8016B308
    // 0x8016B240: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016B308(rdram, ctx);
        goto after_0;
    // 0x8016B240: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x8016B244: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016B248: beql        $a0, $zero, L_8016B25C
    if (ctx->r4 == 0) {
        // 0x8016B24C: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_8016B25C;
    }
    goto skip_0;
    // 0x8016B24C: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x8016B250: jal         0x8016B218
    // 0x8016B254: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016B218(rdram, ctx);
        goto after_1;
    // 0x8016B254: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x8016B258: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_8016B25C:
    // 0x8016B25C: beql        $v0, $zero, L_8016B280
    if (ctx->r2 == 0) {
        // 0x8016B260: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B280;
    }
    goto skip_1;
    // 0x8016B260: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016B264: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x8016B268: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x8016B26C: bnel        $t7, $zero, L_8016B280
    if (ctx->r15 != 0) {
        // 0x8016B270: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B280;
    }
    goto skip_2;
    // 0x8016B270: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016B274: bne         $v0, $zero, L_8016B234
    if (ctx->r2 != 0) {
        // 0x8016B278: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016B234;
    }
    // 0x8016B278: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016B27C:
    // 0x8016B27C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016B280:
    // 0x8016B280: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016B284: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016B288: jr          $ra
    // 0x8016B28C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016B218--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016B28C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_8016B290(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016B290 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016B290++;
    if (lod_text_guard_depth_8016B290 > 128) {
        if (!lod_text_guard_logged_8016B290) {
            lod_text_guard_logged_8016B290 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016B290 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016B290--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016B290: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016B294: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016B298: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016B29C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016B2A0: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016B2A4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016B2A8: beq         $s0, $zero, L_8016B2F4
    if (ctx->r16 == 0) {
        // 0x8016B2AC: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016B2F4;
    }
L_8016B2AC:
    // 0x8016B2AC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016B2B0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8016B2B4: jal         0x8016B308
    // 0x8016B2B8: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016B308(rdram, ctx);
        goto after_0;
    // 0x8016B2B8: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x8016B2BC: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016B2C0: beql        $a0, $zero, L_8016B2D4
    if (ctx->r4 == 0) {
        // 0x8016B2C4: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_8016B2D4;
    }
    goto skip_0;
    // 0x8016B2C4: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x8016B2C8: jal         0x8016B290
    // 0x8016B2CC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016B290(rdram, ctx);
        goto after_1;
    // 0x8016B2CC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x8016B2D0: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_8016B2D4:
    // 0x8016B2D4: beql        $v0, $zero, L_8016B2F8
    if (ctx->r2 == 0) {
        // 0x8016B2D8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B2F8;
    }
    goto skip_1;
    // 0x8016B2D8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016B2DC: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x8016B2E0: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x8016B2E4: bnel        $t7, $zero, L_8016B2F8
    if (ctx->r15 != 0) {
        // 0x8016B2E8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B2F8;
    }
    goto skip_2;
    // 0x8016B2E8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016B2EC: bne         $v0, $zero, L_8016B2AC
    if (ctx->r2 != 0) {
        // 0x8016B2F0: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016B2AC;
    }
    // 0x8016B2F0: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016B2F4:
    // 0x8016B2F4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016B2F8:
    // 0x8016B2F8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016B2FC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016B300: jr          $ra
    // 0x8016B304: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016B290--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016B304: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_8016B308(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016B308: addiu       $sp, $sp, -0x88
    ctx->r29 = ADD32(ctx->r29, -0X88);
    // 0x8016B30C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016B310: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016B314: sw          $a0, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r4;
    // 0x8016B318: sw          $a2, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r6;
    // 0x8016B31C: lw          $v0, 0x74($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X74);
    // 0x8016B320: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016B324: lw          $a3, -0x1134($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X1134);
    // 0x8016B328: beql        $v0, $zero, L_8016B614
    if (ctx->r2 == 0) {
        // 0x8016B32C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B614;
    }
    goto skip_0;
    // 0x8016B32C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8016B330: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
    // 0x8016B334: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x8016B338: beql        $t9, $zero, L_8016B614
    if (ctx->r25 == 0) {
        // 0x8016B33C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B614;
    }
    goto skip_1;
    // 0x8016B33C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016B340: lhu         $v1, 0x20($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X20);
    // 0x8016B344: beql        $v1, $zero, L_8016B614
    if (ctx->r3 == 0) {
        // 0x8016B348: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B614;
    }
    goto skip_2;
    // 0x8016B348: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016B34C: lw          $t2, 0xC($a3)
    ctx->r10 = MEM_W(ctx->r7, 0XC);
    // 0x8016B350: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x8016B354: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016B358: addu        $t5, $t2, $t4
    ctx->r13 = ADD32(ctx->r10, ctx->r12);
    // 0x8016B35C: lhu         $v0, -0x2($t5)
    ctx->r2 = MEM_HU(ctx->r13, -0X2);
    // 0x8016B360: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016B364: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016B368: beq         $v0, $at, L_8016B610
    if (ctx->r2 == ctx->r1) {
        // 0x8016B36C: or          $t0, $v0, $zero
        ctx->r8 = ctx->r2 | 0;
            goto L_8016B610;
    }
    // 0x8016B36C: or          $t0, $v0, $zero
    ctx->r8 = ctx->r2 | 0;
    // 0x8016B370: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x8016B374: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x8016B378: subu        $t8, $t8, $v0
    ctx->r24 = SUB32(ctx->r24, ctx->r2);
    // 0x8016B37C: lw          $t7, 0x4($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X4);
    // 0x8016B380: sll         $t8, $t8, 4
    ctx->r24 = S32(ctx->r24 << 4);
    // 0x8016B384: addiu       $t9, $sp, 0x38
    ctx->r25 = ADD32(ctx->r29, 0X38);
    // 0x8016B388: sw          $t9, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r25;
    // 0x8016B38C: bne         $a1, $zero, L_8016B3C8
    if (ctx->r5 != 0) {
        // 0x8016B390: addu        $s0, $t7, $t8
        ctx->r16 = ADD32(ctx->r15, ctx->r24);
            goto L_8016B3C8;
    }
    // 0x8016B390: addu        $s0, $t7, $t8
    ctx->r16 = ADD32(ctx->r15, ctx->r24);
    // 0x8016B394: lwc1        $f4, 0x50($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X50);
    // 0x8016B398: swc1        $f4, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->f4.u32l;
    // 0x8016B39C: lwc1        $f6, 0x54($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X54);
    // 0x8016B3A0: swc1        $f6, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->f6.u32l;
    // 0x8016B3A4: lwc1        $f8, 0x58($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X58);
    // 0x8016B3A8: swc1        $f8, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->f8.u32l;
    // 0x8016B3AC: lh          $t2, 0x5C($a0)
    ctx->r10 = MEM_H(ctx->r4, 0X5C);
    // 0x8016B3B0: sh          $t2, 0x3E($sp)
    MEM_H(0X3E, ctx->r29) = ctx->r10;
    // 0x8016B3B4: lh          $t4, 0x5E($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X5E);
    // 0x8016B3B8: sh          $t4, 0x40($sp)
    MEM_H(0X40, ctx->r29) = ctx->r12;
    // 0x8016B3BC: lh          $t5, 0x60($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X60);
    // 0x8016B3C0: b           L_8016B3F0
    // 0x8016B3C4: sh          $t5, 0x42($sp)
    MEM_H(0X42, ctx->r29) = ctx->r13;
        goto L_8016B3F0;
    // 0x8016B3C4: sh          $t5, 0x42($sp)
    MEM_H(0X42, ctx->r29) = ctx->r13;
L_8016B3C8:
    // 0x8016B3C8: lw          $a0, 0x88($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X88);
    // 0x8016B3CC: addiu       $a1, $sp, 0x78
    ctx->r5 = ADD32(ctx->r29, 0X78);
    // 0x8016B3D0: addiu       $a2, $sp, 0x3E
    ctx->r6 = ADD32(ctx->r29, 0X3E);
    // 0x8016B3D4: addiu       $a3, $sp, 0x6C
    ctx->r7 = ADD32(ctx->r29, 0X6C);
    // 0x8016B3D8: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    // 0x8016B3DC: jal         0x80167F54
    // 0x8016B3E0: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    func_80167F54(rdram, ctx);
        goto after_0;
    // 0x8016B3E0: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    after_0:
    // 0x8016B3E4: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016B3E8: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016B3EC: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
L_8016B3F0:
    // 0x8016B3F0: lw          $t6, 0x90($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X90);
    // 0x8016B3F4: addiu       $v1, $sp, 0x78
    ctx->r3 = ADD32(ctx->r29, 0X78);
    // 0x8016B3F8: addiu       $v0, $sp, 0x38
    ctx->r2 = ADD32(ctx->r29, 0X38);
    // 0x8016B3FC: bne         $t6, $zero, L_8016B40C
    if (ctx->r14 != 0) {
        // 0x8016B400: lui         $at, 0x4120
        ctx->r1 = S32(0X4120 << 16);
            goto L_8016B40C;
    }
    // 0x8016B400: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x8016B404: sh          $zero, 0x42($sp)
    MEM_H(0X42, ctx->r29) = 0;
    // 0x8016B408: sh          $zero, 0x3E($sp)
    MEM_H(0X3E, ctx->r29) = 0;
L_8016B40C:
    // 0x8016B40C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8016B410: addiu       $a0, $sp, 0x3E
    ctx->r4 = ADD32(ctx->r29, 0X3E);
    // 0x8016B414: lwc1        $f14, 0x0($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016B418: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016B41C: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016B420: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x8016B424: beql        $at, $zero, L_8016B454
    if (ctx->r1 == 0) {
        // 0x8016B428: trunc.w.s   $f16, $f18
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
            goto L_8016B454;
    }
    goto skip_3;
    // 0x8016B428: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    skip_3:
L_8016B42C:
    // 0x8016B42C: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x8016B430: lwc1        $f14, 0x4($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8016B434: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016B438: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016B43C: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x8016B440: mfc1        $t8, $f16
    ctx->r24 = (int32_t)ctx->f16.u32l;
    // 0x8016B444: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016B448: bne         $at, $zero, L_8016B42C
    if (ctx->r1 != 0) {
        // 0x8016B44C: sh          $t8, -0x4($v0)
        MEM_H(-0X4, ctx->r2) = ctx->r24;
            goto L_8016B42C;
    }
    // 0x8016B44C: sh          $t8, -0x4($v0)
    MEM_H(-0X4, ctx->r2) = ctx->r24;
    // 0x8016B450: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
L_8016B454:
    // 0x8016B454: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016B458: mfc1        $t8, $f16
    ctx->r24 = (int32_t)ctx->f16.u32l;
    // 0x8016B45C: nop

    // 0x8016B460: sh          $t8, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r24;
    // 0x8016B464: lh          $t2, 0x3E($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X3E);
    // 0x8016B468: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x8016B46C: sh          $t0, 0x5C($sp)
    MEM_H(0X5C, ctx->r29) = ctx->r8;
    // 0x8016B470: bne         $t2, $zero, L_8016B520
    if (ctx->r10 != 0) {
        // 0x8016B474: sh          $t9, 0x5E($sp)
        MEM_H(0X5E, ctx->r29) = ctx->r25;
            goto L_8016B520;
    }
    // 0x8016B474: sh          $t9, 0x5E($sp)
    MEM_H(0X5E, ctx->r29) = ctx->r25;
    // 0x8016B478: lh          $t4, 0x42($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X42);
    // 0x8016B47C: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x8016B480: bne         $t4, $zero, L_8016B520
    if (ctx->r12 != 0) {
        // 0x8016B484: nop
    
            goto L_8016B520;
    }
    // 0x8016B484: nop

    // 0x8016B488: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x8016B48C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8016B490: jal         0x8016C260
    // 0x8016B494: lw          $a2, 0xC($t3)
    ctx->r6 = MEM_W(ctx->r11, 0XC);
    func_8016C260(rdram, ctx);
        goto after_1;
    // 0x8016B494: lw          $a2, 0xC($t3)
    ctx->r6 = MEM_W(ctx->r11, 0XC);
    after_1:
    // 0x8016B498: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016B49C: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016B4A0: lwc1        $f4, 0x18($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016B4A4: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x8016B4A8: lwc1        $f0, 0x74($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X74);
    // 0x8016B4AC: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x8016B4B0: nop

    // 0x8016B4B4: bc1fl       L_8016B508
    if (!c1cs) {
        // 0x8016B4B8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B508;
    }
    goto skip_4;
    // 0x8016B4B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_4:
    // 0x8016B4BC: lwc1        $f6, 0x24($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X24);
    // 0x8016B4C0: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x8016B4C4: nop

    // 0x8016B4C8: bc1fl       L_8016B508
    if (!c1cs) {
        // 0x8016B4CC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B508;
    }
    goto skip_5;
    // 0x8016B4CC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_5:
    // 0x8016B4D0: lwc1        $f0, 0x7C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X7C);
    // 0x8016B4D4: lwc1        $f8, 0x20($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X20);
    // 0x8016B4D8: c.le.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl <= ctx->f0.fl;
    // 0x8016B4DC: nop

    // 0x8016B4E0: bc1fl       L_8016B508
    if (!c1cs) {
        // 0x8016B4E4: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B508;
    }
    goto skip_6;
    // 0x8016B4E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_6:
    // 0x8016B4E8: lwc1        $f10, 0x2C($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X2C);
    // 0x8016B4EC: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x8016B4F0: nop

    // 0x8016B4F4: bc1fl       L_8016B508
    if (!c1cs) {
        // 0x8016B4F8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B508;
    }
    goto skip_7;
    // 0x8016B4F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_7:
    // 0x8016B4FC: b           L_8016B508
    // 0x8016B500: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016B508;
    // 0x8016B500: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016B504: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016B508:
    // 0x8016B508: beql        $v0, $zero, L_8016B614
    if (ctx->r2 == 0) {
        // 0x8016B50C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B614;
    }
    goto skip_8;
    // 0x8016B50C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_8:
    // 0x8016B510: jal         0x8016BA40
    // 0x8016B514: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_8016BA40(rdram, ctx);
        goto after_2;
    // 0x8016B514: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_2:
    // 0x8016B518: b           L_8016B614
    // 0x8016B51C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016B614;
    // 0x8016B51C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016B520:
    // 0x8016B520: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016B524: lw          $a1, -0x1130($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1130);
    // 0x8016B528: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016B52C: blez        $a1, L_8016B574
    if (SIGNED(ctx->r5) <= 0) {
        // 0x8016B530: nop
    
            goto L_8016B574;
    }
    // 0x8016B530: nop

    // 0x8016B534: lw          $t5, 0x0($t1)
    ctx->r13 = MEM_W(ctx->r9, 0X0);
    // 0x8016B538: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016B53C: lw          $v0, -0x1128($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X1128);
    // 0x8016B540: lw          $v1, 0x10($t5)
    ctx->r3 = MEM_W(ctx->r13, 0X10);
L_8016B544:
    // 0x8016B544: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x8016B548: bnel        $v1, $t6, L_8016B568
    if (ctx->r3 != ctx->r14) {
        // 0x8016B54C: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_8016B568;
    }
    goto skip_9;
    // 0x8016B54C: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    skip_9:
    // 0x8016B550: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    // 0x8016B554: addiu       $t7, $v1, 0x9C
    ctx->r15 = ADD32(ctx->r3, 0X9C);
    // 0x8016B558: sw          $t7, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r15;
    // 0x8016B55C: b           L_8016B574
    // 0x8016B560: addiu       $s0, $v1, 0x6C
    ctx->r16 = ADD32(ctx->r3, 0X6C);
        goto L_8016B574;
    // 0x8016B560: addiu       $s0, $v1, 0x6C
    ctx->r16 = ADD32(ctx->r3, 0X6C);
    // 0x8016B564: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_8016B568:
    // 0x8016B568: slt         $at, $a0, $a1
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x8016B56C: bne         $at, $zero, L_8016B544
    if (ctx->r1 != 0) {
        // 0x8016B570: addiu       $v0, $v0, 0x8
        ctx->r2 = ADD32(ctx->r2, 0X8);
            goto L_8016B544;
    }
    // 0x8016B570: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
L_8016B574:
    // 0x8016B574: beql        $a0, $a1, L_8016B614
    if (ctx->r4 == ctx->r5) {
        // 0x8016B578: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B614;
    }
    goto skip_10;
    // 0x8016B578: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_10:
    // 0x8016B57C: lw          $t8, 0x0($t1)
    ctx->r24 = MEM_W(ctx->r9, 0X0);
    // 0x8016B580: lw          $a0, 0x60($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X60);
    // 0x8016B584: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8016B588: jal         0x8016C260
    // 0x8016B58C: lw          $a2, 0xC($t8)
    ctx->r6 = MEM_W(ctx->r24, 0XC);
    func_8016C260(rdram, ctx);
        goto after_3;
    // 0x8016B58C: lw          $a2, 0xC($t8)
    ctx->r6 = MEM_W(ctx->r24, 0XC);
    after_3:
    // 0x8016B590: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016B594: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016B598: lwc1        $f16, 0x18($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016B59C: lw          $a0, 0x60($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X60);
    // 0x8016B5A0: lwc1        $f0, 0x74($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X74);
    // 0x8016B5A4: c.le.s      $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f16.fl <= ctx->f0.fl;
    // 0x8016B5A8: nop

    // 0x8016B5AC: bc1fl       L_8016B600
    if (!c1cs) {
        // 0x8016B5B0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B600;
    }
    goto skip_11;
    // 0x8016B5B0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_11:
    // 0x8016B5B4: lwc1        $f18, 0x24($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X24);
    // 0x8016B5B8: c.le.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl <= ctx->f18.fl;
    // 0x8016B5BC: nop

    // 0x8016B5C0: bc1fl       L_8016B600
    if (!c1cs) {
        // 0x8016B5C4: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B600;
    }
    goto skip_12;
    // 0x8016B5C4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_12:
    // 0x8016B5C8: lwc1        $f0, 0x7C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X7C);
    // 0x8016B5CC: lwc1        $f4, 0x20($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X20);
    // 0x8016B5D0: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x8016B5D4: nop

    // 0x8016B5D8: bc1fl       L_8016B600
    if (!c1cs) {
        // 0x8016B5DC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B600;
    }
    goto skip_13;
    // 0x8016B5DC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_13:
    // 0x8016B5E0: lwc1        $f6, 0x2C($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X2C);
    // 0x8016B5E4: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x8016B5E8: nop

    // 0x8016B5EC: bc1fl       L_8016B600
    if (!c1cs) {
        // 0x8016B5F0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B600;
    }
    goto skip_14;
    // 0x8016B5F0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_14:
    // 0x8016B5F4: b           L_8016B600
    // 0x8016B5F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016B600;
    // 0x8016B5F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016B5FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016B600:
    // 0x8016B600: beql        $v0, $zero, L_8016B614
    if (ctx->r2 == 0) {
        // 0x8016B604: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016B614;
    }
    goto skip_15;
    // 0x8016B604: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_15:
    // 0x8016B608: jal         0x8016BB60
    // 0x8016B60C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_8016BB60(rdram, ctx);
        goto after_4;
    // 0x8016B60C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_4:
L_8016B610:
    // 0x8016B610: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016B614:
    // 0x8016B614: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016B618: addiu       $sp, $sp, 0x88
    ctx->r29 = ADD32(ctx->r29, 0X88);
    // 0x8016B61C: jr          $ra
    // 0x8016B620: nop

    return;
    // 0x8016B620: nop

;}
RECOMP_FUNC void func_8016B624(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016B624: jr          $ra
    // 0x8016B628: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x8016B628: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8016B62C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016B62C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8016B630: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x8016B634: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016B638: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016B63C: or          $s5, $a0, $zero
    ctx->r21 = ctx->r4 | 0;
    // 0x8016B640: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8016B644: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x8016B648: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x8016B64C: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8016B650: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x8016B654: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x8016B658: beq         $a0, $zero, L_8016B84C
    if (ctx->r4 == 0) {
        // 0x8016B65C: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_8016B84C;
    }
    // 0x8016B65C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016B660: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x8016B664: addiu       $s7, $s7, -0x9FC
    ctx->r23 = ADD32(ctx->r23, -0X9FC);
    // 0x8016B668: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x8016B66C: lwc1        $f4, 0x8($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016B670: addiu       $a1, $s5, 0x8
    ctx->r5 = ADD32(ctx->r21, 0X8);
    // 0x8016B674: lwc1        $f0, 0x14($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X14);
    // 0x8016B678: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x8016B67C: nop

    // 0x8016B680: bc1fl       L_8016B6D4
    if (!c1cs) {
        // 0x8016B684: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B6D4;
    }
    goto skip_0;
    // 0x8016B684: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x8016B688: lwc1        $f6, 0x14($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X14);
    // 0x8016B68C: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x8016B690: nop

    // 0x8016B694: bc1fl       L_8016B6D4
    if (!c1cs) {
        // 0x8016B698: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B6D4;
    }
    goto skip_1;
    // 0x8016B698: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x8016B69C: lwc1        $f0, 0x1C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X1C);
    // 0x8016B6A0: lwc1        $f8, 0x10($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X10);
    // 0x8016B6A4: c.le.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl <= ctx->f0.fl;
    // 0x8016B6A8: nop

    // 0x8016B6AC: bc1fl       L_8016B6D4
    if (!c1cs) {
        // 0x8016B6B0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B6D4;
    }
    goto skip_2;
    // 0x8016B6B0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_2:
    // 0x8016B6B4: lwc1        $f10, 0x1C($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X1C);
    // 0x8016B6B8: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x8016B6BC: nop

    // 0x8016B6C0: bc1fl       L_8016B6D4
    if (!c1cs) {
        // 0x8016B6C4: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B6D4;
    }
    goto skip_3;
    // 0x8016B6C4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_3:
    // 0x8016B6C8: b           L_8016B6D4
    // 0x8016B6CC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016B6D4;
    // 0x8016B6CC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016B6D0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016B6D4:
    // 0x8016B6D4: beq         $v0, $zero, L_8016B84C
    if (ctx->r2 == 0) {
        // 0x8016B6D8: lui         $a0, 0x801A
        ctx->r4 = S32(0X801A << 16);
            goto L_8016B84C;
    }
    // 0x8016B6D8: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016B6DC: jal         0x80169B88
    // 0x8016B6E0: addiu       $a0, $a0, -0x1110
    ctx->r4 = ADD32(ctx->r4, -0X1110);
    func_80169B88(rdram, ctx);
        goto after_0;
    // 0x8016B6E0: addiu       $a0, $a0, -0x1110
    ctx->r4 = ADD32(ctx->r4, -0X1110);
    after_0:
    // 0x8016B6E4: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016B6E8: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x8016B6EC: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8016B6F0: addiu       $s6, $s6, 0x2490
    ctx->r22 = ADD32(ctx->r22, 0X2490);
    // 0x8016B6F4: addiu       $s0, $s0, -0x111C
    ctx->r16 = ADD32(ctx->r16, -0X111C);
    // 0x8016B6F8: addiu       $t6, $t6, -0x1680
    ctx->r14 = ADD32(ctx->r14, -0X1680);
    // 0x8016B6FC: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x8016B700: sw          $t6, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r14;
    // 0x8016B704: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016B708: addiu       $a0, $a0, -0x9F8
    ctx->r4 = ADD32(ctx->r4, -0X9F8);
    // 0x8016B70C: jal         0x80000F30
    // 0x8016B710: lw          $a1, 0x0($s5)
    ctx->r5 = MEM_W(ctx->r21, 0X0);
    bzero_recomp(rdram, ctx);
        goto after_1;
    // 0x8016B710: lw          $a1, 0x0($s5)
    ctx->r5 = MEM_W(ctx->r21, 0X0);
    after_1:
    // 0x8016B714: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016B718: sw          $s1, -0x10F8($at)
    MEM_W(-0X10F8, ctx->r1) = ctx->r17;
    // 0x8016B71C: jal         0x8016B878
    // 0x8016B720: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8016B878(rdram, ctx);
        goto after_2;
    // 0x8016B720: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_2:
    // 0x8016B724: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x8016B728: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x8016B72C: addiu       $s4, $zero, 0x30
    ctx->r20 = ADD32(0, 0X30);
    // 0x8016B730: blez        $v1, L_8016B84C
    if (SIGNED(ctx->r3) <= 0) {
        // 0x8016B734: addiu       $s3, $zero, 0x28
        ctx->r19 = ADD32(0, 0X28);
            goto L_8016B84C;
    }
    // 0x8016B734: addiu       $s3, $zero, 0x28
    ctx->r19 = ADD32(0, 0X28);
    // 0x8016B738: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
L_8016B73C:
    // 0x8016B73C: lw          $t1, 0x4($s5)
    ctx->r9 = MEM_W(ctx->r21, 0X4);
    // 0x8016B740: addu        $t8, $t7, $s2
    ctx->r24 = ADD32(ctx->r15, ctx->r18);
    // 0x8016B744: lh          $t9, 0x0($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X0);
    // 0x8016B748: multu       $t9, $s3
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r19)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016B74C: mflo        $t0
    ctx->r8 = lo;
    // 0x8016B750: addu        $s0, $t0, $t1
    ctx->r16 = ADD32(ctx->r8, ctx->r9);
    // 0x8016B754: lh          $t2, 0x26($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X26);
    // 0x8016B758: beql        $t2, $zero, L_8016B76C
    if (ctx->r10 == 0) {
        // 0x8016B75C: lh          $t4, 0x24($s0)
        ctx->r12 = MEM_H(ctx->r16, 0X24);
            goto L_8016B76C;
    }
    goto skip_4;
    // 0x8016B75C: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
    skip_4:
    // 0x8016B760: b           L_8016B83C
    // 0x8016B764: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016B83C;
    // 0x8016B764: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x8016B768: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
L_8016B76C:
    // 0x8016B76C: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x8016B770: multu       $t4, $s4
    result = U64(U32(ctx->r12)) * U64(U32(ctx->r20)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016B774: lw          $t3, 0x4($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X4);
    // 0x8016B778: mflo        $t5
    ctx->r13 = lo;
    // 0x8016B77C: addu        $s1, $t3, $t5
    ctx->r17 = ADD32(ctx->r11, ctx->r13);
    // 0x8016B780: lw          $t6, 0x0($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X0);
    // 0x8016B784: bnel        $t6, $zero, L_8016B7A4
    if (ctx->r14 != 0) {
        // 0x8016B788: lwc1        $f0, 0x14($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X14);
            goto L_8016B7A4;
    }
    goto skip_5;
    // 0x8016B788: lwc1        $f0, 0x14($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X14);
    skip_5:
    // 0x8016B78C: lw          $t7, 0x8($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X8);
    // 0x8016B790: bnel        $t7, $zero, L_8016B7A4
    if (ctx->r15 != 0) {
        // 0x8016B794: lwc1        $f0, 0x14($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X14);
            goto L_8016B7A4;
    }
    goto skip_6;
    // 0x8016B794: lwc1        $f0, 0x14($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X14);
    skip_6:
    // 0x8016B798: b           L_8016B83C
    // 0x8016B79C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016B83C;
    // 0x8016B79C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x8016B7A0: lwc1        $f0, 0x14($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X14);
L_8016B7A4:
    // 0x8016B7A4: lwc1        $f16, 0xC($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0XC);
    // 0x8016B7A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016B7AC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x8016B7B0: c.le.s      $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f16.fl <= ctx->f0.fl;
    // 0x8016B7B4: nop

    // 0x8016B7B8: bc1fl       L_8016B80C
    if (!c1cs) {
        // 0x8016B7BC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B80C;
    }
    goto skip_7;
    // 0x8016B7BC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_7:
    // 0x8016B7C0: lwc1        $f18, 0x18($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016B7C4: c.le.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl <= ctx->f18.fl;
    // 0x8016B7C8: nop

    // 0x8016B7CC: bc1fl       L_8016B80C
    if (!c1cs) {
        // 0x8016B7D0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B80C;
    }
    goto skip_8;
    // 0x8016B7D0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_8:
    // 0x8016B7D4: lwc1        $f0, 0x1C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X1C);
    // 0x8016B7D8: lwc1        $f4, 0x14($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X14);
    // 0x8016B7DC: c.le.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl <= ctx->f0.fl;
    // 0x8016B7E0: nop

    // 0x8016B7E4: bc1fl       L_8016B80C
    if (!c1cs) {
        // 0x8016B7E8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B80C;
    }
    goto skip_9;
    // 0x8016B7E8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_9:
    // 0x8016B7EC: lwc1        $f6, 0x20($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X20);
    // 0x8016B7F0: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x8016B7F4: nop

    // 0x8016B7F8: bc1fl       L_8016B80C
    if (!c1cs) {
        // 0x8016B7FC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8016B80C;
    }
    goto skip_10;
    // 0x8016B7FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_10:
    // 0x8016B800: b           L_8016B80C
    // 0x8016B804: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016B80C;
    // 0x8016B804: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016B808: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016B80C:
    // 0x8016B80C: bne         $v0, $zero, L_8016B81C
    if (ctx->r2 != 0) {
        // 0x8016B810: nop
    
            goto L_8016B81C;
    }
    // 0x8016B810: nop

    // 0x8016B814: b           L_8016B83C
    // 0x8016B818: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016B83C;
    // 0x8016B818: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_8016B81C:
    // 0x8016B81C: jal         0x8016C260
    // 0x8016B820: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_8016C260(rdram, ctx);
        goto after_3;
    // 0x8016B820: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_3:
    // 0x8016B824: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016B828: jal         0x8016BA40
    // 0x8016B82C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016BA40(rdram, ctx);
        goto after_4;
    // 0x8016B82C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_4:
    // 0x8016B830: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8016B834: lw          $v1, -0x111C($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X111C);
    // 0x8016B838: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_8016B83C:
    // 0x8016B83C: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    // 0x8016B840: slt         $at, $s2, $v0
    ctx->r1 = SIGNED(ctx->r18) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x8016B844: bnel        $at, $zero, L_8016B73C
    if (ctx->r1 != 0) {
        // 0x8016B848: lw          $t7, 0x0($s6)
        ctx->r15 = MEM_W(ctx->r22, 0X0);
            goto L_8016B73C;
    }
    goto skip_11;
    // 0x8016B848: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
    skip_11:
L_8016B84C:
    // 0x8016B84C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x8016B850: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016B854: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016B858: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8016B85C: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x8016B860: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x8016B864: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x8016B868: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x8016B86C: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x8016B870: jr          $ra
    // 0x8016B874: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x8016B874: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_8016B878(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD) ---
    // func_8016B878 self-recurses (two `jal 0x8016B878` sites below) walking
    // a text-run array whose length/base come from the possibly-stale global
    // struct at 0x8019EECC. On valid data the recursion is shallow (bounded
    // by a real string's field count); past depth 128 this is the runaway
    // walk over garbage that otherwise stack-overflows natively. Aborts by
    // returning immediately without doing any work, unwinding one frame at a
    // time back to depth 0 as each stacked call also decrements below.
    lod_text_guard_measure_depth++;
    if (lod_text_guard_measure_depth > 128) {
        if (!lod_text_guard_measure_logged) {
            lod_text_guard_measure_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016B878 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_measure_depth--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016B878: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8016B87C: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x8016B880: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016B884: lw          $t7, -0x10F8($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X10F8);
    // 0x8016B888: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8016B88C: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8016B890: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x8016B894: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016B898: addu        $t2, $t6, $t7
    ctx->r10 = ADD32(ctx->r14, ctx->r15);
    // 0x8016B89C: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    // 0x8016B8A0: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016B8A4: bnel        $v1, $at, L_8016B934
    if (ctx->r3 != ctx->r1) {
        // 0x8016B8A8: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_8016B934;
    }
    goto skip_0;
    // 0x8016B8A8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_0:
    // 0x8016B8AC: lh          $a2, 0x2($t2)
    ctx->r6 = MEM_H(ctx->r10, 0X2);
    // 0x8016B8B0: lw          $v0, 0x4($t2)
    ctx->r2 = MEM_W(ctx->r10, 0X4);
    // 0x8016B8B4: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016B8B8: blez        $a2, L_8016BA30
    if (SIGNED(ctx->r6) <= 0) {
        // 0x8016B8BC: lui         $t1, 0x8019
        ctx->r9 = S32(0X8019 << 16);
            goto L_8016BA30;
    }
    // 0x8016B8BC: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x8016B8C0: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016B8C4: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016B8C8: addiu       $a1, $a1, -0x111C
    ctx->r5 = ADD32(ctx->r5, -0X111C);
    // 0x8016B8CC: addiu       $a3, $a3, -0x9F8
    ctx->r7 = ADD32(ctx->r7, -0X9F8);
    // 0x8016B8D0: addiu       $t1, $t1, 0x2490
    ctx->r9 = ADD32(ctx->r9, 0X2490);
    // 0x8016B8D4: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
L_8016B8D8:
    // 0x8016B8D8: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x8016B8DC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016B8E0: addu        $a0, $a3, $t8
    ctx->r4 = ADD32(ctx->r7, ctx->r24);
    // 0x8016B8E4: lbu         $t9, 0x0($a0)
    ctx->r25 = MEM_BU(ctx->r4, 0X0);
    // 0x8016B8E8: bnel        $t9, $zero, L_8016B920
    if (ctx->r25 != 0) {
        // 0x8016B8EC: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_8016B920;
    }
    goto skip_1;
    // 0x8016B8EC: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_1:
    // 0x8016B8F0: lw          $t5, 0x0($a1)
    ctx->r13 = MEM_W(ctx->r5, 0X0);
    // 0x8016B8F4: lw          $t4, 0x0($t1)
    ctx->r12 = MEM_W(ctx->r9, 0X0);
    // 0x8016B8F8: sb          $t0, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r8;
    // 0x8016B8FC: lh          $t3, 0x0($v0)
    ctx->r11 = MEM_H(ctx->r2, 0X0);
    // 0x8016B900: sll         $t6, $t5, 1
    ctx->r14 = S32(ctx->r13 << 1);
    // 0x8016B904: addu        $t7, $t4, $t6
    ctx->r15 = ADD32(ctx->r12, ctx->r14);
    // 0x8016B908: sh          $t3, 0x0($t7)
    MEM_H(0X0, ctx->r15) = ctx->r11;
    // 0x8016B90C: lw          $t8, 0x0($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X0);
    // 0x8016B910: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x8016B914: sw          $t9, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r25;
    // 0x8016B918: lh          $a2, 0x2($t2)
    ctx->r6 = MEM_H(ctx->r10, 0X2);
    // 0x8016B91C: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
L_8016B920:
    // 0x8016B920: bne         $at, $zero, L_8016B8D8
    if (ctx->r1 != 0) {
        // 0x8016B924: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
            goto L_8016B8D8;
    }
    // 0x8016B924: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016B928: b           L_8016BA34
    // 0x8016B92C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8016BA34;
    // 0x8016B92C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8016B930: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
L_8016B934:
    // 0x8016B934: beq         $v1, $at, L_8016B968
    if (ctx->r3 == ctx->r1) {
        // 0x8016B938: sll         $t3, $v1, 2
        ctx->r11 = S32(ctx->r3 << 2);
            goto L_8016B968;
    }
    // 0x8016B938: sll         $t3, $v1, 2
    ctx->r11 = S32(ctx->r3 << 2);
    // 0x8016B93C: lh          $t5, 0x2($t2)
    ctx->r13 = MEM_H(ctx->r10, 0X2);
    // 0x8016B940: lui         $t4, 0x801A
    ctx->r12 = S32(0X801A << 16);
    // 0x8016B944: lw          $t4, -0x9FC($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X9FC);
    // 0x8016B948: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x8016B94C: sll         $a1, $v1, 2
    ctx->r5 = S32(ctx->r3 << 2);
    // 0x8016B950: addu        $t6, $t4, $a1
    ctx->r14 = ADD32(ctx->r12, ctx->r5);
    // 0x8016B954: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016B958: lwc1        $f6, 0x14($t6)
    ctx->f6.u32l = MEM_W(ctx->r14, 0X14);
    // 0x8016B95C: c.le.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl <= ctx->f6.fl;
    // 0x8016B960: nop

    // 0x8016B964: bc1f        L_8016B9CC
    if (!c1cs) {
        // 0x8016B968: lui         $t7, 0x801A
        ctx->r15 = S32(0X801A << 16);
            goto L_8016B9CC;
    }
L_8016B968:
    // 0x8016B968: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016B96C: addiu       $t7, $t7, -0x1110
    ctx->r15 = ADD32(ctx->r15, -0X1110);
    // 0x8016B970: addu        $v0, $t3, $t7
    ctx->r2 = ADD32(ctx->r11, ctx->r15);
    // 0x8016B974: lwc1        $f8, 0x0($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016B978: swc1        $f8, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f8.u32l;
    // 0x8016B97C: lh          $t8, 0x2($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X2);
    // 0x8016B980: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x8016B984: nop

    // 0x8016B988: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016B98C: swc1        $f16, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f16.u32l;
    // 0x8016B990: lh          $a0, 0x4($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X4);
    // 0x8016B994: jal         0x8016B878
    // 0x8016B998: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    func_8016B878(rdram, ctx);
        goto after_0;
    // 0x8016B998: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    after_0:
    // 0x8016B99C: lw          $t2, 0x1C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X1C);
    // 0x8016B9A0: lwc1        $f18, 0x20($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X20);
    // 0x8016B9A4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016B9A8: lh          $t9, 0x0($t2)
    ctx->r25 = MEM_H(ctx->r10, 0X0);
    // 0x8016B9AC: sll         $t5, $t9, 2
    ctx->r13 = S32(ctx->r25 << 2);
    // 0x8016B9B0: addu        $at, $at, $t5
    ctx->r1 = ADD32(ctx->r1, ctx->r13);
    // 0x8016B9B4: swc1        $f18, -0x1110($at)
    MEM_W(-0X1110, ctx->r1) = ctx->f18.u32l;
    // 0x8016B9B8: lh          $t4, 0x2($t2)
    ctx->r12 = MEM_H(ctx->r10, 0X2);
    // 0x8016B9BC: lh          $v1, 0x0($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X0);
    // 0x8016B9C0: mtc1        $t4, $f4
    ctx->f4.u32l = ctx->r12;
    // 0x8016B9C4: sll         $a1, $v1, 2
    ctx->r5 = S32(ctx->r3 << 2);
    // 0x8016B9C8: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
L_8016B9CC:
    // 0x8016B9CC: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8016B9D0: beq         $v1, $at, L_8016B9F0
    if (ctx->r3 == ctx->r1) {
        // 0x8016B9D4: lui         $t6, 0x801A
        ctx->r14 = S32(0X801A << 16);
            goto L_8016B9F0;
    }
    // 0x8016B9D4: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8016B9D8: lw          $t6, -0x9FC($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X9FC);
    // 0x8016B9DC: addu        $t3, $t6, $a1
    ctx->r11 = ADD32(ctx->r14, ctx->r5);
    // 0x8016B9E0: lwc1        $f6, 0x14($t3)
    ctx->f6.u32l = MEM_W(ctx->r11, 0X14);
    // 0x8016B9E4: c.le.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl <= ctx->f0.fl;
    // 0x8016B9E8: nop

    // 0x8016B9EC: bc1f        L_8016BA30
    if (!c1cs) {
        // 0x8016B9F0: lui         $t7, 0x801A
        ctx->r15 = S32(0X801A << 16);
            goto L_8016BA30;
    }
L_8016B9F0:
    // 0x8016B9F0: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016B9F4: addiu       $t7, $t7, -0x1110
    ctx->r15 = ADD32(ctx->r15, -0X1110);
    // 0x8016B9F8: addu        $v0, $a1, $t7
    ctx->r2 = ADD32(ctx->r5, ctx->r15);
    // 0x8016B9FC: lwc1        $f8, 0xC($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0XC);
    // 0x8016BA00: swc1        $f0, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f0.u32l;
    // 0x8016BA04: swc1        $f8, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f8.u32l;
    // 0x8016BA08: lh          $a0, 0x6($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X6);
    // 0x8016BA0C: jal         0x8016B878
    // 0x8016BA10: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    func_8016B878(rdram, ctx);
        goto after_1;
    // 0x8016BA10: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    after_1:
    // 0x8016BA14: lw          $t2, 0x1C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X1C);
    // 0x8016BA18: lwc1        $f10, 0x20($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X20);
    // 0x8016BA1C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016BA20: lh          $t8, 0x0($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X0);
    // 0x8016BA24: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x8016BA28: addu        $at, $at, $t9
    ctx->r1 = ADD32(ctx->r1, ctx->r25);
    // 0x8016BA2C: swc1        $f10, -0x1104($at)
    MEM_W(-0X1104, ctx->r1) = ctx->f10.u32l;
L_8016BA30:
    // 0x8016BA30: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8016BA34:
    // 0x8016BA34: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8016BA38: jr          $ra
    // 0x8016BA3C: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this is func_8016B878's single natural return point,
    // confirmed the only other `return;` in this function besides the
    // early-abort added above) ---
    lod_text_guard_measure_depth--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016BA3C: nop

;}
RECOMP_FUNC void func_8016BA40(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016BA40: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8016BA44: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x8016BA48: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x8016BA4C: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x8016BA50: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8016BA54: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x8016BA58: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8016BA5C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8016BA60: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016BA64: lui         $s3, 0x801A
    ctx->r19 = S32(0X801A << 16);
    // 0x8016BA68: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x8016BA6C: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x8016BA70: lui         $fp, 0x8019
    ctx->r30 = S32(0X8019 << 16);
    // 0x8016BA74: or          $s4, $a1, $zero
    ctx->r20 = ctx->r5 | 0;
    // 0x8016BA78: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8016BA7C: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8016BA80: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8016BA84: addiu       $fp, $fp, 0x2494
    ctx->r30 = ADD32(ctx->r30, 0X2494);
    // 0x8016BA88: addiu       $s7, $s7, -0x1118
    ctx->r23 = ADD32(ctx->r23, -0X1118);
    // 0x8016BA8C: addiu       $s6, $s6, 0x2530
    ctx->r22 = ADD32(ctx->r22, 0X2530);
    // 0x8016BA90: addiu       $s3, $s3, -0x9FC
    ctx->r19 = ADD32(ctx->r19, -0X9FC);
    // 0x8016BA94: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x8016BA98: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x8016BA9C: addiu       $s5, $zero, 0x10
    ctx->r21 = ADD32(0, 0X10);
L_8016BAA0:
    // 0x8016BAA0: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x8016BAA4: beql        $v0, $zero, L_8016BB28
    if (ctx->r2 == 0) {
        // 0x8016BAA8: addiu       $s1, $s1, 0x8
        ctx->r17 = ADD32(ctx->r17, 0X8);
            goto L_8016BB28;
    }
    goto skip_0;
    // 0x8016BAA8: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
    skip_0:
    // 0x8016BAAC: lw          $v1, 0x0($s3)
    ctx->r3 = MEM_W(ctx->r19, 0X0);
    // 0x8016BAB0: addu        $t6, $s6, $s1
    ctx->r14 = ADD32(ctx->r22, ctx->r17);
    // 0x8016BAB4: lw          $t7, 0x0($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X0);
    // 0x8016BAB8: lw          $t8, 0x6E4($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X6E4);
    // 0x8016BABC: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016BAC0: addiu       $t1, $t1, -0x1580
    ctx->r9 = ADD32(ctx->r9, -0X1580);
    // 0x8016BAC4: and         $t9, $t7, $t8
    ctx->r25 = ctx->r15 & ctx->r24;
    // 0x8016BAC8: beql        $t9, $zero, L_8016BB28
    if (ctx->r25 == 0) {
        // 0x8016BACC: addiu       $s1, $s1, 0x8
        ctx->r17 = ADD32(ctx->r17, 0X8);
            goto L_8016BB28;
    }
    goto skip_1;
    // 0x8016BACC: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
    skip_1:
    // 0x8016BAD0: lw          $t0, 0x18($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X18);
    // 0x8016BAD4: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016BAD8: addiu       $a0, $a0, -0x978
    ctx->r4 = ADD32(ctx->r4, -0X978);
    // 0x8016BADC: sw          $t0, 0x168($v1)
    MEM_W(0X168, ctx->r3) = ctx->r8;
    // 0x8016BAE0: sw          $zero, 0x0($s7)
    MEM_W(0X0, ctx->r23) = 0;
    // 0x8016BAE4: sw          $t1, 0x0($fp)
    MEM_W(0X0, ctx->r30) = ctx->r9;
    // 0x8016BAE8: lw          $t2, 0x0($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X0);
    // 0x8016BAEC: addiu       $s2, $s4, 0x18
    ctx->r18 = ADD32(ctx->r20, 0X18);
    // 0x8016BAF0: jal         0x80000F30
    // 0x8016BAF4: lh          $a1, 0x1C($t2)
    ctx->r5 = MEM_H(ctx->r10, 0X1C);
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8016BAF4: lh          $a1, 0x1C($t2)
    ctx->r5 = MEM_H(ctx->r10, 0X1C);
    after_0:
    // 0x8016BAF8: lw          $a0, 0x0($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X0);
    // 0x8016BAFC: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x8016BB00: jal         0x80169B88
    // 0x8016BB04: addiu       $a0, $a0, 0x5C
    ctx->r4 = ADD32(ctx->r4, 0X5C);
    func_80169B88(rdram, ctx);
        goto after_1;
    // 0x8016BB04: addiu       $a0, $a0, 0x5C
    ctx->r4 = ADD32(ctx->r4, 0X5C);
    after_1:
    // 0x8016BB08: lw          $t3, 0xC($s0)
    ctx->r11 = MEM_W(ctx->r16, 0XC);
    // 0x8016BB0C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016BB10: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016BB14: jal         0x8016BC44
    // 0x8016BB18: sw          $t3, -0x10F4($at)
    MEM_W(-0X10F4, ctx->r1) = ctx->r11;
    func_8016BC44(rdram, ctx);
        goto after_2;
    // 0x8016BB18: sw          $t3, -0x10F4($at)
    MEM_W(-0X10F4, ctx->r1) = ctx->r11;
    after_2:
    // 0x8016BB1C: jal         0x8016BEC0
    // 0x8016BB20: nop

    func_8016BEC0(rdram, ctx);
        goto after_3;
    // 0x8016BB20: nop

    after_3:
    // 0x8016BB24: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
L_8016BB28:
    // 0x8016BB28: bne         $s1, $s5, L_8016BAA0
    if (ctx->r17 != ctx->r21) {
        // 0x8016BB2C: addiu       $s0, $s0, 0x8
        ctx->r16 = ADD32(ctx->r16, 0X8);
            goto L_8016BAA0;
    }
    // 0x8016BB2C: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    // 0x8016BB30: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x8016BB34: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016BB38: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8016BB3C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8016BB40: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8016BB44: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8016BB48: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x8016BB4C: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x8016BB50: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x8016BB54: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x8016BB58: jr          $ra
    // 0x8016BB5C: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x8016BB5C: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_8016BB60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016BB60: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8016BB64: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x8016BB68: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x8016BB6C: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x8016BB70: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8016BB74: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x8016BB78: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x8016BB7C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016BB80: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016BB84: lui         $s3, 0x8019
    ctx->r19 = S32(0X8019 << 16);
    // 0x8016BB88: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016BB8C: lui         $s5, 0x801A
    ctx->r21 = S32(0X801A << 16);
    // 0x8016BB90: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x8016BB94: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x8016BB98: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8016BB9C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x8016BBA0: addiu       $s7, $s7, -0x750
    ctx->r23 = ADD32(ctx->r23, -0X750);
    // 0x8016BBA4: addiu       $s6, $s6, 0x2494
    ctx->r22 = ADD32(ctx->r22, 0X2494);
    // 0x8016BBA8: addiu       $s5, $s5, -0x1118
    ctx->r21 = ADD32(ctx->r21, -0X1118);
    // 0x8016BBAC: addiu       $s4, $s4, -0x9FC
    ctx->r20 = ADD32(ctx->r20, -0X9FC);
    // 0x8016BBB0: addiu       $s3, $s3, 0x2530
    ctx->r19 = ADD32(ctx->r19, 0X2530);
    // 0x8016BBB4: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x8016BBB8: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016BBBC: addiu       $s2, $zero, 0x10
    ctx->r18 = ADD32(0, 0X10);
L_8016BBC0:
    // 0x8016BBC0: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x8016BBC4: beql        $v0, $zero, L_8016BC10
    if (ctx->r2 == 0) {
        // 0x8016BBC8: addiu       $s0, $s0, 0x8
        ctx->r16 = ADD32(ctx->r16, 0X8);
            goto L_8016BC10;
    }
    goto skip_0;
    // 0x8016BBC8: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    skip_0:
    // 0x8016BBCC: lw          $v1, 0x0($s4)
    ctx->r3 = MEM_W(ctx->r20, 0X0);
    // 0x8016BBD0: addu        $t6, $s3, $s0
    ctx->r14 = ADD32(ctx->r19, ctx->r16);
    // 0x8016BBD4: lw          $t7, 0x0($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X0);
    // 0x8016BBD8: lw          $t8, 0x6E4($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X6E4);
    // 0x8016BBDC: and         $t9, $t7, $t8
    ctx->r25 = ctx->r15 & ctx->r24;
    // 0x8016BBE0: beql        $t9, $zero, L_8016BC10
    if (ctx->r25 == 0) {
        // 0x8016BBE4: addiu       $s0, $s0, 0x8
        ctx->r16 = ADD32(ctx->r16, 0X8);
            goto L_8016BC10;
    }
    goto skip_1;
    // 0x8016BBE4: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    skip_1:
    // 0x8016BBE8: lw          $t0, 0x18($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X18);
    // 0x8016BBEC: sw          $t0, 0x168($v1)
    MEM_W(0X168, ctx->r3) = ctx->r8;
    // 0x8016BBF0: lw          $t1, 0x0($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X0);
    // 0x8016BBF4: lh          $a0, 0x1C($t1)
    ctx->r4 = MEM_H(ctx->r9, 0X1C);
    // 0x8016BBF8: beql        $a0, $zero, L_8016BC10
    if (ctx->r4 == 0) {
        // 0x8016BBFC: addiu       $s0, $s0, 0x8
        ctx->r16 = ADD32(ctx->r16, 0X8);
            goto L_8016BC10;
    }
    goto skip_2;
    // 0x8016BBFC: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    skip_2:
    // 0x8016BC00: sw          $a0, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r4;
    // 0x8016BC04: jal         0x8016BEC0
    // 0x8016BC08: sw          $s7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r23;
    func_8016BEC0(rdram, ctx);
        goto after_0;
    // 0x8016BC08: sw          $s7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r23;
    after_0:
    // 0x8016BC0C: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
L_8016BC10:
    // 0x8016BC10: bne         $s0, $s2, L_8016BBC0
    if (ctx->r16 != ctx->r18) {
        // 0x8016BC14: addiu       $s1, $s1, 0x8
        ctx->r17 = ADD32(ctx->r17, 0X8);
            goto L_8016BBC0;
    }
    // 0x8016BC14: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
    // 0x8016BC18: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x8016BC1C: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016BC20: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016BC24: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8016BC28: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x8016BC2C: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x8016BC30: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x8016BC34: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x8016BC38: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x8016BC3C: jr          $ra
    // 0x8016BC40: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x8016BC40: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_8016BC44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016BC44 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016BC44++;
    if (lod_text_guard_depth_8016BC44 > 128) {
        if (!lod_text_guard_logged_8016BC44) {
            lod_text_guard_logged_8016BC44 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016BC44 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016BC44--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016BC44: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8016BC48: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x8016BC4C: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016BC50: lw          $t7, -0x10F4($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X10F4);
    // 0x8016BC54: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8016BC58: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8016BC5C: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x8016BC60: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016BC64: addu        $t1, $t6, $t7
    ctx->r9 = ADD32(ctx->r14, ctx->r15);
    // 0x8016BC68: lh          $v1, 0x0($t1)
    ctx->r3 = MEM_H(ctx->r9, 0X0);
    // 0x8016BC6C: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016BC70: bnel        $v1, $at, L_8016BDA0
    if (ctx->r3 != ctx->r1) {
        // 0x8016BC74: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_8016BDA0;
    }
    goto skip_0;
    // 0x8016BC74: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_0:
    // 0x8016BC78: lh          $t0, 0x2($t1)
    ctx->r8 = MEM_H(ctx->r9, 0X2);
    // 0x8016BC7C: lw          $a1, 0x4($t1)
    ctx->r5 = MEM_W(ctx->r9, 0X4);
    // 0x8016BC80: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8016BC84: blez        $t0, L_8016BEB0
    if (SIGNED(ctx->r8) <= 0) {
        // 0x8016BC88: lui         $t5, 0x801A
        ctx->r13 = S32(0X801A << 16);
            goto L_8016BEB0;
    }
    // 0x8016BC88: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8016BC8C: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x8016BC90: lui         $t2, 0x801A
    ctx->r10 = S32(0X801A << 16);
    // 0x8016BC94: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016BC98: addiu       $a3, $a3, -0x1118
    ctx->r7 = ADD32(ctx->r7, -0X1118);
    // 0x8016BC9C: addiu       $t2, $t2, -0x978
    ctx->r10 = ADD32(ctx->r10, -0X978);
    // 0x8016BCA0: addiu       $t4, $t4, 0x2494
    ctx->r12 = ADD32(ctx->r12, 0X2494);
    // 0x8016BCA4: addiu       $t5, $t5, -0x9FC
    ctx->r13 = ADD32(ctx->r13, -0X9FC);
    // 0x8016BCA8: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
L_8016BCAC:
    // 0x8016BCAC: lw          $v1, 0x0($t5)
    ctx->r3 = MEM_W(ctx->r13, 0X0);
    // 0x8016BCB0: lh          $a0, 0x0($a1)
    ctx->r4 = MEM_H(ctx->r5, 0X0);
    // 0x8016BCB4: addiu       $a2, $a2, 0x1
    ctx->r6 = ADD32(ctx->r6, 0X1);
    // 0x8016BCB8: lw          $t9, 0x168($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X168);
    // 0x8016BCBC: sll         $t8, $a0, 6
    ctx->r24 = S32(ctx->r4 << 6);
    // 0x8016BCC0: lwc1        $f0, 0x74($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X74);
    // 0x8016BCC4: addu        $v0, $t8, $t9
    ctx->r2 = ADD32(ctx->r24, ctx->r25);
    // 0x8016BCC8: lh          $t6, 0x30($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X30);
    // 0x8016BCCC: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x8016BCD0: nop

    // 0x8016BCD4: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016BCD8: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x8016BCDC: nop

    // 0x8016BCE0: bc1tl       L_8016BD8C
    if (c1cs) {
        // 0x8016BCE4: slt         $at, $a2, $t0
        ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
            goto L_8016BD8C;
    }
    goto skip_1;
    // 0x8016BCE4: slt         $at, $a2, $t0
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
    skip_1:
    // 0x8016BCE8: lh          $t7, 0x32($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X32);
    // 0x8016BCEC: mtc1        $t7, $f8
    ctx->f8.u32l = ctx->r15;
    // 0x8016BCF0: nop

    // 0x8016BCF4: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016BCF8: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x8016BCFC: nop

    // 0x8016BD00: bc1tl       L_8016BD8C
    if (c1cs) {
        // 0x8016BD04: slt         $at, $a2, $t0
        ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
            goto L_8016BD8C;
    }
    goto skip_2;
    // 0x8016BD04: slt         $at, $a2, $t0
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
    skip_2:
    // 0x8016BD08: lh          $t8, 0x34($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X34);
    // 0x8016BD0C: lwc1        $f0, 0x7C($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X7C);
    // 0x8016BD10: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016BD14: nop

    // 0x8016BD18: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016BD1C: c.lt.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl < ctx->f18.fl;
    // 0x8016BD20: nop

    // 0x8016BD24: bc1tl       L_8016BD8C
    if (c1cs) {
        // 0x8016BD28: slt         $at, $a2, $t0
        ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
            goto L_8016BD8C;
    }
    goto skip_3;
    // 0x8016BD28: slt         $at, $a2, $t0
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
    skip_3:
    // 0x8016BD2C: lh          $t9, 0x36($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X36);
    // 0x8016BD30: addu        $v0, $t2, $a0
    ctx->r2 = ADD32(ctx->r10, ctx->r4);
    // 0x8016BD34: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x8016BD38: nop

    // 0x8016BD3C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016BD40: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x8016BD44: nop

    // 0x8016BD48: bc1tl       L_8016BD8C
    if (c1cs) {
        // 0x8016BD4C: slt         $at, $a2, $t0
        ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
            goto L_8016BD8C;
    }
    goto skip_4;
    // 0x8016BD4C: slt         $at, $a2, $t0
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
    skip_4:
    // 0x8016BD50: lbu         $t6, 0x0($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X0);
    // 0x8016BD54: bnel        $t6, $zero, L_8016BD8C
    if (ctx->r14 != 0) {
        // 0x8016BD58: slt         $at, $a2, $t0
        ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
            goto L_8016BD8C;
    }
    goto skip_5;
    // 0x8016BD58: slt         $at, $a2, $t0
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
    skip_5:
    // 0x8016BD5C: lw          $t9, 0x0($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X0);
    // 0x8016BD60: lw          $t8, 0x0($t4)
    ctx->r24 = MEM_W(ctx->r12, 0X0);
    // 0x8016BD64: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x8016BD68: lh          $t7, 0x0($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X0);
    // 0x8016BD6C: sll         $t6, $t9, 1
    ctx->r14 = S32(ctx->r25 << 1);
    // 0x8016BD70: addu        $t9, $t8, $t6
    ctx->r25 = ADD32(ctx->r24, ctx->r14);
    // 0x8016BD74: sh          $t7, 0x0($t9)
    MEM_H(0X0, ctx->r25) = ctx->r15;
    // 0x8016BD78: lw          $t8, 0x0($a3)
    ctx->r24 = MEM_W(ctx->r7, 0X0);
    // 0x8016BD7C: addiu       $t6, $t8, 0x1
    ctx->r14 = ADD32(ctx->r24, 0X1);
    // 0x8016BD80: sw          $t6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r14;
    // 0x8016BD84: lh          $t0, 0x2($t1)
    ctx->r8 = MEM_H(ctx->r9, 0X2);
    // 0x8016BD88: slt         $at, $a2, $t0
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r8) ? 1 : 0;
L_8016BD8C:
    // 0x8016BD8C: bne         $at, $zero, L_8016BCAC
    if (ctx->r1 != 0) {
        // 0x8016BD90: addiu       $a1, $a1, 0x2
        ctx->r5 = ADD32(ctx->r5, 0X2);
            goto L_8016BCAC;
    }
    // 0x8016BD90: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x8016BD94: b           L_8016BEB4
    // 0x8016BD98: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8016BEB4;
    // 0x8016BD98: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8016BD9C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
L_8016BDA0:
    // 0x8016BDA0: beq         $v1, $at, L_8016BDD8
    if (ctx->r3 == ctx->r1) {
        // 0x8016BDA4: nop
    
            goto L_8016BDD8;
    }
    // 0x8016BDA4: nop

    // 0x8016BDA8: lh          $t8, 0x2($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X2);
    // 0x8016BDAC: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8016BDB0: addiu       $t5, $t5, -0x9FC
    ctx->r13 = ADD32(ctx->r13, -0X9FC);
    // 0x8016BDB4: mtc1        $t8, $f8
    ctx->f8.u32l = ctx->r24;
    // 0x8016BDB8: lw          $t7, 0x0($t5)
    ctx->r15 = MEM_W(ctx->r13, 0X0);
    // 0x8016BDBC: sll         $t9, $v1, 2
    ctx->r25 = S32(ctx->r3 << 2);
    // 0x8016BDC0: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016BDC4: addu        $v0, $t7, $t9
    ctx->r2 = ADD32(ctx->r15, ctx->r25);
    // 0x8016BDC8: lwc1        $f10, 0x74($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X74);
    // 0x8016BDCC: c.le.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl <= ctx->f10.fl;
    // 0x8016BDD0: nop

    // 0x8016BDD4: bc1f        L_8016BE54
    if (!c1cs) {
        // 0x8016BDD8: lui         $t5, 0x801A
        ctx->r13 = S32(0X801A << 16);
            goto L_8016BE54;
    }
L_8016BDD8:
    // 0x8016BDD8: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8016BDDC: addiu       $t5, $t5, -0x9FC
    ctx->r13 = ADD32(ctx->r13, -0X9FC);
    // 0x8016BDE0: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x8016BDE4: sll         $t7, $v1, 2
    ctx->r15 = S32(ctx->r3 << 2);
    // 0x8016BDE8: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
    // 0x8016BDEC: lwc1        $f16, 0x5C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X5C);
    // 0x8016BDF0: swc1        $f16, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f16.u32l;
    // 0x8016BDF4: lh          $t9, 0x2($t1)
    ctx->r25 = MEM_H(ctx->r9, 0X2);
    // 0x8016BDF8: mtc1        $t9, $f18
    ctx->f18.u32l = ctx->r25;
    // 0x8016BDFC: nop

    // 0x8016BE00: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016BE04: swc1        $f4, 0x5C($v0)
    MEM_W(0X5C, ctx->r2) = ctx->f4.u32l;
    // 0x8016BE08: lh          $a0, 0x4($t1)
    ctx->r4 = MEM_H(ctx->r9, 0X4);
    // 0x8016BE0C: jal         0x8016BC44
    // 0x8016BE10: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    func_8016BC44(rdram, ctx);
        goto after_0;
    // 0x8016BE10: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    after_0:
    // 0x8016BE14: lw          $t1, 0x1C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X1C);
    // 0x8016BE18: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8016BE1C: addiu       $t5, $t5, -0x9FC
    ctx->r13 = ADD32(ctx->r13, -0X9FC);
    // 0x8016BE20: lh          $t6, 0x0($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X0);
    // 0x8016BE24: lw          $t8, 0x0($t5)
    ctx->r24 = MEM_W(ctx->r13, 0X0);
    // 0x8016BE28: lwc1        $f6, 0x20($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X20);
    // 0x8016BE2C: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x8016BE30: addu        $t9, $t8, $t7
    ctx->r25 = ADD32(ctx->r24, ctx->r15);
    // 0x8016BE34: swc1        $f6, 0x5C($t9)
    MEM_W(0X5C, ctx->r25) = ctx->f6.u32l;
    // 0x8016BE38: lh          $t7, 0x2($t1)
    ctx->r15 = MEM_H(ctx->r9, 0X2);
    // 0x8016BE3C: lh          $v1, 0x0($t1)
    ctx->r3 = MEM_H(ctx->r9, 0X0);
    // 0x8016BE40: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x8016BE44: mtc1        $t7, $f8
    ctx->f8.u32l = ctx->r15;
    // 0x8016BE48: sll         $t8, $v1, 2
    ctx->r24 = S32(ctx->r3 << 2);
    // 0x8016BE4C: addu        $v0, $t6, $t8
    ctx->r2 = ADD32(ctx->r14, ctx->r24);
    // 0x8016BE50: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
L_8016BE54:
    // 0x8016BE54: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8016BE58: beql        $v1, $at, L_8016BE78
    if (ctx->r3 == ctx->r1) {
        // 0x8016BE5C: lwc1        $f16, 0x68($v0)
        ctx->f16.u32l = MEM_W(ctx->r2, 0X68);
            goto L_8016BE78;
    }
    goto skip_6;
    // 0x8016BE5C: lwc1        $f16, 0x68($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X68);
    skip_6:
    // 0x8016BE60: lwc1        $f10, 0x74($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X74);
    // 0x8016BE64: c.le.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl <= ctx->f0.fl;
    // 0x8016BE68: nop

    // 0x8016BE6C: bc1fl       L_8016BEB4
    if (!c1cs) {
        // 0x8016BE70: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8016BEB4;
    }
    goto skip_7;
    // 0x8016BE70: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_7:
    // 0x8016BE74: lwc1        $f16, 0x68($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X68);
L_8016BE78:
    // 0x8016BE78: swc1        $f16, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f16.u32l;
    // 0x8016BE7C: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x8016BE80: lh          $a0, 0x6($t1)
    ctx->r4 = MEM_H(ctx->r9, 0X6);
    // 0x8016BE84: jal         0x8016BC44
    // 0x8016BE88: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    func_8016BC44(rdram, ctx);
        goto after_1;
    // 0x8016BE88: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    after_1:
    // 0x8016BE8C: lw          $t1, 0x1C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X1C);
    // 0x8016BE90: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8016BE94: addiu       $t5, $t5, -0x9FC
    ctx->r13 = ADD32(ctx->r13, -0X9FC);
    // 0x8016BE98: lh          $t6, 0x0($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X0);
    // 0x8016BE9C: lw          $t9, 0x0($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X0);
    // 0x8016BEA0: lwc1        $f18, 0x20($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X20);
    // 0x8016BEA4: sll         $t8, $t6, 2
    ctx->r24 = S32(ctx->r14 << 2);
    // 0x8016BEA8: addu        $t7, $t9, $t8
    ctx->r15 = ADD32(ctx->r25, ctx->r24);
    // 0x8016BEAC: swc1        $f18, 0x68($t7)
    MEM_W(0X68, ctx->r15) = ctx->f18.u32l;
L_8016BEB0:
    // 0x8016BEB0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8016BEB4:
    // 0x8016BEB4: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8016BEB8: jr          $ra
    // 0x8016BEBC: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016BC44--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016BEBC: nop

;}
RECOMP_FUNC void func_8016BEC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016BEC0: addiu       $sp, $sp, -0xD8
    ctx->r29 = ADD32(ctx->r29, -0XD8);
    // 0x8016BEC4: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8016BEC8: lw          $t6, -0x1118($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X1118);
    // 0x8016BECC: sw          $s7, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r23;
    // 0x8016BED0: sw          $s5, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r21;
    // 0x8016BED4: sw          $ra, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r31;
    // 0x8016BED8: sw          $fp, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r30;
    // 0x8016BEDC: sw          $s6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r22;
    // 0x8016BEE0: sw          $s4, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r20;
    // 0x8016BEE4: sw          $s3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r19;
    // 0x8016BEE8: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x8016BEEC: sw          $s1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r17;
    // 0x8016BEF0: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x8016BEF4: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x8016BEF8: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x8016BEFC: or          $s7, $zero, $zero
    ctx->r23 = 0 | 0;
    // 0x8016BF00: blez        $t6, L_8016C15C
    if (SIGNED(ctx->r14) <= 0) {
        // 0x8016BF04: or          $s5, $zero, $zero
        ctx->r21 = 0 | 0;
            goto L_8016C15C;
    }
    // 0x8016BF04: or          $s5, $zero, $zero
    ctx->r21 = 0 | 0;
    // 0x8016BF08: lui         $fp, 0x8019
    ctx->r30 = S32(0X8019 << 16);
    // 0x8016BF0C: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x8016BF10: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x8016BF14: addiu       $s1, $s1, -0x9FC
    ctx->r17 = ADD32(ctx->r17, -0X9FC);
    // 0x8016BF18: addiu       $fp, $fp, 0x2494
    ctx->r30 = ADD32(ctx->r30, 0X2494);
    // 0x8016BF1C: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
    // 0x8016BF20: addiu       $s4, $sp, 0xAC
    ctx->r20 = ADD32(ctx->r29, 0XAC);
    // 0x8016BF24: addiu       $s2, $sp, 0xA0
    ctx->r18 = ADD32(ctx->r29, 0XA0);
    // 0x8016BF28: lw          $t7, 0x0($fp)
    ctx->r15 = MEM_W(ctx->r30, 0X0);
L_8016BF2C:
    // 0x8016BF2C: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8016BF30: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x8016BF34: addu        $t8, $t7, $s6
    ctx->r24 = ADD32(ctx->r15, ctx->r22);
    // 0x8016BF38: lh          $t9, 0x0($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X0);
    // 0x8016BF3C: lw          $t1, 0x168($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X168);
    // 0x8016BF40: or          $a2, $s4, $zero
    ctx->r6 = ctx->r20 | 0;
    // 0x8016BF44: sll         $t0, $t9, 6
    ctx->r8 = S32(ctx->r25 << 6);
    // 0x8016BF48: addu        $s3, $t0, $t1
    ctx->r19 = ADD32(ctx->r8, ctx->r9);
    // 0x8016BF4C: lh          $t2, 0x28($s3)
    ctx->r10 = MEM_H(ctx->r19, 0X28);
    // 0x8016BF50: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x8016BF54: addiu       $a1, $s0, 0x74
    ctx->r5 = ADD32(ctx->r16, 0X74);
    // 0x8016BF58: bne         $t2, $at, L_8016BF80
    if (ctx->r10 != ctx->r1) {
        // 0x8016BF5C: nop
    
            goto L_8016BF80;
    }
    // 0x8016BF5C: nop

    // 0x8016BF60: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x8016BF64: jal         0x8016C338
    // 0x8016BF68: addiu       $a1, $s0, 0x74
    ctx->r5 = ADD32(ctx->r16, 0X74);
    func_8016C338(rdram, ctx);
        goto after_0;
    // 0x8016BF68: addiu       $a1, $s0, 0x74
    ctx->r5 = ADD32(ctx->r16, 0X74);
    after_0:
    // 0x8016BF6C: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8016BF70: lw          $t3, 0x6EC($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X6EC);
    // 0x8016BF74: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x8016BF78: b           L_8016BF98
    // 0x8016BF7C: sw          $t4, 0x6EC($s0)
    MEM_W(0X6EC, ctx->r16) = ctx->r12;
        goto L_8016BF98;
    // 0x8016BF7C: sw          $t4, 0x6EC($s0)
    MEM_W(0X6EC, ctx->r16) = ctx->r12;
L_8016BF80:
    // 0x8016BF80: jal         0x8016C714
    // 0x8016BF84: or          $a2, $s4, $zero
    ctx->r6 = ctx->r20 | 0;
    func_8016C714(rdram, ctx);
        goto after_1;
    // 0x8016BF84: or          $a2, $s4, $zero
    ctx->r6 = ctx->r20 | 0;
    after_1:
    // 0x8016BF88: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8016BF8C: lw          $t5, 0x6F0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X6F0);
    // 0x8016BF90: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x8016BF94: sw          $t6, 0x6F0($s0)
    MEM_W(0X6F0, ctx->r16) = ctx->r14;
L_8016BF98:
    // 0x8016BF98: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8016BF9C: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    // 0x8016BFA0: lw          $t7, 0x6E8($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X6E8);
    // 0x8016BFA4: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8016BFA8: bne         $v0, $zero, L_8016C140
    if (ctx->r2 != 0) {
        // 0x8016BFAC: sw          $t8, 0x6E8($s0)
        MEM_W(0X6E8, ctx->r16) = ctx->r24;
            goto L_8016C140;
    }
    // 0x8016BFAC: sw          $t8, 0x6E8($s0)
    MEM_W(0X6E8, ctx->r16) = ctx->r24;
    // 0x8016BFB0: lw          $a0, 0x0($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X0);
    // 0x8016BFB4: addiu       $a2, $sp, 0x94
    ctx->r6 = ADD32(ctx->r29, 0X94);
    // 0x8016BFB8: jal         0x80173524
    // 0x8016BFBC: addiu       $a0, $a0, 0x1E4
    ctx->r4 = ADD32(ctx->r4, 0X1E4);
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x8016BFBC: addiu       $a0, $a0, 0x1E4
    ctx->r4 = ADD32(ctx->r4, 0X1E4);
    after_2:
    // 0x8016BFC0: lw          $t9, 0x0($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X0);
    // 0x8016BFC4: lwc1        $f8, 0x98($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X98);
    // 0x8016BFC8: lbu         $a0, 0x2B($s3)
    ctx->r4 = MEM_BU(ctx->r19, 0X2B);
    // 0x8016BFCC: lw          $t0, 0x0($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X0);
    // 0x8016BFD0: lh          $t1, 0x2($t0)
    ctx->r9 = MEM_H(ctx->r8, 0X2);
    // 0x8016BFD4: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x8016BFD8: nop

    // 0x8016BFDC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016BFE0: jal         0x80166F40
    // 0x8016BFE4: add.s       $f20, $f6, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = ctx->f6.fl + ctx->f8.fl;
    func_80166F40(rdram, ctx);
        goto after_3;
    // 0x8016BFE4: add.s       $f20, $f6, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = ctx->f6.fl + ctx->f8.fl;
    after_3:
    // 0x8016BFE8: bne         $v0, $zero, L_8016BFFC
    if (ctx->r2 != 0) {
        // 0x8016BFEC: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_8016BFFC;
    }
    // 0x8016BFEC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016BFF0: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8016BFF4: b           L_8016C004
    // 0x8016BFF8: addiu       $v0, $s0, 0x230
    ctx->r2 = ADD32(ctx->r16, 0X230);
        goto L_8016C004;
    // 0x8016BFF8: addiu       $v0, $s0, 0x230
    ctx->r2 = ADD32(ctx->r16, 0X230);
L_8016BFFC:
    // 0x8016BFFC: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8016C000: addiu       $v0, $s0, 0x48C
    ctx->r2 = ADD32(ctx->r16, 0X48C);
L_8016C004:
    // 0x8016C004: lwc1        $f10, 0x18($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016C008: sub.s       $f2, $f20, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f20.fl - ctx->f10.fl;
    // 0x8016C00C: c.lt.s      $f22, $f2
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f22.fl < ctx->f2.fl;
    // 0x8016C010: nop

    // 0x8016C014: bc1fl       L_8016C044
    if (!c1cs) {
        // 0x8016C018: lbu         $t3, 0x2A($s3)
        ctx->r11 = MEM_BU(ctx->r19, 0X2A);
            goto L_8016C044;
    }
    goto skip_0;
    // 0x8016C018: lbu         $t3, 0x2A($s3)
    ctx->r11 = MEM_BU(ctx->r19, 0X2A);
    skip_0:
    // 0x8016C01C: lbu         $t2, 0x2A($s3)
    ctx->r10 = MEM_BU(ctx->r19, 0X2A);
    // 0x8016C020: addiu       $s0, $v0, 0xC8
    ctx->r16 = ADD32(ctx->r2, 0XC8);
    // 0x8016C024: bne         $t2, $zero, L_8016C038
    if (ctx->r10 != 0) {
        // 0x8016C028: nop
    
            goto L_8016C038;
    }
    // 0x8016C028: nop

    // 0x8016C02C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x8016C030: b           L_8016C05C
    // 0x8016C034: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
        goto L_8016C05C;
    // 0x8016C034: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_8016C038:
    // 0x8016C038: b           L_8016C05C
    // 0x8016C03C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
        goto L_8016C05C;
    // 0x8016C03C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    // 0x8016C040: lbu         $t3, 0x2A($s3)
    ctx->r11 = MEM_BU(ctx->r19, 0X2A);
L_8016C044:
    // 0x8016C044: addiu       $s0, $v0, 0x12C
    ctx->r16 = ADD32(ctx->r2, 0X12C);
    // 0x8016C048: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x8016C04C: bne         $t3, $zero, L_8016C05C
    if (ctx->r11 != 0) {
        // 0x8016C050: nop
    
            goto L_8016C05C;
    }
    // 0x8016C050: nop

    // 0x8016C054: b           L_8016C05C
    // 0x8016C058: addiu       $s0, $v0, 0x64
    ctx->r16 = ADD32(ctx->r2, 0X64);
        goto L_8016C05C;
    // 0x8016C058: addiu       $s0, $v0, 0x64
    ctx->r16 = ADD32(ctx->r2, 0X64);
L_8016C05C:
    // 0x8016C05C: lh          $t4, 0x60($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X60);
    // 0x8016C060: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x8016C064: addiu       $v0, $sp, 0x94
    ctx->r2 = ADD32(ctx->r29, 0X94);
    // 0x8016C068: beql        $t4, $zero, L_8016C088
    if (ctx->r12 == 0) {
        // 0x8016C06C: addiu       $s7, $zero, 0x1
        ctx->r23 = ADD32(0, 0X1);
            goto L_8016C088;
    }
    goto skip_1;
    // 0x8016C06C: addiu       $s7, $zero, 0x1
    ctx->r23 = ADD32(0, 0X1);
    skip_1:
    // 0x8016C070: lwc1        $f16, 0x18($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016C074: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x8016C078: nop

    // 0x8016C07C: bc1f        L_8016C140
    if (!c1cs) {
        // 0x8016C080: nop
    
            goto L_8016C140;
    }
    // 0x8016C080: nop

    // 0x8016C084: addiu       $s7, $zero, 0x1
    ctx->r23 = ADD32(0, 0X1);
L_8016C088:
    // 0x8016C088: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016C08C: beq         $v0, $s2, L_8016C0CC
    if (ctx->r2 == ctx->r18) {
        // 0x8016C090: lw          $t5, 0x0($s1)
        ctx->r13 = MEM_W(ctx->r17, 0X0);
            goto L_8016C0CC;
    }
    // 0x8016C090: lw          $t5, 0x0($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X0);
L_8016C094:
    // 0x8016C094: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x8016C098: lwc1        $f20, -0x4($v0)
    ctx->f20.u32l = MEM_W(ctx->r2, -0X4);
    // 0x8016C09C: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016C0A0: addu        $t7, $t6, $a0
    ctx->r15 = ADD32(ctx->r14, ctx->r4);
    // 0x8016C0A4: lh          $t8, 0x0($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X0);
    // 0x8016C0A8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C0AC: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x8016C0B0: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016C0B4: nop

    // 0x8016C0B8: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016C0BC: add.s       $f20, $f18, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = ctx->f18.fl + ctx->f20.fl;
    // 0x8016C0C0: swc1        $f20, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f20.u32l;
    // 0x8016C0C4: bne         $v0, $s2, L_8016C094
    if (ctx->r2 != ctx->r18) {
        // 0x8016C0C8: lw          $t5, 0x0($s1)
        ctx->r13 = MEM_W(ctx->r17, 0X0);
            goto L_8016C094;
    }
    // 0x8016C0C8: lw          $t5, 0x0($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X0);
L_8016C0CC:
    // 0x8016C0CC: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x8016C0D0: lwc1        $f20, -0x4($v0)
    ctx->f20.u32l = MEM_W(ctx->r2, -0X4);
    // 0x8016C0D4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C0D8: addu        $t7, $t6, $a0
    ctx->r15 = ADD32(ctx->r14, ctx->r4);
    // 0x8016C0DC: lh          $t8, 0x0($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X0);
    // 0x8016C0E0: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x8016C0E4: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016C0E8: nop

    // 0x8016C0EC: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016C0F0: add.s       $f20, $f18, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = ctx->f18.fl + ctx->f20.fl;
    // 0x8016C0F4: swc1        $f20, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f20.u32l;
    // 0x8016C0F8: swc1        $f22, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f22.u32l;
    // 0x8016C0FC: swc1        $f22, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f22.u32l;
    // 0x8016C100: swc1        $f2, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f2.u32l;
    // 0x8016C104: swc1        $f0, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f0.u32l;
    // 0x8016C108: sw          $s3, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r19;
    // 0x8016C10C: lw          $t9, 0x0($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X0);
    // 0x8016C110: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x8016C114: addiu       $a1, $s0, 0x48
    ctx->r5 = ADD32(ctx->r16, 0X48);
    // 0x8016C118: lw          $t0, 0x8($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X8);
    // 0x8016C11C: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x8016C120: sw          $t0, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r8;
    // 0x8016C124: lw          $t1, 0x0($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X0);
    // 0x8016C128: lw          $t2, 0x0($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X0);
    // 0x8016C12C: lh          $t3, 0x8($t2)
    ctx->r11 = MEM_H(ctx->r10, 0X8);
    // 0x8016C130: jal         0x80001090
    // 0x8016C134: sh          $t3, 0x5C($s0)
    MEM_H(0X5C, ctx->r16) = ctx->r11;
    memory_copy(rdram, ctx);
        goto after_4;
    // 0x8016C134: sh          $t3, 0x5C($s0)
    MEM_H(0X5C, ctx->r16) = ctx->r11;
    after_4:
    // 0x8016C138: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x8016C13C: sh          $t4, 0x60($s0)
    MEM_H(0X60, ctx->r16) = ctx->r12;
L_8016C140:
    // 0x8016C140: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8016C144: lw          $t5, -0x1118($t5)
    ctx->r13 = MEM_W(ctx->r13, -0X1118);
    // 0x8016C148: addiu       $s5, $s5, 0x1
    ctx->r21 = ADD32(ctx->r21, 0X1);
    // 0x8016C14C: addiu       $s6, $s6, 0x2
    ctx->r22 = ADD32(ctx->r22, 0X2);
    // 0x8016C150: slt         $at, $s5, $t5
    ctx->r1 = SIGNED(ctx->r21) < SIGNED(ctx->r13) ? 1 : 0;
    // 0x8016C154: bnel        $at, $zero, L_8016BF2C
    if (ctx->r1 != 0) {
        // 0x8016C158: lw          $t7, 0x0($fp)
        ctx->r15 = MEM_W(ctx->r30, 0X0);
            goto L_8016BF2C;
    }
    goto skip_2;
    // 0x8016C158: lw          $t7, 0x0($fp)
    ctx->r15 = MEM_W(ctx->r30, 0X0);
    skip_2:
L_8016C15C:
    // 0x8016C15C: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
    // 0x8016C160: or          $v0, $s7, $zero
    ctx->r2 = ctx->r23 | 0;
    // 0x8016C164: lw          $s7, 0x44($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X44);
    // 0x8016C168: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x8016C16C: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x8016C170: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x8016C174: lw          $s1, 0x2C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X2C);
    // 0x8016C178: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x8016C17C: lw          $s3, 0x34($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X34);
    // 0x8016C180: lw          $s4, 0x38($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X38);
    // 0x8016C184: lw          $s5, 0x3C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X3C);
    // 0x8016C188: lw          $s6, 0x40($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X40);
    // 0x8016C18C: lw          $fp, 0x48($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X48);
    // 0x8016C190: jr          $ra
    // 0x8016C194: addiu       $sp, $sp, 0xD8
    ctx->r29 = ADD32(ctx->r29, 0XD8);
    return;
    // 0x8016C194: addiu       $sp, $sp, 0xD8
    ctx->r29 = ADD32(ctx->r29, 0XD8);
;}
RECOMP_FUNC void func_8016C198(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016C198: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x8016C19C: addiu       $t0, $t0, -0x9FC
    ctx->r8 = ADD32(ctx->r8, -0X9FC);
    // 0x8016C1A0: sw          $a0, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r4;
    // 0x8016C1A4: sw          $zero, 0x22C($a0)
    MEM_W(0X22C, ctx->r4) = 0;
    // 0x8016C1A8: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1AC: sw          $zero, 0x488($t7)
    MEM_W(0X488, ctx->r15) = 0;
    // 0x8016C1B0: lw          $t8, 0x0($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1B4: sw          $zero, 0x6F0($t8)
    MEM_W(0X6F0, ctx->r24) = 0;
    // 0x8016C1B8: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1BC: lw          $v1, 0x6F0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X6F0);
    // 0x8016C1C0: sw          $v1, 0x6EC($v0)
    MEM_W(0X6EC, ctx->r2) = ctx->r3;
    // 0x8016C1C4: lw          $t9, 0x0($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1C8: sw          $v1, 0x6E8($t9)
    MEM_W(0X6E8, ctx->r25) = ctx->r3;
    // 0x8016C1CC: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1D0: sw          $a1, 0x4($t1)
    MEM_W(0X4, ctx->r9) = ctx->r5;
    // 0x8016C1D4: lw          $t2, 0x0($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1D8: sh          $zero, 0x3BC($t2)
    MEM_H(0X3BC, ctx->r10) = 0;
    // 0x8016C1DC: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1E0: lh          $a3, 0x3BC($v0)
    ctx->r7 = MEM_H(ctx->r2, 0X3BC);
    // 0x8016C1E4: sh          $a3, 0x358($v0)
    MEM_H(0X358, ctx->r2) = ctx->r7;
    // 0x8016C1E8: lw          $t3, 0x0($t0)
    ctx->r11 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1EC: sh          $a3, 0x2F4($t3)
    MEM_H(0X2F4, ctx->r11) = ctx->r7;
    // 0x8016C1F0: lw          $t4, 0x0($t0)
    ctx->r12 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1F4: sh          $a3, 0x290($t4)
    MEM_H(0X290, ctx->r12) = ctx->r7;
    // 0x8016C1F8: lw          $t5, 0x0($t0)
    ctx->r13 = MEM_W(ctx->r8, 0X0);
    // 0x8016C1FC: sh          $zero, 0x618($t5)
    MEM_H(0X618, ctx->r13) = 0;
    // 0x8016C200: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x8016C204: lh          $a3, 0x618($v0)
    ctx->r7 = MEM_H(ctx->r2, 0X618);
    // 0x8016C208: sh          $a3, 0x5B4($v0)
    MEM_H(0X5B4, ctx->r2) = ctx->r7;
    // 0x8016C20C: lw          $t6, 0x0($t0)
    ctx->r14 = MEM_W(ctx->r8, 0X0);
    // 0x8016C210: sh          $a3, 0x550($t6)
    MEM_H(0X550, ctx->r14) = ctx->r7;
    // 0x8016C214: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x8016C218: sh          $a3, 0x4EC($t7)
    MEM_H(0X4EC, ctx->r15) = ctx->r7;
    // 0x8016C21C: lw          $t8, 0x0($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X0);
    // 0x8016C220: jr          $ra
    // 0x8016C224: sw          $a2, 0x6E4($t8)
    MEM_W(0X6E4, ctx->r24) = ctx->r6;
    return;
    // 0x8016C224: sw          $a2, 0x6E4($t8)
    MEM_W(0X6E4, ctx->r24) = ctx->r6;
;}
RECOMP_FUNC void func_8016C228(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016C228: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8016C22C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016C230: addiu       $v0, $v0, -0x9FC
    ctx->r2 = ADD32(ctx->r2, -0X9FC);
    // 0x8016C234: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    // 0x8016C238: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8016C23C:
    // 0x8016C23C: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x8016C240: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016C244: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016C248: addu        $t7, $t6, $v1
    ctx->r15 = ADD32(ctx->r14, ctx->r3);
    // 0x8016C24C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C250: bne         $v1, $a0, L_8016C23C
    if (ctx->r3 != ctx->r4) {
        // 0x8016C254: swc1        $f4, 0x14($t7)
        MEM_W(0X14, ctx->r15) = ctx->f4.u32l;
            goto L_8016C23C;
    }
    // 0x8016C254: swc1        $f4, 0x14($t7)
    MEM_W(0X14, ctx->r15) = ctx->f4.u32l;
    // 0x8016C258: jr          $ra
    // 0x8016C25C: nop

    return;
    // 0x8016C25C: nop

;}
RECOMP_FUNC void func_8016C260(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016C260: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016C264: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016C268: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x8016C26C: lw          $t7, 0x0($t1)
    ctx->r15 = MEM_W(ctx->r9, 0X0);
    // 0x8016C270: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016C274: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x8016C278: sw          $a1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r5;
    // 0x8016C27C: sw          $a0, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->r4;
    // 0x8016C280: lw          $t8, 0x0($t1)
    ctx->r24 = MEM_W(ctx->r9, 0X0);
    // 0x8016C284: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x8016C288: addiu       $v0, $sp, 0x2C
    ctx->r2 = ADD32(ctx->r29, 0X2C);
    // 0x8016C28C: sw          $a2, 0x8($t8)
    MEM_W(0X8, ctx->r24) = ctx->r6;
    // 0x8016C290: lw          $a3, 0x0($t1)
    ctx->r7 = MEM_W(ctx->r9, 0X0);
    // 0x8016C294: lw          $a1, 0x48($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X48);
    // 0x8016C298: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
    // 0x8016C29C: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016C2A0: beq         $v0, $a0, L_8016C2D0
    if (ctx->r2 == ctx->r4) {
        // 0x8016C2A4: lh          $t9, 0x0($a1)
        ctx->r25 = MEM_H(ctx->r5, 0X0);
            goto L_8016C2D0;
    }
    // 0x8016C2A4: lh          $t9, 0x0($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X0);
L_8016C2A8:
    // 0x8016C2A8: mtc1        $t9, $f18
    ctx->f18.u32l = ctx->r25;
    // 0x8016C2AC: lwc1        $f16, 0x14($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X14);
    // 0x8016C2B0: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016C2B4: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016C2B8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C2BC: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x8016C2C0: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016C2C4: swc1        $f18, -0x8($v0)
    MEM_W(-0X8, ctx->r2) = ctx->f18.u32l;
    // 0x8016C2C8: bne         $v0, $a0, L_8016C2A8
    if (ctx->r2 != ctx->r4) {
        // 0x8016C2CC: lh          $t9, 0x0($a1)
        ctx->r25 = MEM_H(ctx->r5, 0X0);
            goto L_8016C2A8;
    }
    // 0x8016C2CC: lh          $t9, 0x0($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X0);
L_8016C2D0:
    // 0x8016C2D0: mtc1        $t9, $f18
    ctx->f18.u32l = ctx->r25;
    // 0x8016C2D4: lwc1        $f16, 0x14($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X14);
    // 0x8016C2D8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C2DC: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016C2E0: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x8016C2E4: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016C2E8: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016C2EC: lw          $t2, 0x48($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X48);
    // 0x8016C2F0: addiu       $a0, $a3, 0x1E4
    ctx->r4 = ADD32(ctx->r7, 0X1E4);
    // 0x8016C2F4: jal         0x801739B8
    // 0x8016C2F8: lh          $a1, 0x8($t2)
    ctx->r5 = MEM_H(ctx->r10, 0X8);
    func_801739B8(rdram, ctx);
        goto after_0;
    // 0x8016C2F8: lh          $a1, 0x8($t2)
    ctx->r5 = MEM_H(ctx->r10, 0X8);
    after_0:
    // 0x8016C2FC: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016C300: lw          $a3, -0x9FC($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X9FC);
    // 0x8016C304: addiu       $a0, $a3, 0x208
    ctx->r4 = ADD32(ctx->r7, 0X208);
    // 0x8016C308: jal         0x80173BC0
    // 0x8016C30C: addiu       $a1, $a3, 0x1E4
    ctx->r5 = ADD32(ctx->r7, 0X1E4);
    func_80173BC0(rdram, ctx);
        goto after_1;
    // 0x8016C30C: addiu       $a1, $a3, 0x1E4
    ctx->r5 = ADD32(ctx->r7, 0X1E4);
    after_1:
    // 0x8016C310: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016C314: lw          $a3, -0x9FC($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X9FC);
    // 0x8016C318: addiu       $a1, $sp, 0x2C
    ctx->r5 = ADD32(ctx->r29, 0X2C);
    // 0x8016C31C: addiu       $a0, $a3, 0x208
    ctx->r4 = ADD32(ctx->r7, 0X208);
    // 0x8016C320: jal         0x80173524
    // 0x8016C324: addiu       $a2, $a3, 0x74
    ctx->r6 = ADD32(ctx->r7, 0X74);
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x8016C324: addiu       $a2, $a3, 0x74
    ctx->r6 = ADD32(ctx->r7, 0X74);
    after_2:
    // 0x8016C328: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8016C32C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x8016C330: jr          $ra
    // 0x8016C334: nop

    return;
    // 0x8016C334: nop

;}
RECOMP_FUNC void func_8016C338(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016C338: lwc1        $f2, 0x4($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016C33C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8016C340: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x8016C344: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016C348: c.lt.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl < ctx->f4.fl;
    // 0x8016C34C: nop

    // 0x8016C350: bc1fl       L_8016C364
    if (!c1cs) {
        // 0x8016C354: mov.s       $f12, $f2
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    ctx->f12.fl = ctx->f2.fl;
            goto L_8016C364;
    }
    goto skip_0;
    // 0x8016C354: mov.s       $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    ctx->f12.fl = ctx->f2.fl;
    skip_0:
    // 0x8016C358: b           L_8016C364
    // 0x8016C35C: neg.s       $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = -ctx->f2.fl;
        goto L_8016C364;
    // 0x8016C35C: neg.s       $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = -ctx->f2.fl;
    // 0x8016C360: mov.s       $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    ctx->f12.fl = ctx->f2.fl;
L_8016C364:
    // 0x8016C364: lwc1        $f10, -0x5144($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X5144);
    // 0x8016C368: c.lt.s      $f12, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f12.fl < ctx->f10.fl;
    // 0x8016C36C: nop

    // 0x8016C370: bc1fl       L_8016C384
    if (!c1cs) {
        // 0x8016C374: lwc1        $f8, 0x0($a0)
        ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
            goto L_8016C384;
    }
    goto skip_1;
    // 0x8016C374: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    skip_1:
    // 0x8016C378: b           L_8016C70C
    // 0x8016C37C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016C70C;
    // 0x8016C37C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016C380: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
L_8016C384:
    // 0x8016C384: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016C388: lwc1        $f10, 0x4($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8016C38C: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x8016C390: mul.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f6.fl);
    // 0x8016C394: or          $a3, $a2, $zero
    ctx->r7 = ctx->r6 | 0;
    // 0x8016C398: or          $t0, $a0, $zero
    ctx->r8 = ctx->r4 | 0;
    // 0x8016C39C: mul.s       $f8, $f2, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f10.fl);
    // 0x8016C3A0: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016C3A4: addiu       $t1, $sp, 0x44
    ctx->r9 = ADD32(ctx->r29, 0X44);
    // 0x8016C3A8: addiu       $t2, $sp, 0x38
    ctx->r10 = ADD32(ctx->r29, 0X38);
    // 0x8016C3AC: addiu       $t3, $sp, 0x2C
    ctx->r11 = ADD32(ctx->r29, 0X2C);
    // 0x8016C3B0: addiu       $v0, $sp, 0x38
    ctx->r2 = ADD32(ctx->r29, 0X38);
    // 0x8016C3B4: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x8016C3B8: lwc1        $f4, 0x8($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016C3BC: mul.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8016C3C0: lwc1        $f4, 0xC($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8016C3C4: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8016C3C8: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8016C3CC: neg.s       $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = -ctx->f6.fl;
    // 0x8016C3D0: div.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016C3D4: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016C3D8: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016C3DC: swc1        $f18, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f18.u32l;
    // 0x8016C3E0: beq         $t3, $v0, L_8016C450
    if (ctx->r11 == ctx->r2) {
        // 0x8016C3E4: lh          $t8, 0x10($t0)
        ctx->r24 = MEM_H(ctx->r8, 0X10);
            goto L_8016C450;
    }
    // 0x8016C3E4: lh          $t8, 0x10($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X10);
L_8016C3E8:
    // 0x8016C3E8: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016C3EC: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016C3F0: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C3F4: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016C3F8: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016C3FC: addiu       $t0, $t0, 0x2
    ctx->r8 = ADD32(ctx->r8, 0X2);
    // 0x8016C400: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016C404: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016C408: sub.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl - ctx->f16.fl;
    // 0x8016C40C: swc1        $f14, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->f14.u32l;
    // 0x8016C410: lh          $t9, 0x14($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X14);
    // 0x8016C414: mtc1        $t9, $f14
    ctx->f14.u32l = ctx->r25;
    // 0x8016C418: nop

    // 0x8016C41C: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C420: sub.s       $f14, $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C424: swc1        $f14, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f14.u32l;
    // 0x8016C428: lh          $t4, 0x1A($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X1A);
    // 0x8016C42C: mtc1        $t4, $f14
    ctx->f14.u32l = ctx->r12;
    // 0x8016C430: nop

    // 0x8016C434: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C438: sub.s       $f16, $f14, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C43C: swc1        $f16, -0x8($t3)
    MEM_W(-0X8, ctx->r11) = ctx->f16.u32l;
    // 0x8016C440: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016C444: swc1        $f18, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f18.u32l;
    // 0x8016C448: bne         $t3, $v0, L_8016C3E8
    if (ctx->r11 != ctx->r2) {
        // 0x8016C44C: lh          $t8, 0x10($t0)
        ctx->r24 = MEM_H(ctx->r8, 0X10);
            goto L_8016C3E8;
    }
    // 0x8016C44C: lh          $t8, 0x10($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X10);
L_8016C450:
    // 0x8016C450: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016C454: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C458: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016C45C: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016C460: addiu       $t0, $t0, 0x2
    ctx->r8 = ADD32(ctx->r8, 0X2);
    // 0x8016C464: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016C468: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016C46C: sub.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl - ctx->f16.fl;
    // 0x8016C470: swc1        $f14, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->f14.u32l;
    // 0x8016C474: lh          $t9, 0x14($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X14);
    // 0x8016C478: mtc1        $t9, $f14
    ctx->f14.u32l = ctx->r25;
    // 0x8016C47C: nop

    // 0x8016C480: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C484: sub.s       $f14, $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C488: swc1        $f14, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f14.u32l;
    // 0x8016C48C: lh          $t4, 0x1A($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X1A);
    // 0x8016C490: mtc1        $t4, $f14
    ctx->f14.u32l = ctx->r12;
    // 0x8016C494: nop

    // 0x8016C498: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C49C: sub.s       $f16, $f14, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C4A0: swc1        $f16, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f16.u32l;
    // 0x8016C4A4: lwc1        $f10, 0x4($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8016C4A8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016C4AC: add.s       $f4, $f10, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f0.fl;
    // 0x8016C4B0: swc1        $f4, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f4.u32l;
    // 0x8016C4B4: lwc1        $f6, 0x48($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016C4B8: lwc1        $f10, 0x44($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016C4BC: add.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f0.fl;
    // 0x8016C4C0: mul.s       $f4, $f10, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f10.fl);
    // 0x8016C4C4: nop

    // 0x8016C4C8: mul.s       $f6, $f8, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f8.fl);
    // 0x8016C4CC: swc1        $f8, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f8.u32l;
    // 0x8016C4D0: lwc1        $f8, 0x4C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016C4D4: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016C4D8: mul.s       $f4, $f8, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f8.fl);
    // 0x8016C4DC: lwc1        $f8, -0x5140($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X5140);
    // 0x8016C4E0: add.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x8016C4E4: lwc1        $f4, 0x38($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016C4E8: c.lt.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl < ctx->f8.fl;
    // 0x8016C4EC: nop

    // 0x8016C4F0: bc1f        L_8016C500
    if (!c1cs) {
        // 0x8016C4F4: nop
    
            goto L_8016C500;
    }
    // 0x8016C4F4: nop

    // 0x8016C4F8: b           L_8016C70C
    // 0x8016C4FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016C70C;
    // 0x8016C4FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016C500:
    // 0x8016C500: trunc.w.s   $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016C504: lwc1        $f8, 0x2C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016C508: lwc1        $f12, 0x34($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016C50C: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016C510: mfc1        $t6, $f10
    ctx->r14 = (int32_t)ctx->f10.u32l;
    // 0x8016C514: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8016C518: mtc1        $t6, $f6
    ctx->f6.u32l = ctx->r14;
    // 0x8016C51C: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    // 0x8016C520: cvt.s.w     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    ctx->f0.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016C524: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x8016C528: nop

    // 0x8016C52C: cvt.s.w     $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    ctx->f2.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016C530: mul.s       $f10, $f0, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x8016C534: nop

    // 0x8016C538: mul.s       $f6, $f14, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x8016C53C: sub.s       $f10, $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f10.fl - ctx->f6.fl;
    // 0x8016C540: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8016C544: mfc1        $v1, $f6
    ctx->r3 = (int32_t)ctx->f6.u32l;
    // 0x8016C548: nop

    // 0x8016C54C: beq         $v1, $zero, L_8016C5A8
    if (ctx->r3 == 0) {
        // 0x8016C550: nop
    
            goto L_8016C5A8;
    }
    // 0x8016C550: nop

    // 0x8016C554: mtc1        $v1, $f6
    ctx->f6.u32l = ctx->r3;
    // 0x8016C558: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016C55C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016C560: cvt.s.w     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016C564: lwc1        $f18, 0x44($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016C568: lwc1        $f16, 0x4C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016C56C: div.s       $f10, $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = DIV_S(ctx->f10.fl, ctx->f6.fl);
    // 0x8016C570: mul.s       $f6, $f12, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f18.fl);
    // 0x8016C574: nop

    // 0x8016C578: mul.s       $f8, $f16, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x8016C57C: sub.s       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x8016C580: swc1        $f10, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f10.u32l;
    // 0x8016C584: mul.s       $f2, $f6, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8016C588: neg.s       $f6, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = -ctx->f14.fl;
    // 0x8016C58C: mul.s       $f8, $f16, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f4.fl);
    // 0x8016C590: nop

    // 0x8016C594: mul.s       $f4, $f6, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f18.fl);
    // 0x8016C598: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x8016C59C: mul.s       $f0, $f6, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8016C5A0: b           L_8016C6C0
    // 0x8016C5A4: nop

        goto L_8016C6C0;
    // 0x8016C5A4: nop

L_8016C5A8:
    // 0x8016C5A8: trunc.w.s   $f8, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    ctx->f8.u32l = TRUNC_W_S(ctx->f14.fl);
    // 0x8016C5AC: lwc1        $f16, 0x30($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016C5B0: lwc1        $f18, 0x3C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016C5B4: mfc1        $t5, $f8
    ctx->r13 = (int32_t)ctx->f8.u32l;
    // 0x8016C5B8: trunc.w.s   $f8, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    ctx->f8.u32l = TRUNC_W_S(ctx->f12.fl);
    // 0x8016C5BC: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x8016C5C0: mfc1        $t7, $f8
    ctx->r15 = (int32_t)ctx->f8.u32l;
    // 0x8016C5C4: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016C5C8: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x8016C5CC: mul.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x8016C5D0: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016C5D4: mul.s       $f8, $f18, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f6.fl);
    // 0x8016C5D8: sub.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x8016C5DC: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016C5E0: mfc1        $v1, $f6
    ctx->r3 = (int32_t)ctx->f6.u32l;
    // 0x8016C5E4: nop

    // 0x8016C5E8: beq         $v1, $zero, L_8016C648
    if (ctx->r3 == 0) {
        // 0x8016C5EC: nop
    
            goto L_8016C648;
    }
    // 0x8016C5EC: nop

    // 0x8016C5F0: mtc1        $v1, $f8
    ctx->f8.u32l = ctx->r3;
    // 0x8016C5F4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016C5F8: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016C5FC: cvt.s.w     $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016C600: lwc1        $f8, 0x4C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016C604: div.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = DIV_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8016C608: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016C60C: mul.s       $f10, $f16, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x8016C610: swc1        $f6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f6.u32l;
    // 0x8016C614: swc1        $f6, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f6.u32l;
    // 0x8016C618: mul.s       $f6, $f4, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x8016C61C: sub.s       $f10, $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f10.fl - ctx->f6.fl;
    // 0x8016C620: lwc1        $f6, 0x0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016C624: mul.s       $f2, $f10, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x8016C628: neg.s       $f10, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = -ctx->f18.fl;
    // 0x8016C62C: mul.s       $f10, $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f10.fl, ctx->f8.fl);
    // 0x8016C630: nop

    // 0x8016C634: mul.s       $f8, $f4, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x8016C638: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x8016C63C: mul.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8016C640: b           L_8016C6C0
    // 0x8016C644: nop

        goto L_8016C6C0;
    // 0x8016C644: nop

L_8016C648:
    // 0x8016C648: mul.s       $f8, $f0, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f16.fl);
    // 0x8016C64C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016C650: mul.s       $f10, $f18, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f2.fl);
    // 0x8016C654: sub.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x8016C658: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016C65C: mfc1        $v1, $f6
    ctx->r3 = (int32_t)ctx->f6.u32l;
    // 0x8016C660: lwc1        $f6, 0x44($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016C664: beq         $v1, $zero, L_8016C6B8
    if (ctx->r3 == 0) {
        // 0x8016C668: nop
    
            goto L_8016C6B8;
    }
    // 0x8016C668: nop

    // 0x8016C66C: mtc1        $v1, $f10
    ctx->f10.u32l = ctx->r3;
    // 0x8016C670: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016C674: lwc1        $f14, 0x48($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016C678: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016C67C: mul.s       $f10, $f16, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f6.fl);
    // 0x8016C680: div.s       $f12, $f8, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = DIV_S(ctx->f8.fl, ctx->f4.fl);
    // 0x8016C684: lwc1        $f8, 0x2C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016C688: mul.s       $f4, $f14, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x8016C68C: sub.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x8016C690: neg.s       $f10, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = -ctx->f18.fl;
    // 0x8016C694: mul.s       $f2, $f8, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = MUL_S(ctx->f8.fl, ctx->f12.fl);
    // 0x8016C698: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016C69C: mul.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x8016C6A0: nop

    // 0x8016C6A4: mul.s       $f10, $f14, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x8016C6A8: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8016C6AC: mul.s       $f0, $f6, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = MUL_S(ctx->f6.fl, ctx->f12.fl);
    // 0x8016C6B0: b           L_8016C6C0
    // 0x8016C6B4: nop

        goto L_8016C6C0;
    // 0x8016C6B4: nop

L_8016C6B8:
    // 0x8016C6B8: b           L_8016C70C
    // 0x8016C6BC: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016C70C;
    // 0x8016C6BC: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016C6C0:
    // 0x8016C6C0: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x8016C6C4: nop

    // 0x8016C6C8: c.lt.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl < ctx->f12.fl;
    // 0x8016C6CC: nop

    // 0x8016C6D0: bc1t        L_8016C704
    if (c1cs) {
        // 0x8016C6D4: nop
    
            goto L_8016C704;
    }
    // 0x8016C6D4: nop

    // 0x8016C6D8: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x8016C6DC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016C6E0: bc1t        L_8016C704
    if (c1cs) {
        // 0x8016C6E4: nop
    
            goto L_8016C704;
    }
    // 0x8016C6E4: nop

    // 0x8016C6E8: add.s       $f10, $f2, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f2.fl + ctx->f0.fl;
    // 0x8016C6EC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016C6F0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016C6F4: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016C6F8: nop

    // 0x8016C6FC: bc1f        L_8016C70C
    if (!c1cs) {
        // 0x8016C700: nop
    
            goto L_8016C70C;
    }
    // 0x8016C700: nop

L_8016C704:
    // 0x8016C704: b           L_8016C70C
    // 0x8016C708: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016C70C;
    // 0x8016C708: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016C70C:
    // 0x8016C70C: jr          $ra
    // 0x8016C710: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x8016C710: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void func_8016C714(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016C714: lwc1        $f0, 0x4($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016C718: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8016C71C: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x8016C720: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016C724: c.lt.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl < ctx->f10.fl;
    // 0x8016C728: nop

    // 0x8016C72C: bc1fl       L_8016C740
    if (!c1cs) {
        // 0x8016C730: mov.s       $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
            goto L_8016C740;
    }
    goto skip_0;
    // 0x8016C730: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    skip_0:
    // 0x8016C734: b           L_8016C740
    // 0x8016C738: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_8016C740;
    // 0x8016C738: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
    // 0x8016C73C: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_8016C740:
    // 0x8016C740: lwc1        $f6, -0x513C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X513C);
    // 0x8016C744: c.lt.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl < ctx->f6.fl;
    // 0x8016C748: nop

    // 0x8016C74C: bc1fl       L_8016C760
    if (!c1cs) {
        // 0x8016C750: lwc1        $f4, 0x0($a0)
        ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
            goto L_8016C760;
    }
    goto skip_1;
    // 0x8016C750: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    skip_1:
    // 0x8016C754: b           L_8016CE54
    // 0x8016C758: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016CE54;
    // 0x8016C758: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016C75C: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
L_8016C760:
    // 0x8016C760: lwc1        $f8, 0x0($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016C764: lwc1        $f6, 0x4($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8016C768: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x8016C76C: mul.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x8016C770: or          $a3, $a2, $zero
    ctx->r7 = ctx->r6 | 0;
    // 0x8016C774: or          $t0, $a0, $zero
    ctx->r8 = ctx->r4 | 0;
    // 0x8016C778: mul.s       $f4, $f0, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x8016C77C: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016C780: addiu       $t1, $sp, 0x44
    ctx->r9 = ADD32(ctx->r29, 0X44);
    // 0x8016C784: addiu       $t2, $sp, 0x38
    ctx->r10 = ADD32(ctx->r29, 0X38);
    // 0x8016C788: addiu       $t3, $sp, 0x2C
    ctx->r11 = ADD32(ctx->r29, 0X2C);
    // 0x8016C78C: addiu       $v0, $sp, 0x38
    ctx->r2 = ADD32(ctx->r29, 0X38);
    // 0x8016C790: add.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8016C794: lwc1        $f10, 0x8($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016C798: mul.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8016C79C: lwc1        $f10, 0xC($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8016C7A0: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x8016C7A4: add.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016C7A8: neg.s       $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = -ctx->f8.fl;
    // 0x8016C7AC: div.s       $f12, $f4, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = DIV_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8016C7B0: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016C7B4: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016C7B8: sltu        $at, $t3, $v0
    ctx->r1 = ctx->r11 < ctx->r2 ? 1 : 0;
    // 0x8016C7BC: swc1        $f18, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f18.u32l;
    // 0x8016C7C0: beq         $at, $zero, L_8016C834
    if (ctx->r1 == 0) {
        // 0x8016C7C4: lh          $t4, 0x10($t0)
        ctx->r12 = MEM_H(ctx->r8, 0X10);
            goto L_8016C834;
    }
    // 0x8016C7C4: lh          $t4, 0x10($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X10);
L_8016C7C8:
    // 0x8016C7C8: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x8016C7CC: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016C7D0: sltu        $at, $t3, $v0
    ctx->r1 = ctx->r11 < ctx->r2 ? 1 : 0;
    // 0x8016C7D4: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016C7D8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C7DC: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016C7E0: addiu       $t0, $t0, 0x2
    ctx->r8 = ADD32(ctx->r8, 0X2);
    // 0x8016C7E4: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016C7E8: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016C7EC: sub.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl - ctx->f16.fl;
    // 0x8016C7F0: swc1        $f14, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->f14.u32l;
    // 0x8016C7F4: lh          $t5, 0x14($t0)
    ctx->r13 = MEM_H(ctx->r8, 0X14);
    // 0x8016C7F8: mtc1        $t5, $f14
    ctx->f14.u32l = ctx->r13;
    // 0x8016C7FC: nop

    // 0x8016C800: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C804: sub.s       $f14, $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C808: swc1        $f14, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f14.u32l;
    // 0x8016C80C: lh          $t6, 0x20($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X20);
    // 0x8016C810: mtc1        $t6, $f14
    ctx->f14.u32l = ctx->r14;
    // 0x8016C814: nop

    // 0x8016C818: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C81C: sub.s       $f16, $f14, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C820: swc1        $f16, -0x8($t3)
    MEM_W(-0X8, ctx->r11) = ctx->f16.u32l;
    // 0x8016C824: lwc1        $f18, 0x0($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016C828: swc1        $f18, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f18.u32l;
    // 0x8016C82C: bne         $at, $zero, L_8016C7C8
    if (ctx->r1 != 0) {
        // 0x8016C830: lh          $t4, 0x10($t0)
        ctx->r12 = MEM_H(ctx->r8, 0X10);
            goto L_8016C7C8;
    }
    // 0x8016C830: lh          $t4, 0x10($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X10);
L_8016C834:
    // 0x8016C834: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x8016C838: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016C83C: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016C840: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016C844: addiu       $t0, $t0, 0x2
    ctx->r8 = ADD32(ctx->r8, 0X2);
    // 0x8016C848: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016C84C: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016C850: sub.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl - ctx->f16.fl;
    // 0x8016C854: swc1        $f14, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->f14.u32l;
    // 0x8016C858: lh          $t5, 0x14($t0)
    ctx->r13 = MEM_H(ctx->r8, 0X14);
    // 0x8016C85C: mtc1        $t5, $f14
    ctx->f14.u32l = ctx->r13;
    // 0x8016C860: nop

    // 0x8016C864: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C868: sub.s       $f14, $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C86C: swc1        $f14, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f14.u32l;
    // 0x8016C870: lh          $t6, 0x20($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X20);
    // 0x8016C874: mtc1        $t6, $f14
    ctx->f14.u32l = ctx->r14;
    // 0x8016C878: nop

    // 0x8016C87C: cvt.s.w     $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    ctx->f14.fl = CVT_S_W(ctx->f14.u32l);
    // 0x8016C880: sub.s       $f16, $f14, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016C884: swc1        $f16, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f16.u32l;
    // 0x8016C888: lwc1        $f6, 0x4($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8016C88C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016C890: add.s       $f10, $f6, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f12.fl;
    // 0x8016C894: swc1        $f10, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f10.u32l;
    // 0x8016C898: lwc1        $f8, 0x48($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016C89C: lwc1        $f6, 0x44($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016C8A0: add.s       $f4, $f8, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f12.fl;
    // 0x8016C8A4: mul.s       $f10, $f6, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f6.fl);
    // 0x8016C8A8: nop

    // 0x8016C8AC: mul.s       $f8, $f4, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f4.fl);
    // 0x8016C8B0: swc1        $f4, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f4.u32l;
    // 0x8016C8B4: lwc1        $f4, 0x4C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016C8B8: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x8016C8BC: mul.s       $f10, $f4, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f4.fl);
    // 0x8016C8C0: lwc1        $f4, -0x5138($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X5138);
    // 0x8016C8C4: add.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8016C8C8: lwc1        $f10, 0x38($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016C8CC: c.lt.s      $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f8.fl < ctx->f4.fl;
    // 0x8016C8D0: nop

    // 0x8016C8D4: bc1f        L_8016C8E4
    if (!c1cs) {
        // 0x8016C8D8: nop
    
            goto L_8016C8E4;
    }
    // 0x8016C8D8: nop

    // 0x8016C8DC: b           L_8016CE54
    // 0x8016C8E0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016CE54;
    // 0x8016C8E0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016C8E4:
    // 0x8016C8E4: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8016C8E8: lwc1        $f4, 0x2C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016C8EC: lwc1        $f14, 0x34($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016C8F0: swc1        $f10, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f10.u32l;
    // 0x8016C8F4: mfc1        $t8, $f6
    ctx->r24 = (int32_t)ctx->f6.u32l;
    // 0x8016C8F8: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016C8FC: or          $a3, $a2, $zero
    ctx->r7 = ctx->r6 | 0;
    // 0x8016C900: mtc1        $t8, $f8
    ctx->f8.u32l = ctx->r24;
    // 0x8016C904: or          $t0, $a0, $zero
    ctx->r8 = ctx->r4 | 0;
    // 0x8016C908: mfc1        $t4, $f6
    ctx->r12 = (int32_t)ctx->f6.u32l;
    // 0x8016C90C: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016C910: addiu       $t1, $sp, 0x44
    ctx->r9 = ADD32(ctx->r29, 0X44);
    // 0x8016C914: mtc1        $t4, $f8
    ctx->f8.u32l = ctx->r12;
    // 0x8016C918: addiu       $t2, $sp, 0x38
    ctx->r10 = ADD32(ctx->r29, 0X38);
    // 0x8016C91C: addiu       $t3, $sp, 0x2C
    ctx->r11 = ADD32(ctx->r29, 0X2C);
    // 0x8016C920: cvt.s.w     $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    ctx->f2.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016C924: mul.s       $f6, $f0, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f14.fl);
    // 0x8016C928: lwc1        $f8, 0x40($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016C92C: addiu       $v0, $sp, 0x38
    ctx->r2 = ADD32(ctx->r29, 0X38);
    // 0x8016C930: mul.s       $f10, $f8, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016C934: sub.s       $f6, $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016C938: trunc.w.s   $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016C93C: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x8016C940: nop

    // 0x8016C944: beql        $v1, $zero, L_8016C9A4
    if (ctx->r3 == 0) {
        // 0x8016C948: trunc.w.s   $f10, $f14
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    ctx->f10.u32l = TRUNC_W_S(ctx->f14.fl);
            goto L_8016C9A4;
    }
    goto skip_2;
    // 0x8016C948: trunc.w.s   $f10, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    ctx->f10.u32l = TRUNC_W_S(ctx->f14.fl);
    skip_2:
    // 0x8016C94C: mtc1        $v1, $f10
    ctx->f10.u32l = ctx->r3;
    // 0x8016C950: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016C954: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8016C958: cvt.s.w     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016C95C: lwc1        $f18, 0x44($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016C960: lwc1        $f16, 0x4C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016C964: div.s       $f0, $f6, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8016C968: mul.s       $f6, $f14, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f18.fl);
    // 0x8016C96C: nop

    // 0x8016C970: mul.s       $f10, $f16, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f4.fl);
    // 0x8016C974: sub.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016C978: lwc1        $f6, 0x0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016C97C: mul.s       $f2, $f4, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8016C980: neg.s       $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = -ctx->f8.fl;
    // 0x8016C984: mul.s       $f10, $f16, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f6.fl);
    // 0x8016C988: nop

    // 0x8016C98C: mul.s       $f6, $f4, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x8016C990: add.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8016C994: mul.s       $f12, $f8, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016C998: b           L_8016CAD8
    // 0x8016C99C: nop

        goto L_8016CAD8;
    // 0x8016C99C: nop

    // 0x8016C9A0: trunc.w.s   $f10, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    ctx->f10.u32l = TRUNC_W_S(ctx->f14.fl);
L_8016C9A4:
    // 0x8016C9A4: lwc1        $f4, 0x3C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016C9A8: lwc1        $f18, 0x30($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016C9AC: mfc1        $t7, $f10
    ctx->r15 = (int32_t)ctx->f10.u32l;
    // 0x8016C9B0: nop

    // 0x8016C9B4: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x8016C9B8: nop

    // 0x8016C9BC: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016C9C0: lwc1        $f6, 0x40($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016C9C4: mul.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x8016C9C8: trunc.w.s   $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x8016C9CC: mfc1        $t9, $f8
    ctx->r25 = (int32_t)ctx->f8.u32l;
    // 0x8016C9D0: nop

    // 0x8016C9D4: mtc1        $t9, $f8
    ctx->f8.u32l = ctx->r25;
    // 0x8016C9D8: nop

    // 0x8016C9DC: cvt.s.w     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016C9E0: mul.s       $f8, $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x8016C9E4: sub.s       $f8, $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x8016C9E8: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8016C9EC: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x8016C9F0: lwc1        $f10, 0x3C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016C9F4: beq         $v1, $zero, L_8016CA50
    if (ctx->r3 == 0) {
        // 0x8016C9F8: nop
    
            goto L_8016CA50;
    }
    // 0x8016C9F8: nop

    // 0x8016C9FC: mtc1        $v1, $f10
    ctx->f10.u32l = ctx->r3;
    // 0x8016CA00: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016CA04: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016CA08: cvt.s.w     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016CA0C: lwc1        $f16, 0x4C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016CA10: lwc1        $f14, 0x48($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016CA14: div.s       $f0, $f8, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f10.fl);
    // 0x8016CA18: lwc1        $f10, 0x34($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016CA1C: mul.s       $f8, $f18, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f16.fl);
    // 0x8016CA20: nop

    // 0x8016CA24: mul.s       $f10, $f14, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f10.fl);
    // 0x8016CA28: sub.s       $f8, $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x8016CA2C: mul.s       $f2, $f8, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016CA30: neg.s       $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = -ctx->f4.fl;
    // 0x8016CA34: mul.s       $f10, $f14, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x8016CA38: nop

    // 0x8016CA3C: mul.s       $f6, $f8, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f16.fl);
    // 0x8016CA40: add.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8016CA44: mul.s       $f12, $f4, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8016CA48: b           L_8016CAD8
    // 0x8016CA4C: nop

        goto L_8016CAD8;
    // 0x8016CA4C: nop

L_8016CA50:
    // 0x8016CA50: mul.s       $f8, $f0, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f18.fl);
    // 0x8016CA54: lui         $at, 0xBF80
    ctx->r1 = S32(0XBF80 << 16);
    // 0x8016CA58: mul.s       $f6, $f10, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8016CA5C: sub.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x8016CA60: trunc.w.s   $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016CA64: mfc1        $v1, $f8
    ctx->r3 = (int32_t)ctx->f8.u32l;
    // 0x8016CA68: nop

    // 0x8016CA6C: beql        $v1, $zero, L_8016CAD4
    if (ctx->r3 == 0) {
        // 0x8016CA70: mtc1        $at, $f2
        ctx->f2.u32l = ctx->r1;
            goto L_8016CAD4;
    }
    goto skip_3;
    // 0x8016CA70: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    skip_3:
    // 0x8016CA74: mtc1        $v1, $f4
    ctx->f4.u32l = ctx->r3;
    // 0x8016CA78: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016CA7C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8016CA80: cvt.s.w     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016CA84: lwc1        $f18, 0x44($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016CA88: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016CA8C: lwc1        $f14, 0x48($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016CA90: div.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8016CA94: lwc1        $f8, 0x2C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016CA98: mul.s       $f6, $f4, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x8016CA9C: nop

    // 0x8016CAA0: mul.s       $f4, $f14, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x8016CAA4: sub.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x8016CAA8: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016CAAC: mul.s       $f2, $f8, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016CAB0: neg.s       $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = -ctx->f10.fl;
    // 0x8016CAB4: mul.s       $f4, $f14, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x8016CAB8: nop

    // 0x8016CABC: mul.s       $f6, $f8, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x8016CAC0: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016CAC4: mul.s       $f12, $f10, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x8016CAC8: b           L_8016CAD8
    // 0x8016CACC: nop

        goto L_8016CAD8;
    // 0x8016CACC: nop

    // 0x8016CAD0: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
L_8016CAD4:
    // 0x8016CAD4: nop

L_8016CAD8:
    // 0x8016CAD8: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8016CADC: nop

    // 0x8016CAE0: c.le.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl <= ctx->f2.fl;
    // 0x8016CAE4: nop

    // 0x8016CAE8: bc1f        L_8016CB24
    if (!c1cs) {
        // 0x8016CAEC: nop
    
            goto L_8016CB24;
    }
    // 0x8016CAEC: nop

    // 0x8016CAF0: c.le.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl <= ctx->f12.fl;
    // 0x8016CAF4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016CAF8: bc1f        L_8016CB24
    if (!c1cs) {
        // 0x8016CAFC: nop
    
            goto L_8016CB24;
    }
    // 0x8016CAFC: nop

    // 0x8016CB00: add.s       $f4, $f2, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f2.fl + ctx->f12.fl;
    // 0x8016CB04: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016CB08: nop

    // 0x8016CB0C: c.le.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl <= ctx->f8.fl;
    // 0x8016CB10: nop

    // 0x8016CB14: bc1f        L_8016CB24
    if (!c1cs) {
        // 0x8016CB18: nop
    
            goto L_8016CB24;
    }
    // 0x8016CB18: nop

    // 0x8016CB1C: b           L_8016CE54
    // 0x8016CB20: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016CE54;
    // 0x8016CB20: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016CB24:
    // 0x8016CB24: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016CB28: beq         $t3, $v0, L_8016CB90
    if (ctx->r11 == ctx->r2) {
        // 0x8016CB2C: lh          $t6, 0x1C($t0)
        ctx->r14 = MEM_H(ctx->r8, 0X1C);
            goto L_8016CB90;
    }
    // 0x8016CB2C: lh          $t6, 0x1C($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X1C);
L_8016CB30:
    // 0x8016CB30: mtc1        $t6, $f18
    ctx->f18.u32l = ctx->r14;
    // 0x8016CB34: lwc1        $f16, 0x0($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016CB38: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016CB3C: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016CB40: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016CB44: addiu       $t0, $t0, 0x2
    ctx->r8 = ADD32(ctx->r8, 0X2);
    // 0x8016CB48: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016CB4C: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016CB50: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016CB54: swc1        $f16, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->f16.u32l;
    // 0x8016CB58: lh          $t7, 0x20($t0)
    ctx->r15 = MEM_H(ctx->r8, 0X20);
    // 0x8016CB5C: mtc1        $t7, $f16
    ctx->f16.u32l = ctx->r15;
    // 0x8016CB60: nop

    // 0x8016CB64: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016CB68: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016CB6C: swc1        $f16, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f16.u32l;
    // 0x8016CB70: lh          $t8, 0x14($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X14);
    // 0x8016CB74: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016CB78: nop

    // 0x8016CB7C: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016CB80: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016CB84: swc1        $f18, -0x8($t3)
    MEM_W(-0X8, ctx->r11) = ctx->f18.u32l;
    // 0x8016CB88: bne         $t3, $v0, L_8016CB30
    if (ctx->r11 != ctx->r2) {
        // 0x8016CB8C: lh          $t6, 0x1C($t0)
        ctx->r14 = MEM_H(ctx->r8, 0X1C);
            goto L_8016CB30;
    }
    // 0x8016CB8C: lh          $t6, 0x1C($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X1C);
L_8016CB90:
    // 0x8016CB90: mtc1        $t6, $f18
    ctx->f18.u32l = ctx->r14;
    // 0x8016CB94: lwc1        $f16, 0x0($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016CB98: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016CB9C: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016CBA0: addiu       $t0, $t0, 0x2
    ctx->r8 = ADD32(ctx->r8, 0X2);
    // 0x8016CBA4: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016CBA8: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016CBAC: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016CBB0: swc1        $f16, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->f16.u32l;
    // 0x8016CBB4: lh          $t7, 0x20($t0)
    ctx->r15 = MEM_H(ctx->r8, 0X20);
    // 0x8016CBB8: mtc1        $t7, $f16
    ctx->f16.u32l = ctx->r15;
    // 0x8016CBBC: nop

    // 0x8016CBC0: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016CBC4: sub.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016CBC8: swc1        $f16, -0x4($t2)
    MEM_W(-0X4, ctx->r10) = ctx->f16.u32l;
    // 0x8016CBCC: lh          $t8, 0x14($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X14);
    // 0x8016CBD0: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016CBD4: nop

    // 0x8016CBD8: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016CBDC: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016CBE0: swc1        $f18, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f18.u32l;
    // 0x8016CBE4: lwc1        $f18, 0x44($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016CBE8: lwc1        $f14, 0x48($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016CBEC: lwc1        $f16, 0x4C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8016CBF0: mul.s       $f10, $f18, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x8016CBF4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016CBF8: mul.s       $f8, $f14, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8016CBFC: add.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x8016CC00: mul.s       $f6, $f16, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x8016CC04: lwc1        $f8, -0x5134($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X5134);
    // 0x8016CC08: add.s       $f10, $f6, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x8016CC0C: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016CC10: c.lt.s      $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f10.fl < ctx->f8.fl;
    // 0x8016CC14: nop

    // 0x8016CC18: bc1fl       L_8016CC2C
    if (!c1cs) {
        // 0x8016CC1C: trunc.w.s   $f4, $f6
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
            goto L_8016CC2C;
    }
    goto skip_4;
    // 0x8016CC1C: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
    skip_4:
    // 0x8016CC20: b           L_8016CE54
    // 0x8016CC24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016CE54;
    // 0x8016CC24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016CC28: trunc.w.s   $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.u32l = TRUNC_W_S(ctx->f6.fl);
L_8016CC2C:
    // 0x8016CC2C: lwc1        $f8, 0x2C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016CC30: swc1        $f6, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f6.u32l;
    // 0x8016CC34: lwc1        $f6, 0x40($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016CC38: mfc1        $t4, $f4
    ctx->r12 = (int32_t)ctx->f4.u32l;
    // 0x8016CC3C: trunc.w.s   $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8016CC40: swc1        $f8, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f8.u32l;
    // 0x8016CC44: mtc1        $t4, $f10
    ctx->f10.u32l = ctx->r12;
    // 0x8016CC48: mfc1        $t6, $f4
    ctx->r14 = (int32_t)ctx->f4.u32l;
    // 0x8016CC4C: cvt.s.w     $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    ctx->f0.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016CC50: lwc1        $f4, 0x34($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016CC54: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x8016CC58: nop

    // 0x8016CC5C: cvt.s.w     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    ctx->f2.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016CC60: mul.s       $f10, $f0, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f4.fl);
    // 0x8016CC64: nop

    // 0x8016CC68: mul.s       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016CC6C: sub.s       $f10, $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x8016CC70: trunc.w.s   $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8016CC74: lwc1        $f10, 0x34($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016CC78: mfc1        $v1, $f8
    ctx->r3 = (int32_t)ctx->f8.u32l;
    // 0x8016CC7C: nop

    // 0x8016CC80: beq         $v1, $zero, L_8016CCD8
    if (ctx->r3 == 0) {
        // 0x8016CC84: nop
    
            goto L_8016CCD8;
    }
    // 0x8016CC84: nop

    // 0x8016CC88: mtc1        $v1, $f8
    ctx->f8.u32l = ctx->r3;
    // 0x8016CC8C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016CC90: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016CC94: cvt.s.w     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016CC98: div.s       $f0, $f10, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = DIV_S(ctx->f10.fl, ctx->f8.fl);
    // 0x8016CC9C: lwc1        $f8, 0x4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4);
    // 0x8016CCA0: mul.s       $f10, $f4, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x8016CCA4: nop

    // 0x8016CCA8: mul.s       $f4, $f16, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x8016CCAC: sub.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x8016CCB0: lwc1        $f10, 0x0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016CCB4: mul.s       $f2, $f8, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016CCB8: neg.s       $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = -ctx->f6.fl;
    // 0x8016CCBC: mul.s       $f4, $f16, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f10.fl);
    // 0x8016CCC0: nop

    // 0x8016CCC4: mul.s       $f10, $f8, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x8016CCC8: add.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x8016CCCC: mul.s       $f12, $f6, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8016CCD0: b           L_8016CE08
    // 0x8016CCD4: nop

        goto L_8016CE08;
    // 0x8016CCD4: nop

L_8016CCD8:
    // 0x8016CCD8: trunc.w.s   $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x8016CCDC: lwc1        $f8, 0x30($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016CCE0: lwc1        $f4, 0x3C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016CCE4: mfc1        $t9, $f6
    ctx->r25 = (int32_t)ctx->f6.u32l;
    // 0x8016CCE8: mov.s       $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    ctx->f12.fl = ctx->f8.fl;
    // 0x8016CCEC: swc1        $f8, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f8.u32l;
    // 0x8016CCF0: mtc1        $t9, $f6
    ctx->f6.u32l = ctx->r25;
    // 0x8016CCF4: lwc1        $f8, 0x40($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016CCF8: swc1        $f4, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f4.u32l;
    // 0x8016CCFC: cvt.s.w     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016CD00: mul.s       $f6, $f4, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8016CD04: trunc.w.s   $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8016CD08: mfc1        $t5, $f4
    ctx->r13 = (int32_t)ctx->f4.u32l;
    // 0x8016CD0C: nop

    // 0x8016CD10: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x8016CD14: nop

    // 0x8016CD18: cvt.s.w     $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    ctx->f4.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016CD1C: mul.s       $f4, $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x8016CD20: sub.s       $f4, $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8016CD24: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016CD28: mfc1        $v1, $f6
    ctx->r3 = (int32_t)ctx->f6.u32l;
    // 0x8016CD2C: lwc1        $f6, 0x3C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016CD30: beq         $v1, $zero, L_8016CD88
    if (ctx->r3 == 0) {
        // 0x8016CD34: nop
    
            goto L_8016CD88;
    }
    // 0x8016CD34: nop

    // 0x8016CD38: mtc1        $v1, $f6
    ctx->f6.u32l = ctx->r3;
    // 0x8016CD3C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016CD40: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8016CD44: cvt.s.w     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016CD48: div.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8016CD4C: lwc1        $f4, 0x4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X4);
    // 0x8016CD50: mul.s       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8016CD54: nop

    // 0x8016CD58: mul.s       $f4, $f14, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f10.fl);
    // 0x8016CD5C: sub.s       $f10, $f6, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x8016CD60: lwc1        $f4, 0x0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X0);
    // 0x8016CD64: mul.s       $f2, $f10, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x8016CD68: neg.s       $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = -ctx->f4.fl;
    // 0x8016CD6C: mul.s       $f6, $f14, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x8016CD70: nop

    // 0x8016CD74: mul.s       $f8, $f10, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x8016CD78: add.s       $f4, $f6, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8016CD7C: mul.s       $f12, $f4, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8016CD80: b           L_8016CE08
    // 0x8016CD84: nop

        goto L_8016CE08;
    // 0x8016CD84: nop

L_8016CD88:
    // 0x8016CD88: mul.s       $f10, $f0, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x8016CD8C: nop

    // 0x8016CD90: mul.s       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016CD94: sub.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x8016CD98: trunc.w.s   $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x8016CD9C: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x8016CDA0: nop

    // 0x8016CDA4: beq         $v1, $zero, L_8016CE00
    if (ctx->r3 == 0) {
        // 0x8016CDA8: nop
    
            goto L_8016CE00;
    }
    // 0x8016CDA8: nop

    // 0x8016CDAC: mtc1        $v1, $f4
    ctx->f4.u32l = ctx->r3;
    // 0x8016CDB0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016CDB4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016CDB8: cvt.s.w     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016CDBC: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016CDC0: div.s       $f0, $f8, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f10.fl);
    // 0x8016CDC4: lwc1        $f10, 0x2C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016CDC8: mul.s       $f8, $f4, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x8016CDCC: nop

    // 0x8016CDD0: mul.s       $f4, $f14, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f10.fl);
    // 0x8016CDD4: sub.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x8016CDD8: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016CDDC: mul.s       $f2, $f10, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x8016CDE0: neg.s       $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = -ctx->f6.fl;
    // 0x8016CDE4: mul.s       $f4, $f14, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x8016CDE8: nop

    // 0x8016CDEC: mul.s       $f8, $f10, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f18.fl);
    // 0x8016CDF0: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x8016CDF4: mul.s       $f12, $f6, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8016CDF8: b           L_8016CE08
    // 0x8016CDFC: nop

        goto L_8016CE08;
    // 0x8016CDFC: nop

L_8016CE00:
    // 0x8016CE00: b           L_8016CE54
    // 0x8016CE04: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016CE54;
    // 0x8016CE04: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016CE08:
    // 0x8016CE08: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8016CE0C: nop

    // 0x8016CE10: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
    // 0x8016CE14: nop

    // 0x8016CE18: bc1t        L_8016CE4C
    if (c1cs) {
        // 0x8016CE1C: nop
    
            goto L_8016CE4C;
    }
    // 0x8016CE1C: nop

    // 0x8016CE20: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x8016CE24: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016CE28: bc1t        L_8016CE4C
    if (c1cs) {
        // 0x8016CE2C: nop
    
            goto L_8016CE4C;
    }
    // 0x8016CE2C: nop

    // 0x8016CE30: add.s       $f4, $f2, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f2.fl + ctx->f12.fl;
    // 0x8016CE34: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016CE38: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016CE3C: c.lt.s      $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f10.fl < ctx->f4.fl;
    // 0x8016CE40: nop

    // 0x8016CE44: bc1f        L_8016CE54
    if (!c1cs) {
        // 0x8016CE48: nop
    
            goto L_8016CE54;
    }
    // 0x8016CE48: nop

L_8016CE4C:
    // 0x8016CE4C: b           L_8016CE54
    // 0x8016CE50: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016CE54;
    // 0x8016CE50: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_8016CE54:
    // 0x8016CE54: jr          $ra
    // 0x8016CE58: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x8016CE58: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void func_8016CE60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016CE60: addiu       $sp, $sp, -0x98
    ctx->r29 = ADD32(ctx->r29, -0X98);
    // 0x8016CE64: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8016CE68: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8016CE6C: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x8016CE70: lw          $s2, -0x1134($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X1134);
    // 0x8016CE74: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x8016CE78: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016CE7C: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x8016CE80: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8016CE84: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8016CE88: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8016CE8C: sw          $a2, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r6;
    // 0x8016CE90: sw          $a3, 0xA4($sp)
    MEM_W(0XA4, ctx->r29) = ctx->r7;
    // 0x8016CE94: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016CE98: addiu       $a0, $a0, -0x10F0
    ctx->r4 = ADD32(ctx->r4, -0X10F0);
    // 0x8016CE9C: jal         0x8016E930
    // 0x8016CEA0: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    func_8016E930(rdram, ctx);
        goto after_0;
    // 0x8016CEA0: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    after_0:
    // 0x8016CEA4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8016CEA8: lwc1        $f4, 0x0($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X0);
    // 0x8016CEAC: addiu       $v1, $sp, 0x84
    ctx->r3 = ADD32(ctx->r29, 0X84);
    // 0x8016CEB0: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x8016CEB4: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x8016CEB8: addiu       $v0, $sp, 0x78
    ctx->r2 = ADD32(ctx->r29, 0X78);
    // 0x8016CEBC: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8016CEC0: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x8016CEC4: bc1fl       L_8016CEEC
    if (!c1cs) {
        // 0x8016CEC8: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_8016CEEC;
    }
    goto skip_0;
    // 0x8016CEC8: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_0:
    // 0x8016CECC: lwc1        $f6, 0x8($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X8);
    // 0x8016CED0: c.eq.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl == ctx->f6.fl;
    // 0x8016CED4: nop

    // 0x8016CED8: bc1fl       L_8016CEEC
    if (!c1cs) {
        // 0x8016CEDC: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_8016CEEC;
    }
    goto skip_1;
    // 0x8016CEDC: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_1:
    // 0x8016CEE0: b           L_8016D1E0
    // 0x8016CEE4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016D1E0;
    // 0x8016CEE4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016CEE8: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
L_8016CEEC:
    // 0x8016CEEC: addiu       $a2, $sp, 0x84
    ctx->r6 = ADD32(ctx->r29, 0X84);
    // 0x8016CEF0: lwc1        $f16, 0x0($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016CEF4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016CEF8: sltu        $at, $v0, $a2
    ctx->r1 = ctx->r2 < ctx->r6 ? 1 : 0;
    // 0x8016CEFC: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016CF00: beql        $at, $zero, L_8016CF40
    if (ctx->r1 == 0) {
        // 0x8016CF04: swc1        $f18, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
            goto L_8016CF40;
    }
    goto skip_2;
    // 0x8016CF04: swc1        $f18, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
    skip_2:
    // 0x8016CF08: swc1        $f18, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
L_8016CF0C:
    // 0x8016CF0C: lwc1        $f16, 0x0($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016CF10: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016CF14: sltu        $at, $v0, $a2
    ctx->r1 = ctx->r2 < ctx->r6 ? 1 : 0;
    // 0x8016CF18: mul.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016CF1C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016CF20: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x8016CF24: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016CF28: swc1        $f16, -0x8($v0)
    MEM_W(-0X8, ctx->r2) = ctx->f16.u32l;
    // 0x8016CF2C: lwc1        $f16, 0x0($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016CF30: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016CF34: bnel        $at, $zero, L_8016CF0C
    if (ctx->r1 != 0) {
        // 0x8016CF38: swc1        $f18, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
            goto L_8016CF0C;
    }
    goto skip_3;
    // 0x8016CF38: swc1        $f18, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
    skip_3:
    // 0x8016CF3C: swc1        $f18, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
L_8016CF40:
    // 0x8016CF40: lwc1        $f16, 0x0($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016CF44: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016CF48: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x8016CF4C: mul.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016CF50: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016CF54: swc1        $f16, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f16.u32l;
    // 0x8016CF58: lwc1        $f6, 0xA0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XA0);
    // 0x8016CF5C: lwc1        $f8, 0xA4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XA4);
    // 0x8016CF60: addiu       $a0, $sp, 0x84
    ctx->r4 = ADD32(ctx->r29, 0X84);
    // 0x8016CF64: mul.s       $f2, $f6, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8016CF68: nop

    // 0x8016CF6C: mul.s       $f12, $f8, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016CF70: mov.s       $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    ctx->f14.fl = ctx->f2.fl;
    // 0x8016CF74: c.lt.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl < ctx->f12.fl;
    // 0x8016CF78: mov.s       $f16, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    ctx->f16.fl = ctx->f12.fl;
    // 0x8016CF7C: bc1f        L_8016CF8C
    if (!c1cs) {
        // 0x8016CF80: nop
    
            goto L_8016CF8C;
    }
    // 0x8016CF80: nop

    // 0x8016CF84: mov.s       $f14, $f12
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    ctx->f14.fl = ctx->f12.fl;
    // 0x8016CF88: mov.s       $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    ctx->f16.fl = ctx->f2.fl;
L_8016CF8C:
    // 0x8016CF8C: mfc1        $a2, $f14
    ctx->r6 = (int32_t)ctx->f14.u32l;
    // 0x8016CF90: mfc1        $a3, $f16
    ctx->r7 = (int32_t)ctx->f16.u32l;
    // 0x8016CF94: jal         0x8016E980
    // 0x8016CF98: addiu       $a1, $sp, 0x78
    ctx->r5 = ADD32(ctx->r29, 0X78);
    func_8016E980(rdram, ctx);
        goto after_1;
    // 0x8016CF98: addiu       $a1, $sp, 0x78
    ctx->r5 = ADD32(ctx->r29, 0X78);
    after_1:
    // 0x8016CF9C: lw          $a0, 0x10($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X10);
    // 0x8016CFA0: jal         0x8016D56C
    // 0x8016CFA4: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    func_8016D56C(rdram, ctx);
        goto after_2;
    // 0x8016CFA4: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    after_2:
    // 0x8016CFA8: lw          $a0, 0x14($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X14);
    // 0x8016CFAC: jal         0x8016D56C
    // 0x8016CFB0: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    func_8016D56C(rdram, ctx);
        goto after_3;
    // 0x8016CFB0: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    after_3:
    // 0x8016CFB4: lw          $a0, 0x18($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X18);
    // 0x8016CFB8: jal         0x8016D56C
    // 0x8016CFBC: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    func_8016D56C(rdram, ctx);
        goto after_4;
    // 0x8016CFBC: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    after_4:
    // 0x8016CFC0: lw          $t6, 0x20($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X20);
    // 0x8016CFC4: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x8016CFC8: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x8016CFCC: blez        $t6, L_8016D008
    if (SIGNED(ctx->r14) <= 0) {
        // 0x8016CFD0: nop
    
            goto L_8016D008;
    }
    // 0x8016CFD0: nop

    // 0x8016CFD4: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
L_8016CFD8:
    // 0x8016CFD8: lw          $t9, 0x38($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X38);
    // 0x8016CFDC: addu        $t8, $t7, $s0
    ctx->r24 = ADD32(ctx->r15, ctx->r16);
    // 0x8016CFE0: addu        $t0, $t9, $s0
    ctx->r8 = ADD32(ctx->r25, ctx->r16);
    // 0x8016CFE4: lw          $a1, 0x0($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X0);
    // 0x8016CFE8: jal         0x8016D56C
    // 0x8016CFEC: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
    func_8016D56C(rdram, ctx);
        goto after_5;
    // 0x8016CFEC: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
    after_5:
    // 0x8016CFF0: lw          $t1, 0x20($s2)
    ctx->r9 = MEM_W(ctx->r18, 0X20);
    // 0x8016CFF4: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x8016CFF8: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x8016CFFC: slt         $at, $s1, $t1
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x8016D000: bnel        $at, $zero, L_8016CFD8
    if (ctx->r1 != 0) {
        // 0x8016D004: lw          $t7, 0x24($s2)
        ctx->r15 = MEM_W(ctx->r18, 0X24);
            goto L_8016CFD8;
    }
    goto skip_4;
    // 0x8016D004: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
    skip_4:
L_8016D008:
    // 0x8016D008: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x8016D00C: lw          $s2, -0x1A20($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X1A20);
    // 0x8016D010: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016D014: addiu       $s4, $s4, -0x9FC
    ctx->r20 = ADD32(ctx->r20, -0X9FC);
    // 0x8016D018: beq         $s2, $zero, L_8016D0C0
    if (ctx->r18 == 0) {
        // 0x8016D01C: or          $s3, $zero, $zero
        ctx->r19 = 0 | 0;
            goto L_8016D0C0;
    }
    // 0x8016D01C: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x8016D020: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
L_8016D024:
    // 0x8016D024: beql        $v0, $zero, L_8016D0B4
    if (ctx->r2 == 0) {
        // 0x8016D028: lw          $s2, 0x0($s2)
        ctx->r18 = MEM_W(ctx->r18, 0X0);
            goto L_8016D0B4;
    }
    goto skip_5;
    // 0x8016D028: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    skip_5:
    // 0x8016D02C: lw          $s0, 0x24($v0)
    ctx->r16 = MEM_W(ctx->r2, 0X24);
    // 0x8016D030: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8016D034: beq         $s0, $zero, L_8016D0B0
    if (ctx->r16 == 0) {
        // 0x8016D038: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016D0B0;
    }
    // 0x8016D038: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016D03C: lw          $t2, 0x0($s4)
    ctx->r10 = MEM_W(ctx->r20, 0X0);
    // 0x8016D040: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
    // 0x8016D044: sw          $s3, 0x10($t2)
    MEM_W(0X10, ctx->r10) = ctx->r19;
    // 0x8016D048: lw          $t3, 0x0($s4)
    ctx->r11 = MEM_W(ctx->r20, 0X0);
    // 0x8016D04C: sw          $v0, 0xC($t3)
    MEM_W(0XC, ctx->r11) = ctx->r2;
    // 0x8016D050: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
    // 0x8016D054: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x8016D058: slti        $at, $v1, 0x282
    ctx->r1 = SIGNED(ctx->r3) < 0X282 ? 1 : 0;
    // 0x8016D05C: bne         $at, $zero, L_8016D074
    if (ctx->r1 != 0) {
        // 0x8016D060: slti        $at, $v1, 0x285
        ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
            goto L_8016D074;
    }
    // 0x8016D060: slti        $at, $v1, 0x285
    ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
    // 0x8016D064: beq         $at, $zero, L_8016D074
    if (ctx->r1 == 0) {
        // 0x8016D068: nop
    
            goto L_8016D074;
    }
    // 0x8016D068: nop

    // 0x8016D06C: b           L_8016D074
    // 0x8016D070: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_8016D074;
    // 0x8016D070: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_8016D074:
    // 0x8016D074: jal         0x8016D2F0
    // 0x8016D078: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016D2F0(rdram, ctx);
        goto after_6;
    // 0x8016D078: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_6:
    // 0x8016D07C: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016D080: beql        $a0, $zero, L_8016D0B4
    if (ctx->r4 == 0) {
        // 0x8016D084: lw          $s2, 0x0($s2)
        ctx->r18 = MEM_W(ctx->r18, 0X0);
            goto L_8016D0B4;
    }
    goto skip_6;
    // 0x8016D084: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    skip_6:
    // 0x8016D088: lhu         $t4, 0x2($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0X2);
    // 0x8016D08C: andi        $t5, $t4, 0x8000
    ctx->r13 = ctx->r12 & 0X8000;
    // 0x8016D090: beq         $t5, $zero, L_8016D0A8
    if (ctx->r13 == 0) {
        // 0x8016D094: nop
    
            goto L_8016D0A8;
    }
    // 0x8016D094: nop

    // 0x8016D098: jal         0x8016D200
    // 0x8016D09C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016D200(rdram, ctx);
        goto after_7;
    // 0x8016D09C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_7:
    // 0x8016D0A0: b           L_8016D0B4
    // 0x8016D0A4: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
        goto L_8016D0B4;
    // 0x8016D0A4: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
L_8016D0A8:
    // 0x8016D0A8: jal         0x8016D278
    // 0x8016D0AC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016D278(rdram, ctx);
        goto after_8;
    // 0x8016D0AC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_8:
L_8016D0B0:
    // 0x8016D0B0: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
L_8016D0B4:
    // 0x8016D0B4: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x8016D0B8: bnel        $s2, $zero, L_8016D024
    if (ctx->r18 != 0) {
        // 0x8016D0BC: lw          $v0, 0x4($s2)
        ctx->r2 = MEM_W(ctx->r18, 0X4);
            goto L_8016D024;
    }
    goto skip_7;
    // 0x8016D0BC: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
    skip_7:
L_8016D0C0:
    // 0x8016D0C0: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016D0C4: addiu       $s4, $s4, -0x9FC
    ctx->r20 = ADD32(ctx->r20, -0X9FC);
    // 0x8016D0C8: lw          $v1, 0x0($s4)
    ctx->r3 = MEM_W(ctx->r20, 0X0);
    // 0x8016D0CC: addiu       $s1, $v1, 0x230
    ctx->r17 = ADD32(ctx->r3, 0X230);
    // 0x8016D0D0: lh          $t6, 0x60($s1)
    ctx->r14 = MEM_H(ctx->r17, 0X60);
    // 0x8016D0D4: beql        $t6, $zero, L_8016D158
    if (ctx->r14 == 0) {
        // 0x8016D0D8: addiu       $s1, $v1, 0x48C
        ctx->r17 = ADD32(ctx->r3, 0X48C);
            goto L_8016D158;
    }
    goto skip_8;
    // 0x8016D0D8: addiu       $s1, $v1, 0x48C
    ctx->r17 = ADD32(ctx->r3, 0X48C);
    skip_8:
    // 0x8016D0DC: jal         0x80172D34
    // 0x8016D0E0: lwc1        $f12, 0x1C($s1)
    ctx->f12.u32l = MEM_W(ctx->r17, 0X1C);
    func_80172D34(rdram, ctx);
        goto after_9;
    // 0x8016D0E0: lwc1        $f12, 0x1C($s1)
    ctx->f12.u32l = MEM_W(ctx->r17, 0X1C);
    after_9:
    // 0x8016D0E4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016D0E8: lwc1        $f2, -0x5130($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X5130);
    // 0x8016D0EC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016D0F0: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x8016D0F4: mul.s       $f10, $f0, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x8016D0F8: addiu       $s0, $zero, 0x3
    ctx->r16 = ADD32(0, 0X3);
    // 0x8016D0FC: swc1        $f10, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->f10.u32l;
    // 0x8016D100: lwc1        $f12, 0x0($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016D104: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016D108: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
    // 0x8016D10C: mul.s       $f18, $f12, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x8016D110: beq         $v1, $s0, L_8016D13C
    if (ctx->r3 == ctx->r16) {
        // 0x8016D114: nop
    
            goto L_8016D13C;
    }
    // 0x8016D114: nop

L_8016D118:
    // 0x8016D118: mul.s       $f14, $f16, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x8016D11C: lwc1        $f12, 0x4($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X4);
    // 0x8016D120: lwc1        $f16, 0x10($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X10);
    // 0x8016D124: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016D128: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x8016D12C: mul.s       $f18, $f12, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x8016D130: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016D134: bne         $v1, $s0, L_8016D118
    if (ctx->r3 != ctx->r16) {
        // 0x8016D138: swc1        $f14, 0x8($v0)
        MEM_W(0X8, ctx->r2) = ctx->f14.u32l;
            goto L_8016D118;
    }
    // 0x8016D138: swc1        $f14, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f14.u32l;
L_8016D13C:
    // 0x8016D13C: mul.s       $f14, $f16, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x8016D140: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016D144: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016D148: swc1        $f14, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f14.u32l;
    // 0x8016D14C: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8016D150: lw          $v1, -0x9FC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X9FC);
    // 0x8016D154: addiu       $s1, $v1, 0x48C
    ctx->r17 = ADD32(ctx->r3, 0X48C);
L_8016D158:
    // 0x8016D158: lh          $t7, 0x60($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X60);
    // 0x8016D15C: addiu       $s0, $zero, 0x3
    ctx->r16 = ADD32(0, 0X3);
    // 0x8016D160: beql        $t7, $zero, L_8016D1E0
    if (ctx->r15 == 0) {
        // 0x8016D164: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_8016D1E0;
    }
    goto skip_9;
    // 0x8016D164: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_9:
    // 0x8016D168: jal         0x80172D34
    // 0x8016D16C: lwc1        $f12, 0x1C($s1)
    ctx->f12.u32l = MEM_W(ctx->r17, 0X1C);
    func_80172D34(rdram, ctx);
        goto after_10;
    // 0x8016D16C: lwc1        $f12, 0x1C($s1)
    ctx->f12.u32l = MEM_W(ctx->r17, 0X1C);
    after_10:
    // 0x8016D170: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016D174: lwc1        $f2, -0x5128($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X5128);
    // 0x8016D178: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016D17C: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x8016D180: mul.s       $f10, $f0, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f2.fl);
    // 0x8016D184: swc1        $f10, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->f10.u32l;
    // 0x8016D188: lwc1        $f12, 0x0($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016D18C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016D190: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
    // 0x8016D194: mul.s       $f18, $f12, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x8016D198: beq         $v1, $s0, L_8016D1C4
    if (ctx->r3 == ctx->r16) {
        // 0x8016D19C: nop
    
            goto L_8016D1C4;
    }
    // 0x8016D19C: nop

L_8016D1A0:
    // 0x8016D1A0: mul.s       $f14, $f16, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x8016D1A4: lwc1        $f12, 0x4($v0)
    ctx->f12.u32l = MEM_W(ctx->r2, 0X4);
    // 0x8016D1A8: lwc1        $f16, 0x10($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X10);
    // 0x8016D1AC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016D1B0: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x8016D1B4: mul.s       $f18, $f12, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x8016D1B8: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016D1BC: bne         $v1, $s0, L_8016D1A0
    if (ctx->r3 != ctx->r16) {
        // 0x8016D1C0: swc1        $f14, 0x8($v0)
        MEM_W(0X8, ctx->r2) = ctx->f14.u32l;
            goto L_8016D1A0;
    }
    // 0x8016D1C0: swc1        $f14, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f14.u32l;
L_8016D1C4:
    // 0x8016D1C4: mul.s       $f14, $f16, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x8016D1C8: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016D1CC: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016D1D0: swc1        $f14, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f14.u32l;
    // 0x8016D1D4: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8016D1D8: lw          $v1, -0x9FC($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X9FC);
    // 0x8016D1DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8016D1E0:
    // 0x8016D1E0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x8016D1E4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016D1E8: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8016D1EC: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8016D1F0: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8016D1F4: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8016D1F8: jr          $ra
    // 0x8016D1FC: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
    return;
    // 0x8016D1FC: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
;}
RECOMP_FUNC void func_8016D200(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016D200 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016D200++;
    if (lod_text_guard_depth_8016D200 > 128) {
        if (!lod_text_guard_logged_8016D200) {
            lod_text_guard_logged_8016D200 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016D200 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016D200--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016D200: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016D204: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016D208: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016D20C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016D210: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016D214: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016D218: beq         $s0, $zero, L_8016D264
    if (ctx->r16 == 0) {
        // 0x8016D21C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016D264;
    }
L_8016D21C:
    // 0x8016D21C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016D220: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x8016D224: jal         0x8016D2F0
    // 0x8016D228: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016D2F0(rdram, ctx);
        goto after_0;
    // 0x8016D228: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x8016D22C: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016D230: beql        $a0, $zero, L_8016D244
    if (ctx->r4 == 0) {
        // 0x8016D234: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_8016D244;
    }
    goto skip_0;
    // 0x8016D234: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x8016D238: jal         0x8016D200
    // 0x8016D23C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016D200(rdram, ctx);
        goto after_1;
    // 0x8016D23C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x8016D240: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_8016D244:
    // 0x8016D244: beql        $v0, $zero, L_8016D268
    if (ctx->r2 == 0) {
        // 0x8016D248: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016D268;
    }
    goto skip_1;
    // 0x8016D248: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016D24C: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x8016D250: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x8016D254: bnel        $t7, $zero, L_8016D268
    if (ctx->r15 != 0) {
        // 0x8016D258: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016D268;
    }
    goto skip_2;
    // 0x8016D258: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016D25C: bne         $v0, $zero, L_8016D21C
    if (ctx->r2 != 0) {
        // 0x8016D260: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016D21C;
    }
    // 0x8016D260: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016D264:
    // 0x8016D264: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016D268:
    // 0x8016D268: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016D26C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016D270: jr          $ra
    // 0x8016D274: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016D200--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016D274: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_8016D278(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016D278 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016D278++;
    if (lod_text_guard_depth_8016D278 > 128) {
        if (!lod_text_guard_logged_8016D278) {
            lod_text_guard_logged_8016D278 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016D278 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016D278--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016D278: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016D27C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016D280: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016D284: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016D288: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016D28C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016D290: beq         $s0, $zero, L_8016D2DC
    if (ctx->r16 == 0) {
        // 0x8016D294: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016D2DC;
    }
L_8016D294:
    // 0x8016D294: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016D298: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8016D29C: jal         0x8016D2F0
    // 0x8016D2A0: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016D2F0(rdram, ctx);
        goto after_0;
    // 0x8016D2A0: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x8016D2A4: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016D2A8: beql        $a0, $zero, L_8016D2BC
    if (ctx->r4 == 0) {
        // 0x8016D2AC: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_8016D2BC;
    }
    goto skip_0;
    // 0x8016D2AC: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x8016D2B0: jal         0x8016D278
    // 0x8016D2B4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016D278(rdram, ctx);
        goto after_1;
    // 0x8016D2B4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x8016D2B8: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_8016D2BC:
    // 0x8016D2BC: beql        $v0, $zero, L_8016D2E0
    if (ctx->r2 == 0) {
        // 0x8016D2C0: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016D2E0;
    }
    goto skip_1;
    // 0x8016D2C0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016D2C4: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x8016D2C8: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x8016D2CC: bnel        $t7, $zero, L_8016D2E0
    if (ctx->r15 != 0) {
        // 0x8016D2D0: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016D2E0;
    }
    goto skip_2;
    // 0x8016D2D0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016D2D4: bne         $v0, $zero, L_8016D294
    if (ctx->r2 != 0) {
        // 0x8016D2D8: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016D294;
    }
    // 0x8016D2D8: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016D2DC:
    // 0x8016D2DC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016D2E0:
    // 0x8016D2E0: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016D2E4: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016D2E8: jr          $ra
    // 0x8016D2EC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016D278--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016D2EC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_8016D2F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016D2F0: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x8016D2F4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016D2F8: sw          $a0, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r4;
    // 0x8016D2FC: sw          $a2, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r6;
    // 0x8016D300: lw          $v0, 0x74($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X74);
    // 0x8016D304: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016D308: lw          $a3, -0x1134($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X1134);
    // 0x8016D30C: beql        $v0, $zero, L_8016D558
    if (ctx->r2 == 0) {
        // 0x8016D310: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8016D558;
    }
    goto skip_0;
    // 0x8016D310: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x8016D314: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
    // 0x8016D318: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x8016D31C: beql        $t9, $zero, L_8016D558
    if (ctx->r25 == 0) {
        // 0x8016D320: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8016D558;
    }
    goto skip_1;
    // 0x8016D320: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x8016D324: lhu         $v1, 0x20($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X20);
    // 0x8016D328: beql        $v1, $zero, L_8016D558
    if (ctx->r3 == 0) {
        // 0x8016D32C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8016D558;
    }
    goto skip_2;
    // 0x8016D32C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x8016D330: lw          $t2, 0xC($a3)
    ctx->r10 = MEM_W(ctx->r7, 0XC);
    // 0x8016D334: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x8016D338: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016D33C: addu        $t5, $t2, $t4
    ctx->r13 = ADD32(ctx->r10, ctx->r12);
    // 0x8016D340: lhu         $v0, -0x2($t5)
    ctx->r2 = MEM_HU(ctx->r13, -0X2);
    // 0x8016D344: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016D348: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016D34C: beq         $v0, $at, L_8016D554
    if (ctx->r2 == ctx->r1) {
        // 0x8016D350: or          $t0, $v0, $zero
        ctx->r8 = ctx->r2 | 0;
            goto L_8016D554;
    }
    // 0x8016D350: or          $t0, $v0, $zero
    ctx->r8 = ctx->r2 | 0;
    // 0x8016D354: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x8016D358: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x8016D35C: subu        $t8, $t8, $v0
    ctx->r24 = SUB32(ctx->r24, ctx->r2);
    // 0x8016D360: lw          $t7, 0x4($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X4);
    // 0x8016D364: sll         $t8, $t8, 4
    ctx->r24 = S32(ctx->r24 << 4);
    // 0x8016D368: addiu       $t3, $sp, 0x30
    ctx->r11 = ADD32(ctx->r29, 0X30);
    // 0x8016D36C: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x8016D370: sw          $t9, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r25;
    // 0x8016D374: bne         $a1, $zero, L_8016D3B0
    if (ctx->r5 != 0) {
        // 0x8016D378: sw          $t3, 0x58($sp)
        MEM_W(0X58, ctx->r29) = ctx->r11;
            goto L_8016D3B0;
    }
    // 0x8016D378: sw          $t3, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r11;
    // 0x8016D37C: lwc1        $f4, 0x50($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X50);
    // 0x8016D380: swc1        $f4, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->f4.u32l;
    // 0x8016D384: lwc1        $f6, 0x54($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X54);
    // 0x8016D388: swc1        $f6, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->f6.u32l;
    // 0x8016D38C: lwc1        $f8, 0x58($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X58);
    // 0x8016D390: swc1        $f8, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->f8.u32l;
    // 0x8016D394: lh          $t4, 0x5C($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X5C);
    // 0x8016D398: sh          $t4, 0x36($sp)
    MEM_H(0X36, ctx->r29) = ctx->r12;
    // 0x8016D39C: lh          $t5, 0x5E($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X5E);
    // 0x8016D3A0: sh          $t5, 0x38($sp)
    MEM_H(0X38, ctx->r29) = ctx->r13;
    // 0x8016D3A4: lh          $t6, 0x60($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X60);
    // 0x8016D3A8: b           L_8016D3D8
    // 0x8016D3AC: sh          $t6, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = ctx->r14;
        goto L_8016D3D8;
    // 0x8016D3AC: sh          $t6, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = ctx->r14;
L_8016D3B0:
    // 0x8016D3B0: lw          $a0, 0x80($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X80);
    // 0x8016D3B4: addiu       $a1, $sp, 0x70
    ctx->r5 = ADD32(ctx->r29, 0X70);
    // 0x8016D3B8: addiu       $a2, $sp, 0x36
    ctx->r6 = ADD32(ctx->r29, 0X36);
    // 0x8016D3BC: addiu       $a3, $sp, 0x64
    ctx->r7 = ADD32(ctx->r29, 0X64);
    // 0x8016D3C0: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    // 0x8016D3C4: jal         0x80167F54
    // 0x8016D3C8: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    func_80167F54(rdram, ctx);
        goto after_0;
    // 0x8016D3C8: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    after_0:
    // 0x8016D3CC: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016D3D0: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016D3D4: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
L_8016D3D8:
    // 0x8016D3D8: lw          $t7, 0x88($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X88);
    // 0x8016D3DC: addiu       $v1, $sp, 0x70
    ctx->r3 = ADD32(ctx->r29, 0X70);
    // 0x8016D3E0: addiu       $v0, $sp, 0x30
    ctx->r2 = ADD32(ctx->r29, 0X30);
    // 0x8016D3E4: bne         $t7, $zero, L_8016D3F4
    if (ctx->r15 != 0) {
        // 0x8016D3E8: lui         $at, 0x4120
        ctx->r1 = S32(0X4120 << 16);
            goto L_8016D3F4;
    }
    // 0x8016D3E8: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x8016D3EC: sh          $zero, 0x3A($sp)
    MEM_H(0X3A, ctx->r29) = 0;
    // 0x8016D3F0: sh          $zero, 0x36($sp)
    MEM_H(0X36, ctx->r29) = 0;
L_8016D3F4:
    // 0x8016D3F4: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8016D3F8: addiu       $a0, $sp, 0x36
    ctx->r4 = ADD32(ctx->r29, 0X36);
    // 0x8016D3FC: lwc1        $f14, 0x0($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016D400: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016D404: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016D408: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x8016D40C: beql        $at, $zero, L_8016D43C
    if (ctx->r1 == 0) {
        // 0x8016D410: trunc.w.s   $f16, $f18
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
            goto L_8016D43C;
    }
    goto skip_3;
    // 0x8016D410: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    skip_3:
L_8016D414:
    // 0x8016D414: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x8016D418: lwc1        $f14, 0x4($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8016D41C: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016D420: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016D424: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x8016D428: mfc1        $t9, $f16
    ctx->r25 = (int32_t)ctx->f16.u32l;
    // 0x8016D42C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016D430: bne         $at, $zero, L_8016D414
    if (ctx->r1 != 0) {
        // 0x8016D434: sh          $t9, -0x4($v0)
        MEM_H(-0X4, ctx->r2) = ctx->r25;
            goto L_8016D414;
    }
    // 0x8016D434: sh          $t9, -0x4($v0)
    MEM_H(-0X4, ctx->r2) = ctx->r25;
    // 0x8016D438: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
L_8016D43C:
    // 0x8016D43C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016D440: mfc1        $t9, $f16
    ctx->r25 = (int32_t)ctx->f16.u32l;
    // 0x8016D444: nop

    // 0x8016D448: sh          $t9, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r25;
    // 0x8016D44C: lh          $t4, 0x36($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X36);
    // 0x8016D450: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x8016D454: sh          $t0, 0x54($sp)
    MEM_H(0X54, ctx->r29) = ctx->r8;
    // 0x8016D458: bne         $t4, $zero, L_8016D4B4
    if (ctx->r12 != 0) {
        // 0x8016D45C: sh          $t3, 0x56($sp)
        MEM_H(0X56, ctx->r29) = ctx->r11;
            goto L_8016D4B4;
    }
    // 0x8016D45C: sh          $t3, 0x56($sp)
    MEM_H(0X56, ctx->r29) = ctx->r11;
    // 0x8016D460: lh          $t5, 0x3A($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X3A);
    // 0x8016D464: bne         $t5, $zero, L_8016D4B4
    if (ctx->r13 != 0) {
        // 0x8016D468: nop
    
            goto L_8016D4B4;
    }
    // 0x8016D468: nop

    // 0x8016D46C: lw          $t2, 0x0($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X0);
    // 0x8016D470: addiu       $a0, $sp, 0x30
    ctx->r4 = ADD32(ctx->r29, 0X30);
    // 0x8016D474: jal         0x8016EA9C
    // 0x8016D478: lw          $a1, 0xC($t2)
    ctx->r5 = MEM_W(ctx->r10, 0XC);
    func_8016EA9C(rdram, ctx);
        goto after_1;
    // 0x8016D478: lw          $a1, 0xC($t2)
    ctx->r5 = MEM_W(ctx->r10, 0XC);
    after_1:
    // 0x8016D47C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016D480: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016D484: lw          $a0, 0x5C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X5C);
    // 0x8016D488: addiu       $a1, $v0, 0x1C0
    ctx->r5 = ADD32(ctx->r2, 0X1C0);
    // 0x8016D48C: addiu       $a2, $v0, 0x44
    ctx->r6 = ADD32(ctx->r2, 0X44);
    // 0x8016D490: lw          $a3, 0x19C($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X19C);
    // 0x8016D494: jal         0x8016ECF0
    // 0x8016D498: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    func_8016ECF0(rdram, ctx);
        goto after_2;
    // 0x8016D498: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    after_2:
    // 0x8016D49C: beq         $v0, $zero, L_8016D554
    if (ctx->r2 == 0) {
        // 0x8016D4A0: addiu       $a0, $sp, 0x30
        ctx->r4 = ADD32(ctx->r29, 0X30);
            goto L_8016D554;
    }
    // 0x8016D4A0: addiu       $a0, $sp, 0x30
    ctx->r4 = ADD32(ctx->r29, 0X30);
    // 0x8016D4A4: jal         0x8016D868
    // 0x8016D4A8: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    func_8016D868(rdram, ctx);
        goto after_3;
    // 0x8016D4A8: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    after_3:
    // 0x8016D4AC: b           L_8016D558
    // 0x8016D4B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8016D558;
    // 0x8016D4B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8016D4B4:
    // 0x8016D4B4: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016D4B8: lw          $a1, -0x1130($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1130);
    // 0x8016D4BC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016D4C0: blez        $a1, L_8016D50C
    if (SIGNED(ctx->r5) <= 0) {
        // 0x8016D4C4: nop
    
            goto L_8016D50C;
    }
    // 0x8016D4C4: nop

    // 0x8016D4C8: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x8016D4CC: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016D4D0: lw          $v0, -0x1128($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X1128);
    // 0x8016D4D4: lw          $v1, 0x10($t6)
    ctx->r3 = MEM_W(ctx->r14, 0X10);
L_8016D4D8:
    // 0x8016D4D8: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x8016D4DC: bnel        $v1, $t7, L_8016D500
    if (ctx->r3 != ctx->r15) {
        // 0x8016D4E0: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_8016D500;
    }
    goto skip_4;
    // 0x8016D4E0: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    skip_4:
    // 0x8016D4E4: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    // 0x8016D4E8: addiu       $t8, $v1, 0x6C
    ctx->r24 = ADD32(ctx->r3, 0X6C);
    // 0x8016D4EC: addiu       $t9, $v1, 0x9C
    ctx->r25 = ADD32(ctx->r3, 0X9C);
    // 0x8016D4F0: sw          $t8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r24;
    // 0x8016D4F4: b           L_8016D50C
    // 0x8016D4F8: sw          $t9, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r25;
        goto L_8016D50C;
    // 0x8016D4F8: sw          $t9, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r25;
    // 0x8016D4FC: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_8016D500:
    // 0x8016D500: slt         $at, $a0, $a1
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x8016D504: bne         $at, $zero, L_8016D4D8
    if (ctx->r1 != 0) {
        // 0x8016D508: addiu       $v0, $v0, 0x8
        ctx->r2 = ADD32(ctx->r2, 0X8);
            goto L_8016D4D8;
    }
    // 0x8016D508: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
L_8016D50C:
    // 0x8016D50C: beql        $a0, $a1, L_8016D558
    if (ctx->r4 == ctx->r5) {
        // 0x8016D510: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8016D558;
    }
    goto skip_5;
    // 0x8016D510: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_5:
    // 0x8016D514: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x8016D518: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x8016D51C: jal         0x8016EA9C
    // 0x8016D520: lw          $a1, 0xC($t3)
    ctx->r5 = MEM_W(ctx->r11, 0XC);
    func_8016EA9C(rdram, ctx);
        goto after_4;
    // 0x8016D520: lw          $a1, 0xC($t3)
    ctx->r5 = MEM_W(ctx->r11, 0XC);
    after_4:
    // 0x8016D524: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016D528: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016D52C: lw          $a0, 0x5C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X5C);
    // 0x8016D530: addiu       $a1, $v0, 0x1C0
    ctx->r5 = ADD32(ctx->r2, 0X1C0);
    // 0x8016D534: addiu       $a2, $v0, 0x44
    ctx->r6 = ADD32(ctx->r2, 0X44);
    // 0x8016D538: lw          $a3, 0x19C($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X19C);
    // 0x8016D53C: jal         0x8016ECF0
    // 0x8016D540: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    func_8016ECF0(rdram, ctx);
        goto after_5;
    // 0x8016D540: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    after_5:
    // 0x8016D544: beq         $v0, $zero, L_8016D554
    if (ctx->r2 == 0) {
        // 0x8016D548: lw          $a0, 0x58($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X58);
            goto L_8016D554;
    }
    // 0x8016D548: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x8016D54C: jal         0x8016D9B4
    // 0x8016D550: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    func_8016D9B4(rdram, ctx);
        goto after_6;
    // 0x8016D550: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    after_6:
L_8016D554:
    // 0x8016D554: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8016D558:
    // 0x8016D558: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    // 0x8016D55C: jr          $ra
    // 0x8016D560: nop

    return;
    // 0x8016D560: nop

;}
RECOMP_FUNC void func_8016D564(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016D564: jr          $ra
    // 0x8016D568: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x8016D568: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8016D56C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016D56C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8016D570: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x8016D574: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016D578: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016D57C: or          $s5, $a0, $zero
    ctx->r21 = ctx->r4 | 0;
    // 0x8016D580: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8016D584: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x8016D588: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x8016D58C: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8016D590: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x8016D594: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x8016D598: beq         $a0, $zero, L_8016D6D8
    if (ctx->r4 == 0) {
        // 0x8016D59C: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_8016D6D8;
    }
    // 0x8016D59C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016D5A0: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x8016D5A4: addiu       $s7, $s7, -0x9FC
    ctx->r23 = ADD32(ctx->r23, -0X9FC);
    // 0x8016D5A8: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x8016D5AC: addiu       $s0, $a0, 0x8
    ctx->r16 = ADD32(ctx->r4, 0X8);
    // 0x8016D5B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016D5B4: addiu       $a1, $v0, 0x184
    ctx->r5 = ADD32(ctx->r2, 0X184);
    // 0x8016D5B8: jal         0x80169BD8
    // 0x8016D5BC: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    func_80169BD8(rdram, ctx);
        goto after_0;
    // 0x8016D5BC: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    after_0:
    // 0x8016D5C0: beq         $v0, $zero, L_8016D6D8
    if (ctx->r2 == 0) {
        // 0x8016D5C4: lui         $a0, 0x801A
        ctx->r4 = S32(0X801A << 16);
            goto L_8016D6D8;
    }
    // 0x8016D5C4: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016D5C8: addiu       $a0, $a0, -0x1110
    ctx->r4 = ADD32(ctx->r4, -0X1110);
    // 0x8016D5CC: jal         0x80169B88
    // 0x8016D5D0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80169B88(rdram, ctx);
        goto after_1;
    // 0x8016D5D0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x8016D5D4: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016D5D8: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x8016D5DC: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8016D5E0: addiu       $s6, $s6, 0x2490
    ctx->r22 = ADD32(ctx->r22, 0X2490);
    // 0x8016D5E4: addiu       $s0, $s0, -0x111C
    ctx->r16 = ADD32(ctx->r16, -0X111C);
    // 0x8016D5E8: addiu       $t6, $t6, -0x1680
    ctx->r14 = ADD32(ctx->r14, -0X1680);
    // 0x8016D5EC: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x8016D5F0: sw          $t6, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r14;
    // 0x8016D5F4: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016D5F8: addiu       $a0, $a0, -0x9F8
    ctx->r4 = ADD32(ctx->r4, -0X9F8);
    // 0x8016D5FC: jal         0x80000F30
    // 0x8016D600: lw          $a1, 0x0($s5)
    ctx->r5 = MEM_W(ctx->r21, 0X0);
    bzero_recomp(rdram, ctx);
        goto after_2;
    // 0x8016D600: lw          $a1, 0x0($s5)
    ctx->r5 = MEM_W(ctx->r21, 0X0);
    after_2:
    // 0x8016D604: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016D608: sw          $s1, -0x10F8($at)
    MEM_W(-0X10F8, ctx->r1) = ctx->r17;
    // 0x8016D60C: lw          $a1, 0x0($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X0);
    // 0x8016D610: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016D614: jal         0x8016D704
    // 0x8016D618: addiu       $a1, $a1, 0x184
    ctx->r5 = ADD32(ctx->r5, 0X184);
    func_8016D704(rdram, ctx);
        goto after_3;
    // 0x8016D618: addiu       $a1, $a1, 0x184
    ctx->r5 = ADD32(ctx->r5, 0X184);
    after_3:
    // 0x8016D61C: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x8016D620: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x8016D624: addiu       $s4, $zero, 0x30
    ctx->r20 = ADD32(0, 0X30);
    // 0x8016D628: blez        $v1, L_8016D6D8
    if (SIGNED(ctx->r3) <= 0) {
        // 0x8016D62C: addiu       $s3, $zero, 0x28
        ctx->r19 = ADD32(0, 0X28);
            goto L_8016D6D8;
    }
    // 0x8016D62C: addiu       $s3, $zero, 0x28
    ctx->r19 = ADD32(0, 0X28);
    // 0x8016D630: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
L_8016D634:
    // 0x8016D634: lw          $t1, 0x4($s5)
    ctx->r9 = MEM_W(ctx->r21, 0X4);
    // 0x8016D638: addu        $t8, $t7, $s1
    ctx->r24 = ADD32(ctx->r15, ctx->r17);
    // 0x8016D63C: lh          $t9, 0x0($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X0);
    // 0x8016D640: multu       $t9, $s3
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r19)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016D644: mflo        $t0
    ctx->r8 = lo;
    // 0x8016D648: addu        $s0, $t0, $t1
    ctx->r16 = ADD32(ctx->r8, ctx->r9);
    // 0x8016D64C: lh          $t2, 0x26($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X26);
    // 0x8016D650: beql        $t2, $zero, L_8016D664
    if (ctx->r10 == 0) {
        // 0x8016D654: lh          $t4, 0x24($s0)
        ctx->r12 = MEM_H(ctx->r16, 0X24);
            goto L_8016D664;
    }
    goto skip_0;
    // 0x8016D654: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
    skip_0:
    // 0x8016D658: b           L_8016D6C8
    // 0x8016D65C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016D6C8;
    // 0x8016D65C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x8016D660: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
L_8016D664:
    // 0x8016D664: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x8016D668: addiu       $a0, $s0, 0xC
    ctx->r4 = ADD32(ctx->r16, 0XC);
    // 0x8016D66C: multu       $t4, $s4
    result = U64(U32(ctx->r12)) * U64(U32(ctx->r20)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016D670: lw          $t3, 0x4($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X4);
    // 0x8016D674: addiu       $a1, $v0, 0x184
    ctx->r5 = ADD32(ctx->r2, 0X184);
    // 0x8016D678: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    // 0x8016D67C: lw          $a3, 0x19C($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X19C);
    // 0x8016D680: mflo        $t5
    ctx->r13 = lo;
    // 0x8016D684: addu        $s2, $t3, $t5
    ctx->r18 = ADD32(ctx->r11, ctx->r13);
    // 0x8016D688: jal         0x8016ECF0
    // 0x8016D68C: nop

    func_8016ECF0(rdram, ctx);
        goto after_4;
    // 0x8016D68C: nop

    after_4:
    // 0x8016D690: bne         $v0, $zero, L_8016D6A8
    if (ctx->r2 != 0) {
        // 0x8016D694: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016D6A8;
    }
    // 0x8016D694: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016D698: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8016D69C: lw          $v1, -0x111C($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X111C);
    // 0x8016D6A0: b           L_8016D6C8
    // 0x8016D6A4: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016D6C8;
    // 0x8016D6A4: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_8016D6A8:
    // 0x8016D6A8: jal         0x8016EA9C
    // 0x8016D6AC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_8016EA9C(rdram, ctx);
        goto after_5;
    // 0x8016D6AC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_5:
    // 0x8016D6B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016D6B4: jal         0x8016D868
    // 0x8016D6B8: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_8016D868(rdram, ctx);
        goto after_6;
    // 0x8016D6B8: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_6:
    // 0x8016D6BC: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8016D6C0: lw          $v1, -0x111C($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X111C);
    // 0x8016D6C4: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_8016D6C8:
    // 0x8016D6C8: addiu       $s1, $s1, 0x2
    ctx->r17 = ADD32(ctx->r17, 0X2);
    // 0x8016D6CC: slt         $at, $s1, $v0
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x8016D6D0: bnel        $at, $zero, L_8016D634
    if (ctx->r1 != 0) {
        // 0x8016D6D4: lw          $t7, 0x0($s6)
        ctx->r15 = MEM_W(ctx->r22, 0X0);
            goto L_8016D634;
    }
    goto skip_1;
    // 0x8016D6D4: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
    skip_1:
L_8016D6D8:
    // 0x8016D6D8: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x8016D6DC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016D6E0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016D6E4: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8016D6E8: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x8016D6EC: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x8016D6F0: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x8016D6F4: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x8016D6F8: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x8016D6FC: jr          $ra
    // 0x8016D700: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x8016D700: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_8016D704(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016D704 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016D704++;
    if (lod_text_guard_depth_8016D704 > 128) {
        if (!lod_text_guard_logged_8016D704) {
            lod_text_guard_logged_8016D704 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016D704 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016D704--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016D704: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016D708: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016D70C: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x8016D710: sw          $a0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r4;
    // 0x8016D714: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016D718: sw          $a1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r5;
    // 0x8016D71C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016D720: addiu       $a0, $a0, -0x1110
    ctx->r4 = ADD32(ctx->r4, -0X1110);
    // 0x8016D724: lw          $a3, 0x19C($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X19C);
    // 0x8016D728: addiu       $a1, $v0, 0x14
    ctx->r5 = ADD32(ctx->r2, 0X14);
    // 0x8016D72C: jal         0x8016ECF0
    // 0x8016D730: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    func_8016ECF0(rdram, ctx);
        goto after_0;
    // 0x8016D730: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    after_0:
    // 0x8016D734: beq         $v0, $zero, L_8016D858
    if (ctx->r2 == 0) {
        // 0x8016D738: lh          $t6, 0x5A($sp)
        ctx->r14 = MEM_H(ctx->r29, 0X5A);
            goto L_8016D858;
    }
    // 0x8016D738: lh          $t6, 0x5A($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X5A);
    // 0x8016D73C: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x8016D740: lw          $t8, -0x10F8($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X10F8);
    // 0x8016D744: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x8016D748: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016D74C: addu        $a3, $t7, $t8
    ctx->r7 = ADD32(ctx->r15, ctx->r24);
    // 0x8016D750: lh          $v1, 0x0($a3)
    ctx->r3 = MEM_H(ctx->r7, 0X0);
    // 0x8016D754: bnel        $v1, $at, L_8016D7E4
    if (ctx->r3 != ctx->r1) {
        // 0x8016D758: lh          $t6, 0x2($a3)
        ctx->r14 = MEM_H(ctx->r7, 0X2);
            goto L_8016D7E4;
    }
    goto skip_0;
    // 0x8016D758: lh          $t6, 0x2($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X2);
    skip_0:
    // 0x8016D75C: lh          $a2, 0x2($a3)
    ctx->r6 = MEM_H(ctx->r7, 0X2);
    // 0x8016D760: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x8016D764: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016D768: blez        $a2, L_8016D858
    if (SIGNED(ctx->r6) <= 0) {
        // 0x8016D76C: lui         $t2, 0x8019
        ctx->r10 = S32(0X8019 << 16);
            goto L_8016D858;
    }
    // 0x8016D76C: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x8016D770: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x8016D774: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016D778: addiu       $a1, $a1, -0x111C
    ctx->r5 = ADD32(ctx->r5, -0X111C);
    // 0x8016D77C: addiu       $t0, $t0, -0x9F8
    ctx->r8 = ADD32(ctx->r8, -0X9F8);
    // 0x8016D780: addiu       $t2, $t2, 0x2490
    ctx->r10 = ADD32(ctx->r10, 0X2490);
    // 0x8016D784: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
L_8016D788:
    // 0x8016D788: lh          $t9, 0x0($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X0);
    // 0x8016D78C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016D790: addu        $a0, $t0, $t9
    ctx->r4 = ADD32(ctx->r8, ctx->r25);
    // 0x8016D794: lbu         $t3, 0x0($a0)
    ctx->r11 = MEM_BU(ctx->r4, 0X0);
    // 0x8016D798: bnel        $t3, $zero, L_8016D7D0
    if (ctx->r11 != 0) {
        // 0x8016D79C: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_8016D7D0;
    }
    goto skip_1;
    // 0x8016D79C: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_1:
    // 0x8016D7A0: lw          $t6, 0x0($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X0);
    // 0x8016D7A4: lw          $t5, 0x0($t2)
    ctx->r13 = MEM_W(ctx->r10, 0X0);
    // 0x8016D7A8: sb          $t1, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r9;
    // 0x8016D7AC: lh          $t4, 0x0($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X0);
    // 0x8016D7B0: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x8016D7B4: addu        $t8, $t5, $t7
    ctx->r24 = ADD32(ctx->r13, ctx->r15);
    // 0x8016D7B8: sh          $t4, 0x0($t8)
    MEM_H(0X0, ctx->r24) = ctx->r12;
    // 0x8016D7BC: lw          $t9, 0x0($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X0);
    // 0x8016D7C0: addiu       $t3, $t9, 0x1
    ctx->r11 = ADD32(ctx->r25, 0X1);
    // 0x8016D7C4: sw          $t3, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r11;
    // 0x8016D7C8: lh          $a2, 0x2($a3)
    ctx->r6 = MEM_H(ctx->r7, 0X2);
    // 0x8016D7CC: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
L_8016D7D0:
    // 0x8016D7D0: bne         $at, $zero, L_8016D788
    if (ctx->r1 != 0) {
        // 0x8016D7D4: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
            goto L_8016D788;
    }
    // 0x8016D7D4: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016D7D8: b           L_8016D85C
    // 0x8016D7DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8016D85C;
    // 0x8016D7DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8016D7E0: lh          $t6, 0x2($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X2);
L_8016D7E4:
    // 0x8016D7E4: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016D7E8: addiu       $t7, $t7, -0x1110
    ctx->r15 = ADD32(ctx->r15, -0X1110);
    // 0x8016D7EC: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x8016D7F0: sll         $t5, $v1, 2
    ctx->r13 = S32(ctx->r3 << 2);
    // 0x8016D7F4: addu        $v0, $t5, $t7
    ctx->r2 = ADD32(ctx->r13, ctx->r15);
    // 0x8016D7F8: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016D7FC: lwc1        $f6, 0x0($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016D800: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    // 0x8016D804: swc1        $f6, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f6.u32l;
    // 0x8016D808: swc1        $f0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f0.u32l;
    // 0x8016D80C: lh          $a0, 0x4($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X4);
    // 0x8016D810: swc1        $f0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f0.u32l;
    // 0x8016D814: sw          $a3, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r7;
    // 0x8016D818: jal         0x8016D704
    // 0x8016D81C: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_8016D704(rdram, ctx);
        goto after_1;
    // 0x8016D81C: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x8016D820: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x8016D824: lwc1        $f0, 0x38($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016D828: lwc1        $f8, 0x48($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016D82C: lwc1        $f10, 0xC($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0XC);
    // 0x8016D830: lw          $a3, 0x44($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X44);
    // 0x8016D834: swc1        $f0, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f0.u32l;
    // 0x8016D838: swc1        $f8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f8.u32l;
    // 0x8016D83C: swc1        $f10, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f10.u32l;
    // 0x8016D840: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    // 0x8016D844: jal         0x8016D704
    // 0x8016D848: lh          $a0, 0x6($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X6);
    func_8016D704(rdram, ctx);
        goto after_2;
    // 0x8016D848: lh          $a0, 0x6($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X6);
    after_2:
    // 0x8016D84C: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x8016D850: lwc1        $f16, 0x48($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016D854: swc1        $f16, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f16.u32l;
L_8016D858:
    // 0x8016D858: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8016D85C:
    // 0x8016D85C: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    // 0x8016D860: jr          $ra
    // 0x8016D864: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016D704--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016D864: nop

;}
RECOMP_FUNC void func_8016D868(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016D868: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8016D86C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016D870: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8016D874: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8016D878: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016D87C: or          $s3, $a1, $zero
    ctx->r19 = ctx->r5 | 0;
    // 0x8016D880: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8016D884: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x8016D888: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x8016D88C: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x8016D890: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x8016D894: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8016D898: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8016D89C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8016D8A0: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x8016D8A4: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    // 0x8016D8A8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8016D8AC: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
L_8016D8B0:
    // 0x8016D8B0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016D8B4: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
L_8016D8B8:
    // 0x8016D8B8: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016D8BC: sll         $t7, $a2, 2
    ctx->r15 = S32(ctx->r6 << 2);
    // 0x8016D8C0: subu        $t7, $t7, $a2
    ctx->r15 = SUB32(ctx->r15, ctx->r6);
    // 0x8016D8C4: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x8016D8C8: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x8016D8CC: lwc1        $f4, 0x18($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X18);
    // 0x8016D8D0: addu        $t9, $t8, $v0
    ctx->r25 = ADD32(ctx->r24, ctx->r2);
    // 0x8016D8D4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016D8D8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016D8DC: bne         $v0, $a0, L_8016D8B8
    if (ctx->r2 != ctx->r4) {
        // 0x8016D8E0: swc1        $f4, 0x5C($t9)
        MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
            goto L_8016D8B8;
    }
    // 0x8016D8E0: swc1        $f4, 0x5C($t9)
    MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
    // 0x8016D8E4: addiu       $a2, $a2, 0x1
    ctx->r6 = ADD32(ctx->r6, 0X1);
    // 0x8016D8E8: slti        $at, $a2, 0x2
    ctx->r1 = SIGNED(ctx->r6) < 0X2 ? 1 : 0;
    // 0x8016D8EC: bne         $at, $zero, L_8016D8B0
    if (ctx->r1 != 0) {
        // 0x8016D8F0: addiu       $a3, $a3, 0xC
        ctx->r7 = ADD32(ctx->r7, 0XC);
            goto L_8016D8B0;
    }
    // 0x8016D8F0: addiu       $a3, $a3, 0xC
    ctx->r7 = ADD32(ctx->r7, 0XC);
    // 0x8016D8F4: addiu       $s1, $s3, 0x4
    ctx->r17 = ADD32(ctx->r19, 0X4);
    // 0x8016D8F8: lui         $fp, 0x801A
    ctx->r30 = S32(0X801A << 16);
    // 0x8016D8FC: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x8016D900: lui         $s6, 0x801A
    ctx->r22 = S32(0X801A << 16);
    // 0x8016D904: lui         $s5, 0x8019
    ctx->r21 = S32(0X8019 << 16);
    // 0x8016D908: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016D90C: addiu       $s4, $s4, -0x1118
    ctx->r20 = ADD32(ctx->r20, -0X1118);
    // 0x8016D910: addiu       $s5, $s5, 0x2494
    ctx->r21 = ADD32(ctx->r21, 0X2494);
    // 0x8016D914: addiu       $s6, $s6, -0x1580
    ctx->r22 = ADD32(ctx->r22, -0X1580);
    // 0x8016D918: addiu       $s7, $s7, -0x978
    ctx->r23 = ADD32(ctx->r23, -0X978);
    // 0x8016D91C: addiu       $fp, $fp, -0x10F4
    ctx->r30 = ADD32(ctx->r30, -0X10F4);
    // 0x8016D920: addiu       $s3, $zero, 0x8
    ctx->r19 = ADD32(0, 0X8);
    // 0x8016D924: addiu       $s2, $zero, 0x4
    ctx->r18 = ADD32(0, 0X4);
L_8016D928:
    // 0x8016D928: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x8016D92C: beql        $v0, $zero, L_8016D97C
    if (ctx->r2 == 0) {
        // 0x8016D930: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_8016D97C;
    }
    goto skip_0;
    // 0x8016D930: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    skip_0:
    // 0x8016D934: lw          $t0, 0x18($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X18);
    // 0x8016D938: lw          $t1, 0x0($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X0);
    // 0x8016D93C: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x8016D940: sw          $t0, 0x168($t1)
    MEM_W(0X168, ctx->r9) = ctx->r8;
    // 0x8016D944: sw          $zero, 0x0($s4)
    MEM_W(0X0, ctx->r20) = 0;
    // 0x8016D948: sw          $s6, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r22;
    // 0x8016D94C: lw          $t2, 0x0($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X0);
    // 0x8016D950: jal         0x80000F30
    // 0x8016D954: lh          $a1, 0x1C($t2)
    ctx->r5 = MEM_H(ctx->r10, 0X1C);
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8016D954: lh          $a1, 0x1C($t2)
    ctx->r5 = MEM_H(ctx->r10, 0X1C);
    after_0:
    // 0x8016D958: lw          $t3, 0xC($s1)
    ctx->r11 = MEM_W(ctx->r17, 0XC);
    // 0x8016D95C: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x8016D960: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016D964: sw          $t3, 0x0($fp)
    MEM_W(0X0, ctx->r30) = ctx->r11;
    // 0x8016D968: jal         0x8016DAC4
    // 0x8016D96C: addiu       $a1, $a1, 0x1C0
    ctx->r5 = ADD32(ctx->r5, 0X1C0);
    func_8016DAC4(rdram, ctx);
        goto after_1;
    // 0x8016D96C: addiu       $a1, $a1, 0x1C0
    ctx->r5 = ADD32(ctx->r5, 0X1C0);
    after_1:
    // 0x8016D970: jal         0x8016DD2C
    // 0x8016D974: nop

    func_8016DD2C(rdram, ctx);
        goto after_2;
    // 0x8016D974: nop

    after_2:
    // 0x8016D978: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
L_8016D97C:
    // 0x8016D97C: bne         $s2, $s3, L_8016D928
    if (ctx->r18 != ctx->r19) {
        // 0x8016D980: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_8016D928;
    }
    // 0x8016D980: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x8016D984: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x8016D988: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016D98C: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8016D990: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8016D994: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8016D998: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8016D99C: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x8016D9A0: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x8016D9A4: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x8016D9A8: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x8016D9AC: jr          $ra
    // 0x8016D9B0: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x8016D9B0: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
