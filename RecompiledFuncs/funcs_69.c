#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

RECOMP_FUNC void overlay_system_func_801D0CE8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D0CE8: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801D0CEC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801D0CF0: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x801D0CF4: lw          $v0, 0x64($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X64);
    // 0x801D0CF8: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x801D0CFC: lh          $t8, -0x7D14($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X7D14);
    // 0x801D0D00: lw          $t7, 0x10($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X10);
    // 0x801D0D04: lui         $at, 0xC348
    ctx->r1 = S32(0XC348 << 16);
    // 0x801D0D08: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x801D0D0C: addu        $t0, $t7, $t9
    ctx->r8 = ADD32(ctx->r15, ctx->r25);
    // 0x801D0D10: lw          $a2, 0x4($t0)
    ctx->r6 = MEM_W(ctx->r8, 0X4);
    // 0x801D0D14: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801D0D18: lui         $v1, 0x8006
    ctx->r3 = S32(0X8006 << 16);
    // 0x801D0D1C: addiu       $v1, $v1, -0x7F60
    ctx->r3 = ADD32(ctx->r3, -0X7F60);
    // 0x801D0D20: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    // 0x801D0D24: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D0D28: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D0D2C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x801D0D30: sw          $a2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r6;
    // 0x801D0D34: jalr        $v1
    // 0x801D0D38: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_0;
    // 0x801D0D38: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x801D0D3C: lw          $t9, 0x20($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X20);
    // 0x801D0D40: lui         $at, 0xC35C
    ctx->r1 = S32(0XC35C << 16);
    // 0x801D0D44: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D0D48: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D0D4C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D0D50: lw          $a2, 0x2C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X2C);
    // 0x801D0D54: addiu       $a3, $zero, 0x1
    ctx->r7 = ADD32(0, 0X1);
    // 0x801D0D58: jalr        $t9
    // 0x801D0D5C: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801D0D5C: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_1:
    // 0x801D0D60: lw          $t9, 0x20($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X20);
    // 0x801D0D64: lui         $at, 0xC35C
    ctx->r1 = S32(0XC35C << 16);
    // 0x801D0D68: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801D0D6C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D0D70: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D0D74: lw          $a2, 0x2C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X2C);
    // 0x801D0D78: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
    // 0x801D0D7C: jalr        $t9
    // 0x801D0D80: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801D0D80: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    after_2:
    // 0x801D0D84: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801D0D88: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x801D0D8C: jr          $ra
    // 0x801D0D90: nop

    return;
    // 0x801D0D90: nop

;}
RECOMP_FUNC void overlay_system_func_801D0DA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D0DA0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801D0DA4: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801D0DA8: lh          $v0, 0x2878($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X2878);
    // 0x801D0DAC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801D0DB0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801D0DB4: bne         $a1, $v0, L_801D0DC8
    if (ctx->r5 != ctx->r2) {
        // 0x801D0DB8: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_801D0DC8;
    }
    // 0x801D0DB8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801D0DBC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801D0DC0: b           L_801D0E30
    // 0x801D0DC4: sb          $zero, 0x292C($at)
    MEM_B(0X292C, ctx->r1) = 0;
        goto L_801D0E30;
    // 0x801D0DC4: sb          $zero, 0x292C($at)
    MEM_B(0X292C, ctx->r1) = 0;
L_801D0DC8:
    // 0x801D0DC8: bne         $v0, $zero, L_801D0E30
    if (ctx->r2 != 0) {
        // 0x801D0DCC: lui         $v0, 0x801D
        ctx->r2 = S32(0X801D << 16);
            goto L_801D0E30;
    }
    // 0x801D0DCC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801D0DD0: lb          $v0, 0x292C($v0)
    ctx->r2 = MEM_B(ctx->r2, 0X292C);
    // 0x801D0DD4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801D0DD8: bne         $v0, $zero, L_801D0E04
    if (ctx->r2 != 0) {
        // 0x801D0DDC: nop

            goto L_801D0E04;
    }
    // 0x801D0DDC: nop

    // 0x801D0DE0: lw          $t6, 0x2960($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2960);
    // 0x801D0DE4: lh          $t8, 0x0($t6)
    ctx->r24 = MEM_H(ctx->r14, 0X0);
    // 0x801D0DE8: bgez        $t8, L_801D0DFC
    if (SIGNED(ctx->r24) >= 0) {
        // 0x801D0DEC: nop

            goto L_801D0DFC;
    }
    // 0x801D0DEC: nop

    // 0x801D0DF0: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x801D0DF4: sll         $v0, $v0, 24
    ctx->r2 = S32(ctx->r2 << 24);
    // 0x801D0DF8: sra         $v0, $v0, 24
    ctx->r2 = S32(SIGNED(ctx->r2) >> 24);
L_801D0DFC:
    // 0x801D0DFC: b           L_801D0E30
    // 0x801D0E00: sb          $v0, 0x292C($at)
    MEM_B(0X292C, ctx->r1) = ctx->r2;
        goto L_801D0E30;
    // 0x801D0E00: sb          $v0, 0x292C($at)
    MEM_B(0X292C, ctx->r1) = ctx->r2;
L_801D0E04:
    // 0x801D0E04: bne         $v0, $a1, L_801D0E30
    if (ctx->r2 != ctx->r5) {
        // 0x801D0E08: nop

            goto L_801D0E30;
    }
    // 0x801D0E08: nop

    // 0x801D0E0C: lw          $t9, 0x2960($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X2960);
    // 0x801D0E10: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x801D0E14: sll         $v0, $v0, 24
    ctx->r2 = S32(ctx->r2 << 24);
    // 0x801D0E18: lh          $t1, 0x0($t9)
    ctx->r9 = MEM_H(ctx->r25, 0X0);
    // 0x801D0E1C: sra         $v0, $v0, 24
    ctx->r2 = S32(SIGNED(ctx->r2) >> 24);
    // 0x801D0E20: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801D0E24: bltz        $t1, L_801D0E30
    if (SIGNED(ctx->r9) < 0) {
        // 0x801D0E28: nop

            goto L_801D0E30;
    }
    // 0x801D0E28: nop

    // 0x801D0E2C: sb          $v0, 0x292C($at)
    MEM_B(0X292C, ctx->r1) = ctx->r2;
L_801D0E30:
    // 0x801D0E30: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801D0E34: lb          $v0, 0x292C($v0)
    ctx->r2 = MEM_B(ctx->r2, 0X292C);
    // 0x801D0E38: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x801D0E3C: bnel        $v0, $at, L_801D0EA0
    if (ctx->r2 != ctx->r1) {
        // 0x801D0E40: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801D0EA0;
    }
    goto skip_0;
    // 0x801D0E40: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801D0E44: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x801D0E48: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801D0E4C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801D0E50: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801D0E54: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801D0E58: sll         $t2, $v1, 1
    ctx->r10 = S32(ctx->r3 << 1);
    // 0x801D0E5C: addu        $v0, $a0, $t2
    ctx->r2 = ADD32(ctx->r4, ctx->r10);
    // 0x801D0E60: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801D0E64: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801D0E68: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801D0E6C: sb          $t4, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r12;
    // 0x801D0E70: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801D0E74: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801D0E78: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801D0E7C: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801D0E80: lw          $t9, 0x2918($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2918);
    // 0x801D0E84: jalr        $t9
    // 0x801D0E88: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801D0E88: nop

    after_0:
    // 0x801D0E8C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801D0E90: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x801D0E94: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x801D0E98: sh          $t8, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r24;
    // 0x801D0E9C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801D0EA0:
    // 0x801D0EA0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801D0EA4: jr          $ra
    // 0x801D0EA8: nop

    return;
    // 0x801D0EA8: nop

;}
RECOMP_FUNC void overlay_system_func_801D0EAC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D0EAC: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x801D0EB0: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x801D0EB4: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x801D0EB8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801D0EBC: addiu       $t7, $t7, 0x2930
    ctx->r15 = ADD32(ctx->r15, 0X2930);
    // 0x801D0EC0: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x801D0EC4: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x801D0EC8: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x801D0ECC: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x801D0ED0: addiu       $t0, $t7, 0x24
    ctx->r8 = ADD32(ctx->r15, 0X24);
    // 0x801D0ED4: addiu       $t6, $sp, 0x48
    ctx->r14 = ADD32(ctx->r29, 0X48);
L_801D0ED8:
    // 0x801D0ED8: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x801D0EDC: addiu       $t7, $t7, 0xC
    ctx->r15 = ADD32(ctx->r15, 0XC);
    // 0x801D0EE0: addiu       $t6, $t6, 0xC
    ctx->r14 = ADD32(ctx->r14, 0XC);
    // 0x801D0EE4: sw          $t9, -0xC($t6)
    MEM_W(-0XC, ctx->r14) = ctx->r25;
    // 0x801D0EE8: lw          $t8, -0x8($t7)
    ctx->r24 = MEM_W(ctx->r15, -0X8);
    // 0x801D0EEC: sw          $t8, -0x8($t6)
    MEM_W(-0X8, ctx->r14) = ctx->r24;
    // 0x801D0EF0: lw          $t9, -0x4($t7)
    ctx->r25 = MEM_W(ctx->r15, -0X4);
    // 0x801D0EF4: bne         $t7, $t0, L_801D0ED8
    if (ctx->r15 != ctx->r8) {
        // 0x801D0EF8: sw          $t9, -0x4($t6)
        MEM_W(-0X4, ctx->r14) = ctx->r25;
            goto L_801D0ED8;
    }
    // 0x801D0EF8: sw          $t9, -0x4($t6)
    MEM_W(-0X4, ctx->r14) = ctx->r25;
    // 0x801D0EFC: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x801D0F00: lui         $at, 0x4500
    ctx->r1 = S32(0X4500 << 16);
    // 0x801D0F04: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D0F08: sw          $t9, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->r25;
    // 0x801D0F0C: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801D0F10: lwc1        $f4, 0x4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801D0F14: lw          $s0, 0x64($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X64);
    // 0x801D0F18: lui         $t3, 0x801D
    ctx->r11 = S32(0X801D << 16);
    // 0x801D0F1C: div.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801D0F20: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x801D0F24: addiu       $t3, $t3, 0x2800
    ctx->r11 = ADD32(ctx->r11, 0X2800);
    // 0x801D0F28: addiu       $t4, $t4, 0x2830
    ctx->r12 = ADD32(ctx->r12, 0X2830);
    // 0x801D0F2C: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x801D0F30: lui         $t0, 0x600
    ctx->r8 = S32(0X600 << 16);
    // 0x801D0F34: addiu       $t0, $t0, 0x7460
    ctx->r8 = ADD32(ctx->r8, 0X7460);
    // 0x801D0F38: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801D0F3C: and         $t7, $t0, $at
    ctx->r15 = ctx->r8 & ctx->r1;
    // 0x801D0F40: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D0F44: addiu       $t9, $t9, 0x3178
    ctx->r25 = ADD32(ctx->r25, 0X3178);
    // 0x801D0F48: addiu       $t6, $zero, 0x20
    ctx->r14 = ADD32(0, 0X20);
    // 0x801D0F4C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801D0F50: addiu       $a3, $zero, 0x140
    ctx->r7 = ADD32(0, 0X140);
    // 0x801D0F54: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801D0F58: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x801D0F5C: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x801D0F60: mfc1        $t2, $f18
    ctx->r10 = (int32_t)ctx->f18.u32l;
    // 0x801D0F64: nop

    // 0x801D0F68: sh          $t2, 0x66($sp)
    MEM_H(0X66, ctx->r29) = ctx->r10;
    // 0x801D0F6C: sw          $t3, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r11;
    // 0x801D0F70: sw          $t4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r12;
    // 0x801D0F74: lw          $t5, 0x6F4($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X6F4);
    // 0x801D0F78: addu        $a2, $t5, $t7
    ctx->r6 = ADD32(ctx->r13, ctx->r15);
    // 0x801D0F7C: sw          $a2, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r6;
    // 0x801D0F80: jalr        $t9
    // 0x801D0F84: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801D0F84: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    after_0:
    // 0x801D0F88: bne         $v0, $zero, L_801D0FA8
    if (ctx->r2 != 0) {
        // 0x801D0F8C: addiu       $a0, $s0, 0x20
        ctx->r4 = ADD32(ctx->r16, 0X20);
            goto L_801D0FA8;
    }
    // 0x801D0F8C: addiu       $a0, $s0, 0x20
    ctx->r4 = ADD32(ctx->r16, 0X20);
    // 0x801D0F90: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x801D0F94: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801D0F98: jalr        $t9
    // 0x801D0F9C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801D0F9C: nop

    after_1:
    // 0x801D0FA0: b           L_801D0FD0
    // 0x801D0FA4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_801D0FD0;
    // 0x801D0FA4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_801D0FA8:
    // 0x801D0FA8: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D0FAC: addiu       $t9, $t9, 0x3794
    ctx->r25 = ADD32(ctx->r25, 0X3794);
    // 0x801D0FB0: jalr        $t9
    // 0x801D0FB4: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801D0FB4: addiu       $a1, $sp, 0x48
    ctx->r5 = ADD32(ctx->r29, 0X48);
    after_2:
    // 0x801D0FB8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801D0FBC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801D0FC0: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x801D0FC4: jalr        $t9
    // 0x801D0FC8: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801D0FC8: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_3:
    // 0x801D0FCC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_801D0FD0:
    // 0x801D0FD0: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x801D0FD4: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x801D0FD8: jr          $ra
    // 0x801D0FDC: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x801D0FDC: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void overlay_system_func_801D0FE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D0FE0: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x801D0FE4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801D0FE8: sw          $a0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r4;
    // 0x801D0FEC: lw          $t7, 0x64($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X64);
    // 0x801D0FF0: addiu       $a1, $sp, 0x44
    ctx->r5 = ADD32(ctx->r29, 0X44);
    // 0x801D0FF4: addiu       $a2, $sp, 0x38
    ctx->r6 = ADD32(ctx->r29, 0X38);
    // 0x801D0FF8: sw          $t7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r15;
    // 0x801D0FFC: lw          $t8, 0x24($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X24);
    // 0x801D1000: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x801D1004: lw          $t0, 0x14($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X14);
    // 0x801D1008: lw          $t1, 0x10($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X10);
    // 0x801D100C: lw          $t2, 0x10($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X10);
    // 0x801D1010: lw          $a3, 0x10($t2)
    ctx->r7 = MEM_W(ctx->r10, 0X10);
    // 0x801D1014: addiu       $a0, $a3, 0x78
    ctx->r4 = ADD32(ctx->r7, 0X78);
    // 0x801D1018: jal         0x80003CB0
    // 0x801D101C: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    func_80003CB0(rdram, ctx);
        goto after_0;
    // 0x801D101C: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    after_0:
    // 0x801D1020: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801D1024: lwc1        $f4, 0x38($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X38);
    // 0x801D1028: lw          $a3, 0x1C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1C);
    // 0x801D102C: lwc1        $f6, 0x3C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x801D1030: c.eq.s      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.fl == ctx->f4.fl;
    // 0x801D1034: nop

    // 0x801D1038: bc1tl       L_801D108C
    if (c1cs) {
        // 0x801D103C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801D108C;
    }
    goto skip_0;
    // 0x801D103C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801D1040: c.eq.s      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.fl == ctx->f6.fl;
    // 0x801D1044: lwc1        $f8, 0x40($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X40);
    // 0x801D1048: bc1tl       L_801D108C
    if (c1cs) {
        // 0x801D104C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801D108C;
    }
    goto skip_1;
    // 0x801D104C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x801D1050: c.eq.s      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.fl == ctx->f8.fl;
    // 0x801D1054: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D1058: addiu       $t9, $t9, 0x41B8
    ctx->r25 = ADD32(ctx->r25, 0X41B8);
    // 0x801D105C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x801D1060: bc1t        L_801D1088
    if (c1cs) {
        // 0x801D1064: addiu       $a1, $zero, 0x2
        ctx->r5 = ADD32(0, 0X2);
            goto L_801D1088;
    }
    // 0x801D1064: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801D1068: jalr        $t9
    // 0x801D106C: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801D106C: or          $a2, $a3, $zero
    ctx->r6 = ctx->r7 | 0;
    after_1:
    // 0x801D1070: lw          $v0, 0x50($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X50);
    // 0x801D1074: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801D1078: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801D107C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x801D1080: jalr        $t9
    // 0x801D1084: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801D1084: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_2:
L_801D1088:
    // 0x801D1088: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801D108C:
    // 0x801D108C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    // 0x801D1090: jr          $ra
    // 0x801D1094: nop

    return;
    // 0x801D1094: nop

;}
RECOMP_FUNC void overlay_system_func_801D1098(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D1098: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801D109C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801D10A0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801D10A4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801D10A8: lw          $t6, 0x24($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X24);
    // 0x801D10AC: lw          $a0, 0x64($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X64);
    // 0x801D10B0: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801D10B4: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801D10B8: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x801D10BC: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801D10C0: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801D10C4: addiu       $a3, $a3, 0x296C
    ctx->r7 = ADD32(ctx->r7, 0X296C);
    // 0x801D10C8: addiu       $a2, $a2, 0x2964
    ctx->r6 = ADD32(ctx->r6, 0X2964);
    // 0x801D10CC: lw          $t9, 0x10($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X10);
    // 0x801D10D0: addiu       $a1, $a1, 0x295C
    ctx->r5 = ADD32(ctx->r5, 0X295C);
    // 0x801D10D4: lw          $t0, 0x10($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X10);
    // 0x801D10D8: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D10DC: addiu       $t9, $t9, 0x77C0
    ctx->r25 = ADD32(ctx->r25, 0X77C0);
    // 0x801D10E0: lw          $t1, 0x10($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X10);
    // 0x801D10E4: jalr        $t9
    // 0x801D10E8: sw          $t1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801D10E8: sw          $t1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r9;
    after_0:
    // 0x801D10EC: lw          $t2, 0x24($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X24);
    // 0x801D10F0: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801D10F4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801D10F8: sw          $v0, 0x3C($t2)
    MEM_W(0X3C, ctx->r10) = ctx->r2;
    // 0x801D10FC: lh          $a1, 0xE($s0)
    ctx->r5 = MEM_H(ctx->r16, 0XE);
    // 0x801D1100: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x801D1104: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x801D1108: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x801D110C: sll         $t3, $a1, 1
    ctx->r11 = S32(ctx->r5 << 1);
    // 0x801D1110: addu        $v1, $s0, $t3
    ctx->r3 = ADD32(ctx->r16, ctx->r11);
    // 0x801D1114: sh          $a1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r5;
    // 0x801D1118: lbu         $t6, 0x9($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X9);
    // 0x801D111C: lbu         $t4, 0x8($v1)
    ctx->r12 = MEM_BU(ctx->r3, 0X8);
    // 0x801D1120: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x801D1124: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x801D1128: sb          $t5, 0x8($v1)
    MEM_B(0X8, ctx->r3) = ctx->r13;
    // 0x801D112C: addu        $t9, $t9, $t7
    ctx->r25 = ADD32(ctx->r25, ctx->r15);
    // 0x801D1130: lw          $t9, 0x2958($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2958);
    // 0x801D1134: jalr        $t9
    // 0x801D1138: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801D1138: nop

    after_1:
    // 0x801D113C: lh          $t8, 0xE($s0)
    ctx->r24 = MEM_H(ctx->r16, 0XE);
    // 0x801D1140: addiu       $t0, $t8, -0x1
    ctx->r8 = ADD32(ctx->r24, -0X1);
    // 0x801D1144: sh          $t0, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r8;
    // 0x801D1148: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801D114C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801D1150: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x801D1154: jr          $ra
    // 0x801D1158: nop

    return;
    // 0x801D1158: nop

;}
RECOMP_FUNC void overlay_system_func_801D115C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D115C: addiu       $sp, $sp, -0xD8
    ctx->r29 = ADD32(ctx->r29, -0XD8);
    // 0x801D1160: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x801D1164: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x801D1168: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x801D116C: lw          $s0, 0x64($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X64);
    // 0x801D1170: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x801D1174: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801D1178: addiu       $a0, $sp, 0x8C
    ctx->r4 = ADD32(ctx->r29, 0X8C);
    // 0x801D117C: addiu       $a1, $sp, 0x4C
    ctx->r5 = ADD32(ctx->r29, 0X4C);
    // 0x801D1180: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801D1184: lw          $t9, 0x10($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X10);
    // 0x801D1188: lw          $t0, 0x10($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X10);
    // 0x801D118C: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D1190: addiu       $t9, $t9, 0x3558
    ctx->r25 = ADD32(ctx->r25, 0X3558);
    // 0x801D1194: lw          $a2, 0x10($t0)
    ctx->r6 = MEM_W(ctx->r8, 0X10);
    // 0x801D1198: sw          $a3, 0xD8($sp)
    MEM_W(0XD8, ctx->r29) = ctx->r7;
    // 0x801D119C: jalr        $t9
    // 0x801D11A0: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801D11A0: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    after_0:
    // 0x801D11A4: lhu         $v0, 0x48($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X48);
    // 0x801D11A8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801D11AC: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    // 0x801D11B0: andi        $t1, $v0, 0x8000
    ctx->r9 = ctx->r2 & 0X8000;
    // 0x801D11B4: beq         $t1, $zero, L_801D11D8
    if (ctx->r9 == 0) {
        // 0x801D11B8: andi        $t2, $v0, 0x1000
        ctx->r10 = ctx->r2 & 0X1000;
            goto L_801D11D8;
    }
    // 0x801D11B8: andi        $t2, $v0, 0x1000
    ctx->r10 = ctx->r2 & 0X1000;
    // 0x801D11BC: beq         $t2, $zero, L_801D11D8
    if (ctx->r10 == 0) {
        // 0x801D11C0: lui         $t9, 0x8005
        ctx->r25 = S32(0X8005 << 16);
            goto L_801D11D8;
    }
    // 0x801D11C0: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D11C4: addiu       $t9, $t9, 0x7ADC
    ctx->r25 = ADD32(ctx->r25, 0X7ADC);
    // 0x801D11C8: jalr        $t9
    // 0x801D11CC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801D11CC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x801D11D0: b           L_801D1258
    // 0x801D11D4: nop

        goto L_801D1258;
    // 0x801D11D4: nop

L_801D11D8:
    // 0x801D11D8: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D11DC: addiu       $t9, $t9, 0x7B6C
    ctx->r25 = ADD32(ctx->r25, 0X7B6C);
    // 0x801D11E0: addiu       $t3, $sp, 0x4C
    ctx->r11 = ADD32(ctx->r29, 0X4C);
    // 0x801D11E4: addiu       $t4, $sp, 0xCC
    ctx->r12 = ADD32(ctx->r29, 0XCC);
    // 0x801D11E8: sw          $t4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r12;
    // 0x801D11EC: sw          $t3, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r11;
    // 0x801D11F0: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    // 0x801D11F4: jalr        $t9
    // 0x801D11F8: addiu       $a3, $sp, 0x8C
    ctx->r7 = ADD32(ctx->r29, 0X8C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801D11F8: addiu       $a3, $sp, 0x8C
    ctx->r7 = ADD32(ctx->r29, 0X8C);
    after_2:
    // 0x801D11FC: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D1200: addiu       $t9, $t9, 0x7CAC
    ctx->r25 = ADD32(ctx->r25, 0X7CAC);
    // 0x801D1204: lui         $a1, 0x3EAA
    ctx->r5 = S32(0X3EAA << 16);
    // 0x801D1208: ori         $a1, $a1, 0xAAAB
    ctx->r5 = ctx->r5 | 0XAAAB;
    // 0x801D120C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801D1210: jalr        $t9
    // 0x801D1214: addiu       $a2, $sp, 0xCC
    ctx->r6 = ADD32(ctx->r29, 0XCC);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801D1214: addiu       $a2, $sp, 0xCC
    ctx->r6 = ADD32(ctx->r29, 0XCC);
    after_3:
    // 0x801D1218: jal         0x801D1284
    // 0x801D121C: lw          $a0, 0xD8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XD8);
    overlay_system_func_801D1284(rdram, ctx);
        goto after_4;
    // 0x801D121C: lw          $a0, 0xD8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XD8);
    after_4:
    // 0x801D1220: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D1224: addiu       $t9, $t9, 0x7E0C
    ctx->r25 = ADD32(ctx->r25, 0X7E0C);
    // 0x801D1228: jalr        $t9
    // 0x801D122C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x801D122C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x801D1230: jal         0x801D1308
    // 0x801D1234: lw          $a0, 0xD8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XD8);
    overlay_system_func_801D1308(rdram, ctx);
        goto after_6;
    // 0x801D1234: lw          $a0, 0xD8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XD8);
    after_6:
    // 0x801D1238: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D123C: addiu       $t9, $t9, 0x7D50
    ctx->r25 = ADD32(ctx->r25, 0X7D50);
    // 0x801D1240: jalr        $t9
    // 0x801D1244: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x801D1244: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_7:
    // 0x801D1248: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D124C: addiu       $t9, $t9, 0x3270
    ctx->r25 = ADD32(ctx->r25, 0X3270);
    // 0x801D1250: jalr        $t9
    // 0x801D1254: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x801D1254: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_8:
L_801D1258:
    // 0x801D1258: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D125C: addiu       $t9, $t9, 0x7ED0
    ctx->r25 = ADD32(ctx->r25, 0X7ED0);
    // 0x801D1260: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801D1264: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801D1268: jalr        $t9
    // 0x801D126C: addiu       $a2, $sp, 0x8C
    ctx->r6 = ADD32(ctx->r29, 0X8C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x801D126C: addiu       $a2, $sp, 0x8C
    ctx->r6 = ADD32(ctx->r29, 0X8C);
    after_9:
    // 0x801D1270: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x801D1274: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x801D1278: addiu       $sp, $sp, 0xD8
    ctx->r29 = ADD32(ctx->r29, 0XD8);
    // 0x801D127C: jr          $ra
    // 0x801D1280: nop

    return;
    // 0x801D1280: nop

;}
RECOMP_FUNC void overlay_system_func_801D1284(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D1284: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801D1288: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x801D128C: lwc1        $f4, 0x4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801D1290: lwc1        $f6, 0x8($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801D1294: lui         $at, 0x4060
    ctx->r1 = S32(0X4060 << 16);
    // 0x801D1298: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x801D129C: div.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x801D12A0: lui         $at, 0x4480
    ctx->r1 = S32(0X4480 << 16);
    // 0x801D12A4: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D12A8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801D12AC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801D12B0: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x801D12B4: lw          $a0, 0x64($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X64);
    // 0x801D12B8: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D12BC: addiu       $t9, $t9, 0x3800
    ctx->r25 = ADD32(ctx->r25, 0X3800);
    // 0x801D12C0: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x801D12C4: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x801D12C8: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x801D12CC: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x801D12D0: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x801D12D4: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x801D12D8: mfc1        $t8, $f18
    ctx->r24 = (int32_t)ctx->f18.u32l;
    // 0x801D12DC: jalr        $t9
    // 0x801D12E0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801D12E0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    after_0:
    // 0x801D12E4: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x801D12E8: lui         $t9, 0x8005
    ctx->r25 = S32(0X8005 << 16);
    // 0x801D12EC: addiu       $t9, $t9, 0x7DE0
    ctx->r25 = ADD32(ctx->r25, 0X7DE0);
    // 0x801D12F0: jalr        $t9
    // 0x801D12F4: addiu       $a1, $a0, 0x20
    ctx->r5 = ADD32(ctx->r4, 0X20);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801D12F4: addiu       $a1, $a0, 0x20
    ctx->r5 = ADD32(ctx->r4, 0X20);
    after_1:
    // 0x801D12F8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801D12FC: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x801D1300: jr          $ra
    // 0x801D1304: nop

    return;
    // 0x801D1304: nop

;}
RECOMP_FUNC void overlay_system_func_801D1308(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801D1308: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x801D130C: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x801D1310: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x801D1314: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x801D1318: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x801D131C: lw          $v0, 0x64($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X64);
    // 0x801D1320: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x801D1324: lh          $t7, -0x7D14($t7)
    ctx->r15 = MEM_H(ctx->r15, -0X7D14);
    // 0x801D1328: lw          $t6, 0x10($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X10);
    // 0x801D132C: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x801D1330: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x801D1334: addu        $t9, $t6, $t8
    ctx->r25 = ADD32(ctx->r14, ctx->r24);
    // 0x801D1338: lw          $s0, 0x4($t9)
    ctx->r16 = MEM_W(ctx->r25, 0X4);
    // 0x801D133C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801D1340: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x801D1344: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x801D1348: addiu       $s1, $s1, -0x7F60
    ctx->r17 = ADD32(ctx->r17, -0X7F60);
    // 0x801D134C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D1350: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D1354: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x801D1358: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D135C: jalr        $s1
    // 0x801D1360: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_0;
    // 0x801D1360: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x801D1364: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x801D1368: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D136C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D1370: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D1374: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1378: addiu       $a3, $zero, 0x3
    ctx->r7 = ADD32(0, 0X3);
    // 0x801D137C: jalr        $s1
    // 0x801D1380: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_1;
    // 0x801D1380: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_1:
    // 0x801D1384: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x801D1388: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801D138C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D1390: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D1394: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1398: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    // 0x801D139C: jalr        $s1
    // 0x801D13A0: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_2;
    // 0x801D13A0: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    after_2:
    // 0x801D13A4: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x801D13A8: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D13AC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D13B0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D13B4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D13B8: addiu       $a3, $zero, 0x8
    ctx->r7 = ADD32(0, 0X8);
    // 0x801D13BC: jalr        $s1
    // 0x801D13C0: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_3;
    // 0x801D13C0: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    after_3:
    // 0x801D13C4: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x801D13C8: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x801D13CC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D13D0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D13D4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D13D8: addiu       $a3, $zero, 0x12
    ctx->r7 = ADD32(0, 0X12);
    // 0x801D13DC: jalr        $s1
    // 0x801D13E0: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_4;
    // 0x801D13E0: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    after_4:
    // 0x801D13E4: lui         $at, 0xC370
    ctx->r1 = S32(0XC370 << 16);
    // 0x801D13E8: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D13EC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D13F0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801D13F4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D13F8: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x801D13FC: jalr        $s1
    // 0x801D1400: swc1        $f18, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f18.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_5;
    // 0x801D1400: swc1        $f18, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f18.u32l;
    after_5:
    // 0x801D1404: lui         $at, 0xC2DC
    ctx->r1 = S32(0XC2DC << 16);
    // 0x801D1408: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801D140C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D1410: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801D1414: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1418: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x801D141C: jalr        $s1
    // 0x801D1420: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_6;
    // 0x801D1420: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_6:
    // 0x801D1424: lui         $at, 0x4270
    ctx->r1 = S32(0X4270 << 16);
    // 0x801D1428: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D142C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D1430: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801D1434: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1438: addiu       $a3, $zero, 0x3
    ctx->r7 = ADD32(0, 0X3);
    // 0x801D143C: jalr        $s1
    // 0x801D1440: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_7;
    // 0x801D1440: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_7:
    // 0x801D1444: lui         $at, 0x435C
    ctx->r1 = S32(0X435C << 16);
    // 0x801D1448: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801D144C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D1450: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801D1454: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1458: addiu       $a3, $zero, 0x3
    ctx->r7 = ADD32(0, 0X3);
    // 0x801D145C: jalr        $s1
    // 0x801D1460: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_8;
    // 0x801D1460: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    after_8:
    // 0x801D1464: lui         $at, 0x4352
    ctx->r1 = S32(0X4352 << 16);
    // 0x801D1468: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D146C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D1470: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D1474: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1478: addiu       $a3, $zero, 0x3
    ctx->r7 = ADD32(0, 0X3);
    // 0x801D147C: jalr        $s1
    // 0x801D1480: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_9;
    // 0x801D1480: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    after_9:
    // 0x801D1484: lui         $at, 0x4270
    ctx->r1 = S32(0X4270 << 16);
    // 0x801D1488: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x801D148C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D1490: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801D1494: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1498: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    // 0x801D149C: jalr        $s1
    // 0x801D14A0: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_10;
    // 0x801D14A0: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    after_10:
    // 0x801D14A4: lui         $at, 0x4352
    ctx->r1 = S32(0X4352 << 16);
    // 0x801D14A8: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D14AC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D14B0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D14B4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D14B8: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    // 0x801D14BC: jalr        $s1
    // 0x801D14C0: swc1        $f18, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f18.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_11;
    // 0x801D14C0: swc1        $f18, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f18.u32l;
    after_11:
    // 0x801D14C4: lui         $at, 0xC35C
    ctx->r1 = S32(0XC35C << 16);
    // 0x801D14C8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801D14CC: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D14D0: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801D14D4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D14D8: addiu       $a3, $zero, 0x7
    ctx->r7 = ADD32(0, 0X7);
    // 0x801D14DC: jalr        $s1
    // 0x801D14E0: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_12;
    // 0x801D14E0: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_12:
    // 0x801D14E4: lui         $at, 0x4320
    ctx->r1 = S32(0X4320 << 16);
    // 0x801D14E8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D14EC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D14F0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801D14F4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D14F8: addiu       $a3, $zero, 0x8
    ctx->r7 = ADD32(0, 0X8);
    // 0x801D14FC: jalr        $s1
    // 0x801D1500: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_13;
    // 0x801D1500: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_13:
    // 0x801D1504: lui         $at, 0x4352
    ctx->r1 = S32(0X4352 << 16);
    // 0x801D1508: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801D150C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D1510: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D1514: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1518: addiu       $a3, $zero, 0x8
    ctx->r7 = ADD32(0, 0X8);
    // 0x801D151C: jalr        $s1
    // 0x801D1520: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_14;
    // 0x801D1520: swc1        $f8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f8.u32l;
    after_14:
    // 0x801D1524: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x801D1528: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D152C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D1530: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801D1534: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1538: addiu       $a3, $zero, 0x12
    ctx->r7 = ADD32(0, 0X12);
    // 0x801D153C: jalr        $s1
    // 0x801D1540: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_15;
    // 0x801D1540: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    after_15:
    // 0x801D1544: lui         $at, 0xC370
    ctx->r1 = S32(0XC370 << 16);
    // 0x801D1548: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x801D154C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D1550: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801D1554: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1558: addiu       $a3, $zero, 0x12
    ctx->r7 = ADD32(0, 0X12);
    // 0x801D155C: jalr        $s1
    // 0x801D1560: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_16;
    // 0x801D1560: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    after_16:
    // 0x801D1564: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801D1568: lw          $t0, -0x5178($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X5178);
    // 0x801D156C: addiu       $at, $zero, 0x2B
    ctx->r1 = ADD32(0, 0X2B);
    // 0x801D1570: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x801D1574: bne         $t0, $at, L_801D15B4
    if (ctx->r8 != ctx->r1) {
        // 0x801D1578: or          $a1, $zero, $zero
        ctx->r5 = 0 | 0;
            goto L_801D15B4;
    }
    // 0x801D1578: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801D157C: lui         $at, 0x42F0
    ctx->r1 = S32(0X42F0 << 16);
    // 0x801D1580: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D1584: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D1588: addiu       $a3, $zero, 0x12
    ctx->r7 = ADD32(0, 0X12);
    // 0x801D158C: jalr        $s1
    // 0x801D1590: swc1        $f18, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f18.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_17;
    // 0x801D1590: swc1        $f18, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f18.u32l;
    after_17:
    // 0x801D1594: lui         $at, 0x4316
    ctx->r1 = S32(0X4316 << 16);
    // 0x801D1598: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801D159C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801D15A0: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801D15A4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x801D15A8: addiu       $a3, $zero, 0x12
    ctx->r7 = ADD32(0, 0X12);
    // 0x801D15AC: jalr        $s1
    // 0x801D15B0: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_18;
    // 0x801D15B0: swc1        $f4, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f4.u32l;
    after_18:
L_801D15B4:
    // 0x801D15B4: lw          $t5, 0x24($s2)
    ctx->r13 = MEM_W(ctx->r18, 0X24);
    // 0x801D15B8: lw          $v0, 0x64($s2)
    ctx->r2 = MEM_W(ctx->r18, 0X64);
    // 0x801D15BC: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801D15C0: lw          $t7, 0x14($t5)
    ctx->r15 = MEM_W(ctx->r13, 0X14);
    // 0x801D15C4: lh          $t2, -0x7D14($t2)
    ctx->r10 = MEM_H(ctx->r10, -0X7D14);
    // 0x801D15C8: lw          $t1, 0x10($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X10);
    // 0x801D15CC: lw          $t6, 0x14($t7)
    ctx->r14 = MEM_W(ctx->r15, 0X14);
    // 0x801D15D0: lui         $s1, 0x800A
    ctx->r17 = S32(0X800A << 16);
    // 0x801D15D4: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801D15D8: lw          $t8, 0x10($t6)
    ctx->r24 = MEM_W(ctx->r14, 0X10);
    // 0x801D15DC: addiu       $s1, $s1, 0x3A50
    ctx->r17 = ADD32(ctx->r17, 0X3A50);
    // 0x801D15E0: addu        $t4, $t1, $t3
    ctx->r12 = ADD32(ctx->r9, ctx->r11);
    // 0x801D15E4: lw          $s0, 0x4($t4)
    ctx->r16 = MEM_W(ctx->r12, 0X4);
    // 0x801D15E8: jalr        $s1
    // 0x801D15EC: lhu         $a0, 0x60($t8)
    ctx->r4 = MEM_HU(ctx->r24, 0X60);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_19;
    // 0x801D15EC: lhu         $a0, 0x60($t8)
    ctx->r4 = MEM_HU(ctx->r24, 0X60);
    after_19:
    // 0x801D15F0: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x801D15F4: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x801D15F8: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D15FC: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801D1600: lui         $at, 0x435C
    ctx->r1 = S32(0X435C << 16);
    // 0x801D1604: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D1608: lui         $v1, 0x8009
    ctx->r3 = S32(0X8009 << 16);
    // 0x801D160C: addiu       $v1, $v1, 0x7330
    ctx->r3 = ADD32(ctx->r3, 0X7330);
    // 0x801D1610: div.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f16.fl = DIV_S(ctx->f8.fl, ctx->f10.fl);
    // 0x801D1614: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x801D1618: swc1        $f4, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f4.u32l;
    // 0x801D161C: lw          $t9, 0x24($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X24);
    // 0x801D1620: lw          $t0, 0x14($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X14);
    // 0x801D1624: lw          $t2, 0x14($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X14);
    // 0x801D1628: lw          $t1, 0x10($t2)
    ctx->r9 = MEM_W(ctx->r10, 0X10);
    // 0x801D162C: lhu         $a0, 0x60($t1)
    ctx->r4 = MEM_HU(ctx->r9, 0X60);
    // 0x801D1630: jalr        $v1
    // 0x801D1634: sw          $v1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_20;
    // 0x801D1634: sw          $v1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r3;
    after_20:
    // 0x801D1638: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x801D163C: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x801D1640: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D1644: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801D1648: lui         $at, 0x435C
    ctx->r1 = S32(0X435C << 16);
    // 0x801D164C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D1650: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801D1654: div.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f16.fl = DIV_S(ctx->f8.fl, ctx->f10.fl);
    // 0x801D1658: swc1        $f0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f0.u32l;
    // 0x801D165C: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x801D1660: swc1        $f4, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f4.u32l;
    // 0x801D1664: lw          $t3, 0x24($s2)
    ctx->r11 = MEM_W(ctx->r18, 0X24);
    // 0x801D1668: lw          $t4, 0x14($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X14);
    // 0x801D166C: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801D1670: lw          $t7, 0x10($t5)
    ctx->r15 = MEM_W(ctx->r13, 0X10);
    // 0x801D1674: lh          $t6, 0x60($t7)
    ctx->r14 = MEM_H(ctx->r15, 0X60);
    // 0x801D1678: slti        $at, $t6, -0x445B
    ctx->r1 = SIGNED(ctx->r14) < -0X445B ? 1 : 0;
    // 0x801D167C: bne         $at, $zero, L_801D16A4
    if (ctx->r1 != 0) {
        // 0x801D1680: lui         $at, 0xC2C8
        ctx->r1 = S32(0XC2C8 << 16);
            goto L_801D16A4;
    }
    // 0x801D1680: lui         $at, 0xC2C8
    ctx->r1 = S32(0XC2C8 << 16);
    // 0x801D1684: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D1688: lui         $at, 0x430C
    ctx->r1 = S32(0X430C << 16);
    // 0x801D168C: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801D1690: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x801D1694: add.s       $f16, $f0, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f16.fl = ctx->f0.fl + ctx->f10.fl;
    // 0x801D1698: swc1        $f8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f8.u32l;
    // 0x801D169C: b           L_801D16B8
    // 0x801D16A0: swc1        $f16, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f16.u32l;
        goto L_801D16B8;
    // 0x801D16A0: swc1        $f16, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f16.u32l;
L_801D16A4:
    // 0x801D16A4: lui         $at, 0x437A
    ctx->r1 = S32(0X437A << 16);
    // 0x801D16A8: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D16AC: nop

    // 0x801D16B0: add.s       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f0.fl + ctx->f18.fl;
    // 0x801D16B4: swc1        $f4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f4.u32l;
L_801D16B8:
    // 0x801D16B8: lw          $t8, 0x24($s2)
    ctx->r24 = MEM_W(ctx->r18, 0X24);
    // 0x801D16BC: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x801D16C0: lw          $t0, 0x14($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X14);
    // 0x801D16C4: lw          $v0, 0x10($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X10);
    // 0x801D16C8: lh          $t2, 0x60($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X60);
    // 0x801D16CC: bgezl       $t2, L_801D1730
    if (SIGNED(ctx->r10) >= 0) {
        // 0x801D16D0: lw          $t0, 0x10($v0)
        ctx->r8 = MEM_W(ctx->r2, 0X10);
            goto L_801D1730;
    }
    goto skip_0;
    // 0x801D16D0: lw          $t0, 0x10($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X10);
    skip_0:
    // 0x801D16D4: lh          $t1, 0x120($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X120);
    // 0x801D16D8: lwc1        $f6, 0x58($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801D16DC: mtc1        $t1, $f8
    ctx->f8.u32l = ctx->r9;
    // 0x801D16E0: nop

    // 0x801D16E4: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801D16E8: c.lt.s      $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f6.fl < ctx->f10.fl;
    // 0x801D16EC: nop

    // 0x801D16F0: bc1fl       L_801D1730
    if (!c1cs) {
        // 0x801D16F4: lw          $t0, 0x10($v0)
        ctx->r8 = MEM_W(ctx->r2, 0X10);
            goto L_801D1730;
    }
    goto skip_1;
    // 0x801D16F4: lw          $t0, 0x10($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X10);
    skip_1:
    // 0x801D16F8: trunc.w.s   $f16, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    ctx->f16.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x801D16FC: mfc1        $t4, $f16
    ctx->r12 = (int32_t)ctx->f16.u32l;
    // 0x801D1700: nop

    // 0x801D1704: sh          $t4, 0x120($s0)
    MEM_H(0X120, ctx->r16) = ctx->r12;
    // 0x801D1708: lwc1        $f18, 0x60($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801D170C: trunc.w.s   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x801D1710: mfc1        $t7, $f4
    ctx->r15 = (int32_t)ctx->f4.u32l;
    // 0x801D1714: nop

    // 0x801D1718: sh          $t7, 0x124($s0)
    MEM_H(0X124, ctx->r16) = ctx->r15;
    // 0x801D171C: lw          $t6, 0x24($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X24);
    // 0x801D1720: lw          $t8, 0x14($t6)
    ctx->r24 = MEM_W(ctx->r14, 0X14);
    // 0x801D1724: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x801D1728: lw          $v0, 0x10($t9)
    ctx->r2 = MEM_W(ctx->r25, 0X10);
    // 0x801D172C: lw          $t0, 0x10($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X10);
L_801D1730:
    // 0x801D1730: jalr        $s1
    // 0x801D1734: lhu         $a0, 0x60($t0)
    ctx->r4 = MEM_HU(ctx->r8, 0X60);
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_21;
    // 0x801D1734: lhu         $a0, 0x60($t0)
    ctx->r4 = MEM_HU(ctx->r8, 0X60);
    after_21:
    // 0x801D1738: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x801D173C: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x801D1740: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D1744: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801D1748: lui         $at, 0x435C
    ctx->r1 = S32(0X435C << 16);
    // 0x801D174C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D1750: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x801D1754: div.s       $f16, $f10, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f6.fl);
    // 0x801D1758: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x801D175C: swc1        $f4, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f4.u32l;
    // 0x801D1760: lw          $t2, 0x24($s2)
    ctx->r10 = MEM_W(ctx->r18, 0X24);
    // 0x801D1764: lw          $t1, 0x14($t2)
    ctx->r9 = MEM_W(ctx->r10, 0X14);
    // 0x801D1768: lw          $t3, 0x14($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X14);
    // 0x801D176C: lw          $t4, 0x10($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X10);
    // 0x801D1770: lw          $t5, 0x10($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X10);
    // 0x801D1774: jalr        $t9
    // 0x801D1778: lhu         $a0, 0x60($t5)
    ctx->r4 = MEM_HU(ctx->r13, 0X60);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_22;
    // 0x801D1778: lhu         $a0, 0x60($t5)
    ctx->r4 = MEM_HU(ctx->r13, 0X60);
    after_22:
    // 0x801D177C: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x801D1780: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x801D1784: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D1788: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801D178C: lui         $at, 0x435C
    ctx->r1 = S32(0X435C << 16);
    // 0x801D1790: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D1794: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801D1798: div.s       $f16, $f10, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f6.fl);
    // 0x801D179C: swc1        $f0, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f0.u32l;
    // 0x801D17A0: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x801D17A4: swc1        $f4, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f4.u32l;
    // 0x801D17A8: lw          $t7, 0x24($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X24);
    // 0x801D17AC: lw          $t6, 0x14($t7)
    ctx->r14 = MEM_W(ctx->r15, 0X14);
    // 0x801D17B0: lw          $t8, 0x14($t6)
    ctx->r24 = MEM_W(ctx->r14, 0X14);
    // 0x801D17B4: lw          $t0, 0x10($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X10);
    // 0x801D17B8: lw          $t2, 0x10($t0)
    ctx->r10 = MEM_W(ctx->r8, 0X10);
    // 0x801D17BC: lh          $t1, 0x60($t2)
    ctx->r9 = MEM_H(ctx->r10, 0X60);
    // 0x801D17C0: slti        $at, $t1, -0x445B
    ctx->r1 = SIGNED(ctx->r9) < -0X445B ? 1 : 0;
    // 0x801D17C4: bne         $at, $zero, L_801D17EC
    if (ctx->r1 != 0) {
        // 0x801D17C8: lui         $at, 0xC2C8
        ctx->r1 = S32(0XC2C8 << 16);
            goto L_801D17EC;
    }
    // 0x801D17C8: lui         $at, 0xC2C8
    ctx->r1 = S32(0XC2C8 << 16);
    // 0x801D17CC: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801D17D0: lui         $at, 0xC30C
    ctx->r1 = S32(0XC30C << 16);
    // 0x801D17D4: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801D17D8: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801D17DC: add.s       $f16, $f0, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f16.fl = ctx->f0.fl + ctx->f6.fl;
    // 0x801D17E0: swc1        $f10, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f10.u32l;
    // 0x801D17E4: b           L_801D1800
    // 0x801D17E8: swc1        $f16, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f16.u32l;
        goto L_801D1800;
    // 0x801D17E8: swc1        $f16, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f16.u32l;
L_801D17EC:
    // 0x801D17EC: lui         $at, 0xC37A
    ctx->r1 = S32(0XC37A << 16);
    // 0x801D17F0: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801D17F4: nop

    // 0x801D17F8: add.s       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f0.fl + ctx->f18.fl;
    // 0x801D17FC: swc1        $f4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f4.u32l;
L_801D1800:
    // 0x801D1800: lw          $t3, 0x24($s2)
    ctx->r11 = MEM_W(ctx->r18, 0X24);
    // 0x801D1804: lw          $t4, 0x14($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X14);
    // 0x801D1808: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801D180C: lw          $t9, 0x10($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X10);
    // 0x801D1810: lw          $t7, 0x10($t9)
    ctx->r15 = MEM_W(ctx->r25, 0X10);
    // 0x801D1814: lh          $t6, 0x60($t7)
    ctx->r14 = MEM_H(ctx->r15, 0X60);
    // 0x801D1818: bgezl       $t6, L_801D186C
    if (SIGNED(ctx->r14) >= 0) {
        // 0x801D181C: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_801D186C;
    }
    goto skip_2;
    // 0x801D181C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_2:
    // 0x801D1820: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x801D1824: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801D1828: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x801D182C: nop

    // 0x801D1830: cvt.s.w     $f6, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    ctx->f6.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801D1834: c.lt.s      $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f8.fl < ctx->f6.fl;
    // 0x801D1838: nop

    // 0x801D183C: bc1fl       L_801D186C
    if (!c1cs) {
        // 0x801D1840: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_801D186C;
    }
    goto skip_3;
    // 0x801D1840: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_3:
    // 0x801D1844: trunc.w.s   $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    ctx->f16.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x801D1848: mfc1        $t2, $f16
    ctx->r10 = (int32_t)ctx->f16.u32l;
    // 0x801D184C: nop

    // 0x801D1850: sh          $t2, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r10;
    // 0x801D1854: lwc1        $f18, 0x60($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801D1858: trunc.w.s   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x801D185C: mfc1        $t3, $f4
    ctx->r11 = (int32_t)ctx->f4.u32l;
    // 0x801D1860: nop

    // 0x801D1864: sh          $t3, 0x4($s0)
    MEM_H(0X4, ctx->r16) = ctx->r11;
    // 0x801D1868: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_801D186C:
    // 0x801D186C: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x801D1870: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x801D1874: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x801D1878: jr          $ra
    // 0x801D187C: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    return;
    // 0x801D187C: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
;}
RECOMP_FUNC void overlay_system_1_func_801CAEA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CAEA0: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x801CAEA4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CAEA8: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CAEAC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CAEB0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801CAEB4: jal         0x80020A0C
    // 0x801CAEB8: lw          $a0, -0x53D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53D4);
    func_80020A0C(rdram, ctx);
        goto after_0;
    // 0x801CAEB8: lw          $a0, -0x53D4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53D4);
    after_0:
    // 0x801CAEBC: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x801CAEC0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801CAEC4: andi        $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 & 0X1000;
    // 0x801CAEC8: beql        $t7, $zero, L_801CB0F8
    if (ctx->r15 == 0) {
        // 0x801CAECC: lh          $v1, 0xE($s0)
        ctx->r3 = MEM_H(ctx->r16, 0XE);
            goto L_801CB0F8;
    }
    goto skip_0;
    // 0x801CAECC: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    skip_0:
    // 0x801CAED0: lw          $a0, -0x5178($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X5178);
    // 0x801CAED4: lw          $v1, 0x34($s0)
    ctx->r3 = MEM_W(ctx->r16, 0X34);
    // 0x801CAED8: lw          $a2, 0x24($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X24);
    // 0x801CAEDC: beq         $a0, $zero, L_801CAF40
    if (ctx->r4 == 0) {
        // 0x801CAEE0: lui         $t5, 0x801D
        ctx->r13 = S32(0X801D << 16);
            goto L_801CAF40;
    }
    // 0x801CAEE0: lui         $t5, 0x801D
    ctx->r13 = S32(0X801D << 16);
    // 0x801CAEE4: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x801CAEE8: lw          $t8, 0x78C4($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X78C4);
    // 0x801CAEEC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CAEF0: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x801CAEF4: bnel        $t8, $zero, L_801CB178
    if (ctx->r24 != 0) {
        // 0x801CAEF8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_801CB178;
    }
    goto skip_1;
    // 0x801CAEF8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x801CAEFC: sw          $a0, 0x78C4($at)
    MEM_W(0X78C4, ctx->r1) = ctx->r4;
    // 0x801CAF00: lw          $t9, 0x0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X0);
    // 0x801CAF04: lui         $at, 0xFFC0
    ctx->r1 = S32(0XFFC0 << 16);
    // 0x801CAF08: ori         $at, $at, 0x1808
    ctx->r1 = ctx->r1 | 0X1808;
    // 0x801CAF0C: and         $t0, $t9, $at
    ctx->r8 = ctx->r25 & ctx->r1;
    // 0x801CAF10: sw          $t0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r8;
    // 0x801CAF14: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X0);
    // 0x801CAF18: lhu         $t3, 0x8($v0)
    ctx->r11 = MEM_HU(ctx->r2, 0X8);
    // 0x801CAF1C: lui         $at, 0x73FF
    ctx->r1 = S32(0X73FF << 16);
    // 0x801CAF20: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801CAF24: and         $t2, $t1, $at
    ctx->r10 = ctx->r9 & ctx->r1;
    // 0x801CAF28: andi        $t4, $t3, 0xF7FF
    ctx->r12 = ctx->r11 & 0XF7FF;
    // 0x801CAF2C: sw          $t2, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r10;
    // 0x801CAF30: sh          $zero, 0xA($v0)
    MEM_H(0XA, ctx->r2) = 0;
    // 0x801CAF34: sh          $zero, 0xC($v0)
    MEM_H(0XC, ctx->r2) = 0;
    // 0x801CAF38: b           L_801CB174
    // 0x801CAF3C: sh          $t4, 0x8($v0)
    MEM_H(0X8, ctx->r2) = ctx->r12;
        goto L_801CB174;
    // 0x801CAF3C: sh          $t4, 0x8($v0)
    MEM_H(0X8, ctx->r2) = ctx->r12;
L_801CAF40:
    // 0x801CAF40: lw          $t5, 0x78C4($t5)
    ctx->r13 = MEM_W(ctx->r13, 0X78C4);
    // 0x801CAF44: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CAF48: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x801CAF4C: beq         $t5, $zero, L_801CAFE8
    if (ctx->r13 == 0) {
        // 0x801CAF50: addiu       $a0, $v1, 0x58
        ctx->r4 = ADD32(ctx->r3, 0X58);
            goto L_801CAFE8;
    }
    // 0x801CAF50: addiu       $a0, $v1, 0x58
    ctx->r4 = ADD32(ctx->r3, 0X58);
    // 0x801CAF54: sw          $zero, 0x78C4($at)
    MEM_W(0X78C4, ctx->r1) = 0;
    // 0x801CAF58: lwc1        $f4, 0x50($a2)
    ctx->f4.u32l = MEM_W(ctx->r6, 0X50);
    // 0x801CAF5C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801CAF60: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801CAF64: swc1        $f4, 0x14($v0)
    MEM_W(0X14, ctx->r2) = ctx->f4.u32l;
    // 0x801CAF68: lwc1        $f6, 0x54($a2)
    ctx->f6.u32l = MEM_W(ctx->r6, 0X54);
    // 0x801CAF6C: addiu       $t9, $t9, -0x51F8
    ctx->r25 = ADD32(ctx->r25, -0X51F8);
    // 0x801CAF70: addiu       $a1, $zero, 0x4
    ctx->r5 = ADD32(0, 0X4);
    // 0x801CAF74: swc1        $f6, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->f6.u32l;
    // 0x801CAF78: lwc1        $f8, 0x58($a2)
    ctx->f8.u32l = MEM_W(ctx->r6, 0X58);
    // 0x801CAF7C: swc1        $f0, 0x2C($v0)
    MEM_W(0X2C, ctx->r2) = ctx->f0.u32l;
    // 0x801CAF80: swc1        $f0, 0x30($v0)
    MEM_W(0X30, ctx->r2) = ctx->f0.u32l;
    // 0x801CAF84: swc1        $f0, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->f0.u32l;
    // 0x801CAF88: swc1        $f8, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->f8.u32l;
    // 0x801CAF8C: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x801CAF90: jalr        $t9
    // 0x801CAF94: sw          $v1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CAF94: sw          $v1, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r3;
    after_1:
    // 0x801CAF98: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CAF9C: addiu       $t9, $t9, -0x8C4
    ctx->r25 = ADD32(ctx->r25, -0X8C4);
    // 0x801CAFA0: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CAFA4: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CAFA8: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CAFAC: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CAFB0: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x801CAFB4: jalr        $t9
    // 0x801CAFB8: addiu       $a2, $zero, 0x4D
    ctx->r6 = ADD32(0, 0X4D);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CAFB8: addiu       $a2, $zero, 0x4D
    ctx->r6 = ADD32(0, 0X4D);
    after_2:
    // 0x801CAFBC: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x801CAFC0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CAFC4: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801CAFC8: lbu         $t6, 0x0($a3)
    ctx->r14 = MEM_BU(ctx->r7, 0X0);
    // 0x801CAFCC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CAFD0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CAFD4: andi        $t7, $t6, 0xFFFD
    ctx->r15 = ctx->r14 & 0XFFFD;
    // 0x801CAFD8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x801CAFDC: jalr        $t9
    // 0x801CAFE0: sb          $t7, 0x0($a3)
    MEM_B(0X0, ctx->r7) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CAFE0: sb          $t7, 0x0($a3)
    MEM_B(0X0, ctx->r7) = ctx->r15;
    after_3:
    // 0x801CAFE4: lw          $v1, 0x44($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X44);
L_801CAFE8:
    // 0x801CAFE8: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x801CAFEC: lh          $a0, 0xEE($v0)
    ctx->r4 = MEM_H(ctx->r2, 0XEE);
    // 0x801CAFF0: sw          $zero, 0x4($v0)
    MEM_W(0X4, ctx->r2) = 0;
    // 0x801CAFF4: beq         $a0, $zero, L_801CB014
    if (ctx->r4 == 0) {
        // 0x801CAFF8: addiu       $t8, $a0, -0x1
        ctx->r24 = ADD32(ctx->r4, -0X1);
            goto L_801CB014;
    }
    // 0x801CAFF8: addiu       $t8, $a0, -0x1
    ctx->r24 = ADD32(ctx->r4, -0X1);
    // 0x801CAFFC: sh          $t8, 0xEE($v0)
    MEM_H(0XEE, ctx->r2) = ctx->r24;
    // 0x801CB000: lw          $t0, 0x0($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X0);
    // 0x801CB004: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x801CB008: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x801CB00C: b           L_801CB028
    // 0x801CB010: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
        goto L_801CB028;
    // 0x801CB010: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
L_801CB014:
    // 0x801CB014: lw          $t2, 0x0($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X0);
    // 0x801CB018: lui         $at, 0xDFFF
    ctx->r1 = S32(0XDFFF << 16);
    // 0x801CB01C: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801CB020: and         $t3, $t2, $at
    ctx->r11 = ctx->r10 & ctx->r1;
    // 0x801CB024: sw          $t3, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r11;
L_801CB028:
    // 0x801CB028: lh          $a0, 0xEC($v0)
    ctx->r4 = MEM_H(ctx->r2, 0XEC);
    // 0x801CB02C: beq         $a0, $zero, L_801CB04C
    if (ctx->r4 == 0) {
        // 0x801CB030: addiu       $t4, $a0, -0x1
        ctx->r12 = ADD32(ctx->r4, -0X1);
            goto L_801CB04C;
    }
    // 0x801CB030: addiu       $t4, $a0, -0x1
    ctx->r12 = ADD32(ctx->r4, -0X1);
    // 0x801CB034: sh          $t4, 0xEC($v0)
    MEM_H(0XEC, ctx->r2) = ctx->r12;
    // 0x801CB038: lw          $t5, 0x0($v1)
    ctx->r13 = MEM_W(ctx->r3, 0X0);
    // 0x801CB03C: lui         $at, 0x6000
    ctx->r1 = S32(0X6000 << 16);
    // 0x801CB040: or          $t6, $t5, $at
    ctx->r14 = ctx->r13 | ctx->r1;
    // 0x801CB044: b           L_801CB060
    // 0x801CB048: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
        goto L_801CB060;
    // 0x801CB048: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
L_801CB04C:
    // 0x801CB04C: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x801CB050: lui         $at, 0xBFFF
    ctx->r1 = S32(0XBFFF << 16);
    // 0x801CB054: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801CB058: and         $t9, $t7, $at
    ctx->r25 = ctx->r15 & ctx->r1;
    // 0x801CB05C: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
L_801CB060:
    // 0x801CB060: lh          $v1, 0xF0($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XF0);
    // 0x801CB064: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CB068: beq         $v1, $zero, L_801CB074
    if (ctx->r3 == 0) {
        // 0x801CB06C: addiu       $t8, $v1, -0x1
        ctx->r24 = ADD32(ctx->r3, -0X1);
            goto L_801CB074;
    }
    // 0x801CB06C: addiu       $t8, $v1, -0x1
    ctx->r24 = ADD32(ctx->r3, -0X1);
    // 0x801CB070: sh          $t8, 0xF0($v0)
    MEM_H(0XF0, ctx->r2) = ctx->r24;
L_801CB074:
    // 0x801CB074: lh          $v1, 0xF4($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XF4);
    // 0x801CB078: beq         $v1, $zero, L_801CB084
    if (ctx->r3 == 0) {
        // 0x801CB07C: addiu       $t0, $v1, -0x1
        ctx->r8 = ADD32(ctx->r3, -0X1);
            goto L_801CB084;
    }
    // 0x801CB07C: addiu       $t0, $v1, -0x1
    ctx->r8 = ADD32(ctx->r3, -0X1);
    // 0x801CB080: sh          $t0, 0xF4($v0)
    MEM_H(0XF4, ctx->r2) = ctx->r8;
L_801CB084:
    // 0x801CB084: lh          $v1, 0xF6($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XF6);
    // 0x801CB088: beq         $v1, $zero, L_801CB094
    if (ctx->r3 == 0) {
        // 0x801CB08C: addiu       $t1, $v1, -0x1
        ctx->r9 = ADD32(ctx->r3, -0X1);
            goto L_801CB094;
    }
    // 0x801CB08C: addiu       $t1, $v1, -0x1
    ctx->r9 = ADD32(ctx->r3, -0X1);
    // 0x801CB090: sh          $t1, 0xF6($v0)
    MEM_H(0XF6, ctx->r2) = ctx->r9;
L_801CB094:
    // 0x801CB094: jal         0x801CDED4
    // 0x801CB098: lw          $a0, 0x3F84($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X3F84);
    overlay_system_1_func_801CDED4(rdram, ctx);
        goto after_4;
    // 0x801CB098: lw          $a0, 0x3F84($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X3F84);
    after_4:
    // 0x801CB09C: jal         0x80027B08
    // 0x801CB0A0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80027B08(rdram, ctx);
        goto after_5;
    // 0x801CB0A0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x801CB0A4: beq         $v0, $zero, L_801CB0C8
    if (ctx->r2 == 0) {
        // 0x801CB0A8: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_801CB0C8;
    }
    // 0x801CB0A8: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CB0AC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB0B0: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801CB0B4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CB0B8: jalr        $t9
    // 0x801CB0BC: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x801CB0BC: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_6:
    // 0x801CB0C0: b           L_801CB0D0
    // 0x801CB0C4: nop

        goto L_801CB0D0;
    // 0x801CB0C4: nop

L_801CB0C8:
    // 0x801CB0C8: jal         0x8002A998
    // 0x801CB0CC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8002A998(rdram, ctx);
        goto after_7;
    // 0x801CB0CC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_7:
L_801CB0D0:
    // 0x801CB0D0: jal         0x8002BB60
    // 0x801CB0D4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8002BB60(rdram, ctx);
        goto after_8;
    // 0x801CB0D4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_8:
    // 0x801CB0D8: beq         $v0, $zero, L_801CB0F4
    if (ctx->r2 == 0) {
        // 0x801CB0DC: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_801CB0F4;
    }
    // 0x801CB0DC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CB0E0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB0E4: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801CB0E8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CB0EC: jalr        $t9
    // 0x801CB0F0: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x801CB0F0: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_9:
L_801CB0F4:
    // 0x801CB0F4: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
L_801CB0F8:
    // 0x801CB0F8: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CB0FC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CB100: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801CB104: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801CB108: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801CB10C: sll         $t2, $v1, 1
    ctx->r10 = S32(ctx->r3 << 1);
    // 0x801CB110: addu        $v0, $s0, $t2
    ctx->r2 = ADD32(ctx->r16, ctx->r10);
    // 0x801CB114: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x801CB118: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801CB11C: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801CB120: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801CB124: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801CB128: sb          $t4, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r12;
    // 0x801CB12C: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801CB130: lw          $t9, 0x7890($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7890);
    // 0x801CB134: jalr        $t9
    // 0x801CB138: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x801CB138: nop

    after_10:
    // 0x801CB13C: lh          $t7, 0xE($s0)
    ctx->r15 = MEM_H(ctx->r16, 0XE);
    // 0x801CB140: lhu         $t0, 0x2($s0)
    ctx->r8 = MEM_HU(ctx->r16, 0X2);
    // 0x801CB144: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x801CB148: andi        $t1, $t0, 0x1000
    ctx->r9 = ctx->r8 & 0X1000;
    // 0x801CB14C: beq         $t1, $zero, L_801CB174
    if (ctx->r9 == 0) {
        // 0x801CB150: sh          $t8, 0xE($s0)
        MEM_H(0XE, ctx->r16) = ctx->r24;
            goto L_801CB174;
    }
    // 0x801CB150: sh          $t8, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r24;
    // 0x801CB154: jal         0x80030A18
    // 0x801CB158: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80030A18(rdram, ctx);
        goto after_11;
    // 0x801CB158: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_11:
    // 0x801CB15C: jal         0x80021BAC
    // 0x801CB160: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80021BAC(rdram, ctx);
        goto after_12;
    // 0x801CB160: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_12:
    // 0x801CB164: jal         0x80020DDC
    // 0x801CB168: nop

    func_80020DDC(rdram, ctx);
        goto after_13;
    // 0x801CB168: nop

    after_13:
    // 0x801CB16C: jal         0x80021898
    // 0x801CB170: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80021898(rdram, ctx);
        goto after_14;
    // 0x801CB170: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_14:
L_801CB174:
    // 0x801CB174: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CB178:
    // 0x801CB178: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CB17C: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x801CB180: jr          $ra
    // 0x801CB184: nop

    return;
    // 0x801CB184: nop

    // 0x801CB188: nop

    // 0x801CB18C: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CB190(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CB190: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CB194: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CB198: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x801CB19C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801CB1A0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801CB1A4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801CB1A8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x801CB1AC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x801CB1B0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801CB1B4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801CB1B8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801CB1BC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801CB1C0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801CB1C4: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801CB1C8: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801CB1CC: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CB1D0: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801CB1D4: lw          $t9, 0x7B24($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7B24);
    // 0x801CB1D8: jalr        $t9
    // 0x801CB1DC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CB1DC: nop

    after_0:
    // 0x801CB1E0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801CB1E4: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801CB1E8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801CB1EC: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801CB1F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CB1F4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CB1F8: jr          $ra
    // 0x801CB1FC: nop

    return;
    // 0x801CB1FC: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CB200(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CB200: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x801CB204: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x801CB208: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x801CB20C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x801CB210: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CB214: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x801CB218: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x801CB21C: addiu       $t9, $t9, 0x524
    ctx->r25 = ADD32(ctx->r25, 0X524);
    // 0x801CB220: sw          $t6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r14;
    // 0x801CB224: lw          $s2, 0x24($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X24);
    // 0x801CB228: jalr        $t9
    // 0x801CB22C: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CB22C: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    after_0:
    // 0x801CB230: bne         $v0, $zero, L_801CB240
    if (ctx->r2 != 0) {
        // 0x801CB234: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_801CB240;
    }
    // 0x801CB234: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x801CB238: b           L_801CB3FC
    // 0x801CB23C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CB3FC;
    // 0x801CB23C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801CB240:
    // 0x801CB240: lw          $t7, 0x4C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X4C);
    // 0x801CB244: lui         $v0, 0x8004
    ctx->r2 = S32(0X8004 << 16);
    // 0x801CB248: addiu       $v0, $v0, 0x628
    ctx->r2 = ADD32(ctx->r2, 0X628);
    // 0x801CB24C: sw          $s1, 0x4($t7)
    MEM_W(0X4, ctx->r15) = ctx->r17;
    // 0x801CB250: lhu         $t8, 0xA($s1)
    ctx->r24 = MEM_HU(ctx->r17, 0XA);
    // 0x801CB254: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801CB258: andi        $t2, $t8, 0xF9FF
    ctx->r10 = ctx->r24 & 0XF9FF;
    // 0x801CB25C: sh          $t2, 0xA($s1)
    MEM_H(0XA, ctx->r17) = ctx->r10;
    // 0x801CB260: andi        $t4, $t2, 0xFFFC
    ctx->r12 = ctx->r10 & 0XFFFC;
    // 0x801CB264: sh          $t2, 0xA($s1)
    MEM_H(0XA, ctx->r17) = ctx->r10;
    // 0x801CB268: sh          $t4, 0xA($s1)
    MEM_H(0XA, ctx->r17) = ctx->r12;
    // 0x801CB26C: ori         $t5, $t4, 0x1
    ctx->r13 = ctx->r12 | 0X1;
    // 0x801CB270: sh          $t5, 0xA($s1)
    MEM_H(0XA, ctx->r17) = ctx->r13;
    // 0x801CB274: lw          $t6, 0x14($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X14);
    // 0x801CB278: lw          $a0, 0x14($t6)
    ctx->r4 = MEM_W(ctx->r14, 0X14);
    // 0x801CB27C: jalr        $v0
    // 0x801CB280: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_1;
    // 0x801CB280: sw          $v0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r2;
    after_1:
    // 0x801CB284: bne         $v0, $zero, L_801CB294
    if (ctx->r2 != 0) {
        // 0x801CB288: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_801CB294;
    }
    // 0x801CB288: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x801CB28C: b           L_801CB3FC
    // 0x801CB290: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CB3FC;
    // 0x801CB290: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801CB294:
    // 0x801CB294: lui         $v0, 0x8004
    ctx->r2 = S32(0X8004 << 16);
    // 0x801CB298: addiu       $v0, $v0, 0x844
    ctx->r2 = ADD32(ctx->r2, 0X844);
    // 0x801CB29C: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    // 0x801CB2A0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CB2A4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801CB2A8: jalr        $v0
    // 0x801CB2AC: addiu       $a2, $zero, 0xE
    ctx->r6 = ADD32(0, 0XE);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_2;
    // 0x801CB2AC: addiu       $a2, $zero, 0xE
    ctx->r6 = ADD32(0, 0XE);
    after_2:
    // 0x801CB2B0: lui         $at, 0xC120
    ctx->r1 = S32(0XC120 << 16);
    // 0x801CB2B4: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801CB2B8: lui         $at, 0x4188
    ctx->r1 = S32(0X4188 << 16);
    // 0x801CB2BC: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801CB2C0: lh          $t9, 0x0($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X0);
    // 0x801CB2C4: lui         $at, 0x4234
    ctx->r1 = S32(0X4234 << 16);
    // 0x801CB2C8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801CB2CC: addiu       $t8, $zero, -0x4000
    ctx->r24 = ADD32(0, -0X4000);
    // 0x801CB2D0: andi        $t7, $t9, 0xFBFF
    ctx->r15 = ctx->r25 & 0XFBFF;
    // 0x801CB2D4: sh          $t7, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r15;
    // 0x801CB2D8: sh          $t8, 0x16($s0)
    MEM_H(0X16, ctx->r16) = ctx->r24;
    // 0x801CB2DC: swc1        $f4, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f4.u32l;
    // 0x801CB2E0: swc1        $f6, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f6.u32l;
    // 0x801CB2E4: swc1        $f8, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f8.u32l;
    // 0x801CB2E8: lw          $t0, 0x14($s2)
    ctx->r8 = MEM_W(ctx->r18, 0X14);
    // 0x801CB2EC: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x801CB2F0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801CB2F4: lw          $t1, 0x14($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X14);
    // 0x801CB2F8: jalr        $t9
    // 0x801CB2FC: lw          $a0, 0x14($t1)
    ctx->r4 = MEM_W(ctx->r9, 0X14);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CB2FC: lw          $a0, 0x14($t1)
    ctx->r4 = MEM_W(ctx->r9, 0X14);
    after_3:
    // 0x801CB300: bne         $v0, $zero, L_801CB310
    if (ctx->r2 != 0) {
        // 0x801CB304: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_801CB310;
    }
    // 0x801CB304: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x801CB308: b           L_801CB3FC
    // 0x801CB30C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CB3FC;
    // 0x801CB30C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801CB310:
    // 0x801CB310: lw          $t9, 0x2C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X2C);
    // 0x801CB314: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CB318: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801CB31C: jalr        $t9
    // 0x801CB320: addiu       $a2, $zero, 0xF
    ctx->r6 = ADD32(0, 0XF);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CB320: addiu       $a2, $zero, 0xF
    ctx->r6 = ADD32(0, 0XF);
    after_4:
    // 0x801CB324: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x801CB328: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801CB32C: lhu         $t7, 0xA($s0)
    ctx->r15 = MEM_HU(ctx->r16, 0XA);
    // 0x801CB330: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x801CB334: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x801CB338: lh          $t2, 0x0($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X0);
    // 0x801CB33C: lui         $at, 0x41D0
    ctx->r1 = S32(0X41D0 << 16);
    // 0x801CB340: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801CB344: andi        $t0, $t7, 0x8FFF
    ctx->r8 = ctx->r15 & 0X8FFF;
    // 0x801CB348: addiu       $t4, $zero, -0x8000
    ctx->r12 = ADD32(0, -0X8000);
    // 0x801CB34C: addiu       $t5, $zero, 0x2
    ctx->r13 = ADD32(0, 0X2);
    // 0x801CB350: addiu       $t6, $zero, 0x258
    ctx->r14 = ADD32(0, 0X258);
    // 0x801CB354: sh          $t0, 0xA($s0)
    MEM_H(0XA, ctx->r16) = ctx->r8;
    // 0x801CB358: ori         $t1, $t0, 0x4000
    ctx->r9 = ctx->r8 | 0X4000;
    // 0x801CB35C: andi        $t3, $t2, 0xFAFF
    ctx->r11 = ctx->r10 & 0XFAFF;
    // 0x801CB360: sh          $t3, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r11;
    // 0x801CB364: sh          $t4, 0x16($s0)
    MEM_H(0X16, ctx->r16) = ctx->r12;
    // 0x801CB368: sh          $t5, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r13;
    // 0x801CB36C: sh          $t6, 0x6($s0)
    MEM_H(0X6, ctx->r16) = ctx->r14;
    // 0x801CB370: sh          $t1, 0xA($s0)
    MEM_H(0XA, ctx->r16) = ctx->r9;
    // 0x801CB374: swc1        $f10, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f10.u32l;
    // 0x801CB378: swc1        $f16, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f16.u32l;
    // 0x801CB37C: swc1        $f18, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f18.u32l;
    // 0x801CB380: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x801CB384: lw          $a0, 0x14($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X14);
    // 0x801CB388: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801CB38C: jalr        $t9
    // 0x801CB390: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x801CB390: nop

    after_5:
    // 0x801CB394: bne         $v0, $zero, L_801CB3A4
    if (ctx->r2 != 0) {
        // 0x801CB398: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_801CB3A4;
    }
    // 0x801CB398: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x801CB39C: b           L_801CB3FC
    // 0x801CB3A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CB3FC;
    // 0x801CB3A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801CB3A4:
    // 0x801CB3A4: lw          $t9, 0x2C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X2C);
    // 0x801CB3A8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CB3AC: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x801CB3B0: jalr        $t9
    // 0x801CB3B4: addiu       $a2, $zero, 0xE
    ctx->r6 = ADD32(0, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x801CB3B4: addiu       $a2, $zero, 0xE
    ctx->r6 = ADD32(0, 0XE);
    after_6:
    // 0x801CB3B8: lui         $at, 0x41A0
    ctx->r1 = S32(0X41A0 << 16);
    // 0x801CB3BC: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801CB3C0: lh          $t2, 0x0($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X0);
    // 0x801CB3C4: lui         $at, 0x42A0
    ctx->r1 = S32(0X42A0 << 16);
    // 0x801CB3C8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801CB3CC: addiu       $t4, $zero, -0x4000
    ctx->r12 = ADD32(0, -0X4000);
    // 0x801CB3D0: andi        $t3, $t2, 0xFBFF
    ctx->r11 = ctx->r10 & 0XFBFF;
    // 0x801CB3D4: sh          $t3, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r11;
    // 0x801CB3D8: sh          $t4, 0x16($s0)
    MEM_H(0X16, ctx->r16) = ctx->r12;
    // 0x801CB3DC: swc1        $f4, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f4.u32l;
    // 0x801CB3E0: swc1        $f6, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f6.u32l;
    // 0x801CB3E4: lw          $v1, 0x4C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X4C);
    // 0x801CB3E8: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x801CB3EC: lhu         $t5, 0x10($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0X10);
    // 0x801CB3F0: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x801CB3F4: ori         $t6, $t5, 0xE000
    ctx->r14 = ctx->r13 | 0XE000;
    // 0x801CB3F8: sh          $t6, 0x8($v1)
    MEM_H(0X8, ctx->r3) = ctx->r14;
L_801CB3FC:
    // 0x801CB3FC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x801CB400: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CB404: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x801CB408: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x801CB40C: jr          $ra
    // 0x801CB410: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x801CB410: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void overlay_system_1_func_801CB414(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CB414: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801CB418: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x801CB41C: lui         $a0, 0xFF
    ctx->r4 = S32(0XFF << 16);
    // 0x801CB420: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CB424: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801CB428: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CB42C: addiu       $a1, $a1, 0x7B48
    ctx->r5 = ADD32(ctx->r5, 0X7B48);
    // 0x801CB430: addiu       $v0, $v0, 0x7B38
    ctx->r2 = ADD32(ctx->r2, 0X7B38);
    // 0x801CB434: ori         $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 | 0XFFFF;
    // 0x801CB438: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801CB43C: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
L_801CB440:
    // 0x801CB440: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x801CB444: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CB448: and         $t7, $t6, $a0
    ctx->r15 = ctx->r14 & ctx->r4;
    // 0x801CB44C: and         $t9, $t8, $a0
    ctx->r25 = ctx->r24 & ctx->r4;
    // 0x801CB450: subu        $v1, $t7, $t9
    ctx->r3 = SUB32(ctx->r15, ctx->r25);
    // 0x801CB454: slt         $at, $a2, $v1
    ctx->r1 = SIGNED(ctx->r6) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x801CB458: beq         $at, $zero, L_801CB464
    if (ctx->r1 == 0) {
        // 0x801CB45C: nop

            goto L_801CB464;
    }
    // 0x801CB45C: nop

    // 0x801CB460: or          $a2, $v1, $zero
    ctx->r6 = ctx->r3 | 0;
L_801CB464:
    // 0x801CB464: bnel        $v0, $a1, L_801CB440
    if (ctx->r2 != ctx->r5) {
        // 0x801CB468: lw          $t6, 0x4($v0)
        ctx->r14 = MEM_W(ctx->r2, 0X4);
            goto L_801CB440;
    }
    goto skip_0;
    // 0x801CB468: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    skip_0:
    // 0x801CB46C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB470: addiu       $t9, $t9, 0x27B0
    ctx->r25 = ADD32(ctx->r25, 0X27B0);
    // 0x801CB474: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x801CB478: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801CB47C: jalr        $t9
    // 0x801CB480: addiu       $a3, $zero, 0x8
    ctx->r7 = ADD32(0, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CB480: addiu       $a3, $zero, 0x8
    ctx->r7 = ADD32(0, 0X8);
    after_0:
    // 0x801CB484: jal         0x801CBE54
    // 0x801CB488: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    overlay_system_1_func_801CBE54(rdram, ctx);
        goto after_1;
    // 0x801CB488: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x801CB48C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x801CB490: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CB494: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x801CB498: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x801CB49C: addiu       $a3, $a3, -0x5260
    ctx->r7 = ADD32(ctx->r7, -0X5260);
    // 0x801CB4A0: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    // 0x801CB4A4: jal         0x80011998
    // 0x801CB4A8: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    func_80011998(rdram, ctx);
        goto after_2;
    // 0x801CB4A8: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_2:
    // 0x801CB4AC: jal         0x80011364
    // 0x801CB4B0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_80011364(rdram, ctx);
        goto after_3;
    // 0x801CB4B0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_3:
    // 0x801CB4B4: lw          $v1, 0x30($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X30);
    // 0x801CB4B8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB4BC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CB4C0: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x801CB4C4: jalr        $t9
    // 0x801CB4C8: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CB4C8: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    after_4:
    // 0x801CB4CC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CB4D0: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801CB4D4: jr          $ra
    // 0x801CB4D8: nop

    return;
    // 0x801CB4D8: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CB4DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CB4DC: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CB4E0: lw          $t6, -0x5260($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5260);
    // 0x801CB4E4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CB4E8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CB4EC: beql        $t6, $zero, L_801CB554
    if (ctx->r14 == 0) {
        // 0x801CB4F0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801CB554;
    }
    goto skip_0;
    // 0x801CB4F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801CB4F4: jal         0x80020960
    // 0x801CB4F8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80020960(rdram, ctx);
        goto after_0;
    // 0x801CB4F8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x801CB4FC: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x801CB500: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB504: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CB508: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801CB50C: jalr        $t9
    // 0x801CB510: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CB510: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
    // 0x801CB514: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801CB518: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CB51C: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x801CB520: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x801CB524: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x801CB528: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x801CB52C: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x801CB530: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CB534: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801CB538: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x801CB53C: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x801CB540: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x801CB544: lw          $t9, 0x7B24($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7B24);
    // 0x801CB548: jalr        $t9
    // 0x801CB54C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CB54C: nop

    after_2:
    // 0x801CB550: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CB554:
    // 0x801CB554: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CB558: jr          $ra
    // 0x801CB55C: nop

    return;
    // 0x801CB55C: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CB560(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CB560: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CB564: lw          $t6, -0x5260($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5260);
    // 0x801CB568: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CB56C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CB570: beq         $t6, $zero, L_801CB5CC
    if (ctx->r14 == 0) {
        // 0x801CB574: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_801CB5CC;
    }
    // 0x801CB574: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801CB578: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB57C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CB580: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801CB584: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x801CB588: jalr        $t9
    // 0x801CB58C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CB58C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x801CB590: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801CB594: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CB598: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x801CB59C: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x801CB5A0: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x801CB5A4: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x801CB5A8: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x801CB5AC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CB5B0: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801CB5B4: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x801CB5B8: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x801CB5BC: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x801CB5C0: lw          $t9, 0x7B24($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7B24);
    // 0x801CB5C4: jalr        $t9
    // 0x801CB5C8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CB5C8: nop

    after_1:
L_801CB5CC:
    // 0x801CB5CC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CB5D0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CB5D4: jr          $ra
    // 0x801CB5D8: nop

    return;
    // 0x801CB5D8: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CB5DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CB5DC: addiu       $sp, $sp, -0x78
    ctx->r29 = ADD32(ctx->r29, -0X78);
    // 0x801CB5E0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB5E4: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x801CB5E8: sw          $s2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r18;
    // 0x801CB5EC: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x801CB5F0: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x801CB5F4: sw          $s1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r17;
    // 0x801CB5F8: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x801CB5FC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801CB600: addiu       $a2, $zero, 0x2E8
    ctx->r6 = ADD32(0, 0X2E8);
    // 0x801CB604: jalr        $t9
    // 0x801CB608: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CB608: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x801CB60C: bne         $v0, $zero, L_801CB62C
    if (ctx->r2 != 0) {
        // 0x801CB610: addiu       $t7, $zero, 0x1
        ctx->r15 = ADD32(0, 0X1);
            goto L_801CB62C;
    }
    // 0x801CB610: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x801CB614: lw          $t9, 0x10($s2)
    ctx->r25 = MEM_W(ctx->r18, 0X10);
    // 0x801CB618: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CB61C: jalr        $t9
    // 0x801CB620: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CB620: nop

    after_1:
    // 0x801CB624: b           L_801CBDD8
    // 0x801CB628: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
        goto L_801CBDD8;
    // 0x801CB628: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_801CB62C:
    // 0x801CB62C: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x801CB630: lw          $t6, 0x34($s2)
    ctx->r14 = MEM_W(ctx->r18, 0X34);
    // 0x801CB634: lw          $t8, 0x78C0($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X78C0);
    // 0x801CB638: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CB63C: sh          $t7, -0x54CA($at)
    MEM_H(-0X54CA, ctx->r1) = ctx->r15;
    // 0x801CB640: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x801CB644: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x801CB648: sw          $t8, 0x3700($at)
    MEM_W(0X3700, ctx->r1) = ctx->r24;
    // 0x801CB64C: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x801CB650: addiu       $t4, $t4, -0x3484
    ctx->r12 = ADD32(ctx->r12, -0X3484);
    // 0x801CB654: sw          $t4, 0x3F80($at)
    MEM_W(0X3F80, ctx->r1) = ctx->r12;
    // 0x801CB658: lui         $t5, 0x801D
    ctx->r13 = S32(0X801D << 16);
    // 0x801CB65C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801CB660: addiu       $t5, $t5, 0x7AC4
    ctx->r13 = ADD32(ctx->r13, 0X7AC4);
    // 0x801CB664: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x801CB668: addiu       $t9, $t9, 0x34AC
    ctx->r25 = ADD32(ctx->r25, 0X34AC);
    // 0x801CB66C: sw          $t5, 0x4190($at)
    MEM_W(0X4190, ctx->r1) = ctx->r13;
    // 0x801CB670: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CB674: jalr        $t9
    // 0x801CB678: sw          $t6, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CB678: sw          $t6, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r14;
    after_2:
    // 0x801CB67C: lw          $s0, 0x70($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X70);
    // 0x801CB680: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CB684: addiu       $t9, $t9, 0x5D90
    ctx->r25 = ADD32(ctx->r25, 0X5D90);
    // 0x801CB688: sw          $v0, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->r2;
    // 0x801CB68C: lw          $a1, 0x24($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X24);
    // 0x801CB690: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x801CB694: addiu       $a2, $a2, 0x78D0
    ctx->r6 = ADD32(ctx->r6, 0X78D0);
    // 0x801CB698: sw          $v0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r2;
    // 0x801CB69C: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x801CB6A0: jalr        $t9
    // 0x801CB6A4: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CB6A4: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    after_3:
    // 0x801CB6A8: lui         $t6, 0x800B
    ctx->r14 = S32(0X800B << 16);
    // 0x801CB6AC: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x801CB6B0: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801CB6B4: addiu       $t6, $t6, 0x2F80
    ctx->r14 = ADD32(ctx->r14, 0X2F80);
    // 0x801CB6B8: sw          $v0, 0x24($s2)
    MEM_W(0X24, ctx->r18) = ctx->r2;
    // 0x801CB6BC: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x801CB6C0: lui         $v1, 0x8001
    ctx->r3 = S32(0X8001 << 16);
    // 0x801CB6C4: sw          $t7, 0x38($v0)
    MEM_W(0X38, ctx->r2) = ctx->r15;
    // 0x801CB6C8: addiu       $v1, $v1, 0x36C4
    ctx->r3 = ADD32(ctx->r3, 0X36C4);
    // 0x801CB6CC: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x801CB6D0: sw          $v1, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r3;
    // 0x801CB6D4: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x801CB6D8: lui         $a1, 0xC
    ctx->r5 = S32(0XC << 16);
    // 0x801CB6DC: jalr        $v1
    // 0x801CB6E0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_4;
    // 0x801CB6E0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_4:
    // 0x801CB6E4: lw          $v1, 0x38($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X38);
    // 0x801CB6E8: lui         $a1, 0x20
    ctx->r5 = S32(0X20 << 16);
    // 0x801CB6EC: ori         $a1, $a1, 0x1
    ctx->r5 = ctx->r5 | 0X1;
    // 0x801CB6F0: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x801CB6F4: jalr        $v1
    // 0x801CB6F8: addiu       $a2, $s1, 0x50
    ctx->r6 = ADD32(ctx->r17, 0X50);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_5;
    // 0x801CB6F8: addiu       $a2, $s1, 0x50
    ctx->r6 = ADD32(ctx->r17, 0X50);
    after_5:
    // 0x801CB6FC: lw          $t8, 0x14($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X14);
    // 0x801CB700: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CB704: lw          $a0, 0x14($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X14);
    // 0x801CB708: lhu         $t4, 0x2($a0)
    ctx->r12 = MEM_HU(ctx->r4, 0X2);
    // 0x801CB70C: ori         $t5, $t4, 0x2
    ctx->r13 = ctx->r12 | 0X2;
    // 0x801CB710: sh          $t5, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r13;
    // 0x801CB714: lw          $t9, 0x14($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X14);
    // 0x801CB718: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CB71C: lw          $t6, 0x14($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X14);
    // 0x801CB720: lw          $a1, 0x14($t6)
    ctx->r5 = MEM_W(ctx->r14, 0X14);
    // 0x801CB724: lhu         $t7, 0x2($a1)
    ctx->r15 = MEM_HU(ctx->r5, 0X2);
    // 0x801CB728: ori         $t8, $t7, 0x2
    ctx->r24 = ctx->r15 | 0X2;
    // 0x801CB72C: sh          $t8, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r24;
    // 0x801CB730: lw          $t4, 0x14($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X14);
    // 0x801CB734: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CB738: lw          $t9, 0x14($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X14);
    // 0x801CB73C: lw          $a2, 0x10($t9)
    ctx->r6 = MEM_W(ctx->r25, 0X10);
    // 0x801CB740: lhu         $t6, 0x2($a2)
    ctx->r14 = MEM_HU(ctx->r6, 0X2);
    // 0x801CB744: ori         $t7, $t6, 0x2
    ctx->r15 = ctx->r14 | 0X2;
    // 0x801CB748: sh          $t7, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r15;
    // 0x801CB74C: lw          $t8, 0x14($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X14);
    // 0x801CB750: lw          $t4, 0x14($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X14);
    // 0x801CB754: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CB758: lw          $t9, 0x10($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X10);
    // 0x801CB75C: lw          $a3, 0x14($t9)
    ctx->r7 = MEM_W(ctx->r25, 0X14);
    // 0x801CB760: lhu         $t6, 0x2($a3)
    ctx->r14 = MEM_HU(ctx->r7, 0X2);
    // 0x801CB764: ori         $t7, $t6, 0x2
    ctx->r15 = ctx->r14 | 0X2;
    // 0x801CB768: sh          $t7, 0x2($a3)
    MEM_H(0X2, ctx->r7) = ctx->r15;
    // 0x801CB76C: lw          $t8, 0x14($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X14);
    // 0x801CB770: lw          $t4, 0x14($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X14);
    // 0x801CB774: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CB778: lw          $t9, 0x10($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X10);
    // 0x801CB77C: lw          $t6, 0x14($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X14);
    // 0x801CB780: lw          $t0, 0x14($t6)
    ctx->r8 = MEM_W(ctx->r14, 0X14);
    // 0x801CB784: lhu         $t7, 0x2($t0)
    ctx->r15 = MEM_HU(ctx->r8, 0X2);
    // 0x801CB788: ori         $t8, $t7, 0x2
    ctx->r24 = ctx->r15 | 0X2;
    // 0x801CB78C: sh          $t8, 0x2($t0)
    MEM_H(0X2, ctx->r8) = ctx->r24;
    // 0x801CB790: lw          $t4, 0x14($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X14);
    // 0x801CB794: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CB798: lw          $t9, 0x14($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X14);
    // 0x801CB79C: lw          $t6, 0x10($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X10);
    // 0x801CB7A0: lw          $t1, 0x10($t6)
    ctx->r9 = MEM_W(ctx->r14, 0X10);
    // 0x801CB7A4: lhu         $t7, 0x2($t1)
    ctx->r15 = MEM_HU(ctx->r9, 0X2);
    // 0x801CB7A8: ori         $t8, $t7, 0x2
    ctx->r24 = ctx->r15 | 0X2;
    // 0x801CB7AC: sh          $t8, 0x2($t1)
    MEM_H(0X2, ctx->r9) = ctx->r24;
    // 0x801CB7B0: lw          $t4, 0x14($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X14);
    // 0x801CB7B4: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CB7B8: lw          $t9, 0x14($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X14);
    // 0x801CB7BC: lw          $t6, 0x10($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X10);
    // 0x801CB7C0: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x801CB7C4: lw          $t2, 0x14($t7)
    ctx->r10 = MEM_W(ctx->r15, 0X14);
    // 0x801CB7C8: lhu         $t8, 0x2($t2)
    ctx->r24 = MEM_HU(ctx->r10, 0X2);
    // 0x801CB7CC: ori         $t4, $t8, 0x2
    ctx->r12 = ctx->r24 | 0X2;
    // 0x801CB7D0: sh          $t4, 0x2($t2)
    MEM_H(0X2, ctx->r10) = ctx->r12;
    // 0x801CB7D4: lw          $t5, 0x14($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X14);
    // 0x801CB7D8: lw          $t9, 0x14($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X14);
    // 0x801CB7DC: lw          $t6, 0x14($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X14);
    // 0x801CB7E0: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x801CB7E4: lw          $t8, 0x10($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X10);
    // 0x801CB7E8: lw          $t4, 0x14($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X14);
    // 0x801CB7EC: lw          $t3, 0x14($t4)
    ctx->r11 = MEM_W(ctx->r12, 0X14);
    // 0x801CB7F0: lhu         $t5, 0x2($t3)
    ctx->r13 = MEM_HU(ctx->r11, 0X2);
    // 0x801CB7F4: ori         $t9, $t5, 0x2
    ctx->r25 = ctx->r13 | 0X2;
    // 0x801CB7F8: sh          $t9, 0x2($t3)
    MEM_H(0X2, ctx->r11) = ctx->r25;
    // 0x801CB7FC: lw          $t6, 0x14($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X14);
    // 0x801CB800: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801CB804: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801CB808: lw          $t4, 0x10($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X10);
    // 0x801CB80C: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CB810: lw          $t9, 0x14($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X14);
    // 0x801CB814: lw          $v1, 0x14($t9)
    ctx->r3 = MEM_W(ctx->r25, 0X14);
    // 0x801CB818: lh          $t6, 0x0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X0);
    // 0x801CB81C: ori         $t7, $t6, 0x8
    ctx->r15 = ctx->r14 | 0X8;
    // 0x801CB820: sh          $t7, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r15;
    // 0x801CB824: lw          $t8, 0x14($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X14);
    // 0x801CB828: lwc1        $f0, 0x7BA0($at)
    ctx->f0.u32l = MEM_W(ctx->r1, 0X7BA0);
    // 0x801CB82C: lw          $t4, 0x14($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X14);
    // 0x801CB830: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CB834: lw          $t9, 0x10($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X10);
    // 0x801CB838: lw          $t6, 0x10($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X10);
    // 0x801CB83C: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801CB840: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801CB844: lw          $v1, 0x14($t8)
    ctx->r3 = MEM_W(ctx->r24, 0X14);
    // 0x801CB848: lh          $t4, 0x0($v1)
    ctx->r12 = MEM_H(ctx->r3, 0X0);
    // 0x801CB84C: ori         $t5, $t4, 0x8
    ctx->r13 = ctx->r12 | 0X8;
    // 0x801CB850: sh          $t5, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r13;
    // 0x801CB854: lw          $t9, 0x14($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X14);
    // 0x801CB858: lw          $t6, 0x14($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X14);
    // 0x801CB85C: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x801CB860: lw          $t8, 0x10($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X10);
    // 0x801CB864: lw          $v1, 0x10($t8)
    ctx->r3 = MEM_W(ctx->r24, 0X10);
    // 0x801CB868: lh          $t4, 0x0($v1)
    ctx->r12 = MEM_H(ctx->r3, 0X0);
    // 0x801CB86C: swc1        $f0, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f0.u32l;
    // 0x801CB870: swc1        $f0, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f0.u32l;
    // 0x801CB874: ori         $t5, $t4, 0x8
    ctx->r13 = ctx->r12 | 0X8;
    // 0x801CB878: sh          $t5, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r13;
    // 0x801CB87C: swc1        $f0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f0.u32l;
    // 0x801CB880: lw          $t9, 0x14($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X14);
    // 0x801CB884: lw          $t6, 0x14($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X14);
    // 0x801CB888: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x801CB88C: lw          $t8, 0x10($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X10);
    // 0x801CB890: lw          $t4, 0x10($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X10);
    // 0x801CB894: lw          $v1, 0x10($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X10);
    // 0x801CB898: lh          $t5, 0x0($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X0);
    // 0x801CB89C: swc1        $f0, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f0.u32l;
    // 0x801CB8A0: swc1        $f0, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f0.u32l;
    // 0x801CB8A4: ori         $t9, $t5, 0x8
    ctx->r25 = ctx->r13 | 0X8;
    // 0x801CB8A8: sh          $t9, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r25;
    // 0x801CB8AC: swc1        $f0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f0.u32l;
    // 0x801CB8B0: lw          $t6, 0x14($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X14);
    // 0x801CB8B4: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801CB8B8: lw          $t8, 0x10($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X10);
    // 0x801CB8BC: lw          $t4, 0x10($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X10);
    // 0x801CB8C0: lw          $t5, 0x10($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X10);
    // 0x801CB8C4: lw          $t9, 0x10($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X10);
    // 0x801CB8C8: lw          $v1, 0x10($t9)
    ctx->r3 = MEM_W(ctx->r25, 0X10);
    // 0x801CB8CC: swc1        $f0, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f0.u32l;
    // 0x801CB8D0: lh          $t6, 0x0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X0);
    // 0x801CB8D4: lwc1        $f2, 0x70($v1)
    ctx->f2.u32l = MEM_W(ctx->r3, 0X70);
    // 0x801CB8D8: ori         $t7, $t6, 0x8
    ctx->r15 = ctx->r14 | 0X8;
    // 0x801CB8DC: sh          $t7, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r15;
    // 0x801CB8E0: swc1        $f2, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f2.u32l;
    // 0x801CB8E4: swc1        $f2, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f2.u32l;
    // 0x801CB8E8: lw          $t8, 0x14($s1)
    ctx->r24 = MEM_W(ctx->r17, 0X14);
    // 0x801CB8EC: lw          $t4, 0x14($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X14);
    // 0x801CB8F0: lw          $t5, 0x10($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X10);
    // 0x801CB8F4: lw          $t9, 0x10($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X10);
    // 0x801CB8F8: lw          $t6, 0x10($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X10);
    // 0x801CB8FC: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x801CB900: lw          $t8, 0x10($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X10);
    // 0x801CB904: lw          $v1, 0x10($t8)
    ctx->r3 = MEM_W(ctx->r24, 0X10);
    // 0x801CB908: lh          $t4, 0x0($v1)
    ctx->r12 = MEM_H(ctx->r3, 0X0);
    // 0x801CB90C: swc1        $f0, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f0.u32l;
    // 0x801CB910: swc1        $f0, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f0.u32l;
    // 0x801CB914: ori         $t5, $t4, 0x8
    ctx->r13 = ctx->r12 | 0X8;
    // 0x801CB918: sh          $t5, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r13;
    // 0x801CB91C: swc1        $f0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f0.u32l;
    // 0x801CB920: lw          $t9, 0x14($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X14);
    // 0x801CB924: lw          $t6, 0x14($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X14);
    // 0x801CB928: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801CB92C: lw          $t8, 0x10($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X10);
    // 0x801CB930: lw          $t4, 0x10($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X10);
    // 0x801CB934: lw          $v1, 0x10($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X10);
    // 0x801CB938: lh          $t5, 0x0($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X0);
    // 0x801CB93C: swc1        $f0, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f0.u32l;
    // 0x801CB940: swc1        $f0, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f0.u32l;
    // 0x801CB944: ori         $t9, $t5, 0x8
    ctx->r25 = ctx->r13 | 0X8;
    // 0x801CB948: sh          $t9, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r25;
    // 0x801CB94C: swc1        $f0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f0.u32l;
    // 0x801CB950: lw          $t6, 0x14($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X14);
    // 0x801CB954: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801CB958: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801CB95C: lw          $t4, 0x10($t8)
    ctx->r12 = MEM_W(ctx->r24, 0X10);
    // 0x801CB960: lw          $t5, 0x10($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X10);
    // 0x801CB964: lw          $t9, 0x10($t5)
    ctx->r25 = MEM_W(ctx->r13, 0X10);
    // 0x801CB968: lw          $v1, 0x10($t9)
    ctx->r3 = MEM_W(ctx->r25, 0X10);
    // 0x801CB96C: lh          $t6, 0x0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X0);
    // 0x801CB970: swc1        $f0, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f0.u32l;
    // 0x801CB974: swc1        $f0, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f0.u32l;
    // 0x801CB978: ori         $t7, $t6, 0x8
    ctx->r15 = ctx->r14 | 0X8;
    // 0x801CB97C: sh          $t7, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r15;
    // 0x801CB980: jal         0x801CC120
    // 0x801CB984: swc1        $f0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f0.u32l;
    overlay_system_1_func_801CC120(rdram, ctx);
        goto after_6;
    // 0x801CB984: swc1        $f0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f0.u32l;
    after_6:
    // 0x801CB988: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CB98C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801CB990: lui         $at, 0x3E80
    ctx->r1 = S32(0X3E80 << 16);
    // 0x801CB994: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x801CB998: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x801CB99C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CB9A0: ldc1        $f6, 0x7BA8($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, 0X7BA8);
    // 0x801CB9A4: cvt.d.s     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f0.d = CVT_D_S(ctx->f4.fl);
    // 0x801CB9A8: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x801CB9AC: div.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d);
    ctx->f8.d = DIV_D(ctx->f6.d, ctx->f0.d);
    // 0x801CB9B0: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x801CB9B4: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801CB9B8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CB9BC: lhu         $t8, 0xE($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0XE);
    // 0x801CB9C0: sub.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d);
    ctx->f16.d = ctx->f10.d - ctx->f0.d;
    // 0x801CB9C4: lui         $v0, 0x8000
    ctx->r2 = S32(0X8000 << 16);
    // 0x801CB9C8: ori         $t4, $t8, 0x110
    ctx->r12 = ctx->r24 | 0X110;
    // 0x801CB9CC: addiu       $v0, $v0, 0x2410
    ctx->r2 = ADD32(ctx->r2, 0X2410);
    // 0x801CB9D0: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CB9D4: addiu       $a1, $zero, 0x91
    ctx->r5 = ADD32(0, 0X91);
    // 0x801CB9D8: div.d       $f18, $f8, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f16.d);
    ctx->f18.d = DIV_D(ctx->f8.d, ctx->f16.d);
    // 0x801CB9DC: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d);
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x801CB9E0: swc1        $f4, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f4.u32l;
    // 0x801CB9E4: lwc1        $f6, 0x7BB0($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X7BB0);
    // 0x801CB9E8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CB9EC: swc1        $f6, 0x274($s0)
    MEM_W(0X274, ctx->r16) = ctx->f6.u32l;
    // 0x801CB9F0: lwc1        $f10, 0x7BB4($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X7BB4);
    // 0x801CB9F4: lui         $at, 0x40C0
    ctx->r1 = S32(0X40C0 << 16);
    // 0x801CB9F8: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801CB9FC: lui         $at, 0x4060
    ctx->r1 = S32(0X4060 << 16);
    // 0x801CBA00: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x801CBA04: swc1        $f10, 0x278($s0)
    MEM_W(0X278, ctx->r16) = ctx->f10.u32l;
    // 0x801CBA08: swc1        $f8, 0x27C($s0)
    MEM_W(0X27C, ctx->r16) = ctx->f8.u32l;
    // 0x801CBA0C: swc1        $f16, 0x280($s0)
    MEM_W(0X280, ctx->r16) = ctx->f16.u32l;
    // 0x801CBA10: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x801CBA14: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBA18: ldc1        $f18, 0x7BB8($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0X7BB8);
    // 0x801CBA1C: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801CBA20: lui         $at, 0x4034
    ctx->r1 = S32(0X4034 << 16);
    // 0x801CBA24: div.d       $f10, $f18, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f6.d);
    ctx->f10.d = DIV_D(ctx->f18.d, ctx->f6.d);
    // 0x801CBA28: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x801CBA2C: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x801CBA30: lui         $at, 0x4044
    ctx->r1 = S32(0X4044 << 16);
    // 0x801CBA34: cvt.s.d     $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d);
    ctx->f8.fl = CVT_S_D(ctx->f10.d);
    // 0x801CBA38: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x801CBA3C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBA40: swc1        $f8, 0x284($s0)
    MEM_W(0X284, ctx->r16) = ctx->f8.u32l;
    // 0x801CBA44: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x801CBA48: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x801CBA4C: cvt.d.s     $f18, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f18.d = CVT_D_S(ctx->f4.fl);
    // 0x801CBA50: div.d       $f6, $f16, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d);
    ctx->f6.d = DIV_D(ctx->f16.d, ctx->f18.d);
    // 0x801CBA54: cvt.s.d     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d);
    ctx->f10.fl = CVT_S_D(ctx->f6.d);
    // 0x801CBA58: swc1        $f10, 0x28C($s0)
    MEM_W(0X28C, ctx->r16) = ctx->f10.u32l;
    // 0x801CBA5C: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x801CBA60: cvt.d.s     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f16.d = CVT_D_S(ctx->f4.fl);
    // 0x801CBA64: div.d       $f18, $f8, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f16.d);
    ctx->f18.d = DIV_D(ctx->f8.d, ctx->f16.d);
    // 0x801CBA68: cvt.s.d     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d);
    ctx->f6.fl = CVT_S_D(ctx->f18.d);
    // 0x801CBA6C: swc1        $f6, 0x294($s0)
    MEM_W(0X294, ctx->r16) = ctx->f6.u32l;
    // 0x801CBA70: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x801CBA74: ldc1        $f10, 0x7BC0($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, 0X7BC0);
    // 0x801CBA78: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBA7C: cvt.d.s     $f8, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f8.d = CVT_D_S(ctx->f4.fl);
    // 0x801CBA80: div.d       $f16, $f10, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f8.d);
    ctx->f16.d = DIV_D(ctx->f10.d, ctx->f8.d);
    // 0x801CBA84: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d);
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x801CBA88: swc1        $f18, 0x29C($s0)
    MEM_W(0X29C, ctx->r16) = ctx->f18.u32l;
    // 0x801CBA8C: lwc1        $f6, 0x7BC8($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X7BC8);
    // 0x801CBA90: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBA94: swc1        $f2, 0x290($s0)
    MEM_W(0X290, ctx->r16) = ctx->f2.u32l;
    // 0x801CBA98: swc1        $f2, 0x298($s0)
    MEM_W(0X298, ctx->r16) = ctx->f2.u32l;
    // 0x801CBA9C: swc1        $f6, 0x288($s0)
    MEM_W(0X288, ctx->r16) = ctx->f6.u32l;
    // 0x801CBAA0: lwc1        $f4, 0x7BCC($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X7BCC);
    // 0x801CBAA4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBAA8: lwc1        $f12, 0x29C($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X29C);
    // 0x801CBAAC: swc1        $f4, 0x2A0($s0)
    MEM_W(0X2A0, ctx->r16) = ctx->f4.u32l;
    // 0x801CBAB0: lwc1        $f10, 0x7BD0($at)
    ctx->f10.u32l = MEM_W(ctx->r1, 0X7BD0);
    // 0x801CBAB4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBAB8: cvt.d.s     $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl);
    ctx->f4.d = CVT_D_S(ctx->f12.fl);
    // 0x801CBABC: swc1        $f10, 0x2A4($s0)
    MEM_W(0X2A4, ctx->r16) = ctx->f10.u32l;
    // 0x801CBAC0: lwc1        $f8, 0x7BD4($at)
    ctx->f8.u32l = MEM_W(ctx->r1, 0X7BD4);
    // 0x801CBAC4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBAC8: swc1        $f8, 0x2A8($s0)
    MEM_W(0X2A8, ctx->r16) = ctx->f8.u32l;
    // 0x801CBACC: lwc1        $f16, 0x7BD8($at)
    ctx->f16.u32l = MEM_W(ctx->r1, 0X7BD8);
    // 0x801CBAD0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBAD4: swc1        $f16, 0x2AC($s0)
    MEM_W(0X2AC, ctx->r16) = ctx->f16.u32l;
    // 0x801CBAD8: lwc1        $f18, 0x7BDC($at)
    ctx->f18.u32l = MEM_W(ctx->r1, 0X7BDC);
    // 0x801CBADC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBAE0: swc1        $f18, 0x2B0($s0)
    MEM_W(0X2B0, ctx->r16) = ctx->f18.u32l;
    // 0x801CBAE4: lwc1        $f6, 0x7BE0($at)
    ctx->f6.u32l = MEM_W(ctx->r1, 0X7BE0);
    // 0x801CBAE8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBAEC: swc1        $f2, 0x2B8($s0)
    MEM_W(0X2B8, ctx->r16) = ctx->f2.u32l;
    // 0x801CBAF0: swc1        $f12, 0x2BC($s0)
    MEM_W(0X2BC, ctx->r16) = ctx->f12.u32l;
    // 0x801CBAF4: swc1        $f6, 0x2B4($s0)
    MEM_W(0X2B4, ctx->r16) = ctx->f6.u32l;
    // 0x801CBAF8: ldc1        $f10, 0x7BE8($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, 0X7BE8);
    // 0x801CBAFC: lwc1        $f18, 0x294($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X294);
    // 0x801CBB00: sh          $t4, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r12;
    // 0x801CBB04: mul.d       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f10.d);
    ctx->f8.d = MUL_D(ctx->f4.d, ctx->f10.d);
    // 0x801CBB08: swc1        $f18, 0x2C4($s0)
    MEM_W(0X2C4, ctx->r16) = ctx->f18.u32l;
    // 0x801CBB0C: cvt.s.d     $f16, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d);
    ctx->f16.fl = CVT_S_D(ctx->f8.d);
    // 0x801CBB10: swc1        $f16, 0x2C0($s0)
    MEM_W(0X2C0, ctx->r16) = ctx->f16.u32l;
    // 0x801CBB14: jalr        $v0
    // 0x801CBB18: sw          $v0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_7;
    // 0x801CBB18: sw          $v0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r2;
    after_7:
    // 0x801CBB1C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CBB20: addiu       $a0, $a0, 0x3F84
    ctx->r4 = ADD32(ctx->r4, 0X3F84);
    // 0x801CBB24: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
    // 0x801CBB28: sb          $zero, 0x9($v0)
    MEM_B(0X9, ctx->r2) = 0;
    // 0x801CBB2C: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBB30: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBB34: addiu       $a1, $zero, 0x91
    ctx->r5 = ADD32(0, 0X91);
    // 0x801CBB38: jalr        $t9
    // 0x801CBB3C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x801CBB3C: nop

    after_8:
    // 0x801CBB40: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CBB44: addiu       $a0, $a0, 0x3FAC
    ctx->r4 = ADD32(ctx->r4, 0X3FAC);
    // 0x801CBB48: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
    // 0x801CBB4C: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x801CBB50: sb          $t5, 0x9($v0)
    MEM_B(0X9, ctx->r2) = ctx->r13;
    // 0x801CBB54: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBB58: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBB5C: addiu       $a1, $zero, 0x91
    ctx->r5 = ADD32(0, 0X91);
    // 0x801CBB60: jalr        $t9
    // 0x801CBB64: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x801CBB64: nop

    after_9:
    // 0x801CBB68: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CBB6C: addiu       $a0, $a0, 0x3FD4
    ctx->r4 = ADD32(ctx->r4, 0X3FD4);
    // 0x801CBB70: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
    // 0x801CBB74: addiu       $t6, $zero, 0x2
    ctx->r14 = ADD32(0, 0X2);
    // 0x801CBB78: sb          $t6, 0x9($v0)
    MEM_B(0X9, ctx->r2) = ctx->r14;
    // 0x801CBB7C: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBB80: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBB84: addiu       $a1, $zero, 0x92
    ctx->r5 = ADD32(0, 0X92);
    // 0x801CBB88: jalr        $t9
    // 0x801CBB8C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x801CBB8C: nop

    after_10:
    // 0x801CBB90: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBB94: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBB98: addiu       $a1, $zero, 0x9B
    ctx->r5 = ADD32(0, 0X9B);
    // 0x801CBB9C: jalr        $t9
    // 0x801CBBA0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x801CBBA0: nop

    after_11:
    // 0x801CBBA4: sw          $s1, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r17;
    // 0x801CBBA8: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBBAC: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBBB0: addiu       $a1, $zero, 0x9C
    ctx->r5 = ADD32(0, 0X9C);
    // 0x801CBBB4: jalr        $t9
    // 0x801CBBB8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x801CBBB8: nop

    after_12:
    // 0x801CBBBC: sw          $s1, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r17;
    // 0x801CBBC0: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBBC4: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBBC8: addiu       $a1, $zero, 0x9D
    ctx->r5 = ADD32(0, 0X9D);
    // 0x801CBBCC: jalr        $t9
    // 0x801CBBD0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_13;
    // 0x801CBBD0: nop

    after_13:
    // 0x801CBBD4: sw          $s1, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r17;
    // 0x801CBBD8: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBBDC: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBBE0: addiu       $a1, $zero, 0x9E
    ctx->r5 = ADD32(0, 0X9E);
    // 0x801CBBE4: jalr        $t9
    // 0x801CBBE8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_14;
    // 0x801CBBE8: nop

    after_14:
    // 0x801CBBEC: sw          $s1, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r17;
    // 0x801CBBF0: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBBF4: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBBF8: addiu       $a1, $zero, 0x9F
    ctx->r5 = ADD32(0, 0X9F);
    // 0x801CBBFC: jalr        $t9
    // 0x801CBC00: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_15;
    // 0x801CBC00: nop

    after_15:
    // 0x801CBC04: sw          $s1, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r17;
    // 0x801CBC08: lw          $t9, 0x38($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X38);
    // 0x801CBC0C: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBC10: addiu       $a1, $zero, 0xA0
    ctx->r5 = ADD32(0, 0XA0);
    // 0x801CBC14: jalr        $t9
    // 0x801CBC18: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_16;
    // 0x801CBC18: nop

    after_16:
    // 0x801CBC1C: lui         $t9, 0x8007
    ctx->r25 = S32(0X8007 << 16);
    // 0x801CBC20: sw          $s1, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r17;
    // 0x801CBC24: addiu       $t9, $t9, -0x7B84
    ctx->r25 = ADD32(ctx->r25, -0X7B84);
    // 0x801CBC28: lui         $a1, 0x3FB3
    ctx->r5 = S32(0X3FB3 << 16);
    // 0x801CBC2C: ori         $a1, $a1, 0x3333
    ctx->r5 = ctx->r5 | 0X3333;
    // 0x801CBC30: lw          $a0, 0x14($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X14);
    // 0x801CBC34: lui         $a2, 0x2
    ctx->r6 = S32(0X2 << 16);
    // 0x801CBC38: jalr        $t9
    // 0x801CBC3C: or          $a3, $s2, $zero
    ctx->r7 = ctx->r18 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_17;
    // 0x801CBC3C: or          $a3, $s2, $zero
    ctx->r7 = ctx->r18 | 0;
    after_17:
    // 0x801CBC40: beq         $v0, $zero, L_801CBC68
    if (ctx->r2 == 0) {
        // 0x801CBC44: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_801CBC68;
    }
    // 0x801CBC44: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x801CBC48: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801CBC4C: addiu       $t9, $t9, -0x2C7C
    ctx->r25 = ADD32(ctx->r25, -0X2C7C);
    // 0x801CBC50: jalr        $t9
    // 0x801CBC54: sw          $v0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_18;
    // 0x801CBC54: sw          $v0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r2;
    after_18:
    // 0x801CBC58: lw          $t7, 0x4($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X4);
    // 0x801CBC5C: lw          $a0, 0x4C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X4C);
    // 0x801CBC60: ori         $t8, $t7, 0x400
    ctx->r24 = ctx->r15 | 0X400;
    // 0x801CBC64: sw          $t8, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r24;
L_801CBC68:
    // 0x801CBC68: beq         $a0, $zero, L_801CBC8C
    if (ctx->r4 == 0) {
        // 0x801CBC6C: lui         $t9, 0x8006
        ctx->r25 = S32(0X8006 << 16);
            goto L_801CBC8C;
    }
    // 0x801CBC6C: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801CBC70: addiu       $t9, $t9, -0x2C7C
    ctx->r25 = ADD32(ctx->r25, -0X2C7C);
    // 0x801CBC74: jalr        $t9
    // 0x801CBC78: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_19;
    // 0x801CBC78: nop

    after_19:
    // 0x801CBC7C: lw          $t4, 0x4($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X4);
    // 0x801CBC80: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x801CBC84: or          $t5, $t4, $at
    ctx->r13 = ctx->r12 | ctx->r1;
    // 0x801CBC88: sw          $t5, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r13;
L_801CBC8C:
    // 0x801CBC8C: jal         0x801CB200
    // 0x801CBC90: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    overlay_system_1_func_801CB200(rdram, ctx);
        goto after_20;
    // 0x801CBC90: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    after_20:
    // 0x801CBC94: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    // 0x801CBC98: lui         $a3, 0x8002
    ctx->r7 = S32(0X8002 << 16);
    // 0x801CBC9C: addiu       $a3, $a3, -0xB18
    ctx->r7 = ADD32(ctx->r7, -0XB18);
    // 0x801CBCA0: addiu       $a0, $a0, 0x58
    ctx->r4 = ADD32(ctx->r4, 0X58);
    // 0x801CBCA4: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x801CBCA8: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x801CBCAC: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    // 0x801CBCB0: jal         0x8000ADAC
    // 0x801CBCB4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    func_8000ADAC(rdram, ctx);
        goto after_21;
    // 0x801CBCB4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_21:
    // 0x801CBCB8: lui         $v0, 0x8002
    ctx->r2 = S32(0X8002 << 16);
    // 0x801CBCBC: addiu       $v0, $v0, -0x8C4
    ctx->r2 = ADD32(ctx->r2, -0X8C4);
    // 0x801CBCC0: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CBCC4: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CBCC8: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CBCCC: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CBCD0: sw          $v0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r2;
    // 0x801CBCD4: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x801CBCD8: jalr        $v0
    // 0x801CBCDC: addiu       $a2, $zero, 0x4D
    ctx->r6 = ADD32(0, 0X4D);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_22;
    // 0x801CBCDC: addiu       $a2, $zero, 0x4D
    ctx->r6 = ADD32(0, 0X4D);
    after_22:
    // 0x801CBCE0: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x801CBCE4: jal         0x8000B9F8
    // 0x801CBCE8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8000B9F8(rdram, ctx);
        goto after_23;
    // 0x801CBCE8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_23:
    // 0x801CBCEC: addiu       $t6, $zero, 0x4
    ctx->r14 = ADD32(0, 0X4);
    // 0x801CBCF0: sh          $t6, 0x94($s0)
    MEM_H(0X94, ctx->r16) = ctx->r14;
    // 0x801CBCF4: addiu       $a0, $s0, 0x98
    ctx->r4 = ADD32(ctx->r16, 0X98);
    // 0x801CBCF8: lui         $a3, 0x8002
    ctx->r7 = S32(0X8002 << 16);
    // 0x801CBCFC: addiu       $t7, $zero, 0x2
    ctx->r15 = ADD32(0, 0X2);
    // 0x801CBD00: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    // 0x801CBD04: addiu       $a3, $a3, -0x960
    ctx->r7 = ADD32(ctx->r7, -0X960);
    // 0x801CBD08: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x801CBD0C: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x801CBD10: jal         0x8000ADAC
    // 0x801CBD14: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    func_8000ADAC(rdram, ctx);
        goto after_24;
    // 0x801CBD14: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_24:
    // 0x801CBD18: lw          $t9, 0x34($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X34);
    // 0x801CBD1C: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CBD20: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CBD24: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CBD28: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CBD2C: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x801CBD30: jalr        $t9
    // 0x801CBD34: addiu       $a2, $zero, 0x4D
    ctx->r6 = ADD32(0, 0X4D);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_25;
    // 0x801CBD34: addiu       $a2, $zero, 0x4D
    ctx->r6 = ADD32(0, 0X4D);
    after_25:
    // 0x801CBD38: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x801CBD3C: jal         0x8000B9F8
    // 0x801CBD40: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_8000B9F8(rdram, ctx);
        goto after_26;
    // 0x801CBD40: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_26:
    // 0x801CBD44: lhu         $t8, 0x2($s2)
    ctx->r24 = MEM_HU(ctx->r18, 0X2);
    // 0x801CBD48: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CBD4C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801CBD50: ori         $t4, $t8, 0x1000
    ctx->r12 = ctx->r24 | 0X1000;
    // 0x801CBD54: sh          $t4, 0x2($s2)
    MEM_H(0X2, ctx->r18) = ctx->r12;
    // 0x801CBD58: sw          $s2, 0x295C($v1)
    MEM_W(0X295C, ctx->r3) = ctx->r18;
    // 0x801CBD5C: lw          $t5, 0x24($s2)
    ctx->r13 = MEM_W(ctx->r18, 0X24);
    // 0x801CBD60: lw          $t6, 0x70($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X70);
    // 0x801CBD64: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBD68: sw          $t5, 0x2960($v1)
    MEM_W(0X2960, ctx->r3) = ctx->r13;
    // 0x801CBD6C: sw          $t6, 0x2964($v1)
    MEM_W(0X2964, ctx->r3) = ctx->r14;
    // 0x801CBD70: sw          $s0, -0x53D8($at)
    MEM_W(-0X53D8, ctx->r1) = ctx->r16;
    // 0x801CBD74: lw          $t7, 0x2908($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X2908);
    // 0x801CBD78: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x801CBD7C: addiu       $a0, $s2, 0x8
    ctx->r4 = ADD32(ctx->r18, 0X8);
    // 0x801CBD80: or          $t9, $t7, $at
    ctx->r25 = ctx->r15 | ctx->r1;
    // 0x801CBD84: sw          $t9, 0x2908($v1)
    MEM_W(0X2908, ctx->r3) = ctx->r25;
    // 0x801CBD88: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CBD8C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CBD90: jalr        $t9
    // 0x801CBD94: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_27;
    // 0x801CBD94: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    after_27:
    // 0x801CBD98: lh          $t8, 0xE($s2)
    ctx->r24 = MEM_H(ctx->r18, 0XE);
    // 0x801CBD9C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBDA0: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CBDA4: sll         $t4, $t8, 1
    ctx->r12 = S32(ctx->r24 << 1);
    // 0x801CBDA8: addu        $v0, $s2, $t4
    ctx->r2 = ADD32(ctx->r18, ctx->r12);
    // 0x801CBDAC: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x801CBDB0: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x801CBDB4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CBDB8: sll         $t9, $t7, 2
    ctx->r25 = S32(ctx->r15 << 2);
    // 0x801CBDBC: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x801CBDC0: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x801CBDC4: addu        $at, $at, $t9
    ctx->r1 = ADD32(ctx->r1, ctx->r25);
    // 0x801CBDC8: lw          $t9, 0x7B24($at)
    ctx->r25 = MEM_W(ctx->r1, 0X7B24);
    // 0x801CBDCC: jalr        $t9
    // 0x801CBDD0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_28;
    // 0x801CBDD0: nop

    after_28:
    // 0x801CBDD4: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_801CBDD8:
    // 0x801CBDD8: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x801CBDDC: lw          $s1, 0x24($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X24);
    // 0x801CBDE0: lw          $s2, 0x28($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X28);
    // 0x801CBDE4: jr          $ra
    // 0x801CBDE8: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
    return;
    // 0x801CBDE8: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
;}
RECOMP_FUNC void overlay_system_1_func_801CBDEC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CBDEC: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CBDF0: lw          $t6, -0x5438($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5438);
    // 0x801CBDF4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CBDF8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CBDFC: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x801CBE00: bgez        $t7, L_801CBE34
    if (SIGNED(ctx->r15) >= 0) {
        // 0x801CBE04: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_801CBE34;
    }
    // 0x801CBE04: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801CBE08: lh          $t8, 0xE($a0)
    ctx->r24 = MEM_H(ctx->r4, 0XE);
    // 0x801CBE0C: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x801CBE10: sll         $t9, $t8, 1
    ctx->r25 = S32(ctx->r24 << 1);
    // 0x801CBE14: addu        $t0, $a0, $t9
    ctx->r8 = ADD32(ctx->r4, ctx->r25);
    // 0x801CBE18: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CBE1C: addiu       $t9, $t9, 0x1C5C
    ctx->r25 = ADD32(ctx->r25, 0X1C5C);
    // 0x801CBE20: sb          $zero, 0x6($t0)
    MEM_B(0X6, ctx->r8) = 0;
    // 0x801CBE24: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x801CBE28: jalr        $t9
    // 0x801CBE2C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CBE2C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    after_0:
    // 0x801CBE30: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
L_801CBE34:
    // 0x801CBE34: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CBE38: addiu       $t9, $t9, 0x1860
    ctx->r25 = ADD32(ctx->r25, 0X1860);
    // 0x801CBE3C: jalr        $t9
    // 0x801CBE40: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CBE40: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    after_1:
    // 0x801CBE44: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CBE48: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CBE4C: jr          $ra
    // 0x801CBE50: nop

    return;
    // 0x801CBE50: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CBE54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CBE54: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CBE58: lh          $t6, -0x54C8($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54C8);
    // 0x801CBE5C: addiu       $v1, $zero, 0x65
    ctx->r3 = ADD32(0, 0X65);
    // 0x801CBE60: beq         $t6, $zero, L_801CBE70
    if (ctx->r14 == 0) {
        // 0x801CBE64: nop

            goto L_801CBE70;
    }
    // 0x801CBE64: nop

    // 0x801CBE68: jr          $ra
    // 0x801CBE6C: addiu       $v0, $zero, 0x64
    ctx->r2 = ADD32(0, 0X64);
    return;
    // 0x801CBE6C: addiu       $v0, $zero, 0x64
    ctx->r2 = ADD32(0, 0X64);
L_801CBE70:
    // 0x801CBE70: jr          $ra
    // 0x801CBE74: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x801CBE74: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void overlay_system_1_func_801CBE78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CBE78: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CBE7C: lh          $t6, -0x54C8($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54C8);
    // 0x801CBE80: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x801CBE84: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x801CBE88: beq         $t6, $zero, L_801CBE98
    if (ctx->r14 == 0) {
        // 0x801CBE8C: lui         $t0, 0x801D
        ctx->r8 = S32(0X801D << 16);
            goto L_801CBE98;
    }
    // 0x801CBE8C: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801CBE90: b           L_801CBE9C
    // 0x801CBE94: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CBE9C;
    // 0x801CBE94: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801CBE98:
    // 0x801CBE98: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801CBE9C:
    // 0x801CBE9C: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x801CBEA0: addiu       $t0, $t0, 0x7B48
    ctx->r8 = ADD32(ctx->r8, 0X7B48);
    // 0x801CBEA4: addiu       $t8, $t8, 0x7B4C
    ctx->r24 = ADD32(ctx->r24, 0X7B4C);
    // 0x801CBEA8: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x801CBEAC: lw          $a0, 0x0($t0)
    ctx->r4 = MEM_W(ctx->r8, 0X0);
    // 0x801CBEB0: addu        $a1, $t7, $t8
    ctx->r5 = ADD32(ctx->r15, ctx->r24);
    // 0x801CBEB4: lw          $t2, 0x0($a1)
    ctx->r10 = MEM_W(ctx->r5, 0X0);
    // 0x801CBEB8: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x801CBEBC: sllv        $t1, $t9, $a0
    ctx->r9 = S32(ctx->r25 << (ctx->r4 & 31));
    // 0x801CBEC0: addiu       $t3, $a0, 0x1
    ctx->r11 = ADD32(ctx->r4, 0X1);
    // 0x801CBEC4: and         $v1, $t1, $t2
    ctx->r3 = ctx->r9 & ctx->r10;
    // 0x801CBEC8: beq         $v1, $zero, L_801CBEFC
    if (ctx->r3 == 0) {
        // 0x801CBECC: sw          $t3, 0x0($t0)
        MEM_W(0X0, ctx->r8) = ctx->r11;
            goto L_801CBEFC;
    }
    // 0x801CBECC: sw          $t3, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r11;
    // 0x801CBED0: lh          $t4, 0x0($a3)
    ctx->r12 = MEM_H(ctx->r7, 0X0);
    // 0x801CBED4: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x801CBED8: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CBEDC: addiu       $t6, $t6, 0x7B4C
    ctx->r14 = ADD32(ctx->r14, 0X7B4C);
    // 0x801CBEE0: or          $t5, $t4, $at
    ctx->r13 = ctx->r12 | ctx->r1;
    // 0x801CBEE4: bne         $a1, $t6, L_801CBFAC
    if (ctx->r5 != ctx->r14) {
        // 0x801CBEE8: sh          $t5, 0x0($a3)
        MEM_H(0X0, ctx->r7) = ctx->r13;
            goto L_801CBFAC;
    }
    // 0x801CBEE8: sh          $t5, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r13;
    // 0x801CBEEC: lw          $t7, 0x0($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X0);
    // 0x801CBEF0: addiu       $t8, $t7, 0xC
    ctx->r24 = ADD32(ctx->r15, 0XC);
    // 0x801CBEF4: jr          $ra
    // 0x801CBEF8: sw          $t8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r24;
    return;
    // 0x801CBEF8: sw          $t8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r24;
L_801CBEFC:
    // 0x801CBEFC: lh          $t9, 0x0($a3)
    ctx->r25 = MEM_H(ctx->r7, 0X0);
    // 0x801CBF00: lw          $t2, 0x8($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X8);
    // 0x801CBF04: andi        $t1, $t9, 0x7FFF
    ctx->r9 = ctx->r25 & 0X7FFF;
    // 0x801CBF08: sh          $t1, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r9;
    // 0x801CBF0C: lw          $t3, 0x40($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X40);
    // 0x801CBF10: sw          $t3, 0x40($a3)
    MEM_W(0X40, ctx->r7) = ctx->r11;
    // 0x801CBF14: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x801CBF18: lh          $t4, 0x4($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X4);
    // 0x801CBF1C: andi        $t5, $t4, 0x2000
    ctx->r13 = ctx->r12 & 0X2000;
    // 0x801CBF20: beql        $t5, $zero, L_801CBF40
    if (ctx->r13 == 0) {
        // 0x801CBF24: lw          $t8, 0x0($v0)
        ctx->r24 = MEM_W(ctx->r2, 0X0);
            goto L_801CBF40;
    }
    goto skip_0;
    // 0x801CBF24: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    skip_0:
    // 0x801CBF28: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x801CBF2C: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x801CBF30: or          $t7, $t6, $at
    ctx->r15 = ctx->r14 | ctx->r1;
    // 0x801CBF34: b           L_801CBF44
    // 0x801CBF38: sw          $t7, 0x3C($a3)
    MEM_W(0X3C, ctx->r7) = ctx->r15;
        goto L_801CBF44;
    // 0x801CBF38: sw          $t7, 0x3C($a3)
    MEM_W(0X3C, ctx->r7) = ctx->r15;
    // 0x801CBF3C: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
L_801CBF40:
    // 0x801CBF40: sw          $t8, 0x3C($a3)
    MEM_W(0X3C, ctx->r7) = ctx->r24;
L_801CBF44:
    // 0x801CBF44: lw          $t9, 0x0($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X0);
    // 0x801CBF48: lui         $at, 0x4170
    ctx->r1 = S32(0X4170 << 16);
    // 0x801CBF4C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x801CBF50: lh          $t1, 0x6($t9)
    ctx->r9 = MEM_H(ctx->r25, 0X6);
    // 0x801CBF54: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x801CBF58: nop

    // 0x801CBF5C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801CBF60: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x801CBF64: swc1        $f8, 0x50($a3)
    MEM_W(0X50, ctx->r7) = ctx->f8.u32l;
    // 0x801CBF68: lw          $t2, 0x0($a2)
    ctx->r10 = MEM_W(ctx->r6, 0X0);
    // 0x801CBF6C: lh          $t3, 0x8($t2)
    ctx->r11 = MEM_H(ctx->r10, 0X8);
    // 0x801CBF70: mtc1        $t3, $f10
    ctx->f10.u32l = ctx->r11;
    // 0x801CBF74: nop

    // 0x801CBF78: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801CBF7C: div.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f18.fl = DIV_S(ctx->f16.fl, ctx->f0.fl);
    // 0x801CBF80: swc1        $f18, 0x54($a3)
    MEM_W(0X54, ctx->r7) = ctx->f18.u32l;
    // 0x801CBF84: lw          $t4, 0x0($a2)
    ctx->r12 = MEM_W(ctx->r6, 0X0);
    // 0x801CBF88: lh          $t5, 0xA($t4)
    ctx->r13 = MEM_H(ctx->r12, 0XA);
    // 0x801CBF8C: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x801CBF90: nop

    // 0x801CBF94: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801CBF98: div.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f8.fl = DIV_S(ctx->f6.fl, ctx->f0.fl);
    // 0x801CBF9C: swc1        $f8, 0x58($a3)
    MEM_W(0X58, ctx->r7) = ctx->f8.u32l;
    // 0x801CBFA0: lw          $t6, 0x0($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X0);
    // 0x801CBFA4: addiu       $t7, $t6, 0xC
    ctx->r15 = ADD32(ctx->r14, 0XC);
    // 0x801CBFA8: sw          $t7, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r15;
L_801CBFAC:
    // 0x801CBFAC: jr          $ra
    // 0x801CBFB0: nop

    return;
    // 0x801CBFB0: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CBFB4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CBFB4: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x801CBFB8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CBFBC: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x801CBFC0: sw          $a1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r5;
    // 0x801CBFC4: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x801CBFC8: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801CBFCC: addiu       $t2, $t2, 0x7B54
    ctx->r10 = ADD32(ctx->r10, 0X7B54);
    // 0x801CBFD0: lw          $t7, 0x14($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X14);
    // 0x801CBFD4: sll         $t1, $a1, 2
    ctx->r9 = S32(ctx->r5 << 2);
    // 0x801CBFD8: addu        $v1, $t1, $t2
    ctx->r3 = ADD32(ctx->r9, ctx->r10);
    // 0x801CBFDC: lwc1        $f4, 0x50($t7)
    ctx->f4.u32l = MEM_W(ctx->r15, 0X50);
    // 0x801CBFE0: lw          $t3, 0x0($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X0);
    // 0x801CBFE4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CBFE8: swc1        $f4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f4.u32l;
    // 0x801CBFEC: lw          $t8, 0x14($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X14);
    // 0x801CBFF0: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801CBFF4: addiu       $a1, $a1, -0x4188
    ctx->r5 = ADD32(ctx->r5, -0X4188);
    // 0x801CBFF8: lwc1        $f6, 0x54($t8)
    ctx->f6.u32l = MEM_W(ctx->r24, 0X54);
    // 0x801CBFFC: addiu       $a3, $sp, 0x30
    ctx->r7 = ADD32(ctx->r29, 0X30);
    // 0x801CC000: swc1        $f6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f6.u32l;
    // 0x801CC004: lw          $t9, 0x14($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X14);
    // 0x801CC008: lwc1        $f8, 0x58($t9)
    ctx->f8.u32l = MEM_W(ctx->r25, 0X58);
    // 0x801CC00C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC010: addiu       $t9, $t9, 0x4E84
    ctx->r25 = ADD32(ctx->r25, 0X4E84);
    // 0x801CC014: swc1        $f8, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f8.u32l;
    // 0x801CC018: lw          $t4, 0x0($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X0);
    // 0x801CC01C: sw          $t4, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r12;
    // 0x801CC020: lw          $t5, 0x0($v1)
    ctx->r13 = MEM_W(ctx->r3, 0X0);
    // 0x801CC024: sw          $zero, 0x7B48($at)
    MEM_W(0X7B48, ctx->r1) = 0;
    // 0x801CC028: addiu       $a2, $t5, 0x4
    ctx->r6 = ADD32(ctx->r13, 0X4);
    // 0x801CC02C: sw          $a2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r6;
    // 0x801CC030: lw          $a0, 0x14($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X14);
    // 0x801CC034: jalr        $t9
    // 0x801CC038: sw          $v0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CC038: sw          $v0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r2;
    after_0:
    // 0x801CC03C: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
    // 0x801CC040: lwc1        $f10, 0x24($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X24);
    // 0x801CC044: lw          $v1, 0x4C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X4C);
    // 0x801CC048: lw          $t7, 0x14($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X14);
    // 0x801CC04C: lui         $t1, 0x600
    ctx->r9 = S32(0X600 << 16);
    // 0x801CC050: addiu       $t1, $t1, 0x71F8
    ctx->r9 = ADD32(ctx->r9, 0X71F8);
    // 0x801CC054: swc1        $f10, 0x50($t7)
    MEM_W(0X50, ctx->r15) = ctx->f10.u32l;
    // 0x801CC058: lwc1        $f16, 0x28($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X28);
    // 0x801CC05C: lw          $t8, 0x14($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X14);
    // 0x801CC060: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x801CC064: swc1        $f16, 0x54($t8)
    MEM_W(0X54, ctx->r24) = ctx->f16.u32l;
    // 0x801CC068: lwc1        $f18, 0x2C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x801CC06C: lw          $t0, 0x14($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X14);
    // 0x801CC070: beq         $v1, $zero, L_801CC08C
    if (ctx->r3 == 0) {
        // 0x801CC074: swc1        $f18, 0x58($t0)
        MEM_W(0X58, ctx->r8) = ctx->f18.u32l;
            goto L_801CC08C;
    }
    // 0x801CC074: swc1        $f18, 0x58($t0)
    MEM_W(0X58, ctx->r8) = ctx->f18.u32l;
    // 0x801CC078: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801CC07C: beq         $v1, $at, L_801CC0CC
    if (ctx->r3 == ctx->r1) {
        // 0x801CC080: lui         $t2, 0x601
        ctx->r10 = S32(0X601 << 16);
            goto L_801CC0CC;
    }
    // 0x801CC080: lui         $t2, 0x601
    ctx->r10 = S32(0X601 << 16);
    // 0x801CC084: b           L_801CC114
    // 0x801CC088: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801CC114;
    // 0x801CC088: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CC08C:
    // 0x801CC08C: lw          $t4, 0x14($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X14);
    // 0x801CC090: or          $t2, $t1, $at
    ctx->r10 = ctx->r9 | ctx->r1;
    // 0x801CC094: sw          $t2, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r10;
    // 0x801CC098: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CC09C: lui         $t3, 0x600
    ctx->r11 = S32(0X600 << 16);
    // 0x801CC0A0: addiu       $t3, $t3, 0x6BC8
    ctx->r11 = ADD32(ctx->r11, 0X6BC8);
    // 0x801CC0A4: lw          $t6, 0x10($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X10);
    // 0x801CC0A8: lui         $t9, 0x600
    ctx->r25 = S32(0X600 << 16);
    // 0x801CC0AC: addiu       $t9, $t9, 0x6E00
    ctx->r25 = ADD32(ctx->r25, 0X6E00);
    // 0x801CC0B0: sw          $t3, 0x34($t6)
    MEM_W(0X34, ctx->r14) = ctx->r11;
    // 0x801CC0B4: lw          $t7, 0x14($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X14);
    // 0x801CC0B8: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801CC0BC: lw          $t0, 0x10($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X10);
    // 0x801CC0C0: lw          $t1, 0x10($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X10);
    // 0x801CC0C4: b           L_801CC110
    // 0x801CC0C8: sw          $t9, 0x34($t1)
    MEM_W(0X34, ctx->r9) = ctx->r25;
        goto L_801CC110;
    // 0x801CC0C8: sw          $t9, 0x34($t1)
    MEM_W(0X34, ctx->r9) = ctx->r25;
L_801CC0CC:
    // 0x801CC0CC: lw          $t3, 0x14($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X14);
    // 0x801CC0D0: addiu       $t2, $t2, -0x7B78
    ctx->r10 = ADD32(ctx->r10, -0X7B78);
    // 0x801CC0D4: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x801CC0D8: or          $t4, $t2, $at
    ctx->r12 = ctx->r10 | ctx->r1;
    // 0x801CC0DC: sw          $t4, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r12;
    // 0x801CC0E0: lw          $t6, 0x14($t3)
    ctx->r14 = MEM_W(ctx->r11, 0X14);
    // 0x801CC0E4: lui         $t5, 0x601
    ctx->r13 = S32(0X601 << 16);
    // 0x801CC0E8: addiu       $t5, $t5, -0x7F90
    ctx->r13 = ADD32(ctx->r13, -0X7F90);
    // 0x801CC0EC: lw          $t7, 0x10($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X10);
    // 0x801CC0F0: lui         $t8, 0x601
    ctx->r24 = S32(0X601 << 16);
    // 0x801CC0F4: addiu       $t8, $t8, -0x7DC8
    ctx->r24 = ADD32(ctx->r24, -0X7DC8);
    // 0x801CC0F8: sw          $t5, 0x34($t7)
    MEM_W(0X34, ctx->r15) = ctx->r13;
    // 0x801CC0FC: lw          $t0, 0x14($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X14);
    // 0x801CC100: lw          $t9, 0x14($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X14);
    // 0x801CC104: lw          $t1, 0x10($t9)
    ctx->r9 = MEM_W(ctx->r25, 0X10);
    // 0x801CC108: lw          $t2, 0x10($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X10);
    // 0x801CC10C: sw          $t8, 0x34($t2)
    MEM_W(0X34, ctx->r10) = ctx->r24;
L_801CC110:
    // 0x801CC110: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CC114:
    // 0x801CC114: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    // 0x801CC118: jr          $ra
    // 0x801CC11C: nop

    return;
    // 0x801CC11C: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CC120(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC120: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CC124: lh          $t6, -0x54C8($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X54C8);
    // 0x801CC128: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CC12C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CC130: beq         $t6, $zero, L_801CC140
    if (ctx->r14 == 0) {
        // 0x801CC134: addiu       $a1, $zero, 0x1
        ctx->r5 = ADD32(0, 0X1);
            goto L_801CC140;
    }
    // 0x801CC134: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x801CC138: b           L_801CC140
    // 0x801CC13C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
        goto L_801CC140;
    // 0x801CC13C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
L_801CC140:
    // 0x801CC140: jal         0x801CBFB4
    // 0x801CC144: nop

    overlay_system_1_func_801CBFB4(rdram, ctx);
        goto after_0;
    // 0x801CC144: nop

    after_0:
    // 0x801CC148: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CC14C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CC150: jr          $ra
    // 0x801CC154: nop

    return;
    // 0x801CC154: nop

    // 0x801CC158: nop

    // 0x801CC15C: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CC160(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC160: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801CC164: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CC168: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CC16C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x801CC170: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CC174: lw          $t6, 0x8($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X8);
    // 0x801CC178: sll         $t7, $t6, 0
    ctx->r15 = S32(ctx->r14 << 0);
    // 0x801CC17C: bltzl       $t7, L_801CC330
    if (SIGNED(ctx->r15) < 0) {
        // 0x801CC180: lh          $v1, 0xE($s0)
        ctx->r3 = MEM_H(ctx->r16, 0XE);
            goto L_801CC330;
    }
    goto skip_0;
    // 0x801CC180: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    skip_0:
    // 0x801CC184: lw          $a3, 0x0($v1)
    ctx->r7 = MEM_W(ctx->r3, 0X0);
    // 0x801CC188: lui         $at, 0x3
    ctx->r1 = S32(0X3 << 16);
    // 0x801CC18C: ori         $at, $at, 0xC000
    ctx->r1 = ctx->r1 | 0XC000;
    // 0x801CC190: and         $t8, $a3, $at
    ctx->r24 = ctx->r7 & ctx->r1;
    // 0x801CC194: beq         $t8, $zero, L_801CC228
    if (ctx->r24 == 0) {
        // 0x801CC198: lui         $t9, 0x800F
        ctx->r25 = S32(0X800F << 16);
            goto L_801CC228;
    }
    // 0x801CC198: lui         $t9, 0x800F
    ctx->r25 = S32(0X800F << 16);
    // 0x801CC19C: addiu       $at, $zero, -0x701
    ctx->r1 = ADD32(0, -0X701);
    // 0x801CC1A0: and         $t9, $a3, $at
    ctx->r25 = ctx->r7 & ctx->r1;
    // 0x801CC1A4: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x801CC1A8: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x801CC1AC: lhu         $t0, 0xC($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0XC);
    // 0x801CC1B0: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x801CC1B4: ori         $at, $at, 0x777F
    ctx->r1 = ctx->r1 | 0X777F;
    // 0x801CC1B8: andi        $t2, $t0, 0xFFF1
    ctx->r10 = ctx->r8 & 0XFFF1;
    // 0x801CC1BC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC1C0: sh          $t2, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r10;
    // 0x801CC1C4: and         $t3, $t2, $at
    ctx->r11 = ctx->r10 & ctx->r1;
    // 0x801CC1C8: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CC1CC: sh          $t3, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r11;
    // 0x801CC1D0: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801CC1D4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CC1D8: jalr        $t9
    // 0x801CC1DC: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CC1DC: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    after_0:
    // 0x801CC1E0: lh          $t4, 0xE($s0)
    ctx->r12 = MEM_H(ctx->r16, 0XE);
    // 0x801CC1E4: lui         $t8, 0x800F
    ctx->r24 = S32(0X800F << 16);
    // 0x801CC1E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC1EC: sll         $t5, $t4, 1
    ctx->r13 = S32(ctx->r12 << 1);
    // 0x801CC1F0: addu        $v0, $s0, $t5
    ctx->r2 = ADD32(ctx->r16, ctx->r13);
    // 0x801CC1F4: lbu         $t6, 0x8($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC1F8: lbu         $t0, 0x9($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC1FC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC200: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x801CC204: sb          $t7, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r15;
    // 0x801CC208: lw          $t8, 0x3700($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X3700);
    // 0x801CC20C: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x801CC210: addu        $t2, $t8, $t1
    ctx->r10 = ADD32(ctx->r24, ctx->r9);
    // 0x801CC214: lw          $t9, 0x0($t2)
    ctx->r25 = MEM_W(ctx->r10, 0X0);
    // 0x801CC218: jalr        $t9
    // 0x801CC21C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CC21C: nop

    after_1:
    // 0x801CC220: b           L_801CC384
    // 0x801CC224: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CC384;
    // 0x801CC224: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CC228:
    // 0x801CC228: lw          $t9, 0x3F80($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3F80);
    // 0x801CC22C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC230: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x801CC234: jalr        $t9
    // 0x801CC238: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CC238: nop

    after_2:
    // 0x801CC23C: beq         $v0, $zero, L_801CC284
    if (ctx->r2 == 0) {
        // 0x801CC240: lw          $v1, 0x24($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X24);
            goto L_801CC284;
    }
    // 0x801CC240: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x801CC244: lw          $t3, 0x0($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X0);
    // 0x801CC248: addiu       $at, $zero, -0x701
    ctx->r1 = ADD32(0, -0X701);
    // 0x801CC24C: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x801CC250: and         $t4, $t3, $at
    ctx->r12 = ctx->r11 & ctx->r1;
    // 0x801CC254: sw          $t4, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r12;
    // 0x801CC258: lhu         $t5, 0xC($v0)
    ctx->r13 = MEM_HU(ctx->r2, 0XC);
    // 0x801CC25C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC260: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801CC264: andi        $t6, $t5, 0xFFF1
    ctx->r14 = ctx->r13 & 0XFFF1;
    // 0x801CC268: sh          $t6, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r14;
    // 0x801CC26C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x801CC270: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CC274: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CC278: jalr        $t9
    // 0x801CC27C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CC27C: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_3:
    // 0x801CC280: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_801CC284:
    // 0x801CC284: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x801CC288: lw          $t7, 0x0($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X0);
    // 0x801CC28C: addiu       $at, $zero, -0x7E1
    ctx->r1 = ADD32(0, -0X7E1);
    // 0x801CC290: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CC294: sll         $t0, $t7, 2
    ctx->r8 = S32(ctx->r15 << 2);
    // 0x801CC298: bgez        $t0, L_801CC32C
    if (SIGNED(ctx->r8) >= 0) {
        // 0x801CC29C: addiu       $a1, $s0, 0xE
        ctx->r5 = ADD32(ctx->r16, 0XE);
            goto L_801CC32C;
    }
    // 0x801CC29C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CC2A0: lw          $t8, 0x0($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X0);
    // 0x801CC2A4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801CC2A8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC2AC: and         $t1, $t8, $at
    ctx->r9 = ctx->r24 & ctx->r1;
    // 0x801CC2B0: sw          $t1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r9;
    // 0x801CC2B4: lhu         $t2, 0xC($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0XC);
    // 0x801CC2B8: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x801CC2BC: ori         $at, $at, 0x777F
    ctx->r1 = ctx->r1 | 0X777F;
    // 0x801CC2C0: andi        $t4, $t2, 0xFFF1
    ctx->r12 = ctx->r10 & 0XFFF1;
    // 0x801CC2C4: sh          $t4, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r12;
    // 0x801CC2C8: and         $t5, $t4, $at
    ctx->r13 = ctx->r12 & ctx->r1;
    // 0x801CC2CC: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CC2D0: sh          $t5, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r13;
    // 0x801CC2D4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x801CC2D8: swc1        $f0, 0x2C($v0)
    MEM_W(0X2C, ctx->r2) = ctx->f0.u32l;
    // 0x801CC2DC: jalr        $t9
    // 0x801CC2E0: swc1        $f0, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CC2E0: swc1        $f0, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->f0.u32l;
    after_4:
    // 0x801CC2E4: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x801CC2E8: lui         $t1, 0x800F
    ctx->r9 = S32(0X800F << 16);
    // 0x801CC2EC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC2F0: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x801CC2F4: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x801CC2F8: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC2FC: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC300: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC304: addiu       $t8, $t0, 0x1
    ctx->r24 = ADD32(ctx->r8, 0X1);
    // 0x801CC308: sb          $t8, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r24;
    // 0x801CC30C: lw          $t1, 0x3700($t1)
    ctx->r9 = MEM_W(ctx->r9, 0X3700);
    // 0x801CC310: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801CC314: addu        $t4, $t1, $t3
    ctx->r12 = ADD32(ctx->r9, ctx->r11);
    // 0x801CC318: lw          $t9, 0x0($t4)
    ctx->r25 = MEM_W(ctx->r12, 0X0);
    // 0x801CC31C: jalr        $t9
    // 0x801CC320: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x801CC320: nop

    after_5:
    // 0x801CC324: b           L_801CC384
    // 0x801CC328: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CC384;
    // 0x801CC328: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CC32C:
    // 0x801CC32C: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
L_801CC330:
    // 0x801CC330: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CC334: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC338: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801CC33C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801CC340: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801CC344: sll         $t5, $v1, 1
    ctx->r13 = S32(ctx->r3 << 1);
    // 0x801CC348: addu        $v0, $s0, $t5
    ctx->r2 = ADD32(ctx->r16, ctx->r13);
    // 0x801CC34C: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x801CC350: lbu         $t0, 0x9($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC354: lbu         $t6, 0x8($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC358: sll         $t8, $t0, 2
    ctx->r24 = S32(ctx->r8 << 2);
    // 0x801CC35C: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x801CC360: sb          $t7, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r15;
    // 0x801CC364: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x801CC368: lw          $t9, 0x7BF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7BF0);
    // 0x801CC36C: jalr        $t9
    // 0x801CC370: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x801CC370: nop

    after_6:
    // 0x801CC374: lh          $t2, 0xE($s0)
    ctx->r10 = MEM_H(ctx->r16, 0XE);
    // 0x801CC378: addiu       $t1, $t2, -0x1
    ctx->r9 = ADD32(ctx->r10, -0X1);
    // 0x801CC37C: sh          $t1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r9;
    // 0x801CC380: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CC384:
    // 0x801CC384: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CC388: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x801CC38C: jr          $ra
    // 0x801CC390: nop

    return;
    // 0x801CC390: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CC394(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC394: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801CC398: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CC39C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CC3A0: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x801CC3A4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CC3A8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC3AC: lhu         $t6, 0x14($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X14);
    // 0x801CC3B0: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CC3B4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CC3B8: andi        $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 & 0X8000;
    // 0x801CC3BC: beq         $t7, $zero, L_801CC410
    if (ctx->r15 == 0) {
        // 0x801CC3C0: addiu       $a0, $a0, 0x8
        ctx->r4 = ADD32(ctx->r4, 0X8);
            goto L_801CC410;
    }
    // 0x801CC3C0: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801CC3C4: jalr        $t9
    // 0x801CC3C8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CC3C8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x801CC3CC: lh          $t8, 0xE($s0)
    ctx->r24 = MEM_H(ctx->r16, 0XE);
    // 0x801CC3D0: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CC3D4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC3D8: sll         $t0, $t8, 1
    ctx->r8 = S32(ctx->r24 << 1);
    // 0x801CC3DC: addu        $v0, $s0, $t0
    ctx->r2 = ADD32(ctx->r16, ctx->r8);
    // 0x801CC3E0: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC3E4: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC3E8: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC3EC: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x801CC3F0: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x801CC3F4: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x801CC3F8: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
    // 0x801CC3FC: lw          $t9, 0x7BF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7BF0);
    // 0x801CC400: jalr        $t9
    // 0x801CC404: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CC404: nop

    after_1:
    // 0x801CC408: b           L_801CC4D8
    // 0x801CC40C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CC4D8;
    // 0x801CC40C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CC410:
    // 0x801CC410: lhu         $v1, 0x14($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X14);
    // 0x801CC414: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CC418: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CC41C: andi        $t5, $v1, 0x800
    ctx->r13 = ctx->r3 & 0X800;
    // 0x801CC420: beq         $t5, $zero, L_801CC47C
    if (ctx->r13 == 0) {
        // 0x801CC424: andi        $t3, $v1, 0x80
        ctx->r11 = ctx->r3 & 0X80;
            goto L_801CC47C;
    }
    // 0x801CC424: andi        $t3, $v1, 0x80
    ctx->r11 = ctx->r3 & 0X80;
    // 0x801CC428: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC42C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CC430: jalr        $t9
    // 0x801CC434: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CC434: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    after_2:
    // 0x801CC438: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x801CC43C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CC440: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC444: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x801CC448: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x801CC44C: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC450: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC454: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC458: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x801CC45C: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x801CC460: sb          $t0, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r8;
    // 0x801CC464: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
    // 0x801CC468: lw          $t9, 0x7BF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7BF0);
    // 0x801CC46C: jalr        $t9
    // 0x801CC470: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CC470: nop

    after_3:
    // 0x801CC474: b           L_801CC4D8
    // 0x801CC478: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CC4D8;
    // 0x801CC478: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CC47C:
    // 0x801CC47C: beq         $t3, $zero, L_801CC4D4
    if (ctx->r11 == 0) {
        // 0x801CC480: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_801CC4D4;
    }
    // 0x801CC480: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CC484: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC488: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CC48C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CC490: jalr        $t9
    // 0x801CC494: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CC494: addiu       $a2, $zero, 0x5
    ctx->r6 = ADD32(0, 0X5);
    after_4:
    // 0x801CC498: lh          $t4, 0xE($s0)
    ctx->r12 = MEM_H(ctx->r16, 0XE);
    // 0x801CC49C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CC4A0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC4A4: sll         $t5, $t4, 1
    ctx->r13 = S32(ctx->r12 << 1);
    // 0x801CC4A8: addu        $v0, $s0, $t5
    ctx->r2 = ADD32(ctx->r16, ctx->r13);
    // 0x801CC4AC: lbu         $t8, 0x9($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC4B0: lbu         $t6, 0x8($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC4B4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC4B8: sll         $t0, $t8, 2
    ctx->r8 = S32(ctx->r24 << 2);
    // 0x801CC4BC: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x801CC4C0: sb          $t7, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r15;
    // 0x801CC4C4: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801CC4C8: lw          $t9, 0x7BF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7BF0);
    // 0x801CC4CC: jalr        $t9
    // 0x801CC4D0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x801CC4D0: nop

    after_5:
L_801CC4D4:
    // 0x801CC4D4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CC4D8:
    // 0x801CC4D8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CC4DC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801CC4E0: jr          $ra
    // 0x801CC4E4: nop

    return;
    // 0x801CC4E4: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CC4E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC4E8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801CC4EC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CC4F0: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801CC4F4: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801CC4F8: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x801CC4FC: addiu       $at, $zero, -0x701
    ctx->r1 = ADD32(0, -0X701);
    // 0x801CC500: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x801CC504: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x801CC508: lhu         $t0, 0x14($v1)
    ctx->r8 = MEM_HU(ctx->r3, 0X14);
    // 0x801CC50C: lbu         $t4, 0x58($v1)
    ctx->r12 = MEM_BU(ctx->r3, 0X58);
    // 0x801CC510: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x801CC514: andi        $t2, $t0, 0xFFF1
    ctx->r10 = ctx->r8 & 0XFFF1;
    // 0x801CC518: ori         $t9, $t7, 0x200
    ctx->r25 = ctx->r15 | 0X200;
    // 0x801CC51C: ori         $t3, $t2, 0x8
    ctx->r11 = ctx->r10 | 0X8;
    // 0x801CC520: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x801CC524: sh          $t2, 0x14($v1)
    MEM_H(0X14, ctx->r3) = ctx->r10;
    // 0x801CC528: andi        $t5, $t4, 0x2
    ctx->r13 = ctx->r12 & 0X2;
    // 0x801CC52C: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x801CC530: beq         $t5, $zero, L_801CC558
    if (ctx->r13 == 0) {
        // 0x801CC534: sh          $t3, 0x14($v1)
        MEM_H(0X14, ctx->r3) = ctx->r11;
            goto L_801CC558;
    }
    // 0x801CC534: sh          $t3, 0x14($v1)
    MEM_H(0X14, ctx->r3) = ctx->r11;
    // 0x801CC538: addiu       $s0, $v1, 0x58
    ctx->r16 = ADD32(ctx->r3, 0X58);
    // 0x801CC53C: lbu         $t6, 0x0($s0)
    ctx->r14 = MEM_BU(ctx->r16, 0X0);
    // 0x801CC540: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x801CC544: sll         $t8, $t7, 3
    ctx->r24 = S32(ctx->r15 << 3);
    // 0x801CC548: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x801CC54C: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x801CC550: addu        $t9, $s0, $t8
    ctx->r25 = ADD32(ctx->r16, ctx->r24);
    // 0x801CC554: sw          $zero, 0x14($t9)
    MEM_W(0X14, ctx->r25) = 0;
L_801CC558:
    // 0x801CC558: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x801CC55C: addiu       $v0, $v0, 0x35F0
    ctx->r2 = ADD32(ctx->r2, 0X35F0);
    // 0x801CC560: lhu         $t0, 0x4($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X4);
    // 0x801CC564: addiu       $s0, $v1, 0x58
    ctx->r16 = ADD32(ctx->r3, 0X58);
    // 0x801CC568: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC56C: beq         $t0, $zero, L_801CC5B0
    if (ctx->r8 == 0) {
        // 0x801CC570: lui         $a1, 0x800B
        ctx->r5 = S32(0X800B << 16);
            goto L_801CC5B0;
    }
    // 0x801CC570: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CC574: lhu         $t1, 0x6($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0X6);
    // 0x801CC578: addiu       $a2, $zero, 0x69
    ctx->r6 = ADD32(0, 0X69);
    // 0x801CC57C: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CC580: andi        $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 & 0X2000;
    // 0x801CC584: beq         $t2, $zero, L_801CC5B0
    if (ctx->r10 == 0) {
        // 0x801CC588: lui         $t9, 0x8002
        ctx->r25 = S32(0X8002 << 16);
            goto L_801CC5B0;
    }
    // 0x801CC588: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC58C: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CC590: addiu       $t9, $t9, -0x754
    ctx->r25 = ADD32(ctx->r25, -0X754);
    // 0x801CC594: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CC598: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC59C: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CC5A0: jalr        $t9
    // 0x801CC5A4: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CC5A4: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x801CC5A8: b           L_801CC5D4
    // 0x801CC5AC: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
        goto L_801CC5D4;
    // 0x801CC5AC: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_801CC5B0:
    // 0x801CC5B0: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC5B4: addiu       $t9, $t9, -0x754
    ctx->r25 = ADD32(ctx->r25, -0X754);
    // 0x801CC5B8: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CC5BC: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CC5C0: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CC5C4: addiu       $a2, $zero, 0x67
    ctx->r6 = ADD32(0, 0X67);
    // 0x801CC5C8: jalr        $t9
    // 0x801CC5CC: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CC5CC: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_1:
    // 0x801CC5D0: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_801CC5D4:
    // 0x801CC5D4: beq         $v0, $zero, L_801CC5F8
    if (ctx->r2 == 0) {
        // 0x801CC5D8: addiu       $a0, $s1, 0x8
        ctx->r4 = ADD32(ctx->r17, 0X8);
            goto L_801CC5F8;
    }
    // 0x801CC5D8: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x801CC5DC: lhu         $t3, 0x16($v1)
    ctx->r11 = MEM_HU(ctx->r3, 0X16);
    // 0x801CC5E0: andi        $t4, $t3, 0x4000
    ctx->r12 = ctx->r11 & 0X4000;
    // 0x801CC5E4: beql        $t4, $zero, L_801CC5FC
    if (ctx->r12 == 0) {
        // 0x801CC5E8: lbu         $t7, 0x0($s0)
        ctx->r15 = MEM_BU(ctx->r16, 0X0);
            goto L_801CC5FC;
    }
    goto skip_0;
    // 0x801CC5E8: lbu         $t7, 0x0($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X0);
    skip_0:
    // 0x801CC5EC: lbu         $t5, 0x0($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X0);
    // 0x801CC5F0: ori         $t6, $t5, 0x10
    ctx->r14 = ctx->r13 | 0X10;
    // 0x801CC5F4: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
L_801CC5F8:
    // 0x801CC5F8: lbu         $t7, 0x0($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X0);
L_801CC5FC:
    // 0x801CC5FC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC600: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CC604: andi        $t8, $t7, 0xFFFD
    ctx->r24 = ctx->r15 & 0XFFFD;
    // 0x801CC608: sb          $t8, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r24;
    // 0x801CC60C: jalr        $t9
    // 0x801CC610: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CC610: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_2:
    // 0x801CC614: lh          $t0, 0xE($s1)
    ctx->r8 = MEM_H(ctx->r17, 0XE);
    // 0x801CC618: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CC61C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CC620: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x801CC624: addu        $v0, $s1, $t1
    ctx->r2 = ADD32(ctx->r17, ctx->r9);
    // 0x801CC628: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC62C: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC630: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC634: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x801CC638: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x801CC63C: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x801CC640: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x801CC644: lw          $t9, 0x7BF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7BF0);
    // 0x801CC648: jalr        $t9
    // 0x801CC64C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CC64C: nop

    after_3:
    // 0x801CC650: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801CC654: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x801CC658: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801CC65C: jr          $ra
    // 0x801CC660: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x801CC660: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void overlay_system_1_func_801CC664(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC664: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801CC668: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CC66C: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x801CC670: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC674: addiu       $t9, $t9, 0x1860
    ctx->r25 = ADD32(ctx->r25, 0X1860);
    // 0x801CC678: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x801CC67C: jalr        $t9
    // 0x801CC680: sw          $t6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CC680: sw          $t6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r14;
    after_0:
    // 0x801CC684: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x801CC688: addiu       $at, $zero, 0xF1
    ctx->r1 = ADD32(0, 0XF1);
    // 0x801CC68C: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC690: lh          $v1, 0xEA($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XEA);
    // 0x801CC694: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x801CC698: addiu       $t9, $t9, 0x4BD8
    ctx->r25 = ADD32(ctx->r25, 0X4BD8);
    // 0x801CC69C: bne         $v1, $at, L_801CC6BC
    if (ctx->r3 != ctx->r1) {
        // 0x801CC6A0: addiu       $v0, $v0, 0x8
        ctx->r2 = ADD32(ctx->r2, 0X8);
            goto L_801CC6BC;
    }
    // 0x801CC6A0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC6A4: lhu         $t7, 0xC($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0XC);
    // 0x801CC6A8: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x801CC6AC: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x801CC6B0: and         $t8, $t7, $at
    ctx->r24 = ctx->r15 & ctx->r1;
    // 0x801CC6B4: sh          $t8, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r24;
    // 0x801CC6B8: lh          $v1, 0xE2($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XE2);
L_801CC6BC:
    // 0x801CC6BC: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x801CC6C0: bne         $v1, $at, L_801CC6D8
    if (ctx->r3 != ctx->r1) {
        // 0x801CC6C4: nop

            goto L_801CC6D8;
    }
    // 0x801CC6C4: nop

    // 0x801CC6C8: lw          $t0, 0x0($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X0);
    // 0x801CC6CC: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x801CC6D0: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x801CC6D4: sw          $t1, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r9;
L_801CC6D8:
    // 0x801CC6D8: jalr        $t9
    // 0x801CC6DC: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CC6DC: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    after_1:
    // 0x801CC6E0: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x801CC6E4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CC6E8: ldc1        $f0, 0x7D30($at)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r1, 0X7D30);
    // 0x801CC6EC: lwc1        $f4, 0x2C($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X2C);
    // 0x801CC6F0: lwc1        $f16, 0x34($v0)
    ctx->f16.u32l = MEM_W(ctx->r2, 0X34);
    // 0x801CC6F4: lw          $t2, 0x0($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X0);
    // 0x801CC6F8: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801CC6FC: cvt.d.s     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl);
    ctx->f18.d = CVT_D_S(ctx->f16.fl);
    // 0x801CC700: mul.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d);
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x801CC704: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801CC708: mul.d       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d);
    ctx->f4.d = MUL_D(ctx->f18.d, ctx->f0.d);
    // 0x801CC70C: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d);
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x801CC710: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d);
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x801CC714: swc1        $f10, 0x2C($v0)
    MEM_W(0X2C, ctx->r2) = ctx->f10.u32l;
    // 0x801CC718: bgez        $t3, L_801CC72C
    if (SIGNED(ctx->r11) >= 0) {
        // 0x801CC71C: swc1        $f6, 0x34($v0)
        MEM_W(0X34, ctx->r2) = ctx->f6.u32l;
            goto L_801CC72C;
    }
    // 0x801CC71C: swc1        $f6, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->f6.u32l;
    // 0x801CC720: lhu         $t4, 0xC($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0XC);
    // 0x801CC724: andi        $t5, $t4, 0xFFF7
    ctx->r13 = ctx->r12 & 0XFFF7;
    // 0x801CC728: sh          $t5, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r13;
L_801CC72C:
    // 0x801CC72C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CC730: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801CC734: jr          $ra
    // 0x801CC738: nop

    return;
    // 0x801CC738: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CC73C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC73C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801CC740: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CC744: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801CC748: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801CC74C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x801CC750: addiu       $at, $zero, -0x701
    ctx->r1 = ADD32(0, -0X701);
    // 0x801CC754: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x801CC758: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x801CC75C: lhu         $t0, 0x14($v1)
    ctx->r8 = MEM_HU(ctx->r3, 0X14);
    // 0x801CC760: lbu         $t4, 0x58($v1)
    ctx->r12 = MEM_BU(ctx->r3, 0X58);
    // 0x801CC764: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x801CC768: andi        $t2, $t0, 0xFFF1
    ctx->r10 = ctx->r8 & 0XFFF1;
    // 0x801CC76C: ori         $t9, $t7, 0x100
    ctx->r25 = ctx->r15 | 0X100;
    // 0x801CC770: ori         $t3, $t2, 0x4
    ctx->r11 = ctx->r10 | 0X4;
    // 0x801CC774: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x801CC778: sh          $t2, 0x14($v1)
    MEM_H(0X14, ctx->r3) = ctx->r10;
    // 0x801CC77C: andi        $t5, $t4, 0x2
    ctx->r13 = ctx->r12 & 0X2;
    // 0x801CC780: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x801CC784: beq         $t5, $zero, L_801CC7AC
    if (ctx->r13 == 0) {
        // 0x801CC788: sh          $t3, 0x14($v1)
        MEM_H(0X14, ctx->r3) = ctx->r11;
            goto L_801CC7AC;
    }
    // 0x801CC788: sh          $t3, 0x14($v1)
    MEM_H(0X14, ctx->r3) = ctx->r11;
    // 0x801CC78C: addiu       $s0, $v1, 0x58
    ctx->r16 = ADD32(ctx->r3, 0X58);
    // 0x801CC790: lbu         $t6, 0x0($s0)
    ctx->r14 = MEM_BU(ctx->r16, 0X0);
    // 0x801CC794: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x801CC798: sll         $t8, $t7, 3
    ctx->r24 = S32(ctx->r15 << 3);
    // 0x801CC79C: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x801CC7A0: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x801CC7A4: addu        $t9, $s0, $t8
    ctx->r25 = ADD32(ctx->r16, ctx->r24);
    // 0x801CC7A8: sw          $zero, 0x14($t9)
    MEM_W(0X14, ctx->r25) = 0;
L_801CC7AC:
    // 0x801CC7AC: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x801CC7B0: addiu       $v0, $v0, 0x35F0
    ctx->r2 = ADD32(ctx->r2, 0X35F0);
    // 0x801CC7B4: lhu         $t0, 0x4($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X4);
    // 0x801CC7B8: addiu       $s0, $v1, 0x58
    ctx->r16 = ADD32(ctx->r3, 0X58);
    // 0x801CC7BC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC7C0: beq         $t0, $zero, L_801CC804
    if (ctx->r8 == 0) {
        // 0x801CC7C4: lui         $a1, 0x800B
        ctx->r5 = S32(0X800B << 16);
            goto L_801CC804;
    }
    // 0x801CC7C4: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CC7C8: lhu         $t1, 0x6($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0X6);
    // 0x801CC7CC: addiu       $a2, $zero, 0x6A
    ctx->r6 = ADD32(0, 0X6A);
    // 0x801CC7D0: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CC7D4: andi        $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 & 0X2000;
    // 0x801CC7D8: beq         $t2, $zero, L_801CC804
    if (ctx->r10 == 0) {
        // 0x801CC7DC: lui         $t9, 0x8002
        ctx->r25 = S32(0X8002 << 16);
            goto L_801CC804;
    }
    // 0x801CC7DC: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC7E0: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CC7E4: addiu       $t9, $t9, -0x754
    ctx->r25 = ADD32(ctx->r25, -0X754);
    // 0x801CC7E8: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CC7EC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC7F0: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CC7F4: jalr        $t9
    // 0x801CC7F8: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CC7F8: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x801CC7FC: b           L_801CC828
    // 0x801CC800: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
        goto L_801CC828;
    // 0x801CC800: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_801CC804:
    // 0x801CC804: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC808: addiu       $t9, $t9, -0x754
    ctx->r25 = ADD32(ctx->r25, -0X754);
    // 0x801CC80C: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CC810: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CC814: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CC818: addiu       $a2, $zero, 0x68
    ctx->r6 = ADD32(0, 0X68);
    // 0x801CC81C: jalr        $t9
    // 0x801CC820: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CC820: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_1:
    // 0x801CC824: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_801CC828:
    // 0x801CC828: beq         $v0, $zero, L_801CC84C
    if (ctx->r2 == 0) {
        // 0x801CC82C: addiu       $a0, $s1, 0x8
        ctx->r4 = ADD32(ctx->r17, 0X8);
            goto L_801CC84C;
    }
    // 0x801CC82C: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x801CC830: lhu         $t3, 0x16($v1)
    ctx->r11 = MEM_HU(ctx->r3, 0X16);
    // 0x801CC834: andi        $t4, $t3, 0x400
    ctx->r12 = ctx->r11 & 0X400;
    // 0x801CC838: beql        $t4, $zero, L_801CC850
    if (ctx->r12 == 0) {
        // 0x801CC83C: lbu         $t7, 0x0($s0)
        ctx->r15 = MEM_BU(ctx->r16, 0X0);
            goto L_801CC850;
    }
    goto skip_0;
    // 0x801CC83C: lbu         $t7, 0x0($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X0);
    skip_0:
    // 0x801CC840: lbu         $t5, 0x0($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X0);
    // 0x801CC844: ori         $t6, $t5, 0x10
    ctx->r14 = ctx->r13 | 0X10;
    // 0x801CC848: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
L_801CC84C:
    // 0x801CC84C: lbu         $t7, 0x0($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X0);
L_801CC850:
    // 0x801CC850: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CC854: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CC858: andi        $t8, $t7, 0xFFFD
    ctx->r24 = ctx->r15 & 0XFFFD;
    // 0x801CC85C: sb          $t8, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r24;
    // 0x801CC860: jalr        $t9
    // 0x801CC864: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CC864: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_2:
    // 0x801CC868: lh          $t0, 0xE($s1)
    ctx->r8 = MEM_H(ctx->r17, 0XE);
    // 0x801CC86C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CC870: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CC874: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x801CC878: addu        $v0, $s1, $t1
    ctx->r2 = ADD32(ctx->r17, ctx->r9);
    // 0x801CC87C: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x801CC880: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x801CC884: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC888: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x801CC88C: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x801CC890: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x801CC894: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x801CC898: lw          $t9, 0x7BF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7BF0);
    // 0x801CC89C: jalr        $t9
    // 0x801CC8A0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CC8A0: nop

    after_3:
    // 0x801CC8A4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801CC8A8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x801CC8AC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801CC8B0: jr          $ra
    // 0x801CC8B4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x801CC8B4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void overlay_system_1_func_801CC8B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC8B8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801CC8BC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CC8C0: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x801CC8C4: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC8C8: addiu       $t9, $t9, 0x1860
    ctx->r25 = ADD32(ctx->r25, 0X1860);
    // 0x801CC8CC: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x801CC8D0: jalr        $t9
    // 0x801CC8D4: sw          $t6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CC8D4: sw          $t6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r14;
    after_0:
    // 0x801CC8D8: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x801CC8DC: addiu       $at, $zero, 0xF1
    ctx->r1 = ADD32(0, 0XF1);
    // 0x801CC8E0: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CC8E4: lh          $v1, 0xEA($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XEA);
    // 0x801CC8E8: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x801CC8EC: addiu       $t9, $t9, 0x4BD8
    ctx->r25 = ADD32(ctx->r25, 0X4BD8);
    // 0x801CC8F0: bne         $v1, $at, L_801CC908
    if (ctx->r3 != ctx->r1) {
        // 0x801CC8F4: addiu       $v0, $v0, 0x8
        ctx->r2 = ADD32(ctx->r2, 0X8);
            goto L_801CC908;
    }
    // 0x801CC8F4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CC8F8: lhu         $t7, 0xC($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0XC);
    // 0x801CC8FC: lh          $v1, 0xE2($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XE2);
    // 0x801CC900: andi        $t8, $t7, 0xF7FF
    ctx->r24 = ctx->r15 & 0XF7FF;
    // 0x801CC904: sh          $t8, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r24;
L_801CC908:
    // 0x801CC908: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x801CC90C: bne         $v1, $at, L_801CC924
    if (ctx->r3 != ctx->r1) {
        // 0x801CC910: nop

            goto L_801CC924;
    }
    // 0x801CC910: nop

    // 0x801CC914: lw          $t0, 0x0($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X0);
    // 0x801CC918: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x801CC91C: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x801CC920: sw          $t1, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r9;
L_801CC924:
    // 0x801CC924: jalr        $t9
    // 0x801CC928: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CC928: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    after_1:
    // 0x801CC92C: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x801CC930: lw          $t2, 0x0($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X0);
    // 0x801CC934: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801CC938: bgezl       $t3, L_801CC950
    if (SIGNED(ctx->r11) >= 0) {
        // 0x801CC93C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801CC950;
    }
    goto skip_0;
    // 0x801CC93C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801CC940: lhu         $t4, 0xC($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0XC);
    // 0x801CC944: andi        $t5, $t4, 0xFFFB
    ctx->r13 = ctx->r12 & 0XFFFB;
    // 0x801CC948: sh          $t5, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r13;
    // 0x801CC94C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CC950:
    // 0x801CC950: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801CC954: jr          $ra
    // 0x801CC958: nop

    return;
    // 0x801CC958: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CC95C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CC95C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801CC960: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CC964: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801CC968: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801CC96C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x801CC970: addiu       $at, $zero, -0x701
    ctx->r1 = ADD32(0, -0X701);
    // 0x801CC974: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x801CC978: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x801CC97C: lhu         $t0, 0x14($v1)
    ctx->r8 = MEM_HU(ctx->r3, 0X14);
    // 0x801CC980: lbu         $t4, 0x58($v1)
    ctx->r12 = MEM_BU(ctx->r3, 0X58);
    // 0x801CC984: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x801CC988: andi        $t2, $t0, 0xFFF1
    ctx->r10 = ctx->r8 & 0XFFF1;
    // 0x801CC98C: ori         $t9, $t7, 0x400
    ctx->r25 = ctx->r15 | 0X400;
    // 0x801CC990: ori         $t3, $t2, 0x2
    ctx->r11 = ctx->r10 | 0X2;
    // 0x801CC994: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x801CC998: sh          $t2, 0x14($v1)
    MEM_H(0X14, ctx->r3) = ctx->r10;
    // 0x801CC99C: andi        $t5, $t4, 0x2
    ctx->r13 = ctx->r12 & 0X2;
    // 0x801CC9A0: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x801CC9A4: beq         $t5, $zero, L_801CC9CC
    if (ctx->r13 == 0) {
        // 0x801CC9A8: sh          $t3, 0x14($v1)
        MEM_H(0X14, ctx->r3) = ctx->r11;
            goto L_801CC9CC;
    }
    // 0x801CC9A8: sh          $t3, 0x14($v1)
    MEM_H(0X14, ctx->r3) = ctx->r11;
    // 0x801CC9AC: addiu       $s0, $v1, 0x58
    ctx->r16 = ADD32(ctx->r3, 0X58);
    // 0x801CC9B0: lbu         $t6, 0x0($s0)
    ctx->r14 = MEM_BU(ctx->r16, 0X0);
    // 0x801CC9B4: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x801CC9B8: sll         $t8, $t7, 3
    ctx->r24 = S32(ctx->r15 << 3);
    // 0x801CC9BC: subu        $t8, $t8, $t7
    ctx->r24 = SUB32(ctx->r24, ctx->r15);
    // 0x801CC9C0: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x801CC9C4: addu        $t9, $s0, $t8
    ctx->r25 = ADD32(ctx->r16, ctx->r24);
    // 0x801CC9C8: sw          $zero, 0x14($t9)
    MEM_W(0X14, ctx->r25) = 0;
L_801CC9CC:
    // 0x801CC9CC: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x801CC9D0: addiu       $v0, $v0, 0x35F0
    ctx->r2 = ADD32(ctx->r2, 0X35F0);
    // 0x801CC9D4: lhu         $t0, 0x4($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X4);
    // 0x801CC9D8: addiu       $s0, $v1, 0x58
    ctx->r16 = ADD32(ctx->r3, 0X58);
    // 0x801CC9DC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CC9E0: beq         $t0, $zero, L_801CCA24
    if (ctx->r8 == 0) {
        // 0x801CC9E4: lui         $a1, 0x800B
        ctx->r5 = S32(0X800B << 16);
            goto L_801CCA24;
    }
    // 0x801CC9E4: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CC9E8: lhu         $t1, 0x6($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0X6);
    // 0x801CC9EC: addiu       $a2, $zero, 0x6C
    ctx->r6 = ADD32(0, 0X6C);
    // 0x801CC9F0: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CC9F4: andi        $t2, $t1, 0x2000
    ctx->r10 = ctx->r9 & 0X2000;
    // 0x801CC9F8: beq         $t2, $zero, L_801CCA24
    if (ctx->r10 == 0) {
        // 0x801CC9FC: lui         $t9, 0x8002
        ctx->r25 = S32(0X8002 << 16);
            goto L_801CCA24;
    }
    // 0x801CC9FC: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CCA00: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CCA04: addiu       $t9, $t9, -0x754
    ctx->r25 = ADD32(ctx->r25, -0X754);
    // 0x801CCA08: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CCA0C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CCA10: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CCA14: jalr        $t9
    // 0x801CCA18: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CCA18: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x801CCA1C: b           L_801CCA48
    // 0x801CCA20: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
        goto L_801CCA48;
    // 0x801CCA20: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_801CCA24:
    // 0x801CCA24: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CCA28: addiu       $t9, $t9, -0x754
    ctx->r25 = ADD32(ctx->r25, -0X754);
    // 0x801CCA2C: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CCA30: lw          $a3, -0x7D34($a3)
    ctx->r7 = MEM_W(ctx->r7, -0X7D34);
    // 0x801CCA34: addiu       $a1, $a1, 0x36A0
    ctx->r5 = ADD32(ctx->r5, 0X36A0);
    // 0x801CCA38: addiu       $a2, $zero, 0x6B
    ctx->r6 = ADD32(0, 0X6B);
    // 0x801CCA3C: jalr        $t9
    // 0x801CCA40: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CCA40: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_1:
    // 0x801CCA44: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
L_801CCA48:
    // 0x801CCA48: beq         $v0, $zero, L_801CCA6C
    if (ctx->r2 == 0) {
        // 0x801CCA4C: addiu       $a0, $s1, 0x8
        ctx->r4 = ADD32(ctx->r17, 0X8);
            goto L_801CCA6C;
    }
    // 0x801CCA4C: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x801CCA50: lhu         $t3, 0x16($v1)
    ctx->r11 = MEM_HU(ctx->r3, 0X16);
    // 0x801CCA54: andi        $t4, $t3, 0x40
    ctx->r12 = ctx->r11 & 0X40;
    // 0x801CCA58: beql        $t4, $zero, L_801CCA70
    if (ctx->r12 == 0) {
        // 0x801CCA5C: lbu         $t7, 0x0($s0)
        ctx->r15 = MEM_BU(ctx->r16, 0X0);
            goto L_801CCA70;
    }
    goto skip_0;
    // 0x801CCA5C: lbu         $t7, 0x0($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X0);
    skip_0:
    // 0x801CCA60: lbu         $t5, 0x0($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X0);
    // 0x801CCA64: ori         $t6, $t5, 0x10
    ctx->r14 = ctx->r13 | 0X10;
    // 0x801CCA68: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
L_801CCA6C:
    // 0x801CCA6C: lbu         $t7, 0x0($s0)
    ctx->r15 = MEM_BU(ctx->r16, 0X0);
L_801CCA70:
    // 0x801CCA70: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CCA74: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CCA78: andi        $t8, $t7, 0xFFFD
    ctx->r24 = ctx->r15 & 0XFFFD;
    // 0x801CCA7C: sb          $t8, 0x0($s0)
    MEM_B(0X0, ctx->r16) = ctx->r24;
    // 0x801CCA80: jalr        $t9
    // 0x801CCA84: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CCA84: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_2:
    // 0x801CCA88: lh          $t0, 0xE($s1)
    ctx->r8 = MEM_H(ctx->r17, 0XE);
    // 0x801CCA8C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CCA90: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CCA94: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x801CCA98: addu        $v0, $s1, $t1
    ctx->r2 = ADD32(ctx->r17, ctx->r9);
    // 0x801CCA9C: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x801CCAA0: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x801CCAA4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CCAA8: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x801CCAAC: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x801CCAB0: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x801CCAB4: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x801CCAB8: lw          $t9, 0x7BF0($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7BF0);
    // 0x801CCABC: jalr        $t9
    // 0x801CCAC0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CCAC0: nop

    after_3:
    // 0x801CCAC4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801CCAC8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x801CCACC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801CCAD0: jr          $ra
    // 0x801CCAD4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x801CCAD4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void overlay_system_1_func_801CCAD8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CCAD8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801CCADC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CCAE0: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x801CCAE4: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CCAE8: addiu       $t9, $t9, 0x1860
    ctx->r25 = ADD32(ctx->r25, 0X1860);
    // 0x801CCAEC: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x801CCAF0: jalr        $t9
    // 0x801CCAF4: sw          $t6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CCAF4: sw          $t6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r14;
    after_0:
    // 0x801CCAF8: lw          $v0, 0x2C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X2C);
    // 0x801CCAFC: addiu       $at, $zero, 0xF1
    ctx->r1 = ADD32(0, 0XF1);
    // 0x801CCB00: lui         $t9, 0x8002
    ctx->r25 = S32(0X8002 << 16);
    // 0x801CCB04: lh          $v1, 0xEA($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XEA);
    // 0x801CCB08: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x801CCB0C: addiu       $t9, $t9, 0x4BD8
    ctx->r25 = ADD32(ctx->r25, 0X4BD8);
    // 0x801CCB10: bne         $v1, $at, L_801CCB28
    if (ctx->r3 != ctx->r1) {
        // 0x801CCB14: addiu       $v0, $v0, 0x8
        ctx->r2 = ADD32(ctx->r2, 0X8);
            goto L_801CCB28;
    }
    // 0x801CCB14: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CCB18: lhu         $t7, 0xC($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0XC);
    // 0x801CCB1C: lh          $v1, 0xE2($v0)
    ctx->r3 = MEM_H(ctx->r2, 0XE2);
    // 0x801CCB20: andi        $t8, $t7, 0xFF7F
    ctx->r24 = ctx->r15 & 0XFF7F;
    // 0x801CCB24: sh          $t8, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r24;
L_801CCB28:
    // 0x801CCB28: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x801CCB2C: bne         $v1, $at, L_801CCB44
    if (ctx->r3 != ctx->r1) {
        // 0x801CCB30: nop

            goto L_801CCB44;
    }
    // 0x801CCB30: nop

    // 0x801CCB34: lw          $t0, 0x0($v0)
    ctx->r8 = MEM_W(ctx->r2, 0X0);
    // 0x801CCB38: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x801CCB3C: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x801CCB40: sw          $t1, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r9;
L_801CCB44:
    // 0x801CCB44: jalr        $t9
    // 0x801CCB48: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CCB48: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    after_1:
    // 0x801CCB4C: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x801CCB50: lw          $t2, 0x0($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X0);
    // 0x801CCB54: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x801CCB58: bgezl       $t3, L_801CCB70
    if (SIGNED(ctx->r11) >= 0) {
        // 0x801CCB5C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801CCB70;
    }
    goto skip_0;
    // 0x801CCB5C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801CCB60: lhu         $t4, 0xC($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0XC);
    // 0x801CCB64: andi        $t5, $t4, 0xFFFD
    ctx->r13 = ctx->r12 & 0XFFFD;
    // 0x801CCB68: sh          $t5, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r13;
    // 0x801CCB6C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CCB70:
    // 0x801CCB70: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801CCB74: jr          $ra
    // 0x801CCB78: nop

    return;
    // 0x801CCB78: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CCB7C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CCB7C: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801CCB80: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CCB84: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x801CCB88: lw          $t0, 0x34($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X34);
    // 0x801CCB8C: or          $t1, $zero, $zero
    ctx->r9 = 0 | 0;
    // 0x801CCB90: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CCB94: lhu         $t7, 0x14($t0)
    ctx->r15 = MEM_HU(ctx->r8, 0X14);
    // 0x801CCB98: addiu       $a2, $t0, 0x8
    ctx->r6 = ADD32(ctx->r8, 0X8);
    // 0x801CCB9C: addiu       $a3, $t0, 0x2D4
    ctx->r7 = ADD32(ctx->r8, 0X2D4);
    // 0x801CCBA0: andi        $t8, $t7, 0x8880
    ctx->r24 = ctx->r15 & 0X8880;
    // 0x801CCBA4: beql        $t8, $zero, L_801CCBB8
    if (ctx->r24 == 0) {
        // 0x801CCBA8: lhu         $t9, 0xC($a2)
        ctx->r25 = MEM_HU(ctx->r6, 0XC);
            goto L_801CCBB8;
    }
    goto skip_0;
    // 0x801CCBA8: lhu         $t9, 0xC($a2)
    ctx->r25 = MEM_HU(ctx->r6, 0XC);
    skip_0:
    // 0x801CCBAC: b           L_801CD0E8
    // 0x801CCBB0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CD0E8;
    // 0x801CCBB0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801CCBB4: lhu         $t9, 0xC($a2)
    ctx->r25 = MEM_HU(ctx->r6, 0XC);
L_801CCBB8:
    // 0x801CCBB8: addiu       $at, $zero, -0x3331
    ctx->r1 = ADD32(0, -0X3331);
    // 0x801CCBBC: addiu       $a0, $a0, 0x35F0
    ctx->r4 = ADD32(ctx->r4, 0X35F0);
    // 0x801CCBC0: and         $t2, $t9, $at
    ctx->r10 = ctx->r25 & ctx->r1;
    // 0x801CCBC4: sh          $t2, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r10;
    // 0x801CCBC8: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
    // 0x801CCBCC: beq         $v0, $zero, L_801CCBF4
    if (ctx->r2 == 0) {
        // 0x801CCBD0: nop

            goto L_801CCBF4;
    }
    // 0x801CCBD0: nop

    // 0x801CCBD4: lhu         $t3, 0x8($a0)
    ctx->r11 = MEM_HU(ctx->r4, 0X8);
    // 0x801CCBD8: ori         $t6, $t2, 0x2000
    ctx->r14 = ctx->r10 | 0X2000;
    // 0x801CCBDC: andi        $t4, $t3, 0x4000
    ctx->r12 = ctx->r11 & 0X4000;
    // 0x801CCBE0: beq         $t4, $zero, L_801CCBF4
    if (ctx->r12 == 0) {
        // 0x801CCBE4: nop

            goto L_801CCBF4;
    }
    // 0x801CCBE4: nop

    // 0x801CCBE8: sh          $t6, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r14;
    // 0x801CCBEC: b           L_801CCC38
    // 0x801CCBF0: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
        goto L_801CCC38;
    // 0x801CCBF0: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
L_801CCBF4:
    // 0x801CCBF4: beql        $v0, $zero, L_801CCC10
    if (ctx->r2 == 0) {
        // 0x801CCBF8: lhu         $t9, 0x12($a0)
        ctx->r25 = MEM_HU(ctx->r4, 0X12);
            goto L_801CCC10;
    }
    goto skip_1;
    // 0x801CCBF8: lhu         $t9, 0x12($a0)
    ctx->r25 = MEM_HU(ctx->r4, 0X12);
    skip_1:
    // 0x801CCBFC: lhu         $t7, 0x6($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X6);
    // 0x801CCC00: andi        $t8, $t7, 0x4000
    ctx->r24 = ctx->r15 & 0X4000;
    // 0x801CCC04: bne         $t8, $zero, L_801CCC38
    if (ctx->r24 != 0) {
        // 0x801CCC08: nop

            goto L_801CCC38;
    }
    // 0x801CCC08: nop

    // 0x801CCC0C: lhu         $t9, 0x12($a0)
    ctx->r25 = MEM_HU(ctx->r4, 0X12);
L_801CCC10:
    // 0x801CCC10: beq         $t9, $zero, L_801CCC38
    if (ctx->r25 == 0) {
        // 0x801CCC14: nop

            goto L_801CCC38;
    }
    // 0x801CCC14: nop

    // 0x801CCC18: lhu         $t2, 0x14($a0)
    ctx->r10 = MEM_HU(ctx->r4, 0X14);
    // 0x801CCC1C: andi        $t3, $t2, 0x4000
    ctx->r11 = ctx->r10 & 0X4000;
    // 0x801CCC20: beq         $t3, $zero, L_801CCC38
    if (ctx->r11 == 0) {
        // 0x801CCC24: nop

            goto L_801CCC38;
    }
    // 0x801CCC24: nop

    // 0x801CCC28: lhu         $t4, 0xC($a2)
    ctx->r12 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCC2C: ori         $t5, $t4, 0x1000
    ctx->r13 = ctx->r12 | 0X1000;
    // 0x801CCC30: sh          $t5, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r13;
    // 0x801CCC34: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
L_801CCC38:
    // 0x801CCC38: beq         $v0, $zero, L_801CCC64
    if (ctx->r2 == 0) {
        // 0x801CCC3C: nop

            goto L_801CCC64;
    }
    // 0x801CCC3C: nop

    // 0x801CCC40: lhu         $t6, 0x8($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X8);
    // 0x801CCC44: andi        $t7, $t6, 0x2
    ctx->r15 = ctx->r14 & 0X2;
    // 0x801CCC48: beq         $t7, $zero, L_801CCC64
    if (ctx->r15 == 0) {
        // 0x801CCC4C: nop

            goto L_801CCC64;
    }
    // 0x801CCC4C: nop

    // 0x801CCC50: lhu         $t8, 0xC($a2)
    ctx->r24 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCC54: ori         $t9, $t8, 0x200
    ctx->r25 = ctx->r24 | 0X200;
    // 0x801CCC58: sh          $t9, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r25;
    // 0x801CCC5C: b           L_801CCCA8
    // 0x801CCC60: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
        goto L_801CCCA8;
    // 0x801CCC60: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
L_801CCC64:
    // 0x801CCC64: beql        $v0, $zero, L_801CCC80
    if (ctx->r2 == 0) {
        // 0x801CCC68: lhu         $t4, 0x12($a0)
        ctx->r12 = MEM_HU(ctx->r4, 0X12);
            goto L_801CCC80;
    }
    goto skip_2;
    // 0x801CCC68: lhu         $t4, 0x12($a0)
    ctx->r12 = MEM_HU(ctx->r4, 0X12);
    skip_2:
    // 0x801CCC6C: lhu         $t2, 0x6($a0)
    ctx->r10 = MEM_HU(ctx->r4, 0X6);
    // 0x801CCC70: andi        $t3, $t2, 0x2
    ctx->r11 = ctx->r10 & 0X2;
    // 0x801CCC74: bne         $t3, $zero, L_801CCCA8
    if (ctx->r11 != 0) {
        // 0x801CCC78: nop

            goto L_801CCCA8;
    }
    // 0x801CCC78: nop

    // 0x801CCC7C: lhu         $t4, 0x12($a0)
    ctx->r12 = MEM_HU(ctx->r4, 0X12);
L_801CCC80:
    // 0x801CCC80: beq         $t4, $zero, L_801CCCA8
    if (ctx->r12 == 0) {
        // 0x801CCC84: nop

            goto L_801CCCA8;
    }
    // 0x801CCC84: nop

    // 0x801CCC88: lhu         $t5, 0x14($a0)
    ctx->r13 = MEM_HU(ctx->r4, 0X14);
    // 0x801CCC8C: andi        $t6, $t5, 0x2
    ctx->r14 = ctx->r13 & 0X2;
    // 0x801CCC90: beq         $t6, $zero, L_801CCCA8
    if (ctx->r14 == 0) {
        // 0x801CCC94: nop

            goto L_801CCCA8;
    }
    // 0x801CCC94: nop

    // 0x801CCC98: lhu         $t7, 0xC($a2)
    ctx->r15 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCC9C: ori         $t8, $t7, 0x100
    ctx->r24 = ctx->r15 | 0X100;
    // 0x801CCCA0: sh          $t8, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r24;
    // 0x801CCCA4: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
L_801CCCA8:
    // 0x801CCCA8: beq         $v0, $zero, L_801CCCD0
    if (ctx->r2 == 0) {
        // 0x801CCCAC: nop

            goto L_801CCCD0;
    }
    // 0x801CCCAC: nop

    // 0x801CCCB0: lhu         $t9, 0x8($a0)
    ctx->r25 = MEM_HU(ctx->r4, 0X8);
    // 0x801CCCB4: andi        $t2, $t9, 0x4
    ctx->r10 = ctx->r25 & 0X4;
    // 0x801CCCB8: beq         $t2, $zero, L_801CCCD0
    if (ctx->r10 == 0) {
        // 0x801CCCBC: nop

            goto L_801CCCD0;
    }
    // 0x801CCCBC: nop

    // 0x801CCCC0: lhu         $t3, 0xC($a2)
    ctx->r11 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCCC4: ori         $t4, $t3, 0x20
    ctx->r12 = ctx->r11 | 0X20;
    // 0x801CCCC8: b           L_801CCD10
    // 0x801CCCCC: sh          $t4, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r12;
        goto L_801CCD10;
    // 0x801CCCCC: sh          $t4, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r12;
L_801CCCD0:
    // 0x801CCCD0: beql        $v0, $zero, L_801CCCEC
    if (ctx->r2 == 0) {
        // 0x801CCCD4: lhu         $t7, 0x12($a0)
        ctx->r15 = MEM_HU(ctx->r4, 0X12);
            goto L_801CCCEC;
    }
    goto skip_3;
    // 0x801CCCD4: lhu         $t7, 0x12($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X12);
    skip_3:
    // 0x801CCCD8: lhu         $t5, 0x6($a0)
    ctx->r13 = MEM_HU(ctx->r4, 0X6);
    // 0x801CCCDC: andi        $t6, $t5, 0x4
    ctx->r14 = ctx->r13 & 0X4;
    // 0x801CCCE0: bnel        $t6, $zero, L_801CCD14
    if (ctx->r14 != 0) {
        // 0x801CCCE4: lhu         $v1, 0xC($a2)
        ctx->r3 = MEM_HU(ctx->r6, 0XC);
            goto L_801CCD14;
    }
    goto skip_4;
    // 0x801CCCE4: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    skip_4:
    // 0x801CCCE8: lhu         $t7, 0x12($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X12);
L_801CCCEC:
    // 0x801CCCEC: beql        $t7, $zero, L_801CCD14
    if (ctx->r15 == 0) {
        // 0x801CCCF0: lhu         $v1, 0xC($a2)
        ctx->r3 = MEM_HU(ctx->r6, 0XC);
            goto L_801CCD14;
    }
    goto skip_5;
    // 0x801CCCF0: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    skip_5:
    // 0x801CCCF4: lhu         $t8, 0x14($a0)
    ctx->r24 = MEM_HU(ctx->r4, 0X14);
    // 0x801CCCF8: andi        $t9, $t8, 0x4
    ctx->r25 = ctx->r24 & 0X4;
    // 0x801CCCFC: beql        $t9, $zero, L_801CCD14
    if (ctx->r25 == 0) {
        // 0x801CCD00: lhu         $v1, 0xC($a2)
        ctx->r3 = MEM_HU(ctx->r6, 0XC);
            goto L_801CCD14;
    }
    goto skip_6;
    // 0x801CCD00: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    skip_6:
    // 0x801CCD04: lhu         $t2, 0xC($a2)
    ctx->r10 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCD08: ori         $t3, $t2, 0x10
    ctx->r11 = ctx->r10 | 0X10;
    // 0x801CCD0C: sh          $t3, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r11;
L_801CCD10:
    // 0x801CCD10: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
L_801CCD14:
    // 0x801CCD14: andi        $t4, $v1, 0x8
    ctx->r12 = ctx->r3 & 0X8;
    // 0x801CCD18: beq         $t4, $zero, L_801CCD80
    if (ctx->r12 == 0) {
        // 0x801CCD1C: andi        $t5, $v1, 0x4000
        ctx->r13 = ctx->r3 & 0X4000;
            goto L_801CCD80;
    }
    // 0x801CCD1C: andi        $t5, $v1, 0x4000
    ctx->r13 = ctx->r3 & 0X4000;
    // 0x801CCD20: beq         $t5, $zero, L_801CCD30
    if (ctx->r13 == 0) {
        // 0x801CCD24: andi        $t6, $v1, 0xEFFF
        ctx->r14 = ctx->r3 & 0XEFFF;
            goto L_801CCD30;
    }
    // 0x801CCD24: andi        $t6, $v1, 0xEFFF
    ctx->r14 = ctx->r3 & 0XEFFF;
    // 0x801CCD28: sh          $t6, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r14;
    // 0x801CCD2C: andi        $v1, $t6, 0xFFFF
    ctx->r3 = ctx->r14 & 0XFFFF;
L_801CCD30:
    // 0x801CCD30: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
    // 0x801CCD34: beq         $v0, $zero, L_801CCD5C
    if (ctx->r2 == 0) {
        // 0x801CCD38: nop

            goto L_801CCD5C;
    }
    // 0x801CCD38: nop

    // 0x801CCD3C: lhu         $t7, 0x6($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X6);
    // 0x801CCD40: ori         $t9, $v1, 0x200
    ctx->r25 = ctx->r3 | 0X200;
    // 0x801CCD44: andi        $t8, $t7, 0x2
    ctx->r24 = ctx->r15 & 0X2;
    // 0x801CCD48: beq         $t8, $zero, L_801CCD5C
    if (ctx->r24 == 0) {
        // 0x801CCD4C: nop

            goto L_801CCD5C;
    }
    // 0x801CCD4C: nop

    // 0x801CCD50: sh          $t9, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r25;
    // 0x801CCD54: lhu         $v0, 0x4($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X4);
    // 0x801CCD58: andi        $v1, $t9, 0xFFFF
    ctx->r3 = ctx->r25 & 0XFFFF;
L_801CCD5C:
    // 0x801CCD5C: beql        $v0, $zero, L_801CCD84
    if (ctx->r2 == 0) {
        // 0x801CCD60: andi        $t5, $v1, 0x4
        ctx->r13 = ctx->r3 & 0X4;
            goto L_801CCD84;
    }
    goto skip_7;
    // 0x801CCD60: andi        $t5, $v1, 0x4
    ctx->r13 = ctx->r3 & 0X4;
    skip_7:
    // 0x801CCD64: lhu         $t2, 0x6($a0)
    ctx->r10 = MEM_HU(ctx->r4, 0X6);
    // 0x801CCD68: ori         $t4, $v1, 0x20
    ctx->r12 = ctx->r3 | 0X20;
    // 0x801CCD6C: andi        $t3, $t2, 0x4
    ctx->r11 = ctx->r10 & 0X4;
    // 0x801CCD70: beql        $t3, $zero, L_801CCD84
    if (ctx->r11 == 0) {
        // 0x801CCD74: andi        $t5, $v1, 0x4
        ctx->r13 = ctx->r3 & 0X4;
            goto L_801CCD84;
    }
    goto skip_8;
    // 0x801CCD74: andi        $t5, $v1, 0x4
    ctx->r13 = ctx->r3 & 0X4;
    skip_8:
    // 0x801CCD78: sh          $t4, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r12;
    // 0x801CCD7C: andi        $v1, $t4, 0xFFFF
    ctx->r3 = ctx->r12 & 0XFFFF;
L_801CCD80:
    // 0x801CCD80: andi        $t5, $v1, 0x4
    ctx->r13 = ctx->r3 & 0X4;
L_801CCD84:
    // 0x801CCD84: beq         $t5, $zero, L_801CCD9C
    if (ctx->r13 == 0) {
        // 0x801CCD88: andi        $t6, $v1, 0x400
        ctx->r14 = ctx->r3 & 0X400;
            goto L_801CCD9C;
    }
    // 0x801CCD88: andi        $t6, $v1, 0x400
    ctx->r14 = ctx->r3 & 0X400;
    // 0x801CCD8C: beq         $t6, $zero, L_801CCD9C
    if (ctx->r14 == 0) {
        // 0x801CCD90: andi        $t7, $v1, 0xFDFF
        ctx->r15 = ctx->r3 & 0XFDFF;
            goto L_801CCD9C;
    }
    // 0x801CCD90: andi        $t7, $v1, 0xFDFF
    ctx->r15 = ctx->r3 & 0XFDFF;
    // 0x801CCD94: sh          $t7, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r15;
    // 0x801CCD98: andi        $v1, $t7, 0xFFFF
    ctx->r3 = ctx->r15 & 0XFFFF;
L_801CCD9C:
    // 0x801CCD9C: andi        $t8, $v1, 0x2
    ctx->r24 = ctx->r3 & 0X2;
    // 0x801CCDA0: beq         $t8, $zero, L_801CCDE4
    if (ctx->r24 == 0) {
        // 0x801CCDA4: lui         $v0, 0x801D
        ctx->r2 = S32(0X801D << 16);
            goto L_801CCDE4;
    }
    // 0x801CCDA4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CCDA8: lhu         $t9, 0x4($a0)
    ctx->r25 = MEM_HU(ctx->r4, 0X4);
    // 0x801CCDAC: beql        $t9, $zero, L_801CCDD4
    if (ctx->r25 == 0) {
        // 0x801CCDB0: andi        $t5, $v1, 0x40
        ctx->r13 = ctx->r3 & 0X40;
            goto L_801CCDD4;
    }
    goto skip_9;
    // 0x801CCDB0: andi        $t5, $v1, 0x40
    ctx->r13 = ctx->r3 & 0X40;
    skip_9:
    // 0x801CCDB4: lhu         $t2, 0x6($a0)
    ctx->r10 = MEM_HU(ctx->r4, 0X6);
    // 0x801CCDB8: ori         $t4, $v1, 0x200
    ctx->r12 = ctx->r3 | 0X200;
    // 0x801CCDBC: andi        $t3, $t2, 0x2
    ctx->r11 = ctx->r10 & 0X2;
    // 0x801CCDC0: beql        $t3, $zero, L_801CCDD4
    if (ctx->r11 == 0) {
        // 0x801CCDC4: andi        $t5, $v1, 0x40
        ctx->r13 = ctx->r3 & 0X40;
            goto L_801CCDD4;
    }
    goto skip_10;
    // 0x801CCDC4: andi        $t5, $v1, 0x40
    ctx->r13 = ctx->r3 & 0X40;
    skip_10:
    // 0x801CCDC8: sh          $t4, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r12;
    // 0x801CCDCC: andi        $v1, $t4, 0xFFFF
    ctx->r3 = ctx->r12 & 0XFFFF;
    // 0x801CCDD0: andi        $t5, $v1, 0x40
    ctx->r13 = ctx->r3 & 0X40;
L_801CCDD4:
    // 0x801CCDD4: beq         $t5, $zero, L_801CCDE4
    if (ctx->r13 == 0) {
        // 0x801CCDD8: andi        $t6, $v1, 0xFFDF
        ctx->r14 = ctx->r3 & 0XFFDF;
            goto L_801CCDE4;
    }
    // 0x801CCDD8: andi        $t6, $v1, 0xFFDF
    ctx->r14 = ctx->r3 & 0XFFDF;
    // 0x801CCDDC: sh          $t6, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r14;
    // 0x801CCDE0: andi        $v1, $t6, 0xFFFF
    ctx->r3 = ctx->r14 & 0XFFFF;
L_801CCDE4:
    // 0x801CCDE4: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x801CCDE8: andi        $t9, $v1, 0xEFFF
    ctx->r25 = ctx->r3 & 0XEFFF;
    // 0x801CCDEC: sll         $t8, $t7, 5
    ctx->r24 = S32(ctx->r15 << 5);
    // 0x801CCDF0: bgezl       $t8, L_801CCE04
    if (SIGNED(ctx->r24) >= 0) {
        // 0x801CCDF4: lw          $t2, 0x0($a3)
        ctx->r10 = MEM_W(ctx->r7, 0X0);
            goto L_801CCE04;
    }
    goto skip_11;
    // 0x801CCDF4: lw          $t2, 0x0($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X0);
    skip_11:
    // 0x801CCDF8: sh          $t9, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r25;
    // 0x801CCDFC: andi        $v1, $t9, 0xFFFF
    ctx->r3 = ctx->r25 & 0XFFFF;
    // 0x801CCE00: lw          $t2, 0x0($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X0);
L_801CCE04:
    // 0x801CCE04: andi        $t7, $v1, 0xEFFF
    ctx->r15 = ctx->r3 & 0XEFFF;
    // 0x801CCE08: sll         $t3, $t2, 3
    ctx->r11 = S32(ctx->r10 << 3);
    // 0x801CCE0C: addu        $t4, $a3, $t3
    ctx->r12 = ADD32(ctx->r7, ctx->r11);
    // 0x801CCE10: lw          $t5, 0x4($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X4);
    // 0x801CCE14: sll         $t6, $t5, 1
    ctx->r14 = S32(ctx->r13 << 1);
    // 0x801CCE18: bltzl       $t6, L_801CCE2C
    if (SIGNED(ctx->r14) < 0) {
        // 0x801CCE1C: andi        $t8, $v1, 0x20
        ctx->r24 = ctx->r3 & 0X20;
            goto L_801CCE2C;
    }
    goto skip_12;
    // 0x801CCE1C: andi        $t8, $v1, 0x20
    ctx->r24 = ctx->r3 & 0X20;
    skip_12:
    // 0x801CCE20: sh          $t7, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r15;
    // 0x801CCE24: andi        $v1, $t7, 0xFFFF
    ctx->r3 = ctx->r15 & 0XFFFF;
    // 0x801CCE28: andi        $t8, $v1, 0x20
    ctx->r24 = ctx->r3 & 0X20;
L_801CCE2C:
    // 0x801CCE2C: beql        $t8, $zero, L_801CCE98
    if (ctx->r24 == 0) {
        // 0x801CCE30: lw          $t6, 0x40($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X40);
            goto L_801CCE98;
    }
    goto skip_13;
    // 0x801CCE30: lw          $t6, 0x40($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X40);
    skip_13:
    // 0x801CCE34: lh          $v0, -0x54C2($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X54C2);
    // 0x801CCE38: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    // 0x801CCE3C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CCE40: beq         $v0, $zero, L_801CCE8C
    if (ctx->r2 == 0) {
        // 0x801CCE44: sll         $t3, $v0, 1
        ctx->r11 = S32(ctx->r2 << 1);
            goto L_801CCE8C;
    }
    // 0x801CCE44: sll         $t3, $v0, 1
    ctx->r11 = S32(ctx->r2 << 1);
    // 0x801CCE48: lw          $t9, -0x51E8($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X51E8);
    // 0x801CCE4C: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x801CCE50: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x801CCE54: sll         $t2, $t9, 3
    ctx->r10 = S32(ctx->r25 << 3);
    // 0x801CCE58: addu        $t4, $t2, $t3
    ctx->r12 = ADD32(ctx->r10, ctx->r11);
    // 0x801CCE5C: addu        $a1, $a1, $t4
    ctx->r5 = ADD32(ctx->r5, ctx->r12);
    // 0x801CCE60: lh          $a1, 0x4566($a1)
    ctx->r5 = MEM_H(ctx->r5, 0X4566);
    // 0x801CCE64: sw          $a3, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r7;
    // 0x801CCE68: sw          $t0, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r8;
    // 0x801CCE6C: jal         0x8008FCA4
    // 0x801CCE70: sh          $t1, 0x26($sp)
    MEM_H(0X26, ctx->r29) = ctx->r9;
    func_8008FCA4(rdram, ctx);
        goto after_0;
    // 0x801CCE70: sh          $t1, 0x26($sp)
    MEM_H(0X26, ctx->r29) = ctx->r9;
    after_0:
    // 0x801CCE74: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x801CCE78: lw          $a3, 0x1C($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X1C);
    // 0x801CCE7C: lw          $t0, 0x3C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X3C);
    // 0x801CCE80: bgez        $v0, L_801CCE94
    if (SIGNED(ctx->r2) >= 0) {
        // 0x801CCE84: lhu         $t1, 0x26($sp)
        ctx->r9 = MEM_HU(ctx->r29, 0X26);
            goto L_801CCE94;
    }
    // 0x801CCE84: lhu         $t1, 0x26($sp)
    ctx->r9 = MEM_HU(ctx->r29, 0X26);
    // 0x801CCE88: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
L_801CCE8C:
    // 0x801CCE8C: andi        $t5, $v1, 0xFFDF
    ctx->r13 = ctx->r3 & 0XFFDF;
    // 0x801CCE90: sh          $t5, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r13;
L_801CCE94:
    // 0x801CCE94: lw          $t6, 0x40($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X40);
L_801CCE98:
    // 0x801CCE98: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x801CCE9C: lbu         $v0, 0x9($t6)
    ctx->r2 = MEM_BU(ctx->r14, 0X9);
    // 0x801CCEA0: bnel        $v0, $at, L_801CCF38
    if (ctx->r2 != ctx->r1) {
        // 0x801CCEA4: addiu       $at, $zero, 0x7
        ctx->r1 = ADD32(0, 0X7);
            goto L_801CCF38;
    }
    goto skip_14;
    // 0x801CCEA4: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    skip_14:
    // 0x801CCEA8: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCEAC: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x801CCEB0: andi        $t7, $v1, 0x1000
    ctx->r15 = ctx->r3 & 0X1000;
    // 0x801CCEB4: beql        $t7, $zero, L_801CCF04
    if (ctx->r15 == 0) {
        // 0x801CCEB8: andi        $t7, $v1, 0x200
        ctx->r15 = ctx->r3 & 0X200;
            goto L_801CCF04;
    }
    goto skip_15;
    // 0x801CCEB8: andi        $t7, $v1, 0x200
    ctx->r15 = ctx->r3 & 0X200;
    skip_15:
    // 0x801CCEBC: lh          $t8, -0x525C($t8)
    ctx->r24 = MEM_H(ctx->r24, -0X525C);
    // 0x801CCEC0: ori         $t6, $v1, 0x8000
    ctx->r14 = ctx->r3 | 0X8000;
    // 0x801CCEC4: beql        $t8, $zero, L_801CCEF4
    if (ctx->r24 == 0) {
        // 0x801CCEC8: sh          $t6, 0xC($a2)
        MEM_H(0XC, ctx->r6) = ctx->r14;
            goto L_801CCEF4;
    }
    goto skip_16;
    // 0x801CCEC8: sh          $t6, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r14;
    skip_16:
    // 0x801CCECC: lw          $t9, 0x0($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X0);
    // 0x801CCED0: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x801CCED4: sll         $t2, $t9, 3
    ctx->r10 = S32(ctx->r25 << 3);
    // 0x801CCED8: addu        $t3, $a3, $t2
    ctx->r11 = ADD32(ctx->r7, ctx->r10);
    // 0x801CCEDC: lw          $v0, 0x8($t3)
    ctx->r2 = MEM_W(ctx->r11, 0X8);
    // 0x801CCEE0: lw          $t4, 0x5C($v0)
    ctx->r12 = MEM_W(ctx->r2, 0X5C);
    // 0x801CCEE4: or          $t5, $t4, $at
    ctx->r13 = ctx->r12 | ctx->r1;
    // 0x801CCEE8: b           L_801CCEF4
    // 0x801CCEEC: sw          $t5, 0x5C($v0)
    MEM_W(0X5C, ctx->r2) = ctx->r13;
        goto L_801CCEF4;
    // 0x801CCEEC: sw          $t5, 0x5C($v0)
    MEM_W(0X5C, ctx->r2) = ctx->r13;
    // 0x801CCEF0: sh          $t6, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r14;
L_801CCEF4:
    // 0x801CCEF4: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCEF8: b           L_801CD0E8
    // 0x801CCEFC: andi        $v0, $v1, 0x8880
    ctx->r2 = ctx->r3 & 0X8880;
        goto L_801CD0E8;
    // 0x801CCEFC: andi        $v0, $v1, 0x8880
    ctx->r2 = ctx->r3 & 0X8880;
    // 0x801CCF00: andi        $t7, $v1, 0x200
    ctx->r15 = ctx->r3 & 0X200;
L_801CCF04:
    // 0x801CCF04: beq         $t7, $zero, L_801CCF1C
    if (ctx->r15 == 0) {
        // 0x801CCF08: andi        $t9, $v1, 0x20
        ctx->r25 = ctx->r3 & 0X20;
            goto L_801CCF1C;
    }
    // 0x801CCF08: andi        $t9, $v1, 0x20
    ctx->r25 = ctx->r3 & 0X20;
    // 0x801CCF0C: ori         $t8, $v1, 0x800
    ctx->r24 = ctx->r3 | 0X800;
    // 0x801CCF10: sh          $t8, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r24;
    // 0x801CCF14: b           L_801CD0E8
    // 0x801CCF18: andi        $v0, $t8, 0x8880
    ctx->r2 = ctx->r24 & 0X8880;
        goto L_801CD0E8;
    // 0x801CCF18: andi        $v0, $t8, 0x8880
    ctx->r2 = ctx->r24 & 0X8880;
L_801CCF1C:
    // 0x801CCF1C: beq         $t9, $zero, L_801CCF2C
    if (ctx->r25 == 0) {
        // 0x801CCF20: ori         $t2, $v1, 0x80
        ctx->r10 = ctx->r3 | 0X80;
            goto L_801CCF2C;
    }
    // 0x801CCF20: ori         $t2, $v1, 0x80
    ctx->r10 = ctx->r3 | 0X80;
    // 0x801CCF24: sh          $t2, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r10;
    // 0x801CCF28: andi        $v1, $t2, 0xFFFF
    ctx->r3 = ctx->r10 & 0XFFFF;
L_801CCF2C:
    // 0x801CCF2C: b           L_801CD0E8
    // 0x801CCF30: andi        $v0, $v1, 0x8880
    ctx->r2 = ctx->r3 & 0X8880;
        goto L_801CD0E8;
    // 0x801CCF30: andi        $v0, $v1, 0x8880
    ctx->r2 = ctx->r3 & 0X8880;
    // 0x801CCF34: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
L_801CCF38:
    // 0x801CCF38: bnel        $v0, $at, L_801CCF90
    if (ctx->r2 != ctx->r1) {
        // 0x801CCF3C: addiu       $at, $zero, 0x9
        ctx->r1 = ADD32(0, 0X9);
            goto L_801CCF90;
    }
    goto skip_17;
    // 0x801CCF3C: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
    skip_17:
    // 0x801CCF40: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCF44: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801CCF48: andi        $t3, $v1, 0x1000
    ctx->r11 = ctx->r3 & 0X1000;
    // 0x801CCF4C: beq         $t3, $zero, L_801CCF60
    if (ctx->r11 == 0) {
        // 0x801CCF50: andi        $t5, $v1, 0x200
        ctx->r13 = ctx->r3 & 0X200;
            goto L_801CCF60;
    }
    // 0x801CCF50: andi        $t5, $v1, 0x200
    ctx->r13 = ctx->r3 & 0X200;
    // 0x801CCF54: ori         $t4, $v1, 0x8000
    ctx->r12 = ctx->r3 | 0X8000;
    // 0x801CCF58: b           L_801CD0E8
    // 0x801CCF5C: sh          $t4, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r12;
        goto L_801CD0E8;
    // 0x801CCF5C: sh          $t4, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r12;
L_801CCF60:
    // 0x801CCF60: beq         $t5, $zero, L_801CCF78
    if (ctx->r13 == 0) {
        // 0x801CCF64: andi        $t7, $v1, 0x20
        ctx->r15 = ctx->r3 & 0X20;
            goto L_801CCF78;
    }
    // 0x801CCF64: andi        $t7, $v1, 0x20
    ctx->r15 = ctx->r3 & 0X20;
    // 0x801CCF68: ori         $t6, $v1, 0x800
    ctx->r14 = ctx->r3 | 0X800;
    // 0x801CCF6C: sh          $t6, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r14;
    // 0x801CCF70: b           L_801CD0E8
    // 0x801CCF74: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CD0E8;
    // 0x801CCF74: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801CCF78:
    // 0x801CCF78: beq         $t7, $zero, L_801CCF84
    if (ctx->r15 == 0) {
        // 0x801CCF7C: ori         $t8, $v1, 0x80
        ctx->r24 = ctx->r3 | 0X80;
            goto L_801CCF84;
    }
    // 0x801CCF7C: ori         $t8, $v1, 0x80
    ctx->r24 = ctx->r3 | 0X80;
    // 0x801CCF80: sh          $t8, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r24;
L_801CCF84:
    // 0x801CCF84: b           L_801CD0E8
    // 0x801CCF88: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_801CD0E8;
    // 0x801CCF88: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801CCF8C: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
L_801CCF90:
    // 0x801CCF90: bne         $v0, $at, L_801CCFFC
    if (ctx->r2 != ctx->r1) {
        // 0x801CCF94: lw          $t9, 0x40($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X40);
            goto L_801CCFFC;
    }
    // 0x801CCF94: lw          $t9, 0x40($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X40);
    // 0x801CCF98: lbu         $v0, 0xB($t9)
    ctx->r2 = MEM_BU(ctx->r25, 0XB);
    // 0x801CCF9C: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    // 0x801CCFA0: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x801CCFA4: bne         $v0, $at, L_801CCFD8
    if (ctx->r2 != ctx->r1) {
        // 0x801CCFA8: andi        $t2, $v1, 0x200
        ctx->r10 = ctx->r3 & 0X200;
            goto L_801CCFD8;
    }
    // 0x801CCFA8: andi        $t2, $v1, 0x200
    ctx->r10 = ctx->r3 & 0X200;
    // 0x801CCFAC: beq         $t2, $zero, L_801CCFC4
    if (ctx->r10 == 0) {
        // 0x801CCFB0: andi        $t4, $v1, 0x20
        ctx->r12 = ctx->r3 & 0X20;
            goto L_801CCFC4;
    }
    // 0x801CCFB0: andi        $t4, $v1, 0x20
    ctx->r12 = ctx->r3 & 0X20;
    // 0x801CCFB4: ori         $t3, $v1, 0x800
    ctx->r11 = ctx->r3 | 0X800;
    // 0x801CCFB8: sh          $t3, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r11;
    // 0x801CCFBC: b           L_801CD0E8
    // 0x801CCFC0: andi        $v0, $t3, 0x8880
    ctx->r2 = ctx->r11 & 0X8880;
        goto L_801CD0E8;
    // 0x801CCFC0: andi        $v0, $t3, 0x8880
    ctx->r2 = ctx->r11 & 0X8880;
L_801CCFC4:
    // 0x801CCFC4: beq         $t4, $zero, L_801CCFF4
    if (ctx->r12 == 0) {
        // 0x801CCFC8: ori         $t5, $v1, 0x80
        ctx->r13 = ctx->r3 | 0X80;
            goto L_801CCFF4;
    }
    // 0x801CCFC8: ori         $t5, $v1, 0x80
    ctx->r13 = ctx->r3 | 0X80;
    // 0x801CCFCC: sh          $t5, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r13;
    // 0x801CCFD0: b           L_801CD0E8
    // 0x801CCFD4: andi        $v0, $t5, 0x8880
    ctx->r2 = ctx->r13 & 0X8880;
        goto L_801CD0E8;
    // 0x801CCFD4: andi        $v0, $t5, 0x8880
    ctx->r2 = ctx->r13 & 0X8880;
L_801CCFD8:
    // 0x801CCFD8: addiu       $at, $zero, 0x6
    ctx->r1 = ADD32(0, 0X6);
    // 0x801CCFDC: bne         $v0, $at, L_801CCFF4
    if (ctx->r2 != ctx->r1) {
        // 0x801CCFE0: andi        $t6, $v1, 0x200
        ctx->r14 = ctx->r3 & 0X200;
            goto L_801CCFF4;
    }
    // 0x801CCFE0: andi        $t6, $v1, 0x200
    ctx->r14 = ctx->r3 & 0X200;
    // 0x801CCFE4: beq         $t6, $zero, L_801CCFF4
    if (ctx->r14 == 0) {
        // 0x801CCFE8: ori         $t7, $v1, 0x800
        ctx->r15 = ctx->r3 | 0X800;
            goto L_801CCFF4;
    }
    // 0x801CCFE8: ori         $t7, $v1, 0x800
    ctx->r15 = ctx->r3 | 0X800;
    // 0x801CCFEC: sh          $t7, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r15;
    // 0x801CCFF0: andi        $v1, $t7, 0xFFFF
    ctx->r3 = ctx->r15 & 0XFFFF;
L_801CCFF4:
    // 0x801CCFF4: b           L_801CD0E8
    // 0x801CCFF8: andi        $v0, $v1, 0x8880
    ctx->r2 = ctx->r3 & 0X8880;
        goto L_801CD0E8;
    // 0x801CCFF8: andi        $v0, $v1, 0x8880
    ctx->r2 = ctx->r3 & 0X8880;
L_801CCFFC:
    // 0x801CCFFC: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x801CD000: bnel        $v0, $at, L_801CD0C8
    if (ctx->r2 != ctx->r1) {
        // 0x801CD004: addiu       $at, $zero, 0x4
        ctx->r1 = ADD32(0, 0X4);
            goto L_801CD0C8;
    }
    goto skip_18;
    // 0x801CD004: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    skip_18:
    // 0x801CD008: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    // 0x801CD00C: andi        $t8, $v1, 0x1000
    ctx->r24 = ctx->r3 & 0X1000;
    // 0x801CD010: beq         $t8, $zero, L_801CD038
    if (ctx->r24 == 0) {
        // 0x801CD014: andi        $t4, $v1, 0x200
        ctx->r12 = ctx->r3 & 0X200;
            goto L_801CD038;
    }
    // 0x801CD014: andi        $t4, $v1, 0x200
    ctx->r12 = ctx->r3 & 0X200;
    // 0x801CD018: lw          $t9, 0x0($t0)
    ctx->r25 = MEM_W(ctx->r8, 0X0);
    // 0x801CD01C: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
    // 0x801CD020: ori         $t3, $v1, 0x8000
    ctx->r11 = ctx->r3 | 0X8000;
    // 0x801CD024: andi        $t2, $t9, 0x9
    ctx->r10 = ctx->r25 & 0X9;
    // 0x801CD028: beq         $t2, $at, L_801CD038
    if (ctx->r10 == ctx->r1) {
        // 0x801CD02C: nop

            goto L_801CD038;
    }
    // 0x801CD02C: nop

    // 0x801CD030: b           L_801CD070
    // 0x801CD034: sh          $t3, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r11;
        goto L_801CD070;
    // 0x801CD034: sh          $t3, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r11;
L_801CD038:
    // 0x801CD038: beq         $t4, $zero, L_801CD04C
    if (ctx->r12 == 0) {
        // 0x801CD03C: andi        $t6, $v1, 0x20
        ctx->r14 = ctx->r3 & 0X20;
            goto L_801CD04C;
    }
    // 0x801CD03C: andi        $t6, $v1, 0x20
    ctx->r14 = ctx->r3 & 0X20;
    // 0x801CD040: ori         $t5, $v1, 0x800
    ctx->r13 = ctx->r3 | 0X800;
    // 0x801CD044: b           L_801CD070
    // 0x801CD048: sh          $t5, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r13;
        goto L_801CD070;
    // 0x801CD048: sh          $t5, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r13;
L_801CD04C:
    // 0x801CD04C: beql        $t6, $zero, L_801CD074
    if (ctx->r14 == 0) {
        // 0x801CD050: lhu         $v0, 0xE($a2)
        ctx->r2 = MEM_HU(ctx->r6, 0XE);
            goto L_801CD074;
    }
    goto skip_19;
    // 0x801CD050: lhu         $v0, 0xE($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0XE);
    skip_19:
    // 0x801CD054: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x801CD058: addiu       $at, $zero, 0x9
    ctx->r1 = ADD32(0, 0X9);
    // 0x801CD05C: ori         $t9, $v1, 0x80
    ctx->r25 = ctx->r3 | 0X80;
    // 0x801CD060: andi        $t8, $t7, 0x9
    ctx->r24 = ctx->r15 & 0X9;
    // 0x801CD064: beql        $t8, $at, L_801CD074
    if (ctx->r24 == ctx->r1) {
        // 0x801CD068: lhu         $v0, 0xE($a2)
        ctx->r2 = MEM_HU(ctx->r6, 0XE);
            goto L_801CD074;
    }
    goto skip_20;
    // 0x801CD068: lhu         $v0, 0xE($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0XE);
    skip_20:
    // 0x801CD06C: sh          $t9, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r25;
L_801CD070:
    // 0x801CD070: lhu         $v0, 0xE($a2)
    ctx->r2 = MEM_HU(ctx->r6, 0XE);
L_801CD074:
    // 0x801CD074: andi        $t2, $v0, 0x8000
    ctx->r10 = ctx->r2 & 0X8000;
    // 0x801CD078: beq         $t2, $zero, L_801CD08C
    if (ctx->r10 == 0) {
        // 0x801CD07C: andi        $t3, $v0, 0x800
        ctx->r11 = ctx->r2 & 0X800;
            goto L_801CD08C;
    }
    // 0x801CD07C: andi        $t3, $v0, 0x800
    ctx->r11 = ctx->r2 & 0X800;
    // 0x801CD080: lhu         $t1, 0xC($a2)
    ctx->r9 = MEM_HU(ctx->r6, 0XC);
    // 0x801CD084: andi        $t1, $t1, 0x8000
    ctx->r9 = ctx->r9 & 0X8000;
    // 0x801CD088: andi        $t1, $t1, 0xFFFF
    ctx->r9 = ctx->r9 & 0XFFFF;
L_801CD08C:
    // 0x801CD08C: beq         $t3, $zero, L_801CD0A4
    if (ctx->r11 == 0) {
        // 0x801CD090: andi        $t6, $v0, 0x80
        ctx->r14 = ctx->r2 & 0X80;
            goto L_801CD0A4;
    }
    // 0x801CD090: andi        $t6, $v0, 0x80
    ctx->r14 = ctx->r2 & 0X80;
    // 0x801CD094: lhu         $t4, 0xC($a2)
    ctx->r12 = MEM_HU(ctx->r6, 0XC);
    // 0x801CD098: andi        $t5, $t4, 0x800
    ctx->r13 = ctx->r12 & 0X800;
    // 0x801CD09C: or          $t1, $t1, $t5
    ctx->r9 = ctx->r9 | ctx->r13;
    // 0x801CD0A0: andi        $t1, $t1, 0xFFFF
    ctx->r9 = ctx->r9 & 0XFFFF;
L_801CD0A4:
    // 0x801CD0A4: beq         $t6, $zero, L_801CD0BC
    if (ctx->r14 == 0) {
        // 0x801CD0A8: nop

            goto L_801CD0BC;
    }
    // 0x801CD0A8: nop

    // 0x801CD0AC: lhu         $t7, 0xC($a2)
    ctx->r15 = MEM_HU(ctx->r6, 0XC);
    // 0x801CD0B0: andi        $t8, $t7, 0x80
    ctx->r24 = ctx->r15 & 0X80;
    // 0x801CD0B4: or          $t1, $t1, $t8
    ctx->r9 = ctx->r9 | ctx->r24;
    // 0x801CD0B8: andi        $t1, $t1, 0xFFFF
    ctx->r9 = ctx->r9 & 0XFFFF;
L_801CD0BC:
    // 0x801CD0BC: b           L_801CD0E8
    // 0x801CD0C0: or          $v0, $t1, $zero
    ctx->r2 = ctx->r9 | 0;
        goto L_801CD0E8;
    // 0x801CD0C0: or          $v0, $t1, $zero
    ctx->r2 = ctx->r9 | 0;
    // 0x801CD0C4: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
L_801CD0C8:
    // 0x801CD0C8: bnel        $v0, $at, L_801CD0E8
    if (ctx->r2 != ctx->r1) {
        // 0x801CD0CC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_801CD0E8;
    }
    goto skip_21;
    // 0x801CD0CC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_21:
    // 0x801CD0D0: lhu         $v1, 0xC($a2)
    ctx->r3 = MEM_HU(ctx->r6, 0XC);
    // 0x801CD0D4: andi        $t9, $v1, 0x200
    ctx->r25 = ctx->r3 & 0X200;
    // 0x801CD0D8: beq         $t9, $zero, L_801CD0E4
    if (ctx->r25 == 0) {
        // 0x801CD0DC: ori         $t2, $v1, 0x800
        ctx->r10 = ctx->r3 | 0X800;
            goto L_801CD0E4;
    }
    // 0x801CD0DC: ori         $t2, $v1, 0x800
    ctx->r10 = ctx->r3 | 0X800;
    // 0x801CD0E0: sh          $t2, 0xC($a2)
    MEM_H(0XC, ctx->r6) = ctx->r10;
L_801CD0E4:
    // 0x801CD0E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_801CD0E8:
    // 0x801CD0E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CD0EC: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x801CD0F0: jr          $ra
    // 0x801CD0F4: nop

    return;
    // 0x801CD0F4: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD0F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD0F8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CD0FC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CD100: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x801CD104: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801CD108: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801CD10C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801CD110: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x801CD114: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x801CD118: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801CD11C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801CD120: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801CD124: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801CD128: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801CD12C: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801CD130: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801CD134: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CD138: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801CD13C: lw          $t9, 0x7C0C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C0C);
    // 0x801CD140: jalr        $t9
    // 0x801CD144: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CD144: nop

    after_0:
    // 0x801CD148: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801CD14C: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801CD150: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801CD154: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801CD158: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CD15C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CD160: jr          $ra
    // 0x801CD164: nop

    return;
    // 0x801CD164: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD168(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD168: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801CD16C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CD170: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x801CD174: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD178: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x801CD17C: andi        $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 & 0X1000;
    // 0x801CD180: beq         $t7, $zero, L_801CD248
    if (ctx->r15 == 0) {
        // 0x801CD184: addiu       $v1, $v1, -0x7D40
        ctx->r3 = ADD32(ctx->r3, -0X7D40);
            goto L_801CD248;
    }
    // 0x801CD184: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801CD188: lw          $t0, 0x2968($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X2968);
    // 0x801CD18C: lw          $t8, 0x2C8($t0)
    ctx->r24 = MEM_W(ctx->r8, 0X2C8);
    // 0x801CD190: sll         $t9, $t8, 0
    ctx->r25 = S32(ctx->r24 << 0);
    // 0x801CD194: bgezl       $t9, L_801CD1B0
    if (SIGNED(ctx->r25) >= 0) {
        // 0x801CD198: lw          $v0, 0x2BC8($v1)
        ctx->r2 = MEM_W(ctx->r3, 0X2BC8);
            goto L_801CD1B0;
    }
    goto skip_0;
    // 0x801CD198: lw          $v0, 0x2BC8($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X2BC8);
    skip_0:
    // 0x801CD19C: jal         0x801CDA44
    // 0x801CD1A0: nop

    overlay_system_1_func_801CDA44(rdram, ctx);
        goto after_0;
    // 0x801CD1A0: nop

    after_0:
    // 0x801CD1A4: b           L_801CD2A8
    // 0x801CD1A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801CD2A8;
    // 0x801CD1A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CD1AC: lw          $v0, 0x2BC8($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X2BC8);
L_801CD1B0:
    // 0x801CD1B0: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x801CD1B4: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x801CD1B8: beq         $v0, $zero, L_801CD234
    if (ctx->r2 == 0) {
        // 0x801CD1BC: nop

            goto L_801CD234;
    }
    // 0x801CD1BC: nop

    // 0x801CD1C0: lw          $t1, 0x7C40($t1)
    ctx->r9 = MEM_W(ctx->r9, 0X7C40);
    // 0x801CD1C4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD1C8: addiu       $a0, $a3, 0x8
    ctx->r4 = ADD32(ctx->r7, 0X8);
    // 0x801CD1CC: bne         $t1, $zero, L_801CD1F8
    if (ctx->r9 != 0) {
        // 0x801CD1D0: addiu       $a1, $a3, 0xE
        ctx->r5 = ADD32(ctx->r7, 0XE);
            goto L_801CD1F8;
    }
    // 0x801CD1D0: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x801CD1D4: sw          $v0, 0x7C40($at)
    MEM_W(0X7C40, ctx->r1) = ctx->r2;
    // 0x801CD1D8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD1DC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD1E0: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801CD1E4: sh          $zero, 0x7C1C($at)
    MEM_H(0X7C1C, ctx->r1) = 0;
    // 0x801CD1E8: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    // 0x801CD1EC: jalr        $t9
    // 0x801CD1F0: sw          $t0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CD1F0: sw          $t0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r8;
    after_1:
    // 0x801CD1F4: lw          $t0, 0x18($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X18);
L_801CD1F8:
    // 0x801CD1F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801CD1FC: addiu       $v1, $t0, 0x2CC
    ctx->r3 = ADD32(ctx->r8, 0X2CC);
    // 0x801CD200: addiu       $a3, $zero, 0x10
    ctx->r7 = ADD32(0, 0X10);
    // 0x801CD204: lui         $a2, 0x4000
    ctx->r6 = S32(0X4000 << 16);
L_801CD208:
    // 0x801CD208: lw          $a0, 0x8($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X8);
    // 0x801CD20C: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CD210: lw          $a1, 0x5C($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X5C);
    // 0x801CD214: andi        $t2, $a1, 0x1
    ctx->r10 = ctx->r5 & 0X1;
    // 0x801CD218: beq         $t2, $zero, L_801CD224
    if (ctx->r10 == 0) {
        // 0x801CD21C: or          $t3, $a1, $a2
        ctx->r11 = ctx->r5 | ctx->r6;
            goto L_801CD224;
    }
    // 0x801CD21C: or          $t3, $a1, $a2
    ctx->r11 = ctx->r5 | ctx->r6;
    // 0x801CD220: sw          $t3, 0x5C($a0)
    MEM_W(0X5C, ctx->r4) = ctx->r11;
L_801CD224:
    // 0x801CD224: bne         $v0, $a3, L_801CD208
    if (ctx->r2 != ctx->r7) {
        // 0x801CD228: addiu       $v1, $v1, 0x8
        ctx->r3 = ADD32(ctx->r3, 0X8);
            goto L_801CD208;
    }
    // 0x801CD228: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x801CD22C: b           L_801CD2A8
    // 0x801CD230: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801CD2A8;
    // 0x801CD230: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CD234:
    // 0x801CD234: lw          $t4, 0x7C40($t4)
    ctx->r12 = MEM_W(ctx->r12, 0X7C40);
    // 0x801CD238: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD23C: beql        $t4, $zero, L_801CD24C
    if (ctx->r12 == 0) {
        // 0x801CD240: lh          $v1, 0xE($a3)
        ctx->r3 = MEM_H(ctx->r7, 0XE);
            goto L_801CD24C;
    }
    goto skip_1;
    // 0x801CD240: lh          $v1, 0xE($a3)
    ctx->r3 = MEM_H(ctx->r7, 0XE);
    skip_1:
    // 0x801CD244: sw          $zero, 0x7C40($at)
    MEM_W(0X7C40, ctx->r1) = 0;
L_801CD248:
    // 0x801CD248: lh          $v1, 0xE($a3)
    ctx->r3 = MEM_H(ctx->r7, 0XE);
L_801CD24C:
    // 0x801CD24C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CD250: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x801CD254: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801CD258: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801CD25C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801CD260: sll         $t5, $v1, 1
    ctx->r13 = S32(ctx->r3 << 1);
    // 0x801CD264: addu        $v0, $a3, $t5
    ctx->r2 = ADD32(ctx->r7, ctx->r13);
    // 0x801CD268: sh          $v1, 0xE($a3)
    MEM_H(0XE, ctx->r7) = ctx->r3;
    // 0x801CD26C: lbu         $t6, 0x8($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X8);
    // 0x801CD270: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x801CD274: sb          $t7, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r15;
    // 0x801CD278: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    // 0x801CD27C: lbu         $t8, 0x9($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X9);
    // 0x801CD280: sll         $t1, $t8, 2
    ctx->r9 = S32(ctx->r24 << 2);
    // 0x801CD284: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x801CD288: lw          $t9, 0x7C28($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C28);
    // 0x801CD28C: jalr        $t9
    // 0x801CD290: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CD290: nop

    after_2:
    // 0x801CD294: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x801CD298: lh          $t2, 0xE($a3)
    ctx->r10 = MEM_H(ctx->r7, 0XE);
    // 0x801CD29C: addiu       $t3, $t2, -0x1
    ctx->r11 = ADD32(ctx->r10, -0X1);
    // 0x801CD2A0: sh          $t3, 0xE($a3)
    MEM_H(0XE, ctx->r7) = ctx->r11;
    // 0x801CD2A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CD2A8:
    // 0x801CD2A8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801CD2AC: jr          $ra
    // 0x801CD2B0: nop

    return;
    // 0x801CD2B0: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD2B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD2B4: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x801CD2B8: sw          $s3, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r19;
    // 0x801CD2BC: lui         $s3, 0x801D
    ctx->r19 = S32(0X801D << 16);
    // 0x801CD2C0: addiu       $s3, $s3, -0x7D40
    ctx->r19 = ADD32(ctx->r19, -0X7D40);
    // 0x801CD2C4: lw          $v0, 0x2968($s3)
    ctx->r2 = MEM_W(ctx->r19, 0X2968);
    // 0x801CD2C8: sw          $s4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r20;
    // 0x801CD2CC: sw          $s2, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r18;
    // 0x801CD2D0: sdc1        $f20, 0x40($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X40, ctx->r29);
    // 0x801CD2D4: sw          $ra, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r31;
    // 0x801CD2D8: sw          $s6, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r22;
    // 0x801CD2DC: sw          $s5, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r21;
    // 0x801CD2E0: sw          $s1, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r17;
    // 0x801CD2E4: sw          $s0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r16;
    // 0x801CD2E8: sdc1        $f22, 0x48($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X48, ctx->r29);
    // 0x801CD2EC: lui         $s2, 0x8007
    ctx->r18 = S32(0X8007 << 16);
    // 0x801CD2F0: lui         $s4, 0x801D
    ctx->r20 = S32(0X801D << 16);
    // 0x801CD2F4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD2F8: sw          $zero, 0x2CC($v0)
    MEM_W(0X2CC, ctx->r2) = 0;
    // 0x801CD2FC: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x801CD300: or          $s6, $a0, $zero
    ctx->r22 = ctx->r4 | 0;
    // 0x801CD304: lwc1        $f22, 0x7D38($at)
    ctx->f22.u32l = MEM_W(ctx->r1, 0X7D38);
    // 0x801CD308: addiu       $s4, $s4, 0x7C44
    ctx->r20 = ADD32(ctx->r20, 0X7C44);
    // 0x801CD30C: addiu       $s2, $s2, -0x5298
    ctx->r18 = ADD32(ctx->r18, -0X5298);
    // 0x801CD310: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x801CD314: addiu       $s5, $zero, 0x10
    ctx->r21 = ADD32(0, 0X10);
    // 0x801CD318: addiu       $s0, $v0, 0x2CC
    ctx->r16 = ADD32(ctx->r2, 0X2CC);
L_801CD31C:
    // 0x801CD31C: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
    // 0x801CD320: lw          $t6, 0x2B58($s3)
    ctx->r14 = MEM_W(ctx->r19, 0X2B58);
    // 0x801CD324: mfc1        $a3, $f22
    ctx->r7 = (int32_t)ctx->f22.u32l;
    // 0x801CD328: addiu       $t8, $zero, 0x71C
    ctx->r24 = ADD32(0, 0X71C);
    // 0x801CD32C: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x801CD330: addu        $v0, $s4, $t7
    ctx->r2 = ADD32(ctx->r20, ctx->r15);
    // 0x801CD334: addiu       $t9, $zero, 0x5DC
    ctx->r25 = ADD32(0, 0X5DC);
    // 0x801CD338: sw          $t9, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r25;
    // 0x801CD33C: lw          $a1, 0x0($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X0);
    // 0x801CD340: lw          $a2, 0x4($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X4);
    // 0x801CD344: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    // 0x801CD348: sw          $zero, 0x34($sp)
    MEM_W(0X34, ctx->r29) = 0;
    // 0x801CD34C: sw          $zero, 0x30($sp)
    MEM_W(0X30, ctx->r29) = 0;
    // 0x801CD350: sw          $zero, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = 0;
    // 0x801CD354: sw          $zero, 0x28($sp)
    MEM_W(0X28, ctx->r29) = 0;
    // 0x801CD358: swc1        $f20, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f20.u32l;
    // 0x801CD35C: swc1        $f20, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f20.u32l;
    // 0x801CD360: swc1        $f20, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f20.u32l;
    // 0x801CD364: sw          $zero, 0x18($sp)
    MEM_W(0X18, ctx->r29) = 0;
    // 0x801CD368: jalr        $s2
    // 0x801CD36C: addiu       $a0, $zero, 0x128
    ctx->r4 = ADD32(0, 0X128);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_0;
    // 0x801CD36C: addiu       $a0, $zero, 0x128
    ctx->r4 = ADD32(0, 0X128);
    after_0:
    // 0x801CD370: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
    // 0x801CD374: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    // 0x801CD378: bne         $s1, $s5, L_801CD31C
    if (ctx->r17 != ctx->r21) {
        // 0x801CD37C: sw          $v0, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r2;
            goto L_801CD31C;
    }
    // 0x801CD37C: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
    // 0x801CD380: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD384: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CD388: addiu       $a0, $s6, 0x8
    ctx->r4 = ADD32(ctx->r22, 0X8);
    // 0x801CD38C: jalr        $t9
    // 0x801CD390: addiu       $a1, $s6, 0xE
    ctx->r5 = ADD32(ctx->r22, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CD390: addiu       $a1, $s6, 0xE
    ctx->r5 = ADD32(ctx->r22, 0XE);
    after_1:
    // 0x801CD394: lh          $t0, 0xE($s6)
    ctx->r8 = MEM_H(ctx->r22, 0XE);
    // 0x801CD398: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CD39C: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x801CD3A0: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x801CD3A4: addu        $v0, $s6, $t1
    ctx->r2 = ADD32(ctx->r22, ctx->r9);
    // 0x801CD3A8: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x801CD3AC: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x801CD3B0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CD3B4: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x801CD3B8: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x801CD3BC: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x801CD3C0: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x801CD3C4: lw          $t9, 0x7C28($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C28);
    // 0x801CD3C8: jalr        $t9
    // 0x801CD3CC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CD3CC: nop

    after_2:
    // 0x801CD3D0: lw          $ra, 0x6C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X6C);
    // 0x801CD3D4: ldc1        $f20, 0x40($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X40);
    // 0x801CD3D8: ldc1        $f22, 0x48($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X48);
    // 0x801CD3DC: lw          $s0, 0x50($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X50);
    // 0x801CD3E0: lw          $s1, 0x54($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X54);
    // 0x801CD3E4: lw          $s2, 0x58($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X58);
    // 0x801CD3E8: lw          $s3, 0x5C($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X5C);
    // 0x801CD3EC: lw          $s4, 0x60($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X60);
    // 0x801CD3F0: lw          $s5, 0x64($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X64);
    // 0x801CD3F4: lw          $s6, 0x68($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X68);
    // 0x801CD3F8: jr          $ra
    // 0x801CD3FC: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x801CD3FC: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void overlay_system_1_func_801CD400(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD400: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801CD404: lw          $a1, -0x53D8($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53D8);
    // 0x801CD408: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CD40C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801CD410: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CD414: addiu       $a0, $zero, 0x10
    ctx->r4 = ADD32(0, 0X10);
    // 0x801CD418: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x801CD41C: addiu       $v1, $a1, 0x2CC
    ctx->r3 = ADD32(ctx->r5, 0X2CC);
L_801CD420:
    // 0x801CD420: lw          $t6, 0x8($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X8);
    // 0x801CD424: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CD428: lhu         $t7, 0x2($t6)
    ctx->r15 = MEM_HU(ctx->r14, 0X2);
    // 0x801CD42C: andi        $t8, $t7, 0x1000
    ctx->r24 = ctx->r15 & 0X1000;
    // 0x801CD430: beql        $t8, $zero, L_801CD464
    if (ctx->r24 == 0) {
        // 0x801CD434: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801CD464;
    }
    goto skip_0;
    // 0x801CD434: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801CD438: bne         $v0, $a0, L_801CD420
    if (ctx->r2 != ctx->r4) {
        // 0x801CD43C: addiu       $v1, $v1, 0x8
        ctx->r3 = ADD32(ctx->r3, 0X8);
            goto L_801CD420;
    }
    // 0x801CD43C: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x801CD440: lhu         $t9, 0x2($a2)
    ctx->r25 = MEM_HU(ctx->r6, 0X2);
    // 0x801CD444: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801CD448: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x801CD44C: ori         $t0, $t9, 0x1000
    ctx->r8 = ctx->r25 | 0X1000;
    // 0x801CD450: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD454: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CD458: jalr        $t9
    // 0x801CD45C: sh          $t0, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CD45C: sh          $t0, 0x2($a2)
    MEM_H(0X2, ctx->r6) = ctx->r8;
    after_0:
    // 0x801CD460: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CD464:
    // 0x801CD464: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CD468: jr          $ra
    // 0x801CD46C: nop

    return;
    // 0x801CD46C: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD470(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD470: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD474: lw          $v1, -0x53D8($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53D8);
    // 0x801CD478: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801CD47C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CD480: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CD484: lhu         $t6, 0xC($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0XC);
    // 0x801CD488: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CD48C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD490: andi        $t7, $t6, 0x8
    ctx->r15 = ctx->r14 & 0X8;
    // 0x801CD494: beq         $t7, $zero, L_801CD5AC
    if (ctx->r15 == 0) {
        // 0x801CD498: nop

            goto L_801CD5AC;
    }
    // 0x801CD498: nop

    // 0x801CD49C: sh          $zero, 0x7C20($at)
    MEM_H(0X7C20, ctx->r1) = 0;
    // 0x801CD4A0: lw          $t8, 0x154($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X154);
    // 0x801CD4A4: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CD4A8: lui         $v0, 0xE000
    ctx->r2 = S32(0XE000 << 16);
    // 0x801CD4AC: srl         $t9, $t8, 29
    ctx->r25 = S32(U32(ctx->r24) >> 29);
    // 0x801CD4B0: bne         $t9, $zero, L_801CD4DC
    if (ctx->r25 != 0) {
        // 0x801CD4B4: addiu       $a3, $a3, 0x7C18
        ctx->r7 = ADD32(ctx->r7, 0X7C18);
            goto L_801CD4DC;
    }
    // 0x801CD4B4: addiu       $a3, $a3, 0x7C18
    ctx->r7 = ADD32(ctx->r7, 0X7C18);
    // 0x801CD4B8: lw          $t1, 0x1F4($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X1F4);
    // 0x801CD4BC: addiu       $t3, $zero, 0x2
    ctx->r11 = ADD32(0, 0X2);
    // 0x801CD4C0: and         $t2, $t1, $v0
    ctx->r10 = ctx->r9 & ctx->r2;
    // 0x801CD4C4: beql        $t2, $zero, L_801CD4E0
    if (ctx->r10 == 0) {
        // 0x801CD4C8: sw          $zero, 0x0($a3)
        MEM_W(0X0, ctx->r7) = 0;
            goto L_801CD4E0;
    }
    goto skip_0;
    // 0x801CD4C8: sw          $zero, 0x0($a3)
    MEM_W(0X0, ctx->r7) = 0;
    skip_0:
    // 0x801CD4CC: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CD4D0: addiu       $a3, $a3, 0x7C18
    ctx->r7 = ADD32(ctx->r7, 0X7C18);
    // 0x801CD4D4: b           L_801CD4E0
    // 0x801CD4D8: sw          $t3, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r11;
        goto L_801CD4E0;
    // 0x801CD4D8: sw          $t3, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r11;
L_801CD4DC:
    // 0x801CD4DC: sw          $zero, 0x0($a3)
    MEM_W(0X0, ctx->r7) = 0;
L_801CD4E0:
    // 0x801CD4E0: lw          $t4, 0x0($a3)
    ctx->r12 = MEM_W(ctx->r7, 0X0);
    // 0x801CD4E4: addiu       $t0, $zero, 0x50
    ctx->r8 = ADD32(0, 0X50);
    // 0x801CD4E8: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x801CD4EC: multu       $t4, $t0
    result = U64(U32(ctx->r12)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801CD4F0: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x801CD4F4: addiu       $a1, $a1, 0x3F88
    ctx->r5 = ADD32(ctx->r5, 0X3F88);
    // 0x801CD4F8: addiu       $a2, $zero, 0x24
    ctx->r6 = ADD32(0, 0X24);
    // 0x801CD4FC: mflo        $t5
    ctx->r13 = lo;
    // 0x801CD500: addu        $v0, $v1, $t5
    ctx->r2 = ADD32(ctx->r3, ctx->r13);
    // 0x801CD504: lw          $t6, 0x134($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X134);
    // 0x801CD508: or          $t7, $t6, $at
    ctx->r15 = ctx->r14 | ctx->r1;
    // 0x801CD50C: sw          $t7, 0x134($v0)
    MEM_W(0X134, ctx->r2) = ctx->r15;
    // 0x801CD510: lw          $t8, 0x0($a3)
    ctx->r24 = MEM_W(ctx->r7, 0X0);
    // 0x801CD514: multu       $t8, $t0
    result = U64(U32(ctx->r24)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x801CD518: mflo        $t9
    ctx->r25 = lo;
    // 0x801CD51C: addu        $a0, $v1, $t9
    ctx->r4 = ADD32(ctx->r3, ctx->r25);
    // 0x801CD520: jal         0x80001090
    // 0x801CD524: addiu       $a0, $a0, 0x154
    ctx->r4 = ADD32(ctx->r4, 0X154);
    memory_copy(rdram, ctx);
        goto after_0;
    // 0x801CD524: addiu       $a0, $a0, 0x154
    ctx->r4 = ADD32(ctx->r4, 0X154);
    after_0:
    // 0x801CD528: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801CD52C: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x801CD530: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801CD534: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x801CD538: addiu       $a0, $zero, 0x21C
    ctx->r4 = ADD32(0, 0X21C);
    // 0x801CD53C: jalr        $t9
    // 0x801CD540: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CD540: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_1:
    // 0x801CD544: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801CD548: addiu       $a0, $a0, 0x7C5C
    ctx->r4 = ADD32(ctx->r4, 0X7C5C);
    // 0x801CD54C: jal         0x80021DDC
    // 0x801CD550: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_80021DDC(rdram, ctx);
        goto after_2;
    // 0x801CD550: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_2:
    // 0x801CD554: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD558: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CD55C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CD560: jalr        $t9
    // 0x801CD564: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CD564: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
    // 0x801CD568: lh          $t1, 0xE($s0)
    ctx->r9 = MEM_H(ctx->r16, 0XE);
    // 0x801CD56C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CD570: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CD574: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x801CD578: addu        $v0, $s0, $t2
    ctx->r2 = ADD32(ctx->r16, ctx->r10);
    // 0x801CD57C: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801CD580: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801CD584: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CD588: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801CD58C: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801CD590: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x801CD594: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801CD598: lw          $t9, 0x7C28($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C28);
    // 0x801CD59C: jalr        $t9
    // 0x801CD5A0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CD5A0: nop

    after_4:
    // 0x801CD5A4: b           L_801CD5B8
    // 0x801CD5A8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CD5B8;
    // 0x801CD5A8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CD5AC:
    // 0x801CD5AC: jal         0x801CDFA4
    // 0x801CD5B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    overlay_system_1_func_801CDFA4(rdram, ctx);
        goto after_5;
    // 0x801CD5B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x801CD5B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CD5B8:
    // 0x801CD5B8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CD5BC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801CD5C0: jr          $ra
    // 0x801CD5C4: nop

    return;
    // 0x801CD5C4: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD5C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD5C8: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CD5CC: lw          $t6, -0x53D8($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53D8);
    // 0x801CD5D0: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x801CD5D4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CD5D8: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x801CD5DC: jal         0x801CD9D4
    // 0x801CD5E0: sw          $t6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r14;
    overlay_system_1_func_801CD9D4(rdram, ctx);
        goto after_0;
    // 0x801CD5E0: sw          $t6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r14;
    after_0:
    // 0x801CD5E4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CD5E8: addiu       $v0, $v0, 0x7C1C
    ctx->r2 = ADD32(ctx->r2, 0X7C1C);
    // 0x801CD5EC: lh          $t7, 0x0($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X0);
    // 0x801CD5F0: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x801CD5F4: mtc1        $at, $f13
    ctx->f_odd[(13 - 1) * 2] = ctx->r1;
    // 0x801CD5F8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801CD5FC: sh          $t8, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r24;
    // 0x801CD600: lh          $t9, 0x0($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X0);
    // 0x801CD604: lui         $at, 0x4014
    ctx->r1 = S32(0X4014 << 16);
    // 0x801CD608: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x801CD60C: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x801CD610: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801CD614: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x801CD618: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801CD61C: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
    // 0x801CD620: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD624: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801CD628: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CD62C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x801CD630: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl);
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x801CD634: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    // 0x801CD638: div.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d);
    ctx->f16.d = DIV_D(ctx->f8.d, ctx->f10.d);
    // 0x801CD63C: cvt.s.d     $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d);
    ctx->f2.fl = CVT_S_D(ctx->f16.d);
    // 0x801CD640: cvt.d.s     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl);
    ctx->f0.d = CVT_D_S(ctx->f2.fl);
    // 0x801CD644: c.lt.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d < ctx->f0.d;
    // 0x801CD648: nop

    // 0x801CD64C: bc1fl       L_801CD664
    if (!c1cs) {
        // 0x801CD650: c.eq.d      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
            goto L_801CD664;
    }
    goto skip_0;
    // 0x801CD650: c.eq.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
    skip_0:
    // 0x801CD654: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x801CD658: nop

    // 0x801CD65C: cvt.d.s     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl);
    ctx->f0.d = CVT_D_S(ctx->f2.fl);
    // 0x801CD660: c.eq.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
L_801CD664:
    // 0x801CD664: nop

    // 0x801CD668: bc1fl       L_801CD680
    if (!c1cs) {
        // 0x801CD66C: lw          $v0, 0x30($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X30);
            goto L_801CD680;
    }
    goto skip_1;
    // 0x801CD66C: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    skip_1:
    // 0x801CD670: jalr        $t9
    // 0x801CD674: swc1        $f2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f2.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CD674: swc1        $f2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f2.u32l;
    after_1:
    // 0x801CD678: lwc1        $f2, 0x28($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X28);
    // 0x801CD67C: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
L_801CD680:
    // 0x801CD680: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CD684: addiu       $a0, $a0, 0x3F88
    ctx->r4 = ADD32(ctx->r4, 0X3F88);
    // 0x801CD688: lhu         $t0, 0xE($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0XE);
    // 0x801CD68C: andi        $t1, $t0, 0x1000
    ctx->r9 = ctx->r8 & 0X1000;
    // 0x801CD690: beq         $t1, $zero, L_801CD754
    if (ctx->r9 == 0) {
        // 0x801CD694: nop

            goto L_801CD754;
    }
    // 0x801CD694: nop

    // 0x801CD698: lw          $t2, 0x0($a0)
    ctx->r10 = MEM_W(ctx->r4, 0X0);
    // 0x801CD69C: srl         $t3, $t2, 29
    ctx->r11 = S32(U32(ctx->r10) >> 29);
    // 0x801CD6A0: beq         $t3, $zero, L_801CD754
    if (ctx->r11 == 0) {
        // 0x801CD6A4: nop

            goto L_801CD754;
    }
    // 0x801CD6A4: nop

    // 0x801CD6A8: lhu         $t4, 0xC($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0XC);
    // 0x801CD6AC: andi        $t5, $t4, 0x4000
    ctx->r13 = ctx->r12 & 0X4000;
    // 0x801CD6B0: bne         $t5, $zero, L_801CD754
    if (ctx->r13 != 0) {
        // 0x801CD6B4: nop

            goto L_801CD754;
    }
    // 0x801CD6B4: nop

    // 0x801CD6B8: jal         0x8002109C
    // 0x801CD6BC: swc1        $f2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f2.u32l;
    func_8002109C(rdram, ctx);
        goto after_2;
    // 0x801CD6BC: swc1        $f2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f2.u32l;
    after_2:
    // 0x801CD6C0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD6C4: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801CD6C8: lwc1        $f18, 0x0($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801CD6CC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD6D0: lwc1        $f4, 0x50($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801CD6D4: addiu       $t9, $t9, 0x4C24
    ctx->r25 = ADD32(ctx->r25, 0X4C24);
    // 0x801CD6D8: sub.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl);
    ctx->f6.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x801CD6DC: swc1        $f6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f6.u32l;
    // 0x801CD6E0: lwc1        $f8, 0x4($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801CD6E4: lwc1        $f10, 0x54($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X54);
    // 0x801CD6E8: lwc1        $f14, 0x1C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x801CD6EC: sub.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f16.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x801CD6F0: swc1        $f16, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f16.u32l;
    // 0x801CD6F4: lwc1        $f18, 0x8($v0)
    ctx->f18.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801CD6F8: lwc1        $f4, 0x58($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X58);
    // 0x801CD6FC: sub.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl);
    ctx->f6.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x801CD700: swc1        $f6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f6.u32l;
    // 0x801CD704: lwc1        $f12, 0x24($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X24);
    // 0x801CD708: jalr        $t9
    // 0x801CD70C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl);
    ctx->f12.fl = -ctx->f12.fl;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CD70C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl);
    ctx->f12.fl = -ctx->f12.fl;
    after_3:
    // 0x801CD710: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD714: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801CD718: lwc1        $f2, 0x28($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X28);
    // 0x801CD71C: lh          $a0, 0x5E($v1)
    ctx->r4 = MEM_H(ctx->r3, 0X5E);
    // 0x801CD720: subu        $a1, $v0, $a0
    ctx->r5 = SUB32(ctx->r2, ctx->r4);
    // 0x801CD724: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x801CD728: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x801CD72C: mtc1        $a1, $f16
    ctx->f16.u32l = ctx->r5;
    // 0x801CD730: mtc1        $a0, $f8
    ctx->f8.u32l = ctx->r4;
    // 0x801CD734: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801CD738: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801CD73C: mul.s       $f4, $f18, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f2.fl);
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f2.fl);
    // 0x801CD740: add.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl);
    ctx->f6.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x801CD744: trunc.w.s   $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x801CD748: mfc1        $t0, $f8
    ctx->r8 = (int32_t)ctx->f8.u32l;
    // 0x801CD74C: nop

    // 0x801CD750: sh          $t0, 0x5E($v1)
    MEM_H(0X5E, ctx->r3) = ctx->r8;
L_801CD754:
    // 0x801CD754: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD758: swc1        $f2, 0x7C24($at)
    MEM_W(0X7C24, ctx->r1) = ctx->f2.u32l;
    // 0x801CD75C: jal         0x801CDA44
    // 0x801CD760: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    overlay_system_1_func_801CDA44(rdram, ctx);
        goto after_4;
    // 0x801CD760: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    after_4:
    // 0x801CD764: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CD768: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x801CD76C: jr          $ra
    // 0x801CD770: nop

    return;
    // 0x801CD770: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD774(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD774: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD778: lw          $v1, -0x53D8($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53D8);
    // 0x801CD77C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801CD780: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CD784: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x801CD788: jal         0x801CD9D4
    // 0x801CD78C: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    overlay_system_1_func_801CD9D4(rdram, ctx);
        goto after_0;
    // 0x801CD78C: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    after_0:
    // 0x801CD790: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x801CD794: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x801CD798: addiu       $t8, $zero, 0x5
    ctx->r24 = ADD32(0, 0X5);
    // 0x801CD79C: lhu         $t6, 0xC($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0XC);
    // 0x801CD7A0: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CD7A4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801CD7A8: andi        $t7, $t6, 0x8
    ctx->r15 = ctx->r14 & 0X8;
    // 0x801CD7AC: bne         $t7, $zero, L_801CD840
    if (ctx->r15 != 0) {
        // 0x801CD7B0: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_801CD840;
    }
    // 0x801CD7B0: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x801CD7B4: lw          $t9, 0x7C18($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C18);
    // 0x801CD7B8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD7BC: sh          $t8, 0x7C1C($at)
    MEM_H(0X7C1C, ctx->r1) = ctx->r24;
    // 0x801CD7C0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801CD7C4: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x801CD7C8: sll         $t0, $t0, 4
    ctx->r8 = S32(ctx->r8 << 4);
    // 0x801CD7CC: addu        $v0, $v1, $t0
    ctx->r2 = ADD32(ctx->r3, ctx->r8);
    // 0x801CD7D0: lw          $t1, 0x134($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X134);
    // 0x801CD7D4: lui         $at, 0xBFFF
    ctx->r1 = S32(0XBFFF << 16);
    // 0x801CD7D8: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801CD7DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD7E0: and         $t2, $t1, $at
    ctx->r10 = ctx->r9 & ctx->r1;
    // 0x801CD7E4: sw          $t2, 0x134($v0)
    MEM_W(0X134, ctx->r2) = ctx->r10;
    // 0x801CD7E8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CD7EC: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x801CD7F0: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801CD7F4: jalr        $t9
    // 0x801CD7F8: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CD7F8: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
    // 0x801CD7FC: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x801CD800: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CD804: lh          $t3, 0xE($a0)
    ctx->r11 = MEM_H(ctx->r4, 0XE);
    // 0x801CD808: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x801CD80C: addu        $v0, $a0, $t4
    ctx->r2 = ADD32(ctx->r4, ctx->r12);
    // 0x801CD810: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x801CD814: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x801CD818: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CD81C: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x801CD820: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x801CD824: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x801CD828: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x801CD82C: lw          $t9, 0x7C28($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C28);
    // 0x801CD830: jalr        $t9
    // 0x801CD834: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CD834: nop

    after_2:
    // 0x801CD838: b           L_801CD854
    // 0x801CD83C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801CD854;
    // 0x801CD83C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CD840:
    // 0x801CD840: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801CD844: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD848: jal         0x801CDA44
    // 0x801CD84C: swc1        $f4, 0x7C24($at)
    MEM_W(0X7C24, ctx->r1) = ctx->f4.u32l;
    overlay_system_1_func_801CDA44(rdram, ctx);
        goto after_3;
    // 0x801CD84C: swc1        $f4, 0x7C24($at)
    MEM_W(0X7C24, ctx->r1) = ctx->f4.u32l;
    after_3:
    // 0x801CD850: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CD854:
    // 0x801CD854: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801CD858: jr          $ra
    // 0x801CD85C: nop

    return;
    // 0x801CD85C: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD860(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD860: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801CD864: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CD868: lw          $t6, -0x53D8($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53D8);
    // 0x801CD86C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CD870: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CD874: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CD878: jal         0x801CD9D4
    // 0x801CD87C: sw          $t6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r14;
    overlay_system_1_func_801CD9D4(rdram, ctx);
        goto after_0;
    // 0x801CD87C: sw          $t6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r14;
    after_0:
    // 0x801CD880: jal         0x801CDFA4
    // 0x801CD884: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    overlay_system_1_func_801CDFA4(rdram, ctx);
        goto after_1;
    // 0x801CD884: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_1:
    // 0x801CD888: lw          $t7, 0x28($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X28);
    // 0x801CD88C: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD890: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CD894: lhu         $t8, 0xC($t7)
    ctx->r24 = MEM_HU(ctx->r15, 0XC);
    // 0x801CD898: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CD89C: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    // 0x801CD8A0: andi        $t9, $t8, 0x8
    ctx->r25 = ctx->r24 & 0X8;
    // 0x801CD8A4: beql        $t9, $zero, L_801CD928
    if (ctx->r25 == 0) {
        // 0x801CD8A8: lh          $t4, 0xE($s0)
        ctx->r12 = MEM_H(ctx->r16, 0XE);
            goto L_801CD928;
    }
    goto skip_0;
    // 0x801CD8A8: lh          $t4, 0xE($s0)
    ctx->r12 = MEM_H(ctx->r16, 0XE);
    skip_0:
    // 0x801CD8AC: lh          $t1, 0xE($s0)
    ctx->r9 = MEM_H(ctx->r16, 0XE);
    // 0x801CD8B0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD8B4: addiu       $v1, $v1, 0x7C1C
    ctx->r3 = ADD32(ctx->r3, 0X7C1C);
    // 0x801CD8B8: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x801CD8BC: addu        $t3, $s0, $t2
    ctx->r11 = ADD32(ctx->r16, ctx->r10);
    // 0x801CD8C0: lbu         $t4, 0x8($t3)
    ctx->r12 = MEM_BU(ctx->r11, 0X8);
    // 0x801CD8C4: lh          $t0, 0x0($v1)
    ctx->r8 = MEM_H(ctx->r3, 0X0);
    // 0x801CD8C8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD8CC: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CD8D0: subu        $t5, $t0, $t4
    ctx->r13 = SUB32(ctx->r8, ctx->r12);
    // 0x801CD8D4: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x801CD8D8: jalr        $t9
    // 0x801CD8DC: sh          $t6, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CD8DC: sh          $t6, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r14;
    after_2:
    // 0x801CD8E0: lh          $t7, 0xE($s0)
    ctx->r15 = MEM_H(ctx->r16, 0XE);
    // 0x801CD8E4: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CD8E8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CD8EC: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x801CD8F0: addu        $v0, $s0, $t8
    ctx->r2 = ADD32(ctx->r16, ctx->r24);
    // 0x801CD8F4: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x801CD8F8: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x801CD8FC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CD900: sll         $t0, $t3, 2
    ctx->r8 = S32(ctx->r11 << 2);
    // 0x801CD904: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x801CD908: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x801CD90C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801CD910: lw          $t9, 0x7C28($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C28);
    // 0x801CD914: jalr        $t9
    // 0x801CD918: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CD918: nop

    after_3:
    // 0x801CD91C: b           L_801CD9C4
    // 0x801CD920: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CD9C4;
    // 0x801CD920: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801CD924: lh          $t4, 0xE($s0)
    ctx->r12 = MEM_H(ctx->r16, 0XE);
L_801CD928:
    // 0x801CD928: addiu       $v1, $v1, 0x7C1C
    ctx->r3 = ADD32(ctx->r3, 0X7C1C);
    // 0x801CD92C: lh          $v0, 0x0($v1)
    ctx->r2 = MEM_H(ctx->r3, 0X0);
    // 0x801CD930: sll         $t5, $t4, 1
    ctx->r13 = S32(ctx->r12 << 1);
    // 0x801CD934: addu        $t6, $s0, $t5
    ctx->r14 = ADD32(ctx->r16, ctx->r13);
    // 0x801CD938: lbu         $t7, 0x8($t6)
    ctx->r15 = MEM_BU(ctx->r14, 0X8);
    // 0x801CD93C: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x801CD940: mtc1        $zero, $f13
    ctx->f_odd[(13 - 1) * 2] = 0;
    // 0x801CD944: subu        $t8, $v0, $t7
    ctx->r24 = SUB32(ctx->r2, ctx->r15);
    // 0x801CD948: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x801CD94C: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801CD950: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x801CD954: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CD958: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CD95C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CD960: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801CD964: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CD968: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    // 0x801CD96C: div.s       $f2, $f6, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x801CD970: cvt.d.s     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl);
    ctx->f0.d = CVT_D_S(ctx->f2.fl);
    // 0x801CD974: c.lt.d      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.d < ctx->f12.d;
    // 0x801CD978: nop

    // 0x801CD97C: bc1fl       L_801CD994
    if (!c1cs) {
        // 0x801CD980: c.eq.d      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
            goto L_801CD994;
    }
    goto skip_1;
    // 0x801CD980: c.eq.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
    skip_1:
    // 0x801CD984: mtc1        $zero, $f2
    ctx->f2.u32l = 0;
    // 0x801CD988: nop

    // 0x801CD98C: cvt.d.s     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl);
    ctx->f0.d = CVT_D_S(ctx->f2.fl);
    // 0x801CD990: c.eq.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
L_801CD994:
    // 0x801CD994: nop

    // 0x801CD998: bc1f        L_801CD9B0
    if (!c1cs) {
        // 0x801CD99C: nop

            goto L_801CD9B0;
    }
    // 0x801CD99C: nop

    // 0x801CD9A0: sh          $zero, 0x0($v1)
    MEM_H(0X0, ctx->r3) = 0;
    // 0x801CD9A4: jalr        $t9
    // 0x801CD9A8: swc1        $f2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f2.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CD9A8: swc1        $f2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f2.u32l;
    after_4:
    // 0x801CD9AC: lwc1        $f2, 0x20($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X20);
L_801CD9B0:
    // 0x801CD9B0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CD9B4: swc1        $f2, 0x7C24($at)
    MEM_W(0X7C24, ctx->r1) = ctx->f2.u32l;
    // 0x801CD9B8: jal         0x801CDA44
    // 0x801CD9BC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    overlay_system_1_func_801CDA44(rdram, ctx);
        goto after_5;
    // 0x801CD9BC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x801CD9C0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CD9C4:
    // 0x801CD9C4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CD9C8: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801CD9CC: jr          $ra
    // 0x801CD9D0: nop

    return;
    // 0x801CD9D0: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CD9D4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CD9D4: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CD9D8: lh          $t6, 0x7C20($t6)
    ctx->r14 = MEM_H(ctx->r14, 0X7C20);
    // 0x801CD9DC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CD9E0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CD9E4: slti        $at, $t6, 0x7
    ctx->r1 = SIGNED(ctx->r14) < 0X7 ? 1 : 0;
    // 0x801CD9E8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CD9EC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801CD9F0: beq         $at, $zero, L_801CDA34
    if (ctx->r1 == 0) {
        // 0x801CD9F4: lw          $v1, -0x53D8($v1)
        ctx->r3 = MEM_W(ctx->r3, -0X53D8);
            goto L_801CDA34;
    }
    // 0x801CD9F4: lw          $v1, -0x53D8($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53D8);
    // 0x801CD9F8: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x801CD9FC: lw          $t7, 0x7C18($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X7C18);
    // 0x801CDA00: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x801CDA04: addiu       $a1, $a1, 0x3F88
    ctx->r5 = ADD32(ctx->r5, 0X3F88);
    // 0x801CDA08: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x801CDA0C: addu        $t8, $t8, $t7
    ctx->r24 = ADD32(ctx->r24, ctx->r15);
    // 0x801CDA10: sll         $t8, $t8, 4
    ctx->r24 = S32(ctx->r24 << 4);
    // 0x801CDA14: addu        $v0, $v1, $t8
    ctx->r2 = ADD32(ctx->r3, ctx->r24);
    // 0x801CDA18: lw          $t9, 0x134($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X134);
    // 0x801CDA1C: addiu       $a0, $v0, 0x154
    ctx->r4 = ADD32(ctx->r2, 0X154);
    // 0x801CDA20: sll         $t0, $t9, 1
    ctx->r8 = S32(ctx->r25 << 1);
    // 0x801CDA24: bgezl       $t0, L_801CDA38
    if (SIGNED(ctx->r8) >= 0) {
        // 0x801CDA28: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801CDA38;
    }
    goto skip_0;
    // 0x801CDA28: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801CDA2C: jal         0x80001090
    // 0x801CDA30: addiu       $a2, $zero, 0x24
    ctx->r6 = ADD32(0, 0X24);
    memory_copy(rdram, ctx);
        goto after_0;
    // 0x801CDA30: addiu       $a2, $zero, 0x24
    ctx->r6 = ADD32(0, 0X24);
    after_0:
L_801CDA34:
    // 0x801CDA34: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CDA38:
    // 0x801CDA38: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CDA3C: jr          $ra
    // 0x801CDA40: nop

    return;
    // 0x801CDA40: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CDA44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CDA44: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x801CDA48: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CDA4C: lw          $t6, -0x53D8($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53D8);
    // 0x801CDA50: sw          $a0, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r4;
    // 0x801CDA54: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x801CDA58: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CDA5C: addiu       $a0, $a0, 0x3F88
    ctx->r4 = ADD32(ctx->r4, 0X3F88);
    // 0x801CDA60: jal         0x8002109C
    // 0x801CDA64: sw          $t6, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r14;
    func_8002109C(rdram, ctx);
        goto after_0;
    // 0x801CDA64: sw          $t6, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r14;
    after_0:
    // 0x801CDA68: lui         $t7, 0x800F
    ctx->r15 = S32(0X800F << 16);
    // 0x801CDA6C: lw          $t7, 0x3F88($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X3F88);
    // 0x801CDA70: or          $t0, $v0, $zero
    ctx->r8 = ctx->r2 | 0;
    // 0x801CDA74: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CDA78: srl         $t8, $t7, 29
    ctx->r24 = S32(U32(ctx->r15) >> 29);
    // 0x801CDA7C: beql        $t8, $zero, L_801CDB48
    if (ctx->r24 == 0) {
        // 0x801CDA80: lw          $t4, 0x68($sp)
        ctx->r12 = MEM_W(ctx->r29, 0X68);
            goto L_801CDB48;
    }
    goto skip_0;
    // 0x801CDA80: lw          $t4, 0x68($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X68);
    skip_0:
    // 0x801CDA84: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801CDA88: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801CDA8C: lui         $t3, 0x801D
    ctx->r11 = S32(0X801D << 16);
    // 0x801CDA90: lwc1        $f6, 0x50($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801CDA94: lh          $t3, 0x7C20($t3)
    ctx->r11 = MEM_H(ctx->r11, 0X7C20);
    // 0x801CDA98: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801CDA9C: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CDAA0: addiu       $a0, $sp, 0x54
    ctx->r4 = ADD32(ctx->r29, 0X54);
    // 0x801CDAA4: addiu       $t2, $t2, 0x7C68
    ctx->r10 = ADD32(ctx->r10, 0X7C68);
    // 0x801CDAA8: addiu       $t9, $zero, -0x471C
    ctx->r25 = ADD32(0, -0X471C);
    // 0x801CDAAC: swc1        $f8, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f8.u32l;
    // 0x801CDAB0: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801CDAB4: lwc1        $f16, 0x54($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X54);
    // 0x801CDAB8: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x801CDABC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801CDAC0: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x801CDAC4: addiu       $a3, $zero, 0x471C
    ctx->r7 = ADD32(0, 0X471C);
    // 0x801CDAC8: swc1        $f18, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f18.u32l;
    // 0x801CDACC: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801CDAD0: lwc1        $f6, 0x58($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X58);
    // 0x801CDAD4: sw          $v0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r2;
    // 0x801CDAD8: sw          $t2, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r10;
    // 0x801CDADC: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CDAE0: sw          $t9, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r25;
    // 0x801CDAE4: sw          $t3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r11;
    // 0x801CDAE8: jal         0x800211A8
    // 0x801CDAEC: swc1        $f8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f8.u32l;
    func_800211A8(rdram, ctx);
        goto after_1;
    // 0x801CDAEC: swc1        $f8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f8.u32l;
    after_1:
    // 0x801CDAF0: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CDAF4: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x801CDAF8: lw          $v1, 0x2960($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X2960);
    // 0x801CDAFC: lwc1        $f10, 0x54($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X54);
    // 0x801CDB00: lwc1        $f4, 0x58($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801CDB04: lwc1        $f16, 0x50($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801CDB08: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x801CDB0C: lw          $a2, 0x7C24($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X7C24);
    // 0x801CDB10: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x801CDB14: lwc1        $f10, 0x5C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x801CDB18: lw          $a0, 0x295C($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X295C);
    // 0x801CDB1C: addiu       $a1, $sp, 0x54
    ctx->r5 = ADD32(ctx->r29, 0X54);
    // 0x801CDB20: swc1        $f18, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f18.u32l;
    // 0x801CDB24: lwc1        $f6, 0x54($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X54);
    // 0x801CDB28: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x801CDB2C: swc1        $f8, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f8.u32l;
    // 0x801CDB30: lwc1        $f16, 0x58($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X58);
    // 0x801CDB34: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x801CDB38: jal         0x80021598
    // 0x801CDB3C: swc1        $f18, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f18.u32l;
    func_80021598(rdram, ctx);
        goto after_2;
    // 0x801CDB3C: swc1        $f18, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f18.u32l;
    after_2:
    // 0x801CDB40: lw          $t0, 0x50($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X50);
    // 0x801CDB44: lw          $t4, 0x68($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X68);
L_801CDB48:
    // 0x801CDB48: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x801CDB4C: lhu         $t5, 0xE($t4)
    ctx->r13 = MEM_HU(ctx->r12, 0XE);
    // 0x801CDB50: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x801CDB54: andi        $t6, $t5, 0x4000
    ctx->r14 = ctx->r13 & 0X4000;
    // 0x801CDB58: beq         $t6, $zero, L_801CDB7C
    if (ctx->r14 == 0) {
        // 0x801CDB5C: nop

            goto L_801CDB7C;
    }
    // 0x801CDB5C: nop

    // 0x801CDB60: lw          $t7, -0x53E0($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X53E0);
    // 0x801CDB64: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801CDB68: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x801CDB6C: lw          $t2, 0x14($t9)
    ctx->r10 = MEM_W(ctx->r25, 0X14);
    // 0x801CDB70: lw          $t3, 0x10($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X10);
    // 0x801CDB74: b           L_801CDB98
    // 0x801CDB78: sw          $t3, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r11;
        goto L_801CDB98;
    // 0x801CDB78: sw          $t3, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r11;
L_801CDB7C:
    // 0x801CDB7C: lw          $t4, -0x53E0($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X53E0);
    // 0x801CDB80: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CDB84: lw          $t6, 0x14($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X14);
    // 0x801CDB88: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801CDB8C: lw          $t8, 0x10($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X10);
    // 0x801CDB90: lw          $t9, 0x10($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X10);
    // 0x801CDB94: sw          $t9, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r25;
L_801CDB98:
    // 0x801CDB98: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X48);
    // 0x801CDB9C: jal         0x8000C9A4
    // 0x801CDBA0: sw          $t0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r8;
    func_8000C9A4(rdram, ctx);
        goto after_3;
    // 0x801CDBA0: sw          $t0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r8;
    after_3:
    // 0x801CDBA4: lui         $t1, 0x800F
    ctx->r9 = S32(0X800F << 16);
    // 0x801CDBA8: addiu       $t1, $t1, 0x3F88
    ctx->r9 = ADD32(ctx->r9, 0X3F88);
    // 0x801CDBAC: lw          $t2, 0x0($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X0);
    // 0x801CDBB0: lw          $t0, 0x50($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X50);
    // 0x801CDBB4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CDBB8: srl         $t3, $t2, 29
    ctx->r11 = S32(U32(ctx->r10) >> 29);
    // 0x801CDBBC: beq         $t3, $zero, L_801CDCC0
    if (ctx->r11 == 0) {
        // 0x801CDBC0: nop

            goto L_801CDCC0;
    }
    // 0x801CDBC0: nop

    // 0x801CDBC4: lh          $v0, 0x7C20($v0)
    ctx->r2 = MEM_H(ctx->r2, 0X7C20);
    // 0x801CDBC8: slti        $at, $v0, 0x6
    ctx->r1 = SIGNED(ctx->r2) < 0X6 ? 1 : 0;
    // 0x801CDBCC: bne         $at, $zero, L_801CDCC0
    if (ctx->r1 != 0) {
        // 0x801CDBD0: slti        $at, $v0, 0x15
        ctx->r1 = SIGNED(ctx->r2) < 0X15 ? 1 : 0;
            goto L_801CDCC0;
    }
    // 0x801CDBD0: slti        $at, $v0, 0x15
    ctx->r1 = SIGNED(ctx->r2) < 0X15 ? 1 : 0;
    // 0x801CDBD4: beq         $at, $zero, L_801CDCC0
    if (ctx->r1 == 0) {
        // 0x801CDBD8: lw          $v1, 0x48($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X48);
            goto L_801CDCC0;
    }
    // 0x801CDBD8: lw          $v1, 0x48($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X48);
    // 0x801CDBDC: slti        $at, $v0, 0x8
    ctx->r1 = SIGNED(ctx->r2) < 0X8 ? 1 : 0;
    // 0x801CDBE0: beq         $at, $zero, L_801CDC04
    if (ctx->r1 == 0) {
        // 0x801CDBE4: addiu       $t4, $zero, 0x8
        ctx->r12 = ADD32(0, 0X8);
            goto L_801CDC04;
    }
    // 0x801CDBE4: addiu       $t4, $zero, 0x8
    ctx->r12 = ADD32(0, 0X8);
    // 0x801CDBE8: subu        $t5, $t4, $v0
    ctx->r13 = SUB32(ctx->r12, ctx->r2);
    // 0x801CDBEC: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x801CDBF0: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x801CDBF4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801CDBF8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801CDBFC: b           L_801CDC38
    // 0x801CDC00: div.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
        goto L_801CDC38;
    // 0x801CDC00: div.s       $f0, $f6, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
L_801CDC04:
    // 0x801CDC04: slti        $at, $v0, 0x10
    ctx->r1 = SIGNED(ctx->r2) < 0X10 ? 1 : 0;
    // 0x801CDC08: beq         $at, $zero, L_801CDC20
    if (ctx->r1 == 0) {
        // 0x801CDC0C: addiu       $t6, $zero, 0x15
        ctx->r14 = ADD32(0, 0X15);
            goto L_801CDC20;
    }
    // 0x801CDC0C: addiu       $t6, $zero, 0x15
    ctx->r14 = ADD32(0, 0X15);
    // 0x801CDC10: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801CDC14: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x801CDC18: b           L_801CDC3C
    // 0x801CDC1C: lwc1        $f4, 0x0($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0X0);
        goto L_801CDC3C;
    // 0x801CDC1C: lwc1        $f4, 0x0($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0X0);
L_801CDC20:
    // 0x801CDC20: subu        $t7, $t6, $v0
    ctx->r15 = SUB32(ctx->r14, ctx->r2);
    // 0x801CDC24: mtc1        $t7, $f10
    ctx->f10.u32l = ctx->r15;
    // 0x801CDC28: lui         $at, 0x40C0
    ctx->r1 = S32(0X40C0 << 16);
    // 0x801CDC2C: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x801CDC30: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801CDC34: div.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f0.fl = DIV_S(ctx->f16.fl, ctx->f18.fl);
L_801CDC38:
    // 0x801CDC38: lwc1        $f4, 0x0($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0X0);
L_801CDC3C:
    // 0x801CDC3C: lwc1        $f6, 0xA8($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0XA8);
    // 0x801CDC40: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CDC44: addiu       $a0, $sp, 0x54
    ctx->r4 = ADD32(ctx->r29, 0X54);
    // 0x801CDC48: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CDC4C: addiu       $t9, $t9, 0x7C6C
    ctx->r25 = ADD32(ctx->r25, 0X7C6C);
    // 0x801CDC50: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x801CDC54: addiu       $t8, $zero, -0x471C
    ctx->r24 = ADD32(0, -0X471C);
    // 0x801CDC58: swc1        $f8, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f8.u32l;
    // 0x801CDC5C: lwc1        $f16, 0xAC($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0XAC);
    // 0x801CDC60: lwc1        $f10, 0x4($t0)
    ctx->f10.u32l = MEM_W(ctx->r8, 0X4);
    // 0x801CDC64: lh          $a2, 0x7C68($a2)
    ctx->r6 = MEM_H(ctx->r6, 0X7C68);
    // 0x801CDC68: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x801CDC6C: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x801CDC70: addiu       $a3, $zero, 0x471C
    ctx->r7 = ADD32(0, 0X471C);
    // 0x801CDC74: swc1        $f18, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f18.u32l;
    // 0x801CDC78: lwc1        $f6, 0xB0($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0XB0);
    // 0x801CDC7C: lwc1        $f4, 0x8($t0)
    ctx->f4.u32l = MEM_W(ctx->r8, 0X8);
    // 0x801CDC80: swc1        $f0, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f0.u32l;
    // 0x801CDC84: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    // 0x801CDC88: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CDC8C: sw          $t9, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r25;
    // 0x801CDC90: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    // 0x801CDC94: jal         0x800211A8
    // 0x801CDC98: swc1        $f8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f8.u32l;
    func_800211A8(rdram, ctx);
        goto after_4;
    // 0x801CDC98: swc1        $f8, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f8.u32l;
    after_4:
    // 0x801CDC9C: lwc1        $f0, 0x4C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x801CDCA0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801CDCA4: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    // 0x801CDCA8: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x801CDCAC: addiu       $a1, $sp, 0x54
    ctx->r5 = ADD32(ctx->r29, 0X54);
    // 0x801CDCB0: jal         0x80021758
    // 0x801CDCB4: addiu       $a3, $zero, -0x1
    ctx->r7 = ADD32(0, -0X1);
    func_80021758(rdram, ctx);
        goto after_5;
    // 0x801CDCB4: addiu       $a3, $zero, -0x1
    ctx->r7 = ADD32(0, -0X1);
    after_5:
    // 0x801CDCB8: lui         $t1, 0x800F
    ctx->r9 = S32(0X800F << 16);
    // 0x801CDCBC: addiu       $t1, $t1, 0x3F88
    ctx->r9 = ADD32(ctx->r9, 0X3F88);
L_801CDCC0:
    // 0x801CDCC0: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801CDCC4: lh          $t2, 0x7C20($t2)
    ctx->r10 = MEM_H(ctx->r10, 0X7C20);
    // 0x801CDCC8: addiu       $at, $zero, 0x8
    ctx->r1 = ADD32(0, 0X8);
    // 0x801CDCCC: bne         $t2, $at, L_801CDE84
    if (ctx->r10 != ctx->r1) {
        // 0x801CDCD0: nop

            goto L_801CDE84;
    }
    // 0x801CDCD0: nop

    // 0x801CDCD4: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x801CDCD8: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x801CDCDC: bgez        $t4, L_801CDCF0
    if (SIGNED(ctx->r12) >= 0) {
        // 0x801CDCE0: nop

            goto L_801CDCF0;
    }
    // 0x801CDCE0: nop

    // 0x801CDCE4: lw          $a1, 0x8($t1)
    ctx->r5 = MEM_W(ctx->r9, 0X8);
    // 0x801CDCE8: b           L_801CDDF8
    // 0x801CDCEC: lw          $a2, 0xC($t1)
    ctx->r6 = MEM_W(ctx->r9, 0XC);
        goto L_801CDDF8;
    // 0x801CDCEC: lw          $a2, 0xC($t1)
    ctx->r6 = MEM_W(ctx->r9, 0XC);
L_801CDCF0:
    // 0x801CDCF0: lui         $t5, 0x801D
    ctx->r13 = S32(0X801D << 16);
    // 0x801CDCF4: lw          $t5, -0x53E0($t5)
    ctx->r13 = MEM_W(ctx->r13, -0X53E0);
    // 0x801CDCF8: jal         0x800A3A50
    // 0x801CDCFC: lhu         $a0, 0x5E($t5)
    ctx->r4 = MEM_HU(ctx->r13, 0X5E);
    func_800A3A50(rdram, ctx);
        goto after_6;
    // 0x801CDCFC: lhu         $a0, 0x5E($t5)
    ctx->r4 = MEM_HU(ctx->r13, 0X5E);
    after_6:
    // 0x801CDD00: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x801CDD04: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CDD08: lwc1        $f18, 0x7D3C($at)
    ctx->f18.u32l = MEM_W(ctx->r1, 0X7D3C);
    // 0x801CDD0C: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801CDD10: lui         $at, 0x4059
    ctx->r1 = S32(0X4059 << 16);
    // 0x801CDD14: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x801CDD18: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x801CDD1C: lw          $t6, 0x68($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X68);
    // 0x801CDD20: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CDD24: div.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = DIV_S(ctx->f16.fl, ctx->f18.fl);
    // 0x801CDD28: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x801CDD2C: lw          $t7, -0x53E0($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X53E0);
    // 0x801CDD30: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801CDD34: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d);
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x801CDD38: ldc1        $f6, 0x7D40($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, 0X7D40);
    // 0x801CDD3C: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d);
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x801CDD40: swc1        $f16, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f16.u32l;
    // 0x801CDD44: lwc1        $f18, 0x274($t6)
    ctx->f18.u32l = MEM_W(ctx->r14, 0X274);
    // 0x801CDD48: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl);
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x801CDD4C: mul.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d);
    ctx->f8.d = MUL_D(ctx->f4.d, ctx->f6.d);
    // 0x801CDD50: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d);
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x801CDD54: swc1        $f10, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f10.u32l;
    // 0x801CDD58: jal         0x80097330
    // 0x801CDD5C: lhu         $a0, 0x5E($t7)
    ctx->r4 = MEM_HU(ctx->r15, 0X5E);
    func_80097330(rdram, ctx);
        goto after_7;
    // 0x801CDD5C: lhu         $a0, 0x5E($t7)
    ctx->r4 = MEM_HU(ctx->r15, 0X5E);
    after_7:
    // 0x801CDD60: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    // 0x801CDD64: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CDD68: lwc1        $f4, 0x7D48($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X7D48);
    // 0x801CDD6C: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801CDD70: lui         $at, 0x4059
    ctx->r1 = S32(0X4059 << 16);
    // 0x801CDD74: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x801CDD78: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x801CDD7C: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CDD80: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801CDD84: div.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl);
    ctx->f6.fl = DIV_S(ctx->f18.fl, ctx->f4.fl);
    // 0x801CDD88: lw          $a0, 0x48($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X48);
    // 0x801CDD8C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801CDD90: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801CDD94: neg.s       $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = -ctx->f6.fl;
    // 0x801CDD98: lwc1        $f6, 0x54($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X54);
    // 0x801CDD9C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl);
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x801CDDA0: mul.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d);
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f16.d);
    // 0x801CDDA4: lwc1        $f16, 0x58($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801CDDA8: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d);
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x801CDDAC: swc1        $f4, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f4.u32l;
    // 0x801CDDB0: lwc1        $f8, 0x50($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801CDDB4: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x801CDDB8: swc1        $f10, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f10.u32l;
    // 0x801CDDBC: lwc1        $f18, 0x54($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X54);
    // 0x801CDDC0: add.s       $f6, $f16, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f6.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x801CDDC4: swc1        $f6, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f6.u32l;
    // 0x801CDDC8: lwc1        $f8, 0x58($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X58);
    // 0x801CDDCC: add.s       $f16, $f4, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f16.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x801CDDD0: swc1        $f16, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f16.u32l;
    // 0x801CDDD4: lwc1        $f18, 0xA8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0XA8);
    // 0x801CDDD8: sub.s       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f10.fl - ctx->f18.fl;
    // 0x801CDDDC: swc1        $f4, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->f4.u32l;
    // 0x801CDDE0: lwc1        $f8, 0xAC($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0XAC);
    // 0x801CDDE4: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x801CDDE8: swc1        $f10, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f10.u32l;
    // 0x801CDDEC: lwc1        $f18, 0xB0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0XB0);
    // 0x801CDDF0: sub.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x801CDDF4: swc1        $f4, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->f4.u32l;
L_801CDDF8:
    // 0x801CDDF8: lw          $t0, 0x68($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X68);
    // 0x801CDDFC: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x801CDE00: addiu       $a0, $t0, 0x2CC
    ctx->r4 = ADD32(ctx->r8, 0X2CC);
    // 0x801CDE04: lw          $t8, 0x0($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X0);
    // 0x801CDE08: sll         $t9, $t8, 3
    ctx->r25 = S32(ctx->r24 << 3);
    // 0x801CDE0C: addu        $t2, $a0, $t9
    ctx->r10 = ADD32(ctx->r4, ctx->r25);
    // 0x801CDE10: lw          $v1, 0x8($t2)
    ctx->r3 = MEM_W(ctx->r10, 0X8);
    // 0x801CDE14: lw          $t3, 0x5C($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X5C);
    // 0x801CDE18: lw          $v0, 0x54($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X54);
    // 0x801CDE1C: or          $t4, $t3, $at
    ctx->r12 = ctx->r11 | ctx->r1;
    // 0x801CDE20: sw          $t4, 0x5C($v1)
    MEM_W(0X5C, ctx->r3) = ctx->r12;
    // 0x801CDE24: lwc1        $f6, 0x54($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X54);
    // 0x801CDE28: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x801CDE2C: swc1        $f6, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f6.u32l;
    // 0x801CDE30: lwc1        $f8, 0x58($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X58);
    // 0x801CDE34: swc1        $f8, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->f8.u32l;
    // 0x801CDE38: lwc1        $f10, 0x5C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x801CDE3C: sw          $a2, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r6;
    // 0x801CDE40: sw          $a1, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r5;
    // 0x801CDE44: swc1        $f10, 0x14($v0)
    MEM_W(0X14, ctx->r2) = ctx->f10.u32l;
    // 0x801CDE48: lw          $a3, 0x0($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X0);
    // 0x801CDE4C: sll         $t5, $a3, 3
    ctx->r13 = S32(ctx->r7 << 3);
    // 0x801CDE50: addu        $t6, $a0, $t5
    ctx->r14 = ADD32(ctx->r4, ctx->r13);
    // 0x801CDE54: lw          $t7, 0x8($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X8);
    // 0x801CDE58: addiu       $t2, $a3, 0x1
    ctx->r10 = ADD32(ctx->r7, 0X1);
    // 0x801CDE5C: lw          $t8, 0x5C($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X5C);
    // 0x801CDE60: andi        $t9, $t8, 0x4
    ctx->r25 = ctx->r24 & 0X4;
    // 0x801CDE64: beq         $t9, $zero, L_801CDE78
    if (ctx->r25 == 0) {
        // 0x801CDE68: nop

            goto L_801CDE78;
    }
    // 0x801CDE68: nop

    // 0x801CDE6C: lh          $v0, 0x12A($t0)
    ctx->r2 = MEM_H(ctx->r8, 0X12A);
    // 0x801CDE70: beq         $v0, $zero, L_801CDE78
    if (ctx->r2 == 0) {
        // 0x801CDE74: nop

            goto L_801CDE78;
    }
    // 0x801CDE74: nop

L_801CDE78:
    // 0x801CDE78: bne         $t2, $at, L_801CDE84
    if (ctx->r10 != ctx->r1) {
        // 0x801CDE7C: sw          $t2, 0x0($a0)
        MEM_W(0X0, ctx->r4) = ctx->r10;
            goto L_801CDE84;
    }
    // 0x801CDE7C: sw          $t2, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r10;
    // 0x801CDE80: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
L_801CDE84:
    // 0x801CDE84: jal         0x801CDE9C
    // 0x801CDE88: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    overlay_system_1_func_801CDE9C(rdram, ctx);
        goto after_8;
    // 0x801CDE88: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    after_8:
    // 0x801CDE8C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x801CDE90: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    // 0x801CDE94: jr          $ra
    // 0x801CDE98: nop

    return;
    // 0x801CDE98: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CDE9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CDE9C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CDEA0: lw          $v0, -0x53D8($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53D8);
    // 0x801CDEA4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x801CDEA8: lw          $t6, 0x2C8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2C8);
    // 0x801CDEAC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CDEB0: addiu       $v0, $v0, 0x7C20
    ctx->r2 = ADD32(ctx->r2, 0X7C20);
    // 0x801CDEB4: sll         $t7, $t6, 0
    ctx->r15 = S32(ctx->r14 << 0);
    // 0x801CDEB8: bltz        $t7, L_801CDECC
    if (SIGNED(ctx->r15) < 0) {
        // 0x801CDEBC: nop

            goto L_801CDECC;
    }
    // 0x801CDEBC: nop

    // 0x801CDEC0: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x801CDEC4: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x801CDEC8: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
L_801CDECC:
    // 0x801CDECC: jr          $ra
    // 0x801CDED0: nop

    return;
    // 0x801CDED0: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CDED4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CDED4: addiu       $sp, $sp, -0x8
    ctx->r29 = ADD32(ctx->r29, -0X8);
    // 0x801CDED8: sw          $s0, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r16;
    // 0x801CDEDC: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x801CDEE0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CDEE4: lw          $v0, -0x53D8($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53D8);
    // 0x801CDEE8: andi        $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 & 0X1000;
    // 0x801CDEEC: beq         $t7, $zero, L_801CDF98
    if (ctx->r15 == 0) {
        // 0x801CDEF0: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_801CDF98;
    }
    // 0x801CDEF0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x801CDEF4: addiu       $a1, $v0, 0x2CC
    ctx->r5 = ADD32(ctx->r2, 0X2CC);
    // 0x801CDEF8: addiu       $a2, $a1, 0x4
    ctx->r6 = ADD32(ctx->r5, 0X4);
    // 0x801CDEFC: addiu       $s0, $zero, 0x10
    ctx->r16 = ADD32(0, 0X10);
    // 0x801CDF00: lui         $t5, 0x400
    ctx->r13 = S32(0X400 << 16);
    // 0x801CDF04: lui         $t4, 0x2000
    ctx->r12 = S32(0X2000 << 16);
    // 0x801CDF08: lui         $t3, 0x800
    ctx->r11 = S32(0X800 << 16);
    // 0x801CDF0C: lui         $t2, 0xC000
    ctx->r10 = S32(0XC000 << 16);
    // 0x801CDF10: lui         $t1, 0x8000
    ctx->r9 = S32(0X8000 << 16);
    // 0x801CDF14: lui         $t0, 0x1000
    ctx->r8 = S32(0X1000 << 16);
L_801CDF18:
    // 0x801CDF18: lw          $t8, 0x8($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X8);
    // 0x801CDF1C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801CDF20: addiu       $a1, $a1, 0x8
    ctx->r5 = ADD32(ctx->r5, 0X8);
    // 0x801CDF24: lw          $t9, 0x5C($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X5C);
    // 0x801CDF28: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x801CDF2C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x801CDF30: and         $t6, $t9, $t0
    ctx->r14 = ctx->r25 & ctx->r8;
    // 0x801CDF34: beq         $t6, $zero, L_801CDF8C
    if (ctx->r14 == 0) {
        // 0x801CDF38: nop

            goto L_801CDF8C;
    }
    // 0x801CDF38: nop

    // 0x801CDF3C: lw          $t7, 0x4($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X4);
    // 0x801CDF40: or          $v1, $t1, $zero
    ctx->r3 = ctx->r9 | 0;
    // 0x801CDF44: lw          $a3, 0x5C($t7)
    ctx->r7 = MEM_W(ctx->r15, 0X5C);
    // 0x801CDF48: andi        $t8, $a3, 0x1
    ctx->r24 = ctx->r7 & 0X1;
    // 0x801CDF4C: beq         $t8, $zero, L_801CDF58
    if (ctx->r24 == 0) {
        // 0x801CDF50: and         $t9, $a3, $t3
        ctx->r25 = ctx->r7 & ctx->r11;
            goto L_801CDF58;
    }
    // 0x801CDF50: and         $t9, $a3, $t3
    ctx->r25 = ctx->r7 & ctx->r11;
    // 0x801CDF54: or          $v1, $t2, $zero
    ctx->r3 = ctx->r10 | 0;
L_801CDF58:
    // 0x801CDF58: beq         $t9, $zero, L_801CDF64
    if (ctx->r25 == 0) {
        // 0x801CDF5C: and         $t6, $a3, $t5
        ctx->r14 = ctx->r7 & ctx->r13;
            goto L_801CDF64;
    }
    // 0x801CDF5C: and         $t6, $a3, $t5
    ctx->r14 = ctx->r7 & ctx->r13;
    // 0x801CDF60: or          $v1, $v1, $t4
    ctx->r3 = ctx->r3 | ctx->r12;
L_801CDF64:
    // 0x801CDF64: beq         $t6, $zero, L_801CDF70
    if (ctx->r14 == 0) {
        // 0x801CDF68: andi        $t7, $a3, 0x8
        ctx->r15 = ctx->r7 & 0X8;
            goto L_801CDF70;
    }
    // 0x801CDF68: andi        $t7, $a3, 0x8
    ctx->r15 = ctx->r7 & 0X8;
    // 0x801CDF6C: or          $v1, $v1, $t0
    ctx->r3 = ctx->r3 | ctx->r8;
L_801CDF70:
    // 0x801CDF70: beq         $t7, $zero, L_801CDF7C
    if (ctx->r15 == 0) {
        // 0x801CDF74: andi        $t8, $a3, 0x4
        ctx->r24 = ctx->r7 & 0X4;
            goto L_801CDF7C;
    }
    // 0x801CDF74: andi        $t8, $a3, 0x4
    ctx->r24 = ctx->r7 & 0X4;
    // 0x801CDF78: or          $v1, $v1, $t3
    ctx->r3 = ctx->r3 | ctx->r11;
L_801CDF7C:
    // 0x801CDF7C: beql        $t8, $zero, L_801CDF90
    if (ctx->r24 == 0) {
        // 0x801CDF80: addiu       $a2, $a2, 0x8
        ctx->r6 = ADD32(ctx->r6, 0X8);
            goto L_801CDF90;
    }
    goto skip_0;
    // 0x801CDF80: addiu       $a2, $a2, 0x8
    ctx->r6 = ADD32(ctx->r6, 0X8);
    skip_0:
    // 0x801CDF84: b           L_801CDF8C
    // 0x801CDF88: or          $v1, $v1, $t5
    ctx->r3 = ctx->r3 | ctx->r13;
        goto L_801CDF8C;
    // 0x801CDF88: or          $v1, $v1, $t5
    ctx->r3 = ctx->r3 | ctx->r13;
L_801CDF8C:
    // 0x801CDF8C: addiu       $a2, $a2, 0x8
    ctx->r6 = ADD32(ctx->r6, 0X8);
L_801CDF90:
    // 0x801CDF90: bne         $a0, $s0, L_801CDF18
    if (ctx->r4 != ctx->r16) {
        // 0x801CDF94: sw          $v1, 0x0($v0)
        MEM_W(0X0, ctx->r2) = ctx->r3;
            goto L_801CDF18;
    }
    // 0x801CDF94: sw          $v1, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r3;
L_801CDF98:
    // 0x801CDF98: lw          $s0, 0x4($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X4);
    // 0x801CDF9C: jr          $ra
    // 0x801CDFA0: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
    return;
    // 0x801CDFA0: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
;}
RECOMP_FUNC void overlay_system_1_func_801CDFA4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CDFA4: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801CDFA8: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x801CDFAC: lw          $v1, 0x2968($t0)
    ctx->r3 = MEM_W(ctx->r8, 0X2968);
    // 0x801CDFB0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x801CDFB4: lw          $v0, 0x2964($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X2964);
    // 0x801CDFB8: lhu         $a1, 0xC($v1)
    ctx->r5 = MEM_HU(ctx->r3, 0XC);
    // 0x801CDFBC: addiu       $a0, $v1, 0x2CC
    ctx->r4 = ADD32(ctx->r3, 0X2CC);
    // 0x801CDFC0: andi        $t6, $a1, 0x8880
    ctx->r14 = ctx->r5 & 0X8880;
    // 0x801CDFC4: bne         $t6, $zero, L_801CE110
    if (ctx->r14 != 0) {
        // 0x801CDFC8: nop

            goto L_801CE110;
    }
    // 0x801CDFC8: nop

    // 0x801CDFCC: lw          $t7, 0x0($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X0);
    // 0x801CDFD0: sll         $t8, $t7, 3
    ctx->r24 = S32(ctx->r15 << 3);
    // 0x801CDFD4: addu        $a2, $a0, $t8
    ctx->r6 = ADD32(ctx->r4, ctx->r24);
    // 0x801CDFD8: lw          $a3, 0x4($a2)
    ctx->r7 = MEM_W(ctx->r6, 0X4);
    // 0x801CDFDC: sll         $t9, $a3, 0
    ctx->r25 = S32(ctx->r7 << 0);
    // 0x801CDFE0: bltz        $t9, L_801CE09C
    if (SIGNED(ctx->r25) < 0) {
        // 0x801CDFE4: sll         $t8, $a3, 1
        ctx->r24 = S32(ctx->r7 << 1);
            goto L_801CE09C;
    }
    // 0x801CDFE4: sll         $t8, $a3, 1
    ctx->r24 = S32(ctx->r7 << 1);
    // 0x801CDFE8: lw          $t1, 0x0($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X0);
    // 0x801CDFEC: lui         $at, 0x8003
    ctx->r1 = S32(0X8003 << 16);
    // 0x801CDFF0: ori         $at, $at, 0xC000
    ctx->r1 = ctx->r1 | 0XC000;
    // 0x801CDFF4: and         $t2, $t1, $at
    ctx->r10 = ctx->r9 & ctx->r1;
    // 0x801CDFF8: bne         $t2, $zero, L_801CE110
    if (ctx->r10 != 0) {
        // 0x801CDFFC: lui         $v0, 0x800F
        ctx->r2 = S32(0X800F << 16);
            goto L_801CE110;
    }
    // 0x801CDFFC: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x801CE000: addiu       $v0, $v0, 0x35F0
    ctx->r2 = ADD32(ctx->r2, 0X35F0);
    // 0x801CE004: lhu         $t3, 0x4($v0)
    ctx->r11 = MEM_HU(ctx->r2, 0X4);
    // 0x801CE008: beq         $t3, $zero, L_801CE110
    if (ctx->r11 == 0) {
        // 0x801CE00C: nop

            goto L_801CE110;
    }
    // 0x801CE00C: nop

    // 0x801CE010: lhu         $t4, 0x6($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0X6);
    // 0x801CE014: andi        $t5, $t4, 0x4000
    ctx->r13 = ctx->r12 & 0X4000;
    // 0x801CE018: beq         $t5, $zero, L_801CE110
    if (ctx->r13 == 0) {
        // 0x801CE01C: nop

            goto L_801CE110;
    }
    // 0x801CE01C: nop

    // 0x801CE020: lh          $t6, 0x2AEA($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X2AEA);
    // 0x801CE024: bne         $t6, $zero, L_801CE110
    if (ctx->r14 != 0) {
        // 0x801CE028: nop

            goto L_801CE110;
    }
    // 0x801CE028: nop

    // 0x801CE02C: lh          $v0, 0x2B62($t0)
    ctx->r2 = MEM_H(ctx->r8, 0X2B62);
    // 0x801CE030: lh          $t7, 0x12A($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X12A);
    // 0x801CE034: beql        $t7, $v0, L_801CE08C
    if (ctx->r15 == ctx->r2) {
        // 0x801CE038: lw          $a1, 0x8($a2)
        ctx->r5 = MEM_W(ctx->r6, 0X8);
            goto L_801CE08C;
    }
    goto skip_0;
    // 0x801CE038: lw          $a1, 0x8($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X8);
    skip_0:
    // 0x801CE03C: sh          $v0, 0x12A($v1)
    MEM_H(0X12A, ctx->r3) = ctx->r2;
    // 0x801CE040: lw          $t8, 0x0($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X0);
    // 0x801CE044: sll         $t9, $t8, 3
    ctx->r25 = S32(ctx->r24 << 3);
    // 0x801CE048: addu        $a2, $a0, $t9
    ctx->r6 = ADD32(ctx->r4, ctx->r25);
    // 0x801CE04C: lw          $a1, 0x8($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X8);
    // 0x801CE050: beql        $a1, $zero, L_801CE08C
    if (ctx->r5 == 0) {
        // 0x801CE054: lw          $a1, 0x8($a2)
        ctx->r5 = MEM_W(ctx->r6, 0X8);
            goto L_801CE08C;
    }
    goto skip_1;
    // 0x801CE054: lw          $a1, 0x8($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X8);
    skip_1:
    // 0x801CE058: lw          $v0, 0x54($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X54);
    // 0x801CE05C: beql        $v0, $zero, L_801CE08C
    if (ctx->r2 == 0) {
        // 0x801CE060: lw          $a1, 0x8($a2)
        ctx->r5 = MEM_W(ctx->r6, 0X8);
            goto L_801CE08C;
    }
    goto skip_2;
    // 0x801CE060: lw          $a1, 0x8($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X8);
    skip_2:
    // 0x801CE064: lh          $t1, 0x12A($v1)
    ctx->r9 = MEM_H(ctx->r3, 0X12A);
    // 0x801CE068: lui         $t3, 0x801D
    ctx->r11 = S32(0X801D << 16);
    // 0x801CE06C: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x801CE070: addu        $t3, $t3, $t2
    ctx->r11 = ADD32(ctx->r11, ctx->r10);
    // 0x801CE074: lh          $t3, 0x7C70($t3)
    ctx->r11 = MEM_H(ctx->r11, 0X7C70);
    // 0x801CE078: sh          $t3, 0x1E($v0)
    MEM_H(0X1E, ctx->r2) = ctx->r11;
    // 0x801CE07C: lw          $t4, 0x0($a0)
    ctx->r12 = MEM_W(ctx->r4, 0X0);
    // 0x801CE080: sll         $t5, $t4, 3
    ctx->r13 = S32(ctx->r12 << 3);
    // 0x801CE084: addu        $a2, $a0, $t5
    ctx->r6 = ADD32(ctx->r4, ctx->r13);
    // 0x801CE088: lw          $a1, 0x8($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X8);
L_801CE08C:
    // 0x801CE08C: lw          $t6, 0x5C($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X5C);
    // 0x801CE090: ori         $t7, $t6, 0x1
    ctx->r15 = ctx->r14 | 0X1;
    // 0x801CE094: jr          $ra
    // 0x801CE098: sw          $t7, 0x5C($a1)
    MEM_W(0X5C, ctx->r5) = ctx->r15;
    return;
    // 0x801CE098: sw          $t7, 0x5C($a1)
    MEM_W(0X5C, ctx->r5) = ctx->r15;
L_801CE09C:
    // 0x801CE09C: bgez        $t8, L_801CE110
    if (SIGNED(ctx->r24) >= 0) {
        // 0x801CE0A0: lui         $v1, 0x4000
        ctx->r3 = S32(0X4000 << 16);
            goto L_801CE110;
    }
    // 0x801CE0A0: lui         $v1, 0x4000
    ctx->r3 = S32(0X4000 << 16);
    // 0x801CE0A4: lw          $t9, 0x0($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X0);
    // 0x801CE0A8: lui         $at, 0x8003
    ctx->r1 = S32(0X8003 << 16);
    // 0x801CE0AC: ori         $at, $at, 0xC000
    ctx->r1 = ctx->r1 | 0XC000;
    // 0x801CE0B0: and         $t1, $t9, $at
    ctx->r9 = ctx->r25 & ctx->r1;
    // 0x801CE0B4: bne         $t1, $zero, L_801CE0F8
    if (ctx->r9 != 0) {
        // 0x801CE0B8: andi        $t7, $a1, 0x8
        ctx->r15 = ctx->r5 & 0X8;
            goto L_801CE0F8;
    }
    // 0x801CE0B8: andi        $t7, $a1, 0x8
    ctx->r15 = ctx->r5 & 0X8;
    // 0x801CE0BC: lh          $t2, 0x2AE4($t0)
    ctx->r10 = MEM_H(ctx->r8, 0X2AE4);
    // 0x801CE0C0: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x801CE0C4: addiu       $v0, $v0, 0x35F0
    ctx->r2 = ADD32(ctx->r2, 0X35F0);
    // 0x801CE0C8: bnel        $t2, $zero, L_801CE0F0
    if (ctx->r10 != 0) {
        // 0x801CE0CC: lh          $t6, 0x2AEA($t0)
        ctx->r14 = MEM_H(ctx->r8, 0X2AEA);
            goto L_801CE0F0;
    }
    goto skip_3;
    // 0x801CE0CC: lh          $t6, 0x2AEA($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X2AEA);
    skip_3:
    // 0x801CE0D0: lhu         $t3, 0x4($v0)
    ctx->r11 = MEM_HU(ctx->r2, 0X4);
    // 0x801CE0D4: beq         $t3, $zero, L_801CE0F8
    if (ctx->r11 == 0) {
        // 0x801CE0D8: nop

            goto L_801CE0F8;
    }
    // 0x801CE0D8: nop

    // 0x801CE0DC: lhu         $t4, 0x6($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0X6);
    // 0x801CE0E0: andi        $t5, $t4, 0x4000
    ctx->r13 = ctx->r12 & 0X4000;
    // 0x801CE0E4: beq         $t5, $zero, L_801CE0F8
    if (ctx->r13 == 0) {
        // 0x801CE0E8: nop

            goto L_801CE0F8;
    }
    // 0x801CE0E8: nop

    // 0x801CE0EC: lh          $t6, 0x2AEA($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X2AEA);
L_801CE0F0:
    // 0x801CE0F0: beq         $t6, $zero, L_801CE110
    if (ctx->r14 == 0) {
        // 0x801CE0F4: nop

            goto L_801CE110;
    }
    // 0x801CE0F4: nop

L_801CE0F8:
    // 0x801CE0F8: bne         $t7, $zero, L_801CE110
    if (ctx->r15 != 0) {
        // 0x801CE0FC: nop

            goto L_801CE110;
    }
    // 0x801CE0FC: nop

    // 0x801CE100: lw          $a1, 0x8($a2)
    ctx->r5 = MEM_W(ctx->r6, 0X8);
    // 0x801CE104: lw          $t8, 0x5C($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X5C);
    // 0x801CE108: or          $t9, $t8, $v1
    ctx->r25 = ctx->r24 | ctx->r3;
    // 0x801CE10C: sw          $t9, 0x5C($a1)
    MEM_W(0X5C, ctx->r5) = ctx->r25;
L_801CE110:
    // 0x801CE110: jr          $ra
    // 0x801CE114: nop

    return;
    // 0x801CE114: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CE118(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CE118: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CE11C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CE120: lhu         $t6, 0x2($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X2);
    // 0x801CE124: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CE128: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x801CE12C: andi        $t7, $t6, 0x1000
    ctx->r15 = ctx->r14 & 0X1000;
    // 0x801CE130: beq         $t7, $zero, L_801CE1FC
    if (ctx->r15 == 0) {
        // 0x801CE134: addiu       $v1, $v1, -0x7D40
        ctx->r3 = ADD32(ctx->r3, -0X7D40);
            goto L_801CE1FC;
    }
    // 0x801CE134: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801CE138: lw          $v0, 0x2968($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X2968);
    // 0x801CE13C: lw          $t1, 0x24($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X24);
    // 0x801CE140: lw          $t8, 0x2C8($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2C8);
    // 0x801CE144: sll         $t9, $t8, 0
    ctx->r25 = S32(ctx->r24 << 0);
    // 0x801CE148: bltzl       $t9, L_801CE25C
    if (SIGNED(ctx->r25) < 0) {
        // 0x801CE14C: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801CE25C;
    }
    goto skip_0;
    // 0x801CE14C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801CE150: lw          $t2, 0x2BC8($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X2BC8);
    // 0x801CE154: lui         $t5, 0x801D
    ctx->r13 = S32(0X801D << 16);
    // 0x801CE158: beq         $t2, $zero, L_801CE1E8
    if (ctx->r10 == 0) {
        // 0x801CE15C: nop

            goto L_801CE1E8;
    }
    // 0x801CE15C: nop

    // 0x801CE160: lui         $t5, 0x801D
    ctx->r13 = S32(0X801D << 16);
    // 0x801CE164: lw          $t5, 0x7CA4($t5)
    ctx->r13 = MEM_W(ctx->r13, 0X7CA4);
    // 0x801CE168: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE16C: bnel        $t5, $zero, L_801CE25C
    if (ctx->r13 != 0) {
        // 0x801CE170: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_801CE25C;
    }
    goto skip_1;
    // 0x801CE170: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x801CE174: sw          $t2, 0x7CA4($at)
    MEM_W(0X7CA4, ctx->r1) = ctx->r10;
    // 0x801CE178: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE17C: sh          $zero, 0x7C78($at)
    MEM_H(0X7C78, ctx->r1) = 0;
    // 0x801CE180: lh          $t6, 0x0($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X0);
    // 0x801CE184: lw          $v0, 0x14($t1)
    ctx->r2 = MEM_W(ctx->r9, 0X14);
    // 0x801CE188: addiu       $t3, $zero, -0x8000
    ctx->r11 = ADD32(0, -0X8000);
    // 0x801CE18C: or          $t7, $t6, $t3
    ctx->r15 = ctx->r14 | ctx->r11;
    // 0x801CE190: sh          $t7, 0x0($t1)
    MEM_H(0X0, ctx->r9) = ctx->r15;
    // 0x801CE194: lh          $t8, 0x0($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X0);
    // 0x801CE198: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CE19C: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801CE1A0: or          $t9, $t8, $t3
    ctx->r25 = ctx->r24 | ctx->r11;
    // 0x801CE1A4: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
    // 0x801CE1A8: lw          $v1, 0x7C88($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X7C88);
    // 0x801CE1AC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CE1B0: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x801CE1B4: lh          $t5, 0x0($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X0);
    // 0x801CE1B8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x801CE1BC: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x801CE1C0: andi        $t6, $t5, 0xFAFF
    ctx->r14 = ctx->r13 & 0XFAFF;
    // 0x801CE1C4: sh          $t6, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r14;
    // 0x801CE1C8: lw          $t0, 0x7C8C($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X7C8C);
    // 0x801CE1CC: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x801CE1D0: lh          $t7, 0x0($t0)
    ctx->r15 = MEM_H(ctx->r8, 0X0);
    // 0x801CE1D4: andi        $t8, $t7, 0xFAFF
    ctx->r24 = ctx->r15 & 0XFAFF;
    // 0x801CE1D8: jalr        $t9
    // 0x801CE1DC: sh          $t8, 0x0($t0)
    MEM_H(0X0, ctx->r8) = ctx->r24;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801CE1DC: sh          $t8, 0x0($t0)
    MEM_H(0X0, ctx->r8) = ctx->r24;
    after_0:
    // 0x801CE1E0: b           L_801CE25C
    // 0x801CE1E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801CE25C;
    // 0x801CE1E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CE1E8:
    // 0x801CE1E8: lw          $t5, 0x7CA4($t5)
    ctx->r13 = MEM_W(ctx->r13, 0X7CA4);
    // 0x801CE1EC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE1F0: beql        $t5, $zero, L_801CE200
    if (ctx->r13 == 0) {
        // 0x801CE1F4: lh          $v1, 0xE($a3)
        ctx->r3 = MEM_H(ctx->r7, 0XE);
            goto L_801CE200;
    }
    goto skip_2;
    // 0x801CE1F4: lh          $v1, 0xE($a3)
    ctx->r3 = MEM_H(ctx->r7, 0XE);
    skip_2:
    // 0x801CE1F8: sw          $zero, 0x7CA4($at)
    MEM_W(0X7CA4, ctx->r1) = 0;
L_801CE1FC:
    // 0x801CE1FC: lh          $v1, 0xE($a3)
    ctx->r3 = MEM_H(ctx->r7, 0XE);
L_801CE200:
    // 0x801CE200: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x801CE204: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x801CE208: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x801CE20C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x801CE210: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x801CE214: addu        $v0, $a3, $t6
    ctx->r2 = ADD32(ctx->r7, ctx->r14);
    // 0x801CE218: sh          $v1, 0xE($a3)
    MEM_H(0XE, ctx->r7) = ctx->r3;
    // 0x801CE21C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801CE220: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801CE224: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801CE228: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    // 0x801CE22C: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801CE230: sll         $t5, $t9, 2
    ctx->r13 = S32(ctx->r25 << 2);
    // 0x801CE234: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CE238: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x801CE23C: lw          $t9, 0x7C90($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C90);
    // 0x801CE240: jalr        $t9
    // 0x801CE244: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CE244: nop

    after_1:
    // 0x801CE248: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x801CE24C: lh          $t6, 0xE($a3)
    ctx->r14 = MEM_H(ctx->r7, 0XE);
    // 0x801CE250: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x801CE254: sh          $t7, 0xE($a3)
    MEM_H(0XE, ctx->r7) = ctx->r15;
    // 0x801CE258: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CE25C:
    // 0x801CE25C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CE260: jr          $ra
    // 0x801CE264: nop

    return;
    // 0x801CE264: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CE268(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CE268: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801CE26C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CE270: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CE274: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CE278: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x801CE27C: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x801CE280: jal         0x80005A30
    // 0x801CE284: addiu       $a0, $zero, 0x210
    ctx->r4 = ADD32(0, 0X210);
    sceneLookup(rdram, ctx);
        goto after_0;
    // 0x801CE284: addiu       $a0, $zero, 0x210
    ctx->r4 = ADD32(0, 0X210);
    after_0:
    // 0x801CE288: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x801CE28C: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x801CE290: lhu         $t8, 0x2($a0)
    ctx->r24 = MEM_HU(ctx->r4, 0X2);
    // 0x801CE294: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x801CE298: lui         $at, 0xFF
    ctx->r1 = S32(0XFF << 16);
    // 0x801CE29C: lh          $t0, 0x0($a0)
    ctx->r8 = MEM_H(ctx->r4, 0X0);
    // 0x801CE2A0: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801CE2A4: addiu       $v0, $v0, 0x58B8
    ctx->r2 = ADD32(ctx->r2, 0X58B8);
    // 0x801CE2A8: lui         $t6, 0x600
    ctx->r14 = S32(0X600 << 16);
    // 0x801CE2AC: addiu       $t7, $zero, 0xBE
    ctx->r15 = ADD32(0, 0XBE);
    // 0x801CE2B0: ori         $t2, $t8, 0x80
    ctx->r10 = ctx->r24 | 0X80;
    // 0x801CE2B4: and         $v0, $v0, $at
    ctx->r2 = ctx->r2 & ctx->r1;
    // 0x801CE2B8: addiu       $t6, $t6, 0x30C0
    ctx->r14 = ADD32(ctx->r14, 0X30C0);
    // 0x801CE2BC: sw          $t7, 0x40($a0)
    MEM_W(0X40, ctx->r4) = ctx->r15;
    // 0x801CE2C0: ori         $t5, $t2, 0x800
    ctx->r13 = ctx->r10 | 0X800;
    // 0x801CE2C4: sw          $t6, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = ctx->r14;
    // 0x801CE2C8: sh          $t2, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r10;
    // 0x801CE2CC: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x801CE2D0: lui         $t7, 0xD0
    ctx->r15 = S32(0XD0 << 16);
    // 0x801CE2D4: sh          $t5, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r13;
    // 0x801CE2D8: addiu       $t4, $zero, -0x1
    ctx->r12 = ADD32(0, -0X1);
    // 0x801CE2DC: ori         $t6, $t5, 0x400
    ctx->r14 = ctx->r13 | 0X400;
    // 0x801CE2E0: ori         $t7, $t7, 0xFF
    ctx->r15 = ctx->r15 | 0XFF;
    // 0x801CE2E4: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x801CE2E8: sw          $v0, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->r2;
    // 0x801CE2EC: sh          $t1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r9;
    // 0x801CE2F0: sw          $t4, 0x18($a0)
    MEM_W(0X18, ctx->r4) = ctx->r12;
    // 0x801CE2F4: sh          $t6, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r14;
    // 0x801CE2F8: sw          $t7, 0x1C($a0)
    MEM_W(0X1C, ctx->r4) = ctx->r15;
    // 0x801CE2FC: lui         $v1, 0x8004
    ctx->r3 = S32(0X8004 << 16);
    // 0x801CE300: addiu       $v1, $v1, 0x724
    ctx->r3 = ADD32(ctx->r3, 0X724);
    // 0x801CE304: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    // 0x801CE308: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x801CE30C: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x801CE310: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801CE314: jalr        $v1
    // 0x801CE318: addiu       $a2, $zero, 0x500
    ctx->r6 = ADD32(0, 0X500);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_1;
    // 0x801CE318: addiu       $a2, $zero, 0x500
    ctx->r6 = ADD32(0, 0X500);
    after_1:
    // 0x801CE31C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE320: sw          $v0, 0x7C88($at)
    MEM_W(0X7C88, ctx->r1) = ctx->r2;
    // 0x801CE324: lw          $a3, 0x34($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X34);
    // 0x801CE328: bne         $v0, $zero, L_801CE348
    if (ctx->r2 != 0) {
        // 0x801CE32C: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_801CE348;
    }
    // 0x801CE32C: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x801CE330: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x801CE334: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CE338: jalr        $t9
    // 0x801CE33C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CE33C: nop

    after_2:
    // 0x801CE340: b           L_801CE4F4
    // 0x801CE344: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CE4F4;
    // 0x801CE344: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CE348:
    // 0x801CE348: lhu         $t1, 0xA($v1)
    ctx->r9 = MEM_HU(ctx->r3, 0XA);
    // 0x801CE34C: lh          $t8, 0x0($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X0);
    // 0x801CE350: lui         $at, 0xC120
    ctx->r1 = S32(0XC120 << 16);
    // 0x801CE354: andi        $t6, $t1, 0xF9FF
    ctx->r14 = ctx->r9 & 0XF9FF;
    // 0x801CE358: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801CE35C: sh          $t6, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r14;
    // 0x801CE360: lui         $at, 0x4248
    ctx->r1 = S32(0X4248 << 16);
    // 0x801CE364: andi        $t9, $t6, 0x8FFF
    ctx->r25 = ctx->r14 & 0X8FFF;
    // 0x801CE368: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x801CE36C: sh          $t6, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r14;
    // 0x801CE370: andi        $t0, $t8, 0xFAFF
    ctx->r8 = ctx->r24 & 0XFAFF;
    // 0x801CE374: addiu       $t4, $zero, 0x2
    ctx->r12 = ADD32(0, 0X2);
    // 0x801CE378: addiu       $t5, $zero, 0x2BC
    ctx->r13 = ADD32(0, 0X2BC);
    // 0x801CE37C: ori         $t8, $t9, 0x3000
    ctx->r24 = ctx->r25 | 0X3000;
    // 0x801CE380: sh          $t9, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r25;
    // 0x801CE384: sh          $t0, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r8;
    // 0x801CE388: sh          $t4, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r12;
    // 0x801CE38C: sh          $t5, 0x6($v1)
    MEM_H(0X6, ctx->r3) = ctx->r13;
    // 0x801CE390: sh          $t8, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r24;
    // 0x801CE394: addiu       $a0, $zero, 0x210
    ctx->r4 = ADD32(0, 0X210);
    // 0x801CE398: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x801CE39C: swc1        $f4, 0x58($v1)
    MEM_W(0X58, ctx->r3) = ctx->f4.u32l;
    // 0x801CE3A0: jal         0x80005A30
    // 0x801CE3A4: swc1        $f6, 0x60($v1)
    MEM_W(0X60, ctx->r3) = ctx->f6.u32l;
    sceneLookup(rdram, ctx);
        goto after_3;
    // 0x801CE3A4: swc1        $f6, 0x60($v1)
    MEM_W(0X60, ctx->r3) = ctx->f6.u32l;
    after_3:
    // 0x801CE3A8: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
    // 0x801CE3AC: lhu         $t3, 0x2($v0)
    ctx->r11 = MEM_HU(ctx->r2, 0X2);
    // 0x801CE3B0: lh          $t5, 0x0($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X0);
    // 0x801CE3B4: lui         $t1, 0x600
    ctx->r9 = S32(0X600 << 16);
    // 0x801CE3B8: addiu       $t2, $zero, 0xBE
    ctx->r10 = ADD32(0, 0XBE);
    // 0x801CE3BC: sw          $t0, 0x38($v0)
    MEM_W(0X38, ctx->r2) = ctx->r8;
    // 0x801CE3C0: ori         $t7, $t3, 0x80
    ctx->r15 = ctx->r11 | 0X80;
    // 0x801CE3C4: addiu       $t1, $t1, 0x30C0
    ctx->r9 = ADD32(ctx->r9, 0X30C0);
    // 0x801CE3C8: sw          $t2, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r10;
    // 0x801CE3CC: ori         $t0, $t7, 0x800
    ctx->r8 = ctx->r15 | 0X800;
    // 0x801CE3D0: sw          $t1, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r9;
    // 0x801CE3D4: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x801CE3D8: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x801CE3DC: lui         $t2, 0xD0
    ctx->r10 = S32(0XD0 << 16);
    // 0x801CE3E0: sh          $t0, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r8;
    // 0x801CE3E4: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    // 0x801CE3E8: ori         $t1, $t0, 0x400
    ctx->r9 = ctx->r8 | 0X400;
    // 0x801CE3EC: ori         $t2, $t2, 0xFF
    ctx->r10 = ctx->r10 | 0XFF;
    // 0x801CE3F0: or          $t6, $t5, $at
    ctx->r14 = ctx->r13 | ctx->r1;
    // 0x801CE3F4: sh          $t6, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r14;
    // 0x801CE3F8: sw          $t8, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r24;
    // 0x801CE3FC: sh          $t1, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r9;
    // 0x801CE400: sw          $t2, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->r10;
    // 0x801CE404: lw          $t9, 0x20($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X20);
    // 0x801CE408: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x801CE40C: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x801CE410: jalr        $t9
    // 0x801CE414: addiu       $a2, $zero, 0x500
    ctx->r6 = ADD32(0, 0X500);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CE414: addiu       $a2, $zero, 0x500
    ctx->r6 = ADD32(0, 0X500);
    after_4:
    // 0x801CE418: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE41C: sw          $v0, 0x7C8C($at)
    MEM_W(0X7C8C, ctx->r1) = ctx->r2;
    // 0x801CE420: bne         $v0, $zero, L_801CE440
    if (ctx->r2 != 0) {
        // 0x801CE424: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_801CE440;
    }
    // 0x801CE424: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x801CE428: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x801CE42C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CE430: jalr        $t9
    // 0x801CE434: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x801CE434: nop

    after_5:
    // 0x801CE438: b           L_801CE4F4
    // 0x801CE43C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CE4F4;
    // 0x801CE43C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CE440:
    // 0x801CE440: lhu         $t5, 0xA($v1)
    ctx->r13 = MEM_HU(ctx->r3, 0XA);
    // 0x801CE444: lui         $at, 0xC120
    ctx->r1 = S32(0XC120 << 16);
    // 0x801CE448: lh          $t3, 0x0($v1)
    ctx->r11 = MEM_H(ctx->r3, 0X0);
    // 0x801CE44C: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x801CE450: andi        $t1, $t5, 0xF9FF
    ctx->r9 = ctx->r13 & 0XF9FF;
    // 0x801CE454: lui         $at, 0x4248
    ctx->r1 = S32(0X4248 << 16);
    // 0x801CE458: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x801CE45C: sh          $t1, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r9;
    // 0x801CE460: andi        $t9, $t1, 0x8FFF
    ctx->r25 = ctx->r9 & 0X8FFF;
    // 0x801CE464: sh          $t1, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r9;
    // 0x801CE468: andi        $t4, $t3, 0xFAFF
    ctx->r12 = ctx->r11 & 0XFAFF;
    // 0x801CE46C: addiu       $t8, $zero, 0x2
    ctx->r24 = ADD32(0, 0X2);
    // 0x801CE470: addiu       $t0, $zero, 0x2BC
    ctx->r8 = ADD32(0, 0X2BC);
    // 0x801CE474: ori         $t3, $t9, 0x3000
    ctx->r11 = ctx->r25 | 0X3000;
    // 0x801CE478: sh          $t9, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r25;
    // 0x801CE47C: sh          $t4, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r12;
    // 0x801CE480: sh          $t8, 0x2($v1)
    MEM_H(0X2, ctx->r3) = ctx->r24;
    // 0x801CE484: sh          $t0, 0x6($v1)
    MEM_H(0X6, ctx->r3) = ctx->r8;
    // 0x801CE488: sh          $t3, 0xA($v1)
    MEM_H(0XA, ctx->r3) = ctx->r11;
    // 0x801CE48C: swc1        $f8, 0x58($v1)
    MEM_W(0X58, ctx->r3) = ctx->f8.u32l;
    // 0x801CE490: swc1        $f10, 0x60($v1)
    MEM_W(0X60, ctx->r3) = ctx->f10.u32l;
    // 0x801CE494: lhu         $t4, 0x2($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0X2);
    // 0x801CE498: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CE49C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CE4A0: ori         $t5, $t4, 0x1000
    ctx->r13 = ctx->r12 | 0X1000;
    // 0x801CE4A4: sh          $t5, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r13;
    // 0x801CE4A8: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CE4AC: jalr        $t9
    // 0x801CE4B0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x801CE4B0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_6:
    // 0x801CE4B4: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x801CE4B8: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CE4BC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CE4C0: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x801CE4C4: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x801CE4C8: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x801CE4CC: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x801CE4D0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CE4D4: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x801CE4D8: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x801CE4DC: sb          $t0, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r8;
    // 0x801CE4E0: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
    // 0x801CE4E4: lw          $t9, 0x7C90($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C90);
    // 0x801CE4E8: jalr        $t9
    // 0x801CE4EC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x801CE4EC: nop

    after_7:
    // 0x801CE4F0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CE4F4:
    // 0x801CE4F4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CE4F8: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x801CE4FC: jr          $ra
    // 0x801CE500: nop

    return;
    // 0x801CE500: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CE504(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CE504: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CE508: lw          $v0, -0x53D8($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53D8);
    // 0x801CE50C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801CE510: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CE514: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CE518: lhu         $t6, 0xC($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0XC);
    // 0x801CE51C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CE520: lw          $a1, 0x24($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X24);
    // 0x801CE524: andi        $t7, $t6, 0x4
    ctx->r15 = ctx->r14 & 0X4;
    // 0x801CE528: beq         $t7, $zero, L_801CE610
    if (ctx->r15 == 0) {
        // 0x801CE52C: lui         $at, 0x801D
        ctx->r1 = S32(0X801D << 16);
            goto L_801CE610;
    }
    // 0x801CE52C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE530: lh          $t8, 0x0($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X0);
    // 0x801CE534: addiu       $v1, $v0, 0x2CC
    ctx->r3 = ADD32(ctx->r2, 0X2CC);
    // 0x801CE538: lui         $a3, 0x4000
    ctx->r7 = S32(0X4000 << 16);
    // 0x801CE53C: andi        $t9, $t8, 0x7FFF
    ctx->r25 = ctx->r24 & 0X7FFF;
    // 0x801CE540: sh          $t9, 0x0($a1)
    MEM_H(0X0, ctx->r5) = ctx->r25;
    // 0x801CE544: lw          $t0, 0x0($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X0);
    // 0x801CE548: addiu       $a0, $v0, 0x1A4
    ctx->r4 = ADD32(ctx->r2, 0X1A4);
    // 0x801CE54C: addiu       $a2, $zero, 0x24
    ctx->r6 = ADD32(0, 0X24);
    // 0x801CE550: sll         $t1, $t0, 3
    ctx->r9 = S32(ctx->r8 << 3);
    // 0x801CE554: addu        $t2, $v1, $t1
    ctx->r10 = ADD32(ctx->r3, ctx->r9);
    // 0x801CE558: lw          $t3, 0x4($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X4);
    // 0x801CE55C: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x801CE560: bltz        $t4, L_801CE578
    if (SIGNED(ctx->r12) < 0) {
        // 0x801CE564: nop

            goto L_801CE578;
    }
    // 0x801CE564: nop

    // 0x801CE568: lw          $v1, 0x14($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X14);
    // 0x801CE56C: lh          $t5, 0x0($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X0);
    // 0x801CE570: andi        $t6, $t5, 0x7FFF
    ctx->r14 = ctx->r13 & 0X7FFF;
    // 0x801CE574: sh          $t6, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r14;
L_801CE578:
    // 0x801CE578: sh          $zero, 0x7C7C($at)
    MEM_H(0X7C7C, ctx->r1) = 0;
    // 0x801CE57C: lw          $t7, 0x184($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X184);
    // 0x801CE580: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x801CE584: addiu       $a1, $a1, 0x3FB0
    ctx->r5 = ADD32(ctx->r5, 0X3FB0);
    // 0x801CE588: or          $t8, $t7, $a3
    ctx->r24 = ctx->r15 | ctx->r7;
    // 0x801CE58C: jal         0x80001090
    // 0x801CE590: sw          $t8, 0x184($v0)
    MEM_W(0X184, ctx->r2) = ctx->r24;
    memory_copy(rdram, ctx);
        goto after_0;
    // 0x801CE590: sw          $t8, 0x184($v0)
    MEM_W(0X184, ctx->r2) = ctx->r24;
    after_0:
    // 0x801CE594: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x801CE598: lw          $a1, -0x53E0($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X53E0);
    // 0x801CE59C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801CE5A0: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x801CE5A4: addiu       $a0, $zero, 0x219
    ctx->r4 = ADD32(0, 0X219);
    // 0x801CE5A8: jalr        $t9
    // 0x801CE5AC: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CE5AC: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_1:
    // 0x801CE5B0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801CE5B4: addiu       $a0, $a0, 0x7CA8
    ctx->r4 = ADD32(ctx->r4, 0X7CA8);
    // 0x801CE5B8: jal         0x80021DDC
    // 0x801CE5BC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_80021DDC(rdram, ctx);
        goto after_2;
    // 0x801CE5BC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_2:
    // 0x801CE5C0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CE5C4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CE5C8: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CE5CC: jalr        $t9
    // 0x801CE5D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CE5D0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
    // 0x801CE5D4: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x801CE5D8: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CE5DC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CE5E0: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x801CE5E4: addu        $v0, $s0, $t1
    ctx->r2 = ADD32(ctx->r16, ctx->r9);
    // 0x801CE5E8: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x801CE5EC: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x801CE5F0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CE5F4: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x801CE5F8: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x801CE5FC: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x801CE600: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x801CE604: lw          $t9, 0x7C90($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C90);
    // 0x801CE608: jalr        $t9
    // 0x801CE60C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801CE60C: nop

    after_4:
L_801CE610:
    // 0x801CE610: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801CE614: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CE618: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801CE61C: jr          $ra
    // 0x801CE620: nop

    return;
    // 0x801CE620: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CE624(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CE624: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CE628: lw          $t6, -0x53D8($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53D8);
    // 0x801CE62C: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x801CE630: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CE634: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CE638: sw          $t6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r14;
    // 0x801CE63C: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x801CE640: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CE644: jal         0x801CEB14
    // 0x801CE648: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    overlay_system_1_func_801CEB14(rdram, ctx);
        goto after_0;
    // 0x801CE648: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    after_0:
    // 0x801CE64C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x801CE650: addiu       $a0, $a0, 0x7C78
    ctx->r4 = ADD32(ctx->r4, 0X7C78);
    // 0x801CE654: lh          $t7, 0x0($a0)
    ctx->r15 = MEM_H(ctx->r4, 0X0);
    // 0x801CE658: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x801CE65C: mtc1        $at, $f13
    ctx->f_odd[(13 - 1) * 2] = ctx->r1;
    // 0x801CE660: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801CE664: sh          $t8, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r24;
    // 0x801CE668: lh          $t9, 0x0($a0)
    ctx->r25 = MEM_H(ctx->r4, 0X0);
    // 0x801CE66C: lui         $at, 0x3FD0
    ctx->r1 = S32(0X3FD0 << 16);
    // 0x801CE670: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x801CE674: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x801CE678: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801CE67C: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x801CE680: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801CE684: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CE688: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x801CE68C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801CE690: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CE694: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CE698: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl);
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x801CE69C: mul.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d);
    ctx->f16.d = MUL_D(ctx->f8.d, ctx->f10.d);
    // 0x801CE6A0: cvt.s.d     $f2, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d);
    ctx->f2.fl = CVT_S_D(ctx->f16.d);
    // 0x801CE6A4: cvt.d.s     $f18, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl);
    ctx->f18.d = CVT_D_S(ctx->f2.fl);
    // 0x801CE6A8: c.lt.d      $f12, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f12.d < ctx->f18.d;
    // 0x801CE6AC: nop

    // 0x801CE6B0: bc1f        L_801CE6C0
    if (!c1cs) {
        // 0x801CE6B4: nop

            goto L_801CE6C0;
    }
    // 0x801CE6B4: nop

    // 0x801CE6B8: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x801CE6BC: nop

L_801CE6C0:
    // 0x801CE6C0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE6C4: lwc1        $f4, 0x7C84($at)
    ctx->f4.u32l = MEM_W(ctx->r1, 0X7C84);
    // 0x801CE6C8: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x801CE6CC: lui         $at, 0x4014
    ctx->r1 = S32(0X4014 << 16);
    // 0x801CE6D0: mul.s       $f2, $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f4.fl);
    ctx->f2.fl = MUL_S(ctx->f2.fl, ctx->f4.fl);
    // 0x801CE6D4: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x801CE6D8: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x801CE6DC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801CE6E0: swc1        $f2, 0x70($v1)
    MEM_W(0X70, ctx->r3) = ctx->f2.u32l;
    // 0x801CE6E4: swc1        $f2, 0x6C($v1)
    MEM_W(0X6C, ctx->r3) = ctx->f2.u32l;
    // 0x801CE6E8: swc1        $f2, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->f2.u32l;
    // 0x801CE6EC: swc1        $f2, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f2.u32l;
    // 0x801CE6F0: swc1        $f2, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f2.u32l;
    // 0x801CE6F4: swc1        $f2, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f2.u32l;
    // 0x801CE6F8: lh          $t0, 0x0($a0)
    ctx->r8 = MEM_H(ctx->r4, 0X0);
    // 0x801CE6FC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CE700: mtc1        $t0, $f6
    ctx->f6.u32l = ctx->r8;
    // 0x801CE704: nop

    // 0x801CE708: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801CE70C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl);
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x801CE710: div.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d);
    ctx->f18.d = DIV_D(ctx->f10.d, ctx->f16.d);
    // 0x801CE714: cvt.s.d     $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d);
    ctx->f2.fl = CVT_S_D(ctx->f18.d);
    // 0x801CE718: cvt.d.s     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl);
    ctx->f0.d = CVT_D_S(ctx->f2.fl);
    // 0x801CE71C: c.lt.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d < ctx->f0.d;
    // 0x801CE720: nop

    // 0x801CE724: bc1fl       L_801CE73C
    if (!c1cs) {
        // 0x801CE728: c.eq.d      $f12, $f0
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
            goto L_801CE73C;
    }
    goto skip_0;
    // 0x801CE728: c.eq.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
    skip_0:
    // 0x801CE72C: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x801CE730: nop

    // 0x801CE734: cvt.d.s     $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl);
    ctx->f0.d = CVT_D_S(ctx->f2.fl);
    // 0x801CE738: c.eq.d      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.d == ctx->f0.d;
L_801CE73C:
    // 0x801CE73C: nop

    // 0x801CE740: bc1fl       L_801CE758
    if (!c1cs) {
        // 0x801CE744: lw          $v0, 0x38($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X38);
            goto L_801CE758;
    }
    goto skip_1;
    // 0x801CE744: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
    skip_1:
    // 0x801CE748: jalr        $t9
    // 0x801CE74C: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CE74C: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    after_1:
    // 0x801CE750: lwc1        $f2, 0x30($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X30);
    // 0x801CE754: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
L_801CE758:
    // 0x801CE758: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x801CE75C: addiu       $a0, $a0, 0x3FB0
    ctx->r4 = ADD32(ctx->r4, 0X3FB0);
    // 0x801CE760: lhu         $t1, 0xE($v0)
    ctx->r9 = MEM_HU(ctx->r2, 0XE);
    // 0x801CE764: andi        $t2, $t1, 0x100
    ctx->r10 = ctx->r9 & 0X100;
    // 0x801CE768: beq         $t2, $zero, L_801CE82C
    if (ctx->r10 == 0) {
        // 0x801CE76C: nop

            goto L_801CE82C;
    }
    // 0x801CE76C: nop

    // 0x801CE770: lw          $t3, 0x0($a0)
    ctx->r11 = MEM_W(ctx->r4, 0X0);
    // 0x801CE774: srl         $t4, $t3, 29
    ctx->r12 = S32(U32(ctx->r11) >> 29);
    // 0x801CE778: beq         $t4, $zero, L_801CE82C
    if (ctx->r12 == 0) {
        // 0x801CE77C: nop

            goto L_801CE82C;
    }
    // 0x801CE77C: nop

    // 0x801CE780: lhu         $t5, 0xC($v0)
    ctx->r13 = MEM_HU(ctx->r2, 0XC);
    // 0x801CE784: andi        $t6, $t5, 0x400
    ctx->r14 = ctx->r13 & 0X400;
    // 0x801CE788: bne         $t6, $zero, L_801CE82C
    if (ctx->r14 != 0) {
        // 0x801CE78C: nop

            goto L_801CE82C;
    }
    // 0x801CE78C: nop

    // 0x801CE790: jal         0x8002109C
    // 0x801CE794: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    func_8002109C(rdram, ctx);
        goto after_2;
    // 0x801CE794: swc1        $f2, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f2.u32l;
    after_2:
    // 0x801CE798: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CE79C: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801CE7A0: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801CE7A4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CE7A8: lwc1        $f6, 0x50($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X50);
    // 0x801CE7AC: addiu       $t9, $t9, 0x4C24
    ctx->r25 = ADD32(ctx->r25, 0X4C24);
    // 0x801CE7B0: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CE7B4: swc1        $f8, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f8.u32l;
    // 0x801CE7B8: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801CE7BC: lwc1        $f16, 0x54($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X54);
    // 0x801CE7C0: lwc1        $f14, 0x20($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X20);
    // 0x801CE7C4: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x801CE7C8: swc1        $f18, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->f18.u32l;
    // 0x801CE7CC: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801CE7D0: lwc1        $f6, 0x58($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X58);
    // 0x801CE7D4: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CE7D8: swc1        $f8, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->f8.u32l;
    // 0x801CE7DC: lwc1        $f12, 0x28($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X28);
    // 0x801CE7E0: jalr        $t9
    // 0x801CE7E4: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl);
    ctx->f12.fl = -ctx->f12.fl;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CE7E4: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl);
    ctx->f12.fl = -ctx->f12.fl;
    after_3:
    // 0x801CE7E8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CE7EC: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x801CE7F0: lwc1        $f2, 0x30($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X30);
    // 0x801CE7F4: lh          $a0, 0x5E($v1)
    ctx->r4 = MEM_H(ctx->r3, 0X5E);
    // 0x801CE7F8: subu        $a1, $v0, $a0
    ctx->r5 = SUB32(ctx->r2, ctx->r4);
    // 0x801CE7FC: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x801CE800: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x801CE804: mtc1        $a1, $f18
    ctx->f18.u32l = ctx->r5;
    // 0x801CE808: mtc1        $a0, $f10
    ctx->f10.u32l = ctx->r4;
    // 0x801CE80C: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x801CE810: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801CE814: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl);
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x801CE818: add.s       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f16.fl + ctx->f6.fl;
    // 0x801CE81C: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x801CE820: mfc1        $t1, $f10
    ctx->r9 = (int32_t)ctx->f10.u32l;
    // 0x801CE824: nop

    // 0x801CE828: sh          $t1, 0x5E($v1)
    MEM_H(0X5E, ctx->r3) = ctx->r9;
L_801CE82C:
    // 0x801CE82C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE830: swc1        $f2, 0x7C80($at)
    MEM_W(0X7C80, ctx->r1) = ctx->f2.u32l;
    // 0x801CE834: jal         0x801CEB68
    // 0x801CE838: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    overlay_system_1_func_801CEB68(rdram, ctx);
        goto after_4;
    // 0x801CE838: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_4:
    // 0x801CE83C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801CE840: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CE844: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x801CE848: jr          $ra
    // 0x801CE84C: nop

    return;
    // 0x801CE84C: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CE850(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CE850: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CE854: lw          $v0, -0x53D8($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53D8);
    // 0x801CE858: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801CE85C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CE860: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x801CE864: jal         0x801CEB14
    // 0x801CE868: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    overlay_system_1_func_801CEB14(rdram, ctx);
        goto after_0;
    // 0x801CE868: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    after_0:
    // 0x801CE86C: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x801CE870: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x801CE874: addiu       $t8, $zero, 0x5
    ctx->r24 = ADD32(0, 0X5);
    // 0x801CE878: lhu         $t6, 0xC($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0XC);
    // 0x801CE87C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801CE880: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x801CE884: andi        $t7, $t6, 0x4
    ctx->r15 = ctx->r14 & 0X4;
    // 0x801CE888: bne         $t7, $zero, L_801CE904
    if (ctx->r15 != 0) {
        // 0x801CE88C: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_801CE904;
    }
    // 0x801CE88C: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x801CE890: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE894: sh          $t8, 0x7C78($at)
    MEM_H(0X7C78, ctx->r1) = ctx->r24;
    // 0x801CE898: lw          $t9, 0x184($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X184);
    // 0x801CE89C: lui         $at, 0xBFFF
    ctx->r1 = S32(0XBFFF << 16);
    // 0x801CE8A0: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x801CE8A4: and         $t0, $t9, $at
    ctx->r8 = ctx->r25 & ctx->r1;
    // 0x801CE8A8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CE8AC: sw          $t0, 0x184($v0)
    MEM_W(0X184, ctx->r2) = ctx->r8;
    // 0x801CE8B0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801CE8B4: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x801CE8B8: jalr        $t9
    // 0x801CE8BC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CE8BC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    after_1:
    // 0x801CE8C0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x801CE8C4: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CE8C8: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801CE8CC: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x801CE8D0: addu        $v0, $a0, $t2
    ctx->r2 = ADD32(ctx->r4, ctx->r10);
    // 0x801CE8D4: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801CE8D8: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801CE8DC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CE8E0: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801CE8E4: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801CE8E8: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x801CE8EC: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801CE8F0: lw          $t9, 0x7C90($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X7C90);
    // 0x801CE8F4: jalr        $t9
    // 0x801CE8F8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CE8F8: nop

    after_2:
    // 0x801CE8FC: b           L_801CE918
    // 0x801CE900: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801CE918;
    // 0x801CE900: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CE904:
    // 0x801CE904: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x801CE908: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE90C: jal         0x801CEB68
    // 0x801CE910: swc1        $f4, 0x7C80($at)
    MEM_W(0X7C80, ctx->r1) = ctx->f4.u32l;
    overlay_system_1_func_801CEB68(rdram, ctx);
        goto after_3;
    // 0x801CE910: swc1        $f4, 0x7C80($at)
    MEM_W(0X7C80, ctx->r1) = ctx->f4.u32l;
    after_3:
    // 0x801CE914: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801CE918:
    // 0x801CE918: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801CE91C: jr          $ra
    // 0x801CE920: nop

    return;
    // 0x801CE920: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CE924(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CE924: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CE928: lw          $t6, -0x53D8($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53D8);
    // 0x801CE92C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x801CE930: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801CE934: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x801CE938: sw          $t6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r14;
    // 0x801CE93C: lw          $t0, 0x24($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X24);
    // 0x801CE940: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x801CE944: jal         0x801CEB14
    // 0x801CE948: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    overlay_system_1_func_801CEB14(rdram, ctx);
        goto after_0;
    // 0x801CE948: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    after_0:
    // 0x801CE94C: lw          $t7, 0x28($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X28);
    // 0x801CE950: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
    // 0x801CE954: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x801CE958: lhu         $t8, 0xC($t7)
    ctx->r24 = MEM_HU(ctx->r15, 0XC);
    // 0x801CE95C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CE960: andi        $t9, $t8, 0x4
    ctx->r25 = ctx->r24 & 0X4;
    // 0x801CE964: beql        $t9, $zero, L_801CE9F0
    if (ctx->r25 == 0) {
        // 0x801CE968: lh          $t6, 0xE($s0)
        ctx->r14 = MEM_H(ctx->r16, 0XE);
            goto L_801CE9F0;
    }
    goto skip_0;
    // 0x801CE968: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    skip_0:
    // 0x801CE96C: lh          $t5, 0xE($s0)
    ctx->r13 = MEM_H(ctx->r16, 0XE);
    // 0x801CE970: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x801CE974: addiu       $t1, $t1, 0x7C78
    ctx->r9 = ADD32(ctx->r9, 0X7C78);
    // 0x801CE978: sll         $t6, $t5, 1
    ctx->r14 = S32(ctx->r13 << 1);
    // 0x801CE97C: addu        $t7, $s0, $t6
    ctx->r15 = ADD32(ctx->r16, ctx->r14);
    // 0x801CE980: lbu         $t8, 0x8($t7)
    ctx->r24 = MEM_BU(ctx->r15, 0X8);
    // 0x801CE984: lh          $t4, 0x0($t1)
    ctx->r12 = MEM_H(ctx->r9, 0X0);
    // 0x801CE988: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CE98C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x801CE990: subu        $t9, $t4, $t8
    ctx->r25 = SUB32(ctx->r12, ctx->r24);
    // 0x801CE994: addiu       $t5, $t9, 0x1
    ctx->r13 = ADD32(ctx->r25, 0X1);
    // 0x801CE998: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CE99C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CE9A0: jalr        $t9
    // 0x801CE9A4: sh          $t5, 0x0($t1)
    MEM_H(0X0, ctx->r9) = ctx->r13;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801CE9A4: sh          $t5, 0x0($t1)
    MEM_H(0X0, ctx->r9) = ctx->r13;
    after_1:
    // 0x801CE9A8: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x801CE9AC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CE9B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CE9B4: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x801CE9B8: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x801CE9BC: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801CE9C0: lbu         $t4, 0x8($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X8);
    // 0x801CE9C4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801CE9C8: sll         $t9, $t5, 2
    ctx->r25 = S32(ctx->r13 << 2);
    // 0x801CE9CC: addiu       $t8, $t4, 0x1
    ctx->r24 = ADD32(ctx->r12, 0X1);
    // 0x801CE9D0: sb          $t8, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r24;
    // 0x801CE9D4: addu        $at, $at, $t9
    ctx->r1 = ADD32(ctx->r1, ctx->r25);
    // 0x801CE9D8: lw          $t9, 0x7C90($at)
    ctx->r25 = MEM_W(ctx->r1, 0X7C90);
    // 0x801CE9DC: jalr        $t9
    // 0x801CE9E0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801CE9E0: nop

    after_2:
    // 0x801CE9E4: b           L_801CEB04
    // 0x801CE9E8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801CEB04;
    // 0x801CE9E8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801CE9EC: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
L_801CE9F0:
    // 0x801CE9F0: addiu       $t1, $t1, 0x7C78
    ctx->r9 = ADD32(ctx->r9, 0X7C78);
    // 0x801CE9F4: lh          $v0, 0x0($t1)
    ctx->r2 = MEM_H(ctx->r9, 0X0);
    // 0x801CE9F8: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x801CE9FC: addu        $t4, $s0, $t7
    ctx->r12 = ADD32(ctx->r16, ctx->r15);
    // 0x801CEA00: lbu         $t8, 0x8($t4)
    ctx->r24 = MEM_BU(ctx->r12, 0X8);
    // 0x801CEA04: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x801CEA08: mtc1        $zero, $f13
    ctx->f_odd[(13 - 1) * 2] = 0;
    // 0x801CEA0C: subu        $t5, $v0, $t8
    ctx->r13 = SUB32(ctx->r2, ctx->r24);
    // 0x801CEA10: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x801CEA14: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801CEA18: mtc1        $zero, $f12
    ctx->f12.u32l = 0;
    // 0x801CEA1C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x801CEA20: div.s       $f0, $f6, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl);
    ctx->f0.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x801CEA24: cvt.d.s     $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl);
    ctx->f2.d = CVT_D_S(ctx->f0.fl);
    // 0x801CEA28: c.lt.d      $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f2.d < ctx->f12.d;
    // 0x801CEA2C: nop

    // 0x801CEA30: bc1fl       L_801CEA48
    if (!c1cs) {
        // 0x801CEA34: c.eq.d      $f12, $f2
        CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.d == ctx->f2.d;
            goto L_801CEA48;
    }
    goto skip_1;
    // 0x801CEA34: c.eq.d      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.d == ctx->f2.d;
    skip_1:
    // 0x801CEA38: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801CEA3C: nop

    // 0x801CEA40: cvt.d.s     $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl);
    ctx->f2.d = CVT_D_S(ctx->f0.fl);
    // 0x801CEA44: c.eq.d      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.d == ctx->f2.d;
L_801CEA48:
    // 0x801CEA48: nop

    // 0x801CEA4C: bc1f        L_801CEAC8
    if (!c1cs) {
        // 0x801CEA50: nop

            goto L_801CEAC8;
    }
    // 0x801CEA50: nop

    // 0x801CEA54: lh          $t9, 0x0($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X0);
    // 0x801CEA58: lw          $v0, 0x14($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X14);
    // 0x801CEA5C: addiu       $t2, $zero, -0x8000
    ctx->r10 = ADD32(0, -0X8000);
    // 0x801CEA60: or          $t6, $t9, $t2
    ctx->r14 = ctx->r25 | ctx->r10;
    // 0x801CEA64: sh          $t6, 0x0($t0)
    MEM_H(0X0, ctx->r8) = ctx->r14;
    // 0x801CEA68: lh          $t7, 0x0($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X0);
    // 0x801CEA6C: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CEA70: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x801CEA74: or          $t4, $t7, $t2
    ctx->r12 = ctx->r15 | ctx->r10;
    // 0x801CEA78: sh          $t4, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r12;
    // 0x801CEA7C: sh          $zero, 0x0($t1)
    MEM_H(0X0, ctx->r9) = 0;
    // 0x801CEA80: lw          $v1, 0x7C88($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X7C88);
    // 0x801CEA84: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801CEA88: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x801CEA8C: lh          $t8, 0x0($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X0);
    // 0x801CEA90: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x801CEA94: andi        $t5, $t8, 0xFAFF
    ctx->r13 = ctx->r24 & 0XFAFF;
    // 0x801CEA98: sh          $t5, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r13;
    // 0x801CEA9C: lw          $a3, 0x7C8C($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X7C8C);
    // 0x801CEAA0: lh          $t9, 0x0($a3)
    ctx->r25 = MEM_H(ctx->r7, 0X0);
    // 0x801CEAA4: andi        $t6, $t9, 0xFAFF
    ctx->r14 = ctx->r25 & 0XFAFF;
    // 0x801CEAA8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801CEAAC: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x801CEAB0: sh          $t6, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r14;
    // 0x801CEAB4: swc1        $f0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f0.u32l;
    // 0x801CEAB8: jalr        $t9
    // 0x801CEABC: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801CEABC: sw          $t0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r8;
    after_3:
    // 0x801CEAC0: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
    // 0x801CEAC4: lwc1        $f0, 0x20($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X20);
L_801CEAC8:
    // 0x801CEAC8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CEACC: swc1        $f0, 0x7C80($at)
    MEM_W(0X7C80, ctx->r1) = ctx->f0.u32l;
    // 0x801CEAD0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CEAD4: lwc1        $f16, 0x7C84($at)
    ctx->f16.u32l = MEM_W(ctx->r1, 0X7C84);
    // 0x801CEAD8: lw          $v0, 0x14($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X14);
    // 0x801CEADC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801CEAE0: mul.s       $f0, $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f16.fl);
    // 0x801CEAE4: swc1        $f0, 0x70($t0)
    MEM_W(0X70, ctx->r8) = ctx->f0.u32l;
    // 0x801CEAE8: swc1        $f0, 0x6C($t0)
    MEM_W(0X6C, ctx->r8) = ctx->f0.u32l;
    // 0x801CEAEC: swc1        $f0, 0x68($t0)
    MEM_W(0X68, ctx->r8) = ctx->f0.u32l;
    // 0x801CEAF0: swc1        $f0, 0x68($v0)
    MEM_W(0X68, ctx->r2) = ctx->f0.u32l;
    // 0x801CEAF4: swc1        $f0, 0x6C($v0)
    MEM_W(0X6C, ctx->r2) = ctx->f0.u32l;
    // 0x801CEAF8: jal         0x801CEB68
    // 0x801CEAFC: swc1        $f0, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f0.u32l;
    overlay_system_1_func_801CEB68(rdram, ctx);
        goto after_4;
    // 0x801CEAFC: swc1        $f0, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->f0.u32l;
    after_4:
    // 0x801CEB00: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801CEB04:
    // 0x801CEB04: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801CEB08: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x801CEB0C: jr          $ra
    // 0x801CEB10: nop

    return;
    // 0x801CEB10: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CEB14(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CEB14: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801CEB18: lh          $t6, 0x7C7C($t6)
    ctx->r14 = MEM_H(ctx->r14, 0X7C7C);
    // 0x801CEB1C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801CEB20: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CEB24: slti        $at, $t6, 0x7
    ctx->r1 = SIGNED(ctx->r14) < 0X7 ? 1 : 0;
    // 0x801CEB28: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801CEB2C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801CEB30: beq         $at, $zero, L_801CEB58
    if (ctx->r1 == 0) {
        // 0x801CEB34: lw          $v0, -0x53D8($v0)
        ctx->r2 = MEM_W(ctx->r2, -0X53D8);
            goto L_801CEB58;
    }
    // 0x801CEB34: lw          $v0, -0x53D8($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53D8);
    // 0x801CEB38: lw          $t7, 0x184($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X184);
    // 0x801CEB3C: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x801CEB40: addiu       $a1, $a1, 0x3FB0
    ctx->r5 = ADD32(ctx->r5, 0X3FB0);
    // 0x801CEB44: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x801CEB48: bgez        $t8, L_801CEB58
    if (SIGNED(ctx->r24) >= 0) {
        // 0x801CEB4C: addiu       $a0, $v0, 0x1A4
        ctx->r4 = ADD32(ctx->r2, 0X1A4);
            goto L_801CEB58;
    }
    // 0x801CEB4C: addiu       $a0, $v0, 0x1A4
    ctx->r4 = ADD32(ctx->r2, 0X1A4);
    // 0x801CEB50: jal         0x80001090
    // 0x801CEB54: addiu       $a2, $zero, 0x24
    ctx->r6 = ADD32(0, 0X24);
    memory_copy(rdram, ctx);
        goto after_0;
    // 0x801CEB54: addiu       $a2, $zero, 0x24
    ctx->r6 = ADD32(0, 0X24);
    after_0:
L_801CEB58:
    // 0x801CEB58: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801CEB5C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801CEB60: jr          $ra
    // 0x801CEB64: nop

    return;
    // 0x801CEB64: nop

;}
RECOMP_FUNC void overlay_system_1_func_801CEB68(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801CEB68: addiu       $sp, $sp, -0x78
    ctx->r29 = ADD32(ctx->r29, -0X78);
    // 0x801CEB6C: sw          $s0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r16;
    // 0x801CEB70: lui         $s0, 0x800F
    ctx->r16 = S32(0X800F << 16);
    // 0x801CEB74: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x801CEB78: sw          $s3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r19;
    // 0x801CEB7C: sw          $s2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r18;
    // 0x801CEB80: sw          $s1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r17;
    // 0x801CEB84: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x801CEB88: sw          $a0, 0x78($sp)
    MEM_W(0X78, ctx->r29) = ctx->r4;
    // 0x801CEB8C: addiu       $s0, $s0, 0x3FB0
    ctx->r16 = ADD32(ctx->r16, 0X3FB0);
    // 0x801CEB90: lw          $s2, 0x24($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X24);
    // 0x801CEB94: jal         0x8002109C
    // 0x801CEB98: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8002109C(rdram, ctx);
        goto after_0;
    // 0x801CEB98: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
    // 0x801CEB9C: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x801CEBA0: srl         $t8, $t7, 29
    ctx->r24 = S32(U32(ctx->r15) >> 29);
    // 0x801CEBA4: beq         $t8, $zero, L_801CEC68
    if (ctx->r24 == 0) {
        // 0x801CEBA8: lui         $s0, 0x801D
        ctx->r16 = S32(0X801D << 16);
            goto L_801CEC68;
    }
    // 0x801CEBA8: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x801CEBAC: lw          $s0, -0x53E0($s0)
    ctx->r16 = MEM_W(ctx->r16, -0X53E0);
    // 0x801CEBB0: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x801CEBB4: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x801CEBB8: lwc1        $f6, 0x50($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X50);
    // 0x801CEBBC: lh          $t1, 0x7C7C($t1)
    ctx->r9 = MEM_H(ctx->r9, 0X7C7C);
    // 0x801CEBC0: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x801CEBC4: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CEBC8: addiu       $s1, $sp, 0x60
    ctx->r17 = ADD32(ctx->r29, 0X60);
    // 0x801CEBCC: addiu       $t0, $t0, 0x7CB4
    ctx->r8 = ADD32(ctx->r8, 0X7CB4);
    // 0x801CEBD0: addiu       $t9, $zero, -0x471C
    ctx->r25 = ADD32(0, -0X471C);
    // 0x801CEBD4: swc1        $f8, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f8.u32l;
    // 0x801CEBD8: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x801CEBDC: lwc1        $f16, 0x54($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X54);
    // 0x801CEBE0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CEBE4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x801CEBE8: sub.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x801CEBEC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x801CEBF0: addiu       $a3, $zero, 0x471C
    ctx->r7 = ADD32(0, 0X471C);
    // 0x801CEBF4: swc1        $f18, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f18.u32l;
    // 0x801CEBF8: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x801CEBFC: lwc1        $f6, 0x58($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X58);
    // 0x801CEC00: sw          $t0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r8;
    // 0x801CEC04: sw          $t9, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r25;
    // 0x801CEC08: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x801CEC0C: sw          $t1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r9;
    // 0x801CEC10: jal         0x800211A8
    // 0x801CEC14: swc1        $f8, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f8.u32l;
    func_800211A8(rdram, ctx);
        goto after_1;
    // 0x801CEC14: swc1        $f8, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f8.u32l;
    after_1:
    // 0x801CEC18: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x801CEC1C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x801CEC20: lw          $s0, 0x2960($v1)
    ctx->r16 = MEM_W(ctx->r3, 0X2960);
    // 0x801CEC24: lwc1        $f10, 0x60($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801CEC28: lwc1        $f4, 0x64($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801CEC2C: lwc1        $f16, 0x50($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X50);
    // 0x801CEC30: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x801CEC34: lw          $a2, 0x7C80($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X7C80);
    // 0x801CEC38: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x801CEC3C: lwc1        $f10, 0x68($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X68);
    // 0x801CEC40: lw          $a0, 0x295C($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X295C);
    // 0x801CEC44: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x801CEC48: swc1        $f18, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f18.u32l;
    // 0x801CEC4C: lwc1        $f6, 0x54($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X54);
    // 0x801CEC50: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x801CEC54: swc1        $f8, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f8.u32l;
    // 0x801CEC58: lwc1        $f16, 0x58($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X58);
    // 0x801CEC5C: add.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl);
    ctx->f18.fl = ctx->f10.fl + ctx->f16.fl;
    // 0x801CEC60: jal         0x80021598
    // 0x801CEC64: swc1        $f18, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f18.u32l;
    func_80021598(rdram, ctx);
        goto after_2;
    // 0x801CEC64: swc1        $f18, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f18.u32l;
    after_2:
L_801CEC68:
    // 0x801CEC68: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x801CEC6C: lw          $t2, -0x53E0($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X53E0);
    // 0x801CEC70: lw          $t3, 0x14($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X14);
    // 0x801CEC74: lw          $t4, 0x14($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X14);
    // 0x801CEC78: lw          $t5, 0x14($t4)
    ctx->r13 = MEM_W(ctx->r12, 0X14);
    // 0x801CEC7C: lw          $t6, 0x10($t5)
    ctx->r14 = MEM_W(ctx->r13, 0X10);
    // 0x801CEC80: lw          $t7, 0x14($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X14);
    // 0x801CEC84: lw          $s0, 0x14($t7)
    ctx->r16 = MEM_W(ctx->r15, 0X14);
    // 0x801CEC88: jal         0x8000C9A4
    // 0x801CEC8C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8000C9A4(rdram, ctx);
        goto after_3;
    // 0x801CEC8C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x801CEC90: lwc1        $f4, 0x68($s2)
    ctx->f4.u32l = MEM_W(ctx->r18, 0X68);
    // 0x801CEC94: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CEC98: ldc1        $f8, 0x7D50($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, 0X7D50);
    // 0x801CEC9C: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl);
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801CECA0: lwc1        $f16, 0x78($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X78);
    // 0x801CECA4: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d);
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x801CECA8: lui         $s3, 0x8000
    ctx->r19 = S32(0X8000 << 16);
    // 0x801CECAC: addiu       $s3, $s3, 0x47C0
    ctx->r19 = ADD32(ctx->r19, 0X47C0);
    // 0x801CECB0: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x801CECB4: cvt.s.d     $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d);
    ctx->f0.fl = CVT_S_D(ctx->f10.d);
    // 0x801CECB8: mul.s       $f18, $f16, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x801CECBC: swc1        $f18, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f18.u32l;
    // 0x801CECC0: lwc1        $f4, 0x7C($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X7C);
    // 0x801CECC4: lwc1        $f16, 0x60($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801CECC8: mul.s       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f0.fl);
    // 0x801CECCC: swc1        $f6, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f6.u32l;
    // 0x801CECD0: lwc1        $f8, 0x80($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X80);
    // 0x801CECD4: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x801CECD8: swc1        $f10, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f10.u32l;
    // 0x801CECDC: lwc1        $f18, 0xA8($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0XA8);
    // 0x801CECE0: add.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x801CECE4: swc1        $f4, 0x50($s2)
    MEM_W(0X50, ctx->r18) = ctx->f4.u32l;
    // 0x801CECE8: lwc1        $f8, 0xAC($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0XAC);
    // 0x801CECEC: lwc1        $f6, 0x64($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801CECF0: add.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f10.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x801CECF4: swc1        $f10, 0x54($s2)
    MEM_W(0X54, ctx->r18) = ctx->f10.u32l;
    // 0x801CECF8: lwc1        $f18, 0xB0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0XB0);
    // 0x801CECFC: lwc1        $f16, 0x68($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X68);
    // 0x801CED00: add.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x801CED04: jalr        $s3
    // 0x801CED08: swc1        $f4, 0x58($s2)
    MEM_W(0X58, ctx->r18) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_4;
    // 0x801CED08: swc1        $f4, 0x58($s2)
    MEM_W(0X58, ctx->r18) = ctx->f4.u32l;
    after_4:
    // 0x801CED0C: addiu       $t8, $v0, -0x2710
    ctx->r24 = ADD32(ctx->r2, -0X2710);
    // 0x801CED10: mtc1        $t8, $f6
    ctx->f6.u32l = ctx->r24;
    // 0x801CED14: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CED18: ldc1        $f20, 0x7D58($at)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r1, 0X7D58);
    // 0x801CED1C: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801CED20: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CED24: ldc1        $f18, 0x7D60($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0X7D60);
    // 0x801CED28: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x801CED2C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl);
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x801CED30: lwc1        $f8, 0x88($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X88);
    // 0x801CED34: mul.d       $f16, $f10, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f20.d);
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f20.d);
    // 0x801CED38: nop

    // 0x801CED3C: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d);
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x801CED40: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d);
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x801CED44: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x801CED48: jalr        $s3
    // 0x801CED4C: swc1        $f10, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_5;
    // 0x801CED4C: swc1        $f10, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f10.u32l;
    after_5:
    // 0x801CED50: addiu       $t9, $v0, -0x2710
    ctx->r25 = ADD32(ctx->r2, -0X2710);
    // 0x801CED54: mtc1        $t9, $f16
    ctx->f16.u32l = ctx->r25;
    // 0x801CED58: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CED5C: ldc1        $f8, 0x7D68($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, 0X7D68);
    // 0x801CED60: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801CED64: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x801CED68: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl);
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x801CED6C: lwc1        $f18, 0x8C($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X8C);
    // 0x801CED70: mul.d       $f6, $f4, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f20.d);
    ctx->f6.d = MUL_D(ctx->f4.d, ctx->f20.d);
    // 0x801CED74: nop

    // 0x801CED78: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d);
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x801CED7C: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d);
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x801CED80: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x801CED84: jalr        $s3
    // 0x801CED88: swc1        $f4, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_6;
    // 0x801CED88: swc1        $f4, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f4.u32l;
    after_6:
    // 0x801CED8C: addiu       $t0, $v0, -0x2710
    ctx->r8 = ADD32(ctx->r2, -0X2710);
    // 0x801CED90: mtc1        $t0, $f6
    ctx->f6.u32l = ctx->r8;
    // 0x801CED94: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CED98: ldc1        $f18, 0x7D70($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0X7D70);
    // 0x801CED9C: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x801CEDA0: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x801CEDA4: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl);
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x801CEDA8: lwc1        $f8, 0x90($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X90);
    // 0x801CEDAC: mul.d       $f16, $f10, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f20.d);
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f20.d);
    // 0x801CEDB0: nop

    // 0x801CEDB4: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d);
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x801CEDB8: lwc1        $f16, 0x60($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801CEDBC: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d);
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x801CEDC0: mul.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x801CEDC4: lwc1        $f6, 0x64($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801CEDC8: swc1        $f10, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f10.u32l;
    // 0x801CEDCC: lwc1        $f18, 0x98($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X98);
    // 0x801CEDD0: add.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f16.fl + ctx->f18.fl;
    // 0x801CEDD4: swc1        $f4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f4.u32l;
    // 0x801CEDD8: lwc1        $f8, 0x9C($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X9C);
    // 0x801CEDDC: mfc1        $a2, $f4
    ctx->r6 = (int32_t)ctx->f4.u32l;
    // 0x801CEDE0: add.s       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl);
    ctx->f16.fl = ctx->f6.fl + ctx->f8.fl;
    // 0x801CEDE4: swc1        $f16, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f16.u32l;
    // 0x801CEDE8: lwc1        $f18, 0xA0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0XA0);
    // 0x801CEDEC: mfc1        $a3, $f16
    ctx->r7 = (int32_t)ctx->f16.u32l;
    // 0x801CEDF0: add.s       $f6, $f10, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f6.fl = ctx->f10.fl + ctx->f18.fl;
    // 0x801CEDF4: swc1        $f6, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f6.u32l;
    // 0x801CEDF8: lh          $a1, 0x5E($s2)
    ctx->r5 = MEM_H(ctx->r18, 0X5E);
    // 0x801CEDFC: jal         0x8000612C
    // 0x801CEE00: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    func_8000612C(rdram, ctx);
        goto after_7;
    // 0x801CEE00: swc1        $f6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f6.u32l;
    after_7:
    // 0x801CEE04: lh          $t1, 0x5E($s2)
    ctx->r9 = MEM_H(ctx->r18, 0X5E);
    // 0x801CEE08: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801CEE0C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x801CEE10: addiu       $t2, $t1, 0x1000
    ctx->r10 = ADD32(ctx->r9, 0X1000);
    // 0x801CEE14: sh          $t2, 0x5E($s2)
    MEM_H(0X5E, ctx->r18) = ctx->r10;
    // 0x801CEE18: lhu         $t3, 0x52C($v0)
    ctx->r11 = MEM_HU(ctx->r2, 0X52C);
    // 0x801CEE1C: addiu       $t6, $zero, 0xF0
    ctx->r14 = ADD32(0, 0XF0);
    // 0x801CEE20: addiu       $t5, $zero, 0x80
    ctx->r13 = ADD32(0, 0X80);
    // 0x801CEE24: andi        $t4, $t3, 0x1
    ctx->r12 = ctx->r11 & 0X1;
    // 0x801CEE28: beql        $t4, $zero, L_801CEE3C
    if (ctx->r12 == 0) {
        // 0x801CEE2C: sb          $t6, 0x1F($s2)
        MEM_B(0X1F, ctx->r18) = ctx->r14;
            goto L_801CEE3C;
    }
    goto skip_0;
    // 0x801CEE2C: sb          $t6, 0x1F($s2)
    MEM_B(0X1F, ctx->r18) = ctx->r14;
    skip_0:
    // 0x801CEE30: b           L_801CEE3C
    // 0x801CEE34: sb          $t5, 0x1F($s2)
    MEM_B(0X1F, ctx->r18) = ctx->r13;
        goto L_801CEE3C;
    // 0x801CEE34: sb          $t5, 0x1F($s2)
    MEM_B(0X1F, ctx->r18) = ctx->r13;
    // 0x801CEE38: sb          $t6, 0x1F($s2)
    MEM_B(0X1F, ctx->r18) = ctx->r14;
L_801CEE3C:
    // 0x801CEE3C: lw          $t7, 0x2960($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X2960);
    // 0x801CEE40: lw          $t8, 0x14($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X14);
    // 0x801CEE44: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x801CEE48: lw          $t0, 0x14($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X14);
    // 0x801CEE4C: lw          $t1, 0x10($t0)
    ctx->r9 = MEM_W(ctx->r8, 0X10);
    // 0x801CEE50: lw          $t2, 0x10($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X10);
    // 0x801CEE54: lw          $t3, 0x14($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X14);
    // 0x801CEE58: lw          $s0, 0x14($t3)
    ctx->r16 = MEM_W(ctx->r11, 0X14);
    // 0x801CEE5C: jal         0x8000C9A4
    // 0x801CEE60: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8000C9A4(rdram, ctx);
        goto after_8;
    // 0x801CEE60: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_8:
    // 0x801CEE64: lw          $s1, 0x14($s2)
    ctx->r17 = MEM_W(ctx->r18, 0X14);
    // 0x801CEE68: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CEE6C: ldc1        $f18, 0x7D78($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0X7D78);
    // 0x801CEE70: lwc1        $f8, 0x68($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X68);
    // 0x801CEE74: lwc1        $f16, 0x78($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X78);
    // 0x801CEE78: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x801CEE7C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl);
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x801CEE80: mul.d       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f18.d);
    ctx->f4.d = MUL_D(ctx->f10.d, ctx->f18.d);
    // 0x801CEE84: cvt.s.d     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d);
    ctx->f0.fl = CVT_S_D(ctx->f4.d);
    // 0x801CEE88: mul.s       $f6, $f16, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f0.fl);
    // 0x801CEE8C: swc1        $f6, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f6.u32l;
    // 0x801CEE90: lwc1        $f8, 0x7C($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X7C);
    // 0x801CEE94: lwc1        $f16, 0x60($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801CEE98: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x801CEE9C: swc1        $f10, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f10.u32l;
    // 0x801CEEA0: lwc1        $f18, 0x80($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X80);
    // 0x801CEEA4: mul.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl);
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x801CEEA8: swc1        $f4, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f4.u32l;
    // 0x801CEEAC: lwc1        $f6, 0xA8($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0XA8);
    // 0x801CEEB0: add.s       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f16.fl + ctx->f6.fl;
    // 0x801CEEB4: swc1        $f8, 0x50($s1)
    MEM_W(0X50, ctx->r17) = ctx->f8.u32l;
    // 0x801CEEB8: lwc1        $f18, 0xAC($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0XAC);
    // 0x801CEEBC: lwc1        $f10, 0x64($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801CEEC0: add.s       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = ctx->f10.fl + ctx->f18.fl;
    // 0x801CEEC4: swc1        $f4, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->f4.u32l;
    // 0x801CEEC8: lwc1        $f6, 0xB0($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0XB0);
    // 0x801CEECC: lwc1        $f16, 0x68($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X68);
    // 0x801CEED0: add.s       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f16.fl + ctx->f6.fl;
    // 0x801CEED4: jalr        $s3
    // 0x801CEED8: swc1        $f8, 0x58($s1)
    MEM_W(0X58, ctx->r17) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_9;
    // 0x801CEED8: swc1        $f8, 0x58($s1)
    MEM_W(0X58, ctx->r17) = ctx->f8.u32l;
    after_9:
    // 0x801CEEDC: addiu       $t4, $v0, -0x2710
    ctx->r12 = ADD32(ctx->r2, -0X2710);
    // 0x801CEEE0: mtc1        $t4, $f10
    ctx->f10.u32l = ctx->r12;
    // 0x801CEEE4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CEEE8: ldc1        $f6, 0x7D80($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, 0X7D80);
    // 0x801CEEEC: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801CEEF0: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x801CEEF4: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl);
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x801CEEF8: lwc1        $f18, 0x88($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X88);
    // 0x801CEEFC: mul.d       $f16, $f4, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f20.d);
    ctx->f16.d = MUL_D(ctx->f4.d, ctx->f20.d);
    // 0x801CEF00: nop

    // 0x801CEF04: mul.d       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d);
    ctx->f8.d = MUL_D(ctx->f16.d, ctx->f6.d);
    // 0x801CEF08: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d);
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x801CEF0C: mul.s       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f18.fl);
    // 0x801CEF10: jalr        $s3
    // 0x801CEF14: swc1        $f4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_10;
    // 0x801CEF14: swc1        $f4, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f4.u32l;
    after_10:
    // 0x801CEF18: addiu       $t5, $v0, -0x2710
    ctx->r13 = ADD32(ctx->r2, -0X2710);
    // 0x801CEF1C: mtc1        $t5, $f16
    ctx->f16.u32l = ctx->r13;
    // 0x801CEF20: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CEF24: ldc1        $f18, 0x7D88($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, 0X7D88);
    // 0x801CEF28: cvt.s.w     $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    ctx->f6.fl = CVT_S_W(ctx->f16.u32l);
    // 0x801CEF2C: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x801CEF30: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl);
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x801CEF34: lwc1        $f6, 0x8C($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X8C);
    // 0x801CEF38: mul.d       $f10, $f8, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f20.d);
    ctx->f10.d = MUL_D(ctx->f8.d, ctx->f20.d);
    // 0x801CEF3C: nop

    // 0x801CEF40: mul.d       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f18.d);
    ctx->f4.d = MUL_D(ctx->f10.d, ctx->f18.d);
    // 0x801CEF44: cvt.s.d     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d);
    ctx->f16.fl = CVT_S_D(ctx->f4.d);
    // 0x801CEF48: mul.s       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f6.fl);
    // 0x801CEF4C: jalr        $s3
    // 0x801CEF50: swc1        $f8, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_11;
    // 0x801CEF50: swc1        $f8, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f8.u32l;
    after_11:
    // 0x801CEF54: addiu       $t6, $v0, -0x2710
    ctx->r14 = ADD32(ctx->r2, -0X2710);
    // 0x801CEF58: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    // 0x801CEF5C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801CEF60: ldc1        $f6, 0x7D90($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, 0X7D90);
    // 0x801CEF64: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x801CEF68: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801CEF6C: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl);
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x801CEF70: lwc1        $f18, 0x90($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X90);
    // 0x801CEF74: mul.d       $f16, $f4, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f20.d);
    ctx->f16.d = MUL_D(ctx->f4.d, ctx->f20.d);
    // 0x801CEF78: nop

    // 0x801CEF7C: mul.d       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d);
    ctx->f8.d = MUL_D(ctx->f16.d, ctx->f6.d);
    // 0x801CEF80: lwc1        $f16, 0x60($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X60);
    // 0x801CEF84: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d);
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x801CEF88: mul.s       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f18.fl);
    // 0x801CEF8C: lwc1        $f10, 0x64($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X64);
    // 0x801CEF90: swc1        $f4, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f4.u32l;
    // 0x801CEF94: lwc1        $f6, 0x98($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X98);
    // 0x801CEF98: add.s       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f8.fl = ctx->f16.fl + ctx->f6.fl;
    // 0x801CEF9C: swc1        $f8, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->f8.u32l;
    // 0x801CEFA0: lwc1        $f18, 0x9C($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X9C);
    // 0x801CEFA4: mfc1        $a2, $f8
    ctx->r6 = (int32_t)ctx->f8.u32l;
    // 0x801CEFA8: add.s       $f16, $f10, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f18.fl);
    ctx->f16.fl = ctx->f10.fl + ctx->f18.fl;
    // 0x801CEFAC: swc1        $f16, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->f16.u32l;
    // 0x801CEFB0: lwc1        $f6, 0xA0($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0XA0);
    // 0x801CEFB4: mfc1        $a3, $f16
    ctx->r7 = (int32_t)ctx->f16.u32l;
    // 0x801CEFB8: add.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl);
    ctx->f10.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x801CEFBC: swc1        $f10, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->f10.u32l;
    // 0x801CEFC0: lh          $a1, 0x5E($s1)
    ctx->r5 = MEM_H(ctx->r17, 0X5E);
    // 0x801CEFC4: jal         0x8000612C
    // 0x801CEFC8: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    func_8000612C(rdram, ctx);
        goto after_12;
    // 0x801CEFC8: swc1        $f10, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f10.u32l;
    after_12:
    // 0x801CEFCC: lh          $t7, 0x5E($s1)
    ctx->r15 = MEM_H(ctx->r17, 0X5E);
    // 0x801CEFD0: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x801CEFD4: addiu       $t2, $zero, 0xF0
    ctx->r10 = ADD32(0, 0XF0);
    // 0x801CEFD8: addiu       $t8, $t7, 0x1000
    ctx->r24 = ADD32(ctx->r15, 0X1000);
    // 0x801CEFDC: sh          $t8, 0x5E($s1)
    MEM_H(0X5E, ctx->r17) = ctx->r24;
    // 0x801CEFE0: lhu         $t9, -0x7814($t9)
    ctx->r25 = MEM_HU(ctx->r25, -0X7814);
    // 0x801CEFE4: addiu       $t1, $zero, 0x80
    ctx->r9 = ADD32(0, 0X80);
    // 0x801CEFE8: andi        $t0, $t9, 0x1
    ctx->r8 = ctx->r25 & 0X1;
    // 0x801CEFEC: beql        $t0, $zero, L_801CF000
    if (ctx->r8 == 0) {
        // 0x801CEFF0: sb          $t2, 0x1F($s1)
        MEM_B(0X1F, ctx->r17) = ctx->r10;
            goto L_801CF000;
    }
    goto skip_1;
    // 0x801CEFF0: sb          $t2, 0x1F($s1)
    MEM_B(0X1F, ctx->r17) = ctx->r10;
    skip_1:
    // 0x801CEFF4: b           L_801CF000
    // 0x801CEFF8: sb          $t1, 0x1F($s1)
    MEM_B(0X1F, ctx->r17) = ctx->r9;
        goto L_801CF000;
    // 0x801CEFF8: sb          $t1, 0x1F($s1)
    MEM_B(0X1F, ctx->r17) = ctx->r9;
    // 0x801CEFFC: sb          $t2, 0x1F($s1)
    MEM_B(0X1F, ctx->r17) = ctx->r10;
L_801CF000:
    // 0x801CF000: jal         0x801CF028
    // 0x801CF004: lw          $a0, 0x78($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X78);
    overlay_system_1_func_801CF028(rdram, ctx);
        goto after_13;
    // 0x801CF004: lw          $a0, 0x78($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X78);
    after_13:
    // 0x801CF008: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x801CF00C: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x801CF010: lw          $s0, 0x2C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X2C);
    // 0x801CF014: lw          $s1, 0x30($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X30);
    // 0x801CF018: lw          $s2, 0x34($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X34);
    // 0x801CF01C: lw          $s3, 0x38($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X38);
    // 0x801CF020: jr          $ra
    // 0x801CF024: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
    return;
    // 0x801CF024: addiu       $sp, $sp, 0x78
    ctx->r29 = ADD32(ctx->r29, 0X78);
;}
