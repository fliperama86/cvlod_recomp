#include "recomp.h"
#include "funcs.h"
#include <stdio.h>

RECOMP_FUNC void func_80181914(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80181914: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80181918: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8018191C: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80181920: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80181924: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x80181928: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8018192C: jal         0x80020E50
    // 0x80181930: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    func_80020E50(rdram, ctx);
        goto after_0;
    // 0x80181930: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
    after_0:
    // 0x80181934: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80181938: jal         0x80020E50
    // 0x8018193C: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    func_80020E50(rdram, ctx);
        goto after_1;
    // 0x8018193C: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    after_1:
    // 0x80181940: beq         $v0, $zero, L_80181A08
    if (ctx->r2 == 0) {
        // 0x80181944: lw          $v1, 0x18($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X18);
            goto L_80181A08;
    }
    // 0x80181944: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80181948: beql        $v1, $zero, L_80181A0C
    if (ctx->r3 == 0) {
        // 0x8018194C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80181A0C;
    }
    goto skip_0;
    // 0x8018194C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80181950: lwc1        $f4, 0x4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X4);
    // 0x80181954: lwc1        $f6, 0x4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80181958: lui         $at, 0xBF80
    ctx->r1 = S32(0XBF80 << 16);
    // 0x8018195C: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80181960: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80181964: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x80181968: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x8018196C: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80181970: mul.s       $f0, $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x80181974: c.lt.s      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.fl < ctx->f0.fl;
    // 0x80181978: nop

    // 0x8018197C: bc1fl       L_80181A0C
    if (!c1cs) {
        // 0x80181980: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80181A0C;
    }
    goto skip_1;
    // 0x80181980: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80181984: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x80181988: lui         $at, 0x405E
    ctx->r1 = S32(0X405E << 16);
    // 0x8018198C: c.lt.s      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.fl < ctx->f0.fl;
    // 0x80181990: nop

    // 0x80181994: bc1fl       L_801819A4
    if (!c1cs) {
        // 0x80181998: lwc1        $f16, 0xF0($v0)
        ctx->f16.u32l = MEM_W(ctx->r2, 0XF0);
            goto L_801819A4;
    }
    goto skip_2;
    // 0x80181998: lwc1        $f16, 0xF0($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XF0);
    skip_2:
    // 0x8018199C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    // 0x801819A0: lwc1        $f16, 0xF0($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XF0);
L_801819A4:
    // 0x801819A4: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x801819A8: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801819AC: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x801819B0: cvt.d.s     $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f6.d = CVT_D_S(ctx->f0.fl);
    // 0x801819B4: sub.d       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f10.d - ctx->f18.d;
    // 0x801819B8: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x801819BC: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x801819C0: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x801819C4: mul.d       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f4.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f4.d);
    // 0x801819C8: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x801819CC: lw          $t7, -0x53D8($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X53D8);
    // 0x801819D0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801819D4: lwc1        $f18, -0x388C($at)
    ctx->f18.u32l = MEM_W(ctx->r1, -0X388C);
    // 0x801819D8: lh          $t8, 0xF8($t7)
    ctx->r24 = MEM_H(ctx->r15, 0XF8);
    // 0x801819DC: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
    // 0x801819E0: div.d       $f10, $f8, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f16.d); 
    ctx->f10.d = DIV_D(ctx->f8.d, ctx->f16.d);
    // 0x801819E4: cvt.s.d     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f2.fl = CVT_S_D(ctx->f10.d);
    // 0x801819E8: mul.s       $f2, $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f18.fl);
    // 0x801819EC: bnel        $t8, $at, L_80181A00
    if (ctx->r24 != ctx->r1) {
        // 0x801819F0: mfc1        $a1, $f2
        ctx->r5 = (int32_t)ctx->f2.u32l;
            goto L_80181A00;
    }
    goto skip_3;
    // 0x801819F0: mfc1        $a1, $f2
    ctx->r5 = (int32_t)ctx->f2.u32l;
    skip_3:
    // 0x801819F4: mul.s       $f2, $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f12.fl);
    // 0x801819F8: nop

    // 0x801819FC: mfc1        $a1, $f2
    ctx->r5 = (int32_t)ctx->f2.u32l;
L_80181A00:
    // 0x80181A00: jal         0x8017FF18
    // 0x80181A04: addiu       $a0, $v0, 0x9C
    ctx->r4 = ADD32(ctx->r2, 0X9C);
    func_8017FF18(rdram, ctx);
        goto after_2;
    // 0x80181A04: addiu       $a0, $v0, 0x9C
    ctx->r4 = ADD32(ctx->r2, 0X9C);
    after_2:
L_80181A08:
    // 0x80181A08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80181A0C:
    // 0x80181A0C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80181A10: jr          $ra
    // 0x80181A14: nop

    return;
    // 0x80181A14: nop

;}
RECOMP_FUNC void func_80181A18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80181A18: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80181A1C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80181A20: sw          $a1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r5;
    // 0x80181A24: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x80181A28: beq         $a0, $zero, L_80181A38
    if (ctx->r4 == 0) {
        // 0x80181A2C: sw          $t6, 0x44($sp)
        MEM_W(0X44, ctx->r29) = ctx->r14;
            goto L_80181A38;
    }
    // 0x80181A2C: sw          $t6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r14;
    // 0x80181A30: bne         $a1, $zero, L_80181A40
    if (ctx->r5 != 0) {
        // 0x80181A34: lui         $v1, 0x800F
        ctx->r3 = S32(0X800F << 16);
            goto L_80181A40;
    }
    // 0x80181A34: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
L_80181A38:
    // 0x80181A38: b           L_80181C08
    // 0x80181A3C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80181C08;
    // 0x80181A3C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80181A40:
    // 0x80181A40: addiu       $v1, $v1, 0x3890
    ctx->r3 = ADD32(ctx->r3, 0X3890);
    // 0x80181A44: lw          $t8, 0x0($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X0);
    // 0x80181A48: andi        $t9, $t8, 0x200
    ctx->r25 = ctx->r24 & 0X200;
    // 0x80181A4C: beql        $t9, $zero, L_80181A88
    if (ctx->r25 == 0) {
        // 0x80181A50: lwc1        $f6, 0x6C($v1)
        ctx->f6.u32l = MEM_W(ctx->r3, 0X6C);
            goto L_80181A88;
    }
    goto skip_0;
    // 0x80181A50: lwc1        $f6, 0x6C($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X6C);
    skip_0:
    // 0x80181A54: jal         0x80001FD0
    // 0x80181A58: lw          $a0, 0x70($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X70);
    func_80001FD0(rdram, ctx);
        goto after_0;
    // 0x80181A58: lw          $a0, 0x70($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X70);
    after_0:
    // 0x80181A5C: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x80181A60: beq         $v0, $zero, L_80181A84
    if (ctx->r2 == 0) {
        // 0x80181A64: addiu       $v1, $v1, 0x3890
        ctx->r3 = ADD32(ctx->r3, 0X3890);
            goto L_80181A84;
    }
    // 0x80181A64: addiu       $v1, $v1, 0x3890
    ctx->r3 = ADD32(ctx->r3, 0X3890);
    // 0x80181A68: lw          $t0, 0x70($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X70);
    // 0x80181A6C: lh          $t1, 0x0($t0)
    ctx->r9 = MEM_H(ctx->r8, 0X0);
    // 0x80181A70: andi        $t2, $t1, 0x800
    ctx->r10 = ctx->r9 & 0X800;
    // 0x80181A74: beql        $t2, $zero, L_80181A88
    if (ctx->r10 == 0) {
        // 0x80181A78: lwc1        $f6, 0x6C($v1)
        ctx->f6.u32l = MEM_W(ctx->r3, 0X6C);
            goto L_80181A88;
    }
    goto skip_1;
    // 0x80181A78: lwc1        $f6, 0x6C($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X6C);
    skip_1:
    // 0x80181A7C: b           L_80181C08
    // 0x80181A80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80181C08;
    // 0x80181A80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80181A84:
    // 0x80181A84: lwc1        $f6, 0x6C($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X6C);
L_80181A88:
    // 0x80181A88: lui         $at, 0xC04B
    ctx->r1 = S32(0XC04B << 16);
    // 0x80181A8C: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x80181A90: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80181A94: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x80181A98: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80181A9C: c.le.d      $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f8.d <= ctx->f4.d;
    // 0x80181AA0: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80181AA4: bc1f        L_80181AB4
    if (!c1cs) {
        // 0x80181AA8: nop
    
            goto L_80181AB4;
    }
    // 0x80181AA8: nop

    // 0x80181AAC: b           L_80181C08
    // 0x80181AB0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80181C08;
    // 0x80181AB0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80181AB4:
    // 0x80181AB4: jal         0x80013CF8
    // 0x80181AB8: addiu       $a1, $a1, 0x3904
    ctx->r5 = ADD32(ctx->r5, 0X3904);
    func_80013CF8(rdram, ctx);
        goto after_1;
    // 0x80181AB8: addiu       $a1, $a1, 0x3904
    ctx->r5 = ADD32(ctx->r5, 0X3904);
    after_1:
    // 0x80181ABC: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80181AC0: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    // 0x80181AC4: jal         0x80013E3C
    // 0x80181AC8: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    func_80013E3C(rdram, ctx);
        goto after_2;
    // 0x80181AC8: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    after_2:
    // 0x80181ACC: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80181AD0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80181AD4: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    // 0x80181AD8: c.lt.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl < ctx->f10.fl;
    // 0x80181ADC: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    // 0x80181AE0: bc1f        L_80181AFC
    if (!c1cs) {
        // 0x80181AE4: nop
    
            goto L_80181AFC;
    }
    // 0x80181AE4: nop

    // 0x80181AE8: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80181AEC: jal         0x80013E3C
    // 0x80181AF0: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    func_80013E3C(rdram, ctx);
        goto after_3;
    // 0x80181AF0: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    after_3:
    // 0x80181AF4: b           L_80181B08
    // 0x80181AF8: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_80181B08;
    // 0x80181AF8: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_80181AFC:
    // 0x80181AFC: jal         0x80013E3C
    // 0x80181B00: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    func_80013E3C(rdram, ctx);
        goto after_4;
    // 0x80181B00: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    after_4:
    // 0x80181B04: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_80181B08:
    // 0x80181B08: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80181B0C: lwc1        $f16, -0x3888($at)
    ctx->f16.u32l = MEM_W(ctx->r1, -0X3888);
    // 0x80181B10: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80181B14: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    // 0x80181B18: c.le.s      $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f16.fl <= ctx->f2.fl;
    // 0x80181B1C: nop

    // 0x80181B20: bc1t        L_80181B8C
    if (c1cs) {
        // 0x80181B24: nop
    
            goto L_80181B8C;
    }
    // 0x80181B24: nop

    // 0x80181B28: jal         0x80013E3C
    // 0x80181B2C: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    func_80013E3C(rdram, ctx);
        goto after_5;
    // 0x80181B2C: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    after_5:
    // 0x80181B30: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80181B34: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80181B38: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    // 0x80181B3C: c.lt.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl < ctx->f18.fl;
    // 0x80181B40: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    // 0x80181B44: bc1f        L_80181B60
    if (!c1cs) {
        // 0x80181B48: nop
    
            goto L_80181B60;
    }
    // 0x80181B48: nop

    // 0x80181B4C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80181B50: jal         0x80013E3C
    // 0x80181B54: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    func_80013E3C(rdram, ctx);
        goto after_6;
    // 0x80181B54: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    after_6:
    // 0x80181B58: b           L_80181B6C
    // 0x80181B5C: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_80181B6C;
    // 0x80181B5C: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_80181B60:
    // 0x80181B60: jal         0x80013E3C
    // 0x80181B64: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    func_80013E3C(rdram, ctx);
        goto after_7;
    // 0x80181B64: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    after_7:
    // 0x80181B68: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_80181B6C:
    // 0x80181B6C: lui         $at, 0x3F00
    ctx->r1 = S32(0X3F00 << 16);
    // 0x80181B70: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80181B74: lw          $t3, 0x44($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X44);
    // 0x80181B78: c.le.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl <= ctx->f6.fl;
    // 0x80181B7C: addiu       $a1, $t3, 0x9C
    ctx->r5 = ADD32(ctx->r11, 0X9C);
    // 0x80181B80: addiu       $a2, $t3, 0xA8
    ctx->r6 = ADD32(ctx->r11, 0XA8);
    // 0x80181B84: bc1f        L_80181B94
    if (!c1cs) {
        // 0x80181B88: nop
    
            goto L_80181B94;
    }
    // 0x80181B88: nop

L_80181B8C:
    // 0x80181B8C: b           L_80181C08
    // 0x80181B90: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80181C08;
    // 0x80181B90: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80181B94:
    // 0x80181B94: jal         0x80013CC4
    // 0x80181B98: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    func_80013CC4(rdram, ctx);
        goto after_8;
    // 0x80181B98: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    after_8:
    // 0x80181B9C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80181BA0: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x80181BA4: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80181BA8: jal         0x80013DE4
    // 0x80181BAC: swc1        $f4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f4.u32l;
    func_80013DE4(rdram, ctx);
        goto after_9;
    // 0x80181BAC: swc1        $f4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f4.u32l;
    after_9:
    // 0x80181BB0: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80181BB4: jal         0x80013E3C
    // 0x80181BB8: addiu       $a1, $sp, 0x2C
    ctx->r5 = ADD32(ctx->r29, 0X2C);
    func_80013E3C(rdram, ctx);
        goto after_10;
    // 0x80181BB8: addiu       $a1, $sp, 0x2C
    ctx->r5 = ADD32(ctx->r29, 0X2C);
    after_10:
    // 0x80181BBC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80181BC0: ldc1        $f10, -0x3880($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X3880);
    // 0x80181BC4: cvt.d.s     $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f8.d = CVT_D_S(ctx->f0.fl);
    // 0x80181BC8: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80181BCC: mul.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = MUL_D(ctx->f8.d, ctx->f10.d);
    // 0x80181BD0: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X4C);
    // 0x80181BD4: cvt.s.d     $f12, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f12.fl = CVT_S_D(ctx->f16.d);
    // 0x80181BD8: c.eq.s      $f18, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f18.fl == ctx->f12.fl;
    // 0x80181BDC: nop

    // 0x80181BE0: bc1fl       L_80181BF4
    if (!c1cs) {
        // 0x80181BE4: lwc1        $f6, 0xC($a0)
        ctx->f6.u32l = MEM_W(ctx->r4, 0XC);
            goto L_80181BF4;
    }
    goto skip_2;
    // 0x80181BE4: lwc1        $f6, 0xC($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0XC);
    skip_2:
    // 0x80181BE8: b           L_80181C08
    // 0x80181BEC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80181C08;
    // 0x80181BEC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80181BF0: lwc1        $f6, 0xC($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0XC);
L_80181BF4:
    // 0x80181BF4: add.s       $f4, $f6, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f12.fl;
    // 0x80181BF8: mfc1        $a1, $f4
    ctx->r5 = (int32_t)ctx->f4.u32l;
    // 0x80181BFC: jal         0x8017FB64
    // 0x80181C00: nop

    func_8017FB64(rdram, ctx);
        goto after_11;
    // 0x80181C00: nop

    after_11:
    // 0x80181C04: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80181C08:
    // 0x80181C08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80181C0C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x80181C10: jr          $ra
    // 0x80181C14: nop

    return;
    // 0x80181C14: nop

;}
RECOMP_FUNC void func_80181C18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80181C18: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80181C1C: jr          $ra
    // 0x80181C20: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    return;
    // 0x80181C20: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
;}
RECOMP_FUNC void func_80181C24(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80181C24: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80181C28: lwc1        $f4, 0xC4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0XC4);
    // 0x80181C2C: lwc1        $f6, 0xB4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0XB4);
    // 0x80181C30: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80181C34: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x80181C38: nop

    // 0x80181C3C: bc1f        L_80181C4C
    if (!c1cs) {
        // 0x80181C40: nop
    
            goto L_80181C4C;
    }
    // 0x80181C40: nop

    // 0x80181C44: jr          $ra
    // 0x80181C48: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80181C48: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80181C4C:
    // 0x80181C4C: jr          $ra
    // 0x80181C50: nop

    return;
    // 0x80181C50: nop

;}
RECOMP_FUNC void func_80181C54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80181C54: addiu       $sp, $sp, -0x98
    ctx->r29 = ADD32(ctx->r29, -0X98);
    // 0x80181C58: sw          $s6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r22;
    // 0x80181C5C: sw          $s5, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r21;
    // 0x80181C60: or          $s5, $a1, $zero
    ctx->r21 = ctx->r5 | 0;
    // 0x80181C64: or          $s6, $a2, $zero
    ctx->r22 = ctx->r6 | 0;
    // 0x80181C68: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80181C6C: sw          $s7, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r23;
    // 0x80181C70: sw          $s4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r20;
    // 0x80181C74: sw          $s3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r19;
    // 0x80181C78: sw          $s2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r18;
    // 0x80181C7C: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x80181C80: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x80181C84: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80181C88: sw          $a0, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->r4;
    // 0x80181C8C: beq         $a0, $zero, L_80181CB0
    if (ctx->r4 == 0) {
        // 0x80181C90: sw          $zero, 0x8C($sp)
        MEM_W(0X8C, ctx->r29) = 0;
            goto L_80181CB0;
    }
    // 0x80181C90: sw          $zero, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = 0;
    // 0x80181C94: slti        $at, $a1, 0x2
    ctx->r1 = SIGNED(ctx->r5) < 0X2 ? 1 : 0;
    // 0x80181C98: bne         $at, $zero, L_80181CB0
    if (ctx->r1 != 0) {
        // 0x80181C9C: slti        $at, $a1, 0x6
        ctx->r1 = SIGNED(ctx->r5) < 0X6 ? 1 : 0;
            goto L_80181CB0;
    }
    // 0x80181C9C: slti        $at, $a1, 0x6
    ctx->r1 = SIGNED(ctx->r5) < 0X6 ? 1 : 0;
    // 0x80181CA0: beq         $at, $zero, L_80181CB0
    if (ctx->r1 == 0) {
        // 0x80181CA4: nop
    
            goto L_80181CB0;
    }
    // 0x80181CA4: nop

    // 0x80181CA8: bne         $a2, $zero, L_80181CB8
    if (ctx->r6 != 0) {
        // 0x80181CAC: sll         $s7, $s5, 1
        ctx->r23 = S32(ctx->r21 << 1);
            goto L_80181CB8;
    }
    // 0x80181CAC: sll         $s7, $s5, 1
    ctx->r23 = S32(ctx->r21 << 1);
L_80181CB0:
    // 0x80181CB0: b           L_80181EBC
    // 0x80181CB4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80181EBC;
    // 0x80181CB4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80181CB8:
    // 0x80181CB8: blez        $s7, L_80181D10
    if (SIGNED(ctx->r23) <= 0) {
        // 0x80181CBC: or          $s0, $zero, $zero
        ctx->r16 = 0 | 0;
            goto L_80181D10;
    }
    // 0x80181CBC: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80181CC0: addiu       $s3, $s7, -0x1
    ctx->r19 = ADD32(ctx->r23, -0X1);
    // 0x80181CC4: addiu       $s2, $sp, 0x58
    ctx->r18 = ADD32(ctx->r29, 0X58);
    // 0x80181CC8: slt         $at, $s0, $s3
    ctx->r1 = SIGNED(ctx->r16) < SIGNED(ctx->r19) ? 1 : 0;
L_80181CCC:
    // 0x80181CCC: beq         $at, $zero, L_80181D04
    if (ctx->r1 == 0) {
        // 0x80181CD0: or          $a0, $s2, $zero
        ctx->r4 = ctx->r18 | 0;
            goto L_80181D04;
    }
    // 0x80181CD0: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80181CD4: sll         $t7, $s0, 2
    ctx->r15 = S32(ctx->r16 << 2);
    // 0x80181CD8: subu        $t7, $t7, $s0
    ctx->r15 = SUB32(ctx->r15, ctx->r16);
    // 0x80181CDC: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80181CE0: addu        $a2, $s6, $t7
    ctx->r6 = ADD32(ctx->r22, ctx->r15);
    // 0x80181CE4: sll         $t8, $s0, 2
    ctx->r24 = S32(ctx->r16 << 2);
    // 0x80181CE8: addiu       $t9, $sp, 0x68
    ctx->r25 = ADD32(ctx->r29, 0X68);
    // 0x80181CEC: addu        $s1, $t8, $t9
    ctx->r17 = ADD32(ctx->r24, ctx->r25);
    // 0x80181CF0: jal         0x80013CC4
    // 0x80181CF4: addiu       $a1, $a2, 0xC
    ctx->r5 = ADD32(ctx->r6, 0XC);
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x80181CF4: addiu       $a1, $a2, 0xC
    ctx->r5 = ADD32(ctx->r6, 0XC);
    after_0:
    // 0x80181CF8: jal         0x80013BD0
    // 0x80181CFC: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    func_80013BD0(rdram, ctx);
        goto after_1;
    // 0x80181CFC: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    after_1:
    // 0x80181D00: swc1        $f0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->f0.u32l;
L_80181D04:
    // 0x80181D04: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80181D08: bnel        $s0, $s7, L_80181CCC
    if (ctx->r16 != ctx->r23) {
        // 0x80181D0C: slt         $at, $s0, $s3
        ctx->r1 = SIGNED(ctx->r16) < SIGNED(ctx->r19) ? 1 : 0;
            goto L_80181CCC;
    }
    goto skip_0;
    // 0x80181D0C: slt         $at, $s0, $s3
    ctx->r1 = SIGNED(ctx->r16) < SIGNED(ctx->r19) ? 1 : 0;
    skip_0:
L_80181D10:
    // 0x80181D10: slti        $at, $s7, 0x4
    ctx->r1 = SIGNED(ctx->r23) < 0X4 ? 1 : 0;
    // 0x80181D14: bne         $at, $zero, L_80181E04
    if (ctx->r1 != 0) {
        // 0x80181D18: or          $s4, $s5, $zero
        ctx->r20 = ctx->r21 | 0;
            goto L_80181E04;
    }
    // 0x80181D18: or          $s4, $s5, $zero
    ctx->r20 = ctx->r21 | 0;
    // 0x80181D1C: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80181D20: addiu       $s5, $sp, 0x68
    ctx->r21 = ADD32(ctx->r29, 0X68);
    // 0x80181D24: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
L_80181D28:
    // 0x80181D28: blez        $s4, L_80181DE8
    if (SIGNED(ctx->r20) <= 0) {
        // 0x80181D2C: or          $s3, $zero, $zero
        ctx->r19 = 0 | 0;
            goto L_80181DE8;
    }
    // 0x80181D2C: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x80181D30: sll         $t0, $s4, 2
    ctx->r8 = S32(ctx->r20 << 2);
    // 0x80181D34: subu        $t0, $t0, $s4
    ctx->r8 = SUB32(ctx->r8, ctx->r20);
    // 0x80181D38: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x80181D3C: addu        $s0, $s6, $t0
    ctx->r16 = ADD32(ctx->r22, ctx->r8);
    // 0x80181D40: or          $s1, $s6, $zero
    ctx->r17 = ctx->r22 | 0;
L_80181D44:
    // 0x80181D44: lwc1        $f4, 0x0($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X0);
    // 0x80181D48: lwc1        $f6, 0x0($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80181D4C: c.eq.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl == ctx->f6.fl;
    // 0x80181D50: nop

    // 0x80181D54: bc1fl       L_80181DDC
    if (!c1cs) {
        // 0x80181D58: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80181DDC;
    }
    goto skip_1;
    // 0x80181D58: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_1:
    // 0x80181D5C: lwc1        $f8, 0x4($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80181D60: lwc1        $f10, 0x4($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80181D64: c.eq.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl == ctx->f10.fl;
    // 0x80181D68: nop

    // 0x80181D6C: bc1fl       L_80181DDC
    if (!c1cs) {
        // 0x80181D70: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80181DDC;
    }
    goto skip_2;
    // 0x80181D70: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_2:
    // 0x80181D74: lwc1        $f16, 0x8($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X8);
    // 0x80181D78: lwc1        $f18, 0x8($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80181D7C: c.eq.s      $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f16.fl == ctx->f18.fl;
    // 0x80181D80: nop

    // 0x80181D84: bc1fl       L_80181DDC
    if (!c1cs) {
        // 0x80181D88: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80181DDC;
    }
    goto skip_3;
    // 0x80181D88: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_3:
    // 0x80181D8C: jal         0x80013BD0
    // 0x80181D90: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80013BD0(rdram, ctx);
        goto after_2;
    // 0x80181D90: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_2:
    // 0x80181D94: c.eq.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl == ctx->f20.fl;
    // 0x80181D98: nop

    // 0x80181D9C: bc1tl       L_80181DDC
    if (c1cs) {
        // 0x80181DA0: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80181DDC;
    }
    goto skip_4;
    // 0x80181DA0: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_4:
    // 0x80181DA4: jal         0x80013BD0
    // 0x80181DA8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80013BD0(rdram, ctx);
        goto after_3;
    // 0x80181DA8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x80181DAC: c.eq.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl == ctx->f20.fl;
    // 0x80181DB0: sll         $t1, $s2, 2
    ctx->r9 = S32(ctx->r18 << 2);
    // 0x80181DB4: addu        $t2, $s5, $t1
    ctx->r10 = ADD32(ctx->r21, ctx->r9);
    // 0x80181DB8: bc1tl       L_80181DDC
    if (c1cs) {
        // 0x80181DBC: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80181DDC;
    }
    goto skip_5;
    // 0x80181DBC: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_5:
    // 0x80181DC0: lwc1        $f4, 0x0($t2)
    ctx->f4.u32l = MEM_W(ctx->r10, 0X0);
    // 0x80181DC4: c.eq.s      $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f20.fl == ctx->f4.fl;
    // 0x80181DC8: nop

    // 0x80181DCC: bc1tl       L_80181DDC
    if (c1cs) {
        // 0x80181DD0: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_80181DDC;
    }
    goto skip_6;
    // 0x80181DD0: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_6:
    // 0x80181DD4: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x80181DD8: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_80181DDC:
    // 0x80181DDC: addiu       $s1, $s1, 0xC
    ctx->r17 = ADD32(ctx->r17, 0XC);
    // 0x80181DE0: bne         $s2, $s4, L_80181D44
    if (ctx->r18 != ctx->r20) {
        // 0x80181DE4: addiu       $s0, $s0, 0xC
        ctx->r16 = ADD32(ctx->r16, 0XC);
            goto L_80181D44;
    }
    // 0x80181DE4: addiu       $s0, $s0, 0xC
    ctx->r16 = ADD32(ctx->r16, 0XC);
L_80181DE8:
    // 0x80181DE8: bne         $s3, $s4, L_80181DF8
    if (ctx->r19 != ctx->r20) {
        // 0x80181DEC: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80181DF8;
    }
    // 0x80181DEC: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80181DF0: b           L_80181E04
    // 0x80181DF4: sw          $s4, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->r20;
        goto L_80181E04;
    // 0x80181DF4: sw          $s4, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->r20;
L_80181DF8:
    // 0x80181DF8: addiu       $s4, $s4, -0x1
    ctx->r20 = ADD32(ctx->r20, -0X1);
    // 0x80181DFC: bnel        $s4, $at, L_80181D28
    if (ctx->r20 != ctx->r1) {
        // 0x80181E00: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_80181D28;
    }
    goto skip_7;
    // 0x80181E00: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    skip_7:
L_80181E04:
    // 0x80181E04: lw          $t3, 0x8C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X8C);
    // 0x80181E08: lw          $t5, 0x98($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X98);
    // 0x80181E0C: addiu       $s3, $s7, -0x1
    ctx->r19 = ADD32(ctx->r23, -0X1);
    // 0x80181E10: slti        $at, $t3, 0x2
    ctx->r1 = SIGNED(ctx->r11) < 0X2 ? 1 : 0;
    // 0x80181E14: bne         $at, $zero, L_80181E78
    if (ctx->r1 != 0) {
        // 0x80181E18: sll         $t4, $t3, 2
        ctx->r12 = S32(ctx->r11 << 2);
            goto L_80181E78;
    }
    // 0x80181E18: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80181E1C: subu        $t4, $t4, $t3
    ctx->r12 = SUB32(ctx->r12, ctx->r11);
    // 0x80181E20: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x80181E24: addu        $v0, $s6, $t4
    ctx->r2 = ADD32(ctx->r22, ctx->r12);
    // 0x80181E28: lwc1        $f8, -0xC($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, -0XC);
    // 0x80181E2C: lwc1        $f6, 0x0($t5)
    ctx->f6.u32l = MEM_W(ctx->r13, 0X0);
    // 0x80181E30: c.eq.s      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.fl == ctx->f8.fl;
    // 0x80181E34: nop

    // 0x80181E38: bc1f        L_80181E78
    if (!c1cs) {
        // 0x80181E3C: nop
    
            goto L_80181E78;
    }
    // 0x80181E3C: nop

    // 0x80181E40: lwc1        $f10, 0x4($t5)
    ctx->f10.u32l = MEM_W(ctx->r13, 0X4);
    // 0x80181E44: lwc1        $f16, -0x8($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, -0X8);
    // 0x80181E48: c.eq.s      $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f10.fl == ctx->f16.fl;
    // 0x80181E4C: nop

    // 0x80181E50: bc1f        L_80181E78
    if (!c1cs) {
        // 0x80181E54: nop
    
            goto L_80181E78;
    }
    // 0x80181E54: nop

    // 0x80181E58: lwc1        $f18, 0x8($t5)
    ctx->f18.u32l = MEM_W(ctx->r13, 0X8);
    // 0x80181E5C: lwc1        $f4, -0x4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, -0X4);
    // 0x80181E60: c.eq.s      $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f18.fl == ctx->f4.fl;
    // 0x80181E64: nop

    // 0x80181E68: bc1f        L_80181E78
    if (!c1cs) {
        // 0x80181E6C: nop
    
            goto L_80181E78;
    }
    // 0x80181E6C: nop

    // 0x80181E70: b           L_80181EBC
    // 0x80181E74: or          $v0, $t3, $zero
    ctx->r2 = ctx->r11 | 0;
        goto L_80181EBC;
    // 0x80181E74: or          $v0, $t3, $zero
    ctx->r2 = ctx->r11 | 0;
L_80181E78:
    // 0x80181E78: blez        $s3, L_80181EAC
    if (SIGNED(ctx->r19) <= 0) {
        // 0x80181E7C: sll         $s0, $s3, 2
        ctx->r16 = S32(ctx->r19 << 2);
            goto L_80181EAC;
    }
    // 0x80181E7C: sll         $s0, $s3, 2
    ctx->r16 = S32(ctx->r19 << 2);
    // 0x80181E80: subu        $s0, $s0, $s3
    ctx->r16 = SUB32(ctx->r16, ctx->r19);
    // 0x80181E84: sll         $s0, $s0, 2
    ctx->r16 = S32(ctx->r16 << 2);
    // 0x80181E88: addu        $s1, $s6, $s0
    ctx->r17 = ADD32(ctx->r22, ctx->r16);
    // 0x80181E8C: addiu       $s3, $s1, -0xC
    ctx->r19 = ADD32(ctx->r17, -0XC);
L_80181E90:
    // 0x80181E90: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80181E94: jal         0x80013CF8
    // 0x80181E98: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    func_80013CF8(rdram, ctx);
        goto after_4;
    // 0x80181E98: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    after_4:
    // 0x80181E9C: addiu       $s0, $s0, -0xC
    ctx->r16 = ADD32(ctx->r16, -0XC);
    // 0x80181EA0: addiu       $s1, $s1, -0xC
    ctx->r17 = ADD32(ctx->r17, -0XC);
    // 0x80181EA4: bne         $s0, $zero, L_80181E90
    if (ctx->r16 != 0) {
        // 0x80181EA8: addiu       $s3, $s3, -0xC
        ctx->r19 = ADD32(ctx->r19, -0XC);
            goto L_80181E90;
    }
    // 0x80181EA8: addiu       $s3, $s3, -0xC
    ctx->r19 = ADD32(ctx->r19, -0XC);
L_80181EAC:
    // 0x80181EAC: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80181EB0: jal         0x80013CF8
    // 0x80181EB4: lw          $a1, 0x98($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X98);
    func_80013CF8(rdram, ctx);
        goto after_5;
    // 0x80181EB4: lw          $a1, 0x98($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X98);
    after_5:
    // 0x80181EB8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80181EBC:
    // 0x80181EBC: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80181EC0: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80181EC4: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x80181EC8: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x80181ECC: lw          $s2, 0x24($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X24);
    // 0x80181ED0: lw          $s3, 0x28($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X28);
    // 0x80181ED4: lw          $s4, 0x2C($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X2C);
    // 0x80181ED8: lw          $s5, 0x30($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X30);
    // 0x80181EDC: lw          $s6, 0x34($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X34);
    // 0x80181EE0: lw          $s7, 0x38($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X38);
    // 0x80181EE4: jr          $ra
    // 0x80181EE8: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
    return;
    // 0x80181EE8: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
;}
RECOMP_FUNC void func_80181EF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80181EF0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80181EF4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80181EF8: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80181EFC: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80181F00: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    // 0x80181F04: lw          $t6, 0x295C($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X295C);
    // 0x80181F08: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80181F0C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80181F10: beq         $t6, $zero, L_80182044
    if (ctx->r14 == 0) {
        // 0x80181F14: or          $a1, $v1, $zero
        ctx->r5 = ctx->r3 | 0;
            goto L_80182044;
    }
    // 0x80181F14: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    // 0x80181F18: lw          $t7, 0x10($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X10);
    // 0x80181F1C: beql        $t7, $zero, L_80182048
    if (ctx->r15 == 0) {
        // 0x80181F20: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80182048;
    }
    goto skip_0;
    // 0x80181F20: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80181F24: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80181F28: addiu       $at, $zero, 0x8
    ctx->r1 = ADD32(0, 0X8);
    // 0x80181F2C: beql        $v0, $zero, L_80181F40
    if (ctx->r2 == 0) {
        // 0x80181F30: lw          $v0, 0x4($v1)
        ctx->r2 = MEM_W(ctx->r3, 0X4);
            goto L_80181F40;
    }
    goto skip_1;
    // 0x80181F30: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    skip_1:
    // 0x80181F34: bnel        $v0, $at, L_80182048
    if (ctx->r2 != ctx->r1) {
        // 0x80181F38: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80182048;
    }
    goto skip_2;
    // 0x80181F38: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x80181F3C: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
L_80181F40:
    // 0x80181F40: addiu       $at, $zero, 0x30
    ctx->r1 = ADD32(0, 0X30);
    // 0x80181F44: beq         $v0, $at, L_80181F74
    if (ctx->r2 == ctx->r1) {
        // 0x80181F48: addiu       $at, $zero, 0x31
        ctx->r1 = ADD32(0, 0X31);
            goto L_80181F74;
    }
    // 0x80181F48: addiu       $at, $zero, 0x31
    ctx->r1 = ADD32(0, 0X31);
    // 0x80181F4C: beq         $v0, $at, L_80181F74
    if (ctx->r2 == ctx->r1) {
        // 0x80181F50: addiu       $at, $zero, 0x32
        ctx->r1 = ADD32(0, 0X32);
            goto L_80181F74;
    }
    // 0x80181F50: addiu       $at, $zero, 0x32
    ctx->r1 = ADD32(0, 0X32);
    // 0x80181F54: beq         $v0, $at, L_80181F74
    if (ctx->r2 == ctx->r1) {
        // 0x80181F58: addiu       $at, $zero, 0x33
        ctx->r1 = ADD32(0, 0X33);
            goto L_80181F74;
    }
    // 0x80181F58: addiu       $at, $zero, 0x33
    ctx->r1 = ADD32(0, 0X33);
    // 0x80181F5C: beq         $v0, $at, L_80181F74
    if (ctx->r2 == ctx->r1) {
        // 0x80181F60: nop
    
            goto L_80181F74;
    }
    // 0x80181F60: nop

    // 0x80181F64: jal         0x80179DA0
    // 0x80181F68: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_80179DA0(rdram, ctx);
        goto after_0;
    // 0x80181F68: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_0:
    // 0x80181F6C: b           L_80182048
    // 0x80181F70: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80182048;
    // 0x80181F70: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80181F74:
    // 0x80181F74: jal         0x8018276C
    // 0x80181F78: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_8018276C(rdram, ctx);
        goto after_1;
    // 0x80181F78: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_1:
    // 0x80181F7C: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80181F80: beq         $v0, $zero, L_80181FB4
    if (ctx->r2 == 0) {
        // 0x80181F84: sw          $v0, 0x584($a1)
        MEM_W(0X584, ctx->r5) = ctx->r2;
            goto L_80181FB4;
    }
    // 0x80181F84: sw          $v0, 0x584($a1)
    MEM_W(0X584, ctx->r5) = ctx->r2;
    // 0x80181F88: jal         0x80179EF8
    // 0x80181F8C: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    func_80179EF8(rdram, ctx);
        goto after_2;
    // 0x80181F8C: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    after_2:
    // 0x80181F90: bne         $v0, $zero, L_80182044
    if (ctx->r2 != 0) {
        // 0x80181F94: or          $a1, $zero, $zero
        ctx->r5 = 0 | 0;
            goto L_80182044;
    }
    // 0x80181F94: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80181F98: jal         0x80179EE4
    // 0x80181F9C: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    func_80179EE4(rdram, ctx);
        goto after_3;
    // 0x80181F9C: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    after_3:
    // 0x80181FA0: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    // 0x80181FA4: jal         0x80179DA0
    // 0x80181FA8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_80179DA0(rdram, ctx);
        goto after_4;
    // 0x80181FA8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_4:
    // 0x80181FAC: b           L_80182048
    // 0x80181FB0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80182048;
    // 0x80181FB0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80181FB4:
    // 0x80181FB4: lh          $v0, 0x28D0($s0)
    ctx->r2 = MEM_H(ctx->r16, 0X28D0);
    // 0x80181FB8: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x80181FBC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80181FC0: sll         $v0, $v0, 2
    ctx->r2 = S32(ctx->r2 << 2);
    // 0x80181FC4: addu        $v1, $v1, $v0
    ctx->r3 = ADD32(ctx->r3, ctx->r2);
    // 0x80181FC8: lw          $v1, 0x63A8($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X63A8);
    // 0x80181FCC: beq         $v1, $zero, L_80181FF4
    if (ctx->r3 == 0) {
        // 0x80181FD0: nop
    
            goto L_80181FF4;
    }
    // 0x80181FD0: nop

    // 0x80181FD4: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80181FD8: addu        $a1, $a1, $v0
    ctx->r5 = ADD32(ctx->r5, ctx->r2);
    // 0x80181FDC: lw          $a1, 0x62E0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X62E0);
    // 0x80181FE0: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    // 0x80181FE4: jal         0x80182058
    // 0x80181FE8: lw          $a2, 0x0($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X0);
    func_80182058(rdram, ctx);
        goto after_5;
    // 0x80181FE8: lw          $a2, 0x0($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X0);
    after_5:
    // 0x80181FEC: b           L_80181FF4
    // 0x80181FF0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
        goto L_80181FF4;
    // 0x80181FF0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
L_80181FF4:
    // 0x80181FF4: bnel        $a1, $zero, L_80182020
    if (ctx->r5 != 0) {
        // 0x80181FF8: lw          $a0, 0x2B6C($s0)
        ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
            goto L_80182020;
    }
    goto skip_3;
    // 0x80181FF8: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    skip_3:
    // 0x80181FFC: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    // 0x80182000: jal         0x80179EE4
    // 0x80182004: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_80179EE4(rdram, ctx);
        goto after_6;
    // 0x80182004: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_6:
    // 0x80182008: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    // 0x8018200C: jal         0x80179DA0
    // 0x80182010: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_80179DA0(rdram, ctx);
        goto after_7;
    // 0x80182010: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_7:
    // 0x80182014: b           L_80182048
    // 0x80182018: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80182048;
    // 0x80182018: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8018201C: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
L_80182020:
    // 0x80182020: jal         0x80179EF8
    // 0x80182024: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    func_80179EF8(rdram, ctx);
        goto after_8;
    // 0x80182024: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    after_8:
    // 0x80182028: bne         $v0, $zero, L_80182044
    if (ctx->r2 != 0) {
        // 0x8018202C: lw          $a1, 0x20($sp)
        ctx->r5 = MEM_W(ctx->r29, 0X20);
            goto L_80182044;
    }
    // 0x8018202C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80182030: jal         0x80179EE4
    // 0x80182034: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    func_80179EE4(rdram, ctx);
        goto after_9;
    // 0x80182034: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    after_9:
    // 0x80182038: lw          $a0, 0x2B6C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B6C);
    // 0x8018203C: jal         0x80179DA0
    // 0x80182040: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_80179DA0(rdram, ctx);
        goto after_10;
    // 0x80182040: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_10:
L_80182044:
    // 0x80182044: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80182048:
    // 0x80182048: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8018204C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80182050: jr          $ra
    // 0x80182054: nop

    return;
    // 0x80182054: nop

;}
RECOMP_FUNC void func_80182058(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182058: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x8018205C: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80182060: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80182064: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80182068: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x8018206C: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80182070: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80182074: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80182078: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8018207C: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80182080: sw          $a2, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r6;
    // 0x80182084: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80182088: sw          $zero, 0x44($sp)
    MEM_W(0X44, ctx->r29) = 0;
    // 0x8018208C: beq         $a0, $zero, L_8018209C
    if (ctx->r4 == 0) {
        // 0x80182090: or          $s6, $zero, $zero
        ctx->r22 = 0 | 0;
            goto L_8018209C;
    }
    // 0x80182090: or          $s6, $zero, $zero
    ctx->r22 = 0 | 0;
    // 0x80182094: bne         $a1, $zero, L_801820A4
    if (ctx->r5 != 0) {
        // 0x80182098: addiu       $s2, $sp, 0x4C
        ctx->r18 = ADD32(ctx->r29, 0X4C);
            goto L_801820A4;
    }
    // 0x80182098: addiu       $s2, $sp, 0x4C
    ctx->r18 = ADD32(ctx->r29, 0X4C);
L_8018209C:
    // 0x8018209C: b           L_80182274
    // 0x801820A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80182274;
    // 0x801820A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801820A4:
    // 0x801820A4: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801820A8: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x801820AC: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801820B0: jal         0x80013CF8
    // 0x801820B4: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x801820B4: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_0:
    // 0x801820B8: lw          $t6, 0x60($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X60);
    // 0x801820BC: lui         $s4, 0x8000
    ctx->r20 = S32(0X8000 << 16);
    // 0x801820C0: addiu       $s4, $s4, 0x48A0
    ctx->r20 = ADD32(ctx->r20, 0X48A0);
    // 0x801820C4: beq         $s1, $t6, L_80182270
    if (ctx->r17 == ctx->r14) {
        // 0x801820C8: addiu       $s5, $zero, 0x1
        ctx->r21 = ADD32(0, 0X1);
            goto L_80182270;
    }
    // 0x801820C8: addiu       $s5, $zero, 0x1
    ctx->r21 = ADD32(0, 0X1);
    // 0x801820CC: lui         $s3, 0x801D
    ctx->r19 = S32(0X801D << 16);
    // 0x801820D0: addiu       $s3, $s3, -0x55A0
    ctx->r19 = ADD32(ctx->r19, -0X55A0);
    // 0x801820D4: lbu         $t7, 0x8($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X8);
L_801820D8:
    // 0x801820D8: sltiu       $at, $t7, 0x5
    ctx->r1 = ctx->r15 < 0X5 ? 1 : 0;
    // 0x801820DC: beq         $at, $zero, L_8018217C
    if (ctx->r1 == 0) {
        // 0x801820E0: sll         $t7, $t7, 2
        ctx->r15 = S32(ctx->r15 << 2);
            goto L_8018217C;
    }
    // 0x801820E0: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x801820E4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801820E8: addu        $at, $at, $t7
    gpr jr_addend_801820F0 = ctx->r15;
    ctx->r1 = ADD32(ctx->r1, ctx->r15);
    // 0x801820EC: lw          $t7, -0x3870($at)
    ctx->r15 = ADD32(ctx->r1, -0X3870);
    // 0x801820F0: jr          $t7
    // 0x801820F4: nop

    switch (jr_addend_801820F0 >> 2) {
        case 0: goto L_801820F8; break;
        case 1: goto L_80182114; break;
        case 2: goto L_8018212C; break;
        case 3: goto L_80182148; break;
        case 4: goto L_80182164; break;
        default: switch_error(__func__, 0x801820F0, 0x8019C790);
    }
    // 0x801820F4: nop

L_801820F8:
    // 0x801820F8: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801820FC: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    // 0x80182100: addiu       $a2, $s0, 0x18
    ctx->r6 = ADD32(ctx->r16, 0X18);
    // 0x80182104: jal         0x80182374
    // 0x80182108: lw          $a3, 0x24($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X24);
    func_80182374(rdram, ctx);
        goto after_1;
    // 0x80182108: lw          $a3, 0x24($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X24);
    after_1:
    // 0x8018210C: b           L_80182180
    // 0x80182110: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
        goto L_80182180;
    // 0x80182110: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_80182114:
    // 0x80182114: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80182118: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    // 0x8018211C: jal         0x801824B4
    // 0x80182120: lw          $a2, 0x18($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X18);
    func_801824B4(rdram, ctx);
        goto after_2;
    // 0x80182120: lw          $a2, 0x18($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X18);
    after_2:
    // 0x80182124: b           L_80182180
    // 0x80182128: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
        goto L_80182180;
    // 0x80182128: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_8018212C:
    // 0x8018212C: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80182130: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    // 0x80182134: lw          $a2, 0x18($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X18);
    // 0x80182138: jal         0x8018254C
    // 0x8018213C: lw          $a3, 0x1C($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X1C);
    func_8018254C(rdram, ctx);
        goto after_3;
    // 0x8018213C: lw          $a3, 0x1C($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X1C);
    after_3:
    // 0x80182140: b           L_80182180
    // 0x80182144: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
        goto L_80182180;
    // 0x80182144: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_80182148:
    // 0x80182148: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8018214C: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    // 0x80182150: lw          $a2, 0x18($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X18);
    // 0x80182154: jal         0x8018261C
    // 0x80182158: lw          $a3, 0x1C($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X1C);
    func_8018261C(rdram, ctx);
        goto after_4;
    // 0x80182158: lw          $a3, 0x1C($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X1C);
    after_4:
    // 0x8018215C: b           L_80182180
    // 0x80182160: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
        goto L_80182180;
    // 0x80182160: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_80182164:
    // 0x80182164: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80182168: addiu       $a1, $s0, 0xC
    ctx->r5 = ADD32(ctx->r16, 0XC);
    // 0x8018216C: jal         0x801826E8
    // 0x80182170: lw          $a2, 0x18($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X18);
    func_801826E8(rdram, ctx);
        goto after_5;
    // 0x80182170: lw          $a2, 0x18($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X18);
    after_5:
    // 0x80182174: b           L_80182180
    // 0x80182178: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
        goto L_80182180;
    // 0x80182178: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_8018217C:
    // 0x8018217C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80182180:
    // 0x80182180: lw          $t8, 0x28($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X28);
    // 0x80182184: sltiu       $at, $t8, 0x8
    ctx->r1 = ctx->r24 < 0X8 ? 1 : 0;
    // 0x80182188: beq         $at, $zero, L_80182240
    if (ctx->r1 == 0) {
        // 0x8018218C: sll         $t8, $t8, 2
        ctx->r24 = S32(ctx->r24 << 2);
            goto L_80182240;
    }
    // 0x8018218C: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80182190: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80182194: addu        $at, $at, $t8
    gpr jr_addend_8018219C = ctx->r24;
    ctx->r1 = ADD32(ctx->r1, ctx->r24);
    // 0x80182198: lw          $t8, -0x385C($at)
    ctx->r24 = ADD32(ctx->r1, -0X385C);
    // 0x8018219C: jr          $t8
    // 0x801821A0: nop

    switch (jr_addend_8018219C >> 2) {
        case 0: goto L_80182240; break;
        case 1: goto L_801821A4; break;
        case 2: goto L_801821A4; break;
        case 3: goto L_801821BC; break;
        case 4: goto L_801821D8; break;
        case 5: goto L_801821F4; break;
        case 6: goto L_80182210; break;
        case 7: goto L_8018222C; break;
        default: switch_error(__func__, 0x8018219C, 0x8019C7A4);
    }
    // 0x801821A0: nop

L_801821A4:
    // 0x801821A4: jal         0x80147FD0
    // 0x801821A8: nop

    func_80147FD0(rdram, ctx);
        goto after_6;
    // 0x801821A8: nop

    after_6:
    // 0x801821AC: bne         $v0, $zero, L_80182240
    if (ctx->r2 != 0) {
        // 0x801821B0: nop
    
            goto L_80182240;
    }
    // 0x801821B0: nop

    // 0x801821B4: b           L_80182240
    // 0x801821B8: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_80182240;
    // 0x801821B8: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_801821BC:
    // 0x801821BC: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x801821C0: jalr        $s4
    // 0x801821C4: addiu       $a1, $zero, 0x154
    ctx->r5 = ADD32(0, 0X154);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_7;
    // 0x801821C4: addiu       $a1, $zero, 0x154
    ctx->r5 = ADD32(0, 0X154);
    after_7:
    // 0x801821C8: bne         $v0, $zero, L_80182240
    if (ctx->r2 != 0) {
        // 0x801821CC: nop
    
            goto L_80182240;
    }
    // 0x801821CC: nop

    // 0x801821D0: b           L_80182240
    // 0x801821D4: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_80182240;
    // 0x801821D4: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_801821D8:
    // 0x801821D8: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x801821DC: jalr        $s4
    // 0x801821E0: addiu       $a1, $zero, 0x286
    ctx->r5 = ADD32(0, 0X286);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_8;
    // 0x801821E0: addiu       $a1, $zero, 0x286
    ctx->r5 = ADD32(0, 0X286);
    after_8:
    // 0x801821E4: bne         $v0, $zero, L_80182240
    if (ctx->r2 != 0) {
        // 0x801821E8: nop
    
            goto L_80182240;
    }
    // 0x801821E8: nop

    // 0x801821EC: b           L_80182240
    // 0x801821F0: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_80182240;
    // 0x801821F0: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_801821F4:
    // 0x801821F4: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x801821F8: jalr        $s4
    // 0x801821FC: addiu       $a1, $zero, 0x224
    ctx->r5 = ADD32(0, 0X224);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_9;
    // 0x801821FC: addiu       $a1, $zero, 0x224
    ctx->r5 = ADD32(0, 0X224);
    after_9:
    // 0x80182200: beq         $v0, $zero, L_80182240
    if (ctx->r2 == 0) {
        // 0x80182204: nop
    
            goto L_80182240;
    }
    // 0x80182204: nop

    // 0x80182208: b           L_80182240
    // 0x8018220C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_80182240;
    // 0x8018220C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80182210:
    // 0x80182210: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80182214: jalr        $s4
    // 0x80182218: addiu       $a1, $zero, 0x258
    ctx->r5 = ADD32(0, 0X258);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_10;
    // 0x80182218: addiu       $a1, $zero, 0x258
    ctx->r5 = ADD32(0, 0X258);
    after_10:
    // 0x8018221C: beq         $v0, $zero, L_80182240
    if (ctx->r2 == 0) {
        // 0x80182220: nop
    
            goto L_80182240;
    }
    // 0x80182220: nop

    // 0x80182224: b           L_80182240
    // 0x80182228: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
        goto L_80182240;
    // 0x80182228: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_8018222C:
    // 0x8018222C: jal         0x8017B470
    // 0x80182230: nop

    func_8017B470(rdram, ctx);
        goto after_11;
    // 0x80182230: nop

    after_11:
    // 0x80182234: beq         $v0, $s5, L_80182240
    if (ctx->r2 == ctx->r21) {
        // 0x80182238: nop
    
            goto L_80182240;
    }
    // 0x80182238: nop

    // 0x8018223C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80182240:
    // 0x80182240: bnel        $s1, $s5, L_80182264
    if (ctx->r17 != ctx->r21) {
        // 0x80182244: lw          $t9, 0x60($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X60);
            goto L_80182264;
    }
    goto skip_0;
    // 0x80182244: lw          $t9, 0x60($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X60);
    skip_0:
    // 0x80182248: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
    // 0x8018224C: slt         $at, $s6, $v0
    ctx->r1 = SIGNED(ctx->r22) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80182250: beql        $at, $zero, L_80182264
    if (ctx->r1 == 0) {
        // 0x80182254: lw          $t9, 0x60($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X60);
            goto L_80182264;
    }
    goto skip_1;
    // 0x80182254: lw          $t9, 0x60($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X60);
    skip_1:
    // 0x80182258: or          $s6, $v0, $zero
    ctx->r22 = ctx->r2 | 0;
    // 0x8018225C: sw          $s0, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r16;
    // 0x80182260: lw          $t9, 0x60($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X60);
L_80182264:
    // 0x80182264: addiu       $s0, $s0, 0x2C
    ctx->r16 = ADD32(ctx->r16, 0X2C);
    // 0x80182268: bnel        $s0, $t9, L_801820D8
    if (ctx->r16 != ctx->r25) {
        // 0x8018226C: lbu         $t7, 0x8($s0)
        ctx->r15 = MEM_BU(ctx->r16, 0X8);
            goto L_801820D8;
    }
    goto skip_2;
    // 0x8018226C: lbu         $t7, 0x8($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X8);
    skip_2:
L_80182270:
    // 0x80182270: lw          $v0, 0x44($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X44);
L_80182274:
    // 0x80182274: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80182278: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8018227C: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80182280: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80182284: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80182288: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8018228C: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80182290: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80182294: jr          $ra
    // 0x80182298: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x80182298: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_8018229C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018229C: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x801822A0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801822A4: ldc1        $f10, -0x3838($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X3838);
    // 0x801822A8: cvt.d.s     $f8, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f8.d = CVT_D_S(ctx->f14.fl);
    // 0x801822AC: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801822B0: mul.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = MUL_D(ctx->f8.d, ctx->f10.d);
    // 0x801822B4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801822B8: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x801822BC: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x801822C0: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801822C4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801822C8: ldc1        $f18, -0x3830($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, -0X3830);
    // 0x801822CC: swc1        $f4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f4.u32l;
    // 0x801822D0: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801822D4: div.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = DIV_D(ctx->f16.d, ctx->f18.d);
    // 0x801822D8: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x801822DC: swc1        $f6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f6.u32l;
    // 0x801822E0: cvt.s.d     $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f20.fl = CVT_S_D(ctx->f4.d);
    // 0x801822E4: jal         0x800A3780
    // 0x801822E8: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    __cosf_recomp(rdram, ctx);
        goto after_0;
    // 0x801822E8: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_0:
    // 0x801822EC: swc1        $f0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f0.u32l;
    // 0x801822F0: jal         0x80097170
    // 0x801822F4: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    sinf_recomp(rdram, ctx);
        goto after_1;
    // 0x801822F4: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_1:
    // 0x801822F8: lwc1        $f6, 0x2C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x801822FC: lwc1        $f8, 0x24($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X24);
    // 0x80182300: lwc1        $f16, 0x28($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80182304: lw          $t6, 0x30($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X30);
    // 0x80182308: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8018230C: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    // 0x80182310: mul.s       $f18, $f0, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f16.fl);
    // 0x80182314: add.s       $f4, $f18, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f10.fl;
    // 0x80182318: jal         0x80097170
    // 0x8018231C: swc1        $f4, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f4.u32l;
    sinf_recomp(rdram, ctx);
        goto after_2;
    // 0x8018231C: swc1        $f4, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f4.u32l;
    after_2:
    // 0x80182320: swc1        $f0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f0.u32l;
    // 0x80182324: jal         0x800A3780
    // 0x80182328: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    __cosf_recomp(rdram, ctx);
        goto after_3;
    // 0x80182328: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_3:
    // 0x8018232C: lwc1        $f6, 0x2C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80182330: lwc1        $f8, 0x24($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X24);
    // 0x80182334: lwc1        $f18, 0x28($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80182338: lw          $t7, 0x30($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X30);
    // 0x8018233C: mul.s       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80182340: nop

    // 0x80182344: mul.s       $f10, $f0, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f18.fl);
    // 0x80182348: sub.s       $f4, $f16, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x8018234C: swc1        $f4, 0x8($t7)
    MEM_W(0X8, ctx->r15) = ctx->f4.u32l;
    // 0x80182350: lw          $t8, 0x34($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X34);
    // 0x80182354: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x80182358: lwc1        $f6, 0x4($t8)
    ctx->f6.u32l = MEM_W(ctx->r24, 0X4);
    // 0x8018235C: swc1        $f6, 0x4($t9)
    MEM_W(0X4, ctx->r25) = ctx->f6.u32l;
    // 0x80182360: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80182364: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80182368: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8018236C: jr          $ra
    // 0x80182370: nop

    return;
    // 0x80182370: nop

;}
RECOMP_FUNC void func_80182374(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182374: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80182378: mtc1        $a3, $f12
    ctx->f12.u32l = ctx->r7;
    // 0x8018237C: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80182380: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80182384: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80182388: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x8018238C: beq         $a0, $zero, L_801823A4
    if (ctx->r4 == 0) {
        // 0x80182390: sw          $a2, 0x38($sp)
        MEM_W(0X38, ctx->r29) = ctx->r6;
            goto L_801823A4;
    }
    // 0x80182390: sw          $a2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r6;
    // 0x80182394: beq         $a1, $zero, L_801823A4
    if (ctx->r5 == 0) {
        // 0x80182398: nop
    
            goto L_801823A4;
    }
    // 0x80182398: nop

    // 0x8018239C: bne         $a2, $zero, L_801823AC
    if (ctx->r6 != 0) {
        // 0x801823A0: addiu       $a0, $sp, 0x24
        ctx->r4 = ADD32(ctx->r29, 0X24);
            goto L_801823AC;
    }
    // 0x801823A0: addiu       $a0, $sp, 0x24
    ctx->r4 = ADD32(ctx->r29, 0X24);
L_801823A4:
    // 0x801823A4: b           L_801824A4
    // 0x801823A8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801824A4;
    // 0x801823A8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801823AC:
    // 0x801823AC: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x801823B0: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x801823B4: jal         0x80013CC4
    // 0x801823B8: swc1        $f12, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f12.u32l;
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x801823B8: swc1        $f12, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f12.u32l;
    after_0:
    // 0x801823BC: lwc1        $f12, 0x3C($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x801823C0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x801823C4: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
    // 0x801823C8: addiu       $a0, $sp, 0x24
    ctx->r4 = ADD32(ctx->r29, 0X24);
    // 0x801823CC: c.eq.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl == ctx->f4.fl;
    // 0x801823D0: nop

    // 0x801823D4: bc1tl       L_801823F8
    if (c1cs) {
        // 0x801823D8: lwc1        $f2, 0x24($sp)
        ctx->f2.u32l = MEM_W(ctx->r29, 0X24);
            goto L_801823F8;
    }
    goto skip_0;
    // 0x801823D8: lwc1        $f2, 0x24($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X24);
    skip_0:
    // 0x801823DC: neg.s       $f6, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = -ctx->f12.fl;
    // 0x801823E0: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x801823E4: mfc1        $a2, $f6
    ctx->r6 = (int32_t)ctx->f6.u32l;
    // 0x801823E8: jal         0x8018229C
    // 0x801823EC: nop

    func_8018229C(rdram, ctx);
        goto after_1;
    // 0x801823EC: nop

    after_1:
    // 0x801823F0: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
    // 0x801823F4: lwc1        $f2, 0x24($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X24);
L_801823F8:
    // 0x801823F8: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x801823FC: nop

    // 0x80182400: c.lt.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl < ctx->f12.fl;
    // 0x80182404: nop

    // 0x80182408: bc1fl       L_8018241C
    if (!c1cs) {
        // 0x8018240C: mov.s       $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
            goto L_8018241C;
    }
    goto skip_1;
    // 0x8018240C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    skip_1:
    // 0x80182410: b           L_8018241C
    // 0x80182414: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
        goto L_8018241C;
    // 0x80182414: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x80182418: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_8018241C:
    // 0x8018241C: lwc1        $f8, 0x0($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80182420: lwc1        $f2, 0x28($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80182424: c.lt.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl < ctx->f8.fl;
    // 0x80182428: nop

    // 0x8018242C: bc1fl       L_801824A4
    if (!c1cs) {
        // 0x80182430: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_801824A4;
    }
    goto skip_2;
    // 0x80182430: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_2:
    // 0x80182434: c.lt.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl < ctx->f12.fl;
    // 0x80182438: nop

    // 0x8018243C: bc1fl       L_80182450
    if (!c1cs) {
        // 0x80182440: mov.s       $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
            goto L_80182450;
    }
    goto skip_3;
    // 0x80182440: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    skip_3:
    // 0x80182444: b           L_80182450
    // 0x80182448: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
        goto L_80182450;
    // 0x80182448: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x8018244C: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_80182450:
    // 0x80182450: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80182454: lwc1        $f2, 0x2C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80182458: c.lt.s      $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f0.fl < ctx->f10.fl;
    // 0x8018245C: nop

    // 0x80182460: bc1fl       L_801824A4
    if (!c1cs) {
        // 0x80182464: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_801824A4;
    }
    goto skip_4;
    // 0x80182464: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_4:
    // 0x80182468: c.lt.s      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.fl < ctx->f12.fl;
    // 0x8018246C: nop

    // 0x80182470: bc1fl       L_80182484
    if (!c1cs) {
        // 0x80182474: mov.s       $f0, $f2
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
            goto L_80182484;
    }
    goto skip_5;
    // 0x80182474: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    skip_5:
    // 0x80182478: b           L_80182484
    // 0x8018247C: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
        goto L_80182484;
    // 0x8018247C: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x80182480: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_80182484:
    // 0x80182484: lwc1        $f16, 0x8($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80182488: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x8018248C: nop

    // 0x80182490: bc1fl       L_801824A4
    if (!c1cs) {
        // 0x80182494: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_801824A4;
    }
    goto skip_6;
    // 0x80182494: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_6:
    // 0x80182498: b           L_801824A4
    // 0x8018249C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_801824A4;
    // 0x8018249C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x801824A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801824A4:
    // 0x801824A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801824A8: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801824AC: jr          $ra
    // 0x801824B0: nop

    return;
    // 0x801824B0: nop

;}
RECOMP_FUNC void func_801824B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801824B4: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x801824B8: beq         $a0, $zero, L_801824E0
    if (ctx->r4 == 0) {
        // 0x801824BC: nop
    
            goto L_801824E0;
    }
    // 0x801824BC: nop

    // 0x801824C0: beq         $a1, $zero, L_801824E0
    if (ctx->r5 == 0) {
        // 0x801824C4: nop
    
            goto L_801824E0;
    }
    // 0x801824C4: nop

    // 0x801824C8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x801824CC: nop

    // 0x801824D0: c.le.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl <= ctx->f4.fl;
    // 0x801824D4: nop

    // 0x801824D8: bc1fl       L_801824EC
    if (!c1cs) {
        // 0x801824DC: lwc1        $f6, 0x8($a0)
        ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
            goto L_801824EC;
    }
    goto skip_0;
    // 0x801824DC: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    skip_0:
L_801824E0:
    // 0x801824E0: jr          $ra
    // 0x801824E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x801824E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801824E8: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
L_801824EC:
    // 0x801824EC: lwc1        $f8, 0x8($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801824F0: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801824F4: lwc1        $f16, 0x0($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801824F8: sub.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x801824FC: lwc1        $f18, 0x4($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80182500: lwc1        $f4, 0x4($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80182504: sub.s       $f2, $f10, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x80182508: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x8018250C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80182510: sub.s       $f14, $f18, $f4
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f14.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x80182514: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80182518: nop

    // 0x8018251C: mul.s       $f10, $f14, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80182520: add.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80182524: mul.s       $f18, $f0, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80182528: add.s       $f4, $f18, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f16.fl;
    // 0x8018252C: c.le.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl <= ctx->f6.fl;
    // 0x80182530: nop

    // 0x80182534: bc1f        L_80182544
    if (!c1cs) {
        // 0x80182538: nop
    
            goto L_80182544;
    }
    // 0x80182538: nop

    // 0x8018253C: jr          $ra
    // 0x80182540: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80182540: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80182544:
    // 0x80182544: jr          $ra
    // 0x80182548: nop

    return;
    // 0x80182548: nop

;}
RECOMP_FUNC void func_8018254C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018254C: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80182550: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x80182554: beq         $a0, $zero, L_8018258C
    if (ctx->r4 == 0) {
        // 0x80182558: nop
    
            goto L_8018258C;
    }
    // 0x80182558: nop

    // 0x8018255C: beq         $a1, $zero, L_8018258C
    if (ctx->r5 == 0) {
        // 0x80182560: nop
    
            goto L_8018258C;
    }
    // 0x80182560: nop

    // 0x80182564: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80182568: nop

    // 0x8018256C: c.le.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl <= ctx->f0.fl;
    // 0x80182570: nop

    // 0x80182574: bc1t        L_8018258C
    if (c1cs) {
        // 0x80182578: nop
    
            goto L_8018258C;
    }
    // 0x80182578: nop

    // 0x8018257C: c.le.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl <= ctx->f0.fl;
    // 0x80182580: nop

    // 0x80182584: bc1fl       L_80182598
    if (!c1cs) {
        // 0x80182588: lwc1        $f0, 0x4($a0)
        ctx->f0.u32l = MEM_W(ctx->r4, 0X4);
            goto L_80182598;
    }
    goto skip_0;
    // 0x80182588: lwc1        $f0, 0x4($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X4);
    skip_0:
L_8018258C:
    // 0x8018258C: jr          $ra
    // 0x80182590: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80182590: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80182594: lwc1        $f0, 0x4($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X4);
L_80182598:
    // 0x80182598: lwc1        $f2, 0x4($a1)
    ctx->f2.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8018259C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801825A0: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x801825A4: nop

    // 0x801825A8: bc1fl       L_801825C0
    if (!c1cs) {
        // 0x801825AC: sub.s       $f16, $f0, $f2
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
            goto L_801825C0;
    }
    goto skip_1;
    // 0x801825AC: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
    skip_1:
    // 0x801825B0: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x801825B4: b           L_801825C0
    // 0x801825B8: neg.s       $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = -ctx->f16.fl;
        goto L_801825C0;
    // 0x801825B8: neg.s       $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = -ctx->f16.fl;
    // 0x801825BC: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
L_801825C0:
    // 0x801825C0: c.lt.s      $f16, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f16.fl < ctx->f14.fl;
    // 0x801825C4: nop

    // 0x801825C8: bc1f        L_80182614
    if (!c1cs) {
        // 0x801825CC: nop
    
            goto L_80182614;
    }
    // 0x801825CC: nop

    // 0x801825D0: lwc1        $f4, 0x8($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X8);
    // 0x801825D4: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801825D8: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801825DC: lwc1        $f10, 0x0($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X0);
    // 0x801825E0: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801825E4: mul.s       $f18, $f12, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x801825E8: sub.s       $f2, $f8, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x801825EC: mul.s       $f4, $f0, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x801825F0: nop

    // 0x801825F4: mul.s       $f6, $f2, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801825F8: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x801825FC: c.le.s      $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f8.fl <= ctx->f18.fl;
    // 0x80182600: nop

    // 0x80182604: bc1f        L_80182614
    if (!c1cs) {
        // 0x80182608: nop
    
            goto L_80182614;
    }
    // 0x80182608: nop

    // 0x8018260C: jr          $ra
    // 0x80182610: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80182610: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80182614:
    // 0x80182614: jr          $ra
    // 0x80182618: nop

    return;
    // 0x80182618: nop

;}
RECOMP_FUNC void func_8018261C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018261C: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80182620: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x80182624: beq         $a0, $zero, L_8018265C
    if (ctx->r4 == 0) {
        // 0x80182628: nop
    
            goto L_8018265C;
    }
    // 0x80182628: nop

    // 0x8018262C: beq         $a1, $zero, L_8018265C
    if (ctx->r5 == 0) {
        // 0x80182630: nop
    
            goto L_8018265C;
    }
    // 0x80182630: nop

    // 0x80182634: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80182638: nop

    // 0x8018263C: c.eq.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl == ctx->f0.fl;
    // 0x80182640: nop

    // 0x80182644: bc1t        L_8018265C
    if (c1cs) {
        // 0x80182648: nop
    
            goto L_8018265C;
    }
    // 0x80182648: nop

    // 0x8018264C: c.eq.s      $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f14.fl == ctx->f0.fl;
    // 0x80182650: nop

    // 0x80182654: bc1fl       L_80182668
    if (!c1cs) {
        // 0x80182658: lwc1        $f0, 0x0($a0)
        ctx->f0.u32l = MEM_W(ctx->r4, 0X0);
            goto L_80182668;
    }
    goto skip_0;
    // 0x80182658: lwc1        $f0, 0x0($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X0);
    skip_0:
L_8018265C:
    // 0x8018265C: jr          $ra
    // 0x80182660: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80182660: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80182664: lwc1        $f0, 0x0($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X0);
L_80182668:
    // 0x80182668: lwc1        $f2, 0x0($a1)
    ctx->f2.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8018266C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80182670: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x80182674: nop

    // 0x80182678: bc1fl       L_80182690
    if (!c1cs) {
        // 0x8018267C: sub.s       $f16, $f0, $f2
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
            goto L_80182690;
    }
    goto skip_1;
    // 0x8018267C: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
    skip_1:
    // 0x80182680: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x80182684: b           L_80182690
    // 0x80182688: neg.s       $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = -ctx->f16.fl;
        goto L_80182690;
    // 0x80182688: neg.s       $f16, $f16
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f16.fl = -ctx->f16.fl;
    // 0x8018268C: sub.s       $f16, $f0, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = ctx->f0.fl - ctx->f2.fl;
L_80182690:
    // 0x80182690: c.lt.s      $f16, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f16.fl < ctx->f12.fl;
    // 0x80182694: nop

    // 0x80182698: bc1f        L_801826E0
    if (!c1cs) {
        // 0x8018269C: nop
    
            goto L_801826E0;
    }
    // 0x8018269C: nop

    // 0x801826A0: lwc1        $f0, 0x8($a0)
    ctx->f0.u32l = MEM_W(ctx->r4, 0X8);
    // 0x801826A4: lwc1        $f2, 0x8($a1)
    ctx->f2.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801826A8: c.lt.s      $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f0.fl < ctx->f2.fl;
    // 0x801826AC: nop

    // 0x801826B0: bc1fl       L_801826C8
    if (!c1cs) {
        // 0x801826B4: sub.s       $f12, $f0, $f2
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = ctx->f0.fl - ctx->f2.fl;
            goto L_801826C8;
    }
    goto skip_2;
    // 0x801826B4: sub.s       $f12, $f0, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = ctx->f0.fl - ctx->f2.fl;
    skip_2:
    // 0x801826B8: sub.s       $f12, $f0, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = ctx->f0.fl - ctx->f2.fl;
    // 0x801826BC: b           L_801826C8
    // 0x801826C0: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
        goto L_801826C8;
    // 0x801826C0: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    // 0x801826C4: sub.s       $f12, $f0, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f12.fl = ctx->f0.fl - ctx->f2.fl;
L_801826C8:
    // 0x801826C8: c.lt.s      $f12, $f14
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f12.fl < ctx->f14.fl;
    // 0x801826CC: nop

    // 0x801826D0: bc1f        L_801826E0
    if (!c1cs) {
        // 0x801826D4: nop
    
            goto L_801826E0;
    }
    // 0x801826D4: nop

    // 0x801826D8: jr          $ra
    // 0x801826DC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x801826DC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801826E0:
    // 0x801826E0: jr          $ra
    // 0x801826E4: nop

    return;
    // 0x801826E4: nop

;}
RECOMP_FUNC void func_801826E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801826E8: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x801826EC: beq         $a0, $zero, L_80182714
    if (ctx->r4 == 0) {
        // 0x801826F0: nop
    
            goto L_80182714;
    }
    // 0x801826F0: nop

    // 0x801826F4: beq         $a1, $zero, L_80182714
    if (ctx->r5 == 0) {
        // 0x801826F8: nop
    
            goto L_80182714;
    }
    // 0x801826F8: nop

    // 0x801826FC: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80182700: nop

    // 0x80182704: c.le.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl <= ctx->f4.fl;
    // 0x80182708: nop

    // 0x8018270C: bc1fl       L_80182720
    if (!c1cs) {
        // 0x80182710: lwc1        $f6, 0x8($a0)
        ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
            goto L_80182720;
    }
    goto skip_0;
    // 0x80182710: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    skip_0:
L_80182714:
    // 0x80182714: jr          $ra
    // 0x80182718: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80182718: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8018271C: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
L_80182720:
    // 0x80182720: lwc1        $f8, 0x8($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80182724: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80182728: lwc1        $f16, 0x0($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8018272C: sub.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80182730: mul.s       $f18, $f12, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80182734: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80182738: sub.s       $f2, $f10, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x8018273C: mul.s       $f4, $f0, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80182740: nop

    // 0x80182744: mul.s       $f6, $f2, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80182748: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x8018274C: c.le.s      $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f8.fl <= ctx->f18.fl;
    // 0x80182750: nop

    // 0x80182754: bc1f        L_80182764
    if (!c1cs) {
        // 0x80182758: nop
    
            goto L_80182764;
    }
    // 0x80182758: nop

    // 0x8018275C: jr          $ra
    // 0x80182760: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80182760: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80182764:
    // 0x80182764: jr          $ra
    // 0x80182768: nop

    return;
    // 0x80182768: nop

;}
RECOMP_FUNC void func_8018276C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018276C: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x80182770: addiu       $v1, $v1, 0x3890
    ctx->r3 = ADD32(ctx->r3, 0X3890);
    // 0x80182774: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80182778: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8018277C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80182780: andi        $t6, $v0, 0x2000
    ctx->r14 = ctx->r2 & 0X2000;
    // 0x80182784: beq         $t6, $zero, L_801827A8
    if (ctx->r14 == 0) {
        // 0x80182788: sw          $a0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r4;
            goto L_801827A8;
    }
    // 0x80182788: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8018278C: jal         0x80001FD0
    // 0x80182790: lw          $a0, 0x10($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X10);
    func_80001FD0(rdram, ctx);
        goto after_0;
    // 0x80182790: lw          $a0, 0x10($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X10);
    after_0:
    // 0x80182794: bne         $v0, $zero, L_801827CC
    if (ctx->r2 != 0) {
        // 0x80182798: lui         $v1, 0x800F
        ctx->r3 = S32(0X800F << 16);
            goto L_801827CC;
    }
    // 0x80182798: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8018279C: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x801827A0: lw          $v0, 0x3890($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X3890);
    // 0x801827A4: addiu       $v1, $v1, 0x3890
    ctx->r3 = ADD32(ctx->r3, 0X3890);
L_801827A8:
    // 0x801827A8: andi        $t7, $v0, 0x200
    ctx->r15 = ctx->r2 & 0X200;
    // 0x801827AC: beq         $t7, $zero, L_801827C4
    if (ctx->r15 == 0) {
        // 0x801827B0: nop
    
            goto L_801827C4;
    }
    // 0x801827B0: nop

    // 0x801827B4: jal         0x80001FD0
    // 0x801827B8: lw          $a0, 0x70($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X70);
    func_80001FD0(rdram, ctx);
        goto after_1;
    // 0x801827B8: lw          $a0, 0x70($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X70);
    after_1:
    // 0x801827BC: bne         $v0, $zero, L_801827CC
    if (ctx->r2 != 0) {
        // 0x801827C0: nop
    
            goto L_801827CC;
    }
    // 0x801827C0: nop

L_801827C4:
    // 0x801827C4: b           L_80182888
    // 0x801827C8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80182888;
    // 0x801827C8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801827CC:
    // 0x801827CC: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x801827D0: lw          $v1, 0x38A0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X38A0);
    // 0x801827D4: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801827D8: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x801827DC: bnel        $v1, $zero, L_80182800
    if (ctx->r3 != 0) {
        // 0x801827E0: lh          $a0, 0x28D0($a1)
        ctx->r4 = MEM_H(ctx->r5, 0X28D0);
            goto L_80182800;
    }
    goto skip_0;
    // 0x801827E0: lh          $a0, 0x28D0($a1)
    ctx->r4 = MEM_H(ctx->r5, 0X28D0);
    skip_0:
    // 0x801827E4: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x801827E8: lw          $v1, 0x3900($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X3900);
    // 0x801827EC: bnel        $v1, $zero, L_80182800
    if (ctx->r3 != 0) {
        // 0x801827F0: lh          $a0, 0x28D0($a1)
        ctx->r4 = MEM_H(ctx->r5, 0X28D0);
            goto L_80182800;
    }
    goto skip_1;
    // 0x801827F0: lh          $a0, 0x28D0($a1)
    ctx->r4 = MEM_H(ctx->r5, 0X28D0);
    skip_1:
    // 0x801827F4: b           L_80182888
    // 0x801827F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80182888;
    // 0x801827F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801827FC: lh          $a0, 0x28D0($a1)
    ctx->r4 = MEM_H(ctx->r5, 0X28D0);
L_80182800:
    // 0x80182800: addiu       $at, $zero, 0x1D
    ctx->r1 = ADD32(0, 0X1D);
    // 0x80182804: bnel        $a0, $at, L_8018284C
    if (ctx->r4 != ctx->r1) {
        // 0x80182808: addiu       $at, $zero, 0x7
        ctx->r1 = ADD32(0, 0X7);
            goto L_8018284C;
    }
    goto skip_2;
    // 0x80182808: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    skip_2:
    // 0x8018280C: lh          $v0, 0x0($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X0);
    // 0x80182810: addiu       $at, $zero, 0x239
    ctx->r1 = ADD32(0, 0X239);
    // 0x80182814: andi        $t8, $v0, 0x800
    ctx->r24 = ctx->r2 & 0X800;
    // 0x80182818: beq         $t8, $zero, L_80182848
    if (ctx->r24 == 0) {
        // 0x8018281C: andi        $t9, $v0, 0x7FF
        ctx->r25 = ctx->r2 & 0X7FF;
            goto L_80182848;
    }
    // 0x8018281C: andi        $t9, $v0, 0x7FF
    ctx->r25 = ctx->r2 & 0X7FF;
    // 0x80182820: bnel        $t9, $at, L_8018284C
    if (ctx->r25 != ctx->r1) {
        // 0x80182824: addiu       $at, $zero, 0x7
        ctx->r1 = ADD32(0, 0X7);
            goto L_8018284C;
    }
    goto skip_3;
    // 0x80182824: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    skip_3:
    // 0x80182828: lw          $t0, 0x24($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X24);
    // 0x8018282C: beql        $t0, $zero, L_8018284C
    if (ctx->r8 == 0) {
        // 0x80182830: addiu       $at, $zero, 0x7
        ctx->r1 = ADD32(0, 0X7);
            goto L_8018284C;
    }
    goto skip_4;
    // 0x80182830: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    skip_4:
    // 0x80182834: lw          $t1, 0x2B58($a1)
    ctx->r9 = MEM_W(ctx->r5, 0X2B58);
    // 0x80182838: beql        $t1, $zero, L_8018284C
    if (ctx->r9 == 0) {
        // 0x8018283C: addiu       $at, $zero, 0x7
        ctx->r1 = ADD32(0, 0X7);
            goto L_8018284C;
    }
    goto skip_5;
    // 0x8018283C: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    skip_5:
    // 0x80182840: b           L_80182888
    // 0x80182844: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
        goto L_80182888;
    // 0x80182844: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80182848:
    // 0x80182848: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
L_8018284C:
    // 0x8018284C: bnel        $a0, $at, L_80182888
    if (ctx->r4 != ctx->r1) {
        // 0x80182850: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80182888;
    }
    goto skip_6;
    // 0x80182850: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_6:
    // 0x80182854: lh          $v0, 0x0($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X0);
    // 0x80182858: addiu       $at, $zero, 0x1FE
    ctx->r1 = ADD32(0, 0X1FE);
    // 0x8018285C: andi        $t2, $v0, 0x800
    ctx->r10 = ctx->r2 & 0X800;
    // 0x80182860: beq         $t2, $zero, L_80182884
    if (ctx->r10 == 0) {
        // 0x80182864: andi        $t3, $v0, 0x7FF
        ctx->r11 = ctx->r2 & 0X7FF;
            goto L_80182884;
    }
    // 0x80182864: andi        $t3, $v0, 0x7FF
    ctx->r11 = ctx->r2 & 0X7FF;
    // 0x80182868: bnel        $t3, $at, L_80182888
    if (ctx->r11 != ctx->r1) {
        // 0x8018286C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80182888;
    }
    goto skip_7;
    // 0x8018286C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_7:
    // 0x80182870: lw          $t4, 0x24($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X24);
    // 0x80182874: beql        $t4, $zero, L_80182888
    if (ctx->r12 == 0) {
        // 0x80182878: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80182888;
    }
    goto skip_8;
    // 0x80182878: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_8:
    // 0x8018287C: b           L_80182888
    // 0x80182880: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
        goto L_80182888;
    // 0x80182880: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80182884:
    // 0x80182884: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80182888:
    // 0x80182888: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8018288C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80182890: jr          $ra
    // 0x80182894: nop

    return;
    // 0x80182894: nop

;}
RECOMP_FUNC void func_80182898(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182898: bne         $a0, $zero, L_801828A8
    if (ctx->r4 != 0) {
        // 0x8018289C: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_801828A8;
    }
    // 0x8018289C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x801828A0: jr          $ra
    // 0x801828A4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x801828A4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801828A8:
    // 0x801828A8: lw          $v0, 0x584($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X584);
    // 0x801828AC: jr          $ra
    // 0x801828B0: nop

    return;
    // 0x801828B0: nop

;}
RECOMP_FUNC void func_801828B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801828B4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801828B8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801828BC: beq         $a0, $zero, L_80182A04
    if (ctx->r4 == 0) {
        // 0x801828C0: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_80182A04;
    }
    // 0x801828C0: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x801828C4: lw          $a1, 0x100($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X100);
    // 0x801828C8: bnel        $a1, $zero, L_801828E0
    if (ctx->r5 != 0) {
        // 0x801828CC: lw          $t7, 0x584($v1)
        ctx->r15 = MEM_W(ctx->r3, 0X584);
            goto L_801828E0;
    }
    goto skip_0;
    // 0x801828CC: lw          $t7, 0x584($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X584);
    skip_0:
    // 0x801828D0: lw          $t6, 0x584($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X584);
    // 0x801828D4: beql        $t6, $zero, L_80182A08
    if (ctx->r14 == 0) {
        // 0x801828D8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80182A08;
    }
    goto skip_1;
    // 0x801828D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x801828DC: lw          $t7, 0x584($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X584);
L_801828E0:
    // 0x801828E0: beql        $t7, $zero, L_80182908
    if (ctx->r15 == 0) {
        // 0x801828E4: lw          $t1, 0x4($a1)
        ctx->r9 = MEM_W(ctx->r5, 0X4);
            goto L_80182908;
    }
    goto skip_2;
    // 0x801828E4: lw          $t1, 0x4($a1)
    ctx->r9 = MEM_W(ctx->r5, 0X4);
    skip_2:
    // 0x801828E8: lw          $t9, 0x10($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X10);
    // 0x801828EC: addiu       $t8, $zero, 0x3
    ctx->r24 = ADD32(0, 0X3);
    // 0x801828F0: lw          $t0, 0x34($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X34);
    // 0x801828F4: jal         0x8017D7C0
    // 0x801828F8: sw          $t8, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r24;
    func_8017D7C0(rdram, ctx);
        goto after_0;
    // 0x801828F8: sw          $t8, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r24;
    after_0:
    // 0x801828FC: b           L_80182A08
    // 0x80182900: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x80182900: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80182904: lw          $t1, 0x4($a1)
    ctx->r9 = MEM_W(ctx->r5, 0X4);
L_80182908:
    // 0x80182908: sll         $t2, $t1, 3
    ctx->r10 = S32(ctx->r9 << 3);
    // 0x8018290C: addu        $t2, $t2, $t1
    ctx->r10 = ADD32(ctx->r10, ctx->r9);
    // 0x80182910: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x80182914: addu        $t3, $v1, $t2
    ctx->r11 = ADD32(ctx->r3, ctx->r10);
    // 0x80182918: lbu         $v0, 0x123($t3)
    ctx->r2 = MEM_BU(ctx->r11, 0X123);
    // 0x8018291C: sltiu       $at, $v0, 0x15
    ctx->r1 = ctx->r2 < 0X15 ? 1 : 0;
    // 0x80182920: beq         $at, $zero, L_80182A04
    if (ctx->r1 == 0) {
        // 0x80182924: sll         $t4, $v0, 2
        ctx->r12 = S32(ctx->r2 << 2);
            goto L_80182A04;
    }
    // 0x80182924: sll         $t4, $v0, 2
    ctx->r12 = S32(ctx->r2 << 2);
    // 0x80182928: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018292C: addu        $at, $at, $t4
    gpr jr_addend_80182934 = ctx->r12;
    ctx->r1 = ADD32(ctx->r1, ctx->r12);
    // 0x80182930: lw          $t4, -0x3828($at)
    ctx->r12 = ADD32(ctx->r1, -0X3828);
    // 0x80182934: jr          $t4
    // 0x80182938: nop

    switch (jr_addend_80182934 >> 2) {
        case 0: goto L_80182A04; break;
        case 1: goto L_8018293C; break;
        case 2: goto L_801829AC; break;
        case 3: goto L_80182958; break;
        case 4: goto L_80182958; break;
        case 5: goto L_801829BC; break;
        case 6: goto L_801829CC; break;
        case 7: goto L_801829DC; break;
        case 8: goto L_80182974; break;
        case 9: goto L_80182990; break;
        case 10: goto L_80182990; break;
        case 11: goto L_801829EC; break;
        case 12: goto L_80182990; break;
        case 13: goto L_80182990; break;
        case 14: goto L_80182990; break;
        case 15: goto L_801829DC; break;
        case 16: goto L_801829FC; break;
        case 17: goto L_80182974; break;
        case 18: goto L_8018293C; break;
        case 19: goto L_80182974; break;
        case 20: goto L_80182974; break;
        default: switch_error(__func__, 0x80182934, 0x8019C7D8);
    }
    // 0x80182938: nop

L_8018293C:
    // 0x8018293C: lw          $t6, 0x10($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X10);
    // 0x80182940: addiu       $t5, $zero, 0x3
    ctx->r13 = ADD32(0, 0X3);
    // 0x80182944: lw          $t7, 0x34($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X34);
    // 0x80182948: jal         0x8017C1CC
    // 0x8018294C: sw          $t5, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->r13;
    func_8017C1CC(rdram, ctx);
        goto after_1;
    // 0x8018294C: sw          $t5, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->r13;
    after_1:
    // 0x80182950: b           L_80182A08
    // 0x80182954: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x80182954: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80182958:
    // 0x80182958: lw          $t8, 0x10($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X10);
    // 0x8018295C: addiu       $t9, $zero, 0x3
    ctx->r25 = ADD32(0, 0X3);
    // 0x80182960: lw          $t0, 0x34($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X34);
    // 0x80182964: jal         0x8017C9B8
    // 0x80182968: sw          $t9, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r25;
    func_8017C9B8(rdram, ctx);
        goto after_2;
    // 0x80182968: sw          $t9, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r25;
    after_2:
    // 0x8018296C: b           L_80182A08
    // 0x80182970: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x80182970: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80182974:
    // 0x80182974: lw          $t2, 0x10($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X10);
    // 0x80182978: addiu       $t1, $zero, 0x3
    ctx->r9 = ADD32(0, 0X3);
    // 0x8018297C: lw          $t3, 0x34($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X34);
    // 0x80182980: jal         0x8017CBFC
    // 0x80182984: sw          $t1, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r9;
    func_8017CBFC(rdram, ctx);
        goto after_3;
    // 0x80182984: sw          $t1, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->r9;
    after_3:
    // 0x80182988: b           L_80182A08
    // 0x8018298C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x8018298C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80182990:
    // 0x80182990: lw          $t6, 0x10($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X10);
    // 0x80182994: addiu       $t4, $zero, 0x5
    ctx->r12 = ADD32(0, 0X5);
    // 0x80182998: lw          $t5, 0x34($t6)
    ctx->r13 = MEM_W(ctx->r14, 0X34);
    // 0x8018299C: jal         0x8017CBFC
    // 0x801829A0: sw          $t4, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->r12;
    func_8017CBFC(rdram, ctx);
        goto after_4;
    // 0x801829A0: sw          $t4, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->r12;
    after_4:
    // 0x801829A4: b           L_80182A08
    // 0x801829A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x801829A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801829AC:
    // 0x801829AC: jal         0x8017C27C
    // 0x801829B0: nop

    func_8017C27C(rdram, ctx);
        goto after_5;
    // 0x801829B0: nop

    after_5:
    // 0x801829B4: b           L_80182A08
    // 0x801829B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x801829B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801829BC:
    // 0x801829BC: jal         0x8017C390
    // 0x801829C0: nop

    func_8017C390(rdram, ctx);
        goto after_6;
    // 0x801829C0: nop

    after_6:
    // 0x801829C4: b           L_80182A08
    // 0x801829C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x801829C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801829CC:
    // 0x801829CC: jal         0x8017C5AC
    // 0x801829D0: nop

    func_8017C5AC(rdram, ctx);
        goto after_7;
    // 0x801829D0: nop

    after_7:
    // 0x801829D4: b           L_80182A08
    // 0x801829D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x801829D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801829DC:
    // 0x801829DC: jal         0x8017C734
    // 0x801829E0: nop

    func_8017C734(rdram, ctx);
        goto after_8;
    // 0x801829E0: nop

    after_8:
    // 0x801829E4: b           L_80182A08
    // 0x801829E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x801829E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801829EC:
    // 0x801829EC: jal         0x8017D378
    // 0x801829F0: nop

    func_8017D378(rdram, ctx);
        goto after_9;
    // 0x801829F0: nop

    after_9:
    // 0x801829F4: b           L_80182A08
    // 0x801829F8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80182A08;
    // 0x801829F8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801829FC:
    // 0x801829FC: jal         0x8017D524
    // 0x80182A00: nop

    func_8017D524(rdram, ctx);
        goto after_10;
    // 0x80182A00: nop

    after_10:
L_80182A04:
    // 0x80182A04: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80182A08:
    // 0x80182A08: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80182A0C: jr          $ra
    // 0x80182A10: nop

    return;
    // 0x80182A10: nop

;}
RECOMP_FUNC void func_80182A20(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182A20: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80182A24: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80182A28: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x80182A2C: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x80182A30: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x80182A34: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x80182A38: lw          $t1, 0x34($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X34);
    // 0x80182A3C: beq         $a0, $zero, L_80182CE0
    if (ctx->r4 == 0) {
        // 0x80182A40: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_80182CE0;
    }
    // 0x80182A40: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80182A44: beql        $a1, $zero, L_80182CE4
    if (ctx->r5 == 0) {
        // 0x80182A48: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80182CE4;
    }
    goto skip_0;
    // 0x80182A48: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_0:
    // 0x80182A4C: lw          $s0, 0x0($a1)
    ctx->r16 = MEM_W(ctx->r5, 0X0);
    // 0x80182A50: beql        $s0, $zero, L_80182CE4
    if (ctx->r16 == 0) {
        // 0x80182A54: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80182CE4;
    }
    goto skip_1;
    // 0x80182A54: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_1:
    // 0x80182A58: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
    // 0x80182A5C: addiu       $t0, $zero, 0x24
    ctx->r8 = ADD32(0, 0X24);
    // 0x80182A60: beq         $v0, $zero, L_80182CE0
    if (ctx->r2 == 0) {
        // 0x80182A64: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80182CE0;
    }
    // 0x80182A64: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80182A68: sltiu       $at, $a0, 0x15
    ctx->r1 = ctx->r4 < 0X15 ? 1 : 0;
L_80182A6C:
    // 0x80182A6C: beq         $at, $zero, L_80182A88
    if (ctx->r1 == 0) {
        // 0x80182A70: sll         $t6, $a0, 2
        ctx->r14 = S32(ctx->r4 << 2);
            goto L_80182A88;
    }
    // 0x80182A70: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x80182A74: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80182A78: addu        $at, $at, $t6
    gpr jr_addend_80182A80 = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x80182A7C: lw          $t6, -0x3798($at)
    ctx->r14 = ADD32(ctx->r1, -0X3798);
    // 0x80182A80: jr          $t6
    // 0x80182A84: nop

    switch (jr_addend_80182A80 >> 2) {
        case 0: goto L_80182A88; break;
        case 1: goto L_80182AD0; break;
        case 2: goto L_80182B60; break;
        case 3: goto L_80182A90; break;
        case 4: goto L_80182A90; break;
        case 5: goto L_80182BCC; break;
        case 6: goto L_80182C38; break;
        case 7: goto L_80182C58; break;
        case 8: goto L_80182A90; break;
        case 9: goto L_80182A90; break;
        case 10: goto L_80182A90; break;
        case 11: goto L_80182C58; break;
        case 12: goto L_80182A90; break;
        case 13: goto L_80182A90; break;
        case 14: goto L_80182A90; break;
        case 15: goto L_80182C58; break;
        case 16: goto L_80182B18; break;
        case 17: goto L_80182A90; break;
        case 18: goto L_80182AD0; break;
        case 19: goto L_80182A90; break;
        case 20: goto L_80182A90; break;
        default: switch_error(__func__, 0x80182A80, 0x8019C868);
    }
    // 0x80182A84: nop

L_80182A88:
    // 0x80182A88: b           L_80182CC0
    // 0x80182A8C: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
        goto L_80182CC0;
    // 0x80182A8C: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
L_80182A90:
    // 0x80182A90: lbu         $a2, 0x1($s0)
    ctx->r6 = MEM_BU(ctx->r16, 0X1);
    // 0x80182A94: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
    // 0x80182A98: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80182A9C: bne         $a2, $zero, L_80182AAC
    if (ctx->r6 != 0) {
        // 0x80182AA0: or          $a0, $s2, $zero
        ctx->r4 = ctx->r18 | 0;
            goto L_80182AAC;
    }
    // 0x80182AA0: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x80182AA4: b           L_80182CC0
    // 0x80182AA8: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
        goto L_80182CC0;
    // 0x80182AA8: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
L_80182AAC:
    // 0x80182AAC: lw          $a3, 0x4($s0)
    ctx->r7 = MEM_W(ctx->r16, 0X4);
    // 0x80182AB0: sw          $v0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r2;
    // 0x80182AB4: lw          $t7, 0x8($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X8);
    // 0x80182AB8: sw          $t1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r9;
    // 0x80182ABC: jal         0x80182D34
    // 0x80182AC0: sw          $t7, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r15;
    func_80182D34(rdram, ctx);
        goto after_0;
    // 0x80182AC0: sw          $t7, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r15;
    after_0:
    // 0x80182AC4: addiu       $t0, $zero, 0x24
    ctx->r8 = ADD32(0, 0X24);
    // 0x80182AC8: b           L_80182CC0
    // 0x80182ACC: lw          $t1, 0x34($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X34);
        goto L_80182CC0;
    // 0x80182ACC: lw          $t1, 0x34($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X34);
L_80182AD0:
    // 0x80182AD0: multu       $v1, $t0
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80182AD4: lw          $t9, 0x4($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X4);
    // 0x80182AD8: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
    // 0x80182ADC: lwc1        $f4, 0x0($t9)
    ctx->f4.u32l = MEM_W(ctx->r25, 0X0);
    // 0x80182AE0: mflo        $t8
    ctx->r24 = lo;
    // 0x80182AE4: addu        $v0, $t1, $t8
    ctx->r2 = ADD32(ctx->r9, ctx->r24);
    // 0x80182AE8: swc1        $f4, 0x128($v0)
    MEM_W(0X128, ctx->r2) = ctx->f4.u32l;
    // 0x80182AEC: lw          $t2, 0x4($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X4);
    // 0x80182AF0: lwc1        $f6, 0x4($t2)
    ctx->f6.u32l = MEM_W(ctx->r10, 0X4);
    // 0x80182AF4: swc1        $f6, 0x12C($v0)
    MEM_W(0X12C, ctx->r2) = ctx->f6.u32l;
    // 0x80182AF8: lw          $t3, 0x4($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X4);
    // 0x80182AFC: lwc1        $f8, 0x8($t3)
    ctx->f8.u32l = MEM_W(ctx->r11, 0X8);
    // 0x80182B00: swc1        $f8, 0x130($v0)
    MEM_W(0X130, ctx->r2) = ctx->f8.u32l;
    // 0x80182B04: lbu         $t4, 0x0($s0)
    ctx->r12 = MEM_BU(ctx->r16, 0X0);
    // 0x80182B08: sw          $t4, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->r12;
    // 0x80182B0C: lw          $t5, 0x8($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X8);
    // 0x80182B10: b           L_80182CC0
    // 0x80182B14: sw          $t5, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r13;
        goto L_80182CC0;
    // 0x80182B14: sw          $t5, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r13;
L_80182B18:
    // 0x80182B18: multu       $v1, $t0
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80182B1C: lw          $t7, 0x4($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X4);
    // 0x80182B20: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
    // 0x80182B24: lwc1        $f10, 0x0($t7)
    ctx->f10.u32l = MEM_W(ctx->r15, 0X0);
    // 0x80182B28: mflo        $t6
    ctx->r14 = lo;
    // 0x80182B2C: addu        $v0, $t1, $t6
    ctx->r2 = ADD32(ctx->r9, ctx->r14);
    // 0x80182B30: swc1        $f10, 0x128($v0)
    MEM_W(0X128, ctx->r2) = ctx->f10.u32l;
    // 0x80182B34: lw          $t8, 0x4($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4);
    // 0x80182B38: lwc1        $f16, 0x4($t8)
    ctx->f16.u32l = MEM_W(ctx->r24, 0X4);
    // 0x80182B3C: swc1        $f16, 0x12C($v0)
    MEM_W(0X12C, ctx->r2) = ctx->f16.u32l;
    // 0x80182B40: lw          $t9, 0x4($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X4);
    // 0x80182B44: lwc1        $f18, 0x8($t9)
    ctx->f18.u32l = MEM_W(ctx->r25, 0X8);
    // 0x80182B48: swc1        $f18, 0x130($v0)
    MEM_W(0X130, ctx->r2) = ctx->f18.u32l;
    // 0x80182B4C: lbu         $t2, 0x0($s0)
    ctx->r10 = MEM_BU(ctx->r16, 0X0);
    // 0x80182B50: sw          $t2, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->r10;
    // 0x80182B54: lw          $t3, 0x8($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X8);
    // 0x80182B58: b           L_80182CC0
    // 0x80182B5C: sw          $t3, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r11;
        goto L_80182CC0;
    // 0x80182B5C: sw          $t3, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r11;
L_80182B60:
    // 0x80182B60: multu       $v1, $t0
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80182B64: lw          $t5, 0x4($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X4);
    // 0x80182B68: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
    // 0x80182B6C: lwc1        $f4, 0x0($t5)
    ctx->f4.u32l = MEM_W(ctx->r13, 0X0);
    // 0x80182B70: mflo        $t4
    ctx->r12 = lo;
    // 0x80182B74: addu        $v0, $t1, $t4
    ctx->r2 = ADD32(ctx->r9, ctx->r12);
    // 0x80182B78: swc1        $f4, 0x128($v0)
    MEM_W(0X128, ctx->r2) = ctx->f4.u32l;
    // 0x80182B7C: lw          $t6, 0x4($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X4);
    // 0x80182B80: lwc1        $f6, 0x4($t6)
    ctx->f6.u32l = MEM_W(ctx->r14, 0X4);
    // 0x80182B84: swc1        $f6, 0x12C($v0)
    MEM_W(0X12C, ctx->r2) = ctx->f6.u32l;
    // 0x80182B88: lw          $t7, 0x4($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X4);
    // 0x80182B8C: lwc1        $f8, 0x8($t7)
    ctx->f8.u32l = MEM_W(ctx->r15, 0X8);
    // 0x80182B90: swc1        $f8, 0x130($v0)
    MEM_W(0X130, ctx->r2) = ctx->f8.u32l;
    // 0x80182B94: lw          $t8, 0x4($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4);
    // 0x80182B98: lwc1        $f10, 0xC($t8)
    ctx->f10.u32l = MEM_W(ctx->r24, 0XC);
    // 0x80182B9C: swc1        $f10, 0x134($v0)
    MEM_W(0X134, ctx->r2) = ctx->f10.u32l;
    // 0x80182BA0: lw          $t9, 0x4($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X4);
    // 0x80182BA4: lwc1        $f16, 0x10($t9)
    ctx->f16.u32l = MEM_W(ctx->r25, 0X10);
    // 0x80182BA8: swc1        $f16, 0x138($v0)
    MEM_W(0X138, ctx->r2) = ctx->f16.u32l;
    // 0x80182BAC: lw          $t2, 0x4($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X4);
    // 0x80182BB0: lwc1        $f18, 0x14($t2)
    ctx->f18.u32l = MEM_W(ctx->r10, 0X14);
    // 0x80182BB4: swc1        $f18, 0x13C($v0)
    MEM_W(0X13C, ctx->r2) = ctx->f18.u32l;
    // 0x80182BB8: lbu         $t3, 0x0($s0)
    ctx->r11 = MEM_BU(ctx->r16, 0X0);
    // 0x80182BBC: sw          $t3, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->r11;
    // 0x80182BC0: lw          $t4, 0x8($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X8);
    // 0x80182BC4: b           L_80182CC0
    // 0x80182BC8: sw          $t4, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r12;
        goto L_80182CC0;
    // 0x80182BC8: sw          $t4, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r12;
L_80182BCC:
    // 0x80182BCC: multu       $v1, $t0
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80182BD0: lw          $t6, 0x4($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X4);
    // 0x80182BD4: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
    // 0x80182BD8: lwc1        $f4, 0x0($t6)
    ctx->f4.u32l = MEM_W(ctx->r14, 0X0);
    // 0x80182BDC: mflo        $t5
    ctx->r13 = lo;
    // 0x80182BE0: addu        $v0, $t1, $t5
    ctx->r2 = ADD32(ctx->r9, ctx->r13);
    // 0x80182BE4: swc1        $f4, 0x128($v0)
    MEM_W(0X128, ctx->r2) = ctx->f4.u32l;
    // 0x80182BE8: lw          $t7, 0x4($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X4);
    // 0x80182BEC: lwc1        $f6, 0x4($t7)
    ctx->f6.u32l = MEM_W(ctx->r15, 0X4);
    // 0x80182BF0: swc1        $f6, 0x12C($v0)
    MEM_W(0X12C, ctx->r2) = ctx->f6.u32l;
    // 0x80182BF4: lw          $t8, 0x4($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4);
    // 0x80182BF8: lwc1        $f8, 0x8($t8)
    ctx->f8.u32l = MEM_W(ctx->r24, 0X8);
    // 0x80182BFC: swc1        $f8, 0x130($v0)
    MEM_W(0X130, ctx->r2) = ctx->f8.u32l;
    // 0x80182C00: lw          $t9, 0x4($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X4);
    // 0x80182C04: lwc1        $f10, 0xC($t9)
    ctx->f10.u32l = MEM_W(ctx->r25, 0XC);
    // 0x80182C08: swc1        $f10, 0x134($v0)
    MEM_W(0X134, ctx->r2) = ctx->f10.u32l;
    // 0x80182C0C: lw          $t2, 0x4($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X4);
    // 0x80182C10: lwc1        $f16, 0x10($t2)
    ctx->f16.u32l = MEM_W(ctx->r10, 0X10);
    // 0x80182C14: swc1        $f16, 0x138($v0)
    MEM_W(0X138, ctx->r2) = ctx->f16.u32l;
    // 0x80182C18: lw          $t3, 0x4($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X4);
    // 0x80182C1C: lwc1        $f18, 0x14($t3)
    ctx->f18.u32l = MEM_W(ctx->r11, 0X14);
    // 0x80182C20: swc1        $f18, 0x13C($v0)
    MEM_W(0X13C, ctx->r2) = ctx->f18.u32l;
    // 0x80182C24: lbu         $t4, 0x0($s0)
    ctx->r12 = MEM_BU(ctx->r16, 0X0);
    // 0x80182C28: sw          $t4, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->r12;
    // 0x80182C2C: lw          $t5, 0x8($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X8);
    // 0x80182C30: b           L_80182CC0
    // 0x80182C34: sw          $t5, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r13;
        goto L_80182CC0;
    // 0x80182C34: sw          $t5, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r13;
L_80182C38:
    // 0x80182C38: multu       $v1, $t0
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80182C3C: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
    // 0x80182C40: mflo        $t6
    ctx->r14 = lo;
    // 0x80182C44: addu        $v0, $t1, $t6
    ctx->r2 = ADD32(ctx->r9, ctx->r14);
    // 0x80182C48: sw          $a0, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->r4;
    // 0x80182C4C: lw          $t7, 0x8($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X8);
    // 0x80182C50: b           L_80182CC0
    // 0x80182C54: sw          $t7, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r15;
        goto L_80182CC0;
    // 0x80182C54: sw          $t7, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r15;
L_80182C58:
    // 0x80182C58: multu       $v1, $t0
    result = U64(U32(ctx->r3)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80182C5C: lw          $t9, 0x4($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X4);
    // 0x80182C60: addiu       $s1, $v1, 0x1
    ctx->r17 = ADD32(ctx->r3, 0X1);
    // 0x80182C64: lwc1        $f4, 0x0($t9)
    ctx->f4.u32l = MEM_W(ctx->r25, 0X0);
    // 0x80182C68: mflo        $t8
    ctx->r24 = lo;
    // 0x80182C6C: addu        $v0, $t1, $t8
    ctx->r2 = ADD32(ctx->r9, ctx->r24);
    // 0x80182C70: swc1        $f4, 0x128($v0)
    MEM_W(0X128, ctx->r2) = ctx->f4.u32l;
    // 0x80182C74: lw          $t2, 0x4($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X4);
    // 0x80182C78: lwc1        $f6, 0x4($t2)
    ctx->f6.u32l = MEM_W(ctx->r10, 0X4);
    // 0x80182C7C: swc1        $f6, 0x12C($v0)
    MEM_W(0X12C, ctx->r2) = ctx->f6.u32l;
    // 0x80182C80: lw          $t3, 0x4($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X4);
    // 0x80182C84: lwc1        $f8, 0x8($t3)
    ctx->f8.u32l = MEM_W(ctx->r11, 0X8);
    // 0x80182C88: swc1        $f8, 0x130($v0)
    MEM_W(0X130, ctx->r2) = ctx->f8.u32l;
    // 0x80182C8C: lw          $t4, 0x4($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X4);
    // 0x80182C90: lwc1        $f10, 0xC($t4)
    ctx->f10.u32l = MEM_W(ctx->r12, 0XC);
    // 0x80182C94: swc1        $f10, 0x134($v0)
    MEM_W(0X134, ctx->r2) = ctx->f10.u32l;
    // 0x80182C98: lw          $t5, 0x4($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X4);
    // 0x80182C9C: lwc1        $f16, 0x10($t5)
    ctx->f16.u32l = MEM_W(ctx->r13, 0X10);
    // 0x80182CA0: swc1        $f16, 0x138($v0)
    MEM_W(0X138, ctx->r2) = ctx->f16.u32l;
    // 0x80182CA4: lw          $t6, 0x4($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X4);
    // 0x80182CA8: lwc1        $f18, 0x14($t6)
    ctx->f18.u32l = MEM_W(ctx->r14, 0X14);
    // 0x80182CAC: swc1        $f18, 0x13C($v0)
    MEM_W(0X13C, ctx->r2) = ctx->f18.u32l;
    // 0x80182CB0: lbu         $t7, 0x0($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X0);
    // 0x80182CB4: sw          $t7, 0x144($v0)
    MEM_W(0X144, ctx->r2) = ctx->r15;
    // 0x80182CB8: lw          $t8, 0x8($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X8);
    // 0x80182CBC: sw          $t8, 0x148($v0)
    MEM_W(0X148, ctx->r2) = ctx->r24;
L_80182CC0:
    // 0x80182CC0: lbu         $v0, 0xC($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0XC);
    // 0x80182CC4: addiu       $s0, $s0, 0xC
    ctx->r16 = ADD32(ctx->r16, 0XC);
    // 0x80182CC8: or          $v1, $s1, $zero
    ctx->r3 = ctx->r17 | 0;
    // 0x80182CCC: beq         $v0, $zero, L_80182CE0
    if (ctx->r2 == 0) {
        // 0x80182CD0: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80182CE0;
    }
    // 0x80182CD0: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80182CD4: slti        $at, $s1, 0x1F
    ctx->r1 = SIGNED(ctx->r17) < 0X1F ? 1 : 0;
    // 0x80182CD8: bnel        $at, $zero, L_80182A6C
    if (ctx->r1 != 0) {
        // 0x80182CDC: sltiu       $at, $a0, 0x15
        ctx->r1 = ctx->r4 < 0X15 ? 1 : 0;
            goto L_80182A6C;
    }
    goto skip_2;
    // 0x80182CDC: sltiu       $at, $a0, 0x15
    ctx->r1 = ctx->r4 < 0X15 ? 1 : 0;
    skip_2:
L_80182CE0:
    // 0x80182CE0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80182CE4:
    // 0x80182CE4: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80182CE8: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x80182CEC: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x80182CF0: jr          $ra
    // 0x80182CF4: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x80182CF4: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_80182CF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182CF8: beq         $a0, $zero, L_80182D0C
    if (ctx->r4 == 0) {
        // 0x80182CFC: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_80182D0C;
    }
    // 0x80182CFC: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80182D00: lw          $a0, 0x100($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X100);
    // 0x80182D04: bnel        $a0, $zero, L_80182D18
    if (ctx->r4 != 0) {
        // 0x80182D08: lw          $t6, 0x4($a0)
        ctx->r14 = MEM_W(ctx->r4, 0X4);
            goto L_80182D18;
    }
    goto skip_0;
    // 0x80182D08: lw          $t6, 0x4($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X4);
    skip_0:
L_80182D0C:
    // 0x80182D0C: jr          $ra
    // 0x80182D10: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80182D10: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80182D14: lw          $t6, 0x4($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X4);
L_80182D18:
    // 0x80182D18: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x80182D1C: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80182D20: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80182D24: addu        $t8, $v1, $t7
    ctx->r24 = ADD32(ctx->r3, ctx->r15);
    // 0x80182D28: lbu         $v0, 0x123($t8)
    ctx->r2 = MEM_BU(ctx->r24, 0X123);
    // 0x80182D2C: jr          $ra
    // 0x80182D30: nop

    return;
    // 0x80182D30: nop

;}
RECOMP_FUNC void func_80182D34(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182D34: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80182D38: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80182D3C: beq         $a0, $zero, L_80182DA4
    if (ctx->r4 == 0) {
        // 0x80182D40: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_80182DA4;
    }
    // 0x80182D40: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80182D44: sltiu       $at, $a1, 0x1F
    ctx->r1 = ctx->r5 < 0X1F ? 1 : 0;
    // 0x80182D48: beql        $at, $zero, L_80182DA8
    if (ctx->r1 == 0) {
        // 0x80182D4C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80182DA8;
    }
    goto skip_0;
    // 0x80182D4C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80182D50: bltz        $a1, L_80182DA4
    if (SIGNED(ctx->r5) < 0) {
        // 0x80182D54: sll         $t6, $a1, 3
        ctx->r14 = S32(ctx->r5 << 3);
            goto L_80182DA4;
    }
    // 0x80182D54: sll         $t6, $a1, 3
    ctx->r14 = S32(ctx->r5 << 3);
    // 0x80182D58: addu        $t6, $t6, $a1
    ctx->r14 = ADD32(ctx->r14, ctx->r5);
    // 0x80182D5C: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80182D60: addu        $v0, $v1, $t6
    ctx->r2 = ADD32(ctx->r3, ctx->r14);
    // 0x80182D64: lui         $at, 0x3F00
    ctx->r1 = S32(0X3F00 << 16);
    // 0x80182D68: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80182D6C: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x80182D70: sb          $t7, 0x104($v0)
    MEM_B(0X104, ctx->r2) = ctx->r15;
    // 0x80182D74: sh          $a2, 0x10C($v0)
    MEM_H(0X10C, ctx->r2) = ctx->r6;
    // 0x80182D78: sw          $a3, 0x110($v0)
    MEM_W(0X110, ctx->r2) = ctx->r7;
    // 0x80182D7C: swc1        $f4, 0x108($v0)
    MEM_W(0X108, ctx->r2) = ctx->f4.u32l;
    // 0x80182D80: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    // 0x80182D84: jal         0x80183734
    // 0x80182D88: addiu       $a0, $v0, 0x104
    ctx->r4 = ADD32(ctx->r2, 0X104);
    func_80183734(rdram, ctx);
        goto after_0;
    // 0x80182D88: addiu       $a0, $v0, 0x104
    ctx->r4 = ADD32(ctx->r2, 0X104);
    after_0:
    // 0x80182D8C: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80182D90: swc1        $f0, 0x11C($v0)
    MEM_W(0X11C, ctx->r2) = ctx->f0.u32l;
    // 0x80182D94: lbu         $t8, 0x33($sp)
    ctx->r24 = MEM_BU(ctx->r29, 0X33);
    // 0x80182D98: sw          $t8, 0x120($v0)
    MEM_W(0X120, ctx->r2) = ctx->r24;
    // 0x80182D9C: lw          $t9, 0x34($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X34);
    // 0x80182DA0: sw          $t9, 0x124($v0)
    MEM_W(0X124, ctx->r2) = ctx->r25;
L_80182DA4:
    // 0x80182DA4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80182DA8:
    // 0x80182DA8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80182DAC: jr          $ra
    // 0x80182DB0: nop

    return;
    // 0x80182DB0: nop

;}
RECOMP_FUNC void func_80182DC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182DC0: mtc1        $a3, $f12
    ctx->f12.u32l = ctx->r7;
    // 0x80182DC4: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x80182DC8: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80182DCC: mul.s       $f0, $f12, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80182DD0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80182DD4: mul.s       $f2, $f0, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x80182DD8: add.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f0.fl + ctx->f0.fl;
    // 0x80182DDC: sub.s       $f4, $f2, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x80182DE0: sub.s       $f10, $f8, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80182DE4: mul.s       $f6, $f4, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x80182DE8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80182DEC: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x80182DF0: sub.s       $f4, $f10, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f12.fl;
    // 0x80182DF4: swc1        $f6, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f6.u32l;
    // 0x80182DF8: sub.s       $f10, $f8, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f14.fl;
    // 0x80182DFC: mul.s       $f16, $f4, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f16.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x80182E00: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80182E04: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80182E08: mul.s       $f4, $f10, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80182E0C: sub.s       $f10, $f14, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f14.fl - ctx->f8.fl;
    // 0x80182E10: mul.s       $f8, $f10, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80182E14: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80182E18: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80182E1C: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x80182E20: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80182E24: add.s       $f18, $f10, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80182E28: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80182E2C: add.s       $f10, $f14, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f14.fl + ctx->f14.fl;
    // 0x80182E30: sub.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80182E34: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80182E38: mul.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80182E3C: sub.s       $f4, $f14, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f14.fl - ctx->f10.fl;
    // 0x80182E40: mul.s       $f10, $f4, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80182E44: add.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80182E48: mul.s       $f10, $f14, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f12.fl);
    // 0x80182E4C: add.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x80182E50: swc1        $f8, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->f8.u32l;
    // 0x80182E54: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80182E58: mul.s       $f10, $f4, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x80182E5C: lwc1        $f4, 0xC($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0XC);
    // 0x80182E60: mul.s       $f4, $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x80182E64: add.s       $f10, $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80182E68: lwc1        $f4, 0x18($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X18);
    // 0x80182E6C: mul.s       $f4, $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x80182E70: add.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80182E74: lwc1        $f10, 0x24($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X24);
    // 0x80182E78: mul.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x80182E7C: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80182E80: swc1        $f10, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f10.u32l;
    // 0x80182E84: lwc1        $f6, 0x4($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80182E88: lwc1        $f8, 0x10($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X10);
    // 0x80182E8C: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80182E90: nop

    // 0x80182E94: mul.s       $f10, $f8, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x80182E98: lwc1        $f8, 0x1C($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X1C);
    // 0x80182E9C: add.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x80182EA0: lwc1        $f4, 0x8($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X8);
    // 0x80182EA4: mul.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x80182EA8: lwc1        $f4, 0x28($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X28);
    // 0x80182EAC: add.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x80182EB0: lwc1        $f6, 0x0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X0);
    // 0x80182EB4: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80182EB8: add.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80182EBC: swc1        $f4, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f4.u32l;
    // 0x80182EC0: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80182EC4: lwc1        $f8, 0x14($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X14);
    // 0x80182EC8: mul.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80182ECC: nop

    // 0x80182ED0: mul.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x80182ED4: lwc1        $f8, 0x20($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X20);
    // 0x80182ED8: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80182EDC: lwc1        $f10, 0x8($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X8);
    // 0x80182EE0: mul.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80182EE4: lwc1        $f10, 0x2C($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X2C);
    // 0x80182EE8: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x80182EEC: lwc1        $f6, 0x0($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X0);
    // 0x80182EF0: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80182EF4: mul.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x80182EF8: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80182EFC: jr          $ra
    // 0x80182F00: swc1        $f10, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f10.u32l;
    return;
    // 0x80182F00: swc1        $f10, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f10.u32l;
;}
RECOMP_FUNC void func_80182F04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80182F04: mtc1        $a3, $f12
    ctx->f12.u32l = ctx->r7;
    // 0x80182F08: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x80182F0C: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80182F10: mul.s       $f0, $f12, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80182F14: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80182F18: mul.s       $f2, $f0, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x80182F1C: add.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = ctx->f0.fl + ctx->f0.fl;
    // 0x80182F20: sub.s       $f4, $f2, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = ctx->f2.fl - ctx->f0.fl;
    // 0x80182F24: sub.s       $f10, $f8, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f2.fl;
    // 0x80182F28: mul.s       $f6, $f4, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x80182F2C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80182F30: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x80182F34: sub.s       $f4, $f10, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f12.fl;
    // 0x80182F38: swc1        $f6, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->f6.u32l;
    // 0x80182F3C: sub.s       $f10, $f8, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f14.fl;
    // 0x80182F40: mul.s       $f16, $f4, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f16.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x80182F44: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80182F48: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80182F4C: mul.s       $f4, $f10, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80182F50: sub.s       $f10, $f14, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f14.fl - ctx->f8.fl;
    // 0x80182F54: mul.s       $f8, $f10, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80182F58: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80182F5C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80182F60: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x80182F64: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80182F68: add.s       $f18, $f10, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80182F6C: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80182F70: add.s       $f10, $f14, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = ctx->f14.fl + ctx->f14.fl;
    // 0x80182F74: sub.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80182F78: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80182F7C: mul.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80182F80: sub.s       $f4, $f14, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f14.fl - ctx->f10.fl;
    // 0x80182F84: mul.s       $f10, $f4, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80182F88: add.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80182F8C: mul.s       $f10, $f14, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f12.fl);
    // 0x80182F90: add.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x80182F94: swc1        $f8, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->f8.u32l;
    // 0x80182F98: lwc1        $f4, 0x0($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80182F9C: mul.s       $f10, $f4, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x80182FA0: lwc1        $f4, 0xC($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0XC);
    // 0x80182FA4: mul.s       $f4, $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x80182FA8: add.s       $f10, $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80182FAC: lwc1        $f4, 0x18($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X18);
    // 0x80182FB0: mul.s       $f4, $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x80182FB4: add.s       $f8, $f10, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80182FB8: lwc1        $f10, 0x24($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X24);
    // 0x80182FBC: mul.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x80182FC0: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80182FC4: nop

    // 0x80182FC8: swc1        $f6, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f6.u32l;
    // 0x80182FCC: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80182FD0: swc1        $f10, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f10.u32l;
    // 0x80182FD4: lwc1        $f4, 0x8($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80182FD8: lwc1        $f10, 0x14($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X14);
    // 0x80182FDC: mul.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x80182FE0: nop

    // 0x80182FE4: mul.s       $f6, $f10, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f18.fl);
    // 0x80182FE8: lwc1        $f10, 0x20($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X20);
    // 0x80182FEC: add.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80182FF0: lwc1        $f8, 0x8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X8);
    // 0x80182FF4: mul.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f8.fl);
    // 0x80182FF8: lwc1        $f8, 0x2C($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X2C);
    // 0x80182FFC: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80183000: lwc1        $f4, 0x0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X0);
    // 0x80183004: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80183008: mul.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x8018300C: add.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x80183010: jr          $ra
    // 0x80183014: swc1        $f8, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f8.u32l;
    return;
    // 0x80183014: swc1        $f8, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f8.u32l;
;}
RECOMP_FUNC void func_80183018(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183018: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x8018301C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80183020: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80183024: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80183028: c.lt.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl < ctx->f4.fl;
    // 0x8018302C: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80183030: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80183034: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183038: bc1tl       L_80183118
    if (c1cs) {
        // 0x8018303C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183118;
    }
    goto skip_0;
    // 0x8018303C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80183040: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80183044: nop

    // 0x80183048: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x8018304C: nop

    // 0x80183050: bc1tl       L_80183118
    if (c1cs) {
        // 0x80183054: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183118;
    }
    goto skip_1;
    // 0x80183054: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x80183058: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x8018305C: nop

    // 0x80183060: bc1fl       L_801830D8
    if (!c1cs) {
        // 0x80183064: lh          $v1, 0x8($s0)
        ctx->r3 = MEM_H(ctx->r16, 0X8);
            goto L_801830D8;
    }
    goto skip_2;
    // 0x80183064: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
    skip_2:
    // 0x80183068: lh          $t6, 0x8($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X8);
    // 0x8018306C: lw          $t1, 0xC($a1)
    ctx->r9 = MEM_W(ctx->r5, 0XC);
    // 0x80183070: lw          $a2, 0x4($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X4);
    // 0x80183074: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x80183078: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x8018307C: nop

    // 0x80183080: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80183084: mul.s       $f12, $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f8.fl);
    // 0x80183088: trunc.w.s   $f10, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    ctx->f10.u32l = TRUNC_W_S(ctx->f12.fl);
    // 0x8018308C: mfc1        $v0, $f10
    ctx->r2 = (int32_t)ctx->f10.u32l;
    // 0x80183090: nop

    // 0x80183094: sll         $t9, $v0, 16
    ctx->r25 = S32(ctx->r2 << 16);
    // 0x80183098: sra         $t0, $t9, 16
    ctx->r8 = S32(SIGNED(ctx->r25) >> 16);
    // 0x8018309C: mtc1        $t0, $f16
    ctx->f16.u32l = ctx->r8;
    // 0x801830A0: sll         $v1, $v0, 16
    ctx->r3 = S32(ctx->r2 << 16);
    // 0x801830A4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801830A8: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801830AC: sll         $t2, $v1, 2
    ctx->r10 = S32(ctx->r3 << 2);
    // 0x801830B0: subu        $t2, $t2, $v1
    ctx->r10 = SUB32(ctx->r10, ctx->r3);
    // 0x801830B4: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x801830B8: addu        $a1, $t1, $t2
    ctx->r5 = ADD32(ctx->r9, ctx->r10);
    // 0x801830BC: sub.s       $f12, $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f12.fl - ctx->f18.fl;
    // 0x801830C0: mfc1        $a3, $f12
    ctx->r7 = (int32_t)ctx->f12.u32l;
    // 0x801830C4: jal         0x80182DC0
    // 0x801830C8: nop

    func_80182DC0(rdram, ctx);
        goto after_0;
    // 0x801830C8: nop

    after_0:
    // 0x801830CC: b           L_80183118
    // 0x801830D0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80183118;
    // 0x801830D0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801830D4: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
L_801830D8:
    // 0x801830D8: lw          $t3, 0xC($s0)
    ctx->r11 = MEM_W(ctx->r16, 0XC);
    // 0x801830DC: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x801830E0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801830E4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801830E8: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x801830EC: subu        $t4, $t4, $v1
    ctx->r12 = SUB32(ctx->r12, ctx->r3);
    // 0x801830F0: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x801830F4: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    // 0x801830F8: lw          $t6, 0xC($v0)
    ctx->r14 = MEM_W(ctx->r2, 0XC);
    // 0x801830FC: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x80183100: sw          $t6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r14;
    // 0x80183104: lw          $t5, 0x4($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X4);
    // 0x80183108: sw          $t5, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r13;
    // 0x8018310C: lw          $t6, 0x8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X8);
    // 0x80183110: sw          $t6, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r14;
    // 0x80183114: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80183118:
    // 0x80183118: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8018311C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80183120: jr          $ra
    // 0x80183124: nop

    return;
    // 0x80183124: nop

;}
RECOMP_FUNC void func_80183128(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183128: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x8018312C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80183130: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80183134: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80183138: c.lt.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl < ctx->f4.fl;
    // 0x8018313C: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80183140: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80183144: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183148: bc1tl       L_80183228
    if (c1cs) {
        // 0x8018314C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183228;
    }
    goto skip_0;
    // 0x8018314C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80183150: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80183154: nop

    // 0x80183158: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x8018315C: nop

    // 0x80183160: bc1tl       L_80183228
    if (c1cs) {
        // 0x80183164: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183228;
    }
    goto skip_1;
    // 0x80183164: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x80183168: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x8018316C: nop

    // 0x80183170: bc1fl       L_801831E8
    if (!c1cs) {
        // 0x80183174: lh          $v1, 0x8($s0)
        ctx->r3 = MEM_H(ctx->r16, 0X8);
            goto L_801831E8;
    }
    goto skip_2;
    // 0x80183174: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
    skip_2:
    // 0x80183178: lh          $t6, 0x8($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X8);
    // 0x8018317C: lw          $t1, 0xC($a1)
    ctx->r9 = MEM_W(ctx->r5, 0XC);
    // 0x80183180: lw          $a2, 0x4($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X4);
    // 0x80183184: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x80183188: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x8018318C: nop

    // 0x80183190: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80183194: mul.s       $f12, $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f8.fl);
    // 0x80183198: trunc.w.s   $f10, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    ctx->f10.u32l = TRUNC_W_S(ctx->f12.fl);
    // 0x8018319C: mfc1        $v0, $f10
    ctx->r2 = (int32_t)ctx->f10.u32l;
    // 0x801831A0: nop

    // 0x801831A4: sll         $t9, $v0, 16
    ctx->r25 = S32(ctx->r2 << 16);
    // 0x801831A8: sra         $t0, $t9, 16
    ctx->r8 = S32(SIGNED(ctx->r25) >> 16);
    // 0x801831AC: mtc1        $t0, $f16
    ctx->f16.u32l = ctx->r8;
    // 0x801831B0: sll         $v1, $v0, 16
    ctx->r3 = S32(ctx->r2 << 16);
    // 0x801831B4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801831B8: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801831BC: sll         $t2, $v1, 2
    ctx->r10 = S32(ctx->r3 << 2);
    // 0x801831C0: subu        $t2, $t2, $v1
    ctx->r10 = SUB32(ctx->r10, ctx->r3);
    // 0x801831C4: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x801831C8: addu        $a1, $t1, $t2
    ctx->r5 = ADD32(ctx->r9, ctx->r10);
    // 0x801831CC: sub.s       $f12, $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f12.fl - ctx->f18.fl;
    // 0x801831D0: mfc1        $a3, $f12
    ctx->r7 = (int32_t)ctx->f12.u32l;
    // 0x801831D4: jal         0x80182F04
    // 0x801831D8: nop

    func_80182F04(rdram, ctx);
        goto after_0;
    // 0x801831D8: nop

    after_0:
    // 0x801831DC: b           L_80183228
    // 0x801831E0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80183228;
    // 0x801831E0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801831E4: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
L_801831E8:
    // 0x801831E8: lw          $t3, 0xC($s0)
    ctx->r11 = MEM_W(ctx->r16, 0XC);
    // 0x801831EC: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x801831F0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801831F4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801831F8: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x801831FC: subu        $t4, $t4, $v1
    ctx->r12 = SUB32(ctx->r12, ctx->r3);
    // 0x80183200: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x80183204: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    // 0x80183208: lw          $t6, 0xC($v0)
    ctx->r14 = MEM_W(ctx->r2, 0XC);
    // 0x8018320C: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x80183210: sw          $t6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r14;
    // 0x80183214: lw          $t5, 0x4($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X4);
    // 0x80183218: sw          $t5, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r13;
    // 0x8018321C: lw          $t6, 0x8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X8);
    // 0x80183220: sw          $t6, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r14;
    // 0x80183224: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80183228:
    // 0x80183228: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8018322C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80183230: jr          $ra
    // 0x80183234: nop

    return;
    // 0x80183234: nop

;}
RECOMP_FUNC void func_80183238(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183238: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x8018323C: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x80183240: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80183244: mul.s       $f0, $f14, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80183248: add.s       $f8, $f14, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = ctx->f14.fl + ctx->f14.fl;
    // 0x8018324C: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80183250: lui         $at, 0xC040
    ctx->r1 = S32(0XC040 << 16);
    // 0x80183254: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80183258: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x8018325C: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80183260: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80183264: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x80183268: mul.s       $f4, $f10, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f12.fl);
    // 0x8018326C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80183270: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183274: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80183278: nop

    // 0x8018327C: mul.s       $f6, $f10, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f14.fl);
    // 0x80183280: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    // 0x80183284: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183288: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8018328C: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x80183290: sub.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f8.fl;
    // 0x80183294: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80183298: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x8018329C: mul.s       $f2, $f6, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = MUL_S(ctx->f6.fl, ctx->f12.fl);
    // 0x801832A0: sub.s       $f8, $f12, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f12.fl - ctx->f10.fl;
    // 0x801832A4: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801832A8: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x801832AC: sub.s       $f10, $f6, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f12.fl;
    // 0x801832B0: swc1        $f8, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->f8.u32l;
    // 0x801832B4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801832B8: nop

    // 0x801832BC: mul.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f8.fl);
    // 0x801832C0: lwc1        $f8, 0x8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X8);
    // 0x801832C4: mul.s       $f10, $f6, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x801832C8: add.s       $f6, $f8, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f8.fl;
    // 0x801832CC: mul.s       $f8, $f6, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x801832D0: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801832D4: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x801832D8: add.s       $f16, $f10, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x801832DC: add.s       $f10, $f12, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = ctx->f12.fl + ctx->f12.fl;
    // 0x801832E0: sub.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f6.fl - ctx->f10.fl;
    // 0x801832E4: add.s       $f6, $f8, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f8.fl;
    // 0x801832E8: swc1        $f8, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f8.u32l;
    // 0x801832EC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801832F0: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x801832F4: mul.s       $f10, $f6, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x801832F8: sub.s       $f6, $f12, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f12.fl - ctx->f8.fl;
    // 0x801832FC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80183300: nop

    // 0x80183304: mul.s       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80183308: nop

    // 0x8018330C: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80183310: add.s       $f6, $f8, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80183314: lwc1        $f8, 0x24($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X24);
    // 0x80183318: add.s       $f18, $f6, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = ctx->f6.fl + ctx->f12.fl;
    // 0x8018331C: mul.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x80183320: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80183324: lwc1        $f4, 0xC($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0XC);
    // 0x80183328: mul.s       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8018332C: nop

    // 0x80183330: mul.s       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f16.fl);
    // 0x80183334: add.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183338: lwc1        $f8, 0x18($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X18);
    // 0x8018333C: mul.s       $f6, $f8, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f18.fl);
    // 0x80183340: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80183344: add.s       $f4, $f10, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80183348: swc1        $f4, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f4.u32l;
    // 0x8018334C: lwc1        $f10, 0x10($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80183350: lwc1        $f6, 0x28($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X28);
    // 0x80183354: lwc1        $f4, 0x4($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80183358: mul.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8018335C: lwc1        $f10, 0x10($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X10);
    // 0x80183360: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80183364: nop

    // 0x80183368: mul.s       $f4, $f10, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x8018336C: add.s       $f10, $f6, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x80183370: lwc1        $f6, 0x1C($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X1C);
    // 0x80183374: mul.s       $f4, $f6, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f18.fl);
    // 0x80183378: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8018337C: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183380: swc1        $f10, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f10.u32l;
    // 0x80183384: lwc1        $f8, 0x10($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80183388: lwc1        $f4, 0x2C($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X2C);
    // 0x8018338C: lwc1        $f10, 0x8($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80183390: mul.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x80183394: lwc1        $f8, 0x14($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X14);
    // 0x80183398: mul.s       $f4, $f10, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x8018339C: nop

    // 0x801833A0: mul.s       $f10, $f8, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f16.fl);
    // 0x801833A4: add.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x801833A8: lwc1        $f4, 0x20($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X20);
    // 0x801833AC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801833B0: mul.s       $f10, $f4, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x801833B4: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x801833B8: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x801833BC: jr          $ra
    // 0x801833C0: swc1        $f8, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f8.u32l;
    return;
    // 0x801833C0: swc1        $f8, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f8.u32l;
;}
RECOMP_FUNC void func_801833C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801833C4: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x801833C8: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x801833CC: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x801833D0: mul.s       $f0, $f14, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x801833D4: add.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = ctx->f14.fl + ctx->f14.fl;
    // 0x801833D8: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x801833DC: lui         $at, 0xC040
    ctx->r1 = S32(0XC040 << 16);
    // 0x801833E0: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801833E4: sub.s       $f18, $f12, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = ctx->f12.fl - ctx->f2.fl;
    // 0x801833E8: mul.s       $f4, $f2, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f0.fl);
    // 0x801833EC: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801833F0: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801833F4: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x801833F8: mul.s       $f10, $f8, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f12.fl);
    // 0x801833FC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80183400: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183404: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80183408: nop

    // 0x8018340C: mul.s       $f4, $f8, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f14.fl);
    // 0x80183410: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    // 0x80183414: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x80183418: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8018341C: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80183420: sub.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl - ctx->f6.fl;
    // 0x80183424: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80183428: nop

    // 0x8018342C: sub.s       $f6, $f8, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f12.fl;
    // 0x80183430: mul.s       $f16, $f4, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x80183434: nop

    // 0x80183438: mul.s       $f4, $f6, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x8018343C: add.s       $f6, $f18, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f18.fl;
    // 0x80183440: mul.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80183444: nop

    // 0x80183448: mul.s       $f4, $f6, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x8018344C: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80183450: add.s       $f8, $f12, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f12.fl + ctx->f12.fl;
    // 0x80183454: swc1        $f6, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->f6.u32l;
    // 0x80183458: sub.s       $f4, $f2, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f2.fl - ctx->f8.fl;
    // 0x8018345C: swc1        $f4, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->f4.u32l;
    // 0x80183460: lwc1        $f8, 0x4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4);
    // 0x80183464: add.s       $f4, $f8, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f8.fl;
    // 0x80183468: mul.s       $f8, $f4, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x8018346C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80183470: nop

    // 0x80183474: sub.s       $f4, $f12, $f4
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f4.fl = ctx->f12.fl - ctx->f4.fl;
    // 0x80183478: mul.s       $f4, $f4, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x8018347C: nop

    // 0x80183480: mul.s       $f4, $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80183484: add.s       $f4, $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80183488: add.s       $f8, $f4, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f12.fl;
    // 0x8018348C: swc1        $f8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->f8.u32l;
    // 0x80183490: lwc1        $f4, 0xC($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0XC);
    // 0x80183494: mul.s       $f4, $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80183498: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x8018349C: mul.s       $f6, $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x801834A0: add.s       $f6, $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x801834A4: lwc1        $f4, 0x18($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X18);
    // 0x801834A8: mul.s       $f4, $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x801834AC: add.s       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x801834B0: lwc1        $f6, 0x24($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X24);
    // 0x801834B4: mul.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x801834B8: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801834BC: nop

    // 0x801834C0: swc1        $f10, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f10.u32l;
    // 0x801834C4: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801834C8: swc1        $f6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f6.u32l;
    // 0x801834CC: lwc1        $f8, 0x8($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X8);
    // 0x801834D0: lwc1        $f4, 0x14($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X14);
    // 0x801834D4: lwc1        $f10, 0x8($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X8);
    // 0x801834D8: mul.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x801834DC: nop

    // 0x801834E0: mul.s       $f4, $f10, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x801834E4: lwc1        $f10, 0x20($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X20);
    // 0x801834E8: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x801834EC: lwc1        $f4, 0x14($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X14);
    // 0x801834F0: mul.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x801834F4: lwc1        $f4, 0x2C($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X2C);
    // 0x801834F8: add.s       $f10, $f8, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x801834FC: lwc1        $f8, 0x10($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X10);
    // 0x80183500: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80183504: mul.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x80183508: add.s       $f4, $f6, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x8018350C: jr          $ra
    // 0x80183510: swc1        $f4, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f4.u32l;
    return;
    // 0x80183510: swc1        $f4, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f4.u32l;
;}
RECOMP_FUNC void func_80183514(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183514: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80183518: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8018351C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80183520: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80183524: c.lt.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl < ctx->f4.fl;
    // 0x80183528: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x8018352C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80183530: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183534: bc1tl       L_80183614
    if (c1cs) {
        // 0x80183538: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183614;
    }
    goto skip_0;
    // 0x80183538: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8018353C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80183540: nop

    // 0x80183544: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80183548: nop

    // 0x8018354C: bc1tl       L_80183614
    if (c1cs) {
        // 0x80183550: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183614;
    }
    goto skip_1;
    // 0x80183550: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x80183554: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x80183558: nop

    // 0x8018355C: bc1fl       L_801835D4
    if (!c1cs) {
        // 0x80183560: lh          $v1, 0x8($s0)
        ctx->r3 = MEM_H(ctx->r16, 0X8);
            goto L_801835D4;
    }
    goto skip_2;
    // 0x80183560: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
    skip_2:
    // 0x80183564: lh          $t6, 0x8($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X8);
    // 0x80183568: lw          $t1, 0xC($a1)
    ctx->r9 = MEM_W(ctx->r5, 0XC);
    // 0x8018356C: lw          $a2, 0x4($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X4);
    // 0x80183570: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x80183574: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x80183578: nop

    // 0x8018357C: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80183580: mul.s       $f12, $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f8.fl);
    // 0x80183584: trunc.w.s   $f10, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    ctx->f10.u32l = TRUNC_W_S(ctx->f12.fl);
    // 0x80183588: mfc1        $v0, $f10
    ctx->r2 = (int32_t)ctx->f10.u32l;
    // 0x8018358C: nop

    // 0x80183590: sll         $t9, $v0, 16
    ctx->r25 = S32(ctx->r2 << 16);
    // 0x80183594: sra         $t0, $t9, 16
    ctx->r8 = S32(SIGNED(ctx->r25) >> 16);
    // 0x80183598: mtc1        $t0, $f16
    ctx->f16.u32l = ctx->r8;
    // 0x8018359C: sll         $v1, $v0, 16
    ctx->r3 = S32(ctx->r2 << 16);
    // 0x801835A0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801835A4: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801835A8: sll         $t2, $v1, 2
    ctx->r10 = S32(ctx->r3 << 2);
    // 0x801835AC: subu        $t2, $t2, $v1
    ctx->r10 = SUB32(ctx->r10, ctx->r3);
    // 0x801835B0: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x801835B4: addu        $a1, $t1, $t2
    ctx->r5 = ADD32(ctx->r9, ctx->r10);
    // 0x801835B8: sub.s       $f12, $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f12.fl - ctx->f18.fl;
    // 0x801835BC: mfc1        $a3, $f12
    ctx->r7 = (int32_t)ctx->f12.u32l;
    // 0x801835C0: jal         0x80183238
    // 0x801835C4: nop

    func_80183238(rdram, ctx);
        goto after_0;
    // 0x801835C4: nop

    after_0:
    // 0x801835C8: b           L_80183614
    // 0x801835CC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80183614;
    // 0x801835CC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801835D0: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
L_801835D4:
    // 0x801835D4: lw          $t3, 0xC($s0)
    ctx->r11 = MEM_W(ctx->r16, 0XC);
    // 0x801835D8: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x801835DC: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801835E0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801835E4: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x801835E8: subu        $t4, $t4, $v1
    ctx->r12 = SUB32(ctx->r12, ctx->r3);
    // 0x801835EC: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x801835F0: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    // 0x801835F4: lw          $t6, 0xC($v0)
    ctx->r14 = MEM_W(ctx->r2, 0XC);
    // 0x801835F8: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x801835FC: sw          $t6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r14;
    // 0x80183600: lw          $t5, 0x4($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X4);
    // 0x80183604: sw          $t5, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r13;
    // 0x80183608: lw          $t6, 0x8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X8);
    // 0x8018360C: sw          $t6, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r14;
    // 0x80183610: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80183614:
    // 0x80183614: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80183618: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8018361C: jr          $ra
    // 0x80183620: nop

    return;
    // 0x80183620: nop

;}
RECOMP_FUNC void func_80183624(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183624: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80183628: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8018362C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80183630: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80183634: c.lt.s      $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f12.fl < ctx->f4.fl;
    // 0x80183638: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x8018363C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80183640: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183644: bc1tl       L_80183724
    if (c1cs) {
        // 0x80183648: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183724;
    }
    goto skip_0;
    // 0x80183648: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8018364C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80183650: nop

    // 0x80183654: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80183658: nop

    // 0x8018365C: bc1tl       L_80183724
    if (c1cs) {
        // 0x80183660: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80183724;
    }
    goto skip_1;
    // 0x80183660: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x80183664: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x80183668: nop

    // 0x8018366C: bc1fl       L_801836E4
    if (!c1cs) {
        // 0x80183670: lh          $v1, 0x8($s0)
        ctx->r3 = MEM_H(ctx->r16, 0X8);
            goto L_801836E4;
    }
    goto skip_2;
    // 0x80183670: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
    skip_2:
    // 0x80183674: lh          $t6, 0x8($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X8);
    // 0x80183678: lw          $t1, 0xC($a1)
    ctx->r9 = MEM_W(ctx->r5, 0XC);
    // 0x8018367C: lw          $a2, 0x4($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X4);
    // 0x80183680: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x80183684: mtc1        $t7, $f6
    ctx->f6.u32l = ctx->r15;
    // 0x80183688: nop

    // 0x8018368C: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80183690: mul.s       $f12, $f12, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f8.fl);
    // 0x80183694: trunc.w.s   $f10, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    ctx->f10.u32l = TRUNC_W_S(ctx->f12.fl);
    // 0x80183698: mfc1        $v0, $f10
    ctx->r2 = (int32_t)ctx->f10.u32l;
    // 0x8018369C: nop

    // 0x801836A0: sll         $t9, $v0, 16
    ctx->r25 = S32(ctx->r2 << 16);
    // 0x801836A4: sra         $t0, $t9, 16
    ctx->r8 = S32(SIGNED(ctx->r25) >> 16);
    // 0x801836A8: mtc1        $t0, $f16
    ctx->f16.u32l = ctx->r8;
    // 0x801836AC: sll         $v1, $v0, 16
    ctx->r3 = S32(ctx->r2 << 16);
    // 0x801836B0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801836B4: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801836B8: sll         $t2, $v1, 2
    ctx->r10 = S32(ctx->r3 << 2);
    // 0x801836BC: subu        $t2, $t2, $v1
    ctx->r10 = SUB32(ctx->r10, ctx->r3);
    // 0x801836C0: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x801836C4: addu        $a1, $t1, $t2
    ctx->r5 = ADD32(ctx->r9, ctx->r10);
    // 0x801836C8: sub.s       $f12, $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f12.fl - ctx->f18.fl;
    // 0x801836CC: mfc1        $a3, $f12
    ctx->r7 = (int32_t)ctx->f12.u32l;
    // 0x801836D0: jal         0x801833C4
    // 0x801836D4: nop

    func_801833C4(rdram, ctx);
        goto after_0;
    // 0x801836D4: nop

    after_0:
    // 0x801836D8: b           L_80183724
    // 0x801836DC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80183724;
    // 0x801836DC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801836E0: lh          $v1, 0x8($s0)
    ctx->r3 = MEM_H(ctx->r16, 0X8);
L_801836E4:
    // 0x801836E4: lw          $t3, 0xC($s0)
    ctx->r11 = MEM_W(ctx->r16, 0XC);
    // 0x801836E8: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x801836EC: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801836F0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801836F4: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x801836F8: subu        $t4, $t4, $v1
    ctx->r12 = SUB32(ctx->r12, ctx->r3);
    // 0x801836FC: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x80183700: addu        $v0, $t3, $t4
    ctx->r2 = ADD32(ctx->r11, ctx->r12);
    // 0x80183704: lw          $t6, 0xC($v0)
    ctx->r14 = MEM_W(ctx->r2, 0XC);
    // 0x80183708: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x8018370C: sw          $t6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r14;
    // 0x80183710: lw          $t5, 0x4($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X4);
    // 0x80183714: sw          $t5, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r13;
    // 0x80183718: lw          $t6, 0x8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X8);
    // 0x8018371C: sw          $t6, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r14;
    // 0x80183720: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80183724:
    // 0x80183724: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80183728: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8018372C: jr          $ra
    // 0x80183730: nop

    return;
    // 0x80183730: nop

;}
RECOMP_FUNC void func_80183734(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183734: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x80183738: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x8018373C: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x80183740: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80183744: sw          $s4, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r20;
    // 0x80183748: sw          $s3, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r19;
    // 0x8018374C: sw          $s2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r18;
    // 0x80183750: sw          $s1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r17;
    // 0x80183754: sw          $s0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r16;
    // 0x80183758: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x8018375C: lh          $s1, 0x8($a0)
    ctx->r17 = MEM_H(ctx->r4, 0X8);
    // 0x80183760: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80183764: addiu       $s4, $sp, 0x64
    ctx->r20 = ADD32(ctx->r29, 0X64);
    // 0x80183768: mfc1        $a2, $f20
    ctx->r6 = (int32_t)ctx->f20.u32l;
    // 0x8018376C: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80183770: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x80183774: jal         0x80183128
    // 0x80183778: sll         $s1, $s1, 3
    ctx->r17 = S32(ctx->r17 << 3);
    func_80183128(rdram, ctx);
        goto after_0;
    // 0x80183778: sll         $s1, $s1, 3
    ctx->r17 = S32(ctx->r17 << 3);
    after_0:
    // 0x8018377C: addiu       $s2, $s1, 0x1
    ctx->r18 = ADD32(ctx->r17, 0X1);
    // 0x80183780: slti        $at, $s2, 0x2
    ctx->r1 = SIGNED(ctx->r18) < 0X2 ? 1 : 0;
    // 0x80183784: bne         $at, $zero, L_80183808
    if (ctx->r1 != 0) {
        // 0x80183788: addiu       $s0, $zero, 0x1
        ctx->r16 = ADD32(0, 0X1);
            goto L_80183808;
    }
    // 0x80183788: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x8018378C: mtc1        $s1, $f4
    ctx->f4.u32l = ctx->r17;
    // 0x80183790: addiu       $s1, $sp, 0x58
    ctx->r17 = ADD32(ctx->r29, 0X58);
    // 0x80183794: cvt.s.w     $f22, $f4
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 4);
    ctx->f22.fl = CVT_S_W(ctx->f4.u32l);
L_80183798:
    // 0x80183798: mtc1        $s0, $f6
    ctx->f6.u32l = ctx->r16;
    // 0x8018379C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801837A0: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x801837A4: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801837A8: div.s       $f10, $f8, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = DIV_S(ctx->f8.fl, ctx->f22.fl);
    // 0x801837AC: mfc1        $a2, $f10
    ctx->r6 = (int32_t)ctx->f10.u32l;
    // 0x801837B0: jal         0x80183128
    // 0x801837B4: nop

    func_80183128(rdram, ctx);
        goto after_1;
    // 0x801837B4: nop

    after_1:
    // 0x801837B8: lwc1        $f16, 0x58($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801837BC: lwc1        $f18, 0x64($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801837C0: lwc1        $f4, 0x60($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801837C4: lwc1        $f6, 0x6C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x801837C8: sub.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x801837CC: sub.s       $f2, $f4, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801837D0: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x801837D4: nop

    // 0x801837D8: mul.s       $f10, $f2, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801837DC: jal         0x800A01E0
    // 0x801837E0: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_2;
    // 0x801837E0: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
    after_2:
    // 0x801837E4: lw          $t7, 0x0($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X0);
    // 0x801837E8: lw          $t6, 0x4($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X4);
    // 0x801837EC: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x801837F0: sw          $t7, 0x0($s4)
    MEM_W(0X0, ctx->r20) = ctx->r15;
    // 0x801837F4: lw          $t7, 0x8($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X8);
    // 0x801837F8: add.s       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f0.fl;
    // 0x801837FC: sw          $t6, 0x4($s4)
    MEM_W(0X4, ctx->r20) = ctx->r14;
    // 0x80183800: bne         $s0, $s2, L_80183798
    if (ctx->r16 != ctx->r18) {
        // 0x80183804: sw          $t7, 0x8($s4)
        MEM_W(0X8, ctx->r20) = ctx->r15;
            goto L_80183798;
    }
    // 0x80183804: sw          $t7, 0x8($s4)
    MEM_W(0X8, ctx->r20) = ctx->r15;
L_80183808:
    // 0x80183808: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x8018380C: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x80183810: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80183814: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x80183818: lw          $s0, 0x28($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X28);
    // 0x8018381C: lw          $s1, 0x2C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X2C);
    // 0x80183820: lw          $s2, 0x30($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X30);
    // 0x80183824: lw          $s3, 0x34($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X34);
    // 0x80183828: lw          $s4, 0x38($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X38);
    // 0x8018382C: jr          $ra
    // 0x80183830: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x80183830: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void func_80183834(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183834: beq         $a0, $zero, L_80183864
    if (ctx->r4 == 0) {
        // 0x80183838: lw          $v0, 0x34($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X34);
            goto L_80183864;
    }
    // 0x80183838: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x8018383C: blez        $a1, L_80183864
    if (SIGNED(ctx->r5) <= 0) {
        // 0x80183840: slti        $at, $a1, 0x20
        ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
            goto L_80183864;
    }
    // 0x80183840: slti        $at, $a1, 0x20
    ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
    // 0x80183844: beq         $at, $zero, L_80183864
    if (ctx->r1 == 0) {
        // 0x80183848: sll         $t6, $a1, 3
        ctx->r14 = S32(ctx->r5 << 3);
            goto L_80183864;
    }
    // 0x80183848: sll         $t6, $a1, 3
    ctx->r14 = S32(ctx->r5 << 3);
    // 0x8018384C: addu        $t6, $t6, $a1
    ctx->r14 = ADD32(ctx->r14, ctx->r5);
    // 0x80183850: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80183854: addu        $v1, $v0, $t6
    ctx->r3 = ADD32(ctx->r2, ctx->r14);
    // 0x80183858: lh          $t7, 0x10C($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X10C);
    // 0x8018385C: bnel        $t7, $zero, L_80183874
    if (ctx->r15 != 0) {
        // 0x80183860: lwc1        $f0, 0x11C($v1)
        ctx->f0.u32l = MEM_W(ctx->r3, 0X11C);
            goto L_80183874;
    }
    goto skip_0;
    // 0x80183860: lwc1        $f0, 0x11C($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X11C);
    skip_0:
L_80183864:
    // 0x80183864: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80183868: jr          $ra
    // 0x8018386C: nop

    return;
    // 0x8018386C: nop

    // 0x80183870: lwc1        $f0, 0x11C($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X11C);
L_80183874:
    // 0x80183874: jr          $ra
    // 0x80183878: nop

    return;
    // 0x80183878: nop

;}
RECOMP_FUNC void func_8018387C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018387C: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80183880: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80183884: mtc1        $a2, $f20
    ctx->f20.u32l = ctx->r6;
    // 0x80183888: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8018388C: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x80183890: beq         $a1, $zero, L_801838A0
    if (ctx->r5 == 0) {
        // 0x80183894: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_801838A0;
    }
    // 0x80183894: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80183898: bne         $a0, $zero, L_801838B8
    if (ctx->r4 != 0) {
        // 0x8018389C: nop
    
            goto L_801838B8;
    }
    // 0x8018389C: nop

L_801838A0:
    // 0x801838A0: lw          $t6, 0x58($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X58);
    // 0x801838A4: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x801838A8: swc1        $f20, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f20.u32l;
    // 0x801838AC: lw          $t7, 0x5C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X5C);
    // 0x801838B0: b           L_801839F4
    // 0x801838B4: swc1        $f14, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->f14.u32l;
        goto L_801839F4;
    // 0x801838B4: swc1        $f14, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->f14.u32l;
L_801838B8:
    // 0x801838B8: bltz        $a1, L_801838E0
    if (SIGNED(ctx->r5) < 0) {
        // 0x801838BC: slti        $at, $a1, 0x20
        ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
            goto L_801838E0;
    }
    // 0x801838BC: slti        $at, $a1, 0x20
    ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
    // 0x801838C0: beq         $at, $zero, L_801838E0
    if (ctx->r1 == 0) {
        // 0x801838C4: sll         $t8, $a1, 3
        ctx->r24 = S32(ctx->r5 << 3);
            goto L_801838E0;
    }
    // 0x801838C4: sll         $t8, $a1, 3
    ctx->r24 = S32(ctx->r5 << 3);
    // 0x801838C8: addu        $t8, $t8, $a1
    ctx->r24 = ADD32(ctx->r24, ctx->r5);
    // 0x801838CC: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x801838D0: addu        $v0, $v1, $t8
    ctx->r2 = ADD32(ctx->r3, ctx->r24);
    // 0x801838D4: lh          $t9, 0x10C($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X10C);
    // 0x801838D8: bnel        $t9, $zero, L_801838FC
    if (ctx->r25 != 0) {
        // 0x801838DC: mtc1        $zero, $f2
        ctx->f2.u32l = 0;
            goto L_801838FC;
    }
    goto skip_0;
    // 0x801838DC: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    skip_0:
L_801838E0:
    // 0x801838E0: lw          $t0, 0x58($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X58);
    // 0x801838E4: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x801838E8: swc1        $f20, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->f20.u32l;
    // 0x801838EC: lw          $t1, 0x5C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X5C);
    // 0x801838F0: b           L_801839F4
    // 0x801838F4: swc1        $f14, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->f14.u32l;
        goto L_801839F4;
    // 0x801838F4: swc1        $f14, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->f14.u32l;
    // 0x801838F8: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
L_801838FC:
    // 0x801838FC: nop

    // 0x80183900: c.lt.s      $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f20.fl < ctx->f2.fl;
    // 0x80183904: nop

    // 0x80183908: bc1fl       L_80183920
    if (!c1cs) {
        // 0x8018390C: lwc1        $f0, 0x11C($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
            goto L_80183920;
    }
    goto skip_1;
    // 0x8018390C: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    skip_1:
    // 0x80183910: mov.s       $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    ctx->f20.fl = ctx->f2.fl;
    // 0x80183914: b           L_80183934
    // 0x80183918: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
        goto L_80183934;
    // 0x80183918: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x8018391C: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
L_80183920:
    // 0x80183920: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x80183924: nop

    // 0x80183928: bc1fl       L_80183938
    if (!c1cs) {
        // 0x8018392C: div.s       $f4, $f20, $f0
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
            goto L_80183938;
    }
    goto skip_2;
    // 0x8018392C: div.s       $f4, $f20, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
    skip_2:
    // 0x80183930: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
L_80183934:
    // 0x80183934: div.s       $f4, $f20, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
L_80183938:
    // 0x80183938: addiu       $a1, $v0, 0x104
    ctx->r5 = ADD32(ctx->r2, 0X104);
    // 0x8018393C: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    // 0x80183940: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80183944: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    // 0x80183948: swc1        $f14, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f14.u32l;
    // 0x8018394C: mfc1        $a2, $f4
    ctx->r6 = (int32_t)ctx->f4.u32l;
    // 0x80183950: jal         0x80183128
    // 0x80183954: nop

    func_80183128(rdram, ctx);
        goto after_0;
    // 0x80183954: nop

    after_0:
    // 0x80183958: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
    // 0x8018395C: lw          $t2, 0x58($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X58);
    // 0x80183960: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x80183964: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80183968: swc1        $f6, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f6.u32l;
    // 0x8018396C: lw          $t3, 0x5C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X5C);
    // 0x80183970: lwc1        $f8, 0x40($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80183974: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80183978: swc1        $f8, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->f8.u32l;
    // 0x8018397C: lwc1        $f10, 0x11C($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x80183980: div.s       $f16, $f20, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = DIV_S(ctx->f20.fl, ctx->f10.fl);
    // 0x80183984: mfc1        $a2, $f16
    ctx->r6 = (int32_t)ctx->f16.u32l;
    // 0x80183988: jal         0x80183624
    // 0x8018398C: nop

    func_80183624(rdram, ctx);
        goto after_1;
    // 0x8018398C: nop

    after_1:
    // 0x80183990: lwc1        $f0, 0x40($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80183994: lwc1        $f2, 0x38($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80183998: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x8018399C: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    // 0x801839A0: mul.s       $f18, $f0, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x801839A4: swc1        $f0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f0.u32l;
    // 0x801839A8: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801839AC: jal         0x800A01E0
    // 0x801839B0: add.s       $f12, $f18, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f4.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_2;
    // 0x801839B0: add.s       $f12, $f18, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f18.fl + ctx->f4.fl;
    after_2:
    // 0x801839B4: lwc1        $f14, 0x54($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X54);
    // 0x801839B8: lwc1        $f8, 0x34($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X34);
    // 0x801839BC: lw          $v0, 0x58($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X58);
    // 0x801839C0: lw          $v1, 0x5C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X5C);
    // 0x801839C4: mul.s       $f10, $f8, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f14.fl);
    // 0x801839C8: lwc1        $f6, 0x0($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801839CC: div.s       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
    // 0x801839D0: add.s       $f18, $f6, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f6.fl + ctx->f16.fl;
    // 0x801839D4: swc1        $f18, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f18.u32l;
    // 0x801839D8: lwc1        $f8, 0x30($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X30);
    // 0x801839DC: lwc1        $f4, 0x0($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X0);
    // 0x801839E0: mul.s       $f10, $f8, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f14.fl);
    // 0x801839E4: div.s       $f6, $f10, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
    // 0x801839E8: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x801839EC: add.s       $f16, $f4, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x801839F0: swc1        $f16, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f16.u32l;
L_801839F4:
    // 0x801839F4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801839F8: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x801839FC: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x80183A00: jr          $ra
    // 0x80183A04: nop

    return;
    // 0x80183A04: nop

;}
RECOMP_FUNC void func_80183A08(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183A08: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80183A0C: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80183A10: mtc1        $a2, $f20
    ctx->f20.u32l = ctx->r6;
    // 0x80183A14: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80183A18: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x80183A1C: beq         $a1, $zero, L_80183A2C
    if (ctx->r5 == 0) {
        // 0x80183A20: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_80183A2C;
    }
    // 0x80183A20: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80183A24: bne         $a0, $zero, L_80183A50
    if (ctx->r4 != 0) {
        // 0x80183A28: nop
    
            goto L_80183A50;
    }
    // 0x80183A28: nop

L_80183A2C:
    // 0x80183A2C: lw          $t6, 0x58($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X58);
    // 0x80183A30: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80183A34: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x80183A38: swc1        $f20, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f20.u32l;
    // 0x80183A3C: lw          $t7, 0x5C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X5C);
    // 0x80183A40: swc1        $f2, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->f2.u32l;
    // 0x80183A44: lw          $t8, 0x60($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X60);
    // 0x80183A48: b           L_80183BA4
    // 0x80183A4C: swc1        $f14, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->f14.u32l;
        goto L_80183BA4;
    // 0x80183A4C: swc1        $f14, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->f14.u32l;
L_80183A50:
    // 0x80183A50: bltz        $a1, L_80183A78
    if (SIGNED(ctx->r5) < 0) {
        // 0x80183A54: slti        $at, $a1, 0x20
        ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
            goto L_80183A78;
    }
    // 0x80183A54: slti        $at, $a1, 0x20
    ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
    // 0x80183A58: beq         $at, $zero, L_80183A78
    if (ctx->r1 == 0) {
        // 0x80183A5C: sll         $t9, $a1, 3
        ctx->r25 = S32(ctx->r5 << 3);
            goto L_80183A78;
    }
    // 0x80183A5C: sll         $t9, $a1, 3
    ctx->r25 = S32(ctx->r5 << 3);
    // 0x80183A60: addu        $t9, $t9, $a1
    ctx->r25 = ADD32(ctx->r25, ctx->r5);
    // 0x80183A64: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x80183A68: addu        $v0, $v1, $t9
    ctx->r2 = ADD32(ctx->r3, ctx->r25);
    // 0x80183A6C: lh          $t0, 0x10C($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X10C);
    // 0x80183A70: bnel        $t0, $zero, L_80183AA0
    if (ctx->r8 != 0) {
        // 0x80183A74: mtc1        $zero, $f2
        ctx->f2.u32l = 0;
            goto L_80183AA0;
    }
    goto skip_0;
    // 0x80183A74: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    skip_0:
L_80183A78:
    // 0x80183A78: lw          $t1, 0x58($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X58);
    // 0x80183A7C: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80183A80: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x80183A84: swc1        $f20, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->f20.u32l;
    // 0x80183A88: lw          $t2, 0x5C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X5C);
    // 0x80183A8C: swc1        $f2, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f2.u32l;
    // 0x80183A90: lw          $t3, 0x60($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X60);
    // 0x80183A94: b           L_80183BA4
    // 0x80183A98: swc1        $f14, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->f14.u32l;
        goto L_80183BA4;
    // 0x80183A98: swc1        $f14, 0x0($t3)
    MEM_W(0X0, ctx->r11) = ctx->f14.u32l;
    // 0x80183A9C: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
L_80183AA0:
    // 0x80183AA0: nop

    // 0x80183AA4: c.lt.s      $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f20.fl < ctx->f2.fl;
    // 0x80183AA8: nop

    // 0x80183AAC: bc1fl       L_80183AC4
    if (!c1cs) {
        // 0x80183AB0: lwc1        $f0, 0x11C($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
            goto L_80183AC4;
    }
    goto skip_1;
    // 0x80183AB0: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    skip_1:
    // 0x80183AB4: mov.s       $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    ctx->f20.fl = ctx->f2.fl;
    // 0x80183AB8: b           L_80183AD8
    // 0x80183ABC: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
        goto L_80183AD8;
    // 0x80183ABC: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x80183AC0: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
L_80183AC4:
    // 0x80183AC4: c.lt.s      $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    c1cs = ctx->f0.fl < ctx->f20.fl;
    // 0x80183AC8: nop

    // 0x80183ACC: bc1fl       L_80183ADC
    if (!c1cs) {
        // 0x80183AD0: div.s       $f4, $f20, $f0
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
            goto L_80183ADC;
    }
    goto skip_2;
    // 0x80183AD0: div.s       $f4, $f20, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
    skip_2:
    // 0x80183AD4: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
L_80183AD8:
    // 0x80183AD8: div.s       $f4, $f20, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f20.fl, ctx->f0.fl);
L_80183ADC:
    // 0x80183ADC: addiu       $a1, $v0, 0x104
    ctx->r5 = ADD32(ctx->r2, 0X104);
    // 0x80183AE0: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    // 0x80183AE4: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80183AE8: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    // 0x80183AEC: swc1        $f14, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f14.u32l;
    // 0x80183AF0: mfc1        $a2, $f4
    ctx->r6 = (int32_t)ctx->f4.u32l;
    // 0x80183AF4: jal         0x80183018
    // 0x80183AF8: nop

    func_80183018(rdram, ctx);
        goto after_0;
    // 0x80183AF8: nop

    after_0:
    // 0x80183AFC: lwc1        $f6, 0x38($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80183B00: lw          $t4, 0x58($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X58);
    // 0x80183B04: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x80183B08: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80183B0C: swc1        $f6, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->f6.u32l;
    // 0x80183B10: lw          $t5, 0x5C($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X5C);
    // 0x80183B14: lwc1        $f8, 0x3C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x80183B18: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80183B1C: swc1        $f8, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->f8.u32l;
    // 0x80183B20: lw          $t6, 0x60($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X60);
    // 0x80183B24: lwc1        $f10, 0x40($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80183B28: swc1        $f10, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->f10.u32l;
    // 0x80183B2C: lwc1        $f16, 0x11C($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x80183B30: div.s       $f18, $f20, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = DIV_S(ctx->f20.fl, ctx->f16.fl);
    // 0x80183B34: mfc1        $a2, $f18
    ctx->r6 = (int32_t)ctx->f18.u32l;
    // 0x80183B38: jal         0x80183514
    // 0x80183B3C: nop

    func_80183514(rdram, ctx);
        goto after_1;
    // 0x80183B3C: nop

    after_1:
    // 0x80183B40: lwc1        $f0, 0x40($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80183B44: lwc1        $f2, 0x38($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80183B48: neg.s       $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = -ctx->f0.fl;
    // 0x80183B4C: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    // 0x80183B50: mul.s       $f4, $f0, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80183B54: swc1        $f0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f0.u32l;
    // 0x80183B58: mul.s       $f6, $f2, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80183B5C: jal         0x800A01E0
    // 0x80183B60: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_2;
    // 0x80183B60: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    after_2:
    // 0x80183B64: lwc1        $f14, 0x54($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80183B68: lwc1        $f10, 0x34($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80183B6C: lw          $v0, 0x58($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X58);
    // 0x80183B70: lw          $v1, 0x60($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X60);
    // 0x80183B74: mul.s       $f16, $f10, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f14.fl);
    // 0x80183B78: lwc1        $f8, 0x0($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80183B7C: div.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f18.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x80183B80: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x80183B84: swc1        $f4, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f4.u32l;
    // 0x80183B88: lwc1        $f10, 0x30($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80183B8C: lwc1        $f6, 0x0($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X0);
    // 0x80183B90: mul.s       $f16, $f10, $f14
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f14.fl);
    // 0x80183B94: div.s       $f8, $f16, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x80183B98: mov.s       $f0, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 20);
    ctx->f0.fl = ctx->f20.fl;
    // 0x80183B9C: add.s       $f18, $f6, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80183BA0: swc1        $f18, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->f18.u32l;
L_80183BA4:
    // 0x80183BA4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80183BA8: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80183BAC: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x80183BB0: jr          $ra
    // 0x80183BB4: nop

    return;
    // 0x80183BB4: nop

;}
RECOMP_FUNC void func_80183BB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183BB8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80183BBC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80183BC0: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80183BC4: beq         $a1, $zero, L_80183BD4
    if (ctx->r5 == 0) {
        // 0x80183BC8: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_80183BD4;
    }
    // 0x80183BC8: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80183BCC: bne         $a0, $zero, L_80183BE0
    if (ctx->r4 != 0) {
        // 0x80183BD0: nop
    
            goto L_80183BE0;
    }
    // 0x80183BD0: nop

L_80183BD4:
    // 0x80183BD4: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80183BD8: b           L_80183C8C
    // 0x80183BDC: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
        goto L_80183C8C;
    // 0x80183BDC: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
L_80183BE0:
    // 0x80183BE0: bltz        $a1, L_80183C08
    if (SIGNED(ctx->r5) < 0) {
        // 0x80183BE4: slti        $at, $a1, 0x20
        ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
            goto L_80183C08;
    }
    // 0x80183BE4: slti        $at, $a1, 0x20
    ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
    // 0x80183BE8: beq         $at, $zero, L_80183C08
    if (ctx->r1 == 0) {
        // 0x80183BEC: sll         $t6, $a1, 3
        ctx->r14 = S32(ctx->r5 << 3);
            goto L_80183C08;
    }
    // 0x80183BEC: sll         $t6, $a1, 3
    ctx->r14 = S32(ctx->r5 << 3);
    // 0x80183BF0: addu        $t6, $t6, $a1
    ctx->r14 = ADD32(ctx->r14, ctx->r5);
    // 0x80183BF4: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80183BF8: addu        $v0, $v1, $t6
    ctx->r2 = ADD32(ctx->r3, ctx->r14);
    // 0x80183BFC: lh          $t7, 0x10C($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X10C);
    // 0x80183C00: bnel        $t7, $zero, L_80183C18
    if (ctx->r15 != 0) {
        // 0x80183C04: mtc1        $zero, $f2
        ctx->f2.u32l = 0;
            goto L_80183C18;
    }
    goto skip_0;
    // 0x80183C04: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    skip_0:
L_80183C08:
    // 0x80183C08: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80183C0C: b           L_80183C8C
    // 0x80183C10: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
        goto L_80183C8C;
    // 0x80183C10: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    // 0x80183C14: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
L_80183C18:
    // 0x80183C18: nop

    // 0x80183C1C: c.lt.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl < ctx->f2.fl;
    // 0x80183C20: nop

    // 0x80183C24: bc1fl       L_80183C3C
    if (!c1cs) {
        // 0x80183C28: lwc1        $f0, 0x11C($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
            goto L_80183C3C;
    }
    goto skip_1;
    // 0x80183C28: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    skip_1:
    // 0x80183C2C: mov.s       $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    ctx->f12.fl = ctx->f2.fl;
    // 0x80183C30: b           L_80183C50
    // 0x80183C34: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
        goto L_80183C50;
    // 0x80183C34: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x80183C38: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
L_80183C3C:
    // 0x80183C3C: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80183C40: nop

    // 0x80183C44: bc1fl       L_80183C54
    if (!c1cs) {
        // 0x80183C48: div.s       $f4, $f12, $f0
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f12.fl, ctx->f0.fl);
            goto L_80183C54;
    }
    goto skip_2;
    // 0x80183C48: div.s       $f4, $f12, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f12.fl, ctx->f0.fl);
    skip_2:
    // 0x80183C4C: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
L_80183C50:
    // 0x80183C50: div.s       $f4, $f12, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = DIV_S(ctx->f12.fl, ctx->f0.fl);
L_80183C54:
    // 0x80183C54: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    // 0x80183C58: addiu       $a1, $v0, 0x104
    ctx->r5 = ADD32(ctx->r2, 0X104);
    // 0x80183C5C: mfc1        $a2, $f4
    ctx->r6 = (int32_t)ctx->f4.u32l;
    // 0x80183C60: jal         0x80183624
    // 0x80183C64: nop

    func_80183624(rdram, ctx);
        goto after_0;
    // 0x80183C64: nop

    after_0:
    // 0x80183C68: lwc1        $f12, 0x20($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X20);
    // 0x80183C6C: jal         0x80004C68
    // 0x80183C70: lwc1        $f14, 0x28($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X28);
    func_80004C68(rdram, ctx);
        goto after_1;
    // 0x80183C70: lwc1        $f14, 0x28($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X28);
    after_1:
    // 0x80183C74: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x80183C78: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80183C7C: lwc1        $f10, -0x3538($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3538);
    // 0x80183C80: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80183C84: mul.s       $f0, $f8, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80183C88: nop

L_80183C8C:
    // 0x80183C8C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80183C90: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80183C94: jr          $ra
    // 0x80183C98: nop

    return;
    // 0x80183C98: nop

;}
RECOMP_FUNC void func_80183C9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183C9C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80183CA0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80183CA4: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80183CA8: beq         $a1, $zero, L_80183CB8
    if (ctx->r5 == 0) {
        // 0x80183CAC: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_80183CB8;
    }
    // 0x80183CAC: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80183CB0: bne         $a0, $zero, L_80183CD8
    if (ctx->r4 != 0) {
        // 0x80183CB4: nop
    
            goto L_80183CD8;
    }
    // 0x80183CB4: nop

L_80183CB8:
    // 0x80183CB8: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80183CBC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183CC0: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80183CC4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80183CC8: swc1        $f2, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->f2.u32l;
    // 0x80183CCC: swc1        $f2, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f2.u32l;
    // 0x80183CD0: b           L_80183D78
    // 0x80183CD4: swc1        $f4, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f4.u32l;
        goto L_80183D78;
    // 0x80183CD4: swc1        $f4, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f4.u32l;
L_80183CD8:
    // 0x80183CD8: bltz        $a1, L_80183D00
    if (SIGNED(ctx->r5) < 0) {
        // 0x80183CDC: slti        $at, $a1, 0x20
        ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
            goto L_80183D00;
    }
    // 0x80183CDC: slti        $at, $a1, 0x20
    ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
    // 0x80183CE0: beq         $at, $zero, L_80183D00
    if (ctx->r1 == 0) {
        // 0x80183CE4: sll         $t6, $a1, 3
        ctx->r14 = S32(ctx->r5 << 3);
            goto L_80183D00;
    }
    // 0x80183CE4: sll         $t6, $a1, 3
    ctx->r14 = S32(ctx->r5 << 3);
    // 0x80183CE8: addu        $t6, $t6, $a1
    ctx->r14 = ADD32(ctx->r14, ctx->r5);
    // 0x80183CEC: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80183CF0: addu        $v0, $v1, $t6
    ctx->r2 = ADD32(ctx->r3, ctx->r14);
    // 0x80183CF4: lh          $t7, 0x10C($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X10C);
    // 0x80183CF8: bnel        $t7, $zero, L_80183D24
    if (ctx->r15 != 0) {
        // 0x80183CFC: mtc1        $zero, $f2
        ctx->f2.u32l = 0;
            goto L_80183D24;
    }
    goto skip_0;
    // 0x80183CFC: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    skip_0:
L_80183D00:
    // 0x80183D00: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x80183D04: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80183D08: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80183D0C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80183D10: swc1        $f2, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->f2.u32l;
    // 0x80183D14: swc1        $f2, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->f2.u32l;
    // 0x80183D18: b           L_80183D78
    // 0x80183D1C: swc1        $f6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f6.u32l;
        goto L_80183D78;
    // 0x80183D1C: swc1        $f6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->f6.u32l;
    // 0x80183D20: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
L_80183D24:
    // 0x80183D24: nop

    // 0x80183D28: c.lt.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl < ctx->f2.fl;
    // 0x80183D2C: nop

    // 0x80183D30: bc1fl       L_80183D48
    if (!c1cs) {
        // 0x80183D34: lwc1        $f0, 0x11C($v0)
        ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
            goto L_80183D48;
    }
    goto skip_1;
    // 0x80183D34: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    skip_1:
    // 0x80183D38: mov.s       $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    ctx->f12.fl = ctx->f2.fl;
    // 0x80183D3C: b           L_80183D5C
    // 0x80183D40: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
        goto L_80183D5C;
    // 0x80183D40: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
    // 0x80183D44: lwc1        $f0, 0x11C($v0)
    ctx->f0.u32l = MEM_W(ctx->r2, 0X11C);
L_80183D48:
    // 0x80183D48: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80183D4C: nop

    // 0x80183D50: bc1fl       L_80183D60
    if (!c1cs) {
        // 0x80183D54: div.s       $f8, $f12, $f0
        CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f12.fl, ctx->f0.fl);
            goto L_80183D60;
    }
    goto skip_2;
    // 0x80183D54: div.s       $f8, $f12, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f12.fl, ctx->f0.fl);
    skip_2:
    // 0x80183D58: mov.s       $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    ctx->f12.fl = ctx->f0.fl;
L_80183D5C:
    // 0x80183D5C: div.s       $f8, $f12, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = DIV_S(ctx->f12.fl, ctx->f0.fl);
L_80183D60:
    // 0x80183D60: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80183D64: addiu       $a1, $v0, 0x104
    ctx->r5 = ADD32(ctx->r2, 0X104);
    // 0x80183D68: mfc1        $a2, $f8
    ctx->r6 = (int32_t)ctx->f8.u32l;
    // 0x80183D6C: jal         0x80183514
    // 0x80183D70: nop

    func_80183514(rdram, ctx);
        goto after_0;
    // 0x80183D70: nop

    after_0:
    // 0x80183D74: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80183D78:
    // 0x80183D78: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80183D7C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80183D80: jr          $ra
    // 0x80183D84: nop

    return;
    // 0x80183D84: nop

;}
RECOMP_FUNC void func_80183D88(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80183D88: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x80183D8C: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x80183D90: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80183D94: mtc1        $a3, $f20
    ctx->f20.u32l = ctx->r7;
    // 0x80183D98: mtc1        $a2, $f22
    ctx->f22.u32l = ctx->r6;
    // 0x80183D9C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80183DA0: beq         $a1, $zero, L_80183DB0
    if (ctx->r5 == 0) {
        // 0x80183DA4: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_80183DB0;
    }
    // 0x80183DA4: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80183DA8: bne         $a0, $zero, L_80183DC0
    if (ctx->r4 != 0) {
        // 0x80183DAC: nop
    
            goto L_80183DC0;
    }
    // 0x80183DAC: nop

L_80183DB0:
    // 0x80183DB0: lw          $t8, 0x90($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X90);
    // 0x80183DB4: mov.s       $f0, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    ctx->f0.fl = ctx->f22.fl;
    // 0x80183DB8: b           L_80184164
    // 0x80183DBC: swc1        $f20, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->f20.u32l;
        goto L_80184164;
    // 0x80183DBC: swc1        $f20, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->f20.u32l;
L_80183DC0:
    // 0x80183DC0: bltz        $a1, L_80183DE8
    if (SIGNED(ctx->r5) < 0) {
        // 0x80183DC4: slti        $at, $a1, 0x20
        ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
            goto L_80183DE8;
    }
    // 0x80183DC4: slti        $at, $a1, 0x20
    ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
    // 0x80183DC8: beq         $at, $zero, L_80183DE8
    if (ctx->r1 == 0) {
        // 0x80183DCC: sll         $t9, $a1, 3
        ctx->r25 = S32(ctx->r5 << 3);
            goto L_80183DE8;
    }
    // 0x80183DCC: sll         $t9, $a1, 3
    ctx->r25 = S32(ctx->r5 << 3);
    // 0x80183DD0: addu        $t9, $t9, $a1
    ctx->r25 = ADD32(ctx->r25, ctx->r5);
    // 0x80183DD4: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x80183DD8: addu        $t1, $v1, $t9
    ctx->r9 = ADD32(ctx->r3, ctx->r25);
    // 0x80183DDC: lh          $v0, 0x10C($t1)
    ctx->r2 = MEM_H(ctx->r9, 0X10C);
    // 0x80183DE0: bnel        $v0, $zero, L_80183DFC
    if (ctx->r2 != 0) {
        // 0x80183DE4: lw          $t0, 0x110($t1)
        ctx->r8 = MEM_W(ctx->r9, 0X110);
            goto L_80183DFC;
    }
    goto skip_0;
    // 0x80183DE4: lw          $t0, 0x110($t1)
    ctx->r8 = MEM_W(ctx->r9, 0X110);
    skip_0:
L_80183DE8:
    // 0x80183DE8: lw          $t2, 0x90($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X90);
    // 0x80183DEC: mov.s       $f0, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    ctx->f0.fl = ctx->f22.fl;
    // 0x80183DF0: b           L_80184164
    // 0x80183DF4: swc1        $f20, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f20.u32l;
        goto L_80184164;
    // 0x80183DF4: swc1        $f20, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f20.u32l;
    // 0x80183DF8: lw          $t0, 0x110($t1)
    ctx->r8 = MEM_W(ctx->r9, 0X110);
L_80183DFC:
    // 0x80183DFC: slti        $at, $v0, 0x2
    ctx->r1 = SIGNED(ctx->r2) < 0X2 ? 1 : 0;
    // 0x80183E00: addiu       $a3, $v0, -0x1
    ctx->r7 = ADD32(ctx->r2, -0X1);
    // 0x80183E04: lwc1        $f4, 0x14($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0X14);
    // 0x80183E08: lwc1        $f10, 0xC($t0)
    ctx->f10.u32l = MEM_W(ctx->r8, 0XC);
    // 0x80183E0C: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x80183E10: sub.s       $f0, $f4, $f20
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f20.fl;
    // 0x80183E14: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80183E18: andi        $a3, $a3, 0x3
    ctx->r7 = ctx->r7 & 0X3;
    // 0x80183E1C: sub.s       $f2, $f10, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80183E20: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80183E24: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80183E28: mul.s       $f6, $f2, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80183E2C: bne         $at, $zero, L_80183F90
    if (ctx->r1 != 0) {
        // 0x80183E30: add.s       $f14, $f8, $f6
        CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f14.fl = ctx->f8.fl + ctx->f6.fl;
            goto L_80183F90;
    }
    // 0x80183E30: add.s       $f14, $f8, $f6
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f14.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183E34: beq         $a3, $zero, L_80183E94
    if (ctx->r7 == 0) {
        // 0x80183E38: addiu       $a1, $a3, 0x1
        ctx->r5 = ADD32(ctx->r7, 0X1);
            goto L_80183E94;
    }
    // 0x80183E38: addiu       $a1, $a3, 0x1
    ctx->r5 = ADD32(ctx->r7, 0X1);
    // 0x80183E3C: sll         $t3, $v1, 2
    ctx->r11 = S32(ctx->r3 << 2);
    // 0x80183E40: subu        $t3, $t3, $v1
    ctx->r11 = SUB32(ctx->r11, ctx->r3);
    // 0x80183E44: sll         $t3, $t3, 2
    ctx->r11 = S32(ctx->r11 << 2);
    // 0x80183E48: addu        $v0, $t0, $t3
    ctx->r2 = ADD32(ctx->r8, ctx->r11);
L_80183E4C:
    // 0x80183E4C: lwc1        $f4, 0x14($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80183E50: lwc1        $f10, 0xC($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80183E54: sub.s       $f2, $f4, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f20.fl;
    // 0x80183E58: sub.s       $f12, $f10, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80183E5C: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80183E60: nop

    // 0x80183E64: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80183E68: add.s       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183E6C: c.lt.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl < ctx->f14.fl;
    // 0x80183E70: nop

    // 0x80183E74: bc1f        L_80183E84
    if (!c1cs) {
        // 0x80183E78: nop
    
            goto L_80183E84;
    }
    // 0x80183E78: nop

    // 0x80183E7C: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80183E80: addiu       $a0, $v1, 0x1
    ctx->r4 = ADD32(ctx->r3, 0X1);
L_80183E84:
    // 0x80183E84: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80183E88: bne         $a1, $v1, L_80183E4C
    if (ctx->r5 != ctx->r3) {
        // 0x80183E8C: addiu       $v0, $v0, 0xC
        ctx->r2 = ADD32(ctx->r2, 0XC);
            goto L_80183E4C;
    }
    // 0x80183E8C: addiu       $v0, $v0, 0xC
    ctx->r2 = ADD32(ctx->r2, 0XC);
    // 0x80183E90: beq         $v1, $a2, L_80183F90
    if (ctx->r3 == ctx->r6) {
        // 0x80183E94: sll         $t4, $v1, 2
        ctx->r12 = S32(ctx->r3 << 2);
            goto L_80183F90;
    }
L_80183E94:
    // 0x80183E94: sll         $t4, $v1, 2
    ctx->r12 = S32(ctx->r3 << 2);
    // 0x80183E98: subu        $t4, $t4, $v1
    ctx->r12 = SUB32(ctx->r12, ctx->r3);
    // 0x80183E9C: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x80183EA0: addu        $v0, $t0, $t4
    ctx->r2 = ADD32(ctx->r8, ctx->r12);
L_80183EA4:
    // 0x80183EA4: lwc1        $f4, 0x14($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80183EA8: lwc1        $f10, 0xC($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80183EAC: sub.s       $f2, $f4, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f20.fl;
    // 0x80183EB0: sub.s       $f12, $f10, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80183EB4: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80183EB8: nop

    // 0x80183EBC: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80183EC0: add.s       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183EC4: c.lt.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl < ctx->f14.fl;
    // 0x80183EC8: nop

    // 0x80183ECC: bc1f        L_80183EDC
    if (!c1cs) {
        // 0x80183ED0: nop
    
            goto L_80183EDC;
    }
    // 0x80183ED0: nop

    // 0x80183ED4: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80183ED8: addiu       $a0, $v1, 0x1
    ctx->r4 = ADD32(ctx->r3, 0X1);
L_80183EDC:
    // 0x80183EDC: lwc1        $f4, 0x20($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X20);
    // 0x80183EE0: lwc1        $f10, 0x18($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X18);
    // 0x80183EE4: sub.s       $f2, $f4, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f20.fl;
    // 0x80183EE8: sub.s       $f12, $f10, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80183EEC: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80183EF0: nop

    // 0x80183EF4: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80183EF8: add.s       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183EFC: c.lt.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl < ctx->f14.fl;
    // 0x80183F00: nop

    // 0x80183F04: bc1f        L_80183F14
    if (!c1cs) {
        // 0x80183F08: nop
    
            goto L_80183F14;
    }
    // 0x80183F08: nop

    // 0x80183F0C: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80183F10: addiu       $a0, $v1, 0x2
    ctx->r4 = ADD32(ctx->r3, 0X2);
L_80183F14:
    // 0x80183F14: lwc1        $f4, 0x2C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X2C);
    // 0x80183F18: lwc1        $f10, 0x24($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X24);
    // 0x80183F1C: sub.s       $f2, $f4, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f20.fl;
    // 0x80183F20: sub.s       $f12, $f10, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80183F24: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80183F28: nop

    // 0x80183F2C: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80183F30: add.s       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183F34: c.lt.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl < ctx->f14.fl;
    // 0x80183F38: nop

    // 0x80183F3C: bc1f        L_80183F4C
    if (!c1cs) {
        // 0x80183F40: nop
    
            goto L_80183F4C;
    }
    // 0x80183F40: nop

    // 0x80183F44: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80183F48: addiu       $a0, $v1, 0x3
    ctx->r4 = ADD32(ctx->r3, 0X3);
L_80183F4C:
    // 0x80183F4C: lwc1        $f4, 0x38($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X38);
    // 0x80183F50: lwc1        $f10, 0x30($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X30);
    // 0x80183F54: sub.s       $f2, $f4, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f20.fl;
    // 0x80183F58: sub.s       $f12, $f10, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80183F5C: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80183F60: nop

    // 0x80183F64: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80183F68: add.s       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80183F6C: c.lt.s      $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    c1cs = ctx->f0.fl < ctx->f14.fl;
    // 0x80183F70: nop

    // 0x80183F74: bc1f        L_80183F84
    if (!c1cs) {
        // 0x80183F78: nop
    
            goto L_80183F84;
    }
    // 0x80183F78: nop

    // 0x80183F7C: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80183F80: addiu       $a0, $v1, 0x4
    ctx->r4 = ADD32(ctx->r3, 0X4);
L_80183F84:
    // 0x80183F84: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80183F88: bne         $v1, $a2, L_80183EA4
    if (ctx->r3 != ctx->r6) {
        // 0x80183F8C: addiu       $v0, $v0, 0x30
        ctx->r2 = ADD32(ctx->r2, 0X30);
            goto L_80183EA4;
    }
    // 0x80183F8C: addiu       $v0, $v0, 0x30
    ctx->r2 = ADD32(ctx->r2, 0X30);
L_80183F90:
    // 0x80183F90: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80183F94: bne         $a0, $at, L_80183FB4
    if (ctx->r4 != ctx->r1) {
        // 0x80183F98: sll         $t5, $a0, 2
        ctx->r13 = S32(ctx->r4 << 2);
            goto L_80183FB4;
    }
    // 0x80183F98: sll         $t5, $a0, 2
    ctx->r13 = S32(ctx->r4 << 2);
    // 0x80183F9C: subu        $t5, $t5, $a0
    ctx->r13 = SUB32(ctx->r13, ctx->r4);
    // 0x80183FA0: sll         $t5, $t5, 2
    ctx->r13 = S32(ctx->r13 << 2);
    // 0x80183FA4: addu        $v0, $t0, $t5
    ctx->r2 = ADD32(ctx->r8, ctx->r13);
    // 0x80183FA8: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80183FAC: b           L_80184040
    // 0x80183FB0: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
        goto L_80184040;
    // 0x80183FB0: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
L_80183FB4:
    // 0x80183FB4: bne         $a0, $a2, L_80183FDC
    if (ctx->r4 != ctx->r6) {
        // 0x80183FB8: sll         $t7, $a0, 2
        ctx->r15 = S32(ctx->r4 << 2);
            goto L_80183FDC;
    }
    // 0x80183FB8: sll         $t7, $a0, 2
    ctx->r15 = S32(ctx->r4 << 2);
    // 0x80183FBC: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
    // 0x80183FC0: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x80183FC4: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x80183FC8: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80183FCC: addu        $v0, $t0, $t6
    ctx->r2 = ADD32(ctx->r8, ctx->r14);
    // 0x80183FD0: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80183FD4: b           L_80184040
    // 0x80183FD8: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
        goto L_80184040;
    // 0x80183FD8: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
L_80183FDC:
    // 0x80183FDC: subu        $t7, $t7, $a0
    ctx->r15 = SUB32(ctx->r15, ctx->r4);
    // 0x80183FE0: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80183FE4: addu        $v0, $t0, $t7
    ctx->r2 = ADD32(ctx->r8, ctx->r15);
    // 0x80183FE8: lwc1        $f4, -0xC($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, -0XC);
    // 0x80183FEC: lwc1        $f10, -0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, -0X4);
    // 0x80183FF0: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80183FF4: sub.s       $f0, $f4, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f22.fl;
    // 0x80183FF8: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80183FFC: sub.s       $f2, $f10, $f20
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f20.fl;
    // 0x80184000: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80184004: sub.s       $f12, $f14, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = ctx->f14.fl - ctx->f20.fl;
    // 0x80184008: mul.s       $f6, $f2, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x8018400C: sub.s       $f18, $f16, $f22
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f22.fl;
    // 0x80184010: mul.s       $f10, $f12, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80184014: add.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80184018: mul.s       $f8, $f18, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f18.fl);
    // 0x8018401C: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80184020: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x80184024: nop

    // 0x80184028: bc1fl       L_80184044
    if (!c1cs) {
        // 0x8018402C: lwc1        $f8, 0x8($v0)
        ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
            goto L_80184044;
    }
    goto skip_1;
    // 0x8018402C: lwc1        $f8, 0x8($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
    skip_1:
    // 0x80184030: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
    // 0x80184034: addiu       $v0, $v0, -0xC
    ctx->r2 = ADD32(ctx->r2, -0XC);
    // 0x80184038: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x8018403C: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
L_80184040:
    // 0x80184040: lwc1        $f8, 0x8($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
L_80184044:
    // 0x80184044: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80184048: sw          $t1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r9;
    // 0x8018404C: sub.s       $f2, $f14, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = ctx->f14.fl - ctx->f8.fl;
    // 0x80184050: sw          $a2, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r6;
    // 0x80184054: sw          $a0, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r4;
    // 0x80184058: sub.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x8018405C: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80184060: swc1        $f2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f2.u32l;
    // 0x80184064: swc1        $f2, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f2.u32l;
    // 0x80184068: neg.s       $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = -ctx->f18.fl;
    // 0x8018406C: swc1        $f18, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f18.u32l;
    // 0x80184070: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80184074: swc1        $f0, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f0.u32l;
    // 0x80184078: sw          $v0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r2;
    // 0x8018407C: jal         0x800A01E0
    // 0x80184080: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x80184080: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    after_0:
    // 0x80184084: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80184088: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x8018408C: lwc1        $f2, 0x60($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X60);
    // 0x80184090: lwc1        $f4, 0x54($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80184094: div.s       $f12, $f10, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80184098: lw          $v0, 0x34($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X34);
    // 0x8018409C: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    // 0x801840A0: lw          $t8, 0x90($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X90);
    // 0x801840A4: lw          $a2, 0x74($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X74);
    // 0x801840A8: lw          $t1, 0x38($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X38);
    // 0x801840AC: addiu       $t9, $a2, -0x1
    ctx->r25 = ADD32(ctx->r6, -0X1);
    // 0x801840B0: mul.s       $f8, $f2, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f12.fl);
    // 0x801840B4: nop

    // 0x801840B8: mul.s       $f6, $f4, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x801840BC: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801840C0: swc1        $f8, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f8.u32l;
    // 0x801840C4: swc1        $f6, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f6.u32l;
    // 0x801840C8: lwc1        $f8, 0x8($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801840CC: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801840D0: swc1        $f4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f4.u32l;
    // 0x801840D4: sub.s       $f18, $f8, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f18.fl = ctx->f8.fl - ctx->f20.fl;
    // 0x801840D8: lwc1        $f8, 0x4C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x801840DC: sub.s       $f16, $f10, $f22
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f16.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x801840E0: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801840E4: nop

    // 0x801840E8: mul.s       $f4, $f2, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f8.fl);
    // 0x801840EC: sub.s       $f4, $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f4.fl - ctx->f10.fl;
    // 0x801840F0: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801840F4: nop

    // 0x801840F8: div.s       $f14, $f10, $f4
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f14.fl = DIV_S(ctx->f10.fl, ctx->f4.fl);
    // 0x801840FC: mul.s       $f10, $f6, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80184100: nop

    // 0x80184104: mul.s       $f4, $f18, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f8.fl);
    // 0x80184108: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x8018410C: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x80184110: mul.s       $f8, $f6, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x80184114: nop

    // 0x80184118: mul.s       $f10, $f2, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f16.fl);
    // 0x8018411C: nop

    // 0x80184120: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x80184124: swc1        $f8, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f8.u32l;
    // 0x80184128: sub.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f6.fl;
    // 0x8018412C: mtc1        $a0, $f6
    ctx->f6.u32l = ctx->r4;
    // 0x80184130: mul.s       $f10, $f4, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f14.fl);
    // 0x80184134: cvt.s.w     $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80184138: swc1        $f10, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f10.u32l;
    // 0x8018413C: swc1        $f10, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->f10.u32l;
    // 0x80184140: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80184144: add.s       $f8, $f4, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80184148: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x8018414C: sub.s       $f6, $f8, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80184150: lwc1        $f10, 0x11C($t1)
    ctx->f10.u32l = MEM_W(ctx->r9, 0X11C);
    // 0x80184154: cvt.s.w     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80184158: div.s       $f2, $f6, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x8018415C: mul.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80184160: nop

L_80184164:
    // 0x80184164: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80184168: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x8018416C: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x80184170: jr          $ra
    // 0x80184174: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    return;
    // 0x80184174: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
;}
RECOMP_FUNC void func_80184178(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184178: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x8018417C: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x80184180: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80184184: mtc1        $a3, $f20
    ctx->f20.u32l = ctx->r7;
    // 0x80184188: mtc1        $a2, $f22
    ctx->f22.u32l = ctx->r6;
    // 0x8018418C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80184190: beq         $a1, $zero, L_801841A0
    if (ctx->r5 == 0) {
        // 0x80184194: lw          $v1, 0x34($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X34);
            goto L_801841A0;
    }
    // 0x80184194: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80184198: bne         $a0, $zero, L_801841B4
    if (ctx->r4 != 0) {
        // 0x8018419C: nop
    
            goto L_801841B4;
    }
    // 0x8018419C: nop

L_801841A0:
    // 0x801841A0: lwc1        $f18, 0x90($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X90);
    // 0x801841A4: lw          $t5, 0x94($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X94);
    // 0x801841A8: mov.s       $f0, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    ctx->f0.fl = ctx->f22.fl;
    // 0x801841AC: b           L_80184508
    // 0x801841B0: swc1        $f18, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->f18.u32l;
        goto L_80184508;
    // 0x801841B0: swc1        $f18, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->f18.u32l;
L_801841B4:
    // 0x801841B4: bltz        $a1, L_801841DC
    if (SIGNED(ctx->r5) < 0) {
        // 0x801841B8: slti        $at, $a1, 0x20
        ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
            goto L_801841DC;
    }
    // 0x801841B8: slti        $at, $a1, 0x20
    ctx->r1 = SIGNED(ctx->r5) < 0X20 ? 1 : 0;
    // 0x801841BC: beq         $at, $zero, L_801841DC
    if (ctx->r1 == 0) {
        // 0x801841C0: sll         $t6, $a1, 3
        ctx->r14 = S32(ctx->r5 << 3);
            goto L_801841DC;
    }
    // 0x801841C0: sll         $t6, $a1, 3
    ctx->r14 = S32(ctx->r5 << 3);
    // 0x801841C4: addu        $t6, $t6, $a1
    ctx->r14 = ADD32(ctx->r14, ctx->r5);
    // 0x801841C8: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x801841CC: addu        $a3, $v1, $t6
    ctx->r7 = ADD32(ctx->r3, ctx->r14);
    // 0x801841D0: lh          $v0, 0x10C($a3)
    ctx->r2 = MEM_H(ctx->r7, 0X10C);
    // 0x801841D4: bnel        $v0, $zero, L_801841F4
    if (ctx->r2 != 0) {
        // 0x801841D8: lw          $a1, 0x110($a3)
        ctx->r5 = MEM_W(ctx->r7, 0X110);
            goto L_801841F4;
    }
    goto skip_0;
    // 0x801841D8: lw          $a1, 0x110($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X110);
    skip_0:
L_801841DC:
    // 0x801841DC: lwc1        $f18, 0x90($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X90);
    // 0x801841E0: lw          $t7, 0x94($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X94);
    // 0x801841E4: mov.s       $f0, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 22);
    ctx->f0.fl = ctx->f22.fl;
    // 0x801841E8: b           L_80184508
    // 0x801841EC: swc1        $f18, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->f18.u32l;
        goto L_80184508;
    // 0x801841EC: swc1        $f18, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->f18.u32l;
    // 0x801841F0: lw          $a1, 0x110($a3)
    ctx->r5 = MEM_W(ctx->r7, 0X110);
L_801841F4:
    // 0x801841F4: lwc1        $f18, 0x90($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X90);
    // 0x801841F8: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x801841FC: lwc1        $f6, 0xC($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0XC);
    // 0x80184200: lwc1        $f8, 0x10($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X10);
    // 0x80184204: lwc1        $f10, 0x14($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X14);
    // 0x80184208: sub.s       $f2, $f6, $f22
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f2.fl = ctx->f6.fl - ctx->f22.fl;
    // 0x8018420C: slti        $at, $v0, 0x2
    ctx->r1 = SIGNED(ctx->r2) < 0X2 ? 1 : 0;
    // 0x80184210: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80184214: sub.s       $f12, $f8, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = ctx->f8.fl - ctx->f20.fl;
    // 0x80184218: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x8018421C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80184220: sub.s       $f0, $f10, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f10.fl - ctx->f18.fl;
    // 0x80184224: mul.s       $f10, $f12, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80184228: andi        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 & 0X1;
    // 0x8018422C: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x80184230: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80184234: add.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x80184238: bne         $at, $zero, L_80184338
    if (ctx->r1 != 0) {
        // 0x8018423C: add.s       $f16, $f8, $f6
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = ctx->f8.fl + ctx->f6.fl;
            goto L_80184338;
    }
    // 0x8018423C: add.s       $f16, $f8, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80184240: beq         $v0, $zero, L_80184294
    if (ctx->r2 == 0) {
        // 0x80184244: addiu       $v0, $a1, 0xC
        ctx->r2 = ADD32(ctx->r5, 0XC);
            goto L_80184294;
    }
    // 0x80184244: addiu       $v0, $a1, 0xC
    ctx->r2 = ADD32(ctx->r5, 0XC);
    // 0x80184248: lwc1        $f10, 0xC($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0XC);
    // 0x8018424C: lwc1        $f8, 0x10($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X10);
    // 0x80184250: lwc1        $f4, 0x14($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80184254: sub.s       $f12, $f10, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80184258: addiu       $v1, $zero, 0x2
    ctx->r3 = ADD32(0, 0X2);
    // 0x8018425C: sub.s       $f14, $f8, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f14.fl = ctx->f8.fl - ctx->f20.fl;
    // 0x80184260: mul.s       $f6, $f12, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80184264: sub.s       $f2, $f4, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f4.fl - ctx->f18.fl;
    // 0x80184268: mul.s       $f4, $f14, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8018426C: add.s       $f10, $f6, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f4.fl;
    // 0x80184270: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80184274: add.s       $f0, $f8, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80184278: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x8018427C: nop

    // 0x80184280: bc1f        L_80184290
    if (!c1cs) {
        // 0x80184284: nop
    
            goto L_80184290;
    }
    // 0x80184284: nop

    // 0x80184288: mov.s       $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    ctx->f16.fl = ctx->f0.fl;
    // 0x8018428C: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
L_80184290:
    // 0x80184290: beq         $v1, $a2, L_80184338
    if (ctx->r3 == ctx->r6) {
        // 0x80184294: sll         $t8, $v1, 2
        ctx->r24 = S32(ctx->r3 << 2);
            goto L_80184338;
    }
L_80184294:
    // 0x80184294: sll         $t8, $v1, 2
    ctx->r24 = S32(ctx->r3 << 2);
    // 0x80184298: subu        $t8, $t8, $v1
    ctx->r24 = SUB32(ctx->r24, ctx->r3);
    // 0x8018429C: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x801842A0: addu        $v0, $a1, $t8
    ctx->r2 = ADD32(ctx->r5, ctx->r24);
L_801842A4:
    // 0x801842A4: lwc1        $f4, 0xC($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0XC);
    // 0x801842A8: lwc1        $f8, 0x10($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X10);
    // 0x801842AC: lwc1        $f6, 0x14($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X14);
    // 0x801842B0: sub.s       $f12, $f4, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f4.fl - ctx->f22.fl;
    // 0x801842B4: sub.s       $f14, $f8, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f14.fl = ctx->f8.fl - ctx->f20.fl;
    // 0x801842B8: mul.s       $f10, $f12, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x801842BC: sub.s       $f2, $f6, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f6.fl - ctx->f18.fl;
    // 0x801842C0: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x801842C4: add.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x801842C8: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801842CC: add.s       $f0, $f8, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x801842D0: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x801842D4: nop

    // 0x801842D8: bc1fl       L_801842EC
    if (!c1cs) {
        // 0x801842DC: lwc1        $f6, 0x18($v0)
        ctx->f6.u32l = MEM_W(ctx->r2, 0X18);
            goto L_801842EC;
    }
    goto skip_1;
    // 0x801842DC: lwc1        $f6, 0x18($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X18);
    skip_1:
    // 0x801842E0: mov.s       $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    ctx->f16.fl = ctx->f0.fl;
    // 0x801842E4: addiu       $a0, $v1, 0x1
    ctx->r4 = ADD32(ctx->r3, 0X1);
    // 0x801842E8: lwc1        $f6, 0x18($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X18);
L_801842EC:
    // 0x801842EC: lwc1        $f8, 0x1C($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X1C);
    // 0x801842F0: lwc1        $f10, 0x20($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X20);
    // 0x801842F4: sub.s       $f12, $f6, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f12.fl = ctx->f6.fl - ctx->f22.fl;
    // 0x801842F8: sub.s       $f14, $f8, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f14.fl = ctx->f8.fl - ctx->f20.fl;
    // 0x801842FC: mul.s       $f4, $f12, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80184300: sub.s       $f2, $f10, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f18.fl;
    // 0x80184304: mul.s       $f10, $f14, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80184308: add.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x8018430C: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80184310: add.s       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x80184314: c.lt.s      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.fl < ctx->f16.fl;
    // 0x80184318: nop

    // 0x8018431C: bc1fl       L_80184330
    if (!c1cs) {
        // 0x80184320: addiu       $v1, $v1, 0x2
        ctx->r3 = ADD32(ctx->r3, 0X2);
            goto L_80184330;
    }
    goto skip_2;
    // 0x80184320: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    skip_2:
    // 0x80184324: mov.s       $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    ctx->f16.fl = ctx->f0.fl;
    // 0x80184328: addiu       $a0, $v1, 0x2
    ctx->r4 = ADD32(ctx->r3, 0X2);
    // 0x8018432C: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
L_80184330:
    // 0x80184330: bne         $v1, $a2, L_801842A4
    if (ctx->r3 != ctx->r6) {
        // 0x80184334: addiu       $v0, $v0, 0x18
        ctx->r2 = ADD32(ctx->r2, 0X18);
            goto L_801842A4;
    }
    // 0x80184334: addiu       $v0, $v0, 0x18
    ctx->r2 = ADD32(ctx->r2, 0X18);
L_80184338:
    // 0x80184338: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8018433C: bne         $a0, $at, L_8018435C
    if (ctx->r4 != ctx->r1) {
        // 0x80184340: sll         $t9, $a0, 2
        ctx->r25 = S32(ctx->r4 << 2);
            goto L_8018435C;
    }
    // 0x80184340: sll         $t9, $a0, 2
    ctx->r25 = S32(ctx->r4 << 2);
    // 0x80184344: subu        $t9, $t9, $a0
    ctx->r25 = SUB32(ctx->r25, ctx->r4);
    // 0x80184348: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x8018434C: addu        $v0, $a1, $t9
    ctx->r2 = ADD32(ctx->r5, ctx->r25);
    // 0x80184350: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x80184354: b           L_801843E8
    // 0x80184358: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
        goto L_801843E8;
    // 0x80184358: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
L_8018435C:
    // 0x8018435C: bne         $a0, $a2, L_80184384
    if (ctx->r4 != ctx->r6) {
        // 0x80184360: sll         $t1, $a0, 2
        ctx->r9 = S32(ctx->r4 << 2);
            goto L_80184384;
    }
    // 0x80184360: sll         $t1, $a0, 2
    ctx->r9 = S32(ctx->r4 << 2);
    // 0x80184364: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
    // 0x80184368: sll         $t0, $a0, 2
    ctx->r8 = S32(ctx->r4 << 2);
    // 0x8018436C: subu        $t0, $t0, $a0
    ctx->r8 = SUB32(ctx->r8, ctx->r4);
    // 0x80184370: sll         $t0, $t0, 2
    ctx->r8 = S32(ctx->r8 << 2);
    // 0x80184374: addu        $v0, $a1, $t0
    ctx->r2 = ADD32(ctx->r5, ctx->r8);
    // 0x80184378: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x8018437C: b           L_801843E8
    // 0x80184380: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
        goto L_801843E8;
    // 0x80184380: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
L_80184384:
    // 0x80184384: subu        $t1, $t1, $a0
    ctx->r9 = SUB32(ctx->r9, ctx->r4);
    // 0x80184388: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x8018438C: addu        $v0, $a1, $t1
    ctx->r2 = ADD32(ctx->r5, ctx->r9);
    // 0x80184390: lwc1        $f4, -0xC($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, -0XC);
    // 0x80184394: lwc1        $f10, -0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, -0X4);
    // 0x80184398: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x8018439C: sub.s       $f0, $f4, $f22
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f22.fl;
    // 0x801843A0: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
    // 0x801843A4: sub.s       $f2, $f10, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f18.fl;
    // 0x801843A8: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x801843AC: sub.s       $f12, $f14, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f14.fl - ctx->f18.fl;
    // 0x801843B0: mul.s       $f6, $f2, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801843B4: sub.s       $f20, $f16, $f22
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f20.fl = ctx->f16.fl - ctx->f22.fl;
    // 0x801843B8: mul.s       $f10, $f12, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x801843BC: add.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x801843C0: mul.s       $f8, $f20, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f20.fl, ctx->f20.fl);
    // 0x801843C4: add.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x801843C8: c.lt.s      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.fl < ctx->f6.fl;
    // 0x801843CC: nop

    // 0x801843D0: bc1fl       L_801843EC
    if (!c1cs) {
        // 0x801843D4: lwc1        $f8, 0x8($v0)
        ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
            goto L_801843EC;
    }
    goto skip_3;
    // 0x801843D4: lwc1        $f8, 0x8($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
    skip_3:
    // 0x801843D8: addiu       $a0, $a0, -0x1
    ctx->r4 = ADD32(ctx->r4, -0X1);
    // 0x801843DC: addiu       $v0, $v0, -0xC
    ctx->r2 = ADD32(ctx->r2, -0XC);
    // 0x801843E0: lwc1        $f14, 0x14($v0)
    ctx->f14.u32l = MEM_W(ctx->r2, 0X14);
    // 0x801843E4: lwc1        $f16, 0xC($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0XC);
L_801843E8:
    // 0x801843E8: lwc1        $f8, 0x8($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
L_801843EC:
    // 0x801843EC: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801843F0: sw          $a3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r7;
    // 0x801843F4: sub.s       $f20, $f14, $f8
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f20.fl = ctx->f14.fl - ctx->f8.fl;
    // 0x801843F8: sw          $a2, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r6;
    // 0x801843FC: sw          $a0, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r4;
    // 0x80184400: sub.s       $f2, $f16, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x80184404: mul.s       $f4, $f20, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f20.fl);
    // 0x80184408: swc1        $f20, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f20.u32l;
    // 0x8018440C: sw          $v0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r2;
    // 0x80184410: neg.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = -ctx->f2.fl;
    // 0x80184414: swc1        $f2, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f2.u32l;
    // 0x80184418: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x8018441C: swc1        $f0, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f0.u32l;
    // 0x80184420: jal         0x800A01E0
    // 0x80184424: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x80184424: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    after_0:
    // 0x80184428: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8018442C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80184430: lwc1        $f4, 0x54($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80184434: lw          $v0, 0x34($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X34);
    // 0x80184438: div.s       $f2, $f10, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = DIV_S(ctx->f10.fl, ctx->f0.fl);
    // 0x8018443C: lwc1        $f18, 0x90($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X90);
    // 0x80184440: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    // 0x80184444: lw          $t2, 0x94($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X94);
    // 0x80184448: lw          $a2, 0x74($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X74);
    // 0x8018444C: lw          $a3, 0x38($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X38);
    // 0x80184450: addiu       $t3, $a2, -0x1
    ctx->r11 = ADD32(ctx->r6, -0X1);
    // 0x80184454: mul.s       $f8, $f20, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x80184458: nop

    // 0x8018445C: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80184460: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80184464: swc1        $f8, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f8.u32l;
    // 0x80184468: swc1        $f6, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f6.u32l;
    // 0x8018446C: lwc1        $f8, 0x8($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80184470: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80184474: swc1        $f4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f4.u32l;
    // 0x80184478: sub.s       $f16, $f8, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f8.fl - ctx->f18.fl;
    // 0x8018447C: lwc1        $f8, 0x4C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80184480: sub.s       $f14, $f10, $f22
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f14.fl = ctx->f10.fl - ctx->f22.fl;
    // 0x80184484: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80184488: nop

    // 0x8018448C: mul.s       $f4, $f20, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f8.fl);
    // 0x80184490: sub.s       $f4, $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f4.fl - ctx->f10.fl;
    // 0x80184494: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x80184498: nop

    // 0x8018449C: div.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = DIV_S(ctx->f10.fl, ctx->f4.fl);
    // 0x801844A0: mul.s       $f10, $f6, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x801844A4: nop

    // 0x801844A8: mul.s       $f4, $f16, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x801844AC: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x801844B0: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x801844B4: mul.s       $f8, $f6, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f12.fl);
    // 0x801844B8: nop

    // 0x801844BC: mul.s       $f10, $f20, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f20.fl, ctx->f14.fl);
    // 0x801844C0: nop

    // 0x801844C4: mul.s       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f4.fl);
    // 0x801844C8: swc1        $f8, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f8.u32l;
    // 0x801844CC: sub.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl - ctx->f6.fl;
    // 0x801844D0: mtc1        $a0, $f6
    ctx->f6.u32l = ctx->r4;
    // 0x801844D4: mul.s       $f10, $f4, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f12.fl);
    // 0x801844D8: cvt.s.w     $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    ctx->f4.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801844DC: swc1        $f10, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f10.u32l;
    // 0x801844E0: swc1        $f10, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->f10.u32l;
    // 0x801844E4: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801844E8: add.s       $f8, $f4, $f8
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801844EC: mtc1        $t3, $f4
    ctx->f4.u32l = ctx->r11;
    // 0x801844F0: sub.s       $f6, $f8, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x801844F4: lwc1        $f10, 0x11C($a3)
    ctx->f10.u32l = MEM_W(ctx->r7, 0X11C);
    // 0x801844F8: cvt.s.w     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801844FC: div.s       $f2, $f6, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80184500: mul.s       $f0, $f10, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f0.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80184504: nop

L_80184508:
    // 0x80184508: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x8018450C: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80184510: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x80184514: jr          $ra
    // 0x80184518: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    return;
    // 0x80184518: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
;}
RECOMP_FUNC void func_80184520(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184520: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80184524: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x80184528: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8018452C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80184530: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80184534: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x80184538: addiu       $a0, $v0, 0x18
    ctx->r4 = ADD32(ctx->r2, 0X18);
    // 0x8018453C: jal         0x80184610
    // 0x80184540: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    func_80184610(rdram, ctx);
        goto after_0;
    // 0x80184540: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    after_0:
    // 0x80184544: beq         $v0, $zero, L_80184570
    if (ctx->r2 == 0) {
        // 0x80184548: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80184570;
    }
    // 0x80184548: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8018454C: beq         $v0, $at, L_80184568
    if (ctx->r2 == ctx->r1) {
        // 0x80184550: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_80184568;
    }
    // 0x80184550: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80184554: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80184558: beql        $v0, $at, L_80184574
    if (ctx->r2 == ctx->r1) {
        // 0x8018455C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184574;
    }
    goto skip_0;
    // 0x8018455C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80184560: b           L_80184574
    // 0x80184564: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80184574;
    // 0x80184564: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80184568:
    // 0x80184568: jal         0x80185738
    // 0x8018456C: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    func_80185738(rdram, ctx);
        goto after_1;
    // 0x8018456C: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    after_1:
L_80184570:
    // 0x80184570: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80184574:
    // 0x80184574: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80184578: jr          $ra
    // 0x8018457C: nop

    return;
    // 0x8018457C: nop

;}
RECOMP_FUNC void func_80184580(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184580: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80184584: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x80184588: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8018458C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80184590: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80184594: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x80184598: addiu       $a0, $v0, 0x18
    ctx->r4 = ADD32(ctx->r2, 0X18);
    // 0x8018459C: jal         0x80184754
    // 0x801845A0: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    func_80184754(rdram, ctx);
        goto after_0;
    // 0x801845A0: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    after_0:
    // 0x801845A4: beq         $v0, $zero, L_801845D0
    if (ctx->r2 == 0) {
        // 0x801845A8: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_801845D0;
    }
    // 0x801845A8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801845AC: beq         $v0, $at, L_801845C8
    if (ctx->r2 == ctx->r1) {
        // 0x801845B0: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_801845C8;
    }
    // 0x801845B0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801845B4: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x801845B8: beql        $v0, $at, L_801845D4
    if (ctx->r2 == ctx->r1) {
        // 0x801845BC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801845D4;
    }
    goto skip_0;
    // 0x801845BC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801845C0: b           L_801845D4
    // 0x801845C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801845D4;
    // 0x801845C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801845C8:
    // 0x801845C8: jal         0x80185738
    // 0x801845CC: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    func_80185738(rdram, ctx);
        goto after_1;
    // 0x801845CC: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    after_1:
L_801845D0:
    // 0x801845D0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801845D4:
    // 0x801845D4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801845D8: jr          $ra
    // 0x801845DC: nop

    return;
    // 0x801845DC: nop

;}
RECOMP_FUNC void func_801845E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801845E0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801845E4: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x801845E8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801845EC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x801845F0: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x801845F4: addiu       $a0, $v0, 0x18
    ctx->r4 = ADD32(ctx->r2, 0X18);
    // 0x801845F8: jal         0x801848D0
    // 0x801845FC: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    func_801848D0(rdram, ctx);
        goto after_0;
    // 0x801845FC: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    after_0:
    // 0x80184600: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80184604: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80184608: jr          $ra
    // 0x8018460C: nop

    return;
    // 0x8018460C: nop

;}
RECOMP_FUNC void func_80184610(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184610: addiu       $sp, $sp, -0x1D8
    ctx->r29 = ADD32(ctx->r29, -0X1D8);
    // 0x80184614: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80184618: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8018461C: sw          $a0, 0x1D8($sp)
    MEM_W(0X1D8, ctx->r29) = ctx->r4;
    // 0x80184620: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80184624: sw          $a2, 0x1E0($sp)
    MEM_W(0X1E0, ctx->r29) = ctx->r6;
    // 0x80184628: jal         0x80013DE4
    // 0x8018462C: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    func_80013DE4(rdram, ctx);
        goto after_0;
    // 0x8018462C: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    after_0:
    // 0x80184630: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80184634: nop

    // 0x80184638: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x8018463C: nop

    // 0x80184640: bc1f        L_80184650
    if (!c1cs) {
        // 0x80184644: nop
    
            goto L_80184650;
    }
    // 0x80184644: nop

    // 0x80184648: b           L_80184740
    // 0x8018464C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80184740;
    // 0x8018464C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184650:
    // 0x80184650: jal         0x80013BD0
    // 0x80184654: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80013BD0(rdram, ctx);
        goto after_1;
    // 0x80184654: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x80184658: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8018465C: ldc1        $f8, -0x3268($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3268);
    // 0x80184660: cvt.d.s     $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f6.d = CVT_D_S(ctx->f0.fl);
    // 0x80184664: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80184668: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x8018466C: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    // 0x80184670: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x80184674: mfc1        $a2, $f16
    ctx->r6 = (int32_t)ctx->f16.u32l;
    // 0x80184678: jal         0x80013D58
    // 0x8018467C: nop

    func_80013D58(rdram, ctx);
        goto after_2;
    // 0x8018467C: nop

    after_2:
    // 0x80184680: lw          $a0, 0x1D8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1D8);
    // 0x80184684: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    // 0x80184688: jal         0x8003F22C
    // 0x8018468C: addiu       $a2, $sp, 0x50
    ctx->r6 = ADD32(ctx->r29, 0X50);
    func_8003F22C(rdram, ctx);
        goto after_3;
    // 0x8018468C: addiu       $a2, $sp, 0x50
    ctx->r6 = ADD32(ctx->r29, 0X50);
    after_3:
    // 0x80184690: lw          $t6, 0x50($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X50);
    // 0x80184694: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X54);
    // 0x80184698: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x8018469C: bgezl       $t7, L_80184740
    if (SIGNED(ctx->r15) >= 0) {
        // 0x801846A0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80184740;
    }
    goto skip_0;
    // 0x801846A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x801846A4: jal         0x801855FC
    // 0x801846A8: lw          $a1, 0x60($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X60);
    func_801855FC(rdram, ctx);
        goto after_4;
    // 0x801846A8: lw          $a1, 0x60($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X60);
    after_4:
    // 0x801846AC: bne         $v0, $zero, L_801846BC
    if (ctx->r2 != 0) {
        // 0x801846B0: lwc1        $f4, 0x64($sp)
        ctx->f4.u32l = MEM_W(ctx->r29, 0X64);
            goto L_801846BC;
    }
    // 0x801846B0: lwc1        $f4, 0x64($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801846B4: b           L_80184740
    // 0x801846B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80184740;
    // 0x801846B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801846BC:
    // 0x801846BC: lwc1        $f18, 0x0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X0);
    // 0x801846C0: lwc1        $f8, 0x4($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X4);
    // 0x801846C4: lwc1        $f10, 0x68($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X68);
    // 0x801846C8: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x801846CC: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x801846D0: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x801846D4: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x801846D8: lwc1        $f8, 0x6C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x801846DC: addiu       $a1, $sp, 0x64
    ctx->r5 = ADD32(ctx->r29, 0X64);
    // 0x801846E0: lui         $a2, 0x3A83
    ctx->r6 = S32(0X3A83 << 16);
    // 0x801846E4: mul.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x801846E8: add.s       $f18, $f6, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f6.fl + ctx->f16.fl;
    // 0x801846EC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x801846F0: add.s       $f0, $f10, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f10.fl + ctx->f18.fl;
    // 0x801846F4: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x801846F8: nop

    // 0x801846FC: bc1fl       L_80184740
    if (!c1cs) {
        // 0x80184700: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80184740;
    }
    goto skip_1;
    // 0x80184700: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x80184704: jal         0x80013D58
    // 0x80184708: ori         $a2, $a2, 0x126F
    ctx->r6 = ctx->r6 | 0X126F;
    func_80013D58(rdram, ctx);
        goto after_5;
    // 0x80184708: ori         $a2, $a2, 0x126F
    ctx->r6 = ctx->r6 | 0X126F;
    after_5:
    // 0x8018470C: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x80184710: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80184714: jal         0x80013C90
    // 0x80184718: addiu       $a2, $sp, 0x90
    ctx->r6 = ADD32(ctx->r29, 0X90);
    func_80013C90(rdram, ctx);
        goto after_6;
    // 0x80184718: addiu       $a2, $sp, 0x90
    ctx->r6 = ADD32(ctx->r29, 0X90);
    after_6:
    // 0x8018471C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80184720: addiu       $a1, $sp, 0x2C
    ctx->r5 = ADD32(ctx->r29, 0X2C);
    // 0x80184724: jal         0x80013CC4
    // 0x80184728: lw          $a2, 0x1D8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1D8);
    func_80013CC4(rdram, ctx);
        goto after_7;
    // 0x80184728: lw          $a2, 0x1D8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1D8);
    after_7:
    // 0x8018472C: jal         0x80184978
    // 0x80184730: addiu       $a0, $sp, 0x64
    ctx->r4 = ADD32(ctx->r29, 0X64);
    func_80184978(rdram, ctx);
        goto after_8;
    // 0x80184730: addiu       $a0, $sp, 0x64
    ctx->r4 = ADD32(ctx->r29, 0X64);
    after_8:
    // 0x80184734: b           L_80184740
    // 0x80184738: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80184740;
    // 0x80184738: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8018473C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184740:
    // 0x80184740: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80184744: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80184748: addiu       $sp, $sp, 0x1D8
    ctx->r29 = ADD32(ctx->r29, 0X1D8);
    // 0x8018474C: jr          $ra
    // 0x80184750: nop

    return;
    // 0x80184750: nop

;}
RECOMP_FUNC void func_80184754(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184754: addiu       $sp, $sp, -0x1B8
    ctx->r29 = ADD32(ctx->r29, -0X1B8);
    // 0x80184758: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8018475C: sw          $a0, 0x1B8($sp)
    MEM_W(0X1B8, ctx->r29) = ctx->r4;
    // 0x80184760: sw          $a1, 0x1BC($sp)
    MEM_W(0X1BC, ctx->r29) = ctx->r5;
    // 0x80184764: sw          $a2, 0x1C0($sp)
    MEM_W(0X1C0, ctx->r29) = ctx->r6;
    // 0x80184768: sw          $zero, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = 0;
    // 0x8018476C: lw          $a2, 0x1BC($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1BC);
    // 0x80184770: lw          $a1, 0x1B8($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1B8);
    // 0x80184774: jal         0x80013C90
    // 0x80184778: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    func_80013C90(rdram, ctx);
        goto after_0;
    // 0x80184778: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    after_0:
    // 0x8018477C: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    // 0x80184780: jal         0x8003E958
    // 0x80184784: addiu       $a1, $sp, 0x30
    ctx->r5 = ADD32(ctx->r29, 0X30);
    func_8003E958(rdram, ctx);
        goto after_1;
    // 0x80184784: addiu       $a1, $sp, 0x30
    ctx->r5 = ADD32(ctx->r29, 0X30);
    after_1:
    // 0x80184788: lw          $t6, 0x30($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X30);
    // 0x8018478C: lwc1        $f4, 0x1C0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1C0);
    // 0x80184790: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184794: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x80184798: beq         $t7, $zero, L_80184808
    if (ctx->r15 == 0) {
        // 0x8018479C: lwc1        $f10, 0x3C($sp)
        ctx->f10.u32l = MEM_W(ctx->r29, 0X3C);
            goto L_80184808;
    }
    // 0x8018479C: lwc1        $f10, 0x3C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x801847A0: ldc1        $f8, -0x3260($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3260);
    // 0x801847A4: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801847A8: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x801847AC: add.d       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f0.d = ctx->f6.d + ctx->f8.d;
    // 0x801847B0: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x801847B4: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    // 0x801847B8: c.le.d      $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f16.d <= ctx->f0.d;
    // 0x801847BC: nop

    // 0x801847C0: bc1fl       L_8018480C
    if (!c1cs) {
        // 0x801847C4: lw          $t1, 0x30($sp)
        ctx->r9 = MEM_W(ctx->r29, 0X30);
            goto L_8018480C;
    }
    goto skip_0;
    // 0x801847C4: lw          $t1, 0x30($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X30);
    skip_0:
    // 0x801847C8: jal         0x801855FC
    // 0x801847CC: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    func_801855FC(rdram, ctx);
        goto after_2;
    // 0x801847CC: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    after_2:
    // 0x801847D0: beq         $v0, $zero, L_80184808
    if (ctx->r2 == 0) {
        // 0x801847D4: ldc1        $f0, 0x18($sp)
        CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
            goto L_80184808;
    }
    // 0x801847D4: ldc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
    // 0x801847D8: lwc1        $f18, 0x74($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X74);
    // 0x801847DC: lw          $t8, 0x1B8($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X1B8);
    // 0x801847E0: lw          $t9, 0x1BC($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X1BC);
    // 0x801847E4: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x801847E8: lwc1        $f8, 0x4($t8)
    ctx->f8.u32l = MEM_W(ctx->r24, 0X4);
    // 0x801847EC: sub.d       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f4.d - ctx->f0.d;
    // 0x801847F0: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x801847F4: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x801847F8: sub.d       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f6.d - ctx->f10.d;
    // 0x801847FC: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x80184800: swc1        $f18, 0x4($t9)
    MEM_W(0X4, ctx->r25) = ctx->f18.u32l;
    // 0x80184804: sw          $t0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r8;
L_80184808:
    // 0x80184808: lw          $t1, 0x30($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X30);
L_8018480C:
    // 0x8018480C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x80184810: andi        $t2, $t1, 0x8000
    ctx->r10 = ctx->r9 & 0X8000;
    // 0x80184814: beql        $t2, $zero, L_80184838
    if (ctx->r10 == 0) {
        // 0x80184818: lw          $t3, 0x30($sp)
        ctx->r11 = MEM_W(ctx->r29, 0X30);
            goto L_80184838;
    }
    goto skip_1;
    // 0x80184818: lw          $t3, 0x30($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X30);
    skip_1:
    // 0x8018481C: jal         0x801855FC
    // 0x80184820: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    func_801855FC(rdram, ctx);
        goto after_3;
    // 0x80184820: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    after_3:
    // 0x80184824: beql        $v0, $zero, L_80184838
    if (ctx->r2 == 0) {
        // 0x80184828: lw          $t3, 0x30($sp)
        ctx->r11 = MEM_W(ctx->r29, 0X30);
            goto L_80184838;
    }
    goto skip_2;
    // 0x80184828: lw          $t3, 0x30($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X30);
    skip_2:
    // 0x8018482C: jal         0x80184978
    // 0x80184830: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    func_80184978(rdram, ctx);
        goto after_4;
    // 0x80184830: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    after_4:
    // 0x80184834: lw          $t3, 0x30($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X30);
L_80184838:
    // 0x80184838: lwc1        $f4, 0x1C0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1C0);
    // 0x8018483C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184840: andi        $t4, $t3, 0x800
    ctx->r12 = ctx->r11 & 0X800;
    // 0x80184844: beql        $t4, $zero, L_801848C0
    if (ctx->r12 == 0) {
        // 0x80184848: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801848C0;
    }
    goto skip_3;
    // 0x80184848: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_3:
    // 0x8018484C: ldc1        $f6, -0x3258($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, -0X3258);
    // 0x80184850: cvt.d.s     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.d = CVT_D_S(ctx->f4.fl);
    // 0x80184854: lwc1        $f10, 0x9C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x80184858: add.d       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f6.d); 
    ctx->f0.d = ctx->f8.d + ctx->f6.d;
    // 0x8018485C: lw          $a0, 0x94($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X94);
    // 0x80184860: lw          $a1, 0xA0($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XA0);
    // 0x80184864: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x80184868: neg.d       $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = -ctx->f0.d;
    // 0x8018486C: c.le.d      $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f18.d <= ctx->f16.d;
    // 0x80184870: nop

    // 0x80184874: bc1fl       L_801848C0
    if (!c1cs) {
        // 0x80184878: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801848C0;
    }
    goto skip_4;
    // 0x80184878: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_4:
    // 0x8018487C: jal         0x801855FC
    // 0x80184880: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    func_801855FC(rdram, ctx);
        goto after_5;
    // 0x80184880: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    after_5:
    // 0x80184884: beq         $v0, $zero, L_801848BC
    if (ctx->r2 == 0) {
        // 0x80184888: ldc1        $f0, 0x18($sp)
        CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
            goto L_801848BC;
    }
    // 0x80184888: ldc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
    // 0x8018488C: lwc1        $f4, 0xD4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XD4);
    // 0x80184890: lw          $t5, 0x1B8($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X1B8);
    // 0x80184894: lw          $t6, 0x1BC($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X1BC);
    // 0x80184898: cvt.d.s     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.d = CVT_D_S(ctx->f4.fl);
    // 0x8018489C: lwc1        $f10, 0x4($t5)
    ctx->f10.u32l = MEM_W(ctx->r13, 0X4);
    // 0x801848A0: sub.d       $f6, $f8, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f8.d - ctx->f0.d;
    // 0x801848A4: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x801848A8: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x801848AC: sub.d       $f18, $f6, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = ctx->f6.d - ctx->f16.d;
    // 0x801848B0: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x801848B4: swc1        $f4, 0x4($t6)
    MEM_W(0X4, ctx->r14) = ctx->f4.u32l;
    // 0x801848B8: sw          $t7, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r15;
L_801848BC:
    // 0x801848BC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801848C0:
    // 0x801848C0: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x801848C4: addiu       $sp, $sp, 0x1B8
    ctx->r29 = ADD32(ctx->r29, 0X1B8);
    // 0x801848C8: jr          $ra
    // 0x801848CC: nop

    return;
    // 0x801848CC: nop

;}
RECOMP_FUNC void func_801848D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801848D0: addiu       $sp, $sp, -0x1B0
    ctx->r29 = ADD32(ctx->r29, -0X1B0);
    // 0x801848D4: sw          $a1, 0x1B4($sp)
    MEM_W(0X1B4, ctx->r29) = ctx->r5;
    // 0x801848D8: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x801848DC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801848E0: sw          $a0, 0x1B0($sp)
    MEM_W(0X1B0, ctx->r29) = ctx->r4;
    // 0x801848E4: sw          $a2, 0x1B8($sp)
    MEM_W(0X1B8, ctx->r29) = ctx->r6;
    // 0x801848E8: lw          $a2, 0x1B4($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1B4);
    // 0x801848EC: jal         0x80013C90
    // 0x801848F0: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    func_80013C90(rdram, ctx);
        goto after_0;
    // 0x801848F0: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    after_0:
    // 0x801848F4: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x801848F8: jal         0x8003E958
    // 0x801848FC: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    func_8003E958(rdram, ctx);
        goto after_1;
    // 0x801848FC: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    after_1:
    // 0x80184900: lw          $t6, 0x28($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X28);
    // 0x80184904: lw          $a0, 0x2C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X2C);
    // 0x80184908: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x8018490C: beql        $t7, $zero, L_80184968
    if (ctx->r15 == 0) {
        // 0x80184910: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184968;
    }
    goto skip_0;
    // 0x80184910: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80184914: jal         0x801855FC
    // 0x80184918: lw          $a1, 0x38($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X38);
    func_801855FC(rdram, ctx);
        goto after_2;
    // 0x80184918: lw          $a1, 0x38($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X38);
    after_2:
    // 0x8018491C: beql        $v0, $zero, L_80184968
    if (ctx->r2 == 0) {
        // 0x80184920: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184968;
    }
    goto skip_1;
    // 0x80184920: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80184924: jal         0x80184978
    // 0x80184928: addiu       $a0, $sp, 0x3C
    ctx->r4 = ADD32(ctx->r29, 0X3C);
    func_80184978(rdram, ctx);
        goto after_3;
    // 0x80184928: addiu       $a0, $sp, 0x3C
    ctx->r4 = ADD32(ctx->r29, 0X3C);
    after_3:
    // 0x8018492C: lwc1        $f4, 0x1B8($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1B8);
    // 0x80184930: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184934: ldc1        $f8, -0x3250($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3250);
    // 0x80184938: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x8018493C: lwc1        $f16, 0x34($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80184940: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x80184944: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80184948: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x8018494C: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x80184950: c.le.d      $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f18.d <= ctx->f10.d;
    // 0x80184954: nop

    // 0x80184958: bc1fl       L_80184968
    if (!c1cs) {
        // 0x8018495C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184968;
    }
    goto skip_2;
    // 0x8018495C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x80184960: sw          $t8, 0x6548($at)
    MEM_W(0X6548, ctx->r1) = ctx->r24;
    // 0x80184964: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80184968:
    // 0x80184968: addiu       $sp, $sp, 0x1B0
    ctx->r29 = ADD32(ctx->r29, 0X1B0);
    // 0x8018496C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80184970: jr          $ra
    // 0x80184974: nop

    return;
    // 0x80184974: nop

;}
RECOMP_FUNC void func_80184978(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184978: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8018497C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80184980: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80184984: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80184988: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8018498C: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    // 0x80184990: jal         0x80013E3C
    // 0x80184994: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80013E3C(rdram, ctx);
        goto after_0;
    // 0x80184994: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_0:
    // 0x80184998: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8018499C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x801849A0: addiu       $a0, $a0, -0x3068
    ctx->r4 = ADD32(ctx->r4, -0X3068);
    // 0x801849A4: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x801849A8: nop

    // 0x801849AC: bc1f        L_801849C4
    if (!c1cs) {
        // 0x801849B0: nop
    
            goto L_801849C4;
    }
    // 0x801849B0: nop

    // 0x801849B4: jal         0x80013E3C
    // 0x801849B8: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80013E3C(rdram, ctx);
        goto after_1;
    // 0x801849B8: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x801849BC: b           L_801849D0
    // 0x801849C0: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
        goto L_801849D0;
    // 0x801849C0: neg.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f2.fl = -ctx->f0.fl;
L_801849C4:
    // 0x801849C4: jal         0x80013E3C
    // 0x801849C8: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80013E3C(rdram, ctx);
        goto after_2;
    // 0x801849C8: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_2:
    // 0x801849CC: mov.s       $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    ctx->f2.fl = ctx->f0.fl;
L_801849D0:
    // 0x801849D0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801849D4: lwc1        $f6, -0x3248($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3248);
    // 0x801849D8: c.le.s      $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f2.fl <= ctx->f6.fl;
    // 0x801849DC: nop

    // 0x801849E0: bc1fl       L_80184AB4
    if (!c1cs) {
        // 0x801849E4: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80184AB4;
    }
    goto skip_0;
    // 0x801849E4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x801849E8: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x801849EC: lwc1        $f2, 0x0($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X0);
    // 0x801849F0: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x801849F4: nop

    // 0x801849F8: mul.s       $f10, $f2, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801849FC: jal         0x800A01E0
    // 0x80184A00: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_3;
    // 0x80184A00: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
    after_3:
    // 0x80184A04: lwc1        $f16, 0x4($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80184A08: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80184A0C: div.s       $f18, $f0, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = DIV_S(ctx->f0.fl, ctx->f16.fl);
    // 0x80184A10: c.lt.s      $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f18.fl < ctx->f4.fl;
    // 0x80184A14: nop

    // 0x80184A18: bc1fl       L_80184A50
    if (!c1cs) {
        // 0x80184A1C: lwc1        $f0, 0x8($s0)
        ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
            goto L_80184A50;
    }
    goto skip_1;
    // 0x80184A1C: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    skip_1:
    // 0x80184A20: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80184A24: lwc1        $f2, 0x0($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80184A28: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80184A2C: nop

    // 0x80184A30: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80184A34: jal         0x800A01E0
    // 0x80184A38: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_4;
    // 0x80184A38: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
    after_4:
    // 0x80184A3C: lwc1        $f10, 0x4($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80184A40: div.s       $f2, $f0, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = DIV_S(ctx->f0.fl, ctx->f10.fl);
    // 0x80184A44: b           L_80184A70
    // 0x80184A48: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
        goto L_80184A70;
    // 0x80184A48: neg.s       $f2, $f2
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f2.fl = -ctx->f2.fl;
    // 0x80184A4C: lwc1        $f0, 0x8($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X8);
L_80184A50:
    // 0x80184A50: lwc1        $f2, 0x0($s0)
    ctx->f2.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80184A54: mul.s       $f16, $f0, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80184A58: nop

    // 0x80184A5C: mul.s       $f18, $f2, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80184A60: jal         0x800A01E0
    // 0x80184A64: add.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f18.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_5;
    // 0x80184A64: add.s       $f12, $f16, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = ctx->f16.fl + ctx->f18.fl;
    after_5:
    // 0x80184A68: lwc1        $f4, 0x4($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80184A6C: div.s       $f2, $f0, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = DIV_S(ctx->f0.fl, ctx->f4.fl);
L_80184A70:
    // 0x80184A70: lui         $at, 0xC01C
    ctx->r1 = S32(0XC01C << 16);
    // 0x80184A74: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x80184A78: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x80184A7C: cvt.d.s     $f6, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f6.d = CVT_D_S(ctx->f2.fl);
    // 0x80184A80: lui         $v0, 0x8019
    ctx->r2 = S32(0X8019 << 16);
    // 0x80184A84: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x80184A88: addiu       $v0, $v0, 0x6544
    ctx->r2 = ADD32(ctx->r2, 0X6544);
    // 0x80184A8C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184A90: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x80184A94: swc1        $f16, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f16.u32l;
    // 0x80184A98: lwc1        $f18, 0x0($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80184A9C: ldc1        $f6, -0x3240($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, -0X3240);
    // 0x80184AA0: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x80184AA4: sub.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f4.d - ctx->f6.d;
    // 0x80184AA8: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x80184AAC: swc1        $f10, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f10.u32l;
    // 0x80184AB0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80184AB4:
    // 0x80184AB4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80184AB8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80184ABC: jr          $ra
    // 0x80184AC0: nop

    return;
    // 0x80184AC0: nop

;}
RECOMP_FUNC void func_80184AC4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184AC4: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80184AC8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80184ACC: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80184AD0: sw          $a1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r5;
    // 0x80184AD4: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80184AD8: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x80184ADC: addiu       $a0, $v0, 0x18
    ctx->r4 = ADD32(ctx->r2, 0X18);
    // 0x80184AE0: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    // 0x80184AE4: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    // 0x80184AE8: jal         0x80184B68
    // 0x80184AEC: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    func_80184B68(rdram, ctx);
        goto after_0;
    // 0x80184AEC: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    after_0:
    // 0x80184AF0: beq         $v0, $zero, L_80184B1C
    if (ctx->r2 == 0) {
        // 0x80184AF4: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80184B1C;
    }
    // 0x80184AF4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80184AF8: beq         $v0, $at, L_80184B14
    if (ctx->r2 == ctx->r1) {
        // 0x80184AFC: lw          $a0, 0x30($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X30);
            goto L_80184B14;
    }
    // 0x80184AFC: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x80184B00: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80184B04: beql        $v0, $at, L_80184B20
    if (ctx->r2 == ctx->r1) {
        // 0x80184B08: lw          $a0, 0x1C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X1C);
            goto L_80184B20;
    }
    goto skip_0;
    // 0x80184B08: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80184B0C: b           L_80184B20
    // 0x80184B10: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
        goto L_80184B20;
    // 0x80184B10: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_80184B14:
    // 0x80184B14: jal         0x80185738
    // 0x80184B18: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    func_80185738(rdram, ctx);
        goto after_1;
    // 0x80184B18: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    after_1:
L_80184B1C:
    // 0x80184B1C: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_80184B20:
    // 0x80184B20: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x80184B24: jal         0x80184CD4
    // 0x80184B28: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    func_80184CD4(rdram, ctx);
        goto after_2;
    // 0x80184B28: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    after_2:
    // 0x80184B2C: beq         $v0, $zero, L_80184B58
    if (ctx->r2 == 0) {
        // 0x80184B30: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_80184B58;
    }
    // 0x80184B30: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80184B34: beq         $v0, $at, L_80184B50
    if (ctx->r2 == ctx->r1) {
        // 0x80184B38: lw          $a0, 0x30($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X30);
            goto L_80184B50;
    }
    // 0x80184B38: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x80184B3C: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80184B40: beql        $v0, $at, L_80184B5C
    if (ctx->r2 == ctx->r1) {
        // 0x80184B44: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184B5C;
    }
    goto skip_1;
    // 0x80184B44: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80184B48: b           L_80184B5C
    // 0x80184B4C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80184B5C;
    // 0x80184B4C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80184B50:
    // 0x80184B50: jal         0x80185738
    // 0x80184B54: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    func_80185738(rdram, ctx);
        goto after_3;
    // 0x80184B54: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    after_3:
L_80184B58:
    // 0x80184B58: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80184B5C:
    // 0x80184B5C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80184B60: jr          $ra
    // 0x80184B64: nop

    return;
    // 0x80184B64: nop

;}
RECOMP_FUNC void func_80184B68(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184B68: addiu       $sp, $sp, -0x1D8
    ctx->r29 = ADD32(ctx->r29, -0X1D8);
    // 0x80184B6C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80184B70: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80184B74: sw          $a0, 0x1D8($sp)
    MEM_W(0X1D8, ctx->r29) = ctx->r4;
    // 0x80184B78: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80184B7C: sw          $a2, 0x1E0($sp)
    MEM_W(0X1E0, ctx->r29) = ctx->r6;
    // 0x80184B80: jal         0x80013DE4
    // 0x80184B84: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    func_80013DE4(rdram, ctx);
        goto after_0;
    // 0x80184B84: addiu       $a0, $sp, 0x44
    ctx->r4 = ADD32(ctx->r29, 0X44);
    after_0:
    // 0x80184B88: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80184B8C: nop

    // 0x80184B90: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x80184B94: nop

    // 0x80184B98: bc1f        L_80184BA8
    if (!c1cs) {
        // 0x80184B9C: nop
    
            goto L_80184BA8;
    }
    // 0x80184B9C: nop

    // 0x80184BA0: b           L_80184CC0
    // 0x80184BA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80184CC0;
    // 0x80184BA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184BA8:
    // 0x80184BA8: jal         0x80013BD0
    // 0x80184BAC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80013BD0(rdram, ctx);
        goto after_1;
    // 0x80184BAC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x80184BB0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184BB4: ldc1        $f8, -0x3238($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3238);
    // 0x80184BB8: cvt.d.s     $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f6.d = CVT_D_S(ctx->f0.fl);
    // 0x80184BBC: addiu       $a0, $sp, 0x38
    ctx->r4 = ADD32(ctx->r29, 0X38);
    // 0x80184BC0: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x80184BC4: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    // 0x80184BC8: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x80184BCC: mfc1        $a2, $f16
    ctx->r6 = (int32_t)ctx->f16.u32l;
    // 0x80184BD0: jal         0x80013D58
    // 0x80184BD4: nop

    func_80013D58(rdram, ctx);
        goto after_2;
    // 0x80184BD4: nop

    after_2:
    // 0x80184BD8: lw          $a0, 0x1D8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1D8);
    // 0x80184BDC: addiu       $a1, $sp, 0x38
    ctx->r5 = ADD32(ctx->r29, 0X38);
    // 0x80184BE0: jal         0x8003F01C
    // 0x80184BE4: addiu       $a2, $sp, 0x50
    ctx->r6 = ADD32(ctx->r29, 0X50);
    func_8003F01C(rdram, ctx);
        goto after_3;
    // 0x80184BE4: addiu       $a2, $sp, 0x50
    ctx->r6 = ADD32(ctx->r29, 0X50);
    after_3:
    // 0x80184BE8: lw          $t6, 0x50($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X50);
    // 0x80184BEC: lw          $a0, 0x54($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X54);
    // 0x80184BF0: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x80184BF4: bgezl       $t7, L_80184CC0
    if (SIGNED(ctx->r15) >= 0) {
        // 0x80184BF8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80184CC0;
    }
    goto skip_0;
    // 0x80184BF8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80184BFC: jal         0x801855FC
    // 0x80184C00: lw          $a1, 0x60($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X60);
    func_801855FC(rdram, ctx);
        goto after_4;
    // 0x80184C00: lw          $a1, 0x60($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X60);
    after_4:
    // 0x80184C04: bne         $v0, $zero, L_80184C14
    if (ctx->r2 != 0) {
        // 0x80184C08: lui         $t8, 0x8019
        ctx->r24 = S32(0X8019 << 16);
            goto L_80184C14;
    }
    // 0x80184C08: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80184C0C: b           L_80184CC0
    // 0x80184C10: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80184CC0;
    // 0x80184C10: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184C14:
    // 0x80184C14: lw          $t8, 0x6548($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X6548);
    // 0x80184C18: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80184C1C: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x80184C20: bne         $t8, $zero, L_80184C3C
    if (ctx->r24 != 0) {
        // 0x80184C24: nop
    
            goto L_80184C3C;
    }
    // 0x80184C24: nop

    // 0x80184C28: jal         0x8018575C
    // 0x80184C2C: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    func_8018575C(rdram, ctx);
        goto after_5;
    // 0x80184C2C: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    after_5:
    // 0x80184C30: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80184C34: bne         $v0, $at, L_80184C44
    if (ctx->r2 != ctx->r1) {
        // 0x80184C38: lwc1        $f4, 0x64($sp)
        ctx->f4.u32l = MEM_W(ctx->r29, 0X64);
            goto L_80184C44;
    }
    // 0x80184C38: lwc1        $f4, 0x64($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X64);
L_80184C3C:
    // 0x80184C3C: b           L_80184CC0
    // 0x80184C40: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80184CC0;
    // 0x80184C40: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184C44:
    // 0x80184C44: lwc1        $f18, 0x0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80184C48: lwc1        $f8, 0x4($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80184C4C: lwc1        $f10, 0x68($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80184C50: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x80184C54: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80184C58: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x80184C5C: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80184C60: lwc1        $f8, 0x6C($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x80184C64: addiu       $a1, $sp, 0x64
    ctx->r5 = ADD32(ctx->r29, 0X64);
    // 0x80184C68: lui         $a2, 0x3D4C
    ctx->r6 = S32(0X3D4C << 16);
    // 0x80184C6C: mul.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x80184C70: add.s       $f18, $f6, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f6.fl + ctx->f16.fl;
    // 0x80184C74: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x80184C78: add.s       $f0, $f10, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = ctx->f10.fl + ctx->f18.fl;
    // 0x80184C7C: c.lt.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl < ctx->f6.fl;
    // 0x80184C80: nop

    // 0x80184C84: bc1fl       L_80184CC0
    if (!c1cs) {
        // 0x80184C88: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80184CC0;
    }
    goto skip_1;
    // 0x80184C88: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x80184C8C: jal         0x80013D58
    // 0x80184C90: ori         $a2, $a2, 0xCCCD
    ctx->r6 = ctx->r6 | 0XCCCD;
    func_80013D58(rdram, ctx);
        goto after_6;
    // 0x80184C90: ori         $a2, $a2, 0xCCCD
    ctx->r6 = ctx->r6 | 0XCCCD;
    after_6:
    // 0x80184C94: addiu       $a0, $sp, 0x2C
    ctx->r4 = ADD32(ctx->r29, 0X2C);
    // 0x80184C98: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80184C9C: jal         0x80013C90
    // 0x80184CA0: addiu       $a2, $sp, 0x90
    ctx->r6 = ADD32(ctx->r29, 0X90);
    func_80013C90(rdram, ctx);
        goto after_7;
    // 0x80184CA0: addiu       $a2, $sp, 0x90
    ctx->r6 = ADD32(ctx->r29, 0X90);
    after_7:
    // 0x80184CA4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80184CA8: addiu       $a1, $sp, 0x2C
    ctx->r5 = ADD32(ctx->r29, 0X2C);
    // 0x80184CAC: jal         0x80013CC4
    // 0x80184CB0: lw          $a2, 0x1D8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1D8);
    func_80013CC4(rdram, ctx);
        goto after_8;
    // 0x80184CB0: lw          $a2, 0x1D8($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1D8);
    after_8:
    // 0x80184CB4: b           L_80184CC0
    // 0x80184CB8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80184CC0;
    // 0x80184CB8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80184CBC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184CC0:
    // 0x80184CC0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80184CC4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80184CC8: addiu       $sp, $sp, 0x1D8
    ctx->r29 = ADD32(ctx->r29, 0X1D8);
    // 0x80184CCC: jr          $ra
    // 0x80184CD0: nop

    return;
    // 0x80184CD0: nop

;}
RECOMP_FUNC void func_80184CD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184CD4: addiu       $sp, $sp, -0x1B8
    ctx->r29 = ADD32(ctx->r29, -0X1B8);
    // 0x80184CD8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80184CDC: sw          $a0, 0x1B8($sp)
    MEM_W(0X1B8, ctx->r29) = ctx->r4;
    // 0x80184CE0: sw          $a1, 0x1BC($sp)
    MEM_W(0X1BC, ctx->r29) = ctx->r5;
    // 0x80184CE4: sw          $a2, 0x1C0($sp)
    MEM_W(0X1C0, ctx->r29) = ctx->r6;
    // 0x80184CE8: sw          $zero, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = 0;
    // 0x80184CEC: lw          $a2, 0x1BC($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1BC);
    // 0x80184CF0: lw          $a1, 0x1B8($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1B8);
    // 0x80184CF4: jal         0x80013C90
    // 0x80184CF8: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    func_80013C90(rdram, ctx);
        goto after_0;
    // 0x80184CF8: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    after_0:
    // 0x80184CFC: addiu       $a0, $sp, 0x20
    ctx->r4 = ADD32(ctx->r29, 0X20);
    // 0x80184D00: jal         0x8003E5D8
    // 0x80184D04: addiu       $a1, $sp, 0x30
    ctx->r5 = ADD32(ctx->r29, 0X30);
    func_8003E5D8(rdram, ctx);
        goto after_1;
    // 0x80184D04: addiu       $a1, $sp, 0x30
    ctx->r5 = ADD32(ctx->r29, 0X30);
    after_1:
    // 0x80184D08: lw          $t6, 0x30($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X30);
    // 0x80184D0C: lwc1        $f4, 0x1C0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1C0);
    // 0x80184D10: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184D14: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x80184D18: beq         $t7, $zero, L_80184DB4
    if (ctx->r15 == 0) {
        // 0x80184D1C: lwc1        $f10, 0x3C($sp)
        ctx->f10.u32l = MEM_W(ctx->r29, 0X3C);
            goto L_80184DB4;
    }
    // 0x80184D1C: lwc1        $f10, 0x3C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x80184D20: ldc1        $f8, -0x3230($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3230);
    // 0x80184D24: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x80184D28: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x80184D2C: add.d       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f0.d = ctx->f6.d + ctx->f8.d;
    // 0x80184D30: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80184D34: c.lt.d      $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f16.d < ctx->f0.d;
    // 0x80184D38: nop

    // 0x80184D3C: bc1fl       L_80184DB8
    if (!c1cs) {
        // 0x80184D40: lw          $t2, 0x30($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X30);
            goto L_80184DB8;
    }
    goto skip_0;
    // 0x80184D40: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
    skip_0:
    // 0x80184D44: lw          $t8, 0x6548($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X6548);
    // 0x80184D48: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80184D4C: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x80184D50: bnel        $t8, $zero, L_80184DB8
    if (ctx->r24 != 0) {
        // 0x80184D54: lw          $t2, 0x30($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X30);
            goto L_80184DB8;
    }
    goto skip_1;
    // 0x80184D54: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
    skip_1:
    // 0x80184D58: lw          $a0, -0x51D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X51D4);
    // 0x80184D5C: jal         0x8018575C
    // 0x80184D60: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    func_8018575C(rdram, ctx);
        goto after_2;
    // 0x80184D60: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    after_2:
    // 0x80184D64: bne         $v0, $zero, L_80184DB4
    if (ctx->r2 != 0) {
        // 0x80184D68: ldc1        $f0, 0x18($sp)
        CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
            goto L_80184DB4;
    }
    // 0x80184D68: ldc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
    // 0x80184D6C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x80184D70: lw          $a1, 0x40($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X40);
    // 0x80184D74: jal         0x801855FC
    // 0x80184D78: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    func_801855FC(rdram, ctx);
        goto after_3;
    // 0x80184D78: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    after_3:
    // 0x80184D7C: beq         $v0, $zero, L_80184DB4
    if (ctx->r2 == 0) {
        // 0x80184D80: ldc1        $f0, 0x18($sp)
        CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
            goto L_80184DB4;
    }
    // 0x80184D80: ldc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
    // 0x80184D84: lwc1        $f18, 0x74($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X74);
    // 0x80184D88: lw          $t9, 0x1B8($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X1B8);
    // 0x80184D8C: lw          $t0, 0x1BC($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1BC);
    // 0x80184D90: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x80184D94: lwc1        $f8, 0x4($t9)
    ctx->f8.u32l = MEM_W(ctx->r25, 0X4);
    // 0x80184D98: add.d       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f4.d + ctx->f0.d;
    // 0x80184D9C: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x80184DA0: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80184DA4: sub.d       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f6.d - ctx->f10.d;
    // 0x80184DA8: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x80184DAC: swc1        $f18, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->f18.u32l;
    // 0x80184DB0: sw          $t1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r9;
L_80184DB4:
    // 0x80184DB4: lw          $t2, 0x30($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X30);
L_80184DB8:
    // 0x80184DB8: lwc1        $f4, 0x1C0($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1C0);
    // 0x80184DBC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184DC0: andi        $t3, $t2, 0x800
    ctx->r11 = ctx->r10 & 0X800;
    // 0x80184DC4: beql        $t3, $zero, L_80184E40
    if (ctx->r11 == 0) {
        // 0x80184DC8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184E40;
    }
    goto skip_2;
    // 0x80184DC8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x80184DCC: ldc1        $f6, -0x3228($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, -0X3228);
    // 0x80184DD0: cvt.d.s     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.d = CVT_D_S(ctx->f4.fl);
    // 0x80184DD4: lwc1        $f16, 0x9C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x80184DD8: add.d       $f0, $f8, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f6.d); 
    ctx->f0.d = ctx->f8.d + ctx->f6.d;
    // 0x80184DDC: lw          $a0, 0x94($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X94);
    // 0x80184DE0: lw          $a1, 0xA0($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XA0);
    // 0x80184DE4: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x80184DE8: neg.d       $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); 
    ctx->f10.d = -ctx->f0.d;
    // 0x80184DEC: c.lt.d      $f10, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f10.d < ctx->f18.d;
    // 0x80184DF0: nop

    // 0x80184DF4: bc1fl       L_80184E40
    if (!c1cs) {
        // 0x80184DF8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184E40;
    }
    goto skip_3;
    // 0x80184DF8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_3:
    // 0x80184DFC: jal         0x801855FC
    // 0x80184E00: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    func_801855FC(rdram, ctx);
        goto after_4;
    // 0x80184E00: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    after_4:
    // 0x80184E04: beq         $v0, $zero, L_80184E3C
    if (ctx->r2 == 0) {
        // 0x80184E08: ldc1        $f0, 0x18($sp)
        CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
            goto L_80184E3C;
    }
    // 0x80184E08: ldc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
    // 0x80184E0C: lwc1        $f4, 0xD4($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0XD4);
    // 0x80184E10: lw          $t4, 0x1B8($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X1B8);
    // 0x80184E14: lw          $t5, 0x1BC($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X1BC);
    // 0x80184E18: cvt.d.s     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.d = CVT_D_S(ctx->f4.fl);
    // 0x80184E1C: lwc1        $f16, 0x4($t4)
    ctx->f16.u32l = MEM_W(ctx->r12, 0X4);
    // 0x80184E20: add.d       $f6, $f8, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = ctx->f8.d + ctx->f0.d;
    // 0x80184E24: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x80184E28: cvt.d.s     $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f10.d = CVT_D_S(ctx->f16.fl);
    // 0x80184E2C: sub.d       $f18, $f6, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = ctx->f6.d - ctx->f10.d;
    // 0x80184E30: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x80184E34: swc1        $f4, 0x4($t5)
    MEM_W(0X4, ctx->r13) = ctx->f4.u32l;
    // 0x80184E38: sw          $t6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r14;
L_80184E3C:
    // 0x80184E3C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80184E40:
    // 0x80184E40: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x80184E44: addiu       $sp, $sp, 0x1B8
    ctx->r29 = ADD32(ctx->r29, 0X1B8);
    // 0x80184E48: jr          $ra
    // 0x80184E4C: nop

    return;
    // 0x80184E4C: nop

;}
RECOMP_FUNC void func_80184E50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184E50: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80184E54: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80184E58: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80184E5C: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x80184E60: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80184E64: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x80184E68: addiu       $a1, $v1, 0x48
    ctx->r5 = ADD32(ctx->r3, 0X48);
    // 0x80184E6C: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80184E70: addiu       $a0, $v1, 0x18
    ctx->r4 = ADD32(ctx->r3, 0X18);
    // 0x80184E74: jal         0x80184F00
    // 0x80184E78: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    func_80184F00(rdram, ctx);
        goto after_0;
    // 0x80184E78: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    after_0:
    // 0x80184E7C: beq         $v0, $zero, L_80184EB4
    if (ctx->r2 == 0) {
        // 0x80184E80: lw          $v1, 0x2C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X2C);
            goto L_80184EB4;
    }
    // 0x80184E80: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x80184E84: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80184E88: beq         $v0, $at, L_80184EA4
    if (ctx->r2 == ctx->r1) {
        // 0x80184E8C: lw          $a0, 0x30($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X30);
            goto L_80184EA4;
    }
    // 0x80184E8C: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x80184E90: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80184E94: beql        $v0, $at, L_80184EB8
    if (ctx->r2 == ctx->r1) {
        // 0x80184E98: lw          $a0, 0x1C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X1C);
            goto L_80184EB8;
    }
    goto skip_0;
    // 0x80184E98: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80184E9C: b           L_80184EB8
    // 0x80184EA0: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
        goto L_80184EB8;
    // 0x80184EA0: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_80184EA4:
    // 0x80184EA4: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80184EA8: jal         0x80185738
    // 0x80184EAC: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    func_80185738(rdram, ctx);
        goto after_1;
    // 0x80184EAC: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    after_1:
    // 0x80184EB0: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
L_80184EB4:
    // 0x80184EB4: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_80184EB8:
    // 0x80184EB8: jal         0x80013BD0
    // 0x80184EBC: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    func_80013BD0(rdram, ctx);
        goto after_2;
    // 0x80184EBC: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    after_2:
    // 0x80184EC0: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184EC4: lwc1        $f4, -0x3220($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X3220);
    // 0x80184EC8: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x80184ECC: c.lt.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl < ctx->f4.fl;
    // 0x80184ED0: nop

    // 0x80184ED4: bc1fl       L_80184EF4
    if (!c1cs) {
        // 0x80184ED8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80184EF4;
    }
    goto skip_1;
    // 0x80184ED8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80184EDC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80184EE0: nop

    // 0x80184EE4: swc1        $f0, 0x50($v1)
    MEM_W(0X50, ctx->r3) = ctx->f0.u32l;
    // 0x80184EE8: swc1        $f0, 0x4C($v1)
    MEM_W(0X4C, ctx->r3) = ctx->f0.u32l;
    // 0x80184EEC: swc1        $f0, 0x48($v1)
    MEM_W(0X48, ctx->r3) = ctx->f0.u32l;
    // 0x80184EF0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80184EF4:
    // 0x80184EF4: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80184EF8: jr          $ra
    // 0x80184EFC: nop

    return;
    // 0x80184EFC: nop

;}
RECOMP_FUNC void func_80184F00(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80184F00: addiu       $sp, $sp, -0x470
    ctx->r29 = ADD32(ctx->r29, -0X470);
    // 0x80184F04: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80184F08: sw          $a0, 0x470($sp)
    MEM_W(0X470, ctx->r29) = ctx->r4;
    // 0x80184F0C: sw          $a1, 0x474($sp)
    MEM_W(0X474, ctx->r29) = ctx->r5;
    // 0x80184F10: sw          $a2, 0x478($sp)
    MEM_W(0X478, ctx->r29) = ctx->r6;
    // 0x80184F14: jal         0x80013DE4
    // 0x80184F18: addiu       $a0, $sp, 0x20C
    ctx->r4 = ADD32(ctx->r29, 0X20C);
    func_80013DE4(rdram, ctx);
        goto after_0;
    // 0x80184F18: addiu       $a0, $sp, 0x20C
    ctx->r4 = ADD32(ctx->r29, 0X20C);
    after_0:
    // 0x80184F1C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80184F20: nop

    // 0x80184F24: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x80184F28: nop

    // 0x80184F2C: bc1f        L_80184F3C
    if (!c1cs) {
        // 0x80184F30: nop
    
            goto L_80184F3C;
    }
    // 0x80184F30: nop

    // 0x80184F34: b           L_8018506C
    // 0x80184F38: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8018506C;
    // 0x80184F38: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184F3C:
    // 0x80184F3C: jal         0x80013BD0
    // 0x80184F40: lw          $a0, 0x474($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X474);
    func_80013BD0(rdram, ctx);
        goto after_1;
    // 0x80184F40: lw          $a0, 0x474($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X474);
    after_1:
    // 0x80184F44: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80184F48: ldc1        $f8, -0x3218($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3218);
    // 0x80184F4C: cvt.d.s     $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f6.d = CVT_D_S(ctx->f0.fl);
    // 0x80184F50: addiu       $a0, $sp, 0x200
    ctx->r4 = ADD32(ctx->r29, 0X200);
    // 0x80184F54: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x80184F58: addiu       $a1, $sp, 0x20C
    ctx->r5 = ADD32(ctx->r29, 0X20C);
    // 0x80184F5C: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x80184F60: mfc1        $a2, $f16
    ctx->r6 = (int32_t)ctx->f16.u32l;
    // 0x80184F64: jal         0x80013D58
    // 0x80184F68: nop

    func_80013D58(rdram, ctx);
        goto after_2;
    // 0x80184F68: nop

    after_2:
    // 0x80184F6C: addiu       $a0, $sp, 0x1E8
    ctx->r4 = ADD32(ctx->r29, 0X1E8);
    // 0x80184F70: lw          $a1, 0x470($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X470);
    // 0x80184F74: jal         0x80013C90
    // 0x80184F78: addiu       $a2, $sp, 0x200
    ctx->r6 = ADD32(ctx->r29, 0X200);
    func_80013C90(rdram, ctx);
        goto after_3;
    // 0x80184F78: addiu       $a2, $sp, 0x200
    ctx->r6 = ADD32(ctx->r29, 0X200);
    after_3:
    // 0x80184F7C: lw          $a0, 0x470($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X470);
    // 0x80184F80: addiu       $a1, $sp, 0x200
    ctx->r5 = ADD32(ctx->r29, 0X200);
    // 0x80184F84: addiu       $a2, $sp, 0x218
    ctx->r6 = ADD32(ctx->r29, 0X218);
    // 0x80184F88: jal         0x8003E2F0
    // 0x80184F8C: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
    func_8003E2F0(rdram, ctx);
        goto after_4;
    // 0x80184F8C: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
    after_4:
    // 0x80184F90: lw          $t6, 0x21C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X21C);
    // 0x80184F94: lw          $a0, 0x284($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X284);
    // 0x80184F98: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80184F9C: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80184FA0: bgez        $t7, L_8018506C
    if (SIGNED(ctx->r15) >= 0) {
        // 0x80184FA4: nop
    
            goto L_8018506C;
    }
    // 0x80184FA4: nop

    // 0x80184FA8: jal         0x801855FC
    // 0x80184FAC: lw          $a1, 0x290($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X290);
    func_801855FC(rdram, ctx);
        goto after_5;
    // 0x80184FAC: lw          $a1, 0x290($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X290);
    after_5:
    // 0x80184FB0: bne         $v0, $zero, L_80184FC0
    if (ctx->r2 != 0) {
        // 0x80184FB4: addiu       $a0, $sp, 0x1CC
        ctx->r4 = ADD32(ctx->r29, 0X1CC);
            goto L_80184FC0;
    }
    // 0x80184FB4: addiu       $a0, $sp, 0x1CC
    ctx->r4 = ADD32(ctx->r29, 0X1CC);
    // 0x80184FB8: b           L_8018506C
    // 0x80184FBC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8018506C;
    // 0x80184FBC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80184FC0:
    // 0x80184FC0: jal         0x80013CF8
    // 0x80184FC4: addiu       $a1, $sp, 0x2A4
    ctx->r5 = ADD32(ctx->r29, 0X2A4);
    func_80013CF8(rdram, ctx);
        goto after_6;
    // 0x80184FC4: addiu       $a1, $sp, 0x2A4
    ctx->r5 = ADD32(ctx->r29, 0X2A4);
    after_6:
    // 0x80184FC8: lwc1        $f18, 0x1D4($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X1D4);
    // 0x80184FCC: lwc1        $f4, 0x208($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X208);
    // 0x80184FD0: lwc1        $f8, 0x200($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X200);
    // 0x80184FD4: lwc1        $f10, 0x1CC($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X1CC);
    // 0x80184FD8: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x80184FDC: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80184FE0: addiu       $a1, $sp, 0x2A4
    ctx->r5 = ADD32(ctx->r29, 0X2A4);
    // 0x80184FE4: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80184FE8: addiu       $a0, $sp, 0x1B4
    ctx->r4 = ADD32(ctx->r29, 0X1B4);
    // 0x80184FEC: lui         $a2, 0x3A83
    ctx->r6 = S32(0X3A83 << 16);
    // 0x80184FF0: add.s       $f0, $f6, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f0.fl = ctx->f6.fl + ctx->f16.fl;
    // 0x80184FF4: c.lt.s      $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f0.fl < ctx->f18.fl;
    // 0x80184FF8: nop

    // 0x80184FFC: bc1f        L_8018503C
    if (!c1cs) {
        // 0x80185000: nop
    
            goto L_8018503C;
    }
    // 0x80185000: nop

    // 0x80185004: lui         $a2, 0x3A83
    ctx->r6 = S32(0X3A83 << 16);
    // 0x80185008: ori         $a2, $a2, 0x126F
    ctx->r6 = ctx->r6 | 0X126F;
    // 0x8018500C: jal         0x80013D58
    // 0x80185010: addiu       $a0, $sp, 0x1F4
    ctx->r4 = ADD32(ctx->r29, 0X1F4);
    func_80013D58(rdram, ctx);
        goto after_7;
    // 0x80185010: addiu       $a0, $sp, 0x1F4
    ctx->r4 = ADD32(ctx->r29, 0X1F4);
    after_7:
    // 0x80185014: addiu       $a0, $sp, 0x1F4
    ctx->r4 = ADD32(ctx->r29, 0X1F4);
    // 0x80185018: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8018501C: jal         0x80013C90
    // 0x80185020: addiu       $a2, $sp, 0x2C0
    ctx->r6 = ADD32(ctx->r29, 0X2C0);
    func_80013C90(rdram, ctx);
        goto after_8;
    // 0x80185020: addiu       $a2, $sp, 0x2C0
    ctx->r6 = ADD32(ctx->r29, 0X2C0);
    after_8:
    // 0x80185024: lw          $a0, 0x474($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X474);
    // 0x80185028: addiu       $a1, $sp, 0x1F4
    ctx->r5 = ADD32(ctx->r29, 0X1F4);
    // 0x8018502C: jal         0x80013CC4
    // 0x80185030: lw          $a2, 0x470($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X470);
    func_80013CC4(rdram, ctx);
        goto after_9;
    // 0x80185030: lw          $a2, 0x470($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X470);
    after_9:
    // 0x80185034: b           L_8018506C
    // 0x80185038: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8018506C;
    // 0x80185038: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8018503C:
    // 0x8018503C: jal         0x80013D58
    // 0x80185040: ori         $a2, $a2, 0x126F
    ctx->r6 = ctx->r6 | 0X126F;
    func_80013D58(rdram, ctx);
        goto after_10;
    // 0x80185040: ori         $a2, $a2, 0x126F
    ctx->r6 = ctx->r6 | 0X126F;
    after_10:
    // 0x80185044: addiu       $a0, $sp, 0x1C0
    ctx->r4 = ADD32(ctx->r29, 0X1C0);
    // 0x80185048: addiu       $a1, $sp, 0x2C0
    ctx->r5 = ADD32(ctx->r29, 0X2C0);
    // 0x8018504C: jal         0x80013C90
    // 0x80185050: addiu       $a2, $sp, 0x1B4
    ctx->r6 = ADD32(ctx->r29, 0X1B4);
    func_80013C90(rdram, ctx);
        goto after_11;
    // 0x80185050: addiu       $a2, $sp, 0x1B4
    ctx->r6 = ADD32(ctx->r29, 0X1B4);
    after_11:
    // 0x80185054: lw          $a0, 0x474($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X474);
    // 0x80185058: addiu       $a1, $sp, 0x1C0
    ctx->r5 = ADD32(ctx->r29, 0X1C0);
    // 0x8018505C: jal         0x80013CC4
    // 0x80185060: lw          $a2, 0x470($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X470);
    func_80013CC4(rdram, ctx);
        goto after_12;
    // 0x80185060: lw          $a2, 0x470($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X470);
    after_12:
    // 0x80185064: b           L_8018506C
    // 0x80185068: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8018506C;
    // 0x80185068: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8018506C:
    // 0x8018506C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80185070: addiu       $sp, $sp, 0x470
    ctx->r29 = ADD32(ctx->r29, 0X470);
    // 0x80185074: jr          $ra
    // 0x80185078: nop

    return;
    // 0x80185078: nop

;}
RECOMP_FUNC void func_8018507C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8018507C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80185080: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x80185084: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80185088: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8018508C: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80185090: mfc1        $a2, $f12
    ctx->r6 = (int32_t)ctx->f12.u32l;
    // 0x80185094: addiu       $a0, $v0, 0x18
    ctx->r4 = ADD32(ctx->r2, 0X18);
    // 0x80185098: jal         0x801850DC
    // 0x8018509C: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    func_801850DC(rdram, ctx);
        goto after_0;
    // 0x8018509C: addiu       $a1, $v0, 0x48
    ctx->r5 = ADD32(ctx->r2, 0X48);
    after_0:
    // 0x801850A0: beq         $v0, $zero, L_801850CC
    if (ctx->r2 == 0) {
        // 0x801850A4: addiu       $at, $zero, 0x1
        ctx->r1 = ADD32(0, 0X1);
            goto L_801850CC;
    }
    // 0x801850A4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801850A8: beq         $v0, $at, L_801850C4
    if (ctx->r2 == ctx->r1) {
        // 0x801850AC: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_801850C4;
    }
    // 0x801850AC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801850B0: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x801850B4: beql        $v0, $at, L_801850D0
    if (ctx->r2 == ctx->r1) {
        // 0x801850B8: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801850D0;
    }
    goto skip_0;
    // 0x801850B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801850BC: b           L_801850D0
    // 0x801850C0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801850D0;
    // 0x801850C0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801850C4:
    // 0x801850C4: jal         0x80185738
    // 0x801850C8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_80185738(rdram, ctx);
        goto after_1;
    // 0x801850C8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_1:
L_801850CC:
    // 0x801850CC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801850D0:
    // 0x801850D0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801850D4: jr          $ra
    // 0x801850D8: nop

    return;
    // 0x801850D8: nop

;}
RECOMP_FUNC void func_801850DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801850DC: addiu       $sp, $sp, -0x218
    ctx->r29 = ADD32(ctx->r29, -0X218);
    // 0x801850E0: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x801850E4: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x801850E8: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x801850EC: sw          $a0, 0x218($sp)
    MEM_W(0X218, ctx->r29) = ctx->r4;
    // 0x801850F0: sw          $a2, 0x220($sp)
    MEM_W(0X220, ctx->r29) = ctx->r6;
    // 0x801850F4: addiu       $t7, $t7, 0x65C4
    ctx->r15 = ADD32(ctx->r15, 0X65C4);
    // 0x801850F8: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x801850FC: addiu       $t6, $sp, 0x78
    ctx->r14 = ADD32(ctx->r29, 0X78);
    // 0x80185100: lw          $t8, 0x4($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X4);
    // 0x80185104: sw          $t9, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->r25;
    // 0x80185108: lw          $t9, 0x8($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X8);
    // 0x8018510C: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80185110: addiu       $t1, $t1, 0x65D0
    ctx->r9 = ADD32(ctx->r9, 0X65D0);
    // 0x80185114: sw          $t8, 0x4($t6)
    MEM_W(0X4, ctx->r14) = ctx->r24;
    // 0x80185118: sw          $t9, 0x8($t6)
    MEM_W(0X8, ctx->r14) = ctx->r25;
    // 0x8018511C: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x80185120: addiu       $t0, $sp, 0x6C
    ctx->r8 = ADD32(ctx->r29, 0X6C);
    // 0x80185124: lw          $t2, 0x4($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X4);
    // 0x80185128: sw          $t3, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r11;
    // 0x8018512C: lw          $t3, 0x8($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X8);
    // 0x80185130: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80185134: sw          $t2, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->r10;
    // 0x80185138: sw          $t3, 0x8($t0)
    MEM_W(0X8, ctx->r8) = ctx->r11;
    // 0x8018513C: lw          $a1, 0x218($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X218);
    // 0x80185140: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x80185144: jal         0x80013C90
    // 0x80185148: addiu       $a0, $sp, 0x84
    ctx->r4 = ADD32(ctx->r29, 0X84);
    func_80013C90(rdram, ctx);
        goto after_0;
    // 0x80185148: addiu       $a0, $sp, 0x84
    ctx->r4 = ADD32(ctx->r29, 0X84);
    after_0:
    // 0x8018514C: lwc1        $f4, 0x220($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X220);
    // 0x80185150: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80185154: ldc1        $f8, -0x3210($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3210);
    // 0x80185158: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x8018515C: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80185160: add.d       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f12.d = ctx->f6.d + ctx->f8.d;
    // 0x80185164: lui         $a3, 0x8019
    ctx->r7 = S32(0X8019 << 16);
    // 0x80185168: addiu       $t4, $sp, 0x90
    ctx->r12 = ADD32(ctx->r29, 0X90);
    // 0x8018516C: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    // 0x80185170: cvt.s.d     $f10, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.d); 
    ctx->f10.fl = CVT_S_D(ctx->f12.d);
    // 0x80185174: lw          $a3, 0x6544($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X6544);
    // 0x80185178: sdc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X30, ctx->r29);
    // 0x8018517C: lw          $a2, 0x6540($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X6540);
    // 0x80185180: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
    // 0x80185184: jal         0x8003F72C
    // 0x80185188: addiu       $a0, $sp, 0x84
    ctx->r4 = ADD32(ctx->r29, 0X84);
    func_8003F72C(rdram, ctx);
        goto after_1;
    // 0x80185188: addiu       $a0, $sp, 0x84
    ctx->r4 = ADD32(ctx->r29, 0X84);
    after_1:
    // 0x8018518C: lw          $t5, 0x90($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X90);
    // 0x80185190: ldc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X30);
    // 0x80185194: lw          $a0, 0x94($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X94);
    // 0x80185198: andi        $t6, $t5, 0x8000
    ctx->r14 = ctx->r13 & 0X8000;
    // 0x8018519C: beq         $t6, $zero, L_80185374
    if (ctx->r14 == 0) {
        // 0x801851A0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80185374;
    }
    // 0x801851A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801851A4: lw          $a1, 0xA0($sp)
    ctx->r5 = MEM_W(ctx->r29, 0XA0);
    // 0x801851A8: jal         0x801855FC
    // 0x801851AC: sdc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X30, ctx->r29);
    func_801855FC(rdram, ctx);
        goto after_2;
    // 0x801851AC: sdc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X30, ctx->r29);
    after_2:
    // 0x801851B0: beq         $v0, $zero, L_8018521C
    if (ctx->r2 == 0) {
        // 0x801851B4: ldc1        $f12, 0x30($sp)
        CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X30);
            goto L_8018521C;
    }
    // 0x801851B4: ldc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X30);
    // 0x801851B8: lwc1        $f16, 0x9C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X9C);
    // 0x801851BC: lwc1        $f8, 0xAC($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XAC);
    // 0x801851C0: lwc1        $f6, 0xBC($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0XBC);
    // 0x801851C4: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x801851C8: addiu       $a0, $sp, 0x78
    ctx->r4 = ADD32(ctx->r29, 0X78);
    // 0x801851CC: sub.d       $f4, $f12, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f12.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f12.d - ctx->f18.d;
    // 0x801851D0: lwc1        $f18, 0xA4($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XA4);
    // 0x801851D4: addiu       $a1, $sp, 0xA4
    ctx->r5 = ADD32(ctx->r29, 0XA4);
    // 0x801851D8: cvt.s.d     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f0.fl = CVT_S_D(ctx->f4.d);
    // 0x801851DC: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x801851E0: lwc1        $f8, 0xB4($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0XB4);
    // 0x801851E4: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x801851E8: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801851EC: mul.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x801851F0: nop

    // 0x801851F4: mul.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f8.fl);
    // 0x801851F8: add.s       $f2, $f16, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f2.fl = ctx->f16.fl + ctx->f6.fl;
    // 0x801851FC: c.le.s      $f10, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f10.fl <= ctx->f2.fl;
    // 0x80185200: nop

    // 0x80185204: bc1f        L_8018521C
    if (!c1cs) {
        // 0x80185208: nop
    
            goto L_8018521C;
    }
    // 0x80185208: nop

    // 0x8018520C: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80185210: jal         0x80013D58
    // 0x80185214: sdc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X30, ctx->r29);
    func_80013D58(rdram, ctx);
        goto after_3;
    // 0x80185214: sdc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X30, ctx->r29);
    after_3:
    // 0x80185218: ldc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X30);
L_8018521C:
    // 0x8018521C: lw          $t7, 0x90($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X90);
    // 0x80185220: lw          $a0, 0xF4($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XF4);
    // 0x80185224: lw          $a1, 0x100($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X100);
    // 0x80185228: andi        $t8, $t7, 0x800
    ctx->r24 = ctx->r15 & 0X800;
    // 0x8018522C: beql        $t8, $zero, L_801852A4
    if (ctx->r24 == 0) {
        // 0x80185230: lw          $v0, 0x218($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X218);
            goto L_801852A4;
    }
    goto skip_0;
    // 0x80185230: lw          $v0, 0x218($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X218);
    skip_0:
    // 0x80185234: jal         0x801855FC
    // 0x80185238: sdc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X30, ctx->r29);
    func_801855FC(rdram, ctx);
        goto after_4;
    // 0x80185238: sdc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    SD(ctx->f12.u64, 0X30, ctx->r29);
    after_4:
    // 0x8018523C: beq         $v0, $zero, L_801852A0
    if (ctx->r2 == 0) {
        // 0x80185240: ldc1        $f12, 0x30($sp)
        CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X30);
            goto L_801852A0;
    }
    // 0x80185240: ldc1        $f12, 0x30($sp)
    CHECK_FR(ctx, 12);
    ctx->f12.u64 = LD(ctx->r29, 0X30);
    // 0x80185244: lwc1        $f18, 0xFC($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0XFC);
    // 0x80185248: lwc1        $f6, 0x10C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X10C);
    // 0x8018524C: lwc1        $f16, 0x11C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X11C);
    // 0x80185250: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x80185254: addiu       $a0, $sp, 0x6C
    ctx->r4 = ADD32(ctx->r29, 0X6C);
    // 0x80185258: sub.d       $f8, $f12, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f12.d); NAN_CHECK(ctx->f4.d); 
    ctx->f8.d = ctx->f12.d - ctx->f4.d;
    // 0x8018525C: lwc1        $f4, 0x104($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X104);
    // 0x80185260: cvt.s.d     $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f0.fl = CVT_S_D(ctx->f8.d);
    // 0x80185264: mul.s       $f10, $f6, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80185268: lwc1        $f6, 0x114($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X114);
    // 0x8018526C: mul.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f10.fl);
    // 0x80185270: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80185274: mul.s       $f8, $f4, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80185278: nop

    // 0x8018527C: mul.s       $f16, $f8, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f6.fl);
    // 0x80185280: add.s       $f2, $f18, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f2.fl = ctx->f18.fl + ctx->f16.fl;
    // 0x80185284: c.le.s      $f10, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f10.fl <= ctx->f2.fl;
    // 0x80185288: nop

    // 0x8018528C: bc1f        L_801852A0
    if (!c1cs) {
        // 0x80185290: nop
    
            goto L_801852A0;
    }
    // 0x80185290: nop

    // 0x80185294: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80185298: jal         0x80013D58
    // 0x8018529C: addiu       $a1, $sp, 0x104
    ctx->r5 = ADD32(ctx->r29, 0X104);
    func_80013D58(rdram, ctx);
        goto after_5;
    // 0x8018529C: addiu       $a1, $sp, 0x104
    ctx->r5 = ADD32(ctx->r29, 0X104);
    after_5:
L_801852A0:
    // 0x801852A0: lw          $v0, 0x218($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X218);
L_801852A4:
    // 0x801852A4: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x801852A8: lwc1        $f18, 0x78($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X78);
    // 0x801852AC: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801852B0: lwc1        $f10, 0x6C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x801852B4: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x801852B8: add.s       $f6, $f4, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801852BC: addiu       $a2, $a2, 0x654C
    ctx->r6 = ADD32(ctx->r6, 0X654C);
    // 0x801852C0: addiu       $a0, $sp, 0x3C
    ctx->r4 = ADD32(ctx->r29, 0X3C);
    // 0x801852C4: addiu       $a1, $zero, 0x5
    ctx->r5 = ADD32(0, 0X5);
    // 0x801852C8: add.s       $f16, $f6, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f6.fl + ctx->f18.fl;
    // 0x801852CC: add.s       $f4, $f10, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x801852D0: lwc1        $f10, 0x7C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X7C);
    // 0x801852D4: swc1        $f4, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f4.u32l;
    // 0x801852D8: lwc1        $f6, 0x4($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X4);
    // 0x801852DC: lwc1        $f8, 0x4($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801852E0: lwc1        $f4, 0x70($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X70);
    // 0x801852E4: add.s       $f18, $f8, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f18.fl = ctx->f8.fl + ctx->f6.fl;
    // 0x801852E8: add.s       $f16, $f18, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f18.fl + ctx->f10.fl;
    // 0x801852EC: add.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x801852F0: lwc1        $f4, 0x80($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X80);
    // 0x801852F4: swc1        $f8, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f8.u32l;
    // 0x801852F8: lwc1        $f18, 0x8($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X8);
    // 0x801852FC: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80185300: lwc1        $f8, 0x74($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X74);
    // 0x80185304: add.s       $f10, $f6, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f18.fl;
    // 0x80185308: add.s       $f16, $f10, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x8018530C: add.s       $f6, $f8, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f16.fl;
    // 0x80185310: jal         0x80181C54
    // 0x80185314: swc1        $f6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f6.u32l;
    func_80181C54(rdram, ctx);
        goto after_6;
    // 0x80185314: swc1        $f6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f6.u32l;
    after_6:
    // 0x80185318: slti        $at, $v0, 0x2
    ctx->r1 = SIGNED(ctx->r2) < 0X2 ? 1 : 0;
    // 0x8018531C: beq         $at, $zero, L_8018535C
    if (ctx->r1 == 0) {
        // 0x80185320: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_8018535C;
    }
    // 0x80185320: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80185324: lwc1        $f18, 0x78($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X78);
    // 0x80185328: lwc1        $f10, 0x6C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x8018532C: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80185330: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80185334: add.s       $f4, $f18, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f10.fl;
    // 0x80185338: add.s       $f16, $f8, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x8018533C: lwc1        $f8, 0x8($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80185340: swc1        $f16, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f16.u32l;
    // 0x80185344: lwc1        $f18, 0x74($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X74);
    // 0x80185348: lwc1        $f6, 0x80($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X80);
    // 0x8018534C: add.s       $f10, $f6, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f18.fl;
    // 0x80185350: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80185354: b           L_80185374
    // 0x80185358: swc1        $f4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f4.u32l;
        goto L_80185374;
    // 0x80185358: swc1        $f4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f4.u32l;
L_8018535C:
    // 0x8018535C: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80185360: addiu       $a1, $a1, 0x654C
    ctx->r5 = ADD32(ctx->r5, 0X654C);
    // 0x80185364: jal         0x80013CC4
    // 0x80185368: lw          $a2, 0x218($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X218);
    func_80013CC4(rdram, ctx);
        goto after_7;
    // 0x80185368: lw          $a2, 0x218($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X218);
    after_7:
    // 0x8018536C: b           L_80185374
    // 0x80185370: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80185374;
    // 0x80185370: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80185374:
    // 0x80185374: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80185378: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8018537C: addiu       $sp, $sp, 0x218
    ctx->r29 = ADD32(ctx->r29, 0X218);
    // 0x80185380: jr          $ra
    // 0x80185384: nop

    return;
    // 0x80185384: nop

    // 0x80185388: jr          $ra
    // 0x8018538C: nop

    return;
    // 0x8018538C: nop

;}
RECOMP_FUNC void func_80185390(uint8_t* rdram, recomp_context* ctx) {
    {
        static int s390 = 0; s390++;
        if (s390 <= 5 || (s390 % 500 == 0)) {
            uint32_t flags = (uint32_t)MEM_W(S32(0x801CABC8), 0);
            uint32_t buf = (uint32_t)MEM_W(ctx->r4, 0x34);
            uint32_t buf98 = buf ? (uint32_t)MEM_W(S32(buf), 0x98) : 0;
            uint32_t buf0 = buf ? (uint32_t)MEM_W(S32(buf), 0) : 0;
            uint32_t buf4 = buf ? (uint32_t)MEM_W(S32(buf), 4) : 0;
            fprintf(stderr, "[80185390] #%d obj=0x%08X buf=0x%08X buf+0x98=0x%08X "
                    "bit28=%d bit30=%d bit31=%d buf[0]=0x%08X buf[4]=0x%08X\n",
                    s390, (uint32_t)ctx->r4, buf, buf98,
                    (int)((flags >> 28) & 1), (int)((buf98 >> 30) & 1), (int)((buf98 >> 31) & 1),
                    buf0, buf4);
        }
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80185390: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80185394: lw          $t6, -0x5438($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5438);
    // 0x80185398: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8018539C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801853A0: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x801853A4: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x801853A8: bgez        $t7, L_801854C4
    if (SIGNED(ctx->r15) >= 0) {
        // 0x801853AC: lw          $v0, 0x34($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X34);
            goto L_801854C4;
    }
    // 0x801853AC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x801853B0: lw          $v1, 0x98($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X98);
    // 0x801853B4: sll         $t9, $v1, 1
    ctx->r25 = S32(ctx->r3 << 1);
    // 0x801853B8: bgez        $t9, L_801854C4
    if (SIGNED(ctx->r25) >= 0) {
        // 0x801853BC: srl         $t0, $v1, 31
        ctx->r8 = S32(U32(ctx->r3) >> 31);
            goto L_801854C4;
    }
    // 0x801853BC: srl         $t0, $v1, 31
    ctx->r8 = S32(U32(ctx->r3) >> 31);
    // 0x801853C0: bnel        $t0, $zero, L_801854C8
    if (ctx->r8 != 0) {
        // 0x801853C4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801854C8;
    }
    goto skip_0;
    // 0x801853C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801853C8: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x801853CC: addiu       $at, $zero, 0x8
    ctx->r1 = ADD32(0, 0X8);
    // 0x801853D0: beql        $v1, $zero, L_801853E4
    if (ctx->r3 == 0) {
        // 0x801853D4: lw          $t1, 0x4($v0)
        ctx->r9 = MEM_W(ctx->r2, 0X4);
            goto L_801853E4;
    }
    goto skip_1;
    // 0x801853D4: lw          $t1, 0x4($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X4);
    skip_1:
    // 0x801853D8: bnel        $v1, $at, L_801854C8
    if (ctx->r3 != ctx->r1) {
        // 0x801853DC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801854C8;
    }
    goto skip_2;
    // 0x801853DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x801853E0: lw          $t1, 0x4($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X4);
L_801853E4:
    // 0x801853E4: addiu       $t2, $t1, -0x9
    ctx->r10 = ADD32(ctx->r9, -0X9);
    // 0x801853E8: sltiu       $at, $t2, 0x2B
    ctx->r1 = ctx->r10 < 0X2B ? 1 : 0;
    // 0x801853EC: beq         $at, $zero, L_801854C4
    if (ctx->r1 == 0) {
        // 0x801853F0: sll         $t2, $t2, 2
        ctx->r10 = S32(ctx->r10 << 2);
            goto L_801854C4;
    }
    // 0x801853F0: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x801853F4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801853F8: addu        $at, $at, $t2
    gpr jr_addend_80185400 = ctx->r10;
    ctx->r1 = ADD32(ctx->r1, ctx->r10);
    // 0x801853FC: lw          $t2, -0x3208($at)
    ctx->r10 = ADD32(ctx->r1, -0X3208);
    // 0x80185400: jr          $t2
    // 0x80185404: nop

    switch (jr_addend_80185400 >> 2) {
        case 0: goto L_80185450; break;
        case 1: goto L_80185450; break;
        case 2: goto L_801854C4; break;
        case 3: goto L_80185450; break;
        case 4: goto L_80185450; break;
        case 5: goto L_80185450; break;
        case 6: goto L_80185450; break;
        case 7: goto L_801854C4; break;
        case 8: goto L_80185450; break;
        case 9: goto L_801854C4; break;
        case 10: goto L_80185450; break;
        case 11: goto L_801854C4; break;
        case 12: goto L_801854C4; break;
        case 13: goto L_801854C4; break;
        case 14: goto L_801854C4; break;
        case 15: goto L_801854C4; break;
        case 16: goto L_801854C4; break;
        case 17: goto L_80185450; break;
        case 18: goto L_80185450; break;
        case 19: goto L_80185450; break;
        case 20: goto L_80185450; break;
        case 21: goto L_80185450; break;
        case 22: goto L_80185450; break;
        case 23: goto L_80185450; break;
        case 24: goto L_80185450; break;
        case 25: goto L_801854C4; break;
        case 26: goto L_801854C4; break;
        case 27: goto L_801854C4; break;
        case 28: goto L_801854C4; break;
        case 29: goto L_801854C4; break;
        case 30: goto L_801854C4; break;
        case 31: goto L_801854C4; break;
        case 32: goto L_801854C4; break;
        case 33: goto L_801854C4; break;
        case 34: goto L_801854C4; break;
        case 35: goto L_801854C4; break;
        case 36: goto L_801854C4; break;
        case 37: goto L_801854C4; break;
        case 38: goto L_801854C4; break;
        case 39: goto L_80185450; break;
        case 40: goto L_80185450; break;
        case 41: goto L_801854C4; break;
        case 42: goto L_80185408; break;
        default: switch_error(__func__, 0x80185400, 0x8019CDF8);
    }
    // 0x80185404: nop

L_80185408:
    // 0x80185408: lw          $a0, 0x100($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X100);
    // 0x8018540C: beql        $a0, $zero, L_801854C8
    if (ctx->r4 == 0) {
        // 0x80185410: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801854C8;
    }
    goto skip_3;
    // 0x80185410: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_3:
    // 0x80185414: lw          $t3, 0x4($a0)
    ctx->r11 = MEM_W(ctx->r4, 0X4);
    // 0x80185418: sll         $t4, $t3, 3
    ctx->r12 = S32(ctx->r11 << 3);
    // 0x8018541C: addu        $t4, $t4, $t3
    ctx->r12 = ADD32(ctx->r12, ctx->r11);
    // 0x80185420: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x80185424: addu        $t5, $v0, $t4
    ctx->r13 = ADD32(ctx->r2, ctx->r12);
    // 0x80185428: lbu         $v1, 0x123($t5)
    ctx->r3 = MEM_BU(ctx->r13, 0X123);
    // 0x8018542C: addiu       $t6, $v1, -0x7
    ctx->r14 = ADD32(ctx->r3, -0X7);
    // 0x80185430: sltiu       $at, $t6, 0x9
    ctx->r1 = ctx->r14 < 0X9 ? 1 : 0;
    // 0x80185434: beq         $at, $zero, L_801854C4
    if (ctx->r1 == 0) {
        // 0x80185438: sll         $t6, $t6, 2
        ctx->r14 = S32(ctx->r14 << 2);
            goto L_801854C4;
    }
    // 0x80185438: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x8018543C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80185440: addu        $at, $at, $t6
    gpr jr_addend_80185448 = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x80185444: lw          $t6, -0x315C($at)
    ctx->r14 = ADD32(ctx->r1, -0X315C);
    // 0x80185448: jr          $t6
    // 0x8018544C: nop

    switch (jr_addend_80185448 >> 2) {
        case 0: goto L_80185450; break;
        case 1: goto L_80185450; break;
        case 2: goto L_80185450; break;
        case 3: goto L_80185450; break;
        case 4: goto L_801854C4; break;
        case 5: goto L_80185450; break;
        case 6: goto L_801854C4; break;
        case 7: goto L_801854C4; break;
        case 8: goto L_80185450; break;
        default: switch_error(__func__, 0x80185448, 0x8019CEA4);
    }
    // 0x8018544C: nop

L_80185450:
    // 0x80185450: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80185454: jal         0x80185750
    // 0x80185458: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    func_80185750(rdram, ctx);
        goto after_0;
    // 0x80185458: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    after_0:
    // 0x8018545C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80185460: lwc1        $f0, -0x3138($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X3138);
    // 0x80185464: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80185468: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8018546C: swc1        $f0, 0x6540($at)
    MEM_W(0X6540, ctx->r1) = ctx->f0.u32l;
    // 0x80185470: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80185474: swc1        $f0, 0x6544($at)
    MEM_W(0X6544, ctx->r1) = ctx->f0.u32l;
    // 0x80185478: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8018547C: sw          $zero, 0x6548($at)
    MEM_W(0X6548, ctx->r1) = 0;
    // 0x80185480: jal         0x80184520
    // 0x80185484: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    func_80184520(rdram, ctx);
        goto after_1;
    // 0x80185484: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    after_1:
    // 0x80185488: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8018548C: jal         0x80184E50
    // 0x80185490: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    func_80184E50(rdram, ctx);
        goto after_2;
    // 0x80185490: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    after_2:
    // 0x80185494: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80185498: jal         0x801845E0
    // 0x8018549C: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    func_801845E0(rdram, ctx);
        goto after_3;
    // 0x8018549C: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    after_3:
    // 0x801854A0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801854A4: jal         0x80184AC4
    // 0x801854A8: lui         $a1, 0x40F0
    ctx->r5 = S32(0X40F0 << 16);
    func_80184AC4(rdram, ctx);
        goto after_4;
    // 0x801854A8: lui         $a1, 0x40F0
    ctx->r5 = S32(0X40F0 << 16);
    after_4:
    // 0x801854AC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801854B0: jal         0x8018507C
    // 0x801854B4: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    func_8018507C(rdram, ctx);
        goto after_5;
    // 0x801854B4: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    after_5:
    // 0x801854B8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801854BC: jal         0x80184580
    // 0x801854C0: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    func_80184580(rdram, ctx);
        goto after_6;
    // 0x801854C0: lui         $a1, 0x40E0
    ctx->r5 = S32(0X40E0 << 16);
    after_6:
L_801854C4:
    // 0x801854C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801854C8:
    // 0x801854C8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801854CC: jr          $ra
    // 0x801854D0: nop

    return;
    // 0x801854D0: nop

;}
