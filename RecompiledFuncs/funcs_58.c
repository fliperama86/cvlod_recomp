#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

// Round 17 (Task B): LOD_FIX_TEXT_MEASURE_GUARD, defense-in-depth. The
// seven functions below (func_8016DAC4/8016F71C/8016F794/8016FE68/
// 801701DC/801728FC/801729E8) were found self-recursive by a systematic
// sweep of every RECOMP_FUNC in the 0x80160000-0x80180000 vaddr range (the
// text/figure family already guarded piecemeal in funcs_57.c across rounds
// 14/15 -- func_80168AA0/func_8016B878); see
// docs/issue27-31-ni0e-findings.md round 17 for the sweep methodology and
// full function list. Same depth-128-cap pattern: a strict no-op skip of
// the entire function body once nesting would exceed 128 levels, which
// only ever fires on an already-corrupted/circular walk. Independent of
// LOD_ENABLE_NI0E_TRACE (must compile/run standalone).
#ifndef LOD_FIX_TEXT_MEASURE_GUARD
#define LOD_FIX_TEXT_MEASURE_GUARD 0
#endif

#if LOD_FIX_TEXT_MEASURE_GUARD
#include <stdio.h>

static int lod_text_guard_depth_8016DAC4 = 0;
static int lod_text_guard_logged_8016DAC4 = 0;
static int lod_text_guard_depth_8016F71C = 0;
static int lod_text_guard_logged_8016F71C = 0;
static int lod_text_guard_depth_8016F794 = 0;
static int lod_text_guard_logged_8016F794 = 0;
static int lod_text_guard_depth_8016FE68 = 0;
static int lod_text_guard_logged_8016FE68 = 0;
static int lod_text_guard_depth_801701DC = 0;
static int lod_text_guard_logged_801701DC = 0;
static int lod_text_guard_depth_801728FC = 0;
static int lod_text_guard_logged_801728FC = 0;
static int lod_text_guard_depth_801729E8 = 0;
static int lod_text_guard_logged_801729E8 = 0;

// Round 20: func_8016F310 (below) is a second, structurally-parallel
// text-draw dispatcher to func_801682D0/func_8016AE10 (funcs_57.c, rounds
// 15/18): it independently re-reads the SAME global workspace pointer from
// RDRAM 0x8019EECC (`lui $s2,0x801A; lw $s2,-0x1134($s2)`, the identical
// instruction sequence) into its own $s2, then feeds that struct's
// +0x10/+0x14/+0x18 fixed slots and +0x20-count/+0x24-array/+0x38-array
// variable-length run into func_8016FBB0 instead of func_8016890C -- a
// different, unguarded draw primitive; see the per-call guard below.
// func_8016F310 has NO workspace validation of any kind: its only caller,
// func_8003F72C (funcs_20.c), null-checks the same global before calling at
// all, but never range-checks it, so a stale non-null pointer (the same
// class of corruption round 18/19 documented elsewhere -- freed/
// uninitialized heap data surviving a map transition) reaches this
// function's $s2 completely unguarded. The repro20 crash (Signal 10 at
// rdram+0x8707F800, stable across three captures, native frame directly
// inside func_8016F310 itself rather than any callee) is consistent with
// $s2 itself (or an early field read at a small offset from it) holding
// that stale value -- exactly the shape
// lod_text_guard_workspace_invalid/lod_text_guard_array_invalid below were
// built to catch for func_801682D0. Duplicated here rather than shared via
// a header because this function lives in a separate translation unit,
// following the same duplication-across-TUs practice as round 19's
// textfile-life probe.
static inline int lod_text_guard_addr_ok(uint32_t addr) {
    if (addr == 0) {
        return 0;
    }
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return phys <= 0x800000u - 4u;
}

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

static int lod_text_guard_workspace_8016F310_logged = 0;
static int lod_text_guard_array_8016F310_logged = 0;

// func_8016FBB0's own incoming-pointer range check: unlike func_8016890C
// (round 14 guard, funcs_57.c), func_8016FBB0 only null-checks a0
// (`beq $a0,$zero,...` below) before immediately dereferencing it
// (obj+0xC/+0x18/+0x8/+0x14/+0x10/+0x1C, unguarded float loads starting at
// 0x8016FBF0). It has exactly one caller (func_8016F310, this file), fed
// obj pointers straight from the same workspace struct's +0x10/+0x14/+0x18
// fields and +0x24 array run the two guards above validate the
// CONTAINER/BASE of, not each individual element's own value -- so even
// with the workspace/array-base guards in place, one stale element could
// still reach here. Same "standard range check" and early-return shape as
// func_8016890C's guard.
static int lod_text_guard_fbb0_logged = 0;
#endif  // LOD_FIX_TEXT_MEASURE_GUARD

// Round 20: LOD_ENABLE_NI0E_TRACE textpipe2 probe. func_8016F310 (below)
// re-reads the same RDRAM 0x8019EECC workspace global func_801682D0/
// func_8016AE10 (funcs_57.c) use, into its own $s2, with no validation at
// all -- see the LOD_FIX_TEXT_MEASURE_GUARD comment above for the full
// repro20 crash analysis. This probe dumps $s2 and the same class of fields
// funcs_57.c's figsrc-entry-fields probe already dumps for func_801682D0
// (+0x10/+0x14/+0x18 fixed slots, +0x20/+0x24/+0x38 array-run fields), plus
// +0x4 (the very first field this function's own code dereferences, fed as
// an arg to its first callee before anything else runs), at every call to
// func_8016F310, so the next capture shows what the stale data actually is.
#if LOD_ENABLE_NI0E_TRACE
#include <stdbool.h>
#include <stdio.h>

static uint32_t lod_ni0e_textpipe2_calls = 0;

static inline bool lod_ni0e_textpipe2_addr_ok(uint32_t addr) {
    if (addr == 0) {
        return false;
    }
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return phys <= 0x800000u - 4u;
}

static void lod_ni0e_textpipe2_probe(uint8_t* rdram, uint32_t s2) {
    lod_ni0e_textpipe2_calls++;
    const bool periodic = (lod_ni0e_textpipe2_calls % 300u) == 0u;
    if (lod_ni0e_textpipe2_calls > 40u && !periodic) {
        return;
    }
    if (!lod_ni0e_textpipe2_addr_ok(s2)) {
        fprintf(stderr,
                "[NI0E_TRACE] textpipe2 call=%u s2=0x%08X (invalid, skipped)\n",
                lod_ni0e_textpipe2_calls, s2);
        return;
    }
    const gpr s2_gpr = (gpr)(int32_t)s2;
    const uint32_t f4  = (uint32_t)MEM_W(0x4,  s2_gpr);
    const uint32_t f10 = (uint32_t)MEM_W(0x10, s2_gpr);
    const uint32_t f14 = (uint32_t)MEM_W(0x14, s2_gpr);
    const uint32_t f18 = (uint32_t)MEM_W(0x18, s2_gpr);
    const uint32_t f20 = (uint32_t)MEM_W(0x20, s2_gpr);
    const uint32_t f24 = (uint32_t)MEM_W(0x24, s2_gpr);
    const uint32_t f38 = (uint32_t)MEM_W(0x38, s2_gpr);
    fprintf(stderr,
            "[NI0E_TRACE] textpipe2 call=%u s2=0x%08X f4=0x%08X f10=0x%08X "
            "f14=0x%08X f18=0x%08X f20=0x%08X f24=0x%08X f38=0x%08X\n",
            lod_ni0e_textpipe2_calls, s2, f4, f10, f14, f18, f20, f24, f38);
}
#endif  // LOD_ENABLE_NI0E_TRACE

RECOMP_FUNC void func_8016D9B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016D9B4: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8016D9B8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016D9BC: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8016D9C0: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x8016D9C4: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016D9C8: or          $s3, $a1, $zero
    ctx->r19 = ctx->r5 | 0;
    // 0x8016D9CC: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8016D9D0: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x8016D9D4: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x8016D9D8: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8016D9DC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8016D9E0: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8016D9E4: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x8016D9E8: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    // 0x8016D9EC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8016D9F0: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
L_8016D9F4:
    // 0x8016D9F4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016D9F8: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
L_8016D9FC:
    // 0x8016D9FC: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016DA00: sll         $t7, $a2, 2
    ctx->r15 = S32(ctx->r6 << 2);
    // 0x8016DA04: subu        $t7, $t7, $a2
    ctx->r15 = SUB32(ctx->r15, ctx->r6);
    // 0x8016DA08: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x8016DA0C: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x8016DA10: lwc1        $f4, 0x18($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X18);
    // 0x8016DA14: addu        $t9, $t8, $v0
    ctx->r25 = ADD32(ctx->r24, ctx->r2);
    // 0x8016DA18: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016DA1C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016DA20: bne         $v0, $a0, L_8016D9FC
    if (ctx->r2 != ctx->r4) {
        // 0x8016DA24: swc1        $f4, 0x5C($t9)
        MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
            goto L_8016D9FC;
    }
    // 0x8016DA24: swc1        $f4, 0x5C($t9)
    MEM_W(0X5C, ctx->r25) = ctx->f4.u32l;
    // 0x8016DA28: addiu       $a2, $a2, 0x1
    ctx->r6 = ADD32(ctx->r6, 0X1);
    // 0x8016DA2C: slti        $at, $a2, 0x2
    ctx->r1 = SIGNED(ctx->r6) < 0X2 ? 1 : 0;
    // 0x8016DA30: bne         $at, $zero, L_8016D9F4
    if (ctx->r1 != 0) {
        // 0x8016DA34: addiu       $a3, $a3, 0xC
        ctx->r7 = ADD32(ctx->r7, 0XC);
            goto L_8016D9F4;
    }
    // 0x8016DA34: addiu       $a3, $a3, 0xC
    ctx->r7 = ADD32(ctx->r7, 0XC);
    // 0x8016DA38: addiu       $s1, $s3, 0x4
    ctx->r17 = ADD32(ctx->r19, 0X4);
    // 0x8016DA3C: lui         $s6, 0x801A
    ctx->r22 = S32(0X801A << 16);
    // 0x8016DA40: lui         $s5, 0x8019
    ctx->r21 = S32(0X8019 << 16);
    // 0x8016DA44: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016DA48: addiu       $s4, $s4, -0x1118
    ctx->r20 = ADD32(ctx->r20, -0X1118);
    // 0x8016DA4C: addiu       $s5, $s5, 0x2494
    ctx->r21 = ADD32(ctx->r21, 0X2494);
    // 0x8016DA50: addiu       $s6, $s6, -0x750
    ctx->r22 = ADD32(ctx->r22, -0X750);
    // 0x8016DA54: addiu       $s3, $zero, 0x8
    ctx->r19 = ADD32(0, 0X8);
    // 0x8016DA58: addiu       $s2, $zero, 0x4
    ctx->r18 = ADD32(0, 0X4);
L_8016DA5C:
    // 0x8016DA5C: lw          $v0, 0x0($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X0);
    // 0x8016DA60: beql        $v0, $zero, L_8016DA94
    if (ctx->r2 == 0) {
        // 0x8016DA64: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_8016DA94;
    }
    goto skip_0;
    // 0x8016DA64: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    skip_0:
    // 0x8016DA68: lw          $t0, 0x18($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X18);
    // 0x8016DA6C: lw          $t1, 0x0($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X0);
    // 0x8016DA70: sw          $t0, 0x168($t1)
    MEM_W(0X168, ctx->r9) = ctx->r8;
    // 0x8016DA74: lw          $t2, 0x0($s1)
    ctx->r10 = MEM_W(ctx->r17, 0X0);
    // 0x8016DA78: lh          $v1, 0x1C($t2)
    ctx->r3 = MEM_H(ctx->r10, 0X1C);
    // 0x8016DA7C: beql        $v1, $zero, L_8016DA94
    if (ctx->r3 == 0) {
        // 0x8016DA80: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_8016DA94;
    }
    goto skip_1;
    // 0x8016DA80: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    skip_1:
    // 0x8016DA84: sw          $v1, 0x0($s4)
    MEM_W(0X0, ctx->r20) = ctx->r3;
    // 0x8016DA88: jal         0x8016DD2C
    // 0x8016DA8C: sw          $s6, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r22;
    func_8016DD2C(rdram, ctx);
        goto after_0;
    // 0x8016DA8C: sw          $s6, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r22;
    after_0:
    // 0x8016DA90: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
L_8016DA94:
    // 0x8016DA94: bne         $s2, $s3, L_8016DA5C
    if (ctx->r18 != ctx->r19) {
        // 0x8016DA98: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_8016DA5C;
    }
    // 0x8016DA98: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x8016DA9C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x8016DAA0: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016DAA4: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8016DAA8: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8016DAAC: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8016DAB0: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8016DAB4: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x8016DAB8: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x8016DABC: jr          $ra
    // 0x8016DAC0: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x8016DAC0: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_8016DAC4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016DAC4 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016DAC4++;
    if (lod_text_guard_depth_8016DAC4 > 128) {
        if (!lod_text_guard_logged_8016DAC4) {
            lod_text_guard_logged_8016DAC4 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016DAC4 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016DAC4--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016DAC4: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x8016DAC8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016DACC: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016DAD0: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x8016DAD4: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x8016DAD8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016DADC: sw          $a0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r4;
    // 0x8016DAE0: sw          $a1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r5;
    // 0x8016DAE4: lw          $a3, 0x19C($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X19C);
    // 0x8016DAE8: addiu       $a1, $v0, 0x1C0
    ctx->r5 = ADD32(ctx->r2, 0X1C0);
    // 0x8016DAEC: addiu       $a0, $v0, 0x5C
    ctx->r4 = ADD32(ctx->r2, 0X5C);
    // 0x8016DAF0: jal         0x8016ECF0
    // 0x8016DAF4: addiu       $a2, $v0, 0x44
    ctx->r6 = ADD32(ctx->r2, 0X44);
    func_8016ECF0(rdram, ctx);
        goto after_0;
    // 0x8016DAF4: addiu       $a2, $v0, 0x44
    ctx->r6 = ADD32(ctx->r2, 0X44);
    after_0:
    // 0x8016DAF8: beq         $v0, $zero, L_8016DD18
    if (ctx->r2 == 0) {
        // 0x8016DAFC: lh          $t6, 0x5A($sp)
        ctx->r14 = MEM_H(ctx->r29, 0X5A);
            goto L_8016DD18;
    }
    // 0x8016DAFC: lh          $t6, 0x5A($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X5A);
    // 0x8016DB00: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x8016DB04: lw          $t8, -0x10F4($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X10F4);
    // 0x8016DB08: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x8016DB0C: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016DB10: addu        $t2, $t7, $t8
    ctx->r10 = ADD32(ctx->r15, ctx->r24);
    // 0x8016DB14: lh          $a2, 0x0($t2)
    ctx->r6 = MEM_H(ctx->r10, 0X0);
    // 0x8016DB18: bnel        $a2, $at, L_8016DC90
    if (ctx->r6 != ctx->r1) {
        // 0x8016DB1C: lh          $t6, 0x2($t2)
        ctx->r14 = MEM_H(ctx->r10, 0X2);
            goto L_8016DC90;
    }
    goto skip_0;
    // 0x8016DB1C: lh          $t6, 0x2($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X2);
    skip_0:
    // 0x8016DB20: lh          $t1, 0x2($t2)
    ctx->r9 = MEM_H(ctx->r10, 0X2);
    // 0x8016DB24: lw          $a2, 0x4($t2)
    ctx->r6 = MEM_W(ctx->r10, 0X4);
    // 0x8016DB28: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x8016DB2C: blez        $t1, L_8016DD18
    if (SIGNED(ctx->r9) <= 0) {
        // 0x8016DB30: lui         $t5, 0x8019
        ctx->r13 = S32(0X8019 << 16);
            goto L_8016DD18;
    }
    // 0x8016DB30: lui         $t5, 0x8019
    ctx->r13 = S32(0X8019 << 16);
    // 0x8016DB34: lui         $t3, 0x801A
    ctx->r11 = S32(0X801A << 16);
    // 0x8016DB38: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016DB3C: addiu       $a3, $a3, -0x1118
    ctx->r7 = ADD32(ctx->r7, -0X1118);
    // 0x8016DB40: addiu       $t3, $t3, -0x978
    ctx->r11 = ADD32(ctx->r11, -0X978);
    // 0x8016DB44: addiu       $t5, $t5, 0x2494
    ctx->r13 = ADD32(ctx->r13, 0X2494);
    // 0x8016DB48: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
L_8016DB4C:
    // 0x8016DB4C: lh          $a0, 0x0($a2)
    ctx->r4 = MEM_H(ctx->r6, 0X0);
    // 0x8016DB50: addiu       $t0, $t0, 0x1
    ctx->r8 = ADD32(ctx->r8, 0X1);
    // 0x8016DB54: addu        $a1, $t3, $a0
    ctx->r5 = ADD32(ctx->r11, ctx->r4);
    // 0x8016DB58: lbu         $t9, 0x0($a1)
    ctx->r25 = MEM_BU(ctx->r5, 0X0);
    // 0x8016DB5C: bnel        $t9, $zero, L_8016DC7C
    if (ctx->r25 != 0) {
        // 0x8016DB60: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_8016DC7C;
    }
    goto skip_1;
    // 0x8016DB60: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_1:
    // 0x8016DB64: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x8016DB68: sll         $t6, $a0, 6
    ctx->r14 = S32(ctx->r4 << 6);
    // 0x8016DB6C: lw          $t7, 0x168($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X168);
    // 0x8016DB70: lwc1        $f4, 0x160($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X160);
    // 0x8016DB74: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x8016DB78: lh          $t8, 0x2C($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X2C);
    // 0x8016DB7C: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x8016DB80: nop

    // 0x8016DB84: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016DB88: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x8016DB8C: nop

    // 0x8016DB90: bc1tl       L_8016DC7C
    if (c1cs) {
        // 0x8016DB94: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_8016DC7C;
    }
    goto skip_2;
    // 0x8016DB94: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_2:
    // 0x8016DB98: lh          $t9, 0x2E($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X2E);
    // 0x8016DB9C: lwc1        $f18, 0x154($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X154);
    // 0x8016DBA0: mtc1        $t9, $f10
    ctx->f10.u32l = ctx->r25;
    // 0x8016DBA4: nop

    // 0x8016DBA8: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016DBAC: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016DBB0: nop

    // 0x8016DBB4: bc1tl       L_8016DC7C
    if (c1cs) {
        // 0x8016DBB8: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_8016DC7C;
    }
    goto skip_3;
    // 0x8016DBB8: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_3:
    // 0x8016DBBC: lh          $t6, 0x30($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X30);
    // 0x8016DBC0: lwc1        $f6, 0x15C($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X15C);
    // 0x8016DBC4: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x8016DBC8: nop

    // 0x8016DBCC: cvt.s.w     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016DBD0: c.lt.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl < ctx->f8.fl;
    // 0x8016DBD4: nop

    // 0x8016DBD8: bc1tl       L_8016DC7C
    if (c1cs) {
        // 0x8016DBDC: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_8016DC7C;
    }
    goto skip_4;
    // 0x8016DBDC: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_4:
    // 0x8016DBE0: lh          $t7, 0x32($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X32);
    // 0x8016DBE4: lwc1        $f18, 0x150($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X150);
    // 0x8016DBE8: mtc1        $t7, $f10
    ctx->f10.u32l = ctx->r15;
    // 0x8016DBEC: nop

    // 0x8016DBF0: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016DBF4: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016DBF8: nop

    // 0x8016DBFC: bc1tl       L_8016DC7C
    if (c1cs) {
        // 0x8016DC00: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_8016DC7C;
    }
    goto skip_5;
    // 0x8016DC00: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_5:
    // 0x8016DC04: lh          $t8, 0x34($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X34);
    // 0x8016DC08: lwc1        $f4, 0x164($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X164);
    // 0x8016DC0C: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x8016DC10: nop

    // 0x8016DC14: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016DC18: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x8016DC1C: nop

    // 0x8016DC20: bc1tl       L_8016DC7C
    if (c1cs) {
        // 0x8016DC24: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_8016DC7C;
    }
    goto skip_6;
    // 0x8016DC24: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_6:
    // 0x8016DC28: lh          $t9, 0x36($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X36);
    // 0x8016DC2C: lwc1        $f18, 0x158($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X158);
    // 0x8016DC30: mtc1        $t9, $f10
    ctx->f10.u32l = ctx->r25;
    // 0x8016DC34: nop

    // 0x8016DC38: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016DC3C: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016DC40: nop

    // 0x8016DC44: bc1tl       L_8016DC7C
    if (c1cs) {
        // 0x8016DC48: slt         $at, $t0, $t1
        ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
            goto L_8016DC7C;
    }
    goto skip_7;
    // 0x8016DC48: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
    skip_7:
    // 0x8016DC4C: lw          $t8, 0x0($a3)
    ctx->r24 = MEM_W(ctx->r7, 0X0);
    // 0x8016DC50: lw          $t7, 0x0($t5)
    ctx->r15 = MEM_W(ctx->r13, 0X0);
    // 0x8016DC54: sb          $t4, 0x0($a1)
    MEM_B(0X0, ctx->r5) = ctx->r12;
    // 0x8016DC58: lh          $t6, 0x0($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X0);
    // 0x8016DC5C: sll         $t9, $t8, 1
    ctx->r25 = S32(ctx->r24 << 1);
    // 0x8016DC60: addu        $t8, $t7, $t9
    ctx->r24 = ADD32(ctx->r15, ctx->r25);
    // 0x8016DC64: sh          $t6, 0x0($t8)
    MEM_H(0X0, ctx->r24) = ctx->r14;
    // 0x8016DC68: lw          $t7, 0x0($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X0);
    // 0x8016DC6C: addiu       $t9, $t7, 0x1
    ctx->r25 = ADD32(ctx->r15, 0X1);
    // 0x8016DC70: sw          $t9, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r25;
    // 0x8016DC74: lh          $t1, 0x2($t2)
    ctx->r9 = MEM_H(ctx->r10, 0X2);
    // 0x8016DC78: slt         $at, $t0, $t1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r9) ? 1 : 0;
L_8016DC7C:
    // 0x8016DC7C: bne         $at, $zero, L_8016DB4C
    if (ctx->r1 != 0) {
        // 0x8016DC80: addiu       $a2, $a2, 0x2
        ctx->r6 = ADD32(ctx->r6, 0X2);
            goto L_8016DB4C;
    }
    // 0x8016DC80: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x8016DC84: b           L_8016DD1C
    // 0x8016DC88: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016DD1C;
    // 0x8016DC88: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8016DC8C: lh          $t6, 0x2($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X2);
L_8016DC90:
    // 0x8016DC90: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
    // 0x8016DC94: sll         $v1, $a2, 2
    ctx->r3 = S32(ctx->r6 << 2);
    // 0x8016DC98: mtc1        $t6, $f6
    ctx->f6.u32l = ctx->r14;
    // 0x8016DC9C: addu        $v0, $t8, $v1
    ctx->r2 = ADD32(ctx->r24, ctx->r3);
    // 0x8016DCA0: lwc1        $f4, 0x5C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X5C);
    // 0x8016DCA4: cvt.s.w     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    ctx->f0.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016DCA8: swc1        $f4, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f4.u32l;
    // 0x8016DCAC: swc1        $f0, 0x5C($v0)
    MEM_W(0X5C, ctx->r2) = ctx->f0.u32l;
    // 0x8016DCB0: lh          $a0, 0x4($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X4);
    // 0x8016DCB4: swc1        $f0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f0.u32l;
    // 0x8016DCB8: sw          $t2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r10;
    // 0x8016DCBC: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x8016DCC0: jal         0x8016DAC4
    // 0x8016DCC4: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    func_8016DAC4(rdram, ctx);
        goto after_1;
    // 0x8016DCC4: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    after_1:
    // 0x8016DCC8: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x8016DCCC: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x8016DCD0: lwc1        $f8, 0x54($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X54);
    // 0x8016DCD4: lw          $t2, 0x38($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X38);
    // 0x8016DCD8: lwc1        $f0, 0x40($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016DCDC: addu        $t9, $t7, $v1
    ctx->r25 = ADD32(ctx->r15, ctx->r3);
    // 0x8016DCE0: swc1        $f8, 0x5C($t9)
    MEM_W(0X5C, ctx->r25) = ctx->f8.u32l;
    // 0x8016DCE4: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016DCE8: addu        $v0, $t6, $v1
    ctx->r2 = ADD32(ctx->r14, ctx->r3);
    // 0x8016DCEC: lwc1        $f10, 0x68($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X68);
    // 0x8016DCF0: swc1        $f10, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f10.u32l;
    // 0x8016DCF4: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x8016DCF8: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    // 0x8016DCFC: jal         0x8016DAC4
    // 0x8016DD00: lh          $a0, 0x6($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X6);
    func_8016DAC4(rdram, ctx);
        goto after_2;
    // 0x8016DD00: lh          $a0, 0x6($t2)
    ctx->r4 = MEM_H(ctx->r10, 0X6);
    after_2:
    // 0x8016DD04: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x8016DD08: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
    // 0x8016DD0C: lwc1        $f16, 0x54($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X54);
    // 0x8016DD10: addu        $t7, $t8, $v1
    ctx->r15 = ADD32(ctx->r24, ctx->r3);
    // 0x8016DD14: swc1        $f16, 0x68($t7)
    MEM_W(0X68, ctx->r15) = ctx->f16.u32l;
L_8016DD18:
    // 0x8016DD18: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016DD1C:
    // 0x8016DD1C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016DD20: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    // 0x8016DD24: jr          $ra
    // 0x8016DD28: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016DAC4--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016DD28: nop

;}
RECOMP_FUNC void func_8016DD2C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016DD2C: addiu       $sp, $sp, -0xC0
    ctx->r29 = ADD32(ctx->r29, -0XC0);
    // 0x8016DD30: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8016DD34: lw          $t6, -0x1118($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X1118);
    // 0x8016DD38: sw          $s5, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r21;
    // 0x8016DD3C: sw          $ra, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r31;
    // 0x8016DD40: sw          $fp, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r30;
    // 0x8016DD44: sw          $s7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r23;
    // 0x8016DD48: sw          $s6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r22;
    // 0x8016DD4C: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x8016DD50: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x8016DD54: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x8016DD58: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x8016DD5C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8016DD60: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x8016DD64: sw          $zero, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = 0;
    // 0x8016DD68: blez        $t6, L_8016DF50
    if (SIGNED(ctx->r14) <= 0) {
        // 0x8016DD6C: or          $s5, $zero, $zero
        ctx->r21 = 0 | 0;
            goto L_8016DF50;
    }
    // 0x8016DD6C: or          $s5, $zero, $zero
    ctx->r21 = 0 | 0;
    // 0x8016DD70: addiu       $s2, $sp, 0x80
    ctx->r18 = ADD32(ctx->r29, 0X80);
    // 0x8016DD74: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016DD78: addiu       $s4, $s4, -0x9FC
    ctx->r20 = ADD32(ctx->r20, -0X9FC);
    // 0x8016DD7C: or          $fp, $s2, $zero
    ctx->r30 = ctx->r18 | 0;
    // 0x8016DD80: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
    // 0x8016DD84: addiu       $s7, $sp, 0x98
    ctx->r23 = ADD32(ctx->r29, 0X98);
L_8016DD88:
    // 0x8016DD88: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x8016DD8C: lw          $t8, 0x2494($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X2494);
    // 0x8016DD90: lw          $t2, 0x0($s4)
    ctx->r10 = MEM_W(ctx->r20, 0X0);
    // 0x8016DD94: or          $a1, $s7, $zero
    ctx->r5 = ctx->r23 | 0;
    // 0x8016DD98: addu        $t9, $t8, $s6
    ctx->r25 = ADD32(ctx->r24, ctx->r22);
    // 0x8016DD9C: lh          $t0, 0x0($t9)
    ctx->r8 = MEM_H(ctx->r25, 0X0);
    // 0x8016DDA0: lw          $t3, 0x168($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X168);
    // 0x8016DDA4: sll         $t1, $t0, 6
    ctx->r9 = S32(ctx->r8 << 6);
    // 0x8016DDA8: addu        $s3, $t1, $t3
    ctx->r19 = ADD32(ctx->r9, ctx->r11);
    // 0x8016DDAC: jal         0x8016DF88
    // 0x8016DDB0: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    func_8016DF88(rdram, ctx);
        goto after_0;
    // 0x8016DDB0: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    after_0:
    // 0x8016DDB4: bne         $v0, $zero, L_8016DF34
    if (ctx->r2 != 0) {
        // 0x8016DDB8: nop
    
            goto L_8016DF34;
    }
    // 0x8016DDB8: nop

    // 0x8016DDBC: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016DDC0: lwc1        $f8, 0x98($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X98);
    // 0x8016DDC4: lwc1        $f4, 0xA0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XA0);
    // 0x8016DDC8: lwc1        $f10, 0x1C0($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X1C0);
    // 0x8016DDCC: lwc1        $f16, 0x9C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x8016DDD0: lwc1        $f6, 0x1C8($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X1C8);
    // 0x8016DDD4: sub.s       $f2, $f8, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x8016DDD8: lwc1        $f18, 0x1C4($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X1C4);
    // 0x8016DDDC: lbu         $a0, 0x2B($s3)
    ctx->r4 = MEM_BU(ctx->r19, 0X2B);
    // 0x8016DDE0: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8016DDE4: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x8016DDE8: sub.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016DDEC: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x8016DDF0: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016DDF4: mul.s       $f10, $f0, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x8016DDF8: jal         0x80166F40
    // 0x8016DDFC: add.s       $f20, $f10, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = ctx->f10.fl + ctx->f8.fl;
    func_80166F40(rdram, ctx);
        goto after_1;
    // 0x8016DDFC: add.s       $f20, $f10, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = ctx->f10.fl + ctx->f8.fl;
    after_1:
    // 0x8016DE00: bne         $v0, $zero, L_8016DE14
    if (ctx->r2 != 0) {
        // 0x8016DE04: or          $a1, $s7, $zero
        ctx->r5 = ctx->r23 | 0;
            goto L_8016DE14;
    }
    // 0x8016DE04: or          $a1, $s7, $zero
    ctx->r5 = ctx->r23 | 0;
    // 0x8016DE08: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016DE0C: b           L_8016DE1C
    // 0x8016DE10: addiu       $s1, $s0, 0x230
    ctx->r17 = ADD32(ctx->r16, 0X230);
        goto L_8016DE1C;
    // 0x8016DE10: addiu       $s1, $s0, 0x230
    ctx->r17 = ADD32(ctx->r16, 0X230);
L_8016DE14:
    // 0x8016DE14: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016DE18: addiu       $s1, $s0, 0x48C
    ctx->r17 = ADD32(ctx->r16, 0X48C);
L_8016DE1C:
    // 0x8016DE1C: lh          $t4, 0x60($s1)
    ctx->r12 = MEM_H(ctx->r17, 0X60);
    // 0x8016DE20: addiu       $a0, $s0, 0x1E4
    ctx->r4 = ADD32(ctx->r16, 0X1E4);
    // 0x8016DE24: or          $a2, $fp, $zero
    ctx->r6 = ctx->r30 | 0;
    // 0x8016DE28: beq         $t4, $zero, L_8016DE44
    if (ctx->r12 == 0) {
        // 0x8016DE2C: addiu       $t5, $zero, 0x1
        ctx->r13 = ADD32(0, 0X1);
            goto L_8016DE44;
    }
    // 0x8016DE2C: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x8016DE30: lwc1        $f16, 0x1C($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X1C);
    // 0x8016DE34: c.le.s      $f16, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f16.fl <= ctx->f20.fl;
    // 0x8016DE38: nop

    // 0x8016DE3C: bc1t        L_8016DF34
    if (c1cs) {
        // 0x8016DE40: nop
    
            goto L_8016DF34;
    }
    // 0x8016DE40: nop

L_8016DE44:
    // 0x8016DE44: jal         0x80173524
    // 0x8016DE48: sw          $t5, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = ctx->r13;
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x8016DE48: sw          $t5, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = ctx->r13;
    after_2:
    // 0x8016DE4C: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016DE50: addiu       $v0, $sp, 0x80
    ctx->r2 = ADD32(ctx->r29, 0X80);
    // 0x8016DE54: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016DE58: addiu       $v1, $sp, 0x74
    ctx->r3 = ADD32(ctx->r29, 0X74);
    // 0x8016DE5C: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8016DE60: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016DE64: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016DE68: addu        $t7, $t6, $a0
    ctx->r15 = ADD32(ctx->r14, ctx->r4);
    // 0x8016DE6C: beq         $v1, $s2, L_8016DEB4
    if (ctx->r3 == ctx->r18) {
        // 0x8016DE70: lh          $t8, 0x0($t7)
        ctx->r24 = MEM_H(ctx->r15, 0X0);
            goto L_8016DEB4;
    }
    // 0x8016DE70: lh          $t8, 0x0($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X0);
L_8016DE74:
    // 0x8016DE74: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016DE78: lwc1        $f16, 0x0($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016DE7C: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x8016DE80: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016DE84: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016DE88: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016DE8C: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016DE90: add.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x8016DE94: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016DE98: lwc1        $f16, 0x10($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X10);
    // 0x8016DE9C: sub.s       $f16, $f18, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = ctx->f18.fl - ctx->f16.fl;
    // 0x8016DEA0: swc1        $f16, -0x8($v1)
    MEM_W(-0X8, ctx->r3) = ctx->f16.u32l;
    // 0x8016DEA4: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016DEA8: addu        $t7, $t6, $a0
    ctx->r15 = ADD32(ctx->r14, ctx->r4);
    // 0x8016DEAC: bne         $v1, $s2, L_8016DE74
    if (ctx->r3 != ctx->r18) {
        // 0x8016DEB0: lh          $t8, 0x0($t7)
        ctx->r24 = MEM_H(ctx->r15, 0X0);
            goto L_8016DE74;
    }
    // 0x8016DEB0: lh          $t8, 0x0($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X0);
L_8016DEB4:
    // 0x8016DEB4: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016DEB8: lwc1        $f16, 0x0($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016DEBC: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x8016DEC0: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016DEC4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016DEC8: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016DECC: add.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x8016DED0: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016DED4: lwc1        $f16, 0x10($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X10);
    // 0x8016DED8: sub.s       $f16, $f18, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = ctx->f18.fl - ctx->f16.fl;
    // 0x8016DEDC: swc1        $f16, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f16.u32l;
    // 0x8016DEE0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x8016DEE4: jal         0x80172E2C
    // 0x8016DEE8: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    func_80172E2C(rdram, ctx);
        goto after_3;
    // 0x8016DEE8: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    after_3:
    // 0x8016DEEC: addiu       $a0, $s1, 0xC
    ctx->r4 = ADD32(ctx->r17, 0XC);
    // 0x8016DEF0: jal         0x80172E2C
    // 0x8016DEF4: addiu       $a1, $sp, 0x74
    ctx->r5 = ADD32(ctx->r29, 0X74);
    func_80172E2C(rdram, ctx);
        goto after_4;
    // 0x8016DEF4: addiu       $a1, $sp, 0x74
    ctx->r5 = ADD32(ctx->r29, 0X74);
    after_4:
    // 0x8016DEF8: swc1        $f20, 0x1C($s1)
    MEM_W(0X1C, ctx->r17) = ctx->f20.u32l;
    // 0x8016DEFC: sw          $s3, 0x44($s1)
    MEM_W(0X44, ctx->r17) = ctx->r19;
    // 0x8016DF00: lw          $t9, 0x0($s4)
    ctx->r25 = MEM_W(ctx->r20, 0X0);
    // 0x8016DF04: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x8016DF08: addiu       $a1, $s1, 0x48
    ctx->r5 = ADD32(ctx->r17, 0X48);
    // 0x8016DF0C: lw          $t0, 0x8($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X8);
    // 0x8016DF10: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x8016DF14: sw          $t0, 0x58($s1)
    MEM_W(0X58, ctx->r17) = ctx->r8;
    // 0x8016DF18: lw          $t2, 0x0($s4)
    ctx->r10 = MEM_W(ctx->r20, 0X0);
    // 0x8016DF1C: lw          $t1, 0x0($t2)
    ctx->r9 = MEM_W(ctx->r10, 0X0);
    // 0x8016DF20: lh          $t3, 0x8($t1)
    ctx->r11 = MEM_H(ctx->r9, 0X8);
    // 0x8016DF24: jal         0x80001090
    // 0x8016DF28: sh          $t3, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r11;
    memory_copy(rdram, ctx);
        goto after_5;
    // 0x8016DF28: sh          $t3, 0x5C($s1)
    MEM_H(0X5C, ctx->r17) = ctx->r11;
    after_5:
    // 0x8016DF2C: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x8016DF30: sh          $t4, 0x60($s1)
    MEM_H(0X60, ctx->r17) = ctx->r12;
L_8016DF34:
    // 0x8016DF34: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8016DF38: lw          $t5, -0x1118($t5)
    ctx->r13 = MEM_W(ctx->r13, -0X1118);
    // 0x8016DF3C: addiu       $s5, $s5, 0x1
    ctx->r21 = ADD32(ctx->r21, 0X1);
    // 0x8016DF40: addiu       $s6, $s6, 0x2
    ctx->r22 = ADD32(ctx->r22, 0X2);
    // 0x8016DF44: slt         $at, $s5, $t5
    ctx->r1 = SIGNED(ctx->r21) < SIGNED(ctx->r13) ? 1 : 0;
    // 0x8016DF48: bne         $at, $zero, L_8016DD88
    if (ctx->r1 != 0) {
        // 0x8016DF4C: nop
    
            goto L_8016DD88;
    }
    // 0x8016DF4C: nop

L_8016DF50:
    // 0x8016DF50: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    // 0x8016DF54: lw          $v0, 0xA8($sp)
    ctx->r2 = MEM_W(ctx->r29, 0XA8);
    // 0x8016DF58: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x8016DF5C: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8016DF60: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x8016DF64: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x8016DF68: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x8016DF6C: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x8016DF70: lw          $s5, 0x34($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X34);
    // 0x8016DF74: lw          $s6, 0x38($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X38);
    // 0x8016DF78: lw          $s7, 0x3C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X3C);
    // 0x8016DF7C: lw          $fp, 0x40($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X40);
    // 0x8016DF80: jr          $ra
    // 0x8016DF84: addiu       $sp, $sp, 0xC0
    ctx->r29 = ADD32(ctx->r29, 0XC0);
    return;
    // 0x8016DF84: addiu       $sp, $sp, 0xC0
    ctx->r29 = ADD32(ctx->r29, 0XC0);
;}
RECOMP_FUNC void func_8016DF88(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016DF88: lbu         $a3, 0x38($a0)
    ctx->r7 = MEM_BU(ctx->r4, 0X38);
    // 0x8016DF8C: lbu         $t0, 0x39($a0)
    ctx->r8 = MEM_BU(ctx->r4, 0X39);
    // 0x8016DF90: addiu       $sp, $sp, -0x88
    ctx->r29 = ADD32(ctx->r29, -0X88);
    // 0x8016DF94: sll         $t6, $a3, 2
    ctx->r14 = S32(ctx->r7 << 2);
    // 0x8016DF98: sll         $t7, $t0, 2
    ctx->r15 = S32(ctx->r8 << 2);
    // 0x8016DF9C: subu        $t6, $t6, $a3
    ctx->r14 = SUB32(ctx->r14, ctx->r7);
    // 0x8016DFA0: subu        $t7, $t7, $t0
    ctx->r15 = SUB32(ctx->r15, ctx->r8);
    // 0x8016DFA4: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x8016DFA8: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x8016DFAC: addu        $t2, $a0, $t6
    ctx->r10 = ADD32(ctx->r4, ctx->r14);
    // 0x8016DFB0: addu        $t5, $a0, $t7
    ctx->r13 = ADD32(ctx->r4, ctx->r15);
    // 0x8016DFB4: addiu       $t3, $sp, 0x34
    ctx->r11 = ADD32(ctx->r29, 0X34);
    // 0x8016DFB8: addiu       $t4, $sp, 0x28
    ctx->r12 = ADD32(ctx->r29, 0X28);
    // 0x8016DFBC: addiu       $v0, $sp, 0x34
    ctx->r2 = ADD32(ctx->r29, 0X34);
    // 0x8016DFC0: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016DFC4: sltu        $at, $t4, $v0
    ctx->r1 = ctx->r12 < ctx->r2 ? 1 : 0;
    // 0x8016DFC8: beq         $at, $zero, L_8016E010
    if (ctx->r1 == 0) {
        // 0x8016DFCC: lh          $t8, 0x10($t2)
        ctx->r24 = MEM_H(ctx->r10, 0X10);
            goto L_8016E010;
    }
    // 0x8016DFCC: lh          $t8, 0x10($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X10);
L_8016DFD0:
    // 0x8016DFD0: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016DFD4: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016DFD8: sltu        $at, $t4, $v0
    ctx->r1 = ctx->r12 < ctx->r2 ? 1 : 0;
    // 0x8016DFDC: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016DFE0: addiu       $t2, $t2, 0x2
    ctx->r10 = ADD32(ctx->r10, 0X2);
    // 0x8016DFE4: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016DFE8: addiu       $t5, $t5, 0x2
    ctx->r13 = ADD32(ctx->r13, 0X2);
    // 0x8016DFEC: swc1        $f18, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f18.u32l;
    // 0x8016DFF0: lh          $t9, 0xE($t5)
    ctx->r25 = MEM_H(ctx->r13, 0XE);
    // 0x8016DFF4: mtc1        $t9, $f16
    ctx->f16.u32l = ctx->r25;
    // 0x8016DFF8: nop

    // 0x8016DFFC: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016E000: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016E004: swc1        $f18, -0x8($t4)
    MEM_W(-0X8, ctx->r12) = ctx->f18.u32l;
    // 0x8016E008: bne         $at, $zero, L_8016DFD0
    if (ctx->r1 != 0) {
        // 0x8016E00C: lh          $t8, 0x10($t2)
        ctx->r24 = MEM_H(ctx->r10, 0X10);
            goto L_8016DFD0;
    }
    // 0x8016E00C: lh          $t8, 0x10($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X10);
L_8016E010:
    // 0x8016E010: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016E014: addiu       $t2, $t2, 0x2
    ctx->r10 = ADD32(ctx->r10, 0X2);
    // 0x8016E018: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016E01C: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016E020: addiu       $t5, $t5, 0x2
    ctx->r13 = ADD32(ctx->r13, 0X2);
    // 0x8016E024: swc1        $f18, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f18.u32l;
    // 0x8016E028: lh          $t9, 0xE($t5)
    ctx->r25 = MEM_H(ctx->r13, 0XE);
    // 0x8016E02C: mtc1        $t9, $f16
    ctx->f16.u32l = ctx->r25;
    // 0x8016E030: nop

    // 0x8016E034: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016E038: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016E03C: swc1        $f18, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f18.u32l;
    // 0x8016E040: lui         $a2, 0x801A
    ctx->r6 = S32(0X801A << 16);
    // 0x8016E044: addiu       $a2, $a2, -0x9FC
    ctx->r6 = ADD32(ctx->r6, -0X9FC);
    // 0x8016E048: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x8016E04C: lwc1        $f8, 0x30($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016E050: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016E054: lwc1        $f4, 0x44($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X44);
    // 0x8016E058: lwc1        $f18, 0x4C($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X4C);
    // 0x8016E05C: addiu       $t4, $sp, 0x28
    ctx->r12 = ADD32(ctx->r29, 0X28);
    // 0x8016E060: swc1        $f4, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f4.u32l;
    // 0x8016E064: lwc1        $f6, 0x18($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X18);
    // 0x8016E068: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E06C: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8016E070: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8016E074: mul.s       $f6, $f4, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x8016E078: sub.s       $f12, $f6, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016E07C: lwc1        $f6, 0x34($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016E080: c.lt.s      $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f12.fl < ctx->f8.fl;
    // 0x8016E084: lwc1        $f8, 0x3C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016E088: bc1fl       L_8016E09C
    if (!c1cs) {
        // 0x8016E08C: mov.s       $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
            goto L_8016E09C;
    }
    goto skip_0;
    // 0x8016E08C: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    skip_0:
    // 0x8016E090: b           L_8016E09C
    // 0x8016E094: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
        goto L_8016E09C;
    // 0x8016E094: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
    // 0x8016E098: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_8016E09C:
    // 0x8016E09C: lwc1        $f4, -0x5124($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X5124);
    // 0x8016E0A0: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x8016E0A4: nop

    // 0x8016E0A8: bc1fl       L_8016E0BC
    if (!c1cs) {
        // 0x8016E0AC: lwc1        $f10, 0x1C0($v1)
        ctx->f10.u32l = MEM_W(ctx->r3, 0X1C0);
            goto L_8016E0BC;
    }
    goto skip_1;
    // 0x8016E0AC: lwc1        $f10, 0x1C0($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X1C0);
    skip_1:
    // 0x8016E0B0: b           L_8016E464
    // 0x8016E0B4: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
        goto L_8016E464;
    // 0x8016E0B4: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
    // 0x8016E0B8: lwc1        $f10, 0x1C0($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X1C0);
L_8016E0BC:
    // 0x8016E0BC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016E0C0: lwc1        $f4, 0x1C8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X1C8);
    // 0x8016E0C4: sub.s       $f14, $f6, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016E0C8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8016E0CC: lwc1        $f10, 0x28($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E0D0: sub.s       $f0, $f8, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x8016E0D4: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016E0D8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016E0DC: div.s       $f12, $f6, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = DIV_S(ctx->f6.fl, ctx->f12.fl);
    // 0x8016E0E0: mul.s       $f8, $f0, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x8016E0E4: nop

    // 0x8016E0E8: mul.s       $f6, $f14, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f4.fl);
    // 0x8016E0EC: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8016E0F0: sub.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x8016E0F4: mul.s       $f2, $f12, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = MUL_S(ctx->f12.fl, ctx->f10.fl);
    // 0x8016E0F8: c.lt.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl < ctx->f4.fl;
    // 0x8016E0FC: mov.s       $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    ctx->f16.fl = ctx->f2.fl;
    // 0x8016E100: bc1t        L_8016E120
    if (c1cs) {
        // 0x8016E104: nop
    
            goto L_8016E120;
    }
    // 0x8016E104: nop

    // 0x8016E108: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016E10C: lwc1        $f6, 0x18($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X18);
    // 0x8016E110: c.lt.s      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.fl < ctx->f2.fl;
    // 0x8016E114: nop

    // 0x8016E118: bc1f        L_8016E128
    if (!c1cs) {
        // 0x8016E11C: nop
    
            goto L_8016E128;
    }
    // 0x8016E11C: nop

L_8016E120:
    // 0x8016E120: b           L_8016E464
    // 0x8016E124: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
        goto L_8016E464;
    // 0x8016E124: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
L_8016E128:
    // 0x8016E128: mul.s       $f10, $f0, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x8016E12C: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8016E130: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016E134: mul.s       $f4, $f18, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8016E138: sub.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x8016E13C: mul.s       $f2, $f8, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = MUL_S(ctx->f8.fl, ctx->f12.fl);
    // 0x8016E140: c.lt.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl < ctx->f6.fl;
    // 0x8016E144: nop

    // 0x8016E148: bc1t        L_8016E168
    if (c1cs) {
        // 0x8016E14C: nop
    
            goto L_8016E168;
    }
    // 0x8016E14C: nop

    // 0x8016E150: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016E154: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E158: c.lt.s      $f10, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f10.fl < ctx->f2.fl;
    // 0x8016E15C: nop

    // 0x8016E160: bc1f        L_8016E170
    if (!c1cs) {
        // 0x8016E164: nop
    
            goto L_8016E170;
    }
    // 0x8016E164: nop

L_8016E168:
    // 0x8016E168: b           L_8016E464
    // 0x8016E16C: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
        goto L_8016E464;
    // 0x8016E16C: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
L_8016E170:
    // 0x8016E170: mul.s       $f8, $f4, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016E174: lwc1        $f6, 0x34($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016E178: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016E17C: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
    // 0x8016E180: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x8016E184: swc1        $f10, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f10.u32l;
    // 0x8016E188: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016E18C: lwc1        $f6, 0x3C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016E190: mul.s       $f8, $f4, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016E194: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x8016E198: swc1        $f10, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f10.u32l;
    // 0x8016E19C: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x8016E1A0: lwc1        $f4, 0x48($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X48);
    // 0x8016E1A4: lwc1        $f6, 0x1C4($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X1C4);
    // 0x8016E1A8: lwc1        $f10, 0x19C($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X19C);
    // 0x8016E1AC: mul.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8016E1B0: add.s       $f18, $f8, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f18.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x8016E1B4: lwc1        $f6, -0x5120($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X5120);
    // 0x8016E1B8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016E1BC: add.s       $f4, $f18, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f10.fl;
    // 0x8016E1C0: swc1        $f4, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f4.u32l;
    // 0x8016E1C4: lwc1        $f8, 0x68($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X68);
    // 0x8016E1C8: lwc1        $f4, -0x511C($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X511C);
    // 0x8016E1CC: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
    // 0x8016E1D0: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x8016E1D4: lwc1        $f8, 0x2C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016E1D8: sub.s       $f18, $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f18.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x8016E1DC: mul.s       $f6, $f8, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016E1E0: swc1        $f10, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f10.u32l;
    // 0x8016E1E4: lwc1        $f10, 0x38($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016E1E8: add.s       $f14, $f6, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016E1EC: swc1        $f14, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f14.u32l;
    // 0x8016E1F0: lbu         $t1, 0x3A($a0)
    ctx->r9 = MEM_BU(ctx->r4, 0X3A);
    // 0x8016E1F4: lbu         $t2, 0x3B($a0)
    ctx->r10 = MEM_BU(ctx->r4, 0X3B);
    // 0x8016E1F8: beql        $t1, $at, L_8016E2E0
    if (ctx->r9 == ctx->r1) {
        // 0x8016E1FC: mtc1        $zero, $f6
        ctx->f6.u32l = 0;
            goto L_8016E2E0;
    }
    goto skip_2;
    // 0x8016E1FC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    skip_2:
    // 0x8016E200: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E204: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8016E208: mflo        $t6
    ctx->r14 = lo;
    // 0x8016E20C: addu        $t3, $a0, $t6
    ctx->r11 = ADD32(ctx->r4, ctx->r14);
    // 0x8016E210: lh          $t8, 0x10($t3)
    ctx->r24 = MEM_H(ctx->r11, 0X10);
    // 0x8016E214: multu       $t1, $v0
    result = U64(U32(ctx->r9)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E218: mflo        $t7
    ctx->r15 = lo;
    // 0x8016E21C: addu        $a3, $a0, $t7
    ctx->r7 = ADD32(ctx->r4, ctx->r15);
    // 0x8016E220: lh          $t0, 0x10($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X10);
    // 0x8016E224: subu        $t9, $t8, $t0
    ctx->r25 = SUB32(ctx->r24, ctx->r8);
    // 0x8016E228: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x8016E22C: nop

    // 0x8016E230: cvt.s.w     $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    ctx->f2.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E234: c.eq.s      $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f2.fl == ctx->f8.fl;
    // 0x8016E238: nop

    // 0x8016E23C: bc1tl       L_8016E28C
    if (c1cs) {
        // 0x8016E240: lh          $t0, 0x14($a3)
        ctx->r8 = MEM_H(ctx->r7, 0X14);
            goto L_8016E28C;
    }
    goto skip_3;
    // 0x8016E240: lh          $t0, 0x14($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X14);
    skip_3:
    // 0x8016E244: lh          $t1, 0x12($a3)
    ctx->r9 = MEM_H(ctx->r7, 0X12);
    // 0x8016E248: lh          $t6, 0x12($t3)
    ctx->r14 = MEM_H(ctx->r11, 0X12);
    // 0x8016E24C: mtc1        $t0, $f8
    ctx->f8.u32l = ctx->r8;
    // 0x8016E250: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016E254: subu        $t7, $t6, $t1
    ctx->r15 = SUB32(ctx->r14, ctx->r9);
    // 0x8016E258: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x8016E25C: nop

    // 0x8016E260: cvt.s.w     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016E264: cvt.s.w     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E268: div.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8016E26C: sub.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8016E270: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x8016E274: nop

    // 0x8016E278: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E27C: mul.s       $f8, $f0, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x8016E280: b           L_8016E414
    // 0x8016E284: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
        goto L_8016E414;
    // 0x8016E284: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8016E288: lh          $t0, 0x14($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X14);
L_8016E28C:
    // 0x8016E28C: lh          $t8, 0x14($t3)
    ctx->r24 = MEM_H(ctx->r11, 0X14);
    // 0x8016E290: lh          $t1, 0x12($a3)
    ctx->r9 = MEM_H(ctx->r7, 0X12);
    // 0x8016E294: lh          $t6, 0x12($t3)
    ctx->r14 = MEM_H(ctx->r11, 0X12);
    // 0x8016E298: subu        $t9, $t8, $t0
    ctx->r25 = SUB32(ctx->r24, ctx->r8);
    // 0x8016E29C: mtc1        $t9, $f10
    ctx->f10.u32l = ctx->r25;
    // 0x8016E2A0: subu        $t7, $t6, $t1
    ctx->r15 = SUB32(ctx->r14, ctx->r9);
    // 0x8016E2A4: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x8016E2A8: cvt.s.w     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    ctx->f2.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E2AC: mtc1        $t0, $f10
    ctx->f10.u32l = ctx->r8;
    // 0x8016E2B0: lwc1        $f8, 0x8($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016E2B4: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E2B8: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E2BC: div.s       $f0, $f6, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016E2C0: sub.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x8016E2C4: mtc1        $t1, $f8
    ctx->f8.u32l = ctx->r9;
    // 0x8016E2C8: nop

    // 0x8016E2CC: cvt.s.w     $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E2D0: mul.s       $f10, $f0, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x8016E2D4: b           L_8016E414
    // 0x8016E2D8: add.s       $f12, $f4, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f10.fl;
        goto L_8016E414;
    // 0x8016E2D8: add.s       $f12, $f4, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x8016E2DC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
L_8016E2E0:
    // 0x8016E2E0: lwc1        $f8, 0x28($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E2E4: swc1        $f14, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f14.u32l;
    // 0x8016E2E8: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
    // 0x8016E2EC: c.eq.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl == ctx->f8.fl;
    // 0x8016E2F0: nop

    // 0x8016E2F4: bc1f        L_8016E38C
    if (!c1cs) {
        // 0x8016E2F8: nop
    
            goto L_8016E38C;
    }
    // 0x8016E2F8: nop

    // 0x8016E2FC: addiu       $v0, $zero, 0x6
    ctx->r2 = ADD32(0, 0X6);
    // 0x8016E300: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E304: lwc1        $f14, 0x8($a1)
    ctx->f14.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016E308: or          $t2, $t0, $zero
    ctx->r10 = ctx->r8 | 0;
    // 0x8016E30C: mflo        $t8
    ctx->r24 = lo;
    // 0x8016E310: addu        $t3, $a0, $t8
    ctx->r11 = ADD32(ctx->r4, ctx->r24);
    // 0x8016E314: lh          $t1, 0x14($t3)
    ctx->r9 = MEM_H(ctx->r11, 0X14);
    // 0x8016E318: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x8016E31C: nop

    // 0x8016E320: cvt.s.w     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    ctx->f16.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E324: c.lt.s      $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f14.fl < ctx->f16.fl;
    // 0x8016E328: nop

    // 0x8016E32C: bc1f        L_8016E33C
    if (!c1cs) {
        // 0x8016E330: nop
    
            goto L_8016E33C;
    }
    // 0x8016E330: nop

    // 0x8016E334: b           L_8016E33C
    // 0x8016E338: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
        goto L_8016E33C;
    // 0x8016E338: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
L_8016E33C:
    // 0x8016E33C: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E340: lh          $a3, 0x12($t3)
    ctx->r7 = MEM_H(ctx->r11, 0X12);
    // 0x8016E344: mtc1        $a3, $f4
    ctx->f4.u32l = ctx->r7;
    // 0x8016E348: mflo        $t9
    ctx->r25 = lo;
    // 0x8016E34C: addu        $t0, $a0, $t9
    ctx->r8 = ADD32(ctx->r4, ctx->r25);
    // 0x8016E350: lh          $t6, 0x14($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X14);
    // 0x8016E354: lh          $t8, 0x12($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X12);
    // 0x8016E358: subu        $t7, $t6, $t1
    ctx->r15 = SUB32(ctx->r14, ctx->r9);
    // 0x8016E35C: subu        $t9, $t8, $a3
    ctx->r25 = SUB32(ctx->r24, ctx->r7);
    // 0x8016E360: mtc1        $t7, $f10
    ctx->f10.u32l = ctx->r15;
    // 0x8016E364: mtc1        $t9, $f6
    ctx->f6.u32l = ctx->r25;
    // 0x8016E368: cvt.s.w     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    ctx->f2.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E36C: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016E370: sub.s       $f6, $f14, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016E374: lwc1        $f14, 0x1C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x8016E378: div.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016E37C: cvt.s.w     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E380: mul.s       $f8, $f0, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x8016E384: b           L_8016E414
    // 0x8016E388: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
        goto L_8016E414;
    // 0x8016E388: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
L_8016E38C:
    // 0x8016E38C: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E390: lwc1        $f14, 0x0($a1)
    ctx->f14.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016E394: or          $t2, $t0, $zero
    ctx->r10 = ctx->r8 | 0;
    // 0x8016E398: mflo        $t6
    ctx->r14 = lo;
    // 0x8016E39C: addu        $t3, $a0, $t6
    ctx->r11 = ADD32(ctx->r4, ctx->r14);
    // 0x8016E3A0: lh          $t1, 0x10($t3)
    ctx->r9 = MEM_H(ctx->r11, 0X10);
    // 0x8016E3A4: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x8016E3A8: nop

    // 0x8016E3AC: cvt.s.w     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    ctx->f16.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E3B0: c.lt.s      $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f14.fl < ctx->f16.fl;
    // 0x8016E3B4: nop

    // 0x8016E3B8: bc1f        L_8016E3C8
    if (!c1cs) {
        // 0x8016E3BC: nop
    
            goto L_8016E3C8;
    }
    // 0x8016E3BC: nop

    // 0x8016E3C0: b           L_8016E3C8
    // 0x8016E3C4: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
        goto L_8016E3C8;
    // 0x8016E3C4: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
L_8016E3C8:
    // 0x8016E3C8: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E3CC: lh          $a3, 0x12($t3)
    ctx->r7 = MEM_H(ctx->r11, 0X12);
    // 0x8016E3D0: mtc1        $a3, $f4
    ctx->f4.u32l = ctx->r7;
    // 0x8016E3D4: mflo        $t7
    ctx->r15 = lo;
    // 0x8016E3D8: addu        $t0, $a0, $t7
    ctx->r8 = ADD32(ctx->r4, ctx->r15);
    // 0x8016E3DC: lh          $t8, 0x10($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X10);
    // 0x8016E3E0: lh          $t6, 0x12($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X12);
    // 0x8016E3E4: subu        $t9, $t8, $t1
    ctx->r25 = SUB32(ctx->r24, ctx->r9);
    // 0x8016E3E8: subu        $t7, $t6, $a3
    ctx->r15 = SUB32(ctx->r14, ctx->r7);
    // 0x8016E3EC: mtc1        $t9, $f6
    ctx->f6.u32l = ctx->r25;
    // 0x8016E3F0: mtc1        $t7, $f10
    ctx->f10.u32l = ctx->r15;
    // 0x8016E3F4: cvt.s.w     $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    ctx->f2.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016E3F8: cvt.s.w     $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E3FC: sub.s       $f10, $f14, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016E400: lwc1        $f14, 0x1C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x8016E404: div.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016E408: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E40C: mul.s       $f8, $f0, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x8016E410: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
L_8016E414:
    // 0x8016E414: c.lt.s      $f12, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f12.fl < ctx->f14.fl;
    // 0x8016E418: nop

    // 0x8016E41C: bc1fl       L_8016E430
    if (!c1cs) {
        // 0x8016E420: c.lt.s      $f12, $f18
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f12.fl < ctx->f18.fl;
            goto L_8016E430;
    }
    goto skip_4;
    // 0x8016E420: c.lt.s      $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f12.fl < ctx->f18.fl;
    skip_4:
    // 0x8016E424: swc1        $f12, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f12.u32l;
    // 0x8016E428: mov.s       $f12, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    ctx->f12.fl = ctx->f14.fl;
    // 0x8016E42C: c.lt.s      $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f12.fl < ctx->f18.fl;
L_8016E430:
    // 0x8016E430: lwc1        $f4, 0x68($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X68);
    // 0x8016E434: lwc1        $f10, 0x60($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X60);
    // 0x8016E438: bc1tl       L_8016E468
    if (c1cs) {
        // 0x8016E43C: lh          $t8, 0x28($a0)
        ctx->r24 = MEM_H(ctx->r4, 0X28);
            goto L_8016E468;
    }
    goto skip_5;
    // 0x8016E43C: lh          $t8, 0x28($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X28);
    skip_5:
    // 0x8016E440: c.lt.s      $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f4.fl < ctx->f10.fl;
    // 0x8016E444: nop

    // 0x8016E448: bc1tl       L_8016E468
    if (c1cs) {
        // 0x8016E44C: lh          $t8, 0x28($a0)
        ctx->r24 = MEM_H(ctx->r4, 0X28);
            goto L_8016E468;
    }
    goto skip_6;
    // 0x8016E44C: lh          $t8, 0x28($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X28);
    skip_6:
    // 0x8016E450: lwc1        $f6, 0x1A4($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X1A4);
    // 0x8016E454: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016E458: sub.s       $f8, $f18, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f18.fl - ctx->f6.fl;
    // 0x8016E45C: b           L_8016E928
    // 0x8016E460: swc1        $f8, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f8.u32l;
        goto L_8016E928;
    // 0x8016E460: swc1        $f8, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f8.u32l;
L_8016E464:
    // 0x8016E464: lh          $t8, 0x28($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X28);
L_8016E468:
    // 0x8016E468: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x8016E46C: addiu       $t3, $sp, 0x34
    ctx->r11 = ADD32(ctx->r29, 0X34);
    // 0x8016E470: bnel        $t8, $at, L_8016E484
    if (ctx->r24 != ctx->r1) {
        // 0x8016E474: lbu         $a3, 0x3C($a0)
        ctx->r7 = MEM_BU(ctx->r4, 0X3C);
            goto L_8016E484;
    }
    goto skip_7;
    // 0x8016E474: lbu         $a3, 0x3C($a0)
    ctx->r7 = MEM_BU(ctx->r4, 0X3C);
    skip_7:
    // 0x8016E478: b           L_8016E928
    // 0x8016E47C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016E928;
    // 0x8016E47C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016E480: lbu         $a3, 0x3C($a0)
    ctx->r7 = MEM_BU(ctx->r4, 0X3C);
L_8016E484:
    // 0x8016E484: lbu         $t0, 0x3D($a0)
    ctx->r8 = MEM_BU(ctx->r4, 0X3D);
    // 0x8016E488: addiu       $t1, $sp, 0x34
    ctx->r9 = ADD32(ctx->r29, 0X34);
    // 0x8016E48C: sll         $t9, $a3, 2
    ctx->r25 = S32(ctx->r7 << 2);
    // 0x8016E490: sll         $t6, $t0, 2
    ctx->r14 = S32(ctx->r8 << 2);
    // 0x8016E494: subu        $t9, $t9, $a3
    ctx->r25 = SUB32(ctx->r25, ctx->r7);
    // 0x8016E498: subu        $t6, $t6, $t0
    ctx->r14 = SUB32(ctx->r14, ctx->r8);
    // 0x8016E49C: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x8016E4A0: sll         $t9, $t9, 1
    ctx->r25 = S32(ctx->r25 << 1);
    // 0x8016E4A4: addu        $t2, $a0, $t9
    ctx->r10 = ADD32(ctx->r4, ctx->r25);
    // 0x8016E4A8: addu        $t5, $a0, $t6
    ctx->r13 = ADD32(ctx->r4, ctx->r14);
    // 0x8016E4AC: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016E4B0: beq         $t4, $t1, L_8016E4F4
    if (ctx->r12 == ctx->r9) {
        // 0x8016E4B4: lh          $t7, 0x10($t2)
        ctx->r15 = MEM_H(ctx->r10, 0X10);
            goto L_8016E4F4;
    }
    // 0x8016E4B4: lh          $t7, 0x10($t2)
    ctx->r15 = MEM_H(ctx->r10, 0X10);
L_8016E4B8:
    // 0x8016E4B8: mtc1        $t7, $f18
    ctx->f18.u32l = ctx->r15;
    // 0x8016E4BC: addiu       $t4, $t4, 0x4
    ctx->r12 = ADD32(ctx->r12, 0X4);
    // 0x8016E4C0: addiu       $t2, $t2, 0x2
    ctx->r10 = ADD32(ctx->r10, 0X2);
    // 0x8016E4C4: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016E4C8: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016E4CC: addiu       $t5, $t5, 0x2
    ctx->r13 = ADD32(ctx->r13, 0X2);
    // 0x8016E4D0: swc1        $f18, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f18.u32l;
    // 0x8016E4D4: lh          $t8, 0xE($t5)
    ctx->r24 = MEM_H(ctx->r13, 0XE);
    // 0x8016E4D8: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016E4DC: nop

    // 0x8016E4E0: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016E4E4: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016E4E8: swc1        $f18, -0x8($t4)
    MEM_W(-0X8, ctx->r12) = ctx->f18.u32l;
    // 0x8016E4EC: bne         $t4, $t1, L_8016E4B8
    if (ctx->r12 != ctx->r9) {
        // 0x8016E4F0: lh          $t7, 0x10($t2)
        ctx->r15 = MEM_H(ctx->r10, 0X10);
            goto L_8016E4B8;
    }
    // 0x8016E4F0: lh          $t7, 0x10($t2)
    ctx->r15 = MEM_H(ctx->r10, 0X10);
L_8016E4F4:
    // 0x8016E4F4: mtc1        $t7, $f18
    ctx->f18.u32l = ctx->r15;
    // 0x8016E4F8: addiu       $t2, $t2, 0x2
    ctx->r10 = ADD32(ctx->r10, 0X2);
    // 0x8016E4FC: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016E500: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016E504: addiu       $t5, $t5, 0x2
    ctx->r13 = ADD32(ctx->r13, 0X2);
    // 0x8016E508: swc1        $f18, -0x4($t3)
    MEM_W(-0X4, ctx->r11) = ctx->f18.u32l;
    // 0x8016E50C: lh          $t8, 0xE($t5)
    ctx->r24 = MEM_H(ctx->r13, 0XE);
    // 0x8016E510: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x8016E514: nop

    // 0x8016E518: cvt.s.w     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8016E51C: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016E520: swc1        $f18, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->f18.u32l;
    // 0x8016E524: lwc1        $f4, 0x44($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X44);
    // 0x8016E528: lwc1        $f18, 0x4C($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X4C);
    // 0x8016E52C: lwc1        $f6, 0x30($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016E530: swc1        $f4, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f4.u32l;
    // 0x8016E534: lwc1        $f10, 0x18($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X18);
    // 0x8016E538: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E53C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016E540: mul.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x8016E544: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8016E548: mul.s       $f10, $f4, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x8016E54C: sub.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x8016E550: lwc1        $f10, 0x34($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016E554: c.lt.s      $f12, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f12.fl < ctx->f6.fl;
    // 0x8016E558: lwc1        $f6, 0x3C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016E55C: bc1fl       L_8016E570
    if (!c1cs) {
        // 0x8016E560: mov.s       $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
            goto L_8016E570;
    }
    goto skip_8;
    // 0x8016E560: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    skip_8:
    // 0x8016E564: b           L_8016E570
    // 0x8016E568: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
        goto L_8016E570;
    // 0x8016E568: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
    // 0x8016E56C: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_8016E570:
    // 0x8016E570: lwc1        $f4, -0x5118($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X5118);
    // 0x8016E574: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x8016E578: nop

    // 0x8016E57C: bc1fl       L_8016E590
    if (!c1cs) {
        // 0x8016E580: lwc1        $f8, 0x1C0($v1)
        ctx->f8.u32l = MEM_W(ctx->r3, 0X1C0);
            goto L_8016E590;
    }
    goto skip_9;
    // 0x8016E580: lwc1        $f8, 0x1C0($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X1C0);
    skip_9:
    // 0x8016E584: b           L_8016E928
    // 0x8016E588: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
        goto L_8016E928;
    // 0x8016E588: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x8016E58C: lwc1        $f8, 0x1C0($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X1C0);
L_8016E590:
    // 0x8016E590: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016E594: lwc1        $f4, 0x1C8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X1C8);
    // 0x8016E598: sub.s       $f14, $f10, $f8
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f14.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x8016E59C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8016E5A0: lwc1        $f8, 0x28($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E5A4: sub.s       $f0, $f6, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f4.fl;
    // 0x8016E5A8: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016E5AC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016E5B0: div.s       $f12, $f10, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = DIV_S(ctx->f10.fl, ctx->f12.fl);
    // 0x8016E5B4: mul.s       $f6, $f0, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f8.fl);
    // 0x8016E5B8: nop

    // 0x8016E5BC: mul.s       $f10, $f14, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f4.fl);
    // 0x8016E5C0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8016E5C4: sub.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8016E5C8: mul.s       $f2, $f12, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = MUL_S(ctx->f12.fl, ctx->f8.fl);
    // 0x8016E5CC: c.lt.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl < ctx->f4.fl;
    // 0x8016E5D0: mov.s       $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    ctx->f16.fl = ctx->f2.fl;
    // 0x8016E5D4: bc1t        L_8016E5F4
    if (c1cs) {
        // 0x8016E5D8: nop
    
            goto L_8016E5F4;
    }
    // 0x8016E5D8: nop

    // 0x8016E5DC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8016E5E0: lwc1        $f10, 0x18($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X18);
    // 0x8016E5E4: c.lt.s      $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f6.fl < ctx->f2.fl;
    // 0x8016E5E8: nop

    // 0x8016E5EC: bc1f        L_8016E5FC
    if (!c1cs) {
        // 0x8016E5F0: nop
    
            goto L_8016E5FC;
    }
    // 0x8016E5F0: nop

L_8016E5F4:
    // 0x8016E5F4: b           L_8016E928
    // 0x8016E5F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016E928;
    // 0x8016E5F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8016E5FC:
    // 0x8016E5FC: mul.s       $f8, $f0, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x8016E600: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8016E604: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016E608: mul.s       $f4, $f18, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x8016E60C: sub.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x8016E610: mul.s       $f2, $f6, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = MUL_S(ctx->f6.fl, ctx->f12.fl);
    // 0x8016E614: c.lt.s      $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f2.fl < ctx->f10.fl;
    // 0x8016E618: nop

    // 0x8016E61C: bc1t        L_8016E63C
    if (c1cs) {
        // 0x8016E620: nop
    
            goto L_8016E63C;
    }
    // 0x8016E620: nop

    // 0x8016E624: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8016E628: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E62C: c.lt.s      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.fl < ctx->f2.fl;
    // 0x8016E630: nop

    // 0x8016E634: bc1f        L_8016E644
    if (!c1cs) {
        // 0x8016E638: nop
    
            goto L_8016E644;
    }
    // 0x8016E638: nop

L_8016E63C:
    // 0x8016E63C: b           L_8016E928
    // 0x8016E640: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016E928;
    // 0x8016E640: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8016E644:
    // 0x8016E644: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016E648: lwc1        $f10, 0x34($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016E64C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016E650: lwc1        $f0, -0x5114($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X5114);
    // 0x8016E654: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
    // 0x8016E658: add.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016E65C: swc1        $f8, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f8.u32l;
    // 0x8016E660: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8016E664: lwc1        $f10, 0x3C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016E668: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016E66C: add.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016E670: swc1        $f8, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f8.u32l;
    // 0x8016E674: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x8016E678: lwc1        $f4, 0x48($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X48);
    // 0x8016E67C: lwc1        $f10, 0x1C4($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X1C4);
    // 0x8016E680: lwc1        $f8, 0x19C($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X19C);
    // 0x8016E684: mul.s       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8016E688: add.s       $f18, $f6, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8016E68C: add.s       $f4, $f18, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f8.fl;
    // 0x8016E690: lwc1        $f8, 0x2C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8016E694: sub.s       $f18, $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = ctx->f18.fl - ctx->f0.fl;
    // 0x8016E698: swc1        $f4, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f4.u32l;
    // 0x8016E69C: lwc1        $f6, 0x68($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X68);
    // 0x8016E6A0: mul.s       $f4, $f8, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x8016E6A4: add.s       $f10, $f6, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f0.fl;
    // 0x8016E6A8: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016E6AC: swc1        $f10, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f10.u32l;
    // 0x8016E6B0: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016E6B4: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8016E6B8: swc1        $f10, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f10.u32l;
    // 0x8016E6BC: swc1        $f10, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f10.u32l;
    // 0x8016E6C0: lbu         $t1, 0x3E($a0)
    ctx->r9 = MEM_BU(ctx->r4, 0X3E);
    // 0x8016E6C4: lbu         $t2, 0x3F($a0)
    ctx->r10 = MEM_BU(ctx->r4, 0X3F);
    // 0x8016E6C8: beql        $t1, $at, L_8016E7B0
    if (ctx->r9 == ctx->r1) {
        // 0x8016E6CC: mtc1        $zero, $f6
        ctx->f6.u32l = 0;
            goto L_8016E7B0;
    }
    goto skip_10;
    // 0x8016E6CC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    skip_10:
    // 0x8016E6D0: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E6D4: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8016E6D8: mflo        $t9
    ctx->r25 = lo;
    // 0x8016E6DC: addu        $t3, $a0, $t9
    ctx->r11 = ADD32(ctx->r4, ctx->r25);
    // 0x8016E6E0: lh          $t7, 0x10($t3)
    ctx->r15 = MEM_H(ctx->r11, 0X10);
    // 0x8016E6E4: multu       $t1, $v0
    result = U64(U32(ctx->r9)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E6E8: mflo        $t6
    ctx->r14 = lo;
    // 0x8016E6EC: addu        $a3, $a0, $t6
    ctx->r7 = ADD32(ctx->r4, ctx->r14);
    // 0x8016E6F0: lh          $t0, 0x10($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X10);
    // 0x8016E6F4: subu        $t8, $t7, $t0
    ctx->r24 = SUB32(ctx->r15, ctx->r8);
    // 0x8016E6F8: mtc1        $t8, $f8
    ctx->f8.u32l = ctx->r24;
    // 0x8016E6FC: nop

    // 0x8016E700: cvt.s.w     $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    ctx->f2.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E704: c.eq.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl == ctx->f4.fl;
    // 0x8016E708: nop

    // 0x8016E70C: bc1tl       L_8016E75C
    if (c1cs) {
        // 0x8016E710: lh          $t0, 0x14($a3)
        ctx->r8 = MEM_H(ctx->r7, 0X14);
            goto L_8016E75C;
    }
    goto skip_11;
    // 0x8016E710: lh          $t0, 0x14($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X14);
    skip_11:
    // 0x8016E714: lh          $t1, 0x12($a3)
    ctx->r9 = MEM_H(ctx->r7, 0X12);
    // 0x8016E718: lh          $t9, 0x12($t3)
    ctx->r25 = MEM_H(ctx->r11, 0X12);
    // 0x8016E71C: mtc1        $t0, $f4
    ctx->f4.u32l = ctx->r8;
    // 0x8016E720: lwc1        $f8, 0x0($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016E724: subu        $t6, $t9, $t1
    ctx->r14 = SUB32(ctx->r25, ctx->r9);
    // 0x8016E728: mtc1        $t6, $f6
    ctx->f6.u32l = ctx->r14;
    // 0x8016E72C: nop

    // 0x8016E730: cvt.s.w     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016E734: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E738: div.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8016E73C: sub.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x8016E740: mtc1        $t1, $f8
    ctx->f8.u32l = ctx->r9;
    // 0x8016E744: nop

    // 0x8016E748: cvt.s.w     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E74C: mul.s       $f4, $f0, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x8016E750: b           L_8016E8D0
    // 0x8016E754: add.s       $f12, $f6, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f4.fl;
        goto L_8016E8D0;
    // 0x8016E754: add.s       $f12, $f6, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x8016E758: lh          $t0, 0x14($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X14);
L_8016E75C:
    // 0x8016E75C: lh          $t7, 0x14($t3)
    ctx->r15 = MEM_H(ctx->r11, 0X14);
    // 0x8016E760: lh          $t1, 0x12($a3)
    ctx->r9 = MEM_H(ctx->r7, 0X12);
    // 0x8016E764: lh          $t9, 0x12($t3)
    ctx->r25 = MEM_H(ctx->r11, 0X12);
    // 0x8016E768: subu        $t8, $t7, $t0
    ctx->r24 = SUB32(ctx->r15, ctx->r8);
    // 0x8016E76C: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x8016E770: subu        $t6, $t9, $t1
    ctx->r14 = SUB32(ctx->r25, ctx->r9);
    // 0x8016E774: mtc1        $t6, $f8
    ctx->f8.u32l = ctx->r14;
    // 0x8016E778: cvt.s.w     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    ctx->f2.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E77C: mtc1        $t0, $f10
    ctx->f10.u32l = ctx->r8;
    // 0x8016E780: lwc1        $f4, 0x8($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016E784: cvt.s.w     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E788: cvt.s.w     $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E78C: div.s       $f0, $f6, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016E790: sub.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x8016E794: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x8016E798: nop

    // 0x8016E79C: cvt.s.w     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016E7A0: mul.s       $f10, $f0, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x8016E7A4: b           L_8016E8D0
    // 0x8016E7A8: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
        goto L_8016E8D0;
    // 0x8016E7A8: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x8016E7AC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
L_8016E7B0:
    // 0x8016E7B0: nop

    // 0x8016E7B4: c.eq.s      $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f6.fl == ctx->f4.fl;
    // 0x8016E7B8: nop

    // 0x8016E7BC: bc1f        L_8016E84C
    if (!c1cs) {
        // 0x8016E7C0: nop
    
            goto L_8016E84C;
    }
    // 0x8016E7C0: nop

    // 0x8016E7C4: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E7C8: lwc1        $f14, 0x8($a1)
    ctx->f14.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016E7CC: or          $t2, $t0, $zero
    ctx->r10 = ctx->r8 | 0;
    // 0x8016E7D0: mflo        $t7
    ctx->r15 = lo;
    // 0x8016E7D4: addu        $t3, $a0, $t7
    ctx->r11 = ADD32(ctx->r4, ctx->r15);
    // 0x8016E7D8: lh          $t1, 0x14($t3)
    ctx->r9 = MEM_H(ctx->r11, 0X14);
    // 0x8016E7DC: mtc1        $t1, $f8
    ctx->f8.u32l = ctx->r9;
    // 0x8016E7E0: nop

    // 0x8016E7E4: cvt.s.w     $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    ctx->f16.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E7E8: c.lt.s      $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f14.fl < ctx->f16.fl;
    // 0x8016E7EC: nop

    // 0x8016E7F0: bc1f        L_8016E800
    if (!c1cs) {
        // 0x8016E7F4: nop
    
            goto L_8016E800;
    }
    // 0x8016E7F4: nop

    // 0x8016E7F8: b           L_8016E800
    // 0x8016E7FC: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
        goto L_8016E800;
    // 0x8016E7FC: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
L_8016E800:
    // 0x8016E800: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E804: lh          $a3, 0x12($t3)
    ctx->r7 = MEM_H(ctx->r11, 0X12);
    // 0x8016E808: mtc1        $a3, $f8
    ctx->f8.u32l = ctx->r7;
    // 0x8016E80C: mflo        $t8
    ctx->r24 = lo;
    // 0x8016E810: addu        $t0, $a0, $t8
    ctx->r8 = ADD32(ctx->r4, ctx->r24);
    // 0x8016E814: lh          $t9, 0x14($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X14);
    // 0x8016E818: lh          $t7, 0x12($t0)
    ctx->r15 = MEM_H(ctx->r8, 0X12);
    // 0x8016E81C: subu        $t6, $t9, $t1
    ctx->r14 = SUB32(ctx->r25, ctx->r9);
    // 0x8016E820: subu        $t8, $t7, $a3
    ctx->r24 = SUB32(ctx->r15, ctx->r7);
    // 0x8016E824: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x8016E828: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x8016E82C: cvt.s.w     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    ctx->f2.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E830: cvt.s.w     $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016E834: sub.s       $f6, $f14, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016E838: div.s       $f0, $f4, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016E83C: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E840: mul.s       $f4, $f0, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x8016E844: b           L_8016E8D0
    // 0x8016E848: add.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f4.fl;
        goto L_8016E8D0;
    // 0x8016E848: add.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f4.fl;
L_8016E84C:
    // 0x8016E84C: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E850: lwc1        $f14, 0x0($a1)
    ctx->f14.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016E854: or          $t2, $t0, $zero
    ctx->r10 = ctx->r8 | 0;
    // 0x8016E858: mflo        $t9
    ctx->r25 = lo;
    // 0x8016E85C: addu        $t3, $a0, $t9
    ctx->r11 = ADD32(ctx->r4, ctx->r25);
    // 0x8016E860: lh          $t1, 0x10($t3)
    ctx->r9 = MEM_H(ctx->r11, 0X10);
    // 0x8016E864: mtc1        $t1, $f8
    ctx->f8.u32l = ctx->r9;
    // 0x8016E868: nop

    // 0x8016E86C: cvt.s.w     $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    ctx->f16.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E870: c.lt.s      $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f14.fl < ctx->f16.fl;
    // 0x8016E874: nop

    // 0x8016E878: bc1f        L_8016E888
    if (!c1cs) {
        // 0x8016E87C: nop
    
            goto L_8016E888;
    }
    // 0x8016E87C: nop

    // 0x8016E880: b           L_8016E888
    // 0x8016E884: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
        goto L_8016E888;
    // 0x8016E884: or          $t2, $a3, $zero
    ctx->r10 = ctx->r7 | 0;
L_8016E888:
    // 0x8016E888: multu       $t2, $v0
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016E88C: lh          $a3, 0x12($t3)
    ctx->r7 = MEM_H(ctx->r11, 0X12);
    // 0x8016E890: mtc1        $a3, $f8
    ctx->f8.u32l = ctx->r7;
    // 0x8016E894: mflo        $t6
    ctx->r14 = lo;
    // 0x8016E898: addu        $t0, $a0, $t6
    ctx->r8 = ADD32(ctx->r4, ctx->r14);
    // 0x8016E89C: lh          $t7, 0x10($t0)
    ctx->r15 = MEM_H(ctx->r8, 0X10);
    // 0x8016E8A0: lh          $t9, 0x12($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X12);
    // 0x8016E8A4: subu        $t8, $t7, $t1
    ctx->r24 = SUB32(ctx->r15, ctx->r9);
    // 0x8016E8A8: subu        $t6, $t9, $a3
    ctx->r14 = SUB32(ctx->r25, ctx->r7);
    // 0x8016E8AC: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x8016E8B0: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x8016E8B4: cvt.s.w     $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    ctx->f2.fl = CVT_S_W(ctx->f6.u32l);
    // 0x8016E8B8: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8016E8BC: sub.s       $f10, $f14, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x8016E8C0: div.s       $f0, $f4, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8016E8C4: cvt.s.w     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8016E8C8: mul.s       $f4, $f0, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x8016E8CC: add.s       $f12, $f6, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f4.fl;
L_8016E8D0:
    // 0x8016E8D0: lwc1        $f8, 0x1C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x8016E8D4: c.lt.s      $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f12.fl < ctx->f8.fl;
    // 0x8016E8D8: nop

    // 0x8016E8DC: bc1fl       L_8016E8F0
    if (!c1cs) {
        // 0x8016E8E0: c.lt.s      $f12, $f18
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f12.fl < ctx->f18.fl;
            goto L_8016E8F0;
    }
    goto skip_12;
    // 0x8016E8E0: c.lt.s      $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f12.fl < ctx->f18.fl;
    skip_12:
    // 0x8016E8E4: swc1        $f12, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f12.u32l;
    // 0x8016E8E8: mov.s       $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    ctx->f12.fl = ctx->f8.fl;
    // 0x8016E8EC: c.lt.s      $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f12.fl < ctx->f18.fl;
L_8016E8F0:
    // 0x8016E8F0: lwc1        $f10, 0x68($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X68);
    // 0x8016E8F4: lwc1        $f6, 0x60($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X60);
    // 0x8016E8F8: bc1t        L_8016E910
    if (c1cs) {
        // 0x8016E8FC: nop
    
            goto L_8016E910;
    }
    // 0x8016E8FC: nop

    // 0x8016E900: c.lt.s      $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f10.fl < ctx->f6.fl;
    // 0x8016E904: nop

    // 0x8016E908: bc1fl       L_8016E91C
    if (!c1cs) {
        // 0x8016E90C: lwc1        $f4, 0x1A4($v1)
        ctx->f4.u32l = MEM_W(ctx->r3, 0X1A4);
            goto L_8016E91C;
    }
    goto skip_13;
    // 0x8016E90C: lwc1        $f4, 0x1A4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X1A4);
    skip_13:
L_8016E910:
    // 0x8016E910: b           L_8016E928
    // 0x8016E914: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
        goto L_8016E928;
    // 0x8016E914: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x8016E918: lwc1        $f4, 0x1A4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X1A4);
L_8016E91C:
    // 0x8016E91C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016E920: sub.s       $f8, $f18, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x8016E924: swc1        $f8, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f8.u32l;
L_8016E928:
    // 0x8016E928: jr          $ra
    // 0x8016E92C: addiu       $sp, $sp, 0x88
    ctx->r29 = ADD32(ctx->r29, 0X88);
    return;
    // 0x8016E92C: addiu       $sp, $sp, 0x88
    ctx->r29 = ADD32(ctx->r29, 0X88);
;}
RECOMP_FUNC void func_8016E930(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016E930: lui         $a2, 0x801A
    ctx->r6 = S32(0X801A << 16);
    // 0x8016E934: addiu       $a2, $a2, -0x9FC
    ctx->r6 = ADD32(ctx->r6, -0X9FC);
    // 0x8016E938: sw          $a0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r4;
    // 0x8016E93C: sw          $zero, 0x22C($a0)
    MEM_W(0X22C, ctx->r4) = 0;
    // 0x8016E940: lw          $t7, 0x0($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X0);
    // 0x8016E944: sw          $zero, 0x488($t7)
    MEM_W(0X488, ctx->r15) = 0;
    // 0x8016E948: lw          $t8, 0x0($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X0);
    // 0x8016E94C: sw          $zero, 0x6F0($t8)
    MEM_W(0X6F0, ctx->r24) = 0;
    // 0x8016E950: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x8016E954: lw          $v1, 0x6F0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X6F0);
    // 0x8016E958: sw          $v1, 0x6EC($v0)
    MEM_W(0X6EC, ctx->r2) = ctx->r3;
    // 0x8016E95C: lw          $t9, 0x0($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X0);
    // 0x8016E960: sw          $v1, 0x6E8($t9)
    MEM_W(0X6E8, ctx->r25) = ctx->r3;
    // 0x8016E964: lw          $t0, 0x0($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X0);
    // 0x8016E968: sw          $a1, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->r5;
    // 0x8016E96C: lw          $t1, 0x0($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X0);
    // 0x8016E970: sh          $zero, 0x290($t1)
    MEM_H(0X290, ctx->r9) = 0;
    // 0x8016E974: lw          $t2, 0x0($a2)
    ctx->r10 = MEM_W(ctx->r6, 0X0);
    // 0x8016E978: jr          $ra
    // 0x8016E97C: sh          $zero, 0x4EC($t2)
    MEM_H(0X4EC, ctx->r10) = 0;
    return;
    // 0x8016E97C: sh          $zero, 0x4EC($t2)
    MEM_H(0X4EC, ctx->r10) = 0;
;}
RECOMP_FUNC void func_8016E980(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016E980: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x8016E984: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x8016E988: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x8016E98C: addiu       $t0, $t0, -0x9FC
    ctx->r8 = ADD32(ctx->r8, -0X9FC);
    // 0x8016E990: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x8016E994: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8016E998: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8016E99C:
    // 0x8016E99C: lw          $t6, 0x0($t0)
    ctx->r14 = MEM_W(ctx->r8, 0X0);
    // 0x8016E9A0: lwc1        $f0, 0x0($a2)
    ctx->f0.u32l = MEM_W(ctx->r6, 0X0);
    // 0x8016E9A4: addiu       $a2, $a2, 0x4
    ctx->r6 = ADD32(ctx->r6, 0X4);
    // 0x8016E9A8: addu        $t7, $t6, $v1
    ctx->r15 = ADD32(ctx->r14, ctx->r3);
    // 0x8016E9AC: swc1        $f0, 0x184($t7)
    MEM_W(0X184, ctx->r15) = ctx->f0.u32l;
    // 0x8016E9B0: lw          $t8, 0x0($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X0);
    // 0x8016E9B4: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016E9B8: addu        $t9, $t8, $v1
    ctx->r25 = ADD32(ctx->r24, ctx->r3);
    // 0x8016E9BC: swc1        $f0, 0x16C($t9)
    MEM_W(0X16C, ctx->r25) = ctx->f0.u32l;
    // 0x8016E9C0: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x8016E9C4: addu        $t2, $t1, $v1
    ctx->r10 = ADD32(ctx->r9, ctx->r3);
    // 0x8016E9C8: swc1        $f0, 0x14($t2)
    MEM_W(0X14, ctx->r10) = ctx->f0.u32l;
    // 0x8016E9CC: lw          $t3, 0x0($t0)
    ctx->r11 = MEM_W(ctx->r8, 0X0);
    // 0x8016E9D0: lwc1        $f4, -0x4($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, -0X4);
    // 0x8016E9D4: addu        $t4, $t3, $v1
    ctx->r12 = ADD32(ctx->r11, ctx->r3);
    // 0x8016E9D8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016E9DC: slti        $at, $v1, 0xC
    ctx->r1 = SIGNED(ctx->r3) < 0XC ? 1 : 0;
    // 0x8016E9E0: bne         $at, $zero, L_8016E99C
    if (ctx->r1 != 0) {
        // 0x8016E9E4: swc1        $f4, 0x20($t4)
        MEM_W(0X20, ctx->r12) = ctx->f4.u32l;
            goto L_8016E99C;
    }
    // 0x8016E9E4: swc1        $f4, 0x20($t4)
    MEM_W(0X20, ctx->r12) = ctx->f4.u32l;
    // 0x8016E9E8: lw          $a0, 0x0($t0)
    ctx->r4 = MEM_W(ctx->r8, 0X0);
    // 0x8016E9EC: sub.s       $f18, $f12, $f14
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f18.fl = ctx->f12.fl - ctx->f14.fl;
    // 0x8016E9F0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016E9F4: lwc1        $f6, 0x170($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X170);
    // 0x8016E9F8: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x8016E9FC: add.s       $f8, $f6, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f12.fl;
    // 0x8016EA00: swc1        $f8, 0x170($a0)
    MEM_W(0X170, ctx->r4) = ctx->f8.u32l;
    // 0x8016EA04: lw          $a0, 0x0($t0)
    ctx->r4 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA08: lwc1        $f10, 0x188($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X188);
    // 0x8016EA0C: add.s       $f16, $f10, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f14.fl;
    // 0x8016EA10: swc1        $f16, 0x188($a0)
    MEM_W(0X188, ctx->r4) = ctx->f16.u32l;
    // 0x8016EA14: lw          $t5, 0x0($t0)
    ctx->r13 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA18: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    // 0x8016EA1C: swc1        $f18, 0x19C($t5)
    MEM_W(0X19C, ctx->r13) = ctx->f18.u32l;
    // 0x8016EA20: lw          $t6, 0x0($t0)
    ctx->r14 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA24: swc1        $f12, 0x1A0($t6)
    MEM_W(0X1A0, ctx->r14) = ctx->f12.u32l;
    // 0x8016EA28: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA2C: swc1        $f14, 0x1A4($t7)
    MEM_W(0X1A4, ctx->r15) = ctx->f14.u32l;
L_8016EA30:
    // 0x8016EA30: lw          $t8, 0x0($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA34: lwc1        $f4, 0x0($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X0);
    // 0x8016EA38: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x8016EA3C: addu        $v0, $t8, $v1
    ctx->r2 = ADD32(ctx->r24, ctx->r3);
    // 0x8016EA40: lwc1        $f6, 0x16C($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X16C);
    // 0x8016EA44: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016EA48: swc1        $f8, 0x178($v0)
    MEM_W(0X178, ctx->r2) = ctx->f8.u32l;
    // 0x8016EA4C: lw          $t9, 0x0($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA50: lwc1        $f10, -0x4($a3)
    ctx->f10.u32l = MEM_W(ctx->r7, -0X4);
    // 0x8016EA54: addu        $v0, $t9, $v1
    ctx->r2 = ADD32(ctx->r25, ctx->r3);
    // 0x8016EA58: lwc1        $f16, 0x184($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X184);
    // 0x8016EA5C: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x8016EA60: swc1        $f18, 0x190($v0)
    MEM_W(0X190, ctx->r2) = ctx->f18.u32l;
    // 0x8016EA64: lw          $t1, 0x0($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA68: lwc1        $f4, -0x4($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, -0X4);
    // 0x8016EA6C: addu        $v0, $t1, $v1
    ctx->r2 = ADD32(ctx->r9, ctx->r3);
    // 0x8016EA70: lwc1        $f6, 0x14($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X14);
    // 0x8016EA74: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8016EA78: swc1        $f8, 0x2C($v0)
    MEM_W(0X2C, ctx->r2) = ctx->f8.u32l;
    // 0x8016EA7C: lw          $t2, 0x0($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X0);
    // 0x8016EA80: addu        $v0, $t2, $v1
    ctx->r2 = ADD32(ctx->r10, ctx->r3);
    // 0x8016EA84: lwc1        $f10, 0x184($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X184);
    // 0x8016EA88: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016EA8C: bne         $v1, $a0, L_8016EA30
    if (ctx->r3 != ctx->r4) {
        // 0x8016EA90: swc1        $f10, 0xA4($v0)
        MEM_W(0XA4, ctx->r2) = ctx->f10.u32l;
            goto L_8016EA30;
    }
    // 0x8016EA90: swc1        $f10, 0xA4($v0)
    MEM_W(0XA4, ctx->r2) = ctx->f10.u32l;
    // 0x8016EA94: jr          $ra
    // 0x8016EA98: nop

    return;
    // 0x8016EA98: nop

;}
RECOMP_FUNC void func_8016EA9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016EA9C: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x8016EAA0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016EAA4: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016EAA8: addiu       $s0, $s0, -0x9FC
    ctx->r16 = ADD32(ctx->r16, -0X9FC);
    // 0x8016EAAC: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016EAB0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016EAB4: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016EAB8: sw          $a0, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->r4;
    // 0x8016EABC: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x8016EAC0: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x8016EAC4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8016EAC8: sw          $a1, 0x8($t7)
    MEM_W(0X8, ctx->r15) = ctx->r5;
    // 0x8016EACC: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EAD0: addiu       $a0, $sp, 0x48
    ctx->r4 = ADD32(ctx->r29, 0X48);
    // 0x8016EAD4: addiu       $v1, $sp, 0x3C
    ctx->r3 = ADD32(ctx->r29, 0X3C);
    // 0x8016EAD8: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x8016EADC: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016EAE0: sltu        $at, $v1, $a0
    ctx->r1 = ctx->r3 < ctx->r4 ? 1 : 0;
    // 0x8016EAE4: beq         $at, $zero, L_8016EB18
    if (ctx->r1 == 0) {
        // 0x8016EAE8: lh          $t8, 0x0($a2)
        ctx->r24 = MEM_H(ctx->r6, 0X0);
            goto L_8016EB18;
    }
    // 0x8016EAE8: lh          $t8, 0x0($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X0);
L_8016EAEC:
    // 0x8016EAEC: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016EAF0: lwc1        $f16, 0x14($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X14);
    // 0x8016EAF4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016EAF8: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016EAFC: sltu        $at, $v1, $a0
    ctx->r1 = ctx->r3 < ctx->r4 ? 1 : 0;
    // 0x8016EB00: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016EB04: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x8016EB08: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016EB0C: swc1        $f18, -0x8($v1)
    MEM_W(-0X8, ctx->r3) = ctx->f18.u32l;
    // 0x8016EB10: bne         $at, $zero, L_8016EAEC
    if (ctx->r1 != 0) {
        // 0x8016EB14: lh          $t8, 0x0($a2)
        ctx->r24 = MEM_H(ctx->r6, 0X0);
            goto L_8016EAEC;
    }
    // 0x8016EB14: lh          $t8, 0x0($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X0);
L_8016EB18:
    // 0x8016EB18: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8016EB1C: lwc1        $f16, 0x14($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X14);
    // 0x8016EB20: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016EB24: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x8016EB28: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x8016EB2C: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x8016EB30: swc1        $f18, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f18.u32l;
    // 0x8016EB34: addiu       $a0, $s1, 0x1E4
    ctx->r4 = ADD32(ctx->r17, 0X1E4);
    // 0x8016EB38: jal         0x801739B8
    // 0x8016EB3C: lh          $a1, 0x8($a3)
    ctx->r5 = MEM_H(ctx->r7, 0X8);
    func_801739B8(rdram, ctx);
        goto after_0;
    // 0x8016EB3C: lh          $a1, 0x8($a3)
    ctx->r5 = MEM_H(ctx->r7, 0X8);
    after_0:
    // 0x8016EB40: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EB44: addiu       $a0, $s1, 0x208
    ctx->r4 = ADD32(ctx->r17, 0X208);
    // 0x8016EB48: jal         0x80173BC0
    // 0x8016EB4C: addiu       $a1, $s1, 0x1E4
    ctx->r5 = ADD32(ctx->r17, 0X1E4);
    func_80173BC0(rdram, ctx);
        goto after_1;
    // 0x8016EB4C: addiu       $a1, $s1, 0x1E4
    ctx->r5 = ADD32(ctx->r17, 0X1E4);
    after_1:
    // 0x8016EB50: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EB54: addiu       $a1, $sp, 0x3C
    ctx->r5 = ADD32(ctx->r29, 0X3C);
    // 0x8016EB58: addiu       $a0, $s1, 0x208
    ctx->r4 = ADD32(ctx->r17, 0X208);
    // 0x8016EB5C: jal         0x80173524
    // 0x8016EB60: addiu       $a2, $s1, 0x38
    ctx->r6 = ADD32(ctx->r17, 0X38);
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x8016EB60: addiu       $a2, $s1, 0x38
    ctx->r6 = ADD32(ctx->r17, 0X38);
    after_2:
    // 0x8016EB64: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EB68: addiu       $a0, $s1, 0x208
    ctx->r4 = ADD32(ctx->r17, 0X208);
    // 0x8016EB6C: addiu       $a1, $s1, 0x20
    ctx->r5 = ADD32(ctx->r17, 0X20);
    // 0x8016EB70: jal         0x80173524
    // 0x8016EB74: addiu       $a2, $s1, 0x44
    ctx->r6 = ADD32(ctx->r17, 0X44);
    func_80173524(rdram, ctx);
        goto after_3;
    // 0x8016EB74: addiu       $a2, $s1, 0x44
    ctx->r6 = ADD32(ctx->r17, 0X44);
    after_3:
    // 0x8016EB78: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8016EB7C:
    // 0x8016EB7C: lw          $t9, 0x0($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X0);
    // 0x8016EB80: addu        $v0, $t9, $v1
    ctx->r2 = ADD32(ctx->r25, ctx->r3);
    // 0x8016EB84: lwc1        $f2, 0x38($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X38);
    // 0x8016EB88: swc1        $f2, 0x1C0($v0)
    MEM_W(0X1C0, ctx->r2) = ctx->f2.u32l;
    // 0x8016EB8C: lw          $t1, 0x0($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X0);
    // 0x8016EB90: addu        $t2, $t1, $v1
    ctx->r10 = ADD32(ctx->r9, ctx->r3);
    // 0x8016EB94: swc1        $f2, 0x1A8($t2)
    MEM_W(0X1A8, ctx->r10) = ctx->f2.u32l;
    // 0x8016EB98: lw          $t3, 0x0($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X0);
    // 0x8016EB9C: addu        $v0, $t3, $v1
    ctx->r2 = ADD32(ctx->r11, ctx->r3);
    // 0x8016EBA0: lwc1        $f16, 0x44($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X44);
    // 0x8016EBA4: lwc1        $f18, 0x38($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X38);
    // 0x8016EBA8: add.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x8016EBAC: swc1        $f0, 0x1CC($v0)
    MEM_W(0X1CC, ctx->r2) = ctx->f0.u32l;
    // 0x8016EBB0: lw          $t4, 0x0($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X0);
    // 0x8016EBB4: addu        $t5, $t4, $v1
    ctx->r13 = ADD32(ctx->r12, ctx->r3);
    // 0x8016EBB8: swc1        $f0, 0x1B4($t5)
    MEM_W(0X1B4, ctx->r13) = ctx->f0.u32l;
    // 0x8016EBBC: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8016EBC0: addu        $t7, $t6, $v1
    ctx->r15 = ADD32(ctx->r14, ctx->r3);
    // 0x8016EBC4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016EBC8: slti        $at, $v1, 0xC
    ctx->r1 = SIGNED(ctx->r3) < 0XC ? 1 : 0;
    // 0x8016EBCC: bne         $at, $zero, L_8016EB7C
    if (ctx->r1 != 0) {
        // 0x8016EBD0: swc1        $f0, 0x50($t7)
        MEM_W(0X50, ctx->r15) = ctx->f0.u32l;
            goto L_8016EB7C;
    }
    // 0x8016EBD0: swc1        $f0, 0x50($t7)
    MEM_W(0X50, ctx->r15) = ctx->f0.u32l;
    // 0x8016EBD4: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EBD8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016EBDC: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x8016EBE0: lwc1        $f6, 0x1AC($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X1AC);
    // 0x8016EBE4: lwc1        $f4, 0x1A0($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X1A0);
    // 0x8016EBE8: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x8016EBEC: swc1        $f8, 0x1AC($s1)
    MEM_W(0X1AC, ctx->r17) = ctx->f8.u32l;
    // 0x8016EBF0: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EBF4: lwc1        $f10, 0x1B8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X1B8);
    // 0x8016EBF8: lwc1        $f16, 0x1A0($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X1A0);
    // 0x8016EBFC: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x8016EC00: swc1        $f18, 0x1B8($s1)
    MEM_W(0X1B8, ctx->r17) = ctx->f18.u32l;
    // 0x8016EC04: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EC08: lwc1        $f6, 0x1C4($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X1C4);
    // 0x8016EC0C: lwc1        $f4, 0x1A4($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X1A4);
    // 0x8016EC10: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x8016EC14: swc1        $f8, 0x1C4($s1)
    MEM_W(0X1C4, ctx->r17) = ctx->f8.u32l;
    // 0x8016EC18: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EC1C: lwc1        $f10, 0x1D0($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X1D0);
    // 0x8016EC20: lwc1        $f16, 0x1A4($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X1A4);
    // 0x8016EC24: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x8016EC28: swc1        $f18, 0x1D0($s1)
    MEM_W(0X1D0, ctx->r17) = ctx->f18.u32l;
    // 0x8016EC2C: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
L_8016EC30:
    // 0x8016EC30: addu        $v0, $t8, $v1
    ctx->r2 = ADD32(ctx->r24, ctx->r3);
    // 0x8016EC34: lwc1        $f0, 0x1C0($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X1C0);
    // 0x8016EC38: lwc1        $f2, 0x1CC($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X1CC);
    // 0x8016EC3C: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x8016EC40: nop

    // 0x8016EC44: bc1fl       L_8016EC68
    if (!c1cs) {
        // 0x8016EC48: swc1        $f2, 0x150($v0)
        MEM_W(0X150, ctx->r2) = ctx->f2.u32l;
            goto L_8016EC68;
    }
    goto skip_0;
    // 0x8016EC48: swc1        $f2, 0x150($v0)
    MEM_W(0X150, ctx->r2) = ctx->f2.u32l;
    skip_0:
    // 0x8016EC4C: swc1        $f0, 0x150($v0)
    MEM_W(0X150, ctx->r2) = ctx->f0.u32l;
    // 0x8016EC50: lw          $t9, 0x0($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X0);
    // 0x8016EC54: addu        $v0, $t9, $v1
    ctx->r2 = ADD32(ctx->r25, ctx->r3);
    // 0x8016EC58: lwc1        $f6, 0x1CC($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X1CC);
    // 0x8016EC5C: b           L_8016EC78
    // 0x8016EC60: swc1        $f6, 0x15C($v0)
    MEM_W(0X15C, ctx->r2) = ctx->f6.u32l;
        goto L_8016EC78;
    // 0x8016EC60: swc1        $f6, 0x15C($v0)
    MEM_W(0X15C, ctx->r2) = ctx->f6.u32l;
    // 0x8016EC64: swc1        $f2, 0x150($v0)
    MEM_W(0X150, ctx->r2) = ctx->f2.u32l;
L_8016EC68:
    // 0x8016EC68: lw          $t1, 0x0($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X0);
    // 0x8016EC6C: addu        $v0, $t1, $v1
    ctx->r2 = ADD32(ctx->r9, ctx->r3);
    // 0x8016EC70: lwc1        $f4, 0x1C0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X1C0);
    // 0x8016EC74: swc1        $f4, 0x15C($v0)
    MEM_W(0X15C, ctx->r2) = ctx->f4.u32l;
L_8016EC78:
    // 0x8016EC78: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x8016EC7C: bnel        $v1, $a0, L_8016EC30
    if (ctx->r3 != ctx->r4) {
        // 0x8016EC80: lw          $t8, 0x0($s0)
        ctx->r24 = MEM_W(ctx->r16, 0X0);
            goto L_8016EC30;
    }
    goto skip_1;
    // 0x8016EC80: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
    skip_1:
    // 0x8016EC84: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016EC88: lwc1        $f0, 0x1C4($s1)
    ctx->f0.u32l = MEM_W(ctx->r17, 0X1C4);
    // 0x8016EC8C: lwc1        $f2, 0x1D0($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X1D0);
    // 0x8016EC90: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x8016EC94: nop

    // 0x8016EC98: bc1fl       L_8016ECAC
    if (!c1cs) {
        // 0x8016EC9C: swc1        $f2, 0x154($s1)
        MEM_W(0X154, ctx->r17) = ctx->f2.u32l;
            goto L_8016ECAC;
    }
    goto skip_2;
    // 0x8016EC9C: swc1        $f2, 0x154($s1)
    MEM_W(0X154, ctx->r17) = ctx->f2.u32l;
    skip_2:
    // 0x8016ECA0: b           L_8016ECAC
    // 0x8016ECA4: swc1        $f0, 0x154($s1)
    MEM_W(0X154, ctx->r17) = ctx->f0.u32l;
        goto L_8016ECAC;
    // 0x8016ECA4: swc1        $f0, 0x154($s1)
    MEM_W(0X154, ctx->r17) = ctx->f0.u32l;
    // 0x8016ECA8: swc1        $f2, 0x154($s1)
    MEM_W(0X154, ctx->r17) = ctx->f2.u32l;
L_8016ECAC:
    // 0x8016ECAC: lw          $s1, 0x0($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X0);
    // 0x8016ECB0: lwc1        $f8, 0x1AC($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X1AC);
    // 0x8016ECB4: lwc1        $f10, 0x1B8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X1B8);
    // 0x8016ECB8: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016ECBC: nop

    // 0x8016ECC0: bc1fl       L_8016ECD8
    if (!c1cs) {
        // 0x8016ECC4: lwc1        $f18, 0x1C4($s1)
        ctx->f18.u32l = MEM_W(ctx->r17, 0X1C4);
            goto L_8016ECD8;
    }
    goto skip_3;
    // 0x8016ECC4: lwc1        $f18, 0x1C4($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X1C4);
    skip_3:
    // 0x8016ECC8: lwc1        $f16, 0x1D0($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X1D0);
    // 0x8016ECCC: b           L_8016ECDC
    // 0x8016ECD0: swc1        $f16, 0x160($s1)
    MEM_W(0X160, ctx->r17) = ctx->f16.u32l;
        goto L_8016ECDC;
    // 0x8016ECD0: swc1        $f16, 0x160($s1)
    MEM_W(0X160, ctx->r17) = ctx->f16.u32l;
    // 0x8016ECD4: lwc1        $f18, 0x1C4($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X1C4);
L_8016ECD8:
    // 0x8016ECD8: swc1        $f18, 0x160($s1)
    MEM_W(0X160, ctx->r17) = ctx->f18.u32l;
L_8016ECDC:
    // 0x8016ECDC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8016ECE0: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016ECE4: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016ECE8: jr          $ra
    // 0x8016ECEC: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x8016ECEC: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_8016ECF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016ECF0: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x8016ECF4: sw          $s0, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r16;
    // 0x8016ECF8: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016ECFC: lwc1        $f0, 0x0($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016ED00: mtc1        $a3, $f12
    ctx->f12.u32l = ctx->r7;
    // 0x8016ED04: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x8016ED08: nop

    // 0x8016ED0C: bc1fl       L_8016ED20
    if (!c1cs) {
        // 0x8016ED10: lwc1        $f6, 0xC($a0)
        ctx->f6.u32l = MEM_W(ctx->r4, 0XC);
            goto L_8016ED20;
    }
    goto skip_0;
    // 0x8016ED10: lwc1        $f6, 0xC($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0XC);
    skip_0:
    // 0x8016ED14: b           L_8016ED40
    // 0x8016ED18: sh          $zero, 0x2C($sp)
    MEM_H(0X2C, ctx->r29) = 0;
        goto L_8016ED40;
    // 0x8016ED18: sh          $zero, 0x2C($sp)
    MEM_H(0X2C, ctx->r29) = 0;
    // 0x8016ED1C: lwc1        $f6, 0xC($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0XC);
L_8016ED20:
    // 0x8016ED20: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
    // 0x8016ED24: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x8016ED28: nop

    // 0x8016ED2C: bc1f        L_8016ED3C
    if (!c1cs) {
        // 0x8016ED30: nop
    
            goto L_8016ED3C;
    }
    // 0x8016ED30: nop

    // 0x8016ED34: b           L_8016ED3C
    // 0x8016ED38: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016ED3C;
    // 0x8016ED38: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016ED3C:
    // 0x8016ED3C: sh          $v1, 0x2C($sp)
    MEM_H(0X2C, ctx->r29) = ctx->r3;
L_8016ED40:
    // 0x8016ED40: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8016ED44: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016ED48: add.s       $f8, $f0, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f0.fl + ctx->f12.fl;
    // 0x8016ED4C: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016ED50: nop

    // 0x8016ED54: bc1fl       L_8016ED68
    if (!c1cs) {
        // 0x8016ED58: lwc1        $f4, 0x10($a0)
        ctx->f4.u32l = MEM_W(ctx->r4, 0X10);
            goto L_8016ED68;
    }
    goto skip_1;
    // 0x8016ED58: lwc1        $f4, 0x10($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X10);
    skip_1:
    // 0x8016ED5C: b           L_8016ED88
    // 0x8016ED60: sh          $zero, 0x2E($sp)
    MEM_H(0X2E, ctx->r29) = 0;
        goto L_8016ED88;
    // 0x8016ED60: sh          $zero, 0x2E($sp)
    MEM_H(0X2E, ctx->r29) = 0;
    // 0x8016ED64: lwc1        $f4, 0x10($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X10);
L_8016ED68:
    // 0x8016ED68: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
    // 0x8016ED6C: c.lt.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl < ctx->f0.fl;
    // 0x8016ED70: nop

    // 0x8016ED74: bc1f        L_8016ED84
    if (!c1cs) {
        // 0x8016ED78: nop
    
            goto L_8016ED84;
    }
    // 0x8016ED78: nop

    // 0x8016ED7C: b           L_8016ED84
    // 0x8016ED80: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016ED84;
    // 0x8016ED80: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016ED84:
    // 0x8016ED84: sh          $v1, 0x2E($sp)
    MEM_H(0X2E, ctx->r29) = ctx->r3;
L_8016ED88:
    // 0x8016ED88: lwc1        $f0, 0x8($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016ED8C: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016ED90: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x8016ED94: nop

    // 0x8016ED98: bc1fl       L_8016EDAC
    if (!c1cs) {
        // 0x8016ED9C: lwc1        $f8, 0x14($a0)
        ctx->f8.u32l = MEM_W(ctx->r4, 0X14);
            goto L_8016EDAC;
    }
    goto skip_2;
    // 0x8016ED9C: lwc1        $f8, 0x14($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X14);
    skip_2:
    // 0x8016EDA0: b           L_8016EDCC
    // 0x8016EDA4: sh          $zero, 0x30($sp)
    MEM_H(0X30, ctx->r29) = 0;
        goto L_8016EDCC;
    // 0x8016EDA4: sh          $zero, 0x30($sp)
    MEM_H(0X30, ctx->r29) = 0;
    // 0x8016EDA8: lwc1        $f8, 0x14($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X14);
L_8016EDAC:
    // 0x8016EDAC: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
    // 0x8016EDB0: c.lt.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl < ctx->f0.fl;
    // 0x8016EDB4: nop

    // 0x8016EDB8: bc1f        L_8016EDC8
    if (!c1cs) {
        // 0x8016EDBC: nop
    
            goto L_8016EDC8;
    }
    // 0x8016EDBC: nop

    // 0x8016EDC0: b           L_8016EDC8
    // 0x8016EDC4: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016EDC8;
    // 0x8016EDC4: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016EDC8:
    // 0x8016EDC8: sh          $v1, 0x30($sp)
    MEM_H(0X30, ctx->r29) = ctx->r3;
L_8016EDCC:
    // 0x8016EDCC: lh          $t6, 0x2C($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X2C);
    // 0x8016EDD0: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x8016EDD4: lh          $t7, 0x2E($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X2E);
    // 0x8016EDD8: bnel        $v0, $t6, L_8016EDFC
    if (ctx->r2 != ctx->r14) {
        // 0x8016EDDC: lwc1        $f10, 0x0($a2)
        ctx->f10.u32l = MEM_W(ctx->r6, 0X0);
            goto L_8016EDFC;
    }
    goto skip_3;
    // 0x8016EDDC: lwc1        $f10, 0x0($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X0);
    skip_3:
    // 0x8016EDE0: bne         $v0, $t7, L_8016EDF8
    if (ctx->r2 != ctx->r15) {
        // 0x8016EDE4: lh          $t8, 0x30($sp)
        ctx->r24 = MEM_H(ctx->r29, 0X30);
            goto L_8016EDF8;
    }
    // 0x8016EDE4: lh          $t8, 0x30($sp)
    ctx->r24 = MEM_H(ctx->r29, 0X30);
    // 0x8016EDE8: bnel        $v0, $t8, L_8016EDFC
    if (ctx->r2 != ctx->r24) {
        // 0x8016EDEC: lwc1        $f10, 0x0($a2)
        ctx->f10.u32l = MEM_W(ctx->r6, 0X0);
            goto L_8016EDFC;
    }
    goto skip_4;
    // 0x8016EDEC: lwc1        $f10, 0x0($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X0);
    skip_4:
    // 0x8016EDF0: b           L_8016F134
    // 0x8016EDF4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016F134;
    // 0x8016EDF4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8016EDF8:
    // 0x8016EDF8: lwc1        $f10, 0x0($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X0);
L_8016EDFC:
    // 0x8016EDFC: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016EE00: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8016EE04: swc1        $f6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f6.u32l;
    // 0x8016EE08: lwc1        $f10, 0x4($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8016EE0C: lwc1        $f8, 0x4($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8016EE10: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x8016EE14: swc1        $f4, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f4.u32l;
    // 0x8016EE18: lwc1        $f8, 0x8($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016EE1C: lwc1        $f6, 0x8($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X8);
    // 0x8016EE20: lwc1        $f4, 0x40($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016EE24: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x8016EE28: swc1        $f10, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f10.u32l;
    // 0x8016EE2C: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016EE30: lwc1        $f10, 0x40($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X40);
    // 0x8016EE34: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016EE38: nop

    // 0x8016EE3C: bc1fl       L_8016EE50
    if (!c1cs) {
        // 0x8016EE40: lwc1        $f8, 0xC($a0)
        ctx->f8.u32l = MEM_W(ctx->r4, 0XC);
            goto L_8016EE50;
    }
    goto skip_5;
    // 0x8016EE40: lwc1        $f8, 0xC($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0XC);
    skip_5:
    // 0x8016EE44: b           L_8016EE70
    // 0x8016EE48: sh          $zero, 0x24($sp)
    MEM_H(0X24, ctx->r29) = 0;
        goto L_8016EE70;
    // 0x8016EE48: sh          $zero, 0x24($sp)
    MEM_H(0X24, ctx->r29) = 0;
    // 0x8016EE4C: lwc1        $f8, 0xC($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0XC);
L_8016EE50:
    // 0x8016EE50: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x8016EE54: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016EE58: nop

    // 0x8016EE5C: bc1f        L_8016EE6C
    if (!c1cs) {
        // 0x8016EE60: nop
    
            goto L_8016EE6C;
    }
    // 0x8016EE60: nop

    // 0x8016EE64: b           L_8016EE6C
    // 0x8016EE68: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016EE6C;
    // 0x8016EE68: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016EE6C:
    // 0x8016EE6C: sh          $v1, 0x24($sp)
    MEM_H(0X24, ctx->r29) = ctx->r3;
L_8016EE70:
    // 0x8016EE70: lwc1        $f4, 0x44($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016EE74: lwc1        $f8, 0x4($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016EE78: addiu       $a3, $sp, 0x2C
    ctx->r7 = ADD32(ctx->r29, 0X2C);
    // 0x8016EE7C: add.s       $f6, $f4, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f12.fl;
    // 0x8016EE80: lwc1        $f4, 0x44($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8016EE84: addiu       $t1, $sp, 0x24
    ctx->r9 = ADD32(ctx->r29, 0X24);
    // 0x8016EE88: c.lt.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl < ctx->f8.fl;
    // 0x8016EE8C: nop

    // 0x8016EE90: bc1fl       L_8016EEA4
    if (!c1cs) {
        // 0x8016EE94: lwc1        $f10, 0x10($a0)
        ctx->f10.u32l = MEM_W(ctx->r4, 0X10);
            goto L_8016EEA4;
    }
    goto skip_6;
    // 0x8016EE94: lwc1        $f10, 0x10($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X10);
    skip_6:
    // 0x8016EE98: b           L_8016EEC4
    // 0x8016EE9C: sh          $zero, 0x26($sp)
    MEM_H(0X26, ctx->r29) = 0;
        goto L_8016EEC4;
    // 0x8016EE9C: sh          $zero, 0x26($sp)
    MEM_H(0X26, ctx->r29) = 0;
    // 0x8016EEA0: lwc1        $f10, 0x10($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X10);
L_8016EEA4:
    // 0x8016EEA4: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x8016EEA8: c.lt.s      $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f10.fl < ctx->f4.fl;
    // 0x8016EEAC: nop

    // 0x8016EEB0: bc1f        L_8016EEC0
    if (!c1cs) {
        // 0x8016EEB4: nop
    
            goto L_8016EEC0;
    }
    // 0x8016EEB4: nop

    // 0x8016EEB8: b           L_8016EEC0
    // 0x8016EEBC: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016EEC0;
    // 0x8016EEBC: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016EEC0:
    // 0x8016EEC0: sh          $v1, 0x26($sp)
    MEM_H(0X26, ctx->r29) = ctx->r3;
L_8016EEC4:
    // 0x8016EEC4: lwc1        $f6, 0x48($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016EEC8: lwc1        $f8, 0x8($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016EECC: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8016EED0: c.lt.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl < ctx->f8.fl;
    // 0x8016EED4: nop

    // 0x8016EED8: bc1fl       L_8016EEEC
    if (!c1cs) {
        // 0x8016EEDC: lwc1        $f10, 0x14($a0)
        ctx->f10.u32l = MEM_W(ctx->r4, 0X14);
            goto L_8016EEEC;
    }
    goto skip_7;
    // 0x8016EEDC: lwc1        $f10, 0x14($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X14);
    skip_7:
    // 0x8016EEE0: b           L_8016EF0C
    // 0x8016EEE4: sh          $zero, 0x28($sp)
    MEM_H(0X28, ctx->r29) = 0;
        goto L_8016EF0C;
    // 0x8016EEE4: sh          $zero, 0x28($sp)
    MEM_H(0X28, ctx->r29) = 0;
    // 0x8016EEE8: lwc1        $f10, 0x14($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X14);
L_8016EEEC:
    // 0x8016EEEC: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x8016EEF0: c.lt.s      $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f10.fl < ctx->f4.fl;
    // 0x8016EEF4: nop

    // 0x8016EEF8: bc1f        L_8016EF08
    if (!c1cs) {
        // 0x8016EEFC: nop
    
            goto L_8016EF08;
    }
    // 0x8016EEFC: nop

    // 0x8016EF00: b           L_8016EF08
    // 0x8016EF04: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016EF08;
    // 0x8016EF04: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016EF08:
    // 0x8016EF08: sh          $v1, 0x28($sp)
    MEM_H(0X28, ctx->r29) = ctx->r3;
L_8016EF0C:
    // 0x8016EF0C: lh          $t9, 0x24($sp)
    ctx->r25 = MEM_H(ctx->r29, 0X24);
    // 0x8016EF10: lh          $t6, 0x26($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X26);
    // 0x8016EF14: addiu       $v1, $sp, 0x2A
    ctx->r3 = ADD32(ctx->r29, 0X2A);
    // 0x8016EF18: bne         $v0, $t9, L_8016EF38
    if (ctx->r2 != ctx->r25) {
        // 0x8016EF1C: nop
    
            goto L_8016EF38;
    }
    // 0x8016EF1C: nop

    // 0x8016EF20: bne         $v0, $t6, L_8016EF38
    if (ctx->r2 != ctx->r14) {
        // 0x8016EF24: lh          $t7, 0x28($sp)
        ctx->r15 = MEM_H(ctx->r29, 0X28);
            goto L_8016EF38;
    }
    // 0x8016EF24: lh          $t7, 0x28($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X28);
    // 0x8016EF28: bne         $v0, $t7, L_8016EF38
    if (ctx->r2 != ctx->r15) {
        // 0x8016EF2C: nop
    
            goto L_8016EF38;
    }
    // 0x8016EF2C: nop

    // 0x8016EF30: b           L_8016F134
    // 0x8016EF34: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016F134;
    // 0x8016EF34: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8016EF38:
    // 0x8016EF38: lh          $t0, 0x0($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X0);
    // 0x8016EF3C: lh          $t8, 0x0($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X0);
    // 0x8016EF40: addiu       $t1, $t1, 0x2
    ctx->r9 = ADD32(ctx->r9, 0X2);
    // 0x8016EF44: slti        $at, $t0, 0x2
    ctx->r1 = SIGNED(ctx->r8) < 0X2 ? 1 : 0;
    // 0x8016EF48: bnel        $t0, $t8, L_8016EF64
    if (ctx->r8 != ctx->r24) {
        // 0x8016EF4C: sltu        $at, $t1, $v1
        ctx->r1 = ctx->r9 < ctx->r3 ? 1 : 0;
            goto L_8016EF64;
    }
    goto skip_8;
    // 0x8016EF4C: sltu        $at, $t1, $v1
    ctx->r1 = ctx->r9 < ctx->r3 ? 1 : 0;
    skip_8:
    // 0x8016EF50: beql        $at, $zero, L_8016EF64
    if (ctx->r1 == 0) {
        // 0x8016EF54: sltu        $at, $t1, $v1
        ctx->r1 = ctx->r9 < ctx->r3 ? 1 : 0;
            goto L_8016EF64;
    }
    goto skip_9;
    // 0x8016EF54: sltu        $at, $t1, $v1
    ctx->r1 = ctx->r9 < ctx->r3 ? 1 : 0;
    skip_9:
    // 0x8016EF58: b           L_8016F134
    // 0x8016EF5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016F134;
    // 0x8016EF5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016EF60: sltu        $at, $t1, $v1
    ctx->r1 = ctx->r9 < ctx->r3 ? 1 : 0;
L_8016EF64:
    // 0x8016EF64: bne         $at, $zero, L_8016EF38
    if (ctx->r1 != 0) {
        // 0x8016EF68: addiu       $a3, $a3, 0x2
        ctx->r7 = ADD32(ctx->r7, 0X2);
            goto L_8016EF38;
    }
    // 0x8016EF68: addiu       $a3, $a3, 0x2
    ctx->r7 = ADD32(ctx->r7, 0X2);
    // 0x8016EF6C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016EF70: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x8016EF74: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x8016EF78: lwc1        $f16, -0x5110($at)
    ctx->f16.u32l = MEM_W(ctx->r1, -0X5110);
    // 0x8016EF7C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016EF80: addiu       $a3, $sp, 0x2C
    ctx->r7 = ADD32(ctx->r29, 0X2C);
    // 0x8016EF84: addiu       $s0, $sp, 0x40
    ctx->r16 = ADD32(ctx->r29, 0X40);
    // 0x8016EF88: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x8016EF8C: addiu       $t4, $zero, 0x3
    ctx->r12 = ADD32(0, 0X3);
L_8016EF90:
    // 0x8016EF90: lh          $t0, 0x0($a3)
    ctx->r8 = MEM_H(ctx->r7, 0X0);
    // 0x8016EF94: sll         $t1, $v1, 2
    ctx->r9 = S32(ctx->r3 << 2);
    // 0x8016EF98: addu        $t9, $a2, $t1
    ctx->r25 = ADD32(ctx->r6, ctx->r9);
    // 0x8016EF9C: beql        $v0, $t0, L_8016F128
    if (ctx->r2 == ctx->r8) {
        // 0x8016EFA0: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_10;
    // 0x8016EFA0: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_10:
    // 0x8016EFA4: lwc1        $f0, 0x0($t9)
    ctx->f0.u32l = MEM_W(ctx->r25, 0X0);
    // 0x8016EFA8: sll         $t6, $t0, 2
    ctx->r14 = S32(ctx->r8 << 2);
    // 0x8016EFAC: subu        $t6, $t6, $t0
    ctx->r14 = SUB32(ctx->r14, ctx->r8);
    // 0x8016EFB0: c.lt.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl < ctx->f14.fl;
    // 0x8016EFB4: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x8016EFB8: addu        $t7, $a0, $t6
    ctx->r15 = ADD32(ctx->r4, ctx->r14);
    // 0x8016EFBC: addu        $t8, $t7, $t1
    ctx->r24 = ADD32(ctx->r15, ctx->r9);
    // 0x8016EFC0: bc1f        L_8016EFD0
    if (!c1cs) {
        // 0x8016EFC4: addu        $t9, $a1, $t1
        ctx->r25 = ADD32(ctx->r5, ctx->r9);
            goto L_8016EFD0;
    }
    // 0x8016EFC4: addu        $t9, $a1, $t1
    ctx->r25 = ADD32(ctx->r5, ctx->r9);
    // 0x8016EFC8: b           L_8016EFD4
    // 0x8016EFCC: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_8016EFD4;
    // 0x8016EFCC: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_8016EFD0:
    // 0x8016EFD0: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_8016EFD4:
    // 0x8016EFD4: c.lt.s      $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f2.fl < ctx->f16.fl;
    // 0x8016EFD8: nop

    // 0x8016EFDC: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016EFE0: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_11;
    // 0x8016EFE0: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_11:
    // 0x8016EFE4: lwc1        $f6, 0x0($t8)
    ctx->f6.u32l = MEM_W(ctx->r24, 0X0);
    // 0x8016EFE8: lwc1        $f8, 0x0($t9)
    ctx->f8.u32l = MEM_W(ctx->r25, 0X0);
    // 0x8016EFEC: addiu       $t1, $sp, 0x34
    ctx->r9 = ADD32(ctx->r29, 0X34);
    // 0x8016EFF0: or          $t2, $a2, $zero
    ctx->r10 = ctx->r6 | 0;
    // 0x8016EFF4: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x8016EFF8: or          $t3, $a1, $zero
    ctx->r11 = ctx->r5 | 0;
    // 0x8016EFFC: beq         $v1, $t5, L_8016F02C
    if (ctx->r3 == ctx->r13) {
        // 0x8016F000: div.s       $f2, $f10, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
            goto L_8016F02C;
    }
    // 0x8016F000: div.s       $f2, $f10, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
    // 0x8016F004: c.lt.s      $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f2.fl < ctx->f18.fl;
    // 0x8016F008: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016F00C: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F010: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_12;
    // 0x8016F010: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_12:
    // 0x8016F014: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8016F018: nop

    // 0x8016F01C: c.lt.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl < ctx->f2.fl;
    // 0x8016F020: nop

    // 0x8016F024: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F028: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_13;
    // 0x8016F028: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_13:
L_8016F02C:
    // 0x8016F02C: lwc1        $f6, 0x0($t2)
    ctx->f6.u32l = MEM_W(ctx->r10, 0X0);
    // 0x8016F030: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016F034: lwc1        $f8, 0x0($t3)
    ctx->f8.u32l = MEM_W(ctx->r11, 0X0);
    // 0x8016F038: mul.s       $f10, $f6, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016F03C: beql        $t1, $s0, L_8016F070
    if (ctx->r9 == ctx->r16) {
        // 0x8016F040: add.s       $f6, $f10, $f8
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
            goto L_8016F070;
    }
    goto skip_14;
    // 0x8016F040: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    skip_14:
    // 0x8016F044: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
L_8016F048:
    // 0x8016F048: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x8016F04C: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016F050: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016F054: swc1        $f6, -0x8($t1)
    MEM_W(-0X8, ctx->r9) = ctx->f6.u32l;
    // 0x8016F058: lwc1        $f6, 0x0($t2)
    ctx->f6.u32l = MEM_W(ctx->r10, 0X0);
    // 0x8016F05C: lwc1        $f8, 0x0($t3)
    ctx->f8.u32l = MEM_W(ctx->r11, 0X0);
    // 0x8016F060: mul.s       $f10, $f6, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8016F064: bnel        $t1, $s0, L_8016F048
    if (ctx->r9 != ctx->r16) {
        // 0x8016F068: add.s       $f6, $f10, $f8
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
            goto L_8016F048;
    }
    goto skip_15;
    // 0x8016F068: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    skip_15:
    // 0x8016F06C: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
L_8016F070:
    // 0x8016F070: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x8016F074: addiu       $t3, $t3, 0x4
    ctx->r11 = ADD32(ctx->r11, 0X4);
    // 0x8016F078: swc1        $f6, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->f6.u32l;
    // 0x8016F07C: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    // 0x8016F080: lwc1        $f6, 0x34($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X34);
    // 0x8016F084: sub.s       $f10, $f8, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f16.fl;
    // 0x8016F088: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x8016F08C: nop

    // 0x8016F090: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F094: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_16;
    // 0x8016F094: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_16:
    // 0x8016F098: lwc1        $f4, 0xC($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8016F09C: lwc1        $f10, 0x38($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8016F0A0: add.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x8016F0A4: c.lt.s      $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f8.fl < ctx->f6.fl;
    // 0x8016F0A8: nop

    // 0x8016F0AC: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F0B0: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_17;
    // 0x8016F0B0: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_17:
    // 0x8016F0B4: lwc1        $f8, 0x4($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8016F0B8: add.s       $f4, $f10, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f12.fl;
    // 0x8016F0BC: sub.s       $f6, $f8, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f16.fl;
    // 0x8016F0C0: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016F0C4: nop

    // 0x8016F0C8: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F0CC: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_18;
    // 0x8016F0CC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_18:
    // 0x8016F0D0: lwc1        $f8, 0x10($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X10);
    // 0x8016F0D4: add.s       $f4, $f8, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f16.fl;
    // 0x8016F0D8: c.lt.s      $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f4.fl < ctx->f10.fl;
    // 0x8016F0DC: nop

    // 0x8016F0E0: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F0E4: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_19;
    // 0x8016F0E4: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_19:
    // 0x8016F0E8: lwc1        $f8, 0x8($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016F0EC: lwc1        $f6, 0x3C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x8016F0F0: sub.s       $f4, $f8, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f16.fl;
    // 0x8016F0F4: c.lt.s      $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f6.fl < ctx->f4.fl;
    // 0x8016F0F8: nop

    // 0x8016F0FC: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F100: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_20;
    // 0x8016F100: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_20:
    // 0x8016F104: lwc1        $f10, 0x14($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X14);
    // 0x8016F108: add.s       $f8, $f10, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x8016F10C: c.lt.s      $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f8.fl < ctx->f6.fl;
    // 0x8016F110: nop

    // 0x8016F114: bc1tl       L_8016F128
    if (c1cs) {
        // 0x8016F118: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_8016F128;
    }
    goto skip_21;
    // 0x8016F118: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    skip_21:
    // 0x8016F11C: b           L_8016F134
    // 0x8016F120: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8016F134;
    // 0x8016F120: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016F124: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
L_8016F128:
    // 0x8016F128: bne         $v1, $t4, L_8016EF90
    if (ctx->r3 != ctx->r12) {
        // 0x8016F12C: addiu       $a3, $a3, 0x2
        ctx->r7 = ADD32(ctx->r7, 0X2);
            goto L_8016EF90;
    }
    // 0x8016F12C: addiu       $a3, $a3, 0x2
    ctx->r7 = ADD32(ctx->r7, 0X2);
    // 0x8016F130: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016F134:
    // 0x8016F134: lw          $s0, 0x4($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X4);
    // 0x8016F138: jr          $ra
    // 0x8016F13C: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x8016F13C: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_8016F140(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016F140: lw          $t6, 0x10($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X10);
    // 0x8016F144: sll         $v0, $a0, 2
    ctx->r2 = S32(ctx->r4 << 2);
    // 0x8016F148: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8016F14C: addu        $t7, $t6, $v0
    ctx->r15 = ADD32(ctx->r14, ctx->r2);
    // 0x8016F150: lwc1        $f0, 0x0($t7)
    ctx->f0.u32l = MEM_W(ctx->r15, 0X0);
    // 0x8016F154: sll         $t8, $a1, 2
    ctx->r24 = S32(ctx->r5 << 2);
    // 0x8016F158: subu        $t8, $t8, $a1
    ctx->r24 = SUB32(ctx->r24, ctx->r5);
    // 0x8016F15C: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x8016F160: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x8016F164: addu        $t9, $a2, $t8
    ctx->r25 = ADD32(ctx->r6, ctx->r24);
    // 0x8016F168: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016F16C: bc1f        L_8016F17C
    if (!c1cs) {
        // 0x8016F170: addu        $t1, $t9, $v0
        ctx->r9 = ADD32(ctx->r25, ctx->r2);
            goto L_8016F17C;
    }
    // 0x8016F170: addu        $t1, $t9, $v0
    ctx->r9 = ADD32(ctx->r25, ctx->r2);
    // 0x8016F174: b           L_8016F180
    // 0x8016F178: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_8016F180;
    // 0x8016F178: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_8016F17C:
    // 0x8016F17C: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_8016F180:
    // 0x8016F180: lwc1        $f6, -0x510C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X510C);
    // 0x8016F184: addu        $t2, $a3, $v0
    ctx->r10 = ADD32(ctx->r7, ctx->r2);
    // 0x8016F188: c.lt.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl < ctx->f6.fl;
    // 0x8016F18C: nop

    // 0x8016F190: bc1fl       L_8016F1A4
    if (!c1cs) {
        // 0x8016F194: lwc1        $f8, 0x0($t1)
        ctx->f8.u32l = MEM_W(ctx->r9, 0X0);
            goto L_8016F1A4;
    }
    goto skip_0;
    // 0x8016F194: lwc1        $f8, 0x0($t1)
    ctx->f8.u32l = MEM_W(ctx->r9, 0X0);
    skip_0:
    // 0x8016F198: jr          $ra
    // 0x8016F19C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    return;
    // 0x8016F19C: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x8016F1A0: lwc1        $f8, 0x0($t1)
    ctx->f8.u32l = MEM_W(ctx->r9, 0X0);
L_8016F1A4:
    // 0x8016F1A4: lwc1        $f10, 0x0($t2)
    ctx->f10.u32l = MEM_W(ctx->r10, 0X0);
    // 0x8016F1A8: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x8016F1AC: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x8016F1B0: sub.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x8016F1B4: lw          $a1, 0x10($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X10);
    // 0x8016F1B8: or          $a0, $v1, $zero
    ctx->r4 = ctx->r3 | 0;
    // 0x8016F1BC: beq         $v0, $at, L_8016F1FC
    if (ctx->r2 == ctx->r1) {
        // 0x8016F1C0: div.s       $f2, $f16, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
            goto L_8016F1FC;
    }
    // 0x8016F1C0: div.s       $f2, $f16, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016F1C4: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x8016F1C8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8016F1CC: c.lt.s      $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f2.fl < ctx->f18.fl;
    // 0x8016F1D0: nop

    // 0x8016F1D4: bc1t        L_8016F1F4
    if (c1cs) {
        // 0x8016F1D8: nop
    
            goto L_8016F1F4;
    }
    // 0x8016F1D8: nop

    // 0x8016F1DC: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8016F1E0: nop

    // 0x8016F1E4: c.lt.s      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.fl < ctx->f2.fl;
    // 0x8016F1E8: nop

    // 0x8016F1EC: bc1fl       L_8016F200
    if (!c1cs) {
        // 0x8016F1F0: or          $t0, $a3, $zero
        ctx->r8 = ctx->r7 | 0;
            goto L_8016F200;
    }
    goto skip_1;
    // 0x8016F1F0: or          $t0, $a3, $zero
    ctx->r8 = ctx->r7 | 0;
    skip_1:
L_8016F1F4:
    // 0x8016F1F4: jr          $ra
    // 0x8016F1F8: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    return;
    // 0x8016F1F8: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
L_8016F1FC:
    // 0x8016F1FC: or          $t0, $a3, $zero
    ctx->r8 = ctx->r7 | 0;
L_8016F200:
    // 0x8016F200: addiu       $a3, $zero, 0x3
    ctx->r7 = ADD32(0, 0X3);
    // 0x8016F204: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016F208: lwc1        $f14, 0x0($a1)
    ctx->f14.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016F20C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x8016F210: lwc1        $f16, 0x0($t0)
    ctx->f16.u32l = MEM_W(ctx->r8, 0X0);
    // 0x8016F214: mul.s       $f18, $f14, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x8016F218: beql        $v0, $a3, L_8016F250
    if (ctx->r2 == ctx->r7) {
        // 0x8016F21C: add.s       $f14, $f18, $f16
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_8016F250;
    }
    goto skip_2;
    // 0x8016F21C: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
    skip_2:
    // 0x8016F220: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
L_8016F224:
    // 0x8016F224: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x8016F228: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x8016F22C: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016F230: swc1        $f14, -0x4($a0)
    MEM_W(-0X4, ctx->r4) = ctx->f14.u32l;
    // 0x8016F234: lwc1        $f14, 0x0($a1)
    ctx->f14.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016F238: lwc1        $f16, 0x4($t0)
    ctx->f16.u32l = MEM_W(ctx->r8, 0X4);
    // 0x8016F23C: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016F240: mul.s       $f18, $f14, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x8016F244: bnel        $v0, $a3, L_8016F224
    if (ctx->r2 != ctx->r7) {
        // 0x8016F248: add.s       $f14, $f18, $f16
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
            goto L_8016F224;
    }
    goto skip_3;
    // 0x8016F248: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
    skip_3:
    // 0x8016F24C: add.s       $f14, $f18, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f18.fl + ctx->f16.fl;
L_8016F250:
    // 0x8016F250: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x8016F254: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8016F258: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x8016F25C: swc1        $f14, -0x4($a0)
    MEM_W(-0X4, ctx->r4) = ctx->f14.u32l;
    // 0x8016F260: lwc1        $f0, 0x0($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016F264: lwc1        $f18, 0x0($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X0);
    // 0x8016F268: c.lt.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl < ctx->f18.fl;
    // 0x8016F26C: nop

    // 0x8016F270: bc1t        L_8016F2F0
    if (c1cs) {
        // 0x8016F274: nop
    
            goto L_8016F2F0;
    }
    // 0x8016F274: nop

    // 0x8016F278: lwc1        $f4, 0xC($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0XC);
    // 0x8016F27C: lwc1        $f6, 0x14($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X14);
    // 0x8016F280: c.lt.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl < ctx->f0.fl;
    // 0x8016F284: nop

    // 0x8016F288: bc1t        L_8016F2F0
    if (c1cs) {
        // 0x8016F28C: nop
    
            goto L_8016F2F0;
    }
    // 0x8016F28C: nop

    // 0x8016F290: lwc1        $f0, 0x4($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8016F294: lwc1        $f10, 0x4($a2)
    ctx->f10.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8016F298: add.s       $f8, $f0, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f0.fl + ctx->f6.fl;
    // 0x8016F29C: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016F2A0: nop

    // 0x8016F2A4: bc1t        L_8016F2F0
    if (c1cs) {
        // 0x8016F2A8: nop
    
            goto L_8016F2F0;
    }
    // 0x8016F2A8: nop

    // 0x8016F2AC: lwc1        $f16, 0x10($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X10);
    // 0x8016F2B0: c.lt.s      $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f16.fl < ctx->f0.fl;
    // 0x8016F2B4: nop

    // 0x8016F2B8: bc1t        L_8016F2F0
    if (c1cs) {
        // 0x8016F2BC: nop
    
            goto L_8016F2F0;
    }
    // 0x8016F2BC: nop

    // 0x8016F2C0: lwc1        $f0, 0x8($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X8);
    // 0x8016F2C4: lwc1        $f18, 0x8($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X8);
    // 0x8016F2C8: c.lt.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl < ctx->f18.fl;
    // 0x8016F2CC: nop

    // 0x8016F2D0: bc1t        L_8016F2F0
    if (c1cs) {
        // 0x8016F2D4: nop
    
            goto L_8016F2F0;
    }
    // 0x8016F2D4: nop

    // 0x8016F2D8: lwc1        $f4, 0x14($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X14);
    // 0x8016F2DC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016F2E0: c.lt.s      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.fl < ctx->f0.fl;
    // 0x8016F2E4: nop

    // 0x8016F2E8: bc1f        L_8016F2F8
    if (!c1cs) {
        // 0x8016F2EC: nop
    
            goto L_8016F2F8;
    }
    // 0x8016F2EC: nop

L_8016F2F0:
    // 0x8016F2F0: b           L_8016F2F8
    // 0x8016F2F4: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_8016F2F8;
    // 0x8016F2F4: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8016F2F8:
    // 0x8016F2F8: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x8016F2FC: jr          $ra
    // 0x8016F300: nop

    return;
    // 0x8016F300: nop

;}
RECOMP_FUNC void func_8016F310(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016F310: addiu       $sp, $sp, -0x148
    ctx->r29 = ADD32(ctx->r29, -0X148);
    // 0x8016F314: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8016F318: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016F31C: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x8016F320: lw          $s2, -0x1134($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X1134);
    // 0x8016F324: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016F328: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8016F32C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8016F330: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8016F334: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8016F338: sw          $a1, 0x14C($sp)
    MEM_W(0X14C, ctx->r29) = ctx->r5;
    // 0x8016F33C: sw          $a2, 0x150($sp)
    MEM_W(0X150, ctx->r29) = ctx->r6;
    // 0x8016F340: sw          $a3, 0x154($sp)
    MEM_W(0X154, ctx->r29) = ctx->r7;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_textpipe2_probe(rdram, (uint32_t)ctx->r18);
#endif
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: workspace guard (LOD_FIX_TEXT_MEASURE_GUARD, round 20) ---
    // Mirrors func_801682D0's round-15 workspace guard (funcs_57.c): if $s2
    // (the struct read from global 0x8019EECC above) is invalid, skip the
    // entire s2-dependent span below -- the func_80172078/func_801720EC
    // calls, the three fixed func_8016FBB0 calls, and the func_8016FBB0
    // array loop -- by jumping straight to L_8016F478, the same safe
    // landing spot this function's own count<=0 array-loop skip already
    // uses (it immediately overwrites $s2 with an unrelated global's list
    // head for the function's second, independent half). All prologue
    // register saves (ra/s4/s3/s1, plus this function's own a1/a2/a3
    // spills, and the caller's s0/s2 saved even earlier above) have already
    // completed above this point, so the epilogue's restores at L_8016F6FC
    // stay intact. $v0 -- the value func_8003F72C (the only caller) branches
    // on and dereferences -- is produced entirely by the second half after
    // L_8016F478, independent of $s2, so it is unaffected by this early
    // skip. See docs/issue27-31-ni0e-findings.md Round 20 for the caller
    // audit.
    if (lod_text_guard_workspace_invalid(rdram, (uint32_t)ctx->r18)) {
        if (!lod_text_guard_workspace_8016F310_logged) {
            lod_text_guard_workspace_8016F310_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016F310 rejected invalid workspace s2=0x%08X "
                    "(stale text-struct pointer), skipping text draw\n",
                    (uint32_t)ctx->r18);
        }
        goto L_8016F478;
    }
    // --- END PATCH ---
#endif
    // 0x8016F344: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016F348: addiu       $a0, $a0, -0x10F0
    ctx->r4 = ADD32(ctx->r4, -0X10F0);
    // 0x8016F34C: jal         0x80172078
    // 0x8016F350: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    func_80172078(rdram, ctx);
        goto after_0;
    // 0x8016F350: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    after_0:
    // 0x8016F354: lwc1        $f4, 0x154($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X154);
    // 0x8016F358: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x8016F35C: addiu       $v0, $sp, 0x134
    ctx->r2 = ADD32(ctx->r29, 0X134);
    // 0x8016F360: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x8016F364: c.eq.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl == ctx->f6.fl;
    // 0x8016F368: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x8016F36C: bc1fl       L_8016F380
    if (!c1cs) {
        // 0x8016F370: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_8016F380;
    }
    goto skip_0;
    // 0x8016F370: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_0:
    // 0x8016F374: b           L_8016F6FC
    // 0x8016F378: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016F6FC;
    // 0x8016F378: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8016F37C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
L_8016F380:
    // 0x8016F380: addiu       $a0, $sp, 0x140
    ctx->r4 = ADD32(ctx->r29, 0X140);
    // 0x8016F384: lwc1        $f16, 0x0($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016F388: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016F38C: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016F390: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016F394: beql        $at, $zero, L_8016F3BC
    if (ctx->r1 == 0) {
        // 0x8016F398: swc1        $f18, -0x4($v0)
        MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
            goto L_8016F3BC;
    }
    goto skip_1;
    // 0x8016F398: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    skip_1:
L_8016F39C:
    // 0x8016F39C: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016F3A0: lwc1        $f16, 0x4($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8016F3A4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016F3A8: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016F3AC: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016F3B0: bne         $at, $zero, L_8016F39C
    if (ctx->r1 != 0) {
        // 0x8016F3B4: addiu       $v1, $v1, 0x4
        ctx->r3 = ADD32(ctx->r3, 0X4);
            goto L_8016F39C;
    }
    // 0x8016F3B4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016F3B8: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
L_8016F3BC:
    // 0x8016F3BC: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016F3C0: lwc1        $f4, 0x14C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X14C);
    // 0x8016F3C4: lwc1        $f6, 0x150($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X150);
    // 0x8016F3C8: lwc1        $f8, 0x154($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X154);
    // 0x8016F3CC: mul.s       $f2, $f4, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8016F3D0: nop

    // 0x8016F3D4: mul.s       $f12, $f6, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8016F3D8: mov.s       $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    ctx->f16.fl = ctx->f2.fl;
    // 0x8016F3DC: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x8016F3E0: c.lt.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl < ctx->f12.fl;
    // 0x8016F3E4: mov.s       $f18, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    ctx->f18.fl = ctx->f12.fl;
    // 0x8016F3E8: bc1f        L_8016F3F8
    if (!c1cs) {
        // 0x8016F3EC: nop
    
            goto L_8016F3F8;
    }
    // 0x8016F3EC: nop

    // 0x8016F3F0: mov.s       $f16, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    ctx->f16.fl = ctx->f12.fl;
    // 0x8016F3F4: mov.s       $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    ctx->f18.fl = ctx->f2.fl;
L_8016F3F8:
    // 0x8016F3F8: mfc1        $a1, $f16
    ctx->r5 = (int32_t)ctx->f16.u32l;
    // 0x8016F3FC: mfc1        $a2, $f18
    ctx->r6 = (int32_t)ctx->f18.u32l;
    // 0x8016F400: mfc1        $a3, $f10
    ctx->r7 = (int32_t)ctx->f10.u32l;
    // 0x8016F404: jal         0x801720EC
    // 0x8016F408: addiu       $a0, $sp, 0x134
    ctx->r4 = ADD32(ctx->r29, 0X134);
    func_801720EC(rdram, ctx);
        goto after_1;
    // 0x8016F408: addiu       $a0, $sp, 0x134
    ctx->r4 = ADD32(ctx->r29, 0X134);
    after_1:
    // 0x8016F40C: lw          $a0, 0x10($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X10);
    // 0x8016F410: jal         0x8016FBB0
    // 0x8016F414: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    func_8016FBB0(rdram, ctx);
        goto after_2;
    // 0x8016F414: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    after_2:
    // 0x8016F418: lw          $a0, 0x14($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X14);
    // 0x8016F41C: jal         0x8016FBB0
    // 0x8016F420: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    func_8016FBB0(rdram, ctx);
        goto after_3;
    // 0x8016F420: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    after_3:
    // 0x8016F424: lw          $a0, 0x18($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X18);
    // 0x8016F428: jal         0x8016FBB0
    // 0x8016F42C: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    func_8016FBB0(rdram, ctx);
        goto after_4;
    // 0x8016F42C: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    after_4:
    // 0x8016F430: lw          $t6, 0x20($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X20);
    // 0x8016F434: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x8016F438: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: array-loop guard (LOD_FIX_TEXT_MEASURE_GUARD, round 20) ---
    // Mirrors func_801682D0's round-18 array-loop guard (funcs_57.c): only
    // checked when the loop is actually about to run (count>0, matching the
    // blez below); $s1/$s0 are already 0 (just set above), so jumping to
    // the existing L_8016F478 skip target needs no further register fixup.
    if (SIGNED(ctx->r14) > 0 && lod_text_guard_array_invalid(rdram, (uint32_t)ctx->r18)) {
        if (!lod_text_guard_array_8016F310_logged) {
            lod_text_guard_array_8016F310_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016F310 rejected invalid array bases "
                    "obj_base=0x%08X str_base=0x%08X count=%d (stale text-struct array), "
                    "skipping array loop\n",
                    (uint32_t)MEM_W(ctx->r18, 0X24), (uint32_t)MEM_W(ctx->r18, 0X38),
                    (int32_t)ctx->r14);
        }
        goto L_8016F478;
    }
    // --- END PATCH ---
#endif
    // 0x8016F43C: blez        $t6, L_8016F478
    if (SIGNED(ctx->r14) <= 0) {
        // 0x8016F440: nop
    
            goto L_8016F478;
    }
    // 0x8016F440: nop

    // 0x8016F444: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
L_8016F448:
    // 0x8016F448: lw          $t9, 0x38($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X38);
    // 0x8016F44C: addu        $t8, $t7, $s0
    ctx->r24 = ADD32(ctx->r15, ctx->r16);
    // 0x8016F450: addu        $t0, $t9, $s0
    ctx->r8 = ADD32(ctx->r25, ctx->r16);
    // 0x8016F454: lw          $a1, 0x0($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X0);
    // 0x8016F458: jal         0x8016FBB0
    // 0x8016F45C: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
    func_8016FBB0(rdram, ctx);
        goto after_5;
    // 0x8016F45C: lw          $a0, 0x0($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X0);
    after_5:
    // 0x8016F460: lw          $t1, 0x20($s2)
    ctx->r9 = MEM_W(ctx->r18, 0X20);
    // 0x8016F464: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x8016F468: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x8016F46C: slt         $at, $s1, $t1
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x8016F470: bnel        $at, $zero, L_8016F448
    if (ctx->r1 != 0) {
        // 0x8016F474: lw          $t7, 0x24($s2)
        ctx->r15 = MEM_W(ctx->r18, 0X24);
            goto L_8016F448;
    }
    goto skip_2;
    // 0x8016F474: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
    skip_2:
L_8016F478:
    // 0x8016F478: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x8016F47C: lw          $s2, -0x1A20($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X1A20);
    // 0x8016F480: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016F484: addiu       $s4, $s4, -0x9FC
    ctx->r20 = ADD32(ctx->r20, -0X9FC);
    // 0x8016F488: beq         $s2, $zero, L_8016F530
    if (ctx->r18 == 0) {
        // 0x8016F48C: or          $s3, $zero, $zero
        ctx->r19 = 0 | 0;
            goto L_8016F530;
    }
    // 0x8016F48C: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x8016F490: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
L_8016F494:
    // 0x8016F494: beql        $v0, $zero, L_8016F524
    if (ctx->r2 == 0) {
        // 0x8016F498: lw          $s2, 0x0($s2)
        ctx->r18 = MEM_W(ctx->r18, 0X0);
            goto L_8016F524;
    }
    goto skip_3;
    // 0x8016F498: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    skip_3:
    // 0x8016F49C: lw          $s0, 0x24($v0)
    ctx->r16 = MEM_W(ctx->r2, 0X24);
    // 0x8016F4A0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8016F4A4: beq         $s0, $zero, L_8016F520
    if (ctx->r16 == 0) {
        // 0x8016F4A8: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016F520;
    }
    // 0x8016F4A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016F4AC: lw          $t2, 0x0($s4)
    ctx->r10 = MEM_W(ctx->r20, 0X0);
    // 0x8016F4B0: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
    // 0x8016F4B4: sw          $s3, 0x10($t2)
    MEM_W(0X10, ctx->r10) = ctx->r19;
    // 0x8016F4B8: lw          $t3, 0x0($s4)
    ctx->r11 = MEM_W(ctx->r20, 0X0);
    // 0x8016F4BC: sw          $v0, 0xC($t3)
    MEM_W(0XC, ctx->r11) = ctx->r2;
    // 0x8016F4C0: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
    // 0x8016F4C4: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x8016F4C8: slti        $at, $v1, 0x282
    ctx->r1 = SIGNED(ctx->r3) < 0X282 ? 1 : 0;
    // 0x8016F4CC: bne         $at, $zero, L_8016F4E4
    if (ctx->r1 != 0) {
        // 0x8016F4D0: slti        $at, $v1, 0x285
        ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
            goto L_8016F4E4;
    }
    // 0x8016F4D0: slti        $at, $v1, 0x285
    ctx->r1 = SIGNED(ctx->r3) < 0X285 ? 1 : 0;
    // 0x8016F4D4: beq         $at, $zero, L_8016F4E4
    if (ctx->r1 == 0) {
        // 0x8016F4D8: nop
    
            goto L_8016F4E4;
    }
    // 0x8016F4D8: nop

    // 0x8016F4DC: b           L_8016F4E4
    // 0x8016F4E0: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_8016F4E4;
    // 0x8016F4E0: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_8016F4E4:
    // 0x8016F4E4: jal         0x8016F80C
    // 0x8016F4E8: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016F80C(rdram, ctx);
        goto after_6;
    // 0x8016F4E8: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_6:
    // 0x8016F4EC: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016F4F0: beql        $a0, $zero, L_8016F524
    if (ctx->r4 == 0) {
        // 0x8016F4F4: lw          $s2, 0x0($s2)
        ctx->r18 = MEM_W(ctx->r18, 0X0);
            goto L_8016F524;
    }
    goto skip_4;
    // 0x8016F4F4: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    skip_4:
    // 0x8016F4F8: lhu         $t4, 0x2($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0X2);
    // 0x8016F4FC: andi        $t5, $t4, 0x8000
    ctx->r13 = ctx->r12 & 0X8000;
    // 0x8016F500: beq         $t5, $zero, L_8016F518
    if (ctx->r13 == 0) {
        // 0x8016F504: nop
    
            goto L_8016F518;
    }
    // 0x8016F504: nop

    // 0x8016F508: jal         0x8016F71C
    // 0x8016F50C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016F71C(rdram, ctx);
        goto after_7;
    // 0x8016F50C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_7:
    // 0x8016F510: b           L_8016F524
    // 0x8016F514: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
        goto L_8016F524;
    // 0x8016F514: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
L_8016F518:
    // 0x8016F518: jal         0x8016F794
    // 0x8016F51C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016F794(rdram, ctx);
        goto after_8;
    // 0x8016F51C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_8:
L_8016F520:
    // 0x8016F520: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
L_8016F524:
    // 0x8016F524: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x8016F528: bnel        $s2, $zero, L_8016F494
    if (ctx->r18 != 0) {
        // 0x8016F52C: lw          $v0, 0x4($s2)
        ctx->r2 = MEM_W(ctx->r18, 0X4);
            goto L_8016F494;
    }
    goto skip_5;
    // 0x8016F52C: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
    skip_5:
L_8016F530:
    // 0x8016F530: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8016F534: addiu       $s4, $s4, -0x9FC
    ctx->r20 = ADD32(ctx->r20, -0X9FC);
    // 0x8016F538: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016F53C: lwc1        $f0, -0x5100($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X5100);
    // 0x8016F540: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016F544: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8016F548: addiu       $a3, $zero, 0xC8
    ctx->r7 = ADD32(0, 0XC8);
    // 0x8016F54C: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    // 0x8016F550: addu        $v0, $s0, $a2
    ctx->r2 = ADD32(ctx->r16, ctx->r6);
L_8016F554:
    // 0x8016F554: addiu       $a1, $v0, 0x230
    ctx->r5 = ADD32(ctx->r2, 0X230);
    // 0x8016F558: lh          $t6, 0x60($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X60);
    // 0x8016F55C: beql        $t6, $zero, L_8016F5B8
    if (ctx->r14 == 0) {
        // 0x8016F560: addiu       $a1, $v0, 0x48C
        ctx->r5 = ADD32(ctx->r2, 0X48C);
            goto L_8016F5B8;
    }
    goto skip_6;
    // 0x8016F560: addiu       $a1, $v0, 0x48C
    ctx->r5 = ADD32(ctx->r2, 0X48C);
    skip_6:
    // 0x8016F564: lwc1        $f4, 0x18($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X18);
    // 0x8016F568: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016F56C: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x8016F570: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8016F574: swc1        $f6, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->f6.u32l;
    // 0x8016F578: lwc1        $f16, 0x0($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016F57C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016F580: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016F584: beql        $v1, $a0, L_8016F5A8
    if (ctx->r3 == ctx->r4) {
        // 0x8016F588: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_8016F5A8;
    }
    goto skip_7;
    // 0x8016F588: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    skip_7:
L_8016F58C:
    // 0x8016F58C: lwc1        $f16, 0x4($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X4);
    // 0x8016F590: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016F594: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x8016F598: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016F59C: bne         $v1, $a0, L_8016F58C
    if (ctx->r3 != ctx->r4) {
        // 0x8016F5A0: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_8016F58C;
    }
    // 0x8016F5A0: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016F5A4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
L_8016F5A8:
    // 0x8016F5A8: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016F5AC: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016F5B0: addu        $v0, $s0, $a2
    ctx->r2 = ADD32(ctx->r16, ctx->r6);
    // 0x8016F5B4: addiu       $a1, $v0, 0x48C
    ctx->r5 = ADD32(ctx->r2, 0X48C);
L_8016F5B8:
    // 0x8016F5B8: lh          $t7, 0x60($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X60);
    // 0x8016F5BC: beql        $t7, $zero, L_8016F614
    if (ctx->r15 == 0) {
        // 0x8016F5C0: addiu       $a2, $a2, 0x64
        ctx->r6 = ADD32(ctx->r6, 0X64);
            goto L_8016F614;
    }
    goto skip_8;
    // 0x8016F5C0: addiu       $a2, $a2, 0x64
    ctx->r6 = ADD32(ctx->r6, 0X64);
    skip_8:
    // 0x8016F5C4: lwc1        $f4, 0x18($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X18);
    // 0x8016F5C8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016F5CC: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x8016F5D0: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8016F5D4: swc1        $f6, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->f6.u32l;
    // 0x8016F5D8: lwc1        $f16, 0x0($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016F5DC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016F5E0: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016F5E4: beql        $v1, $a0, L_8016F608
    if (ctx->r3 == ctx->r4) {
        // 0x8016F5E8: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_8016F608;
    }
    goto skip_9;
    // 0x8016F5E8: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    skip_9:
L_8016F5EC:
    // 0x8016F5EC: lwc1        $f16, 0x4($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X4);
    // 0x8016F5F0: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016F5F4: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x8016F5F8: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x8016F5FC: bne         $v1, $a0, L_8016F5EC
    if (ctx->r3 != ctx->r4) {
        // 0x8016F600: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_8016F5EC;
    }
    // 0x8016F600: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8016F604: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
L_8016F608:
    // 0x8016F608: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8016F60C: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016F610: addiu       $a2, $a2, 0x64
    ctx->r6 = ADD32(ctx->r6, 0X64);
L_8016F614:
    // 0x8016F614: bnel        $a2, $a3, L_8016F554
    if (ctx->r6 != ctx->r7) {
        // 0x8016F618: addu        $v0, $s0, $a2
        ctx->r2 = ADD32(ctx->r16, ctx->r6);
            goto L_8016F554;
    }
    goto skip_10;
    // 0x8016F618: addu        $v0, $s0, $a2
    ctx->r2 = ADD32(ctx->r16, ctx->r6);
    skip_10:
    // 0x8016F61C: lh          $t8, 0x290($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X290);
    // 0x8016F620: beql        $t8, $zero, L_8016F68C
    if (ctx->r24 == 0) {
        // 0x8016F624: lh          $t0, 0x4EC($s0)
        ctx->r8 = MEM_H(ctx->r16, 0X4EC);
            goto L_8016F68C;
    }
    goto skip_11;
    // 0x8016F624: lh          $t0, 0x4EC($s0)
    ctx->r8 = MEM_H(ctx->r16, 0X4EC);
    skip_11:
    // 0x8016F628: lh          $t9, 0x2F4($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X2F4);
    // 0x8016F62C: beql        $t9, $zero, L_8016F68C
    if (ctx->r25 == 0) {
        // 0x8016F630: lh          $t0, 0x4EC($s0)
        ctx->r8 = MEM_H(ctx->r16, 0X4EC);
            goto L_8016F68C;
    }
    goto skip_12;
    // 0x8016F630: lh          $t0, 0x4EC($s0)
    ctx->r8 = MEM_H(ctx->r16, 0X4EC);
    skip_12:
    // 0x8016F634: lwc1        $f4, 0x2AC($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X2AC);
    // 0x8016F638: lwc1        $f6, 0x248($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X248);
    // 0x8016F63C: addiu       $s1, $sp, 0xA0
    ctx->r17 = ADD32(ctx->r29, 0XA0);
    // 0x8016F640: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x8016F644: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016F648: addiu       $a0, $s0, 0x230
    ctx->r4 = ADD32(ctx->r16, 0X230);
    // 0x8016F64C: bc1fl       L_8016F68C
    if (!c1cs) {
        // 0x8016F650: lh          $t0, 0x4EC($s0)
        ctx->r8 = MEM_H(ctx->r16, 0X4EC);
            goto L_8016F68C;
    }
    goto skip_13;
    // 0x8016F650: lh          $t0, 0x4EC($s0)
    ctx->r8 = MEM_H(ctx->r16, 0X4EC);
    skip_13:
    // 0x8016F654: jal         0x80001090
    // 0x8016F658: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    memory_copy(rdram, ctx);
        goto after_9;
    // 0x8016F658: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    after_9:
    // 0x8016F65C: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016F660: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    // 0x8016F664: addiu       $a0, $s0, 0x294
    ctx->r4 = ADD32(ctx->r16, 0X294);
    // 0x8016F668: jal         0x80001090
    // 0x8016F66C: addiu       $a1, $s0, 0x230
    ctx->r5 = ADD32(ctx->r16, 0X230);
    memory_copy(rdram, ctx);
        goto after_10;
    // 0x8016F66C: addiu       $a1, $s0, 0x230
    ctx->r5 = ADD32(ctx->r16, 0X230);
    after_10:
    // 0x8016F670: lw          $a1, 0x0($s4)
    ctx->r5 = MEM_W(ctx->r20, 0X0);
    // 0x8016F674: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x8016F678: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    // 0x8016F67C: jal         0x80001090
    // 0x8016F680: addiu       $a1, $a1, 0x294
    ctx->r5 = ADD32(ctx->r5, 0X294);
    memory_copy(rdram, ctx);
        goto after_11;
    // 0x8016F680: addiu       $a1, $a1, 0x294
    ctx->r5 = ADD32(ctx->r5, 0X294);
    after_11:
    // 0x8016F684: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016F688: lh          $t0, 0x4EC($s0)
    ctx->r8 = MEM_H(ctx->r16, 0X4EC);
L_8016F68C:
    // 0x8016F68C: beql        $t0, $zero, L_8016F6FC
    if (ctx->r8 == 0) {
        // 0x8016F690: or          $v0, $s0, $zero
        ctx->r2 = ctx->r16 | 0;
            goto L_8016F6FC;
    }
    goto skip_14;
    // 0x8016F690: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
    skip_14:
    // 0x8016F694: lh          $t1, 0x550($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X550);
    // 0x8016F698: beql        $t1, $zero, L_8016F6FC
    if (ctx->r9 == 0) {
        // 0x8016F69C: or          $v0, $s0, $zero
        ctx->r2 = ctx->r16 | 0;
            goto L_8016F6FC;
    }
    goto skip_15;
    // 0x8016F69C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
    skip_15:
    // 0x8016F6A0: lwc1        $f8, 0x508($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X508);
    // 0x8016F6A4: lwc1        $f10, 0x4A4($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X4A4);
    // 0x8016F6A8: addiu       $s1, $sp, 0x3C
    ctx->r17 = ADD32(ctx->r29, 0X3C);
    // 0x8016F6AC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x8016F6B0: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016F6B4: addiu       $a0, $s0, 0x48C
    ctx->r4 = ADD32(ctx->r16, 0X48C);
    // 0x8016F6B8: bc1fl       L_8016F6FC
    if (!c1cs) {
        // 0x8016F6BC: or          $v0, $s0, $zero
        ctx->r2 = ctx->r16 | 0;
            goto L_8016F6FC;
    }
    goto skip_16;
    // 0x8016F6BC: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
    skip_16:
    // 0x8016F6C0: jal         0x80001090
    // 0x8016F6C4: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    memory_copy(rdram, ctx);
        goto after_12;
    // 0x8016F6C4: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    after_12:
    // 0x8016F6C8: lw          $s0, 0x0($s4)
    ctx->r16 = MEM_W(ctx->r20, 0X0);
    // 0x8016F6CC: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    // 0x8016F6D0: addiu       $a0, $s0, 0x4F0
    ctx->r4 = ADD32(ctx->r16, 0X4F0);
    // 0x8016F6D4: jal         0x80001090
    // 0x8016F6D8: addiu       $a1, $s0, 0x48C
    ctx->r5 = ADD32(ctx->r16, 0X48C);
    memory_copy(rdram, ctx);
        goto after_13;
    // 0x8016F6D8: addiu       $a1, $s0, 0x48C
    ctx->r5 = ADD32(ctx->r16, 0X48C);
    after_13:
    // 0x8016F6DC: lw          $a1, 0x0($s4)
    ctx->r5 = MEM_W(ctx->r20, 0X0);
    // 0x8016F6E0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x8016F6E4: addiu       $a2, $zero, 0x64
    ctx->r6 = ADD32(0, 0X64);
    // 0x8016F6E8: jal         0x80001090
    // 0x8016F6EC: addiu       $a1, $a1, 0x4F0
    ctx->r5 = ADD32(ctx->r5, 0X4F0);
    memory_copy(rdram, ctx);
        goto after_14;
    // 0x8016F6EC: addiu       $a1, $a1, 0x4F0
    ctx->r5 = ADD32(ctx->r5, 0X4F0);
    after_14:
    // 0x8016F6F0: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016F6F4: lw          $s0, -0x9FC($s0)
    ctx->r16 = MEM_W(ctx->r16, -0X9FC);
    // 0x8016F6F8: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_8016F6FC:
    // 0x8016F6FC: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x8016F700: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016F704: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8016F708: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8016F70C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8016F710: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8016F714: jr          $ra
    // 0x8016F718: addiu       $sp, $sp, 0x148
    ctx->r29 = ADD32(ctx->r29, 0X148);
    return;
    // 0x8016F718: addiu       $sp, $sp, 0x148
    ctx->r29 = ADD32(ctx->r29, 0X148);
;}
RECOMP_FUNC void func_8016F71C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016F71C is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016F71C++;
    if (lod_text_guard_depth_8016F71C > 128) {
        if (!lod_text_guard_logged_8016F71C) {
            lod_text_guard_logged_8016F71C = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016F71C recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016F71C--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016F71C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016F720: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016F724: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016F728: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016F72C: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016F730: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016F734: beq         $s0, $zero, L_8016F780
    if (ctx->r16 == 0) {
        // 0x8016F738: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016F780;
    }
L_8016F738:
    // 0x8016F738: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016F73C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x8016F740: jal         0x8016F80C
    // 0x8016F744: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016F80C(rdram, ctx);
        goto after_0;
    // 0x8016F744: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x8016F748: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016F74C: beql        $a0, $zero, L_8016F760
    if (ctx->r4 == 0) {
        // 0x8016F750: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_8016F760;
    }
    goto skip_0;
    // 0x8016F750: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x8016F754: jal         0x8016F71C
    // 0x8016F758: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016F71C(rdram, ctx);
        goto after_1;
    // 0x8016F758: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x8016F75C: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_8016F760:
    // 0x8016F760: beql        $v0, $zero, L_8016F784
    if (ctx->r2 == 0) {
        // 0x8016F764: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016F784;
    }
    goto skip_1;
    // 0x8016F764: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016F768: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x8016F76C: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x8016F770: bnel        $t7, $zero, L_8016F784
    if (ctx->r15 != 0) {
        // 0x8016F774: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016F784;
    }
    goto skip_2;
    // 0x8016F774: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016F778: bne         $v0, $zero, L_8016F738
    if (ctx->r2 != 0) {
        // 0x8016F77C: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016F738;
    }
    // 0x8016F77C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016F780:
    // 0x8016F780: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016F784:
    // 0x8016F784: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016F788: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016F78C: jr          $ra
    // 0x8016F790: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016F71C--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016F790: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_8016F794(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016F794 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016F794++;
    if (lod_text_guard_depth_8016F794 > 128) {
        if (!lod_text_guard_logged_8016F794) {
            lod_text_guard_logged_8016F794 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016F794 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016F794--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016F794: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8016F798: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016F79C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016F7A0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8016F7A4: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016F7A8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016F7AC: beq         $s0, $zero, L_8016F7F8
    if (ctx->r16 == 0) {
        // 0x8016F7B0: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8016F7F8;
    }
L_8016F7B0:
    // 0x8016F7B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016F7B4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8016F7B8: jal         0x8016F80C
    // 0x8016F7BC: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_8016F80C(rdram, ctx);
        goto after_0;
    // 0x8016F7BC: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_0:
    // 0x8016F7C0: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x8016F7C4: beql        $a0, $zero, L_8016F7D8
    if (ctx->r4 == 0) {
        // 0x8016F7C8: lw          $v0, 0x10($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X10);
            goto L_8016F7D8;
    }
    goto skip_0;
    // 0x8016F7C8: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    skip_0:
    // 0x8016F7CC: jal         0x8016F794
    // 0x8016F7D0: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8016F794(rdram, ctx);
        goto after_1;
    // 0x8016F7D0: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x8016F7D4: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
L_8016F7D8:
    // 0x8016F7D8: beql        $v0, $zero, L_8016F7FC
    if (ctx->r2 == 0) {
        // 0x8016F7DC: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016F7FC;
    }
    goto skip_1;
    // 0x8016F7DC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016F7E0: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x8016F7E4: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x8016F7E8: bnel        $t7, $zero, L_8016F7FC
    if (ctx->r15 != 0) {
        // 0x8016F7EC: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016F7FC;
    }
    goto skip_2;
    // 0x8016F7EC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016F7F0: bne         $v0, $zero, L_8016F7B0
    if (ctx->r2 != 0) {
        // 0x8016F7F4: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8016F7B0;
    }
    // 0x8016F7F4: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8016F7F8:
    // 0x8016F7F8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016F7FC:
    // 0x8016F7FC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016F800: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016F804: jr          $ra
    // 0x8016F808: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016F794--;
    // --- END PATCH ---
#endif
    return;
    // 0x8016F808: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_8016F80C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016F80C: addiu       $sp, $sp, -0x88
    ctx->r29 = ADD32(ctx->r29, -0X88);
    // 0x8016F810: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8016F814: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8016F818: sw          $a0, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r4;
    // 0x8016F81C: sw          $a2, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r6;
    // 0x8016F820: lw          $v0, 0x74($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X74);
    // 0x8016F824: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x8016F828: lw          $a3, -0x1134($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X1134);
    // 0x8016F82C: beql        $v0, $zero, L_8016FB98
    if (ctx->r2 == 0) {
        // 0x8016F830: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_0;
    // 0x8016F830: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8016F834: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
    // 0x8016F838: andi        $t9, $t8, 0x1
    ctx->r25 = ctx->r24 & 0X1;
    // 0x8016F83C: beql        $t9, $zero, L_8016FB98
    if (ctx->r25 == 0) {
        // 0x8016F840: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_1;
    // 0x8016F840: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8016F844: lhu         $v1, 0x20($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X20);
    // 0x8016F848: beql        $v1, $zero, L_8016FB98
    if (ctx->r3 == 0) {
        // 0x8016F84C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_2;
    // 0x8016F84C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8016F850: lw          $t2, 0xC($a3)
    ctx->r10 = MEM_W(ctx->r7, 0XC);
    // 0x8016F854: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x8016F858: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016F85C: addu        $t5, $t2, $t4
    ctx->r13 = ADD32(ctx->r10, ctx->r12);
    // 0x8016F860: lhu         $v0, -0x2($t5)
    ctx->r2 = MEM_HU(ctx->r13, -0X2);
    // 0x8016F864: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016F868: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016F86C: beq         $v0, $at, L_8016FB94
    if (ctx->r2 == ctx->r1) {
        // 0x8016F870: or          $t0, $v0, $zero
        ctx->r8 = ctx->r2 | 0;
            goto L_8016FB94;
    }
    // 0x8016F870: or          $t0, $v0, $zero
    ctx->r8 = ctx->r2 | 0;
    // 0x8016F874: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x8016F878: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x8016F87C: subu        $t8, $t8, $v0
    ctx->r24 = SUB32(ctx->r24, ctx->r2);
    // 0x8016F880: lw          $t7, 0x4($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X4);
    // 0x8016F884: sll         $t8, $t8, 4
    ctx->r24 = S32(ctx->r24 << 4);
    // 0x8016F888: addiu       $t9, $sp, 0x38
    ctx->r25 = ADD32(ctx->r29, 0X38);
    // 0x8016F88C: sw          $t9, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r25;
    // 0x8016F890: bne         $a1, $zero, L_8016F8CC
    if (ctx->r5 != 0) {
        // 0x8016F894: addu        $s0, $t7, $t8
        ctx->r16 = ADD32(ctx->r15, ctx->r24);
            goto L_8016F8CC;
    }
    // 0x8016F894: addu        $s0, $t7, $t8
    ctx->r16 = ADD32(ctx->r15, ctx->r24);
    // 0x8016F898: lwc1        $f4, 0x50($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X50);
    // 0x8016F89C: swc1        $f4, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->f4.u32l;
    // 0x8016F8A0: lwc1        $f6, 0x54($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X54);
    // 0x8016F8A4: swc1        $f6, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->f6.u32l;
    // 0x8016F8A8: lwc1        $f8, 0x58($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X58);
    // 0x8016F8AC: swc1        $f8, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->f8.u32l;
    // 0x8016F8B0: lh          $t2, 0x5C($a0)
    ctx->r10 = MEM_H(ctx->r4, 0X5C);
    // 0x8016F8B4: sh          $t2, 0x3E($sp)
    MEM_H(0X3E, ctx->r29) = ctx->r10;
    // 0x8016F8B8: lh          $t4, 0x5E($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X5E);
    // 0x8016F8BC: sh          $t4, 0x40($sp)
    MEM_H(0X40, ctx->r29) = ctx->r12;
    // 0x8016F8C0: lh          $t5, 0x60($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X60);
    // 0x8016F8C4: b           L_8016F8F4
    // 0x8016F8C8: sh          $t5, 0x42($sp)
    MEM_H(0X42, ctx->r29) = ctx->r13;
        goto L_8016F8F4;
    // 0x8016F8C8: sh          $t5, 0x42($sp)
    MEM_H(0X42, ctx->r29) = ctx->r13;
L_8016F8CC:
    // 0x8016F8CC: lw          $a0, 0x88($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X88);
    // 0x8016F8D0: addiu       $a1, $sp, 0x78
    ctx->r5 = ADD32(ctx->r29, 0X78);
    // 0x8016F8D4: addiu       $a2, $sp, 0x3E
    ctx->r6 = ADD32(ctx->r29, 0X3E);
    // 0x8016F8D8: addiu       $a3, $sp, 0x6C
    ctx->r7 = ADD32(ctx->r29, 0X6C);
    // 0x8016F8DC: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    // 0x8016F8E0: jal         0x80167F54
    // 0x8016F8E4: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    func_80167F54(rdram, ctx);
        goto after_0;
    // 0x8016F8E4: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    after_0:
    // 0x8016F8E8: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x8016F8EC: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x8016F8F0: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
L_8016F8F4:
    // 0x8016F8F4: lw          $t6, 0x90($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X90);
    // 0x8016F8F8: addiu       $v1, $sp, 0x78
    ctx->r3 = ADD32(ctx->r29, 0X78);
    // 0x8016F8FC: addiu       $v0, $sp, 0x38
    ctx->r2 = ADD32(ctx->r29, 0X38);
    // 0x8016F900: bne         $t6, $zero, L_8016F910
    if (ctx->r14 != 0) {
        // 0x8016F904: lui         $at, 0x4120
        ctx->r1 = S32(0X4120 << 16);
            goto L_8016F910;
    }
    // 0x8016F904: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x8016F908: sh          $zero, 0x42($sp)
    MEM_H(0X42, ctx->r29) = 0;
    // 0x8016F90C: sh          $zero, 0x3E($sp)
    MEM_H(0X3E, ctx->r29) = 0;
L_8016F910:
    // 0x8016F910: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x8016F914: addiu       $a0, $sp, 0x3E
    ctx->r4 = ADD32(ctx->r29, 0X3E);
    // 0x8016F918: lwc1        $f14, 0x0($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8016F91C: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016F920: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016F924: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x8016F928: beql        $at, $zero, L_8016F958
    if (ctx->r1 == 0) {
        // 0x8016F92C: trunc.w.s   $f16, $f18
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
            goto L_8016F958;
    }
    goto skip_3;
    // 0x8016F92C: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    skip_3:
L_8016F930:
    // 0x8016F930: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x8016F934: lwc1        $f14, 0x4($v1)
    ctx->f14.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8016F938: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016F93C: sltu        $at, $v0, $a0
    ctx->r1 = ctx->r2 < ctx->r4 ? 1 : 0;
    // 0x8016F940: mul.s       $f18, $f14, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x8016F944: mfc1        $t8, $f16
    ctx->r24 = (int32_t)ctx->f16.u32l;
    // 0x8016F948: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016F94C: bne         $at, $zero, L_8016F930
    if (ctx->r1 != 0) {
        // 0x8016F950: sh          $t8, -0x4($v0)
        MEM_H(-0X4, ctx->r2) = ctx->r24;
            goto L_8016F930;
    }
    // 0x8016F950: sh          $t8, -0x4($v0)
    MEM_H(-0X4, ctx->r2) = ctx->r24;
    // 0x8016F954: trunc.w.s   $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.u32l = TRUNC_W_S(ctx->f18.fl);
L_8016F958:
    // 0x8016F958: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8016F95C: mfc1        $t8, $f16
    ctx->r24 = (int32_t)ctx->f16.u32l;
    // 0x8016F960: nop

    // 0x8016F964: sh          $t8, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r24;
    // 0x8016F968: lh          $t2, 0x3E($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X3E);
    // 0x8016F96C: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x8016F970: sh          $t0, 0x5C($sp)
    MEM_H(0X5C, ctx->r29) = ctx->r8;
    // 0x8016F974: bne         $t2, $zero, L_8016FA64
    if (ctx->r10 != 0) {
        // 0x8016F978: sh          $t9, 0x5E($sp)
        MEM_H(0X5E, ctx->r29) = ctx->r25;
            goto L_8016FA64;
    }
    // 0x8016F978: sh          $t9, 0x5E($sp)
    MEM_H(0X5E, ctx->r29) = ctx->r25;
    // 0x8016F97C: lh          $t4, 0x42($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X42);
    // 0x8016F980: bne         $t4, $zero, L_8016FA64
    if (ctx->r12 != 0) {
        // 0x8016F984: nop
    
            goto L_8016FA64;
    }
    // 0x8016F984: nop

    // 0x8016F988: lw          $t3, 0x4($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X4);
    // 0x8016F98C: beql        $t3, $zero, L_8016FB98
    if (ctx->r11 == 0) {
        // 0x8016F990: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_4;
    // 0x8016F990: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_4:
    // 0x8016F994: lw          $t5, 0x0($t1)
    ctx->r13 = MEM_W(ctx->r9, 0X0);
    // 0x8016F998: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x8016F99C: jal         0x801721CC
    // 0x8016F9A0: lw          $a1, 0xC($t5)
    ctx->r5 = MEM_W(ctx->r13, 0XC);
    func_801721CC(rdram, ctx);
        goto after_1;
    // 0x8016F9A0: lw          $a1, 0xC($t5)
    ctx->r5 = MEM_W(ctx->r13, 0XC);
    after_1:
    // 0x8016F9A4: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016F9A8: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016F9AC: lwc1        $f6, 0x1C($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x8016F9B0: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x8016F9B4: lwc1        $f4, 0x12C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X12C);
    // 0x8016F9B8: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016F9BC: nop

    // 0x8016F9C0: bc1t        L_8016FA44
    if (c1cs) {
        // 0x8016F9C4: nop
    
            goto L_8016FA44;
    }
    // 0x8016F9C4: nop

    // 0x8016F9C8: lwc1        $f8, 0x28($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X28);
    // 0x8016F9CC: lwc1        $f10, 0x120($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X120);
    // 0x8016F9D0: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016F9D4: nop

    // 0x8016F9D8: bc1t        L_8016FA44
    if (c1cs) {
        // 0x8016F9DC: nop
    
            goto L_8016FA44;
    }
    // 0x8016F9DC: nop

    // 0x8016F9E0: lwc1        $f16, 0x128($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X128);
    // 0x8016F9E4: lwc1        $f18, 0x18($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016F9E8: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016F9EC: nop

    // 0x8016F9F0: bc1t        L_8016FA44
    if (c1cs) {
        // 0x8016F9F4: nop
    
            goto L_8016FA44;
    }
    // 0x8016F9F4: nop

    // 0x8016F9F8: lwc1        $f4, 0x24($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X24);
    // 0x8016F9FC: lwc1        $f6, 0x11C($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x8016FA00: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FA04: nop

    // 0x8016FA08: bc1t        L_8016FA44
    if (c1cs) {
        // 0x8016FA0C: nop
    
            goto L_8016FA44;
    }
    // 0x8016FA0C: nop

    // 0x8016FA10: lwc1        $f8, 0x130($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X130);
    // 0x8016FA14: lwc1        $f10, 0x20($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X20);
    // 0x8016FA18: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FA1C: nop

    // 0x8016FA20: bc1t        L_8016FA44
    if (c1cs) {
        // 0x8016FA24: nop
    
            goto L_8016FA44;
    }
    // 0x8016FA24: nop

    // 0x8016FA28: lwc1        $f16, 0x2C($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X2C);
    // 0x8016FA2C: lwc1        $f18, 0x124($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X124);
    // 0x8016FA30: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016FA34: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FA38: nop

    // 0x8016FA3C: bc1f        L_8016FA4C
    if (!c1cs) {
        // 0x8016FA40: nop
    
            goto L_8016FA4C;
    }
    // 0x8016FA40: nop

L_8016FA44:
    // 0x8016FA44: b           L_8016FA4C
    // 0x8016FA48: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016FA4C;
    // 0x8016FA48: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016FA4C:
    // 0x8016FA4C: beql        $v0, $zero, L_8016FB98
    if (ctx->r2 == 0) {
        // 0x8016FA50: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_5;
    // 0x8016FA50: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_5:
    // 0x8016FA54: jal         0x80170048
    // 0x8016FA58: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80170048(rdram, ctx);
        goto after_2;
    // 0x8016FA58: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_2:
    // 0x8016FA5C: b           L_8016FB98
    // 0x8016FA60: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8016FB98;
    // 0x8016FA60: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016FA64:
    // 0x8016FA64: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016FA68: lw          $a1, -0x1130($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1130);
    // 0x8016FA6C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8016FA70: blez        $a1, L_8016FAB8
    if (SIGNED(ctx->r5) <= 0) {
        // 0x8016FA74: nop
    
            goto L_8016FAB8;
    }
    // 0x8016FA74: nop

    // 0x8016FA78: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x8016FA7C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016FA80: lw          $v0, -0x1128($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X1128);
    // 0x8016FA84: lw          $v1, 0x10($t6)
    ctx->r3 = MEM_W(ctx->r14, 0X10);
L_8016FA88:
    // 0x8016FA88: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x8016FA8C: bnel        $v1, $t7, L_8016FAAC
    if (ctx->r3 != ctx->r15) {
        // 0x8016FA90: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_8016FAAC;
    }
    goto skip_6;
    // 0x8016FA90: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    skip_6:
    // 0x8016FA94: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    // 0x8016FA98: addiu       $t8, $v1, 0x9C
    ctx->r24 = ADD32(ctx->r3, 0X9C);
    // 0x8016FA9C: sw          $t8, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r24;
    // 0x8016FAA0: b           L_8016FAB8
    // 0x8016FAA4: addiu       $s0, $v1, 0x6C
    ctx->r16 = ADD32(ctx->r3, 0X6C);
        goto L_8016FAB8;
    // 0x8016FAA4: addiu       $s0, $v1, 0x6C
    ctx->r16 = ADD32(ctx->r3, 0X6C);
    // 0x8016FAA8: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_8016FAAC:
    // 0x8016FAAC: slt         $at, $a0, $a1
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x8016FAB0: bne         $at, $zero, L_8016FA88
    if (ctx->r1 != 0) {
        // 0x8016FAB4: addiu       $v0, $v0, 0x8
        ctx->r2 = ADD32(ctx->r2, 0X8);
            goto L_8016FA88;
    }
    // 0x8016FAB4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
L_8016FAB8:
    // 0x8016FAB8: beql        $a0, $a1, L_8016FB98
    if (ctx->r4 == ctx->r5) {
        // 0x8016FABC: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_7;
    // 0x8016FABC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_7:
    // 0x8016FAC0: lw          $t9, 0x4($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X4);
    // 0x8016FAC4: beql        $t9, $zero, L_8016FB98
    if (ctx->r25 == 0) {
        // 0x8016FAC8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_8;
    // 0x8016FAC8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_8:
    // 0x8016FACC: lw          $t2, 0x0($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X0);
    // 0x8016FAD0: lw          $a0, 0x60($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X60);
    // 0x8016FAD4: jal         0x801721CC
    // 0x8016FAD8: lw          $a1, 0xC($t2)
    ctx->r5 = MEM_W(ctx->r10, 0XC);
    func_801721CC(rdram, ctx);
        goto after_3;
    // 0x8016FAD8: lw          $a1, 0xC($t2)
    ctx->r5 = MEM_W(ctx->r10, 0XC);
    after_3:
    // 0x8016FADC: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016FAE0: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016FAE4: lwc1        $f6, 0x1C($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x8016FAE8: lw          $a0, 0x60($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X60);
    // 0x8016FAEC: lwc1        $f4, 0x12C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X12C);
    // 0x8016FAF0: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FAF4: nop

    // 0x8016FAF8: bc1t        L_8016FB7C
    if (c1cs) {
        // 0x8016FAFC: nop
    
            goto L_8016FB7C;
    }
    // 0x8016FAFC: nop

    // 0x8016FB00: lwc1        $f8, 0x28($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X28);
    // 0x8016FB04: lwc1        $f10, 0x120($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X120);
    // 0x8016FB08: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FB0C: nop

    // 0x8016FB10: bc1t        L_8016FB7C
    if (c1cs) {
        // 0x8016FB14: nop
    
            goto L_8016FB7C;
    }
    // 0x8016FB14: nop

    // 0x8016FB18: lwc1        $f16, 0x128($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X128);
    // 0x8016FB1C: lwc1        $f18, 0x18($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016FB20: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FB24: nop

    // 0x8016FB28: bc1t        L_8016FB7C
    if (c1cs) {
        // 0x8016FB2C: nop
    
            goto L_8016FB7C;
    }
    // 0x8016FB2C: nop

    // 0x8016FB30: lwc1        $f4, 0x24($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X24);
    // 0x8016FB34: lwc1        $f6, 0x11C($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x8016FB38: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FB3C: nop

    // 0x8016FB40: bc1t        L_8016FB7C
    if (c1cs) {
        // 0x8016FB44: nop
    
            goto L_8016FB7C;
    }
    // 0x8016FB44: nop

    // 0x8016FB48: lwc1        $f8, 0x130($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X130);
    // 0x8016FB4C: lwc1        $f10, 0x20($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X20);
    // 0x8016FB50: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FB54: nop

    // 0x8016FB58: bc1t        L_8016FB7C
    if (c1cs) {
        // 0x8016FB5C: nop
    
            goto L_8016FB7C;
    }
    // 0x8016FB5C: nop

    // 0x8016FB60: lwc1        $f16, 0x2C($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X2C);
    // 0x8016FB64: lwc1        $f18, 0x124($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X124);
    // 0x8016FB68: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016FB6C: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FB70: nop

    // 0x8016FB74: bc1f        L_8016FB84
    if (!c1cs) {
        // 0x8016FB78: nop
    
            goto L_8016FB84;
    }
    // 0x8016FB78: nop

L_8016FB7C:
    // 0x8016FB7C: b           L_8016FB84
    // 0x8016FB80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016FB84;
    // 0x8016FB80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016FB84:
    // 0x8016FB84: beql        $v0, $zero, L_8016FB98
    if (ctx->r2 == 0) {
        // 0x8016FB88: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8016FB98;
    }
    goto skip_9;
    // 0x8016FB88: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_9:
    // 0x8016FB8C: jal         0x80170124
    // 0x8016FB90: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80170124(rdram, ctx);
        goto after_4;
    // 0x8016FB90: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_4:
L_8016FB94:
    // 0x8016FB94: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8016FB98:
    // 0x8016FB98: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8016FB9C: addiu       $sp, $sp, 0x88
    ctx->r29 = ADD32(ctx->r29, 0X88);
    // 0x8016FBA0: jr          $ra
    // 0x8016FBA4: nop

    return;
    // 0x8016FBA4: nop

;}
RECOMP_FUNC void func_8016FBA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016FBA8: jr          $ra
    // 0x8016FBAC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x8016FBAC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8016FBB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8016FBB0: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8016FBB4: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x8016FBB8: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8016FBBC: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8016FBC0: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x8016FBC4: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8016FBC8: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x8016FBCC: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x8016FBD0: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x8016FBD4: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8016FBD8: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: pointer-range guard (LOD_FIX_TEXT_MEASURE_GUARD, round 20) ---
    // Mirrors func_8016890C's round-14 guard (funcs_57.c): the original
    // code only null-checks a0 (the `beq $a0,$zero` just below) before
    // immediately dereferencing it (obj+0xC at 0x8016FBF0, the first of
    // several unguarded float loads). func_8016F310's own workspace/array
    // guards (above in this file) validate $s2 and the array BASE pointers,
    // not each individual +0x10/+0x14/+0x18 field or array element's own
    // value, so a single stale element could still reach here even with
    // those in place. Same "standard range check" and early-return shape
    // as func_8016890C's guard, taking the exact same path the game's own
    // null check uses when it fails.
    if (ctx->r4 != 0 && !lod_text_guard_addr_ok((uint32_t)ctx->r4)) {
        if (!lod_text_guard_fbb0_logged) {
            lod_text_guard_fbb0_logged = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016FBB0 rejected out-of-range obj=0x%08X "
                    "(stale text-struct pointer)\n",
                    (uint32_t)ctx->r4);
        }
        MEM_W(0X14, ctx->r29) = ctx->r16;
        goto L_8016FE3C;
    }
    // --- END PATCH ---
#endif
    // 0x8016FBDC: beq         $a0, $zero, L_8016FE3C
    if (ctx->r4 == 0) {
        // 0x8016FBE0: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_8016FE3C;
    }
    // 0x8016FBE0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8016FBE4: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x8016FBE8: addiu       $s7, $s7, -0x9FC
    ctx->r23 = ADD32(ctx->r23, -0X9FC);
    // 0x8016FBEC: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x8016FBF0: lwc1        $f6, 0xC($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8016FBF4: addiu       $a1, $s3, 0x8
    ctx->r5 = ADD32(ctx->r19, 0X8);
    // 0x8016FBF8: lwc1        $f4, 0x114($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X114);
    // 0x8016FBFC: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FC00: nop

    // 0x8016FC04: bc1t        L_8016FC88
    if (c1cs) {
        // 0x8016FC08: nop
    
            goto L_8016FC88;
    }
    // 0x8016FC08: nop

    // 0x8016FC0C: lwc1        $f8, 0x18($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X18);
    // 0x8016FC10: lwc1        $f10, 0x108($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X108);
    // 0x8016FC14: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FC18: nop

    // 0x8016FC1C: bc1t        L_8016FC88
    if (c1cs) {
        // 0x8016FC20: nop
    
            goto L_8016FC88;
    }
    // 0x8016FC20: nop

    // 0x8016FC24: lwc1        $f16, 0x110($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X110);
    // 0x8016FC28: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8016FC2C: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FC30: nop

    // 0x8016FC34: bc1t        L_8016FC88
    if (c1cs) {
        // 0x8016FC38: nop
    
            goto L_8016FC88;
    }
    // 0x8016FC38: nop

    // 0x8016FC3C: lwc1        $f4, 0x14($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X14);
    // 0x8016FC40: lwc1        $f6, 0x104($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X104);
    // 0x8016FC44: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FC48: nop

    // 0x8016FC4C: bc1t        L_8016FC88
    if (c1cs) {
        // 0x8016FC50: nop
    
            goto L_8016FC88;
    }
    // 0x8016FC50: nop

    // 0x8016FC54: lwc1        $f8, 0x118($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X118);
    // 0x8016FC58: lwc1        $f10, 0x10($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X10);
    // 0x8016FC5C: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FC60: nop

    // 0x8016FC64: bc1t        L_8016FC88
    if (c1cs) {
        // 0x8016FC68: nop
    
            goto L_8016FC88;
    }
    // 0x8016FC68: nop

    // 0x8016FC6C: lwc1        $f16, 0x1C($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X1C);
    // 0x8016FC70: lwc1        $f18, 0x10C($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X10C);
    // 0x8016FC74: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016FC78: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FC7C: nop

    // 0x8016FC80: bc1f        L_8016FC90
    if (!c1cs) {
        // 0x8016FC84: nop
    
            goto L_8016FC90;
    }
    // 0x8016FC84: nop

L_8016FC88:
    // 0x8016FC88: b           L_8016FC90
    // 0x8016FC8C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016FC90;
    // 0x8016FC8C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016FC90:
    // 0x8016FC90: beq         $v0, $zero, L_8016FE3C
    if (ctx->r2 == 0) {
        // 0x8016FC94: lui         $a0, 0x801A
        ctx->r4 = S32(0X801A << 16);
            goto L_8016FE3C;
    }
    // 0x8016FC94: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016FC98: jal         0x80169B88
    // 0x8016FC9C: addiu       $a0, $a0, -0x1110
    ctx->r4 = ADD32(ctx->r4, -0X1110);
    func_80169B88(rdram, ctx);
        goto after_0;
    // 0x8016FC9C: addiu       $a0, $a0, -0x1110
    ctx->r4 = ADD32(ctx->r4, -0X1110);
    after_0:
    // 0x8016FCA0: lui         $s0, 0x801A
    ctx->r16 = S32(0X801A << 16);
    // 0x8016FCA4: lui         $s6, 0x8019
    ctx->r22 = S32(0X8019 << 16);
    // 0x8016FCA8: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8016FCAC: addiu       $s6, $s6, 0x2490
    ctx->r22 = ADD32(ctx->r22, 0X2490);
    // 0x8016FCB0: addiu       $s0, $s0, -0x111C
    ctx->r16 = ADD32(ctx->r16, -0X111C);
    // 0x8016FCB4: addiu       $t6, $t6, -0x1680
    ctx->r14 = ADD32(ctx->r14, -0X1680);
    // 0x8016FCB8: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x8016FCBC: sw          $t6, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r14;
    // 0x8016FCC0: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x8016FCC4: addiu       $a0, $a0, -0x9F8
    ctx->r4 = ADD32(ctx->r4, -0X9F8);
    // 0x8016FCC8: jal         0x80000F30
    // 0x8016FCCC: lw          $a1, 0x0($s3)
    ctx->r5 = MEM_W(ctx->r19, 0X0);
    bzero_recomp(rdram, ctx);
        goto after_1;
    // 0x8016FCCC: lw          $a1, 0x0($s3)
    ctx->r5 = MEM_W(ctx->r19, 0X0);
    after_1:
    // 0x8016FCD0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8016FCD4: sw          $s1, -0x10F8($at)
    MEM_W(-0X10F8, ctx->r1) = ctx->r17;
    // 0x8016FCD8: jal         0x8016FE68
    // 0x8016FCDC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8016FE68(rdram, ctx);
        goto after_2;
    // 0x8016FCDC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_2:
    // 0x8016FCE0: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x8016FCE4: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x8016FCE8: addiu       $s5, $zero, 0x30
    ctx->r21 = ADD32(0, 0X30);
    // 0x8016FCEC: blez        $v1, L_8016FE30
    if (SIGNED(ctx->r3) <= 0) {
        // 0x8016FCF0: addiu       $s4, $zero, 0x28
        ctx->r20 = ADD32(0, 0X28);
            goto L_8016FE30;
    }
    // 0x8016FCF0: addiu       $s4, $zero, 0x28
    ctx->r20 = ADD32(0, 0X28);
    // 0x8016FCF4: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
L_8016FCF8:
    // 0x8016FCF8: lw          $t1, 0x4($s3)
    ctx->r9 = MEM_W(ctx->r19, 0X4);
    // 0x8016FCFC: addu        $t8, $t7, $s2
    ctx->r24 = ADD32(ctx->r15, ctx->r18);
    // 0x8016FD00: lh          $t9, 0x0($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X0);
    // 0x8016FD04: multu       $t9, $s4
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r20)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016FD08: mflo        $t0
    ctx->r8 = lo;
    // 0x8016FD0C: addu        $s0, $t0, $t1
    ctx->r16 = ADD32(ctx->r8, ctx->r9);
    // 0x8016FD10: lh          $t2, 0x26($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X26);
    // 0x8016FD14: beql        $t2, $zero, L_8016FD28
    if (ctx->r10 == 0) {
        // 0x8016FD18: lh          $t4, 0x24($s0)
        ctx->r12 = MEM_H(ctx->r16, 0X24);
            goto L_8016FD28;
    }
    goto skip_0;
    // 0x8016FD18: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
    skip_0:
    // 0x8016FD1C: b           L_8016FE20
    // 0x8016FD20: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016FE20;
    // 0x8016FD20: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x8016FD24: lh          $t4, 0x24($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X24);
L_8016FD28:
    // 0x8016FD28: lw          $v0, 0x0($s7)
    ctx->r2 = MEM_W(ctx->r23, 0X0);
    // 0x8016FD2C: multu       $t4, $s5
    result = U64(U32(ctx->r12)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8016FD30: lw          $t3, 0x4($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X4);
    // 0x8016FD34: mflo        $t5
    ctx->r13 = lo;
    // 0x8016FD38: addu        $s1, $t3, $t5
    ctx->r17 = ADD32(ctx->r11, ctx->r13);
    // 0x8016FD3C: lw          $t6, 0x4($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X4);
    // 0x8016FD40: bnel        $t6, $zero, L_8016FD54
    if (ctx->r14 != 0) {
        // 0x8016FD44: lwc1        $f4, 0x114($v0)
        ctx->f4.u32l = MEM_W(ctx->r2, 0X114);
            goto L_8016FD54;
    }
    goto skip_1;
    // 0x8016FD44: lwc1        $f4, 0x114($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X114);
    skip_1:
    // 0x8016FD48: b           L_8016FE20
    // 0x8016FD4C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016FE20;
    // 0x8016FD4C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
    // 0x8016FD50: lwc1        $f4, 0x114($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X114);
L_8016FD54:
    // 0x8016FD54: lwc1        $f6, 0x10($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X10);
    // 0x8016FD58: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016FD5C: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FD60: nop

    // 0x8016FD64: bc1t        L_8016FDE8
    if (c1cs) {
        // 0x8016FD68: nop
    
            goto L_8016FDE8;
    }
    // 0x8016FD68: nop

    // 0x8016FD6C: lwc1        $f8, 0x1C($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x8016FD70: lwc1        $f10, 0x108($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X108);
    // 0x8016FD74: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FD78: nop

    // 0x8016FD7C: bc1t        L_8016FDE8
    if (c1cs) {
        // 0x8016FD80: nop
    
            goto L_8016FDE8;
    }
    // 0x8016FD80: nop

    // 0x8016FD84: lwc1        $f16, 0x110($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X110);
    // 0x8016FD88: lwc1        $f18, 0xC($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0XC);
    // 0x8016FD8C: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FD90: nop

    // 0x8016FD94: bc1t        L_8016FDE8
    if (c1cs) {
        // 0x8016FD98: nop
    
            goto L_8016FDE8;
    }
    // 0x8016FD98: nop

    // 0x8016FD9C: lwc1        $f4, 0x18($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8016FDA0: lwc1        $f6, 0x104($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X104);
    // 0x8016FDA4: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FDA8: nop

    // 0x8016FDAC: bc1t        L_8016FDE8
    if (c1cs) {
        // 0x8016FDB0: nop
    
            goto L_8016FDE8;
    }
    // 0x8016FDB0: nop

    // 0x8016FDB4: lwc1        $f8, 0x118($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X118);
    // 0x8016FDB8: lwc1        $f10, 0x14($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X14);
    // 0x8016FDBC: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FDC0: nop

    // 0x8016FDC4: bc1t        L_8016FDE8
    if (c1cs) {
        // 0x8016FDC8: nop
    
            goto L_8016FDE8;
    }
    // 0x8016FDC8: nop

    // 0x8016FDCC: lwc1        $f16, 0x20($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X20);
    // 0x8016FDD0: lwc1        $f18, 0x10C($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X10C);
    // 0x8016FDD4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016FDD8: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FDDC: nop

    // 0x8016FDE0: bc1f        L_8016FDF0
    if (!c1cs) {
        // 0x8016FDE4: nop
    
            goto L_8016FDF0;
    }
    // 0x8016FDE4: nop

L_8016FDE8:
    // 0x8016FDE8: b           L_8016FDF0
    // 0x8016FDEC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016FDF0;
    // 0x8016FDEC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016FDF0:
    // 0x8016FDF0: bne         $v0, $zero, L_8016FE00
    if (ctx->r2 != 0) {
        // 0x8016FDF4: nop
    
            goto L_8016FE00;
    }
    // 0x8016FDF4: nop

    // 0x8016FDF8: b           L_8016FE20
    // 0x8016FDFC: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_8016FE20;
    // 0x8016FDFC: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_8016FE00:
    // 0x8016FE00: jal         0x801721CC
    // 0x8016FE04: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_801721CC(rdram, ctx);
        goto after_3;
    // 0x8016FE04: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_3:
    // 0x8016FE08: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8016FE0C: jal         0x80170048
    // 0x8016FE10: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80170048(rdram, ctx);
        goto after_4;
    // 0x8016FE10: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_4:
    // 0x8016FE14: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8016FE18: lw          $v1, -0x111C($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X111C);
    // 0x8016FE1C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_8016FE20:
    // 0x8016FE20: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    // 0x8016FE24: slt         $at, $s2, $v0
    ctx->r1 = SIGNED(ctx->r18) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x8016FE28: bnel        $at, $zero, L_8016FCF8
    if (ctx->r1 != 0) {
        // 0x8016FE2C: lw          $t7, 0x0($s6)
        ctx->r15 = MEM_W(ctx->r22, 0X0);
            goto L_8016FCF8;
    }
    goto skip_2;
    // 0x8016FE2C: lw          $t7, 0x0($s6)
    ctx->r15 = MEM_W(ctx->r22, 0X0);
    skip_2:
L_8016FE30:
    // 0x8016FE30: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016FE34: addiu       $t7, $t7, -0x1680
    ctx->r15 = ADD32(ctx->r15, -0X1680);
    // 0x8016FE38: sw          $t7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r15;
L_8016FE3C:
    // 0x8016FE3C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x8016FE40: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8016FE44: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8016FE48: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8016FE4C: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x8016FE50: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x8016FE54: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x8016FE58: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x8016FE5C: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x8016FE60: jr          $ra
    // 0x8016FE64: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x8016FE64: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_8016FE68(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_8016FE68 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_8016FE68++;
    if (lod_text_guard_depth_8016FE68 > 128) {
        if (!lod_text_guard_logged_8016FE68) {
            lod_text_guard_logged_8016FE68 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_8016FE68 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_8016FE68--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x8016FE68: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8016FE6C: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8016FE70: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8016FE74: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016FE78: addiu       $a1, $a1, -0x1110
    ctx->r5 = ADD32(ctx->r5, -0X1110);
    // 0x8016FE7C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8016FE80: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8016FE84: lwc1        $f6, 0x4($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8016FE88: lwc1        $f4, 0x114($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X114);
    // 0x8016FE8C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8016FE90: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8016FE94: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FE98: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x8016FE9C: bc1t        L_8016FF20
    if (c1cs) {
        // 0x8016FEA0: nop
    
            goto L_8016FF20;
    }
    // 0x8016FEA0: nop

    // 0x8016FEA4: lwc1        $f8, 0x10($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X10);
    // 0x8016FEA8: lwc1        $f10, 0x108($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X108);
    // 0x8016FEAC: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FEB0: nop

    // 0x8016FEB4: bc1t        L_8016FF20
    if (c1cs) {
        // 0x8016FEB8: nop
    
            goto L_8016FF20;
    }
    // 0x8016FEB8: nop

    // 0x8016FEBC: lwc1        $f16, 0x110($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X110);
    // 0x8016FEC0: lwc1        $f18, 0x0($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8016FEC4: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FEC8: nop

    // 0x8016FECC: bc1t        L_8016FF20
    if (c1cs) {
        // 0x8016FED0: nop
    
            goto L_8016FF20;
    }
    // 0x8016FED0: nop

    // 0x8016FED4: lwc1        $f4, 0xC($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0XC);
    // 0x8016FED8: lwc1        $f6, 0x104($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X104);
    // 0x8016FEDC: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8016FEE0: nop

    // 0x8016FEE4: bc1t        L_8016FF20
    if (c1cs) {
        // 0x8016FEE8: nop
    
            goto L_8016FF20;
    }
    // 0x8016FEE8: nop

    // 0x8016FEEC: lwc1        $f8, 0x118($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X118);
    // 0x8016FEF0: lwc1        $f10, 0x8($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8016FEF4: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8016FEF8: nop

    // 0x8016FEFC: bc1t        L_8016FF20
    if (c1cs) {
        // 0x8016FF00: nop
    
            goto L_8016FF20;
    }
    // 0x8016FF00: nop

    // 0x8016FF04: lwc1        $f16, 0x14($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X14);
    // 0x8016FF08: lwc1        $f18, 0x10C($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X10C);
    // 0x8016FF0C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8016FF10: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x8016FF14: nop

    // 0x8016FF18: bc1f        L_8016FF28
    if (!c1cs) {
        // 0x8016FF1C: nop
    
            goto L_8016FF28;
    }
    // 0x8016FF1C: nop

L_8016FF20:
    // 0x8016FF20: b           L_8016FF28
    // 0x8016FF24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8016FF28;
    // 0x8016FF24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8016FF28:
    // 0x8016FF28: beql        $v0, $zero, L_8017003C
    if (ctx->r2 == 0) {
        // 0x8016FF2C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_8017003C;
    }
    goto skip_0;
    // 0x8016FF2C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x8016FF30: lw          $t7, -0x10F8($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X10F8);
    // 0x8016FF34: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x8016FF38: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8016FF3C: addu        $a3, $t6, $t7
    ctx->r7 = ADD32(ctx->r14, ctx->r15);
    // 0x8016FF40: lh          $v1, 0x0($a3)
    ctx->r3 = MEM_H(ctx->r7, 0X0);
    // 0x8016FF44: bnel        $v1, $at, L_8016FFD4
    if (ctx->r3 != ctx->r1) {
        // 0x8016FF48: lh          $t5, 0x2($a3)
        ctx->r13 = MEM_H(ctx->r7, 0X2);
            goto L_8016FFD4;
    }
    goto skip_1;
    // 0x8016FF48: lh          $t5, 0x2($a3)
    ctx->r13 = MEM_H(ctx->r7, 0X2);
    skip_1:
    // 0x8016FF4C: lh          $a2, 0x2($a3)
    ctx->r6 = MEM_H(ctx->r7, 0X2);
    // 0x8016FF50: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x8016FF54: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8016FF58: blez        $a2, L_80170038
    if (SIGNED(ctx->r6) <= 0) {
        // 0x8016FF5C: lui         $t2, 0x8019
        ctx->r10 = S32(0X8019 << 16);
            goto L_80170038;
    }
    // 0x8016FF5C: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x8016FF60: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x8016FF64: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x8016FF68: addiu       $a1, $a1, -0x111C
    ctx->r5 = ADD32(ctx->r5, -0X111C);
    // 0x8016FF6C: addiu       $t0, $t0, -0x9F8
    ctx->r8 = ADD32(ctx->r8, -0X9F8);
    // 0x8016FF70: addiu       $t2, $t2, 0x2490
    ctx->r10 = ADD32(ctx->r10, 0X2490);
    // 0x8016FF74: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
L_8016FF78:
    // 0x8016FF78: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x8016FF7C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8016FF80: addu        $a0, $t0, $t8
    ctx->r4 = ADD32(ctx->r8, ctx->r24);
    // 0x8016FF84: lbu         $t9, 0x0($a0)
    ctx->r25 = MEM_BU(ctx->r4, 0X0);
    // 0x8016FF88: bnel        $t9, $zero, L_8016FFC0
    if (ctx->r25 != 0) {
        // 0x8016FF8C: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_8016FFC0;
    }
    goto skip_2;
    // 0x8016FF8C: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_2:
    // 0x8016FF90: lw          $t5, 0x0($a1)
    ctx->r13 = MEM_W(ctx->r5, 0X0);
    // 0x8016FF94: lw          $t4, 0x0($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X0);
    // 0x8016FF98: sb          $t1, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r9;
    // 0x8016FF9C: lh          $t3, 0x0($v0)
    ctx->r11 = MEM_H(ctx->r2, 0X0);
    // 0x8016FFA0: sll         $t6, $t5, 1
    ctx->r14 = S32(ctx->r13 << 1);
    // 0x8016FFA4: addu        $t7, $t4, $t6
    ctx->r15 = ADD32(ctx->r12, ctx->r14);
    // 0x8016FFA8: sh          $t3, 0x0($t7)
    MEM_H(0X0, ctx->r15) = ctx->r11;
    // 0x8016FFAC: lw          $t8, 0x0($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X0);
    // 0x8016FFB0: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x8016FFB4: sw          $t9, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r25;
    // 0x8016FFB8: lh          $a2, 0x2($a3)
    ctx->r6 = MEM_H(ctx->r7, 0X2);
    // 0x8016FFBC: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
L_8016FFC0:
    // 0x8016FFC0: bne         $at, $zero, L_8016FF78
    if (ctx->r1 != 0) {
        // 0x8016FFC4: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
            goto L_8016FF78;
    }
    // 0x8016FFC4: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8016FFC8: b           L_8017003C
    // 0x8016FFCC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8017003C;
    // 0x8016FFCC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8016FFD0: lh          $t5, 0x2($a3)
    ctx->r13 = MEM_H(ctx->r7, 0X2);
L_8016FFD4:
    // 0x8016FFD4: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x8016FFD8: addu        $v0, $a1, $t4
    ctx->r2 = ADD32(ctx->r5, ctx->r12);
    // 0x8016FFDC: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x8016FFE0: lwc1        $f6, 0x0($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8016FFE4: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8016FFE8: swc1        $f6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f6.u32l;
    // 0x8016FFEC: swc1        $f0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f0.u32l;
    // 0x8016FFF0: lh          $a0, 0x4($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X4);
    // 0x8016FFF4: swc1        $f0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f0.u32l;
    // 0x8016FFF8: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    // 0x8016FFFC: jal         0x8016FE68
    // 0x80170000: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_8016FE68(rdram, ctx);
        goto after_0;
    // 0x80170000: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x80170004: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80170008: lwc1        $f0, 0x2C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8017000C: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80170010: lwc1        $f10, 0xC($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80170014: lw          $a3, 0x34($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X34);
    // 0x80170018: swc1        $f0, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f0.u32l;
    // 0x8017001C: swc1        $f8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f8.u32l;
    // 0x80170020: swc1        $f10, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f10.u32l;
    // 0x80170024: jal         0x8016FE68
    // 0x80170028: lh          $a0, 0x6($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X6);
    func_8016FE68(rdram, ctx);
        goto after_1;
    // 0x80170028: lh          $a0, 0x6($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X6);
    after_1:
    // 0x8017002C: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80170030: lwc1        $f16, 0x38($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80170034: swc1        $f16, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f16.u32l;
L_80170038:
    // 0x80170038: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017003C:
    // 0x8017003C: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x80170040: jr          $ra
    // 0x80170044: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_8016FE68--;
    // --- END PATCH ---
#endif
    return;
    // 0x80170044: nop

;}
RECOMP_FUNC void func_80170048(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80170048: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017004C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80170050: addiu       $v0, $v0, -0x9FC
    ctx->r2 = ADD32(ctx->r2, -0X9FC);
    // 0x80170054: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80170058: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8017005C: lwc1        $f4, 0x18($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X18);
    // 0x80170060: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80170064: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80170068: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x8017006C: swc1        $f4, 0x5C($t6)
    MEM_W(0X5C, ctx->r14) = ctx->f4.u32l;
    // 0x80170070: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x80170074: lwc1        $f6, 0x1C($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X1C);
    // 0x80170078: addiu       $t5, $t5, -0x1580
    ctx->r13 = ADD32(ctx->r13, -0X1580);
    // 0x8017007C: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    // 0x80170080: swc1        $f6, 0x60($t7)
    MEM_W(0X60, ctx->r15) = ctx->f6.u32l;
    // 0x80170084: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x80170088: lwc1        $f8, 0x20($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X20);
    // 0x8017008C: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80170090: addiu       $a0, $a0, -0x978
    ctx->r4 = ADD32(ctx->r4, -0X978);
    // 0x80170094: swc1        $f8, 0x64($t8)
    MEM_W(0X64, ctx->r24) = ctx->f8.u32l;
    // 0x80170098: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X0);
    // 0x8017009C: lwc1        $f10, 0x24($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X24);
    // 0x801700A0: swc1        $f10, 0x68($t9)
    MEM_W(0X68, ctx->r25) = ctx->f10.u32l;
    // 0x801700A4: lw          $t0, 0x0($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X0);
    // 0x801700A8: lwc1        $f16, 0x28($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X28);
    // 0x801700AC: swc1        $f16, 0x6C($t0)
    MEM_W(0X6C, ctx->r8) = ctx->f16.u32l;
    // 0x801700B0: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X0);
    // 0x801700B4: lwc1        $f18, 0x2C($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X2C);
    // 0x801700B8: swc1        $f18, 0x70($t1)
    MEM_W(0X70, ctx->r9) = ctx->f18.u32l;
    // 0x801700BC: lw          $t2, 0x4($a1)
    ctx->r10 = MEM_W(ctx->r5, 0X4);
    // 0x801700C0: lw          $t4, 0x0($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X0);
    // 0x801700C4: lw          $t3, 0x18($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X18);
    // 0x801700C8: sw          $t3, 0x168($t4)
    MEM_W(0X168, ctx->r12) = ctx->r11;
    // 0x801700CC: sw          $zero, -0x1118($at)
    MEM_W(-0X1118, ctx->r1) = 0;
    // 0x801700D0: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801700D4: sw          $t5, 0x2494($at)
    MEM_W(0X2494, ctx->r1) = ctx->r13;
    // 0x801700D8: lw          $t6, 0x4($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X4);
    // 0x801700DC: lh          $a1, 0x1C($t6)
    ctx->r5 = MEM_H(ctx->r14, 0X1C);
    // 0x801700E0: jal         0x80000F30
    // 0x801700E4: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x801700E4: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    after_0:
    // 0x801700E8: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x801700EC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801700F0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801700F4: lw          $t7, 0x10($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X10);
    // 0x801700F8: jal         0x801701DC
    // 0x801700FC: sw          $t7, -0x10F4($at)
    MEM_W(-0X10F4, ctx->r1) = ctx->r15;
    func_801701DC(rdram, ctx);
        goto after_1;
    // 0x801700FC: sw          $t7, -0x10F4($at)
    MEM_W(-0X10F4, ctx->r1) = ctx->r15;
    after_1:
    // 0x80170100: jal         0x801703D8
    // 0x80170104: nop

    func_801703D8(rdram, ctx);
        goto after_2;
    // 0x80170104: nop

    after_2:
    // 0x80170108: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017010C: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x80170110: addiu       $t8, $t8, -0x1580
    ctx->r24 = ADD32(ctx->r24, -0X1580);
    // 0x80170114: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80170118: sw          $t8, 0x2494($at)
    MEM_W(0X2494, ctx->r1) = ctx->r24;
    // 0x8017011C: jr          $ra
    // 0x80170120: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80170120: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_80170124(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80170124: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80170128: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8017012C: addiu       $v0, $v0, -0x9FC
    ctx->r2 = ADD32(ctx->r2, -0X9FC);
    // 0x80170130: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80170134: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80170138: lwc1        $f4, 0x18($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X18);
    // 0x8017013C: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80170140: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80170144: swc1        $f4, 0x5C($t6)
    MEM_W(0X5C, ctx->r14) = ctx->f4.u32l;
    // 0x80170148: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x8017014C: lwc1        $f6, 0x1C($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X1C);
    // 0x80170150: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x80170154: addiu       $t6, $t6, -0x750
    ctx->r14 = ADD32(ctx->r14, -0X750);
    // 0x80170158: swc1        $f6, 0x60($t7)
    MEM_W(0X60, ctx->r15) = ctx->f6.u32l;
    // 0x8017015C: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x80170160: lwc1        $f8, 0x20($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X20);
    // 0x80170164: swc1        $f8, 0x64($t8)
    MEM_W(0X64, ctx->r24) = ctx->f8.u32l;
    // 0x80170168: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X0);
    // 0x8017016C: lwc1        $f10, 0x24($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X24);
    // 0x80170170: swc1        $f10, 0x68($t9)
    MEM_W(0X68, ctx->r25) = ctx->f10.u32l;
    // 0x80170174: lw          $t0, 0x0($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X0);
    // 0x80170178: lwc1        $f16, 0x28($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X28);
    // 0x8017017C: swc1        $f16, 0x6C($t0)
    MEM_W(0X6C, ctx->r8) = ctx->f16.u32l;
    // 0x80170180: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X0);
    // 0x80170184: lwc1        $f18, 0x2C($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X2C);
    // 0x80170188: swc1        $f18, 0x70($t1)
    MEM_W(0X70, ctx->r9) = ctx->f18.u32l;
    // 0x8017018C: lw          $t2, 0x4($a1)
    ctx->r10 = MEM_W(ctx->r5, 0X4);
    // 0x80170190: lw          $t4, 0x0($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X0);
    // 0x80170194: lw          $t3, 0x18($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X18);
    // 0x80170198: sw          $t3, 0x168($t4)
    MEM_W(0X168, ctx->r12) = ctx->r11;
    // 0x8017019C: lw          $t5, 0x4($a1)
    ctx->r13 = MEM_W(ctx->r5, 0X4);
    // 0x801701A0: lh          $v1, 0x1C($t5)
    ctx->r3 = MEM_H(ctx->r13, 0X1C);
    // 0x801701A4: beql        $v1, $zero, L_801701D0
    if (ctx->r3 == 0) {
        // 0x801701A8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801701D0;
    }
    goto skip_0;
    // 0x801701A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801701AC: sw          $v1, -0x1118($at)
    MEM_W(-0X1118, ctx->r1) = ctx->r3;
    // 0x801701B0: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801701B4: jal         0x801703D8
    // 0x801701B8: sw          $t6, 0x2494($at)
    MEM_W(0X2494, ctx->r1) = ctx->r14;
    func_801703D8(rdram, ctx);
        goto after_0;
    // 0x801701B8: sw          $t6, 0x2494($at)
    MEM_W(0X2494, ctx->r1) = ctx->r14;
    after_0:
    // 0x801701BC: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x801701C0: addiu       $t7, $t7, -0x1580
    ctx->r15 = ADD32(ctx->r15, -0X1580);
    // 0x801701C4: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801701C8: sw          $t7, 0x2494($at)
    MEM_W(0X2494, ctx->r1) = ctx->r15;
    // 0x801701CC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801701D0:
    // 0x801701D0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801701D4: jr          $ra
    // 0x801701D8: nop

    return;
    // 0x801701D8: nop

;}
RECOMP_FUNC void func_801701DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_801701DC is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_801701DC++;
    if (lod_text_guard_depth_801701DC > 128) {
        if (!lod_text_guard_logged_801701DC) {
            lod_text_guard_logged_801701DC = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_801701DC recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_801701DC--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x801701DC: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801701E0: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x801701E4: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801701E8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801701EC: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x801701F0: lwc1        $f6, 0x60($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X60);
    // 0x801701F4: lwc1        $f4, 0x12C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X12C);
    // 0x801701F8: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x801701FC: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80170200: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x80170204: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x80170208: bc1t        L_8017028C
    if (c1cs) {
        // 0x8017020C: nop
    
            goto L_8017028C;
    }
    // 0x8017020C: nop

    // 0x80170210: lwc1        $f8, 0x6C($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X6C);
    // 0x80170214: lwc1        $f10, 0x120($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X120);
    // 0x80170218: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8017021C: nop

    // 0x80170220: bc1t        L_8017028C
    if (c1cs) {
        // 0x80170224: nop
    
            goto L_8017028C;
    }
    // 0x80170224: nop

    // 0x80170228: lwc1        $f16, 0x128($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X128);
    // 0x8017022C: lwc1        $f18, 0x5C($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X5C);
    // 0x80170230: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x80170234: nop

    // 0x80170238: bc1t        L_8017028C
    if (c1cs) {
        // 0x8017023C: nop
    
            goto L_8017028C;
    }
    // 0x8017023C: nop

    // 0x80170240: lwc1        $f4, 0x68($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X68);
    // 0x80170244: lwc1        $f6, 0x11C($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x80170248: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x8017024C: nop

    // 0x80170250: bc1t        L_8017028C
    if (c1cs) {
        // 0x80170254: nop
    
            goto L_8017028C;
    }
    // 0x80170254: nop

    // 0x80170258: lwc1        $f8, 0x130($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X130);
    // 0x8017025C: lwc1        $f10, 0x64($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X64);
    // 0x80170260: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x80170264: nop

    // 0x80170268: bc1t        L_8017028C
    if (c1cs) {
        // 0x8017026C: nop
    
            goto L_8017028C;
    }
    // 0x8017026C: nop

    // 0x80170270: lwc1        $f16, 0x70($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X70);
    // 0x80170274: lwc1        $f18, 0x124($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X124);
    // 0x80170278: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x8017027C: c.lt.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl < ctx->f18.fl;
    // 0x80170280: nop

    // 0x80170284: bc1f        L_80170294
    if (!c1cs) {
        // 0x80170288: nop
    
            goto L_80170294;
    }
    // 0x80170288: nop

L_8017028C:
    // 0x8017028C: b           L_80170294
    // 0x80170290: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
        goto L_80170294;
    // 0x80170290: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80170294:
    // 0x80170294: beql        $v1, $zero, L_801703CC
    if (ctx->r3 == 0) {
        // 0x80170298: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801703CC;
    }
    goto skip_0;
    // 0x80170298: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x8017029C: lw          $t7, -0x10F4($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X10F4);
    // 0x801702A0: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x801702A4: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x801702A8: addu        $a3, $t6, $t7
    ctx->r7 = ADD32(ctx->r14, ctx->r15);
    // 0x801702AC: lh          $a2, 0x0($a3)
    ctx->r6 = MEM_H(ctx->r7, 0X0);
    // 0x801702B0: bnel        $a2, $at, L_80170340
    if (ctx->r6 != ctx->r1) {
        // 0x801702B4: lh          $t5, 0x2($a3)
        ctx->r13 = MEM_H(ctx->r7, 0X2);
            goto L_80170340;
    }
    goto skip_1;
    // 0x801702B4: lh          $t5, 0x2($a3)
    ctx->r13 = MEM_H(ctx->r7, 0X2);
    skip_1:
    // 0x801702B8: lh          $a2, 0x2($a3)
    ctx->r6 = MEM_H(ctx->r7, 0X2);
    // 0x801702BC: lw          $v0, 0x4($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X4);
    // 0x801702C0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x801702C4: blez        $a2, L_801703C8
    if (SIGNED(ctx->r6) <= 0) {
        // 0x801702C8: lui         $t2, 0x8019
        ctx->r10 = S32(0X8019 << 16);
            goto L_801703C8;
    }
    // 0x801702C8: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x801702CC: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x801702D0: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x801702D4: addiu       $a1, $a1, -0x1118
    ctx->r5 = ADD32(ctx->r5, -0X1118);
    // 0x801702D8: addiu       $t0, $t0, -0x978
    ctx->r8 = ADD32(ctx->r8, -0X978);
    // 0x801702DC: addiu       $t2, $t2, 0x2494
    ctx->r10 = ADD32(ctx->r10, 0X2494);
    // 0x801702E0: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
L_801702E4:
    // 0x801702E4: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x801702E8: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801702EC: addu        $a0, $t0, $t8
    ctx->r4 = ADD32(ctx->r8, ctx->r24);
    // 0x801702F0: lbu         $t9, 0x0($a0)
    ctx->r25 = MEM_BU(ctx->r4, 0X0);
    // 0x801702F4: bnel        $t9, $zero, L_8017032C
    if (ctx->r25 != 0) {
        // 0x801702F8: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_8017032C;
    }
    goto skip_2;
    // 0x801702F8: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_2:
    // 0x801702FC: lw          $t5, 0x0($a1)
    ctx->r13 = MEM_W(ctx->r5, 0X0);
    // 0x80170300: lw          $t4, 0x0($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X0);
    // 0x80170304: sb          $t1, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r9;
    // 0x80170308: lh          $t3, 0x0($v0)
    ctx->r11 = MEM_H(ctx->r2, 0X0);
    // 0x8017030C: sll         $t6, $t5, 1
    ctx->r14 = S32(ctx->r13 << 1);
    // 0x80170310: addu        $t7, $t4, $t6
    ctx->r15 = ADD32(ctx->r12, ctx->r14);
    // 0x80170314: sh          $t3, 0x0($t7)
    MEM_H(0X0, ctx->r15) = ctx->r11;
    // 0x80170318: lw          $t8, 0x0($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X0);
    // 0x8017031C: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x80170320: sw          $t9, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r25;
    // 0x80170324: lh          $a2, 0x2($a3)
    ctx->r6 = MEM_H(ctx->r7, 0X2);
    // 0x80170328: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
L_8017032C:
    // 0x8017032C: bne         $at, $zero, L_801702E4
    if (ctx->r1 != 0) {
        // 0x80170330: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
            goto L_801702E4;
    }
    // 0x80170330: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80170334: b           L_801703CC
    // 0x80170338: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801703CC;
    // 0x80170338: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017033C: lh          $t5, 0x2($a3)
    ctx->r13 = MEM_H(ctx->r7, 0X2);
L_80170340:
    // 0x80170340: sll         $a1, $a2, 2
    ctx->r5 = S32(ctx->r6 << 2);
    // 0x80170344: addu        $v1, $v0, $a1
    ctx->r3 = ADD32(ctx->r2, ctx->r5);
    // 0x80170348: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x8017034C: lwc1        $f6, 0x5C($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X5C);
    // 0x80170350: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80170354: swc1        $f6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f6.u32l;
    // 0x80170358: swc1        $f0, 0x5C($v1)
    MEM_W(0X5C, ctx->r3) = ctx->f0.u32l;
    // 0x8017035C: lh          $a0, 0x4($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X4);
    // 0x80170360: swc1        $f0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f0.u32l;
    // 0x80170364: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    // 0x80170368: jal         0x801701DC
    // 0x8017036C: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    func_801701DC(rdram, ctx);
        goto after_0;
    // 0x8017036C: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    after_0:
    // 0x80170370: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80170374: addiu       $v0, $v0, -0x9FC
    ctx->r2 = ADD32(ctx->r2, -0X9FC);
    // 0x80170378: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x8017037C: lw          $t4, 0x0($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X0);
    // 0x80170380: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80170384: lw          $a3, 0x34($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X34);
    // 0x80170388: lwc1        $f0, 0x2C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8017038C: addu        $t6, $t4, $a1
    ctx->r14 = ADD32(ctx->r12, ctx->r5);
    // 0x80170390: swc1        $f8, 0x5C($t6)
    MEM_W(0X5C, ctx->r14) = ctx->f8.u32l;
    // 0x80170394: lw          $t3, 0x0($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X0);
    // 0x80170398: addu        $v1, $t3, $a1
    ctx->r3 = ADD32(ctx->r11, ctx->r5);
    // 0x8017039C: lwc1        $f10, 0x68($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X68);
    // 0x801703A0: swc1        $f10, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f10.u32l;
    // 0x801703A4: swc1        $f0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f0.u32l;
    // 0x801703A8: jal         0x801701DC
    // 0x801703AC: lh          $a0, 0x6($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X6);
    func_801701DC(rdram, ctx);
        goto after_1;
    // 0x801703AC: lh          $a0, 0x6($a3)
    ctx->r4 = MEM_H(ctx->r7, 0X6);
    after_1:
    // 0x801703B0: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x801703B4: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x801703B8: lw          $t7, -0x9FC($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X9FC);
    // 0x801703BC: lwc1        $f16, 0x38($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X38);
    // 0x801703C0: addu        $t8, $t7, $a1
    ctx->r24 = ADD32(ctx->r15, ctx->r5);
    // 0x801703C4: swc1        $f16, 0x68($t8)
    MEM_W(0X68, ctx->r24) = ctx->f16.u32l;
L_801703C8:
    // 0x801703C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801703CC:
    // 0x801703CC: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x801703D0: jr          $ra
    // 0x801703D4: nop

#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_801701DC--;
    // --- END PATCH ---
#endif
    return;
    // 0x801703D4: nop

;}
RECOMP_FUNC void func_801703D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801703D8: addiu       $sp, $sp, -0x110
    ctx->r29 = ADD32(ctx->r29, -0X110);
    // 0x801703DC: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801703E0: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x801703E4: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801703E8: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x801703EC: addiu       $t6, $v0, 0x11C
    ctx->r14 = ADD32(ctx->r2, 0X11C);
    // 0x801703F0: addiu       $t7, $v0, 0x128
    ctx->r15 = ADD32(ctx->r2, 0X128);
    // 0x801703F4: sw          $ra, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r31;
    // 0x801703F8: sw          $fp, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r30;
    // 0x801703FC: sw          $s7, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r23;
    // 0x80170400: sw          $s6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r22;
    // 0x80170404: sw          $s5, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r21;
    // 0x80170408: sw          $s4, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r20;
    // 0x8017040C: sw          $s3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r19;
    // 0x80170410: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x80170414: sw          $s1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r17;
    // 0x80170418: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x8017041C: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80170420: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80170424: sw          $zero, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = 0;
    // 0x80170428: sw          $t6, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->r14;
    // 0x8017042C: blez        $v1, L_80170E5C
    if (SIGNED(ctx->r3) <= 0) {
        // 0x80170430: sw          $t7, 0x90($sp)
        MEM_W(0X90, ctx->r29) = ctx->r15;
            goto L_80170E5C;
    }
    // 0x80170430: sw          $t7, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r15;
    // 0x80170434: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80170438: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x8017043C: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x80170440: addiu       $s2, $s2, -0x678
    ctx->r18 = ADD32(ctx->r18, -0X678);
    // 0x80170444: lwc1        $f20, -0x50FC($at)
    ctx->f20.u32l = MEM_W(ctx->r1, -0X50FC);
    // 0x80170448: sw          $zero, 0x64($sp)
    MEM_W(0X64, ctx->r29) = 0;
    // 0x8017044C: addiu       $fp, $zero, 0xC8
    ctx->r30 = ADD32(0, 0XC8);
    // 0x80170450: addiu       $s7, $zero, 0xC8
    ctx->r23 = ADD32(0, 0XC8);
    // 0x80170454: addiu       $s6, $zero, 0xC8
    ctx->r22 = ADD32(0, 0XC8);
    // 0x80170458: addiu       $s5, $zero, 0xC8
    ctx->r21 = ADD32(0, 0XC8);
    // 0x8017045C: addiu       $s4, $zero, 0xC8
    ctx->r20 = ADD32(0, 0XC8);
    // 0x80170460: addiu       $s3, $sp, 0xCC
    ctx->r19 = ADD32(ctx->r29, 0XCC);
L_80170464:
    // 0x80170464: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80170468: lw          $t8, 0x2494($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X2494);
    // 0x8017046C: lw          $t9, 0x64($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X64);
    // 0x80170470: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x80170474: lw          $t5, -0x9FC($t5)
    ctx->r13 = MEM_W(ctx->r13, -0X9FC);
    // 0x80170478: addu        $t2, $t8, $t9
    ctx->r10 = ADD32(ctx->r24, ctx->r25);
    // 0x8017047C: lh          $t3, 0x0($t2)
    ctx->r11 = MEM_H(ctx->r10, 0X0);
    // 0x80170480: lw          $t6, 0x168($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X168);
    // 0x80170484: lw          $t7, 0x90($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X90);
    // 0x80170488: sll         $t4, $t3, 6
    ctx->r12 = S32(ctx->r11 << 6);
    // 0x8017048C: addu        $s1, $t4, $t6
    ctx->r17 = ADD32(ctx->r12, ctx->r14);
    // 0x80170490: lh          $t8, 0x2C($s1)
    ctx->r24 = MEM_H(ctx->r17, 0X2C);
    // 0x80170494: lwc1        $f4, 0x4($t7)
    ctx->f4.u32l = MEM_W(ctx->r15, 0X4);
    // 0x80170498: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x8017049C: nop

    // 0x801704A0: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801704A4: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x801704A8: nop

    // 0x801704AC: bc1t        L_80170570
    if (c1cs) {
        // 0x801704B0: nop
    
            goto L_80170570;
    }
    // 0x801704B0: nop

    // 0x801704B4: lh          $t9, 0x2E($s1)
    ctx->r25 = MEM_H(ctx->r17, 0X2E);
    // 0x801704B8: lw          $t2, 0x94($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X94);
    // 0x801704BC: mtc1        $t9, $f10
    ctx->f10.u32l = ctx->r25;
    // 0x801704C0: lwc1        $f4, 0x4($t2)
    ctx->f4.u32l = MEM_W(ctx->r10, 0X4);
    // 0x801704C4: cvt.s.w     $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801704C8: c.lt.s      $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f6.fl < ctx->f4.fl;
    // 0x801704CC: nop

    // 0x801704D0: bc1t        L_80170570
    if (c1cs) {
        // 0x801704D4: nop
    
            goto L_80170570;
    }
    // 0x801704D4: nop

    // 0x801704D8: lh          $t3, 0x30($s1)
    ctx->r11 = MEM_H(ctx->r17, 0X30);
    // 0x801704DC: lwc1        $f8, 0x0($t7)
    ctx->f8.u32l = MEM_W(ctx->r15, 0X0);
    // 0x801704E0: mtc1        $t3, $f10
    ctx->f10.u32l = ctx->r11;
    // 0x801704E4: nop

    // 0x801704E8: cvt.s.w     $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801704EC: c.lt.s      $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f8.fl < ctx->f6.fl;
    // 0x801704F0: nop

    // 0x801704F4: bc1t        L_80170570
    if (c1cs) {
        // 0x801704F8: nop
    
            goto L_80170570;
    }
    // 0x801704F8: nop

    // 0x801704FC: lh          $t5, 0x32($s1)
    ctx->r13 = MEM_H(ctx->r17, 0X32);
    // 0x80170500: lwc1        $f8, 0x0($t2)
    ctx->f8.u32l = MEM_W(ctx->r10, 0X0);
    // 0x80170504: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x80170508: nop

    // 0x8017050C: cvt.s.w     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80170510: c.lt.s      $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f10.fl < ctx->f8.fl;
    // 0x80170514: nop

    // 0x80170518: bc1t        L_80170570
    if (c1cs) {
        // 0x8017051C: nop
    
            goto L_80170570;
    }
    // 0x8017051C: nop

    // 0x80170520: lh          $t4, 0x34($s1)
    ctx->r12 = MEM_H(ctx->r17, 0X34);
    // 0x80170524: lwc1        $f6, 0x8($t7)
    ctx->f6.u32l = MEM_W(ctx->r15, 0X8);
    // 0x80170528: mtc1        $t4, $f4
    ctx->f4.u32l = ctx->r12;
    // 0x8017052C: nop

    // 0x80170530: cvt.s.w     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80170534: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x80170538: nop

    // 0x8017053C: bc1t        L_80170570
    if (c1cs) {
        // 0x80170540: nop
    
            goto L_80170570;
    }
    // 0x80170540: nop

    // 0x80170544: lh          $t6, 0x36($s1)
    ctx->r14 = MEM_H(ctx->r17, 0X36);
    // 0x80170548: lwc1        $f6, 0x8($t2)
    ctx->f6.u32l = MEM_W(ctx->r10, 0X8);
    // 0x8017054C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80170550: mtc1        $t6, $f8
    ctx->f8.u32l = ctx->r14;
    // 0x80170554: addiu       $a1, $sp, 0xE4
    ctx->r5 = ADD32(ctx->r29, 0XE4);
    // 0x80170558: addiu       $a2, $sp, 0xD8
    ctx->r6 = ADD32(ctx->r29, 0XD8);
    // 0x8017055C: cvt.s.w     $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80170560: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x80170564: nop

    // 0x80170568: bc1f        L_80170578
    if (!c1cs) {
        // 0x8017056C: nop
    
            goto L_80170578;
    }
    // 0x8017056C: nop

L_80170570:
    // 0x80170570: b           L_80170E48
    // 0x80170574: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170574: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170578:
    // 0x80170578: jal         0x801711F8
    // 0x8017057C: addiu       $a3, $sp, 0xF0
    ctx->r7 = ADD32(ctx->r29, 0XF0);
    func_801711F8(rdram, ctx);
        goto after_0;
    // 0x8017057C: addiu       $a3, $sp, 0xF0
    ctx->r7 = ADD32(ctx->r29, 0XF0);
    after_0:
    // 0x80170580: beq         $v0, $zero, L_80170598
    if (ctx->r2 == 0) {
        // 0x80170584: lui         $a0, 0x801A
        ctx->r4 = S32(0X801A << 16);
            goto L_80170598;
    }
    // 0x80170584: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80170588: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8017058C: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170590: b           L_80170E48
    // 0x80170594: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170594: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170598:
    // 0x80170598: lw          $a0, -0x9FC($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X9FC);
    // 0x8017059C: addiu       $a1, $sp, 0xE4
    ctx->r5 = ADD32(ctx->r29, 0XE4);
    // 0x801705A0: addiu       $a2, $sp, 0xC0
    ctx->r6 = ADD32(ctx->r29, 0XC0);
    // 0x801705A4: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x801705A8: jal         0x80173524
    // 0x801705AC: addiu       $a0, $a0, 0x1E4
    ctx->r4 = ADD32(ctx->r4, 0X1E4);
    func_80173524(rdram, ctx);
        goto after_1;
    // 0x801705AC: addiu       $a0, $a0, 0x1E4
    ctx->r4 = ADD32(ctx->r4, 0X1E4);
    after_1:
    // 0x801705B0: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x801705B4: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x801705B8: lwc1        $f10, 0xC0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XC0);
    // 0x801705BC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x801705C0: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x801705C4: addiu       $a2, $sp, 0x80
    ctx->r6 = ADD32(ctx->r29, 0X80);
    // 0x801705C8: addiu       $a0, $v0, 0x1E4
    ctx->r4 = ADD32(ctx->r2, 0X1E4);
    // 0x801705CC: lh          $t9, 0x0($t8)
    ctx->r25 = MEM_H(ctx->r24, 0X0);
    // 0x801705D0: mtc1        $t9, $f8
    ctx->f8.u32l = ctx->r25;
    // 0x801705D4: nop

    // 0x801705D8: cvt.s.w     $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801705DC: lwc1        $f8, 0xC4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XC4);
    // 0x801705E0: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x801705E4: swc1        $f6, 0xC0($sp)
    MEM_W(0XC0, ctx->r29) = ctx->f6.u32l;
    // 0x801705E8: lw          $t3, 0x0($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X0);
    // 0x801705EC: lh          $t5, 0x2($t3)
    ctx->r13 = MEM_H(ctx->r11, 0X2);
    // 0x801705F0: mtc1        $t5, $f10
    ctx->f10.u32l = ctx->r13;
    // 0x801705F4: nop

    // 0x801705F8: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801705FC: lwc1        $f10, 0xC8($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XC8);
    // 0x80170600: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80170604: swc1        $f6, 0xC4($sp)
    MEM_W(0XC4, ctx->r29) = ctx->f6.u32l;
    // 0x80170608: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x8017060C: lh          $t4, 0x4($t7)
    ctx->r12 = MEM_H(ctx->r15, 0X4);
    // 0x80170610: mtc1        $t4, $f8
    ctx->f8.u32l = ctx->r12;
    // 0x80170614: nop

    // 0x80170618: cvt.s.w     $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8017061C: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80170620: jal         0x80173524
    // 0x80170624: swc1        $f6, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f6.u32l;
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x80170624: swc1        $f6, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f6.u32l;
    after_2:
    // 0x80170628: lwc1        $f8, 0x80($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X80);
    // 0x8017062C: lwc1        $f10, 0xC0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XC0);
    // 0x80170630: lwc1        $f6, 0x88($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X88);
    // 0x80170634: mul.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80170638: lwc1        $f8, 0xC8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XC8);
    // 0x8017063C: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80170640: lwc1        $f8, 0xC4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XC4);
    // 0x80170644: add.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x80170648: lwc1        $f4, 0x84($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X84);
    // 0x8017064C: mul.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x80170650: add.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x80170654: neg.s       $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = -ctx->f8.fl;
    // 0x80170658: swc1        $f4, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->f4.u32l;
    // 0x8017065C: jal         0x80166F40
    // 0x80170660: lbu         $a0, 0x2B($s1)
    ctx->r4 = MEM_BU(ctx->r17, 0X2B);
    func_80166F40(rdram, ctx);
        goto after_3;
    // 0x80170660: lbu         $a0, 0x2B($s1)
    ctx->r4 = MEM_BU(ctx->r17, 0X2B);
    after_3:
    // 0x80170664: bne         $v0, $zero, L_8017067C
    if (ctx->r2 != 0) {
        // 0x80170668: addiu       $a1, $sp, 0xD8
        ctx->r5 = ADD32(ctx->r29, 0XD8);
            goto L_8017067C;
    }
    // 0x80170668: addiu       $a1, $sp, 0xD8
    ctx->r5 = ADD32(ctx->r29, 0XD8);
    // 0x8017066C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80170670: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x80170674: b           L_80170688
    // 0x80170678: addiu       $t0, $v0, 0x230
    ctx->r8 = ADD32(ctx->r2, 0X230);
        goto L_80170688;
    // 0x80170678: addiu       $t0, $v0, 0x230
    ctx->r8 = ADD32(ctx->r2, 0X230);
L_8017067C:
    // 0x8017067C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80170680: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x80170684: addiu       $t0, $v0, 0x48C
    ctx->r8 = ADD32(ctx->r2, 0X48C);
L_80170688:
    // 0x80170688: addiu       $a0, $v0, 0x1E4
    ctx->r4 = ADD32(ctx->r2, 0X1E4);
    // 0x8017068C: addiu       $a2, $sp, 0xCC
    ctx->r6 = ADD32(ctx->r29, 0XCC);
    // 0x80170690: jal         0x80173524
    // 0x80170694: sw          $t0, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->r8;
    func_80173524(rdram, ctx);
        goto after_4;
    // 0x80170694: sw          $t0, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->r8;
    after_4:
    // 0x80170698: lw          $t0, 0x98($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X98);
    // 0x8017069C: or          $v1, $t0, $zero
    ctx->r3 = ctx->r8 | 0;
L_801706A0:
    // 0x801706A0: lh          $t6, 0x60($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X60);
    // 0x801706A4: or          $a3, $v1, $zero
    ctx->r7 = ctx->r3 | 0;
    // 0x801706A8: lwc1        $f10, 0xCC($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XCC);
    // 0x801706AC: beql        $t6, $zero, L_8017072C
    if (ctx->r14 == 0) {
        // 0x801706B0: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_8017072C;
    }
    goto skip_0;
    // 0x801706B0: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_0:
    // 0x801706B4: lwc1        $f2, 0x20($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X20);
    // 0x801706B8: lwc1        $f6, 0xCC($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XCC);
    // 0x801706BC: lwc1        $f8, 0xD4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XD4);
    // 0x801706C0: c.lt.s      $f10, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f10.fl < ctx->f2.fl;
    // 0x801706C4: nop

    // 0x801706C8: bc1fl       L_801706E0
    if (!c1cs) {
        // 0x801706CC: sub.s       $f0, $f6, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f2.fl;
            goto L_801706E0;
    }
    goto skip_1;
    // 0x801706CC: sub.s       $f0, $f6, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f2.fl;
    skip_1:
    // 0x801706D0: sub.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f2.fl;
    // 0x801706D4: b           L_801706E0
    // 0x801706D8: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
        goto L_801706E0;
    // 0x801706D8: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x801706DC: sub.s       $f0, $f6, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f2.fl;
L_801706E0:
    // 0x801706E0: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x801706E4: nop

    // 0x801706E8: bc1fl       L_8017072C
    if (!c1cs) {
        // 0x801706EC: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_8017072C;
    }
    goto skip_2;
    // 0x801706EC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_2:
    // 0x801706F0: lwc1        $f2, 0x28($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X28);
    // 0x801706F4: lwc1        $f4, 0xD4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XD4);
    // 0x801706F8: c.lt.s      $f8, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f8.fl < ctx->f2.fl;
    // 0x801706FC: nop

    // 0x80170700: bc1fl       L_80170718
    if (!c1cs) {
        // 0x80170704: sub.s       $f0, $f4, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f2.fl;
            goto L_80170718;
    }
    goto skip_3;
    // 0x80170704: sub.s       $f0, $f4, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f2.fl;
    skip_3:
    // 0x80170708: sub.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x8017070C: b           L_80170718
    // 0x80170710: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
        goto L_80170718;
    // 0x80170710: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x80170714: sub.s       $f0, $f4, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f2.fl;
L_80170718:
    // 0x80170718: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x8017071C: nop

    // 0x80170720: bc1tl       L_8017073C
    if (c1cs) {
        // 0x80170724: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_8017073C;
    }
    goto skip_4;
    // 0x80170724: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    skip_4:
    // 0x80170728: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_8017072C:
    // 0x8017072C: slti        $at, $s0, 0x2
    ctx->r1 = SIGNED(ctx->r16) < 0X2 ? 1 : 0;
    // 0x80170730: bne         $at, $zero, L_801706A0
    if (ctx->r1 != 0) {
        // 0x80170734: addiu       $v1, $v1, 0x64
        ctx->r3 = ADD32(ctx->r3, 0X64);
            goto L_801706A0;
    }
    // 0x80170734: addiu       $v1, $v1, 0x64
    ctx->r3 = ADD32(ctx->r3, 0X64);
    // 0x80170738: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
L_8017073C:
    // 0x8017073C: beq         $s0, $at, L_8017076C
    if (ctx->r16 == ctx->r1) {
        // 0x80170740: lwc1        $f10, 0xF0($sp)
        ctx->f10.u32l = MEM_W(ctx->r29, 0XF0);
            goto L_8017076C;
    }
    // 0x80170740: lwc1        $f10, 0xF0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XF0);
    // 0x80170744: lwc1        $f6, 0x18($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X18);
    // 0x80170748: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x8017074C: addiu       $s0, $zero, 0xA
    ctx->r16 = ADD32(0, 0XA);
    // 0x80170750: c.le.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl <= ctx->f10.fl;
    // 0x80170754: nop

    // 0x80170758: bc1f        L_8017076C
    if (!c1cs) {
        // 0x8017075C: nop
    
            goto L_8017076C;
    }
    // 0x8017075C: nop

    // 0x80170760: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170764: b           L_80170E48
    // 0x80170768: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170768: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_8017076C:
    // 0x8017076C: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x80170770: beq         $s0, $at, L_801708DC
    if (ctx->r16 == ctx->r1) {
        // 0x80170774: or          $v1, $t0, $zero
        ctx->r3 = ctx->r8 | 0;
            goto L_801708DC;
    }
    // 0x80170774: or          $v1, $t0, $zero
    ctx->r3 = ctx->r8 | 0;
    // 0x80170778: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_8017077C:
    // 0x8017077C: lh          $t2, 0x60($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X60);
    // 0x80170780: or          $a3, $v1, $zero
    ctx->r7 = ctx->r3 | 0;
    // 0x80170784: lwc1        $f16, 0x80($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X80);
    // 0x80170788: beq         $t2, $zero, L_8017086C
    if (ctx->r10 == 0) {
        // 0x8017078C: sll         $t8, $s0, 4
        ctx->r24 = S32(ctx->r16 << 4);
            goto L_8017086C;
    }
    // 0x8017078C: sll         $t8, $s0, 4
    ctx->r24 = S32(ctx->r16 << 4);
    // 0x80170790: addu        $v0, $s2, $t8
    ctx->r2 = ADD32(ctx->r18, ctx->r24);
    // 0x80170794: lwc1        $f2, 0x0($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80170798: lwc1        $f12, 0x8C($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X8C);
    // 0x8017079C: lwc1        $f14, 0x84($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X84);
    // 0x801707A0: c.lt.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl < ctx->f2.fl;
    // 0x801707A4: lwc1        $f18, 0x88($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X88);
    // 0x801707A8: bc1fl       L_801707C0
    if (!c1cs) {
        // 0x801707AC: sub.s       $f0, $f16, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f16.fl - ctx->f2.fl;
            goto L_801707C0;
    }
    goto skip_5;
    // 0x801707AC: sub.s       $f0, $f16, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f16.fl - ctx->f2.fl;
    skip_5:
    // 0x801707B0: sub.s       $f0, $f16, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f16.fl - ctx->f2.fl;
    // 0x801707B4: b           L_801707C0
    // 0x801707B8: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
        goto L_801707C0;
    // 0x801707B8: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x801707BC: sub.s       $f0, $f16, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f16.fl - ctx->f2.fl;
L_801707C0:
    // 0x801707C0: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x801707C4: nop

    // 0x801707C8: bc1fl       L_80170870
    if (!c1cs) {
        // 0x801707CC: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80170870;
    }
    goto skip_6;
    // 0x801707CC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_6:
    // 0x801707D0: lwc1        $f2, 0x4($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801707D4: c.lt.s      $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f14.fl < ctx->f2.fl;
    // 0x801707D8: nop

    // 0x801707DC: bc1fl       L_801707F4
    if (!c1cs) {
        // 0x801707E0: sub.s       $f0, $f14, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f2.fl;
            goto L_801707F4;
    }
    goto skip_7;
    // 0x801707E0: sub.s       $f0, $f14, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f2.fl;
    skip_7:
    // 0x801707E4: sub.s       $f0, $f14, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f2.fl;
    // 0x801707E8: b           L_801707F4
    // 0x801707EC: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
        goto L_801707F4;
    // 0x801707EC: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x801707F0: sub.s       $f0, $f14, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = ctx->f14.fl - ctx->f2.fl;
L_801707F4:
    // 0x801707F4: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x801707F8: nop

    // 0x801707FC: bc1fl       L_80170870
    if (!c1cs) {
        // 0x80170800: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80170870;
    }
    goto skip_8;
    // 0x80170800: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_8:
    // 0x80170804: lwc1        $f0, 0x8($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80170808: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x8017080C: nop

    // 0x80170810: bc1fl       L_80170828
    if (!c1cs) {
        // 0x80170814: sub.s       $f2, $f18, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f18.fl - ctx->f0.fl;
            goto L_80170828;
    }
    goto skip_9;
    // 0x80170814: sub.s       $f2, $f18, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f18.fl - ctx->f0.fl;
    skip_9:
    // 0x80170818: sub.s       $f2, $f18, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f18.fl - ctx->f0.fl;
    // 0x8017081C: b           L_80170828
    // 0x80170820: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
        goto L_80170828;
    // 0x80170820: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
    // 0x80170824: sub.s       $f2, $f18, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f18.fl - ctx->f0.fl;
L_80170828:
    // 0x80170828: c.lt.s      $f2, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f2.fl < ctx->f20.fl;
    // 0x8017082C: nop

    // 0x80170830: bc1fl       L_80170870
    if (!c1cs) {
        // 0x80170834: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80170870;
    }
    goto skip_10;
    // 0x80170834: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_10:
    // 0x80170838: lwc1        $f0, 0xC($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0XC);
    // 0x8017083C: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x80170840: nop

    // 0x80170844: bc1fl       L_8017085C
    if (!c1cs) {
        // 0x80170848: sub.s       $f2, $f12, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f12.fl - ctx->f0.fl;
            goto L_8017085C;
    }
    goto skip_11;
    // 0x80170848: sub.s       $f2, $f12, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f12.fl - ctx->f0.fl;
    skip_11:
    // 0x8017084C: sub.s       $f2, $f12, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f12.fl - ctx->f0.fl;
    // 0x80170850: b           L_8017085C
    // 0x80170854: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
        goto L_8017085C;
    // 0x80170854: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
    // 0x80170858: sub.s       $f2, $f12, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f12.fl - ctx->f0.fl;
L_8017085C:
    // 0x8017085C: c.lt.s      $f2, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f2.fl < ctx->f20.fl;
    // 0x80170860: nop

    // 0x80170864: bc1tl       L_80170880
    if (c1cs) {
        // 0x80170868: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_80170880;
    }
    goto skip_12;
    // 0x80170868: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    skip_12:
L_8017086C:
    // 0x8017086C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80170870:
    // 0x80170870: slti        $at, $s0, 0x2
    ctx->r1 = SIGNED(ctx->r16) < 0X2 ? 1 : 0;
    // 0x80170874: bne         $at, $zero, L_8017077C
    if (ctx->r1 != 0) {
        // 0x80170878: addiu       $v1, $v1, 0x64
        ctx->r3 = ADD32(ctx->r3, 0X64);
            goto L_8017077C;
    }
    // 0x80170878: addiu       $v1, $v1, 0x64
    ctx->r3 = ADD32(ctx->r3, 0X64);
    // 0x8017087C: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
L_80170880:
    // 0x80170880: beq         $s0, $at, L_801708DC
    if (ctx->r16 == ctx->r1) {
        // 0x80170884: lwc1        $f8, 0xF0($sp)
        ctx->f8.u32l = MEM_W(ctx->r29, 0XF0);
            goto L_801708DC;
    }
    // 0x80170884: lwc1        $f8, 0xF0($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XF0);
    // 0x80170888: lwc1        $f4, 0x18($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X18);
    // 0x8017088C: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170890: lui         $t9, 0x801A
    ctx->r25 = S32(0X801A << 16);
    // 0x80170894: c.le.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl <= ctx->f8.fl;
    // 0x80170898: nop

    // 0x8017089C: bc1f        L_801708B0
    if (!c1cs) {
        // 0x801708A0: nop
    
            goto L_801708B0;
    }
    // 0x801708A0: nop

    // 0x801708A4: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x801708A8: b           L_80170E48
    // 0x801708AC: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x801708AC: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_801708B0:
    // 0x801708B0: lh          $t9, -0x680($t9)
    ctx->r25 = MEM_H(ctx->r25, -0X680);
    // 0x801708B4: addiu       $s0, $zero, 0xA
    ctx->r16 = ADD32(0, 0XA);
    // 0x801708B8: beq         $t9, $zero, L_801708DC
    if (ctx->r25 == 0) {
        // 0x801708BC: nop
    
            goto L_801708DC;
    }
    // 0x801708BC: nop

    // 0x801708C0: lh          $t3, 0x5E($a3)
    ctx->r11 = MEM_H(ctx->r7, 0X5E);
    // 0x801708C4: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801708C8: bne         $t3, $zero, L_801708DC
    if (ctx->r11 != 0) {
        // 0x801708CC: nop
    
            goto L_801708DC;
    }
    // 0x801708CC: nop

    // 0x801708D0: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x801708D4: b           L_80170E48
    // 0x801708D8: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x801708D8: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_801708DC:
    // 0x801708DC: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x801708E0: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x801708E4: bne         $s0, $at, L_80170C30
    if (ctx->r16 != ctx->r1) {
        // 0x801708E8: lh          $t1, -0x680($t1)
        ctx->r9 = MEM_H(ctx->r9, -0X680);
            goto L_80170C30;
    }
    // 0x801708E8: lh          $t1, -0x680($t1)
    ctx->r9 = MEM_H(ctx->r9, -0X680);
    // 0x801708EC: bne         $t1, $zero, L_80170994
    if (ctx->r9 != 0) {
        // 0x801708F0: or          $a2, $zero, $zero
        ctx->r6 = 0 | 0;
            goto L_80170994;
    }
    // 0x801708F0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801708F4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801708F8: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
L_801708FC:
    // 0x801708FC: lh          $t5, 0x60($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X60);
    // 0x80170900: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
    // 0x80170904: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80170908: beq         $t5, $zero, L_80170984
    if (ctx->r13 == 0) {
        // 0x8017090C: nop
    
            goto L_80170984;
    }
    // 0x8017090C: nop

    // 0x80170910: lh          $t7, 0x5E($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X5E);
    // 0x80170914: lwc1        $f10, 0x80($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X80);
    // 0x80170918: beq         $t7, $zero, L_80170984
    if (ctx->r15 == 0) {
        // 0x8017091C: nop
    
            goto L_80170984;
    }
    // 0x8017091C: nop

    // 0x80170920: lwc1        $f6, 0x38($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X38);
    // 0x80170924: lwc1        $f4, 0x84($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X84);
    // 0x80170928: mul.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x8017092C: lwc1        $f10, 0x3C($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X3C);
    // 0x80170930: mul.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f10.fl);
    // 0x80170934: lwc1        $f10, 0x88($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X88);
    // 0x80170938: add.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x8017093C: lwc1        $f8, 0x40($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X40);
    // 0x80170940: mul.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f8.fl);
    // 0x80170944: lwc1        $f8, 0x8C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X8C);
    // 0x80170948: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017094C: add.s       $f2, $f8, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80170950: c.lt.s      $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f2.fl < ctx->f22.fl;
    // 0x80170954: nop

    // 0x80170958: bc1fl       L_8017096C
    if (!c1cs) {
        // 0x8017095C: mov.s       $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
            goto L_8017096C;
    }
    goto skip_13;
    // 0x8017095C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    skip_13:
    // 0x80170960: b           L_8017096C
    // 0x80170964: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
        goto L_8017096C;
    // 0x80170964: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x80170968: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_8017096C:
    // 0x8017096C: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x80170970: nop

    // 0x80170974: bc1f        L_80170984
    if (!c1cs) {
        // 0x80170978: nop
    
            goto L_80170984;
    }
    // 0x80170978: nop

    // 0x8017097C: b           L_80170A2C
    // 0x80170980: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
        goto L_80170A2C;
    // 0x80170980: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
L_80170984:
    // 0x80170984: bne         $a0, $s7, L_801708FC
    if (ctx->r4 != ctx->r23) {
        // 0x80170988: addiu       $v0, $v0, 0x64
        ctx->r2 = ADD32(ctx->r2, 0X64);
            goto L_801708FC;
    }
    // 0x80170988: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
    // 0x8017098C: b           L_80170A2C
    // 0x80170990: nop

        goto L_80170A2C;
    // 0x80170990: nop

L_80170994:
    // 0x80170994: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80170998: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
L_8017099C:
    // 0x8017099C: lh          $t4, 0x60($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X60);
    // 0x801709A0: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
    // 0x801709A4: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x801709A8: beq         $t4, $zero, L_80170A24
    if (ctx->r12 == 0) {
        // 0x801709AC: nop
    
            goto L_80170A24;
    }
    // 0x801709AC: nop

    // 0x801709B0: lh          $t6, 0x5E($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X5E);
    // 0x801709B4: lwc1        $f6, 0xE4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XE4);
    // 0x801709B8: bne         $t6, $zero, L_80170A24
    if (ctx->r14 != 0) {
        // 0x801709BC: nop
    
            goto L_80170A24;
    }
    // 0x801709BC: nop

    // 0x801709C0: lwc1        $f4, 0x48($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X48);
    // 0x801709C4: lwc1        $f10, 0x4C($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4C);
    // 0x801709C8: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801709CC: lwc1        $f4, 0xE8($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XE8);
    // 0x801709D0: mul.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x801709D4: lwc1        $f4, 0x50($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X50);
    // 0x801709D8: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x801709DC: lwc1        $f8, 0xEC($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XEC);
    // 0x801709E0: mul.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x801709E4: lwc1        $f8, 0x54($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X54);
    // 0x801709E8: add.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x801709EC: add.s       $f2, $f8, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x801709F0: c.lt.s      $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f2.fl < ctx->f22.fl;
    // 0x801709F4: nop

    // 0x801709F8: bc1fl       L_80170A0C
    if (!c1cs) {
        // 0x801709FC: mov.s       $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
            goto L_80170A0C;
    }
    goto skip_14;
    // 0x801709FC: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    skip_14:
    // 0x80170A00: b           L_80170A0C
    // 0x80170A04: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
        goto L_80170A0C;
    // 0x80170A04: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x80170A08: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_80170A0C:
    // 0x80170A0C: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x80170A10: nop

    // 0x80170A14: bc1f        L_80170A24
    if (!c1cs) {
        // 0x80170A18: nop
    
            goto L_80170A24;
    }
    // 0x80170A18: nop

    // 0x80170A1C: b           L_80170A2C
    // 0x80170A20: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
        goto L_80170A2C;
    // 0x80170A20: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
L_80170A24:
    // 0x80170A24: bne         $a0, $fp, L_8017099C
    if (ctx->r4 != ctx->r30) {
        // 0x80170A28: addiu       $v0, $v0, 0x64
        ctx->r2 = ADD32(ctx->r2, 0X64);
            goto L_8017099C;
    }
    // 0x80170A28: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
L_80170A2C:
    // 0x80170A2C: beq         $a2, $zero, L_80170ACC
    if (ctx->r6 == 0) {
        // 0x80170A30: nop
    
            goto L_80170ACC;
    }
    // 0x80170A30: nop

    // 0x80170A34: beql        $t1, $zero, L_80170A68
    if (ctx->r9 == 0) {
        // 0x80170A38: lh          $t8, 0x60($a1)
        ctx->r24 = MEM_H(ctx->r5, 0X60);
            goto L_80170A68;
    }
    goto skip_15;
    // 0x80170A38: lh          $t8, 0x60($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X60);
    skip_15:
    // 0x80170A3C: lh          $t2, 0xC4($t0)
    ctx->r10 = MEM_H(ctx->r8, 0XC4);
    // 0x80170A40: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170A44: addiu       $a3, $t0, 0x64
    ctx->r7 = ADD32(ctx->r8, 0X64);
    // 0x80170A48: beq         $t2, $zero, L_80170A5C
    if (ctx->r10 == 0) {
        // 0x80170A4C: nop
    
            goto L_80170A5C;
    }
    // 0x80170A4C: nop

    // 0x80170A50: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170A54: b           L_80170E48
    // 0x80170A58: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170A58: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170A5C:
    // 0x80170A5C: b           L_80170C30
    // 0x80170A60: addiu       $s0, $zero, 0x2
    ctx->r16 = ADD32(0, 0X2);
        goto L_80170C30;
    // 0x80170A60: addiu       $s0, $zero, 0x2
    ctx->r16 = ADD32(0, 0X2);
    // 0x80170A64: lh          $t8, 0x60($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X60);
L_80170A68:
    // 0x80170A68: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x80170A6C: beq         $t8, $zero, L_80170AC4
    if (ctx->r24 == 0) {
        // 0x80170A70: nop
    
            goto L_80170AC4;
    }
    // 0x80170A70: nop

    // 0x80170A74: lh          $t9, 0x5E($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X5E);
    // 0x80170A78: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80170A7C: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80170A80: bne         $t9, $zero, L_80170AC4
    if (ctx->r25 != 0) {
        // 0x80170A84: addiu       $at, $zero, 0xC8
        ctx->r1 = ADD32(0, 0XC8);
            goto L_80170AC4;
    }
    // 0x80170A84: addiu       $at, $zero, 0xC8
    ctx->r1 = ADD32(0, 0XC8);
L_80170A88:
    // 0x80170A88: lh          $v1, 0x60($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X60);
    // 0x80170A8C: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80170A90: beq         $v1, $zero, L_80170AB8
    if (ctx->r3 == 0) {
        // 0x80170A94: nop
    
            goto L_80170AB8;
    }
    // 0x80170A94: nop

    // 0x80170A98: beql        $v1, $zero, L_80170AB0
    if (ctx->r3 == 0) {
        // 0x80170A9C: addiu       $a0, $a0, 0x64
        ctx->r4 = ADD32(ctx->r4, 0X64);
            goto L_80170AB0;
    }
    goto skip_16;
    // 0x80170A9C: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
    skip_16:
    // 0x80170AA0: lh          $t3, 0x5E($v0)
    ctx->r11 = MEM_H(ctx->r2, 0X5E);
    // 0x80170AA4: bne         $t3, $zero, L_80170AB8
    if (ctx->r11 != 0) {
        // 0x80170AA8: nop
    
            goto L_80170AB8;
    }
    // 0x80170AA8: nop

    // 0x80170AAC: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
L_80170AB0:
    // 0x80170AB0: bne         $a0, $s4, L_80170A88
    if (ctx->r4 != ctx->r20) {
        // 0x80170AB4: addiu       $v0, $v0, 0x64
        ctx->r2 = ADD32(ctx->r2, 0X64);
            goto L_80170A88;
    }
    // 0x80170AB4: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
L_80170AB8:
    // 0x80170AB8: beq         $a0, $at, L_80170AC4
    if (ctx->r4 == ctx->r1) {
        // 0x80170ABC: nop
    
            goto L_80170AC4;
    }
    // 0x80170ABC: nop

    // 0x80170AC0: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
L_80170AC4:
    // 0x80170AC4: b           L_80170C30
    // 0x80170AC8: addiu       $s0, $zero, 0x2
    ctx->r16 = ADD32(0, 0X2);
        goto L_80170C30;
    // 0x80170AC8: addiu       $s0, $zero, 0x2
    ctx->r16 = ADD32(0, 0X2);
L_80170ACC:
    // 0x80170ACC: bne         $t1, $zero, L_80170BD0
    if (ctx->r9 != 0) {
        // 0x80170AD0: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_80170BD0;
    }
    // 0x80170AD0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80170AD4: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80170AD8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80170ADC: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
L_80170AE0:
    // 0x80170AE0: lh          $t5, 0x60($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X60);
    // 0x80170AE4: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
    // 0x80170AE8: slti        $at, $a0, 0xC8
    ctx->r1 = SIGNED(ctx->r4) < 0XC8 ? 1 : 0;
    // 0x80170AEC: beq         $t5, $zero, L_80170B04
    if (ctx->r13 == 0) {
        // 0x80170AF0: nop
    
            goto L_80170B04;
    }
    // 0x80170AF0: nop

    // 0x80170AF4: lh          $t7, 0x5E($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X5E);
    // 0x80170AF8: bne         $t7, $zero, L_80170B04
    if (ctx->r15 != 0) {
        // 0x80170AFC: nop
    
            goto L_80170B04;
    }
    // 0x80170AFC: nop

    // 0x80170B00: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80170B04:
    // 0x80170B04: bne         $at, $zero, L_80170AE0
    if (ctx->r1 != 0) {
        // 0x80170B08: addiu       $v0, $v0, 0x64
        ctx->r2 = ADD32(ctx->r2, 0X64);
            goto L_80170AE0;
    }
    // 0x80170B08: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
    // 0x80170B0C: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80170B10: bne         $s0, $at, L_80170B70
    if (ctx->r16 != ctx->r1) {
        // 0x80170B14: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_80170B70;
    }
    // 0x80170B14: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80170B18: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
L_80170B1C:
    // 0x80170B1C: lh          $t4, 0x60($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X60);
    // 0x80170B20: lwc1        $f10, 0xF0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XF0);
    // 0x80170B24: beql        $t4, $zero, L_80170B50
    if (ctx->r12 == 0) {
        // 0x80170B28: addiu       $at, $zero, 0xC8
        ctx->r1 = ADD32(0, 0XC8);
            goto L_80170B50;
    }
    goto skip_17;
    // 0x80170B28: addiu       $at, $zero, 0xC8
    ctx->r1 = ADD32(0, 0XC8);
    skip_17:
    // 0x80170B2C: lwc1        $f6, 0x18($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X18);
    // 0x80170B30: c.lt.s      $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f10.fl < ctx->f6.fl;
    // 0x80170B34: nop

    // 0x80170B38: bc1tl       L_80170B50
    if (c1cs) {
        // 0x80170B3C: addiu       $at, $zero, 0xC8
        ctx->r1 = ADD32(0, 0XC8);
            goto L_80170B50;
    }
    goto skip_18;
    // 0x80170B3C: addiu       $at, $zero, 0xC8
    ctx->r1 = ADD32(0, 0XC8);
    skip_18:
    // 0x80170B40: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
    // 0x80170B44: bne         $a0, $s5, L_80170B1C
    if (ctx->r4 != ctx->r21) {
        // 0x80170B48: addiu       $v0, $v0, 0x64
        ctx->r2 = ADD32(ctx->r2, 0X64);
            goto L_80170B1C;
    }
    // 0x80170B48: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
    // 0x80170B4C: addiu       $at, $zero, 0xC8
    ctx->r1 = ADD32(0, 0XC8);
L_80170B50:
    // 0x80170B50: bne         $a0, $at, L_80170B68
    if (ctx->r4 != ctx->r1) {
        // 0x80170B54: or          $a3, $v0, $zero
        ctx->r7 = ctx->r2 | 0;
            goto L_80170B68;
    }
    // 0x80170B54: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x80170B58: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170B5C: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170B60: b           L_80170E48
    // 0x80170B64: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170B64: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170B68:
    // 0x80170B68: b           L_80170C30
    // 0x80170B6C: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
        goto L_80170C30;
    // 0x80170B6C: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
L_80170B70:
    // 0x80170B70: lh          $t6, 0x60($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X60);
    // 0x80170B74: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x80170B78: bnel        $t6, $zero, L_80170B8C
    if (ctx->r14 != 0) {
        // 0x80170B7C: lh          $t2, 0xC4($t0)
        ctx->r10 = MEM_H(ctx->r8, 0XC4);
            goto L_80170B8C;
    }
    goto skip_19;
    // 0x80170B7C: lh          $t2, 0xC4($t0)
    ctx->r10 = MEM_H(ctx->r8, 0XC4);
    skip_19:
    // 0x80170B80: b           L_80170BB4
    // 0x80170B84: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
        goto L_80170BB4;
    // 0x80170B84: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80170B88: lh          $t2, 0xC4($t0)
    ctx->r10 = MEM_H(ctx->r8, 0XC4);
L_80170B8C:
    // 0x80170B8C: bnel        $t2, $zero, L_80170BA0
    if (ctx->r10 != 0) {
        // 0x80170B90: lh          $t8, 0xC2($t0)
        ctx->r24 = MEM_H(ctx->r8, 0XC2);
            goto L_80170BA0;
    }
    goto skip_20;
    // 0x80170B90: lh          $t8, 0xC2($t0)
    ctx->r24 = MEM_H(ctx->r8, 0XC2);
    skip_20:
    // 0x80170B94: b           L_80170BB4
    // 0x80170B98: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_80170BB4;
    // 0x80170B98: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80170B9C: lh          $t8, 0xC2($t0)
    ctx->r24 = MEM_H(ctx->r8, 0XC2);
L_80170BA0:
    // 0x80170BA0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80170BA4: beq         $t8, $zero, L_80170BB4
    if (ctx->r24 == 0) {
        // 0x80170BA8: nop
    
            goto L_80170BB4;
    }
    // 0x80170BA8: nop

    // 0x80170BAC: b           L_80170BB4
    // 0x80170BB0: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_80170BB4;
    // 0x80170BB0: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_80170BB4:
    // 0x80170BB4: sll         $t9, $v1, 2
    ctx->r25 = S32(ctx->r3 << 2);
    // 0x80170BB8: subu        $t9, $t9, $v1
    ctx->r25 = SUB32(ctx->r25, ctx->r3);
    // 0x80170BBC: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x80170BC0: addu        $t9, $t9, $v1
    ctx->r25 = ADD32(ctx->r25, ctx->r3);
    // 0x80170BC4: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x80170BC8: b           L_80170C30
    // 0x80170BCC: addu        $a3, $t9, $t0
    ctx->r7 = ADD32(ctx->r25, ctx->r8);
        goto L_80170C30;
    // 0x80170BCC: addu        $a3, $t9, $t0
    ctx->r7 = ADD32(ctx->r25, ctx->r8);
L_80170BD0:
    // 0x80170BD0: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
L_80170BD4:
    // 0x80170BD4: lh          $t3, 0x60($v0)
    ctx->r11 = MEM_H(ctx->r2, 0X60);
    // 0x80170BD8: lwc1        $f8, 0xF0($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XF0);
    // 0x80170BDC: beql        $t3, $zero, L_80170C14
    if (ctx->r11 == 0) {
        // 0x80170BE0: addiu       $at, $zero, 0xC8
        ctx->r1 = ADD32(0, 0XC8);
            goto L_80170C14;
    }
    goto skip_21;
    // 0x80170BE0: addiu       $at, $zero, 0xC8
    ctx->r1 = ADD32(0, 0XC8);
    skip_21:
    // 0x80170BE4: lh          $t5, 0x5E($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X5E);
    // 0x80170BE8: beql        $t5, $zero, L_80170C08
    if (ctx->r13 == 0) {
        // 0x80170BEC: addiu       $a0, $a0, 0x64
        ctx->r4 = ADD32(ctx->r4, 0X64);
            goto L_80170C08;
    }
    goto skip_22;
    // 0x80170BEC: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
    skip_22:
    // 0x80170BF0: lwc1        $f4, 0x18($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X18);
    // 0x80170BF4: c.lt.s      $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f8.fl < ctx->f4.fl;
    // 0x80170BF8: nop

    // 0x80170BFC: bc1tl       L_80170C14
    if (c1cs) {
        // 0x80170C00: addiu       $at, $zero, 0xC8
        ctx->r1 = ADD32(0, 0XC8);
            goto L_80170C14;
    }
    goto skip_23;
    // 0x80170C00: addiu       $at, $zero, 0xC8
    ctx->r1 = ADD32(0, 0XC8);
    skip_23:
    // 0x80170C04: addiu       $a0, $a0, 0x64
    ctx->r4 = ADD32(ctx->r4, 0X64);
L_80170C08:
    // 0x80170C08: bne         $a0, $s6, L_80170BD4
    if (ctx->r4 != ctx->r22) {
        // 0x80170C0C: addiu       $v0, $v0, 0x64
        ctx->r2 = ADD32(ctx->r2, 0X64);
            goto L_80170BD4;
    }
    // 0x80170C0C: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
    // 0x80170C10: addiu       $at, $zero, 0xC8
    ctx->r1 = ADD32(0, 0XC8);
L_80170C14:
    // 0x80170C14: bne         $a0, $at, L_80170C2C
    if (ctx->r4 != ctx->r1) {
        // 0x80170C18: or          $a3, $v0, $zero
        ctx->r7 = ctx->r2 | 0;
            goto L_80170C2C;
    }
    // 0x80170C18: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x80170C1C: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170C20: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170C24: b           L_80170E48
    // 0x80170C28: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170C28: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170C2C:
    // 0x80170C2C: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_80170C30:
    // 0x80170C30: beq         $t1, $zero, L_80170C60
    if (ctx->r9 == 0) {
        // 0x80170C34: addiu       $at, $zero, 0xA
        ctx->r1 = ADD32(0, 0XA);
            goto L_80170C60;
    }
    // 0x80170C34: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x80170C38: lh          $t7, 0x60($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X60);
    // 0x80170C3C: beq         $t7, $zero, L_80170C60
    if (ctx->r15 == 0) {
        // 0x80170C40: nop
    
            goto L_80170C60;
    }
    // 0x80170C40: nop

    // 0x80170C44: lh          $t4, 0x5E($a3)
    ctx->r12 = MEM_H(ctx->r7, 0X5E);
    // 0x80170C48: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170C4C: bne         $t4, $zero, L_80170C60
    if (ctx->r12 != 0) {
        // 0x80170C50: nop
    
            goto L_80170C60;
    }
    // 0x80170C50: nop

    // 0x80170C54: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170C58: b           L_80170E48
    // 0x80170C5C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170C5C: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170C60:
    // 0x80170C60: beq         $s0, $at, L_80170D30
    if (ctx->r16 == ctx->r1) {
        // 0x80170C64: addiu       $a0, $sp, 0xD8
        ctx->r4 = ADD32(ctx->r29, 0XD8);
            goto L_80170D30;
    }
    // 0x80170C64: addiu       $a0, $sp, 0xD8
    ctx->r4 = ADD32(ctx->r29, 0XD8);
    // 0x80170C68: lh          $v0, 0x60($t0)
    ctx->r2 = MEM_H(ctx->r8, 0X60);
    // 0x80170C6C: beq         $v0, $zero, L_80170CBC
    if (ctx->r2 == 0) {
        // 0x80170C70: nop
    
            goto L_80170CBC;
    }
    // 0x80170C70: nop

    // 0x80170C74: lh          $t6, 0xC4($t0)
    ctx->r14 = MEM_H(ctx->r8, 0XC4);
    // 0x80170C78: or          $t9, $t0, $zero
    ctx->r25 = ctx->r8 | 0;
    // 0x80170C7C: or          $t3, $t0, $zero
    ctx->r11 = ctx->r8 | 0;
    // 0x80170C80: bne         $t6, $zero, L_80170CBC
    if (ctx->r14 != 0) {
        // 0x80170C84: addiu       $t5, $t0, 0x60
        ctx->r13 = ADD32(ctx->r8, 0X60);
            goto L_80170CBC;
    }
    // 0x80170C84: addiu       $t5, $t0, 0x60
    ctx->r13 = ADD32(ctx->r8, 0X60);
L_80170C88:
    // 0x80170C88: lw          $t8, 0x0($t9)
    ctx->r24 = MEM_W(ctx->r25, 0X0);
    // 0x80170C8C: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x80170C90: addiu       $t3, $t3, 0xC
    ctx->r11 = ADD32(ctx->r11, 0XC);
    // 0x80170C94: sw          $t8, 0x58($t3)
    MEM_W(0X58, ctx->r11) = ctx->r24;
    // 0x80170C98: lw          $t2, -0x8($t9)
    ctx->r10 = MEM_W(ctx->r25, -0X8);
    // 0x80170C9C: sw          $t2, 0x5C($t3)
    MEM_W(0X5C, ctx->r11) = ctx->r10;
    // 0x80170CA0: lw          $t8, -0x4($t9)
    ctx->r24 = MEM_W(ctx->r25, -0X4);
    // 0x80170CA4: bne         $t9, $t5, L_80170C88
    if (ctx->r25 != ctx->r13) {
        // 0x80170CA8: sw          $t8, 0x60($t3)
        MEM_W(0X60, ctx->r11) = ctx->r24;
            goto L_80170C88;
    }
    // 0x80170CA8: sw          $t8, 0x60($t3)
    MEM_W(0X60, ctx->r11) = ctx->r24;
    // 0x80170CAC: lw          $t8, 0x0($t9)
    ctx->r24 = MEM_W(ctx->r25, 0X0);
    // 0x80170CB0: or          $a3, $t0, $zero
    ctx->r7 = ctx->r8 | 0;
    // 0x80170CB4: b           L_80170D30
    // 0x80170CB8: sw          $t8, 0x64($t3)
    MEM_W(0X64, ctx->r11) = ctx->r24;
        goto L_80170D30;
    // 0x80170CB8: sw          $t8, 0x64($t3)
    MEM_W(0X64, ctx->r11) = ctx->r24;
L_80170CBC:
    // 0x80170CBC: bnel        $s0, $zero, L_80170D34
    if (ctx->r16 != 0) {
        // 0x80170CC0: addiu       $t8, $zero, 0x1
        ctx->r24 = ADD32(0, 0X1);
            goto L_80170D34;
    }
    goto skip_24;
    // 0x80170CC0: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    skip_24:
    // 0x80170CC4: beql        $v0, $zero, L_80170D34
    if (ctx->r2 == 0) {
        // 0x80170CC8: addiu       $t8, $zero, 0x1
        ctx->r24 = ADD32(0, 0X1);
            goto L_80170D34;
    }
    goto skip_25;
    // 0x80170CC8: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    skip_25:
    // 0x80170CCC: beq         $t1, $zero, L_80170CFC
    if (ctx->r9 == 0) {
        // 0x80170CD0: or          $t9, $t0, $zero
        ctx->r25 = ctx->r8 | 0;
            goto L_80170CFC;
    }
    // 0x80170CD0: or          $t9, $t0, $zero
    ctx->r25 = ctx->r8 | 0;
    // 0x80170CD4: lh          $t7, 0xC4($t0)
    ctx->r15 = MEM_H(ctx->r8, 0XC4);
    // 0x80170CD8: beql        $t7, $zero, L_80170D00
    if (ctx->r15 == 0) {
        // 0x80170CDC: or          $t3, $t0, $zero
        ctx->r11 = ctx->r8 | 0;
            goto L_80170D00;
    }
    goto skip_26;
    // 0x80170CDC: or          $t3, $t0, $zero
    ctx->r11 = ctx->r8 | 0;
    skip_26:
    // 0x80170CE0: lh          $t4, 0xC2($t0)
    ctx->r12 = MEM_H(ctx->r8, 0XC2);
    // 0x80170CE4: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170CE8: bnel        $t4, $zero, L_80170D00
    if (ctx->r12 != 0) {
        // 0x80170CEC: or          $t3, $t0, $zero
        ctx->r11 = ctx->r8 | 0;
            goto L_80170D00;
    }
    goto skip_27;
    // 0x80170CEC: or          $t3, $t0, $zero
    ctx->r11 = ctx->r8 | 0;
    skip_27:
    // 0x80170CF0: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170CF4: b           L_80170E48
    // 0x80170CF8: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
        goto L_80170E48;
    // 0x80170CF8: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170CFC:
    // 0x80170CFC: or          $t3, $t0, $zero
    ctx->r11 = ctx->r8 | 0;
L_80170D00:
    // 0x80170D00: addiu       $t2, $t0, 0x60
    ctx->r10 = ADD32(ctx->r8, 0X60);
L_80170D04:
    // 0x80170D04: lw          $t5, 0x0($t9)
    ctx->r13 = MEM_W(ctx->r25, 0X0);
    // 0x80170D08: addiu       $t9, $t9, 0xC
    ctx->r25 = ADD32(ctx->r25, 0XC);
    // 0x80170D0C: addiu       $t3, $t3, 0xC
    ctx->r11 = ADD32(ctx->r11, 0XC);
    // 0x80170D10: sw          $t5, 0x58($t3)
    MEM_W(0X58, ctx->r11) = ctx->r13;
    // 0x80170D14: lw          $t6, -0x8($t9)
    ctx->r14 = MEM_W(ctx->r25, -0X8);
    // 0x80170D18: sw          $t6, 0x5C($t3)
    MEM_W(0X5C, ctx->r11) = ctx->r14;
    // 0x80170D1C: lw          $t5, -0x4($t9)
    ctx->r13 = MEM_W(ctx->r25, -0X4);
    // 0x80170D20: bne         $t9, $t2, L_80170D04
    if (ctx->r25 != ctx->r10) {
        // 0x80170D24: sw          $t5, 0x60($t3)
        MEM_W(0X60, ctx->r11) = ctx->r13;
            goto L_80170D04;
    }
    // 0x80170D24: sw          $t5, 0x60($t3)
    MEM_W(0X60, ctx->r11) = ctx->r13;
    // 0x80170D28: lw          $t5, 0x0($t9)
    ctx->r13 = MEM_W(ctx->r25, 0X0);
    // 0x80170D2C: sw          $t5, 0x64($t3)
    MEM_W(0X64, ctx->r11) = ctx->r13;
L_80170D30:
    // 0x80170D30: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
L_80170D34:
    // 0x80170D34: sw          $t8, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->r24;
    // 0x80170D38: or          $v0, $a3, $zero
    ctx->r2 = ctx->r7 | 0;
    // 0x80170D3C: addiu       $a1, $sp, 0xCC
    ctx->r5 = ADD32(ctx->r29, 0XCC);
    // 0x80170D40: addiu       $a2, $sp, 0xE4
    ctx->r6 = ADD32(ctx->r29, 0XE4);
    // 0x80170D44: addiu       $v1, $sp, 0xC0
    ctx->r3 = ADD32(ctx->r29, 0XC0);
L_80170D48:
    // 0x80170D48: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80170D4C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80170D50: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80170D54: swc1        $f10, 0x28($v0)
    MEM_W(0X28, ctx->r2) = ctx->f10.u32l;
    // 0x80170D58: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80170D5C: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x80170D60: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x80170D64: swc1        $f6, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->f6.u32l;
    // 0x80170D68: lwc1        $f8, 0x0($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X0);
    // 0x80170D6C: addiu       $a2, $a2, 0x4
    ctx->r6 = ADD32(ctx->r6, 0X4);
    // 0x80170D70: swc1        $f8, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->f8.u32l;
    // 0x80170D74: lwc1        $f4, -0x4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, -0X4);
    // 0x80170D78: bne         $v1, $s3, L_80170D48
    if (ctx->r3 != ctx->r19) {
        // 0x80170D7C: swc1        $f4, -0x4($v0)
        MEM_W(-0X4, ctx->r2) = ctx->f4.u32l;
            goto L_80170D48;
    }
    // 0x80170D7C: swc1        $f4, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f4.u32l;
    // 0x80170D80: bne         $t0, $a3, L_80170DA4
    if (ctx->r8 != ctx->r7) {
        // 0x80170D84: addiu       $a0, $sp, 0x80
        ctx->r4 = ADD32(ctx->r29, 0X80);
            goto L_80170DA4;
    }
    // 0x80170D84: addiu       $a0, $sp, 0x80
    ctx->r4 = ADD32(ctx->r29, 0X80);
    // 0x80170D88: addiu       $a0, $sp, 0x80
    ctx->r4 = ADD32(ctx->r29, 0X80);
    // 0x80170D8C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80170D90: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x80170D94: jal         0x80001090
    // 0x80170D98: sw          $a3, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r7;
    memory_copy(rdram, ctx);
        goto after_5;
    // 0x80170D98: sw          $a3, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r7;
    after_5:
    // 0x80170D9C: b           L_80170DBC
    // 0x80170DA0: lw          $a3, 0xA0($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XA0);
        goto L_80170DBC;
    // 0x80170DA0: lw          $a3, 0xA0($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XA0);
L_80170DA4:
    // 0x80170DA4: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80170DA8: addiu       $a1, $a1, -0x668
    ctx->r5 = ADD32(ctx->r5, -0X668);
    // 0x80170DAC: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x80170DB0: jal         0x80001090
    // 0x80170DB4: sw          $a3, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r7;
    memory_copy(rdram, ctx);
        goto after_6;
    // 0x80170DB4: sw          $a3, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r7;
    after_6:
    // 0x80170DB8: lw          $a3, 0xA0($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XA0);
L_80170DBC:
    // 0x80170DBC: lwc1        $f10, 0xF0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XF0);
    // 0x80170DC0: sw          $s1, 0x44($a3)
    MEM_W(0X44, ctx->r7) = ctx->r17;
    // 0x80170DC4: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x80170DC8: swc1        $f10, 0x18($a3)
    MEM_W(0X18, ctx->r7) = ctx->f10.u32l;
    // 0x80170DCC: lw          $t7, -0x9FC($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X9FC);
    // 0x80170DD0: lui         $t2, 0x801A
    ctx->r10 = S32(0X801A << 16);
    // 0x80170DD4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80170DD8: lw          $t4, 0x8($t7)
    ctx->r12 = MEM_W(ctx->r15, 0X8);
    // 0x80170DDC: addiu       $a1, $a3, 0x48
    ctx->r5 = ADD32(ctx->r7, 0X48);
    // 0x80170DE0: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x80170DE4: sw          $t4, 0x58($a3)
    MEM_W(0X58, ctx->r7) = ctx->r12;
    // 0x80170DE8: lw          $t2, -0x9FC($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X9FC);
    // 0x80170DEC: lw          $t9, 0x0($t2)
    ctx->r25 = MEM_W(ctx->r10, 0X0);
    // 0x80170DF0: lh          $t3, 0x8($t9)
    ctx->r11 = MEM_H(ctx->r25, 0X8);
    // 0x80170DF4: sh          $t3, 0x5C($a3)
    MEM_H(0X5C, ctx->r7) = ctx->r11;
    // 0x80170DF8: jal         0x80001090
    // 0x80170DFC: sw          $a3, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r7;
    memory_copy(rdram, ctx);
        goto after_7;
    // 0x80170DFC: sw          $a3, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r7;
    after_7:
    // 0x80170E00: lw          $a3, 0xA0($sp)
    ctx->r7 = MEM_W(ctx->r29, 0XA0);
    // 0x80170E04: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x80170E08: lh          $t6, -0x680($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X680);
    // 0x80170E0C: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80170E10: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80170E14: sh          $t5, 0x60($a3)
    MEM_H(0X60, ctx->r7) = ctx->r13;
    // 0x80170E18: sh          $t6, 0x5E($a3)
    MEM_H(0X5E, ctx->r7) = ctx->r14;
    // 0x80170E1C: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x80170E20: lwc1        $f8, 0xF0($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XF0);
    // 0x80170E24: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170E28: lwc1        $f6, 0x134($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X134);
    // 0x80170E2C: c.lt.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl < ctx->f8.fl;
    // 0x80170E30: nop

    // 0x80170E34: bc1f        L_80170E40
    if (!c1cs) {
        // 0x80170E38: nop
    
            goto L_80170E40;
    }
    // 0x80170E38: nop

    // 0x80170E3C: swc1        $f8, 0x134($v0)
    MEM_W(0X134, ctx->r2) = ctx->f8.u32l;
L_80170E40:
    // 0x80170E40: lw          $v1, -0x1118($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1118);
    // 0x80170E44: sll         $v0, $v1, 1
    ctx->r2 = S32(ctx->r3 << 1);
L_80170E48:
    // 0x80170E48: lw          $t8, 0x64($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X64);
    // 0x80170E4C: addiu       $t7, $t8, 0x2
    ctx->r15 = ADD32(ctx->r24, 0X2);
    // 0x80170E50: slt         $at, $t7, $v0
    ctx->r1 = SIGNED(ctx->r15) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80170E54: bne         $at, $zero, L_80170464
    if (ctx->r1 != 0) {
        // 0x80170E58: sw          $t7, 0x64($sp)
        MEM_W(0X64, ctx->r29) = ctx->r15;
            goto L_80170464;
    }
    // 0x80170E58: sw          $t7, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r15;
L_80170E5C:
    // 0x80170E5C: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
    // 0x80170E60: lw          $v0, 0xF4($sp)
    ctx->r2 = MEM_W(ctx->r29, 0XF4);
    // 0x80170E64: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80170E68: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x80170E6C: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x80170E70: lw          $s1, 0x2C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X2C);
    // 0x80170E74: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x80170E78: lw          $s3, 0x34($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X34);
    // 0x80170E7C: lw          $s4, 0x38($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X38);
    // 0x80170E80: lw          $s5, 0x3C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X3C);
    // 0x80170E84: lw          $s6, 0x40($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X40);
    // 0x80170E88: lw          $s7, 0x44($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X44);
    // 0x80170E8C: lw          $fp, 0x48($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X48);
    // 0x80170E90: jr          $ra
    // 0x80170E94: addiu       $sp, $sp, 0x110
    ctx->r29 = ADD32(ctx->r29, 0X110);
    return;
    // 0x80170E94: addiu       $sp, $sp, 0x110
    ctx->r29 = ADD32(ctx->r29, 0X110);
;}
RECOMP_FUNC void func_80170E98(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80170E98: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x80170E9C: lwc1        $f12, 0x78($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X78);
    // 0x80170EA0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80170EA4: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80170EA8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80170EAC: c.lt.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl < ctx->f4.fl;
    // 0x80170EB0: sw          $a3, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r7;
    // 0x80170EB4: addiu       $v1, $v1, -0x9FC
    ctx->r3 = ADD32(ctx->r3, -0X9FC);
    // 0x80170EB8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80170EBC: bc1fl       L_80170ED4
    if (!c1cs) {
        // 0x80170EC0: swc1        $f12, 0x34($sp)
        MEM_W(0X34, ctx->r29) = ctx->f12.u32l;
            goto L_80170ED4;
    }
    goto skip_0;
    // 0x80170EC0: swc1        $f12, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f12.u32l;
    skip_0:
    // 0x80170EC4: neg.s       $f6, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = -ctx->f12.fl;
    // 0x80170EC8: b           L_80170ED4
    // 0x80170ECC: swc1        $f6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f6.u32l;
        goto L_80170ED4;
    // 0x80170ECC: swc1        $f6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f6.u32l;
    // 0x80170ED0: swc1        $f12, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f12.u32l;
L_80170ED4:
    // 0x80170ED4: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80170ED8: lwc1        $f10, 0x34($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80170EDC: lwc1        $f8, 0xE0($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0XE0);
    // 0x80170EE0: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x80170EE4: nop

    // 0x80170EE8: bc1fl       L_80170EFC
    if (!c1cs) {
        // 0x80170EEC: lh          $t6, 0x12($a0)
        ctx->r14 = MEM_H(ctx->r4, 0X12);
            goto L_80170EFC;
    }
    goto skip_1;
    // 0x80170EEC: lh          $t6, 0x12($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X12);
    skip_1:
    // 0x80170EF0: b           L_801711E8
    // 0x80170EF4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_801711E8;
    // 0x80170EF4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80170EF8: lh          $t6, 0x12($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X12);
L_80170EFC:
    // 0x80170EFC: lh          $t7, 0x18($a0)
    ctx->r15 = MEM_H(ctx->r4, 0X18);
    // 0x80170F00: bnel        $t6, $t7, L_80170F68
    if (ctx->r14 != ctx->r15) {
        // 0x80170F04: lh          $t2, 0x16($a0)
        ctx->r10 = MEM_H(ctx->r4, 0X16);
            goto L_80170F68;
    }
    goto skip_2;
    // 0x80170F04: lh          $t2, 0x16($a0)
    ctx->r10 = MEM_H(ctx->r4, 0X16);
    skip_2:
    // 0x80170F08: lh          $t8, 0x10($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X10);
    // 0x80170F0C: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x80170F10: nop

    // 0x80170F14: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80170F18: swc1        $f0, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f0.u32l;
    // 0x80170F1C: lh          $t9, 0x16($a0)
    ctx->r25 = MEM_H(ctx->r4, 0X16);
    // 0x80170F20: mtc1        $t9, $f6
    ctx->f6.u32l = ctx->r25;
    // 0x80170F24: nop

    // 0x80170F28: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80170F2C: sub.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f0.fl;
    // 0x80170F30: swc1        $f10, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f10.u32l;
    // 0x80170F34: lh          $t0, 0x14($a0)
    ctx->r8 = MEM_H(ctx->r4, 0X14);
    // 0x80170F38: mtc1        $t0, $f4
    ctx->f4.u32l = ctx->r8;
    // 0x80170F3C: nop

    // 0x80170F40: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80170F44: swc1        $f0, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f0.u32l;
    // 0x80170F48: lh          $t1, 0x1A($a0)
    ctx->r9 = MEM_H(ctx->r4, 0X1A);
    // 0x80170F4C: mtc1        $t1, $f6
    ctx->f6.u32l = ctx->r9;
    // 0x80170F50: nop

    // 0x80170F54: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80170F58: sub.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f0.fl;
    // 0x80170F5C: b           L_80170FBC
    // 0x80170F60: swc1        $f10, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f10.u32l;
        goto L_80170FBC;
    // 0x80170F60: swc1        $f10, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f10.u32l;
    // 0x80170F64: lh          $t2, 0x16($a0)
    ctx->r10 = MEM_H(ctx->r4, 0X16);
L_80170F68:
    // 0x80170F68: mtc1        $t2, $f4
    ctx->f4.u32l = ctx->r10;
    // 0x80170F6C: nop

    // 0x80170F70: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80170F74: swc1        $f0, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f0.u32l;
    // 0x80170F78: lh          $t3, 0x1C($a0)
    ctx->r11 = MEM_H(ctx->r4, 0X1C);
    // 0x80170F7C: mtc1        $t3, $f6
    ctx->f6.u32l = ctx->r11;
    // 0x80170F80: nop

    // 0x80170F84: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80170F88: sub.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f0.fl;
    // 0x80170F8C: swc1        $f10, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f10.u32l;
    // 0x80170F90: lh          $t4, 0x1A($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X1A);
    // 0x80170F94: mtc1        $t4, $f4
    ctx->f4.u32l = ctx->r12;
    // 0x80170F98: nop

    // 0x80170F9C: cvt.s.w     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    ctx->f0.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80170FA0: swc1        $f0, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f0.u32l;
    // 0x80170FA4: lh          $t5, 0x20($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X20);
    // 0x80170FA8: mtc1        $t5, $f6
    ctx->f6.u32l = ctx->r13;
    // 0x80170FAC: nop

    // 0x80170FB0: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80170FB4: sub.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f0.fl;
    // 0x80170FB8: swc1        $f10, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f10.u32l;
L_80170FBC:
    // 0x80170FBC: lwc1        $f16, 0x8($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80170FC0: lwc1        $f4, 0x50($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X50);
    // 0x80170FC4: lwc1        $f18, 0x0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80170FC8: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80170FCC: mul.s       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x80170FD0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80170FD4: mul.s       $f10, $f18, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f8.fl);
    // 0x80170FD8: sub.s       $f2, $f6, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x80170FDC: c.lt.s      $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f2.fl < ctx->f4.fl;
    // 0x80170FE0: lwc1        $f4, 0x64($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X64);
    // 0x80170FE4: bc1fl       L_80170FF8
    if (!c1cs) {
        // 0x80170FE8: mov.s       $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
            goto L_80170FF8;
    }
    goto skip_3;
    // 0x80170FE8: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    skip_3:
    // 0x80170FEC: b           L_80170FF8
    // 0x80170FF0: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
        goto L_80170FF8;
    // 0x80170FF0: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x80170FF4: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_80170FF8:
    // 0x80170FF8: lwc1        $f8, -0x50F8($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X50F8);
    // 0x80170FFC: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x80171000: nop

    // 0x80171004: bc1fl       L_80171018
    if (!c1cs) {
        // 0x80171008: lwc1        $f8, 0xF0($v0)
        ctx->f8.u32l = MEM_W(ctx->r2, 0XF0);
            goto L_80171018;
    }
    goto skip_4;
    // 0x80171008: lwc1        $f8, 0xF0($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0XF0);
    skip_4:
    // 0x8017100C: b           L_801711E8
    // 0x80171010: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_801711E8;
    // 0x80171010: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80171014: lwc1        $f8, 0xF0($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0XF0);
L_80171018:
    // 0x80171018: lwc1        $f6, 0x5C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8017101C: lwc1        $f10, 0xE8($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0XE8);
    // 0x80171020: sub.s       $f14, $f4, $f8
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f14.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x80171024: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80171028: sub.s       $f12, $f6, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x8017102C: mul.s       $f10, $f14, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f18.fl);
    // 0x80171030: nop

    // 0x80171034: mul.s       $f8, $f16, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f12.fl);
    // 0x80171038: sub.s       $f10, $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x8017103C: lwc1        $f8, 0x50($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X50);
    // 0x80171040: div.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80171044: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x80171048: add.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x8017104C: lwc1        $f10, 0x58($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80171050: mul.s       $f6, $f10, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80171054: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80171058: swc1        $f8, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f8.u32l;
    // 0x8017105C: c.lt.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl < ctx->f10.fl;
    // 0x80171060: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x80171064: bc1t        L_80171084
    if (c1cs) {
        // 0x80171068: swc1        $f8, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->f8.u32l;
            goto L_80171084;
    }
    // 0x80171068: swc1        $f8, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f8.u32l;
    // 0x8017106C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80171070: lwc1        $f2, 0x34($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80171074: lwc1        $f4, 0x18($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X18);
    // 0x80171078: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x8017107C: nop

    // 0x80171080: bc1f        L_80171148
    if (!c1cs) {
        // 0x80171084: lui         $at, 0x3F80
        ctx->r1 = S32(0X3F80 << 16);
            goto L_80171148;
    }
L_80171084:
    // 0x80171084: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80171088: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8017108C: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80171090: mov.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
    // 0x80171094: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x80171098: swc1        $f4, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f4.u32l;
    // 0x8017109C: lwc1        $f8, 0x64($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801710A0: mov.s       $f16, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    ctx->f16.fl = ctx->f14.fl;
    // 0x801710A4: bc1f        L_801710D8
    if (!c1cs) {
        // 0x801710A8: swc1        $f8, 0x8($a1)
        MEM_W(0X8, ctx->r5) = ctx->f8.u32l;
            goto L_801710D8;
    }
    // 0x801710A8: swc1        $f8, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f8.u32l;
    // 0x801710AC: lwc1        $f6, 0x50($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X50);
    // 0x801710B0: lwc1        $f8, 0x0($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801710B4: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801710B8: add.s       $f2, $f12, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f12.fl + ctx->f6.fl;
    // 0x801710BC: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x801710C0: add.s       $f16, $f14, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = ctx->f14.fl + ctx->f4.fl;
    // 0x801710C4: swc1        $f10, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f10.u32l;
    // 0x801710C8: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801710CC: lwc1        $f4, 0x8($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801710D0: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801710D4: swc1        $f6, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f6.u32l;
L_801710D8:
    // 0x801710D8: mul.s       $f10, $f2, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801710DC: sw          $a1, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r5;
    // 0x801710E0: sw          $a2, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r6;
    // 0x801710E4: mul.s       $f4, $f16, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x801710E8: jal         0x800A01E0
    // 0x801710EC: add.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f4.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x801710EC: add.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f4.fl;
    after_0:
    // 0x801710F0: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x801710F4: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801710F8: addiu       $v1, $v1, -0x9FC
    ctx->r3 = ADD32(ctx->r3, -0X9FC);
    // 0x801710FC: c.eq.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl == ctx->f8.fl;
    // 0x80171100: lw          $a1, 0x6C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X6C);
    // 0x80171104: lw          $a2, 0x70($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X70);
    // 0x80171108: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x8017110C: bc1f        L_80171118
    if (!c1cs) {
        // 0x80171110: lui         $at, 0x801A
        ctx->r1 = S32(0X801A << 16);
            goto L_80171118;
    }
    // 0x80171110: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171114: lwc1        $f2, -0x50F4($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X50F4);
L_80171118:
    // 0x80171118: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x8017111C: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x80171120: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171124: lwc1        $f6, 0xE0($t6)
    ctx->f6.u32l = MEM_W(ctx->r14, 0XE0);
    // 0x80171128: c.lt.s      $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f6.fl < ctx->f2.fl;
    // 0x8017112C: nop

    // 0x80171130: bc1f        L_80171140
    if (!c1cs) {
        // 0x80171134: nop
    
            goto L_80171140;
    }
    // 0x80171134: nop

    // 0x80171138: b           L_801711E8
    // 0x8017113C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_801711E8;
    // 0x8017113C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80171140:
    // 0x80171140: b           L_80171178
    // 0x80171144: sh          $t7, -0x680($at)
    MEM_H(-0X680, ctx->r1) = ctx->r15;
        goto L_80171178;
    // 0x80171144: sh          $t7, -0x680($at)
    MEM_H(-0X680, ctx->r1) = ctx->r15;
L_80171148:
    // 0x80171148: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8017114C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171150: c.eq.s      $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f2.fl == ctx->f10.fl;
    // 0x80171154: nop

    // 0x80171158: bc1fl       L_80171168
    if (!c1cs) {
        // 0x8017115C: swc1        $f4, 0x0($a1)
        MEM_W(0X0, ctx->r5) = ctx->f4.u32l;
            goto L_80171168;
    }
    goto skip_5;
    // 0x8017115C: swc1        $f4, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f4.u32l;
    skip_5:
    // 0x80171160: lwc1        $f2, -0x50F0($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X50F0);
    // 0x80171164: swc1        $f4, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f4.u32l;
L_80171168:
    // 0x80171168: lwc1        $f8, 0x20($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X20);
    // 0x8017116C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171170: swc1        $f8, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f8.u32l;
    // 0x80171174: sh          $zero, -0x680($at)
    MEM_H(-0X680, ctx->r1) = 0;
L_80171178:
    // 0x80171178: lw          $t8, 0x74($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X74);
    // 0x8017117C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80171180: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80171184: swc1        $f2, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->f2.u32l;
    // 0x80171188: lw          $t9, 0x0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X0);
    // 0x8017118C: lwc1        $f10, 0x0($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80171190: lwc1        $f6, 0xE8($t9)
    ctx->f6.u32l = MEM_W(ctx->r25, 0XE8);
    // 0x80171194: sub.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x80171198: swc1        $f4, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->f4.u32l;
    // 0x8017119C: lw          $t0, 0x0($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X0);
    // 0x801711A0: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801711A4: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x801711A8: lwc1        $f8, 0xF0($t0)
    ctx->f8.u32l = MEM_W(ctx->r8, 0XF0);
    // 0x801711AC: swc1        $f4, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f4.u32l;
    // 0x801711B0: sub.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x801711B4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801711B8: lwc1        $f6, 0x0($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X0);
    // 0x801711BC: div.s       $f0, $f8, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f2.fl);
    // 0x801711C0: swc1        $f10, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f10.u32l;
    // 0x801711C4: lwc1        $f4, 0x8($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X8);
    // 0x801711C8: mul.s       $f10, $f6, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x801711CC: nop

    // 0x801711D0: mul.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x801711D4: swc1        $f10, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->f10.u32l;
    // 0x801711D8: swc1        $f8, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f8.u32l;
    // 0x801711DC: lw          $t1, 0x0($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X0);
    // 0x801711E0: lwc1        $f6, 0xEC($t1)
    ctx->f6.u32l = MEM_W(ctx->r9, 0XEC);
    // 0x801711E4: swc1        $f6, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f6.u32l;
L_801711E8:
    // 0x801711E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801711EC: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    // 0x801711F0: jr          $ra
    // 0x801711F4: nop

    return;
    // 0x801711F4: nop

;}
RECOMP_FUNC void func_801711F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801711F8: addiu       $sp, $sp, -0x170
    ctx->r29 = ADD32(ctx->r29, -0X170);
    // 0x801711FC: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171200: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171204: lw          $t0, 0x0($t1)
    ctx->r8 = MEM_W(ctx->r9, 0X0);
    // 0x80171208: sw          $ra, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r31;
    // 0x8017120C: sw          $fp, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r30;
    // 0x80171210: sw          $s7, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r23;
    // 0x80171214: sw          $s6, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r22;
    // 0x80171218: sw          $s5, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r21;
    // 0x8017121C: sw          $s4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r20;
    // 0x80171220: sw          $s3, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r19;
    // 0x80171224: sw          $s2, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r18;
    // 0x80171228: sw          $s1, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r17;
    // 0x8017122C: sw          $s0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r16;
    // 0x80171230: sdc1        $f30, 0x48($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X48, ctx->r29);
    // 0x80171234: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x80171238: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x8017123C: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x80171240: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x80171244: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x80171248: sw          $a1, 0x174($sp)
    MEM_W(0X174, ctx->r29) = ctx->r5;
    // 0x8017124C: sw          $a2, 0x178($sp)
    MEM_W(0X178, ctx->r29) = ctx->r6;
    // 0x80171250: sw          $a3, 0x17C($sp)
    MEM_W(0X17C, ctx->r29) = ctx->r7;
    // 0x80171254: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80171258: lwc1        $f0, 0xE8($t0)
    ctx->f0.u32l = MEM_W(ctx->r8, 0XE8);
    // 0x8017125C: lwc1        $f8, 0x8($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80171260: lwc1        $f14, 0xF0($t0)
    ctx->f14.u32l = MEM_W(ctx->r8, 0XF0);
    // 0x80171264: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80171268: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x8017126C: lw          $t6, 0x24A8($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X24A8);
    // 0x80171270: mul.s       $f10, $f8, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f14.fl);
    // 0x80171274: lwc1        $f8, 0xC($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0XC);
    // 0x80171278: or          $fp, $a0, $zero
    ctx->r30 = ctx->r4 | 0;
    // 0x8017127C: add.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x80171280: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80171284: beq         $t6, $zero, L_801712AC
    if (ctx->r14 == 0) {
        // 0x80171288: swc1        $f6, 0x110($sp)
        MEM_W(0X110, ctx->r29) = ctx->f6.u32l;
            goto L_801712AC;
    }
    // 0x80171288: swc1        $f6, 0x110($sp)
    MEM_W(0X110, ctx->r29) = ctx->f6.u32l;
    // 0x8017128C: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x80171290: lwc1        $f10, 0x110($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X110);
    // 0x80171294: c.lt.s      $f10, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f10.fl < ctx->f22.fl;
    // 0x80171298: nop

    // 0x8017129C: bc1fl       L_801712B0
    if (!c1cs) {
        // 0x801712A0: lwc1        $f8, 0x110($sp)
        ctx->f8.u32l = MEM_W(ctx->r29, 0X110);
            goto L_801712B0;
    }
    goto skip_0;
    // 0x801712A0: lwc1        $f8, 0x110($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X110);
    skip_0:
    // 0x801712A4: b           L_80171E38
    // 0x801712A8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80171E38;
    // 0x801712A8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801712AC:
    // 0x801712AC: lwc1        $f8, 0x110($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X110);
L_801712B0:
    // 0x801712B0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x801712B4: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x801712B8: lwc1        $f10, 0x110($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X110);
    // 0x801712BC: c.lt.s      $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f8.fl < ctx->f4.fl;
    // 0x801712C0: nop

    // 0x801712C4: bc1fl       L_801712DC
    if (!c1cs) {
        // 0x801712C8: swc1        $f10, 0x10C($sp)
        MEM_W(0X10C, ctx->r29) = ctx->f10.u32l;
            goto L_801712DC;
    }
    goto skip_1;
    // 0x801712C8: swc1        $f10, 0x10C($sp)
    MEM_W(0X10C, ctx->r29) = ctx->f10.u32l;
    skip_1:
    // 0x801712CC: neg.s       $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = -ctx->f8.fl;
    // 0x801712D0: b           L_801712DC
    // 0x801712D4: swc1        $f6, 0x10C($sp)
    MEM_W(0X10C, ctx->r29) = ctx->f6.u32l;
        goto L_801712DC;
    // 0x801712D4: swc1        $f6, 0x10C($sp)
    MEM_W(0X10C, ctx->r29) = ctx->f6.u32l;
    // 0x801712D8: swc1        $f10, 0x10C($sp)
    MEM_W(0X10C, ctx->r29) = ctx->f10.u32l;
L_801712DC:
    // 0x801712DC: lwc1        $f4, 0xE0($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0XE0);
    // 0x801712E0: lwc1        $f8, 0x10C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X10C);
    // 0x801712E4: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x801712E8: nop

    // 0x801712EC: bc1fl       L_80171300
    if (!c1cs) {
        // 0x801712F0: lh          $a0, 0x28($fp)
        ctx->r4 = MEM_H(ctx->r30, 0X28);
            goto L_80171300;
    }
    goto skip_2;
    // 0x801712F0: lh          $a0, 0x28($fp)
    ctx->r4 = MEM_H(ctx->r30, 0X28);
    skip_2:
    // 0x801712F4: b           L_80171E38
    // 0x801712F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80171E38;
    // 0x801712F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801712FC: lh          $a0, 0x28($fp)
    ctx->r4 = MEM_H(ctx->r30, 0X28);
L_80171300:
    // 0x80171300: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x80171304: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
    // 0x80171308: bne         $a0, $at, L_80171410
    if (ctx->r4 != ctx->r1) {
        // 0x8017130C: or          $a3, $zero, $zero
        ctx->r7 = 0 | 0;
            goto L_80171410;
    }
    // 0x8017130C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x80171310: lh          $t7, 0x10($fp)
    ctx->r15 = MEM_H(ctx->r30, 0X10);
    // 0x80171314: lh          $t8, 0x16($fp)
    ctx->r24 = MEM_H(ctx->r30, 0X16);
    // 0x80171318: addiu       $v1, $fp, 0x10
    ctx->r3 = ADD32(ctx->r30, 0X10);
    // 0x8017131C: addiu       $a2, $fp, 0x16
    ctx->r6 = ADD32(ctx->r30, 0X16);
    // 0x80171320: bnel        $t7, $t8, L_80171380
    if (ctx->r15 != ctx->r24) {
        // 0x80171324: addiu       $a2, $fp, 0x16
        ctx->r6 = ADD32(ctx->r30, 0X16);
            goto L_80171380;
    }
    goto skip_3;
    // 0x80171324: addiu       $a2, $fp, 0x16
    ctx->r6 = ADD32(ctx->r30, 0X16);
    skip_3:
    // 0x80171328: lh          $t9, 0x4($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X4);
    // 0x8017132C: lh          $t5, 0x4($a2)
    ctx->r13 = MEM_H(ctx->r6, 0X4);
    // 0x80171330: addiu       $a1, $fp, 0x1C
    ctx->r5 = ADD32(ctx->r30, 0X1C);
    // 0x80171334: addiu       $v0, $fp, 0x22
    ctx->r2 = ADD32(ctx->r30, 0X22);
    // 0x80171338: bnel        $t9, $t5, L_80171380
    if (ctx->r25 != ctx->r13) {
        // 0x8017133C: addiu       $a2, $fp, 0x16
        ctx->r6 = ADD32(ctx->r30, 0X16);
            goto L_80171380;
    }
    goto skip_4;
    // 0x8017133C: addiu       $a2, $fp, 0x16
    ctx->r6 = ADD32(ctx->r30, 0X16);
    skip_4:
    // 0x80171340: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80171344: lh          $t7, 0x0($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X0);
    // 0x80171348: bnel        $t6, $t7, L_80171380
    if (ctx->r14 != ctx->r15) {
        // 0x8017134C: addiu       $a2, $fp, 0x16
        ctx->r6 = ADD32(ctx->r30, 0X16);
            goto L_80171380;
    }
    goto skip_5;
    // 0x8017134C: addiu       $a2, $fp, 0x16
    ctx->r6 = ADD32(ctx->r30, 0X16);
    skip_5:
    // 0x80171350: lh          $t8, 0x4($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X4);
    // 0x80171354: lh          $t9, 0x4($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X4);
    // 0x80171358: bnel        $t8, $t9, L_80171380
    if (ctx->r24 != ctx->r25) {
        // 0x8017135C: addiu       $a2, $fp, 0x16
        ctx->r6 = ADD32(ctx->r30, 0X16);
            goto L_80171380;
    }
    goto skip_6;
    // 0x8017135C: addiu       $a2, $fp, 0x16
    ctx->r6 = ADD32(ctx->r30, 0X16);
    skip_6:
    // 0x80171360: lh          $t5, 0x2($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X2);
    // 0x80171364: lh          $t6, 0x2($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X2);
    // 0x80171368: bnel        $t5, $t6, L_80171380
    if (ctx->r13 != ctx->r14) {
        // 0x8017136C: addiu       $a2, $fp, 0x16
        ctx->r6 = ADD32(ctx->r30, 0X16);
            goto L_80171380;
    }
    goto skip_7;
    // 0x8017136C: addiu       $a2, $fp, 0x16
    ctx->r6 = ADD32(ctx->r30, 0X16);
    skip_7:
    // 0x80171370: lh          $t7, 0x2($a2)
    ctx->r15 = MEM_H(ctx->r6, 0X2);
    // 0x80171374: lh          $t8, 0x2($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X2);
    // 0x80171378: beq         $t7, $t8, L_801713EC
    if (ctx->r15 == ctx->r24) {
        // 0x8017137C: addiu       $a2, $fp, 0x16
        ctx->r6 = ADD32(ctx->r30, 0X16);
            goto L_801713EC;
    }
    // 0x8017137C: addiu       $a2, $fp, 0x16
    ctx->r6 = ADD32(ctx->r30, 0X16);
L_80171380:
    // 0x80171380: addiu       $a1, $fp, 0x1C
    ctx->r5 = ADD32(ctx->r30, 0X1C);
    // 0x80171384: lh          $t5, 0x0($a1)
    ctx->r13 = MEM_H(ctx->r5, 0X0);
    // 0x80171388: lh          $t9, 0x0($a2)
    ctx->r25 = MEM_H(ctx->r6, 0X0);
    // 0x8017138C: bne         $t9, $t5, L_80171410
    if (ctx->r25 != ctx->r13) {
        // 0x80171390: nop
    
            goto L_80171410;
    }
    // 0x80171390: nop

    // 0x80171394: lh          $t6, 0x4($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X4);
    // 0x80171398: lh          $t7, 0x4($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X4);
    // 0x8017139C: addiu       $v0, $fp, 0x22
    ctx->r2 = ADD32(ctx->r30, 0X22);
    // 0x801713A0: addiu       $v1, $fp, 0x10
    ctx->r3 = ADD32(ctx->r30, 0X10);
    // 0x801713A4: bne         $t6, $t7, L_80171410
    if (ctx->r14 != ctx->r15) {
        // 0x801713A8: nop
    
            goto L_80171410;
    }
    // 0x801713A8: nop

    // 0x801713AC: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x801713B0: lh          $t9, 0x0($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X0);
    // 0x801713B4: bne         $t8, $t9, L_80171410
    if (ctx->r24 != ctx->r25) {
        // 0x801713B8: nop
    
            goto L_80171410;
    }
    // 0x801713B8: nop

    // 0x801713BC: lh          $t5, 0x4($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X4);
    // 0x801713C0: lh          $t6, 0x4($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X4);
    // 0x801713C4: bne         $t5, $t6, L_80171410
    if (ctx->r13 != ctx->r14) {
        // 0x801713C8: nop
    
            goto L_80171410;
    }
    // 0x801713C8: nop

    // 0x801713CC: lh          $t7, 0x2($a2)
    ctx->r15 = MEM_H(ctx->r6, 0X2);
    // 0x801713D0: lh          $t8, 0x2($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X2);
    // 0x801713D4: bne         $t7, $t8, L_80171410
    if (ctx->r15 != ctx->r24) {
        // 0x801713D8: nop
    
            goto L_80171410;
    }
    // 0x801713D8: nop

    // 0x801713DC: lh          $t9, 0x2($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X2);
    // 0x801713E0: lh          $t5, 0x2($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X2);
    // 0x801713E4: bne         $t9, $t5, L_80171410
    if (ctx->r25 != ctx->r13) {
        // 0x801713E8: nop
    
            goto L_80171410;
    }
    // 0x801713E8: nop

L_801713EC:
    // 0x801713EC: lwc1        $f6, 0x110($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X110);
    // 0x801713F0: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x801713F4: lw          $a1, 0x174($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X174);
    // 0x801713F8: lw          $a2, 0x178($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X178);
    // 0x801713FC: lw          $a3, 0x17C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X17C);
    // 0x80171400: jal         0x80170E98
    // 0x80171404: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    func_80170E98(rdram, ctx);
        goto after_0;
    // 0x80171404: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_0:
    // 0x80171408: b           L_80171E3C
    // 0x8017140C: lw          $ra, 0x74($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X74);
        goto L_80171E3C;
    // 0x8017140C: lw          $ra, 0x74($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X74);
L_80171410:
    // 0x80171410: blez        $a0, L_80171480
    if (SIGNED(ctx->r4) <= 0) {
        // 0x80171414: sw          $zero, 0x114($sp)
        MEM_W(0X114, ctx->r29) = 0;
            goto L_80171480;
    }
    // 0x80171414: sw          $zero, 0x114($sp)
    MEM_W(0X114, ctx->r29) = 0;
    // 0x80171418: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8017141C: addiu       $v0, $v0, -0x640
    ctx->r2 = ADD32(ctx->r2, -0X640);
    // 0x80171420: or          $v1, $fp, $zero
    ctx->r3 = ctx->r30 | 0;
    // 0x80171424: lh          $t6, 0x10($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X10);
L_80171428:
    // 0x80171428: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    // 0x8017142C: addiu       $v1, $v1, 0x6
    ctx->r3 = ADD32(ctx->r3, 0X6);
    // 0x80171430: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x80171434: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x80171438: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8017143C: swc1        $f4, -0xC($v0)
    MEM_W(-0XC, ctx->r2) = ctx->f4.u32l;
    // 0x80171440: lh          $t7, 0xC($v1)
    ctx->r15 = MEM_H(ctx->r3, 0XC);
    // 0x80171444: mtc1        $t7, $f8
    ctx->f8.u32l = ctx->r15;
    // 0x80171448: nop

    // 0x8017144C: cvt.s.w     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80171450: swc1        $f6, -0x8($v0)
    MEM_W(-0X8, ctx->r2) = ctx->f6.u32l;
    // 0x80171454: lh          $t8, 0xE($v1)
    ctx->r24 = MEM_H(ctx->r3, 0XE);
    // 0x80171458: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x8017145C: nop

    // 0x80171460: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80171464: swc1        $f4, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f4.u32l;
    // 0x80171468: lh          $a0, 0x28($fp)
    ctx->r4 = MEM_H(ctx->r30, 0X28);
    // 0x8017146C: slt         $at, $a3, $a0
    ctx->r1 = SIGNED(ctx->r7) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x80171470: bnel        $at, $zero, L_80171428
    if (ctx->r1 != 0) {
        // 0x80171474: lh          $t6, 0x10($v1)
        ctx->r14 = MEM_H(ctx->r3, 0X10);
            goto L_80171428;
    }
    goto skip_8;
    // 0x80171474: lh          $t6, 0x10($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X10);
    skip_8:
    // 0x80171478: lwc1        $f0, 0xE8($t0)
    ctx->f0.u32l = MEM_W(ctx->r8, 0XE8);
    // 0x8017147C: lwc1        $f14, 0xF0($t0)
    ctx->f14.u32l = MEM_W(ctx->r8, 0XF0);
L_80171480:
    // 0x80171480: lwc1        $f8, 0x12C($t0)
    ctx->f8.u32l = MEM_W(ctx->r8, 0X12C);
    // 0x80171484: lwc1        $f30, 0x120($t0)
    ctx->f30.u32l = MEM_W(ctx->r8, 0X120);
    // 0x80171488: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x8017148C: mov.s       $f12, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    ctx->f12.fl = ctx->f14.fl;
    // 0x80171490: blez        $a0, L_80171534
    if (SIGNED(ctx->r4) <= 0) {
        // 0x80171494: swc1        $f8, 0xF4($sp)
        MEM_W(0XF4, ctx->r29) = ctx->f8.u32l;
            goto L_80171534;
    }
    // 0x80171494: swc1        $f8, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->f8.u32l;
    // 0x80171498: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x8017149C: addiu       $v0, $v0, -0x640
    ctx->r2 = ADD32(ctx->r2, -0X640);
    // 0x801714A0: lwc1        $f0, 0x4($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X4);
L_801714A4:
    // 0x801714A4: sll         $t9, $a0, 2
    ctx->r25 = S32(ctx->r4 << 2);
    // 0x801714A8: subu        $t9, $t9, $a0
    ctx->r25 = SUB32(ctx->r25, ctx->r4);
    // 0x801714AC: c.lt.s      $f0, $f30
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 30);
    c1cs = ctx->f0.fl < ctx->f30.fl;
    // 0x801714B0: lui         $t5, 0x801A
    ctx->r13 = S32(0X801A << 16);
    // 0x801714B4: lwc1        $f6, 0xF4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XF4);
    // 0x801714B8: addiu       $t5, $t5, -0x640
    ctx->r13 = ADD32(ctx->r13, -0X640);
    // 0x801714BC: bc1t        L_801714DC
    if (c1cs) {
        // 0x801714C0: sll         $t9, $t9, 2
        ctx->r25 = S32(ctx->r25 << 2);
            goto L_801714DC;
    }
    // 0x801714C0: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x801714C4: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x801714C8: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x801714CC: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x801714D0: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x801714D4: bc1f        L_801714E4
    if (!c1cs) {
        // 0x801714D8: lui         $t7, 0x801A
        ctx->r15 = S32(0X801A << 16);
            goto L_801714E4;
    }
    // 0x801714D8: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
L_801714DC:
    // 0x801714DC: b           L_80171524
    // 0x801714E0: addu        $v1, $t9, $t5
    ctx->r3 = ADD32(ctx->r25, ctx->r13);
        goto L_80171524;
    // 0x801714E0: addu        $v1, $t9, $t5
    ctx->r3 = ADD32(ctx->r25, ctx->r13);
L_801714E4:
    // 0x801714E4: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801714E8: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801714EC: addiu       $t7, $t7, -0x640
    ctx->r15 = ADD32(ctx->r15, -0X640);
    // 0x801714F0: sub.s       $f16, $f10, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f10.fl - ctx->f2.fl;
    // 0x801714F4: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x801714F8: sub.s       $f18, $f4, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = ctx->f4.fl - ctx->f12.fl;
    // 0x801714FC: mul.s       $f8, $f16, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x80171500: lwc1        $f4, 0xE4($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0XE4);
    // 0x80171504: mul.s       $f6, $f18, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x80171508: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x8017150C: c.lt.s      $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f10.fl < ctx->f4.fl;
    // 0x80171510: nop

    // 0x80171514: bc1f        L_80171524
    if (!c1cs) {
        // 0x80171518: nop
    
            goto L_80171524;
    }
    // 0x80171518: nop

    // 0x8017151C: b           L_80171534
    // 0x80171520: addiu       $s6, $zero, 0x1
    ctx->r22 = ADD32(0, 0X1);
        goto L_80171534;
    // 0x80171520: addiu       $s6, $zero, 0x1
    ctx->r22 = ADD32(0, 0X1);
L_80171524:
    // 0x80171524: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x80171528: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x8017152C: bnel        $at, $zero, L_801714A4
    if (ctx->r1 != 0) {
        // 0x80171530: lwc1        $f0, 0x4($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X4);
            goto L_801714A4;
    }
    goto skip_9;
    // 0x80171530: lwc1        $f0, 0x4($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X4);
    skip_9:
L_80171534:
    // 0x80171534: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80171538: lui         $s4, 0x801A
    ctx->r20 = S32(0X801A << 16);
    // 0x8017153C: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171540: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x80171544: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171548: addiu       $s4, $s4, -0x640
    ctx->r20 = ADD32(ctx->r20, -0X640);
    // 0x8017154C: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80171550: addiu       $s5, $zero, 0xC
    ctx->r21 = ADD32(0, 0XC);
    // 0x80171554: addiu       $s0, $zero, 0x18
    ctx->r16 = ADD32(0, 0X18);
    // 0x80171558: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x8017155C: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
L_80171560:
    // 0x80171560: bnel        $s2, $zero, L_801715A4
    if (ctx->r18 != 0) {
        // 0x80171564: lh          $t7, 0x28($fp)
        ctx->r15 = MEM_H(ctx->r30, 0X28);
            goto L_801715A4;
    }
    goto skip_10;
    // 0x80171564: lh          $t7, 0x28($fp)
    ctx->r15 = MEM_H(ctx->r30, 0X28);
    skip_10:
    // 0x80171568: lbu         $t8, 0x38($fp)
    ctx->r24 = MEM_BU(ctx->r30, 0X38);
    // 0x8017156C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171570: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x80171574: sw          $t8, -0x658($at)
    MEM_W(-0X658, ctx->r1) = ctx->r24;
    // 0x80171578: lbu         $t9, 0x39($fp)
    ctx->r25 = MEM_BU(ctx->r30, 0X39);
    // 0x8017157C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171580: sw          $t9, -0x654($at)
    MEM_W(-0X654, ctx->r1) = ctx->r25;
    // 0x80171584: lbu         $t5, 0x3A($fp)
    ctx->r13 = MEM_BU(ctx->r30, 0X3A);
    // 0x80171588: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017158C: sw          $t5, -0x650($at)
    MEM_W(-0X650, ctx->r1) = ctx->r13;
    // 0x80171590: lbu         $t6, 0x3B($fp)
    ctx->r14 = MEM_BU(ctx->r30, 0X3B);
    // 0x80171594: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171598: b           L_801715E8
    // 0x8017159C: sw          $t6, -0x64C($at)
    MEM_W(-0X64C, ctx->r1) = ctx->r14;
        goto L_801715E8;
    // 0x8017159C: sw          $t6, -0x64C($at)
    MEM_W(-0X64C, ctx->r1) = ctx->r14;
    // 0x801715A0: lh          $t7, 0x28($fp)
    ctx->r15 = MEM_H(ctx->r30, 0X28);
L_801715A4:
    // 0x801715A4: bnel        $t2, $t7, L_801715B8
    if (ctx->r10 != ctx->r15) {
        // 0x801715A8: lbu         $t8, 0x3C($fp)
        ctx->r24 = MEM_BU(ctx->r30, 0X3C);
            goto L_801715B8;
    }
    goto skip_11;
    // 0x801715A8: lbu         $t8, 0x3C($fp)
    ctx->r24 = MEM_BU(ctx->r30, 0X3C);
    skip_11:
    // 0x801715AC: b           L_80171E38
    // 0x801715B0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80171E38;
    // 0x801715B0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801715B4: lbu         $t8, 0x3C($fp)
    ctx->r24 = MEM_BU(ctx->r30, 0X3C);
L_801715B8:
    // 0x801715B8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801715BC: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x801715C0: sw          $t8, -0x658($at)
    MEM_W(-0X658, ctx->r1) = ctx->r24;
    // 0x801715C4: lbu         $t9, 0x3D($fp)
    ctx->r25 = MEM_BU(ctx->r30, 0X3D);
    // 0x801715C8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801715CC: sw          $t9, -0x654($at)
    MEM_W(-0X654, ctx->r1) = ctx->r25;
    // 0x801715D0: lbu         $t5, 0x3E($fp)
    ctx->r13 = MEM_BU(ctx->r30, 0X3E);
    // 0x801715D4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801715D8: sw          $t5, -0x650($at)
    MEM_W(-0X650, ctx->r1) = ctx->r13;
    // 0x801715DC: lbu         $t6, 0x3F($fp)
    ctx->r14 = MEM_BU(ctx->r30, 0X3F);
    // 0x801715E0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801715E4: sw          $t6, -0x64C($at)
    MEM_W(-0X64C, ctx->r1) = ctx->r14;
L_801715E8:
    // 0x801715E8: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x801715EC: lw          $t7, -0x658($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X658);
    // 0x801715F0: lui         $t9, 0x801A
    ctx->r25 = S32(0X801A << 16);
    // 0x801715F4: lw          $t9, -0x654($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X654);
    // 0x801715F8: multu       $t7, $s5
    result = U64(U32(ctx->r15)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801715FC: mflo        $t8
    ctx->r24 = lo;
    // 0x80171600: addu        $s3, $s4, $t8
    ctx->r19 = ADD32(ctx->r20, ctx->r24);
    // 0x80171604: nop

    // 0x80171608: multu       $t9, $s5
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8017160C: mflo        $t5
    ctx->r13 = lo;
    // 0x80171610: addu        $s1, $s4, $t5
    ctx->r17 = ADD32(ctx->r20, ctx->r13);
    // 0x80171614: bnel        $s6, $zero, L_80171780
    if (ctx->r22 != 0) {
        // 0x80171618: addiu       $v0, $sp, 0x15C
        ctx->r2 = ADD32(ctx->r29, 0X15C);
            goto L_80171780;
    }
    goto skip_12;
    // 0x80171618: addiu       $v0, $sp, 0x15C
    ctx->r2 = ADD32(ctx->r29, 0X15C);
    skip_12:
L_8017161C:
    // 0x8017161C: bnel        $s6, $zero, L_80171774
    if (ctx->r22 != 0) {
        // 0x80171620: addiu       $a3, $a3, 0x1
        ctx->r7 = ADD32(ctx->r7, 0X1);
            goto L_80171774;
    }
    goto skip_13;
    // 0x80171620: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    skip_13:
    // 0x80171624: bne         $a3, $zero, L_80171630
    if (ctx->r7 != 0) {
        // 0x80171628: sll         $t6, $s2, 2
        ctx->r14 = S32(ctx->r18 << 2);
            goto L_80171630;
    }
    // 0x80171628: sll         $t6, $s2, 2
    ctx->r14 = S32(ctx->r18 << 2);
    // 0x8017162C: bne         $s2, $zero, L_80171770
    if (ctx->r18 != 0) {
        // 0x80171630: subu        $t6, $t6, $s2
        ctx->r14 = SUB32(ctx->r14, ctx->r18);
            goto L_80171770;
    }
L_80171630:
    // 0x80171630: subu        $t6, $t6, $s2
    ctx->r14 = SUB32(ctx->r14, ctx->r18);
    // 0x80171634: sll         $t6, $t6, 3
    ctx->r14 = S32(ctx->r14 << 3);
    // 0x80171638: addu        $t7, $t4, $t6
    ctx->r15 = ADD32(ctx->r12, ctx->r14);
    // 0x8017163C: sll         $t8, $a3, 3
    ctx->r24 = S32(ctx->r7 << 3);
    // 0x80171640: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x80171644: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X0);
    // 0x80171648: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    // 0x8017164C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171650: multu       $t9, $s5
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80171654: mflo        $t5
    ctx->r13 = lo;
    // 0x80171658: addu        $a0, $s4, $t5
    ctx->r4 = ADD32(ctx->r20, ctx->r13);
    // 0x8017165C: lwc1        $f6, 0x4($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80171660: multu       $t6, $s5
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80171664: mflo        $t7
    ctx->r15 = lo;
    // 0x80171668: addu        $a1, $s4, $t7
    ctx->r5 = ADD32(ctx->r20, ctx->r15);
    // 0x8017166C: lwc1        $f8, 0x4($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80171670: sub.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x80171674: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x80171678: swc1        $f10, 0x160($sp)
    MEM_W(0X160, ctx->r29) = ctx->f10.u32l;
    // 0x8017167C: lwc1        $f4, 0x160($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X160);
    // 0x80171680: lwc1        $f0, 0x160($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X160);
    // 0x80171684: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x80171688: nop

    // 0x8017168C: bc1f        L_8017169C
    if (!c1cs) {
        // 0x80171690: nop
    
            goto L_8017169C;
    }
    // 0x80171690: nop

    // 0x80171694: b           L_8017169C
    // 0x80171698: neg.s       $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = -ctx->f4.fl;
        goto L_8017169C;
    // 0x80171698: neg.s       $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = -ctx->f4.fl;
L_8017169C:
    // 0x8017169C: lwc1        $f6, -0x50EC($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X50EC);
    // 0x801716A0: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x801716A4: nop

    // 0x801716A8: bc1tl       L_80171774
    if (c1cs) {
        // 0x801716AC: addiu       $a3, $a3, 0x1
        ctx->r7 = ADD32(ctx->r7, 0X1);
            goto L_80171774;
    }
    goto skip_14;
    // 0x801716AC: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    skip_14:
    // 0x801716B0: lwc1        $f10, 0x0($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801716B4: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801716B8: lw          $t0, 0x0($t1)
    ctx->r8 = MEM_W(ctx->r9, 0X0);
    // 0x801716BC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801716C0: sub.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x801716C4: or          $v1, $t0, $zero
    ctx->r3 = ctx->r8 | 0;
    // 0x801716C8: swc1        $f4, 0x15C($sp)
    MEM_W(0X15C, ctx->r29) = ctx->f4.u32l;
    // 0x801716CC: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x801716D0: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801716D4: sub.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x801716D8: swc1        $f8, 0x164($sp)
    MEM_W(0X164, ctx->r29) = ctx->f8.u32l;
    // 0x801716DC: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801716E0: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x801716E4: lwc1        $f6, 0xE8($t0)
    ctx->f6.u32l = MEM_W(ctx->r8, 0XE8);
    // 0x801716E8: lwc1        $f8, 0xF0($t0)
    ctx->f8.u32l = MEM_W(ctx->r8, 0XF0);
    // 0x801716EC: lwc1        $f14, 0x4($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X4);
    // 0x801716F0: sub.s       $f16, $f4, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801716F4: sub.s       $f18, $f10, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = ctx->f10.fl - ctx->f8.fl;
L_801716F8:
    // 0x801716F8: lwc1        $f4, 0x120($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X120);
    // 0x801716FC: lwc1        $f10, 0x160($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X160);
    // 0x80171700: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x80171704: sub.s       $f6, $f4, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f14.fl;
    // 0x80171708: div.s       $f0, $f6, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8017170C: c.le.s      $f22, $f0
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f22.fl <= ctx->f0.fl;
    // 0x80171710: nop

    // 0x80171714: bc1f        L_80171768
    if (!c1cs) {
        // 0x80171718: nop
    
            goto L_80171768;
    }
    // 0x80171718: nop

    // 0x8017171C: c.le.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl <= ctx->f20.fl;
    // 0x80171720: lwc1        $f8, 0x15C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X15C);
    // 0x80171724: bc1f        L_80171768
    if (!c1cs) {
        // 0x80171728: nop
    
            goto L_80171768;
    }
    // 0x80171728: nop

    // 0x8017172C: mul.s       $f4, $f8, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x80171730: lwc1        $f6, 0x164($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X164);
    // 0x80171734: mul.s       $f10, $f6, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80171738: add.s       $f2, $f4, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f2.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x8017173C: add.s       $f12, $f10, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f18.fl;
    // 0x80171740: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80171744: lwc1        $f10, 0xE4($t0)
    ctx->f10.u32l = MEM_W(ctx->r8, 0XE4);
    // 0x80171748: mul.s       $f4, $f12, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x8017174C: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80171750: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x80171754: nop

    // 0x80171758: bc1f        L_80171768
    if (!c1cs) {
        // 0x8017175C: nop
    
            goto L_80171768;
    }
    // 0x8017175C: nop

    // 0x80171760: b           L_80171770
    // 0x80171764: addiu       $s6, $zero, 0x1
    ctx->r22 = ADD32(0, 0X1);
        goto L_80171770;
    // 0x80171764: addiu       $s6, $zero, 0x1
    ctx->r22 = ADD32(0, 0X1);
L_80171768:
    // 0x80171768: bne         $v0, $s0, L_801716F8
    if (ctx->r2 != ctx->r16) {
        // 0x8017176C: addiu       $v1, $v1, 0xC
        ctx->r3 = ADD32(ctx->r3, 0XC);
            goto L_801716F8;
    }
    // 0x8017176C: addiu       $v1, $v1, 0xC
    ctx->r3 = ADD32(ctx->r3, 0XC);
L_80171770:
    // 0x80171770: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
L_80171774:
    // 0x80171774: bne         $a3, $t2, L_8017161C
    if (ctx->r7 != ctx->r10) {
        // 0x80171778: nop
    
            goto L_8017161C;
    }
    // 0x80171778: nop

    // 0x8017177C: addiu       $v0, $sp, 0x15C
    ctx->r2 = ADD32(ctx->r29, 0X15C);
L_80171780:
    // 0x80171780: or          $v1, $s1, $zero
    ctx->r3 = ctx->r17 | 0;
    // 0x80171784: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
L_80171788:
    // 0x80171788: lwc1        $f8, 0x0($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X0);
    // 0x8017178C: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80171790: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80171794: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80171798: sub.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x8017179C: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x801717A0: bne         $v0, $t3, L_80171788
    if (ctx->r2 != ctx->r11) {
        // 0x801717A4: swc1        $f6, -0x4($v0)
        MEM_W(-0X4, ctx->r2) = ctx->f6.u32l;
            goto L_80171788;
    }
    // 0x801717A4: swc1        $f6, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f6.u32l;
    // 0x801717A8: lwc1        $f14, 0x8($fp)
    ctx->f14.u32l = MEM_W(ctx->r30, 0X8);
    // 0x801717AC: lwc1        $f10, 0x15C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X15C);
    // 0x801717B0: lwc1        $f12, 0x0($fp)
    ctx->f12.u32l = MEM_W(ctx->r30, 0X0);
    // 0x801717B4: lwc1        $f4, 0x164($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X164);
    // 0x801717B8: mul.s       $f8, $f10, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f14.fl);
    // 0x801717BC: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801717C0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801717C4: mul.s       $f6, $f12, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f4.fl);
    // 0x801717C8: sub.s       $f2, $f8, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x801717CC: c.lt.s      $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f2.fl < ctx->f10.fl;
    // 0x801717D0: nop

    // 0x801717D4: bc1fl       L_801717E8
    if (!c1cs) {
        // 0x801717D8: mov.s       $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
            goto L_801717E8;
    }
    goto skip_15;
    // 0x801717D8: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    skip_15:
    // 0x801717DC: b           L_801717E8
    // 0x801717E0: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
        goto L_801717E8;
    // 0x801717E0: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x801717E4: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_801717E8:
    // 0x801717E8: lwc1        $f4, -0x50E8($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X50E8);
    // 0x801717EC: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x801717F0: nop

    // 0x801717F4: bc1fl       L_80171808
    if (!c1cs) {
        // 0x801717F8: lw          $t0, 0x0($t1)
        ctx->r8 = MEM_W(ctx->r9, 0X0);
            goto L_80171808;
    }
    goto skip_16;
    // 0x801717F8: lw          $t0, 0x0($t1)
    ctx->r8 = MEM_W(ctx->r9, 0X0);
    skip_16:
    // 0x801717FC: b           L_80171E10
    // 0x80171800: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
        goto L_80171E10;
    // 0x80171800: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x80171804: lw          $t0, 0x0($t1)
    ctx->r8 = MEM_W(ctx->r9, 0X0);
L_80171808:
    // 0x80171808: div.s       $f2, $f20, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = DIV_S(ctx->f20.fl, ctx->f2.fl);
    // 0x8017180C: lwc1        $f6, 0x8($s3)
    ctx->f6.u32l = MEM_W(ctx->r19, 0X8);
    // 0x80171810: lwc1        $f10, 0xF0($t0)
    ctx->f10.u32l = MEM_W(ctx->r8, 0XF0);
    // 0x80171814: lwc1        $f0, 0x0($s3)
    ctx->f0.u32l = MEM_W(ctx->r19, 0X0);
    // 0x80171818: lwc1        $f8, 0xE8($t0)
    ctx->f8.u32l = MEM_W(ctx->r8, 0XE8);
    // 0x8017181C: sub.s       $f26, $f6, $f10
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x80171820: lwc1        $f10, 0x15C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X15C);
    // 0x80171824: sub.s       $f24, $f0, $f8
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f24.fl = ctx->f0.fl - ctx->f8.fl;
    // 0x80171828: mul.s       $f4, $f26, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f26.fl, ctx->f12.fl);
    // 0x8017182C: nop

    // 0x80171830: mul.s       $f8, $f14, $f24
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f24.fl);
    // 0x80171834: sub.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x80171838: mul.s       $f28, $f6, $f2
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f28.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8017183C: lwc1        $f6, 0x164($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X164);
    // 0x80171840: mul.s       $f4, $f10, $f28
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f28.fl);
    // 0x80171844: c.lt.s      $f28, $f22
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f28.fl < ctx->f22.fl;
    // 0x80171848: mul.s       $f10, $f6, $f28
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f28.fl);
    // 0x8017184C: add.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f0.fl;
    // 0x80171850: swc1        $f8, 0xE0($sp)
    MEM_W(0XE0, ctx->r29) = ctx->f8.u32l;
    // 0x80171854: lwc1        $f4, 0x8($s3)
    ctx->f4.u32l = MEM_W(ctx->r19, 0X8);
    // 0x80171858: add.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8017185C: bc1t        L_80171878
    if (c1cs) {
        // 0x80171860: swc1        $f8, 0xE8($sp)
        MEM_W(0XE8, ctx->r29) = ctx->f8.u32l;
            goto L_80171878;
    }
    // 0x80171860: swc1        $f8, 0xE8($sp)
    MEM_W(0XE8, ctx->r29) = ctx->f8.u32l;
    // 0x80171864: c.lt.s      $f20, $f28
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 28);
    c1cs = ctx->f20.fl < ctx->f28.fl;
    // 0x80171868: lwc1        $f0, 0x10C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X10C);
    // 0x8017186C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171870: bc1fl       L_80171940
    if (!c1cs) {
        // 0x80171874: mtc1        $zero, $f10
        ctx->f10.u32l = 0;
            goto L_80171940;
    }
    goto skip_17;
    // 0x80171874: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    skip_17:
L_80171878:
    // 0x80171878: c.lt.s      $f28, $f22
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 22);
    c1cs = ctx->f28.fl < ctx->f22.fl;
    // 0x8017187C: lwc1        $f4, 0x15C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X15C);
    // 0x80171880: bc1fl       L_801718A8
    if (!c1cs) {
        // 0x80171884: lwc1        $f6, 0x0($s1)
        ctx->f6.u32l = MEM_W(ctx->r17, 0X0);
            goto L_801718A8;
    }
    goto skip_18;
    // 0x80171884: lwc1        $f6, 0x0($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X0);
    skip_18:
    // 0x80171888: lwc1        $f6, 0x0($s3)
    ctx->f6.u32l = MEM_W(ctx->r19, 0X0);
    // 0x8017188C: mov.s       $f16, $f24
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 24);
    ctx->f16.fl = ctx->f24.fl;
    // 0x80171890: mov.s       $f18, $f26
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 26);
    ctx->f18.fl = ctx->f26.fl;
    // 0x80171894: swc1        $f6, 0x150($sp)
    MEM_W(0X150, ctx->r29) = ctx->f6.u32l;
    // 0x80171898: lwc1        $f10, 0x8($s3)
    ctx->f10.u32l = MEM_W(ctx->r19, 0X8);
    // 0x8017189C: b           L_801718C0
    // 0x801718A0: swc1        $f10, 0x158($sp)
    MEM_W(0X158, ctx->r29) = ctx->f10.u32l;
        goto L_801718C0;
    // 0x801718A0: swc1        $f10, 0x158($sp)
    MEM_W(0X158, ctx->r29) = ctx->f10.u32l;
    // 0x801718A4: lwc1        $f6, 0x0($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X0);
L_801718A8:
    // 0x801718A8: lwc1        $f8, 0x164($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X164);
    // 0x801718AC: add.s       $f16, $f4, $f24
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f16.fl = ctx->f4.fl + ctx->f24.fl;
    // 0x801718B0: swc1        $f6, 0x150($sp)
    MEM_W(0X150, ctx->r29) = ctx->f6.u32l;
    // 0x801718B4: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x801718B8: add.s       $f18, $f8, $f26
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f18.fl = ctx->f8.fl + ctx->f26.fl;
    // 0x801718BC: swc1        $f10, 0x158($sp)
    MEM_W(0X158, ctx->r29) = ctx->f10.u32l;
L_801718C0:
    // 0x801718C0: mul.s       $f4, $f16, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f16.fl);
    // 0x801718C4: nop

    // 0x801718C8: mul.s       $f8, $f18, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x801718CC: jal         0x800A01E0
    // 0x801718D0: add.s       $f12, $f4, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x801718D0: add.s       $f12, $f4, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f8.fl;
    after_1:
    // 0x801718D4: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x801718D8: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x801718DC: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x801718E0: c.eq.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl == ctx->f6.fl;
    // 0x801718E4: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x801718E8: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x801718EC: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x801718F0: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x801718F4: bc1f        L_80171908
    if (!c1cs) {
        // 0x801718F8: swc1        $f0, 0x108($sp)
        MEM_W(0X108, ctx->r29) = ctx->f0.u32l;
            goto L_80171908;
    }
    // 0x801718F8: swc1        $f0, 0x108($sp)
    MEM_W(0X108, ctx->r29) = ctx->f0.u32l;
    // 0x801718FC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171900: lwc1        $f10, -0x50E4($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X50E4);
    // 0x80171904: swc1        $f10, 0x108($sp)
    MEM_W(0X108, ctx->r29) = ctx->f10.u32l;
L_80171908:
    // 0x80171908: lw          $t0, 0x0($t1)
    ctx->r8 = MEM_W(ctx->r9, 0X0);
    // 0x8017190C: lwc1        $f8, 0x108($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X108);
    // 0x80171910: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x80171914: lwc1        $f4, 0xE0($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0XE0);
    // 0x80171918: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017191C: c.lt.s      $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f4.fl < ctx->f8.fl;
    // 0x80171920: nop

    // 0x80171924: bc1f        L_80171934
    if (!c1cs) {
        // 0x80171928: nop
    
            goto L_80171934;
    }
    // 0x80171928: nop

    // 0x8017192C: b           L_80171E10
    // 0x80171930: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
        goto L_80171E10;
    // 0x80171930: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80171934:
    // 0x80171934: b           L_8017197C
    // 0x80171938: sh          $t8, -0x680($at)
    MEM_H(-0X680, ctx->r1) = ctx->r24;
        goto L_8017197C;
    // 0x80171938: sh          $t8, -0x680($at)
    MEM_H(-0X680, ctx->r1) = ctx->r24;
    // 0x8017193C: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
L_80171940:
    // 0x80171940: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80171944: c.eq.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl == ctx->f10.fl;
    // 0x80171948: nop

    // 0x8017194C: bc1t        L_80171958
    if (c1cs) {
        // 0x80171950: nop
    
            goto L_80171958;
    }
    // 0x80171950: nop

    // 0x80171954: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80171958:
    // 0x80171958: bne         $v0, $zero, L_80171968
    if (ctx->r2 != 0) {
        // 0x8017195C: sh          $zero, -0x680($at)
        MEM_H(-0X680, ctx->r1) = 0;
            goto L_80171968;
    }
    // 0x8017195C: sh          $zero, -0x680($at)
    MEM_H(-0X680, ctx->r1) = 0;
    // 0x80171960: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171964: lwc1        $f0, -0x50E0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X50E0);
L_80171968:
    // 0x80171968: lwc1        $f4, 0xE0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XE0);
    // 0x8017196C: lwc1        $f8, 0xE8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XE8);
    // 0x80171970: swc1        $f0, 0x108($sp)
    MEM_W(0X108, ctx->r29) = ctx->f0.u32l;
    // 0x80171974: swc1        $f4, 0x150($sp)
    MEM_W(0X150, ctx->r29) = ctx->f4.u32l;
    // 0x80171978: swc1        $f8, 0x158($sp)
    MEM_W(0X158, ctx->r29) = ctx->f8.u32l;
L_8017197C:
    // 0x8017197C: bnel        $s6, $zero, L_80171D08
    if (ctx->r22 != 0) {
        // 0x80171980: lw          $t9, 0x114($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X114);
            goto L_80171D08;
    }
    goto skip_19;
    // 0x80171980: lw          $t9, 0x114($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X114);
    skip_19:
    // 0x80171984: lwc1        $f0, 0x0($s3)
    ctx->f0.u32l = MEM_W(ctx->r19, 0X0);
    // 0x80171988: lwc1        $f2, 0x0($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X0);
    // 0x8017198C: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x80171990: nop

    // 0x80171994: bc1fl       L_801719AC
    if (!c1cs) {
        // 0x80171998: mov.s       $f30, $f2
        CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 2);
    ctx->f30.fl = ctx->f2.fl;
            goto L_801719AC;
    }
    goto skip_20;
    // 0x80171998: mov.s       $f30, $f2
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 2);
    ctx->f30.fl = ctx->f2.fl;
    skip_20:
    // 0x8017199C: mov.s       $f30, $f0
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 0);
    ctx->f30.fl = ctx->f0.fl;
    // 0x801719A0: b           L_801719B0
    // 0x801719A4: swc1        $f2, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->f2.u32l;
        goto L_801719B0;
    // 0x801719A4: swc1        $f2, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->f2.u32l;
    // 0x801719A8: mov.s       $f30, $f2
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 2);
    ctx->f30.fl = ctx->f2.fl;
L_801719AC:
    // 0x801719AC: swc1        $f0, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->f0.u32l;
L_801719B0:
    // 0x801719B0: lwc1        $f0, 0x8($s3)
    ctx->f0.u32l = MEM_W(ctx->r19, 0X8);
    // 0x801719B4: lwc1        $f2, 0x8($s1)
    ctx->f2.u32l = MEM_W(ctx->r17, 0X8);
    // 0x801719B8: lwc1        $f6, 0x108($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X108);
    // 0x801719BC: lwc1        $f4, 0x160($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X160);
    // 0x801719C0: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x801719C4: nop

    // 0x801719C8: bc1fl       L_801719E0
    if (!c1cs) {
        // 0x801719CC: mov.s       $f26, $f2
        CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    ctx->f26.fl = ctx->f2.fl;
            goto L_801719E0;
    }
    goto skip_21;
    // 0x801719CC: mov.s       $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    ctx->f26.fl = ctx->f2.fl;
    skip_21:
    // 0x801719D0: mov.s       $f26, $f0
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    ctx->f26.fl = ctx->f0.fl;
    // 0x801719D4: b           L_801719E4
    // 0x801719D8: mov.s       $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    ctx->f14.fl = ctx->f2.fl;
        goto L_801719E4;
    // 0x801719D8: mov.s       $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    ctx->f14.fl = ctx->f2.fl;
    // 0x801719DC: mov.s       $f26, $f2
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 2);
    ctx->f26.fl = ctx->f2.fl;
L_801719E0:
    // 0x801719E0: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
L_801719E4:
    // 0x801719E4: lwc1        $f10, 0xE0($t0)
    ctx->f10.u32l = MEM_W(ctx->r8, 0XE0);
    // 0x801719E8: c.eq.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl == ctx->f10.fl;
    // 0x801719EC: lwc1        $f6, 0x164($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X164);
    // 0x801719F0: bc1f        L_80171A4C
    if (!c1cs) {
        // 0x801719F4: nop
    
            goto L_80171A4C;
    }
    // 0x801719F4: nop

    // 0x801719F8: mul.s       $f8, $f4, $f28
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f28.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f28.fl);
    // 0x801719FC: lwc1        $f6, 0x4($s3)
    ctx->f6.u32l = MEM_W(ctx->r19, 0X4);
    // 0x80171A00: lwc1        $f4, 0xE0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XE0);
    // 0x80171A04: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80171A08: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80171A0C: addiu       $a3, $sp, 0x15C
    ctx->r7 = ADD32(ctx->r29, 0X15C);
    // 0x80171A10: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    // 0x80171A14: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80171A18: lwc1        $f8, 0xE8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XE8);
    // 0x80171A1C: mfc1        $a2, $f10
    ctx->r6 = (int32_t)ctx->f10.u32l;
    // 0x80171A20: jal         0x80171E80
    // 0x80171A24: swc1        $f8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f8.u32l;
    func_80171E80(rdram, ctx);
        goto after_2;
    // 0x80171A24: swc1        $f8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f8.u32l;
    after_2:
    // 0x80171A28: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171A2C: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171A30: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171A34: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171A38: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x80171A3C: bne         $v0, $zero, L_80171D04
    if (ctx->r2 != 0) {
        // 0x80171A40: addiu       $t3, $sp, 0x168
        ctx->r11 = ADD32(ctx->r29, 0X168);
            goto L_80171D04;
    }
    // 0x80171A40: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x80171A44: b           L_80171E10
    // 0x80171A48: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
        goto L_80171E10;
    // 0x80171A48: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80171A4C:
    // 0x80171A4C: mul.s       $f10, $f6, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f6.fl);
    // 0x80171A50: lwc1        $f4, 0x15C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X15C);
    // 0x80171A54: lwc1        $f0, 0x110($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X110);
    // 0x80171A58: addiu       $s7, $zero, 0x2
    ctx->r23 = ADD32(0, 0X2);
    // 0x80171A5C: mul.s       $f8, $f4, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f4.fl);
    // 0x80171A60: lwc1        $f4, 0xE4($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0XE4);
    // 0x80171A64: swc1        $f14, 0xEC($sp)
    MEM_W(0XEC, ctx->r29) = ctx->f14.u32l;
    // 0x80171A68: addiu       $s1, $zero, 0x2
    ctx->r17 = ADD32(0, 0X2);
    // 0x80171A6C: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80171A70: mul.s       $f10, $f0, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80171A74: sub.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f10.fl;
    // 0x80171A78: jal         0x80172D34
    // 0x80171A7C: div.s       $f12, $f8, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = DIV_S(ctx->f8.fl, ctx->f6.fl);
    func_80172D34(rdram, ctx);
        goto after_3;
    // 0x80171A7C: div.s       $f12, $f8, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = DIV_S(ctx->f8.fl, ctx->f6.fl);
    after_3:
    // 0x80171A80: lwc1        $f2, 0x15C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X15C);
    // 0x80171A84: lwc1        $f12, 0x164($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X164);
    // 0x80171A88: lwc1        $f4, 0xE0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XE0);
    // 0x80171A8C: mul.s       $f16, $f2, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f2.fl, ctx->f0.fl);
    // 0x80171A90: lwc1        $f6, 0xE8($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XE8);
    // 0x80171A94: c.le.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl <= ctx->f2.fl;
    // 0x80171A98: mul.s       $f18, $f12, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x80171A9C: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171AA0: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171AA4: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171AA8: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171AAC: add.s       $f10, $f16, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f16.fl + ctx->f4.fl;
    // 0x80171AB0: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x80171AB4: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x80171AB8: sub.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f16.fl;
    // 0x80171ABC: swc1        $f10, 0xD4($sp)
    MEM_W(0XD4, ctx->r29) = ctx->f10.u32l;
    // 0x80171AC0: lwc1        $f14, 0xEC($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0XEC);
    // 0x80171AC4: add.s       $f10, $f18, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f18.fl + ctx->f6.fl;
    // 0x80171AC8: swc1        $f8, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f8.u32l;
    // 0x80171ACC: sub.s       $f4, $f6, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f18.fl;
    // 0x80171AD0: swc1        $f10, 0xDC($sp)
    MEM_W(0XDC, ctx->r29) = ctx->f10.u32l;
    // 0x80171AD4: lwc1        $f10, 0xD4($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XD4);
    // 0x80171AD8: bc1f        L_80171BEC
    if (!c1cs) {
        // 0x80171ADC: swc1        $f4, 0xD0($sp)
        MEM_W(0XD0, ctx->r29) = ctx->f4.u32l;
            goto L_80171BEC;
    }
    // 0x80171ADC: swc1        $f4, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f4.u32l;
    // 0x80171AE0: c.le.s      $f30, $f10
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f30.fl <= ctx->f10.fl;
    // 0x80171AE4: lwc1        $f8, 0x160($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X160);
    // 0x80171AE8: lwc1        $f6, 0xF4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XF4);
    // 0x80171AEC: bc1f        L_80171B54
    if (!c1cs) {
        // 0x80171AF0: div.s       $f24, $f8, $f2
        CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = DIV_S(ctx->f8.fl, ctx->f2.fl);
            goto L_80171B54;
    }
    // 0x80171AF0: div.s       $f24, $f8, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = DIV_S(ctx->f8.fl, ctx->f2.fl);
    // 0x80171AF4: c.le.s      $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f10.fl <= ctx->f6.fl;
    // 0x80171AF8: nop

    // 0x80171AFC: bc1fl       L_80171B58
    if (!c1cs) {
        // 0x80171B00: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171B58;
    }
    goto skip_22;
    // 0x80171B00: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_22:
    // 0x80171B04: lwc1        $f4, 0x0($s3)
    ctx->f4.u32l = MEM_W(ctx->r19, 0X0);
    // 0x80171B08: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80171B0C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80171B10: sub.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x80171B14: lwc1        $f4, 0x4($s3)
    ctx->f4.u32l = MEM_W(ctx->r19, 0X4);
    // 0x80171B18: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    // 0x80171B1C: addiu       $a3, $sp, 0x15C
    ctx->r7 = ADD32(ctx->r29, 0X15C);
    // 0x80171B20: mul.s       $f6, $f24, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f24.fl, ctx->f8.fl);
    // 0x80171B24: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80171B28: lwc1        $f4, 0xDC($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XDC);
    // 0x80171B2C: mfc1        $a2, $f8
    ctx->r6 = (int32_t)ctx->f8.u32l;
    // 0x80171B30: jal         0x80171E80
    // 0x80171B34: swc1        $f4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f4.u32l;
    func_80171E80(rdram, ctx);
        goto after_4;
    // 0x80171B34: swc1        $f4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f4.u32l;
    after_4:
    // 0x80171B38: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171B3C: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171B40: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171B44: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171B48: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x80171B4C: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x80171B50: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_80171B54:
    // 0x80171B54: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
L_80171B58:
    // 0x80171B58: beq         $s1, $at, L_80171BD0
    if (ctx->r17 == ctx->r1) {
        // 0x80171B5C: lwc1        $f6, 0xC8($sp)
        ctx->f6.u32l = MEM_W(ctx->r29, 0XC8);
            goto L_80171BD0;
    }
    // 0x80171B5C: lwc1        $f6, 0xC8($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XC8);
    // 0x80171B60: c.le.s      $f30, $f6
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f30.fl <= ctx->f6.fl;
    // 0x80171B64: lwc1        $f8, 0xF4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XF4);
    // 0x80171B68: bc1fl       L_80171BD4
    if (!c1cs) {
        // 0x80171B6C: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171BD4;
    }
    goto skip_23;
    // 0x80171B6C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_23:
    // 0x80171B70: c.le.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl <= ctx->f8.fl;
    // 0x80171B74: nop

    // 0x80171B78: bc1fl       L_80171BD4
    if (!c1cs) {
        // 0x80171B7C: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171BD4;
    }
    goto skip_24;
    // 0x80171B7C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_24:
    // 0x80171B80: lwc1        $f10, 0x0($s3)
    ctx->f10.u32l = MEM_W(ctx->r19, 0X0);
    // 0x80171B84: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80171B88: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80171B8C: sub.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x80171B90: lwc1        $f10, 0x4($s3)
    ctx->f10.u32l = MEM_W(ctx->r19, 0X4);
    // 0x80171B94: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    // 0x80171B98: addiu       $a3, $sp, 0x15C
    ctx->r7 = ADD32(ctx->r29, 0X15C);
    // 0x80171B9C: mul.s       $f8, $f24, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = MUL_S(ctx->f24.fl, ctx->f4.fl);
    // 0x80171BA0: add.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80171BA4: lwc1        $f10, 0xD0($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XD0);
    // 0x80171BA8: mfc1        $a2, $f4
    ctx->r6 = (int32_t)ctx->f4.u32l;
    // 0x80171BAC: jal         0x80171E80
    // 0x80171BB0: swc1        $f10, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f10.u32l;
    func_80171E80(rdram, ctx);
        goto after_5;
    // 0x80171BB0: swc1        $f10, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f10.u32l;
    after_5:
    // 0x80171BB4: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171BB8: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171BBC: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171BC0: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171BC4: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x80171BC8: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x80171BCC: or          $s7, $v0, $zero
    ctx->r23 = ctx->r2 | 0;
L_80171BD0:
    // 0x80171BD0: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
L_80171BD4:
    // 0x80171BD4: beq         $s1, $at, L_80171D04
    if (ctx->r17 == ctx->r1) {
        // 0x80171BD8: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171D04;
    }
    // 0x80171BD8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80171BDC: beql        $s7, $at, L_80171D08
    if (ctx->r23 == ctx->r1) {
        // 0x80171BE0: lw          $t9, 0x114($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X114);
            goto L_80171D08;
    }
    goto skip_25;
    // 0x80171BE0: lw          $t9, 0x114($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X114);
    skip_25:
    // 0x80171BE4: b           L_80171E10
    // 0x80171BE8: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
        goto L_80171E10;
    // 0x80171BE8: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80171BEC:
    // 0x80171BEC: lwc1        $f4, 0xDC($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XDC);
    // 0x80171BF0: lwc1        $f8, 0x160($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X160);
    // 0x80171BF4: c.le.s      $f26, $f4
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f26.fl <= ctx->f4.fl;
    // 0x80171BF8: div.s       $f24, $f8, $f12
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f24.fl = DIV_S(ctx->f8.fl, ctx->f12.fl);
    // 0x80171BFC: bc1fl       L_80171C70
    if (!c1cs) {
        // 0x80171C00: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171C70;
    }
    goto skip_26;
    // 0x80171C00: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_26:
    // 0x80171C04: c.le.s      $f4, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f4.fl <= ctx->f14.fl;
    // 0x80171C08: nop

    // 0x80171C0C: bc1fl       L_80171C70
    if (!c1cs) {
        // 0x80171C10: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171C70;
    }
    goto skip_27;
    // 0x80171C10: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_27:
    // 0x80171C14: lwc1        $f6, 0x8($s3)
    ctx->f6.u32l = MEM_W(ctx->r19, 0X8);
    // 0x80171C18: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80171C1C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80171C20: sub.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80171C24: lwc1        $f6, 0x4($s3)
    ctx->f6.u32l = MEM_W(ctx->r19, 0X4);
    // 0x80171C28: swc1        $f14, 0xEC($sp)
    MEM_W(0XEC, ctx->r29) = ctx->f14.u32l;
    // 0x80171C2C: swc1        $f4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f4.u32l;
    // 0x80171C30: mul.s       $f8, $f24, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f24.fl, ctx->f10.fl);
    // 0x80171C34: addiu       $a3, $sp, 0x15C
    ctx->r7 = ADD32(ctx->r29, 0X15C);
    // 0x80171C38: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80171C3C: lwc1        $f6, 0xD4($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XD4);
    // 0x80171C40: mfc1        $a2, $f10
    ctx->r6 = (int32_t)ctx->f10.u32l;
    // 0x80171C44: jal         0x80171E80
    // 0x80171C48: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    func_80171E80(rdram, ctx);
        goto after_6;
    // 0x80171C48: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_6:
    // 0x80171C4C: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171C50: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171C54: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171C58: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171C5C: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x80171C60: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x80171C64: lwc1        $f14, 0xEC($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0XEC);
    // 0x80171C68: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x80171C6C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
L_80171C70:
    // 0x80171C70: beq         $s1, $at, L_80171CE8
    if (ctx->r17 == ctx->r1) {
        // 0x80171C74: lwc1        $f8, 0xD0($sp)
        ctx->f8.u32l = MEM_W(ctx->r29, 0XD0);
            goto L_80171CE8;
    }
    // 0x80171C74: lwc1        $f8, 0xD0($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XD0);
    // 0x80171C78: c.le.s      $f26, $f8
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f26.fl <= ctx->f8.fl;
    // 0x80171C7C: nop

    // 0x80171C80: bc1fl       L_80171CEC
    if (!c1cs) {
        // 0x80171C84: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171CEC;
    }
    goto skip_28;
    // 0x80171C84: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_28:
    // 0x80171C88: c.le.s      $f8, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f8.fl <= ctx->f14.fl;
    // 0x80171C8C: nop

    // 0x80171C90: bc1fl       L_80171CEC
    if (!c1cs) {
        // 0x80171C94: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171CEC;
    }
    goto skip_29;
    // 0x80171C94: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    skip_29:
    // 0x80171C98: lwc1        $f10, 0x8($s3)
    ctx->f10.u32l = MEM_W(ctx->r19, 0X8);
    // 0x80171C9C: or          $a0, $fp, $zero
    ctx->r4 = ctx->r30 | 0;
    // 0x80171CA0: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80171CA4: sub.s       $f6, $f8, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80171CA8: lwc1        $f10, 0x4($s3)
    ctx->f10.u32l = MEM_W(ctx->r19, 0X4);
    // 0x80171CAC: swc1        $f8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f8.u32l;
    // 0x80171CB0: addiu       $a3, $sp, 0x15C
    ctx->r7 = ADD32(ctx->r29, 0X15C);
    // 0x80171CB4: mul.s       $f4, $f24, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f24.fl, ctx->f6.fl);
    // 0x80171CB8: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80171CBC: lwc1        $f10, 0xC8($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0XC8);
    // 0x80171CC0: mfc1        $a2, $f6
    ctx->r6 = (int32_t)ctx->f6.u32l;
    // 0x80171CC4: jal         0x80171E80
    // 0x80171CC8: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    func_80171E80(rdram, ctx);
        goto after_7;
    // 0x80171CC8: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    after_7:
    // 0x80171CCC: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171CD0: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171CD4: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171CD8: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171CDC: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x80171CE0: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x80171CE4: or          $s7, $v0, $zero
    ctx->r23 = ctx->r2 | 0;
L_80171CE8:
    // 0x80171CE8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
L_80171CEC:
    // 0x80171CEC: beq         $s1, $at, L_80171D04
    if (ctx->r17 == ctx->r1) {
        // 0x80171CF0: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80171D04;
    }
    // 0x80171CF0: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80171CF4: beql        $s7, $at, L_80171D08
    if (ctx->r23 == ctx->r1) {
        // 0x80171CF8: lw          $t9, 0x114($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X114);
            goto L_80171D08;
    }
    goto skip_30;
    // 0x80171CF8: lw          $t9, 0x114($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X114);
    skip_30:
    // 0x80171CFC: b           L_80171E10
    // 0x80171D00: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
        goto L_80171E10;
    // 0x80171D00: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80171D04:
    // 0x80171D04: lw          $t9, 0x114($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X114);
L_80171D08:
    // 0x80171D08: lw          $t5, 0x17C($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X17C);
    // 0x80171D0C: lwc1        $f10, 0x108($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X108);
    // 0x80171D10: beq         $t9, $zero, L_80171D30
    if (ctx->r25 == 0) {
        // 0x80171D14: lw          $t6, 0x17C($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X17C);
            goto L_80171D30;
    }
    // 0x80171D14: lw          $t6, 0x17C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X17C);
    // 0x80171D18: lwc1        $f4, 0x108($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X108);
    // 0x80171D1C: lwc1        $f6, 0x0($t5)
    ctx->f6.u32l = MEM_W(ctx->r13, 0X0);
    // 0x80171D20: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x80171D24: nop

    // 0x80171D28: bc1f        L_80171DF4
    if (!c1cs) {
        // 0x80171D2C: nop
    
            goto L_80171DF4;
    }
    // 0x80171D2C: nop

L_80171D30:
    // 0x80171D30: lw          $v0, 0x178($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X178);
    // 0x80171D34: lw          $v1, 0x174($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X174);
    // 0x80171D38: swc1        $f10, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f10.u32l;
    // 0x80171D3C: lw          $t7, 0x0($t1)
    ctx->r15 = MEM_W(ctx->r9, 0X0);
    // 0x80171D40: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80171D44: lwc1        $f8, 0xEC($t7)
    ctx->f8.u32l = MEM_W(ctx->r15, 0XEC);
    // 0x80171D48: swc1        $f8, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f8.u32l;
    // 0x80171D4C: lwc1        $f0, 0x150($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X150);
    // 0x80171D50: swc1        $f0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f0.u32l;
    // 0x80171D54: lw          $t8, 0x0($t1)
    ctx->r24 = MEM_W(ctx->r9, 0X0);
    // 0x80171D58: lwc1        $f4, 0xE8($t8)
    ctx->f4.u32l = MEM_W(ctx->r24, 0XE8);
    // 0x80171D5C: sub.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x80171D60: swc1        $f6, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f6.u32l;
    // 0x80171D64: lwc1        $f0, 0x158($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X158);
    // 0x80171D68: swc1        $f0, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->f0.u32l;
    // 0x80171D6C: lw          $t9, 0x0($t1)
    ctx->r25 = MEM_W(ctx->r9, 0X0);
    // 0x80171D70: lwc1        $f14, 0x0($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80171D74: lwc1        $f10, 0xF0($t9)
    ctx->f10.u32l = MEM_W(ctx->r25, 0XF0);
    // 0x80171D78: swc1        $f22, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f22.u32l;
    // 0x80171D7C: sub.s       $f8, $f10, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f10.fl - ctx->f0.fl;
    // 0x80171D80: swc1        $f8, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f8.u32l;
    // 0x80171D84: lwc1        $f2, 0x8($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80171D88: sw          $t5, 0x114($sp)
    MEM_W(0X114, ctx->r29) = ctx->r13;
    // 0x80171D8C: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80171D90: nop

    // 0x80171D94: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80171D98: jal         0x800A01E0
    // 0x80171D9C: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_8;
    // 0x80171D9C: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    after_8:
    // 0x80171DA0: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80171DA4: lui         $t1, 0x801A
    ctx->r9 = S32(0X801A << 16);
    // 0x80171DA8: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80171DAC: c.eq.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl == ctx->f10.fl;
    // 0x80171DB0: addiu       $t4, $t4, 0x2550
    ctx->r12 = ADD32(ctx->r12, 0X2550);
    // 0x80171DB4: addiu       $t1, $t1, -0x9FC
    ctx->r9 = ADD32(ctx->r9, -0X9FC);
    // 0x80171DB8: addiu       $t2, $zero, 0x3
    ctx->r10 = ADD32(0, 0X3);
    // 0x80171DBC: bc1f        L_80171DD0
    if (!c1cs) {
        // 0x80171DC0: addiu       $t3, $sp, 0x168
        ctx->r11 = ADD32(ctx->r29, 0X168);
            goto L_80171DD0;
    }
    // 0x80171DC0: addiu       $t3, $sp, 0x168
    ctx->r11 = ADD32(ctx->r29, 0X168);
    // 0x80171DC4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80171DC8: b           L_80171DD4
    // 0x80171DCC: lwc1        $f24, -0x50DC($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X50DC);
        goto L_80171DD4;
    // 0x80171DCC: lwc1        $f24, -0x50DC($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X50DC);
L_80171DD0:
    // 0x80171DD0: div.s       $f24, $f20, $f0
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f24.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
L_80171DD4:
    // 0x80171DD4: lw          $v0, 0x178($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X178);
    // 0x80171DD8: lwc1        $f8, 0x0($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80171DDC: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80171DE0: mul.s       $f4, $f8, $f24
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f24.fl);
    // 0x80171DE4: nop

    // 0x80171DE8: mul.s       $f10, $f6, $f24
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f24.fl);
    // 0x80171DEC: swc1        $f4, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f4.u32l;
    // 0x80171DF0: swc1        $f10, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f10.u32l;
L_80171DF4:
    // 0x80171DF4: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x80171DF8: lh          $t6, -0x680($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X680);
    // 0x80171DFC: bnel        $t6, $zero, L_80171E10
    if (ctx->r14 != 0) {
        // 0x80171E00: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80171E10;
    }
    goto skip_31;
    // 0x80171E00: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_31:
    // 0x80171E04: b           L_80171E38
    // 0x80171E08: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80171E38;
    // 0x80171E08: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80171E0C: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80171E10:
    // 0x80171E10: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80171E14: bne         $s2, $at, L_80171560
    if (ctx->r18 != ctx->r1) {
        // 0x80171E18: nop
    
            goto L_80171560;
    }
    // 0x80171E18: nop

    // 0x80171E1C: lw          $t7, 0x114($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X114);
    // 0x80171E20: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80171E24: beq         $t7, $zero, L_80171E34
    if (ctx->r15 == 0) {
        // 0x80171E28: nop
    
            goto L_80171E34;
    }
    // 0x80171E28: nop

    // 0x80171E2C: b           L_80171E34
    // 0x80171E30: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
        goto L_80171E34;
    // 0x80171E30: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80171E34:
    // 0x80171E34: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80171E38:
    // 0x80171E38: lw          $ra, 0x74($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X74);
L_80171E3C:
    // 0x80171E3C: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x80171E40: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x80171E44: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x80171E48: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x80171E4C: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x80171E50: ldc1        $f30, 0x48($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X48);
    // 0x80171E54: lw          $s0, 0x50($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X50);
    // 0x80171E58: lw          $s1, 0x54($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X54);
    // 0x80171E5C: lw          $s2, 0x58($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X58);
    // 0x80171E60: lw          $s3, 0x5C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X5C);
    // 0x80171E64: lw          $s4, 0x60($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X60);
    // 0x80171E68: lw          $s5, 0x64($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X64);
    // 0x80171E6C: lw          $s6, 0x68($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X68);
    // 0x80171E70: lw          $s7, 0x6C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X6C);
    // 0x80171E74: lw          $fp, 0x70($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X70);
    // 0x80171E78: jr          $ra
    // 0x80171E7C: addiu       $sp, $sp, 0x170
    ctx->r29 = ADD32(ctx->r29, 0X170);
    return;
    // 0x80171E7C: addiu       $sp, $sp, 0x170
    ctx->r29 = ADD32(ctx->r29, 0X170);
;}
RECOMP_FUNC void func_80171E80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80171E80: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80171E84: lw          $v1, -0x650($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X650);
    // 0x80171E88: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80171E8C: addiu       $a2, $zero, 0xC
    ctx->r6 = ADD32(0, 0XC);
    // 0x80171E90: multu       $v1, $a2
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r6)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80171E94: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x80171E98: lw          $t7, -0x64C($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X64C);
    // 0x80171E9C: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80171EA0: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x80171EA4: addiu       $a1, $a1, -0x640
    ctx->r5 = ADD32(ctx->r5, -0X640);
    // 0x80171EA8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80171EAC: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
    // 0x80171EB0: mflo        $t6
    ctx->r14 = lo;
    // 0x80171EB4: addu        $v0, $a1, $t6
    ctx->r2 = ADD32(ctx->r5, ctx->r14);
    // 0x80171EB8: nop

    // 0x80171EBC: multu       $t7, $a2
    result = U64(U32(ctx->r15)) * U64(U32(ctx->r6)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80171EC0: mflo        $t8
    ctx->r24 = lo;
    // 0x80171EC4: addu        $a0, $a1, $t8
    ctx->r4 = ADD32(ctx->r5, ctx->r24);
    // 0x80171EC8: beql        $v1, $at, L_80171F48
    if (ctx->r3 == ctx->r1) {
        // 0x80171ECC: lwc1        $f8, 0x0($a3)
        ctx->f8.u32l = MEM_W(ctx->r7, 0X0);
            goto L_80171F48;
    }
    goto skip_0;
    // 0x80171ECC: lwc1        $f8, 0x0($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X0);
    skip_0:
    // 0x80171ED0: lwc1        $f14, 0x0($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80171ED4: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80171ED8: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80171EDC: sub.s       $f2, $f4, $f14
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f14.fl;
    // 0x80171EE0: lwc1        $f4, 0x10($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80171EE4: c.eq.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl == ctx->f6.fl;
    // 0x80171EE8: nop

    // 0x80171EEC: bc1tl       L_80171F18
    if (c1cs) {
        // 0x80171EF0: lwc1        $f18, 0x4($v0)
        ctx->f18.u32l = MEM_W(ctx->r2, 0X4);
            goto L_80171F18;
    }
    goto skip_1;
    // 0x80171EF0: lwc1        $f18, 0x4($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X4);
    skip_1:
    // 0x80171EF4: lwc1        $f18, 0x4($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80171EF8: lwc1        $f8, 0x4($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80171EFC: sub.s       $f6, $f4, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f14.fl;
    // 0x80171F00: sub.s       $f10, $f8, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f18.fl;
    // 0x80171F04: div.s       $f16, $f10, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80171F08: mul.s       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f6.fl);
    // 0x80171F0C: b           L_8017201C
    // 0x80171F10: add.s       $f0, $f18, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f18.fl + ctx->f8.fl;
        goto L_8017201C;
    // 0x80171F10: add.s       $f0, $f18, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f18.fl + ctx->f8.fl;
    // 0x80171F14: lwc1        $f18, 0x4($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X4);
L_80171F18:
    // 0x80171F18: lwc1        $f2, 0x8($v0)
    ctx->f2.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80171F1C: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80171F20: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80171F24: sub.s       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f18.fl;
    // 0x80171F28: lwc1        $f10, 0x14($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X14);
    // 0x80171F2C: sub.s       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f2.fl;
    // 0x80171F30: sub.s       $f6, $f10, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f2.fl;
    // 0x80171F34: div.s       $f16, $f4, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = DIV_S(ctx->f4.fl, ctx->f8.fl);
    // 0x80171F38: mul.s       $f4, $f16, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f6.fl);
    // 0x80171F3C: b           L_8017201C
    // 0x80171F40: add.s       $f0, $f18, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f18.fl + ctx->f4.fl;
        goto L_8017201C;
    // 0x80171F40: add.s       $f0, $f18, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x80171F44: lwc1        $f8, 0x0($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X0);
L_80171F48:
    // 0x80171F48: lwc1        $f10, 0x8($a3)
    ctx->f10.u32l = MEM_W(ctx->r7, 0X8);
    // 0x80171F4C: lwc1        $f6, 0x10($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80171F50: c.le.s      $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f10.fl <= ctx->f8.fl;
    // 0x80171F54: nop

    // 0x80171F58: bc1fl       L_80171FC4
    if (!c1cs) {
        // 0x80171F5C: lwc1        $f2, 0x8($a0)
        ctx->f2.u32l = MEM_W(ctx->r4, 0X8);
            goto L_80171FC4;
    }
    goto skip_2;
    // 0x80171F5C: lwc1        $f2, 0x8($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X8);
    skip_2:
    // 0x80171F60: lwc1        $f2, 0x0($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80171F64: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80171F68: c.lt.s      $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f6.fl < ctx->f2.fl;
    // 0x80171F6C: nop

    // 0x80171F70: bc1f        L_80171F84
    if (!c1cs) {
        // 0x80171F74: nop
    
            goto L_80171F84;
    }
    // 0x80171F74: nop

    // 0x80171F78: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80171F7C: b           L_80171F88
    // 0x80171F80: lw          $v0, -0x658($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X658);
        goto L_80171F88;
    // 0x80171F80: lw          $v0, -0x658($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X658);
L_80171F84:
    // 0x80171F84: lw          $v0, -0x654($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X654);
L_80171F88:
    // 0x80171F88: multu       $v0, $a2
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r6)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80171F8C: lwc1        $f14, 0x4($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80171F90: mflo        $t9
    ctx->r25 = lo;
    // 0x80171F94: addu        $v1, $a1, $t9
    ctx->r3 = ADD32(ctx->r5, ctx->r25);
    // 0x80171F98: lwc1        $f4, 0x4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X4);
    // 0x80171F9C: lwc1        $f10, 0x0($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X0);
    // 0x80171FA0: sub.s       $f8, $f4, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f14.fl;
    // 0x80171FA4: lwc1        $f4, 0x10($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80171FA8: sub.s       $f6, $f10, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f2.fl;
    // 0x80171FAC: sub.s       $f10, $f4, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x80171FB0: div.s       $f16, $f8, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = DIV_S(ctx->f8.fl, ctx->f6.fl);
    // 0x80171FB4: mul.s       $f8, $f16, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f10.fl);
    // 0x80171FB8: b           L_8017201C
    // 0x80171FBC: add.s       $f0, $f14, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f14.fl + ctx->f8.fl;
        goto L_8017201C;
    // 0x80171FBC: add.s       $f0, $f14, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f14.fl + ctx->f8.fl;
    // 0x80171FC0: lwc1        $f2, 0x8($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X8);
L_80171FC4:
    // 0x80171FC4: lwc1        $f6, 0x14($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X14);
    // 0x80171FC8: c.lt.s      $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f6.fl < ctx->f2.fl;
    // 0x80171FCC: nop

    // 0x80171FD0: bc1f        L_80171FE0
    if (!c1cs) {
        // 0x80171FD4: lui         $v0, 0x801A
        ctx->r2 = S32(0X801A << 16);
            goto L_80171FE0;
    }
    // 0x80171FD4: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80171FD8: b           L_80171FE8
    // 0x80171FDC: lw          $v0, -0x658($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X658);
        goto L_80171FE8;
    // 0x80171FDC: lw          $v0, -0x658($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X658);
L_80171FE0:
    // 0x80171FE0: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80171FE4: lw          $v0, -0x654($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X654);
L_80171FE8:
    // 0x80171FE8: multu       $v0, $a2
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r6)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80171FEC: lwc1        $f14, 0x4($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80171FF0: mflo        $t0
    ctx->r8 = lo;
    // 0x80171FF4: addu        $v1, $a1, $t0
    ctx->r3 = ADD32(ctx->r5, ctx->r8);
    // 0x80171FF8: lwc1        $f4, 0x4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X4);
    // 0x80171FFC: lwc1        $f8, 0x8($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X8);
    // 0x80172000: sub.s       $f10, $f4, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f4.fl - ctx->f14.fl;
    // 0x80172004: lwc1        $f4, 0x14($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X14);
    // 0x80172008: sub.s       $f6, $f8, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x8017200C: sub.s       $f8, $f4, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x80172010: div.s       $f16, $f10, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f6.fl);
    // 0x80172014: mul.s       $f10, $f16, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x80172018: add.s       $f0, $f14, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = ctx->f14.fl + ctx->f10.fl;
L_8017201C:
    // 0x8017201C: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80172020: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80172024: bc1f        L_80172038
    if (!c1cs) {
        // 0x80172028: nop
    
            goto L_80172038;
    }
    // 0x80172028: nop

    // 0x8017202C: mov.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
    // 0x80172030: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
    // 0x80172034: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_80172038:
    // 0x80172038: lw          $v0, -0x9FC($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X9FC);
    // 0x8017203C: lwc1        $f6, 0x120($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X120);
    // 0x80172040: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x80172044: nop

    // 0x80172048: bc1t        L_80172068
    if (c1cs) {
        // 0x8017204C: nop
    
            goto L_80172068;
    }
    // 0x8017204C: nop

    // 0x80172050: lwc1        $f4, 0x12C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X12C);
    // 0x80172054: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80172058: c.lt.s      $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f4.fl < ctx->f12.fl;
    // 0x8017205C: nop

    // 0x80172060: bc1f        L_80172070
    if (!c1cs) {
        // 0x80172064: nop
    
            goto L_80172070;
    }
    // 0x80172064: nop

L_80172068:
    // 0x80172068: jr          $ra
    // 0x8017206C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x8017206C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80172070:
    // 0x80172070: jr          $ra
    // 0x80172074: nop

    return;
    // 0x80172074: nop

;}
RECOMP_FUNC void func_80172078(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172078: lui         $a2, 0x801A
    ctx->r6 = S32(0X801A << 16);
    // 0x8017207C: addiu       $a2, $a2, -0x9FC
    ctx->r6 = ADD32(ctx->r6, -0X9FC);
    // 0x80172080: sw          $a0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r4;
    // 0x80172084: sw          $zero, 0x22C($a0)
    MEM_W(0X22C, ctx->r4) = 0;
    // 0x80172088: lw          $t7, 0x0($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X0);
    // 0x8017208C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80172090: sw          $zero, 0x488($t7)
    MEM_W(0X488, ctx->r15) = 0;
    // 0x80172094: lw          $t8, 0x0($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X0);
    // 0x80172098: sw          $zero, 0x6F0($t8)
    MEM_W(0X6F0, ctx->r24) = 0;
    // 0x8017209C: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x801720A0: lw          $v1, 0x6F0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X6F0);
    // 0x801720A4: sw          $v1, 0x6EC($v0)
    MEM_W(0X6EC, ctx->r2) = ctx->r3;
    // 0x801720A8: lw          $t9, 0x0($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X0);
    // 0x801720AC: sw          $v1, 0x6E8($t9)
    MEM_W(0X6E8, ctx->r25) = ctx->r3;
    // 0x801720B0: lw          $t0, 0x0($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X0);
    // 0x801720B4: sw          $a1, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->r5;
    // 0x801720B8: lw          $t1, 0x0($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X0);
    // 0x801720BC: sh          $zero, 0x2F4($t1)
    MEM_H(0X2F4, ctx->r9) = 0;
    // 0x801720C0: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x801720C4: lh          $t2, 0x2F4($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X2F4);
    // 0x801720C8: sh          $t2, 0x290($v0)
    MEM_H(0X290, ctx->r2) = ctx->r10;
    // 0x801720CC: lw          $t3, 0x0($a2)
    ctx->r11 = MEM_W(ctx->r6, 0X0);
    // 0x801720D0: sh          $zero, 0x550($t3)
    MEM_H(0X550, ctx->r11) = 0;
    // 0x801720D4: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x801720D8: lh          $t4, 0x550($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X550);
    // 0x801720DC: sh          $t4, 0x4EC($v0)
    MEM_H(0X4EC, ctx->r2) = ctx->r12;
    // 0x801720E0: lw          $t5, 0x0($a2)
    ctx->r13 = MEM_W(ctx->r6, 0X0);
    // 0x801720E4: jr          $ra
    // 0x801720E8: swc1        $f4, 0x134($t5)
    MEM_W(0X134, ctx->r13) = ctx->f4.u32l;
    return;
    // 0x801720E8: swc1        $f4, 0x134($t5)
    MEM_W(0X134, ctx->r13) = ctx->f4.u32l;
;}
RECOMP_FUNC void func_801720EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801720EC: addiu       $sp, $sp, -0x10
    ctx->r29 = ADD32(ctx->r29, -0X10);
    // 0x801720F0: sdc1        $f20, 0x8($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X8, ctx->r29);
    // 0x801720F4: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x801720F8: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x801720FC: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80172100: mtc1        $a3, $f20
    ctx->f20.u32l = ctx->r7;
    // 0x80172104: addiu       $v0, $v0, -0x9FC
    ctx->r2 = ADD32(ctx->r2, -0X9FC);
    // 0x80172108: addiu       $a2, $zero, 0xC
    ctx->r6 = ADD32(0, 0XC);
    // 0x8017210C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80172110: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80172114:
    // 0x80172114: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80172118: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8017211C: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x80172120: addu        $t7, $t6, $v1
    ctx->r15 = ADD32(ctx->r14, ctx->r3);
    // 0x80172124: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80172128: bne         $v1, $a2, L_80172114
    if (ctx->r3 != ctx->r6) {
        // 0x8017212C: swc1        $f4, 0xD4($t7)
        MEM_W(0XD4, ctx->r15) = ctx->f4.u32l;
            goto L_80172114;
    }
    // 0x8017212C: swc1        $f4, 0xD4($t7)
    MEM_W(0XD4, ctx->r15) = ctx->f4.u32l;
    // 0x80172130: sub.s       $f6, $f12, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = ctx->f12.fl - ctx->f14.fl;
    // 0x80172134: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x80172138: mul.s       $f8, $f20, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f20.fl, ctx->f20.fl);
    // 0x8017213C: swc1        $f6, 0x19C($t8)
    MEM_W(0X19C, ctx->r24) = ctx->f6.u32l;
    // 0x80172140: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X0);
    // 0x80172144: swc1        $f12, 0x1A0($t9)
    MEM_W(0X1A0, ctx->r25) = ctx->f12.u32l;
    // 0x80172148: lw          $t0, 0x0($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X0);
    // 0x8017214C: swc1        $f14, 0x1A4($t0)
    MEM_W(0X1A4, ctx->r8) = ctx->f14.u32l;
    // 0x80172150: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X0);
    // 0x80172154: swc1        $f20, 0xE0($t1)
    MEM_W(0XE0, ctx->r9) = ctx->f20.u32l;
    // 0x80172158: lw          $t2, 0x0($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X0);
    // 0x8017215C: swc1        $f8, 0xE4($t2)
    MEM_W(0XE4, ctx->r10) = ctx->f8.u32l;
    // 0x80172160: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172164: lw          $t3, 0x0($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X0);
    // 0x80172168: add.s       $f16, $f10, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f14.fl;
    // 0x8017216C: swc1        $f16, 0x108($t3)
    MEM_W(0X108, ctx->r11) = ctx->f16.u32l;
    // 0x80172170: lwc1        $f18, 0x4($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172174: lw          $t4, 0x0($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X0);
    // 0x80172178: add.s       $f4, $f18, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f12.fl;
    // 0x8017217C: swc1        $f4, 0x114($t4)
    MEM_W(0X114, ctx->r12) = ctx->f4.u32l;
    // 0x80172180: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172184: lw          $t5, 0x0($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X0);
    // 0x80172188: sub.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f20.fl;
    // 0x8017218C: swc1        $f8, 0x104($t5)
    MEM_W(0X104, ctx->r13) = ctx->f8.u32l;
    // 0x80172190: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172194: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80172198: add.s       $f16, $f10, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f20.fl;
    // 0x8017219C: swc1        $f16, 0x110($t6)
    MEM_W(0X110, ctx->r14) = ctx->f16.u32l;
    // 0x801721A0: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x801721A4: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x801721A8: sub.s       $f4, $f18, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = ctx->f18.fl - ctx->f20.fl;
    // 0x801721AC: swc1        $f4, 0x10C($t7)
    MEM_W(0X10C, ctx->r15) = ctx->f4.u32l;
    // 0x801721B0: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    // 0x801721B4: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x801721B8: add.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f20.fl;
    // 0x801721BC: swc1        $f8, 0x118($t8)
    MEM_W(0X118, ctx->r24) = ctx->f8.u32l;
    // 0x801721C0: ldc1        $f20, 0x8($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X8);
    // 0x801721C4: jr          $ra
    // 0x801721C8: addiu       $sp, $sp, 0x10
    ctx->r29 = ADD32(ctx->r29, 0X10);
    return;
    // 0x801721C8: addiu       $sp, $sp, 0x10
    ctx->r29 = ADD32(ctx->r29, 0X10);
;}
RECOMP_FUNC void func_801721CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801721CC: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x801721D0: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801721D4: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x801721D8: addiu       $s1, $s1, -0x9FC
    ctx->r17 = ADD32(ctx->r17, -0X9FC);
    // 0x801721DC: lw          $t6, 0x0($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X0);
    // 0x801721E0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801721E4: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801721E8: sw          $a0, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->r4;
    // 0x801721EC: lw          $t7, 0x0($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X0);
    // 0x801721F0: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x801721F4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801721F8: sw          $a1, 0x8($t7)
    MEM_W(0X8, ctx->r15) = ctx->r5;
    // 0x801721FC: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x80172200: addiu       $a0, $sp, 0x40
    ctx->r4 = ADD32(ctx->r29, 0X40);
    // 0x80172204: addiu       $v0, $sp, 0x34
    ctx->r2 = ADD32(ctx->r29, 0X34);
    // 0x80172208: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x8017220C: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80172210: beq         $v0, $a0, L_80172240
    if (ctx->r2 == ctx->r4) {
        // 0x80172214: lh          $t8, 0x0($a2)
        ctx->r24 = MEM_H(ctx->r6, 0X0);
            goto L_80172240;
    }
    // 0x80172214: lh          $t8, 0x0($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X0);
L_80172218:
    // 0x80172218: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x8017221C: lwc1        $f16, 0xD4($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0XD4);
    // 0x80172220: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80172224: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80172228: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8017222C: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x80172230: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x80172234: swc1        $f18, -0x8($v0)
    MEM_W(-0X8, ctx->r2) = ctx->f18.u32l;
    // 0x80172238: bne         $v0, $a0, L_80172218
    if (ctx->r2 != ctx->r4) {
        // 0x8017223C: lh          $t8, 0x0($a2)
        ctx->r24 = MEM_H(ctx->r6, 0X0);
            goto L_80172218;
    }
    // 0x8017223C: lh          $t8, 0x0($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X0);
L_80172240:
    // 0x80172240: mtc1        $t8, $f18
    ctx->f18.u32l = ctx->r24;
    // 0x80172244: lwc1        $f16, 0xD4($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0XD4);
    // 0x80172248: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8017224C: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80172250: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x80172254: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x80172258: swc1        $f18, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = ctx->f18.u32l;
    // 0x8017225C: addiu       $a0, $s0, 0x1E4
    ctx->r4 = ADD32(ctx->r16, 0X1E4);
    // 0x80172260: jal         0x801739B8
    // 0x80172264: lh          $a1, 0x8($a3)
    ctx->r5 = MEM_H(ctx->r7, 0X8);
    func_801739B8(rdram, ctx);
        goto after_0;
    // 0x80172264: lh          $a1, 0x8($a3)
    ctx->r5 = MEM_H(ctx->r7, 0X8);
    after_0:
    // 0x80172268: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8017226C: addiu       $a0, $s0, 0x208
    ctx->r4 = ADD32(ctx->r16, 0X208);
    // 0x80172270: jal         0x80173BC0
    // 0x80172274: addiu       $a1, $s0, 0x1E4
    ctx->r5 = ADD32(ctx->r16, 0X1E4);
    func_80173BC0(rdram, ctx);
        goto after_1;
    // 0x80172274: addiu       $a1, $s0, 0x1E4
    ctx->r5 = ADD32(ctx->r16, 0X1E4);
    after_1:
    // 0x80172278: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x8017227C: addiu       $a1, $sp, 0x34
    ctx->r5 = ADD32(ctx->r29, 0X34);
    // 0x80172280: addiu       $a0, $s0, 0x208
    ctx->r4 = ADD32(ctx->r16, 0X208);
    // 0x80172284: jal         0x80173524
    // 0x80172288: addiu       $a2, $s0, 0xE8
    ctx->r6 = ADD32(ctx->r16, 0XE8);
    func_80173524(rdram, ctx);
        goto after_2;
    // 0x80172288: addiu       $a2, $s0, 0xE8
    ctx->r6 = ADD32(ctx->r16, 0XE8);
    after_2:
    // 0x8017228C: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x80172290: lwc1        $f16, 0xEC($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0XEC);
    // 0x80172294: lwc1        $f18, 0x1A4($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X1A4);
    // 0x80172298: add.s       $f6, $f16, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x8017229C: swc1        $f6, 0x120($s0)
    MEM_W(0X120, ctx->r16) = ctx->f6.u32l;
    // 0x801722A0: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x801722A4: lwc1        $f4, 0xEC($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0XEC);
    // 0x801722A8: lwc1        $f8, 0x1A0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X1A0);
    // 0x801722AC: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801722B0: swc1        $f10, 0x12C($s0)
    MEM_W(0X12C, ctx->r16) = ctx->f10.u32l;
    // 0x801722B4: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x801722B8: lwc1        $f16, 0xE8($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0XE8);
    // 0x801722BC: lwc1        $f18, 0xE0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0XE0);
    // 0x801722C0: sub.s       $f6, $f16, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x801722C4: swc1        $f6, 0x11C($s0)
    MEM_W(0X11C, ctx->r16) = ctx->f6.u32l;
    // 0x801722C8: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x801722CC: lwc1        $f4, 0xE8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0XE8);
    // 0x801722D0: lwc1        $f8, 0xE0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0XE0);
    // 0x801722D4: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801722D8: swc1        $f10, 0x128($s0)
    MEM_W(0X128, ctx->r16) = ctx->f10.u32l;
    // 0x801722DC: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x801722E0: lwc1        $f16, 0xF0($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0XF0);
    // 0x801722E4: lwc1        $f18, 0xE0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0XE0);
    // 0x801722E8: sub.s       $f6, $f16, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x801722EC: swc1        $f6, 0x124($s0)
    MEM_W(0X124, ctx->r16) = ctx->f6.u32l;
    // 0x801722F0: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x801722F4: lwc1        $f4, 0xF0($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0XF0);
    // 0x801722F8: lwc1        $f8, 0xE0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0XE0);
    // 0x801722FC: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80172300: swc1        $f10, 0x130($s0)
    MEM_W(0X130, ctx->r16) = ctx->f10.u32l;
    // 0x80172304: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80172308: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8017230C: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80172310: jr          $ra
    // 0x80172314: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x80172314: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_80172318(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172318: lwc1        $f0, 0x4($a2)
    ctx->f0.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8017231C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80172320: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172324: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x80172328: nop

    // 0x8017232C: bc1fl       L_80172340
    if (!c1cs) {
        // 0x80172330: mov.s       $f2, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
            goto L_80172340;
    }
    goto skip_0;
    // 0x80172330: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    skip_0:
    // 0x80172334: b           L_80172340
    // 0x80172338: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_80172340;
    // 0x80172338: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
    // 0x8017233C: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_80172340:
    // 0x80172340: lwc1        $f6, -0x50D8($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X50D8);
    // 0x80172344: c.lt.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl < ctx->f6.fl;
    // 0x80172348: nop

    // 0x8017234C: bc1fl       L_80172360
    if (!c1cs) {
        // 0x80172350: lwc1        $f8, 0x4($a1)
        ctx->f8.u32l = MEM_W(ctx->r5, 0X4);
            goto L_80172360;
    }
    goto skip_1;
    // 0x80172350: lwc1        $f8, 0x4($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X4);
    skip_1:
    // 0x80172354: jr          $ra
    // 0x80172358: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    return;
    // 0x80172358: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x8017235C: lwc1        $f8, 0x4($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X4);
L_80172360:
    // 0x80172360: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80172364: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80172368: sub.s       $f10, $f12, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f12.fl - ctx->f8.fl;
    // 0x8017236C: div.s       $f2, $f10, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80172370: c.lt.s      $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f2.fl < ctx->f16.fl;
    // 0x80172374: nop

    // 0x80172378: bc1t        L_80172398
    if (c1cs) {
        // 0x8017237C: nop
    
            goto L_80172398;
    }
    // 0x8017237C: nop

    // 0x80172380: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x80172384: nop

    // 0x80172388: c.lt.s      $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f18.fl < ctx->f2.fl;
    // 0x8017238C: nop

    // 0x80172390: bc1fl       L_801723A4
    if (!c1cs) {
        // 0x80172394: swc1        $f12, 0x4($a3)
        MEM_W(0X4, ctx->r7) = ctx->f12.u32l;
            goto L_801723A4;
    }
    goto skip_2;
    // 0x80172394: swc1        $f12, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f12.u32l;
    skip_2:
L_80172398:
    // 0x80172398: jr          $ra
    // 0x8017239C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    return;
    // 0x8017239C: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x801723A0: swc1        $f12, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f12.u32l;
L_801723A4:
    // 0x801723A4: lwc1        $f4, 0x0($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X0);
    // 0x801723A8: lwc1        $f8, 0x0($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801723AC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801723B0: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x801723B4: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x801723B8: swc1        $f10, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f10.u32l;
    // 0x801723BC: lwc1        $f16, 0x8($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X8);
    // 0x801723C0: lwc1        $f4, 0x8($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801723C4: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x801723C8: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x801723CC: swc1        $f6, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->f6.u32l;
    // 0x801723D0: jr          $ra
    // 0x801723D4: nop

    return;
    // 0x801723D4: nop

;}
RECOMP_FUNC void func_801723D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801723D8: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801723DC: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x801723E0: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x801723E4: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x801723E8: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    // 0x801723EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801723F0: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x801723F4: sll         $a1, $a2, 16
    ctx->r5 = S32(ctx->r6 << 16);
    // 0x801723F8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x801723FC: jal         0x801739B8
    // 0x80172400: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    func_801739B8(rdram, ctx);
        goto after_0;
    // 0x80172400: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    after_0:
    // 0x80172404: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    // 0x80172408: lw          $a1, 0x44($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X44);
    // 0x8017240C: jal         0x80173524
    // 0x80172410: lw          $a2, 0x40($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X40);
    func_80173524(rdram, ctx);
        goto after_1;
    // 0x80172410: lw          $a2, 0x40($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X40);
    after_1:
    // 0x80172414: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80172418: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8017241C: jr          $ra
    // 0x80172420: nop

    return;
    // 0x80172420: nop

;}
RECOMP_FUNC void func_80172430(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172430: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80172434: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80172438: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8017243C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80172440: or          $s2, $a1, $zero
    ctx->r18 = ctx->r5 | 0;
    // 0x80172444: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80172448: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8017244C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80172450: addiu       $v0, $v0, -0x750
    ctx->r2 = ADD32(ctx->r2, -0X750);
    // 0x80172454: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80172458:
    // 0x80172458: sh          $s1, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r17;
    // 0x8017245C: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80172460: slti        $at, $s1, 0x40
    ctx->r1 = SIGNED(ctx->r17) < 0X40 ? 1 : 0;
    // 0x80172464: bne         $at, $zero, L_80172458
    if (ctx->r1 != 0) {
        // 0x80172468: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
            goto L_80172458;
    }
    // 0x80172468: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8017246C: lui         $s0, 0x8000
    ctx->r16 = S32(0X8000 << 16);
    // 0x80172470: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172474: addiu       $s0, $s0, 0x1514
    ctx->r16 = ADD32(ctx->r16, 0X1514);
    // 0x80172478: sw          $zero, -0x1130($at)
    MEM_W(-0X1130, ctx->r1) = 0;
    // 0x8017247C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80172480: addiu       $a1, $zero, 0x1900
    ctx->r5 = ADD32(0, 0X1900);
    // 0x80172484: jalr        $s0
    // 0x80172488: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_0;
    // 0x80172488: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x8017248C: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80172490: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80172494: addiu       $v1, $v1, -0x1120
    ctx->r3 = ADD32(ctx->r3, -0X1120);
    // 0x80172498: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x8017249C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801724A0: addiu       $a1, $zero, 0x1260
    ctx->r5 = ADD32(0, 0X1260);
    // 0x801724A4: jalr        $s0
    // 0x801724A8: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_1;
    // 0x801724A8: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_1:
    // 0x801724AC: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801724B0: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x801724B4: addiu       $v1, $v1, -0x1124
    ctx->r3 = ADD32(ctx->r3, -0X1124);
    // 0x801724B8: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x801724BC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801724C0: addiu       $a1, $zero, 0xC0
    ctx->r5 = ADD32(0, 0XC0);
    // 0x801724C4: jalr        $s0
    // 0x801724C8: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_2;
    // 0x801724C8: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_2:
    // 0x801724CC: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x801724D0: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801724D4: addiu       $v1, $v1, -0x1128
    ctx->r3 = ADD32(ctx->r3, -0X1128);
    // 0x801724D8: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x801724DC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801724E0: sw          $a2, -0x610($at)
    MEM_W(-0X610, ctx->r1) = ctx->r6;
    // 0x801724E4: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x801724E8: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x801724EC: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x801724F0: lui         $a3, 0x801A
    ctx->r7 = S32(0X801A << 16);
    // 0x801724F4: addiu       $a3, $a3, -0x600
    ctx->r7 = ADD32(ctx->r7, -0X600);
    // 0x801724F8: addiu       $a1, $a1, -0x5FC
    ctx->r5 = ADD32(ctx->r5, -0X5FC);
    // 0x801724FC: addiu       $a0, $a0, -0x5F8
    ctx->r4 = ADD32(ctx->r4, -0X5F8);
    // 0x80172500: addiu       $v1, $v1, -0x5F4
    ctx->r3 = ADD32(ctx->r3, -0X5F4);
    // 0x80172504: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
    // 0x80172508: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
    // 0x8017250C: sw          $zero, 0x0($a1)
    MEM_W(0X0, ctx->r5) = 0;
    // 0x80172510: sw          $zero, 0x0($a3)
    MEM_W(0X0, ctx->r7) = 0;
    // 0x80172514: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172518: sw          $zero, -0x604($at)
    MEM_W(-0X604, ctx->r1) = 0;
    // 0x8017251C: lw          $v0, 0x4($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X4);
    // 0x80172520: beq         $v0, $zero, L_80172538
    if (ctx->r2 == 0) {
        // 0x80172524: lui         $a1, 0xFF
        ctx->r5 = S32(0XFF << 16);
            goto L_80172538;
    }
    // 0x80172524: lui         $a1, 0xFF
    ctx->r5 = S32(0XFF << 16);
    // 0x80172528: ori         $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 | 0XFFFF;
    // 0x8017252C: and         $t0, $v0, $a1
    ctx->r8 = ctx->r2 & ctx->r5;
    // 0x80172530: addu        $t1, $t0, $a2
    ctx->r9 = ADD32(ctx->r8, ctx->r6);
    // 0x80172534: sw          $t1, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r9;
L_80172538:
    // 0x80172538: lw          $v0, 0xC($s2)
    ctx->r2 = MEM_W(ctx->r18, 0XC);
    // 0x8017253C: lui         $a1, 0xFF
    ctx->r5 = S32(0XFF << 16);
    // 0x80172540: ori         $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 | 0XFFFF;
    // 0x80172544: beq         $v0, $zero, L_80172554
    if (ctx->r2 == 0) {
        // 0x80172548: and         $t2, $v0, $a1
        ctx->r10 = ctx->r2 & ctx->r5;
            goto L_80172554;
    }
    // 0x80172548: and         $t2, $v0, $a1
    ctx->r10 = ctx->r2 & ctx->r5;
    // 0x8017254C: addu        $t3, $t2, $a2
    ctx->r11 = ADD32(ctx->r10, ctx->r6);
    // 0x80172550: sw          $t3, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r11;
L_80172554:
    // 0x80172554: lw          $v0, 0x10($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X10);
    // 0x80172558: beq         $v0, $zero, L_80172568
    if (ctx->r2 == 0) {
        // 0x8017255C: and         $t4, $v0, $a1
        ctx->r12 = ctx->r2 & ctx->r5;
            goto L_80172568;
    }
    // 0x8017255C: and         $t4, $v0, $a1
    ctx->r12 = ctx->r2 & ctx->r5;
    // 0x80172560: addu        $t5, $t4, $a2
    ctx->r13 = ADD32(ctx->r12, ctx->r6);
    // 0x80172564: sw          $t5, 0x10($s2)
    MEM_W(0X10, ctx->r18) = ctx->r13;
L_80172568:
    // 0x80172568: lw          $v0, 0x14($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X14);
    // 0x8017256C: beq         $v0, $zero, L_8017257C
    if (ctx->r2 == 0) {
        // 0x80172570: and         $t6, $v0, $a1
        ctx->r14 = ctx->r2 & ctx->r5;
            goto L_8017257C;
    }
    // 0x80172570: and         $t6, $v0, $a1
    ctx->r14 = ctx->r2 & ctx->r5;
    // 0x80172574: addu        $t7, $t6, $a2
    ctx->r15 = ADD32(ctx->r14, ctx->r6);
    // 0x80172578: sw          $t7, 0x14($s2)
    MEM_W(0X14, ctx->r18) = ctx->r15;
L_8017257C:
    // 0x8017257C: lw          $v0, 0x18($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X18);
    // 0x80172580: beq         $v0, $zero, L_80172590
    if (ctx->r2 == 0) {
        // 0x80172584: and         $t8, $v0, $a1
        ctx->r24 = ctx->r2 & ctx->r5;
            goto L_80172590;
    }
    // 0x80172584: and         $t8, $v0, $a1
    ctx->r24 = ctx->r2 & ctx->r5;
    // 0x80172588: addu        $t9, $t8, $a2
    ctx->r25 = ADD32(ctx->r24, ctx->r6);
    // 0x8017258C: sw          $t9, 0x18($s2)
    MEM_W(0X18, ctx->r18) = ctx->r25;
L_80172590:
    // 0x80172590: lw          $v0, 0x1C($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X1C);
    // 0x80172594: beq         $v0, $zero, L_801725A4
    if (ctx->r2 == 0) {
        // 0x80172598: and         $t0, $v0, $a1
        ctx->r8 = ctx->r2 & ctx->r5;
            goto L_801725A4;
    }
    // 0x80172598: and         $t0, $v0, $a1
    ctx->r8 = ctx->r2 & ctx->r5;
    // 0x8017259C: addu        $t1, $t0, $a2
    ctx->r9 = ADD32(ctx->r8, ctx->r6);
    // 0x801725A0: sw          $t1, 0x1C($s2)
    MEM_W(0X1C, ctx->r18) = ctx->r9;
L_801725A4:
    // 0x801725A4: lw          $v0, 0x24($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X24);
    // 0x801725A8: beql        $v0, $zero, L_80172600
    if (ctx->r2 == 0) {
        // 0x801725AC: lw          $v0, 0x28($s2)
        ctx->r2 = MEM_W(ctx->r18, 0X28);
            goto L_80172600;
    }
    goto skip_0;
    // 0x801725AC: lw          $v0, 0x28($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X28);
    skip_0:
    // 0x801725B0: lw          $a0, 0x20($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X20);
    // 0x801725B4: and         $t2, $v0, $a1
    ctx->r10 = ctx->r2 & ctx->r5;
    // 0x801725B8: addu        $t3, $t2, $a2
    ctx->r11 = ADD32(ctx->r10, ctx->r6);
    // 0x801725BC: sw          $t3, 0x24($s2)
    MEM_W(0X24, ctx->r18) = ctx->r11;
    // 0x801725C0: blez        $a0, L_801725FC
    if (SIGNED(ctx->r4) <= 0) {
        // 0x801725C4: or          $s1, $zero, $zero
        ctx->r17 = 0 | 0;
            goto L_801725FC;
    }
    // 0x801725C4: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801725C8: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_801725CC:
    // 0x801725CC: lw          $t4, 0x24($s2)
    ctx->r12 = MEM_W(ctx->r18, 0X24);
    // 0x801725D0: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x801725D4: addu        $v0, $t4, $s0
    ctx->r2 = ADD32(ctx->r12, ctx->r16);
    // 0x801725D8: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x801725DC: beq         $v1, $zero, L_801725F0
    if (ctx->r3 == 0) {
        // 0x801725E0: and         $t5, $v1, $a1
        ctx->r13 = ctx->r3 & ctx->r5;
            goto L_801725F0;
    }
    // 0x801725E0: and         $t5, $v1, $a1
    ctx->r13 = ctx->r3 & ctx->r5;
    // 0x801725E4: addu        $t6, $t5, $a2
    ctx->r14 = ADD32(ctx->r13, ctx->r6);
    // 0x801725E8: sw          $t6, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r14;
    // 0x801725EC: lw          $a0, 0x20($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X20);
L_801725F0:
    // 0x801725F0: slt         $at, $s1, $a0
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x801725F4: bne         $at, $zero, L_801725CC
    if (ctx->r1 != 0) {
        // 0x801725F8: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_801725CC;
    }
    // 0x801725F8: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_801725FC:
    // 0x801725FC: lw          $v0, 0x28($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X28);
L_80172600:
    // 0x80172600: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80172604: beq         $v0, $zero, L_80172614
    if (ctx->r2 == 0) {
        // 0x80172608: and         $t7, $v0, $a1
        ctx->r15 = ctx->r2 & ctx->r5;
            goto L_80172614;
    }
    // 0x80172608: and         $t7, $v0, $a1
    ctx->r15 = ctx->r2 & ctx->r5;
    // 0x8017260C: addu        $t8, $t7, $a2
    ctx->r24 = ADD32(ctx->r15, ctx->r6);
    // 0x80172610: sw          $t8, 0x28($s2)
    MEM_W(0X28, ctx->r18) = ctx->r24;
L_80172614:
    // 0x80172614: lw          $v0, 0x2C($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X2C);
    // 0x80172618: beq         $v0, $zero, L_80172628
    if (ctx->r2 == 0) {
        // 0x8017261C: and         $t9, $v0, $a1
        ctx->r25 = ctx->r2 & ctx->r5;
            goto L_80172628;
    }
    // 0x8017261C: and         $t9, $v0, $a1
    ctx->r25 = ctx->r2 & ctx->r5;
    // 0x80172620: addu        $t0, $t9, $a2
    ctx->r8 = ADD32(ctx->r25, ctx->r6);
    // 0x80172624: sw          $t0, 0x2C($s2)
    MEM_W(0X2C, ctx->r18) = ctx->r8;
L_80172628:
    // 0x80172628: lw          $v0, 0x30($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X30);
    // 0x8017262C: beq         $v0, $zero, L_8017263C
    if (ctx->r2 == 0) {
        // 0x80172630: and         $t1, $v0, $a1
        ctx->r9 = ctx->r2 & ctx->r5;
            goto L_8017263C;
    }
    // 0x80172630: and         $t1, $v0, $a1
    ctx->r9 = ctx->r2 & ctx->r5;
    // 0x80172634: addu        $t2, $t1, $a2
    ctx->r10 = ADD32(ctx->r9, ctx->r6);
    // 0x80172638: sw          $t2, 0x30($s2)
    MEM_W(0X30, ctx->r18) = ctx->r10;
L_8017263C:
    // 0x8017263C: lw          $v0, 0x34($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X34);
    // 0x80172640: beq         $v0, $zero, L_80172650
    if (ctx->r2 == 0) {
        // 0x80172644: and         $t3, $v0, $a1
        ctx->r11 = ctx->r2 & ctx->r5;
            goto L_80172650;
    }
    // 0x80172644: and         $t3, $v0, $a1
    ctx->r11 = ctx->r2 & ctx->r5;
    // 0x80172648: addu        $t4, $t3, $a2
    ctx->r12 = ADD32(ctx->r11, ctx->r6);
    // 0x8017264C: sw          $t4, 0x34($s2)
    MEM_W(0X34, ctx->r18) = ctx->r12;
L_80172650:
    // 0x80172650: lw          $v0, 0x38($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X38);
    // 0x80172654: beql        $v0, $zero, L_801726AC
    if (ctx->r2 == 0) {
        // 0x80172658: lw          $a0, 0x0($s2)
        ctx->r4 = MEM_W(ctx->r18, 0X0);
            goto L_801726AC;
    }
    goto skip_1;
    // 0x80172658: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    skip_1:
    // 0x8017265C: lw          $a0, 0x20($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X20);
    // 0x80172660: and         $t5, $v0, $a1
    ctx->r13 = ctx->r2 & ctx->r5;
    // 0x80172664: addu        $t6, $t5, $a2
    ctx->r14 = ADD32(ctx->r13, ctx->r6);
    // 0x80172668: blez        $a0, L_801726A8
    if (SIGNED(ctx->r4) <= 0) {
        // 0x8017266C: sw          $t6, 0x38($s2)
        MEM_W(0X38, ctx->r18) = ctx->r14;
            goto L_801726A8;
    }
    // 0x8017266C: sw          $t6, 0x38($s2)
    MEM_W(0X38, ctx->r18) = ctx->r14;
    // 0x80172670: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_80172674:
    // 0x80172674: lw          $t7, 0x38($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X38);
    // 0x80172678: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x8017267C: addu        $v0, $t7, $s0
    ctx->r2 = ADD32(ctx->r15, ctx->r16);
    // 0x80172680: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x80172684: beq         $v1, $zero, L_80172698
    if (ctx->r3 == 0) {
        // 0x80172688: and         $t8, $v1, $a1
        ctx->r24 = ctx->r3 & ctx->r5;
            goto L_80172698;
    }
    // 0x80172688: and         $t8, $v1, $a1
    ctx->r24 = ctx->r3 & ctx->r5;
    // 0x8017268C: addu        $t9, $t8, $a2
    ctx->r25 = ADD32(ctx->r24, ctx->r6);
    // 0x80172690: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
    // 0x80172694: lw          $a0, 0x20($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X20);
L_80172698:
    // 0x80172698: slt         $at, $s1, $a0
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x8017269C: bne         $at, $zero, L_80172674
    if (ctx->r1 != 0) {
        // 0x801726A0: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80172674;
    }
    // 0x801726A0: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x801726A4: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_801726A8:
    // 0x801726A8: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
L_801726AC:
    // 0x801726AC: jal         0x80172740
    // 0x801726B0: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    func_80172740(rdram, ctx);
        goto after_3;
    // 0x801726B0: lw          $a1, 0x4($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X4);
    after_3:
    // 0x801726B4: lw          $a0, 0x10($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X10);
    // 0x801726B8: jal         0x80172994
    // 0x801726BC: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    func_80172994(rdram, ctx);
        goto after_4;
    // 0x801726BC: lw          $a1, 0x28($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X28);
    after_4:
    // 0x801726C0: lw          $a0, 0x14($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X14);
    // 0x801726C4: jal         0x80172994
    // 0x801726C8: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    func_80172994(rdram, ctx);
        goto after_5;
    // 0x801726C8: lw          $a1, 0x2C($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X2C);
    after_5:
    // 0x801726CC: lw          $a0, 0x18($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X18);
    // 0x801726D0: jal         0x80172994
    // 0x801726D4: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    func_80172994(rdram, ctx);
        goto after_6;
    // 0x801726D4: lw          $a1, 0x30($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X30);
    after_6:
    // 0x801726D8: lw          $a0, 0x1C($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X1C);
    // 0x801726DC: jal         0x80172994
    // 0x801726E0: lw          $a1, 0x34($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X34);
    func_80172994(rdram, ctx);
        goto after_7;
    // 0x801726E0: lw          $a1, 0x34($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X34);
    after_7:
    // 0x801726E4: lw          $t0, 0x20($s2)
    ctx->r8 = MEM_W(ctx->r18, 0X20);
    // 0x801726E8: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x801726EC: blezl       $t0, L_8017272C
    if (SIGNED(ctx->r8) <= 0) {
        // 0x801726F0: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_8017272C;
    }
    goto skip_2;
    // 0x801726F0: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_2:
    // 0x801726F4: lw          $t1, 0x24($s2)
    ctx->r9 = MEM_W(ctx->r18, 0X24);
L_801726F8:
    // 0x801726F8: lw          $t3, 0x38($s2)
    ctx->r11 = MEM_W(ctx->r18, 0X38);
    // 0x801726FC: addu        $t2, $t1, $s0
    ctx->r10 = ADD32(ctx->r9, ctx->r16);
    // 0x80172700: addu        $t4, $t3, $s0
    ctx->r12 = ADD32(ctx->r11, ctx->r16);
    // 0x80172704: lw          $a1, 0x0($t4)
    ctx->r5 = MEM_W(ctx->r12, 0X0);
    // 0x80172708: jal         0x80172994
    // 0x8017270C: lw          $a0, 0x0($t2)
    ctx->r4 = MEM_W(ctx->r10, 0X0);
    func_80172994(rdram, ctx);
        goto after_8;
    // 0x8017270C: lw          $a0, 0x0($t2)
    ctx->r4 = MEM_W(ctx->r10, 0X0);
    after_8:
    // 0x80172710: lw          $t5, 0x20($s2)
    ctx->r13 = MEM_W(ctx->r18, 0X20);
    // 0x80172714: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80172718: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x8017271C: slt         $at, $s1, $t5
    ctx->r1 = SIGNED(ctx->r17) < SIGNED(ctx->r13) ? 1 : 0;
    // 0x80172720: bnel        $at, $zero, L_801726F8
    if (ctx->r1 != 0) {
        // 0x80172724: lw          $t1, 0x24($s2)
        ctx->r9 = MEM_W(ctx->r18, 0X24);
            goto L_801726F8;
    }
    goto skip_3;
    // 0x80172724: lw          $t1, 0x24($s2)
    ctx->r9 = MEM_W(ctx->r18, 0X24);
    skip_3:
    // 0x80172728: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_8017272C:
    // 0x8017272C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80172730: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80172734: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80172738: jr          $ra
    // 0x8017273C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x8017273C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80172740(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172740: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80172744: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80172748: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x8017274C: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80172750: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80172754: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80172758: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8017275C: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80172760: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80172764: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80172768: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8017276C: beq         $a1, $zero, L_801728CC
    if (ctx->r5 == 0) {
        // 0x80172770: sw          $a0, 0x50($sp)
        MEM_W(0X50, ctx->r29) = ctx->r4;
            goto L_801728CC;
    }
    // 0x80172770: sw          $a0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r4;
    // 0x80172774: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172778: sw          $a0, -0x5F4($at)
    MEM_W(-0X5F4, ctx->r1) = ctx->r4;
    // 0x8017277C: sw          $zero, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = 0;
    // 0x80172780: blez        $a0, L_801728CC
    if (SIGNED(ctx->r4) <= 0) {
        // 0x80172784: sw          $a1, 0x44($sp)
        MEM_W(0X44, ctx->r29) = ctx->r5;
            goto L_801728CC;
    }
    // 0x80172784: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    // 0x80172788: lui         $fp, 0x801A
    ctx->r30 = S32(0X801A << 16);
    // 0x8017278C: lui         $s7, 0x801A
    ctx->r23 = S32(0X801A << 16);
    // 0x80172790: lui         $s6, 0xFF
    ctx->r22 = S32(0XFF << 16);
    // 0x80172794: lui         $s3, 0x801A
    ctx->r19 = S32(0X801A << 16);
    // 0x80172798: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x8017279C: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x801727A0: addiu       $s1, $s1, -0x604
    ctx->r17 = ADD32(ctx->r17, -0X604);
    // 0x801727A4: addiu       $s2, $s2, -0x5FC
    ctx->r18 = ADD32(ctx->r18, -0X5FC);
    // 0x801727A8: addiu       $s3, $s3, -0x5F8
    ctx->r19 = ADD32(ctx->r19, -0X5F8);
    // 0x801727AC: ori         $s6, $s6, 0xFFFF
    ctx->r22 = ctx->r22 | 0XFFFF;
    // 0x801727B0: addiu       $s7, $s7, -0x610
    ctx->r23 = ADD32(ctx->r23, -0X610);
    // 0x801727B4: addiu       $fp, $fp, -0x600
    ctx->r30 = ADD32(ctx->r30, -0X600);
    // 0x801727B8: addiu       $s4, $zero, 0x3
    ctx->r20 = ADD32(0, 0X3);
L_801727BC:
    // 0x801727BC: or          $s5, $zero, $zero
    ctx->r21 = 0 | 0;
    // 0x801727C0: lw          $s0, 0x44($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X44);
L_801727C4:
    // 0x801727C4: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    // 0x801727C8: beql        $a0, $zero, L_80172878
    if (ctx->r4 == 0) {
        // 0x801727CC: lw          $v0, 0xC($s0)
        ctx->r2 = MEM_W(ctx->r16, 0XC);
            goto L_80172878;
    }
    goto skip_0;
    // 0x801727CC: lw          $v0, 0xC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC);
    skip_0:
    // 0x801727D0: lw          $t8, 0x0($s7)
    ctx->r24 = MEM_W(ctx->r23, 0X0);
    // 0x801727D4: and         $t7, $a0, $s6
    ctx->r15 = ctx->r4 & ctx->r22;
    // 0x801727D8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x801727DC: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x801727E0: sw          $t9, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r25;
    // 0x801727E4: lw          $t0, 0x18($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X18);
    // 0x801727E8: lw          $t2, 0x0($s7)
    ctx->r10 = MEM_W(ctx->r23, 0X0);
    // 0x801727EC: and         $t1, $t0, $s6
    ctx->r9 = ctx->r8 & ctx->r22;
    // 0x801727F0: addu        $t3, $t1, $t2
    ctx->r11 = ADD32(ctx->r9, ctx->r10);
    // 0x801727F4: sw          $t3, 0x18($t9)
    MEM_W(0X18, ctx->r25) = ctx->r11;
    // 0x801727F8: lw          $t5, 0x0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X0);
    // 0x801727FC: lw          $t4, 0x0($fp)
    ctx->r12 = MEM_W(ctx->r30, 0X0);
    // 0x80172800: lh          $t6, 0x1C($t5)
    ctx->r14 = MEM_H(ctx->r13, 0X1C);
    // 0x80172804: addu        $t7, $t4, $t6
    ctx->r15 = ADD32(ctx->r12, ctx->r14);
    // 0x80172808: sw          $t7, 0x0($fp)
    MEM_W(0X0, ctx->r30) = ctx->r15;
    // 0x8017280C: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    // 0x80172810: lh          $t8, 0x1C($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X1C);
    // 0x80172814: lw          $v0, 0x18($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X18);
    // 0x80172818: blezl       $t8, L_80172878
    if (SIGNED(ctx->r24) <= 0) {
        // 0x8017281C: lw          $v0, 0xC($s0)
        ctx->r2 = MEM_W(ctx->r16, 0XC);
            goto L_80172878;
    }
    goto skip_1;
    // 0x8017281C: lw          $v0, 0xC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC);
    skip_1:
    // 0x80172820: lw          $t9, 0x0($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X0);
L_80172824:
    // 0x80172824: lh          $t0, 0x28($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X28);
    // 0x80172828: addu        $t1, $t9, $t0
    ctx->r9 = ADD32(ctx->r25, ctx->r8);
    // 0x8017282C: sw          $t1, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r9;
    // 0x80172830: lh          $t2, 0x28($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X28);
    // 0x80172834: bnel        $s4, $t2, L_80172850
    if (ctx->r20 != ctx->r10) {
        // 0x80172838: lw          $t4, 0x0($s3)
        ctx->r12 = MEM_W(ctx->r19, 0X0);
            goto L_80172850;
    }
    goto skip_2;
    // 0x80172838: lw          $t4, 0x0($s3)
    ctx->r12 = MEM_W(ctx->r19, 0X0);
    skip_2:
    // 0x8017283C: lw          $t3, 0x0($s2)
    ctx->r11 = MEM_W(ctx->r18, 0X0);
    // 0x80172840: addiu       $t5, $t3, 0x1
    ctx->r13 = ADD32(ctx->r11, 0X1);
    // 0x80172844: b           L_80172858
    // 0x80172848: sw          $t5, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r13;
        goto L_80172858;
    // 0x80172848: sw          $t5, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r13;
    // 0x8017284C: lw          $t4, 0x0($s3)
    ctx->r12 = MEM_W(ctx->r19, 0X0);
L_80172850:
    // 0x80172850: addiu       $t6, $t4, 0x1
    ctx->r14 = ADD32(ctx->r12, 0X1);
    // 0x80172854: sw          $t6, 0x0($s3)
    MEM_W(0X0, ctx->r19) = ctx->r14;
L_80172858:
    // 0x80172858: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x8017285C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80172860: addiu       $v0, $v0, 0x40
    ctx->r2 = ADD32(ctx->r2, 0X40);
    // 0x80172864: lh          $t8, 0x1C($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X1C);
    // 0x80172868: slt         $at, $v1, $t8
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r24) ? 1 : 0;
    // 0x8017286C: bnel        $at, $zero, L_80172824
    if (ctx->r1 != 0) {
        // 0x80172870: lw          $t9, 0x0($s1)
        ctx->r25 = MEM_W(ctx->r17, 0X0);
            goto L_80172824;
    }
    goto skip_3;
    // 0x80172870: lw          $t9, 0x0($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X0);
    skip_3:
    // 0x80172874: lw          $v0, 0xC($s0)
    ctx->r2 = MEM_W(ctx->r16, 0XC);
L_80172878:
    // 0x80172878: beq         $v0, $zero, L_8017289C
    if (ctx->r2 == 0) {
        // 0x8017287C: and         $t9, $v0, $s6
        ctx->r25 = ctx->r2 & ctx->r22;
            goto L_8017289C;
    }
    // 0x8017287C: and         $t9, $v0, $s6
    ctx->r25 = ctx->r2 & ctx->r22;
    // 0x80172880: lw          $t0, 0x0($s7)
    ctx->r8 = MEM_W(ctx->r23, 0X0);
    // 0x80172884: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172888: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8017288C: addu        $t1, $t9, $t0
    ctx->r9 = ADD32(ctx->r25, ctx->r8);
    // 0x80172890: sw          $t1, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r9;
    // 0x80172894: jal         0x801728FC
    // 0x80172898: sw          $t1, -0x608($at)
    MEM_W(-0X608, ctx->r1) = ctx->r9;
    func_801728FC(rdram, ctx);
        goto after_0;
    // 0x80172898: sw          $t1, -0x608($at)
    MEM_W(-0X608, ctx->r1) = ctx->r9;
    after_0:
L_8017289C:
    // 0x8017289C: addiu       $s5, $s5, 0x4
    ctx->r21 = ADD32(ctx->r21, 0X4);
    // 0x801728A0: addiu       $at, $zero, 0xC
    ctx->r1 = ADD32(0, 0XC);
    // 0x801728A4: bne         $s5, $at, L_801727C4
    if (ctx->r21 != ctx->r1) {
        // 0x801728A8: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_801727C4;
    }
    // 0x801728A8: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x801728AC: lw          $t3, 0x4C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X4C);
    // 0x801728B0: lw          $t4, 0x44($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X44);
    // 0x801728B4: lw          $t7, 0x50($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X50);
    // 0x801728B8: addiu       $t5, $t3, 0x1
    ctx->r13 = ADD32(ctx->r11, 0X1);
    // 0x801728BC: addiu       $t6, $t4, 0x30
    ctx->r14 = ADD32(ctx->r12, 0X30);
    // 0x801728C0: sw          $t6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r14;
    // 0x801728C4: bne         $t5, $t7, L_801727BC
    if (ctx->r13 != ctx->r15) {
        // 0x801728C8: sw          $t5, 0x4C($sp)
        MEM_W(0X4C, ctx->r29) = ctx->r13;
            goto L_801727BC;
    }
    // 0x801728C8: sw          $t5, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r13;
L_801728CC:
    // 0x801728CC: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x801728D0: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801728D4: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x801728D8: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x801728DC: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x801728E0: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x801728E4: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x801728E8: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x801728EC: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x801728F0: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x801728F4: jr          $ra
    // 0x801728F8: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x801728F8: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_801728FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_801728FC is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_801728FC++;
    if (lod_text_guard_depth_801728FC > 128) {
        if (!lod_text_guard_logged_801728FC) {
            lod_text_guard_logged_801728FC = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_801728FC recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_801728FC--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x801728FC: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80172900: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80172904: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x80172908: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8017290C: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x80172910: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80172914: addiu       $s2, $zero, -0x1
    ctx->r18 = ADD32(0, -0X1);
    // 0x80172918: addiu       $s1, $s1, -0x608
    ctx->r17 = ADD32(ctx->r17, -0X608);
    // 0x8017291C: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80172920: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80172924: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
L_80172928:
    // 0x80172928: lw          $t7, 0x0($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X0);
    // 0x8017292C: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x80172930: addu        $s0, $t6, $t7
    ctx->r16 = ADD32(ctx->r14, ctx->r15);
    // 0x80172934: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x80172938: bne         $s2, $t8, L_8017296C
    if (ctx->r18 != ctx->r24) {
        // 0x8017293C: nop
    
            goto L_8017296C;
    }
    // 0x8017293C: nop

    // 0x80172940: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x80172944: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x80172948: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x8017294C: beql        $v0, $zero, L_80172980
    if (ctx->r2 == 0) {
        // 0x80172950: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_80172980;
    }
    goto skip_0;
    // 0x80172950: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_0:
    // 0x80172954: lw          $t0, -0x610($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X610);
    // 0x80172958: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x8017295C: and         $t9, $v0, $at
    ctx->r25 = ctx->r2 & ctx->r1;
    // 0x80172960: addu        $t1, $t9, $t0
    ctx->r9 = ADD32(ctx->r25, ctx->r8);
    // 0x80172964: b           L_8017297C
    // 0x80172968: sw          $t1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r9;
        goto L_8017297C;
    // 0x80172968: sw          $t1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r9;
L_8017296C:
    // 0x8017296C: jal         0x801728FC
    // 0x80172970: lh          $a0, 0x4($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X4);
    func_801728FC(rdram, ctx);
        goto after_0;
    // 0x80172970: lh          $a0, 0x4($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X4);
    after_0:
    // 0x80172974: b           L_80172928
    // 0x80172978: lh          $a0, 0x6($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X6);
        goto L_80172928;
    // 0x80172978: lh          $a0, 0x6($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X6);
L_8017297C:
    // 0x8017297C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_80172980:
    // 0x80172980: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80172984: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80172988: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8017298C: jr          $ra
    // 0x80172990: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_801728FC--;
    // --- END PATCH ---
#endif
    return;
    // 0x80172990: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80172994(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172994: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80172998: beq         $a0, $zero, L_801729D8
    if (ctx->r4 == 0) {
        // 0x8017299C: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_801729D8;
    }
    // 0x8017299C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801729A0: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    // 0x801729A4: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x801729A8: lui         $t7, 0x801A
    ctx->r15 = S32(0X801A << 16);
    // 0x801729AC: beq         $v0, $zero, L_801729C8
    if (ctx->r2 == 0) {
        // 0x801729B0: nop
    
            goto L_801729C8;
    }
    // 0x801729B0: nop

    // 0x801729B4: lw          $t7, -0x610($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X610);
    // 0x801729B8: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801729BC: and         $t6, $v0, $at
    ctx->r14 = ctx->r2 & ctx->r1;
    // 0x801729C0: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x801729C4: sw          $t8, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r24;
L_801729C8:
    // 0x801729C8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801729CC: sw          $a1, -0x60C($at)
    MEM_W(-0X60C, ctx->r1) = ctx->r5;
    // 0x801729D0: jal         0x801729E8
    // 0x801729D4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_801729E8(rdram, ctx);
        goto after_0;
    // 0x801729D4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
L_801729D8:
    // 0x801729D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801729DC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801729E0: jr          $ra
    // 0x801729E4: nop

    return;
    // 0x801729E4: nop

;}
RECOMP_FUNC void func_801729E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard (LOD_FIX_TEXT_MEASURE_GUARD, round 17
    // Task B). func_801729E8 is self-recursive (found by the systematic sweep, see
    // docs/issue27-31-ni0e-findings.md round 17); confirmed single natural
    // return point by full-body scan. Same depth-128-cap pattern as
    // func_80168AA0/func_8016B878 (rounds 14/15): a strict no-op skip of the
    // entire function body whenever depth would exceed 128, matching
    // whatever a naturally-terminating (non-corrupted) recursion already
    // does at its own base case -- nothing runs, so no ctx register or
    // shared counter/table the caller depends on is partially written. ---
    lod_text_guard_depth_801729E8++;
    if (lod_text_guard_depth_801729E8 > 128) {
        if (!lod_text_guard_logged_801729E8) {
            lod_text_guard_logged_801729E8 = 1;
            fprintf(stderr,
                    "[TEXT_GUARD] func_801729E8 recursion depth exceeded 128, "
                    "aborting recursion (stale text-struct pointer)\n");
        }
        lod_text_guard_depth_801729E8--;
        return;
    }
    // --- END PATCH ---
#endif
    // 0x801729E8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801729EC: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x801729F0: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x801729F4: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x801729F8: lui         $s1, 0x801A
    ctx->r17 = S32(0X801A << 16);
    // 0x801729FC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80172A00: addiu       $s2, $zero, -0x1
    ctx->r18 = ADD32(0, -0X1);
    // 0x80172A04: addiu       $s1, $s1, -0x60C
    ctx->r17 = ADD32(ctx->r17, -0X60C);
    // 0x80172A08: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80172A0C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80172A10: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
L_80172A14:
    // 0x80172A14: lw          $t7, 0x0($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X0);
    // 0x80172A18: sll         $t6, $a0, 3
    ctx->r14 = S32(ctx->r4 << 3);
    // 0x80172A1C: addu        $s0, $t6, $t7
    ctx->r16 = ADD32(ctx->r14, ctx->r15);
    // 0x80172A20: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x80172A24: bne         $s2, $t8, L_80172A58
    if (ctx->r18 != ctx->r24) {
        // 0x80172A28: nop
    
            goto L_80172A58;
    }
    // 0x80172A28: nop

    // 0x80172A2C: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x80172A30: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x80172A34: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x80172A38: beql        $v0, $zero, L_80172A6C
    if (ctx->r2 == 0) {
        // 0x80172A3C: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_80172A6C;
    }
    goto skip_0;
    // 0x80172A3C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_0:
    // 0x80172A40: lw          $t0, -0x610($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X610);
    // 0x80172A44: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80172A48: and         $t9, $v0, $at
    ctx->r25 = ctx->r2 & ctx->r1;
    // 0x80172A4C: addu        $t1, $t9, $t0
    ctx->r9 = ADD32(ctx->r25, ctx->r8);
    // 0x80172A50: b           L_80172A68
    // 0x80172A54: sw          $t1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r9;
        goto L_80172A68;
    // 0x80172A54: sw          $t1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r9;
L_80172A58:
    // 0x80172A58: jal         0x801729E8
    // 0x80172A5C: lh          $a0, 0x4($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X4);
    func_801729E8(rdram, ctx);
        goto after_0;
    // 0x80172A5C: lh          $a0, 0x4($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X4);
    after_0:
    // 0x80172A60: b           L_80172A14
    // 0x80172A64: lh          $a0, 0x6($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X6);
        goto L_80172A14;
    // 0x80172A64: lh          $a0, 0x6($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X6);
L_80172A68:
    // 0x80172A68: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_80172A6C:
    // 0x80172A6C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80172A70: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80172A74: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80172A78: jr          $ra
    // 0x80172A7C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
#if LOD_FIX_TEXT_MEASURE_GUARD
    // --- PATCH: recursion-depth guard, matching decrement for the entry
    // increment above (this function's single natural return point) ---
    lod_text_guard_depth_801729E8--;
    // --- END PATCH ---
#endif
    return;
    // 0x80172A7C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80172A80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172A80: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172A84: lwc1        $f0, -0x4F10($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X4F10);
    // 0x80172A88: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80172A8C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80172A90: c.lt.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl < ctx->f0.fl;
    // 0x80172A94: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172A98: bc1fl       L_80172AC0
    if (!c1cs) {
        // 0x80172A9C: c.lt.s      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
            goto L_80172AC0;
    }
    goto skip_0;
    // 0x80172A9C: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    skip_0:
    // 0x80172AA0: lwc1        $f4, -0x4F0C($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X4F0C);
    // 0x80172AA4: c.lt.s      $f4, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f4.fl < ctx->f14.fl;
    // 0x80172AA8: nop

    // 0x80172AAC: bc1fl       L_80172AC0
    if (!c1cs) {
        // 0x80172AB0: c.lt.s      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
            goto L_80172AC0;
    }
    goto skip_1;
    // 0x80172AB0: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    skip_1:
    // 0x80172AB4: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x80172AB8: nop

    // 0x80172ABC: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
L_80172AC0:
    // 0x80172AC0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172AC4: bc1f        L_80172AE8
    if (!c1cs) {
        // 0x80172AC8: nop
    
            goto L_80172AE8;
    }
    // 0x80172AC8: nop

    // 0x80172ACC: lwc1        $f6, -0x4F08($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X4F08);
    // 0x80172AD0: c.lt.s      $f6, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f6.fl < ctx->f12.fl;
    // 0x80172AD4: nop

    // 0x80172AD8: bc1f        L_80172AE8
    if (!c1cs) {
        // 0x80172ADC: nop
    
            goto L_80172AE8;
    }
    // 0x80172ADC: nop

    // 0x80172AE0: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x80172AE4: nop

L_80172AE8:
    // 0x80172AE8: jal         0x80004C24
    // 0x80172AEC: nop

    func_80004C24(rdram, ctx);
        goto after_0;
    // 0x80172AEC: nop

    after_0:
    // 0x80172AF0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80172AF4: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x80172AF8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80172AFC: jr          $ra
    // 0x80172B00: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
    return;
    // 0x80172B00: cvt.s.w     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    ctx->f0.fl = CVT_S_W(ctx->f8.u32l);
;}
RECOMP_FUNC void func_80172B04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172B04: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172B08: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80172B0C: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172B10: lwc1        $f16, 0x4($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80172B14: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80172B18: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80172B1C: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80172B20: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80172B24: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80172B28: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x80172B2C: jr          $ra
    // 0x80172B30: add.s       $f0, $f16, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f16.fl + ctx->f4.fl;
    return;
    // 0x80172B30: add.s       $f0, $f16, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f16.fl + ctx->f4.fl;
;}
RECOMP_FUNC void func_80172B34(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172B34: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172B38: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80172B3C: lwc1        $f10, 0x4($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80172B40: lwc1        $f16, 0x8($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80172B44: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80172B48: nop

    // 0x80172B4C: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80172B50: sub.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f18.fl;
    // 0x80172B54: swc1        $f4, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->f4.u32l;
    // 0x80172B58: lwc1        $f10, 0x0($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80172B5C: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80172B60: lwc1        $f18, 0x0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172B64: lwc1        $f8, 0x8($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80172B68: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80172B6C: nop

    // 0x80172B70: mul.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x80172B74: sub.s       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f16.fl - ctx->f4.fl;
    // 0x80172B78: swc1        $f6, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f6.u32l;
    // 0x80172B7C: lwc1        $f8, 0x4($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80172B80: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172B84: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172B88: lwc1        $f16, 0x0($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80172B8C: mul.s       $f18, $f10, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f8.fl);
    // 0x80172B90: nop

    // 0x80172B94: mul.s       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f4.fl);
    // 0x80172B98: sub.s       $f10, $f18, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f18.fl - ctx->f6.fl;
    // 0x80172B9C: jr          $ra
    // 0x80172BA0: swc1        $f10, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f10.u32l;
    return;
    // 0x80172BA0: swc1        $f10, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f10.u32l;
;}
RECOMP_FUNC void func_80172BA4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172BA4: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80172BA8: or          $t1, $a0, $zero
    ctx->r9 = ctx->r4 | 0;
    // 0x80172BAC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80172BB0: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80172BB4: addiu       $t0, $sp, 0x38
    ctx->r8 = ADD32(ctx->r29, 0X38);
    // 0x80172BB8: addiu       $v1, $sp, 0x2C
    ctx->r3 = ADD32(ctx->r29, 0X2C);
    // 0x80172BBC: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80172BC0: or          $t2, $a2, $zero
    ctx->r10 = ctx->r6 | 0;
L_80172BC4:
    // 0x80172BC4: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80172BC8: lwc1        $f6, 0x0($t1)
    ctx->f6.u32l = MEM_W(ctx->r9, 0X0);
    // 0x80172BCC: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80172BD0: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x80172BD4: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80172BD8: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80172BDC: addiu       $t1, $t1, 0x4
    ctx->r9 = ADD32(ctx->r9, 0X4);
    // 0x80172BE0: addiu       $t2, $t2, 0x4
    ctx->r10 = ADD32(ctx->r10, 0X4);
    // 0x80172BE4: swc1        $f8, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->f8.u32l;
    // 0x80172BE8: lwc1        $f16, -0x4($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, -0X4);
    // 0x80172BEC: lwc1        $f10, -0x4($t2)
    ctx->f10.u32l = MEM_W(ctx->r10, -0X4);
    // 0x80172BF0: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x80172BF4: bne         $v1, $a0, L_80172BC4
    if (ctx->r3 != ctx->r4) {
        // 0x80172BF8: swc1        $f18, -0x4($v1)
        MEM_W(-0X4, ctx->r3) = ctx->f18.u32l;
            goto L_80172BC4;
    }
    // 0x80172BF8: swc1        $f18, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f18.u32l;
    // 0x80172BFC: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80172C00: addiu       $a1, $sp, 0x2C
    ctx->r5 = ADD32(ctx->r29, 0X2C);
    // 0x80172C04: jal         0x80172B34
    // 0x80172C08: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    func_80172B34(rdram, ctx);
        goto after_0;
    // 0x80172C08: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    after_0:
    // 0x80172C0C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80172C10: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x80172C14: jr          $ra
    // 0x80172C18: nop

    return;
    // 0x80172C18: nop

;}
RECOMP_FUNC void func_80172C1C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172C1C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80172C20: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80172C24: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80172C28: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80172C2C: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80172C30: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80172C34: andi        $a0, $a1, 0xFFFF
    ctx->r4 = ctx->r5 & 0XFFFF;
    // 0x80172C38: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x80172C3C: jal         0x800A3A50
    // 0x80172C40: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    func_800A3A50(rdram, ctx);
        goto after_0;
    // 0x80172C40: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    after_0:
    // 0x80172C44: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80172C48: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172C4C: lwc1        $f8, -0x4F04($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X4F04);
    // 0x80172C50: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80172C54: lwc1        $f16, 0x28($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80172C58: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    // 0x80172C5C: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80172C60: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80172C64: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80172C68: jal         0x80097330
    // 0x80172C6C: swc1        $f18, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f18.u32l;
    func_80097330(rdram, ctx);
        goto after_1;
    // 0x80172C6C: swc1        $f18, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f18.u32l;
    after_1:
    // 0x80172C70: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80172C74: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80172C78: lwc1        $f8, -0x4F00($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X4F00);
    // 0x80172C7C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80172C80: lwc1        $f18, 0x28($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80172C84: lw          $t7, 0x20($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X20);
    // 0x80172C88: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80172C8C: neg.s       $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = -ctx->f10.fl;
    // 0x80172C90: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x80172C94: swc1        $f4, 0x8($t7)
    MEM_W(0X8, ctx->r15) = ctx->f4.u32l;
    // 0x80172C98: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80172C9C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80172CA0: jr          $ra
    // 0x80172CA4: nop

    return;
    // 0x80172CA4: nop

;}
RECOMP_FUNC void func_80172CA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172CA8: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x80172CAC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80172CB0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80172CB4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80172CB8: jal         0x80172D54
    // 0x80172CBC: swc1        $f12, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f12.u32l;
    func_80172D54(rdram, ctx);
        goto after_0;
    // 0x80172CBC: swc1        $f12, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f12.u32l;
    after_0:
    // 0x80172CC0: lwc1        $f12, 0x1C($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x80172CC4: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80172CC8: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x80172CCC: nop

    // 0x80172CD0: bc1fl       L_80172D28
    if (!c1cs) {
        // 0x80172CD4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80172D28;
    }
    goto skip_0;
    // 0x80172CD4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80172CD8: jal         0x80172EC4
    // 0x80172CDC: swc1        $f12, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f12.u32l;
    func_80172EC4(rdram, ctx);
        goto after_1;
    // 0x80172CDC: swc1        $f12, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f12.u32l;
    after_1:
    // 0x80172CE0: lwc1        $f12, 0x1C($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x80172CE4: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80172CE8: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80172CEC: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    // 0x80172CF0: lwc1        $f16, 0x0($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80172CF4: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80172CF8: mul.s       $f18, $f16, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f12.fl);
    // 0x80172CFC: beql        $v1, $a0, L_80172D20
    if (ctx->r3 == ctx->r4) {
        // 0x80172D00: swc1        $f18, 0x0($v0)
        MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
            goto L_80172D20;
    }
    goto skip_1;
    // 0x80172D00: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    skip_1:
L_80172D04:
    // 0x80172D04: lwc1        $f16, 0x4($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80172D08: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80172D0C: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x80172D10: mul.s       $f18, $f16, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f12.fl);
    // 0x80172D14: bne         $v1, $a0, L_80172D04
    if (ctx->r3 != ctx->r4) {
        // 0x80172D18: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_80172D04;
    }
    // 0x80172D18: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80172D1C: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
L_80172D20:
    // 0x80172D20: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80172D24: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80172D28:
    // 0x80172D28: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80172D2C: jr          $ra
    // 0x80172D30: nop

    return;
    // 0x80172D30: nop

;}
RECOMP_FUNC void func_80172D34(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172D34: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80172D38: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80172D3C: jal         0x800A01E0
    // 0x80172D40: nop

    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x80172D40: nop

    after_0:
    // 0x80172D44: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80172D48: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80172D4C: jr          $ra
    // 0x80172D50: nop

    return;
    // 0x80172D50: nop

;}
RECOMP_FUNC void func_80172D54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172D54: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80172D58: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80172D5C: lwc1        $f2, 0x0($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172D60: lwc1        $f14, 0x4($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172D64: lwc1        $f0, 0x8($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80172D68: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80172D6C: nop

    // 0x80172D70: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80172D74: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80172D78: mul.s       $f10, $f0, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80172D7C: jal         0x800A01E0
    // 0x80172D80: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x80172D80: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
    after_0:
    // 0x80172D84: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80172D88: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80172D8C: jr          $ra
    // 0x80172D90: nop

    return;
    // 0x80172D90: nop

;}
RECOMP_FUNC void func_80172D94(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172D94: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80172D98: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    // 0x80172D9C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80172DA0: or          $a3, $a2, $zero
    ctx->r7 = ctx->r6 | 0;
    // 0x80172DA4: or          $t0, $a1, $zero
    ctx->r8 = ctx->r5 | 0;
L_80172DA8:
    // 0x80172DA8: lwc1        $f4, 0x0($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80172DAC: lwc1        $f6, 0x0($t0)
    ctx->f6.u32l = MEM_W(ctx->r8, 0X0);
    // 0x80172DB0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80172DB4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80172DB8: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80172DBC: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x80172DC0: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x80172DC4: bne         $v0, $a0, L_80172DA8
    if (ctx->r2 != ctx->r4) {
        // 0x80172DC8: swc1        $f8, -0x4($v1)
        MEM_W(-0X4, ctx->r3) = ctx->f8.u32l;
            goto L_80172DA8;
    }
    // 0x80172DC8: swc1        $f8, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f8.u32l;
    // 0x80172DCC: jr          $ra
    // 0x80172DD0: nop

    return;
    // 0x80172DD0: nop

;}
RECOMP_FUNC void func_80172DD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172DD4: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80172DD8: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    // 0x80172DDC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80172DE0: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x80172DE4: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
L_80172DE8:
    // 0x80172DE8: lwc1        $f4, 0x0($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80172DEC: lwc1        $f6, 0x0($t0)
    ctx->f6.u32l = MEM_W(ctx->r8, 0X0);
    // 0x80172DF0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80172DF4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80172DF8: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80172DFC: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x80172E00: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x80172E04: bne         $v0, $a0, L_80172DE8
    if (ctx->r2 != ctx->r4) {
        // 0x80172E08: swc1        $f8, -0x4($v1)
        MEM_W(-0X4, ctx->r3) = ctx->f8.u32l;
            goto L_80172DE8;
    }
    // 0x80172E08: swc1        $f8, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f8.u32l;
    // 0x80172E0C: jr          $ra
    // 0x80172E10: nop

    return;
    // 0x80172E10: nop

;}
RECOMP_FUNC void func_80172E14(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172E14: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80172E18: nop

    // 0x80172E1C: swc1        $f0, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f0.u32l;
    // 0x80172E20: swc1        $f0, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f0.u32l;
    // 0x80172E24: jr          $ra
    // 0x80172E28: swc1        $f0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f0.u32l;
    return;
    // 0x80172E28: swc1        $f0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f0.u32l;
;}
RECOMP_FUNC void func_80172E2C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172E2C: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80172E30: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    // 0x80172E34: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80172E38: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
L_80172E3C:
    // 0x80172E3C: lwc1        $f4, 0x0($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X0);
    // 0x80172E40: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80172E44: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80172E48: addiu       $a2, $a2, 0x4
    ctx->r6 = ADD32(ctx->r6, 0X4);
    // 0x80172E4C: bne         $v0, $a0, L_80172E3C
    if (ctx->r2 != ctx->r4) {
        // 0x80172E50: swc1        $f4, -0x4($v1)
        MEM_W(-0X4, ctx->r3) = ctx->f4.u32l;
            goto L_80172E3C;
    }
    // 0x80172E50: swc1        $f4, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f4.u32l;
    // 0x80172E54: jr          $ra
    // 0x80172E58: nop

    return;
    // 0x80172E58: nop

;}
RECOMP_FUNC void func_80172E5C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172E5C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80172E60: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80172E64: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80172E68: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80172E6C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80172E70: blez        $a2, L_80172EA8
    if (SIGNED(ctx->r6) <= 0) {
        // 0x80172E74: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_80172EA8;
    }
    // 0x80172E74: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80172E78: sll         $t6, $a2, 2
    ctx->r14 = S32(ctx->r6 << 2);
    // 0x80172E7C: subu        $t6, $t6, $a2
    ctx->r14 = SUB32(ctx->r14, ctx->r6);
    // 0x80172E80: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80172E84: addu        $s3, $t6, $a1
    ctx->r19 = ADD32(ctx->r14, ctx->r5);
    // 0x80172E88: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80172E8C: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
L_80172E90:
    // 0x80172E90: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80172E94: jal         0x80172E2C
    // 0x80172E98: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80172E2C(rdram, ctx);
        goto after_0;
    // 0x80172E98: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_0:
    // 0x80172E9C: addiu       $s0, $s0, 0xC
    ctx->r16 = ADD32(ctx->r16, 0XC);
    // 0x80172EA0: bne         $s0, $s3, L_80172E90
    if (ctx->r16 != ctx->r19) {
        // 0x80172EA4: addiu       $s1, $s1, 0xC
        ctx->r17 = ADD32(ctx->r17, 0XC);
            goto L_80172E90;
    }
    // 0x80172EA4: addiu       $s1, $s1, 0xC
    ctx->r17 = ADD32(ctx->r17, 0XC);
L_80172EA8:
    // 0x80172EA8: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80172EAC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80172EB0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80172EB4: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80172EB8: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80172EBC: jr          $ra
    // 0x80172EC0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80172EC0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80172EC4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172EC4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80172EC8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80172ECC: jal         0x80172D54
    // 0x80172ED0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80172D54(rdram, ctx);
        goto after_0;
    // 0x80172ED0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80172ED4: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80172ED8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80172EDC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80172EE0: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x80172EE4: nop

    // 0x80172EE8: bc1tl       L_80172F2C
    if (c1cs) {
        // 0x80172EEC: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_80172F2C;
    }
    goto skip_0;
    // 0x80172EEC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_0:
    // 0x80172EF0: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80172EF4: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172EF8: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172EFC: div.s       $f2, $f6, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80172F00: lwc1        $f4, 0x8($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80172F04: mul.s       $f10, $f8, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x80172F08: nop

    // 0x80172F0C: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x80172F10: nop

    // 0x80172F14: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80172F18: swc1        $f10, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f10.u32l;
    // 0x80172F1C: swc1        $f18, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f18.u32l;
    // 0x80172F20: b           L_80172F3C
    // 0x80172F24: swc1        $f6, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f6.u32l;
        goto L_80172F3C;
    // 0x80172F24: swc1        $f6, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f6.u32l;
    // 0x80172F28: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
L_80172F2C:
    // 0x80172F2C: nop

    // 0x80172F30: swc1        $f0, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f0.u32l;
    // 0x80172F34: swc1        $f0, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f0.u32l;
    // 0x80172F38: swc1        $f0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f0.u32l;
L_80172F3C:
    // 0x80172F3C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80172F40: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80172F44: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80172F48: jr          $ra
    // 0x80172F4C: nop

    return;
    // 0x80172F4C: nop

;}
RECOMP_FUNC void func_80172F50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80172F50: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80172F54: lwc1        $f2, 0x8($a1)
    ctx->f2.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80172F58: lwc1        $f4, 0x4($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X4);
    // 0x80172F5C: lwc1        $f8, 0x8($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80172F60: lwc1        $f18, 0x4($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172F64: sub.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x80172F68: or          $v0, $a3, $zero
    ctx->r2 = ctx->r7 | 0;
    // 0x80172F6C: sub.s       $f10, $f2, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f2.fl - ctx->f8.fl;
    // 0x80172F70: lwc1        $f8, 0x8($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X8);
    // 0x80172F74: sub.s       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f0.fl - ctx->f18.fl;
    // 0x80172F78: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80172F7C: sub.s       $f6, $f8, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80172F80: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80172F84: sub.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x80172F88: swc1        $f18, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f18.u32l;
    // 0x80172F8C: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172F90: lwc1        $f12, 0x0($a1)
    ctx->f12.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80172F94: lwc1        $f8, 0x8($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X8);
    // 0x80172F98: lwc1        $f2, 0x8($a1)
    ctx->f2.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80172F9C: sub.s       $f16, $f12, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = ctx->f12.fl - ctx->f6.fl;
    // 0x80172FA0: lwc1        $f6, 0x0($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X0);
    // 0x80172FA4: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80172FA8: sub.s       $f4, $f8, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80172FAC: sub.s       $f8, $f2, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = ctx->f2.fl - ctx->f18.fl;
    // 0x80172FB0: mul.s       $f10, $f4, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x80172FB4: sub.s       $f4, $f6, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f12.fl;
    // 0x80172FB8: mul.s       $f16, $f8, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x80172FBC: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x80172FC0: swc1        $f18, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f18.u32l;
    // 0x80172FC4: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80172FC8: lwc1        $f6, 0x0($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X0);
    // 0x80172FCC: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80172FD0: lwc1        $f12, 0x0($a1)
    ctx->f12.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80172FD4: lwc1        $f18, 0x0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80172FD8: sub.s       $f10, $f0, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x80172FDC: lwc1        $f4, 0x4($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X4);
    // 0x80172FE0: sub.s       $f8, $f6, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f12.fl;
    // 0x80172FE4: sub.s       $f6, $f12, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = ctx->f12.fl - ctx->f18.fl;
    // 0x80172FE8: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80172FEC: sub.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x80172FF0: lwc1        $f4, 0x0($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80172FF4: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80172FF8: sub.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x80172FFC: swc1        $f18, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->f18.u32l;
    // 0x80173000: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80173004: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80173008: lwc1        $f16, 0x8($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X8);
    // 0x8017300C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80173010: lwc1        $f6, 0x4($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80173014: mul.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f10.fl);
    // 0x80173018: lwc1        $f16, 0x4($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X4);
    // 0x8017301C: mul.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80173020: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x80173024: add.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80173028: neg.s       $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = -ctx->f8.fl;
    // 0x8017302C: jr          $ra
    // 0x80173030: swc1        $f18, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->f18.u32l;
    return;
    // 0x80173030: swc1        $f18, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->f18.u32l;
;}
RECOMP_FUNC void func_80173034(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80173034: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80173038: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8017303C: lwc1        $f4, 0x4($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X4);
    // 0x80173040: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80173044: lwc1        $f8, 0x8($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80173048: lwc1        $f2, 0x8($a1)
    ctx->f2.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8017304C: sub.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x80173050: lwc1        $f18, 0x4($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80173054: sub.s       $f10, $f2, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f2.fl - ctx->f8.fl;
    // 0x80173058: lwc1        $f8, 0x8($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X8);
    // 0x8017305C: sub.s       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f0.fl - ctx->f18.fl;
    // 0x80173060: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80173064: sub.s       $f6, $f8, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80173068: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8017306C: sub.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x80173070: swc1        $f18, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f18.u32l;
    // 0x80173074: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80173078: lwc1        $f12, 0x0($a1)
    ctx->f12.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8017307C: lwc1        $f8, 0x8($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X8);
    // 0x80173080: lwc1        $f2, 0x8($a1)
    ctx->f2.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80173084: sub.s       $f16, $f12, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = ctx->f12.fl - ctx->f6.fl;
    // 0x80173088: lwc1        $f6, 0x0($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X0);
    // 0x8017308C: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80173090: sub.s       $f4, $f8, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80173094: sub.s       $f8, $f2, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = ctx->f2.fl - ctx->f18.fl;
    // 0x80173098: mul.s       $f10, $f4, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x8017309C: lwc1        $f2, 0x0($a3)
    ctx->f2.u32l = MEM_W(ctx->r7, 0X0);
    // 0x801730A0: sub.s       $f4, $f6, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f12.fl;
    // 0x801730A4: mul.s       $f16, $f8, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x801730A8: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x801730AC: swc1        $f18, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f18.u32l;
    // 0x801730B0: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x801730B4: lwc1        $f6, 0x0($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X0);
    // 0x801730B8: lwc1        $f0, 0x4($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0X4);
    // 0x801730BC: lwc1        $f12, 0x0($a1)
    ctx->f12.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801730C0: lwc1        $f18, 0x0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801730C4: sub.s       $f10, $f0, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x801730C8: lwc1        $f4, 0x4($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X4);
    // 0x801730CC: lwc1        $f14, 0x4($a3)
    ctx->f14.u32l = MEM_W(ctx->r7, 0X4);
    // 0x801730D0: sub.s       $f8, $f6, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f12.fl;
    // 0x801730D4: sub.s       $f6, $f12, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = ctx->f12.fl - ctx->f18.fl;
    // 0x801730D8: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x801730DC: sub.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f0.fl;
    // 0x801730E0: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x801730E4: sub.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x801730E8: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801730EC: nop

    // 0x801730F0: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x801730F4: swc1        $f18, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->f18.u32l;
    // 0x801730F8: lwc1        $f0, 0x8($a3)
    ctx->f0.u32l = MEM_W(ctx->r7, 0X8);
    // 0x801730FC: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    // 0x80173100: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80173104: mul.s       $f16, $f0, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80173108: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8017310C: jal         0x800A01E0
    // 0x80173110: add.s       $f12, $f16, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x80173110: add.s       $f12, $f16, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f8.fl;
    after_0:
    // 0x80173114: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80173118: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8017311C: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x80173120: c.eq.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl == ctx->f10.fl;
    // 0x80173124: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x80173128: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017312C: bc1fl       L_8017313C
    if (!c1cs) {
        // 0x80173130: lui         $at, 0x3F80
        ctx->r1 = S32(0X3F80 << 16);
            goto L_8017313C;
    }
    goto skip_0;
    // 0x80173130: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    skip_0:
    // 0x80173134: lwc1        $f2, -0x4EFC($at)
    ctx->f2.u32l = MEM_W(ctx->r1, -0X4EFC);
    // 0x80173138: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
L_8017313C:
    // 0x8017313C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x80173140: lwc1        $f4, 0x0($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80173144: lwc1        $f16, 0x8($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X8);
    // 0x80173148: div.s       $f2, $f18, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = DIV_S(ctx->f18.fl, ctx->f2.fl);
    // 0x8017314C: lwc1        $f10, 0x4($a3)
    ctx->f10.u32l = MEM_W(ctx->r7, 0X4);
    // 0x80173150: or          $v0, $a3, $zero
    ctx->r2 = ctx->r7 | 0;
    // 0x80173154: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80173158: nop

    // 0x8017315C: mul.s       $f8, $f16, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x80173160: nop

    // 0x80173164: mul.s       $f18, $f10, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80173168: swc1        $f6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f6.u32l;
    // 0x8017316C: lwc1        $f4, 0x0($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X0);
    // 0x80173170: swc1        $f8, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->f8.u32l;
    // 0x80173174: lwc1        $f8, 0x8($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X8);
    // 0x80173178: swc1        $f18, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f18.u32l;
    // 0x8017317C: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80173180: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80173184: mul.s       $f16, $f4, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80173188: lwc1        $f6, 0x4($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8017318C: mul.s       $f18, $f8, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80173190: lwc1        $f8, 0x4($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X4);
    // 0x80173194: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80173198: add.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x8017319C: add.s       $f16, $f10, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x801731A0: neg.s       $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = -ctx->f16.fl;
    // 0x801731A4: swc1        $f18, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->f18.u32l;
    // 0x801731A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801731AC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801731B0: jr          $ra
    // 0x801731B4: nop

    return;
    // 0x801731B4: nop

;}
RECOMP_FUNC void func_801731B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801731B8: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801731BC: lwc1        $f6, 0x0($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801731C0: lwc1        $f10, 0x4($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X4);
    // 0x801731C4: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x801731C8: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801731CC: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801731D0: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x801731D4: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x801731D8: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x801731DC: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x801731E0: lwc1        $f18, 0xC($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0XC);
    // 0x801731E4: add.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x801731E8: add.s       $f2, $f18, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = ctx->f18.fl + ctx->f8.fl;
    // 0x801731EC: jr          $ra
    // 0x801731F0: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    return;
    // 0x801731F0: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
;}
