#include "recomp.h"
#include "funcs.h"
#include <stdio.h>

RECOMP_FUNC void func_800100FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800100FC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80010100: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010104: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80010108: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x8001010C: lw          $t6, 0x40($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X40);
    // 0x80010110: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x80010114: lw          $t9, 0x3C($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X3C);
    // 0x80010118: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x8001011C: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80010120: mtc1        $a3, $f12
    ctx->f12.u32l = ctx->r7;
    // 0x80010124: lw          $t8, -0x77D0($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X77D0);
    // 0x80010128: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x8001012C: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80010130: and         $t0, $t9, $at
    ctx->r8 = ctx->r25 & ctx->r1;
    // 0x80010134: addu        $t1, $t8, $t0
    ctx->r9 = ADD32(ctx->r24, ctx->r8);
    // 0x80010138: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x8001013C: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    // 0x80010140: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80010144: jal         0x8000FF88
    // 0x80010148: lbu         $a1, 0x2B($sp)
    ctx->r5 = MEM_BU(ctx->r29, 0X2B);
    func_8000FF88(rdram, ctx);
        goto after_0;
    // 0x80010148: lbu         $a1, 0x2B($sp)
    ctx->r5 = MEM_BU(ctx->r29, 0X2B);
    after_0:
    // 0x8001014C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80010150: sh          $zero, 0x8($a0)
    MEM_H(0X8, ctx->r4) = 0;
    // 0x80010154: lw          $t2, 0x1C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X1C);
    // 0x80010158: lbu         $t5, 0x0($a0)
    ctx->r13 = MEM_BU(ctx->r4, 0X0);
    // 0x8001015C: lhu         $t3, 0x6($t2)
    ctx->r11 = MEM_HU(ctx->r10, 0X6);
    // 0x80010160: ori         $t7, $t5, 0x2
    ctx->r15 = ctx->r13 | 0X2;
    // 0x80010164: ori         $t9, $t7, 0x2
    ctx->r25 = ctx->r15 | 0X2;
    // 0x80010168: sb          $t7, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r15;
    // 0x8001016C: ori         $t4, $t3, 0x4000
    ctx->r12 = ctx->r11 | 0X4000;
    // 0x80010170: sh          $t4, 0xA($a0)
    MEM_H(0XA, ctx->r4) = ctx->r12;
    // 0x80010174: sb          $t9, 0x0($a0)
    MEM_B(0X0, ctx->r4) = ctx->r25;
    // 0x80010178: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001017C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80010180: jr          $ra
    // 0x80010184: nop

    return;
    // 0x80010184: nop

;}
RECOMP_FUNC void func_80010188(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010188: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001018C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010190: lbu         $t6, 0x5($a0)
    ctx->r14 = MEM_BU(ctx->r4, 0X5);
    // 0x80010194: lbu         $v0, 0x4($a0)
    ctx->r2 = MEM_BU(ctx->r4, 0X4);
    // 0x80010198: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x8001019C: slt         $at, $v0, $t6
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r14) ? 1 : 0;
    // 0x800101A0: beq         $at, $zero, L_800101B0
    if (ctx->r1 == 0) {
        // 0x800101A4: addiu       $t7, $v0, 0x1
        ctx->r15 = ADD32(ctx->r2, 0X1);
            goto L_800101B0;
    }
    // 0x800101A4: addiu       $t7, $v0, 0x1
    ctx->r15 = ADD32(ctx->r2, 0X1);
    // 0x800101A8: b           L_800101B4
    // 0x800101AC: sb          $t7, 0x4($a0)
    MEM_B(0X4, ctx->r4) = ctx->r15;
        goto L_800101B4;
    // 0x800101AC: sb          $t7, 0x4($a0)
    MEM_B(0X4, ctx->r4) = ctx->r15;
L_800101B0:
    // 0x800101B0: sb          $t8, 0x4($a0)
    MEM_B(0X4, ctx->r4) = ctx->r24;
L_800101B4:
    // 0x800101B4: jal         0x800102D8
    // 0x800101B8: nop

    func_800102D8(rdram, ctx);
        goto after_0;
    // 0x800101B8: nop

    after_0:
    // 0x800101BC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800101C0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800101C4: jr          $ra
    // 0x800101C8: nop

    return;
    // 0x800101C8: nop

;}
RECOMP_FUNC void func_800101CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800101CC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800101D0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800101D4: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x800101D8: lbu         $t6, 0x5($a0)
    ctx->r14 = MEM_BU(ctx->r4, 0X5);
    // 0x800101DC: lbu         $v0, 0x4($a0)
    ctx->r2 = MEM_BU(ctx->r4, 0X4);
    // 0x800101E0: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x800101E4: slt         $at, $v0, $t6
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r14) ? 1 : 0;
    // 0x800101E8: beq         $at, $zero, L_800101F8
    if (ctx->r1 == 0) {
        // 0x800101EC: addiu       $t7, $v0, 0x1
        ctx->r15 = ADD32(ctx->r2, 0X1);
            goto L_800101F8;
    }
    // 0x800101EC: addiu       $t7, $v0, 0x1
    ctx->r15 = ADD32(ctx->r2, 0X1);
    // 0x800101F0: b           L_800101FC
    // 0x800101F4: sb          $t7, 0x4($a0)
    MEM_B(0X4, ctx->r4) = ctx->r15;
        goto L_800101FC;
    // 0x800101F4: sb          $t7, 0x4($a0)
    MEM_B(0X4, ctx->r4) = ctx->r15;
L_800101F8:
    // 0x800101F8: sb          $t8, 0x4($a0)
    MEM_B(0X4, ctx->r4) = ctx->r24;
L_800101FC:
    // 0x800101FC: jal         0x80010214
    // 0x80010200: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80010214(rdram, ctx);
        goto after_0;
    // 0x80010200: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80010204: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80010208: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001020C: jr          $ra
    // 0x80010210: nop

    return;
    // 0x80010210: nop

;}
RECOMP_FUNC void func_80010214(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010214: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80010218: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x8001021C: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80010220: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80010224: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80010228: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x8001022C: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80010230: andi        $s5, $a2, 0xFFFF
    ctx->r21 = ctx->r6 & 0XFFFF;
    // 0x80010234: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80010238: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8001023C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80010240: sw          $a2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r6;
    // 0x80010244: beq         $a1, $zero, L_800102B0
    if (ctx->r5 == 0) {
        // 0x80010248: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_800102B0;
    }
    // 0x80010248: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x8001024C: or          $s1, $s5, $zero
    ctx->r17 = ctx->r21 | 0;
    // 0x80010250: ori         $s4, $zero, 0x8000
    ctx->r20 = 0 | 0X8000;
L_80010254:
    // 0x80010254: beq         $s1, $zero, L_8001026C
    if (ctx->r17 == 0) {
        // 0x80010258: or          $a0, $s3, $zero
        ctx->r4 = ctx->r19 | 0;
            goto L_8001026C;
    }
    // 0x80010258: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x8001025C: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x80010260: and         $t7, $t6, $s1
    ctx->r15 = ctx->r14 & ctx->r17;
    // 0x80010264: beq         $t7, $zero, L_80010278
    if (ctx->r15 == 0) {
        // 0x80010268: nop
    
            goto L_80010278;
    }
    // 0x80010268: nop

L_8001026C:
    // 0x8001026C: jal         0x800102D8
    // 0x80010270: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_800102D8(rdram, ctx);
        goto after_0;
    // 0x80010270: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_0:
    // 0x80010274: or          $s2, $v0, $zero
    ctx->r18 = ctx->r2 | 0;
L_80010278:
    // 0x80010278: beq         $s2, $s4, L_800102B0
    if (ctx->r18 == ctx->r20) {
        // 0x8001027C: or          $a0, $s3, $zero
        ctx->r4 = ctx->r19 | 0;
            goto L_800102B0;
    }
    // 0x8001027C: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80010280: lw          $a1, 0x14($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X14);
    // 0x80010284: beql        $a1, $zero, L_80010298
    if (ctx->r5 == 0) {
        // 0x80010288: lh          $t8, 0x0($s0)
        ctx->r24 = MEM_H(ctx->r16, 0X0);
            goto L_80010298;
    }
    goto skip_0;
    // 0x80010288: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    skip_0:
    // 0x8001028C: jal         0x80010214
    // 0x80010290: andi        $a2, $s5, 0xFFFF
    ctx->r6 = ctx->r21 & 0XFFFF;
    func_80010214(rdram, ctx);
        goto after_1;
    // 0x80010290: andi        $a2, $s5, 0xFFFF
    ctx->r6 = ctx->r21 & 0XFFFF;
    after_1:
    // 0x80010294: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
L_80010298:
    // 0x80010298: andi        $t9, $t8, 0x800
    ctx->r25 = ctx->r24 & 0X800;
    // 0x8001029C: bnel        $t9, $zero, L_800102B4
    if (ctx->r25 != 0) {
        // 0x800102A0: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_800102B4;
    }
    goto skip_1;
    // 0x800102A0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_1:
    // 0x800102A4: lw          $s0, 0x10($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X10);
    // 0x800102A8: bne         $s0, $zero, L_80010254
    if (ctx->r16 != 0) {
        // 0x800102AC: nop
    
            goto L_80010254;
    }
    // 0x800102AC: nop

L_800102B0:
    // 0x800102B0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_800102B4:
    // 0x800102B4: or          $v0, $s2, $zero
    ctx->r2 = ctx->r18 | 0;
    // 0x800102B8: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x800102BC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x800102C0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x800102C4: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x800102C8: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x800102CC: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x800102D0: jr          $ra
    // 0x800102D4: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x800102D4: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_800102D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800102D8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x800102DC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800102E0: lbu         $t6, 0x5($a0)
    ctx->r14 = MEM_BU(ctx->r4, 0X5);
    // 0x800102E4: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x800102E8: mtc1        $at, $f3
    ctx->f_odd[(3 - 1) * 2] = ctx->r1;
    // 0x800102EC: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x800102F0: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x800102F4: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x800102F8: bgez        $t6, L_80010314
    if (SIGNED(ctx->r14) >= 0) {
        // 0x800102FC: cvt.d.w     $f6, $f4
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
            goto L_80010314;
    }
    // 0x800102FC: cvt.d.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
    // 0x80010300: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x80010304: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x80010308: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x8001030C: nop

    // 0x80010310: add.d       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f6.d = ctx->f6.d + ctx->f8.d;
L_80010314:
    // 0x80010314: lbu         $t7, 0x4($a0)
    ctx->r15 = MEM_BU(ctx->r4, 0X4);
    // 0x80010318: div.d       $f10, $f2, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f6.d); 
    ctx->f10.d = DIV_D(ctx->f2.d, ctx->f6.d);
    // 0x8001031C: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x80010320: mtc1        $t7, $f16
    ctx->f16.u32l = ctx->r15;
    // 0x80010324: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80010328: bgez        $t7, L_80010340
    if (SIGNED(ctx->r15) >= 0) {
        // 0x8001032C: cvt.d.w     $f18, $f16
        CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
            goto L_80010340;
    }
    // 0x8001032C: cvt.d.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.d = CVT_D_W(ctx->f16.u32l);
    // 0x80010330: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x80010334: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80010338: nop

    // 0x8001033C: add.d       $f18, $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f18.d = ctx->f18.d + ctx->f4.d;
L_80010340:
    // 0x80010340: mul.d       $f8, $f10, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f18.d); 
    ctx->f8.d = MUL_D(ctx->f10.d, ctx->f18.d);
    // 0x80010344: cvt.s.d     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f0.fl = CVT_S_D(ctx->f8.d);
    // 0x80010348: cvt.d.s     $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f6.d = CVT_D_S(ctx->f0.fl);
    // 0x8001034C: c.le.d      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.d <= ctx->f6.d;
    // 0x80010350: nop

    // 0x80010354: bc1fl       L_800103E8
    if (!c1cs) {
        // 0x80010358: lhu         $v1, 0x28($a3)
        ctx->r3 = MEM_HU(ctx->r7, 0X28);
            goto L_800103E8;
    }
    goto skip_0;
    // 0x80010358: lhu         $v1, 0x28($a3)
    ctx->r3 = MEM_HU(ctx->r7, 0X28);
    skip_0:
    // 0x8001035C: lhu         $t8, 0x28($a3)
    ctx->r24 = MEM_HU(ctx->r7, 0X28);
    // 0x80010360: lhu         $t0, 0x2A($a3)
    ctx->r8 = MEM_HU(ctx->r7, 0X2A);
    // 0x80010364: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80010368: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x8001036C: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80010370: sh          $t9, 0x28($a3)
    MEM_H(0X28, ctx->r7) = ctx->r25;
    // 0x80010374: sh          $t1, 0x2A($a3)
    MEM_H(0X2A, ctx->r7) = ctx->r9;
    // 0x80010378: lhu         $v1, 0xA($a0)
    ctx->r3 = MEM_HU(ctx->r4, 0XA);
    // 0x8001037C: andi        $v0, $t9, 0xFFFF
    ctx->r2 = ctx->r25 & 0XFFFF;
    // 0x80010380: addiu       $at, $zero, -0x4001
    ctx->r1 = ADD32(0, -0X4001);
    // 0x80010384: andi        $t2, $v1, 0x2000
    ctx->r10 = ctx->r3 & 0X2000;
    // 0x80010388: beql        $t2, $zero, L_800103C0
    if (ctx->r10 == 0) {
        // 0x8001038C: lhu         $v0, 0x28($a3)
        ctx->r2 = MEM_HU(ctx->r7, 0X28);
            goto L_800103C0;
    }
    goto skip_1;
    // 0x8001038C: lhu         $v0, 0x28($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X28);
    skip_1:
    // 0x80010390: addiu       $at, $zero, -0x6001
    ctx->r1 = ADD32(0, -0X6001);
    // 0x80010394: and         $t3, $v1, $at
    ctx->r11 = ctx->r3 & ctx->r1;
    // 0x80010398: addiu       $t4, $t3, -0x1
    ctx->r12 = ADD32(ctx->r11, -0X1);
    // 0x8001039C: slt         $at, $v0, $t4
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r12) ? 1 : 0;
    // 0x800103A0: bne         $at, $zero, L_800103B4
    if (ctx->r1 != 0) {
        // 0x800103A4: nop
    
            goto L_800103B4;
    }
    // 0x800103A4: nop

    // 0x800103A8: sh          $zero, 0x2A($a3)
    MEM_H(0X2A, ctx->r7) = 0;
    // 0x800103AC: sh          $zero, 0x28($a3)
    MEM_H(0X28, ctx->r7) = 0;
    // 0x800103B0: andi        $v0, $zero, 0xFFFF
    ctx->r2 = 0 & 0XFFFF;
L_800103B4:
    // 0x800103B4: b           L_800103E8
    // 0x800103B8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_800103E8;
    // 0x800103B8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x800103BC: lhu         $v0, 0x28($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X28);
L_800103C0:
    // 0x800103C0: and         $t6, $v1, $at
    ctx->r14 = ctx->r3 & ctx->r1;
    // 0x800103C4: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x800103C8: slt         $at, $v0, $t7
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x800103CC: bne         $at, $zero, L_800103DC
    if (ctx->r1 != 0) {
        // 0x800103D0: nop
    
            goto L_800103DC;
    }
    // 0x800103D0: nop

    // 0x800103D4: b           L_800103E8
    // 0x800103D8: ori         $v1, $zero, 0x8000
    ctx->r3 = 0 | 0X8000;
        goto L_800103E8;
    // 0x800103D8: ori         $v1, $zero, 0x8000
    ctx->r3 = 0 | 0X8000;
L_800103DC:
    // 0x800103DC: b           L_800103E8
    // 0x800103E0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_800103E8;
    // 0x800103E0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x800103E4: lhu         $v1, 0x28($a3)
    ctx->r3 = MEM_HU(ctx->r7, 0X28);
L_800103E8:
    // 0x800103E8: lbu         $t8, 0x0($a0)
    ctx->r24 = MEM_BU(ctx->r4, 0X0);
    // 0x800103EC: addiu       $t0, $sp, 0x24
    ctx->r8 = ADD32(ctx->r29, 0X24);
    // 0x800103F0: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x800103F4: andi        $t9, $t8, 0x2
    ctx->r25 = ctx->r24 & 0X2;
    // 0x800103F8: beql        $t9, $zero, L_800104BC
    if (ctx->r25 == 0) {
        // 0x800103FC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_800104BC;
    }
    goto skip_2;
    // 0x800103FC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x80010400: lw          $t2, 0x18($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X18);
    // 0x80010404: cvt.d.s     $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f16.d = CVT_D_S(ctx->f0.fl);
    // 0x80010408: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x8001040C: sub.d       $f4, $f2, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f16.d); 
    ctx->f4.d = ctx->f2.d - ctx->f16.d;
    // 0x80010410: sw          $t2, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r10;
    // 0x80010414: ldc1        $f10, -0x31F0($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X31F0);
    // 0x80010418: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x8001041C: lui         $at, 0x41E0
    ctx->r1 = S32(0X41E0 << 16);
    // 0x80010420: mul.d       $f18, $f4, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = MUL_D(ctx->f4.d, ctx->f10.d);
    // 0x80010424: cfc1        $t3, $FpcCsr
    ctx->r11 = get_cop1_cs();
    // 0x80010428: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x8001042C: nop

    // 0x80010430: cvt.w.d     $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    ctx->f8.u32l = CVT_W_D(ctx->f18.d);
    // 0x80010434: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x80010438: nop

    // 0x8001043C: andi        $t4, $t4, 0x78
    ctx->r12 = ctx->r12 & 0X78;
    // 0x80010440: beql        $t4, $zero, L_80010494
    if (ctx->r12 == 0) {
        // 0x80010444: mfc1        $t4, $f8
        ctx->r12 = (int32_t)ctx->f8.u32l;
            goto L_80010494;
    }
    goto skip_3;
    // 0x80010444: mfc1        $t4, $f8
    ctx->r12 = (int32_t)ctx->f8.u32l;
    skip_3:
    // 0x80010448: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x8001044C: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x80010450: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x80010454: sub.d       $f8, $f18, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f8.d); 
    ctx->f8.d = ctx->f18.d - ctx->f8.d;
    // 0x80010458: ctc1        $t4, $FpcCsr
    set_cop1_cs(ctx->r12);
    // 0x8001045C: nop

    // 0x80010460: cvt.w.d     $f8, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    ctx->f8.u32l = CVT_W_D(ctx->f8.d);
    // 0x80010464: cfc1        $t4, $FpcCsr
    ctx->r12 = get_cop1_cs();
    // 0x80010468: nop

    // 0x8001046C: andi        $t4, $t4, 0x78
    ctx->r12 = ctx->r12 & 0X78;
    // 0x80010470: bne         $t4, $zero, L_80010488
    if (ctx->r12 != 0) {
        // 0x80010474: nop
    
            goto L_80010488;
    }
    // 0x80010474: nop

    // 0x80010478: mfc1        $t4, $f8
    ctx->r12 = (int32_t)ctx->f8.u32l;
    // 0x8001047C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80010480: b           L_800104A0
    // 0x80010484: or          $t4, $t4, $at
    ctx->r12 = ctx->r12 | ctx->r1;
        goto L_800104A0;
    // 0x80010484: or          $t4, $t4, $at
    ctx->r12 = ctx->r12 | ctx->r1;
L_80010488:
    // 0x80010488: b           L_800104A0
    // 0x8001048C: addiu       $t4, $zero, -0x1
    ctx->r12 = ADD32(0, -0X1);
        goto L_800104A0;
    // 0x8001048C: addiu       $t4, $zero, -0x1
    ctx->r12 = ADD32(0, -0X1);
    // 0x80010490: mfc1        $t4, $f8
    ctx->r12 = (int32_t)ctx->f8.u32l;
L_80010494:
    // 0x80010494: nop

    // 0x80010498: bltz        $t4, L_80010488
    if (SIGNED(ctx->r12) < 0) {
        // 0x8001049C: nop
    
            goto L_80010488;
    }
    // 0x8001049C: nop

L_800104A0:
    // 0x800104A0: ctc1        $t3, $FpcCsr
    set_cop1_cs(ctx->r11);
    // 0x800104A4: sb          $t4, 0x27($sp)
    MEM_B(0X27, ctx->r29) = ctx->r12;
    // 0x800104A8: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x800104AC: jal         0x8000F560
    // 0x800104B0: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    func_8000F560(rdram, ctx);
        goto after_0;
    // 0x800104B0: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    after_0:
    // 0x800104B4: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x800104B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_800104BC:
    // 0x800104BC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x800104C0: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x800104C4: jr          $ra
    // 0x800104C8: nop

    return;
    // 0x800104C8: nop

;}
RECOMP_FUNC void func_800104D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800104D0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x800104D4: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x800104D8: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x800104DC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x800104E0: jr          $ra
    // 0x800104E4: sh          $a0, -0x7CB2($at)
    MEM_H(-0X7CB2, ctx->r1) = ctx->r4;
    return;
    // 0x800104E4: sh          $a0, -0x7CB2($at)
    MEM_H(-0X7CB2, ctx->r1) = ctx->r4;
;}
RECOMP_FUNC void func_800104E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800104E8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x800104EC: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x800104F0: lh          $t6, 0x8E($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X8E);
    // 0x800104F4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x800104F8: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x800104FC: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80010500: or          $t7, $t6, $a0
    ctx->r15 = ctx->r14 | ctx->r4;
    // 0x80010504: jr          $ra
    // 0x80010508: sh          $t7, 0x8E($v0)
    MEM_H(0X8E, ctx->r2) = ctx->r15;
    return;
    // 0x80010508: sh          $t7, 0x8E($v0)
    MEM_H(0X8E, ctx->r2) = ctx->r15;
;}
RECOMP_FUNC void func_8001050C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001050C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80010510: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80010514: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80010518: lh          $t6, 0x8E($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X8E);
    // 0x8001051C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80010520: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80010524: nor         $t7, $a0, $zero
    ctx->r15 = ~(ctx->r4 | 0);
    // 0x80010528: and         $t8, $t6, $t7
    ctx->r24 = ctx->r14 & ctx->r15;
    // 0x8001052C: jr          $ra
    // 0x80010530: sh          $t8, 0x8E($v0)
    MEM_H(0X8E, ctx->r2) = ctx->r24;
    return;
    // 0x80010530: sh          $t8, 0x8E($v0)
    MEM_H(0X8E, ctx->r2) = ctx->r24;
;}
RECOMP_FUNC void func_80010534(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010534: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80010538: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001053C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80010540: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80010544: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80010548: sb          $a0, 0x90($v0)
    MEM_B(0X90, ctx->r2) = ctx->r4;
    // 0x8001054C: sb          $a1, 0x91($v0)
    MEM_B(0X91, ctx->r2) = ctx->r5;
    // 0x80010550: jr          $ra
    // 0x80010554: sb          $a2, 0x92($v0)
    MEM_B(0X92, ctx->r2) = ctx->r6;
    return;
    // 0x80010554: sb          $a2, 0x92($v0)
    MEM_B(0X92, ctx->r2) = ctx->r6;
;}
RECOMP_FUNC void func_80010558(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010558: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8001055C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80010560: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80010564: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80010568: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001056C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80010570: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x80010574: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80010578: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    // 0x8001057C: andi        $t6, $a0, 0x8000
    ctx->r14 = ctx->r4 & 0X8000;
    // 0x80010580: andi        $a3, $a3, 0xFF
    ctx->r7 = ctx->r7 & 0XFF;
    // 0x80010584: andi        $a2, $a2, 0xFF
    ctx->r6 = ctx->r6 & 0XFF;
    // 0x80010588: sh          $a0, 0x8E($v0)
    MEM_H(0X8E, ctx->r2) = ctx->r4;
    // 0x8001058C: beq         $t6, $zero, L_800105A0
    if (ctx->r14 == 0) {
        // 0x80010590: sh          $a1, 0x96($v0)
        MEM_H(0X96, ctx->r2) = ctx->r5;
            goto L_800105A0;
    }
    // 0x80010590: sh          $a1, 0x96($v0)
    MEM_H(0X96, ctx->r2) = ctx->r5;
    // 0x80010594: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x80010598: b           L_800105A8
    // 0x8001059C: sh          $t7, 0x94($v0)
    MEM_H(0X94, ctx->r2) = ctx->r15;
        goto L_800105A8;
    // 0x8001059C: sh          $t7, 0x94($v0)
    MEM_H(0X94, ctx->r2) = ctx->r15;
L_800105A0:
    // 0x800105A0: addiu       $t8, $a1, -0x1
    ctx->r24 = ADD32(ctx->r5, -0X1);
    // 0x800105A4: sh          $t8, 0x94($v0)
    MEM_H(0X94, ctx->r2) = ctx->r24;
L_800105A8:
    // 0x800105A8: lbu         $t9, 0x13($sp)
    ctx->r25 = MEM_BU(ctx->r29, 0X13);
    // 0x800105AC: sb          $a2, 0x90($v0)
    MEM_B(0X90, ctx->r2) = ctx->r6;
    // 0x800105B0: sb          $a3, 0x91($v0)
    MEM_B(0X91, ctx->r2) = ctx->r7;
    // 0x800105B4: jr          $ra
    // 0x800105B8: sb          $t9, 0x92($v0)
    MEM_B(0X92, ctx->r2) = ctx->r25;
    return;
    // 0x800105B8: sb          $t9, 0x92($v0)
    MEM_B(0X92, ctx->r2) = ctx->r25;
;}
RECOMP_FUNC void func_800105BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800105BC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x800105C0: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x800105C4: lh          $v1, 0x8E($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X8E);
    // 0x800105C8: beq         $v1, $zero, L_800105F8
    if (ctx->r3 == 0) {
        // 0x800105CC: andi        $t6, $v1, 0x8000
        ctx->r14 = ctx->r3 & 0X8000;
            goto L_800105F8;
    }
    // 0x800105CC: andi        $t6, $v1, 0x8000
    ctx->r14 = ctx->r3 & 0X8000;
    // 0x800105D0: beq         $t6, $zero, L_800105F0
    if (ctx->r14 == 0) {
        // 0x800105D4: nop
    
            goto L_800105F0;
    }
    // 0x800105D4: nop

    // 0x800105D8: lhu         $t7, 0x96($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X96);
    // 0x800105DC: lhu         $t8, 0x94($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X94);
    // 0x800105E0: bne         $t7, $t8, L_800105F0
    if (ctx->r15 != ctx->r24) {
        // 0x800105E4: nop
    
            goto L_800105F0;
    }
    // 0x800105E4: nop

    // 0x800105E8: jr          $ra
    // 0x800105EC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x800105EC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800105F0:
    // 0x800105F0: jr          $ra
    // 0x800105F4: andi        $v0, $v1, 0xC000
    ctx->r2 = ctx->r3 & 0XC000;
    return;
    // 0x800105F4: andi        $v0, $v1, 0xC000
    ctx->r2 = ctx->r3 & 0XC000;
L_800105F8:
    // 0x800105F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x800105FC: jr          $ra
    // 0x80010600: nop

    return;
    // 0x80010600: nop

;}
RECOMP_FUNC void func_80010604(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010604: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x80010608: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x8001060C: lh          $v0, 0x8E($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X8E);
    // 0x80010610: beq         $v0, $zero, L_800107E0
    if (ctx->r2 == 0) {
        // 0x80010614: nop
    
            goto L_800107E0;
    }
    // 0x80010614: nop

    // 0x80010618: lhu         $v1, 0x94($a1)
    ctx->r3 = MEM_HU(ctx->r5, 0X94);
    // 0x8001061C: andi        $t6, $v0, 0x8000
    ctx->r14 = ctx->r2 & 0X8000;
    // 0x80010620: beq         $v1, $zero, L_800107E0
    if (ctx->r3 == 0) {
        // 0x80010624: nop
    
            goto L_800107E0;
    }
    // 0x80010624: nop

    // 0x80010628: mtc1        $v1, $f4
    ctx->f4.u32l = ctx->r3;
    // 0x8001062C: lhu         $a0, 0x96($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X96);
    // 0x80010630: bgez        $v1, L_80010648
    if (SIGNED(ctx->r3) >= 0) {
        // 0x80010634: cvt.s.w     $f6, $f4
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
            goto L_80010648;
    }
    // 0x80010634: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80010638: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x8001063C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80010640: nop

    // 0x80010644: add.s       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f8.fl;
L_80010648:
    // 0x80010648: mtc1        $a0, $f10
    ctx->f10.u32l = ctx->r4;
    // 0x8001064C: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x80010650: bgez        $a0, L_80010664
    if (SIGNED(ctx->r4) >= 0) {
        // 0x80010654: cvt.s.w     $f16, $f10
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
            goto L_80010664;
    }
    // 0x80010654: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80010658: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x8001065C: nop

    // 0x80010660: add.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl + ctx->f18.fl;
L_80010664:
    // 0x80010664: beq         $t6, $zero, L_80010680
    if (ctx->r14 == 0) {
        // 0x80010668: div.s       $f0, $f6, $f16
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f16.fl);
            goto L_80010680;
    }
    // 0x80010668: div.s       $f0, $f6, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f16.fl);
    // 0x8001066C: slt         $at, $v1, $a0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x80010670: beq         $at, $zero, L_80010694
    if (ctx->r1 == 0) {
        // 0x80010674: addiu       $t7, $v1, 0x1
        ctx->r15 = ADD32(ctx->r3, 0X1);
            goto L_80010694;
    }
    // 0x80010674: addiu       $t7, $v1, 0x1
    ctx->r15 = ADD32(ctx->r3, 0X1);
    // 0x80010678: b           L_80010694
    // 0x8001067C: sh          $t7, 0x94($a1)
    MEM_H(0X94, ctx->r5) = ctx->r15;
        goto L_80010694;
    // 0x8001067C: sh          $t7, 0x94($a1)
    MEM_H(0X94, ctx->r5) = ctx->r15;
L_80010680:
    // 0x80010680: addiu       $t8, $v1, -0x1
    ctx->r24 = ADD32(ctx->r3, -0X1);
    // 0x80010684: andi        $t9, $t8, 0xFFFF
    ctx->r25 = ctx->r24 & 0XFFFF;
    // 0x80010688: bne         $t9, $zero, L_80010694
    if (ctx->r25 != 0) {
        // 0x8001068C: sh          $t8, 0x94($a1)
        MEM_H(0X94, ctx->r5) = ctx->r24;
            goto L_80010694;
    }
    // 0x8001068C: sh          $t8, 0x94($a1)
    MEM_H(0X94, ctx->r5) = ctx->r24;
    // 0x80010690: sh          $zero, 0x8E($a1)
    MEM_H(0X8E, ctx->r5) = 0;
L_80010694:
    // 0x80010694: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80010698: ldc1        $f8, -0x31E0($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X31E0);
    // 0x8001069C: cvt.d.s     $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.d = CVT_D_S(ctx->f0.fl);
    // 0x800106A0: andi        $t2, $v0, 0x1
    ctx->r10 = ctx->r2 & 0X1;
    // 0x800106A4: mul.d       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f4.d, ctx->f8.d);
    // 0x800106A8: lui         $a2, 0x800D
    ctx->r6 = S32(0X800D << 16);
    // 0x800106AC: addiu       $a2, $a2, -0x18A0
    ctx->r6 = ADD32(ctx->r6, -0X18A0);
    // 0x800106B0: lui         $at, 0xF600
    ctx->r1 = S32(0XF600 << 16);
    // 0x800106B4: trunc.w.d   $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.u32l = TRUNC_W_D(ctx->f10.d);
    // 0x800106B8: mfc1        $t1, $f18
    ctx->r9 = (int32_t)ctx->f18.u32l;
    // 0x800106BC: beq         $t2, $zero, L_80010730
    if (ctx->r10 == 0) {
        // 0x800106C0: sb          $t1, 0x93($a1)
        MEM_B(0X93, ctx->r5) = ctx->r9;
            goto L_80010730;
    }
    // 0x800106C0: sb          $t1, 0x93($a1)
    MEM_B(0X93, ctx->r5) = ctx->r9;
    // 0x800106C4: lui         $a2, 0x800D
    ctx->r6 = S32(0X800D << 16);
    // 0x800106C8: addiu       $a2, $a2, -0x18A0
    ctx->r6 = ADD32(ctx->r6, -0X18A0);
    // 0x800106CC: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x800106D0: lui         $t4, 0xF800
    ctx->r12 = S32(0XF800 << 16);
    // 0x800106D4: lui         $t2, 0x800B
    ctx->r10 = S32(0X800B << 16);
    // 0x800106D8: addiu       $t3, $v1, 0x8
    ctx->r11 = ADD32(ctx->r3, 0X8);
    // 0x800106DC: sw          $t3, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r11;
    // 0x800106E0: sw          $t4, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r12;
    // 0x800106E4: lbu         $t9, 0x91($a1)
    ctx->r25 = MEM_BU(ctx->r5, 0X91);
    // 0x800106E8: lbu         $t6, 0x90($a1)
    ctx->r14 = MEM_BU(ctx->r5, 0X90);
    // 0x800106EC: lbu         $t3, 0x92($a1)
    ctx->r11 = MEM_BU(ctx->r5, 0X92);
    // 0x800106F0: lbu         $t8, 0x93($a1)
    ctx->r24 = MEM_BU(ctx->r5, 0X93);
    // 0x800106F4: sll         $t0, $t9, 16
    ctx->r8 = S32(ctx->r25 << 16);
    // 0x800106F8: sll         $t7, $t6, 24
    ctx->r15 = S32(ctx->r14 << 24);
    // 0x800106FC: or          $t1, $t7, $t0
    ctx->r9 = ctx->r15 | ctx->r8;
    // 0x80010700: sll         $t4, $t3, 8
    ctx->r12 = S32(ctx->r11 << 8);
    // 0x80010704: or          $t5, $t1, $t4
    ctx->r13 = ctx->r9 | ctx->r12;
    // 0x80010708: or          $t9, $t5, $t8
    ctx->r25 = ctx->r13 | ctx->r24;
    // 0x8001070C: sw          $t9, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r25;
    // 0x80010710: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x80010714: lui         $t0, 0xDE00
    ctx->r8 = S32(0XDE00 << 16);
    // 0x80010718: addiu       $t2, $t2, -0x15C8
    ctx->r10 = ADD32(ctx->r10, -0X15C8);
    // 0x8001071C: addiu       $t7, $v1, 0x8
    ctx->r15 = ADD32(ctx->r3, 0X8);
    // 0x80010720: sw          $t7, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r15;
    // 0x80010724: sw          $t2, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r10;
    // 0x80010728: b           L_80010790
    // 0x8001072C: sw          $t0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r8;
        goto L_80010790;
    // 0x8001072C: sw          $t0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r8;
L_80010730:
    // 0x80010730: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x80010734: lui         $t1, 0xFA00
    ctx->r9 = S32(0XFA00 << 16);
    // 0x80010738: lui         $t2, 0x800B
    ctx->r10 = S32(0X800B << 16);
    // 0x8001073C: addiu       $t3, $v1, 0x8
    ctx->r11 = ADD32(ctx->r3, 0X8);
    // 0x80010740: sw          $t3, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r11;
    // 0x80010744: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
    // 0x80010748: lbu         $t9, 0x91($a1)
    ctx->r25 = MEM_BU(ctx->r5, 0X91);
    // 0x8001074C: lbu         $t6, 0x90($a1)
    ctx->r14 = MEM_BU(ctx->r5, 0X90);
    // 0x80010750: lbu         $t3, 0x92($a1)
    ctx->r11 = MEM_BU(ctx->r5, 0X92);
    // 0x80010754: lbu         $t8, 0x93($a1)
    ctx->r24 = MEM_BU(ctx->r5, 0X93);
    // 0x80010758: sll         $t7, $t9, 16
    ctx->r15 = S32(ctx->r25 << 16);
    // 0x8001075C: sll         $t5, $t6, 24
    ctx->r13 = S32(ctx->r14 << 24);
    // 0x80010760: or          $t0, $t5, $t7
    ctx->r8 = ctx->r13 | ctx->r15;
    // 0x80010764: sll         $t1, $t3, 8
    ctx->r9 = S32(ctx->r11 << 8);
    // 0x80010768: or          $t4, $t0, $t1
    ctx->r12 = ctx->r8 | ctx->r9;
    // 0x8001076C: or          $t9, $t4, $t8
    ctx->r25 = ctx->r12 | ctx->r24;
    // 0x80010770: sw          $t9, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r25;
    // 0x80010774: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x80010778: lui         $t7, 0xDE00
    ctx->r15 = S32(0XDE00 << 16);
    // 0x8001077C: addiu       $t2, $t2, -0x1620
    ctx->r10 = ADD32(ctx->r10, -0X1620);
    // 0x80010780: addiu       $t5, $v1, 0x8
    ctx->r13 = ADD32(ctx->r3, 0X8);
    // 0x80010784: sw          $t5, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r13;
    // 0x80010788: sw          $t2, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r10;
    // 0x8001078C: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
L_80010790:
    // 0x80010790: lh          $t0, 0x54($a1)
    ctx->r8 = MEM_H(ctx->r5, 0X54);
    // 0x80010794: lh          $t8, 0x56($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X56);
    // 0x80010798: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x8001079C: andi        $t1, $t0, 0x3FF
    ctx->r9 = ctx->r8 & 0X3FF;
    // 0x800107A0: sll         $t6, $t1, 14
    ctx->r14 = S32(ctx->r9 << 14);
    // 0x800107A4: andi        $t9, $t8, 0x3FF
    ctx->r25 = ctx->r24 & 0X3FF;
    // 0x800107A8: sll         $t5, $t9, 2
    ctx->r13 = S32(ctx->r25 << 2);
    // 0x800107AC: or          $t4, $t6, $at
    ctx->r12 = ctx->r14 | ctx->r1;
    // 0x800107B0: addiu       $t3, $v1, 0x8
    ctx->r11 = ADD32(ctx->r3, 0X8);
    // 0x800107B4: sw          $t3, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r11;
    // 0x800107B8: or          $t7, $t4, $t5
    ctx->r15 = ctx->r12 | ctx->r13;
    // 0x800107BC: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x800107C0: lh          $t1, 0x52($a1)
    ctx->r9 = MEM_H(ctx->r5, 0X52);
    // 0x800107C4: lh          $t2, 0x50($a1)
    ctx->r10 = MEM_H(ctx->r5, 0X50);
    // 0x800107C8: andi        $t6, $t1, 0x3FF
    ctx->r14 = ctx->r9 & 0X3FF;
    // 0x800107CC: andi        $t3, $t2, 0x3FF
    ctx->r11 = ctx->r10 & 0X3FF;
    // 0x800107D0: sll         $t0, $t3, 14
    ctx->r8 = S32(ctx->r11 << 14);
    // 0x800107D4: sll         $t8, $t6, 2
    ctx->r24 = S32(ctx->r14 << 2);
    // 0x800107D8: or          $t9, $t0, $t8
    ctx->r25 = ctx->r8 | ctx->r24;
    // 0x800107DC: sw          $t9, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r25;
L_800107E0:
    // 0x800107E0: jr          $ra
    // 0x800107E4: nop

    return;
    // 0x800107E4: nop

;}
RECOMP_FUNC void func_800107F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800107F0: jr          $ra
    // 0x800107F4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x800107F4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_800107F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800107F8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800107FC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80010800: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010804: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010808: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8001080C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80010810: jalr        $t9
    // 0x80010814: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010814: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    after_0:
    // 0x80010818: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001081C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80010820: jr          $ra
    // 0x80010824: nop

    return;
    // 0x80010824: nop

;}
RECOMP_FUNC void func_80010828(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010828: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001082C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010830: lh          $t6, 0xE($a0)
    ctx->r14 = MEM_H(ctx->r4, 0XE);
    // 0x80010834: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80010838: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8001083C: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80010840: addu        $t8, $a0, $t7
    ctx->r24 = ADD32(ctx->r4, ctx->r15);
    // 0x80010844: lbu         $t9, 0x8($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0X8);
    // 0x80010848: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x8001084C: bne         $t9, $at, L_80010860
    if (ctx->r25 != ctx->r1) {
        // 0x80010850: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80010860;
    }
    // 0x80010850: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80010854: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010858: jalr        $t9
    // 0x8001085C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001085C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_80010860:
    // 0x80010860: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80010864: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80010868: jr          $ra
    // 0x8001086C: nop

    return;
    // 0x8001086C: nop

;}
RECOMP_FUNC void func_80010870(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010870: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80010874: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010878: lh          $t6, 0xE($a0)
    ctx->r14 = MEM_H(ctx->r4, 0XE);
    // 0x8001087C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80010880: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x80010884: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80010888: addu        $t8, $a0, $t7
    ctx->r24 = ADD32(ctx->r4, ctx->r15);
    // 0x8001088C: lbu         $t9, 0x8($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0X8);
    // 0x80010890: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80010894: bne         $t9, $at, L_800108A8
    if (ctx->r25 != ctx->r1) {
        // 0x80010898: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_800108A8;
    }
    // 0x80010898: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001089C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x800108A0: jalr        $t9
    // 0x800108A4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x800108A4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_800108A8:
    // 0x800108A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800108AC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800108B0: jr          $ra
    // 0x800108B4: nop

    return;
    // 0x800108B4: nop

;}
RECOMP_FUNC void func_800108B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800108B8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800108BC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800108C0: lh          $t6, 0xE($a0)
    ctx->r14 = MEM_H(ctx->r4, 0XE);
    // 0x800108C4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x800108C8: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x800108CC: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x800108D0: addu        $t8, $a0, $t7
    ctx->r24 = ADD32(ctx->r4, ctx->r15);
    // 0x800108D4: lbu         $t9, 0x8($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0X8);
    // 0x800108D8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x800108DC: bne         $t9, $at, L_800108F0
    if (ctx->r25 != ctx->r1) {
        // 0x800108E0: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_800108F0;
    }
    // 0x800108E0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x800108E4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x800108E8: jalr        $t9
    // 0x800108EC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x800108EC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_800108F0:
    // 0x800108F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800108F4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800108F8: jr          $ra
    // 0x800108FC: nop

    return;
    // 0x800108FC: nop

;}
RECOMP_FUNC void func_80010900(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010900: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80010904: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010908: lh          $t6, 0xE($a0)
    ctx->r14 = MEM_H(ctx->r4, 0XE);
    // 0x8001090C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80010910: addiu       $at, $zero, 0x5
    ctx->r1 = ADD32(0, 0X5);
    // 0x80010914: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80010918: addu        $t8, $a0, $t7
    ctx->r24 = ADD32(ctx->r4, ctx->r15);
    // 0x8001091C: lbu         $t9, 0x8($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0X8);
    // 0x80010920: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80010924: bne         $t9, $at, L_80010938
    if (ctx->r25 != ctx->r1) {
        // 0x80010928: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80010938;
    }
    // 0x80010928: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001092C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010930: jalr        $t9
    // 0x80010934: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010934: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_80010938:
    // 0x80010938: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001093C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80010940: jr          $ra
    // 0x80010944: nop

    return;
    // 0x80010944: nop

;}
RECOMP_FUNC void func_80010948(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010948: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001094C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80010950: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x80010954: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80010958: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001095C: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x80010960: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x80010964: addiu       $a1, $zero, 0x9
    ctx->r5 = ADD32(0, 0X9);
    // 0x80010968: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8001096C: jalr        $t9
    // 0x80010970: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010970: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x80010974: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80010978: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001097C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010980: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80010984: jalr        $t9
    // 0x80010988: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80010988: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x8001098C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80010990: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80010994: jr          $ra
    // 0x80010998: nop

    return;
    // 0x80010998: nop

;}
RECOMP_FUNC void func_8001099C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001099C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800109A0: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x800109A4: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x800109A8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800109AC: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x800109B0: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x800109B4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x800109B8: addiu       $a1, $zero, 0xF
    ctx->r5 = ADD32(0, 0XF);
    // 0x800109BC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x800109C0: jalr        $t9
    // 0x800109C4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x800109C4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x800109C8: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x800109CC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x800109D0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x800109D4: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x800109D8: jalr        $t9
    // 0x800109DC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x800109DC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x800109E0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800109E4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800109E8: jr          $ra
    // 0x800109EC: nop

    return;
    // 0x800109EC: nop

;}
RECOMP_FUNC void func_800109F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800109F0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800109F4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x800109F8: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x800109FC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80010A00: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80010A04: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x80010A08: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x80010A0C: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    // 0x80010A10: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80010A14: jalr        $t9
    // 0x80010A18: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010A18: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x80010A1C: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80010A20: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80010A24: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010A28: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80010A2C: jalr        $t9
    // 0x80010A30: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80010A30: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x80010A34: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80010A38: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80010A3C: jr          $ra
    // 0x80010A40: nop

    return;
    // 0x80010A40: nop

;}
RECOMP_FUNC void func_80010A44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010A44: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80010A48: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80010A4C: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x80010A50: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80010A54: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80010A58: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x80010A5C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x80010A60: addiu       $a1, $zero, 0x1E
    ctx->r5 = ADD32(0, 0X1E);
    // 0x80010A64: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80010A68: jalr        $t9
    // 0x80010A6C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010A6C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x80010A70: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80010A74: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80010A78: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010A7C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80010A80: jalr        $t9
    // 0x80010A84: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80010A84: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x80010A88: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80010A8C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80010A90: jr          $ra
    // 0x80010A94: nop

    return;
    // 0x80010A94: nop

;}
RECOMP_FUNC void func_80010A98(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010A98: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80010A9C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80010AA0: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x80010AA4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80010AA8: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80010AAC: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x80010AB0: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x80010AB4: addiu       $a1, $zero, 0x2D
    ctx->r5 = ADD32(0, 0X2D);
    // 0x80010AB8: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80010ABC: jalr        $t9
    // 0x80010AC0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010AC0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x80010AC4: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80010AC8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80010ACC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010AD0: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80010AD4: jalr        $t9
    // 0x80010AD8: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80010AD8: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x80010ADC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80010AE0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80010AE4: jr          $ra
    // 0x80010AE8: nop

    return;
    // 0x80010AE8: nop

;}
RECOMP_FUNC void func_80010AEC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010AEC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80010AF0: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80010AF4: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x80010AF8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80010AFC: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80010B00: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x80010B04: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x80010B08: addiu       $a1, $zero, 0x3C
    ctx->r5 = ADD32(0, 0X3C);
    // 0x80010B0C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80010B10: jalr        $t9
    // 0x80010B14: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010B14: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x80010B18: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80010B1C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80010B20: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010B24: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80010B28: jalr        $t9
    // 0x80010B2C: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80010B2C: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x80010B30: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80010B34: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80010B38: jr          $ra
    // 0x80010B3C: nop

    return;
    // 0x80010B3C: nop

;}
RECOMP_FUNC void func_80010B40(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010B40: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80010B44: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80010B48: addiu       $t9, $t9, 0x5BC
    ctx->r25 = ADD32(ctx->r25, 0X5BC);
    // 0x80010B4C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010B50: jalr        $t9
    // 0x80010B54: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010B54: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80010B58: bne         $v0, $zero, L_80010B74
    if (ctx->r2 != 0) {
        // 0x80010B5C: lw          $v1, 0x18($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X18);
            goto L_80010B74;
    }
    // 0x80010B5C: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80010B60: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80010B64: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80010B68: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x80010B6C: jalr        $t9
    // 0x80010B70: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80010B70: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    after_1:
L_80010B74:
    // 0x80010B74: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80010B78: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80010B7C: jr          $ra
    // 0x80010B80: nop

    return;
    // 0x80010B80: nop

;}
RECOMP_FUNC void func_80010B84(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010B84: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80010B88: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80010B8C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80010B90: addiu       $s1, $a0, 0x34
    ctx->r17 = ADD32(ctx->r4, 0X34);
    // 0x80010B94: addiu       $s4, $a0, 0x74
    ctx->r20 = ADD32(ctx->r4, 0X74);
    // 0x80010B98: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80010B9C: sltu        $at, $s1, $s4
    ctx->r1 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x80010BA0: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80010BA4: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80010BA8: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x80010BAC: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80010BB0: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80010BB4: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80010BB8: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80010BBC: beq         $at, $zero, L_80010C50
    if (ctx->r1 == 0) {
        // 0x80010BC0: sw          $s0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r16;
            goto L_80010C50;
    }
    // 0x80010BC0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80010BC4: lui         $s7, 0x800B
    ctx->r23 = S32(0X800B << 16);
    // 0x80010BC8: lui         $s6, 0x803F
    ctx->r22 = S32(0X803F << 16);
    // 0x80010BCC: ori         $s6, $s6, 0xFFFF
    ctx->r22 = ctx->r22 | 0XFFFF;
    // 0x80010BD0: addiu       $s7, $s7, -0x1570
    ctx->r23 = ADD32(ctx->r23, -0X1570);
    // 0x80010BD4: lui         $fp, 0x4000
    ctx->r30 = S32(0X4000 << 16);
    // 0x80010BD8: lui         $s5, 0x7FC0
    ctx->r21 = S32(0X7FC0 << 16);
L_80010BDC:
    // 0x80010BDC: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // DEBUG: Log child spec processing
    {
        static int log_count = 0;
        if (log_count < 50) {
            uint32_t slot = (uint32_t)((ctx->r17 - (ctx->r19 + 0x34)) / 4);
            fprintf(stderr, "[func_80010B84] #%d slot=%u raw=0x%08X\n",
                    log_count, slot, (uint32_t)ctx->r16);
            log_count++;
        }
    }
    // 0x80010BE0: beq         $s0, $zero, L_80010C50
    if (ctx->r16 == 0) {
        // 0x80010BE4: and         $s2, $s0, $s5
        ctx->r18 = ctx->r16 & ctx->r21;
            goto L_80010C50;
    }
    // 0x80010BE4: and         $s2, $s0, $s5
    ctx->r18 = ctx->r16 & ctx->r21;
    // 0x80010BE8: and         $t6, $s0, $s6
    ctx->r14 = ctx->r16 & ctx->r22;
    // 0x80010BEC: sw          $t6, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r14;
    // 0x80010BF0: blez        $t6, L_80010C3C
    if (SIGNED(ctx->r14) <= 0) {
        // 0x80010BF4: or          $s0, $t6, $zero
        ctx->r16 = ctx->r14 | 0;
            goto L_80010C3C;
    }
    // 0x80010BF4: or          $s0, $t6, $zero
    ctx->r16 = ctx->r14 | 0;
    // 0x80010BF8: andi        $v0, $t6, 0x7FF
    ctx->r2 = ctx->r14 & 0X7FF;
    // 0x80010BFC: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80010C00: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x80010C04: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x80010C08: addu        $t8, $s7, $t7
    ctx->r24 = ADD32(ctx->r23, ctx->r15);
    // 0x80010C0C: lw          $t9, -0x4($t8)
    ctx->r25 = MEM_W(ctx->r24, -0X4);
    // DEBUG: Log descriptor lookup result
    {
        static int desc_log = 0;
        if (desc_log < 30) {
            int16_t idx = (int16_t)(ctx->r2 & 0xFFFF); // sign-extended index
            uint32_t desc_addr = (uint32_t)(ctx->r24 - 4); // 0x800AEA8C + idx*4
            fprintf(stderr, "  -> index=%d desc_addr=0x%08X desc_value=0x%08X %s\n",
                    idx, desc_addr, (uint32_t)ctx->r25,
                    ctx->r25 == 0 ? "(will create obj)" : "(overlay exists, skip)");
            desc_log++;
        }
    }
    // 0x80010C10: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80010C14: beq         $t9, $zero, L_80010C24
    if (ctx->r25 == 0) {
        // 0x80010C18: nop
    
            goto L_80010C24;
    }
    // 0x80010C18: nop

    // 0x80010C1C: b           L_80010C3C
    // 0x80010C20: or          $s2, $s2, $fp
    ctx->r18 = ctx->r18 | ctx->r30;
        goto L_80010C3C;
    // 0x80010C20: or          $s2, $s2, $fp
    ctx->r18 = ctx->r18 | ctx->r30;
L_80010C24:
    // 0x80010C24: jal         0x80002410
    // 0x80010C28: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80002410(rdram, ctx);
        goto after_0;
    // 0x80010C28: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_0:
    // 0x80010C2C: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x80010C30: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80010C34: and         $s0, $v0, $at
    ctx->r16 = ctx->r2 & ctx->r1;
    // 0x80010C38: sw          $s0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r16;
L_80010C3C:
    // 0x80010C3C: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x80010C40: sltu        $at, $s1, $s4
    ctx->r1 = ctx->r17 < ctx->r20 ? 1 : 0;
    // 0x80010C44: or          $t1, $s0, $s2
    ctx->r9 = ctx->r16 | ctx->r18;
    // 0x80010C48: bne         $at, $zero, L_80010BDC
    if (ctx->r1 != 0) {
        // 0x80010C4C: sw          $t1, -0x4($s1)
        MEM_W(-0X4, ctx->r17) = ctx->r9;
            goto L_80010BDC;
    }
    // 0x80010C4C: sw          $t1, -0x4($s1)
    MEM_W(-0X4, ctx->r17) = ctx->r9;
L_80010C50:
    // 0x80010C50: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80010C54: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80010C58: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80010C5C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80010C60: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80010C64: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80010C68: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80010C6C: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80010C70: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80010C74: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80010C78: jr          $ra
    // 0x80010C7C: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x80010C7C: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_80010C80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010C80: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80010C84: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x80010C88: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80010C8C: addiu       $s0, $a0, 0x34
    ctx->r16 = ADD32(ctx->r4, 0X34);
    // 0x80010C90: addiu       $s7, $a0, 0x74
    ctx->r23 = ADD32(ctx->r4, 0X74);
    // 0x80010C94: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x80010C98: sltu        $at, $s0, $s7
    ctx->r1 = ctx->r16 < ctx->r23 ? 1 : 0;
    // 0x80010C9C: or          $s6, $a0, $zero
    ctx->r22 = ctx->r4 | 0;
    // 0x80010CA0: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80010CA4: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x80010CA8: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x80010CAC: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80010CB0: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80010CB4: beq         $at, $zero, L_80010D1C
    if (ctx->r1 == 0) {
        // 0x80010CB8: sw          $s1, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r17;
            goto L_80010D1C;
    }
    // 0x80010CB8: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80010CBC: lui         $s5, 0x7FFF
    ctx->r21 = S32(0X7FFF << 16);
    // 0x80010CC0: lui         $s3, 0x803F
    ctx->r19 = S32(0X803F << 16);
    // 0x80010CC4: ori         $s3, $s3, 0xFFFF
    ctx->r19 = ctx->r19 | 0XFFFF;
    // 0x80010CC8: ori         $s5, $s5, 0xFFFF
    ctx->r21 = ctx->r21 | 0XFFFF;
    // 0x80010CCC: lui         $s4, 0x4000
    ctx->r20 = S32(0X4000 << 16);
    // 0x80010CD0: lui         $s2, 0x7FC0
    ctx->r18 = S32(0X7FC0 << 16);
L_80010CD4:
    // 0x80010CD4: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80010CD8: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80010CDC: beq         $v0, $zero, L_80010D1C
    if (ctx->r2 == 0) {
        // 0x80010CE0: and         $s1, $v0, $s2
        ctx->r17 = ctx->r2 & ctx->r18;
            goto L_80010D1C;
    }
    // 0x80010CE0: and         $s1, $v0, $s2
    ctx->r17 = ctx->r2 & ctx->r18;
    // 0x80010CE4: and         $t6, $v0, $s3
    ctx->r14 = ctx->r2 & ctx->r19;
    // 0x80010CE8: and         $t7, $s1, $s4
    ctx->r15 = ctx->r17 & ctx->r20;
    // 0x80010CEC: beq         $t7, $zero, L_80010D04
    if (ctx->r15 == 0) {
        // 0x80010CF0: sw          $t6, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r14;
            goto L_80010D04;
    }
    // 0x80010CF0: sw          $t6, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r14;
    // 0x80010CF4: jal         0x80002410
    // 0x80010CF8: or          $a1, $t6, $zero
    ctx->r5 = ctx->r14 | 0;
    func_80002410(rdram, ctx);
        goto after_0;
    // 0x80010CF8: or          $a1, $t6, $zero
    ctx->r5 = ctx->r14 | 0;
    after_0:
    // 0x80010CFC: and         $t8, $v0, $s5
    ctx->r24 = ctx->r2 & ctx->r21;
    // 0x80010D00: sw          $t8, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r24;
L_80010D04:
    // 0x80010D04: lw          $t9, 0x0($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X0);
    // 0x80010D08: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80010D0C: sltu        $at, $s0, $s7
    ctx->r1 = ctx->r16 < ctx->r23 ? 1 : 0;
    // 0x80010D10: or          $t0, $t9, $s1
    ctx->r8 = ctx->r25 | ctx->r17;
    // 0x80010D14: bne         $at, $zero, L_80010CD4
    if (ctx->r1 != 0) {
        // 0x80010D18: sw          $t0, -0x4($s0)
        MEM_W(-0X4, ctx->r16) = ctx->r8;
            goto L_80010CD4;
    }
    // 0x80010D18: sw          $t0, -0x4($s0)
    MEM_W(-0X4, ctx->r16) = ctx->r8;
L_80010D1C:
    // 0x80010D1C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80010D20: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80010D24: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80010D28: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80010D2C: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80010D30: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x80010D34: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x80010D38: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x80010D3C: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x80010D40: jr          $ra
    // 0x80010D44: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x80010D44: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_80010D48(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010D48: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80010D4C: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80010D50: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80010D54: addiu       $s0, $a0, 0x34
    ctx->r16 = ADD32(ctx->r4, 0X34);
    // 0x80010D58: addiu       $s2, $a0, 0x74
    ctx->r18 = ADD32(ctx->r4, 0X74);
    // 0x80010D5C: sltu        $at, $s0, $s2
    ctx->r1 = ctx->r16 < ctx->r18 ? 1 : 0;
    // 0x80010D60: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80010D64: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80010D68: beq         $at, $zero, L_80010DB8
    if (ctx->r1 == 0) {
        // 0x80010D6C: sw          $s1, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r17;
            goto L_80010DB8;
    }
    // 0x80010D6C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80010D70: lui         $s1, 0x803F
    ctx->r17 = S32(0X803F << 16);
    // 0x80010D74: ori         $s1, $s1, 0xFFFF
    ctx->r17 = ctx->r17 | 0XFFFF;
    // 0x80010D78: lui         $s3, 0x8000
    ctx->r19 = S32(0X8000 << 16);
    // 0x80010D7C: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
L_80010D80:
    // 0x80010D80: beq         $v0, $zero, L_80010DB8
    if (ctx->r2 == 0) {
        // 0x80010D84: and         $t6, $v0, $s1
        ctx->r14 = ctx->r2 & ctx->r17;
            goto L_80010DB8;
    }
    // 0x80010D84: and         $t6, $v0, $s1
    ctx->r14 = ctx->r2 & ctx->r17;
    // 0x80010D88: bgez        $v0, L_80010DA0
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80010D8C: and         $t9, $v0, $s1
        ctx->r25 = ctx->r2 & ctx->r17;
            goto L_80010DA0;
    }
    // 0x80010D8C: and         $t9, $v0, $s1
    ctx->r25 = ctx->r2 & ctx->r17;
    // 0x80010D90: jalr        $t9
    // 0x80010D94: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010D94: nop

    after_0:
    // 0x80010D98: b           L_80010DAC
    // 0x80010D9C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
        goto L_80010DAC;
    // 0x80010D9C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80010DA0:
    // 0x80010DA0: jal         0x800029EC
    // 0x80010DA4: or          $a0, $t6, $s3
    ctx->r4 = ctx->r14 | ctx->r19;
    func_800029EC(rdram, ctx);
        goto after_1;
    // 0x80010DA4: or          $a0, $t6, $s3
    ctx->r4 = ctx->r14 | ctx->r19;
    after_1:
    // 0x80010DA8: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80010DAC:
    // 0x80010DAC: sltu        $at, $s0, $s2
    ctx->r1 = ctx->r16 < ctx->r18 ? 1 : 0;
    // 0x80010DB0: bnel        $at, $zero, L_80010D80
    if (ctx->r1 != 0) {
        // 0x80010DB4: lw          $v0, 0x0($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X0);
            goto L_80010D80;
    }
    goto skip_0;
    // 0x80010DB4: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    skip_0:
L_80010DB8:
    // 0x80010DB8: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80010DBC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80010DC0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80010DC4: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80010DC8: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80010DCC: jr          $ra
    // 0x80010DD0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80010DD0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80010DD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010DD4: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80010DD8: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x80010DDC: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80010DE0: addiu       $s0, $a0, 0x34
    ctx->r16 = ADD32(ctx->r4, 0X34);
    // 0x80010DE4: addiu       $s4, $a0, 0x74
    ctx->r20 = ADD32(ctx->r4, 0X74);
    // 0x80010DE8: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80010DEC: sltu        $at, $s0, $s4
    ctx->r1 = ctx->r16 < ctx->r20 ? 1 : 0;
    // 0x80010DF0: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x80010DF4: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80010DF8: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x80010DFC: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80010E00: beq         $at, $zero, L_80010E70
    if (ctx->r1 == 0) {
        // 0x80010E04: sw          $s2, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r18;
            goto L_80010E70;
    }
    // 0x80010E04: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80010E08: lui         $s2, 0x803F
    ctx->r18 = S32(0X803F << 16);
    // 0x80010E0C: ori         $s2, $s2, 0xFFFF
    ctx->r18 = ctx->r18 | 0XFFFF;
    // 0x80010E10: lui         $s5, 0x4000
    ctx->r21 = S32(0X4000 << 16);
    // 0x80010E14: lui         $s3, 0x8000
    ctx->r19 = S32(0X8000 << 16);
    // 0x80010E18: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
L_80010E1C:
    // 0x80010E1C: beq         $v0, $zero, L_80010E70
    if (ctx->r2 == 0) {
        // 0x80010E20: and         $t6, $v0, $s1
        ctx->r14 = ctx->r2 & ctx->r17;
            goto L_80010E70;
    }
    // 0x80010E20: and         $t6, $v0, $s1
    ctx->r14 = ctx->r2 & ctx->r17;
    // 0x80010E24: beql        $t6, $zero, L_80010E64
    if (ctx->r14 == 0) {
        // 0x80010E28: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80010E64;
    }
    goto skip_0;
    // 0x80010E28: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    skip_0:
    // 0x80010E2C: bgez        $v0, L_80010E48
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80010E30: and         $t7, $s1, $s3
        ctx->r15 = ctx->r17 & ctx->r19;
            goto L_80010E48;
    }
    // 0x80010E30: and         $t7, $s1, $s3
    ctx->r15 = ctx->r17 & ctx->r19;
    // 0x80010E34: and         $t9, $v0, $s2
    ctx->r25 = ctx->r2 & ctx->r18;
    // 0x80010E38: jalr        $t9
    // 0x80010E3C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80010E3C: nop

    after_0:
    // 0x80010E40: b           L_80010E64
    // 0x80010E44: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
        goto L_80010E64;
    // 0x80010E44: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80010E48:
    // 0x80010E48: beq         $t7, $zero, L_80010E54
    if (ctx->r15 == 0) {
        // 0x80010E4C: and         $t8, $v0, $s5
        ctx->r24 = ctx->r2 & ctx->r21;
            goto L_80010E54;
    }
    // 0x80010E4C: and         $t8, $v0, $s5
    ctx->r24 = ctx->r2 & ctx->r21;
    // 0x80010E50: bne         $t8, $zero, L_80010E60
    if (ctx->r24 != 0) {
        // 0x80010E54: and         $t0, $v0, $s2
        ctx->r8 = ctx->r2 & ctx->r18;
            goto L_80010E60;
    }
L_80010E54:
    // 0x80010E54: and         $t0, $v0, $s2
    ctx->r8 = ctx->r2 & ctx->r18;
    // 0x80010E58: jal         0x800029EC
    // 0x80010E5C: or          $a0, $t0, $s3
    ctx->r4 = ctx->r8 | ctx->r19;
    {
        static int dd4_dispatch = 0; dd4_dispatch++;
        if (dd4_dispatch <= 20 || (dd4_dispatch % 1000 == 0))
            fprintf(stderr, "[DD4-dispatch] #%d obj=0x%08X\n", dd4_dispatch, (uint32_t)ctx->r4);
    }
    func_800029EC(rdram, ctx);
        goto after_1;
    // 0x80010E5C: or          $a0, $t0, $s3
    ctx->r4 = ctx->r8 | ctx->r19;
    after_1:
L_80010E60:
    // 0x80010E60: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80010E64:
    // 0x80010E64: sltu        $at, $s0, $s4
    ctx->r1 = ctx->r16 < ctx->r20 ? 1 : 0;
    // 0x80010E68: bnel        $at, $zero, L_80010E1C
    if (ctx->r1 != 0) {
        // 0x80010E6C: lw          $v0, 0x0($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X0);
            goto L_80010E1C;
    }
    goto skip_1;
    // 0x80010E6C: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    skip_1:
L_80010E70:
    // 0x80010E70: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80010E74: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80010E78: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80010E7C: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80010E80: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80010E84: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x80010E88: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x80010E8C: jr          $ra
    // 0x80010E90: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80010E90: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80010EA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010EA0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80010EA4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010EA8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80010EAC: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80010EB0: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x80010EB4: lh          $v0, 0x0($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X0);
    // 0x80010EB8: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x80010EBC: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80010EC0: andi        $v0, $v0, 0x7FF
    ctx->r2 = ctx->r2 & 0X7FF;
    // 0x80010EC4: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80010EC8: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x80010ECC: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x80010ED0: addu        $a1, $a1, $t7
    ctx->r5 = ADD32(ctx->r5, ctx->r15);
    // 0x80010ED4: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    // 0x80010ED8: lw          $a1, -0x1574($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1574);
    // 0x80010EDC: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x80010EE0: jal         0x800119CC
    // 0x80010EE4: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_800119CC(rdram, ctx);
        goto after_0;
    // 0x80010EE4: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80010EE8: lw          $t8, 0x20($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X20);
    // 0x80010EEC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80010EF0: lw          $v0, 0x0($t8)
    ctx->r2 = MEM_W(ctx->r24, 0X0);
    // 0x80010EF4: jr          $ra
    // 0x80010EF8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80010EF8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_80010EFC(uint8_t* rdram, recomp_context* ctx) {
    // Let native overlay loading run. With the decompressed ROM, DMA reads
    // get pre-decompressed data. The LZKN64 decompressor sees flag=0x00 and
    // does a plain copy. Bit 0x2000 should be cleared by the completion path.
    {
        static int efc_log = 0; efc_log++;
        // Ensure overlay selector is set
        {
            gpr ovl_sel = S32(0x800F3F54);
            if ((int16_t)MEM_H(ovl_sel, 0) == 0)
                MEM_H(ovl_sel, 0) = (int16_t)1;
        }
        if (efc_log <= 10 || (efc_log % 200 == 0))
            fprintf(stderr, "[func_80010EFC] #%d NATIVE obj=0x%08X id=%u sel=%d load_mgr=0x%08X\n",
                    efc_log, (uint32_t)ctx->r5,
                    (uint16_t)MEM_HU(ctx->r5, 0x0) & 0x7FF,
                    (int16_t)MEM_H(S32(0x800F3F54), 0),
                    (uint32_t)MEM_W(S32(0x800C1600), 0));
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80010EFC: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80010F00: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80010F04: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80010F08: lh          $a3, 0x0($a1)
    ctx->r7 = MEM_H(ctx->r5, 0X0);
    // 0x80010F0C: lui         $t7, 0x800B
    ctx->r15 = S32(0X800B << 16);
    // 0x80010F10: andi        $a3, $a3, 0x7FF
    ctx->r7 = ctx->r7 & 0X7FF;
    // 0x80010F14: sll         $t6, $a3, 2
    ctx->r14 = S32(ctx->r7 << 2);
    // 0x80010F18: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80010F1C: lw          $t7, -0x1574($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X1574);
    // 0x80010F20: beql        $t7, $zero, L_80011050
    if (ctx->r15 == 0) {
        // 0x80010F24: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80011050;
    }
    goto skip_0;
    // 0x80010F24: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80010F28: lhu         $t8, 0x2($a1)
    ctx->r24 = MEM_HU(ctx->r5, 0X2);
    // 0x80010F2C: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80010F30: ori         $t9, $t8, 0x2000
    ctx->r25 = ctx->r24 | 0X2000;
    // 0x80010F34: sh          $t9, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r25;
    // 0x80010F38: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
    // 0x80010F3C: lw          $t1, 0x34($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X34);
    // 0x80010F40: beq         $t1, $zero, L_80010F6C
    if (ctx->r9 == 0) {
        // 0x80010F44: sw          $t1, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r9;
            goto L_80010F6C;
    }
    // 0x80010F44: sw          $t1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r9;
L_80010F48:
    // 0x80010F48: lw          $t2, 0x20($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X20);
    // 0x80010F4C: lw          $t5, 0x20($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X20);
    // 0x80010F50: lw          $t3, 0x4($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X4);
    // 0x80010F54: lw          $t4, 0x4($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X4);
    // 0x80010F58: beql        $a3, $t4, L_80010F70
    if (ctx->r7 == ctx->r12) {
        // 0x80010F5C: lw          $v0, 0x20($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X20);
            goto L_80010F70;
    }
    goto skip_1;
    // 0x80010F5C: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    skip_1:
    // 0x80010F60: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x80010F64: bne         $t6, $zero, L_80010F48
    if (ctx->r14 != 0) {
        // 0x80010F68: sw          $t6, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r14;
            goto L_80010F48;
    }
    // 0x80010F68: sw          $t6, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r14;
L_80010F6C:
    // 0x80010F6C: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
L_80010F70:
    // 0x80010F70: beql        $v0, $zero, L_80010F8C
    if (ctx->r2 == 0) {
        // 0x80010F74: sw          $a1, 0x34($sp)
        MEM_W(0X34, ctx->r29) = ctx->r5;
            goto L_80010F8C;
    }
    goto skip_2;
    // 0x80010F74: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    skip_2:
    // 0x80010F78: jal         0x800019E8
    // 0x80010F7C: addiu       $a0, $v0, 0x4
    ctx->r4 = ADD32(ctx->r2, 0X4);
    func_800019E8(rdram, ctx);
        goto after_0;
    // 0x80010F7C: addiu       $a0, $v0, 0x4
    ctx->r4 = ADD32(ctx->r2, 0X4);
    after_0:
    // 0x80010F80: b           L_80011050
    // 0x80010F84: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80011050;
    // 0x80010F84: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80010F88: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
L_80010F8C:
    // 0x80010F8C: jal         0x80001968
    // 0x80010F90: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    func_80001968(rdram, ctx);
        goto after_1;
    // 0x80010F90: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_1:
    // 0x80010F94: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x80010F98: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    // 0x80010F9C: jal         0x800019B0
    // 0x80010FA0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_800019B0(rdram, ctx);
        goto after_2;
    // 0x80010FA0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_2:
    // 0x80010FA4: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x80010FA8: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80010FAC: sw          $zero, 0x1604($at)
    MEM_W(0X1604, ctx->r1) = 0;
    // 0x80010FB0: addiu       $a2, $a2, 0x4
    ctx->r6 = ADD32(ctx->r6, 0X4);
    // 0x80010FB4: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    // 0x80010FB8: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x80010FBC: jal         0x80010EA0
    // 0x80010FC0: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    func_80010EA0(rdram, ctx);
        goto after_3;
    // 0x80010FC0: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    after_3:
    // 0x80010FC4: addiu       $a0, $zero, -0x1
    ctx->r4 = ADD32(0, -0X1);
    // 0x80010FC8: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80010FCC: addiu       $a1, $a1, 0x1604
    ctx->r5 = ADD32(ctx->r5, 0X1604);
    // 0x80010FD0: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80010FD4: bne         $v0, $a0, L_80010FF4
    if (ctx->r2 != ctx->r4) {
        // 0x80010FD8: lw          $a3, 0x2C($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X2C);
            goto L_80010FF4;
    }
    // 0x80010FD8: lw          $a3, 0x2C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X2C);
    // 0x80010FDC: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
    // 0x80010FE0: lw          $t7, 0x20($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X20);
    // 0x80010FE4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x80010FE8: lw          $t8, 0x0($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X0);
    // 0x80010FEC: b           L_80011050
    // 0x80010FF0: sw          $zero, 0x4($t8)
    MEM_W(0X4, ctx->r24) = 0;
        goto L_80011050;
    // 0x80010FF0: sw          $zero, 0x4($t8)
    MEM_W(0X4, ctx->r24) = 0;
L_80010FF4:
    // 0x80010FF4: beq         $v0, $zero, L_80011008
    if (ctx->r2 == 0) {
        // 0x80010FF8: addiu       $t0, $zero, 0x1
        ctx->r8 = ADD32(0, 0X1);
            goto L_80011008;
    }
    // 0x80010FF8: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x80010FFC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80011000: b           L_8001100C
    // 0x80011004: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
        goto L_8001100C;
    // 0x80011004: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
L_80011008:
    // 0x80011008: sw          $t0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r8;
L_8001100C:
    // 0x8001100C: lw          $v0, 0x0($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X0);
    // 0x80011010: bnel        $v0, $zero, L_80011024
    if (ctx->r2 != 0) {
        // 0x80011014: addiu       $a0, $sp, 0x20
        ctx->r4 = ADD32(ctx->r29, 0X20);
            goto L_80011024;
    }
    goto skip_3;
    // 0x80011014: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    skip_3:
    // 0x80011018: sw          $a0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r4;
    // 0x8001101C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80011020: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
L_80011024:
    // 0x80011024: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80011028: jal         0x800019B0
    // 0x8001102C: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    func_800019B0(rdram, ctx);
        goto after_4;
    // 0x8001102C: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    after_4:
    // 0x80011030: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    // 0x80011034: jal         0x800019B0
    // 0x80011038: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    func_800019B0(rdram, ctx);
        goto after_5;
    // 0x80011038: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    after_5:
    // 0x8001103C: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x80011040: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80011044: jal         0x800019B0
    // 0x80011048: addiu       $a0, $a0, 0x34
    ctx->r4 = ADD32(ctx->r4, 0X34);
    func_800019B0(rdram, ctx);
        goto after_6;
    // 0x80011048: addiu       $a0, $a0, 0x34
    ctx->r4 = ADD32(ctx->r4, 0X34);
    after_6:
    // 0x8001104C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80011050:
    {
        static int efc_ret = 0; efc_ret++;
        if (efc_ret <= 5)
            fprintf(stderr, "[func_80010EFC] returning v0=0x%08X (0=ok, -1=fail)\n", (uint32_t)ctx->r2);
    }
    // 0x80011050: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80011054: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80011058: jr          $ra
    // 0x8001105C: nop

    return;
    // 0x8001105C: nop

;}
RECOMP_FUNC void func_80011060(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    {
        static int mc = 0; mc++;
        if (mc <= 3) {
            uint32_t o = (uint32_t)ctx->r4 & 0x3FFFFFFF;
            int16_t id = (int16_t)((rdram[o] << 8) | rdram[o+1]);
            uint16_t idx = id & 0x7FF;
            uint32_t t = 0xAEA8C + idx * 4;
            uint32_t dp = (rdram[t]<<24)|(rdram[t+1]<<16)|(rdram[t+2]<<8)|rdram[t+3];
            uint32_t d = dp & 0x3FFFFFFF;
            uint32_t w0 = (rdram[d]<<24)|(rdram[d+1]<<16)|(rdram[d+2]<<8)|rdram[d+3];
            uint32_t w1 = (rdram[d+4]<<24)|(rdram[d+5]<<16)|(rdram[d+6]<<8)|rdram[d+7];
            uint32_t so = 0xF3F54;
            int16_t ovl_sel = (int16_t)((rdram[so] << 8) | rdram[so+1]);
            fprintf(stderr, "[mapOverlay] #%d obj=0x%X id=0x%04X idx=%d sel=0x%04X desc=0x%08X\n",
                    mc, (uint32_t)ctx->r4, (uint16_t)id, idx, (uint16_t)ovl_sel, dp);
            fflush(stderr);
        }
    }
    // 0x80011060: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80011064: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80011068: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x8001106C: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x80011070: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x80011074: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x80011078: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8001107C: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x80011080: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80011084: lui         $at, 0xFFF
    ctx->r1 = S32(0XFFF << 16);
    // 0x80011088: andi        $t7, $t6, 0x7FF
    ctx->r15 = ctx->r14 & 0X7FF;
    // 0x8001108C: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80011090: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x80011094: lw          $t9, -0x1574($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X1574);
    // 0x80011098: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x8001109C: lui         $s2, 0x801D
    ctx->r18 = S32(0X801D << 16);
    // 0x800110A0: lw          $v0, 0x0($t9)
    ctx->r2 = MEM_W(ctx->r25, 0X0);
    {
        static int mo_trace = 0; mo_trace++;
        if (mo_trace <= 5)
            fprintf(stderr, "[mapOverlay TRACE] id=%d desc_ptr=0x%08X desc_val=0x%08X slot_idx=%d\n",
                    (int)(ctx->r14 & 0x7FF), (uint32_t)ctx->r25, (uint32_t)ctx->r2,
                    (int)(ctx->r7));
    }
    // 0x800110A4: lui         $a3, 0x800C
    ctx->r7 = S32(0X800C << 16);
    // 0x800110A8: lw          $a3, 0x15E4($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X15E4);
    // 0x800110AC: and         $t2, $v0, $at
    ctx->r10 = ctx->r2 & ctx->r1;
    // 0x800110B0: sll         $t1, $t2, 2
    ctx->r9 = S32(ctx->r10 << 2);
    // 0x800110B4: addu        $s2, $s2, $t1
    ctx->r18 = ADD32(ctx->r18, ctx->r9);
    // 0x800110B8: lw          $s2, -0x77D0($s2)
    ctx->r18 = MEM_W(ctx->r18, -0X77D0);
    // 0x800110BC: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x800110C0: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x800110C4: and         $s2, $s2, $at
    ctx->r18 = ctx->r18 & ctx->r1;
    // 0x800110C8: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800110CC: lui         $t4, 0x800B
    ctx->r12 = S32(0X800B << 16);
    // 0x800110D0: sll         $v1, $a3, 2
    ctx->r3 = S32(ctx->r7 << 2);
    // 0x800110D4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x800110D8: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x800110DC: addiu       $t4, $t4, 0xC4
    ctx->r12 = ADD32(ctx->r12, 0XC4);
    // 0x800110E0: sll         $t3, $t2, 3
    ctx->r11 = S32(ctx->r10 << 3);
    // 0x800110E4: sw          $s2, 0x15E8($at)
    MEM_W(0X15E8, ctx->r1) = ctx->r18;
    // 0x800110E8: addu        $a0, $t3, $t4
    ctx->r4 = ADD32(ctx->r11, ctx->r12);
    // 0x800110EC: lw          $t6, -0x8($a0)
    ctx->r14 = MEM_W(ctx->r4, -0X8);
    // 0x800110F0: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x800110F4: lw          $t5, -0x4($a0)
    ctx->r13 = MEM_W(ctx->r4, -0X4);
    // 0x800110F8: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x800110FC: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x80011100: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80011104: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x80011108: subu        $s4, $t5, $t7
    ctx->r20 = SUB32(ctx->r13, ctx->r15);
    // 0x8001110C: sw          $s4, 0x15F8($at)
    MEM_W(0X15F8, ctx->r1) = ctx->r20;
    // 0x80011110: lh          $t8, 0x0($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X0);
    // 0x80011114: lui         $t0, 0x800B
    ctx->r8 = S32(0X800B << 16);
    // 0x80011118: lui         $at, 0xF00
    ctx->r1 = S32(0XF00 << 16);
    // 0x8001111C: andi        $t9, $t8, 0x7FF
    ctx->r25 = ctx->r24 & 0X7FF;
    // 0x80011120: sll         $t1, $t9, 2
    ctx->r9 = S32(ctx->r25 << 2);
    // 0x80011124: addu        $t0, $t0, $t1
    ctx->r8 = ADD32(ctx->r8, ctx->r9);
    // 0x80011128: lw          $t0, -0x29C0($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X29C0);
    // 0x8001112C: and         $t0, $t0, $at
    ctx->r8 = ctx->r8 & ctx->r1;
    // 0x80011130: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80011134: addu        $at, $at, $v1
    ctx->r1 = ADD32(ctx->r1, ctx->r3);
    // 0x80011138: sw          $t0, 0x15F0($at)
    MEM_W(0X15F0, ctx->r1) = ctx->r8;
    // 0x8001113C: addiu       $at, $zero, 0xC
    ctx->r1 = ADD32(0, 0XC);
    // 0x80011140: srl         $a1, $t0, 24
    ctx->r5 = S32(U32(ctx->r8) >> 24);
    // 0x80011144: beq         $a1, $at, L_800111A8
    if (ctx->r5 == ctx->r1) {
        // 0x80011148: or          $s1, $t0, $zero
        ctx->r17 = ctx->r8 | 0;
            goto L_800111A8;
    }
    // 0x80011148: or          $s1, $t0, $zero
    ctx->r17 = ctx->r8 | 0;
    // 0x8001114C: addiu       $at, $zero, 0xD
    ctx->r1 = ADD32(0, 0XD);
    // 0x80011150: beq         $a1, $at, L_800111C4
    if (ctx->r5 == ctx->r1) {
        // 0x80011154: addiu       $s3, $zero, 0x8
        ctx->r19 = ADD32(0, 0X8);
            goto L_800111C4;
    }
    // 0x80011154: addiu       $s3, $zero, 0x8
    ctx->r19 = ADD32(0, 0X8);
    // 0x80011158: addiu       $at, $zero, 0xE
    ctx->r1 = ADD32(0, 0XE);
    // 0x8001115C: beq         $a1, $at, L_80011190
    if (ctx->r5 == ctx->r1) {
        // 0x80011160: addiu       $at, $zero, 0xF
        ctx->r1 = ADD32(0, 0XF);
            goto L_80011190;
    }
    // 0x80011160: addiu       $at, $zero, 0xF
    ctx->r1 = ADD32(0, 0XF);
    // 0x80011164: beql        $a1, $at, L_80011178
    if (ctx->r5 == ctx->r1) {
        // 0x80011168: lui         $at, 0x3
        ctx->r1 = S32(0X3 << 16);
            goto L_80011178;
    }
    goto skip_0;
    // 0x80011168: lui         $at, 0x3
    ctx->r1 = S32(0X3 << 16);
    skip_0:
    // 0x8001116C: b           L_800111C4
    // 0x80011170: lw          $s3, 0x38($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X38);
        goto L_800111C4;
    // 0x80011170: lw          $s3, 0x38($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X38);
    // 0x80011174: lui         $at, 0x3
    ctx->r1 = S32(0X3 << 16);
L_80011178:
    // 0x80011178: ori         $at, $at, 0x6000
    ctx->r1 = ctx->r1 | 0X6000;
    // 0x8001117C: sltu        $at, $s4, $at
    ctx->r1 = ctx->r20 < ctx->r1 ? 1 : 0;
    // 0x80011180: bne         $at, $zero, L_800111C4
    if (ctx->r1 != 0) {
        // 0x80011184: or          $s3, $zero, $zero
        ctx->r19 = 0 | 0;
            goto L_800111C4;
    }
    // 0x80011184: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x80011188: b           L_800111C4
    // 0x8001118C: nop

        goto L_800111C4;
    // 0x8001118C: nop

L_80011190:
    // 0x80011190: ori         $at, $zero, 0x8000
    ctx->r1 = 0 | 0X8000;
    // 0x80011194: sltu        $at, $s4, $at
    ctx->r1 = ctx->r20 < ctx->r1 ? 1 : 0;
    // 0x80011198: bne         $at, $zero, L_800111C4
    if (ctx->r1 != 0) {
        // 0x8001119C: addiu       $s3, $zero, 0x1B
        ctx->r19 = ADD32(0, 0X1B);
            goto L_800111C4;
    }
    // 0x8001119C: addiu       $s3, $zero, 0x1B
    ctx->r19 = ADD32(0, 0X1B);
    // 0x800111A0: b           L_800111C4
    // 0x800111A4: nop

        goto L_800111C4;
    // 0x800111A4: nop

L_800111A8:
    // 0x800111A8: lui         $at, 0x1
    ctx->r1 = S32(0X1 << 16);
    // 0x800111AC: ori         $at, $at, 0x6000
    ctx->r1 = ctx->r1 | 0X6000;
    // 0x800111B0: sltu        $at, $s4, $at
    ctx->r1 = ctx->r20 < ctx->r1 ? 1 : 0;
    // 0x800111B4: bne         $at, $zero, L_800111C4
    if (ctx->r1 != 0) {
        // 0x800111B8: addiu       $s3, $zero, 0x10
        ctx->r19 = ADD32(0, 0X10);
            goto L_800111C4;
    }
    // 0x800111B8: addiu       $s3, $zero, 0x10
    ctx->r19 = ADD32(0, 0X10);
    // 0x800111BC: b           L_800111C4
    // 0x800111C0: nop

        goto L_800111C4;
    // 0x800111C0: nop

L_800111C4:
    // 0x800111C4: beq         $s4, $zero, L_80011210
    if (ctx->r20 == 0) {
        // 0x800111C8: or          $s0, $zero, $zero
        ctx->r16 = 0 | 0;
            goto L_80011210;
    }
    // 0x800111C8: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_800111CC:
    // 0x800111CC: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x800111D0: addiu       $t2, $s2, 0x1000
    ctx->r10 = ADD32(ctx->r18, 0X1000);
    // 0x800111D4: addiu       $t3, $zero, -0x1
    ctx->r11 = ADD32(0, -0X1);
    // 0x800111D8: sw          $t3, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r11;
    // 0x800111DC: sw          $t2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r10;
    // 0x800111E0: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x800111E4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x800111E8: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x800111EC: jal         0x80097730
    // 0x800111F0: or          $a3, $s2, $zero
    ctx->r7 = ctx->r18 | 0;
    func_80097730(rdram, ctx);
        goto after_0;
    // 0x800111F0: or          $a3, $s2, $zero
    ctx->r7 = ctx->r18 | 0;
    after_0:
    // 0x800111F4: addiu       $s0, $s0, 0x2000
    ctx->r16 = ADD32(ctx->r16, 0X2000);
    // 0x800111F8: sltu        $at, $s0, $s4
    ctx->r1 = ctx->r16 < ctx->r20 ? 1 : 0;
    // 0x800111FC: addiu       $s1, $s1, 0x2000
    ctx->r17 = ADD32(ctx->r17, 0X2000);
    // 0x80011200: bne         $at, $zero, L_800111CC
    if (ctx->r1 != 0) {
        // 0x80011204: addiu       $s2, $s2, 0x2000
        ctx->r18 = ADD32(ctx->r18, 0X2000);
            goto L_800111CC;
    }
    // 0x80011204: addiu       $s2, $s2, 0x2000
    ctx->r18 = ADD32(ctx->r18, 0X2000);
    // 0x80011208: lui         $a3, 0x800C
    ctx->r7 = S32(0X800C << 16);
    // 0x8001120C: lw          $a3, 0x15E4($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X15E4);
L_80011210:
    // 0x80011210: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80011214: addiu       $t4, $a3, 0x1
    ctx->r12 = ADD32(ctx->r7, 0X1);
    // 0x80011218: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x8001121C: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80011220: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80011224: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x80011228: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x8001122C: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x80011230: sw          $t4, 0x15E4($at)
    MEM_W(0X15E4, ctx->r1) = ctx->r12;
    // 0x80011234: jr          $ra
    // 0x80011238: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x80011238: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_8001123C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001123C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80011240: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80011244: lui         $s2, 0x800C
    ctx->r18 = S32(0X800C << 16);
    // 0x80011248: addiu       $s2, $s2, 0x15E4
    ctx->r18 = ADD32(ctx->r18, 0X15E4);
    // 0x8001124C: lw          $t6, 0x0($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X0);
    // 0x80011250: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80011254: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x80011258: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x8001125C: sll         $s1, $t7, 2
    ctx->r17 = S32(ctx->r15 << 2);
    // 0x80011260: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80011264: sw          $t7, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r15;
    // 0x80011268: addu        $a0, $a0, $s1
    ctx->r4 = ADD32(ctx->r4, ctx->r17);
    // 0x8001126C: lw          $a0, 0x15F0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X15F0);
    // 0x80011270: addiu       $at, $zero, 0xC
    ctx->r1 = ADD32(0, 0XC);
    // 0x80011274: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80011278: sra         $v0, $a0, 24
    ctx->r2 = S32(SIGNED(ctx->r4) >> 24);
    // 0x8001127C: beq         $v0, $at, L_8001134C
    if (ctx->r2 == ctx->r1) {
        // 0x80011280: sw          $s0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r16;
            goto L_8001134C;
    }
    // 0x80011280: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80011284: addiu       $at, $zero, 0xD
    ctx->r1 = ADD32(0, 0XD);
    // 0x80011288: beq         $v0, $at, L_8001134C
    if (ctx->r2 == ctx->r1) {
        // 0x8001128C: lui         $a1, 0x800C
        ctx->r5 = S32(0X800C << 16);
            goto L_8001134C;
    }
    // 0x8001128C: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80011290: addiu       $at, $zero, 0xE
    ctx->r1 = ADD32(0, 0XE);
    // 0x80011294: beq         $v0, $at, L_800112B0
    if (ctx->r2 == ctx->r1) {
        // 0x80011298: addu        $a1, $a1, $s1
        ctx->r5 = ADD32(ctx->r5, ctx->r17);
            goto L_800112B0;
    }
    // 0x80011298: addu        $a1, $a1, $s1
    ctx->r5 = ADD32(ctx->r5, ctx->r17);
    // 0x8001129C: addiu       $at, $zero, 0xF
    ctx->r1 = ADD32(0, 0XF);
    // 0x800112A0: bnel        $v0, $at, L_80011350
    if (ctx->r2 != ctx->r1) {
        // 0x800112A4: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_80011350;
    }
    goto skip_0;
    // 0x800112A4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_0:
    // 0x800112A8: b           L_800112B4
    // 0x800112AC: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
        goto L_800112B4;
    // 0x800112AC: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_800112B0:
    // 0x800112B0: addiu       $s0, $zero, 0x1B
    ctx->r16 = ADD32(0, 0X1B);
L_800112B4:
    // 0x800112B4: jal         0x80099440
    // 0x800112B8: lw          $a1, 0x15F8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X15F8);
    osWritebackDCache_recomp(rdram, ctx);
        goto after_0;
    // 0x800112B8: lw          $a1, 0x15F8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X15F8);
    after_0:
    // 0x800112BC: lw          $s1, 0x0($s2)
    ctx->r17 = MEM_W(ctx->r18, 0X0);
    // 0x800112C0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800112C4: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x800112C8: sll         $s1, $s1, 2
    ctx->r17 = S32(ctx->r17 << 2);
    // 0x800112CC: addu        $a0, $a0, $s1
    ctx->r4 = ADD32(ctx->r4, ctx->r17);
    // 0x800112D0: addu        $a1, $a1, $s1
    ctx->r5 = ADD32(ctx->r5, ctx->r17);
    // 0x800112D4: lw          $a1, 0x15F8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X15F8);
    // 0x800112D8: jal         0x800A13E0
    // 0x800112DC: lw          $a0, 0x15F0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X15F0);
    osInvalICache_recomp(rdram, ctx);
        goto after_1;
    // 0x800112DC: lw          $a0, 0x15F0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X15F0);
    after_1:
    // 0x800112E0: lw          $s1, 0x0($s2)
    ctx->r17 = MEM_W(ctx->r18, 0X0);
    // 0x800112E4: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800112E8: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x800112EC: sll         $s1, $s1, 2
    ctx->r17 = S32(ctx->r17 << 2);
    // 0x800112F0: addu        $a0, $a0, $s1
    ctx->r4 = ADD32(ctx->r4, ctx->r17);
    // 0x800112F4: addu        $a1, $a1, $s1
    ctx->r5 = ADD32(ctx->r5, ctx->r17);
    // 0x800112F8: lw          $a1, 0x15F8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X15F8);
    // 0x800112FC: jal         0x800A1490
    // 0x80011300: lw          $a0, 0x15F0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X15F0);
    osInvalDCache_recomp(rdram, ctx);
        goto after_2;
    // 0x80011300: lw          $a0, 0x15F0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X15F0);
    after_2:
    // 0x80011304: lw          $t8, 0x0($s2)
    ctx->r24 = MEM_W(ctx->r18, 0X0);
    // 0x80011308: lui         $t0, 0x800C
    ctx->r8 = S32(0X800C << 16);
    // 0x8001130C: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80011310: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80011314: lw          $t0, 0x15F8($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X15F8);
    // 0x80011318: addiu       $t1, $t0, 0x1FFF
    ctx->r9 = ADD32(ctx->r8, 0X1FFF);
    // 0x8001131C: bgez        $t1, L_8001132C
    if (SIGNED(ctx->r9) >= 0) {
        // 0x80011320: sra         $t2, $t1, 13
        ctx->r10 = S32(SIGNED(ctx->r9) >> 13);
            goto L_8001132C;
    }
    // 0x80011320: sra         $t2, $t1, 13
    ctx->r10 = S32(SIGNED(ctx->r9) >> 13);
    // 0x80011324: addiu       $at, $t1, 0x1FFF
    ctx->r1 = ADD32(ctx->r9, 0X1FFF);
    // 0x80011328: sra         $t2, $at, 13
    ctx->r10 = S32(SIGNED(ctx->r1) >> 13);
L_8001132C:
    // 0x8001132C: addu        $s1, $t2, $s0
    ctx->r17 = ADD32(ctx->r10, ctx->r16);
    // 0x80011330: blez        $s1, L_8001134C
    if (SIGNED(ctx->r17) <= 0) {
        // 0x80011334: or          $s0, $zero, $zero
        ctx->r16 = 0 | 0;
            goto L_8001134C;
    }
    // 0x80011334: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_80011338:
    // 0x80011338: jal         0x800A02F0
    // 0x8001133C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_800A02F0(rdram, ctx);
        goto after_3;
    // 0x8001133C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x80011340: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80011344: bne         $s0, $s1, L_80011338
    if (ctx->r16 != ctx->r17) {
        // 0x80011348: nop
    
            goto L_80011338;
    }
    // 0x80011348: nop

L_8001134C:
    // 0x8001134C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_80011350:
    // 0x80011350: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80011354: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80011358: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8001135C: jr          $ra
    // 0x80011360: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80011360: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80011364(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011364: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80011368: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001136C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80011370: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80011374: addiu       $a0, $a0, -0x7C14
    ctx->r4 = ADD32(ctx->r4, -0X7C14);
    // 0x80011378: jal         0x800048A0
    // 0x8001137C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    func_800048A0(rdram, ctx);
        goto after_0;
    // 0x8001137C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    after_0:
    // 0x80011380: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80011384: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80011388: addiu       $a0, $a0, -0x7C14
    ctx->r4 = ADD32(ctx->r4, -0X7C14);
    // 0x8001138C: jal         0x800048C4
    // 0x80011390: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    func_800048C4(rdram, ctx);
        goto after_1;
    // 0x80011390: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x80011394: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80011398: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x8001139C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800113A0: jr          $ra
    // 0x800113A4: nop

    return;
    // 0x800113A4: nop

;}
RECOMP_FUNC void func_800113A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800113A8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800113AC: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x800113B0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800113B4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800113B8: addiu       $a0, $a0, -0x7C14
    ctx->r4 = ADD32(ctx->r4, -0X7C14);
    // 0x800113BC: jal         0x800048A0
    // 0x800113C0: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    func_800048A0(rdram, ctx);
        goto after_0;
    // 0x800113C0: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    after_0:
    // 0x800113C4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800113C8: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x800113CC: addiu       $a0, $a0, -0x7C14
    ctx->r4 = ADD32(ctx->r4, -0X7C14);
    // 0x800113D0: jal         0x800048EC
    // 0x800113D4: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    func_800048EC(rdram, ctx);
        goto after_1;
    // 0x800113D4: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x800113D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800113DC: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x800113E0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800113E4: jr          $ra
    // 0x800113E8: nop

    return;
    // 0x800113E8: nop

;}
RECOMP_FUNC void func_800113EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800113EC: addiu       $sp, $sp, -0x270
    ctx->r29 = ADD32(ctx->r29, -0X270);
    // 0x800113F0: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800113F4: sw          $a0, 0x15E0($at)
    MEM_W(0X15E0, ctx->r1) = ctx->r4;
    // 0x800113F8: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800113FC: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80011400: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x80011404: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80011408: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x8001140C: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80011410: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80011414: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80011418: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8001141C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80011420: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80011424: sw          $zero, 0x15E4($at)
    MEM_W(0X15E4, ctx->r1) = 0;
    // 0x80011428: lw          $s2, 0x34($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X34);
    // 0x8001142C: lui         $s4, 0x801D
    ctx->r20 = S32(0X801D << 16);
    // 0x80011430: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80011434: beq         $s2, $zero, L_800114B8
    if (ctx->r18 == 0) {
        // 0x80011438: addiu       $s4, $s4, -0x7A14
        ctx->r20 = ADD32(ctx->r20, -0X7A14);
            goto L_800114B8;
    }
    // 0x80011438: addiu       $s4, $s4, -0x7A14
    ctx->r20 = ADD32(ctx->r20, -0X7A14);
    // 0x8001143C: addiu       $s1, $zero, -0x2001
    ctx->r17 = ADD32(0, -0X2001);
    // 0x80011440: lw          $s0, 0x4($s2)
    ctx->r16 = MEM_W(ctx->r18, 0X4);
L_80011444:
    // 0x80011444: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x80011448: lw          $a1, 0x4($t6)
    ctx->r5 = MEM_W(ctx->r14, 0X4);
    // 0x8001144C: bltzl       $a1, L_80011464
    if (SIGNED(ctx->r5) < 0) {
        // 0x80011450: addiu       $a0, $s3, 0x34
        ctx->r4 = ADD32(ctx->r19, 0X34);
            goto L_80011464;
    }
    goto skip_0;
    // 0x80011450: addiu       $a0, $s3, 0x34
    ctx->r4 = ADD32(ctx->r19, 0X34);
    skip_0:
    // 0x80011454: jal         0x800048A0
    // 0x80011458: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    func_800048A0(rdram, ctx);
        goto after_0;
    // 0x80011458: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    after_0:
    // 0x8001145C: bne         $v0, $zero, L_800114AC
    if (ctx->r2 != 0) {
        // 0x80011460: addiu       $a0, $s3, 0x34
        ctx->r4 = ADD32(ctx->r19, 0X34);
            goto L_800114AC;
    }
    // 0x80011460: addiu       $a0, $s3, 0x34
    ctx->r4 = ADD32(ctx->r19, 0X34);
L_80011464:
    // 0x80011464: jal         0x80001B00
    // 0x80011468: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80001B00(rdram, ctx);
        goto after_1;
    // 0x80011468: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x8001146C: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x80011470: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x80011474: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x80011478: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x8001147C: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x80011480: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x80011484: beql        $s0, $zero, L_800114B0
    if (ctx->r16 == 0) {
        // 0x80011488: lw          $s2, 0x0($s2)
        ctx->r18 = MEM_W(ctx->r18, 0X0);
            goto L_800114B0;
    }
    goto skip_1;
    // 0x80011488: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
    skip_1:
    // 0x8001148C: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
L_80011490:
    // 0x80011490: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
    // 0x80011494: and         $t8, $t7, $s1
    ctx->r24 = ctx->r15 & ctx->r17;
    // 0x80011498: sh          $t8, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r24;
    // 0x8001149C: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x800114A0: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x800114A4: bnel        $s0, $zero, L_80011490
    if (ctx->r16 != 0) {
        // 0x800114A8: lw          $v0, 0x4($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X4);
            goto L_80011490;
    }
    goto skip_2;
    // 0x800114A8: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    skip_2:
L_800114AC:
    // 0x800114AC: lw          $s2, 0x0($s2)
    ctx->r18 = MEM_W(ctx->r18, 0X0);
L_800114B0:
    // 0x800114B0: bnel        $s2, $zero, L_80011444
    if (ctx->r18 != 0) {
        // 0x800114B4: lw          $s0, 0x4($s2)
        ctx->r16 = MEM_W(ctx->r18, 0X4);
            goto L_80011444;
    }
    goto skip_3;
    // 0x800114B4: lw          $s0, 0x4($s2)
    ctx->r16 = MEM_W(ctx->r18, 0X4);
    skip_3:
L_800114B8:
    // 0x800114B8: addiu       $s6, $sp, 0x4C
    ctx->r22 = ADD32(ctx->r29, 0X4C);
    // 0x800114BC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800114C0: addiu       $a0, $a0, -0x7C14
    ctx->r4 = ADD32(ctx->r4, -0X7C14);
    // 0x800114C4: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    // 0x800114C8: jal         0x80001090
    // 0x800114CC: addiu       $a2, $zero, 0x200
    ctx->r6 = ADD32(0, 0X200);
    func_80001090(rdram, ctx);
        goto after_2;
    // 0x800114CC: addiu       $a2, $zero, 0x200
    ctx->r6 = ADD32(0, 0X200);
    after_2:
    // 0x800114D0: lui         $fp, 0x800B
    ctx->r30 = S32(0X800B << 16);
    // 0x800114D4: lui         $s4, 0xFFF
    ctx->r20 = S32(0XFFF << 16);
    // 0x800114D8: ori         $s4, $s4, 0xFFFF
    ctx->r20 = ctx->r20 | 0XFFFF;
    // 0x800114DC: addiu       $fp, $fp, -0x156C
    ctx->r30 = ADD32(ctx->r30, -0X156C);
    // 0x800114E0: addiu       $s7, $zero, 0x1
    ctx->r23 = ADD32(0, 0X1);
    // 0x800114E4: lui         $s5, 0x4000
    ctx->r21 = S32(0X4000 << 16);
    // 0x800114E8: lui         $s3, 0x8000
    ctx->r19 = S32(0X8000 << 16);
L_800114EC:
    // 0x800114EC: lw          $s1, -0x4($fp)
    ctx->r17 = MEM_W(ctx->r30, -0X4);
    // 0x800114F0: sll         $t9, $s7, 1
    ctx->r25 = S32(ctx->r23 << 1);
    // 0x800114F4: lui         $t0, 0x801B
    ctx->r8 = S32(0X801B << 16);
    // 0x800114F8: beq         $s1, $zero, L_80011544
    if (ctx->r17 == 0) {
        // 0x800114FC: addu        $t0, $t0, $t9
        ctx->r8 = ADD32(ctx->r8, ctx->r25);
            goto L_80011544;
    }
    // 0x800114FC: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80011500: lhu         $t0, 0x3D5E($t0)
    ctx->r8 = MEM_HU(ctx->r8, 0X3D5E);
    // 0x80011504: beql        $t0, $zero, L_80011548
    if (ctx->r8 == 0) {
        // 0x80011508: addiu       $s7, $s7, 0x1
        ctx->r23 = ADD32(ctx->r23, 0X1);
            goto L_80011548;
    }
    goto skip_4;
    // 0x80011508: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
    skip_4:
L_8001150C:
    // 0x8001150C: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x80011510: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80011514: and         $t1, $s0, $s3
    ctx->r9 = ctx->r16 & ctx->r19;
    // 0x80011518: bne         $t1, $zero, L_80011530
    if (ctx->r9 != 0) {
        // 0x8001151C: and         $s2, $s0, $s4
        ctx->r18 = ctx->r16 & ctx->r20;
            goto L_80011530;
    }
    // 0x8001151C: and         $s2, $s0, $s4
    ctx->r18 = ctx->r16 & ctx->r20;
    // 0x80011520: sll         $s2, $s2, 16
    ctx->r18 = S32(ctx->r18 << 16);
    // 0x80011524: jal         0x800048C4
    // 0x80011528: sra         $a1, $s2, 16
    ctx->r5 = S32(SIGNED(ctx->r18) >> 16);
    func_800048C4(rdram, ctx);
        goto after_3;
    // 0x80011528: sra         $a1, $s2, 16
    ctx->r5 = S32(SIGNED(ctx->r18) >> 16);
    after_3:
    // 0x8001152C: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
L_80011530:
    // 0x80011530: and         $t2, $s0, $s5
    ctx->r10 = ctx->r16 & ctx->r21;
    // 0x80011534: bnel        $t2, $zero, L_80011548
    if (ctx->r10 != 0) {
        // 0x80011538: addiu       $s7, $s7, 0x1
        ctx->r23 = ADD32(ctx->r23, 0X1);
            goto L_80011548;
    }
    goto skip_5;
    // 0x80011538: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
    skip_5:
    // 0x8001153C: b           L_8001150C
    // 0x80011540: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
        goto L_8001150C;
    // 0x80011540: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
L_80011544:
    // 0x80011544: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_80011548:
    // 0x80011548: slti        $at, $s7, 0x2B0
    ctx->r1 = SIGNED(ctx->r23) < 0X2B0 ? 1 : 0;
    // 0x8001154C: bne         $at, $zero, L_800114EC
    if (ctx->r1 != 0) {
        // 0x80011550: addiu       $fp, $fp, 0x4
        ctx->r30 = ADD32(ctx->r30, 0X4);
            goto L_800114EC;
    }
    // 0x80011550: addiu       $fp, $fp, 0x4
    ctx->r30 = ADD32(ctx->r30, 0X4);
    // 0x80011554: lui         $s1, 0x801D
    ctx->r17 = S32(0X801D << 16);
    // 0x80011558: addiu       $s1, $s1, -0x7D40
    ctx->r17 = ADD32(ctx->r17, -0X7D40);
    // 0x8001155C: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80011560: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
L_80011564:
    // 0x80011564: jal         0x800048A0
    // 0x80011568: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_800048A0(rdram, ctx);
        goto after_4;
    // 0x80011568: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_4:
    // 0x8001156C: bne         $v0, $zero, L_80011590
    if (ctx->r2 != 0) {
        // 0x80011570: sll         $t3, $s2, 2
        ctx->r11 = S32(ctx->r18 << 2);
            goto L_80011590;
    }
    // 0x80011570: sll         $t3, $s2, 2
    ctx->r11 = S32(ctx->r18 << 2);
    // 0x80011574: addu        $s0, $s1, $t3
    ctx->r16 = ADD32(ctx->r17, ctx->r11);
    // 0x80011578: lw          $v0, 0x570($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X570);
    // 0x8001157C: beql        $v0, $zero, L_80011594
    if (ctx->r2 == 0) {
        // 0x80011580: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80011594;
    }
    goto skip_6;
    // 0x80011580: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_6:
    // 0x80011584: jal         0x80001918
    // 0x80011588: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    func_80001918(rdram, ctx);
        goto after_5;
    // 0x80011588: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    after_5:
    // 0x8001158C: sw          $zero, 0x570($s0)
    MEM_W(0X570, ctx->r16) = 0;
L_80011590:
    // 0x80011590: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80011594:
    // 0x80011594: sll         $s2, $s2, 16
    ctx->r18 = S32(ctx->r18 << 16);
    // 0x80011598: sra         $s2, $s2, 16
    ctx->r18 = S32(SIGNED(ctx->r18) >> 16);
    // 0x8001159C: slti        $at, $s2, 0x200
    ctx->r1 = SIGNED(ctx->r18) < 0X200 ? 1 : 0;
    // 0x800115A0: bnel        $at, $zero, L_80011564
    if (ctx->r1 != 0) {
        // 0x800115A4: or          $a0, $s6, $zero
        ctx->r4 = ctx->r22 | 0;
            goto L_80011564;
    }
    goto skip_7;
    // 0x800115A4: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    skip_7:
    // 0x800115A8: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x800115AC: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800115B0: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x800115B4: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x800115B8: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x800115BC: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x800115C0: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x800115C4: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x800115C8: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x800115CC: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x800115D0: jr          $ra
    // 0x800115D4: addiu       $sp, $sp, 0x270
    ctx->r29 = ADD32(ctx->r29, 0X270);
    return;
    // 0x800115D4: addiu       $sp, $sp, 0x270
    ctx->r29 = ADD32(ctx->r29, 0X270);
;}
RECOMP_FUNC void func_800115E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800115E0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800115E4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800115E8: lh          $v1, 0x0($a0)
    ctx->r3 = MEM_H(ctx->r4, 0X0);
    // 0x800115EC: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x800115F0: lui         $a0, 0x800B
    ctx->r4 = S32(0X800B << 16);
    // 0x800115F4: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x800115F8: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x800115FC: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80011600: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80011604: addiu       $a0, $a0, -0xAB0
    ctx->r4 = ADD32(ctx->r4, -0XAB0);
    // 0x80011608: sll         $t6, $v0, 1
    ctx->r14 = S32(ctx->r2 << 1);
L_8001160C:
    // 0x8001160C: addu        $t7, $a0, $t6
    ctx->r15 = ADD32(ctx->r4, ctx->r14);
    // 0x80011610: lh          $t8, 0x0($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X0);
    // 0x80011614: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80011618: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x8001161C: bne         $v1, $t8, L_80011634
    if (ctx->r3 != ctx->r24) {
        // 0x80011620: sra         $v0, $v0, 16
        ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
            goto L_80011634;
    }
    // 0x80011620: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x80011624: jal         0x8001164C
    // 0x80011628: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_8001164C(rdram, ctx);
        goto after_0;
    // 0x80011628: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_0:
    // 0x8001162C: b           L_80011640
    // 0x80011630: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80011640;
    // 0x80011630: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80011634:
    // 0x80011634: bgezl       $v0, L_8001160C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80011638: sll         $t6, $v0, 1
        ctx->r14 = S32(ctx->r2 << 1);
            goto L_8001160C;
    }
    goto skip_0;
    // 0x80011638: sll         $t6, $v0, 1
    ctx->r14 = S32(ctx->r2 << 1);
    skip_0:
    // 0x8001163C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80011640:
    // 0x80011640: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80011644: jr          $ra
    // 0x80011648: nop

    return;
    // 0x80011648: nop

;}
RECOMP_FUNC void func_8001164C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001164C: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x80011650: lui         $a0, 0x800B
    ctx->r4 = S32(0X800B << 16);
    // 0x80011654: addiu       $a0, $a0, -0xAB0
    ctx->r4 = ADD32(ctx->r4, -0XAB0);
    // 0x80011658: andi        $v0, $v0, 0x7FF
    ctx->r2 = ctx->r2 & 0X7FF;
    // 0x8001165C: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80011660: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x80011664: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80011668: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
L_8001166C:
    // 0x8001166C: addu        $t7, $a0, $t6
    ctx->r15 = ADD32(ctx->r4, ctx->r14);
    // 0x80011670: lh          $t8, 0x0($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X0);
    // 0x80011674: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x80011678: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8001167C: beq         $v0, $t8, L_8001168C
    if (ctx->r2 == ctx->r24) {
        // 0x80011680: sra         $v1, $v1, 16
        ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
            goto L_8001168C;
    }
    // 0x80011680: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80011684: bgezl       $v1, L_8001166C
    if (SIGNED(ctx->r3) >= 0) {
        // 0x80011688: sll         $t6, $v1, 1
        ctx->r14 = S32(ctx->r3 << 1);
            goto L_8001166C;
    }
    goto skip_0;
    // 0x80011688: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    skip_0:
L_8001168C:
    // 0x8001168C: jr          $ra
    // 0x80011690: nop

    return;
    // 0x80011690: nop

;}
RECOMP_FUNC void func_800116A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800116A0: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x800116A4: jr          $ra
    // 0x800116A8: lhu         $v0, 0x846($v1)
    ctx->r2 = MEM_HU(ctx->r3, 0X846);
    return;
    // 0x800116A8: lhu         $v0, 0x846($v1)
    ctx->r2 = MEM_HU(ctx->r3, 0X846);
;}
RECOMP_FUNC void func_800116AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800116AC: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x800116B0: lw          $v0, 0x838($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X838);
    // 0x800116B4: jr          $ra
    // 0x800116B8: sw          $a1, 0x838($v1)
    MEM_W(0X838, ctx->r3) = ctx->r5;
    return;
    // 0x800116B8: sw          $a1, 0x838($v1)
    MEM_W(0X838, ctx->r3) = ctx->r5;
;}
RECOMP_FUNC void func_800116BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800116BC: lw          $t6, 0x10($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X10);
    // 0x800116C0: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x800116C4: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x800116C8: beq         $t6, $zero, L_800116D4
    if (ctx->r14 == 0) {
        // 0x800116CC: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_800116D4;
    }
    // 0x800116CC: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x800116D0: sw          $zero, 0x0($t6)
    MEM_W(0X0, ctx->r14) = 0;
L_800116D4:
    // 0x800116D4: lhu         $a0, 0x846($v1)
    ctx->r4 = MEM_HU(ctx->r3, 0X846);
    // 0x800116D8: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    // 0x800116DC: slti        $at, $a0, 0x10
    ctx->r1 = SIGNED(ctx->r4) < 0X10 ? 1 : 0;
    // 0x800116E0: beq         $at, $zero, L_8001174C
    if (ctx->r1 == 0) {
        // 0x800116E4: addiu       $t1, $a0, 0x1
        ctx->r9 = ADD32(ctx->r4, 0X1);
            goto L_8001174C;
    }
    // 0x800116E4: addiu       $t1, $a0, 0x1
    ctx->r9 = ADD32(ctx->r4, 0X1);
    // 0x800116E8: lhu         $t7, 0x844($v1)
    ctx->r15 = MEM_HU(ctx->r3, 0X844);
    // 0x800116EC: sh          $t1, 0x846($v1)
    MEM_H(0X846, ctx->r3) = ctx->r9;
    // 0x800116F0: lw          $t2, 0x4($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X4);
    // 0x800116F4: addu        $t8, $a0, $t7
    ctx->r24 = ADD32(ctx->r4, ctx->r15);
    // 0x800116F8: bgez        $t8, L_8001170C
    if (SIGNED(ctx->r24) >= 0) {
        // 0x800116FC: andi        $t9, $t8, 0xF
        ctx->r25 = ctx->r24 & 0XF;
            goto L_8001170C;
    }
    // 0x800116FC: andi        $t9, $t8, 0xF
    ctx->r25 = ctx->r24 & 0XF;
    // 0x80011700: beq         $t9, $zero, L_8001170C
    if (ctx->r25 == 0) {
        // 0x80011704: nop
    
            goto L_8001170C;
    }
    // 0x80011704: nop

    // 0x80011708: addiu       $t9, $t9, -0x10
    ctx->r25 = ADD32(ctx->r25, -0X10);
L_8001170C:
    // 0x8001170C: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80011710: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80011714: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x80011718: addu        $a2, $v1, $t0
    ctx->r6 = ADD32(ctx->r3, ctx->r8);
    // 0x8001171C: sw          $t2, 0x848($a2)
    MEM_W(0X848, ctx->r6) = ctx->r10;
    // 0x80011720: lw          $t3, 0x8($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X8);
    // 0x80011724: sw          $a3, 0x850($a2)
    MEM_W(0X850, ctx->r6) = ctx->r7;
    // 0x80011728: lui         $t4, 0x800C
    ctx->r12 = S32(0X800C << 16);
    // 0x8001172C: sw          $t3, 0x84C($a2)
    MEM_W(0X84C, ctx->r6) = ctx->r11;
    // 0x80011730: lw          $t4, 0x1608($t4)
    ctx->r12 = MEM_W(ctx->r12, 0X1608);
    // 0x80011734: addiu       $a2, $a2, 0x848
    ctx->r6 = ADD32(ctx->r6, 0X848);
    // 0x80011738: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001173C: sw          $t4, 0xC($a2)
    MEM_W(0XC, ctx->r6) = ctx->r12;
    // 0x80011740: lw          $t5, 0x10($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X10);
    // 0x80011744: jr          $ra
    // 0x80011748: sw          $t5, 0x10($a2)
    MEM_W(0X10, ctx->r6) = ctx->r13;
    return;
    // 0x80011748: sw          $t5, 0x10($a2)
    MEM_W(0X10, ctx->r6) = ctx->r13;
L_8001174C:
    // 0x8001174C: jr          $ra
    // 0x80011750: nop

    return;
    // 0x80011750: nop

;}
RECOMP_FUNC void func_80011754(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011754: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80011758: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x8001175C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x80011760: sll         $t6, $a1, 0
    ctx->r14 = S32(ctx->r5 << 0);
    // 0x80011764: or          $s0, $a2, $zero
    ctx->r16 = ctx->r6 | 0;
    // 0x80011768: or          $s2, $a1, $zero
    ctx->r18 = ctx->r5 | 0;
    // 0x8001176C: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80011770: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x80011774: sw          $a0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r4;
    // 0x80011778: bgez        $t6, L_800117BC
    if (SIGNED(ctx->r14) >= 0) {
        // 0x8001177C: sw          $a3, 0x5C($sp)
        MEM_W(0X5C, ctx->r29) = ctx->r7;
            goto L_800117BC;
    }
    // 0x8001177C: sw          $a3, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r7;
    // 0x80011780: lui         $at, 0x8FFF
    ctx->r1 = S32(0X8FFF << 16);
    // 0x80011784: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80011788: and         $v1, $s2, $at
    ctx->r3 = ctx->r18 & ctx->r1;
    // 0x8001178C: lw          $a2, 0x0($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X0);
    // 0x80011790: lw          $t7, 0x4($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X4);
    // 0x80011794: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x80011798: lw          $a3, 0x8($v1)
    ctx->r7 = MEM_W(ctx->r3, 0X8);
    // 0x8001179C: subu        $t8, $t7, $a2
    ctx->r24 = SUB32(ctx->r15, ctx->r6);
    // 0x800117A0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    // 0x800117A4: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x800117A8: jal         0x80012ED0
    // 0x800117AC: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    func_80012ED0(rdram, ctx);
        goto after_0;
    // 0x800117AC: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    after_0:
    // 0x800117B0: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x800117B4: b           L_80011958
    // 0x800117B8: lw          $s0, 0x8($v1)
    ctx->r16 = MEM_W(ctx->r3, 0X8);
        goto L_80011958;
    // 0x800117B8: lw          $s0, 0x8($v1)
    ctx->r16 = MEM_W(ctx->r3, 0X8);
L_800117BC:
    // 0x800117BC: lui         $at, 0xFFF
    ctx->r1 = S32(0XFFF << 16);
    // 0x800117C0: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x800117C4: and         $s2, $s2, $at
    ctx->r18 = ctx->r18 & ctx->r1;
    // 0x800117C8: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x800117CC: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x800117D0: sll         $t9, $s2, 2
    ctx->r25 = S32(ctx->r18 << 2);
    // 0x800117D4: addu        $a3, $t9, $t0
    ctx->r7 = ADD32(ctx->r25, ctx->r8);
    // 0x800117D8: lw          $v0, 0x570($a3)
    ctx->r2 = MEM_W(ctx->r7, 0X570);
    // 0x800117DC: sll         $t1, $s2, 1
    ctx->r9 = S32(ctx->r18 << 1);
    // 0x800117E0: sll         $t3, $t1, 2
    ctx->r11 = S32(ctx->r9 << 2);
    // 0x800117E4: bne         $v0, $zero, L_80011954
    if (ctx->r2 != 0) {
        // 0x800117E8: addiu       $t4, $t3, -0x8
        ctx->r12 = ADD32(ctx->r11, -0X8);
            goto L_80011954;
    }
    // 0x800117E8: addiu       $t4, $t3, -0x8
    ctx->r12 = ADD32(ctx->r11, -0X8);
    // 0x800117EC: lui         $t5, 0x800B
    ctx->r13 = S32(0X800B << 16);
    // 0x800117F0: addiu       $t5, $t5, 0xC4
    ctx->r13 = ADD32(ctx->r13, 0XC4);
    // 0x800117F4: addu        $v0, $t4, $t5
    ctx->r2 = ADD32(ctx->r12, ctx->r13);
    // 0x800117F8: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x800117FC: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    // 0x80011800: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x80011804: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80011808: and         $v1, $v1, $at
    ctx->r3 = ctx->r3 & ctx->r1;
    // 0x8001180C: subu        $s1, $t6, $v1
    ctx->r17 = SUB32(ctx->r14, ctx->r3);
    // 0x80011810: addiu       $s1, $s1, 0x7
    ctx->r17 = ADD32(ctx->r17, 0X7);
    // 0x80011814: addiu       $at, $zero, -0x8
    ctx->r1 = ADD32(0, -0X8);
    // 0x80011818: sw          $t1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r9;
    // 0x8001181C: bltz        $s0, L_8001187C
    if (SIGNED(ctx->r16) < 0) {
        // 0x80011820: and         $s1, $s1, $at
        ctx->r17 = ctx->r17 & ctx->r1;
            goto L_8001187C;
    }
    // 0x80011820: and         $s1, $s1, $at
    ctx->r17 = ctx->r17 & ctx->r1;
    // 0x80011824: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x80011828: sra         $a0, $s0, 24
    ctx->r4 = S32(SIGNED(ctx->r16) >> 24);
    // 0x8001182C: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80011830: and         $s0, $s0, $at
    ctx->r16 = ctx->r16 & ctx->r1;
    // 0x80011834: slti        $at, $s0, 0x9
    ctx->r1 = SIGNED(ctx->r16) < 0X9 ? 1 : 0;
    // 0x80011838: bne         $at, $zero, L_80011864
    if (ctx->r1 != 0) {
        // 0x8001183C: or          $a1, $s1, $zero
        ctx->r5 = ctx->r17 | 0;
            goto L_80011864;
    }
    // 0x8001183C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80011840: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80011844: addiu       $t9, $t9, 0x163C
    ctx->r25 = ADD32(ctx->r25, 0X163C);
    // 0x80011848: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x8001184C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x80011850: jalr        $t9
    // 0x80011854: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80011854: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    after_1:
    // 0x80011858: lw          $a3, 0x34($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X34);
    // 0x8001185C: b           L_8001187C
    // 0x80011860: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
        goto L_8001187C;
    // 0x80011860: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80011864:
    // 0x80011864: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80011868: addiu       $t9, $t9, 0x1514
    ctx->r25 = ADD32(ctx->r25, 0X1514);
    // 0x8001186C: jalr        $t9
    // 0x80011870: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80011870: sw          $a3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r7;
    after_2:
    // 0x80011874: lw          $a3, 0x34($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X34);
    // 0x80011878: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8001187C:
    // 0x8001187C: bne         $s0, $zero, L_8001188C
    if (ctx->r16 != 0) {
        // 0x80011880: sw          $s0, 0x570($a3)
        MEM_W(0X570, ctx->r7) = ctx->r16;
            goto L_8001188C;
    }
    // 0x80011880: sw          $s0, 0x570($a3)
    MEM_W(0X570, ctx->r7) = ctx->r16;
    // 0x80011884: b           L_8001195C
    // 0x80011888: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_8001195C;
    // 0x80011888: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_8001188C:
    // 0x8001188C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80011890: jal         0x800A13E0
    // 0x80011894: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    osInvalICache_recomp(rdram, ctx);
        goto after_3;
    // 0x80011894: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_3:
    // 0x80011898: lw          $t7, 0x30($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X30);
    // 0x8001189C: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x800118A0: addiu       $t1, $t1, 0xF88
    ctx->r9 = ADD32(ctx->r9, 0XF88);
    // 0x800118A4: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x800118A8: addiu       $t0, $t8, -0x8
    ctx->r8 = ADD32(ctx->r24, -0X8);
    // 0x800118AC: addu        $v0, $t0, $t1
    ctx->r2 = ADD32(ctx->r8, ctx->r9);
    // 0x800118B0: lw          $v1, 0x10($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X10);
    // 0x800118B4: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x800118B8: lw          $t2, 0x14($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X14);
    // 0x800118BC: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x800118C0: and         $a2, $v1, $at
    ctx->r6 = ctx->r3 & ctx->r1;
    // 0x800118C4: sll         $t3, $v1, 0
    ctx->r11 = S32(ctx->r3 << 0);
    // 0x800118C8: bgez        $t3, L_80011914
    if (SIGNED(ctx->r11) >= 0) {
        // 0x800118CC: subu        $s1, $t2, $a2
        ctx->r17 = SUB32(ctx->r10, ctx->r6);
            goto L_80011914;
    }
    // 0x800118CC: subu        $s1, $t2, $a2
    ctx->r17 = SUB32(ctx->r10, ctx->r6);
    // 0x800118D0: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x800118D4: addiu       $v0, $v0, 0x1608
    ctx->r2 = ADD32(ctx->r2, 0X1608);
    // 0x800118D8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800118DC: addiu       $a0, $a0, -0x7A14
    ctx->r4 = ADD32(ctx->r4, -0X7A14);
    // 0x800118E0: sw          $s2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r18;
    // 0x800118E4: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x800118E8: jal         0x800048C4
    // 0x800118EC: sw          $a2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r6;
    func_800048C4(rdram, ctx);
        goto after_4;
    // 0x800118EC: sw          $a2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r6;
    after_4:
    // 0x800118F0: lw          $t4, 0x5C($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X5C);
    // 0x800118F4: lw          $a0, 0x50($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X50);
    // 0x800118F8: lw          $a1, 0x4C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X4C);
    // 0x800118FC: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x80011900: or          $a3, $s1, $zero
    ctx->r7 = ctx->r17 | 0;
    // 0x80011904: jal         0x800116BC
    // 0x80011908: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    func_800116BC(rdram, ctx);
        goto after_5;
    // 0x80011908: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    after_5:
    // 0x8001190C: b           L_8001195C
    // 0x80011910: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
        goto L_8001195C;
    // 0x80011910: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80011914:
    // 0x80011914: lui         $t5, 0x800C
    ctx->r13 = S32(0X800C << 16);
    // 0x80011918: lw          $t5, 0x160C($t5)
    ctx->r13 = MEM_W(ctx->r13, 0X160C);
    // 0x8001191C: lw          $a0, 0x50($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X50);
    // 0x80011920: lw          $a1, 0x5C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X5C);
    // 0x80011924: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    // 0x80011928: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    // 0x8001192C: jal         0x80012ED0
    // 0x80011930: sw          $t5, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r13;
    func_80012ED0(rdram, ctx);
        goto after_6;
    // 0x80011930: sw          $t5, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r13;
    after_6:
    // 0x80011934: beq         $v0, $zero, L_80011948
    if (ctx->r2 == 0) {
        // 0x80011938: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_80011948;
    }
    // 0x80011938: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x8001193C: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80011940: jal         0x80013008
    // 0x80011944: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_80013008(rdram, ctx);
        goto after_7;
    // 0x80011944: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_7:
L_80011948:
    // 0x80011948: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x8001194C: b           L_80011958
    // 0x80011950: sw          $s1, 0x160C($at)
    MEM_W(0X160C, ctx->r1) = ctx->r17;
        goto L_80011958;
    // 0x80011950: sw          $s1, 0x160C($at)
    MEM_W(0X160C, ctx->r1) = ctx->r17;
L_80011954:
    // 0x80011954: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80011958:
    // 0x80011958: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_8001195C:
    // 0x8001195C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80011960: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80011964: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80011968: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x8001196C: jr          $ra
    // 0x80011970: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x80011970: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_80011974(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011974: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80011978: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001197C: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80011980: jal         0x80011754
    // 0x80011984: sw          $zero, 0x160C($at)
    MEM_W(0X160C, ctx->r1) = 0;
    func_80011754(rdram, ctx);
        goto after_0;
    // 0x80011984: sw          $zero, 0x160C($at)
    MEM_W(0X160C, ctx->r1) = 0;
    after_0:
    // 0x80011988: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001198C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80011990: jr          $ra
    // 0x80011994: nop

    return;
    // 0x80011994: nop

;}
RECOMP_FUNC void func_80011998(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011998: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001199C: beq         $a1, $zero, L_800119B4
    if (ctx->r5 == 0) {
        // 0x800119A0: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_800119B4;
    }
    // 0x800119A0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800119A4: sll         $t6, $a1, 2
    ctx->r14 = S32(ctx->r5 << 2);
    // 0x800119A8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x800119AC: addu        $at, $at, $t6
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x800119B0: sw          $zero, -0x77D0($at)
    MEM_W(-0X77D0, ctx->r1) = 0;
L_800119B4:
    // 0x800119B4: jal         0x80011974
    // 0x800119B8: nop

    func_80011974(rdram, ctx);
        goto after_0;
    // 0x800119B8: nop

    after_0:
    // 0x800119BC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800119C0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800119C4: jr          $ra
    // 0x800119C8: nop

    return;
    // 0x800119C8: nop

;}
RECOMP_FUNC void func_800119CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800119CC: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800119D0: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x800119D4: or          $s2, $a2, $zero
    ctx->r18 = ctx->r6 | 0;
    // 0x800119D8: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x800119DC: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x800119E0: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800119E4: addiu       $v1, $v1, 0x1604
    ctx->r3 = ADD32(ctx->r3, 0X1604);
    // 0x800119E8: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x800119EC: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x800119F0: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x800119F4: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x800119F8: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x800119FC: sw          $zero, 0x160C($at)
    MEM_W(0X160C, ctx->r1) = 0;
    // 0x80011A00: lui         $s6, 0xFFF
    ctx->r22 = S32(0XFFF << 16);
    // 0x80011A04: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x80011A08: or          $s7, $a1, $zero
    ctx->r23 = ctx->r5 | 0;
    // 0x80011A0C: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80011A10: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80011A14: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x80011A18: sw          $a3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r7;
    // 0x80011A1C: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
    // 0x80011A20: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x80011A24: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x80011A28: ori         $s6, $s6, 0xFFFF
    ctx->r22 = ctx->r22 | 0XFFFF;
    // 0x80011A2C: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x80011A30: lui         $s4, 0x8000
    ctx->r20 = S32(0X8000 << 16);
    // 0x80011A34: lui         $s5, 0x4000
    ctx->r21 = S32(0X4000 << 16);
L_80011A38:
    // 0x80011A38: lw          $a1, 0x0($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X0);
    // 0x80011A3C: addiu       $t7, $zero, -0x1
    ctx->r15 = ADD32(0, -0X1);
    // 0x80011A40: and         $t6, $a1, $s4
    ctx->r14 = ctx->r5 & ctx->r20;
    // 0x80011A44: beq         $t6, $zero, L_80011A58
    if (ctx->r14 == 0) {
        // 0x80011A48: and         $s0, $a1, $s6
        ctx->r16 = ctx->r5 & ctx->r22;
            goto L_80011A58;
    }
    // 0x80011A48: and         $s0, $a1, $s6
    ctx->r16 = ctx->r5 & ctx->r22;
    // 0x80011A4C: addiu       $s3, $zero, -0x1
    ctx->r19 = ADD32(0, -0X1);
    // 0x80011A50: b           L_80011AA4
    // 0x80011A54: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
        goto L_80011AA4;
    // 0x80011A54: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
L_80011A58:
    // 0x80011A58: sll         $t8, $s0, 2
    ctx->r24 = S32(ctx->r16 << 2);
    // 0x80011A5C: addu        $t9, $a2, $t8
    ctx->r25 = ADD32(ctx->r6, ctx->r24);
    // 0x80011A60: lw          $v0, 0x570($t9)
    ctx->r2 = MEM_W(ctx->r25, 0X570);
    // 0x80011A64: bne         $v0, $zero, L_80011A70
    if (ctx->r2 != 0) {
        // 0x80011A68: nop
    
            goto L_80011A70;
    }
    // 0x80011A68: nop

    // 0x80011A6C: or          $s3, $a1, $zero
    ctx->r19 = ctx->r5 | 0;
L_80011A70:
    // 0x80011A70: beql        $v0, $zero, L_80011AA4
    if (ctx->r2 == 0) {
        // 0x80011A74: sw          $s0, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r16;
            goto L_80011AA4;
    }
    goto skip_0;
    // 0x80011A74: sw          $s0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r16;
    skip_0:
    // 0x80011A78: bgtz        $s3, L_80011AA4
    if (SIGNED(ctx->r19) > 0) {
        // 0x80011A7C: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_80011AA4;
    }
    // 0x80011A7C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80011A80: addiu       $a0, $a0, -0x7A14
    ctx->r4 = ADD32(ctx->r4, -0X7A14);
    // 0x80011A84: jal         0x800048A0
    // 0x80011A88: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_800048A0(rdram, ctx);
        goto after_0;
    // 0x80011A88: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_0:
    // 0x80011A8C: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80011A90: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x80011A94: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x80011A98: beq         $v0, $zero, L_80011AA4
    if (ctx->r2 == 0) {
        // 0x80011A9C: addiu       $v1, $v1, 0x1604
        ctx->r3 = ADD32(ctx->r3, 0X1604);
            goto L_80011AA4;
    }
    // 0x80011A9C: addiu       $v1, $v1, 0x1604
    ctx->r3 = ADD32(ctx->r3, 0X1604);
    // 0x80011AA0: sw          $s0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r16;
L_80011AA4:
    // 0x80011AA4: lw          $t0, 0x0($s1)
    ctx->r8 = MEM_W(ctx->r17, 0X0);
    // 0x80011AA8: and         $t1, $t0, $s5
    ctx->r9 = ctx->r8 & ctx->r21;
    // 0x80011AAC: bnel        $t1, $zero, L_80011AC0
    if (ctx->r9 != 0) {
        // 0x80011AB0: lw          $t2, 0x0($v1)
        ctx->r10 = MEM_W(ctx->r3, 0X0);
            goto L_80011AC0;
    }
    goto skip_1;
    // 0x80011AB0: lw          $t2, 0x0($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X0);
    skip_1:
    // 0x80011AB4: b           L_80011A38
    // 0x80011AB8: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
        goto L_80011A38;
    // 0x80011AB8: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
    // 0x80011ABC: lw          $t2, 0x0($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X0);
L_80011AC0:
    // 0x80011AC0: lw          $t3, 0x54($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X54);
    // 0x80011AC4: bgtz        $t2, L_80011AF8
    if (SIGNED(ctx->r10) > 0) {
        // 0x80011AC8: nop
    
            goto L_80011AF8;
    }
    // 0x80011AC8: nop

    // 0x80011ACC: beq         $t3, $zero, L_80011AD8
    if (ctx->r11 == 0) {
        // 0x80011AD0: lui         $t4, 0x8000
        ctx->r12 = S32(0X8000 << 16);
            goto L_80011AD8;
    }
    // 0x80011AD0: lui         $t4, 0x8000
    ctx->r12 = S32(0X8000 << 16);
    // 0x80011AD4: sw          $t4, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r12;
L_80011AD8:
    // 0x80011AD8: bnel        $s3, $zero, L_80011B30
    if (ctx->r19 != 0) {
        // 0x80011ADC: or          $s1, $s7, $zero
        ctx->r17 = ctx->r23 | 0;
            goto L_80011B30;
    }
    goto skip_2;
    // 0x80011ADC: or          $s1, $s7, $zero
    ctx->r17 = ctx->r23 | 0;
    skip_2:
    // 0x80011AE0: lw          $t5, 0x0($s7)
    ctx->r13 = MEM_W(ctx->r23, 0X0);
    // 0x80011AE4: and         $t6, $t5, $s6
    ctx->r14 = ctx->r13 & ctx->r22;
    // 0x80011AE8: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80011AEC: addu        $t8, $a2, $t7
    ctx->r24 = ADD32(ctx->r6, ctx->r15);
    // 0x80011AF0: b           L_80011BE4
    // 0x80011AF4: lw          $v0, 0x570($t8)
    ctx->r2 = MEM_W(ctx->r24, 0X570);
        goto L_80011BE4;
    // 0x80011AF4: lw          $v0, 0x570($t8)
    ctx->r2 = MEM_W(ctx->r24, 0X570);
L_80011AF8:
    // 0x80011AF8: bgtzl       $s3, L_80011B30
    if (SIGNED(ctx->r19) > 0) {
        // 0x80011AFC: or          $s1, $s7, $zero
        ctx->r17 = ctx->r23 | 0;
            goto L_80011B30;
    }
    goto skip_3;
    // 0x80011AFC: or          $s1, $s7, $zero
    ctx->r17 = ctx->r23 | 0;
    skip_3:
    // 0x80011B00: lw          $t9, 0x54($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X54);
    // 0x80011B04: beq         $t9, $zero, L_80011B10
    if (ctx->r25 == 0) {
        // 0x80011B08: nop
    
            goto L_80011B10;
    }
    // 0x80011B08: nop

    // 0x80011B0C: sw          $zero, 0x0($t9)
    MEM_W(0X0, ctx->r25) = 0;
L_80011B10:
    // 0x80011B10: beql        $s2, $zero, L_80011B24
    if (ctx->r18 == 0) {
        // 0x80011B14: lw          $v1, 0x4($s7)
        ctx->r3 = MEM_W(ctx->r23, 0X4);
            goto L_80011B24;
    }
    goto skip_4;
    // 0x80011B14: lw          $v1, 0x4($s7)
    ctx->r3 = MEM_W(ctx->r23, 0X4);
    skip_4:
    // 0x80011B18: b           L_80011BE4
    // 0x80011B1C: or          $v0, $s2, $zero
    ctx->r2 = ctx->r18 | 0;
        goto L_80011BE4;
    // 0x80011B1C: or          $v0, $s2, $zero
    ctx->r2 = ctx->r18 | 0;
    // 0x80011B20: lw          $v1, 0x4($s7)
    ctx->r3 = MEM_W(ctx->r23, 0X4);
L_80011B24:
    // 0x80011B24: b           L_80011BE4
    // 0x80011B28: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
        goto L_80011BE4;
    // 0x80011B28: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80011B2C: or          $s1, $s7, $zero
    ctx->r17 = ctx->r23 | 0;
L_80011B30:
    // 0x80011B30: beq         $s2, $zero, L_80011B40
    if (ctx->r18 == 0) {
        // 0x80011B34: lw          $a0, 0x48($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X48);
            goto L_80011B40;
    }
    // 0x80011B34: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X48);
    // 0x80011B38: b           L_80011B44
    // 0x80011B3C: or          $a2, $s2, $zero
    ctx->r6 = ctx->r18 | 0;
        goto L_80011B44;
    // 0x80011B3C: or          $a2, $s2, $zero
    ctx->r6 = ctx->r18 | 0;
L_80011B40:
    // 0x80011B40: lw          $a2, 0x4($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X4);
L_80011B44:
    // 0x80011B44: lw          $a1, 0x0($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X0);
    // 0x80011B48: and         $t0, $a1, $s4
    ctx->r8 = ctx->r5 & ctx->r20;
    // 0x80011B4C: bne         $t0, $zero, L_80011B70
    if (ctx->r8 != 0) {
        // 0x80011B50: nop
    
            goto L_80011B70;
    }
    // 0x80011B50: nop

    // 0x80011B54: bne         $s3, $a1, L_80011B70
    if (ctx->r19 != ctx->r5) {
        // 0x80011B58: nop
    
            goto L_80011B70;
    }
    // 0x80011B58: nop

    // 0x80011B5C: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X48);
    // 0x80011B60: jal         0x80011754
    // 0x80011B64: lw          $a3, 0x54($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X54);
    func_80011754(rdram, ctx);
        goto after_1;
    // 0x80011B64: lw          $a3, 0x54($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X54);
    after_1:
    // 0x80011B68: b           L_80011BE4
    // 0x80011B6C: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
        goto L_80011BE4;
    // 0x80011B6C: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
L_80011B70:
    // 0x80011B70: jal         0x80011754
    // 0x80011B74: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    func_80011754(rdram, ctx);
        goto after_2;
    // 0x80011B74: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_2:
    // 0x80011B78: bne         $s1, $s7, L_80011B84
    if (ctx->r17 != ctx->r23) {
        // 0x80011B7C: lui         $t3, 0x800B
        ctx->r11 = S32(0X800B << 16);
            goto L_80011B84;
    }
    // 0x80011B7C: lui         $t3, 0x800B
    ctx->r11 = S32(0X800B << 16);
    // 0x80011B80: sw          $v0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r2;
L_80011B84:
    // 0x80011B84: bgezl       $s2, L_80011BCC
    if (SIGNED(ctx->r18) >= 0) {
        // 0x80011B88: lw          $t6, 0x0($s1)
        ctx->r14 = MEM_W(ctx->r17, 0X0);
            goto L_80011BCC;
    }
    goto skip_5;
    // 0x80011B88: lw          $t6, 0x0($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X0);
    skip_5:
    // 0x80011B8C: lw          $t1, 0x0($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X0);
    // 0x80011B90: addiu       $t3, $t3, 0xC4
    ctx->r11 = ADD32(ctx->r11, 0XC4);
    // 0x80011B94: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x80011B98: and         $t2, $t1, $s6
    ctx->r10 = ctx->r9 & ctx->r22;
    // 0x80011B9C: sll         $t4, $t2, 3
    ctx->r12 = S32(ctx->r10 << 3);
    // 0x80011BA0: addu        $v0, $t4, $t3
    ctx->r2 = ADD32(ctx->r12, ctx->r11);
    // 0x80011BA4: lw          $v1, -0x8($v0)
    ctx->r3 = MEM_W(ctx->r2, -0X8);
    // 0x80011BA8: lw          $t5, -0x4($v0)
    ctx->r13 = MEM_W(ctx->r2, -0X4);
    // 0x80011BAC: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80011BB0: and         $v1, $v1, $at
    ctx->r3 = ctx->r3 & ctx->r1;
    // 0x80011BB4: subu        $a0, $t5, $v1
    ctx->r4 = SUB32(ctx->r13, ctx->r3);
    // 0x80011BB8: addiu       $a0, $a0, 0x7
    ctx->r4 = ADD32(ctx->r4, 0X7);
    // 0x80011BBC: addiu       $at, $zero, -0x8
    ctx->r1 = ADD32(0, -0X8);
    // 0x80011BC0: and         $a0, $a0, $at
    ctx->r4 = ctx->r4 & ctx->r1;
    // 0x80011BC4: addu        $s2, $s2, $a0
    ctx->r18 = ADD32(ctx->r18, ctx->r4);
    // 0x80011BC8: lw          $t6, 0x0($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X0);
L_80011BCC:
    // 0x80011BCC: and         $t7, $t6, $s5
    ctx->r15 = ctx->r14 & ctx->r21;
    // 0x80011BD0: bnel        $t7, $zero, L_80011BE4
    if (ctx->r15 != 0) {
        // 0x80011BD4: lw          $v0, 0x38($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X38);
            goto L_80011BE4;
    }
    goto skip_6;
    // 0x80011BD4: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
    skip_6:
    // 0x80011BD8: b           L_80011B30
    // 0x80011BDC: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
        goto L_80011B30;
    // 0x80011BDC: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
    // 0x80011BE0: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
L_80011BE4:
    // 0x80011BE4: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80011BE8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80011BEC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80011BF0: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80011BF4: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80011BF8: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x80011BFC: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x80011C00: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x80011C04: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x80011C08: jr          $ra
    // 0x80011C0C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x80011C0C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_80011C10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011C10: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80011C14: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80011C18: lw          $t7, 0x808($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X808);
    // 0x80011C1C: addiu       $t6, $zero, 0x800
    ctx->r14 = ADD32(0, 0X800);
    // 0x80011C20: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80011C24: addiu       $t8, $t7, -0x800
    ctx->r24 = ADD32(ctx->r15, -0X800);
    // 0x80011C28: sw          $t6, 0x810($a0)
    MEM_W(0X810, ctx->r4) = ctx->r14;
    // 0x80011C2C: bgez        $t8, L_80011C4C
    if (SIGNED(ctx->r24) >= 0) {
        // 0x80011C30: sw          $t8, 0x808($a0)
        MEM_W(0X808, ctx->r4) = ctx->r24;
            goto L_80011C4C;
    }
    // 0x80011C30: sw          $t8, 0x808($a0)
    MEM_W(0X808, ctx->r4) = ctx->r24;
    // 0x80011C34: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x80011C38: addiu       $at, $zero, -0x2
    ctx->r1 = ADD32(0, -0X2);
    // 0x80011C3C: and         $t1, $t0, $at
    ctx->r9 = ctx->r8 & ctx->r1;
    // 0x80011C40: addu        $t2, $t6, $t1
    ctx->r10 = ADD32(ctx->r14, ctx->r9);
    // 0x80011C44: sw          $t2, 0x810($a0)
    MEM_W(0X810, ctx->r4) = ctx->r10;
    // 0x80011C48: sw          $zero, 0x808($a0)
    MEM_W(0X808, ctx->r4) = 0;
L_80011C4C:
    // 0x80011C4C: lw          $v0, 0x810($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X810);
    // 0x80011C50: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80011C54: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80011C58: blezl       $v0, L_80011CA0
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80011C5C: lw          $t8, 0x800($a1)
        ctx->r24 = MEM_W(ctx->r5, 0X800);
            goto L_80011CA0;
    }
    goto skip_0;
    // 0x80011C5C: lw          $t8, 0x800($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X800);
    skip_0:
    // 0x80011C60: lw          $t3, 0x834($a1)
    ctx->r11 = MEM_W(ctx->r5, 0X834);
    // 0x80011C64: lui         $t6, 0x800D
    ctx->r14 = S32(0X800D << 16);
    // 0x80011C68: addiu       $t6, $t6, -0x1898
    ctx->r14 = ADD32(ctx->r14, -0X1898);
    // 0x80011C6C: or          $t4, $t3, $at
    ctx->r12 = ctx->r11 | ctx->r1;
    // 0x80011C70: sw          $t4, 0x834($a1)
    MEM_W(0X834, ctx->r5) = ctx->r12;
    // 0x80011C74: sb          $t5, 0x0($t6)
    MEM_B(0X0, ctx->r14) = ctx->r13;
    // 0x80011C78: lw          $a2, 0x810($a1)
    ctx->r6 = MEM_W(ctx->r5, 0X810);
    // 0x80011C7C: lw          $a0, 0x800($a1)
    ctx->r4 = MEM_W(ctx->r5, 0X800);
    // 0x80011C80: jal         0x8001A42C
    // 0x80011C84: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    func_8001A42C(rdram, ctx);
        goto after_0;
    // 0x80011C84: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    after_0:
    // 0x80011C88: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x80011C8C: lui         $t7, 0x800D
    ctx->r15 = S32(0X800D << 16);
    // 0x80011C90: addiu       $t7, $t7, -0x1898
    ctx->r15 = ADD32(ctx->r15, -0X1898);
    // 0x80011C94: sb          $zero, 0x0($t7)
    MEM_B(0X0, ctx->r15) = 0;
    // 0x80011C98: lw          $v0, 0x810($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X810);
    // 0x80011C9C: lw          $t8, 0x800($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X800);
L_80011CA0:
    // 0x80011CA0: addu        $t0, $t8, $v0
    ctx->r8 = ADD32(ctx->r24, ctx->r2);
    // 0x80011CA4: sw          $t0, 0x800($a1)
    MEM_W(0X800, ctx->r5) = ctx->r8;
    // 0x80011CA8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80011CAC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80011CB0: jr          $ra
    // 0x80011CB4: nop

    return;
    // 0x80011CB4: nop

;}
RECOMP_FUNC void func_80011CB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011CB8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80011CBC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80011CC0: lw          $t6, 0x804($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X804);
    // 0x80011CC4: lw          $t8, 0x810($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X810);
    // 0x80011CC8: lw          $t0, 0x80C($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X80C);
    // 0x80011CCC: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80011CD0: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x80011CD4: addiu       $t1, $t0, -0x1
    ctx->r9 = ADD32(ctx->r8, -0X1);
    // 0x80011CD8: sw          $t7, 0x804($a0)
    MEM_W(0X804, ctx->r4) = ctx->r15;
    // 0x80011CDC: sw          $t9, 0x810($a0)
    MEM_W(0X810, ctx->r4) = ctx->r25;
    // 0x80011CE0: beq         $t9, $zero, L_80011CF8
    if (ctx->r25 == 0) {
        // 0x80011CE4: sw          $t1, 0x80C($a0)
        MEM_W(0X80C, ctx->r4) = ctx->r9;
            goto L_80011CF8;
    }
    // 0x80011CE4: sw          $t1, 0x80C($a0)
    MEM_W(0X80C, ctx->r4) = ctx->r9;
    // 0x80011CE8: lw          $t3, 0x818($a0)
    ctx->r11 = MEM_W(ctx->r4, 0X818);
    // 0x80011CEC: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x80011CF0: b           L_80011D00
    // 0x80011CF4: sw          $t4, 0x818($a0)
    MEM_W(0X818, ctx->r4) = ctx->r12;
        goto L_80011D00;
    // 0x80011CF4: sw          $t4, 0x818($a0)
    MEM_W(0X818, ctx->r4) = ctx->r12;
L_80011CF8:
    // 0x80011CF8: jal         0x80011C10
    // 0x80011CFC: sw          $a0, 0x818($a0)
    MEM_W(0X818, ctx->r4) = ctx->r4;
    func_80011C10(rdram, ctx);
        goto after_0;
    // 0x80011CFC: sw          $a0, 0x818($a0)
    MEM_W(0X818, ctx->r4) = ctx->r4;
    after_0:
L_80011D00:
    // 0x80011D00: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80011D04: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80011D08: jr          $ra
    // 0x80011D0C: nop

    return;
    // 0x80011D0C: nop

;}
RECOMP_FUNC void func_80011D10(uint8_t* rdram, recomp_context* ctx) {
    {
        static int dm_log = 0; dm_log++;
        if (dm_log <= 10 || (dm_log % 500 == 0)) {
            int16_t fe = (int16_t)MEM_H(ctx->r4, 0xE);
            uint8_t st = MEM_BU(ctx->r4, 0x9);
            fprintf(stderr, "[DMAMgr] #%d field_e=%d state=%d 0x800C1604=0x%08X\n",
                    dm_log, fe, st, (uint32_t)MEM_W(S32(0x800C1604), 0));
        }
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011D10: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80011D14: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80011D18: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80011D1C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80011D20: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80011D24: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80011D28: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80011D2C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80011D30: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80011D34: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80011D38: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80011D3C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80011D40: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80011D44: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80011D48: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80011D4C: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80011D50: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80011D54: lw          $t9, -0xAA0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0XAA0);
    // 0x80011D58: jalr        $t9
    // 0x80011D5C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80011D5C: nop

    after_0:
    // 0x80011D60: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80011D64: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80011D68: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80011D6C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80011D70: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80011D74: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80011D78: jr          $ra
    // 0x80011D7C: nop

    return;
    // 0x80011D7C: nop

;}
RECOMP_FUNC void func_80011D80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011D80: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80011D84: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80011D88: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80011D8C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80011D90: addiu       $t9, $t9, 0x27B0
    ctx->r25 = ADD32(ctx->r25, 0X27B0);
    // 0x80011D94: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80011D98: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80011D9C: addiu       $a2, $zero, 0x988
    ctx->r6 = ADD32(0, 0X988);
    // 0x80011DA0: jalr        $t9
    // 0x80011DA4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80011DA4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x80011DA8: bne         $v0, $zero, L_80011DC8
    if (ctx->r2 != 0) {
        // 0x80011DAC: lui         $at, 0x800C
        ctx->r1 = S32(0X800C << 16);
            goto L_80011DC8;
    }
    // 0x80011DAC: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80011DB0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80011DB4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80011DB8: jalr        $t9
    // 0x80011DBC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80011DBC: nop

    after_1:
    // 0x80011DC0: b           L_80011E38
    // 0x80011DC4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80011E38;
    // 0x80011DC4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80011DC8:
    // 0x80011DC8: lw          $v0, 0x34($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X34);
    // 0x80011DCC: sw          $s0, 0x1600($at)
    MEM_W(0X1600, ctx->r1) = ctx->r16;
    // 0x80011DD0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80011DD4: addiu       $t6, $zero, 0x800
    ctx->r14 = ADD32(0, 0X800);
    // 0x80011DD8: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80011DDC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80011DE0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80011DE4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x80011DE8: sw          $t6, 0x838($v0)
    MEM_W(0X838, ctx->r2) = ctx->r14;
    // 0x80011DEC: sh          $zero, 0x844($v0)
    MEM_H(0X844, ctx->r2) = 0;
    // 0x80011DF0: jalr        $t9
    // 0x80011DF4: sh          $zero, 0x846($v0)
    MEM_H(0X846, ctx->r2) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80011DF4: sh          $zero, 0x846($v0)
    MEM_H(0X846, ctx->r2) = 0;
    after_2:
    // 0x80011DF8: lh          $t8, 0xE($s0)
    ctx->r24 = MEM_H(ctx->r16, 0XE);
    // 0x80011DFC: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80011E00: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80011E04: sll         $t0, $t8, 1
    ctx->r8 = S32(ctx->r24 << 1);
    // 0x80011E08: addu        $v0, $s0, $t0
    ctx->r2 = ADD32(ctx->r16, ctx->r8);
    // 0x80011E0C: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x80011E10: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x80011E14: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80011E18: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80011E1C: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80011E20: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x80011E24: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
    // 0x80011E28: lw          $t9, -0xAA0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0XAA0);
    // 0x80011E2C: jalr        $t9
    // 0x80011E30: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80011E30: nop

    after_3:
    // 0x80011E34: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80011E38:
    // 0x80011E38: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80011E3C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80011E40: jr          $ra
    // 0x80011E44: nop

    return;
    // 0x80011E44: nop

;}
RECOMP_FUNC void func_80011E48(uint8_t* rdram, recomp_context* ctx) {
    {
        static int e48_log = 0; e48_log++;
        if (e48_log <= 5 || (e48_log % 500 == 0)) {
            uint32_t ctx_ptr = (uint32_t)MEM_W(ctx->r4, 0x34);
            uint16_t pending = (ctx_ptr != 0) ? (uint16_t)MEM_HU(S32(ctx_ptr), 0x846) : 0;
            fprintf(stderr, "[DMAMgr_state1] #%d ctx=0x%08X pending=%d\n",
                    e48_log, ctx_ptr, pending);
        }
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80011E48: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80011E4C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80011E50: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80011E54: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80011E58: addiu       $t6, $zero, 0x0
    ctx->r14 = ADD32(0, 0X0);
    // 0x80011E5C: addiu       $t7, $zero, 0x0
    ctx->r15 = ADD32(0, 0X0);
    // 0x80011E60: sw          $t7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r15;
    // 0x80011E64: sw          $t6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r14;
    // 0x80011E68: lhu         $t8, 0x846($v1)
    ctx->r24 = MEM_HU(ctx->r3, 0X846);
    // 0x80011E6C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80011E70: beql        $t8, $zero, L_80011FAC
    if (ctx->r24 == 0) {
        // 0x80011E74: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80011FAC;
    }
    goto skip_0;
    // 0x80011E74: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80011E78: lhu         $t9, 0x844($v1)
    ctx->r25 = MEM_HU(ctx->r3, 0X844);
    // 0x80011E7C: lui         $t6, 0x800D
    ctx->r14 = S32(0X800D << 16);
    // 0x80011E80: addiu       $t6, $t6, -0x1898
    ctx->r14 = ADD32(ctx->r14, -0X1898);
    // 0x80011E84: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80011E88: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x80011E8C: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x80011E90: addu        $v0, $v1, $t0
    ctx->r2 = ADD32(ctx->r3, ctx->r8);
    // 0x80011E94: lw          $t1, 0x848($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X848);
    // 0x80011E98: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80011E9C: addiu       $v0, $v0, 0x848
    ctx->r2 = ADD32(ctx->r2, 0X848);
    // 0x80011EA0: sw          $t1, 0x828($v1)
    MEM_W(0X828, ctx->r3) = ctx->r9;
    // 0x80011EA4: lw          $t2, 0x4($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X4);
    // 0x80011EA8: addiu       $a1, $sp, 0x30
    ctx->r5 = ADD32(ctx->r29, 0X30);
    // 0x80011EAC: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x80011EB0: sw          $t2, 0x82C($v1)
    MEM_W(0X82C, ctx->r3) = ctx->r10;
    // 0x80011EB4: lw          $t3, 0x8($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X8);
    // 0x80011EB8: sw          $t3, 0x830($v1)
    MEM_W(0X830, ctx->r3) = ctx->r11;
    // 0x80011EBC: lw          $t4, 0xC($v0)
    ctx->r12 = MEM_W(ctx->r2, 0XC);
    // 0x80011EC0: sw          $t4, 0x840($v1)
    MEM_W(0X840, ctx->r3) = ctx->r12;
    // 0x80011EC4: sb          $t5, 0x0($t6)
    MEM_B(0X0, ctx->r14) = ctx->r13;
    // 0x80011EC8: jal         0x8001A42C
    // 0x80011ECC: lw          $a0, 0x828($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X828);
    func_8001A42C(rdram, ctx);
        goto after_0;
    // 0x80011ECC: lw          $a0, 0x828($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X828);
    after_0:
    // 0x80011ED0: lui         $t7, 0x800D
    ctx->r15 = S32(0X800D << 16);
    // 0x80011ED4: addiu       $t7, $t7, -0x1898
    ctx->r15 = ADD32(ctx->r15, -0X1898);
    // 0x80011ED8: sb          $zero, 0x0($t7)
    MEM_B(0X0, ctx->r15) = 0;
    // 0x80011EDC: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    // 0x80011EE0: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x80011EE4: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x80011EE8: jal         0x800977F0
    // 0x80011EEC: addiu       $a3, $zero, 0x20
    ctx->r7 = ADD32(0, 0X20);
    __ll_rshift_recomp(rdram, ctx);
        goto after_1;
    // 0x80011EEC: addiu       $a3, $zero, 0x20
    ctx->r7 = ADD32(0, 0X20);
    after_1:
    // 0x80011EF0: sll         $t9, $v1, 0
    ctx->r25 = S32(ctx->r3 << 0);
    // 0x80011EF4: bgez        $t9, L_80011F58
    if (SIGNED(ctx->r25) >= 0) {
        // 0x80011EF8: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_80011F58;
    }
    // 0x80011EF8: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80011EFC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80011F00: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80011F04: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x80011F08: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80011F0C: jalr        $t9
    // 0x80011F10: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80011F10: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_2:
    // 0x80011F14: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x80011F18: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80011F1C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80011F20: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x80011F24: addu        $v0, $s0, $t1
    ctx->r2 = ADD32(ctx->r16, ctx->r9);
    // 0x80011F28: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x80011F2C: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x80011F30: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80011F34: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x80011F38: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80011F3C: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x80011F40: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x80011F44: lw          $t9, -0xAA0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0XAA0);
    // 0x80011F48: jalr        $t9
    // 0x80011F4C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80011F4C: nop

    after_3:
    // 0x80011F50: b           L_80011FAC
    // 0x80011F54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80011FAC;
    // 0x80011F54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80011F58:
    // 0x80011F58: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80011F5C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x80011F60: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80011F64: jalr        $t9
    // 0x80011F68: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80011F68: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_4:
    // 0x80011F6C: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x80011F70: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80011F74: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80011F78: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80011F7C: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x80011F80: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x80011F84: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x80011F88: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80011F8C: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80011F90: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x80011F94: sb          $t0, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r8;
    // 0x80011F98: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
    // 0x80011F9C: lw          $t9, -0xAA0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0XAA0);
    // 0x80011FA0: jalr        $t9
    // 0x80011FA4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80011FA4: nop

    after_5:
    // 0x80011FA8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80011FAC:
    // 0x80011FAC: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80011FB0: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x80011FB4: jr          $ra
    // 0x80011FB8: nop

    return;
    // 0x80011FB8: nop

;}
