#include "recomp.h"
#include "funcs.h"
#include <stdio.h>

RECOMP_FUNC void func_8008FCA4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FCA4: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8008FCA8: addiu       $t6, $t6, -0x7D40
    ctx->r14 = ADD32(ctx->r14, -0X7D40);
    // 0x8008FCAC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8008FCB0: lb          $t7, 0x2883($v0)
    ctx->r15 = MEM_B(ctx->r2, 0X2883);
    // 0x8008FCB4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8008FCB8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8008FCBC: subu        $v1, $t7, $a1
    ctx->r3 = SUB32(ctx->r15, ctx->r5);
    // 0x8008FCC0: bltz        $v1, L_8008FD0C
    if (SIGNED(ctx->r3) < 0) {
        // 0x8008FCC4: lui         $t8, 0x801D
        ctx->r24 = S32(0X801D << 16);
            goto L_8008FD0C;
    }
    // 0x8008FCC4: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x8008FCC8: addiu       $t8, $t8, -0x7D3E
    ctx->r24 = ADD32(ctx->r24, -0X7D3E);
    // 0x8008FCCC: bne         $v0, $t8, L_8008FD18
    if (ctx->r2 != ctx->r24) {
        // 0x8008FCD0: sb          $v1, 0x2883($v0)
        MEM_B(0X2883, ctx->r2) = ctx->r3;
            goto L_8008FD18;
    }
    // 0x8008FCD0: sb          $v1, 0x2883($v0)
    MEM_B(0X2883, ctx->r2) = ctx->r3;
    // 0x8008FCD4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8008FCD8: addiu       $t9, $t9, 0x2560
    ctx->r25 = ADD32(ctx->r25, 0X2560);
    // 0x8008FCDC: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    // 0x8008FCE0: jalr        $t9
    // 0x8008FCE4: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8008FCE4: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x8008FCE8: beq         $v0, $zero, L_8008FCF8
    if (ctx->r2 == 0) {
        // 0x8008FCEC: lw          $a1, 0x1C($sp)
        ctx->r5 = MEM_W(ctx->r29, 0X1C);
            goto L_8008FCF8;
    }
    // 0x8008FCEC: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x8008FCF0: b           L_8008FCFC
    // 0x8008FCF4: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
        goto L_8008FCFC;
    // 0x8008FCF4: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
L_8008FCF8:
    // 0x8008FCF8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8008FCFC:
    // 0x8008FCFC: lbu         $t0, 0x0($v1)
    ctx->r8 = MEM_BU(ctx->r3, 0X0);
    // 0x8008FD00: ori         $t1, $t0, 0x8
    ctx->r9 = ctx->r8 | 0X8;
    // 0x8008FD04: b           L_8008FD18
    // 0x8008FD08: sb          $t1, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r9;
        goto L_8008FD18;
    // 0x8008FD08: sb          $t1, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r9;
L_8008FD0C:
    // 0x8008FD0C: sll         $v0, $v1, 24
    ctx->r2 = S32(ctx->r3 << 24);
    // 0x8008FD10: b           L_8008FD20
    // 0x8008FD14: sra         $v0, $v0, 24
    ctx->r2 = S32(SIGNED(ctx->r2) >> 24);
        goto L_8008FD20;
    // 0x8008FD14: sra         $v0, $v0, 24
    ctx->r2 = S32(SIGNED(ctx->r2) >> 24);
L_8008FD18:
    // 0x8008FD18: sll         $v0, $a1, 24
    ctx->r2 = S32(ctx->r5 << 24);
    // 0x8008FD1C: sra         $v0, $v0, 24
    ctx->r2 = S32(SIGNED(ctx->r2) >> 24);
L_8008FD20:
    // 0x8008FD20: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8008FD24: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8008FD28: jr          $ra
    // 0x8008FD2C: nop

    return;
    // 0x8008FD2C: nop

;}
RECOMP_FUNC void func_8008FD30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FD30: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8008FD34: addu        $v0, $v0, $a0
    ctx->r2 = ADD32(ctx->r2, ctx->r4);
    // 0x8008FD38: jr          $ra
    // 0x8008FD3C: lb          $v0, -0x54BD($v0)
    ctx->r2 = MEM_B(ctx->r2, -0X54BD);
    return;
    // 0x8008FD3C: lb          $v0, -0x54BD($v0)
    ctx->r2 = MEM_B(ctx->r2, -0X54BD);
;}
RECOMP_FUNC void func_8008FD40(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FD40: blez        $a0, L_8008FD5C
    if (SIGNED(ctx->r4) <= 0) {
        // 0x8008FD44: addiu       $sp, $sp, -0x8
        ctx->r29 = ADD32(ctx->r29, -0X8);
            goto L_8008FD5C;
    }
    // 0x8008FD44: addiu       $sp, $sp, -0x8
    ctx->r29 = ADD32(ctx->r29, -0X8);
    // 0x8008FD48: slti        $at, $a0, 0x7
    ctx->r1 = SIGNED(ctx->r4) < 0X7 ? 1 : 0;
    // 0x8008FD4C: beq         $at, $zero, L_8008FD5C
    if (ctx->r1 == 0) {
        // 0x8008FD50: addiu       $v1, $zero, 0x63
        ctx->r3 = ADD32(0, 0X63);
            goto L_8008FD5C;
    }
    // 0x8008FD50: addiu       $v1, $zero, 0x63
    ctx->r3 = ADD32(0, 0X63);
    // 0x8008FD54: b           L_8008FDE4
    // 0x8008FD58: sw          $v1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r3;
        goto L_8008FDE4;
    // 0x8008FD58: sw          $v1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r3;
L_8008FD5C:
    // 0x8008FD5C: slti        $at, $a0, 0x7
    ctx->r1 = SIGNED(ctx->r4) < 0X7 ? 1 : 0;
    // 0x8008FD60: bne         $at, $zero, L_8008FD78
    if (ctx->r1 != 0) {
        // 0x8008FD64: slti        $at, $a0, 0xD
        ctx->r1 = SIGNED(ctx->r4) < 0XD ? 1 : 0;
            goto L_8008FD78;
    }
    // 0x8008FD64: slti        $at, $a0, 0xD
    ctx->r1 = SIGNED(ctx->r4) < 0XD ? 1 : 0;
    // 0x8008FD68: beq         $at, $zero, L_8008FD78
    if (ctx->r1 == 0) {
        // 0x8008FD6C: addiu       $v1, $zero, 0xA
        ctx->r3 = ADD32(0, 0XA);
            goto L_8008FD78;
    }
    // 0x8008FD6C: addiu       $v1, $zero, 0xA
    ctx->r3 = ADD32(0, 0XA);
    // 0x8008FD70: b           L_8008FDE4
    // 0x8008FD74: sw          $v1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r3;
        goto L_8008FDE4;
    // 0x8008FD74: sw          $v1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r3;
L_8008FD78:
    // 0x8008FD78: slti        $at, $a0, 0xD
    ctx->r1 = SIGNED(ctx->r4) < 0XD ? 1 : 0;
    // 0x8008FD7C: bne         $at, $zero, L_8008FD94
    if (ctx->r1 != 0) {
        // 0x8008FD80: slti        $at, $a0, 0x11
        ctx->r1 = SIGNED(ctx->r4) < 0X11 ? 1 : 0;
            goto L_8008FD94;
    }
    // 0x8008FD80: slti        $at, $a0, 0x11
    ctx->r1 = SIGNED(ctx->r4) < 0X11 ? 1 : 0;
    // 0x8008FD84: beql        $at, $zero, L_8008FD98
    if (ctx->r1 == 0) {
        // 0x8008FD88: slti        $at, $a0, 0x11
        ctx->r1 = SIGNED(ctx->r4) < 0X11 ? 1 : 0;
            goto L_8008FD98;
    }
    goto skip_0;
    // 0x8008FD88: slti        $at, $a0, 0x11
    ctx->r1 = SIGNED(ctx->r4) < 0X11 ? 1 : 0;
    skip_0:
    // 0x8008FD8C: b           L_8008FE00
    // 0x8008FD90: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8008FE00;
    // 0x8008FD90: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8008FD94:
    // 0x8008FD94: slti        $at, $a0, 0x11
    ctx->r1 = SIGNED(ctx->r4) < 0X11 ? 1 : 0;
L_8008FD98:
    // 0x8008FD98: bne         $at, $zero, L_8008FDC8
    if (ctx->r1 != 0) {
        // 0x8008FD9C: slti        $at, $a0, 0x2E
        ctx->r1 = SIGNED(ctx->r4) < 0X2E ? 1 : 0;
            goto L_8008FDC8;
    }
    // 0x8008FD9C: slti        $at, $a0, 0x2E
    ctx->r1 = SIGNED(ctx->r4) < 0X2E ? 1 : 0;
    // 0x8008FDA0: beq         $at, $zero, L_8008FDC8
    if (ctx->r1 == 0) {
        // 0x8008FDA4: addiu       $at, $zero, 0x14
        ctx->r1 = ADD32(0, 0X14);
            goto L_8008FDC8;
    }
    // 0x8008FDA4: addiu       $at, $zero, 0x14
    ctx->r1 = ADD32(0, 0X14);
    // 0x8008FDA8: beq         $a0, $at, L_8008FDB8
    if (ctx->r4 == ctx->r1) {
        // 0x8008FDAC: addiu       $at, $zero, 0x15
        ctx->r1 = ADD32(0, 0X15);
            goto L_8008FDB8;
    }
    // 0x8008FDAC: addiu       $at, $zero, 0x15
    ctx->r1 = ADD32(0, 0X15);
    // 0x8008FDB0: bne         $a0, $at, L_8008FDC0
    if (ctx->r4 != ctx->r1) {
        // 0x8008FDB4: addiu       $v1, $zero, 0x1
        ctx->r3 = ADD32(0, 0X1);
            goto L_8008FDC0;
    }
    // 0x8008FDB4: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_8008FDB8:
    // 0x8008FDB8: b           L_8008FDC0
    // 0x8008FDBC: addiu       $v1, $zero, 0xA
    ctx->r3 = ADD32(0, 0XA);
        goto L_8008FDC0;
    // 0x8008FDBC: addiu       $v1, $zero, 0xA
    ctx->r3 = ADD32(0, 0XA);
L_8008FDC0:
    // 0x8008FDC0: b           L_8008FDE4
    // 0x8008FDC4: sw          $v1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r3;
        goto L_8008FDE4;
    // 0x8008FDC4: sw          $v1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r3;
L_8008FDC8:
    // 0x8008FDC8: slti        $at, $a0, 0x2E
    ctx->r1 = SIGNED(ctx->r4) < 0X2E ? 1 : 0;
    // 0x8008FDCC: bne         $at, $zero, L_8008FDE4
    if (ctx->r1 != 0) {
        // 0x8008FDD0: slti        $at, $a0, 0x31
        ctx->r1 = SIGNED(ctx->r4) < 0X31 ? 1 : 0;
            goto L_8008FDE4;
    }
    // 0x8008FDD0: slti        $at, $a0, 0x31
    ctx->r1 = SIGNED(ctx->r4) < 0X31 ? 1 : 0;
    // 0x8008FDD4: beq         $at, $zero, L_8008FDE4
    if (ctx->r1 == 0) {
        // 0x8008FDD8: lui         $v1, 0x1
        ctx->r3 = S32(0X1 << 16);
            goto L_8008FDE4;
    }
    // 0x8008FDD8: lui         $v1, 0x1
    ctx->r3 = S32(0X1 << 16);
    // 0x8008FDDC: ori         $v1, $v1, 0x869F
    ctx->r3 = ctx->r3 | 0X869F;
    // 0x8008FDE0: sw          $v1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r3;
L_8008FDE4:
    // 0x8008FDE4: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8008FDE8: addu        $t6, $t6, $a0
    ctx->r14 = ADD32(ctx->r14, ctx->r4);
    // 0x8008FDEC: lw          $v1, 0x4($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X4);
    // 0x8008FDF0: lb          $t6, -0x54BD($t6)
    ctx->r14 = MEM_B(ctx->r14, -0X54BD);
    // 0x8008FDF4: subu        $v0, $v1, $t6
    ctx->r2 = SUB32(ctx->r3, ctx->r14);
    // 0x8008FDF8: sll         $v0, $v0, 24
    ctx->r2 = S32(ctx->r2 << 24);
    // 0x8008FDFC: sra         $v0, $v0, 24
    ctx->r2 = S32(SIGNED(ctx->r2) >> 24);
L_8008FE00:
    // 0x8008FE00: jr          $ra
    // 0x8008FE04: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
    return;
    // 0x8008FE04: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
;}
RECOMP_FUNC void func_8008FE08(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FE08: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8008FE0C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8008FE10: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x8008FE14: jal         0x8008F5E0
    // 0x8008FE18: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    func_8008F5E0(rdram, ctx);
        goto after_0;
    // 0x8008FE18: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x8008FE1C: beq         $v0, $zero, L_8008FED4
    if (ctx->r2 == 0) {
        // 0x8008FE20: addiu       $a0, $zero, 0x210
        ctx->r4 = ADD32(0, 0X210);
            goto L_8008FED4;
    }
    // 0x8008FE20: addiu       $a0, $zero, 0x210
    ctx->r4 = ADD32(0, 0X210);
    // 0x8008FE24: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x8008FE28: jal         0x80005A30
    // 0x8008FE2C: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_80005A30(rdram, ctx);
        goto after_1;
    // 0x8008FE2C: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x8008FE30: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8008FE34: lw          $t6, -0x5228($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5228);
    // 0x8008FE38: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x8008FE3C: addiu       $at, $zero, 0xFF
    ctx->r1 = ADD32(0, 0XFF);
    // 0x8008FE40: sw          $t6, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r14;
    // 0x8008FE44: lbu         $t7, 0x8($a2)
    ctx->r15 = MEM_BU(ctx->r6, 0X8);
    // 0x8008FE48: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8008FE4C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x8008FE50: bne         $t7, $at, L_8008FE6C
    if (ctx->r15 != ctx->r1) {
        // 0x8008FE54: addiu       $t5, $zero, -0x1
        ctx->r13 = ADD32(0, -0X1);
            goto L_8008FE6C;
    }
    // 0x8008FE54: addiu       $t5, $zero, -0x1
    ctx->r13 = ADD32(0, -0X1);
    // 0x8008FE58: lw          $t8, 0x0($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X0);
    // 0x8008FE5C: sh          $zero, 0x28($v0)
    MEM_H(0X28, ctx->r2) = 0;
    // 0x8008FE60: sh          $zero, 0x2A($v0)
    MEM_H(0X2A, ctx->r2) = 0;
    // 0x8008FE64: b           L_8008FE8C
    // 0x8008FE68: sw          $t8, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r24;
        goto L_8008FE8C;
    // 0x8008FE68: sw          $t8, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r24;
L_8008FE6C:
    // 0x8008FE6C: lw          $t9, 0x0($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X0);
    // 0x8008FE70: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x8008FE74: or          $t0, $t9, $at
    ctx->r8 = ctx->r25 | ctx->r1;
    // 0x8008FE78: sw          $t0, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r8;
    // 0x8008FE7C: lbu         $t1, 0x8($a2)
    ctx->r9 = MEM_BU(ctx->r6, 0X8);
    // 0x8008FE80: sh          $t1, 0x28($v0)
    MEM_H(0X28, ctx->r2) = ctx->r9;
    // 0x8008FE84: lbu         $t2, 0x9($a2)
    ctx->r10 = MEM_BU(ctx->r6, 0X9);
    // 0x8008FE88: sh          $t2, 0x2A($v0)
    MEM_H(0X2A, ctx->r2) = ctx->r10;
L_8008FE8C:
    // 0x8008FE8C: lhu         $t3, 0x2($v1)
    ctx->r11 = MEM_HU(ctx->r3, 0X2);
    // 0x8008FE90: sw          $t5, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r13;
    // 0x8008FE94: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8008FE98: ori         $t4, $t3, 0x800
    ctx->r12 = ctx->r11 | 0X800;
    // 0x8008FE9C: sh          $t4, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r12;
    // 0x8008FEA0: lbu         $t6, 0xA($a2)
    ctx->r14 = MEM_BU(ctx->r6, 0XA);
    // 0x8008FEA4: swc1        $f0, 0x50($v1)
    MEM_W(0X50, ctx->r3) = ctx->f0.u32l;
    // 0x8008FEA8: swc1        $f0, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f0.u32l;
    // 0x8008FEAC: swc1        $f0, 0x58($v1)
    MEM_W(0X58, ctx->r3) = ctx->f0.u32l;
    // 0x8008FEB0: sb          $t6, 0x1B($v1)
    MEM_B(0X1B, ctx->r3) = ctx->r14;
    // 0x8008FEB4: lwc1        $f4, 0x4($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8008FEB8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8008FEBC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x8008FEC0: swc1        $f4, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f4.u32l;
    // 0x8008FEC4: lwc1        $f6, 0x4($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X4);
    // 0x8008FEC8: swc1        $f8, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f8.u32l;
    // 0x8008FECC: b           L_8008FED8
    // 0x8008FED0: swc1        $f6, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f6.u32l;
        goto L_8008FED8;
    // 0x8008FED0: swc1        $f6, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f6.u32l;
L_8008FED4:
    // 0x8008FED4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8008FED8:
    // 0x8008FED8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8008FEDC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8008FEE0: jr          $ra
    // 0x8008FEE4: nop

    return;
    // 0x8008FEE4: nop

;}
RECOMP_FUNC void func_8008FEF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FEF0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8008FEF4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8008FEF8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8008FEFC: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8008FF00: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x8008FF04: jal         0x80002560
    // 0x8008FF08: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x8008FF08: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    after_0:
    // 0x8008FF0C: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
    // 0x8008FF10: lh          $a0, 0x22($sp)
    ctx->r4 = MEM_H(ctx->r29, 0X22);
    // 0x8008FF14: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x8008FF18: lbu         $t6, 0x0($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X0);
    // 0x8008FF1C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8008FF20: ori         $t7, $t6, 0x4
    ctx->r15 = ctx->r14 | 0X4;
    // 0x8008FF24: sb          $t7, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r15;
    // 0x8008FF28: sb          $t8, 0x6D($v0)
    MEM_B(0X6D, ctx->r2) = ctx->r24;
    // 0x8008FF2C: lbu         $t9, 0x1B($sp)
    ctx->r25 = MEM_BU(ctx->r29, 0X1B);
    // 0x8008FF30: sb          $t9, 0x15($v1)
    MEM_B(0X15, ctx->r3) = ctx->r25;
    // 0x8008FF34: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
    // 0x8008FF38: sh          $zero, 0x20($v1)
    MEM_H(0X20, ctx->r3) = 0;
    // 0x8008FF3C: sh          $a0, 0x1C($v1)
    MEM_H(0X1C, ctx->r3) = ctx->r4;
    // 0x8008FF40: sh          $a0, 0x1E($v1)
    MEM_H(0X1E, ctx->r3) = ctx->r4;
    // 0x8008FF44: swc1        $f4, 0x24($v1)
    MEM_W(0X24, ctx->r3) = ctx->f4.u32l;
    // 0x8008FF48: sw          $t0, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r8;
    // 0x8008FF4C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8008FF50: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8008FF54: jr          $ra
    // 0x8008FF58: nop

    return;
    // 0x8008FF58: nop

;}
RECOMP_FUNC void func_8008FF5C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FF5C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8008FF60: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8008FF64: jal         0x80002560
    // 0x8008FF68: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x8008FF68: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    after_0:
    // 0x8008FF6C: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
    // 0x8008FF70: sw          $zero, 0x18($v1)
    MEM_W(0X18, ctx->r3) = 0;
    // 0x8008FF74: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8008FF78: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8008FF7C: jr          $ra
    // 0x8008FF80: nop

    return;
    // 0x8008FF80: nop

;}
RECOMP_FUNC void func_8008FF84(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FF84: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8008FF88: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8008FF8C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8008FF90: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8008FF94: jal         0x80002560
    // 0x8008FF98: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x8008FF98: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    after_0:
    // 0x8008FF9C: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
    // 0x8008FFA0: lh          $t7, 0x1A($sp)
    ctx->r15 = MEM_H(ctx->r29, 0X1A);
    // 0x8008FFA4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8008FFA8: lh          $t6, 0x2($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X2);
    // 0x8008FFAC: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x8008FFB0: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x8008FFB4: sh          $t8, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r24;
    // 0x8008FFB8: lw          $t9, 0x28C4($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X28C4);
    // 0x8008FFBC: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
    // 0x8008FFC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8008FFC4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8008FFC8: or          $t1, $t9, $t0
    ctx->r9 = ctx->r25 | ctx->r8;
    // 0x8008FFCC: jr          $ra
    // 0x8008FFD0: sw          $t1, 0x28C4($a0)
    MEM_W(0X28C4, ctx->r4) = ctx->r9;
    return;
    // 0x8008FFD0: sw          $t1, 0x28C4($a0)
    MEM_W(0X28C4, ctx->r4) = ctx->r9;
;}
RECOMP_FUNC void func_8008FFD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8008FFD4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8008FFD8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8008FFDC: jal         0x80002560
    // 0x8008FFE0: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x8008FFE0: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    after_0:
    // 0x8008FFE4: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
    // 0x8008FFE8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8008FFEC: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x8008FFF0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8008FFF4: addiu       $t6, $zero, 0x2710
    ctx->r14 = ADD32(0, 0X2710);
    // 0x8008FFF8: sw          $zero, 0x28C4($a0)
    MEM_W(0X28C4, ctx->r4) = 0;
    // 0x8008FFFC: sh          $t6, 0x287A($a0)
    MEM_H(0X287A, ctx->r4) = ctx->r14;
    // 0x80090000: sh          $zero, 0x2($v1)
    MEM_H(0X2, ctx->r3) = 0;
    // 0x80090004: swc1        $f4, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->f4.u32l;
    // 0x80090008: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8009000C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80090010: jr          $ra
    // 0x80090014: nop

    return;
    // 0x80090014: nop

;}
RECOMP_FUNC void func_80090018(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090018: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8009001C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80090020: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80090024: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80090028: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x8009002C: jal         0x80002560
    // 0x80090030: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x80090030: addiu       $a0, $zero, 0x187
    ctx->r4 = ADD32(0, 0X187);
    after_0:
    // 0x80090034: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    // 0x80090038: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x8009003C: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x80090040: beql        $t6, $zero, L_80090050
    if (ctx->r14 == 0) {
        // 0x80090044: lw          $v1, 0x70($v0)
        ctx->r3 = MEM_W(ctx->r2, 0X70);
            goto L_80090050;
    }
    goto skip_0;
    // 0x80090044: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
    skip_0:
    // 0x80090048: sb          $t7, 0x78C4($at)
    MEM_B(0X78C4, ctx->r1) = ctx->r15;
    // 0x8009004C: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
L_80090050:
    // 0x80090050: lh          $t9, 0x1A($sp)
    ctx->r25 = MEM_H(ctx->r29, 0X1A);
    // 0x80090054: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80090058: lh          $t8, 0xA($v1)
    ctx->r24 = MEM_H(ctx->r3, 0XA);
    // 0x8009005C: addiu       $a0, $a0, -0x7D40
    ctx->r4 = ADD32(ctx->r4, -0X7D40);
    // 0x80090060: addu        $t0, $t8, $t9
    ctx->r8 = ADD32(ctx->r24, ctx->r25);
    // 0x80090064: sh          $t0, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r8;
    // 0x80090068: lw          $t2, 0x1C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X1C);
    // 0x8009006C: lw          $t1, 0x28C4($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X28C4);
    // 0x80090070: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80090074: nor         $t3, $t2, $zero
    ctx->r11 = ~(ctx->r10 | 0);
    // 0x80090078: and         $t4, $t1, $t3
    ctx->r12 = ctx->r9 & ctx->r11;
    // 0x8009007C: sw          $t4, 0x28C4($a0)
    MEM_W(0X28C4, ctx->r4) = ctx->r12;
    // 0x80090080: jr          $ra
    // 0x80090084: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80090084: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_80090090(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090090: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80090094: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80090098: jal         0x800404D0
    // 0x8009009C: nop

    func_800404D0(rdram, ctx);
        goto after_0;
    // 0x8009009C: nop

    after_0:
    // 0x800900A0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x800900A4: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x800900A8: lw          $t6, 0x2BD0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2BD0);
    // 0x800900AC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800900B0: addiu       $at, $zero, -0x2
    ctx->r1 = ADD32(0, -0X2);
    // 0x800900B4: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x800900B8: sw          $t7, 0x2BD0($v0)
    MEM_W(0X2BD0, ctx->r2) = ctx->r15;
    // 0x800900BC: jr          $ra
    // 0x800900C0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x800900C0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_800900C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800900C4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800900C8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800900CC: jal         0x800901E4
    // 0x800900D0: nop

    func_800901E4(rdram, ctx);
        goto after_0;
    // 0x800900D0: nop

    after_0:
    // 0x800900D4: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x800900D8: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x800900DC: sw          $zero, 0x2880($v1)
    MEM_W(0X2880, ctx->r3) = 0;
    // 0x800900E0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800900E4:
    // 0x800900E4: addu        $t6, $v1, $v0
    ctx->r14 = ADD32(ctx->r3, ctx->r2);
    // 0x800900E8: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800900EC: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x800900F0: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x800900F4: slti        $at, $v0, 0x31
    ctx->r1 = SIGNED(ctx->r2) < 0X31 ? 1 : 0;
    // 0x800900F8: bne         $at, $zero, L_800900E4
    if (ctx->r1 != 0) {
        // 0x800900FC: sb          $zero, 0x2883($t6)
        MEM_B(0X2883, ctx->r14) = 0;
            goto L_800900E4;
    }
    // 0x800900FC: sb          $zero, 0x2883($t6)
    MEM_B(0X2883, ctx->r14) = 0;
    // 0x80090100: sh          $zero, 0x285C($v1)
    MEM_H(0X285C, ctx->r3) = 0;
    // 0x80090104: sh          $zero, 0x285E($v1)
    MEM_H(0X285E, ctx->r3) = 0;
    // 0x80090108: sh          $zero, 0x2860($v1)
    MEM_H(0X2860, ctx->r3) = 0;
    // 0x8009010C: sh          $zero, 0x2862($v1)
    MEM_H(0X2862, ctx->r3) = 0;
    // 0x80090110: sh          $zero, 0x2864($v1)
    MEM_H(0X2864, ctx->r3) = 0;
    // 0x80090114: sh          $zero, 0x2866($v1)
    MEM_H(0X2866, ctx->r3) = 0;
    // 0x80090118: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8009011C:
    // 0x8009011C: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x80090120: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80090124: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80090128: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x8009012C: slti        $at, $v0, 0x2E
    ctx->r1 = SIGNED(ctx->r2) < 0X2E ? 1 : 0;
    // 0x80090130: addu        $t8, $v1, $t7
    ctx->r24 = ADD32(ctx->r3, ctx->r15);
    // 0x80090134: bne         $at, $zero, L_8009011C
    if (ctx->r1 != 0) {
        // 0x80090138: sw          $zero, 0x27A0($t8)
        MEM_W(0X27A0, ctx->r24) = 0;
            goto L_8009011C;
    }
    // 0x80090138: sw          $zero, 0x27A0($t8)
    MEM_W(0X27A0, ctx->r24) = 0;
    // 0x8009013C: lw          $t9, 0x2BD0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X2BD0);
    // 0x80090140: lh          $v0, 0x2874($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X2874);
    // 0x80090144: sw          $zero, 0x28D8($v1)
    MEM_W(0X28D8, ctx->r3) = 0;
    // 0x80090148: ori         $t0, $t9, 0x10
    ctx->r8 = ctx->r25 | 0X10;
    // 0x8009014C: sw          $zero, 0x2900($v1)
    MEM_W(0X2900, ctx->r3) = 0;
    // 0x80090150: beq         $v0, $zero, L_80090174
    if (ctx->r2 == 0) {
        // 0x80090154: sw          $t0, 0x2BD0($v1)
        MEM_W(0X2BD0, ctx->r3) = ctx->r8;
            goto L_80090174;
    }
    // 0x80090154: sw          $t0, 0x2BD0($v1)
    MEM_W(0X2BD0, ctx->r3) = ctx->r8;
    // 0x80090158: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8009015C: beq         $v0, $at, L_80090174
    if (ctx->r2 == ctx->r1) {
        // 0x80090160: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_80090174;
    }
    // 0x80090160: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80090164: beq         $v0, $at, L_8009018C
    if (ctx->r2 == ctx->r1) {
        // 0x80090168: addiu       $t4, $zero, 0x5A
        ctx->r12 = ADD32(0, 0X5A);
            goto L_8009018C;
    }
    // 0x80090168: addiu       $t4, $zero, 0x5A
    ctx->r12 = ADD32(0, 0X5A);
    // 0x8009016C: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x80090170: bne         $v0, $at, L_8009019C
    if (ctx->r2 != ctx->r1) {
        // 0x80090174: addiu       $t1, $zero, 0x61
        ctx->r9 = ADD32(0, 0X61);
            goto L_8009019C;
    }
L_80090174:
    // 0x80090174: addiu       $t1, $zero, 0x61
    ctx->r9 = ADD32(0, 0X61);
    // 0x80090178: addiu       $t3, $zero, 0x2C
    ctx->r11 = ADD32(0, 0X2C);
    // 0x8009017C: sw          $t1, 0x2BCC($v1)
    MEM_W(0X2BCC, ctx->r3) = ctx->r9;
    // 0x80090180: sw          $t1, 0x2BC8($v1)
    MEM_W(0X2BC8, ctx->r3) = ctx->r9;
    // 0x80090184: b           L_8009019C
    // 0x80090188: sh          $t3, 0x2BB8($v1)
    MEM_H(0X2BB8, ctx->r3) = ctx->r11;
        goto L_8009019C;
    // 0x80090188: sh          $t3, 0x2BB8($v1)
    MEM_H(0X2BB8, ctx->r3) = ctx->r11;
L_8009018C:
    // 0x8009018C: addiu       $t6, $zero, 0x9
    ctx->r14 = ADD32(0, 0X9);
    // 0x80090190: sw          $t4, 0x2BCC($v1)
    MEM_W(0X2BCC, ctx->r3) = ctx->r12;
    // 0x80090194: sw          $t4, 0x2BC8($v1)
    MEM_W(0X2BC8, ctx->r3) = ctx->r12;
    // 0x80090198: sh          $t6, 0x2BB8($v1)
    MEM_H(0X2BB8, ctx->r3) = ctx->r14;
L_8009019C:
    // 0x8009019C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800901A0: sh          $zero, 0x2BBA($v1)
    MEM_H(0X2BBA, ctx->r3) = 0;
    // 0x800901A4: sh          $zero, 0x2B4E($v1)
    MEM_H(0X2B4E, ctx->r3) = 0;
    // 0x800901A8: lh          $t7, 0x2BBA($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X2BBA);
    // 0x800901AC: lh          $t8, 0x2B4E($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X2B4E);
    // 0x800901B0: sw          $zero, 0x2BD4($v1)
    MEM_W(0X2BD4, ctx->r3) = 0;
    // 0x800901B4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800901B8: sh          $t7, 0x28D2($v1)
    MEM_H(0X28D2, ctx->r3) = ctx->r15;
    // 0x800901BC: jr          $ra
    // 0x800901C0: sh          $t8, 0x2B4C($v1)
    MEM_H(0X2B4C, ctx->r3) = ctx->r24;
    return;
    // 0x800901C0: sh          $t8, 0x2B4C($v1)
    MEM_H(0X2B4C, ctx->r3) = ctx->r24;
;}
RECOMP_FUNC void func_800901C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800901C4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800901C8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800901CC: jal         0x800901E4
    // 0x800901D0: nop

    func_800901E4(rdram, ctx);
        goto after_0;
    // 0x800901D0: nop

    after_0:
    // 0x800901D4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800901D8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800901DC: jr          $ra
    // 0x800901E0: nop

    return;
    // 0x800901E0: nop

;}
RECOMP_FUNC void func_800901E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800901E4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800901E8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800901EC: jal         0x800404D0
    // 0x800901F0: nop

    func_800404D0(rdram, ctx);
        goto after_0;
    // 0x800901F0: nop

    after_0:
    // 0x800901F4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x800901F8: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x800901FC: lw          $t8, 0x2BD0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2BD0);
    // 0x80090200: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80090204: addiu       $at, $zero, -0x2
    ctx->r1 = ADD32(0, -0X2);
    // 0x80090208: addiu       $t6, $zero, 0x2710
    ctx->r14 = ADD32(0, 0X2710);
    // 0x8009020C: addiu       $t7, $zero, 0x64
    ctx->r15 = ADD32(0, 0X64);
    // 0x80090210: and         $t9, $t8, $at
    ctx->r25 = ctx->r24 & ctx->r1;
    // 0x80090214: sh          $t6, 0x287A($v0)
    MEM_H(0X287A, ctx->r2) = ctx->r14;
    // 0x80090218: sh          $t7, 0x287C($v0)
    MEM_H(0X287C, ctx->r2) = ctx->r15;
    // 0x8009021C: sh          $zero, 0x287E($v0)
    MEM_H(0X287E, ctx->r2) = 0;
    // 0x80090220: sw          $zero, 0x28C4($v0)
    MEM_W(0X28C4, ctx->r2) = 0;
    // 0x80090224: sh          $zero, 0x2B62($v0)
    MEM_H(0X2B62, ctx->r2) = 0;
    // 0x80090228: sh          $zero, 0x2B64($v0)
    MEM_H(0X2B64, ctx->r2) = 0;
    // 0x8009022C: sh          $zero, 0x2B66($v0)
    MEM_H(0X2B66, ctx->r2) = 0;
    // 0x80090230: sh          $zero, 0x28C8($v0)
    MEM_H(0X28C8, ctx->r2) = 0;
    // 0x80090234: sh          $zero, 0x28CA($v0)
    MEM_H(0X28CA, ctx->r2) = 0;
    // 0x80090238: sw          $t9, 0x2BD0($v0)
    MEM_W(0X2BD0, ctx->r2) = ctx->r25;
    // 0x8009023C: sh          $zero, 0x2B38($v0)
    MEM_H(0X2B38, ctx->r2) = 0;
    // 0x80090240: jr          $ra
    // 0x80090244: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80090244: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_80090250(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090250: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80090254: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80090258: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8009025C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x80090260: jal         0x8000233C
    // 0x80090264: addiu       $a1, $zero, 0x2199
    ctx->r5 = ADD32(0, 0X2199);
    func_8000233C(rdram, ctx);
        goto after_0;
    // 0x80090264: addiu       $a1, $zero, 0x2199
    ctx->r5 = ADD32(0, 0X2199);
    after_0:
    // 0x80090268: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x8009026C: lw          $t6, 0x1C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X1C);
    // 0x80090270: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80090274: sw          $zero, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = 0;
    // 0x80090278: sw          $zero, 0x44($v0)
    MEM_W(0X44, ctx->r2) = 0;
    // 0x8009027C: sw          $zero, 0x34($v0)
    MEM_W(0X34, ctx->r2) = 0;
    // 0x80090280: beq         $a0, $zero, L_80090290
    if (ctx->r4 == 0) {
        // 0x80090284: sw          $t6, 0x38($v0)
        MEM_W(0X38, ctx->r2) = ctx->r14;
            goto L_80090290;
    }
    // 0x80090284: sw          $t6, 0x38($v0)
    MEM_W(0X38, ctx->r2) = ctx->r14;
    // 0x80090288: b           L_8009029C
    // 0x8009028C: sw          $a0, 0x4C($v0)
    MEM_W(0X4C, ctx->r2) = ctx->r4;
        goto L_8009029C;
    // 0x8009028C: sw          $a0, 0x4C($v0)
    MEM_W(0X4C, ctx->r2) = ctx->r4;
L_80090290:
    // 0x80090290: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80090294: lw          $t7, 0x27C0($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X27C0);
    // 0x80090298: sw          $t7, 0x4C($v0)
    MEM_W(0X4C, ctx->r2) = ctx->r15;
L_8009029C:
    // 0x8009029C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800902A0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800902A4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x800902A8: jr          $ra
    // 0x800902AC: nop

    return;
    // 0x800902AC: nop

;}
RECOMP_FUNC void func_800902B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800902B0: jr          $ra
    // 0x800902B4: lw          $v0, 0x44($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X44);
    return;
    // 0x800902B4: lw          $v0, 0x44($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X44);
;}
RECOMP_FUNC void func_800902B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800902B8: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x800902BC: or          $t7, $t6, $a1
    ctx->r15 = ctx->r14 | ctx->r5;
    // 0x800902C0: jr          $ra
    // 0x800902C4: sw          $t7, 0x34($a0)
    MEM_W(0X34, ctx->r4) = ctx->r15;
    return;
    // 0x800902C4: sw          $t7, 0x34($a0)
    MEM_W(0X34, ctx->r4) = ctx->r15;
;}
RECOMP_FUNC void func_800902D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800902D0: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x800902D4: sw          $s3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r19;
    // 0x800902D8: lui         $t6, 0x8009
    ctx->r14 = S32(0X8009 << 16);
    // 0x800902DC: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x800902E0: sw          $s5, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r21;
    // 0x800902E4: sw          $s4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r20;
    // 0x800902E8: sw          $s2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r18;
    // 0x800902EC: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x800902F0: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x800902F4: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x800902F8: addiu       $t6, $t6, 0xAA8
    ctx->r14 = ADD32(ctx->r14, 0XAA8);
    // 0x800902FC: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80090300: sw          $t6, 0x10($a0)
    MEM_W(0X10, ctx->r4) = ctx->r14;
    // 0x80090304: or          $s4, $a2, $zero
    ctx->r20 = ctx->r6 | 0;
    // 0x80090308: jal         0x800A2A30
    // 0x8009030C: lw          $a0, 0x0($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X0);
    osAiSetFrequency_recomp(rdram, ctx);
        goto after_0;
    // 0x8009030C: lw          $a0, 0x0($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X0);
    after_0:
    // 0x80090310: sw          $v0, 0x18($s3)
    MEM_W(0X18, ctx->r19) = ctx->r2;
    // 0x80090314: addiu       $s1, $zero, 0x10
    ctx->r17 = ADD32(0, 0X10);
    // 0x80090318: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
L_8009031C:
    // 0x8009031C: bnel        $s1, $zero, L_8009031C
    if (ctx->r17 != 0) {
        // 0x80090320: addiu       $s1, $s1, -0x1
        ctx->r17 = ADD32(ctx->r17, -0X1);
            goto L_8009031C;
    }
    goto skip_0;
    // 0x80090320: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
    skip_0:
    // 0x80090324: lw          $t7, 0x4($s4)
    ctx->r15 = MEM_W(ctx->r20, 0X4);
    // 0x80090328: lui         $v1, 0x8010
    ctx->r3 = S32(0X8010 << 16);
    // 0x8009032C: addiu       $v1, $v1, -0x1E24
    ctx->r3 = ADD32(ctx->r3, -0X1E24);
    // 0x80090330: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x80090334: lui         $s5, 0x8010
    ctx->r21 = S32(0X8010 << 16);
    // 0x80090338: bgez        $t7, L_80090350
    if (SIGNED(ctx->r15) >= 0) {
        // 0x8009033C: cvt.s.w     $f6, $f4
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
            goto L_80090350;
    }
    // 0x8009033C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80090340: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x80090344: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80090348: nop

    // 0x8009034C: add.s       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f8.fl;
L_80090350:
    // 0x80090350: lw          $t8, 0x18($s3)
    ctx->r24 = MEM_W(ctx->r19, 0X18);
    // 0x80090354: lui         $at, 0x4270
    ctx->r1 = S32(0X4270 << 16);
    // 0x80090358: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8009035C: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x80090360: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x80090364: addiu       $s5, $s5, -0x1E20
    ctx->r21 = ADD32(ctx->r21, -0X1E20);
    // 0x80090368: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x8009036C: addiu       $a0, $a0, -0x6270
    ctx->r4 = ADD32(ctx->r4, -0X6270);
    // 0x80090370: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x80090374: mul.s       $f18, $f6, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80090378: div.s       $f0, $f18, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = DIV_S(ctx->f18.fl, ctx->f4.fl);
    // 0x8009037C: trunc.w.s   $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    ctx->f8.u32l = TRUNC_W_S(ctx->f0.fl);
    // 0x80090380: mfc1        $v0, $f8
    ctx->r2 = (int32_t)ctx->f8.u32l;
    // 0x80090384: nop

    // 0x80090388: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x8009038C: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x80090390: bgez        $v0, L_800903A8
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80090394: cvt.s.w     $f6, $f10
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.fl = CVT_S_W(ctx->f10.u32l);
            goto L_800903A8;
    }
    // 0x80090394: cvt.s.w     $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80090398: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x8009039C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x800903A0: nop

    // 0x800903A4: add.s       $f6, $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f16.fl;
L_800903A8:
    // 0x800903A8: c.lt.s      $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f6.fl < ctx->f0.fl;
    // 0x800903AC: nop

    // 0x800903B0: bc1f        L_800903C0
    if (!c1cs) {
        // 0x800903B4: addiu       $t1, $v0, 0x1
        ctx->r9 = ADD32(ctx->r2, 0X1);
            goto L_800903C0;
    }
    // 0x800903B4: addiu       $t1, $v0, 0x1
    ctx->r9 = ADD32(ctx->r2, 0X1);
    // 0x800903B8: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
    // 0x800903BC: or          $v0, $t1, $zero
    ctx->r2 = ctx->r9 | 0;
L_800903C0:
    // 0x800903C0: andi        $t2, $v0, 0xF
    ctx->r10 = ctx->r2 & 0XF;
    // 0x800903C4: beq         $t2, $zero, L_800903D8
    if (ctx->r10 == 0) {
        // 0x800903C8: addiu       $at, $zero, -0x10
        ctx->r1 = ADD32(0, -0X10);
            goto L_800903D8;
    }
    // 0x800903C8: addiu       $at, $zero, -0x10
    ctx->r1 = ADD32(0, -0X10);
    // 0x800903CC: and         $t3, $v0, $at
    ctx->r11 = ctx->r2 & ctx->r1;
    // 0x800903D0: addiu       $v0, $t3, 0x10
    ctx->r2 = ADD32(ctx->r11, 0X10);
    // 0x800903D4: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
L_800903D8:
    // 0x800903D8: addiu       $t5, $v0, -0x10
    ctx->r13 = ADD32(ctx->r2, -0X10);
    // 0x800903DC: lui         $at, 0x8010
    ctx->r1 = S32(0X8010 << 16);
    // 0x800903E0: sw          $t5, -0x1E28($at)
    MEM_W(-0X1E28, ctx->r1) = ctx->r13;
    // 0x800903E4: addiu       $t6, $v0, 0x100
    ctx->r14 = ADD32(ctx->r2, 0X100);
    // 0x800903E8: jal         0x80096F4C
    // 0x800903EC: sw          $t6, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r14;
    func_80096F4C(rdram, ctx);
        goto after_1;
    // 0x800903EC: sw          $t6, 0x0($s5)
    MEM_W(0X0, ctx->r21) = ctx->r14;
    after_1:
    // 0x800903F0: lui         $v0, 0x8010
    ctx->r2 = S32(0X8010 << 16);
    // 0x800903F4: addiu       $v0, $v0, -0x2210
    ctx->r2 = ADD32(ctx->r2, -0X2210);
    // 0x800903F8: lui         $s2, 0x8010
    ctx->r18 = S32(0X8010 << 16);
    // 0x800903FC: lui         $s0, 0x8010
    ctx->r16 = S32(0X8010 << 16);
    // 0x80090400: sw          $zero, 0x4($v0)
    MEM_W(0X4, ctx->r2) = 0;
    // 0x80090404: sw          $zero, 0x0($v0)
    MEM_W(0X0, ctx->r2) = 0;
    // 0x80090408: addiu       $s0, $s0, -0x21FC
    ctx->r16 = ADD32(ctx->r16, -0X21FC);
    // 0x8009040C: addiu       $s2, $s2, -0x2210
    ctx->r18 = ADD32(ctx->r18, -0X2210);
    // 0x80090410: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80090414:
    // 0x80090414: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80090418: jal         0x80096EF0
    // 0x8009041C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    __osEnqueueThread_recomp(rdram, ctx);
        goto after_2;
    // 0x8009041C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_2:
    // 0x80090420: lw          $a2, 0x14($s3)
    ctx->r6 = MEM_W(ctx->r19, 0X14);
    // 0x80090424: addiu       $t7, $zero, 0xA00
    ctx->r15 = ADD32(0, 0XA00);
    // 0x80090428: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    // 0x8009042C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80090430: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80090434: jal         0x800A6A10
    // 0x80090438: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    func_800A6A10(rdram, ctx);
        goto after_3;
    // 0x80090438: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    after_3:
    // 0x8009043C: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80090440: sltiu       $at, $s1, 0x31
    ctx->r1 = ctx->r17 < 0X31 ? 1 : 0;
    // 0x80090444: addiu       $s2, $s2, 0x14
    ctx->r18 = ADD32(ctx->r18, 0X14);
    // 0x80090448: addiu       $s0, $s0, 0x14
    ctx->r16 = ADD32(ctx->r16, 0X14);
    // 0x8009044C: bne         $at, $zero, L_80090414
    if (ctx->r1 != 0) {
        // 0x80090450: sw          $v0, -0x4($s2)
        MEM_W(-0X4, ctx->r18) = ctx->r2;
            goto L_80090414;
    }
    // 0x80090450: sw          $v0, -0x4($s2)
    MEM_W(-0X4, ctx->r18) = ctx->r2;
    // 0x80090454: lw          $a2, 0x14($s3)
    ctx->r6 = MEM_W(ctx->r19, 0X14);
    // 0x80090458: addiu       $t8, $zero, 0xA00
    ctx->r24 = ADD32(0, 0XA00);
    // 0x8009045C: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    // 0x80090460: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80090464: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80090468: jal         0x800A6A10
    // 0x8009046C: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    func_800A6A10(rdram, ctx);
        goto after_4;
    // 0x8009046C: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    after_4:
    // 0x80090470: lui         $s0, 0x8010
    ctx->r16 = S32(0X8010 << 16);
    // 0x80090474: lui         $s1, 0x8010
    ctx->r17 = S32(0X8010 << 16);
    // 0x80090478: sw          $v0, 0x10($s2)
    MEM_W(0X10, ctx->r18) = ctx->r2;
    // 0x8009047C: addiu       $s1, $s1, -0x6660
    ctx->r17 = ADD32(ctx->r17, -0X6660);
    // 0x80090480: addiu       $s0, $s0, -0x6668
    ctx->r16 = ADD32(ctx->r16, -0X6668);
L_80090484:
    // 0x80090484: lw          $t9, 0x8($s4)
    ctx->r25 = MEM_W(ctx->r20, 0X8);
    // 0x80090488: lw          $a2, 0x14($s3)
    ctx->r6 = MEM_W(ctx->r19, 0X14);
    // 0x8009048C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80090490: sll         $t0, $t9, 3
    ctx->r8 = S32(ctx->r25 << 3);
    // 0x80090494: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    // 0x80090498: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8009049C: jal         0x800A6A10
    // 0x800904A0: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    func_800A6A10(rdram, ctx);
        goto after_5;
    // 0x800904A0: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    after_5:
    // 0x800904A4: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x800904A8: sltu        $at, $s0, $s1
    ctx->r1 = ctx->r16 < ctx->r17 ? 1 : 0;
    // 0x800904AC: bne         $at, $zero, L_80090484
    if (ctx->r1 != 0) {
        // 0x800904B0: sw          $v0, -0x4($s0)
        MEM_W(-0X4, ctx->r16) = ctx->r2;
            goto L_80090484;
    }
    // 0x800904B0: sw          $v0, -0x4($s0)
    MEM_W(-0X4, ctx->r16) = ctx->r2;
    // 0x800904B4: lw          $t1, 0x8($s4)
    ctx->r9 = MEM_W(ctx->r20, 0X8);
    // 0x800904B8: lui         $at, 0x8010
    ctx->r1 = S32(0X8010 << 16);
    // 0x800904BC: lui         $s0, 0x8010
    ctx->r16 = S32(0X8010 << 16);
    // 0x800904C0: lui         $s2, 0x8010
    ctx->r18 = S32(0X8010 << 16);
    // 0x800904C4: addiu       $s2, $s2, -0x665C
    ctx->r18 = ADD32(ctx->r18, -0X665C);
    // 0x800904C8: addiu       $s0, $s0, -0x6668
    ctx->r16 = ADD32(ctx->r16, -0X6668);
    // 0x800904CC: addiu       $s1, $zero, 0x2
    ctx->r17 = ADD32(0, 0X2);
    // 0x800904D0: sw          $t1, -0x1E1C($at)
    MEM_W(-0X1E1C, ctx->r1) = ctx->r9;
L_800904D4:
    // 0x800904D4: lw          $a2, 0x14($s3)
    ctx->r6 = MEM_W(ctx->r19, 0X14);
    // 0x800904D8: addiu       $t2, $zero, 0x68
    ctx->r10 = ADD32(0, 0X68);
    // 0x800904DC: sw          $t2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r10;
    // 0x800904E0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x800904E4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x800904E8: jal         0x800A6A10
    // 0x800904EC: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    func_800A6A10(rdram, ctx);
        goto after_6;
    // 0x800904EC: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    after_6:
    // 0x800904F0: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;
    // 0x800904F4: sh          $s1, 0x60($v0)
    MEM_H(0X60, ctx->r2) = ctx->r17;
    // 0x800904F8: lw          $v1, 0x8($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X8);
    // 0x800904FC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80090500: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80090504: sw          $v1, 0x64($v1)
    MEM_W(0X64, ctx->r3) = ctx->r3;
    // 0x80090508: lw          $t3, 0x0($s5)
    ctx->r11 = MEM_W(ctx->r21, 0X0);
    // 0x8009050C: lw          $a2, 0x14($s3)
    ctx->r6 = MEM_W(ctx->r19, 0X14);
    // 0x80090510: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80090514: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80090518: jal         0x800A6A10
    // 0x8009051C: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    func_800A6A10(rdram, ctx);
        goto after_7;
    // 0x8009051C: sw          $t4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r12;
    after_7:
    // 0x80090520: lw          $t5, 0x8($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X8);
    // 0x80090524: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80090528: bne         $s0, $s2, L_800904D4
    if (ctx->r16 != ctx->r18) {
        // 0x8009052C: sw          $v0, 0x0($t5)
        MEM_W(0X0, ctx->r13) = ctx->r2;
            goto L_800904D4;
    }
    // 0x8009052C: sw          $v0, 0x0($t5)
    MEM_W(0X0, ctx->r13) = ctx->r2;
    // 0x80090530: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x80090534: lui         $a1, 0x8010
    ctx->r5 = S32(0X8010 << 16);
    // 0x80090538: addiu       $a1, $a1, -0x6370
    ctx->r5 = ADD32(ctx->r5, -0X6370);
    // 0x8009053C: addiu       $a0, $a0, -0x6388
    ctx->r4 = ADD32(ctx->r4, -0X6388);
    // 0x80090540: jal         0x800A1460
    // 0x80090544: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_8;
    // 0x80090544: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_8:
    // 0x80090548: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x8009054C: lui         $a1, 0x8010
    ctx->r5 = S32(0X8010 << 16);
    // 0x80090550: addiu       $a1, $a1, -0x6488
    ctx->r5 = ADD32(ctx->r5, -0X6488);
    // 0x80090554: addiu       $a0, $a0, -0x64A0
    ctx->r4 = ADD32(ctx->r4, -0X64A0);
    // 0x80090558: jal         0x800A1460
    // 0x8009055C: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_9;
    // 0x8009055C: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_9:
    // 0x80090560: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x80090564: lui         $a1, 0x8010
    ctx->r5 = S32(0X8010 << 16);
    // 0x80090568: addiu       $a1, $a1, -0x1800
    ctx->r5 = ADD32(ctx->r5, -0X1800);
    // 0x8009056C: addiu       $a0, $a0, -0x1818
    ctx->r4 = ADD32(ctx->r4, -0X1818);
    // 0x80090570: jal         0x800A1460
    // 0x80090574: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_10;
    // 0x80090574: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_10:
    // 0x80090578: lw          $t7, 0x3C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X3C);
    // 0x8009057C: lui         $s0, 0x8010
    ctx->r16 = S32(0X8010 << 16);
    // 0x80090580: lui         $t6, 0x8010
    ctx->r14 = S32(0X8010 << 16);
    // 0x80090584: addiu       $s0, $s0, -0x6650
    ctx->r16 = ADD32(ctx->r16, -0X6650);
    // 0x80090588: addiu       $t6, $t6, -0x2220
    ctx->r14 = ADD32(ctx->r14, -0X2220);
    // 0x8009058C: lui         $a2, 0x8009
    ctx->r6 = S32(0X8009 << 16);
    // 0x80090590: addiu       $a2, $a2, 0x5D8
    ctx->r6 = ADD32(ctx->r6, 0X5D8);
    // 0x80090594: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x80090598: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8009059C: addiu       $a1, $zero, 0x3
    ctx->r5 = ADD32(0, 0X3);
    // 0x800905A0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x800905A4: jal         0x80098A00
    // 0x800905A8: sw          $t7, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r15;
    osCreateThread_recomp(rdram, ctx);
        goto after_11;
    // 0x800905A8: sw          $t7, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r15;
    after_11:
    // 0x800905AC: jal         0x800A6E30
    // 0x800905B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    osStartThread_recomp(rdram, ctx);
        goto after_12;
    // 0x800905B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_12:
    // 0x800905B4: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x800905B8: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x800905BC: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x800905C0: lw          $s2, 0x24($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X24);
    // 0x800905C4: lw          $s3, 0x28($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X28);
    // 0x800905C8: lw          $s4, 0x2C($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X2C);
    // 0x800905CC: lw          $s5, 0x30($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X30);
    // 0x800905D0: jr          $ra
    // 0x800905D4: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x800905D4: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_800905D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800905D8: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x800905DC: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x800905E0: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x800905E4: addiu       $s1, $s1, 0x5E80
    ctx->r17 = ADD32(ctx->r17, 0X5E80);
    // 0x800905E8: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x800905EC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x800905F0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800905F4: sw          $a0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r4;
    // 0x800905F8: lui         $a2, 0x8010
    ctx->r6 = S32(0X8010 << 16);
    // 0x800905FC: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x80090600: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80090604: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80090608: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x8009060C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80090610: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80090614: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80090618: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x8009061C: addiu       $a2, $a2, -0x64A0
    ctx->r6 = ADD32(ctx->r6, -0X64A0);
    // 0x80090620: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80090624: jal         0x80018890
    // 0x80090628: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    func_80018890(rdram, ctx);
        goto after_0;
    // 0x80090628: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    after_0:
    // 0x8009062C: jal         0x8001875C
    // 0x80090630: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_8001875C(rdram, ctx);
        goto after_1;
    // 0x80090630: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_1:
    // 0x80090634: lui         $at, 0x8010
    ctx->r1 = S32(0X8010 << 16);
    // 0x80090638: lui         $s7, 0x800C
    ctx->r23 = S32(0X800C << 16);
    // 0x8009063C: lui         $s6, 0x8010
    ctx->r22 = S32(0X8010 << 16);
    // 0x80090640: sw          $v0, -0x6670($at)
    MEM_W(-0X6670, ctx->r1) = ctx->r2;
    // 0x80090644: addiu       $s6, $s6, -0x6668
    ctx->r22 = ADD32(ctx->r22, -0X6668);
    // 0x80090648: addiu       $s7, $s7, -0x7920
    ctx->r23 = ADD32(ctx->r23, -0X7920);
    // 0x8009064C: addiu       $fp, $zero, 0x3
    ctx->r30 = ADD32(0, 0X3);
    // 0x80090650: addiu       $s5, $zero, 0x20
    ctx->r21 = ADD32(0, 0X20);
    // 0x80090654: addiu       $s4, $zero, 0x3
    ctx->r20 = ADD32(0, 0X3);
    // 0x80090658: addiu       $s3, $zero, 0x1
    ctx->r19 = ADD32(0, 0X1);
    // 0x8009065C: addiu       $s1, $sp, 0x54
    ctx->r17 = ADD32(ctx->r29, 0X54);
L_80090660:
    // 0x80090660: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x80090664: addiu       $a0, $a0, -0x64A0
    ctx->r4 = ADD32(ctx->r4, -0X64A0);
    // 0x80090668: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x8009066C: jal         0x80096CE0
    // 0x80090670: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    osRecvMesg_recomp(rdram, ctx);
        goto after_2;
    // 0x80090670: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    after_2:
    // 0x80090674: lw          $t6, 0x54($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X54);
    // 0x80090678: lh          $v1, 0x0($t6)
    ctx->r3 = MEM_H(ctx->r14, 0X0);
    // 0x8009067C: beql        $v1, $s3, L_800906A0
    if (ctx->r3 == ctx->r19) {
        // 0x80090680: lw          $t7, 0x0($s7)
        ctx->r15 = MEM_W(ctx->r23, 0X0);
            goto L_800906A0;
    }
    goto skip_0;
    // 0x80090680: lw          $t7, 0x0($s7)
    ctx->r15 = MEM_W(ctx->r23, 0X0);
    skip_0:
    // 0x80090684: beq         $v1, $s4, L_80090700
    if (ctx->r3 == ctx->r20) {
        // 0x80090688: nop
    
            goto L_80090700;
    }
    // 0x80090688: nop

    // 0x8009068C: beql        $v1, $s5, L_80090700
    if (ctx->r3 == ctx->r21) {
        // 0x80090690: addiu       $s2, $zero, 0x1
        ctx->r18 = ADD32(0, 0X1);
            goto L_80090700;
    }
    goto skip_1;
    // 0x80090690: addiu       $s2, $zero, 0x1
    ctx->r18 = ADD32(0, 0X1);
    skip_1:
    // 0x80090694: b           L_80090700
    // 0x80090698: nop

        goto L_80090700;
    // 0x80090698: nop

    // 0x8009069C: lw          $t7, 0x0($s7)
    ctx->r15 = MEM_W(ctx->r23, 0X0);
L_800906A0:
    // 0x800906A0: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x800906A4: divu        $zero, $t7, $fp
    lo = S32(U32(ctx->r15) / U32(ctx->r30)); hi = S32(U32(ctx->r15) % U32(ctx->r30));
    // 0x800906A8: mfhi        $t8
    ctx->r24 = hi;
    // 0x800906AC: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x800906B0: addu        $t0, $s6, $t9
    ctx->r8 = ADD32(ctx->r22, ctx->r25);
    // 0x800906B4: bne         $fp, $zero, L_800906C0
    if (ctx->r30 != 0) {
        // 0x800906B8: nop
    
            goto L_800906C0;
    }
    // 0x800906B8: nop

    // 0x800906BC: break       7
    do_break(2148075196);
L_800906C0:
    // 0x800906C0: lw          $a0, 0x8($t0)
    ctx->r4 = MEM_W(ctx->r8, 0X8);
    // 0x800906C4: jal         0x80090744
    // 0x800906C8: nop

    func_80090744(rdram, ctx);
        goto after_3;
    // 0x800906C8: nop

    after_3:
    // 0x800906CC: beq         $v0, $zero, L_80090700
    if (ctx->r2 == 0) {
        // 0x800906D0: lui         $a0, 0x8010
        ctx->r4 = S32(0X8010 << 16);
            goto L_80090700;
    }
    // 0x800906D0: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x800906D4: addiu       $a0, $a0, -0x6388
    ctx->r4 = ADD32(ctx->r4, -0X6388);
    // 0x800906D8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x800906DC: jal         0x80096CE0
    // 0x800906E0: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    osRecvMesg_recomp(rdram, ctx);
        goto after_4;
    // 0x800906E0: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    after_4:
    // 0x800906E4: lw          $t1, 0x54($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X54);
    // 0x800906E8: jal         0x800908EC
    // 0x800906EC: lw          $a0, 0x4($t1)
    ctx->r4 = MEM_W(ctx->r9, 0X4);
    func_800908EC(rdram, ctx);
        goto after_5;
    // 0x800906EC: lw          $a0, 0x4($t1)
    ctx->r4 = MEM_W(ctx->r9, 0X4);
    after_5:
    // 0x800906F0: lw          $t2, 0x54($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X54);
    // 0x800906F4: b           L_80090700
    // 0x800906F8: lw          $s0, 0x4($t2)
    ctx->r16 = MEM_W(ctx->r10, 0X4);
        goto L_80090700;
    // 0x800906F8: lw          $s0, 0x4($t2)
    ctx->r16 = MEM_W(ctx->r10, 0X4);
    // 0x800906FC: addiu       $s2, $zero, 0x1
    ctx->r18 = ADD32(0, 0X1);
L_80090700:
    // 0x80090700: beq         $s2, $zero, L_80090660
    if (ctx->r18 == 0) {
        // 0x80090704: nop
    
            goto L_80090660;
    }
    // 0x80090704: nop

    // 0x80090708: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x8009070C: jal         0x80096F14
    // 0x80090710: addiu       $a0, $a0, -0x6270
    ctx->r4 = ADD32(ctx->r4, -0X6270);
    func_80096F14(rdram, ctx);
        goto after_6;
    // 0x80090710: addiu       $a0, $a0, -0x6270
    ctx->r4 = ADD32(ctx->r4, -0X6270);
    after_6:
    // 0x80090714: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80090718: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8009071C: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80090720: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80090724: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80090728: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8009072C: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80090730: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80090734: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80090738: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x8009073C: jr          $ra
    // 0x80090740: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    return;
    // 0x80090740: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
;}
RECOMP_FUNC void func_80090744(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090744: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80090748: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8009074C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80090750: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80090754: jal         0x80090AE0
    // 0x80090758: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    func_80090AE0(rdram, ctx);
        goto after_0;
    // 0x80090758: sw          $a1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r5;
    after_0:
    // 0x8009075C: jal         0x800993C0
    // 0x80090760: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    osVirtualToPhysical_recomp(rdram, ctx);
        goto after_1;
    // 0x80090760: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    after_1:
    // 0x80090764: lw          $v1, 0x44($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X44);
    // 0x80090768: sw          $v0, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r2;
    // 0x8009076C: beq         $v1, $zero, L_80090784
    if (ctx->r3 == 0) {
        // 0x80090770: nop
    
            goto L_80090784;
    }
    // 0x80090770: nop

    // 0x80090774: lh          $a1, 0x4($v1)
    ctx->r5 = MEM_H(ctx->r3, 0X4);
    // 0x80090778: lw          $a0, 0x0($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X0);
    // 0x8009077C: jal         0x800A7240
    // 0x80090780: sll         $a1, $a1, 2
    ctx->r5 = S32(ctx->r5 << 2);
    osAiSetNextBuffer_recomp(rdram, ctx);
        goto after_2;
    // 0x80090780: sll         $a1, $a1, 2
    ctx->r5 = S32(ctx->r5 << 2);
    after_2:
L_80090784:
    // 0x80090784: jal         0x800A4CF0
    // 0x80090788: nop

    osAiGetLength_recomp(rdram, ctx);
        goto after_3;
    // 0x80090788: nop

    after_3:
    // 0x8009078C: lui         $t6, 0x8010
    ctx->r14 = S32(0X8010 << 16);
    // 0x80090790: lw          $t6, -0x1E24($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X1E24);
    // 0x80090794: srl         $t7, $v0, 2
    ctx->r15 = S32(U32(ctx->r2) >> 2);
    // 0x80090798: lui         $v1, 0x8010
    ctx->r3 = S32(0X8010 << 16);
    // 0x8009079C: subu        $t8, $t6, $t7
    ctx->r24 = SUB32(ctx->r14, ctx->r15);
    // 0x800907A0: addiu       $t9, $t8, 0x100
    ctx->r25 = ADD32(ctx->r24, 0X100);
    // 0x800907A4: andi        $t1, $t9, 0xFFF0
    ctx->r9 = ctx->r25 & 0XFFF0;
    // 0x800907A8: sh          $t1, 0x4($s0)
    MEM_H(0X4, ctx->r16) = ctx->r9;
    // 0x800907AC: lw          $v1, -0x1E28($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X1E28);
    // 0x800907B0: lh          $a3, 0x4($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X4);
    // 0x800907B4: lui         $t2, 0x800C
    ctx->r10 = S32(0X800C << 16);
    // 0x800907B8: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x800907BC: sltu        $at, $a3, $v1
    ctx->r1 = ctx->r7 < ctx->r3 ? 1 : 0;
    // 0x800907C0: beq         $at, $zero, L_800907D0
    if (ctx->r1 == 0) {
        // 0x800907C4: nop
    
            goto L_800907D0;
    }
    // 0x800907C4: nop

    // 0x800907C8: sh          $v1, 0x4($s0)
    MEM_H(0X4, ctx->r16) = ctx->r3;
    // 0x800907CC: lh          $a3, 0x4($s0)
    ctx->r7 = MEM_H(ctx->r16, 0X4);
L_800907D0:
    // 0x800907D0: lw          $t2, -0x7918($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X7918);
    // 0x800907D4: addiu       $a1, $sp, 0x34
    ctx->r5 = ADD32(ctx->r29, 0X34);
    // 0x800907D8: lw          $a2, 0x3C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X3C);
    // 0x800907DC: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x800907E0: addu        $a0, $a0, $t3
    ctx->r4 = ADD32(ctx->r4, ctx->r11);
    // 0x800907E4: jal         0x8009CA90
    // 0x800907E8: lw          $a0, -0x6668($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X6668);
    func_8009CA90(rdram, ctx);
        goto after_4;
    // 0x800907E8: lw          $a0, -0x6668($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X6668);
    after_4:
    // 0x800907EC: lw          $t4, 0x34($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X34);
    // 0x800907F0: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x800907F4: lui         $t0, 0x8010
    ctx->r8 = S32(0X8010 << 16);
    // 0x800907F8: addiu       $t0, $t0, -0x6668
    ctx->r8 = ADD32(ctx->r8, -0X6668);
    // 0x800907FC: addiu       $v1, $v1, -0x7918
    ctx->r3 = ADD32(ctx->r3, -0X7918);
    // 0x80090800: bne         $t4, $zero, L_80090810
    if (ctx->r12 != 0) {
        // 0x80090804: or          $a3, $v0, $zero
        ctx->r7 = ctx->r2 | 0;
            goto L_80090810;
    }
    // 0x80090804: or          $a3, $v0, $zero
    ctx->r7 = ctx->r2 | 0;
    // 0x80090808: b           L_800908D8
    // 0x8009080C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_800908D8;
    // 0x8009080C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80090810:
    // 0x80090810: lui         $t5, 0x8010
    ctx->r13 = S32(0X8010 << 16);
    // 0x80090814: addiu       $t5, $t5, -0x6388
    ctx->r13 = ADD32(ctx->r13, -0X6388);
    // 0x80090818: addiu       $t6, $s0, 0x60
    ctx->r14 = ADD32(ctx->r16, 0X60);
    // 0x8009081C: sw          $zero, 0x8($s0)
    MEM_W(0X8, ctx->r16) = 0;
    // 0x80090820: sw          $t5, 0x58($s0)
    MEM_W(0X58, ctx->r16) = ctx->r13;
    // 0x80090824: sw          $t6, 0x5C($s0)
    MEM_W(0X5C, ctx->r16) = ctx->r14;
    // 0x80090828: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x8009082C: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x80090830: addiu       $v0, $v0, -0x5260
    ctx->r2 = ADD32(ctx->r2, -0X5260);
    // 0x80090834: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x80090838: addu        $t9, $t0, $t8
    ctx->r25 = ADD32(ctx->r8, ctx->r24);
    // 0x8009083C: lw          $t1, 0x0($t9)
    ctx->r9 = MEM_W(ctx->r25, 0X0);
    // 0x80090840: addiu       $t9, $zero, 0x2
    ctx->r25 = ADD32(0, 0X2);
    // 0x80090844: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x80090848: sw          $t1, 0x48($s0)
    MEM_W(0X48, ctx->r16) = ctx->r9;
    // 0x8009084C: lw          $t2, 0x0($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X0);
    // 0x80090850: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80090854: addiu       $t1, $t1, -0x5190
    ctx->r9 = ADD32(ctx->r9, -0X5190);
    // 0x80090858: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x8009085C: addu        $t4, $t0, $t3
    ctx->r12 = ADD32(ctx->r8, ctx->r11);
    // 0x80090860: lw          $t5, 0x0($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X0);
    // 0x80090864: lui         $t3, 0x800B
    ctx->r11 = S32(0X800B << 16);
    // 0x80090868: lui         $t4, 0x800C
    ctx->r12 = S32(0X800C << 16);
    // 0x8009086C: subu        $t6, $a3, $t5
    ctx->r14 = SUB32(ctx->r7, ctx->r13);
    // 0x80090870: sra         $t7, $t6, 3
    ctx->r15 = S32(SIGNED(ctx->r14) >> 3);
    // 0x80090874: sll         $t8, $t7, 3
    ctx->r24 = S32(ctx->r15 << 3);
    // 0x80090878: subu        $t2, $t1, $v0
    ctx->r10 = SUB32(ctx->r9, ctx->r2);
    // 0x8009087C: addiu       $t3, $t3, -0x3E00
    ctx->r11 = ADD32(ctx->r11, -0X3E00);
    // 0x80090880: addiu       $t4, $t4, 0x1260
    ctx->r12 = ADD32(ctx->r12, 0X1260);
    // 0x80090884: addiu       $t5, $zero, 0x800
    ctx->r13 = ADD32(0, 0X800);
    // 0x80090888: sw          $t8, 0x4C($s0)
    MEM_W(0X4C, ctx->r16) = ctx->r24;
    // 0x8009088C: sw          $t9, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r25;
    // 0x80090890: sw          $v0, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->r2;
    // 0x80090894: sw          $t2, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r10;
    // 0x80090898: sw          $zero, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = 0;
    // 0x8009089C: sw          $t3, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->r11;
    // 0x800908A0: sw          $t4, 0x30($s0)
    MEM_W(0X30, ctx->r16) = ctx->r12;
    // 0x800908A4: sw          $t5, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->r13;
    // 0x800908A8: sw          $zero, 0x50($s0)
    MEM_W(0X50, ctx->r16) = 0;
    // 0x800908AC: sw          $zero, 0x54($s0)
    MEM_W(0X54, ctx->r16) = 0;
    // 0x800908B0: lw          $a0, -0x6670($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X6670);
    // 0x800908B4: addiu       $a1, $s0, 0x8
    ctx->r5 = ADD32(ctx->r16, 0X8);
    // 0x800908B8: jal         0x80096760
    // 0x800908BC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osJamMesg_recomp(rdram, ctx);
        goto after_5;
    // 0x800908BC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_5:
    // 0x800908C0: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x800908C4: addiu       $v1, $v1, -0x7918
    ctx->r3 = ADD32(ctx->r3, -0X7918);
    // 0x800908C8: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x800908CC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800908D0: xori        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 ^ 0X1;
    // 0x800908D4: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
L_800908D8:
    // 0x800908D8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800908DC: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800908E0: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x800908E4: jr          $ra
    // 0x800908E8: nop

    return;
    // 0x800908E8: nop

;}
RECOMP_FUNC void func_800908EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800908EC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800908F0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800908F4: jal         0x800A4CF0
    // 0x800908F8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    osAiGetLength_recomp(rdram, ctx);
        goto after_0;
    // 0x800908F8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x800908FC: srl         $t6, $v0, 2
    ctx->r14 = S32(U32(ctx->r2) >> 2);
    // 0x80090900: bne         $t6, $zero, L_8009091C
    if (ctx->r14 != 0) {
        // 0x80090904: lui         $t7, 0x800C
        ctx->r15 = S32(0X800C << 16);
            goto L_8009091C;
    }
    // 0x80090904: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x80090908: lw          $t7, -0x7914($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X7914);
    // 0x8009090C: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80090910: bnel        $t7, $zero, L_80090920
    if (ctx->r15 != 0) {
        // 0x80090914: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80090920;
    }
    goto skip_0;
    // 0x80090914: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80090918: sw          $zero, -0x7914($at)
    MEM_W(-0X7914, ctx->r1) = 0;
L_8009091C:
    // 0x8009091C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80090920:
    // 0x80090920: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80090924: jr          $ra
    // 0x80090928: nop

    return;
    // 0x80090928: nop

;}
RECOMP_FUNC void func_8009092C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8009092C: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80090930: sw          $s1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r17;
    // 0x80090934: lui         $s1, 0x8010
    ctx->r17 = S32(0X8010 << 16);
    // 0x80090938: addiu       $s1, $s1, -0x2220
    ctx->r17 = ADD32(ctx->r17, -0X2220);
    // 0x8009093C: sw          $s0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r16;
    // 0x80090940: lw          $s0, 0x4($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X4);
    // 0x80090944: sw          $a2, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r6;
    // 0x80090948: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8009094C: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80090950: beq         $s0, $zero, L_800909A8
    if (ctx->r16 == 0) {
        // 0x80090954: or          $a2, $zero, $zero
        ctx->r6 = 0 | 0;
            goto L_800909A8;
    }
    // 0x80090954: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80090958: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
L_8009095C:
    // 0x8009095C: addu        $t6, $a3, $a1
    ctx->r14 = ADD32(ctx->r7, ctx->r5);
    // 0x80090960: sltu        $at, $a3, $v0
    ctx->r1 = ctx->r7 < ctx->r2 ? 1 : 0;
    // 0x80090964: bne         $at, $zero, L_800909A8
    if (ctx->r1 != 0) {
        // 0x80090968: addiu       $v1, $v0, 0xA00
        ctx->r3 = ADD32(ctx->r2, 0XA00);
            goto L_800909A8;
    }
    // 0x80090968: addiu       $v1, $v0, 0xA00
    ctx->r3 = ADD32(ctx->r2, 0XA00);
    // 0x8009096C: slt         $at, $v1, $t6
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r14) ? 1 : 0;
    // 0x80090970: bne         $at, $zero, L_8009099C
    if (ctx->r1 != 0) {
        // 0x80090974: or          $a2, $s0, $zero
        ctx->r6 = ctx->r16 | 0;
            goto L_8009099C;
    }
    // 0x80090974: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x80090978: lw          $t8, 0x10($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X10);
    // 0x8009097C: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x80090980: lw          $t7, -0x7920($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X7920);
    // 0x80090984: addu        $t9, $t8, $a3
    ctx->r25 = ADD32(ctx->r24, ctx->r7);
    // 0x80090988: subu        $a0, $t9, $v0
    ctx->r4 = SUB32(ctx->r25, ctx->r2);
    // 0x8009098C: jal         0x800993C0
    // 0x80090990: sw          $t7, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r15;
    osVirtualToPhysical_recomp(rdram, ctx);
        goto after_0;
    // 0x80090990: sw          $t7, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r15;
    after_0:
    // 0x80090994: b           L_80090A98
    // 0x80090998: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
        goto L_80090A98;
    // 0x80090998: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_8009099C:
    // 0x8009099C: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x800909A0: bnel        $s0, $zero, L_8009095C
    if (ctx->r16 != 0) {
        // 0x800909A4: lw          $v0, 0x8($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X8);
            goto L_8009095C;
    }
    goto skip_0;
    // 0x800909A4: lw          $v0, 0x8($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X8);
    skip_0:
L_800909A8:
    // 0x800909A8: lw          $s0, 0x8($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X8);
    // 0x800909AC: lw          $t1, 0x0($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X0);
    // 0x800909B0: sw          $a3, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r7;
    // 0x800909B4: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x800909B8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800909BC: jal         0x80096EC0
    // 0x800909C0: sw          $t1, 0x8($s1)
    MEM_W(0X8, ctx->r17) = ctx->r9;
    __osDequeueThread_recomp(rdram, ctx);
        goto after_1;
    // 0x800909C0: sw          $t1, 0x8($s1)
    MEM_W(0X8, ctx->r17) = ctx->r9;
    after_1:
    // 0x800909C4: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x800909C8: lw          $a3, 0x48($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X48);
    // 0x800909CC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800909D0: beq         $a2, $zero, L_800909E8
    if (ctx->r6 == 0) {
        // 0x800909D4: or          $a1, $a2, $zero
        ctx->r5 = ctx->r6 | 0;
            goto L_800909E8;
    }
    // 0x800909D4: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x800909D8: jal         0x80096EF0
    // 0x800909DC: sw          $a3, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r7;
    __osEnqueueThread_recomp(rdram, ctx);
        goto after_2;
    // 0x800909DC: sw          $a3, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r7;
    after_2:
    // 0x800909E0: b           L_80090A14
    // 0x800909E4: lw          $a3, 0x48($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X48);
        goto L_80090A14;
    // 0x800909E4: lw          $a3, 0x48($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X48);
L_800909E8:
    // 0x800909E8: lw          $v0, 0x4($s1)
    ctx->r2 = MEM_W(ctx->r17, 0X4);
    // 0x800909EC: beql        $v0, $zero, L_80090A0C
    if (ctx->r2 == 0) {
        // 0x800909F0: sw          $s0, 0x4($s1)
        MEM_W(0X4, ctx->r17) = ctx->r16;
            goto L_80090A0C;
    }
    goto skip_1;
    // 0x800909F0: sw          $s0, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r16;
    skip_1:
    // 0x800909F4: sw          $s0, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r16;
    // 0x800909F8: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
    // 0x800909FC: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x80090A00: b           L_80090A14
    // 0x80090A04: sw          $s0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r16;
        goto L_80090A14;
    // 0x80090A04: sw          $s0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r16;
    // 0x80090A08: sw          $s0, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r16;
L_80090A0C:
    // 0x80090A0C: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x80090A10: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
L_80090A14:
    // 0x80090A14: andi        $v0, $a3, 0x1
    ctx->r2 = ctx->r7 & 0X1;
    // 0x80090A18: lw          $s1, 0x10($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X10);
    // 0x80090A1C: sw          $v0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r2;
    // 0x80090A20: subu        $a3, $a3, $v0
    ctx->r7 = SUB32(ctx->r7, ctx->r2);
    // 0x80090A24: sw          $a3, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r7;
    // 0x80090A28: lui         $t2, 0x800C
    ctx->r10 = S32(0X800C << 16);
    // 0x80090A2C: lw          $t2, -0x7920($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X7920);
    // 0x80090A30: lui         $t0, 0x800C
    ctx->r8 = S32(0X800C << 16);
    // 0x80090A34: addiu       $t0, $t0, -0x791C
    ctx->r8 = ADD32(ctx->r8, -0X791C);
    // 0x80090A38: sw          $t2, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->r10;
    // 0x80090A3C: lw          $v1, 0x0($t0)
    ctx->r3 = MEM_W(ctx->r8, 0X0);
    // 0x80090A40: lui         $t4, 0x8010
    ctx->r12 = S32(0X8010 << 16);
    // 0x80090A44: lui         $t6, 0x8010
    ctx->r14 = S32(0X8010 << 16);
    // 0x80090A48: sll         $t3, $v1, 2
    ctx->r11 = S32(ctx->r3 << 2);
    // 0x80090A4C: subu        $t3, $t3, $v1
    ctx->r11 = SUB32(ctx->r11, ctx->r3);
    // 0x80090A50: sll         $t3, $t3, 3
    ctx->r11 = S32(ctx->r11 << 3);
    // 0x80090A54: addiu       $t6, $t6, -0x1818
    ctx->r14 = ADD32(ctx->r14, -0X1818);
    // 0x80090A58: addiu       $t4, $t4, -0x1E18
    ctx->r12 = ADD32(ctx->r12, -0X1E18);
    // 0x80090A5C: addiu       $t5, $zero, 0xA00
    ctx->r13 = ADD32(0, 0XA00);
    // 0x80090A60: addiu       $t7, $v1, 0x1
    ctx->r15 = ADD32(ctx->r3, 0X1);
    // 0x80090A64: sw          $t7, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r15;
    // 0x80090A68: sw          $t5, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r13;
    // 0x80090A6C: addu        $a0, $t3, $t4
    ctx->r4 = ADD32(ctx->r11, ctx->r12);
    // 0x80090A70: sw          $t6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r14;
    // 0x80090A74: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80090A78: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80090A7C: jal         0x8009C560
    // 0x80090A80: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    func_8009C560(rdram, ctx);
        goto after_3;
    // 0x80090A80: sw          $s1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r17;
    after_3:
    // 0x80090A84: jal         0x800993C0
    // 0x80090A88: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    osVirtualToPhysical_recomp(rdram, ctx);
        goto after_4;
    // 0x80090A88: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_4:
    // 0x80090A8C: lw          $t8, 0x40($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X40);
    // 0x80090A90: addu        $v0, $v0, $t8
    ctx->r2 = ADD32(ctx->r2, ctx->r24);
    // 0x80090A94: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80090A98:
    // 0x80090A98: lw          $s0, 0x24($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X24);
    // 0x80090A9C: lw          $s1, 0x28($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X28);
    // 0x80090AA0: jr          $ra
    // 0x80090AA4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x80090AA4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_80090AA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090AA8: lui         $v1, 0x8010
    ctx->r3 = S32(0X8010 << 16);
    // 0x80090AAC: addiu       $v1, $v1, -0x2220
    ctx->r3 = ADD32(ctx->r3, -0X2220);
    // 0x80090AB0: lbu         $t6, 0x0($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X0);
    // 0x80090AB4: lui         $v0, 0x8009
    ctx->r2 = S32(0X8009 << 16);
    // 0x80090AB8: lui         $t7, 0x8010
    ctx->r15 = S32(0X8010 << 16);
    // 0x80090ABC: bne         $t6, $zero, L_80090AD8
    if (ctx->r14 != 0) {
        // 0x80090AC0: addiu       $v0, $v0, 0x92C
        ctx->r2 = ADD32(ctx->r2, 0X92C);
            goto L_80090AD8;
    }
    // 0x80090AC0: addiu       $v0, $v0, 0x92C
    ctx->r2 = ADD32(ctx->r2, 0X92C);
    // 0x80090AC4: addiu       $t7, $t7, -0x2210
    ctx->r15 = ADD32(ctx->r15, -0X2210);
    // 0x80090AC8: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x80090ACC: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x80090AD0: sw          $t7, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->r15;
    // 0x80090AD4: sb          $t8, 0x0($v1)
    MEM_B(0X0, ctx->r3) = ctx->r24;
L_80090AD8:
    // 0x80090AD8: jr          $ra
    // 0x80090ADC: sw          $v1, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r3;
    return;
    // 0x80090ADC: sw          $v1, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r3;
;}
RECOMP_FUNC void func_80090AE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090AE0: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80090AE4: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80090AE8: lui         $s4, 0x800C
    ctx->r20 = S32(0X800C << 16);
    // 0x80090AEC: addiu       $s4, $s4, -0x791C
    ctx->r20 = ADD32(ctx->r20, -0X791C);
    // 0x80090AF0: lw          $t6, 0x0($s4)
    ctx->r14 = MEM_W(ctx->r20, 0X0);
    // 0x80090AF4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80090AF8: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80090AFC: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80090B00: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80090B04: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80090B08: beq         $t6, $zero, L_80090B40
    if (ctx->r14 == 0) {
        // 0x80090B0C: or          $s0, $zero, $zero
        ctx->r16 = 0 | 0;
            goto L_80090B40;
    }
    // 0x80090B0C: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80090B10: lui         $s1, 0x8010
    ctx->r17 = S32(0X8010 << 16);
    // 0x80090B14: addiu       $s1, $s1, -0x1818
    ctx->r17 = ADD32(ctx->r17, -0X1818);
    // 0x80090B18: addiu       $s2, $sp, 0x40
    ctx->r18 = ADD32(ctx->r29, 0X40);
    // 0x80090B1C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
L_80090B20:
    // 0x80090B20: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80090B24: jal         0x80096CE0
    // 0x80090B28: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    osRecvMesg_recomp(rdram, ctx);
        goto after_0;
    // 0x80090B28: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80090B2C: lw          $t7, 0x0($s4)
    ctx->r15 = MEM_W(ctx->r20, 0X0);
    // 0x80090B30: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80090B34: sltu        $at, $s0, $t7
    ctx->r1 = ctx->r16 < ctx->r15 ? 1 : 0;
    // 0x80090B38: bnel        $at, $zero, L_80090B20
    if (ctx->r1 != 0) {
        // 0x80090B3C: or          $a0, $s1, $zero
        ctx->r4 = ctx->r17 | 0;
            goto L_80090B20;
    }
    goto skip_0;
    // 0x80090B3C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    skip_0:
L_80090B40:
    // 0x80090B40: lui         $s2, 0x8010
    ctx->r18 = S32(0X8010 << 16);
    // 0x80090B44: addiu       $s2, $s2, -0x2220
    ctx->r18 = ADD32(ctx->r18, -0X2220);
    // 0x80090B48: lw          $s0, 0x4($s2)
    ctx->r16 = MEM_W(ctx->r18, 0X4);
    // 0x80090B4C: lui         $s3, 0x800C
    ctx->r19 = S32(0X800C << 16);
    // 0x80090B50: addiu       $s3, $s3, -0x7920
    ctx->r19 = ADD32(ctx->r19, -0X7920);
    // 0x80090B54: beq         $s0, $zero, L_80090BC0
    if (ctx->r16 == 0) {
        // 0x80090B58: nop
    
            goto L_80090BC0;
    }
    // 0x80090B58: nop

L_80090B5C:
    // 0x80090B5C: lw          $t9, 0xC($s0)
    ctx->r25 = MEM_W(ctx->r16, 0XC);
    // 0x80090B60: lw          $t8, 0x0($s3)
    ctx->r24 = MEM_W(ctx->r19, 0X0);
    // 0x80090B64: lw          $v0, 0x0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X0);
    // 0x80090B68: addiu       $t0, $t9, 0x5
    ctx->r8 = ADD32(ctx->r25, 0X5);
    // 0x80090B6C: sltu        $at, $t0, $t8
    ctx->r1 = ctx->r8 < ctx->r24 ? 1 : 0;
    // 0x80090B70: beq         $at, $zero, L_80090BB8
    if (ctx->r1 == 0) {
        // 0x80090B74: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_80090BB8;
    }
    // 0x80090B74: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x80090B78: lw          $t1, 0x4($s2)
    ctx->r9 = MEM_W(ctx->r18, 0X4);
    // 0x80090B7C: bne         $s0, $t1, L_80090B88
    if (ctx->r16 != ctx->r9) {
        // 0x80090B80: nop
    
            goto L_80090B88;
    }
    // 0x80090B80: nop

    // 0x80090B84: sw          $v0, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r2;
L_80090B88:
    // 0x80090B88: jal         0x80096EC0
    // 0x80090B8C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    __osDequeueThread_recomp(rdram, ctx);
        goto after_1;
    // 0x80090B8C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x80090B90: lw          $a1, 0x8($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X8);
    // 0x80090B94: beql        $a1, $zero, L_80090BB0
    if (ctx->r5 == 0) {
        // 0x80090B98: sw          $s0, 0x8($s2)
        MEM_W(0X8, ctx->r18) = ctx->r16;
            goto L_80090BB0;
    }
    goto skip_1;
    // 0x80090B98: sw          $s0, 0x8($s2)
    MEM_W(0X8, ctx->r18) = ctx->r16;
    skip_1:
    // 0x80090B9C: jal         0x80096EF0
    // 0x80090BA0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    __osEnqueueThread_recomp(rdram, ctx);
        goto after_2;
    // 0x80090BA0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x80090BA4: b           L_80090BB8
    // 0x80090BA8: nop

        goto L_80090BB8;
    // 0x80090BA8: nop

    // 0x80090BAC: sw          $s0, 0x8($s2)
    MEM_W(0X8, ctx->r18) = ctx->r16;
L_80090BB0:
    // 0x80090BB0: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    // 0x80090BB4: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
L_80090BB8:
    // 0x80090BB8: bne         $s1, $zero, L_80090B5C
    if (ctx->r17 != 0) {
        // 0x80090BBC: or          $s0, $s1, $zero
        ctx->r16 = ctx->r17 | 0;
            goto L_80090B5C;
    }
    // 0x80090BBC: or          $s0, $s1, $zero
    ctx->r16 = ctx->r17 | 0;
L_80090BC0:
    // 0x80090BC0: lui         $s3, 0x800C
    ctx->r19 = S32(0X800C << 16);
    // 0x80090BC4: addiu       $s3, $s3, -0x7920
    ctx->r19 = ADD32(ctx->r19, -0X7920);
    // 0x80090BC8: lw          $t2, 0x0($s3)
    ctx->r10 = MEM_W(ctx->r19, 0X0);
    // 0x80090BCC: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80090BD0: sw          $zero, 0x0($s4)
    MEM_W(0X0, ctx->r20) = 0;
    // 0x80090BD4: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80090BD8: sw          $t3, 0x0($s3)
    MEM_W(0X0, ctx->r19) = ctx->r11;
    // 0x80090BDC: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80090BE0: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80090BE4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80090BE8: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80090BEC: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80090BF0: jr          $ra
    // 0x80090BF4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x80090BF4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_80090C00(uint8_t* rdram, recomp_context* ctx) {
    fprintf(stderr, "[func_80090C00] ENTERED\n"); fflush(stderr);
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090C00: addiu       $sp, $sp, -0xA0
    ctx->r29 = ADD32(ctx->r29, -0XA0);
    // 0x80090C04: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80090C08: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80090C0C: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80090C10: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x80090C14: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80090C18: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80090C1C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80090C20: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80090C24: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80090C28: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80090C2C: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80090C30: lui         $s6, 0x10
    ctx->r22 = S32(0X10 << 16);
    // 0x80090C34: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80090C38:
    // 0x80090C38: bnel        $s0, $s6, L_80090C38
    if (ctx->r16 != ctx->r22) {
        // 0x80090C3C: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80090C38;
    }
    goto skip_0;
    // 0x80090C3C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    skip_0:
    fprintf(stderr, "[func_80090C00] delay loop done, starting DMA copies\n"); fflush(stderr);
    // 0x80090C40: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x80090C44: lui         $a1, 0x8010
    ctx->r5 = S32(0X8010 << 16);
    // 0x80090C48: addiu       $a1, $a1, -0x16C8
    ctx->r5 = ADD32(ctx->r5, -0X16C8);
    // 0x80090C4C: addiu       $a0, $a0, -0x16E8
    ctx->r4 = ADD32(ctx->r4, -0X16E8);
    // 0x80090C50: jal         0x800A1300
    // 0x80090C54: lui         $a2, 0x4
    ctx->r6 = S32(0X4 << 16);
    func_800A1300(rdram, ctx);
        goto after_0;
    // 0x80090C54: lui         $a2, 0x4
    ctx->r6 = S32(0X4 << 16);
    after_0:
    // 0x80090C58: lui         $s4, 0x800C
    ctx->r20 = S32(0X800C << 16);
    // 0x80090C5C: lui         $s5, 0x800C
    ctx->r21 = S32(0X800C << 16);
    // 0x80090C60: lui         $s2, 0x8010
    ctx->r18 = S32(0X8010 << 16);
    // 0x80090C64: lui         $fp, 0x8010
    ctx->r30 = S32(0X8010 << 16);
    // 0x80090C68: lui         $s7, 0x8028
    ctx->r23 = S32(0X8028 << 16);
    // 0x80090C6C: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x80090C70: addiu       $s7, $s7, -0x2F20
    ctx->r23 = ADD32(ctx->r23, -0X2F20);
    // 0x80090C74: addiu       $fp, $fp, -0x16CC
    ctx->r30 = ADD32(ctx->r30, -0X16CC);
    // 0x80090C78: addiu       $s2, $s2, -0x16D8
    ctx->r18 = ADD32(ctx->r18, -0X16D8);
    // 0x80090C7C: addiu       $s5, $s5, -0x7910
    ctx->r21 = ADD32(ctx->r21, -0X7910);
    // 0x80090C80: addiu       $s4, $s4, -0x7904
    ctx->r20 = ADD32(ctx->r20, -0X7904);
    // 0x80090C84: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_80090C88:
    // 0x80090C88: lw          $a3, 0x0($s5)
    ctx->r7 = MEM_W(ctx->r21, 0X0);
    // 0x80090C8C: lw          $t6, 0x0($s4)
    ctx->r14 = MEM_W(ctx->r20, 0X0);
    // 0x80090C90: addu        $a1, $s7, $s3
    ctx->r5 = ADD32(ctx->r23, ctx->r19);
    // 0x80090C94: sw          $a1, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r5;
    // 0x80090C98: subu        $s1, $t6, $a3
    ctx->r17 = SUB32(ctx->r14, ctx->r7);
    // 0x80090C9C: beq         $s1, $zero, L_80090CEC
    if (ctx->r17 == 0) {
        // 0x80090CA0: or          $a2, $s1, $zero
        ctx->r6 = ctx->r17 | 0;
            goto L_80090CEC;
    }
    // 0x80090CA0: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80090CA4: andi        $a0, $s1, 0x3
    ctx->r4 = ctx->r17 & 0X3;
    // 0x80090CA8: beq         $a0, $zero, L_80090CC8
    if (ctx->r4 == 0) {
        // 0x80090CAC: or          $v1, $a0, $zero
        ctx->r3 = ctx->r4 | 0;
            goto L_80090CC8;
    }
    // 0x80090CAC: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80090CB0: addu        $v0, $a1, $s0
    ctx->r2 = ADD32(ctx->r5, ctx->r16);
L_80090CB4:
    // 0x80090CB4: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x80090CB8: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80090CBC: bne         $v1, $s0, L_80090CB4
    if (ctx->r3 != ctx->r16) {
        // 0x80090CC0: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80090CB4;
    }
    // 0x80090CC0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80090CC4: beq         $s0, $s1, L_80090CE8
    if (ctx->r16 == ctx->r17) {
        // 0x80090CC8: addu        $v0, $a1, $s0
        ctx->r2 = ADD32(ctx->r5, ctx->r16);
            goto L_80090CE8;
    }
L_80090CC8:
    // 0x80090CC8: addu        $v0, $a1, $s0
    ctx->r2 = ADD32(ctx->r5, ctx->r16);
L_80090CCC:
    // 0x80090CCC: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80090CD0: sb          $zero, 0x1($v0)
    MEM_B(0X1, ctx->r2) = 0;
    // 0x80090CD4: sb          $zero, 0x2($v0)
    MEM_B(0X2, ctx->r2) = 0;
    // 0x80090CD8: sb          $zero, 0x3($v0)
    MEM_B(0X3, ctx->r2) = 0;
    // 0x80090CDC: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80090CE0: bne         $s0, $s1, L_80090CCC
    if (ctx->r16 != ctx->r17) {
        // 0x80090CE4: sb          $zero, -0x4($v0)
        MEM_B(-0X4, ctx->r2) = 0;
            goto L_80090CCC;
    }
    // 0x80090CE4: sb          $zero, -0x4($v0)
    MEM_B(-0X4, ctx->r2) = 0;
L_80090CE8:
    // 0x80090CE8: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_80090CEC:
    // 0x80090CEC: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80090CF0: jal         0x8001A42C
    // 0x80090CF4: lw          $a1, 0x0($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X0);
    func_8001A42C(rdram, ctx);
        goto after_1;
    // 0x80090CF4: lw          $a1, 0x0($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X0);
    after_1:
    // 0x80090CF8: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x80090CFC: sltu        $at, $s2, $fp
    ctx->r1 = ctx->r18 < ctx->r30 ? 1 : 0;
    // 0x80090D00: addiu       $s4, $s4, 0x4
    ctx->r20 = ADD32(ctx->r20, 0X4);
    // 0x80090D04: addiu       $s5, $s5, 0x4
    ctx->r21 = ADD32(ctx->r21, 0X4);
    // 0x80090D08: bne         $at, $zero, L_80090C88
    if (ctx->r1 != 0) {
        // 0x80090D0C: addu        $s3, $s3, $s1
        ctx->r19 = ADD32(ctx->r19, ctx->r17);
            goto L_80090C88;
    }
    // 0x80090D0C: addu        $s3, $s3, $s1
    ctx->r19 = ADD32(ctx->r19, ctx->r17);
    // 0x80090D10: lui         $s2, 0x8010
    ctx->r18 = S32(0X8010 << 16);
    // 0x80090D14: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x80090D18: lui         $s3, 0x800C
    ctx->r19 = S32(0X800C << 16);
    // 0x80090D1C: addiu       $s3, $s3, -0x78EC
    ctx->r19 = ADD32(ctx->r19, -0X78EC);
    // 0x80090D20: addiu       $s1, $s1, -0x78F8
    ctx->r17 = ADD32(ctx->r17, -0X78F8);
    // 0x80090D24: addiu       $s2, $s2, -0x16D8
    ctx->r18 = ADD32(ctx->r18, -0X16D8);
L_80090D28:
    // 0x80090D28: lw          $a0, 0x0($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X0);
    // 0x80090D2C: jal         0x80099044
    // 0x80090D30: lw          $a1, 0x0($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X0);
    func_80099044(rdram, ctx);
        goto after_2;
    // 0x80090D30: lw          $a1, 0x0($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X0);
    after_2:
    // 0x80090D34: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x80090D38: bne         $s1, $s3, L_80090D28
    if (ctx->r17 != ctx->r19) {
        // 0x80090D3C: addiu       $s2, $s2, 0x4
        ctx->r18 = ADD32(ctx->r18, 0X4);
            goto L_80090D28;
    }
    // 0x80090D3C: addiu       $s2, $s2, 0x4
    ctx->r18 = ADD32(ctx->r18, 0X4);
    // 0x80090D40: lui         $t0, 0x8010
    ctx->r8 = S32(0X8010 << 16);
    // 0x80090D44: lui         $t2, 0x800C
    ctx->r10 = S32(0X800C << 16);
    // 0x80090D48: addiu       $t7, $zero, 0x14
    ctx->r15 = ADD32(0, 0X14);
    // 0x80090D4C: addiu       $t8, $zero, 0x14
    ctx->r24 = ADD32(0, 0X14);
    // 0x80090D50: addiu       $t9, $zero, 0xA0
    ctx->r25 = ADD32(0, 0XA0);
    // 0x80090D54: addiu       $t0, $t0, -0x16E8
    ctx->r8 = ADD32(ctx->r8, -0X16E8);
    // 0x80090D58: addiu       $t1, $zero, 0x6
    ctx->r9 = ADD32(0, 0X6);
    // 0x80090D5C: addiu       $t2, $t2, -0x78E0
    ctx->r10 = ADD32(ctx->r10, -0X78E0);
    // 0x80090D60: ori         $t3, $zero, 0xAC44
    ctx->r11 = 0 | 0XAC44;
    // 0x80090D64: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x80090D68: addiu       $t5, $zero, 0x1000
    ctx->r13 = ADD32(0, 0X1000);
    // 0x80090D6C: sw          $t7, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r15;
    // 0x80090D70: sw          $t8, 0x80($sp)
    MEM_W(0X80, ctx->r29) = ctx->r24;
    // 0x80090D74: sw          $t9, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r25;
    // 0x80090D78: sw          $zero, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = 0;
    // 0x80090D7C: sw          $zero, 0x94($sp)
    MEM_W(0X94, ctx->r29) = 0;
    // 0x80090D80: sw          $t0, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r8;
    // 0x80090D84: sb          $t1, 0x98($sp)
    MEM_B(0X98, ctx->r29) = ctx->r9;
    // 0x80090D88: sw          $t2, 0x9C($sp)
    MEM_W(0X9C, ctx->r29) = ctx->r10;
    // 0x80090D8C: sw          $t3, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r11;
    // 0x80090D90: sw          $t4, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r12;
    // 0x80090D94: sw          $t5, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r13;
    // 0x80090D98: addiu       $a0, $sp, 0x7C
    ctx->r4 = ADD32(ctx->r29, 0X7C);
    // 0x80090D9C: addiu       $a1, $zero, 0xC
    ctx->r5 = ADD32(0, 0XC);
    // 0x80090DA0: jal         0x800902D0
    // 0x80090DA4: addiu       $a2, $sp, 0x70
    ctx->r6 = ADD32(ctx->r29, 0X70);
    func_800902D0(rdram, ctx);
        goto after_3;
    // 0x80090DA4: addiu       $a2, $sp, 0x70
    ctx->r6 = ADD32(ctx->r29, 0X70);
    after_3:
    // 0x80090DA8: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x80090DAC: jal         0x80090DF0
    // 0x80090DB0: addiu       $a0, $a0, -0x1700
    ctx->r4 = ADD32(ctx->r4, -0X1700);
    func_80090DF0(rdram, ctx);
        goto after_4;
    // 0x80090DB0: addiu       $a0, $a0, -0x1700
    ctx->r4 = ADD32(ctx->r4, -0X1700);
    after_4:
    // 0x80090DB4: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80090DB8:
    // 0x80090DB8: bnel        $s0, $s6, L_80090DB8
    if (ctx->r16 != ctx->r22) {
        // 0x80090DBC: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80090DB8;
    }
    goto skip_1;
    // 0x80090DBC: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    skip_1:
    // 0x80090DC0: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80090DC4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80090DC8: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80090DCC: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80090DD0: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80090DD4: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80090DD8: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80090DDC: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80090DE0: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80090DE4: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80090DE8: jr          $ra
    // 0x80090DEC: addiu       $sp, $sp, 0xA0
    ctx->r29 = ADD32(ctx->r29, 0XA0);
    return;
    // 0x80090DEC: addiu       $sp, $sp, 0xA0
    ctx->r29 = ADD32(ctx->r29, 0XA0);
;}
RECOMP_FUNC void func_80090DF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090DF0: addiu       $t6, $zero, 0x8
    ctx->r14 = ADD32(0, 0X8);
    // 0x80090DF4: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80090DF8: sb          $t6, -0x16C8($at)
    MEM_B(-0X16C8, ctx->r1) = ctx->r14;
    // 0x80090DFC: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80090E00: lui         $v1, 0x8014
    ctx->r3 = S32(0X8014 << 16);
    // 0x80090E04: lui         $a2, 0x8014
    ctx->r6 = S32(0X8014 << 16);
    // 0x80090E08: lui         $a3, 0x8014
    ctx->r7 = S32(0X8014 << 16);
    // 0x80090E0C: addiu       $a3, $a3, -0x1646
    ctx->r7 = ADD32(ctx->r7, -0X1646);
    // 0x80090E10: addiu       $a2, $a2, -0x16C3
    ctx->r6 = ADD32(ctx->r6, -0X16C3);
    // 0x80090E14: addiu       $v1, $v1, -0x16C0
    ctx->r3 = ADD32(ctx->r3, -0X16C0);
    // 0x80090E18: addiu       $v0, $v0, -0x1647
    ctx->r2 = ADD32(ctx->r2, -0X1647);
    // 0x80090E1C: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80090E20: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80090E24: sb          $zero, 0x0($v1)
    MEM_B(0X0, ctx->r3) = 0;
    // 0x80090E28: sb          $zero, 0x0($a2)
    MEM_B(0X0, ctx->r6) = 0;
    // 0x80090E2C: sb          $zero, 0x0($a3)
    MEM_B(0X0, ctx->r7) = 0;
    // 0x80090E30: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80090E34: andi        $t1, $zero, 0xFF
    ctx->r9 = 0 & 0XFF;
    // 0x80090E38: addiu       $t0, $t0, -0x16C2
    ctx->r8 = ADD32(ctx->r8, -0X16C2);
    // 0x80090E3C: sh          $t1, -0x16C6($at)
    MEM_H(-0X16C6, ctx->r1) = ctx->r9;
    // 0x80090E40: sb          $zero, 0x0($t0)
    MEM_B(0X0, ctx->r8) = 0;
    // 0x80090E44: lb          $t2, 0x0($t0)
    ctx->r10 = MEM_B(ctx->r8, 0X0);
    // 0x80090E48: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80090E4C: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80090E50: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80090E54: sb          $t2, -0x16C1($at)
    MEM_B(-0X16C1, ctx->r1) = ctx->r10;
    // 0x80090E58: lui         $t3, 0x8009
    ctx->r11 = S32(0X8009 << 16);
    // 0x80090E5C: addiu       $t3, $t3, 0xE8C
    ctx->r11 = ADD32(ctx->r11, 0XE8C);
    // 0x80090E60: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80090E64: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
    // 0x80090E68: sw          $t3, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r11;
    // 0x80090E6C: sw          $a0, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r4;
    // 0x80090E70: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80090E74: jal         0x800A3AF0
    // 0x80090E78: lw          $a0, -0x6530($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X6530);
    func_800A3AF0(rdram, ctx);
        goto after_0;
    // 0x80090E78: lw          $a0, -0x6530($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X6530);
    after_0:
    // 0x80090E7C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80090E80: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80090E84: jr          $ra
    // 0x80090E88: nop

    return;
    // 0x80090E88: nop

;}
RECOMP_FUNC void func_80090E8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090E8C: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80090E90: lbu         $v0, -0x16C8($v0)
    ctx->r2 = MEM_BU(ctx->r2, -0X16C8);
    // 0x80090E94: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80090E98: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80090E9C: beq         $v0, $zero, L_80090ED0
    if (ctx->r2 == 0) {
        // 0x80090EA0: sw          $a0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r4;
            goto L_80090ED0;
    }
    // 0x80090EA0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80090EA4: addiu       $at, $zero, 0x8
    ctx->r1 = ADD32(0, 0X8);
    // 0x80090EA8: bnel        $v0, $at, L_80090EC4
    if (ctx->r2 != ctx->r1) {
        // 0x80090EAC: addiu       $t6, $v0, -0x1
        ctx->r14 = ADD32(ctx->r2, -0X1);
            goto L_80090EC4;
    }
    goto skip_0;
    // 0x80090EAC: addiu       $t6, $v0, -0x1
    ctx->r14 = ADD32(ctx->r2, -0X1);
    skip_0:
    // 0x80090EB0: jal         0x80090F60
    // 0x80090EB4: nop

    func_80090F60(rdram, ctx);
        goto after_0;
    // 0x80090EB4: nop

    after_0:
    // 0x80090EB8: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80090EBC: lbu         $v0, -0x16C8($v0)
    ctx->r2 = MEM_BU(ctx->r2, -0X16C8);
    // 0x80090EC0: addiu       $t6, $v0, -0x1
    ctx->r14 = ADD32(ctx->r2, -0X1);
L_80090EC4:
    // 0x80090EC4: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80090EC8: b           L_80090F4C
    // 0x80090ECC: sb          $t6, -0x16C8($at)
    MEM_B(-0X16C8, ctx->r1) = ctx->r14;
        goto L_80090F4C;
    // 0x80090ECC: sb          $t6, -0x16C8($at)
    MEM_B(-0X16C8, ctx->r1) = ctx->r14;
L_80090ED0:
    // 0x80090ED0: lui         $t7, 0x8014
    ctx->r15 = S32(0X8014 << 16);
    // 0x80090ED4: lhu         $t7, -0x161C($t7)
    ctx->r15 = MEM_HU(ctx->r15, -0X161C);
    // 0x80090ED8: beq         $t7, $zero, L_80090EE8
    if (ctx->r15 == 0) {
        // 0x80090EDC: nop
    
            goto L_80090EE8;
    }
    // 0x80090EDC: nop

    // 0x80090EE0: jal         0x80092710
    // 0x80090EE4: nop

    func_80092710(rdram, ctx);
        goto after_1;
    // 0x80090EE4: nop

    after_1:
L_80090EE8:
    // 0x80090EE8: lui         $t8, 0x8014
    ctx->r24 = S32(0X8014 << 16);
    // 0x80090EEC: lbu         $t8, -0x1390($t8)
    ctx->r24 = MEM_BU(ctx->r24, -0X1390);
    // 0x80090EF0: beq         $t8, $zero, L_80090F00
    if (ctx->r24 == 0) {
        // 0x80090EF4: nop
    
            goto L_80090F00;
    }
    // 0x80090EF4: nop

    // 0x80090EF8: jal         0x8009175C
    // 0x80090EFC: nop

    func_8009175C(rdram, ctx);
        goto after_2;
    // 0x80090EFC: nop

    after_2:
L_80090F00:
    // 0x80090F00: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x80090F04: lbu         $t9, -0x1358($t9)
    ctx->r25 = MEM_BU(ctx->r25, -0X1358);
    // 0x80090F08: beq         $t9, $zero, L_80090F18
    if (ctx->r25 == 0) {
        // 0x80090F0C: nop
    
            goto L_80090F18;
    }
    // 0x80090F0C: nop

    // 0x80090F10: jal         0x80091984
    // 0x80090F14: nop

    func_80091984(rdram, ctx);
        goto after_3;
    // 0x80090F14: nop

    after_3:
L_80090F18:
    // 0x80090F18: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80090F1C: lbu         $t0, -0x16C7($t0)
    ctx->r8 = MEM_BU(ctx->r8, -0X16C7);
    // 0x80090F20: bne         $t0, $zero, L_80090F30
    if (ctx->r8 != 0) {
        // 0x80090F24: nop
    
            goto L_80090F30;
    }
    // 0x80090F24: nop

    // 0x80090F28: jal         0x80091630
    // 0x80090F2C: nop

    func_80091630(rdram, ctx);
        goto after_4;
    // 0x80090F2C: nop

    after_4:
L_80090F30:
    // 0x80090F30: jal         0x80093430
    // 0x80090F34: nop

    func_80093430(rdram, ctx);
        goto after_5;
    // 0x80090F34: nop

    after_5:
    // 0x80090F38: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80090F3C: addiu       $v0, $v0, -0x16C6
    ctx->r2 = ADD32(ctx->r2, -0X16C6);
    // 0x80090F40: lhu         $t1, 0x0($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0X0);
    // 0x80090F44: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80090F48: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
L_80090F4C:
    // 0x80090F4C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80090F50: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80090F54: addiu       $v0, $zero, 0x1388
    ctx->r2 = ADD32(0, 0X1388);
    // 0x80090F58: jr          $ra
    // 0x80090F5C: nop

    return;
    // 0x80090F5C: nop

;}
RECOMP_FUNC void func_80090F60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80090F60: addiu       $sp, $sp, -0x78
    ctx->r29 = ADD32(ctx->r29, -0X78);
    // 0x80090F64: sw          $s7, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r23;
    // 0x80090F68: sw          $s5, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r21;
    // 0x80090F6C: sw          $s3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r19;
    // 0x80090F70: sw          $s2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r18;
    // 0x80090F74: sw          $s1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r17;
    // 0x80090F78: sw          $s0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r16;
    // 0x80090F7C: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x80090F80: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80090F84: sw          $s6, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r22;
    // 0x80090F88: sw          $s4, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r20;
    // 0x80090F8C: lui         $s0, 0x8014
    ctx->r16 = S32(0X8014 << 16);
    // 0x80090F90: lui         $s1, 0x8014
    ctx->r17 = S32(0X8014 << 16);
    // 0x80090F94: lui         $s2, 0x8029
    ctx->r18 = S32(0X8029 << 16);
    // 0x80090F98: lui         $s3, 0x800C
    ctx->r19 = S32(0X800C << 16);
    // 0x80090F9C: lui         $s5, 0x8010
    ctx->r21 = S32(0X8010 << 16);
    // 0x80090FA0: lui         $s7, 0x8029
    ctx->r23 = S32(0X8029 << 16);
    // 0x80090FA4: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x80090FA8: sw          $ra, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r31;
    // 0x80090FAC: addiu       $s7, $s7, 0x11C0
    ctx->r23 = ADD32(ctx->r23, 0X11C0);
    // 0x80090FB0: addiu       $s5, $s5, -0x16D8
    ctx->r21 = ADD32(ctx->r21, -0X16D8);
    // 0x80090FB4: addiu       $s3, $s3, -0x6530
    ctx->r19 = ADD32(ctx->r19, -0X6530);
    // 0x80090FB8: addiu       $s2, $s2, 0xE0
    ctx->r18 = ADD32(ctx->r18, 0XE0);
    // 0x80090FBC: addiu       $s1, $s1, -0x15C8
    ctx->r17 = ADD32(ctx->r17, -0X15C8);
    // 0x80090FC0: addiu       $s0, $s0, -0x1394
    ctx->r16 = ADD32(ctx->r16, -0X1394);
    // 0x80090FC4: addiu       $s4, $sp, 0x70
    ctx->r20 = ADD32(ctx->r29, 0X70);
    // 0x80090FC8: addiu       $s6, $zero, 0x1
    ctx->r22 = ADD32(0, 0X1);
L_80090FCC:
    // 0x80090FCC: addiu       $t6, $zero, 0xA
    ctx->r14 = ADD32(0, 0XA);
    // 0x80090FD0: sh          $t6, 0x70($sp)
    MEM_H(0X70, ctx->r29) = ctx->r14;
    // 0x80090FD4: sh          $zero, 0x72($sp)
    MEM_H(0X72, ctx->r29) = 0;
    // 0x80090FD8: sb          $zero, 0x74($sp)
    MEM_B(0X74, ctx->r29) = 0;
    // 0x80090FDC: lw          $a0, 0x0($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X0);
    // 0x80090FE0: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80090FE4: jal         0x80099278
    // 0x80090FE8: or          $a2, $s4, $zero
    ctx->r6 = ctx->r20 | 0;
    func_80099278(rdram, ctx);
        goto after_0;
    // 0x80090FE8: or          $a2, $s4, $zero
    ctx->r6 = ctx->r20 | 0;
    after_0:
    // 0x80090FEC: sw          $s2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r18;
    // 0x80090FF0: sb          $zero, 0xD($s2)
    MEM_B(0XD, ctx->r18) = 0;
    // 0x80090FF4: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x80090FF8: mfc1        $a3, $f20
    ctx->r7 = (int32_t)ctx->f20.u32l;
    // 0x80090FFC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80091000: lbu         $t8, 0xD($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0XD);
    // 0x80091004: sb          $t8, 0xC($v1)
    MEM_B(0XC, ctx->r3) = ctx->r24;
    // 0x80091008: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    // 0x8009100C: lbu         $t9, 0xC($v1)
    ctx->r25 = MEM_BU(ctx->r3, 0XC);
    // 0x80091010: lbu         $t6, 0xD($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0XD);
    // 0x80091014: sll         $t3, $t9, 2
    ctx->r11 = S32(ctx->r25 << 2);
    // 0x80091018: addu        $t4, $s5, $t3
    ctx->r12 = ADD32(ctx->r21, ctx->r11);
    // 0x8009101C: lw          $t5, 0x0($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X0);
    // 0x80091020: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80091024: lw          $t0, 0x4($t5)
    ctx->r8 = MEM_W(ctx->r13, 0X4);
    // 0x80091028: addu        $t8, $t0, $t7
    ctx->r24 = ADD32(ctx->r8, ctx->r15);
    // 0x8009102C: lw          $t1, 0xC($t8)
    ctx->r9 = MEM_W(ctx->r24, 0XC);
    // 0x80091030: addiu       $t8, $zero, 0x40
    ctx->r24 = ADD32(0, 0X40);
    // 0x80091034: lw          $t2, 0x10($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X10);
    // 0x80091038: lw          $t9, 0x8($t2)
    ctx->r25 = MEM_W(ctx->r10, 0X8);
    // 0x8009103C: sw          $t9, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->r25;
    // 0x80091040: lw          $t3, 0x0($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X0);
    // 0x80091044: swc1        $f20, 0xB0($t3)
    MEM_W(0XB0, ctx->r11) = ctx->f20.u32l;
    // 0x80091048: lw          $t4, 0x0($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X0);
    // 0x8009104C: sh          $zero, 0xB4($t4)
    MEM_H(0XB4, ctx->r12) = 0;
    // 0x80091050: lw          $t5, 0x0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X0);
    // 0x80091054: sh          $zero, 0xB6($t5)
    MEM_H(0XB6, ctx->r13) = 0;
    // 0x80091058: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x8009105C: sb          $s6, 0xE($t6)
    MEM_B(0XE, ctx->r14) = ctx->r22;
    // 0x80091060: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x80091064: lw          $a0, 0x0($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X0);
    // 0x80091068: lw          $a2, 0x14($t7)
    ctx->r6 = MEM_W(ctx->r15, 0X14);
    // 0x8009106C: sw          $zero, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = 0;
    // 0x80091070: sw          $zero, 0x18($sp)
    MEM_W(0X18, ctx->r29) = 0;
    // 0x80091074: sw          $t8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r24;
    // 0x80091078: jal         0x800A4DF0
    // 0x8009107C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    func_800A4DF0(rdram, ctx);
        goto after_1;
    // 0x8009107C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
    // 0x80091080: addiu       $s2, $s2, 0xD8
    ctx->r18 = ADD32(ctx->r18, 0XD8);
    // 0x80091084: bne         $s2, $s7, L_80090FCC
    if (ctx->r18 != ctx->r23) {
        // 0x80091088: addiu       $s1, $s1, 0x1C
        ctx->r17 = ADD32(ctx->r17, 0X1C);
            goto L_80090FCC;
    }
    // 0x80091088: addiu       $s1, $s1, 0x1C
    ctx->r17 = ADD32(ctx->r17, 0X1C);
    // 0x8009108C: jal         0x80092BE0
    // 0x80091090: nop

    func_80092BE0(rdram, ctx);
        goto after_2;
    // 0x80091090: nop

    after_2:
    // 0x80091094: lw          $a0, 0x0($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X0);
    // 0x80091098: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8009109C: jal         0x800A1700
    // 0x800910A0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_800A1700(rdram, ctx);
        goto after_3;
    // 0x800910A0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_3:
    // 0x800910A4: lw          $ra, 0x4C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X4C);
    // 0x800910A8: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x800910AC: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x800910B0: lw          $s0, 0x2C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X2C);
    // 0x800910B4: lw          $s1, 0x30($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X30);
    // 0x800910B8: lw          $s2, 0x34($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X34);
    // 0x800910BC: lw          $s3, 0x38($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X38);
    // 0x800910C0: lw          $s4, 0x3C($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X3C);
    // 0x800910C4: lw          $s5, 0x40($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X40);
    // 0x800910C8: lw          $s6, 0x44($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X44);
    // 0x800910CC: lw          $s7, 0x48($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X48);
    // 0x800910D0: sw          $v0, -0x1320($at)
    MEM_W(-0X1320, ctx->r1) = ctx->r2;
    // 0x800910D4: jr          $ra
    // 0x800910D8: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
    return;
    // 0x800910D8: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
;}
RECOMP_FUNC void func_800910DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800910DC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800910E0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800910E4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x800910E8: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x800910EC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x800910F0: jal         0x80091194
    // 0x800910F4: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80091194(rdram, ctx);
        goto after_0;
    // 0x800910F4: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x800910F8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800910FC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80091100: jr          $ra
    // 0x80091104: nop

    return;
    // 0x80091104: nop

;}
RECOMP_FUNC void func_80091108(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091108: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8009110C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80091110: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80091114: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x80091118: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8009111C: jal         0x80091194
    // 0x80091120: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80091194(rdram, ctx);
        goto after_0;
    // 0x80091120: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80091124: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80091128: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8009112C: jr          $ra
    // 0x80091130: nop

    return;
    // 0x80091130: nop

;}
RECOMP_FUNC void func_80091134(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091134: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80091138: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8009113C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80091140: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80091144: andi        $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 & 0XFF;
    // 0x80091148: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x8009114C: jal         0x80091194
    // 0x80091150: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80091194(rdram, ctx);
        goto after_0;
    // 0x80091150: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80091154: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80091158: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8009115C: jr          $ra
    // 0x80091160: nop

    return;
    // 0x80091160: nop

;}
RECOMP_FUNC void func_80091164(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091164: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80091168: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8009116C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80091170: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80091174: andi        $a2, $a1, 0xFF
    ctx->r6 = ctx->r5 & 0XFF;
    // 0x80091178: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x8009117C: jal         0x80091194
    // 0x80091180: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_80091194(rdram, ctx);
        goto after_0;
    // 0x80091180: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_0:
    // 0x80091184: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80091188: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8009118C: jr          $ra
    // 0x80091190: nop

    return;
    // 0x80091190: nop

;}
RECOMP_FUNC void func_80091194(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091194: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80091198: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x8009119C: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x800911A0: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x800911A4: andi        $a2, $a2, 0xFF
    ctx->r6 = ctx->r6 & 0XFF;
    // 0x800911A8: beq         $a0, $zero, L_80091264
    if (ctx->r4 == 0) {
        // 0x800911AC: andi        $a1, $a1, 0xFF
        ctx->r5 = ctx->r5 & 0XFF;
            goto L_80091264;
    }
    // 0x800911AC: andi        $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 & 0XFF;
    // 0x800911B0: lui         $t1, 0x8014
    ctx->r9 = S32(0X8014 << 16);
    // 0x800911B4: addiu       $t1, $t1, -0x16C7
    ctx->r9 = ADD32(ctx->r9, -0X16C7);
    // 0x800911B8: addiu       $t6, $zero, 0xFF
    ctx->r14 = ADD32(0, 0XFF);
    // 0x800911BC: beq         $a1, $zero, L_800911D0
    if (ctx->r5 == 0) {
        // 0x800911C0: sb          $t6, 0x0($t1)
        MEM_B(0X0, ctx->r9) = ctx->r14;
            goto L_800911D0;
    }
    // 0x800911C0: sb          $t6, 0x0($t1)
    MEM_B(0X0, ctx->r9) = ctx->r14;
    // 0x800911C4: slti        $at, $a1, 0x80
    ctx->r1 = SIGNED(ctx->r5) < 0X80 ? 1 : 0;
    // 0x800911C8: bne         $at, $zero, L_800911D8
    if (ctx->r1 != 0) {
        // 0x800911CC: or          $v0, $a1, $zero
        ctx->r2 = ctx->r5 | 0;
            goto L_800911D8;
    }
    // 0x800911CC: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
L_800911D0:
    // 0x800911D0: b           L_800911E0
    // 0x800911D4: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
        goto L_800911E0;
    // 0x800911D4: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
L_800911D8:
    // 0x800911D8: sll         $t7, $v0, 16
    ctx->r15 = S32(ctx->r2 << 16);
    // 0x800911DC: addu        $v1, $a0, $t7
    ctx->r3 = ADD32(ctx->r4, ctx->r15);
L_800911E0:
    // 0x800911E0: beq         $a2, $zero, L_800911F0
    if (ctx->r6 == 0) {
        // 0x800911E4: lui         $t2, 0x8014
        ctx->r10 = S32(0X8014 << 16);
            goto L_800911F0;
    }
    // 0x800911E4: lui         $t2, 0x8014
    ctx->r10 = S32(0X8014 << 16);
    // 0x800911E8: sll         $t8, $a2, 24
    ctx->r24 = S32(ctx->r6 << 24);
    // 0x800911EC: or          $v1, $v1, $t8
    ctx->r3 = ctx->r3 | ctx->r24;
L_800911F0:
    // 0x800911F0: addiu       $t2, $t2, -0x1646
    ctx->r10 = ADD32(ctx->r10, -0X1646);
    // 0x800911F4: lbu         $v0, 0x0($t2)
    ctx->r2 = MEM_BU(ctx->r10, 0X0);
    // 0x800911F8: slti        $at, $v0, 0x8
    ctx->r1 = SIGNED(ctx->r2) < 0X8 ? 1 : 0;
    // 0x800911FC: beq         $at, $zero, L_80091260
    if (ctx->r1 == 0) {
        // 0x80091200: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80091260;
    }
    // 0x80091200: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80091204: beql        $v0, $zero, L_8009124C
    if (ctx->r2 == 0) {
        // 0x80091208: sll         $t4, $v0, 2
        ctx->r12 = S32(ctx->r2 << 2);
            goto L_8009124C;
    }
    goto skip_0;
    // 0x80091208: sll         $t4, $v0, 2
    ctx->r12 = S32(ctx->r2 << 2);
    skip_0:
    // 0x8009120C: blez        $a1, L_80091248
    if (SIGNED(ctx->r5) <= 0) {
        // 0x80091210: or          $a2, $zero, $zero
        ctx->r6 = 0 | 0;
            goto L_80091248;
    }
    // 0x80091210: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x80091214: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80091218: addiu       $t0, $t0, -0x1640
    ctx->r8 = ADD32(ctx->r8, -0X1640);
    // 0x8009121C: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
L_80091220:
    // 0x80091220: lw          $t9, 0x0($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X0);
    // 0x80091224: addiu       $a2, $a2, 0x1
    ctx->r6 = ADD32(ctx->r6, 0X1);
    // 0x80091228: slt         $at, $a2, $a1
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x8009122C: andi        $t3, $t9, 0xFFFF
    ctx->r11 = ctx->r25 & 0XFFFF;
    // 0x80091230: bne         $a3, $t3, L_80091240
    if (ctx->r7 != ctx->r11) {
        // 0x80091234: nop
    
            goto L_80091240;
    }
    // 0x80091234: nop

    // 0x80091238: jr          $ra
    // 0x8009123C: sb          $zero, 0x0($t1)
    MEM_B(0X0, ctx->r9) = 0;
    return;
    // 0x8009123C: sb          $zero, 0x0($t1)
    MEM_B(0X0, ctx->r9) = 0;
L_80091240:
    // 0x80091240: bne         $at, $zero, L_80091220
    if (ctx->r1 != 0) {
        // 0x80091244: addiu       $t0, $t0, 0x4
        ctx->r8 = ADD32(ctx->r8, 0X4);
            goto L_80091220;
    }
    // 0x80091244: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
L_80091248:
    // 0x80091248: sll         $t4, $v0, 2
    ctx->r12 = S32(ctx->r2 << 2);
L_8009124C:
    // 0x8009124C: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80091250: addu        $at, $at, $t4
    ctx->r1 = ADD32(ctx->r1, ctx->r12);
    // 0x80091254: sw          $v1, -0x1640($at)
    MEM_W(-0X1640, ctx->r1) = ctx->r3;
    // 0x80091258: addiu       $t5, $v0, 0x1
    ctx->r13 = ADD32(ctx->r2, 0X1);
    // 0x8009125C: sb          $t5, 0x0($t2)
    MEM_B(0X0, ctx->r10) = ctx->r13;
L_80091260:
    // 0x80091260: sb          $zero, 0x0($t1)
    MEM_B(0X0, ctx->r9) = 0;
L_80091264:
    // 0x80091264: jr          $ra
    // 0x80091268: nop

    return;
    // 0x80091268: nop

;}
RECOMP_FUNC void func_8009126C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8009126C: lui         $a3, 0x8014
    ctx->r7 = S32(0X8014 << 16);
    // 0x80091270: addiu       $a3, $a3, -0x1648
    ctx->r7 = ADD32(ctx->r7, -0X1648);
    // 0x80091274: lbu         $v0, 0x0($a3)
    ctx->r2 = MEM_BU(ctx->r7, 0X0);
    // 0x80091278: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x8009127C: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x80091280: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80091284: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80091288: slti        $at, $v0, 0x8
    ctx->r1 = SIGNED(ctx->r2) < 0X8 ? 1 : 0;
    // 0x8009128C: andi        $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 & 0XFF;
    // 0x80091290: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x80091294: beq         $at, $zero, L_800912DC
    if (ctx->r1 == 0) {
        // 0x80091298: sra         $a2, $a2, 16
        ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
            goto L_800912DC;
    }
    // 0x80091298: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x8009129C: beq         $a0, $zero, L_800912DC
    if (ctx->r4 == 0) {
        // 0x800912A0: slti        $at, $a1, 0x3
        ctx->r1 = SIGNED(ctx->r5) < 0X3 ? 1 : 0;
            goto L_800912DC;
    }
    // 0x800912A0: slti        $at, $a1, 0x3
    ctx->r1 = SIGNED(ctx->r5) < 0X3 ? 1 : 0;
    // 0x800912A4: beq         $at, $zero, L_800912DC
    if (ctx->r1 == 0) {
        // 0x800912A8: sll         $t6, $v0, 2
        ctx->r14 = S32(ctx->r2 << 2);
            goto L_800912DC;
    }
    // 0x800912A8: sll         $t6, $v0, 2
    ctx->r14 = S32(ctx->r2 << 2);
    // 0x800912AC: subu        $t6, $t6, $v0
    ctx->r14 = SUB32(ctx->r14, ctx->r2);
    // 0x800912B0: lui         $t7, 0x8014
    ctx->r15 = S32(0X8014 << 16);
    // 0x800912B4: addiu       $t7, $t7, -0x1678
    ctx->r15 = ADD32(ctx->r15, -0X1678);
    // 0x800912B8: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x800912BC: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x800912C0: addiu       $t8, $v0, 0x1
    ctx->r24 = ADD32(ctx->r2, 0X1);
    // 0x800912C4: sh          $a0, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r4;
    // 0x800912C8: sh          $a2, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r6;
    // 0x800912CC: sb          $a1, 0x4($v1)
    MEM_B(0X4, ctx->r3) = ctx->r5;
    // 0x800912D0: sb          $t8, 0x0($a3)
    MEM_B(0X0, ctx->r7) = ctx->r24;
    // 0x800912D4: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x800912D8: sb          $zero, -0x16C7($at)
    MEM_B(-0X16C7, ctx->r1) = 0;
L_800912DC:
    // 0x800912DC: jr          $ra
    // 0x800912E0: nop

    return;
    // 0x800912E0: nop

;}
RECOMP_FUNC void func_800912E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800912E4: lui         $t6, 0x8014
    ctx->r14 = S32(0X8014 << 16);
    // 0x800912E8: lw          $t6, -0x1394($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X1394);
    // 0x800912EC: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x800912F0: andi        $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 & 0XFF;
    // 0x800912F4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x800912F8: addu        $v0, $t6, $a1
    ctx->r2 = ADD32(ctx->r14, ctx->r5);
    // 0x800912FC: lbu         $v1, 0xB8($v0)
    ctx->r3 = MEM_BU(ctx->r2, 0XB8);
    // 0x80091300: andi        $a2, $a0, 0xFF
    ctx->r6 = ctx->r4 & 0XFF;
    // 0x80091304: sll         $t1, $a2, 2
    ctx->r9 = S32(ctx->r6 << 2);
    // 0x80091308: beq         $v1, $zero, L_80091364
    if (ctx->r3 == 0) {
        // 0x8009130C: subu        $t1, $t1, $a2
        ctx->r9 = SUB32(ctx->r9, ctx->r6);
            goto L_80091364;
    }
    // 0x8009130C: subu        $t1, $t1, $a2
    ctx->r9 = SUB32(ctx->r9, ctx->r6);
    // 0x80091310: bne         $a1, $zero, L_80091324
    if (ctx->r5 != 0) {
        // 0x80091314: sll         $t7, $a2, 2
        ctx->r15 = S32(ctx->r6 << 2);
            goto L_80091324;
    }
    // 0x80091314: sll         $t7, $a2, 2
    ctx->r15 = S32(ctx->r6 << 2);
    // 0x80091318: lb          $a0, 0xBB($v0)
    ctx->r4 = MEM_B(ctx->r2, 0XBB);
    // 0x8009131C: b           L_80091328
    // 0x80091320: sll         $a0, $a0, 8
    ctx->r4 = S32(ctx->r4 << 8);
        goto L_80091328;
    // 0x80091320: sll         $a0, $a0, 8
    ctx->r4 = S32(ctx->r4 << 8);
L_80091324:
    // 0x80091324: lb          $a0, 0xBB($v0)
    ctx->r4 = MEM_B(ctx->r2, 0XBB);
L_80091328:
    // 0x80091328: subu        $t7, $t7, $a2
    ctx->r15 = SUB32(ctx->r15, ctx->r6);
    // 0x8009132C: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x80091330: lui         $t8, 0x8014
    ctx->r24 = S32(0X8014 << 16);
    // 0x80091334: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80091338: lh          $t8, -0x1676($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X1676);
    // 0x8009133C: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x80091340: multu       $t8, $v1
    result = U64(U32(ctx->r24)) * U64(U32(ctx->r3)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091344: mflo        $t9
    ctx->r25 = lo;
    // 0x80091348: nop

    // 0x8009134C: nop

    // 0x80091350: div         $zero, $t9, $at
    lo = S32(S64(S32(ctx->r25)) / S64(S32(ctx->r1))); hi = S32(S64(S32(ctx->r25)) % S64(S32(ctx->r1)));
    // 0x80091354: mflo        $t0
    ctx->r8 = lo;
    // 0x80091358: addu        $v0, $a0, $t0
    ctx->r2 = ADD32(ctx->r4, ctx->r8);
    // 0x8009135C: jr          $ra
    // 0x80091360: nop

    return;
    // 0x80091360: nop

L_80091364:
    // 0x80091364: sll         $t1, $t1, 1
    ctx->r9 = S32(ctx->r9 << 1);
    // 0x80091368: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x8009136C: addu        $v0, $v0, $t1
    ctx->r2 = ADD32(ctx->r2, ctx->r9);
    // 0x80091370: lh          $v0, -0x1676($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X1676);
    // 0x80091374: jr          $ra
    // 0x80091378: nop

    return;
    // 0x80091378: nop

;}
RECOMP_FUNC void func_8009137C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8009137C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80091380: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x80091384: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x80091388: lui         $t7, 0x8014
    ctx->r15 = S32(0X8014 << 16);
    // 0x8009138C: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x80091390: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x80091394: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80091398: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8009139C: addiu       $t7, $t7, -0x1678
    ctx->r15 = ADD32(ctx->r15, -0X1678);
    // 0x800913A0: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x800913A4: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x800913A8: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x800913AC: addu        $s6, $t6, $t7
    ctx->r22 = ADD32(ctx->r14, ctx->r15);
    // 0x800913B0: lui         $s1, 0x8029
    ctx->r17 = S32(0X8029 << 16);
    // 0x800913B4: lui         $s2, 0x8014
    ctx->r18 = S32(0X8014 << 16);
    // 0x800913B8: lui         $s5, 0x8029
    ctx->r21 = S32(0X8029 << 16);
    // 0x800913BC: or          $s7, $a0, $zero
    ctx->r23 = ctx->r4 | 0;
    // 0x800913C0: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x800913C4: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x800913C8: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x800913CC: lbu         $s3, 0x4($s6)
    ctx->r19 = MEM_BU(ctx->r22, 0X4);
    // 0x800913D0: addiu       $s5, $s5, 0x11C0
    ctx->r21 = ADD32(ctx->r21, 0X11C0);
    // 0x800913D4: addiu       $s2, $s2, -0x1394
    ctx->r18 = ADD32(ctx->r18, -0X1394);
    // 0x800913D8: addiu       $s1, $s1, 0xE0
    ctx->r17 = ADD32(ctx->r17, 0XE0);
    // 0x800913DC: sw          $s1, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r17;
L_800913E0:
    // 0x800913E0: lw          $t9, 0x0($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X0);
    // 0x800913E4: lhu         $t1, 0x0($s6)
    ctx->r9 = MEM_HU(ctx->r22, 0X0);
    // 0x800913E8: andi        $a0, $s7, 0xFF
    ctx->r4 = ctx->r23 & 0XFF;
    // 0x800913EC: andi        $t0, $t9, 0x7FFF
    ctx->r8 = ctx->r25 & 0X7FFF;
    // 0x800913F0: bne         $t0, $t1, L_80091460
    if (ctx->r8 != ctx->r9) {
        // 0x800913F4: andi        $a1, $s3, 0xFF
        ctx->r5 = ctx->r19 & 0XFF;
            goto L_80091460;
    }
    // 0x800913F4: andi        $a1, $s3, 0xFF
    ctx->r5 = ctx->r19 & 0XFF;
    // 0x800913F8: lui         $t3, 0x800C
    ctx->r11 = S32(0X800C << 16);
    // 0x800913FC: addiu       $t3, $t3, -0x7388
    ctx->r11 = ADD32(ctx->r11, -0X7388);
    // 0x80091400: sll         $t2, $s3, 2
    ctx->r10 = S32(ctx->r19 << 2);
    // 0x80091404: addu        $s0, $t2, $t3
    ctx->r16 = ADD32(ctx->r10, ctx->r11);
    // 0x80091408: jal         0x800912E4
    // 0x8009140C: sll         $s4, $s3, 1
    ctx->r20 = S32(ctx->r19 << 1);
    func_800912E4(rdram, ctx);
        goto after_0;
    // 0x8009140C: sll         $s4, $s3, 1
    ctx->r20 = S32(ctx->r19 << 1);
    after_0:
    // 0x80091410: lh          $a0, 0x0($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X0);
    // 0x80091414: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80091418: slt         $at, $v0, $a0
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x8009141C: beql        $at, $zero, L_80091430
    if (ctx->r1 == 0) {
        // 0x80091420: lh          $a0, 0x2($s0)
        ctx->r4 = MEM_H(ctx->r16, 0X2);
            goto L_80091430;
    }
    goto skip_0;
    // 0x80091420: lh          $a0, 0x2($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X2);
    skip_0:
    // 0x80091424: b           L_80091440
    // 0x80091428: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
        goto L_80091440;
    // 0x80091428: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x8009142C: lh          $a0, 0x2($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X2);
L_80091430:
    // 0x80091430: slt         $at, $a0, $v0
    ctx->r1 = SIGNED(ctx->r4) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80091434: beql        $at, $zero, L_80091444
    if (ctx->r1 == 0) {
        // 0x80091438: lw          $t4, 0x0($s2)
        ctx->r12 = MEM_W(ctx->r18, 0X0);
            goto L_80091444;
    }
    goto skip_1;
    // 0x80091438: lw          $t4, 0x0($s2)
    ctx->r12 = MEM_W(ctx->r18, 0X0);
    skip_1:
    // 0x8009143C: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
L_80091440:
    // 0x80091440: lw          $t4, 0x0($s2)
    ctx->r12 = MEM_W(ctx->r18, 0X0);
L_80091444:
    // 0x80091444: sll         $t5, $v1, 16
    ctx->r13 = S32(ctx->r3 << 16);
    // 0x80091448: sra         $t6, $t5, 16
    ctx->r14 = S32(SIGNED(ctx->r13) >> 16);
    // 0x8009144C: addu        $v0, $t4, $s4
    ctx->r2 = ADD32(ctx->r12, ctx->r20);
    // 0x80091450: lh          $t7, 0xA2($v0)
    ctx->r15 = MEM_H(ctx->r2, 0XA2);
    // 0x80091454: beql        $t6, $t7, L_80091464
    if (ctx->r14 == ctx->r15) {
        // 0x80091458: addiu       $s1, $s1, 0xD8
        ctx->r17 = ADD32(ctx->r17, 0XD8);
            goto L_80091464;
    }
    goto skip_2;
    // 0x80091458: addiu       $s1, $s1, 0xD8
    ctx->r17 = ADD32(ctx->r17, 0XD8);
    skip_2:
    // 0x8009145C: sh          $v1, 0xA2($v0)
    MEM_H(0XA2, ctx->r2) = ctx->r3;
L_80091460:
    // 0x80091460: addiu       $s1, $s1, 0xD8
    ctx->r17 = ADD32(ctx->r17, 0XD8);
L_80091464:
    // 0x80091464: bnel        $s1, $s5, L_800913E0
    if (ctx->r17 != ctx->r21) {
        // 0x80091468: sw          $s1, 0x0($s2)
        MEM_W(0X0, ctx->r18) = ctx->r17;
            goto L_800913E0;
    }
    goto skip_3;
    // 0x80091468: sw          $s1, 0x0($s2)
    MEM_W(0X0, ctx->r18) = ctx->r17;
    skip_3:
    // 0x8009146C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80091470: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80091474: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80091478: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8009147C: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80091480: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x80091484: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x80091488: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x8009148C: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x80091490: jr          $ra
    // 0x80091494: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x80091494: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_80091498(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091498: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8009149C: andi        $a1, $a1, 0xFF
    ctx->r5 = ctx->r5 & 0XFF;
    // 0x800914A0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x800914A4: slti        $at, $a1, 0x6
    ctx->r1 = SIGNED(ctx->r5) < 0X6 ? 1 : 0;
    // 0x800914A8: bne         $at, $zero, L_800914B4
    if (ctx->r1 != 0) {
        // 0x800914AC: andi        $a0, $a0, 0xFF
        ctx->r4 = ctx->r4 & 0XFF;
            goto L_800914B4;
    }
    // 0x800914AC: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x800914B0: addiu       $a1, $zero, 0x5
    ctx->r5 = ADD32(0, 0X5);
L_800914B4:
    // 0x800914B4: slti        $at, $a0, 0x2
    ctx->r1 = SIGNED(ctx->r4) < 0X2 ? 1 : 0;
    // 0x800914B8: beq         $at, $zero, L_800914D4
    if (ctx->r1 == 0) {
        // 0x800914BC: lui         $v0, 0x8014
        ctx->r2 = S32(0X8014 << 16);
            goto L_800914D4;
    }
    // 0x800914BC: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x800914C0: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x800914C4: addiu       $v0, $v0, -0x1698
    ctx->r2 = ADD32(ctx->r2, -0X1698);
    // 0x800914C8: addu        $t6, $v0, $a0
    ctx->r14 = ADD32(ctx->r2, ctx->r4);
    // 0x800914CC: jr          $ra
    // 0x800914D0: sb          $a1, 0x0($t6)
    MEM_B(0X0, ctx->r14) = ctx->r5;
    return;
    // 0x800914D0: sb          $a1, 0x0($t6)
    MEM_B(0X0, ctx->r14) = ctx->r5;
L_800914D4:
    // 0x800914D4: addiu       $v0, $v0, -0x1698
    ctx->r2 = ADD32(ctx->r2, -0X1698);
    // 0x800914D8: sb          $a1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r5;
    // 0x800914DC: sb          $a1, 0x1($v0)
    MEM_B(0X1, ctx->r2) = ctx->r5;
    // 0x800914E0: jr          $ra
    // 0x800914E4: nop

    return;
    // 0x800914E4: nop

;}
RECOMP_FUNC void func_800914E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800914E8: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x800914EC: addiu       $v0, $v0, -0x1698
    ctx->r2 = ADD32(ctx->r2, -0X1698);
    // 0x800914F0: sb          $zero, 0x1($v0)
    MEM_B(0X1, ctx->r2) = 0;
    // 0x800914F4: jr          $ra
    // 0x800914F8: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    return;
    // 0x800914F8: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
;}
RECOMP_FUNC void func_800914FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800914FC: lui         $t6, 0x8014
    ctx->r14 = S32(0X8014 << 16);
    // 0x80091500: lui         $t7, 0x8014
    ctx->r15 = S32(0X8014 << 16);
    // 0x80091504: lhu         $t7, -0x16B6($t7)
    ctx->r15 = MEM_HU(ctx->r15, -0X16B6);
    // 0x80091508: lhu         $t6, -0x16BA($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X16BA);
    // 0x8009150C: lui         $v0, 0x8029
    ctx->r2 = S32(0X8029 << 16);
    // 0x80091510: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x80091514: bne         $v1, $zero, L_80091534
    if (ctx->r3 != 0) {
        // 0x80091518: nop
    
            goto L_80091534;
    }
    // 0x80091518: nop

    // 0x8009151C: lw          $v0, 0xE0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0XE0);
    // 0x80091520: beq         $v0, $zero, L_80091534
    if (ctx->r2 == 0) {
        // 0x80091524: sltiu       $at, $v0, 0x100
        ctx->r1 = ctx->r2 < 0X100 ? 1 : 0;
            goto L_80091534;
    }
    // 0x80091524: sltiu       $at, $v0, 0x100
    ctx->r1 = ctx->r2 < 0X100 ? 1 : 0;
    // 0x80091528: beq         $at, $zero, L_80091534
    if (ctx->r1 == 0) {
        // 0x8009152C: nop
    
            goto L_80091534;
    }
    // 0x8009152C: nop

    // 0x80091530: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
L_80091534:
    // 0x80091534: jr          $ra
    // 0x80091538: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80091538: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_8009153C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8009153C: lui         $a2, 0x8029
    ctx->r6 = S32(0X8029 << 16);
    // 0x80091540: lui         $v0, 0x8029
    ctx->r2 = S32(0X8029 << 16);
    // 0x80091544: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80091548: andi        $v1, $a0, 0xFFFF
    ctx->r3 = ctx->r4 & 0XFFFF;
    // 0x8009154C: addiu       $v0, $v0, 0x11C0
    ctx->r2 = ADD32(ctx->r2, 0X11C0);
    // 0x80091550: addiu       $a2, $a2, 0xE0
    ctx->r6 = ADD32(ctx->r6, 0XE0);
    // 0x80091554: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
L_80091558:
    // 0x80091558: lw          $t6, 0x0($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X0);
    // 0x8009155C: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x80091560: bne         $v1, $t7, L_80091588
    if (ctx->r3 != ctx->r15) {
        // 0x80091564: lui         $t8, 0x8014
        ctx->r24 = S32(0X8014 << 16);
            goto L_80091588;
    }
    // 0x80091564: lui         $t8, 0x8014
    ctx->r24 = S32(0X8014 << 16);
    // 0x80091568: lw          $t8, -0x1394($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X1394);
    // 0x8009156C: addu        $t9, $t8, $a1
    ctx->r25 = ADD32(ctx->r24, ctx->r5);
    // 0x80091570: lh          $v0, 0xB4($t9)
    ctx->r2 = MEM_H(ctx->r25, 0XB4);
    // 0x80091574: bne         $v0, $zero, L_80091580
    if (ctx->r2 != 0) {
        // 0x80091578: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80091580;
    }
    // 0x80091578: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x8009157C: addiu       $v1, $v0, 0x1
    ctx->r3 = ADD32(ctx->r2, 0X1);
L_80091580:
    // 0x80091580: jr          $ra
    // 0x80091584: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80091584: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80091588:
    // 0x80091588: lw          $t0, 0xD8($a2)
    ctx->r8 = MEM_W(ctx->r6, 0XD8);
    // 0x8009158C: andi        $t1, $t0, 0x7FFF
    ctx->r9 = ctx->r8 & 0X7FFF;
    // 0x80091590: bne         $v1, $t1, L_800915B8
    if (ctx->r3 != ctx->r9) {
        // 0x80091594: lui         $t2, 0x8014
        ctx->r10 = S32(0X8014 << 16);
            goto L_800915B8;
    }
    // 0x80091594: lui         $t2, 0x8014
    ctx->r10 = S32(0X8014 << 16);
    // 0x80091598: lw          $t2, -0x1394($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X1394);
    // 0x8009159C: addu        $t3, $t2, $a1
    ctx->r11 = ADD32(ctx->r10, ctx->r5);
    // 0x800915A0: lh          $v1, 0x18C($t3)
    ctx->r3 = MEM_H(ctx->r11, 0X18C);
    // 0x800915A4: bne         $v1, $zero, L_800915B0
    if (ctx->r3 != 0) {
        // 0x800915A8: nop
    
            goto L_800915B0;
    }
    // 0x800915A8: nop

    // 0x800915AC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
L_800915B0:
    // 0x800915B0: jr          $ra
    // 0x800915B4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x800915B4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_800915B8:
    // 0x800915B8: lw          $t4, 0x1B0($a2)
    ctx->r12 = MEM_W(ctx->r6, 0X1B0);
    // 0x800915BC: andi        $t5, $t4, 0x7FFF
    ctx->r13 = ctx->r12 & 0X7FFF;
    // 0x800915C0: bne         $v1, $t5, L_800915E8
    if (ctx->r3 != ctx->r13) {
        // 0x800915C4: lui         $t6, 0x8014
        ctx->r14 = S32(0X8014 << 16);
            goto L_800915E8;
    }
    // 0x800915C4: lui         $t6, 0x8014
    ctx->r14 = S32(0X8014 << 16);
    // 0x800915C8: lw          $t6, -0x1394($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X1394);
    // 0x800915CC: addu        $t7, $t6, $a1
    ctx->r15 = ADD32(ctx->r14, ctx->r5);
    // 0x800915D0: lh          $v1, 0x264($t7)
    ctx->r3 = MEM_H(ctx->r15, 0X264);
    // 0x800915D4: bne         $v1, $zero, L_800915E0
    if (ctx->r3 != 0) {
        // 0x800915D8: nop
    
            goto L_800915E0;
    }
    // 0x800915D8: nop

    // 0x800915DC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
L_800915E0:
    // 0x800915E0: jr          $ra
    // 0x800915E4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x800915E4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_800915E8:
    // 0x800915E8: lw          $t8, 0x288($a2)
    ctx->r24 = MEM_W(ctx->r6, 0X288);
    // 0x800915EC: addiu       $a2, $a2, 0x360
    ctx->r6 = ADD32(ctx->r6, 0X360);
    // 0x800915F0: andi        $t9, $t8, 0x7FFF
    ctx->r25 = ctx->r24 & 0X7FFF;
    // 0x800915F4: bne         $v1, $t9, L_8009161C
    if (ctx->r3 != ctx->r25) {
        // 0x800915F8: lui         $t0, 0x8014
        ctx->r8 = S32(0X8014 << 16);
            goto L_8009161C;
    }
    // 0x800915F8: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x800915FC: lw          $t0, -0x1394($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X1394);
    // 0x80091600: addu        $t1, $t0, $a1
    ctx->r9 = ADD32(ctx->r8, ctx->r5);
    // 0x80091604: lh          $v1, 0x33C($t1)
    ctx->r3 = MEM_H(ctx->r9, 0X33C);
    // 0x80091608: bne         $v1, $zero, L_80091614
    if (ctx->r3 != 0) {
        // 0x8009160C: nop
    
            goto L_80091614;
    }
    // 0x8009160C: nop

    // 0x80091610: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
L_80091614:
    // 0x80091614: jr          $ra
    // 0x80091618: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80091618: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8009161C:
    // 0x8009161C: bne         $a2, $v0, L_80091558
    if (ctx->r6 != ctx->r2) {
        // 0x80091620: addiu       $a1, $a1, 0x360
        ctx->r5 = ADD32(ctx->r5, 0X360);
            goto L_80091558;
    }
    // 0x80091620: addiu       $a1, $a1, 0x360
    ctx->r5 = ADD32(ctx->r5, 0X360);
    // 0x80091624: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80091628: jr          $ra
    // 0x8009162C: nop

    return;
    // 0x8009162C: nop

;}
RECOMP_FUNC void func_80091630(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091630: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80091634: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80091638: lui         $s2, 0x8014
    ctx->r18 = S32(0X8014 << 16);
    // 0x8009163C: addiu       $s2, $s2, -0x1646
    ctx->r18 = ADD32(ctx->r18, -0X1646);
    // 0x80091640: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
    // 0x80091644: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80091648: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8009164C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80091650: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80091654: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80091658: beq         $a0, $zero, L_80091708
    if (ctx->r4 == 0) {
        // 0x8009165C: or          $s1, $zero, $zero
        ctx->r17 = 0 | 0;
            goto L_80091708;
    }
    // 0x8009165C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80091660: lui         $s0, 0x8014
    ctx->r16 = S32(0X8014 << 16);
    // 0x80091664: lui         $s3, 0x800C
    ctx->r19 = S32(0X800C << 16);
    // 0x80091668: lui         $s1, 0x8014
    ctx->r17 = S32(0X8014 << 16);
    // 0x8009166C: addiu       $s1, $s1, -0x1620
    ctx->r17 = ADD32(ctx->r17, -0X1620);
    // 0x80091670: addiu       $s3, $s3, -0x737C
    ctx->r19 = ADD32(ctx->r19, -0X737C);
    // 0x80091674: addiu       $s0, $s0, -0x1640
    ctx->r16 = ADD32(ctx->r16, -0X1640);
    // 0x80091678: ori         $s4, $zero, 0x8000
    ctx->r20 = 0 | 0X8000;
    // 0x8009167C: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
L_80091680:
    // 0x80091680: addiu       $t7, $a0, -0x1
    ctx->r15 = ADD32(ctx->r4, -0X1);
    // 0x80091684: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80091688: sw          $v1, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r3;
    // 0x8009168C: andi        $v1, $v1, 0xFFFF
    ctx->r3 = ctx->r3 & 0XFFFF;
    // 0x80091690: slti        $at, $v1, 0x100
    ctx->r1 = SIGNED(ctx->r3) < 0X100 ? 1 : 0;
    // 0x80091694: sb          $t7, 0x0($s2)
    MEM_B(0X0, ctx->r18) = ctx->r15;
    // 0x80091698: beq         $at, $zero, L_800916D4
    if (ctx->r1 == 0) {
        // 0x8009169C: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_800916D4;
    }
    // 0x8009169C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x800916A0: slti        $at, $v1, 0x10
    ctx->r1 = SIGNED(ctx->r3) < 0X10 ? 1 : 0;
    // 0x800916A4: beq         $at, $zero, L_800916C4
    if (ctx->r1 == 0) {
        // 0x800916A8: sll         $t8, $v1, 2
        ctx->r24 = S32(ctx->r3 << 2);
            goto L_800916C4;
    }
    // 0x800916A8: sll         $t8, $v1, 2
    ctx->r24 = S32(ctx->r3 << 2);
    // 0x800916AC: addu        $t9, $s3, $t8
    ctx->r25 = ADD32(ctx->r19, ctx->r24);
    // 0x800916B0: lw          $t9, -0x4($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X4);
    // 0x800916B4: jalr        $t9
    // 0x800916B8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x800916B8: nop

    after_0:
    // 0x800916BC: b           L_800916FC
    // 0x800916C0: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
        goto L_800916FC;
    // 0x800916C0: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
L_800916C4:
    // 0x800916C4: jal         0x80092578
    // 0x800916C8: nop

    func_80092578(rdram, ctx);
        goto after_1;
    // 0x800916C8: nop

    after_1:
    // 0x800916CC: b           L_800916FC
    // 0x800916D0: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
        goto L_800916FC;
    // 0x800916D0: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
L_800916D4:
    // 0x800916D4: slt         $at, $v0, $s4
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r20) ? 1 : 0;
    // 0x800916D8: beq         $at, $zero, L_800916F0
    if (ctx->r1 == 0) {
        // 0x800916DC: nop
    
            goto L_800916F0;
    }
    // 0x800916DC: nop

    // 0x800916E0: jal         0x8009201C
    // 0x800916E4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8009201C(rdram, ctx);
        goto after_2;
    // 0x800916E4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_2:
    // 0x800916E8: b           L_800916FC
    // 0x800916EC: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
        goto L_800916FC;
    // 0x800916EC: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
L_800916F0:
    // 0x800916F0: jal         0x800932F0
    // 0x800916F4: nop

    func_800932F0(rdram, ctx);
        goto after_3;
    // 0x800916F4: nop

    after_3:
    // 0x800916F8: lbu         $a0, 0x0($s2)
    ctx->r4 = MEM_BU(ctx->r18, 0X0);
L_800916FC:
    // 0x800916FC: bnel        $a0, $zero, L_80091680
    if (ctx->r4 != 0) {
        // 0x80091700: lw          $v1, 0x0($s0)
        ctx->r3 = MEM_W(ctx->r16, 0X0);
            goto L_80091680;
    }
    goto skip_0;
    // 0x80091700: lw          $v1, 0x0($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X0);
    skip_0:
    // 0x80091704: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
L_80091708:
    // 0x80091708: lui         $s0, 0x8014
    ctx->r16 = S32(0X8014 << 16);
    // 0x8009170C: addiu       $s0, $s0, -0x1648
    ctx->r16 = ADD32(ctx->r16, -0X1648);
    // 0x80091710: lbu         $t0, 0x0($s0)
    ctx->r8 = MEM_BU(ctx->r16, 0X0);
    // 0x80091714: beql        $t0, $zero, L_80091740
    if (ctx->r8 == 0) {
        // 0x80091718: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80091740;
    }
    goto skip_1;
    // 0x80091718: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_1:
L_8009171C:
    // 0x8009171C: jal         0x8009137C
    // 0x80091720: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_8009137C(rdram, ctx);
        goto after_4;
    // 0x80091720: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_4:
    // 0x80091724: lbu         $t1, 0x0($s0)
    ctx->r9 = MEM_BU(ctx->r16, 0X0);
    // 0x80091728: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x8009172C: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80091730: andi        $t3, $t2, 0xFF
    ctx->r11 = ctx->r10 & 0XFF;
    // 0x80091734: bne         $t3, $zero, L_8009171C
    if (ctx->r11 != 0) {
        // 0x80091738: sb          $t2, 0x0($s0)
        MEM_B(0X0, ctx->r16) = ctx->r10;
            goto L_8009171C;
    }
    // 0x80091738: sb          $t2, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r10;
    // 0x8009173C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80091740:
    // 0x80091740: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80091744: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80091748: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8009174C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80091750: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80091754: jr          $ra
    // 0x80091758: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80091758: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_8009175C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8009175C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80091760: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80091764: lui         $s2, 0x8014
    ctx->r18 = S32(0X8014 << 16);
    // 0x80091768: addiu       $s2, $s2, -0x1390
    ctx->r18 = ADD32(ctx->r18, -0X1390);
    // 0x8009176C: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80091770: lbu         $ra, 0x0($s2)
    ctx->r31 = MEM_BU(ctx->r18, 0X0);
    // 0x80091774: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x80091778: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8009177C: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80091780: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80091784: beq         $ra, $zero, L_80091960
    if (ctx->r31 == 0) {
        // 0x80091788: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_80091960;
    }
    // 0x80091788: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8009178C: lui         $s4, 0x8014
    ctx->r20 = S32(0X8014 << 16);
    // 0x80091790: lui         $s3, 0x8014
    ctx->r19 = S32(0X8014 << 16);
    // 0x80091794: lui         $s1, 0x8014
    ctx->r17 = S32(0X8014 << 16);
    // 0x80091798: addiu       $s1, $s1, -0x1690
    ctx->r17 = ADD32(ctx->r17, -0X1690);
    // 0x8009179C: addiu       $s3, $s3, -0x1620
    ctx->r19 = ADD32(ctx->r19, -0X1620);
    // 0x800917A0: addiu       $s4, $s4, -0x1388
    ctx->r20 = ADD32(ctx->r20, -0X1388);
    // 0x800917A4: addiu       $s5, $zero, 0xC
    ctx->r21 = ADD32(0, 0XC);
    // 0x800917A8: addiu       $t6, $ra, -0x1
    ctx->r14 = ADD32(ctx->r31, -0X1);
L_800917AC:
    // 0x800917AC: andi        $ra, $t6, 0xFF
    ctx->r31 = ctx->r14 & 0XFF;
    // 0x800917B0: multu       $ra, $s5
    result = U64(U32(ctx->r31)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x800917B4: sb          $t6, 0x0($s2)
    MEM_B(0X0, ctx->r18) = ctx->r14;
    // 0x800917B8: mflo        $t7
    ctx->r15 = lo;
    // 0x800917BC: addu        $s0, $s4, $t7
    ctx->r16 = ADD32(ctx->r20, ctx->r15);
    // 0x800917C0: lhu         $t4, 0x0($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0X0);
    // 0x800917C4: lw          $t3, 0x4($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X4);
    // 0x800917C8: sra         $v0, $t4, 3
    ctx->r2 = S32(SIGNED(ctx->r12) >> 3);
    // 0x800917CC: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x800917D0: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x800917D4: sra         $a2, $v0, 3
    ctx->r6 = S32(SIGNED(ctx->r2) >> 3);
    // 0x800917D8: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x800917DC: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x800917E0: sra         $t5, $a2, 3
    ctx->r13 = S32(SIGNED(ctx->r6) >> 3);
    // 0x800917E4: sll         $t5, $t5, 16
    ctx->r13 = S32(ctx->r13 << 16);
    // 0x800917E8: lbu         $t1, 0x2($t3)
    ctx->r9 = MEM_BU(ctx->r11, 0X2);
    // 0x800917EC: sra         $t5, $t5, 16
    ctx->r13 = S32(SIGNED(ctx->r13) >> 16);
    // 0x800917F0: andi        $t0, $v0, 0x3FFF
    ctx->r8 = ctx->r2 & 0X3FFF;
    // 0x800917F4: andi        $t2, $a2, 0xFF
    ctx->r10 = ctx->r6 & 0XFF;
    // 0x800917F8: andi        $t5, $t5, 0x7F
    ctx->r13 = ctx->r13 & 0X7F;
    // 0x800917FC: sll         $v1, $t0, 16
    ctx->r3 = S32(ctx->r8 << 16);
    // 0x80091800: sll         $a0, $t2, 16
    ctx->r4 = S32(ctx->r10 << 16);
    // 0x80091804: sll         $a1, $t5, 16
    ctx->r5 = S32(ctx->r13 << 16);
    // 0x80091808: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8009180C: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80091810: beq         $t1, $zero, L_80091858
    if (ctx->r9 == 0) {
        // 0x80091814: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_80091858;
    }
    // 0x80091814: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80091818: or          $v0, $t1, $zero
    ctx->r2 = ctx->r9 | 0;
    // 0x8009181C: sll         $v0, $v0, 24
    ctx->r2 = S32(ctx->r2 << 24);
    // 0x80091820: sll         $t8, $t0, 16
    ctx->r24 = S32(ctx->r8 << 16);
    // 0x80091824: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x80091828: sra         $t9, $t8, 16
    ctx->r25 = S32(SIGNED(ctx->r24) >> 16);
    // 0x8009182C: slt         $at, $v0, $t9
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r25) ? 1 : 0;
    // 0x80091830: beql        $at, $zero, L_80091860
    if (ctx->r1 == 0) {
        // 0x80091834: lbu         $a2, 0x3($t3)
        ctx->r6 = MEM_BU(ctx->r11, 0X3);
            goto L_80091860;
    }
    goto skip_0;
    // 0x80091834: lbu         $a2, 0x3($t3)
    ctx->r6 = MEM_BU(ctx->r11, 0X3);
    skip_0:
    // 0x80091838: sra         $v1, $v1, 1
    ctx->r3 = S32(SIGNED(ctx->r3) >> 1);
L_8009183C:
    // 0x8009183C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80091840: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80091844: slt         $at, $v0, $v1
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x80091848: bnel        $at, $zero, L_8009183C
    if (ctx->r1 != 0) {
        // 0x8009184C: sra         $v1, $v1, 1
        ctx->r3 = S32(SIGNED(ctx->r3) >> 1);
            goto L_8009183C;
    }
    goto skip_1;
    // 0x8009184C: sra         $v1, $v1, 1
    ctx->r3 = S32(SIGNED(ctx->r3) >> 1);
    skip_1:
    // 0x80091850: b           L_80091860
    // 0x80091854: lbu         $a2, 0x3($t3)
    ctx->r6 = MEM_BU(ctx->r11, 0X3);
        goto L_80091860;
    // 0x80091854: lbu         $a2, 0x3($t3)
    ctx->r6 = MEM_BU(ctx->r11, 0X3);
L_80091858:
    // 0x80091858: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8009185C: lbu         $a2, 0x3($t3)
    ctx->r6 = MEM_BU(ctx->r11, 0X3);
L_80091860:
    // 0x80091860: sll         $t6, $t2, 16
    ctx->r14 = S32(ctx->r10 << 16);
    // 0x80091864: sra         $t7, $t6, 16
    ctx->r15 = S32(SIGNED(ctx->r14) >> 16);
    // 0x80091868: beq         $a2, $zero, L_80091898
    if (ctx->r6 == 0) {
        // 0x8009186C: slt         $at, $a2, $t7
        ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r15) ? 1 : 0;
            goto L_80091898;
    }
    // 0x8009186C: slt         $at, $a2, $t7
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x80091870: beq         $at, $zero, L_8009189C
    if (ctx->r1 == 0) {
        // 0x80091874: or          $v0, $a2, $zero
        ctx->r2 = ctx->r6 | 0;
            goto L_8009189C;
    }
    // 0x80091874: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80091878: sra         $a0, $a0, 1
    ctx->r4 = S32(SIGNED(ctx->r4) >> 1);
L_8009187C:
    // 0x8009187C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80091880: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x80091884: slt         $at, $v0, $a0
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x80091888: bnel        $at, $zero, L_8009187C
    if (ctx->r1 != 0) {
        // 0x8009188C: sra         $a0, $a0, 1
        ctx->r4 = S32(SIGNED(ctx->r4) >> 1);
            goto L_8009187C;
    }
    goto skip_2;
    // 0x8009188C: sra         $a0, $a0, 1
    ctx->r4 = S32(SIGNED(ctx->r4) >> 1);
    skip_2:
    // 0x80091890: b           L_800918A0
    // 0x80091894: lbu         $a2, 0x4($t3)
    ctx->r6 = MEM_BU(ctx->r11, 0X4);
        goto L_800918A0;
    // 0x80091894: lbu         $a2, 0x4($t3)
    ctx->r6 = MEM_BU(ctx->r11, 0X4);
L_80091898:
    // 0x80091898: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
L_8009189C:
    // 0x8009189C: lbu         $a2, 0x4($t3)
    ctx->r6 = MEM_BU(ctx->r11, 0X4);
L_800918A0:
    // 0x800918A0: sll         $t8, $t5, 16
    ctx->r24 = S32(ctx->r13 << 16);
    // 0x800918A4: andi        $t6, $t4, 0x1
    ctx->r14 = ctx->r12 & 0X1;
    // 0x800918A8: beq         $a2, $zero, L_800918E0
    if (ctx->r6 == 0) {
        // 0x800918AC: andi        $t7, $t4, 0x2
        ctx->r15 = ctx->r12 & 0X2;
            goto L_800918E0;
    }
    // 0x800918AC: andi        $t7, $t4, 0x2
    ctx->r15 = ctx->r12 & 0X2;
    // 0x800918B0: sra         $t9, $t8, 16
    ctx->r25 = S32(SIGNED(ctx->r24) >> 16);
    // 0x800918B4: slt         $at, $a2, $t9
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r25) ? 1 : 0;
    // 0x800918B8: beq         $at, $zero, L_800918E4
    if (ctx->r1 == 0) {
        // 0x800918BC: or          $v0, $a2, $zero
        ctx->r2 = ctx->r6 | 0;
            goto L_800918E4;
    }
    // 0x800918BC: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x800918C0: sra         $a1, $a1, 1
    ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
L_800918C4:
    // 0x800918C4: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x800918C8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800918CC: slt         $at, $v0, $a1
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x800918D0: bnel        $at, $zero, L_800918C4
    if (ctx->r1 != 0) {
        // 0x800918D4: sra         $a1, $a1, 1
        ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
            goto L_800918C4;
    }
    goto skip_3;
    // 0x800918D4: sra         $a1, $a1, 1
    ctx->r5 = S32(SIGNED(ctx->r5) >> 1);
    skip_3:
    // 0x800918D8: b           L_800918E4
    // 0x800918DC: nop

        goto L_800918E4;
    // 0x800918DC: nop

L_800918E0:
    // 0x800918E0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
L_800918E4:
    // 0x800918E4: beq         $t6, $zero, L_800918F8
    if (ctx->r14 == 0) {
        // 0x800918E8: andi        $t8, $t4, 0x4
        ctx->r24 = ctx->r12 & 0X4;
            goto L_800918F8;
    }
    // 0x800918E8: andi        $t8, $t4, 0x4
    ctx->r24 = ctx->r12 & 0X4;
    // 0x800918EC: negu        $v1, $v1
    ctx->r3 = SUB32(0, ctx->r3);
    // 0x800918F0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x800918F4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
L_800918F8:
    // 0x800918F8: beq         $t7, $zero, L_8009190C
    if (ctx->r15 == 0) {
        // 0x800918FC: nop
    
            goto L_8009190C;
    }
    // 0x800918FC: nop

    // 0x80091900: negu        $a0, $a0
    ctx->r4 = SUB32(0, ctx->r4);
    // 0x80091904: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80091908: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
L_8009190C:
    // 0x8009190C: beql        $t8, $zero, L_80091924
    if (ctx->r24 == 0) {
        // 0x80091910: lhu         $t9, 0x8($s0)
        ctx->r25 = MEM_HU(ctx->r16, 0X8);
            goto L_80091924;
    }
    goto skip_4;
    // 0x80091910: lhu         $t9, 0x8($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X8);
    skip_4:
    // 0x80091914: negu        $a1, $a1
    ctx->r5 = SUB32(0, ctx->r5);
    // 0x80091918: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x8009191C: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80091920: lhu         $t9, 0x8($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X8);
L_80091924:
    // 0x80091924: lhu         $t7, 0x0($t3)
    ctx->r15 = MEM_HU(ctx->r11, 0X0);
    // 0x80091928: sh          $v1, 0xC($s1)
    MEM_H(0XC, ctx->r17) = ctx->r3;
    // 0x8009192C: sll         $t6, $t9, 16
    ctx->r14 = S32(ctx->r25 << 16);
    // 0x80091930: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x80091934: andi        $t6, $t8, 0x7FFF
    ctx->r14 = ctx->r24 & 0X7FFF;
    // 0x80091938: sltiu       $at, $t6, 0x100
    ctx->r1 = ctx->r14 < 0X100 ? 1 : 0;
    // 0x8009193C: sw          $t8, 0x0($s3)
    MEM_W(0X0, ctx->r19) = ctx->r24;
    // 0x80091940: sh          $a0, 0xE($s1)
    MEM_H(0XE, ctx->r17) = ctx->r4;
    // 0x80091944: bne         $at, $zero, L_80091958
    if (ctx->r1 != 0) {
        // 0x80091948: sh          $a1, 0x10($s1)
        MEM_H(0X10, ctx->r17) = ctx->r5;
            goto L_80091958;
    }
    // 0x80091948: sh          $a1, 0x10($s1)
    MEM_H(0X10, ctx->r17) = ctx->r5;
    // 0x8009194C: jal         0x8009201C
    // 0x80091950: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    func_8009201C(rdram, ctx);
        goto after_0;
    // 0x80091950: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_0:
    // 0x80091954: lbu         $ra, 0x0($s2)
    ctx->r31 = MEM_BU(ctx->r18, 0X0);
L_80091958:
    // 0x80091958: bnel        $ra, $zero, L_800917AC
    if (ctx->r31 != 0) {
        // 0x8009195C: addiu       $t6, $ra, -0x1
        ctx->r14 = ADD32(ctx->r31, -0X1);
            goto L_800917AC;
    }
    goto skip_5;
    // 0x8009195C: addiu       $t6, $ra, -0x1
    ctx->r14 = ADD32(ctx->r31, -0X1);
    skip_5:
L_80091960:
    // 0x80091960: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80091964: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80091968: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8009196C: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80091970: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80091974: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x80091978: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x8009197C: jr          $ra
    // 0x80091980: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80091980: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80091984(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091984: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80091988: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8009198C: lui         $s0, 0x8014
    ctx->r16 = S32(0X8014 << 16);
    // 0x80091990: addiu       $s0, $s0, -0x1358
    ctx->r16 = ADD32(ctx->r16, -0X1358);
    // 0x80091994: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
    // 0x80091998: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8009199C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x800919A0: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x800919A4: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x800919A8: beq         $v0, $zero, L_80091AA0
    if (ctx->r2 == 0) {
        // 0x800919AC: sw          $s1, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r17;
            goto L_80091AA0;
    }
    // 0x800919AC: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x800919B0: lui         $s4, 0x800C
    ctx->r20 = S32(0X800C << 16);
    // 0x800919B4: lui         $s3, 0x8014
    ctx->r19 = S32(0X8014 << 16);
    // 0x800919B8: lui         $s1, 0x8014
    ctx->r17 = S32(0X8014 << 16);
    // 0x800919BC: addiu       $s1, $s1, -0x1350
    ctx->r17 = ADD32(ctx->r17, -0X1350);
    // 0x800919C0: addiu       $s3, $s3, -0x1620
    ctx->r19 = ADD32(ctx->r19, -0X1620);
    // 0x800919C4: addiu       $s4, $s4, -0x737C
    ctx->r20 = ADD32(ctx->r20, -0X737C);
    // 0x800919C8: addiu       $s2, $zero, 0x6
    ctx->r18 = ADD32(0, 0X6);
    // 0x800919CC: addiu       $t6, $v0, -0x1
    ctx->r14 = ADD32(ctx->r2, -0X1);
L_800919D0:
    // 0x800919D0: andi        $v0, $t6, 0xFF
    ctx->r2 = ctx->r14 & 0XFF;
    // 0x800919D4: multu       $v0, $s2
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r18)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x800919D8: sb          $t6, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r14;
    // 0x800919DC: mflo        $t7
    ctx->r15 = lo;
    // 0x800919E0: addu        $v1, $s1, $t7
    ctx->r3 = ADD32(ctx->r17, ctx->r15);
    // 0x800919E4: lbu         $t8, 0x2($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0X2);
    // 0x800919E8: lhu         $a1, 0x0($v1)
    ctx->r5 = MEM_HU(ctx->r3, 0X0);
    // 0x800919EC: sltiu       $at, $t8, 0x5
    ctx->r1 = ctx->r24 < 0X5 ? 1 : 0;
    // 0x800919F0: beq         $at, $zero, L_80091A98
    if (ctx->r1 == 0) {
        // 0x800919F4: sll         $t8, $t8, 2
        ctx->r24 = S32(ctx->r24 << 2);
            goto L_80091A98;
    }
    // 0x800919F4: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x800919F8: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800919FC: addu        $at, $at, $t8
    gpr jr_addend_80091A04 = ctx->r24;
    ctx->r1 = ADD32(ctx->r1, ctx->r24);
    // 0x80091A00: lw          $t8, 0x3D0($at)
    ctx->r24 = ADD32(ctx->r1, 0X3D0);
    // 0x80091A04: jr          $t8
    // 0x80091A08: nop

    switch (jr_addend_80091A04 >> 2) {
        case 0: goto L_80091A0C; break;
        case 1: goto L_80091A5C; break;
        case 2: goto L_80091A6C; break;
        case 3: goto L_80091A7C; break;
        case 4: goto L_80091A8C; break;
        default: switch_error(__func__, 0x80091A04, 0x800C03D0);
    }
    // 0x80091A08: nop

L_80091A0C:
    // 0x80091A0C: sltiu       $at, $a1, 0x100
    ctx->r1 = ctx->r5 < 0X100 ? 1 : 0;
    // 0x80091A10: beq         $at, $zero, L_80091A4C
    if (ctx->r1 == 0) {
        // 0x80091A14: sw          $a1, 0x0($s3)
        MEM_W(0X0, ctx->r19) = ctx->r5;
            goto L_80091A4C;
    }
    // 0x80091A14: sw          $a1, 0x0($s3)
    MEM_W(0X0, ctx->r19) = ctx->r5;
    // 0x80091A18: sltiu       $at, $a1, 0x10
    ctx->r1 = ctx->r5 < 0X10 ? 1 : 0;
    // 0x80091A1C: beq         $at, $zero, L_80091A3C
    if (ctx->r1 == 0) {
        // 0x80091A20: sll         $t9, $a1, 2
        ctx->r25 = S32(ctx->r5 << 2);
            goto L_80091A3C;
    }
    // 0x80091A20: sll         $t9, $a1, 2
    ctx->r25 = S32(ctx->r5 << 2);
    // 0x80091A24: addu        $t0, $s4, $t9
    ctx->r8 = ADD32(ctx->r20, ctx->r25);
    // 0x80091A28: lw          $t9, -0x4($t0)
    ctx->r25 = MEM_W(ctx->r8, -0X4);
    // 0x80091A2C: jalr        $t9
    // 0x80091A30: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80091A30: nop

    after_0:
    // 0x80091A34: b           L_80091A54
    // 0x80091A38: nop

        goto L_80091A54;
    // 0x80091A38: nop

L_80091A3C:
    // 0x80091A3C: jal         0x80092578
    // 0x80091A40: nop

    func_80092578(rdram, ctx);
        goto after_1;
    // 0x80091A40: nop

    after_1:
    // 0x80091A44: b           L_80091A54
    // 0x80091A48: nop

        goto L_80091A54;
    // 0x80091A48: nop

L_80091A4C:
    // 0x80091A4C: jal         0x8009201C
    // 0x80091A50: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8009201C(rdram, ctx);
        goto after_2;
    // 0x80091A50: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_2:
L_80091A54:
    // 0x80091A54: b           L_80091A98
    // 0x80091A58: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
        goto L_80091A98;
    // 0x80091A58: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
L_80091A5C:
    // 0x80091A5C: jal         0x80091AC0
    // 0x80091A60: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80091AC0(rdram, ctx);
        goto after_3;
    // 0x80091A60: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_3:
    // 0x80091A64: b           L_80091A98
    // 0x80091A68: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
        goto L_80091A98;
    // 0x80091A68: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
L_80091A6C:
    // 0x80091A6C: jal         0x80091ED8
    // 0x80091A70: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_80091ED8(rdram, ctx);
        goto after_4;
    // 0x80091A70: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_4:
    // 0x80091A74: b           L_80091A98
    // 0x80091A78: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
        goto L_80091A98;
    // 0x80091A78: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
L_80091A7C:
    // 0x80091A7C: jal         0x80091ED8
    // 0x80091A80: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    func_80091ED8(rdram, ctx);
        goto after_5;
    // 0x80091A80: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_5:
    // 0x80091A84: b           L_80091A98
    // 0x80091A88: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
        goto L_80091A98;
    // 0x80091A88: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
L_80091A8C:
    // 0x80091A8C: jal         0x80091ED8
    // 0x80091A90: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    func_80091ED8(rdram, ctx);
        goto after_6;
    // 0x80091A90: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    after_6:
    // 0x80091A94: lbu         $v0, 0x0($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X0);
L_80091A98:
    // 0x80091A98: bnel        $v0, $zero, L_800919D0
    if (ctx->r2 != 0) {
        // 0x80091A9C: addiu       $t6, $v0, -0x1
        ctx->r14 = ADD32(ctx->r2, -0X1);
            goto L_800919D0;
    }
    goto skip_0;
    // 0x80091A9C: addiu       $t6, $v0, -0x1
    ctx->r14 = ADD32(ctx->r2, -0X1);
    skip_0:
L_80091AA0:
    // 0x80091AA0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80091AA4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80091AA8: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80091AAC: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80091AB0: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80091AB4: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80091AB8: jr          $ra
    // 0x80091ABC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80091ABC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80091AC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091AC0: lui         $t4, 0x8014
    ctx->r12 = S32(0X8014 << 16);
    // 0x80091AC4: addiu       $t4, $t4, -0x1358
    ctx->r12 = ADD32(ctx->r12, -0X1358);
    // 0x80091AC8: lbu         $t6, 0x0($t4)
    ctx->r14 = MEM_BU(ctx->r12, 0X0);
    // 0x80091ACC: addiu       $t5, $zero, 0x6
    ctx->r13 = ADD32(0, 0X6);
    // 0x80091AD0: lui         $t3, 0x8014
    ctx->r11 = S32(0X8014 << 16);
    // 0x80091AD4: multu       $t6, $t5
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091AD8: addiu       $t3, $t3, -0x1350
    ctx->r11 = ADD32(ctx->r11, -0X1350);
    // 0x80091ADC: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80091AE0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80091AE4: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80091AE8: lui         $t6, 0x8029
    ctx->r14 = S32(0X8029 << 16);
    // 0x80091AEC: lui         $v1, 0x8014
    ctx->r3 = S32(0X8014 << 16);
    // 0x80091AF0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80091AF4: addiu       $v1, $v1, -0x1394
    ctx->r3 = ADD32(ctx->r3, -0X1394);
    // 0x80091AF8: addiu       $t6, $t6, 0xE0
    ctx->r14 = ADD32(ctx->r14, 0XE0);
    // 0x80091AFC: mflo        $t7
    ctx->r15 = lo;
    // 0x80091B00: addu        $t8, $t3, $t7
    ctx->r24 = ADD32(ctx->r11, ctx->r15);
    // 0x80091B04: lbu         $t9, 0x3($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0X3);
    // 0x80091B08: addiu       $s0, $zero, 0x2
    ctx->r16 = ADD32(0, 0X2);
    // 0x80091B0C: lui         $t0, 0x8029
    ctx->r8 = S32(0X8029 << 16);
    // 0x80091B10: beq         $t9, $zero, L_80091E58
    if (ctx->r25 == 0) {
        // 0x80091B14: sltiu       $at, $a1, 0x100
        ctx->r1 = ctx->r5 < 0X100 ? 1 : 0;
            goto L_80091E58;
    }
    // 0x80091B14: sltiu       $at, $a1, 0x100
    ctx->r1 = ctx->r5 < 0X100 ? 1 : 0;
    // 0x80091B18: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80091B1C: sw          $t6, -0x1394($at)
    MEM_W(-0X1394, ctx->r1) = ctx->r14;
    // 0x80091B20: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091B24: lui         $t1, 0x8029
    ctx->r9 = S32(0X8029 << 16);
    // 0x80091B28: lui         $t2, 0x8029
    ctx->r10 = S32(0X8029 << 16);
    // 0x80091B2C: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x80091B30: addiu       $t2, $t2, 0x440
    ctx->r10 = ADD32(ctx->r10, 0X440);
    // 0x80091B34: addiu       $t1, $t1, 0x368
    ctx->r9 = ADD32(ctx->r9, 0X368);
    // 0x80091B38: andi        $t8, $t7, 0x7FFF
    ctx->r24 = ctx->r15 & 0X7FFF;
    // 0x80091B3C: bne         $a0, $t8, L_80091BB0
    if (ctx->r4 != ctx->r24) {
        // 0x80091B40: addiu       $t0, $t0, 0x290
        ctx->r8 = ADD32(ctx->r8, 0X290);
            goto L_80091BB0;
    }
    // 0x80091B40: addiu       $t0, $t0, 0x290
    ctx->r8 = ADD32(ctx->r8, 0X290);
    // 0x80091B44: addiu       $a2, $zero, 0xFF
    ctx->r6 = ADD32(0, 0XFF);
    // 0x80091B48: sb          $a2, 0xBE($v0)
    MEM_B(0XBE, ctx->r2) = ctx->r6;
    // 0x80091B4C: lbu         $t9, 0x0($t4)
    ctx->r25 = MEM_BU(ctx->r12, 0X0);
    // 0x80091B50: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80091B54: multu       $t9, $t5
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091B58: addiu       $t9, $zero, 0x100
    ctx->r25 = ADD32(0, 0X100);
    // 0x80091B5C: mflo        $t6
    ctx->r14 = lo;
    // 0x80091B60: addu        $t7, $t3, $t6
    ctx->r15 = ADD32(ctx->r11, ctx->r14);
    // 0x80091B64: lbu         $t8, 0x3($t7)
    ctx->r24 = MEM_BU(ctx->r15, 0X3);
    // 0x80091B68: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x80091B6C: div         $zero, $t9, $t8
    lo = S32(S64(S32(ctx->r25)) / S64(S32(ctx->r24))); hi = S32(S64(S32(ctx->r25)) % S64(S32(ctx->r24)));
    // 0x80091B70: mflo        $t6
    ctx->r14 = lo;
    // 0x80091B74: sb          $t6, 0xBF($t7)
    MEM_B(0XBF, ctx->r15) = ctx->r14;
    // 0x80091B78: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091B7C: bne         $t8, $zero, L_80091B88
    if (ctx->r24 != 0) {
        // 0x80091B80: nop
    
            goto L_80091B88;
    }
    // 0x80091B80: nop

    // 0x80091B84: break       7
    do_break(2148080516);
L_80091B88:
    // 0x80091B88: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091B8C: bne         $t8, $at, L_80091BA0
    if (ctx->r24 != ctx->r1) {
        // 0x80091B90: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091BA0;
    }
    // 0x80091B90: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091B94: bne         $t9, $at, L_80091BA0
    if (ctx->r25 != ctx->r1) {
        // 0x80091B98: nop
    
            goto L_80091BA0;
    }
    // 0x80091B98: nop

    // 0x80091B9C: break       6
    do_break(2148080540);
L_80091BA0:
    // 0x80091BA0: lbu         $t9, 0xBF($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0XBF);
    // 0x80091BA4: bne         $t9, $zero, L_80091BB0
    if (ctx->r25 != 0) {
        // 0x80091BA8: nop
    
            goto L_80091BB0;
    }
    // 0x80091BA8: nop

    // 0x80091BAC: sb          $a3, 0xBF($v0)
    MEM_B(0XBF, ctx->r2) = ctx->r7;
L_80091BB0:
    // 0x80091BB0: lui         $t8, 0x8029
    ctx->r24 = S32(0X8029 << 16);
    // 0x80091BB4: addiu       $t8, $t8, 0x1B8
    ctx->r24 = ADD32(ctx->r24, 0X1B8);
    // 0x80091BB8: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80091BBC: sw          $t8, -0x1394($at)
    MEM_W(-0X1394, ctx->r1) = ctx->r24;
    // 0x80091BC0: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091BC4: lui         $a0, 0x8029
    ctx->r4 = S32(0X8029 << 16);
    // 0x80091BC8: addiu       $a0, $a0, 0x518
    ctx->r4 = ADD32(ctx->r4, 0X518);
    // 0x80091BCC: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80091BD0: addiu       $a2, $zero, 0xFF
    ctx->r6 = ADD32(0, 0XFF);
    // 0x80091BD4: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x80091BD8: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x80091BDC: bne         $a1, $t7, L_80091C48
    if (ctx->r5 != ctx->r15) {
        // 0x80091BE0: lui         $ra, 0x8029
        ctx->r31 = S32(0X8029 << 16);
            goto L_80091C48;
    }
    // 0x80091BE0: lui         $ra, 0x8029
    ctx->r31 = S32(0X8029 << 16);
    // 0x80091BE4: sb          $a2, 0xBE($v0)
    MEM_B(0XBE, ctx->r2) = ctx->r6;
    // 0x80091BE8: lbu         $t9, 0x0($t4)
    ctx->r25 = MEM_BU(ctx->r12, 0X0);
    // 0x80091BEC: multu       $t9, $t5
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091BF0: addiu       $t9, $zero, 0x100
    ctx->r25 = ADD32(0, 0X100);
    // 0x80091BF4: mflo        $t8
    ctx->r24 = lo;
    // 0x80091BF8: addu        $t6, $t3, $t8
    ctx->r14 = ADD32(ctx->r11, ctx->r24);
    // 0x80091BFC: lbu         $t7, 0x3($t6)
    ctx->r15 = MEM_BU(ctx->r14, 0X3);
    // 0x80091C00: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x80091C04: div         $zero, $t9, $t7
    lo = S32(S64(S32(ctx->r25)) / S64(S32(ctx->r15))); hi = S32(S64(S32(ctx->r25)) % S64(S32(ctx->r15)));
    // 0x80091C08: mflo        $t8
    ctx->r24 = lo;
    // 0x80091C0C: sb          $t8, 0xBF($t6)
    MEM_B(0XBF, ctx->r14) = ctx->r24;
    // 0x80091C10: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091C14: bne         $t7, $zero, L_80091C20
    if (ctx->r15 != 0) {
        // 0x80091C18: nop
    
            goto L_80091C20;
    }
    // 0x80091C18: nop

    // 0x80091C1C: break       7
    do_break(2148080668);
L_80091C20:
    // 0x80091C20: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091C24: bne         $t7, $at, L_80091C38
    if (ctx->r15 != ctx->r1) {
        // 0x80091C28: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091C38;
    }
    // 0x80091C28: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091C2C: bne         $t9, $at, L_80091C38
    if (ctx->r25 != ctx->r1) {
        // 0x80091C30: nop
    
            goto L_80091C38;
    }
    // 0x80091C30: nop

    // 0x80091C34: break       6
    do_break(2148080692);
L_80091C38:
    // 0x80091C38: lbu         $t9, 0xBF($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0XBF);
    // 0x80091C3C: bne         $t9, $zero, L_80091C48
    if (ctx->r25 != 0) {
        // 0x80091C40: nop
    
            goto L_80091C48;
    }
    // 0x80091C40: nop

    // 0x80091C44: sb          $a3, 0xBF($v0)
    MEM_B(0XBF, ctx->r2) = ctx->r7;
L_80091C48:
    // 0x80091C48: addiu       $ra, $ra, 0x15F8
    ctx->r31 = ADD32(ctx->r31, 0X15F8);
L_80091C4C:
    // 0x80091C4C: sw          $t0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r8;
    // 0x80091C50: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x80091C54: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80091C58: addiu       $t0, $t0, 0x360
    ctx->r8 = ADD32(ctx->r8, 0X360);
    // 0x80091C5C: andi        $t8, $t7, 0x7FFF
    ctx->r24 = ctx->r15 & 0X7FFF;
    // 0x80091C60: bnel        $a1, $t8, L_80091CD0
    if (ctx->r5 != ctx->r24) {
        // 0x80091C64: sw          $t1, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r9;
            goto L_80091CD0;
    }
    goto skip_0;
    // 0x80091C64: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
    skip_0:
    // 0x80091C68: sb          $a2, 0xBE($v0)
    MEM_B(0XBE, ctx->r2) = ctx->r6;
    // 0x80091C6C: lbu         $t6, 0x0($t4)
    ctx->r14 = MEM_BU(ctx->r12, 0X0);
    // 0x80091C70: multu       $t6, $t5
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091C74: addiu       $t6, $zero, 0x100
    ctx->r14 = ADD32(0, 0X100);
    // 0x80091C78: mflo        $t9
    ctx->r25 = lo;
    // 0x80091C7C: addu        $t7, $t3, $t9
    ctx->r15 = ADD32(ctx->r11, ctx->r25);
    // 0x80091C80: lbu         $t8, 0x3($t7)
    ctx->r24 = MEM_BU(ctx->r15, 0X3);
    // 0x80091C84: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x80091C88: div         $zero, $t6, $t8
    lo = S32(S64(S32(ctx->r14)) / S64(S32(ctx->r24))); hi = S32(S64(S32(ctx->r14)) % S64(S32(ctx->r24)));
    // 0x80091C8C: mflo        $t9
    ctx->r25 = lo;
    // 0x80091C90: sb          $t9, 0xBF($t7)
    MEM_B(0XBF, ctx->r15) = ctx->r25;
    // 0x80091C94: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091C98: bne         $t8, $zero, L_80091CA4
    if (ctx->r24 != 0) {
        // 0x80091C9C: nop
    
            goto L_80091CA4;
    }
    // 0x80091C9C: nop

    // 0x80091CA0: break       7
    do_break(2148080800);
L_80091CA4:
    // 0x80091CA4: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091CA8: bne         $t8, $at, L_80091CBC
    if (ctx->r24 != ctx->r1) {
        // 0x80091CAC: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091CBC;
    }
    // 0x80091CAC: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091CB0: bne         $t6, $at, L_80091CBC
    if (ctx->r14 != ctx->r1) {
        // 0x80091CB4: nop
    
            goto L_80091CBC;
    }
    // 0x80091CB4: nop

    // 0x80091CB8: break       6
    do_break(2148080824);
L_80091CBC:
    // 0x80091CBC: lbu         $t6, 0xBF($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0XBF);
    // 0x80091CC0: bnel        $t6, $zero, L_80091CD0
    if (ctx->r14 != 0) {
        // 0x80091CC4: sw          $t1, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r9;
            goto L_80091CD0;
    }
    goto skip_1;
    // 0x80091CC4: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
    skip_1:
    // 0x80091CC8: sb          $a3, 0xBF($v0)
    MEM_B(0XBF, ctx->r2) = ctx->r7;
    // 0x80091CCC: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
L_80091CD0:
    // 0x80091CD0: lw          $t8, 0x0($t1)
    ctx->r24 = MEM_W(ctx->r9, 0X0);
    // 0x80091CD4: or          $v0, $t1, $zero
    ctx->r2 = ctx->r9 | 0;
    // 0x80091CD8: addiu       $t1, $t1, 0x360
    ctx->r9 = ADD32(ctx->r9, 0X360);
    // 0x80091CDC: andi        $t9, $t8, 0x7FFF
    ctx->r25 = ctx->r24 & 0X7FFF;
    // 0x80091CE0: bnel        $a1, $t9, L_80091D50
    if (ctx->r5 != ctx->r25) {
        // 0x80091CE4: sw          $t2, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r10;
            goto L_80091D50;
    }
    goto skip_2;
    // 0x80091CE4: sw          $t2, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r10;
    skip_2:
    // 0x80091CE8: sb          $a2, 0xBE($v0)
    MEM_B(0XBE, ctx->r2) = ctx->r6;
    // 0x80091CEC: lbu         $t7, 0x0($t4)
    ctx->r15 = MEM_BU(ctx->r12, 0X0);
    // 0x80091CF0: multu       $t7, $t5
    result = U64(U32(ctx->r15)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091CF4: addiu       $t7, $zero, 0x100
    ctx->r15 = ADD32(0, 0X100);
    // 0x80091CF8: mflo        $t6
    ctx->r14 = lo;
    // 0x80091CFC: addu        $t8, $t3, $t6
    ctx->r24 = ADD32(ctx->r11, ctx->r14);
    // 0x80091D00: lbu         $t9, 0x3($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0X3);
    // 0x80091D04: lw          $t8, 0x0($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X0);
    // 0x80091D08: div         $zero, $t7, $t9
    lo = S32(S64(S32(ctx->r15)) / S64(S32(ctx->r25))); hi = S32(S64(S32(ctx->r15)) % S64(S32(ctx->r25)));
    // 0x80091D0C: mflo        $t6
    ctx->r14 = lo;
    // 0x80091D10: sb          $t6, 0xBF($t8)
    MEM_B(0XBF, ctx->r24) = ctx->r14;
    // 0x80091D14: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091D18: bne         $t9, $zero, L_80091D24
    if (ctx->r25 != 0) {
        // 0x80091D1C: nop
    
            goto L_80091D24;
    }
    // 0x80091D1C: nop

    // 0x80091D20: break       7
    do_break(2148080928);
L_80091D24:
    // 0x80091D24: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091D28: bne         $t9, $at, L_80091D3C
    if (ctx->r25 != ctx->r1) {
        // 0x80091D2C: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091D3C;
    }
    // 0x80091D2C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091D30: bne         $t7, $at, L_80091D3C
    if (ctx->r15 != ctx->r1) {
        // 0x80091D34: nop
    
            goto L_80091D3C;
    }
    // 0x80091D34: nop

    // 0x80091D38: break       6
    do_break(2148080952);
L_80091D3C:
    // 0x80091D3C: lbu         $t7, 0xBF($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0XBF);
    // 0x80091D40: bnel        $t7, $zero, L_80091D50
    if (ctx->r15 != 0) {
        // 0x80091D44: sw          $t2, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r10;
            goto L_80091D50;
    }
    goto skip_3;
    // 0x80091D44: sw          $t2, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r10;
    skip_3:
    // 0x80091D48: sb          $a3, 0xBF($v0)
    MEM_B(0XBF, ctx->r2) = ctx->r7;
    // 0x80091D4C: sw          $t2, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r10;
L_80091D50:
    // 0x80091D50: lw          $t9, 0x0($t2)
    ctx->r25 = MEM_W(ctx->r10, 0X0);
    // 0x80091D54: or          $v0, $t2, $zero
    ctx->r2 = ctx->r10 | 0;
    // 0x80091D58: andi        $t6, $t9, 0x7FFF
    ctx->r14 = ctx->r25 & 0X7FFF;
    // 0x80091D5C: bnel        $a1, $t6, L_80091DCC
    if (ctx->r5 != ctx->r14) {
        // 0x80091D60: sw          $a0, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r4;
            goto L_80091DCC;
    }
    goto skip_4;
    // 0x80091D60: sw          $a0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r4;
    skip_4:
    // 0x80091D64: sb          $a2, 0xBE($v0)
    MEM_B(0XBE, ctx->r2) = ctx->r6;
    // 0x80091D68: lbu         $t8, 0x0($t4)
    ctx->r24 = MEM_BU(ctx->r12, 0X0);
    // 0x80091D6C: multu       $t8, $t5
    result = U64(U32(ctx->r24)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091D70: addiu       $t8, $zero, 0x100
    ctx->r24 = ADD32(0, 0X100);
    // 0x80091D74: mflo        $t7
    ctx->r15 = lo;
    // 0x80091D78: addu        $t9, $t3, $t7
    ctx->r25 = ADD32(ctx->r11, ctx->r15);
    // 0x80091D7C: lbu         $t6, 0x3($t9)
    ctx->r14 = MEM_BU(ctx->r25, 0X3);
    // 0x80091D80: lw          $t9, 0x0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X0);
    // 0x80091D84: div         $zero, $t8, $t6
    lo = S32(S64(S32(ctx->r24)) / S64(S32(ctx->r14))); hi = S32(S64(S32(ctx->r24)) % S64(S32(ctx->r14)));
    // 0x80091D88: mflo        $t7
    ctx->r15 = lo;
    // 0x80091D8C: sb          $t7, 0xBF($t9)
    MEM_B(0XBF, ctx->r25) = ctx->r15;
    // 0x80091D90: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091D94: bne         $t6, $zero, L_80091DA0
    if (ctx->r14 != 0) {
        // 0x80091D98: nop
    
            goto L_80091DA0;
    }
    // 0x80091D98: nop

    // 0x80091D9C: break       7
    do_break(2148081052);
L_80091DA0:
    // 0x80091DA0: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091DA4: bne         $t6, $at, L_80091DB8
    if (ctx->r14 != ctx->r1) {
        // 0x80091DA8: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091DB8;
    }
    // 0x80091DA8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091DAC: bne         $t8, $at, L_80091DB8
    if (ctx->r24 != ctx->r1) {
        // 0x80091DB0: nop
    
            goto L_80091DB8;
    }
    // 0x80091DB0: nop

    // 0x80091DB4: break       6
    do_break(2148081076);
L_80091DB8:
    // 0x80091DB8: lbu         $t8, 0xBF($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0XBF);
    // 0x80091DBC: bnel        $t8, $zero, L_80091DCC
    if (ctx->r24 != 0) {
        // 0x80091DC0: sw          $a0, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r4;
            goto L_80091DCC;
    }
    goto skip_5;
    // 0x80091DC0: sw          $a0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r4;
    skip_5:
    // 0x80091DC4: sb          $a3, 0xBF($v0)
    MEM_B(0XBF, ctx->r2) = ctx->r7;
    // 0x80091DC8: sw          $a0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r4;
L_80091DCC:
    // 0x80091DCC: lw          $t6, 0x0($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X0);
    // 0x80091DD0: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80091DD4: addiu       $a0, $a0, 0x360
    ctx->r4 = ADD32(ctx->r4, 0X360);
    // 0x80091DD8: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x80091DDC: bne         $a1, $t7, L_80091E48
    if (ctx->r5 != ctx->r15) {
        // 0x80091DE0: nop
    
            goto L_80091E48;
    }
    // 0x80091DE0: nop

    // 0x80091DE4: sb          $a2, 0xBE($v0)
    MEM_B(0XBE, ctx->r2) = ctx->r6;
    // 0x80091DE8: lbu         $t9, 0x0($t4)
    ctx->r25 = MEM_BU(ctx->r12, 0X0);
    // 0x80091DEC: multu       $t9, $t5
    result = U64(U32(ctx->r25)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80091DF0: addiu       $t9, $zero, 0x100
    ctx->r25 = ADD32(0, 0X100);
    // 0x80091DF4: mflo        $t8
    ctx->r24 = lo;
    // 0x80091DF8: addu        $t6, $t3, $t8
    ctx->r14 = ADD32(ctx->r11, ctx->r24);
    // 0x80091DFC: lbu         $t7, 0x3($t6)
    ctx->r15 = MEM_BU(ctx->r14, 0X3);
    // 0x80091E00: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x80091E04: div         $zero, $t9, $t7
    lo = S32(S64(S32(ctx->r25)) / S64(S32(ctx->r15))); hi = S32(S64(S32(ctx->r25)) % S64(S32(ctx->r15)));
    // 0x80091E08: mflo        $t8
    ctx->r24 = lo;
    // 0x80091E0C: sb          $t8, 0xBF($t6)
    MEM_B(0XBF, ctx->r14) = ctx->r24;
    // 0x80091E10: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80091E14: bne         $t7, $zero, L_80091E20
    if (ctx->r15 != 0) {
        // 0x80091E18: nop
    
            goto L_80091E20;
    }
    // 0x80091E18: nop

    // 0x80091E1C: break       7
    do_break(2148081180);
L_80091E20:
    // 0x80091E20: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091E24: bne         $t7, $at, L_80091E38
    if (ctx->r15 != ctx->r1) {
        // 0x80091E28: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091E38;
    }
    // 0x80091E28: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091E2C: bne         $t9, $at, L_80091E38
    if (ctx->r25 != ctx->r1) {
        // 0x80091E30: nop
    
            goto L_80091E38;
    }
    // 0x80091E30: nop

    // 0x80091E34: break       6
    do_break(2148081204);
L_80091E38:
    // 0x80091E38: lbu         $t9, 0xBF($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0XBF);
    // 0x80091E3C: bne         $t9, $zero, L_80091E48
    if (ctx->r25 != 0) {
        // 0x80091E40: nop
    
            goto L_80091E48;
    }
    // 0x80091E40: nop

    // 0x80091E44: sb          $a3, 0xBF($v0)
    MEM_B(0XBF, ctx->r2) = ctx->r7;
L_80091E48:
    // 0x80091E48: bne         $a0, $ra, L_80091C4C
    if (ctx->r4 != ctx->r31) {
        // 0x80091E4C: addiu       $t2, $t2, 0x360
        ctx->r10 = ADD32(ctx->r10, 0X360);
            goto L_80091C4C;
    }
    // 0x80091E4C: addiu       $t2, $t2, 0x360
    ctx->r10 = ADD32(ctx->r10, 0X360);
    // 0x80091E50: b           L_80091EC8
    // 0x80091E54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80091EC8;
    // 0x80091E54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80091E58:
    // 0x80091E58: beq         $at, $zero, L_80091E84
    if (ctx->r1 == 0) {
        // 0x80091E5C: or          $s0, $zero, $zero
        ctx->r16 = 0 | 0;
            goto L_80091E84;
    }
    // 0x80091E5C: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80091E60: lui         $t7, 0x8029
    ctx->r15 = S32(0X8029 << 16);
    // 0x80091E64: lw          $t7, 0xE0($t7)
    ctx->r15 = MEM_W(ctx->r15, 0XE0);
    // 0x80091E68: andi        $t8, $t7, 0x7FFF
    ctx->r24 = ctx->r15 & 0X7FFF;
    // 0x80091E6C: bnel        $a1, $t8, L_80091EC8
    if (ctx->r5 != ctx->r24) {
        // 0x80091E70: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80091EC8;
    }
    goto skip_6;
    // 0x80091E70: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_6:
    // 0x80091E74: jal         0x80092C80
    // 0x80091E78: nop

    func_80092C80(rdram, ctx);
        goto after_0;
    // 0x80091E78: nop

    after_0:
    // 0x80091E7C: b           L_80091EC8
    // 0x80091E80: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80091EC8;
    // 0x80091E80: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80091E84:
    // 0x80091E84: lui         $v0, 0x8029
    ctx->r2 = S32(0X8029 << 16);
    // 0x80091E88: addiu       $v0, $v0, 0xE0
    ctx->r2 = ADD32(ctx->r2, 0XE0);
L_80091E8C:
    // 0x80091E8C: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80091E90: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80091E94: andi        $t9, $t6, 0x7FFF
    ctx->r25 = ctx->r14 & 0X7FFF;
    // 0x80091E98: bnel        $a1, $t9, L_80091EB8
    if (ctx->r5 != ctx->r25) {
        // 0x80091E9C: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_80091EB8;
    }
    goto skip_7;
    // 0x80091E9C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_7:
    // 0x80091EA0: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x80091EA4: jal         0x80092D2C
    // 0x80091EA8: sw          $a1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r5;
    func_80092D2C(rdram, ctx);
        goto after_1;
    // 0x80091EA8: sw          $a1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r5;
    after_1:
    // 0x80091EAC: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x80091EB0: lw          $a1, 0x28($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X28);
    // 0x80091EB4: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_80091EB8:
    // 0x80091EB8: addiu       $at, $zero, 0x16
    ctx->r1 = ADD32(0, 0X16);
    // 0x80091EBC: bne         $s0, $at, L_80091E8C
    if (ctx->r16 != ctx->r1) {
        // 0x80091EC0: addiu       $v0, $v0, 0xD8
        ctx->r2 = ADD32(ctx->r2, 0XD8);
            goto L_80091E8C;
    }
    // 0x80091EC0: addiu       $v0, $v0, 0xD8
    ctx->r2 = ADD32(ctx->r2, 0XD8);
    // 0x80091EC4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80091EC8:
    // 0x80091EC8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80091ECC: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80091ED0: jr          $ra
    // 0x80091ED4: nop

    return;
    // 0x80091ED4: nop

;}
RECOMP_FUNC void func_80091ED8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80091ED8: lui         $t6, 0x8014
    ctx->r14 = S32(0X8014 << 16);
    // 0x80091EDC: lbu         $t6, -0x1358($t6)
    ctx->r14 = MEM_BU(ctx->r14, -0X1358);
    // 0x80091EE0: lui         $t8, 0x8014
    ctx->r24 = S32(0X8014 << 16);
    // 0x80091EE4: addiu       $t8, $t8, -0x1350
    ctx->r24 = ADD32(ctx->r24, -0X1350);
    // 0x80091EE8: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80091EEC: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80091EF0: sll         $t7, $t7, 1
    ctx->r15 = S32(ctx->r15 << 1);
    // 0x80091EF4: addu        $v1, $t7, $t8
    ctx->r3 = ADD32(ctx->r15, ctx->r24);
    // 0x80091EF8: lui         $t0, 0x8029
    ctx->r8 = S32(0X8029 << 16);
    // 0x80091EFC: lui         $t3, 0x8029
    ctx->r11 = S32(0X8029 << 16);
    // 0x80091F00: lui         $t5, 0x8029
    ctx->r13 = S32(0X8029 << 16);
    // 0x80091F04: lui         $t4, 0x8014
    ctx->r12 = S32(0X8014 << 16);
    // 0x80091F08: lbu         $v0, 0x3($v1)
    ctx->r2 = MEM_BU(ctx->r3, 0X3);
    // 0x80091F0C: lh          $a2, 0x4($v1)
    ctx->r6 = MEM_H(ctx->r3, 0X4);
    // 0x80091F10: addiu       $t4, $t4, -0x1394
    ctx->r12 = ADD32(ctx->r12, -0X1394);
    // 0x80091F14: addiu       $t5, $t5, 0x1298
    ctx->r13 = ADD32(ctx->r13, 0X1298);
    // 0x80091F18: addiu       $t3, $t3, 0x1B8
    ctx->r11 = ADD32(ctx->r11, 0X1B8);
    // 0x80091F1C: addiu       $t0, $t0, 0xE0
    ctx->r8 = ADD32(ctx->r8, 0XE0);
L_80091F20:
    // 0x80091F20: sw          $t0, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r8;
    // 0x80091F24: lw          $t9, 0x0($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X0);
    // 0x80091F28: or          $v1, $t0, $zero
    ctx->r3 = ctx->r8 | 0;
    // 0x80091F2C: sll         $a3, $a0, 1
    ctx->r7 = S32(ctx->r4 << 1);
    // 0x80091F30: andi        $t6, $t9, 0x7FFF
    ctx->r14 = ctx->r25 & 0X7FFF;
    // 0x80091F34: bne         $a1, $t6, L_80091F94
    if (ctx->r5 != ctx->r14) {
        // 0x80091F38: addu        $t7, $v1, $a3
        ctx->r15 = ADD32(ctx->r3, ctx->r7);
            goto L_80091F94;
    }
    // 0x80091F38: addu        $t7, $v1, $a3
    ctx->r15 = ADD32(ctx->r3, ctx->r7);
    // 0x80091F3C: lh          $t1, 0xC6($t7)
    ctx->r9 = MEM_H(ctx->r15, 0XC6);
    // 0x80091F40: beq         $a2, $t1, L_80091F94
    if (ctx->r6 == ctx->r9) {
        // 0x80091F44: subu        $t2, $a2, $t1
        ctx->r10 = SUB32(ctx->r6, ctx->r9);
            goto L_80091F94;
    }
    // 0x80091F44: subu        $t2, $a2, $t1
    ctx->r10 = SUB32(ctx->r6, ctx->r9);
    // 0x80091F48: div         $zero, $t2, $v0
    lo = S32(S64(S32(ctx->r10)) / S64(S32(ctx->r2))); hi = S32(S64(S32(ctx->r10)) % S64(S32(ctx->r2)));
    // 0x80091F4C: addu        $t8, $v1, $a0
    ctx->r24 = ADD32(ctx->r3, ctx->r4);
    // 0x80091F50: sb          $v0, 0xD2($t8)
    MEM_B(0XD2, ctx->r24) = ctx->r2;
    // 0x80091F54: lw          $t9, 0x0($t4)
    ctx->r25 = MEM_W(ctx->r12, 0X0);
    // 0x80091F58: bne         $v0, $zero, L_80091F64
    if (ctx->r2 != 0) {
        // 0x80091F5C: nop
    
            goto L_80091F64;
    }
    // 0x80091F5C: nop

    // 0x80091F60: break       7
    do_break(2148081504);
L_80091F64:
    // 0x80091F64: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091F68: bne         $v0, $at, L_80091F7C
    if (ctx->r2 != ctx->r1) {
        // 0x80091F6C: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091F7C;
    }
    // 0x80091F6C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091F70: bne         $t2, $at, L_80091F7C
    if (ctx->r10 != ctx->r1) {
        // 0x80091F74: nop
    
            goto L_80091F7C;
    }
    // 0x80091F74: nop

    // 0x80091F78: break       6
    do_break(2148081528);
L_80091F7C:
    // 0x80091F7C: addu        $t6, $t9, $a3
    ctx->r14 = ADD32(ctx->r25, ctx->r7);
    // 0x80091F80: sh          $a2, 0xC0($t6)
    MEM_H(0XC0, ctx->r14) = ctx->r6;
    // 0x80091F84: lw          $t7, 0x0($t4)
    ctx->r15 = MEM_W(ctx->r12, 0X0);
    // 0x80091F88: mflo        $t2
    ctx->r10 = lo;
    // 0x80091F8C: addu        $t8, $t7, $a3
    ctx->r24 = ADD32(ctx->r15, ctx->r7);
    // 0x80091F90: sh          $t2, 0xCC($t8)
    MEM_H(0XCC, ctx->r24) = ctx->r10;
L_80091F94:
    // 0x80091F94: sw          $t3, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r11;
    // 0x80091F98: lw          $t9, 0x0($t3)
    ctx->r25 = MEM_W(ctx->r11, 0X0);
    // 0x80091F9C: or          $v1, $t3, $zero
    ctx->r3 = ctx->r11 | 0;
    // 0x80091FA0: addiu       $t3, $t3, 0x1B0
    ctx->r11 = ADD32(ctx->r11, 0X1B0);
    // 0x80091FA4: andi        $t6, $t9, 0x7FFF
    ctx->r14 = ctx->r25 & 0X7FFF;
    // 0x80091FA8: bne         $a1, $t6, L_8009200C
    if (ctx->r5 != ctx->r14) {
        // 0x80091FAC: sll         $a3, $a0, 1
        ctx->r7 = S32(ctx->r4 << 1);
            goto L_8009200C;
    }
    // 0x80091FAC: sll         $a3, $a0, 1
    ctx->r7 = S32(ctx->r4 << 1);
    // 0x80091FB0: addu        $t7, $v1, $a3
    ctx->r15 = ADD32(ctx->r3, ctx->r7);
    // 0x80091FB4: lh          $t1, 0xC6($t7)
    ctx->r9 = MEM_H(ctx->r15, 0XC6);
    // 0x80091FB8: beq         $a2, $t1, L_8009200C
    if (ctx->r6 == ctx->r9) {
        // 0x80091FBC: subu        $t2, $a2, $t1
        ctx->r10 = SUB32(ctx->r6, ctx->r9);
            goto L_8009200C;
    }
    // 0x80091FBC: subu        $t2, $a2, $t1
    ctx->r10 = SUB32(ctx->r6, ctx->r9);
    // 0x80091FC0: div         $zero, $t2, $v0
    lo = S32(S64(S32(ctx->r10)) / S64(S32(ctx->r2))); hi = S32(S64(S32(ctx->r10)) % S64(S32(ctx->r2)));
    // 0x80091FC4: addu        $t8, $v1, $a0
    ctx->r24 = ADD32(ctx->r3, ctx->r4);
    // 0x80091FC8: sb          $v0, 0xD2($t8)
    MEM_B(0XD2, ctx->r24) = ctx->r2;
    // 0x80091FCC: lw          $t9, 0x0($t4)
    ctx->r25 = MEM_W(ctx->r12, 0X0);
    // 0x80091FD0: bne         $v0, $zero, L_80091FDC
    if (ctx->r2 != 0) {
        // 0x80091FD4: nop
    
            goto L_80091FDC;
    }
    // 0x80091FD4: nop

    // 0x80091FD8: break       7
    do_break(2148081624);
L_80091FDC:
    // 0x80091FDC: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80091FE0: bne         $v0, $at, L_80091FF4
    if (ctx->r2 != ctx->r1) {
        // 0x80091FE4: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_80091FF4;
    }
    // 0x80091FE4: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80091FE8: bne         $t2, $at, L_80091FF4
    if (ctx->r10 != ctx->r1) {
        // 0x80091FEC: nop
    
            goto L_80091FF4;
    }
    // 0x80091FEC: nop

    // 0x80091FF0: break       6
    do_break(2148081648);
L_80091FF4:
    // 0x80091FF4: addu        $t6, $t9, $a3
    ctx->r14 = ADD32(ctx->r25, ctx->r7);
    // 0x80091FF8: sh          $a2, 0xC0($t6)
    MEM_H(0XC0, ctx->r14) = ctx->r6;
    // 0x80091FFC: lw          $t7, 0x0($t4)
    ctx->r15 = MEM_W(ctx->r12, 0X0);
    // 0x80092000: mflo        $t2
    ctx->r10 = lo;
    // 0x80092004: addu        $t8, $t7, $a3
    ctx->r24 = ADD32(ctx->r15, ctx->r7);
    // 0x80092008: sh          $t2, 0xCC($t8)
    MEM_H(0XCC, ctx->r24) = ctx->r10;
L_8009200C:
    // 0x8009200C: bne         $t3, $t5, L_80091F20
    if (ctx->r11 != ctx->r13) {
        // 0x80092010: addiu       $t0, $t0, 0x1B0
        ctx->r8 = ADD32(ctx->r8, 0X1B0);
            goto L_80091F20;
    }
    // 0x80092010: addiu       $t0, $t0, 0x1B0
    ctx->r8 = ADD32(ctx->r8, 0X1B0);
    // 0x80092014: jr          $ra
    // 0x80092018: nop

    return;
    // 0x80092018: nop

;}
RECOMP_FUNC void func_8009201C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8009201C: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80092020: lw          $v0, -0x1620($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X1620);
    // 0x80092024: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80092028: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8009202C: andi        $v0, $v0, 0x7FFF
    ctx->r2 = ctx->r2 & 0X7FFF;
    // 0x80092030: andi        $a1, $v0, 0xFFFF
    ctx->r5 = ctx->r2 & 0XFFFF;
    // 0x80092034: slti        $at, $a1, 0x3E5
    ctx->r1 = SIGNED(ctx->r5) < 0X3E5 ? 1 : 0;
    // 0x80092038: bne         $at, $zero, L_80092050
    if (ctx->r1 != 0) {
        // 0x8009203C: sw          $a0, 0x38($sp)
        MEM_W(0X38, ctx->r29) = ctx->r4;
            goto L_80092050;
    }
    // 0x8009203C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x80092040: jal         0x800922D4
    // 0x80092044: nop

    func_800922D4(rdram, ctx);
        goto after_0;
    // 0x80092044: nop

    after_0:
    // 0x80092048: b           L_800922C8
    // 0x8009204C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_800922C8;
    // 0x8009204C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80092050:
    // 0x80092050: slti        $at, $a1, 0x200
    ctx->r1 = SIGNED(ctx->r5) < 0X200 ? 1 : 0;
    // 0x80092054: bne         $at, $zero, L_80092160
    if (ctx->r1 != 0) {
        // 0x80092058: slti        $at, $a1, 0x3E5
        ctx->r1 = SIGNED(ctx->r5) < 0X3E5 ? 1 : 0;
            goto L_80092160;
    }
    // 0x80092058: slti        $at, $a1, 0x3E5
    ctx->r1 = SIGNED(ctx->r5) < 0X3E5 ? 1 : 0;
    // 0x8009205C: beq         $at, $zero, L_800922C4
    if (ctx->r1 == 0) {
        // 0x80092060: sra         $t6, $a1, 8
        ctx->r14 = S32(SIGNED(ctx->r5) >> 8);
            goto L_800922C4;
    }
    // 0x80092060: sra         $t6, $a1, 8
    ctx->r14 = S32(SIGNED(ctx->r5) >> 8);
    // 0x80092064: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80092068: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x8009206C: addu        $v0, $v0, $t7
    ctx->r2 = ADD32(ctx->r2, ctx->r15);
    // 0x80092070: lw          $v0, -0x78F0($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X78F0);
    // 0x80092074: andi        $t8, $a1, 0xFF
    ctx->r24 = ctx->r5 & 0XFF;
    // 0x80092078: sll         $t9, $t8, 3
    ctx->r25 = S32(ctx->r24 << 3);
    // 0x8009207C: addu        $t1, $v0, $t9
    ctx->r9 = ADD32(ctx->r2, ctx->r25);
    // 0x80092080: lbu         $t3, 0x0($t1)
    ctx->r11 = MEM_BU(ctx->r9, 0X0);
    // 0x80092084: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80092088: addiu       $t0, $t0, -0x1690
    ctx->r8 = ADD32(ctx->r8, -0X1690);
    // 0x8009208C: sb          $t3, 0x2($t0)
    MEM_B(0X2, ctx->r8) = ctx->r11;
    // 0x80092090: andi        $t4, $t3, 0x1F
    ctx->r12 = ctx->r11 & 0X1F;
    // 0x80092094: sb          $t4, 0x2($t0)
    MEM_B(0X2, ctx->r8) = ctx->r12;
    // 0x80092098: lbu         $a3, 0x1($t1)
    ctx->r7 = MEM_BU(ctx->r9, 0X1);
    // 0x8009209C: addiu       $v0, $zero, 0xA
    ctx->r2 = ADD32(0, 0XA);
    // 0x800920A0: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x800920A4: andi        $t5, $a3, 0xF
    ctx->r13 = ctx->r7 & 0XF;
    // 0x800920A8: sra         $a3, $a3, 4
    ctx->r7 = S32(SIGNED(ctx->r7) >> 4);
    // 0x800920AC: andi        $a3, $a3, 0xFF
    ctx->r7 = ctx->r7 & 0XFF;
    // 0x800920B0: sb          $t5, 0x3($t0)
    MEM_B(0X3, ctx->r8) = ctx->r13;
    // 0x800920B4: bne         $a3, $zero, L_80092110
    if (ctx->r7 != 0) {
        // 0x800920B8: or          $a2, $a3, $zero
        ctx->r6 = ctx->r7 | 0;
            goto L_80092110;
    }
    // 0x800920B8: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x800920BC: lui         $v1, 0x8029
    ctx->r3 = S32(0X8029 << 16);
    // 0x800920C0: addiu       $v1, $v1, 0x950
    ctx->r3 = ADD32(ctx->r3, 0X950);
    // 0x800920C4: addiu       $a1, $zero, 0x14
    ctx->r5 = ADD32(0, 0X14);
L_800920C8:
    // 0x800920C8: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x800920CC: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x800920D0: bnel        $a0, $t7, L_800920E8
    if (ctx->r4 != ctx->r15) {
        // 0x800920D4: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_800920E8;
    }
    goto skip_0;
    // 0x800920D4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_0:
    // 0x800920D8: andi        $a3, $v0, 0xFF
    ctx->r7 = ctx->r2 & 0XFF;
    // 0x800920DC: b           L_800920F0
    // 0x800920E0: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
        goto L_800920F0;
    // 0x800920E0: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x800920E4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_800920E8:
    // 0x800920E8: bne         $v0, $a1, L_800920C8
    if (ctx->r2 != ctx->r5) {
        // 0x800920EC: addiu       $v1, $v1, 0xD8
        ctx->r3 = ADD32(ctx->r3, 0XD8);
            goto L_800920C8;
    }
    // 0x800920EC: addiu       $v1, $v1, 0xD8
    ctx->r3 = ADD32(ctx->r3, 0XD8);
L_800920F0:
    // 0x800920F0: bne         $a2, $zero, L_80092110
    if (ctx->r6 != 0) {
        // 0x800920F4: addiu       $a0, $zero, 0x1
        ctx->r4 = ADD32(0, 0X1);
            goto L_80092110;
    }
    // 0x800920F4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x800920F8: jal         0x80092410
    // 0x800920FC: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    func_80092410(rdram, ctx);
        goto after_1;
    // 0x800920FC: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    after_1:
    // 0x80092100: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80092104: addiu       $t0, $t0, -0x1690
    ctx->r8 = ADD32(ctx->r8, -0X1690);
    // 0x80092108: lw          $t1, 0x1C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X1C);
    // 0x8009210C: andi        $a3, $v0, 0xFF
    ctx->r7 = ctx->r2 & 0XFF;
L_80092110:
    // 0x80092110: sll         $t9, $a3, 3
    ctx->r25 = S32(ctx->r7 << 3);
    // 0x80092114: subu        $t9, $t9, $a3
    ctx->r25 = SUB32(ctx->r25, ctx->r7);
    // 0x80092118: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x8009211C: subu        $t9, $t9, $a3
    ctx->r25 = SUB32(ctx->r25, ctx->r7);
    // 0x80092120: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x80092124: lui         $t2, 0x8029
    ctx->r10 = S32(0X8029 << 16);
    // 0x80092128: addu        $t2, $t2, $t9
    ctx->r10 = ADD32(ctx->r10, ctx->r25);
    // 0x8009212C: lbu         $t2, 0xE4($t2)
    ctx->r10 = MEM_BU(ctx->r10, 0XE4);
    // 0x80092130: lbu         $t8, 0x2($t0)
    ctx->r24 = MEM_BU(ctx->r8, 0X2);
    // 0x80092134: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80092138: addiu       $v0, $v0, -0x1618
    ctx->r2 = ADD32(ctx->r2, -0X1618);
    // 0x8009213C: slt         $at, $t8, $t2
    ctx->r1 = SIGNED(ctx->r24) < SIGNED(ctx->r10) ? 1 : 0;
    // 0x80092140: bne         $at, $zero, L_800922C4
    if (ctx->r1 != 0) {
        // 0x80092144: addiu       $t3, $zero, 0x1
        ctx->r11 = ADD32(0, 0X1);
            goto L_800922C4;
    }
    // 0x80092144: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x80092148: sb          $a3, 0x1($t0)
    MEM_B(0X1, ctx->r8) = ctx->r7;
    // 0x8009214C: sb          $t3, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r11;
    // 0x80092150: sw          $v0, 0x8($t0)
    MEM_W(0X8, ctx->r8) = ctx->r2;
    // 0x80092154: lw          $t4, 0x4($t1)
    ctx->r12 = MEM_W(ctx->r9, 0X4);
    // 0x80092158: b           L_800922B4
    // 0x8009215C: sw          $t4, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r12;
        goto L_800922B4;
    // 0x8009215C: sw          $t4, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r12;
L_80092160:
    // 0x80092160: slti        $at, $a1, 0x1F8
    ctx->r1 = SIGNED(ctx->r5) < 0X1F8 ? 1 : 0;
    // 0x80092164: beq         $at, $zero, L_800922C4
    if (ctx->r1 == 0) {
        // 0x80092168: sra         $t5, $a1, 8
        ctx->r13 = S32(SIGNED(ctx->r5) >> 8);
            goto L_800922C4;
    }
    // 0x80092168: sra         $t5, $a1, 8
    ctx->r13 = S32(SIGNED(ctx->r5) >> 8);
    // 0x8009216C: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x80092170: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80092174: addu        $v0, $v0, $t6
    ctx->r2 = ADD32(ctx->r2, ctx->r14);
    // 0x80092178: lw          $v0, -0x78F0($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X78F0);
    // 0x8009217C: andi        $t7, $a1, 0xFF
    ctx->r15 = ctx->r5 & 0XFF;
    // 0x80092180: sll         $t9, $t7, 3
    ctx->r25 = S32(ctx->r15 << 3);
    // 0x80092184: addu        $t1, $v0, $t9
    ctx->r9 = ADD32(ctx->r2, ctx->r25);
    // 0x80092188: lbu         $a3, 0x0($t1)
    ctx->r7 = MEM_BU(ctx->r9, 0X0);
    // 0x8009218C: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80092190: addiu       $t0, $t0, -0x1690
    ctx->r8 = ADD32(ctx->r8, -0X1690);
    // 0x80092194: sra         $t2, $a3, 5
    ctx->r10 = S32(SIGNED(ctx->r7) >> 5);
    // 0x80092198: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8009219C: andi        $t8, $a3, 0x1F
    ctx->r24 = ctx->r7 & 0X1F;
    // 0x800921A0: sb          $t8, 0x2($t0)
    MEM_B(0X2, ctx->r8) = ctx->r24;
    // 0x800921A4: sb          $t3, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r11;
    // 0x800921A8: lbu         $a3, 0x1($t1)
    ctx->r7 = MEM_BU(ctx->r9, 0X1);
    // 0x800921AC: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    // 0x800921B0: lui         $t6, 0x8029
    ctx->r14 = S32(0X8029 << 16);
    // 0x800921B4: andi        $t4, $a3, 0xF
    ctx->r12 = ctx->r7 & 0XF;
    // 0x800921B8: sra         $a3, $a3, 4
    ctx->r7 = S32(SIGNED(ctx->r7) >> 4);
    // 0x800921BC: andi        $a3, $a3, 0xFF
    ctx->r7 = ctx->r7 & 0XFF;
    // 0x800921C0: sb          $t4, 0x3($t0)
    MEM_B(0X3, ctx->r8) = ctx->r12;
    // 0x800921C4: bne         $a3, $at, L_800921E0
    if (ctx->r7 != ctx->r1) {
        // 0x800921C8: or          $a2, $a3, $zero
        ctx->r6 = ctx->r7 | 0;
            goto L_800921E0;
    }
    // 0x800921C8: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x800921CC: lbu         $t6, 0x6CC($t6)
    ctx->r14 = MEM_BU(ctx->r14, 0X6CC);
    // 0x800921D0: andi        $t5, $t8, 0xFF
    ctx->r13 = ctx->r24 & 0XFF;
    // 0x800921D4: slt         $at, $t5, $t6
    ctx->r1 = SIGNED(ctx->r13) < SIGNED(ctx->r14) ? 1 : 0;
    // 0x800921D8: bnel        $at, $zero, L_800922C8
    if (ctx->r1 != 0) {
        // 0x800921DC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_800922C8;
    }
    goto skip_1;
    // 0x800921DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
L_800921E0:
    // 0x800921E0: lbu         $t7, 0x0($t0)
    ctx->r15 = MEM_BU(ctx->r8, 0X0);
    // 0x800921E4: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x800921E8: bnel        $t7, $at, L_800922AC
    if (ctx->r15 != ctx->r1) {
        // 0x800921EC: sb          $a3, 0x1($t0)
        MEM_B(0X1, ctx->r8) = ctx->r7;
            goto L_800922AC;
    }
    goto skip_2;
    // 0x800921EC: sb          $a3, 0x1($t0)
    MEM_B(0X1, ctx->r8) = ctx->r7;
    skip_2:
    // 0x800921F0: bne         $a2, $zero, L_80092250
    if (ctx->r6 != 0) {
        // 0x800921F4: addiu       $v0, $zero, 0x12
        ctx->r2 = ADD32(0, 0X12);
            goto L_80092250;
    }
    // 0x800921F4: addiu       $v0, $zero, 0x12
    ctx->r2 = ADD32(0, 0X12);
    // 0x800921F8: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x800921FC: lui         $v1, 0x8029
    ctx->r3 = S32(0X8029 << 16);
    // 0x80092200: addiu       $v1, $v1, 0x1010
    ctx->r3 = ADD32(ctx->r3, 0X1010);
    // 0x80092204: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
L_80092208:
    // 0x80092208: lw          $t9, 0x0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X0);
    // 0x8009220C: andi        $t8, $t9, 0x7FFF
    ctx->r24 = ctx->r25 & 0X7FFF;
    // 0x80092210: bnel        $a0, $t8, L_80092228
    if (ctx->r4 != ctx->r24) {
        // 0x80092214: addiu       $v0, $v0, -0x2
        ctx->r2 = ADD32(ctx->r2, -0X2);
            goto L_80092228;
    }
    goto skip_3;
    // 0x80092214: addiu       $v0, $v0, -0x2
    ctx->r2 = ADD32(ctx->r2, -0X2);
    skip_3:
    // 0x80092218: andi        $a3, $v0, 0xFF
    ctx->r7 = ctx->r2 & 0XFF;
    // 0x8009221C: b           L_80092230
    // 0x80092220: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
        goto L_80092230;
    // 0x80092220: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    // 0x80092224: addiu       $v0, $v0, -0x2
    ctx->r2 = ADD32(ctx->r2, -0X2);
L_80092228:
    // 0x80092228: bne         $v0, $a1, L_80092208
    if (ctx->r2 != ctx->r5) {
        // 0x8009222C: addiu       $v1, $v1, -0x1B0
        ctx->r3 = ADD32(ctx->r3, -0X1B0);
            goto L_80092208;
    }
    // 0x8009222C: addiu       $v1, $v1, -0x1B0
    ctx->r3 = ADD32(ctx->r3, -0X1B0);
L_80092230:
    // 0x80092230: bne         $a2, $zero, L_80092250
    if (ctx->r6 != 0) {
        // 0x80092234: addiu       $a0, $zero, 0x2
        ctx->r4 = ADD32(0, 0X2);
            goto L_80092250;
    }
    // 0x80092234: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x80092238: jal         0x80092410
    // 0x8009223C: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    func_80092410(rdram, ctx);
        goto after_2;
    // 0x8009223C: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    after_2:
    // 0x80092240: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80092244: addiu       $t0, $t0, -0x1690
    ctx->r8 = ADD32(ctx->r8, -0X1690);
    // 0x80092248: lw          $t1, 0x1C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X1C);
    // 0x8009224C: andi        $a3, $v0, 0xFF
    ctx->r7 = ctx->r2 & 0XFF;
L_80092250:
    // 0x80092250: sll         $t2, $a3, 3
    ctx->r10 = S32(ctx->r7 << 3);
    // 0x80092254: subu        $t2, $t2, $a3
    ctx->r10 = SUB32(ctx->r10, ctx->r7);
    // 0x80092258: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x8009225C: subu        $t2, $t2, $a3
    ctx->r10 = SUB32(ctx->r10, ctx->r7);
    // 0x80092260: sll         $t2, $t2, 3
    ctx->r10 = S32(ctx->r10 << 3);
    // 0x80092264: lui         $t3, 0x8029
    ctx->r11 = S32(0X8029 << 16);
    // 0x80092268: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x8009226C: lbu         $t3, 0xE4($t3)
    ctx->r11 = MEM_BU(ctx->r11, 0XE4);
    // 0x80092270: lbu         $v0, 0x2($t0)
    ctx->r2 = MEM_BU(ctx->r8, 0X2);
    // 0x80092274: sll         $t4, $a3, 3
    ctx->r12 = S32(ctx->r7 << 3);
    // 0x80092278: subu        $t4, $t4, $a3
    ctx->r12 = SUB32(ctx->r12, ctx->r7);
    // 0x8009227C: slt         $at, $v0, $t3
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r11) ? 1 : 0;
    // 0x80092280: bne         $at, $zero, L_800922C4
    if (ctx->r1 != 0) {
        // 0x80092284: sll         $t4, $t4, 2
        ctx->r12 = S32(ctx->r12 << 2);
            goto L_800922C4;
    }
    // 0x80092284: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x80092288: subu        $t4, $t4, $a3
    ctx->r12 = SUB32(ctx->r12, ctx->r7);
    // 0x8009228C: sll         $t4, $t4, 3
    ctx->r12 = S32(ctx->r12 << 3);
    // 0x80092290: lui         $t5, 0x8029
    ctx->r13 = S32(0X8029 << 16);
    // 0x80092294: addu        $t5, $t5, $t4
    ctx->r13 = ADD32(ctx->r13, ctx->r12);
    // 0x80092298: lbu         $t5, 0x1BC($t5)
    ctx->r13 = MEM_BU(ctx->r13, 0X1BC);
    // 0x8009229C: slt         $at, $v0, $t5
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r13) ? 1 : 0;
    // 0x800922A0: bnel        $at, $zero, L_800922C8
    if (ctx->r1 != 0) {
        // 0x800922A4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_800922C8;
    }
    goto skip_4;
    // 0x800922A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_4:
    // 0x800922A8: sb          $a3, 0x1($t0)
    MEM_B(0X1, ctx->r8) = ctx->r7;
L_800922AC:
    // 0x800922AC: lw          $t6, 0x4($t1)
    ctx->r14 = MEM_W(ctx->r9, 0X4);
    // 0x800922B0: sw          $t6, 0x8($t0)
    MEM_W(0X8, ctx->r8) = ctx->r14;
L_800922B4:
    // 0x800922B4: addiu       $t7, $zero, 0x6
    ctx->r15 = ADD32(0, 0X6);
    // 0x800922B8: sb          $t7, 0x4($t0)
    MEM_B(0X4, ctx->r8) = ctx->r15;
    // 0x800922BC: jal         0x800928E0
    // 0x800922C0: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    func_800928E0(rdram, ctx);
        goto after_3;
    // 0x800922C0: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    after_3:
L_800922C4:
    // 0x800922C4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_800922C8:
    // 0x800922C8: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x800922CC: jr          $ra
    // 0x800922D0: nop

    return;
    // 0x800922D0: nop

;}
RECOMP_FUNC void func_800922D4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800922D4: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x800922D8: lw          $v0, -0x1620($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X1620);
    // 0x800922DC: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x800922E0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800922E4: andi        $v0, $v0, 0x7FFF
    ctx->r2 = ctx->r2 & 0X7FFF;
    // 0x800922E8: andi        $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 & 0XFFFF;
    // 0x800922EC: slti        $at, $v0, 0x421
    ctx->r1 = SIGNED(ctx->r2) < 0X421 ? 1 : 0;
    // 0x800922F0: beq         $at, $zero, L_800923F8
    if (ctx->r1 == 0) {
        // 0x800922F4: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_800923F8;
    }
    // 0x800922F4: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x800922F8: lui         $t7, 0x8028
    ctx->r15 = S32(0X8028 << 16);
    // 0x800922FC: addiu       $t7, $t7, -0x3108
    ctx->r15 = ADD32(ctx->r15, -0X3108);
    // 0x80092300: sll         $t6, $v0, 3
    ctx->r14 = S32(ctx->r2 << 3);
    // 0x80092304: addu        $t1, $t6, $t7
    ctx->r9 = ADD32(ctx->r14, ctx->r15);
    // 0x80092308: lbu         $a2, -0x1F27($t1)
    ctx->r6 = MEM_BU(ctx->r9, -0X1F27);
    // 0x8009230C: lbu         $t9, -0x1F28($t1)
    ctx->r25 = MEM_BU(ctx->r9, -0X1F28);
    // 0x80092310: lui         $a3, 0x8014
    ctx->r7 = S32(0X8014 << 16);
    // 0x80092314: sra         $a2, $a2, 4
    ctx->r6 = S32(SIGNED(ctx->r6) >> 4);
    // 0x80092318: addiu       $a3, $a3, -0x1690
    ctx->r7 = ADD32(ctx->r7, -0X1690);
    // 0x8009231C: addiu       $a2, $a2, 0xF
    ctx->r6 = ADD32(ctx->r6, 0XF);
    // 0x80092320: addiu       $t2, $zero, 0xF
    ctx->r10 = ADD32(0, 0XF);
    // 0x80092324: andi        $a2, $a2, 0xFF
    ctx->r6 = ctx->r6 & 0XFF;
    // 0x80092328: sb          $t9, 0x2($a3)
    MEM_B(0X2, ctx->r7) = ctx->r25;
    // 0x8009232C: andi        $t3, $t9, 0x1F
    ctx->r11 = ctx->r25 & 0X1F;
    // 0x80092330: sb          $t3, 0x2($a3)
    MEM_B(0X2, ctx->r7) = ctx->r11;
    // 0x80092334: bne         $t2, $a2, L_80092398
    if (ctx->r10 != ctx->r6) {
        // 0x80092338: or          $t0, $a2, $zero
        ctx->r8 = ctx->r6 | 0;
            goto L_80092398;
    }
    // 0x80092338: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
    // 0x8009233C: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80092340: lui         $v1, 0x8029
    ctx->r3 = S32(0X8029 << 16);
    // 0x80092344: addiu       $v1, $v1, 0x11C0
    ctx->r3 = ADD32(ctx->r3, 0X11C0);
    // 0x80092348: addiu       $a1, $zero, 0x16
    ctx->r5 = ADD32(0, 0X16);
    // 0x8009234C: addiu       $v0, $zero, 0x14
    ctx->r2 = ADD32(0, 0X14);
L_80092350:
    // 0x80092350: lw          $t4, 0x0($v1)
    ctx->r12 = MEM_W(ctx->r3, 0X0);
    // 0x80092354: andi        $t5, $t4, 0x7FFF
    ctx->r13 = ctx->r12 & 0X7FFF;
    // 0x80092358: bnel        $a0, $t5, L_80092370
    if (ctx->r4 != ctx->r13) {
        // 0x8009235C: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80092370;
    }
    goto skip_0;
    // 0x8009235C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    skip_0:
    // 0x80092360: andi        $a2, $v0, 0xFF
    ctx->r6 = ctx->r2 & 0XFF;
    // 0x80092364: b           L_80092378
    // 0x80092368: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
        goto L_80092378;
    // 0x80092368: or          $t0, $a2, $zero
    ctx->r8 = ctx->r6 | 0;
    // 0x8009236C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
L_80092370:
    // 0x80092370: bne         $v0, $a1, L_80092350
    if (ctx->r2 != ctx->r5) {
        // 0x80092374: addiu       $v1, $v1, 0xD8
        ctx->r3 = ADD32(ctx->r3, 0XD8);
            goto L_80092350;
    }
    // 0x80092374: addiu       $v1, $v1, 0xD8
    ctx->r3 = ADD32(ctx->r3, 0XD8);
L_80092378:
    // 0x80092378: bne         $t2, $t0, L_80092398
    if (ctx->r10 != ctx->r8) {
        // 0x8009237C: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_80092398;
    }
    // 0x8009237C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80092380: jal         0x80092410
    // 0x80092384: sw          $t1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r9;
    func_80092410(rdram, ctx);
        goto after_0;
    // 0x80092384: sw          $t1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r9;
    after_0:
    // 0x80092388: lui         $a3, 0x8014
    ctx->r7 = S32(0X8014 << 16);
    // 0x8009238C: addiu       $a3, $a3, -0x1690
    ctx->r7 = ADD32(ctx->r7, -0X1690);
    // 0x80092390: lw          $t1, 0x18($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X18);
    // 0x80092394: andi        $a2, $v0, 0xFF
    ctx->r6 = ctx->r2 & 0XFF;
L_80092398:
    // 0x80092398: sll         $t7, $a2, 3
    ctx->r15 = S32(ctx->r6 << 3);
    // 0x8009239C: subu        $t7, $t7, $a2
    ctx->r15 = SUB32(ctx->r15, ctx->r6);
    // 0x800923A0: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x800923A4: subu        $t7, $t7, $a2
    ctx->r15 = SUB32(ctx->r15, ctx->r6);
    // 0x800923A8: sll         $t7, $t7, 3
    ctx->r15 = S32(ctx->r15 << 3);
    // 0x800923AC: lui         $t8, 0x8029
    ctx->r24 = S32(0X8029 << 16);
    // 0x800923B0: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x800923B4: lbu         $t8, 0xE4($t8)
    ctx->r24 = MEM_BU(ctx->r24, 0XE4);
    // 0x800923B8: lbu         $t6, 0x2($a3)
    ctx->r14 = MEM_BU(ctx->r7, 0X2);
    // 0x800923BC: lui         $t3, 0x8014
    ctx->r11 = S32(0X8014 << 16);
    // 0x800923C0: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x800923C4: slt         $at, $t6, $t8
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r24) ? 1 : 0;
    // 0x800923C8: bne         $at, $zero, L_800923F8
    if (ctx->r1 != 0) {
        // 0x800923CC: addiu       $t3, $t3, -0x1618
        ctx->r11 = ADD32(ctx->r11, -0X1618);
            goto L_800923F8;
    }
    // 0x800923CC: addiu       $t3, $t3, -0x1618
    ctx->r11 = ADD32(ctx->r11, -0X1618);
    // 0x800923D0: lw          $t4, -0x1F24($t1)
    ctx->r12 = MEM_W(ctx->r9, -0X1F24);
    // 0x800923D4: sb          $a2, 0x1($a3)
    MEM_B(0X1, ctx->r7) = ctx->r6;
    // 0x800923D8: sb          $t9, 0x0($a3)
    MEM_B(0X0, ctx->r7) = ctx->r25;
    // 0x800923DC: sw          $t3, 0x8($a3)
    MEM_W(0X8, ctx->r7) = ctx->r11;
    // 0x800923E0: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x800923E4: sw          $t4, -0x1618($at)
    MEM_W(-0X1618, ctx->r1) = ctx->r12;
    // 0x800923E8: sb          $zero, 0x3($a3)
    MEM_B(0X3, ctx->r7) = 0;
    // 0x800923EC: sb          $zero, 0x4($a3)
    MEM_B(0X4, ctx->r7) = 0;
    // 0x800923F0: jal         0x800928E0
    // 0x800923F4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_800928E0(rdram, ctx);
        goto after_1;
    // 0x800923F4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_1:
L_800923F8:
    // 0x800923F8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800923FC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80092400: jr          $ra
    // 0x80092404: nop

    return;
    // 0x80092404: nop

    // 0x80092408: jr          $ra
    // 0x8009240C: nop

    return;
    // 0x8009240C: nop

;}
RECOMP_FUNC void func_80092410(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80092410: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80092414: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x80092418: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8009241C: bne         $a0, $at, L_8009247C
    if (ctx->r4 != ctx->r1) {
        // 0x80092420: addiu       $v1, $zero, 0xA
        ctx->r3 = ADD32(0, 0XA);
            goto L_8009247C;
    }
    // 0x80092420: addiu       $v1, $zero, 0xA
    ctx->r3 = ADD32(0, 0XA);
    // 0x80092424: lui         $v0, 0x8029
    ctx->r2 = S32(0X8029 << 16);
    // 0x80092428: lui         $t1, 0x8029
    ctx->r9 = S32(0X8029 << 16);
    // 0x8009242C: lbu         $v0, 0x954($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X954);
    // 0x80092430: addiu       $t1, $t1, 0xE0
    ctx->r9 = ADD32(ctx->r9, 0XE0);
    // 0x80092434: addiu       $a0, $zero, 0xB
    ctx->r4 = ADD32(0, 0XB);
    // 0x80092438: addiu       $t2, $zero, 0xD8
    ctx->r10 = ADD32(0, 0XD8);
L_8009243C:
    // 0x8009243C: multu       $a0, $t2
    result = U64(U32(ctx->r4)) * U64(U32(ctx->r10)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80092440: mflo        $t6
    ctx->r14 = lo;
    // 0x80092444: addu        $t7, $t1, $t6
    ctx->r15 = ADD32(ctx->r9, ctx->r14);
    // 0x80092448: lbu         $a1, 0x4($t7)
    ctx->r5 = MEM_BU(ctx->r15, 0X4);
    // 0x8009244C: slt         $at, $v0, $a1
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x80092450: bnel        $at, $zero, L_80092464
    if (ctx->r1 != 0) {
        // 0x80092454: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_80092464;
    }
    goto skip_0;
    // 0x80092454: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    skip_0:
    // 0x80092458: andi        $v1, $a0, 0xFF
    ctx->r3 = ctx->r4 & 0XFF;
    // 0x8009245C: andi        $v0, $a1, 0xFF
    ctx->r2 = ctx->r5 & 0XFF;
    // 0x80092460: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_80092464:
    // 0x80092464: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x80092468: slti        $at, $a0, 0x14
    ctx->r1 = SIGNED(ctx->r4) < 0X14 ? 1 : 0;
    // 0x8009246C: bne         $at, $zero, L_8009243C
    if (ctx->r1 != 0) {
        // 0x80092470: nop
    
            goto L_8009243C;
    }
    // 0x80092470: nop

    // 0x80092474: jr          $ra
    // 0x80092478: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80092478: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8009247C:
    // 0x8009247C: bne         $a0, $zero, L_800924E0
    if (ctx->r4 != 0) {
        // 0x80092480: addiu       $v1, $zero, 0x12
        ctx->r3 = ADD32(0, 0X12);
            goto L_800924E0;
    }
    // 0x80092480: addiu       $v1, $zero, 0x12
    ctx->r3 = ADD32(0, 0X12);
    // 0x80092484: lui         $v0, 0x8029
    ctx->r2 = S32(0X8029 << 16);
    // 0x80092488: lui         $t1, 0x8029
    ctx->r9 = S32(0X8029 << 16);
    // 0x8009248C: addiu       $v1, $zero, 0x14
    ctx->r3 = ADD32(0, 0X14);
    // 0x80092490: lbu         $v0, 0x11C4($v0)
    ctx->r2 = MEM_BU(ctx->r2, 0X11C4);
    // 0x80092494: addiu       $t1, $t1, 0xE0
    ctx->r9 = ADD32(ctx->r9, 0XE0);
    // 0x80092498: addiu       $a0, $zero, 0x15
    ctx->r4 = ADD32(0, 0X15);
    // 0x8009249C: addiu       $t2, $zero, 0xD8
    ctx->r10 = ADD32(0, 0XD8);
L_800924A0:
    // 0x800924A0: multu       $a0, $t2
    result = U64(U32(ctx->r4)) * U64(U32(ctx->r10)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x800924A4: mflo        $t8
    ctx->r24 = lo;
    // 0x800924A8: addu        $t9, $t1, $t8
    ctx->r25 = ADD32(ctx->r9, ctx->r24);
    // 0x800924AC: lbu         $a1, 0x4($t9)
    ctx->r5 = MEM_BU(ctx->r25, 0X4);
    // 0x800924B0: slt         $at, $v0, $a1
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x800924B4: bnel        $at, $zero, L_800924C8
    if (ctx->r1 != 0) {
        // 0x800924B8: addiu       $a0, $a0, 0x1
        ctx->r4 = ADD32(ctx->r4, 0X1);
            goto L_800924C8;
    }
    goto skip_1;
    // 0x800924B8: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    skip_1:
    // 0x800924BC: andi        $v1, $a0, 0xFF
    ctx->r3 = ctx->r4 & 0XFF;
    // 0x800924C0: andi        $v0, $a1, 0xFF
    ctx->r2 = ctx->r5 & 0XFF;
    // 0x800924C4: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_800924C8:
    // 0x800924C8: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x800924CC: slti        $at, $a0, 0x16
    ctx->r1 = SIGNED(ctx->r4) < 0X16 ? 1 : 0;
    // 0x800924D0: bne         $at, $zero, L_800924A0
    if (ctx->r1 != 0) {
        // 0x800924D4: nop
    
            goto L_800924A0;
    }
    // 0x800924D4: nop

    // 0x800924D8: jr          $ra
    // 0x800924DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x800924DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_800924E0:
    // 0x800924E0: lui         $t1, 0x8029
    ctx->r9 = S32(0X8029 << 16);
    // 0x800924E4: addiu       $v0, $zero, 0xFF
    ctx->r2 = ADD32(0, 0XFF);
    // 0x800924E8: addiu       $t1, $t1, 0xE0
    ctx->r9 = ADD32(ctx->r9, 0XE0);
    // 0x800924EC: addiu       $a0, $zero, 0x12
    ctx->r4 = ADD32(0, 0X12);
    // 0x800924F0: addiu       $a2, $zero, 0x12
    ctx->r6 = ADD32(0, 0X12);
    // 0x800924F4: addiu       $t3, $zero, 0xD8
    ctx->r11 = ADD32(0, 0XD8);
    // 0x800924F8: addiu       $t2, $zero, 0xD8
    ctx->r10 = ADD32(0, 0XD8);
L_800924FC:
    // 0x800924FC: multu       $a0, $t2
    result = U64(U32(ctx->r4)) * U64(U32(ctx->r10)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80092500: andi        $a3, $a0, 0xFF
    ctx->r7 = ctx->r4 & 0XFF;
    // 0x80092504: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80092508: mflo        $t4
    ctx->r12 = lo;
    // 0x8009250C: addu        $t5, $t1, $t4
    ctx->r13 = ADD32(ctx->r9, ctx->r12);
    // 0x80092510: lbu         $t6, 0x4($t5)
    ctx->r14 = MEM_BU(ctx->r13, 0X4);
    // 0x80092514: multu       $a2, $t3
    result = U64(U32(ctx->r6)) * U64(U32(ctx->r11)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80092518: mflo        $t7
    ctx->r15 = lo;
    // 0x8009251C: addu        $t8, $t1, $t7
    ctx->r24 = ADD32(ctx->r9, ctx->r15);
    // 0x80092520: lbu         $t9, 0xDC($t8)
    ctx->r25 = MEM_BU(ctx->r24, 0XDC);
    // 0x80092524: slt         $at, $t6, $t9
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r25) ? 1 : 0;
    // 0x80092528: beq         $at, $zero, L_80092538
    if (ctx->r1 == 0) {
        // 0x8009252C: nop
    
            goto L_80092538;
    }
    // 0x8009252C: nop

    // 0x80092530: addiu       $a3, $a0, 0x1
    ctx->r7 = ADD32(ctx->r4, 0X1);
    // 0x80092534: andi        $a3, $a3, 0xFF
    ctx->r7 = ctx->r7 & 0XFF;
L_80092538:
    // 0x80092538: multu       $a3, $t2
    result = U64(U32(ctx->r7)) * U64(U32(ctx->r10)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8009253C: mflo        $t4
    ctx->r12 = lo;
    // 0x80092540: addu        $t5, $t1, $t4
    ctx->r13 = ADD32(ctx->r9, ctx->r12);
    // 0x80092544: lbu         $t0, 0x4($t5)
    ctx->r8 = MEM_BU(ctx->r13, 0X4);
    // 0x80092548: slt         $at, $a1, $t0
    ctx->r1 = SIGNED(ctx->r5) < SIGNED(ctx->r8) ? 1 : 0;
    // 0x8009254C: bnel        $at, $zero, L_80092560
    if (ctx->r1 != 0) {
        // 0x80092550: addiu       $a0, $a2, -0x2
        ctx->r4 = ADD32(ctx->r6, -0X2);
            goto L_80092560;
    }
    goto skip_2;
    // 0x80092550: addiu       $a0, $a2, -0x2
    ctx->r4 = ADD32(ctx->r6, -0X2);
    skip_2:
    // 0x80092554: andi        $v1, $a0, 0xFF
    ctx->r3 = ctx->r4 & 0XFF;
    // 0x80092558: andi        $v0, $t0, 0xFF
    ctx->r2 = ctx->r8 & 0XFF;
    // 0x8009255C: addiu       $a0, $a2, -0x2
    ctx->r4 = ADD32(ctx->r6, -0X2);
L_80092560:
    // 0x80092560: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x80092564: slti        $at, $a0, 0xA
    ctx->r1 = SIGNED(ctx->r4) < 0XA ? 1 : 0;
    // 0x80092568: beq         $at, $zero, L_800924FC
    if (ctx->r1 == 0) {
        // 0x8009256C: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_800924FC;
    }
    // 0x8009256C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80092570: jr          $ra
    // 0x80092574: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80092574: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_80092578(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80092578: lui         $t6, 0x8014
    ctx->r14 = S32(0X8014 << 16);
    // 0x8009257C: lhu         $t6, -0x16B6($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X16B6);
    // 0x80092580: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80092584: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80092588: beq         $t6, $zero, L_800925A0
    if (ctx->r14 == 0) {
        // 0x8009258C: nop
    
            goto L_800925A0;
    }
    // 0x8009258C: nop

    // 0x80092590: jal         0x80092BE0
    // 0x80092594: nop

    func_80092BE0(rdram, ctx);
        goto after_0;
    // 0x80092594: nop

    after_0:
    // 0x80092598: b           L_800925A8
    // 0x8009259C: nop

        goto L_800925A8;
    // 0x8009259C: nop

L_800925A0:
    // 0x800925A0: jal         0x80092C80
    // 0x800925A4: nop

    func_80092C80(rdram, ctx);
        goto after_1;
    // 0x800925A4: nop

    after_1:
L_800925A8:
    // 0x800925A8: lui         $a2, 0x8014
    ctx->r6 = S32(0X8014 << 16);
    // 0x800925AC: lw          $a2, -0x1620($a2)
    ctx->r6 = MEM_W(ctx->r6, -0X1620);
    // 0x800925B0: andi        $a2, $a2, 0x7FFF
    ctx->r6 = ctx->r6 & 0X7FFF;
    // 0x800925B4: andi        $a2, $a2, 0xFFFF
    ctx->r6 = ctx->r6 & 0XFFFF;
    // 0x800925B8: slti        $at, $a2, 0xC0
    ctx->r1 = SIGNED(ctx->r6) < 0XC0 ? 1 : 0;
    // 0x800925BC: beq         $at, $zero, L_80092604
    if (ctx->r1 == 0) {
        // 0x800925C0: or          $a1, $a2, $zero
        ctx->r5 = ctx->r6 | 0;
            goto L_80092604;
    }
    // 0x800925C0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x800925C4: slti        $at, $a2, 0xB6
    ctx->r1 = SIGNED(ctx->r6) < 0XB6 ? 1 : 0;
    // 0x800925C8: beq         $at, $zero, L_80092660
    if (ctx->r1 == 0) {
        // 0x800925CC: lui         $t7, 0x8014
        ctx->r15 = S32(0X8014 << 16);
            goto L_80092660;
    }
    // 0x800925CC: lui         $t7, 0x8014
    ctx->r15 = S32(0X8014 << 16);
    // 0x800925D0: lbu         $t7, -0x1647($t7)
    ctx->r15 = MEM_BU(ctx->r15, -0X1647);
    // 0x800925D4: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x800925D8: addiu       $t8, $zero, 0x4
    ctx->r24 = ADD32(0, 0X4);
    // 0x800925DC: beq         $t7, $a2, L_800925F4
    if (ctx->r15 == ctx->r6) {
        // 0x800925E0: andi        $a0, $a2, 0xFFFF
        ctx->r4 = ctx->r6 & 0XFFFF;
            goto L_800925F4;
    }
    // 0x800925E0: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x800925E4: sh          $a2, -0x161C($at)
    MEM_H(-0X161C, ctx->r1) = ctx->r6;
    // 0x800925E8: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x800925EC: b           L_80092660
    // 0x800925F0: sb          $t8, -0x161A($at)
    MEM_B(-0X161A, ctx->r1) = ctx->r24;
        goto L_80092660;
    // 0x800925F0: sb          $t8, -0x161A($at)
    MEM_B(-0X161A, ctx->r1) = ctx->r24;
L_800925F4:
    // 0x800925F4: jal         0x80092818
    // 0x800925F8: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    func_80092818(rdram, ctx);
        goto after_2;
    // 0x800925F8: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    after_2:
    // 0x800925FC: b           L_80092664
    // 0x80092600: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80092664;
    // 0x80092600: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80092604:
    // 0x80092604: slti        $at, $a1, 0xC1
    ctx->r1 = SIGNED(ctx->r5) < 0XC1 ? 1 : 0;
    // 0x80092608: beq         $at, $zero, L_80092660
    if (ctx->r1 == 0) {
        // 0x8009260C: lui         $v0, 0x8014
        ctx->r2 = S32(0X8014 << 16);
            goto L_80092660;
    }
    // 0x8009260C: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80092610: addiu       $a2, $a1, -0xC0
    ctx->r6 = ADD32(ctx->r5, -0XC0);
    // 0x80092614: andi        $a2, $a2, 0xFFFF
    ctx->r6 = ctx->r6 & 0XFFFF;
    // 0x80092618: lui         $t0, 0x8027
    ctx->r8 = S32(0X8027 << 16);
    // 0x8009261C: addiu       $t0, $t0, 0x94C
    ctx->r8 = ADD32(ctx->r8, 0X94C);
    // 0x80092620: sll         $t9, $a2, 3
    ctx->r25 = S32(ctx->r6 << 3);
    // 0x80092624: addiu       $v0, $v0, -0x1690
    ctx->r2 = ADD32(ctx->r2, -0X1690);
    // 0x80092628: addu        $v1, $t9, $t0
    ctx->r3 = ADD32(ctx->r25, ctx->r8);
    // 0x8009262C: lbu         $t1, 0x0($v1)
    ctx->r9 = MEM_BU(ctx->r3, 0X0);
    // 0x80092630: lbu         $t2, 0x1($v1)
    ctx->r10 = MEM_BU(ctx->r3, 0X1);
    // 0x80092634: lw          $t3, 0x4($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X4);
    // 0x80092638: addiu       $t4, $zero, 0x1E
    ctx->r12 = ADD32(0, 0X1E);
    // 0x8009263C: addiu       $t5, $zero, 0x6
    ctx->r13 = ADD32(0, 0X6);
    // 0x80092640: sb          $zero, 0x1($v0)
    MEM_B(0X1, ctx->r2) = 0;
    // 0x80092644: sb          $t4, 0x2($v0)
    MEM_B(0X2, ctx->r2) = ctx->r12;
    // 0x80092648: sb          $t5, 0x4($v0)
    MEM_B(0X4, ctx->r2) = ctx->r13;
    // 0x8009264C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80092650: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80092654: sb          $t2, 0x3($v0)
    MEM_B(0X3, ctx->r2) = ctx->r10;
    // 0x80092658: jal         0x800928E0
    // 0x8009265C: sw          $t3, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r11;
    func_800928E0(rdram, ctx);
        goto after_3;
    // 0x8009265C: sw          $t3, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r11;
    after_3:
L_80092660:
    // 0x80092660: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80092664:
    // 0x80092664: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80092668: jr          $ra
    // 0x8009266C: nop

    return;
    // 0x8009266C: nop

;}
RECOMP_FUNC void func_80092670(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80092670: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80092674: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x80092678: sll         $t6, $a0, 1
    ctx->r14 = S32(ctx->r4 << 1);
    // 0x8009267C: addu        $a1, $a1, $t6
    ctx->r5 = ADD32(ctx->r5, ctx->r14);
    // 0x80092680: lbu         $v0, 0x0($a1)
    ctx->r2 = MEM_BU(ctx->r5, 0X0);
    // 0x80092684: lbu         $t7, 0x1($a1)
    ctx->r15 = MEM_BU(ctx->r5, 0X1);
    // 0x80092688: lbu         $a2, 0x2($a1)
    ctx->r6 = MEM_BU(ctx->r5, 0X2);
    // 0x8009268C: lbu         $t8, 0x3($a1)
    ctx->r24 = MEM_BU(ctx->r5, 0X3);
    // 0x80092690: sll         $v0, $v0, 8
    ctx->r2 = S32(ctx->r2 << 8);
    // 0x80092694: addu        $v0, $v0, $t7
    ctx->r2 = ADD32(ctx->r2, ctx->r15);
    // 0x80092698: sll         $a2, $a2, 8
    ctx->r6 = S32(ctx->r6 << 8);
    // 0x8009269C: andi        $v1, $v0, 0xFFFF
    ctx->r3 = ctx->r2 & 0XFFFF;
    // 0x800926A0: addu        $a2, $a2, $t8
    ctx->r6 = ADD32(ctx->r6, ctx->r24);
    // 0x800926A4: andi        $a2, $a2, 0xFFFF
    ctx->r6 = ctx->r6 & 0XFFFF;
    // 0x800926A8: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x800926AC: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x800926B0: blez        $v1, L_800926D8
    if (SIGNED(ctx->r3) <= 0) {
        // 0x800926B4: or          $t0, $zero, $zero
        ctx->r8 = 0 | 0;
            goto L_800926D8;
    }
    // 0x800926B4: or          $t0, $zero, $zero
    ctx->r8 = 0 | 0;
L_800926B8:
    // 0x800926B8: lbu         $t9, 0x0($a1)
    ctx->r25 = MEM_BU(ctx->r5, 0X0);
    // 0x800926BC: addiu       $t0, $t0, 0x1
    ctx->r8 = ADD32(ctx->r8, 0X1);
    // 0x800926C0: andi        $t0, $t0, 0xFFFF
    ctx->r8 = ctx->r8 & 0XFFFF;
    // 0x800926C4: slt         $at, $t0, $v1
    ctx->r1 = SIGNED(ctx->r8) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x800926C8: addu        $a3, $a3, $t9
    ctx->r7 = ADD32(ctx->r7, ctx->r25);
    // 0x800926CC: andi        $a3, $a3, 0xFFFF
    ctx->r7 = ctx->r7 & 0XFFFF;
    // 0x800926D0: bne         $at, $zero, L_800926B8
    if (ctx->r1 != 0) {
        // 0x800926D4: addiu       $a1, $a1, 0x1
        ctx->r5 = ADD32(ctx->r5, 0X1);
            goto L_800926B8;
    }
    // 0x800926D4: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
L_800926D8:
    // 0x800926D8: bne         $a3, $a2, L_800926F4
    if (ctx->r7 != ctx->r6) {
        // 0x800926DC: lui         $v1, 0x800C
        ctx->r3 = S32(0X800C << 16);
            goto L_800926F4;
    }
    // 0x800926DC: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x800926E0: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x800926E4: addiu       $v1, $v1, -0x7340
    ctx->r3 = ADD32(ctx->r3, -0X7340);
    // 0x800926E8: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
    // 0x800926EC: jr          $ra
    // 0x800926F0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x800926F0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800926F4:
    // 0x800926F4: addiu       $v1, $v1, -0x7340
    ctx->r3 = ADD32(ctx->r3, -0X7340);
    // 0x800926F8: lw          $t1, 0x0($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X0);
    // 0x800926FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80092700: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80092704: sw          $t2, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r10;
    // 0x80092708: jr          $ra
    // 0x8009270C: nop

    return;
    // 0x8009270C: nop

;}
RECOMP_FUNC void func_80092710(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80092710: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x80092714: addiu       $t0, $t0, -0x161A
    ctx->r8 = ADD32(ctx->r8, -0X161A);
    // 0x80092718: lbu         $t6, 0x0($t0)
    ctx->r14 = MEM_BU(ctx->r8, 0X0);
    // 0x8009271C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80092720: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80092724: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x80092728: andi        $t8, $t7, 0xFF
    ctx->r24 = ctx->r15 & 0XFF;
    // 0x8009272C: bne         $t8, $zero, L_80092808
    if (ctx->r24 != 0) {
        // 0x80092730: sb          $t7, 0x0($t0)
        MEM_B(0X0, ctx->r8) = ctx->r15;
            goto L_80092808;
    }
    // 0x80092730: sb          $t7, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r15;
    // 0x80092734: lui         $t9, 0x800D
    ctx->r25 = S32(0X800D << 16);
    // 0x80092738: lbu         $t9, -0x1898($t9)
    ctx->r25 = MEM_BU(ctx->r25, -0X1898);
    // 0x8009273C: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x80092740: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80092744: beq         $t9, $zero, L_80092758
    if (ctx->r25 == 0) {
        // 0x80092748: lui         $a0, 0x800C
        ctx->r4 = S32(0X800C << 16);
            goto L_80092758;
    }
    // 0x80092748: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8009274C: addiu       $t1, $zero, 0x1
    ctx->r9 = ADD32(0, 0X1);
    // 0x80092750: b           L_80092808
    // 0x80092754: sb          $t1, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r9;
        goto L_80092808;
    // 0x80092754: sb          $t1, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r9;
L_80092758:
    // 0x80092758: lhu         $v0, -0x161C($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X161C);
    // 0x8009275C: lui         $t2, 0x800C
    ctx->r10 = S32(0X800C << 16);
    // 0x80092760: lui         $a1, 0x8029
    ctx->r5 = S32(0X8029 << 16);
    // 0x80092764: addiu       $a3, $v0, -0x10
    ctx->r7 = ADD32(ctx->r2, -0X10);
    // 0x80092768: andi        $a3, $a3, 0xFFFF
    ctx->r7 = ctx->r7 & 0XFFFF;
    // 0x8009276C: sll         $v1, $a3, 2
    ctx->r3 = S32(ctx->r7 << 2);
    // 0x80092770: addu        $a0, $a0, $v1
    ctx->r4 = ADD32(ctx->r4, ctx->r3);
    // 0x80092774: addu        $t2, $t2, $v1
    ctx->r10 = ADD32(ctx->r10, ctx->r3);
    // 0x80092778: lw          $t2, -0x7620($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X7620);
    // 0x8009277C: lw          $a0, -0x78B8($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X78B8);
    // 0x80092780: sh          $a3, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r7;
    // 0x80092784: addiu       $a1, $a1, 0x1370
    ctx->r5 = ADD32(ctx->r5, 0X1370);
    // 0x80092788: sb          $v0, -0x1647($at)
    MEM_B(-0X1647, ctx->r1) = ctx->r2;
    // 0x8009278C: jal         0x800199C0
    // 0x80092790: subu        $a2, $t2, $a0
    ctx->r6 = SUB32(ctx->r10, ctx->r4);
    func_800199C0(rdram, ctx);
        goto after_0;
    // 0x80092790: subu        $a2, $t2, $a0
    ctx->r6 = SUB32(ctx->r10, ctx->r4);
    after_0:
    // 0x80092794: lhu         $a3, 0x1E($sp)
    ctx->r7 = MEM_HU(ctx->r29, 0X1E);
    // 0x80092798: lui         $a0, 0x8027
    ctx->r4 = S32(0X8027 << 16);
    // 0x8009279C: lui         $a1, 0x8029
    ctx->r5 = S32(0X8029 << 16);
    // 0x800927A0: sll         $t3, $a3, 1
    ctx->r11 = S32(ctx->r7 << 1);
    // 0x800927A4: addu        $a0, $a0, $t3
    ctx->r4 = ADD32(ctx->r4, ctx->r11);
    // 0x800927A8: lbu         $a0, 0x800($a0)
    ctx->r4 = MEM_BU(ctx->r4, 0X800);
    // 0x800927AC: jal         0x80092670
    // 0x800927B0: addiu       $a1, $a1, 0x1370
    ctx->r5 = ADD32(ctx->r5, 0X1370);
    func_80092670(rdram, ctx);
        goto after_1;
    // 0x800927B0: addiu       $a1, $a1, 0x1370
    ctx->r5 = ADD32(ctx->r5, 0X1370);
    after_1:
    // 0x800927B4: lui         $t0, 0x8014
    ctx->r8 = S32(0X8014 << 16);
    // 0x800927B8: bne         $v0, $zero, L_800927F0
    if (ctx->r2 != 0) {
        // 0x800927BC: addiu       $t0, $t0, -0x161A
        ctx->r8 = ADD32(ctx->r8, -0X161A);
            goto L_800927F0;
    }
    // 0x800927BC: addiu       $t0, $t0, -0x161A
    ctx->r8 = ADD32(ctx->r8, -0X161A);
    // 0x800927C0: lui         $t4, 0x800C
    ctx->r12 = S32(0X800C << 16);
    // 0x800927C4: lw          $t4, -0x7340($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X7340);
    // 0x800927C8: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x800927CC: slti        $at, $t4, 0x5
    ctx->r1 = SIGNED(ctx->r12) < 0X5 ? 1 : 0;
    // 0x800927D0: bne         $at, $zero, L_800927E8
    if (ctx->r1 != 0) {
        // 0x800927D4: lui         $at, 0x8014
        ctx->r1 = S32(0X8014 << 16);
            goto L_800927E8;
    }
    // 0x800927D4: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x800927D8: sh          $zero, -0x161C($at)
    MEM_H(-0X161C, ctx->r1) = 0;
    // 0x800927DC: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x800927E0: b           L_80092808
    // 0x800927E4: sb          $zero, -0x1647($at)
    MEM_B(-0X1647, ctx->r1) = 0;
        goto L_80092808;
    // 0x800927E4: sb          $zero, -0x1647($at)
    MEM_B(-0X1647, ctx->r1) = 0;
L_800927E8:
    // 0x800927E8: b           L_80092808
    // 0x800927EC: sb          $t5, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r13;
        goto L_80092808;
    // 0x800927EC: sb          $t5, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r13;
L_800927F0:
    // 0x800927F0: lui         $a0, 0x8014
    ctx->r4 = S32(0X8014 << 16);
    // 0x800927F4: lhu         $a0, -0x161C($a0)
    ctx->r4 = MEM_HU(ctx->r4, -0X161C);
    // 0x800927F8: jal         0x80092818
    // 0x800927FC: addiu       $a1, $zero, 0x5
    ctx->r5 = ADD32(0, 0X5);
    func_80092818(rdram, ctx);
        goto after_2;
    // 0x800927FC: addiu       $a1, $zero, 0x5
    ctx->r5 = ADD32(0, 0X5);
    after_2:
    // 0x80092800: lui         $at, 0x8014
    ctx->r1 = S32(0X8014 << 16);
    // 0x80092804: sh          $zero, -0x161C($at)
    MEM_H(-0X161C, ctx->r1) = 0;
L_80092808:
    // 0x80092808: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8009280C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80092810: jr          $ra
    // 0x80092814: nop

    return;
    // 0x80092814: nop

;}
