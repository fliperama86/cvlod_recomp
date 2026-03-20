#include "recomp.h"
#include "funcs.h"

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
RECOMP_FUNC void map_ovl_00_func_802E3B70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E3B70: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E3B74: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E3B78: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x802E3B7C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E3B80: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E3B84: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E3B88: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x802E3B8C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x802E3B90: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x802E3B94: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x802E3B98: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x802E3B9C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x802E3BA0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E3BA4: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x802E3BA8: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x802E3BAC: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E3BB0: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x802E3BB4: lw          $t9, -0x46A8($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X46A8);
    // 0x802E3BB8: jalr        $t9
    // 0x802E3BBC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E3BBC: nop

    after_0:
    // 0x802E3BC0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E3BC4: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x802E3BC8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x802E3BCC: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x802E3BD0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E3BD4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E3BD8: jr          $ra
    // 0x802E3BDC: nop

    return;
    // 0x802E3BDC: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E3BE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E3BE0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x802E3BE4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E3BE8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E3BEC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E3BF0: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x802E3BF4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E3BF8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x802E3BFC: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    // 0x802E3C00: jalr        $t9
    // 0x802E3C04: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E3C04: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x802E3C08: bne         $v0, $zero, L_802E3C28
    if (ctx->r2 != 0) {
        // 0x802E3C0C: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_802E3C28;
    }
    // 0x802E3C0C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E3C10: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x802E3C14: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E3C18: jalr        $t9
    // 0x802E3C1C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E3C1C: nop

    after_1:
    // 0x802E3C20: b           L_802E3C3C
    // 0x802E3C24: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E3C3C;
    // 0x802E3C24: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E3C28:
    // 0x802E3C28: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E3C2C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E3C30: jalr        $t9
    // 0x802E3C34: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E3C34: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x802E3C38: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E3C3C:
    // 0x802E3C3C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E3C40: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x802E3C44: jr          $ra
    // 0x802E3C48: nop

    return;
    // 0x802E3C48: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E3C4C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E3C4C: addiu       $sp, $sp, -0xA8
    ctx->r29 = ADD32(ctx->r29, -0XA8);
    // 0x802E3C50: sw          $ra, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r31;
    // 0x802E3C54: sw          $fp, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r30;
    // 0x802E3C58: sw          $s7, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r23;
    // 0x802E3C5C: sw          $s6, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r22;
    // 0x802E3C60: sw          $s5, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r21;
    // 0x802E3C64: sw          $s4, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r20;
    // 0x802E3C68: sw          $s3, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->r19;
    // 0x802E3C6C: sw          $s2, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r18;
    // 0x802E3C70: sw          $s1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r17;
    // 0x802E3C74: sw          $s0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r16;
    // 0x802E3C78: sdc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X30, ctx->r29);
    // 0x802E3C7C: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x802E3C80: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x802E3C84: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x802E3C88: sw          $a0, 0xA8($sp)
    MEM_W(0XA8, ctx->r29) = ctx->r4;
    // 0x802E3C8C: lui         $t8, 0x802F
    ctx->r24 = S32(0X802F << 16);
    // 0x802E3C90: addiu       $t8, $t8, -0x4690
    ctx->r24 = ADD32(ctx->r24, -0X4690);
    // 0x802E3C94: lw          $t0, 0x0($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X0);
    // 0x802E3C98: lw          $t9, 0x4($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X4);
    // 0x802E3C9C: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x802E3CA0: addiu       $t7, $sp, 0x74
    ctx->r15 = ADD32(ctx->r29, 0X74);
    // 0x802E3CA4: sw          $t0, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->r8;
    // 0x802E3CA8: sw          $t9, 0x4($t7)
    MEM_W(0X4, ctx->r15) = ctx->r25;
    // 0x802E3CAC: lw          $t9, 0xC($t8)
    ctx->r25 = MEM_W(ctx->r24, 0XC);
    // 0x802E3CB0: lw          $t0, 0x8($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X8);
    // 0x802E3CB4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E3CB8: sw          $t9, 0xC($t7)
    MEM_W(0XC, ctx->r15) = ctx->r25;
    // 0x802E3CBC: sw          $t0, 0x8($t7)
    MEM_W(0X8, ctx->r15) = ctx->r8;
    // 0x802E3CC0: lw          $t0, 0x10($t8)
    ctx->r8 = MEM_W(ctx->r24, 0X10);
    // 0x802E3CC4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E3CC8: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E3CCC: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E3CD0: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x802E3CD4: jalr        $t9
    // 0x802E3CD8: sw          $t0, 0x10($t7)
    MEM_W(0X10, ctx->r15) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E3CD8: sw          $t0, 0x10($t7)
    MEM_W(0X10, ctx->r15) = ctx->r8;
    after_0:
    // 0x802E3CDC: beq         $v0, $zero, L_802E3D0C
    if (ctx->r2 == 0) {
        // 0x802E3CE0: addiu       $a0, $zero, 0x201
        ctx->r4 = ADD32(0, 0X201);
            goto L_802E3D0C;
    }
    // 0x802E3CE0: addiu       $a0, $zero, 0x201
    ctx->r4 = ADD32(0, 0X201);
    // 0x802E3CE4: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E3CE8: addiu       $t9, $t9, 0x76FC
    ctx->r25 = ADD32(ctx->r25, 0X76FC);
    // 0x802E3CEC: jalr        $t9
    // 0x802E3CF0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E3CF0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_1:
    // 0x802E3CF4: lw          $a0, 0xA8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XA8);
    // 0x802E3CF8: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x802E3CFC: jalr        $t9
    // 0x802E3D00: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E3D00: nop

    after_2:
    // 0x802E3D04: b           L_802E3ECC
    // 0x802E3D08: lw          $ra, 0x5C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X5C);
        goto L_802E3ECC;
    // 0x802E3D08: lw          $ra, 0x5C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X5C);
L_802E3D0C:
    // 0x802E3D0C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E3D10: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x802E3D14: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x802E3D18: jalr        $t9
    // 0x802E3D1C: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E3D1C: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    after_3:
    // 0x802E3D20: lw          $a0, 0xA8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XA8);
    // 0x802E3D24: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E3D28: addiu       $t9, $t9, 0x31CC
    ctx->r25 = ADD32(ctx->r25, 0X31CC);
    // 0x802E3D2C: or          $s4, $v0, $zero
    ctx->r20 = ctx->r2 | 0;
    // 0x802E3D30: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x802E3D34: jalr        $t9
    // 0x802E3D38: sw          $v0, 0x24($a0)
    MEM_W(0X24, ctx->r4) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E3D38: sw          $v0, 0x24($a0)
    MEM_W(0X24, ctx->r4) = ctx->r2;
    after_4:
    // 0x802E3D3C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x802E3D40: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x802E3D44: lw          $a1, 0x2B14($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X2B14);
    // 0x802E3D48: lhu         $t2, 0x2($s4)
    ctx->r10 = MEM_HU(ctx->r20, 0X2);
    // 0x802E3D4C: lui         $a0, 0x603
    ctx->r4 = S32(0X603 << 16);
    // 0x802E3D50: lui         $t4, 0x9999
    ctx->r12 = S32(0X9999 << 16);
    // 0x802E3D54: addiu       $a0, $a0, -0x3088
    ctx->r4 = ADD32(ctx->r4, -0X3088);
    // 0x802E3D58: ori         $t4, $t4, 0x99FF
    ctx->r12 = ctx->r12 | 0X99FF;
    // 0x802E3D5C: ori         $t3, $t2, 0x980
    ctx->r11 = ctx->r10 | 0X980;
    // 0x802E3D60: sw          $a0, 0x3C($s4)
    MEM_W(0X3C, ctx->r20) = ctx->r4;
    // 0x802E3D64: sh          $t3, 0x2($s4)
    MEM_H(0X2, ctx->r20) = ctx->r11;
    // 0x802E3D68: sw          $t4, 0x18($s4)
    MEM_W(0X18, ctx->r20) = ctx->r12;
    // 0x802E3D6C: sw          $a1, 0x40($s4)
    MEM_W(0X40, ctx->r20) = ctx->r5;
    // 0x802E3D70: lw          $t5, 0x7C($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X7C);
    // 0x802E3D74: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E3D78: addiu       $t9, $t9, 0x6504
    ctx->r25 = ADD32(ctx->r25, 0X6504);
    // 0x802E3D7C: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    // 0x802E3D80: jalr        $t9
    // 0x802E3D84: sw          $t5, 0x24($s4)
    MEM_W(0X24, ctx->r20) = ctx->r13;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E3D84: sw          $t5, 0x24($s4)
    MEM_W(0X24, ctx->r20) = ctx->r13;
    after_5:
    // 0x802E3D88: lw          $v1, 0xA8($sp)
    ctx->r3 = MEM_W(ctx->r29, 0XA8);
    // 0x802E3D8C: sw          $v0, 0x74($s4)
    MEM_W(0X74, ctx->r20) = ctx->r2;
    // 0x802E3D90: lui         $t7, 0x802E
    ctx->r15 = S32(0X802E << 16);
    // 0x802E3D94: sw          $v0, 0x68($v1)
    MEM_W(0X68, ctx->r3) = ctx->r2;
    // 0x802E3D98: lwc1        $f4, 0x50($s4)
    ctx->f4.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E3D9C: addiu       $v0, $s0, 0x8
    ctx->r2 = ADD32(ctx->r16, 0X8);
    // 0x802E3DA0: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E3DA4: swc1        $f4, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f4.u32l;
    // 0x802E3DA8: lwc1        $f6, 0x54($s4)
    ctx->f6.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E3DAC: addiu       $t7, $t7, 0x4610
    ctx->r15 = ADD32(ctx->r15, 0X4610);
    // 0x802E3DB0: addiu       $t9, $t9, 0x524
    ctx->r25 = ADD32(ctx->r25, 0X524);
    // 0x802E3DB4: swc1        $f6, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->f6.u32l;
    // 0x802E3DB8: lw          $t6, 0x10($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X10);
    // 0x802E3DBC: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x802E3DC0: sw          $t6, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r14;
    // 0x802E3DC4: jalr        $t9
    // 0x802E3DC8: sw          $t7, 0x10($v1)
    MEM_W(0X10, ctx->r3) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E3DC8: sw          $t7, 0x10($v1)
    MEM_W(0X10, ctx->r3) = ctx->r15;
    after_6:
    // 0x802E3DCC: beq         $v0, $zero, L_802E3EA4
    if (ctx->r2 == 0) {
        // 0x802E3DD0: or          $s5, $v0, $zero
        ctx->r21 = ctx->r2 | 0;
            goto L_802E3EA4;
    }
    // 0x802E3DD0: or          $s5, $v0, $zero
    ctx->r21 = ctx->r2 | 0;
    // 0x802E3DD4: sw          $v0, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r2;
    // 0x802E3DD8: lhu         $t8, 0xA($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0XA);
    // 0x802E3DDC: lui         $at, 0x43C8
    ctx->r1 = S32(0X43C8 << 16);
    // 0x802E3DE0: mtc1        $at, $f26
    ctx->f26.u32l = ctx->r1;
    // 0x802E3DE4: lui         $at, 0x4248
    ctx->r1 = S32(0X4248 << 16);
    // 0x802E3DE8: mtc1        $at, $f24
    ctx->f24.u32l = ctx->r1;
    // 0x802E3DEC: andi        $t1, $t8, 0xF9FF
    ctx->r9 = ctx->r24 & 0XF9FF;
    // 0x802E3DF0: ori         $t3, $t1, 0x200
    ctx->r11 = ctx->r9 | 0X200;
    // 0x802E3DF4: lui         $at, 0x41C8
    ctx->r1 = S32(0X41C8 << 16);
    // 0x802E3DF8: sh          $t1, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r9;
    // 0x802E3DFC: andi        $t5, $t3, 0xFFFC
    ctx->r13 = ctx->r11 & 0XFFFC;
    // 0x802E3E00: mtc1        $at, $f22
    ctx->f22.u32l = ctx->r1;
    // 0x802E3E04: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x802E3E08: sh          $t3, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r11;
    // 0x802E3E0C: lui         $at, 0x4348
    ctx->r1 = S32(0X4348 << 16);
    // 0x802E3E10: addiu       $fp, $zero, 0xBB8
    ctx->r30 = ADD32(0, 0XBB8);
    // 0x802E3E14: sh          $t5, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r13;
    // 0x802E3E18: ori         $t6, $t5, 0x3
    ctx->r14 = ctx->r13 | 0X3;
    // 0x802E3E1C: addiu       $t7, $zero, 0x2
    ctx->r15 = ADD32(0, 0X2);
    // 0x802E3E20: lui         $s6, 0x8004
    ctx->r22 = S32(0X8004 << 16);
    // 0x802E3E24: lui         $s7, 0x8004
    ctx->r23 = S32(0X8004 << 16);
    // 0x802E3E28: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x802E3E2C: sb          $a0, 0xD($v0)
    MEM_B(0XD, ctx->r2) = ctx->r4;
    // 0x802E3E30: sh          $t6, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r14;
    // 0x802E3E34: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x802E3E38: sh          $a0, 0x4($v0)
    MEM_H(0X4, ctx->r2) = ctx->r4;
    // 0x802E3E3C: sh          $fp, 0x8($v0)
    MEM_H(0X8, ctx->r2) = ctx->r30;
    // 0x802E3E40: addiu       $s7, $s7, 0x844
    ctx->r23 = ADD32(ctx->r23, 0X844);
    // 0x802E3E44: addiu       $s6, $s6, 0x628
    ctx->r22 = ADD32(ctx->r22, 0X628);
    // 0x802E3E48: addiu       $s1, $sp, 0x74
    ctx->r17 = ADD32(ctx->r29, 0X74);
    // 0x802E3E4C: addiu       $s3, $sp, 0x88
    ctx->r19 = ADD32(ctx->r29, 0X88);
    // 0x802E3E50: addiu       $s2, $zero, 0x4000
    ctx->r18 = ADD32(0, 0X4000);
L_802E3E54:
    // 0x802E3E54: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x802E3E58: jalr        $s6
    // 0x802E3E5C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r22)(rdram, ctx);
        goto after_7;
    // 0x802E3E5C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_7:
    // 0x802E3E60: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x802E3E64: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x802E3E68: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x802E3E6C: jalr        $s7
    // 0x802E3E70: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_8;
    // 0x802E3E70: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
    after_8:
    // 0x802E3E74: sh          $zero, 0x12($s0)
    MEM_H(0X12, ctx->r16) = 0;
    // 0x802E3E78: sh          $zero, 0x14($s0)
    MEM_H(0X14, ctx->r16) = 0;
    // 0x802E3E7C: sh          $s2, 0x16($s0)
    MEM_H(0X16, ctx->r16) = ctx->r18;
    // 0x802E3E80: swc1        $f20, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f20.u32l;
    // 0x802E3E84: lwc1        $f8, 0x0($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X0);
    // 0x802E3E88: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x802E3E8C: swc1        $f22, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f22.u32l;
    // 0x802E3E90: swc1        $f24, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f24.u32l;
    // 0x802E3E94: swc1        $f26, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f26.u32l;
    // 0x802E3E98: sh          $fp, 0x8($s0)
    MEM_H(0X8, ctx->r16) = ctx->r30;
    // 0x802E3E9C: bne         $s1, $s3, L_802E3E54
    if (ctx->r17 != ctx->r19) {
        // 0x802E3EA0: swc1        $f8, 0x1C($s0)
        MEM_W(0X1C, ctx->r16) = ctx->f8.u32l;
            goto L_802E3E54;
    }
    // 0x802E3EA0: swc1        $f8, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f8.u32l;
L_802E3EA4:
    // 0x802E3EA4: jal         0x80143234
    // 0x802E3EA8: lw          $a0, 0xA8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XA8);
    func_80143234(rdram, ctx);
        goto after_9;
    // 0x802E3EA8: lw          $a0, 0xA8($sp)
    ctx->r4 = MEM_W(ctx->r29, 0XA8);
    after_9:
    // 0x802E3EAC: lw          $t9, 0xA8($sp)
    ctx->r25 = MEM_W(ctx->r29, 0XA8);
    // 0x802E3EB0: addiu       $a0, $t9, 0x8
    ctx->r4 = ADD32(ctx->r25, 0X8);
    // 0x802E3EB4: addiu       $a1, $t9, 0xE
    ctx->r5 = ADD32(ctx->r25, 0XE);
    // 0x802E3EB8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E3EBC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E3EC0: jalr        $t9
    // 0x802E3EC4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x802E3EC4: nop

    after_10:
    // 0x802E3EC8: lw          $ra, 0x5C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X5C);
L_802E3ECC:
    // 0x802E3ECC: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x802E3ED0: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x802E3ED4: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x802E3ED8: ldc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X30);
    // 0x802E3EDC: lw          $s0, 0x38($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X38);
    // 0x802E3EE0: lw          $s1, 0x3C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X3C);
    // 0x802E3EE4: lw          $s2, 0x40($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X40);
    // 0x802E3EE8: lw          $s3, 0x44($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X44);
    // 0x802E3EEC: lw          $s4, 0x48($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X48);
    // 0x802E3EF0: lw          $s5, 0x4C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X4C);
    // 0x802E3EF4: lw          $s6, 0x50($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X50);
    // 0x802E3EF8: lw          $s7, 0x54($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X54);
    // 0x802E3EFC: lw          $fp, 0x58($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X58);
    // 0x802E3F00: jr          $ra
    // 0x802E3F04: addiu       $sp, $sp, 0xA8
    ctx->r29 = ADD32(ctx->r29, 0XA8);
    return;
    // 0x802E3F04: addiu       $sp, $sp, 0xA8
    ctx->r29 = ADD32(ctx->r29, 0XA8);
;}
RECOMP_FUNC void map_ovl_00_func_802E3F08(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E3F08: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x802E3F0C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E3F10: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E3F14: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x802E3F18: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x802E3F1C: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E3F20: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x802E3F24: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E3F28: sw          $v1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r3;
    // 0x802E3F2C: addiu       $a1, $v1, 0x50
    ctx->r5 = ADD32(ctx->r3, 0X50);
    // 0x802E3F30: jalr        $t9
    // 0x802E3F34: sw          $t6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E3F34: sw          $t6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r14;
    after_0:
    // 0x802E3F38: beq         $v0, $zero, L_802E3F58
    if (ctx->r2 == 0) {
        // 0x802E3F3C: lw          $v1, 0x34($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X34);
            goto L_802E3F58;
    }
    // 0x802E3F3C: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x802E3F40: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x802E3F44: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E3F48: jalr        $t9
    // 0x802E3F4C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E3F4C: nop

    after_1:
    // 0x802E3F50: b           L_802E405C
    // 0x802E3F54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E405C;
    // 0x802E3F54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E3F58:
    // 0x802E3F58: lw          $t7, 0x30($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X30);
    // 0x802E3F5C: lui         $t8, 0x801D
    ctx->r24 = S32(0X801D << 16);
    // 0x802E3F60: lw          $t8, -0x7CC4($t8)
    ctx->r24 = MEM_W(ctx->r24, -0X7CC4);
    // 0x802E3F64: lw          $a0, 0x4($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X4);
    // 0x802E3F68: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E3F6C: sw          $t8, 0x24($v1)
    MEM_W(0X24, ctx->r3) = ctx->r24;
    // 0x802E3F70: lw          $t0, 0x4C($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X4C);
    // 0x802E3F74: addiu       $t9, $t9, 0x4780
    ctx->r25 = ADD32(ctx->r25, 0X4780);
    // 0x802E3F78: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x802E3F7C: beql        $t0, $zero, L_802E4008
    if (ctx->r8 == 0) {
        // 0x802E3F80: lh          $v1, 0xE($s0)
        ctx->r3 = MEM_H(ctx->r16, 0XE);
            goto L_802E4008;
    }
    goto skip_0;
    // 0x802E3F80: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    skip_0:
    // 0x802E3F84: jalr        $t9
    // 0x802E3F88: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E3F88: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    after_2:
    // 0x802E3F8C: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    // 0x802E3F90: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E3F94: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x802E3F98: lh          $t1, 0x8($a0)
    ctx->r9 = MEM_H(ctx->r4, 0X8);
    // 0x802E3F9C: lh          $t2, 0x4A($a0)
    ctx->r10 = MEM_H(ctx->r4, 0X4A);
    // 0x802E3FA0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x802E3FA4: subu        $t3, $t1, $t2
    ctx->r11 = SUB32(ctx->r9, ctx->r10);
    // 0x802E3FA8: sh          $t3, 0x8($a0)
    MEM_H(0X8, ctx->r4) = ctx->r11;
    // 0x802E3FAC: lh          $t4, 0x8($a0)
    ctx->r12 = MEM_H(ctx->r4, 0X8);
    // 0x802E3FB0: bgtz        $t4, L_802E3FEC
    if (SIGNED(ctx->r12) > 0) {
        // 0x802E3FB4: nop
    
            goto L_802E3FEC;
    }
    // 0x802E3FB4: nop

    // 0x802E3FB8: jalr        $t9
    // 0x802E3FBC: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E3FBC: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    after_3:
    // 0x802E3FC0: lw          $t6, 0x30($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X30);
    // 0x802E3FC4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E3FC8: ori         $t5, $zero, 0xFFFF
    ctx->r13 = 0 | 0XFFFF;
    // 0x802E3FCC: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x802E3FD0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E3FD4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x802E3FD8: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    // 0x802E3FDC: jalr        $t9
    // 0x802E3FE0: sw          $t5, 0x8($t6)
    MEM_W(0X8, ctx->r14) = ctx->r13;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E3FE0: sw          $t5, 0x8($t6)
    MEM_W(0X8, ctx->r14) = ctx->r13;
    after_4:
    // 0x802E3FE4: b           L_802E405C
    // 0x802E3FE8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E405C;
    // 0x802E3FE8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E3FEC:
    // 0x802E3FEC: lwc1        $f4, -0x7D38($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x802E3FF0: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
    // 0x802E3FF4: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x802E3FF8: mfc1        $t8, $f6
    ctx->r24 = (int32_t)ctx->f6.u32l;
    // 0x802E3FFC: nop

    // 0x802E4000: sw          $t8, 0xC($t0)
    MEM_W(0XC, ctx->r8) = ctx->r24;
    // 0x802E4004: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
L_802E4008:
    // 0x802E4008: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E400C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4010: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E4014: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E4018: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E401C: sll         $t1, $v1, 1
    ctx->r9 = S32(ctx->r3 << 1);
    // 0x802E4020: addu        $v0, $s0, $t1
    ctx->r2 = ADD32(ctx->r16, ctx->r9);
    // 0x802E4024: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x802E4028: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x802E402C: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x802E4030: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x802E4034: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x802E4038: sb          $t3, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r11;
    // 0x802E403C: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x802E4040: lw          $t9, -0x467C($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X467C);
    // 0x802E4044: jalr        $t9
    // 0x802E4048: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E4048: nop

    after_5:
    // 0x802E404C: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x802E4050: addiu       $t7, $t6, -0x1
    ctx->r15 = ADD32(ctx->r14, -0X1);
    // 0x802E4054: sh          $t7, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r15;
    // 0x802E4058: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E405C:
    // 0x802E405C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E4060: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x802E4064: jr          $ra
    // 0x802E4068: nop

    return;
    // 0x802E4068: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E406C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E406C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E4070: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E4074: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x802E4078: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x802E407C: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    // 0x802E4080: lw          $t6, 0x4C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4C);
    // 0x802E4084: beql        $t6, $zero, L_802E40CC
    if (ctx->r14 == 0) {
        // 0x802E4088: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_802E40CC;
    }
    goto skip_0;
    // 0x802E4088: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x802E408C: lh          $t7, 0x8($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X8);
    // 0x802E4090: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x802E4094: blezl       $t7, L_802E40CC
    if (SIGNED(ctx->r15) <= 0) {
        // 0x802E4098: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_802E40CC;
    }
    goto skip_1;
    // 0x802E4098: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x802E409C: lwc1        $f4, -0x7D38($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x802E40A0: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x802E40A4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x802E40A8: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x802E40AC: mfc1        $t9, $f6
    ctx->r25 = (int32_t)ctx->f6.u32l;
    // 0x802E40B0: nop

    // 0x802E40B4: sw          $t9, 0xC($v1)
    MEM_W(0XC, ctx->r3) = ctx->r25;
    // 0x802E40B8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E40BC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E40C0: jalr        $t9
    // 0x802E40C4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E40C4: nop

    after_0:
    // 0x802E40C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E40CC:
    // 0x802E40CC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E40D0: jr          $ra
    // 0x802E40D4: nop

    return;
    // 0x802E40D4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E40D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E40D8: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x802E40DC: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x802E40E0: addiu       $a2, $a2, -0x7D40
    ctx->r6 = ADD32(ctx->r6, -0X7D40);
    // 0x802E40E4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E40E8: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x802E40EC: lw          $a3, 0x24($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X24);
    // 0x802E40F0: lw          $t7, 0x7C($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X7C);
    // 0x802E40F4: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x802E40F8: sw          $t7, 0x24($a3)
    MEM_W(0X24, ctx->r7) = ctx->r15;
    // 0x802E40FC: lw          $t8, 0xC($v0)
    ctx->r24 = MEM_W(ctx->r2, 0XC);
    // 0x802E4100: addiu       $v1, $v0, 0x8
    ctx->r3 = ADD32(ctx->r2, 0X8);
    // 0x802E4104: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x802E4108: bgez        $t9, L_802E4148
    if (SIGNED(ctx->r25) >= 0) {
        // 0x802E410C: sw          $t9, 0xC($v0)
        MEM_W(0XC, ctx->r2) = ctx->r25;
            goto L_802E4148;
    }
    // 0x802E410C: sw          $t9, 0xC($v0)
    MEM_W(0XC, ctx->r2) = ctx->r25;
    // 0x802E4110: addiu       $v1, $v0, 0x8
    ctx->r3 = ADD32(ctx->r2, 0X8);
    // 0x802E4114: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x802E4118: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E411C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x802E4120: swc1        $f4, 0x50($a3)
    MEM_W(0X50, ctx->r7) = ctx->f4.u32l;
    // 0x802E4124: lwc1        $f6, 0xC($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0XC);
    // 0x802E4128: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x802E412C: swc1        $f6, 0x54($a3)
    MEM_W(0X54, ctx->r7) = ctx->f6.u32l;
    // 0x802E4130: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x802E4134: addiu       $a0, $t1, 0x8
    ctx->r4 = ADD32(ctx->r9, 0X8);
    // 0x802E4138: jalr        $t9
    // 0x802E413C: addiu       $a1, $t1, 0xE
    ctx->r5 = ADD32(ctx->r9, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E413C: addiu       $a1, $t1, 0xE
    ctx->r5 = ADD32(ctx->r9, 0XE);
    after_0:
    // 0x802E4140: b           L_802E422C
    // 0x802E4144: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_802E422C;
    // 0x802E4144: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E4148:
    // 0x802E4148: lw          $t2, 0x4($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X4);
    // 0x802E414C: lwc1        $f16, 0x8($a2)
    ctx->f16.u32l = MEM_W(ctx->r6, 0X8);
    // 0x802E4150: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E4154: mtc1        $t2, $f8
    ctx->f8.u32l = ctx->r10;
    // 0x802E4158: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x802E415C: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x802E4160: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E4164: lui         $a1, 0x8000
    ctx->r5 = S32(0X8000 << 16);
    // 0x802E4168: addiu       $a1, $a1, 0x47C0
    ctx->r5 = ADD32(ctx->r5, 0X47C0);
    // 0x802E416C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x802E4170: sw          $a3, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r7;
    // 0x802E4174: sw          $v1, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r3;
    // 0x802E4178: div.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = DIV_S(ctx->f10.fl, ctx->f16.fl);
    // 0x802E417C: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x802E4180: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x802E4184: mul.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = MUL_D(ctx->f4.d, ctx->f6.d);
    // 0x802E4188: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x802E418C: jalr        $a1
    // 0x802E4190: swc1        $f10, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r5)(rdram, ctx);
        goto after_1;
    // 0x802E4190: swc1        $f10, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->f10.u32l;
    after_1:
    // 0x802E4194: addiu       $t3, $v0, -0x2710
    ctx->r11 = ADD32(ctx->r2, -0X2710);
    // 0x802E4198: mtc1        $t3, $f18
    ctx->f18.u32l = ctx->r11;
    // 0x802E419C: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E41A0: ldc1        $f8, -0x3E38($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3E38);
    // 0x802E41A4: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E41A8: lwc1        $f16, 0x2C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x802E41AC: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x802E41B0: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
    // 0x802E41B4: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x802E41B8: cvt.d.s     $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f0.d = CVT_D_S(ctx->f16.fl);
    // 0x802E41BC: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x802E41C0: lwc1        $f18, 0x8($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X8);
    // 0x802E41C4: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x802E41C8: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x802E41CC: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x802E41D0: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x802E41D4: add.d       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = ctx->f16.d + ctx->f4.d;
    // 0x802E41D8: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x802E41DC: swc1        $f8, 0x50($a3)
    MEM_W(0X50, ctx->r7) = ctx->f8.u32l;
    // 0x802E41E0: jalr        $a1
    // 0x802E41E4: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    LOOKUP_FUNC(ctx->r5)(rdram, ctx);
        goto after_2;
    // 0x802E41E4: sdc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    SD(ctx->f0.u64, 0X18, ctx->r29);
    after_2:
    // 0x802E41E8: addiu       $t4, $v0, -0x2710
    ctx->r12 = ADD32(ctx->r2, -0X2710);
    // 0x802E41EC: mtc1        $t4, $f10
    ctx->f10.u32l = ctx->r12;
    // 0x802E41F0: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E41F4: ldc1        $f4, -0x3E30($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X3E30);
    // 0x802E41F8: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E41FC: ldc1        $f0, 0x18($sp)
    CHECK_FR(ctx, 0);
    ctx->f0.u64 = LD(ctx->r29, 0X18);
    // 0x802E4200: lw          $v1, 0x28($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X28);
    // 0x802E4204: lw          $a3, 0x30($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X30);
    // 0x802E4208: lwc1        $f10, 0xC($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0XC);
    // 0x802E420C: cvt.d.s     $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f16.d = CVT_D_S(ctx->f18.fl);
    // 0x802E4210: mul.d       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f16.d, ctx->f4.d);
    // 0x802E4214: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x802E4218: mul.d       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x802E421C: add.d       $f16, $f8, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f18.d); 
    ctx->f16.d = ctx->f8.d + ctx->f18.d;
    // 0x802E4220: cvt.s.d     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f4.fl = CVT_S_D(ctx->f16.d);
    // 0x802E4224: swc1        $f4, 0x54($a3)
    MEM_W(0X54, ctx->r7) = ctx->f4.u32l;
    // 0x802E4228: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E422C:
    // 0x802E422C: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x802E4230: jr          $ra
    // 0x802E4234: nop

    return;
    // 0x802E4234: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E4238(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E4238: addiu       $sp, $sp, -0x70
    ctx->r29 = ADD32(ctx->r29, -0X70);
    // 0x802E423C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x802E4240: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x802E4244: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x802E4248: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x802E424C: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x802E4250: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x802E4254: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x802E4258: lw          $s0, 0x24($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X24);
    // 0x802E425C: lw          $t6, 0x7C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X7C);
    // 0x802E4260: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x802E4264: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x802E4268: sw          $t6, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r14;
    // 0x802E426C: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x802E4270: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x802E4274: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4278: lwc1        $f10, -0x3E28($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3E28);
    // 0x802E427C: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x802E4280: lw          $t7, 0x8($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X8);
    // 0x802E4284: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x802E4288: lui         $at, 0xC024
    ctx->r1 = S32(0XC024 << 16);
    // 0x802E428C: mtc1        $t7, $f18
    ctx->f18.u32l = ctx->r15;
    // 0x802E4290: div.s       $f16, $f10, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f8.fl);
    // 0x802E4294: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E4298: sub.s       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f16.fl;
    // 0x802E429C: trunc.w.s   $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x802E42A0: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    // 0x802E42A4: nop

    // 0x802E42A8: bgez        $t9, L_802E43B0
    if (SIGNED(ctx->r25) >= 0) {
        // 0x802E42AC: sw          $t9, 0x8($v1)
        MEM_W(0X8, ctx->r3) = ctx->r25;
            goto L_802E43B0;
    }
    // 0x802E42AC: sw          $t9, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->r25;
    // 0x802E42B0: lh          $t1, 0x0($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X0);
    // 0x802E42B4: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x802E42B8: lui         $s1, 0x801D
    ctx->r17 = S32(0X801D << 16);
    // 0x802E42BC: or          $t2, $t1, $at
    ctx->r10 = ctx->r9 | ctx->r1;
    // 0x802E42C0: sh          $t2, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r10;
    // 0x802E42C4: lw          $t3, 0x2908($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X2908);
    // 0x802E42C8: addiu       $s1, $s1, -0x55A0
    ctx->r17 = ADD32(ctx->r17, -0X55A0);
    // 0x802E42CC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E42D0: andi        $t4, $t3, 0x40
    ctx->r12 = ctx->r11 & 0X40;
    // 0x802E42D4: bne         $t4, $zero, L_802E45B8
    if (ctx->r12 != 0) {
        // 0x802E42D8: addiu       $a1, $zero, 0x2
        ctx->r5 = ADD32(0, 0X2);
            goto L_802E45B8;
    }
    // 0x802E42D8: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x802E42DC: lui         $s0, 0x8000
    ctx->r16 = S32(0X8000 << 16);
    // 0x802E42E0: addiu       $s0, $s0, 0x48C4
    ctx->r16 = ADD32(ctx->r16, 0X48C4);
    // 0x802E42E4: sw          $v1, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r3;
    // 0x802E42E8: jalr        $s0
    // 0x802E42EC: sw          $a2, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_0;
    // 0x802E42EC: sw          $a2, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r6;
    after_0:
    // 0x802E42F0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E42F4: jalr        $s0
    // 0x802E42F8: addiu       $a1, $zero, 0x2B4
    ctx->r5 = ADD32(0, 0X2B4);
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_1;
    // 0x802E42F8: addiu       $a1, $zero, 0x2B4
    ctx->r5 = ADD32(0, 0X2B4);
    after_1:
    // 0x802E42FC: lui         $s3, 0x8014
    ctx->r19 = S32(0X8014 << 16);
    // 0x802E4300: lui         $s2, 0x8015
    ctx->r18 = S32(0X8015 << 16);
    // 0x802E4304: addiu       $s2, $s2, 0x23AC
    ctx->r18 = ADD32(ctx->r18, 0X23AC);
    // 0x802E4308: addiu       $s3, $s3, 0x78F4
    ctx->r19 = ADD32(ctx->r19, 0X78F4);
    // 0x802E430C: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x802E4310: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x802E4314: addiu       $s1, $sp, 0x54
    ctx->r17 = ADD32(ctx->r29, 0X54);
L_802E4318:
    // 0x802E4318: jalr        $s3
    // 0x802E431C: addiu       $a0, $zero, 0x1B9
    ctx->r4 = ADD32(0, 0X1B9);
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_2;
    // 0x802E431C: addiu       $a0, $zero, 0x1B9
    ctx->r4 = ADD32(0, 0X1B9);
    after_2:
    // 0x802E4320: sll         $t5, $s0, 2
    ctx->r13 = S32(ctx->r16 << 2);
    // 0x802E4324: addu        $v1, $s1, $t5
    ctx->r3 = ADD32(ctx->r17, ctx->r13);
    // 0x802E4328: beq         $v0, $zero, L_802E4338
    if (ctx->r2 == 0) {
        // 0x802E432C: sw          $v0, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r2;
            goto L_802E4338;
    }
    // 0x802E432C: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x802E4330: jalr        $s2
    // 0x802E4334: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_3;
    // 0x802E4334: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    after_3:
L_802E4338:
    // 0x802E4338: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x802E433C: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x802E4340: slti        $at, $s0, 0x2
    ctx->r1 = SIGNED(ctx->r16) < 0X2 ? 1 : 0;
    // 0x802E4344: bne         $at, $zero, L_802E4318
    if (ctx->r1 != 0) {
        // 0x802E4348: or          $a1, $s0, $zero
        ctx->r5 = ctx->r16 | 0;
            goto L_802E4318;
    }
    // 0x802E4348: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x802E434C: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E4350: addiu       $t9, $t9, 0x76FC
    ctx->r25 = ADD32(ctx->r25, 0X76FC);
    // 0x802E4354: jalr        $t9
    // 0x802E4358: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E4358: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_4:
    // 0x802E435C: lw          $t6, 0x68($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X68);
    // 0x802E4360: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E4364: addiu       $t9, $t9, 0x938
    ctx->r25 = ADD32(ctx->r25, 0X938);
    // 0x802E4368: jalr        $t9
    // 0x802E436C: lw          $a0, 0x4($t6)
    ctx->r4 = MEM_W(ctx->r14, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E436C: lw          $a0, 0x4($t6)
    ctx->r4 = MEM_W(ctx->r14, 0X4);
    after_5:
    // 0x802E4370: lw          $t7, 0x54($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X54);
    // 0x802E4374: lw          $v0, 0x70($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X70);
    // 0x802E4378: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E437C: bne         $t7, $zero, L_802E439C
    if (ctx->r15 != 0) {
        // 0x802E4380: addiu       $a0, $v0, 0x8
        ctx->r4 = ADD32(ctx->r2, 0X8);
            goto L_802E439C;
    }
    // 0x802E4380: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x802E4384: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    // 0x802E4388: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x802E438C: jalr        $t9
    // 0x802E4390: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E4390: nop

    after_6:
    // 0x802E4394: b           L_802E45BC
    // 0x802E4398: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_802E45BC;
    // 0x802E4398: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_802E439C:
    // 0x802E439C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E43A0: jalr        $t9
    // 0x802E43A4: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x802E43A4: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_7:
    // 0x802E43A8: b           L_802E45BC
    // 0x802E43AC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_802E45BC;
    // 0x802E43AC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_802E43B0:
    // 0x802E43B0: lwc1        $f8, 0x50($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X50);
    // 0x802E43B4: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E43B8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E43BC: cvt.d.s     $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.d = CVT_D_S(ctx->f8.fl);
    // 0x802E43C0: lui         $s2, 0x8000
    ctx->r18 = S32(0X8000 << 16);
    // 0x802E43C4: add.d       $f16, $f18, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f16.d = ctx->f18.d + ctx->f4.d;
    // 0x802E43C8: addiu       $s2, $s2, 0x47C0
    ctx->r18 = ADD32(ctx->r18, 0X47C0);
    // 0x802E43CC: sw          $v1, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r3;
    // 0x802E43D0: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x802E43D4: cvt.s.d     $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f6.fl = CVT_S_D(ctx->f16.d);
    // 0x802E43D8: jalr        $s2
    // 0x802E43DC: swc1        $f6, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_8;
    // 0x802E43DC: swc1        $f6, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f6.u32l;
    after_8:
    // 0x802E43E0: addiu       $t8, $v0, -0x2710
    ctx->r24 = ADD32(ctx->r2, -0X2710);
    // 0x802E43E4: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x802E43E8: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E43EC: ldc1        $f4, -0x3E20($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X3E20);
    // 0x802E43F0: cvt.s.w     $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E43F4: lui         $at, 0x4034
    ctx->r1 = S32(0X4034 << 16);
    // 0x802E43F8: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x802E43FC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x802E4400: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x802E4404: cvt.d.s     $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.d = CVT_D_S(ctx->f8.fl);
    // 0x802E4408: lwc1        $f8, 0x54($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X54);
    // 0x802E440C: mul.d       $f16, $f18, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f16.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x802E4410: cvt.d.s     $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.d = CVT_D_S(ctx->f8.fl);
    // 0x802E4414: mul.d       $f10, $f16, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d); 
    ctx->f10.d = MUL_D(ctx->f16.d, ctx->f6.d);
    // 0x802E4418: add.d       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f10.d + ctx->f18.d;
    // 0x802E441C: cvt.s.d     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f16.fl = CVT_S_D(ctx->f4.d);
    // 0x802E4420: jalr        $s2
    // 0x802E4424: swc1        $f16, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_9;
    // 0x802E4424: swc1        $f16, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f16.u32l;
    after_9:
    // 0x802E4428: addiu       $t0, $v0, -0x2710
    ctx->r8 = ADD32(ctx->r2, -0X2710);
    // 0x802E442C: mtc1        $t0, $f6
    ctx->f6.u32l = ctx->r8;
    // 0x802E4430: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4434: ldc1        $f18, -0x3E18($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, -0X3E18);
    // 0x802E4438: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x802E443C: lui         $at, 0x4034
    ctx->r1 = S32(0X4034 << 16);
    // 0x802E4440: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x802E4444: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x802E4448: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x802E444C: addiu       $t9, $t9, -0x6B48
    ctx->r25 = ADD32(ctx->r25, -0X6B48);
    // 0x802E4450: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x802E4454: lwc1        $f8, 0x58($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X58);
    // 0x802E4458: mul.d       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f10.d, ctx->f18.d);
    // 0x802E445C: addiu       $a0, $zero, 0x1A
    ctx->r4 = ADD32(0, 0X1A);
    // 0x802E4460: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x802E4464: addiu       $a1, $zero, 0x1A
    ctx->r5 = ADD32(0, 0X1A);
    // 0x802E4468: addiu       $a2, $sp, 0x48
    ctx->r6 = ADD32(ctx->r29, 0X48);
    // 0x802E446C: lui         $a3, 0x2000
    ctx->r7 = S32(0X2000 << 16);
    // 0x802E4470: mul.d       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f16.d); 
    ctx->f6.d = MUL_D(ctx->f4.d, ctx->f16.d);
    // 0x802E4474: add.d       $f18, $f6, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = ctx->f6.d + ctx->f10.d;
    // 0x802E4478: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x802E447C: jalr        $t9
    // 0x802E4480: swc1        $f4, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f4.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x802E4480: swc1        $f4, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f4.u32l;
    after_10:
    // 0x802E4484: beq         $v0, $zero, L_802E4510
    if (ctx->r2 == 0) {
        // 0x802E4488: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_802E4510;
    }
    // 0x802E4488: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x802E448C: jalr        $s2
    // 0x802E4490: addiu       $a0, $zero, 0x7FFF
    ctx->r4 = ADD32(0, 0X7FFF);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_11;
    // 0x802E4490: addiu       $a0, $zero, 0x7FFF
    ctx->r4 = ADD32(0, 0X7FFF);
    after_11:
    // 0x802E4494: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    // 0x802E4498: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x802E449C: bgez        $v0, L_802E44B8
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E44A0: cvt.d.w     $f8, $f16
        CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    ctx->f8.d = CVT_D_W(ctx->f16.u32l);
            goto L_802E44B8;
    }
    // 0x802E44A0: cvt.d.w     $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    ctx->f8.d = CVT_D_W(ctx->f16.u32l);
    // 0x802E44A4: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x802E44A8: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x802E44AC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x802E44B0: nop

    // 0x802E44B4: add.d       $f8, $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f8.d + ctx->f6.d;
L_802E44B8:
    // 0x802E44B8: lui         $at, 0x40F0
    ctx->r1 = S32(0X40F0 << 16);
    // 0x802E44BC: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x802E44C0: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E44C4: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x802E44C8: div.d       $f18, $f8, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = DIV_D(ctx->f8.d, ctx->f10.d);
    // 0x802E44CC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x802E44D0: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E44D4: ldc1        $f10, -0x3E10($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X3E10);
    // 0x802E44D8: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x802E44DC: addiu       $t9, $t9, -0x3768
    ctx->r25 = ADD32(ctx->r25, -0X3768);
    // 0x802E44E0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E44E4: addiu       $a1, $sp, 0x3C
    ctx->r5 = ADD32(ctx->r29, 0X3C);
    // 0x802E44E8: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    // 0x802E44EC: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x802E44F0: cvt.d.s     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f16.d = CVT_D_S(ctx->f4.fl);
    // 0x802E44F4: mul.d       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = MUL_D(ctx->f16.d, ctx->f6.d);
    // 0x802E44F8: add.d       $f18, $f8, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = ctx->f8.d + ctx->f10.d;
    // 0x802E44FC: cvt.s.d     $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f0.fl = CVT_S_D(ctx->f18.d);
    // 0x802E4500: swc1        $f0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f0.u32l;
    // 0x802E4504: swc1        $f0, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f0.u32l;
    // 0x802E4508: jalr        $t9
    // 0x802E450C: swc1        $f0, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x802E450C: swc1        $f0, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f0.u32l;
    after_12:
L_802E4510:
    // 0x802E4510: jalr        $s2
    // 0x802E4514: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_13;
    // 0x802E4514: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    after_13:
    // 0x802E4518: addiu       $t1, $v0, -0x2710
    ctx->r9 = ADD32(ctx->r2, -0X2710);
    // 0x802E451C: mtc1        $t1, $f4
    ctx->f4.u32l = ctx->r9;
    // 0x802E4520: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4524: ldc1        $f8, -0x3E08($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X3E08);
    // 0x802E4528: cvt.s.w     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    ctx->f16.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E452C: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E4530: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x802E4534: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x802E4538: lw          $s1, 0x68($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X68);
    // 0x802E453C: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x802E4540: cvt.d.s     $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f6.d = CVT_D_S(ctx->f16.fl);
    // 0x802E4544: lwc1        $f16, 0x10($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0X10);
    // 0x802E4548: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x802E454C: addiu       $s1, $s1, 0x8
    ctx->r17 = ADD32(ctx->r17, 0X8);
    // 0x802E4550: cvt.d.s     $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f6.d = CVT_D_S(ctx->f16.fl);
    // 0x802E4554: mul.d       $f4, $f10, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f10.d, ctx->f18.d);
    // 0x802E4558: add.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f4.d + ctx->f6.d;
    // 0x802E455C: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x802E4560: jalr        $s2
    // 0x802E4564: swc1        $f10, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_14;
    // 0x802E4564: swc1        $f10, 0x50($s0)
    MEM_W(0X50, ctx->r16) = ctx->f10.u32l;
    after_14:
    // 0x802E4568: addiu       $t2, $v0, -0x2710
    ctx->r10 = ADD32(ctx->r2, -0X2710);
    // 0x802E456C: mtc1        $t2, $f18
    ctx->f18.u32l = ctx->r10;
    // 0x802E4570: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4574: ldc1        $f6, -0x3E00($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, -0X3E00);
    // 0x802E4578: cvt.s.w     $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E457C: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E4580: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x802E4584: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x802E4588: cvt.d.s     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f4.d = CVT_D_S(ctx->f16.fl);
    // 0x802E458C: lwc1        $f16, 0xC($s1)
    ctx->f16.u32l = MEM_W(ctx->r17, 0XC);
    // 0x802E4590: mul.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = MUL_D(ctx->f4.d, ctx->f6.d);
    // 0x802E4594: cvt.d.s     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f4.d = CVT_D_S(ctx->f16.fl);
    // 0x802E4598: mul.d       $f18, $f8, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = MUL_D(ctx->f8.d, ctx->f10.d);
    // 0x802E459C: add.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = ctx->f18.d + ctx->f4.d;
    // 0x802E45A0: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x802E45A4: swc1        $f8, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->f8.u32l;
    // 0x802E45A8: lw          $t3, 0x0($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X0);
    // 0x802E45AC: andi        $t4, $t3, 0xFF00
    ctx->r12 = ctx->r11 & 0XFF00;
    // 0x802E45B0: sra         $t5, $t4, 8
    ctx->r13 = S32(SIGNED(ctx->r12) >> 8);
    // 0x802E45B4: sb          $t5, 0x1B($s0)
    MEM_B(0X1B, ctx->r16) = ctx->r13;
L_802E45B8:
    // 0x802E45B8: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_802E45BC:
    // 0x802E45BC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x802E45C0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x802E45C4: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x802E45C8: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x802E45CC: jr          $ra
    // 0x802E45D0: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
    return;
    // 0x802E45D0: addiu       $sp, $sp, 0x70
    ctx->r29 = ADD32(ctx->r29, 0X70);
;}
RECOMP_FUNC void map_ovl_00_func_802E45D4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E45D4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E45D8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E45DC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E45E0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E45E4: addiu       $t9, $t9, 0x76FC
    ctx->r25 = ADD32(ctx->r25, 0X76FC);
    // 0x802E45E8: jalr        $t9
    // 0x802E45EC: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E45EC: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    after_0:
    // 0x802E45F0: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E45F4: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x802E45F8: jalr        $t9
    // 0x802E45FC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E45FC: nop

    after_1:
    // 0x802E4600: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E4604: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E4608: jr          $ra
    // 0x802E460C: nop

    return;
    // 0x802E460C: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E4610(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E4610: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x802E4614: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E4618: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x802E461C: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x802E4620: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E4624: addiu       $t9, $t9, 0x938
    ctx->r25 = ADD32(ctx->r25, 0X938);
    // 0x802E4628: lw          $a0, 0x4($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X4);
    // 0x802E462C: jalr        $t9
    // 0x802E4630: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E4630: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x802E4634: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x802E4638: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x802E463C: lw          $t9, 0x18($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X18);
    // 0x802E4640: jalr        $t9
    // 0x802E4644: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E4644: nop

    after_1:
    // 0x802E4648: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E464C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x802E4650: jr          $ra
    // 0x802E4654: nop

    return;
    // 0x802E4654: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E4658(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E4658: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E465C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E4660: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x802E4664: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E4668: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E466C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E4670: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x802E4674: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x802E4678: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x802E467C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x802E4680: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x802E4684: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x802E4688: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E468C: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x802E4690: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x802E4694: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E4698: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x802E469C: lw          $t9, -0x4674($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X4674);
    // 0x802E46A0: jalr        $t9
    // 0x802E46A4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E46A4: nop

    after_0:
    // 0x802E46A8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E46AC: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x802E46B0: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x802E46B4: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x802E46B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E46BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E46C0: jr          $ra
    // 0x802E46C4: nop

    return;
    // 0x802E46C4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E46C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E46C8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x802E46CC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E46D0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E46D4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E46D8: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x802E46DC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E46E0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x802E46E4: addiu       $a2, $zero, 0x4C
    ctx->r6 = ADD32(0, 0X4C);
    // 0x802E46E8: jalr        $t9
    // 0x802E46EC: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E46EC: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x802E46F0: bne         $v0, $zero, L_802E4710
    if (ctx->r2 != 0) {
        // 0x802E46F4: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_802E4710;
    }
    // 0x802E46F4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E46F8: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x802E46FC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4700: jalr        $t9
    // 0x802E4704: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E4704: nop

    after_1:
    // 0x802E4708: b           L_802E4724
    // 0x802E470C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E4724;
    // 0x802E470C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E4710:
    // 0x802E4710: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E4714: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E4718: jalr        $t9
    // 0x802E471C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E471C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x802E4720: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E4724:
    // 0x802E4724: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E4728: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x802E472C: jr          $ra
    // 0x802E4730: nop

    return;
    // 0x802E4730: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E4734(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E4734: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x802E4738: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E473C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x802E4740: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x802E4744: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x802E4748: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E474C: addiu       $t9, $t9, -0x4890
    ctx->r25 = ADD32(ctx->r25, -0X4890);
    // 0x802E4750: sw          $t6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r14;
    // 0x802E4754: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x802E4758: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E475C: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x802E4760: lhu         $t7, 0x18($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X18);
    // 0x802E4764: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x802E4768: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x802E476C: sll         $t8, $t7, 4
    ctx->r24 = S32(ctx->r15 << 4);
    // 0x802E4770: addu        $t0, $t8, $t9
    ctx->r8 = ADD32(ctx->r24, ctx->r25);
    // 0x802E4774: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E4778: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x802E477C: sw          $t0, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r8;
    // 0x802E4780: jalr        $t9
    // 0x802E4784: sw          $v0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E4784: sw          $v0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r2;
    after_0:
    // 0x802E4788: sw          $v0, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r2;
    // 0x802E478C: beq         $v0, $zero, L_802E49B8
    if (ctx->r2 == 0) {
        // 0x802E4790: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_802E49B8;
    }
    // 0x802E4790: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x802E4794: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E4798: addiu       $t9, $t9, 0x31CC
    ctx->r25 = ADD32(ctx->r25, 0X31CC);
    // 0x802E479C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E47A0: jalr        $t9
    // 0x802E47A4: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E47A4: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_1:
    // 0x802E47A8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x802E47AC: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x802E47B0: lw          $t1, 0x2B14($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X2B14);
    // 0x802E47B4: lhu         $t4, 0x2($s1)
    ctx->r12 = MEM_HU(ctx->r17, 0X2);
    // 0x802E47B8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E47BC: sw          $t1, 0x40($s1)
    MEM_W(0X40, ctx->r17) = ctx->r9;
    // 0x802E47C0: lw          $t2, 0x3C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X3C);
    // 0x802E47C4: ori         $t5, $t4, 0x980
    ctx->r13 = ctx->r12 | 0X980;
    // 0x802E47C8: addiu       $t9, $t9, 0x6504
    ctx->r25 = ADD32(ctx->r25, 0X6504);
    // 0x802E47CC: lw          $t3, 0x0($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X0);
    // 0x802E47D0: sh          $t5, 0x2($s1)
    MEM_H(0X2, ctx->r17) = ctx->r13;
    // 0x802E47D4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x802E47D8: sw          $t3, 0x3C($s1)
    MEM_W(0X3C, ctx->r17) = ctx->r11;
    // 0x802E47DC: lw          $t6, 0x2B0C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2B0C);
    // 0x802E47E0: sw          $t6, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->r14;
    // 0x802E47E4: lw          $t7, 0x7C($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X7C);
    // 0x802E47E8: sw          $t7, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r15;
    // 0x802E47EC: lh          $t8, 0x0($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X0);
    // 0x802E47F0: ori         $t0, $t8, 0x1800
    ctx->r8 = ctx->r24 | 0X1800;
    // 0x802E47F4: sh          $t0, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r8;
    // 0x802E47F8: lw          $a1, 0x40($s1)
    ctx->r5 = MEM_W(ctx->r17, 0X40);
    // 0x802E47FC: jalr        $t9
    // 0x802E4800: lw          $a0, 0x3C($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X3C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E4800: lw          $a0, 0x3C($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X3C);
    after_2:
    // 0x802E4804: lui         $t1, 0x802E
    ctx->r9 = S32(0X802E << 16);
    // 0x802E4808: sw          $v0, 0x74($s1)
    MEM_W(0X74, ctx->r17) = ctx->r2;
    // 0x802E480C: addiu       $t1, $t1, 0x5210
    ctx->r9 = ADD32(ctx->r9, 0X5210);
    // 0x802E4810: sw          $v0, 0x68($s0)
    MEM_W(0X68, ctx->r16) = ctx->r2;
    // 0x802E4814: sw          $t1, 0x60($s0)
    MEM_W(0X60, ctx->r16) = ctx->r9;
    // 0x802E4818: jal         0x80146900
    // 0x802E481C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80146900(rdram, ctx);
        goto after_3;
    // 0x802E481C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x802E4820: lw          $v1, 0x4C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X4C);
    // 0x802E4824: lwc1        $f4, 0x50($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X50);
    // 0x802E4828: lw          $v0, 0x3C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X3C);
    // 0x802E482C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x802E4830: swc1        $f4, 0x2C($v1)
    MEM_W(0X2C, ctx->r3) = ctx->f4.u32l;
    // 0x802E4834: lwc1        $f6, 0x54($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X54);
    // 0x802E4838: lui         $t6, 0x802E
    ctx->r14 = S32(0X802E << 16);
    // 0x802E483C: addiu       $t6, $t6, 0x51B0
    ctx->r14 = ADD32(ctx->r14, 0X51B0);
    // 0x802E4840: swc1        $f6, 0x30($v1)
    MEM_W(0X30, ctx->r3) = ctx->f6.u32l;
    // 0x802E4844: lwc1        $f8, 0x58($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X58);
    // 0x802E4848: swc1        $f0, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = ctx->f0.u32l;
    // 0x802E484C: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x802E4850: swc1        $f8, 0x2C($v1)
    MEM_W(0X2C, ctx->r3) = ctx->f8.u32l;
    // 0x802E4854: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x802E4858: swc1        $f0, 0x38($v1)
    MEM_W(0X38, ctx->r3) = ctx->f0.u32l;
    // 0x802E485C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4860: swc1        $f10, 0x30($v1)
    MEM_W(0X30, ctx->r3) = ctx->f10.u32l;
    // 0x802E4864: lh          $t2, 0x8($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X8);
    // 0x802E4868: mtc1        $t2, $f16
    ctx->f16.u32l = ctx->r10;
    // 0x802E486C: nop

    // 0x802E4870: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x802E4874: trunc.w.s   $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.u32l = TRUNC_W_S(ctx->f18.fl);
    // 0x802E4878: mfc1        $t4, $f4
    ctx->r12 = (int32_t)ctx->f4.u32l;
    // 0x802E487C: nop

    // 0x802E4880: sh          $t4, 0x3C($v1)
    MEM_H(0X3C, ctx->r3) = ctx->r12;
    // 0x802E4884: lw          $t5, 0x10($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X10);
    // 0x802E4888: sw          $t5, 0x10($v1)
    MEM_W(0X10, ctx->r3) = ctx->r13;
    // 0x802E488C: sw          $t6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r14;
    // 0x802E4890: jal         0x80143234
    // 0x802E4894: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    func_80143234(rdram, ctx);
        goto after_4;
    // 0x802E4894: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    after_4:
    // 0x802E4898: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E489C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E48A0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E48A4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    // 0x802E48A8: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x802E48AC: jalr        $t9
    // 0x802E48B0: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E48B0: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    after_5:
    // 0x802E48B4: lw          $t7, 0x3C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X3C);
    // 0x802E48B8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E48BC: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x802E48C0: lw          $v0, 0xC($t7)
    ctx->r2 = MEM_W(ctx->r15, 0XC);
    // 0x802E48C4: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E48C8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x802E48CC: beq         $v0, $zero, L_802E48F0
    if (ctx->r2 == 0) {
        // 0x802E48D0: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_802E48F0;
    }
    // 0x802E48D0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x802E48D4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E48D8: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E48DC: jalr        $t9
    // 0x802E48E0: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E48E0: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    after_6:
    // 0x802E48E4: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x802E48E8: b           L_802E48F0
    // 0x802E48EC: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
        goto L_802E48F0;
    // 0x802E48EC: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
L_802E48F0:
    // 0x802E48F0: beq         $a0, $zero, L_802E4968
    if (ctx->r4 == 0) {
        // 0x802E48F4: lw          $t2, 0x40($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X40);
            goto L_802E4968;
    }
    // 0x802E48F4: lw          $t2, 0x40($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X40);
    // 0x802E48F8: lw          $t8, 0x3C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X3C);
    // 0x802E48FC: lwc1        $f18, 0x54($s1)
    ctx->f18.u32l = MEM_W(ctx->r17, 0X54);
    // 0x802E4900: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E4904: lh          $t0, 0x8($t8)
    ctx->r8 = MEM_H(ctx->r24, 0X8);
    // 0x802E4908: lwc1        $f6, 0x4($t8)
    ctx->f6.u32l = MEM_W(ctx->r24, 0X4);
    // 0x802E490C: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E4910: mtc1        $t0, $f8
    ctx->f8.u32l = ctx->r8;
    // 0x802E4914: addiu       $t9, $t9, 0x76FC
    ctx->r25 = ADD32(ctx->r25, 0X76FC);
    // 0x802E4918: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E491C: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x802E4920: add.s       $f4, $f18, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f16.fl;
    // 0x802E4924: swc1        $f4, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->f4.u32l;
    // 0x802E4928: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x802E492C: lhu         $v0, 0x18($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X18);
    // 0x802E4930: beq         $v0, $at, L_802E4940
    if (ctx->r2 == ctx->r1) {
        // 0x802E4934: addiu       $at, $zero, 0x3
        ctx->r1 = ADD32(0, 0X3);
            goto L_802E4940;
    }
    // 0x802E4934: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x802E4938: bne         $v0, $at, L_802E4948
    if (ctx->r2 != ctx->r1) {
        // 0x802E493C: nop
    
            goto L_802E4948;
    }
    // 0x802E493C: nop

L_802E4940:
    // 0x802E4940: jalr        $t9
    // 0x802E4944: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x802E4944: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    after_7:
L_802E4948:
    // 0x802E4948: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E494C: addiu       $t9, $t9, 0x1ED8
    ctx->r25 = ADD32(ctx->r25, 0X1ED8);
    // 0x802E4950: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x802E4954: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x802E4958: jalr        $t9
    // 0x802E495C: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x802E495C: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    after_8:
    // 0x802E4960: b           L_802E49BC
    // 0x802E4964: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E49BC;
    // 0x802E4964: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E4968:
    // 0x802E4968: lhu         $t3, 0x18($t2)
    ctx->r11 = MEM_HU(ctx->r10, 0X18);
    // 0x802E496C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x802E4970: bne         $t3, $at, L_802E49A4
    if (ctx->r11 != ctx->r1) {
        // 0x802E4974: nop
    
            goto L_802E49A4;
    }
    // 0x802E4974: nop

    // 0x802E4978: lwc1        $f0, 0x30($v1)
    ctx->f0.u32l = MEM_W(ctx->r3, 0X30);
    // 0x802E497C: lwc1        $f8, 0x28($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X28);
    // 0x802E4980: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E4984: swc1        $f0, 0x34($v1)
    MEM_W(0X34, ctx->r3) = ctx->f0.u32l;
    // 0x802E4988: add.s       $f6, $f8, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f0.fl;
    // 0x802E498C: addiu       $t9, $t9, 0x7728
    ctx->r25 = ADD32(ctx->r25, 0X7728);
    // 0x802E4990: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x802E4994: swc1        $f6, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->f6.u32l;
    // 0x802E4998: jalr        $t9
    // 0x802E499C: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x802E499C: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    after_9:
    // 0x802E49A0: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
L_802E49A4:
    // 0x802E49A4: lui         $t9, 0x8015
    ctx->r25 = S32(0X8015 << 16);
    // 0x802E49A8: addiu       $t9, $t9, -0x7BEC
    ctx->r25 = ADD32(ctx->r25, -0X7BEC);
    // 0x802E49AC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E49B0: jalr        $t9
    // 0x802E49B4: addiu       $a1, $v1, 0x18
    ctx->r5 = ADD32(ctx->r3, 0X18);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x802E49B4: addiu       $a1, $v1, 0x18
    ctx->r5 = ADD32(ctx->r3, 0X18);
    after_10:
L_802E49B8:
    // 0x802E49B8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E49BC:
    // 0x802E49BC: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x802E49C0: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x802E49C4: jr          $ra
    // 0x802E49C8: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x802E49C8: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void map_ovl_00_func_802E49CC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E49CC: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x802E49D0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E49D4: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x802E49D8: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x802E49DC: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x802E49E0: lui         $t7, 0x802F
    ctx->r15 = S32(0X802F << 16);
    // 0x802E49E4: addiu       $t7, $t7, -0x4668
    ctx->r15 = ADD32(ctx->r15, -0X4668);
    // 0x802E49E8: sw          $t6, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r14;
    // 0x802E49EC: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x802E49F0: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x802E49F4: addiu       $a1, $sp, 0x3C
    ctx->r5 = ADD32(ctx->r29, 0X3C);
    // 0x802E49F8: lw          $s1, 0x24($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X24);
    // 0x802E49FC: sw          $t9, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r25;
    // 0x802E4A00: lw          $t8, 0x4($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X4);
    // 0x802E4A04: lui         $t1, 0x802F
    ctx->r9 = S32(0X802F << 16);
    // 0x802E4A08: addiu       $t1, $t1, -0x465C
    ctx->r9 = ADD32(ctx->r9, -0X465C);
    // 0x802E4A0C: sw          $t8, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r24;
    // 0x802E4A10: lw          $t9, 0x8($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X8);
    // 0x802E4A14: addiu       $t0, $sp, 0x30
    ctx->r8 = ADD32(ctx->r29, 0X30);
    // 0x802E4A18: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E4A1C: sw          $t9, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->r25;
    // 0x802E4A20: lw          $t3, 0x0($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X0);
    // 0x802E4A24: lw          $t2, 0x4($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X4);
    // 0x802E4A28: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E4A2C: sw          $t3, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r11;
    // 0x802E4A30: lw          $t3, 0x8($t1)
    ctx->r11 = MEM_W(ctx->r9, 0X8);
    // 0x802E4A34: sw          $t2, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->r10;
    // 0x802E4A38: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4A3C: sw          $t3, 0x8($t0)
    MEM_W(0X8, ctx->r8) = ctx->r11;
    // 0x802E4A40: lhu         $v1, 0x18($v0)
    ctx->r3 = MEM_HU(ctx->r2, 0X18);
    // 0x802E4A44: beq         $v1, $at, L_802E4A54
    if (ctx->r3 == ctx->r1) {
        // 0x802E4A48: addiu       $at, $zero, 0x3
        ctx->r1 = ADD32(0, 0X3);
            goto L_802E4A54;
    }
    // 0x802E4A48: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x802E4A4C: bne         $v1, $at, L_802E4A98
    if (ctx->r3 != ctx->r1) {
        // 0x802E4A50: lui         $t9, 0x8014
        ctx->r25 = S32(0X8014 << 16);
            goto L_802E4A98;
    }
    // 0x802E4A50: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
L_802E4A54:
    // 0x802E4A54: lui         $v1, 0x8014
    ctx->r3 = S32(0X8014 << 16);
    // 0x802E4A58: addiu       $v1, $v1, 0x314C
    ctx->r3 = ADD32(ctx->r3, 0X314C);
    // 0x802E4A5C: jalr        $v1
    // 0x802E4A60: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_0;
    // 0x802E4A60: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x802E4A64: beq         $v0, $zero, L_802E4AC8
    if (ctx->r2 == 0) {
        // 0x802E4A68: lw          $v1, 0x24($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X24);
            goto L_802E4AC8;
    }
    // 0x802E4A68: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x802E4A6C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4A70: jalr        $v1
    // 0x802E4A74: addiu       $a1, $sp, 0x30
    ctx->r5 = ADD32(ctx->r29, 0X30);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_1;
    // 0x802E4A74: addiu       $a1, $sp, 0x30
    ctx->r5 = ADD32(ctx->r29, 0X30);
    after_1:
    // 0x802E4A78: beql        $v0, $zero, L_802E4ACC
    if (ctx->r2 == 0) {
        // 0x802E4A7C: lw          $v0, 0x5C($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X5C);
            goto L_802E4ACC;
    }
    goto skip_0;
    // 0x802E4A7C: lw          $v0, 0x5C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X5C);
    skip_0:
    // 0x802E4A80: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x802E4A84: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4A88: jalr        $t9
    // 0x802E4A8C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E4A8C: nop

    after_2:
    // 0x802E4A90: b           L_802E4B60
    // 0x802E4A94: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E4B60;
    // 0x802E4A94: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E4A98:
    // 0x802E4A98: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x802E4A9C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4AA0: jalr        $t9
    // 0x802E4AA4: addiu       $a1, $s1, 0x50
    ctx->r5 = ADD32(ctx->r17, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E4AA4: addiu       $a1, $s1, 0x50
    ctx->r5 = ADD32(ctx->r17, 0X50);
    after_3:
    // 0x802E4AA8: beql        $v0, $zero, L_802E4ACC
    if (ctx->r2 == 0) {
        // 0x802E4AAC: lw          $v0, 0x5C($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X5C);
            goto L_802E4ACC;
    }
    goto skip_1;
    // 0x802E4AAC: lw          $v0, 0x5C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X5C);
    skip_1:
    // 0x802E4AB0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x802E4AB4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4AB8: jalr        $t9
    // 0x802E4ABC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E4ABC: nop

    after_4:
    // 0x802E4AC0: b           L_802E4B60
    // 0x802E4AC4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E4B60;
    // 0x802E4AC4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E4AC8:
    // 0x802E4AC8: lw          $v0, 0x5C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X5C);
L_802E4ACC:
    // 0x802E4ACC: lui         $t9, 0x8015
    ctx->r25 = S32(0X8015 << 16);
    // 0x802E4AD0: addiu       $t9, $t9, -0x7BEC
    ctx->r25 = ADD32(ctx->r25, -0X7BEC);
    // 0x802E4AD4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x802E4AD8: addiu       $a1, $v0, 0x18
    ctx->r5 = ADD32(ctx->r2, 0X18);
    // 0x802E4ADC: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x802E4AE0: jalr        $t9
    // 0x802E4AE4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E4AE4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_5:
    // 0x802E4AE8: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    // 0x802E4AEC: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E4AF0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E4AF4: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E4AF8: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E4AFC: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E4B00: sll         $t4, $v1, 1
    ctx->r12 = S32(ctx->r3 << 1);
    // 0x802E4B04: addu        $v0, $s0, $t4
    ctx->r2 = ADD32(ctx->r16, ctx->r12);
    // 0x802E4B08: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x802E4B0C: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x802E4B10: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x802E4B14: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x802E4B18: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x802E4B1C: sb          $t6, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r14;
    // 0x802E4B20: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x802E4B24: lw          $t9, -0x4650($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X4650);
    // 0x802E4B28: jalr        $t9
    // 0x802E4B2C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E4B2C: nop

    after_6:
    // 0x802E4B30: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x802E4B34: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x802E4B38: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x802E4B3C: addiu       $t1, $t0, -0x1
    ctx->r9 = ADD32(ctx->r8, -0X1);
    // 0x802E4B40: sh          $t1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r9;
    // 0x802E4B44: lw          $t2, 0x2B0C($v0)
    ctx->r10 = MEM_W(ctx->r2, 0X2B0C);
    // 0x802E4B48: sw          $t2, 0x18($s1)
    MEM_W(0X18, ctx->r17) = ctx->r10;
    // 0x802E4B4C: lw          $t3, 0x7C($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X7C);
    // 0x802E4B50: sw          $t3, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r11;
    // 0x802E4B54: lw          $t4, 0x24($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X24);
    // 0x802E4B58: sw          $zero, 0x40($t4)
    MEM_W(0X40, ctx->r12) = 0;
    // 0x802E4B5C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E4B60:
    // 0x802E4B60: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x802E4B64: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x802E4B68: jr          $ra
    // 0x802E4B6C: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    return;
    // 0x802E4B6C: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
;}
RECOMP_FUNC void map_ovl_00_func_802E4B70(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E4B70: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x802E4B74: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E4B78: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x802E4B7C: lw          $a2, 0x70($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X70);
    // 0x802E4B80: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E4B84: addiu       $t9, $t9, -0x4890
    ctx->r25 = ADD32(ctx->r25, -0X4890);
    // 0x802E4B88: lhu         $t7, 0x18($a2)
    ctx->r15 = MEM_HU(ctx->r6, 0X18);
    // 0x802E4B8C: lw          $t0, 0x34($a0)
    ctx->r8 = MEM_W(ctx->r4, 0X34);
    // 0x802E4B90: lw          $t1, 0x24($a0)
    ctx->r9 = MEM_W(ctx->r4, 0X24);
    // 0x802E4B94: sll         $t8, $t7, 4
    ctx->r24 = S32(ctx->r15 << 4);
    // 0x802E4B98: addu        $a3, $t8, $t9
    ctx->r7 = ADD32(ctx->r24, ctx->r25);
    // 0x802E4B9C: lw          $v0, 0xC($a3)
    ctx->r2 = MEM_W(ctx->r7, 0XC);
    // 0x802E4BA0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E4BA4: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E4BA8: beq         $v0, $zero, L_802E4BE8
    if (ctx->r2 == 0) {
        // 0x802E4BAC: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_802E4BE8;
    }
    // 0x802E4BAC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x802E4BB0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E4BB4: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E4BB8: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x802E4BBC: sw          $a2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r6;
    // 0x802E4BC0: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    // 0x802E4BC4: sw          $t0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r8;
    // 0x802E4BC8: jalr        $t9
    // 0x802E4BCC: sw          $t1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E4BCC: sw          $t1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r9;
    after_0:
    // 0x802E4BD0: lw          $a2, 0x34($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X34);
    // 0x802E4BD4: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x802E4BD8: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
    // 0x802E4BDC: lw          $t1, 0x20($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X20);
    // 0x802E4BE0: b           L_802E4BE8
    // 0x802E4BE4: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_802E4BE8;
    // 0x802E4BE4: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_802E4BE8:
    // 0x802E4BE8: beql        $v1, $zero, L_802E4CBC
    if (ctx->r3 == 0) {
        // 0x802E4BEC: lhu         $t8, 0x18($a2)
        ctx->r24 = MEM_HU(ctx->r6, 0X18);
            goto L_802E4CBC;
    }
    goto skip_0;
    // 0x802E4BEC: lhu         $t8, 0x18($a2)
    ctx->r24 = MEM_HU(ctx->r6, 0X18);
    skip_0:
    // 0x802E4BF0: lh          $t2, 0xA($a3)
    ctx->r10 = MEM_H(ctx->r7, 0XA);
    // 0x802E4BF4: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E4BF8: addiu       $v1, $t0, 0x8
    ctx->r3 = ADD32(ctx->r8, 0X8);
    // 0x802E4BFC: sll         $t3, $t2, 4
    ctx->r11 = S32(ctx->r10 << 4);
    // 0x802E4C00: subu        $t3, $t3, $t2
    ctx->r11 = SUB32(ctx->r11, ctx->r10);
    // 0x802E4C04: sll         $t3, $t3, 1
    ctx->r11 = S32(ctx->r11 << 1);
    // 0x802E4C08: sh          $t3, 0x3E($v1)
    MEM_H(0X3E, ctx->r3) = ctx->r11;
    // 0x802E4C0C: swc1        $f4, 0x34($v1)
    MEM_W(0X34, ctx->r3) = ctx->f4.u32l;
    // 0x802E4C10: lhu         $t4, 0x18($a2)
    ctx->r12 = MEM_HU(ctx->r6, 0X18);
    // 0x802E4C14: addiu       $a0, $zero, 0x29E
    ctx->r4 = ADD32(0, 0X29E);
    // 0x802E4C18: addiu       $a1, $t1, 0x50
    ctx->r5 = ADD32(ctx->r9, 0X50);
    // 0x802E4C1C: andi        $t5, $t4, 0x2
    ctx->r13 = ctx->r12 & 0X2;
    // 0x802E4C20: bne         $t5, $zero, L_802E4C94
    if (ctx->r13 != 0) {
        // 0x802E4C24: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_802E4C94;
    }
    // 0x802E4C24: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E4C28: addiu       $t9, $t9, 0x6DD8
    ctx->r25 = ADD32(ctx->r25, 0X6DD8);
    // 0x802E4C2C: jalr        $t9
    // 0x802E4C30: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E4C30: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    after_1:
    // 0x802E4C34: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x802E4C38: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E4C3C: addiu       $t9, $t9, 0x717C
    ctx->r25 = ADD32(ctx->r25, 0X717C);
    // 0x802E4C40: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x802E4C44: jalr        $t9
    // 0x802E4C48: sw          $v0, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E4C48: sw          $v0, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->r2;
    after_2:
    // 0x802E4C4C: beq         $v0, $zero, L_802E4C98
    if (ctx->r2 == 0) {
        // 0x802E4C50: lw          $v1, 0x18($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X18);
            goto L_802E4C98;
    }
    // 0x802E4C50: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x802E4C54: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E4C58: addiu       $t9, $t9, 0x6FB8
    ctx->r25 = ADD32(ctx->r25, 0X6FB8);
    // 0x802E4C5C: lw          $a0, 0x14($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X14);
    // 0x802E4C60: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    // 0x802E4C64: lui         $a1, 0x41A0
    ctx->r5 = S32(0X41A0 << 16);
    // 0x802E4C68: jalr        $t9
    // 0x802E4C6C: lui         $a2, 0x43FA
    ctx->r6 = S32(0X43FA << 16);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E4C6C: lui         $a2, 0x43FA
    ctx->r6 = S32(0X43FA << 16);
    after_3:
    // 0x802E4C70: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x802E4C74: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E4C78: addiu       $t9, $t9, 0x700C
    ctx->r25 = ADD32(ctx->r25, 0X700C);
    // 0x802E4C7C: lui         $a1, 0x4000
    ctx->r5 = S32(0X4000 << 16);
    // 0x802E4C80: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x802E4C84: jalr        $t9
    // 0x802E4C88: lw          $a0, 0x14($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X14);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E4C88: lw          $a0, 0x14($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X14);
    after_4:
    // 0x802E4C8C: b           L_802E4C9C
    // 0x802E4C90: lw          $t7, 0x38($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X38);
        goto L_802E4C9C;
    // 0x802E4C90: lw          $t7, 0x38($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X38);
L_802E4C94:
    // 0x802E4C94: sw          $zero, 0x14($v1)
    MEM_W(0X14, ctx->r3) = 0;
L_802E4C98:
    // 0x802E4C98: lw          $t7, 0x38($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X38);
L_802E4C9C:
    // 0x802E4C9C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E4CA0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E4CA4: addiu       $a0, $t7, 0x8
    ctx->r4 = ADD32(ctx->r15, 0X8);
    // 0x802E4CA8: jalr        $t9
    // 0x802E4CAC: addiu       $a1, $t7, 0xE
    ctx->r5 = ADD32(ctx->r15, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E4CAC: addiu       $a1, $t7, 0xE
    ctx->r5 = ADD32(ctx->r15, 0XE);
    after_5:
    // 0x802E4CB0: b           L_802E4D00
    // 0x802E4CB4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_802E4D00;
    // 0x802E4CB4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E4CB8: lhu         $t8, 0x18($a2)
    ctx->r24 = MEM_HU(ctx->r6, 0X18);
L_802E4CBC:
    // 0x802E4CBC: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x802E4CC0: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x802E4CC4: bne         $t8, $at, L_802E4CFC
    if (ctx->r24 != ctx->r1) {
        // 0x802E4CC8: lui         $t9, 0x8014
        ctx->r25 = S32(0X8014 << 16);
            goto L_802E4CFC;
    }
    // 0x802E4CC8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E4CCC: addiu       $t9, $t9, 0x7718
    ctx->r25 = ADD32(ctx->r25, 0X7718);
    // 0x802E4CD0: sw          $t0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r8;
    // 0x802E4CD4: jalr        $t9
    // 0x802E4CD8: sw          $t1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E4CD8: sw          $t1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r9;
    after_6:
    // 0x802E4CDC: lw          $t0, 0x30($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X30);
    // 0x802E4CE0: beq         $v0, $zero, L_802E4CFC
    if (ctx->r2 == 0) {
        // 0x802E4CE4: lw          $t1, 0x20($sp)
        ctx->r9 = MEM_W(ctx->r29, 0X20);
            goto L_802E4CFC;
    }
    // 0x802E4CE4: lw          $t1, 0x20($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X20);
    // 0x802E4CE8: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x802E4CEC: addiu       $v1, $t0, 0x8
    ctx->r3 = ADD32(ctx->r8, 0X8);
    // 0x802E4CF0: lwc1        $f8, 0x28($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X28);
    // 0x802E4CF4: swc1        $f6, 0x34($v1)
    MEM_W(0X34, ctx->r3) = ctx->f6.u32l;
    // 0x802E4CF8: swc1        $f8, 0x54($t1)
    MEM_W(0X54, ctx->r9) = ctx->f8.u32l;
L_802E4CFC:
    // 0x802E4CFC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E4D00:
    // 0x802E4D00: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x802E4D04: jr          $ra
    // 0x802E4D08: nop

    return;
    // 0x802E4D08: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E4D0C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E4D0C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x802E4D10: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E4D14: lw          $t6, 0x24($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X24);
    // 0x802E4D18: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x802E4D1C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x802E4D20: sw          $t6, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r14;
    // 0x802E4D24: lw          $a0, 0x1C($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X1C);
    // 0x802E4D28: sw          $a1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r5;
    // 0x802E4D2C: jal         0x8001717C
    // 0x802E4D30: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    func_8001717C(rdram, ctx);
        goto after_0;
    // 0x802E4D30: sw          $v0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r2;
    after_0:
    // 0x802E4D34: beq         $v0, $zero, L_802E4D4C
    if (ctx->r2 == 0) {
        // 0x802E4D38: lw          $t7, 0x2C($sp)
        ctx->r15 = MEM_W(ctx->r29, 0X2C);
            goto L_802E4D4C;
    }
    // 0x802E4D38: lw          $t7, 0x2C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X2C);
    // 0x802E4D3C: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x802E4D40: lw          $a0, 0x1C($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X1C);
    // 0x802E4D44: jal         0x800170E8
    // 0x802E4D48: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    func_800170E8(rdram, ctx);
        goto after_1;
    // 0x802E4D48: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_1:
L_802E4D4C:
    // 0x802E4D4C: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x802E4D50: lui         $a1, 0x8000
    ctx->r5 = S32(0X8000 << 16);
    // 0x802E4D54: addiu       $a1, $a1, 0x47C0
    ctx->r5 = ADD32(ctx->r5, 0X47C0);
    // 0x802E4D58: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x802E4D5C: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x802E4D60: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    // 0x802E4D64: jalr        $a1
    // 0x802E4D68: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    LOOKUP_FUNC(ctx->r5)(rdram, ctx);
        goto after_2;
    // 0x802E4D68: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    after_2:
    // 0x802E4D6C: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x802E4D70: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x802E4D74: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x802E4D78: bgez        $v0, L_802E4D90
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E4D7C: cvt.s.w     $f6, $f4
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
            goto L_802E4D90;
    }
    // 0x802E4D7C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E4D80: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E4D84: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x802E4D88: nop

    // 0x802E4D8C: add.s       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f8.fl;
L_802E4D90:
    // 0x802E4D90: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4D94: ldc1        $f16, -0x3DF8($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, -0X3DF8);
    // 0x802E4D98: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x802E4D9C: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E4DA0: mul.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f16.d);
    // 0x802E4DA4: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E4DA8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E4DAC: lwc1        $f6, 0x24($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X24);
    // 0x802E4DB0: lui         $at, 0x3FD0
    ctx->r1 = S32(0X3FD0 << 16);
    // 0x802E4DB4: lw          $t8, 0x20($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X20);
    // 0x802E4DB8: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    // 0x802E4DBC: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x802E4DC0: mul.d       $f8, $f18, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f8.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x802E4DC4: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x802E4DC8: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x802E4DCC: add.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d + ctx->f10.d;
    // 0x802E4DD0: sub.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f16.d - ctx->f18.d;
    // 0x802E4DD4: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x802E4DD8: jalr        $a1
    // 0x802E4DDC: swc1        $f6, 0x50($t8)
    MEM_W(0X50, ctx->r24) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r5)(rdram, ctx);
        goto after_3;
    // 0x802E4DDC: swc1        $f6, 0x50($t8)
    MEM_W(0X50, ctx->r24) = ctx->f6.u32l;
    after_3:
    // 0x802E4DE0: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x802E4DE4: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x802E4DE8: bgez        $v0, L_802E4E00
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E4DEC: cvt.s.w     $f10, $f8
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
            goto L_802E4E00;
    }
    // 0x802E4DEC: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E4DF0: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E4DF4: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x802E4DF8: nop

    // 0x802E4DFC: add.s       $f10, $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f10.fl + ctx->f16.fl;
L_802E4E00:
    // 0x802E4E00: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4E04: ldc1        $f4, -0x3DF0($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X3DF0);
    // 0x802E4E08: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x802E4E0C: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E4E10: mul.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x802E4E14: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x802E4E18: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x802E4E1C: lwc1        $f10, 0x2C($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X2C);
    // 0x802E4E20: lui         $at, 0x3FD0
    ctx->r1 = S32(0X3FD0 << 16);
    // 0x802E4E24: lw          $t9, 0x20($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X20);
    // 0x802E4E28: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x802E4E2C: mul.d       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f16.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x802E4E30: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x802E4E34: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x802E4E38: add.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f16.d + ctx->f18.d;
    // 0x802E4E3C: sub.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f4.d - ctx->f6.d;
    // 0x802E4E40: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x802E4E44: swc1        $f10, 0x58($t9)
    MEM_W(0X58, ctx->r25) = ctx->f10.u32l;
    // 0x802E4E48: lh          $t0, 0x3E($v1)
    ctx->r8 = MEM_H(ctx->r3, 0X3E);
    // 0x802E4E4C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E4E50: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E4E54: addiu       $t1, $t0, -0x1
    ctx->r9 = ADD32(ctx->r8, -0X1);
    // 0x802E4E58: sh          $t1, 0x3E($v1)
    MEM_H(0X3E, ctx->r3) = ctx->r9;
    // 0x802E4E5C: lh          $t2, 0x3E($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X3E);
    // 0x802E4E60: lw          $v0, 0x30($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X30);
    // 0x802E4E64: bgez        $t2, L_802E4E74
    if (SIGNED(ctx->r10) >= 0) {
        // 0x802E4E68: addiu       $a0, $v0, 0x8
        ctx->r4 = ADD32(ctx->r2, 0X8);
            goto L_802E4E74;
    }
    // 0x802E4E68: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x802E4E6C: jalr        $t9
    // 0x802E4E70: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E4E70: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_4:
L_802E4E74:
    // 0x802E4E74: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E4E78: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x802E4E7C: jr          $ra
    // 0x802E4E80: nop

    return;
    // 0x802E4E80: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E4E84(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E4E84: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x802E4E88: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x802E4E8C: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x802E4E90: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x802E4E94: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E4E98: lw          $t6, 0x70($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X70);
    // 0x802E4E9C: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x802E4EA0: lui         $v1, 0x8001
    ctx->r3 = S32(0X8001 << 16);
    // 0x802E4EA4: sw          $t6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r14;
    // 0x802E4EA8: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x802E4EAC: lw          $s1, 0x24($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X24);
    // 0x802E4EB0: addiu       $v1, $v1, 0x717C
    ctx->r3 = ADD32(ctx->r3, 0X717C);
    // 0x802E4EB4: lw          $a0, 0x1C($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X1C);
    // 0x802E4EB8: sw          $v1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r3;
    // 0x802E4EBC: jalr        $v1
    // 0x802E4EC0: sw          $v0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_0;
    // 0x802E4EC0: sw          $v0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r2;
    after_0:
    // 0x802E4EC4: beq         $v0, $zero, L_802E4EE0
    if (ctx->r2 == 0) {
        // 0x802E4EC8: lw          $t7, 0x48($sp)
        ctx->r15 = MEM_W(ctx->r29, 0X48);
            goto L_802E4EE0;
    }
    // 0x802E4EC8: lw          $t7, 0x48($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X48);
    // 0x802E4ECC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E4ED0: addiu       $t9, $t9, 0x70E8
    ctx->r25 = ADD32(ctx->r25, 0X70E8);
    // 0x802E4ED4: lw          $a0, 0x1C($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X1C);
    // 0x802E4ED8: jalr        $t9
    // 0x802E4EDC: addiu       $a1, $s1, 0x50
    ctx->r5 = ADD32(ctx->r17, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E4EDC: addiu       $a1, $s1, 0x50
    ctx->r5 = ADD32(ctx->r17, 0X50);
    after_1:
L_802E4EE0:
    // 0x802E4EE0: lw          $s0, 0x48($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X48);
    // 0x802E4EE4: lui         $v1, 0x8000
    ctx->r3 = S32(0X8000 << 16);
    // 0x802E4EE8: addiu       $v1, $v1, 0x47C0
    ctx->r3 = ADD32(ctx->r3, 0X47C0);
    // 0x802E4EEC: sw          $v1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r3;
    // 0x802E4EF0: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    // 0x802E4EF4: jalr        $v1
    // 0x802E4EF8: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_2;
    // 0x802E4EF8: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    after_2:
    // 0x802E4EFC: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x802E4F00: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x802E4F04: bgez        $v0, L_802E4F1C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E4F08: cvt.s.w     $f6, $f4
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
            goto L_802E4F1C;
    }
    // 0x802E4F08: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E4F0C: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E4F10: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x802E4F14: nop

    // 0x802E4F18: add.s       $f6, $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f8.fl;
L_802E4F1C:
    // 0x802E4F1C: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4F20: ldc1        $f16, -0x3DE8($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, -0X3DE8);
    // 0x802E4F24: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x802E4F28: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E4F2C: mul.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f16.d);
    // 0x802E4F30: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E4F34: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E4F38: lwc1        $f6, 0x24($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X24);
    // 0x802E4F3C: lui         $at, 0x3FD0
    ctx->r1 = S32(0X3FD0 << 16);
    // 0x802E4F40: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    // 0x802E4F44: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x802E4F48: mul.d       $f8, $f18, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f8.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x802E4F4C: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x802E4F50: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x802E4F54: add.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d + ctx->f10.d;
    // 0x802E4F58: sub.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f16.d - ctx->f18.d;
    // 0x802E4F5C: cvt.s.d     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f6.fl = CVT_S_D(ctx->f4.d);
    // 0x802E4F60: jalr        $v1
    // 0x802E4F64: swc1        $f6, 0x50($s1)
    MEM_W(0X50, ctx->r17) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_3;
    // 0x802E4F64: swc1        $f6, 0x50($s1)
    MEM_W(0X50, ctx->r17) = ctx->f6.u32l;
    after_3:
    // 0x802E4F68: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x802E4F6C: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E4F70: bgez        $v0, L_802E4F84
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E4F74: cvt.s.w     $f10, $f8
        CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
            goto L_802E4F84;
    }
    // 0x802E4F74: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E4F78: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x802E4F7C: nop

    // 0x802E4F80: add.s       $f10, $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = ctx->f10.fl + ctx->f16.fl;
L_802E4F84:
    // 0x802E4F84: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4F88: ldc1        $f4, -0x3DE0($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X3DE0);
    // 0x802E4F8C: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x802E4F90: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E4F94: mul.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x802E4F98: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x802E4F9C: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x802E4FA0: lwc1        $f10, 0x2C($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X2C);
    // 0x802E4FA4: lui         $at, 0x3FD0
    ctx->r1 = S32(0X3FD0 << 16);
    // 0x802E4FA8: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x802E4FAC: mul.d       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f16.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x802E4FB0: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x802E4FB4: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x802E4FB8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x802E4FBC: add.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = ctx->f16.d + ctx->f18.d;
    // 0x802E4FC0: sub.d       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f4.d - ctx->f6.d;
    // 0x802E4FC4: cvt.s.d     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f10.fl = CVT_S_D(ctx->f8.d);
    // 0x802E4FC8: swc1        $f10, 0x58($s1)
    MEM_W(0X58, ctx->r17) = ctx->f10.u32l;
    // 0x802E4FCC: lwc1        $f14, -0x7D38($at)
    ctx->f14.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x802E4FD0: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E4FD4: ldc1        $f16, -0x3DD8($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, -0X3DD8);
    // 0x802E4FD8: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x802E4FDC: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x802E4FE0: lwc1        $f12, 0x38($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X38);
    // 0x802E4FE4: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x802E4FE8: mul.s       $f4, $f14, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f18.fl);
    // 0x802E4FEC: cvt.d.s     $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.d = CVT_D_S(ctx->f12.fl);
    // 0x802E4FF0: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x802E4FF4: div.d       $f0, $f16, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d); 
    ctx->f0.d = DIV_D(ctx->f16.d, ctx->f6.d);
    // 0x802E4FF8: c.lt.d      $f2, $f0
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f2.d < ctx->f0.d;
    // 0x802E4FFC: nop

    // 0x802E5000: bc1fl       L_802E5030
    if (!c1cs) {
        // 0x802E5004: lh          $t8, 0x3C($s0)
        ctx->r24 = MEM_H(ctx->r16, 0X3C);
            goto L_802E5030;
    }
    goto skip_0;
    // 0x802E5004: lh          $t8, 0x3C($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X3C);
    skip_0:
    // 0x802E5008: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x802E500C: nop

    // 0x802E5010: mul.s       $f10, $f14, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f8.fl);
    // 0x802E5014: cvt.d.s     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f18.d = CVT_D_S(ctx->f10.fl);
    // 0x802E5018: div.d       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = DIV_D(ctx->f0.d, ctx->f18.d);
    // 0x802E501C: add.d       $f16, $f2, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f4.d); 
    ctx->f16.d = ctx->f2.d + ctx->f4.d;
    // 0x802E5020: cvt.s.d     $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f6.fl = CVT_S_D(ctx->f16.d);
    // 0x802E5024: swc1        $f6, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->f6.u32l;
    // 0x802E5028: lwc1        $f12, 0x38($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X38);
    // 0x802E502C: lh          $t8, 0x3C($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X3C);
L_802E5030:
    // 0x802E5030: mtc1        $zero, $f19
    ctx->f_odd[(19 - 1) * 2] = 0;
    // 0x802E5034: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x802E5038: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x802E503C: lwc1        $f8, 0x34($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X34);
    // 0x802E5040: cvt.d.w     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    ctx->f16.d = CVT_D_W(ctx->f4.u32l);
    // 0x802E5044: add.s       $f10, $f8, $f12
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f12.fl;
    // 0x802E5048: c.lt.d      $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f18.d < ctx->f16.d;
    // 0x802E504C: swc1        $f10, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->f10.u32l;
    // 0x802E5050: bc1fl       L_802E5068
    if (!c1cs) {
        // 0x802E5054: lwc1        $f0, 0x34($s0)
        ctx->f0.u32l = MEM_W(ctx->r16, 0X34);
            goto L_802E5068;
    }
    goto skip_1;
    // 0x802E5054: lwc1        $f0, 0x34($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X34);
    skip_1:
    // 0x802E5058: lwc1        $f6, 0x34($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X34);
    // 0x802E505C: add.s       $f8, $f6, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f6.fl + ctx->f12.fl;
    // 0x802E5060: swc1        $f8, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->f8.u32l;
    // 0x802E5064: lwc1        $f0, 0x34($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X34);
L_802E5068:
    // 0x802E5068: lwc1        $f10, 0x30($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X30);
    // 0x802E506C: c.lt.s      $f10, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f10.fl < ctx->f0.fl;
    // 0x802E5070: nop

    // 0x802E5074: bc1fl       L_802E5174
    if (!c1cs) {
        // 0x802E5078: lh          $t6, 0x3C($s0)
        ctx->r14 = MEM_H(ctx->r16, 0X3C);
            goto L_802E5174;
    }
    goto skip_2;
    // 0x802E5078: lh          $t6, 0x3C($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X3C);
    skip_2:
    // 0x802E507C: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x802E5080: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x802E5084: addiu       $t0, $s1, 0x50
    ctx->r8 = ADD32(ctx->r17, 0X50);
    // 0x802E5088: jalr        $t9
    // 0x802E508C: sw          $t0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E508C: sw          $t0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r8;
    after_4:
    // 0x802E5090: beq         $v0, $zero, L_802E50C4
    if (ctx->r2 == 0) {
        // 0x802E5094: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_802E50C4;
    }
    // 0x802E5094: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E5098: addiu       $t9, $t9, 0x6EBC
    ctx->r25 = ADD32(ctx->r25, 0X6EBC);
    // 0x802E509C: jalr        $t9
    // 0x802E50A0: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E50A0: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    after_5:
    // 0x802E50A4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x802E50A8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E50AC: addiu       $t9, $t9, 0x6220
    ctx->r25 = ADD32(ctx->r25, 0X6220);
    // 0x802E50B0: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x802E50B4: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x802E50B8: sw          $zero, 0x14($s0)
    MEM_W(0X14, ctx->r16) = 0;
    // 0x802E50BC: jalr        $t9
    // 0x802E50C0: addiu       $a0, $zero, 0x29E
    ctx->r4 = ADD32(0, 0X29E);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E50C0: addiu       $a0, $zero, 0x29E
    ctx->r4 = ADD32(0, 0X29E);
    after_6:
L_802E50C4:
    // 0x802E50C4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E50C8: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x802E50CC: addiu       $a0, $zero, 0x27D
    ctx->r4 = ADD32(0, 0X27D);
    // 0x802E50D0: jalr        $t9
    // 0x802E50D4: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x802E50D4: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    after_7:
    // 0x802E50D8: lh          $t1, 0x0($s2)
    ctx->r9 = MEM_H(ctx->r18, 0X0);
    // 0x802E50DC: lui         $t9, 0x8015
    ctx->r25 = S32(0X8015 << 16);
    // 0x802E50E0: addiu       $t9, $t9, -0x7BEC
    ctx->r25 = ADD32(ctx->r25, -0X7BEC);
    // 0x802E50E4: andi        $t2, $t1, 0xF7FF
    ctx->r10 = ctx->r9 & 0XF7FF;
    // 0x802E50E8: sh          $t2, 0x0($s2)
    MEM_H(0X0, ctx->r18) = ctx->r10;
    // 0x802E50EC: lwc1        $f4, 0x30($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X30);
    // 0x802E50F0: lwc1        $f18, 0x24($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X24);
    // 0x802E50F4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E50F8: swc1        $f4, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->f4.u32l;
    // 0x802E50FC: swc1        $f18, 0x50($s1)
    MEM_W(0X50, ctx->r17) = ctx->f18.u32l;
    // 0x802E5100: lh          $t3, 0x3C($s0)
    ctx->r11 = MEM_H(ctx->r16, 0X3C);
    // 0x802E5104: lwc1        $f16, 0x34($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X34);
    // 0x802E5108: lwc1        $f4, 0x28($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X28);
    // 0x802E510C: mtc1        $t3, $f6
    ctx->f6.u32l = ctx->r11;
    // 0x802E5110: addiu       $a1, $s0, 0x18
    ctx->r5 = ADD32(ctx->r16, 0X18);
    // 0x802E5114: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x802E5118: mul.s       $f10, $f16, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f16.fl, ctx->f8.fl);
    // 0x802E511C: add.s       $f18, $f4, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x802E5120: swc1        $f18, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->f18.u32l;
    // 0x802E5124: lwc1        $f6, 0x2C($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X2C);
    // 0x802E5128: jalr        $t9
    // 0x802E512C: swc1        $f6, 0x58($s1)
    MEM_W(0X58, ctx->r17) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x802E512C: swc1        $f6, 0x58($s1)
    MEM_W(0X58, ctx->r17) = ctx->f6.u32l;
    after_8:
    // 0x802E5130: lw          $t4, 0x4C($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X4C);
    // 0x802E5134: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E5138: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E513C: lhu         $t5, 0x18($t4)
    ctx->r13 = MEM_HU(ctx->r12, 0X18);
    // 0x802E5140: addiu       $t9, $t9, 0x76FC
    ctx->r25 = ADD32(ctx->r25, 0X76FC);
    // 0x802E5144: bne         $t5, $at, L_802E5154
    if (ctx->r13 != ctx->r1) {
        // 0x802E5148: nop
    
            goto L_802E5154;
    }
    // 0x802E5148: nop

    // 0x802E514C: jalr        $t9
    // 0x802E5150: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x802E5150: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    after_9:
L_802E5154:
    // 0x802E5154: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5158: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E515C: addiu       $a0, $s2, 0x8
    ctx->r4 = ADD32(ctx->r18, 0X8);
    // 0x802E5160: jalr        $t9
    // 0x802E5164: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x802E5164: addiu       $a1, $s2, 0xE
    ctx->r5 = ADD32(ctx->r18, 0XE);
    after_10:
    // 0x802E5168: b           L_802E5194
    // 0x802E516C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_802E5194;
    // 0x802E516C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x802E5170: lh          $t6, 0x3C($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X3C);
L_802E5174:
    // 0x802E5174: lwc1        $f16, 0x28($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X28);
    // 0x802E5178: mtc1        $t6, $f8
    ctx->f8.u32l = ctx->r14;
    // 0x802E517C: nop

    // 0x802E5180: cvt.s.w     $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E5184: mul.s       $f10, $f0, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f4.fl);
    // 0x802E5188: add.s       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f18.fl = ctx->f16.fl + ctx->f10.fl;
    // 0x802E518C: swc1        $f18, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->f18.u32l;
    // 0x802E5190: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_802E5194:
    // 0x802E5194: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E5198: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x802E519C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x802E51A0: jr          $ra
    // 0x802E51A4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x802E51A4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void map_ovl_00_func_802E51A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E51A8: jr          $ra
    // 0x802E51AC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x802E51AC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void map_ovl_00_func_802E51B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E51B0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x802E51B4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E51B8: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x802E51BC: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x802E51C0: jal         0x801469F4
    // 0x802E51C4: sw          $t6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r14;
    func_801469F4(rdram, ctx);
        goto after_0;
    // 0x802E51C4: sw          $t6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r14;
    after_0:
    // 0x802E51C8: lw          $v0, 0x24($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X24);
    // 0x802E51CC: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x802E51D0: lw          $a0, 0x14($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X14);
    // 0x802E51D4: jal         0x8001717C
    // 0x802E51D8: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    func_8001717C(rdram, ctx);
        goto after_1;
    // 0x802E51D8: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    after_1:
    // 0x802E51DC: beq         $v0, $zero, L_802E51EC
    if (ctx->r2 == 0) {
        // 0x802E51E0: lw          $t7, 0x18($sp)
        ctx->r15 = MEM_W(ctx->r29, 0X18);
            goto L_802E51EC;
    }
    // 0x802E51E0: lw          $t7, 0x18($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X18);
    // 0x802E51E4: jal         0x80016EBC
    // 0x802E51E8: lw          $a0, 0x14($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X14);
    func_80016EBC(rdram, ctx);
        goto after_2;
    // 0x802E51E8: lw          $a0, 0x14($t7)
    ctx->r4 = MEM_W(ctx->r15, 0X14);
    after_2:
L_802E51EC:
    // 0x802E51EC: lw          $t8, 0x18($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X18);
    // 0x802E51F0: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x802E51F4: lw          $t9, 0x10($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X10);
    // 0x802E51F8: jalr        $t9
    // 0x802E51FC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E51FC: nop

    after_3:
    // 0x802E5200: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E5204: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x802E5208: jr          $ra
    // 0x802E520C: nop

    return;
    // 0x802E520C: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5210(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5210: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E5214: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E5218: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x802E521C: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x802E5220: lui         $t9, 0x8015
    ctx->r25 = S32(0X8015 << 16);
    // 0x802E5224: addiu       $t9, $t9, -0x7DE8
    ctx->r25 = ADD32(ctx->r25, -0X7DE8);
    // 0x802E5228: sw          $t6, 0x48($v0)
    MEM_W(0X48, ctx->r2) = ctx->r14;
    // 0x802E522C: lw          $a0, 0x24($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X24);
    // 0x802E5230: jalr        $t9
    // 0x802E5234: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5234: addiu       $a2, $v0, 0x20
    ctx->r6 = ADD32(ctx->r2, 0X20);
    after_0:
    // 0x802E5238: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E523C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E5240: jr          $ra
    // 0x802E5244: nop

    return;
    // 0x802E5244: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5248(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5248: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E524C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E5250: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x802E5254: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E5258: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E525C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E5260: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x802E5264: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x802E5268: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x802E526C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x802E5270: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x802E5274: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x802E5278: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E527C: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x802E5280: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x802E5284: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E5288: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x802E528C: lw          $t9, -0x4640($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X4640);
    // 0x802E5290: jalr        $t9
    // 0x802E5294: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5294: nop

    after_0:
    // 0x802E5298: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E529C: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x802E52A0: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x802E52A4: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x802E52A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E52AC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E52B0: jr          $ra
    // 0x802E52B4: nop

    return;
    // 0x802E52B4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E52B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E52B8: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x802E52BC: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x802E52C0: lui         $v0, 0x8000
    ctx->r2 = S32(0X8000 << 16);
    // 0x802E52C4: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x802E52C8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E52CC: addiu       $v0, $v0, 0x5A30
    ctx->r2 = ADD32(ctx->r2, 0X5A30);
    // 0x802E52D0: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x802E52D4: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x802E52D8: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x802E52DC: sw          $v0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r2;
    // 0x802E52E0: jalr        $v0
    // 0x802E52E4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_0;
    // 0x802E52E4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_0:
    // 0x802E52E8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E52EC: addiu       $t9, $t9, 0x31CC
    ctx->r25 = ADD32(ctx->r25, 0X31CC);
    // 0x802E52F0: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x802E52F4: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x802E52F8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E52FC: jalr        $t9
    // 0x802E5300: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5300: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    after_1:
    // 0x802E5304: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x802E5308: lw          $a1, -0x522C($t6)
    ctx->r5 = MEM_W(ctx->r14, -0X522C);
    // 0x802E530C: lhu         $t8, 0x2($s0)
    ctx->r24 = MEM_HU(ctx->r16, 0X2);
    // 0x802E5310: lui         $a0, 0x603
    ctx->r4 = S32(0X603 << 16);
    // 0x802E5314: addiu       $a0, $a0, -0x1CA8
    ctx->r4 = ADD32(ctx->r4, -0X1CA8);
    // 0x802E5318: addiu       $t7, $zero, -0x4000
    ctx->r15 = ADD32(0, -0X4000);
    // 0x802E531C: lui         $v0, 0x8014
    ctx->r2 = S32(0X8014 << 16);
    // 0x802E5320: ori         $t0, $t8, 0x80
    ctx->r8 = ctx->r24 | 0X80;
    // 0x802E5324: sw          $a0, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r4;
    // 0x802E5328: sh          $t7, 0x5C($s0)
    MEM_H(0X5C, ctx->r16) = ctx->r15;
    // 0x802E532C: sh          $t0, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r8;
    // 0x802E5330: addiu       $v0, $v0, 0x6504
    ctx->r2 = ADD32(ctx->r2, 0X6504);
    // 0x802E5334: sw          $a1, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r5;
    // 0x802E5338: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x802E533C: jalr        $v0
    // 0x802E5340: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_2;
    // 0x802E5340: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_2:
    // 0x802E5344: sw          $v0, 0x68($s1)
    MEM_W(0X68, ctx->r17) = ctx->r2;
    // 0x802E5348: lw          $t9, 0x24($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X24);
    // 0x802E534C: lw          $a1, 0x40($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X40);
    // 0x802E5350: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    // 0x802E5354: jalr        $t9
    // 0x802E5358: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E5358: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_3:
    // 0x802E535C: sw          $v0, 0x74($s0)
    MEM_W(0X74, ctx->r16) = ctx->r2;
    // 0x802E5360: jal         0x80146900
    // 0x802E5364: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80146900(rdram, ctx);
        goto after_4;
    // 0x802E5364: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_4:
    // 0x802E5368: lw          $t9, 0x28($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X28);
    // 0x802E536C: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x802E5370: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x802E5374: jalr        $t9
    // 0x802E5378: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E5378: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_5:
    // 0x802E537C: sw          $v0, 0x28($s1)
    MEM_W(0X28, ctx->r17) = ctx->r2;
    // 0x802E5380: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x802E5384: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x802E5388: lw          $t1, -0x522C($t1)
    ctx->r9 = MEM_W(ctx->r9, -0X522C);
    // 0x802E538C: lui         $a0, 0x603
    ctx->r4 = S32(0X603 << 16);
    // 0x802E5390: addiu       $a0, $a0, -0x36D0
    ctx->r4 = ADD32(ctx->r4, -0X36D0);
    // 0x802E5394: ori         $t3, $t2, 0x80
    ctx->r11 = ctx->r10 | 0X80;
    // 0x802E5398: sw          $a0, 0x3C($v0)
    MEM_W(0X3C, ctx->r2) = ctx->r4;
    // 0x802E539C: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x802E53A0: sw          $t1, 0x40($v0)
    MEM_W(0X40, ctx->r2) = ctx->r9;
    // 0x802E53A4: lw          $t9, 0x24($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X24);
    // 0x802E53A8: lw          $a1, 0x40($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X40);
    // 0x802E53AC: sw          $v0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r2;
    // 0x802E53B0: jalr        $t9
    // 0x802E53B4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E53B4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_6:
    // 0x802E53B8: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    // 0x802E53BC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E53C0: jal         0x802E5AD8
    // 0x802E53C4: sw          $v0, 0x74($a1)
    MEM_W(0X74, ctx->r5) = ctx->r2;
    map_ovl_00_func_802E5AD8(rdram, ctx);
        goto after_7;
    // 0x802E53C4: sw          $v0, 0x74($a1)
    MEM_W(0X74, ctx->r5) = ctx->r2;
    after_7:
    // 0x802E53C8: addiu       $v0, $s1, 0x34
    ctx->r2 = ADD32(ctx->r17, 0X34);
    // 0x802E53CC: sw          $zero, 0xC($v0)
    MEM_W(0XC, ctx->r2) = 0;
    // 0x802E53D0: sw          $zero, 0x8($v0)
    MEM_W(0X8, ctx->r2) = 0;
    // 0x802E53D4: sw          $zero, 0x4($v0)
    MEM_W(0X4, ctx->r2) = 0;
    // 0x802E53D8: lw          $t5, 0x10($s1)
    ctx->r13 = MEM_W(ctx->r17, 0X10);
    // 0x802E53DC: addiu       $t4, $zero, 0x4
    ctx->r12 = ADD32(0, 0X4);
    // 0x802E53E0: sw          $t4, 0x64($s1)
    MEM_W(0X64, ctx->r17) = ctx->r12;
    // 0x802E53E4: lui         $t6, 0x802E
    ctx->r14 = S32(0X802E << 16);
    // 0x802E53E8: addiu       $t6, $t6, 0x5AA8
    ctx->r14 = ADD32(ctx->r14, 0X5AA8);
    // 0x802E53EC: sw          $t5, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r13;
    // 0x802E53F0: sw          $t6, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r14;
    // 0x802E53F4: jal         0x80143234
    // 0x802E53F8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80143234(rdram, ctx);
        goto after_8;
    // 0x802E53F8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_8:
    // 0x802E53FC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5400: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E5404: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x802E5408: jalr        $t9
    // 0x802E540C: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x802E540C: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_9:
    // 0x802E5410: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x802E5414: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x802E5418: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x802E541C: jr          $ra
    // 0x802E5420: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x802E5420: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void map_ovl_00_func_802E5424(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5424: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x802E5428: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E542C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E5430: lw          $t6, 0x28($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X28);
    // 0x802E5434: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E5438: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x802E543C: sw          $t6, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r14;
    // 0x802E5440: lw          $a2, 0x24($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X24);
    // 0x802E5444: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E5448: addiu       $a1, $a2, 0x50
    ctx->r5 = ADD32(ctx->r6, 0X50);
    // 0x802E544C: jalr        $t9
    // 0x802E5450: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5450: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_0:
    // 0x802E5454: beq         $v0, $zero, L_802E5474
    if (ctx->r2 == 0) {
        // 0x802E5458: lw          $a2, 0x20($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X20);
            goto L_802E5474;
    }
    // 0x802E5458: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x802E545C: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x802E5460: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E5464: jalr        $t9
    // 0x802E5468: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5468: nop

    after_1:
    // 0x802E546C: b           L_802E54D8
    // 0x802E5470: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E54D8;
    // 0x802E5470: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E5474:
    // 0x802E5474: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x802E5478: jal         0x802E5AD8
    // 0x802E547C: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    map_ovl_00_func_802E5AD8(rdram, ctx);
        goto after_2;
    // 0x802E547C: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    after_2:
    // 0x802E5480: lh          $v1, 0xE($s0)
    ctx->r3 = MEM_H(ctx->r16, 0XE);
    // 0x802E5484: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E5488: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E548C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E5490: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E5494: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E5498: sll         $t7, $v1, 1
    ctx->r15 = S32(ctx->r3 << 1);
    // 0x802E549C: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x802E54A0: sh          $v1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r3;
    // 0x802E54A4: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x802E54A8: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x802E54AC: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x802E54B0: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x802E54B4: sb          $t0, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r8;
    // 0x802E54B8: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
    // 0x802E54BC: lw          $t9, -0x4638($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X4638);
    // 0x802E54C0: jalr        $t9
    // 0x802E54C4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E54C4: nop

    after_3:
    // 0x802E54C8: lh          $t3, 0xE($s0)
    ctx->r11 = MEM_H(ctx->r16, 0XE);
    // 0x802E54CC: addiu       $t4, $t3, -0x1
    ctx->r12 = ADD32(ctx->r11, -0X1);
    // 0x802E54D0: sh          $t4, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r12;
    // 0x802E54D4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E54D8:
    // 0x802E54D8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E54DC: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x802E54E0: jr          $ra
    // 0x802E54E4: nop

    return;
    // 0x802E54E4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E54E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E54E8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E54EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E54F0: lw          $t6, 0x64($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X64);
    // 0x802E54F4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x802E54F8: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E54FC: beq         $t6, $at, L_802E5524
    if (ctx->r14 == ctx->r1) {
        // 0x802E5500: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_802E5524;
    }
    // 0x802E5500: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E5504: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5508: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E550C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E5510: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    // 0x802E5514: jalr        $t9
    // 0x802E5518: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5518: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x802E551C: beq         $v0, $zero, L_802E5538
    if (ctx->r2 == 0) {
        // 0x802E5520: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_802E5538;
    }
    // 0x802E5520: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
L_802E5524:
    // 0x802E5524: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5528: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E552C: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x802E5530: jalr        $t9
    // 0x802E5534: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5534: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_802E5538:
    // 0x802E5538: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E553C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E5540: jr          $ra
    // 0x802E5544: nop

    return;
    // 0x802E5544: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5548(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5548: addiu       $sp, $sp, -0xB8
    ctx->r29 = ADD32(ctx->r29, -0XB8);
    // 0x802E554C: sw          $ra, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r31;
    // 0x802E5550: sw          $fp, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r30;
    // 0x802E5554: sw          $s7, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r23;
    // 0x802E5558: sw          $s6, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r22;
    // 0x802E555C: sw          $s5, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r21;
    // 0x802E5560: sw          $s4, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r20;
    // 0x802E5564: sw          $s3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r19;
    // 0x802E5568: sw          $s2, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r18;
    // 0x802E556C: sw          $s1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r17;
    // 0x802E5570: sw          $s0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r16;
    // 0x802E5574: sdc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X40, ctx->r29);
    // 0x802E5578: sdc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X38, ctx->r29);
    // 0x802E557C: sdc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X30, ctx->r29);
    // 0x802E5580: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x802E5584: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x802E5588: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x802E558C: lw          $t6, 0x38($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X38);
    // 0x802E5590: lw          $t7, 0x3C($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X3C);
    // 0x802E5594: lui         $at, 0x40
    ctx->r1 = S32(0X40 << 16);
    // 0x802E5598: ori         $at, $at, 0x1
    ctx->r1 = ctx->r1 | 0X1;
    // 0x802E559C: addu        $t8, $t6, $t7
    ctx->r24 = ADD32(ctx->r14, ctx->r15);
    // 0x802E55A0: slt         $at, $t8, $at
    ctx->r1 = SIGNED(ctx->r24) < SIGNED(ctx->r1) ? 1 : 0;
    // 0x802E55A4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E55A8: lw          $s2, 0x24($a0)
    ctx->r18 = MEM_W(ctx->r4, 0X24);
    // 0x802E55AC: beq         $at, $zero, L_802E55D4
    if (ctx->r1 == 0) {
        // 0x802E55B0: sw          $t8, 0x38($a0)
        MEM_W(0X38, ctx->r4) = ctx->r24;
            goto L_802E55D4;
    }
    // 0x802E55B0: sw          $t8, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->r24;
    // 0x802E55B4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E55B8: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E55BC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E55C0: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E55C4: jalr        $t9
    // 0x802E55C8: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E55C8: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    after_0:
    // 0x802E55CC: beq         $v0, $zero, L_802E5628
    if (ctx->r2 == 0) {
        // 0x802E55D0: lui         $at, 0x801D
        ctx->r1 = S32(0X801D << 16);
            goto L_802E5628;
    }
    // 0x802E55D0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
L_802E55D4:
    // 0x802E55D4: addiu       $s1, $s0, 0x34
    ctx->r17 = ADD32(ctx->r16, 0X34);
    // 0x802E55D8: lui         $t0, 0x40
    ctx->r8 = S32(0X40 << 16);
    // 0x802E55DC: sw          $t0, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r8;
    // 0x802E55E0: sw          $zero, 0xC($s1)
    MEM_W(0XC, ctx->r17) = 0;
    // 0x802E55E4: lh          $t1, 0x0($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X0);
    // 0x802E55E8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E55EC: addiu       $t9, $t9, 0x6504
    ctx->r25 = ADD32(ctx->r25, 0X6504);
    // 0x802E55F0: ori         $t2, $t1, 0x1000
    ctx->r10 = ctx->r9 | 0X1000;
    // 0x802E55F4: sh          $t2, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r10;
    // 0x802E55F8: lw          $a1, 0x40($s2)
    ctx->r5 = MEM_W(ctx->r18, 0X40);
    // 0x802E55FC: lw          $a0, 0x3C($s2)
    ctx->r4 = MEM_W(ctx->r18, 0X3C);
    // 0x802E5600: jalr        $t9
    // 0x802E5604: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5604: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_1:
    // 0x802E5608: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E560C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E5610: sw          $v0, 0x74($s2)
    MEM_W(0X74, ctx->r18) = ctx->r2;
    // 0x802E5614: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E5618: jalr        $t9
    // 0x802E561C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E561C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x802E5620: b           L_802E5694
    // 0x802E5624: lw          $t6, 0x4($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X4);
        goto L_802E5694;
    // 0x802E5624: lw          $t6, 0x4($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X4);
L_802E5628:
    // 0x802E5628: lwc1        $f0, -0x7D38($at)
    ctx->f0.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x802E562C: lui         $at, 0x4A80
    ctx->r1 = S32(0X4A80 << 16);
    // 0x802E5630: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x802E5634: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x802E5638: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x802E563C: addiu       $s1, $s0, 0x34
    ctx->r17 = ADD32(ctx->r16, 0X34);
    // 0x802E5640: lw          $t3, 0x8($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X8);
    // 0x802E5644: mul.s       $f10, $f8, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f0.fl);
    // 0x802E5648: lui         $at, 0x40A0
    ctx->r1 = S32(0X40A0 << 16);
    // 0x802E564C: mtc1        $t3, $f4
    ctx->f4.u32l = ctx->r11;
    // 0x802E5650: nop

    // 0x802E5654: cvt.s.w     $f12, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    ctx->f12.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E5658: div.s       $f2, $f6, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f10.fl);
    // 0x802E565C: c.lt.s      $f12, $f2
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    c1cs = ctx->f12.fl < ctx->f2.fl;
    // 0x802E5660: nop

    // 0x802E5664: bc1fl       L_802E5694
    if (!c1cs) {
        // 0x802E5668: lw          $t6, 0x4($s1)
        ctx->r14 = MEM_W(ctx->r17, 0X4);
            goto L_802E5694;
    }
    goto skip_0;
    // 0x802E5668: lw          $t6, 0x4($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X4);
    skip_0:
    // 0x802E566C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x802E5670: nop

    // 0x802E5674: mul.s       $f18, $f0, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f16.fl);
    // 0x802E5678: div.s       $f4, $f2, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = DIV_S(ctx->f2.fl, ctx->f18.fl);
    // 0x802E567C: add.s       $f8, $f12, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f12.fl + ctx->f4.fl;
    // 0x802E5680: trunc.w.s   $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    ctx->f6.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x802E5684: mfc1        $t5, $f6
    ctx->r13 = (int32_t)ctx->f6.u32l;
    // 0x802E5688: nop

    // 0x802E568C: sw          $t5, 0x8($s1)
    MEM_W(0X8, ctx->r17) = ctx->r13;
    // 0x802E5690: lw          $t6, 0x4($s1)
    ctx->r14 = MEM_W(ctx->r17, 0X4);
L_802E5694:
    // 0x802E5694: ori         $at, $zero, 0xC000
    ctx->r1 = 0 | 0XC000;
    // 0x802E5698: lui         $fp, 0x8006
    ctx->r30 = S32(0X8006 << 16);
    // 0x802E569C: sra         $t7, $t6, 8
    ctx->r15 = S32(SIGNED(ctx->r14) >> 8);
    // 0x802E56A0: addu        $t8, $t7, $at
    ctx->r24 = ADD32(ctx->r15, ctx->r1);
    // 0x802E56A4: sh          $t8, 0x5C($s2)
    MEM_H(0X5C, ctx->r18) = ctx->r24;
    // 0x802E56A8: lw          $t0, 0xC($s1)
    ctx->r8 = MEM_W(ctx->r17, 0XC);
    // 0x802E56AC: addiu       $fp, $fp, -0x6B48
    ctx->r30 = ADD32(ctx->r30, -0X6B48);
    // 0x802E56B0: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x802E56B4: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x802E56B8: andi        $t9, $t1, 0xF
    ctx->r25 = ctx->r9 & 0XF;
    // 0x802E56BC: bne         $t9, $zero, L_802E57AC
    if (ctx->r25 != 0) {
        // 0x802E56C0: sw          $t1, 0xC($s1)
        MEM_W(0XC, ctx->r17) = ctx->r9;
            goto L_802E57AC;
    }
    // 0x802E56C0: sw          $t1, 0xC($s1)
    MEM_W(0XC, ctx->r17) = ctx->r9;
    // 0x802E56C4: lui         $at, 0xC2A0
    ctx->r1 = S32(0XC2A0 << 16);
    // 0x802E56C8: mtc1        $at, $f30
    ctx->f30.u32l = ctx->r1;
    // 0x802E56CC: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E56D0: lwc1        $f28, -0x3DD0($at)
    ctx->f28.u32l = MEM_W(ctx->r1, -0X3DD0);
    // 0x802E56D4: lui         $at, 0x434D
    ctx->r1 = S32(0X434D << 16);
    // 0x802E56D8: mtc1        $at, $f26
    ctx->f26.u32l = ctx->r1;
    // 0x802E56DC: lui         $at, 0x4270
    ctx->r1 = S32(0X4270 << 16);
    // 0x802E56E0: mtc1        $at, $f24
    ctx->f24.u32l = ctx->r1;
    // 0x802E56E4: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E56E8: ldc1        $f22, -0x3DC8($at)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r1, -0X3DC8);
    // 0x802E56EC: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E56F0: lui         $s5, 0x8000
    ctx->r21 = S32(0X8000 << 16);
    // 0x802E56F4: mtc1        $at, $f21
    ctx->f_odd[(21 - 1) * 2] = ctx->r1;
    // 0x802E56F8: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x802E56FC: addiu       $s5, $s5, 0x47C0
    ctx->r21 = ADD32(ctx->r21, 0X47C0);
    // 0x802E5700: addiu       $s7, $zero, 0x2
    ctx->r23 = ADD32(0, 0X2);
    // 0x802E5704: addiu       $s6, $sp, 0x8C
    ctx->r22 = ADD32(ctx->r29, 0X8C);
    // 0x802E5708: lui         $s4, 0x2000
    ctx->r20 = S32(0X2000 << 16);
    // 0x802E570C: addiu       $s3, $sp, 0x98
    ctx->r19 = ADD32(ctx->r29, 0X98);
L_802E5710:
    // 0x802E5710: beq         $s0, $zero, L_802E5720
    if (ctx->r16 == 0) {
        // 0x802E5714: addiu       $a0, $zero, 0x1E
        ctx->r4 = ADD32(0, 0X1E);
            goto L_802E5720;
    }
    // 0x802E5714: addiu       $a0, $zero, 0x1E
    ctx->r4 = ADD32(0, 0X1E);
    // 0x802E5718: b           L_802E5724
    // 0x802E571C: swc1        $f24, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->f24.u32l;
        goto L_802E5724;
    // 0x802E571C: swc1        $f24, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->f24.u32l;
L_802E5720:
    // 0x802E5720: swc1        $f30, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->f30.u32l;
L_802E5724:
    // 0x802E5724: swc1        $f26, 0x9C($sp)
    MEM_W(0X9C, ctx->r29) = ctx->f26.u32l;
    // 0x802E5728: swc1        $f28, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->f28.u32l;
    // 0x802E572C: addiu       $a1, $zero, 0x1E
    ctx->r5 = ADD32(0, 0X1E);
    // 0x802E5730: or          $a2, $s3, $zero
    ctx->r6 = ctx->r19 | 0;
    // 0x802E5734: jalr        $fp
    // 0x802E5738: or          $a3, $s4, $zero
    ctx->r7 = ctx->r20 | 0;
    LOOKUP_FUNC(ctx->r30)(rdram, ctx);
        goto after_3;
    // 0x802E5738: or          $a3, $s4, $zero
    ctx->r7 = ctx->r20 | 0;
    after_3:
    // 0x802E573C: beq         $v0, $zero, L_802E57A0
    if (ctx->r2 == 0) {
        // 0x802E5740: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_802E57A0;
    }
    // 0x802E5740: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x802E5744: lui         $s2, 0x8006
    ctx->r18 = S32(0X8006 << 16);
    // 0x802E5748: addiu       $s2, $s2, -0x3768
    ctx->r18 = ADD32(ctx->r18, -0X3768);
    // 0x802E574C: jalr        $s5
    // 0x802E5750: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_4;
    // 0x802E5750: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    after_4:
    // 0x802E5754: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x802E5758: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E575C: bgez        $v0, L_802E5770
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E5760: cvt.s.w     $f16, $f10
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
            goto L_802E5770;
    }
    // 0x802E5760: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E5764: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x802E5768: nop

    // 0x802E576C: add.s       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f16.fl = ctx->f16.fl + ctx->f18.fl;
L_802E5770:
    // 0x802E5770: cvt.d.s     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f4.d = CVT_D_S(ctx->f16.fl);
    // 0x802E5774: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E5778: mul.d       $f8, $f4, $f22
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f22.d); 
    ctx->f8.d = MUL_D(ctx->f4.d, ctx->f22.d);
    // 0x802E577C: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    // 0x802E5780: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    // 0x802E5784: mul.d       $f6, $f8, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f20.d); 
    ctx->f6.d = MUL_D(ctx->f8.d, ctx->f20.d);
    // 0x802E5788: add.d       $f10, $f6, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f20.d); 
    ctx->f10.d = ctx->f6.d + ctx->f20.d;
    // 0x802E578C: cvt.s.d     $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f0.fl = CVT_S_D(ctx->f10.d);
    // 0x802E5790: swc1        $f0, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->f0.u32l;
    // 0x802E5794: swc1        $f0, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->f0.u32l;
    // 0x802E5798: jalr        $s2
    // 0x802E579C: swc1        $f0, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_5;
    // 0x802E579C: swc1        $f0, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->f0.u32l;
    after_5:
L_802E57A0:
    // 0x802E57A0: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    // 0x802E57A4: bne         $s0, $s7, L_802E5710
    if (ctx->r16 != ctx->r23) {
        // 0x802E57A8: nop
    
            goto L_802E5710;
    }
    // 0x802E57A8: nop

L_802E57AC:
    // 0x802E57AC: lw          $ra, 0x6C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X6C);
    // 0x802E57B0: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x802E57B4: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x802E57B8: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x802E57BC: ldc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X30);
    // 0x802E57C0: ldc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X38);
    // 0x802E57C4: ldc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X40);
    // 0x802E57C8: lw          $s0, 0x48($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X48);
    // 0x802E57CC: lw          $s1, 0x4C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X4C);
    // 0x802E57D0: lw          $s2, 0x50($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X50);
    // 0x802E57D4: lw          $s3, 0x54($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X54);
    // 0x802E57D8: lw          $s4, 0x58($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X58);
    // 0x802E57DC: lw          $s5, 0x5C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X5C);
    // 0x802E57E0: lw          $s6, 0x60($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X60);
    // 0x802E57E4: lw          $s7, 0x64($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X64);
    // 0x802E57E8: lw          $fp, 0x68($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X68);
    // 0x802E57EC: jr          $ra
    // 0x802E57F0: addiu       $sp, $sp, 0xB8
    ctx->r29 = ADD32(ctx->r29, 0XB8);
    return;
    // 0x802E57F0: addiu       $sp, $sp, 0xB8
    ctx->r29 = ADD32(ctx->r29, 0XB8);
;}
RECOMP_FUNC void map_ovl_00_func_802E57F4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E57F4: addiu       $sp, $sp, -0xB8
    ctx->r29 = ADD32(ctx->r29, -0XB8);
    // 0x802E57F8: sw          $ra, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r31;
    // 0x802E57FC: sw          $fp, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r30;
    // 0x802E5800: sw          $s7, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r23;
    // 0x802E5804: sw          $s6, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r22;
    // 0x802E5808: sw          $s5, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r21;
    // 0x802E580C: sw          $s4, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r20;
    // 0x802E5810: sw          $s3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r19;
    // 0x802E5814: sw          $s2, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r18;
    // 0x802E5818: sw          $s1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r17;
    // 0x802E581C: sw          $s0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r16;
    // 0x802E5820: sdc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    SD(ctx->f30.u64, 0X40, ctx->r29);
    // 0x802E5824: sdc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X38, ctx->r29);
    // 0x802E5828: sdc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X30, ctx->r29);
    // 0x802E582C: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x802E5830: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x802E5834: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x802E5838: lw          $t6, 0x40($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X40);
    // 0x802E583C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x802E5840: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E5844: addiu       $t8, $t6, 0x1
    ctx->r24 = ADD32(ctx->r14, 0X1);
    // 0x802E5848: sw          $t8, 0x40($a0)
    MEM_W(0X40, ctx->r4) = ctx->r24;
    // 0x802E584C: lwc1        $f4, -0x7D38($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X7D38);
    // 0x802E5850: lui         $at, 0x4040
    ctx->r1 = S32(0X4040 << 16);
    // 0x802E5854: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x802E5858: mtc1        $t8, $f10
    ctx->f10.u32l = ctx->r24;
    // 0x802E585C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E5860: div.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x802E5864: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5868: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E586C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E5870: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E5874: c.lt.s      $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f8.fl < ctx->f16.fl;
    // 0x802E5878: nop

    // 0x802E587C: bc1t        L_802E5894
    if (c1cs) {
        // 0x802E5880: nop
    
            goto L_802E5894;
    }
    // 0x802E5880: nop

    // 0x802E5884: jalr        $t9
    // 0x802E5888: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5888: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    after_0:
    // 0x802E588C: beq         $v0, $zero, L_802E58B0
    if (ctx->r2 == 0) {
        // 0x802E5890: or          $s3, $zero, $zero
        ctx->r19 = 0 | 0;
            goto L_802E58B0;
    }
    // 0x802E5890: or          $s3, $zero, $zero
    ctx->r19 = 0 | 0;
L_802E5894:
    // 0x802E5894: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5898: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E589C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E58A0: jalr        $t9
    // 0x802E58A4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E58A4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_1:
    // 0x802E58A8: b           L_802E5A5C
    // 0x802E58AC: lw          $ra, 0x6C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X6C);
        goto L_802E5A5C;
    // 0x802E58AC: lw          $ra, 0x6C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X6C);
L_802E58B0:
    // 0x802E58B0: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x802E58B4: mtc1        $at, $f31
    ctx->f_odd[(31 - 1) * 2] = ctx->r1;
    // 0x802E58B8: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E58BC: ldc1        $f28, -0x3DC0($at)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r1, -0X3DC0);
    // 0x802E58C0: lui         $at, 0x4034
    ctx->r1 = S32(0X4034 << 16);
    // 0x802E58C4: mtc1        $at, $f27
    ctx->f_odd[(27 - 1) * 2] = ctx->r1;
    // 0x802E58C8: lui         $at, 0xC04E
    ctx->r1 = S32(0XC04E << 16);
    // 0x802E58CC: mtc1        $at, $f25
    ctx->f_odd[(25 - 1) * 2] = ctx->r1;
    // 0x802E58D0: lui         $at, 0x405E
    ctx->r1 = S32(0X405E << 16);
    // 0x802E58D4: mtc1        $at, $f23
    ctx->f_odd[(23 - 1) * 2] = ctx->r1;
    // 0x802E58D8: lui         $t0, 0x8006
    ctx->r8 = S32(0X8006 << 16);
    // 0x802E58DC: addiu       $t0, $t0, -0x6B48
    ctx->r8 = ADD32(ctx->r8, -0X6B48);
    // 0x802E58E0: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E58E4: lui         $s4, 0x8000
    ctx->r20 = S32(0X8000 << 16);
    // 0x802E58E8: lui         $fp, 0x5748
    ctx->r30 = S32(0X5748 << 16);
    // 0x802E58EC: mtc1        $zero, $f30
    ctx->f30.u32l = 0;
    // 0x802E58F0: mtc1        $zero, $f26
    ctx->f26.u32l = 0;
    // 0x802E58F4: mtc1        $zero, $f24
    ctx->f24.u32l = 0;
    // 0x802E58F8: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x802E58FC: ori         $fp, $fp, 0x30FF
    ctx->r30 = ctx->r30 | 0X30FF;
    // 0x802E5900: addiu       $s4, $s4, 0x47C0
    ctx->r20 = ADD32(ctx->r20, 0X47C0);
    // 0x802E5904: ldc1        $f20, -0x3DB8($at)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r1, -0X3DB8);
    // 0x802E5908: sw          $t0, 0x7C($sp)
    MEM_W(0X7C, ctx->r29) = ctx->r8;
    // 0x802E590C: addiu       $s7, $sp, 0x8C
    ctx->r23 = ADD32(ctx->r29, 0X8C);
    // 0x802E5910: lui         $s6, 0x2000
    ctx->r22 = S32(0X2000 << 16);
    // 0x802E5914: addiu       $s5, $sp, 0x98
    ctx->r21 = ADD32(ctx->r29, 0X98);
L_802E5918:
    // 0x802E5918: jalr        $s4
    // 0x802E591C: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_2;
    // 0x802E591C: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    after_2:
    // 0x802E5920: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x802E5924: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E5928: bgez        $v0, L_802E593C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E592C: cvt.s.w     $f4, $f18
        CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
            goto L_802E593C;
    }
    // 0x802E592C: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E5930: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x802E5934: nop

    // 0x802E5938: add.s       $f4, $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f4.fl + ctx->f6.fl;
L_802E593C:
    // 0x802E593C: cvt.d.s     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f10.d = CVT_D_S(ctx->f4.fl);
    // 0x802E5940: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x802E5944: mul.d       $f8, $f10, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f20.d); 
    ctx->f8.d = MUL_D(ctx->f10.d, ctx->f20.d);
    // 0x802E5948: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x802E594C: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    // 0x802E5950: swc1        $f4, 0x9C($sp)
    MEM_W(0X9C, ctx->r29) = ctx->f4.u32l;
    // 0x802E5954: mul.d       $f16, $f8, $f22
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f22.d); 
    ctx->f16.d = MUL_D(ctx->f8.d, ctx->f22.d);
    // 0x802E5958: add.d       $f18, $f16, $f24
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f24.d); 
    ctx->f18.d = ctx->f16.d + ctx->f24.d;
    // 0x802E595C: cvt.s.d     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f6.fl = CVT_S_D(ctx->f18.d);
    // 0x802E5960: jalr        $s4
    // 0x802E5964: swc1        $f6, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->f6.u32l;
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_3;
    // 0x802E5964: swc1        $f6, 0x98($sp)
    MEM_W(0X98, ctx->r29) = ctx->f6.u32l;
    after_3:
    // 0x802E5968: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x802E596C: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E5970: bgez        $v0, L_802E5984
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E5974: cvt.s.w     $f8, $f10
        CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.fl = CVT_S_W(ctx->f10.u32l);
            goto L_802E5984;
    }
    // 0x802E5974: cvt.s.w     $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    ctx->f8.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E5978: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x802E597C: nop

    // 0x802E5980: add.s       $f8, $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f8.fl + ctx->f16.fl;
L_802E5984:
    // 0x802E5984: cvt.d.s     $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.d = CVT_D_S(ctx->f8.fl);
    // 0x802E5988: lw          $t9, 0x7C($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X7C);
    // 0x802E598C: mul.d       $f6, $f18, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f20.d); 
    ctx->f6.d = MUL_D(ctx->f18.d, ctx->f20.d);
    // 0x802E5990: addiu       $a0, $zero, 0x1E
    ctx->r4 = ADD32(0, 0X1E);
    // 0x802E5994: addiu       $a1, $zero, 0x1E
    ctx->r5 = ADD32(0, 0X1E);
    // 0x802E5998: or          $a2, $s5, $zero
    ctx->r6 = ctx->r21 | 0;
    // 0x802E599C: or          $a3, $s6, $zero
    ctx->r7 = ctx->r22 | 0;
    // 0x802E59A0: mul.d       $f4, $f6, $f26
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f26.d); 
    ctx->f4.d = MUL_D(ctx->f6.d, ctx->f26.d);
    // 0x802E59A4: add.d       $f10, $f4, $f28
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f28.d); 
    ctx->f10.d = ctx->f4.d + ctx->f28.d;
    // 0x802E59A8: sub.d       $f16, $f10, $f30
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 30);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f30.d); 
    ctx->f16.d = ctx->f10.d - ctx->f30.d;
    // 0x802E59AC: cvt.s.d     $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f8.fl = CVT_S_D(ctx->f16.d);
    // 0x802E59B0: jalr        $t9
    // 0x802E59B4: swc1        $f8, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->f8.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E59B4: swc1        $f8, 0xA0($sp)
    MEM_W(0XA0, ctx->r29) = ctx->f8.u32l;
    after_4:
    // 0x802E59B8: beq         $v0, $zero, L_802E5A48
    if (ctx->r2 == 0) {
        // 0x802E59BC: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_802E5A48;
    }
    // 0x802E59BC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x802E59C0: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x802E59C4: lui         $s2, 0x8006
    ctx->r18 = S32(0X8006 << 16);
    // 0x802E59C8: addiu       $s2, $s2, -0x337C
    ctx->r18 = ADD32(ctx->r18, -0X337C);
    // 0x802E59CC: addiu       $s1, $s1, -0x3768
    ctx->r17 = ADD32(ctx->r17, -0X3768);
    // 0x802E59D0: jalr        $s4
    // 0x802E59D4: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    LOOKUP_FUNC(ctx->r20)(rdram, ctx);
        goto after_5;
    // 0x802E59D4: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    after_5:
    // 0x802E59D8: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x802E59DC: lui         $at, 0x4F80
    ctx->r1 = S32(0X4F80 << 16);
    // 0x802E59E0: bgez        $v0, L_802E59F4
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E59E4: cvt.s.w     $f6, $f18
        CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    ctx->f6.fl = CVT_S_W(ctx->f18.u32l);
            goto L_802E59F4;
    }
    // 0x802E59E4: cvt.s.w     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    ctx->f6.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E59E8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x802E59EC: nop

    // 0x802E59F0: add.s       $f6, $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f6.fl + ctx->f4.fl;
L_802E59F4:
    // 0x802E59F4: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x802E59F8: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E59FC: mul.d       $f16, $f10, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f20.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f20.d);
    // 0x802E5A00: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x802E5A04: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x802E5A08: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E5A0C: ldc1        $f4, -0x3DB0($at)
    CHECK_FR(ctx, 4);
    ctx->f4.u64 = LD(ctx->r1, -0X3DB0);
    // 0x802E5A10: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E5A14: or          $a1, $s7, $zero
    ctx->r5 = ctx->r23 | 0;
    // 0x802E5A18: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    // 0x802E5A1C: mul.d       $f18, $f16, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f8.d); 
    ctx->f18.d = MUL_D(ctx->f16.d, ctx->f8.d);
    // 0x802E5A20: add.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = ctx->f18.d + ctx->f4.d;
    // 0x802E5A24: cvt.s.d     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f0.fl = CVT_S_D(ctx->f6.d);
    // 0x802E5A28: swc1        $f0, 0x94($sp)
    MEM_W(0X94, ctx->r29) = ctx->f0.u32l;
    // 0x802E5A2C: swc1        $f0, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->f0.u32l;
    // 0x802E5A30: jalr        $s1
    // 0x802E5A34: swc1        $f0, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_6;
    // 0x802E5A34: swc1        $f0, 0x8C($sp)
    MEM_W(0X8C, ctx->r29) = ctx->f0.u32l;
    after_6:
    // 0x802E5A38: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E5A3C: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    // 0x802E5A40: jalr        $s2
    // 0x802E5A44: addiu       $a2, $zero, 0x800
    ctx->r6 = ADD32(0, 0X800);
    LOOKUP_FUNC(ctx->r18)(rdram, ctx);
        goto after_7;
    // 0x802E5A44: addiu       $a2, $zero, 0x800
    ctx->r6 = ADD32(0, 0X800);
    after_7:
L_802E5A48:
    // 0x802E5A48: addiu       $s3, $s3, 0x1
    ctx->r19 = ADD32(ctx->r19, 0X1);
    // 0x802E5A4C: addiu       $at, $zero, 0x4
    ctx->r1 = ADD32(0, 0X4);
    // 0x802E5A50: bne         $s3, $at, L_802E5918
    if (ctx->r19 != ctx->r1) {
        // 0x802E5A54: nop
    
            goto L_802E5918;
    }
    // 0x802E5A54: nop

    // 0x802E5A58: lw          $ra, 0x6C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X6C);
L_802E5A5C:
    // 0x802E5A5C: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x802E5A60: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x802E5A64: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x802E5A68: ldc1        $f26, 0x30($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X30);
    // 0x802E5A6C: ldc1        $f28, 0x38($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X38);
    // 0x802E5A70: ldc1        $f30, 0x40($sp)
    CHECK_FR(ctx, 30);
    ctx->f30.u64 = LD(ctx->r29, 0X40);
    // 0x802E5A74: lw          $s0, 0x48($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X48);
    // 0x802E5A78: lw          $s1, 0x4C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X4C);
    // 0x802E5A7C: lw          $s2, 0x50($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X50);
    // 0x802E5A80: lw          $s3, 0x54($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X54);
    // 0x802E5A84: lw          $s4, 0x58($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X58);
    // 0x802E5A88: lw          $s5, 0x5C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X5C);
    // 0x802E5A8C: lw          $s6, 0x60($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X60);
    // 0x802E5A90: lw          $s7, 0x64($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X64);
    // 0x802E5A94: lw          $fp, 0x68($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X68);
    // 0x802E5A98: jr          $ra
    // 0x802E5A9C: addiu       $sp, $sp, 0xB8
    ctx->r29 = ADD32(ctx->r29, 0XB8);
    return;
    // 0x802E5A9C: addiu       $sp, $sp, 0xB8
    ctx->r29 = ADD32(ctx->r29, 0XB8);
;}
RECOMP_FUNC void map_ovl_00_func_802E5AA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5AA0: jr          $ra
    // 0x802E5AA4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x802E5AA4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void map_ovl_00_func_802E5AA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5AA8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E5AAC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E5AB0: jal         0x801469F4
    // 0x802E5AB4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_801469F4(rdram, ctx);
        goto after_0;
    // 0x802E5AB4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x802E5AB8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E5ABC: lw          $t9, 0x34($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X34);
    // 0x802E5AC0: jalr        $t9
    // 0x802E5AC4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5AC4: nop

    after_1:
    // 0x802E5AC8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E5ACC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E5AD0: jr          $ra
    // 0x802E5AD4: nop

    return;
    // 0x802E5AD4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5AD8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5AD8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x802E5ADC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E5AE0: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x802E5AE4: sw          $a1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r5;
    // 0x802E5AE8: lh          $v0, 0x5C($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X5C);
    // 0x802E5AEC: lui         $t9, 0x8009
    ctx->r25 = S32(0X8009 << 16);
    // 0x802E5AF0: addiu       $t9, $t9, 0x7330
    ctx->r25 = ADD32(ctx->r25, 0X7330);
    // 0x802E5AF4: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    // 0x802E5AF8: jalr        $t9
    // 0x802E5AFC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5AFC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x802E5B00: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x802E5B04: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x802E5B08: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x802E5B0C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E5B10: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x802E5B14: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E5B18: ldc1        $f18, -0x3DA8($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, -0X3DA8);
    // 0x802E5B1C: lw          $t7, 0x28($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X28);
    // 0x802E5B20: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x802E5B24: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x802E5B28: lwc1        $f6, 0x54($t7)
    ctx->f6.u32l = MEM_W(ctx->r15, 0X54);
    // 0x802E5B2C: div.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = DIV_D(ctx->f8.d, ctx->f10.d);
    // 0x802E5B30: addiu       $t9, $t9, 0x3A50
    ctx->r25 = ADD32(ctx->r25, 0X3A50);
    // 0x802E5B34: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E5B38: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x802E5B3C: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x802E5B40: sub.d       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f4.d); 
    ctx->f10.d = ctx->f8.d - ctx->f4.d;
    // 0x802E5B44: cvt.s.d     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f16.fl = CVT_S_D(ctx->f10.d);
    // 0x802E5B48: jalr        $t9
    // 0x802E5B4C: swc1        $f16, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f16.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5B4C: swc1        $f16, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->f16.u32l;
    after_1:
    // 0x802E5B50: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x802E5B54: lui         $at, 0x40F0
    ctx->r1 = S32(0X40F0 << 16);
    // 0x802E5B58: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    // 0x802E5B5C: cvt.s.w     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    ctx->f6.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E5B60: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x802E5B64: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E5B68: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E5B6C: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E5B70: ldc1        $f16, -0x3DA0($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, -0X3DA0);
    // 0x802E5B74: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x802E5B78: lw          $t8, 0x28($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X28);
    // 0x802E5B7C: div.d       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f4.d); 
    ctx->f10.d = DIV_D(ctx->f8.d, ctx->f4.d);
    // 0x802E5B80: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E5B84: lwc1        $f6, 0x58($t8)
    ctx->f6.u32l = MEM_W(ctx->r24, 0X58);
    // 0x802E5B88: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x802E5B8C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5B90: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x802E5B94: addiu       $t9, $t9, 0x4AB0
    ctx->r25 = ADD32(ctx->r25, 0X4AB0);
    // 0x802E5B98: mul.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f16.d);
    // 0x802E5B9C: lwc1        $f16, 0x20($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X20);
    // 0x802E5BA0: ldc1        $f10, -0x3D98($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X3D98);
    // 0x802E5BA4: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E5BA8: cvt.d.s     $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f6.d = CVT_D_S(ctx->f16.fl);
    // 0x802E5BAC: add.d       $f4, $f18, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f8.d); 
    ctx->f4.d = ctx->f18.d + ctx->f8.d;
    // 0x802E5BB0: sub.d       $f18, $f10, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f6.d); 
    ctx->f18.d = ctx->f10.d - ctx->f6.d;
    // 0x802E5BB4: ldc1        $f10, -0x3D90($at)
    CHECK_FR(ctx, 10);
    ctx->f10.u64 = LD(ctx->r1, -0X3D90);
    // 0x802E5BB8: cvt.s.d     $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f2.fl = CVT_S_D(ctx->f4.d);
    // 0x802E5BBC: mul.d       $f8, $f18, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d); 
    ctx->f8.d = MUL_D(ctx->f18.d, ctx->f0.d);
    // 0x802E5BC0: cvt.d.s     $f16, $f2
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); 
    ctx->f16.d = CVT_D_S(ctx->f2.fl);
    // 0x802E5BC4: swc1        $f2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->f2.u32l;
    // 0x802E5BC8: sub.d       $f6, $f16, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f10.d); 
    ctx->f6.d = ctx->f16.d - ctx->f10.d;
    // 0x802E5BCC: trunc.w.d   $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.u32l = TRUNC_W_D(ctx->f8.d);
    // 0x802E5BD0: mul.d       $f18, $f6, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = MUL_D(ctx->f6.d, ctx->f0.d);
    // 0x802E5BD4: mfc1        $a0, $f4
    ctx->r4 = (int32_t)ctx->f4.u32l;
    // 0x802E5BD8: trunc.w.d   $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    ctx->f8.u32l = TRUNC_W_D(ctx->f18.d);
    // 0x802E5BDC: mfc1        $a1, $f8
    ctx->r5 = (int32_t)ctx->f8.u32l;
    // 0x802E5BE0: jalr        $t9
    // 0x802E5BE4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E5BE4: nop

    after_2:
    // 0x802E5BE8: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
    // 0x802E5BEC: lw          $v1, 0x2C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X2C);
    // 0x802E5BF0: lwc1        $f2, 0x1C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x802E5BF4: lwc1        $f4, 0x50($t2)
    ctx->f4.u32l = MEM_W(ctx->r10, 0X50);
    // 0x802E5BF8: swc1        $f4, 0x50($v1)
    MEM_W(0X50, ctx->r3) = ctx->f4.u32l;
    // 0x802E5BFC: lwc1        $f16, 0x20($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X20);
    // 0x802E5C00: sh          $v0, 0x5C($v1)
    MEM_H(0X5C, ctx->r3) = ctx->r2;
    // 0x802E5C04: swc1        $f2, 0x58($v1)
    MEM_W(0X58, ctx->r3) = ctx->f2.u32l;
    // 0x802E5C08: swc1        $f16, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f16.u32l;
    // 0x802E5C0C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E5C10: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x802E5C14: jr          $ra
    // 0x802E5C18: nop

    return;
    // 0x802E5C18: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5C1C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5C1C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E5C20: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E5C24: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x802E5C28: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E5C2C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E5C30: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E5C34: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x802E5C38: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x802E5C3C: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x802E5C40: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x802E5C44: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x802E5C48: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x802E5C4C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E5C50: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x802E5C54: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x802E5C58: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E5C5C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x802E5C60: lw          $t9, -0x4628($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X4628);
    // 0x802E5C64: jalr        $t9
    // 0x802E5C68: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5C68: nop

    after_0:
    // 0x802E5C6C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E5C70: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x802E5C74: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x802E5C78: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x802E5C7C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E5C80: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E5C84: jr          $ra
    // 0x802E5C88: nop

    return;
    // 0x802E5C88: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5C8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5C8C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x802E5C90: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5C94: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E5C98: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E5C9C: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x802E5CA0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E5CA4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x802E5CA8: addiu       $a2, $zero, 0x1C
    ctx->r6 = ADD32(0, 0X1C);
    // 0x802E5CAC: jalr        $t9
    // 0x802E5CB0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5CB0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x802E5CB4: bne         $v0, $zero, L_802E5CD4
    if (ctx->r2 != 0) {
        // 0x802E5CB8: addiu       $a0, $s0, 0x8
        ctx->r4 = ADD32(ctx->r16, 0X8);
            goto L_802E5CD4;
    }
    // 0x802E5CB8: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E5CBC: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x802E5CC0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E5CC4: jalr        $t9
    // 0x802E5CC8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5CC8: nop

    after_1:
    // 0x802E5CCC: b           L_802E5CE8
    // 0x802E5CD0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E5CE8;
    // 0x802E5CD0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E5CD4:
    // 0x802E5CD4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5CD8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E5CDC: jalr        $t9
    // 0x802E5CE0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E5CE0: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x802E5CE4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E5CE8:
    // 0x802E5CE8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E5CEC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x802E5CF0: jr          $ra
    // 0x802E5CF4: nop

    return;
    // 0x802E5CF4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5CF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5CF8: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x802E5CFC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E5D00: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x802E5D04: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x802E5D08: lw          $t6, 0x34($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X34);
    // 0x802E5D0C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5D10: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x802E5D14: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x802E5D18: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x802E5D1C: addiu       $a2, $zero, 0x20
    ctx->r6 = ADD32(0, 0X20);
    // 0x802E5D20: addiu       $a3, $zero, 0x2
    ctx->r7 = ADD32(0, 0X2);
    // 0x802E5D24: jalr        $t9
    // 0x802E5D28: sw          $t6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r14;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5D28: sw          $t6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r14;
    after_0:
    // 0x802E5D2C: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x802E5D30: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x802E5D34: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x802E5D38: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x802E5D3C: bne         $v0, $zero, L_802E5D5C
    if (ctx->r2 != 0) {
        // 0x802E5D40: sw          $v0, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r2;
            goto L_802E5D5C;
    }
    // 0x802E5D40: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x802E5D44: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x802E5D48: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E5D4C: jalr        $t9
    // 0x802E5D50: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5D50: nop

    after_1:
    // 0x802E5D54: b           L_802E5EF4
    // 0x802E5D58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E5EF4;
    // 0x802E5D58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E5D5C:
    // 0x802E5D5C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5D60: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x802E5D64: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x802E5D68: jalr        $t9
    // 0x802E5D6C: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E5D6C: sw          $v1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r3;
    after_2:
    // 0x802E5D70: sw          $v0, 0x24($s1)
    MEM_W(0X24, ctx->r17) = ctx->r2;
    // 0x802E5D74: bne         $v0, $zero, L_802E5D94
    if (ctx->r2 != 0) {
        // 0x802E5D78: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_802E5D94;
    }
    // 0x802E5D78: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x802E5D7C: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x802E5D80: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E5D84: jalr        $t9
    // 0x802E5D88: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E5D88: nop

    after_3:
    // 0x802E5D8C: b           L_802E5EF4
    // 0x802E5D90: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E5EF4;
    // 0x802E5D90: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E5D94:
    // 0x802E5D94: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E5D98: addiu       $t9, $t9, 0x31CC
    ctx->r25 = ADD32(ctx->r25, 0X31CC);
    // 0x802E5D9C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E5DA0: jalr        $t9
    // 0x802E5DA4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E5DA4: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_4:
    // 0x802E5DA8: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x802E5DAC: lw          $t7, -0x522C($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X522C);
    // 0x802E5DB0: lui         $t8, 0x603
    ctx->r24 = S32(0X603 << 16);
    // 0x802E5DB4: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x802E5DB8: addiu       $t8, $t8, 0x50D8
    ctx->r24 = ADD32(ctx->r24, 0X50D8);
    // 0x802E5DBC: addiu       $t9, $t9, -0x6C40
    ctx->r25 = ADD32(ctx->r25, -0X6C40);
    // 0x802E5DC0: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x802E5DC4: sw          $t8, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r24;
    // 0x802E5DC8: addiu       $a0, $a0, -0x19F0
    ctx->r4 = ADD32(ctx->r4, -0X19F0);
    // 0x802E5DCC: jalr        $t9
    // 0x802E5DD0: sw          $t7, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E5DD0: sw          $t7, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r15;
    after_5:
    // 0x802E5DD4: lhu         $t0, 0x2($s0)
    ctx->r8 = MEM_HU(ctx->r16, 0X2);
    // 0x802E5DD8: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x802E5DDC: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x802E5DE0: ori         $t1, $t0, 0x900
    ctx->r9 = ctx->r8 | 0X900;
    // 0x802E5DE4: sw          $v0, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->r2;
    // 0x802E5DE8: sh          $t1, 0x2($s0)
    MEM_H(0X2, ctx->r16) = ctx->r9;
    // 0x802E5DEC: lw          $t2, 0x2B0C($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X2B0C);
    // 0x802E5DF0: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E5DF4: addiu       $t9, $t9, 0x6504
    ctx->r25 = ADD32(ctx->r25, 0X6504);
    // 0x802E5DF8: sw          $t2, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r10;
    // 0x802E5DFC: lw          $t3, 0x7C($v1)
    ctx->r11 = MEM_W(ctx->r3, 0X7C);
    // 0x802E5E00: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x802E5E04: sw          $t3, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r11;
    // 0x802E5E08: lh          $t4, 0x0($s1)
    ctx->r12 = MEM_H(ctx->r17, 0X0);
    // 0x802E5E0C: ori         $t5, $t4, 0x1000
    ctx->r13 = ctx->r12 | 0X1000;
    // 0x802E5E10: sh          $t5, 0x0($s1)
    MEM_H(0X0, ctx->r17) = ctx->r13;
    // 0x802E5E14: lw          $a1, 0x40($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X40);
    // 0x802E5E18: jalr        $t9
    // 0x802E5E1C: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x802E5E1C: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    after_6:
    // 0x802E5E20: sw          $v0, 0x74($s0)
    MEM_W(0X74, ctx->r16) = ctx->r2;
    // 0x802E5E24: sw          $v0, 0x68($s1)
    MEM_W(0X68, ctx->r17) = ctx->r2;
    // 0x802E5E28: jal         0x80146900
    // 0x802E5E2C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80146900(rdram, ctx);
        goto after_7;
    // 0x802E5E2C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_7:
    // 0x802E5E30: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E5E34: addiu       $t9, $t9, 0x724
    ctx->r25 = ADD32(ctx->r25, 0X724);
    // 0x802E5E38: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E5E3C: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x802E5E40: jalr        $t9
    // 0x802E5E44: addiu       $a2, $zero, 0x200
    ctx->r6 = ADD32(0, 0X200);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x802E5E44: addiu       $a2, $zero, 0x200
    ctx->r6 = ADD32(0, 0X200);
    after_8:
    // 0x802E5E48: beq         $v0, $zero, L_802E5EC0
    if (ctx->r2 == 0) {
        // 0x802E5E4C: lui         $t9, 0x802E
        ctx->r25 = S32(0X802E << 16);
            goto L_802E5EC0;
    }
    // 0x802E5E4C: lui         $t9, 0x802E
    ctx->r25 = S32(0X802E << 16);
    // 0x802E5E50: lw          $t6, 0x34($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X34);
    // 0x802E5E54: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x802E5E58: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x802E5E5C: sw          $v0, 0x4($t6)
    MEM_W(0X4, ctx->r14) = ctx->r2;
    // 0x802E5E60: lui         $at, 0xC270
    ctx->r1 = S32(0XC270 << 16);
    // 0x802E5E64: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x802E5E68: lhu         $t7, 0xA($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0XA);
    // 0x802E5E6C: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x802E5E70: mtc1        $at, $f10
    ctx->f10.u32l = ctx->r1;
    // 0x802E5E74: lui         $at, 0x4370
    ctx->r1 = S32(0X4370 << 16);
    // 0x802E5E78: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E5E7C: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x802E5E80: andi        $t0, $t7, 0xF9FF
    ctx->r8 = ctx->r15 & 0XF9FF;
    // 0x802E5E84: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x802E5E88: sh          $t0, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r8;
    // 0x802E5E8C: ori         $t1, $t0, 0x400
    ctx->r9 = ctx->r8 | 0X400;
    // 0x802E5E90: addiu       $t2, $zero, 0x4000
    ctx->r10 = ADD32(0, 0X4000);
    // 0x802E5E94: addiu       $t3, $zero, 0x3E8
    ctx->r11 = ADD32(0, 0X3E8);
    // 0x802E5E98: sb          $v1, 0xD($v0)
    MEM_B(0XD, ctx->r2) = ctx->r3;
    // 0x802E5E9C: sh          $t1, 0xA($v0)
    MEM_H(0XA, ctx->r2) = ctx->r9;
    // 0x802E5EA0: sh          $v1, 0x4($v0)
    MEM_H(0X4, ctx->r2) = ctx->r3;
    // 0x802E5EA4: sh          $t2, 0x4E($v0)
    MEM_H(0X4E, ctx->r2) = ctx->r10;
    // 0x802E5EA8: sh          $t3, 0x8($v0)
    MEM_H(0X8, ctx->r2) = ctx->r11;
    // 0x802E5EAC: swc1        $f6, 0x58($v0)
    MEM_W(0X58, ctx->r2) = ctx->f6.u32l;
    // 0x802E5EB0: swc1        $f8, 0x5C($v0)
    MEM_W(0X5C, ctx->r2) = ctx->f8.u32l;
    // 0x802E5EB4: swc1        $f10, 0x60($v0)
    MEM_W(0X60, ctx->r2) = ctx->f10.u32l;
    // 0x802E5EB8: swc1        $f4, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f4.u32l;
    // 0x802E5EBC: swc1        $f16, 0x64($v0)
    MEM_W(0X64, ctx->r2) = ctx->f16.u32l;
L_802E5EC0:
    // 0x802E5EC0: lw          $t4, 0x10($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X10);
    // 0x802E5EC4: lw          $t5, 0x20($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X20);
    // 0x802E5EC8: addiu       $t9, $t9, 0x6BC0
    ctx->r25 = ADD32(ctx->r25, 0X6BC0);
    // 0x802E5ECC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E5ED0: sw          $t4, 0x10($t5)
    MEM_W(0X10, ctx->r13) = ctx->r12;
    // 0x802E5ED4: jal         0x80143234
    // 0x802E5ED8: sw          $t9, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r25;
    func_80143234(rdram, ctx);
        goto after_9;
    // 0x802E5ED8: sw          $t9, 0x10($s1)
    MEM_W(0X10, ctx->r17) = ctx->r25;
    after_9:
    // 0x802E5EDC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E5EE0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E5EE4: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x802E5EE8: jalr        $t9
    // 0x802E5EEC: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x802E5EEC: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_10:
    // 0x802E5EF0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E5EF4:
    // 0x802E5EF4: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x802E5EF8: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x802E5EFC: jr          $ra
    // 0x802E5F00: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x802E5F00: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void map_ovl_00_func_802E5F04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5F04: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E5F08: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E5F0C: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x802E5F10: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E5F14: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x802E5F18: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E5F1C: jalr        $t9
    // 0x802E5F20: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E5F20: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    after_0:
    // 0x802E5F24: beq         $v0, $zero, L_802E5F40
    if (ctx->r2 == 0) {
        // 0x802E5F28: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_802E5F40;
    }
    // 0x802E5F28: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E5F2C: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x802E5F30: jalr        $t9
    // 0x802E5F34: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E5F34: nop

    after_1:
    // 0x802E5F38: b           L_802E5F9C
    // 0x802E5F3C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_802E5F9C;
    // 0x802E5F3C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E5F40:
    // 0x802E5F40: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x802E5F44: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E5F48: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E5F4C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E5F50: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E5F54: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x802E5F58: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x802E5F5C: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x802E5F60: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x802E5F64: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x802E5F68: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x802E5F6C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E5F70: lbu         $t0, 0x9($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X9);
    // 0x802E5F74: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x802E5F78: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x802E5F7C: lw          $t9, -0x461C($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X461C);
    // 0x802E5F80: jalr        $t9
    // 0x802E5F84: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E5F84: nop

    after_2:
    // 0x802E5F88: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E5F8C: lh          $t2, 0xE($a0)
    ctx->r10 = MEM_H(ctx->r4, 0XE);
    // 0x802E5F90: addiu       $t3, $t2, -0x1
    ctx->r11 = ADD32(ctx->r10, -0X1);
    // 0x802E5F94: sh          $t3, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r11;
    // 0x802E5F98: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E5F9C:
    // 0x802E5F9C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E5FA0: jr          $ra
    // 0x802E5FA4: nop

    return;
    // 0x802E5FA4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E5FA8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E5FA8: addiu       $sp, $sp, -0x110
    ctx->r29 = ADD32(ctx->r29, -0X110);
    // 0x802E5FAC: sw          $ra, 0x6C($sp)
    MEM_W(0X6C, ctx->r29) = ctx->r31;
    // 0x802E5FB0: sw          $fp, 0x68($sp)
    MEM_W(0X68, ctx->r29) = ctx->r30;
    // 0x802E5FB4: sw          $s7, 0x64($sp)
    MEM_W(0X64, ctx->r29) = ctx->r23;
    // 0x802E5FB8: sw          $s6, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r22;
    // 0x802E5FBC: sw          $s5, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r21;
    // 0x802E5FC0: sw          $s4, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r20;
    // 0x802E5FC4: sw          $s3, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r19;
    // 0x802E5FC8: sw          $s2, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r18;
    // 0x802E5FCC: sw          $s1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r17;
    // 0x802E5FD0: sw          $s0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r16;
    // 0x802E5FD4: sdc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    SD(ctx->f28.u64, 0X40, ctx->r29);
    // 0x802E5FD8: sdc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    SD(ctx->f26.u64, 0X38, ctx->r29);
    // 0x802E5FDC: sdc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X30, ctx->r29);
    // 0x802E5FE0: sdc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X28, ctx->r29);
    // 0x802E5FE4: sdc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X20, ctx->r29);
    // 0x802E5FE8: lw          $t6, 0x70($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X70);
    // 0x802E5FEC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x802E5FF0: lw          $s4, 0x24($a0)
    ctx->r20 = MEM_W(ctx->r4, 0X24);
    // 0x802E5FF4: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x802E5FF8: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x802E5FFC: lw          $t7, 0x2B0C($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X2B0C);
    // 0x802E6000: sw          $t6, 0xFC($sp)
    MEM_W(0XFC, ctx->r29) = ctx->r14;
    // 0x802E6004: lw          $s1, 0x4($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X4);
    // 0x802E6008: sw          $t7, 0x18($s4)
    MEM_W(0X18, ctx->r20) = ctx->r15;
    // 0x802E600C: lw          $t8, 0x7C($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X7C);
    // 0x802E6010: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x802E6014: addiu       $a0, $zero, 0x145
    ctx->r4 = ADD32(0, 0X145);
    // 0x802E6018: sw          $t8, 0x24($s4)
    MEM_W(0X24, ctx->r20) = ctx->r24;
    // 0x802E601C: lbu         $t9, 0x4A($s1)
    ctx->r25 = MEM_BU(ctx->r17, 0X4A);
    // 0x802E6020: addiu       $a1, $s4, 0x50
    ctx->r5 = ADD32(ctx->r20, 0X50);
    // 0x802E6024: beq         $t9, $zero, L_802E68A8
    if (ctx->r25 == 0) {
        // 0x802E6028: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_802E68A8;
    }
    // 0x802E6028: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x802E602C: addiu       $t9, $t9, 0x6294
    ctx->r25 = ADD32(ctx->r25, 0X6294);
    // 0x802E6030: jalr        $t9
    // 0x802E6034: sw          $a2, 0x110($sp)
    MEM_W(0X110, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6034: sw          $a2, 0x110($sp)
    MEM_W(0X110, ctx->r29) = ctx->r6;
    after_0:
    // 0x802E6038: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E603C: addiu       $t9, $t9, 0x4780
    ctx->r25 = ADD32(ctx->r25, 0X4780);
    // 0x802E6040: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E6044: jalr        $t9
    // 0x802E6048: lw          $a1, 0x110($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X110);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E6048: lw          $a1, 0x110($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X110);
    after_1:
    // 0x802E604C: addiu       $v1, $s0, 0x8
    ctx->r3 = ADD32(ctx->r16, 0X8);
    // 0x802E6050: ori         $t0, $zero, 0xFFFF
    ctx->r8 = 0 | 0XFFFF;
    // 0x802E6054: sw          $t0, 0xC($v1)
    MEM_W(0XC, ctx->r3) = ctx->r8;
    // 0x802E6058: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x802E605C: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E6060: lui         $t3, 0x8009
    ctx->r11 = S32(0X8009 << 16);
    // 0x802E6064: lh          $t1, 0x0($v0)
    ctx->r9 = MEM_H(ctx->r2, 0X0);
    // 0x802E6068: lui         $t4, 0x800A
    ctx->r12 = S32(0X800A << 16);
    // 0x802E606C: addiu       $t4, $t4, 0x3A50
    ctx->r12 = ADD32(ctx->r12, 0X3A50);
    // 0x802E6070: andi        $t2, $t1, 0xF9FF
    ctx->r10 = ctx->r9 & 0XF9FF;
    // 0x802E6074: sh          $t2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r10;
    // 0x802E6078: ldc1        $f28, -0x3D88($at)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r1, -0X3D88);
    // 0x802E607C: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x802E6080: mtc1        $at, $f27
    ctx->f_odd[(27 - 1) * 2] = ctx->r1;
    // 0x802E6084: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E6088: addiu       $t3, $t3, 0x7330
    ctx->r11 = ADD32(ctx->r11, 0X7330);
    // 0x802E608C: lui         $s7, 0x8000
    ctx->r23 = S32(0X8000 << 16);
    // 0x802E6090: lui         $s5, 0x8006
    ctx->r21 = S32(0X8006 << 16);
    // 0x802E6094: mtc1        $zero, $f26
    ctx->f26.u32l = 0;
    // 0x802E6098: addiu       $s5, $s5, -0x6B48
    ctx->r21 = ADD32(ctx->r21, -0X6B48);
    // 0x802E609C: addiu       $s7, $s7, 0x47C0
    ctx->r23 = ADD32(ctx->r23, 0X47C0);
    // 0x802E60A0: sw          $t3, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r11;
    // 0x802E60A4: ldc1        $f24, -0x3D80($at)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r1, -0X3D80);
    // 0x802E60A8: sw          $v1, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r3;
    // 0x802E60AC: sw          $t4, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r12;
    // 0x802E60B0: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x802E60B4: lui         $fp, 0x2000
    ctx->r30 = S32(0X2000 << 16);
    // 0x802E60B8: addiu       $s6, $sp, 0xE4
    ctx->r22 = ADD32(ctx->r29, 0XE4);
L_802E60BC:
    // 0x802E60BC: jalr        $s7
    // 0x802E60C0: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_2;
    // 0x802E60C0: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    after_2:
    // 0x802E60C4: addiu       $t5, $v0, -0x2710
    ctx->r13 = ADD32(ctx->r2, -0X2710);
    // 0x802E60C8: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x802E60CC: lui         $at, 0x4008
    ctx->r1 = S32(0X4008 << 16);
    // 0x802E60D0: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x802E60D4: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E60D8: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x802E60DC: lui         $at, 0xC000
    ctx->r1 = S32(0XC000 << 16);
    // 0x802E60E0: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E60E4: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E60E8: addiu       $a0, $zero, 0x4E20
    ctx->r4 = ADD32(0, 0X4E20);
    // 0x802E60EC: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x802E60F0: mul.d       $f10, $f8, $f24
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f24.d); 
    ctx->f10.d = MUL_D(ctx->f8.d, ctx->f24.d);
    // 0x802E60F4: nop

    // 0x802E60F8: mul.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f16.d);
    // 0x802E60FC: add.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = ctx->f18.d + ctx->f4.d;
    // 0x802E6100: jalr        $s7
    // 0x802E6104: cvt.s.d     $f20, $f6
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f20.fl = CVT_S_D(ctx->f6.d);
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_3;
    // 0x802E6104: cvt.s.d     $f20, $f6
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f20.fl = CVT_S_D(ctx->f6.d);
    after_3:
    // 0x802E6108: addiu       $t6, $v0, -0x2710
    ctx->r14 = ADD32(ctx->r2, -0X2710);
    // 0x802E610C: mtc1        $t6, $f8
    ctx->f8.u32l = ctx->r14;
    // 0x802E6110: lui         $at, 0x4028
    ctx->r1 = S32(0X4028 << 16);
    // 0x802E6114: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E6118: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E611C: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E6120: lui         $at, 0x400C
    ctx->r1 = S32(0X400C << 16);
    // 0x802E6124: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x802E6128: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x802E612C: lw          $t9, 0x88($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X88);
    // 0x802E6130: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x802E6134: lhu         $a0, 0x5E($s4)
    ctx->r4 = MEM_HU(ctx->r20, 0X5E);
    // 0x802E6138: mul.d       $f18, $f16, $f24
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f24.d); 
    ctx->f18.d = MUL_D(ctx->f16.d, ctx->f24.d);
    // 0x802E613C: nop

    // 0x802E6140: mul.d       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f18.d, ctx->f4.d);
    // 0x802E6144: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x802E6148: jalr        $t9
    // 0x802E614C: cvt.s.d     $f22, $f10
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f22.fl = CVT_S_D(ctx->f10.d);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x802E614C: cvt.s.d     $f22, $f10
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f22.fl = CVT_S_D(ctx->f10.d);
    after_4:
    // 0x802E6150: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    // 0x802E6154: lw          $t9, 0x84($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X84);
    // 0x802E6158: lhu         $a0, 0x5E($s4)
    ctx->r4 = MEM_HU(ctx->r20, 0X5E);
    // 0x802E615C: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x802E6160: cvt.d.s     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f4.d = CVT_D_S(ctx->f18.fl);
    // 0x802E6164: div.d       $f6, $f4, $f26
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f26.d); 
    ctx->f6.d = DIV_D(ctx->f4.d, ctx->f26.d);
    // 0x802E6168: cvt.s.d     $f14, $f6
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f14.fl = CVT_S_D(ctx->f6.d);
    // 0x802E616C: jalr        $t9
    // 0x802E6170: swc1        $f14, 0xB0($sp)
    MEM_W(0XB0, ctx->r29) = ctx->f14.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x802E6170: swc1        $f14, 0xB0($sp)
    MEM_W(0XB0, ctx->r29) = ctx->f14.u32l;
    after_5:
    // 0x802E6174: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x802E6178: lwc1        $f14, 0xB0($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0XB0);
    // 0x802E617C: lwc1        $f4, 0x50($s4)
    ctx->f4.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E6180: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E6184: addiu       $a0, $zero, 0x1A
    ctx->r4 = ADD32(0, 0X1A);
    // 0x802E6188: addiu       $a1, $zero, 0x1A
    ctx->r5 = ADD32(0, 0X1A);
    // 0x802E618C: or          $a2, $s6, $zero
    ctx->r6 = ctx->r22 | 0;
    // 0x802E6190: or          $a3, $fp, $zero
    ctx->r7 = ctx->r30 | 0;
    // 0x802E6194: cvt.d.s     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.d = CVT_D_S(ctx->f10.fl);
    // 0x802E6198: div.d       $f18, $f16, $f26
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 26);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f26.d); 
    ctx->f18.d = DIV_D(ctx->f16.d, ctx->f26.d);
    // 0x802E619C: lwc1        $f16, 0x58($s4)
    ctx->f16.u32l = MEM_W(ctx->r20, 0X58);
    // 0x802E61A0: cvt.s.d     $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f0.fl = CVT_S_D(ctx->f18.d);
    // 0x802E61A4: mul.s       $f6, $f20, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f20.fl, ctx->f0.fl);
    // 0x802E61A8: nop

    // 0x802E61AC: mul.s       $f10, $f22, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f22.fl, ctx->f14.fl);
    // 0x802E61B0: nop

    // 0x802E61B4: mul.s       $f18, $f20, $f14
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f18.fl = MUL_S(ctx->f20.fl, ctx->f14.fl);
    // 0x802E61B8: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x802E61BC: mul.s       $f6, $f22, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f22.fl, ctx->f0.fl);
    // 0x802E61C0: add.s       $f2, $f8, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x802E61C4: sub.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f16.fl - ctx->f18.fl;
    // 0x802E61C8: swc1        $f2, 0xE4($sp)
    MEM_W(0XE4, ctx->r29) = ctx->f2.u32l;
    // 0x802E61CC: lwc1        $f8, 0x54($s4)
    ctx->f8.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E61D0: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x802E61D4: swc1        $f8, 0xE8($sp)
    MEM_W(0XE8, ctx->r29) = ctx->f8.u32l;
    // 0x802E61D8: jalr        $s5
    // 0x802E61DC: swc1        $f12, 0xEC($sp)
    MEM_W(0XEC, ctx->r29) = ctx->f12.u32l;
    LOOKUP_FUNC(ctx->r21)(rdram, ctx);
        goto after_6;
    // 0x802E61DC: swc1        $f12, 0xEC($sp)
    MEM_W(0XEC, ctx->r29) = ctx->f12.u32l;
    after_6:
    // 0x802E61E0: beq         $v0, $zero, L_802E6280
    if (ctx->r2 == 0) {
        // 0x802E61E4: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_802E6280;
    }
    // 0x802E61E4: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x802E61E8: lui         $t7, 0x8006
    ctx->r15 = S32(0X8006 << 16);
    // 0x802E61EC: addiu       $t7, $t7, -0x2F4C
    ctx->r15 = ADD32(ctx->r15, -0X2F4C);
    // 0x802E61F0: lui         $s1, 0x8006
    ctx->r17 = S32(0X8006 << 16);
    // 0x802E61F4: lui         $s3, 0x802F
    ctx->r19 = S32(0X802F << 16);
    // 0x802E61F8: addiu       $s3, $s3, -0x5670
    ctx->r19 = ADD32(ctx->r19, -0X5670);
    // 0x802E61FC: addiu       $s1, $s1, -0x3768
    ctx->r17 = ADD32(ctx->r17, -0X3768);
    // 0x802E6200: sw          $t7, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r15;
    // 0x802E6204: jalr        $s7
    // 0x802E6208: addiu       $a0, $zero, 0x7FFF
    ctx->r4 = ADD32(0, 0X7FFF);
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_7;
    // 0x802E6208: addiu       $a0, $zero, 0x7FFF
    ctx->r4 = ADD32(0, 0X7FFF);
    after_7:
    // 0x802E620C: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x802E6210: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E6214: bgez        $v0, L_802E6230
    if (SIGNED(ctx->r2) >= 0) {
        // 0x802E6218: cvt.d.w     $f16, $f10
        CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.d = CVT_D_W(ctx->f10.u32l);
            goto L_802E6230;
    }
    // 0x802E6218: cvt.d.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.d = CVT_D_W(ctx->f10.u32l);
    // 0x802E621C: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x802E6220: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x802E6224: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x802E6228: nop

    // 0x802E622C: add.d       $f16, $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f16.d = ctx->f16.d + ctx->f18.d;
L_802E6230:
    // 0x802E6230: lui         $at, 0x40F0
    ctx->r1 = S32(0X40F0 << 16);
    // 0x802E6234: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E6238: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E623C: addiu       $a1, $sp, 0xD8
    ctx->r5 = ADD32(ctx->r29, 0XD8);
    // 0x802E6240: div.d       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = DIV_D(ctx->f16.d, ctx->f4.d);
    // 0x802E6244: ori         $a2, $zero, 0x8000
    ctx->r6 = 0 | 0X8000;
    // 0x802E6248: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x802E624C: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x802E6250: mul.d       $f18, $f10, $f28
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f28.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f28.d);
    // 0x802E6254: add.d       $f16, $f18, $f28
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 28);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f28.d); 
    ctx->f16.d = ctx->f18.d + ctx->f28.d;
    // 0x802E6258: cvt.s.d     $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f0.fl = CVT_S_D(ctx->f16.d);
    // 0x802E625C: swc1        $f0, 0xDC($sp)
    MEM_W(0XDC, ctx->r29) = ctx->f0.u32l;
    // 0x802E6260: swc1        $f0, 0xD8($sp)
    MEM_W(0XD8, ctx->r29) = ctx->f0.u32l;
    // 0x802E6264: jalr        $s1
    // 0x802E6268: swc1        $f0, 0xE0($sp)
    MEM_W(0XE0, ctx->r29) = ctx->f0.u32l;
    LOOKUP_FUNC(ctx->r17)(rdram, ctx);
        goto after_8;
    // 0x802E6268: swc1        $f0, 0xE0($sp)
    MEM_W(0XE0, ctx->r29) = ctx->f0.u32l;
    after_8:
    // 0x802E626C: lw          $t9, 0x74($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X74);
    // 0x802E6270: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E6274: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x802E6278: jalr        $t9
    // 0x802E627C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x802E627C: nop

    after_9:
L_802E6280:
    // 0x802E6280: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x802E6284: slti        $at, $s2, 0x8
    ctx->r1 = SIGNED(ctx->r18) < 0X8 ? 1 : 0;
    // 0x802E6288: bne         $at, $zero, L_802E60BC
    if (ctx->r1 != 0) {
        // 0x802E628C: nop
    
            goto L_802E60BC;
    }
    // 0x802E628C: nop

    // 0x802E6290: lui         $t9, 0x800A
    ctx->r25 = S32(0X800A << 16);
    // 0x802E6294: addiu       $t9, $t9, -0x6C40
    ctx->r25 = ADD32(ctx->r25, -0X6C40);
    // 0x802E6298: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x802E629C: jalr        $t9
    // 0x802E62A0: addiu       $a0, $a0, -0x19B8
    ctx->r4 = ADD32(ctx->r4, -0X19B8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_10;
    // 0x802E62A0: addiu       $a0, $a0, -0x19B8
    ctx->r4 = ADD32(ctx->r4, -0X19B8);
    after_10:
    // 0x802E62A4: sw          $v0, 0x38($s4)
    MEM_W(0X38, ctx->r20) = ctx->r2;
    // 0x802E62A8: jalr        $s7
    // 0x802E62AC: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_11;
    // 0x802E62AC: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    after_11:
    // 0x802E62B0: bne         $v0, $zero, L_802E62C8
    if (ctx->r2 != 0) {
        // 0x802E62B4: lw          $a0, 0x110($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X110);
            goto L_802E62C8;
    }
    // 0x802E62B4: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    // 0x802E62B8: jal         0x802E68EC
    // 0x802E62BC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    map_ovl_00_func_802E68EC(rdram, ctx);
        goto after_12;
    // 0x802E62BC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_12:
    // 0x802E62C0: beql        $v0, $zero, L_802E6304
    if (ctx->r2 == 0) {
        // 0x802E62C4: lw          $t3, 0xFC($sp)
        ctx->r11 = MEM_W(ctx->r29, 0XFC);
            goto L_802E6304;
    }
    goto skip_0;
    // 0x802E62C4: lw          $t3, 0xFC($sp)
    ctx->r11 = MEM_W(ctx->r29, 0XFC);
    skip_0:
L_802E62C8:
    // 0x802E62C8: lw          $t8, 0xFC($sp)
    ctx->r24 = MEM_W(ctx->r29, 0XFC);
    // 0x802E62CC: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x802E62D0: lhu         $t0, 0x18($t8)
    ctx->r8 = MEM_HU(ctx->r24, 0X18);
    // 0x802E62D4: bne         $t0, $zero, L_802E676C
    if (ctx->r8 != 0) {
        // 0x802E62D8: nop
    
            goto L_802E676C;
    }
    // 0x802E62D8: nop

    // 0x802E62DC: lw          $t1, -0x54E8($t1)
    ctx->r9 = MEM_W(ctx->r9, -0X54E8);
    // 0x802E62E0: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    // 0x802E62E4: andi        $t2, $t1, 0x40
    ctx->r10 = ctx->r9 & 0X40;
    // 0x802E62E8: bne         $t2, $zero, L_802E676C
    if (ctx->r10 != 0) {
        // 0x802E62EC: nop
    
            goto L_802E676C;
    }
    // 0x802E62EC: nop

    // 0x802E62F0: jal         0x802E68EC
    // 0x802E62F4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    map_ovl_00_func_802E68EC(rdram, ctx);
        goto after_13;
    // 0x802E62F4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_13:
    // 0x802E62F8: bne         $v0, $zero, L_802E676C
    if (ctx->r2 != 0) {
        // 0x802E62FC: nop
    
            goto L_802E676C;
    }
    // 0x802E62FC: nop

    // 0x802E6300: lw          $t3, 0xFC($sp)
    ctx->r11 = MEM_W(ctx->r29, 0XFC);
L_802E6304:
    // 0x802E6304: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x802E6308: lhu         $v0, 0x18($t3)
    ctx->r2 = MEM_HU(ctx->r11, 0X18);
    // 0x802E630C: bne         $v0, $zero, L_802E64BC
    if (ctx->r2 != 0) {
        // 0x802E6310: nop
    
            goto L_802E64BC;
    }
    // 0x802E6310: nop

    // 0x802E6314: lw          $t4, -0x54E8($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X54E8);
    // 0x802E6318: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x802E631C: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x802E6320: andi        $t5, $t4, 0x10
    ctx->r13 = ctx->r12 & 0X10;
    // 0x802E6324: beq         $t5, $zero, L_802E64BC
    if (ctx->r13 == 0) {
        // 0x802E6328: addiu       $fp, $sp, 0xC8
        ctx->r30 = ADD32(ctx->r29, 0XC8);
            goto L_802E64BC;
    }
    // 0x802E6328: addiu       $fp, $sp, 0xC8
    ctx->r30 = ADD32(ctx->r29, 0XC8);
    // 0x802E632C: lui         $s6, 0x802F
    ctx->r22 = S32(0X802F << 16);
    // 0x802E6330: mtc1        $at, $f22
    ctx->f22.u32l = ctx->r1;
    // 0x802E6334: addiu       $s6, $s6, -0x4830
    ctx->r22 = ADD32(ctx->r22, -0X4830);
    // 0x802E6338: addiu       $s5, $zero, 0xC
    ctx->r21 = ADD32(0, 0XC);
L_802E633C:
    // 0x802E633C: jalr        $s7
    // 0x802E6340: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_14;
    // 0x802E6340: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_14:
    // 0x802E6344: andi        $t6, $v0, 0x7
    ctx->r14 = ctx->r2 & 0X7;
    // 0x802E6348: multu       $t6, $s5
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x802E634C: sll         $s1, $s2, 2
    ctx->r17 = S32(ctx->r18 << 2);
    // 0x802E6350: addu        $s1, $s1, $s2
    ctx->r17 = ADD32(ctx->r17, ctx->r18);
    // 0x802E6354: mflo        $t7
    ctx->r15 = lo;
    // 0x802E6358: addu        $s0, $s6, $t7
    ctx->r16 = ADD32(ctx->r22, ctx->r15);
    // 0x802E635C: lhu         $t9, 0x2($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X2);
    // 0x802E6360: beql        $t9, $zero, L_802E6400
    if (ctx->r25 == 0) {
        // 0x802E6364: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_802E6400;
    }
    goto skip_1;
    // 0x802E6364: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_1:
    // 0x802E6368: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E636C: lui         $s3, 0x8016
    ctx->r19 = S32(0X8016 << 16);
    // 0x802E6370: addiu       $s3, $s3, 0x3958
    ctx->r19 = ADD32(ctx->r19, 0X3958);
    // 0x802E6374: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E6378: jal         0x800A3A50
    // 0x802E637C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_800A3A50(rdram, ctx);
        goto after_15;
    // 0x802E637C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_15:
    // 0x802E6380: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x802E6384: mtc1        $s1, $f4
    ctx->f4.u32l = ctx->r17;
    // 0x802E6388: lwc1        $f16, 0x50($s4)
    ctx->f16.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E638C: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x802E6390: cvt.s.w     $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    ctx->f20.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E6394: div.s       $f10, $f8, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = DIV_S(ctx->f8.fl, ctx->f22.fl);
    // 0x802E6398: mul.s       $f18, $f10, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f20.fl);
    // 0x802E639C: add.s       $f4, $f18, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f18.fl + ctx->f16.fl;
    // 0x802E63A0: swc1        $f4, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f4.u32l;
    // 0x802E63A4: lwc1        $f6, 0x54($s4)
    ctx->f6.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E63A8: swc1        $f6, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->f6.u32l;
    // 0x802E63AC: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E63B0: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E63B4: jal         0x80097330
    // 0x802E63B8: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_16;
    // 0x802E63B8: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_16:
    // 0x802E63BC: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x802E63C0: lwc1        $f8, 0x58($s4)
    ctx->f8.u32l = MEM_W(ctx->r20, 0X58);
    // 0x802E63C4: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    // 0x802E63C8: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E63CC: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    // 0x802E63D0: div.s       $f16, $f18, $f22
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f16.fl = DIV_S(ctx->f18.fl, ctx->f22.fl);
    // 0x802E63D4: mul.s       $f4, $f16, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x802E63D8: sub.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl - ctx->f4.fl;
    // 0x802E63DC: swc1        $f6, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f6.u32l;
    // 0x802E63E0: lw          $t8, 0x4($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4);
    // 0x802E63E4: lhu         $a3, 0x8($s0)
    ctx->r7 = MEM_HU(ctx->r16, 0X8);
    // 0x802E63E8: lhu         $a2, 0x2($s0)
    ctx->r6 = MEM_HU(ctx->r16, 0X2);
    // 0x802E63EC: jalr        $s3
    // 0x802E63F0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_17;
    // 0x802E63F0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    after_17:
    // 0x802E63F4: lw          $t0, 0x90($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X90);
    // 0x802E63F8: sw          $zero, 0x4($t0)
    MEM_W(0X4, ctx->r8) = 0;
    // 0x802E63FC: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_802E6400:
    // 0x802E6400: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E6404: bne         $s2, $at, L_802E633C
    if (ctx->r18 != ctx->r1) {
        // 0x802E6408: nop
    
            goto L_802E633C;
    }
    // 0x802E6408: nop

    // 0x802E640C: lhu         $t1, 0x62($s6)
    ctx->r9 = MEM_HU(ctx->r22, 0X62);
    // 0x802E6410: beql        $t1, $zero, L_802E675C
    if (ctx->r9 == 0) {
        // 0x802E6414: lw          $a0, 0x110($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X110);
            goto L_802E675C;
    }
    goto skip_2;
    // 0x802E6414: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    skip_2:
    // 0x802E6418: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E641C: lui         $s3, 0x8016
    ctx->r19 = S32(0X8016 << 16);
    // 0x802E6420: addiu       $s3, $s3, 0x3958
    ctx->r19 = ADD32(ctx->r19, 0X3958);
    // 0x802E6424: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E6428: jal         0x800A3A50
    // 0x802E642C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_800A3A50(rdram, ctx);
        goto after_18;
    // 0x802E642C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_18:
    // 0x802E6430: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x802E6434: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x802E6438: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x802E643C: cvt.s.w     $f18, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    ctx->f18.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E6440: lwc1        $f4, 0x50($s4)
    ctx->f4.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E6444: div.s       $f16, $f18, $f22
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f16.fl = DIV_S(ctx->f18.fl, ctx->f22.fl);
    // 0x802E6448: mul.s       $f8, $f16, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x802E644C: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x802E6450: swc1        $f6, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f6.u32l;
    // 0x802E6454: lwc1        $f10, 0x54($s4)
    ctx->f10.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E6458: swc1        $f10, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->f10.u32l;
    // 0x802E645C: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E6460: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E6464: jal         0x80097330
    // 0x802E6468: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_19;
    // 0x802E6468: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_19:
    // 0x802E646C: mtc1        $v0, $f16
    ctx->f16.u32l = ctx->r2;
    // 0x802E6470: lwc1        $f18, 0x58($s4)
    ctx->f18.u32l = MEM_W(ctx->r20, 0X58);
    // 0x802E6474: lui         $t2, 0x802F
    ctx->r10 = S32(0X802F << 16);
    // 0x802E6478: cvt.s.w     $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    ctx->f8.fl = CVT_S_W(ctx->f16.u32l);
    // 0x802E647C: lw          $t2, -0x47CC($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X47CC);
    // 0x802E6480: lui         $a2, 0x802F
    ctx->r6 = S32(0X802F << 16);
    // 0x802E6484: lui         $a3, 0x802F
    ctx->r7 = S32(0X802F << 16);
    // 0x802E6488: lhu         $a3, -0x47C8($a3)
    ctx->r7 = MEM_HU(ctx->r7, -0X47C8);
    // 0x802E648C: lhu         $a2, -0x47CE($a2)
    ctx->r6 = MEM_HU(ctx->r6, -0X47CE);
    // 0x802E6490: div.s       $f4, $f8, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = DIV_S(ctx->f8.fl, ctx->f22.fl);
    // 0x802E6494: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    // 0x802E6498: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    // 0x802E649C: sw          $t2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r10;
    // 0x802E64A0: mul.s       $f6, $f4, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f20.fl);
    // 0x802E64A4: sub.s       $f10, $f18, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f18.fl - ctx->f6.fl;
    // 0x802E64A8: jalr        $s3
    // 0x802E64AC: swc1        $f10, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f10.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_20;
    // 0x802E64AC: swc1        $f10, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f10.u32l;
    after_20:
    // 0x802E64B0: lw          $t3, 0x90($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X90);
    // 0x802E64B4: b           L_802E6758
    // 0x802E64B8: sw          $zero, 0x4($t3)
    MEM_W(0X4, ctx->r11) = 0;
        goto L_802E6758;
    // 0x802E64B8: sw          $zero, 0x4($t3)
    MEM_W(0X4, ctx->r11) = 0;
L_802E64BC:
    // 0x802E64BC: bne         $v0, $zero, L_802E666C
    if (ctx->r2 != 0) {
        // 0x802E64C0: lui         $t4, 0x801D
        ctx->r12 = S32(0X801D << 16);
            goto L_802E666C;
    }
    // 0x802E64C0: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x802E64C4: lw          $t4, -0x54E8($t4)
    ctx->r12 = MEM_W(ctx->r12, -0X54E8);
    // 0x802E64C8: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x802E64CC: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x802E64D0: andi        $t5, $t4, 0x20
    ctx->r13 = ctx->r12 & 0X20;
    // 0x802E64D4: beq         $t5, $zero, L_802E666C
    if (ctx->r13 == 0) {
        // 0x802E64D8: addiu       $fp, $sp, 0xC8
        ctx->r30 = ADD32(ctx->r29, 0XC8);
            goto L_802E666C;
    }
    // 0x802E64D8: addiu       $fp, $sp, 0xC8
    ctx->r30 = ADD32(ctx->r29, 0XC8);
    // 0x802E64DC: lui         $s6, 0x802F
    ctx->r22 = S32(0X802F << 16);
    // 0x802E64E0: mtc1        $at, $f22
    ctx->f22.u32l = ctx->r1;
    // 0x802E64E4: addiu       $s6, $s6, -0x4830
    ctx->r22 = ADD32(ctx->r22, -0X4830);
    // 0x802E64E8: addiu       $s5, $zero, 0xC
    ctx->r21 = ADD32(0, 0XC);
L_802E64EC:
    // 0x802E64EC: jalr        $s7
    // 0x802E64F0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_21;
    // 0x802E64F0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_21:
    // 0x802E64F4: andi        $t6, $v0, 0x7
    ctx->r14 = ctx->r2 & 0X7;
    // 0x802E64F8: multu       $t6, $s5
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x802E64FC: sll         $s1, $s2, 2
    ctx->r17 = S32(ctx->r18 << 2);
    // 0x802E6500: addu        $s1, $s1, $s2
    ctx->r17 = ADD32(ctx->r17, ctx->r18);
    // 0x802E6504: mflo        $t7
    ctx->r15 = lo;
    // 0x802E6508: addu        $s0, $s6, $t7
    ctx->r16 = ADD32(ctx->r22, ctx->r15);
    // 0x802E650C: lhu         $t9, 0x2($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X2);
    // 0x802E6510: beql        $t9, $zero, L_802E65B0
    if (ctx->r25 == 0) {
        // 0x802E6514: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_802E65B0;
    }
    goto skip_3;
    // 0x802E6514: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_3:
    // 0x802E6518: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E651C: lui         $s3, 0x8016
    ctx->r19 = S32(0X8016 << 16);
    // 0x802E6520: addiu       $s3, $s3, 0x3958
    ctx->r19 = ADD32(ctx->r19, 0X3958);
    // 0x802E6524: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E6528: jal         0x800A3A50
    // 0x802E652C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_800A3A50(rdram, ctx);
        goto after_22;
    // 0x802E652C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_22:
    // 0x802E6530: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x802E6534: mtc1        $s1, $f16
    ctx->f16.u32l = ctx->r17;
    // 0x802E6538: lwc1        $f10, 0x50($s4)
    ctx->f10.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E653C: cvt.s.w     $f4, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    ctx->f4.fl = CVT_S_W(ctx->f8.u32l);
    // 0x802E6540: cvt.s.w     $f20, $f16
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 16);
    ctx->f20.fl = CVT_S_W(ctx->f16.u32l);
    // 0x802E6544: div.s       $f18, $f4, $f22
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f18.fl = DIV_S(ctx->f4.fl, ctx->f22.fl);
    // 0x802E6548: mul.s       $f6, $f18, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f20.fl);
    // 0x802E654C: add.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f6.fl + ctx->f10.fl;
    // 0x802E6550: swc1        $f16, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f16.u32l;
    // 0x802E6554: lwc1        $f8, 0x54($s4)
    ctx->f8.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E6558: swc1        $f8, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->f8.u32l;
    // 0x802E655C: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E6560: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E6564: jal         0x80097330
    // 0x802E6568: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_23;
    // 0x802E6568: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_23:
    // 0x802E656C: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x802E6570: lwc1        $f4, 0x58($s4)
    ctx->f4.u32l = MEM_W(ctx->r20, 0X58);
    // 0x802E6574: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    // 0x802E6578: cvt.s.w     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    ctx->f6.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E657C: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    // 0x802E6580: div.s       $f10, $f6, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f22.fl);
    // 0x802E6584: mul.s       $f16, $f10, $f20
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f20.fl);
    // 0x802E6588: sub.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f16.fl;
    // 0x802E658C: swc1        $f8, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f8.u32l;
    // 0x802E6590: lw          $t8, 0x4($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X4);
    // 0x802E6594: lhu         $a3, 0x8($s0)
    ctx->r7 = MEM_HU(ctx->r16, 0X8);
    // 0x802E6598: lhu         $a2, 0x2($s0)
    ctx->r6 = MEM_HU(ctx->r16, 0X2);
    // 0x802E659C: jalr        $s3
    // 0x802E65A0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_24;
    // 0x802E65A0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    after_24:
    // 0x802E65A4: lw          $t0, 0x90($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X90);
    // 0x802E65A8: sw          $zero, 0x4($t0)
    MEM_W(0X4, ctx->r8) = 0;
    // 0x802E65AC: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_802E65B0:
    // 0x802E65B0: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E65B4: bne         $s2, $at, L_802E64EC
    if (ctx->r18 != ctx->r1) {
        // 0x802E65B8: nop
    
            goto L_802E64EC;
    }
    // 0x802E65B8: nop

    // 0x802E65BC: lhu         $t1, 0x4A($s6)
    ctx->r9 = MEM_HU(ctx->r22, 0X4A);
    // 0x802E65C0: beql        $t1, $zero, L_802E675C
    if (ctx->r9 == 0) {
        // 0x802E65C4: lw          $a0, 0x110($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X110);
            goto L_802E675C;
    }
    goto skip_4;
    // 0x802E65C4: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    skip_4:
    // 0x802E65C8: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E65CC: lui         $s3, 0x8016
    ctx->r19 = S32(0X8016 << 16);
    // 0x802E65D0: addiu       $s3, $s3, 0x3958
    ctx->r19 = ADD32(ctx->r19, 0X3958);
    // 0x802E65D4: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E65D8: jal         0x800A3A50
    // 0x802E65DC: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_800A3A50(rdram, ctx);
        goto after_25;
    // 0x802E65DC: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_25:
    // 0x802E65E0: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x802E65E4: lui         $at, 0x4120
    ctx->r1 = S32(0X4120 << 16);
    // 0x802E65E8: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x802E65EC: cvt.s.w     $f6, $f18
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    ctx->f6.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E65F0: lwc1        $f16, 0x50($s4)
    ctx->f16.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E65F4: div.s       $f10, $f6, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f22.fl);
    // 0x802E65F8: mul.s       $f4, $f10, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f20.fl);
    // 0x802E65FC: add.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x802E6600: swc1        $f8, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f8.u32l;
    // 0x802E6604: lwc1        $f18, 0x54($s4)
    ctx->f18.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E6608: swc1        $f18, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->f18.u32l;
    // 0x802E660C: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E6610: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E6614: jal         0x80097330
    // 0x802E6618: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_26;
    // 0x802E6618: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_26:
    // 0x802E661C: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x802E6620: lwc1        $f6, 0x58($s4)
    ctx->f6.u32l = MEM_W(ctx->r20, 0X58);
    // 0x802E6624: lui         $t2, 0x802F
    ctx->r10 = S32(0X802F << 16);
    // 0x802E6628: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E662C: lw          $t2, -0x47E4($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X47E4);
    // 0x802E6630: lui         $a2, 0x802F
    ctx->r6 = S32(0X802F << 16);
    // 0x802E6634: lui         $a3, 0x802F
    ctx->r7 = S32(0X802F << 16);
    // 0x802E6638: lhu         $a3, -0x47E0($a3)
    ctx->r7 = MEM_HU(ctx->r7, -0X47E0);
    // 0x802E663C: lhu         $a2, -0x47E6($a2)
    ctx->r6 = MEM_HU(ctx->r6, -0X47E6);
    // 0x802E6640: div.s       $f16, $f4, $f22
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f16.fl = DIV_S(ctx->f4.fl, ctx->f22.fl);
    // 0x802E6644: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    // 0x802E6648: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    // 0x802E664C: sw          $t2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r10;
    // 0x802E6650: mul.s       $f8, $f16, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x802E6654: sub.s       $f18, $f6, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x802E6658: jalr        $s3
    // 0x802E665C: swc1        $f18, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f18.u32l;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_27;
    // 0x802E665C: swc1        $f18, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f18.u32l;
    after_27:
    // 0x802E6660: lw          $t3, 0x90($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X90);
    // 0x802E6664: b           L_802E6758
    // 0x802E6668: sw          $zero, 0x4($t3)
    MEM_W(0X4, ctx->r11) = 0;
        goto L_802E6758;
    // 0x802E6668: sw          $zero, 0x4($t3)
    MEM_W(0X4, ctx->r11) = 0;
L_802E666C:
    // 0x802E666C: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x802E6670: lui         $s6, 0x802F
    ctx->r22 = S32(0X802F << 16);
    // 0x802E6674: mtc1        $at, $f22
    ctx->f22.u32l = ctx->r1;
    // 0x802E6678: addiu       $s6, $s6, -0x4830
    ctx->r22 = ADD32(ctx->r22, -0X4830);
    // 0x802E667C: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x802E6680: addiu       $fp, $sp, 0xC8
    ctx->r30 = ADD32(ctx->r29, 0XC8);
    // 0x802E6684: addiu       $s5, $zero, 0xC
    ctx->r21 = ADD32(0, 0XC);
L_802E6688:
    // 0x802E6688: jalr        $s7
    // 0x802E668C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    LOOKUP_FUNC(ctx->r23)(rdram, ctx);
        goto after_28;
    // 0x802E668C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_28:
    // 0x802E6690: andi        $t4, $v0, 0x7
    ctx->r12 = ctx->r2 & 0X7;
    // 0x802E6694: multu       $t4, $s5
    result = U64(U32(ctx->r12)) * U64(U32(ctx->r21)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x802E6698: sll         $s1, $s2, 2
    ctx->r17 = S32(ctx->r18 << 2);
    // 0x802E669C: addu        $s1, $s1, $s2
    ctx->r17 = ADD32(ctx->r17, ctx->r18);
    // 0x802E66A0: mflo        $t5
    ctx->r13 = lo;
    // 0x802E66A4: addu        $s0, $s6, $t5
    ctx->r16 = ADD32(ctx->r22, ctx->r13);
    // 0x802E66A8: lhu         $t6, 0x2($s0)
    ctx->r14 = MEM_HU(ctx->r16, 0X2);
    // 0x802E66AC: beql        $t6, $zero, L_802E674C
    if (ctx->r14 == 0) {
        // 0x802E66B0: addiu       $s2, $s2, 0x1
        ctx->r18 = ADD32(ctx->r18, 0X1);
            goto L_802E674C;
    }
    goto skip_5;
    // 0x802E66B0: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    skip_5:
    // 0x802E66B4: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E66B8: lui         $s3, 0x8016
    ctx->r19 = S32(0X8016 << 16);
    // 0x802E66BC: addiu       $s3, $s3, 0x3958
    ctx->r19 = ADD32(ctx->r19, 0X3958);
    // 0x802E66C0: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E66C4: jal         0x800A3A50
    // 0x802E66C8: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_800A3A50(rdram, ctx);
        goto after_29;
    // 0x802E66C8: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_29:
    // 0x802E66CC: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x802E66D0: mtc1        $s1, $f10
    ctx->f10.u32l = ctx->r17;
    // 0x802E66D4: lwc1        $f18, 0x50($s4)
    ctx->f18.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E66D8: cvt.s.w     $f16, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    ctx->f16.fl = CVT_S_W(ctx->f4.u32l);
    // 0x802E66DC: cvt.s.w     $f20, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    ctx->f20.fl = CVT_S_W(ctx->f10.u32l);
    // 0x802E66E0: div.s       $f6, $f16, $f22
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f6.fl = DIV_S(ctx->f16.fl, ctx->f22.fl);
    // 0x802E66E4: mul.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f20.fl);
    // 0x802E66E8: add.s       $f10, $f8, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x802E66EC: swc1        $f10, 0xC8($sp)
    MEM_W(0XC8, ctx->r29) = ctx->f10.u32l;
    // 0x802E66F0: lwc1        $f4, 0x54($s4)
    ctx->f4.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E66F4: swc1        $f4, 0xCC($sp)
    MEM_W(0XCC, ctx->r29) = ctx->f4.u32l;
    // 0x802E66F8: lh          $a0, 0x5E($s4)
    ctx->r4 = MEM_H(ctx->r20, 0X5E);
    // 0x802E66FC: addiu       $a0, $a0, -0x4000
    ctx->r4 = ADD32(ctx->r4, -0X4000);
    // 0x802E6700: jal         0x80097330
    // 0x802E6704: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_30;
    // 0x802E6704: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_30:
    // 0x802E6708: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x802E670C: lwc1        $f16, 0x58($s4)
    ctx->f16.u32l = MEM_W(ctx->r20, 0X58);
    // 0x802E6710: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
    // 0x802E6714: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x802E6718: or          $a1, $fp, $zero
    ctx->r5 = ctx->r30 | 0;
    // 0x802E671C: div.s       $f18, $f8, $f22
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f18.fl = DIV_S(ctx->f8.fl, ctx->f22.fl);
    // 0x802E6720: mul.s       $f10, $f18, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f20.fl);
    // 0x802E6724: sub.s       $f4, $f16, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f16.fl - ctx->f10.fl;
    // 0x802E6728: swc1        $f4, 0xD0($sp)
    MEM_W(0XD0, ctx->r29) = ctx->f4.u32l;
    // 0x802E672C: lw          $t7, 0x4($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X4);
    // 0x802E6730: lhu         $a3, 0x8($s0)
    ctx->r7 = MEM_HU(ctx->r16, 0X8);
    // 0x802E6734: lhu         $a2, 0x2($s0)
    ctx->r6 = MEM_HU(ctx->r16, 0X2);
    // 0x802E6738: jalr        $s3
    // 0x802E673C: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_31;
    // 0x802E673C: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    after_31:
    // 0x802E6740: lw          $t9, 0x90($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X90);
    // 0x802E6744: sw          $zero, 0x4($t9)
    MEM_W(0X4, ctx->r25) = 0;
    // 0x802E6748: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
L_802E674C:
    // 0x802E674C: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x802E6750: bne         $s2, $at, L_802E6688
    if (ctx->r18 != ctx->r1) {
        // 0x802E6754: nop
    
            goto L_802E6688;
    }
    // 0x802E6754: nop

L_802E6758:
    // 0x802E6758: lw          $a0, 0x110($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X110);
L_802E675C:
    // 0x802E675C: jal         0x802E68EC
    // 0x802E6760: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    map_ovl_00_func_802E68EC(rdram, ctx);
        goto after_32;
    // 0x802E6760: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_32:
    // 0x802E6764: b           L_802E6894
    // 0x802E6768: lw          $v0, 0x110($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X110);
        goto L_802E6894;
    // 0x802E6768: lw          $v0, 0x110($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X110);
L_802E676C:
    // 0x802E676C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E6770: lw          $a0, -0x540C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X540C);
    // 0x802E6774: jal         0x80002410
    // 0x802E6778: addiu       $a1, $zero, 0x20E4
    ctx->r5 = ADD32(0, 0X20E4);
    object_createAndSetChild(rdram, ctx);
        goto after_33;
    // 0x802E6778: addiu       $a1, $zero, 0x20E4
    ctx->r5 = ADD32(0, 0X20E4);
    after_33:
    // 0x802E677C: lw          $t8, 0x90($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X90);
    // 0x802E6780: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x802E6784: beq         $v0, $zero, L_802E6890
    if (ctx->r2 == 0) {
        // 0x802E6788: sw          $v0, 0x4($t8)
        MEM_W(0X4, ctx->r24) = ctx->r2;
            goto L_802E6890;
    }
    // 0x802E6788: sw          $v0, 0x4($t8)
    MEM_W(0X4, ctx->r24) = ctx->r2;
    // 0x802E678C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6790: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x802E6794: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x802E6798: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x802E679C: addiu       $a2, $zero, 0x20
    ctx->r6 = ADD32(0, 0X20);
    // 0x802E67A0: jalr        $t9
    // 0x802E67A4: addiu       $a3, $zero, 0xF
    ctx->r7 = ADD32(0, 0XF);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_34;
    // 0x802E67A4: addiu       $a3, $zero, 0xF
    ctx->r7 = ADD32(0, 0XF);
    after_34:
    // 0x802E67A8: bne         $v0, $zero, L_802E67C8
    if (ctx->r2 != 0) {
        // 0x802E67AC: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_802E67C8;
    }
    // 0x802E67AC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x802E67B0: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x802E67B4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x802E67B8: jalr        $t9
    // 0x802E67BC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_35;
    // 0x802E67BC: nop

    after_35:
    // 0x802E67C0: b           L_802E6894
    // 0x802E67C4: lw          $v0, 0x110($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X110);
        goto L_802E6894;
    // 0x802E67C4: lw          $v0, 0x110($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X110);
L_802E67C8:
    // 0x802E67C8: addiu       $t0, $zero, 0x10
    ctx->r8 = ADD32(0, 0X10);
    // 0x802E67CC: sh          $t0, 0x18($s0)
    MEM_H(0X18, ctx->r16) = ctx->r8;
    // 0x802E67D0: lw          $t9, 0x88($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X88);
    // 0x802E67D4: lw          $t1, 0xFC($sp)
    ctx->r9 = MEM_W(ctx->r29, 0XFC);
    // 0x802E67D8: jalr        $t9
    // 0x802E67DC: lhu         $a0, 0x16($t1)
    ctx->r4 = MEM_HU(ctx->r9, 0X16);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_36;
    // 0x802E67DC: lhu         $a0, 0x16($t1)
    ctx->r4 = MEM_HU(ctx->r9, 0X16);
    after_36:
    // 0x802E67E0: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x802E67E4: lui         $at, 0x4021
    ctx->r1 = S32(0X4021 << 16);
    // 0x802E67E8: mtc1        $at, $f21
    ctx->f_odd[(21 - 1) * 2] = ctx->r1;
    // 0x802E67EC: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x802E67F0: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x802E67F4: mtc1        $at, $f22
    ctx->f22.u32l = ctx->r1;
    // 0x802E67F8: mtc1        $zero, $f20
    ctx->f20.u32l = 0;
    // 0x802E67FC: lwc1        $f4, 0x50($s4)
    ctx->f4.u32l = MEM_W(ctx->r20, 0X50);
    // 0x802E6800: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x802E6804: div.s       $f18, $f8, $f22
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f18.fl = DIV_S(ctx->f8.fl, ctx->f22.fl);
    // 0x802E6808: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x802E680C: cvt.d.s     $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f16.d = CVT_D_S(ctx->f18.fl);
    // 0x802E6810: mul.d       $f10, $f16, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f20.d); 
    ctx->f10.d = MUL_D(ctx->f16.d, ctx->f20.d);
    // 0x802E6814: add.d       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = ctx->f10.d + ctx->f6.d;
    // 0x802E6818: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x802E681C: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x802E6820: cvt.s.d     $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f18.fl = CVT_S_D(ctx->f8.d);
    // 0x802E6824: swc1        $f18, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f18.u32l;
    // 0x802E6828: lwc1        $f16, 0x54($s4)
    ctx->f16.u32l = MEM_W(ctx->r20, 0X54);
    // 0x802E682C: cvt.d.s     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f4.d = CVT_D_S(ctx->f16.fl);
    // 0x802E6830: sub.d       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f10.d); 
    ctx->f6.d = ctx->f4.d - ctx->f10.d;
    // 0x802E6834: cvt.s.d     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f8.fl = CVT_S_D(ctx->f6.d);
    // 0x802E6838: swc1        $f8, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f8.u32l;
    // 0x802E683C: lw          $t9, 0x84($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X84);
    // 0x802E6840: lw          $t2, 0xFC($sp)
    ctx->r10 = MEM_W(ctx->r29, 0XFC);
    // 0x802E6844: jalr        $t9
    // 0x802E6848: lhu         $a0, 0x16($t2)
    ctx->r4 = MEM_HU(ctx->r10, 0X16);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_37;
    // 0x802E6848: lhu         $a0, 0x16($t2)
    ctx->r4 = MEM_HU(ctx->r10, 0X16);
    after_37:
    // 0x802E684C: mtc1        $v0, $f18
    ctx->f18.u32l = ctx->r2;
    // 0x802E6850: lwc1        $f8, 0x58($s4)
    ctx->f8.u32l = MEM_W(ctx->r20, 0X58);
    // 0x802E6854: sh          $zero, 0x14($s0)
    MEM_H(0X14, ctx->r16) = 0;
    // 0x802E6858: cvt.s.w     $f16, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    ctx->f16.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E685C: cvt.d.s     $f18, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f18.d = CVT_D_S(ctx->f8.fl);
    // 0x802E6860: div.s       $f4, $f16, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = DIV_S(ctx->f16.fl, ctx->f22.fl);
    // 0x802E6864: cvt.d.s     $f10, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f10.d = CVT_D_S(ctx->f4.fl);
    // 0x802E6868: mul.d       $f6, $f10, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f20.d); 
    ctx->f6.d = MUL_D(ctx->f10.d, ctx->f20.d);
    // 0x802E686C: add.d       $f16, $f6, $f18
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f18.d); 
    ctx->f16.d = ctx->f6.d + ctx->f18.d;
    // 0x802E6870: cvt.s.d     $f4, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f4.fl = CVT_S_D(ctx->f16.d);
    // 0x802E6874: swc1        $f4, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f4.u32l;
    // 0x802E6878: lw          $t3, 0xFC($sp)
    ctx->r11 = MEM_W(ctx->r29, 0XFC);
    // 0x802E687C: lhu         $t4, 0x16($t3)
    ctx->r12 = MEM_HU(ctx->r11, 0X16);
    // 0x802E6880: sh          $zero, 0x2($s0)
    MEM_H(0X2, ctx->r16) = 0;
    // 0x802E6884: addiu       $t6, $t4, 0x4000
    ctx->r14 = ADD32(ctx->r12, 0X4000);
    // 0x802E6888: sh          $t6, 0x16($s0)
    MEM_H(0X16, ctx->r16) = ctx->r14;
    // 0x802E688C: sw          $s0, 0x70($s1)
    MEM_W(0X70, ctx->r17) = ctx->r16;
L_802E6890:
    // 0x802E6890: lw          $v0, 0x110($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X110);
L_802E6894:
    // 0x802E6894: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6898: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E689C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x802E68A0: jalr        $t9
    // 0x802E68A4: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_38;
    // 0x802E68A4: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_38:
L_802E68A8:
    // 0x802E68A8: lw          $ra, 0x6C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X6C);
    // 0x802E68AC: ldc1        $f20, 0x20($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X20);
    // 0x802E68B0: ldc1        $f22, 0x28($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X28);
    // 0x802E68B4: ldc1        $f24, 0x30($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X30);
    // 0x802E68B8: ldc1        $f26, 0x38($sp)
    CHECK_FR(ctx, 26);
    ctx->f26.u64 = LD(ctx->r29, 0X38);
    // 0x802E68BC: ldc1        $f28, 0x40($sp)
    CHECK_FR(ctx, 28);
    ctx->f28.u64 = LD(ctx->r29, 0X40);
    // 0x802E68C0: lw          $s0, 0x48($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X48);
    // 0x802E68C4: lw          $s1, 0x4C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X4C);
    // 0x802E68C8: lw          $s2, 0x50($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X50);
    // 0x802E68CC: lw          $s3, 0x54($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X54);
    // 0x802E68D0: lw          $s4, 0x58($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X58);
    // 0x802E68D4: lw          $s5, 0x5C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X5C);
    // 0x802E68D8: lw          $s6, 0x60($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X60);
    // 0x802E68DC: lw          $s7, 0x64($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X64);
    // 0x802E68E0: lw          $fp, 0x68($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X68);
    // 0x802E68E4: jr          $ra
    // 0x802E68E8: addiu       $sp, $sp, 0x110
    ctx->r29 = ADD32(ctx->r29, 0X110);
    return;
    // 0x802E68E8: addiu       $sp, $sp, 0x110
    ctx->r29 = ADD32(ctx->r29, 0X110);
;}
RECOMP_FUNC void map_ovl_00_func_802E68EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E68EC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E68F0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E68F4: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x802E68F8: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x802E68FC: sll         $a2, $a1, 16
    ctx->r6 = S32(ctx->r5 << 16);
    // 0x802E6900: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x802E6904: lhu         $t6, 0x18($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X18);
    // 0x802E6908: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x802E690C: sltiu       $at, $t6, 0x5
    ctx->r1 = ctx->r14 < 0X5 ? 1 : 0;
    // 0x802E6910: beq         $at, $zero, L_802E6954
    if (ctx->r1 == 0) {
        // 0x802E6914: sll         $t6, $t6, 2
        ctx->r14 = S32(ctx->r14 << 2);
            goto L_802E6954;
    }
    // 0x802E6914: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x802E6918: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E691C: addu        $at, $at, $t6
    gpr jr_addend_802E6924 = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x802E6920: lw          $t6, -0x3D78($at)
    ctx->r14 = ADD32(ctx->r1, -0X3D78);
    // 0x802E6924: jr          $t6
    // 0x802E6928: nop

    switch (jr_addend_802E6924 >> 2) {
        case 0: goto L_802E692C; break;
        case 1: goto L_802E6934; break;
        case 2: goto L_802E693C; break;
        case 3: goto L_802E6944; break;
        case 4: goto L_802E694C; break;
        default: switch_error(__func__, 0x802E6924, 0x802EC288);
    }
    // 0x802E6928: nop

L_802E692C:
    // 0x802E692C: b           L_802E6954
    // 0x802E6930: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
        goto L_802E6954;
    // 0x802E6930: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
L_802E6934:
    // 0x802E6934: b           L_802E6954
    // 0x802E6938: addiu       $a1, $zero, 0x11
    ctx->r5 = ADD32(0, 0X11);
        goto L_802E6954;
    // 0x802E6938: addiu       $a1, $zero, 0x11
    ctx->r5 = ADD32(0, 0X11);
L_802E693C:
    // 0x802E693C: b           L_802E6954
    // 0x802E6940: addiu       $a1, $zero, 0x12
    ctx->r5 = ADD32(0, 0X12);
        goto L_802E6954;
    // 0x802E6940: addiu       $a1, $zero, 0x12
    ctx->r5 = ADD32(0, 0X12);
L_802E6944:
    // 0x802E6944: b           L_802E6954
    // 0x802E6948: addiu       $a1, $zero, 0x13
    ctx->r5 = ADD32(0, 0X13);
        goto L_802E6954;
    // 0x802E6948: addiu       $a1, $zero, 0x13
    ctx->r5 = ADD32(0, 0X13);
L_802E694C:
    // 0x802E694C: b           L_802E6954
    // 0x802E6950: addiu       $a1, $zero, 0x14
    ctx->r5 = ADD32(0, 0X14);
        goto L_802E6954;
    // 0x802E6950: addiu       $a1, $zero, 0x14
    ctx->r5 = ADD32(0, 0X14);
L_802E6954:
    // 0x802E6954: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x802E6958: beq         $a2, $at, L_802E6974
    if (ctx->r6 == ctx->r1) {
        // 0x802E695C: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_802E6974;
    }
    // 0x802E695C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E6960: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E6964: beq         $a2, $at, L_802E698C
    if (ctx->r6 == ctx->r1) {
        // 0x802E6968: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_802E698C;
    }
    // 0x802E6968: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E696C: b           L_802E69A4
    // 0x802E6970: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
        goto L_802E69A4;
    // 0x802E6970: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_802E6974:
    // 0x802E6974: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6978: addiu       $t9, $t9, 0x48C4
    ctx->r25 = ADD32(ctx->r25, 0X48C4);
    // 0x802E697C: jalr        $t9
    // 0x802E6980: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6980: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_0:
    // 0x802E6984: b           L_802E69D8
    // 0x802E6988: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_802E69D8;
    // 0x802E6988: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_802E698C:
    // 0x802E698C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6990: addiu       $t9, $t9, 0x48EC
    ctx->r25 = ADD32(ctx->r25, 0X48EC);
    // 0x802E6994: jalr        $t9
    // 0x802E6998: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E6998: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_1:
    // 0x802E699C: b           L_802E69D8
    // 0x802E69A0: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_802E69D8;
    // 0x802E69A0: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
L_802E69A4:
    // 0x802E69A4: beq         $a1, $zero, L_802E69C4
    if (ctx->r5 == 0) {
        // 0x802E69A8: lui         $a0, 0x801D
        ctx->r4 = S32(0X801D << 16);
            goto L_802E69C4;
    }
    // 0x802E69A8: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E69AC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E69B0: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E69B4: jalr        $t9
    // 0x802E69B8: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E69B8: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_2:
    // 0x802E69BC: b           L_802E69C4
    // 0x802E69C0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
        goto L_802E69C4;
    // 0x802E69C0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_802E69C4:
    // 0x802E69C4: beql        $v1, $zero, L_802E69D8
    if (ctx->r3 == 0) {
        // 0x802E69C8: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_802E69D8;
    }
    goto skip_0;
    // 0x802E69C8: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    skip_0:
    // 0x802E69CC: b           L_802E69D8
    // 0x802E69D0: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
        goto L_802E69D8;
    // 0x802E69D0: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x802E69D4: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_802E69D8:
    // 0x802E69D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E69DC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E69E0: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x802E69E4: jr          $ra
    // 0x802E69E8: nop

    return;
    // 0x802E69E8: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E69EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E69EC: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x802E69F0: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x802E69F4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x802E69F8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x802E69FC: lwc1        $f4, 0x8($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X8);
    // 0x802E6A00: lui         $at, 0x802F
    ctx->r1 = S32(0X802F << 16);
    // 0x802E6A04: lwc1        $f10, -0x3D64($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X3D64);
    // 0x802E6A08: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x802E6A0C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x802E6A10: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E6A14: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E6A18: lw          $a2, 0x34($a0)
    ctx->r6 = MEM_W(ctx->r4, 0X34);
    // 0x802E6A1C: lw          $a3, 0x70($a0)
    ctx->r7 = MEM_W(ctx->r4, 0X70);
    // 0x802E6A20: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x802E6A24: div.s       $f16, $f10, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f8.fl);
    // 0x802E6A28: lw          $t6, 0x14($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X14);
    // 0x802E6A2C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E6A30: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x802E6A34: mtc1        $t6, $f18
    ctx->f18.u32l = ctx->r14;
    // 0x802E6A38: nop

    // 0x802E6A3C: cvt.s.w     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    ctx->f4.fl = CVT_S_W(ctx->f18.u32l);
    // 0x802E6A40: sub.s       $f6, $f4, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f16.fl;
    // 0x802E6A44: trunc.w.s   $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    ctx->f10.u32l = TRUNC_W_S(ctx->f6.fl);
    // 0x802E6A48: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    // 0x802E6A4C: nop

    // 0x802E6A50: bgez        $t8, L_802E6B58
    if (SIGNED(ctx->r24) >= 0) {
        // 0x802E6A54: sw          $t8, 0x14($a2)
        MEM_W(0X14, ctx->r6) = ctx->r24;
            goto L_802E6B58;
    }
    // 0x802E6A54: sw          $t8, 0x14($a2)
    MEM_W(0X14, ctx->r6) = ctx->r24;
    // 0x802E6A58: lh          $t0, 0x0($v0)
    ctx->r8 = MEM_H(ctx->r2, 0X0);
    // 0x802E6A5C: addiu       $at, $zero, -0x8000
    ctx->r1 = ADD32(0, -0X8000);
    // 0x802E6A60: or          $t1, $t0, $at
    ctx->r9 = ctx->r8 | ctx->r1;
    // 0x802E6A64: sh          $t1, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r9;
    // 0x802E6A68: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    // 0x802E6A6C: jal         0x801469F4
    // 0x802E6A70: sw          $a2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r6;
    func_801469F4(rdram, ctx);
        goto after_0;
    // 0x802E6A70: sw          $a2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r6;
    after_0:
    // 0x802E6A74: lw          $a2, 0x2C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X2C);
    // 0x802E6A78: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x802E6A7C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E6A80: addiu       $a1, $zero, 0x27
    ctx->r5 = ADD32(0, 0X27);
    // 0x802E6A84: sw          $a2, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r6;
    // 0x802E6A88: jal         0x80002410
    // 0x802E6A8C: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    object_createAndSetChild(rdram, ctx);
        goto after_1;
    // 0x802E6A8C: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    after_1:
    // 0x802E6A90: lw          $a2, 0x2C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X2C);
    // 0x802E6A94: beq         $v0, $zero, L_802E6B1C
    if (ctx->r2 == 0) {
        // 0x802E6A98: lw          $a3, 0x20($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X20);
            goto L_802E6B1C;
    }
    // 0x802E6A98: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x802E6A9C: lui         $t3, 0x802F
    ctx->r11 = S32(0X802F << 16);
    // 0x802E6AA0: addiu       $t3, $t3, -0x4850
    ctx->r11 = ADD32(ctx->r11, -0X4850);
    // 0x802E6AA4: addiu       $v1, $a2, 0x8
    ctx->r3 = ADD32(ctx->r6, 0X8);
    // 0x802E6AA8: lw          $t2, 0x0($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X0);
    // 0x802E6AAC: lw          $t5, 0x0($t3)
    ctx->r13 = MEM_W(ctx->r11, 0X0);
    // 0x802E6AB0: sw          $t5, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->r13;
    // 0x802E6AB4: lw          $t4, 0x4($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X4);
    // 0x802E6AB8: sw          $t4, 0x4($t2)
    MEM_W(0X4, ctx->r10) = ctx->r12;
    // 0x802E6ABC: lw          $t5, 0x8($t3)
    ctx->r13 = MEM_W(ctx->r11, 0X8);
    // 0x802E6AC0: sw          $t5, 0x8($t2)
    MEM_W(0X8, ctx->r10) = ctx->r13;
    // 0x802E6AC4: lw          $t4, 0xC($t3)
    ctx->r12 = MEM_W(ctx->r11, 0XC);
    // 0x802E6AC8: sw          $t4, 0xC($t2)
    MEM_W(0XC, ctx->r10) = ctx->r12;
    // 0x802E6ACC: lw          $t5, 0x10($t3)
    ctx->r13 = MEM_W(ctx->r11, 0X10);
    // 0x802E6AD0: sw          $t5, 0x10($t2)
    MEM_W(0X10, ctx->r10) = ctx->r13;
    // 0x802E6AD4: lw          $t4, 0x14($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X14);
    // 0x802E6AD8: sw          $t4, 0x14($t2)
    MEM_W(0X14, ctx->r10) = ctx->r12;
    // 0x802E6ADC: lw          $t5, 0x18($t3)
    ctx->r13 = MEM_W(ctx->r11, 0X18);
    // 0x802E6AE0: sw          $t5, 0x18($t2)
    MEM_W(0X18, ctx->r10) = ctx->r13;
    // 0x802E6AE4: lw          $t4, 0x1C($t3)
    ctx->r12 = MEM_W(ctx->r11, 0X1C);
    // 0x802E6AE8: sw          $t4, 0x1C($t2)
    MEM_W(0X1C, ctx->r10) = ctx->r12;
    // 0x802E6AEC: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x802E6AF0: lwc1        $f8, 0x4($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X4);
    // 0x802E6AF4: swc1        $f8, 0x4($t6)
    MEM_W(0X4, ctx->r14) = ctx->f8.u32l;
    // 0x802E6AF8: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x802E6AFC: lwc1        $f18, 0x8($a3)
    ctx->f18.u32l = MEM_W(ctx->r7, 0X8);
    // 0x802E6B00: swc1        $f18, 0x8($t7)
    MEM_W(0X8, ctx->r15) = ctx->f18.u32l;
    // 0x802E6B04: lw          $t8, 0x0($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X0);
    // 0x802E6B08: lwc1        $f4, 0xC($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0XC);
    // 0x802E6B0C: swc1        $f4, 0xC($t8)
    MEM_W(0XC, ctx->r24) = ctx->f4.u32l;
    // 0x802E6B10: lw          $t9, 0x0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X0);
    // 0x802E6B14: b           L_802E6B20
    // 0x802E6B18: sw          $t9, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->r25;
        goto L_802E6B20;
    // 0x802E6B18: sw          $t9, 0x70($v0)
    MEM_W(0X70, ctx->r2) = ctx->r25;
L_802E6B1C:
    // 0x802E6B1C: addiu       $v1, $a2, 0x8
    ctx->r3 = ADD32(ctx->r6, 0X8);
L_802E6B20:
    // 0x802E6B20: lw          $a0, 0x4($v1)
    ctx->r4 = MEM_W(ctx->r3, 0X4);
    // 0x802E6B24: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E6B28: addiu       $t9, $t9, 0x5800
    ctx->r25 = ADD32(ctx->r25, 0X5800);
    // 0x802E6B2C: beq         $a0, $zero, L_802E6B3C
    if (ctx->r4 == 0) {
        // 0x802E6B30: nop
    
            goto L_802E6B3C;
    }
    // 0x802E6B30: nop

    // 0x802E6B34: jalr        $t9
    // 0x802E6B38: addiu       $a1, $zero, 0x200
    ctx->r5 = ADD32(0, 0X200);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E6B38: addiu       $a1, $zero, 0x200
    ctx->r5 = ADD32(0, 0X200);
    after_2:
L_802E6B3C:
    // 0x802E6B3C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6B40: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E6B44: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E6B48: jalr        $t9
    // 0x802E6B4C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E6B4C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
    // 0x802E6B50: b           L_802E6BA8
    // 0x802E6B54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_802E6BA8;
    // 0x802E6B54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E6B58:
    // 0x802E6B58: lwc1        $f16, 0x8($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X8);
    // 0x802E6B5C: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x802E6B60: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x802E6B64: cvt.d.s     $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f0.d = CVT_D_S(ctx->f16.fl);
    // 0x802E6B68: addiu       $v1, $a2, 0x8
    ctx->r3 = ADD32(ctx->r6, 0X8);
    // 0x802E6B6C: div.d       $f18, $f8, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f0.d); 
    ctx->f18.d = DIV_D(ctx->f8.d, ctx->f0.d);
    // 0x802E6B70: lwc1        $f6, 0x8($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X8);
    // 0x802E6B74: cvt.d.s     $f10, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f10.d = CVT_D_S(ctx->f6.fl);
    // 0x802E6B78: div.d       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.d); NAN_CHECK(ctx->f0.d); 
    ctx->f4.d = DIV_D(ctx->f18.d, ctx->f0.d);
    // 0x802E6B7C: add.d       $f16, $f10, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f4.d); 
    ctx->f16.d = ctx->f10.d + ctx->f4.d;
    // 0x802E6B80: cvt.s.d     $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f6.fl = CVT_S_D(ctx->f16.d);
    // 0x802E6B84: swc1        $f6, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->f6.u32l;
    // 0x802E6B88: lwc1        $f8, 0x54($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X54);
    // 0x802E6B8C: lwc1        $f18, 0x8($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X8);
    // 0x802E6B90: sub.s       $f10, $f8, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = ctx->f8.fl - ctx->f18.fl;
    // 0x802E6B94: swc1        $f10, 0x54($v0)
    MEM_W(0X54, ctx->r2) = ctx->f10.u32l;
    // 0x802E6B98: lw          $t0, 0xC($v1)
    ctx->r8 = MEM_W(ctx->r3, 0XC);
    // 0x802E6B9C: sra         $t1, $t0, 8
    ctx->r9 = S32(SIGNED(ctx->r8) >> 8);
    // 0x802E6BA0: sb          $t1, 0x1B($v0)
    MEM_B(0X1B, ctx->r2) = ctx->r9;
    // 0x802E6BA4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_802E6BA8:
    // 0x802E6BA8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E6BAC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x802E6BB0: jr          $ra
    // 0x802E6BB4: nop

    return;
    // 0x802E6BB4: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E6BB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E6BB8: jr          $ra
    // 0x802E6BBC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x802E6BBC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void map_ovl_00_func_802E6BC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E6BC0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x802E6BC4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E6BC8: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x802E6BCC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x802E6BD0: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x802E6BD4: addiu       $t9, $t9, 0x938
    ctx->r25 = ADD32(ctx->r25, 0X938);
    // 0x802E6BD8: lw          $a0, 0x4($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X4);
    // 0x802E6BDC: jalr        $t9
    // 0x802E6BE0: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6BE0: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x802E6BE4: jal         0x801469F4
    // 0x802E6BE8: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_801469F4(rdram, ctx);
        goto after_1;
    // 0x802E6BE8: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x802E6BEC: lw          $t7, 0x1C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X1C);
    // 0x802E6BF0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x802E6BF4: lw          $t9, 0x18($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X18);
    // 0x802E6BF8: jalr        $t9
    // 0x802E6BFC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x802E6BFC: nop

    after_2:
    // 0x802E6C00: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E6C04: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x802E6C08: jr          $ra
    // 0x802E6C0C: nop

    return;
    // 0x802E6C0C: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E6C10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E6C10: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E6C14: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E6C18: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x802E6C1C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x802E6C20: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x802E6C24: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x802E6C28: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x802E6C2C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x802E6C30: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x802E6C34: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x802E6C38: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x802E6C3C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x802E6C40: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x802E6C44: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x802E6C48: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x802E6C4C: lui         $t9, 0x802F
    ctx->r25 = S32(0X802F << 16);
    // 0x802E6C50: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x802E6C54: lw          $t9, -0x4610($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X4610);
    // 0x802E6C58: jalr        $t9
    // 0x802E6C5C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6C5C: nop

    after_0:
    // 0x802E6C60: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E6C64: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x802E6C68: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x802E6C6C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x802E6C70: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E6C74: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E6C78: jr          $ra
    // 0x802E6C7C: nop

    return;
    // 0x802E6C7C: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E6C80(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E6C80: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x802E6C84: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x802E6C88: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6C8C: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x802E6C90: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x802E6C94: addiu       $t9, $t9, 0x5A30
    ctx->r25 = ADD32(ctx->r25, 0X5A30);
    // 0x802E6C98: lui         $a1, 0x801A
    ctx->r5 = S32(0X801A << 16);
    // 0x802E6C9C: lw          $a1, -0x1A10($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X1A10);
    // 0x802E6CA0: jalr        $t9
    // 0x802E6CA4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6CA4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_0:
    // 0x802E6CA8: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E6CAC: addiu       $t9, $t9, 0x31CC
    ctx->r25 = ADD32(ctx->r25, 0X31CC);
    // 0x802E6CB0: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x802E6CB4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E6CB8: jalr        $t9
    // 0x802E6CBC: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E6CBC: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    after_1:
    // 0x802E6CC0: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x802E6CC4: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x802E6CC8: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x802E6CCC: lhu         $t8, 0x2($a1)
    ctx->r24 = MEM_HU(ctx->r5, 0X2);
    // 0x802E6CD0: lw          $t6, 0x2B14($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2B14);
    // 0x802E6CD4: lui         $t7, 0x603
    ctx->r15 = S32(0X603 << 16);
    // 0x802E6CD8: addiu       $t7, $t7, -0x32E0
    ctx->r15 = ADD32(ctx->r15, -0X32E0);
    // 0x802E6CDC: ori         $t0, $t8, 0x900
    ctx->r8 = ctx->r24 | 0X900;
    // 0x802E6CE0: sw          $t7, 0x3C($a1)
    MEM_W(0X3C, ctx->r5) = ctx->r15;
    // 0x802E6CE4: sh          $t0, 0x2($a1)
    MEM_H(0X2, ctx->r5) = ctx->r8;
    // 0x802E6CE8: sw          $t6, 0x40($a1)
    MEM_W(0X40, ctx->r5) = ctx->r14;
    // 0x802E6CEC: lw          $t1, 0x2B0C($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X2B0C);
    // 0x802E6CF0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E6CF4: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x802E6CF8: sw          $t1, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->r9;
    // 0x802E6CFC: lw          $t2, 0x7C($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X7C);
    // 0x802E6D00: addiu       $v0, $s0, 0x34
    ctx->r2 = ADD32(ctx->r16, 0X34);
    // 0x802E6D04: lui         $at, 0x4049
    ctx->r1 = S32(0X4049 << 16);
    // 0x802E6D08: sw          $t2, 0x24($a1)
    MEM_W(0X24, ctx->r5) = ctx->r10;
    // 0x802E6D0C: sw          $t3, 0x64($s0)
    MEM_W(0X64, ctx->r16) = ctx->r11;
    // 0x802E6D10: sw          $a1, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->r5;
    // 0x802E6D14: swc1        $f4, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f4.u32l;
    // 0x802E6D18: lwc1        $f6, 0x54($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X54);
    // 0x802E6D1C: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x802E6D20: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x802E6D24: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x802E6D28: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x802E6D2C: add.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = ctx->f8.d + ctx->f10.d;
    // 0x802E6D30: cvt.s.d     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.d); 
    ctx->f18.fl = CVT_S_D(ctx->f16.d);
    // 0x802E6D34: jal         0x80143234
    // 0x802E6D38: swc1        $f18, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f18.u32l;
    func_80143234(rdram, ctx);
        goto after_2;
    // 0x802E6D38: swc1        $f18, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f18.u32l;
    after_2:
    // 0x802E6D3C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6D40: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E6D44: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x802E6D48: jalr        $t9
    // 0x802E6D4C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x802E6D4C: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
    // 0x802E6D50: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x802E6D54: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x802E6D58: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x802E6D5C: jr          $ra
    // 0x802E6D60: nop

    return;
    // 0x802E6D60: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E6D64(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E6D64: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E6D68: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x802E6D6C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x802E6D70: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E6D74: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x802E6D78: lw          $t6, 0x2B0C($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2B0C);
    // 0x802E6D7C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x802E6D80: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x802E6D84: sw          $t6, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r14;
    // 0x802E6D88: lw          $t7, 0x7C($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X7C);
    // 0x802E6D8C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6D90: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E6D94: sw          $t7, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r15;
    // 0x802E6D98: lw          $t8, 0x64($a0)
    ctx->r24 = MEM_W(ctx->r4, 0X64);
    // 0x802E6D9C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E6DA0: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E6DA4: beq         $t8, $at, L_802E6DBC
    if (ctx->r24 == ctx->r1) {
        // 0x802E6DA8: addiu       $a1, $zero, 0x15
        ctx->r5 = ADD32(0, 0X15);
            goto L_802E6DBC;
    }
    // 0x802E6DA8: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    // 0x802E6DAC: jalr        $t9
    // 0x802E6DB0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6DB0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x802E6DB4: beq         $v0, $zero, L_802E6DD0
    if (ctx->r2 == 0) {
        // 0x802E6DB8: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_802E6DD0;
    }
    // 0x802E6DB8: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
L_802E6DBC:
    // 0x802E6DBC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6DC0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E6DC4: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x802E6DC8: jalr        $t9
    // 0x802E6DCC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E6DCC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_1:
L_802E6DD0:
    // 0x802E6DD0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E6DD4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E6DD8: jr          $ra
    // 0x802E6DDC: nop

    return;
    // 0x802E6DDC: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E6DE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E6DE0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x802E6DE4: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x802E6DE8: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x802E6DEC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E6DF0: lw          $v1, 0x24($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X24);
    // 0x802E6DF4: lw          $t6, 0x2B0C($a3)
    ctx->r14 = MEM_W(ctx->r7, 0X2B0C);
    // 0x802E6DF8: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x802E6DFC: lwc1        $f4, 0x54($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X54);
    // 0x802E6E00: sw          $t6, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r14;
    // 0x802E6E04: lw          $t7, 0x7C($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X7C);
    // 0x802E6E08: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6E0C: addiu       $t9, $t9, 0x48A0
    ctx->r25 = ADD32(ctx->r25, 0X48A0);
    // 0x802E6E10: sw          $t7, 0x24($v1)
    MEM_W(0X24, ctx->r3) = ctx->r15;
    // 0x802E6E14: lwc1        $f6, 0x34($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X34);
    // 0x802E6E18: addiu       $a1, $zero, 0x15
    ctx->r5 = ADD32(0, 0X15);
    // 0x802E6E1C: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x802E6E20: swc1        $f8, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f8.u32l;
    // 0x802E6E24: lwc1        $f10, 0x38($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X38);
    // 0x802E6E28: lwc1        $f16, 0x54($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X54);
    // 0x802E6E2C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x802E6E30: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x802E6E34: c.lt.s      $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f10.fl < ctx->f16.fl;
    // 0x802E6E38: nop

    // 0x802E6E3C: bc1tl       L_802E6E68
    if (c1cs) {
        // 0x802E6E40: lwc1        $f18, 0x38($a2)
        ctx->f18.u32l = MEM_W(ctx->r6, 0X38);
            goto L_802E6E68;
    }
    goto skip_0;
    // 0x802E6E40: lwc1        $f18, 0x38($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X38);
    skip_0:
    // 0x802E6E44: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    // 0x802E6E48: jalr        $t9
    // 0x802E6E4C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6E4C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_0:
    // 0x802E6E50: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x802E6E54: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x802E6E58: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x802E6E5C: beq         $v0, $zero, L_802E6E88
    if (ctx->r2 == 0) {
        // 0x802E6E60: lw          $a2, 0x20($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X20);
            goto L_802E6E88;
    }
    // 0x802E6E60: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x802E6E64: lwc1        $f18, 0x38($a2)
    ctx->f18.u32l = MEM_W(ctx->r6, 0X38);
L_802E6E68:
    // 0x802E6E68: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x802E6E6C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x802E6E70: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x802E6E74: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x802E6E78: jalr        $t9
    // 0x802E6E7C: swc1        $f18, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f18.u32l;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E6E7C: swc1        $f18, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f18.u32l;
    after_1:
    // 0x802E6E80: b           L_802E6EF0
    // 0x802E6E84: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_802E6EF0;
    // 0x802E6E84: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E6E88:
    // 0x802E6E88: lui         $at, 0x4049
    ctx->r1 = S32(0X4049 << 16);
    // 0x802E6E8C: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x802E6E90: lwc1        $f6, 0x8($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X8);
    // 0x802E6E94: lui         $at, 0x4004
    ctx->r1 = S32(0X4004 << 16);
    // 0x802E6E98: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x802E6E9C: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x802E6EA0: cvt.d.s     $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f0.d = CVT_D_S(ctx->f6.fl);
    // 0x802E6EA4: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x802E6EA8: mul.d       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = MUL_D(ctx->f10.d, ctx->f0.d);
    // 0x802E6EAC: addiu       $v0, $a2, 0x34
    ctx->r2 = ADD32(ctx->r6, 0X34);
    // 0x802E6EB0: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x802E6EB4: lui         $at, 0x3FE0
    ctx->r1 = S32(0X3FE0 << 16);
    // 0x802E6EB8: cvt.d.s     $f2, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f2.d = CVT_D_S(ctx->f4.fl);
    // 0x802E6EBC: div.d       $f18, $f8, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = DIV_D(ctx->f8.d, ctx->f16.d);
    // 0x802E6EC0: c.lt.d      $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f2.d < ctx->f18.d;
    // 0x802E6EC4: nop

    // 0x802E6EC8: bc1fl       L_802E6EF0
    if (!c1cs) {
        // 0x802E6ECC: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_802E6EF0;
    }
    goto skip_1;
    // 0x802E6ECC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x802E6ED0: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x802E6ED4: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x802E6ED8: nop

    // 0x802E6EDC: div.d       $f6, $f4, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.d); NAN_CHECK(ctx->f0.d); 
    ctx->f6.d = DIV_D(ctx->f4.d, ctx->f0.d);
    // 0x802E6EE0: add.d       $f10, $f2, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f2.d); NAN_CHECK(ctx->f6.d); 
    ctx->f10.d = ctx->f2.d + ctx->f6.d;
    // 0x802E6EE4: cvt.s.d     $f8, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f8.fl = CVT_S_D(ctx->f10.d);
    // 0x802E6EE8: swc1        $f8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f8.u32l;
    // 0x802E6EEC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_802E6EF0:
    // 0x802E6EF0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x802E6EF4: jr          $ra
    // 0x802E6EF8: nop

    return;
    // 0x802E6EF8: nop

;}
RECOMP_FUNC void map_ovl_00_func_802E6EFC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x802E6EFC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x802E6F00: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x802E6F04: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x802E6F08: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x802E6F0C: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x802E6F10: lw          $t6, 0x2B0C($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X2B0C);
    // 0x802E6F14: lui         $t9, 0x8014
    ctx->r25 = S32(0X8014 << 16);
    // 0x802E6F18: addiu       $t9, $t9, 0x314C
    ctx->r25 = ADD32(ctx->r25, 0X314C);
    // 0x802E6F1C: sw          $t6, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r14;
    // 0x802E6F20: lw          $t7, 0x7C($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X7C);
    // 0x802E6F24: addiu       $a1, $v0, 0x50
    ctx->r5 = ADD32(ctx->r2, 0X50);
    // 0x802E6F28: sw          $t7, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r15;
    // 0x802E6F2C: jalr        $t9
    // 0x802E6F30: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x802E6F30: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x802E6F34: beq         $v0, $zero, L_802E6F48
    if (ctx->r2 == 0) {
        // 0x802E6F38: lw          $a0, 0x18($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X18);
            goto L_802E6F48;
    }
    // 0x802E6F38: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x802E6F3C: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x802E6F40: jalr        $t9
    // 0x802E6F44: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x802E6F44: nop

    after_1:
L_802E6F48:
    // 0x802E6F48: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x802E6F4C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x802E6F50: jr          $ra
    // 0x802E6F54: nop

    return;
    // 0x802E6F54: nop

;}
