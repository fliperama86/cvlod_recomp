#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

#if LOD_ENABLE_NI0E_TRACE
// Round 8: gate instrumentation for pair 129's day/time controller state
// function ni_ovl_129_func_0F0000C8 (vaddr 0x0F0000C8). See
// docs/issue27-31-ni0e-findings.md Round 8 for the full branch map this
// instrumentation is keyed to.
#include <stdio.h>
#include <stdbool.h>

extern uint32_t lod_current_map_overlay_rom(void);

// Round 8 (task 3): no static writer of the days-banner text struct's
// +0x10/+0x14/+0x18 fields (the pointer resolved from global 0x8019EECC)
// was found anywhere in RecompiledFuncs (see docs/issue27-31-ni0e-findings.md
// Round 8, task 3). ni_ovl_129_func_0F000938 ("banner DL build", called from
// the BUILD path below) is the closest static candidate per the task brief,
// even though it statically appears to populate a *different* structure
// (obj+0x38, offsets +0x0/+0x4/+0x8/+0xC). This probe empirically checks
// whether the global struct's fields change across that call at runtime.
static uint32_t lod_ni0e_daypop_logged = 0;

static bool lod_ni0e_daypop_addr_ok(uint32_t addr) {
    if (addr == 0) {
        return false;
    }
    const uint32_t phys = addr & 0x1FFFFFFFu;
    return phys <= 0x800000u - 0x3Cu;
}

static void lod_ni0e_daypop_probe(uint8_t* rdram, const char* site) {
    if (lod_ni0e_daypop_logged >= 40u) {
        return;
    }
    lod_ni0e_daypop_logged++;
    const gpr global_gpr = (gpr)(int32_t)0x8019EECCu;
    const uint32_t ptr = (uint32_t)MEM_W(0x0, global_gpr);
    if (!lod_ni0e_daypop_addr_ok(ptr)) {
        fprintf(stderr, "[NI0E_TRACE] daypop %s ptr=0x%08X (invalid, skipped)\n", site, ptr);
        return;
    }
    const gpr ptr_gpr = (gpr)(int32_t)ptr;
    const uint32_t f10 = (uint32_t)MEM_W(0x10, ptr_gpr);
    const uint32_t f14 = (uint32_t)MEM_W(0x14, ptr_gpr);
    const uint32_t f18 = (uint32_t)MEM_W(0x18, ptr_gpr);
    fprintf(stderr, "[NI0E_TRACE] daypop %s ptr=0x%08X f10=0x%08X f14=0x%08X f18=0x%08X\n",
            site, ptr, f10, f14, f18);
}

static uint32_t lod_ni0e_daygate_entry_calls = 0;
static uint32_t lod_ni0e_daygate_flagtest_calls = 0;
static uint32_t lod_ni0e_daygate_build_calls = 0;
static uint32_t lod_ni0e_daygate_skip_busy = 0;
static uint32_t lod_ni0e_daygate_skip_locked = 0;
static uint32_t lod_ni0e_daygate_skip_state5 = 0;
static uint32_t lod_ni0e_daygate_skip_hour_unchanged = 0;
static uint32_t lod_ni0e_daygate_skip_day_flag_set = 0;

// Gate inputs: sys+0x2B4E halfword, sys+0x2BC8 word, obj+0x3C (hour cache).
static void lod_ni0e_daygate_entry_probe(uint8_t* rdram, gpr obj) {
    lod_ni0e_daygate_entry_calls++;
    if (lod_ni0e_daygate_entry_calls > 60 && (lod_ni0e_daygate_entry_calls % 300) != 0) {
        return;
    }
    const gpr sys = (gpr)(int32_t)0x801C82C0u;
    const uint16_t sys_2b4e = (uint16_t)MEM_H(sys, 0X2B4E);
    const uint32_t sys_2bc8 = (uint32_t)MEM_W(sys, 0X2BC8);
    const uint32_t obj_3c = (uint32_t)MEM_W(obj, 0X3C);
    fprintf(stderr,
            "[NI0E_TRACE] daygate entry #%u sys_2B4E=0x%04X sys_2BC8=0x%08X obj_3C=0x%08X "
            "obj=0x%08X map_rom=0x%08X\n",
            lod_ni0e_daygate_entry_calls, sys_2b4e, sys_2bc8, obj_3c, (uint32_t)obj,
            lod_current_map_overlay_rom());
}

// Result of func_800048A0(0x801CAA60, 0x2A0) (save-event flag test), captured
// right after the call returns since it cannot be known at function entry.
static void lod_ni0e_daygate_flagtest_probe(uint32_t result) {
    lod_ni0e_daygate_flagtest_calls++;
    if (lod_ni0e_daygate_flagtest_calls > 60 && (lod_ni0e_daygate_flagtest_calls % 300) != 0) {
        return;
    }
    fprintf(stderr, "[NI0E_TRACE] daygate flagtest #%u result=0x%08X\n",
            lod_ni0e_daygate_flagtest_calls, result);
}

static void lod_ni0e_daygate_skip(uint32_t* counter, const char* tag) {
    (*counter)++;
    if (*counter > 20) {
        return;
    }
    fprintf(stderr, "[NI0E_TRACE] daygate skip=%s #%u map_rom=0x%08X\n", tag, *counter,
            lod_current_map_overlay_rom());
}

static void lod_ni0e_daygate_build_probe(gpr obj) {
    lod_ni0e_daygate_build_calls++;
    fprintf(stderr, "[NI0E_TRACE] daygate BUILD #%u obj=0x%08X map_rom=0x%08X\n",
            lod_ni0e_daygate_build_calls, (uint32_t)obj, lod_current_map_overlay_rom());
}
#endif  // LOD_ENABLE_NI0E_TRACE

RECOMP_FUNC void ni_ovl_123_func_0F0032DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0032DC: lbu         $t6, 0x67($a0)
    ctx->r14 = MEM_BU(ctx->r4, 0X67);
    // 0x0F0032E0: andi        $t7, $a1, 0x1
    ctx->r15 = ctx->r5 & 0X1;
    // 0x0F0032E4: beq         $t6, $zero, L_0F0032F4
    if (ctx->r14 == 0) {
        // 0x0F0032E8: nop
    
            goto L_0F0032F4;
    }
    // 0x0F0032E8: nop

    // 0x0F0032EC: jr          $ra
    // 0x0F0032F0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x0F0032F0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_0F0032F4:
    // 0x0F0032F4: beq         $t7, $zero, L_0F0033B8
    if (ctx->r15 == 0) {
            // 0x0F0032F8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    ni_ovl_123_func_0F0033B8(rdram, ctx);
    return;
    }
    // 0x0F0032F8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x0F0032FC: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x0F003300: lh          $t8, 0x53A($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X53A);
    // 0x0F003304: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x0F003308: slti        $at, $t8, 0x29
    ctx->r1 = SIGNED(ctx->r24) < 0X29 ? 1 : 0;
    // 0x0F00330C: bne         $at, $zero, L_0F0033B8
    if (ctx->r1 != 0) {
            // 0x0F003310: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    ni_ovl_123_func_0F0033B8(rdram, ctx);
    return;
    }
    // 0x0F003310: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F003314: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F003318: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x0F00331C: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x0F003320: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F003324: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F003328: cfc1        $t9, $FpcCsr
    ctx->r25 = get_cop1_cs();
    // 0x0F00332C: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x0F003330: nop

    // 0x0F003334: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F003338: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    // 0x0F00333C: nop

    // 0x0F003340: andi        $t0, $t0, 0x78
    ctx->r8 = ctx->r8 & 0X78;
    // 0x0F003344: beql        $t0, $zero, L_0F003394
    if (ctx->r8 == 0) {
        // 0x0F003348: mfc1        $t0, $f10
        ctx->r8 = (int32_t)ctx->f10.u32l;
            goto L_0F003394;
    }
    goto skip_0;
    // 0x0F003348: mfc1        $t0, $f10
    ctx->r8 = (int32_t)ctx->f10.u32l;
    skip_0:
    // 0x0F00334C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F003350: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x0F003354: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F003358: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x0F00335C: nop

    // 0x0F003360: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F003364: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    // 0x0F003368: nop

    // 0x0F00336C: andi        $t0, $t0, 0x78
    ctx->r8 = ctx->r8 & 0X78;
    // 0x0F003370: bne         $t0, $zero, L_0F003388
    if (ctx->r8 != 0) {
        // 0x0F003374: nop
    
            goto L_0F003388;
    }
    // 0x0F003374: nop

    // 0x0F003378: mfc1        $t0, $f10
    ctx->r8 = (int32_t)ctx->f10.u32l;
    // 0x0F00337C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F003380: b           L_0F0033A0
    // 0x0F003384: or          $t0, $t0, $at
    ctx->r8 = ctx->r8 | ctx->r1;
        goto L_0F0033A0;
    // 0x0F003384: or          $t0, $t0, $at
    ctx->r8 = ctx->r8 | ctx->r1;
L_0F003388:
    // 0x0F003388: b           L_0F0033A0
    // 0x0F00338C: addiu       $t0, $zero, -0x1
    ctx->r8 = ADD32(0, -0X1);
        goto L_0F0033A0;
    // 0x0F00338C: addiu       $t0, $zero, -0x1
    ctx->r8 = ADD32(0, -0X1);
    // 0x0F003390: mfc1        $t0, $f10
    ctx->r8 = (int32_t)ctx->f10.u32l;
L_0F003394:
    // 0x0F003394: nop

    // 0x0F003398: bltz        $t0, L_0F003388
    if (SIGNED(ctx->r8) < 0) {
        // 0x0F00339C: nop
    
            goto L_0F003388;
    }
    // 0x0F00339C: nop

L_0F0033A0:
    // 0x0F0033A0: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x0F0033A4: divu        $zero, $t0, $at
    lo = S32(U32(ctx->r8) / U32(ctx->r1)); hi = S32(U32(ctx->r8) % U32(ctx->r1));
    // 0x0F0033A8: mflo        $t1
    ctx->r9 = lo;
    // 0x0F0033AC: ctc1        $t9, $FpcCsr
    set_cop1_cs(ctx->r25);
    // 0x0F0033B0: jr          $ra
    // 0x0F0033B4: sb          $t1, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r9;
    return;
    // 0x0F0033B4: sb          $t1, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r9;
;}
RECOMP_FUNC void ni_ovl_123_func_0F0033B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0033B8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x0F0033BC: andi        $t2, $a1, 0x2
    ctx->r10 = ctx->r5 & 0X2;
    // 0x0F0033C0: beq         $t2, $zero, L_0F003480
    if (ctx->r10 == 0) {
        // 0x0F0033C4: addiu       $v1, $v1, -0x7D40
        ctx->r3 = ADD32(ctx->r3, -0X7D40);
            goto L_0F003480;
    }
    // 0x0F0033C4: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x0F0033C8: lh          $t3, 0x53A($v1)
    ctx->r11 = MEM_H(ctx->r3, 0X53A);
    // 0x0F0033CC: addiu       $v0, $zero, 0x2
    ctx->r2 = ADD32(0, 0X2);
    // 0x0F0033D0: slti        $at, $t3, -0x28
    ctx->r1 = SIGNED(ctx->r11) < -0X28 ? 1 : 0;
    // 0x0F0033D4: beq         $at, $zero, L_0F003480
    if (ctx->r1 == 0) {
        // 0x0F0033D8: lui         $at, 0x3F80
        ctx->r1 = S32(0X3F80 << 16);
            goto L_0F003480;
    }
    // 0x0F0033D8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0033DC: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F0033E0: lwc1        $f16, 0x8($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X8);
    // 0x0F0033E4: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x0F0033E8: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0033EC: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x0F0033F0: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x0F0033F4: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F0033F8: nop

    // 0x0F0033FC: cvt.w.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = CVT_W_S(ctx->f4.fl);
    // 0x0F003400: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F003404: nop

    // 0x0F003408: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x0F00340C: beql        $t5, $zero, L_0F00345C
    if (ctx->r13 == 0) {
        // 0x0F003410: mfc1        $t5, $f6
        ctx->r13 = (int32_t)ctx->f6.u32l;
            goto L_0F00345C;
    }
    goto skip_0;
    // 0x0F003410: mfc1        $t5, $f6
    ctx->r13 = (int32_t)ctx->f6.u32l;
    skip_0:
    // 0x0F003414: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F003418: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x0F00341C: sub.s       $f6, $f4, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x0F003420: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F003424: nop

    // 0x0F003428: cvt.w.s     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.u32l = CVT_W_S(ctx->f6.fl);
    // 0x0F00342C: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F003430: nop

    // 0x0F003434: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x0F003438: bne         $t5, $zero, L_0F003450
    if (ctx->r13 != 0) {
        // 0x0F00343C: nop
    
            goto L_0F003450;
    }
    // 0x0F00343C: nop

    // 0x0F003440: mfc1        $t5, $f6
    ctx->r13 = (int32_t)ctx->f6.u32l;
    // 0x0F003444: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F003448: b           L_0F003468
    // 0x0F00344C: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
        goto L_0F003468;
    // 0x0F00344C: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
L_0F003450:
    // 0x0F003450: b           L_0F003468
    // 0x0F003454: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
        goto L_0F003468;
    // 0x0F003454: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
    // 0x0F003458: mfc1        $t5, $f6
    ctx->r13 = (int32_t)ctx->f6.u32l;
L_0F00345C:
    // 0x0F00345C: nop

    // 0x0F003460: bltz        $t5, L_0F003450
    if (SIGNED(ctx->r13) < 0) {
        // 0x0F003464: nop
    
            goto L_0F003450;
    }
    // 0x0F003464: nop

L_0F003468:
    // 0x0F003468: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x0F00346C: divu        $zero, $t5, $at
    lo = S32(U32(ctx->r13) / U32(ctx->r1)); hi = S32(U32(ctx->r13) % U32(ctx->r1));
    // 0x0F003470: mflo        $t6
    ctx->r14 = lo;
    // 0x0F003474: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x0F003478: jr          $ra
    // 0x0F00347C: sb          $t6, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r14;
    return;
    // 0x0F00347C: sb          $t6, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r14;
L_0F003480:
    // 0x0F003480: andi        $t7, $a1, 0x4
    ctx->r15 = ctx->r5 & 0X4;
    // 0x0F003484: beq         $t7, $zero, L_0F003544
    if (ctx->r15 == 0) {
        // 0x0F003488: andi        $t2, $a1, 0x8
        ctx->r10 = ctx->r5 & 0X8;
            goto L_0F003544;
    }
    // 0x0F003488: andi        $t2, $a1, 0x8
    ctx->r10 = ctx->r5 & 0X8;
    // 0x0F00348C: lh          $t8, 0x53C($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X53C);
    // 0x0F003490: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x0F003494: slti        $at, $t8, 0x29
    ctx->r1 = SIGNED(ctx->r24) < 0X29 ? 1 : 0;
    // 0x0F003498: bne         $at, $zero, L_0F003544
    if (ctx->r1 != 0) {
        // 0x0F00349C: lui         $at, 0x3F80
        ctx->r1 = S32(0X3F80 << 16);
            goto L_0F003544;
    }
    // 0x0F00349C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0034A0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0034A4: lwc1        $f8, 0x8($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X8);
    // 0x0F0034A8: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x0F0034AC: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0034B0: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x0F0034B4: cfc1        $t9, $FpcCsr
    ctx->r25 = get_cop1_cs();
    // 0x0F0034B8: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x0F0034BC: nop

    // 0x0F0034C0: cvt.w.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = CVT_W_S(ctx->f16.fl);
    // 0x0F0034C4: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    // 0x0F0034C8: nop

    // 0x0F0034CC: andi        $t0, $t0, 0x78
    ctx->r8 = ctx->r8 & 0X78;
    // 0x0F0034D0: beql        $t0, $zero, L_0F003520
    if (ctx->r8 == 0) {
        // 0x0F0034D4: mfc1        $t0, $f18
        ctx->r8 = (int32_t)ctx->f18.u32l;
            goto L_0F003520;
    }
    goto skip_1;
    // 0x0F0034D4: mfc1        $t0, $f18
    ctx->r8 = (int32_t)ctx->f18.u32l;
    skip_1:
    // 0x0F0034D8: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F0034DC: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x0F0034E0: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x0F0034E4: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x0F0034E8: nop

    // 0x0F0034EC: cvt.w.s     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.u32l = CVT_W_S(ctx->f18.fl);
    // 0x0F0034F0: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    // 0x0F0034F4: nop

    // 0x0F0034F8: andi        $t0, $t0, 0x78
    ctx->r8 = ctx->r8 & 0X78;
    // 0x0F0034FC: bne         $t0, $zero, L_0F003514
    if (ctx->r8 != 0) {
        // 0x0F003500: nop
    
            goto L_0F003514;
    }
    // 0x0F003500: nop

    // 0x0F003504: mfc1        $t0, $f18
    ctx->r8 = (int32_t)ctx->f18.u32l;
    // 0x0F003508: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F00350C: b           L_0F00352C
    // 0x0F003510: or          $t0, $t0, $at
    ctx->r8 = ctx->r8 | ctx->r1;
        goto L_0F00352C;
    // 0x0F003510: or          $t0, $t0, $at
    ctx->r8 = ctx->r8 | ctx->r1;
L_0F003514:
    // 0x0F003514: b           L_0F00352C
    // 0x0F003518: addiu       $t0, $zero, -0x1
    ctx->r8 = ADD32(0, -0X1);
        goto L_0F00352C;
    // 0x0F003518: addiu       $t0, $zero, -0x1
    ctx->r8 = ADD32(0, -0X1);
    // 0x0F00351C: mfc1        $t0, $f18
    ctx->r8 = (int32_t)ctx->f18.u32l;
L_0F003520:
    // 0x0F003520: nop

    // 0x0F003524: bltz        $t0, L_0F003514
    if (SIGNED(ctx->r8) < 0) {
        // 0x0F003528: nop
    
            goto L_0F003514;
    }
    // 0x0F003528: nop

L_0F00352C:
    // 0x0F00352C: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x0F003530: divu        $zero, $t0, $at
    lo = S32(U32(ctx->r8) / U32(ctx->r1)); hi = S32(U32(ctx->r8) % U32(ctx->r1));
    // 0x0F003534: mflo        $t1
    ctx->r9 = lo;
    // 0x0F003538: ctc1        $t9, $FpcCsr
    set_cop1_cs(ctx->r25);
    // 0x0F00353C: jr          $ra
    // 0x0F003540: sb          $t1, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r9;
    return;
    // 0x0F003540: sb          $t1, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r9;
L_0F003544:
    // 0x0F003544: beql        $t2, $zero, L_0F003608
    if (ctx->r10 == 0) {
        // 0x0F003548: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_0F003608;
    }
    goto skip_2;
    // 0x0F003548: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_2:
    // 0x0F00354C: lh          $t3, 0x53C($v1)
    ctx->r11 = MEM_H(ctx->r3, 0X53C);
    // 0x0F003550: addiu       $v0, $zero, 0x8
    ctx->r2 = ADD32(0, 0X8);
    // 0x0F003554: slti        $at, $t3, -0x28
    ctx->r1 = SIGNED(ctx->r11) < -0X28 ? 1 : 0;
    // 0x0F003558: beq         $at, $zero, L_0F003604
    if (ctx->r1 == 0) {
        // 0x0F00355C: lui         $at, 0x3F80
        ctx->r1 = S32(0X3F80 << 16);
            goto L_0F003604;
    }
    // 0x0F00355C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F003560: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F003564: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x0F003568: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x0F00356C: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F003570: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F003574: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x0F003578: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F00357C: nop

    // 0x0F003580: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F003584: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F003588: nop

    // 0x0F00358C: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x0F003590: beql        $t5, $zero, L_0F0035E0
    if (ctx->r13 == 0) {
        // 0x0F003594: mfc1        $t5, $f10
        ctx->r13 = (int32_t)ctx->f10.u32l;
            goto L_0F0035E0;
    }
    goto skip_3;
    // 0x0F003594: mfc1        $t5, $f10
    ctx->r13 = (int32_t)ctx->f10.u32l;
    skip_3:
    // 0x0F003598: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F00359C: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x0F0035A0: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F0035A4: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F0035A8: nop

    // 0x0F0035AC: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F0035B0: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F0035B4: nop

    // 0x0F0035B8: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x0F0035BC: bne         $t5, $zero, L_0F0035D4
    if (ctx->r13 != 0) {
        // 0x0F0035C0: nop
    
            goto L_0F0035D4;
    }
    // 0x0F0035C0: nop

    // 0x0F0035C4: mfc1        $t5, $f10
    ctx->r13 = (int32_t)ctx->f10.u32l;
    // 0x0F0035C8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0035CC: b           L_0F0035EC
    // 0x0F0035D0: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
        goto L_0F0035EC;
    // 0x0F0035D0: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
L_0F0035D4:
    // 0x0F0035D4: b           L_0F0035EC
    // 0x0F0035D8: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
        goto L_0F0035EC;
    // 0x0F0035D8: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
    // 0x0F0035DC: mfc1        $t5, $f10
    ctx->r13 = (int32_t)ctx->f10.u32l;
L_0F0035E0:
    // 0x0F0035E0: nop

    // 0x0F0035E4: bltz        $t5, L_0F0035D4
    if (SIGNED(ctx->r13) < 0) {
        // 0x0F0035E8: nop
    
            goto L_0F0035D4;
    }
    // 0x0F0035E8: nop

L_0F0035EC:
    // 0x0F0035EC: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x0F0035F0: divu        $zero, $t5, $at
    lo = S32(U32(ctx->r13) / U32(ctx->r1)); hi = S32(U32(ctx->r13) % U32(ctx->r1));
    // 0x0F0035F4: mflo        $t6
    ctx->r14 = lo;
    // 0x0F0035F8: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x0F0035FC: jr          $ra
    // 0x0F003600: sb          $t6, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r14;
    return;
    // 0x0F003600: sb          $t6, 0x67($a0)
    MEM_B(0X67, ctx->r4) = ctx->r14;
L_0F003604:
    // 0x0F003604: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_0F003608:
    // 0x0F003608: jr          $ra
    // 0x0F00360C: nop

    return;
    // 0x0F00360C: nop

    // 0x0F003610: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x0F003614: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x0F003618: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x0F00361C: blez        $a1, L_0F00378C
    if (SIGNED(ctx->r5) <= 0) {
        // 0x0F003620: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_0F00378C;
    }
    // 0x0F003620: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x0F003624: andi        $v1, $a1, 0x1
    ctx->r3 = ctx->r5 & 0X1;
    // 0x0F003628: beql        $v1, $zero, L_0F0036AC
    if (ctx->r3 == 0) {
        // 0x0F00362C: ori         $t1, $zero, 0xFFFF
        ctx->r9 = 0 | 0XFFFF;
            goto L_0F0036AC;
    }
    goto skip_4;
    // 0x0F00362C: ori         $t1, $zero, 0xFFFF
    ctx->r9 = 0 | 0XFFFF;
    skip_4:
    // 0x0F003630: lhu         $a2, 0x0($a0)
    ctx->r6 = MEM_HU(ctx->r4, 0X0);
    // 0x0F003634: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x0F003638: ori         $t1, $zero, 0xFFFF
    ctx->r9 = 0 | 0XFFFF;
    // 0x0F00363C: or          $v1, $a2, $zero
    ctx->r3 = ctx->r6 | 0;
    // 0x0F003640: slti        $at, $v1, 0x100
    ctx->r1 = SIGNED(ctx->r3) < 0X100 ? 1 : 0;
    // 0x0F003644: bne         $at, $zero, L_0F003670
    if (ctx->r1 != 0) {
        // 0x0F003648: or          $a3, $v1, $zero
        ctx->r7 = ctx->r3 | 0;
            goto L_0F003670;
    }
    // 0x0F003648: or          $a3, $v1, $zero
    ctx->r7 = ctx->r3 | 0;
    // 0x0F00364C: bne         $t1, $a2, L_0F003664
    if (ctx->r9 != ctx->r6) {
        // 0x0F003650: addiu       $t6, $a1, -0x1
        ctx->r14 = ADD32(ctx->r5, -0X1);
            goto L_0F003664;
    }
    // 0x0F003650: addiu       $t6, $a1, -0x1
    ctx->r14 = ADD32(ctx->r5, -0X1);
    // 0x0F003654: blez        $t6, L_0F003664
    if (SIGNED(ctx->r14) <= 0) {
        // 0x0F003658: ori         $t7, $zero, 0xB700
        ctx->r15 = 0 | 0XB700;
            goto L_0F003664;
    }
    // 0x0F003658: ori         $t7, $zero, 0xB700
    ctx->r15 = 0 | 0XB700;
    // 0x0F00365C: b           L_0F0036A0
    // 0x0F003660: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
        goto L_0F0036A0;
    // 0x0F003660: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
L_0F003664:
    // 0x0F003664: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
    // 0x0F003668: b           L_0F0036A0
    // 0x0F00366C: nop

        goto L_0F0036A0;
    // 0x0F00366C: nop

L_0F003670:
    // 0x0F003670: slti        $at, $a2, 0x20
    ctx->r1 = SIGNED(ctx->r6) < 0X20 ? 1 : 0;
    // 0x0F003674: bne         $at, $zero, L_0F003688
    if (ctx->r1 != 0) {
        // 0x0F003678: or          $t0, $a2, $zero
        ctx->r8 = ctx->r6 | 0;
            goto L_0F003688;
    }
    // 0x0F003678: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
    // 0x0F00367C: addiu       $t8, $a3, -0x20
    ctx->r24 = ADD32(ctx->r7, -0X20);
    // 0x0F003680: b           L_0F0036A0
    // 0x0F003684: sh          $t8, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r24;
        goto L_0F0036A0;
    // 0x0F003684: sh          $t8, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r24;
L_0F003688:
    // 0x0F003688: addiu       $t2, $zero, 0xA
    ctx->r10 = ADD32(0, 0XA);
    // 0x0F00368C: bne         $t2, $t0, L_0F00369C
    if (ctx->r10 != ctx->r8) {
        // 0x0F003690: ori         $t3, $zero, 0xB600
        ctx->r11 = 0 | 0XB600;
            goto L_0F00369C;
    }
    // 0x0F003690: ori         $t3, $zero, 0xB600
    ctx->r11 = 0 | 0XB600;
    // 0x0F003694: b           L_0F0036A0
    // 0x0F003698: sh          $t3, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r11;
        goto L_0F0036A0;
    // 0x0F003698: sh          $t3, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r11;
L_0F00369C:
    // 0x0F00369C: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
L_0F0036A0:
    // 0x0F0036A0: beq         $v0, $a1, L_0F00378C
    if (ctx->r2 == ctx->r5) {
        // 0x0F0036A4: addiu       $a0, $a0, 0x2
        ctx->r4 = ADD32(ctx->r4, 0X2);
            goto L_0F00378C;
    }
    // 0x0F0036A4: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x0F0036A8: ori         $t1, $zero, 0xFFFF
    ctx->r9 = 0 | 0XFFFF;
L_0F0036AC:
    // 0x0F0036AC: addiu       $t2, $zero, 0xA
    ctx->r10 = ADD32(0, 0XA);
    // 0x0F0036B0: ori         $t3, $zero, 0xB600
    ctx->r11 = 0 | 0XB600;
L_0F0036B4:
    // 0x0F0036B4: lhu         $a2, 0x0($a0)
    ctx->r6 = MEM_HU(ctx->r4, 0X0);
    // 0x0F0036B8: or          $v1, $a2, $zero
    ctx->r3 = ctx->r6 | 0;
    // 0x0F0036BC: slti        $at, $v1, 0x100
    ctx->r1 = SIGNED(ctx->r3) < 0X100 ? 1 : 0;
    // 0x0F0036C0: bne         $at, $zero, L_0F0036EC
    if (ctx->r1 != 0) {
        // 0x0F0036C4: or          $a3, $v1, $zero
        ctx->r7 = ctx->r3 | 0;
            goto L_0F0036EC;
    }
    // 0x0F0036C4: or          $a3, $v1, $zero
    ctx->r7 = ctx->r3 | 0;
    // 0x0F0036C8: bne         $t1, $a2, L_0F0036E4
    if (ctx->r9 != ctx->r6) {
        // 0x0F0036CC: addiu       $t9, $a1, -0x1
        ctx->r25 = ADD32(ctx->r5, -0X1);
            goto L_0F0036E4;
    }
    // 0x0F0036CC: addiu       $t9, $a1, -0x1
    ctx->r25 = ADD32(ctx->r5, -0X1);
    // 0x0F0036D0: slt         $at, $v0, $t9
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r25) ? 1 : 0;
    // 0x0F0036D4: beq         $at, $zero, L_0F0036E4
    if (ctx->r1 == 0) {
        // 0x0F0036D8: ori         $t4, $zero, 0xB700
        ctx->r12 = 0 | 0XB700;
            goto L_0F0036E4;
    }
    // 0x0F0036D8: ori         $t4, $zero, 0xB700
    ctx->r12 = 0 | 0XB700;
    // 0x0F0036DC: b           L_0F003718
    // 0x0F0036E0: sh          $t4, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r12;
        goto L_0F003718;
    // 0x0F0036E0: sh          $t4, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r12;
L_0F0036E4:
    // 0x0F0036E4: b           L_0F003718
    // 0x0F0036E8: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
        goto L_0F003718;
    // 0x0F0036E8: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
L_0F0036EC:
    // 0x0F0036EC: slti        $at, $a2, 0x20
    ctx->r1 = SIGNED(ctx->r6) < 0X20 ? 1 : 0;
    // 0x0F0036F0: bne         $at, $zero, L_0F003704
    if (ctx->r1 != 0) {
        // 0x0F0036F4: or          $t0, $a2, $zero
        ctx->r8 = ctx->r6 | 0;
            goto L_0F003704;
    }
    // 0x0F0036F4: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
    // 0x0F0036F8: addiu       $t5, $a3, -0x20
    ctx->r13 = ADD32(ctx->r7, -0X20);
    // 0x0F0036FC: b           L_0F003718
    // 0x0F003700: sh          $t5, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r13;
        goto L_0F003718;
    // 0x0F003700: sh          $t5, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r13;
L_0F003704:
    // 0x0F003704: bnel        $t2, $t0, L_0F003718
    if (ctx->r10 != ctx->r8) {
        // 0x0F003708: sh          $v1, 0x0($a0)
        MEM_H(0X0, ctx->r4) = ctx->r3;
            goto L_0F003718;
    }
    goto skip_5;
    // 0x0F003708: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
    skip_5:
    // 0x0F00370C: b           L_0F003718
    // 0x0F003710: sh          $t3, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r11;
        goto L_0F003718;
    // 0x0F003710: sh          $t3, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r11;
    // 0x0F003714: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
L_0F003718:
    // 0x0F003718: lhu         $a2, 0x2($a0)
    ctx->r6 = MEM_HU(ctx->r4, 0X2);
    // 0x0F00371C: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x0F003720: or          $v1, $a2, $zero
    ctx->r3 = ctx->r6 | 0;
    // 0x0F003724: slti        $at, $v1, 0x100
    ctx->r1 = SIGNED(ctx->r3) < 0X100 ? 1 : 0;
    // 0x0F003728: bne         $at, $zero, L_0F003754
    if (ctx->r1 != 0) {
        // 0x0F00372C: or          $a3, $v1, $zero
        ctx->r7 = ctx->r3 | 0;
            goto L_0F003754;
    }
    // 0x0F00372C: or          $a3, $v1, $zero
    ctx->r7 = ctx->r3 | 0;
    // 0x0F003730: bne         $t1, $a2, L_0F00374C
    if (ctx->r9 != ctx->r6) {
        // 0x0F003734: addiu       $t6, $a1, -0x2
        ctx->r14 = ADD32(ctx->r5, -0X2);
            goto L_0F00374C;
    }
    // 0x0F003734: addiu       $t6, $a1, -0x2
    ctx->r14 = ADD32(ctx->r5, -0X2);
    // 0x0F003738: slt         $at, $v0, $t6
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r14) ? 1 : 0;
    // 0x0F00373C: beq         $at, $zero, L_0F00374C
    if (ctx->r1 == 0) {
        // 0x0F003740: ori         $t7, $zero, 0xB700
        ctx->r15 = 0 | 0XB700;
            goto L_0F00374C;
    }
    // 0x0F003740: ori         $t7, $zero, 0xB700
    ctx->r15 = 0 | 0XB700;
    // 0x0F003744: b           L_0F003780
    // 0x0F003748: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
        goto L_0F003780;
    // 0x0F003748: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
L_0F00374C:
    // 0x0F00374C: b           L_0F003780
    // 0x0F003750: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
        goto L_0F003780;
    // 0x0F003750: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
L_0F003754:
    // 0x0F003754: slti        $at, $a2, 0x20
    ctx->r1 = SIGNED(ctx->r6) < 0X20 ? 1 : 0;
    // 0x0F003758: bne         $at, $zero, L_0F00376C
    if (ctx->r1 != 0) {
        // 0x0F00375C: or          $t0, $a2, $zero
        ctx->r8 = ctx->r6 | 0;
            goto L_0F00376C;
    }
    // 0x0F00375C: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
    // 0x0F003760: addiu       $t8, $a3, -0x20
    ctx->r24 = ADD32(ctx->r7, -0X20);
    // 0x0F003764: b           L_0F003780
    // 0x0F003768: sh          $t8, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r24;
        goto L_0F003780;
    // 0x0F003768: sh          $t8, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r24;
L_0F00376C:
    // 0x0F00376C: bnel        $t2, $t0, L_0F003780
    if (ctx->r10 != ctx->r8) {
        // 0x0F003770: sh          $v1, 0x0($a0)
        MEM_H(0X0, ctx->r4) = ctx->r3;
            goto L_0F003780;
    }
    goto skip_6;
    // 0x0F003770: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
    skip_6:
    // 0x0F003774: b           L_0F003780
    // 0x0F003778: sh          $t3, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r11;
        goto L_0F003780;
    // 0x0F003778: sh          $t3, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r11;
    // 0x0F00377C: sh          $v1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r3;
L_0F003780:
    // 0x0F003780: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x0F003784: bne         $v0, $a1, L_0F0036B4
    if (ctx->r2 != ctx->r5) {
        // 0x0F003788: addiu       $a0, $a0, 0x2
        ctx->r4 = ADD32(ctx->r4, 0X2);
            goto L_0F0036B4;
    }
    // 0x0F003788: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
L_0F00378C:
    // 0x0F00378C: jr          $ra
    // 0x0F003790: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x0F003790: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
;}
RECOMP_FUNC void ni_ovl_124_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0x8B0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X8B0);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_124_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00008C: addiu       $a2, $zero, 0x2C
    ctx->r6 = ADD32(0, 0X2C);
    // 0x0F000090: jalr        $t9
    // 0x0F000094: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000094: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    after_0:
    // 0x0F000098: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00009C: addiu       $t9, $t9, 0x2410
    ctx->r25 = ADD32(ctx->r25, 0X2410);
    // 0x0F0000A0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0000A4: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    // 0x0F0000A8: jalr        $t9
    // 0x0F0000AC: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000AC: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    after_1:
    // 0x0F0000B0: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x0F0000B4: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F0000B8: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F0000BC: sw          $v0, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r2;
    // 0x0F0000C0: lw          $t6, 0x2908($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X2908);
    // 0x0F0000C4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000C8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0000CC: ori         $t7, $t6, 0x40
    ctx->r15 = ctx->r14 | 0X40;
    // 0x0F0000D0: ori         $t0, $t7, 0x8
    ctx->r8 = ctx->r15 | 0X8;
    // 0x0F0000D4: sw          $t7, 0x2908($a2)
    MEM_W(0X2908, ctx->r6) = ctx->r15;
    // 0x0F0000D8: sw          $t0, 0x2908($a2)
    MEM_W(0X2908, ctx->r6) = ctx->r8;
    // 0x0F0000DC: ori         $t2, $t0, 0x10
    ctx->r10 = ctx->r8 | 0X10;
    // 0x0F0000E0: sw          $t2, 0x2908($a2)
    MEM_W(0X2908, ctx->r6) = ctx->r10;
    // 0x0F0000E4: lw          $t3, 0x34($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X34);
    // 0x0F0000E8: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0000EC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F0000F0: sw          $t3, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r11;
    // 0x0F0000F4: lw          $t4, 0x24($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X24);
    // 0x0F0000F8: sw          $t4, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r12;
    // 0x0F0000FC: lwc1        $f4, 0x64($t3)
    ctx->f4.u32l = MEM_W(ctx->r11, 0X64);
    // 0x0F000100: swc1        $f4, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->f4.u32l;
    // 0x0F000104: lwc1        $f6, 0x68($t3)
    ctx->f6.u32l = MEM_W(ctx->r11, 0X68);
    // 0x0F000108: swc1        $f6, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->f6.u32l;
    // 0x0F00010C: lwc1        $f8, 0x6C($t3)
    ctx->f8.u32l = MEM_W(ctx->r11, 0X6C);
    // 0x0F000110: jalr        $t9
    // 0x0F000114: swc1        $f8, 0x10($a3)
    MEM_W(0X10, ctx->r7) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000114: swc1        $f8, 0x10($a3)
    MEM_W(0X10, ctx->r7) = ctx->f8.u32l;
    after_2:
    // 0x0F000118: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x0F00011C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000120: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x0F000124: jr          $ra
    // 0x0F000128: nop

    return;
    // 0x0F000128: nop

;}
RECOMP_FUNC void ni_ovl_124_func_0F00012C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F00012C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x0F000130: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000134: lw          $v0, 0x38($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X38);
    // 0x0F000138: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x0F00013C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F000140: sw          $zero, 0x28($v0)
    MEM_W(0X28, ctx->r2) = 0;
    // 0x0F000144: lh          $t6, -0x7D0C($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X7D0C);
    // 0x0F000148: lw          $a1, 0x40($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X40);
    // 0x0F00014C: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    // 0x0F000150: beq         $t6, $zero, L_0F0001E4
    if (ctx->r14 == 0) {
        // 0x0F000154: lw          $v1, 0x34($a1)
        ctx->r3 = MEM_W(ctx->r5, 0X34);
            goto L_0F0001E4;
    }
    // 0x0F000154: lw          $v1, 0x34($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X34);
    // 0x0F000158: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00015C: addiu       $t9, $t9, 0x2560
    ctx->r25 = ADD32(ctx->r25, 0X2560);
    // 0x0F000160: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    // 0x0F000164: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x0F000168: jalr        $t9
    // 0x0F00016C: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00016C: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x0F000170: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000174: beq         $v0, $zero, L_0F000240
    if (ctx->r2 == 0) {
        // 0x0F000178: lw          $a2, 0x28($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X28);
            goto L_0F000240;
    }
    // 0x0F000178: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x0F00017C: lhu         $t7, 0x1A($v1)
    ctx->r15 = MEM_HU(ctx->r3, 0X1A);
    // 0x0F000180: addiu       $t8, $zero, 0x262
    ctx->r24 = ADD32(0, 0X262);
    // 0x0F000184: addiu       $t1, $zero, 0x1F8
    ctx->r9 = ADD32(0, 0X1F8);
    // 0x0F000188: ori         $t0, $t7, 0x2
    ctx->r8 = ctx->r15 | 0X2;
    // 0x0F00018C: ori         $t2, $t0, 0x8
    ctx->r10 = ctx->r8 | 0X8;
    // 0x0F000190: ori         $t4, $t2, 0x80
    ctx->r12 = ctx->r10 | 0X80;
    // 0x0F000194: sh          $t0, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r8;
    // 0x0F000198: sh          $t2, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r10;
    // 0x0F00019C: ori         $t6, $t4, 0x100
    ctx->r14 = ctx->r12 | 0X100;
    // 0x0F0001A0: ori         $t2, $t6, 0x800
    ctx->r10 = ctx->r14 | 0X800;
    // 0x0F0001A4: sh          $t4, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r12;
    // 0x0F0001A8: sh          $t6, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r14;
    // 0x0F0001AC: sh          $t2, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r10;
    // 0x0F0001B0: addiu       $t4, $zero, 0xC8
    ctx->r12 = ADD32(0, 0XC8);
    // 0x0F0001B4: ori         $t3, $t2, 0x40
    ctx->r11 = ctx->r10 | 0X40;
    // 0x0F0001B8: addiu       $t0, $zero, 0x18
    ctx->r8 = ADD32(0, 0X18);
    // 0x0F0001BC: addiu       $t7, $zero, 0x30
    ctx->r15 = ADD32(0, 0X30);
    // 0x0F0001C0: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x0F0001C4: sh          $t5, 0x18($v1)
    MEM_H(0X18, ctx->r3) = ctx->r13;
    // 0x0F0001C8: sh          $t7, 0x1C($v1)
    MEM_H(0X1C, ctx->r3) = ctx->r15;
    // 0x0F0001CC: sh          $t0, 0x20($v1)
    MEM_H(0X20, ctx->r3) = ctx->r8;
    // 0x0F0001D0: sh          $t3, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r11;
    // 0x0F0001D4: sh          $t4, 0x32($v1)
    MEM_H(0X32, ctx->r3) = ctx->r12;
    // 0x0F0001D8: sh          $t1, 0x24($v1)
    MEM_H(0X24, ctx->r3) = ctx->r9;
    // 0x0F0001DC: b           L_0F000240
    // 0x0F0001E0: sh          $t8, 0x1E($v1)
    MEM_H(0X1E, ctx->r3) = ctx->r24;
        goto L_0F000240;
    // 0x0F0001E0: sh          $t8, 0x1E($v1)
    MEM_H(0X1E, ctx->r3) = ctx->r24;
L_0F0001E4:
    // 0x0F0001E4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0001E8: addiu       $t9, $t9, 0x2560
    ctx->r25 = ADD32(ctx->r25, 0X2560);
    // 0x0F0001EC: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x0F0001F0: jalr        $t9
    // 0x0F0001F4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0001F4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_1:
    // 0x0F0001F8: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x0F0001FC: beq         $v0, $zero, L_0F000240
    if (ctx->r2 == 0) {
        // 0x0F000200: lw          $a2, 0x28($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X28);
            goto L_0F000240;
    }
    // 0x0F000200: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x0F000204: lhu         $t6, 0x1A($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X1A);
    // 0x0F000208: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x0F00020C: sh          $t7, 0x18($v1)
    MEM_H(0X18, ctx->r3) = ctx->r15;
    // 0x0F000210: ori         $t8, $t6, 0x2
    ctx->r24 = ctx->r14 | 0X2;
    // 0x0F000214: ori         $t1, $t8, 0x80
    ctx->r9 = ctx->r24 | 0X80;
    // 0x0F000218: ori         $t3, $t1, 0x100
    ctx->r11 = ctx->r9 | 0X100;
    // 0x0F00021C: sh          $t8, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r24;
    // 0x0F000220: sh          $t1, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r9;
    // 0x0F000224: ori         $t5, $t3, 0x800
    ctx->r13 = ctx->r11 | 0X800;
    // 0x0F000228: sh          $t3, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r11;
    // 0x0F00022C: sh          $t5, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r13;
    // 0x0F000230: ori         $t9, $t5, 0x40
    ctx->r25 = ctx->r13 | 0X40;
    // 0x0F000234: addiu       $t6, $zero, 0xC8
    ctx->r14 = ADD32(0, 0XC8);
    // 0x0F000238: sh          $t6, 0x32($v1)
    MEM_H(0X32, ctx->r3) = ctx->r14;
    // 0x0F00023C: sh          $t9, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r25;
L_0F000240:
    // 0x0F000240: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000244: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000248: sw          $zero, 0x3C($a2)
    MEM_W(0X3C, ctx->r6) = 0;
    // 0x0F00024C: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F000250: jalr        $t9
    // 0x0F000254: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000254: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_2:
    // 0x0F000258: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F00025C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x0F000260: jr          $ra
    // 0x0F000264: nop

    return;
    // 0x0F000264: nop

;}
RECOMP_FUNC void ni_ovl_124_func_0F000268(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000268: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F00026C: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x0F000270: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x0F000274: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x0F000278: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F00027C: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F000280: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000284: lw          $s3, 0x38($a0)
    ctx->r19 = MEM_W(ctx->r4, 0X38);
    // 0x0F000288: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x0F00028C: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x0F000290: lwc1        $f6, 0xC($s3)
    ctx->f6.u32l = MEM_W(ctx->r19, 0XC);
    // 0x0F000294: lw          $v1, 0x4($s3)
    ctx->r3 = MEM_W(ctx->r19, 0X4);
    // 0x0F000298: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x0F00029C: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x0F0002A0: lwc1        $f4, 0x54($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X54);
    // 0x0F0002A4: add.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d + ctx->f10.d;
    // 0x0F0002A8: or          $s4, $a0, $zero
    ctx->r20 = ctx->r4 | 0;
    // 0x0F0002AC: cvt.d.s     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f0.d = CVT_D_S(ctx->f4.fl);
    // 0x0F0002B0: c.le.d      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.d <= ctx->f16.d;
    // 0x0F0002B4: nop

    // 0x0F0002B8: bc1fl       L_0F0002EC
    if (!c1cs) {
        // 0x0F0002BC: lw          $a0, 0x28($s3)
        ctx->r4 = MEM_W(ctx->r19, 0X28);
            goto L_0F0002EC;
    }
    goto skip_0;
    // 0x0F0002BC: lw          $a0, 0x28($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X28);
    skip_0:
    // 0x0F0002C0: lw          $t6, 0x28($s3)
    ctx->r14 = MEM_W(ctx->r19, 0X28);
    // 0x0F0002C4: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0002C8: bnel        $t6, $zero, L_0F0002EC
    if (ctx->r14 != 0) {
        // 0x0F0002CC: lw          $a0, 0x28($s3)
        ctx->r4 = MEM_W(ctx->r19, 0X28);
            goto L_0F0002EC;
    }
    goto skip_1;
    // 0x0F0002CC: lw          $a0, 0x28($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X28);
    skip_1:
    // 0x0F0002D0: ldc1        $f18, 0x8D0($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0X8D0);
    // 0x0F0002D4: add.d       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f0.d + ctx->f18.d;
    // 0x0F0002D8: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F0002DC: swc1        $f6, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f6.u32l;
    // 0x0F0002E0: b           L_0F00030C
    // 0x0F0002E4: lw          $a0, 0x28($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X28);
        goto L_0F00030C;
    // 0x0F0002E4: lw          $a0, 0x28($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X28);
    // 0x0F0002E8: lw          $a0, 0x28($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X28);
L_0F0002EC:
    // 0x0F0002EC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0002F0: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x0F0002F4: bne         $a0, $zero, L_0F00030C
    if (ctx->r4 != 0) {
        // 0x0F0002F8: addiu       $t9, $t9, 0x66F8
        ctx->r25 = ADD32(ctx->r25, 0X66F8);
            goto L_0F00030C;
    }
    // 0x0F0002F8: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F0002FC: sw          $t7, 0x28($s3)
    MEM_W(0X28, ctx->r19) = ctx->r15;
    // 0x0F000300: jalr        $t9
    // 0x0F000304: addiu       $a0, $zero, 0x189
    ctx->r4 = ADD32(0, 0X189);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000304: addiu       $a0, $zero, 0x189
    ctx->r4 = ADD32(0, 0X189);
    after_0:
    // 0x0F000308: lw          $a0, 0x28($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X28);
L_0F00030C:
    // 0x0F00030C: blez        $a0, L_0F00037C
    if (SIGNED(ctx->r4) <= 0) {
        // 0x0F000310: slti        $at, $a0, 0x4
        ctx->r1 = SIGNED(ctx->r4) < 0X4 ? 1 : 0;
            goto L_0F00037C;
    }
    // 0x0F000310: slti        $at, $a0, 0x4
    ctx->r1 = SIGNED(ctx->r4) < 0X4 ? 1 : 0;
    // 0x0F000314: beq         $at, $zero, L_0F00037C
    if (ctx->r1 == 0) {
        // 0x0F000318: addiu       $a1, $zero, 0x4A
        ctx->r5 = ADD32(0, 0X4A);
            goto L_0F00037C;
    }
    // 0x0F000318: addiu       $a1, $zero, 0x4A
    ctx->r5 = ADD32(0, 0X4A);
    // 0x0F00031C: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x0F000320: addiu       $t9, $t9, -0x6BBC
    ctx->r25 = ADD32(ctx->r25, -0X6BBC);
    // 0x0F000324: jalr        $t9
    // 0x0F000328: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000328: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_1:
    // 0x0F00032C: lw          $t8, 0x28($s3)
    ctx->r24 = MEM_W(ctx->r19, 0X28);
    // 0x0F000330: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x0F000334: addiu       $t9, $t9, -0xA1C
    ctx->r25 = ADD32(ctx->r25, -0XA1C);
    // 0x0F000338: sll         $t1, $t8, 2
    ctx->r9 = S32(ctx->r24 << 2);
    // 0x0F00033C: addu        $t2, $s3, $t1
    ctx->r10 = ADD32(ctx->r19, ctx->r9);
    // 0x0F000340: sw          $v0, 0x10($t2)
    MEM_W(0X10, ctx->r10) = ctx->r2;
    // 0x0F000344: lw          $v1, 0x4($s3)
    ctx->r3 = MEM_W(ctx->r19, 0X4);
    // 0x0F000348: lw          $t3, 0x28($s3)
    ctx->r11 = MEM_W(ctx->r19, 0X28);
    // 0x0F00034C: ori         $t6, $zero, 0x8000
    ctx->r14 = 0 | 0X8000;
    // 0x0F000350: lw          $a1, 0x50($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X50);
    // 0x0F000354: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x0F000358: addu        $t5, $s3, $t4
    ctx->r13 = ADD32(ctx->r19, ctx->r12);
    // 0x0F00035C: lw          $a0, 0x10($t5)
    ctx->r4 = MEM_W(ctx->r13, 0X10);
    // 0x0F000360: lw          $a2, 0x54($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X54);
    // 0x0F000364: lw          $a3, 0x58($v1)
    ctx->r7 = MEM_W(ctx->r3, 0X58);
    // 0x0F000368: jalr        $t9
    // 0x0F00036C: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00036C: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    after_2:
    // 0x0F000370: lw          $t7, 0x28($s3)
    ctx->r15 = MEM_W(ctx->r19, 0X28);
    // 0x0F000374: addiu       $a0, $t7, 0x1
    ctx->r4 = ADD32(ctx->r15, 0X1);
    // 0x0F000378: sw          $a0, 0x28($s3)
    MEM_W(0X28, ctx->r19) = ctx->r4;
L_0F00037C:
    // 0x0F00037C: slti        $at, $a0, 0x4
    ctx->r1 = SIGNED(ctx->r4) < 0X4 ? 1 : 0;
    // 0x0F000380: bne         $at, $zero, L_0F000694
    if (ctx->r1 != 0) {
        // 0x0F000384: lui         $t0, 0x801D
        ctx->r8 = S32(0X801D << 16);
            goto L_0F000694;
    }
    // 0x0F000384: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x0F000388: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x0F00038C: lui         $at, 0x4020
    ctx->r1 = S32(0X4020 << 16);
    // 0x0F000390: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000394: lwc1        $f8, 0x8($t0)
    ctx->f8.u32l = MEM_W(ctx->r8, 0X8);
    // 0x0F000398: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x0F00039C: lw          $v0, 0x3C($s4)
    ctx->r2 = MEM_W(ctx->r20, 0X3C);
    // 0x0F0003A0: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x0F0003A4: or          $s1, $s3, $zero
    ctx->r17 = ctx->r19 | 0;
    // 0x0F0003A8: lui         $s2, 0x8006
    ctx->r18 = S32(0X8006 << 16);
    // 0x0F0003AC: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0003B0: addiu       $s2, $s2, -0x2FBC
    ctx->r18 = ADD32(ctx->r18, -0X2FBC);
    // 0x0F0003B4: addiu       $s3, $zero, 0xC
    ctx->r19 = ADD32(0, 0XC);
    // 0x0F0003B8: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x0F0003BC: cfc1        $t1, $FpcCsr
    ctx->r9 = get_cop1_cs();
    // 0x0F0003C0: ctc1        $t2, $FpcCsr
    set_cop1_cs(ctx->r10);
    // 0x0F0003C4: addiu       $t3, $v0, 0x1
    ctx->r11 = ADD32(ctx->r2, 0X1);
    // 0x0F0003C8: cvt.w.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = CVT_W_S(ctx->f16.fl);
    // 0x0F0003CC: cfc1        $t2, $FpcCsr
    ctx->r10 = get_cop1_cs();
    // 0x0F0003D0: nop

    // 0x0F0003D4: andi        $t2, $t2, 0x78
    ctx->r10 = ctx->r10 & 0X78;
    // 0x0F0003D8: beql        $t2, $zero, L_0F000428
    if (ctx->r10 == 0) {
        // 0x0F0003DC: mfc1        $t2, $f18
        ctx->r10 = (int32_t)ctx->f18.u32l;
            goto L_0F000428;
    }
    goto skip_2;
    // 0x0F0003DC: mfc1        $t2, $f18
    ctx->r10 = (int32_t)ctx->f18.u32l;
    skip_2:
    // 0x0F0003E0: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F0003E4: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x0F0003E8: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x0F0003EC: ctc1        $t2, $FpcCsr
    set_cop1_cs(ctx->r10);
    // 0x0F0003F0: nop

    // 0x0F0003F4: cvt.w.s     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.u32l = CVT_W_S(ctx->f18.fl);
    // 0x0F0003F8: cfc1        $t2, $FpcCsr
    ctx->r10 = get_cop1_cs();
    // 0x0F0003FC: nop

    // 0x0F000400: andi        $t2, $t2, 0x78
    ctx->r10 = ctx->r10 & 0X78;
    // 0x0F000404: bne         $t2, $zero, L_0F00041C
    if (ctx->r10 != 0) {
        // 0x0F000408: nop
    
            goto L_0F00041C;
    }
    // 0x0F000408: nop

    // 0x0F00040C: mfc1        $t2, $f18
    ctx->r10 = (int32_t)ctx->f18.u32l;
    // 0x0F000410: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F000414: b           L_0F000434
    // 0x0F000418: or          $t2, $t2, $at
    ctx->r10 = ctx->r10 | ctx->r1;
        goto L_0F000434;
    // 0x0F000418: or          $t2, $t2, $at
    ctx->r10 = ctx->r10 | ctx->r1;
L_0F00041C:
    // 0x0F00041C: b           L_0F000434
    // 0x0F000420: addiu       $t2, $zero, -0x1
    ctx->r10 = ADD32(0, -0X1);
        goto L_0F000434;
    // 0x0F000420: addiu       $t2, $zero, -0x1
    ctx->r10 = ADD32(0, -0X1);
    // 0x0F000424: mfc1        $t2, $f18
    ctx->r10 = (int32_t)ctx->f18.u32l;
L_0F000428:
    // 0x0F000428: nop

    // 0x0F00042C: bltz        $t2, L_0F00041C
    if (SIGNED(ctx->r10) < 0) {
        // 0x0F000430: nop
    
            goto L_0F00041C;
    }
    // 0x0F000430: nop

L_0F000434:
    // 0x0F000434: ctc1        $t1, $FpcCsr
    set_cop1_cs(ctx->r9);
    // 0x0F000438: sltu        $v1, $t2, $v0
    ctx->r3 = ctx->r10 < ctx->r2 ? 1 : 0;
    // 0x0F00043C: beq         $v1, $zero, L_0F000458
    if (ctx->r3 == 0) {
        // 0x0F000440: sw          $t3, 0x3C($s4)
        MEM_W(0X3C, ctx->r20) = ctx->r11;
            goto L_0F000458;
    }
    // 0x0F000440: sw          $t3, 0x3C($s4)
    MEM_W(0X3C, ctx->r20) = ctx->r11;
    // 0x0F000444: lw          $v1, 0x40($s4)
    ctx->r3 = MEM_W(ctx->r20, 0X40);
    // 0x0F000448: lw          $v0, 0x34($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X34);
    // 0x0F00044C: lhu         $t4, 0x1A($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0X1A);
    // 0x0F000450: andi        $t5, $t4, 0xFF7F
    ctx->r13 = ctx->r12 & 0XFF7F;
    // 0x0F000454: sh          $t5, 0x1A($v0)
    MEM_H(0X1A, ctx->r2) = ctx->r13;
L_0F000458:
    // 0x0F000458: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x0F00045C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000460: lwc1        $f4, 0x8($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0X8);
    // 0x0F000464: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x0F000468: lw          $v0, 0x3C($s4)
    ctx->r2 = MEM_W(ctx->r20, 0X3C);
    // 0x0F00046C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F000470: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000474: addiu       $t7, $v0, 0x1
    ctx->r15 = ADD32(ctx->r2, 0X1);
    // 0x0F000478: cfc1        $t6, $FpcCsr
    ctx->r14 = get_cop1_cs();
    // 0x0F00047C: ctc1        $t9, $FpcCsr
    set_cop1_cs(ctx->r25);
    // 0x0F000480: nop

    // 0x0F000484: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F000488: cfc1        $t9, $FpcCsr
    ctx->r25 = get_cop1_cs();
    // 0x0F00048C: nop

    // 0x0F000490: andi        $t9, $t9, 0x78
    ctx->r25 = ctx->r25 & 0X78;
    // 0x0F000494: beql        $t9, $zero, L_0F0004E4
    if (ctx->r25 == 0) {
        // 0x0F000498: mfc1        $t9, $f10
        ctx->r25 = (int32_t)ctx->f10.u32l;
            goto L_0F0004E4;
    }
    goto skip_3;
    // 0x0F000498: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    skip_3:
    // 0x0F00049C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0004A0: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x0F0004A4: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F0004A8: ctc1        $t9, $FpcCsr
    set_cop1_cs(ctx->r25);
    // 0x0F0004AC: nop

    // 0x0F0004B0: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F0004B4: cfc1        $t9, $FpcCsr
    ctx->r25 = get_cop1_cs();
    // 0x0F0004B8: nop

    // 0x0F0004BC: andi        $t9, $t9, 0x78
    ctx->r25 = ctx->r25 & 0X78;
    // 0x0F0004C0: bne         $t9, $zero, L_0F0004D8
    if (ctx->r25 != 0) {
        // 0x0F0004C4: nop
    
            goto L_0F0004D8;
    }
    // 0x0F0004C4: nop

    // 0x0F0004C8: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    // 0x0F0004CC: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0004D0: b           L_0F0004F0
    // 0x0F0004D4: or          $t9, $t9, $at
    ctx->r25 = ctx->r25 | ctx->r1;
        goto L_0F0004F0;
    // 0x0F0004D4: or          $t9, $t9, $at
    ctx->r25 = ctx->r25 | ctx->r1;
L_0F0004D8:
    // 0x0F0004D8: b           L_0F0004F0
    // 0x0F0004DC: addiu       $t9, $zero, -0x1
    ctx->r25 = ADD32(0, -0X1);
        goto L_0F0004F0;
    // 0x0F0004DC: addiu       $t9, $zero, -0x1
    ctx->r25 = ADD32(0, -0X1);
    // 0x0F0004E0: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
L_0F0004E4:
    // 0x0F0004E4: nop

    // 0x0F0004E8: bltz        $t9, L_0F0004D8
    if (SIGNED(ctx->r25) < 0) {
        // 0x0F0004EC: nop
    
            goto L_0F0004D8;
    }
    // 0x0F0004EC: nop

L_0F0004F0:
    // 0x0F0004F0: ctc1        $t6, $FpcCsr
    set_cop1_cs(ctx->r14);
    // 0x0F0004F4: sltu        $v1, $t9, $v0
    ctx->r3 = ctx->r25 < ctx->r2 ? 1 : 0;
    // 0x0F0004F8: beq         $v1, $zero, L_0F000524
    if (ctx->r3 == 0) {
        // 0x0F0004FC: sw          $t7, 0x3C($s4)
        MEM_W(0X3C, ctx->r20) = ctx->r15;
            goto L_0F000524;
    }
    // 0x0F0004FC: sw          $t7, 0x3C($s4)
    MEM_W(0X3C, ctx->r20) = ctx->r15;
L_0F000500:
    // 0x0F000500: lw          $a0, 0x14($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X14);
    // 0x0F000504: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    // 0x0F000508: jalr        $s2
    // 0x0F00050C: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_3;
    // 0x0F00050C: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    after_3:
    // 0x0F000510: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x0F000514: bne         $s0, $s3, L_0F000500
    if (ctx->r16 != ctx->r19) {
        // 0x0F000518: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_0F000500;
    }
    // 0x0F000518: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x0F00051C: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x0F000520: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
L_0F000524:
    // 0x0F000524: lui         $at, 0x4090
    ctx->r1 = S32(0X4090 << 16);
    // 0x0F000528: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F00052C: lwc1        $f16, 0x8($t0)
    ctx->f16.u32l = MEM_W(ctx->r8, 0X8);
    // 0x0F000530: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x0F000534: lw          $v0, 0x3C($s4)
    ctx->r2 = MEM_W(ctx->r20, 0X3C);
    // 0x0F000538: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x0F00053C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000540: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000544: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x0F000548: addiu       $a2, $zero, 0xFF
    ctx->r6 = ADD32(0, 0XFF);
    // 0x0F00054C: addiu       $a3, $zero, 0xFF
    ctx->r7 = ADD32(0, 0XFF);
    // 0x0F000550: addiu       $t4, $zero, 0xFF
    ctx->r12 = ADD32(0, 0XFF);
    // 0x0F000554: cfc1        $t8, $FpcCsr
    ctx->r24 = get_cop1_cs();
    // 0x0F000558: ctc1        $t1, $FpcCsr
    set_cop1_cs(ctx->r9);
    // 0x0F00055C: addiu       $t2, $v0, 0x1
    ctx->r10 = ADD32(ctx->r2, 0X1);
    // 0x0F000560: cvt.w.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = CVT_W_S(ctx->f4.fl);
    // 0x0F000564: cfc1        $t1, $FpcCsr
    ctx->r9 = get_cop1_cs();
    // 0x0F000568: nop

    // 0x0F00056C: andi        $t1, $t1, 0x78
    ctx->r9 = ctx->r9 & 0X78;
    // 0x0F000570: beql        $t1, $zero, L_0F0005C0
    if (ctx->r9 == 0) {
        // 0x0F000574: mfc1        $t1, $f6
        ctx->r9 = (int32_t)ctx->f6.u32l;
            goto L_0F0005C0;
    }
    goto skip_4;
    // 0x0F000574: mfc1        $t1, $f6
    ctx->r9 = (int32_t)ctx->f6.u32l;
    skip_4:
    // 0x0F000578: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F00057C: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x0F000580: sub.s       $f6, $f4, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x0F000584: ctc1        $t1, $FpcCsr
    set_cop1_cs(ctx->r9);
    // 0x0F000588: nop

    // 0x0F00058C: cvt.w.s     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.u32l = CVT_W_S(ctx->f6.fl);
    // 0x0F000590: cfc1        $t1, $FpcCsr
    ctx->r9 = get_cop1_cs();
    // 0x0F000594: nop

    // 0x0F000598: andi        $t1, $t1, 0x78
    ctx->r9 = ctx->r9 & 0X78;
    // 0x0F00059C: bne         $t1, $zero, L_0F0005B4
    if (ctx->r9 != 0) {
        // 0x0F0005A0: nop
    
            goto L_0F0005B4;
    }
    // 0x0F0005A0: nop

    // 0x0F0005A4: mfc1        $t1, $f6
    ctx->r9 = (int32_t)ctx->f6.u32l;
    // 0x0F0005A8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0005AC: b           L_0F0005CC
    // 0x0F0005B0: or          $t1, $t1, $at
    ctx->r9 = ctx->r9 | ctx->r1;
        goto L_0F0005CC;
    // 0x0F0005B0: or          $t1, $t1, $at
    ctx->r9 = ctx->r9 | ctx->r1;
L_0F0005B4:
    // 0x0F0005B4: b           L_0F0005CC
    // 0x0F0005B8: addiu       $t1, $zero, -0x1
    ctx->r9 = ADD32(0, -0X1);
        goto L_0F0005CC;
    // 0x0F0005B8: addiu       $t1, $zero, -0x1
    ctx->r9 = ADD32(0, -0X1);
    // 0x0F0005BC: mfc1        $t1, $f6
    ctx->r9 = (int32_t)ctx->f6.u32l;
L_0F0005C0:
    // 0x0F0005C0: nop

    // 0x0F0005C4: bltz        $t1, L_0F0005B4
    if (SIGNED(ctx->r9) < 0) {
        // 0x0F0005C8: nop
    
            goto L_0F0005B4;
    }
    // 0x0F0005C8: nop

L_0F0005CC:
    // 0x0F0005CC: ctc1        $t8, $FpcCsr
    set_cop1_cs(ctx->r24);
    // 0x0F0005D0: sltu        $v1, $t1, $v0
    ctx->r3 = ctx->r9 < ctx->r2 ? 1 : 0;
    // 0x0F0005D4: beq         $v1, $zero, L_0F000694
    if (ctx->r3 == 0) {
        // 0x0F0005D8: sw          $t2, 0x3C($s4)
        MEM_W(0X3C, ctx->r20) = ctx->r10;
            goto L_0F000694;
    }
    // 0x0F0005D8: sw          $t2, 0x3C($s4)
    MEM_W(0X3C, ctx->r20) = ctx->r10;
    // 0x0F0005DC: lui         $at, 0x3F00
    ctx->r1 = S32(0X3F00 << 16);
    // 0x0F0005E0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0005E4: lwc1        $f8, 0x8($t0)
    ctx->f8.u32l = MEM_W(ctx->r8, 0X8);
    // 0x0F0005E8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F0005EC: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0005F0: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x0F0005F4: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x0F0005F8: cfc1        $t3, $FpcCsr
    ctx->r11 = get_cop1_cs();
    // 0x0F0005FC: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x0F000600: nop

    // 0x0F000604: cvt.w.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = CVT_W_S(ctx->f16.fl);
    // 0x0F000608: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x0F00060C: nop

    // 0x0F000610: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x0F000614: beql        $a1, $zero, L_0F000664
    if (ctx->r5 == 0) {
        // 0x0F000618: mfc1        $a1, $f18
        ctx->r5 = (int32_t)ctx->f18.u32l;
            goto L_0F000664;
    }
    goto skip_5;
    // 0x0F000618: mfc1        $a1, $f18
    ctx->r5 = (int32_t)ctx->f18.u32l;
    skip_5:
    // 0x0F00061C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000620: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000624: sub.s       $f18, $f16, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x0F000628: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x0F00062C: nop

    // 0x0F000630: cvt.w.s     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.u32l = CVT_W_S(ctx->f18.fl);
    // 0x0F000634: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x0F000638: nop

    // 0x0F00063C: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x0F000640: bne         $a1, $zero, L_0F000658
    if (ctx->r5 != 0) {
        // 0x0F000644: nop
    
            goto L_0F000658;
    }
    // 0x0F000644: nop

    // 0x0F000648: mfc1        $a1, $f18
    ctx->r5 = (int32_t)ctx->f18.u32l;
    // 0x0F00064C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F000650: b           L_0F000670
    // 0x0F000654: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
        goto L_0F000670;
    // 0x0F000654: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
L_0F000658:
    // 0x0F000658: b           L_0F000670
    // 0x0F00065C: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
        goto L_0F000670;
    // 0x0F00065C: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
    // 0x0F000660: mfc1        $a1, $f18
    ctx->r5 = (int32_t)ctx->f18.u32l;
L_0F000664:
    // 0x0F000664: nop

    // 0x0F000668: bltz        $a1, L_0F000658
    if (SIGNED(ctx->r5) < 0) {
        // 0x0F00066C: nop
    
            goto L_0F000658;
    }
    // 0x0F00066C: nop

L_0F000670:
    // 0x0F000670: ctc1        $t3, $FpcCsr
    set_cop1_cs(ctx->r11);
    // 0x0F000674: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x0F000678: jalr        $t9
    // 0x0F00067C: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F00067C: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    after_4:
    // 0x0F000680: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000684: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000688: addiu       $a0, $s4, 0x8
    ctx->r4 = ADD32(ctx->r20, 0X8);
    // 0x0F00068C: jalr        $t9
    // 0x0F000690: addiu       $a1, $s4, 0xE
    ctx->r5 = ADD32(ctx->r20, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000690: addiu       $a1, $s4, 0xE
    ctx->r5 = ADD32(ctx->r20, 0XE);
    after_5:
L_0F000694:
    // 0x0F000694: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x0F000698: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F00069C: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F0006A0: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F0006A4: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0006A8: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x0F0006AC: jr          $ra
    // 0x0F0006B0: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x0F0006B0: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void ni_ovl_124_func_0F0006B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0006B4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F0006B8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0006BC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F0006C0: addiu       $t9, $t9, 0x5BC
    ctx->r25 = ADD32(ctx->r25, 0X5BC);
    // 0x0F0006C4: jalr        $t9
    // 0x0F0006C8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0006C8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x0F0006CC: bne         $v0, $zero, L_0F00073C
    if (ctx->r2 != 0) {
        // 0x0F0006D0: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_0F00073C;
    }
    // 0x0F0006D0: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x0F0006D4: lw          $a2, 0x40($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X40);
    // 0x0F0006D8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x0F0006DC: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x0F0006E0: lw          $v1, 0x34($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X34);
    // 0x0F0006E4: addiu       $at, $zero, -0x41
    ctx->r1 = ADD32(0, -0X41);
    // 0x0F0006E8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0006EC: lhu         $t6, 0x1A($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X1A);
    // 0x0F0006F0: addiu       $t1, $zero, 0xA
    ctx->r9 = ADD32(0, 0XA);
    // 0x0F0006F4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0006F8: ori         $t8, $t6, 0x80
    ctx->r24 = ctx->r14 | 0X80;
    // 0x0F0006FC: ori         $t0, $t8, 0x1
    ctx->r8 = ctx->r24 | 0X1;
    // 0x0F000700: sh          $t8, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r24;
    // 0x0F000704: sh          $t0, 0x1A($v1)
    MEM_H(0X1A, ctx->r3) = ctx->r8;
    // 0x0F000708: lw          $t2, 0x2908($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X2908);
    // 0x0F00070C: sh          $t1, 0x2B4E($v0)
    MEM_H(0X2B4E, ctx->r2) = ctx->r9;
    // 0x0F000710: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x0F000714: and         $t3, $t2, $at
    ctx->r11 = ctx->r10 & ctx->r1;
    // 0x0F000718: addiu       $at, $zero, -0x9
    ctx->r1 = ADD32(0, -0X9);
    // 0x0F00071C: and         $t5, $t3, $at
    ctx->r13 = ctx->r11 & ctx->r1;
    // 0x0F000720: addiu       $at, $zero, -0x11
    ctx->r1 = ADD32(0, -0X11);
    // 0x0F000724: sw          $t3, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r11;
    // 0x0F000728: sw          $t5, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r13;
    // 0x0F00072C: and         $t6, $t5, $at
    ctx->r14 = ctx->r13 & ctx->r1;
    // 0x0F000730: sw          $t6, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r14;
    // 0x0F000734: jalr        $t9
    // 0x0F000738: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000738: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_1:
L_0F00073C:
    // 0x0F00073C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000740: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000744: jr          $ra
    // 0x0F000748: nop

    return;
    // 0x0F000748: nop

;}
RECOMP_FUNC void ni_ovl_124_func_0F00074C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F00074C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x0F000750: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x0F000754: lh          $t6, 0x2B4E($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X2B4E);
    // 0x0F000758: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F00075C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000760: bne         $t6, $zero, L_0F000898
    if (ctx->r14 != 0) {
        // 0x0F000764: lw          $a1, 0x38($a0)
        ctx->r5 = MEM_W(ctx->r4, 0X38);
            goto L_0F000898;
    }
    // 0x0F000764: lw          $a1, 0x38($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X38);
    // 0x0F000768: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00076C: lwc1        $f6, 0x8D8($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X8D8);
    // 0x0F000770: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x0F000774: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F000778: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F00077C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F000780: lw          $v1, 0x4($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X4);
    // 0x0F000784: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x0F000788: ctc1        $t8, $FpcCsr
    set_cop1_cs(ctx->r24);
    // 0x0F00078C: nop

    // 0x0F000790: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F000794: cfc1        $t8, $FpcCsr
    ctx->r24 = get_cop1_cs();
    // 0x0F000798: nop

    // 0x0F00079C: andi        $t8, $t8, 0x78
    ctx->r24 = ctx->r24 & 0X78;
    // 0x0F0007A0: beql        $t8, $zero, L_0F0007F0
    if (ctx->r24 == 0) {
        // 0x0F0007A4: mfc1        $t8, $f10
        ctx->r24 = (int32_t)ctx->f10.u32l;
            goto L_0F0007F0;
    }
    goto skip_0;
    // 0x0F0007A4: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    skip_0:
    // 0x0F0007A8: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0007AC: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F0007B0: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F0007B4: ctc1        $t8, $FpcCsr
    set_cop1_cs(ctx->r24);
    // 0x0F0007B8: nop

    // 0x0F0007BC: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F0007C0: cfc1        $t8, $FpcCsr
    ctx->r24 = get_cop1_cs();
    // 0x0F0007C4: nop

    // 0x0F0007C8: andi        $t8, $t8, 0x78
    ctx->r24 = ctx->r24 & 0X78;
    // 0x0F0007CC: bne         $t8, $zero, L_0F0007E4
    if (ctx->r24 != 0) {
        // 0x0F0007D0: nop
    
            goto L_0F0007E4;
    }
    // 0x0F0007D0: nop

    // 0x0F0007D4: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    // 0x0F0007D8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0007DC: b           L_0F0007FC
    // 0x0F0007E0: or          $t8, $t8, $at
    ctx->r24 = ctx->r24 | ctx->r1;
        goto L_0F0007FC;
    // 0x0F0007E0: or          $t8, $t8, $at
    ctx->r24 = ctx->r24 | ctx->r1;
L_0F0007E4:
    // 0x0F0007E4: b           L_0F0007FC
    // 0x0F0007E8: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
        goto L_0F0007FC;
    // 0x0F0007E8: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    // 0x0F0007EC: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
L_0F0007F0:
    // 0x0F0007F0: nop

    // 0x0F0007F4: bltz        $t8, L_0F0007E4
    if (SIGNED(ctx->r24) < 0) {
        // 0x0F0007F8: nop
    
            goto L_0F0007E4;
    }
    // 0x0F0007F8: nop

L_0F0007FC:
    // 0x0F0007FC: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x0F000800: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x0F000804: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F000808: bgez        $t8, L_0F000824
    if (SIGNED(ctx->r24) >= 0) {
        // 0x0F00080C: cvt.d.w     $f18, $f16
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
            goto L_0F000824;
    }
    // 0x0F00080C: cvt.d.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
    // 0x0F000810: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x0F000814: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x0F000818: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x0F00081C: nop

    // 0x0F000820: add.d       $f18, $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f18.d = ctx->f18.d + ctx->f4.d;
L_0F000824:
    // 0x0F000824: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x0F000828: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x0F00082C: lwc1        $f10, 0x54($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X54);
    // 0x0F000830: div.d       $f8, $f6, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f18.d); 
    ctx->f8.d = DIV_D(ctx->f6.d, ctx->f18.d);
    // 0x0F000834: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x0F000838: sub.d       $f4, $f16, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f8.d); 
    ctx->f4.d = ctx->f16.d - ctx->f8.d;
    // 0x0F00083C: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F000840: swc1        $f6, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f6.u32l;
    // 0x0F000844: lw          $v1, 0x4($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X4);
    // 0x0F000848: lwc1        $f0, 0xC($a1)
    ctx->f0.u32l = MEM_W(ctx->r5, 0XC);
    // 0x0F00084C: lwc1        $f18, 0x54($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X54);
    // 0x0F000850: c.lt.s      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.fl < ctx->f0.fl;
    // 0x0F000854: nop

    // 0x0F000858: bc1fl       L_0F00089C
    if (!c1cs) {
        // 0x0F00085C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F00089C;
    }
    goto skip_1;
    // 0x0F00085C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x0F000860: swc1        $f0, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f0.u32l;
    // 0x0F000864: lw          $t9, 0x2908($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X2908);
    // 0x0F000868: addiu       $at, $zero, -0x41
    ctx->r1 = ADD32(0, -0X41);
    // 0x0F00086C: and         $t0, $t9, $at
    ctx->r8 = ctx->r25 & ctx->r1;
    // 0x0F000870: addiu       $at, $zero, -0x9
    ctx->r1 = ADD32(0, -0X9);
    // 0x0F000874: and         $t2, $t0, $at
    ctx->r10 = ctx->r8 & ctx->r1;
    // 0x0F000878: addiu       $at, $zero, -0x11
    ctx->r1 = ADD32(0, -0X11);
    // 0x0F00087C: sw          $t0, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r8;
    // 0x0F000880: sw          $t2, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r10;
    // 0x0F000884: and         $t4, $t2, $at
    ctx->r12 = ctx->r10 & ctx->r1;
    // 0x0F000888: sw          $t4, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r12;
    // 0x0F00088C: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F000890: jalr        $t9
    // 0x0F000894: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000894: nop

    after_0:
L_0F000898:
    // 0x0F000898: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F00089C:
    // 0x0F00089C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0008A0: jr          $ra
    // 0x0F0008A4: nop

    return;
    // 0x0F0008A4: nop

;}
RECOMP_FUNC void ni_ovl_125_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0xB04($t9)
    ctx->r25 = MEM_W(ctx->r25, 0XB04);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_125_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00008C: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    // 0x0F000090: jalr        $t9
    // 0x0F000094: addiu       $a3, $zero, 0xA
    ctx->r7 = ADD32(0, 0XA);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000094: addiu       $a3, $zero, 0xA
    ctx->r7 = ADD32(0, 0XA);
    after_0:
    // 0x0F000098: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x0F00009C: lui         $a3, 0x8000
    ctx->r7 = S32(0X8000 << 16);
    // 0x0F0000A0: addiu       $a2, $a2, 0x27B8
    ctx->r6 = ADD32(ctx->r6, 0X27B8);
    // 0x0F0000A4: addiu       $a3, $a3, 0x572C
    ctx->r7 = ADD32(ctx->r7, 0X572C);
    // 0x0F0000A8: sw          $v0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r2;
    // 0x0F0000AC: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    // 0x0F0000B0: lw          $a0, 0x0($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X0);
    // 0x0F0000B4: jalr        $a3
    // 0x0F0000B8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    LOOKUP_FUNC(ctx->r7)(rdram, ctx);
        goto after_1;
    // 0x0F0000B8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_1:
    // 0x0F0000BC: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x0F0000C0: addiu       $a2, $a2, 0x27B8
    ctx->r6 = ADD32(ctx->r6, 0X27B8);
    // 0x0F0000C4: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x0F0000C8: lw          $t6, 0x0($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X0);
    // 0x0F0000CC: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x0F0000D0: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x0F0000D4: swc1        $f12, 0x50($t6)
    MEM_W(0X50, ctx->r14) = ctx->f12.u32l;
    // 0x0F0000D8: lw          $t7, 0x0($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X0);
    // 0x0F0000DC: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0000E0: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x0F0000E4: swc1        $f12, 0x54($t7)
    MEM_W(0X54, ctx->r15) = ctx->f12.u32l;
    // 0x0F0000E8: lw          $t8, 0x0($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X0);
    // 0x0F0000EC: addiu       $v0, $v0, 0x27BC
    ctx->r2 = ADD32(ctx->r2, 0X27BC);
    // 0x0F0000F0: addiu       $a1, $zero, 0x3
    ctx->r5 = ADD32(0, 0X3);
    // 0x0F0000F4: swc1        $f4, 0x58($t8)
    MEM_W(0X58, ctx->r24) = ctx->f4.u32l;
    // 0x0F0000F8: lw          $t0, 0x0($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X0);
    // 0x0F0000FC: swc1        $f12, 0x70($t0)
    MEM_W(0X70, ctx->r8) = ctx->f12.u32l;
    // 0x0F000100: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x0F000104: lwc1        $f0, 0x70($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X70);
    // 0x0F000108: swc1        $f0, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f0.u32l;
    // 0x0F00010C: lw          $t1, 0x0($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X0);
    // 0x0F000110: swc1        $f0, 0x68($t1)
    MEM_W(0X68, ctx->r9) = ctx->f0.u32l;
    // 0x0F000114: jalr        $a3
    // 0x0F000118: lw          $a0, 0x0($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X0);
    LOOKUP_FUNC(ctx->r7)(rdram, ctx);
        goto after_2;
    // 0x0F000118: lw          $a0, 0x0($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X0);
    after_2:
    // 0x0F00011C: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x0F000120: addiu       $v0, $v0, 0x27BC
    ctx->r2 = ADD32(ctx->r2, 0X27BC);
    // 0x0F000124: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x0F000128: lw          $t2, 0x0($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X0);
    // 0x0F00012C: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x0F000130: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000134: swc1        $f12, 0x50($t2)
    MEM_W(0X50, ctx->r10) = ctx->f12.u32l;
    // 0x0F000138: lw          $t3, 0x0($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X0);
    // 0x0F00013C: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F000140: addiu       $a2, $a2, 0xAD0
    ctx->r6 = ADD32(ctx->r6, 0XAD0);
    // 0x0F000144: swc1        $f12, 0x54($t3)
    MEM_W(0X54, ctx->r11) = ctx->f12.u32l;
    // 0x0F000148: lw          $t4, 0x0($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X0);
    // 0x0F00014C: addiu       $a0, $zero, 0x210
    ctx->r4 = ADD32(0, 0X210);
    // 0x0F000150: swc1        $f6, 0x58($t4)
    MEM_W(0X58, ctx->r12) = ctx->f6.u32l;
    // 0x0F000154: lw          $t5, 0x0($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X0);
    // 0x0F000158: swc1        $f12, 0x70($t5)
    MEM_W(0X70, ctx->r13) = ctx->f12.u32l;
    // 0x0F00015C: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x0F000160: lwc1        $f0, 0x70($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X70);
    // 0x0F000164: swc1        $f0, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f0.u32l;
    // 0x0F000168: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X0);
    // 0x0F00016C: swc1        $f0, 0x68($t9)
    MEM_W(0X68, ctx->r25) = ctx->f0.u32l;
    // 0x0F000170: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000174: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x0F000178: jalr        $t9
    // 0x0F00017C: lw          $a1, 0x0($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F00017C: lw          $a1, 0x0($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X0);
    after_3:
    // 0x0F000180: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000184: ldc1        $f0, 0xB10($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0XB10);
    // 0x0F000188: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00018C: ldc1        $f2, 0xB18($at)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r1, 0XB18);
    // 0x0F000190: sw          $v0, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->r2;
    // 0x0F000194: sw          $v0, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r2;
    // 0x0F000198: lui         $at, 0x4034
    ctx->r1 = S32(0X4034 << 16);
    // 0x0F00019C: mtc1        $at, $f15
    ctx->f_odd[(15 - 1) * 2] = ctx->r1;
    // 0x0F0001A0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0001A4: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F0001A8: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x0F0001AC: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0001B0: lui         $at, 0xC392
    ctx->r1 = S32(0XC392 << 16);
    // 0x0F0001B4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0001B8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x0F0001BC: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    // 0x0F0001C0: ori         $t7, $t6, 0x880
    ctx->r15 = ctx->r14 | 0X880;
    // 0x0F0001C4: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x0F0001C8: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x0F0001CC: sw          $t8, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r24;
    // 0x0F0001D0: swc1        $f16, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f16.u32l;
    // 0x0F0001D4: swc1        $f16, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f16.u32l;
    // 0x0F0001D8: swc1        $f16, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f16.u32l;
    // 0x0F0001DC: swc1        $f12, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f12.u32l;
    // 0x0F0001E0: swc1        $f12, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f12.u32l;
    // 0x0F0001E4: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    // 0x0F0001E8: lwc1        $f10, 0xC($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0XC);
    // 0x0F0001EC: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x0F0001F0: lui         $at, 0x402E
    ctx->r1 = S32(0X402E << 16);
    // 0x0F0001F4: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x0F0001F8: addiu       $a1, $zero, 0x3
    ctx->r5 = ADD32(0, 0X3);
    // 0x0F0001FC: mul.d       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f0.d);
    // 0x0F000200: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000204: trunc.w.d   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_D(ctx->f4.d);
    // 0x0F000208: mfc1        $t1, $f6
    ctx->r9 = (int32_t)ctx->f6.u32l;
    // 0x0F00020C: nop

    // 0x0F000210: sll         $t2, $t1, 16
    ctx->r10 = S32(ctx->r9 << 16);
    // 0x0F000214: sra         $t3, $t2, 16
    ctx->r11 = S32(SIGNED(ctx->r10) >> 16);
    // 0x0F000218: negu        $t4, $t3
    ctx->r12 = SUB32(0, ctx->r11);
    // 0x0F00021C: mtc1        $t4, $f8
    ctx->f8.u32l = ctx->r12;
    // 0x0F000220: nop

    // 0x0F000224: cvt.d.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.d = CVT_D_W(ctx->f8.u32l);
    // 0x0F000228: mul.d       $f18, $f10, $f14
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f14.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f14.d);
    // 0x0F00022C: nop

    // 0x0F000230: mul.d       $f4, $f18, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f2.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f2.d);
    // 0x0F000234: trunc.w.d   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_D(ctx->f4.d);
    // 0x0F000238: mfc1        $t9, $f6
    ctx->r25 = (int32_t)ctx->f6.u32l;
    // 0x0F00023C: nop

    // 0x0F000240: sh          $t9, 0x5C($v0)
    MEM_H(0X5C, ctx->r2) = ctx->r25;
    // 0x0F000244: lwc1        $f8, 0xC($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0XC);
    // 0x0F000248: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x0F00024C: mul.d       $f18, $f10, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x0F000250: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x0F000254: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x0F000258: trunc.w.d   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_D(ctx->f18.d);
    // 0x0F00025C: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
    // 0x0F000260: nop

    // 0x0F000264: sll         $t8, $t7, 16
    ctx->r24 = S32(ctx->r15 << 16);
    // 0x0F000268: sra         $t0, $t8, 16
    ctx->r8 = S32(SIGNED(ctx->r24) >> 16);
    // 0x0F00026C: negu        $t1, $t0
    ctx->r9 = SUB32(0, ctx->r8);
    // 0x0F000270: mtc1        $t1, $f6
    ctx->f6.u32l = ctx->r9;
    // 0x0F000274: nop

    // 0x0F000278: cvt.d.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.d = CVT_D_W(ctx->f6.u32l);
    // 0x0F00027C: mul.d       $f18, $f8, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = MUL_D(ctx->f8.d, ctx->f10.d);
    // 0x0F000280: nop

    // 0x0F000284: mul.d       $f4, $f18, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f2.d); 
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f2.d);
    // 0x0F000288: trunc.w.d   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_D(ctx->f4.d);
    // 0x0F00028C: mfc1        $t3, $f6
    ctx->r11 = (int32_t)ctx->f6.u32l;
    // 0x0F000290: nop

    // 0x0F000294: sh          $t3, 0x5E($v0)
    MEM_H(0X5E, ctx->r2) = ctx->r11;
    // 0x0F000298: lwc1        $f8, 0xC($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0XC);
    // 0x0F00029C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x0F0002A0: mul.d       $f18, $f10, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x0F0002A4: trunc.w.d   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_D(ctx->f18.d);
    // 0x0F0002A8: mfc1        $t5, $f4
    ctx->r13 = (int32_t)ctx->f4.u32l;
    // 0x0F0002AC: nop

    // 0x0F0002B0: sll         $t9, $t5, 16
    ctx->r25 = S32(ctx->r13 << 16);
    // 0x0F0002B4: sra         $t6, $t9, 16
    ctx->r14 = S32(SIGNED(ctx->r25) >> 16);
    // 0x0F0002B8: mtc1        $t6, $f6
    ctx->f6.u32l = ctx->r14;
    // 0x0F0002BC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0002C0: addiu       $t9, $t9, -0x5768
    ctx->r25 = ADD32(ctx->r25, -0X5768);
    // 0x0F0002C4: cvt.d.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.d = CVT_D_W(ctx->f6.u32l);
    // 0x0F0002C8: mul.d       $f10, $f8, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f14.d); 
    ctx->f10.d = MUL_D(ctx->f8.d, ctx->f14.d);
    // 0x0F0002CC: nop

    // 0x0F0002D0: mul.d       $f18, $f10, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f2.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f2.d);
    // 0x0F0002D4: trunc.w.d   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_D(ctx->f18.d);
    // 0x0F0002D8: mfc1        $t8, $f4
    ctx->r24 = (int32_t)ctx->f4.u32l;
    // 0x0F0002DC: nop

    // 0x0F0002E0: sh          $t8, 0x60($v0)
    MEM_H(0X60, ctx->r2) = ctx->r24;
    // 0x0F0002E4: sw          $zero, 0x34($s0)
    MEM_W(0X34, ctx->r16) = 0;
    // 0x0F0002E8: sw          $zero, 0x38($s0)
    MEM_W(0X38, ctx->r16) = 0;
    // 0x0F0002EC: jalr        $t9
    // 0x0F0002F0: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F0002F0: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    after_4:
    // 0x0F0002F4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0002F8: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F0002FC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000300: addiu       $t9, $t9, -0x54D0
    ctx->r25 = ADD32(ctx->r25, -0X54D0);
    // 0x0F000304: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F000308: mfc1        $a2, $f16
    ctx->r6 = (int32_t)ctx->f16.u32l;
    // 0x0F00030C: addiu       $a1, $a1, 0xAEC
    ctx->r5 = ADD32(ctx->r5, 0XAEC);
    // 0x0F000310: jalr        $t9
    // 0x0F000314: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000314: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    after_5:
    // 0x0F000318: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F00031C: addiu       $t9, $t9, -0x52F4
    ctx->r25 = ADD32(ctx->r25, -0X52F4);
    // 0x0F000320: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x0F000324: jalr        $t9
    // 0x0F000328: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000328: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    after_6:
    // 0x0F00032C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000330: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F000334: jalr        $t9
    // 0x0F000338: addiu       $a0, $zero, 0xD
    ctx->r4 = ADD32(0, 0XD);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000338: addiu       $a0, $zero, 0xD
    ctx->r4 = ADD32(0, 0XD);
    after_7:
    // 0x0F00033C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F000340: lwc1        $f6, -0x7D38($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F000344: lui         $at, 0x3F00
    ctx->r1 = S32(0X3F00 << 16);
    // 0x0F000348: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F00034C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000350: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000354: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x0F000358: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x0F00035C: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000360: addiu       $a0, $zero, 0x4000
    ctx->r4 = ADD32(0, 0X4000);
    // 0x0F000364: addiu       $a2, $zero, 0xFF
    ctx->r6 = ADD32(0, 0XFF);
    // 0x0F000368: addiu       $a3, $zero, 0xFF
    ctx->r7 = ADD32(0, 0XFF);
    // 0x0F00036C: addiu       $t1, $zero, 0xFF
    ctx->r9 = ADD32(0, 0XFF);
    // 0x0F000370: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    // 0x0F000374: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x0F000378: nop

    // 0x0F00037C: cvt.w.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F000380: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x0F000384: nop

    // 0x0F000388: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x0F00038C: beql        $a1, $zero, L_0F0003DC
    if (ctx->r5 == 0) {
        // 0x0F000390: mfc1        $a1, $f18
        ctx->r5 = (int32_t)ctx->f18.u32l;
            goto L_0F0003DC;
    }
    goto skip_0;
    // 0x0F000390: mfc1        $a1, $f18
    ctx->r5 = (int32_t)ctx->f18.u32l;
    skip_0:
    // 0x0F000394: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000398: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F00039C: sub.s       $f18, $f10, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f18.fl = ctx->f10.fl - ctx->f18.fl;
    // 0x0F0003A0: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x0F0003A4: nop

    // 0x0F0003A8: cvt.w.s     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.u32l = CVT_W_S(ctx->f18.fl);
    // 0x0F0003AC: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x0F0003B0: nop

    // 0x0F0003B4: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x0F0003B8: bne         $a1, $zero, L_0F0003D0
    if (ctx->r5 != 0) {
        // 0x0F0003BC: nop
    
            goto L_0F0003D0;
    }
    // 0x0F0003BC: nop

    // 0x0F0003C0: mfc1        $a1, $f18
    ctx->r5 = (int32_t)ctx->f18.u32l;
    // 0x0F0003C4: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0003C8: b           L_0F0003E8
    // 0x0F0003CC: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
        goto L_0F0003E8;
    // 0x0F0003CC: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
L_0F0003D0:
    // 0x0F0003D0: b           L_0F0003E8
    // 0x0F0003D4: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
        goto L_0F0003E8;
    // 0x0F0003D4: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
    // 0x0F0003D8: mfc1        $a1, $f18
    ctx->r5 = (int32_t)ctx->f18.u32l;
L_0F0003DC:
    // 0x0F0003DC: nop

    // 0x0F0003E0: bltz        $a1, L_0F0003D0
    if (SIGNED(ctx->r5) < 0) {
        // 0x0F0003E4: nop
    
            goto L_0F0003D0;
    }
    // 0x0F0003E4: nop

L_0F0003E8:
    // 0x0F0003E8: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x0F0003EC: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x0F0003F0: jalr        $t9
    // 0x0F0003F4: sw          $t1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F0003F4: sw          $t1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r9;
    after_8:
    // 0x0F0003F8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0003FC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000400: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000404: jalr        $t9
    // 0x0F000408: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F000408: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_9:
    // 0x0F00040C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x0F000410: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F000414: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x0F000418: jr          $ra
    // 0x0F00041C: nop

    return;
    // 0x0F00041C: nop

;}
RECOMP_FUNC void ni_ovl_125_func_0F000420(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000420: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F000424: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F000428: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00042C: lwc1        $f6, 0xB20($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0XB20);
    // 0x0F000430: lwc1        $f4, 0x8($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X8);
    // 0x0F000434: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x0F000438: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x0F00043C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F000440: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F000444: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000448: lw          $t6, 0x5C($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X5C);
    // 0x0F00044C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000450: sw          $t6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r14;
    // 0x0F000454: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x0F000458: ctc1        $v1, $FpcCsr
    set_cop1_cs(ctx->r3);
    // 0x0F00045C: lw          $a3, 0x28($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X28);
    // 0x0F000460: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F000464: cfc1        $v1, $FpcCsr
    ctx->r3 = get_cop1_cs();
    // 0x0F000468: nop

    // 0x0F00046C: andi        $v1, $v1, 0x78
    ctx->r3 = ctx->r3 & 0X78;
    // 0x0F000470: beq         $v1, $zero, L_0F0004BC
    if (ctx->r3 == 0) {
        // 0x0F000474: lui         $at, 0x4F00
        ctx->r1 = S32(0X4F00 << 16);
            goto L_0F0004BC;
    }
    // 0x0F000474: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000478: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F00047C: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x0F000480: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F000484: ctc1        $v1, $FpcCsr
    set_cop1_cs(ctx->r3);
    // 0x0F000488: nop

    // 0x0F00048C: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F000490: cfc1        $v1, $FpcCsr
    ctx->r3 = get_cop1_cs();
    // 0x0F000494: nop

    // 0x0F000498: andi        $v1, $v1, 0x78
    ctx->r3 = ctx->r3 & 0X78;
    // 0x0F00049C: bne         $v1, $zero, L_0F0004B4
    if (ctx->r3 != 0) {
        // 0x0F0004A0: nop
    
            goto L_0F0004B4;
    }
    // 0x0F0004A0: nop

    // 0x0F0004A4: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x0F0004A8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0004AC: b           L_0F0004CC
    // 0x0F0004B0: or          $v1, $v1, $at
    ctx->r3 = ctx->r3 | ctx->r1;
        goto L_0F0004CC;
    // 0x0F0004B0: or          $v1, $v1, $at
    ctx->r3 = ctx->r3 | ctx->r1;
L_0F0004B4:
    // 0x0F0004B4: b           L_0F0004CC
    // 0x0F0004B8: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
        goto L_0F0004CC;
    // 0x0F0004B8: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
L_0F0004BC:
    // 0x0F0004BC: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x0F0004C0: nop

    // 0x0F0004C4: bltz        $v1, L_0F0004B4
    if (SIGNED(ctx->r3) < 0) {
        // 0x0F0004C8: nop
    
            goto L_0F0004B4;
    }
    // 0x0F0004C8: nop

L_0F0004CC:
    // 0x0F0004CC: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
    // 0x0F0004D0: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x0F0004D4: sltu        $at, $v1, $v0
    ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
    // 0x0F0004D8: bnel        $at, $zero, L_0F000504
    if (ctx->r1 != 0) {
        // 0x0F0004DC: lwc1        $f18, 0xC($a2)
        ctx->f18.u32l = MEM_W(ctx->r6, 0XC);
            goto L_0F000504;
    }
    goto skip_0;
    // 0x0F0004DC: lwc1        $f18, 0xC($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0XC);
    skip_0:
    // 0x0F0004E0: lhu         $a0, 0x538($a2)
    ctx->r4 = MEM_HU(ctx->r6, 0X538);
    // 0x0F0004E4: andi        $t8, $a0, 0x1080
    ctx->r24 = ctx->r4 & 0X1080;
    // 0x0F0004E8: bne         $t8, $zero, L_0F0004F8
    if (ctx->r24 != 0) {
        // 0x0F0004EC: andi        $t9, $a0, 0x4000
        ctx->r25 = ctx->r4 & 0X4000;
            goto L_0F0004F8;
    }
    // 0x0F0004EC: andi        $t9, $a0, 0x4000
    ctx->r25 = ctx->r4 & 0X4000;
    // 0x0F0004F0: beql        $t9, $zero, L_0F000504
    if (ctx->r25 == 0) {
        // 0x0F0004F4: lwc1        $f18, 0xC($a2)
        ctx->f18.u32l = MEM_W(ctx->r6, 0XC);
            goto L_0F000504;
    }
    goto skip_1;
    // 0x0F0004F4: lwc1        $f18, 0xC($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0XC);
    skip_1:
L_0F0004F8:
    // 0x0F0004F8: sw          $v1, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->r3;
    // 0x0F0004FC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x0F000500: lwc1        $f18, 0xC($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0XC);
L_0F000504:
    // 0x0F000504: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000508: ldc1        $f16, 0xB28($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, 0XB28);
    // 0x0F00050C: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x0F000510: mul.d       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f16.d, ctx->f4.d);
    // 0x0F000514: trunc.w.d   $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.u32l = TRUNC_W_D(ctx->f6.d);
    // 0x0F000518: mfc1        $v1, $f8
    ctx->r3 = (int32_t)ctx->f8.u32l;
    // 0x0F00051C: nop

    // 0x0F000520: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000524: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000528: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x0F00052C: beq         $at, $zero, L_0F00068C
    if (ctx->r1 == 0) {
        // 0x0F000530: lui         $at, 0xF00
        ctx->r1 = S32(0XF00 << 16);
            goto L_0F00068C;
    }
    // 0x0F000530: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000534: ldc1        $f2, 0xB30($at)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r1, 0XB30);
    // 0x0F000538: mtc1        $v1, $f16
    ctx->f16.u32l = ctx->r3;
    // 0x0F00053C: lui         $at, 0x4396
    ctx->r1 = S32(0X4396 << 16);
    // 0x0F000540: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000544: lwc1        $f0, 0x58($a3)
    ctx->f0.u32l = MEM_W(ctx->r7, 0X58);
    // 0x0F000548: cvt.s.w     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    ctx->f4.fl = CVT_S_W(ctx->f16.u32l);
    // 0x0F00054C: lh          $t1, 0x5C($a3)
    ctx->r9 = MEM_H(ctx->r7, 0X5C);
    // 0x0F000550: lh          $t4, 0x5E($a3)
    ctx->r12 = MEM_H(ctx->r7, 0X5E);
    // 0x0F000554: lh          $t7, 0x60($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X60);
    // 0x0F000558: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00055C: sub.s       $f18, $f0, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f0.fl - ctx->f10.fl;
    // 0x0F000560: mtc1        $t1, $f10
    ctx->f10.u32l = ctx->r9;
    // 0x0F000564: addiu       $a0, $a3, 0x50
    ctx->r4 = ADD32(ctx->r7, 0X50);
    // 0x0F000568: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F00056C: div.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = DIV_S(ctx->f18.fl, ctx->f4.fl);
    // 0x0F000570: cvt.d.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.d = CVT_D_W(ctx->f10.u32l);
    // 0x0F000574: add.d       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f2.d); 
    ctx->f18.d = ctx->f16.d + ctx->f2.d;
    // 0x0F000578: sub.s       $f8, $f0, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f0.fl - ctx->f6.fl;
    // 0x0F00057C: mtc1        $t4, $f6
    ctx->f6.u32l = ctx->r12;
    // 0x0F000580: trunc.w.d   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_D(ctx->f18.d);
    // 0x0F000584: swc1        $f8, 0x58($a3)
    MEM_W(0X58, ctx->r7) = ctx->f8.u32l;
    // 0x0F000588: cvt.d.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.d = CVT_D_W(ctx->f6.u32l);
    // 0x0F00058C: mfc1        $t3, $f4
    ctx->r11 = (int32_t)ctx->f4.u32l;
    // 0x0F000590: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x0F000594: sh          $t3, 0x5C($a3)
    MEM_H(0X5C, ctx->r7) = ctx->r11;
    // 0x0F000598: cvt.d.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
    // 0x0F00059C: ldc1        $f10, 0xB38($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, 0XB38);
    // 0x0F0005A0: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x0F0005A4: add.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d + ctx->f10.d;
    // 0x0F0005A8: sub.d       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f2.d); 
    ctx->f8.d = ctx->f6.d - ctx->f2.d;
    // 0x0F0005AC: trunc.w.d   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_D(ctx->f16.d);
    // 0x0F0005B0: trunc.w.d   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_D(ctx->f8.d);
    // 0x0F0005B4: mfc1        $t6, $f18
    ctx->r14 = (int32_t)ctx->f18.u32l;
    // 0x0F0005B8: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    // 0x0F0005BC: sh          $t6, 0x5E($a3)
    MEM_H(0X5E, ctx->r7) = ctx->r14;
    // 0x0F0005C0: sh          $t9, 0x60($a3)
    MEM_H(0X60, ctx->r7) = ctx->r25;
    // 0x0F0005C4: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
    // 0x0F0005C8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0005CC: addiu       $t9, $t9, 0x3B9C
    ctx->r25 = ADD32(ctx->r25, 0X3B9C);
    // 0x0F0005D0: divu        $zero, $v0, $at
    lo = S32(U32(ctx->r2) / U32(ctx->r1)); hi = S32(U32(ctx->r2) % U32(ctx->r1));
    // 0x0F0005D4: mfhi        $t0
    ctx->r8 = hi;
    // 0x0F0005D8: bne         $t0, $zero, L_0F00068C
    if (ctx->r8 != 0) {
        // 0x0F0005DC: nop
    
            goto L_0F00068C;
    }
    // 0x0F0005DC: nop

    // 0x0F0005E0: lw          $a1, 0x27BC($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27BC);
    // 0x0F0005E4: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    // 0x0F0005E8: jalr        $t9
    // 0x0F0005EC: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0005EC: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_0:
    // 0x0F0005F0: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x0F0005F4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0005F8: addiu       $t9, $t9, 0x439C
    ctx->r25 = ADD32(ctx->r25, 0X439C);
    // 0x0F0005FC: mtc1        $at, $f14
    ctx->f14.u32l = ctx->r1;
    // 0x0F000600: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
    // 0x0F000604: jalr        $t9
    // 0x0F000608: lui         $a2, 0x43C8
    ctx->r6 = S32(0X43C8 << 16);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000608: lui         $a2, 0x43C8
    ctx->r6 = S32(0X43C8 << 16);
    after_1:
    // 0x0F00060C: lw          $t1, 0x38($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X38);
    // 0x0F000610: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F000614: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000618: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x0F00061C: beq         $t1, $zero, L_0F000658
    if (ctx->r9 == 0) {
        // 0x0F000620: sub.s       $f2, $f16, $f0
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f16.fl - ctx->f0.fl;
            goto L_0F000658;
    }
    // 0x0F000620: sub.s       $f2, $f16, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = ctx->f16.fl - ctx->f0.fl;
    // 0x0F000624: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000628: addiu       $t9, $t9, 0x6220
    ctx->r25 = ADD32(ctx->r25, 0X6220);
    // 0x0F00062C: mfc1        $a2, $f2
    ctx->r6 = (int32_t)ctx->f2.u32l;
    // 0x0F000630: addiu       $a0, $zero, 0x21B
    ctx->r4 = ADD32(0, 0X21B);
    // 0x0F000634: addiu       $a1, $zero, 0x0
    ctx->r5 = ADD32(0, 0X0);
    // 0x0F000638: jalr        $t9
    // 0x0F00063C: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00063C: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_2:
    // 0x0F000640: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000644: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F000648: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F00064C: sw          $zero, 0x38($s0)
    MEM_W(0X38, ctx->r16) = 0;
    // 0x0F000650: b           L_0F00068C
    // 0x0F000654: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
        goto L_0F00068C;
    // 0x0F000654: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
L_0F000658:
    // 0x0F000658: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F00065C: addiu       $t9, $t9, 0x6220
    ctx->r25 = ADD32(ctx->r25, 0X6220);
    // 0x0F000660: mfc1        $a2, $f2
    ctx->r6 = (int32_t)ctx->f2.u32l;
    // 0x0F000664: addiu       $a0, $zero, 0x21A
    ctx->r4 = ADD32(0, 0X21A);
    // 0x0F000668: addiu       $a1, $zero, 0x0
    ctx->r5 = ADD32(0, 0X0);
    // 0x0F00066C: jalr        $t9
    // 0x0F000670: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000670: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_3:
    // 0x0F000674: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000678: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x0F00067C: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F000680: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F000684: sw          $t2, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->r10;
    // 0x0F000688: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
L_0F00068C:
    // 0x0F00068C: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000690: lwc1        $f18, 0xB40($at)
    ctx->f18.u32l = MEM_W(ctx->r1, 0XB40);
    // 0x0F000694: lwc1        $f0, 0x8($a2)
    ctx->f0.u32l = MEM_W(ctx->r6, 0X8);
    // 0x0F000698: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x0F00069C: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0006A0: mul.s       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f18.fl);
    // 0x0F0006A4: cfc1        $t3, $FpcCsr
    ctx->r11 = get_cop1_cs();
    // 0x0F0006A8: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x0F0006AC: nop

    // 0x0F0006B0: cvt.w.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = CVT_W_S(ctx->f4.fl);
    // 0x0F0006B4: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x0F0006B8: nop

    // 0x0F0006BC: andi        $t4, $t4, 0x78
    ctx->r12 = ctx->r12 & 0X78;
    // 0x0F0006C0: beql        $t4, $zero, L_0F000710
    if (ctx->r12 == 0) {
        // 0x0F0006C4: mfc1        $t4, $f6
        ctx->r12 = (int32_t)ctx->f6.u32l;
            goto L_0F000710;
    }
    goto skip_2;
    // 0x0F0006C4: mfc1        $t4, $f6
    ctx->r12 = (int32_t)ctx->f6.u32l;
    skip_2:
    // 0x0F0006C8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0006CC: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x0F0006D0: sub.s       $f6, $f4, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x0F0006D4: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x0F0006D8: nop

    // 0x0F0006DC: cvt.w.s     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.u32l = CVT_W_S(ctx->f6.fl);
    // 0x0F0006E0: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x0F0006E4: nop

    // 0x0F0006E8: andi        $t4, $t4, 0x78
    ctx->r12 = ctx->r12 & 0X78;
    // 0x0F0006EC: bne         $t4, $zero, L_0F000704
    if (ctx->r12 != 0) {
        // 0x0F0006F0: nop
    
            goto L_0F000704;
    }
    // 0x0F0006F0: nop

    // 0x0F0006F4: mfc1        $t4, $f6
    ctx->r12 = (int32_t)ctx->f6.u32l;
    // 0x0F0006F8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0006FC: b           L_0F00071C
    // 0x0F000700: or          $t4, $t4, $at
    ctx->r12 = ctx->r12 | ctx->r1;
        goto L_0F00071C;
    // 0x0F000700: or          $t4, $t4, $at
    ctx->r12 = ctx->r12 | ctx->r1;
L_0F000704:
    // 0x0F000704: b           L_0F00071C
    // 0x0F000708: addiu       $t4, $zero, -0x1
    ctx->r12 = ADD32(0, -0X1);
        goto L_0F00071C;
    // 0x0F000708: addiu       $t4, $zero, -0x1
    ctx->r12 = ADD32(0, -0X1);
    // 0x0F00070C: mfc1        $t4, $f6
    ctx->r12 = (int32_t)ctx->f6.u32l;
L_0F000710:
    // 0x0F000710: nop

    // 0x0F000714: bltz        $t4, L_0F000704
    if (SIGNED(ctx->r12) < 0) {
        // 0x0F000718: nop
    
            goto L_0F000704;
    }
    // 0x0F000718: nop

L_0F00071C:
    // 0x0F00071C: ctc1        $t3, $FpcCsr
    set_cop1_cs(ctx->r11);
    // 0x0F000720: bne         $t4, $v0, L_0F000750
    if (ctx->r12 != ctx->r2) {
        // 0x0F000724: addiu       $a0, $zero, 0x225
        ctx->r4 = ADD32(0, 0X225);
            goto L_0F000750;
    }
    // 0x0F000724: addiu       $a0, $zero, 0x225
    ctx->r4 = ADD32(0, 0X225);
    // 0x0F000728: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F00072C: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x0F000730: jalr        $t9
    // 0x0F000734: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000734: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_4:
    // 0x0F000738: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F00073C: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F000740: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F000744: lwc1        $f0, -0x7D38($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F000748: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x0F00074C: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
L_0F000750:
    // 0x0F000750: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x0F000754: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000758: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x0F00075C: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000760: mul.s       $f10, $f0, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f8.fl);
    // 0x0F000764: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x0F000768: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F00076C: ctc1        $t6, $FpcCsr
    set_cop1_cs(ctx->r14);
    // 0x0F000770: nop

    // 0x0F000774: cvt.w.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F000778: cfc1        $t6, $FpcCsr
    ctx->r14 = get_cop1_cs();
    // 0x0F00077C: nop

    // 0x0F000780: andi        $t6, $t6, 0x78
    ctx->r14 = ctx->r14 & 0X78;
    // 0x0F000784: beql        $t6, $zero, L_0F0007D4
    if (ctx->r14 == 0) {
        // 0x0F000788: mfc1        $t6, $f16
        ctx->r14 = (int32_t)ctx->f16.u32l;
            goto L_0F0007D4;
    }
    goto skip_3;
    // 0x0F000788: mfc1        $t6, $f16
    ctx->r14 = (int32_t)ctx->f16.u32l;
    skip_3:
    // 0x0F00078C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000790: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x0F000794: sub.s       $f16, $f10, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x0F000798: ctc1        $t6, $FpcCsr
    set_cop1_cs(ctx->r14);
    // 0x0F00079C: nop

    // 0x0F0007A0: cvt.w.s     $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    ctx->f16.u32l = CVT_W_S(ctx->f16.fl);
    // 0x0F0007A4: cfc1        $t6, $FpcCsr
    ctx->r14 = get_cop1_cs();
    // 0x0F0007A8: nop

    // 0x0F0007AC: andi        $t6, $t6, 0x78
    ctx->r14 = ctx->r14 & 0X78;
    // 0x0F0007B0: bne         $t6, $zero, L_0F0007C8
    if (ctx->r14 != 0) {
        // 0x0F0007B4: nop
    
            goto L_0F0007C8;
    }
    // 0x0F0007B4: nop

    // 0x0F0007B8: mfc1        $t6, $f16
    ctx->r14 = (int32_t)ctx->f16.u32l;
    // 0x0F0007BC: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0007C0: b           L_0F0007E0
    // 0x0F0007C4: or          $t6, $t6, $at
    ctx->r14 = ctx->r14 | ctx->r1;
        goto L_0F0007E0;
    // 0x0F0007C4: or          $t6, $t6, $at
    ctx->r14 = ctx->r14 | ctx->r1;
L_0F0007C8:
    // 0x0F0007C8: b           L_0F0007E0
    // 0x0F0007CC: addiu       $t6, $zero, -0x1
    ctx->r14 = ADD32(0, -0X1);
        goto L_0F0007E0;
    // 0x0F0007CC: addiu       $t6, $zero, -0x1
    ctx->r14 = ADD32(0, -0X1);
    // 0x0F0007D0: mfc1        $t6, $f16
    ctx->r14 = (int32_t)ctx->f16.u32l;
L_0F0007D4:
    // 0x0F0007D4: nop

    // 0x0F0007D8: bltz        $t6, L_0F0007C8
    if (SIGNED(ctx->r14) < 0) {
        // 0x0F0007DC: nop
    
            goto L_0F0007C8;
    }
    // 0x0F0007DC: nop

L_0F0007E0:
    // 0x0F0007E0: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F0007E4: bne         $t6, $v0, L_0F000804
    if (ctx->r14 != ctx->r2) {
        // 0x0F0007E8: lui         $at, 0xF00
        ctx->r1 = S32(0XF00 << 16);
            goto L_0F000804;
    }
    // 0x0F0007E8: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0007EC: lwc1        $f18, 0xB44($at)
    ctx->f18.u32l = MEM_W(ctx->r1, 0XB44);
    // 0x0F0007F0: sh          $zero, 0x5C($a3)
    MEM_H(0X5C, ctx->r7) = 0;
    // 0x0F0007F4: sh          $zero, 0x5E($a3)
    MEM_H(0X5E, ctx->r7) = 0;
    // 0x0F0007F8: sh          $zero, 0x60($a3)
    MEM_H(0X60, ctx->r7) = 0;
    // 0x0F0007FC: swc1        $f18, 0x58($a3)
    MEM_W(0X58, ctx->r7) = ctx->f18.u32l;
    // 0x0F000800: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
L_0F000804:
    // 0x0F000804: lwc1        $f4, 0xC($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0XC);
    // 0x0F000808: lui         $at, 0x404E
    ctx->r1 = S32(0X404E << 16);
    // 0x0F00080C: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x0F000810: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F000814: cvt.d.s     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f0.d = CVT_D_S(ctx->f4.fl);
    // 0x0F000818: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00081C: mul.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x0F000820: ldc1        $f16, 0xB48($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, 0XB48);
    // 0x0F000824: mul.d       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = MUL_D(ctx->f16.d, ctx->f0.d);
    // 0x0F000828: trunc.w.d   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_D(ctx->f8.d);
    // 0x0F00082C: trunc.w.d   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_D(ctx->f18.d);
    // 0x0F000830: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    // 0x0F000834: mfc1        $t3, $f4
    ctx->r11 = (int32_t)ctx->f4.u32l;
    // 0x0F000838: sll         $t0, $t8, 16
    ctx->r8 = S32(ctx->r24 << 16);
    // 0x0F00083C: sra         $t1, $t0, 16
    ctx->r9 = S32(SIGNED(ctx->r8) >> 16);
    // 0x0F000840: sll         $t4, $t3, 16
    ctx->r12 = S32(ctx->r11 << 16);
    // 0x0F000844: sra         $t9, $t4, 16
    ctx->r25 = S32(SIGNED(ctx->r12) >> 16);
    // 0x0F000848: subu        $t5, $v0, $t9
    ctx->r13 = SUB32(ctx->r2, ctx->r25);
    // 0x0F00084C: sltu        $at, $t1, $t5
    ctx->r1 = ctx->r9 < ctx->r13 ? 1 : 0;
    // 0x0F000850: bne         $at, $zero, L_0F000870
    if (ctx->r1 != 0) {
        // 0x0F000854: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_0F000870;
    }
    // 0x0F000854: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000858: addiu       $t9, $t9, -0x52F4
    ctx->r25 = ADD32(ctx->r25, -0X52F4);
    // 0x0F00085C: jalr        $t9
    // 0x0F000860: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000860: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    after_5:
    // 0x0F000864: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F000868: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F00086C: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
L_0F000870:
    // 0x0F000870: addiu       $t6, $v0, 0x1
    ctx->r14 = ADD32(ctx->r2, 0X1);
    // 0x0F000874: sw          $t6, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->r14;
    // 0x0F000878: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00087C: lwc1        $f6, 0xB50($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0XB50);
    // 0x0F000880: lwc1        $f0, 0x8($a2)
    ctx->f0.u32l = MEM_W(ctx->r6, 0X8);
    // 0x0F000884: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F000888: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F00088C: mul.s       $f8, $f0, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x0F000890: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000894: or          $v0, $t6, $zero
    ctx->r2 = ctx->r14 | 0;
    // 0x0F000898: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x0F00089C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F0008A0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F0008A4: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x0F0008A8: ctc1        $t8, $FpcCsr
    set_cop1_cs(ctx->r24);
    // 0x0F0008AC: nop

    // 0x0F0008B0: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F0008B4: cfc1        $t8, $FpcCsr
    ctx->r24 = get_cop1_cs();
    // 0x0F0008B8: nop

    // 0x0F0008BC: andi        $t8, $t8, 0x78
    ctx->r24 = ctx->r24 & 0X78;
    // 0x0F0008C0: beql        $t8, $zero, L_0F000910
    if (ctx->r24 == 0) {
        // 0x0F0008C4: mfc1        $t8, $f10
        ctx->r24 = (int32_t)ctx->f10.u32l;
            goto L_0F000910;
    }
    goto skip_4;
    // 0x0F0008C4: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    skip_4:
    // 0x0F0008C8: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0008CC: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F0008D0: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F0008D4: ctc1        $t8, $FpcCsr
    set_cop1_cs(ctx->r24);
    // 0x0F0008D8: nop

    // 0x0F0008DC: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F0008E0: cfc1        $t8, $FpcCsr
    ctx->r24 = get_cop1_cs();
    // 0x0F0008E4: nop

    // 0x0F0008E8: andi        $t8, $t8, 0x78
    ctx->r24 = ctx->r24 & 0X78;
    // 0x0F0008EC: bne         $t8, $zero, L_0F000904
    if (ctx->r24 != 0) {
        // 0x0F0008F0: nop
    
            goto L_0F000904;
    }
    // 0x0F0008F0: nop

    // 0x0F0008F4: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    // 0x0F0008F8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0008FC: b           L_0F00091C
    // 0x0F000900: or          $t8, $t8, $at
    ctx->r24 = ctx->r24 | ctx->r1;
        goto L_0F00091C;
    // 0x0F000900: or          $t8, $t8, $at
    ctx->r24 = ctx->r24 | ctx->r1;
L_0F000904:
    // 0x0F000904: b           L_0F00091C
    // 0x0F000908: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
        goto L_0F00091C;
    // 0x0F000908: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    // 0x0F00090C: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
L_0F000910:
    // 0x0F000910: nop

    // 0x0F000914: bltz        $t8, L_0F000904
    if (SIGNED(ctx->r24) < 0) {
        // 0x0F000918: nop
    
            goto L_0F000904;
    }
    // 0x0F000918: nop

L_0F00091C:
    // 0x0F00091C: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x0F000920: bne         $t8, $v0, L_0F0009D0
    if (ctx->r24 != ctx->r2) {
        // 0x0F000924: lui         $at, 0x3F80
        ctx->r1 = S32(0X3F80 << 16);
            goto L_0F0009D0;
    }
    // 0x0F000924: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F000928: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F00092C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000930: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000934: mul.s       $f18, $f0, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f16.fl);
    // 0x0F000938: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x0F00093C: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    // 0x0F000940: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x0F000944: nop

    // 0x0F000948: cvt.w.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = CVT_W_S(ctx->f18.fl);
    // 0x0F00094C: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x0F000950: nop

    // 0x0F000954: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x0F000958: beql        $a1, $zero, L_0F0009A8
    if (ctx->r5 == 0) {
        // 0x0F00095C: mfc1        $a1, $f4
        ctx->r5 = (int32_t)ctx->f4.u32l;
            goto L_0F0009A8;
    }
    goto skip_5;
    // 0x0F00095C: mfc1        $a1, $f4
    ctx->r5 = (int32_t)ctx->f4.u32l;
    skip_5:
    // 0x0F000960: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000964: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000968: sub.s       $f4, $f18, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f4.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x0F00096C: ctc1        $a1, $FpcCsr
    set_cop1_cs(ctx->r5);
    // 0x0F000970: nop

    // 0x0F000974: cvt.w.s     $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    ctx->f4.u32l = CVT_W_S(ctx->f4.fl);
    // 0x0F000978: cfc1        $a1, $FpcCsr
    ctx->r5 = get_cop1_cs();
    // 0x0F00097C: nop

    // 0x0F000980: andi        $a1, $a1, 0x78
    ctx->r5 = ctx->r5 & 0X78;
    // 0x0F000984: bne         $a1, $zero, L_0F00099C
    if (ctx->r5 != 0) {
        // 0x0F000988: nop
    
            goto L_0F00099C;
    }
    // 0x0F000988: nop

    // 0x0F00098C: mfc1        $a1, $f4
    ctx->r5 = (int32_t)ctx->f4.u32l;
    // 0x0F000990: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F000994: b           L_0F0009B4
    // 0x0F000998: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
        goto L_0F0009B4;
    // 0x0F000998: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
L_0F00099C:
    // 0x0F00099C: b           L_0F0009B4
    // 0x0F0009A0: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
        goto L_0F0009B4;
    // 0x0F0009A0: addiu       $a1, $zero, -0x1
    ctx->r5 = ADD32(0, -0X1);
    // 0x0F0009A4: mfc1        $a1, $f4
    ctx->r5 = (int32_t)ctx->f4.u32l;
L_0F0009A8:
    // 0x0F0009A8: nop

    // 0x0F0009AC: bltz        $a1, L_0F00099C
    if (SIGNED(ctx->r5) < 0) {
        // 0x0F0009B0: nop
    
            goto L_0F00099C;
    }
    // 0x0F0009B0: nop

L_0F0009B4:
    // 0x0F0009B4: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x0F0009B8: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x0F0009BC: jalr        $t9
    // 0x0F0009C0: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F0009C0: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_6:
    // 0x0F0009C4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F0009C8: lwc1        $f0, -0x7D38($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F0009CC: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
L_0F0009D0:
    // 0x0F0009D0: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0009D4: lwc1        $f6, 0xB54($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0XB54);
    // 0x0F0009D8: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x0F0009DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0009E0: mul.s       $f8, $f0, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f6.fl);
    // 0x0F0009E4: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0009E8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0009EC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0009F0: cfc1        $t2, $FpcCsr
    ctx->r10 = get_cop1_cs();
    // 0x0F0009F4: ctc1        $t3, $FpcCsr
    set_cop1_cs(ctx->r11);
    // 0x0F0009F8: nop

    // 0x0F0009FC: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F000A00: cfc1        $t3, $FpcCsr
    ctx->r11 = get_cop1_cs();
    // 0x0F000A04: nop

    // 0x0F000A08: andi        $t3, $t3, 0x78
    ctx->r11 = ctx->r11 & 0X78;
    // 0x0F000A0C: beql        $t3, $zero, L_0F000A5C
    if (ctx->r11 == 0) {
        // 0x0F000A10: mfc1        $t3, $f10
        ctx->r11 = (int32_t)ctx->f10.u32l;
            goto L_0F000A5C;
    }
    goto skip_6;
    // 0x0F000A10: mfc1        $t3, $f10
    ctx->r11 = (int32_t)ctx->f10.u32l;
    skip_6:
    // 0x0F000A14: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000A18: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x0F000A1C: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F000A20: ctc1        $t3, $FpcCsr
    set_cop1_cs(ctx->r11);
    // 0x0F000A24: nop

    // 0x0F000A28: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F000A2C: cfc1        $t3, $FpcCsr
    ctx->r11 = get_cop1_cs();
    // 0x0F000A30: nop

    // 0x0F000A34: andi        $t3, $t3, 0x78
    ctx->r11 = ctx->r11 & 0X78;
    // 0x0F000A38: bne         $t3, $zero, L_0F000A50
    if (ctx->r11 != 0) {
        // 0x0F000A3C: nop
    
            goto L_0F000A50;
    }
    // 0x0F000A3C: nop

    // 0x0F000A40: mfc1        $t3, $f10
    ctx->r11 = (int32_t)ctx->f10.u32l;
    // 0x0F000A44: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F000A48: b           L_0F000A68
    // 0x0F000A4C: or          $t3, $t3, $at
    ctx->r11 = ctx->r11 | ctx->r1;
        goto L_0F000A68;
    // 0x0F000A4C: or          $t3, $t3, $at
    ctx->r11 = ctx->r11 | ctx->r1;
L_0F000A50:
    // 0x0F000A50: b           L_0F000A68
    // 0x0F000A54: addiu       $t3, $zero, -0x1
    ctx->r11 = ADD32(0, -0X1);
        goto L_0F000A68;
    // 0x0F000A54: addiu       $t3, $zero, -0x1
    ctx->r11 = ADD32(0, -0X1);
    // 0x0F000A58: mfc1        $t3, $f10
    ctx->r11 = (int32_t)ctx->f10.u32l;
L_0F000A5C:
    // 0x0F000A5C: nop

    // 0x0F000A60: bltz        $t3, L_0F000A50
    if (SIGNED(ctx->r11) < 0) {
        // 0x0F000A64: nop
    
            goto L_0F000A50;
    }
    // 0x0F000A64: nop

L_0F000A68:
    // 0x0F000A68: ctc1        $t2, $FpcCsr
    set_cop1_cs(ctx->r10);
    // 0x0F000A6C: sltu        $at, $t3, $v0
    ctx->r1 = ctx->r11 < ctx->r2 ? 1 : 0;
    // 0x0F000A70: beql        $at, $zero, L_0F000A84
    if (ctx->r1 == 0) {
        // 0x0F000A74: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_0F000A84;
    }
    goto skip_7;
    // 0x0F000A74: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_7:
    // 0x0F000A78: jalr        $t9
    // 0x0F000A7C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000A7C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_7:
    // 0x0F000A80: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_0F000A84:
    // 0x0F000A84: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F000A88: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x0F000A8C: jr          $ra
    // 0x0F000A90: nop

    return;
    // 0x0F000A90: nop

;}
RECOMP_FUNC void ni_ovl_125_func_0F000A94(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000A94: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000A98: addiu       $t6, $zero, 0xFF
    ctx->r14 = ADD32(0, 0XFF);
    // 0x0F000A9C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F000AA0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000AA4: sw          $t6, -0x7CC4($at)
    MEM_W(-0X7CC4, ctx->r1) = ctx->r14;
    // 0x0F000AA8: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F000AAC: jalr        $t9
    // 0x0F000AB0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000AB0: nop

    after_0:
    // 0x0F000AB4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000AB8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000ABC: jr          $ra
    // 0x0F000AC0: nop

    return;
    // 0x0F000AC0: nop

;}
RECOMP_FUNC void ni_ovl_126_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0xA88($t9)
    ctx->r25 = MEM_W(ctx->r25, 0XA88);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_126_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x0F000074: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000078: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x0F00007C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x0F000080: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x0F000084: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x0F000088: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x0F00008C: beq         $v0, $zero, L_0F00009C
    if (ctx->r2 == 0) {
        // 0x0F000090: lui         $t9, 0xF00
        ctx->r25 = S32(0XF00 << 16);
            goto L_0F00009C;
    }
    // 0x0F000090: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000094: lhu         $t6, 0x18($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X18);
    // 0x0F000098: sw          $t6, 0x34($a0)
    MEM_W(0X34, ctx->r4) = ctx->r14;
L_0F00009C:
    // 0x0F00009C: lw          $t7, 0x34($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X34);
    // 0x0F0000A0: addiu       $t9, $t9, 0x844
    ctx->r25 = ADD32(ctx->r25, 0X844);
    // 0x0F0000A4: lh          $v1, -0x546E($v1)
    ctx->r3 = MEM_H(ctx->r3, -0X546E);
    // 0x0F0000A8: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x0F0000AC: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x0F0000B0: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x0F0000B4: addu        $v0, $t8, $t9
    ctx->r2 = ADD32(ctx->r24, ctx->r25);
    // 0x0F0000B8: lbu         $t2, 0x4($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X4);
    // 0x0F0000BC: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x0F0000C0: addiu       $t9, $t9, 0x3234
    ctx->r25 = ADD32(ctx->r25, 0X3234);
    // 0x0F0000C4: beq         $v1, $t2, L_0F0000F0
    if (ctx->r3 == ctx->r10) {
        // 0x0F0000C8: nop
    
            goto L_0F0000F0;
    }
    // 0x0F0000C8: nop

    // 0x0F0000CC: lbu         $t3, 0x5($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X5);
    // 0x0F0000D0: beq         $v1, $t3, L_0F0000F0
    if (ctx->r3 == ctx->r11) {
        // 0x0F0000D4: nop
    
            goto L_0F0000F0;
    }
    // 0x0F0000D4: nop

    // 0x0F0000D8: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x0F0000DC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F0000E0: jalr        $t9
    // 0x0F0000E4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0000E4: nop

    after_0:
    // 0x0F0000E8: b           L_0F000308
    // 0x0F0000EC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F000308;
    // 0x0F0000EC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0000F0:
    // 0x0F0000F0: jalr        $t9
    // 0x0F0000F4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000F4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_1:
    // 0x0F0000F8: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x0F0000FC: lw          $t4, -0x5170($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X5170);
    // 0x0F000100: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x0F000104: addiu       $t9, $t9, -0x38E4
    ctx->r25 = ADD32(ctx->r25, -0X38E4);
    // 0x0F000108: andi        $t5, $t4, 0x1
    ctx->r13 = ctx->r12 & 0X1;
    // 0x0F00010C: bnel        $t5, $zero, L_0F000308
    if (ctx->r13 != 0) {
        // 0x0F000110: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_0F000308;
    }
    goto skip_0;
    // 0x0F000110: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x0F000114: jalr        $t9
    // 0x0F000118: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000118: nop

    after_2:
    // 0x0F00011C: lw          $t6, 0x34($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X34);
    // 0x0F000120: lui         $t8, 0xF00
    ctx->r24 = S32(0XF00 << 16);
    // 0x0F000124: addiu       $t8, $t8, 0x844
    ctx->r24 = ADD32(ctx->r24, 0X844);
    // 0x0F000128: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x0F00012C: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x0F000130: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x0F000134: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x0F000138: lh          $a3, 0x2($v0)
    ctx->r7 = MEM_H(ctx->r2, 0X2);
    // 0x0F00013C: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F000140: addiu       $a2, $a2, 0x9D8
    ctx->r6 = ADD32(ctx->r6, 0X9D8);
    // 0x0F000144: sw          $a3, 0x60($s1)
    MEM_W(0X60, ctx->r17) = ctx->r7;
    // 0x0F000148: lbu         $v1, 0x6($v0)
    ctx->r3 = MEM_BU(ctx->r2, 0X6);
    // 0x0F00014C: lui         $t1, 0xF00
    ctx->r9 = S32(0XF00 << 16);
    // 0x0F000150: addiu       $t1, $t1, 0x958
    ctx->r9 = ADD32(ctx->r9, 0X958);
    // 0x0F000154: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x0F000158: bgez        $v1, L_0F000164
    if (SIGNED(ctx->r3) >= 0) {
        // 0x0F00015C: addu        $at, $v1, $zero
        ctx->r1 = ADD32(ctx->r3, 0);
            goto L_0F000164;
    }
    // 0x0F00015C: addu        $at, $v1, $zero
    ctx->r1 = ADD32(ctx->r3, 0);
    // 0x0F000160: addiu       $at, $v1, 0x1
    ctx->r1 = ADD32(ctx->r3, 0X1);
L_0F000164:
    // 0x0F000164: sra         $v1, $at, 1
    ctx->r3 = S32(SIGNED(ctx->r1) >> 1);
    // 0x0F000168: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x0F00016C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000170: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000174: sh          $v1, 0x30($a2)
    MEM_H(0X30, ctx->r6) = ctx->r3;
    // 0x0F000178: lh          $t2, 0x30($a2)
    ctx->r10 = MEM_H(ctx->r6, 0X30);
    // 0x0F00017C: lui         $t0, 0x800A
    ctx->r8 = S32(0X800A << 16);
    // 0x0F000180: negu        $a1, $v1
    ctx->r5 = SUB32(0, ctx->r3);
    // 0x0F000184: lui         $t3, 0xE700
    ctx->r11 = S32(0XE700 << 16);
    // 0x0F000188: addiu       $t0, $t0, -0x6C40
    ctx->r8 = ADD32(ctx->r8, -0X6C40);
    // 0x0F00018C: lui         $t4, 0xDE00
    ctx->r12 = S32(0XDE00 << 16);
    // 0x0F000190: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000194: sh          $a1, 0x20($a2)
    MEM_H(0X20, ctx->r6) = ctx->r5;
    // 0x0F000198: sh          $a1, 0x0($a2)
    MEM_H(0X0, ctx->r6) = ctx->r5;
    // 0x0F00019C: sw          $t3, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r11;
    // 0x0F0001A0: sw          $zero, 0x4($t1)
    MEM_W(0X4, ctx->r9) = 0;
    // 0x0F0001A4: addiu       $a0, $a0, 0xA18
    ctx->r4 = ADD32(ctx->r4, 0XA18);
    // 0x0F0001A8: sw          $t4, 0x8($t1)
    MEM_W(0X8, ctx->r9) = ctx->r12;
    // 0x0F0001AC: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    // 0x0F0001B0: jalr        $t0
    // 0x0F0001B4: sh          $t2, 0x10($a2)
    MEM_H(0X10, ctx->r6) = ctx->r10;
    LOOKUP_FUNC(ctx->r8)(rdram, ctx);
        goto after_3;
    // 0x0F0001B4: sh          $t2, 0x10($a2)
    MEM_H(0X10, ctx->r6) = ctx->r10;
    after_3:
    // 0x0F0001B8: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0001BC: lui         $v1, 0xF00
    ctx->r3 = S32(0XF00 << 16);
    // 0x0F0001C0: sw          $v0, 0x964($at)
    MEM_W(0X964, ctx->r1) = ctx->r2;
    // 0x0F0001C4: addiu       $v1, $v1, 0x968
    ctx->r3 = ADD32(ctx->r3, 0X968);
    // 0x0F0001C8: lui         $s0, 0xF00
    ctx->r16 = S32(0XF00 << 16);
    // 0x0F0001CC: addiu       $s0, $s0, 0x970
    ctx->r16 = ADD32(ctx->r16, 0X970);
    // 0x0F0001D0: lui         $t5, 0xE700
    ctx->r13 = S32(0XE700 << 16);
    // 0x0F0001D4: lui         $t9, 0x100
    ctx->r25 = S32(0X100 << 16);
    // 0x0F0001D8: sw          $t5, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r13;
    // 0x0F0001DC: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x0F0001E0: ori         $t9, $t9, 0x4008
    ctx->r25 = ctx->r25 | 0X4008;
    // 0x0F0001E4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x0F0001E8: sw          $t9, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r25;
    // 0x0F0001EC: lw          $t9, 0x24($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X24);
    // 0x0F0001F0: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F0001F4: addiu       $a0, $a2, 0x9D8
    ctx->r4 = ADD32(ctx->r6, 0X9D8);
    // 0x0F0001F8: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x0F0001FC: jalr        $t9
    // 0x0F000200: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F000200: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    after_4:
    // 0x0F000204: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    // 0x0F000208: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x0F00020C: lui         $t6, 0x604
    ctx->r14 = S32(0X604 << 16);
    // 0x0F000210: lui         $t7, 0x6
    ctx->r15 = S32(0X6 << 16);
    // 0x0F000214: sw          $v0, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r2;
    // 0x0F000218: ori         $t7, $t7, 0x402
    ctx->r15 = ctx->r15 | 0X402;
    // 0x0F00021C: ori         $t6, $t6, 0x200
    ctx->r14 = ctx->r14 | 0X200;
    // 0x0F000220: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    // 0x0F000224: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x0F000228: sw          $t7, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r15;
    // 0x0F00022C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x0F000230: lui         $t8, 0xDE00
    ctx->r24 = S32(0XDE00 << 16);
    // 0x0F000234: sw          $t8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r24;
    // 0x0F000238: lw          $t9, 0x24($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X24);
    // 0x0F00023C: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000240: addiu       $a0, $a0, 0xA70
    ctx->r4 = ADD32(ctx->r4, 0XA70);
    // 0x0F000244: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    // 0x0F000248: jalr        $t9
    // 0x0F00024C: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F00024C: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    after_5:
    // 0x0F000250: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x0F000254: lui         $t2, 0xDF00
    ctx->r10 = S32(0XDF00 << 16);
    // 0x0F000258: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00025C: sw          $v0, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r2;
    // 0x0F000260: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x0F000264: sw          $t2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r10;
    // 0x0F000268: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x0F00026C: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F000270: lw          $a1, 0x27B0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27B0);
    // 0x0F000274: jalr        $t9
    // 0x0F000278: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000278: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_6:
    // 0x0F00027C: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x0F000280: sw          $zero, 0x40($v0)
    MEM_W(0X40, ctx->r2) = 0;
    // 0x0F000284: lw          $t9, 0x24($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X24);
    // 0x0F000288: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F00028C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000290: jalr        $t9
    // 0x0F000294: addiu       $a0, $a0, 0x958
    ctx->r4 = ADD32(ctx->r4, 0X958);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000294: addiu       $a0, $a0, 0x958
    ctx->r4 = ADD32(ctx->r4, 0X958);
    after_7:
    // 0x0F000298: lhu         $t3, 0x2($s0)
    ctx->r11 = MEM_HU(ctx->r16, 0X2);
    // 0x0F00029C: addiu       $t5, $zero, -0x100
    ctx->r13 = ADD32(0, -0X100);
    // 0x0F0002A0: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F0002A4: ori         $t4, $t3, 0x800
    ctx->r12 = ctx->r11 | 0X800;
    // 0x0F0002A8: sw          $v0, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r2;
    // 0x0F0002AC: sh          $t4, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r12;
    // 0x0F0002B0: sw          $t5, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r13;
    // 0x0F0002B4: addiu       $t9, $t9, 0x2FE0
    ctx->r25 = ADD32(ctx->r25, 0X2FE0);
    // 0x0F0002B8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F0002BC: jalr        $t9
    // 0x0F0002C0: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F0002C0: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
    after_8:
    // 0x0F0002C4: sw          $v0, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->r2;
    // 0x0F0002C8: sw          $zero, 0x50($s1)
    MEM_W(0X50, ctx->r17) = 0;
    // 0x0F0002CC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F0002D0: lwc1        $f4, -0x7D38($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F0002D4: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x0F0002D8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0002DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0002E0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0002E4: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F0002E8: sw          $zero, 0x38($s1)
    MEM_W(0X38, ctx->r17) = 0;
    // 0x0F0002EC: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x0F0002F0: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    // 0x0F0002F4: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x0F0002F8: mfc1        $t7, $f10
    ctx->r15 = (int32_t)ctx->f10.u32l;
    // 0x0F0002FC: jalr        $t9
    // 0x0F000300: sw          $t7, 0x48($s1)
    MEM_W(0X48, ctx->r17) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F000300: sw          $t7, 0x48($s1)
    MEM_W(0X48, ctx->r17) = ctx->r15;
    after_9:
    // 0x0F000304: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000308:
    // 0x0F000308: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x0F00030C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x0F000310: jr          $ra
    // 0x0F000314: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x0F000314: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void ni_ovl_126_func_0F000318(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000318: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x0F00031C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x0F000320: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000324: ldc1        $f2, 0xAA0($at)
    CHECK_FR(ctx, 2);
    ctx->f2.u64 = LD(ctx->r1, 0XAA0);
    // 0x0F000328: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x0F00032C: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x0F000330: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x0F000334: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x0F000338: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x0F00033C: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x0F000340: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x0F000344: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F000348: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x0F00034C: lw          $t6, 0x54($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X54);
    // 0x0F000350: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F000354: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000358: sw          $t6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r14;
    // 0x0F00035C: lw          $t7, 0x38($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X38);
    // 0x0F000360: div.d       $f16, $f2, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = DIV_D(ctx->f2.d, ctx->f10.d);
    // 0x0F000364: mtc1        $t7, $f18
    ctx->f18.u32l = ctx->r15;
    // 0x0F000368: nop

    // 0x0F00036C: cvt.d.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.d = CVT_D_W(ctx->f18.u32l);
    // 0x0F000370: add.d       $f0, $f4, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f16.d); 
    ctx->f0.d = ctx->f4.d + ctx->f16.d;
    // 0x0F000374: c.lt.d      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.d < ctx->f2.d;
    // 0x0F000378: nop

    // 0x0F00037C: bc1fl       L_0F0003AC
    if (!c1cs) {
        // 0x0F000380: lh          $t1, 0x2B4E($v1)
        ctx->r9 = MEM_H(ctx->r3, 0X2B4E);
            goto L_0F0003AC;
    }
    goto skip_0;
    // 0x0F000380: lh          $t1, 0x2B4E($v1)
    ctx->r9 = MEM_H(ctx->r3, 0X2B4E);
    skip_0:
    // 0x0F000384: trunc.w.d   $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    ctx->f6.u32l = TRUNC_W_D(ctx->f0.d);
    // 0x0F000388: swc1        $f6, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->f6.u32l;
    // 0x0F00038C: lw          $t9, 0x38($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X38);
    // 0x0F000390: bgez        $t9, L_0F0003A0
    if (SIGNED(ctx->r25) >= 0) {
        // 0x0F000394: sra         $t0, $t9, 8
        ctx->r8 = S32(SIGNED(ctx->r25) >> 8);
            goto L_0F0003A0;
    }
    // 0x0F000394: sra         $t0, $t9, 8
    ctx->r8 = S32(SIGNED(ctx->r25) >> 8);
    // 0x0F000398: addiu       $at, $t9, 0xFF
    ctx->r1 = ADD32(ctx->r25, 0XFF);
    // 0x0F00039C: sra         $t0, $at, 8
    ctx->r8 = S32(SIGNED(ctx->r1) >> 8);
L_0F0003A0:
    // 0x0F0003A0: b           L_0F00047C
    // 0x0F0003A4: sb          $t0, 0x1B($v0)
    MEM_B(0X1B, ctx->r2) = ctx->r8;
        goto L_0F00047C;
    // 0x0F0003A4: sb          $t0, 0x1B($v0)
    MEM_B(0X1B, ctx->r2) = ctx->r8;
    // 0x0F0003A8: lh          $t1, 0x2B4E($v1)
    ctx->r9 = MEM_H(ctx->r3, 0X2B4E);
L_0F0003AC:
    // 0x0F0003AC: lui         $t3, 0xF00
    ctx->r11 = S32(0XF00 << 16);
    // 0x0F0003B0: addiu       $t3, $t3, 0x670
    ctx->r11 = ADD32(ctx->r11, 0X670);
    // 0x0F0003B4: bne         $t1, $zero, L_0F00047C
    if (ctx->r9 != 0) {
        // 0x0F0003B8: lui         $at, 0xFF
        ctx->r1 = S32(0XFF << 16);
            goto L_0F00047C;
    }
    // 0x0F0003B8: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x0F0003BC: lw          $t2, 0xAF8($v1)
    ctx->r10 = MEM_W(ctx->r3, 0XAF8);
    // 0x0F0003C0: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x0F0003C4: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F0003C8: addiu       $t9, $t9, 0x3FA0
    ctx->r25 = ADD32(ctx->r25, 0X3FA0);
    // 0x0F0003CC: and         $t4, $t3, $at
    ctx->r12 = ctx->r11 & ctx->r1;
    // 0x0F0003D0: lw          $a1, 0x60($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X60);
    // 0x0F0003D4: jalr        $t9
    // 0x0F0003D8: addu        $a0, $t2, $t4
    ctx->r4 = ADD32(ctx->r10, ctx->r12);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0003D8: addu        $a0, $t2, $t4
    ctx->r4 = ADD32(ctx->r10, ctx->r12);
    after_0:
    // 0x0F0003DC: lw          $t5, 0x34($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X34);
    // 0x0F0003E0: lui         $v1, 0xF00
    ctx->r3 = S32(0XF00 << 16);
    // 0x0F0003E4: addiu       $t7, $zero, 0x140
    ctx->r15 = ADD32(0, 0X140);
    // 0x0F0003E8: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x0F0003EC: subu        $t6, $t6, $t5
    ctx->r14 = SUB32(ctx->r14, ctx->r13);
    // 0x0F0003F0: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x0F0003F4: addu        $v1, $v1, $t6
    ctx->r3 = ADD32(ctx->r3, ctx->r14);
    // 0x0F0003F8: lbu         $v1, 0x84A($v1)
    ctx->r3 = MEM_BU(ctx->r3, 0X84A);
    // 0x0F0003FC: sw          $zero, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = 0;
    // 0x0F000400: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x0F000404: subu        $t8, $t7, $v1
    ctx->r24 = SUB32(ctx->r15, ctx->r3);
    // 0x0F000408: sw          $t9, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r25;
    // 0x0F00040C: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F000410: addiu       $t9, $t9, 0x3008
    ctx->r25 = ADD32(ctx->r25, 0X3008);
    // 0x0F000414: addiu       $t4, $zero, 0x10
    ctx->r12 = ADD32(0, 0X10);
    // 0x0F000418: sw          $t4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r12;
    // 0x0F00041C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x0F000420: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x0F000424: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000428: bgez        $t8, L_0F000438
    if (SIGNED(ctx->r24) >= 0) {
        // 0x0F00042C: sra         $t0, $t8, 1
        ctx->r8 = S32(SIGNED(ctx->r24) >> 1);
            goto L_0F000438;
    }
    // 0x0F00042C: sra         $t0, $t8, 1
    ctx->r8 = S32(SIGNED(ctx->r24) >> 1);
    // 0x0F000430: addiu       $at, $t8, 0x1
    ctx->r1 = ADD32(ctx->r24, 0X1);
    // 0x0F000434: sra         $t0, $at, 1
    ctx->r8 = S32(SIGNED(ctx->r1) >> 1);
L_0F000438:
    // 0x0F000438: sll         $t1, $t0, 16
    ctx->r9 = S32(ctx->r8 << 16);
    // 0x0F00043C: sra         $t3, $t1, 16
    ctx->r11 = S32(SIGNED(ctx->r9) >> 16);
    // 0x0F000440: addiu       $t2, $t3, -0x9B
    ctx->r10 = ADD32(ctx->r11, -0X9B);
    // 0x0F000444: mtc1        $t2, $f8
    ctx->f8.u32l = ctx->r10;
    // 0x0F000448: lui         $at, 0xC220
    ctx->r1 = S32(0XC220 << 16);
    // 0x0F00044C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000450: cvt.s.w     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.fl = CVT_S_W(ctx->f8.u32l);
    // 0x0F000454: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    // 0x0F000458: mfc1        $a3, $f8
    ctx->r7 = (int32_t)ctx->f8.u32l;
    // 0x0F00045C: jalr        $t9
    // 0x0F000460: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000460: nop

    after_1:
    // 0x0F000464: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000468: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F00046C: sw          $zero, 0x4C($s0)
    MEM_W(0X4C, ctx->r16) = 0;
    // 0x0F000470: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000474: jalr        $t9
    // 0x0F000478: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000478: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
L_0F00047C:
    // 0x0F00047C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000480: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x0F000484: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x0F000488: jr          $ra
    // 0x0F00048C: nop

    return;
    // 0x0F00048C: nop

;}
RECOMP_FUNC void ni_ovl_126_func_0F000490(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000490: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x0F000494: lh          $t6, -0x51F2($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X51F2);
    // 0x0F000498: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F00049C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F0004A0: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0004A4: beq         $t6, $zero, L_0F0004B8
    if (ctx->r14 == 0) {
        // 0x0F0004A8: lw          $a3, 0x54($a0)
        ctx->r7 = MEM_W(ctx->r4, 0X54);
            goto L_0F0004B8;
    }
    // 0x0F0004A8: lw          $a3, 0x54($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X54);
    // 0x0F0004AC: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x0F0004B0: b           L_0F0004BC
    // 0x0F0004B4: sw          $t7, 0x58($a0)
    MEM_W(0X58, ctx->r4) = ctx->r15;
        goto L_0F0004BC;
    // 0x0F0004B4: sw          $t7, 0x58($a0)
    MEM_W(0X58, ctx->r4) = ctx->r15;
L_0F0004B8:
    // 0x0F0004B8: sw          $zero, 0x58($a2)
    MEM_W(0X58, ctx->r6) = 0;
L_0F0004BC:
    // 0x0F0004BC: lw          $t8, 0x58($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X58);
    // 0x0F0004C0: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x0F0004C4: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x0F0004C8: bne         $t8, $at, L_0F0004F4
    if (ctx->r24 != ctx->r1) {
        // 0x0F0004CC: addiu       $a1, $zero, 0x1
        ctx->r5 = ADD32(0, 0X1);
            goto L_0F0004F4;
    }
    // 0x0F0004CC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F0004D0: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F0004D4: addiu       $t9, $t9, 0x3C60
    ctx->r25 = ADD32(ctx->r25, 0X3C60);
    // 0x0F0004D8: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x0F0004DC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F0004E0: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x0F0004E4: jalr        $t9
    // 0x0F0004E8: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0004E8: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    after_0:
    // 0x0F0004EC: b           L_0F00050C
    // 0x0F0004F0: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
        goto L_0F00050C;
    // 0x0F0004F0: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
L_0F0004F4:
    // 0x0F0004F4: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F0004F8: addiu       $t9, $t9, 0x3C60
    ctx->r25 = ADD32(ctx->r25, 0X3C60);
    // 0x0F0004FC: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x0F000500: jalr        $t9
    // 0x0F000504: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000504: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    after_1:
    // 0x0F000508: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
L_0F00050C:
    // 0x0F00050C: lw          $v0, 0x48($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X48);
    // 0x0F000510: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F000514: lui         $t3, 0x801D
    ctx->r11 = S32(0X801D << 16);
    // 0x0F000518: beq         $v0, $zero, L_0F000540
    if (ctx->r2 == 0) {
        // 0x0F00051C: addiu       $t9, $t9, 0x3B10
        ctx->r25 = ADD32(ctx->r25, 0X3B10);
            goto L_0F000540;
    }
    // 0x0F00051C: addiu       $t9, $t9, 0x3B10
    ctx->r25 = ADD32(ctx->r25, 0X3B10);
    // 0x0F000520: lw          $v1, 0x4C($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X4C);
    // 0x0F000524: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x0F000528: slt         $at, $v0, $v1
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x0F00052C: bne         $at, $zero, L_0F00053C
    if (ctx->r1 != 0) {
        // 0x0F000530: addiu       $t0, $v1, 0x1
        ctx->r8 = ADD32(ctx->r3, 0X1);
            goto L_0F00053C;
    }
    // 0x0F000530: addiu       $t0, $v1, 0x1
    ctx->r8 = ADD32(ctx->r3, 0X1);
    // 0x0F000534: b           L_0F000540
    // 0x0F000538: sw          $t0, 0x4C($a2)
    MEM_W(0X4C, ctx->r6) = ctx->r8;
        goto L_0F000540;
    // 0x0F000538: sw          $t0, 0x4C($a2)
    MEM_W(0X4C, ctx->r6) = ctx->r8;
L_0F00053C:
    // 0x0F00053C: sw          $t1, 0x50($a2)
    MEM_W(0X50, ctx->r6) = ctx->r9;
L_0F000540:
    // 0x0F000540: lw          $t2, 0x50($a2)
    ctx->r10 = MEM_W(ctx->r6, 0X50);
    // 0x0F000544: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000548: bne         $t2, $zero, L_0F00055C
    if (ctx->r10 != 0) {
        // 0x0F00054C: nop
    
            goto L_0F00055C;
    }
    // 0x0F00054C: nop

    // 0x0F000550: lh          $t3, -0x51F2($t3)
    ctx->r11 = MEM_H(ctx->r11, -0X51F2);
    // 0x0F000554: beql        $t3, $zero, L_0F000584
    if (ctx->r11 == 0) {
        // 0x0F000558: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F000584;
    }
    goto skip_0;
    // 0x0F000558: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
L_0F00055C:
    // 0x0F00055C: jalr        $t9
    // 0x0F000560: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000560: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_2:
    // 0x0F000564: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x0F000568: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00056C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000570: sw          $zero, 0x54($a2)
    MEM_W(0X54, ctx->r6) = 0;
    // 0x0F000574: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F000578: jalr        $t9
    // 0x0F00057C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F00057C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_3:
    // 0x0F000580: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F000584:
    // 0x0F000584: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F000588: jr          $ra
    // 0x0F00058C: nop

    return;
    // 0x0F00058C: nop

;}
RECOMP_FUNC void ni_ovl_126_func_0F000590(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000590: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F000594: lwc1        $f4, -0x7D38($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F000598: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x0F00059C: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x0F0005A0: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x0F0005A4: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x0F0005A8: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0005AC: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x0F0005B0: ldc1        $f16, 0xAA8($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, 0XAA8);
    // 0x0F0005B4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F0005B8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F0005BC: lw          $t6, 0x38($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X38);
    // 0x0F0005C0: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0005C4: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F0005C8: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x0F0005CC: div.d       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = DIV_D(ctx->f16.d, ctx->f10.d);
    // 0x0F0005D0: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x0F0005D4: cvt.d.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
    // 0x0F0005D8: sub.d       $f8, $f6, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f18.d); 
    ctx->f8.d = ctx->f6.d - ctx->f18.d;
    // 0x0F0005DC: trunc.w.d   $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    ctx->f16.u32l = TRUNC_W_D(ctx->f8.d);
    // 0x0F0005E0: swc1        $f16, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->f16.u32l;
    // 0x0F0005E4: lw          $v1, 0x38($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X38);
    // 0x0F0005E8: bgez        $v1, L_0F000628
    if (SIGNED(ctx->r3) >= 0) {
        // 0x0F0005EC: nop
    
            goto L_0F000628;
    }
    // 0x0F0005EC: nop

    // 0x0F0005F0: sw          $zero, 0x38($a0)
    MEM_W(0X38, ctx->r4) = 0;
    // 0x0F0005F4: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x0F0005F8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x0F0005FC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x0F000600: or          $t9, $t8, $at
    ctx->r25 = ctx->r24 | ctx->r1;
    // 0x0F000604: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
    // 0x0F000608: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00060C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000610: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x0F000614: jalr        $t9
    // 0x0F000618: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000618: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x0F00061C: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x0F000620: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000624: lw          $v1, 0x38($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X38);
L_0F000628:
    // 0x0F000628: bgez        $v1, L_0F000638
    if (SIGNED(ctx->r3) >= 0) {
        // 0x0F00062C: sra         $t0, $v1, 8
        ctx->r8 = S32(SIGNED(ctx->r3) >> 8);
            goto L_0F000638;
    }
    // 0x0F00062C: sra         $t0, $v1, 8
    ctx->r8 = S32(SIGNED(ctx->r3) >> 8);
    // 0x0F000630: addiu       $at, $v1, 0xFF
    ctx->r1 = ADD32(ctx->r3, 0XFF);
    // 0x0F000634: sra         $t0, $at, 8
    ctx->r8 = S32(SIGNED(ctx->r1) >> 8);
L_0F000638:
    // 0x0F000638: sb          $t0, 0x1B($v0)
    MEM_B(0X1B, ctx->r2) = ctx->r8;
    // 0x0F00063C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000640: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F000644: jr          $ra
    // 0x0F000648: nop

    return;
    // 0x0F000648: nop

;}
RECOMP_FUNC void ni_ovl_126_func_0F00064C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F00064C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000650: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000654: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F000658: jalr        $t9
    // 0x0F00065C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00065C: nop

    after_0:
    // 0x0F000660: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000664: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000668: jr          $ra
    // 0x0F00066C: nop

    return;
    // 0x0F00066C: nop

;}
RECOMP_FUNC void ni_ovl_127_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0x94C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X94C);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_127_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F00008C: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F000090: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x0F000094: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000098: addiu       $a2, $zero, 0x20
    ctx->r6 = ADD32(0, 0X20);
    // 0x0F00009C: jalr        $t9
    // 0x0F0000A0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0000A0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x0F0000A4: lw          $t6, 0x44($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X44);
    // 0x0F0000A8: or          $s2, $v0, $zero
    ctx->r18 = ctx->r2 | 0;
    // 0x0F0000AC: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x0F0000B0: bne         $t6, $zero, L_0F0000C0
    if (ctx->r14 != 0) {
        // 0x0F0000B4: lui         $a1, 0x8019
        ctx->r5 = S32(0X8019 << 16);
            goto L_0F0000C0;
    }
    // 0x0F0000B4: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F0000B8: addiu       $t7, $zero, 0x2710
    ctx->r15 = ADD32(0, 0X2710);
    // 0x0F0000BC: sw          $t7, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r15;
L_0F0000C0:
    // 0x0F0000C0: lui         $s1, 0x8000
    ctx->r17 = S32(0X8000 << 16);
    // 0x0F0000C4: addiu       $s1, $s1, 0x5A30
    ctx->r17 = ADD32(ctx->r17, 0X5A30);
    // 0x0F0000C8: jalr        $s1
    // 0x0F0000CC: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_1;
    // 0x0F0000CC: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    after_1:
    // 0x0F0000D0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0000D4: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x0F0000D8: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x0F0000DC: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
    // 0x0F0000E0: lui         $at, 0x42B8
    ctx->r1 = S32(0X42B8 << 16);
    // 0x0F0000E4: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0000E8: lui         $at, 0xC2C6
    ctx->r1 = S32(0XC2C6 << 16);
    // 0x0F0000EC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0000F0: lhu         $t1, 0x2($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0000F4: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x0F0000F8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0000FC: lui         $t8, 0x600
    ctx->r24 = S32(0X600 << 16);
    // 0x0F000100: lui         $t3, 0xA0A0
    ctx->r11 = S32(0XA0A0 << 16);
    // 0x0F000104: addiu       $t8, $t8, 0x990
    ctx->r24 = ADD32(ctx->r24, 0X990);
    // 0x0F000108: addiu       $t0, $zero, 0xD5
    ctx->r8 = ADD32(0, 0XD5);
    // 0x0F00010C: ori         $t3, $t3, 0xA0FF
    ctx->r11 = ctx->r11 | 0XA0FF;
    // 0x0F000110: ori         $t2, $t1, 0x800
    ctx->r10 = ctx->r9 | 0X800;
    // 0x0F000114: sw          $t8, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r24;
    // 0x0F000118: sw          $t0, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r8;
    // 0x0F00011C: sh          $t2, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r10;
    // 0x0F000120: sw          $t3, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r11;
    // 0x0F000124: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000128: swc1        $f4, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f4.u32l;
    // 0x0F00012C: swc1        $f6, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f6.u32l;
    // 0x0F000130: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    // 0x0F000134: lwc1        $f10, 0x960($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X960);
    // 0x0F000138: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00013C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000140: swc1        $f10, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f10.u32l;
    // 0x0F000144: lwc1        $f16, 0x964($at)
    ctx->f16.u32l = MEM_W(ctx->r1, 0X964);
    // 0x0F000148: addiu       $t9, $t9, 0x5ABC
    ctx->r25 = ADD32(ctx->r25, 0X5ABC);
    // 0x0F00014C: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F000150: swc1        $f16, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f16.u32l;
    // 0x0F000154: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    // 0x0F000158: jalr        $t9
    // 0x0F00015C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00015C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_2:
    // 0x0F000160: sw          $v0, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r2;
    // 0x0F000164: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x0F000168: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x0F00016C: lui         $at, 0xC29A
    ctx->r1 = S32(0XC29A << 16);
    // 0x0F000170: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000174: lui         $t4, 0x600
    ctx->r12 = S32(0X600 << 16);
    // 0x0F000178: addiu       $t4, $t4, 0xA78
    ctx->r12 = ADD32(ctx->r12, 0XA78);
    // 0x0F00017C: addiu       $t5, $zero, 0xD5
    ctx->r13 = ADD32(0, 0XD5);
    // 0x0F000180: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    // 0x0F000184: ori         $t7, $t6, 0x800
    ctx->r15 = ctx->r14 | 0X800;
    // 0x0F000188: sw          $t4, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r12;
    // 0x0F00018C: sw          $t5, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r13;
    // 0x0F000190: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x0F000194: sw          $t8, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r24;
    // 0x0F000198: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00019C: swc1        $f0, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f0.u32l;
    // 0x0F0001A0: swc1        $f0, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f0.u32l;
    // 0x0F0001A4: swc1        $f18, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f18.u32l;
    // 0x0F0001A8: lwc1        $f4, 0x968($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X968);
    // 0x0F0001AC: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0001B0: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F0001B4: swc1        $f4, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f4.u32l;
    // 0x0F0001B8: lwc1        $f6, 0x96C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X96C);
    // 0x0F0001BC: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0001C0: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x0F0001C4: swc1        $f6, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f6.u32l;
    // 0x0F0001C8: lwc1        $f8, 0x970($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X970);
    // 0x0F0001CC: swc1        $f8, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f8.u32l;
    // 0x0F0001D0: jalr        $s1
    // 0x0F0001D4: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_3;
    // 0x0F0001D4: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    after_3:
    // 0x0F0001D8: sw          $v0, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->r2;
    // 0x0F0001DC: sw          $v0, 0x8($s2)
    MEM_W(0X8, ctx->r18) = ctx->r2;
    // 0x0F0001E0: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0001E4: lui         $t0, 0x600
    ctx->r8 = S32(0X600 << 16);
    // 0x0F0001E8: addiu       $t0, $t0, 0x6710
    ctx->r8 = ADD32(ctx->r8, 0X6710);
    // 0x0F0001EC: addiu       $t1, $zero, 0xC6
    ctx->r9 = ADD32(0, 0XC6);
    // 0x0F0001F0: ori         $t3, $t2, 0x800
    ctx->r11 = ctx->r10 | 0X800;
    // 0x0F0001F4: sw          $t0, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r8;
    // 0x0F0001F8: sw          $t1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r9;
    // 0x0F0001FC: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x0F000200: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000204: lui         $at, 0x41A0
    ctx->r1 = S32(0X41A0 << 16);
    // 0x0F000208: lw          $t9, 0x930($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X930);
    // 0x0F00020C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000210: lui         $at, 0xC2C4
    ctx->r1 = S32(0XC2C4 << 16);
    // 0x0F000214: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000218: lui         $at, 0x40A0
    ctx->r1 = S32(0X40A0 << 16);
    // 0x0F00021C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000220: swc1        $f20, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f20.u32l;
    // 0x0F000224: swc1        $f20, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f20.u32l;
    // 0x0F000228: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F00022C: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F000230: sw          $t9, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r25;
    // 0x0F000234: swc1        $f10, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f10.u32l;
    // 0x0F000238: swc1        $f16, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f16.u32l;
    // 0x0F00023C: swc1        $f18, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f18.u32l;
    // 0x0F000240: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    // 0x0F000244: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    // 0x0F000248: jalr        $s1
    // 0x0F00024C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_4;
    // 0x0F00024C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_4:
    // 0x0F000250: lw          $v1, 0x30($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X30);
    // 0x0F000254: sw          $v0, 0x2C($s0)
    MEM_W(0X2C, ctx->r16) = ctx->r2;
    // 0x0F000258: sw          $v0, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r2;
    // 0x0F00025C: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000260: lui         $t4, 0x600
    ctx->r12 = S32(0X600 << 16);
    // 0x0F000264: addiu       $t4, $t4, 0x6710
    ctx->r12 = ADD32(ctx->r12, 0X6710);
    // 0x0F000268: addiu       $t5, $zero, 0xC6
    ctx->r13 = ADD32(0, 0XC6);
    // 0x0F00026C: ori         $t7, $t6, 0x800
    ctx->r15 = ctx->r14 | 0X800;
    // 0x0F000270: sw          $t4, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r12;
    // 0x0F000274: sw          $t5, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r13;
    // 0x0F000278: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x0F00027C: lui         $t8, 0xF00
    ctx->r24 = S32(0XF00 << 16);
    // 0x0F000280: lw          $t8, 0x944($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X944);
    // 0x0F000284: lui         $at, 0x42C6
    ctx->r1 = S32(0X42C6 << 16);
    // 0x0F000288: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F00028C: sw          $t8, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r24;
    // 0x0F000290: lwc1        $f4, 0x68($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X68);
    // 0x0F000294: lwc1        $f10, 0x50($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X50);
    // 0x0F000298: lui         $at, 0x40A0
    ctx->r1 = S32(0X40A0 << 16);
    // 0x0F00029C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F0002A0: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0002A4: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F0002A8: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F0002AC: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x0F0002B0: add.s       $f16, $f10, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x0F0002B4: swc1        $f16, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f16.u32l;
    // 0x0F0002B8: lwc1        $f18, 0x54($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X54);
    // 0x0F0002BC: swc1        $f20, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f20.u32l;
    // 0x0F0002C0: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F0002C4: swc1        $f4, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f4.u32l;
    // 0x0F0002C8: swc1        $f6, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f6.u32l;
    // 0x0F0002CC: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x0F0002D0: jalr        $s1
    // 0x0F0002D4: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_5;
    // 0x0F0002D4: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    after_5:
    // 0x0F0002D8: sw          $v0, 0x30($s0)
    MEM_W(0X30, ctx->r16) = ctx->r2;
    // 0x0F0002DC: sw          $v0, 0x10($s2)
    MEM_W(0X10, ctx->r18) = ctx->r2;
    // 0x0F0002E0: lui         $at, 0x4020
    ctx->r1 = S32(0X4020 << 16);
    // 0x0F0002E4: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x0F0002E8: lui         $t9, 0xFF00
    ctx->r25 = S32(0XFF00 << 16);
    // 0x0F0002EC: lui         $at, 0x42F0
    ctx->r1 = S32(0X42F0 << 16);
    // 0x0F0002F0: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0002F4: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0002F8: ori         $t9, $t9, 0xFF
    ctx->r25 = ctx->r25 | 0XFF;
    // 0x0F0002FC: sw          $t9, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r25;
    // 0x0F000300: lui         $at, 0xC290
    ctx->r1 = S32(0XC290 << 16);
    // 0x0F000304: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000308: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x0F00030C: lui         $t0, 0x600
    ctx->r8 = S32(0X600 << 16);
    // 0x0F000310: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000314: addiu       $t0, $t0, 0x940
    ctx->r8 = ADD32(ctx->r8, 0X940);
    // 0x0F000318: addiu       $t1, $zero, 0xD5
    ctx->r9 = ADD32(0, 0XD5);
    // 0x0F00031C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000320: ori         $t3, $t2, 0x800
    ctx->r11 = ctx->r10 | 0X800;
    // 0x0F000324: sw          $t0, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r8;
    // 0x0F000328: sw          $t1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r9;
    // 0x0F00032C: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x0F000330: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F000334: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000338: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F00033C: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x0F000340: swc1        $f0, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f0.u32l;
    // 0x0F000344: swc1        $f10, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f10.u32l;
    // 0x0F000348: swc1        $f8, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f8.u32l;
    // 0x0F00034C: jalr        $t9
    // 0x0F000350: swc1        $f16, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000350: swc1        $f16, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f16.u32l;
    after_6:
    // 0x0F000354: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000358: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x0F00035C: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F000360: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F000364: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F000368: jr          $ra
    // 0x0F00036C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x0F00036C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void ni_ovl_127_func_0F000370(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000370: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000374: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000378: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F00037C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F000380: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000384: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x0F000388: jalr        $t9
    // 0x0F00038C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00038C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    after_0:
    // 0x0F000390: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000394: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000398: jr          $ra
    // 0x0F00039C: nop

    return;
    // 0x0F00039C: nop

;}
RECOMP_FUNC void ni_ovl_127_func_0F0003A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0003A0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x0F0003A4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F0003A8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F0003AC: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F0003B0: lh          $t6, 0x50($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X50);
    // 0x0F0003B4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F0003B8: lw          $t7, 0x10($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X10);
    // 0x0F0003BC: sh          $t6, 0x60($t7)
    MEM_H(0X60, ctx->r15) = ctx->r14;
    // 0x0F0003C0: jal         0x0F00078C
    // 0x0F0003C4: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    static_181_0F00078C(rdram, ctx);
        goto after_0;
    // 0x0F0003C4: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x0F0003C8: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x0F0003CC: addiu       $t1, $t1, -0x7D40
    ctx->r9 = ADD32(ctx->r9, -0X7D40);
    // 0x0F0003D0: lh          $t8, 0x2B4E($t1)
    ctx->r24 = MEM_H(ctx->r9, 0X2B4E);
    // 0x0F0003D4: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x0F0003D8: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0003DC: beql        $t8, $zero, L_0F000440
    if (ctx->r24 == 0) {
        // 0x0F0003E0: lw          $a0, 0x0($v1)
        ctx->r4 = MEM_W(ctx->r3, 0X0);
            goto L_0F000440;
    }
    goto skip_0;
    // 0x0F0003E0: lw          $a0, 0x0($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X0);
    skip_0:
    // 0x0F0003E4: lw          $a0, 0x0($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X0);
    // 0x0F0003E8: addiu       $t0, $zero, -0x8000
    ctx->r8 = ADD32(0, -0X8000);
    // 0x0F0003EC: lh          $t9, 0x0($a0)
    ctx->r25 = MEM_H(ctx->r4, 0X0);
    // 0x0F0003F0: or          $t2, $t9, $t0
    ctx->r10 = ctx->r25 | ctx->r8;
    // 0x0F0003F4: sh          $t2, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r10;
    // 0x0F0003F8: lw          $a1, 0x4($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X4);
    // 0x0F0003FC: lh          $t3, 0x0($a1)
    ctx->r11 = MEM_H(ctx->r5, 0X0);
    // 0x0F000400: or          $t4, $t3, $t0
    ctx->r12 = ctx->r11 | ctx->r8;
    // 0x0F000404: sh          $t4, 0x0($a1)
    MEM_H(0X0, ctx->r5) = ctx->r12;
    // 0x0F000408: lw          $a2, 0x8($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X8);
    // 0x0F00040C: lh          $t5, 0x0($a2)
    ctx->r13 = MEM_H(ctx->r6, 0X0);
    // 0x0F000410: or          $t6, $t5, $t0
    ctx->r14 = ctx->r13 | ctx->r8;
    // 0x0F000414: sh          $t6, 0x0($a2)
    MEM_H(0X0, ctx->r6) = ctx->r14;
    // 0x0F000418: lw          $a3, 0xC($v1)
    ctx->r7 = MEM_W(ctx->r3, 0XC);
    // 0x0F00041C: lh          $t7, 0x0($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X0);
    // 0x0F000420: or          $t8, $t7, $t0
    ctx->r24 = ctx->r15 | ctx->r8;
    // 0x0F000424: sh          $t8, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r24;
    // 0x0F000428: lw          $v0, 0x10($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X10);
    // 0x0F00042C: lh          $t9, 0x0($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X0);
    // 0x0F000430: or          $t2, $t9, $t0
    ctx->r10 = ctx->r25 | ctx->r8;
    // 0x0F000434: b           L_0F0004B0
    // 0x0F000438: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
        goto L_0F0004B0;
    // 0x0F000438: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
    // 0x0F00043C: lw          $a0, 0x0($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X0);
L_0F000440:
    // 0x0F000440: lh          $t3, 0x0($a0)
    ctx->r11 = MEM_H(ctx->r4, 0X0);
    // 0x0F000444: andi        $t4, $t3, 0x7FFF
    ctx->r12 = ctx->r11 & 0X7FFF;
    // 0x0F000448: sh          $t4, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r12;
    // 0x0F00044C: lw          $a1, 0x4($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X4);
    // 0x0F000450: lh          $t5, 0x0($a1)
    ctx->r13 = MEM_H(ctx->r5, 0X0);
    // 0x0F000454: andi        $t6, $t5, 0x7FFF
    ctx->r14 = ctx->r13 & 0X7FFF;
    // 0x0F000458: sh          $t6, 0x0($a1)
    MEM_H(0X0, ctx->r5) = ctx->r14;
    // 0x0F00045C: lw          $a2, 0x8($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X8);
    // 0x0F000460: lh          $t7, 0x0($a2)
    ctx->r15 = MEM_H(ctx->r6, 0X0);
    // 0x0F000464: andi        $t8, $t7, 0x7FFF
    ctx->r24 = ctx->r15 & 0X7FFF;
    // 0x0F000468: sh          $t8, 0x0($a2)
    MEM_H(0X0, ctx->r6) = ctx->r24;
    // 0x0F00046C: lw          $a3, 0xC($v1)
    ctx->r7 = MEM_W(ctx->r3, 0XC);
    // 0x0F000470: lh          $t9, 0x0($a3)
    ctx->r25 = MEM_H(ctx->r7, 0X0);
    // 0x0F000474: andi        $t2, $t9, 0x7FFF
    ctx->r10 = ctx->r25 & 0X7FFF;
    // 0x0F000478: sh          $t2, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r10;
    // 0x0F00047C: lw          $t3, 0x54($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X54);
    // 0x0F000480: beql        $t3, $zero, L_0F0004A4
    if (ctx->r11 == 0) {
        // 0x0F000484: lw          $v0, 0x10($v1)
        ctx->r2 = MEM_W(ctx->r3, 0X10);
            goto L_0F0004A4;
    }
    goto skip_1;
    // 0x0F000484: lw          $v0, 0x10($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X10);
    skip_1:
    // 0x0F000488: lw          $v0, 0x10($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X10);
    // 0x0F00048C: addiu       $t0, $zero, -0x8000
    ctx->r8 = ADD32(0, -0X8000);
    // 0x0F000490: lh          $t4, 0x0($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X0);
    // 0x0F000494: or          $t5, $t4, $t0
    ctx->r13 = ctx->r12 | ctx->r8;
    // 0x0F000498: b           L_0F0004B0
    // 0x0F00049C: sh          $t5, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r13;
        goto L_0F0004B0;
    // 0x0F00049C: sh          $t5, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r13;
    // 0x0F0004A0: lw          $v0, 0x10($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X10);
L_0F0004A4:
    // 0x0F0004A4: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x0F0004A8: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x0F0004AC: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
L_0F0004B0:
    // 0x0F0004B0: lw          $t8, 0x4C($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4C);
    // 0x0F0004B4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0004B8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F0004BC: beql        $t8, $zero, L_0F000584
    if (ctx->r24 == 0) {
        // 0x0F0004C0: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_0F000584;
    }
    goto skip_2;
    // 0x0F0004C0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x0F0004C4: lwc1        $f4, 0x8($t1)
    ctx->f4.u32l = MEM_W(ctx->r9, 0X8);
    // 0x0F0004C8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0004CC: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x0F0004D0: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0004D4: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F0004D8: cfc1        $t9, $FpcCsr
    ctx->r25 = get_cop1_cs();
    // 0x0F0004DC: ctc1        $t2, $FpcCsr
    set_cop1_cs(ctx->r10);
    // 0x0F0004E0: nop

    // 0x0F0004E4: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F0004E8: cfc1        $t2, $FpcCsr
    ctx->r10 = get_cop1_cs();
    // 0x0F0004EC: nop

    // 0x0F0004F0: andi        $t2, $t2, 0x78
    ctx->r10 = ctx->r10 & 0X78;
    // 0x0F0004F4: beql        $t2, $zero, L_0F000544
    if (ctx->r10 == 0) {
        // 0x0F0004F8: mfc1        $t2, $f10
        ctx->r10 = (int32_t)ctx->f10.u32l;
            goto L_0F000544;
    }
    goto skip_3;
    // 0x0F0004F8: mfc1        $t2, $f10
    ctx->r10 = (int32_t)ctx->f10.u32l;
    skip_3:
    // 0x0F0004FC: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000500: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x0F000504: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F000508: ctc1        $t2, $FpcCsr
    set_cop1_cs(ctx->r10);
    // 0x0F00050C: nop

    // 0x0F000510: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F000514: cfc1        $t2, $FpcCsr
    ctx->r10 = get_cop1_cs();
    // 0x0F000518: nop

    // 0x0F00051C: andi        $t2, $t2, 0x78
    ctx->r10 = ctx->r10 & 0X78;
    // 0x0F000520: bne         $t2, $zero, L_0F000538
    if (ctx->r10 != 0) {
        // 0x0F000524: nop
    
            goto L_0F000538;
    }
    // 0x0F000524: nop

    // 0x0F000528: mfc1        $t2, $f10
    ctx->r10 = (int32_t)ctx->f10.u32l;
    // 0x0F00052C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F000530: b           L_0F000550
    // 0x0F000534: or          $t2, $t2, $at
    ctx->r10 = ctx->r10 | ctx->r1;
        goto L_0F000550;
    // 0x0F000534: or          $t2, $t2, $at
    ctx->r10 = ctx->r10 | ctx->r1;
L_0F000538:
    // 0x0F000538: b           L_0F000550
    // 0x0F00053C: addiu       $t2, $zero, -0x1
    ctx->r10 = ADD32(0, -0X1);
        goto L_0F000550;
    // 0x0F00053C: addiu       $t2, $zero, -0x1
    ctx->r10 = ADD32(0, -0X1);
    // 0x0F000540: mfc1        $t2, $f10
    ctx->r10 = (int32_t)ctx->f10.u32l;
L_0F000544:
    // 0x0F000544: nop

    // 0x0F000548: bltz        $t2, L_0F000538
    if (SIGNED(ctx->r10) < 0) {
        // 0x0F00054C: nop
    
            goto L_0F000538;
    }
    // 0x0F00054C: nop

L_0F000550:
    // 0x0F000550: ctc1        $t9, $FpcCsr
    set_cop1_cs(ctx->r25);
    // 0x0F000554: mtc1        $t2, $f16
    ctx->f16.u32l = ctx->r10;
    // 0x0F000558: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00055C: bgez        $t2, L_0F000574
    if (SIGNED(ctx->r10) >= 0) {
        // 0x0F000560: cvt.s.w     $f18, $f16
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
            goto L_0F000574;
    }
    // 0x0F000560: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x0F000564: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x0F000568: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F00056C: nop

    // 0x0F000570: add.s       $f18, $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f18.fl = ctx->f18.fl + ctx->f4.fl;
L_0F000574:
    // 0x0F000574: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000578: jalr        $t9
    // 0x0F00057C: swc1        $f18, 0x48($s0)
    MEM_W(0X48, ctx->r16) = ctx->f18.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F00057C: swc1        $f18, 0x48($s0)
    MEM_W(0X48, ctx->r16) = ctx->f18.u32l;
    after_1:
    // 0x0F000580: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000584:
    // 0x0F000584: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000588: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x0F00058C: jr          $ra
    // 0x0F000590: nop

    return;
    // 0x0F000590: nop

;}
RECOMP_FUNC void ni_ovl_127_func_0F000594(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000594: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000598: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F00059C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0005A0: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x0F0005A4: lwc1        $f4, 0x48($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X48);
    // 0x0F0005A8: mtc1        $zero, $f9
    ctx->f_odd[(9 - 1) * 2] = 0;
    // 0x0F0005AC: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x0F0005B0: sub.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f2.fl;
    // 0x0F0005B4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0005B8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0005BC: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x0F0005C0: swc1        $f6, 0x48($a0)
    MEM_W(0X48, ctx->r4) = ctx->f6.u32l;
    // 0x0F0005C4: lwc1        $f0, 0x48($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X48);
    // 0x0F0005C8: lw          $a1, 0x24($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X24);
    // 0x0F0005CC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0005D0: cvt.d.s     $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f10.d = CVT_D_S(ctx->f0.fl);
    // 0x0F0005D4: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x0F0005D8: c.le.d      $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f10.d <= ctx->f8.d;
    // 0x0F0005DC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F0005E0: bc1f        L_0F0005F8
    if (!c1cs) {
        // 0x0F0005E4: nop
    
            goto L_0F0005F8;
    }
    // 0x0F0005E4: nop

    // 0x0F0005E8: jalr        $t9
    // 0x0F0005EC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0005EC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
    // 0x0F0005F0: b           L_0F00075C
    // 0x0F0005F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_0F00075C;
    // 0x0F0005F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F0005F8:
    // 0x0F0005F8: lwc1        $f16, -0x7D38($at)
    ctx->f16.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F0005FC: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x0F000600: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F000604: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x0F000608: cfc1        $t6, $FpcCsr
    ctx->r14 = get_cop1_cs();
    // 0x0F00060C: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x0F000610: nop

    // 0x0F000614: cvt.w.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = CVT_W_S(ctx->f18.fl);
    // 0x0F000618: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x0F00061C: nop

    // 0x0F000620: andi        $t7, $t7, 0x78
    ctx->r15 = ctx->r15 & 0X78;
    // 0x0F000624: beql        $t7, $zero, L_0F000674
    if (ctx->r15 == 0) {
        // 0x0F000628: mfc1        $t7, $f4
        ctx->r15 = (int32_t)ctx->f4.u32l;
            goto L_0F000674;
    }
    goto skip_0;
    // 0x0F000628: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
    skip_0:
    // 0x0F00062C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000630: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x0F000634: sub.s       $f4, $f18, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f4.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x0F000638: ctc1        $t7, $FpcCsr
    set_cop1_cs(ctx->r15);
    // 0x0F00063C: nop

    // 0x0F000640: cvt.w.s     $f4, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    ctx->f4.u32l = CVT_W_S(ctx->f4.fl);
    // 0x0F000644: cfc1        $t7, $FpcCsr
    ctx->r15 = get_cop1_cs();
    // 0x0F000648: nop

    // 0x0F00064C: andi        $t7, $t7, 0x78
    ctx->r15 = ctx->r15 & 0X78;
    // 0x0F000650: bne         $t7, $zero, L_0F000668
    if (ctx->r15 != 0) {
        // 0x0F000654: nop
    
            goto L_0F000668;
    }
    // 0x0F000654: nop

    // 0x0F000658: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
    // 0x0F00065C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F000660: b           L_0F000680
    // 0x0F000664: or          $t7, $t7, $at
    ctx->r15 = ctx->r15 | ctx->r1;
        goto L_0F000680;
    // 0x0F000664: or          $t7, $t7, $at
    ctx->r15 = ctx->r15 | ctx->r1;
L_0F000668:
    // 0x0F000668: b           L_0F000680
    // 0x0F00066C: addiu       $t7, $zero, -0x1
    ctx->r15 = ADD32(0, -0X1);
        goto L_0F000680;
    // 0x0F00066C: addiu       $t7, $zero, -0x1
    ctx->r15 = ADD32(0, -0X1);
    // 0x0F000670: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
L_0F000674:
    // 0x0F000674: nop

    // 0x0F000678: bltz        $t7, L_0F000668
    if (SIGNED(ctx->r15) < 0) {
        // 0x0F00067C: nop
    
            goto L_0F000668;
    }
    // 0x0F00067C: nop

L_0F000680:
    // 0x0F000680: ctc1        $t6, $FpcCsr
    set_cop1_cs(ctx->r14);
    // 0x0F000684: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x0F000688: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x0F00068C: bgez        $t7, L_0F0006A0
    if (SIGNED(ctx->r15) >= 0) {
        // 0x0F000690: cvt.s.w     $f8, $f6
        CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
            goto L_0F0006A0;
    }
    // 0x0F000690: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x0F000694: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000698: nop

    // 0x0F00069C: add.s       $f8, $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f8.fl + ctx->f10.fl;
L_0F0006A0:
    // 0x0F0006A0: div.s       $f16, $f0, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = DIV_S(ctx->f0.fl, ctx->f8.fl);
    // 0x0F0006A4: lui         $at, 0x437F
    ctx->r1 = S32(0X437F << 16);
    // 0x0F0006A8: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F0006AC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x0F0006B0: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0006B4: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x0F0006B8: cfc1        $t8, $FpcCsr
    ctx->r24 = get_cop1_cs();
    // 0x0F0006BC: ctc1        $v0, $FpcCsr
    set_cop1_cs(ctx->r2);
    // 0x0F0006C0: nop

    // 0x0F0006C4: cvt.w.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = CVT_W_S(ctx->f4.fl);
    // 0x0F0006C8: cfc1        $v0, $FpcCsr
    ctx->r2 = get_cop1_cs();
    // 0x0F0006CC: nop

    // 0x0F0006D0: andi        $v0, $v0, 0x78
    ctx->r2 = ctx->r2 & 0X78;
    // 0x0F0006D4: beql        $v0, $zero, L_0F000724
    if (ctx->r2 == 0) {
        // 0x0F0006D8: mfc1        $v0, $f6
        ctx->r2 = (int32_t)ctx->f6.u32l;
            goto L_0F000724;
    }
    goto skip_1;
    // 0x0F0006D8: mfc1        $v0, $f6
    ctx->r2 = (int32_t)ctx->f6.u32l;
    skip_1:
    // 0x0F0006DC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0006E0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x0F0006E4: sub.s       $f6, $f4, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x0F0006E8: ctc1        $v0, $FpcCsr
    set_cop1_cs(ctx->r2);
    // 0x0F0006EC: nop

    // 0x0F0006F0: cvt.w.s     $f6, $f6
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    ctx->f6.u32l = CVT_W_S(ctx->f6.fl);
    // 0x0F0006F4: cfc1        $v0, $FpcCsr
    ctx->r2 = get_cop1_cs();
    // 0x0F0006F8: nop

    // 0x0F0006FC: andi        $v0, $v0, 0x78
    ctx->r2 = ctx->r2 & 0X78;
    // 0x0F000700: bne         $v0, $zero, L_0F000718
    if (ctx->r2 != 0) {
        // 0x0F000704: nop
    
            goto L_0F000718;
    }
    // 0x0F000704: nop

    // 0x0F000708: mfc1        $v0, $f6
    ctx->r2 = (int32_t)ctx->f6.u32l;
    // 0x0F00070C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F000710: b           L_0F000730
    // 0x0F000714: or          $v0, $v0, $at
    ctx->r2 = ctx->r2 | ctx->r1;
        goto L_0F000730;
    // 0x0F000714: or          $v0, $v0, $at
    ctx->r2 = ctx->r2 | ctx->r1;
L_0F000718:
    // 0x0F000718: b           L_0F000730
    // 0x0F00071C: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_0F000730;
    // 0x0F00071C: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x0F000720: mfc1        $v0, $f6
    ctx->r2 = (int32_t)ctx->f6.u32l;
L_0F000724:
    // 0x0F000724: nop

    // 0x0F000728: bltz        $v0, L_0F000718
    if (SIGNED(ctx->r2) < 0) {
        // 0x0F00072C: nop
    
            goto L_0F000718;
    }
    // 0x0F00072C: nop

L_0F000730:
    // 0x0F000730: sb          $v0, 0x1B($a1)
    MEM_B(0X1B, ctx->r5) = ctx->r2;
    // 0x0F000734: lw          $t0, 0x8($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X8);
    // 0x0F000738: ctc1        $t8, $FpcCsr
    set_cop1_cs(ctx->r24);
    // 0x0F00073C: sb          $v0, 0x1B($t0)
    MEM_B(0X1B, ctx->r8) = ctx->r2;
    // 0x0F000740: lw          $t1, 0x4($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X4);
    // 0x0F000744: sb          $v0, 0x1B($t1)
    MEM_B(0X1B, ctx->r9) = ctx->r2;
    // 0x0F000748: lw          $t2, 0xC($v1)
    ctx->r10 = MEM_W(ctx->r3, 0XC);
    // 0x0F00074C: sb          $v0, 0x1B($t2)
    MEM_B(0X1B, ctx->r10) = ctx->r2;
    // 0x0F000750: lw          $t3, 0x10($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X10);
    // 0x0F000754: sb          $v0, 0x1B($t3)
    MEM_B(0X1B, ctx->r11) = ctx->r2;
    // 0x0F000758: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F00075C:
    // 0x0F00075C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000760: jr          $ra
    // 0x0F000764: nop

    return;
    // 0x0F000764: nop

;}
RECOMP_FUNC void ni_ovl_127_func_0F000768(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000768: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F00076C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000770: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F000774: jalr        $t9
    // 0x0F000778: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000778: nop

    after_0:
    // 0x0F00077C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000780: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000784: jr          $ra
    // 0x0F000788: nop

    return;
    // 0x0F000788: nop

    // 0x0F00078C: lw          $v1, 0x40($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X40);
    // 0x0F000790: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x0F000794: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000798: beq         $v1, $zero, L_0F0007A8
    if (ctx->r3 == 0) {
        // 0x0F00079C: nop
    
            goto L_0F0007A8;
    }
    // 0x0F00079C: nop

    // 0x0F0007A0: b           L_0F0007A8
    // 0x0F0007A4: lh          $a1, 0x0($v1)
    ctx->r5 = MEM_H(ctx->r3, 0X0);
        goto L_0F0007A8;
    // 0x0F0007A4: lh          $a1, 0x0($v1)
    ctx->r5 = MEM_H(ctx->r3, 0X0);
L_0F0007A8:
    // 0x0F0007A8: lh          $t6, 0x14($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X14);
    // 0x0F0007AC: subu        $t7, $t6, $a1
    ctx->r15 = SUB32(ctx->r14, ctx->r5);
    // 0x0F0007B0: sh          $t7, 0x16($v0)
    MEM_H(0X16, ctx->r2) = ctx->r15;
    // 0x0F0007B4: lh          $v1, 0x16($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X16);
    // 0x0F0007B8: beql        $v1, $zero, L_0F00083C
    if (ctx->r3 == 0) {
        // 0x0F0007BC: lh          $v1, 0x18($v0)
        ctx->r3 = MEM_H(ctx->r2, 0X18);
            goto L_0F00083C;
    }
    goto skip_0;
    // 0x0F0007BC: lh          $v1, 0x18($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X18);
    skip_0:
    // 0x0F0007C0: lh          $t8, 0x18($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X18);
    // 0x0F0007C4: sh          $zero, 0x16($v0)
    MEM_H(0X16, ctx->r2) = 0;
    // 0x0F0007C8: mtc1        $a1, $f4
    ctx->f4.u32l = ctx->r5;
    // 0x0F0007CC: addu        $t9, $t8, $v1
    ctx->r25 = ADD32(ctx->r24, ctx->r3);
    // 0x0F0007D0: sh          $t9, 0x18($v0)
    MEM_H(0X18, ctx->r2) = ctx->r25;
    // 0x0F0007D4: lw          $t0, 0x44($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X44);
    // 0x0F0007D8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x0F0007DC: lw          $t1, 0x8($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X8);
    // 0x0F0007E0: mtc1        $t0, $f8
    ctx->f8.u32l = ctx->r8;
    // 0x0F0007E4: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0007E8: lui         $t2, 0xF00
    ctx->r10 = S32(0XF00 << 16);
    // 0x0F0007EC: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x0F0007F0: lui         $t3, 0xF00
    ctx->r11 = S32(0XF00 << 16);
    // 0x0F0007F4: div.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x0F0007F8: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x0F0007FC: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x0F000800: swc1        $f4, 0x68($t1)
    MEM_W(0X68, ctx->r9) = ctx->f4.u32l;
    // 0x0F000804: lw          $a2, 0x8($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X8);
    // 0x0F000808: ldc1        $f8, 0x978($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, 0X978);
    // 0x0F00080C: lwc1        $f6, 0x68($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X68);
    // 0x0F000810: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x0F000814: c.le.d      $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f10.d <= ctx->f8.d;
    // 0x0F000818: nop

    // 0x0F00081C: bc1f        L_0F000830
    if (!c1cs) {
        // 0x0F000820: nop
    
            goto L_0F000830;
    }
    // 0x0F000820: nop

    // 0x0F000824: lw          $t2, 0x934($t2)
    ctx->r10 = MEM_W(ctx->r10, 0X934);
    // 0x0F000828: b           L_0F000838
    // 0x0F00082C: sw          $t2, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r10;
        goto L_0F000838;
    // 0x0F00082C: sw          $t2, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r10;
L_0F000830:
    // 0x0F000830: lw          $t3, 0x930($t3)
    ctx->r11 = MEM_W(ctx->r11, 0X930);
    // 0x0F000834: sw          $t3, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r11;
L_0F000838:
    // 0x0F000838: lh          $v1, 0x18($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X18);
L_0F00083C:
    // 0x0F00083C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F000840: beq         $v1, $zero, L_0F00091C
    if (ctx->r3 == 0) {
        // 0x0F000844: nop
    
            goto L_0F00091C;
    }
    // 0x0F000844: nop

    // 0x0F000848: lwc1        $f18, -0x7D38($at)
    ctx->f18.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F00084C: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F000850: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000854: mtc1        $v1, $f16
    ctx->f16.u32l = ctx->r3;
    // 0x0F000858: mtc1        $zero, $f3
    ctx->f_odd[(3 - 1) * 2] = 0;
    // 0x0F00085C: div.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = DIV_S(ctx->f18.fl, ctx->f4.fl);
    // 0x0F000860: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x0F000864: cvt.s.w     $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    ctx->f0.fl = CVT_S_W(ctx->f16.u32l);
    // 0x0F000868: div.s       $f8, $f0, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = DIV_S(ctx->f0.fl, ctx->f6.fl);
    // 0x0F00086C: sub.s       $f10, $f0, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f0.fl - ctx->f8.fl;
    // 0x0F000870: trunc.w.s   $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.u32l = TRUNC_W_S(ctx->f10.fl);
    // 0x0F000874: mfc1        $t5, $f16
    ctx->r13 = (int32_t)ctx->f16.u32l;
    // 0x0F000878: nop

    // 0x0F00087C: sh          $t5, 0x18($v0)
    MEM_H(0X18, ctx->r2) = ctx->r13;
    // 0x0F000880: lh          $t6, 0x18($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X18);
    // 0x0F000884: mtc1        $t6, $f18
    ctx->f18.u32l = ctx->r14;
    // 0x0F000888: nop

    // 0x0F00088C: cvt.d.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.d = CVT_D_W(ctx->f18.u32l);
    // 0x0F000890: c.lt.d      $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f4.d < ctx->f2.d;
    // 0x0F000894: nop

    // 0x0F000898: bc1fl       L_0F0008A8
    if (!c1cs) {
        // 0x0F00089C: lw          $a2, 0x8($v0)
        ctx->r6 = MEM_W(ctx->r2, 0X8);
            goto L_0F0008A8;
    }
    goto skip_1;
    // 0x0F00089C: lw          $a2, 0x8($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X8);
    skip_1:
    // 0x0F0008A0: sh          $zero, 0x18($v0)
    MEM_H(0X18, ctx->r2) = 0;
    // 0x0F0008A4: lw          $a2, 0x8($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X8);
L_0F0008A8:
    // 0x0F0008A8: lui         $at, 0x42C6
    ctx->r1 = S32(0X42C6 << 16);
    // 0x0F0008AC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0008B0: lwc1        $f6, 0x68($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X68);
    // 0x0F0008B4: lwc1        $f16, 0x50($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X50);
    // 0x0F0008B8: lw          $t7, 0xC($v0)
    ctx->r15 = MEM_W(ctx->r2, 0XC);
    // 0x0F0008BC: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x0F0008C0: add.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f16.fl + ctx->f10.fl;
    // 0x0F0008C4: swc1        $f18, 0x50($t7)
    MEM_W(0X50, ctx->r15) = ctx->f18.u32l;
    // 0x0F0008C8: lw          $t9, 0x44($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X44);
    // 0x0F0008CC: lh          $t8, 0x18($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X18);
    // 0x0F0008D0: lw          $v1, 0xC($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XC);
    // 0x0F0008D4: mtc1        $t9, $f8
    ctx->f8.u32l = ctx->r25;
    // 0x0F0008D8: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x0F0008DC: cvt.s.w     $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    ctx->f16.fl = CVT_S_W(ctx->f8.u32l);
    // 0x0F0008E0: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x0F0008E4: div.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f16.fl);
    // 0x0F0008E8: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x0F0008EC: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x0F0008F0: swc1        $f4, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f4.u32l;
    // 0x0F0008F4: lw          $v1, 0xC($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XC);
    // 0x0F0008F8: lwc1        $f8, 0x68($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X68);
    // 0x0F0008FC: cvt.d.s     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f6.d = CVT_D_S(ctx->f8.fl);
    // 0x0F000900: c.lt.d      $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f6.d < ctx->f2.d;
    // 0x0F000904: nop

    // 0x0F000908: bc1f        L_0F00091C
    if (!c1cs) {
        // 0x0F00090C: nop
    
            goto L_0F00091C;
    }
    // 0x0F00090C: nop

    // 0x0F000910: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x0F000914: nop

    // 0x0F000918: swc1        $f16, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f16.u32l;
L_0F00091C:
    // 0x0F00091C: jr          $ra
    // 0x0F000920: sh          $a1, 0x14($v0)
    MEM_H(0X14, ctx->r2) = ctx->r5;
    return;
    // 0x0F000920: sh          $a1, 0x14($v0)
    MEM_H(0X14, ctx->r2) = ctx->r5;
;}
RECOMP_FUNC void ni_ovl_128_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0x5A0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X5A0);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_128_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x0F00008C: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x0F000090: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x0F000094: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x0F000098: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00009C: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    // 0x0F0000A0: jalr        $t9
    // 0x0F0000A4: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0000A4: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    after_0:
    // 0x0F0000A8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000AC: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x0F0000B0: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F0000B4: or          $s2, $v0, $zero
    ctx->r18 = ctx->r2 | 0;
    // 0x0F0000B8: lw          $a1, 0x27C0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27C0);
    // 0x0F0000BC: jalr        $t9
    // 0x0F0000C0: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000C0: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_1:
    // 0x0F0000C4: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0000C8: lwc1        $f0, 0x5B0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X5B0);
    // 0x0F0000CC: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x0F0000D0: sw          $v0, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r2;
    // 0x0F0000D4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F0000D8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0000DC: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0000E0: lui         $at, 0x4290
    ctx->r1 = S32(0X4290 << 16);
    // 0x0F0000E4: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0000E8: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x0F0000EC: lui         $t6, 0x600
    ctx->r14 = S32(0X600 << 16);
    // 0x0F0000F0: addiu       $s3, $zero, 0xD6
    ctx->r19 = ADD32(0, 0XD6);
    // 0x0F0000F4: addiu       $s4, $zero, -0x1
    ctx->r20 = ADD32(0, -0X1);
    // 0x0F0000F8: addiu       $t6, $t6, 0x600
    ctx->r14 = ADD32(ctx->r14, 0X600);
    // 0x0F0000FC: ori         $t8, $t7, 0x800
    ctx->r24 = ctx->r15 | 0X800;
    // 0x0F000100: sw          $t6, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r14;
    // 0x0F000104: sw          $s3, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r19;
    // 0x0F000108: sh          $t8, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r24;
    // 0x0F00010C: sw          $s4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r20;
    // 0x0F000110: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x0F000114: swc1        $f0, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f0.u32l;
    // 0x0F000118: swc1        $f4, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f4.u32l;
    // 0x0F00011C: swc1        $f6, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f6.u32l;
    // 0x0F000120: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    // 0x0F000124: lw          $v1, 0x44($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X44);
    // 0x0F000128: lui         $s1, 0x8000
    ctx->r17 = S32(0X8000 << 16);
    // 0x0F00012C: addiu       $s1, $s1, 0x5ABC
    ctx->r17 = ADD32(ctx->r17, 0X5ABC);
    // 0x0F000130: beq         $v1, $zero, L_0F000154
    if (ctx->r3 == 0) {
        // 0x0F000134: addiu       $a0, $zero, 0x4
        ctx->r4 = ADD32(0, 0X4);
            goto L_0F000154;
    }
    // 0x0F000134: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x0F000138: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x0F00013C: beq         $v1, $at, L_0F000164
    if (ctx->r3 == ctx->r1) {
        // 0x0F000140: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_0F000164;
    }
    // 0x0F000140: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x0F000144: beql        $v1, $at, L_0F000178
    if (ctx->r3 == ctx->r1) {
        // 0x0F000148: lui         $at, 0xC260
        ctx->r1 = S32(0XC260 << 16);
            goto L_0F000178;
    }
    goto skip_0;
    // 0x0F000148: lui         $at, 0xC260
    ctx->r1 = S32(0XC260 << 16);
    skip_0:
    // 0x0F00014C: b           L_0F000188
    // 0x0F000150: lw          $t0, 0x40($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X40);
        goto L_0F000188;
    // 0x0F000150: lw          $t0, 0x40($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X40);
L_0F000154:
    // 0x0F000154: lui         $at, 0x428A
    ctx->r1 = S32(0X428A << 16);
    // 0x0F000158: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F00015C: b           L_0F000184
    // 0x0F000160: swc1        $f10, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f10.u32l;
        goto L_0F000184;
    // 0x0F000160: swc1        $f10, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f10.u32l;
L_0F000164:
    // 0x0F000164: lui         $at, 0x40C0
    ctx->r1 = S32(0X40C0 << 16);
    // 0x0F000168: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F00016C: b           L_0F000184
    // 0x0F000170: swc1        $f16, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f16.u32l;
        goto L_0F000184;
    // 0x0F000170: swc1        $f16, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f16.u32l;
    // 0x0F000174: lui         $at, 0xC260
    ctx->r1 = S32(0XC260 << 16);
L_0F000178:
    // 0x0F000178: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F00017C: nop

    // 0x0F000180: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
L_0F000184:
    // 0x0F000184: lw          $t0, 0x40($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X40);
L_0F000188:
    // 0x0F000188: beq         $t0, $zero, L_0F0001A0
    if (ctx->r8 == 0) {
        // 0x0F00018C: nop
    
            goto L_0F0001A0;
    }
    // 0x0F00018C: nop

    // 0x0F000190: lh          $t1, 0x0($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X0);
    // 0x0F000194: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x0F000198: or          $t2, $t1, $at
    ctx->r10 = ctx->r9 | ctx->r1;
    // 0x0F00019C: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
L_0F0001A0:
    // 0x0F0001A0: jalr        $s1
    // 0x0F0001A4: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_2;
    // 0x0F0001A4: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    after_2:
    // 0x0F0001A8: sw          $v0, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r2;
    // 0x0F0001AC: lui         $at, 0xC1C8
    ctx->r1 = S32(0XC1C8 << 16);
    // 0x0F0001B0: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F0001B4: lhu         $t4, 0x2($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0001B8: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F0001BC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0001C0: lui         $t3, 0x600
    ctx->r11 = S32(0X600 << 16);
    // 0x0F0001C4: addiu       $t3, $t3, 0x6D8
    ctx->r11 = ADD32(ctx->r11, 0X6D8);
    // 0x0F0001C8: ori         $t5, $t4, 0x800
    ctx->r13 = ctx->r12 | 0X800;
    // 0x0F0001CC: sw          $t3, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r11;
    // 0x0F0001D0: sh          $t5, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r13;
    // 0x0F0001D4: sw          $s4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r20;
    // 0x0F0001D8: sw          $s3, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r19;
    // 0x0F0001DC: swc1        $f4, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f4.u32l;
    // 0x0F0001E0: swc1        $f6, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f6.u32l;
    // 0x0F0001E4: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    // 0x0F0001E8: jalr        $s1
    // 0x0F0001EC: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_3;
    // 0x0F0001EC: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_3:
    // 0x0F0001F0: sw          $v0, 0x8($s2)
    MEM_W(0X8, ctx->r18) = ctx->r2;
    // 0x0F0001F4: lui         $at, 0xC170
    ctx->r1 = S32(0XC170 << 16);
    // 0x0F0001F8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0001FC: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000200: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F000204: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000208: lui         $t9, 0x600
    ctx->r25 = S32(0X600 << 16);
    // 0x0F00020C: addiu       $t9, $t9, 0x6D8
    ctx->r25 = ADD32(ctx->r25, 0X6D8);
    // 0x0F000210: ori         $t7, $t6, 0x800
    ctx->r15 = ctx->r14 | 0X800;
    // 0x0F000214: sw          $t9, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r25;
    // 0x0F000218: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x0F00021C: sw          $s4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r20;
    // 0x0F000220: sw          $s3, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r19;
    // 0x0F000224: swc1        $f8, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f8.u32l;
    // 0x0F000228: swc1        $f10, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f10.u32l;
    // 0x0F00022C: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    // 0x0F000230: jalr        $s1
    // 0x0F000234: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_4;
    // 0x0F000234: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_4:
    // 0x0F000238: sw          $v0, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r2;
    // 0x0F00023C: lui         $at, 0xC0A0
    ctx->r1 = S32(0XC0A0 << 16);
    // 0x0F000240: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000244: lhu         $t0, 0x2($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000248: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F00024C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000250: lui         $t8, 0x600
    ctx->r24 = S32(0X600 << 16);
    // 0x0F000254: addiu       $t8, $t8, 0x6D8
    ctx->r24 = ADD32(ctx->r24, 0X6D8);
    // 0x0F000258: ori         $t1, $t0, 0x800
    ctx->r9 = ctx->r8 | 0X800;
    // 0x0F00025C: sw          $t8, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r24;
    // 0x0F000260: sh          $t1, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r9;
    // 0x0F000264: sw          $s4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r20;
    // 0x0F000268: sw          $s3, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r19;
    // 0x0F00026C: swc1        $f16, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f16.u32l;
    // 0x0F000270: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x0F000274: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    // 0x0F000278: jalr        $s1
    // 0x0F00027C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_5;
    // 0x0F00027C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_5:
    // 0x0F000280: sw          $v0, 0x10($s2)
    MEM_W(0X10, ctx->r18) = ctx->r2;
    // 0x0F000284: lui         $at, 0x40A0
    ctx->r1 = S32(0X40A0 << 16);
    // 0x0F000288: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F00028C: lhu         $t3, 0x2($v0)
    ctx->r11 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000290: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F000294: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000298: lui         $t2, 0x600
    ctx->r10 = S32(0X600 << 16);
    // 0x0F00029C: addiu       $t2, $t2, 0x6D8
    ctx->r10 = ADD32(ctx->r10, 0X6D8);
    // 0x0F0002A0: ori         $t4, $t3, 0x800
    ctx->r12 = ctx->r11 | 0X800;
    // 0x0F0002A4: sw          $t2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r10;
    // 0x0F0002A8: sw          $s3, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r19;
    // 0x0F0002AC: sh          $t4, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r12;
    // 0x0F0002B0: sw          $s4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r20;
    // 0x0F0002B4: swc1        $f4, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f4.u32l;
    // 0x0F0002B8: swc1        $f6, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f6.u32l;
    // 0x0F0002BC: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    // 0x0F0002C0: jalr        $s1
    // 0x0F0002C4: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_6;
    // 0x0F0002C4: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_6:
    // 0x0F0002C8: sw          $v0, 0x14($s2)
    MEM_W(0X14, ctx->r18) = ctx->r2;
    // 0x0F0002CC: lui         $at, 0x4170
    ctx->r1 = S32(0X4170 << 16);
    // 0x0F0002D0: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0002D4: lhu         $t9, 0x2($v0)
    ctx->r25 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0002D8: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F0002DC: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0002E0: lui         $t5, 0x600
    ctx->r13 = S32(0X600 << 16);
    // 0x0F0002E4: addiu       $t5, $t5, 0x6D8
    ctx->r13 = ADD32(ctx->r13, 0X6D8);
    // 0x0F0002E8: ori         $t6, $t9, 0x800
    ctx->r14 = ctx->r25 | 0X800;
    // 0x0F0002EC: sw          $t5, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r13;
    // 0x0F0002F0: sw          $s3, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r19;
    // 0x0F0002F4: sh          $t6, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r14;
    // 0x0F0002F8: sw          $s4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r20;
    // 0x0F0002FC: swc1        $f8, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f8.u32l;
    // 0x0F000300: swc1        $f10, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f10.u32l;
    // 0x0F000304: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    // 0x0F000308: jalr        $s1
    // 0x0F00030C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_7;
    // 0x0F00030C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_7:
    // 0x0F000310: sw          $v0, 0x18($s2)
    MEM_W(0X18, ctx->r18) = ctx->r2;
    // 0x0F000314: lui         $at, 0x41C8
    ctx->r1 = S32(0X41C8 << 16);
    // 0x0F000318: lhu         $t8, 0x2($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X2);
    // 0x0F00031C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000320: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x0F000324: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000328: lui         $t7, 0x600
    ctx->r15 = S32(0X600 << 16);
    // 0x0F00032C: addiu       $t7, $t7, 0x6D8
    ctx->r15 = ADD32(ctx->r15, 0X6D8);
    // 0x0F000330: ori         $t0, $t8, 0x800
    ctx->r8 = ctx->r24 | 0X800;
    // 0x0F000334: sw          $t7, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r15;
    // 0x0F000338: sw          $s3, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r19;
    // 0x0F00033C: sh          $t0, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r8;
    // 0x0F000340: sw          $s4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r20;
    // 0x0F000344: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000348: swc1        $f16, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f16.u32l;
    // 0x0F00034C: jal         0x0F000468
    // 0x0F000350: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    static_182_0F000468(rdram, ctx);
        goto after_8;
    // 0x0F000350: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    after_8:
    // 0x0F000354: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000358: addiu       $t1, $zero, 0x2
    ctx->r9 = ADD32(0, 0X2);
    // 0x0F00035C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000360: sw          $t1, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r9;
    // 0x0F000364: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000368: jalr        $t9
    // 0x0F00036C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F00036C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_9:
    // 0x0F000370: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000374: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000378: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x0F00037C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x0F000380: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x0F000384: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x0F000388: jr          $ra
    // 0x0F00038C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x0F00038C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void ni_ovl_128_func_0F000390(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000390: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F000394: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000398: lw          $v0, 0x3C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X3C);
    // 0x0F00039C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x0F0003A0: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0003A4: beq         $v0, $at, L_0F0003C0
    if (ctx->r2 == ctx->r1) {
        // 0x0F0003A8: lw          $v1, 0x38($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X38);
            goto L_0F0003C0;
    }
    // 0x0F0003A8: lw          $v1, 0x38($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X38);
    // 0x0F0003AC: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x0F0003B0: beql        $v0, $at, L_0F0003E4
    if (ctx->r2 == ctx->r1) {
        // 0x0F0003B4: lw          $t7, 0x40($a2)
        ctx->r15 = MEM_W(ctx->r6, 0X40);
            goto L_0F0003E4;
    }
    goto skip_0;
    // 0x0F0003B4: lw          $t7, 0x40($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X40);
    skip_0:
    // 0x0F0003B8: b           L_0F0003E4
    // 0x0F0003BC: lw          $t7, 0x40($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X40);
        goto L_0F0003E4;
    // 0x0F0003BC: lw          $t7, 0x40($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X40);
L_0F0003C0:
    // 0x0F0003C0: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x0F0003C4: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x0F0003C8: jal         0x0F000468
    // 0x0F0003CC: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    static_182_0F000468(rdram, ctx);
        goto after_0;
    // 0x0F0003CC: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_0:
    // 0x0F0003D0: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x0F0003D4: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x0F0003D8: addiu       $t6, $zero, 0x2
    ctx->r14 = ADD32(0, 0X2);
    // 0x0F0003DC: sw          $t6, 0x3C($a2)
    MEM_W(0X3C, ctx->r6) = ctx->r14;
    // 0x0F0003E0: lw          $t7, 0x40($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X40);
L_0F0003E4:
    // 0x0F0003E4: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F0003E8: beql        $t7, $zero, L_0F00040C
    if (ctx->r15 == 0) {
        // 0x0F0003EC: lw          $v0, 0x0($v1)
        ctx->r2 = MEM_W(ctx->r3, 0X0);
            goto L_0F00040C;
    }
    goto skip_1;
    // 0x0F0003EC: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    skip_1:
    // 0x0F0003F0: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x0F0003F4: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x0F0003F8: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x0F0003FC: or          $t9, $t8, $at
    ctx->r25 = ctx->r24 | ctx->r1;
    // 0x0F000400: b           L_0F000418
    // 0x0F000404: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
        goto L_0F000418;
    // 0x0F000404: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
    // 0x0F000408: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
L_0F00040C:
    // 0x0F00040C: lh          $t0, 0x0($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X0);
    // 0x0F000410: andi        $t1, $t0, 0x7FFF
    ctx->r9 = ctx->r8 & 0X7FFF;
    // 0x0F000414: sh          $t1, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r9;
L_0F000418:
    // 0x0F000418: lw          $t2, 0x3C($a2)
    ctx->r10 = MEM_W(ctx->r6, 0X3C);
    // 0x0F00041C: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x0F000420: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000424: bne         $t2, $at, L_0F000434
    if (ctx->r10 != ctx->r1) {
        // 0x0F000428: addiu       $t9, $t9, 0x1CE8
        ctx->r25 = ADD32(ctx->r25, 0X1CE8);
            goto L_0F000434;
    }
    // 0x0F000428: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F00042C: jalr        $t9
    // 0x0F000430: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000430: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_0F000434:
    // 0x0F000434: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000438: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F00043C: jr          $ra
    // 0x0F000440: nop

    return;
    // 0x0F000440: nop

;}
RECOMP_FUNC void ni_ovl_128_func_0F000444(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000444: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000448: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F00044C: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F000450: jalr        $t9
    // 0x0F000454: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000454: nop

    after_0:
    // 0x0F000458: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F00045C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000460: jr          $ra
    // 0x0F000464: nop

    return;
    // 0x0F000464: nop

    // 0x0F000468: lw          $t6, 0x48($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X48);
    // 0x0F00046C: lw          $v0, 0x38($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X38);
    // 0x0F000470: beql        $t6, $zero, L_0F000494
    if (ctx->r14 == 0) {
        // 0x0F000474: lw          $v1, 0x4($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X4);
            goto L_0F000494;
    }
    goto skip_0;
    // 0x0F000474: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    skip_0:
    // 0x0F000478: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    // 0x0F00047C: addiu       $a1, $zero, -0x8000
    ctx->r5 = ADD32(0, -0X8000);
    // 0x0F000480: lh          $t7, 0x0($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X0);
    // 0x0F000484: andi        $t8, $t7, 0x7FFF
    ctx->r24 = ctx->r15 & 0X7FFF;
    // 0x0F000488: b           L_0F0004A4
    // 0x0F00048C: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
        goto L_0F0004A4;
    // 0x0F00048C: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
    // 0x0F000490: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
L_0F000494:
    // 0x0F000494: addiu       $a1, $zero, -0x8000
    ctx->r5 = ADD32(0, -0X8000);
    // 0x0F000498: lh          $t9, 0x0($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X0);
    // 0x0F00049C: or          $t0, $t9, $a1
    ctx->r8 = ctx->r25 | ctx->r5;
    // 0x0F0004A0: sh          $t0, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r8;
L_0F0004A4:
    // 0x0F0004A4: lw          $t1, 0x4C($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X4C);
    // 0x0F0004A8: beql        $t1, $zero, L_0F0004C8
    if (ctx->r9 == 0) {
        // 0x0F0004AC: lw          $v1, 0x8($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X8);
            goto L_0F0004C8;
    }
    goto skip_1;
    // 0x0F0004AC: lw          $v1, 0x8($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X8);
    skip_1:
    // 0x0F0004B0: lw          $v1, 0x8($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X8);
    // 0x0F0004B4: lh          $t2, 0x0($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X0);
    // 0x0F0004B8: andi        $t3, $t2, 0x7FFF
    ctx->r11 = ctx->r10 & 0X7FFF;
    // 0x0F0004BC: b           L_0F0004D4
    // 0x0F0004C0: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
        goto L_0F0004D4;
    // 0x0F0004C0: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
    // 0x0F0004C4: lw          $v1, 0x8($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X8);
L_0F0004C8:
    // 0x0F0004C8: lh          $t4, 0x0($v1)
    ctx->r12 = MEM_H(ctx->r3, 0X0);
    // 0x0F0004CC: or          $t5, $t4, $a1
    ctx->r13 = ctx->r12 | ctx->r5;
    // 0x0F0004D0: sh          $t5, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r13;
L_0F0004D4:
    // 0x0F0004D4: lw          $t6, 0x50($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X50);
    // 0x0F0004D8: beql        $t6, $zero, L_0F0004F8
    if (ctx->r14 == 0) {
        // 0x0F0004DC: lw          $v1, 0xC($v0)
        ctx->r3 = MEM_W(ctx->r2, 0XC);
            goto L_0F0004F8;
    }
    goto skip_2;
    // 0x0F0004DC: lw          $v1, 0xC($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XC);
    skip_2:
    // 0x0F0004E0: lw          $v1, 0xC($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XC);
    // 0x0F0004E4: lh          $t7, 0x0($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X0);
    // 0x0F0004E8: andi        $t8, $t7, 0x7FFF
    ctx->r24 = ctx->r15 & 0X7FFF;
    // 0x0F0004EC: b           L_0F000504
    // 0x0F0004F0: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
        goto L_0F000504;
    // 0x0F0004F0: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
    // 0x0F0004F4: lw          $v1, 0xC($v0)
    ctx->r3 = MEM_W(ctx->r2, 0XC);
L_0F0004F8:
    // 0x0F0004F8: lh          $t9, 0x0($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X0);
    // 0x0F0004FC: or          $t0, $t9, $a1
    ctx->r8 = ctx->r25 | ctx->r5;
    // 0x0F000500: sh          $t0, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r8;
L_0F000504:
    // 0x0F000504: lw          $t1, 0x54($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X54);
    // 0x0F000508: beql        $t1, $zero, L_0F000528
    if (ctx->r9 == 0) {
        // 0x0F00050C: lw          $v1, 0x10($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X10);
            goto L_0F000528;
    }
    goto skip_3;
    // 0x0F00050C: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
    skip_3:
    // 0x0F000510: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
    // 0x0F000514: lh          $t2, 0x0($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X0);
    // 0x0F000518: andi        $t3, $t2, 0x7FFF
    ctx->r11 = ctx->r10 & 0X7FFF;
    // 0x0F00051C: b           L_0F000534
    // 0x0F000520: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
        goto L_0F000534;
    // 0x0F000520: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
    // 0x0F000524: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
L_0F000528:
    // 0x0F000528: lh          $t4, 0x0($v1)
    ctx->r12 = MEM_H(ctx->r3, 0X0);
    // 0x0F00052C: or          $t5, $t4, $a1
    ctx->r13 = ctx->r12 | ctx->r5;
    // 0x0F000530: sh          $t5, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r13;
L_0F000534:
    // 0x0F000534: lw          $t6, 0x58($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X58);
    // 0x0F000538: beql        $t6, $zero, L_0F000558
    if (ctx->r14 == 0) {
        // 0x0F00053C: lw          $v1, 0x14($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X14);
            goto L_0F000558;
    }
    goto skip_4;
    // 0x0F00053C: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
    skip_4:
    // 0x0F000540: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
    // 0x0F000544: lh          $t7, 0x0($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X0);
    // 0x0F000548: andi        $t8, $t7, 0x7FFF
    ctx->r24 = ctx->r15 & 0X7FFF;
    // 0x0F00054C: b           L_0F000564
    // 0x0F000550: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
        goto L_0F000564;
    // 0x0F000550: sh          $t8, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r24;
    // 0x0F000554: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
L_0F000558:
    // 0x0F000558: lh          $t9, 0x0($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X0);
    // 0x0F00055C: or          $t0, $t9, $a1
    ctx->r8 = ctx->r25 | ctx->r5;
    // 0x0F000560: sh          $t0, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r8;
L_0F000564:
    // 0x0F000564: lw          $t1, 0x5C($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X5C);
    // 0x0F000568: beql        $t1, $zero, L_0F000588
    if (ctx->r9 == 0) {
        // 0x0F00056C: lw          $v1, 0x18($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X18);
            goto L_0F000588;
    }
    goto skip_5;
    // 0x0F00056C: lw          $v1, 0x18($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X18);
    skip_5:
    // 0x0F000570: lw          $v1, 0x18($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X18);
    // 0x0F000574: lh          $t2, 0x0($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X0);
    // 0x0F000578: andi        $t3, $t2, 0x7FFF
    ctx->r11 = ctx->r10 & 0X7FFF;
    // 0x0F00057C: jr          $ra
    // 0x0F000580: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
    return;
    // 0x0F000580: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
    // 0x0F000584: lw          $v1, 0x18($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X18);
L_0F000588:
    // 0x0F000588: lh          $t4, 0x0($v1)
    ctx->r12 = MEM_H(ctx->r3, 0X0);
    // 0x0F00058C: or          $t5, $t4, $a1
    ctx->r13 = ctx->r12 | ctx->r5;
    // 0x0F000590: sh          $t5, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r13;
    // 0x0F000594: jr          $ra
    // 0x0F000598: nop

    return;
    // 0x0F000598: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0xD88($t9)
    ctx->r25 = MEM_W(ctx->r25, 0XD88);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F00007C: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000080: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000084: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000088: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x0F00008C: jalr        $t9
    // 0x0F000090: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000090: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    after_0:
    // 0x0F000094: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x0F000098: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x0F00009C: lh          $t6, -0x54E2($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54E2);
    // 0x0F0000A0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000A4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0000A8: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x0F0000AC: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    // 0x0F0000B0: jalr        $t9
    // 0x0F0000B4: sw          $t6, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000B4: sw          $t6, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = ctx->r14;
    after_1:
    // 0x0F0000B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F0000BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0000C0: jr          $ra
    // 0x0F0000C4: nop

    return;
    // 0x0F0000C4: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F0000C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0000C8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x0F0000CC: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F0000D0: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F0000D4: lh          $t7, 0x2B4E($a2)
    ctx->r15 = MEM_H(ctx->r6, 0X2B4E);
    // 0x0F0000D8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F0000DC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F0000E0: lw          $t6, 0x38($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X38);
    // 0x0F0000E4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_daygate_entry_probe(rdram, ctx->r16);
#endif
    // 0x0F0000E8: bne         $t7, $zero, L_0F000230
    if (ctx->r15 != 0) {
        // 0x0F0000EC: sw          $t6, 0x24($sp)
        MEM_W(0X24, ctx->r29) = ctx->r14;
#if LOD_ENABLE_NI0E_TRACE
        lod_ni0e_daygate_skip(&lod_ni0e_daygate_skip_busy, "busy");
#endif
            goto L_0F000230;
    }
    // 0x0F0000EC: sw          $t6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r14;
    // 0x0F0000F0: lw          $t8, 0x2BC8($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X2BC8);
    // 0x0F0000F4: bnel        $t8, $zero, L_0F000234
    if (ctx->r24 != 0) {
        // 0x0F0000F8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
#if LOD_ENABLE_NI0E_TRACE
        lod_ni0e_daygate_skip(&lod_ni0e_daygate_skip_locked, "locked");
#endif
            goto L_0F000234;
    }
    goto skip_0;
    // 0x0F0000F8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x0F0000FC: jal         0x0F000B7C
    // 0x0F000100: nop

    ni_ovl_129_func_0F000B7C(rdram, ctx);
        goto after_0;
    // 0x0F000100: nop

    after_0:
    // 0x0F000104: beq         $v0, $zero, L_0F00012C
    if (ctx->r2 == 0) {
        // 0x0F000108: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_0F00012C;
    }
    // 0x0F000108: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F00010C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000110: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x0F000114: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000118: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F00011C: jalr        $t9
    // 0x0F000120: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000120: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    after_1:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_daygate_skip(&lod_ni0e_daygate_skip_state5, "state5");
#endif
    // 0x0F000124: b           L_0F000234
    // 0x0F000128: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F000234;
    // 0x0F000128: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F00012C:
    // 0x0F00012C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000130: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x0F000134: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x0F000138: jalr        $t9
    // 0x0F00013C: addiu       $a1, $zero, 0x2A0
    ctx->r5 = ADD32(0, 0X2A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00013C: addiu       $a1, $zero, 0x2A0
    ctx->r5 = ADD32(0, 0X2A0);
    after_2:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_daygate_flagtest_probe((uint32_t)ctx->r2);
#endif
    // 0x0F000140: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F000144: beq         $v0, $zero, L_0F000168
    if (ctx->r2 == 0) {
        // 0x0F000148: addiu       $a2, $a2, -0x7D40
        ctx->r6 = ADD32(ctx->r6, -0X7D40);
            goto L_0F000168;
    }
    // 0x0F000148: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F00014C: lh          $t0, 0x285E($a2)
    ctx->r8 = MEM_H(ctx->r6, 0X285E);
    // 0x0F000150: lw          $t1, 0x3C($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X3C);
    // 0x0F000154: bnel        $t0, $t1, L_0F00016C
    if (ctx->r8 != ctx->r9) {
        // 0x0F000158: lh          $t3, 0x285C($a2)
        ctx->r11 = MEM_H(ctx->r6, 0X285C);
            goto L_0F00016C;
    }
    goto skip_1;
    // 0x0F000158: lh          $t3, 0x285C($a2)
    ctx->r11 = MEM_H(ctx->r6, 0X285C);
    skip_1:
    // 0x0F00015C: lh          $t2, 0x285C($a2)
    ctx->r10 = MEM_H(ctx->r6, 0X285C);
    // 0x0F000160: beql        $t2, $zero, L_0F000234
    if (ctx->r10 == 0) {
        // 0x0F000164: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
#if LOD_ENABLE_NI0E_TRACE
        lod_ni0e_daygate_skip(&lod_ni0e_daygate_skip_hour_unchanged, "hour_unchanged");
#endif
            goto L_0F000234;
    }
    goto skip_2;
    // 0x0F000164: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
L_0F000168:
    // 0x0F000168: lh          $t3, 0x285C($a2)
    ctx->r11 = MEM_H(ctx->r6, 0X285C);
L_0F00016C:
    // 0x0F00016C: addiu       $t4, $zero, 0x7
    ctx->r12 = ADD32(0, 0X7);
    // 0x0F000170: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000174: beq         $t3, $zero, L_0F000184
    if (ctx->r11 == 0) {
        // 0x0F000178: lui         $v1, 0x8008
        ctx->r3 = S32(0X8008 << 16);
            goto L_0F000184;
    }
    // 0x0F000178: lui         $v1, 0x8008
    ctx->r3 = S32(0X8008 << 16);
    // 0x0F00017C: b           L_0F00018C
    // 0x0F000180: sw          $t4, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r12;
        goto L_0F00018C;
    // 0x0F000180: sw          $t4, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r12;
L_0F000184:
    // 0x0F000184: lh          $t5, 0x285E($a2)
    ctx->r13 = MEM_H(ctx->r6, 0X285E);
    // 0x0F000188: sw          $t5, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r13;
L_0F00018C:
    // 0x0F00018C: lh          $t6, 0x285C($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X285C);
    // 0x0F000190: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    // 0x0F000194: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x0F000198: beq         $t6, $zero, L_0F0001B8
    if (ctx->r14 == 0) {
        // 0x0F00019C: addiu       $v1, $v1, 0x2FE0
        ctx->r3 = ADD32(ctx->r3, 0X2FE0);
            goto L_0F0001B8;
    }
    // 0x0F00019C: addiu       $v1, $v1, 0x2FE0
    ctx->r3 = ADD32(ctx->r3, 0X2FE0);
    // 0x0F0001A0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0001A4: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x0F0001A8: jalr        $t9
    // 0x0F0001AC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F0001AC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    after_3:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_daygate_skip(&lod_ni0e_daygate_skip_day_flag_set, "day_flag_set");
#endif
    // 0x0F0001B0: b           L_0F000234
    // 0x0F0001B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F000234;
    // 0x0F0001B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F0001B8:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_daygate_build_probe(ctx->r16);
#endif
    // 0x0F0001B8: jalr        $v1
    // 0x0F0001BC: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_4;
    // 0x0F0001BC: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    after_4:
    // 0x0F0001C0: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x0F0001C4: lw          $t7, 0x24($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X24);
    // 0x0F0001C8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001CC: jalr        $v1
    // 0x0F0001D0: sw          $v0, 0x4($t7)
    MEM_W(0X4, ctx->r15) = ctx->r2;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_5;
    // 0x0F0001D0: sw          $v0, 0x4($t7)
    MEM_W(0X4, ctx->r15) = ctx->r2;
    after_5:
    // 0x0F0001D4: lw          $t8, 0x24($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X24);
    // 0x0F0001D8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001DC: jal         0x0F000938
    // 0x0F0001E0: sw          $v0, 0x8($t8)
    MEM_W(0X8, ctx->r24) = ctx->r2;
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_daypop_probe(rdram, "pre-0F000938");
#endif
    ni_ovl_129_func_0F000938(rdram, ctx);
        goto after_6;
    // 0x0F0001E0: sw          $v0, 0x8($t8)
    MEM_W(0X8, ctx->r24) = ctx->r2;
    after_6:
#if LOD_ENABLE_NI0E_TRACE
    lod_ni0e_daypop_probe(rdram, "post-0F000938");
#endif
    // 0x0F0001E4: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
    // 0x0F0001E8: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F0001EC: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F0001F0: sh          $zero, 0xC($t0)
    MEM_H(0XC, ctx->r8) = 0;
    // 0x0F0001F4: sw          $zero, 0x44($s0)
    MEM_W(0X44, ctx->r16) = 0;
    // 0x0F0001F8: lh          $t1, 0x28D0($a2)
    ctx->r9 = MEM_H(ctx->r6, 0X28D0);
    // 0x0F0001FC: addiu       $at, $zero, 0x2C
    ctx->r1 = ADD32(0, 0X2C);
    // 0x0F000200: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F000204: beq         $t1, $at, L_0F00021C
    if (ctx->r9 == ctx->r1) {
        // 0x0F000208: addiu       $a0, $a0, -0x55A0
        ctx->r4 = ADD32(ctx->r4, -0X55A0);
            goto L_0F00021C;
    }
    // 0x0F000208: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x0F00020C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000210: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x0F000214: jalr        $t9
    // 0x0F000218: addiu       $a1, $zero, 0x2A0
    ctx->r5 = ADD32(0, 0X2A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F000218: addiu       $a1, $zero, 0x2A0
    ctx->r5 = ADD32(0, 0X2A0);
    after_7:
L_0F00021C:
    // 0x0F00021C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000220: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000224: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F000228: jalr        $t9
    // 0x0F00022C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x0F00022C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_8:
L_0F000230:
    // 0x0F000230: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F000234:
    // 0x0F000234: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000238: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x0F00023C: jr          $ra
    // 0x0F000240: nop

    return;
    // 0x0F000240: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F000244(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000244: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x0F000248: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x0F00024C: lh          $t6, 0x2B4E($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X2B4E);
    // 0x0F000250: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000254: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F000258: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x0F00025C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000260: bne         $t6, $zero, L_0F000270
    if (ctx->r14 != 0) {
        // 0x0F000264: lw          $v1, 0x38($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X38);
            goto L_0F000270;
    }
    // 0x0F000264: lw          $v1, 0x38($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X38);
    // 0x0F000268: lw          $t7, 0x2BC8($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X2BC8);
    // 0x0F00026C: beq         $t7, $zero, L_0F000290
    if (ctx->r15 == 0) {
        // 0x0F000270: addiu       $t8, $zero, 0x1
        ctx->r24 = ADD32(0, 0X1);
            goto L_0F000290;
    }
L_0F000270:
    // 0x0F000270: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F000274: sw          $t8, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r24;
    // 0x0F000278: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x0F00027C: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x0F000280: lh          $t9, 0x0($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X0);
    // 0x0F000284: or          $t0, $t9, $at
    ctx->r8 = ctx->r25 | ctx->r1;
    // 0x0F000288: b           L_0F000494
    // 0x0F00028C: sh          $t0, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r8;
        goto L_0F000494;
    // 0x0F00028C: sh          $t0, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r8;
L_0F000290:
    // 0x0F000290: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x0F000294: lui         $t3, 0x801B
    ctx->r11 = S32(0X801B << 16);
    // 0x0F000298: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00029C: lh          $t1, 0x0($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X0);
    // 0x0F0002A0: andi        $t2, $t1, 0x7FFF
    ctx->r10 = ctx->r9 & 0X7FFF;
    // 0x0F0002A4: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
    // 0x0F0002A8: sw          $zero, 0x40($s0)
    MEM_W(0X40, ctx->r16) = 0;
    // 0x0F0002AC: lh          $t3, 0x1D48($t3)
    ctx->r11 = MEM_H(ctx->r11, 0X1D48);
    // 0x0F0002B0: bnel        $t3, $zero, L_0F000498
    if (ctx->r11 != 0) {
        // 0x0F0002B4: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_0F000498;
    }
    goto skip_0;
    // 0x0F0002B4: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_0:
    // 0x0F0002B8: lhu         $t4, 0xC($v1)
    ctx->r12 = MEM_HU(ctx->r3, 0XC);
    // 0x0F0002BC: lwc1        $f6, 0x8($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X8);
    // 0x0F0002C0: ldc1        $f4, 0xDA0($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, 0XDA0);
    // 0x0F0002C4: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x0F0002C8: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x0F0002CC: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x0F0002D0: div.d       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = DIV_D(ctx->f4.d, ctx->f8.d);
    // 0x0F0002D4: bgez        $t4, L_0F0002EC
    if (SIGNED(ctx->r12) >= 0) {
        // 0x0F0002D8: cvt.d.w     $f18, $f16
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
            goto L_0F0002EC;
    }
    // 0x0F0002D8: cvt.d.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
    // 0x0F0002DC: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x0F0002E0: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F0002E4: nop

    // 0x0F0002E8: add.d       $f18, $f18, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f6.d); 
    ctx->f18.d = ctx->f18.d + ctx->f6.d;
L_0F0002EC:
    // 0x0F0002EC: add.d       $f4, $f18, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f10.d); 
    ctx->f4.d = ctx->f18.d + ctx->f10.d;
    // 0x0F0002F0: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x0F0002F4: lui         $t4, 0xF00
    ctx->r12 = S32(0XF00 << 16);
    // 0x0F0002F8: lui         $at, 0x41E0
    ctx->r1 = S32(0X41E0 << 16);
    // 0x0F0002FC: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F000300: ctc1        $t6, $FpcCsr
    set_cop1_cs(ctx->r14);
    // 0x0F000304: addiu       $t9, $zero, 0x7FFF
    ctx->r25 = ADD32(0, 0X7FFF);
    // 0x0F000308: addiu       $t4, $t4, 0xC30
    ctx->r12 = ADD32(ctx->r12, 0XC30);
    // 0x0F00030C: cvt.w.d     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.u32l = CVT_W_D(ctx->f4.d);
    // 0x0F000310: cfc1        $t6, $FpcCsr
    ctx->r14 = get_cop1_cs();
    // 0x0F000314: nop

    // 0x0F000318: andi        $t6, $t6, 0x78
    ctx->r14 = ctx->r14 & 0X78;
    // 0x0F00031C: beql        $t6, $zero, L_0F000370
    if (ctx->r14 == 0) {
        // 0x0F000320: mfc1        $t6, $f8
        ctx->r14 = (int32_t)ctx->f8.u32l;
            goto L_0F000370;
    }
    goto skip_1;
    // 0x0F000320: mfc1        $t6, $f8
    ctx->r14 = (int32_t)ctx->f8.u32l;
    skip_1:
    // 0x0F000324: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x0F000328: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x0F00032C: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x0F000330: sub.d       $f8, $f4, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f8.d); 
    ctx->f8.d = ctx->f4.d - ctx->f8.d;
    // 0x0F000334: ctc1        $t6, $FpcCsr
    set_cop1_cs(ctx->r14);
    // 0x0F000338: nop

    // 0x0F00033C: cvt.w.d     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.u32l = CVT_W_D(ctx->f8.d);
    // 0x0F000340: cfc1        $t6, $FpcCsr
    ctx->r14 = get_cop1_cs();
    // 0x0F000344: nop

    // 0x0F000348: andi        $t6, $t6, 0x78
    ctx->r14 = ctx->r14 & 0X78;
    // 0x0F00034C: bne         $t6, $zero, L_0F000364
    if (ctx->r14 != 0) {
        // 0x0F000350: nop
    
            goto L_0F000364;
    }
    // 0x0F000350: nop

    // 0x0F000354: mfc1        $t6, $f8
    ctx->r14 = (int32_t)ctx->f8.u32l;
    // 0x0F000358: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F00035C: b           L_0F00037C
    // 0x0F000360: or          $t6, $t6, $at
    ctx->r14 = ctx->r14 | ctx->r1;
        goto L_0F00037C;
    // 0x0F000360: or          $t6, $t6, $at
    ctx->r14 = ctx->r14 | ctx->r1;
L_0F000364:
    // 0x0F000364: b           L_0F00037C
    // 0x0F000368: addiu       $t6, $zero, -0x1
    ctx->r14 = ADD32(0, -0X1);
        goto L_0F00037C;
    // 0x0F000368: addiu       $t6, $zero, -0x1
    ctx->r14 = ADD32(0, -0X1);
    // 0x0F00036C: mfc1        $t6, $f8
    ctx->r14 = (int32_t)ctx->f8.u32l;
L_0F000370:
    // 0x0F000370: nop

    // 0x0F000374: bltz        $t6, L_0F000364
    if (SIGNED(ctx->r14) < 0) {
        // 0x0F000378: nop
    
            goto L_0F000364;
    }
    // 0x0F000378: nop

L_0F00037C:
    // 0x0F00037C: andi        $v0, $t6, 0xFFFF
    ctx->r2 = ctx->r14 & 0XFFFF;
    // 0x0F000380: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F000384: slti        $at, $v0, 0x7FFF
    ctx->r1 = SIGNED(ctx->r2) < 0X7FFF ? 1 : 0;
    // 0x0F000388: beq         $at, $zero, L_0F0003AC
    if (ctx->r1 == 0) {
        // 0x0F00038C: sh          $t6, 0xC($v1)
        MEM_H(0XC, ctx->r3) = ctx->r14;
            goto L_0F0003AC;
    }
    // 0x0F00038C: sh          $t6, 0xC($v1)
    MEM_H(0XC, ctx->r3) = ctx->r14;
    // 0x0F000390: lw          $t8, 0x0($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X0);
    // 0x0F000394: bgez        $v0, L_0F0003A4
    if (SIGNED(ctx->r2) >= 0) {
        // 0x0F000398: sra         $t7, $v0, 8
        ctx->r15 = S32(SIGNED(ctx->r2) >> 8);
            goto L_0F0003A4;
    }
    // 0x0F000398: sra         $t7, $v0, 8
    ctx->r15 = S32(SIGNED(ctx->r2) >> 8);
    // 0x0F00039C: addiu       $at, $v0, 0xFF
    ctx->r1 = ADD32(ctx->r2, 0XFF);
    // 0x0F0003A0: sra         $t7, $at, 8
    ctx->r15 = S32(SIGNED(ctx->r1) >> 8);
L_0F0003A4:
    // 0x0F0003A4: b           L_0F000494
    // 0x0F0003A8: sb          $t7, 0x1B($t8)
    MEM_B(0X1B, ctx->r24) = ctx->r15;
        goto L_0F000494;
    // 0x0F0003A8: sb          $t7, 0x1B($t8)
    MEM_B(0X1B, ctx->r24) = ctx->r15;
L_0F0003AC:
    // 0x0F0003AC: lw          $t1, 0x0($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X0);
    // 0x0F0003B0: sh          $t9, 0xC($v1)
    MEM_H(0XC, ctx->r3) = ctx->r25;
    // 0x0F0003B4: addiu       $t0, $zero, 0xFF
    ctx->r8 = ADD32(0, 0XFF);
    // 0x0F0003B8: sb          $t0, 0x1B($t1)
    MEM_B(0X1B, ctx->r9) = ctx->r8;
    // 0x0F0003BC: lw          $t2, 0x3C($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X3C);
    // 0x0F0003C0: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F0003C4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x0F0003C8: sltiu       $at, $t2, 0x6
    ctx->r1 = ctx->r10 < 0X6 ? 1 : 0;
    // 0x0F0003CC: beq         $at, $zero, L_0F0003DC
    if (ctx->r1 == 0) {
        // 0x0F0003D0: addiu       $t9, $t9, 0x3FA0
        ctx->r25 = ADD32(ctx->r25, 0X3FA0);
            goto L_0F0003DC;
    }
    // 0x0F0003D0: addiu       $t9, $t9, 0x3FA0
    ctx->r25 = ADD32(ctx->r25, 0X3FA0);
    // 0x0F0003D4: b           L_0F0003DC
    // 0x0F0003D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_0F0003DC;
    // 0x0F0003D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_0F0003DC:
    // 0x0F0003DC: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x0F0003E0: lw          $t3, 0xB04($a2)
    ctx->r11 = MEM_W(ctx->r6, 0XB04);
    // 0x0F0003E4: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x0F0003E8: and         $t5, $t4, $at
    ctx->r13 = ctx->r12 & ctx->r1;
    // 0x0F0003EC: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x0F0003F0: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x0F0003F4: jalr        $t9
    // 0x0F0003F8: addu        $a0, $t3, $t5
    ctx->r4 = ADD32(ctx->r11, ctx->r13);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0003F8: addu        $a0, $t3, $t5
    ctx->r4 = ADD32(ctx->r11, ctx->r13);
    after_0:
    // 0x0F0003FC: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F000400: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    // 0x0F000404: lw          $t6, 0x3C($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X3C);
    // 0x0F000408: lui         $at, 0x4190
    ctx->r1 = S32(0X4190 << 16);
    // 0x0F00040C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000410: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F000414: lw          $a0, 0x4($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X4);
    // 0x0F000418: addiu       $t9, $t9, 0x35D0
    ctx->r25 = ADD32(ctx->r25, 0X35D0);
    // 0x0F00041C: addiu       $t7, $zero, 0x7
    ctx->r15 = ADD32(0, 0X7);
    // 0x0F000420: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F000424: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x0F000428: sw          $t0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r8;
    // 0x0F00042C: sw          $t8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r24;
    // 0x0F000430: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000434: lui         $a3, 0xC210
    ctx->r7 = S32(0XC210 << 16);
    // 0x0F000438: subu        $a1, $t7, $t6
    ctx->r5 = SUB32(ctx->r15, ctx->r14);
    // 0x0F00043C: jalr        $t9
    // 0x0F000440: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000440: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    after_1:
    // 0x0F000444: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x0F000448: lui         $at, 0x4190
    ctx->r1 = S32(0X4190 << 16);
    // 0x0F00044C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000450: lui         $t9, 0x8008
    ctx->r25 = S32(0X8008 << 16);
    // 0x0F000454: lw          $a0, 0x8($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X8);
    // 0x0F000458: addiu       $t9, $t9, 0x3008
    ctx->r25 = ADD32(ctx->r25, 0X3008);
    // 0x0F00045C: addiu       $t1, $zero, 0x9
    ctx->r9 = ADD32(0, 0X9);
    // 0x0F000460: addiu       $t2, $zero, 0x1
    ctx->r10 = ADD32(0, 0X1);
    // 0x0F000464: sw          $t2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r10;
    // 0x0F000468: sw          $t1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r9;
    // 0x0F00046C: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    // 0x0F000470: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000474: lui         $a3, 0xC1B0
    ctx->r7 = S32(0XC1B0 << 16);
    // 0x0F000478: jalr        $t9
    // 0x0F00047C: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00047C: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_2:
    // 0x0F000480: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000484: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000488: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F00048C: jalr        $t9
    // 0x0F000490: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000490: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
L_0F000494:
    // 0x0F000494: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_0F000498:
    // 0x0F000498: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x0F00049C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F0004A0: jr          $ra
    // 0x0F0004A4: nop

    return;
    // 0x0F0004A4: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F0004A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0004A8: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x0F0004AC: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x0F0004B0: lh          $t6, 0x2B4E($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X2B4E);
    // 0x0F0004B4: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x0F0004B8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F0004BC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F0004C0: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0004C4: bne         $t6, $zero, L_0F0004D4
    if (ctx->r14 != 0) {
        // 0x0F0004C8: lw          $s0, 0x38($a0)
        ctx->r16 = MEM_W(ctx->r4, 0X38);
            goto L_0F0004D4;
    }
    // 0x0F0004C8: lw          $s0, 0x38($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X38);
    // 0x0F0004CC: lw          $t7, 0x2BC8($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X2BC8);
    // 0x0F0004D0: beq         $t7, $zero, L_0F0004E0
    if (ctx->r15 == 0) {
        // 0x0F0004D4: addiu       $t8, $zero, 0x1
        ctx->r24 = ADD32(0, 0X1);
            goto L_0F0004E0;
    }
L_0F0004D4:
    // 0x0F0004D4: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x0F0004D8: b           L_0F0004E4
    // 0x0F0004DC: sw          $t8, 0x40($a2)
    MEM_W(0X40, ctx->r6) = ctx->r24;
        goto L_0F0004E4;
    // 0x0F0004DC: sw          $t8, 0x40($a2)
    MEM_W(0X40, ctx->r6) = ctx->r24;
L_0F0004E0:
    // 0x0F0004E0: sw          $zero, 0x40($a2)
    MEM_W(0X40, ctx->r6) = 0;
L_0F0004E4:
    // 0x0F0004E4: lw          $t9, 0x40($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X40);
    // 0x0F0004E8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x0F0004EC: bnel        $t9, $at, L_0F000540
    if (ctx->r25 != ctx->r1) {
        // 0x0F0004F0: lw          $v0, 0x0($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X0);
            goto L_0F000540;
    }
    goto skip_0;
    // 0x0F0004F0: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    skip_0:
    // 0x0F0004F4: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x0F0004F8: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x0F0004FC: lui         $v1, 0x8008
    ctx->r3 = S32(0X8008 << 16);
    // 0x0F000500: lh          $t0, 0x0($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X0);
    // 0x0F000504: addiu       $v1, $v1, 0x3C60
    ctx->r3 = ADD32(ctx->r3, 0X3C60);
    // 0x0F000508: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00050C: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x0F000510: sh          $t1, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r9;
    // 0x0F000514: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
    // 0x0F000518: jalr        $v1
    // 0x0F00051C: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_0;
    // 0x0F00051C: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    after_0:
    // 0x0F000520: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x0F000524: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x0F000528: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F00052C: jalr        $v1
    // 0x0F000530: nop

    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_1;
    // 0x0F000530: nop

    after_1:
    // 0x0F000534: b           L_0F00067C
    // 0x0F000538: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_0F00067C;
    // 0x0F000538: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x0F00053C: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
L_0F000540:
    // 0x0F000540: lui         $v1, 0x8008
    ctx->r3 = S32(0X8008 << 16);
    // 0x0F000544: addiu       $v1, $v1, 0x3C60
    ctx->r3 = ADD32(ctx->r3, 0X3C60);
    // 0x0F000548: lh          $t2, 0x0($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X0);
    // 0x0F00054C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000550: andi        $t3, $t2, 0x7FFF
    ctx->r11 = ctx->r10 & 0X7FFF;
    // 0x0F000554: sh          $t3, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r11;
    // 0x0F000558: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
    // 0x0F00055C: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x0F000560: jalr        $v1
    // 0x0F000564: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_2;
    // 0x0F000564: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    after_2:
    // 0x0F000568: lw          $v1, 0x20($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X20);
    // 0x0F00056C: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x0F000570: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x0F000574: jalr        $v1
    // 0x0F000578: nop

    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_3;
    // 0x0F000578: nop

    after_3:
    // 0x0F00057C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F000580: lwc1        $f4, -0x7D38($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F000584: lui         $at, 0x4060
    ctx->r1 = S32(0X4060 << 16);
    // 0x0F000588: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F00058C: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x0F000590: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x0F000594: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F000598: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F00059C: lw          $v0, 0x44($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X44);
    // 0x0F0005A0: addiu       $t6, $v0, 0x1
    ctx->r14 = ADD32(ctx->r2, 0X1);
    // 0x0F0005A4: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x0F0005A8: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F0005AC: nop

    // 0x0F0005B0: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F0005B4: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F0005B8: nop

    // 0x0F0005BC: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x0F0005C0: beql        $t5, $zero, L_0F000610
    if (ctx->r13 == 0) {
        // 0x0F0005C4: mfc1        $t5, $f10
        ctx->r13 = (int32_t)ctx->f10.u32l;
            goto L_0F000610;
    }
    goto skip_1;
    // 0x0F0005C4: mfc1        $t5, $f10
    ctx->r13 = (int32_t)ctx->f10.u32l;
    skip_1:
    // 0x0F0005C8: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0005CC: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x0F0005D0: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F0005D4: ctc1        $t5, $FpcCsr
    set_cop1_cs(ctx->r13);
    // 0x0F0005D8: nop

    // 0x0F0005DC: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F0005E0: cfc1        $t5, $FpcCsr
    ctx->r13 = get_cop1_cs();
    // 0x0F0005E4: nop

    // 0x0F0005E8: andi        $t5, $t5, 0x78
    ctx->r13 = ctx->r13 & 0X78;
    // 0x0F0005EC: bne         $t5, $zero, L_0F000604
    if (ctx->r13 != 0) {
        // 0x0F0005F0: nop
    
            goto L_0F000604;
    }
    // 0x0F0005F0: nop

    // 0x0F0005F4: mfc1        $t5, $f10
    ctx->r13 = (int32_t)ctx->f10.u32l;
    // 0x0F0005F8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0005FC: b           L_0F00061C
    // 0x0F000600: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
        goto L_0F00061C;
    // 0x0F000600: or          $t5, $t5, $at
    ctx->r13 = ctx->r13 | ctx->r1;
L_0F000604:
    // 0x0F000604: b           L_0F00061C
    // 0x0F000608: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
        goto L_0F00061C;
    // 0x0F000608: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
    // 0x0F00060C: mfc1        $t5, $f10
    ctx->r13 = (int32_t)ctx->f10.u32l;
L_0F000610:
    // 0x0F000610: nop

    // 0x0F000614: bltz        $t5, L_0F000604
    if (SIGNED(ctx->r13) < 0) {
        // 0x0F000618: nop
    
            goto L_0F000604;
    }
    // 0x0F000618: nop

L_0F00061C:
    // 0x0F00061C: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x0F000620: sltu        $at, $v0, $t5
    ctx->r1 = ctx->r2 < ctx->r13 ? 1 : 0;
    // 0x0F000624: beq         $at, $zero, L_0F000634
    if (ctx->r1 == 0) {
        // 0x0F000628: nop
    
            goto L_0F000634;
    }
    // 0x0F000628: nop

    // 0x0F00062C: b           L_0F000678
    // 0x0F000630: sw          $t6, 0x44($a2)
    MEM_W(0X44, ctx->r6) = ctx->r14;
        goto L_0F000678;
    // 0x0F000630: sw          $t6, 0x44($a2)
    MEM_W(0X44, ctx->r6) = ctx->r14;
L_0F000634:
    // 0x0F000634: lui         $v0, 0x8008
    ctx->r2 = S32(0X8008 << 16);
    // 0x0F000638: addiu       $v0, $v0, 0x3B10
    ctx->r2 = ADD32(ctx->r2, 0X3B10);
    // 0x0F00063C: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
    // 0x0F000640: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x0F000644: jalr        $v0
    // 0x0F000648: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_4;
    // 0x0F000648: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    after_4:
    // 0x0F00064C: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x0F000650: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x0F000654: jalr        $v0
    // 0x0F000658: nop

    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_5;
    // 0x0F000658: nop

    after_5:
    // 0x0F00065C: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x0F000660: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000664: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000668: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x0F00066C: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x0F000670: jalr        $t9
    // 0x0F000674: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F000674: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_6:
L_0F000678:
    // 0x0F000678: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_0F00067C:
    // 0x0F00067C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000680: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x0F000684: jr          $ra
    // 0x0F000688: nop

    return;
    // 0x0F000688: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F00068C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F00068C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000690: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000694: lw          $v0, 0x38($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X38);
    // 0x0F000698: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F00069C: ldc1        $f4, 0xDA8($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, 0XDA8);
    // 0x0F0006A0: lhu         $t6, 0xC($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0XC);
    // 0x0F0006A4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F0006A8: lwc1        $f6, -0x7D38($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x0F0006AC: mtc1        $t6, $f16
    ctx->f16.u32l = ctx->r14;
    // 0x0F0006B0: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x0F0006B4: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x0F0006B8: cvt.d.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
    // 0x0F0006BC: bgez        $t6, L_0F0006D8
    if (SIGNED(ctx->r14) >= 0) {
        // 0x0F0006C0: div.d       $f10, $f4, $f8
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = DIV_D(ctx->f4.d, ctx->f8.d);
            goto L_0F0006D8;
    }
    // 0x0F0006C0: div.d       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = DIV_D(ctx->f4.d, ctx->f8.d);
    // 0x0F0006C4: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x0F0006C8: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x0F0006CC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F0006D0: nop

    // 0x0F0006D4: add.d       $f18, $f18, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f6.d); 
    ctx->f18.d = ctx->f18.d + ctx->f6.d;
L_0F0006D8:
    // 0x0F0006D8: sub.d       $f0, $f18, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f10.d); 
    ctx->f0.d = ctx->f18.d - ctx->f10.d;
    // 0x0F0006DC: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x0F0006E0: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x0F0006E4: trunc.w.d   $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    ctx->f4.u32l = TRUNC_W_D(ctx->f0.d);
    // 0x0F0006E8: mfc1        $t8, $f4
    ctx->r24 = (int32_t)ctx->f4.u32l;
    // 0x0F0006EC: nop

    // 0x0F0006F0: bgezl       $t8, L_0F000750
    if (SIGNED(ctx->r24) >= 0) {
        // 0x0F0006F4: cfc1        $t0, $FpcCsr
        ctx->r8 = get_cop1_cs();
            goto L_0F000750;
    }
    goto skip_0;
    // 0x0F0006F4: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
    skip_0:
    // 0x0F0006F8: jal         0x0F000B2C
    // 0x0F0006FC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    ni_ovl_129_func_0F000B2C(rdram, ctx);
        goto after_0;
    // 0x0F0006FC: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_0:
    // 0x0F000700: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x0F000704: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    // 0x0F000708: lw          $t9, 0x3C($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X3C);
    // 0x0F00070C: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x0F000710: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x0F000714: bne         $t9, $at, L_0F000734
    if (ctx->r25 != ctx->r1) {
        // 0x0F000718: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_0F000734;
    }
    // 0x0F000718: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00071C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000720: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x0F000724: jalr        $t9
    // 0x0F000728: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F000728: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    after_1:
    // 0x0F00072C: b           L_0F0007F0
    // 0x0F000730: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_0F0007F0;
    // 0x0F000730: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F000734:
    // 0x0F000734: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000738: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x0F00073C: jalr        $t9
    // 0x0F000740: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F000740: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_2:
    // 0x0F000744: b           L_0F0007F0
    // 0x0F000748: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_0F0007F0;
    // 0x0F000748: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F00074C: cfc1        $t0, $FpcCsr
    ctx->r8 = get_cop1_cs();
L_0F000750:
    // 0x0F000750: ctc1        $t1, $FpcCsr
    set_cop1_cs(ctx->r9);
    // 0x0F000754: lui         $at, 0x41E0
    ctx->r1 = S32(0X41E0 << 16);
    // 0x0F000758: cvt.w.d     $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    ctx->f8.u32l = CVT_W_D(ctx->f0.d);
    // 0x0F00075C: cfc1        $t1, $FpcCsr
    ctx->r9 = get_cop1_cs();
    // 0x0F000760: nop

    // 0x0F000764: andi        $t1, $t1, 0x78
    ctx->r9 = ctx->r9 & 0X78;
    // 0x0F000768: beql        $t1, $zero, L_0F0007BC
    if (ctx->r9 == 0) {
        // 0x0F00076C: mfc1        $t1, $f8
        ctx->r9 = (int32_t)ctx->f8.u32l;
            goto L_0F0007BC;
    }
    goto skip_1;
    // 0x0F00076C: mfc1        $t1, $f8
    ctx->r9 = (int32_t)ctx->f8.u32l;
    skip_1:
    // 0x0F000770: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x0F000774: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x0F000778: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x0F00077C: sub.d       $f8, $f0, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f8.d); 
    ctx->f8.d = ctx->f0.d - ctx->f8.d;
    // 0x0F000780: ctc1        $t1, $FpcCsr
    set_cop1_cs(ctx->r9);
    // 0x0F000784: nop

    // 0x0F000788: cvt.w.d     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.u32l = CVT_W_D(ctx->f8.d);
    // 0x0F00078C: cfc1        $t1, $FpcCsr
    ctx->r9 = get_cop1_cs();
    // 0x0F000790: nop

    // 0x0F000794: andi        $t1, $t1, 0x78
    ctx->r9 = ctx->r9 & 0X78;
    // 0x0F000798: bne         $t1, $zero, L_0F0007B0
    if (ctx->r9 != 0) {
        // 0x0F00079C: nop
    
            goto L_0F0007B0;
    }
    // 0x0F00079C: nop

    // 0x0F0007A0: mfc1        $t1, $f8
    ctx->r9 = (int32_t)ctx->f8.u32l;
    // 0x0F0007A4: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0007A8: b           L_0F0007C8
    // 0x0F0007AC: or          $t1, $t1, $at
    ctx->r9 = ctx->r9 | ctx->r1;
        goto L_0F0007C8;
    // 0x0F0007AC: or          $t1, $t1, $at
    ctx->r9 = ctx->r9 | ctx->r1;
L_0F0007B0:
    // 0x0F0007B0: b           L_0F0007C8
    // 0x0F0007B4: addiu       $t1, $zero, -0x1
    ctx->r9 = ADD32(0, -0X1);
        goto L_0F0007C8;
    // 0x0F0007B4: addiu       $t1, $zero, -0x1
    ctx->r9 = ADD32(0, -0X1);
    // 0x0F0007B8: mfc1        $t1, $f8
    ctx->r9 = (int32_t)ctx->f8.u32l;
L_0F0007BC:
    // 0x0F0007BC: nop

    // 0x0F0007C0: bltz        $t1, L_0F0007B0
    if (SIGNED(ctx->r9) < 0) {
        // 0x0F0007C4: nop
    
            goto L_0F0007B0;
    }
    // 0x0F0007C4: nop

L_0F0007C8:
    // 0x0F0007C8: andi        $t2, $t1, 0xFFFF
    ctx->r10 = ctx->r9 & 0XFFFF;
    // 0x0F0007CC: lw          $t4, 0x0($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X0);
    // 0x0F0007D0: ctc1        $t0, $FpcCsr
    set_cop1_cs(ctx->r8);
    // 0x0F0007D4: sh          $t1, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r9;
    // 0x0F0007D8: bgez        $t2, L_0F0007E8
    if (SIGNED(ctx->r10) >= 0) {
        // 0x0F0007DC: sra         $t3, $t2, 8
        ctx->r11 = S32(SIGNED(ctx->r10) >> 8);
            goto L_0F0007E8;
    }
    // 0x0F0007DC: sra         $t3, $t2, 8
    ctx->r11 = S32(SIGNED(ctx->r10) >> 8);
    // 0x0F0007E0: addiu       $at, $t2, 0xFF
    ctx->r1 = ADD32(ctx->r10, 0XFF);
    // 0x0F0007E4: sra         $t3, $at, 8
    ctx->r11 = S32(SIGNED(ctx->r1) >> 8);
L_0F0007E8:
    // 0x0F0007E8: sb          $t3, 0x1B($t4)
    MEM_B(0X1B, ctx->r12) = ctx->r11;
    // 0x0F0007EC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F0007F0:
    // 0x0F0007F0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0007F4: jr          $ra
    // 0x0F0007F8: nop

    return;
    // 0x0F0007F8: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F0007FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0007FC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x0F000800: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x0F000804: lh          $t6, 0x2B4E($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X2B4E);
    // 0x0F000808: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F00080C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000810: bnel        $t6, $zero, L_0F00092C
    if (ctx->r14 != 0) {
        // 0x0F000814: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F00092C;
    }
    goto skip_0;
    // 0x0F000814: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x0F000818: lw          $t7, 0x2BC8($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X2BC8);
    // 0x0F00081C: bnel        $t7, $zero, L_0F00092C
    if (ctx->r15 != 0) {
        // 0x0F000820: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F00092C;
    }
    goto skip_1;
    // 0x0F000820: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x0F000824: lw          $t8, 0x2960($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2960);
    // 0x0F000828: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F00082C: beql        $t8, $zero, L_0F000860
    if (ctx->r24 == 0) {
        // 0x0F000830: lwc1        $f4, 0x8($v0)
        ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
            goto L_0F000860;
    }
    goto skip_2;
    // 0x0F000830: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    skip_2:
    // 0x0F000834: lw          $t9, 0x2964($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X2964);
    // 0x0F000838: lui         $v1, 0x8000
    ctx->r3 = S32(0X8000 << 16);
    // 0x0F00083C: lw          $t0, 0x0($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X0);
    // 0x0F000840: sll         $t1, $t0, 0
    ctx->r9 = S32(ctx->r8 << 0);
    // 0x0F000844: bltzl       $t1, L_0F00092C
    if (SIGNED(ctx->r9) < 0) {
        // 0x0F000848: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F00092C;
    }
    goto skip_3;
    // 0x0F000848: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_3:
    // 0x0F00084C: lw          $t2, 0x28C4($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X28C4);
    // 0x0F000850: and         $t3, $t2, $v1
    ctx->r11 = ctx->r10 & ctx->r3;
    // 0x0F000854: bnel        $t3, $zero, L_0F00092C
    if (ctx->r11 != 0) {
        // 0x0F000858: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_0F00092C;
    }
    goto skip_4;
    // 0x0F000858: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_4:
    // 0x0F00085C: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
L_0F000860:
    // 0x0F000860: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000864: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x0F000868: lw          $t4, 0x2BD0($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X2BD0);
    // 0x0F00086C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x0F000870: addiu       $t6, $zero, 0x31
    ctx->r14 = ADD32(0, 0X31);
    // 0x0F000874: addiu       $t7, $zero, 0x1C
    ctx->r15 = ADD32(0, 0X1C);
    // 0x0F000878: addiu       $t8, $zero, 0x3
    ctx->r24 = ADD32(0, 0X3);
    // 0x0F00087C: ori         $t5, $t4, 0x10
    ctx->r13 = ctx->r12 | 0X10;
    // 0x0F000880: sw          $t5, 0x2BD0($v0)
    MEM_W(0X2BD0, ctx->r2) = ctx->r13;
    // 0x0F000884: sw          $t6, 0x2BCC($v0)
    MEM_W(0X2BCC, ctx->r2) = ctx->r14;
    // 0x0F000888: cfc1        $t9, $FpcCsr
    ctx->r25 = get_cop1_cs();
    // 0x0F00088C: ctc1        $v1, $FpcCsr
    set_cop1_cs(ctx->r3);
    // 0x0F000890: sh          $t8, 0x2BBA($v0)
    MEM_H(0X2BBA, ctx->r2) = ctx->r24;
    // 0x0F000894: sh          $t7, 0x2BB8($v0)
    MEM_H(0X2BB8, ctx->r2) = ctx->r15;
    // 0x0F000898: cvt.w.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = CVT_W_S(ctx->f8.fl);
    // 0x0F00089C: lui         $at, 0x4F00
    ctx->r1 = S32(0X4F00 << 16);
    // 0x0F0008A0: sb          $zero, 0x2BBC($v0)
    MEM_B(0X2BBC, ctx->r2) = 0;
    // 0x0F0008A4: sb          $zero, 0x2BBD($v0)
    MEM_B(0X2BBD, ctx->r2) = 0;
    // 0x0F0008A8: cfc1        $v1, $FpcCsr
    ctx->r3 = get_cop1_cs();
    // 0x0F0008AC: sb          $zero, 0x2BBE($v0)
    MEM_B(0X2BBE, ctx->r2) = 0;
    // 0x0F0008B0: andi        $v1, $v1, 0x78
    ctx->r3 = ctx->r3 & 0X78;
    // 0x0F0008B4: beql        $v1, $zero, L_0F000904
    if (ctx->r3 == 0) {
        // 0x0F0008B8: mfc1        $v1, $f10
        ctx->r3 = (int32_t)ctx->f10.u32l;
            goto L_0F000904;
    }
    goto skip_5;
    // 0x0F0008B8: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    skip_5:
    // 0x0F0008BC: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0008C0: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x0F0008C4: sub.s       $f10, $f8, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x0F0008C8: ctc1        $v1, $FpcCsr
    set_cop1_cs(ctx->r3);
    // 0x0F0008CC: nop

    // 0x0F0008D0: cvt.w.s     $f10, $f10
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    ctx->f10.u32l = CVT_W_S(ctx->f10.fl);
    // 0x0F0008D4: cfc1        $v1, $FpcCsr
    ctx->r3 = get_cop1_cs();
    // 0x0F0008D8: nop

    // 0x0F0008DC: andi        $v1, $v1, 0x78
    ctx->r3 = ctx->r3 & 0X78;
    // 0x0F0008E0: bne         $v1, $zero, L_0F0008F8
    if (ctx->r3 != 0) {
        // 0x0F0008E4: nop
    
            goto L_0F0008F8;
    }
    // 0x0F0008E4: nop

    // 0x0F0008E8: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
    // 0x0F0008EC: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x0F0008F0: b           L_0F000910
    // 0x0F0008F4: or          $v1, $v1, $at
    ctx->r3 = ctx->r3 | ctx->r1;
        goto L_0F000910;
    // 0x0F0008F4: or          $v1, $v1, $at
    ctx->r3 = ctx->r3 | ctx->r1;
L_0F0008F8:
    // 0x0F0008F8: b           L_0F000910
    // 0x0F0008FC: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
        goto L_0F000910;
    // 0x0F0008FC: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
    // 0x0F000900: mfc1        $v1, $f10
    ctx->r3 = (int32_t)ctx->f10.u32l;
L_0F000904:
    // 0x0F000904: nop

    // 0x0F000908: bltz        $v1, L_0F0008F8
    if (SIGNED(ctx->r3) < 0) {
        // 0x0F00090C: nop
    
            goto L_0F0008F8;
    }
    // 0x0F00090C: nop

L_0F000910:
    // 0x0F000910: ctc1        $t9, $FpcCsr
    set_cop1_cs(ctx->r25);
    // 0x0F000914: sh          $v1, 0x2BC0($v0)
    MEM_H(0X2BC0, ctx->r2) = ctx->r3;
    // 0x0F000918: sh          $v1, 0x2BC2($v0)
    MEM_H(0X2BC2, ctx->r2) = ctx->r3;
    // 0x0F00091C: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F000920: jalr        $t9
    // 0x0F000924: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000924: nop

    after_0:
    // 0x0F000928: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_0F00092C:
    // 0x0F00092C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000930: jr          $ra
    // 0x0F000934: nop

    return;
    // 0x0F000934: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F000938(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000938: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x0F00093C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F000940: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x0F000944: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x0F000948: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x0F00094C: sw          $a0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r4;
    // 0x0F000950: lw          $s1, 0x38($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X38);
    // 0x0F000954: lui         $v1, 0xF00
    ctx->r3 = S32(0XF00 << 16);
    // 0x0F000958: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F00095C: lw          $t7, 0x0($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X0);
    // 0x0F000960: addiu       $a1, $a1, 0xCD8
    ctx->r5 = ADD32(ctx->r5, 0XCD8);
    // 0x0F000964: addiu       $v1, $v1, 0xC58
    ctx->r3 = ADD32(ctx->r3, 0XC58);
    // 0x0F000968: beq         $t7, $zero, L_0F000978
    if (ctx->r15 == 0) {
        // 0x0F00096C: addiu       $v0, $zero, -0x34
        ctx->r2 = ADD32(0, -0X34);
            goto L_0F000978;
    }
    // 0x0F00096C: addiu       $v0, $zero, -0x34
    ctx->r2 = ADD32(0, -0X34);
    // 0x0F000970: b           L_0F000B14
    // 0x0F000974: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_0F000B14;
    // 0x0F000974: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_0F000978:
    // 0x0F000978: addiu       $t8, $zero, 0x34
    ctx->r24 = ADD32(0, 0X34);
    // 0x0F00097C: sh          $t8, 0x30($a1)
    MEM_H(0X30, ctx->r5) = ctx->r24;
    // 0x0F000980: lh          $t9, 0x30($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X30);
    // 0x0F000984: lui         $s2, 0x800A
    ctx->r18 = S32(0X800A << 16);
    // 0x0F000988: lui         $t0, 0xE700
    ctx->r8 = S32(0XE700 << 16);
    // 0x0F00098C: addiu       $s2, $s2, -0x6C40
    ctx->r18 = ADD32(ctx->r18, -0X6C40);
    // 0x0F000990: lui         $t1, 0xDE00
    ctx->r9 = S32(0XDE00 << 16);
    // 0x0F000994: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000998: sh          $v0, 0x20($a1)
    MEM_H(0X20, ctx->r5) = ctx->r2;
    // 0x0F00099C: sh          $v0, 0x0($a1)
    MEM_H(0X0, ctx->r5) = ctx->r2;
    // 0x0F0009A0: sw          $t0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r8;
    // 0x0F0009A4: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x0F0009A8: addiu       $a0, $a0, 0xD18
    ctx->r4 = ADD32(ctx->r4, 0XD18);
    // 0x0F0009AC: sw          $t1, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->r9;
    // 0x0F0009B0: jalr        $s2
    // 0x0F0009B4: sh          $t9, 0x10($a1)
    MEM_H(0X10, ctx->r5) = ctx->r25;
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_0;
    // 0x0F0009B4: sh          $t9, 0x10($a1)
    MEM_H(0X10, ctx->r5) = ctx->r25;
    after_0:
    // 0x0F0009B8: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0009BC: lui         $v1, 0xF00
    ctx->r3 = S32(0XF00 << 16);
    // 0x0F0009C0: sw          $v0, 0xC64($at)
    MEM_W(0XC64, ctx->r1) = ctx->r2;
    // 0x0F0009C4: addiu       $v1, $v1, 0xC68
    ctx->r3 = ADD32(ctx->r3, 0XC68);
    // 0x0F0009C8: lui         $s0, 0xF00
    ctx->r16 = S32(0XF00 << 16);
    // 0x0F0009CC: addiu       $s0, $s0, 0xC70
    ctx->r16 = ADD32(ctx->r16, 0XC70);
    // 0x0F0009D0: lui         $t2, 0xE700
    ctx->r10 = S32(0XE700 << 16);
    // 0x0F0009D4: lui         $t3, 0x100
    ctx->r11 = S32(0X100 << 16);
    // 0x0F0009D8: sw          $t2, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r10;
    // 0x0F0009DC: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x0F0009E0: ori         $t3, $t3, 0x4008
    ctx->r11 = ctx->r11 | 0X4008;
    // 0x0F0009E4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x0F0009E8: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F0009EC: sw          $t3, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r11;
    // 0x0F0009F0: addiu       $a0, $a1, 0xCD8
    ctx->r4 = ADD32(ctx->r5, 0XCD8);
    // 0x0F0009F4: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    // 0x0F0009F8: jalr        $s2
    // 0x0F0009FC: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_1;
    // 0x0F0009FC: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    after_1:
    // 0x0F000A00: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x0F000A04: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x0F000A08: lui         $t4, 0x604
    ctx->r12 = S32(0X604 << 16);
    // 0x0F000A0C: lui         $t5, 0x6
    ctx->r13 = S32(0X6 << 16);
    // 0x0F000A10: sw          $v0, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r2;
    // 0x0F000A14: ori         $t5, $t5, 0x402
    ctx->r13 = ctx->r13 | 0X402;
    // 0x0F000A18: ori         $t4, $t4, 0x200
    ctx->r12 = ctx->r12 | 0X200;
    // 0x0F000A1C: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    // 0x0F000A20: sw          $t4, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r12;
    // 0x0F000A24: sw          $t5, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r13;
    // 0x0F000A28: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    // 0x0F000A2C: lui         $t6, 0xDE00
    ctx->r14 = S32(0XDE00 << 16);
    // 0x0F000A30: sw          $t6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r14;
    // 0x0F000A34: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000A38: addiu       $a0, $a0, 0xD70
    ctx->r4 = ADD32(ctx->r4, 0XD70);
    // 0x0F000A3C: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    // 0x0F000A40: jalr        $s2
    // 0x0F000A44: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_2;
    // 0x0F000A44: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    after_2:
    // 0x0F000A48: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000A4C: lui         $t7, 0xDF00
    ctx->r15 = S32(0XDF00 << 16);
    // 0x0F000A50: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000A54: sw          $v0, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r2;
    // 0x0F000A58: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x0F000A5C: sw          $t7, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r15;
    // 0x0F000A60: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x0F000A64: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F000A68: lw          $a1, 0x27B0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27B0);
    // 0x0F000A6C: jalr        $t9
    // 0x0F000A70: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000A70: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_3:
    // 0x0F000A74: lw          $t8, 0x50($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X50);
    // 0x0F000A78: lui         $a0, 0xF00
    ctx->r4 = S32(0XF00 << 16);
    // 0x0F000A7C: addiu       $a0, $a0, 0xC58
    ctx->r4 = ADD32(ctx->r4, 0XC58);
    // 0x0F000A80: sw          $v0, 0x24($t8)
    MEM_W(0X24, ctx->r24) = ctx->r2;
    // 0x0F000A84: sw          $v0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r2;
    // 0x0F000A88: jalr        $s2
    // 0x0F000A8C: sw          $zero, 0x40($v0)
    MEM_W(0X40, ctx->r2) = 0;
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_4;
    // 0x0F000A8C: sw          $zero, 0x40($v0)
    MEM_W(0X40, ctx->r2) = 0;
    after_4:
    // 0x0F000A90: lw          $t1, 0x0($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X0);
    // 0x0F000A94: addiu       $t4, $zero, -0x100
    ctx->r12 = ADD32(0, -0X100);
    // 0x0F000A98: lui         $at, 0x40C0
    ctx->r1 = S32(0X40C0 << 16);
    // 0x0F000A9C: sw          $v0, 0x3C($t1)
    MEM_W(0X3C, ctx->r9) = ctx->r2;
    // 0x0F000AA0: lw          $v1, 0x0($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X0);
    // 0x0F000AA4: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000AA8: lui         $at, 0x426C
    ctx->r1 = S32(0X426C << 16);
    // 0x0F000AAC: lhu         $t2, 0x2($v1)
    ctx->r10 = MEM_HU(ctx->r3, 0X2);
    // 0x0F000AB0: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000AB4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x0F000AB8: ori         $t3, $t2, 0x800
    ctx->r11 = ctx->r10 | 0X800;
    // 0x0F000ABC: sh          $t3, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r11;
    // 0x0F000AC0: lw          $t5, 0x0($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X0);
    // 0x0F000AC4: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x0F000AC8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x0F000ACC: sw          $t4, 0x18($t5)
    MEM_W(0X18, ctx->r13) = ctx->r12;
    // 0x0F000AD0: lw          $t6, 0x0($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X0);
    // 0x0F000AD4: swc1        $f4, 0x50($t6)
    MEM_W(0X50, ctx->r14) = ctx->f4.u32l;
    // 0x0F000AD8: lw          $t7, 0x0($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X0);
    // 0x0F000ADC: swc1        $f6, 0x54($t7)
    MEM_W(0X54, ctx->r15) = ctx->f6.u32l;
    // 0x0F000AE0: lh          $t9, 0x2B4E($a0)
    ctx->r25 = MEM_H(ctx->r4, 0X2B4E);
    // 0x0F000AE4: lw          $t1, 0x50($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X50);
    // 0x0F000AE8: bnel        $t9, $zero, L_0F000AFC
    if (ctx->r25 != 0) {
        // 0x0F000AEC: addiu       $t0, $zero, 0x1
        ctx->r8 = ADD32(0, 0X1);
            goto L_0F000AFC;
    }
    goto skip_0;
    // 0x0F000AEC: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    skip_0:
    // 0x0F000AF0: lw          $t8, 0x2BC8($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X2BC8);
    // 0x0F000AF4: beq         $t8, $zero, L_0F000B14
    if (ctx->r24 == 0) {
        // 0x0F000AF8: addiu       $t0, $zero, 0x1
        ctx->r8 = ADD32(0, 0X1);
            goto L_0F000B14;
    }
    // 0x0F000AF8: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
L_0F000AFC:
    // 0x0F000AFC: sw          $t0, 0x40($t1)
    MEM_W(0X40, ctx->r9) = ctx->r8;
    // 0x0F000B00: lw          $v1, 0x0($s1)
    ctx->r3 = MEM_W(ctx->r17, 0X0);
    // 0x0F000B04: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x0F000B08: lh          $t2, 0x0($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X0);
    // 0x0F000B0C: or          $t3, $t2, $at
    ctx->r11 = ctx->r10 | ctx->r1;
    // 0x0F000B10: sh          $t3, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r11;
L_0F000B14:
    // 0x0F000B14: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x0F000B18: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x0F000B1C: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000B20: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x0F000B24: jr          $ra
    // 0x0F000B28: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x0F000B28: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void ni_ovl_129_func_0F000B2C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000B2C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F000B30: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000B34: lw          $v1, 0x38($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X38);
    // 0x0F000B38: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000B3C: addiu       $t9, $t9, 0x516C
    ctx->r25 = ADD32(ctx->r25, 0X516C);
    // 0x0F000B40: lw          $a2, 0x0($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X0);
    // 0x0F000B44: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000B48: bne         $a2, $zero, L_0F000B58
    if (ctx->r6 != 0) {
        // 0x0F000B4C: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_0F000B58;
    }
    // 0x0F000B4C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x0F000B50: b           L_0F000B6C
    // 0x0F000B54: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_0F000B6C;
    // 0x0F000B54: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_0F000B58:
    // 0x0F000B58: jalr        $t9
    // 0x0F000B5C: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000B5C: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_0:
    // 0x0F000B60: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000B64: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x0F000B68: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
L_0F000B6C:
    // 0x0F000B6C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000B70: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x0F000B74: jr          $ra
    // 0x0F000B78: nop

    return;
    // 0x0F000B78: nop

;}
RECOMP_FUNC void ni_ovl_129_func_0F000B7C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000B7C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x0F000B80: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x0F000B84: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x0F000B88: lui         $s1, 0x801D
    ctx->r17 = S32(0X801D << 16);
    // 0x0F000B8C: lui         $s0, 0x8000
    ctx->r16 = S32(0X8000 << 16);
    // 0x0F000B90: addiu       $s1, $s1, -0x55A0
    ctx->r17 = ADD32(ctx->r17, -0X55A0);
    // 0x0F000B94: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x0F000B98: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x0F000B9C: addiu       $s0, $s0, 0x48A0
    ctx->r16 = ADD32(ctx->r16, 0X48A0);
    // 0x0F000BA0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000BA4: jalr        $s0
    // 0x0F000BA8: addiu       $a1, $zero, 0x2A9
    ctx->r5 = ADD32(0, 0X2A9);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_0;
    // 0x0F000BA8: addiu       $a1, $zero, 0x2A9
    ctx->r5 = ADD32(0, 0X2A9);
    after_0:
    // 0x0F000BAC: beq         $v0, $zero, L_0F000C0C
    if (ctx->r2 == 0) {
        // 0x0F000BB0: or          $a0, $s1, $zero
        ctx->r4 = ctx->r17 | 0;
            goto L_0F000C0C;
    }
    // 0x0F000BB0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000BB4: jalr        $s0
    // 0x0F000BB8: addiu       $a1, $zero, 0x2A8
    ctx->r5 = ADD32(0, 0X2A8);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_1;
    // 0x0F000BB8: addiu       $a1, $zero, 0x2A8
    ctx->r5 = ADD32(0, 0X2A8);
    after_1:
    // 0x0F000BBC: beq         $v0, $zero, L_0F000C0C
    if (ctx->r2 == 0) {
        // 0x0F000BC0: or          $a0, $s1, $zero
        ctx->r4 = ctx->r17 | 0;
            goto L_0F000C0C;
    }
    // 0x0F000BC0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000BC4: jalr        $s0
    // 0x0F000BC8: addiu       $a1, $zero, 0x2A7
    ctx->r5 = ADD32(0, 0X2A7);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_2;
    // 0x0F000BC8: addiu       $a1, $zero, 0x2A7
    ctx->r5 = ADD32(0, 0X2A7);
    after_2:
    // 0x0F000BCC: beq         $v0, $zero, L_0F000C0C
    if (ctx->r2 == 0) {
        // 0x0F000BD0: or          $a0, $s1, $zero
        ctx->r4 = ctx->r17 | 0;
            goto L_0F000C0C;
    }
    // 0x0F000BD0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000BD4: jalr        $s0
    // 0x0F000BD8: addiu       $a1, $zero, 0x2A6
    ctx->r5 = ADD32(0, 0X2A6);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_3;
    // 0x0F000BD8: addiu       $a1, $zero, 0x2A6
    ctx->r5 = ADD32(0, 0X2A6);
    after_3:
    // 0x0F000BDC: beq         $v0, $zero, L_0F000C0C
    if (ctx->r2 == 0) {
        // 0x0F000BE0: or          $a0, $s1, $zero
        ctx->r4 = ctx->r17 | 0;
            goto L_0F000C0C;
    }
    // 0x0F000BE0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000BE4: jalr        $s0
    // 0x0F000BE8: addiu       $a1, $zero, 0x2A5
    ctx->r5 = ADD32(0, 0X2A5);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_4;
    // 0x0F000BE8: addiu       $a1, $zero, 0x2A5
    ctx->r5 = ADD32(0, 0X2A5);
    after_4:
    // 0x0F000BEC: beq         $v0, $zero, L_0F000C0C
    if (ctx->r2 == 0) {
        // 0x0F000BF0: or          $a0, $s1, $zero
        ctx->r4 = ctx->r17 | 0;
            goto L_0F000C0C;
    }
    // 0x0F000BF0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x0F000BF4: jalr        $s0
    // 0x0F000BF8: addiu       $a1, $zero, 0x2A4
    ctx->r5 = ADD32(0, 0X2A4);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_5;
    // 0x0F000BF8: addiu       $a1, $zero, 0x2A4
    ctx->r5 = ADD32(0, 0X2A4);
    after_5:
    // 0x0F000BFC: beql        $v0, $zero, L_0F000C10
    if (ctx->r2 == 0) {
        // 0x0F000C00: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_0F000C10;
    }
    goto skip_0;
    // 0x0F000C00: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x0F000C04: b           L_0F000C10
    // 0x0F000C08: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_0F000C10;
    // 0x0F000C08: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_0F000C0C:
    // 0x0F000C0C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_0F000C10:
    // 0x0F000C10: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x0F000C14: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x0F000C18: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x0F000C1C: jr          $ra
    // 0x0F000C20: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x0F000C20: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void ni_ovl_130_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0xCFC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0XCFC);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_130_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000078: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F00007C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000080: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x0F000084: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000088: sw          $zero, 0x30($sp)
    MEM_W(0X30, ctx->r29) = 0;
    // 0x0F00008C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000090: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    // 0x0F000094: jalr        $t9
    // 0x0F000098: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000098: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x0F00009C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0000A0: addiu       $t9, $t9, 0x34AC
    ctx->r25 = ADD32(ctx->r25, 0X34AC);
    // 0x0F0000A4: sw          $v0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r2;
    // 0x0F0000A8: jalr        $t9
    // 0x0F0000AC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0000AC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x0F0000B0: lw          $a1, 0x3C($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X3C);
    // 0x0F0000B4: sw          $v0, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->r2;
    // 0x0F0000B8: sw          $v0, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->r2;
    // 0x0F0000BC: bne         $a1, $zero, L_0F0000CC
    if (ctx->r5 != 0) {
        // 0x0F0000C0: lui         $t6, 0x8019
        ctx->r14 = S32(0X8019 << 16);
            goto L_0F0000CC;
    }
    // 0x0F0000C0: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x0F0000C4: lw          $a1, 0x27A8($t6)
    ctx->r5 = MEM_W(ctx->r14, 0X27A8);
    // 0x0F0000C8: sw          $a1, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r5;
L_0F0000CC:
    // 0x0F0000CC: lw          $t7, 0x5C($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X5C);
    // 0x0F0000D0: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x0F0000D4: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F0000D8: sltiu       $at, $t7, 0x3
    ctx->r1 = ctx->r15 < 0X3 ? 1 : 0;
    // 0x0F0000DC: beq         $at, $zero, L_0F0001B8
    if (ctx->r1 == 0) {
        // 0x0F0000E0: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_0F0001B8;
    }
    // 0x0F0000E0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000E4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0000E8: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x0F0000EC: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F0000F0: addiu       $a2, $a2, 0x5A0
    ctx->r6 = ADD32(ctx->r6, 0X5A0);
    // 0x0F0000F4: jalr        $t9
    // 0x0F0000F8: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F0000F8: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_2:
    // 0x0F0000FC: lw          $t8, 0x5C($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X5C);
    // 0x0F000100: or          $t0, $v0, $zero
    ctx->r8 = ctx->r2 | 0;
    // 0x0F000104: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x0F000108: bne         $t8, $zero, L_0F000114
    if (ctx->r24 != 0) {
        // 0x0F00010C: addiu       $t1, $zero, 0x1
        ctx->r9 = ADD32(0, 0X1);
            goto L_0F000114;
    }
    // 0x0F00010C: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x0F000110: sw          $t1, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r9;
L_0F000114:
    // 0x0F000114: lw          $t2, 0x14($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X14);
    // 0x0F000118: addiu       $a1, $zero, 0x5
    ctx->r5 = ADD32(0, 0X5);
    // 0x0F00011C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x0F000120: lw          $t3, 0x14($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X14);
    // 0x0F000124: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x0F000128: lw          $t4, 0x14($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X14);
    // 0x0F00012C: lw          $v1, 0x10($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X10);
    // 0x0F000130: lh          $t5, 0x0($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X0);
    // 0x0F000134: ori         $t6, $t5, 0x8
    ctx->r14 = ctx->r13 | 0X8;
    // 0x0F000138: sh          $t6, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r14;
    // 0x0F00013C: lw          $v0, 0x14($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X14);
    // 0x0F000140: lw          $t7, 0x3C($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X3C);
    // 0x0F000144: sw          $t7, 0x74($v0)
    MEM_W(0X74, ctx->r2) = ctx->r15;
    // 0x0F000148: lw          $t9, 0x14($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X14);
    // 0x0F00014C: lw          $v0, 0x14($t9)
    ctx->r2 = MEM_W(ctx->r25, 0X14);
    // 0x0F000150: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000154: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F000158: lw          $t8, 0x3C($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X3C);
    // 0x0F00015C: sw          $t8, 0x74($v0)
    MEM_W(0X74, ctx->r2) = ctx->r24;
    // 0x0F000160: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    // 0x0F000164: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F000168: jalr        $t9
    // 0x0F00016C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F00016C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    after_3:
    // 0x0F000170: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F000174: lwc1        $f4, -0x7D34($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D34);
    // 0x0F000178: lui         $t1, 0x8001
    ctx->r9 = S32(0X8001 << 16);
    // 0x0F00017C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000180: addiu       $t9, $t9, -0x4B64
    ctx->r25 = ADD32(ctx->r25, -0X4B64);
    // 0x0F000184: addiu       $t1, $t1, -0x4F40
    ctx->r9 = ADD32(ctx->r9, -0X4F40);
    // 0x0F000188: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F00018C: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F000190: lui         $a3, 0xF00
    ctx->r7 = S32(0XF00 << 16);
    // 0x0F000194: addiu       $a3, $a3, 0xC4C
    ctx->r7 = ADD32(ctx->r7, 0XC4C);
    // 0x0F000198: addiu       $a2, $a2, 0xA1C
    ctx->r6 = ADD32(ctx->r6, 0XA1C);
    // 0x0F00019C: addiu       $a1, $a1, 0xBAC
    ctx->r5 = ADD32(ctx->r5, 0XBAC);
    // 0x0F0001A0: sw          $t1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r9;
    // 0x0F0001A4: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x0F0001A8: jalr        $t9
    // 0x0F0001AC: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x0F0001AC: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_4:
    // 0x0F0001B0: b           L_0F0002B8
    // 0x0F0001B4: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
        goto L_0F0002B8;
    // 0x0F0001B4: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
L_0F0001B8:
    // 0x0F0001B8: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x0F0001BC: jalr        $t9
    // 0x0F0001C0: addiu       $a2, $a2, 0x5D4
    ctx->r6 = ADD32(ctx->r6, 0X5D4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F0001C0: addiu       $a2, $a2, 0x5D4
    ctx->r6 = ADD32(ctx->r6, 0X5D4);
    after_5:
    // 0x0F0001C4: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x0F0001C8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0001CC: addiu       $t9, $t9, -0x5254
    ctx->r25 = ADD32(ctx->r25, -0X5254);
    // 0x0F0001D0: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    // 0x0F0001D4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F0001D8: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x0F0001DC: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x0F0001E0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x0F0001E4: jalr        $t9
    // 0x0F0001E8: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x0F0001E8: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_6:
    // 0x0F0001EC: lw          $v0, 0x50($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X50);
    // 0x0F0001F0: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x0F0001F4: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
    // 0x0F0001F8: beq         $v0, $at, L_0F00022C
    if (ctx->r2 == ctx->r1) {
        // 0x0F0001FC: lui         $t2, 0xF00
        ctx->r10 = S32(0XF00 << 16);
            goto L_0F00022C;
    }
    // 0x0F0001FC: lui         $t2, 0xF00
    ctx->r10 = S32(0XF00 << 16);
    // 0x0F000200: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x0F000204: beq         $v0, $at, L_0F000244
    if (ctx->r2 == ctx->r1) {
        // 0x0F000208: lui         $t4, 0xF00
        ctx->r12 = S32(0XF00 << 16);
            goto L_0F000244;
    }
    // 0x0F000208: lui         $t4, 0xF00
    ctx->r12 = S32(0XF00 << 16);
    // 0x0F00020C: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x0F000210: beq         $v0, $at, L_0F00025C
    if (ctx->r2 == ctx->r1) {
        // 0x0F000214: lui         $t6, 0xF00
        ctx->r14 = S32(0XF00 << 16);
            goto L_0F00025C;
    }
    // 0x0F000214: lui         $t6, 0xF00
    ctx->r14 = S32(0XF00 << 16);
    // 0x0F000218: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x0F00021C: beq         $v0, $at, L_0F000274
    if (ctx->r2 == ctx->r1) {
        // 0x0F000220: lui         $a1, 0xF00
        ctx->r5 = S32(0XF00 << 16);
            goto L_0F000274;
    }
    // 0x0F000220: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F000224: b           L_0F000284
    // 0x0F000228: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
        goto L_0F000284;
    // 0x0F000228: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
L_0F00022C:
    // 0x0F00022C: lui         $t3, 0xF00
    ctx->r11 = S32(0XF00 << 16);
    // 0x0F000230: addiu       $t3, $t3, 0x5FC
    ctx->r11 = ADD32(ctx->r11, 0X5FC);
    // 0x0F000234: addiu       $a1, $t2, 0x63C
    ctx->r5 = ADD32(ctx->r10, 0X63C);
    // 0x0F000238: sw          $a1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r5;
    // 0x0F00023C: b           L_0F000284
    // 0x0F000240: sw          $t3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r11;
        goto L_0F000284;
    // 0x0F000240: sw          $t3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r11;
L_0F000244:
    // 0x0F000244: lui         $t5, 0xF00
    ctx->r13 = S32(0XF00 << 16);
    // 0x0F000248: addiu       $t5, $t5, 0x65C
    ctx->r13 = ADD32(ctx->r13, 0X65C);
    // 0x0F00024C: addiu       $a1, $t4, 0x77C
    ctx->r5 = ADD32(ctx->r12, 0X77C);
    // 0x0F000250: sw          $a1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r5;
    // 0x0F000254: b           L_0F000284
    // 0x0F000258: sw          $t5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r13;
        goto L_0F000284;
    // 0x0F000258: sw          $t5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r13;
L_0F00025C:
    // 0x0F00025C: lui         $t7, 0xF00
    ctx->r15 = S32(0XF00 << 16);
    // 0x0F000260: addiu       $t7, $t7, 0x80C
    ctx->r15 = ADD32(ctx->r15, 0X80C);
    // 0x0F000264: addiu       $a1, $t6, 0x8AC
    ctx->r5 = ADD32(ctx->r14, 0X8AC);
    // 0x0F000268: sw          $a1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r5;
    // 0x0F00026C: b           L_0F000284
    // 0x0F000270: sw          $t7, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r15;
        goto L_0F000284;
    // 0x0F000270: sw          $t7, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r15;
L_0F000274:
    // 0x0F000274: lui         $t8, 0xF00
    ctx->r24 = S32(0XF00 << 16);
    // 0x0F000278: addiu       $t8, $t8, 0x8FC
    ctx->r24 = ADD32(ctx->r24, 0X8FC);
    // 0x0F00027C: sw          $t8, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r24;
    // 0x0F000280: addiu       $a1, $a1, 0x9BC
    ctx->r5 = ADD32(ctx->r5, 0X9BC);
L_0F000284:
    // 0x0F000284: beq         $a1, $zero, L_0F0002B8
    if (ctx->r5 == 0) {
        // 0x0F000288: lw          $a0, 0x34($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X34);
            goto L_0F0002B8;
    }
    // 0x0F000288: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x0F00028C: lui         $t1, 0x8001
    ctx->r9 = S32(0X8001 << 16);
    // 0x0F000290: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000294: addiu       $t9, $t9, -0x4C94
    ctx->r25 = ADD32(ctx->r25, -0X4C94);
    // 0x0F000298: addiu       $t1, $t1, -0x4F40
    ctx->r9 = ADD32(ctx->r9, -0X4F40);
    // 0x0F00029C: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x0F0002A0: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x0F0002A4: sw          $t1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r9;
    // 0x0F0002A8: lw          $a2, 0x2C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0002AC: jalr        $t9
    // 0x0F0002B0: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x0F0002B0: sw          $t0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r8;
    after_7:
    // 0x0F0002B4: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
L_0F0002B8:
    // 0x0F0002B8: lhu         $t3, 0x2($t0)
    ctx->r11 = MEM_HU(ctx->r8, 0X2);
    // 0x0F0002BC: lh          $t5, 0x0($t0)
    ctx->r13 = MEM_H(ctx->r8, 0X0);
    // 0x0F0002C0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0002C4: ori         $t2, $t3, 0x1000
    ctx->r10 = ctx->r11 | 0X1000;
    // 0x0F0002C8: ori         $t4, $t5, 0x200
    ctx->r12 = ctx->r13 | 0X200;
    // 0x0F0002CC: sh          $t2, 0x2($t0)
    MEM_H(0X2, ctx->r8) = ctx->r10;
    // 0x0F0002D0: sh          $t4, 0x0($t0)
    MEM_H(0X0, ctx->r8) = ctx->r12;
    // 0x0F0002D4: jal         0x0F0004F4
    // 0x0F0002D8: lh          $a1, 0x5E($s0)
    ctx->r5 = MEM_H(ctx->r16, 0X5E);
    static_184_0F0004F4(rdram, ctx);
        goto after_8;
    // 0x0F0002D8: lh          $a1, 0x5E($s0)
    ctx->r5 = MEM_H(ctx->r16, 0X5E);
    after_8:
    // 0x0F0002DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0002E0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0002E4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0002E8: jalr        $t9
    // 0x0F0002EC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F0002EC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_9:
    // 0x0F0002F0: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x0F0002F4: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F0002F8: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x0F0002FC: jr          $ra
    // 0x0F000300: nop

    return;
    // 0x0F000300: nop

;}
RECOMP_FUNC void ni_ovl_130_func_0F000304(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000304: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x0F000308: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x0F00030C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000310: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x0F000314: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x0F000318: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x0F00031C: sw          $t6, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r14;
    // 0x0F000320: lw          $v0, 0x5C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X5C);
    // 0x0F000324: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x0F000328: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x0F00032C: beq         $v0, $a2, L_0F000350
    if (ctx->r2 == ctx->r6) {
        // 0x0F000330: lui         $a3, 0xF00
        ctx->r7 = S32(0XF00 << 16);
            goto L_0F000350;
    }
    // 0x0F000330: lui         $a3, 0xF00
    ctx->r7 = S32(0XF00 << 16);
    // 0x0F000334: beq         $v0, $a0, L_0F000350
    if (ctx->r2 == ctx->r4) {
        // 0x0F000338: nop
    
            goto L_0F000350;
    }
    // 0x0F000338: nop

    // 0x0F00033C: addiu       $a3, $zero, 0x3
    ctx->r7 = ADD32(0, 0X3);
    // 0x0F000340: beql        $v0, $a3, L_0F0003B8
    if (ctx->r2 == ctx->r7) {
        // 0x0F000344: lw          $v1, 0x50($s0)
        ctx->r3 = MEM_W(ctx->r16, 0X50);
            goto L_0F0003B8;
    }
    goto skip_0;
    // 0x0F000344: lw          $v1, 0x50($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X50);
    skip_0:
    // 0x0F000348: b           L_0F000480
    // 0x0F00034C: lw          $v0, 0x54($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X54);
        goto L_0F000480;
    // 0x0F00034C: lw          $v0, 0x54($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X54);
L_0F000350:
    // 0x0F000350: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x0F000354: lwc1        $f4, -0x7D34($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D34);
    // 0x0F000358: lui         $v0, 0x8001
    ctx->r2 = S32(0X8001 << 16);
    // 0x0F00035C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000360: addiu       $t9, $t9, -0x4B64
    ctx->r25 = ADD32(ctx->r25, -0X4B64);
    // 0x0F000364: addiu       $v0, $v0, -0x4F40
    ctx->r2 = ADD32(ctx->r2, -0X4F40);
    // 0x0F000368: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F00036C: lui         $a2, 0xF00
    ctx->r6 = S32(0XF00 << 16);
    // 0x0F000370: addiu       $a2, $a2, 0xA1C
    ctx->r6 = ADD32(ctx->r6, 0XA1C);
    // 0x0F000374: addiu       $a1, $a1, 0xBAC
    ctx->r5 = ADD32(ctx->r5, 0XBAC);
    // 0x0F000378: sw          $v0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r2;
    // 0x0F00037C: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    // 0x0F000380: lw          $a0, 0x3C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3C);
    // 0x0F000384: addiu       $a3, $a3, 0xC4C
    ctx->r7 = ADD32(ctx->r7, 0XC4C);
    // 0x0F000388: jalr        $t9
    // 0x0F00038C: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00038C: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x0F000390: lw          $t7, 0x50($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X50);
    // 0x0F000394: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000398: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F00039C: beq         $t7, $zero, L_0F00047C
    if (ctx->r15 == 0) {
        // 0x0F0003A0: lw          $a0, 0x3C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X3C);
            goto L_0F00047C;
    }
    // 0x0F0003A0: lw          $a0, 0x3C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3C);
    // 0x0F0003A4: jalr        $t9
    // 0x0F0003A8: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x0F0003A8: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    after_1:
    // 0x0F0003AC: b           L_0F000480
    // 0x0F0003B0: lw          $v0, 0x54($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X54);
        goto L_0F000480;
    // 0x0F0003B0: lw          $v0, 0x54($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X54);
    // 0x0F0003B4: lw          $v1, 0x50($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X50);
L_0F0003B8:
    // 0x0F0003B8: lui         $t8, 0xF00
    ctx->r24 = S32(0XF00 << 16);
    // 0x0F0003BC: addiu       $t8, $t8, 0x5FC
    ctx->r24 = ADD32(ctx->r24, 0X5FC);
    // 0x0F0003C0: beq         $v1, $a2, L_0F0003EC
    if (ctx->r3 == ctx->r6) {
        // 0x0F0003C4: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_0F0003EC;
    }
    // 0x0F0003C4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x0F0003C8: beq         $v0, $a0, L_0F0003FC
    if (ctx->r2 == ctx->r4) {
        // 0x0F0003CC: lui         $t0, 0xF00
        ctx->r8 = S32(0XF00 << 16);
            goto L_0F0003FC;
    }
    // 0x0F0003CC: lui         $t0, 0xF00
    ctx->r8 = S32(0XF00 << 16);
    // 0x0F0003D0: beq         $v0, $a3, L_0F000410
    if (ctx->r2 == ctx->r7) {
        // 0x0F0003D4: lui         $t1, 0xF00
        ctx->r9 = S32(0XF00 << 16);
            goto L_0F000410;
    }
    // 0x0F0003D4: lui         $t1, 0xF00
    ctx->r9 = S32(0XF00 << 16);
    // 0x0F0003D8: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x0F0003DC: beq         $v0, $at, L_0F000424
    if (ctx->r2 == ctx->r1) {
        // 0x0F0003E0: lui         $t2, 0xF00
        ctx->r10 = S32(0XF00 << 16);
            goto L_0F000424;
    }
    // 0x0F0003E0: lui         $t2, 0xF00
    ctx->r10 = S32(0XF00 << 16);
    // 0x0F0003E4: b           L_0F000434
    // 0x0F0003E8: nop

        goto L_0F000434;
    // 0x0F0003E8: nop

L_0F0003EC:
    // 0x0F0003EC: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F0003F0: addiu       $a1, $a1, 0x63C
    ctx->r5 = ADD32(ctx->r5, 0X63C);
    // 0x0F0003F4: b           L_0F000434
    // 0x0F0003F8: sw          $t8, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r24;
        goto L_0F000434;
    // 0x0F0003F8: sw          $t8, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r24;
L_0F0003FC:
    // 0x0F0003FC: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F000400: addiu       $t0, $t0, 0x65C
    ctx->r8 = ADD32(ctx->r8, 0X65C);
    // 0x0F000404: sw          $t0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r8;
    // 0x0F000408: b           L_0F000434
    // 0x0F00040C: addiu       $a1, $a1, 0x77C
    ctx->r5 = ADD32(ctx->r5, 0X77C);
        goto L_0F000434;
    // 0x0F00040C: addiu       $a1, $a1, 0x77C
    ctx->r5 = ADD32(ctx->r5, 0X77C);
L_0F000410:
    // 0x0F000410: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F000414: addiu       $t1, $t1, 0x80C
    ctx->r9 = ADD32(ctx->r9, 0X80C);
    // 0x0F000418: sw          $t1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r9;
    // 0x0F00041C: b           L_0F000434
    // 0x0F000420: addiu       $a1, $a1, 0x8AC
    ctx->r5 = ADD32(ctx->r5, 0X8AC);
        goto L_0F000434;
    // 0x0F000420: addiu       $a1, $a1, 0x8AC
    ctx->r5 = ADD32(ctx->r5, 0X8AC);
L_0F000424:
    // 0x0F000424: lui         $a1, 0xF00
    ctx->r5 = S32(0XF00 << 16);
    // 0x0F000428: addiu       $t2, $t2, 0x8FC
    ctx->r10 = ADD32(ctx->r10, 0X8FC);
    // 0x0F00042C: sw          $t2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r10;
    // 0x0F000430: addiu       $a1, $a1, 0x9BC
    ctx->r5 = ADD32(ctx->r5, 0X9BC);
L_0F000434:
    // 0x0F000434: beq         $a1, $zero, L_0F000464
    if (ctx->r5 == 0) {
        // 0x0F000438: lw          $a0, 0x3C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X3C);
            goto L_0F000464;
    }
    // 0x0F000438: lw          $a0, 0x3C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3C);
    // 0x0F00043C: lui         $t3, 0x8001
    ctx->r11 = S32(0X8001 << 16);
    // 0x0F000440: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000444: addiu       $t9, $t9, -0x4C94
    ctx->r25 = ADD32(ctx->r25, -0X4C94);
    // 0x0F000448: addiu       $t3, $t3, -0x4F40
    ctx->r11 = ADD32(ctx->r11, -0X4F40);
    // 0x0F00044C: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x0F000450: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x0F000454: sw          $t3, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r11;
    // 0x0F000458: jalr        $t9
    // 0x0F00045C: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00045C: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    after_2:
    // 0x0F000460: lw          $v1, 0x50($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X50);
L_0F000464:
    // 0x0F000464: beq         $v1, $zero, L_0F00047C
    if (ctx->r3 == 0) {
        // 0x0F000468: lw          $a0, 0x3C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X3C);
            goto L_0F00047C;
    }
    // 0x0F000468: lw          $a0, 0x3C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3C);
    // 0x0F00046C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F000470: addiu       $t9, $t9, -0x4608
    ctx->r25 = ADD32(ctx->r25, -0X4608);
    // 0x0F000474: jalr        $t9
    // 0x0F000478: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x0F000478: lw          $a1, 0x24($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X24);
    after_3:
L_0F00047C:
    // 0x0F00047C: lw          $v0, 0x54($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X54);
L_0F000480:
    // 0x0F000480: beq         $v0, $zero, L_0F0004A0
    if (ctx->r2 == 0) {
        // 0x0F000484: sltiu       $at, $v0, 0x3
        ctx->r1 = ctx->r2 < 0X3 ? 1 : 0;
            goto L_0F0004A0;
    }
    // 0x0F000484: sltiu       $at, $v0, 0x3
    ctx->r1 = ctx->r2 < 0X3 ? 1 : 0;
    // 0x0F000488: beq         $at, $zero, L_0F0004A0
    if (ctx->r1 == 0) {
        // 0x0F00048C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_0F0004A0;
    }
    // 0x0F00048C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000490: sll         $a1, $v0, 16
    ctx->r5 = S32(ctx->r2 << 16);
    // 0x0F000494: jal         0x0F0004F4
    // 0x0F000498: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    static_184_0F0004F4(rdram, ctx);
        goto after_4;
    // 0x0F000498: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    after_4:
    // 0x0F00049C: sw          $zero, 0x54($s0)
    MEM_W(0X54, ctx->r16) = 0;
L_0F0004A0:
    // 0x0F0004A0: lw          $t4, 0x58($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X58);
    // 0x0F0004A4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0004A8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0004AC: beq         $t4, $zero, L_0F0004BC
    if (ctx->r12 == 0) {
        // 0x0F0004B0: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_0F0004BC;
    }
    // 0x0F0004B0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x0F0004B4: jalr        $t9
    // 0x0F0004B8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F0004B8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_5:
L_0F0004BC:
    // 0x0F0004BC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x0F0004C0: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F0004C4: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x0F0004C8: jr          $ra
    // 0x0F0004CC: nop

    return;
    // 0x0F0004CC: nop

;}
RECOMP_FUNC void ni_ovl_130_func_0F0004D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0004D0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F0004D4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F0004D8: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F0004DC: jalr        $t9
    // 0x0F0004E0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0004E0: nop

    after_0:
    // 0x0F0004E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F0004E8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0004EC: jr          $ra
    // 0x0F0004F0: nop

    return;
    // 0x0F0004F0: nop

    // 0x0F0004F4: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x0F0004F8: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x0F0004FC: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x0F000500: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x0F000504: beq         $a1, $at, L_0F000520
    if (ctx->r5 == ctx->r1) {
        // 0x0F000508: lw          $v0, 0x24($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X24);
            goto L_0F000520;
    }
    // 0x0F000508: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x0F00050C: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x0F000510: beql        $a1, $at, L_0F000560
    if (ctx->r5 == ctx->r1) {
        // 0x0F000514: lw          $v1, 0x14($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X14);
            goto L_0F000560;
    }
    goto skip_0;
    // 0x0F000514: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
    skip_0:
    // 0x0F000518: jr          $ra
    // 0x0F00051C: nop

    return;
    // 0x0F00051C: nop

L_0F000520:
    // 0x0F000520: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
    // 0x0F000524: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x0F000528: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x0F00052C: sw          $zero, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = 0;
    // 0x0F000530: lw          $t6, 0x14($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X14);
    // 0x0F000534: lw          $v1, 0x14($t6)
    ctx->r3 = MEM_W(ctx->r14, 0X14);
    // 0x0F000538: sw          $zero, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = 0;
    // 0x0F00053C: lw          $t7, 0x14($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X14);
    // 0x0F000540: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x0F000544: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x0F000548: lw          $v1, 0x10($t9)
    ctx->r3 = MEM_W(ctx->r25, 0X10);
    // 0x0F00054C: lh          $t0, 0x0($v1)
    ctx->r8 = MEM_H(ctx->r3, 0X0);
    // 0x0F000550: and         $t1, $t0, $at
    ctx->r9 = ctx->r8 & ctx->r1;
    // 0x0F000554: jr          $ra
    // 0x0F000558: sh          $t1, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r9;
    return;
    // 0x0F000558: sh          $t1, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r9;
    // 0x0F00055C: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
L_0F000560:
    // 0x0F000560: lw          $t2, 0x74($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X74);
    // 0x0F000564: sw          $t2, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = ctx->r10;
    // 0x0F000568: lw          $t3, 0x14($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X14);
    // 0x0F00056C: lw          $v1, 0x14($t3)
    ctx->r3 = MEM_W(ctx->r11, 0X14);
    // 0x0F000570: lw          $t4, 0x74($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X74);
    // 0x0F000574: sw          $t4, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = ctx->r12;
    // 0x0F000578: lw          $t5, 0x14($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X14);
    // 0x0F00057C: lw          $t6, 0x14($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X14);
    // 0x0F000580: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x0F000584: lw          $v1, 0x10($t7)
    ctx->r3 = MEM_W(ctx->r15, 0X10);
    // 0x0F000588: lh          $t8, 0x0($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X0);
    // 0x0F00058C: ori         $t9, $t8, 0x8000
    ctx->r25 = ctx->r24 | 0X8000;
    // 0x0F000590: sh          $t9, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r25;
    // 0x0F000594: jr          $ra
    // 0x0F000598: nop

    return;
    // 0x0F000598: nop

;}
RECOMP_FUNC void ni_ovl_131_func_0F000000(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000000: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000004: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000008: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00000C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000010: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000014: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000018: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00001C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000020: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000024: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000028: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00002C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000030: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000034: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000038: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00003C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000040: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000044: lw          $t9, 0xBF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0XBF0);
    // 0x0F000048: jalr        $t9
    // 0x0F00004C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00004C: nop

    after_0:
    // 0x0F000050: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F000054: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F000058: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F00005C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F000060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000064: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000068: jr          $ra
    // 0x0F00006C: nop

    return;
    // 0x0F00006C: nop

;}
RECOMP_FUNC void ni_ovl_131_func_0F000070(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000070: addiu       $sp, $sp, -0x90
    ctx->r29 = ADD32(ctx->r29, -0X90);
    // 0x0F000074: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F000078: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F00007C: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x0F000080: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x0F000084: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x0F000088: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x0F00008C: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F000090: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000094: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x0F000098: jalr        $t9
    // 0x0F00009C: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00009C: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_0:
    // 0x0F0000A0: sw          $v0, 0x24($s2)
    MEM_W(0X24, ctx->r18) = ctx->r2;
    // 0x0F0000A4: lhu         $t8, 0x2($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0000A8: lui         $t6, 0x600
    ctx->r14 = S32(0X600 << 16);
    // 0x0F0000AC: addiu       $t6, $t6, 0x1EC0
    ctx->r14 = ADD32(ctx->r14, 0X1EC0);
    // 0x0F0000B0: addiu       $t7, $zero, 0xD8
    ctx->r15 = ADD32(0, 0XD8);
    // 0x0F0000B4: ori         $t0, $t8, 0x1080
    ctx->r8 = ctx->r24 | 0X1080;
    // 0x0F0000B8: sw          $t6, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r14;
    // 0x0F0000BC: sw          $t7, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r15;
    // 0x0F0000C0: sh          $t0, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r8;
    // 0x0F0000C4: lwc1        $f4, 0x60($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X60);
    // 0x0F0000C8: lui         $t2, 0xF00
    ctx->r10 = S32(0XF00 << 16);
    // 0x0F0000CC: addiu       $t2, $t2, 0xBFC
    ctx->r10 = ADD32(ctx->r10, 0XBFC);
    // 0x0F0000D0: swc1        $f4, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f4.u32l;
    // 0x0F0000D4: lwc1        $f6, 0x64($s2)
    ctx->f6.u32l = MEM_W(ctx->r18, 0X64);
    // 0x0F0000D8: addiu       $t5, $t2, 0x30
    ctx->r13 = ADD32(ctx->r10, 0X30);
    // 0x0F0000DC: addiu       $t1, $sp, 0x54
    ctx->r9 = ADD32(ctx->r29, 0X54);
    // 0x0F0000E0: swc1        $f6, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f6.u32l;
    // 0x0F0000E4: lwc1        $f8, 0x68($s2)
    ctx->f8.u32l = MEM_W(ctx->r18, 0X68);
    // 0x0F0000E8: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    // 0x0F0000EC: addiu       $a2, $zero, 0x6
    ctx->r6 = ADD32(0, 0X6);
    // 0x0F0000F0: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
L_0F0000F4:
    // 0x0F0000F4: lw          $t4, 0x0($t2)
    ctx->r12 = MEM_W(ctx->r10, 0X0);
    // 0x0F0000F8: addiu       $t2, $t2, 0xC
    ctx->r10 = ADD32(ctx->r10, 0XC);
    // 0x0F0000FC: addiu       $t1, $t1, 0xC
    ctx->r9 = ADD32(ctx->r9, 0XC);
    // 0x0F000100: sw          $t4, -0xC($t1)
    MEM_W(-0XC, ctx->r9) = ctx->r12;
    // 0x0F000104: lw          $t3, -0x8($t2)
    ctx->r11 = MEM_W(ctx->r10, -0X8);
    // 0x0F000108: sw          $t3, -0x8($t1)
    MEM_W(-0X8, ctx->r9) = ctx->r11;
    // 0x0F00010C: lw          $t4, -0x4($t2)
    ctx->r12 = MEM_W(ctx->r10, -0X4);
    // 0x0F000110: bne         $t2, $t5, L_0F0000F4
    if (ctx->r10 != ctx->r13) {
        // 0x0F000114: sw          $t4, -0x4($t1)
        MEM_W(-0X4, ctx->r9) = ctx->r12;
            goto L_0F0000F4;
    }
    // 0x0F000114: sw          $t4, -0x4($t1)
    MEM_W(-0X4, ctx->r9) = ctx->r12;
    // 0x0F000118: lw          $a0, 0x24($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X24);
    // 0x0F00011C: lui         $v1, 0x8006
    ctx->r3 = S32(0X8006 << 16);
    // 0x0F000120: addiu       $v1, $v1, -0x6A94
    ctx->r3 = ADD32(ctx->r3, -0X6A94);
    // 0x0F000124: lui         $t9, 0x3000
    ctx->r25 = S32(0X3000 << 16);
    // 0x0F000128: sw          $t9, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r25;
    // 0x0F00012C: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    // 0x0F000130: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F000134: jalr        $v1
    // 0x0F000138: addiu       $a3, $sp, 0x60
    ctx->r7 = ADD32(ctx->r29, 0X60);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_1;
    // 0x0F000138: addiu       $a3, $sp, 0x60
    ctx->r7 = ADD32(ctx->r29, 0X60);
    after_1:
    // 0x0F00013C: beq         $v0, $zero, L_0F000208
    if (ctx->r2 == 0) {
        // 0x0F000140: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_0F000208;
    }
    // 0x0F000140: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000144: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000148: lwc1        $f0, 0xC30($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0XC30);
    // 0x0F00014C: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x0F000150: addiu       $t9, $t9, -0x3768
    ctx->r25 = ADD32(ctx->r25, -0X3768);
    // 0x0F000154: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x0F000158: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    // 0x0F00015C: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    // 0x0F000160: swc1        $f0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f0.u32l;
    // 0x0F000164: swc1        $f0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f0.u32l;
    // 0x0F000168: jalr        $t9
    // 0x0F00016C: swc1        $f0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x0F00016C: swc1        $f0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f0.u32l;
    after_2:
    // 0x0F000170: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x0F000174: addiu       $s1, $s1, -0x337C
    ctx->r17 = ADD32(ctx->r17, -0X337C);
    // 0x0F000178: lui         $a1, 0xFFC0
    ctx->r5 = S32(0XFFC0 << 16);
    // 0x0F00017C: ori         $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 | 0XFF;
    // 0x0F000180: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000184: jalr        $s1
    // 0x0F000188: addiu       $a2, $zero, 0x800
    ctx->r6 = ADD32(0, 0X800);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_3;
    // 0x0F000188: addiu       $a2, $zero, 0x800
    ctx->r6 = ADD32(0, 0X800);
    after_3:
    // 0x0F00018C: lui         $a1, 0xFFFF
    ctx->r5 = S32(0XFFFF << 16);
    // 0x0F000190: ori         $a1, $a1, 0x80
    ctx->r5 = ctx->r5 | 0X80;
    // 0x0F000194: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000198: jalr        $s1
    // 0x0F00019C: addiu       $a2, $zero, 0x400
    ctx->r6 = ADD32(0, 0X400);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_4;
    // 0x0F00019C: addiu       $a2, $zero, 0x400
    ctx->r6 = ADD32(0, 0X400);
    after_4:
    // 0x0F0001A0: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x0F0001A4: addiu       $s1, $s1, -0x2FBC
    ctx->r17 = ADD32(ctx->r17, -0X2FBC);
    // 0x0F0001A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001AC: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x0F0001B0: jalr        $s1
    // 0x0F0001B4: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_5;
    // 0x0F0001B4: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_5:
    // 0x0F0001B8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001BC: addiu       $a1, $zero, 0x1000
    ctx->r5 = ADD32(0, 0X1000);
    // 0x0F0001C0: jalr        $s1
    // 0x0F0001C4: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_6;
    // 0x0F0001C4: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_6:
    // 0x0F0001C8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001CC: lui         $a1, 0x20
    ctx->r5 = S32(0X20 << 16);
    // 0x0F0001D0: jalr        $s1
    // 0x0F0001D4: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_7;
    // 0x0F0001D4: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    after_7:
    // 0x0F0001D8: lui         $v1, 0x8006
    ctx->r3 = S32(0X8006 << 16);
    // 0x0F0001DC: addiu       $v1, $v1, -0x35B8
    ctx->r3 = ADD32(ctx->r3, -0X35B8);
    // 0x0F0001E0: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x0F0001E4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001E8: addiu       $a1, $zero, 0x40
    ctx->r5 = ADD32(0, 0X40);
    // 0x0F0001EC: jalr        $v1
    // 0x0F0001F0: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_8;
    // 0x0F0001F0: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    after_8:
    // 0x0F0001F4: lw          $t9, 0x34($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X34);
    // 0x0F0001F8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0001FC: addiu       $a1, $zero, 0x20
    ctx->r5 = ADD32(0, 0X20);
    // 0x0F000200: jalr        $t9
    // 0x0F000204: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x0F000204: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_9:
L_0F000208:
    // 0x0F000208: lw          $t9, 0x3C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X3C);
    // 0x0F00020C: lw          $a0, 0x24($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X24);
    // 0x0F000210: lui         $t6, 0x3000
    ctx->r14 = S32(0X3000 << 16);
    // 0x0F000214: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x0F000218: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F00021C: addiu       $a1, $zero, 0xD
    ctx->r5 = ADD32(0, 0XD);
    // 0x0F000220: addiu       $a2, $zero, 0xD
    ctx->r6 = ADD32(0, 0XD);
    // 0x0F000224: jalr        $t9
    // 0x0F000228: addiu       $a3, $sp, 0x78
    ctx->r7 = ADD32(ctx->r29, 0X78);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x0F000228: addiu       $a3, $sp, 0x78
    ctx->r7 = ADD32(ctx->r29, 0X78);
    after_10:
    // 0x0F00022C: beq         $v0, $zero, L_0F0002B8
    if (ctx->r2 == 0) {
        // 0x0F000230: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_0F0002B8;
    }
    // 0x0F000230: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000234: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000238: lwc1        $f0, 0xC34($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0XC34);
    // 0x0F00023C: lui         $t7, 0x8006
    ctx->r15 = S32(0X8006 << 16);
    // 0x0F000240: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x0F000244: addiu       $t9, $t9, -0x3768
    ctx->r25 = ADD32(ctx->r25, -0X3768);
    // 0x0F000248: addiu       $t7, $t7, -0x35B8
    ctx->r15 = ADD32(ctx->r15, -0X35B8);
    // 0x0F00024C: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x0F000250: addiu       $s1, $s1, -0x2FBC
    ctx->r17 = ADD32(ctx->r17, -0X2FBC);
    // 0x0F000254: sw          $t7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r15;
    // 0x0F000258: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x0F00025C: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    // 0x0F000260: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    // 0x0F000264: swc1        $f0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f0.u32l;
    // 0x0F000268: swc1        $f0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f0.u32l;
    // 0x0F00026C: jalr        $t9
    // 0x0F000270: swc1        $f0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x0F000270: swc1        $f0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f0.u32l;
    after_11:
    // 0x0F000274: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000278: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x0F00027C: jalr        $s1
    // 0x0F000280: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_12;
    // 0x0F000280: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_12:
    // 0x0F000284: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000288: addiu       $a1, $zero, 0x1000
    ctx->r5 = ADD32(0, 0X1000);
    // 0x0F00028C: jalr        $s1
    // 0x0F000290: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_13;
    // 0x0F000290: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_13:
    // 0x0F000294: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000298: lui         $a1, 0x20
    ctx->r5 = S32(0X20 << 16);
    // 0x0F00029C: jalr        $s1
    // 0x0F0002A0: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_14;
    // 0x0F0002A0: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    after_14:
    // 0x0F0002A4: lw          $t9, 0x34($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X34);
    // 0x0F0002A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0002AC: addiu       $a1, $zero, 0x60
    ctx->r5 = ADD32(0, 0X60);
    // 0x0F0002B0: jalr        $t9
    // 0x0F0002B4: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_15;
    // 0x0F0002B4: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    after_15:
L_0F0002B8:
    // 0x0F0002B8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0002BC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0002C0: sw          $zero, 0x58($s2)
    MEM_W(0X58, ctx->r18) = 0;
    // 0x0F0002C4: addiu       $a0, $s2, 0x8
    ctx->r4 = ADD32(ctx->r18, 0X8);
    // 0x0F0002C8: jalr        $t9
    // 0x0F0002CC: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_16;
    // 0x0F0002CC: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    after_16:
    // 0x0F0002D0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0002D4: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F0002D8: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F0002DC: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F0002E0: jr          $ra
    // 0x0F0002E4: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
    return;
    // 0x0F0002E4: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
;}
RECOMP_FUNC void ni_ovl_131_func_0F0002E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0002E8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F0002EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F0002F0: lw          $t6, 0x58($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X58);
    // 0x0F0002F4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x0F0002F8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x0F0002FC: beq         $t6, $zero, L_0F000310
    if (ctx->r14 == 0) {
        // 0x0F000300: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_0F000310;
    }
    // 0x0F000300: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000304: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000308: jalr        $t9
    // 0x0F00030C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00030C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_0F000310:
    // 0x0F000310: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000314: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000318: jr          $ra
    // 0x0F00031C: nop

    return;
    // 0x0F00031C: nop

;}
RECOMP_FUNC void ni_ovl_131_func_0F000320(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000320: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000324: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000328: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F00032C: jalr        $t9
    // 0x0F000330: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000330: nop

    after_0:
    // 0x0F000334: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000338: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F00033C: jr          $ra
    // 0x0F000340: nop

    return;
    // 0x0F000340: nop

;}
RECOMP_FUNC void ni_ovl_131_func_0F000350(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000350: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000354: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000358: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x0F00035C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x0F000360: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x0F000364: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x0F000368: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x0F00036C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x0F000370: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x0F000374: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x0F000378: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x0F00037C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x0F000380: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x0F000384: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x0F000388: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x0F00038C: lui         $t9, 0xF00
    ctx->r25 = S32(0XF00 << 16);
    // 0x0F000390: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x0F000394: lw          $t9, 0xC40($t9)
    ctx->r25 = MEM_W(ctx->r25, 0XC40);
    // 0x0F000398: jalr        $t9
    // 0x0F00039C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F00039C: nop

    after_0:
    // 0x0F0003A0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x0F0003A4: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x0F0003A8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x0F0003AC: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x0F0003B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F0003B4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F0003B8: jr          $ra
    // 0x0F0003BC: nop

    return;
    // 0x0F0003BC: nop

;}
RECOMP_FUNC void ni_ovl_131_func_0F0003C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F0003C0: addiu       $sp, $sp, -0x100
    ctx->r29 = ADD32(ctx->r29, -0X100);
    // 0x0F0003C4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x0F0003C8: sw          $ra, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r31;
    // 0x0F0003CC: sw          $s7, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r23;
    // 0x0F0003D0: addiu       $t9, $t9, 0x34AC
    ctx->r25 = ADD32(ctx->r25, 0X34AC);
    // 0x0F0003D4: or          $s7, $a0, $zero
    ctx->r23 = ctx->r4 | 0;
    // 0x0F0003D8: sw          $fp, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r30;
    // 0x0F0003DC: sw          $s6, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r22;
    // 0x0F0003E0: sw          $s5, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r21;
    // 0x0F0003E4: sw          $s4, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r20;
    // 0x0F0003E8: sw          $s3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r19;
    // 0x0F0003EC: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x0F0003F0: sw          $s1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r17;
    // 0x0F0003F4: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x0F0003F8: jalr        $t9
    // 0x0F0003FC: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F0003FC: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    after_0:
    // 0x0F000400: lui         $s5, 0x8000
    ctx->r21 = S32(0X8000 << 16);
    // 0x0F000404: addiu       $s5, $s5, 0x5A30
    ctx->r21 = ADD32(ctx->r21, 0X5A30);
    // 0x0F000408: or          $s4, $v0, $zero
    ctx->r20 = ctx->r2 | 0;
    // 0x0F00040C: addiu       $a0, $zero, 0x1010
    ctx->r4 = ADD32(0, 0X1010);
    // 0x0F000410: jalr        $s5
    // 0x0F000414: lw          $a1, 0x24($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X24);
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_1;
    // 0x0F000414: lw          $a1, 0x24($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X24);
    after_1:
    // 0x0F000418: sw          $v0, 0x24($s7)
    MEM_W(0X24, ctx->r23) = ctx->r2;
    // 0x0F00041C: lui         $at, 0x4310
    ctx->r1 = S32(0X4310 << 16);
    // 0x0F000420: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000424: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000428: lui         $at, 0xC3F0
    ctx->r1 = S32(0XC3F0 << 16);
    // 0x0F00042C: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F000430: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000434: lui         $t6, 0x600
    ctx->r14 = S32(0X600 << 16);
    // 0x0F000438: lui         $s0, 0x8001
    ctx->r16 = S32(0X8001 << 16);
    // 0x0F00043C: addiu       $s1, $zero, 0xD8
    ctx->r17 = ADD32(0, 0XD8);
    // 0x0F000440: addiu       $t6, $t6, 0x7410
    ctx->r14 = ADD32(ctx->r14, 0X7410);
    // 0x0F000444: addiu       $s0, $s0, 0x36C4
    ctx->r16 = ADD32(ctx->r16, 0X36C4);
    // 0x0F000448: ori         $t8, $t7, 0x1080
    ctx->r24 = ctx->r15 | 0X1080;
    // 0x0F00044C: or          $s2, $v0, $zero
    ctx->r18 = ctx->r2 | 0;
    // 0x0F000450: sw          $t6, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r14;
    // 0x0F000454: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F000458: sh          $t8, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r24;
    // 0x0F00045C: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x0F000460: lui         $a1, 0xC
    ctx->r5 = S32(0XC << 16);
    // 0x0F000464: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F000468: swc1        $f4, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f4.u32l;
    // 0x0F00046C: swc1        $f6, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f6.u32l;
    // 0x0F000470: jalr        $s0
    // 0x0F000474: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_2;
    // 0x0F000474: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    after_2:
    // 0x0F000478: lui         $a1, 0x20
    ctx->r5 = S32(0X20 << 16);
    // 0x0F00047C: ori         $a1, $a1, 0x1
    ctx->r5 = ctx->r5 | 0X1;
    // 0x0F000480: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x0F000484: jalr        $s0
    // 0x0F000488: addiu       $a2, $s2, 0x50
    ctx->r6 = ADD32(ctx->r18, 0X50);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_3;
    // 0x0F000488: addiu       $a2, $s2, 0x50
    ctx->r6 = ADD32(ctx->r18, 0X50);
    after_3:
    // 0x0F00048C: lui         $s0, 0x8000
    ctx->r16 = S32(0X8000 << 16);
    // 0x0F000490: addiu       $s0, $s0, 0x5ABC
    ctx->r16 = ADD32(ctx->r16, 0X5ABC);
    // 0x0F000494: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x0F000498: jalr        $s0
    // 0x0F00049C: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_4;
    // 0x0F00049C: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    after_4:
    // 0x0F0004A0: lui         $at, 0x424C
    ctx->r1 = S32(0X424C << 16);
    // 0x0F0004A4: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x0F0004A8: sw          $v0, 0x60($s7)
    MEM_W(0X60, ctx->r23) = ctx->r2;
    // 0x0F0004AC: lui         $at, 0xC258
    ctx->r1 = S32(0XC258 << 16);
    // 0x0F0004B0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0004B4: lhu         $t0, 0x2($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0004B8: lui         $at, 0x4316
    ctx->r1 = S32(0X4316 << 16);
    // 0x0F0004BC: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F0004C0: lui         $s3, 0x600
    ctx->r19 = S32(0X600 << 16);
    // 0x0F0004C4: addiu       $s3, $s3, 0x7DB0
    ctx->r19 = ADD32(ctx->r19, 0X7DB0);
    // 0x0F0004C8: ori         $t1, $t0, 0x40
    ctx->r9 = ctx->r8 | 0X40;
    // 0x0F0004CC: sw          $s3, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r19;
    // 0x0F0004D0: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F0004D4: sh          $t1, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r9;
    // 0x0F0004D8: swc1        $f20, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f20.u32l;
    // 0x0F0004DC: swc1        $f10, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f10.u32l;
    // 0x0F0004E0: swc1        $f16, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f16.u32l;
    // 0x0F0004E4: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    // 0x0F0004E8: jalr        $s0
    // 0x0F0004EC: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_5;
    // 0x0F0004EC: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_5:
    // 0x0F0004F0: lui         $at, 0xC258
    ctx->r1 = S32(0XC258 << 16);
    // 0x0F0004F4: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F0004F8: sw          $v0, 0x60($s7)
    MEM_W(0X60, ctx->r23) = ctx->r2;
    // 0x0F0004FC: lui         $at, 0x42DE
    ctx->r1 = S32(0X42DE << 16);
    // 0x0F000500: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000504: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000508: lui         $at, 0xC210
    ctx->r1 = S32(0XC210 << 16);
    // 0x0F00050C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000510: ori         $t3, $t2, 0x40
    ctx->r11 = ctx->r10 | 0X40;
    // 0x0F000514: sw          $s3, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r19;
    // 0x0F000518: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F00051C: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x0F000520: swc1        $f18, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f18.u32l;
    // 0x0F000524: swc1        $f4, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f4.u32l;
    // 0x0F000528: swc1        $f6, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f6.u32l;
    // 0x0F00052C: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    // 0x0F000530: jalr        $s0
    // 0x0F000534: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_6;
    // 0x0F000534: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_6:
    // 0x0F000538: sw          $v0, 0x60($s7)
    MEM_W(0X60, ctx->r23) = ctx->r2;
    // 0x0F00053C: lui         $at, 0x4258
    ctx->r1 = S32(0X4258 << 16);
    // 0x0F000540: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000544: lhu         $t4, 0x2($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000548: lui         $at, 0x4316
    ctx->r1 = S32(0X4316 << 16);
    // 0x0F00054C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000550: ori         $t5, $t4, 0x40
    ctx->r13 = ctx->r12 | 0X40;
    // 0x0F000554: sw          $s3, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r19;
    // 0x0F000558: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F00055C: sh          $t5, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r13;
    // 0x0F000560: swc1        $f20, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f20.u32l;
    // 0x0F000564: swc1        $f8, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f8.u32l;
    // 0x0F000568: swc1        $f10, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f10.u32l;
    // 0x0F00056C: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    // 0x0F000570: jalr        $s0
    // 0x0F000574: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_7;
    // 0x0F000574: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_7:
    // 0x0F000578: lui         $at, 0x4258
    ctx->r1 = S32(0X4258 << 16);
    // 0x0F00057C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000580: sw          $v0, 0x60($s7)
    MEM_W(0X60, ctx->r23) = ctx->r2;
    // 0x0F000584: lui         $at, 0x42DE
    ctx->r1 = S32(0X42DE << 16);
    // 0x0F000588: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F00058C: lhu         $t9, 0x2($v0)
    ctx->r25 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000590: lui         $at, 0xC210
    ctx->r1 = S32(0XC210 << 16);
    // 0x0F000594: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000598: ori         $t6, $t9, 0x40
    ctx->r14 = ctx->r25 | 0X40;
    // 0x0F00059C: sw          $s3, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r19;
    // 0x0F0005A0: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F0005A4: sh          $t6, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r14;
    // 0x0F0005A8: swc1        $f16, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f16.u32l;
    // 0x0F0005AC: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x0F0005B0: swc1        $f4, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f4.u32l;
    // 0x0F0005B4: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    // 0x0F0005B8: jalr        $s0
    // 0x0F0005BC: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_8;
    // 0x0F0005BC: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_8:
    // 0x0F0005C0: lhu         $t8, 0x2($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0005C4: lui         $at, 0x4331
    ctx->r1 = S32(0X4331 << 16);
    // 0x0F0005C8: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x0F0005CC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0005D0: lui         $t7, 0x600
    ctx->r15 = S32(0X600 << 16);
    // 0x0F0005D4: addiu       $t7, $t7, 0x7CD0
    ctx->r15 = ADD32(ctx->r15, 0X7CD0);
    // 0x0F0005D8: ori         $t0, $t8, 0x40
    ctx->r8 = ctx->r24 | 0X40;
    // 0x0F0005DC: sw          $t7, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r15;
    // 0x0F0005E0: sh          $t0, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r8;
    // 0x0F0005E4: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F0005E8: swc1        $f20, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f20.u32l;
    // 0x0F0005EC: swc1        $f6, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f6.u32l;
    // 0x0F0005F0: swc1        $f8, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f8.u32l;
    // 0x0F0005F4: lw          $a1, 0x24($s4)
    ctx->r5 = MEM_W(ctx->r20, 0X24);
    // 0x0F0005F8: jalr        $s5
    // 0x0F0005FC: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_9;
    // 0x0F0005FC: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_9:
    // 0x0F000600: sw          $v0, 0x5C($s7)
    MEM_W(0X5C, ctx->r23) = ctx->r2;
    // 0x0F000604: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000608: lh          $t4, 0x0($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X0);
    // 0x0F00060C: lui         $t1, 0x600
    ctx->r9 = S32(0X600 << 16);
    // 0x0F000610: addiu       $t1, $t1, 0x7BF0
    ctx->r9 = ADD32(ctx->r9, 0X7BF0);
    // 0x0F000614: ori         $t3, $t2, 0x1060
    ctx->r11 = ctx->r10 | 0X1060;
    // 0x0F000618: ori         $t5, $t4, 0x200
    ctx->r13 = ctx->r12 | 0X200;
    // 0x0F00061C: sw          $t1, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r9;
    // 0x0F000620: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F000624: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x0F000628: sh          $t5, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r13;
    // 0x0F00062C: lwc1        $f10, 0x50($s2)
    ctx->f10.u32l = MEM_W(ctx->r18, 0X50);
    // 0x0F000630: lui         $at, 0x41AC
    ctx->r1 = S32(0X41AC << 16);
    // 0x0F000634: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000638: swc1        $f10, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f10.u32l;
    // 0x0F00063C: lwc1        $f16, 0x54($s2)
    ctx->f16.u32l = MEM_W(ctx->r18, 0X54);
    // 0x0F000640: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000644: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x0F000648: add.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x0F00064C: swc1        $f4, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f4.u32l;
    // 0x0F000650: lwc1        $f8, 0xCB0($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0XCB0);
    // 0x0F000654: lwc1        $f6, 0x58($s2)
    ctx->f6.u32l = MEM_W(ctx->r18, 0X58);
    // 0x0F000658: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x0F00065C: swc1        $f10, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f10.u32l;
    // 0x0F000660: jalr        $s0
    // 0x0F000664: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_10;
    // 0x0F000664: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    after_10:
    // 0x0F000668: lui         $at, 0x3FC0
    ctx->r1 = S32(0X3FC0 << 16);
    // 0x0F00066C: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x0F000670: lui         $at, 0xC260
    ctx->r1 = S32(0XC260 << 16);
    // 0x0F000674: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000678: lui         $at, 0x42F4
    ctx->r1 = S32(0X42F4 << 16);
    // 0x0F00067C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000680: lhu         $t9, 0x2($v0)
    ctx->r25 = MEM_HU(ctx->r2, 0X2);
    // 0x0F000684: lui         $at, 0x42A4
    ctx->r1 = S32(0X42A4 << 16);
    // 0x0F000688: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F00068C: lui         $s2, 0x600
    ctx->r18 = S32(0X600 << 16);
    // 0x0F000690: addiu       $s2, $s2, 0x7E90
    ctx->r18 = ADD32(ctx->r18, 0X7E90);
    // 0x0F000694: ori         $t6, $t9, 0x60
    ctx->r14 = ctx->r25 | 0X60;
    // 0x0F000698: sw          $s2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r18;
    // 0x0F00069C: sh          $t6, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r14;
    // 0x0F0006A0: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F0006A4: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F0006A8: swc1        $f20, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f20.u32l;
    // 0x0F0006AC: swc1        $f20, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f20.u32l;
    // 0x0F0006B0: swc1        $f16, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f16.u32l;
    // 0x0F0006B4: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x0F0006B8: swc1        $f4, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f4.u32l;
    // 0x0F0006BC: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    // 0x0F0006C0: jalr        $s0
    // 0x0F0006C4: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_11;
    // 0x0F0006C4: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_11:
    // 0x0F0006C8: lui         $at, 0xC260
    ctx->r1 = S32(0XC260 << 16);
    // 0x0F0006CC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F0006D0: lui         $at, 0x42F4
    ctx->r1 = S32(0X42F4 << 16);
    // 0x0F0006D4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F0006D8: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
    // 0x0F0006DC: lui         $at, 0x4198
    ctx->r1 = S32(0X4198 << 16);
    // 0x0F0006E0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F0006E4: ori         $t8, $t7, 0x60
    ctx->r24 = ctx->r15 | 0X60;
    // 0x0F0006E8: sw          $s2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r18;
    // 0x0F0006EC: sh          $t8, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r24;
    // 0x0F0006F0: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F0006F4: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F0006F8: swc1        $f20, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f20.u32l;
    // 0x0F0006FC: swc1        $f20, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f20.u32l;
    // 0x0F000700: swc1        $f6, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f6.u32l;
    // 0x0F000704: swc1        $f8, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f8.u32l;
    // 0x0F000708: swc1        $f10, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f10.u32l;
    // 0x0F00070C: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    // 0x0F000710: jalr        $s0
    // 0x0F000714: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_12;
    // 0x0F000714: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_12:
    // 0x0F000718: lui         $at, 0x4260
    ctx->r1 = S32(0X4260 << 16);
    // 0x0F00071C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000720: lui         $at, 0x42F4
    ctx->r1 = S32(0X42F4 << 16);
    // 0x0F000724: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F000728: lhu         $t0, 0x2($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X2);
    // 0x0F00072C: lui         $at, 0x4198
    ctx->r1 = S32(0X4198 << 16);
    // 0x0F000730: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000734: ori         $t1, $t0, 0x60
    ctx->r9 = ctx->r8 | 0X60;
    // 0x0F000738: sw          $s2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r18;
    // 0x0F00073C: sh          $t1, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r9;
    // 0x0F000740: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F000744: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F000748: swc1        $f20, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f20.u32l;
    // 0x0F00074C: swc1        $f20, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f20.u32l;
    // 0x0F000750: swc1        $f16, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f16.u32l;
    // 0x0F000754: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x0F000758: swc1        $f4, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f4.u32l;
    // 0x0F00075C: lw          $a1, 0x24($s7)
    ctx->r5 = MEM_W(ctx->r23, 0X24);
    // 0x0F000760: jalr        $s0
    // 0x0F000764: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_13;
    // 0x0F000764: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    after_13:
    // 0x0F000768: lui         $at, 0x4260
    ctx->r1 = S32(0X4260 << 16);
    // 0x0F00076C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x0F000770: lui         $at, 0x42F4
    ctx->r1 = S32(0X42F4 << 16);
    // 0x0F000774: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000778: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x0F00077C: lui         $at, 0x42A4
    ctx->r1 = S32(0X42A4 << 16);
    // 0x0F000780: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x0F000784: lui         $t5, 0xF00
    ctx->r13 = S32(0XF00 << 16);
    // 0x0F000788: addiu       $t5, $t5, 0xC4C
    ctx->r13 = ADD32(ctx->r13, 0XC4C);
    // 0x0F00078C: ori         $t3, $t2, 0x60
    ctx->r11 = ctx->r10 | 0X60;
    // 0x0F000790: sw          $s2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r18;
    // 0x0F000794: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x0F000798: sw          $s1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r17;
    // 0x0F00079C: swc1        $f20, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f20.u32l;
    // 0x0F0007A0: swc1        $f20, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f20.u32l;
    // 0x0F0007A4: swc1        $f20, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f20.u32l;
    // 0x0F0007A8: addiu       $t7, $t5, 0x60
    ctx->r15 = ADD32(ctx->r13, 0X60);
    // 0x0F0007AC: addiu       $t4, $sp, 0x8C
    ctx->r12 = ADD32(ctx->r29, 0X8C);
    // 0x0F0007B0: swc1        $f6, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f6.u32l;
    // 0x0F0007B4: swc1        $f8, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f8.u32l;
    // 0x0F0007B8: swc1        $f10, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f10.u32l;
L_0F0007BC:
    // 0x0F0007BC: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x0F0007C0: addiu       $t5, $t5, 0xC
    ctx->r13 = ADD32(ctx->r13, 0XC);
    // 0x0F0007C4: addiu       $t4, $t4, 0xC
    ctx->r12 = ADD32(ctx->r12, 0XC);
    // 0x0F0007C8: sw          $t6, -0xC($t4)
    MEM_W(-0XC, ctx->r12) = ctx->r14;
    // 0x0F0007CC: lw          $t9, -0x8($t5)
    ctx->r25 = MEM_W(ctx->r13, -0X8);
    // 0x0F0007D0: sw          $t9, -0x8($t4)
    MEM_W(-0X8, ctx->r12) = ctx->r25;
    // 0x0F0007D4: lw          $t6, -0x4($t5)
    ctx->r14 = MEM_W(ctx->r13, -0X4);
    // 0x0F0007D8: bne         $t5, $t7, L_0F0007BC
    if (ctx->r13 != ctx->r15) {
        // 0x0F0007DC: sw          $t6, -0x4($t4)
        MEM_W(-0X4, ctx->r12) = ctx->r14;
            goto L_0F0007BC;
    }
    // 0x0F0007DC: sw          $t6, -0x4($t4)
    MEM_W(-0X4, ctx->r12) = ctx->r14;
    // 0x0F0007E0: sh          $zero, 0x50($s7)
    MEM_H(0X50, ctx->r23) = 0;
    // 0x0F0007E4: lui         $fp, 0x8006
    ctx->r30 = S32(0X8006 << 16);
    // 0x0F0007E8: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0007EC: lwc1        $f20, 0xCB4($at)
    ctx->f20.u32l = MEM_W(ctx->r1, 0XCB4);
    // 0x0F0007F0: addiu       $fp, $fp, -0x6A94
    ctx->r30 = ADD32(ctx->r30, -0X6A94);
    // 0x0F0007F4: addiu       $s5, $sp, 0x8C
    ctx->r21 = ADD32(ctx->r29, 0X8C);
    // 0x0F0007F8: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
    // 0x0F0007FC: addiu       $s3, $s7, 0x18
    ctx->r19 = ADD32(ctx->r23, 0X18);
L_0F000800:
    // 0x0F000800: lw          $a0, 0x24($s7)
    ctx->r4 = MEM_W(ctx->r23, 0X24);
    // 0x0F000804: lui         $t8, 0x2000
    ctx->r24 = S32(0X2000 << 16);
    // 0x0F000808: sw          $t8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r24;
    // 0x0F00080C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F000810: addiu       $a1, $zero, 0x13
    ctx->r5 = ADD32(0, 0X13);
    // 0x0F000814: addiu       $a2, $zero, 0x13
    ctx->r6 = ADD32(0, 0X13);
    // 0x0F000818: jalr        $fp
    // 0x0F00081C: or          $a3, $s5, $zero
    ctx->r7 = ctx->r21 | 0;
    LOOKUP_FUNC(ctx->r30)(rdram, ctx);
        goto after_14;
    // 0x0F00081C: or          $a3, $s5, $zero
    ctx->r7 = ctx->r21 | 0;
    after_14:
    // 0x0F000820: beq         $v0, $zero, L_0F0008E0
    if (ctx->r2 == 0) {
        // 0x0F000824: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_0F0008E0;
    }
    // 0x0F000824: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000828: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x0F00082C: addiu       $t9, $t9, -0x3768
    ctx->r25 = ADD32(ctx->r25, -0X3768);
    // 0x0F000830: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x0F000834: lui         $s2, 0x8006
    ctx->r18 = S32(0X8006 << 16);
    // 0x0F000838: lui         $s4, 0x8006
    ctx->r20 = S32(0X8006 << 16);
    // 0x0F00083C: swc1        $f20, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->f20.u32l;
    // 0x0F000840: swc1        $f20, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->f20.u32l;
    // 0x0F000844: swc1        $f20, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->f20.u32l;
    // 0x0F000848: addiu       $s4, $s4, -0x2D20
    ctx->r20 = ADD32(ctx->r20, -0X2D20);
    // 0x0F00084C: addiu       $s2, $s2, -0x35B8
    ctx->r18 = ADD32(ctx->r18, -0X35B8);
    // 0x0F000850: addiu       $s1, $s1, -0x2FBC
    ctx->r17 = ADD32(ctx->r17, -0X2FBC);
    // 0x0F000854: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x0F000858: addiu       $a1, $sp, 0x80
    ctx->r5 = ADD32(ctx->r29, 0X80);
    // 0x0F00085C: jalr        $t9
    // 0x0F000860: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_15;
    // 0x0F000860: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    after_15:
    // 0x0F000864: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000868: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x0F00086C: jalr        $s1
    // 0x0F000870: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_16;
    // 0x0F000870: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_16:
    // 0x0F000874: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000878: addiu       $a1, $zero, 0x1000
    ctx->r5 = ADD32(0, 0X1000);
    // 0x0F00087C: jalr        $s1
    // 0x0F000880: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_17;
    // 0x0F000880: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_17:
    // 0x0F000884: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000888: addiu       $a1, $zero, 0x40
    ctx->r5 = ADD32(0, 0X40);
    // 0x0F00088C: jalr        $s2
    // 0x0F000890: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_18;
    // 0x0F000890: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    after_18:
    // 0x0F000894: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000898: addiu       $a1, $zero, 0x20
    ctx->r5 = ADD32(0, 0X20);
    // 0x0F00089C: jalr        $s2
    // 0x0F0008A0: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_19;
    // 0x0F0008A0: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_19:
    // 0x0F0008A4: jalr        $s4
    // 0x0F0008A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_20;
    // 0x0F0008A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_20:
    // 0x0F0008AC: beq         $v0, $zero, L_0F0008D8
    if (ctx->r2 == 0) {
        // 0x0F0008B0: lui         $at, 0xF00
        ctx->r1 = S32(0XF00 << 16);
            goto L_0F0008D8;
    }
    // 0x0F0008B0: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0008B4: sw          $s0, 0x28($v0)
    MEM_W(0X28, ctx->r2) = ctx->r16;
    // 0x0F0008B8: lwc1        $f16, 0xCB8($at)
    ctx->f16.u32l = MEM_W(ctx->r1, 0XCB8);
    // 0x0F0008BC: lui         $at, 0x420C
    ctx->r1 = S32(0X420C << 16);
    // 0x0F0008C0: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x0F0008C4: lui         $t0, 0xC000
    ctx->r8 = S32(0XC000 << 16);
    // 0x0F0008C8: ori         $t0, $t0, 0x20
    ctx->r8 = ctx->r8 | 0X20;
    // 0x0F0008CC: sw          $t0, 0x14($v0)
    MEM_W(0X14, ctx->r2) = ctx->r8;
    // 0x0F0008D0: swc1        $f16, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f16.u32l;
    // 0x0F0008D4: swc1        $f18, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->f18.u32l;
L_0F0008D8:
    // 0x0F0008D8: b           L_0F0008E4
    // 0x0F0008DC: sw          $s0, 0x34($s3)
    MEM_W(0X34, ctx->r19) = ctx->r16;
        goto L_0F0008E4;
    // 0x0F0008DC: sw          $s0, 0x34($s3)
    MEM_W(0X34, ctx->r19) = ctx->r16;
L_0F0008E0:
    // 0x0F0008E0: sw          $zero, 0x34($s3)
    MEM_W(0X34, ctx->r19) = 0;
L_0F0008E4:
    // 0x0F0008E4: addiu       $s6, $s6, 0x4
    ctx->r22 = ADD32(ctx->r22, 0X4);
    // 0x0F0008E8: slti        $at, $s6, 0x10
    ctx->r1 = SIGNED(ctx->r22) < 0X10 ? 1 : 0;
    // 0x0F0008EC: addiu       $s5, $s5, 0xC
    ctx->r21 = ADD32(ctx->r21, 0XC);
    // 0x0F0008F0: bne         $at, $zero, L_0F000800
    if (ctx->r1 != 0) {
        // 0x0F0008F4: addiu       $s3, $s3, -0x4
        ctx->r19 = ADD32(ctx->r19, -0X4);
            goto L_0F000800;
    }
    // 0x0F0008F4: addiu       $s3, $s3, -0x4
    ctx->r19 = ADD32(ctx->r19, -0X4);
    // 0x0F0008F8: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F0008FC: lwc1        $f20, 0xCBC($at)
    ctx->f20.u32l = MEM_W(ctx->r1, 0XCBC);
    // 0x0F000900: addiu       $s3, $sp, 0xBC
    ctx->r19 = ADD32(ctx->r29, 0XBC);
    // 0x0F000904: addiu       $s6, $sp, 0xEC
    ctx->r22 = ADD32(ctx->r29, 0XEC);
    // 0x0F000908: lui         $s5, 0x3000
    ctx->r21 = S32(0X3000 << 16);
    // 0x0F00090C: lw          $a0, 0x24($s7)
    ctx->r4 = MEM_W(ctx->r23, 0X24);
L_0F000910:
    // 0x0F000910: sw          $s5, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r21;
    // 0x0F000914: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x0F000918: addiu       $a1, $zero, 0xD
    ctx->r5 = ADD32(0, 0XD);
    // 0x0F00091C: addiu       $a2, $zero, 0xD
    ctx->r6 = ADD32(0, 0XD);
    // 0x0F000920: jalr        $fp
    // 0x0F000924: or          $a3, $s3, $zero
    ctx->r7 = ctx->r19 | 0;
    LOOKUP_FUNC(ctx->r30)(rdram, ctx);
        goto after_21;
    // 0x0F000924: or          $a3, $s3, $zero
    ctx->r7 = ctx->r19 | 0;
    after_21:
    // 0x0F000928: beq         $v0, $zero, L_0F0009B0
    if (ctx->r2 == 0) {
        // 0x0F00092C: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_0F0009B0;
    }
    // 0x0F00092C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x0F000930: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x0F000934: addiu       $t9, $t9, -0x3768
    ctx->r25 = ADD32(ctx->r25, -0X3768);
    // 0x0F000938: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x0F00093C: lui         $s2, 0x8006
    ctx->r18 = S32(0X8006 << 16);
    // 0x0F000940: lui         $s4, 0x8006
    ctx->r20 = S32(0X8006 << 16);
    // 0x0F000944: swc1        $f20, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->f20.u32l;
    // 0x0F000948: swc1        $f20, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->f20.u32l;
    // 0x0F00094C: swc1        $f20, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->f20.u32l;
    // 0x0F000950: addiu       $s4, $s4, -0x347C
    ctx->r20 = ADD32(ctx->r20, -0X347C);
    // 0x0F000954: addiu       $s2, $s2, -0x35B8
    ctx->r18 = ADD32(ctx->r18, -0X35B8);
    // 0x0F000958: addiu       $s1, $s1, -0x2FBC
    ctx->r17 = ADD32(ctx->r17, -0X2FBC);
    // 0x0F00095C: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x0F000960: addiu       $a1, $sp, 0x80
    ctx->r5 = ADD32(ctx->r29, 0X80);
    // 0x0F000964: jalr        $t9
    // 0x0F000968: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_22;
    // 0x0F000968: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    after_22:
    // 0x0F00096C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000970: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x0F000974: jalr        $s1
    // 0x0F000978: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_23;
    // 0x0F000978: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_23:
    // 0x0F00097C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000980: addiu       $a1, $zero, 0x1000
    ctx->r5 = ADD32(0, 0X1000);
    // 0x0F000984: jalr        $s1
    // 0x0F000988: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_24;
    // 0x0F000988: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_24:
    // 0x0F00098C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F000990: addiu       $a1, $zero, 0x60
    ctx->r5 = ADD32(0, 0X60);
    // 0x0F000994: jalr        $s2
    // 0x0F000998: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_25;
    // 0x0F000998: addiu       $a2, $zero, 0x80
    ctx->r6 = ADD32(0, 0X80);
    after_25:
    // 0x0F00099C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x0F0009A0: addiu       $a1, $zero, 0x14
    ctx->r5 = ADD32(0, 0X14);
    // 0x0F0009A4: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x0F0009A8: jalr        $s4
    // 0x0F0009AC: addiu       $a3, $zero, 0x32
    ctx->r7 = ADD32(0, 0X32);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_26;
    // 0x0F0009AC: addiu       $a3, $zero, 0x32
    ctx->r7 = ADD32(0, 0X32);
    after_26:
L_0F0009B0:
    // 0x0F0009B0: addiu       $s3, $s3, 0xC
    ctx->r19 = ADD32(ctx->r19, 0XC);
    // 0x0F0009B4: bnel        $s3, $s6, L_0F000910
    if (ctx->r19 != ctx->r22) {
        // 0x0F0009B8: lw          $a0, 0x24($s7)
        ctx->r4 = MEM_W(ctx->r23, 0X24);
            goto L_0F000910;
    }
    goto skip_0;
    // 0x0F0009B8: lw          $a0, 0x24($s7)
    ctx->r4 = MEM_W(ctx->r23, 0X24);
    skip_0:
    // 0x0F0009BC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F0009C0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F0009C4: sw          $zero, 0x58($s7)
    MEM_W(0X58, ctx->r23) = 0;
    // 0x0F0009C8: addiu       $a0, $s7, 0x8
    ctx->r4 = ADD32(ctx->r23, 0X8);
    // 0x0F0009CC: jalr        $t9
    // 0x0F0009D0: addiu       $a1, $s7, 0xE
    ctx->r5 = ADD32(ctx->r23, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_27;
    // 0x0F0009D0: addiu       $a1, $s7, 0xE
    ctx->r5 = ADD32(ctx->r23, 0XE);
    after_27:
    // 0x0F0009D4: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
    // 0x0F0009D8: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x0F0009DC: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x0F0009E0: lw          $s1, 0x2C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X2C);
    // 0x0F0009E4: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x0F0009E8: lw          $s3, 0x34($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X34);
    // 0x0F0009EC: lw          $s4, 0x38($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X38);
    // 0x0F0009F0: lw          $s5, 0x3C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X3C);
    // 0x0F0009F4: lw          $s6, 0x40($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X40);
    // 0x0F0009F8: lw          $s7, 0x44($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X44);
    // 0x0F0009FC: lw          $fp, 0x48($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X48);
    // 0x0F000A00: jr          $ra
    // 0x0F000A04: addiu       $sp, $sp, 0x100
    ctx->r29 = ADD32(ctx->r29, 0X100);
    return;
    // 0x0F000A04: addiu       $sp, $sp, 0x100
    ctx->r29 = ADD32(ctx->r29, 0X100);
;}
RECOMP_FUNC void ni_ovl_131_func_0F000A08(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000A08: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x0F000A0C: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x0F000A10: or          $s4, $a0, $zero
    ctx->r20 = ctx->r4 | 0;
    // 0x0F000A14: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x0F000A18: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x0F000A1C: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x0F000A20: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x0F000A24: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x0F000A28: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x0F000A2C: lw          $a0, 0x54($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X54);
    // 0x0F000A30: lui         $s0, 0x8009
    ctx->r16 = S32(0X8009 << 16);
    // 0x0F000A34: addiu       $s0, $s0, 0x7330
    ctx->r16 = ADD32(ctx->r16, 0X7330);
    // 0x0F000A38: sll         $a0, $a0, 9
    ctx->r4 = S32(ctx->r4 << 9);
    // 0x0F000A3C: jalr        $s0
    // 0x0F000A40: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_0;
    // 0x0F000A40: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_0:
    // 0x0F000A44: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x0F000A48: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x0F000A4C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x0F000A50: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x0F000A54: lw          $t6, 0x54($s4)
    ctx->r14 = MEM_W(ctx->r20, 0X54);
    // 0x0F000A58: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000A5C: lw          $v1, 0x5C($s4)
    ctx->r3 = MEM_W(ctx->r20, 0X5C);
    // 0x0F000A60: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x0F000A64: sw          $t7, 0x54($s4)
    MEM_W(0X54, ctx->r20) = ctx->r15;
    // 0x0F000A68: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x0F000A6C: ldc1        $f18, 0xCC0($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0XCC0);
    // 0x0F000A70: lwc1        $f8, 0x54($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X54);
    // 0x0F000A74: lui         $s1, 0x8000
    ctx->r17 = S32(0X8000 << 16);
    // 0x0F000A78: addiu       $s1, $s1, 0x47C0
    ctx->r17 = ADD32(ctx->r17, 0X47C0);
    // 0x0F000A7C: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x0F000A80: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x0F000A84: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x0F000A88: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x0F000A8C: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x0F000A90: jalr        $s1
    // 0x0F000A94: swc1        $f10, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_1;
    // 0x0F000A94: swc1        $f10, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f10.u32l;
    after_1:
    // 0x0F000A98: andi        $s2, $v0, 0xFFFF
    ctx->r18 = ctx->r2 & 0XFFFF;
    // 0x0F000A9C: jalr        $s0
    // 0x0F000AA0: lhu         $a0, 0x50($s4)
    ctx->r4 = MEM_HU(ctx->r20, 0X50);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_2;
    // 0x0F000AA0: lhu         $a0, 0x50($s4)
    ctx->r4 = MEM_HU(ctx->r20, 0X50);
    after_2:
    // 0x0F000AA4: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    // 0x0F000AA8: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x0F000AAC: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x0F000AB0: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x0F000AB4: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000AB8: lwc1        $f6, 0xCC8($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0XCC8);
    // 0x0F000ABC: addiu       $t8, $s2, -0x2710
    ctx->r24 = ADD32(ctx->r18, -0X2710);
    // 0x0F000AC0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x0F000AC4: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x0F000AC8: div.s       $f8, $f18, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = DIV_S(ctx->f18.fl, ctx->f4.fl);
    // 0x0F000ACC: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000AD0: addiu       $a0, $zero, 0x1000
    ctx->r4 = ADD32(0, 0X1000);
    // 0x0F000AD4: mul.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f6.fl);
    // 0x0F000AD8: mtc1        $t8, $f8
    ctx->f8.u32l = ctx->r24;
    // 0x0F000ADC: nop

    // 0x0F000AE0: cvt.s.w     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.fl = CVT_S_W(ctx->f8.u32l);
    // 0x0F000AE4: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x0F000AE8: ldc1        $f16, 0xCD0($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, 0XCD0);
    // 0x0F000AEC: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000AF0: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x0F000AF4: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x0F000AF8: mul.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f16.d);
    // 0x0F000AFC: ldc1        $f8, 0xCD8($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, 0XCD8);
    // 0x0F000B00: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x0F000B04: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x0F000B08: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x0F000B0C: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x0F000B10: mul.d       $f6, $f18, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f8.d); 
    ctx->f6.d = MUL_D(ctx->f18.d, ctx->f8.d);
    // 0x0F000B14: ldc1        $f18, 0xCE0($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0XCE0);
    // 0x0F000B18: add.d       $f16, $f10, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f6.d); 
    ctx->f16.d = ctx->f10.d + ctx->f6.d;
    // 0x0F000B1C: mul.d       $f8, $f18, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f16.d); 
    ctx->f8.d = MUL_D(ctx->f18.d, ctx->f16.d);
    // 0x0F000B20: nop

    // 0x0F000B24: mul.d       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f4.d, ctx->f8.d);
    // 0x0F000B28: jalr        $s1
    // 0x0F000B2C: cvt.s.d     $f20, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f20.fl = CVT_S_D(ctx->f10.d);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_3;
    // 0x0F000B2C: cvt.s.d     $f20, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f20.fl = CVT_S_D(ctx->f10.d);
    after_3:
    // 0x0F000B30: lhu         $t9, 0x50($s4)
    ctx->r25 = MEM_HU(ctx->r20, 0X50);
    // 0x0F000B34: lui         $s2, 0x8006
    ctx->r18 = S32(0X8006 << 16);
    // 0x0F000B38: addiu       $s2, $s2, -0x2D20
    ctx->r18 = ADD32(ctx->r18, -0X2D20);
    // 0x0F000B3C: addu        $t0, $v0, $t9
    ctx->r8 = ADD32(ctx->r2, ctx->r25);
    // 0x0F000B40: addiu       $t1, $t0, 0x800
    ctx->r9 = ADD32(ctx->r8, 0X800);
    // 0x0F000B44: sh          $t1, 0x50($s4)
    MEM_H(0X50, ctx->r20) = ctx->r9;
    // 0x0F000B48: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x0F000B4C: addiu       $s1, $s4, 0x18
    ctx->r17 = ADD32(ctx->r20, 0X18);
    // 0x0F000B50: addiu       $s3, $zero, 0x10
    ctx->r19 = ADD32(0, 0X10);
L_0F000B54:
    // 0x0F000B54: lw          $a0, 0x34($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X34);
    // 0x0F000B58: beql        $a0, $zero, L_0F000B78
    if (ctx->r4 == 0) {
        // 0x0F000B5C: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_0F000B78;
    }
    goto skip_0;
    // 0x0F000B5C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    skip_0:
    // 0x0F000B60: jalr        $s2
    // 0x0F000B64: nop

    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_4;
    // 0x0F000B64: nop

    after_4:
    // 0x0F000B68: beql        $v0, $zero, L_0F000B78
    if (ctx->r2 == 0) {
        // 0x0F000B6C: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_0F000B78;
    }
    goto skip_1;
    // 0x0F000B6C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    skip_1:
    // 0x0F000B70: swc1        $f20, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->f20.u32l;
    // 0x0F000B74: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_0F000B78:
    // 0x0F000B78: bne         $s0, $s3, L_0F000B54
    if (ctx->r16 != ctx->r19) {
        // 0x0F000B7C: addiu       $s1, $s1, -0x4
        ctx->r17 = ADD32(ctx->r17, -0X4);
            goto L_0F000B54;
    }
    // 0x0F000B7C: addiu       $s1, $s1, -0x4
    ctx->r17 = ADD32(ctx->r17, -0X4);
    // 0x0F000B80: lw          $t2, 0x58($s4)
    ctx->r10 = MEM_W(ctx->r20, 0X58);
    // 0x0F000B84: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x0F000B88: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x0F000B8C: beq         $t2, $zero, L_0F000B9C
    if (ctx->r10 == 0) {
        // 0x0F000B90: addiu       $a0, $s4, 0x8
        ctx->r4 = ADD32(ctx->r20, 0X8);
            goto L_0F000B9C;
    }
    // 0x0F000B90: addiu       $a0, $s4, 0x8
    ctx->r4 = ADD32(ctx->r20, 0X8);
    // 0x0F000B94: jalr        $t9
    // 0x0F000B98: addiu       $a1, $s4, 0xE
    ctx->r5 = ADD32(ctx->r20, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x0F000B98: addiu       $a1, $s4, 0xE
    ctx->r5 = ADD32(ctx->r20, 0XE);
    after_5:
L_0F000B9C:
    // 0x0F000B9C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x0F000BA0: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x0F000BA4: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x0F000BA8: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x0F000BAC: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x0F000BB0: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x0F000BB4: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x0F000BB8: jr          $ra
    // 0x0F000BBC: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x0F000BBC: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void ni_ovl_131_func_0F000BC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x0F000BC0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x0F000BC4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x0F000BC8: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x0F000BCC: jalr        $t9
    // 0x0F000BD0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x0F000BD0: nop

    after_0:
    // 0x0F000BD4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x0F000BD8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x0F000BDC: jr          $ra
    // 0x0F000BE0: nop

    return;
    // 0x0F000BE0: nop

;}
