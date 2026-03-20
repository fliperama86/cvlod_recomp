#include "recomp.h"
#include "funcs.h"

RECOMP_FUNC void func_8003F22C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003F22C: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8003F230: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8003F234: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8003F238: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8003F23C: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8003F240: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8003F244: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x8003F248: or          $s6, $a2, $zero
    ctx->r22 = ctx->r6 | 0;
    // 0x8003F24C: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x8003F250: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x8003F254: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x8003F258: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8003F25C: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8003F260: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8003F264: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    // 0x8003F268: jal         0x80000F30
    // 0x8003F26C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8003F26C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_0:
    // 0x8003F270: lw          $t6, 0x0($s6)
    ctx->r14 = MEM_W(ctx->r22, 0X0);
    // 0x8003F274: lui         $at, 0x200
    ctx->r1 = S32(0X200 << 16);
    // 0x8003F278: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x8003F27C: or          $t7, $t6, $at
    ctx->r15 = ctx->r14 | ctx->r1;
    // 0x8003F280: sw          $t7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r15;
    // 0x8003F284: lw          $t8, -0x1134($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X1134);
    // 0x8003F288: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8003F28C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x8003F290: beql        $t8, $zero, L_8003F420
    if (ctx->r24 == 0) {
        // 0x8003F294: lw          $ra, 0x3C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X3C);
            goto L_8003F420;
    }
    goto skip_0;
    // 0x8003F294: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    skip_0:
    // 0x8003F298: jal         0x801682D0
    // 0x8003F29C: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    func_801682D0(rdram, ctx);
        goto after_1;
    // 0x8003F29C: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_1:
    // 0x8003F2A0: beq         $v0, $zero, L_8003F41C
    if (ctx->r2 == 0) {
        // 0x8003F2A4: or          $s7, $v0, $zero
        ctx->r23 = ctx->r2 | 0;
            goto L_8003F41C;
    }
    // 0x8003F2A4: or          $s7, $v0, $zero
    ctx->r23 = ctx->r2 | 0;
    // 0x8003F2A8: or          $s4, $zero, $zero
    ctx->r20 = 0 | 0;
    // 0x8003F2AC: addiu       $fp, $zero, 0x2
    ctx->r30 = ADD32(0, 0X2);
L_8003F2B0:
    // 0x8003F2B0: bne         $s4, $zero, L_8003F2C0
    if (ctx->r20 != 0) {
        // 0x8003F2B4: addiu       $s1, $s7, 0x61C
        ctx->r17 = ADD32(ctx->r23, 0X61C);
            goto L_8003F2C0;
    }
    // 0x8003F2B4: addiu       $s1, $s7, 0x61C
    ctx->r17 = ADD32(ctx->r23, 0X61C);
    // 0x8003F2B8: b           L_8003F2C0
    // 0x8003F2BC: addiu       $s1, $s7, 0x3C0
    ctx->r17 = ADD32(ctx->r23, 0X3C0);
        goto L_8003F2C0;
    // 0x8003F2BC: addiu       $s1, $s7, 0x3C0
    ctx->r17 = ADD32(ctx->r23, 0X3C0);
L_8003F2C0:
    // 0x8003F2C0: lh          $t9, 0x60($s1)
    ctx->r25 = MEM_H(ctx->r17, 0X60);
    // 0x8003F2C4: lui         $s3, 0x3280
    ctx->r19 = S32(0X3280 << 16);
    // 0x8003F2C8: ori         $s3, $s3, 0x8000
    ctx->r19 = ctx->r19 | 0X8000;
    // 0x8003F2CC: beq         $t9, $zero, L_8003F3CC
    if (ctx->r25 == 0) {
        // 0x8003F2D0: lui         $s5, 0x8000
        ctx->r21 = S32(0X8000 << 16);
            goto L_8003F3CC;
    }
    // 0x8003F2D0: lui         $s5, 0x8000
    ctx->r21 = S32(0X8000 << 16);
    // 0x8003F2D4: bne         $s4, $zero, L_8003F2E4
    if (ctx->r20 != 0) {
        // 0x8003F2D8: addiu       $s5, $s5, 0x1090
        ctx->r21 = ADD32(ctx->r21, 0X1090);
            goto L_8003F2E4;
    }
    // 0x8003F2D8: addiu       $s5, $s5, 0x1090
    ctx->r21 = ADD32(ctx->r21, 0X1090);
    // 0x8003F2DC: b           L_8003F2E8
    // 0x8003F2E0: addiu       $s0, $s6, 0x4
    ctx->r16 = ADD32(ctx->r22, 0X4);
        goto L_8003F2E8;
    // 0x8003F2E0: addiu       $s0, $s6, 0x4
    ctx->r16 = ADD32(ctx->r22, 0X4);
L_8003F2E4:
    // 0x8003F2E4: addiu       $s0, $s6, 0xC8
    ctx->r16 = ADD32(ctx->r22, 0XC8);
L_8003F2E8:
    // 0x8003F2E8: lwc1        $f4, 0x18($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X18);
    // 0x8003F2EC: addiu       $s2, $s0, 0x10
    ctx->r18 = ADD32(ctx->r16, 0X10);
    // 0x8003F2F0: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8003F2F4: swc1        $f4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f4.u32l;
    // 0x8003F2F8: lwc1        $f6, 0x0($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X0);
    // 0x8003F2FC: swc1        $f6, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f6.u32l;
    // 0x8003F300: lwc1        $f8, 0x4($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X4);
    // 0x8003F304: swc1        $f8, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->f8.u32l;
    // 0x8003F308: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x8003F30C: swc1        $f10, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->f10.u32l;
    // 0x8003F310: lh          $a2, 0x5C($s1)
    ctx->r6 = MEM_H(ctx->r17, 0X5C);
    // 0x8003F314: bne         $a2, $zero, L_8003F33C
    if (ctx->r6 != 0) {
        // 0x8003F318: nop
    
            goto L_8003F33C;
    }
    // 0x8003F318: nop

    // 0x8003F31C: lwc1        $f16, 0x48($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X48);
    // 0x8003F320: addiu       $s2, $s0, 0x10
    ctx->r18 = ADD32(ctx->r16, 0X10);
    // 0x8003F324: swc1        $f16, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f16.u32l;
    // 0x8003F328: lwc1        $f18, 0x4C($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X4C);
    // 0x8003F32C: swc1        $f18, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f18.u32l;
    // 0x8003F330: lwc1        $f4, 0x50($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X50);
    // 0x8003F334: b           L_8003F344
    // 0x8003F338: swc1        $f4, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f4.u32l;
        goto L_8003F344;
    // 0x8003F338: swc1        $f4, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f4.u32l;
L_8003F33C:
    // 0x8003F33C: jal         0x8003FA10
    // 0x8003F340: addiu       $a1, $s1, 0x48
    ctx->r5 = ADD32(ctx->r17, 0X48);
    func_8003FA10(rdram, ctx);
        goto after_2;
    // 0x8003F340: addiu       $a1, $s1, 0x48
    ctx->r5 = ADD32(ctx->r17, 0X48);
    after_2:
L_8003F344:
    // 0x8003F344: lwc1        $f6, 0x10($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X10);
    // 0x8003F348: lwc1        $f8, 0x3C($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X3C);
    // 0x8003F34C: lwc1        $f16, 0x14($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X14);
    // 0x8003F350: lwc1        $f18, 0x40($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X40);
    // 0x8003F354: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8003F358: lwc1        $f8, 0x18($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8003F35C: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8003F360: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x8003F364: lwc1        $f16, 0x44($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X44);
    // 0x8003F368: addiu       $a1, $s0, 0x20
    ctx->r5 = ADD32(ctx->r16, 0X20);
    // 0x8003F36C: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x8003F370: mul.s       $f18, $f8, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f8.fl, ctx->f16.fl);
    // 0x8003F374: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8003F378: add.s       $f10, $f6, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f18.fl;
    // 0x8003F37C: neg.s       $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = -ctx->f10.fl;
    // 0x8003F380: jalr        $s5
    // 0x8003F384: swc1        $f4, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_3;
    // 0x8003F384: swc1        $f4, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f4.u32l;
    after_3:
    // 0x8003F388: addiu       $v0, $s0, 0x48
    ctx->r2 = ADD32(ctx->r16, 0X48);
    // 0x8003F38C: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
    // 0x8003F390: lw          $t0, 0x44($s1)
    ctx->r8 = MEM_W(ctx->r17, 0X44);
    // 0x8003F394: lbu         $t1, 0x2B($t0)
    ctx->r9 = MEM_BU(ctx->r8, 0X2B);
    // 0x8003F398: addiu       $t2, $t1, 0x400
    ctx->r10 = ADD32(ctx->r9, 0X400);
    // 0x8003F39C: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
    // 0x8003F3A0: lw          $t3, 0x44($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X44);
    // 0x8003F3A4: sw          $t3, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r11;
    // 0x8003F3A8: lw          $t4, 0x58($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X58);
    // 0x8003F3AC: beq         $t4, $zero, L_8003F3B8
    if (ctx->r12 == 0) {
        // 0x8003F3B0: sw          $t4, 0xC($s0)
        MEM_W(0XC, ctx->r16) = ctx->r12;
            goto L_8003F3B8;
    }
    // 0x8003F3B0: sw          $t4, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r12;
    // 0x8003F3B4: ori         $s3, $s3, 0x2000
    ctx->r19 = ctx->r19 | 0X2000;
L_8003F3B8:
    // 0x8003F3B8: bnel        $s4, $zero, L_8003F3CC
    if (ctx->r20 != 0) {
        // 0x8003F3BC: sw          $s3, 0xC4($s6)
        MEM_W(0XC4, ctx->r22) = ctx->r19;
            goto L_8003F3CC;
    }
    goto skip_1;
    // 0x8003F3BC: sw          $s3, 0xC4($s6)
    MEM_W(0XC4, ctx->r22) = ctx->r19;
    skip_1:
    // 0x8003F3C0: b           L_8003F3CC
    // 0x8003F3C4: sw          $s3, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r19;
        goto L_8003F3CC;
    // 0x8003F3C4: sw          $s3, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r19;
    // 0x8003F3C8: sw          $s3, 0xC4($s6)
    MEM_W(0XC4, ctx->r22) = ctx->r19;
L_8003F3CC:
    // 0x8003F3CC: addiu       $s4, $s4, 0x1
    ctx->r20 = ADD32(ctx->r20, 0X1);
    // 0x8003F3D0: bne         $s4, $fp, L_8003F2B0
    if (ctx->r20 != ctx->r30) {
        // 0x8003F3D4: nop
    
            goto L_8003F2B0;
    }
    // 0x8003F3D4: nop

    // 0x8003F3D8: lw          $t6, 0x0($s6)
    ctx->r14 = MEM_W(ctx->r22, 0X0);
    // 0x8003F3DC: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x8003F3E0: beql        $t7, $zero, L_8003F420
    if (ctx->r15 == 0) {
        // 0x8003F3E4: lw          $ra, 0x3C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X3C);
            goto L_8003F420;
    }
    goto skip_2;
    // 0x8003F3E4: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    skip_2:
    // 0x8003F3E8: lw          $v0, 0xC4($s6)
    ctx->r2 = MEM_W(ctx->r22, 0XC4);
    // 0x8003F3EC: andi        $t8, $v0, 0x8000
    ctx->r24 = ctx->r2 & 0X8000;
    // 0x8003F3F0: beql        $t8, $zero, L_8003F420
    if (ctx->r24 == 0) {
        // 0x8003F3F4: lw          $ra, 0x3C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X3C);
            goto L_8003F420;
    }
    goto skip_3;
    // 0x8003F3F4: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    skip_3:
    // 0x8003F3F8: lwc1        $f8, 0xC($s6)
    ctx->f8.u32l = MEM_W(ctx->r22, 0XC);
    // 0x8003F3FC: lwc1        $f16, 0xD0($s6)
    ctx->f16.u32l = MEM_W(ctx->r22, 0XD0);
    // 0x8003F400: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x8003F404: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x8003F408: c.lt.s      $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f8.fl < ctx->f16.fl;
    // 0x8003F40C: and         $t9, $v0, $at
    ctx->r25 = ctx->r2 & ctx->r1;
    // 0x8003F410: bc1fl       L_8003F420
    if (!c1cs) {
        // 0x8003F414: lw          $ra, 0x3C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X3C);
            goto L_8003F420;
    }
    goto skip_4;
    // 0x8003F414: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    skip_4:
    // 0x8003F418: sw          $t9, 0xC4($s6)
    MEM_W(0XC4, ctx->r22) = ctx->r25;
L_8003F41C:
    // 0x8003F41C: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_8003F420:
    // 0x8003F420: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8003F424: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8003F428: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8003F42C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8003F430: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8003F434: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x8003F438: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x8003F43C: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x8003F440: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x8003F444: jr          $ra
    // 0x8003F448: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x8003F448: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_8003F44C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003F44C: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x8003F450: sw          $s6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r22;
    // 0x8003F454: lw          $s6, 0x90($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X90);
    // 0x8003F458: sw          $s7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r23;
    // 0x8003F45C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8003F460: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x8003F464: mtc1        $a2, $f20
    ctx->f20.u32l = ctx->r6;
    // 0x8003F468: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x8003F46C: or          $s7, $a0, $zero
    ctx->r23 = ctx->r4 | 0;
    // 0x8003F470: sw          $ra, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r31;
    // 0x8003F474: sw          $fp, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r30;
    // 0x8003F478: sw          $s5, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r21;
    // 0x8003F47C: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x8003F480: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x8003F484: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x8003F488: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x8003F48C: sw          $a3, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->r7;
    // 0x8003F490: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    // 0x8003F494: jal         0x80000F30
    // 0x8003F498: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8003F498: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    after_0:
    // 0x8003F49C: lw          $t6, 0x0($s6)
    ctx->r14 = MEM_W(ctx->r22, 0X0);
    // 0x8003F4A0: lui         $at, 0x400
    ctx->r1 = S32(0X400 << 16);
    // 0x8003F4A4: lui         $t8, 0x801A
    ctx->r24 = S32(0X801A << 16);
    // 0x8003F4A8: or          $t7, $t6, $at
    ctx->r15 = ctx->r14 | ctx->r1;
    // 0x8003F4AC: sw          $t7, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r15;
    // 0x8003F4B0: lw          $t8, -0x1134($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X1134);
    // 0x8003F4B4: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x8003F4B8: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x8003F4BC: beql        $t8, $zero, L_8003F6FC
    if (ctx->r24 == 0) {
        // 0x8003F4C0: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F6FC;
    }
    goto skip_0;
    // 0x8003F4C0: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_0:
    // 0x8003F4C4: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x8003F4C8: jal         0x8016CE60
    // 0x8003F4CC: lw          $a3, 0x8C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X8C);
    func_8016CE60(rdram, ctx);
        goto after_1;
    // 0x8003F4CC: lw          $a3, 0x8C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X8C);
    after_1:
    // 0x8003F4D0: beq         $v0, $zero, L_8003F6F8
    if (ctx->r2 == 0) {
        // 0x8003F4D4: or          $fp, $v0, $zero
        ctx->r30 = ctx->r2 | 0;
            goto L_8003F6F8;
    }
    // 0x8003F4D4: or          $fp, $v0, $zero
    ctx->r30 = ctx->r2 | 0;
    // 0x8003F4D8: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x8003F4DC: or          $s4, $zero, $zero
    ctx->r20 = 0 | 0;
L_8003F4E0:
    // 0x8003F4E0: bne         $s4, $zero, L_8003F4F0
    if (ctx->r20 != 0) {
        // 0x8003F4E4: addiu       $s1, $fp, 0x48C
        ctx->r17 = ADD32(ctx->r30, 0X48C);
            goto L_8003F4F0;
    }
    // 0x8003F4E4: addiu       $s1, $fp, 0x48C
    ctx->r17 = ADD32(ctx->r30, 0X48C);
    // 0x8003F4E8: b           L_8003F4F0
    // 0x8003F4EC: addiu       $s1, $fp, 0x230
    ctx->r17 = ADD32(ctx->r30, 0X230);
        goto L_8003F4F0;
    // 0x8003F4EC: addiu       $s1, $fp, 0x230
    ctx->r17 = ADD32(ctx->r30, 0X230);
L_8003F4F0:
    // 0x8003F4F0: lh          $t9, 0x60($s1)
    ctx->r25 = MEM_H(ctx->r17, 0X60);
    // 0x8003F4F4: lui         $s3, 0x2480
    ctx->r19 = S32(0X2480 << 16);
    // 0x8003F4F8: ori         $s3, $s3, 0x8000
    ctx->r19 = ctx->r19 | 0X8000;
    // 0x8003F4FC: beq         $t9, $zero, L_8003F6A4
    if (ctx->r25 == 0) {
        // 0x8003F500: lui         $at, 0x1000
        ctx->r1 = S32(0X1000 << 16);
            goto L_8003F6A4;
    }
    // 0x8003F500: lui         $at, 0x1000
    ctx->r1 = S32(0X1000 << 16);
    // 0x8003F504: lui         $s5, 0x8000
    ctx->r21 = S32(0X8000 << 16);
    // 0x8003F508: or          $s3, $s3, $at
    ctx->r19 = ctx->r19 | ctx->r1;
    // 0x8003F50C: bne         $s4, $zero, L_8003F51C
    if (ctx->r20 != 0) {
        // 0x8003F510: addiu       $s5, $s5, 0x1090
        ctx->r21 = ADD32(ctx->r21, 0X1090);
            goto L_8003F51C;
    }
    // 0x8003F510: addiu       $s5, $s5, 0x1090
    ctx->r21 = ADD32(ctx->r21, 0X1090);
    // 0x8003F514: b           L_8003F520
    // 0x8003F518: addiu       $s0, $s6, 0x4
    ctx->r16 = ADD32(ctx->r22, 0X4);
        goto L_8003F520;
    // 0x8003F518: addiu       $s0, $s6, 0x4
    ctx->r16 = ADD32(ctx->r22, 0X4);
L_8003F51C:
    // 0x8003F51C: addiu       $s0, $s6, 0xC8
    ctx->r16 = ADD32(ctx->r22, 0XC8);
L_8003F520:
    // 0x8003F520: lwc1        $f4, 0x0($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X0);
    // 0x8003F524: lwc1        $f6, 0x0($s7)
    ctx->f6.u32l = MEM_W(ctx->r23, 0X0);
    // 0x8003F528: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8003F52C: swc1        $f8, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f8.u32l;
    // 0x8003F530: lwc1        $f16, 0x4($s7)
    ctx->f16.u32l = MEM_W(ctx->r23, 0X4);
    // 0x8003F534: lwc1        $f10, 0x4($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X4);
    // 0x8003F538: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x8003F53C: lwc1        $f10, 0x4C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x8003F540: swc1        $f18, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f18.u32l;
    // 0x8003F544: lwc1        $f6, 0x8($s7)
    ctx->f6.u32l = MEM_W(ctx->r23, 0X8);
    // 0x8003F548: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x8003F54C: mul.s       $f16, $f10, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f10.fl);
    // 0x8003F550: lwc1        $f18, 0x50($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X50);
    // 0x8003F554: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8003F558: mul.s       $f4, $f18, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x8003F55C: swc1        $f8, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f8.u32l;
    // 0x8003F560: lwc1        $f8, 0x54($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X54);
    // 0x8003F564: mul.s       $f10, $f8, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f8.fl);
    // 0x8003F568: add.s       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f16.fl + ctx->f4.fl;
    // 0x8003F56C: jal         0x800A01E0
    // 0x8003F570: add.s       $f12, $f10, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_2;
    // 0x8003F570: add.s       $f12, $f10, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f6.fl;
    after_2:
    // 0x8003F574: swc1        $f0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f0.u32l;
    // 0x8003F578: lwc1        $f18, 0x0($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X0);
    // 0x8003F57C: addiu       $s2, $s0, 0x10
    ctx->r18 = ADD32(ctx->r16, 0X10);
    // 0x8003F580: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8003F584: swc1        $f18, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f18.u32l;
    // 0x8003F588: lwc1        $f16, 0x4($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X4);
    // 0x8003F58C: swc1        $f16, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->f16.u32l;
    // 0x8003F590: lwc1        $f4, 0x8($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X8);
    // 0x8003F594: swc1        $f4, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->f4.u32l;
    // 0x8003F598: lh          $a2, 0x5C($s1)
    ctx->r6 = MEM_H(ctx->r17, 0X5C);
    // 0x8003F59C: bne         $a2, $zero, L_8003F5C0
    if (ctx->r6 != 0) {
        // 0x8003F5A0: nop
    
            goto L_8003F5C0;
    }
    // 0x8003F5A0: nop

    // 0x8003F5A4: lwc1        $f8, 0x48($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X48);
    // 0x8003F5A8: swc1        $f20, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f20.u32l;
    // 0x8003F5AC: addiu       $s2, $s0, 0x10
    ctx->r18 = ADD32(ctx->r16, 0X10);
    // 0x8003F5B0: swc1        $f8, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f8.u32l;
    // 0x8003F5B4: lwc1        $f10, 0x50($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X50);
    // 0x8003F5B8: b           L_8003F5C8
    // 0x8003F5BC: swc1        $f10, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f10.u32l;
        goto L_8003F5C8;
    // 0x8003F5BC: swc1        $f10, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f10.u32l;
L_8003F5C0:
    // 0x8003F5C0: jal         0x8003FA10
    // 0x8003F5C4: addiu       $a1, $s1, 0x48
    ctx->r5 = ADD32(ctx->r17, 0X48);
    func_8003FA10(rdram, ctx);
        goto after_3;
    // 0x8003F5C4: addiu       $a1, $s1, 0x48
    ctx->r5 = ADD32(ctx->r17, 0X48);
    after_3:
L_8003F5C8:
    // 0x8003F5C8: lwc1        $f6, 0x10($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X10);
    // 0x8003F5CC: lwc1        $f18, 0x3C($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X3C);
    // 0x8003F5D0: lwc1        $f4, 0x18($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8003F5D4: lwc1        $f8, 0x44($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X44);
    // 0x8003F5D8: mul.s       $f16, $f6, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f18.fl);
    // 0x8003F5DC: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8003F5E0: addiu       $a1, $s0, 0x20
    ctx->r5 = ADD32(ctx->r16, 0X20);
    // 0x8003F5E4: mul.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x8003F5E8: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    // 0x8003F5EC: add.s       $f6, $f16, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f16.fl + ctx->f10.fl;
    // 0x8003F5F0: neg.s       $f18, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f18.fl = -ctx->f6.fl;
    // 0x8003F5F4: jalr        $s5
    // 0x8003F5F8: swc1        $f18, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f18.u32l;
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_4;
    // 0x8003F5F8: swc1        $f18, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f18.u32l;
    after_4:
    // 0x8003F5FC: bne         $s4, $zero, L_8003F60C
    if (ctx->r20 != 0) {
        // 0x8003F600: or          $a0, $s2, $zero
        ctx->r4 = ctx->r18 | 0;
            goto L_8003F60C;
    }
    // 0x8003F600: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8003F604: b           L_8003F610
    // 0x8003F608: addiu       $v0, $s6, 0x64
    ctx->r2 = ADD32(ctx->r22, 0X64);
        goto L_8003F610;
    // 0x8003F608: addiu       $v0, $s6, 0x64
    ctx->r2 = ADD32(ctx->r22, 0X64);
L_8003F60C:
    // 0x8003F60C: addiu       $v0, $s6, 0x128
    ctx->r2 = ADD32(ctx->r22, 0X128);
L_8003F610:
    // 0x8003F610: addiu       $a1, $v0, 0x10
    ctx->r5 = ADD32(ctx->r2, 0X10);
    // 0x8003F614: jalr        $s5
    // 0x8003F618: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_5;
    // 0x8003F618: addiu       $a2, $zero, 0x10
    ctx->r6 = ADD32(0, 0X10);
    after_5:
    // 0x8003F61C: lwc1        $f12, 0x18($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8003F620: lwc1        $f4, 0x8($s7)
    ctx->f4.u32l = MEM_W(ctx->r23, 0X8);
    // 0x8003F624: lwc1        $f2, 0x10($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X10);
    // 0x8003F628: lwc1        $f16, 0x0($s7)
    ctx->f16.u32l = MEM_W(ctx->r23, 0X0);
    // 0x8003F62C: mul.s       $f8, $f4, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x8003F630: lwc1        $f18, 0x1C($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X1C);
    // 0x8003F634: addiu       $v0, $s0, 0x48
    ctx->r2 = ADD32(ctx->r16, 0X48);
    // 0x8003F638: mul.s       $f10, $f2, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f16.fl);
    // 0x8003F63C: add.s       $f6, $f8, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x8003F640: add.s       $f0, $f6, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f6.fl + ctx->f18.fl;
    // 0x8003F644: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x8003F648: nop

    // 0x8003F64C: bc1fl       L_8003F668
    if (!c1cs) {
        // 0x8003F650: sw          $v0, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r2;
            goto L_8003F668;
    }
    goto skip_1;
    // 0x8003F650: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
    skip_1:
    // 0x8003F654: neg.s       $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = -ctx->f2.fl;
    // 0x8003F658: neg.s       $f16, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = -ctx->f12.fl;
    // 0x8003F65C: swc1        $f4, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f4.u32l;
    // 0x8003F660: swc1        $f16, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f16.u32l;
    // 0x8003F664: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
L_8003F668:
    // 0x8003F668: lw          $t0, 0x44($s1)
    ctx->r8 = MEM_W(ctx->r17, 0X44);
    // 0x8003F66C: lbu         $t1, 0x2B($t0)
    ctx->r9 = MEM_BU(ctx->r8, 0X2B);
    // 0x8003F670: addiu       $t2, $t1, 0x200
    ctx->r10 = ADD32(ctx->r9, 0X200);
    // 0x8003F674: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
    // 0x8003F678: lw          $t3, 0x44($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X44);
    // 0x8003F67C: sw          $t3, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r11;
    // 0x8003F680: lw          $t4, 0x58($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X58);
    // 0x8003F684: beq         $t4, $zero, L_8003F690
    if (ctx->r12 == 0) {
        // 0x8003F688: sw          $t4, 0xC($s0)
        MEM_W(0XC, ctx->r16) = ctx->r12;
            goto L_8003F690;
    }
    // 0x8003F688: sw          $t4, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r12;
    // 0x8003F68C: ori         $s3, $s3, 0x2000
    ctx->r19 = ctx->r19 | 0X2000;
L_8003F690:
    // 0x8003F690: bnel        $s4, $zero, L_8003F6A4
    if (ctx->r20 != 0) {
        // 0x8003F694: sw          $s3, 0xC4($s6)
        MEM_W(0XC4, ctx->r22) = ctx->r19;
            goto L_8003F6A4;
    }
    goto skip_2;
    // 0x8003F694: sw          $s3, 0xC4($s6)
    MEM_W(0XC4, ctx->r22) = ctx->r19;
    skip_2:
    // 0x8003F698: b           L_8003F6A4
    // 0x8003F69C: sw          $s3, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r19;
        goto L_8003F6A4;
    // 0x8003F69C: sw          $s3, 0x0($s6)
    MEM_W(0X0, ctx->r22) = ctx->r19;
    // 0x8003F6A0: sw          $s3, 0xC4($s6)
    MEM_W(0XC4, ctx->r22) = ctx->r19;
L_8003F6A4:
    // 0x8003F6A4: addiu       $s4, $s4, 0x1
    ctx->r20 = ADD32(ctx->r20, 0X1);
    // 0x8003F6A8: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8003F6AC: bne         $s4, $at, L_8003F4E0
    if (ctx->r20 != ctx->r1) {
        // 0x8003F6B0: nop
    
            goto L_8003F4E0;
    }
    // 0x8003F6B0: nop

    // 0x8003F6B4: lw          $t6, 0x0($s6)
    ctx->r14 = MEM_W(ctx->r22, 0X0);
    // 0x8003F6B8: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x8003F6BC: beql        $t7, $zero, L_8003F6FC
    if (ctx->r15 == 0) {
        // 0x8003F6C0: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F6FC;
    }
    goto skip_3;
    // 0x8003F6C0: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_3:
    // 0x8003F6C4: lw          $v0, 0xC4($s6)
    ctx->r2 = MEM_W(ctx->r22, 0XC4);
    // 0x8003F6C8: andi        $t8, $v0, 0x8000
    ctx->r24 = ctx->r2 & 0X8000;
    // 0x8003F6CC: beql        $t8, $zero, L_8003F6FC
    if (ctx->r24 == 0) {
        // 0x8003F6D0: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F6FC;
    }
    goto skip_4;
    // 0x8003F6D0: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_4:
    // 0x8003F6D4: lwc1        $f8, 0xC($s6)
    ctx->f8.u32l = MEM_W(ctx->r22, 0XC);
    // 0x8003F6D8: lwc1        $f10, 0xD0($s6)
    ctx->f10.u32l = MEM_W(ctx->r22, 0XD0);
    // 0x8003F6DC: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x8003F6E0: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x8003F6E4: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x8003F6E8: and         $t9, $v0, $at
    ctx->r25 = ctx->r2 & ctx->r1;
    // 0x8003F6EC: bc1fl       L_8003F6FC
    if (!c1cs) {
        // 0x8003F6F0: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F6FC;
    }
    goto skip_5;
    // 0x8003F6F0: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_5:
    // 0x8003F6F4: sw          $t9, 0xC4($s6)
    MEM_W(0XC4, ctx->r22) = ctx->r25;
L_8003F6F8:
    // 0x8003F6F8: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
L_8003F6FC:
    // 0x8003F6FC: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x8003F700: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8003F704: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x8003F708: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x8003F70C: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x8003F710: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x8003F714: lw          $s5, 0x34($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X34);
    // 0x8003F718: lw          $s6, 0x38($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X38);
    // 0x8003F71C: lw          $s7, 0x3C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X3C);
    // 0x8003F720: lw          $fp, 0x40($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X40);
    // 0x8003F724: jr          $ra
    // 0x8003F728: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    return;
    // 0x8003F728: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
;}
RECOMP_FUNC void func_8003F72C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003F72C: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x8003F730: sw          $s5, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r21;
    // 0x8003F734: lw          $s5, 0x70($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X70);
    // 0x8003F738: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8003F73C: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x8003F740: mtc1        $a2, $f20
    ctx->f20.u32l = ctx->r6;
    // 0x8003F744: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8003F748: sw          $ra, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r31;
    // 0x8003F74C: sw          $a1, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r5;
    // 0x8003F750: sw          $fp, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r30;
    // 0x8003F754: sw          $s7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r23;
    // 0x8003F758: sw          $s6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r22;
    // 0x8003F75C: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x8003F760: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x8003F764: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x8003F768: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x8003F76C: sw          $a3, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r7;
    // 0x8003F770: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    // 0x8003F774: jal         0x80000F30
    // 0x8003F778: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8003F778: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    after_0:
    // 0x8003F77C: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x8003F780: lw          $t6, -0x1134($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X1134);
    // 0x8003F784: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8003F788: lw          $a2, 0x6C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X6C);
    // 0x8003F78C: beql        $t6, $zero, L_8003F9E0
    if (ctx->r14 == 0) {
        // 0x8003F790: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F9E0;
    }
    goto skip_0;
    // 0x8003F790: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_0:
    // 0x8003F794: mfc1        $a1, $f20
    ctx->r5 = (int32_t)ctx->f20.u32l;
    // 0x8003F798: jal         0x8016F310
    // 0x8003F79C: lw          $a3, 0x64($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X64);
    func_8016F310(rdram, ctx);
        goto after_1;
    // 0x8003F79C: lw          $a3, 0x64($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X64);
    after_1:
    // 0x8003F7A0: beq         $v0, $zero, L_8003F9DC
    if (ctx->r2 == 0) {
        // 0x8003F7A4: sw          $v0, 0x48($sp)
        MEM_W(0X48, ctx->r29) = ctx->r2;
            goto L_8003F9DC;
    }
    // 0x8003F7A4: sw          $v0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r2;
    // 0x8003F7A8: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x8003F7AC: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
    // 0x8003F7B0: lui         $fp, 0x2000
    ctx->r30 = S32(0X2000 << 16);
    // 0x8003F7B4: addiu       $s7, $zero, 0x2
    ctx->r23 = ADD32(0, 0X2);
L_8003F7B8:
    // 0x8003F7B8: bne         $s6, $zero, L_8003F7CC
    if (ctx->r22 != 0) {
        // 0x8003F7BC: or          $s4, $zero, $zero
        ctx->r20 = 0 | 0;
            goto L_8003F7CC;
    }
    // 0x8003F7BC: or          $s4, $zero, $zero
    ctx->r20 = 0 | 0;
    // 0x8003F7C0: lw          $s1, 0x48($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X48);
    // 0x8003F7C4: b           L_8003F7D4
    // 0x8003F7C8: addiu       $s1, $s1, 0x230
    ctx->r17 = ADD32(ctx->r17, 0X230);
        goto L_8003F7D4;
    // 0x8003F7C8: addiu       $s1, $s1, 0x230
    ctx->r17 = ADD32(ctx->r17, 0X230);
L_8003F7CC:
    // 0x8003F7CC: lw          $s1, 0x48($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X48);
    // 0x8003F7D0: addiu       $s1, $s1, 0x48C
    ctx->r17 = ADD32(ctx->r17, 0X48C);
L_8003F7D4:
    // 0x8003F7D4: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
L_8003F7D8:
    // 0x8003F7D8: lh          $t7, 0x60($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X60);
    // 0x8003F7DC: sll         $s3, $s2, 2
    ctx->r19 = S32(ctx->r18 << 2);
    // 0x8003F7E0: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8003F7E4: beq         $t7, $zero, L_8003F928
    if (ctx->r15 == 0) {
        // 0x8003F7E8: addu        $t9, $t9, $s3
        ctx->r25 = ADD32(ctx->r25, ctx->r19);
            goto L_8003F928;
    }
    // 0x8003F7E8: addu        $t9, $t9, $s3
    ctx->r25 = ADD32(ctx->r25, ctx->r19);
    // 0x8003F7EC: lh          $t0, 0x5E($s1)
    ctx->r8 = MEM_H(ctx->r17, 0X5E);
    // 0x8003F7F0: lw          $t9, 0x4D00($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X4D00);
    // 0x8003F7F4: or          $t8, $s4, $fp
    ctx->r24 = ctx->r20 | ctx->r30;
    // 0x8003F7F8: beq         $t0, $zero, L_8003F810
    if (ctx->r8 == 0) {
        // 0x8003F7FC: or          $s4, $t8, $t9
        ctx->r20 = ctx->r24 | ctx->r25;
            goto L_8003F810;
    }
    // 0x8003F7FC: or          $s4, $t8, $t9
    ctx->r20 = ctx->r24 | ctx->r25;
    // 0x8003F800: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x8003F804: addu        $t1, $t1, $s3
    ctx->r9 = ADD32(ctx->r9, ctx->r19);
    // 0x8003F808: lw          $t1, 0x4D14($t1)
    ctx->r9 = MEM_W(ctx->r9, 0X4D14);
    // 0x8003F80C: or          $s4, $s4, $t1
    ctx->r20 = ctx->r20 | ctx->r9;
L_8003F810:
    // 0x8003F810: bne         $s6, $zero, L_8003F830
    if (ctx->r22 != 0) {
        // 0x8003F814: addiu       $a1, $s1, 0x48
        ctx->r5 = ADD32(ctx->r17, 0X48);
            goto L_8003F830;
    }
    // 0x8003F814: addiu       $a1, $s1, 0x48
    ctx->r5 = ADD32(ctx->r17, 0X48);
    // 0x8003F818: bne         $s2, $zero, L_8003F828
    if (ctx->r18 != 0) {
        // 0x8003F81C: nop
    
            goto L_8003F828;
    }
    // 0x8003F81C: nop

    // 0x8003F820: b           L_8003F840
    // 0x8003F824: addiu       $s0, $s5, 0x4
    ctx->r16 = ADD32(ctx->r21, 0X4);
        goto L_8003F840;
    // 0x8003F824: addiu       $s0, $s5, 0x4
    ctx->r16 = ADD32(ctx->r21, 0X4);
L_8003F828:
    // 0x8003F828: b           L_8003F840
    // 0x8003F82C: addiu       $s0, $s5, 0x64
    ctx->r16 = ADD32(ctx->r21, 0X64);
        goto L_8003F840;
    // 0x8003F82C: addiu       $s0, $s5, 0x64
    ctx->r16 = ADD32(ctx->r21, 0X64);
L_8003F830:
    // 0x8003F830: bne         $s2, $zero, L_8003F840
    if (ctx->r18 != 0) {
        // 0x8003F834: addiu       $s0, $s5, 0x128
        ctx->r16 = ADD32(ctx->r21, 0X128);
            goto L_8003F840;
    }
    // 0x8003F834: addiu       $s0, $s5, 0x128
    ctx->r16 = ADD32(ctx->r21, 0X128);
    // 0x8003F838: b           L_8003F840
    // 0x8003F83C: addiu       $s0, $s5, 0xC8
    ctx->r16 = ADD32(ctx->r21, 0XC8);
        goto L_8003F840;
    // 0x8003F83C: addiu       $s0, $s5, 0xC8
    ctx->r16 = ADD32(ctx->r21, 0XC8);
L_8003F840:
    // 0x8003F840: lwc1        $f4, 0x18($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X18);
    // 0x8003F844: addiu       $t2, $s0, 0x48
    ctx->r10 = ADD32(ctx->r16, 0X48);
    // 0x8003F848: swc1        $f4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f4.u32l;
    // 0x8003F84C: lwc1        $f6, 0x0($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X0);
    // 0x8003F850: swc1        $f6, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f6.u32l;
    // 0x8003F854: lwc1        $f8, 0x4($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X4);
    // 0x8003F858: lwc1        $f6, 0x3C($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X3C);
    // 0x8003F85C: swc1        $f8, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->f8.u32l;
    // 0x8003F860: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x8003F864: swc1        $f10, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->f10.u32l;
    // 0x8003F868: lwc1        $f16, 0x20($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X20);
    // 0x8003F86C: swc1        $f20, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f20.u32l;
    // 0x8003F870: swc1        $f16, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f16.u32l;
    // 0x8003F874: lwc1        $f18, 0x28($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X28);
    // 0x8003F878: lwc1        $f4, 0x10($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X10);
    // 0x8003F87C: lwc1        $f16, 0x44($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X44);
    // 0x8003F880: swc1        $f18, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f18.u32l;
    // 0x8003F884: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8003F888: lwc1        $f10, 0x18($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X18);
    // 0x8003F88C: sw          $t2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r10;
    // 0x8003F890: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x8003F894: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x8003F898: neg.s       $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = -ctx->f4.fl;
    // 0x8003F89C: swc1        $f6, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f6.u32l;
    // 0x8003F8A0: lw          $t3, 0x44($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X44);
    // 0x8003F8A4: lbu         $t4, 0x2B($t3)
    ctx->r12 = MEM_BU(ctx->r11, 0X2B);
    // 0x8003F8A8: addiu       $t5, $t4, 0x200
    ctx->r13 = ADD32(ctx->r12, 0X200);
    // 0x8003F8AC: sh          $t5, 0x0($t2)
    MEM_H(0X0, ctx->r10) = ctx->r13;
    // 0x8003F8B0: lw          $t7, 0x44($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X44);
    // 0x8003F8B4: sw          $t7, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r15;
    // 0x8003F8B8: lw          $t8, 0x58($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X58);
    // 0x8003F8BC: sw          $t8, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r24;
    // 0x8003F8C0: lh          $a2, 0x5C($s1)
    ctx->r6 = MEM_H(ctx->r17, 0X5C);
    // 0x8003F8C4: bne         $a2, $zero, L_8003F8E4
    if (ctx->r6 != 0) {
        // 0x8003F8C8: nop
    
            goto L_8003F8E4;
    }
    // 0x8003F8C8: nop

    // 0x8003F8CC: lwc1        $f10, 0x48($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X48);
    // 0x8003F8D0: swc1        $f20, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f20.u32l;
    // 0x8003F8D4: swc1        $f10, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f10.u32l;
    // 0x8003F8D8: lwc1        $f16, 0x50($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X50);
    // 0x8003F8DC: b           L_8003F8EC
    // 0x8003F8E0: swc1        $f16, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f16.u32l;
        goto L_8003F8EC;
    // 0x8003F8E0: swc1        $f16, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f16.u32l;
L_8003F8E4:
    // 0x8003F8E4: jal         0x8003FA10
    // 0x8003F8E8: addiu       $a0, $s0, 0x20
    ctx->r4 = ADD32(ctx->r16, 0X20);
    func_8003FA10(rdram, ctx);
        goto after_2;
    // 0x8003F8E8: addiu       $a0, $s0, 0x20
    ctx->r4 = ADD32(ctx->r16, 0X20);
    after_2:
L_8003F8EC:
    // 0x8003F8EC: lwc1        $f8, 0x28($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X28);
    // 0x8003F8F0: lwc1        $f18, 0x44($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X44);
    // 0x8003F8F4: lwc1        $f6, 0x20($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X20);
    // 0x8003F8F8: lwc1        $f10, 0x3C($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X3C);
    // 0x8003F8FC: mul.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x8003F900: lw          $t9, 0xC($s0)
    ctx->r25 = MEM_W(ctx->r16, 0XC);
    // 0x8003F904: lui         $t0, 0x800B
    ctx->r8 = S32(0X800B << 16);
    // 0x8003F908: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8003F90C: addu        $t0, $t0, $s3
    ctx->r8 = ADD32(ctx->r8, ctx->r19);
    // 0x8003F910: add.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x8003F914: neg.s       $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = -ctx->f8.fl;
    // 0x8003F918: beq         $t9, $zero, L_8003F928
    if (ctx->r25 == 0) {
        // 0x8003F91C: swc1        $f18, 0x2C($s0)
        MEM_W(0X2C, ctx->r16) = ctx->f18.u32l;
            goto L_8003F928;
    }
    // 0x8003F91C: swc1        $f18, 0x2C($s0)
    MEM_W(0X2C, ctx->r16) = ctx->f18.u32l;
    // 0x8003F920: lw          $t0, 0x4D08($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X4D08);
    // 0x8003F924: or          $s4, $s4, $t0
    ctx->r20 = ctx->r20 | ctx->r8;
L_8003F928:
    // 0x8003F928: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x8003F92C: bne         $s2, $s7, L_8003F7D8
    if (ctx->r18 != ctx->r23) {
        // 0x8003F930: addiu       $s1, $s1, 0x64
        ctx->r17 = ADD32(ctx->r17, 0X64);
            goto L_8003F7D8;
    }
    // 0x8003F930: addiu       $s1, $s1, 0x64
    ctx->r17 = ADD32(ctx->r17, 0X64);
    // 0x8003F934: bnel        $s6, $zero, L_8003F948
    if (ctx->r22 != 0) {
        // 0x8003F938: sw          $s4, 0xC4($s5)
        MEM_W(0XC4, ctx->r21) = ctx->r20;
            goto L_8003F948;
    }
    goto skip_1;
    // 0x8003F938: sw          $s4, 0xC4($s5)
    MEM_W(0XC4, ctx->r21) = ctx->r20;
    skip_1:
    // 0x8003F93C: b           L_8003F948
    // 0x8003F940: sw          $s4, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r20;
        goto L_8003F948;
    // 0x8003F940: sw          $s4, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r20;
    // 0x8003F944: sw          $s4, 0xC4($s5)
    MEM_W(0XC4, ctx->r21) = ctx->r20;
L_8003F948:
    // 0x8003F948: addiu       $s6, $s6, 0x1
    ctx->r22 = ADD32(ctx->r22, 0X1);
    // 0x8003F94C: bne         $s6, $s7, L_8003F7B8
    if (ctx->r22 != ctx->r23) {
        // 0x8003F950: nop
    
            goto L_8003F7B8;
    }
    // 0x8003F950: nop

    // 0x8003F954: lw          $v0, 0x0($s5)
    ctx->r2 = MEM_W(ctx->r21, 0X0);
    // 0x8003F958: andi        $t1, $v0, 0x8000
    ctx->r9 = ctx->r2 & 0X8000;
    // 0x8003F95C: beql        $t1, $zero, L_8003F9A0
    if (ctx->r9 == 0) {
        // 0x8003F960: andi        $t4, $v0, 0x800
        ctx->r12 = ctx->r2 & 0X800;
            goto L_8003F9A0;
    }
    goto skip_2;
    // 0x8003F960: andi        $t4, $v0, 0x800
    ctx->r12 = ctx->r2 & 0X800;
    skip_2:
    // 0x8003F964: lw          $v1, 0xC4($s5)
    ctx->r3 = MEM_W(ctx->r21, 0XC4);
    // 0x8003F968: andi        $t2, $v1, 0x8000
    ctx->r10 = ctx->r3 & 0X8000;
    // 0x8003F96C: beql        $t2, $zero, L_8003F9A0
    if (ctx->r10 == 0) {
        // 0x8003F970: andi        $t4, $v0, 0x800
        ctx->r12 = ctx->r2 & 0X800;
            goto L_8003F9A0;
    }
    goto skip_3;
    // 0x8003F970: andi        $t4, $v0, 0x800
    ctx->r12 = ctx->r2 & 0X800;
    skip_3:
    // 0x8003F974: lwc1        $f6, 0xC($s5)
    ctx->f6.u32l = MEM_W(ctx->r21, 0XC);
    // 0x8003F978: lwc1        $f10, 0xD0($s5)
    ctx->f10.u32l = MEM_W(ctx->r21, 0XD0);
    // 0x8003F97C: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x8003F980: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x8003F984: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x8003F988: and         $t3, $v1, $at
    ctx->r11 = ctx->r3 & ctx->r1;
    // 0x8003F98C: bc1fl       L_8003F9A0
    if (!c1cs) {
        // 0x8003F990: andi        $t4, $v0, 0x800
        ctx->r12 = ctx->r2 & 0X800;
            goto L_8003F9A0;
    }
    goto skip_4;
    // 0x8003F990: andi        $t4, $v0, 0x800
    ctx->r12 = ctx->r2 & 0X800;
    skip_4:
    // 0x8003F994: sw          $t3, 0xC4($s5)
    MEM_W(0XC4, ctx->r21) = ctx->r11;
    // 0x8003F998: lw          $v0, 0x0($s5)
    ctx->r2 = MEM_W(ctx->r21, 0X0);
    // 0x8003F99C: andi        $t4, $v0, 0x800
    ctx->r12 = ctx->r2 & 0X800;
L_8003F9A0:
    // 0x8003F9A0: beql        $t4, $zero, L_8003F9E0
    if (ctx->r12 == 0) {
        // 0x8003F9A4: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F9E0;
    }
    goto skip_5;
    // 0x8003F9A4: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_5:
    // 0x8003F9A8: lw          $v1, 0xC4($s5)
    ctx->r3 = MEM_W(ctx->r21, 0XC4);
    // 0x8003F9AC: andi        $t5, $v1, 0x800
    ctx->r13 = ctx->r3 & 0X800;
    // 0x8003F9B0: beql        $t5, $zero, L_8003F9E0
    if (ctx->r13 == 0) {
        // 0x8003F9B4: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F9E0;
    }
    goto skip_6;
    // 0x8003F9B4: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_6:
    // 0x8003F9B8: lwc1        $f4, 0x6C($s5)
    ctx->f4.u32l = MEM_W(ctx->r21, 0X6C);
    // 0x8003F9BC: lwc1        $f16, 0x130($s5)
    ctx->f16.u32l = MEM_W(ctx->r21, 0X130);
    // 0x8003F9C0: addiu       $at, $zero, -0x801
    ctx->r1 = ADD32(0, -0X801);
    // 0x8003F9C4: and         $t6, $v1, $at
    ctx->r14 = ctx->r3 & ctx->r1;
    // 0x8003F9C8: c.lt.s      $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f4.fl < ctx->f16.fl;
    // 0x8003F9CC: nop

    // 0x8003F9D0: bc1fl       L_8003F9E0
    if (!c1cs) {
        // 0x8003F9D4: lw          $ra, 0x44($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X44);
            goto L_8003F9E0;
    }
    goto skip_7;
    // 0x8003F9D4: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    skip_7:
    // 0x8003F9D8: sw          $t6, 0xC4($s5)
    MEM_W(0XC4, ctx->r21) = ctx->r14;
L_8003F9DC:
    // 0x8003F9DC: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
L_8003F9E0:
    // 0x8003F9E0: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x8003F9E4: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8003F9E8: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x8003F9EC: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x8003F9F0: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x8003F9F4: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x8003F9F8: lw          $s5, 0x34($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X34);
    // 0x8003F9FC: lw          $s6, 0x38($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X38);
    // 0x8003FA00: lw          $s7, 0x3C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X3C);
    // 0x8003FA04: lw          $fp, 0x40($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X40);
    // 0x8003FA08: jr          $ra
    // 0x8003FA0C: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    return;
    // 0x8003FA0C: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
;}
RECOMP_FUNC void func_8003FA10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FA10: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8003FA14: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x8003FA18: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x8003FA1C: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x8003FA20: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    // 0x8003FA24: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FA28: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8003FA2C: sll         $a1, $a2, 16
    ctx->r5 = S32(ctx->r6 << 16);
    // 0x8003FA30: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x8003FA34: jal         0x801739B8
    // 0x8003FA38: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    func_801739B8(rdram, ctx);
        goto after_0;
    // 0x8003FA38: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    after_0:
    // 0x8003FA3C: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    // 0x8003FA40: lw          $a1, 0x44($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X44);
    // 0x8003FA44: jal         0x80173524
    // 0x8003FA48: lw          $a2, 0x40($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X40);
    func_80173524(rdram, ctx);
        goto after_1;
    // 0x8003FA48: lw          $a2, 0x40($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X40);
    after_1:
    // 0x8003FA4C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FA50: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8003FA54: jr          $ra
    // 0x8003FA58: nop

    return;
    // 0x8003FA58: nop

;}
RECOMP_FUNC void func_8003FA60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FA60: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8003FA64: jr          $ra
    // 0x8003FA68: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    return;
    // 0x8003FA68: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
;}
RECOMP_FUNC void func_8003FA6C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FA6C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8003FA70: jr          $ra
    // 0x8003FA74: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    return;
    // 0x8003FA74: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
;}
RECOMP_FUNC void func_8003FA78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FA78: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8003FA7C: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8003FA80: jr          $ra
    // 0x8003FA84: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    return;
    // 0x8003FA84: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
;}
RECOMP_FUNC void func_8003FA88(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FA88: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8003FA8C: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8003FA90: jr          $ra
    // 0x8003FA94: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    return;
    // 0x8003FA94: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
;}
RECOMP_FUNC void func_8003FA98(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FA98: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8003FA9C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FAA0: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8003FAA4: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8003FAA8: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8003FAAC: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8003FAB0: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8003FAB4: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8003FAB8: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8003FABC: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x8003FAC0: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8003FAC4: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8003FAC8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8003FACC: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x8003FAD0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8003FAD4: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8003FAD8: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8003FADC: lw          $t9, 0x4E18($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X4E18);
    // 0x8003FAE0: jalr        $t9
    // 0x8003FAE4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8003FAE4: nop

    after_0:
    // 0x8003FAE8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8003FAEC: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x8003FAF0: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8003FAF4: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x8003FAF8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FAFC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8003FB00: jr          $ra
    // 0x8003FB04: nop

    return;
    // 0x8003FB04: nop

;}
RECOMP_FUNC void func_8003FB08(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FB08: jr          $ra
    // 0x8003FB0C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x8003FB0C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8003FB10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FB10: jr          $ra
    // 0x8003FB14: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x8003FB14: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8003FB18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FB18: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8003FB1C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8003FB20: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8003FB24: andi        $v0, $a0, 0xFF
    ctx->r2 = ctx->r4 & 0XFF;
    // 0x8003FB28: sll         $a1, $v0, 16
    ctx->r5 = S32(ctx->r2 << 16);
    // 0x8003FB2C: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x8003FB30: sll         $v1, $v0, 16
    ctx->r3 = S32(ctx->r2 << 16);
    // 0x8003FB34: slti        $at, $a1, 0xF
    ctx->r1 = SIGNED(ctx->r5) < 0XF ? 1 : 0;
    // 0x8003FB38: bne         $at, $zero, L_8003FB48
    if (ctx->r1 != 0) {
        // 0x8003FB3C: sra         $v1, $v1, 16
        ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
            goto L_8003FB48;
    }
    // 0x8003FB3C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8003FB40: slti        $at, $a1, 0x21
    ctx->r1 = SIGNED(ctx->r5) < 0X21 ? 1 : 0;
    // 0x8003FB44: bne         $at, $zero, L_8003FB60
    if (ctx->r1 != 0) {
        // 0x8003FB48: lui         $t6, 0x801D
        ctx->r14 = S32(0X801D << 16);
            goto L_8003FB60;
    }
L_8003FB48:
    // 0x8003FB48: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8003FB4C: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x8003FB50: lui         $v1, 0x800B
    ctx->r3 = S32(0X800B << 16);
    // 0x8003FB54: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x8003FB58: addu        $v1, $v1, $t7
    ctx->r3 = ADD32(ctx->r3, ctx->r15);
    // 0x8003FB5C: lh          $v1, 0x4E22($v1)
    ctx->r3 = MEM_H(ctx->r3, 0X4E22);
L_8003FB60:
    // 0x8003FB60: addiu       $v1, $v1, -0xF
    ctx->r3 = ADD32(ctx->r3, -0XF);
    // 0x8003FB64: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8003FB68: jr          $ra
    // 0x8003FB6C: sra         $v0, $v1, 16
    ctx->r2 = S32(SIGNED(ctx->r3) >> 16);
    return;
    // 0x8003FB6C: sra         $v0, $v1, 16
    ctx->r2 = S32(SIGNED(ctx->r3) >> 16);
;}
RECOMP_FUNC void func_8003FB70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FB70: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8003FB74: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8003FB78: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x8003FB7C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8003FB80: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8003FB84: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x8003FB88: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8003FB8C: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8003FB90: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FB94: bne         $a1, $at, L_8003FBB0
    if (ctx->r5 != ctx->r1) {
        // 0x8003FB98: sw          $a2, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r6;
            goto L_8003FBB0;
    }
    // 0x8003FB98: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x8003FB9C: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8003FBA0: lh          $t6, -0x54C8($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54C8);
    // 0x8003FBA4: beq         $t6, $zero, L_8003FBB0
    if (ctx->r14 == 0) {
        // 0x8003FBA8: nop
    
            goto L_8003FBB0;
    }
    // 0x8003FBA8: nop

    // 0x8003FBAC: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
L_8003FBB0:
    // 0x8003FBB0: jal         0x8003FB18
    // 0x8003FBB4: sh          $a1, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r5;
    func_8003FB18(rdram, ctx);
        goto after_0;
    // 0x8003FBB4: sh          $a1, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r5;
    after_0:
    // 0x8003FBB8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8003FBBC: lh          $v1, -0x5470($v1)
    ctx->r3 = MEM_H(ctx->r3, -0X5470);
    // 0x8003FBC0: sll         $a0, $v0, 16
    ctx->r4 = S32(ctx->r2 << 16);
    // 0x8003FBC4: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x8003FBC8: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8003FBCC: beq         $v1, $at, L_8003FBDC
    if (ctx->r3 == ctx->r1) {
        // 0x8003FBD0: lh          $a1, 0x1E($sp)
        ctx->r5 = MEM_H(ctx->r29, 0X1E);
            goto L_8003FBDC;
    }
    // 0x8003FBD0: lh          $a1, 0x1E($sp)
    ctx->r5 = MEM_H(ctx->r29, 0X1E);
    // 0x8003FBD4: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x8003FBD8: bne         $v1, $at, L_8003FBEC
    if (ctx->r3 != ctx->r1) {
        // 0x8003FBDC: addiu       $at, $zero, 0xA
        ctx->r1 = ADD32(0, 0XA);
            goto L_8003FBEC;
    }
L_8003FBDC:
    // 0x8003FBDC: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x8003FBE0: bnel        $v0, $at, L_8003FBF0
    if (ctx->r2 != ctx->r1) {
        // 0x8003FBE4: sll         $t7, $a1, 3
        ctx->r15 = S32(ctx->r5 << 3);
            goto L_8003FBF0;
    }
    goto skip_0;
    // 0x8003FBE4: sll         $t7, $a1, 3
    ctx->r15 = S32(ctx->r5 << 3);
    skip_0:
    // 0x8003FBE8: addiu       $a0, $zero, 0x12
    ctx->r4 = ADD32(0, 0X12);
L_8003FBEC:
    // 0x8003FBEC: sll         $t7, $a1, 3
    ctx->r15 = S32(ctx->r5 << 3);
L_8003FBF0:
    // 0x8003FBF0: subu        $t7, $t7, $a1
    ctx->r15 = SUB32(ctx->r15, ctx->r5);
    // 0x8003FBF4: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x8003FBF8: addu        $t7, $t7, $a1
    ctx->r15 = ADD32(ctx->r15, ctx->r5);
    // 0x8003FBFC: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x8003FC00: sll         $t8, $a0, 1
    ctx->r24 = S32(ctx->r4 << 1);
    // 0x8003FC04: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x8003FC08: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x8003FC0C: addu        $v0, $v0, $t9
    ctx->r2 = ADD32(ctx->r2, ctx->r25);
    // 0x8003FC10: lh          $v0, 0x4EE8($v0)
    ctx->r2 = MEM_H(ctx->r2, 0X4EE8);
    // 0x8003FC14: addiu       $at, $zero, 0x28E
    ctx->r1 = ADD32(0, 0X28E);
    // 0x8003FC18: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8003FC1C: bne         $v0, $at, L_8003FC44
    if (ctx->r2 != ctx->r1) {
        // 0x8003FC20: addiu       $t9, $t9, 0x6294
        ctx->r25 = ADD32(ctx->r25, 0X6294);
            goto L_8003FC44;
    }
    // 0x8003FC20: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x8003FC24: lui         $v1, 0x800B
    ctx->r3 = S32(0X800B << 16);
    // 0x8003FC28: lw          $v1, 0x516C($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X516C);
    // 0x8003FC2C: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x8003FC30: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8003FC34: andi        $t0, $v1, 0x1
    ctx->r8 = ctx->r3 & 0X1;
    // 0x8003FC38: beq         $t0, $zero, L_8003FC44
    if (ctx->r8 == 0) {
        // 0x8003FC3C: sw          $v1, 0x516C($at)
        MEM_W(0X516C, ctx->r1) = ctx->r3;
            goto L_8003FC44;
    }
    // 0x8003FC3C: sw          $v1, 0x516C($at)
    MEM_W(0X516C, ctx->r1) = ctx->r3;
    // 0x8003FC40: addiu       $v0, $zero, 0x28F
    ctx->r2 = ADD32(0, 0X28F);
L_8003FC44:
    // 0x8003FC44: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    // 0x8003FC48: jalr        $t9
    // 0x8003FC4C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8003FC4C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x8003FC50: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FC54: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8003FC58: jr          $ra
    // 0x8003FC5C: nop

    return;
    // 0x8003FC5C: nop

;}
RECOMP_FUNC void func_8003FC60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FC60: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8003FC64: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8003FC68: sll         $a3, $a1, 16
    ctx->r7 = S32(ctx->r5 << 16);
    // 0x8003FC6C: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x8003FC70: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8003FC74: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8003FC78: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8003FC7C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FC80: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8003FC84: bne         $a3, $at, L_8003FCA0
    if (ctx->r7 != ctx->r1) {
        // 0x8003FC88: sw          $a2, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r6;
            goto L_8003FCA0;
    }
    // 0x8003FC88: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x8003FC8C: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8003FC90: lh          $t6, -0x54C8($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54C8);
    // 0x8003FC94: beq         $t6, $zero, L_8003FCA0
    if (ctx->r14 == 0) {
        // 0x8003FC98: nop
    
            goto L_8003FCA0;
    }
    // 0x8003FC98: nop

    // 0x8003FC9C: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
L_8003FCA0:
    // 0x8003FCA0: jal         0x8003FB18
    // 0x8003FCA4: sh          $a3, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r7;
    func_8003FB18(rdram, ctx);
        goto after_0;
    // 0x8003FCA4: sh          $a3, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r7;
    after_0:
    // 0x8003FCA8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8003FCAC: lh          $v1, -0x5470($v1)
    ctx->r3 = MEM_H(ctx->r3, -0X5470);
    // 0x8003FCB0: sll         $a2, $v0, 16
    ctx->r6 = S32(ctx->r2 << 16);
    // 0x8003FCB4: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x8003FCB8: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x8003FCBC: beq         $v1, $at, L_8003FCCC
    if (ctx->r3 == ctx->r1) {
        // 0x8003FCC0: lh          $a3, 0x1E($sp)
        ctx->r7 = MEM_H(ctx->r29, 0X1E);
            goto L_8003FCCC;
    }
    // 0x8003FCC0: lh          $a3, 0x1E($sp)
    ctx->r7 = MEM_H(ctx->r29, 0X1E);
    // 0x8003FCC4: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x8003FCC8: bne         $v1, $at, L_8003FCDC
    if (ctx->r3 != ctx->r1) {
        // 0x8003FCCC: addiu       $at, $zero, 0xA
        ctx->r1 = ADD32(0, 0XA);
            goto L_8003FCDC;
    }
L_8003FCCC:
    // 0x8003FCCC: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x8003FCD0: bnel        $v0, $at, L_8003FCE0
    if (ctx->r2 != ctx->r1) {
        // 0x8003FCD4: sll         $t7, $a3, 3
        ctx->r15 = S32(ctx->r7 << 3);
            goto L_8003FCE0;
    }
    goto skip_0;
    // 0x8003FCD4: sll         $t7, $a3, 3
    ctx->r15 = S32(ctx->r7 << 3);
    skip_0:
    // 0x8003FCD8: addiu       $a2, $zero, 0x12
    ctx->r6 = ADD32(0, 0X12);
L_8003FCDC:
    // 0x8003FCDC: sll         $t7, $a3, 3
    ctx->r15 = S32(ctx->r7 << 3);
L_8003FCE0:
    // 0x8003FCE0: subu        $t7, $t7, $a3
    ctx->r15 = SUB32(ctx->r15, ctx->r7);
    // 0x8003FCE4: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x8003FCE8: addu        $t7, $t7, $a3
    ctx->r15 = ADD32(ctx->r15, ctx->r7);
    // 0x8003FCEC: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x8003FCF0: sll         $t8, $a2, 1
    ctx->r24 = S32(ctx->r6 << 1);
    // 0x8003FCF4: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x8003FCF8: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x8003FCFC: addu        $v0, $v0, $t9
    ctx->r2 = ADD32(ctx->r2, ctx->r25);
    // 0x8003FD00: lh          $v0, 0x4F0E($v0)
    ctx->r2 = MEM_H(ctx->r2, 0X4F0E);
    // 0x8003FD04: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8003FD08: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x8003FD0C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x8003FD10: jalr        $t9
    // 0x8003FD14: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8003FD14: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    after_1:
    // 0x8003FD18: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FD1C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8003FD20: jr          $ra
    // 0x8003FD24: nop

    return;
    // 0x8003FD24: nop

;}
RECOMP_FUNC void func_8003FD28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FD28: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8003FD2C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8003FD30: sll         $a3, $a1, 16
    ctx->r7 = S32(ctx->r5 << 16);
    // 0x8003FD34: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x8003FD38: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8003FD3C: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8003FD40: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8003FD44: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FD48: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8003FD4C: bne         $a3, $at, L_8003FD68
    if (ctx->r7 != ctx->r1) {
        // 0x8003FD50: sw          $a2, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r6;
            goto L_8003FD68;
    }
    // 0x8003FD50: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x8003FD54: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8003FD58: lh          $t6, -0x54C8($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54C8);
    // 0x8003FD5C: beq         $t6, $zero, L_8003FD68
    if (ctx->r14 == 0) {
        // 0x8003FD60: nop
    
            goto L_8003FD68;
    }
    // 0x8003FD60: nop

    // 0x8003FD64: addiu       $a3, $zero, 0x4
    ctx->r7 = ADD32(0, 0X4);
L_8003FD68:
    // 0x8003FD68: jal         0x8003FB18
    // 0x8003FD6C: sh          $a3, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r7;
    func_8003FB18(rdram, ctx);
        goto after_0;
    // 0x8003FD6C: sh          $a3, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r7;
    after_0:
    // 0x8003FD70: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8003FD74: lh          $v1, -0x5470($v1)
    ctx->r3 = MEM_H(ctx->r3, -0X5470);
    // 0x8003FD78: sll         $a2, $v0, 16
    ctx->r6 = S32(ctx->r2 << 16);
    // 0x8003FD7C: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x8003FD80: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x8003FD84: beq         $v1, $at, L_8003FD94
    if (ctx->r3 == ctx->r1) {
        // 0x8003FD88: lh          $a3, 0x1E($sp)
        ctx->r7 = MEM_H(ctx->r29, 0X1E);
            goto L_8003FD94;
    }
    // 0x8003FD88: lh          $a3, 0x1E($sp)
    ctx->r7 = MEM_H(ctx->r29, 0X1E);
    // 0x8003FD8C: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x8003FD90: bne         $v1, $at, L_8003FDA4
    if (ctx->r3 != ctx->r1) {
        // 0x8003FD94: addiu       $at, $zero, 0xA
        ctx->r1 = ADD32(0, 0XA);
            goto L_8003FDA4;
    }
L_8003FD94:
    // 0x8003FD94: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x8003FD98: bnel        $v0, $at, L_8003FDA8
    if (ctx->r2 != ctx->r1) {
        // 0x8003FD9C: sll         $t7, $a3, 3
        ctx->r15 = S32(ctx->r7 << 3);
            goto L_8003FDA8;
    }
    goto skip_0;
    // 0x8003FD9C: sll         $t7, $a3, 3
    ctx->r15 = S32(ctx->r7 << 3);
    skip_0:
    // 0x8003FDA0: addiu       $a2, $zero, 0x12
    ctx->r6 = ADD32(0, 0X12);
L_8003FDA4:
    // 0x8003FDA4: sll         $t7, $a3, 3
    ctx->r15 = S32(ctx->r7 << 3);
L_8003FDA8:
    // 0x8003FDA8: subu        $t7, $t7, $a3
    ctx->r15 = SUB32(ctx->r15, ctx->r7);
    // 0x8003FDAC: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x8003FDB0: addu        $t7, $t7, $a3
    ctx->r15 = ADD32(ctx->r15, ctx->r7);
    // 0x8003FDB4: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x8003FDB8: sll         $t8, $a2, 1
    ctx->r24 = S32(ctx->r6 << 1);
    // 0x8003FDBC: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x8003FDC0: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x8003FDC4: addu        $v0, $v0, $t9
    ctx->r2 = ADD32(ctx->r2, ctx->r25);
    // 0x8003FDC8: lh          $v0, 0x4F34($v0)
    ctx->r2 = MEM_H(ctx->r2, 0X4F34);
    // 0x8003FDCC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8003FDD0: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x8003FDD4: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x8003FDD8: jalr        $t9
    // 0x8003FDDC: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8003FDDC: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    after_1:
    // 0x8003FDE0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FDE4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8003FDE8: jr          $ra
    // 0x8003FDEC: nop

    return;
    // 0x8003FDEC: nop

;}
RECOMP_FUNC void func_8003FDF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FDF0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8003FDF4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8003FDF8: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8003FDFC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FE00: jal         0x8003FB18
    // 0x8003FE04: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    func_8003FB18(rdram, ctx);
        goto after_0;
    // 0x8003FE04: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    after_0:
    // 0x8003FE08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FE0C: sll         $t6, $v0, 2
    ctx->r14 = S32(ctx->r2 << 2);
    // 0x8003FE10: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x8003FE14: addu        $at, $at, $t6
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x8003FE18: lwc1        $f0, 0x5124($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X5124);
    // 0x8003FE1C: jr          $ra
    // 0x8003FE20: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x8003FE20: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8003FE24(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FE24: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8003FE28: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FE2C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8003FE30: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x8003FE34: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x8003FE38: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8003FE3C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8003FE40: andi        $t6, $v0, 0x10
    ctx->r14 = ctx->r2 & 0X10;
    // 0x8003FE44: beq         $t6, $zero, L_8003FEF4
    if (ctx->r14 == 0) {
        // 0x8003FE48: andi        $t7, $v0, 0x4
        ctx->r15 = ctx->r2 & 0X4;
            goto L_8003FEF4;
    }
    // 0x8003FE48: andi        $t7, $v0, 0x4
    ctx->r15 = ctx->r2 & 0X4;
    // 0x8003FE4C: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x8003FE50: beql        $t7, $zero, L_8003FE80
    if (ctx->r15 == 0) {
        // 0x8003FE54: andi        $t9, $v0, 0x8
        ctx->r25 = ctx->r2 & 0X8;
            goto L_8003FE80;
    }
    goto skip_0;
    // 0x8003FE54: andi        $t9, $v0, 0x8
    ctx->r25 = ctx->r2 & 0X8;
    skip_0:
    // 0x8003FE58: lw          $t8, 0x64($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X64);
    // 0x8003FE5C: lh          $a0, 0x0($t8)
    ctx->r4 = MEM_H(ctx->r24, 0X0);
    // 0x8003FE60: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x8003FE64: jal         0x8003FDF0
    // 0x8003FE68: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_8003FDF0(rdram, ctx);
        goto after_0;
    // 0x8003FE68: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x8003FE6C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x8003FE70: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x8003FE74: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x8003FE78: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x8003FE7C: andi        $t9, $v0, 0x8
    ctx->r25 = ctx->r2 & 0X8;
L_8003FE80:
    // 0x8003FE80: beql        $t9, $zero, L_8003FEA4
    if (ctx->r25 == 0) {
        // 0x8003FE84: lui         $at, 0x3FF0
        ctx->r1 = S32(0X3FF0 << 16);
            goto L_8003FEA4;
    }
    goto skip_1;
    // 0x8003FE84: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    skip_1:
    // 0x8003FE88: lw          $t0, 0x4($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X4);
    // 0x8003FE8C: lh          $a0, 0x0($t0)
    ctx->r4 = MEM_H(ctx->r8, 0X0);
    // 0x8003FE90: jal         0x8003FDF0
    // 0x8003FE94: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_8003FDF0(rdram, ctx);
        goto after_1;
    // 0x8003FE94: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_1:
    // 0x8003FE98: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x8003FE9C: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x8003FEA0: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
L_8003FEA4:
    // 0x8003FEA4: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    // 0x8003FEA8: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8003FEAC: cvt.d.s     $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f4.d = CVT_D_S(ctx->f2.fl);
    // 0x8003FEB0: lwc1        $f8, 0x20($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X20);
    // 0x8003FEB4: sub.d       $f6, $f0, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = ctx->f0.d - ctx->f4.d;
    // 0x8003FEB8: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8003FEBC: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x8003FEC0: mul.d       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = MUL_D(ctx->f6.d, ctx->f10.d);
    // 0x8003FEC4: lwc1        $f6, 0x4($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8003FEC8: sub.d       $f18, $f0, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = ctx->f0.d - ctx->f16.d;
    // 0x8003FECC: lwc1        $f16, 0x8($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X8);
    // 0x8003FED0: cvt.s.d     $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f2.fl = CVT_S_D(ctx->f18.d);
    // 0x8003FED4: mul.s       $f8, $f4, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8003FED8: nop

    // 0x8003FEDC: mul.s       $f10, $f6, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8003FEE0: nop

    // 0x8003FEE4: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x8003FEE8: swc1        $f8, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->f8.u32l;
    // 0x8003FEEC: swc1        $f10, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->f10.u32l;
    // 0x8003FEF0: swc1        $f18, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->f18.u32l;
L_8003FEF4:
    // 0x8003FEF4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FEF8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8003FEFC: jr          $ra
    // 0x8003FF00: nop

    return;
    // 0x8003FF00: nop

;}
RECOMP_FUNC void func_8003FF04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FF04: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8003FF08: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8003FF0C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x8003FF10: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FF14: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8003FF18: jal         0x8003FB18
    // 0x8003FF1C: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_8003FB18(rdram, ctx);
        goto after_0;
    // 0x8003FF1C: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x8003FF20: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8003FF24: lh          $t6, -0x54CA($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54CA);
    // 0x8003FF28: sll         $t8, $v0, 1
    ctx->r24 = S32(ctx->r2 << 1);
    // 0x8003FF2C: lui         $v1, 0x800B
    ctx->r3 = S32(0X800B << 16);
    // 0x8003FF30: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x8003FF34: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x8003FF38: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x8003FF3C: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x8003FF40: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x8003FF44: addu        $t9, $t7, $t8
    ctx->r25 = ADD32(ctx->r15, ctx->r24);
    // 0x8003FF48: addu        $v1, $v1, $t9
    ctx->r3 = ADD32(ctx->r3, ctx->r25);
    // 0x8003FF4C: lh          $v1, 0x4EE8($v1)
    ctx->r3 = MEM_H(ctx->r3, 0X4EE8);
    // 0x8003FF50: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8003FF54: addiu       $t9, $t9, 0x648C
    ctx->r25 = ADD32(ctx->r25, 0X648C);
    // 0x8003FF58: lui         $a2, 0x3F4C
    ctx->r6 = S32(0X3F4C << 16);
    // 0x8003FF5C: ori         $a2, $a2, 0xCCCD
    ctx->r6 = ctx->r6 | 0XCCCD;
    // 0x8003FF60: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x8003FF64: jalr        $t9
    // 0x8003FF68: andi        $a0, $v1, 0xFFFF
    ctx->r4 = ctx->r3 & 0XFFFF;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8003FF68: andi        $a0, $v1, 0xFFFF
    ctx->r4 = ctx->r3 & 0XFFFF;
    after_1:
    // 0x8003FF6C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8003FF70: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8003FF74: jr          $ra
    // 0x8003FF78: nop

    return;
    // 0x8003FF78: nop

;}
RECOMP_FUNC void func_8003FF80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FF80: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8003FF84: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8003FF88: jr          $ra
    // 0x8003FF8C: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    return;
    // 0x8003FF8C: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
;}
RECOMP_FUNC void func_8003FF90(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8003FF90: addiu       $sp, $sp, -0x1A8
    ctx->r29 = ADD32(ctx->r29, -0X1A8);
    // 0x8003FF94: sw          $a0, 0x1A8($sp)
    MEM_W(0X1A8, ctx->r29) = ctx->r4;
    // 0x8003FF98: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8003FF9C: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    // 0x8003FFA0: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x8003FFA4: addiu       $a1, $zero, 0x1C
    ctx->r5 = ADD32(0, 0X1C);
    // 0x8003FFA8: jal         0x80000F30
    // 0x8003FFAC: sw          $a2, 0x1AC($sp)
    MEM_W(0X1AC, ctx->r29) = ctx->r6;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8003FFAC: sw          $a2, 0x1AC($sp)
    MEM_W(0X1AC, ctx->r29) = ctx->r6;
    after_0:
    // 0x8003FFB0: lw          $a0, 0x1A8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1A8);
    // 0x8003FFB4: jal         0x8003E5D8
    // 0x8003FFB8: addiu       $a1, $sp, 0x20
    ctx->r5 = ADD32(ctx->r29, 0X20);
    func_8003E5D8(rdram, ctx);
        goto after_1;
    // 0x8003FFB8: addiu       $a1, $sp, 0x20
    ctx->r5 = ADD32(ctx->r29, 0X20);
    after_1:
    // 0x8003FFBC: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x8003FFC0: lw          $a2, 0x1AC($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1AC);
    // 0x8003FFC4: sll         $t7, $v0, 0
    ctx->r15 = S32(ctx->r2 << 0);
    // 0x8003FFC8: bgez        $t7, L_8003FFD8
    if (SIGNED(ctx->r15) >= 0) {
        // 0x8003FFCC: sw          $v0, 0x0($a2)
        MEM_W(0X0, ctx->r6) = ctx->r2;
            goto L_8003FFD8;
    }
    // 0x8003FFCC: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x8003FFD0: b           L_8004006C
    // 0x8003FFD4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_8004006C;
    // 0x8003FFD4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_8003FFD8:
    // 0x8003FFD8: lw          $t8, 0x24($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X24);
    // 0x8003FFDC: sll         $t2, $v0, 2
    ctx->r10 = S32(ctx->r2 << 2);
    // 0x8003FFE0: andi        $t3, $v0, 0x8800
    ctx->r11 = ctx->r2 & 0X8800;
    // 0x8003FFE4: sw          $t8, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->r24;
    // 0x8003FFE8: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x8003FFEC: ori         $at, $zero, 0x8800
    ctx->r1 = 0 | 0X8800;
    // 0x8003FFF0: sw          $t9, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->r25;
    // 0x8003FFF4: lwc1        $f4, 0x2C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8003FFF8: swc1        $f4, 0xC($a2)
    MEM_W(0XC, ctx->r6) = ctx->f4.u32l;
    // 0x8003FFFC: lw          $t0, 0x84($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X84);
    // 0x80040000: sw          $t0, 0x10($a2)
    MEM_W(0X10, ctx->r6) = ctx->r8;
    // 0x80040004: lw          $t1, 0x90($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X90);
    // 0x80040008: sw          $t1, 0x14($a2)
    MEM_W(0X14, ctx->r6) = ctx->r9;
    // 0x8004000C: lwc1        $f6, 0x8C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X8C);
    // 0x80040010: bltz        $t2, L_80040020
    if (SIGNED(ctx->r10) < 0) {
        // 0x80040014: swc1        $f6, 0x18($a2)
        MEM_W(0X18, ctx->r6) = ctx->f6.u32l;
            goto L_80040020;
    }
    // 0x80040014: swc1        $f6, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->f6.u32l;
    // 0x80040018: b           L_8004006C
    // 0x8004001C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8004006C;
    // 0x8004001C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040020:
    // 0x80040020: bne         $t3, $at, L_8004005C
    if (ctx->r11 != ctx->r1) {
        // 0x80040024: andi        $t6, $v0, 0x8000
        ctx->r14 = ctx->r2 & 0X8000;
            goto L_8004005C;
    }
    // 0x80040024: andi        $t6, $v0, 0x8000
    ctx->r14 = ctx->r2 & 0X8000;
    // 0x80040028: lwc1        $f8, 0x18($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X18);
    // 0x8004002C: lwc1        $f16, 0xC($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0XC);
    // 0x80040030: neg.s       $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = -ctx->f8.fl;
    // 0x80040034: c.lt.s      $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f10.fl < ctx->f16.fl;
    // 0x80040038: nop

    // 0x8004003C: bc1fl       L_80040054
    if (!c1cs) {
        // 0x80040040: lw          $t5, 0x4($a2)
        ctx->r13 = MEM_W(ctx->r6, 0X4);
            goto L_80040054;
    }
    goto skip_0;
    // 0x80040040: lw          $t5, 0x4($a2)
    ctx->r13 = MEM_W(ctx->r6, 0X4);
    skip_0:
    // 0x80040044: lw          $t4, 0x10($a2)
    ctx->r12 = MEM_W(ctx->r6, 0X10);
    // 0x80040048: b           L_8004006C
    // 0x8004004C: lhu         $v0, 0x0($t4)
    ctx->r2 = MEM_HU(ctx->r12, 0X0);
        goto L_8004006C;
    // 0x8004004C: lhu         $v0, 0x0($t4)
    ctx->r2 = MEM_HU(ctx->r12, 0X0);
    // 0x80040050: lw          $t5, 0x4($a2)
    ctx->r13 = MEM_W(ctx->r6, 0X4);
L_80040054:
    // 0x80040054: b           L_8004006C
    // 0x80040058: lhu         $v0, 0x0($t5)
    ctx->r2 = MEM_HU(ctx->r13, 0X0);
        goto L_8004006C;
    // 0x80040058: lhu         $v0, 0x0($t5)
    ctx->r2 = MEM_HU(ctx->r13, 0X0);
L_8004005C:
    // 0x8004005C: beql        $t6, $zero, L_80040070
    if (ctx->r14 == 0) {
        // 0x80040060: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80040070;
    }
    goto skip_1;
    // 0x80040060: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80040064: lw          $t7, 0x4($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X4);
    // 0x80040068: lhu         $v0, 0x0($t7)
    ctx->r2 = MEM_HU(ctx->r15, 0X0);
L_8004006C:
    // 0x8004006C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80040070:
    // 0x80040070: addiu       $sp, $sp, 0x1A8
    ctx->r29 = ADD32(ctx->r29, 0X1A8);
    // 0x80040074: jr          $ra
    // 0x80040078: nop

    return;
    // 0x80040078: nop

;}
RECOMP_FUNC void func_8004007C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8004007C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80040080: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80040084: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80040088: jal         0x8003FF90
    // 0x8004008C: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_8003FF90(rdram, ctx);
        goto after_0;
    // 0x8004008C: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x80040090: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x80040094: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80040098: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x8004009C: lw          $t6, 0x4($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X4);
    // 0x800400A0: beql        $t6, $zero, L_800400BC
    if (ctx->r14 == 0) {
        // 0x800400A4: lw          $t7, 0x10($a1)
        ctx->r15 = MEM_W(ctx->r5, 0X10);
            goto L_800400BC;
    }
    goto skip_0;
    // 0x800400A4: lw          $t7, 0x10($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X10);
    skip_0:
    // 0x800400A8: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800400AC: lwc1        $f6, 0xC($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0XC);
    // 0x800400B0: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x800400B4: swc1        $f8, 0xC($a1)
    MEM_W(0XC, ctx->r5) = ctx->f8.u32l;
    // 0x800400B8: lw          $t7, 0x10($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X10);
L_800400BC:
    // 0x800400BC: beql        $t7, $zero, L_800400D8
    if (ctx->r15 == 0) {
        // 0x800400C0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_800400D8;
    }
    goto skip_1;
    // 0x800400C0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x800400C4: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x800400C8: lwc1        $f16, 0x18($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X18);
    // 0x800400CC: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x800400D0: swc1        $f18, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->f18.u32l;
    // 0x800400D4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_800400D8:
    // 0x800400D8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800400DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x800400E0: jr          $ra
    // 0x800400E4: nop

    return;
    // 0x800400E4: nop

;}
RECOMP_FUNC void func_800400E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800400E8: addiu       $sp, $sp, -0x1A0
    ctx->r29 = ADD32(ctx->r29, -0X1A0);
    // 0x800400EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800400F0: sw          $a0, 0x1A0($sp)
    MEM_W(0X1A0, ctx->r29) = ctx->r4;
    // 0x800400F4: sw          $a1, 0x1A4($sp)
    MEM_W(0X1A4, ctx->r29) = ctx->r5;
    // 0x800400F8: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x800400FC: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80040100: jal         0x80000F30
    // 0x80040104: sw          $a2, 0x1A8($sp)
    MEM_W(0X1A8, ctx->r29) = ctx->r6;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x80040104: sw          $a2, 0x1A8($sp)
    MEM_W(0X1A8, ctx->r29) = ctx->r6;
    after_0:
    // 0x80040108: lw          $a0, 0x1A0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1A0);
    // 0x8004010C: lw          $a1, 0x1A4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1A4);
    // 0x80040110: jal         0x8003F22C
    // 0x80040114: addiu       $a2, $sp, 0x18
    ctx->r6 = ADD32(ctx->r29, 0X18);
    func_8003F22C(rdram, ctx);
        goto after_1;
    // 0x80040114: addiu       $a2, $sp, 0x18
    ctx->r6 = ADD32(ctx->r29, 0X18);
    after_1:
    // 0x80040118: lw          $t6, 0x18($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X18);
    // 0x8004011C: lw          $a3, 0x1A8($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1A8);
    // 0x80040120: sll         $t8, $t6, 0
    ctx->r24 = S32(ctx->r14 << 0);
    // 0x80040124: bgez        $t8, L_80040134
    if (SIGNED(ctx->r24) >= 0) {
        // 0x80040128: sw          $t6, 0x0($a3)
        MEM_W(0X0, ctx->r7) = ctx->r14;
            goto L_80040134;
    }
    // 0x80040128: sw          $t6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r14;
    // 0x8004012C: b           L_8004015C
    // 0x80040130: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_8004015C;
    // 0x80040130: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80040134:
    // 0x80040134: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80040138: sw          $v0, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r2;
    // 0x8004013C: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
    // 0x80040140: sw          $t0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r8;
    // 0x80040144: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80040148: swc1        $f4, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->f4.u32l;
    // 0x8004014C: lwc1        $f6, 0x5C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80040150: swc1        $f6, 0x10($a3)
    MEM_W(0X10, ctx->r7) = ctx->f6.u32l;
    // 0x80040154: lwc1        $f8, 0x60($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X60);
    // 0x80040158: swc1        $f8, 0x14($a3)
    MEM_W(0X14, ctx->r7) = ctx->f8.u32l;
L_8004015C:
    // 0x8004015C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80040160: addiu       $sp, $sp, 0x1A0
    ctx->r29 = ADD32(ctx->r29, 0X1A0);
    // 0x80040164: jr          $ra
    // 0x80040168: nop

    return;
    // 0x80040168: nop

;}
RECOMP_FUNC void func_8004016C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8004016C: addiu       $sp, $sp, -0x1A0
    ctx->r29 = ADD32(ctx->r29, -0X1A0);
    // 0x80040170: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80040174: sw          $a0, 0x1A0($sp)
    MEM_W(0X1A0, ctx->r29) = ctx->r4;
    // 0x80040178: sw          $a1, 0x1A4($sp)
    MEM_W(0X1A4, ctx->r29) = ctx->r5;
    // 0x8004017C: addiu       $a1, $zero, 0x18
    ctx->r5 = ADD32(0, 0X18);
    // 0x80040180: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80040184: jal         0x80000F30
    // 0x80040188: sw          $a2, 0x1A8($sp)
    MEM_W(0X1A8, ctx->r29) = ctx->r6;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x80040188: sw          $a2, 0x1A8($sp)
    MEM_W(0X1A8, ctx->r29) = ctx->r6;
    after_0:
    // 0x8004018C: lw          $a0, 0x1A0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1A0);
    // 0x80040190: lw          $a1, 0x1A4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1A4);
    // 0x80040194: jal         0x8003F01C
    // 0x80040198: addiu       $a2, $sp, 0x18
    ctx->r6 = ADD32(ctx->r29, 0X18);
    func_8003F01C(rdram, ctx);
        goto after_1;
    // 0x80040198: addiu       $a2, $sp, 0x18
    ctx->r6 = ADD32(ctx->r29, 0X18);
    after_1:
    // 0x8004019C: lw          $t6, 0x18($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X18);
    // 0x800401A0: lw          $a3, 0x1A8($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1A8);
    // 0x800401A4: sll         $t8, $t6, 0
    ctx->r24 = S32(ctx->r14 << 0);
    // 0x800401A8: bgez        $t8, L_800401B8
    if (SIGNED(ctx->r24) >= 0) {
        // 0x800401AC: sw          $t6, 0x0($a3)
        MEM_W(0X0, ctx->r7) = ctx->r14;
            goto L_800401B8;
    }
    // 0x800401AC: sw          $t6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r14;
    // 0x800401B0: b           L_800401E0
    // 0x800401B4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_800401E0;
    // 0x800401B4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_800401B8:
    // 0x800401B8: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x800401BC: sw          $v0, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r2;
    // 0x800401C0: lw          $t0, 0x28($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X28);
    // 0x800401C4: sw          $t0, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r8;
    // 0x800401C8: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x800401CC: swc1        $f4, 0xC($a3)
    MEM_W(0XC, ctx->r7) = ctx->f4.u32l;
    // 0x800401D0: lwc1        $f6, 0x5C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x800401D4: swc1        $f6, 0x10($a3)
    MEM_W(0X10, ctx->r7) = ctx->f6.u32l;
    // 0x800401D8: lwc1        $f8, 0x60($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X60);
    // 0x800401DC: swc1        $f8, 0x14($a3)
    MEM_W(0X14, ctx->r7) = ctx->f8.u32l;
L_800401E0:
    // 0x800401E0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800401E4: addiu       $sp, $sp, 0x1A0
    ctx->r29 = ADD32(ctx->r29, 0X1A0);
    // 0x800401E8: jr          $ra
    // 0x800401EC: nop

    return;
    // 0x800401EC: nop

;}
RECOMP_FUNC void func_800401F0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800401F0: addiu       $sp, $sp, -0x1B0
    ctx->r29 = ADD32(ctx->r29, -0X1B0);
    // 0x800401F4: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x800401F8: lw          $s0, 0x1C0($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C0);
    // 0x800401FC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80040200: sw          $a0, 0x1B0($sp)
    MEM_W(0X1B0, ctx->r29) = ctx->r4;
    // 0x80040204: sw          $a1, 0x1B4($sp)
    MEM_W(0X1B4, ctx->r29) = ctx->r5;
    // 0x80040208: sw          $a2, 0x1B8($sp)
    MEM_W(0X1B8, ctx->r29) = ctx->r6;
    // 0x8004020C: sw          $a3, 0x1BC($sp)
    MEM_W(0X1BC, ctx->r29) = ctx->r7;
    // 0x80040210: addiu       $a1, $zero, 0x34
    ctx->r5 = ADD32(0, 0X34);
    // 0x80040214: jal         0x80000F30
    // 0x80040218: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x80040218: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
    // 0x8004021C: addiu       $t6, $sp, 0x28
    ctx->r14 = ADD32(ctx->r29, 0X28);
    // 0x80040220: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x80040224: lw          $a0, 0x1B0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1B0);
    // 0x80040228: lw          $a1, 0x1B4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1B4);
    // 0x8004022C: lw          $a2, 0x1B8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1B8);
    // 0x80040230: jal         0x8003ECC4
    // 0x80040234: lw          $a3, 0x1BC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1BC);
    func_8003ECC4(rdram, ctx);
        goto after_1;
    // 0x80040234: lw          $a3, 0x1BC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1BC);
    after_1:
    // 0x80040238: lw          $t7, 0x28($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X28);
    // 0x8004023C: lw          $v0, 0x1B0($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1B0);
    // 0x80040240: sll         $t9, $t7, 0
    ctx->r25 = S32(ctx->r15 << 0);
    // 0x80040244: bgez        $t9, L_80040254
    if (SIGNED(ctx->r25) >= 0) {
        // 0x80040248: sw          $t7, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r15;
            goto L_80040254;
    }
    // 0x80040248: sw          $t7, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r15;
    // 0x8004024C: b           L_800402D0
    // 0x80040250: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_800402D0;
    // 0x80040250: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80040254:
    // 0x80040254: lw          $t0, 0x2C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X2C);
    // 0x80040258: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x8004025C: addiu       $t9, $t9, 0x1E0
    ctx->r25 = ADD32(ctx->r25, 0X1E0);
    // 0x80040260: sw          $t0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r8;
    // 0x80040264: lw          $t1, 0x38($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X38);
    // 0x80040268: sw          $t1, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r9;
    // 0x8004026C: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80040270: lwc1        $f4, 0x70($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X70);
    // 0x80040274: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80040278: lwc1        $f8, 0x68($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X68);
    // 0x8004027C: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80040280: sub.s       $f2, $f8, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80040284: mul.s       $f16, $f0, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80040288: nop

    // 0x8004028C: mul.s       $f18, $f2, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80040290: jalr        $t9
    // 0x80040294: add.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f18.fl;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80040294: add.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f18.fl;
    after_2:
    // 0x80040298: swc1        $f0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f0.u32l;
    // 0x8004029C: lwc1        $f4, 0x68($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X68);
    // 0x800402A0: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x800402A4: swc1        $f4, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f4.u32l;
    // 0x800402A8: lwc1        $f6, 0x6C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x800402AC: swc1        $f6, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f6.u32l;
    // 0x800402B0: lwc1        $f8, 0x70($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X70);
    // 0x800402B4: swc1        $f8, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f8.u32l;
    // 0x800402B8: lw          $t2, 0x8C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X8C);
    // 0x800402BC: sw          $t2, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r10;
    // 0x800402C0: lw          $t3, 0x98($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X98);
    // 0x800402C4: sw          $t3, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->r11;
    // 0x800402C8: lwc1        $f10, 0x94($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X94);
    // 0x800402CC: swc1        $f10, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f10.u32l;
L_800402D0:
    // 0x800402D0: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x800402D4: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x800402D8: addiu       $sp, $sp, 0x1B0
    ctx->r29 = ADD32(ctx->r29, 0X1B0);
    // 0x800402DC: jr          $ra
    // 0x800402E0: nop

    return;
    // 0x800402E0: nop

;}
RECOMP_FUNC void func_800402E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800402E4: addiu       $sp, $sp, -0x1B0
    ctx->r29 = ADD32(ctx->r29, -0X1B0);
    // 0x800402E8: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x800402EC: lw          $s0, 0x1C0($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C0);
    // 0x800402F0: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x800402F4: sw          $a0, 0x1B0($sp)
    MEM_W(0X1B0, ctx->r29) = ctx->r4;
    // 0x800402F8: sw          $a1, 0x1B4($sp)
    MEM_W(0X1B4, ctx->r29) = ctx->r5;
    // 0x800402FC: sw          $a2, 0x1B8($sp)
    MEM_W(0X1B8, ctx->r29) = ctx->r6;
    // 0x80040300: sw          $a3, 0x1BC($sp)
    MEM_W(0X1BC, ctx->r29) = ctx->r7;
    // 0x80040304: addiu       $a1, $zero, 0x34
    ctx->r5 = ADD32(0, 0X34);
    // 0x80040308: jal         0x80000F30
    // 0x8004030C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8004030C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
    // 0x80040310: addiu       $t6, $sp, 0x28
    ctx->r14 = ADD32(ctx->r29, 0X28);
    // 0x80040314: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x80040318: lw          $a0, 0x1B0($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1B0);
    // 0x8004031C: lw          $a1, 0x1B4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1B4);
    // 0x80040320: lw          $a2, 0x1B8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1B8);
    // 0x80040324: jal         0x8003F44C
    // 0x80040328: lw          $a3, 0x1BC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1BC);
    func_8003F44C(rdram, ctx);
        goto after_1;
    // 0x80040328: lw          $a3, 0x1BC($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1BC);
    after_1:
    // 0x8004032C: lw          $t7, 0x28($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X28);
    // 0x80040330: lw          $v0, 0x1B0($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1B0);
    // 0x80040334: sll         $t9, $t7, 0
    ctx->r25 = S32(ctx->r15 << 0);
    // 0x80040338: bgez        $t9, L_80040348
    if (SIGNED(ctx->r25) >= 0) {
        // 0x8004033C: sw          $t7, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r15;
            goto L_80040348;
    }
    // 0x8004033C: sw          $t7, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r15;
    // 0x80040340: b           L_800403AC
    // 0x80040344: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_800403AC;
    // 0x80040344: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80040348:
    // 0x80040348: lw          $t0, 0x2C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X2C);
    // 0x8004034C: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x80040350: addiu       $t9, $t9, 0x1E0
    ctx->r25 = ADD32(ctx->r25, 0X1E0);
    // 0x80040354: sw          $t0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r8;
    // 0x80040358: lw          $t1, 0x38($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X38);
    // 0x8004035C: sw          $t1, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r9;
    // 0x80040360: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80040364: lwc1        $f4, 0x70($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X70);
    // 0x80040368: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8004036C: lwc1        $f8, 0x68($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80040370: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80040374: sub.s       $f2, $f8, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80040378: mul.s       $f16, $f0, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x8004037C: nop

    // 0x80040380: mul.s       $f18, $f2, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80040384: jalr        $t9
    // 0x80040388: add.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f18.fl;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80040388: add.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f18.fl;
    after_2:
    // 0x8004038C: swc1        $f0, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f0.u32l;
    // 0x80040390: lwc1        $f4, 0x68($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80040394: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x80040398: swc1        $f4, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f4.u32l;
    // 0x8004039C: lwc1        $f6, 0x6C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x800403A0: swc1        $f6, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f6.u32l;
    // 0x800403A4: lwc1        $f8, 0x70($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X70);
    // 0x800403A8: swc1        $f8, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f8.u32l;
L_800403AC:
    // 0x800403AC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x800403B0: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x800403B4: addiu       $sp, $sp, 0x1B0
    ctx->r29 = ADD32(ctx->r29, 0X1B0);
    // 0x800403B8: jr          $ra
    // 0x800403BC: nop

    return;
    // 0x800403BC: nop

;}
RECOMP_FUNC void func_800403C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800403C0: addiu       $sp, $sp, -0x1B8
    ctx->r29 = ADD32(ctx->r29, -0X1B8);
    // 0x800403C4: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x800403C8: lw          $s0, 0x1C8($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C8);
    // 0x800403CC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x800403D0: sw          $a0, 0x1B8($sp)
    MEM_W(0X1B8, ctx->r29) = ctx->r4;
    // 0x800403D4: sw          $a1, 0x1BC($sp)
    MEM_W(0X1BC, ctx->r29) = ctx->r5;
    // 0x800403D8: sw          $a2, 0x1C0($sp)
    MEM_W(0X1C0, ctx->r29) = ctx->r6;
    // 0x800403DC: sw          $a3, 0x1C4($sp)
    MEM_W(0X1C4, ctx->r29) = ctx->r7;
    // 0x800403E0: addiu       $a1, $zero, 0x2C
    ctx->r5 = ADD32(0, 0X2C);
    // 0x800403E4: jal         0x80000F30
    // 0x800403E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x800403E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
    // 0x800403EC: addiu       $t6, $sp, 0x30
    ctx->r14 = ADD32(ctx->r29, 0X30);
    // 0x800403F0: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x800403F4: lw          $a0, 0x1B8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1B8);
    // 0x800403F8: lw          $a1, 0x1C4($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C4);
    // 0x800403FC: lw          $a2, 0x1BC($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1BC);
    // 0x80040400: jal         0x8003F72C
    // 0x80040404: lw          $a3, 0x1C0($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1C0);
    func_8003F72C(rdram, ctx);
        goto after_1;
    // 0x80040404: lw          $a3, 0x1C0($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1C0);
    after_1:
    // 0x80040408: lw          $t7, 0x30($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X30);
    // 0x8004040C: sll         $t9, $t7, 0
    ctx->r25 = S32(ctx->r15 << 0);
    // 0x80040410: bgez        $t9, L_80040420
    if (SIGNED(ctx->r25) >= 0) {
        // 0x80040414: sw          $t7, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r15;
            goto L_80040420;
    }
    // 0x80040414: sw          $t7, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r15;
    // 0x80040418: b           L_800404A0
    // 0x8004041C: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
        goto L_800404A0;
    // 0x8004041C: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80040420:
    // 0x80040420: lw          $t0, 0x34($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X34);
    // 0x80040424: sw          $t0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r8;
    // 0x80040428: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x8004042C: sw          $t1, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r9;
    // 0x80040430: lwc1        $f4, 0x3C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x80040434: swc1        $f4, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f4.u32l;
    // 0x80040438: lwc1        $f6, 0x44($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8004043C: swc1        $f6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f6.u32l;
    // 0x80040440: lwc1        $f8, 0x4C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80040444: swc1        $f8, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f8.u32l;
    // 0x80040448: lw          $t2, 0x94($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X94);
    // 0x8004044C: sw          $t2, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r10;
    // 0x80040450: lw          $t3, 0xA0($sp)
    ctx->r11 = MEM_W(ctx->r29, 0XA0);
    // 0x80040454: sw          $t3, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r11;
    // 0x80040458: lwc1        $f10, 0x9C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x8004045C: swc1        $f10, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f10.u32l;
    // 0x80040460: lwc1        $f16, 0xA4($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0XA4);
    // 0x80040464: swc1        $f16, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f16.u32l;
    // 0x80040468: lwc1        $f18, 0xAC($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XAC);
    // 0x8004046C: swc1        $f18, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f18.u32l;
    // 0x80040470: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x80040474: sll         $t4, $v0, 9
    ctx->r12 = S32(ctx->r2 << 9);
    // 0x80040478: bgez        $t4, L_80040488
    if (SIGNED(ctx->r12) >= 0) {
        // 0x8004047C: sll         $t5, $v0, 8
        ctx->r13 = S32(ctx->r2 << 8);
            goto L_80040488;
    }
    // 0x8004047C: sll         $t5, $v0, 8
    ctx->r13 = S32(ctx->r2 << 8);
    // 0x80040480: b           L_8004049C
    // 0x80040484: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
        goto L_8004049C;
    // 0x80040484: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
L_80040488:
    // 0x80040488: bgez        $t5, L_80040498
    if (SIGNED(ctx->r13) >= 0) {
        // 0x8004048C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80040498;
    }
    // 0x8004048C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80040490: b           L_80040498
    // 0x80040494: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80040498;
    // 0x80040494: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80040498:
    // 0x80040498: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_8004049C:
    // 0x8004049C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_800404A0:
    // 0x800404A0: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x800404A4: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x800404A8: addiu       $sp, $sp, 0x1B8
    ctx->r29 = ADD32(ctx->r29, 0X1B8);
    // 0x800404AC: jr          $ra
    // 0x800404B0: nop

    return;
    // 0x800404B0: nop

;}
RECOMP_FUNC void atari_work_table_init(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800404D0: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x800404D4: addiu       $a0, $a0, -0x6F18
    ctx->r4 = ADD32(ctx->r4, -0X6F18);
    // 0x800404D8: b           L_800404FC
    // 0x800404DC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
        goto L_800404FC;
    // 0x800404DC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x800404E0: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
L_800404E4:
    // 0x800404E4: addiu       $v0, $v0, -0x9C
    ctx->r2 = ADD32(ctx->r2, -0X9C);
    // 0x800404E8: bne         $v1, $v0, L_800404E4
    if (ctx->r3 != ctx->r2) {
        // 0x800404EC: sh          $zero, 0x0($v0)
        MEM_H(0X0, ctx->r2) = 0;
            goto L_800404E4;
    }
    // 0x800404EC: sh          $zero, 0x0($v0)
    MEM_H(0X0, ctx->r2) = 0;
    // 0x800404F0: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x800404F4: addiu       $v1, $v1, 0x42E8
    ctx->r3 = ADD32(ctx->r3, 0X42E8);
    // 0x800404F8: beq         $v0, $v1, L_8004051C
    if (ctx->r2 == ctx->r3) {
        // 0x800404FC: lui         $v1, 0x800F
        ctx->r3 = S32(0X800F << 16);
            goto L_8004051C;
    }
L_800404FC:
    // 0x800404FC: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x80040500: addiu       $v1, $v1, 0x42E8
    ctx->r3 = ADD32(ctx->r3, 0X42E8);
L_80040504:
    // 0x80040504: addiu       $v0, $v0, -0x270
    ctx->r2 = ADD32(ctx->r2, -0X270);
    // 0x80040508: sh          $zero, 0x1D4($v0)
    MEM_H(0X1D4, ctx->r2) = 0;
    // 0x8004050C: sh          $zero, 0x138($v0)
    MEM_H(0X138, ctx->r2) = 0;
    // 0x80040510: sh          $zero, 0x9C($v0)
    MEM_H(0X9C, ctx->r2) = 0;
    // 0x80040514: bne         $v0, $v1, L_80040504
    if (ctx->r2 != ctx->r3) {
        // 0x80040518: sh          $zero, 0x0($v0)
        MEM_H(0X0, ctx->r2) = 0;
            goto L_80040504;
    }
    // 0x80040518: sh          $zero, 0x0($v0)
    MEM_H(0X0, ctx->r2) = 0;
L_8004051C:
    // 0x8004051C: jr          $ra
    // 0x80040520: nop

    return;
    // 0x80040520: nop

;}
RECOMP_FUNC void poolObject_create(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040524: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x80040528: addiu       $v0, $v0, 0x42E8
    ctx->r2 = ADD32(ctx->r2, 0X42E8);
    // 0x8004052C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80040530: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80040534: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80040538: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x8004053C: lh          $t6, 0x0($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X0);
L_80040540:
    // 0x80040540: addiu       $t8, $v0, 0x4E00
    ctx->r24 = ADD32(ctx->r2, 0X4E00);
    // 0x80040544: andi        $t7, $t6, 0x3000
    ctx->r15 = ctx->r14 & 0X3000;
    // 0x80040548: bne         $t7, $zero, L_80040560
    if (ctx->r15 != 0) {
        // 0x8004054C: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_80040560;
    }
    // 0x8004054C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80040550: jal         0x80040580
    // 0x80040554: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    func_80040580(rdram, ctx);
        goto after_0;
    // 0x80040554: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    after_0:
    // 0x80040558: b           L_80040570
    // 0x8004055C: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
        goto L_80040570;
    // 0x8004055C: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
L_80040560:
    // 0x80040560: addiu       $a2, $a2, 0x9C
    ctx->r6 = ADD32(ctx->r6, 0X9C);
    // 0x80040564: bnel        $a2, $t8, L_80040540
    if (ctx->r6 != ctx->r24) {
        // 0x80040568: lh          $t6, 0x0($a2)
        ctx->r14 = MEM_H(ctx->r6, 0X0);
            goto L_80040540;
    }
    goto skip_0;
    // 0x80040568: lh          $t6, 0x0($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X0);
    skip_0:
    // 0x8004056C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040570:
    // 0x80040570: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80040574: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80040578: jr          $ra
    // 0x8004057C: nop

    return;
    // 0x8004057C: nop

;}
RECOMP_FUNC void func_80040580(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040580: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80040584: addiu       $t6, $zero, 0x2000
    ctx->r14 = ADD32(0, 0X2000);
    // 0x80040588: sh          $t6, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r14;
    // 0x8004058C: sh          $zero, 0x2($a0)
    MEM_H(0X2, ctx->r4) = 0;
    // 0x80040590: sh          $zero, 0x4($a0)
    MEM_H(0X4, ctx->r4) = 0;
    // 0x80040594: sh          $zero, 0x6($a0)
    MEM_H(0X6, ctx->r4) = 0;
    // 0x80040598: sh          $zero, 0x8($a0)
    MEM_H(0X8, ctx->r4) = 0;
    // 0x8004059C: sh          $zero, 0xA($a0)
    MEM_H(0XA, ctx->r4) = 0;
    // 0x800405A0: sb          $zero, 0xD($a0)
    MEM_B(0XD, ctx->r4) = 0;
    // 0x800405A4: sh          $zero, 0xE($a0)
    MEM_H(0XE, ctx->r4) = 0;
    // 0x800405A8: sw          $zero, 0x1C($a0)
    MEM_W(0X1C, ctx->r4) = 0;
    // 0x800405AC: sw          $zero, 0x20($a0)
    MEM_W(0X20, ctx->r4) = 0;
    // 0x800405B0: sh          $zero, 0x44($a0)
    MEM_H(0X44, ctx->r4) = 0;
    // 0x800405B4: sh          $zero, 0x46($a0)
    MEM_H(0X46, ctx->r4) = 0;
    // 0x800405B8: sh          $zero, 0x48($a0)
    MEM_H(0X48, ctx->r4) = 0;
    // 0x800405BC: sh          $zero, 0x4A($a0)
    MEM_H(0X4A, ctx->r4) = 0;
    // 0x800405C0: sw          $zero, 0x4C($a0)
    MEM_W(0X4C, ctx->r4) = 0;
    // 0x800405C4: sw          $zero, 0x50($a0)
    MEM_W(0X50, ctx->r4) = 0;
    // 0x800405C8: sw          $zero, 0x54($a0)
    MEM_W(0X54, ctx->r4) = 0;
    // 0x800405CC: sw          $a1, 0x58($a0)
    MEM_W(0X58, ctx->r4) = ctx->r5;
    // 0x800405D0: sw          $zero, 0x60($a0)
    MEM_W(0X60, ctx->r4) = 0;
    // 0x800405D4: sw          $zero, 0x70($a0)
    MEM_W(0X70, ctx->r4) = 0;
    // 0x800405D8: sw          $zero, 0x64($a0)
    MEM_W(0X64, ctx->r4) = 0;
    // 0x800405DC: sw          $zero, 0x74($a0)
    MEM_W(0X74, ctx->r4) = 0;
    // 0x800405E0: sw          $zero, 0x68($a0)
    MEM_W(0X68, ctx->r4) = 0;
    // 0x800405E4: sw          $zero, 0x78($a0)
    MEM_W(0X78, ctx->r4) = 0;
    // 0x800405E8: sw          $zero, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = 0;
    // 0x800405EC: sw          $zero, 0x7C($a0)
    MEM_W(0X7C, ctx->r4) = 0;
    // 0x800405F0: sb          $zero, 0x80($a0)
    MEM_B(0X80, ctx->r4) = 0;
    // 0x800405F4: sb          $zero, 0x81($a0)
    MEM_B(0X81, ctx->r4) = 0;
    // 0x800405F8: sb          $zero, 0x82($a0)
    MEM_B(0X82, ctx->r4) = 0;
    // 0x800405FC: sb          $zero, 0x83($a0)
    MEM_B(0X83, ctx->r4) = 0;
    // 0x80040600: swc1        $f0, 0x10($a0)
    MEM_W(0X10, ctx->r4) = ctx->f0.u32l;
    // 0x80040604: swc1        $f0, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->f0.u32l;
    // 0x80040608: swc1        $f0, 0x18($a0)
    MEM_W(0X18, ctx->r4) = ctx->f0.u32l;
    // 0x8004060C: swc1        $f0, 0x24($a0)
    MEM_W(0X24, ctx->r4) = ctx->f0.u32l;
    // 0x80040610: swc1        $f0, 0x28($a0)
    MEM_W(0X28, ctx->r4) = ctx->f0.u32l;
    // 0x80040614: swc1        $f0, 0x2C($a0)
    MEM_W(0X2C, ctx->r4) = ctx->f0.u32l;
    // 0x80040618: swc1        $f0, 0x30($a0)
    MEM_W(0X30, ctx->r4) = ctx->f0.u32l;
    // 0x8004061C: swc1        $f0, 0x34($a0)
    MEM_W(0X34, ctx->r4) = ctx->f0.u32l;
    // 0x80040620: jr          $ra
    // 0x80040624: swc1        $f0, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->f0.u32l;
    return;
    // 0x80040624: swc1        $f0, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->f0.u32l;
;}
RECOMP_FUNC void poolObject_attachChild(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040628: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8004062C: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x80040630: addiu       $v0, $v0, 0x42E8
    ctx->r2 = ADD32(ctx->r2, 0X42E8);
    // 0x80040634: sll         $a2, $a1, 16
    ctx->r6 = S32(ctx->r5 << 16);
    // 0x80040638: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x8004063C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80040640: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80040644: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80040648: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x8004064C: lh          $t6, 0x0($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X0);
L_80040650:
    // 0x80040650: addiu       $t8, $v0, 0x4E00
    ctx->r24 = ADD32(ctx->r2, 0X4E00);
    // 0x80040654: andi        $t7, $t6, 0x3000
    ctx->r15 = ctx->r14 & 0X3000;
    // 0x80040658: bne         $t7, $zero, L_80040674
    if (ctx->r15 != 0) {
        // 0x8004065C: or          $a0, $a3, $zero
        ctx->r4 = ctx->r7 | 0;
            goto L_80040674;
    }
    // 0x8004065C: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80040660: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80040664: jal         0x80040694
    // 0x80040668: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    func_80040694(rdram, ctx);
        goto after_0;
    // 0x80040668: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    after_0:
    // 0x8004066C: b           L_80040684
    // 0x80040670: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
        goto L_80040684;
    // 0x80040670: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
L_80040674:
    // 0x80040674: addiu       $a3, $a3, 0x9C
    ctx->r7 = ADD32(ctx->r7, 0X9C);
    // 0x80040678: bnel        $a3, $t8, L_80040650
    if (ctx->r7 != ctx->r24) {
        // 0x8004067C: lh          $t6, 0x0($a3)
        ctx->r14 = MEM_H(ctx->r7, 0X0);
            goto L_80040650;
    }
    goto skip_0;
    // 0x8004067C: lh          $t6, 0x0($a3)
    ctx->r14 = MEM_H(ctx->r7, 0X0);
    skip_0:
    // 0x80040680: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040684:
    // 0x80040684: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80040688: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8004068C: jr          $ra
    // 0x80040690: nop

    return;
    // 0x80040690: nop

;}
RECOMP_FUNC void func_80040694(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040694: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80040698: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8004069C: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x800406A0: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x800406A4: addiu       $t6, $a2, 0x1000
    ctx->r14 = ADD32(ctx->r6, 0X1000);
    // 0x800406A8: sh          $t6, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r14;
    // 0x800406AC: sh          $zero, 0x2($a0)
    MEM_H(0X2, ctx->r4) = 0;
    // 0x800406B0: sh          $zero, 0x4($a0)
    MEM_H(0X4, ctx->r4) = 0;
    // 0x800406B4: sh          $zero, 0x6($a0)
    MEM_H(0X6, ctx->r4) = 0;
    // 0x800406B8: sh          $zero, 0x8($a0)
    MEM_H(0X8, ctx->r4) = 0;
    // 0x800406BC: sh          $zero, 0xC($a0)
    MEM_H(0XC, ctx->r4) = 0;
    // 0x800406C0: sh          $zero, 0x12($a0)
    MEM_H(0X12, ctx->r4) = 0;
    // 0x800406C4: sh          $zero, 0x14($a0)
    MEM_H(0X14, ctx->r4) = 0;
    // 0x800406C8: sh          $zero, 0x16($a0)
    MEM_H(0X16, ctx->r4) = 0;
    // 0x800406CC: sw          $a1, 0x2C($a0)
    MEM_W(0X2C, ctx->r4) = ctx->r5;
    // 0x800406D0: sw          $zero, 0x30($a0)
    MEM_W(0X30, ctx->r4) = 0;
    // 0x800406D4: sw          $zero, 0x34($a0)
    MEM_W(0X34, ctx->r4) = 0;
    // 0x800406D8: sw          $zero, 0x38($a0)
    MEM_W(0X38, ctx->r4) = 0;
    // 0x800406DC: sw          $zero, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = 0;
    // 0x800406E0: sw          $zero, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = 0;
    // 0x800406E4: sh          $zero, 0x90($a0)
    MEM_H(0X90, ctx->r4) = 0;
    // 0x800406E8: sh          $zero, 0x92($a0)
    MEM_H(0X92, ctx->r4) = 0;
    // 0x800406EC: sh          $zero, 0x94($a0)
    MEM_H(0X94, ctx->r4) = 0;
    // 0x800406F0: sh          $zero, 0x96($a0)
    MEM_H(0X96, ctx->r4) = 0;
    // 0x800406F4: swc1        $f0, 0x18($a0)
    MEM_W(0X18, ctx->r4) = ctx->f0.u32l;
    // 0x800406F8: swc1        $f0, 0x1C($a0)
    MEM_W(0X1C, ctx->r4) = ctx->f0.u32l;
    // 0x800406FC: swc1        $f0, 0x20($a0)
    MEM_W(0X20, ctx->r4) = ctx->f0.u32l;
    // 0x80040700: swc1        $f0, 0x24($a0)
    MEM_W(0X24, ctx->r4) = ctx->f0.u32l;
    // 0x80040704: swc1        $f0, 0x28($a0)
    MEM_W(0X28, ctx->r4) = ctx->f0.u32l;
    // 0x80040708: swc1        $f0, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->f0.u32l;
    // 0x8004070C: swc1        $f0, 0x74($a0)
    MEM_W(0X74, ctx->r4) = ctx->f0.u32l;
    // 0x80040710: swc1        $f0, 0x78($a0)
    MEM_W(0X78, ctx->r4) = ctx->f0.u32l;
    // 0x80040714: swc1        $f0, 0x7C($a0)
    MEM_W(0X7C, ctx->r4) = ctx->f0.u32l;
    // 0x80040718: swc1        $f0, 0x80($a0)
    MEM_W(0X80, ctx->r4) = ctx->f0.u32l;
    // 0x8004071C: jr          $ra
    // 0x80040720: swc1        $f0, 0x84($a0)
    MEM_W(0X84, ctx->r4) = ctx->f0.u32l;
    return;
    // 0x80040720: swc1        $f0, 0x84($a0)
    MEM_W(0X84, ctx->r4) = ctx->f0.u32l;
;}
RECOMP_FUNC void func_80040724(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040724: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80040728: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x8004072C: addiu       $v0, $v0, 0x42E8
    ctx->r2 = ADD32(ctx->r2, 0X42E8);
    // 0x80040730: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80040734: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80040738: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8004073C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80040740: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x80040744: andi        $a3, $a2, 0xFFFF
    ctx->r7 = ctx->r6 & 0XFFFF;
    // 0x80040748: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x8004074C: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
L_80040750:
    // 0x80040750: addiu       $t8, $v0, 0x4E00
    ctx->r24 = ADD32(ctx->r2, 0X4E00);
    // 0x80040754: andi        $t7, $t6, 0x3000
    ctx->r15 = ctx->r14 & 0X3000;
    // 0x80040758: bne         $t7, $zero, L_80040774
    if (ctx->r15 != 0) {
        // 0x8004075C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80040774;
    }
    // 0x8004075C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040760: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80040764: jal         0x80040798
    // 0x80040768: lh          $a2, 0x26($sp)
    ctx->r6 = MEM_H(ctx->r29, 0X26);
    func_80040798(rdram, ctx);
        goto after_0;
    // 0x80040768: lh          $a2, 0x26($sp)
    ctx->r6 = MEM_H(ctx->r29, 0X26);
    after_0:
    // 0x8004076C: b           L_80040784
    // 0x80040770: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
        goto L_80040784;
    // 0x80040770: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80040774:
    // 0x80040774: addiu       $s0, $s0, 0x9C
    ctx->r16 = ADD32(ctx->r16, 0X9C);
    // 0x80040778: bnel        $s0, $t8, L_80040750
    if (ctx->r16 != ctx->r24) {
        // 0x8004077C: lh          $t6, 0x0($s0)
        ctx->r14 = MEM_H(ctx->r16, 0X0);
            goto L_80040750;
    }
    goto skip_0;
    // 0x8004077C: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    skip_0:
    // 0x80040780: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040784:
    // 0x80040784: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80040788: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8004078C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80040790: jr          $ra
    // 0x80040794: nop

    return;
    // 0x80040794: nop

;}
RECOMP_FUNC void func_80040798(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040798: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x8004079C: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    // 0x800407A0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800407A4: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x800407A8: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x800407AC: addu        $t6, $a2, $a3
    ctx->r14 = ADD32(ctx->r6, ctx->r7);
    // 0x800407B0: addiu       $t7, $t6, 0x3000
    ctx->r15 = ADD32(ctx->r14, 0X3000);
    // 0x800407B4: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    // 0x800407B8: sh          $zero, 0x2($a0)
    MEM_H(0X2, ctx->r4) = 0;
    // 0x800407BC: sh          $zero, 0x4($a0)
    MEM_H(0X4, ctx->r4) = 0;
    // 0x800407C0: sh          $zero, 0x6($a0)
    MEM_H(0X6, ctx->r4) = 0;
    // 0x800407C4: sh          $zero, 0x8($a0)
    MEM_H(0X8, ctx->r4) = 0;
    // 0x800407C8: sh          $zero, 0xA($a0)
    MEM_H(0XA, ctx->r4) = 0;
    // 0x800407CC: sb          $zero, 0xD($a0)
    MEM_B(0XD, ctx->r4) = 0;
    // 0x800407D0: sh          $zero, 0xE($a0)
    MEM_H(0XE, ctx->r4) = 0;
    // 0x800407D4: sw          $zero, 0x1C($a0)
    MEM_W(0X1C, ctx->r4) = 0;
    // 0x800407D8: sb          $zero, 0x4A($a0)
    MEM_B(0X4A, ctx->r4) = 0;
    // 0x800407DC: sb          $zero, 0x4B($a0)
    MEM_B(0X4B, ctx->r4) = 0;
    // 0x800407E0: sh          $zero, 0x40($a0)
    MEM_H(0X40, ctx->r4) = 0;
    // 0x800407E4: sh          $zero, 0x42($a0)
    MEM_H(0X42, ctx->r4) = 0;
    // 0x800407E8: sh          $zero, 0x44($a0)
    MEM_H(0X44, ctx->r4) = 0;
    // 0x800407EC: sh          $zero, 0x46($a0)
    MEM_H(0X46, ctx->r4) = 0;
    // 0x800407F0: sh          $zero, 0x48($a0)
    MEM_H(0X48, ctx->r4) = 0;
    // 0x800407F4: sh          $zero, 0x4C($a0)
    MEM_H(0X4C, ctx->r4) = 0;
    // 0x800407F8: sh          $zero, 0x4E($a0)
    MEM_H(0X4E, ctx->r4) = 0;
    // 0x800407FC: sh          $zero, 0x50($a0)
    MEM_H(0X50, ctx->r4) = 0;
    // 0x80040800: sh          $zero, 0x52($a0)
    MEM_H(0X52, ctx->r4) = 0;
    // 0x80040804: sw          $a1, 0x68($a0)
    MEM_W(0X68, ctx->r4) = ctx->r5;
    // 0x80040808: swc1        $f0, 0x10($a0)
    MEM_W(0X10, ctx->r4) = ctx->f0.u32l;
    // 0x8004080C: swc1        $f0, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->f0.u32l;
    // 0x80040810: swc1        $f0, 0x18($a0)
    MEM_W(0X18, ctx->r4) = ctx->f0.u32l;
    // 0x80040814: swc1        $f0, 0x20($a0)
    MEM_W(0X20, ctx->r4) = ctx->f0.u32l;
    // 0x80040818: swc1        $f0, 0x24($a0)
    MEM_W(0X24, ctx->r4) = ctx->f0.u32l;
    // 0x8004081C: swc1        $f0, 0x28($a0)
    MEM_W(0X28, ctx->r4) = ctx->f0.u32l;
    // 0x80040820: swc1        $f0, 0x2C($a0)
    MEM_W(0X2C, ctx->r4) = ctx->f0.u32l;
    // 0x80040824: swc1        $f0, 0x30($a0)
    MEM_W(0X30, ctx->r4) = ctx->f0.u32l;
    // 0x80040828: swc1        $f0, 0x34($a0)
    MEM_W(0X34, ctx->r4) = ctx->f0.u32l;
    // 0x8004082C: swc1        $f0, 0x54($a0)
    MEM_W(0X54, ctx->r4) = ctx->f0.u32l;
    // 0x80040830: swc1        $f0, 0x58($a0)
    MEM_W(0X58, ctx->r4) = ctx->f0.u32l;
    // 0x80040834: swc1        $f0, 0x5C($a0)
    MEM_W(0X5C, ctx->r4) = ctx->f0.u32l;
    // 0x80040838: swc1        $f0, 0x60($a0)
    MEM_W(0X60, ctx->r4) = ctx->f0.u32l;
    // 0x8004083C: jr          $ra
    // 0x80040840: swc1        $f0, 0x64($a0)
    MEM_W(0X64, ctx->r4) = ctx->f0.u32l;
    return;
    // 0x80040840: swc1        $f0, 0x64($a0)
    MEM_W(0X64, ctx->r4) = ctx->f0.u32l;
;}
RECOMP_FUNC void poolObject_attachChild2(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040844: addiu       $sp, $sp, -0x8
    ctx->r29 = ADD32(ctx->r29, -0X8);
    // 0x80040848: sw          $a2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r6;
    // 0x8004084C: andi        $a2, $a2, 0xFFFF
    ctx->r6 = ctx->r6 & 0XFFFF;
    // 0x80040850: sw          $s0, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r16;
    // 0x80040854: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80040858: andi        $v1, $a2, 0xFFFF
    ctx->r3 = ctx->r6 & 0XFFFF;
    // 0x8004085C: addiu       $v0, $a0, 0x60
    ctx->r2 = ADD32(ctx->r4, 0X60);
    // 0x80040860: addiu       $a3, $a0, 0x70
    ctx->r7 = ADD32(ctx->r4, 0X70);
    // 0x80040864: addiu       $t0, $a0, 0x80
    ctx->r8 = ADD32(ctx->r4, 0X80);
    // 0x80040868: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x8004086C: or          $t2, $zero, $zero
    ctx->r10 = 0 | 0;
    // 0x80040870: or          $t3, $zero, $zero
    ctx->r11 = 0 | 0;
L_80040874:
    // 0x80040874: and         $t6, $a2, $t1
    ctx->r14 = ctx->r6 & ctx->r9;
    // 0x80040878: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x8004087C: beq         $t6, $zero, L_800408D8
    if (ctx->r14 == 0) {
        // 0x80040880: or          $t4, $t1, $zero
        ctx->r12 = ctx->r9 | 0;
            goto L_800408D8;
    }
    // 0x80040880: or          $t4, $t1, $zero
    ctx->r12 = ctx->r9 | 0;
    // 0x80040884: lbu         $a2, 0x0($t0)
    ctx->r6 = MEM_BU(ctx->r8, 0X0);
    // 0x80040888: slti        $at, $a2, 0x20
    ctx->r1 = SIGNED(ctx->r6) < 0X20 ? 1 : 0;
    // 0x8004088C: beql        $at, $zero, L_800408D4
    if (ctx->r1 == 0) {
        // 0x80040890: or          $t2, $t2, $t4
        ctx->r10 = ctx->r10 | ctx->r12;
            goto L_800408D4;
    }
    goto skip_0;
    // 0x80040890: or          $t2, $t2, $t4
    ctx->r10 = ctx->r10 | ctx->r12;
    skip_0:
    // 0x80040894: beq         $a2, $zero, L_800408B0
    if (ctx->r6 == 0) {
        // 0x80040898: sll         $t1, $t3, 2
        ctx->r9 = S32(ctx->r11 << 2);
            goto L_800408B0;
    }
    // 0x80040898: sll         $t1, $t3, 2
    ctx->r9 = S32(ctx->r11 << 2);
    // 0x8004089C: lw          $a2, 0x0($a3)
    ctx->r6 = MEM_W(ctx->r7, 0X0);
    // 0x800408A0: sll         $t1, $t3, 2
    ctx->r9 = S32(ctx->r11 << 2);
    // 0x800408A4: addu        $t7, $a2, $t1
    ctx->r15 = ADD32(ctx->r6, ctx->r9);
    // 0x800408A8: b           L_800408B4
    // 0x800408AC: sw          $s0, 0x30($t7)
    MEM_W(0X30, ctx->r15) = ctx->r16;
        goto L_800408B4;
    // 0x800408AC: sw          $s0, 0x30($t7)
    MEM_W(0X30, ctx->r15) = ctx->r16;
L_800408B0:
    // 0x800408B0: sw          $s0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r16;
L_800408B4:
    // 0x800408B4: sw          $s0, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r16;
    // 0x800408B8: addu        $t8, $s0, $t1
    ctx->r24 = ADD32(ctx->r16, ctx->r9);
    // 0x800408BC: sw          $a0, 0x30($t8)
    MEM_W(0X30, ctx->r24) = ctx->r4;
    // 0x800408C0: lbu         $t9, 0x0($t0)
    ctx->r25 = MEM_BU(ctx->r8, 0X0);
    // 0x800408C4: addiu       $t5, $t9, 0x1
    ctx->r13 = ADD32(ctx->r25, 0X1);
    // 0x800408C8: b           L_800408D8
    // 0x800408CC: sb          $t5, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r13;
        goto L_800408D8;
    // 0x800408CC: sb          $t5, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r13;
    // 0x800408D0: or          $t2, $t2, $t4
    ctx->r10 = ctx->r10 | ctx->r12;
L_800408D4:
    // 0x800408D4: andi        $t2, $t2, 0xFFFF
    ctx->r10 = ctx->r10 & 0XFFFF;
L_800408D8:
    // 0x800408D8: nor         $t6, $t4, $zero
    ctx->r14 = ~(ctx->r12 | 0);
    // 0x800408DC: and         $a2, $a1, $t6
    ctx->r6 = ctx->r5 & ctx->r14;
    // 0x800408E0: andi        $a2, $a2, 0xFFFF
    ctx->r6 = ctx->r6 & 0XFFFF;
    // 0x800408E4: sll         $t1, $t4, 1
    ctx->r9 = S32(ctx->r12 << 1);
    // 0x800408E8: addiu       $t3, $t3, 0x1
    ctx->r11 = ADD32(ctx->r11, 0X1);
    // 0x800408EC: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x800408F0: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x800408F4: addiu       $t0, $t0, 0x1
    ctx->r8 = ADD32(ctx->r8, 0X1);
    // 0x800408F8: andi        $t1, $t1, 0xFFFF
    ctx->r9 = ctx->r9 & 0XFFFF;
    // 0x800408FC: bne         $a2, $zero, L_80040874
    if (ctx->r6 != 0) {
        // 0x80040900: andi        $t3, $t3, 0xFF
        ctx->r11 = ctx->r11 & 0XFF;
            goto L_80040874;
    }
    // 0x80040900: andi        $t3, $t3, 0xFF
    ctx->r11 = ctx->r11 & 0XFF;
    // 0x80040904: lh          $t8, 0x0($a0)
    ctx->r24 = MEM_H(ctx->r4, 0X0);
    // 0x80040908: nor         $t7, $t2, $zero
    ctx->r15 = ~(ctx->r10 | 0);
    // 0x8004090C: and         $v1, $v1, $t7
    ctx->r3 = ctx->r3 & ctx->r15;
    // 0x80040910: sll         $v1, $v1, 8
    ctx->r3 = S32(ctx->r3 << 8);
    // 0x80040914: or          $t9, $t8, $v1
    ctx->r25 = ctx->r24 | ctx->r3;
    // 0x80040918: sh          $t9, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r25;
    // 0x8004091C: lh          $t5, 0x0($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X0);
    // 0x80040920: or          $v0, $t2, $zero
    ctx->r2 = ctx->r10 | 0;
    // 0x80040924: or          $t6, $t5, $v1
    ctx->r14 = ctx->r13 | ctx->r3;
    // 0x80040928: sh          $t6, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r14;
    // 0x8004092C: lw          $s0, 0x4($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X4);
    // 0x80040930: jr          $ra
    // 0x80040934: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
    return;
    // 0x80040934: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
;}
RECOMP_FUNC void func_80040938(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040938: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x8004093C: ori         $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 | 0X8000;
    // 0x80040940: jr          $ra
    // 0x80040944: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    return;
    // 0x80040944: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
;}
RECOMP_FUNC void atari_work_entry_tick(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040948: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x8004094C: sh          $zero, 0x0($a0)
    MEM_H(0X0, ctx->r4) = 0;
    // 0x80040950: addiu       $v1, $a0, 0x60
    ctx->r3 = ADD32(ctx->r4, 0X60);
    // 0x80040954: andi        $t6, $v0, 0x1000
    ctx->r14 = ctx->r2 & 0X1000;
    // 0x80040958: bne         $t6, $zero, L_80040998
    if (ctx->r14 != 0) {
        // 0x8004095C: addiu       $v0, $zero, 0x4
        ctx->r2 = ADD32(0, 0X4);
            goto L_80040998;
    }
    // 0x8004095C: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x80040960: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
L_80040964:
    // 0x80040964: andi        $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 & 0XFF;
    // 0x80040968: sll         $a2, $v0, 2
    ctx->r6 = S32(ctx->r2 << 2);
    // 0x8004096C: addu        $t7, $v1, $a2
    ctx->r15 = ADD32(ctx->r3, ctx->r6);
    // 0x80040970: lw          $a1, 0x0($t7)
    ctx->r5 = MEM_W(ctx->r15, 0X0);
    // 0x80040974: beq         $a1, $zero, L_80040990
    if (ctx->r5 == 0) {
        // 0x80040978: nop
    
            goto L_80040990;
    }
    // 0x80040978: nop

    // 0x8004097C: sh          $zero, 0x0($a1)
    MEM_H(0X0, ctx->r5) = 0;
L_80040980:
    // 0x80040980: addu        $t8, $a1, $a2
    ctx->r24 = ADD32(ctx->r5, ctx->r6);
    // 0x80040984: lw          $a1, 0x30($t8)
    ctx->r5 = MEM_W(ctx->r24, 0X30);
    // 0x80040988: bnel        $a1, $a0, L_80040980
    if (ctx->r5 != ctx->r4) {
        // 0x8004098C: sh          $zero, 0x0($a1)
        MEM_H(0X0, ctx->r5) = 0;
            goto L_80040980;
    }
    goto skip_0;
    // 0x8004098C: sh          $zero, 0x0($a1)
    MEM_H(0X0, ctx->r5) = 0;
    skip_0:
L_80040990:
    // 0x80040990: bnel        $v0, $zero, L_80040964
    if (ctx->r2 != 0) {
        // 0x80040994: addiu       $v0, $v0, -0x1
        ctx->r2 = ADD32(ctx->r2, -0X1);
            goto L_80040964;
    }
    goto skip_1;
    // 0x80040994: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    skip_1:
L_80040998:
    // 0x80040998: jr          $ra
    // 0x8004099C: nop

    return;
    // 0x8004099C: nop

;}
RECOMP_FUNC void func_800409A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800409A0: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x800409A4: addiu       $a0, $a0, 0x42E8
    ctx->r4 = ADD32(ctx->r4, 0X42E8);
    // 0x800409A8: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x800409AC: addiu       $a2, $zero, -0x29
    ctx->r6 = ADD32(0, -0X29);
    // 0x800409B0: addiu       $a1, $zero, -0x5
    ctx->r5 = ADD32(0, -0X5);
    // 0x800409B4: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
L_800409B8:
    // 0x800409B8: addiu       $t1, $a0, 0x4E00
    ctx->r9 = ADD32(ctx->r4, 0X4E00);
    // 0x800409BC: andi        $t6, $v1, 0x2000
    ctx->r14 = ctx->r3 & 0X2000;
    // 0x800409C0: beql        $t6, $zero, L_800409DC
    if (ctx->r14 == 0) {
        // 0x800409C4: andi        $t9, $v1, 0x1000
        ctx->r25 = ctx->r3 & 0X1000;
            goto L_800409DC;
    }
    goto skip_0;
    // 0x800409C4: andi        $t9, $v1, 0x1000
    ctx->r25 = ctx->r3 & 0X1000;
    skip_0:
    // 0x800409C8: lhu         $t7, 0xA($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0XA);
    // 0x800409CC: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
    // 0x800409D0: and         $t8, $t7, $a1
    ctx->r24 = ctx->r15 & ctx->r5;
    // 0x800409D4: sh          $t8, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r24;
    // 0x800409D8: andi        $t9, $v1, 0x1000
    ctx->r25 = ctx->r3 & 0X1000;
L_800409DC:
    // 0x800409DC: beq         $t9, $zero, L_800409E8
    if (ctx->r25 == 0) {
        // 0x800409E0: and         $t0, $v1, $a2
        ctx->r8 = ctx->r3 & ctx->r6;
            goto L_800409E8;
    }
    // 0x800409E0: and         $t0, $v1, $a2
    ctx->r8 = ctx->r3 & ctx->r6;
    // 0x800409E4: sh          $t0, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r8;
L_800409E8:
    // 0x800409E8: addiu       $v0, $v0, 0x9C
    ctx->r2 = ADD32(ctx->r2, 0X9C);
    // 0x800409EC: bnel        $v0, $t1, L_800409B8
    if (ctx->r2 != ctx->r9) {
        // 0x800409F0: lh          $v1, 0x0($v0)
        ctx->r3 = MEM_H(ctx->r2, 0X0);
            goto L_800409B8;
    }
    goto skip_1;
    // 0x800409F0: lh          $v1, 0x0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0X0);
    skip_1:
    // 0x800409F4: jr          $ra
    // 0x800409F8: nop

    return;
    // 0x800409F8: nop

;}
RECOMP_FUNC void poolObject_tick(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800409FC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80040A00: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80040A04: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80040A08: lui         $s0, 0x8010
    ctx->r16 = S32(0X8010 << 16);
    // 0x80040A0C: lui         $s1, 0x800F
    ctx->r17 = S32(0X800F << 16);
    // 0x80040A10: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80040A14: addiu       $s0, $s0, -0x6F18
    ctx->r16 = ADD32(ctx->r16, -0X6F18);
    // 0x80040A18: addiu       $s1, $s1, 0x42E8
    ctx->r17 = ADD32(ctx->r17, 0X42E8);
    // 0x80040A1C: lh          $t6, -0x9C($s0)
    ctx->r14 = MEM_H(ctx->r16, -0X9C);
L_80040A20:
    // 0x80040A20: addiu       $s0, $s0, -0x9C
    ctx->r16 = ADD32(ctx->r16, -0X9C);
    // 0x80040A24: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x80040A28: beq         $t7, $zero, L_80040A38
    if (ctx->r15 == 0) {
        // 0x80040A2C: nop
    
            goto L_80040A38;
    }
    // 0x80040A2C: nop

    // 0x80040A30: jal         0x80040948
    // 0x80040A34: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    atari_work_entry_tick(rdram, ctx);
        goto after_0;
    // 0x80040A34: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
L_80040A38:
    // 0x80040A38: bnel        $s0, $s1, L_80040A20
    if (ctx->r16 != ctx->r17) {
        // 0x80040A3C: lh          $t6, -0x9C($s0)
        ctx->r14 = MEM_H(ctx->r16, -0X9C);
            goto L_80040A20;
    }
    goto skip_0;
    // 0x80040A3C: lh          $t6, -0x9C($s0)
    ctx->r14 = MEM_H(ctx->r16, -0X9C);
    skip_0:
    // 0x80040A40: jal         0x80040A6C
    // 0x80040A44: nop

    atari_work_post_update(rdram, ctx);
        goto after_1;
    // 0x80040A44: nop

    after_1:
    // 0x80040A48: jal         0x8004190C
    // 0x80040A4C: nop

    func_8004190C(rdram, ctx);
        goto after_2;
    // 0x80040A4C: nop

    after_2:
    // 0x80040A50: jal         0x80042DF8
    // 0x80040A54: nop

    func_80042DF8(rdram, ctx);
        goto after_3;
    // 0x80040A54: nop

    after_3:
    // 0x80040A58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80040A5C: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80040A60: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80040A64: jr          $ra
    // 0x80040A68: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x80040A68: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void atari_work_post_update(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040A6C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80040A70: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x80040A74: lui         $s1, 0x800F
    ctx->r17 = S32(0X800F << 16);
    // 0x80040A78: sw          $s3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r19;
    // 0x80040A7C: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80040A80: addiu       $s1, $s1, 0x42E8
    ctx->r17 = ADD32(ctx->r17, 0X42E8);
    // 0x80040A84: sw          $s2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r18;
    // 0x80040A88: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x80040A8C: lui         $s3, 0x800B
    ctx->r19 = S32(0X800B << 16);
    // 0x80040A90: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80040A94: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80040A98: or          $s0, $s1, $zero
    ctx->r16 = ctx->r17 | 0;
    // 0x80040A9C: addiu       $s3, $s3, 0x5170
    ctx->r19 = ADD32(ctx->r19, 0X5170);
    // 0x80040AA0: addiu       $s2, $zero, -0x41
    ctx->r18 = ADD32(0, -0X41);
    // 0x80040AA4: lh          $a1, 0x0($s0)
    ctx->r5 = MEM_H(ctx->r16, 0X0);
L_80040AA8:
    // 0x80040AA8: andi        $v0, $a1, 0x2000
    ctx->r2 = ctx->r5 & 0X2000;
    // 0x80040AAC: beq         $v0, $zero, L_80040B20
    if (ctx->r2 == 0) {
        // 0x80040AB0: andi        $t9, $a1, 0x1000
        ctx->r25 = ctx->r5 & 0X1000;
            goto L_80040B20;
    }
    // 0x80040AB0: andi        $t9, $a1, 0x1000
    ctx->r25 = ctx->r5 & 0X1000;
    // 0x80040AB4: andi        $t6, $a1, 0x1000
    ctx->r14 = ctx->r5 & 0X1000;
    // 0x80040AB8: beql        $t6, $zero, L_80040AF4
    if (ctx->r14 == 0) {
        // 0x80040ABC: lhu         $t8, 0x44($s0)
        ctx->r24 = MEM_HU(ctx->r16, 0X44);
            goto L_80040AF4;
    }
    goto skip_0;
    // 0x80040ABC: lhu         $t8, 0x44($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X44);
    skip_0:
    // 0x80040AC0: lhu         $t7, 0x40($s0)
    ctx->r15 = MEM_HU(ctx->r16, 0X40);
    // 0x80040AC4: sh          $zero, 0x40($s0)
    MEM_H(0X40, ctx->r16) = 0;
    // 0x80040AC8: sh          $zero, 0x44($s0)
    MEM_H(0X44, ctx->r16) = 0;
    // 0x80040ACC: sb          $zero, 0x4A($s0)
    MEM_B(0X4A, ctx->r16) = 0;
    // 0x80040AD0: sb          $zero, 0x4B($s0)
    MEM_B(0X4B, ctx->r16) = 0;
    // 0x80040AD4: sh          $zero, 0x46($s0)
    MEM_H(0X46, ctx->r16) = 0;
    // 0x80040AD8: sh          $zero, 0xE($s0)
    MEM_H(0XE, ctx->r16) = 0;
    // 0x80040ADC: swc1        $f20, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f20.u32l;
    // 0x80040AE0: swc1        $f20, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f20.u32l;
    // 0x80040AE4: swc1        $f20, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f20.u32l;
    // 0x80040AE8: b           L_80040B20
    // 0x80040AEC: sh          $t7, 0x42($s0)
    MEM_H(0X42, ctx->r16) = ctx->r15;
        goto L_80040B20;
    // 0x80040AEC: sh          $t7, 0x42($s0)
    MEM_H(0X42, ctx->r16) = ctx->r15;
    // 0x80040AF0: lhu         $t8, 0x44($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X44);
L_80040AF4:
    // 0x80040AF4: sh          $zero, 0x44($s0)
    MEM_H(0X44, ctx->r16) = 0;
    // 0x80040AF8: sh          $zero, 0x48($s0)
    MEM_H(0X48, ctx->r16) = 0;
    // 0x80040AFC: sw          $zero, 0x4C($s0)
    MEM_W(0X4C, ctx->r16) = 0;
    // 0x80040B00: sw          $zero, 0x50($s0)
    MEM_W(0X50, ctx->r16) = 0;
    // 0x80040B04: sw          $zero, 0x54($s0)
    MEM_W(0X54, ctx->r16) = 0;
    // 0x80040B08: sh          $zero, 0x4A($s0)
    MEM_H(0X4A, ctx->r16) = 0;
    // 0x80040B0C: sh          $zero, 0xE($s0)
    MEM_H(0XE, ctx->r16) = 0;
    // 0x80040B10: swc1        $f20, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f20.u32l;
    // 0x80040B14: swc1        $f20, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f20.u32l;
    // 0x80040B18: swc1        $f20, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f20.u32l;
    // 0x80040B1C: sh          $t8, 0x46($s0)
    MEM_H(0X46, ctx->r16) = ctx->r24;
L_80040B20:
    // 0x80040B20: beq         $t9, $zero, L_80040B70
    if (ctx->r25 == 0) {
        // 0x80040B24: and         $a1, $a1, $s2
        ctx->r5 = ctx->r5 & ctx->r18;
            goto L_80040B70;
    }
    // 0x80040B24: and         $a1, $a1, $s2
    ctx->r5 = ctx->r5 & ctx->r18;
    // 0x80040B28: bne         $v0, $zero, L_80040B3C
    if (ctx->r2 != 0) {
        // 0x80040B2C: sll         $a1, $a1, 16
        ctx->r5 = S32(ctx->r5 << 16);
            goto L_80040B3C;
    }
    // 0x80040B2C: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80040B30: sh          $zero, 0x90($s0)
    MEM_H(0X90, ctx->r16) = 0;
    // 0x80040B34: sh          $zero, 0x92($s0)
    MEM_H(0X92, ctx->r16) = 0;
    // 0x80040B38: sh          $zero, 0x94($s0)
    MEM_H(0X94, ctx->r16) = 0;
L_80040B3C:
    // 0x80040B3C: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80040B40: andi        $t0, $a1, 0x60
    ctx->r8 = ctx->r5 & 0X60;
    // 0x80040B44: bne         $t0, $zero, L_80040B70
    if (ctx->r8 != 0) {
        // 0x80040B48: andi        $t1, $a1, 0x7
        ctx->r9 = ctx->r5 & 0X7;
            goto L_80040B70;
    }
    // 0x80040B48: andi        $t1, $a1, 0x7
    ctx->r9 = ctx->r5 & 0X7;
    // 0x80040B4C: sll         $t2, $t1, 16
    ctx->r10 = S32(ctx->r9 << 16);
    // 0x80040B50: sra         $t3, $t2, 16
    ctx->r11 = S32(SIGNED(ctx->r10) >> 16);
    // 0x80040B54: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80040B58: addu        $t5, $s3, $t4
    ctx->r13 = ADD32(ctx->r19, ctx->r12);
    // 0x80040B5C: lw          $t9, 0x0($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X0);
    // 0x80040B60: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040B64: jalr        $t9
    // 0x80040B68: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80040B68: nop

    after_0:
    // 0x80040B6C: sh          $v0, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r2;
L_80040B70:
    // 0x80040B70: addiu       $s0, $s0, 0x9C
    ctx->r16 = ADD32(ctx->r16, 0X9C);
    // 0x80040B74: addiu       $t6, $s1, 0x4E00
    ctx->r14 = ADD32(ctx->r17, 0X4E00);
    // 0x80040B78: bnel        $s0, $t6, L_80040AA8
    if (ctx->r16 != ctx->r14) {
        // 0x80040B7C: lh          $a1, 0x0($s0)
        ctx->r5 = MEM_H(ctx->r16, 0X0);
            goto L_80040AA8;
    }
    goto skip_1;
    // 0x80040B7C: lh          $a1, 0x0($s0)
    ctx->r5 = MEM_H(ctx->r16, 0X0);
    skip_1:
    // 0x80040B80: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80040B84: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80040B88: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x80040B8C: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x80040B90: lw          $s2, 0x24($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X24);
    // 0x80040B94: lw          $s3, 0x28($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X28);
    // 0x80040B98: jr          $ra
    // 0x80040B9C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80040B9C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80040BA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040BA0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80040BA4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80040BA8: lh          $a1, 0x0($a0)
    ctx->r5 = MEM_H(ctx->r4, 0X0);
    // 0x80040BAC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80040BB0: andi        $t6, $a1, 0x1000
    ctx->r14 = ctx->r5 & 0X1000;
    // 0x80040BB4: beq         $t6, $zero, L_80040C20
    if (ctx->r14 == 0) {
        // 0x80040BB8: andi        $a1, $a1, 0xFF9F
        ctx->r5 = ctx->r5 & 0XFF9F;
            goto L_80040C20;
    }
    // 0x80040BB8: andi        $a1, $a1, 0xFF9F
    ctx->r5 = ctx->r5 & 0XFF9F;
    // 0x80040BBC: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80040BC0: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80040BC4: andi        $t7, $a1, 0x7
    ctx->r15 = ctx->r5 & 0X7;
    // 0x80040BC8: sll         $t8, $t7, 16
    ctx->r24 = S32(ctx->r15 << 16);
    // 0x80040BCC: sra         $t9, $t8, 16
    ctx->r25 = S32(SIGNED(ctx->r24) >> 16);
    // 0x80040BD0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80040BD4: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80040BD8: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80040BDC: lw          $t9, 0x5170($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X5170);
    // 0x80040BE0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80040BE4: jalr        $t9
    // 0x80040BE8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80040BE8: nop

    after_0:
    // 0x80040BEC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80040BF0: sll         $a1, $v0, 16
    ctx->r5 = S32(ctx->r2 << 16);
    // 0x80040BF4: andi        $t1, $v0, 0x40
    ctx->r9 = ctx->r2 & 0X40;
    // 0x80040BF8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80040BFC: beq         $t1, $zero, L_80040C10
    if (ctx->r9 == 0) {
        // 0x80040C00: sh          $v0, 0x0($a0)
        MEM_H(0X0, ctx->r4) = ctx->r2;
            goto L_80040C10;
    }
    // 0x80040C00: sh          $v0, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r2;
    // 0x80040C04: lh          $t2, 0x0($a0)
    ctx->r10 = MEM_H(ctx->r4, 0X0);
    // 0x80040C08: ori         $t3, $t2, 0x60
    ctx->r11 = ctx->r10 | 0X60;
    // 0x80040C0C: sh          $t3, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r11;
L_80040C10:
    // 0x80040C10: andi        $v0, $a1, 0x60
    ctx->r2 = ctx->r5 & 0X60;
    // 0x80040C14: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80040C18: b           L_80040C20
    // 0x80040C1C: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
        goto L_80040C20;
    // 0x80040C1C: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
L_80040C20:
    // 0x80040C20: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80040C24: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80040C28: jr          $ra
    // 0x80040C2C: nop

    return;
    // 0x80040C2C: nop

;}
RECOMP_FUNC void func_80040C30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040C30: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80040C34: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x80040C38: bne         $a1, $zero, L_80040C48
    if (ctx->r5 != 0) {
        // 0x80040C3C: sra         $a2, $a2, 16
        ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
            goto L_80040C48;
    }
    // 0x80040C3C: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80040C40: jr          $ra
    // 0x80040C44: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80040C44: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040C48:
    // 0x80040C48: lh          $t7, 0x0($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X0);
    // 0x80040C4C: addiu       $a3, $zero, -0x8000
    ctx->r7 = ADD32(0, -0X8000);
    // 0x80040C50: andi        $t8, $a2, 0x2000
    ctx->r24 = ctx->r6 & 0X2000;
    // 0x80040C54: bgez        $t7, L_80040C64
    if (SIGNED(ctx->r15) >= 0) {
        // 0x80040C58: nop
    
            goto L_80040C64;
    }
    // 0x80040C58: nop

    // 0x80040C5C: jr          $ra
    // 0x80040C60: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80040C60: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040C64:
    // 0x80040C64: bnel        $t8, $zero, L_80040CC4
    if (ctx->r24 != 0) {
        // 0x80040C68: andi        $t5, $a2, 0xF00
        ctx->r13 = ctx->r6 & 0XF00;
            goto L_80040CC4;
    }
    goto skip_0;
    // 0x80040C68: andi        $t5, $a2, 0xF00
    ctx->r13 = ctx->r6 & 0XF00;
    skip_0:
    // 0x80040C6C: lw          $t9, 0x30($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X30);
    // 0x80040C70: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80040C74: bnel        $t9, $zero, L_80040C9C
    if (ctx->r25 != 0) {
        // 0x80040C78: sll         $v1, $v0, 2
        ctx->r3 = S32(ctx->r2 << 2);
            goto L_80040C9C;
    }
    goto skip_1;
    // 0x80040C78: sll         $v1, $v0, 2
    ctx->r3 = S32(ctx->r2 << 2);
    skip_1:
    // 0x80040C7C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_80040C80:
    // 0x80040C80: andi        $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 & 0XFF;
    // 0x80040C84: sll         $t0, $v0, 2
    ctx->r8 = S32(ctx->r2 << 2);
    // 0x80040C88: addu        $t1, $a0, $t0
    ctx->r9 = ADD32(ctx->r4, ctx->r8);
    // 0x80040C8C: lw          $t2, 0x30($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X30);
    // 0x80040C90: beql        $t2, $zero, L_80040C80
    if (ctx->r10 == 0) {
        // 0x80040C94: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80040C80;
    }
    goto skip_2;
    // 0x80040C94: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_2:
    // 0x80040C98: sll         $v1, $v0, 2
    ctx->r3 = S32(ctx->r2 << 2);
L_80040C9C:
    // 0x80040C9C: addu        $t3, $a0, $v1
    ctx->r11 = ADD32(ctx->r4, ctx->r3);
L_80040CA0:
    // 0x80040CA0: lw          $a0, 0x30($t3)
    ctx->r4 = MEM_W(ctx->r11, 0X30);
    // 0x80040CA4: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x80040CA8: andi        $t4, $v0, 0x2000
    ctx->r12 = ctx->r2 & 0X2000;
    // 0x80040CAC: beql        $t4, $zero, L_80040CA0
    if (ctx->r12 == 0) {
        // 0x80040CB0: addu        $t3, $a0, $v1
        ctx->r11 = ADD32(ctx->r4, ctx->r3);
            goto L_80040CA0;
    }
    goto skip_3;
    // 0x80040CB0: addu        $t3, $a0, $v1
    ctx->r11 = ADD32(ctx->r4, ctx->r3);
    skip_3:
    // 0x80040CB4: and         $a2, $a2, $v0
    ctx->r6 = ctx->r6 & ctx->r2;
    // 0x80040CB8: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x80040CBC: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80040CC0: andi        $t5, $a2, 0xF00
    ctx->r13 = ctx->r6 & 0XF00;
L_80040CC4:
    // 0x80040CC4: bne         $t5, $zero, L_80040CD4
    if (ctx->r13 != 0) {
        // 0x80040CC8: andi        $t6, $a2, 0x2000
        ctx->r14 = ctx->r6 & 0X2000;
            goto L_80040CD4;
    }
    // 0x80040CC8: andi        $t6, $a2, 0x2000
    ctx->r14 = ctx->r6 & 0X2000;
    // 0x80040CCC: jr          $ra
    // 0x80040CD0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80040CD0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040CD4:
    // 0x80040CD4: bnel        $t6, $zero, L_80040D10
    if (ctx->r14 != 0) {
        // 0x80040CD8: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80040D10;
    }
    goto skip_4;
    // 0x80040CD8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_4:
    // 0x80040CDC: lw          $v0, 0x58($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X58);
    // 0x80040CE0: beql        $a1, $v0, L_80040D10
    if (ctx->r5 == ctx->r2) {
        // 0x80040CE4: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_80040D10;
    }
    goto skip_5;
    // 0x80040CE4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_5:
    // 0x80040CE8: lw          $a1, 0x8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X8);
L_80040CEC:
    // 0x80040CEC: lh          $t7, 0x0($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X0);
    // 0x80040CF0: and         $t8, $t7, $a3
    ctx->r24 = ctx->r15 & ctx->r7;
    // 0x80040CF4: beq         $t8, $zero, L_80040D04
    if (ctx->r24 == 0) {
        // 0x80040CF8: nop
    
            goto L_80040D04;
    }
    // 0x80040CF8: nop

    // 0x80040CFC: jr          $ra
    // 0x80040D00: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80040D00: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80040D04:
    // 0x80040D04: bnel        $a1, $v0, L_80040CEC
    if (ctx->r5 != ctx->r2) {
        // 0x80040D08: lw          $a1, 0x8($a1)
        ctx->r5 = MEM_W(ctx->r5, 0X8);
            goto L_80040CEC;
    }
    goto skip_6;
    // 0x80040D08: lw          $a1, 0x8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X8);
    skip_6:
    // 0x80040D0C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80040D10:
    // 0x80040D10: jr          $ra
    // 0x80040D14: nop

    return;
    // 0x80040D14: nop

;}
RECOMP_FUNC void func_80040D18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80040D18: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x80040D1C: sll         $a2, $a1, 16
    ctx->r6 = S32(ctx->r5 << 16);
    // 0x80040D20: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80040D24: andi        $t6, $a2, 0x2000
    ctx->r14 = ctx->r6 & 0X2000;
    // 0x80040D28: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80040D2C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x80040D30: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80040D34: beq         $t6, $zero, L_80040D44
    if (ctx->r14 == 0) {
        // 0x80040D38: sw          $a1, 0x5C($sp)
        MEM_W(0X5C, ctx->r29) = ctx->r5;
            goto L_80040D44;
    }
    // 0x80040D38: sw          $a1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r5;
    // 0x80040D3C: b           L_80040D48
    // 0x80040D40: lw          $a3, 0x68($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X68);
        goto L_80040D48;
    // 0x80040D40: lw          $a3, 0x68($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X68);
L_80040D44:
    // 0x80040D44: lw          $a3, 0x2C($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X2C);
L_80040D48:
    // 0x80040D48: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x80040D4C: sw          $a0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r4;
    // 0x80040D50: sh          $a2, 0x5E($sp)
    MEM_H(0X5E, ctx->r29) = ctx->r6;
    // 0x80040D54: jal         0x80040C30
    // 0x80040D58: sw          $a3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r7;
    func_80040C30(rdram, ctx);
        goto after_0;
    // 0x80040D58: sw          $a3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r7;
    after_0:
    // 0x80040D5C: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x80040D60: beq         $v0, $zero, L_8004100C
    if (ctx->r2 == 0) {
        // 0x80040D64: lw          $a3, 0x54($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X54);
            goto L_8004100C;
    }
    // 0x80040D64: lw          $a3, 0x54($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X54);
    // 0x80040D68: lh          $t7, 0x5E($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X5E);
    // 0x80040D6C: ori         $t1, $t7, 0x60
    ctx->r9 = ctx->r15 | 0X60;
    // 0x80040D70: andi        $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 & 0X2000;
    // 0x80040D74: beq         $t2, $zero, L_80040D9C
    if (ctx->r10 == 0) {
        // 0x80040D78: sh          $t1, 0x5E($sp)
        MEM_H(0X5E, ctx->r29) = ctx->r9;
            goto L_80040D9C;
    }
    // 0x80040D78: sh          $t1, 0x5E($sp)
    MEM_H(0X5E, ctx->r29) = ctx->r9;
    // 0x80040D7C: addiu       $t3, $a0, 0x60
    ctx->r11 = ADD32(ctx->r4, 0X60);
    // 0x80040D80: addiu       $t4, $a0, 0x94
    ctx->r12 = ADD32(ctx->r4, 0X94);
    // 0x80040D84: addiu       $s0, $a0, 0x54
    ctx->r16 = ADD32(ctx->r4, 0X54);
    // 0x80040D88: sw          $t3, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r11;
    // 0x80040D8C: addiu       $v1, $a0, 0x70
    ctx->r3 = ADD32(ctx->r4, 0X70);
    // 0x80040D90: addiu       $t0, $a0, 0x7C
    ctx->r8 = ADD32(ctx->r4, 0X7C);
    // 0x80040D94: b           L_80040DB8
    // 0x80040D98: sw          $t4, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r12;
        goto L_80040DB8;
    // 0x80040D98: sw          $t4, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r12;
L_80040D9C:
    // 0x80040D9C: addiu       $t5, $a0, 0x24
    ctx->r13 = ADD32(ctx->r4, 0X24);
    // 0x80040DA0: addiu       $t6, $a0, 0x64
    ctx->r14 = ADD32(ctx->r4, 0X64);
    // 0x80040DA4: addiu       $s0, $a0, 0x18
    ctx->r16 = ADD32(ctx->r4, 0X18);
    // 0x80040DA8: sw          $t5, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r13;
    // 0x80040DAC: addiu       $v1, $a0, 0x40
    ctx->r3 = ADD32(ctx->r4, 0X40);
    // 0x80040DB0: addiu       $t0, $a0, 0x4C
    ctx->r8 = ADD32(ctx->r4, 0X4C);
    // 0x80040DB4: sw          $t6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r14;
L_80040DB8:
    // 0x80040DB8: lhu         $v0, 0x2($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X2);
    // 0x80040DBC: andi        $t7, $v0, 0x8000
    ctx->r15 = ctx->r2 & 0X8000;
    // 0x80040DC0: beq         $t7, $zero, L_80040DD8
    if (ctx->r15 == 0) {
        // 0x80040DC4: andi        $t8, $v0, 0x2000
        ctx->r24 = ctx->r2 & 0X2000;
            goto L_80040DD8;
    }
    // 0x80040DC4: andi        $t8, $v0, 0x2000
    ctx->r24 = ctx->r2 & 0X2000;
    // 0x80040DC8: beq         $t8, $zero, L_80040DD8
    if (ctx->r24 == 0) {
        // 0x80040DCC: addiu       $t9, $a3, 0x78
        ctx->r25 = ADD32(ctx->r7, 0X78);
            goto L_80040DD8;
    }
    // 0x80040DCC: addiu       $t9, $a3, 0x78
    ctx->r25 = ADD32(ctx->r7, 0X78);
    // 0x80040DD0: b           L_80040E0C
    // 0x80040DD4: sw          $t9, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r25;
        goto L_80040E0C;
    // 0x80040DD4: sw          $t9, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r25;
L_80040DD8:
    // 0x80040DD8: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x80040DDC: lh          $t1, -0x7CA0($t1)
    ctx->r9 = MEM_H(ctx->r9, -0X7CA0);
    // 0x80040DE0: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80040DE4: addiu       $a0, $a0, 0x4270
    ctx->r4 = ADD32(ctx->r4, 0X4270);
    // 0x80040DE8: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x80040DEC: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x80040DF0: addu        $t3, $a3, $t2
    ctx->r11 = ADD32(ctx->r7, ctx->r10);
    // 0x80040DF4: lw          $a1, 0x44($t3)
    ctx->r5 = MEM_W(ctx->r11, 0X44);
    // 0x80040DF8: sw          $t0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r8;
    // 0x80040DFC: jal         0x8009A4E8
    // 0x80040E00: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    func_8009A4E8(rdram, ctx);
        goto after_1;
    // 0x80040E00: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    after_1:
    // 0x80040E04: lw          $v1, 0x3C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X3C);
    // 0x80040E08: lw          $t0, 0x38($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X38);
L_80040E0C:
    // 0x80040E0C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80040E10: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x80040E14: lw          $a2, 0x4($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X4);
    // 0x80040E18: lw          $a3, 0x8($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X8);
    // 0x80040E1C: sw          $t0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r8;
    // 0x80040E20: sw          $v1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r3;
    // 0x80040E24: jal         0x800A17E0
    // 0x80040E28: addiu       $a0, $a0, 0x4230
    ctx->r4 = ADD32(ctx->r4, 0X4230);
    guTranslateF(rdram, ctx);
        goto after_2;
    // 0x80040E28: addiu       $a0, $a0, 0x4230
    ctx->r4 = ADD32(ctx->r4, 0X4230);
    after_2:
    // 0x80040E2C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80040E30: addiu       $a0, $a0, 0x4230
    ctx->r4 = ADD32(ctx->r4, 0X4230);
    // 0x80040E34: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80040E38: jal         0x8000CC18
    // 0x80040E3C: lw          $a1, 0x48($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X48);
    func_8000CC18(rdram, ctx);
        goto after_3;
    // 0x80040E3C: lw          $a1, 0x48($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X48);
    after_3:
    // 0x80040E40: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040E44: lw          $v1, 0x3C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X3C);
    // 0x80040E48: lw          $t0, 0x38($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X38);
    // 0x80040E4C: lwc1        $f0, 0x4260($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4260);
    // 0x80040E50: lwc1        $f2, 0x4264($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4264);
    // 0x80040E54: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040E58: lwc1        $f12, 0x4268($at)
    ctx->f12.u32l = MEM_W(ctx->r1, 0X4268);
    // 0x80040E5C: swc1        $f0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f0.u32l;
    // 0x80040E60: swc1        $f2, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f2.u32l;
    // 0x80040E64: swc1        $f12, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->f12.u32l;
    // 0x80040E68: swc1        $f0, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->f0.u32l;
    // 0x80040E6C: swc1        $f2, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->f2.u32l;
    // 0x80040E70: swc1        $f12, 0x8($t0)
    MEM_W(0X8, ctx->r8) = ctx->f12.u32l;
    // 0x80040E74: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x80040E78: lw          $t4, 0x40($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X40);
    // 0x80040E7C: lui         $s0, 0x800F
    ctx->r16 = S32(0X800F << 16);
    // 0x80040E80: addiu       $s0, $s0, 0x41F0
    ctx->r16 = ADD32(ctx->r16, 0X41F0);
    // 0x80040E84: lw          $a1, 0x0($t4)
    ctx->r5 = MEM_W(ctx->r12, 0X0);
    // 0x80040E88: mfc1        $a2, $f14
    ctx->r6 = (int32_t)ctx->f14.u32l;
    // 0x80040E8C: mfc1        $a3, $f14
    ctx->r7 = (int32_t)ctx->f14.u32l;
    // 0x80040E90: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040E94: swc1        $f0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f0.u32l;
    // 0x80040E98: swc1        $f2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f2.u32l;
    // 0x80040E9C: jal         0x800A17E0
    // 0x80040EA0: swc1        $f12, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f12.u32l;
    guTranslateF(rdram, ctx);
        goto after_4;
    // 0x80040EA0: swc1        $f12, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f12.u32l;
    after_4:
    // 0x80040EA4: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80040EA8: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x80040EAC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040EB0: jal         0x8000CC18
    // 0x80040EB4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_8000CC18(rdram, ctx);
        goto after_5;
    // 0x80040EB4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_5:
    // 0x80040EB8: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040EBC: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x80040EC0: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80040EC4: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x80040EC8: lwc1        $f6, 0x2C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80040ECC: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x80040ED0: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040ED4: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x80040ED8: sub.s       $f2, $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f6.fl;
    // 0x80040EDC: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80040EE0: lwc1        $f8, 0x28($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80040EE4: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80040EE8: sub.s       $f14, $f14, $f8
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f8.fl;
    // 0x80040EEC: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80040EF0: add.s       $f10, $f0, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x80040EF4: jal         0x800A01E0
    // 0x80040EF8: add.s       $f12, $f10, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_6;
    // 0x80040EF8: add.s       $f12, $f10, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f14.fl;
    after_6:
    // 0x80040EFC: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80040F00: lw          $t5, 0x40($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X40);
    // 0x80040F04: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
    // 0x80040F08: mfc1        $a1, $f2
    ctx->r5 = (int32_t)ctx->f2.u32l;
    // 0x80040F0C: mfc1        $a3, $f2
    ctx->r7 = (int32_t)ctx->f2.u32l;
    // 0x80040F10: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040F14: jal         0x800A17E0
    // 0x80040F18: lw          $a2, 0x0($t5)
    ctx->r6 = MEM_W(ctx->r13, 0X0);
    guTranslateF(rdram, ctx);
        goto after_7;
    // 0x80040F18: lw          $a2, 0x0($t5)
    ctx->r6 = MEM_W(ctx->r13, 0X0);
    after_7:
    // 0x80040F1C: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80040F20: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x80040F24: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040F28: jal         0x8000CC18
    // 0x80040F2C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_8000CC18(rdram, ctx);
        goto after_8;
    // 0x80040F2C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_8:
    // 0x80040F30: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040F34: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x80040F38: lwc1        $f16, 0x30($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80040F3C: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x80040F40: lwc1        $f18, 0x2C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80040F44: sub.s       $f0, $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f16.fl;
    // 0x80040F48: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040F4C: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x80040F50: sub.s       $f2, $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f18.fl;
    // 0x80040F54: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80040F58: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80040F5C: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80040F60: sub.s       $f14, $f14, $f4
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f4.fl;
    // 0x80040F64: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80040F68: add.s       $f6, $f0, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x80040F6C: jal         0x800A01E0
    // 0x80040F70: add.s       $f12, $f6, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_9;
    // 0x80040F70: add.s       $f12, $f6, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f14.fl;
    after_9:
    // 0x80040F74: lw          $t6, 0x40($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X40);
    // 0x80040F78: add.s       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f0.fl;
    // 0x80040F7C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040F80: addiu       $a1, $zero, 0x0
    ctx->r5 = ADD32(0, 0X0);
    // 0x80040F84: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x80040F88: jal         0x800A17E0
    // 0x80040F8C: lw          $a3, 0x0($t6)
    ctx->r7 = MEM_W(ctx->r14, 0X0);
    guTranslateF(rdram, ctx);
        goto after_10;
    // 0x80040F8C: lw          $a3, 0x0($t6)
    ctx->r7 = MEM_W(ctx->r14, 0X0);
    after_10:
    // 0x80040F90: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80040F94: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x80040F98: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80040F9C: jal         0x8000CC18
    // 0x80040FA0: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_8000CC18(rdram, ctx);
        goto after_11;
    // 0x80040FA0: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_11:
    // 0x80040FA4: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040FA8: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x80040FAC: lwc1        $f8, 0x30($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80040FB0: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x80040FB4: lwc1        $f10, 0x2C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80040FB8: sub.s       $f0, $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f8.fl;
    // 0x80040FBC: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80040FC0: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x80040FC4: sub.s       $f2, $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f10.fl;
    // 0x80040FC8: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80040FCC: lwc1        $f16, 0x28($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80040FD0: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80040FD4: sub.s       $f14, $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f16.fl;
    // 0x80040FD8: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80040FDC: add.s       $f18, $f0, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x80040FE0: jal         0x800A01E0
    // 0x80040FE4: add.s       $f12, $f18, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_12;
    // 0x80040FE4: add.s       $f12, $f18, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f14.fl;
    after_12:
    // 0x80040FE8: add.s       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f0.fl;
    // 0x80040FEC: lui         $at, 0x4008
    ctx->r1 = S32(0X4008 << 16);
    // 0x80040FF0: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x80040FF4: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80040FF8: cvt.d.s     $f4, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f4.d = CVT_D_S(ctx->f20.fl);
    // 0x80040FFC: lw          $t7, 0x34($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X34);
    // 0x80041000: div.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = DIV_D(ctx->f4.d, ctx->f6.d);
    // 0x80041004: cvt.s.d     $f20, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f20.fl = CVT_S_D(ctx->f8.d);
    // 0x80041008: swc1        $f20, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->f20.u32l;
L_8004100C:
    // 0x8004100C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80041010: lh          $v0, 0x5E($sp)
    ctx->r2 = MEM_H(ctx->r29, 0X5E);
    // 0x80041014: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80041018: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8004101C: jr          $ra
    // 0x80041020: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x80041020: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_80041024(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80041024: addiu       $sp, $sp, -0x98
    ctx->r29 = ADD32(ctx->r29, -0X98);
    // 0x80041028: sll         $a2, $a1, 16
    ctx->r6 = S32(ctx->r5 << 16);
    // 0x8004102C: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80041030: andi        $t6, $a2, 0x2000
    ctx->r14 = ctx->r6 & 0X2000;
    // 0x80041034: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80041038: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x8004103C: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80041040: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80041044: beq         $t6, $zero, L_80041054
    if (ctx->r14 == 0) {
        // 0x80041048: sw          $a1, 0x9C($sp)
        MEM_W(0X9C, ctx->r29) = ctx->r5;
            goto L_80041054;
    }
    // 0x80041048: sw          $a1, 0x9C($sp)
    MEM_W(0X9C, ctx->r29) = ctx->r5;
    // 0x8004104C: b           L_80041058
    // 0x80041050: lw          $a3, 0x68($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X68);
        goto L_80041058;
    // 0x80041050: lw          $a3, 0x68($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X68);
L_80041054:
    // 0x80041054: lw          $a3, 0x2C($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X2C);
L_80041058:
    // 0x80041058: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x8004105C: sw          $a0, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->r4;
    // 0x80041060: sh          $a2, 0x9E($sp)
    MEM_H(0X9E, ctx->r29) = ctx->r6;
    // 0x80041064: jal         0x80040C30
    // 0x80041068: sw          $a3, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->r7;
    func_80040C30(rdram, ctx);
        goto after_0;
    // 0x80041068: sw          $a3, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->r7;
    after_0:
    // 0x8004106C: lw          $a0, 0x98($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X98);
    // 0x80041070: beq         $v0, $zero, L_800414A4
    if (ctx->r2 == 0) {
        // 0x80041074: lw          $a3, 0x94($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X94);
            goto L_800414A4;
    }
    // 0x80041074: lw          $a3, 0x94($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X94);
    // 0x80041078: lh          $t7, 0x9E($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X9E);
    // 0x8004107C: ori         $t0, $t7, 0x60
    ctx->r8 = ctx->r15 | 0X60;
    // 0x80041080: andi        $t1, $t0, 0x2000
    ctx->r9 = ctx->r8 & 0X2000;
    // 0x80041084: beq         $t1, $zero, L_800410C4
    if (ctx->r9 == 0) {
        // 0x80041088: sh          $t0, 0x9E($sp)
        MEM_H(0X9E, ctx->r29) = ctx->r8;
            goto L_800410C4;
    }
    // 0x80041088: sh          $t0, 0x9E($sp)
    MEM_H(0X9E, ctx->r29) = ctx->r8;
    // 0x8004108C: addiu       $t2, $a0, 0x4E
    ctx->r10 = ADD32(ctx->r4, 0X4E);
    // 0x80041090: addiu       $t3, $a0, 0x60
    ctx->r11 = ADD32(ctx->r4, 0X60);
    // 0x80041094: addiu       $t4, $a0, 0x70
    ctx->r12 = ADD32(ctx->r4, 0X70);
    // 0x80041098: addiu       $t5, $a0, 0x7C
    ctx->r13 = ADD32(ctx->r4, 0X7C);
    // 0x8004109C: addiu       $t6, $a0, 0x88
    ctx->r14 = ADD32(ctx->r4, 0X88);
    // 0x800410A0: addiu       $t7, $a0, 0x94
    ctx->r15 = ADD32(ctx->r4, 0X94);
    // 0x800410A4: sw          $t2, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r10;
    // 0x800410A8: addiu       $s0, $a0, 0x54
    ctx->r16 = ADD32(ctx->r4, 0X54);
    // 0x800410AC: sw          $t3, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r11;
    // 0x800410B0: sw          $t4, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r12;
    // 0x800410B4: sw          $t5, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r13;
    // 0x800410B8: sw          $t6, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r14;
    // 0x800410BC: b           L_800410F8
    // 0x800410C0: sw          $t7, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r15;
        goto L_800410F8;
    // 0x800410C0: sw          $t7, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r15;
L_800410C4:
    // 0x800410C4: addiu       $t8, $a0, 0x12
    ctx->r24 = ADD32(ctx->r4, 0X12);
    // 0x800410C8: addiu       $t9, $a0, 0x24
    ctx->r25 = ADD32(ctx->r4, 0X24);
    // 0x800410CC: addiu       $t0, $a0, 0x40
    ctx->r8 = ADD32(ctx->r4, 0X40);
    // 0x800410D0: addiu       $t1, $a0, 0x4C
    ctx->r9 = ADD32(ctx->r4, 0X4C);
    // 0x800410D4: addiu       $t2, $a0, 0x58
    ctx->r10 = ADD32(ctx->r4, 0X58);
    // 0x800410D8: addiu       $t3, $a0, 0x64
    ctx->r11 = ADD32(ctx->r4, 0X64);
    // 0x800410DC: sw          $t8, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r24;
    // 0x800410E0: addiu       $s0, $a0, 0x18
    ctx->r16 = ADD32(ctx->r4, 0X18);
    // 0x800410E4: sw          $t9, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r25;
    // 0x800410E8: sw          $t0, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r8;
    // 0x800410EC: sw          $t1, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r9;
    // 0x800410F0: sw          $t2, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r10;
    // 0x800410F4: sw          $t3, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r11;
L_800410F8:
    // 0x800410F8: lhu         $v0, 0x2($a3)
    ctx->r2 = MEM_HU(ctx->r7, 0X2);
    // 0x800410FC: andi        $t4, $v0, 0x8000
    ctx->r12 = ctx->r2 & 0X8000;
    // 0x80041100: beq         $t4, $zero, L_80041118
    if (ctx->r12 == 0) {
        // 0x80041104: andi        $t5, $v0, 0x2000
        ctx->r13 = ctx->r2 & 0X2000;
            goto L_80041118;
    }
    // 0x80041104: andi        $t5, $v0, 0x2000
    ctx->r13 = ctx->r2 & 0X2000;
    // 0x80041108: beq         $t5, $zero, L_80041118
    if (ctx->r13 == 0) {
        // 0x8004110C: addiu       $t6, $a3, 0x78
        ctx->r14 = ADD32(ctx->r7, 0X78);
            goto L_80041118;
    }
    // 0x8004110C: addiu       $t6, $a3, 0x78
    ctx->r14 = ADD32(ctx->r7, 0X78);
    // 0x80041110: b           L_8004113C
    // 0x80041114: sw          $t6, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r14;
        goto L_8004113C;
    // 0x80041114: sw          $t6, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r14;
L_80041118:
    // 0x80041118: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x8004111C: lh          $t7, -0x7CA0($t7)
    ctx->r15 = MEM_H(ctx->r15, -0X7CA0);
    // 0x80041120: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041124: addiu       $a0, $a0, 0x4270
    ctx->r4 = ADD32(ctx->r4, 0X4270);
    // 0x80041128: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8004112C: sw          $a0, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r4;
    // 0x80041130: addu        $t9, $a3, $t8
    ctx->r25 = ADD32(ctx->r7, ctx->r24);
    // 0x80041134: jal         0x8009A4E8
    // 0x80041138: lw          $a1, 0x44($t9)
    ctx->r5 = MEM_W(ctx->r25, 0X44);
    func_8009A4E8(rdram, ctx);
        goto after_1;
    // 0x80041138: lw          $a1, 0x44($t9)
    ctx->r5 = MEM_W(ctx->r25, 0X44);
    after_1:
L_8004113C:
    // 0x8004113C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041140: addiu       $a0, $a0, 0x4230
    ctx->r4 = ADD32(ctx->r4, 0X4230);
    // 0x80041144: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x80041148: lw          $a2, 0x4($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X4);
    // 0x8004114C: jal         0x800A17E0
    // 0x80041150: lw          $a3, 0x8($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X8);
    guTranslateF(rdram, ctx);
        goto after_2;
    // 0x80041150: lw          $a3, 0x8($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X8);
    after_2:
    // 0x80041154: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041158: addiu       $a0, $a0, 0x4230
    ctx->r4 = ADD32(ctx->r4, 0X4230);
    // 0x8004115C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80041160: jal         0x8000CC18
    // 0x80041164: lw          $a1, 0x88($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X88);
    func_8000CC18(rdram, ctx);
        goto after_3;
    // 0x80041164: lw          $a1, 0x88($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X88);
    after_3:
    // 0x80041168: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8004116C: lwc1        $f4, 0x4260($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X4260);
    // 0x80041170: lwc1        $f6, 0x4264($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X4264);
    // 0x80041174: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80041178: lwc1        $f8, 0x4268($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X4268);
    // 0x8004117C: lui         $s0, 0x800F
    ctx->r16 = S32(0X800F << 16);
    // 0x80041180: addiu       $s0, $s0, 0x41F0
    ctx->r16 = ADD32(ctx->r16, 0X41F0);
    // 0x80041184: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80041188: lw          $a1, 0x84($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X84);
    // 0x8004118C: swc1        $f4, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f4.u32l;
    // 0x80041190: swc1        $f6, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f6.u32l;
    // 0x80041194: jal         0x8000322C
    // 0x80041198: swc1        $f8, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f8.u32l;
    func_8000322C(rdram, ctx);
        goto after_4;
    // 0x80041198: swc1        $f8, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f8.u32l;
    after_4:
    // 0x8004119C: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x800411A0: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x800411A4: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    // 0x800411A8: jal         0x8000CC18
    // 0x800411AC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8000CC18(rdram, ctx);
        goto after_5;
    // 0x800411AC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x800411B0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800411B4: lw          $t0, 0x7C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X7C);
    // 0x800411B8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800411BC: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x800411C0: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x800411C4: jal         0x800A17E0
    // 0x800411C8: lw          $a2, 0x4($t0)
    ctx->r6 = MEM_W(ctx->r8, 0X4);
    guTranslateF(rdram, ctx);
        goto after_6;
    // 0x800411C8: lw          $a2, 0x4($t0)
    ctx->r6 = MEM_W(ctx->r8, 0X4);
    after_6:
    // 0x800411CC: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x800411D0: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x800411D4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800411D8: jal         0x8000CC18
    // 0x800411DC: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_8000CC18(rdram, ctx);
        goto after_7;
    // 0x800411DC: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_7:
    // 0x800411E0: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800411E4: lwc1        $f18, 0x4220($at)
    ctx->f18.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x800411E8: lwc1        $f10, 0x5C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x800411EC: lwc1        $f20, 0x4224($at)
    ctx->f20.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x800411F0: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x800411F4: sub.s       $f0, $f18, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = ctx->f18.fl - ctx->f10.fl;
    // 0x800411F8: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800411FC: lwc1        $f22, 0x4228($at)
    ctx->f22.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x80041200: sub.s       $f2, $f20, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f20.fl - ctx->f4.fl;
    // 0x80041204: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80041208: lwc1        $f6, 0x54($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X54);
    // 0x8004120C: swc1        $f0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f0.u32l;
    // 0x80041210: mul.s       $f10, $f2, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80041214: swc1        $f2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f2.u32l;
    // 0x80041218: sub.s       $f14, $f22, $f6
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f14.fl = ctx->f22.fl - ctx->f6.fl;
    // 0x8004121C: swc1        $f18, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f18.u32l;
    // 0x80041220: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80041224: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80041228: swc1        $f14, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f14.u32l;
    // 0x8004122C: jal         0x800A01E0
    // 0x80041230: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_8;
    // 0x80041230: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    after_8:
    // 0x80041234: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80041238: lw          $t1, 0x7C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X7C);
    // 0x8004123C: swc1        $f0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f0.u32l;
    // 0x80041240: mfc1        $a2, $f2
    ctx->r6 = (int32_t)ctx->f2.u32l;
    // 0x80041244: mfc1        $a3, $f2
    ctx->r7 = (int32_t)ctx->f2.u32l;
    // 0x80041248: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8004124C: jal         0x800A17E0
    // 0x80041250: lw          $a1, 0x0($t1)
    ctx->r5 = MEM_W(ctx->r9, 0X0);
    guTranslateF(rdram, ctx);
        goto after_9;
    // 0x80041250: lw          $a1, 0x0($t1)
    ctx->r5 = MEM_W(ctx->r9, 0X0);
    after_9:
    // 0x80041254: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80041258: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x8004125C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80041260: jal         0x8000CC18
    // 0x80041264: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_8000CC18(rdram, ctx);
        goto after_10;
    // 0x80041264: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_10:
    // 0x80041268: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8004126C: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x80041270: lwc1        $f8, 0x5C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80041274: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x80041278: lwc1        $f10, 0x58($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X58);
    // 0x8004127C: sub.s       $f0, $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f8.fl;
    // 0x80041280: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80041284: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x80041288: sub.s       $f2, $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f10.fl;
    // 0x8004128C: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80041290: lwc1        $f4, 0x54($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80041294: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80041298: sub.s       $f14, $f14, $f4
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f4.fl;
    // 0x8004129C: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x800412A0: add.s       $f6, $f0, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x800412A4: jal         0x800A01E0
    // 0x800412A8: add.s       $f12, $f6, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_11;
    // 0x800412A8: add.s       $f12, $f6, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f14.fl;
    after_11:
    // 0x800412AC: lw          $t2, 0x7C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X7C);
    // 0x800412B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800412B4: addiu       $a1, $zero, 0x0
    ctx->r5 = ADD32(0, 0X0);
    // 0x800412B8: lw          $a3, 0x0($t2)
    ctx->r7 = MEM_W(ctx->r10, 0X0);
    // 0x800412BC: swc1        $f0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f0.u32l;
    // 0x800412C0: jal         0x800A17E0
    // 0x800412C4: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    guTranslateF(rdram, ctx);
        goto after_12;
    // 0x800412C4: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    after_12:
    // 0x800412C8: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x800412CC: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x800412D0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800412D4: jal         0x8000CC18
    // 0x800412D8: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_8000CC18(rdram, ctx);
        goto after_13;
    // 0x800412D8: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_13:
    // 0x800412DC: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800412E0: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x800412E4: lwc1        $f8, 0x5C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x800412E8: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x800412EC: lwc1        $f10, 0x58($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X58);
    // 0x800412F0: sub.s       $f0, $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f8.fl;
    // 0x800412F4: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800412F8: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x800412FC: sub.s       $f2, $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f10.fl;
    // 0x80041300: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80041304: lwc1        $f4, 0x54($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80041308: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x8004130C: sub.s       $f14, $f14, $f4
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f4.fl;
    // 0x80041310: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80041314: add.s       $f6, $f0, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x80041318: jal         0x800A01E0
    // 0x8004131C: add.s       $f12, $f6, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_14;
    // 0x8004131C: add.s       $f12, $f6, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f14.fl;
    after_14:
    // 0x80041320: lwc1        $f16, 0x38($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80041324: lwc1        $f2, 0x34($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80041328: lwc1        $f18, 0x68($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X68);
    // 0x8004132C: add.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = ctx->f16.fl + ctx->f0.fl;
    // 0x80041330: lwc1        $f10, 0x50($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X50);
    // 0x80041334: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x80041338: c.le.s      $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f2.fl <= ctx->f16.fl;
    // 0x8004133C: nop

    // 0x80041340: bc1fl       L_800413E4
    if (!c1cs) {
        // 0x80041344: mtc1        $at, $f15
        ctx->f_odd[(15 - 1) * 2] = ctx->r1;
            goto L_800413E4;
    }
    goto skip_0;
    // 0x80041344: mtc1        $at, $f15
    ctx->f_odd[(15 - 1) * 2] = ctx->r1;
    skip_0:
    // 0x80041348: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x8004134C: mtc1        $at, $f15
    ctx->f_odd[(15 - 1) * 2] = ctx->r1;
    // 0x80041350: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x80041354: cvt.d.s     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f4.d = CVT_D_S(ctx->f10.fl);
    // 0x80041358: cvt.d.s     $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f8.d = CVT_D_S(ctx->f18.fl);
    // 0x8004135C: mul.d       $f6, $f4, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f14.d); 
    ctx->f6.d = MUL_D(ctx->f4.d, ctx->f14.d);
    // 0x80041360: cvt.d.s     $f4, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f4.d = CVT_D_S(ctx->f20.fl);
    // 0x80041364: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80041368: lw          $v0, 0x78($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X78);
    // 0x8004136C: lw          $v1, 0x74($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X74);
    // 0x80041370: sub.d       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f6.d); 
    ctx->f10.d = ctx->f8.d - ctx->f6.d;
    // 0x80041374: lwc1        $f8, 0x4C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80041378: cvt.d.s     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f6.d = CVT_D_S(ctx->f8.fl);
    // 0x8004137C: cvt.s.d     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f18.fl = CVT_S_D(ctx->f10.d);
    // 0x80041380: mul.d       $f10, $f6, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f14.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f14.d);
    // 0x80041384: cvt.d.s     $f6, $f22
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f22.fl); 
    ctx->f6.d = CVT_D_S(ctx->f22.fl);
    // 0x80041388: sub.d       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f10.d); 
    ctx->f8.d = ctx->f4.d - ctx->f10.d;
    // 0x8004138C: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80041390: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x80041394: cvt.d.s     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f10.d = CVT_D_S(ctx->f4.fl);
    // 0x80041398: cvt.s.d     $f20, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f20.fl = CVT_S_D(ctx->f8.d);
    // 0x8004139C: mul.d       $f8, $f10, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f14.d); 
    ctx->f8.d = MUL_D(ctx->f10.d, ctx->f14.d);
    // 0x800413A0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x800413A4: swc1        $f20, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f20.u32l;
    // 0x800413A8: sub.d       $f4, $f6, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f4.d = ctx->f6.d - ctx->f8.d;
    // 0x800413AC: div.s       $f6, $f2, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = DIV_S(ctx->f2.fl, ctx->f10.fl);
    // 0x800413B0: cvt.s.d     $f22, $f4
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f22.fl = CVT_S_D(ctx->f4.d);
    // 0x800413B4: swc1        $f22, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f22.u32l;
    // 0x800413B8: swc1        $f18, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
    // 0x800413BC: swc1        $f20, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f20.u32l;
    // 0x800413C0: swc1        $f22, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->f22.u32l;
    // 0x800413C4: lw          $t3, 0x6C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X6C);
    // 0x800413C8: swc1        $f6, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->f6.u32l;
    // 0x800413CC: lh          $v0, 0x9E($sp)
    ctx->r2 = MEM_H(ctx->r29, 0X9E);
    // 0x800413D0: ori         $v0, $v0, 0x8
    ctx->r2 = ctx->r2 | 0X8;
    // 0x800413D4: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x800413D8: b           L_800414A8
    // 0x800413DC: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
        goto L_800414A8;
    // 0x800413DC: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x800413E0: mtc1        $at, $f15
    ctx->f_odd[(15 - 1) * 2] = ctx->r1;
L_800413E4:
    // 0x800413E4: mtc1        $zero, $f14
    ctx->f14.u32l = 0;
    // 0x800413E8: cvt.d.s     $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f8.d = CVT_D_S(ctx->f16.fl);
    // 0x800413EC: lw          $t4, 0x6C($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X6C);
    // 0x800413F0: mul.d       $f4, $f8, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f14.d); 
    ctx->f4.d = MUL_D(ctx->f8.d, ctx->f14.d);
    // 0x800413F4: lw          $v1, 0x74($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X74);
    // 0x800413F8: cvt.s.d     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f16.fl = CVT_S_D(ctx->f4.d);
    // 0x800413FC: swc1        $f16, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->f16.u32l;
    // 0x80041400: lwc1        $f10, 0x50($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X50);
    // 0x80041404: lwc1        $f8, 0x34($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80041408: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x8004140C: mul.s       $f6, $f10, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80041410: div.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80041414: lwc1        $f6, 0x4C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80041418: add.s       $f10, $f4, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f0.fl;
    // 0x8004141C: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80041420: lwc1        $f6, 0x58($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80041424: sub.s       $f18, $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = ctx->f18.fl - ctx->f0.fl;
    // 0x80041428: swc1        $f10, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f10.u32l;
    // 0x8004142C: div.s       $f2, $f4, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f4.fl, ctx->f8.fl);
    // 0x80041430: add.s       $f4, $f6, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f2.fl;
    // 0x80041434: lwc1        $f6, 0x48($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80041438: sub.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = ctx->f20.fl - ctx->f2.fl;
    // 0x8004143C: swc1        $f4, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f4.u32l;
    // 0x80041440: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80041444: lwc1        $f6, 0x54($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80041448: div.s       $f12, $f4, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = DIV_S(ctx->f4.fl, ctx->f8.fl);
    // 0x8004144C: add.s       $f4, $f6, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f12.fl;
    // 0x80041450: sub.s       $f22, $f22, $f12
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f22.fl = ctx->f22.fl - ctx->f12.fl;
    // 0x80041454: swc1        $f4, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f4.u32l;
    // 0x80041458: swc1        $f10, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f10.u32l;
    // 0x8004145C: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80041460: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    // 0x80041464: lw          $v0, 0x78($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X78);
    // 0x80041468: swc1        $f8, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f8.u32l;
    // 0x8004146C: lwc1        $f6, 0x54($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80041470: swc1        $f6, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->f6.u32l;
    // 0x80041474: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x80041478: swc1        $f20, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f20.u32l;
    // 0x8004147C: swc1        $f22, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f22.u32l;
    // 0x80041480: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80041484: sub.s       $f10, $f18, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x80041488: swc1        $f10, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f10.u32l;
    // 0x8004148C: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80041490: sub.s       $f6, $f20, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f20.fl - ctx->f8.fl;
    // 0x80041494: swc1        $f6, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f6.u32l;
    // 0x80041498: lwc1        $f4, 0x54($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X54);
    // 0x8004149C: sub.s       $f10, $f22, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f22.fl - ctx->f4.fl;
    // 0x800414A0: swc1        $f10, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f10.u32l;
L_800414A4:
    // 0x800414A4: lh          $v0, 0x9E($sp)
    ctx->r2 = MEM_H(ctx->r29, 0X9E);
L_800414A8:
    // 0x800414A8: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x800414AC: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x800414B0: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x800414B4: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x800414B8: jr          $ra
    // 0x800414BC: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
    return;
    // 0x800414BC: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
;}
RECOMP_FUNC void func_800414C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800414C0: sll         $a2, $a1, 16
    ctx->r6 = S32(ctx->r5 << 16);
    // 0x800414C4: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x800414C8: addiu       $sp, $sp, -0x88
    ctx->r29 = ADD32(ctx->r29, -0X88);
    // 0x800414CC: andi        $v1, $a2, 0x2000
    ctx->r3 = ctx->r6 & 0X2000;
    // 0x800414D0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800414D4: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x800414D8: beq         $v1, $zero, L_800414E8
    if (ctx->r3 == 0) {
        // 0x800414DC: sw          $a1, 0x8C($sp)
        MEM_W(0X8C, ctx->r29) = ctx->r5;
            goto L_800414E8;
    }
    // 0x800414DC: sw          $a1, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->r5;
    // 0x800414E0: b           L_800414EC
    // 0x800414E4: lw          $a3, 0x68($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X68);
        goto L_800414EC;
    // 0x800414E4: lw          $a3, 0x68($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X68);
L_800414E8:
    // 0x800414E8: lw          $a3, 0x2C($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X2C);
L_800414EC:
    // 0x800414EC: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x800414F0: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x800414F4: sw          $a0, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r4;
    // 0x800414F8: sh          $a2, 0x8E($sp)
    MEM_H(0X8E, ctx->r29) = ctx->r6;
    // 0x800414FC: jal         0x80040C30
    // 0x80041500: sw          $a3, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r7;
    func_80040C30(rdram, ctx);
        goto after_0;
    // 0x80041500: sw          $a3, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r7;
    after_0:
    // 0x80041504: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x80041508: lw          $a0, 0x88($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X88);
    // 0x8004150C: beq         $v0, $zero, L_800418AC
    if (ctx->r2 == 0) {
        // 0x80041510: lw          $a3, 0x84($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X84);
            goto L_800418AC;
    }
    // 0x80041510: lw          $a3, 0x84($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X84);
    // 0x80041514: beq         $v1, $zero, L_80041544
    if (ctx->r3 == 0) {
        // 0x80041518: addiu       $t6, $a0, 0x60
        ctx->r14 = ADD32(ctx->r4, 0X60);
            goto L_80041544;
    }
    // 0x80041518: addiu       $t6, $a0, 0x60
    ctx->r14 = ADD32(ctx->r4, 0X60);
    // 0x8004151C: addiu       $t7, $a0, 0x7C
    ctx->r15 = ADD32(ctx->r4, 0X7C);
    // 0x80041520: addiu       $t8, $a0, 0x88
    ctx->r24 = ADD32(ctx->r4, 0X88);
    // 0x80041524: addiu       $t9, $a0, 0x94
    ctx->r25 = ADD32(ctx->r4, 0X94);
    // 0x80041528: addiu       $t0, $a0, 0x54
    ctx->r8 = ADD32(ctx->r4, 0X54);
    // 0x8004152C: sw          $t6, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r14;
    // 0x80041530: addiu       $v0, $a0, 0x70
    ctx->r2 = ADD32(ctx->r4, 0X70);
    // 0x80041534: sw          $t7, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r15;
    // 0x80041538: sw          $t8, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r24;
    // 0x8004153C: b           L_8004156C
    // 0x80041540: sw          $t9, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r25;
        goto L_8004156C;
    // 0x80041540: sw          $t9, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r25;
L_80041544:
    // 0x80041544: addiu       $t1, $a0, 0x24
    ctx->r9 = ADD32(ctx->r4, 0X24);
    // 0x80041548: addiu       $t2, $a0, 0x4C
    ctx->r10 = ADD32(ctx->r4, 0X4C);
    // 0x8004154C: addiu       $t3, $a0, 0x58
    ctx->r11 = ADD32(ctx->r4, 0X58);
    // 0x80041550: addiu       $t4, $a0, 0x64
    ctx->r12 = ADD32(ctx->r4, 0X64);
    // 0x80041554: addiu       $t0, $a0, 0x18
    ctx->r8 = ADD32(ctx->r4, 0X18);
    // 0x80041558: sw          $t1, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r9;
    // 0x8004155C: addiu       $v0, $a0, 0x40
    ctx->r2 = ADD32(ctx->r4, 0X40);
    // 0x80041560: sw          $t2, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r10;
    // 0x80041564: sw          $t3, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r11;
    // 0x80041568: sw          $t4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r12;
L_8004156C:
    // 0x8004156C: lhu         $v1, 0x2($a3)
    ctx->r3 = MEM_HU(ctx->r7, 0X2);
    // 0x80041570: andi        $t5, $v1, 0x8000
    ctx->r13 = ctx->r3 & 0X8000;
    // 0x80041574: beq         $t5, $zero, L_8004158C
    if (ctx->r13 == 0) {
        // 0x80041578: andi        $t6, $v1, 0x2000
        ctx->r14 = ctx->r3 & 0X2000;
            goto L_8004158C;
    }
    // 0x80041578: andi        $t6, $v1, 0x2000
    ctx->r14 = ctx->r3 & 0X2000;
    // 0x8004157C: beq         $t6, $zero, L_8004158C
    if (ctx->r14 == 0) {
        // 0x80041580: addiu       $t7, $a3, 0x78
        ctx->r15 = ADD32(ctx->r7, 0X78);
            goto L_8004158C;
    }
    // 0x80041580: addiu       $t7, $a3, 0x78
    ctx->r15 = ADD32(ctx->r7, 0X78);
    // 0x80041584: b           L_800415C0
    // 0x80041588: sw          $t7, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r15;
        goto L_800415C0;
    // 0x80041588: sw          $t7, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r15;
L_8004158C:
    // 0x8004158C: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x80041590: lh          $t8, -0x7CA0($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X7CA0);
    // 0x80041594: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041598: addiu       $a0, $a0, 0x4270
    ctx->r4 = ADD32(ctx->r4, 0X4270);
    // 0x8004159C: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x800415A0: sw          $a0, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r4;
    // 0x800415A4: addu        $t1, $a3, $t9
    ctx->r9 = ADD32(ctx->r7, ctx->r25);
    // 0x800415A8: lw          $a1, 0x44($t1)
    ctx->r5 = MEM_W(ctx->r9, 0X44);
    // 0x800415AC: sw          $t0, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r8;
    // 0x800415B0: jal         0x8009A4E8
    // 0x800415B4: sw          $v0, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r2;
    func_8009A4E8(rdram, ctx);
        goto after_1;
    // 0x800415B4: sw          $v0, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r2;
    after_1:
    // 0x800415B8: lw          $v0, 0x6C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X6C);
    // 0x800415BC: lw          $t0, 0x74($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X74);
L_800415C0:
    // 0x800415C0: lh          $v1, 0x8E($sp)
    ctx->r3 = MEM_H(ctx->r29, 0X8E);
    // 0x800415C4: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x800415C8: addiu       $a0, $a0, 0x4230
    ctx->r4 = ADD32(ctx->r4, 0X4230);
    // 0x800415CC: andi        $v1, $v1, 0x10
    ctx->r3 = ctx->r3 & 0X10;
    // 0x800415D0: beql        $v1, $zero, L_80041618
    if (ctx->r3 == 0) {
        // 0x800415D4: lw          $a1, 0x0($t0)
        ctx->r5 = MEM_W(ctx->r8, 0X0);
            goto L_80041618;
    }
    goto skip_0;
    // 0x800415D4: lw          $a1, 0x0($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X0);
    skip_0:
    // 0x800415D8: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x800415DC: lw          $t2, 0x68($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X68);
    // 0x800415E0: swc1        $f4, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f4.u32l;
    // 0x800415E4: lwc1        $f6, 0x50($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X50);
    // 0x800415E8: swc1        $f6, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f6.u32l;
    // 0x800415EC: lwc1        $f8, 0x4($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X4);
    // 0x800415F0: lw          $t3, 0x68($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X68);
    // 0x800415F4: swc1        $f8, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f8.u32l;
    // 0x800415F8: lwc1        $f10, 0x4C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x800415FC: swc1        $f10, 0x4($t3)
    MEM_W(0X4, ctx->r11) = ctx->f10.u32l;
    // 0x80041600: lwc1        $f18, 0x8($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80041604: lw          $t4, 0x68($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X68);
    // 0x80041608: swc1        $f18, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f18.u32l;
    // 0x8004160C: lwc1        $f4, 0x48($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80041610: swc1        $f4, 0x8($t4)
    MEM_W(0X8, ctx->r12) = ctx->f4.u32l;
    // 0x80041614: lw          $a1, 0x0($t0)
    ctx->r5 = MEM_W(ctx->r8, 0X0);
L_80041618:
    // 0x80041618: lw          $a2, 0x4($t0)
    ctx->r6 = MEM_W(ctx->r8, 0X4);
    // 0x8004161C: lw          $a3, 0x8($t0)
    ctx->r7 = MEM_W(ctx->r8, 0X8);
    // 0x80041620: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x80041624: jal         0x800A17E0
    // 0x80041628: sw          $v0, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r2;
    guTranslateF(rdram, ctx);
        goto after_2;
    // 0x80041628: sw          $v0, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r2;
    after_2:
    // 0x8004162C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041630: addiu       $a0, $a0, 0x4230
    ctx->r4 = ADD32(ctx->r4, 0X4230);
    // 0x80041634: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80041638: jal         0x8000CC18
    // 0x8004163C: lw          $a1, 0x78($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X78);
    func_8000CC18(rdram, ctx);
        goto after_3;
    // 0x8004163C: lw          $a1, 0x78($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X78);
    after_3:
    // 0x80041640: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80041644: lw          $v0, 0x6C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X6C);
    // 0x80041648: lwc1        $f0, 0x4260($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4260);
    // 0x8004164C: lwc1        $f2, 0x4264($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4264);
    // 0x80041650: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80041654: lwc1        $f12, 0x4268($at)
    ctx->f12.u32l = MEM_W(ctx->r1, 0X4268);
    // 0x80041658: swc1        $f0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f0.u32l;
    // 0x8004165C: swc1        $f2, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f2.u32l;
    // 0x80041660: swc1        $f12, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f12.u32l;
    // 0x80041664: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80041668: lw          $t5, 0x70($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X70);
    // 0x8004166C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041670: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x80041674: lw          $a1, 0x0($t5)
    ctx->r5 = MEM_W(ctx->r13, 0X0);
    // 0x80041678: mfc1        $a3, $f20
    ctx->r7 = (int32_t)ctx->f20.u32l;
    // 0x8004167C: addiu       $a0, $a0, 0x41F0
    ctx->r4 = ADD32(ctx->r4, 0X41F0);
    // 0x80041680: swc1        $f0, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f0.u32l;
    // 0x80041684: swc1        $f2, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f2.u32l;
    // 0x80041688: jal         0x800A17E0
    // 0x8004168C: swc1        $f12, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f12.u32l;
    guTranslateF(rdram, ctx);
        goto after_4;
    // 0x8004168C: swc1        $f12, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f12.u32l;
    after_4:
    // 0x80041690: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041694: addiu       $a0, $a0, 0x41F0
    ctx->r4 = ADD32(ctx->r4, 0X41F0);
    // 0x80041698: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8004169C: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x800416A0: jal         0x8000CC18
    // 0x800416A4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    func_8000CC18(rdram, ctx);
        goto after_5;
    // 0x800416A4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    after_5:
    // 0x800416A8: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800416AC: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x800416B0: lwc1        $f6, 0x5C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x800416B4: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x800416B8: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x800416BC: sub.s       $f0, $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f6.fl;
    // 0x800416C0: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800416C4: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x800416C8: sub.s       $f2, $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f8.fl;
    // 0x800416CC: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x800416D0: lwc1        $f10, 0x54($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X54);
    // 0x800416D4: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x800416D8: sub.s       $f14, $f14, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f10.fl;
    // 0x800416DC: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x800416E0: add.s       $f18, $f0, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x800416E4: jal         0x800A01E0
    // 0x800416E8: add.s       $f12, $f18, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_6;
    // 0x800416E8: add.s       $f12, $f18, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f14.fl;
    after_6:
    // 0x800416EC: lw          $t6, 0x70($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X70);
    // 0x800416F0: swc1        $f0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f0.u32l;
    // 0x800416F4: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x800416F8: mfc1        $a1, $f20
    ctx->r5 = (int32_t)ctx->f20.u32l;
    // 0x800416FC: mfc1        $a3, $f20
    ctx->r7 = (int32_t)ctx->f20.u32l;
    // 0x80041700: addiu       $a0, $a0, 0x41F0
    ctx->r4 = ADD32(ctx->r4, 0X41F0);
    // 0x80041704: jal         0x800A17E0
    // 0x80041708: lw          $a2, 0x0($t6)
    ctx->r6 = MEM_W(ctx->r14, 0X0);
    guTranslateF(rdram, ctx);
        goto after_7;
    // 0x80041708: lw          $a2, 0x0($t6)
    ctx->r6 = MEM_W(ctx->r14, 0X0);
    after_7:
    // 0x8004170C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041710: addiu       $a0, $a0, 0x41F0
    ctx->r4 = ADD32(ctx->r4, 0X41F0);
    // 0x80041714: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80041718: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x8004171C: jal         0x8000CC18
    // 0x80041720: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    func_8000CC18(rdram, ctx);
        goto after_8;
    // 0x80041720: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    after_8:
    // 0x80041724: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80041728: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x8004172C: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80041730: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x80041734: lwc1        $f6, 0x58($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80041738: sub.s       $f0, $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f4.fl;
    // 0x8004173C: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x80041740: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x80041744: sub.s       $f2, $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f6.fl;
    // 0x80041748: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x8004174C: lwc1        $f8, 0x54($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80041750: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80041754: sub.s       $f14, $f14, $f8
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f8.fl;
    // 0x80041758: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8004175C: add.s       $f10, $f0, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x80041760: jal         0x800A01E0
    // 0x80041764: add.s       $f12, $f10, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_9;
    // 0x80041764: add.s       $f12, $f10, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f14.fl;
    after_9:
    // 0x80041768: lwc1        $f18, 0x2C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x8004176C: lw          $t7, 0x70($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X70);
    // 0x80041770: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80041774: add.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f0.fl;
    // 0x80041778: mfc1        $a1, $f20
    ctx->r5 = (int32_t)ctx->f20.u32l;
    // 0x8004177C: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x80041780: addiu       $a0, $a0, 0x41F0
    ctx->r4 = ADD32(ctx->r4, 0X41F0);
    // 0x80041784: swc1        $f4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f4.u32l;
    // 0x80041788: jal         0x800A17E0
    // 0x8004178C: lw          $a3, 0x0($t7)
    ctx->r7 = MEM_W(ctx->r15, 0X0);
    guTranslateF(rdram, ctx);
        goto after_10;
    // 0x8004178C: lw          $a3, 0x0($t7)
    ctx->r7 = MEM_W(ctx->r15, 0X0);
    after_10:
    // 0x80041790: lui         $t8, 0x800F
    ctx->r24 = S32(0X800F << 16);
    // 0x80041794: addiu       $a2, $t8, 0x41F0
    ctx->r6 = ADD32(ctx->r24, 0X41F0);
    // 0x80041798: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8004179C: addiu       $a1, $a1, 0x4230
    ctx->r5 = ADD32(ctx->r5, 0X4230);
    // 0x800417A0: jal         0x8000CC18
    // 0x800417A4: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    func_8000CC18(rdram, ctx);
        goto after_11;
    // 0x800417A4: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_11:
    // 0x800417A8: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800417AC: lwc1        $f0, 0x4220($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X4220);
    // 0x800417B0: lwc1        $f6, 0x5C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x800417B4: lwc1        $f2, 0x4224($at)
    ctx->f2.u32l = MEM_W(ctx->r1, 0X4224);
    // 0x800417B8: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x800417BC: sub.s       $f0, $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f0.fl - ctx->f6.fl;
    // 0x800417C0: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800417C4: lwc1        $f14, 0x4228($at)
    ctx->f14.u32l = MEM_W(ctx->r1, 0X4228);
    // 0x800417C8: sub.s       $f2, $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = ctx->f2.fl - ctx->f8.fl;
    // 0x800417CC: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x800417D0: lwc1        $f10, 0x54($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X54);
    // 0x800417D4: mul.s       $f2, $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x800417D8: sub.s       $f14, $f14, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f14.fl - ctx->f10.fl;
    // 0x800417DC: mul.s       $f14, $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x800417E0: add.s       $f18, $f0, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = ctx->f0.fl + ctx->f2.fl;
    // 0x800417E4: jal         0x800A01E0
    // 0x800417E8: add.s       $f12, $f18, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f14.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_12;
    // 0x800417E8: add.s       $f12, $f18, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f14.fl;
    after_12:
    // 0x800417EC: lwc1        $f16, 0x2C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x800417F0: lui         $at, 0x4008
    ctx->r1 = S32(0X4008 << 16);
    // 0x800417F4: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x800417F8: add.s       $f16, $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = ctx->f16.fl + ctx->f0.fl;
    // 0x800417FC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80041800: lw          $t9, 0x60($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X60);
    // 0x80041804: mov.s       $f2, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 20);
    ctx->f2.fl = ctx->f20.fl;
    // 0x80041808: cvt.d.s     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f4.d = CVT_D_S(ctx->f16.fl);
    // 0x8004180C: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    // 0x80041810: div.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = DIV_D(ctx->f4.d, ctx->f6.d);
    // 0x80041814: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    // 0x80041818: cvt.s.d     $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f16.fl = CVT_S_D(ctx->f8.d);
    // 0x8004181C: swc1        $f16, 0x0($t9)
    MEM_W(0X0, ctx->r25) = ctx->f16.u32l;
    // 0x80041820: lw          $t1, 0x24($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X24);
    // 0x80041824: lwc1        $f10, 0x5C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80041828: lw          $v0, 0x68($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X68);
    // 0x8004182C: beq         $t1, $zero, L_80041858
    if (ctx->r9 == 0) {
        // 0x80041830: lwc1        $f18, 0x5C($sp)
        ctx->f18.u32l = MEM_W(ctx->r29, 0X5C);
            goto L_80041858;
    }
    // 0x80041830: lwc1        $f18, 0x5C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80041834: lwc1        $f18, 0x50($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X50);
    // 0x80041838: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x8004183C: lwc1        $f6, 0x4C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80041840: sub.s       $f2, $f10, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f18.fl;
    // 0x80041844: lwc1        $f10, 0x48($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80041848: lwc1        $f8, 0x54($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X54);
    // 0x8004184C: sub.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80041850: b           L_8004186C
    // 0x80041854: sub.s       $f14, $f8, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f8.fl - ctx->f10.fl;
        goto L_8004186C;
    // 0x80041854: sub.s       $f14, $f8, $f10
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f14.fl = ctx->f8.fl - ctx->f10.fl;
L_80041858:
    // 0x80041858: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x8004185C: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80041860: swc1        $f4, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f4.u32l;
    // 0x80041864: lwc1        $f6, 0x54($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80041868: swc1        $f6, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f6.u32l;
L_8004186C:
    // 0x8004186C: add.s       $f8, $f2, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f2.fl + ctx->f12.fl;
    // 0x80041870: lw          $v0, 0x64($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X64);
    // 0x80041874: add.s       $f10, $f8, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f14.fl;
    // 0x80041878: swc1        $f2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f2.u32l;
    // 0x8004187C: swc1        $f12, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f12.u32l;
    // 0x80041880: swc1        $f14, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f14.u32l;
    // 0x80041884: c.eq.s      $f20, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f20.fl == ctx->f10.fl;
    // 0x80041888: lh          $t2, 0x8E($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X8E);
    // 0x8004188C: bc1f        L_80041898
    if (!c1cs) {
        // 0x80041890: ori         $t3, $t2, 0x8
        ctx->r11 = ctx->r10 | 0X8;
            goto L_80041898;
    }
    // 0x80041890: ori         $t3, $t2, 0x8
    ctx->r11 = ctx->r10 | 0X8;
    // 0x80041894: sh          $t3, 0x8E($sp)
    MEM_H(0X8E, ctx->r29) = ctx->r11;
L_80041898:
    // 0x80041898: lh          $v0, 0x8E($sp)
    ctx->r2 = MEM_H(ctx->r29, 0X8E);
    // 0x8004189C: ori         $v0, $v0, 0x70
    ctx->r2 = ctx->r2 | 0X70;
    // 0x800418A0: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x800418A4: b           L_800418BC
    // 0x800418A8: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
        goto L_800418BC;
    // 0x800418A8: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
L_800418AC:
    // 0x800418AC: lh          $v0, 0x8E($sp)
    ctx->r2 = MEM_H(ctx->r29, 0X8E);
    // 0x800418B0: andi        $v0, $v0, 0xFFEF
    ctx->r2 = ctx->r2 & 0XFFEF;
    // 0x800418B4: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x800418B8: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
L_800418BC:
    // 0x800418BC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800418C0: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x800418C4: addiu       $sp, $sp, 0x88
    ctx->r29 = ADD32(ctx->r29, 0X88);
    // 0x800418C8: jr          $ra
    // 0x800418CC: nop

    return;
    // 0x800418CC: nop

;}
RECOMP_FUNC void func_800418D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800418D0: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x800418D4: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x800418D8: sra         $v0, $a1, 16
    ctx->r2 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800418DC: jr          $ra
    // 0x800418E0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x800418E0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_800418E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800418E4: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x800418E8: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x800418EC: sra         $v0, $a1, 16
    ctx->r2 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800418F0: jr          $ra
    // 0x800418F4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x800418F4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_800418F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800418F8: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x800418FC: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80041900: sra         $v0, $a1, 16
    ctx->r2 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80041904: jr          $ra
    // 0x80041908: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x80041908: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8004190C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8004190C: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x80041910: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80041914: lhu         $t6, -0x7814($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X7814);
    // 0x80041918: sw          $ra, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r31;
    // 0x8004191C: sw          $fp, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r30;
    // 0x80041920: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x80041924: sw          $s7, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r23;
    // 0x80041928: sw          $s6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r22;
    // 0x8004192C: sw          $s5, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r21;
    // 0x80041930: sw          $s4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r20;
    // 0x80041934: sw          $s3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r19;
    // 0x80041938: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x8004193C: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x80041940: beq         $t7, $zero, L_80041968
    if (ctx->r15 == 0) {
        // 0x80041944: sw          $s0, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r16;
            goto L_80041968;
    }
    // 0x80041944: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x80041948: lui         $v0, 0x8010
    ctx->r2 = S32(0X8010 << 16);
    // 0x8004194C: lui         $t8, 0x800F
    ctx->r24 = S32(0X800F << 16);
    // 0x80041950: addiu       $v0, $v0, -0x6F18
    ctx->r2 = ADD32(ctx->r2, -0X6F18);
    // 0x80041954: addiu       $t8, $t8, 0x42E8
    ctx->r24 = ADD32(ctx->r24, 0X42E8);
    // 0x80041958: sw          $t8, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r24;
    // 0x8004195C: or          $fp, $v0, $zero
    ctx->r30 = ctx->r2 | 0;
    // 0x80041960: b           L_80041988
    // 0x80041964: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
        goto L_80041988;
    // 0x80041964: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
L_80041968:
    // 0x80041968: lui         $t9, 0x8010
    ctx->r25 = S32(0X8010 << 16);
    // 0x8004196C: addiu       $t9, $t9, -0x6FB4
    ctx->r25 = ADD32(ctx->r25, -0X6FB4);
    // 0x80041970: lui         $fp, 0x800F
    ctx->r30 = S32(0X800F << 16);
    // 0x80041974: lui         $v0, 0x8010
    ctx->r2 = S32(0X8010 << 16);
    // 0x80041978: sw          $t9, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r25;
    // 0x8004197C: addiu       $fp, $fp, 0x424C
    ctx->r30 = ADD32(ctx->r30, 0X424C);
    // 0x80041980: addiu       $v0, $v0, -0x6F18
    ctx->r2 = ADD32(ctx->r2, -0X6F18);
    // 0x80041984: addiu       $s1, $zero, -0x1
    ctx->r17 = ADD32(0, -0X1);
L_80041988:
    // 0x80041988: lui         $s6, 0x800F
    ctx->r22 = S32(0X800F << 16);
    // 0x8004198C: addiu       $s6, $s6, 0x42E8
    ctx->r22 = ADD32(ctx->r22, 0X42E8);
    // 0x80041990: lh          $t0, 0x0($s6)
    ctx->r8 = MEM_H(ctx->r22, 0X0);
L_80041994:
    // 0x80041994: sh          $t0, 0x54($sp)
    MEM_H(0X54, ctx->r29) = ctx->r8;
    // 0x80041998: lh          $t1, 0x54($sp)
    ctx->r9 = MEM_H(ctx->r29, 0X54);
    // 0x8004199C: andi        $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 & 0X2000;
    // 0x800419A0: beql        $t2, $zero, L_800419BC
    if (ctx->r10 == 0) {
        // 0x800419A4: addiu       $s6, $s6, 0x9C
        ctx->r22 = ADD32(ctx->r22, 0X9C);
            goto L_800419BC;
    }
    goto skip_0;
    // 0x800419A4: addiu       $s6, $s6, 0x9C
    ctx->r22 = ADD32(ctx->r22, 0X9C);
    skip_0:
    // 0x800419A8: lhu         $t4, 0xA($s6)
    ctx->r12 = MEM_HU(ctx->r22, 0XA);
    // 0x800419AC: andi        $t5, $t4, 0xE00
    ctx->r13 = ctx->r12 & 0XE00;
    // 0x800419B0: beq         $t5, $zero, L_800419C8
    if (ctx->r13 == 0) {
        // 0x800419B4: sh          $t4, 0x4E($sp)
        MEM_H(0X4E, ctx->r29) = ctx->r12;
            goto L_800419C8;
    }
    // 0x800419B4: sh          $t4, 0x4E($sp)
    MEM_H(0X4E, ctx->r29) = ctx->r12;
    // 0x800419B8: addiu       $s6, $s6, 0x9C
    ctx->r22 = ADD32(ctx->r22, 0X9C);
L_800419BC:
    // 0x800419BC: sltu        $at, $s6, $v0
    ctx->r1 = ctx->r22 < ctx->r2 ? 1 : 0;
    // 0x800419C0: bnel        $at, $zero, L_80041994
    if (ctx->r1 != 0) {
        // 0x800419C4: lh          $t0, 0x0($s6)
        ctx->r8 = MEM_H(ctx->r22, 0X0);
            goto L_80041994;
    }
    goto skip_1;
    // 0x800419C4: lh          $t0, 0x0($s6)
    ctx->r8 = MEM_H(ctx->r22, 0X0);
    skip_1:
L_800419C8:
    // 0x800419C8: sltu        $at, $s6, $v0
    ctx->r1 = ctx->r22 < ctx->r2 ? 1 : 0;
    // 0x800419CC: bne         $at, $zero, L_800419D8
    if (ctx->r1 != 0) {
        // 0x800419D0: lh          $t6, 0x54($sp)
        ctx->r14 = MEM_H(ctx->r29, 0X54);
            goto L_800419D8;
    }
    // 0x800419D0: lh          $t6, 0x54($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X54);
    // 0x800419D4: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
L_800419D8:
    // 0x800419D8: beq         $s6, $zero, L_80041A58
    if (ctx->r22 == 0) {
        // 0x800419DC: andi        $t7, $t6, 0x100
        ctx->r15 = ctx->r14 & 0X100;
            goto L_80041A58;
    }
    // 0x800419DC: andi        $t7, $t6, 0x100
    ctx->r15 = ctx->r14 & 0X100;
    // 0x800419E0: beq         $t7, $zero, L_80041A58
    if (ctx->r15 == 0) {
        // 0x800419E4: lw          $s0, 0x60($sp)
        ctx->r16 = MEM_W(ctx->r29, 0X60);
            goto L_80041A58;
    }
    // 0x800419E4: lw          $s0, 0x60($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X60);
    // 0x800419E8: sll         $s7, $s1, 2
    ctx->r23 = S32(ctx->r17 << 2);
    // 0x800419EC: addu        $s7, $s7, $s1
    ctx->r23 = ADD32(ctx->r23, ctx->r17);
    // 0x800419F0: sll         $s7, $s7, 3
    ctx->r23 = S32(ctx->r23 << 3);
    // 0x800419F4: subu        $s7, $s7, $s1
    ctx->r23 = SUB32(ctx->r23, ctx->r17);
    // 0x800419F8: sll         $s7, $s7, 2
    ctx->r23 = S32(ctx->r23 << 2);
    // 0x800419FC: addiu       $s5, $zero, 0x600
    ctx->r21 = ADD32(0, 0X600);
    // 0x80041A00: lh          $a3, 0x0($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X0);
L_80041A04:
    // 0x80041A04: andi        $t8, $a3, 0x2000
    ctx->r24 = ctx->r7 & 0X2000;
    // 0x80041A08: beq         $t8, $zero, L_80041A4C
    if (ctx->r24 == 0) {
        // 0x80041A0C: andi        $t9, $a3, 0x600
        ctx->r25 = ctx->r7 & 0X600;
            goto L_80041A4C;
    }
    // 0x80041A0C: andi        $t9, $a3, 0x600
    ctx->r25 = ctx->r7 & 0X600;
    // 0x80041A10: beql        $t9, $zero, L_80041A50
    if (ctx->r25 == 0) {
        // 0x80041A14: addu        $s0, $s0, $s7
        ctx->r16 = ADD32(ctx->r16, ctx->r23);
            goto L_80041A50;
    }
    goto skip_2;
    // 0x80041A14: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
    skip_2:
    // 0x80041A18: lhu         $v1, 0xA($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0XA);
    // 0x80041A1C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80041A20: andi        $v0, $v0, 0x600
    ctx->r2 = ctx->r2 & 0X600;
    // 0x80041A24: beql        $v0, $zero, L_80041A50
    if (ctx->r2 == 0) {
        // 0x80041A28: addu        $s0, $s0, $s7
        ctx->r16 = ADD32(ctx->r16, ctx->r23);
            goto L_80041A50;
    }
    goto skip_3;
    // 0x80041A28: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
    skip_3:
    // 0x80041A2C: beq         $s5, $v0, L_80041A4C
    if (ctx->r21 == ctx->r2) {
        // 0x80041A30: or          $a0, $s6, $zero
        ctx->r4 = ctx->r22 | 0;
            goto L_80041A4C;
    }
    // 0x80041A30: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80041A34: lhu         $t0, 0x4E($sp)
    ctx->r8 = MEM_HU(ctx->r29, 0X4E);
    // 0x80041A38: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80041A3C: lh          $a2, 0x54($sp)
    ctx->r6 = MEM_H(ctx->r29, 0X54);
    // 0x80041A40: sw          $v1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r3;
    // 0x80041A44: jal         0x80041C54
    // 0x80041A48: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    poolObject_complete(rdram, ctx);
        goto after_0;
    // 0x80041A48: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    after_0:
L_80041A4C:
    // 0x80041A4C: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
L_80041A50:
    // 0x80041A50: bnel        $s0, $fp, L_80041A04
    if (ctx->r16 != ctx->r30) {
        // 0x80041A54: lh          $a3, 0x0($s0)
        ctx->r7 = MEM_H(ctx->r16, 0X0);
            goto L_80041A04;
    }
    goto skip_4;
    // 0x80041A54: lh          $a3, 0x0($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X0);
    skip_4:
L_80041A58:
    // 0x80041A58: sll         $s7, $s1, 2
    ctx->r23 = S32(ctx->r17 << 2);
    // 0x80041A5C: addu        $s7, $s7, $s1
    ctx->r23 = ADD32(ctx->r23, ctx->r17);
    // 0x80041A60: sll         $s7, $s7, 3
    ctx->r23 = S32(ctx->r23 << 3);
    // 0x80041A64: subu        $s7, $s7, $s1
    ctx->r23 = SUB32(ctx->r23, ctx->r17);
    // 0x80041A68: addiu       $s5, $zero, 0x600
    ctx->r21 = ADD32(0, 0X600);
    // 0x80041A6C: lw          $s4, 0x60($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X60);
    // 0x80041A70: sll         $s7, $s7, 2
    ctx->r23 = S32(ctx->r23 << 2);
    // 0x80041A74: lh          $s3, 0x0($s4)
    ctx->r19 = MEM_H(ctx->r20, 0X0);
L_80041A78:
    // 0x80041A78: andi        $t1, $s3, 0x2000
    ctx->r9 = ctx->r19 & 0X2000;
    // 0x80041A7C: beq         $t1, $zero, L_80041AFC
    if (ctx->r9 == 0) {
        // 0x80041A80: andi        $t2, $s3, 0x100
        ctx->r10 = ctx->r19 & 0X100;
            goto L_80041AFC;
    }
    // 0x80041A80: andi        $t2, $s3, 0x100
    ctx->r10 = ctx->r19 & 0X100;
    // 0x80041A84: beql        $t2, $zero, L_80041B00
    if (ctx->r10 == 0) {
        // 0x80041A88: addu        $s4, $s4, $s7
        ctx->r20 = ADD32(ctx->r20, ctx->r23);
            goto L_80041B00;
    }
    goto skip_5;
    // 0x80041A88: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
    skip_5:
    // 0x80041A8C: lhu         $s2, 0xA($s4)
    ctx->r18 = MEM_HU(ctx->r20, 0XA);
    // 0x80041A90: andi        $t3, $s2, 0x600
    ctx->r11 = ctx->r18 & 0X600;
    // 0x80041A94: bnel        $t3, $zero, L_80041B00
    if (ctx->r11 != 0) {
        // 0x80041A98: addu        $s4, $s4, $s7
        ctx->r20 = ADD32(ctx->r20, ctx->r23);
            goto L_80041B00;
    }
    goto skip_6;
    // 0x80041A98: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
    skip_6:
    // 0x80041A9C: beq         $s4, $s6, L_80041AFC
    if (ctx->r20 == ctx->r22) {
        // 0x80041AA0: lw          $s0, 0x60($sp)
        ctx->r16 = MEM_W(ctx->r29, 0X60);
            goto L_80041AFC;
    }
    // 0x80041AA0: lw          $s0, 0x60($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X60);
    // 0x80041AA4: lh          $a3, 0x0($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X0);
L_80041AA8:
    // 0x80041AA8: andi        $t4, $a3, 0x2000
    ctx->r12 = ctx->r7 & 0X2000;
    // 0x80041AAC: beq         $t4, $zero, L_80041AF0
    if (ctx->r12 == 0) {
        // 0x80041AB0: andi        $t5, $a3, 0x600
        ctx->r13 = ctx->r7 & 0X600;
            goto L_80041AF0;
    }
    // 0x80041AB0: andi        $t5, $a3, 0x600
    ctx->r13 = ctx->r7 & 0X600;
    // 0x80041AB4: beql        $t5, $zero, L_80041AF4
    if (ctx->r13 == 0) {
        // 0x80041AB8: addu        $s0, $s0, $s7
        ctx->r16 = ADD32(ctx->r16, ctx->r23);
            goto L_80041AF4;
    }
    goto skip_7;
    // 0x80041AB8: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
    skip_7:
    // 0x80041ABC: lhu         $v1, 0xA($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0XA);
    // 0x80041AC0: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80041AC4: andi        $v0, $v0, 0x600
    ctx->r2 = ctx->r2 & 0X600;
    // 0x80041AC8: beql        $v0, $zero, L_80041AF4
    if (ctx->r2 == 0) {
        // 0x80041ACC: addu        $s0, $s0, $s7
        ctx->r16 = ADD32(ctx->r16, ctx->r23);
            goto L_80041AF4;
    }
    goto skip_8;
    // 0x80041ACC: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
    skip_8:
    // 0x80041AD0: beq         $s5, $v0, L_80041AF0
    if (ctx->r21 == ctx->r2) {
        // 0x80041AD4: or          $a0, $s4, $zero
        ctx->r4 = ctx->r20 | 0;
            goto L_80041AF0;
    }
    // 0x80041AD4: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80041AD8: sll         $a2, $s3, 16
    ctx->r6 = S32(ctx->r19 << 16);
    // 0x80041ADC: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80041AE0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80041AE4: sw          $s2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r18;
    // 0x80041AE8: jal         0x80041C54
    // 0x80041AEC: sw          $v1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r3;
    poolObject_complete(rdram, ctx);
        goto after_1;
    // 0x80041AEC: sw          $v1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r3;
    after_1:
L_80041AF0:
    // 0x80041AF0: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
L_80041AF4:
    // 0x80041AF4: bnel        $s0, $fp, L_80041AA8
    if (ctx->r16 != ctx->r30) {
        // 0x80041AF8: lh          $a3, 0x0($s0)
        ctx->r7 = MEM_H(ctx->r16, 0X0);
            goto L_80041AA8;
    }
    goto skip_9;
    // 0x80041AF8: lh          $a3, 0x0($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X0);
    skip_9:
L_80041AFC:
    // 0x80041AFC: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
L_80041B00:
    // 0x80041B00: bnel        $s4, $fp, L_80041A78
    if (ctx->r20 != ctx->r30) {
        // 0x80041B04: lh          $s3, 0x0($s4)
        ctx->r19 = MEM_H(ctx->r20, 0X0);
            goto L_80041A78;
    }
    goto skip_10;
    // 0x80041B04: lh          $s3, 0x0($s4)
    ctx->r19 = MEM_H(ctx->r20, 0X0);
    skip_10:
    // 0x80041B08: beq         $s6, $zero, L_80041B7C
    if (ctx->r22 == 0) {
        // 0x80041B0C: lh          $t6, 0x54($sp)
        ctx->r14 = MEM_H(ctx->r29, 0X54);
            goto L_80041B7C;
    }
    // 0x80041B0C: lh          $t6, 0x54($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X54);
    // 0x80041B10: andi        $t7, $t6, 0x600
    ctx->r15 = ctx->r14 & 0X600;
    // 0x80041B14: beq         $t7, $zero, L_80041B7C
    if (ctx->r15 == 0) {
        // 0x80041B18: lw          $s4, 0x60($sp)
        ctx->r20 = MEM_W(ctx->r29, 0X60);
            goto L_80041B7C;
    }
    // 0x80041B18: lw          $s4, 0x60($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X60);
    // 0x80041B1C: lh          $s3, 0x0($s4)
    ctx->r19 = MEM_H(ctx->r20, 0X0);
L_80041B20:
    // 0x80041B20: andi        $t8, $s3, 0x2000
    ctx->r24 = ctx->r19 & 0X2000;
    // 0x80041B24: beq         $t8, $zero, L_80041B70
    if (ctx->r24 == 0) {
        // 0x80041B28: andi        $t9, $s3, 0x100
        ctx->r25 = ctx->r19 & 0X100;
            goto L_80041B70;
    }
    // 0x80041B28: andi        $t9, $s3, 0x100
    ctx->r25 = ctx->r19 & 0X100;
    // 0x80041B2C: beql        $t9, $zero, L_80041B74
    if (ctx->r25 == 0) {
        // 0x80041B30: addu        $s4, $s4, $s7
        ctx->r20 = ADD32(ctx->r20, ctx->r23);
            goto L_80041B74;
    }
    goto skip_11;
    // 0x80041B30: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
    skip_11:
    // 0x80041B34: lhu         $s2, 0xA($s4)
    ctx->r18 = MEM_HU(ctx->r20, 0XA);
    // 0x80041B38: or          $s1, $s2, $zero
    ctx->r17 = ctx->r18 | 0;
    // 0x80041B3C: andi        $s1, $s1, 0x600
    ctx->r17 = ctx->r17 & 0X600;
    // 0x80041B40: beql        $s1, $zero, L_80041B74
    if (ctx->r17 == 0) {
        // 0x80041B44: addu        $s4, $s4, $s7
        ctx->r20 = ADD32(ctx->r20, ctx->r23);
            goto L_80041B74;
    }
    goto skip_12;
    // 0x80041B44: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
    skip_12:
    // 0x80041B48: beq         $s5, $s1, L_80041B70
    if (ctx->r21 == ctx->r17) {
        // 0x80041B4C: or          $a0, $s4, $zero
        ctx->r4 = ctx->r20 | 0;
            goto L_80041B70;
    }
    // 0x80041B4C: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80041B50: lhu         $t0, 0x4E($sp)
    ctx->r8 = MEM_HU(ctx->r29, 0X4E);
    // 0x80041B54: sll         $a2, $s3, 16
    ctx->r6 = S32(ctx->r19 << 16);
    // 0x80041B58: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80041B5C: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    // 0x80041B60: lh          $a3, 0x54($sp)
    ctx->r7 = MEM_H(ctx->r29, 0X54);
    // 0x80041B64: sw          $s2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r18;
    // 0x80041B68: jal         0x80041C54
    // 0x80041B6C: sw          $t0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r8;
    poolObject_complete(rdram, ctx);
        goto after_2;
    // 0x80041B6C: sw          $t0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r8;
    after_2:
L_80041B70:
    // 0x80041B70: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
L_80041B74:
    // 0x80041B74: bnel        $s4, $fp, L_80041B20
    if (ctx->r20 != ctx->r30) {
        // 0x80041B78: lh          $s3, 0x0($s4)
        ctx->r19 = MEM_H(ctx->r20, 0X0);
            goto L_80041B20;
    }
    goto skip_13;
    // 0x80041B78: lh          $s3, 0x0($s4)
    ctx->r19 = MEM_H(ctx->r20, 0X0);
    skip_13:
L_80041B7C:
    // 0x80041B7C: lw          $s4, 0x60($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X60);
    // 0x80041B80: addiu       $s6, $zero, 0x400
    ctx->r22 = ADD32(0, 0X400);
    // 0x80041B84: addiu       $s5, $zero, 0x200
    ctx->r21 = ADD32(0, 0X200);
    // 0x80041B88: lh          $s3, 0x0($s4)
    ctx->r19 = MEM_H(ctx->r20, 0X0);
L_80041B8C:
    // 0x80041B8C: andi        $t1, $s3, 0x2000
    ctx->r9 = ctx->r19 & 0X2000;
    // 0x80041B90: beq         $t1, $zero, L_80041C18
    if (ctx->r9 == 0) {
        // 0x80041B94: andi        $t2, $s3, 0x100
        ctx->r10 = ctx->r19 & 0X100;
            goto L_80041C18;
    }
    // 0x80041B94: andi        $t2, $s3, 0x100
    ctx->r10 = ctx->r19 & 0X100;
    // 0x80041B98: beql        $t2, $zero, L_80041C1C
    if (ctx->r10 == 0) {
        // 0x80041B9C: addu        $s4, $s4, $s7
        ctx->r20 = ADD32(ctx->r20, ctx->r23);
            goto L_80041C1C;
    }
    goto skip_14;
    // 0x80041B9C: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
    skip_14:
    // 0x80041BA0: lhu         $s2, 0xA($s4)
    ctx->r18 = MEM_HU(ctx->r20, 0XA);
    // 0x80041BA4: lw          $s0, 0x60($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X60);
    // 0x80041BA8: or          $s1, $s2, $zero
    ctx->r17 = ctx->r18 | 0;
    // 0x80041BAC: andi        $s1, $s1, 0x600
    ctx->r17 = ctx->r17 & 0X600;
    // 0x80041BB0: beq         $s5, $s1, L_80041BC0
    if (ctx->r21 == ctx->r17) {
        // 0x80041BB4: nop
    
            goto L_80041BC0;
    }
    // 0x80041BB4: nop

    // 0x80041BB8: bnel        $s6, $s1, L_80041C1C
    if (ctx->r22 != ctx->r17) {
        // 0x80041BBC: addu        $s4, $s4, $s7
        ctx->r20 = ADD32(ctx->r20, ctx->r23);
            goto L_80041C1C;
    }
    goto skip_15;
    // 0x80041BBC: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
    skip_15:
L_80041BC0:
    // 0x80041BC0: lh          $a3, 0x0($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X0);
L_80041BC4:
    // 0x80041BC4: andi        $t3, $a3, 0x2000
    ctx->r11 = ctx->r7 & 0X2000;
    // 0x80041BC8: beq         $t3, $zero, L_80041C0C
    if (ctx->r11 == 0) {
        // 0x80041BCC: andi        $t4, $a3, 0x600
        ctx->r12 = ctx->r7 & 0X600;
            goto L_80041C0C;
    }
    // 0x80041BCC: andi        $t4, $a3, 0x600
    ctx->r12 = ctx->r7 & 0X600;
    // 0x80041BD0: beql        $t4, $zero, L_80041C10
    if (ctx->r12 == 0) {
        // 0x80041BD4: addu        $s0, $s0, $s7
        ctx->r16 = ADD32(ctx->r16, ctx->r23);
            goto L_80041C10;
    }
    goto skip_16;
    // 0x80041BD4: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
    skip_16:
    // 0x80041BD8: lhu         $v1, 0xA($s0)
    ctx->r3 = MEM_HU(ctx->r16, 0XA);
    // 0x80041BDC: andi        $t6, $v1, 0x600
    ctx->r14 = ctx->r3 & 0X600;
    // 0x80041BE0: beql        $t6, $s1, L_80041C10
    if (ctx->r14 == ctx->r17) {
        // 0x80041BE4: addu        $s0, $s0, $s7
        ctx->r16 = ADD32(ctx->r16, ctx->r23);
            goto L_80041C10;
    }
    goto skip_17;
    // 0x80041BE4: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
    skip_17:
    // 0x80041BE8: beq         $s5, $s1, L_80041BF4
    if (ctx->r21 == ctx->r17) {
        // 0x80041BEC: or          $a0, $s4, $zero
        ctx->r4 = ctx->r20 | 0;
            goto L_80041BF4;
    }
    // 0x80041BEC: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80041BF0: bne         $s6, $s1, L_80041C0C
    if (ctx->r22 != ctx->r17) {
        // 0x80041BF4: sll         $a2, $s3, 16
        ctx->r6 = S32(ctx->r19 << 16);
            goto L_80041C0C;
    }
L_80041BF4:
    // 0x80041BF4: sll         $a2, $s3, 16
    ctx->r6 = S32(ctx->r19 << 16);
    // 0x80041BF8: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80041BFC: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80041C00: sw          $s2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r18;
    // 0x80041C04: jal         0x80041C54
    // 0x80041C08: sw          $v1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r3;
    poolObject_complete(rdram, ctx);
        goto after_3;
    // 0x80041C08: sw          $v1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r3;
    after_3:
L_80041C0C:
    // 0x80041C0C: addu        $s0, $s0, $s7
    ctx->r16 = ADD32(ctx->r16, ctx->r23);
L_80041C10:
    // 0x80041C10: bnel        $s0, $fp, L_80041BC4
    if (ctx->r16 != ctx->r30) {
        // 0x80041C14: lh          $a3, 0x0($s0)
        ctx->r7 = MEM_H(ctx->r16, 0X0);
            goto L_80041BC4;
    }
    goto skip_18;
    // 0x80041C14: lh          $a3, 0x0($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X0);
    skip_18:
L_80041C18:
    // 0x80041C18: addu        $s4, $s4, $s7
    ctx->r20 = ADD32(ctx->r20, ctx->r23);
L_80041C1C:
    // 0x80041C1C: bnel        $s4, $fp, L_80041B8C
    if (ctx->r20 != ctx->r30) {
        // 0x80041C20: lh          $s3, 0x0($s4)
        ctx->r19 = MEM_H(ctx->r20, 0X0);
            goto L_80041B8C;
    }
    goto skip_19;
    // 0x80041C20: lh          $s3, 0x0($s4)
    ctx->r19 = MEM_H(ctx->r20, 0X0);
    skip_19:
    // 0x80041C24: lw          $ra, 0x44($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X44);
    // 0x80041C28: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80041C2C: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80041C30: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x80041C34: lw          $s3, 0x2C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X2C);
    // 0x80041C38: lw          $s4, 0x30($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X30);
    // 0x80041C3C: lw          $s5, 0x34($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X34);
    // 0x80041C40: lw          $s6, 0x38($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X38);
    // 0x80041C44: lw          $s7, 0x3C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X3C);
    // 0x80041C48: lw          $fp, 0x40($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X40);
    // 0x80041C4C: jr          $ra
    // 0x80041C50: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x80041C50: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void poolObject_complete(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80041C54: addiu       $sp, $sp, -0x178
    ctx->r29 = ADD32(ctx->r29, -0X178);
    // 0x80041C58: sw          $a2, 0x180($sp)
    MEM_W(0X180, ctx->r29) = ctx->r6;
    // 0x80041C5C: lh          $t0, 0x182($sp)
    ctx->r8 = MEM_H(ctx->r29, 0X182);
    // 0x80041C60: sw          $s1, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->r17;
    // 0x80041C64: sw          $s0, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r16;
    // 0x80041C68: andi        $t0, $t0, 0x1000
    ctx->r8 = ctx->r8 & 0X1000;
    // 0x80041C6C: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80041C70: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80041C74: sw          $ra, 0xAC($sp)
    MEM_W(0XAC, ctx->r29) = ctx->r31;
    // 0x80041C78: sw          $fp, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = ctx->r30;
    // 0x80041C7C: sw          $s7, 0xA4($sp)
    MEM_W(0XA4, ctx->r29) = ctx->r23;
    // 0x80041C80: sw          $s6, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->r22;
    // 0x80041C84: sw          $s5, 0x9C($sp)
    MEM_W(0X9C, ctx->r29) = ctx->r21;
    // 0x80041C88: sw          $s4, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->r20;
    // 0x80041C8C: sw          $s3, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->r19;
    // 0x80041C90: sw          $s2, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r18;
    // 0x80041C94: sdc1        $f30, 0x80($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X80, ctx->r29);
    // 0x80041C98: sdc1        $f28, 0x78($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X78, ctx->r29);
    // 0x80041C9C: sdc1        $f26, 0x70($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X70, ctx->r29);
    // 0x80041CA0: sdc1        $f24, 0x68($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X68, ctx->r29);
    // 0x80041CA4: sdc1        $f22, 0x60($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X60, ctx->r29);
    // 0x80041CA8: sdc1        $f20, 0x58($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X58, ctx->r29);
    // 0x80041CAC: beq         $t0, $zero, L_80041CF0
    if (ctx->r8 == 0) {
        // 0x80041CB0: sw          $a3, 0x184($sp)
        MEM_W(0X184, ctx->r29) = ctx->r7;
            goto L_80041CF0;
    }
    // 0x80041CB0: sw          $a3, 0x184($sp)
    MEM_W(0X184, ctx->r29) = ctx->r7;
    // 0x80041CB4: sw          $a0, 0x168($sp)
    MEM_W(0X168, ctx->r29) = ctx->r4;
    // 0x80041CB8: lhu         $t6, 0x2($s1)
    ctx->r14 = MEM_HU(ctx->r17, 0X2);
    // 0x80041CBC: lw          $a1, 0x68($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X68);
    // 0x80041CC0: sh          $t6, 0x15A($sp)
    MEM_H(0X15A, ctx->r29) = ctx->r14;
    // 0x80041CC4: lh          $t7, 0x6($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X6);
    // 0x80041CC8: sh          $t7, 0x158($sp)
    MEM_H(0X158, ctx->r29) = ctx->r15;
    // 0x80041CCC: lhu         $t8, 0x4C($s1)
    ctx->r24 = MEM_HU(ctx->r17, 0X4C);
    // 0x80041CD0: sh          $t8, 0x156($sp)
    MEM_H(0X156, ctx->r29) = ctx->r24;
    // 0x80041CD4: lwc1        $f4, 0x7C($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X7C);
    // 0x80041CD8: swc1        $f4, 0x12C($sp)
    MEM_W(0X12C, ctx->r29) = ctx->f4.u32l;
    // 0x80041CDC: lwc1        $f6, 0x80($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X80);
    // 0x80041CE0: swc1        $f6, 0x128($sp)
    MEM_W(0X128, ctx->r29) = ctx->f6.u32l;
    // 0x80041CE4: lwc1        $f8, 0x84($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X84);
    // 0x80041CE8: b           L_80041D40
    // 0x80041CEC: swc1        $f8, 0x124($sp)
    MEM_W(0X124, ctx->r29) = ctx->f8.u32l;
        goto L_80041D40;
    // 0x80041CEC: swc1        $f8, 0x124($sp)
    MEM_W(0X124, ctx->r29) = ctx->f8.u32l;
L_80041CF0:
    // 0x80041CF0: lw          $v0, 0x60($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X60);
    // 0x80041CF4: beql        $v0, $zero, L_8004240C
    if (ctx->r2 == 0) {
        // 0x80041CF8: lw          $ra, 0xAC($sp)
        ctx->r31 = MEM_W(ctx->r29, 0XAC);
            goto L_8004240C;
    }
    goto skip_0;
    // 0x80041CF8: lw          $ra, 0xAC($sp)
    ctx->r31 = MEM_W(ctx->r29, 0XAC);
    skip_0:
    // 0x80041CFC: sw          $v0, 0x174($sp)
    MEM_W(0X174, ctx->r29) = ctx->r2;
    // 0x80041D00: lw          $a1, 0x2C($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X2C);
    // 0x80041D04: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80041D08: sw          $a1, 0x144($sp)
    MEM_W(0X144, ctx->r29) = ctx->r5;
    // 0x80041D0C: lhu         $t1, 0x2($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0X2);
    // 0x80041D10: sh          $t1, 0x15A($sp)
    MEM_H(0X15A, ctx->r29) = ctx->r9;
    // 0x80041D14: lh          $t2, 0x6($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X6);
    // 0x80041D18: sh          $t2, 0x158($sp)
    MEM_H(0X158, ctx->r29) = ctx->r10;
    // 0x80041D1C: lhu         $t3, 0xC($v0)
    ctx->r11 = MEM_HU(ctx->r2, 0XC);
    // 0x80041D20: sh          $t3, 0x156($sp)
    MEM_H(0X156, ctx->r29) = ctx->r11;
    // 0x80041D24: lwc1        $f10, 0x4C($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4C);
    // 0x80041D28: swc1        $f10, 0x12C($sp)
    MEM_W(0X12C, ctx->r29) = ctx->f10.u32l;
    // 0x80041D2C: lwc1        $f4, 0x50($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80041D30: swc1        $f4, 0x128($sp)
    MEM_W(0X128, ctx->r29) = ctx->f4.u32l;
    // 0x80041D34: lwc1        $f6, 0x54($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X54);
    // 0x80041D38: sw          $v0, 0x168($sp)
    MEM_W(0X168, ctx->r29) = ctx->r2;
    // 0x80041D3C: swc1        $f6, 0x124($sp)
    MEM_W(0X124, ctx->r29) = ctx->f6.u32l;
L_80041D40:
    // 0x80041D40: lh          $fp, 0x186($sp)
    ctx->r30 = MEM_H(ctx->r29, 0X186);
    // 0x80041D44: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
    // 0x80041D48: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80041D4C: andi        $fp, $fp, 0x1000
    ctx->r30 = ctx->r30 & 0X1000;
    // 0x80041D50: beq         $fp, $zero, L_80041D70
    if (ctx->r30 == 0) {
        // 0x80041D54: or          $s7, $zero, $zero
        ctx->r23 = 0 | 0;
            goto L_80041D70;
    }
    // 0x80041D54: or          $s7, $zero, $zero
    ctx->r23 = 0 | 0;
    // 0x80041D58: sw          $s0, 0x164($sp)
    MEM_W(0X164, ctx->r29) = ctx->r16;
    // 0x80041D5C: sw          $s0, 0x160($sp)
    MEM_W(0X160, ctx->r29) = ctx->r16;
    // 0x80041D60: lbu         $t4, 0x4A($s0)
    ctx->r12 = MEM_BU(ctx->r16, 0X4A);
    // 0x80041D64: sw          $t4, 0x140($sp)
    MEM_W(0X140, ctx->r29) = ctx->r12;
    // 0x80041D68: b           L_80041D90
    // 0x80041D6C: sw          $t4, 0x134($sp)
    MEM_W(0X134, ctx->r29) = ctx->r12;
        goto L_80041D90;
    // 0x80041D6C: sw          $t4, 0x134($sp)
    MEM_W(0X134, ctx->r29) = ctx->r12;
L_80041D70:
    // 0x80041D70: lw          $t6, 0x64($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X64);
    // 0x80041D74: sw          $t6, 0x164($sp)
    MEM_W(0X164, ctx->r29) = ctx->r14;
    // 0x80041D78: lw          $t7, 0x68($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X68);
    // 0x80041D7C: sw          $t7, 0x160($sp)
    MEM_W(0X160, ctx->r29) = ctx->r15;
    // 0x80041D80: lw          $t8, 0x4C($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4C);
    // 0x80041D84: sw          $t8, 0x140($sp)
    MEM_W(0X140, ctx->r29) = ctx->r24;
    // 0x80041D88: lw          $t9, 0x50($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X50);
    // 0x80041D8C: sw          $t9, 0x134($sp)
    MEM_W(0X134, ctx->r29) = ctx->r25;
L_80041D90:
    // 0x80041D90: sw          $s1, 0x178($sp)
    MEM_W(0X178, ctx->r29) = ctx->r17;
    // 0x80041D94: lui         $s1, 0x800F
    ctx->r17 = S32(0X800F << 16);
    // 0x80041D98: mtc1        $zero, $f28
    ctx->f28.u32l = 0;
    // 0x80041D9C: addiu       $s1, $s1, 0x41F0
    ctx->r17 = ADD32(ctx->r17, 0X41F0);
    // 0x80041DA0: sw          $t0, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->r8;
    // 0x80041DA4: sw          $s0, 0x17C($sp)
    MEM_W(0X17C, ctx->r29) = ctx->r16;
    // 0x80041DA8: lw          $s5, 0x15C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X15C);
    // 0x80041DAC: addiu       $t1, $v1, -0x1
    ctx->r9 = ADD32(ctx->r3, -0X1);
L_80041DB0:
    // 0x80041DB0: and         $t2, $t1, $s6
    ctx->r10 = ctx->r9 & ctx->r22;
    // 0x80041DB4: beq         $t2, $zero, L_80041DEC
    if (ctx->r10 == 0) {
        // 0x80041DB8: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80041DEC;
    }
    // 0x80041DB8: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80041DBC: lw          $s5, 0x168($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X168);
    // 0x80041DC0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80041DC4:
    // 0x80041DC4: and         $t3, $s6, $v0
    ctx->r11 = ctx->r22 & ctx->r2;
    // 0x80041DC8: beql        $t3, $zero, L_80041DE0
    if (ctx->r11 == 0) {
        // 0x80041DCC: sll         $v0, $v0, 1
        ctx->r2 = S32(ctx->r2 << 1);
            goto L_80041DE0;
    }
    goto skip_1;
    // 0x80041DCC: sll         $v0, $v0, 1
    ctx->r2 = S32(ctx->r2 << 1);
    skip_1:
    // 0x80041DD0: lw          $t4, 0x2C($s5)
    ctx->r12 = MEM_W(ctx->r21, 0X2C);
    // 0x80041DD4: beql        $a1, $t4, L_80041DF0
    if (ctx->r5 == ctx->r12) {
        // 0x80041DD8: and         $t6, $v0, $v1
        ctx->r14 = ctx->r2 & ctx->r3;
            goto L_80041DF0;
    }
    goto skip_2;
    // 0x80041DD8: and         $t6, $v0, $v1
    ctx->r14 = ctx->r2 & ctx->r3;
    skip_2:
    // 0x80041DDC: sll         $v0, $v0, 1
    ctx->r2 = S32(ctx->r2 << 1);
L_80041DE0:
    // 0x80041DE0: and         $t5, $v0, $v1
    ctx->r13 = ctx->r2 & ctx->r3;
    // 0x80041DE4: beq         $t5, $zero, L_80041DC4
    if (ctx->r13 == 0) {
        // 0x80041DE8: lw          $s5, 0x30($s5)
        ctx->r21 = MEM_W(ctx->r21, 0X30);
            goto L_80041DC4;
    }
    // 0x80041DE8: lw          $s5, 0x30($s5)
    ctx->r21 = MEM_W(ctx->r21, 0X30);
L_80041DEC:
    // 0x80041DEC: and         $t6, $v0, $v1
    ctx->r14 = ctx->r2 & ctx->r3;
L_80041DF0:
    // 0x80041DF0: sw          $v1, 0x148($sp)
    MEM_W(0X148, ctx->r29) = ctx->r3;
    // 0x80041DF4: sw          $a0, 0x174($sp)
    MEM_W(0X174, ctx->r29) = ctx->r4;
    // 0x80041DF8: beq         $t6, $zero, L_8004228C
    if (ctx->r14 == 0) {
        // 0x80041DFC: sw          $a1, 0x144($sp)
        MEM_W(0X144, ctx->r29) = ctx->r5;
            goto L_8004228C;
    }
    // 0x80041DFC: sw          $a1, 0x144($sp)
    MEM_W(0X144, ctx->r29) = ctx->r5;
    // 0x80041E00: lh          $s4, 0x0($a0)
    ctx->r20 = MEM_H(ctx->r4, 0X0);
    // 0x80041E04: sw          $v1, 0x148($sp)
    MEM_W(0X148, ctx->r29) = ctx->r3;
    // 0x80041E08: andi        $t7, $s4, 0x100
    ctx->r15 = ctx->r20 & 0X100;
    // 0x80041E0C: beq         $t7, $zero, L_8004228C
    if (ctx->r15 == 0) {
        // 0x80041E10: andi        $t8, $s4, 0x60
        ctx->r24 = ctx->r20 & 0X60;
            goto L_8004228C;
    }
    // 0x80041E10: andi        $t8, $s4, 0x60
    ctx->r24 = ctx->r20 & 0X60;
    // 0x80041E14: beq         $t8, $zero, L_8004228C
    if (ctx->r24 == 0) {
        // 0x80041E18: sw          $v1, 0x148($sp)
        MEM_W(0X148, ctx->r29) = ctx->r3;
            goto L_8004228C;
    }
    // 0x80041E18: sw          $v1, 0x148($sp)
    MEM_W(0X148, ctx->r29) = ctx->r3;
    // 0x80041E1C: lw          $t9, 0xC8($sp)
    ctx->r25 = MEM_W(ctx->r29, 0XC8);
    // 0x80041E20: lh          $s3, 0x186($sp)
    ctx->r19 = MEM_H(ctx->r29, 0X186);
    // 0x80041E24: sw          $zero, 0x130($sp)
    MEM_W(0X130, ctx->r29) = 0;
    // 0x80041E28: bne         $t9, $zero, L_80041E48
    if (ctx->r25 != 0) {
        // 0x80041E2C: andi        $s3, $s3, 0x400
        ctx->r19 = ctx->r19 & 0X400;
            goto L_80041E48;
    }
    // 0x80041E2C: andi        $s3, $s3, 0x400
    ctx->r19 = ctx->r19 & 0X400;
    // 0x80041E30: lwc1        $f8, 0x4C($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X4C);
    // 0x80041E34: swc1        $f8, 0x12C($sp)
    MEM_W(0X12C, ctx->r29) = ctx->f8.u32l;
    // 0x80041E38: lwc1        $f10, 0x50($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X50);
    // 0x80041E3C: swc1        $f10, 0x128($sp)
    MEM_W(0X128, ctx->r29) = ctx->f10.u32l;
    // 0x80041E40: lwc1        $f4, 0x54($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X54);
    // 0x80041E44: swc1        $f4, 0x124($sp)
    MEM_W(0X124, ctx->r29) = ctx->f4.u32l;
L_80041E48:
    // 0x80041E48: bne         $s7, $zero, L_8004206C
    if (ctx->r23 != 0) {
        // 0x80041E4C: lh          $t1, 0x186($sp)
        ctx->r9 = MEM_H(ctx->r29, 0X186);
            goto L_8004206C;
    }
    // 0x80041E4C: lh          $t1, 0x186($sp)
    ctx->r9 = MEM_H(ctx->r29, 0X186);
    // 0x80041E50: andi        $t2, $t1, 0x200
    ctx->r10 = ctx->r9 & 0X200;
    // 0x80041E54: beq         $t2, $zero, L_8004206C
    if (ctx->r10 == 0) {
        // 0x80041E58: lw          $t3, 0x164($sp)
        ctx->r11 = MEM_W(ctx->r29, 0X164);
            goto L_8004206C;
    }
    // 0x80041E58: lw          $t3, 0x164($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X164);
    // 0x80041E5C: beq         $t3, $zero, L_8004206C
    if (ctx->r11 == 0) {
        // 0x80041E60: or          $s0, $t3, $zero
        ctx->r16 = ctx->r11 | 0;
            goto L_8004206C;
    }
    // 0x80041E60: or          $s0, $t3, $zero
    ctx->r16 = ctx->r11 | 0;
    // 0x80041E64: addiu       $s2, $zero, 0x1
    ctx->r18 = ADD32(0, 0X1);
    // 0x80041E68: lh          $a2, 0x0($s0)
    ctx->r6 = MEM_H(ctx->r16, 0X0);
L_80041E6C:
    // 0x80041E6C: andi        $t4, $a2, 0x200
    ctx->r12 = ctx->r6 & 0X200;
    // 0x80041E70: beq         $t4, $zero, L_80042050
    if (ctx->r12 == 0) {
        // 0x80041E74: andi        $t5, $a2, 0x60
        ctx->r13 = ctx->r6 & 0X60;
            goto L_80042050;
    }
    // 0x80041E74: andi        $t5, $a2, 0x60
    ctx->r13 = ctx->r6 & 0X60;
    // 0x80041E78: beq         $t5, $zero, L_80042050
    if (ctx->r13 == 0) {
        // 0x80041E7C: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80042050;
    }
    // 0x80041E7C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80041E80: andi        $t8, $a2, 0x7
    ctx->r24 = ctx->r6 & 0X7;
    // 0x80041E84: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80041E88: subu        $t9, $t9, $t8
    ctx->r25 = SUB32(ctx->r25, ctx->r24);
    // 0x80041E8C: sll         $t2, $t9, 1
    ctx->r10 = S32(ctx->r25 << 1);
    // 0x80041E90: andi        $t5, $s4, 0x7
    ctx->r13 = ctx->r20 & 0X7;
    // 0x80041E94: addu        $t6, $t2, $t5
    ctx->r14 = ADD32(ctx->r10, ctx->r13);
    // 0x80041E98: sll         $t7, $t6, 16
    ctx->r15 = S32(ctx->r14 << 16);
    // 0x80041E9C: sra         $t8, $t7, 16
    ctx->r24 = S32(SIGNED(ctx->r15) >> 16);
    // 0x80041EA0: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x80041EA4: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x80041EA8: addu        $at, $at, $t9
    ctx->r1 = ADD32(ctx->r1, ctx->r25);
    // 0x80041EAC: lw          $t9, 0x5188($at)
    ctx->r25 = MEM_W(ctx->r1, 0X5188);
    // 0x80041EB0: sll         $a3, $s4, 16
    ctx->r7 = S32(ctx->r20 << 16);
    // 0x80041EB4: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x80041EB8: jalr        $t9
    // 0x80041EBC: lw          $a1, 0x174($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X174);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80041EBC: lw          $a1, 0x174($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X174);
    after_0:
    // 0x80041EC0: c.eq.s      $f0, $f28
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    c1cs = ctx->f0.fl == ctx->f28.fl;
    // 0x80041EC4: swc1        $f0, 0x114($sp)
    MEM_W(0X114, ctx->r29) = ctx->f0.u32l;
    // 0x80041EC8: lw          $t1, 0x148($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X148);
    // 0x80041ECC: bc1tl       L_80042054
    if (c1cs) {
        // 0x80041ED0: mtc1        $zero, $f28
        ctx->f28.u32l = 0;
            goto L_80042054;
    }
    goto skip_3;
    // 0x80041ED0: mtc1        $zero, $f28
    ctx->f28.u32l = 0;
    skip_3:
    // 0x80041ED4: beq         $fp, $zero, L_80041EE4
    if (ctx->r30 == 0) {
        // 0x80041ED8: or          $s6, $s6, $t1
        ctx->r22 = ctx->r22 | ctx->r9;
            goto L_80041EE4;
    }
    // 0x80041ED8: or          $s6, $s6, $t1
    ctx->r22 = ctx->r22 | ctx->r9;
    // 0x80041EDC: b           L_80041EE8
    // 0x80041EE0: lwc1        $f26, 0x94($s0)
    ctx->f26.u32l = MEM_W(ctx->r16, 0X94);
        goto L_80041EE8;
    // 0x80041EE0: lwc1        $f26, 0x94($s0)
    ctx->f26.u32l = MEM_W(ctx->r16, 0X94);
L_80041EE4:
    // 0x80041EE4: lwc1        $f26, 0x64($s0)
    ctx->f26.u32l = MEM_W(ctx->r16, 0X64);
L_80041EE8:
    // 0x80041EE8: lwc1        $f16, 0xF4($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0XF4);
    // 0x80041EEC: lwc1        $f20, 0xCC($s1)
    ctx->f20.u32l = MEM_W(ctx->r17, 0XCC);
    // 0x80041EF0: lwc1        $f22, 0xD0($s1)
    ctx->f22.u32l = MEM_W(ctx->r17, 0XD0);
    // 0x80041EF4: c.eq.s      $f16, $f28
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 28);
    c1cs = ctx->f16.fl == ctx->f28.fl;
    // 0x80041EF8: lwc1        $f24, 0xD4($s1)
    ctx->f24.u32l = MEM_W(ctx->r17, 0XD4);
    // 0x80041EFC: lwc1        $f8, 0x12C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X12C);
    // 0x80041F00: lwc1        $f10, 0x128($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X128);
    // 0x80041F04: bc1tl       L_80041F7C
    if (c1cs) {
        // 0x80041F08: sub.s       $f0, $f8, $f20
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f20.fl;
            goto L_80041F7C;
    }
    goto skip_4;
    // 0x80041F08: sub.s       $f0, $f8, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f20.fl;
    skip_4:
    // 0x80041F0C: lwc1        $f28, 0xE4($s1)
    ctx->f28.u32l = MEM_W(ctx->r17, 0XE4);
    // 0x80041F10: lwc1        $f30, 0xE8($s1)
    ctx->f30.u32l = MEM_W(ctx->r17, 0XE8);
    // 0x80041F14: lwc1        $f18, 0xEC($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0XEC);
    // 0x80041F18: mul.s       $f6, $f28, $f26
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f6.fl = MUL_S(ctx->f28.fl, ctx->f26.fl);
    // 0x80041F1C: div.s       $f8, $f6, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80041F20: mul.s       $f10, $f30, $f26
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f10.fl = MUL_S(ctx->f30.fl, ctx->f26.fl);
    // 0x80041F24: div.s       $f4, $f10, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = DIV_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80041F28: lwc1        $f10, 0x12C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X12C);
    // 0x80041F2C: mul.s       $f6, $f18, $f26
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f26.fl);
    // 0x80041F30: add.s       $f20, $f20, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f8.fl;
    // 0x80041F34: div.s       $f8, $f6, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80041F38: lwc1        $f6, 0x124($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X124);
    // 0x80041F3C: add.s       $f22, $f22, $f4
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f22.fl = ctx->f22.fl + ctx->f4.fl;
    // 0x80041F40: lwc1        $f4, 0x128($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X128);
    // 0x80041F44: sub.s       $f0, $f10, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f20.fl;
    // 0x80041F48: sub.s       $f2, $f4, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f22.fl;
    // 0x80041F4C: add.s       $f24, $f24, $f8
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f24.fl = ctx->f24.fl + ctx->f8.fl;
    // 0x80041F50: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80041F54: nop

    // 0x80041F58: mul.s       $f10, $f2, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80041F5C: sub.s       $f14, $f6, $f24
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f14.fl = ctx->f6.fl - ctx->f24.fl;
    // 0x80041F60: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80041F64: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80041F68: jal         0x800A01E0
    // 0x80041F6C: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80041F6C: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    after_1:
    // 0x80041F70: b           L_80041FE8
    // 0x80041F74: mov.s       $f26, $f0
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    ctx->f26.fl = ctx->f0.fl;
        goto L_80041FE8;
    // 0x80041F74: mov.s       $f26, $f0
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    ctx->f26.fl = ctx->f0.fl;
    // 0x80041F78: sub.s       $f0, $f8, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f8.fl - ctx->f20.fl;
L_80041F7C:
    // 0x80041F7C: lwc1        $f4, 0x124($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X124);
    // 0x80041F80: sub.s       $f30, $f10, $f22
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f30.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80041F84: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80041F88: mov.s       $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    ctx->f28.fl = ctx->f0.fl;
    // 0x80041F8C: sub.s       $f18, $f4, $f24
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f18.fl = ctx->f4.fl - ctx->f24.fl;
    // 0x80041F90: mul.s       $f8, $f30, $f30
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f30.fl); 
    ctx->f8.fl = MUL_S(ctx->f30.fl, ctx->f30.fl);
    // 0x80041F94: swc1        $f18, 0xE8($sp)
    MEM_W(0XE8, ctx->r29) = ctx->f18.u32l;
    // 0x80041F98: mul.s       $f4, $f18, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x80041F9C: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80041FA0: jal         0x800A01E0
    // 0x80041FA4: add.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f4.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_2;
    // 0x80041FA4: add.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f10.fl + ctx->f4.fl;
    after_2:
    // 0x80041FA8: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80041FAC: lwc1        $f18, 0xE8($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XE8);
    // 0x80041FB0: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x80041FB4: c.eq.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl == ctx->f6.fl;
    // 0x80041FB8: nop

    // 0x80041FBC: bc1t        L_80041FC8
    if (c1cs) {
        // 0x80041FC0: nop
    
            goto L_80041FC8;
    }
    // 0x80041FC0: nop

    // 0x80041FC4: div.s       $f2, $f26, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f26.fl, ctx->f0.fl);
L_80041FC8:
    // 0x80041FC8: mul.s       $f8, $f28, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f28.fl, ctx->f2.fl);
    // 0x80041FCC: sub.s       $f26, $f0, $f26
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f26.fl = ctx->f0.fl - ctx->f26.fl;
    // 0x80041FD0: mul.s       $f10, $f30, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f30.fl, ctx->f2.fl);
    // 0x80041FD4: nop

    // 0x80041FD8: mul.s       $f4, $f18, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f2.fl);
    // 0x80041FDC: add.s       $f20, $f20, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f8.fl;
    // 0x80041FE0: add.s       $f22, $f22, $f10
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f22.fl = ctx->f22.fl + ctx->f10.fl;
    // 0x80041FE4: add.s       $f24, $f24, $f4
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f24.fl = ctx->f24.fl + ctx->f4.fl;
L_80041FE8:
    // 0x80041FE8: beq         $s7, $zero, L_80042000
    if (ctx->r23 == 0) {
        // 0x80041FEC: lwc1        $f6, 0x104($sp)
        ctx->f6.u32l = MEM_W(ctx->r29, 0X104);
            goto L_80042000;
    }
    // 0x80041FEC: lwc1        $f6, 0x104($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X104);
    // 0x80041FF0: c.lt.s      $f26, $f6
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f26.fl < ctx->f6.fl;
    // 0x80041FF4: nop

    // 0x80041FF8: bc1fl       L_80042054
    if (!c1cs) {
        // 0x80041FFC: mtc1        $zero, $f28
        ctx->f28.u32l = 0;
            goto L_80042054;
    }
    goto skip_5;
    // 0x80041FFC: mtc1        $zero, $f28
    ctx->f28.u32l = 0;
    skip_5:
L_80042000:
    // 0x80042000: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x80042004: mov.s       $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    ctx->f2.fl = ctx->f22.fl;
    // 0x80042008: mov.s       $f12, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    ctx->f12.fl = ctx->f24.fl;
    // 0x8004200C: lwc1        $f20, 0xCC($s1)
    ctx->f20.u32l = MEM_W(ctx->r17, 0XCC);
    // 0x80042010: lwc1        $f22, 0xD0($s1)
    ctx->f22.u32l = MEM_W(ctx->r17, 0XD0);
    // 0x80042014: lwc1        $f24, 0xD4($s1)
    ctx->f24.u32l = MEM_W(ctx->r17, 0XD4);
    // 0x80042018: sub.s       $f8, $f0, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = ctx->f0.fl - ctx->f20.fl;
    // 0x8004201C: lwc1        $f6, 0x114($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X114);
    // 0x80042020: swc1        $f26, 0x104($sp)
    MEM_W(0X104, ctx->r29) = ctx->f26.u32l;
    // 0x80042024: sub.s       $f10, $f2, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = ctx->f2.fl - ctx->f22.fl;
    // 0x80042028: swc1        $f8, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->f8.u32l;
    // 0x8004202C: or          $s5, $s0, $zero
    ctx->r21 = ctx->r16 | 0;
    // 0x80042030: sub.s       $f4, $f12, $f24
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f4.fl = ctx->f12.fl - ctx->f24.fl;
    // 0x80042034: swc1        $f10, 0xF8($sp)
    MEM_W(0XF8, ctx->r29) = ctx->f10.u32l;
    // 0x80042038: swc1        $f12, 0x110($sp)
    MEM_W(0X110, ctx->r29) = ctx->f12.u32l;
    // 0x8004203C: swc1        $f2, 0x10C($sp)
    MEM_W(0X10C, ctx->r29) = ctx->f2.u32l;
    // 0x80042040: swc1        $f4, 0xFC($sp)
    MEM_W(0XFC, ctx->r29) = ctx->f4.u32l;
    // 0x80042044: swc1        $f0, 0x108($sp)
    MEM_W(0X108, ctx->r29) = ctx->f0.u32l;
    // 0x80042048: or          $s7, $s2, $zero
    ctx->r23 = ctx->r18 | 0;
    // 0x8004204C: swc1        $f6, 0x100($sp)
    MEM_W(0X100, ctx->r29) = ctx->f6.u32l;
L_80042050:
    // 0x80042050: mtc1        $zero, $f28
    ctx->f28.u32l = 0;
L_80042054:
    // 0x80042054: bne         $fp, $zero, L_80042064
    if (ctx->r30 != 0) {
        // 0x80042058: lw          $t3, 0x17C($sp)
        ctx->r11 = MEM_W(ctx->r29, 0X17C);
            goto L_80042064;
    }
    // 0x80042058: lw          $t3, 0x17C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X17C);
    // 0x8004205C: lw          $s0, 0x34($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X34);
    // 0x80042060: sll         $s2, $s2, 1
    ctx->r18 = S32(ctx->r18 << 1);
L_80042064:
    // 0x80042064: bnel        $s0, $t3, L_80041E6C
    if (ctx->r16 != ctx->r11) {
        // 0x80042068: lh          $a2, 0x0($s0)
        ctx->r6 = MEM_H(ctx->r16, 0X0);
            goto L_80041E6C;
    }
    goto skip_6;
    // 0x80042068: lh          $a2, 0x0($s0)
    ctx->r6 = MEM_H(ctx->r16, 0X0);
    skip_6:
L_8004206C:
    // 0x8004206C: beq         $s3, $zero, L_8004228C
    if (ctx->r19 == 0) {
        // 0x80042070: lw          $t4, 0x160($sp)
        ctx->r12 = MEM_W(ctx->r29, 0X160);
            goto L_8004228C;
    }
    // 0x80042070: lw          $t4, 0x160($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X160);
    // 0x80042074: beq         $t4, $zero, L_8004228C
    if (ctx->r12 == 0) {
        // 0x80042078: or          $s0, $t4, $zero
        ctx->r16 = ctx->r12 | 0;
            goto L_8004228C;
    }
    // 0x80042078: or          $s0, $t4, $zero
    ctx->r16 = ctx->r12 | 0;
    // 0x8004207C: addiu       $s2, $zero, 0x1
    ctx->r18 = ADD32(0, 0X1);
    // 0x80042080: lh          $a2, 0x0($s0)
    ctx->r6 = MEM_H(ctx->r16, 0X0);
L_80042084:
    // 0x80042084: andi        $t2, $a2, 0x400
    ctx->r10 = ctx->r6 & 0X400;
    // 0x80042088: beq         $t2, $zero, L_80042270
    if (ctx->r10 == 0) {
        // 0x8004208C: andi        $t5, $a2, 0x60
        ctx->r13 = ctx->r6 & 0X60;
            goto L_80042270;
    }
    // 0x8004208C: andi        $t5, $a2, 0x60
    ctx->r13 = ctx->r6 & 0X60;
    // 0x80042090: beq         $t5, $zero, L_80042270
    if (ctx->r13 == 0) {
        // 0x80042094: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80042270;
    }
    // 0x80042094: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80042098: andi        $t8, $a2, 0x7
    ctx->r24 = ctx->r6 & 0X7;
    // 0x8004209C: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x800420A0: subu        $t9, $t9, $t8
    ctx->r25 = SUB32(ctx->r25, ctx->r24);
    // 0x800420A4: sll         $t3, $t9, 1
    ctx->r11 = S32(ctx->r25 << 1);
    // 0x800420A8: andi        $t5, $s4, 0x7
    ctx->r13 = ctx->r20 & 0X7;
    // 0x800420AC: addu        $t6, $t3, $t5
    ctx->r14 = ADD32(ctx->r11, ctx->r13);
    // 0x800420B0: sll         $t7, $t6, 16
    ctx->r15 = S32(ctx->r14 << 16);
    // 0x800420B4: sra         $t8, $t7, 16
    ctx->r24 = S32(SIGNED(ctx->r15) >> 16);
    // 0x800420B8: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x800420BC: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x800420C0: addu        $at, $at, $t9
    ctx->r1 = ADD32(ctx->r1, ctx->r25);
    // 0x800420C4: lw          $t9, 0x5188($at)
    ctx->r25 = MEM_W(ctx->r1, 0X5188);
    // 0x800420C8: sll         $a3, $s4, 16
    ctx->r7 = S32(ctx->r20 << 16);
    // 0x800420CC: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x800420D0: jalr        $t9
    // 0x800420D4: lw          $a1, 0x174($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X174);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x800420D4: lw          $a1, 0x174($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X174);
    after_3:
    // 0x800420D8: c.eq.s      $f0, $f28
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 28);
    c1cs = ctx->f0.fl == ctx->f28.fl;
    // 0x800420DC: lw          $t4, 0x130($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X130);
    // 0x800420E0: swc1        $f0, 0x114($sp)
    MEM_W(0X114, ctx->r29) = ctx->f0.u32l;
    // 0x800420E4: lw          $t1, 0x148($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X148);
    // 0x800420E8: bc1t        L_80042270
    if (c1cs) {
        // 0x800420EC: or          $s3, $s7, $t4
        ctx->r19 = ctx->r23 | ctx->r12;
            goto L_80042270;
    }
    // 0x800420EC: or          $s3, $s7, $t4
    ctx->r19 = ctx->r23 | ctx->r12;
    // 0x800420F0: beq         $fp, $zero, L_80042100
    if (ctx->r30 == 0) {
        // 0x800420F4: or          $s6, $s6, $t1
        ctx->r22 = ctx->r22 | ctx->r9;
            goto L_80042100;
    }
    // 0x800420F4: or          $s6, $s6, $t1
    ctx->r22 = ctx->r22 | ctx->r9;
    // 0x800420F8: b           L_80042104
    // 0x800420FC: lwc1        $f26, 0x94($s0)
    ctx->f26.u32l = MEM_W(ctx->r16, 0X94);
        goto L_80042104;
    // 0x800420FC: lwc1        $f26, 0x94($s0)
    ctx->f26.u32l = MEM_W(ctx->r16, 0X94);
L_80042100:
    // 0x80042100: lwc1        $f26, 0x64($s0)
    ctx->f26.u32l = MEM_W(ctx->r16, 0X64);
L_80042104:
    // 0x80042104: lwc1        $f16, 0xF4($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0XF4);
    // 0x80042108: lwc1        $f20, 0xCC($s1)
    ctx->f20.u32l = MEM_W(ctx->r17, 0XCC);
    // 0x8004210C: lwc1        $f22, 0xD0($s1)
    ctx->f22.u32l = MEM_W(ctx->r17, 0XD0);
    // 0x80042110: c.eq.s      $f16, $f28
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 28);
    c1cs = ctx->f16.fl == ctx->f28.fl;
    // 0x80042114: lwc1        $f24, 0xD4($s1)
    ctx->f24.u32l = MEM_W(ctx->r17, 0XD4);
    // 0x80042118: lwc1        $f10, 0x12C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X12C);
    // 0x8004211C: lwc1        $f4, 0x128($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X128);
    // 0x80042120: bc1tl       L_80042198
    if (c1cs) {
        // 0x80042124: sub.s       $f0, $f10, $f20
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f20.fl;
            goto L_80042198;
    }
    goto skip_7;
    // 0x80042124: sub.s       $f0, $f10, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f20.fl;
    skip_7:
    // 0x80042128: lwc1        $f28, 0xE4($s1)
    ctx->f28.u32l = MEM_W(ctx->r17, 0XE4);
    // 0x8004212C: lwc1        $f30, 0xE8($s1)
    ctx->f30.u32l = MEM_W(ctx->r17, 0XE8);
    // 0x80042130: lwc1        $f18, 0xEC($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0XEC);
    // 0x80042134: mul.s       $f8, $f28, $f26
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f8.fl = MUL_S(ctx->f28.fl, ctx->f26.fl);
    // 0x80042138: div.s       $f10, $f8, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = DIV_S(ctx->f8.fl, ctx->f16.fl);
    // 0x8004213C: mul.s       $f4, $f30, $f26
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f4.fl = MUL_S(ctx->f30.fl, ctx->f26.fl);
    // 0x80042140: div.s       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = DIV_S(ctx->f4.fl, ctx->f16.fl);
    // 0x80042144: lwc1        $f4, 0x12C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X12C);
    // 0x80042148: mul.s       $f8, $f18, $f26
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f26.fl);
    // 0x8004214C: add.s       $f20, $f20, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f10.fl;
    // 0x80042150: div.s       $f10, $f8, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = DIV_S(ctx->f8.fl, ctx->f16.fl);
    // 0x80042154: lwc1        $f8, 0x124($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X124);
    // 0x80042158: add.s       $f22, $f22, $f6
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f22.fl = ctx->f22.fl + ctx->f6.fl;
    // 0x8004215C: lwc1        $f6, 0x128($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X128);
    // 0x80042160: sub.s       $f0, $f4, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f20.fl;
    // 0x80042164: sub.s       $f2, $f6, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = ctx->f6.fl - ctx->f22.fl;
    // 0x80042168: add.s       $f24, $f24, $f10
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f24.fl = ctx->f24.fl + ctx->f10.fl;
    // 0x8004216C: mul.s       $f10, $f0, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80042170: nop

    // 0x80042174: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80042178: sub.s       $f14, $f8, $f24
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f14.fl = ctx->f8.fl - ctx->f24.fl;
    // 0x8004217C: mul.s       $f8, $f14, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80042180: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80042184: jal         0x800A01E0
    // 0x80042188: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_4;
    // 0x80042188: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
    after_4:
    // 0x8004218C: b           L_80042204
    // 0x80042190: mov.s       $f26, $f0
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    ctx->f26.fl = ctx->f0.fl;
        goto L_80042204;
    // 0x80042190: mov.s       $f26, $f0
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    ctx->f26.fl = ctx->f0.fl;
    // 0x80042194: sub.s       $f0, $f10, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f20.fl;
L_80042198:
    // 0x80042198: lwc1        $f6, 0x124($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X124);
    // 0x8004219C: sub.s       $f30, $f4, $f22
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f30.fl = ctx->f4.fl - ctx->f22.fl;
    // 0x800421A0: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x800421A4: mov.s       $f28, $f0
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 0);
    ctx->f28.fl = ctx->f0.fl;
    // 0x800421A8: sub.s       $f18, $f6, $f24
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f18.fl = ctx->f6.fl - ctx->f24.fl;
    // 0x800421AC: mul.s       $f10, $f30, $f30
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f30.fl); 
    ctx->f10.fl = MUL_S(ctx->f30.fl, ctx->f30.fl);
    // 0x800421B0: swc1        $f18, 0xE8($sp)
    MEM_W(0XE8, ctx->r29) = ctx->f18.u32l;
    // 0x800421B4: mul.s       $f6, $f18, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x800421B8: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x800421BC: jal         0x800A01E0
    // 0x800421C0: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_5;
    // 0x800421C0: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    after_5:
    // 0x800421C4: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x800421C8: lwc1        $f18, 0xE8($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XE8);
    // 0x800421CC: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
    // 0x800421D0: c.eq.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl == ctx->f8.fl;
    // 0x800421D4: nop

    // 0x800421D8: bc1t        L_800421E4
    if (c1cs) {
        // 0x800421DC: nop
    
            goto L_800421E4;
    }
    // 0x800421DC: nop

    // 0x800421E0: div.s       $f2, $f26, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f26.fl, ctx->f0.fl);
L_800421E4:
    // 0x800421E4: mul.s       $f10, $f28, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 28);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f28.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f28.fl, ctx->f2.fl);
    // 0x800421E8: sub.s       $f26, $f0, $f26
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f26.fl = ctx->f0.fl - ctx->f26.fl;
    // 0x800421EC: mul.s       $f4, $f30, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 30);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f30.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f30.fl, ctx->f2.fl);
    // 0x800421F0: nop

    // 0x800421F4: mul.s       $f6, $f18, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f2.fl);
    // 0x800421F8: add.s       $f20, $f20, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f10.fl;
    // 0x800421FC: add.s       $f22, $f22, $f4
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f22.fl = ctx->f22.fl + ctx->f4.fl;
    // 0x80042200: add.s       $f24, $f24, $f6
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f24.fl = ctx->f24.fl + ctx->f6.fl;
L_80042204:
    // 0x80042204: beq         $s3, $zero, L_8004221C
    if (ctx->r19 == 0) {
        // 0x80042208: lwc1        $f8, 0x104($sp)
        ctx->f8.u32l = MEM_W(ctx->r29, 0X104);
            goto L_8004221C;
    }
    // 0x80042208: lwc1        $f8, 0x104($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X104);
    // 0x8004220C: c.le.s      $f26, $f8
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f26.fl <= ctx->f8.fl;
    // 0x80042210: nop

    // 0x80042214: bc1fl       L_80042274
    if (!c1cs) {
        // 0x80042218: mtc1        $zero, $f28
        ctx->f28.u32l = 0;
            goto L_80042274;
    }
    goto skip_8;
    // 0x80042218: mtc1        $zero, $f28
    ctx->f28.u32l = 0;
    skip_8:
L_8004221C:
    // 0x8004221C: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x80042220: mov.s       $f2, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    ctx->f2.fl = ctx->f22.fl;
    // 0x80042224: mov.s       $f12, $f24
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    ctx->f12.fl = ctx->f24.fl;
    // 0x80042228: lwc1        $f20, 0xCC($s1)
    ctx->f20.u32l = MEM_W(ctx->r17, 0XCC);
    // 0x8004222C: lwc1        $f22, 0xD0($s1)
    ctx->f22.u32l = MEM_W(ctx->r17, 0XD0);
    // 0x80042230: lwc1        $f24, 0xD4($s1)
    ctx->f24.u32l = MEM_W(ctx->r17, 0XD4);
    // 0x80042234: sub.s       $f10, $f0, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = ctx->f0.fl - ctx->f20.fl;
    // 0x80042238: lwc1        $f8, 0x114($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X114);
    // 0x8004223C: swc1        $f26, 0x104($sp)
    MEM_W(0X104, ctx->r29) = ctx->f26.u32l;
    // 0x80042240: sub.s       $f4, $f2, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = ctx->f2.fl - ctx->f22.fl;
    // 0x80042244: swc1        $f10, 0xF4($sp)
    MEM_W(0XF4, ctx->r29) = ctx->f10.u32l;
    // 0x80042248: or          $s5, $s0, $zero
    ctx->r21 = ctx->r16 | 0;
    // 0x8004224C: sub.s       $f6, $f12, $f24
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f6.fl = ctx->f12.fl - ctx->f24.fl;
    // 0x80042250: swc1        $f4, 0xF8($sp)
    MEM_W(0XF8, ctx->r29) = ctx->f4.u32l;
    // 0x80042254: sw          $s2, 0x130($sp)
    MEM_W(0X130, ctx->r29) = ctx->r18;
    // 0x80042258: swc1        $f12, 0x110($sp)
    MEM_W(0X110, ctx->r29) = ctx->f12.u32l;
    // 0x8004225C: swc1        $f6, 0xFC($sp)
    MEM_W(0XFC, ctx->r29) = ctx->f6.u32l;
    // 0x80042260: swc1        $f2, 0x10C($sp)
    MEM_W(0X10C, ctx->r29) = ctx->f2.u32l;
    // 0x80042264: swc1        $f0, 0x108($sp)
    MEM_W(0X108, ctx->r29) = ctx->f0.u32l;
    // 0x80042268: or          $s7, $zero, $zero
    ctx->r23 = 0 | 0;
    // 0x8004226C: swc1        $f8, 0x100($sp)
    MEM_W(0X100, ctx->r29) = ctx->f8.u32l;
L_80042270:
    // 0x80042270: mtc1        $zero, $f28
    ctx->f28.u32l = 0;
L_80042274:
    // 0x80042274: bne         $fp, $zero, L_80042284
    if (ctx->r30 != 0) {
        // 0x80042278: lw          $t2, 0x17C($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X17C);
            goto L_80042284;
    }
    // 0x80042278: lw          $t2, 0x17C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X17C);
    // 0x8004227C: lw          $s0, 0x38($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X38);
    // 0x80042280: sll         $s2, $s2, 1
    ctx->r18 = S32(ctx->r18 << 1);
L_80042284:
    // 0x80042284: bnel        $s0, $t2, L_80042084
    if (ctx->r16 != ctx->r10) {
        // 0x80042288: lh          $a2, 0x0($s0)
        ctx->r6 = MEM_H(ctx->r16, 0X0);
            goto L_80042084;
    }
    goto skip_9;
    // 0x80042288: lh          $a2, 0x0($s0)
    ctx->r6 = MEM_H(ctx->r16, 0X0);
    skip_9:
L_8004228C:
    // 0x8004228C: lw          $v1, 0x148($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X148);
    // 0x80042290: lw          $t3, 0x174($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X174);
    // 0x80042294: lw          $a1, 0x144($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X144);
    // 0x80042298: and         $t5, $s6, $v1
    ctx->r13 = ctx->r22 & ctx->r3;
    // 0x8004229C: beq         $t5, $zero, L_8004234C
    if (ctx->r13 == 0) {
        // 0x800422A0: or          $a0, $t3, $zero
        ctx->r4 = ctx->r11 | 0;
            goto L_8004234C;
    }
    // 0x800422A0: or          $a0, $t3, $zero
    ctx->r4 = ctx->r11 | 0;
    // 0x800422A4: lhu         $v0, 0x18E($sp)
    ctx->r2 = MEM_HU(ctx->r29, 0X18E);
    // 0x800422A8: lh          $t9, 0x182($sp)
    ctx->r25 = MEM_H(ctx->r29, 0X182);
    // 0x800422AC: lhu         $t1, 0x18A($sp)
    ctx->r9 = MEM_HU(ctx->r29, 0X18A);
    // 0x800422B0: sw          $t3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r11;
    // 0x800422B4: sra         $t3, $v0, 9
    ctx->r11 = S32(SIGNED(ctx->r2) >> 9);
    // 0x800422B8: sw          $t9, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r25;
    // 0x800422BC: andi        $t9, $t3, 0x3
    ctx->r25 = ctx->r11 & 0X3;
    // 0x800422C0: sw          $t1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r9;
    // 0x800422C4: lw          $t6, 0x17C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X17C);
    // 0x800422C8: lh          $t7, 0x186($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X186);
    // 0x800422CC: sll         $t1, $t9, 2
    ctx->r9 = S32(ctx->r25 << 2);
    // 0x800422D0: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x800422D4: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x800422D8: lw          $t9, 0x5218($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X5218);
    // 0x800422DC: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x800422E0: sw          $t7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r15;
    // 0x800422E4: lh          $t7, 0x156($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X156);
    // 0x800422E8: lh          $t6, 0x158($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X158);
    // 0x800422EC: lw          $t8, 0x178($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X178);
    // 0x800422F0: lwc1        $f10, 0x104($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X104);
    // 0x800422F4: lwc1        $f4, 0x100($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X100);
    // 0x800422F8: lhu         $t5, 0x15A($sp)
    ctx->r13 = MEM_HU(ctx->r29, 0X15A);
    // 0x800422FC: addiu       $t4, $sp, 0x108
    ctx->r12 = ADD32(ctx->r29, 0X108);
    // 0x80042300: addiu       $t2, $sp, 0xF4
    ctx->r10 = ADD32(ctx->r29, 0XF4);
    // 0x80042304: sw          $t2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r10;
    // 0x80042308: sw          $t4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r12;
    // 0x8004230C: addiu       $a0, $sp, 0x140
    ctx->r4 = ADD32(ctx->r29, 0X140);
    // 0x80042310: or          $a1, $s7, $zero
    ctx->r5 = ctx->r23 | 0;
    // 0x80042314: addiu       $a2, $sp, 0x134
    ctx->r6 = ADD32(ctx->r29, 0X134);
    // 0x80042318: lw          $a3, 0x130($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X130);
    // 0x8004231C: sw          $s5, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r21;
    // 0x80042320: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80042324: sw          $t7, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r15;
    // 0x80042328: sw          $t6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r14;
    // 0x8004232C: sw          $t8, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r24;
    // 0x80042330: swc1        $f10, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f10.u32l;
    // 0x80042334: swc1        $f4, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f4.u32l;
    // 0x80042338: jalr        $t9
    // 0x8004233C: sw          $t5, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r13;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x8004233C: sw          $t5, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r13;
    after_6:
    // 0x80042340: lw          $a1, 0x144($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X144);
    // 0x80042344: lw          $a0, 0x174($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X174);
    // 0x80042348: lw          $v1, 0x148($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X148);
L_8004234C:
    // 0x8004234C: lw          $t4, 0xC8($sp)
    ctx->r12 = MEM_W(ctx->r29, 0XC8);
    // 0x80042350: bnel        $t4, $zero, L_80042398
    if (ctx->r12 != 0) {
        // 0x80042354: lw          $t7, 0x178($sp)
        ctx->r15 = MEM_W(ctx->r29, 0X178);
            goto L_80042398;
    }
    goto skip_10;
    // 0x80042354: lw          $t7, 0x178($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X178);
    skip_10:
    // 0x80042358: lw          $a0, 0x30($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X30);
    // 0x8004235C: sll         $v1, $v1, 1
    ctx->r3 = S32(ctx->r3 << 1);
    // 0x80042360: lhu         $t2, 0x2($a0)
    ctx->r10 = MEM_HU(ctx->r4, 0X2);
    // 0x80042364: lw          $a1, 0x2C($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X2C);
    // 0x80042368: sh          $t2, 0x15A($sp)
    MEM_H(0X15A, ctx->r29) = ctx->r10;
    // 0x8004236C: lh          $t5, 0x6($a0)
    ctx->r13 = MEM_H(ctx->r4, 0X6);
    // 0x80042370: sh          $t5, 0x158($sp)
    MEM_H(0X158, ctx->r29) = ctx->r13;
    // 0x80042374: lhu         $t6, 0xC($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0XC);
    // 0x80042378: sh          $t6, 0x156($sp)
    MEM_H(0X156, ctx->r29) = ctx->r14;
    // 0x8004237C: lwc1        $f6, 0x4C($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X4C);
    // 0x80042380: swc1        $f6, 0x12C($sp)
    MEM_W(0X12C, ctx->r29) = ctx->f6.u32l;
    // 0x80042384: lwc1        $f8, 0x50($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X50);
    // 0x80042388: swc1        $f8, 0x128($sp)
    MEM_W(0X128, ctx->r29) = ctx->f8.u32l;
    // 0x8004238C: lwc1        $f10, 0x54($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X54);
    // 0x80042390: swc1        $f10, 0x124($sp)
    MEM_W(0X124, ctx->r29) = ctx->f10.u32l;
    // 0x80042394: lw          $t7, 0x178($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X178);
L_80042398:
    // 0x80042398: bnel        $a0, $t7, L_80041DB0
    if (ctx->r4 != ctx->r15) {
        // 0x8004239C: addiu       $t1, $v1, -0x1
        ctx->r9 = ADD32(ctx->r3, -0X1);
            goto L_80041DB0;
    }
    goto skip_11;
    // 0x8004239C: addiu       $t1, $v1, -0x1
    ctx->r9 = ADD32(ctx->r3, -0X1);
    skip_11:
    // 0x800423A0: lw          $t8, 0xC8($sp)
    ctx->r24 = MEM_W(ctx->r29, 0XC8);
    // 0x800423A4: sw          $s5, 0x15C($sp)
    MEM_W(0X15C, ctx->r29) = ctx->r21;
    // 0x800423A8: lw          $t9, 0x178($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X178);
    // 0x800423AC: beql        $t8, $zero, L_800423C8
    if (ctx->r24 == 0) {
        // 0x800423B0: lw          $t4, 0x54($t9)
        ctx->r12 = MEM_W(ctx->r25, 0X54);
            goto L_800423C8;
    }
    goto skip_12;
    // 0x800423B0: lw          $t4, 0x54($t9)
    ctx->r12 = MEM_W(ctx->r25, 0X54);
    skip_12:
    // 0x800423B4: lbu         $t3, 0x4B($t7)
    ctx->r11 = MEM_BU(ctx->r15, 0X4B);
    // 0x800423B8: or          $t1, $t3, $s6
    ctx->r9 = ctx->r11 | ctx->r22;
    // 0x800423BC: b           L_800423D0
    // 0x800423C0: sb          $t1, 0x4B($t7)
    MEM_B(0X4B, ctx->r15) = ctx->r9;
        goto L_800423D0;
    // 0x800423C0: sb          $t1, 0x4B($t7)
    MEM_B(0X4B, ctx->r15) = ctx->r9;
    // 0x800423C4: lw          $t4, 0x54($t9)
    ctx->r12 = MEM_W(ctx->r25, 0X54);
L_800423C8:
    // 0x800423C8: or          $t2, $t4, $s6
    ctx->r10 = ctx->r12 | ctx->r22;
    // 0x800423CC: sw          $t2, 0x54($t9)
    MEM_W(0X54, ctx->r25) = ctx->r10;
L_800423D0:
    // 0x800423D0: beq         $fp, $zero, L_800423F4
    if (ctx->r30 == 0) {
        // 0x800423D4: lw          $t1, 0x140($sp)
        ctx->r9 = MEM_W(ctx->r29, 0X140);
            goto L_800423F4;
    }
    // 0x800423D4: lw          $t1, 0x140($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X140);
    // 0x800423D8: lw          $t5, 0x140($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X140);
    // 0x800423DC: lw          $t6, 0x134($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X134);
    // 0x800423E0: lw          $t3, 0x17C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X17C);
    // 0x800423E4: or          $t8, $t5, $t6
    ctx->r24 = ctx->r13 | ctx->r14;
    // 0x800423E8: sw          $t8, 0x140($sp)
    MEM_W(0X140, ctx->r29) = ctx->r24;
    // 0x800423EC: b           L_80042408
    // 0x800423F0: sb          $t8, 0x4A($t3)
    MEM_B(0X4A, ctx->r11) = ctx->r24;
        goto L_80042408;
    // 0x800423F0: sb          $t8, 0x4A($t3)
    MEM_B(0X4A, ctx->r11) = ctx->r24;
L_800423F4:
    // 0x800423F4: lw          $t7, 0x17C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X17C);
    // 0x800423F8: sw          $t1, 0x4C($t7)
    MEM_W(0X4C, ctx->r15) = ctx->r9;
    // 0x800423FC: lw          $t2, 0x17C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X17C);
    // 0x80042400: lw          $t4, 0x134($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X134);
    // 0x80042404: sw          $t4, 0x50($t2)
    MEM_W(0X50, ctx->r10) = ctx->r12;
L_80042408:
    // 0x80042408: lw          $ra, 0xAC($sp)
    ctx->r31 = MEM_W(ctx->r29, 0XAC);
L_8004240C:
    // 0x8004240C: ldc1        $f20, 0x58($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X58);
    // 0x80042410: ldc1        $f22, 0x60($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X60);
    // 0x80042414: ldc1        $f24, 0x68($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X68);
    // 0x80042418: ldc1        $f26, 0x70($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X70);
    // 0x8004241C: ldc1        $f28, 0x78($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X78);
    // 0x80042420: ldc1        $f30, 0x80($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X80);
    // 0x80042424: lw          $s0, 0x88($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X88);
    // 0x80042428: lw          $s1, 0x8C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X8C);
    // 0x8004242C: lw          $s2, 0x90($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X90);
    // 0x80042430: lw          $s3, 0x94($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X94);
    // 0x80042434: lw          $s4, 0x98($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X98);
    // 0x80042438: lw          $s5, 0x9C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X9C);
    // 0x8004243C: lw          $s6, 0xA0($sp)
    ctx->r22 = MEM_W(ctx->r29, 0XA0);
    // 0x80042440: lw          $s7, 0xA4($sp)
    ctx->r23 = MEM_W(ctx->r29, 0XA4);
    // 0x80042444: lw          $fp, 0xA8($sp)
    ctx->r30 = MEM_W(ctx->r29, 0XA8);
    // 0x80042448: jr          $ra
    // 0x8004244C: addiu       $sp, $sp, 0x178
    ctx->r29 = ADD32(ctx->r29, 0X178);
    return;
    // 0x8004244C: addiu       $sp, $sp, 0x178
    ctx->r29 = ADD32(ctx->r29, 0X178);
;}
RECOMP_FUNC void func_80042450(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80042450: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80042454: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80042458: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8004245C: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    // 0x80042460: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x80042464: sw          $a3, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r7;
    // 0x80042468: jal         0x80042CD4
    // 0x8004246C: lhu         $a0, 0x6E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X6E);
    func_80042CD4(rdram, ctx);
        goto after_0;
    // 0x8004246C: lhu         $a0, 0x6E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X6E);
    after_0:
    // 0x80042470: lh          $v1, 0x5A($sp)
    ctx->r3 = MEM_H(ctx->r29, 0X5A);
    // 0x80042474: lw          $t0, 0x40($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X40);
    // 0x80042478: sh          $v0, 0x3E($sp)
    MEM_H(0X3E, ctx->r29) = ctx->r2;
    // 0x8004247C: andi        $v1, $v1, 0x1000
    ctx->r3 = ctx->r3 & 0X1000;
    // 0x80042480: bne         $v1, $zero, L_80042494
    if (ctx->r3 != 0) {
        // 0x80042484: lw          $t6, 0x54($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X54);
            goto L_80042494;
    }
    // 0x80042484: lw          $t6, 0x54($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X54);
    // 0x80042488: lhu         $t7, 0x90($t6)
    ctx->r15 = MEM_HU(ctx->r14, 0X90);
    // 0x8004248C: or          $t8, $t7, $v0
    ctx->r24 = ctx->r15 | ctx->r2;
    // 0x80042490: sh          $t8, 0x90($t6)
    MEM_H(0X90, ctx->r14) = ctx->r24;
L_80042494:
    // 0x80042494: lw          $v0, 0x4C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X4C);
    // 0x80042498: lw          $t9, 0x48($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X48);
    // 0x8004249C: beq         $v0, $zero, L_80042568
    if (ctx->r2 == 0) {
        // 0x800424A0: nop
    
            goto L_80042568;
    }
    // 0x800424A0: nop

    // 0x800424A4: lw          $t1, 0x0($t9)
    ctx->r9 = MEM_W(ctx->r25, 0X0);
    // 0x800424A8: and         $t2, $t1, $v0
    ctx->r10 = ctx->r9 & ctx->r2;
    // 0x800424AC: bnel        $t2, $zero, L_800426C4
    if (ctx->r10 != 0) {
        // 0x800424B0: lw          $ra, 0x34($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X34);
            goto L_800426C4;
    }
    goto skip_0;
    // 0x800424B0: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    skip_0:
    // 0x800424B4: bne         $v1, $zero, L_80042508
    if (ctx->r3 != 0) {
        // 0x800424B8: lw          $a0, 0x50($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X50);
            goto L_80042508;
    }
    // 0x800424B8: lw          $a0, 0x50($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X50);
    // 0x800424BC: lw          $t3, 0x74($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X74);
    // 0x800424C0: lwc1        $f4, 0x78($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X78);
    // 0x800424C4: lwc1        $f6, 0x7C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X7C);
    // 0x800424C8: lhu         $t4, 0x82($sp)
    ctx->r12 = MEM_HU(ctx->r29, 0X82);
    // 0x800424CC: lh          $t5, 0x86($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X86);
    // 0x800424D0: lh          $t7, 0x8A($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X8A);
    // 0x800424D4: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X54);
    // 0x800424D8: lh          $a1, 0x5A($sp)
    ctx->r5 = MEM_H(ctx->r29, 0X5A);
    // 0x800424DC: lw          $a2, 0x64($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X64);
    // 0x800424E0: lw          $a3, 0x70($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X70);
    // 0x800424E4: sw          $t3, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r11;
    // 0x800424E8: swc1        $f4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f4.u32l;
    // 0x800424EC: swc1        $f6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f6.u32l;
    // 0x800424F0: sw          $t4, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r12;
    // 0x800424F4: sw          $t5, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r13;
    // 0x800424F8: jal         0x800429FC
    // 0x800424FC: sw          $t7, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r15;
    func_800429FC(rdram, ctx);
        goto after_1;
    // 0x800424FC: sw          $t7, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r15;
    after_1:
    // 0x80042500: b           L_80042554
    // 0x80042504: lw          $t3, 0x48($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X48);
        goto L_80042554;
    // 0x80042504: lw          $t3, 0x48($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X48);
L_80042508:
    // 0x80042508: lw          $t8, 0x70($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X70);
    // 0x8004250C: lw          $t6, 0x74($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X74);
    // 0x80042510: lwc1        $f8, 0x78($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X78);
    // 0x80042514: lwc1        $f10, 0x7C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X7C);
    // 0x80042518: lhu         $t9, 0x82($sp)
    ctx->r25 = MEM_HU(ctx->r29, 0X82);
    // 0x8004251C: lh          $t1, 0x86($sp)
    ctx->r9 = MEM_H(ctx->r29, 0X86);
    // 0x80042520: lh          $t2, 0x8A($sp)
    ctx->r10 = MEM_H(ctx->r29, 0X8A);
    // 0x80042524: lw          $a1, 0x54($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X54);
    // 0x80042528: lh          $a2, 0x5A($sp)
    ctx->r6 = MEM_H(ctx->r29, 0X5A);
    // 0x8004252C: lw          $a3, 0x64($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X64);
    // 0x80042530: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    // 0x80042534: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x80042538: swc1        $f8, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f8.u32l;
    // 0x8004253C: swc1        $f10, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f10.u32l;
    // 0x80042540: sw          $t9, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r25;
    // 0x80042544: sw          $t1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r9;
    // 0x80042548: jal         0x80042AC8
    // 0x8004254C: sw          $t2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r10;
    func_80042AC8(rdram, ctx);
        goto after_2;
    // 0x8004254C: sw          $t2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r10;
    after_2:
    // 0x80042550: lw          $t3, 0x48($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X48);
L_80042554:
    // 0x80042554: lw          $t5, 0x4C($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X4C);
    // 0x80042558: lw          $t4, 0x0($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X0);
    // 0x8004255C: or          $t7, $t4, $t5
    ctx->r15 = ctx->r12 | ctx->r13;
    // 0x80042560: b           L_800426C0
    // 0x80042564: sw          $t7, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r15;
        goto L_800426C0;
    // 0x80042564: sw          $t7, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r15;
L_80042568:
    // 0x80042568: bne         $v1, $zero, L_80042620
    if (ctx->r3 != 0) {
        // 0x8004256C: lw          $t6, 0x44($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X44);
            goto L_80042620;
    }
    // 0x8004256C: lw          $t6, 0x44($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X44);
    // 0x80042570: lw          $t8, 0x0($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X0);
    // 0x80042574: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X54);
    // 0x80042578: lh          $a1, 0x5A($sp)
    ctx->r5 = MEM_H(ctx->r29, 0X5A);
    // 0x8004257C: and         $t9, $t8, $t6
    ctx->r25 = ctx->r24 & ctx->r14;
    // 0x80042580: bne         $t9, $zero, L_800425DC
    if (ctx->r25 != 0) {
        // 0x80042584: lw          $a2, 0x64($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X64);
            goto L_800425DC;
    }
    // 0x80042584: lw          $a2, 0x64($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X64);
    // 0x80042588: lw          $t1, 0x74($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X74);
    // 0x8004258C: lwc1        $f16, 0x78($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X78);
    // 0x80042590: lwc1        $f18, 0x7C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X7C);
    // 0x80042594: lhu         $t2, 0x82($sp)
    ctx->r10 = MEM_HU(ctx->r29, 0X82);
    // 0x80042598: lh          $t4, 0x86($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X86);
    // 0x8004259C: lh          $t5, 0x8A($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X8A);
    // 0x800425A0: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X54);
    // 0x800425A4: lh          $a1, 0x5A($sp)
    ctx->r5 = MEM_H(ctx->r29, 0X5A);
    // 0x800425A8: lw          $a2, 0x64($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X64);
    // 0x800425AC: lw          $a3, 0x70($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X70);
    // 0x800425B0: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x800425B4: sw          $t1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r9;
    // 0x800425B8: swc1        $f16, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f16.u32l;
    // 0x800425BC: swc1        $f18, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f18.u32l;
    // 0x800425C0: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    // 0x800425C4: sw          $t4, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r12;
    // 0x800425C8: jal         0x800429FC
    // 0x800425CC: sw          $t5, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r13;
    func_800429FC(rdram, ctx);
        goto after_3;
    // 0x800425CC: sw          $t5, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r13;
    after_3:
    // 0x800425D0: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x800425D4: b           L_80042620
    // 0x800425D8: lw          $t0, 0x40($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X40);
        goto L_80042620;
    // 0x800425D8: lw          $t0, 0x40($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X40);
L_800425DC:
    // 0x800425DC: lw          $t7, 0x74($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X74);
    // 0x800425E0: lwc1        $f4, 0x78($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X78);
    // 0x800425E4: lwc1        $f6, 0x7C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X7C);
    // 0x800425E8: lhu         $t3, 0x82($sp)
    ctx->r11 = MEM_HU(ctx->r29, 0X82);
    // 0x800425EC: lh          $t8, 0x86($sp)
    ctx->r24 = MEM_H(ctx->r29, 0X86);
    // 0x800425F0: lh          $t6, 0x8A($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X8A);
    // 0x800425F4: lw          $a3, 0x70($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X70);
    // 0x800425F8: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x800425FC: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    // 0x80042600: swc1        $f4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f4.u32l;
    // 0x80042604: swc1        $f6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f6.u32l;
    // 0x80042608: sw          $t3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r11;
    // 0x8004260C: sw          $t8, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r24;
    // 0x80042610: jal         0x80042A94
    // 0x80042614: sw          $t6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r14;
    func_80042A94(rdram, ctx);
        goto after_4;
    // 0x80042614: sw          $t6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r14;
    after_4:
    // 0x80042618: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x8004261C: lw          $t0, 0x40($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X40);
L_80042620:
    // 0x80042620: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x80042624: lw          $a0, 0x50($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X50);
    // 0x80042628: lw          $a1, 0x54($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X54);
    // 0x8004262C: bne         $v0, $zero, L_800426B4
    if (ctx->r2 != 0) {
        // 0x80042630: lh          $a2, 0x5A($sp)
        ctx->r6 = MEM_H(ctx->r29, 0X5A);
            goto L_800426B4;
    }
    // 0x80042630: lh          $a2, 0x5A($sp)
    ctx->r6 = MEM_H(ctx->r29, 0X5A);
    // 0x80042634: lw          $t9, 0x70($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X70);
    // 0x80042638: lw          $t1, 0x74($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X74);
    // 0x8004263C: lwc1        $f8, 0x78($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X78);
    // 0x80042640: lwc1        $f10, 0x7C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X7C);
    // 0x80042644: lhu         $t2, 0x82($sp)
    ctx->r10 = MEM_HU(ctx->r29, 0X82);
    // 0x80042648: lh          $t4, 0x86($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X86);
    // 0x8004264C: lh          $t5, 0x8A($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X8A);
    // 0x80042650: lw          $a3, 0x64($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X64);
    // 0x80042654: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x80042658: sw          $t9, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r25;
    // 0x8004265C: sw          $t1, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r9;
    // 0x80042660: swc1        $f8, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f8.u32l;
    // 0x80042664: swc1        $f10, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f10.u32l;
    // 0x80042668: sw          $t2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r10;
    // 0x8004266C: sw          $t4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r12;
    // 0x80042670: jal         0x80042AC8
    // 0x80042674: sw          $t5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r13;
    func_80042AC8(rdram, ctx);
        goto after_5;
    // 0x80042674: sw          $t5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r13;
    after_5:
    // 0x80042678: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x8004267C: lw          $a0, 0x50($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X50);
    // 0x80042680: lw          $t0, 0x40($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X40);
    // 0x80042684: beq         $v1, $zero, L_800426A4
    if (ctx->r3 == 0) {
        // 0x80042688: lhu         $t9, 0x3E($sp)
        ctx->r25 = MEM_HU(ctx->r29, 0X3E);
            goto L_800426A4;
    }
    // 0x80042688: lhu         $t9, 0x3E($sp)
    ctx->r25 = MEM_HU(ctx->r29, 0X3E);
    // 0x8004268C: lhu         $t7, 0x40($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X40);
    // 0x80042690: lhu         $t3, 0x3E($sp)
    ctx->r11 = MEM_HU(ctx->r29, 0X3E);
    // 0x80042694: or          $t8, $t7, $t3
    ctx->r24 = ctx->r15 | ctx->r11;
    // 0x80042698: sh          $t8, 0x40($a0)
    MEM_H(0X40, ctx->r4) = ctx->r24;
    // 0x8004269C: b           L_800426B4
    // 0x800426A0: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
        goto L_800426B4;
    // 0x800426A0: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
L_800426A4:
    // 0x800426A4: lhu         $t6, 0x44($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X44);
    // 0x800426A8: or          $t1, $t6, $t9
    ctx->r9 = ctx->r14 | ctx->r25;
    // 0x800426AC: sh          $t1, 0x44($a0)
    MEM_H(0X44, ctx->r4) = ctx->r9;
    // 0x800426B0: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
L_800426B4:
    // 0x800426B4: lw          $t2, 0x44($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X44);
    // 0x800426B8: or          $t4, $v0, $t2
    ctx->r12 = ctx->r2 | ctx->r10;
    // 0x800426BC: sw          $t4, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r12;
L_800426C0:
    // 0x800426C0: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
L_800426C4:
    // 0x800426C4: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x800426C8: jr          $ra
    // 0x800426CC: nop

    return;
    // 0x800426CC: nop

;}
