#include "recomp.h"
#include "funcs.h"

RECOMP_FUNC void func_80141B70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141B70: lui         $t6, 0x801A
    ctx->r14 = S32(0X801A << 16);
    // 0x80141B74: lw          $t6, -0x2E90($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X2E90);
    // 0x80141B78: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80141B7C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80141B80: beq         $t6, $zero, L_80141BDC
    if (ctx->r14 == 0) {
        // 0x80141B84: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_80141BDC;
    }
    // 0x80141B84: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80141B88: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80141B8C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80141B90: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80141B94: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80141B98: jalr        $t9
    // 0x80141B9C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80141B9C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x80141BA0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80141BA4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80141BA8: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x80141BAC: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x80141BB0: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x80141BB4: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80141BB8: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80141BBC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80141BC0: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80141BC4: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80141BC8: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80141BCC: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80141BD0: lw          $t9, -0x37A0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X37A0);
    // 0x80141BD4: jalr        $t9
    // 0x80141BD8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80141BD8: nop

    after_1:
L_80141BDC:
    // 0x80141BDC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80141BE0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80141BE4: jr          $ra
    // 0x80141BE8: nop

    return;
    // 0x80141BE8: nop

;}
RECOMP_FUNC void func_80141BEC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141BEC: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80141BF0: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80141BF4: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80141BF8: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80141BFC: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80141C00: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80141C04: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x80141C08: lw          $t7, 0x38($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X38);
    // 0x80141C0C: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80141C10: addiu       $s3, $zero, -0x1
    ctx->r19 = ADD32(0, -0X1);
    // 0x80141C14: beq         $t7, $zero, L_80141CD0
    if (ctx->r15 == 0) {
        // 0x80141C18: or          $s1, $t7, $zero
        ctx->r17 = ctx->r15 | 0;
            goto L_80141CD0;
    }
    // 0x80141C18: or          $s1, $t7, $zero
    ctx->r17 = ctx->r15 | 0;
    // 0x80141C1C: addiu       $s2, $s2, -0x2EB0
    ctx->r18 = ADD32(ctx->r18, -0X2EB0);
    // 0x80141C20: lh          $t8, 0x10($s1)
    ctx->r24 = MEM_H(ctx->r17, 0X10);
L_80141C24:
    // 0x80141C24: beql        $s3, $t8, L_80141CBC
    if (ctx->r19 == ctx->r24) {
        // 0x80141C28: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141CBC;
    }
    goto skip_0;
    // 0x80141C28: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_0:
    // 0x80141C2C: lhu         $t9, 0x2($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0X2);
    // 0x80141C30: andi        $t0, $t9, 0x8000
    ctx->r8 = ctx->r25 & 0X8000;
    // 0x80141C34: bnel        $t0, $zero, L_80141CBC
    if (ctx->r8 != 0) {
        // 0x80141C38: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141CBC;
    }
    goto skip_1;
    // 0x80141C38: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_1:
    // 0x80141C3C: jal         0x801420A8
    // 0x80141C40: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_801420A8(rdram, ctx);
        goto after_0;
    // 0x80141C40: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_0:
    // 0x80141C44: beql        $v0, $zero, L_80141CBC
    if (ctx->r2 == 0) {
        // 0x80141C48: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141CBC;
    }
    goto skip_2;
    // 0x80141C48: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_2:
    // 0x80141C4C: lh          $s0, 0x10($s1)
    ctx->r16 = MEM_H(ctx->r17, 0X10);
    // 0x80141C50: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80141C54: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80141C58: slti        $at, $s0, 0x112
    ctx->r1 = SIGNED(ctx->r16) < 0X112 ? 1 : 0;
    // 0x80141C5C: bne         $at, $zero, L_80141C98
    if (ctx->r1 != 0) {
        // 0x80141C60: sll         $a1, $s0, 16
        ctx->r5 = S32(ctx->r16 << 16);
            goto L_80141C98;
    }
    // 0x80141C60: sll         $a1, $s0, 16
    ctx->r5 = S32(ctx->r16 << 16);
    // 0x80141C64: slti        $at, $s0, 0x127
    ctx->r1 = SIGNED(ctx->r16) < 0X127 ? 1 : 0;
    // 0x80141C68: beql        $at, $zero, L_80141C9C
    if (ctx->r1 == 0) {
        // 0x80141C6C: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_80141C9C;
    }
    goto skip_3;
    // 0x80141C6C: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    skip_3:
    // 0x80141C70: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80141C74: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80141C78: jal         0x80059CB8
    // 0x80141C7C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80059CB8(rdram, ctx);
        goto after_1;
    // 0x80141C7C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_1:
    // 0x80141C80: beql        $v0, $zero, L_80141CBC
    if (ctx->r2 == 0) {
        // 0x80141C84: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141CBC;
    }
    goto skip_4;
    // 0x80141C84: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_4:
    // 0x80141C88: lw          $t1, 0xC($s2)
    ctx->r9 = MEM_W(ctx->r18, 0XC);
    // 0x80141C8C: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80141C90: b           L_80141CB8
    // 0x80141C94: sw          $t2, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r10;
        goto L_80141CB8;
    // 0x80141C94: sw          $t2, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r10;
L_80141C98:
    // 0x80141C98: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
L_80141C9C:
    // 0x80141C9C: jal         0x80142FA8
    // 0x80141CA0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    func_80142FA8(rdram, ctx);
        goto after_2;
    // 0x80141CA0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_2:
    // 0x80141CA4: beql        $v0, $zero, L_80141CBC
    if (ctx->r2 == 0) {
        // 0x80141CA8: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141CBC;
    }
    goto skip_5;
    // 0x80141CA8: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_5:
    // 0x80141CAC: lw          $t3, 0x4($s2)
    ctx->r11 = MEM_W(ctx->r18, 0X4);
    // 0x80141CB0: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x80141CB4: sw          $t4, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r12;
L_80141CB8:
    // 0x80141CB8: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
L_80141CBC:
    // 0x80141CBC: addiu       $s1, $s1, 0x20
    ctx->r17 = ADD32(ctx->r17, 0X20);
    // 0x80141CC0: andi        $v0, $v0, 0x8000
    ctx->r2 = ctx->r2 & 0X8000;
    // 0x80141CC4: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x80141CC8: bnel        $v0, $zero, L_80141C24
    if (ctx->r2 != 0) {
        // 0x80141CCC: lh          $t8, 0x10($s1)
        ctx->r24 = MEM_H(ctx->r17, 0X10);
            goto L_80141C24;
    }
    goto skip_6;
    // 0x80141CCC: lh          $t8, 0x10($s1)
    ctx->r24 = MEM_H(ctx->r17, 0X10);
    skip_6:
L_80141CD0:
    // 0x80141CD0: lw          $t5, 0x28($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X28);
    // 0x80141CD4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80141CD8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80141CDC: addiu       $a0, $t5, 0x8
    ctx->r4 = ADD32(ctx->r13, 0X8);
    // 0x80141CE0: jalr        $t9
    // 0x80141CE4: addiu       $a1, $t5, 0xE
    ctx->r5 = ADD32(ctx->r13, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80141CE4: addiu       $a1, $t5, 0xE
    ctx->r5 = ADD32(ctx->r13, 0XE);
    after_3:
    // 0x80141CE8: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80141CEC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80141CF0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80141CF4: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80141CF8: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80141CFC: jr          $ra
    // 0x80141D00: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80141D00: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80141D04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141D04: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80141D08: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80141D0C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80141D10: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80141D14: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80141D18: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80141D1C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80141D20: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80141D24: lw          $t7, 0x34($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X34);
    // 0x80141D28: addiu       $s3, $zero, -0x1
    ctx->r19 = ADD32(0, -0X1);
    // 0x80141D2C: addiu       $s4, $zero, -0x1001
    ctx->r20 = ADD32(0, -0X1001);
    // 0x80141D30: beq         $t7, $zero, L_80141E20
    if (ctx->r15 == 0) {
        // 0x80141D34: or          $s1, $t7, $zero
        ctx->r17 = ctx->r15 | 0;
            goto L_80141E20;
    }
    // 0x80141D34: or          $s1, $t7, $zero
    ctx->r17 = ctx->r15 | 0;
    // 0x80141D38: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80141D3C: addiu       $s2, $s2, -0x2EB0
    ctx->r18 = ADD32(ctx->r18, -0X2EB0);
    // 0x80141D40: lh          $t8, 0x10($s1)
    ctx->r24 = MEM_H(ctx->r17, 0X10);
L_80141D44:
    // 0x80141D44: beql        $s3, $t8, L_80141E0C
    if (ctx->r19 == ctx->r24) {
        // 0x80141D48: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141E0C;
    }
    goto skip_0;
    // 0x80141D48: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_0:
    // 0x80141D4C: lhu         $t9, 0x2($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0X2);
    // 0x80141D50: andi        $t0, $t9, 0x8000
    ctx->r8 = ctx->r25 & 0X8000;
    // 0x80141D54: bnel        $t0, $zero, L_80141E0C
    if (ctx->r8 != 0) {
        // 0x80141D58: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141E0C;
    }
    goto skip_1;
    // 0x80141D58: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_1:
    // 0x80141D5C: jal         0x80142388
    // 0x80141D60: addiu       $a0, $s1, 0x4
    ctx->r4 = ADD32(ctx->r17, 0X4);
    func_80142388(rdram, ctx);
        goto after_0;
    // 0x80141D60: addiu       $a0, $s1, 0x4
    ctx->r4 = ADD32(ctx->r17, 0X4);
    after_0:
    // 0x80141D64: beql        $v0, $zero, L_80141D80
    if (ctx->r2 == 0) {
        // 0x80141D68: lhu         $t3, 0x2($s1)
        ctx->r11 = MEM_HU(ctx->r17, 0X2);
            goto L_80141D80;
    }
    goto skip_2;
    // 0x80141D68: lhu         $t3, 0x2($s1)
    ctx->r11 = MEM_HU(ctx->r17, 0X2);
    skip_2:
    // 0x80141D6C: lhu         $t1, 0x2($s1)
    ctx->r9 = MEM_HU(ctx->r17, 0X2);
    // 0x80141D70: and         $t2, $t1, $s4
    ctx->r10 = ctx->r9 & ctx->r20;
    // 0x80141D74: b           L_80141E08
    // 0x80141D78: sh          $t2, 0x2($s1)
    MEM_H(0X2, ctx->r17) = ctx->r10;
        goto L_80141E08;
    // 0x80141D78: sh          $t2, 0x2($s1)
    MEM_H(0X2, ctx->r17) = ctx->r10;
    // 0x80141D7C: lhu         $t3, 0x2($s1)
    ctx->r11 = MEM_HU(ctx->r17, 0X2);
L_80141D80:
    // 0x80141D80: andi        $t4, $t3, 0x1000
    ctx->r12 = ctx->r11 & 0X1000;
    // 0x80141D84: bnel        $t4, $zero, L_80141E0C
    if (ctx->r12 != 0) {
        // 0x80141D88: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141E0C;
    }
    goto skip_3;
    // 0x80141D88: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_3:
    // 0x80141D8C: jal         0x801420A8
    // 0x80141D90: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_801420A8(rdram, ctx);
        goto after_1;
    // 0x80141D90: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_1:
    // 0x80141D94: beql        $v0, $zero, L_80141E0C
    if (ctx->r2 == 0) {
        // 0x80141D98: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141E0C;
    }
    goto skip_4;
    // 0x80141D98: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_4:
    // 0x80141D9C: lh          $s0, 0x10($s1)
    ctx->r16 = MEM_H(ctx->r17, 0X10);
    // 0x80141DA0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80141DA4: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80141DA8: slti        $at, $s0, 0x112
    ctx->r1 = SIGNED(ctx->r16) < 0X112 ? 1 : 0;
    // 0x80141DAC: bne         $at, $zero, L_80141DE8
    if (ctx->r1 != 0) {
        // 0x80141DB0: sll         $a1, $s0, 16
        ctx->r5 = S32(ctx->r16 << 16);
            goto L_80141DE8;
    }
    // 0x80141DB0: sll         $a1, $s0, 16
    ctx->r5 = S32(ctx->r16 << 16);
    // 0x80141DB4: slti        $at, $s0, 0x127
    ctx->r1 = SIGNED(ctx->r16) < 0X127 ? 1 : 0;
    // 0x80141DB8: beql        $at, $zero, L_80141DEC
    if (ctx->r1 == 0) {
        // 0x80141DBC: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_80141DEC;
    }
    goto skip_5;
    // 0x80141DBC: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    skip_5:
    // 0x80141DC0: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80141DC4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80141DC8: jal         0x80059CB8
    // 0x80141DCC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80059CB8(rdram, ctx);
        goto after_2;
    // 0x80141DCC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_2:
    // 0x80141DD0: beql        $v0, $zero, L_80141E0C
    if (ctx->r2 == 0) {
        // 0x80141DD4: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141E0C;
    }
    goto skip_6;
    // 0x80141DD4: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_6:
    // 0x80141DD8: lw          $t5, 0xC($s2)
    ctx->r13 = MEM_W(ctx->r18, 0XC);
    // 0x80141DDC: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x80141DE0: b           L_80141E08
    // 0x80141DE4: sw          $t6, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r14;
        goto L_80141E08;
    // 0x80141DE4: sw          $t6, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r14;
L_80141DE8:
    // 0x80141DE8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
L_80141DEC:
    // 0x80141DEC: jal         0x80142FA8
    // 0x80141DF0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    func_80142FA8(rdram, ctx);
        goto after_3;
    // 0x80141DF0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_3:
    // 0x80141DF4: beql        $v0, $zero, L_80141E0C
    if (ctx->r2 == 0) {
        // 0x80141DF8: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141E0C;
    }
    goto skip_7;
    // 0x80141DF8: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_7:
    // 0x80141DFC: lw          $t7, 0x4($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X4);
    // 0x80141E00: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80141E04: sw          $t8, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r24;
L_80141E08:
    // 0x80141E08: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
L_80141E0C:
    // 0x80141E0C: addiu       $s1, $s1, 0x20
    ctx->r17 = ADD32(ctx->r17, 0X20);
    // 0x80141E10: andi        $v0, $v0, 0x8000
    ctx->r2 = ctx->r2 & 0X8000;
    // 0x80141E14: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x80141E18: bnel        $v0, $zero, L_80141D44
    if (ctx->r2 != 0) {
        // 0x80141E1C: lh          $t8, 0x10($s1)
        ctx->r24 = MEM_H(ctx->r17, 0X10);
            goto L_80141D44;
    }
    goto skip_8;
    // 0x80141E1C: lh          $t8, 0x10($s1)
    ctx->r24 = MEM_H(ctx->r17, 0X10);
    skip_8:
L_80141E20:
    // 0x80141E20: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x80141E24: lw          $t1, 0x30($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X30);
    // 0x80141E28: lw          $t0, 0x34($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X34);
    // 0x80141E2C: addiu       $a0, $t1, 0x8
    ctx->r4 = ADD32(ctx->r9, 0X8);
    // 0x80141E30: bne         $t0, $zero, L_80141E50
    if (ctx->r8 != 0) {
        // 0x80141E34: nop
    
            goto L_80141E50;
    }
    // 0x80141E34: nop

    // 0x80141E38: or          $a0, $t9, $zero
    ctx->r4 = ctx->r25 | 0;
    // 0x80141E3C: lw          $t9, 0x10($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X10);
    // 0x80141E40: jalr        $t9
    // 0x80141E44: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80141E44: nop

    after_4:
    // 0x80141E48: b           L_80141E64
    // 0x80141E4C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
        goto L_80141E64;
    // 0x80141E4C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80141E50:
    // 0x80141E50: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80141E54: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80141E58: jalr        $t9
    // 0x80141E5C: addiu       $a1, $t1, 0xE
    ctx->r5 = ADD32(ctx->r9, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80141E5C: addiu       $a1, $t1, 0xE
    ctx->r5 = ADD32(ctx->r9, 0XE);
    after_5:
    // 0x80141E60: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80141E64:
    // 0x80141E64: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80141E68: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80141E6C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80141E70: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80141E74: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80141E78: jr          $ra
    // 0x80141E7C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80141E7C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80141E80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141E80: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80141E84: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80141E88: lw          $t6, 0x38($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X38);
    // 0x80141E8C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80141E90: beql        $t6, $zero, L_80141EB0
    if (ctx->r14 == 0) {
        // 0x80141E94: lw          $a0, 0x34($a2)
        ctx->r4 = MEM_W(ctx->r6, 0X34);
            goto L_80141EB0;
    }
    goto skip_0;
    // 0x80141E94: lw          $a0, 0x34($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X34);
    skip_0:
    // 0x80141E98: lw          $a0, 0x38($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X38);
    // 0x80141E9C: jal         0x801424C4
    // 0x80141EA0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_801424C4(rdram, ctx);
        goto after_0;
    // 0x80141EA0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x80141EA4: bne         $v0, $zero, L_80141EF0
    if (ctx->r2 != 0) {
        // 0x80141EA8: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_80141EF0;
    }
    // 0x80141EA8: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80141EAC: lw          $a0, 0x34($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X34);
L_80141EB0:
    // 0x80141EB0: beq         $a0, $zero, L_80141EC8
    if (ctx->r4 == 0) {
        // 0x80141EB4: nop
    
            goto L_80141EC8;
    }
    // 0x80141EB4: nop

    // 0x80141EB8: jal         0x801424C4
    // 0x80141EBC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_801424C4(rdram, ctx);
        goto after_1;
    // 0x80141EBC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_1:
    // 0x80141EC0: bne         $v0, $zero, L_80141EF0
    if (ctx->r2 != 0) {
        // 0x80141EC4: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_80141EF0;
    }
    // 0x80141EC4: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
L_80141EC8:
    // 0x80141EC8: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80141ECC: addiu       $v0, $v0, -0x2E80
    ctx->r2 = ADD32(ctx->r2, -0X2E80);
    // 0x80141ED0: lw          $t7, 0x18($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X18);
    // 0x80141ED4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80141ED8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80141EDC: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80141EE0: sw          $t8, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r24;
    // 0x80141EE4: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80141EE8: jalr        $t9
    // 0x80141EEC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80141EEC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_2:
L_80141EF0:
    // 0x80141EF0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80141EF4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80141EF8: jr          $ra
    // 0x80141EFC: nop

    return;
    // 0x80141EFC: nop

;}
RECOMP_FUNC void func_80141F00(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80141F00: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80141F04: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80141F08: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80141F0C: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80141F10: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80141F14: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80141F18: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80141F1C: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80141F20: lw          $s1, 0x34($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X34);
    // 0x80141F24: addiu       $s2, $s2, -0x2EB0
    ctx->r18 = ADD32(ctx->r18, -0X2EB0);
    // 0x80141F28: addiu       $s3, $zero, -0x1
    ctx->r19 = ADD32(0, -0X1);
    // 0x80141F2C: addiu       $s4, $zero, -0x1001
    ctx->r20 = ADD32(0, -0X1001);
    // 0x80141F30: lh          $t6, 0x10($s1)
    ctx->r14 = MEM_H(ctx->r17, 0X10);
L_80141F34:
    // 0x80141F34: beql        $s3, $t6, L_80141FFC
    if (ctx->r19 == ctx->r14) {
        // 0x80141F38: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141FFC;
    }
    goto skip_0;
    // 0x80141F38: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_0:
    // 0x80141F3C: lhu         $t7, 0x2($s1)
    ctx->r15 = MEM_HU(ctx->r17, 0X2);
    // 0x80141F40: andi        $t8, $t7, 0x8000
    ctx->r24 = ctx->r15 & 0X8000;
    // 0x80141F44: bnel        $t8, $zero, L_80141FFC
    if (ctx->r24 != 0) {
        // 0x80141F48: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141FFC;
    }
    goto skip_1;
    // 0x80141F48: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_1:
    // 0x80141F4C: jal         0x80142388
    // 0x80141F50: addiu       $a0, $s1, 0x4
    ctx->r4 = ADD32(ctx->r17, 0X4);
    func_80142388(rdram, ctx);
        goto after_0;
    // 0x80141F50: addiu       $a0, $s1, 0x4
    ctx->r4 = ADD32(ctx->r17, 0X4);
    after_0:
    // 0x80141F54: beql        $v0, $zero, L_80141F70
    if (ctx->r2 == 0) {
        // 0x80141F58: lhu         $t1, 0x2($s1)
        ctx->r9 = MEM_HU(ctx->r17, 0X2);
            goto L_80141F70;
    }
    goto skip_2;
    // 0x80141F58: lhu         $t1, 0x2($s1)
    ctx->r9 = MEM_HU(ctx->r17, 0X2);
    skip_2:
    // 0x80141F5C: lhu         $t9, 0x2($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0X2);
    // 0x80141F60: and         $t0, $t9, $s4
    ctx->r8 = ctx->r25 & ctx->r20;
    // 0x80141F64: b           L_80141FF8
    // 0x80141F68: sh          $t0, 0x2($s1)
    MEM_H(0X2, ctx->r17) = ctx->r8;
        goto L_80141FF8;
    // 0x80141F68: sh          $t0, 0x2($s1)
    MEM_H(0X2, ctx->r17) = ctx->r8;
    // 0x80141F6C: lhu         $t1, 0x2($s1)
    ctx->r9 = MEM_HU(ctx->r17, 0X2);
L_80141F70:
    // 0x80141F70: andi        $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 & 0X1000;
    // 0x80141F74: bnel        $t2, $zero, L_80141FFC
    if (ctx->r10 != 0) {
        // 0x80141F78: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141FFC;
    }
    goto skip_3;
    // 0x80141F78: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_3:
    // 0x80141F7C: jal         0x801420A8
    // 0x80141F80: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_801420A8(rdram, ctx);
        goto after_1;
    // 0x80141F80: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_1:
    // 0x80141F84: beql        $v0, $zero, L_80141FFC
    if (ctx->r2 == 0) {
        // 0x80141F88: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141FFC;
    }
    goto skip_4;
    // 0x80141F88: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_4:
    // 0x80141F8C: lh          $s0, 0x10($s1)
    ctx->r16 = MEM_H(ctx->r17, 0X10);
    // 0x80141F90: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80141F94: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80141F98: slti        $at, $s0, 0x112
    ctx->r1 = SIGNED(ctx->r16) < 0X112 ? 1 : 0;
    // 0x80141F9C: bne         $at, $zero, L_80141FD8
    if (ctx->r1 != 0) {
        // 0x80141FA0: sll         $a1, $s0, 16
        ctx->r5 = S32(ctx->r16 << 16);
            goto L_80141FD8;
    }
    // 0x80141FA0: sll         $a1, $s0, 16
    ctx->r5 = S32(ctx->r16 << 16);
    // 0x80141FA4: slti        $at, $s0, 0x127
    ctx->r1 = SIGNED(ctx->r16) < 0X127 ? 1 : 0;
    // 0x80141FA8: beql        $at, $zero, L_80141FDC
    if (ctx->r1 == 0) {
        // 0x80141FAC: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_80141FDC;
    }
    goto skip_5;
    // 0x80141FAC: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    skip_5:
    // 0x80141FB0: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80141FB4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80141FB8: jal         0x80059CB8
    // 0x80141FBC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    func_80059CB8(rdram, ctx);
        goto after_2;
    // 0x80141FBC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_2:
    // 0x80141FC0: beql        $v0, $zero, L_80141FFC
    if (ctx->r2 == 0) {
        // 0x80141FC4: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141FFC;
    }
    goto skip_6;
    // 0x80141FC4: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_6:
    // 0x80141FC8: lw          $t3, 0xC($s2)
    ctx->r11 = MEM_W(ctx->r18, 0XC);
    // 0x80141FCC: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x80141FD0: b           L_80141FF8
    // 0x80141FD4: sw          $t4, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r12;
        goto L_80141FF8;
    // 0x80141FD4: sw          $t4, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r12;
L_80141FD8:
    // 0x80141FD8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
L_80141FDC:
    // 0x80141FDC: jal         0x80142FA8
    // 0x80141FE0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    func_80142FA8(rdram, ctx);
        goto after_3;
    // 0x80141FE0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_3:
    // 0x80141FE4: beql        $v0, $zero, L_80141FFC
    if (ctx->r2 == 0) {
        // 0x80141FE8: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80141FFC;
    }
    goto skip_7;
    // 0x80141FE8: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_7:
    // 0x80141FEC: lw          $t5, 0x4($s2)
    ctx->r13 = MEM_W(ctx->r18, 0X4);
    // 0x80141FF0: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x80141FF4: sw          $t6, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r14;
L_80141FF8:
    // 0x80141FF8: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
L_80141FFC:
    // 0x80141FFC: addiu       $s1, $s1, 0x20
    ctx->r17 = ADD32(ctx->r17, 0X20);
    // 0x80142000: andi        $v0, $v0, 0x8000
    ctx->r2 = ctx->r2 & 0X8000;
    // 0x80142004: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x80142008: bnel        $v0, $zero, L_80141F34
    if (ctx->r2 != 0) {
        // 0x8014200C: lh          $t6, 0x10($s1)
        ctx->r14 = MEM_H(ctx->r17, 0X10);
            goto L_80141F34;
    }
    goto skip_8;
    // 0x8014200C: lh          $t6, 0x10($s1)
    ctx->r14 = MEM_H(ctx->r17, 0X10);
    skip_8:
    // 0x80142010: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80142014: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80142018: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8014201C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80142020: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80142024: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80142028: jr          $ra
    // 0x8014202C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x8014202C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80142030(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142030: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142034: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142038: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8014203C: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x80142040: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x80142044: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x80142048: lhu         $t7, 0x2($v1)
    ctx->r15 = MEM_HU(ctx->r3, 0X2);
    // 0x8014204C: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142050: addiu       $v0, $v0, -0x2EB0
    ctx->r2 = ADD32(ctx->r2, -0X2EB0);
    // 0x80142054: and         $t9, $t7, $at
    ctx->r25 = ctx->r15 & ctx->r1;
    // 0x80142058: andi        $t0, $t9, 0xBFFF
    ctx->r8 = ctx->r25 & 0XBFFF;
    // 0x8014205C: sh          $t9, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r25;
    // 0x80142060: sh          $t0, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r8;
    // 0x80142064: lw          $t1, 0x4($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X4);
    // 0x80142068: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x8014206C: addiu       $a0, $v1, 0x4
    ctx->r4 = ADD32(ctx->r3, 0X4);
    // 0x80142070: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80142074: jal         0x80142388
    // 0x80142078: sw          $t2, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r10;
    func_80142388(rdram, ctx);
        goto after_0;
    // 0x80142078: sw          $t2, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r10;
    after_0:
    // 0x8014207C: bne         $v0, $zero, L_80142090
    if (ctx->r2 != 0) {
        // 0x80142080: lw          $v1, 0x1C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X1C);
            goto L_80142090;
    }
    // 0x80142080: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80142084: lhu         $t3, 0x2($v1)
    ctx->r11 = MEM_HU(ctx->r3, 0X2);
    // 0x80142088: ori         $t4, $t3, 0x1000
    ctx->r12 = ctx->r11 | 0X1000;
    // 0x8014208C: sh          $t4, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r12;
L_80142090:
    // 0x80142090: jal         0x80002CE0
    // 0x80142094: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_80002CE0(rdram, ctx);
        goto after_1;
    // 0x80142094: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x80142098: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8014209C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801420A0: jr          $ra
    // 0x801420A4: nop

    return;
    // 0x801420A4: nop

;}
RECOMP_FUNC void func_801420A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801420A8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801420AC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801420B0: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x801420B4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801420B8: andi        $t7, $t6, 0x800
    ctx->r15 = ctx->r14 & 0X800;
    // 0x801420BC: beql        $t7, $zero, L_801420D0
    if (ctx->r15 == 0) {
        // 0x801420C0: lhu         $v0, 0x0($a2)
        ctx->r2 = MEM_HU(ctx->r6, 0X0);
            goto L_801420D0;
    }
    goto skip_0;
    // 0x801420C0: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
    skip_0:
    // 0x801420C4: b           L_80142378
    // 0x801420C8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x801420C8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801420CC: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
L_801420D0:
    // 0x801420D0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801420D4: andi        $t8, $v0, 0x7800
    ctx->r24 = ctx->r2 & 0X7800;
    // 0x801420D8: beq         $t8, $zero, L_8014215C
    if (ctx->r24 == 0) {
        // 0x801420DC: andi        $t3, $v0, 0x700
        ctx->r11 = ctx->r2 & 0X700;
            goto L_8014215C;
    }
    // 0x801420DC: andi        $t3, $v0, 0x700
    ctx->r11 = ctx->r2 & 0X700;
    // 0x801420E0: lh          $v1, -0x54CC($v1)
    ctx->r3 = MEM_H(ctx->r3, -0X54CC);
    // 0x801420E4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801420E8: andi        $t9, $v0, 0x800
    ctx->r25 = ctx->r2 & 0X800;
    // 0x801420EC: beq         $v1, $zero, L_8014211C
    if (ctx->r3 == 0) {
        // 0x801420F0: nop
    
            goto L_8014211C;
    }
    // 0x801420F0: nop

    // 0x801420F4: beq         $v1, $at, L_8014212C
    if (ctx->r3 == ctx->r1) {
        // 0x801420F8: andi        $t0, $v0, 0x1000
        ctx->r8 = ctx->r2 & 0X1000;
            goto L_8014212C;
    }
    // 0x801420F8: andi        $t0, $v0, 0x1000
    ctx->r8 = ctx->r2 & 0X1000;
    // 0x801420FC: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80142100: beq         $v1, $at, L_8014213C
    if (ctx->r3 == ctx->r1) {
        // 0x80142104: andi        $t1, $v0, 0x2000
        ctx->r9 = ctx->r2 & 0X2000;
            goto L_8014213C;
    }
    // 0x80142104: andi        $t1, $v0, 0x2000
    ctx->r9 = ctx->r2 & 0X2000;
    // 0x80142108: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x8014210C: beq         $v1, $at, L_8014214C
    if (ctx->r3 == ctx->r1) {
        // 0x80142110: andi        $t2, $v0, 0x4000
        ctx->r10 = ctx->r2 & 0X4000;
            goto L_8014214C;
    }
    // 0x80142110: andi        $t2, $v0, 0x4000
    ctx->r10 = ctx->r2 & 0X4000;
    // 0x80142114: b           L_8014215C
    // 0x80142118: nop

        goto L_8014215C;
    // 0x80142118: nop

L_8014211C:
    // 0x8014211C: bne         $t9, $zero, L_8014215C
    if (ctx->r25 != 0) {
        // 0x80142120: nop
    
            goto L_8014215C;
    }
    // 0x80142120: nop

    // 0x80142124: b           L_80142378
    // 0x80142128: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x80142128: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8014212C:
    // 0x8014212C: bne         $t0, $zero, L_8014215C
    if (ctx->r8 != 0) {
        // 0x80142130: nop
    
            goto L_8014215C;
    }
    // 0x80142130: nop

    // 0x80142134: b           L_80142378
    // 0x80142138: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x80142138: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8014213C:
    // 0x8014213C: bne         $t1, $zero, L_8014215C
    if (ctx->r9 != 0) {
        // 0x80142140: nop
    
            goto L_8014215C;
    }
    // 0x80142140: nop

    // 0x80142144: b           L_80142378
    // 0x80142148: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x80142148: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8014214C:
    // 0x8014214C: bne         $t2, $zero, L_8014215C
    if (ctx->r10 != 0) {
        // 0x80142150: nop
    
            goto L_8014215C;
    }
    // 0x80142150: nop

    // 0x80142154: b           L_80142378
    // 0x80142158: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x80142158: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8014215C:
    // 0x8014215C: beq         $t3, $zero, L_801421BC
    if (ctx->r11 == 0) {
        // 0x80142160: lui         $v1, 0x801D
        ctx->r3 = S32(0X801D << 16);
            goto L_801421BC;
    }
    // 0x80142160: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80142164: lw          $v1, -0x54E8($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X54E8);
    // 0x80142168: andi        $t5, $v0, 0x100
    ctx->r13 = ctx->r2 & 0X100;
    // 0x8014216C: andi        $t4, $v1, 0x10
    ctx->r12 = ctx->r3 & 0X10;
    // 0x80142170: beq         $t4, $zero, L_80142188
    if (ctx->r12 == 0) {
        // 0x80142174: andi        $t6, $v1, 0x20
        ctx->r14 = ctx->r3 & 0X20;
            goto L_80142188;
    }
    // 0x80142174: andi        $t6, $v1, 0x20
    ctx->r14 = ctx->r3 & 0X20;
    // 0x80142178: bne         $t5, $zero, L_80142188
    if (ctx->r13 != 0) {
        // 0x8014217C: nop
    
            goto L_80142188;
    }
    // 0x8014217C: nop

    // 0x80142180: b           L_80142378
    // 0x80142184: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x80142184: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80142188:
    // 0x80142188: beq         $t6, $zero, L_801421A0
    if (ctx->r14 == 0) {
        // 0x8014218C: andi        $t7, $v0, 0x200
        ctx->r15 = ctx->r2 & 0X200;
            goto L_801421A0;
    }
    // 0x8014218C: andi        $t7, $v0, 0x200
    ctx->r15 = ctx->r2 & 0X200;
    // 0x80142190: bnel        $t7, $zero, L_801421A4
    if (ctx->r15 != 0) {
        // 0x80142194: andi        $t8, $v1, 0x40
        ctx->r24 = ctx->r3 & 0X40;
            goto L_801421A4;
    }
    goto skip_1;
    // 0x80142194: andi        $t8, $v1, 0x40
    ctx->r24 = ctx->r3 & 0X40;
    skip_1:
    // 0x80142198: b           L_80142378
    // 0x8014219C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x8014219C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801421A0:
    // 0x801421A0: andi        $t8, $v1, 0x40
    ctx->r24 = ctx->r3 & 0X40;
L_801421A4:
    // 0x801421A4: beq         $t8, $zero, L_801421BC
    if (ctx->r24 == 0) {
        // 0x801421A8: andi        $t9, $v0, 0x400
        ctx->r25 = ctx->r2 & 0X400;
            goto L_801421BC;
    }
    // 0x801421A8: andi        $t9, $v0, 0x400
    ctx->r25 = ctx->r2 & 0X400;
    // 0x801421AC: bnel        $t9, $zero, L_801421C0
    if (ctx->r25 != 0) {
        // 0x801421B0: andi        $t0, $v0, 0x4
        ctx->r8 = ctx->r2 & 0X4;
            goto L_801421C0;
    }
    goto skip_2;
    // 0x801421B0: andi        $t0, $v0, 0x4
    ctx->r8 = ctx->r2 & 0X4;
    skip_2:
    // 0x801421B4: b           L_80142378
    // 0x801421B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x801421B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801421BC:
    // 0x801421BC: andi        $t0, $v0, 0x4
    ctx->r8 = ctx->r2 & 0X4;
L_801421C0:
    // 0x801421C0: beq         $t0, $zero, L_801421E4
    if (ctx->r8 == 0) {
        // 0x801421C4: andi        $t3, $v0, 0x20
        ctx->r11 = ctx->r2 & 0X20;
            goto L_801421E4;
    }
    // 0x801421C4: andi        $t3, $v0, 0x20
    ctx->r11 = ctx->r2 & 0X20;
    // 0x801421C8: lhu         $t1, 0x14($a2)
    ctx->r9 = MEM_HU(ctx->r6, 0X14);
    // 0x801421CC: lhu         $t2, 0x16($a2)
    ctx->r10 = MEM_HU(ctx->r6, 0X16);
    // 0x801421D0: slt         $at, $t1, $t2
    ctx->r1 = SIGNED(ctx->r9) < SIGNED(ctx->r10) ? 1 : 0;
    // 0x801421D4: beq         $at, $zero, L_801421E4
    if (ctx->r1 == 0) {
        // 0x801421D8: nop
    
            goto L_801421E4;
    }
    // 0x801421D8: nop

    // 0x801421DC: b           L_80142378
    // 0x801421E0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x801421E0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801421E4:
    // 0x801421E4: beql        $t3, $zero, L_80142230
    if (ctx->r11 == 0) {
        // 0x801421E8: andi        $t4, $v0, 0x80
        ctx->r12 = ctx->r2 & 0X80;
            goto L_80142230;
    }
    goto skip_3;
    // 0x801421E8: andi        $t4, $v0, 0x80
    ctx->r12 = ctx->r2 & 0X80;
    skip_3:
    // 0x801421EC: lhu         $v0, 0x12($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X12);
    // 0x801421F0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801421F4: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x801421F8: beq         $v0, $zero, L_80142220
    if (ctx->r2 == 0) {
        // 0x801421FC: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_80142220;
    }
    // 0x801421FC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80142200: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80142204: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80142208: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x8014220C: jalr        $t9
    // 0x80142210: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80142210: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x80142214: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80142218: b           L_80142220
    // 0x8014221C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80142220;
    // 0x8014221C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80142220:
    // 0x80142220: beq         $v1, $zero, L_80142274
    if (ctx->r3 == 0) {
        // 0x80142224: nop
    
            goto L_80142274;
    }
    // 0x80142224: nop

    // 0x80142228: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
    // 0x8014222C: andi        $t4, $v0, 0x80
    ctx->r12 = ctx->r2 & 0X80;
L_80142230:
    // 0x80142230: beql        $t4, $zero, L_80142280
    if (ctx->r12 == 0) {
        // 0x80142234: lhu         $v0, 0x0($a2)
        ctx->r2 = MEM_HU(ctx->r6, 0X0);
            goto L_80142280;
    }
    goto skip_4;
    // 0x80142234: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
    skip_4:
    // 0x80142238: lhu         $v0, 0x12($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X12);
    // 0x8014223C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80142240: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80142244: beq         $v0, $zero, L_8014226C
    if (ctx->r2 == 0) {
        // 0x80142248: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_8014226C;
    }
    // 0x80142248: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8014224C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80142250: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80142254: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80142258: jalr        $t9
    // 0x8014225C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8014225C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_1:
    // 0x80142260: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80142264: b           L_8014226C
    // 0x80142268: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_8014226C;
    // 0x80142268: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_8014226C:
    // 0x8014226C: beql        $v1, $zero, L_80142280
    if (ctx->r3 == 0) {
        // 0x80142270: lhu         $v0, 0x0($a2)
        ctx->r2 = MEM_HU(ctx->r6, 0X0);
            goto L_80142280;
    }
    goto skip_5;
    // 0x80142270: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
    skip_5:
L_80142274:
    // 0x80142274: b           L_80142378
    // 0x80142278: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x80142278: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8014227C: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
L_80142280:
    // 0x80142280: andi        $t5, $v0, 0x1
    ctx->r13 = ctx->r2 & 0X1;
    // 0x80142284: beq         $t5, $zero, L_801422A4
    if (ctx->r13 == 0) {
        // 0x80142288: andi        $t8, $v0, 0x8
        ctx->r24 = ctx->r2 & 0X8;
            goto L_801422A4;
    }
    // 0x80142288: andi        $t8, $v0, 0x8
    ctx->r24 = ctx->r2 & 0X8;
    // 0x8014228C: lhu         $t6, 0x2($a2)
    ctx->r14 = MEM_HU(ctx->r6, 0X2);
    // 0x80142290: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x80142294: beq         $t7, $zero, L_801422A4
    if (ctx->r15 == 0) {
        // 0x80142298: nop
    
            goto L_801422A4;
    }
    // 0x80142298: nop

    // 0x8014229C: b           L_80142378
    // 0x801422A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x801422A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801422A4:
    // 0x801422A4: beql        $t8, $zero, L_801422D4
    if (ctx->r24 == 0) {
        // 0x801422A8: lhu         $t0, 0x0($a2)
        ctx->r8 = MEM_HU(ctx->r6, 0X0);
            goto L_801422D4;
    }
    goto skip_6;
    // 0x801422A8: lhu         $t0, 0x0($a2)
    ctx->r8 = MEM_HU(ctx->r6, 0X0);
    skip_6:
    // 0x801422AC: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x801422B0: lw          $t9, 0x1C($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X1C);
    // 0x801422B4: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x801422B8: jalr        $t9
    // 0x801422BC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801422BC: nop

    after_2:
    // 0x801422C0: bne         $v0, $zero, L_801422D0
    if (ctx->r2 != 0) {
        // 0x801422C4: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_801422D0;
    }
    // 0x801422C4: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x801422C8: b           L_80142378
    // 0x801422CC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142378;
    // 0x801422CC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801422D0:
    // 0x801422D0: lhu         $t0, 0x0($a2)
    ctx->r8 = MEM_HU(ctx->r6, 0X0);
L_801422D4:
    // 0x801422D4: andi        $t1, $t0, 0x2
    ctx->r9 = ctx->r8 & 0X2;
    // 0x801422D8: beql        $t1, $zero, L_8014230C
    if (ctx->r9 == 0) {
        // 0x801422DC: lhu         $v0, 0x0($a2)
        ctx->r2 = MEM_HU(ctx->r6, 0X0);
            goto L_8014230C;
    }
    goto skip_7;
    // 0x801422DC: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
    skip_7:
    // 0x801422E0: lhu         $t2, 0x2($a2)
    ctx->r10 = MEM_HU(ctx->r6, 0X2);
    // 0x801422E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801422E8: xori        $t3, $t2, 0x2
    ctx->r11 = ctx->r10 ^ 0X2;
    // 0x801422EC: andi        $v1, $t3, 0xFFFF
    ctx->r3 = ctx->r11 & 0XFFFF;
    // 0x801422F0: andi        $t4, $v1, 0x2
    ctx->r12 = ctx->r3 & 0X2;
    // 0x801422F4: bne         $t4, $zero, L_80142308
    if (ctx->r12 != 0) {
        // 0x801422F8: sh          $t3, 0x2($a2)
        MEM_H(0X2, ctx->r6) = ctx->r11;
            goto L_80142308;
    }
    // 0x801422F8: sh          $t3, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r11;
    // 0x801422FC: ori         $t5, $v1, 0x1000
    ctx->r13 = ctx->r3 | 0X1000;
    // 0x80142300: b           L_80142378
    // 0x80142304: sh          $t5, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r13;
        goto L_80142378;
    // 0x80142304: sh          $t5, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r13;
L_80142308:
    // 0x80142308: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
L_8014230C:
    // 0x8014230C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80142310: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80142314: andi        $t6, $v0, 0x4
    ctx->r14 = ctx->r2 & 0X4;
    // 0x80142318: beql        $t6, $zero, L_80142334
    if (ctx->r14 == 0) {
        // 0x8014231C: andi        $t9, $v0, 0x10
        ctx->r25 = ctx->r2 & 0X10;
            goto L_80142334;
    }
    goto skip_8;
    // 0x8014231C: andi        $t9, $v0, 0x10
    ctx->r25 = ctx->r2 & 0X10;
    skip_8:
    // 0x80142320: lhu         $t7, 0x16($a2)
    ctx->r15 = MEM_HU(ctx->r6, 0X16);
    // 0x80142324: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
    // 0x80142328: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8014232C: sh          $t8, 0x16($a2)
    MEM_H(0X16, ctx->r6) = ctx->r24;
    // 0x80142330: andi        $t9, $v0, 0x10
    ctx->r25 = ctx->r2 & 0X10;
L_80142334:
    // 0x80142334: beq         $t9, $zero, L_80142354
    if (ctx->r25 == 0) {
        // 0x80142338: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80142354;
    }
    // 0x80142338: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8014233C: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x80142340: lhu         $a1, 0x12($a2)
    ctx->r5 = MEM_HU(ctx->r6, 0X12);
    // 0x80142344: jalr        $t9
    // 0x80142348: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80142348: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_3:
    // 0x8014234C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80142350: lhu         $v0, 0x0($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0X0);
L_80142354:
    // 0x80142354: andi        $t0, $v0, 0x40
    ctx->r8 = ctx->r2 & 0X40;
    // 0x80142358: beq         $t0, $zero, L_80142374
    if (ctx->r8 == 0) {
        // 0x8014235C: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_80142374;
    }
    // 0x8014235C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80142360: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80142364: addiu       $t9, $t9, 0x48EC
    ctx->r25 = ADD32(ctx->r25, 0X48EC);
    // 0x80142368: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8014236C: jalr        $t9
    // 0x80142370: lhu         $a1, 0x12($a2)
    ctx->r5 = MEM_HU(ctx->r6, 0X12);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80142370: lhu         $a1, 0x12($a2)
    ctx->r5 = MEM_HU(ctx->r6, 0X12);
    after_4:
L_80142374:
    // 0x80142374: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80142378:
    // 0x80142378: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8014237C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80142380: jr          $ra
    // 0x80142384: nop

    return;
    // 0x80142384: nop

;}
RECOMP_FUNC void func_80142388(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142388: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8014238C: lw          $v0, -0x5214($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X5214);
    // 0x80142390: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142394: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142398: lwc1        $f8, 0x50($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X50);
    // 0x8014239C: lwc1        $f4, -0x2EB0($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X2EB0);
    // 0x801423A0: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x801423A4: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x801423A8: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    // 0x801423AC: nop

    // 0x801423B0: sw          $t8, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r24;
    // 0x801423B4: lwc1        $f16, 0x54($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X54);
    // 0x801423B8: mfc1        $v1, $f6
    ctx->r3 = (int32_t)ctx->f6.u32l;
    // 0x801423BC: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x801423C0: mfc1        $t0, $f18
    ctx->r8 = (int32_t)ctx->f18.u32l;
    // 0x801423C4: nop

    // 0x801423C8: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    // 0x801423CC: lwc1        $f4, 0x58($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X58);
    // 0x801423D0: lw          $t0, 0xC($sp)
    ctx->r8 = MEM_W(ctx->r29, 0XC);
    // 0x801423D4: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x801423D8: mfc1        $t2, $f6
    ctx->r10 = (int32_t)ctx->f6.u32l;
    // 0x801423DC: nop

    // 0x801423E0: sw          $t2, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r10;
    // 0x801423E4: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    // 0x801423E8: lw          $t2, 0x14($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X14);
    // 0x801423EC: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x801423F0: mfc1        $t4, $f10
    ctx->r12 = (int32_t)ctx->f10.u32l;
    // 0x801423F4: nop

    // 0x801423F8: sw          $t4, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r12;
    // 0x801423FC: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80142400: subu        $a1, $t4, $t0
    ctx->r5 = SUB32(ctx->r12, ctx->r8);
    // 0x80142404: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80142408: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x8014240C: mfc1        $t6, $f18
    ctx->r14 = (int32_t)ctx->f18.u32l;
    // 0x80142410: nop

    // 0x80142414: sw          $t6, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r14;
    // 0x80142418: lwc1        $f4, 0x8($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8014241C: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x80142420: mfc1        $t8, $f6
    ctx->r24 = (int32_t)ctx->f6.u32l;
    // 0x80142424: bgez        $a1, L_80142434
    if (SIGNED(ctx->r5) >= 0) {
        // 0x80142428: sw          $t8, 0x8($sp)
        MEM_W(0X8, ctx->r29) = ctx->r24;
            goto L_80142434;
    }
    // 0x80142428: sw          $t8, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r24;
    // 0x8014242C: b           L_80142434
    // 0x80142430: negu        $v0, $a1
    ctx->r2 = SUB32(0, ctx->r5);
        goto L_80142434;
    // 0x80142430: negu        $v0, $a1
    ctx->r2 = SUB32(0, ctx->r5);
L_80142434:
    // 0x80142434: slt         $at, $v1, $v0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80142438: beq         $at, $zero, L_80142448
    if (ctx->r1 == 0) {
        // 0x8014243C: lw          $t1, 0x8($sp)
        ctx->r9 = MEM_W(ctx->r29, 0X8);
            goto L_80142448;
    }
    // 0x8014243C: lw          $t1, 0x8($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X8);
    // 0x80142440: b           L_8014249C
    // 0x80142444: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8014249C;
    // 0x80142444: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80142448:
    // 0x80142448: subu        $a0, $t1, $t2
    ctx->r4 = SUB32(ctx->r9, ctx->r10);
    // 0x8014244C: bgez        $a0, L_8014245C
    if (SIGNED(ctx->r4) >= 0) {
        // 0x80142450: or          $v0, $a0, $zero
        ctx->r2 = ctx->r4 | 0;
            goto L_8014245C;
    }
    // 0x80142450: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80142454: b           L_8014245C
    // 0x80142458: negu        $v0, $a0
    ctx->r2 = SUB32(0, ctx->r4);
        goto L_8014245C;
    // 0x80142458: negu        $v0, $a0
    ctx->r2 = SUB32(0, ctx->r4);
L_8014245C:
    // 0x8014245C: slt         $at, $v1, $v0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x80142460: beq         $at, $zero, L_80142470
    if (ctx->r1 == 0) {
        // 0x80142464: lw          $t3, 0x4($sp)
        ctx->r11 = MEM_W(ctx->r29, 0X4);
            goto L_80142470;
    }
    // 0x80142464: lw          $t3, 0x4($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X4);
    // 0x80142468: b           L_8014249C
    // 0x8014246C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8014249C;
    // 0x8014246C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80142470:
    // 0x80142470: lw          $t4, 0x10($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X10);
    // 0x80142474: subu        $a0, $t3, $t4
    ctx->r4 = SUB32(ctx->r11, ctx->r12);
    // 0x80142478: bgez        $a0, L_80142488
    if (SIGNED(ctx->r4) >= 0) {
        // 0x8014247C: or          $v0, $a0, $zero
        ctx->r2 = ctx->r4 | 0;
            goto L_80142488;
    }
    // 0x8014247C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80142480: b           L_80142488
    // 0x80142484: negu        $v0, $a0
    ctx->r2 = SUB32(0, ctx->r4);
        goto L_80142488;
    // 0x80142484: negu        $v0, $a0
    ctx->r2 = SUB32(0, ctx->r4);
L_80142488:
    // 0x80142488: slt         $at, $v1, $v0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r2) ? 1 : 0;
    // 0x8014248C: beq         $at, $zero, L_8014249C
    if (ctx->r1 == 0) {
        // 0x80142490: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8014249C;
    }
    // 0x80142490: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80142494: b           L_8014249C
    // 0x80142498: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8014249C;
    // 0x80142498: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8014249C:
    // 0x8014249C: jr          $ra
    // 0x801424A0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x801424A0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_801424A4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801424A4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801424A8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801424AC: jal         0x80142388
    // 0x801424B0: nop

    func_80142388(rdram, ctx);
        goto after_0;
    // 0x801424B0: nop

    after_0:
    // 0x801424B4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801424B8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801424BC: jr          $ra
    // 0x801424C0: nop

    return;
    // 0x801424C0: nop

;}
RECOMP_FUNC void func_801424C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801424C4: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x801424C8: bne         $a0, $zero, L_801424D8
    if (ctx->r4 != 0) {
        // 0x801424CC: addiu       $v1, $zero, 0x1
        ctx->r3 = ADD32(0, 0X1);
            goto L_801424D8;
    }
    // 0x801424CC: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x801424D0: jr          $ra
    // 0x801424D4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    return;
    // 0x801424D4: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_801424D8:
    // 0x801424D8: lhu         $a0, 0x2($v0)
    ctx->r4 = MEM_HU(ctx->r2, 0X2);
L_801424DC:
    // 0x801424DC: andi        $t6, $a0, 0x8000
    ctx->r14 = ctx->r4 & 0X8000;
    // 0x801424E0: beq         $t6, $zero, L_801424F4
    if (ctx->r14 == 0) {
        // 0x801424E4: andi        $t7, $a0, 0x4000
        ctx->r15 = ctx->r4 & 0X4000;
            goto L_801424F4;
    }
    // 0x801424E4: andi        $t7, $a0, 0x4000
    ctx->r15 = ctx->r4 & 0X4000;
    // 0x801424E8: bnel        $t7, $zero, L_801424F8
    if (ctx->r15 != 0) {
        // 0x801424EC: lhu         $a0, 0x0($v0)
        ctx->r4 = MEM_HU(ctx->r2, 0X0);
            goto L_801424F8;
    }
    goto skip_0;
    // 0x801424EC: lhu         $a0, 0x0($v0)
    ctx->r4 = MEM_HU(ctx->r2, 0X0);
    skip_0:
    // 0x801424F0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_801424F4:
    // 0x801424F4: lhu         $a0, 0x0($v0)
    ctx->r4 = MEM_HU(ctx->r2, 0X0);
L_801424F8:
    // 0x801424F8: addiu       $v0, $v0, 0x20
    ctx->r2 = ADD32(ctx->r2, 0X20);
    // 0x801424FC: andi        $a0, $a0, 0x8000
    ctx->r4 = ctx->r4 & 0X8000;
    // 0x80142500: sltiu       $a0, $a0, 0x1
    ctx->r4 = ctx->r4 < 0X1 ? 1 : 0;
    // 0x80142504: bnel        $a0, $zero, L_801424DC
    if (ctx->r4 != 0) {
        // 0x80142508: lhu         $a0, 0x2($v0)
        ctx->r4 = MEM_HU(ctx->r2, 0X2);
            goto L_801424DC;
    }
    goto skip_1;
    // 0x80142508: lhu         $a0, 0x2($v0)
    ctx->r4 = MEM_HU(ctx->r2, 0X2);
    skip_1:
    // 0x8014250C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80142510: beq         $v1, $at, L_80142520
    if (ctx->r3 == ctx->r1) {
        // 0x80142514: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80142520;
    }
    // 0x80142514: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80142518: jr          $ra
    // 0x8014251C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x8014251C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80142520:
    // 0x80142520: jr          $ra
    // 0x80142524: nop

    return;
    // 0x80142524: nop

;}
RECOMP_FUNC void func_80142530(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142530: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80142534: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142538: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8014253C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80142540: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80142544: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80142548: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8014254C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80142550: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80142554: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80142558: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8014255C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80142560: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80142564: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80142568: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8014256C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80142570: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80142574: lw          $t9, -0x3780($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X3780);
    // 0x80142578: jalr        $t9
    // 0x8014257C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8014257C: nop

    after_0:
    // 0x80142580: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80142584: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80142588: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8014258C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80142590: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142594: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80142598: jr          $ra
    // 0x8014259C: nop

    return;
    // 0x8014259C: nop

;}
RECOMP_FUNC void func_801425A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801425A0: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801425A4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801425A8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801425AC: lw          $a3, 0x70($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X70);
    // 0x801425B0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801425B4: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x801425B8: lhu         $t6, 0x18($a3)
    ctx->r14 = MEM_HU(ctx->r7, 0X18);
    // 0x801425BC: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x801425C0: andi        $t7, $t6, 0x400
    ctx->r15 = ctx->r14 & 0X400;
    // 0x801425C4: beq         $t7, $zero, L_801425D4
    if (ctx->r15 == 0) {
        // 0x801425C8: nop
    
            goto L_801425D4;
    }
    // 0x801425C8: nop

    // 0x801425CC: b           L_801425D4
    // 0x801425D0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
        goto L_801425D4;
    // 0x801425D0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
L_801425D4:
    // 0x801425D4: ori         $a0, $a0, 0x2000
    ctx->r4 = ctx->r4 | 0X2000;
    // 0x801425D8: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x801425DC: jal         0x80005A30
    // 0x801425E0: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    func_80005A30(rdram, ctx);
        goto after_0;
    // 0x801425E0: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    after_0:
    // 0x801425E4: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x801425E8: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801425EC: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x801425F0: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x801425F4: lh          $t8, 0x28D0($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X28D0);
    // 0x801425F8: lw          $t1, 0x2B14($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X2B14);
    // 0x801425FC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80142600: sll         $t9, $t8, 4
    ctx->r25 = S32(ctx->r24 << 4);
    // 0x80142604: addu        $a0, $a0, $t9
    ctx->r4 = ADD32(ctx->r4, ctx->r25);
    // 0x80142608: lw          $a0, -0x3760($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X3760);
    // 0x8014260C: sw          $t1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r9;
    // 0x80142610: lhu         $t2, 0x18($a3)
    ctx->r10 = MEM_HU(ctx->r7, 0X18);
    // 0x80142614: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
    // 0x80142618: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x8014261C: andi        $t3, $t2, 0xFF
    ctx->r11 = ctx->r10 & 0XFF;
    // 0x80142620: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80142624: subu        $t4, $t4, $t3
    ctx->r12 = SUB32(ctx->r12, ctx->r11);
    // 0x80142628: sll         $t4, $t4, 2
    ctx->r12 = S32(ctx->r12 << 2);
    // 0x8014262C: addu        $t5, $a0, $t4
    ctx->r13 = ADD32(ctx->r4, ctx->r12);
    // 0x80142630: lw          $t6, 0x0($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X0);
    // 0x80142634: ori         $t8, $t7, 0x100
    ctx->r24 = ctx->r15 | 0X100;
    // 0x80142638: sh          $t8, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r24;
    // 0x8014263C: sw          $t6, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r14;
    // 0x80142640: lhu         $v1, 0x18($a3)
    ctx->r3 = MEM_HU(ctx->r7, 0X18);
    // 0x80142644: andi        $t9, $v1, 0x400
    ctx->r25 = ctx->r3 & 0X400;
    // 0x80142648: beql        $t9, $zero, L_80142690
    if (ctx->r25 == 0) {
        // 0x8014264C: andi        $t3, $v1, 0x800
        ctx->r11 = ctx->r3 & 0X800;
            goto L_80142690;
    }
    goto skip_0;
    // 0x8014264C: andi        $t3, $v1, 0x800
    ctx->r11 = ctx->r3 & 0X800;
    skip_0:
    // 0x80142650: lh          $t1, 0x0($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X0);
    // 0x80142654: ori         $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 | 0X1000;
    // 0x80142658: sh          $t2, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r10;
    // 0x8014265C: lw          $a1, 0x40($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X40);
    // 0x80142660: lw          $a0, 0x3C($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X3C);
    // 0x80142664: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    // 0x80142668: jal         0x801464B0
    // 0x8014266C: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    func_801464B0(rdram, ctx);
        goto after_1;
    // 0x8014266C: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    after_1:
    // 0x80142670: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x80142674: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x80142678: sw          $v0, 0x68($s0)
    MEM_W(0X68, ctx->r16) = ctx->r2;
    // 0x8014267C: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80142680: sw          $v0, 0x74($a2)
    MEM_W(0X74, ctx->r6) = ctx->r2;
    // 0x80142684: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80142688: lhu         $v1, 0x18($a3)
    ctx->r3 = MEM_HU(ctx->r7, 0X18);
    // 0x8014268C: andi        $t3, $v1, 0x800
    ctx->r11 = ctx->r3 & 0X800;
L_80142690:
    // 0x80142690: beql        $t3, $zero, L_801426A8
    if (ctx->r11 == 0) {
        // 0x80142694: lw          $t6, 0x2B0C($t0)
        ctx->r14 = MEM_W(ctx->r8, 0X2B0C);
            goto L_801426A8;
    }
    goto skip_1;
    // 0x80142694: lw          $t6, 0x2B0C($t0)
    ctx->r14 = MEM_W(ctx->r8, 0X2B0C);
    skip_1:
    // 0x80142698: lhu         $t4, 0x2($a2)
    ctx->r12 = MEM_HU(ctx->r6, 0X2);
    // 0x8014269C: ori         $t5, $t4, 0x800
    ctx->r13 = ctx->r12 | 0X800;
    // 0x801426A0: sh          $t5, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r13;
    // 0x801426A4: lw          $t6, 0x2B0C($t0)
    ctx->r14 = MEM_W(ctx->r8, 0X2B0C);
L_801426A8:
    // 0x801426A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801426AC: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x801426B0: sw          $t6, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->r14;
    // 0x801426B4: lw          $t7, 0x7C($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X7C);
    // 0x801426B8: sw          $t7, 0x24($a2)
    MEM_W(0X24, ctx->r6) = ctx->r15;
    // 0x801426BC: sw          $a3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r7;
    // 0x801426C0: jal         0x801431AC
    // 0x801426C4: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    func_801431AC(rdram, ctx);
        goto after_2;
    // 0x801426C4: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    after_2:
    // 0x801426C8: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x801426CC: lw          $a3, 0x28($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X28);
    // 0x801426D0: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801426D4: sh          $zero, 0x60($a2)
    MEM_H(0X60, ctx->r6) = 0;
    // 0x801426D8: lh          $t8, 0x60($a2)
    ctx->r24 = MEM_H(ctx->r6, 0X60);
    // 0x801426DC: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x801426E0: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x801426E4: sh          $t8, 0x5C($a2)
    MEM_H(0X5C, ctx->r6) = ctx->r24;
    // 0x801426E8: lhu         $t9, 0x16($a3)
    ctx->r25 = MEM_HU(ctx->r7, 0X16);
    // 0x801426EC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801426F0: sh          $t9, 0x5E($a2)
    MEM_H(0X5E, ctx->r6) = ctx->r25;
    // 0x801426F4: lh          $t1, 0x28D0($t0)
    ctx->r9 = MEM_H(ctx->r8, 0X28D0);
    // 0x801426F8: sll         $t2, $t1, 4
    ctx->r10 = S32(ctx->r9 << 4);
    // 0x801426FC: addu        $v1, $v1, $t2
    ctx->r3 = ADD32(ctx->r3, ctx->r10);
    // 0x80142700: lw          $v1, -0x3754($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X3754);
    // 0x80142704: beql        $v1, $zero, L_8014272C
    if (ctx->r3 == 0) {
        // 0x80142708: addiu       $a0, $s0, 0x34
        ctx->r4 = ADD32(ctx->r16, 0X34);
            goto L_8014272C;
    }
    goto skip_2;
    // 0x80142708: addiu       $a0, $s0, 0x34
    ctx->r4 = ADD32(ctx->r16, 0X34);
    skip_2:
    // 0x8014270C: lhu         $t3, 0x14($a3)
    ctx->r11 = MEM_HU(ctx->r7, 0X14);
    // 0x80142710: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80142714: addu        $t5, $v1, $t4
    ctx->r13 = ADD32(ctx->r3, ctx->r12);
    // 0x80142718: lw          $a0, 0x0($t5)
    ctx->r4 = MEM_W(ctx->r13, 0X0);
    // 0x8014271C: beql        $a0, $zero, L_8014272C
    if (ctx->r4 == 0) {
        // 0x80142720: addiu       $a0, $s0, 0x34
        ctx->r4 = ADD32(ctx->r16, 0X34);
            goto L_8014272C;
    }
    goto skip_3;
    // 0x80142720: addiu       $a0, $s0, 0x34
    ctx->r4 = ADD32(ctx->r16, 0X34);
    skip_3:
    // 0x80142724: sw          $a0, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->r4;
    // 0x80142728: addiu       $a0, $s0, 0x34
    ctx->r4 = ADD32(ctx->r16, 0X34);
L_8014272C:
    // 0x8014272C: lw          $v1, 0x0($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X0);
    // 0x80142730: beq         $v1, $zero, L_8014277C
    if (ctx->r3 == 0) {
        // 0x80142734: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_8014277C;
    }
    // 0x80142734: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80142738: sh          $zero, 0x2($v0)
    MEM_H(0X2, ctx->r2) = 0;
L_8014273C:
    // 0x8014273C: lhu         $t6, 0xC($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0XC);
    // 0x80142740: addiu       $v0, $v0, 0x20
    ctx->r2 = ADD32(ctx->r2, 0X20);
    // 0x80142744: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80142748: sh          $t7, 0xC($a0)
    MEM_H(0XC, ctx->r4) = ctx->r15;
    // 0x8014274C: lhu         $v1, -0x20($v0)
    ctx->r3 = MEM_HU(ctx->r2, -0X20);
    // 0x80142750: andi        $v1, $v1, 0x8000
    ctx->r3 = ctx->r3 & 0X8000;
    // 0x80142754: sltiu       $v1, $v1, 0x1
    ctx->r3 = ctx->r3 < 0X1 ? 1 : 0;
    // 0x80142758: bnel        $v1, $zero, L_8014273C
    if (ctx->r3 != 0) {
        // 0x8014275C: sh          $zero, 0x2($v0)
        MEM_H(0X2, ctx->r2) = 0;
            goto L_8014273C;
    }
    goto skip_4;
    // 0x8014275C: sh          $zero, 0x2($v0)
    MEM_H(0X2, ctx->r2) = 0;
    skip_4:
    // 0x80142760: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80142764: addiu       $v1, $v1, -0x2EB0
    ctx->r3 = ADD32(ctx->r3, -0X2EB0);
    // 0x80142768: lw          $t8, 0x18($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X18);
    // 0x8014276C: andi        $t9, $t7, 0xFFFF
    ctx->r25 = ctx->r15 & 0XFFFF;
    // 0x80142770: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142774: addu        $t1, $t8, $t9
    ctx->r9 = ADD32(ctx->r24, ctx->r25);
    // 0x80142778: sw          $t1, -0x2E98($at)
    MEM_W(-0X2E98, ctx->r1) = ctx->r9;
L_8014277C:
    // 0x8014277C: lui         $v1, 0x801A
    ctx->r3 = S32(0X801A << 16);
    // 0x80142780: addiu       $v1, $v1, -0x2EB0
    ctx->r3 = ADD32(ctx->r3, -0X2EB0);
    // 0x80142784: lw          $t2, 0x1C($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X1C);
    // 0x80142788: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x8014278C: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80142790: sw          $t3, 0x1C($v1)
    MEM_W(0X1C, ctx->r3) = ctx->r11;
    // 0x80142794: sb          $zero, 0x4($a0)
    MEM_B(0X4, ctx->r4) = 0;
    // 0x80142798: sb          $zero, 0x5($a0)
    MEM_B(0X5, ctx->r4) = 0;
    // 0x8014279C: sw          $zero, 0x8($a0)
    MEM_W(0X8, ctx->r4) = 0;
    // 0x801427A0: lh          $t7, 0x28D2($t0)
    ctx->r15 = MEM_H(ctx->r8, 0X28D2);
    // 0x801427A4: lh          $t4, 0x28D0($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X28D0);
    // 0x801427A8: lhu         $t1, 0x14($a3)
    ctx->r9 = MEM_HU(ctx->r7, 0X14);
    // 0x801427AC: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x801427B0: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x801427B4: addu        $t6, $t6, $t5
    ctx->r14 = ADD32(ctx->r14, ctx->r13);
    // 0x801427B8: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x801427BC: lw          $t6, -0xB8($t6)
    ctx->r14 = MEM_W(ctx->r14, -0XB8);
    // 0x801427C0: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x801427C4: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x801427C8: sll         $t8, $t8, 1
    ctx->r24 = S32(ctx->r24 << 1);
    // 0x801427CC: addu        $v0, $t6, $t8
    ctx->r2 = ADD32(ctx->r14, ctx->r24);
    // 0x801427D0: lhu         $t9, 0x0($v0)
    ctx->r25 = MEM_HU(ctx->r2, 0X0);
    // 0x801427D4: ori         $t3, $zero, 0x1
    ctx->r11 = 0 | 0X1;
    // 0x801427D8: bne         $t9, $t1, L_801427E4
    if (ctx->r25 != ctx->r9) {
        // 0x801427DC: nop
    
            goto L_801427E4;
    }
    // 0x801427DC: nop

    // 0x801427E0: sb          $t3, 0x4($a0)
    MEM_B(0X4, ctx->r4) = ctx->r11;
L_801427E4:
    // 0x801427E4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801427E8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801427EC: jalr        $t9
    // 0x801427F0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801427F0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    after_3:
    // 0x801427F4: lh          $t4, 0xE($s0)
    ctx->r12 = MEM_H(ctx->r16, 0XE);
    // 0x801427F8: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801427FC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80142800: sll         $t5, $t4, 1
    ctx->r13 = S32(ctx->r12 << 1);
    // 0x80142804: addu        $v0, $s0, $t5
    ctx->r2 = ADD32(ctx->r16, ctx->r13);
    // 0x80142808: lbu         $t8, 0x9($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X9);
    // 0x8014280C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80142810: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80142814: sll         $t1, $t8, 2
    ctx->r9 = S32(ctx->r24 << 2);
    // 0x80142818: addiu       $t6, $t7, 0x1
    ctx->r14 = ADD32(ctx->r15, 0X1);
    // 0x8014281C: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x80142820: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x80142824: lw          $t9, -0x3780($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X3780);
    // 0x80142828: jalr        $t9
    // 0x8014282C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x8014282C: nop

    after_4:
    // 0x80142830: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80142834: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80142838: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8014283C: jr          $ra
    // 0x80142840: nop

    return;
    // 0x80142840: nop

;}
RECOMP_FUNC void func_80142844(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142844: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80142848: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8014284C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80142850: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80142854: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80142858: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x8014285C: lw          $t6, 0x2B0C($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2B0C);
    // 0x80142860: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80142864: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x80142868: sw          $t6, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r14;
    // 0x8014286C: lw          $t7, 0x7C($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X7C);
    // 0x80142870: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80142874: sw          $t7, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r15;
    // 0x80142878: lbu         $t9, 0x30($a0)
    ctx->r25 = MEM_BU(ctx->r4, 0X30);
    // 0x8014287C: lbu         $t8, 0x31($a0)
    ctx->r24 = MEM_BU(ctx->r4, 0X31);
    // 0x80142880: beq         $t8, $t9, L_80142894
    if (ctx->r24 == ctx->r25) {
        // 0x80142884: lui         $t9, 0x8000
        ctx->r25 = S32(0X8000 << 16);
            goto L_80142894;
    }
    // 0x80142884: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80142888: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x8014288C: jalr        $t9
    // 0x80142890: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80142890: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
L_80142894:
    // 0x80142894: addiu       $a1, $s0, 0x34
    ctx->r5 = ADD32(ctx->r16, 0X34);
    // 0x80142898: lbu         $t0, 0x4($a1)
    ctx->r8 = MEM_BU(ctx->r5, 0X4);
    // 0x8014289C: andi        $t1, $t0, 0x1
    ctx->r9 = ctx->r8 & 0X1;
    // 0x801428A0: beql        $t1, $zero, L_8014290C
    if (ctx->r9 == 0) {
        // 0x801428A4: lh          $v1, 0xE($s0)
        ctx->r3 = MEM_H(ctx->r16, 0XE);
            goto L_8014290C;
    }
    goto skip_0;
    // 0x801428A4: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    skip_0:
    // 0x801428A8: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    // 0x801428AC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801428B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801428B4: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801428B8: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801428BC: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801428C0: sll         $t2, $v1, 1
    ctx->r10 = S32(ctx->r3 << 1);
    // 0x801428C4: addu        $v0, $s0, $t2
    ctx->r2 = ADD32(ctx->r16, ctx->r10);
    // 0x801428C8: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x801428CC: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801428D0: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801428D4: sb          $t4, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r12;
    // 0x801428D8: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    // 0x801428DC: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801428E0: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801428E4: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801428E8: lw          $t9, -0x3778($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X3778);
    // 0x801428EC: jalr        $t9
    // 0x801428F0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801428F0: nop

    after_1:
    // 0x801428F4: lh          $t7, 0xE($s0)
    ctx->r15 = MEM_H(ctx->r16, 0XE);
    // 0x801428F8: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x801428FC: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x80142900: b           L_80142964
    // 0x80142904: sh          $t8, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r24;
        goto L_80142964;
    // 0x80142904: sh          $t8, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r24;
    // 0x80142908: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
L_8014290C:
    // 0x8014290C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80142910: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80142914: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80142918: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8014291C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80142920: sll         $t0, $v1, 1
    ctx->r8 = S32(ctx->r3 << 1);
    // 0x80142924: addu        $v0, $s0, $t0
    ctx->r2 = ADD32(ctx->r16, ctx->r8);
    // 0x80142928: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x8014292C: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x80142930: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80142934: sb          $t2, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r10;
    // 0x80142938: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    // 0x8014293C: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x80142940: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80142944: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
    // 0x80142948: lw          $t9, -0x3768($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X3768);
    // 0x8014294C: jalr        $t9
    // 0x80142950: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80142950: nop

    after_2:
    // 0x80142954: lh          $t5, 0xE($s0)
    ctx->r13 = MEM_H(ctx->r16, 0XE);
    // 0x80142958: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x8014295C: addiu       $t6, $t5, -0x1
    ctx->r14 = ADD32(ctx->r13, -0X1);
    // 0x80142960: sh          $t6, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r14;
L_80142964:
    // 0x80142964: lbu         $t7, 0x4($a1)
    ctx->r15 = MEM_BU(ctx->r5, 0X4);
    // 0x80142968: sb          $t7, 0x5($a1)
    MEM_B(0X5, ctx->r5) = ctx->r15;
    // 0x8014296C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80142970: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80142974: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80142978: jr          $ra
    // 0x8014297C: nop

    return;
    // 0x8014297C: nop

;}
RECOMP_FUNC void func_80142980(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142980: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80142984: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142988: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x8014298C: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x80142990: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80142994: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x80142998: addiu       $t1, $a2, 0x34
    ctx->r9 = ADD32(ctx->r6, 0X34);
    // 0x8014299C: or          $t7, $t6, $at
    ctx->r15 = ctx->r14 | ctx->r1;
    // 0x801429A0: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x801429A4: lw          $t8, 0x34($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X34);
    // 0x801429A8: beql        $t8, $zero, L_801429DC
    if (ctx->r24 == 0) {
        // 0x801429AC: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_801429DC;
    }
    goto skip_0;
    // 0x801429AC: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    skip_0:
    // 0x801429B0: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x801429B4: addiu       $a0, $zero, -0x1001
    ctx->r4 = ADD32(0, -0X1001);
L_801429B8:
    // 0x801429B8: lhu         $v1, 0x0($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X0);
    // 0x801429BC: lhu         $t9, 0x2($v0)
    ctx->r25 = MEM_HU(ctx->r2, 0X2);
    // 0x801429C0: addiu       $v0, $v0, 0x20
    ctx->r2 = ADD32(ctx->r2, 0X20);
    // 0x801429C4: andi        $v1, $v1, 0x8000
    ctx->r3 = ctx->r3 & 0X8000;
    // 0x801429C8: sltiu       $v1, $v1, 0x1
    ctx->r3 = ctx->r3 < 0X1 ? 1 : 0;
    // 0x801429CC: and         $t0, $t9, $a0
    ctx->r8 = ctx->r25 & ctx->r4;
    // 0x801429D0: bne         $v1, $zero, L_801429B8
    if (ctx->r3 != 0) {
        // 0x801429D4: sh          $t0, -0x1E($v0)
        MEM_H(-0X1E, ctx->r2) = ctx->r8;
            goto L_801429B8;
    }
    // 0x801429D4: sh          $t0, -0x1E($v0)
    MEM_H(-0X1E, ctx->r2) = ctx->r8;
    // 0x801429D8: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
L_801429DC:
    // 0x801429DC: sw          $t1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r9;
    // 0x801429E0: jal         0x80143234
    // 0x801429E4: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    func_80143234(rdram, ctx);
        goto after_0;
    // 0x801429E4: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    after_0:
    // 0x801429E8: lw          $a2, 0x30($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X30);
    // 0x801429EC: lw          $t2, 0x1C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X1C);
    // 0x801429F0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801429F4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801429F8: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801429FC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80142A00: jalr        $t9
    // 0x80142A04: sw          $zero, 0x8($t2)
    MEM_W(0X8, ctx->r10) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80142A04: sw          $zero, 0x8($t2)
    MEM_W(0X8, ctx->r10) = 0;
    after_1:
    // 0x80142A08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142A0C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80142A10: jr          $ra
    // 0x80142A14: nop

    return;
    // 0x80142A14: nop

;}
RECOMP_FUNC void func_80142A18(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142A18: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80142A1C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142A20: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x80142A24: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80142A28: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142A2C: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x80142A30: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80142A34: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80142A38: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x80142A3C: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x80142A40: sw          $a0, -0x2E8C($at)
    MEM_W(-0X2E8C, ctx->r1) = ctx->r4;
    // 0x80142A44: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80142A48: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x80142A4C: jalr        $t9
    // 0x80142A50: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80142A50: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
    // 0x80142A54: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80142A58: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80142A5C: lh          $t8, 0xE($a0)
    ctx->r24 = MEM_H(ctx->r4, 0XE);
    // 0x80142A60: sll         $t0, $t8, 1
    ctx->r8 = S32(ctx->r24 << 1);
    // 0x80142A64: addu        $v0, $a0, $t0
    ctx->r2 = ADD32(ctx->r4, ctx->r8);
    // 0x80142A68: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x80142A6C: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x80142A70: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80142A74: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80142A78: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80142A7C: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x80142A80: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
    // 0x80142A84: lw          $t9, -0x3778($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X3778);
    // 0x80142A88: jalr        $t9
    // 0x80142A8C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80142A8C: nop

    after_1:
    // 0x80142A90: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142A94: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80142A98: jr          $ra
    // 0x80142A9C: nop

    return;
    // 0x80142A9C: nop

;}
RECOMP_FUNC void func_80142AA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142AA0: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80142AA4: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80142AA8: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80142AAC: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80142AB0: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80142AB4: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80142AB8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80142ABC: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x80142AC0: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80142AC4: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80142AC8: beq         $t6, $zero, L_80142B90
    if (ctx->r14 == 0) {
        // 0x80142ACC: addiu       $s4, $zero, -0x1
        ctx->r20 = ADD32(0, -0X1);
            goto L_80142B90;
    }
    // 0x80142ACC: addiu       $s4, $zero, -0x1
    ctx->r20 = ADD32(0, -0X1);
    // 0x80142AD0: lw          $s1, 0x34($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X34);
    // 0x80142AD4: addiu       $s2, $s2, -0x2EB0
    ctx->r18 = ADD32(ctx->r18, -0X2EB0);
    // 0x80142AD8: lh          $t7, 0x10($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X10);
L_80142ADC:
    // 0x80142ADC: beql        $s4, $t7, L_80142B7C
    if (ctx->r20 == ctx->r15) {
        // 0x80142AE0: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142B7C;
    }
    goto skip_0;
    // 0x80142AE0: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_0:
    // 0x80142AE4: lhu         $v0, 0x2($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X2);
    // 0x80142AE8: andi        $t8, $v0, 0x8000
    ctx->r24 = ctx->r2 & 0X8000;
    // 0x80142AEC: bne         $t8, $zero, L_80142B78
    if (ctx->r24 != 0) {
        // 0x80142AF0: andi        $t9, $v0, 0x1000
        ctx->r25 = ctx->r2 & 0X1000;
            goto L_80142B78;
    }
    // 0x80142AF0: andi        $t9, $v0, 0x1000
    ctx->r25 = ctx->r2 & 0X1000;
    // 0x80142AF4: bnel        $t9, $zero, L_80142B7C
    if (ctx->r25 != 0) {
        // 0x80142AF8: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142B7C;
    }
    goto skip_1;
    // 0x80142AF8: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_1:
    // 0x80142AFC: jal         0x801420A8
    // 0x80142B00: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_801420A8(rdram, ctx);
        goto after_0;
    // 0x80142B00: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_0:
    // 0x80142B04: beql        $v0, $zero, L_80142B7C
    if (ctx->r2 == 0) {
        // 0x80142B08: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142B7C;
    }
    goto skip_2;
    // 0x80142B08: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_2:
    // 0x80142B0C: lh          $s0, 0x10($s1)
    ctx->r16 = MEM_H(ctx->r17, 0X10);
    // 0x80142B10: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80142B14: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80142B18: slti        $at, $s0, 0x112
    ctx->r1 = SIGNED(ctx->r16) < 0X112 ? 1 : 0;
    // 0x80142B1C: bne         $at, $zero, L_80142B58
    if (ctx->r1 != 0) {
        // 0x80142B20: sll         $a1, $s0, 16
        ctx->r5 = S32(ctx->r16 << 16);
            goto L_80142B58;
    }
    // 0x80142B20: sll         $a1, $s0, 16
    ctx->r5 = S32(ctx->r16 << 16);
    // 0x80142B24: slti        $at, $s0, 0x127
    ctx->r1 = SIGNED(ctx->r16) < 0X127 ? 1 : 0;
    // 0x80142B28: beql        $at, $zero, L_80142B5C
    if (ctx->r1 == 0) {
        // 0x80142B2C: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_80142B5C;
    }
    goto skip_3;
    // 0x80142B2C: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    skip_3:
    // 0x80142B30: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80142B34: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80142B38: jal         0x80059CB8
    // 0x80142B3C: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    func_80059CB8(rdram, ctx);
        goto after_1;
    // 0x80142B3C: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    after_1:
    // 0x80142B40: beql        $v0, $zero, L_80142B7C
    if (ctx->r2 == 0) {
        // 0x80142B44: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142B7C;
    }
    goto skip_4;
    // 0x80142B44: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_4:
    // 0x80142B48: lw          $t0, 0xC($s2)
    ctx->r8 = MEM_W(ctx->r18, 0XC);
    // 0x80142B4C: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80142B50: b           L_80142B78
    // 0x80142B54: sw          $t1, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r9;
        goto L_80142B78;
    // 0x80142B54: sw          $t1, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r9;
L_80142B58:
    // 0x80142B58: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
L_80142B5C:
    // 0x80142B5C: jal         0x80142FA8
    // 0x80142B60: or          $a3, $s3, $zero
    ctx->r7 = ctx->r19 | 0;
    func_80142FA8(rdram, ctx);
        goto after_2;
    // 0x80142B60: or          $a3, $s3, $zero
    ctx->r7 = ctx->r19 | 0;
    after_2:
    // 0x80142B64: beql        $v0, $zero, L_80142B7C
    if (ctx->r2 == 0) {
        // 0x80142B68: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142B7C;
    }
    goto skip_5;
    // 0x80142B68: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_5:
    // 0x80142B6C: lw          $t2, 0x4($s2)
    ctx->r10 = MEM_W(ctx->r18, 0X4);
    // 0x80142B70: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80142B74: sw          $t3, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r11;
L_80142B78:
    // 0x80142B78: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
L_80142B7C:
    // 0x80142B7C: addiu       $s1, $s1, 0x20
    ctx->r17 = ADD32(ctx->r17, 0X20);
    // 0x80142B80: andi        $v0, $v0, 0x8000
    ctx->r2 = ctx->r2 & 0X8000;
    // 0x80142B84: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x80142B88: bnel        $v0, $zero, L_80142ADC
    if (ctx->r2 != 0) {
        // 0x80142B8C: lh          $t7, 0x10($s1)
        ctx->r15 = MEM_H(ctx->r17, 0X10);
            goto L_80142ADC;
    }
    goto skip_6;
    // 0x80142B8C: lh          $t7, 0x10($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X10);
    skip_6:
L_80142B90:
    // 0x80142B90: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80142B94: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80142B98: addiu       $a0, $s3, 0x8
    ctx->r4 = ADD32(ctx->r19, 0X8);
    // 0x80142B9C: jalr        $t9
    // 0x80142BA0: addiu       $a1, $s3, 0xE
    ctx->r5 = ADD32(ctx->r19, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80142BA0: addiu       $a1, $s3, 0xE
    ctx->r5 = ADD32(ctx->r19, 0XE);
    after_3:
    // 0x80142BA4: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80142BA8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80142BAC: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80142BB0: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80142BB4: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80142BB8: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80142BBC: jr          $ra
    // 0x80142BC0: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80142BC0: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80142BC4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142BC4: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80142BC8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142BCC: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x80142BD0: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80142BD4: beql        $t6, $zero, L_80142BF4
    if (ctx->r14 == 0) {
        // 0x80142BD8: addiu       $t7, $a2, 0x34
        ctx->r15 = ADD32(ctx->r6, 0X34);
            goto L_80142BF4;
    }
    goto skip_0;
    // 0x80142BD8: addiu       $t7, $a2, 0x34
    ctx->r15 = ADD32(ctx->r6, 0X34);
    skip_0:
    // 0x80142BDC: lw          $a0, 0x34($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X34);
    // 0x80142BE0: jal         0x801424C4
    // 0x80142BE4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    func_801424C4(rdram, ctx);
        goto after_0;
    // 0x80142BE4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x80142BE8: bne         $v0, $zero, L_80142C28
    if (ctx->r2 != 0) {
        // 0x80142BEC: lw          $a2, 0x28($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X28);
            goto L_80142C28;
    }
    // 0x80142BEC: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80142BF0: addiu       $t7, $a2, 0x34
    ctx->r15 = ADD32(ctx->r6, 0X34);
L_80142BF4:
    // 0x80142BF4: sw          $t7, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r15;
    // 0x80142BF8: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80142BFC: jal         0x80143234
    // 0x80142C00: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    func_80143234(rdram, ctx);
        goto after_1;
    // 0x80142C00: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_1:
    // 0x80142C04: lw          $t9, 0x1C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X1C);
    // 0x80142C08: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80142C0C: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x80142C10: sw          $t8, 0x8($t9)
    MEM_W(0X8, ctx->r25) = ctx->r24;
    // 0x80142C14: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80142C18: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80142C1C: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80142C20: jalr        $t9
    // 0x80142C24: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80142C24: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_2:
L_80142C28:
    // 0x80142C28: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142C2C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80142C30: jr          $ra
    // 0x80142C34: nop

    return;
    // 0x80142C34: nop

;}
RECOMP_FUNC void func_80142C38(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142C38: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80142C3C: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80142C40: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80142C44: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80142C48: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80142C4C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80142C50: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80142C54: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x80142C58: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80142C5C: lui         $s2, 0x801A
    ctx->r18 = S32(0X801A << 16);
    // 0x80142C60: beq         $t6, $zero, L_80142D28
    if (ctx->r14 == 0) {
        // 0x80142C64: addiu       $s4, $zero, -0x1
        ctx->r20 = ADD32(0, -0X1);
            goto L_80142D28;
    }
    // 0x80142C64: addiu       $s4, $zero, -0x1
    ctx->r20 = ADD32(0, -0X1);
    // 0x80142C68: lw          $s1, 0x34($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X34);
    // 0x80142C6C: addiu       $s2, $s2, -0x2EB0
    ctx->r18 = ADD32(ctx->r18, -0X2EB0);
    // 0x80142C70: lh          $t7, 0x10($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X10);
L_80142C74:
    // 0x80142C74: beql        $s4, $t7, L_80142D14
    if (ctx->r20 == ctx->r15) {
        // 0x80142C78: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142D14;
    }
    goto skip_0;
    // 0x80142C78: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_0:
    // 0x80142C7C: lhu         $v0, 0x2($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X2);
    // 0x80142C80: andi        $t8, $v0, 0x8000
    ctx->r24 = ctx->r2 & 0X8000;
    // 0x80142C84: bne         $t8, $zero, L_80142D10
    if (ctx->r24 != 0) {
        // 0x80142C88: andi        $t9, $v0, 0x1000
        ctx->r25 = ctx->r2 & 0X1000;
            goto L_80142D10;
    }
    // 0x80142C88: andi        $t9, $v0, 0x1000
    ctx->r25 = ctx->r2 & 0X1000;
    // 0x80142C8C: bnel        $t9, $zero, L_80142D14
    if (ctx->r25 != 0) {
        // 0x80142C90: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142D14;
    }
    goto skip_1;
    // 0x80142C90: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_1:
    // 0x80142C94: jal         0x801420A8
    // 0x80142C98: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_801420A8(rdram, ctx);
        goto after_0;
    // 0x80142C98: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_0:
    // 0x80142C9C: beql        $v0, $zero, L_80142D14
    if (ctx->r2 == 0) {
        // 0x80142CA0: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142D14;
    }
    goto skip_2;
    // 0x80142CA0: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_2:
    // 0x80142CA4: lh          $s0, 0x10($s1)
    ctx->r16 = MEM_H(ctx->r17, 0X10);
    // 0x80142CA8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80142CAC: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80142CB0: slti        $at, $s0, 0x112
    ctx->r1 = SIGNED(ctx->r16) < 0X112 ? 1 : 0;
    // 0x80142CB4: bne         $at, $zero, L_80142CF0
    if (ctx->r1 != 0) {
        // 0x80142CB8: sll         $a1, $s0, 16
        ctx->r5 = S32(ctx->r16 << 16);
            goto L_80142CF0;
    }
    // 0x80142CB8: sll         $a1, $s0, 16
    ctx->r5 = S32(ctx->r16 << 16);
    // 0x80142CBC: slti        $at, $s0, 0x127
    ctx->r1 = SIGNED(ctx->r16) < 0X127 ? 1 : 0;
    // 0x80142CC0: beql        $at, $zero, L_80142CF4
    if (ctx->r1 == 0) {
        // 0x80142CC4: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_80142CF4;
    }
    goto skip_3;
    // 0x80142CC4: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    skip_3:
    // 0x80142CC8: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80142CCC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80142CD0: jal         0x80059CB8
    // 0x80142CD4: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    func_80059CB8(rdram, ctx);
        goto after_1;
    // 0x80142CD4: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    after_1:
    // 0x80142CD8: beql        $v0, $zero, L_80142D14
    if (ctx->r2 == 0) {
        // 0x80142CDC: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142D14;
    }
    goto skip_4;
    // 0x80142CDC: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_4:
    // 0x80142CE0: lw          $t0, 0xC($s2)
    ctx->r8 = MEM_W(ctx->r18, 0XC);
    // 0x80142CE4: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80142CE8: b           L_80142D10
    // 0x80142CEC: sw          $t1, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r9;
        goto L_80142D10;
    // 0x80142CEC: sw          $t1, 0xC($s2)
    MEM_W(0XC, ctx->r18) = ctx->r9;
L_80142CF0:
    // 0x80142CF0: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
L_80142CF4:
    // 0x80142CF4: jal         0x80142FA8
    // 0x80142CF8: or          $a3, $s3, $zero
    ctx->r7 = ctx->r19 | 0;
    func_80142FA8(rdram, ctx);
        goto after_2;
    // 0x80142CF8: or          $a3, $s3, $zero
    ctx->r7 = ctx->r19 | 0;
    after_2:
    // 0x80142CFC: beql        $v0, $zero, L_80142D14
    if (ctx->r2 == 0) {
        // 0x80142D00: lhu         $v0, 0x0($s1)
        ctx->r2 = MEM_HU(ctx->r17, 0X0);
            goto L_80142D14;
    }
    goto skip_5;
    // 0x80142D00: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
    skip_5:
    // 0x80142D04: lw          $t2, 0x4($s2)
    ctx->r10 = MEM_W(ctx->r18, 0X4);
    // 0x80142D08: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80142D0C: sw          $t3, 0x4($s2)
    MEM_W(0X4, ctx->r18) = ctx->r11;
L_80142D10:
    // 0x80142D10: lhu         $v0, 0x0($s1)
    ctx->r2 = MEM_HU(ctx->r17, 0X0);
L_80142D14:
    // 0x80142D14: addiu       $s1, $s1, 0x20
    ctx->r17 = ADD32(ctx->r17, 0X20);
    // 0x80142D18: andi        $v0, $v0, 0x8000
    ctx->r2 = ctx->r2 & 0X8000;
    // 0x80142D1C: sltiu       $v0, $v0, 0x1
    ctx->r2 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x80142D20: bnel        $v0, $zero, L_80142C74
    if (ctx->r2 != 0) {
        // 0x80142D24: lh          $t7, 0x10($s1)
        ctx->r15 = MEM_H(ctx->r17, 0X10);
            goto L_80142C74;
    }
    goto skip_6;
    // 0x80142D24: lh          $t7, 0x10($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X10);
    skip_6:
L_80142D28:
    // 0x80142D28: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80142D2C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80142D30: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80142D34: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80142D38: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80142D3C: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80142D40: jr          $ra
    // 0x80142D44: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80142D44: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_80142D48(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142D48: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142D4C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142D50: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x80142D54: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x80142D58: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x80142D5C: lhu         $t6, 0x2($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X2);
    // 0x80142D60: lui         $v0, 0x801A
    ctx->r2 = S32(0X801A << 16);
    // 0x80142D64: addiu       $v0, $v0, -0x2EB0
    ctx->r2 = ADD32(ctx->r2, -0X2EB0);
    // 0x80142D68: and         $t8, $t6, $at
    ctx->r24 = ctx->r14 & ctx->r1;
    // 0x80142D6C: andi        $t9, $t8, 0xBFFF
    ctx->r25 = ctx->r24 & 0XBFFF;
    // 0x80142D70: sh          $t8, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r24;
    // 0x80142D74: sh          $t9, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r25;
    // 0x80142D78: lw          $t0, 0x4($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X4);
    // 0x80142D7C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80142D80: addiu       $t1, $t0, -0x1
    ctx->r9 = ADD32(ctx->r8, -0X1);
    // 0x80142D84: sw          $t1, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r9;
    // 0x80142D88: lw          $a0, 0x6C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X6C);
    // 0x80142D8C: sw          $a1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r5;
    // 0x80142D90: jal         0x80142E90
    // 0x80142D94: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    func_80142E90(rdram, ctx);
        goto after_0;
    // 0x80142D94: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_0:
    // 0x80142D98: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80142D9C: beq         $v0, $zero, L_80142DB0
    if (ctx->r2 == 0) {
        // 0x80142DA0: lw          $a1, 0x20($sp)
        ctx->r5 = MEM_W(ctx->r29, 0X20);
            goto L_80142DB0;
    }
    // 0x80142DA0: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80142DA4: lhu         $t2, 0x2($v1)
    ctx->r10 = MEM_HU(ctx->r3, 0X2);
    // 0x80142DA8: ori         $t3, $t2, 0x1000
    ctx->r11 = ctx->r10 | 0X1000;
    // 0x80142DAC: sh          $t3, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r11;
L_80142DB0:
    // 0x80142DB0: lhu         $t4, 0x2($v1)
    ctx->r12 = MEM_HU(ctx->r3, 0X2);
    // 0x80142DB4: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80142DB8: andi        $t5, $t4, 0xDFFF
    ctx->r13 = ctx->r12 & 0XDFFF;
    // 0x80142DBC: jal         0x80002CE0
    // 0x80142DC0: sh          $t5, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r13;
    func_80002CE0(rdram, ctx);
        goto after_1;
    // 0x80142DC0: sh          $t5, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r13;
    after_1:
    // 0x80142DC4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142DC8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80142DCC: jr          $ra
    // 0x80142DD0: nop

    return;
    // 0x80142DD0: nop

;}
RECOMP_FUNC void func_80142DD4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142DD4: jr          $ra
    // 0x80142DD8: lw          $v0, 0x3C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X3C);
    return;
    // 0x80142DD8: lw          $v0, 0x3C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X3C);
;}
RECOMP_FUNC void func_80142DDC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142DDC: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80142DE0: lw          $a0, -0x2E8C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X2E8C);
    // 0x80142DE4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142DE8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142DEC: beql        $a0, $zero, L_80142E14
    if (ctx->r4 == 0) {
        // 0x80142DF0: addiu       $v0, $zero, -0x1
        ctx->r2 = ADD32(0, -0X1);
            goto L_80142E14;
    }
    goto skip_0;
    // 0x80142DF0: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    skip_0:
    // 0x80142DF4: jal         0x80142E90
    // 0x80142DF8: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    func_80142E90(rdram, ctx);
        goto after_0;
    // 0x80142DF8: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    after_0:
    // 0x80142DFC: beq         $v0, $zero, L_80142E10
    if (ctx->r2 == 0) {
        // 0x80142E00: lw          $a0, 0x1C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X1C);
            goto L_80142E10;
    }
    // 0x80142E00: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80142E04: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x80142E08: b           L_80142E14
    // 0x80142E0C: lhu         $v0, 0x14($v1)
    ctx->r2 = MEM_HU(ctx->r3, 0X14);
        goto L_80142E14;
    // 0x80142E0C: lhu         $v0, 0x14($v1)
    ctx->r2 = MEM_HU(ctx->r3, 0X14);
L_80142E10:
    // 0x80142E10: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80142E14:
    // 0x80142E14: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142E18: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80142E1C: jr          $ra
    // 0x80142E20: nop

    return;
    // 0x80142E20: nop

;}
RECOMP_FUNC void func_80142E24(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142E24: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80142E28: lw          $a0, -0x2E8C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X2E8C);
    // 0x80142E2C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142E30: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142E34: beql        $a0, $zero, L_80142E58
    if (ctx->r4 == 0) {
        // 0x80142E38: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80142E58;
    }
    goto skip_0;
    // 0x80142E38: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80142E3C: jal         0x80142E90
    // 0x80142E40: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    func_80142E90(rdram, ctx);
        goto after_0;
    // 0x80142E40: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    after_0:
    // 0x80142E44: beq         $v0, $zero, L_80142E54
    if (ctx->r2 == 0) {
        // 0x80142E48: lw          $a0, 0x1C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X1C);
            goto L_80142E54;
    }
    // 0x80142E48: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x80142E4C: b           L_80142E58
    // 0x80142E50: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
        goto L_80142E58;
    // 0x80142E50: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
L_80142E54:
    // 0x80142E54: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80142E58:
    // 0x80142E58: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142E5C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80142E60: jr          $ra
    // 0x80142E64: nop

    return;
    // 0x80142E64: nop

;}
RECOMP_FUNC void func_80142E68(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142E68: lbu         $t6, 0x38($a0)
    ctx->r14 = MEM_BU(ctx->r4, 0X38);
    // 0x80142E6C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80142E70: ori         $t7, $t6, 0x1
    ctx->r15 = ctx->r14 | 0X1;
    // 0x80142E74: jr          $ra
    // 0x80142E78: sb          $t7, 0x38($a0)
    MEM_B(0X38, ctx->r4) = ctx->r15;
    return;
    // 0x80142E78: sb          $t7, 0x38($a0)
    MEM_B(0X38, ctx->r4) = ctx->r15;
;}
RECOMP_FUNC void func_80142E7C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142E7C: lbu         $t6, 0x38($a0)
    ctx->r14 = MEM_BU(ctx->r4, 0X38);
    // 0x80142E80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80142E84: andi        $t7, $t6, 0xFFFE
    ctx->r15 = ctx->r14 & 0XFFFE;
    // 0x80142E88: jr          $ra
    // 0x80142E8C: sb          $t7, 0x38($a0)
    MEM_B(0X38, ctx->r4) = ctx->r15;
    return;
    // 0x80142E8C: sb          $t7, 0x38($a0)
    MEM_B(0X38, ctx->r4) = ctx->r15;
;}
RECOMP_FUNC void func_80142E90(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142E90: lbu         $t6, 0x38($a0)
    ctx->r14 = MEM_BU(ctx->r4, 0X38);
    // 0x80142E94: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80142E98: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x80142E9C: beq         $t7, $zero, L_80142EAC
    if (ctx->r15 == 0) {
        // 0x80142EA0: nop
    
            goto L_80142EAC;
    }
    // 0x80142EA0: nop

    // 0x80142EA4: jr          $ra
    // 0x80142EA8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80142EA8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80142EAC:
    // 0x80142EAC: jr          $ra
    // 0x80142EB0: nop

    return;
    // 0x80142EB0: nop

;}
RECOMP_FUNC void func_80142EB4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142EB4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80142EB8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142EBC: jal         0x80142E90
    // 0x80142EC0: nop

    func_80142E90(rdram, ctx);
        goto after_0;
    // 0x80142EC0: nop

    after_0:
    // 0x80142EC4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80142EC8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80142ECC: jr          $ra
    // 0x80142ED0: nop

    return;
    // 0x80142ED0: nop

;}
RECOMP_FUNC void func_80142ED4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142ED4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80142ED8: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80142EDC: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80142EE0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80142EE4: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80142EE8: jal         0x80002560
    // 0x80142EEC: addiu       $a0, $zero, 0x1B3
    ctx->r4 = ADD32(0, 0X1B3);
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x80142EEC: addiu       $a0, $zero, 0x1B3
    ctx->r4 = ADD32(0, 0X1B3);
    after_0:
    // 0x80142EF0: bne         $v0, $zero, L_80142F00
    if (ctx->r2 != 0) {
        // 0x80142EF4: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_80142F00;
    }
    // 0x80142EF4: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x80142EF8: b           L_80142F74
    // 0x80142EFC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142F74;
    // 0x80142EFC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80142F00:
    // 0x80142F00: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
    // 0x80142F04: bnel        $v1, $zero, L_80142F18
    if (ctx->r3 != 0) {
        // 0x80142F08: lhu         $t6, 0x14($v1)
        ctx->r14 = MEM_HU(ctx->r3, 0X14);
            goto L_80142F18;
    }
    goto skip_0;
    // 0x80142F08: lhu         $t6, 0x14($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X14);
    skip_0:
    // 0x80142F0C: b           L_80142F74
    // 0x80142F10: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80142F74;
    // 0x80142F10: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80142F14: lhu         $t6, 0x14($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X14);
L_80142F18:
    // 0x80142F18: addiu       $a0, $zero, 0x1B3
    ctx->r4 = ADD32(0, 0X1B3);
    // 0x80142F1C: bne         $s1, $t6, L_80142F2C
    if (ctx->r17 != ctx->r14) {
        // 0x80142F20: nop
    
            goto L_80142F2C;
    }
    // 0x80142F20: nop

    // 0x80142F24: b           L_80142F74
    // 0x80142F28: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
        goto L_80142F74;
    // 0x80142F28: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80142F2C:
    // 0x80142F2C: jal         0x80002510
    // 0x80142F30: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80002510(rdram, ctx);
        goto after_1;
    // 0x80142F30: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x80142F34: beq         $v0, $zero, L_80142F70
    if (ctx->r2 == 0) {
        // 0x80142F38: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_80142F70;
    }
    // 0x80142F38: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80142F3C:
    // 0x80142F3C: lw          $v1, 0x70($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X70);
    // 0x80142F40: addiu       $a0, $zero, 0x1B3
    ctx->r4 = ADD32(0, 0X1B3);
    // 0x80142F44: beq         $v1, $zero, L_80142F60
    if (ctx->r3 == 0) {
        // 0x80142F48: nop
    
            goto L_80142F60;
    }
    // 0x80142F48: nop

    // 0x80142F4C: lhu         $t7, 0x14($v1)
    ctx->r15 = MEM_HU(ctx->r3, 0X14);
    // 0x80142F50: bne         $s1, $t7, L_80142F60
    if (ctx->r17 != ctx->r15) {
        // 0x80142F54: nop
    
            goto L_80142F60;
    }
    // 0x80142F54: nop

    // 0x80142F58: b           L_80142F74
    // 0x80142F5C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
        goto L_80142F74;
    // 0x80142F5C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80142F60:
    // 0x80142F60: jal         0x80002510
    // 0x80142F64: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80002510(rdram, ctx);
        goto after_2;
    // 0x80142F64: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_2:
    // 0x80142F68: bne         $v0, $zero, L_80142F3C
    if (ctx->r2 != 0) {
        // 0x80142F6C: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_80142F3C;
    }
    // 0x80142F6C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80142F70:
    // 0x80142F70: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80142F74:
    // 0x80142F74: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80142F78: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80142F7C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80142F80: jr          $ra
    // 0x80142F84: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x80142F84: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80142F90(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142F90: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142F94: jr          $ra
    // 0x80142F98: swc1        $f12, -0x2EB0($at)
    MEM_W(-0X2EB0, ctx->r1) = ctx->f12.u32l;
    return;
    // 0x80142F98: swc1        $f12, -0x2EB0($at)
    MEM_W(-0X2EB0, ctx->r1) = ctx->f12.u32l;
;}
RECOMP_FUNC void func_80142F9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142F9C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80142FA0: jr          $ra
    // 0x80142FA4: lwc1        $f0, -0x2EB0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X2EB0);
    return;
    // 0x80142FA4: lwc1        $f0, -0x2EB0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X2EB0);
;}
RECOMP_FUNC void func_80142FA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80142FA8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80142FAC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80142FB0: bne         $a0, $zero, L_801430C4
    if (ctx->r4 != 0) {
        // 0x80142FB4: sw          $a1, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r5;
            goto L_801430C4;
    }
    // 0x80142FB4: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80142FB8: lh          $v0, 0x1E($sp)
    ctx->r2 = MEM_H(ctx->r29, 0X1E);
    // 0x80142FBC: andi        $v0, $v0, 0x7FF
    ctx->r2 = ctx->r2 & 0X7FF;
    // 0x80142FC0: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x80142FC4: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x80142FC8: slti        $at, $v0, 0x16
    ctx->r1 = SIGNED(ctx->r2) < 0X16 ? 1 : 0;
    // 0x80142FCC: bne         $at, $zero, L_80142FE8
    if (ctx->r1 != 0) {
        // 0x80142FD0: slti        $at, $v0, 0x88
        ctx->r1 = SIGNED(ctx->r2) < 0X88 ? 1 : 0;
            goto L_80142FE8;
    }
    // 0x80142FD0: slti        $at, $v0, 0x88
    ctx->r1 = SIGNED(ctx->r2) < 0X88 ? 1 : 0;
    // 0x80142FD4: beql        $at, $zero, L_80142FEC
    if (ctx->r1 == 0) {
        // 0x80142FD8: slti        $at, $v0, 0x8D
        ctx->r1 = SIGNED(ctx->r2) < 0X8D ? 1 : 0;
            goto L_80142FEC;
    }
    goto skip_0;
    // 0x80142FD8: slti        $at, $v0, 0x8D
    ctx->r1 = SIGNED(ctx->r2) < 0X8D ? 1 : 0;
    skip_0:
    // 0x80142FDC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80142FE0: b           L_801430C4
    // 0x80142FE4: lw          $a0, -0x5434($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5434);
        goto L_801430C4;
    // 0x80142FE4: lw          $a0, -0x5434($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5434);
L_80142FE8:
    // 0x80142FE8: slti        $at, $v0, 0x8D
    ctx->r1 = SIGNED(ctx->r2) < 0X8D ? 1 : 0;
L_80142FEC:
    // 0x80142FEC: bne         $at, $zero, L_80143008
    if (ctx->r1 != 0) {
        // 0x80142FF0: slti        $at, $v0, 0xA3
        ctx->r1 = SIGNED(ctx->r2) < 0XA3 ? 1 : 0;
            goto L_80143008;
    }
    // 0x80142FF0: slti        $at, $v0, 0xA3
    ctx->r1 = SIGNED(ctx->r2) < 0XA3 ? 1 : 0;
    // 0x80142FF4: beql        $at, $zero, L_8014300C
    if (ctx->r1 == 0) {
        // 0x80142FF8: slti        $at, $v0, 0xA9
        ctx->r1 = SIGNED(ctx->r2) < 0XA9 ? 1 : 0;
            goto L_8014300C;
    }
    goto skip_1;
    // 0x80142FF8: slti        $at, $v0, 0xA9
    ctx->r1 = SIGNED(ctx->r2) < 0XA9 ? 1 : 0;
    skip_1:
    // 0x80142FFC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80143000: b           L_801430C4
    // 0x80143004: lw          $a0, -0x5424($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5424);
        goto L_801430C4;
    // 0x80143004: lw          $a0, -0x5424($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5424);
L_80143008:
    // 0x80143008: slti        $at, $v0, 0xA9
    ctx->r1 = SIGNED(ctx->r2) < 0XA9 ? 1 : 0;
L_8014300C:
    // 0x8014300C: bne         $at, $zero, L_80143028
    if (ctx->r1 != 0) {
        // 0x80143010: slti        $at, $v0, 0xFD
        ctx->r1 = SIGNED(ctx->r2) < 0XFD ? 1 : 0;
            goto L_80143028;
    }
    // 0x80143010: slti        $at, $v0, 0xFD
    ctx->r1 = SIGNED(ctx->r2) < 0XFD ? 1 : 0;
    // 0x80143014: beql        $at, $zero, L_8014302C
    if (ctx->r1 == 0) {
        // 0x80143018: slti        $at, $v0, 0x102
        ctx->r1 = SIGNED(ctx->r2) < 0X102 ? 1 : 0;
            goto L_8014302C;
    }
    goto skip_2;
    // 0x80143018: slti        $at, $v0, 0x102
    ctx->r1 = SIGNED(ctx->r2) < 0X102 ? 1 : 0;
    skip_2:
    // 0x8014301C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80143020: b           L_801430C4
    // 0x80143024: lw          $a0, -0x540C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X540C);
        goto L_801430C4;
    // 0x80143024: lw          $a0, -0x540C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X540C);
L_80143028:
    // 0x80143028: slti        $at, $v0, 0x102
    ctx->r1 = SIGNED(ctx->r2) < 0X102 ? 1 : 0;
L_8014302C:
    // 0x8014302C: bne         $at, $zero, L_80143048
    if (ctx->r1 != 0) {
        // 0x80143030: slti        $at, $v0, 0x106
        ctx->r1 = SIGNED(ctx->r2) < 0X106 ? 1 : 0;
            goto L_80143048;
    }
    // 0x80143030: slti        $at, $v0, 0x106
    ctx->r1 = SIGNED(ctx->r2) < 0X106 ? 1 : 0;
    // 0x80143034: beql        $at, $zero, L_8014304C
    if (ctx->r1 == 0) {
        // 0x80143038: slti        $at, $v0, 0x15E
        ctx->r1 = SIGNED(ctx->r2) < 0X15E ? 1 : 0;
            goto L_8014304C;
    }
    goto skip_3;
    // 0x80143038: slti        $at, $v0, 0x15E
    ctx->r1 = SIGNED(ctx->r2) < 0X15E ? 1 : 0;
    skip_3:
    // 0x8014303C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80143040: b           L_801430C4
    // 0x80143044: lw          $a0, -0x5414($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5414);
        goto L_801430C4;
    // 0x80143044: lw          $a0, -0x5414($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5414);
L_80143048:
    // 0x80143048: slti        $at, $v0, 0x15E
    ctx->r1 = SIGNED(ctx->r2) < 0X15E ? 1 : 0;
L_8014304C:
    // 0x8014304C: bne         $at, $zero, L_80143068
    if (ctx->r1 != 0) {
        // 0x80143050: slti        $at, $v0, 0x184
        ctx->r1 = SIGNED(ctx->r2) < 0X184 ? 1 : 0;
            goto L_80143068;
    }
    // 0x80143050: slti        $at, $v0, 0x184
    ctx->r1 = SIGNED(ctx->r2) < 0X184 ? 1 : 0;
    // 0x80143054: beql        $at, $zero, L_8014306C
    if (ctx->r1 == 0) {
        // 0x80143058: slti        $at, $v0, 0x185
        ctx->r1 = SIGNED(ctx->r2) < 0X185 ? 1 : 0;
            goto L_8014306C;
    }
    goto skip_4;
    // 0x80143058: slti        $at, $v0, 0x185
    ctx->r1 = SIGNED(ctx->r2) < 0X185 ? 1 : 0;
    skip_4:
    // 0x8014305C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80143060: b           L_801430C4
    // 0x80143064: lw          $a0, -0x5404($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5404);
        goto L_801430C4;
    // 0x80143064: lw          $a0, -0x5404($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5404);
L_80143068:
    // 0x80143068: slti        $at, $v0, 0x185
    ctx->r1 = SIGNED(ctx->r2) < 0X185 ? 1 : 0;
L_8014306C:
    // 0x8014306C: bne         $at, $zero, L_80143088
    if (ctx->r1 != 0) {
        // 0x80143070: slti        $at, $v0, 0x1AB
        ctx->r1 = SIGNED(ctx->r2) < 0X1AB ? 1 : 0;
            goto L_80143088;
    }
    // 0x80143070: slti        $at, $v0, 0x1AB
    ctx->r1 = SIGNED(ctx->r2) < 0X1AB ? 1 : 0;
    // 0x80143074: beql        $at, $zero, L_8014308C
    if (ctx->r1 == 0) {
        // 0x80143078: slti        $at, $v0, 0x1AB
        ctx->r1 = SIGNED(ctx->r2) < 0X1AB ? 1 : 0;
            goto L_8014308C;
    }
    goto skip_5;
    // 0x80143078: slti        $at, $v0, 0x1AB
    ctx->r1 = SIGNED(ctx->r2) < 0X1AB ? 1 : 0;
    skip_5:
    // 0x8014307C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80143080: b           L_801430C4
    // 0x80143084: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
        goto L_801430C4;
    // 0x80143084: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
L_80143088:
    // 0x80143088: slti        $at, $v0, 0x1AB
    ctx->r1 = SIGNED(ctx->r2) < 0X1AB ? 1 : 0;
L_8014308C:
    // 0x8014308C: bne         $at, $zero, L_801430A8
    if (ctx->r1 != 0) {
        // 0x80143090: slti        $at, $v0, 0x1B3
        ctx->r1 = SIGNED(ctx->r2) < 0X1B3 ? 1 : 0;
            goto L_801430A8;
    }
    // 0x80143090: slti        $at, $v0, 0x1B3
    ctx->r1 = SIGNED(ctx->r2) < 0X1B3 ? 1 : 0;
    // 0x80143094: beql        $at, $zero, L_801430AC
    if (ctx->r1 == 0) {
        // 0x80143098: slti        $at, $v0, 0x1B3
        ctx->r1 = SIGNED(ctx->r2) < 0X1B3 ? 1 : 0;
            goto L_801430AC;
    }
    goto skip_6;
    // 0x80143098: slti        $at, $v0, 0x1B3
    ctx->r1 = SIGNED(ctx->r2) < 0X1B3 ? 1 : 0;
    skip_6:
    // 0x8014309C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801430A0: b           L_801430C4
    // 0x801430A4: lw          $a0, -0x541C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X541C);
        goto L_801430C4;
    // 0x801430A4: lw          $a0, -0x541C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X541C);
L_801430A8:
    // 0x801430A8: slti        $at, $v0, 0x1B3
    ctx->r1 = SIGNED(ctx->r2) < 0X1B3 ? 1 : 0;
L_801430AC:
    // 0x801430AC: bne         $at, $zero, L_801430C4
    if (ctx->r1 != 0) {
        // 0x801430B0: slti        $at, $v0, 0x285
        ctx->r1 = SIGNED(ctx->r2) < 0X285 ? 1 : 0;
            goto L_801430C4;
    }
    // 0x801430B0: slti        $at, $v0, 0x285
    ctx->r1 = SIGNED(ctx->r2) < 0X285 ? 1 : 0;
    // 0x801430B4: beql        $at, $zero, L_801430C8
    if (ctx->r1 == 0) {
        // 0x801430B8: lhu         $a1, 0x1E($sp)
        ctx->r5 = MEM_HU(ctx->r29, 0X1E);
            goto L_801430C8;
    }
    goto skip_7;
    // 0x801430B8: lhu         $a1, 0x1E($sp)
    ctx->r5 = MEM_HU(ctx->r29, 0X1E);
    skip_7:
    // 0x801430BC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801430C0: lw          $a0, -0x542C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X542C);
L_801430C4:
    // 0x801430C4: lhu         $a1, 0x1E($sp)
    ctx->r5 = MEM_HU(ctx->r29, 0X1E);
L_801430C8:
    // 0x801430C8: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x801430CC: jal         0x80002410
    // 0x801430D0: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    func_80002410(rdram, ctx);
        goto after_0;
    // 0x801430D0: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    after_0:
    // 0x801430D4: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x801430D8: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x801430DC: bne         $v0, $zero, L_801430EC
    if (ctx->r2 != 0) {
        // 0x801430E0: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_801430EC;
    }
    // 0x801430E0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x801430E4: b           L_8014313C
    // 0x801430E8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8014313C;
    // 0x801430E8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801430EC:
    // 0x801430EC: beq         $a3, $zero, L_80143104
    if (ctx->r7 == 0) {
        // 0x801430F0: sw          $a2, 0x70($v0)
        MEM_W(0X70, ctx->r2) = ctx->r6;
            goto L_80143104;
    }
    // 0x801430F0: sw          $a2, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->r6;
    // 0x801430F4: lhu         $t6, 0x2($a2)
    ctx->r14 = MEM_HU(ctx->r6, 0X2);
    // 0x801430F8: ori         $t7, $t6, 0x2000
    ctx->r15 = ctx->r14 | 0X2000;
    // 0x801430FC: sh          $t7, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r15;
    // 0x80143100: sw          $a3, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->r7;
L_80143104:
    // 0x80143104: lhu         $t8, 0x2($a2)
    ctx->r24 = MEM_HU(ctx->r6, 0X2);
    // 0x80143108: lui         $t3, 0x8014
    ctx->r11 = S32(0X8014 << 16);
    // 0x8014310C: addiu       $t3, $t3, 0x2D48
    ctx->r11 = ADD32(ctx->r11, 0X2D48);
    // 0x80143110: ori         $t0, $t8, 0x8000
    ctx->r8 = ctx->r24 | 0X8000;
    // 0x80143114: sh          $t0, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r8;
    // 0x80143118: ori         $t1, $t0, 0x1
    ctx->r9 = ctx->r8 | 0X1;
    // 0x8014311C: bne         $a3, $zero, L_80143134
    if (ctx->r7 != 0) {
        // 0x80143120: sh          $t1, 0x2($a2)
        MEM_H(0X2, ctx->r6) = ctx->r9;
            goto L_80143134;
    }
    // 0x80143120: sh          $t1, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r9;
    // 0x80143124: lui         $t2, 0x8014
    ctx->r10 = S32(0X8014 << 16);
    // 0x80143128: addiu       $t2, $t2, 0x2030
    ctx->r10 = ADD32(ctx->r10, 0X2030);
    // 0x8014312C: b           L_80143138
    // 0x80143130: sw          $t2, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r10;
        goto L_80143138;
    // 0x80143130: sw          $t2, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r10;
L_80143134:
    // 0x80143134: sw          $t3, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r11;
L_80143138:
    // 0x80143138: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_8014313C:
    // 0x8014313C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143140: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143144: jr          $ra
    // 0x80143148: nop

    return;
    // 0x80143148: nop

;}
RECOMP_FUNC void func_8014314C(uint8_t* rdram, recomp_context* ctx) {
    // PATCH: Always return 1 (ready). Without this, the overlay's inner state
    // machine hangs and DL generation stops after frame 4. The NI file loading
    // doesn't work because obj+0x70 (file handle) is never set.
    {
        ctx->r2 = 1;
        return;
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014314C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143150: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143154: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80143158: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x8014315C: andi        $t7, $t6, 0x2000
    ctx->r15 = ctx->r14 & 0X2000;
    // 0x80143160: beq         $t7, $zero, L_80143180
    if (ctx->r15 == 0) {
        // 0x80143164: nop
    
            goto L_80143180;
    }
    // 0x80143164: nop

    // 0x80143168: jal         0x80142E90
    // 0x8014316C: lw          $a0, 0x6C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X6C);
    func_80142E90(rdram, ctx);
        goto after_0;
    // 0x8014316C: lw          $a0, 0x6C($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X6C);
    after_0:
    // 0x80143170: bnel        $v0, $zero, L_8014319C
    if (ctx->r2 != 0) {
        // 0x80143174: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8014319C;
    }
    goto skip_0;
    // 0x80143174: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80143178: b           L_8014319C
    // 0x8014317C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8014319C;
    // 0x8014317C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80143180:
    // 0x80143180: jal         0x80142388
    // 0x80143184: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80142388(rdram, ctx);
        goto after_1;
    // 0x80143184: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_1:
    // 0x80143188: beql        $v0, $zero, L_8014319C
    if (ctx->r2 == 0) {
        // 0x8014318C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8014319C;
    }
    goto skip_1;
    // 0x8014318C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x80143190: b           L_8014319C
    // 0x80143194: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8014319C;
    // 0x80143194: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80143198: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8014319C:
    // 0x8014319C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801431A0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801431A4: jr          $ra
    // 0x801431A8: nop

    return;
    // 0x801431A8: nop

;}
RECOMP_FUNC void func_801431AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801431AC: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x801431B0: lwc1        $f4, 0x4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801431B4: swc1        $f4, 0x50($a1)
    MEM_W(0X50, ctx->r5) = ctx->f4.u32l;
    // 0x801431B8: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801431BC: swc1        $f6, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f6.u32l;
    // 0x801431C0: lwc1        $f8, 0xC($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0XC);
    // 0x801431C4: jr          $ra
    // 0x801431C8: swc1        $f8, 0x58($a1)
    MEM_W(0X58, ctx->r5) = ctx->f8.u32l;
    return;
    // 0x801431C8: swc1        $f8, 0x58($a1)
    MEM_W(0X58, ctx->r5) = ctx->f8.u32l;
;}
RECOMP_FUNC void func_801431CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801431CC: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x801431D0: lwc1        $f4, 0x4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801431D4: swc1        $f4, 0x50($a1)
    MEM_W(0X50, ctx->r5) = ctx->f4.u32l;
    // 0x801431D8: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801431DC: swc1        $f6, 0x54($a1)
    MEM_W(0X54, ctx->r5) = ctx->f6.u32l;
    // 0x801431E0: lwc1        $f8, 0xC($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0XC);
    // 0x801431E4: swc1        $f8, 0x58($a1)
    MEM_W(0X58, ctx->r5) = ctx->f8.u32l;
    // 0x801431E8: lhu         $t6, 0x14($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X14);
    // 0x801431EC: sh          $t6, 0x5C($a1)
    MEM_H(0X5C, ctx->r5) = ctx->r14;
    // 0x801431F0: lhu         $t7, 0x16($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X16);
    // 0x801431F4: jr          $ra
    // 0x801431F8: sh          $t7, 0x5E($a1)
    MEM_H(0X5E, ctx->r5) = ctx->r15;
    return;
    // 0x801431F8: sh          $t7, 0x5E($a1)
    MEM_H(0X5E, ctx->r5) = ctx->r15;
;}
RECOMP_FUNC void func_801431FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801431FC: sw          $a1, 0x70($a0)
    MEM_W(0X70, ctx->r4) = ctx->r5;
    // 0x80143200: sw          $a2, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = ctx->r6;
    // 0x80143204: lhu         $t6, 0x2($a1)
    ctx->r14 = MEM_HU(ctx->r5, 0X2);
    // 0x80143208: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8014320C: ori         $t7, $t6, 0x2000
    ctx->r15 = ctx->r14 | 0X2000;
    // 0x80143210: jr          $ra
    // 0x80143214: sh          $t7, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r15;
    return;
    // 0x80143214: sh          $t7, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r15;
;}
RECOMP_FUNC void func_80143218(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143218: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x8014321C: sw          $zero, 0x6C($a0)
    MEM_W(0X6C, ctx->r4) = 0;
    // 0x80143220: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80143224: lhu         $t6, 0x2($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X2);
    // 0x80143228: andi        $t7, $t6, 0xDFFF
    ctx->r15 = ctx->r14 & 0XDFFF;
    // 0x8014322C: jr          $ra
    // 0x80143230: sh          $t7, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r15;
    return;
    // 0x80143230: sh          $t7, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r15;
;}
RECOMP_FUNC void func_80143234(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143234: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80143238: beq         $v0, $zero, L_8014324C
    if (ctx->r2 == 0) {
        // 0x8014323C: nop
    
            goto L_8014324C;
    }
    // 0x8014323C: nop

    // 0x80143240: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x80143244: ori         $t7, $t6, 0x4000
    ctx->r15 = ctx->r14 | 0X4000;
    // 0x80143248: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
L_8014324C:
    // 0x8014324C: jr          $ra
    // 0x80143250: nop

    return;
    // 0x80143250: nop

;}
RECOMP_FUNC void func_80143254(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143254: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80143258: beq         $v0, $zero, L_8014326C
    if (ctx->r2 == 0) {
        // 0x8014325C: nop
    
            goto L_8014326C;
    }
    // 0x8014325C: nop

    // 0x80143260: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x80143264: ori         $t7, $t6, 0x800
    ctx->r15 = ctx->r14 | 0X800;
    // 0x80143268: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
L_8014326C:
    // 0x8014326C: jr          $ra
    // 0x80143270: nop

    return;
    // 0x80143270: nop

;}
RECOMP_FUNC void func_80143274(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143274: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143278: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8014327C: jal         0x80143254
    // 0x80143280: nop

    func_80143254(rdram, ctx);
        goto after_0;
    // 0x80143280: nop

    after_0:
    // 0x80143284: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143288: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8014328C: jr          $ra
    // 0x80143290: nop

    return;
    // 0x80143290: nop

;}
RECOMP_FUNC void func_80143294(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143294: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143298: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8014329C: jal         0x80143254
    // 0x801432A0: nop

    func_80143254(rdram, ctx);
        goto after_0;
    // 0x801432A0: nop

    after_0:
    // 0x801432A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801432A8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801432AC: jr          $ra
    // 0x801432B0: nop

    return;
    // 0x801432B0: nop

;}
RECOMP_FUNC void func_801432C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801432C0: jr          $ra
    // 0x801432C4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x801432C4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_801432C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801432C8: jr          $ra
    // 0x801432CC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x801432CC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_801432D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801432D0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801432D4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801432D8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x801432DC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801432E0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801432E4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801432E8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x801432EC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x801432F0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801432F4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801432F8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801432FC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80143300: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80143304: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80143308: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8014330C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80143310: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80143314: lw          $t9, -0x2F44($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2F44);
    // 0x80143318: jalr        $t9
    // 0x8014331C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8014331C: nop

    after_0:
    // 0x80143320: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80143324: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80143328: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8014332C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80143330: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143334: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143338: jr          $ra
    // 0x8014333C: nop

    return;
    // 0x8014333C: nop

;}
RECOMP_FUNC void func_80143340(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143340: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143344: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143348: lw          $v1, 0x70($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X70);
    // 0x8014334C: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80143350: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x80143354: lhu         $t9, 0x18($v1)
    ctx->r25 = MEM_HU(ctx->r3, 0X18);
    // 0x80143358: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x8014335C: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80143360: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x80143364: andi        $t0, $t9, 0xFFF
    ctx->r8 = ctx->r25 & 0XFFF;
    // 0x80143368: lw          $t8, -0x2F00($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X2F00);
    // 0x8014336C: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x80143370: subu        $t1, $t1, $t0
    ctx->r9 = SUB32(ctx->r9, ctx->r8);
    // 0x80143374: sll         $t1, $t1, 2
    ctx->r9 = S32(ctx->r9 << 2);
    // 0x80143378: addu        $v0, $t8, $t1
    ctx->r2 = ADD32(ctx->r24, ctx->r9);
    // 0x8014337C: sw          $v0, 0x40($a0)
    MEM_W(0X40, ctx->r4) = ctx->r2;
    // 0x80143380: sw          $zero, 0x4C($a0)
    MEM_W(0X4C, ctx->r4) = 0;
    // 0x80143384: lhu         $t2, 0xA($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0XA);
    // 0x80143388: lw          $t3, 0x10($a0)
    ctx->r11 = MEM_W(ctx->r4, 0X10);
    // 0x8014338C: lui         $t4, 0x8014
    ctx->r12 = S32(0X8014 << 16);
    // 0x80143390: addiu       $t4, $t4, 0x3730
    ctx->r12 = ADD32(ctx->r12, 0X3730);
    // 0x80143394: sw          $t4, 0x10($a0)
    MEM_W(0X10, ctx->r4) = ctx->r12;
    // 0x80143398: sh          $t2, 0x56($a0)
    MEM_H(0X56, ctx->r4) = ctx->r10;
    // 0x8014339C: sw          $t3, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r11;
    // 0x801433A0: jal         0x80143234
    // 0x801433A4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80143234(rdram, ctx);
        goto after_0;
    // 0x801433A4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x801433A8: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x801433AC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801433B0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801433B4: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801433B8: jalr        $t9
    // 0x801433BC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801433BC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
    // 0x801433C0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801433C4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801433C8: jr          $ra
    // 0x801433CC: nop

    return;
    // 0x801433CC: nop

;}
RECOMP_FUNC void func_801433D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801433D0: addiu       $sp, $sp, -0x238
    ctx->r29 = ADD32(ctx->r29, -0X238);
    // 0x801433D4: sw          $ra, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r31;
    // 0x801433D8: sw          $s5, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r21;
    // 0x801433DC: sw          $s4, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r20;
    // 0x801433E0: or          $s5, $a0, $zero
    ctx->r21 = ctx->r4 | 0;
    // 0x801433E4: sw          $fp, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r30;
    // 0x801433E8: sw          $s7, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r23;
    // 0x801433EC: sw          $s6, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r22;
    // 0x801433F0: sw          $s3, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r19;
    // 0x801433F4: sw          $s2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r18;
    // 0x801433F8: sw          $s1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r17;
    // 0x801433FC: sw          $s0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r16;
    // 0x80143400: sdc1        $f24, 0x38($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X38, ctx->r29);
    // 0x80143404: sdc1        $f22, 0x30($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X30, ctx->r29);
    // 0x80143408: sdc1        $f20, 0x28($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X28, ctx->r29);
    // 0x8014340C: lui         $s4, 0x801D
    ctx->r20 = S32(0X801D << 16);
    // 0x80143410: lw          $s4, -0x53E0($s4)
    ctx->r20 = MEM_W(ctx->r20, -0X53E0);
    // 0x80143414: lw          $a0, 0x70($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X70);
    // 0x80143418: jal         0x801454DC
    // 0x8014341C: lw          $s3, 0x40($s5)
    ctx->r19 = MEM_W(ctx->r21, 0X40);
    func_801454DC(rdram, ctx);
        goto after_0;
    // 0x8014341C: lw          $s3, 0x40($s5)
    ctx->r19 = MEM_W(ctx->r21, 0X40);
    after_0:
    // 0x80143420: beq         $v0, $zero, L_801436F4
    if (ctx->r2 == 0) {
        // 0x80143424: addiu       $v0, $s5, 0x34
        ctx->r2 = ADD32(ctx->r21, 0X34);
            goto L_801436F4;
    }
    // 0x80143424: addiu       $v0, $s5, 0x34
    ctx->r2 = ADD32(ctx->r21, 0X34);
    // 0x80143428: lw          $t6, 0x18($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X18);
    // 0x8014342C: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x80143430: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80143434: sw          $t7, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r15;
    // 0x80143438: lhu         $t8, 0x8($s3)
    ctx->r24 = MEM_HU(ctx->r19, 0X8);
    // 0x8014343C: slt         $at, $t8, $t7
    ctx->r1 = SIGNED(ctx->r24) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x80143440: beql        $at, $zero, L_801436F8
    if (ctx->r1 == 0) {
        // 0x80143444: lw          $ra, 0x64($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X64);
            goto L_801436F8;
    }
    goto skip_0;
    // 0x80143444: lw          $ra, 0x64($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X64);
    skip_0:
    // 0x80143448: jal         0x80145420
    // 0x8014344C: sw          $zero, 0x18($v0)
    MEM_W(0X18, ctx->r2) = 0;
    func_80145420(rdram, ctx);
        goto after_1;
    // 0x8014344C: sw          $zero, 0x18($v0)
    MEM_W(0X18, ctx->r2) = 0;
    after_1:
    // 0x80143450: lhu         $t2, 0xA($s3)
    ctx->r10 = MEM_HU(ctx->r19, 0XA);
    // 0x80143454: subu        $t3, $t2, $v0
    ctx->r11 = SUB32(ctx->r10, ctx->r2);
    // 0x80143458: sh          $t3, 0x7C($sp)
    MEM_H(0X7C, ctx->r29) = ctx->r11;
    // 0x8014345C: lh          $t4, 0x7C($sp)
    ctx->r12 = MEM_H(ctx->r29, 0X7C);
    // 0x80143460: blezl       $t4, L_801436F8
    if (SIGNED(ctx->r12) <= 0) {
        // 0x80143464: lw          $ra, 0x64($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X64);
            goto L_801436F8;
    }
    goto skip_1;
    // 0x80143464: lw          $ra, 0x64($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X64);
    skip_1:
    // 0x80143468: lw          $t0, 0x4($s3)
    ctx->r8 = MEM_W(ctx->r19, 0X4);
    // 0x8014346C: or          $t1, $zero, $zero
    ctx->r9 = 0 | 0;
    // 0x80143470: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80143474: beq         $t0, $zero, L_80143504
    if (ctx->r8 == 0) {
        // 0x80143478: addiu       $v1, $sp, 0x11C
        ctx->r3 = ADD32(ctx->r29, 0X11C);
            goto L_80143504;
    }
    // 0x80143478: addiu       $v1, $sp, 0x11C
    ctx->r3 = ADD32(ctx->r29, 0X11C);
    // 0x8014347C: addiu       $s2, $sp, 0x80
    ctx->r18 = ADD32(ctx->r29, 0X80);
    // 0x80143480: lw          $t5, 0x0($s3)
    ctx->r13 = MEM_W(ctx->r19, 0X0);
L_80143484:
    // 0x80143484: lwc1        $f8, 0x50($s4)
    ctx->f8.u32l = MEM_W(ctx->r20, 0X50);
    // 0x80143488: lwc1        $f18, 0x54($s4)
    ctx->f18.u32l = MEM_W(ctx->r20, 0X54);
    // 0x8014348C: addu        $s0, $t5, $v0
    ctx->r16 = ADD32(ctx->r13, ctx->r2);
    // 0x80143490: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x80143494: lh          $t7, 0x2($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X2);
    // 0x80143498: lh          $t8, 0x4($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X4);
    // 0x8014349C: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x801434A0: mtc1        $t7, $f10
    ctx->f10.u32l = ctx->r15;
    // 0x801434A4: addiu       $v0, $v0, 0x18
    ctx->r2 = ADD32(ctx->r2, 0X18);
    // 0x801434A8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801434AC: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x801434B0: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x801434B4: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    // 0x801434B8: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801434BC: sub.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f0.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x801434C0: lwc1        $f8, 0x58($s4)
    ctx->f8.u32l = MEM_W(ctx->r20, 0X58);
    // 0x801434C4: sh          $t1, -0x2($s2)
    MEM_H(-0X2, ctx->r18) = ctx->r9;
    // 0x801434C8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801434CC: mul.s       $f10, $f0, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x801434D0: addiu       $t1, $t1, 0x1
    ctx->r9 = ADD32(ctx->r9, 0X1);
    // 0x801434D4: sub.s       $f2, $f16, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x801434D8: sub.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x801434DC: mul.s       $f16, $f2, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f16.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801434E0: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x801434E4: mul.s       $f4, $f12, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x801434E8: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x801434EC: swc1        $f6, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->f6.u32l;
    // 0x801434F0: lw          $t0, 0x4($s3)
    ctx->r8 = MEM_W(ctx->r19, 0X4);
    // 0x801434F4: sltu        $at, $t1, $t0
    ctx->r1 = ctx->r9 < ctx->r8 ? 1 : 0;
    // 0x801434F8: bnel        $at, $zero, L_80143484
    if (ctx->r1 != 0) {
        // 0x801434FC: lw          $t5, 0x0($s3)
        ctx->r13 = MEM_W(ctx->r19, 0X0);
            goto L_80143484;
    }
    goto skip_2;
    // 0x801434FC: lw          $t5, 0x0($s3)
    ctx->r13 = MEM_W(ctx->r19, 0X0);
    skip_2:
    // 0x80143500: or          $t1, $zero, $zero
    ctx->r9 = 0 | 0;
L_80143504:
    // 0x80143504: addiu       $v0, $t0, -0x1
    ctx->r2 = ADD32(ctx->r8, -0X1);
    // 0x80143508: beq         $v0, $zero, L_80143588
    if (ctx->r2 == 0) {
        // 0x8014350C: addiu       $a3, $t0, -0x1
        ctx->r7 = ADD32(ctx->r8, -0X1);
            goto L_80143588;
    }
    // 0x8014350C: addiu       $a3, $t0, -0x1
    ctx->r7 = ADD32(ctx->r8, -0X1);
    // 0x80143510: addiu       $s1, $sp, 0x11C
    ctx->r17 = ADD32(ctx->r29, 0X11C);
L_80143514:
    // 0x80143514: beq         $a3, $zero, L_80143578
    if (ctx->r7 == 0) {
        // 0x80143518: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_80143578;
    }
    // 0x80143518: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8014351C: addiu       $v0, $sp, 0x80
    ctx->r2 = ADD32(ctx->r29, 0X80);
L_80143520:
    // 0x80143520: lh          $a1, 0x2($v0)
    ctx->r5 = MEM_H(ctx->r2, 0X2);
    // 0x80143524: lh          $a2, 0x0($v0)
    ctx->r6 = MEM_H(ctx->r2, 0X0);
    // 0x80143528: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x8014352C: sll         $t9, $a1, 2
    ctx->r25 = S32(ctx->r5 << 2);
    // 0x80143530: sll         $t3, $a2, 2
    ctx->r11 = S32(ctx->r6 << 2);
    // 0x80143534: addu        $t4, $s1, $t3
    ctx->r12 = ADD32(ctx->r17, ctx->r11);
    // 0x80143538: addu        $t2, $s1, $t9
    ctx->r10 = ADD32(ctx->r17, ctx->r25);
    // 0x8014353C: lwc1        $f8, 0x0($t2)
    ctx->f8.u32l = MEM_W(ctx->r10, 0X0);
    // 0x80143540: lwc1        $f10, 0x0($t4)
    ctx->f10.u32l = MEM_W(ctx->r12, 0X0);
    // 0x80143544: c.lt.s      $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f8.fl < ctx->f10.fl;
    // 0x80143548: nop

    // 0x8014354C: bc1fl       L_8014356C
    if (!c1cs) {
        // 0x80143550: sltu        $at, $a0, $a3
        ctx->r1 = ctx->r4 < ctx->r7 ? 1 : 0;
            goto L_8014356C;
    }
    goto skip_3;
    // 0x80143550: sltu        $at, $a0, $a3
    ctx->r1 = ctx->r4 < ctx->r7 ? 1 : 0;
    skip_3:
    // 0x80143554: sh          $a1, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r5;
    // 0x80143558: sh          $a2, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r6;
    // 0x8014355C: lw          $t0, 0x4($s3)
    ctx->r8 = MEM_W(ctx->r19, 0X4);
    // 0x80143560: subu        $a3, $t0, $t1
    ctx->r7 = SUB32(ctx->r8, ctx->r9);
    // 0x80143564: addiu       $a3, $a3, -0x1
    ctx->r7 = ADD32(ctx->r7, -0X1);
    // 0x80143568: sltu        $at, $a0, $a3
    ctx->r1 = ctx->r4 < ctx->r7 ? 1 : 0;
L_8014356C:
    // 0x8014356C: bne         $at, $zero, L_80143520
    if (ctx->r1 != 0) {
        // 0x80143570: addiu       $v0, $v0, 0x2
        ctx->r2 = ADD32(ctx->r2, 0X2);
            goto L_80143520;
    }
    // 0x80143570: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80143574: addiu       $v0, $t0, -0x1
    ctx->r2 = ADD32(ctx->r8, -0X1);
L_80143578:
    // 0x80143578: addiu       $t1, $t1, 0x1
    ctx->r9 = ADD32(ctx->r9, 0X1);
    // 0x8014357C: sltu        $at, $t1, $v0
    ctx->r1 = ctx->r9 < ctx->r2 ? 1 : 0;
    // 0x80143580: bne         $at, $zero, L_80143514
    if (ctx->r1 != 0) {
        // 0x80143584: addiu       $a3, $a3, -0x1
        ctx->r7 = ADD32(ctx->r7, -0X1);
            goto L_80143514;
    }
    // 0x80143584: addiu       $a3, $a3, -0x1
    ctx->r7 = ADD32(ctx->r7, -0X1);
L_80143588:
    // 0x80143588: lh          $t5, 0x7C($sp)
    ctx->r13 = MEM_H(ctx->r29, 0X7C);
    // 0x8014358C: addiu       $s1, $sp, 0x11C
    ctx->r17 = ADD32(ctx->r29, 0X11C);
    // 0x80143590: addiu       $s2, $sp, 0x80
    ctx->r18 = ADD32(ctx->r29, 0X80);
    // 0x80143594: blez        $t5, L_801436F4
    if (SIGNED(ctx->r13) <= 0) {
        // 0x80143598: sll         $t7, $t5, 1
        ctx->r15 = S32(ctx->r13 << 1);
            goto L_801436F4;
    }
    // 0x80143598: sll         $t7, $t5, 1
    ctx->r15 = S32(ctx->r13 << 1);
    // 0x8014359C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801435A0: ldc1        $f22, -0x7F00($at)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r1, -0X7F00);
    // 0x801435A4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801435A8: lui         $fp, 0x8000
    ctx->r30 = S32(0X8000 << 16);
    // 0x801435AC: mtc1        $zero, $f24
    ctx->f24.u32l = 0;
    // 0x801435B0: addiu       $fp, $fp, 0x47C0
    ctx->r30 = ADD32(ctx->r30, 0X47C0);
    // 0x801435B4: ldc1        $f20, -0x7EF8($at)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r1, -0X7EF8);
    // 0x801435B8: addu        $s6, $t7, $s2
    ctx->r22 = ADD32(ctx->r15, ctx->r18);
    // 0x801435BC: addiu       $s7, $zero, 0x18
    ctx->r23 = ADD32(0, 0X18);
    // 0x801435C0: lh          $v0, 0x0($s2)
    ctx->r2 = MEM_H(ctx->r18, 0X0);
L_801435C4:
    // 0x801435C4: lw          $t9, 0x0($s3)
    ctx->r25 = MEM_W(ctx->r19, 0X0);
    // 0x801435C8: lwc1        $f18, 0x54($s4)
    ctx->f18.u32l = MEM_W(ctx->r20, 0X54);
    // 0x801435CC: multu       $v0, $s7
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r23)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801435D0: sll         $t3, $v0, 2
    ctx->r11 = S32(ctx->r2 << 2);
    // 0x801435D4: addu        $t4, $s1, $t3
    ctx->r12 = ADD32(ctx->r17, ctx->r11);
    // 0x801435D8: mflo        $t8
    ctx->r24 = lo;
    // 0x801435DC: addu        $s0, $t8, $t9
    ctx->r16 = ADD32(ctx->r24, ctx->r25);
    // 0x801435E0: lh          $t2, 0x2($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X2);
    // 0x801435E4: mtc1        $t2, $f16
    ctx->f16.u32l = ctx->r10;
    // 0x801435E8: nop

    // 0x801435EC: cvt.s.w     $f14, $f16
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 16);
    ctx->f14.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801435F0: sub.s       $f2, $f14, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f2.fl = ctx->f14.fl - ctx->f18.fl;
    // 0x801435F4: mul.s       $f4, $f2, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x801435F8: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801435FC: c.lt.d      $f20, $f6
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f20.d < ctx->f6.d;
    // 0x80143600: nop

    // 0x80143604: bc1tl       L_801436EC
    if (c1cs) {
        // 0x80143608: addiu       $s2, $s2, 0x2
        ctx->r18 = ADD32(ctx->r18, 0X2);
            goto L_801436EC;
    }
    goto skip_4;
    // 0x80143608: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    skip_4:
    // 0x8014360C: lwc1        $f8, 0x0($t4)
    ctx->f8.u32l = MEM_W(ctx->r12, 0X0);
    // 0x80143610: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80143614: c.lt.d      $f22, $f10
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f22.d < ctx->f10.d;
    // 0x80143618: nop

    // 0x8014361C: bc1tl       L_801436EC
    if (c1cs) {
        // 0x80143620: addiu       $s2, $s2, 0x2
        ctx->r18 = ADD32(ctx->r18, 0X2);
            goto L_801436EC;
    }
    goto skip_5;
    // 0x80143620: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    skip_5:
    // 0x80143624: lh          $t7, 0x4($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X4);
    // 0x80143628: lh          $t5, 0x0($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X0);
    // 0x8014362C: mtc1        $t7, $f18
    ctx->f18.u32l = ctx->r15;
    // 0x80143630: mtc1        $t5, $f16
    ctx->f16.u32l = ctx->r13;
    // 0x80143634: cvt.s.w     $f18, $f18
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 18);
    ctx->f18.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80143638: cvt.s.w     $f12, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    ctx->f12.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8014363C: mfc1        $a2, $f18
    ctx->r6 = (int32_t)ctx->f18.u32l;
    // 0x80143640: jal         0x801452E4
    // 0x80143644: nop

    func_801452E4(rdram, ctx);
        goto after_2;
    // 0x80143644: nop

    after_2:
    // 0x80143648: c.eq.s      $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    c1cs = ctx->f0.fl == ctx->f24.fl;
    // 0x8014364C: nop

    // 0x80143650: bc1tl       L_801436EC
    if (c1cs) {
        // 0x80143654: addiu       $s2, $s2, 0x2
        ctx->r18 = ADD32(ctx->r18, 0X2);
            goto L_801436EC;
    }
    goto skip_6;
    // 0x80143654: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
    skip_6:
    // 0x80143658: lhu         $a0, 0xE($s0)
    ctx->r4 = MEM_HU(ctx->r16, 0XE);
    // 0x8014365C: bne         $a0, $zero, L_801436A0
    if (ctx->r4 != 0) {
        // 0x80143660: nop
    
            goto L_801436A0;
    }
    // 0x80143660: nop

    // 0x80143664: lhu         $t6, 0xC($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0XC);
    // 0x80143668: lhu         $a1, 0x6($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X6);
    // 0x8014366C: lhu         $a2, 0x8($s0)
    ctx->r6 = MEM_HU(ctx->r16, 0X8);
    // 0x80143670: lhu         $a3, 0xA($s0)
    ctx->r7 = MEM_HU(ctx->r16, 0XA);
    // 0x80143674: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x80143678: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x8014367C: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x80143680: sw          $t8, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r24;
    // 0x80143684: lh          $t9, 0x2($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X2);
    // 0x80143688: sw          $t9, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r25;
    // 0x8014368C: lh          $t2, 0x4($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X4);
    // 0x80143690: jal         0x80144EE4
    // 0x80143694: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    func_80144EE4(rdram, ctx);
        goto after_3;
    // 0x80143694: sw          $t2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r10;
    after_3:
    // 0x80143698: b           L_801436EC
    // 0x8014369C: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
        goto L_801436EC;
    // 0x8014369C: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
L_801436A0:
    // 0x801436A0: jalr        $fp
    // 0x801436A4: nop

    LOOKUP_FUNC(ctx->r30)(rdram, ctx);
        goto after_4;
    // 0x801436A4: nop

    after_4:
    // 0x801436A8: lw          $t4, 0x10($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X10);
    // 0x801436AC: sll         $t3, $v0, 3
    ctx->r11 = S32(ctx->r2 << 3);
    // 0x801436B0: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x801436B4: addu        $v1, $t3, $t4
    ctx->r3 = ADD32(ctx->r11, ctx->r12);
    // 0x801436B8: lhu         $t5, 0x6($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X6);
    // 0x801436BC: lhu         $a1, 0x0($v1)
    ctx->r5 = MEM_HU(ctx->r3, 0X0);
    // 0x801436C0: lhu         $a2, 0x2($v1)
    ctx->r6 = MEM_HU(ctx->r3, 0X2);
    // 0x801436C4: lhu         $a3, 0x4($v1)
    ctx->r7 = MEM_HU(ctx->r3, 0X4);
    // 0x801436C8: sw          $t5, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r13;
    // 0x801436CC: lh          $t7, 0x0($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X0);
    // 0x801436D0: sw          $t7, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r15;
    // 0x801436D4: lh          $t6, 0x2($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X2);
    // 0x801436D8: sw          $t6, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r14;
    // 0x801436DC: lh          $t8, 0x4($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X4);
    // 0x801436E0: jal         0x80144EE4
    // 0x801436E4: sw          $t8, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r24;
    func_80144EE4(rdram, ctx);
        goto after_5;
    // 0x801436E4: sw          $t8, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r24;
    after_5:
    // 0x801436E8: addiu       $s2, $s2, 0x2
    ctx->r18 = ADD32(ctx->r18, 0X2);
L_801436EC:
    // 0x801436EC: bnel        $s2, $s6, L_801435C4
    if (ctx->r18 != ctx->r22) {
        // 0x801436F0: lh          $v0, 0x0($s2)
        ctx->r2 = MEM_H(ctx->r18, 0X0);
            goto L_801435C4;
    }
    goto skip_7;
    // 0x801436F0: lh          $v0, 0x0($s2)
    ctx->r2 = MEM_H(ctx->r18, 0X0);
    skip_7:
L_801436F4:
    // 0x801436F4: lw          $ra, 0x64($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X64);
L_801436F8:
    // 0x801436F8: ldc1        $f20, 0x28($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X28);
    // 0x801436FC: ldc1        $f22, 0x30($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X30);
    // 0x80143700: ldc1        $f24, 0x38($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X38);
    // 0x80143704: lw          $s0, 0x40($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X40);
    // 0x80143708: lw          $s1, 0x44($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X44);
    // 0x8014370C: lw          $s2, 0x48($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X48);
    // 0x80143710: lw          $s3, 0x4C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X4C);
    // 0x80143714: lw          $s4, 0x50($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X50);
    // 0x80143718: lw          $s5, 0x54($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X54);
    // 0x8014371C: lw          $s6, 0x58($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X58);
    // 0x80143720: lw          $s7, 0x5C($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X5C);
    // 0x80143724: lw          $fp, 0x60($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X60);
    // 0x80143728: jr          $ra
    // 0x8014372C: addiu       $sp, $sp, 0x238
    ctx->r29 = ADD32(ctx->r29, 0X238);
    return;
    // 0x8014372C: addiu       $sp, $sp, 0x238
    ctx->r29 = ADD32(ctx->r29, 0X238);
;}
RECOMP_FUNC void func_80143730(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143730: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143734: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143738: lw          $t9, 0x3C($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X3C);
    // 0x8014373C: jalr        $t9
    // 0x80143740: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80143740: nop

    after_0:
    // 0x80143744: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143748: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8014374C: jr          $ra
    // 0x80143750: nop

    return;
    // 0x80143750: nop

;}
RECOMP_FUNC void func_80143754(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143754: jr          $ra
    // 0x80143758: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x80143758: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_8014375C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8014375C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143760: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143764: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80143768: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8014376C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80143770: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80143774: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80143778: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8014377C: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80143780: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80143784: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80143788: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8014378C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80143790: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80143794: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80143798: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x8014379C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801437A0: lw          $t9, -0x2F3C($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2F3C);
    // 0x801437A4: jalr        $t9
    // 0x801437A8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801437A8: nop

    after_0:
    // 0x801437AC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801437B0: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801437B4: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801437B8: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801437BC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801437C0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801437C4: jr          $ra
    // 0x801437C8: nop

    return;
    // 0x801437C8: nop

;}
RECOMP_FUNC void func_801437CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801437CC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801437D0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801437D4: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x801437D8: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801437DC: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801437E0: lhu         $a1, 0x18($v0)
    ctx->r5 = MEM_HU(ctx->r2, 0X18);
    // 0x801437E4: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x801437E8: andi        $a1, $a1, 0xFFF
    ctx->r5 = ctx->r5 & 0XFFF;
    // 0x801437EC: slti        $at, $a1, 0x64
    ctx->r1 = SIGNED(ctx->r5) < 0X64 ? 1 : 0;
    // 0x801437F0: bne         $at, $zero, L_80143808
    if (ctx->r1 != 0) {
        // 0x801437F4: nop
    
            goto L_80143808;
    }
    // 0x801437F4: nop

    // 0x801437F8: lw          $v1, 0x68($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X68);
    // 0x801437FC: addiu       $v0, $a0, 0x34
    ctx->r2 = ADD32(ctx->r4, 0X34);
    // 0x80143800: b           L_80143830
    // 0x80143804: sw          $v1, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r3;
        goto L_80143830;
    // 0x80143804: sw          $v1, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r3;
L_80143808:
    // 0x80143808: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x8014380C: sll         $t9, $a1, 2
    ctx->r25 = S32(ctx->r5 << 2);
    // 0x80143810: subu        $t9, $t9, $a1
    ctx->r25 = SUB32(ctx->r25, ctx->r5);
    // 0x80143814: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80143818: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x8014381C: lw          $t8, -0x2E54($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X2E54);
    // 0x80143820: sll         $t9, $t9, 3
    ctx->r25 = S32(ctx->r25 << 3);
    // 0x80143824: addiu       $v0, $a2, 0x34
    ctx->r2 = ADD32(ctx->r6, 0X34);
    // 0x80143828: addu        $v1, $t8, $t9
    ctx->r3 = ADD32(ctx->r24, ctx->r25);
    // 0x8014382C: sw          $v1, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r3;
L_80143830:
    // 0x80143830: sw          $zero, 0x18($v0)
    MEM_W(0X18, ctx->r2) = 0;
    // 0x80143834: lw          $t0, 0x10($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X10);
    // 0x80143838: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x8014383C: sh          $t0, 0x20($v0)
    MEM_H(0X20, ctx->r2) = ctx->r8;
    // 0x80143840: lbu         $t1, 0xF($v1)
    ctx->r9 = MEM_BU(ctx->r3, 0XF);
    // 0x80143844: sh          $t1, 0x22($v0)
    MEM_H(0X22, ctx->r2) = ctx->r9;
    // 0x80143848: lbu         $t2, 0xE($v1)
    ctx->r10 = MEM_BU(ctx->r3, 0XE);
    // 0x8014384C: sh          $t2, 0x24($v0)
    MEM_H(0X24, ctx->r2) = ctx->r10;
    // 0x80143850: jal         0x80143234
    // 0x80143854: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_80143234(rdram, ctx);
        goto after_0;
    // 0x80143854: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x80143858: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x8014385C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80143860: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80143864: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80143868: jalr        $t9
    // 0x8014386C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8014386C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
    // 0x80143870: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143874: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143878: jr          $ra
    // 0x8014387C: nop

    return;
    // 0x8014387C: nop

;}
RECOMP_FUNC void func_80143880(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143880: addiu       $sp, $sp, -0x3A8
    ctx->r29 = ADD32(ctx->r29, -0X3A8);
    // 0x80143884: sw          $ra, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r31;
    // 0x80143888: sw          $fp, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r30;
    // 0x8014388C: sw          $s7, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r23;
    // 0x80143890: sw          $s6, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r22;
    // 0x80143894: sw          $s5, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r21;
    // 0x80143898: sw          $s4, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r20;
    // 0x8014389C: sw          $s3, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r19;
    // 0x801438A0: sw          $s2, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r18;
    // 0x801438A4: sw          $s1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r17;
    // 0x801438A8: sw          $s0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r16;
    // 0x801438AC: sdc1        $f30, 0x50($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X50, ctx->r29);
    // 0x801438B0: sdc1        $f28, 0x48($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X48, ctx->r29);
    // 0x801438B4: sdc1        $f26, 0x40($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X40, ctx->r29);
    // 0x801438B8: sdc1        $f24, 0x38($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X38, ctx->r29);
    // 0x801438BC: sdc1        $f22, 0x30($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X30, ctx->r29);
    // 0x801438C0: sdc1        $f20, 0x28($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X28, ctx->r29);
    // 0x801438C4: sw          $a0, 0x3A8($sp)
    MEM_W(0X3A8, ctx->r29) = ctx->r4;
    // 0x801438C8: lw          $s1, 0x70($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X70);
    // 0x801438CC: lui         $s5, 0x801D
    ctx->r21 = S32(0X801D << 16);
    // 0x801438D0: lw          $fp, 0x40($a0)
    ctx->r30 = MEM_W(ctx->r4, 0X40);
    // 0x801438D4: lhu         $t7, 0x18($s1)
    ctx->r15 = MEM_HU(ctx->r17, 0X18);
    // 0x801438D8: lw          $s5, -0x53E0($s5)
    ctx->r21 = MEM_W(ctx->r21, -0X53E0);
    // 0x801438DC: andi        $t8, $t7, 0x8000
    ctx->r24 = ctx->r15 & 0X8000;
    // 0x801438E0: bnel        $t8, $zero, L_80143918
    if (ctx->r24 != 0) {
        // 0x801438E4: lw          $v0, 0x14($fp)
        ctx->r2 = MEM_W(ctx->r30, 0X14);
            goto L_80143918;
    }
    goto skip_0;
    // 0x801438E4: lw          $v0, 0x14($fp)
    ctx->r2 = MEM_W(ctx->r30, 0X14);
    skip_0:
    // 0x801438E8: lw          $a1, 0x4($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X4);
    // 0x801438EC: lw          $a2, 0x8($s1)
    ctx->r6 = MEM_W(ctx->r17, 0X8);
    // 0x801438F0: jal         0x80145198
    // 0x801438F4: lw          $a3, 0xC($s1)
    ctx->r7 = MEM_W(ctx->r17, 0XC);
    func_80145198(rdram, ctx);
        goto after_0;
    // 0x801438F4: lw          $a3, 0xC($s1)
    ctx->r7 = MEM_W(ctx->r17, 0XC);
    after_0:
    // 0x801438F8: beq         $v0, $zero, L_80143914
    if (ctx->r2 == 0) {
        // 0x801438FC: lw          $a0, 0x3A8($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X3A8);
            goto L_80143914;
    }
    // 0x801438FC: lw          $a0, 0x3A8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3A8);
    // 0x80143900: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x80143904: jalr        $t9
    // 0x80143908: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80143908: nop

    after_1:
    // 0x8014390C: b           L_80143D2C
    // 0x80143910: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
        goto L_80143D2C;
    // 0x80143910: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
L_80143914:
    // 0x80143914: lw          $v0, 0x14($fp)
    ctx->r2 = MEM_W(ctx->r30, 0X14);
L_80143918:
    // 0x80143918: lui         $at, 0x3FFF
    ctx->r1 = S32(0X3FFF << 16);
    // 0x8014391C: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80143920: sll         $t0, $v0, 0
    ctx->r8 = S32(ctx->r2 << 0);
    // 0x80143924: bgez        $t0, L_8014395C
    if (SIGNED(ctx->r8) >= 0) {
        // 0x80143928: and         $v1, $v0, $at
        ctx->r3 = ctx->r2 & ctx->r1;
            goto L_8014395C;
    }
    // 0x80143928: and         $v1, $v0, $at
    ctx->r3 = ctx->r2 & ctx->r1;
    // 0x8014392C: beq         $v1, $zero, L_80143950
    if (ctx->r3 == 0) {
        // 0x80143930: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_80143950;
    }
    // 0x80143930: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80143934: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80143938: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x8014393C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80143940: jalr        $t9
    // 0x80143944: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80143944: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_2:
    // 0x80143948: b           L_80143954
    // 0x8014394C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_80143954;
    // 0x8014394C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80143950:
    // 0x80143950: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80143954:
    // 0x80143954: beql        $v1, $zero, L_80143D2C
    if (ctx->r3 == 0) {
        // 0x80143958: lw          $ra, 0x7C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X7C);
            goto L_80143D2C;
    }
    goto skip_1;
    // 0x80143958: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
    skip_1:
L_8014395C:
    // 0x8014395C: lw          $v0, 0x14($fp)
    ctx->r2 = MEM_W(ctx->r30, 0X14);
    // 0x80143960: lui         $at, 0x3FFF
    ctx->r1 = S32(0X3FFF << 16);
    // 0x80143964: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80143968: sll         $t1, $v0, 1
    ctx->r9 = S32(ctx->r2 << 1);
    // 0x8014396C: bgez        $t1, L_801439A0
    if (SIGNED(ctx->r9) >= 0) {
        // 0x80143970: and         $v1, $v0, $at
        ctx->r3 = ctx->r2 & ctx->r1;
            goto L_801439A0;
    }
    // 0x80143970: and         $v1, $v0, $at
    ctx->r3 = ctx->r2 & ctx->r1;
    // 0x80143974: beq         $v1, $zero, L_80143998
    if (ctx->r3 == 0) {
        // 0x80143978: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_80143998;
    }
    // 0x80143978: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8014397C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80143980: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x80143984: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x80143988: jalr        $t9
    // 0x8014398C: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8014398C: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    after_3:
    // 0x80143990: b           L_8014399C
    // 0x80143994: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_8014399C;
    // 0x80143994: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80143998:
    // 0x80143998: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8014399C:
    // 0x8014399C: bne         $v1, $zero, L_80143D28
    if (ctx->r3 != 0) {
        // 0x801439A0: lui         $t2, 0x801D
        ctx->r10 = S32(0X801D << 16);
            goto L_80143D28;
    }
L_801439A0:
    // 0x801439A0: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801439A4: lw          $t2, -0x5170($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X5170);
    // 0x801439A8: andi        $t3, $t2, 0x1
    ctx->r11 = ctx->r10 & 0X1;
    // 0x801439AC: bnel        $t3, $zero, L_80143D2C
    if (ctx->r11 != 0) {
        // 0x801439B0: lw          $ra, 0x7C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X7C);
            goto L_80143D2C;
    }
    goto skip_2;
    // 0x801439B0: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
    skip_2:
    // 0x801439B4: jal         0x80145420
    // 0x801439B8: lw          $a0, 0x3A8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3A8);
    func_80145420(rdram, ctx);
        goto after_4;
    // 0x801439B8: lw          $a0, 0x3A8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3A8);
    after_4:
    // 0x801439BC: lw          $s0, 0x3A8($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X3A8);
    // 0x801439C0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x801439C4: lh          $t4, 0x56($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X56);
    // 0x801439C8: addiu       $s0, $s0, 0x34
    ctx->r16 = ADD32(ctx->r16, 0X34);
    // 0x801439CC: slt         $at, $v0, $t4
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r12) ? 1 : 0;
    // 0x801439D0: beql        $at, $zero, L_80143D2C
    if (ctx->r1 == 0) {
        // 0x801439D4: lw          $ra, 0x7C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X7C);
            goto L_80143D2C;
    }
    goto skip_3;
    // 0x801439D4: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
    skip_3:
    // 0x801439D8: lhu         $t5, 0x0($s1)
    ctx->r13 = MEM_HU(ctx->r17, 0X0);
    // 0x801439DC: andi        $t7, $t5, 0x4
    ctx->r15 = ctx->r13 & 0X4;
    // 0x801439E0: beq         $t7, $zero, L_80143A00
    if (ctx->r15 == 0) {
        // 0x801439E4: nop
    
            goto L_80143A00;
    }
    // 0x801439E4: nop

    // 0x801439E8: jal         0x8014555C
    // 0x801439EC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_8014555C(rdram, ctx);
        goto after_5;
    // 0x801439EC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_5:
    // 0x801439F0: bnel        $v0, $zero, L_80143A14
    if (ctx->r2 != 0) {
        // 0x801439F4: lw          $t8, 0x18($s0)
        ctx->r24 = MEM_W(ctx->r16, 0X18);
            goto L_80143A14;
    }
    goto skip_4;
    // 0x801439F4: lw          $t8, 0x18($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X18);
    skip_4:
    // 0x801439F8: b           L_80143D2C
    // 0x801439FC: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
        goto L_80143D2C;
    // 0x801439FC: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
L_80143A00:
    // 0x80143A00: jal         0x801454DC
    // 0x80143A04: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_801454DC(rdram, ctx);
        goto after_6;
    // 0x80143A04: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_6:
    // 0x80143A08: beql        $v0, $zero, L_80143D2C
    if (ctx->r2 == 0) {
        // 0x80143A0C: lw          $ra, 0x7C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X7C);
            goto L_80143D2C;
    }
    goto skip_5;
    // 0x80143A0C: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
    skip_5:
    // 0x80143A10: lw          $t8, 0x18($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X18);
L_80143A14:
    // 0x80143A14: lh          $t1, 0x20($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X20);
    // 0x80143A18: lui         $t9, 0x8010
    ctx->r25 = S32(0X8010 << 16);
    // 0x80143A1C: addiu       $t6, $t8, 0x1
    ctx->r14 = ADD32(ctx->r24, 0X1);
    // 0x80143A20: slt         $at, $t6, $t1
    ctx->r1 = SIGNED(ctx->r14) < SIGNED(ctx->r9) ? 1 : 0;
    // 0x80143A24: bne         $at, $zero, L_80143D28
    if (ctx->r1 != 0) {
        // 0x80143A28: sw          $t6, 0x18($s0)
        MEM_W(0X18, ctx->r16) = ctx->r14;
            goto L_80143D28;
    }
    // 0x80143A28: sw          $t6, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r14;
    // 0x80143A2C: sw          $zero, 0x18($s0)
    MEM_W(0X18, ctx->r16) = 0;
    // 0x80143A30: lw          $t9, -0x6EE0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X6EE0);
    // 0x80143A34: lh          $t2, 0x24($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X24);
    // 0x80143A38: slt         $at, $t9, $t2
    ctx->r1 = SIGNED(ctx->r25) < SIGNED(ctx->r10) ? 1 : 0;
    // 0x80143A3C: beql        $at, $zero, L_80143D2C
    if (ctx->r1 == 0) {
        // 0x80143A40: lw          $ra, 0x7C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X7C);
            goto L_80143D2C;
    }
    goto skip_6;
    // 0x80143A40: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
    skip_6:
    // 0x80143A44: jal         0x801454CC
    // 0x80143A48: nop

    func_801454CC(rdram, ctx);
        goto after_7;
    // 0x80143A48: nop

    after_7:
    // 0x80143A4C: lwc1        $f4, 0x4($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80143A50: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x80143A54: lwc1        $f8, 0x50($s5)
    ctx->f8.u32l = MEM_W(ctx->r21, 0X50);
    // 0x80143A58: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80143A5C: lwc1        $f4, 0xC($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0XC);
    // 0x80143A60: lwc1        $f18, 0x54($s5)
    ctx->f18.u32l = MEM_W(ctx->r21, 0X54);
    // 0x80143A64: mul.s       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80143A68: lh          $v0, 0x8($fp)
    ctx->r2 = MEM_H(ctx->r30, 0X8);
    // 0x80143A6C: lui         $s4, 0x8000
    ctx->r20 = S32(0X8000 << 16);
    // 0x80143A70: addiu       $s4, $s4, 0x47C0
    ctx->r20 = ADD32(ctx->r20, 0X47C0);
    // 0x80143A74: multu       $v0, $v0
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r2)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80143A78: sub.s       $f26, $f6, $f8
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f26.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80143A7C: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x80143A80: lwc1        $f8, 0x58($s5)
    ctx->f8.u32l = MEM_W(ctx->r21, 0X58);
    // 0x80143A84: sub.s       $f22, $f16, $f18
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f22.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x80143A88: mul.s       $f18, $f26, $f26
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f26.fl); NAN_CHECK(ctx->f26.fl); 
    ctx->f18.fl = MUL_S(ctx->f26.fl, ctx->f26.fl);
    // 0x80143A8C: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
    // 0x80143A90: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x80143A94: mul.s       $f4, $f22, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = MUL_S(ctx->f22.fl, ctx->f22.fl);
    // 0x80143A98: addiu       $s7, $sp, 0x30C
    ctx->r23 = ADD32(ctx->r29, 0X30C);
    // 0x80143A9C: sub.s       $f24, $f6, $f8
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f24.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80143AA0: mflo        $t3
    ctx->r11 = lo;
    // 0x80143AA4: mtc1        $t3, $f10
    ctx->f10.u32l = ctx->r11;
    // 0x80143AA8: addiu       $s6, $sp, 0x2F4
    ctx->r22 = ADD32(ctx->r29, 0X2F4);
    // 0x80143AAC: mul.s       $f8, $f24, $f24
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f8.fl = MUL_S(ctx->f24.fl, ctx->f24.fl);
    // 0x80143AB0: add.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f4.fl;
    // 0x80143AB4: lui         $s2, 0x2000
    ctx->r18 = S32(0X2000 << 16);
    // 0x80143AB8: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80143ABC: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x80143AC0: c.lt.s      $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f16.fl < ctx->f10.fl;
    // 0x80143AC4: nop

    // 0x80143AC8: bc1tl       L_80143D2C
    if (c1cs) {
        // 0x80143ACC: lw          $ra, 0x7C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X7C);
            goto L_80143D2C;
    }
    goto skip_7;
    // 0x80143ACC: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
    skip_7:
    // 0x80143AD0: mtc1        $at, $f31
    ctx->f_odd[(31 - 1) * 2] = ctx->r1;
    // 0x80143AD4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80143AD8: mtc1        $zero, $f30
    ctx->f30.u32l = 0;
    // 0x80143ADC: ldc1        $f28, -0x7EF0($at)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r1, -0X7EF0);
L_80143AE0:
    // 0x80143AE0: jalr        $s4
    // 0x80143AE4: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_8;
    // 0x80143AE4: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    after_8:
    // 0x80143AE8: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x80143AEC: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x80143AF0: bgez        $v0, L_80143B04
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80143AF4: cvt.s.w     $f4, $f18
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
            goto L_80143B04;
    }
    // 0x80143AF4: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x80143AF8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80143AFC: nop

    // 0x80143B00: add.s       $f4, $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f4.fl + ctx->f6.fl;
L_80143B04:
    // 0x80143B04: lh          $t4, 0xC($fp)
    ctx->r12 = MEM_H(ctx->r30, 0XC);
    // 0x80143B08: cvt.d.s     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f8.d = CVT_D_S(ctx->f4.fl);
    // 0x80143B0C: mtc1        $t4, $f10
    ctx->f10.u32l = ctx->r12;
    // 0x80143B10: mul.d       $f16, $f8, $f28
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f28.d); 
    ctx->f16.d = MUL_D(ctx->f8.d, ctx->f28.d);
    // 0x80143B14: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80143B18: cvt.d.s     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f6.d = CVT_D_S(ctx->f18.fl);
    // 0x80143B1C: mul.d       $f4, $f16, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f6.d);
    // 0x80143B20: trunc.w.d   $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    ctx->f8.u32l = TRUNC_W_D(ctx->f4.d);
    // 0x80143B24: mfc1        $a0, $f8
    ctx->r4 = (int32_t)ctx->f8.u32l;
    // 0x80143B28: jal         0x80145450
    // 0x80143B2C: nop

    func_80145450(rdram, ctx);
        goto after_9;
    // 0x80143B2C: nop

    after_9:
    // 0x80143B30: lh          $t7, 0x5E($s5)
    ctx->r15 = MEM_H(ctx->r21, 0X5E);
    // 0x80143B34: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x80143B38: addu        $s1, $t7, $v0
    ctx->r17 = ADD32(ctx->r15, ctx->r2);
    // 0x80143B3C: jalr        $s4
    // 0x80143B40: andi        $s1, $s1, 0xFFFF
    ctx->r17 = ctx->r17 & 0XFFFF;
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_10;
    // 0x80143B40: andi        $s1, $s1, 0xFFFF
    ctx->r17 = ctx->r17 & 0XFFFF;
    after_10:
    // 0x80143B44: addiu       $t8, $v0, -0x2710
    ctx->r24 = ADD32(ctx->r2, -0X2710);
    // 0x80143B48: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x80143B4C: lh          $t6, 0xA($fp)
    ctx->r14 = MEM_H(ctx->r30, 0XA);
    // 0x80143B50: lui         $at, 0x4034
    ctx->r1 = S32(0X4034 << 16);
    // 0x80143B54: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80143B58: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x80143B5C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80143B60: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x80143B64: andi        $s0, $s1, 0xFFFF
    ctx->r16 = ctx->r17 & 0XFFFF;
    // 0x80143B68: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80143B6C: cvt.d.s     $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f16.d = CVT_D_S(ctx->f18.fl);
    // 0x80143B70: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80143B74: mul.d       $f6, $f16, $f28
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f28.d); 
    ctx->f6.d = MUL_D(ctx->f16.d, ctx->f28.d);
    // 0x80143B78: cvt.d.s     $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f16.d = CVT_D_S(ctx->f18.fl);
    // 0x80143B7C: mul.d       $f8, $f6, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f4.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f4.d);
    // 0x80143B80: add.d       $f6, $f8, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f16.d); 
    ctx->f6.d = ctx->f8.d + ctx->f16.d;
    // 0x80143B84: jal         0x800A3A50
    // 0x80143B88: cvt.s.d     $f24, $f6
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f24.fl = CVT_S_D(ctx->f6.d);
    func_800A3A50(rdram, ctx);
        goto after_11;
    // 0x80143B88: cvt.s.d     $f24, $f6
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f24.fl = CVT_S_D(ctx->f6.d);
    after_11:
    // 0x80143B8C: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80143B90: cvt.d.s     $f20, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f24.fl); 
    ctx->f20.d = CVT_D_S(ctx->f24.fl);
    // 0x80143B94: lwc1        $f6, 0x50($s5)
    ctx->f6.u32l = MEM_W(ctx->r21, 0X50);
    // 0x80143B98: cvt.s.w     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80143B9C: andi        $a0, $s0, 0xFFFF
    ctx->r4 = ctx->r16 & 0XFFFF;
    // 0x80143BA0: cvt.d.s     $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f4.d = CVT_D_S(ctx->f6.fl);
    // 0x80143BA4: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x80143BA8: div.d       $f8, $f18, $f30
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f30.d); 
    ctx->f8.d = DIV_D(ctx->f18.d, ctx->f30.d);
    // 0x80143BAC: mul.d       $f16, $f8, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f20.d); 
    ctx->f16.d = MUL_D(ctx->f8.d, ctx->f20.d);
    // 0x80143BB0: add.d       $f10, $f16, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f4.d); 
    ctx->f10.d = ctx->f16.d + ctx->f4.d;
    // 0x80143BB4: cvt.s.d     $f26, $f10
    CHECK_FR(ctx, 26);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f26.fl = CVT_S_D(ctx->f10.d);
    // 0x80143BB8: swc1        $f26, 0x344($sp)
    MEM_W(0X344, ctx->r29) = ctx->f26.u32l;
    // 0x80143BBC: lwc1        $f18, 0x54($s5)
    ctx->f18.u32l = MEM_W(ctx->r21, 0X54);
    // 0x80143BC0: add.s       $f22, $f18, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = ctx->f18.fl + ctx->f24.fl;
    // 0x80143BC4: jal         0x80097330
    // 0x80143BC8: swc1        $f22, 0x348($sp)
    MEM_W(0X348, ctx->r29) = ctx->f22.u32l;
    func_80097330(rdram, ctx);
        goto after_12;
    // 0x80143BC8: swc1        $f22, 0x348($sp)
    MEM_W(0X348, ctx->r29) = ctx->f22.u32l;
    after_12:
    // 0x80143BCC: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    // 0x80143BD0: lwc1        $f8, 0x58($s5)
    ctx->f8.u32l = MEM_W(ctx->r21, 0X58);
    // 0x80143BD4: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x80143BD8: cvt.s.w     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    ctx->f4.fl = CVT_S_W(ctx->f16.u32l);
    // 0x80143BDC: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80143BE0: addiu       $a0, $sp, 0x344
    ctx->r4 = ADD32(ctx->r29, 0X344);
    // 0x80143BE4: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x80143BE8: cvt.d.s     $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f6.d = CVT_D_S(ctx->f8.fl);
    // 0x80143BEC: cvt.d.s     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f10.d = CVT_D_S(ctx->f4.fl);
    // 0x80143BF0: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80143BF4: div.d       $f18, $f10, $f30
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f30.d); 
    ctx->f18.d = DIV_D(ctx->f10.d, ctx->f30.d);
    // 0x80143BF8: mul.d       $f8, $f18, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f20.d); 
    ctx->f8.d = MUL_D(ctx->f18.d, ctx->f20.d);
    // 0x80143BFC: sub.d       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f16.d = ctx->f6.d - ctx->f8.d;
    // 0x80143C00: cvt.s.d     $f24, $f16
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f24.fl = CVT_S_D(ctx->f16.d);
    // 0x80143C04: jal         0x80045FA0
    // 0x80143C08: swc1        $f24, 0x34C($sp)
    MEM_W(0X34C, ctx->r29) = ctx->f24.u32l;
    func_80045FA0(rdram, ctx);
        goto after_13;
    // 0x80143C08: swc1        $f24, 0x34C($sp)
    MEM_W(0X34C, ctx->r29) = ctx->f24.u32l;
    after_13:
    // 0x80143C0C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80143C10: beq         $v0, $at, L_80143CD4
    if (ctx->r2 == ctx->r1) {
        // 0x80143C14: addiu       $a3, $sp, 0x32E
        ctx->r7 = ADD32(ctx->r29, 0X32E);
            goto L_80143CD4;
    }
    // 0x80143C14: addiu       $a3, $sp, 0x32E
    ctx->r7 = ADD32(ctx->r29, 0X32E);
    // 0x80143C18: mfc1        $a2, $f24
    ctx->r6 = (int32_t)ctx->f24.u32l;
    // 0x80143C1C: mov.s       $f12, $f26
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 26);
    ctx->f12.fl = ctx->f26.fl;
    // 0x80143C20: jal         0x801451FC
    // 0x80143C24: mov.s       $f14, $f22
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 22);
    ctx->f14.fl = ctx->f22.fl;
    func_801451FC(rdram, ctx);
        goto after_14;
    // 0x80143C24: mov.s       $f14, $f22
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 22);
    ctx->f14.fl = ctx->f22.fl;
    after_14:
    // 0x80143C28: beql        $v0, $zero, L_80143CD8
    if (ctx->r2 == 0) {
        // 0x80143C2C: addiu       $s3, $s3, 0x1
        ctx->r19 = ADD32(ctx->r19, 0X1);
            goto L_80143CD8;
    }
    goto skip_8;
    // 0x80143C2C: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    skip_8:
    // 0x80143C30: lh          $t0, 0x32E($sp)
    ctx->r8 = MEM_H(ctx->r29, 0X32E);
    // 0x80143C34: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x80143C38: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    // 0x80143C3C: mtc1        $t0, $f4
    ctx->f4.u32l = ctx->r8;
    // 0x80143C40: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80143C44: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80143C48: cvt.d.w     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    ctx->f10.d = CVT_D_W(ctx->f4.u32l);
    // 0x80143C4C: lw          $v0, -0x53E0($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53E0);
    // 0x80143C50: swc1        $f26, 0x300($sp)
    MEM_W(0X300, ctx->r29) = ctx->f26.u32l;
    // 0x80143C54: swc1        $f24, 0x308($sp)
    MEM_W(0X308, ctx->r29) = ctx->f24.u32l;
    // 0x80143C58: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80143C5C: add.d       $f18, $f10, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = ctx->f10.d + ctx->f0.d;
    // 0x80143C60: addiu       $a1, $sp, 0x300
    ctx->r5 = ADD32(ctx->r29, 0X300);
    // 0x80143C64: or          $a2, $s7, $zero
    ctx->r6 = ctx->r23 | 0;
    // 0x80143C68: cvt.s.d     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f6.fl = CVT_S_D(ctx->f18.d);
    // 0x80143C6C: swc1        $f6, 0x304($sp)
    MEM_W(0X304, ctx->r29) = ctx->f6.u32l;
    // 0x80143C70: lwc1        $f8, 0x50($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80143C74: swc1        $f8, 0x30C($sp)
    MEM_W(0X30C, ctx->r29) = ctx->f8.u32l;
    // 0x80143C78: lwc1        $f16, 0x54($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X54);
    // 0x80143C7C: cvt.d.s     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f4.d = CVT_D_S(ctx->f16.fl);
    // 0x80143C80: add.d       $f10, $f4, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f10.d = ctx->f4.d + ctx->f0.d;
    // 0x80143C84: cvt.s.d     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f18.fl = CVT_S_D(ctx->f10.d);
    // 0x80143C88: swc1        $f18, 0x310($sp)
    MEM_W(0X310, ctx->r29) = ctx->f18.u32l;
    // 0x80143C8C: lwc1        $f6, 0x58($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X58);
    // 0x80143C90: jal         0x80013CC4
    // 0x80143C94: swc1        $f6, 0x314($sp)
    MEM_W(0X314, ctx->r29) = ctx->f6.u32l;
    func_80013CC4(rdram, ctx);
        goto after_15;
    // 0x80143C94: swc1        $f6, 0x314($sp)
    MEM_W(0X314, ctx->r29) = ctx->f6.u32l;
    after_15:
    // 0x80143C98: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x80143C9C: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    // 0x80143CA0: addiu       $a2, $sp, 0x9C
    ctx->r6 = ADD32(ctx->r29, 0X9C);
    // 0x80143CA4: jal         0x8003E2F0
    // 0x80143CA8: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
    func_8003E2F0(rdram, ctx);
        goto after_16;
    // 0x80143CA8: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
    after_16:
    // 0x80143CAC: lw          $t1, 0x9C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X9C);
    // 0x80143CB0: lw          $t2, 0xA0($sp)
    ctx->r10 = MEM_W(ctx->r29, 0XA0);
    // 0x80143CB4: and         $t9, $t1, $s2
    ctx->r25 = ctx->r9 & ctx->r18;
    // 0x80143CB8: bne         $t9, $zero, L_80143CD4
    if (ctx->r25 != 0) {
        // 0x80143CBC: and         $t3, $t2, $s2
        ctx->r11 = ctx->r10 & ctx->r18;
            goto L_80143CD4;
    }
    // 0x80143CBC: and         $t3, $t2, $s2
    ctx->r11 = ctx->r10 & ctx->r18;
    // 0x80143CC0: bne         $t3, $zero, L_80143CD4
    if (ctx->r11 != 0) {
        // 0x80143CC4: lw          $t4, 0xA4($sp)
        ctx->r12 = MEM_W(ctx->r29, 0XA4);
            goto L_80143CD4;
    }
    // 0x80143CC4: lw          $t4, 0xA4($sp)
    ctx->r12 = MEM_W(ctx->r29, 0XA4);
    // 0x80143CC8: and         $t5, $t4, $s2
    ctx->r13 = ctx->r12 & ctx->r18;
    // 0x80143CCC: beql        $t5, $zero, L_80143CE8
    if (ctx->r13 == 0) {
        // 0x80143CD0: slti        $at, $s3, 0x10
        ctx->r1 = SIGNED(ctx->r19) < 0X10 ? 1 : 0;
            goto L_80143CE8;
    }
    goto skip_9;
    // 0x80143CD0: slti        $at, $s3, 0x10
    ctx->r1 = SIGNED(ctx->r19) < 0X10 ? 1 : 0;
    skip_9:
L_80143CD4:
    // 0x80143CD4: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
L_80143CD8:
    // 0x80143CD8: addiu       $at, $zero, 0x10
    ctx->r1 = ADD32(0, 0X10);
    // 0x80143CDC: bne         $s3, $at, L_80143AE0
    if (ctx->r19 != ctx->r1) {
        // 0x80143CE0: nop
    
            goto L_80143AE0;
    }
    // 0x80143CE0: nop

    // 0x80143CE4: slti        $at, $s3, 0x10
    ctx->r1 = SIGNED(ctx->r19) < 0X10 ? 1 : 0;
L_80143CE8:
    // 0x80143CE8: beq         $at, $zero, L_80143D28
    if (ctx->r1 == 0) {
        // 0x80143CEC: lw          $a0, 0x3A8($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X3A8);
            goto L_80143D28;
    }
    // 0x80143CEC: lw          $a0, 0x3A8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X3A8);
    // 0x80143CF0: trunc.w.s   $f8, $f26
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 26);
    ctx->f8.u32l = TRUNC_W_S(ctx->f26.fl);
    // 0x80143CF4: lhu         $t7, 0x6($fp)
    ctx->r15 = MEM_HU(ctx->r30, 0X6);
    // 0x80143CF8: lhu         $a1, 0x0($fp)
    ctx->r5 = MEM_HU(ctx->r30, 0X0);
    // 0x80143CFC: trunc.w.s   $f16, $f24
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 24);
    ctx->f16.u32l = TRUNC_W_S(ctx->f24.fl);
    // 0x80143D00: lhu         $a2, 0x2($fp)
    ctx->r6 = MEM_HU(ctx->r30, 0X2);
    // 0x80143D04: lhu         $a3, 0x4($fp)
    ctx->r7 = MEM_HU(ctx->r30, 0X4);
    // 0x80143D08: mfc1        $t6, $f8
    ctx->r14 = (int32_t)ctx->f8.u32l;
    // 0x80143D0C: mfc1        $t9, $f16
    ctx->r25 = (int32_t)ctx->f16.u32l;
    // 0x80143D10: lh          $t0, 0x32E($sp)
    ctx->r8 = MEM_H(ctx->r29, 0X32E);
    // 0x80143D14: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    // 0x80143D18: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x80143D1C: sw          $t0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r8;
    // 0x80143D20: jal         0x80144EE4
    // 0x80143D24: sw          $t9, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r25;
    func_80144EE4(rdram, ctx);
        goto after_17;
    // 0x80143D24: sw          $t9, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r25;
    after_17:
L_80143D28:
    // 0x80143D28: lw          $ra, 0x7C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X7C);
L_80143D2C:
    // 0x80143D2C: ldc1        $f20, 0x28($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X28);
    // 0x80143D30: ldc1        $f22, 0x30($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X30);
    // 0x80143D34: ldc1        $f24, 0x38($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X38);
    // 0x80143D38: ldc1        $f26, 0x40($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X40);
    // 0x80143D3C: ldc1        $f28, 0x48($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X48);
    // 0x80143D40: ldc1        $f30, 0x50($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X50);
    // 0x80143D44: lw          $s0, 0x58($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X58);
    // 0x80143D48: lw          $s1, 0x5C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X5C);
    // 0x80143D4C: lw          $s2, 0x60($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X60);
    // 0x80143D50: lw          $s3, 0x64($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X64);
    // 0x80143D54: lw          $s4, 0x68($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X68);
    // 0x80143D58: lw          $s5, 0x6C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X6C);
    // 0x80143D5C: lw          $s6, 0x70($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X70);
    // 0x80143D60: lw          $s7, 0x74($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X74);
    // 0x80143D64: lw          $fp, 0x78($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X78);
    // 0x80143D68: jr          $ra
    // 0x80143D6C: addiu       $sp, $sp, 0x3A8
    ctx->r29 = ADD32(ctx->r29, 0X3A8);
    return;
    // 0x80143D6C: addiu       $sp, $sp, 0x3A8
    ctx->r29 = ADD32(ctx->r29, 0X3A8);
;}
RECOMP_FUNC void func_80143D70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80143D70: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80143D74: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80143D78: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80143D7C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80143D80: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80143D84: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80143D88: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80143D8C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80143D90: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80143D94: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80143D98: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80143D9C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80143DA0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80143DA4: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80143DA8: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80143DAC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80143DB0: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80143DB4: lw          $t9, -0x2F34($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X2F34);
    // 0x80143DB8: jalr        $t9
    // 0x80143DBC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80143DBC: nop

    after_0:
    // 0x80143DC0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80143DC4: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80143DC8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80143DCC: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80143DD0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80143DD4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80143DD8: jr          $ra
    // 0x80143DDC: nop

    return;
    // 0x80143DDC: nop

;}
