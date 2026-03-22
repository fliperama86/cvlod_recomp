#include "recomp.h"
#include "funcs.h"

RECOMP_FUNC void func_80004698(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004698: mtc1        $zero, $f7
    ctx->f_odd[(7 - 1) * 2] = 0;
    // 0x8000469C: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x800046A0: cvt.d.s     $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f4.d = CVT_D_S(ctx->f12.fl);
    // 0x800046A4: c.lt.d      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.d < ctx->f6.d;
    // 0x800046A8: nop

    // 0x800046AC: bc1fl       L_800046C4
    if (!c1cs) {
        // 0x800046B0: mov.s       $f2, $f12
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
            goto L_800046C4;
    }
    goto skip_0;
    // 0x800046B0: mov.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
    skip_0:
    // 0x800046B4: neg.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = -ctx->f12.fl;
    // 0x800046B8: jr          $ra
    // 0x800046BC: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    return;
    // 0x800046BC: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    // 0x800046C0: mov.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
L_800046C4:
    // 0x800046C4: jr          $ra
    // 0x800046C8: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    return;
    // 0x800046C8: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
;}
RECOMP_FUNC void func_800046CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800046CC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800046D0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800046D4: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x800046D8: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x800046DC: lwc1        $f10, 0x4($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X4);
    // 0x800046E0: lwc1        $f8, 0x4($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800046E4: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x800046E8: lwc1        $f18, 0x8($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X8);
    // 0x800046EC: lwc1        $f16, 0x8($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X8);
    // 0x800046F0: mul.s       $f12, $f0, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x800046F4: sub.s       $f0, $f8, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x800046F8: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x800046FC: add.s       $f12, $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f12.fl + ctx->f0.fl;
    // 0x80004700: sub.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x80004704: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80004708: jal         0x800A01E0
    // 0x8000470C: add.s       $f12, $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f12.fl + ctx->f0.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x8000470C: add.s       $f12, $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f12.fl + ctx->f0.fl;
    after_0:
    // 0x80004710: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80004714: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80004718: jr          $ra
    // 0x8000471C: nop

    return;
    // 0x8000471C: nop

;}
RECOMP_FUNC void func_80004720(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004720: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80004724: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80004728: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8000472C: lwc1        $f2, 0x0($a0)
    ctx->f2.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80004730: lwc1        $f14, 0x4($a0)
    ctx->f14.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80004734: lwc1        $f0, 0x8($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80004738: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x8000473C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80004740: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80004744: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80004748: mul.s       $f10, $f0, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x8000474C: jal         0x800A01E0
    // 0x80004750: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x80004750: add.s       $f12, $f10, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f8.fl;
    after_0:
    // 0x80004754: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80004758: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8000475C: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x80004760: c.eq.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl == ctx->f16.fl;
    // 0x80004764: lwc1        $f18, 0x1C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x80004768: bc1tl       L_80004778
    if (c1cs) {
        // 0x8000476C: lwc1        $f4, 0x0($a0)
        ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
            goto L_80004778;
    }
    goto skip_0;
    // 0x8000476C: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    skip_0:
    // 0x80004770: div.s       $f2, $f18, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80004774: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
L_80004778:
    // 0x80004778: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8000477C: lwc1        $f16, 0x8($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80004780: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80004784: nop

    // 0x80004788: mul.s       $f8, $f10, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8000478C: nop

    // 0x80004790: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x80004794: swc1        $f6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f6.u32l;
    // 0x80004798: swc1        $f8, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f8.u32l;
    // 0x8000479C: swc1        $f18, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f18.u32l;
    // 0x800047A0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800047A4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800047A8: jr          $ra
    // 0x800047AC: nop

    return;
    // 0x800047AC: nop

;}
RECOMP_FUNC void func_800047B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800047B0: addiu       $t6, $zero, 0x1FC
    ctx->r14 = ADD32(0, 0X1FC);
    // 0x800047B4: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x800047B8: jr          $ra
    // 0x800047BC: sw          $t6, -0x1ED0($at)
    MEM_W(-0X1ED0, ctx->r1) = ctx->r14;
    return;
    // 0x800047BC: sw          $t6, -0x1ED0($at)
    MEM_W(-0X1ED0, ctx->r1) = ctx->r14;
;}
RECOMP_FUNC void func_800047C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800047C0: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x800047C4: addiu       $v0, $v0, -0x1ED0
    ctx->r2 = ADD32(ctx->r2, -0X1ED0);
    // 0x800047C8: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x800047CC: lui         $at, 0x41C6
    ctx->r1 = S32(0X41C6 << 16);
    // 0x800047D0: ori         $at, $at, 0x4E6D
    ctx->r1 = ctx->r1 | 0X4E6D;
    // 0x800047D4: multu       $t6, $at
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r1)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x800047D8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x800047DC: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x800047E0: mflo        $t7
    ctx->r15 = lo;
    // 0x800047E4: addiu       $t8, $t7, 0x3039
    ctx->r24 = ADD32(ctx->r15, 0X3039);
    // 0x800047E8: srl         $v1, $t8, 16
    ctx->r3 = S32(U32(ctx->r24) >> 16);
    // 0x800047EC: sw          $t8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r24;
    // 0x800047F0: beq         $a0, $zero, L_8000480C
    if (ctx->r4 == 0) {
        // 0x800047F4: andi        $v1, $v1, 0x7FFF
        ctx->r3 = ctx->r3 & 0X7FFF;
            goto L_8000480C;
    }
    // 0x800047F4: andi        $v1, $v1, 0x7FFF
    ctx->r3 = ctx->r3 & 0X7FFF;
    // 0x800047F8: divu        $zero, $v1, $a0
    lo = S32(U32(ctx->r3) / U32(ctx->r4)); hi = S32(U32(ctx->r3) % U32(ctx->r4));
    // 0x800047FC: mfhi        $v1
    ctx->r3 = hi;
    // 0x80004800: bne         $a0, $zero, L_8000480C
    if (ctx->r4 != 0) {
        // 0x80004804: nop
    
            goto L_8000480C;
    }
    // 0x80004804: nop

    // 0x80004808: break       7
    do_break(2147502088);
L_8000480C:
    // 0x8000480C: jr          $ra
    // 0x80004810: andi        $v0, $v1, 0xFFFF
    ctx->r2 = ctx->r3 & 0XFFFF;
    return;
    // 0x80004810: andi        $v0, $v1, 0xFFFF
    ctx->r2 = ctx->r3 & 0XFFFF;
;}
RECOMP_FUNC void func_80004814(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004814: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80004818: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8000481C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80004820: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80004824: jal         0x800A2B80
    // 0x80004828: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    guNormalize(rdram, ctx);
        goto after_0;
    // 0x80004828: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x8000482C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80004830: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80004834: jr          $ra
    // 0x80004838: nop

    return;
    // 0x80004838: nop

;}
RECOMP_FUNC void func_8000483C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000483C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80004840: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80004844: jal         0x800A02C0
    // 0x80004848: nop

    func_800A02C0(rdram, ctx);
        goto after_0;
    // 0x80004848: nop

    after_0:
    // 0x8000484C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80004850: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80004854: jr          $ra
    // 0x80004858: nop

    return;
    // 0x80004858: nop

;}
RECOMP_FUNC void func_8000485C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000485C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80004860: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80004864: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80004868: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x8000486C: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80004870: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x80004874: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80004878: swc1        $f0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f0.u32l;
    // 0x8000487C: swc1        $f0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f0.u32l;
    // 0x80004880: swc1        $f0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f0.u32l;
    // 0x80004884: jal         0x800A2160
    // 0x80004888: swc1        $f0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f0.u32l;
    func_800A2160(rdram, ctx);
        goto after_0;
    // 0x80004888: swc1        $f0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f0.u32l;
    after_0:
    // 0x8000488C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80004890: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80004894: jr          $ra
    // 0x80004898: nop

    return;
    // 0x80004898: nop

;}
RECOMP_FUNC void func_800048A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800048A0: sra         $t6, $a1, 5
    ctx->r14 = S32(SIGNED(ctx->r5) >> 5);
    // 0x800048A4: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x800048A8: addu        $t8, $a0, $t7
    ctx->r24 = ADD32(ctx->r4, ctx->r15);
    // Guard: if computed address is outside valid RDRAM (8MB), return 0
    {
        uint32_t addr = (uint32_t)ctx->r24;
        if (addr < 0x80000000 || addr >= 0x80800000) {
            ctx->r2 = 0;
            return;
        }
    }
    // 0x800048AC: lw          $t9, 0x0($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X0);
    // 0x800048B0: andi        $t0, $a1, 0x1F
    ctx->r8 = ctx->r5 & 0X1F;
    // 0x800048B4: lui         $t1, 0x8000
    ctx->r9 = S32(0X8000 << 16);
    // 0x800048B8: srlv        $t2, $t1, $t0
    ctx->r10 = S32(U32(ctx->r9) >> (ctx->r8 & 31));
    // 0x800048BC: jr          $ra
    // 0x800048C0: and         $v0, $t9, $t2
    ctx->r2 = ctx->r25 & ctx->r10;
    return;
    // 0x800048C0: and         $v0, $t9, $t2
    ctx->r2 = ctx->r25 & ctx->r10;
;}
RECOMP_FUNC void func_800048C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800048C4: sra         $t6, $a1, 5
    ctx->r14 = S32(SIGNED(ctx->r5) >> 5);
    // 0x800048C8: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x800048CC: addu        $v0, $a0, $t7
    ctx->r2 = ADD32(ctx->r4, ctx->r15);
    // Guard: if computed address is outside valid RDRAM, return without writing
    {
        uint32_t addr = (uint32_t)ctx->r2;
        if (addr < 0x80000000 || addr >= 0x80800000) {
            ctx->r2 = 0;
            return;
        }
    }
    // 0x800048D0: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x800048D4: andi        $t9, $a1, 0x1F
    ctx->r25 = ctx->r5 & 0X1F;
    // 0x800048D8: lui         $t0, 0x8000
    ctx->r8 = S32(0X8000 << 16);
    // 0x800048DC: srlv        $t1, $t0, $t9
    ctx->r9 = S32(U32(ctx->r8) >> (ctx->r25 & 31));
    // 0x800048E0: or          $t2, $t8, $t1
    ctx->r10 = ctx->r24 | ctx->r9;
    // 0x800048E4: jr          $ra
    // 0x800048E8: sw          $t2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r10;
    return;
    // 0x800048E8: sw          $t2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r10;
;}
RECOMP_FUNC void func_800048EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800048EC: sra         $t6, $a1, 5
    ctx->r14 = S32(SIGNED(ctx->r5) >> 5);
    // 0x800048F0: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x800048F4: addu        $v0, $a0, $t7
    ctx->r2 = ADD32(ctx->r4, ctx->r15);
    // 0x800048F8: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x800048FC: andi        $t9, $a1, 0x1F
    ctx->r25 = ctx->r5 & 0X1F;
    // 0x80004900: lui         $t0, 0x8000
    ctx->r8 = S32(0X8000 << 16);
    // 0x80004904: srlv        $t1, $t0, $t9
    ctx->r9 = S32(U32(ctx->r8) >> (ctx->r25 & 31));
    // 0x80004908: nor         $t2, $t1, $zero
    ctx->r10 = ~(ctx->r9 | 0);
    // 0x8000490C: and         $t3, $t8, $t2
    ctx->r11 = ctx->r24 & ctx->r10;
    // 0x80004910: jr          $ra
    // 0x80004914: sw          $t3, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r11;
    return;
    // 0x80004914: sw          $t3, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r11;
;}
RECOMP_FUNC void func_80004918(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004918: sra         $t6, $a1, 5
    ctx->r14 = S32(SIGNED(ctx->r5) >> 5);
    // 0x8000491C: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80004920: addu        $v0, $a0, $t7
    ctx->r2 = ADD32(ctx->r4, ctx->r15);
    // 0x80004924: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x80004928: andi        $t9, $a1, 0x1F
    ctx->r25 = ctx->r5 & 0X1F;
    // 0x8000492C: lui         $t0, 0x8000
    ctx->r8 = S32(0X8000 << 16);
    // 0x80004930: srlv        $t1, $t0, $t9
    ctx->r9 = S32(U32(ctx->r8) >> (ctx->r25 & 31));
    // 0x80004934: xor         $t2, $t8, $t1
    ctx->r10 = ctx->r24 ^ ctx->r9;
    // 0x80004938: jr          $ra
    // 0x8000493C: sw          $t2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r10;
    return;
    // 0x8000493C: sw          $t2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r10;
;}
RECOMP_FUNC void func_80004940(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004940: andi        $t6, $a1, 0x1F
    ctx->r14 = ctx->r5 & 0X1F;
    // 0x80004944: lui         $t7, 0x8000
    ctx->r15 = S32(0X8000 << 16);
    // 0x80004948: slt         $at, $a1, $a2
    ctx->r1 = SIGNED(ctx->r5) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x8000494C: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x80004950: sra         $v0, $a1, 5
    ctx->r2 = S32(SIGNED(ctx->r5) >> 5);
    // 0x80004954: beq         $at, $zero, L_800049DC
    if (ctx->r1 == 0) {
        // 0x80004958: srlv        $a3, $t7, $t6
        ctx->r7 = S32(U32(ctx->r15) >> (ctx->r14 & 31));
            goto L_800049DC;
    }
    // 0x80004958: srlv        $a3, $t7, $t6
    ctx->r7 = S32(U32(ctx->r15) >> (ctx->r14 & 31));
    // 0x8000495C: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x80004960: addu        $a1, $a0, $t8
    ctx->r5 = ADD32(ctx->r4, ctx->r24);
    // 0x80004964: lw          $t0, 0x0($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X0);
    // 0x80004968: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x8000496C: and         $t9, $t0, $a3
    ctx->r25 = ctx->r8 & ctx->r7;
L_80004970:
    // 0x80004970: beq         $t9, $zero, L_80004988
    if (ctx->r25 == 0) {
        // 0x80004974: nop
    
            goto L_80004988;
    }
    // 0x80004974: nop

L_80004978:
    // 0x80004978: srl         $a3, $a3, 1
    ctx->r7 = S32(U32(ctx->r7) >> 1);
    // 0x8000497C: and         $t1, $t0, $a3
    ctx->r9 = ctx->r8 & ctx->r7;
    // 0x80004980: bne         $t1, $zero, L_80004978
    if (ctx->r9 != 0) {
        // 0x80004984: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_80004978;
    }
    // 0x80004984: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
L_80004988:
    // 0x80004988: beq         $a3, $zero, L_80004998
    if (ctx->r7 == 0) {
        // 0x8000498C: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_80004998;
    }
    // 0x8000498C: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x80004990: jr          $ra
    // 0x80004994: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80004994: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80004998:
    // 0x80004998: lw          $t0, 0x4($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X4);
    // 0x8000499C: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x800049A0: lui         $a3, 0x8000
    ctx->r7 = S32(0X8000 << 16);
    // 0x800049A4: bnel        $v0, $t0, L_800049D4
    if (ctx->r2 != ctx->r8) {
        // 0x800049A8: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_800049D4;
    }
    goto skip_0;
    // 0x800049A8: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_0:
    // 0x800049AC: beql        $at, $zero, L_800049D4
    if (ctx->r1 == 0) {
        // 0x800049B0: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_800049D4;
    }
    goto skip_1;
    // 0x800049B0: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_1:
    // 0x800049B4: lw          $t0, 0x4($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X4);
L_800049B8:
    // 0x800049B8: addiu       $v1, $v1, 0x20
    ctx->r3 = ADD32(ctx->r3, 0X20);
    // 0x800049BC: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x800049C0: bne         $v0, $t0, L_800049D0
    if (ctx->r2 != ctx->r8) {
        // 0x800049C4: addiu       $a1, $a1, 0x4
        ctx->r5 = ADD32(ctx->r5, 0X4);
            goto L_800049D0;
    }
    // 0x800049C4: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x800049C8: bnel        $at, $zero, L_800049B8
    if (ctx->r1 != 0) {
        // 0x800049CC: lw          $t0, 0x4($a1)
        ctx->r8 = MEM_W(ctx->r5, 0X4);
            goto L_800049B8;
    }
    goto skip_2;
    // 0x800049CC: lw          $t0, 0x4($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X4);
    skip_2:
L_800049D0:
    // 0x800049D0: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
L_800049D4:
    // 0x800049D4: bnel        $at, $zero, L_80004970
    if (ctx->r1 != 0) {
        // 0x800049D8: and         $t9, $t0, $a3
        ctx->r25 = ctx->r8 & ctx->r7;
            goto L_80004970;
    }
    goto skip_3;
    // 0x800049D8: and         $t9, $t0, $a3
    ctx->r25 = ctx->r8 & ctx->r7;
    skip_3:
L_800049DC:
    // 0x800049DC: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x800049E0: jr          $ra
    // 0x800049E4: nop

    return;
    // 0x800049E4: nop

;}
RECOMP_FUNC void func_800049E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800049E8: andi        $t6, $a1, 0x1F
    ctx->r14 = ctx->r5 & 0X1F;
    // 0x800049EC: lui         $t7, 0x8000
    ctx->r15 = S32(0X8000 << 16);
    // 0x800049F0: slt         $at, $a1, $a2
    ctx->r1 = SIGNED(ctx->r5) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x800049F4: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x800049F8: sra         $v0, $a1, 5
    ctx->r2 = S32(SIGNED(ctx->r5) >> 5);
    // 0x800049FC: beq         $at, $zero, L_80004A98
    if (ctx->r1 == 0) {
        // 0x80004A00: srlv        $a3, $t7, $t6
        ctx->r7 = S32(U32(ctx->r15) >> (ctx->r14 & 31));
            goto L_80004A98;
    }
    // 0x80004A00: srlv        $a3, $t7, $t6
    ctx->r7 = S32(U32(ctx->r15) >> (ctx->r14 & 31));
L_80004A04:
    // 0x80004A04: beq         $a3, $zero, L_80004A38
    if (ctx->r7 == 0) {
        // 0x80004A08: sll         $t8, $v0, 2
        ctx->r24 = S32(ctx->r2 << 2);
            goto L_80004A38;
    }
    // 0x80004A08: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x80004A0C: addu        $t9, $a0, $t8
    ctx->r25 = ADD32(ctx->r4, ctx->r24);
    // 0x80004A10: lw          $a1, 0x0($t9)
    ctx->r5 = MEM_W(ctx->r25, 0X0);
    // 0x80004A14: and         $t0, $a1, $a3
    ctx->r8 = ctx->r5 & ctx->r7;
    // 0x80004A18: bne         $t0, $zero, L_80004A38
    if (ctx->r8 != 0) {
        // 0x80004A1C: nop
    
            goto L_80004A38;
    }
    // 0x80004A1C: nop

    // 0x80004A20: srl         $a3, $a3, 1
    ctx->r7 = S32(U32(ctx->r7) >> 1);
L_80004A24:
    // 0x80004A24: beq         $a3, $zero, L_80004A38
    if (ctx->r7 == 0) {
        // 0x80004A28: addiu       $v1, $v1, 0x1
        ctx->r3 = ADD32(ctx->r3, 0X1);
            goto L_80004A38;
    }
    // 0x80004A28: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80004A2C: and         $t1, $a1, $a3
    ctx->r9 = ctx->r5 & ctx->r7;
    // 0x80004A30: beql        $t1, $zero, L_80004A24
    if (ctx->r9 == 0) {
        // 0x80004A34: srl         $a3, $a3, 1
        ctx->r7 = S32(U32(ctx->r7) >> 1);
            goto L_80004A24;
    }
    goto skip_0;
    // 0x80004A34: srl         $a3, $a3, 1
    ctx->r7 = S32(U32(ctx->r7) >> 1);
    skip_0:
L_80004A38:
    // 0x80004A38: beq         $a3, $zero, L_80004A48
    if (ctx->r7 == 0) {
        // 0x80004A3C: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80004A48;
    }
    // 0x80004A3C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80004A40: jr          $ra
    // 0x80004A44: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80004A44: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80004A48:
    // 0x80004A48: sll         $t2, $v0, 2
    ctx->r10 = S32(ctx->r2 << 2);
    // 0x80004A4C: addu        $a1, $a0, $t2
    ctx->r5 = ADD32(ctx->r4, ctx->r10);
    // 0x80004A50: lw          $t3, 0x0($a1)
    ctx->r11 = MEM_W(ctx->r5, 0X0);
    // 0x80004A54: lui         $a3, 0x8000
    ctx->r7 = S32(0X8000 << 16);
    // 0x80004A58: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x80004A5C: bnel        $t3, $zero, L_80004A90
    if (ctx->r11 != 0) {
        // 0x80004A60: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_80004A90;
    }
    goto skip_1;
    // 0x80004A60: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_1:
    // 0x80004A64: beql        $at, $zero, L_80004A90
    if (ctx->r1 == 0) {
        // 0x80004A68: slt         $at, $v1, $a2
        ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
            goto L_80004A90;
    }
    goto skip_2;
    // 0x80004A68: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    skip_2:
    // 0x80004A6C: lw          $t4, 0x4($a1)
    ctx->r12 = MEM_W(ctx->r5, 0X4);
L_80004A70:
    // 0x80004A70: addiu       $v1, $v1, 0x20
    ctx->r3 = ADD32(ctx->r3, 0X20);
    // 0x80004A74: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80004A78: bne         $t4, $zero, L_80004A8C
    if (ctx->r12 != 0) {
        // 0x80004A7C: addiu       $a1, $a1, 0x4
        ctx->r5 = ADD32(ctx->r5, 0X4);
            goto L_80004A8C;
    }
    // 0x80004A7C: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x80004A80: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
    // 0x80004A84: bnel        $at, $zero, L_80004A70
    if (ctx->r1 != 0) {
        // 0x80004A88: lw          $t4, 0x4($a1)
        ctx->r12 = MEM_W(ctx->r5, 0X4);
            goto L_80004A70;
    }
    goto skip_3;
    // 0x80004A88: lw          $t4, 0x4($a1)
    ctx->r12 = MEM_W(ctx->r5, 0X4);
    skip_3:
L_80004A8C:
    // 0x80004A8C: slt         $at, $v1, $a2
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r6) ? 1 : 0;
L_80004A90:
    // 0x80004A90: bne         $at, $zero, L_80004A04
    if (ctx->r1 != 0) {
        // 0x80004A94: nop
    
            goto L_80004A04;
    }
    // 0x80004A94: nop

L_80004A98:
    // 0x80004A98: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80004A9C: jr          $ra
    // 0x80004AA0: nop

    return;
    // 0x80004AA0: nop

;}
RECOMP_FUNC void func_80004AB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004AB0: bgez        $a1, L_80004AC0
    if (SIGNED(ctx->r5) >= 0) {
        // 0x80004AB4: addiu       $sp, $sp, -0x10
        ctx->r29 = ADD32(ctx->r29, -0X10);
            goto L_80004AC0;
    }
    // 0x80004AB4: addiu       $sp, $sp, -0x10
    ctx->r29 = ADD32(ctx->r29, -0X10);
    // 0x80004AB8: b           L_80004AC4
    // 0x80004ABC: negu        $v0, $a1
    ctx->r2 = SUB32(0, ctx->r5);
        goto L_80004AC4;
    // 0x80004ABC: negu        $v0, $a1
    ctx->r2 = SUB32(0, ctx->r5);
L_80004AC0:
    // 0x80004AC0: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
L_80004AC4:
    // 0x80004AC4: bgez        $a0, L_80004AD4
    if (SIGNED(ctx->r4) >= 0) {
        // 0x80004AC8: lui         $a2, 0x80
        ctx->r6 = S32(0X80 << 16);
            goto L_80004AD4;
    }
    // 0x80004AC8: lui         $a2, 0x80
    ctx->r6 = S32(0X80 << 16);
    // 0x80004ACC: b           L_80004AD8
    // 0x80004AD0: negu        $v1, $a0
    ctx->r3 = SUB32(0, ctx->r4);
        goto L_80004AD8;
    // 0x80004AD0: negu        $v1, $a0
    ctx->r3 = SUB32(0, ctx->r4);
L_80004AD4:
    // 0x80004AD4: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
L_80004AD8:
    // 0x80004AD8: ori         $a2, $a2, 0x1
    ctx->r6 = ctx->r6 | 0X1;
    // 0x80004ADC: sltu        $at, $v0, $a2
    ctx->r1 = ctx->r2 < ctx->r6 ? 1 : 0;
    // 0x80004AE0: beq         $at, $zero, L_80004AF0
    if (ctx->r1 == 0) {
        // 0x80004AE4: sltu        $at, $v1, $a2
        ctx->r1 = ctx->r3 < ctx->r6 ? 1 : 0;
            goto L_80004AF0;
    }
    // 0x80004AE4: sltu        $at, $v1, $a2
    ctx->r1 = ctx->r3 < ctx->r6 ? 1 : 0;
    // 0x80004AE8: bnel        $at, $zero, L_80004B10
    if (ctx->r1 != 0) {
        // 0x80004AEC: sltu        $at, $v1, $v0
        ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
            goto L_80004B10;
    }
    goto skip_0;
    // 0x80004AEC: sltu        $at, $v1, $v0
    ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
    skip_0:
L_80004AF0:
    // 0x80004AF0: srl         $v0, $v0, 1
    ctx->r2 = S32(U32(ctx->r2) >> 1);
L_80004AF4:
    // 0x80004AF4: sltu        $at, $v0, $a2
    ctx->r1 = ctx->r2 < ctx->r6 ? 1 : 0;
    // 0x80004AF8: beq         $at, $zero, L_80004AF0
    if (ctx->r1 == 0) {
        // 0x80004AFC: srl         $v1, $v1, 1
        ctx->r3 = S32(U32(ctx->r3) >> 1);
            goto L_80004AF0;
    }
    // 0x80004AFC: srl         $v1, $v1, 1
    ctx->r3 = S32(U32(ctx->r3) >> 1);
    // 0x80004B00: sltu        $at, $v1, $a2
    ctx->r1 = ctx->r3 < ctx->r6 ? 1 : 0;
    // 0x80004B04: beql        $at, $zero, L_80004AF4
    if (ctx->r1 == 0) {
        // 0x80004B08: srl         $v0, $v0, 1
        ctx->r2 = S32(U32(ctx->r2) >> 1);
            goto L_80004AF4;
    }
    goto skip_1;
    // 0x80004B08: srl         $v0, $v0, 1
    ctx->r2 = S32(U32(ctx->r2) >> 1);
    skip_1:
    // 0x80004B0C: sltu        $at, $v1, $v0
    ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
L_80004B10:
    // 0x80004B10: beql        $at, $zero, L_80004B68
    if (ctx->r1 == 0) {
        // 0x80004B14: sltu        $at, $v0, $v1
        ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
            goto L_80004B68;
    }
    goto skip_2;
    // 0x80004B14: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    skip_2:
    // 0x80004B18: bne         $a1, $zero, L_80004B28
    if (ctx->r5 != 0) {
        // 0x80004B1C: sll         $t6, $v1, 9
        ctx->r14 = S32(ctx->r3 << 9);
            goto L_80004B28;
    }
    // 0x80004B1C: sll         $t6, $v1, 9
    ctx->r14 = S32(ctx->r3 << 9);
    // 0x80004B20: b           L_80004C1C
    // 0x80004B24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80004C1C;
    // 0x80004B24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80004B28:
    // 0x80004B28: divu        $zero, $t6, $v0
    lo = S32(U32(ctx->r14) / U32(ctx->r2)); hi = S32(U32(ctx->r14) % U32(ctx->r2));
    // 0x80004B2C: mflo        $a3
    ctx->r7 = lo;
    // 0x80004B30: sltiu       $at, $a3, 0x200
    ctx->r1 = ctx->r7 < 0X200 ? 1 : 0;
    // 0x80004B34: bne         $v0, $zero, L_80004B40
    if (ctx->r2 != 0) {
        // 0x80004B38: nop
    
            goto L_80004B40;
    }
    // 0x80004B38: nop

    // 0x80004B3C: break       7
    do_break(2147502908);
L_80004B40:
    // 0x80004B40: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x80004B44: bne         $at, $zero, L_80004B50
    if (ctx->r1 != 0) {
        // 0x80004B48: lui         $v0, 0x800B
        ctx->r2 = S32(0X800B << 16);
            goto L_80004B50;
    }
    // 0x80004B48: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x80004B4C: addiu       $a2, $zero, 0x1FF
    ctx->r6 = ADD32(0, 0X1FF);
L_80004B50:
    // 0x80004B50: sll         $t7, $a2, 1
    ctx->r15 = S32(ctx->r6 << 1);
    // 0x80004B54: addu        $v0, $v0, $t7
    ctx->r2 = ADD32(ctx->r2, ctx->r15);
    // 0x80004B58: lhu         $v0, -0x1EC0($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X1EC0);
    // 0x80004B5C: b           L_80004BD0
    // 0x80004B60: sw          $v0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r2;
        goto L_80004BD0;
    // 0x80004B60: sw          $v0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r2;
    // 0x80004B64: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
L_80004B68:
    // 0x80004B68: beq         $at, $zero, L_80004BC4
    if (ctx->r1 == 0) {
        // 0x80004B6C: nop
    
            goto L_80004BC4;
    }
    // 0x80004B6C: nop

    // 0x80004B70: bne         $a0, $zero, L_80004B80
    if (ctx->r4 != 0) {
        // 0x80004B74: sll         $t8, $v0, 9
        ctx->r24 = S32(ctx->r2 << 9);
            goto L_80004B80;
    }
    // 0x80004B74: sll         $t8, $v0, 9
    ctx->r24 = S32(ctx->r2 << 9);
    // 0x80004B78: b           L_80004C1C
    // 0x80004B7C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80004C1C;
    // 0x80004B7C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80004B80:
    // 0x80004B80: divu        $zero, $t8, $v1
    lo = S32(U32(ctx->r24) / U32(ctx->r3)); hi = S32(U32(ctx->r24) % U32(ctx->r3));
    // 0x80004B84: mflo        $a3
    ctx->r7 = lo;
    // 0x80004B88: sltiu       $at, $a3, 0x200
    ctx->r1 = ctx->r7 < 0X200 ? 1 : 0;
    // 0x80004B8C: bne         $v1, $zero, L_80004B98
    if (ctx->r3 != 0) {
        // 0x80004B90: nop
    
            goto L_80004B98;
    }
    // 0x80004B90: nop

    // 0x80004B94: break       7
    do_break(2147502996);
L_80004B98:
    // 0x80004B98: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x80004B9C: bne         $at, $zero, L_80004BA8
    if (ctx->r1 != 0) {
        // 0x80004BA0: lui         $t0, 0x800B
        ctx->r8 = S32(0X800B << 16);
            goto L_80004BA8;
    }
    // 0x80004BA0: lui         $t0, 0x800B
    ctx->r8 = S32(0X800B << 16);
    // 0x80004BA4: addiu       $a2, $zero, 0x1FF
    ctx->r6 = ADD32(0, 0X1FF);
L_80004BA8:
    // 0x80004BA8: sll         $t9, $a2, 1
    ctx->r25 = S32(ctx->r6 << 1);
    // 0x80004BAC: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80004BB0: lhu         $t0, -0x1EC0($t0)
    ctx->r8 = MEM_HU(ctx->r8, -0X1EC0);
    // 0x80004BB4: addiu       $t1, $zero, 0x4000
    ctx->r9 = ADD32(0, 0X4000);
    // 0x80004BB8: subu        $v0, $t1, $t0
    ctx->r2 = SUB32(ctx->r9, ctx->r8);
    // 0x80004BBC: b           L_80004BD0
    // 0x80004BC0: sw          $v0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r2;
        goto L_80004BD0;
    // 0x80004BC0: sw          $v0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r2;
L_80004BC4:
    // 0x80004BC4: bne         $v0, $v1, L_80004BD0
    if (ctx->r2 != ctx->r3) {
        // 0x80004BC8: addiu       $v0, $zero, 0x2000
        ctx->r2 = ADD32(0, 0X2000);
            goto L_80004BD0;
    }
    // 0x80004BC8: addiu       $v0, $zero, 0x2000
    ctx->r2 = ADD32(0, 0X2000);
    // 0x80004BCC: sw          $v0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r2;
L_80004BD0:
    // 0x80004BD0: bgez        $a1, L_80004BE8
    if (SIGNED(ctx->r5) >= 0) {
        // 0x80004BD4: lw          $v0, 0x0($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X0);
            goto L_80004BE8;
    }
    // 0x80004BD4: lw          $v0, 0x0($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X0);
    // 0x80004BD8: bltz        $a0, L_80004BE8
    if (SIGNED(ctx->r4) < 0) {
        // 0x80004BDC: ori         $t2, $zero, 0x8000
        ctx->r10 = 0 | 0X8000;
            goto L_80004BE8;
    }
    // 0x80004BDC: ori         $t2, $zero, 0x8000
    ctx->r10 = 0 | 0X8000;
    // 0x80004BE0: b           L_80004C18
    // 0x80004BE4: subu        $v1, $t2, $v0
    ctx->r3 = SUB32(ctx->r10, ctx->r2);
        goto L_80004C18;
    // 0x80004BE4: subu        $v1, $t2, $v0
    ctx->r3 = SUB32(ctx->r10, ctx->r2);
L_80004BE8:
    // 0x80004BE8: bgez        $a1, L_80004C00
    if (SIGNED(ctx->r5) >= 0) {
        // 0x80004BEC: nop
    
            goto L_80004C00;
    }
    // 0x80004BEC: nop

    // 0x80004BF0: bgez        $a0, L_80004C00
    if (SIGNED(ctx->r4) >= 0) {
        // 0x80004BF4: ori         $at, $zero, 0x8000
        ctx->r1 = 0 | 0X8000;
            goto L_80004C00;
    }
    // 0x80004BF4: ori         $at, $zero, 0x8000
    ctx->r1 = 0 | 0X8000;
    // 0x80004BF8: b           L_80004C18
    // 0x80004BFC: addu        $v1, $v0, $at
    ctx->r3 = ADD32(ctx->r2, ctx->r1);
        goto L_80004C18;
    // 0x80004BFC: addu        $v1, $v0, $at
    ctx->r3 = ADD32(ctx->r2, ctx->r1);
L_80004C00:
    // 0x80004C00: bltz        $a1, L_80004C18
    if (SIGNED(ctx->r5) < 0) {
        // 0x80004C04: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80004C18;
    }
    // 0x80004C04: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80004C08: bgez        $a0, L_80004C18
    if (SIGNED(ctx->r4) >= 0) {
        // 0x80004C0C: lui         $t3, 0x1
        ctx->r11 = S32(0X1 << 16);
            goto L_80004C18;
    }
    // 0x80004C0C: lui         $t3, 0x1
    ctx->r11 = S32(0X1 << 16);
    // 0x80004C10: b           L_80004C18
    // 0x80004C14: subu        $v1, $t3, $v0
    ctx->r3 = SUB32(ctx->r11, ctx->r2);
        goto L_80004C18;
    // 0x80004C14: subu        $v1, $t3, $v0
    ctx->r3 = SUB32(ctx->r11, ctx->r2);
L_80004C18:
    // 0x80004C18: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80004C1C:
    // 0x80004C1C: jr          $ra
    // 0x80004C20: addiu       $sp, $sp, 0x10
    ctx->r29 = ADD32(ctx->r29, 0X10);
    return;
    // 0x80004C20: addiu       $sp, $sp, 0x10
    ctx->r29 = ADD32(ctx->r29, 0X10);
;}
RECOMP_FUNC void func_80004C24(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004C24: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80004C28: lwc1        $f0, -0x3280($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X3280);
    // 0x80004C2C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80004C30: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80004C34: mul.s       $f4, $f12, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x80004C38: nop

    // 0x80004C3C: mul.s       $f8, $f14, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x80004C40: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x80004C44: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x80004C48: mfc1        $a0, $f6
    ctx->r4 = (int32_t)ctx->f6.u32l;
    // 0x80004C4C: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
    // 0x80004C50: jal         0x80004AB0
    // 0x80004C54: nop

    func_80004AB0(rdram, ctx);
        goto after_0;
    // 0x80004C54: nop

    after_0:
    // 0x80004C58: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80004C5C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80004C60: jr          $ra
    // 0x80004C64: nop

    return;
    // 0x80004C64: nop

;}
RECOMP_FUNC void func_80004C68(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004C68: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80004C6C: lwc1        $f0, -0x327C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X327C);
    // 0x80004C70: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80004C74: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80004C78: mul.s       $f4, $f12, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x80004C7C: nop

    // 0x80004C80: mul.s       $f8, $f14, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x80004C84: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x80004C88: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x80004C8C: mfc1        $a0, $f6
    ctx->r4 = (int32_t)ctx->f6.u32l;
    // 0x80004C90: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
    // 0x80004C94: jal         0x80004AB0
    // 0x80004C98: nop

    func_80004AB0(rdram, ctx);
        goto after_0;
    // 0x80004C98: nop

    after_0:
    // 0x80004C9C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80004CA0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80004CA4: jr          $ra
    // 0x80004CA8: nop

    return;
    // 0x80004CA8: nop

;}
RECOMP_FUNC void func_80004CB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004CB0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80004CB4: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x80004CB8: lw          $t6, 0x28($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X28);
    // 0x80004CBC: addiu       $t0, $a1, 0x1F
    ctx->r8 = ADD32(ctx->r5, 0X1F);
    // 0x80004CC0: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x80004CC4: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80004CC8: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80004CCC: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x80004CD0: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80004CD4: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x80004CD8: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x80004CDC: sll         $t6, $a3, 6
    ctx->r14 = S32(ctx->r7 << 6);
    // 0x80004CE0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80004CE4: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    // 0x80004CE8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80004CEC: bgez        $t0, L_80004CF8
    if (SIGNED(ctx->r8) >= 0) {
        // 0x80004CF0: addu        $at, $t0, $zero
        ctx->r1 = ADD32(ctx->r8, 0);
            goto L_80004CF8;
    }
    // 0x80004CF0: addu        $at, $t0, $zero
    ctx->r1 = ADD32(ctx->r8, 0);
    // 0x80004CF4: addiu       $at, $t0, 0x1F
    ctx->r1 = ADD32(ctx->r8, 0X1F);
L_80004CF8:
    // 0x80004CF8: sra         $t0, $at, 5
    ctx->r8 = S32(SIGNED(ctx->r1) >> 5);
    // 0x80004CFC: sll         $t8, $t0, 2
    ctx->r24 = S32(ctx->r8 << 2);
    // 0x80004D00: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x80004D04: lw          $t8, 0x30($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X30);
    // 0x80004D08: addu        $t7, $t9, $t6
    ctx->r15 = ADD32(ctx->r25, ctx->r14);
    // 0x80004D0C: sw          $t0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r8;
    // 0x80004D10: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80004D14: subu        $t9, $t9, $t8
    ctx->r25 = SUB32(ctx->r25, ctx->r24);
    // 0x80004D18: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x80004D1C: addu        $a1, $t7, $t9
    ctx->r5 = ADD32(ctx->r15, ctx->r25);
    // 0x80004D20: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80004D24: addiu       $t9, $t9, 0x1514
    ctx->r25 = ADD32(ctx->r25, 0X1514);
    // 0x80004D28: jalr        $t9
    // 0x80004D2C: addiu       $a1, $a1, 0x48
    ctx->r5 = ADD32(ctx->r5, 0X48);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80004D2C: addiu       $a1, $a1, 0x48
    ctx->r5 = ADD32(ctx->r5, 0X48);
    after_0:
    // 0x80004D30: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80004D34: addiu       $v1, $v1, 0x1550
    ctx->r3 = ADD32(ctx->r3, 0X1550);
    // 0x80004D38: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x80004D3C: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80004D40: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
    // 0x80004D44: bne         $v0, $zero, L_80004D54
    if (ctx->r2 != 0) {
        // 0x80004D48: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80004D54;
    }
    // 0x80004D48: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80004D4C: b           L_80004E48
    // 0x80004D50: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80004E48;
    // 0x80004D50: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80004D54:
    // 0x80004D54: lw          $t6, 0x28($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X28);
    // 0x80004D58: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80004D5C: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x80004D60: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80004D64: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80004D68: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x80004D6C: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80004D70: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x80004D74: lui         $t1, 0x800C
    ctx->r9 = S32(0X800C << 16);
    // 0x80004D78: addu        $t9, $t7, $a0
    ctx->r25 = ADD32(ctx->r15, ctx->r4);
    // 0x80004D7C: addiu       $t1, $t1, 0x1560
    ctx->r9 = ADD32(ctx->r9, 0X1560);
    // 0x80004D80: addiu       $a2, $a2, 0x1554
    ctx->r6 = ADD32(ctx->r6, 0X1554);
    // 0x80004D84: addiu       $a1, $a1, 0x1558
    ctx->r5 = ADD32(ctx->r5, 0X1558);
    // 0x80004D88: sw          $t9, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r25;
    // 0x80004D8C: sw          $t9, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r25;
    // 0x80004D90: sw          $t6, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r14;
    // 0x80004D94: sw          $zero, 0x0($t9)
    MEM_W(0X0, ctx->r25) = 0;
    // 0x80004D98: lw          $t6, 0x0($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X0);
    // 0x80004D9C: sll         $t7, $t0, 2
    ctx->r15 = S32(ctx->r8 << 2);
    // 0x80004DA0: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80004DA4: addiu       $t8, $t6, 0x4
    ctx->r24 = ADD32(ctx->r14, 0X4);
    // 0x80004DA8: lui         $t2, 0x800C
    ctx->r10 = S32(0X800C << 16);
    // 0x80004DAC: addu        $t6, $t7, $t8
    ctx->r14 = ADD32(ctx->r15, ctx->r24);
    // 0x80004DB0: addiu       $t2, $t2, 0x1564
    ctx->r10 = ADD32(ctx->r10, 0X1564);
    // 0x80004DB4: addiu       $v1, $v1, 0x1568
    ctx->r3 = ADD32(ctx->r3, 0X1568);
    // 0x80004DB8: addiu       $t7, $t6, 0x3F
    ctx->r15 = ADD32(ctx->r14, 0X3F);
    // 0x80004DBC: addiu       $at, $zero, -0x40
    ctx->r1 = ADD32(0, -0X40);
    // 0x80004DC0: sw          $t8, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->r24;
    // 0x80004DC4: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x80004DC8: and         $t9, $t7, $at
    ctx->r25 = ctx->r15 & ctx->r1;
    // 0x80004DCC: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x80004DD0: sll         $t8, $a3, 6
    ctx->r24 = S32(ctx->r7 << 6);
    // 0x80004DD4: lui         $t3, 0x800C
    ctx->r11 = S32(0X800C << 16);
    // 0x80004DD8: lui         $t4, 0x800C
    ctx->r12 = S32(0X800C << 16);
    // 0x80004DDC: lui         $t5, 0x800C
    ctx->r13 = S32(0X800C << 16);
    // 0x80004DE0: addu        $t9, $t8, $t9
    ctx->r25 = ADD32(ctx->r24, ctx->r25);
    // 0x80004DE4: addiu       $t5, $t5, 0x1574
    ctx->r13 = ADD32(ctx->r13, 0X1574);
    // 0x80004DE8: addiu       $t4, $t4, 0x156C
    ctx->r12 = ADD32(ctx->r12, 0X156C);
    // 0x80004DEC: addiu       $t3, $t3, 0x1570
    ctx->r11 = ADD32(ctx->r11, 0X1570);
    // 0x80004DF0: sw          $a3, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r7;
    // 0x80004DF4: sw          $t9, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r25;
    // 0x80004DF8: sw          $t9, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->r25;
    // 0x80004DFC: sw          $zero, 0x0($t9)
    MEM_W(0X0, ctx->r25) = 0;
    // 0x80004E00: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x80004E04: lw          $t7, 0x30($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X30);
    // 0x80004E08: lui         $ra, 0x800C
    ctx->r31 = S32(0X800C << 16);
    // 0x80004E0C: addiu       $ra, $ra, 0x1578
    ctx->r31 = ADD32(ctx->r31, 0X1578);
    // 0x80004E10: addiu       $t8, $t6, 0x4
    ctx->r24 = ADD32(ctx->r14, 0X4);
    // 0x80004E14: sw          $t8, 0x0($ra)
    MEM_W(0X0, ctx->r31) = ctx->r24;
    // 0x80004E18: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80004E1C: sw          $t7, 0x1580($at)
    MEM_W(0X1580, ctx->r1) = ctx->r15;
    // 0x80004E20: lw          $t6, 0x0($ra)
    ctx->r14 = MEM_W(ctx->r31, 0X0);
    // 0x80004E24: sll         $t9, $t7, 2
    ctx->r25 = S32(ctx->r15 << 2);
    // 0x80004E28: subu        $t9, $t9, $t7
    ctx->r25 = SUB32(ctx->r25, ctx->r15);
    // 0x80004E2C: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x80004E30: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80004E34: addu        $t8, $t9, $t6
    ctx->r24 = ADD32(ctx->r25, ctx->r14);
    // 0x80004E38: jal         0x800053BC
    // 0x80004E3C: sw          $t8, 0x157C($at)
    MEM_W(0X157C, ctx->r1) = ctx->r24;
    func_800053BC(rdram, ctx);
        goto after_1;
    // 0x80004E3C: sw          $t8, 0x157C($at)
    MEM_W(0X157C, ctx->r1) = ctx->r24;
    after_1:
    // 0x80004E40: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80004E44: lw          $v0, 0x1558($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1558);
L_80004E48:
    // 0x80004E48: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80004E4C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80004E50: jr          $ra
    // 0x80004E54: nop

    return;
    // 0x80004E54: nop

;}
RECOMP_FUNC void func_80004E58(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004E58: lui         $t6, 0x800C
    ctx->r14 = S32(0X800C << 16);
    // 0x80004E5C: lw          $t6, 0x1550($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X1550);
    // 0x80004E60: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x80004E64: sltu        $v0, $a0, $t6
    ctx->r2 = ctx->r4 < ctx->r14 ? 1 : 0;
    // 0x80004E68: xori        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 ^ 0X1;
    // 0x80004E6C: beq         $v0, $zero, L_80004E7C
    if (ctx->r2 == 0) {
        // 0x80004E70: nop
    
            goto L_80004E7C;
    }
    // 0x80004E70: nop

    // 0x80004E74: lw          $t7, 0x1554($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X1554);
    // 0x80004E78: sltu        $v0, $a0, $t7
    ctx->r2 = ctx->r4 < ctx->r15 ? 1 : 0;
L_80004E7C:
    // 0x80004E7C: jr          $ra
    // 0x80004E80: nop

    return;
    // 0x80004E80: nop

;}
RECOMP_FUNC void func_80004E84(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004E84: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80004E88: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80004E8C: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80004E90: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80004E94: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80004E98: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80004E9C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80004EA0: or          $s2, $a2, $zero
    ctx->r18 = ctx->r6 | 0;
    // 0x80004EA4: or          $s3, $a3, $zero
    ctx->r19 = ctx->r7 | 0;
    // 0x80004EA8: or          $s4, $a1, $zero
    ctx->r20 = ctx->r5 | 0;
    // 0x80004EAC: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80004EB0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
L_80004EB4:
    // 0x80004EB4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80004EB8: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80004EBC: jalr        $s4
    // 0x80004EC0: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_0;
    // 0x80004EC0: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    after_0:
    // 0x80004EC4: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
L_80004EC8:
    // 0x80004EC8: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80004ECC: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x80004ED0: beq         $v0, $zero, L_80004EE8
    if (ctx->r2 == 0) {
        // 0x80004ED4: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80004EE8;
    }
    // 0x80004ED4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80004ED8: jalr        $s4
    // 0x80004EDC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_1;
    // 0x80004EDC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_1:
    // 0x80004EE0: b           L_80004EC8
    // 0x80004EE4: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
        goto L_80004EC8;
    // 0x80004EE4: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
L_80004EE8:
    // 0x80004EE8: beql        $s0, $s1, L_80004F28
    if (ctx->r16 == ctx->r17) {
        // 0x80004EEC: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80004F28;
    }
    goto skip_0;
    // 0x80004EEC: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_0:
    // 0x80004EF0: lw          $v0, 0x10($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X10);
    // 0x80004EF4: bne         $v0, $zero, L_80004F1C
    if (ctx->r2 != 0) {
        // 0x80004EF8: nop
    
            goto L_80004F1C;
    }
    // 0x80004EF8: nop

    // 0x80004EFC: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
L_80004F00:
    // 0x80004F00: beql        $v0, $zero, L_80004F28
    if (ctx->r2 == 0) {
        // 0x80004F04: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80004F28;
    }
    goto skip_1;
    // 0x80004F04: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_1:
    // 0x80004F08: beq         $v0, $s1, L_80004F24
    if (ctx->r2 == ctx->r17) {
        // 0x80004F0C: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_80004F24;
    }
    // 0x80004F0C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x80004F10: lw          $v0, 0x10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X10);
    // 0x80004F14: beql        $v0, $zero, L_80004F00
    if (ctx->r2 == 0) {
        // 0x80004F18: lw          $v0, 0x8($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X8);
            goto L_80004F00;
    }
    goto skip_2;
    // 0x80004F18: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
    skip_2:
L_80004F1C:
    // 0x80004F1C: b           L_80004EB4
    // 0x80004F20: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
        goto L_80004EB4;
    // 0x80004F20: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80004F24:
    // 0x80004F24: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80004F28:
    // 0x80004F28: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80004F2C: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80004F30: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80004F34: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80004F38: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80004F3C: jr          $ra
    // 0x80004F40: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80004F40: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80004F44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004F44: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80004F48: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80004F4C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80004F50: lui         $s3, 0x801D
    ctx->r19 = S32(0X801D << 16);
    // 0x80004F54: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80004F58: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80004F5C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80004F60: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80004F64: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80004F68: beq         $a0, $zero, L_8000514C
    if (ctx->r4 == 0) {
        // 0x80004F6C: lh          $s3, -0x7D1C($s3)
        ctx->r19 = MEM_H(ctx->r19, -0X7D1C);
            goto L_8000514C;
    }
    // 0x80004F6C: lh          $s3, -0x7D1C($s3)
    ctx->r19 = MEM_H(ctx->r19, -0X7D1C);
    // 0x80004F70: lui         $s2, 0x800C
    ctx->r18 = S32(0X800C << 16);
    // 0x80004F74: addiu       $s2, $s2, 0x1540
    ctx->r18 = ADD32(ctx->r18, 0X1540);
    // 0x80004F78: lui         $s4, 0x8000
    ctx->r20 = S32(0X8000 << 16);
    // 0x80004F7C: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
L_80004F80:
    // 0x80004F80: beq         $a1, $zero, L_80004F90
    if (ctx->r5 == 0) {
        // 0x80004F84: nop
    
            goto L_80004F90;
    }
    // 0x80004F84: nop

    // 0x80004F88: jal         0x80004F44
    // 0x80004F8C: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80004F44(rdram, ctx);
        goto after_0;
    // 0x80004F8C: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_0:
L_80004F90:
    // 0x80004F90: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80004F94: lhu         $v0, -0x7C18($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X7C18);
    // 0x80004F98: beql        $v0, $zero, L_80004FB4
    if (ctx->r2 == 0) {
        // 0x80004F9C: lw          $a0, 0x8($s1)
        ctx->r4 = MEM_W(ctx->r17, 0X8);
            goto L_80004FB4;
    }
    goto skip_0;
    // 0x80004F9C: lw          $a0, 0x8($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X8);
    skip_0:
    // 0x80004FA0: lhu         $t6, 0x2($s1)
    ctx->r14 = MEM_HU(ctx->r17, 0X2);
    // 0x80004FA4: and         $t7, $v0, $t6
    ctx->r15 = ctx->r2 & ctx->r14;
    // 0x80004FA8: beql        $t7, $zero, L_80005144
    if (ctx->r15 == 0) {
        // 0x80004FAC: lw          $s1, 0x10($s1)
        ctx->r17 = MEM_W(ctx->r17, 0X10);
            goto L_80005144;
    }
    goto skip_1;
    // 0x80004FAC: lw          $s1, 0x10($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X10);
    skip_1:
    // 0x80004FB0: lw          $a0, 0x8($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X8);
L_80004FB4:
    // 0x80004FB4: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
    // 0x80004FB8: beq         $a0, $zero, L_8000502C
    if (ctx->r4 == 0) {
        // 0x80004FBC: nop
    
            goto L_8000502C;
    }
    // 0x80004FBC: nop

    // 0x80004FC0: lw          $a2, 0x14($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X14);
    // 0x80004FC4: bnel        $s1, $a2, L_80004FF0
    if (ctx->r17 != ctx->r6) {
        // 0x80004FC8: lw          $v1, 0x10($a2)
        ctx->r3 = MEM_W(ctx->r6, 0X10);
            goto L_80004FF0;
    }
    goto skip_2;
    // 0x80004FC8: lw          $v1, 0x10($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X10);
    skip_2:
    // 0x80004FCC: beql        $a1, $zero, L_80004FE0
    if (ctx->r5 == 0) {
        // 0x80004FD0: lw          $t8, 0x10($s1)
        ctx->r24 = MEM_W(ctx->r17, 0X10);
            goto L_80004FE0;
    }
    goto skip_3;
    // 0x80004FD0: lw          $t8, 0x10($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X10);
    skip_3:
    // 0x80004FD4: b           L_80004FE4
    // 0x80004FD8: sw          $a1, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->r5;
        goto L_80004FE4;
    // 0x80004FD8: sw          $a1, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->r5;
    // 0x80004FDC: lw          $t8, 0x10($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X10);
L_80004FE0:
    // 0x80004FE0: sw          $t8, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->r24;
L_80004FE4:
    // 0x80004FE4: b           L_8000502C
    // 0x80004FE8: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
        goto L_8000502C;
    // 0x80004FE8: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
    // 0x80004FEC: lw          $v1, 0x10($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X10);
L_80004FF0:
    // 0x80004FF0: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80004FF4: beq         $s1, $v1, L_8000500C
    if (ctx->r17 == ctx->r3) {
        // 0x80004FF8: nop
    
            goto L_8000500C;
    }
    // 0x80004FF8: nop

    // 0x80004FFC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80005000:
    // 0x80005000: lw          $v1, 0x10($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X10);
    // 0x80005004: bnel        $s1, $v1, L_80005000
    if (ctx->r17 != ctx->r3) {
        // 0x80005008: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80005000;
    }
    goto skip_4;
    // 0x80005008: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_4:
L_8000500C:
    // 0x8000500C: beql        $a1, $zero, L_80005024
    if (ctx->r5 == 0) {
        // 0x80005010: lw          $t9, 0x10($s1)
        ctx->r25 = MEM_W(ctx->r17, 0X10);
            goto L_80005024;
    }
    goto skip_5;
    // 0x80005010: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    skip_5:
    // 0x80005014: sw          $a1, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r5;
    // 0x80005018: b           L_8000502C
    // 0x8000501C: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
        goto L_8000502C;
    // 0x8000501C: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
    // 0x80005020: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
L_80005024:
    // 0x80005024: sw          $t9, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r25;
    // 0x80005028: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
L_8000502C:
    // 0x8000502C: beql        $a1, $zero, L_80005074
    if (ctx->r5 == 0) {
        // 0x80005030: lh          $t4, 0x0($s1)
        ctx->r12 = MEM_H(ctx->r17, 0X0);
            goto L_80005074;
    }
    goto skip_6;
    // 0x80005030: lh          $t4, 0x0($s1)
    ctx->r12 = MEM_H(ctx->r17, 0X0);
    skip_6:
    // 0x80005034: lw          $t0, 0x10($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X10);
    // 0x80005038: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x8000503C: beql        $t0, $zero, L_8000506C
    if (ctx->r8 == 0) {
        // 0x80005040: lw          $t3, 0x10($s1)
        ctx->r11 = MEM_W(ctx->r17, 0X10);
            goto L_8000506C;
    }
    goto skip_7;
    // 0x80005040: lw          $t3, 0x10($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X10);
    skip_7:
    // 0x80005044: lw          $t1, 0xC($v0)
    ctx->r9 = MEM_W(ctx->r2, 0XC);
L_80005048:
    // 0x80005048: sw          $a0, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r4;
    // 0x8000504C: beql        $t1, $zero, L_8000505C
    if (ctx->r9 == 0) {
        // 0x80005050: lw          $v0, 0x10($v0)
        ctx->r2 = MEM_W(ctx->r2, 0X10);
            goto L_8000505C;
    }
    goto skip_8;
    // 0x80005050: lw          $v0, 0x10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X10);
    skip_8:
    // 0x80005054: sw          $a0, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r4;
    // 0x80005058: lw          $v0, 0x10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X10);
L_8000505C:
    // 0x8000505C: lw          $t2, 0x10($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X10);
    // 0x80005060: bnel        $t2, $zero, L_80005048
    if (ctx->r10 != 0) {
        // 0x80005064: lw          $t1, 0xC($v0)
        ctx->r9 = MEM_W(ctx->r2, 0XC);
            goto L_80005048;
    }
    goto skip_9;
    // 0x80005064: lw          $t1, 0xC($v0)
    ctx->r9 = MEM_W(ctx->r2, 0XC);
    skip_9:
    // 0x80005068: lw          $t3, 0x10($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X10);
L_8000506C:
    // 0x8000506C: sw          $t3, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r11;
    // 0x80005070: lh          $t4, 0x0($s1)
    ctx->r12 = MEM_H(ctx->r17, 0X0);
L_80005074:
    // 0x80005074: andi        $t5, $t4, 0xC0
    ctx->r13 = ctx->r12 & 0XC0;
    // 0x80005078: bnel        $t5, $zero, L_800050D8
    if (ctx->r13 != 0) {
        // 0x8000507C: lw          $s0, 0x44($s1)
        ctx->r16 = MEM_W(ctx->r17, 0X44);
            goto L_800050D8;
    }
    goto skip_10;
    // 0x8000507C: lw          $s0, 0x44($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X44);
    skip_10:
    // 0x80005080: lw          $a1, 0x38($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X38);
    // 0x80005084: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x80005088: and         $t6, $a1, $s4
    ctx->r14 = ctx->r5 & ctx->r20;
    // 0x8000508C: beql        $t6, $zero, L_800050A0
    if (ctx->r14 == 0) {
        // 0x80005090: lw          $a1, 0x3C($s1)
        ctx->r5 = MEM_W(ctx->r17, 0X3C);
            goto L_800050A0;
    }
    goto skip_11;
    // 0x80005090: lw          $a1, 0x3C($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X3C);
    skip_11:
    // 0x80005094: jal         0x80002D5C
    // 0x80005098: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    GameStateMgr_enqueue(rdram, ctx);
        goto after_1;
    // 0x80005098: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    after_1:
    // 0x8000509C: lw          $a1, 0x3C($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X3C);
L_800050A0:
    // 0x800050A0: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x800050A4: and         $t7, $a1, $s4
    ctx->r15 = ctx->r5 & ctx->r20;
    // 0x800050A8: beql        $t7, $zero, L_800050BC
    if (ctx->r15 == 0) {
        // 0x800050AC: lw          $a1, 0x34($s1)
        ctx->r5 = MEM_W(ctx->r17, 0X34);
            goto L_800050BC;
    }
    goto skip_12;
    // 0x800050AC: lw          $a1, 0x34($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X34);
    skip_12:
    // 0x800050B0: jal         0x80002D5C
    // 0x800050B4: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    GameStateMgr_enqueue(rdram, ctx);
        goto after_2;
    // 0x800050B4: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    after_2:
    // 0x800050B8: lw          $a1, 0x34($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X34);
L_800050BC:
    // 0x800050BC: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x800050C0: and         $t8, $a1, $s4
    ctx->r24 = ctx->r5 & ctx->r20;
    // 0x800050C4: beql        $t8, $zero, L_800050D8
    if (ctx->r24 == 0) {
        // 0x800050C8: lw          $s0, 0x44($s1)
        ctx->r16 = MEM_W(ctx->r17, 0X44);
            goto L_800050D8;
    }
    goto skip_13;
    // 0x800050C8: lw          $s0, 0x44($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X44);
    skip_13:
    // 0x800050CC: jal         0x80002D5C
    // 0x800050D0: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    GameStateMgr_enqueue(rdram, ctx);
        goto after_3;
    // 0x800050D0: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    after_3:
    // 0x800050D4: lw          $s0, 0x44($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X44);
L_800050D8:
    // 0x800050D8: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x800050DC: beq         $s0, $zero, L_80005124
    if (ctx->r16 == 0) {
        // 0x800050E0: or          $a1, $s0, $zero
        ctx->r5 = ctx->r16 | 0;
            goto L_80005124;
    }
    // 0x800050E0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x800050E4: jal         0x80002D5C
    // 0x800050E8: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    GameStateMgr_enqueue(rdram, ctx);
        goto after_4;
    // 0x800050E8: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    after_4:
    // 0x800050EC: lw          $s0, 0x44($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X44);
    // 0x800050F0: lw          $a1, 0x48($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X48);
    // 0x800050F4: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x800050F8: beql        $s0, $a1, L_80005110
    if (ctx->r16 == ctx->r5) {
        // 0x800050FC: lw          $a1, 0x4C($s1)
        ctx->r5 = MEM_W(ctx->r17, 0X4C);
            goto L_80005110;
    }
    goto skip_14;
    // 0x800050FC: lw          $a1, 0x4C($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X4C);
    skip_14:
    // 0x80005100: jal         0x80002D5C
    // 0x80005104: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    GameStateMgr_enqueue(rdram, ctx);
        goto after_5;
    // 0x80005104: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    after_5:
    // 0x80005108: lw          $s0, 0x44($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X44);
    // 0x8000510C: lw          $a1, 0x4C($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X4C);
L_80005110:
    // 0x80005110: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x80005114: beql        $s0, $a1, L_80005128
    if (ctx->r16 == ctx->r5) {
        // 0x80005118: lhu         $t9, 0x2($s1)
        ctx->r25 = MEM_HU(ctx->r17, 0X2);
            goto L_80005128;
    }
    goto skip_15;
    // 0x80005118: lhu         $t9, 0x2($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0X2);
    skip_15:
    // 0x8000511C: jal         0x80002D5C
    // 0x80005120: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    GameStateMgr_enqueue(rdram, ctx);
        goto after_6;
    // 0x80005120: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    after_6:
L_80005124:
    // 0x80005124: lhu         $t9, 0x2($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0X2);
L_80005128:
    // 0x80005128: andi        $t0, $t9, 0x10
    ctx->r8 = ctx->r25 & 0X10;
    // 0x8000512C: beql        $t0, $zero, L_80005140
    if (ctx->r8 == 0) {
        // 0x80005130: sw          $zero, 0x0($s1)
        MEM_W(0X0, ctx->r17) = 0;
            goto L_80005140;
    }
    goto skip_16;
    // 0x80005130: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
    skip_16:
    // 0x80005134: lw          $t1, 0x74($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X74);
    // 0x80005138: sw          $zero, 0x0($t1)
    MEM_W(0X0, ctx->r9) = 0;
    // 0x8000513C: sw          $zero, 0x0($s1)
    MEM_W(0X0, ctx->r17) = 0;
L_80005140:
    // 0x80005140: lw          $s1, 0x10($s1)
    ctx->r17 = MEM_W(ctx->r17, 0X10);
L_80005144:
    // 0x80005144: bnel        $s1, $zero, L_80004F80
    if (ctx->r17 != 0) {
        // 0x80005148: lw          $a1, 0x14($s1)
        ctx->r5 = MEM_W(ctx->r17, 0X14);
            goto L_80004F80;
    }
    goto skip_17;
    // 0x80005148: lw          $a1, 0x14($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X14);
    skip_17:
L_8000514C:
    // 0x8000514C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80005150: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80005154: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80005158: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8000515C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80005160: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80005164: jr          $ra
    // 0x80005168: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80005168: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void object_dispatchChild(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000516C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80005170: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80005174: lh          $t6, 0x24($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X24);
    // 0x80005178: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8000517C: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x80005180: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x80005184: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80005188: sh          $a1, 0x128($v0)
    MEM_H(0X128, ctx->r2) = ctx->r5;
    // 0x8000518C: sw          $t6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r14;
    // 0x80005190: lw          $a2, 0x14($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X14);
    // 0x80005194: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80005198: beq         $a2, $zero, L_800051B4
    if (ctx->r6 == 0) {
        // 0x8000519C: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_800051B4;
    }
    // 0x8000519C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x800051A0: sh          $a1, 0x36($sp)
    MEM_H(0X36, ctx->r29) = ctx->r5;
    // 0x800051A4: jal         0x80004F44
    // 0x800051A8: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    func_80004F44(rdram, ctx);
        goto after_0;
    // 0x800051A8: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    after_0:
    // 0x800051AC: lhu         $a1, 0x36($sp)
    ctx->r5 = MEM_HU(ctx->r29, 0X36);
    // 0x800051B0: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
L_800051B4:
    // 0x800051B4: beql        $a1, $zero, L_800051D0
    if (ctx->r5 == 0) {
        // 0x800051B8: lw          $a0, 0x8($a3)
        ctx->r4 = MEM_W(ctx->r7, 0X8);
            goto L_800051D0;
    }
    goto skip_0;
    // 0x800051B8: lw          $a0, 0x8($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X8);
    skip_0:
    // 0x800051BC: lhu         $t7, 0x2($a3)
    ctx->r15 = MEM_HU(ctx->r7, 0X2);
    // 0x800051C0: and         $t8, $t7, $a1
    ctx->r24 = ctx->r15 & ctx->r5;
    // 0x800051C4: beql        $t8, $zero, L_800053B0
    if (ctx->r24 == 0) {
        // 0x800051C8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_800053B0;
    }
    goto skip_1;
    // 0x800051C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x800051CC: lw          $a0, 0x8($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X8);
L_800051D0:
    // 0x800051D0: lw          $a2, 0x14($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X14);
    // 0x800051D4: beq         $a0, $zero, L_80005248
    if (ctx->r4 == 0) {
        // 0x800051D8: nop
    
            goto L_80005248;
    }
    // 0x800051D8: nop

    // 0x800051DC: lw          $a1, 0x14($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X14);
    // 0x800051E0: bnel        $a3, $a1, L_8000520C
    if (ctx->r7 != ctx->r5) {
        // 0x800051E4: lw          $v1, 0x10($a1)
        ctx->r3 = MEM_W(ctx->r5, 0X10);
            goto L_8000520C;
    }
    goto skip_2;
    // 0x800051E4: lw          $v1, 0x10($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X10);
    skip_2:
    // 0x800051E8: beql        $a2, $zero, L_800051FC
    if (ctx->r6 == 0) {
        // 0x800051EC: lw          $t9, 0x10($a3)
        ctx->r25 = MEM_W(ctx->r7, 0X10);
            goto L_800051FC;
    }
    goto skip_3;
    // 0x800051EC: lw          $t9, 0x10($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X10);
    skip_3:
    // 0x800051F0: b           L_80005200
    // 0x800051F4: sw          $a2, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->r6;
        goto L_80005200;
    // 0x800051F4: sw          $a2, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->r6;
    // 0x800051F8: lw          $t9, 0x10($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X10);
L_800051FC:
    // 0x800051FC: sw          $t9, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->r25;
L_80005200:
    // 0x80005200: b           L_80005248
    // 0x80005204: lw          $a2, 0x14($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X14);
        goto L_80005248;
    // 0x80005204: lw          $a2, 0x14($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X14);
    // 0x80005208: lw          $v1, 0x10($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X10);
L_8000520C:
    // 0x8000520C: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80005210: beq         $a3, $v1, L_80005228
    if (ctx->r7 == ctx->r3) {
        // 0x80005214: nop
    
            goto L_80005228;
    }
    // 0x80005214: nop

    // 0x80005218: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8000521C:
    // 0x8000521C: lw          $v1, 0x10($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X10);
    // 0x80005220: bnel        $a3, $v1, L_8000521C
    if (ctx->r7 != ctx->r3) {
        // 0x80005224: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_8000521C;
    }
    goto skip_4;
    // 0x80005224: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_4:
L_80005228:
    // 0x80005228: beql        $a2, $zero, L_80005240
    if (ctx->r6 == 0) {
        // 0x8000522C: lw          $t0, 0x10($a3)
        ctx->r8 = MEM_W(ctx->r7, 0X10);
            goto L_80005240;
    }
    goto skip_5;
    // 0x8000522C: lw          $t0, 0x10($a3)
    ctx->r8 = MEM_W(ctx->r7, 0X10);
    skip_5:
    // 0x80005230: sw          $a2, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r6;
    // 0x80005234: b           L_80005248
    // 0x80005238: lw          $a2, 0x14($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X14);
        goto L_80005248;
    // 0x80005238: lw          $a2, 0x14($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X14);
    // 0x8000523C: lw          $t0, 0x10($a3)
    ctx->r8 = MEM_W(ctx->r7, 0X10);
L_80005240:
    // 0x80005240: sw          $t0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r8;
    // 0x80005244: lw          $a2, 0x14($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X14);
L_80005248:
    // 0x80005248: beql        $a2, $zero, L_80005290
    if (ctx->r6 == 0) {
        // 0x8000524C: lh          $t5, 0x0($a3)
        ctx->r13 = MEM_H(ctx->r7, 0X0);
            goto L_80005290;
    }
    goto skip_6;
    // 0x8000524C: lh          $t5, 0x0($a3)
    ctx->r13 = MEM_H(ctx->r7, 0X0);
    skip_6:
    // 0x80005250: lw          $t1, 0x10($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X10);
    // 0x80005254: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80005258: beql        $t1, $zero, L_80005288
    if (ctx->r9 == 0) {
        // 0x8000525C: lw          $t4, 0x10($a3)
        ctx->r12 = MEM_W(ctx->r7, 0X10);
            goto L_80005288;
    }
    goto skip_7;
    // 0x8000525C: lw          $t4, 0x10($a3)
    ctx->r12 = MEM_W(ctx->r7, 0X10);
    skip_7:
    // 0x80005260: lw          $t2, 0xC($v0)
    ctx->r10 = MEM_W(ctx->r2, 0XC);
L_80005264:
    // 0x80005264: sw          $a0, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r4;
    // 0x80005268: beql        $t2, $zero, L_80005278
    if (ctx->r10 == 0) {
        // 0x8000526C: lw          $v0, 0x10($v0)
        ctx->r2 = MEM_W(ctx->r2, 0X10);
            goto L_80005278;
    }
    goto skip_8;
    // 0x8000526C: lw          $v0, 0x10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X10);
    skip_8:
    // 0x80005270: sw          $a0, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r4;
    // 0x80005274: lw          $v0, 0x10($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X10);
L_80005278:
    // 0x80005278: lw          $t3, 0x10($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X10);
    // 0x8000527C: bnel        $t3, $zero, L_80005264
    if (ctx->r11 != 0) {
        // 0x80005280: lw          $t2, 0xC($v0)
        ctx->r10 = MEM_W(ctx->r2, 0XC);
            goto L_80005264;
    }
    goto skip_9;
    // 0x80005280: lw          $t2, 0xC($v0)
    ctx->r10 = MEM_W(ctx->r2, 0XC);
    skip_9:
    // 0x80005284: lw          $t4, 0x10($a3)
    ctx->r12 = MEM_W(ctx->r7, 0X10);
L_80005288:
    // 0x80005288: sw          $t4, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r12;
    // 0x8000528C: lh          $t5, 0x0($a3)
    ctx->r13 = MEM_H(ctx->r7, 0X0);
L_80005290:
    // 0x80005290: andi        $t6, $t5, 0xC0
    ctx->r14 = ctx->r13 & 0XC0;
    // 0x80005294: bnel        $t6, $zero, L_80005318
    if (ctx->r14 != 0) {
        // 0x80005298: lw          $a1, 0x44($a3)
        ctx->r5 = MEM_W(ctx->r7, 0X44);
            goto L_80005318;
    }
    goto skip_10;
    // 0x80005298: lw          $a1, 0x44($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X44);
    skip_10:
    // 0x8000529C: lw          $a1, 0x38($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X38);
    // 0x800052A0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800052A4: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x800052A8: sll         $t7, $a1, 0
    ctx->r15 = S32(ctx->r5 << 0);
    // 0x800052AC: bgezl       $t7, L_800052C8
    if (SIGNED(ctx->r15) >= 0) {
        // 0x800052B0: lw          $a1, 0x3C($a3)
        ctx->r5 = MEM_W(ctx->r7, 0X3C);
            goto L_800052C8;
    }
    goto skip_11;
    // 0x800052B0: lw          $a1, 0x3C($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X3C);
    skip_11:
    // 0x800052B4: lw          $a0, 0x1540($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1540);
    // 0x800052B8: jal         0x80002D5C
    // 0x800052BC: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    GameStateMgr_enqueue(rdram, ctx);
        goto after_1;
    // 0x800052BC: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    after_1:
    // 0x800052C0: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
    // 0x800052C4: lw          $a1, 0x3C($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X3C);
L_800052C8:
    // 0x800052C8: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800052CC: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x800052D0: sll         $t8, $a1, 0
    ctx->r24 = S32(ctx->r5 << 0);
    // 0x800052D4: bgezl       $t8, L_800052F0
    if (SIGNED(ctx->r24) >= 0) {
        // 0x800052D8: lw          $a1, 0x34($a3)
        ctx->r5 = MEM_W(ctx->r7, 0X34);
            goto L_800052F0;
    }
    goto skip_12;
    // 0x800052D8: lw          $a1, 0x34($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X34);
    skip_12:
    // 0x800052DC: lw          $a0, 0x1540($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1540);
    // 0x800052E0: jal         0x80002D5C
    // 0x800052E4: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    GameStateMgr_enqueue(rdram, ctx);
        goto after_2;
    // 0x800052E4: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    after_2:
    // 0x800052E8: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
    // 0x800052EC: lw          $a1, 0x34($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X34);
L_800052F0:
    // 0x800052F0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800052F4: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x800052F8: sll         $t9, $a1, 0
    ctx->r25 = S32(ctx->r5 << 0);
    // 0x800052FC: bgezl       $t9, L_80005318
    if (SIGNED(ctx->r25) >= 0) {
        // 0x80005300: lw          $a1, 0x44($a3)
        ctx->r5 = MEM_W(ctx->r7, 0X44);
            goto L_80005318;
    }
    goto skip_13;
    // 0x80005300: lw          $a1, 0x44($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X44);
    skip_13:
    // 0x80005304: lw          $a0, 0x1540($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1540);
    // 0x80005308: jal         0x80002D5C
    // 0x8000530C: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    GameStateMgr_enqueue(rdram, ctx);
        goto after_3;
    // 0x8000530C: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    after_3:
    // 0x80005310: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
    // 0x80005314: lw          $a1, 0x44($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X44);
L_80005318:
    // 0x80005318: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8000531C: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x80005320: beql        $a1, $zero, L_80005394
    if (ctx->r5 == 0) {
        // 0x80005324: lhu         $t4, 0x2($a3)
        ctx->r12 = MEM_HU(ctx->r7, 0X2);
            goto L_80005394;
    }
    goto skip_14;
    // 0x80005324: lhu         $t4, 0x2($a3)
    ctx->r12 = MEM_HU(ctx->r7, 0X2);
    skip_14:
    // 0x80005328: lw          $a0, 0x1540($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1540);
    // 0x8000532C: jal         0x80002D5C
    // 0x80005330: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    GameStateMgr_enqueue(rdram, ctx);
        goto after_4;
    // 0x80005330: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    after_4:
    // 0x80005334: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
    // 0x80005338: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8000533C: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x80005340: lw          $a1, 0x44($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X44);
    // 0x80005344: lw          $t0, 0x48($a3)
    ctx->r8 = MEM_W(ctx->r7, 0X48);
    // 0x80005348: beq         $a1, $t0, L_80005368
    if (ctx->r5 == ctx->r8) {
        // 0x8000534C: sw          $t0, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r8;
            goto L_80005368;
    }
    // 0x8000534C: sw          $t0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r8;
    // 0x80005350: lw          $a0, 0x1540($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1540);
    // 0x80005354: or          $a1, $t0, $zero
    ctx->r5 = ctx->r8 | 0;
    // 0x80005358: jal         0x80002D5C
    // 0x8000535C: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    GameStateMgr_enqueue(rdram, ctx);
        goto after_5;
    // 0x8000535C: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    after_5:
    // 0x80005360: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
    // 0x80005364: lw          $a1, 0x44($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X44);
L_80005368:
    // 0x80005368: lw          $t2, 0x4C($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X4C);
    // 0x8000536C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80005370: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x80005374: beq         $a1, $t2, L_80005390
    if (ctx->r5 == ctx->r10) {
        // 0x80005378: sw          $t2, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r10;
            goto L_80005390;
    }
    // 0x80005378: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    // 0x8000537C: lw          $a0, 0x1540($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1540);
    // 0x80005380: or          $a1, $t2, $zero
    ctx->r5 = ctx->r10 | 0;
    // 0x80005384: jal         0x80002D5C
    // 0x80005388: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    GameStateMgr_enqueue(rdram, ctx);
        goto after_6;
    // 0x80005388: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    after_6:
    // 0x8000538C: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
L_80005390:
    // 0x80005390: lhu         $t4, 0x2($a3)
    ctx->r12 = MEM_HU(ctx->r7, 0X2);
L_80005394:
    // 0x80005394: andi        $t5, $t4, 0x10
    ctx->r13 = ctx->r12 & 0X10;
    // 0x80005398: beql        $t5, $zero, L_800053AC
    if (ctx->r13 == 0) {
        // 0x8000539C: sw          $zero, 0x0($a3)
        MEM_W(0X0, ctx->r7) = 0;
            goto L_800053AC;
    }
    goto skip_15;
    // 0x8000539C: sw          $zero, 0x0($a3)
    MEM_W(0X0, ctx->r7) = 0;
    skip_15:
    // 0x800053A0: lw          $t6, 0x74($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X74);
    // 0x800053A4: sw          $zero, 0x0($t6)
    MEM_W(0X0, ctx->r14) = 0;
    // 0x800053A8: sw          $zero, 0x0($a3)
    MEM_W(0X0, ctx->r7) = 0;
L_800053AC:
    // 0x800053AC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_800053B0:
    // 0x800053B0: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x800053B4: jr          $ra
    // 0x800053B8: nop

    return;
    // 0x800053B8: nop

;}
RECOMP_FUNC void func_800053BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800053BC: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800053C0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x800053C4: addiu       $a0, $a0, 0x1554
    ctx->r4 = ADD32(ctx->r4, 0X1554);
    // 0x800053C8: sh          $zero, -0x7CA4($at)
    MEM_H(-0X7CA4, ctx->r1) = 0;
    // 0x800053CC: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x800053D0: lw          $v1, 0x1550($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1550);
    // 0x800053D4: lw          $t6, 0x0($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X0);
    // 0x800053D8: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x800053DC: addiu       $a1, $a1, 0x1564
    ctx->r5 = ADD32(ctx->r5, 0X1564);
    // 0x800053E0: sltu        $at, $v1, $t6
    ctx->r1 = ctx->r3 < ctx->r14 ? 1 : 0;
    // 0x800053E4: beq         $at, $zero, L_8000540C
    if (ctx->r1 == 0) {
        // 0x800053E8: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_8000540C;
    }
    // 0x800053E8: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x800053EC: sw          $zero, 0x0($v0)
    MEM_W(0X0, ctx->r2) = 0;
L_800053F0:
    // 0x800053F0: lw          $t7, 0x0($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X0);
    // 0x800053F4: addiu       $v0, $v0, 0xB8
    ctx->r2 = ADD32(ctx->r2, 0XB8);
    // 0x800053F8: sltu        $at, $v0, $t7
    ctx->r1 = ctx->r2 < ctx->r15 ? 1 : 0;
    // 0x800053FC: bnel        $at, $zero, L_800053F0
    if (ctx->r1 != 0) {
        // 0x80005400: sw          $zero, 0x0($v0)
        MEM_W(0X0, ctx->r2) = 0;
            goto L_800053F0;
    }
    goto skip_0;
    // 0x80005400: sw          $zero, 0x0($v0)
    MEM_W(0X0, ctx->r2) = 0;
    skip_0:
    // 0x80005404: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80005408: lw          $v1, 0x1550($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1550);
L_8000540C:
    // 0x8000540C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80005410: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80005414: addiu       $a0, $a0, 0x1570
    ctx->r4 = ADD32(ctx->r4, 0X1570);
    // 0x80005418: sw          $v1, 0x155C($at)
    MEM_W(0X155C, ctx->r1) = ctx->r3;
    // 0x8000541C: lw          $t8, 0x0($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X0);
    // 0x80005420: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80005424: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80005428: addiu       $t9, $t8, 0x1F
    ctx->r25 = ADD32(ctx->r24, 0X1F);
    // 0x8000542C: bgez        $t9, L_8000543C
    if (SIGNED(ctx->r25) >= 0) {
        // 0x80005430: sra         $t0, $t9, 5
        ctx->r8 = S32(SIGNED(ctx->r25) >> 5);
            goto L_8000543C;
    }
    // 0x80005430: sra         $t0, $t9, 5
    ctx->r8 = S32(SIGNED(ctx->r25) >> 5);
    // 0x80005434: addiu       $at, $t9, 0x1F
    ctx->r1 = ADD32(ctx->r25, 0X1F);
    // 0x80005438: sra         $t0, $at, 5
    ctx->r8 = S32(SIGNED(ctx->r1) >> 5);
L_8000543C:
    // 0x8000543C: blez        $t0, L_8000547C
    if (SIGNED(ctx->r8) <= 0) {
        // 0x80005440: nop
    
            goto L_8000547C;
    }
    // 0x80005440: nop

    // 0x80005444: lw          $t1, 0x0($a1)
    ctx->r9 = MEM_W(ctx->r5, 0X0);
L_80005448:
    // 0x80005448: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x8000544C: addu        $t2, $t1, $v1
    ctx->r10 = ADD32(ctx->r9, ctx->r3);
    // 0x80005450: sw          $zero, 0x0($t2)
    MEM_W(0X0, ctx->r10) = 0;
    // 0x80005454: lw          $t3, 0x0($a0)
    ctx->r11 = MEM_W(ctx->r4, 0X0);
    // 0x80005458: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8000545C: addiu       $t4, $t3, 0x1F
    ctx->r12 = ADD32(ctx->r11, 0X1F);
    // 0x80005460: bgez        $t4, L_80005470
    if (SIGNED(ctx->r12) >= 0) {
        // 0x80005464: sra         $t5, $t4, 5
        ctx->r13 = S32(SIGNED(ctx->r12) >> 5);
            goto L_80005470;
    }
    // 0x80005464: sra         $t5, $t4, 5
    ctx->r13 = S32(SIGNED(ctx->r12) >> 5);
    // 0x80005468: addiu       $at, $t4, 0x1F
    ctx->r1 = ADD32(ctx->r12, 0X1F);
    // 0x8000546C: sra         $t5, $at, 5
    ctx->r13 = S32(SIGNED(ctx->r1) >> 5);
L_80005470:
    // 0x80005470: slt         $at, $v0, $t5
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r13) ? 1 : 0;
    // 0x80005474: bnel        $at, $zero, L_80005448
    if (ctx->r1 != 0) {
        // 0x80005478: lw          $t1, 0x0($a1)
        ctx->r9 = MEM_W(ctx->r5, 0X0);
            goto L_80005448;
    }
    goto skip_1;
    // 0x80005478: lw          $t1, 0x0($a1)
    ctx->r9 = MEM_W(ctx->r5, 0X0);
    skip_1:
L_8000547C:
    // 0x8000547C: jr          $ra
    // 0x80005480: nop

    return;
    // 0x80005480: nop

;}
RECOMP_FUNC void func_80005484(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005484: lui         $t6, 0x800C
    ctx->r14 = S32(0X800C << 16);
    // 0x80005488: lw          $t6, 0x1568($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X1568);
    // 0x8000548C: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x80005490: sltu        $v0, $a0, $t6
    ctx->r2 = ctx->r4 < ctx->r14 ? 1 : 0;
    // 0x80005494: xori        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 ^ 0X1;
    // 0x80005498: beq         $v0, $zero, L_800054A8
    if (ctx->r2 == 0) {
        // 0x8000549C: nop
    
            goto L_800054A8;
    }
    // 0x8000549C: nop

    // 0x800054A0: lw          $t7, 0x156C($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X156C);
    // 0x800054A4: sltu        $v0, $a0, $t7
    ctx->r2 = ctx->r4 < ctx->r15 ? 1 : 0;
L_800054A8:
    // 0x800054A8: jr          $ra
    // 0x800054AC: nop

    return;
    // 0x800054AC: nop

;}
RECOMP_FUNC void func_800054B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800054B0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800054B4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800054B8: bne         $a0, $zero, L_800054C8
    if (ctx->r4 != 0) {
        // 0x800054BC: or          $a3, $a0, $zero
        ctx->r7 = ctx->r4 | 0;
            goto L_800054C8;
    }
    // 0x800054BC: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x800054C0: lui         $a3, 0x800C
    ctx->r7 = S32(0X800C << 16);
    // 0x800054C4: lw          $a3, 0x1568($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X1568);
L_800054C8:
    // 0x800054C8: lui         $t6, 0x800C
    ctx->r14 = S32(0X800C << 16);
    // 0x800054CC: lw          $t6, 0x1568($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X1568);
    // 0x800054D0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800054D4: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x800054D8: subu        $a1, $a3, $t6
    ctx->r5 = SUB32(ctx->r7, ctx->r14);
    // 0x800054DC: sra         $a1, $a1, 6
    ctx->r5 = S32(SIGNED(ctx->r5) >> 6);
    // 0x800054E0: lw          $a2, 0x1570($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X1570);
    // 0x800054E4: jal         0x80004940
    // 0x800054E8: lw          $a0, 0x1564($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1564);
    func_80004940(rdram, ctx);
        goto after_0;
    // 0x800054E8: lw          $a0, 0x1564($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1564);
    after_0:
    // 0x800054EC: bgez        $v0, L_800054FC
    if (SIGNED(ctx->r2) >= 0) {
        // 0x800054F0: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_800054FC;
    }
    // 0x800054F0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x800054F4: b           L_80005520
    // 0x800054F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80005520;
    // 0x800054F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800054FC:
    // 0x800054FC: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80005500: lw          $a0, 0x1564($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1564);
    // 0x80005504: jal         0x800048C4
    // 0x80005508: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_800048C4(rdram, ctx);
        goto after_1;
    // 0x80005508: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_1:
    // 0x8000550C: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x80005510: lui         $t8, 0x800C
    ctx->r24 = S32(0X800C << 16);
    // 0x80005514: lw          $t8, 0x1568($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X1568);
    // 0x80005518: sll         $t7, $a1, 6
    ctx->r15 = S32(ctx->r5 << 6);
    // 0x8000551C: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
L_80005520:
    // 0x80005520: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005524: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80005528: jr          $ra
    // 0x8000552C: nop

    return;
    // 0x8000552C: nop

;}
RECOMP_FUNC void func_80005530(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005530: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80005534: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80005538: jal         0x80005484
    // 0x8000553C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80005484(rdram, ctx);
        goto after_0;
    // 0x8000553C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80005540: beq         $v0, $zero, L_80005564
    if (ctx->r2 == 0) {
        // 0x80005544: lw          $t6, 0x18($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X18);
            goto L_80005564;
    }
    // 0x80005544: lw          $t6, 0x18($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X18);
    // 0x80005548: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x8000554C: lw          $t7, 0x1568($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X1568);
    // 0x80005550: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80005554: lw          $a0, 0x1564($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1564);
    // 0x80005558: subu        $a1, $t6, $t7
    ctx->r5 = SUB32(ctx->r14, ctx->r15);
    // 0x8000555C: jal         0x800048EC
    // 0x80005560: sra         $a1, $a1, 6
    ctx->r5 = S32(SIGNED(ctx->r5) >> 6);
    func_800048EC(rdram, ctx);
        goto after_1;
    // 0x80005560: sra         $a1, $a1, 6
    ctx->r5 = S32(SIGNED(ctx->r5) >> 6);
    after_1:
L_80005564:
    // 0x80005564: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005568: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8000556C: jr          $ra
    // 0x80005570: nop

    return;
    // 0x80005570: nop

;}
RECOMP_FUNC void func_80005574(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005574: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80005578: andi        $t7, $a0, 0x10DF
    ctx->r15 = ctx->r4 & 0X10DF;
    // 0x8000557C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80005580: beq         $t7, $zero, L_80005658
    if (ctx->r15 == 0) {
        // 0x80005584: sw          $a0, 0x30($sp)
        MEM_W(0X30, ctx->r29) = ctx->r4;
            goto L_80005658;
    }
    // 0x80005584: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80005588: jal         0x800054B0
    // 0x8000558C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_800054B0(rdram, ctx);
        goto after_0;
    // 0x8000558C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x80005590: lw          $t8, 0x30($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X30);
    // 0x80005594: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x80005598: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x8000559C: andi        $t9, $t8, 0x2000
    ctx->r25 = ctx->r24 & 0X2000;
    // 0x800055A0: beq         $t9, $zero, L_800055B0
    if (ctx->r25 == 0) {
        // 0x800055A4: or          $a0, $a3, $zero
        ctx->r4 = ctx->r7 | 0;
            goto L_800055B0;
    }
    // 0x800055A4: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x800055A8: b           L_800055EC
    // 0x800055AC: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
        goto L_800055EC;
    // 0x800055AC: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
L_800055B0:
    // 0x800055B0: jal         0x800054B0
    // 0x800055B4: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_800054B0(rdram, ctx);
        goto after_1;
    // 0x800055B4: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_1:
    // 0x800055B8: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x800055BC: lh          $t0, -0x7D1C($t0)
    ctx->r8 = MEM_H(ctx->r8, -0X7D1C);
    // 0x800055C0: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x800055C4: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x800055C8: slti        $at, $t0, 0x3
    ctx->r1 = SIGNED(ctx->r8) < 0X3 ? 1 : 0;
    // 0x800055CC: bne         $at, $zero, L_800055EC
    if (ctx->r1 != 0) {
        // 0x800055D0: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_800055EC;
    }
    // 0x800055D0: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x800055D4: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x800055D8: jal         0x800054B0
    // 0x800055DC: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_800054B0(rdram, ctx);
        goto after_2;
    // 0x800055DC: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_2:
    // 0x800055E0: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x800055E4: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x800055E8: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
L_800055EC:
    // 0x800055EC: beq         $a3, $zero, L_80005618
    if (ctx->r7 == 0) {
        // 0x800055F0: nop
    
            goto L_80005618;
    }
    // 0x800055F0: nop

    // 0x800055F4: beq         $a2, $zero, L_80005618
    if (ctx->r6 == 0) {
        // 0x800055F8: lui         $t1, 0x801D
        ctx->r9 = S32(0X801D << 16);
            goto L_80005618;
    }
    // 0x800055F8: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x800055FC: lh          $t1, -0x7D1C($t1)
    ctx->r9 = MEM_H(ctx->r9, -0X7D1C);
    // 0x80005600: lw          $t2, 0x20($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X20);
    // 0x80005604: slti        $at, $t1, 0x3
    ctx->r1 = SIGNED(ctx->r9) < 0X3 ? 1 : 0;
    // 0x80005608: bne         $at, $zero, L_80005664
    if (ctx->r1 != 0) {
        // 0x8000560C: nop
    
            goto L_80005664;
    }
    // 0x8000560C: nop

    // 0x80005610: bne         $t2, $zero, L_80005664
    if (ctx->r10 != 0) {
        // 0x80005614: nop
    
            goto L_80005664;
    }
    // 0x80005614: nop

L_80005618:
    // 0x80005618: beq         $a3, $zero, L_8000562C
    if (ctx->r7 == 0) {
        // 0x8000561C: or          $a0, $a3, $zero
        ctx->r4 = ctx->r7 | 0;
            goto L_8000562C;
    }
    // 0x8000561C: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80005620: jal         0x80005530
    // 0x80005624: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    func_80005530(rdram, ctx);
        goto after_3;
    // 0x80005624: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    after_3:
    // 0x80005628: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
L_8000562C:
    // 0x8000562C: beql        $a2, $zero, L_80005640
    if (ctx->r6 == 0) {
        // 0x80005630: lw          $t3, 0x20($sp)
        ctx->r11 = MEM_W(ctx->r29, 0X20);
            goto L_80005640;
    }
    goto skip_0;
    // 0x80005630: lw          $t3, 0x20($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X20);
    skip_0:
    // 0x80005634: jal         0x80005530
    // 0x80005638: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    func_80005530(rdram, ctx);
        goto after_4;
    // 0x80005638: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_4:
    // 0x8000563C: lw          $t3, 0x20($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X20);
L_80005640:
    // 0x80005640: beq         $t3, $zero, L_80005650
    if (ctx->r11 == 0) {
        // 0x80005644: nop
    
            goto L_80005650;
    }
    // 0x80005644: nop

    // 0x80005648: jal         0x80005530
    // 0x8000564C: or          $a0, $t3, $zero
    ctx->r4 = ctx->r11 | 0;
    func_80005530(rdram, ctx);
        goto after_5;
    // 0x8000564C: or          $a0, $t3, $zero
    ctx->r4 = ctx->r11 | 0;
    after_5:
L_80005650:
    // 0x80005650: b           L_8000571C
    // 0x80005654: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8000571C;
    // 0x80005654: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80005658:
    // 0x80005658: sw          $zero, 0x20($sp)
    MEM_W(0X20, ctx->r29) = 0;
    // 0x8000565C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80005660: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
L_80005664:
    // 0x80005664: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80005668: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x8000566C: lw          $v0, 0x1554($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1554);
    // 0x80005670: lw          $a0, 0x1550($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1550);
    // 0x80005674: lw          $t5, 0x30($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X30);
    // 0x80005678: addiu       $a1, $zero, 0xB8
    ctx->r5 = ADD32(0, 0XB8);
    // 0x8000567C: sltu        $at, $a0, $v0
    ctx->r1 = ctx->r4 < ctx->r2 ? 1 : 0;
    // 0x80005680: beq         $at, $zero, L_80005718
    if (ctx->r1 == 0) {
        // 0x80005684: andi        $t6, $t5, 0x2000
        ctx->r14 = ctx->r13 & 0X2000;
            goto L_80005718;
    }
    // 0x80005684: andi        $t6, $t5, 0x2000
    ctx->r14 = ctx->r13 & 0X2000;
    // 0x80005688: lh          $t4, 0x0($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X0);
L_8000568C:
    // 0x8000568C: bnel        $t4, $zero, L_8000570C
    if (ctx->r12 != 0) {
        // 0x80005690: addiu       $a0, $a0, 0xB8
        ctx->r4 = ADD32(ctx->r4, 0XB8);
            goto L_8000570C;
    }
    goto skip_1;
    // 0x80005690: addiu       $a0, $a0, 0xB8
    ctx->r4 = ADD32(ctx->r4, 0XB8);
    skip_1:
    // 0x80005694: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    // 0x80005698: sw          $a0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r4;
    // 0x8000569C: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    // 0x800056A0: jal         0x80000F30
    // 0x800056A4: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    bzero_recomp(rdram, ctx);
        goto after_6;
    // 0x800056A4: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_6:
    // 0x800056A8: lw          $a0, 0x2C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X2C);
    // 0x800056AC: lw          $t7, 0x30($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X30);
    // 0x800056B0: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x800056B4: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x800056B8: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    // 0x800056BC: lw          $t8, 0x1C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X1C);
    // 0x800056C0: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x800056C4: addiu       $v0, $v0, 0x155C
    ctx->r2 = ADD32(ctx->r2, 0X155C);
    // 0x800056C8: beq         $t8, $zero, L_800056DC
    if (ctx->r24 == 0) {
        // 0x800056CC: addiu       $t2, $a0, 0xB8
        ctx->r10 = ADD32(ctx->r4, 0XB8);
            goto L_800056DC;
    }
    // 0x800056CC: addiu       $t2, $a0, 0xB8
    ctx->r10 = ADD32(ctx->r4, 0XB8);
    // 0x800056D0: lhu         $t9, 0x2($a0)
    ctx->r25 = MEM_HU(ctx->r4, 0X2);
    // 0x800056D4: ori         $t0, $t9, 0x4000
    ctx->r8 = ctx->r25 | 0X4000;
    // 0x800056D8: sh          $t0, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r8;
L_800056DC:
    // 0x800056DC: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X0);
    // 0x800056E0: sltu        $at, $a0, $t1
    ctx->r1 = ctx->r4 < ctx->r9 ? 1 : 0;
    // 0x800056E4: bnel        $at, $zero, L_800056F4
    if (ctx->r1 != 0) {
        // 0x800056E8: sw          $a3, 0x44($a0)
        MEM_W(0X44, ctx->r4) = ctx->r7;
            goto L_800056F4;
    }
    goto skip_2;
    // 0x800056E8: sw          $a3, 0x44($a0)
    MEM_W(0X44, ctx->r4) = ctx->r7;
    skip_2:
    // 0x800056EC: sw          $t2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r10;
    // 0x800056F0: sw          $a3, 0x44($a0)
    MEM_W(0X44, ctx->r4) = ctx->r7;
L_800056F4:
    // 0x800056F4: sw          $a2, 0x48($a0)
    MEM_W(0X48, ctx->r4) = ctx->r6;
    // 0x800056F8: lw          $t3, 0x20($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X20);
    // 0x800056FC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80005700: b           L_8000571C
    // 0x80005704: sw          $t3, 0x4C($a0)
    MEM_W(0X4C, ctx->r4) = ctx->r11;
        goto L_8000571C;
    // 0x80005704: sw          $t3, 0x4C($a0)
    MEM_W(0X4C, ctx->r4) = ctx->r11;
    // 0x80005708: addiu       $a0, $a0, 0xB8
    ctx->r4 = ADD32(ctx->r4, 0XB8);
L_8000570C:
    // 0x8000570C: sltu        $at, $a0, $v0
    ctx->r1 = ctx->r4 < ctx->r2 ? 1 : 0;
    // 0x80005710: bnel        $at, $zero, L_8000568C
    if (ctx->r1 != 0) {
        // 0x80005714: lh          $t4, 0x0($a0)
        ctx->r12 = MEM_H(ctx->r4, 0X0);
            goto L_8000568C;
    }
    goto skip_3;
    // 0x80005714: lh          $t4, 0x0($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X0);
    skip_3:
L_80005718:
    // 0x80005718: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8000571C:
    // 0x8000571C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005720: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80005724: jr          $ra
    // 0x80005728: nop

    return;
    // 0x80005728: nop

;}
RECOMP_FUNC void func_8000572C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000572C: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x80005730: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80005734: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80005738: ori         $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 | 0X8000;
    // 0x8000573C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80005740: sh          $t7, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r15;
    // 0x80005744: addiu       $t9, $t9, -0x17CC
    ctx->r25 = ADD32(ctx->r25, -0X17CC);
    // 0x80005748: sll         $t8, $a1, 4
    ctx->r24 = S32(ctx->r5 << 4);
    // 0x8000574C: lh          $t1, 0x34($v1)
    ctx->r9 = MEM_H(ctx->r3, 0X34);
    // 0x80005750: addu        $v0, $t8, $t9
    ctx->r2 = ADD32(ctx->r24, ctx->r25);
    // 0x80005754: lw          $t0, 0x0($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X0);
    // 0x80005758: lh          $t4, 0x0($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X0);
    // 0x8000575C: sll         $t2, $t1, 4
    ctx->r10 = S32(ctx->r9 << 4);
    // 0x80005760: addu        $t3, $t0, $t2
    ctx->r11 = ADD32(ctx->r8, ctx->r10);
    // 0x80005764: andi        $t5, $t4, 0x40
    ctx->r13 = ctx->r12 & 0X40;
    // 0x80005768: beq         $t5, $zero, L_80005790
    if (ctx->r13 == 0) {
        // 0x8000576C: sw          $t3, 0x38($a0)
        MEM_W(0X38, ctx->r4) = ctx->r11;
            goto L_80005790;
    }
    // 0x8000576C: sw          $t3, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->r11;
    // 0x80005770: lh          $t7, 0x34($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X34);
    // 0x80005774: lw          $t6, 0x8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X8);
    // 0x80005778: sll         $t8, $t7, 3
    ctx->r24 = S32(ctx->r15 << 3);
    // 0x8000577C: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x80005780: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80005784: addu        $t9, $t6, $t8
    ctx->r25 = ADD32(ctx->r14, ctx->r24);
    // 0x80005788: b           L_80005798
    // 0x8000578C: sw          $t9, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r25;
        goto L_80005798;
    // 0x8000578C: sw          $t9, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r25;
L_80005790:
    // 0x80005790: lw          $t1, 0x4($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X4);
    // 0x80005794: sw          $t1, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r9;
L_80005798:
    // 0x80005798: lh          $t2, 0x34($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X34);
    // 0x8000579C: lw          $t0, 0xC($v0)
    ctx->r8 = MEM_W(ctx->r2, 0XC);
    // 0x800057A0: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x800057A4: addu        $t4, $t0, $t3
    ctx->r12 = ADD32(ctx->r8, ctx->r11);
    // 0x800057A8: lw          $t5, 0x0($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X0);
    // 0x800057AC: jr          $ra
    // 0x800057B0: sw          $t5, 0x40($a0)
    MEM_W(0X40, ctx->r4) = ctx->r13;
    return;
    // 0x800057B0: sw          $t5, 0x40($a0)
    MEM_W(0X40, ctx->r4) = ctx->r13;
;}
RECOMP_FUNC void func_800057B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800057B4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800057B8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800057BC: jal         0x80005574
    // 0x800057C0: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_80005574(rdram, ctx);
        goto after_0;
    // 0x800057C0: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_0:
    // 0x800057C4: beq         $v0, $zero, L_800057DC
    if (ctx->r2 == 0) {
        // 0x800057C8: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_800057DC;
    }
    // 0x800057C8: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x800057CC: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x800057D0: jal         0x8000572C
    // 0x800057D4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_8000572C(rdram, ctx);
        goto after_1;
    // 0x800057D4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x800057D8: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_800057DC:
    // 0x800057DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800057E0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800057E4: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x800057E8: jr          $ra
    // 0x800057EC: nop

    return;
    // 0x800057EC: nop

;}
RECOMP_FUNC void func_800057F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800057F0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800057F4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800057F8: jal         0x80005574
    // 0x800057FC: nop

    func_80005574(rdram, ctx);
        goto after_0;
    // 0x800057FC: nop

    after_0:
    // 0x80005800: beq         $v0, $zero, L_80005848
    if (ctx->r2 == 0) {
        // 0x80005804: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80005848;
    }
    // 0x80005804: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80005808: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x8000580C: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    // 0x80005810: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80005814: ori         $t7, $t6, 0x6000
    ctx->r15 = ctx->r14 | 0X6000;
    // 0x80005818: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x8000581C: sw          $t8, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r24;
    // 0x80005820: addiu       $t9, $t9, 0x1858
    ctx->r25 = ADD32(ctx->r25, 0X1858);
    // 0x80005824: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80005828: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8000582C: jalr        $t9
    // 0x80005830: addiu       $a1, $zero, 0x78
    ctx->r5 = ADD32(0, 0X78);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80005830: addiu       $a1, $zero, 0x78
    ctx->r5 = ADD32(0, 0X78);
    after_1:
    // 0x80005834: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80005838: bne         $v0, $zero, L_80005848
    if (ctx->r2 != 0) {
        // 0x8000583C: sw          $v0, 0x3C($v1)
        MEM_W(0X3C, ctx->r3) = ctx->r2;
            goto L_80005848;
    }
    // 0x8000583C: sw          $v0, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = ctx->r2;
    // 0x80005840: sh          $zero, 0x0($v1)
    MEM_H(0X0, ctx->r3) = 0;
    // 0x80005844: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80005848:
    // 0x80005848: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8000584C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80005850: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80005854: jr          $ra
    // 0x80005858: nop

    return;
    // 0x80005858: nop

;}
RECOMP_FUNC void func_8000585C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000585C: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x80005860: ori         $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 | 0X1000;
    // 0x80005864: beq         $a1, $zero, L_8000589C
    if (ctx->r5 == 0) {
        // 0x80005868: sh          $t7, 0x2($a0)
        MEM_H(0X2, ctx->r4) = ctx->r15;
            goto L_8000589C;
    }
    // 0x80005868: sh          $t7, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r15;
    // 0x8000586C: lw          $t8, 0x8($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X8);
    // 0x80005870: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80005874: sw          $t8, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r24;
    // 0x80005878: lw          $t9, 0x10($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X10);
    // 0x8000587C: beql        $t9, $zero, L_8000589C
    if (ctx->r25 == 0) {
        // 0x80005880: sw          $a0, 0x10($v0)
        MEM_W(0X10, ctx->r2) = ctx->r4;
            goto L_8000589C;
    }
    goto skip_0;
    // 0x80005880: sw          $a0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r4;
    skip_0:
    // 0x80005884: lw          $v1, 0x10($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X10);
    // 0x80005888: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8000588C:
    // 0x8000588C: lw          $v1, 0x10($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X10);
    // 0x80005890: bnel        $v1, $zero, L_8000588C
    if (ctx->r3 != 0) {
        // 0x80005894: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_8000588C;
    }
    goto skip_1;
    // 0x80005894: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_1:
    // 0x80005898: sw          $a0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r4;
L_8000589C:
    // 0x8000589C: jr          $ra
    // 0x800058A0: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x800058A0: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
;}
RECOMP_FUNC void func_800058A4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800058A4: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x800058A8: ori         $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 | 0X1000;
    // 0x800058AC: beq         $a1, $zero, L_800058F0
    if (ctx->r5 == 0) {
        // 0x800058B0: sh          $t7, 0x2($a0)
        MEM_H(0X2, ctx->r4) = ctx->r15;
            goto L_800058F0;
    }
    // 0x800058B0: sh          $t7, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r15;
    // 0x800058B4: sw          $a1, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r5;
    // 0x800058B8: lw          $v0, 0x14($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X14);
    // 0x800058BC: bnel        $v0, $zero, L_800058D4
    if (ctx->r2 != 0) {
        // 0x800058C0: lw          $v1, 0x10($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X10);
            goto L_800058D4;
    }
    goto skip_0;
    // 0x800058C0: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
    skip_0:
    // 0x800058C4: sw          $a0, 0x14($a1)
    MEM_W(0X14, ctx->r5) = ctx->r4;
    // 0x800058C8: jr          $ra
    // 0x800058CC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x800058CC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x800058D0: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
L_800058D4:
    // 0x800058D4: beql        $v1, $zero, L_800058F0
    if (ctx->r3 == 0) {
        // 0x800058D8: sw          $a0, 0x10($v0)
        MEM_W(0X10, ctx->r2) = ctx->r4;
            goto L_800058F0;
    }
    goto skip_1;
    // 0x800058D8: sw          $a0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r4;
    skip_1:
    // 0x800058DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_800058E0:
    // 0x800058E0: lw          $v1, 0x10($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X10);
    // 0x800058E4: bnel        $v1, $zero, L_800058E0
    if (ctx->r3 != 0) {
        // 0x800058E8: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_800058E0;
    }
    goto skip_2;
    // 0x800058E8: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_2:
    // 0x800058EC: sw          $a0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r4;
L_800058F0:
    // 0x800058F0: jr          $ra
    // 0x800058F4: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x800058F4: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
;}
RECOMP_FUNC void func_800058F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800058F8: beq         $a1, $zero, L_80005940
    if (ctx->r5 == 0) {
        // 0x800058FC: nop
    
            goto L_80005940;
    }
    // 0x800058FC: nop

    // 0x80005900: sw          $a1, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r5;
    // 0x80005904: sw          $a1, 0xC($a0)
    MEM_W(0XC, ctx->r4) = ctx->r5;
    // 0x80005908: lw          $v0, 0x14($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X14);
    // 0x8000590C: bnel        $v0, $zero, L_80005924
    if (ctx->r2 != 0) {
        // 0x80005910: lw          $v1, 0x10($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X10);
            goto L_80005924;
    }
    goto skip_0;
    // 0x80005910: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
    skip_0:
    // 0x80005914: sw          $a0, 0x14($a1)
    MEM_W(0X14, ctx->r5) = ctx->r4;
    // 0x80005918: jr          $ra
    // 0x8000591C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x8000591C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80005920: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
L_80005924:
    // 0x80005924: beql        $v1, $zero, L_80005940
    if (ctx->r3 == 0) {
        // 0x80005928: sw          $a0, 0x10($v0)
        MEM_W(0X10, ctx->r2) = ctx->r4;
            goto L_80005940;
    }
    goto skip_1;
    // 0x80005928: sw          $a0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r4;
    skip_1:
    // 0x8000592C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80005930:
    // 0x80005930: lw          $v1, 0x10($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X10);
    // 0x80005934: bnel        $v1, $zero, L_80005930
    if (ctx->r3 != 0) {
        // 0x80005938: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80005930;
    }
    goto skip_2;
    // 0x80005938: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_2:
    // 0x8000593C: sw          $a0, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r4;
L_80005940:
    // 0x80005940: jr          $ra
    // 0x80005944: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x80005944: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
;}
RECOMP_FUNC void func_80005948(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005948: lw          $v0, 0x8($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X8);
    // 0x8000594C: beq         $v0, $zero, L_800059A0
    if (ctx->r2 == 0) {
        // 0x80005950: nop
    
            goto L_800059A0;
    }
    // 0x80005950: nop

    // 0x80005954: lw          $v1, 0x14($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X14);
    // 0x80005958: bnel        $a0, $v1, L_80005970
    if (ctx->r4 != ctx->r3) {
        // 0x8000595C: lw          $a1, 0x10($v1)
        ctx->r5 = MEM_W(ctx->r3, 0X10);
            goto L_80005970;
    }
    goto skip_0;
    // 0x8000595C: lw          $a1, 0x10($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X10);
    skip_0:
    // 0x80005960: lw          $t6, 0x10($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X10);
    // 0x80005964: b           L_80005994
    // 0x80005968: sw          $t6, 0x14($v0)
    MEM_W(0X14, ctx->r2) = ctx->r14;
        goto L_80005994;
    // 0x80005968: sw          $t6, 0x14($v0)
    MEM_W(0X14, ctx->r2) = ctx->r14;
    // 0x8000596C: lw          $a1, 0x10($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X10);
L_80005970:
    // 0x80005970: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80005974: beql        $a0, $a1, L_80005990
    if (ctx->r4 == ctx->r5) {
        // 0x80005978: lw          $t7, 0x10($a0)
        ctx->r15 = MEM_W(ctx->r4, 0X10);
            goto L_80005990;
    }
    goto skip_1;
    // 0x80005978: lw          $t7, 0x10($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X10);
    skip_1:
    // 0x8000597C: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
L_80005980:
    // 0x80005980: lw          $a1, 0x10($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X10);
    // 0x80005984: bnel        $a0, $a1, L_80005980
    if (ctx->r4 != ctx->r5) {
        // 0x80005988: or          $v0, $a1, $zero
        ctx->r2 = ctx->r5 | 0;
            goto L_80005980;
    }
    goto skip_2;
    // 0x80005988: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    skip_2:
    // 0x8000598C: lw          $t7, 0x10($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X10);
L_80005990:
    // 0x80005990: sw          $t7, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r15;
L_80005994:
    // 0x80005994: sw          $zero, 0x10($a0)
    MEM_W(0X10, ctx->r4) = 0;
    // 0x80005998: sw          $zero, 0xC($a0)
    MEM_W(0XC, ctx->r4) = 0;
    // 0x8000599C: sw          $zero, 0x8($a0)
    MEM_W(0X8, ctx->r4) = 0;
L_800059A0:
    // 0x800059A0: jr          $ra
    // 0x800059A4: nop

    return;
    // 0x800059A4: nop

;}
RECOMP_FUNC void func_800059A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800059A8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800059AC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800059B0: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x800059B4: jal         0x80005574
    // 0x800059B8: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_80005574(rdram, ctx);
        goto after_0;
    // 0x800059B8: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_0:
    // 0x800059BC: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x800059C0: beq         $v0, $zero, L_80005A1C
    if (ctx->r2 == 0) {
        // 0x800059C4: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80005A1C;
    }
    // 0x800059C4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x800059C8: beql        $a1, $zero, L_800059E0
    if (ctx->r5 == 0) {
        // 0x800059CC: lw          $t6, 0x20($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X20);
            goto L_800059E0;
    }
    goto skip_0;
    // 0x800059CC: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    skip_0:
    // 0x800059D0: jal         0x8000585C
    // 0x800059D4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_8000585C(rdram, ctx);
        goto after_1;
    // 0x800059D4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x800059D8: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x800059DC: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
L_800059E0:
    // 0x800059E0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x800059E4: andi        $t7, $t6, 0x19
    ctx->r15 = ctx->r14 & 0X19;
    // 0x800059E8: beql        $t7, $zero, L_80005A0C
    if (ctx->r15 == 0) {
        // 0x800059EC: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_80005A0C;
    }
    goto skip_1;
    // 0x800059EC: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_1:
    // 0x800059F0: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800059F4: lwc1        $f0, -0x3270($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X3270);
    // 0x800059F8: swc1        $f0, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->f0.u32l;
    // 0x800059FC: swc1        $f0, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f0.u32l;
    // 0x80005A00: b           L_80005A1C
    // 0x80005A04: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
        goto L_80005A1C;
    // 0x80005A04: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
    // 0x80005A08: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
L_80005A0C:
    // 0x80005A0C: nop

    // 0x80005A10: swc1        $f0, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->f0.u32l;
    // 0x80005A14: swc1        $f0, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f0.u32l;
    // 0x80005A18: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
L_80005A1C:
    // 0x80005A1C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005A20: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80005A24: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80005A28: jr          $ra
    // 0x80005A2C: nop

    return;
    // 0x80005A2C: nop

;}
RECOMP_FUNC void sceneLookup(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005A30: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80005A34: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80005A38: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80005A3C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80005A40: jal         0x80005574
    // 0x80005A44: ori         $a0, $a0, 0x1000
    ctx->r4 = ctx->r4 | 0X1000;
    func_80005574(rdram, ctx);
        goto after_0;
    // 0x80005A44: ori         $a0, $a0, 0x1000
    ctx->r4 = ctx->r4 | 0X1000;
    after_0:
    // 0x80005A48: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80005A4C: beq         $v0, $zero, L_80005AA8
    if (ctx->r2 == 0) {
        // 0x80005A50: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80005AA8;
    }
    // 0x80005A50: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80005A54: beql        $a1, $zero, L_80005A6C
    if (ctx->r5 == 0) {
        // 0x80005A58: lw          $t6, 0x20($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X20);
            goto L_80005A6C;
    }
    goto skip_0;
    // 0x80005A58: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    skip_0:
    // 0x80005A5C: jal         0x800058A4
    // 0x80005A60: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_800058A4(rdram, ctx);
        goto after_1;
    // 0x80005A60: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x80005A64: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80005A68: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
L_80005A6C:
    // 0x80005A6C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80005A70: andi        $t7, $t6, 0x19
    ctx->r15 = ctx->r14 & 0X19;
    // 0x80005A74: beql        $t7, $zero, L_80005A98
    if (ctx->r15 == 0) {
        // 0x80005A78: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_80005A98;
    }
    goto skip_1;
    // 0x80005A78: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_1:
    // 0x80005A7C: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80005A80: lwc1        $f0, -0x326C($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X326C);
    // 0x80005A84: swc1        $f0, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->f0.u32l;
    // 0x80005A88: swc1        $f0, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f0.u32l;
    // 0x80005A8C: b           L_80005AA8
    // 0x80005A90: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
        goto L_80005AA8;
    // 0x80005A90: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
    // 0x80005A94: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
L_80005A98:
    // 0x80005A98: nop

    // 0x80005A9C: swc1        $f0, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->f0.u32l;
    // 0x80005AA0: swc1        $f0, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f0.u32l;
    // 0x80005AA4: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
L_80005AA8:
    // 0x80005AA8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005AAC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80005AB0: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80005AB4: jr          $ra
    // 0x80005AB8: nop

    return;
    // 0x80005AB8: nop

;}
RECOMP_FUNC void func_80005ABC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005ABC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80005AC0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80005AC4: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80005AC8: jal         0x80005574
    // 0x80005ACC: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_80005574(rdram, ctx);
        goto after_0;
    // 0x80005ACC: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_0:
    // 0x80005AD0: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80005AD4: beq         $v0, $zero, L_80005B60
    if (ctx->r2 == 0) {
        // 0x80005AD8: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80005B60;
    }
    // 0x80005AD8: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80005ADC: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x80005AE0: ori         $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 | 0X8000;
    // 0x80005AE4: beq         $a1, $zero, L_80005B20
    if (ctx->r5 == 0) {
        // 0x80005AE8: sh          $t7, 0x2($v0)
        MEM_H(0X2, ctx->r2) = ctx->r15;
            goto L_80005B20;
    }
    // 0x80005AE8: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x80005AEC: jal         0x800058F8
    // 0x80005AF0: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    func_800058F8(rdram, ctx);
        goto after_1;
    // 0x80005AF0: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    after_1:
    // 0x80005AF4: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80005AF8: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80005AFC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80005B00: lhu         $t8, 0x2($a1)
    ctx->r24 = MEM_HU(ctx->r5, 0X2);
    // 0x80005B04: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80005B08: ori         $t9, $t8, 0x8000
    ctx->r25 = ctx->r24 | 0X8000;
    // 0x80005B0C: sh          $t9, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r25;
    // 0x80005B10: swc1        $f0, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->f0.u32l;
    // 0x80005B14: swc1        $f0, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->f0.u32l;
    // 0x80005B18: b           L_80005B60
    // 0x80005B1C: swc1        $f0, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->f0.u32l;
        goto L_80005B60;
    // 0x80005B1C: swc1        $f0, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->f0.u32l;
L_80005B20:
    // 0x80005B20: lw          $t0, 0x20($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X20);
    // 0x80005B24: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80005B28: andi        $t1, $t0, 0x19
    ctx->r9 = ctx->r8 & 0X19;
    // 0x80005B2C: beql        $t1, $zero, L_80005B50
    if (ctx->r9 == 0) {
        // 0x80005B30: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_80005B50;
    }
    goto skip_0;
    // 0x80005B30: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_0:
    // 0x80005B34: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80005B38: lwc1        $f0, -0x3268($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X3268);
    // 0x80005B3C: swc1        $f0, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f0.u32l;
    // 0x80005B40: swc1        $f0, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f0.u32l;
    // 0x80005B44: b           L_80005B60
    // 0x80005B48: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
        goto L_80005B60;
    // 0x80005B48: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x80005B4C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
L_80005B50:
    // 0x80005B50: nop

    // 0x80005B54: swc1        $f0, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f0.u32l;
    // 0x80005B58: swc1        $f0, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f0.u32l;
    // 0x80005B5C: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
L_80005B60:
    // 0x80005B60: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005B64: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80005B68: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80005B6C: jr          $ra
    // 0x80005B70: nop

    return;
    // 0x80005B70: nop

;}
RECOMP_FUNC void func_80005B74(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005B74: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80005B78: sw          $s2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r18;
    // 0x80005B7C: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80005B80: lui         $at, 0x4170
    ctx->r1 = S32(0X4170 << 16);
    // 0x80005B84: sw          $s3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r19;
    // 0x80005B88: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x80005B8C: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x80005B90: lui         $s2, 0x801D
    ctx->r18 = S32(0X801D << 16);
    // 0x80005B94: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x80005B98: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80005B9C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80005BA0: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80005BA4: addiu       $s2, $s2, -0x7D40
    ctx->r18 = ADD32(ctx->r18, -0X7D40);
    // 0x80005BA8: lui         $s3, 0x4000
    ctx->r19 = S32(0X4000 << 16);
L_80005BAC:
    // 0x80005BAC: lh          $a0, 0x0($s1)
    ctx->r4 = MEM_H(ctx->r17, 0X0);
    // 0x80005BB0: jal         0x80005ABC
    // 0x80005BB4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80005ABC(rdram, ctx);
        goto after_0;
    // 0x80005BB4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_0:
    // 0x80005BB8: beq         $v0, $zero, L_80005C80
    if (ctx->r2 == 0) {
        // 0x80005BBC: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80005C80;
    }
    // 0x80005BBC: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80005BC0: lw          $t6, 0x40($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X40);
    // 0x80005BC4: addiu       $v1, $s0, 0xC
    ctx->r3 = ADD32(ctx->r16, 0XC);
    // 0x80005BC8: sw          $t6, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r14;
    // 0x80005BCC: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x80005BD0: sw          $t7, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r15;
    // 0x80005BD4: lh          $t8, 0x4($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X4);
    // 0x80005BD8: or          $t1, $t7, $s3
    ctx->r9 = ctx->r15 | ctx->r19;
    // 0x80005BDC: andi        $t9, $t8, 0x2000
    ctx->r25 = ctx->r24 & 0X2000;
    // 0x80005BE0: beql        $t9, $zero, L_80005BF0
    if (ctx->r25 == 0) {
        // 0x80005BE4: lh          $t2, 0x6($s0)
        ctx->r10 = MEM_H(ctx->r16, 0X6);
            goto L_80005BF0;
    }
    goto skip_0;
    // 0x80005BE4: lh          $t2, 0x6($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X6);
    skip_0:
    // 0x80005BE8: sw          $t1, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r9;
    // 0x80005BEC: lh          $t2, 0x6($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X6);
L_80005BF0:
    // 0x80005BF0: mtc1        $t2, $f4
    ctx->f4.u32l = ctx->r10;
    // 0x80005BF4: nop

    // 0x80005BF8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80005BFC: div.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f20.fl);
    // 0x80005C00: swc1        $f8, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f8.u32l;
    // 0x80005C04: lh          $t3, 0x8($s0)
    ctx->r11 = MEM_H(ctx->r16, 0X8);
    // 0x80005C08: mtc1        $t3, $f10
    ctx->f10.u32l = ctx->r11;
    // 0x80005C0C: nop

    // 0x80005C10: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80005C14: div.s       $f18, $f16, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f18.fl = DIV_S(ctx->f16.fl, ctx->f20.fl);
    // 0x80005C18: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x80005C1C: lh          $t4, 0xA($s0)
    ctx->r12 = MEM_H(ctx->r16, 0XA);
    // 0x80005C20: mtc1        $t4, $f4
    ctx->f4.u32l = ctx->r12;
    // 0x80005C24: nop

    // 0x80005C28: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80005C2C: div.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f20.fl);
    // 0x80005C30: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    // 0x80005C34: lh          $t5, 0x4($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X4);
    // 0x80005C38: andi        $t6, $t5, 0x4000
    ctx->r14 = ctx->r13 & 0X4000;
    // 0x80005C3C: beql        $t6, $zero, L_80005C64
    if (ctx->r14 == 0) {
        // 0x80005C40: sw          $v1, 0x128($s2)
        MEM_W(0X128, ctx->r18) = ctx->r3;
            goto L_80005C64;
    }
    goto skip_1;
    // 0x80005C40: sw          $v1, 0x128($s2)
    MEM_W(0X128, ctx->r18) = ctx->r3;
    skip_1:
    // 0x80005C44: jal         0x80005B74
    // 0x80005C48: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    func_80005B74(rdram, ctx);
        goto after_1;
    // 0x80005C48: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    after_1:
    // 0x80005C4C: sw          $v0, 0x128($s2)
    MEM_W(0X128, ctx->r18) = ctx->r2;
    // 0x80005C50: bne         $v0, $zero, L_80005C64
    if (ctx->r2 != 0) {
        // 0x80005C54: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80005C64;
    }
    // 0x80005C54: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80005C58: b           L_80005C84
    // 0x80005C5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80005C84;
    // 0x80005C5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80005C60: sw          $v1, 0x128($s2)
    MEM_W(0X128, ctx->r18) = ctx->r3;
L_80005C64:
    // 0x80005C64: lh          $t8, 0x4($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X4);
    // 0x80005C68: bgez        $t8, L_80005C78
    if (SIGNED(ctx->r24) >= 0) {
        // 0x80005C6C: nop
    
            goto L_80005C78;
    }
    // 0x80005C6C: nop

    // 0x80005C70: b           L_80005C84
    // 0x80005C74: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
        goto L_80005C84;
    // 0x80005C74: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80005C78:
    // 0x80005C78: b           L_80005BAC
    // 0x80005C7C: or          $s0, $v1, $zero
    ctx->r16 = ctx->r3 | 0;
        goto L_80005BAC;
    // 0x80005C7C: or          $s0, $v1, $zero
    ctx->r16 = ctx->r3 | 0;
L_80005C80:
    // 0x80005C80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80005C84:
    // 0x80005C84: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80005C88: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80005C8C: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x80005C90: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x80005C94: lw          $s2, 0x24($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X24);
    // 0x80005C98: lw          $s3, 0x28($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X28);
    // 0x80005C9C: jr          $ra
    // 0x80005CA0: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80005CA0: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80005CA4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005CA4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80005CA8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80005CAC: jal         0x80005A30
    // 0x80005CB0: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    sceneLookup(rdram, ctx);
        goto after_0;
    // 0x80005CB0: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x80005CB4: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80005CB8: beq         $v0, $zero, L_80005D7C
    if (ctx->r2 == 0) {
        // 0x80005CBC: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80005D7C;
    }
    // 0x80005CBC: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80005CC0: lw          $t6, 0x0($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X0);
    // 0x80005CC4: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80005CC8: sw          $t6, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r14;
    // 0x80005CCC: lw          $t7, 0x4($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X4);
    // 0x80005CD0: sw          $t7, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r15;
    // 0x80005CD4: lh          $t8, 0x8($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X8);
    // 0x80005CD8: or          $t1, $t7, $at
    ctx->r9 = ctx->r15 | ctx->r1;
    // 0x80005CDC: andi        $t9, $t8, 0x2000
    ctx->r25 = ctx->r24 & 0X2000;
    // 0x80005CE0: beql        $t9, $zero, L_80005CF0
    if (ctx->r25 == 0) {
        // 0x80005CE4: lh          $t2, 0xA($a2)
        ctx->r10 = MEM_H(ctx->r6, 0XA);
            goto L_80005CF0;
    }
    goto skip_0;
    // 0x80005CE4: lh          $t2, 0xA($a2)
    ctx->r10 = MEM_H(ctx->r6, 0XA);
    skip_0:
    // 0x80005CE8: sw          $t1, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r9;
    // 0x80005CEC: lh          $t2, 0xA($a2)
    ctx->r10 = MEM_H(ctx->r6, 0XA);
L_80005CF0:
    // 0x80005CF0: lui         $at, 0x4170
    ctx->r1 = S32(0X4170 << 16);
    // 0x80005CF4: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80005CF8: mtc1        $t2, $f4
    ctx->f4.u32l = ctx->r10;
    // 0x80005CFC: addiu       $a3, $a2, 0x10
    ctx->r7 = ADD32(ctx->r6, 0X10);
    // 0x80005D00: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80005D04: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80005D08: swc1        $f8, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f8.u32l;
    // 0x80005D0C: lh          $t3, 0xC($a2)
    ctx->r11 = MEM_H(ctx->r6, 0XC);
    // 0x80005D10: mtc1        $t3, $f10
    ctx->f10.u32l = ctx->r11;
    // 0x80005D14: nop

    // 0x80005D18: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80005D1C: div.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x80005D20: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x80005D24: lh          $t4, 0xE($a2)
    ctx->r12 = MEM_H(ctx->r6, 0XE);
    // 0x80005D28: mtc1        $t4, $f4
    ctx->f4.u32l = ctx->r12;
    // 0x80005D2C: nop

    // 0x80005D30: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80005D34: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80005D38: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    // 0x80005D3C: lh          $a1, 0x8($a2)
    ctx->r5 = MEM_H(ctx->r6, 0X8);
    // 0x80005D40: andi        $a1, $a1, 0x4000
    ctx->r5 = ctx->r5 & 0X4000;
    // 0x80005D44: beq         $a1, $zero, L_80005D7C
    if (ctx->r5 == 0) {
        // 0x80005D48: or          $a1, $a3, $zero
        ctx->r5 = ctx->r7 | 0;
            goto L_80005D7C;
    }
    // 0x80005D48: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x80005D4C: jal         0x80005B74
    // 0x80005D50: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    func_80005B74(rdram, ctx);
        goto after_1;
    // 0x80005D50: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    after_1:
    // 0x80005D54: beq         $v0, $zero, L_80005D6C
    if (ctx->r2 == 0) {
        // 0x80005D58: lw          $a0, 0x1C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X1C);
            goto L_80005D6C;
    }
    // 0x80005D58: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80005D5C: lh          $t5, 0x0($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X0);
    // 0x80005D60: ori         $t6, $t5, 0x800
    ctx->r14 = ctx->r13 | 0X800;
    // 0x80005D64: b           L_80005D7C
    // 0x80005D68: sh          $t6, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r14;
        goto L_80005D7C;
    // 0x80005D68: sh          $t6, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r14;
L_80005D6C:
    // 0x80005D6C: jal         0x8000516C
    // 0x80005D70: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    object_dispatchChild(rdram, ctx);
        goto after_2;
    // 0x80005D70: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_2:
    // 0x80005D74: b           L_80005D80
    // 0x80005D78: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80005D80;
    // 0x80005D78: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80005D7C:
    // 0x80005D7C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
L_80005D80:
    // 0x80005D80: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005D84: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80005D88: jr          $ra
    // 0x80005D8C: nop

    return;
    // 0x80005D8C: nop

;}
RECOMP_FUNC void func_80005D90(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005D90: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80005D94: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80005D98: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80005D9C: ori         $a0, $a0, 0x1000
    ctx->r4 = ctx->r4 | 0X1000;
    // 0x80005DA0: jal         0x80005A30
    // 0x80005DA4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    sceneLookup(rdram, ctx);
        goto after_0;
    // 0x80005DA4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x80005DA8: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80005DAC: beq         $v0, $zero, L_80005EBC
    if (ctx->r2 == 0) {
        // 0x80005DB0: or          $a3, $v0, $zero
        ctx->r7 = ctx->r2 | 0;
            goto L_80005EBC;
    }
    // 0x80005DB0: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x80005DB4: lw          $t6, 0x0($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X0);
    // 0x80005DB8: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80005DBC: sw          $t6, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r14;
    // 0x80005DC0: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    // 0x80005DC4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x80005DC8: jal         0x80005ABC
    // 0x80005DCC: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_80005ABC(rdram, ctx);
        goto after_1;
    // 0x80005DCC: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x80005DD0: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80005DD4: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80005DD8: beq         $v0, $zero, L_80005EA8
    if (ctx->r2 == 0) {
        // 0x80005DDC: sw          $v0, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r2;
            goto L_80005EA8;
    }
    // 0x80005DDC: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80005DE0: lw          $t7, 0x0($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X0);
    // 0x80005DE4: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80005DE8: sw          $t7, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r15;
    // 0x80005DEC: lw          $t8, 0x4($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X4);
    // 0x80005DF0: sw          $t8, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r24;
    // 0x80005DF4: lh          $t9, 0x8($a2)
    ctx->r25 = MEM_H(ctx->r6, 0X8);
    // 0x80005DF8: or          $t2, $t8, $at
    ctx->r10 = ctx->r24 | ctx->r1;
    // 0x80005DFC: andi        $t0, $t9, 0x2000
    ctx->r8 = ctx->r25 & 0X2000;
    // 0x80005E00: beql        $t0, $zero, L_80005E10
    if (ctx->r8 == 0) {
        // 0x80005E04: lh          $t3, 0xA($a2)
        ctx->r11 = MEM_H(ctx->r6, 0XA);
            goto L_80005E10;
    }
    goto skip_0;
    // 0x80005E04: lh          $t3, 0xA($a2)
    ctx->r11 = MEM_H(ctx->r6, 0XA);
    skip_0:
    // 0x80005E08: sw          $t2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r10;
    // 0x80005E0C: lh          $t3, 0xA($a2)
    ctx->r11 = MEM_H(ctx->r6, 0XA);
L_80005E10:
    // 0x80005E10: lui         $at, 0x4170
    ctx->r1 = S32(0X4170 << 16);
    // 0x80005E14: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80005E18: mtc1        $t3, $f4
    ctx->f4.u32l = ctx->r11;
    // 0x80005E1C: addiu       $a1, $a2, 0x10
    ctx->r5 = ADD32(ctx->r6, 0X10);
    // 0x80005E20: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80005E24: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80005E28: swc1        $f8, 0x50($v0)
    MEM_W(0X50, ctx->r2) = ctx->f8.u32l;
    // 0x80005E2C: lh          $t4, 0xC($a2)
    ctx->r12 = MEM_H(ctx->r6, 0XC);
    // 0x80005E30: mtc1        $t4, $f10
    ctx->f10.u32l = ctx->r12;
    // 0x80005E34: nop

    // 0x80005E38: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80005E3C: div.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x80005E40: swc1        $f18, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f18.u32l;
    // 0x80005E44: lh          $t5, 0xE($a2)
    ctx->r13 = MEM_H(ctx->r6, 0XE);
    // 0x80005E48: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x80005E4C: nop

    // 0x80005E50: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80005E54: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80005E58: swc1        $f8, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f8.u32l;
    // 0x80005E5C: lh          $a0, 0x8($a2)
    ctx->r4 = MEM_H(ctx->r6, 0X8);
    // 0x80005E60: andi        $a0, $a0, 0x4000
    ctx->r4 = ctx->r4 & 0X4000;
    // 0x80005E64: beql        $a0, $zero, L_80005EC0
    if (ctx->r4 == 0) {
        // 0x80005E68: or          $v0, $a3, $zero
        ctx->r2 = ctx->r7 | 0;
            goto L_80005EC0;
    }
    goto skip_1;
    // 0x80005E68: or          $v0, $a3, $zero
    ctx->r2 = ctx->r7 | 0;
    skip_1:
    // 0x80005E6C: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80005E70: jal         0x80005B74
    // 0x80005E74: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    func_80005B74(rdram, ctx);
        goto after_2;
    // 0x80005E74: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_2:
    // 0x80005E78: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80005E7C: beq         $v0, $zero, L_80005E94
    if (ctx->r2 == 0) {
        // 0x80005E80: lw          $a3, 0x18($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X18);
            goto L_80005E94;
    }
    // 0x80005E80: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80005E84: lh          $t6, 0x0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X0);
    // 0x80005E88: ori         $t7, $t6, 0x800
    ctx->r15 = ctx->r14 | 0X800;
    // 0x80005E8C: b           L_80005EBC
    // 0x80005E90: sh          $t7, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r15;
        goto L_80005EBC;
    // 0x80005E90: sh          $t7, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r15;
L_80005E94:
    // 0x80005E94: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80005E98: jal         0x8000516C
    // 0x80005E9C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    object_dispatchChild(rdram, ctx);
        goto after_3;
    // 0x80005E9C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_3:
    // 0x80005EA0: b           L_80005EC0
    // 0x80005EA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80005EC0;
    // 0x80005EA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80005EA8:
    // 0x80005EA8: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80005EAC: jal         0x8000516C
    // 0x80005EB0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    object_dispatchChild(rdram, ctx);
        goto after_4;
    // 0x80005EB0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_4:
    // 0x80005EB4: b           L_80005EC0
    // 0x80005EB8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80005EC0;
    // 0x80005EB8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80005EBC:
    // 0x80005EBC: or          $v0, $a3, $zero
    ctx->r2 = ctx->r7 | 0;
L_80005EC0:
    // 0x80005EC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80005EC4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80005EC8: jr          $ra
    // 0x80005ECC: nop

    return;
    // 0x80005ECC: nop

;}
RECOMP_FUNC void func_80005ED0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005ED0: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80005ED4: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x80005ED8: beq         $v0, $zero, L_80005EFC
    if (ctx->r2 == 0) {
        // 0x80005EDC: nop
    
            goto L_80005EFC;
    }
    // 0x80005EDC: nop

    // 0x80005EE0: beq         $a1, $zero, L_80005EF4
    if (ctx->r5 == 0) {
        // 0x80005EE4: addiu       $at, $zero, -0x8000
        ctx->r1 = ADD32(0, -0X8000);
            goto L_80005EF4;
    }
    // 0x80005EE4: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x80005EE8: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x80005EEC: and         $t7, $t6, $a1
    ctx->r15 = ctx->r14 & ctx->r5;
    // 0x80005EF0: beq         $t7, $zero, L_80005EFC
    if (ctx->r15 == 0) {
        // 0x80005EF4: or          $t8, $v0, $at
        ctx->r24 = ctx->r2 | ctx->r1;
            goto L_80005EFC;
    }
L_80005EF4:
    // 0x80005EF4: or          $t8, $v0, $at
    ctx->r24 = ctx->r2 | ctx->r1;
    // 0x80005EF8: sh          $t8, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r24;
L_80005EFC:
    // 0x80005EFC: jr          $ra
    // 0x80005F00: nop

    return;
    // 0x80005F00: nop

;}
RECOMP_FUNC void func_80005F04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005F04: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80005F08: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80005F0C: lw          $v1, 0x1554($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1554);
    // 0x80005F10: lw          $v0, 0x1550($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1550);
    // 0x80005F14: addiu       $a1, $zero, -0x8000
    ctx->r5 = ADD32(0, -0X8000);
    // 0x80005F18: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80005F1C: beq         $at, $zero, L_80005F4C
    if (ctx->r1 == 0) {
        // 0x80005F20: nop
    
            goto L_80005F4C;
    }
    // 0x80005F20: nop

    // 0x80005F24: lh          $a0, 0x0($v0)
    ctx->r4 = MEM_H(ctx->r2, 0X0);
L_80005F28:
    // 0x80005F28: beq         $a0, $zero, L_80005F3C
    if (ctx->r4 == 0) {
        // 0x80005F2C: or          $t6, $a0, $a1
        ctx->r14 = ctx->r4 | ctx->r5;
            goto L_80005F3C;
    }
    // 0x80005F2C: or          $t6, $a0, $a1
    ctx->r14 = ctx->r4 | ctx->r5;
    // 0x80005F30: sh          $t6, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r14;
    // 0x80005F34: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80005F38: lw          $v1, 0x1554($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1554);
L_80005F3C:
    // 0x80005F3C: addiu       $v0, $v0, 0xB8
    ctx->r2 = ADD32(ctx->r2, 0XB8);
    // 0x80005F40: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80005F44: bnel        $at, $zero, L_80005F28
    if (ctx->r1 != 0) {
        // 0x80005F48: lh          $a0, 0x0($v0)
        ctx->r4 = MEM_H(ctx->r2, 0X0);
            goto L_80005F28;
    }
    goto skip_0;
    // 0x80005F48: lh          $a0, 0x0($v0)
    ctx->r4 = MEM_H(ctx->r2, 0X0);
    skip_0:
L_80005F4C:
    // 0x80005F4C: jr          $ra
    // 0x80005F50: nop

    return;
    // 0x80005F50: nop

;}
RECOMP_FUNC void func_80005F54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005F54: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80005F58: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x80005F5C: beq         $v0, $zero, L_80005F80
    if (ctx->r2 == 0) {
        // 0x80005F60: nop
    
            goto L_80005F80;
    }
    // 0x80005F60: nop

    // 0x80005F64: beq         $a1, $zero, L_80005F7C
    if (ctx->r5 == 0) {
        // 0x80005F68: andi        $t8, $v0, 0x7FFF
        ctx->r24 = ctx->r2 & 0X7FFF;
            goto L_80005F7C;
    }
    // 0x80005F68: andi        $t8, $v0, 0x7FFF
    ctx->r24 = ctx->r2 & 0X7FFF;
    // 0x80005F6C: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x80005F70: and         $t7, $t6, $a1
    ctx->r15 = ctx->r14 & ctx->r5;
    // 0x80005F74: beq         $t7, $zero, L_80005F80
    if (ctx->r15 == 0) {
        // 0x80005F78: nop
    
            goto L_80005F80;
    }
    // 0x80005F78: nop

L_80005F7C:
    // 0x80005F7C: sh          $t8, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r24;
L_80005F80:
    // 0x80005F80: jr          $ra
    // 0x80005F84: nop

    return;
    // 0x80005F84: nop

;}
RECOMP_FUNC void func_80005F88(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005F88: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80005F8C: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80005F90: lw          $v1, 0x1554($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1554);
    // 0x80005F94: lw          $v0, 0x1550($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1550);
    // 0x80005F98: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80005F9C: beq         $at, $zero, L_80005FCC
    if (ctx->r1 == 0) {
        // 0x80005FA0: nop
    
            goto L_80005FCC;
    }
    // 0x80005FA0: nop

    // 0x80005FA4: lh          $a0, 0x0($v0)
    ctx->r4 = MEM_H(ctx->r2, 0X0);
L_80005FA8:
    // 0x80005FA8: beq         $a0, $zero, L_80005FBC
    if (ctx->r4 == 0) {
        // 0x80005FAC: andi        $t6, $a0, 0x7FFF
        ctx->r14 = ctx->r4 & 0X7FFF;
            goto L_80005FBC;
    }
    // 0x80005FAC: andi        $t6, $a0, 0x7FFF
    ctx->r14 = ctx->r4 & 0X7FFF;
    // 0x80005FB0: sh          $t6, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r14;
    // 0x80005FB4: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80005FB8: lw          $v1, 0x1554($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1554);
L_80005FBC:
    // 0x80005FBC: addiu       $v0, $v0, 0xB8
    ctx->r2 = ADD32(ctx->r2, 0XB8);
    // 0x80005FC0: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80005FC4: bnel        $at, $zero, L_80005FA8
    if (ctx->r1 != 0) {
        // 0x80005FC8: lh          $a0, 0x0($v0)
        ctx->r4 = MEM_H(ctx->r2, 0X0);
            goto L_80005FA8;
    }
    goto skip_0;
    // 0x80005FC8: lh          $a0, 0x0($v0)
    ctx->r4 = MEM_H(ctx->r2, 0X0);
    skip_0:
L_80005FCC:
    // 0x80005FCC: jr          $ra
    // 0x80005FD0: nop

    return;
    // 0x80005FD0: nop

;}
RECOMP_FUNC void func_80005FD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80005FD4: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80005FD8: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x80005FDC: beq         $t6, $zero, L_80006008
    if (ctx->r14 == 0) {
        // 0x80005FE0: nop
    
            goto L_80006008;
    }
    // 0x80005FE0: nop

    // 0x80005FE4: beql        $a1, $zero, L_80006000
    if (ctx->r5 == 0) {
        // 0x80005FE8: lhu         $t9, 0x2($a0)
        ctx->r25 = MEM_HU(ctx->r4, 0X2);
            goto L_80006000;
    }
    goto skip_0;
    // 0x80005FE8: lhu         $t9, 0x2($a0)
    ctx->r25 = MEM_HU(ctx->r4, 0X2);
    skip_0:
    // 0x80005FEC: lhu         $t7, 0x2($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X2);
    // 0x80005FF0: and         $t8, $t7, $a1
    ctx->r24 = ctx->r15 & ctx->r5;
    // 0x80005FF4: beq         $t8, $zero, L_80006008
    if (ctx->r24 == 0) {
        // 0x80005FF8: nop
    
            goto L_80006008;
    }
    // 0x80005FF8: nop

    // 0x80005FFC: lhu         $t9, 0x2($a0)
    ctx->r25 = MEM_HU(ctx->r4, 0X2);
L_80006000:
    // 0x80006000: andi        $t0, $t9, 0xDFFF
    ctx->r8 = ctx->r25 & 0XDFFF;
    // 0x80006004: sh          $t0, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r8;
L_80006008:
    // 0x80006008: jr          $ra
    // 0x8000600C: nop

    return;
    // 0x8000600C: nop

;}
RECOMP_FUNC void func_80006010(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80006010: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80006014: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x80006018: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x8000601C: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x80006020: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80006024: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80006028: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x8000602C: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80006030: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80006034: addiu       $t6, $s0, 0x50
    ctx->r14 = ADD32(ctx->r16, 0X50);
    // 0x80006038: addiu       $t7, $s0, 0x68
    ctx->r15 = ADD32(ctx->r16, 0X68);
    // 0x8000603C: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    // 0x80006040: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x80006044: mfc1        $a3, $f14
    ctx->r7 = (int32_t)ctx->f14.u32l;
    // 0x80006048: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x8000604C: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x80006050: sw          $t7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r15;
    // 0x80006054: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x80006058: jal         0x80004078
    // 0x8000605C: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    func_80004078(rdram, ctx);
        goto after_0;
    // 0x8000605C: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x80006060: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x80006064: lh          $t8, -0x7CA0($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X7CA0);
    // 0x80006068: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x8000606C: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80006070: addu        $t0, $s0, $t9
    ctx->r8 = ADD32(ctx->r16, ctx->r25);
    // 0x80006074: jal         0x8000CCCC
    // 0x80006078: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    func_8000CCCC(rdram, ctx);
        goto after_1;
    // 0x80006078: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    after_1:
    // 0x8000607C: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
    // 0x80006080: ori         $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 | 0X2000;
    // 0x80006084: sh          $t2, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r10;
    // 0x80006088: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x8000608C: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x80006090: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80006094: jr          $ra
    // 0x80006098: nop

    return;
    // 0x80006098: nop

;}
RECOMP_FUNC void func_8000609C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000609C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x800060A0: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x800060A4: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x800060A8: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x800060AC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x800060B0: lwc1        $f4, 0x40($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X40);
    // 0x800060B4: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x800060B8: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x800060BC: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x800060C0: addiu       $t6, $s0, 0x50
    ctx->r14 = ADD32(ctx->r16, 0X50);
    // 0x800060C4: addiu       $t7, $s0, 0x68
    ctx->r15 = ADD32(ctx->r16, 0X68);
    // 0x800060C8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800060CC: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x800060D0: mfc1        $a3, $f14
    ctx->r7 = (int32_t)ctx->f14.u32l;
    // 0x800060D4: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800060D8: addiu       $a0, $a0, -0x7C58
    ctx->r4 = ADD32(ctx->r4, -0X7C58);
    // 0x800060DC: sw          $t7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r15;
    // 0x800060E0: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x800060E4: jal         0x80004078
    // 0x800060E8: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    func_80004078(rdram, ctx);
        goto after_0;
    // 0x800060E8: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x800060EC: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x800060F0: lh          $t8, -0x7CA0($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X7CA0);
    // 0x800060F4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800060F8: addiu       $a0, $a0, -0x7C58
    ctx->r4 = ADD32(ctx->r4, -0X7C58);
    // 0x800060FC: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80006100: addu        $t0, $s0, $t9
    ctx->r8 = ADD32(ctx->r16, ctx->r25);
    // 0x80006104: jal         0x8000CCCC
    // 0x80006108: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    func_8000CCCC(rdram, ctx);
        goto after_1;
    // 0x80006108: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    after_1:
    // 0x8000610C: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
    // 0x80006110: ori         $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 | 0X2000;
    // 0x80006114: sh          $t2, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r10;
    // 0x80006118: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x8000611C: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x80006120: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80006124: jr          $ra
    // 0x80006128: nop

    return;
    // 0x80006128: nop

;}
RECOMP_FUNC void func_8000612C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000612C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80006130: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x80006134: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80006138: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x8000613C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80006140: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80006144: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x80006148: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x8000614C: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80006150: addiu       $t6, $s0, 0x50
    ctx->r14 = ADD32(ctx->r16, 0X50);
    // 0x80006154: addiu       $t7, $s0, 0x68
    ctx->r15 = ADD32(ctx->r16, 0X68);
    // 0x80006158: addiu       $a0, $a0, 0x78
    ctx->r4 = ADD32(ctx->r4, 0X78);
    // 0x8000615C: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x80006160: mfc1        $a3, $f14
    ctx->r7 = (int32_t)ctx->f14.u32l;
    // 0x80006164: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80006168: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x8000616C: sw          $t7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r15;
    // 0x80006170: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x80006174: jal         0x800043BC
    // 0x80006178: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    func_800043BC(rdram, ctx);
        goto after_0;
    // 0x80006178: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x8000617C: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x80006180: lh          $t8, -0x7CA0($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X7CA0);
    // 0x80006184: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x80006188: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x8000618C: addu        $t0, $s0, $t9
    ctx->r8 = ADD32(ctx->r16, ctx->r25);
    // 0x80006190: jal         0x8000CCCC
    // 0x80006194: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    func_8000CCCC(rdram, ctx);
        goto after_1;
    // 0x80006194: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    after_1:
    // 0x80006198: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
    // 0x8000619C: ori         $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 | 0X2000;
    // 0x800061A0: sh          $t2, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r10;
    // 0x800061A4: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x800061A8: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x800061AC: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x800061B0: jr          $ra
    // 0x800061B4: nop

    return;
    // 0x800061B4: nop

;}
RECOMP_FUNC void func_800061B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800061B8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x800061BC: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x800061C0: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x800061C4: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x800061C8: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x800061CC: lwc1        $f4, 0x40($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X40);
    // 0x800061D0: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x800061D4: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x800061D8: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x800061DC: addiu       $t6, $s0, 0x50
    ctx->r14 = ADD32(ctx->r16, 0X50);
    // 0x800061E0: addiu       $t7, $s0, 0x68
    ctx->r15 = ADD32(ctx->r16, 0X68);
    // 0x800061E4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800061E8: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x800061EC: mfc1        $a3, $f14
    ctx->r7 = (int32_t)ctx->f14.u32l;
    // 0x800061F0: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800061F4: addiu       $a0, $a0, -0x7C58
    ctx->r4 = ADD32(ctx->r4, -0X7C58);
    // 0x800061F8: sw          $t7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r15;
    // 0x800061FC: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x80006200: jal         0x800043BC
    // 0x80006204: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    func_800043BC(rdram, ctx);
        goto after_0;
    // 0x80006204: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x80006208: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x8000620C: lh          $t8, -0x7CA0($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X7CA0);
    // 0x80006210: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80006214: addiu       $a0, $a0, -0x7C58
    ctx->r4 = ADD32(ctx->r4, -0X7C58);
    // 0x80006218: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x8000621C: addu        $t0, $s0, $t9
    ctx->r8 = ADD32(ctx->r16, ctx->r25);
    // 0x80006220: jal         0x8000CCCC
    // 0x80006224: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    func_8000CCCC(rdram, ctx);
        goto after_1;
    // 0x80006224: lw          $a1, 0x44($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X44);
    after_1:
    // 0x80006228: lhu         $t1, 0x2($s0)
    ctx->r9 = MEM_HU(ctx->r16, 0X2);
    // 0x8000622C: ori         $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 | 0X2000;
    // 0x80006230: sh          $t2, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r10;
    // 0x80006234: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80006238: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x8000623C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80006240: jr          $ra
    // 0x80006244: nop

    return;
    // 0x80006244: nop

;}
RECOMP_FUNC void func_80006248(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80006248: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x8000624C: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x80006250: lh          $t6, 0x24($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X24);
    // 0x80006254: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80006258: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8000625C: slti        $at, $t6, 0x3
    ctx->r1 = SIGNED(ctx->r14) < 0X3 ? 1 : 0;
    // 0x80006260: bne         $at, $zero, L_80006270
    if (ctx->r1 != 0) {
        // 0x80006264: or          $a3, $zero, $zero
        ctx->r7 = 0 | 0;
            goto L_80006270;
    }
    // 0x80006264: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x80006268: b           L_80006274
    // 0x8000626C: lh          $v0, 0x2C($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X2C);
        goto L_80006274;
    // 0x8000626C: lh          $v0, 0x2C($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X2C);
L_80006270:
    // 0x80006270: lh          $v0, 0x78($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X78);
L_80006274:
    // 0x80006274: sll         $t7, $v0, 1
    ctx->r15 = S32(ctx->r2 << 1);
    // 0x80006278: addu        $a2, $a1, $t7
    ctx->r6 = ADD32(ctx->r5, ctx->r15);
    // 0x8000627C: lh          $t8, 0x60($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X60);
    // 0x80006280: lh          $t9, 0x58($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X58);
    // 0x80006284: lh          $t7, 0x66($a2)
    ctx->r15 = MEM_H(ctx->r6, 0X66);
    // 0x80006288: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8000628C: addu        $t6, $t8, $t9
    ctx->r14 = ADD32(ctx->r24, ctx->r25);
    // 0x80006290: sh          $t6, 0x60($a2)
    MEM_H(0X60, ctx->r6) = ctx->r14;
    // 0x80006294: lh          $t8, 0x5A($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X5A);
    // 0x80006298: lh          $t6, 0x6C($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X6C);
    // 0x8000629C: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x800062A0: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x800062A4: sh          $t9, 0x66($a2)
    MEM_H(0X66, ctx->r6) = ctx->r25;
    // 0x800062A8: lh          $t7, 0x5C($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X5C);
    // 0x800062AC: lh          $t9, 0x72($a2)
    ctx->r25 = MEM_H(ctx->r6, 0X72);
    // 0x800062B0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x800062B4: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x800062B8: sh          $t8, 0x6C($a2)
    MEM_H(0X6C, ctx->r6) = ctx->r24;
    // 0x800062BC: lh          $t6, 0x5E($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X5E);
    // 0x800062C0: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
    // 0x800062C4: addiu       $t2, $t2, -0x7D40
    ctx->r10 = ADD32(ctx->r10, -0X7D40);
    // 0x800062C8: addu        $t7, $t9, $t6
    ctx->r15 = ADD32(ctx->r25, ctx->r14);
    // 0x800062CC: sh          $t7, 0x72($a2)
    MEM_H(0X72, ctx->r6) = ctx->r15;
    // 0x800062D0: sh          $zero, 0x58($a1)
    MEM_H(0X58, ctx->r5) = 0;
    // 0x800062D4: sh          $zero, 0x5A($a1)
    MEM_H(0X5A, ctx->r5) = 0;
    // 0x800062D8: sh          $zero, 0x5C($a1)
    MEM_H(0X5C, ctx->r5) = 0;
    // 0x800062DC: sh          $zero, 0x5E($a1)
    MEM_H(0X5E, ctx->r5) = 0;
    // 0x800062E0: addiu       $v0, $v0, -0x7D3A
    ctx->r2 = ADD32(ctx->r2, -0X7D3A);
L_800062E4:
    // 0x800062E4: lh          $t8, 0x60($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X60);
    // 0x800062E8: lh          $t9, 0x66($t2)
    ctx->r25 = MEM_H(ctx->r10, 0X66);
    // 0x800062EC: lh          $t6, 0x6C($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X6C);
    // 0x800062F0: lh          $t7, 0x72($t2)
    ctx->r15 = MEM_H(ctx->r10, 0X72);
    // 0x800062F4: addu        $t0, $t0, $t8
    ctx->r8 = ADD32(ctx->r8, ctx->r24);
    // 0x800062F8: addu        $a0, $a0, $t9
    ctx->r4 = ADD32(ctx->r4, ctx->r25);
    // 0x800062FC: addu        $a3, $a3, $t6
    ctx->r7 = ADD32(ctx->r7, ctx->r14);
    // 0x80006300: addu        $v1, $v1, $t7
    ctx->r3 = ADD32(ctx->r3, ctx->r15);
    // 0x80006304: addiu       $t2, $t2, 0x2
    ctx->r10 = ADD32(ctx->r10, 0X2);
    // 0x80006308: sll         $t0, $t0, 16
    ctx->r8 = S32(ctx->r8 << 16);
    // 0x8000630C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80006310: sll         $a3, $a3, 16
    ctx->r7 = S32(ctx->r7 << 16);
    // 0x80006314: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80006318: sra         $t0, $t0, 16
    ctx->r8 = S32(SIGNED(ctx->r8) >> 16);
    // 0x8000631C: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80006320: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x80006324: bne         $t2, $v0, L_800062E4
    if (ctx->r10 != ctx->r2) {
        // 0x80006328: sra         $v1, $v1, 16
        ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
            goto L_800062E4;
    }
    // 0x80006328: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8000632C: bne         $a0, $zero, L_80006338
    if (ctx->r4 != 0) {
        // 0x80006330: lui         $a2, 0x800D
        ctx->r6 = S32(0X800D << 16);
            goto L_80006338;
    }
    // 0x80006330: lui         $a2, 0x800D
    ctx->r6 = S32(0X800D << 16);
    // 0x80006334: beq         $v1, $zero, L_80006438
    if (ctx->r3 == 0) {
        // 0x80006338: addiu       $a2, $a2, -0x18A0
        ctx->r6 = ADD32(ctx->r6, -0X18A0);
            goto L_80006438;
    }
L_80006338:
    // 0x80006338: addiu       $a2, $a2, -0x18A0
    ctx->r6 = ADD32(ctx->r6, -0X18A0);
    // 0x8000633C: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80006340: lui         $t9, 0xE300
    ctx->r25 = S32(0XE300 << 16);
    // 0x80006344: ori         $t9, $t9, 0xA01
    ctx->r25 = ctx->r25 | 0XA01;
    // 0x80006348: addiu       $t8, $v0, 0x8
    ctx->r24 = ADD32(ctx->r2, 0X8);
    // 0x8000634C: sw          $t8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r24;
    // 0x80006350: lui         $t6, 0x30
    ctx->r14 = S32(0X30 << 16);
    // 0x80006354: sw          $t6, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r14;
    // 0x80006358: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
    // 0x8000635C: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80006360: lui         $t9, 0x1
    ctx->r25 = S32(0X1 << 16);
    // 0x80006364: ori         $t9, $t9, 0x1
    ctx->r25 = ctx->r25 | 0X1;
    // 0x80006368: addiu       $t7, $v0, 0x8
    ctx->r15 = ADD32(ctx->r2, 0X8);
    // 0x8000636C: sw          $t7, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r15;
    // 0x80006370: lui         $t8, 0xF700
    ctx->r24 = S32(0XF700 << 16);
    // 0x80006374: sw          $t8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r24;
    // 0x80006378: blez        $a0, L_800063D8
    if (SIGNED(ctx->r4) <= 0) {
        // 0x8000637C: sw          $t9, 0x4($v0)
        MEM_W(0X4, ctx->r2) = ctx->r25;
            goto L_800063D8;
    }
    // 0x8000637C: sw          $t9, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r25;
    // 0x80006380: lh          $t7, 0x54($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X54);
    // 0x80006384: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80006388: lui         $at, 0xF600
    ctx->r1 = S32(0XF600 << 16);
    // 0x8000638C: andi        $t8, $t7, 0x3FF
    ctx->r24 = ctx->r15 & 0X3FF;
    // 0x80006390: lh          $t7, 0x52($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X52);
    // 0x80006394: sll         $t9, $t8, 14
    ctx->r25 = S32(ctx->r24 << 14);
    // 0x80006398: addiu       $t6, $v0, 0x8
    ctx->r14 = ADD32(ctx->r2, 0X8);
    // 0x8000639C: sw          $t6, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r14;
    // 0x800063A0: or          $t6, $t9, $at
    ctx->r14 = ctx->r25 | ctx->r1;
    // 0x800063A4: addu        $t8, $t7, $a0
    ctx->r24 = ADD32(ctx->r15, ctx->r4);
    // 0x800063A8: andi        $t9, $t8, 0x3FF
    ctx->r25 = ctx->r24 & 0X3FF;
    // 0x800063AC: sll         $t7, $t9, 2
    ctx->r15 = S32(ctx->r25 << 2);
    // 0x800063B0: or          $t8, $t6, $t7
    ctx->r24 = ctx->r14 | ctx->r15;
    // 0x800063B4: sw          $t8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r24;
    // 0x800063B8: lh          $t9, 0x50($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X50);
    // 0x800063BC: lh          $t8, 0x52($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X52);
    // 0x800063C0: andi        $t6, $t9, 0x3FF
    ctx->r14 = ctx->r25 & 0X3FF;
    // 0x800063C4: sll         $t7, $t6, 14
    ctx->r15 = S32(ctx->r14 << 14);
    // 0x800063C8: andi        $t9, $t8, 0x3FF
    ctx->r25 = ctx->r24 & 0X3FF;
    // 0x800063CC: sll         $t6, $t9, 2
    ctx->r14 = S32(ctx->r25 << 2);
    // 0x800063D0: or          $t8, $t7, $t6
    ctx->r24 = ctx->r15 | ctx->r14;
    // 0x800063D4: sw          $t8, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r24;
L_800063D8:
    // 0x800063D8: bgezl       $v1, L_8000643C
    if (SIGNED(ctx->r3) >= 0) {
        // 0x800063DC: lh          $t9, 0x50($a1)
        ctx->r25 = MEM_H(ctx->r5, 0X50);
            goto L_8000643C;
    }
    goto skip_0;
    // 0x800063DC: lh          $t9, 0x50($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X50);
    skip_0:
    // 0x800063E0: lh          $t7, 0x54($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X54);
    // 0x800063E4: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x800063E8: lui         $at, 0xF600
    ctx->r1 = S32(0XF600 << 16);
    // 0x800063EC: andi        $t6, $t7, 0x3FF
    ctx->r14 = ctx->r15 & 0X3FF;
    // 0x800063F0: lh          $t7, 0x56($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X56);
    // 0x800063F4: sll         $t8, $t6, 14
    ctx->r24 = S32(ctx->r14 << 14);
    // 0x800063F8: addiu       $t9, $v0, 0x8
    ctx->r25 = ADD32(ctx->r2, 0X8);
    // 0x800063FC: sw          $t9, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r25;
    // 0x80006400: or          $t9, $t8, $at
    ctx->r25 = ctx->r24 | ctx->r1;
    // 0x80006404: andi        $t6, $t7, 0x3FF
    ctx->r14 = ctx->r15 & 0X3FF;
    // 0x80006408: sll         $t8, $t6, 2
    ctx->r24 = S32(ctx->r14 << 2);
    // 0x8000640C: or          $t7, $t9, $t8
    ctx->r15 = ctx->r25 | ctx->r24;
    // 0x80006410: sw          $t7, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r15;
    // 0x80006414: lh          $t6, 0x50($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X50);
    // 0x80006418: lh          $t7, 0x56($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X56);
    // 0x8000641C: andi        $t9, $t6, 0x3FF
    ctx->r25 = ctx->r14 & 0X3FF;
    // 0x80006420: sll         $t8, $t9, 14
    ctx->r24 = S32(ctx->r25 << 14);
    // 0x80006424: addu        $t6, $t7, $v1
    ctx->r14 = ADD32(ctx->r15, ctx->r3);
    // 0x80006428: andi        $t9, $t6, 0x3FF
    ctx->r25 = ctx->r14 & 0X3FF;
    // 0x8000642C: sll         $t7, $t9, 2
    ctx->r15 = S32(ctx->r25 << 2);
    // 0x80006430: or          $t6, $t8, $t7
    ctx->r14 = ctx->r24 | ctx->r15;
    // 0x80006434: sw          $t6, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r14;
L_80006438:
    // 0x80006438: lh          $t9, 0x50($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X50);
L_8000643C:
    // 0x8000643C: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x80006440: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80006444: addu        $t8, $t9, $t0
    ctx->r24 = ADD32(ctx->r25, ctx->r8);
    // 0x80006448: lh          $t9, 0x54($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X54);
    // 0x8000644C: sh          $t8, 0x50($a1)
    MEM_H(0X50, ctx->r5) = ctx->r24;
    // 0x80006450: lh          $t7, 0x52($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X52);
    // 0x80006454: addu        $t8, $t9, $a3
    ctx->r24 = ADD32(ctx->r25, ctx->r7);
    // 0x80006458: sh          $t8, 0x54($a1)
    MEM_H(0X54, ctx->r5) = ctx->r24;
    // 0x8000645C: lh          $t8, 0x50($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X50);
    // 0x80006460: addu        $t6, $t7, $a0
    ctx->r14 = ADD32(ctx->r15, ctx->r4);
    // 0x80006464: lh          $t7, 0x56($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X56);
    // 0x80006468: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x8000646C: lui         $a2, 0x800D
    ctx->r6 = S32(0X800D << 16);
    // 0x80006470: addiu       $a2, $a2, -0x18A0
    ctx->r6 = ADD32(ctx->r6, -0X18A0);
    // 0x80006474: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80006478: sh          $t6, 0x52($a1)
    MEM_H(0X52, ctx->r5) = ctx->r14;
    // 0x8000647C: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80006480: addu        $t6, $t7, $v1
    ctx->r14 = ADD32(ctx->r15, ctx->r3);
    // 0x80006484: sh          $t6, 0x56($a1)
    MEM_H(0X56, ctx->r5) = ctx->r14;
    // 0x80006488: addiu       $t9, $v0, 0x8
    ctx->r25 = ADD32(ctx->r2, 0X8);
    // 0x8000648C: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80006490: sw          $t9, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r25;
    // 0x80006494: lui         $at, 0xED00
    ctx->r1 = S32(0XED00 << 16);
    // 0x80006498: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8000649C: mfc1        $t6, $f10
    ctx->r14 = (int32_t)ctx->f10.u32l;
    // 0x800064A0: nop

    // 0x800064A4: andi        $t9, $t6, 0xFFF
    ctx->r25 = ctx->r14 & 0XFFF;
    // 0x800064A8: lh          $t6, 0x52($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X52);
    // 0x800064AC: sll         $t8, $t9, 12
    ctx->r24 = S32(ctx->r25 << 12);
    // 0x800064B0: or          $t7, $t8, $at
    ctx->r15 = ctx->r24 | ctx->r1;
    // 0x800064B4: mtc1        $t6, $f16
    ctx->f16.u32l = ctx->r14;
    // 0x800064B8: nop

    // 0x800064BC: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x800064C0: mul.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x800064C4: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x800064C8: mfc1        $t8, $f6
    ctx->r24 = (int32_t)ctx->f6.u32l;
    // 0x800064CC: nop

    // 0x800064D0: andi        $t6, $t8, 0xFFF
    ctx->r14 = ctx->r24 & 0XFFF;
    // 0x800064D4: or          $t9, $t7, $t6
    ctx->r25 = ctx->r15 | ctx->r14;
    // 0x800064D8: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
    // 0x800064DC: lh          $t8, 0x54($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X54);
    // 0x800064E0: lh          $t7, 0x56($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X56);
    // 0x800064E4: mtc1        $t8, $f8
    ctx->f8.u32l = ctx->r24;
    // 0x800064E8: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x800064EC: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x800064F0: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800064F4: mul.s       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x800064F8: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x800064FC: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80006500: mfc1        $t6, $f18
    ctx->r14 = (int32_t)ctx->f18.u32l;
    // 0x80006504: nop

    // 0x80006508: andi        $t9, $t6, 0xFFF
    ctx->r25 = ctx->r14 & 0XFFF;
    // 0x8000650C: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x80006510: sll         $t8, $t9, 12
    ctx->r24 = S32(ctx->r25 << 12);
    // 0x80006514: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    // 0x80006518: nop

    // 0x8000651C: andi        $t7, $t9, 0xFFF
    ctx->r15 = ctx->r25 & 0XFFF;
    // 0x80006520: or          $t6, $t8, $t7
    ctx->r14 = ctx->r24 | ctx->r15;
    // 0x80006524: sw          $t6, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r14;
    // 0x80006528: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x8000652C: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x80006530: lui         $t8, 0xDC08
    ctx->r24 = S32(0XDC08 << 16);
    // 0x80006534: addiu       $t9, $v0, 0x8
    ctx->r25 = ADD32(ctx->r2, 0X8);
    // 0x80006538: sw          $t9, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r25;
    // 0x8000653C: ori         $t8, $t8, 0xA
    ctx->r24 = ctx->r24 | 0XA;
    // 0x80006540: addiu       $t7, $t7, 0x1660
    ctx->r15 = ADD32(ctx->r15, 0X1660);
    // 0x80006544: sw          $t7, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r15;
    // 0x80006548: sw          $t8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r24;
    // 0x8000654C: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80006550: lui         $t8, 0x800C
    ctx->r24 = S32(0X800C << 16);
    // 0x80006554: lui         $t9, 0xDC08
    ctx->r25 = S32(0XDC08 << 16);
    // 0x80006558: addiu       $t6, $v0, 0x8
    ctx->r14 = ADD32(ctx->r2, 0X8);
    // 0x8000655C: sw          $t6, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r14;
    // 0x80006560: ori         $t9, $t9, 0x30A
    ctx->r25 = ctx->r25 | 0X30A;
    // 0x80006564: addiu       $t8, $t8, 0x1670
    ctx->r24 = ADD32(ctx->r24, 0X1670);
    // 0x80006568: sw          $t8, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r24;
    // 0x8000656C: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
    // 0x80006570: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80006574: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80006578: addiu       $t9, $t9, -0x16A8
    ctx->r25 = ADD32(ctx->r25, -0X16A8);
    // 0x8000657C: addiu       $t7, $v0, 0x8
    ctx->r15 = ADD32(ctx->r2, 0X8);
    // 0x80006580: sw          $t7, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r15;
    // 0x80006584: lui         $t6, 0xDE00
    ctx->r14 = S32(0XDE00 << 16);
    // 0x80006588: sw          $t6, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r14;
    // 0x8000658C: sw          $t9, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r25;
    // 0x80006590: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x80006594: lui         $t6, 0x1900
    ctx->r14 = S32(0X1900 << 16);
    // 0x80006598: ori         $t6, $t6, 0xE800
    ctx->r14 = ctx->r14 | 0XE800;
    // 0x8000659C: addiu       $t8, $v0, 0x8
    ctx->r24 = ADD32(ctx->r2, 0X8);
    // 0x800065A0: sw          $t8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r24;
    // 0x800065A4: lui         $t7, 0xDB08
    ctx->r15 = S32(0XDB08 << 16);
    // 0x800065A8: sw          $t7, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r15;
    // 0x800065AC: sw          $t6, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r14;
    // 0x800065B0: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x800065B4: lui         $t8, 0xF800
    ctx->r24 = S32(0XF800 << 16);
    // 0x800065B8: addiu       $t6, $zero, 0xFF
    ctx->r14 = ADD32(0, 0XFF);
    // 0x800065BC: addiu       $t9, $v0, 0x8
    ctx->r25 = ADD32(ctx->r2, 0X8);
    // 0x800065C0: sw          $t9, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r25;
    // 0x800065C4: sw          $v0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r2;
    // 0x800065C8: sw          $t8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r24;
    // 0x800065CC: lw          $t9, 0x0($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X0);
    // 0x800065D0: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x800065D4: jr          $ra
    // 0x800065D8: sw          $t6, 0x4($t9)
    MEM_W(0X4, ctx->r25) = ctx->r14;
    return;
    // 0x800065D8: sw          $t6, 0x4($t9)
    MEM_W(0X4, ctx->r25) = ctx->r14;
;}
