#include "recomp.h"
#include "funcs.h"
#include <stdio.h>

RECOMP_FUNC void func_8001A8C4(uint8_t* rdram, recomp_context* ctx) {
    fprintf(stderr, "[func_8001A8C4] CALLED — this sets sys+0x2B70!\n"); fflush(stderr);
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A8C4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001A8C8: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001A8CC: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x8001A8D0: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x8001A8D4: lw          $t6, 0x2BCC($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X2BCC);
    // 0x8001A8D8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001A8DC: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001A8E0: beq         $t6, $zero, L_8001A900
    if (ctx->r14 == 0) {
        // 0x8001A8E4: sw          $t6, 0x2BC8($s0)
        MEM_W(0X2BC8, ctx->r16) = ctx->r14;
            goto L_8001A900;
    }
    // 0x8001A8E4: sw          $t6, 0x2BC8($s0)
    MEM_W(0X2BC8, ctx->r16) = ctx->r14;
    // 0x8001A8E8: lw          $v0, 0x2BD0($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2BD0);
    // 0x8001A8EC: andi        $v0, $v0, 0x10
    ctx->r2 = ctx->r2 & 0X10;
    // 0x8001A8F0: beql        $v0, $zero, L_8001A904
    if (ctx->r2 == 0) {
        // 0x8001A8F4: sw          $zero, 0x2BD0($s0)
        MEM_W(0X2BD0, ctx->r16) = 0;
            goto L_8001A904;
    }
    goto skip_0;
    // 0x8001A8F4: sw          $zero, 0x2BD0($s0)
    MEM_W(0X2BD0, ctx->r16) = 0;
    skip_0:
    // 0x8001A8F8: b           L_8001A904
    // 0x8001A8FC: sw          $v0, 0x2BD0($s0)
    MEM_W(0X2BD0, ctx->r16) = ctx->r2;
        goto L_8001A904;
    // 0x8001A8FC: sw          $v0, 0x2BD0($s0)
    MEM_W(0X2BD0, ctx->r16) = ctx->r2;
L_8001A900:
    // 0x8001A900: sw          $zero, 0x2BD0($s0)
    MEM_W(0X2BD0, ctx->r16) = 0;
L_8001A904:
    // 0x8001A904: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A908: jal         0x8000233C
    // 0x8001A90C: addiu       $a1, $zero, 0x16
    ctx->r5 = ADD32(0, 0X16);
    func_8000233C(rdram, ctx);
        goto after_0;
    // 0x8001A90C: addiu       $a1, $zero, 0x16
    ctx->r5 = ADD32(0, 0X16);
    after_0:
    // 0x8001A910: sw          $v0, 0x290C($s0)
    MEM_W(0X290C, ctx->r16) = ctx->r2;
    // 0x8001A914: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A918: jal         0x8000233C
    // 0x8001A91C: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    func_8000233C(rdram, ctx);
        goto after_1;
    // 0x8001A91C: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    after_1:
    // 0x8001A920: sw          $v0, 0x2914($s0)
    MEM_W(0X2914, ctx->r16) = ctx->r2;
    // 0x8001A924: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A928: jal         0x8000233C
    // 0x8001A92C: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    func_8000233C(rdram, ctx);
        goto after_2;
    // 0x8001A92C: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    after_2:
    // 0x8001A930: sw          $v0, 0x291C($s0)
    MEM_W(0X291C, ctx->r16) = ctx->r2;
    // 0x8001A934: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A938: jal         0x8000233C
    // 0x8001A93C: addiu       $a1, $zero, 0x9
    ctx->r5 = ADD32(0, 0X9);
    func_8000233C(rdram, ctx);
        goto after_3;
    // 0x8001A93C: addiu       $a1, $zero, 0x9
    ctx->r5 = ADD32(0, 0X9);
    after_3:
    // 0x8001A940: sw          $v0, 0x2924($s0)
    MEM_W(0X2924, ctx->r16) = ctx->r2;
    // 0x8001A944: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A948: jal         0x8000233C
    // 0x8001A94C: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
    func_8000233C(rdram, ctx);
        goto after_4;
    // 0x8001A94C: addiu       $a1, $zero, 0xA
    ctx->r5 = ADD32(0, 0XA);
    after_4:
    // 0x8001A950: sw          $v0, 0x292C($s0)
    MEM_W(0X292C, ctx->r16) = ctx->r2;
    // 0x8001A954: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A958: jal         0x8000233C
    // 0x8001A95C: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    func_8000233C(rdram, ctx);
        goto after_5;
    // 0x8001A95C: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    after_5:
    // 0x8001A960: sw          $v0, 0x2934($s0)
    MEM_W(0X2934, ctx->r16) = ctx->r2;
    // 0x8001A964: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A968: jal         0x8000233C
    // 0x8001A96C: addiu       $a1, $zero, 0x106
    ctx->r5 = ADD32(0, 0X106);
    func_8000233C(rdram, ctx);
        goto after_6;
    // 0x8001A96C: addiu       $a1, $zero, 0x106
    ctx->r5 = ADD32(0, 0X106);
    after_6:
    // 0x8001A970: lh          $t8, 0x2BB8($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X2BB8);
    // 0x8001A974: lui         $t0, 0x800B
    ctx->r8 = S32(0X800B << 16);
    // 0x8001A978: sw          $v0, 0x293C($s0)
    MEM_W(0X293C, ctx->r16) = ctx->r2;
    // 0x8001A97C: sll         $t9, $t8, 2
    ctx->r25 = S32(ctx->r24 << 2);
    // 0x8001A980: subu        $t9, $t9, $t8
    ctx->r25 = SUB32(ctx->r25, ctx->r24);
    // 0x8001A984: sll         $t9, $t9, 2
    ctx->r25 = S32(ctx->r25 << 2);
    // 0x8001A988: addu        $t0, $t0, $t9
    ctx->r8 = ADD32(ctx->r8, ctx->r25);
    // 0x8001A98C: lh          $t0, -0x2C18($t0)
    ctx->r8 = MEM_H(ctx->r8, -0X2C18);
    // 0x8001A990: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A994: addiu       $a1, $zero, 0xA2
    ctx->r5 = ADD32(0, 0XA2);
    // 0x8001A998: jal         0x8000233C
    // 0x8001A99C: sw          $t0, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->r8;
    func_8000233C(rdram, ctx);
        goto after_7;
    // 0x8001A99C: sw          $t0, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->r8;
    after_7:
    // 0x8001A9A0: sw          $v0, 0x2944($s0)
    MEM_W(0X2944, ctx->r16) = ctx->r2;
    // 0x8001A9A4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A9A8: jal         0x8000233C
    // 0x8001A9AC: addiu       $a1, $zero, 0x185
    ctx->r5 = ADD32(0, 0X185);
    func_8000233C(rdram, ctx);
        goto after_8;
    // 0x8001A9AC: addiu       $a1, $zero, 0x185
    ctx->r5 = ADD32(0, 0X185);
    after_8:
    // 0x8001A9B0: sw          $v0, 0x294C($s0)
    MEM_W(0X294C, ctx->r16) = ctx->r2;
    // 0x8001A9B4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A9B8: jal         0x8000233C
    // 0x8001A9BC: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    func_8000233C(rdram, ctx);
        goto after_9;
    // 0x8001A9BC: addiu       $a1, $zero, 0x6
    ctx->r5 = ADD32(0, 0X6);
    after_9:
    // 0x8001A9C0: sw          $v0, 0x2954($s0)
    MEM_W(0X2954, ctx->r16) = ctx->r2;
    // 0x8001A9C4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001A9C8: jal         0x8000233C
    // 0x8001A9CC: addiu       $a1, $zero, 0x89
    ctx->r5 = ADD32(0, 0X89);
    func_8000233C(rdram, ctx);
        goto after_10;
    // 0x8001A9CC: addiu       $a1, $zero, 0x89
    ctx->r5 = ADD32(0, 0X89);
    after_10:
    // 0x8001A9D0: sw          $v0, 0x2B70($s0)
    MEM_W(0X2B70, ctx->r16) = ctx->r2;
    // 0x8001A9D4: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x8001A9D8: lui         $t1, 0x8002
    ctx->r9 = S32(0X8002 << 16);
    // 0x8001A9DC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001A9E0: addiu       $t1, $t1, -0x5838
    ctx->r9 = ADD32(ctx->r9, -0X5838);
    // 0x8001A9E4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001A9E8: sw          $t1, 0x10($v0)
    MEM_W(0X10, ctx->r2) = ctx->r9;
    // 0x8001A9EC: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001A9F0: jalr        $t9
    // 0x8001A9F4: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x8001A9F4: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_11:
    // 0x8001A9F8: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x8001A9FC: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001AA00: lh          $t2, 0xE($a0)
    ctx->r10 = MEM_H(ctx->r4, 0XE);
    // 0x8001AA04: sll         $t3, $t2, 1
    ctx->r11 = S32(ctx->r10 << 1);
    // 0x8001AA08: addu        $v0, $a0, $t3
    ctx->r2 = ADD32(ctx->r4, ctx->r11);
    // 0x8001AA0C: lbu         $t6, 0x9($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X9);
    // 0x8001AA10: lbu         $t4, 0x8($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X8);
    // 0x8001AA14: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001AA18: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x8001AA1C: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x8001AA20: sb          $t5, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r13;
    // 0x8001AA24: addu        $t9, $t9, $t7
    ctx->r25 = ADD32(ctx->r25, ctx->r15);
    // 0x8001AA28: lw          $t9, 0x37C4($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X37C4);
    // 0x8001AA2C: jalr        $t9
    // 0x8001AA30: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x8001AA30: nop

    after_12:
    // 0x8001AA34: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001AA38: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001AA3C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001AA40: jr          $ra
    // 0x8001AA44: nop

    return;
    // 0x8001AA44: nop

;}
RECOMP_FUNC void func_8001AA48(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001AA48: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001AA4C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001AA50: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8001AA54: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8001AA58: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8001AA5C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8001AA60: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8001AA64: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8001AA68: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8001AA6C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x8001AA70: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001AA74: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8001AA78: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001AA7C: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x8001AA80: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8001AA84: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001AA88: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8001AA8C: lw          $t9, 0x37DC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X37DC);
    // 0x8001AA90: jalr        $t9
    // 0x8001AA94: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001AA94: nop

    after_0:
    // 0x8001AA98: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001AA9C: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x8001AAA0: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8001AAA4: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x8001AAA8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001AAAC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001AAB0: jr          $ra
    // 0x8001AAB4: nop

    return;
    // 0x8001AAB4: nop

;}
RECOMP_FUNC void func_8001AAB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001AAB8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001AABC: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8001AAC0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001AAC4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001AAC8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001AACC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x8001AAD0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x8001AAD4: jalr        $t9
    // 0x8001AAD8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001AAD8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    after_0:
    // 0x8001AADC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001AAE0: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001AAE4: lh          $t6, 0xE($a0)
    ctx->r14 = MEM_H(ctx->r4, 0XE);
    // 0x8001AAE8: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x8001AAEC: addu        $v0, $a0, $t7
    ctx->r2 = ADD32(ctx->r4, ctx->r15);
    // 0x8001AAF0: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x8001AAF4: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x8001AAF8: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001AAFC: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x8001AB00: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x8001AB04: sb          $t0, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r8;
    // 0x8001AB08: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
    // 0x8001AB0C: lw          $t9, 0x37DC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X37DC);
    // 0x8001AB10: jalr        $t9
    // 0x8001AB14: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001AB14: nop

    after_1:
    // 0x8001AB18: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001AB1C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001AB20: jr          $ra
    // 0x8001AB24: nop

    return;
    // 0x8001AB24: nop

;}
RECOMP_FUNC void func_8001AB28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001AB28: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8001AB2C: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8001AB30: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x8001AB34: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x8001AB38: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x8001AB3C: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x8001AB40: jal         0x800029EC
    // 0x8001AB44: lw          $a0, 0x290C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X290C);
    func_800029EC(rdram, ctx);
        goto after_0;
    // 0x8001AB44: lw          $a0, 0x290C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X290C);
    after_0:
    // 0x8001AB48: lw          $t6, 0x2910($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X2910);
    // 0x8001AB4C: lw          $a0, 0x2914($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2914);
    // 0x8001AB50: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x8001AB54: jal         0x800029EC
    // 0x8001AB58: sw          $t7, 0x2910($s0)
    MEM_W(0X2910, ctx->r16) = ctx->r15;
    func_800029EC(rdram, ctx);
        goto after_1;
    // 0x8001AB58: sw          $t7, 0x2910($s0)
    MEM_W(0X2910, ctx->r16) = ctx->r15;
    after_1:
    // 0x8001AB5C: lw          $t8, 0x2918($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X2918);
    // 0x8001AB60: lw          $a0, 0x291C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X291C);
    // 0x8001AB64: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x8001AB68: jal         0x800029EC
    // 0x8001AB6C: sw          $t9, 0x2918($s0)
    MEM_W(0X2918, ctx->r16) = ctx->r25;
    func_800029EC(rdram, ctx);
        goto after_2;
    // 0x8001AB6C: sw          $t9, 0x2918($s0)
    MEM_W(0X2918, ctx->r16) = ctx->r25;
    after_2:
    // 0x8001AB70: lw          $t0, 0x2920($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X2920);
    // 0x8001AB74: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x8001AB78: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x8001AB7C: jal         0x800029EC
    // 0x8001AB80: sw          $t1, 0x2920($s0)
    MEM_W(0X2920, ctx->r16) = ctx->r9;
    func_800029EC(rdram, ctx);
        goto after_3;
    // 0x8001AB80: sw          $t1, 0x2920($s0)
    MEM_W(0X2920, ctx->r16) = ctx->r9;
    after_3:
    // 0x8001AB84: lw          $t2, 0x2928($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X2928);
    // 0x8001AB88: lw          $a0, 0x292C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X292C);
    // 0x8001AB8C: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8001AB90: jal         0x800029EC
    // 0x8001AB94: sw          $t3, 0x2928($s0)
    MEM_W(0X2928, ctx->r16) = ctx->r11;
    func_800029EC(rdram, ctx);
        goto after_4;
    // 0x8001AB94: sw          $t3, 0x2928($s0)
    MEM_W(0X2928, ctx->r16) = ctx->r11;
    after_4:
    // 0x8001AB98: lw          $t4, 0x2930($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X2930);
    // 0x8001AB9C: lw          $a0, 0x2934($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2934);
    // 0x8001ABA0: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x8001ABA4: jal         0x800029EC
    // 0x8001ABA8: sw          $t5, 0x2930($s0)
    MEM_W(0X2930, ctx->r16) = ctx->r13;
    func_800029EC(rdram, ctx);
        goto after_5;
    // 0x8001ABA8: sw          $t5, 0x2930($s0)
    MEM_W(0X2930, ctx->r16) = ctx->r13;
    after_5:
    // 0x8001ABAC: lw          $t6, 0x2938($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X2938);
    // 0x8001ABB0: lw          $a0, 0x293C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X293C);
    // 0x8001ABB4: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x8001ABB8: jal         0x800029EC
    // 0x8001ABBC: sw          $t7, 0x2938($s0)
    MEM_W(0X2938, ctx->r16) = ctx->r15;
    func_800029EC(rdram, ctx);
        goto after_6;
    // 0x8001ABBC: sw          $t7, 0x2938($s0)
    MEM_W(0X2938, ctx->r16) = ctx->r15;
    after_6:
    // 0x8001ABC0: lw          $t8, 0x2940($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X2940);
    // 0x8001ABC4: lw          $a0, 0x2944($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2944);
    // 0x8001ABC8: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x8001ABCC: jal         0x800029EC
    // 0x8001ABD0: sw          $t9, 0x2940($s0)
    MEM_W(0X2940, ctx->r16) = ctx->r25;
    func_800029EC(rdram, ctx);
        goto after_7;
    // 0x8001ABD0: sw          $t9, 0x2940($s0)
    MEM_W(0X2940, ctx->r16) = ctx->r25;
    after_7:
    // 0x8001ABD4: lw          $t0, 0x2948($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X2948);
    // 0x8001ABD8: lw          $a0, 0x294C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X294C);
    // 0x8001ABDC: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x8001ABE0: jal         0x800029EC
    // 0x8001ABE4: sw          $t1, 0x2948($s0)
    MEM_W(0X2948, ctx->r16) = ctx->r9;
    func_800029EC(rdram, ctx);
        goto after_8;
    // 0x8001ABE4: sw          $t1, 0x2948($s0)
    MEM_W(0X2948, ctx->r16) = ctx->r9;
    after_8:
    // 0x8001ABE8: lw          $t2, 0x2950($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X2950);
    // 0x8001ABEC: lw          $a0, 0x2B70($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B70);
    // 0x8001ABF0: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8001ABF4: jal         0x800029EC
    // 0x8001ABF8: sw          $t3, 0x2950($s0)
    MEM_W(0X2950, ctx->r16) = ctx->r11;
    func_800029EC(rdram, ctx);
        goto after_9;
    // 0x8001ABF8: sw          $t3, 0x2950($s0)
    MEM_W(0X2950, ctx->r16) = ctx->r11;
    after_9:
    // 0x8001ABFC: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001AC00: lui         $at, 0x1000
    ctx->r1 = S32(0X1000 << 16);
    // 0x8001AC04: and         $v1, $v0, $at
    ctx->r3 = ctx->r2 & ctx->r1;
    // 0x8001AC08: beql        $v1, $zero, L_8001ACA0
    if (ctx->r3 == 0) {
        // 0x8001AC0C: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_8001ACA0;
    }
    goto skip_0;
    // 0x8001AC0C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_0:
    // 0x8001AC10: lw          $t4, 0x2BC8($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X2BC8);
    // 0x8001AC14: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x8001AC18: beq         $t4, $zero, L_8001AC30
    if (ctx->r12 == 0) {
        // 0x8001AC1C: nop
    
            goto L_8001AC30;
    }
    // 0x8001AC1C: nop

    // 0x8001AC20: lw          $t5, 0x2BD0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X2BD0);
    // 0x8001AC24: andi        $t6, $t5, 0x10
    ctx->r14 = ctx->r13 & 0X10;
    // 0x8001AC28: bnel        $t6, $zero, L_8001ACA0
    if (ctx->r14 != 0) {
        // 0x8001AC2C: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_8001ACA0;
    }
    goto skip_1;
    // 0x8001AC2C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_1:
L_8001AC30:
    // 0x8001AC30: bgezl       $t7, L_8001ACA0
    if (SIGNED(ctx->r15) >= 0) {
        // 0x8001AC34: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_8001ACA0;
    }
    goto skip_2;
    // 0x8001AC34: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_2:
    // 0x8001AC38: beq         $v1, $zero, L_8001AC9C
    if (ctx->r3 == 0) {
        // 0x8001AC3C: lui         $a0, 0x800C
        ctx->r4 = S32(0X800C << 16);
            goto L_8001AC9C;
    }
    // 0x8001AC3C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001AC40: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8001AC44: lw          $a1, 0x1480($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X1480);
    // 0x8001AC48: jal         0x800116AC
    // 0x8001AC4C: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    func_800116AC(rdram, ctx);
        goto after_10;
    // 0x8001AC4C: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    after_10:
    // 0x8001AC50: lw          $t8, 0x2908($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X2908);
    // 0x8001AC54: lui         $at, 0x800
    ctx->r1 = S32(0X800 << 16);
    // 0x8001AC58: lbu         $t0, 0x2BBE($s0)
    ctx->r8 = MEM_BU(ctx->r16, 0X2BBE);
    // 0x8001AC5C: or          $t9, $t8, $at
    ctx->r25 = ctx->r24 | ctx->r1;
    // 0x8001AC60: sw          $t9, 0x2908($s0)
    MEM_W(0X2908, ctx->r16) = ctx->r25;
    // 0x8001AC64: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001AC68: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x8001AC6C: addiu       $a0, $zero, 0x4000
    ctx->r4 = ADD32(0, 0X4000);
    // 0x8001AC70: lhu         $a1, 0x2BC2($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X2BC2);
    // 0x8001AC74: lbu         $a2, 0x2BBC($s0)
    ctx->r6 = MEM_BU(ctx->r16, 0X2BBC);
    // 0x8001AC78: lbu         $a3, 0x2BBD($s0)
    ctx->r7 = MEM_BU(ctx->r16, 0X2BBD);
    // 0x8001AC7C: jalr        $t9
    // 0x8001AC80: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_11;
    // 0x8001AC80: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    after_11:
    // 0x8001AC84: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x8001AC88: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001AC8C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001AC90: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001AC94: jalr        $t9
    // 0x8001AC98: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x8001AC98: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_12:
L_8001AC9C:
    // 0x8001AC9C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_8001ACA0:
    // 0x8001ACA0: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8001ACA4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8001ACA8: jr          $ra
    // 0x8001ACAC: nop

    return;
    // 0x8001ACAC: nop

;}
RECOMP_FUNC void func_8001ACB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001ACB0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8001ACB4: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8001ACB8: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x8001ACBC: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x8001ACC0: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x8001ACC4: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x8001ACC8: jal         0x800409A0
    // 0x8001ACCC: sh          $zero, 0x9E($s0)
    MEM_H(0X9E, ctx->r16) = 0;
    func_800409A0(rdram, ctx);
        goto after_0;
    // 0x8001ACCC: sh          $zero, 0x9E($s0)
    MEM_H(0X9E, ctx->r16) = 0;
    after_0:
    // 0x8001ACD0: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001ACD4: andi        $t6, $v0, 0x100
    ctx->r14 = ctx->r2 & 0X100;
    // 0x8001ACD8: bnel        $t6, $zero, L_8001ACFC
    if (ctx->r14 != 0) {
        // 0x8001ACDC: andi        $t9, $v0, 0x200
        ctx->r25 = ctx->r2 & 0X200;
            goto L_8001ACFC;
    }
    goto skip_0;
    // 0x8001ACDC: andi        $t9, $v0, 0x200
    ctx->r25 = ctx->r2 & 0X200;
    skip_0:
    // 0x8001ACE0: jal         0x800029EC
    // 0x8001ACE4: lw          $a0, 0x290C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X290C);
    func_800029EC(rdram, ctx);
        goto after_1;
    // 0x8001ACE4: lw          $a0, 0x290C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X290C);
    after_1:
    // 0x8001ACE8: lw          $t7, 0x2910($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X2910);
    // 0x8001ACEC: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001ACF0: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001ACF4: sw          $t8, 0x2910($s0)
    MEM_W(0X2910, ctx->r16) = ctx->r24;
    // 0x8001ACF8: andi        $t9, $v0, 0x200
    ctx->r25 = ctx->r2 & 0X200;
L_8001ACFC:
    // 0x8001ACFC: bne         $t9, $zero, L_8001AE00
    if (ctx->r25 != 0) {
        // 0x8001AD00: andi        $t0, $v0, 0x80
        ctx->r8 = ctx->r2 & 0X80;
            goto L_8001AE00;
    }
    // 0x8001AD00: andi        $t0, $v0, 0x80
    ctx->r8 = ctx->r2 & 0X80;
    // 0x8001AD04: bnel        $t0, $zero, L_8001AD2C
    if (ctx->r8 != 0) {
        // 0x8001AD08: andi        $t3, $v0, 0x40
        ctx->r11 = ctx->r2 & 0X40;
            goto L_8001AD2C;
    }
    goto skip_1;
    // 0x8001AD08: andi        $t3, $v0, 0x40
    ctx->r11 = ctx->r2 & 0X40;
    skip_1:
    // 0x8001AD0C: jal         0x800029EC
    // 0x8001AD10: lw          $a0, 0x2914($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2914);
    func_800029EC(rdram, ctx);
        goto after_2;
    // 0x8001AD10: lw          $a0, 0x2914($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2914);
    after_2:
    // 0x8001AD14: lw          $t1, 0x2918($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X2918);
    // 0x8001AD18: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x8001AD1C: jal         0x80166F98
    // 0x8001AD20: sw          $t2, 0x2918($s0)
    MEM_W(0X2918, ctx->r16) = ctx->r10;
    func_80166F98(rdram, ctx);
        goto after_3;
    // 0x8001AD20: sw          $t2, 0x2918($s0)
    MEM_W(0X2918, ctx->r16) = ctx->r10;
    after_3:
    // 0x8001AD24: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001AD28: andi        $t3, $v0, 0x40
    ctx->r11 = ctx->r2 & 0X40;
L_8001AD2C:
    // 0x8001AD2C: bnel        $t3, $zero, L_8001AD50
    if (ctx->r11 != 0) {
        // 0x8001AD30: andi        $t6, $v0, 0x20
        ctx->r14 = ctx->r2 & 0X20;
            goto L_8001AD50;
    }
    goto skip_2;
    // 0x8001AD30: andi        $t6, $v0, 0x20
    ctx->r14 = ctx->r2 & 0X20;
    skip_2:
    // 0x8001AD34: jal         0x800029EC
    // 0x8001AD38: lw          $a0, 0x291C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X291C);
    func_800029EC(rdram, ctx);
        goto after_4;
    // 0x8001AD38: lw          $a0, 0x291C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X291C);
    after_4:
    // 0x8001AD3C: lw          $t4, 0x2920($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X2920);
    // 0x8001AD40: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001AD44: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x8001AD48: sw          $t5, 0x2920($s0)
    MEM_W(0X2920, ctx->r16) = ctx->r13;
    // 0x8001AD4C: andi        $t6, $v0, 0x20
    ctx->r14 = ctx->r2 & 0X20;
L_8001AD50:
    // 0x8001AD50: bnel        $t6, $zero, L_8001AD74
    if (ctx->r14 != 0) {
        // 0x8001AD54: andi        $t9, $v0, 0x10
        ctx->r25 = ctx->r2 & 0X10;
            goto L_8001AD74;
    }
    goto skip_3;
    // 0x8001AD54: andi        $t9, $v0, 0x10
    ctx->r25 = ctx->r2 & 0X10;
    skip_3:
    // 0x8001AD58: jal         0x800029EC
    // 0x8001AD5C: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    func_800029EC(rdram, ctx);
        goto after_5;
    // 0x8001AD5C: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    after_5:
    // 0x8001AD60: lw          $t7, 0x2928($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X2928);
    // 0x8001AD64: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001AD68: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001AD6C: sw          $t8, 0x2928($s0)
    MEM_W(0X2928, ctx->r16) = ctx->r24;
    // 0x8001AD70: andi        $t9, $v0, 0x10
    ctx->r25 = ctx->r2 & 0X10;
L_8001AD74:
    // 0x8001AD74: bnel        $t9, $zero, L_8001AD98
    if (ctx->r25 != 0) {
        // 0x8001AD78: andi        $t2, $v0, 0x8
        ctx->r10 = ctx->r2 & 0X8;
            goto L_8001AD98;
    }
    goto skip_4;
    // 0x8001AD78: andi        $t2, $v0, 0x8
    ctx->r10 = ctx->r2 & 0X8;
    skip_4:
    // 0x8001AD7C: jal         0x800029EC
    // 0x8001AD80: lw          $a0, 0x292C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X292C);
    func_800029EC(rdram, ctx);
        goto after_6;
    // 0x8001AD80: lw          $a0, 0x292C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X292C);
    after_6:
    // 0x8001AD84: lw          $t0, 0x2930($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X2930);
    // 0x8001AD88: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001AD8C: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x8001AD90: sw          $t1, 0x2930($s0)
    MEM_W(0X2930, ctx->r16) = ctx->r9;
    // 0x8001AD94: andi        $t2, $v0, 0x8
    ctx->r10 = ctx->r2 & 0X8;
L_8001AD98:
    // 0x8001AD98: bnel        $t2, $zero, L_8001ADBC
    if (ctx->r10 != 0) {
        // 0x8001AD9C: andi        $t5, $v0, 0x4
        ctx->r13 = ctx->r2 & 0X4;
            goto L_8001ADBC;
    }
    goto skip_5;
    // 0x8001AD9C: andi        $t5, $v0, 0x4
    ctx->r13 = ctx->r2 & 0X4;
    skip_5:
    // 0x8001ADA0: jal         0x800029EC
    // 0x8001ADA4: lw          $a0, 0x2934($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2934);
    func_800029EC(rdram, ctx);
        goto after_7;
    // 0x8001ADA4: lw          $a0, 0x2934($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2934);
    after_7:
    // 0x8001ADA8: lw          $t3, 0x2938($s0)
    ctx->r11 = MEM_W(ctx->r16, 0X2938);
    // 0x8001ADAC: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001ADB0: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x8001ADB4: sw          $t4, 0x2938($s0)
    MEM_W(0X2938, ctx->r16) = ctx->r12;
    // 0x8001ADB8: andi        $t5, $v0, 0x4
    ctx->r13 = ctx->r2 & 0X4;
L_8001ADBC:
    // 0x8001ADBC: bnel        $t5, $zero, L_8001ADE0
    if (ctx->r13 != 0) {
        // 0x8001ADC0: andi        $t8, $v0, 0x2
        ctx->r24 = ctx->r2 & 0X2;
            goto L_8001ADE0;
    }
    goto skip_6;
    // 0x8001ADC0: andi        $t8, $v0, 0x2
    ctx->r24 = ctx->r2 & 0X2;
    skip_6:
    // 0x8001ADC4: jal         0x800029EC
    // 0x8001ADC8: lw          $a0, 0x293C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X293C);
    func_800029EC(rdram, ctx);
        goto after_8;
    // 0x8001ADC8: lw          $a0, 0x293C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X293C);
    after_8:
    // 0x8001ADCC: lw          $t6, 0x2940($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X2940);
    // 0x8001ADD0: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001ADD4: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x8001ADD8: sw          $t7, 0x2940($s0)
    MEM_W(0X2940, ctx->r16) = ctx->r15;
    // 0x8001ADDC: andi        $t8, $v0, 0x2
    ctx->r24 = ctx->r2 & 0X2;
L_8001ADE0:
    // 0x8001ADE0: bnel        $t8, $zero, L_8001AE04
    if (ctx->r24 != 0) {
        // 0x8001ADE4: andi        $t1, $v0, 0x1
        ctx->r9 = ctx->r2 & 0X1;
            goto L_8001AE04;
    }
    goto skip_7;
    // 0x8001ADE4: andi        $t1, $v0, 0x1
    ctx->r9 = ctx->r2 & 0X1;
    skip_7:
    // 0x8001ADE8: jal         0x800029EC
    // 0x8001ADEC: lw          $a0, 0x2944($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2944);
    func_800029EC(rdram, ctx);
        goto after_9;
    // 0x8001ADEC: lw          $a0, 0x2944($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2944);
    after_9:
    // 0x8001ADF0: lw          $t9, 0x2948($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X2948);
    // 0x8001ADF4: lw          $v0, 0x2908($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X2908);
    // 0x8001ADF8: addiu       $t0, $t9, 0x1
    ctx->r8 = ADD32(ctx->r25, 0X1);
    // 0x8001ADFC: sw          $t0, 0x2948($s0)
    MEM_W(0X2948, ctx->r16) = ctx->r8;
L_8001AE00:
    // 0x8001AE00: andi        $t1, $v0, 0x1
    ctx->r9 = ctx->r2 & 0X1;
L_8001AE04:
    // 0x8001AE04: bne         $t1, $zero, L_8001AE20
    if (ctx->r9 != 0) {
        // 0x8001AE08: nop
    
            goto L_8001AE20;
    }
    // 0x8001AE08: nop

    // 0x8001AE0C: jal         0x800029EC
    // 0x8001AE10: lw          $a0, 0x294C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X294C);
    func_800029EC(rdram, ctx);
        goto after_10;
    // 0x8001AE10: lw          $a0, 0x294C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X294C);
    after_10:
    // 0x8001AE14: lw          $t2, 0x2950($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X2950);
    // 0x8001AE18: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8001AE1C: sw          $t3, 0x2950($s0)
    MEM_W(0X2950, ctx->r16) = ctx->r11;
L_8001AE20:
    // 0x8001AE20: jal         0x800029EC
    // 0x8001AE24: lw          $a0, 0x2B70($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B70);
    func_800029EC(rdram, ctx);
        goto after_11;
    // 0x8001AE24: lw          $a0, 0x2B70($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2B70);
    after_11:
    // 0x8001AE28: lh          $t4, 0x2B4E($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X2B4E);
    // 0x8001AE2C: lh          $t5, 0x2B4C($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X2B4C);
    // 0x8001AE30: beql        $t4, $t5, L_8001AEEC
    if (ctx->r12 == ctx->r13) {
        // 0x8001AE34: lh          $t6, 0x2BB8($s0)
        ctx->r14 = MEM_H(ctx->r16, 0X2BB8);
            goto L_8001AEEC;
    }
    goto skip_8;
    // 0x8001AE34: lh          $t6, 0x2BB8($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X2BB8);
    skip_8:
    // 0x8001AE38: lw          $t6, 0x2908($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X2908);
    // 0x8001AE3C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001AE40: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x8001AE44: andi        $t7, $t6, 0x4000
    ctx->r15 = ctx->r14 & 0X4000;
    // 0x8001AE48: bnel        $t7, $zero, L_8001AEEC
    if (ctx->r15 != 0) {
        // 0x8001AE4C: lh          $t6, 0x2BB8($s0)
        ctx->r14 = MEM_H(ctx->r16, 0X2BB8);
            goto L_8001AEEC;
    }
    goto skip_9;
    // 0x8001AE4C: lh          $t6, 0x2BB8($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X2BB8);
    skip_9:
    // 0x8001AE50: jalr        $t9
    // 0x8001AE54: ori         $a0, $zero, 0x8348
    ctx->r4 = 0 | 0X8348;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_12;
    // 0x8001AE54: ori         $a0, $zero, 0x8348
    ctx->r4 = 0 | 0X8348;
    after_12:
    // 0x8001AE58: lh          $t8, 0x2B4E($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X2B4E);
    // 0x8001AE5C: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x8001AE60: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x8001AE64: bne         $t8, $at, L_8001AE90
    if (ctx->r24 != ctx->r1) {
        // 0x8001AE68: addiu       $a0, $v0, 0x8
        ctx->r4 = ADD32(ctx->r2, 0X8);
            goto L_8001AE90;
    }
    // 0x8001AE68: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001AE6C: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x8001AE70: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001AE74: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x8001AE78: addiu       $a2, $zero, 0x9
    ctx->r6 = ADD32(0, 0X9);
    // 0x8001AE7C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001AE80: jalr        $t9
    // 0x8001AE84: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_13;
    // 0x8001AE84: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_13:
    // 0x8001AE88: b           L_8001AF44
    // 0x8001AE8C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_8001AF44;
    // 0x8001AE8C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_8001AE90:
    // 0x8001AE90: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001AE94: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x8001AE98: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    // 0x8001AE9C: jalr        $t9
    // 0x8001AEA0: addiu       $a2, $zero, 0x7
    ctx->r6 = ADD32(0, 0X7);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_14;
    // 0x8001AEA0: addiu       $a2, $zero, 0x7
    ctx->r6 = ADD32(0, 0X7);
    after_14:
    // 0x8001AEA4: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x8001AEA8: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001AEAC: lh          $t0, 0xE($a0)
    ctx->r8 = MEM_H(ctx->r4, 0XE);
    // 0x8001AEB0: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x8001AEB4: addu        $v0, $a0, $t1
    ctx->r2 = ADD32(ctx->r4, ctx->r9);
    // 0x8001AEB8: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x8001AEBC: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x8001AEC0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001AEC4: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x8001AEC8: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8001AECC: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x8001AED0: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x8001AED4: lw          $t9, 0x37DC($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X37DC);
    // 0x8001AED8: jalr        $t9
    // 0x8001AEDC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_15;
    // 0x8001AEDC: nop

    after_15:
    // 0x8001AEE0: b           L_8001AF44
    // 0x8001AEE4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
        goto L_8001AF44;
    // 0x8001AEE4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x8001AEE8: lh          $t6, 0x2BB8($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X2BB8);
L_8001AEEC:
    // 0x8001AEEC: lh          $t7, 0x28D0($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X28D0);
    // 0x8001AEF0: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001AEF4: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x8001AEF8: bne         $t6, $t7, L_8001AF10
    if (ctx->r14 != ctx->r15) {
        // 0x8001AEFC: addiu       $a0, $zero, -0x8000
        ctx->r4 = ADD32(0, -0X8000);
            goto L_8001AF10;
    }
    // 0x8001AEFC: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x8001AF00: lh          $t8, 0x2BBA($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X2BBA);
    // 0x8001AF04: lh          $t0, 0x28D2($s0)
    ctx->r8 = MEM_H(ctx->r16, 0X28D2);
    // 0x8001AF08: beql        $t8, $t0, L_8001AF44
    if (ctx->r24 == ctx->r8) {
        // 0x8001AF0C: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_8001AF44;
    }
    goto skip_10;
    // 0x8001AF0C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_10:
L_8001AF10:
    // 0x8001AF10: lbu         $t1, 0x2BBE($s0)
    ctx->r9 = MEM_BU(ctx->r16, 0X2BBE);
    // 0x8001AF14: lhu         $a1, 0x2BC0($s0)
    ctx->r5 = MEM_HU(ctx->r16, 0X2BC0);
    // 0x8001AF18: lbu         $a2, 0x2BBC($s0)
    ctx->r6 = MEM_BU(ctx->r16, 0X2BBC);
    // 0x8001AF1C: lbu         $a3, 0x2BBD($s0)
    ctx->r7 = MEM_BU(ctx->r16, 0X2BBD);
    // 0x8001AF20: jalr        $t9
    // 0x8001AF24: sw          $t1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_16;
    // 0x8001AF24: sw          $t1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r9;
    after_16:
    // 0x8001AF28: lw          $v0, 0x28($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X28);
    // 0x8001AF2C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001AF30: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001AF34: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001AF38: jalr        $t9
    // 0x8001AF3C: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_17;
    // 0x8001AF3C: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_17:
    // 0x8001AF40: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_8001AF44:
    // 0x8001AF44: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8001AF48: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8001AF4C: jr          $ra
    // 0x8001AF50: nop

    return;
    // 0x8001AF50: nop

;}
RECOMP_FUNC void func_8001AF54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001AF54: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001AF58: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001AF5C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001AF60: jal         0x8000053C
    // 0x8001AF64: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    func_8000053C(rdram, ctx);
        goto after_0;
    // 0x8001AF64: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    after_0:
    // 0x8001AF68: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x8001AF6C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001AF70: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001AF74: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001AF78: jalr        $t9
    // 0x8001AF7C: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001AF7C: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x8001AF80: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001AF84: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001AF88: jr          $ra
    // 0x8001AF8C: nop

    return;
    // 0x8001AF8C: nop

;}
RECOMP_FUNC void func_8001AF90(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001AF90: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001AF94: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001AF98: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001AF9C: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001AFA0: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x8001AFA4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x8001AFA8: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x8001AFAC: sh          $t6, -0x7CA2($at)
    MEM_H(-0X7CA2, ctx->r1) = ctx->r14;
    // 0x8001AFB0: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x8001AFB4: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    // 0x8001AFB8: addiu       $a1, $zero, 0x9
    ctx->r5 = ADD32(0, 0X9);
    // 0x8001AFBC: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8001AFC0: jalr        $t9
    // 0x8001AFC4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001AFC4: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_0:
    // 0x8001AFC8: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x8001AFCC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001AFD0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001AFD4: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001AFD8: jalr        $t9
    // 0x8001AFDC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001AFDC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x8001AFE0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001AFE4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001AFE8: jr          $ra
    // 0x8001AFEC: nop

    return;
    // 0x8001AFEC: nop

;}
RECOMP_FUNC void func_8001AFF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001AFF0: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x8001AFF4: addiu       $t4, $t4, -0x7D40
    ctx->r12 = ADD32(ctx->r12, -0X7D40);
    // 0x8001AFF8: lw          $v1, 0x2B6C($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X2B6C);
    // 0x8001AFFC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B000: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B004: lh          $t6, 0x0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X0);
    // 0x8001B008: lui         $a3, 0x8019
    ctx->r7 = S32(0X8019 << 16);
    // 0x8001B00C: addiu       $v0, $zero, -0x8000
    ctx->r2 = ADD32(0, -0X8000);
    // 0x8001B010: ori         $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 | 0X8000;
    // 0x8001B014: sh          $t7, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r15;
    // 0x8001B018: lw          $a3, 0x27A8($a3)
    ctx->r7 = MEM_W(ctx->r7, 0X27A8);
    // 0x8001B01C: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x8001B020: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x8001B024: lh          $t8, 0x0($a3)
    ctx->r24 = MEM_H(ctx->r7, 0X0);
    // 0x8001B028: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x8001B02C: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x8001B030: or          $t9, $t8, $v0
    ctx->r25 = ctx->r24 | ctx->r2;
    // 0x8001B034: sh          $t9, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r25;
    // 0x8001B038: lw          $t0, 0x27AC($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X27AC);
    // 0x8001B03C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8001B040: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x8001B044: lh          $t5, 0x0($t0)
    ctx->r13 = MEM_H(ctx->r8, 0X0);
    // 0x8001B048: or          $t6, $t5, $v0
    ctx->r14 = ctx->r13 | ctx->r2;
    // 0x8001B04C: sh          $t6, 0x0($t0)
    MEM_H(0X0, ctx->r8) = ctx->r14;
    // 0x8001B050: lw          $t1, 0x27A4($t1)
    ctx->r9 = MEM_W(ctx->r9, 0X27A4);
    // 0x8001B054: lh          $t7, 0x0($t1)
    ctx->r15 = MEM_H(ctx->r9, 0X0);
    // 0x8001B058: or          $t8, $t7, $v0
    ctx->r24 = ctx->r15 | ctx->r2;
    // 0x8001B05C: sh          $t8, 0x0($t1)
    MEM_H(0X0, ctx->r9) = ctx->r24;
    // 0x8001B060: lw          $t2, 0x27A0($t2)
    ctx->r10 = MEM_W(ctx->r10, 0X27A0);
    // 0x8001B064: lh          $t9, 0x0($t2)
    ctx->r25 = MEM_H(ctx->r10, 0X0);
    // 0x8001B068: or          $t5, $t9, $v0
    ctx->r13 = ctx->r25 | ctx->r2;
    // 0x8001B06C: sh          $t5, 0x0($t2)
    MEM_H(0X0, ctx->r10) = ctx->r13;
    // 0x8001B070: lw          $t3, 0x27B0($t3)
    ctx->r11 = MEM_W(ctx->r11, 0X27B0);
    // 0x8001B074: lh          $t6, 0x0($t3)
    ctx->r14 = MEM_H(ctx->r11, 0X0);
    // 0x8001B078: or          $t7, $t6, $v0
    ctx->r15 = ctx->r14 | ctx->r2;
    // 0x8001B07C: sh          $t7, 0x0($t3)
    MEM_H(0X0, ctx->r11) = ctx->r15;
    // 0x8001B080: sh          $zero, 0x9E($t4)
    MEM_H(0X9E, ctx->r12) = 0;
    // 0x8001B084: lh          $t8, 0xE($a0)
    ctx->r24 = MEM_H(ctx->r4, 0XE);
    // 0x8001B088: sll         $t9, $t8, 1
    ctx->r25 = S32(ctx->r24 << 1);
    // 0x8001B08C: addu        $t5, $a0, $t9
    ctx->r13 = ADD32(ctx->r4, ctx->r25);
    // 0x8001B090: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B094: addiu       $t9, $t9, 0x1C5C
    ctx->r25 = ADD32(ctx->r25, 0X1C5C);
    // 0x8001B098: sb          $zero, 0x6($t5)
    MEM_B(0X6, ctx->r13) = 0;
    // 0x8001B09C: jalr        $t9
    // 0x8001B0A0: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B0A0: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    after_0:
    // 0x8001B0A4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B0A8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B0AC: jr          $ra
    // 0x8001B0B0: nop

    return;
    // 0x8001B0B0: nop

;}
RECOMP_FUNC void func_8001B0B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B0B4: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B0B8: lw          $v0, -0x51D4($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X51D4);
    // 0x8001B0BC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001B0C0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001B0C4: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001B0C8: lh          $t6, 0x0($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X0);
    // 0x8001B0CC: lui         $at, 0xFFFF
    ctx->r1 = S32(0XFFFF << 16);
    // 0x8001B0D0: ori         $at, $at, 0x7FFF
    ctx->r1 = ctx->r1 | 0X7FFF;
    // 0x8001B0D4: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x8001B0D8: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x8001B0DC: sh          $t7, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r15;
    // 0x8001B0E0: lw          $v1, 0x27A8($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X27A8);
    // 0x8001B0E4: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x8001B0E8: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x8001B0EC: lh          $t8, 0x0($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X0);
    // 0x8001B0F0: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x8001B0F4: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x8001B0F8: andi        $t9, $t8, 0x7FFF
    ctx->r25 = ctx->r24 & 0X7FFF;
    // 0x8001B0FC: sh          $t9, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r25;
    // 0x8001B100: lw          $t0, 0x27AC($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X27AC);
    // 0x8001B104: addiu       $a0, $zero, 0x4000
    ctx->r4 = ADD32(0, 0X4000);
    // 0x8001B108: addiu       $a1, $zero, 0xF
    ctx->r5 = ADD32(0, 0XF);
    // 0x8001B10C: lh          $t4, 0x0($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X0);
    // 0x8001B110: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8001B114: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x8001B118: andi        $t5, $t4, 0x7FFF
    ctx->r13 = ctx->r12 & 0X7FFF;
    // 0x8001B11C: sh          $t5, 0x0($t0)
    MEM_H(0X0, ctx->r8) = ctx->r13;
    // 0x8001B120: lw          $t1, 0x27A4($t1)
    ctx->r9 = MEM_W(ctx->r9, 0X27A4);
    // 0x8001B124: lh          $t6, 0x0($t1)
    ctx->r14 = MEM_H(ctx->r9, 0X0);
    // 0x8001B128: andi        $t7, $t6, 0x7FFF
    ctx->r15 = ctx->r14 & 0X7FFF;
    // 0x8001B12C: sh          $t7, 0x0($t1)
    MEM_H(0X0, ctx->r9) = ctx->r15;
    // 0x8001B130: lw          $t2, 0x27A0($t2)
    ctx->r10 = MEM_W(ctx->r10, 0X27A0);
    // 0x8001B134: lh          $t8, 0x0($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X0);
    // 0x8001B138: andi        $t9, $t8, 0x7FFF
    ctx->r25 = ctx->r24 & 0X7FFF;
    // 0x8001B13C: sh          $t9, 0x0($t2)
    MEM_H(0X0, ctx->r10) = ctx->r25;
    // 0x8001B140: lw          $t3, 0x27B0($t3)
    ctx->r11 = MEM_W(ctx->r11, 0X27B0);
    // 0x8001B144: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001B148: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x8001B14C: lh          $t4, 0x0($t3)
    ctx->r12 = MEM_H(ctx->r11, 0X0);
    // 0x8001B150: andi        $t5, $t4, 0x7FFF
    ctx->r13 = ctx->r12 & 0X7FFF;
    // 0x8001B154: sh          $t5, 0x0($t3)
    MEM_H(0X0, ctx->r11) = ctx->r13;
    // 0x8001B158: jalr        $t9
    // 0x8001B15C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B15C: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_0:
    // 0x8001B160: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x8001B164: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B168: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001B16C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001B170: jalr        $t9
    // 0x8001B174: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001B174: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x8001B178: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001B17C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001B180: jr          $ra
    // 0x8001B184: nop

    return;
    // 0x8001B184: nop

;}
RECOMP_FUNC void func_8001B188(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B188: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B18C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B190: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B194: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x8001B198: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x8001B19C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x8001B1A0: sh          $zero, -0x7CA2($at)
    MEM_H(-0X7CA2, ctx->r1) = 0;
    // 0x8001B1A4: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x8001B1A8: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x8001B1AC: jalr        $t9
    // 0x8001B1B0: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B1B0: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    after_0:
    // 0x8001B1B4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B1B8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B1BC: jr          $ra
    // 0x8001B1C0: nop

    return;
    // 0x8001B1C0: nop

;}
RECOMP_FUNC void func_8001B1C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B1C4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B1C8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B1CC: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8001B1D0: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8001B1D4: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8001B1D8: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8001B1DC: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8001B1E0: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8001B1E4: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8001B1E8: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B1EC: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001B1F0: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8001B1F4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001B1F8: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B1FC: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8001B200: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B204: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8001B208: lw          $t9, 0x380C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X380C);
    // 0x8001B20C: jalr        $t9
    // 0x8001B210: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B210: nop

    after_0:
    // 0x8001B214: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001B218: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x8001B21C: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8001B220: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x8001B224: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B228: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B22C: jr          $ra
    // 0x8001B230: nop

    return;
    // 0x8001B230: nop

;}
RECOMP_FUNC void func_8001B234(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B234: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B238: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001B23C: lw          $t6, 0x2908($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2908);
    // 0x8001B240: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001B244: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001B248: lui         $t8, 0x802E
    ctx->r24 = S32(0X802E << 16);
    // 0x8001B24C: lui         $t9, 0x802A
    ctx->r25 = S32(0X802A << 16);
    // 0x8001B250: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8001B254: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001B258: addiu       $t9, $t9, 0x3B70
    ctx->r25 = ADD32(ctx->r25, 0X3B70);
    // 0x8001B25C: addiu       $t8, $t8, 0x3B70
    ctx->r24 = ADD32(ctx->r24, 0X3B70);
    // 0x8001B260: lui         $a1, 0x802A
    ctx->r5 = S32(0X802A << 16);
    // 0x8001B264: ori         $t7, $t6, 0x200
    ctx->r15 = ctx->r14 | 0X200;
    // 0x8001B268: sw          $t7, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r15;
    // 0x8001B26C: addiu       $a1, $a1, 0x3B70
    ctx->r5 = ADD32(ctx->r5, 0X3B70);
    // 0x8001B270: subu        $a2, $t8, $t9
    ctx->r6 = SUB32(ctx->r24, ctx->r25);
    // 0x8001B274: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    // 0x8001B278: jal         0x800013E8
    // 0x8001B27C: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    func_800013E8(rdram, ctx);
        goto after_0;
    // 0x8001B27C: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    after_0:
    // 0x8001B280: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B284: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001B288: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x8001B28C: jalr        $t9
    // 0x8001B290: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001B290: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_1:
    // 0x8001B294: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x8001B298: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B29C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001B2A0: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x8001B2A4: addu        $v0, $s0, $t1
    ctx->r2 = ADD32(ctx->r16, ctx->r9);
    // 0x8001B2A8: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B2AC: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B2B0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001B2B4: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x8001B2B8: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8001B2BC: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x8001B2C0: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x8001B2C4: lw          $t9, 0x380C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X380C);
    // 0x8001B2C8: jalr        $t9
    // 0x8001B2CC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001B2CC: nop

    after_2:
    // 0x8001B2D0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001B2D4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001B2D8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001B2DC: jr          $ra
    // 0x8001B2E0: nop

    return;
    // 0x8001B2E0: nop

;}
RECOMP_FUNC void func_8001B2E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B2E4: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8001B2E8: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8001B2EC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8001B2F0: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x8001B2F4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001B2F8: jal         0x800029EC
    // 0x8001B2FC: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
    func_800029EC(rdram, ctx);
        goto after_0;
    // 0x8001B2FC: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
    after_0:
    // 0x8001B300: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B304: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001B308: lw          $t6, 0x2908($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2908);
    // 0x8001B30C: sll         $t7, $t6, 5
    ctx->r15 = S32(ctx->r14 << 5);
    // 0x8001B310: bgezl       $t7, L_8001B398
    if (SIGNED(ctx->r15) >= 0) {
        // 0x8001B314: lw          $ra, 0x24($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X24);
            goto L_8001B398;
    }
    goto skip_0;
    // 0x8001B314: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    skip_0:
    // 0x8001B318: lh          $t8, 0x2B4E($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X2B4E);
    // 0x8001B31C: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x8001B320: addiu       $a0, $zero, 0x4000
    ctx->r4 = ADD32(0, 0X4000);
    // 0x8001B324: beq         $t8, $at, L_8001B344
    if (ctx->r24 == ctx->r1) {
        // 0x8001B328: addiu       $a1, $zero, 0xF
        ctx->r5 = ADD32(0, 0XF);
            goto L_8001B344;
    }
    // 0x8001B328: addiu       $a1, $zero, 0xF
    ctx->r5 = ADD32(0, 0XF);
    // 0x8001B32C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001B330: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x8001B334: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x8001B338: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x8001B33C: jalr        $t9
    // 0x8001B340: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001B340: sw          $zero, 0x10($sp)
    MEM_W(0X10, ctx->r29) = 0;
    after_1:
L_8001B344:
    // 0x8001B344: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B348: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001B34C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x8001B350: jalr        $t9
    // 0x8001B354: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001B354: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x8001B358: lh          $t0, 0xE($s0)
    ctx->r8 = MEM_H(ctx->r16, 0XE);
    // 0x8001B35C: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B360: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001B364: sll         $t1, $t0, 1
    ctx->r9 = S32(ctx->r8 << 1);
    // 0x8001B368: addu        $v0, $s0, $t1
    ctx->r2 = ADD32(ctx->r16, ctx->r9);
    // 0x8001B36C: lbu         $t4, 0x9($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B370: lbu         $t2, 0x8($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B374: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001B378: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x8001B37C: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x8001B380: sb          $t3, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r11;
    // 0x8001B384: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x8001B388: lw          $t9, 0x380C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X380C);
    // 0x8001B38C: jalr        $t9
    // 0x8001B390: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8001B390: nop

    after_3:
    // 0x8001B394: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
L_8001B398:
    // 0x8001B398: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8001B39C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8001B3A0: jr          $ra
    // 0x8001B3A4: nop

    return;
    // 0x8001B3A4: nop

;}
RECOMP_FUNC void func_8001B3A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B3A8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B3AC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001B3B0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B3B4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001B3B8: jal         0x800029EC
    // 0x8001B3BC: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
    func_800029EC(rdram, ctx);
        goto after_0;
    // 0x8001B3BC: lw          $a0, -0x53F4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53F4);
    after_0:
    // 0x8001B3C0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B3C4: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001B3C8: lh          $t6, 0x2B4E($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X2B4E);
    // 0x8001B3CC: lh          $t7, 0x2B4C($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X2B4C);
    // 0x8001B3D0: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x8001B3D4: addiu       $a2, $zero, 0x3
    ctx->r6 = ADD32(0, 0X3);
    // 0x8001B3D8: bne         $t6, $t7, L_8001B3F0
    if (ctx->r14 != ctx->r15) {
        // 0x8001B3DC: addiu       $a0, $v1, 0x8
        ctx->r4 = ADD32(ctx->r3, 0X8);
            goto L_8001B3F0;
    }
    // 0x8001B3DC: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x8001B3E0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B3E4: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x8001B3E8: jalr        $t9
    // 0x8001B3EC: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001B3EC: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    after_1:
L_8001B3F0:
    // 0x8001B3F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B3F4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B3F8: jr          $ra
    // 0x8001B3FC: nop

    return;
    // 0x8001B3FC: nop

;}
RECOMP_FUNC void func_8001B400(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B400: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B404: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B408: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001B40C: jal         0x80001438
    // 0x8001B410: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    func_80001438(rdram, ctx);
        goto after_0;
    // 0x8001B410: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    after_0:
    // 0x8001B414: jal         0x8001A52C
    // 0x8001B418: nop

    func_8001A52C(rdram, ctx);
        goto after_1;
    // 0x8001B418: nop

    after_1:
    // 0x8001B41C: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x8001B420: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B424: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001B428: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001B42C: jalr        $t9
    // 0x8001B430: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001B430: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_2:
    // 0x8001B434: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B438: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B43C: jr          $ra
    // 0x8001B440: nop

    return;
    // 0x8001B440: nop

;}
RECOMP_FUNC void func_8001B444(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B444: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B448: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001B44C: lw          $t6, 0x2908($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2908);
    // 0x8001B450: lui         $at, 0xFBFF
    ctx->r1 = S32(0XFBFF << 16);
    // 0x8001B454: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x8001B458: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x8001B45C: addiu       $at, $zero, -0x201
    ctx->r1 = ADD32(0, -0X201);
    // 0x8001B460: sw          $t7, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r15;
    // 0x8001B464: and         $t9, $t7, $at
    ctx->r25 = ctx->r15 & ctx->r1;
    // 0x8001B468: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x8001B46C: sw          $t9, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r25;
    // 0x8001B470: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001B474: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B478: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B47C: addiu       $t9, $t9, 0x1EA0
    ctx->r25 = ADD32(ctx->r25, 0X1EA0);
    // 0x8001B480: addiu       $a1, $a3, 0xE
    ctx->r5 = ADD32(ctx->r7, 0XE);
    // 0x8001B484: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x8001B488: sw          $a0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r4;
    // 0x8001B48C: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    // 0x8001B490: jalr        $t9
    // 0x8001B494: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B494: addiu       $a2, $zero, 0x4
    ctx->r6 = ADD32(0, 0X4);
    after_0:
    // 0x8001B498: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B49C: addiu       $t9, $t9, 0x1E30
    ctx->r25 = ADD32(ctx->r25, 0X1E30);
    // 0x8001B4A0: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x8001B4A4: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x8001B4A8: jalr        $t9
    // 0x8001B4AC: addiu       $a2, $zero, 0xA
    ctx->r6 = ADD32(0, 0XA);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001B4AC: addiu       $a2, $zero, 0xA
    ctx->r6 = ADD32(0, 0XA);
    after_1:
    // 0x8001B4B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B4B4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001B4B8: jr          $ra
    // 0x8001B4BC: nop

    return;
    // 0x8001B4BC: nop

;}
RECOMP_FUNC void func_8001B4C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B4C0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B4C4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B4C8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8001B4CC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8001B4D0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8001B4D4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8001B4D8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8001B4DC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8001B4E0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8001B4E4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B4E8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001B4EC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8001B4F0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001B4F4: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B4F8: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8001B4FC: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B500: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8001B504: lw          $t9, 0x3824($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3824);
    // 0x8001B508: jalr        $t9
    // 0x8001B50C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B50C: nop

    after_0:
    // 0x8001B510: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001B514: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x8001B518: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8001B51C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x8001B520: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B524: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B528: jr          $ra
    // 0x8001B52C: nop

    return;
    // 0x8001B52C: nop

;}
RECOMP_FUNC void func_8001B530(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B530: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B534: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001B538: lh          $t6, 0x2874($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X2874);
    // 0x8001B53C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8001B540: lui         $t8, 0x800B
    ctx->r24 = S32(0X800B << 16);
    // 0x8001B544: addiu       $t8, $t8, 0x2C38
    ctx->r24 = ADD32(ctx->r24, 0X2C38);
    // 0x8001B548: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x8001B54C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x8001B550: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8001B554: sw          $zero, 0x6F0($v0)
    MEM_W(0X6F0, ctx->r2) = 0;
    // 0x8001B558: sw          $zero, 0x6F4($v0)
    MEM_W(0X6F4, ctx->r2) = 0;
    // 0x8001B55C: sw          $zero, 0x700($v0)
    MEM_W(0X700, ctx->r2) = 0;
    // 0x8001B560: sw          $zero, 0x704($v0)
    MEM_W(0X704, ctx->r2) = 0;
    // 0x8001B564: sw          $zero, 0x708($v0)
    MEM_W(0X708, ctx->r2) = 0;
    // 0x8001B568: sw          $zero, 0x70C($v0)
    MEM_W(0X70C, ctx->r2) = 0;
    // 0x8001B56C: sw          $zero, 0x710($v0)
    MEM_W(0X710, ctx->r2) = 0;
    // 0x8001B570: sw          $zero, 0x714($v0)
    MEM_W(0X714, ctx->r2) = 0;
    // 0x8001B574: sw          $zero, 0x718($v0)
    MEM_W(0X718, ctx->r2) = 0;
    // 0x8001B578: addu        $v1, $t7, $t8
    ctx->r3 = ADD32(ctx->r15, ctx->r24);
    // 0x8001B57C: lw          $a2, 0x0($v1)
    ctx->r6 = MEM_W(ctx->r3, 0X0);
    // 0x8001B580: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8001B584: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001B588: beq         $a2, $zero, L_8001B5BC
    if (ctx->r6 == 0) {
        // 0x8001B58C: lui         $a1, 0x801D
        ctx->r5 = S32(0X801D << 16);
            goto L_8001B5BC;
    }
    // 0x8001B58C: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x8001B590: lw          $t9, 0x4($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X4);
    // 0x8001B594: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8001B598: addiu       $a3, $a3, -0x5160
    ctx->r7 = ADD32(ctx->r7, -0X5160);
    // 0x8001B59C: subu        $t0, $t9, $a2
    ctx->r8 = SUB32(ctx->r25, ctx->r6);
    // 0x8001B5A0: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    // 0x8001B5A4: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    // 0x8001B5A8: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    // 0x8001B5AC: jal         0x80012ED0
    // 0x8001B5B0: addiu       $a1, $a1, -0x5264
    ctx->r5 = ADD32(ctx->r5, -0X5264);
    func_80012ED0(rdram, ctx);
        goto after_0;
    // 0x8001B5B0: addiu       $a1, $a1, -0x5264
    ctx->r5 = ADD32(ctx->r5, -0X5264);
    after_0:
    // 0x8001B5B4: b           L_8001B5C8
    // 0x8001B5B8: nop

        goto L_8001B5C8;
    // 0x8001B5B8: nop

L_8001B5BC:
    // 0x8001B5BC: lw          $t1, 0x2ADC($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X2ADC);
    // 0x8001B5C0: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x8001B5C4: sw          $t2, 0x2ADC($v0)
    MEM_W(0X2ADC, ctx->r2) = ctx->r10;
L_8001B5C8:
    // 0x8001B5C8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B5CC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001B5D0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x8001B5D4: jalr        $t9
    // 0x8001B5D8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001B5D8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_1:
    // 0x8001B5DC: lh          $t3, 0xE($s0)
    ctx->r11 = MEM_H(ctx->r16, 0XE);
    // 0x8001B5E0: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B5E4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001B5E8: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x8001B5EC: addu        $v0, $s0, $t4
    ctx->r2 = ADD32(ctx->r16, ctx->r12);
    // 0x8001B5F0: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B5F4: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B5F8: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001B5FC: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8001B600: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x8001B604: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x8001B608: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x8001B60C: lw          $t9, 0x3824($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3824);
    // 0x8001B610: jalr        $t9
    // 0x8001B614: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001B614: nop

    after_2:
    // 0x8001B618: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x8001B61C: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8001B620: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8001B624: jr          $ra
    // 0x8001B628: nop

    return;
    // 0x8001B628: nop

;}
RECOMP_FUNC void func_8001B62C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B62C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001B630: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001B634: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x8001B638: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x8001B63C: lw          $t6, 0x2ADC($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X2ADC);
    // 0x8001B640: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001B644: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001B648: beql        $t6, $zero, L_8001B708
    if (ctx->r14 == 0) {
        // 0x8001B64C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8001B708;
    }
    goto skip_0;
    // 0x8001B64C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8001B650: lh          $t7, 0x2874($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X2874);
    // 0x8001B654: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001B658: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001B65C: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x8001B660: addu        $a1, $a1, $t8
    ctx->r5 = ADD32(ctx->r5, ctx->r24);
    // 0x8001B664: jal         0x8000233C
    // 0x8001B668: lh          $a1, 0x382C($a1)
    ctx->r5 = MEM_H(ctx->r5, 0X382C);
    func_8000233C(rdram, ctx);
        goto after_0;
    // 0x8001B668: lh          $a1, 0x382C($a1)
    ctx->r5 = MEM_H(ctx->r5, 0X382C);
    after_0:
    // 0x8001B66C: addiu       $t9, $zero, -0x1
    ctx->r25 = ADD32(0, -0X1);
    // 0x8001B670: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001B674: sw          $v0, 0x291C($s0)
    MEM_W(0X291C, ctx->r16) = ctx->r2;
    // 0x8001B678: sw          $t9, 0x2AF8($s0)
    MEM_W(0X2AF8, ctx->r16) = ctx->r25;
    // 0x8001B67C: addiu       $a0, $a0, 0x3708
    ctx->r4 = ADD32(ctx->r4, 0X3708);
    // 0x8001B680: jal         0x80000F30
    // 0x8001B684: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    bzero_recomp(rdram, ctx);
        goto after_1;
    // 0x8001B684: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    after_1:
    // 0x8001B688: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001B68C: addiu       $a0, $a0, 0x3890
    ctx->r4 = ADD32(ctx->r4, 0X3890);
    // 0x8001B690: jal         0x80000F30
    // 0x8001B694: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    bzero_recomp(rdram, ctx);
        goto after_2;
    // 0x8001B694: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    after_2:
    // 0x8001B698: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001B69C: addiu       $a0, $a0, 0x3A18
    ctx->r4 = ADD32(ctx->r4, 0X3A18);
    // 0x8001B6A0: jal         0x80000F30
    // 0x8001B6A4: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    bzero_recomp(rdram, ctx);
        goto after_3;
    // 0x8001B6A4: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    after_3:
    // 0x8001B6A8: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001B6AC: addiu       $a0, $a0, 0x3BA0
    ctx->r4 = ADD32(ctx->r4, 0X3BA0);
    // 0x8001B6B0: jal         0x80000F30
    // 0x8001B6B4: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    bzero_recomp(rdram, ctx);
        goto after_4;
    // 0x8001B6B4: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    after_4:
    // 0x8001B6B8: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001B6BC: addiu       $a0, $a0, 0x3D28
    ctx->r4 = ADD32(ctx->r4, 0X3D28);
    // 0x8001B6C0: jal         0x80000F30
    // 0x8001B6C4: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    bzero_recomp(rdram, ctx);
        goto after_5;
    // 0x8001B6C4: addiu       $a1, $zero, 0x188
    ctx->r5 = ADD32(0, 0X188);
    after_5:
    // 0x8001B6C8: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001B6CC: addiu       $a0, $a0, 0x3EB8
    ctx->r4 = ADD32(ctx->r4, 0X3EB8);
    // 0x8001B6D0: jal         0x80000F30
    // 0x8001B6D4: addiu       $a1, $zero, 0x60
    ctx->r5 = ADD32(0, 0X60);
    bzero_recomp(rdram, ctx);
        goto after_6;
    // 0x8001B6D4: addiu       $a1, $zero, 0x60
    ctx->r5 = ADD32(0, 0X60);
    after_6:
    // 0x8001B6D8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x8001B6DC: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8001B6E0: swc1        $f4, 0x3F18($at)
    MEM_W(0X3F18, ctx->r1) = ctx->f4.u32l;
    // 0x8001B6E4: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8001B6E8: sh          $zero, 0x3F1C($at)
    MEM_H(0X3F1C, ctx->r1) = 0;
    // 0x8001B6EC: jal         0x800029EC
    // 0x8001B6F0: lw          $a0, 0x291C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X291C);
    func_800029EC(rdram, ctx);
        goto after_7;
    // 0x8001B6F0: lw          $a0, 0x291C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X291C);
    after_7:
    // 0x8001B6F4: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x8001B6F8: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001B6FC: jalr        $t9
    // 0x8001B700: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x8001B700: nop

    after_8:
    // 0x8001B704: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001B708:
    // 0x8001B708: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001B70C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001B710: jr          $ra
    // 0x8001B714: nop

    return;
    // 0x8001B714: nop

;}
RECOMP_FUNC void func_8001B718(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B718: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B71C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B720: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8001B724: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8001B728: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8001B72C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8001B730: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8001B734: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8001B738: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8001B73C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B740: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001B744: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8001B748: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001B74C: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B750: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8001B754: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B758: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8001B75C: lw          $t9, 0x3834($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3834);
    // 0x8001B760: jalr        $t9
    // 0x8001B764: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B764: nop

    after_0:
    // 0x8001B768: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001B76C: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x8001B770: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8001B774: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x8001B778: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B77C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B780: jr          $ra
    // 0x8001B784: nop

    return;
    // 0x8001B784: nop

;}
RECOMP_FUNC void func_8001B788(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B788: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B78C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001B790: lh          $t6, 0x2BB8($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X2BB8);
    // 0x8001B794: lh          $t7, 0x2BBA($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X2BBA);
    // 0x8001B798: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001B79C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8001B7A0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B7A4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B7A8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001B7AC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x8001B7B0: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    // 0x8001B7B4: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x8001B7B8: sh          $t6, 0x28D0($v0)
    MEM_H(0X28D0, ctx->r2) = ctx->r14;
    // 0x8001B7BC: jalr        $t9
    // 0x8001B7C0: sh          $t7, 0x28D2($v0)
    MEM_H(0X28D2, ctx->r2) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001B7C0: sh          $t7, 0x28D2($v0)
    MEM_H(0X28D2, ctx->r2) = ctx->r15;
    after_0:
    // 0x8001B7C4: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001B7C8: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B7CC: lh          $t8, 0xE($a0)
    ctx->r24 = MEM_H(ctx->r4, 0XE);
    // 0x8001B7D0: sll         $t0, $t8, 1
    ctx->r8 = S32(ctx->r24 << 1);
    // 0x8001B7D4: addu        $v0, $a0, $t0
    ctx->r2 = ADD32(ctx->r4, ctx->r8);
    // 0x8001B7D8: lbu         $t3, 0x9($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B7DC: lbu         $t1, 0x8($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B7E0: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001B7E4: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x8001B7E8: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x8001B7EC: sb          $t2, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r10;
    // 0x8001B7F0: addu        $t9, $t9, $t4
    ctx->r25 = ADD32(ctx->r25, ctx->r12);
    // 0x8001B7F4: lw          $t9, 0x3834($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3834);
    // 0x8001B7F8: jalr        $t9
    // 0x8001B7FC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001B7FC: nop

    after_1:
    // 0x8001B800: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B804: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001B808: jr          $ra
    // 0x8001B80C: nop

    return;
    // 0x8001B80C: nop

;}
RECOMP_FUNC void func_8001B810(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B810: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001B814: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001B818: lw          $t6, 0x2BC8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2BC8);
    // 0x8001B81C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8001B820: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001B824: bne         $t6, $zero, L_8001B938
    if (ctx->r14 != 0) {
        // 0x8001B828: sw          $a0, 0x28($sp)
        MEM_W(0X28, ctx->r29) = ctx->r4;
            goto L_8001B938;
    }
    // 0x8001B828: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x8001B82C: lw          $t7, 0x2BCC($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X2BCC);
    // 0x8001B830: bnel        $t7, $zero, L_8001B93C
    if (ctx->r15 != 0) {
        // 0x8001B834: lw          $t2, 0x28($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X28);
            goto L_8001B93C;
    }
    goto skip_0;
    // 0x8001B834: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
    skip_0:
    // 0x8001B838: lh          $a0, 0x28D0($v0)
    ctx->r4 = MEM_H(ctx->r2, 0X28D0);
    // 0x8001B83C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8001B840: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x8001B844: beql        $a0, $at, L_8001B860
    if (ctx->r4 == ctx->r1) {
        // 0x8001B848: lh          $t8, 0x2874($v0)
        ctx->r24 = MEM_H(ctx->r2, 0X2874);
            goto L_8001B860;
    }
    goto skip_1;
    // 0x8001B848: lh          $t8, 0x2874($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X2874);
    skip_1:
    // 0x8001B84C: beql        $a0, $a1, L_8001B8B8
    if (ctx->r4 == ctx->r5) {
        // 0x8001B850: lh          $t9, 0x2874($v0)
        ctx->r25 = MEM_H(ctx->r2, 0X2874);
            goto L_8001B8B8;
    }
    goto skip_2;
    // 0x8001B850: lh          $t9, 0x2874($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X2874);
    skip_2:
    // 0x8001B854: b           L_8001B930
    // 0x8001B858: nop

        goto L_8001B930;
    // 0x8001B858: nop

    // 0x8001B85C: lh          $t8, 0x2874($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X2874);
L_8001B860:
    // 0x8001B860: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x8001B864: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001B868: beq         $t8, $at, L_8001B8A4
    if (ctx->r24 == ctx->r1) {
        // 0x8001B86C: addiu       $a0, $a0, -0x55A0
        ctx->r4 = ADD32(ctx->r4, -0X55A0);
            goto L_8001B8A4;
    }
    // 0x8001B86C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8001B870: lui         $v1, 0x8000
    ctx->r3 = S32(0X8000 << 16);
    // 0x8001B874: addiu       $v1, $v1, 0x48A0
    ctx->r3 = ADD32(ctx->r3, 0X48A0);
    // 0x8001B878: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x8001B87C: jalr        $v1
    // 0x8001B880: addiu       $a1, $zero, 0x52
    ctx->r5 = ADD32(0, 0X52);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_0;
    // 0x8001B880: addiu       $a1, $zero, 0x52
    ctx->r5 = ADD32(0, 0X52);
    after_0:
    // 0x8001B884: beq         $v0, $zero, L_8001B8A4
    if (ctx->r2 == 0) {
        // 0x8001B888: lw          $v1, 0x1C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X1C);
            goto L_8001B8A4;
    }
    // 0x8001B888: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x8001B88C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001B890: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8001B894: jalr        $v1
    // 0x8001B898: addiu       $a1, $zero, 0x53
    ctx->r5 = ADD32(0, 0X53);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_1;
    // 0x8001B898: addiu       $a1, $zero, 0x53
    ctx->r5 = ADD32(0, 0X53);
    after_1:
    // 0x8001B89C: bnel        $v0, $zero, L_8001B93C
    if (ctx->r2 != 0) {
        // 0x8001B8A0: lw          $t2, 0x28($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X28);
            goto L_8001B93C;
    }
    goto skip_3;
    // 0x8001B8A0: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
    skip_3:
L_8001B8A4:
    // 0x8001B8A4: jal         0x8001A4E0
    // 0x8001B8A8: nop

    func_8001A4E0(rdram, ctx);
        goto after_2;
    // 0x8001B8A8: nop

    after_2:
    // 0x8001B8AC: b           L_8001B93C
    // 0x8001B8B0: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
        goto L_8001B93C;
    // 0x8001B8B0: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
    // 0x8001B8B4: lh          $t9, 0x2874($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X2874);
L_8001B8B8:
    // 0x8001B8B8: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x8001B8BC: lui         $v1, 0x8000
    ctx->r3 = S32(0X8000 << 16);
    // 0x8001B8C0: bne         $t9, $at, L_8001B8FC
    if (ctx->r25 != ctx->r1) {
        // 0x8001B8C4: addiu       $v1, $v1, 0x48A0
        ctx->r3 = ADD32(ctx->r3, 0X48A0);
            goto L_8001B8FC;
    }
    // 0x8001B8C4: addiu       $v1, $v1, 0x48A0
    ctx->r3 = ADD32(ctx->r3, 0X48A0);
    // 0x8001B8C8: bne         $a1, $a0, L_8001B930
    if (ctx->r5 != ctx->r4) {
        // 0x8001B8CC: nop
    
            goto L_8001B930;
    }
    // 0x8001B8CC: nop

    // 0x8001B8D0: lhu         $t0, 0x2904($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X2904);
    // 0x8001B8D4: addiu       $at, $zero, 0x82
    ctx->r1 = ADD32(0, 0X82);
    // 0x8001B8D8: addiu       $t1, $zero, 0x9
    ctx->r9 = ADD32(0, 0X9);
    // 0x8001B8DC: beq         $t0, $at, L_8001B930
    if (ctx->r8 == ctx->r1) {
        // 0x8001B8E0: andi        $a0, $t1, 0xFFFF
        ctx->r4 = ctx->r9 & 0XFFFF;
            goto L_8001B930;
    }
    // 0x8001B8E0: andi        $a0, $t1, 0xFFFF
    ctx->r4 = ctx->r9 & 0XFFFF;
    // 0x8001B8E4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001B8E8: addiu       $t9, $t9, 0x731C
    ctx->r25 = ADD32(ctx->r25, 0X731C);
    // 0x8001B8EC: jalr        $t9
    // 0x8001B8F0: sh          $t1, 0x2904($v0)
    MEM_H(0X2904, ctx->r2) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8001B8F0: sh          $t1, 0x2904($v0)
    MEM_H(0X2904, ctx->r2) = ctx->r9;
    after_3:
    // 0x8001B8F4: b           L_8001B93C
    // 0x8001B8F8: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
        goto L_8001B93C;
    // 0x8001B8F8: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
L_8001B8FC:
    // 0x8001B8FC: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001B900: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8001B904: addiu       $a1, $zero, 0x52
    ctx->r5 = ADD32(0, 0X52);
    // 0x8001B908: jalr        $v1
    // 0x8001B90C: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_4;
    // 0x8001B90C: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_4:
    // 0x8001B910: beq         $v0, $zero, L_8001B930
    if (ctx->r2 == 0) {
        // 0x8001B914: lw          $v1, 0x1C($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X1C);
            goto L_8001B930;
    }
    // 0x8001B914: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x8001B918: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001B91C: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8001B920: jalr        $v1
    // 0x8001B924: addiu       $a1, $zero, 0x53
    ctx->r5 = ADD32(0, 0X53);
    LOOKUP_FUNC(ctx->r3)(rdram, ctx);
        goto after_5;
    // 0x8001B924: addiu       $a1, $zero, 0x53
    ctx->r5 = ADD32(0, 0X53);
    after_5:
    // 0x8001B928: bnel        $v0, $zero, L_8001B93C
    if (ctx->r2 != 0) {
        // 0x8001B92C: lw          $t2, 0x28($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X28);
            goto L_8001B93C;
    }
    goto skip_4;
    // 0x8001B92C: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
    skip_4:
L_8001B930:
    // 0x8001B930: jal         0x8001A4E0
    // 0x8001B934: nop

    func_8001A4E0(rdram, ctx);
        goto after_6;
    // 0x8001B934: nop

    after_6:
L_8001B938:
    // 0x8001B938: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
L_8001B93C:
    // 0x8001B93C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001B940: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001B944: addiu       $a0, $t2, 0x8
    ctx->r4 = ADD32(ctx->r10, 0X8);
    // 0x8001B948: jalr        $t9
    // 0x8001B94C: addiu       $a1, $t2, 0xE
    ctx->r5 = ADD32(ctx->r10, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x8001B94C: addiu       $a1, $t2, 0xE
    ctx->r5 = ADD32(ctx->r10, 0XE);
    after_7:
    // 0x8001B950: lw          $t3, 0x28($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X28);
    // 0x8001B954: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001B958: lh          $t4, 0xE($t3)
    ctx->r12 = MEM_H(ctx->r11, 0XE);
    // 0x8001B95C: sll         $t5, $t4, 1
    ctx->r13 = S32(ctx->r12 << 1);
    // 0x8001B960: addu        $v0, $t3, $t5
    ctx->r2 = ADD32(ctx->r11, ctx->r13);
    // 0x8001B964: lbu         $t8, 0x9($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X9);
    // 0x8001B968: lbu         $t6, 0x8($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X8);
    // 0x8001B96C: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001B970: sll         $t0, $t8, 2
    ctx->r8 = S32(ctx->r24 << 2);
    // 0x8001B974: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x8001B978: sb          $t7, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r15;
    // 0x8001B97C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8001B980: lw          $t9, 0x3834($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3834);
    // 0x8001B984: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x8001B988: jalr        $t9
    // 0x8001B98C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x8001B98C: nop

    after_8:
    // 0x8001B990: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001B994: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8001B998: jr          $ra
    // 0x8001B99C: nop

    return;
    // 0x8001B99C: nop

;}
RECOMP_FUNC void func_8001B9A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001B9A0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8001B9A4: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8001B9A8: lh          $t6, 0x28D0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X28D0);
    // 0x8001B9AC: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8001B9B0: lui         $t8, 0x800B
    ctx->r24 = S32(0X800B << 16);
    // 0x8001B9B4: addiu       $t8, $t8, 0x2DE8
    ctx->r24 = ADD32(ctx->r24, 0X2DE8);
    // 0x8001B9B8: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x8001B9BC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x8001B9C0: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x8001B9C4: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x8001B9C8: lw          $a2, 0x0($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X0);
    // 0x8001B9CC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8001B9D0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001B9D4: beq         $a2, $zero, L_8001BA08
    if (ctx->r6 == 0) {
        // 0x8001B9D8: lui         $a1, 0x801D
        ctx->r5 = S32(0X801D << 16);
            goto L_8001BA08;
    }
    // 0x8001B9D8: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x8001B9DC: lw          $t9, 0x4($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X4);
    // 0x8001B9E0: lui         $a3, 0x802E
    ctx->r7 = S32(0X802E << 16);
    // 0x8001B9E4: addiu       $a3, $a3, 0x3B70
    ctx->r7 = ADD32(ctx->r7, 0X3B70);
    // 0x8001B9E8: subu        $t0, $t9, $a2
    ctx->r8 = SUB32(ctx->r25, ctx->r6);
    // 0x8001B9EC: sw          $t0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r8;
    // 0x8001B9F0: sw          $zero, 0x14($sp)
    MEM_W(0X14, ctx->r29) = 0;
    // 0x8001B9F4: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    // 0x8001B9F8: jal         0x80012ED0
    // 0x8001B9FC: addiu       $a1, $a1, -0x521C
    ctx->r5 = ADD32(ctx->r5, -0X521C);
    func_80012ED0(rdram, ctx);
        goto after_0;
    // 0x8001B9FC: addiu       $a1, $a1, -0x521C
    ctx->r5 = ADD32(ctx->r5, -0X521C);
    after_0:
    // 0x8001BA00: b           L_8001BA14
    // 0x8001BA04: nop

        goto L_8001BA14;
    // 0x8001BA04: nop

L_8001BA08:
    // 0x8001BA08: lw          $t1, 0x2B24($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X2B24);
    // 0x8001BA0C: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x8001BA10: sw          $t2, 0x2B24($v1)
    MEM_W(0X2B24, ctx->r3) = ctx->r10;
L_8001BA14:
    // 0x8001BA14: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001BA18: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001BA1C: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x8001BA20: jalr        $t9
    // 0x8001BA24: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001BA24: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_1:
    // 0x8001BA28: lh          $t3, 0xE($s0)
    ctx->r11 = MEM_H(ctx->r16, 0XE);
    // 0x8001BA2C: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001BA30: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001BA34: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x8001BA38: addu        $v0, $s0, $t4
    ctx->r2 = ADD32(ctx->r16, ctx->r12);
    // 0x8001BA3C: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x8001BA40: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x8001BA44: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001BA48: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8001BA4C: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x8001BA50: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x8001BA54: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x8001BA58: lw          $t9, 0x3834($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3834);
    // 0x8001BA5C: jalr        $t9
    // 0x8001BA60: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001BA60: nop

    after_2:
    // 0x8001BA64: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x8001BA68: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x8001BA6C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8001BA70: jr          $ra
    // 0x8001BA74: nop

    return;
    // 0x8001BA74: nop

;}
RECOMP_FUNC void func_8001BA78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BA78: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8001BA7C: lw          $t6, -0x521C($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X521C);
    // 0x8001BA80: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001BA84: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001BA88: beq         $t6, $zero, L_8001BB2C
    if (ctx->r14 == 0) {
        // 0x8001BA8C: sw          $a0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r4;
            goto L_8001BB2C;
    }
    // 0x8001BA8C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001BA90: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BA94: jal         0x80002410
    // 0x8001BA98: addiu       $a1, $zero, 0x1AB
    ctx->r5 = ADD32(0, 0X1AB);
    func_80002410(rdram, ctx);
        goto after_0;
    // 0x8001BA98: addiu       $a1, $zero, 0x1AB
    ctx->r5 = ADD32(0, 0X1AB);
    after_0:
    // 0x8001BA9C: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8001BAA0: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8001BAA4: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x8001BAA8: sw          $v0, 0x2924($v1)
    MEM_W(0X2924, ctx->r3) = ctx->r2;
    // 0x8001BAAC: addiu       $a0, $a0, -0x6B50
    ctx->r4 = ADD32(ctx->r4, -0X6B50);
    // 0x8001BAB0: jal         0x80000F30
    // 0x8001BAB4: addiu       $a1, $zero, 0x1A4
    ctx->r5 = ADD32(0, 0X1A4);
    bzero_recomp(rdram, ctx);
        goto after_1;
    // 0x8001BAB4: addiu       $a1, $zero, 0x1A4
    ctx->r5 = ADD32(0, 0X1A4);
    after_1:
    // 0x8001BAB8: lui         $a0, 0x8010
    ctx->r4 = S32(0X8010 << 16);
    // 0x8001BABC: addiu       $a0, $a0, -0x68D0
    ctx->r4 = ADD32(ctx->r4, -0X68D0);
    // 0x8001BAC0: jal         0x80000F30
    // 0x8001BAC4: addiu       $a1, $zero, 0x28
    ctx->r5 = ADD32(0, 0X28);
    bzero_recomp(rdram, ctx);
        goto after_2;
    // 0x8001BAC4: addiu       $a1, $zero, 0x28
    ctx->r5 = ADD32(0, 0X28);
    after_2:
    // 0x8001BAC8: lui         $v1, 0x8010
    ctx->r3 = S32(0X8010 << 16);
    // 0x8001BACC: lui         $t7, 0x8010
    ctx->r15 = S32(0X8010 << 16);
    // 0x8001BAD0: addiu       $t7, $t7, -0x6B50
    ctx->r15 = ADD32(ctx->r15, -0X6B50);
    // 0x8001BAD4: addiu       $v1, $v1, -0x69AC
    ctx->r3 = ADD32(ctx->r3, -0X69AC);
    // 0x8001BAD8: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x8001BADC: addiu       $t8, $zero, 0x6
    ctx->r24 = ADD32(0, 0X6);
    // 0x8001BAE0: sw          $t8, 0x0($t7)
    MEM_W(0X0, ctx->r15) = ctx->r24;
    // 0x8001BAE4: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x8001BAE8: lui         $t9, 0x8004
    ctx->r25 = S32(0X8004 << 16);
    // 0x8001BAEC: addiu       $t9, $t9, 0x5640
    ctx->r25 = ADD32(ctx->r25, 0X5640);
    // 0x8001BAF0: addiu       $t0, $v0, 0x4
    ctx->r8 = ADD32(ctx->r2, 0X4);
    // 0x8001BAF4: sw          $t0, 0x184($v0)
    MEM_W(0X184, ctx->r2) = ctx->r8;
    // 0x8001BAF8: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x8001BAFC: addiu       $t1, $v0, 0x188
    ctx->r9 = ADD32(ctx->r2, 0X188);
    // 0x8001BB00: jalr        $t9
    // 0x8001BB04: sw          $t1, 0x1A0($v0)
    MEM_W(0X1A0, ctx->r2) = ctx->r9;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8001BB04: sw          $t1, 0x1A0($v0)
    MEM_W(0X1A0, ctx->r2) = ctx->r9;
    after_3:
    // 0x8001BB08: lui         $at, 0x8010
    ctx->r1 = S32(0X8010 << 16);
    // 0x8001BB0C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001BB10: sw          $zero, -0x6BD8($at)
    MEM_W(-0X6BD8, ctx->r1) = 0;
    // 0x8001BB14: jal         0x800029EC
    // 0x8001BB18: lw          $a0, -0x541C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X541C);
    func_800029EC(rdram, ctx);
        goto after_4;
    // 0x8001BB18: lw          $a0, -0x541C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X541C);
    after_4:
    // 0x8001BB1C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001BB20: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001BB24: jalr        $t9
    // 0x8001BB28: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x8001BB28: nop

    after_5:
L_8001BB2C:
    // 0x8001BB2C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001BB30: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001BB34: jr          $ra
    // 0x8001BB38: nop

    return;
    // 0x8001BB38: nop

;}
RECOMP_FUNC void func_8001BB3C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BB3C: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x8001BB40: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x8001BB44: lw          $v0, 0x2908($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X2908);
    // 0x8001BB48: andi        $t6, $v0, 0x200
    ctx->r14 = ctx->r2 & 0X200;
    // 0x8001BB4C: bne         $t6, $zero, L_8001BC54
    if (ctx->r14 != 0) {
        // 0x8001BB50: andi        $t7, $v0, 0x40
        ctx->r15 = ctx->r2 & 0X40;
            goto L_8001BC54;
    }
    // 0x8001BB50: andi        $t7, $v0, 0x40
    ctx->r15 = ctx->r2 & 0X40;
    // 0x8001BB54: bne         $t7, $zero, L_8001BC54
    if (ctx->r15 != 0) {
        // 0x8001BB58: nop
    
            goto L_8001BC54;
    }
    // 0x8001BB58: nop

    // 0x8001BB5C: lw          $t8, 0x2BC8($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X2BC8);
    // 0x8001BB60: bne         $t8, $zero, L_8001BC54
    if (ctx->r24 != 0) {
        // 0x8001BB64: nop
    
            goto L_8001BC54;
    }
    // 0x8001BB64: nop

    // 0x8001BB68: lhu         $t1, 0x2866($a1)
    ctx->r9 = MEM_HU(ctx->r5, 0X2866);
    // 0x8001BB6C: lw          $t9, 0x2868($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X2868);
    // 0x8001BB70: addiu       $a2, $zero, 0x258
    ctx->r6 = ADD32(0, 0X258);
    // 0x8001BB74: addiu       $t2, $t1, 0x3E8
    ctx->r10 = ADD32(ctx->r9, 0X3E8);
    // 0x8001BB78: andi        $v0, $t2, 0xFFFF
    ctx->r2 = ctx->r10 & 0XFFFF;
    // 0x8001BB7C: slti        $at, $v0, 0x258
    ctx->r1 = SIGNED(ctx->r2) < 0X258 ? 1 : 0;
    // 0x8001BB80: addiu       $t0, $t9, 0x1
    ctx->r8 = ADD32(ctx->r25, 0X1);
    // 0x8001BB84: sw          $t0, 0x2868($a1)
    MEM_W(0X2868, ctx->r5) = ctx->r8;
    // 0x8001BB88: bne         $at, $zero, L_8001BC54
    if (ctx->r1 != 0) {
        // 0x8001BB8C: sh          $t2, 0x2866($a1)
        MEM_H(0X2866, ctx->r5) = ctx->r10;
            goto L_8001BC54;
    }
    // 0x8001BB8C: sh          $t2, 0x2866($a1)
    MEM_H(0X2866, ctx->r5) = ctx->r10;
    // 0x8001BB90: div         $zero, $v0, $a2
    lo = S32(S64(S32(ctx->r2)) / S64(S32(ctx->r6))); hi = S32(S64(S32(ctx->r2)) % S64(S32(ctx->r6)));
    // 0x8001BB94: lh          $t4, 0x2864($a1)
    ctx->r12 = MEM_H(ctx->r5, 0X2864);
    // 0x8001BB98: mflo        $v1
    ctx->r3 = lo;
    // 0x8001BB9C: mfhi        $t3
    ctx->r11 = hi;
    // 0x8001BBA0: addu        $t5, $t4, $v1
    ctx->r13 = ADD32(ctx->r12, ctx->r3);
    // 0x8001BBA4: bne         $a2, $zero, L_8001BBB0
    if (ctx->r6 != 0) {
        // 0x8001BBA8: nop
    
            goto L_8001BBB0;
    }
    // 0x8001BBA8: nop

    // 0x8001BBAC: break       7
    do_break(2147597228);
L_8001BBB0:
    // 0x8001BBB0: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x8001BBB4: bne         $a2, $at, L_8001BBC8
    if (ctx->r6 != ctx->r1) {
        // 0x8001BBB8: lui         $at, 0x8000
        ctx->r1 = S32(0X8000 << 16);
            goto L_8001BBC8;
    }
    // 0x8001BBB8: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x8001BBBC: bne         $v0, $at, L_8001BBC8
    if (ctx->r2 != ctx->r1) {
        // 0x8001BBC0: nop
    
            goto L_8001BBC8;
    }
    // 0x8001BBC0: nop

    // 0x8001BBC4: break       6
    do_break(2147597252);
L_8001BBC8:
    // 0x8001BBC8: sh          $t5, 0x2864($a1)
    MEM_H(0X2864, ctx->r5) = ctx->r13;
    // 0x8001BBCC: lh          $a0, 0x2864($a1)
    ctx->r4 = MEM_H(ctx->r5, 0X2864);
    // 0x8001BBD0: sh          $t3, 0x2866($a1)
    MEM_H(0X2866, ctx->r5) = ctx->r11;
    // 0x8001BBD4: slti        $at, $a0, 0x3C
    ctx->r1 = SIGNED(ctx->r4) < 0X3C ? 1 : 0;
    // 0x8001BBD8: bne         $at, $zero, L_8001BC54
    if (ctx->r1 != 0) {
        // 0x8001BBDC: nop
    
            goto L_8001BC54;
    }
    // 0x8001BBDC: nop

    // 0x8001BBE0: lh          $t7, 0x2862($a1)
    ctx->r15 = MEM_H(ctx->r5, 0X2862);
    // 0x8001BBE4: addiu       $t6, $a0, -0x3C
    ctx->r14 = ADD32(ctx->r4, -0X3C);
    // 0x8001BBE8: sh          $t6, 0x2864($a1)
    MEM_H(0X2864, ctx->r5) = ctx->r14;
    // 0x8001BBEC: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001BBF0: sh          $t8, 0x2862($a1)
    MEM_H(0X2862, ctx->r5) = ctx->r24;
    // 0x8001BBF4: lh          $t9, 0x2862($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X2862);
    // 0x8001BBF8: slti        $at, $t9, 0x3C
    ctx->r1 = SIGNED(ctx->r25) < 0X3C ? 1 : 0;
    // 0x8001BBFC: bne         $at, $zero, L_8001BC54
    if (ctx->r1 != 0) {
        // 0x8001BC00: nop
    
            goto L_8001BC54;
    }
    // 0x8001BC00: nop

    // 0x8001BC04: lh          $t0, 0x2860($a1)
    ctx->r8 = MEM_H(ctx->r5, 0X2860);
    // 0x8001BC08: sh          $zero, 0x2862($a1)
    MEM_H(0X2862, ctx->r5) = 0;
    // 0x8001BC0C: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x8001BC10: sh          $t1, 0x2860($a1)
    MEM_H(0X2860, ctx->r5) = ctx->r9;
    // 0x8001BC14: lh          $t2, 0x2860($a1)
    ctx->r10 = MEM_H(ctx->r5, 0X2860);
    // 0x8001BC18: slti        $at, $t2, 0x18
    ctx->r1 = SIGNED(ctx->r10) < 0X18 ? 1 : 0;
    // 0x8001BC1C: bne         $at, $zero, L_8001BC54
    if (ctx->r1 != 0) {
        // 0x8001BC20: nop
    
            goto L_8001BC54;
    }
    // 0x8001BC20: nop

    // 0x8001BC24: lh          $t3, 0x285E($a1)
    ctx->r11 = MEM_H(ctx->r5, 0X285E);
    // 0x8001BC28: sh          $zero, 0x2860($a1)
    MEM_H(0X2860, ctx->r5) = 0;
    // 0x8001BC2C: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x8001BC30: sh          $t4, 0x285E($a1)
    MEM_H(0X285E, ctx->r5) = ctx->r12;
    // 0x8001BC34: lh          $t5, 0x285E($a1)
    ctx->r13 = MEM_H(ctx->r5, 0X285E);
    // 0x8001BC38: slti        $at, $t5, 0x7
    ctx->r1 = SIGNED(ctx->r13) < 0X7 ? 1 : 0;
    // 0x8001BC3C: bne         $at, $zero, L_8001BC54
    if (ctx->r1 != 0) {
        // 0x8001BC40: nop
    
            goto L_8001BC54;
    }
    // 0x8001BC40: nop

    // 0x8001BC44: lh          $t6, 0x285C($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X285C);
    // 0x8001BC48: sh          $zero, 0x285E($a1)
    MEM_H(0X285E, ctx->r5) = 0;
    // 0x8001BC4C: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x8001BC50: sh          $t7, 0x285C($a1)
    MEM_H(0X285C, ctx->r5) = ctx->r15;
L_8001BC54:
    // 0x8001BC54: jr          $ra
    // 0x8001BC58: nop

    return;
    // 0x8001BC58: nop

;}
RECOMP_FUNC void func_8001BC5C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BC5C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001BC60: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001BC64: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8001BC68: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8001BC6C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8001BC70: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8001BC74: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8001BC78: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8001BC7C: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8001BC80: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x8001BC84: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001BC88: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8001BC8C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001BC90: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x8001BC94: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8001BC98: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001BC9C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8001BCA0: lw          $t9, 0x3848($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X3848);
    // 0x8001BCA4: jalr        $t9
    // 0x8001BCA8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001BCA8: nop

    after_0:
    // 0x8001BCAC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001BCB0: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x8001BCB4: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8001BCB8: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x8001BCBC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001BCC0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001BCC4: jr          $ra
    // 0x8001BCC8: nop

    return;
    // 0x8001BCC8: nop

;}
RECOMP_FUNC void func_8001BCD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BCD0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001BCD4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001BCD8: jal         0x8001CE78
    // 0x8001BCDC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001CE78(rdram, ctx);
        goto after_0;
    // 0x8001BCDC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8001BCE0: jal         0x8001C8B0
    // 0x8001BCE4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    func_8001C8B0(rdram, ctx);
        goto after_1;
    // 0x8001BCE4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    after_1:
    // 0x8001BCE8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001BCEC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001BCF0: jr          $ra
    // 0x8001BCF4: nop

    return;
    // 0x8001BCF4: nop

;}
RECOMP_FUNC void func_8001BCF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BCF8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001BCFC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001BD00: jal         0x8001CE78
    // 0x8001BD04: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001CE78(rdram, ctx);
        goto after_0;
    // 0x8001BD04: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8001BD08: jal         0x8001C8B0
    // 0x8001BD0C: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    func_8001C8B0(rdram, ctx);
        goto after_1;
    // 0x8001BD0C: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    after_1:
    // 0x8001BD10: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001BD14: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001BD18: jr          $ra
    // 0x8001BD1C: nop

    return;
    // 0x8001BD1C: nop

;}
RECOMP_FUNC void func_8001BD20(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BD20: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x8001BD24: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8001BD28: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x8001BD2C: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001BD30: sw          $s3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r19;
    // 0x8001BD34: sw          $s2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r18;
    // 0x8001BD38: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x8001BD3C: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x8001BD40: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001BD44: addiu       $a3, $sp, 0x3C
    ctx->r7 = ADD32(ctx->r29, 0X3C);
    // 0x8001BD48: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BD4C: jal         0x8001CF0C
    // 0x8001BD50: addiu       $a2, $zero, 0x1100
    ctx->r6 = ADD32(0, 0X1100);
    func_8001CF0C(rdram, ctx);
        goto after_0;
    // 0x8001BD50: addiu       $a2, $zero, 0x1100
    ctx->r6 = ADD32(0, 0X1100);
    after_0:
    // 0x8001BD54: beq         $v0, $zero, L_8001BD6C
    if (ctx->r2 == 0) {
        // 0x8001BD58: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001BD6C;
    }
    // 0x8001BD58: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001BD5C: jal         0x8001C8B0
    // 0x8001BD60: nop

    func_8001C8B0(rdram, ctx);
        goto after_1;
    // 0x8001BD60: nop

    after_1:
    // 0x8001BD64: b           L_8001BDE0
    // 0x8001BD68: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
        goto L_8001BDE0;
    // 0x8001BD68: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_8001BD6C:
    // 0x8001BD6C: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x8001BD70: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8001BD74: addiu       $v1, $v1, 0x1E40
    ctx->r3 = ADD32(ctx->r3, 0X1E40);
    // 0x8001BD78: addiu       $v0, $v0, 0x1A40
    ctx->r2 = ADD32(ctx->r2, 0X1A40);
L_8001BD7C:
    // 0x8001BD7C: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x8001BD80: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x8001BD84: bne         $at, $zero, L_8001BD7C
    if (ctx->r1 != 0) {
        // 0x8001BD88: sb          $zero, -0x1($v0)
        MEM_B(-0X1, ctx->r2) = 0;
            goto L_8001BD7C;
    }
    // 0x8001BD88: sb          $zero, -0x1($v0)
    MEM_B(-0X1, ctx->r2) = 0;
    // 0x8001BD8C: lui         $s2, 0x800F
    ctx->r18 = S32(0X800F << 16);
    // 0x8001BD90: addiu       $s2, $s2, 0x1A40
    ctx->r18 = ADD32(ctx->r18, 0X1A40);
    // 0x8001BD94: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x8001BD98: addiu       $s3, $zero, 0x1000
    ctx->r19 = ADD32(0, 0X1000);
    // 0x8001BD9C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
L_8001BDA0:
    // 0x8001BDA0: lw          $a1, 0x3C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X3C);
    // 0x8001BDA4: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x8001BDA8: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    // 0x8001BDAC: jal         0x8001D288
    // 0x8001BDB0: sw          $s2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r18;
    func_8001D288(rdram, ctx);
        goto after_2;
    // 0x8001BDB0: sw          $s2, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r18;
    after_2:
    // 0x8001BDB4: beq         $v0, $zero, L_8001BDC8
    if (ctx->r2 == 0) {
        // 0x8001BDB8: addiu       $s0, $s0, 0x400
        ctx->r16 = ADD32(ctx->r16, 0X400);
            goto L_8001BDC8;
    }
    // 0x8001BDB8: addiu       $s0, $s0, 0x400
    ctx->r16 = ADD32(ctx->r16, 0X400);
    // 0x8001BDBC: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x8001BDC0: sllv        $t7, $t6, $v0
    ctx->r15 = S32(ctx->r14 << (ctx->r2 & 31));
    // 0x8001BDC4: or          $s1, $s1, $t7
    ctx->r17 = ctx->r17 | ctx->r15;
L_8001BDC8:
    // 0x8001BDC8: bnel        $s0, $s3, L_8001BDA0
    if (ctx->r16 != ctx->r19) {
        // 0x8001BDCC: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_8001BDA0;
    }
    goto skip_0;
    // 0x8001BDCC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    skip_0:
    // 0x8001BDD0: jal         0x8001CB8C
    // 0x8001BDD4: nop

    func_8001CB8C(rdram, ctx);
        goto after_3;
    // 0x8001BDD4: nop

    after_3:
    // 0x8001BDD8: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    // 0x8001BDDC: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_8001BDE0:
    // 0x8001BDE0: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x8001BDE4: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x8001BDE8: lw          $s2, 0x24($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X24);
    // 0x8001BDEC: lw          $s3, 0x28($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X28);
    // 0x8001BDF0: jr          $ra
    // 0x8001BDF4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x8001BDF4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_8001BDF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BDF8: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8001BDFC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001BE00: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x8001BE04: jal         0x8001C364
    // 0x8001BE08: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001C364(rdram, ctx);
        goto after_0;
    // 0x8001BE08: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8001BE0C: jal         0x8001C324
    // 0x8001BE10: nop

    func_8001C324(rdram, ctx);
        goto after_1;
    // 0x8001BE10: nop

    after_1:
    // 0x8001BE14: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001BE18: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001BE1C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BE20: jal         0x8001D024
    // 0x8001BE24: addiu       $a2, $sp, 0x24
    ctx->r6 = ADD32(ctx->r29, 0X24);
    func_8001D024(rdram, ctx);
        goto after_2;
    // 0x8001BE24: addiu       $a2, $sp, 0x24
    ctx->r6 = ADD32(ctx->r29, 0X24);
    after_2:
    // 0x8001BE28: beq         $v0, $zero, L_8001BE38
    if (ctx->r2 == 0) {
        // 0x8001BE2C: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_8001BE38;
    }
    // 0x8001BE2C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BE30: b           L_8001BE5C
    // 0x8001BE34: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001BE5C;
    // 0x8001BE34: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001BE38:
    // 0x8001BE38: lbu         $a2, 0x33($sp)
    ctx->r6 = MEM_BU(ctx->r29, 0X33);
    // 0x8001BE3C: lui         $t6, 0x800F
    ctx->r14 = S32(0X800F << 16);
    // 0x8001BE40: addiu       $t6, $t6, 0x1640
    ctx->r14 = ADD32(ctx->r14, 0X1640);
    // 0x8001BE44: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x8001BE48: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x8001BE4C: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    // 0x8001BE50: jal         0x8001D288
    // 0x8001BE54: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    func_8001D288(rdram, ctx);
        goto after_3;
    // 0x8001BE54: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    after_3:
    // 0x8001BE58: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001BE5C:
    // 0x8001BE5C: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8001BE60: jr          $ra
    // 0x8001BE64: nop

    return;
    // 0x8001BE64: nop

;}
RECOMP_FUNC void func_8001BE68(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BE68: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8001BE6C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001BE70: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8001BE74: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001BE78: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001BE7C: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BE80: jal         0x8001D024
    // 0x8001BE84: addiu       $a2, $sp, 0x34
    ctx->r6 = ADD32(ctx->r29, 0X34);
    func_8001D024(rdram, ctx);
        goto after_0;
    // 0x8001BE84: addiu       $a2, $sp, 0x34
    ctx->r6 = ADD32(ctx->r29, 0X34);
    after_0:
    // 0x8001BE88: beq         $v0, $zero, L_8001BE98
    if (ctx->r2 == 0) {
        // 0x8001BE8C: nop
    
            goto L_8001BE98;
    }
    // 0x8001BE8C: nop

    // 0x8001BE90: b           L_8001BF58
    // 0x8001BE94: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001BF58;
    // 0x8001BE94: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001BE98:
    // 0x8001BE98: jal         0x8001C364
    // 0x8001BE9C: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    func_8001C364(rdram, ctx);
        goto after_1;
    // 0x8001BE9C: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    after_1:
    // 0x8001BEA0: lbu         $a2, 0x43($sp)
    ctx->r6 = MEM_BU(ctx->r29, 0X43);
    // 0x8001BEA4: lui         $t6, 0x800F
    ctx->r14 = S32(0X800F << 16);
    // 0x8001BEA8: addiu       $t6, $t6, 0x1A40
    ctx->r14 = ADD32(ctx->r14, 0X1A40);
    // 0x8001BEAC: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x8001BEB0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BEB4: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    // 0x8001BEB8: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    // 0x8001BEBC: jal         0x8001D204
    // 0x8001BEC0: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    func_8001D204(rdram, ctx);
        goto after_2;
    // 0x8001BEC0: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    after_2:
    // 0x8001BEC4: jal         0x8001C364
    // 0x8001BEC8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001C364(rdram, ctx);
        goto after_3;
    // 0x8001BEC8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_3:
    // 0x8001BECC: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8001BED0: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8001BED4: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x8001BED8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BEDC: addiu       $v0, $v0, 0x1BA4
    ctx->r2 = ADD32(ctx->r2, 0X1BA4);
    // 0x8001BEE0: addiu       $v1, $v1, 0x1A40
    ctx->r3 = ADD32(ctx->r3, 0X1A40);
    // 0x8001BEE4: addiu       $a1, $a1, 0x1640
    ctx->r5 = ADD32(ctx->r5, 0X1640);
L_8001BEE8:
    // 0x8001BEE8: lbu         $t7, 0x0($a1)
    ctx->r15 = MEM_BU(ctx->r5, 0X0);
    // 0x8001BEEC: lbu         $t8, 0x0($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0X0);
    // 0x8001BEF0: beql        $t7, $t8, L_8001BF00
    if (ctx->r15 == ctx->r24) {
        // 0x8001BEF4: lbu         $t9, 0x1($a1)
        ctx->r25 = MEM_BU(ctx->r5, 0X1);
            goto L_8001BF00;
    }
    goto skip_0;
    // 0x8001BEF4: lbu         $t9, 0x1($a1)
    ctx->r25 = MEM_BU(ctx->r5, 0X1);
    skip_0:
    // 0x8001BEF8: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x8001BEFC: lbu         $t9, 0x1($a1)
    ctx->r25 = MEM_BU(ctx->r5, 0X1);
L_8001BF00:
    // 0x8001BF00: lbu         $t0, 0x1($v1)
    ctx->r8 = MEM_BU(ctx->r3, 0X1);
    // 0x8001BF04: beql        $t9, $t0, L_8001BF14
    if (ctx->r25 == ctx->r8) {
        // 0x8001BF08: lbu         $t1, 0x2($a1)
        ctx->r9 = MEM_BU(ctx->r5, 0X2);
            goto L_8001BF14;
    }
    goto skip_1;
    // 0x8001BF08: lbu         $t1, 0x2($a1)
    ctx->r9 = MEM_BU(ctx->r5, 0X2);
    skip_1:
    // 0x8001BF0C: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x8001BF10: lbu         $t1, 0x2($a1)
    ctx->r9 = MEM_BU(ctx->r5, 0X2);
L_8001BF14:
    // 0x8001BF14: lbu         $t2, 0x2($v1)
    ctx->r10 = MEM_BU(ctx->r3, 0X2);
    // 0x8001BF18: beql        $t1, $t2, L_8001BF28
    if (ctx->r9 == ctx->r10) {
        // 0x8001BF1C: lbu         $t3, 0x3($a1)
        ctx->r11 = MEM_BU(ctx->r5, 0X3);
            goto L_8001BF28;
    }
    goto skip_2;
    // 0x8001BF1C: lbu         $t3, 0x3($a1)
    ctx->r11 = MEM_BU(ctx->r5, 0X3);
    skip_2:
    // 0x8001BF20: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x8001BF24: lbu         $t3, 0x3($a1)
    ctx->r11 = MEM_BU(ctx->r5, 0X3);
L_8001BF28:
    // 0x8001BF28: lbu         $t4, 0x3($v1)
    ctx->r12 = MEM_BU(ctx->r3, 0X3);
    // 0x8001BF2C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8001BF30: beq         $t3, $t4, L_8001BF3C
    if (ctx->r11 == ctx->r12) {
        // 0x8001BF34: nop
    
            goto L_8001BF3C;
    }
    // 0x8001BF34: nop

    // 0x8001BF38: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_8001BF3C:
    // 0x8001BF3C: bne         $v1, $v0, L_8001BEE8
    if (ctx->r3 != ctx->r2) {
        // 0x8001BF40: addiu       $a1, $a1, 0x4
        ctx->r5 = ADD32(ctx->r5, 0X4);
            goto L_8001BEE8;
    }
    // 0x8001BF40: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8001BF44: beq         $a0, $zero, L_8001BF54
    if (ctx->r4 == 0) {
        // 0x8001BF48: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8001BF54;
    }
    // 0x8001BF48: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001BF4C: b           L_8001BF54
    // 0x8001BF50: lui         $v0, 0x10
    ctx->r2 = S32(0X10 << 16);
        goto L_8001BF54;
    // 0x8001BF50: lui         $v0, 0x10
    ctx->r2 = S32(0X10 << 16);
L_8001BF54:
    // 0x8001BF54: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001BF58:
    // 0x8001BF58: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8001BF5C: jr          $ra
    // 0x8001BF60: nop

    return;
    // 0x8001BF60: nop

;}
RECOMP_FUNC void func_8001BF64(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BF64: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8001BF68: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001BF6C: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x8001BF70: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001BF74: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001BF78: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BF7C: jal         0x8001D024
    // 0x8001BF80: addiu       $a2, $sp, 0x24
    ctx->r6 = ADD32(ctx->r29, 0X24);
    func_8001D024(rdram, ctx);
        goto after_0;
    // 0x8001BF80: addiu       $a2, $sp, 0x24
    ctx->r6 = ADD32(ctx->r29, 0X24);
    after_0:
    // 0x8001BF84: beq         $v0, $zero, L_8001BF9C
    if (ctx->r2 == 0) {
        // 0x8001BF88: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001BF9C;
    }
    // 0x8001BF88: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001BF8C: jal         0x8001C8B0
    // 0x8001BF90: nop

    func_8001C8B0(rdram, ctx);
        goto after_1;
    // 0x8001BF90: nop

    after_1:
    // 0x8001BF94: b           L_8001BFCC
    // 0x8001BF98: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001BFCC;
    // 0x8001BF98: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001BF9C:
    // 0x8001BF9C: lbu         $a2, 0x33($sp)
    ctx->r6 = MEM_BU(ctx->r29, 0X33);
    // 0x8001BFA0: lui         $t6, 0x800F
    ctx->r14 = S32(0X800F << 16);
    // 0x8001BFA4: addiu       $t6, $t6, 0x1640
    ctx->r14 = ADD32(ctx->r14, 0X1640);
    // 0x8001BFA8: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x8001BFAC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001BFB0: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x8001BFB4: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    // 0x8001BFB8: jal         0x8001D204
    // 0x8001BFBC: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    func_8001D204(rdram, ctx);
        goto after_2;
    // 0x8001BFBC: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    after_2:
    // 0x8001BFC0: jal         0x8001C8B0
    // 0x8001BFC4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    func_8001C8B0(rdram, ctx);
        goto after_3;
    // 0x8001BFC4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    after_3:
    // 0x8001BFC8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001BFCC:
    // 0x8001BFCC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8001BFD0: jr          $ra
    // 0x8001BFD4: nop

    return;
    // 0x8001BFD4: nop

;}
RECOMP_FUNC void func_8001BFD8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001BFD8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001BFDC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001BFE0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001BFE4: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x8001BFE8: jal         0x8001BF64
    // 0x8001BFEC: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_8001BF64(rdram, ctx);
        goto after_0;
    // 0x8001BFEC: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x8001BFF0: beq         $v0, $zero, L_8001C000
    if (ctx->r2 == 0) {
        // 0x8001BFF4: nop
    
            goto L_8001C000;
    }
    // 0x8001BFF4: nop

    // 0x8001BFF8: b           L_8001C040
    // 0x8001BFFC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8001C040;
    // 0x8001BFFC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8001C000:
    // 0x8001C000: jal         0x8001C04C
    // 0x8001C004: nop

    func_8001C04C(rdram, ctx);
        goto after_1;
    // 0x8001C004: nop

    after_1:
    // 0x8001C008: jal         0x8001BDF8
    // 0x8001C00C: lbu         $a0, 0x1F($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X1F);
    func_8001BDF8(rdram, ctx);
        goto after_2;
    // 0x8001C00C: lbu         $a0, 0x1F($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X1F);
    after_2:
    // 0x8001C010: beq         $v0, $zero, L_8001C020
    if (ctx->r2 == 0) {
        // 0x8001C014: nop
    
            goto L_8001C020;
    }
    // 0x8001C014: nop

    // 0x8001C018: b           L_8001C040
    // 0x8001C01C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8001C040;
    // 0x8001C01C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8001C020:
    // 0x8001C020: jal         0x8001BE68
    // 0x8001C024: lbu         $a0, 0x1F($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X1F);
    func_8001BE68(rdram, ctx);
        goto after_3;
    // 0x8001C024: lbu         $a0, 0x1F($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X1F);
    after_3:
    // 0x8001C028: beql        $v0, $zero, L_8001C03C
    if (ctx->r2 == 0) {
        // 0x8001C02C: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8001C03C;
    }
    goto skip_0;
    // 0x8001C02C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x8001C030: b           L_8001C040
    // 0x8001C034: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8001C040;
    // 0x8001C034: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C038: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8001C03C:
    // 0x8001C03C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8001C040:
    // 0x8001C040: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001C044: jr          $ra
    // 0x8001C048: nop

    return;
    // 0x8001C048: nop

;}
RECOMP_FUNC void func_8001C04C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C04C: lui         $t6, 0x800B
    ctx->r14 = S32(0X800B << 16);
    // 0x8001C050: lw          $t6, 0x7CB4($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X7CB4);
    // 0x8001C054: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001C058: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C05C: beq         $t6, $zero, L_8001C074
    if (ctx->r14 == 0) {
        // 0x8001C060: lui         $t8, 0x800B
        ctx->r24 = S32(0X800B << 16);
            goto L_8001C074;
    }
    // 0x8001C060: lui         $t8, 0x800B
    ctx->r24 = S32(0X800B << 16);
    // 0x8001C064: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001C068: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001C06C: lh          $t7, 0x286C($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X286C);
    // 0x8001C070: sw          $t7, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r15;
L_8001C074:
    // 0x8001C074: lw          $t8, 0x7CB8($t8)
    ctx->r24 = MEM_W(ctx->r24, 0X7CB8);
    // 0x8001C078: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001C07C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001C080: beq         $t8, $zero, L_8001C090
    if (ctx->r24 == 0) {
        // 0x8001C084: nop
    
            goto L_8001C090;
    }
    // 0x8001C084: nop

    // 0x8001C088: lh          $t9, 0x286E($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X286E);
    // 0x8001C08C: sw          $t9, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r25;
L_8001C090:
    // 0x8001C090: jal         0x8001C364
    // 0x8001C094: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    func_8001C364(rdram, ctx);
        goto after_0;
    // 0x8001C094: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_0:
    // 0x8001C098: lui         $t0, 0x800B
    ctx->r8 = S32(0X800B << 16);
    // 0x8001C09C: lw          $t0, 0x7CB4($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X7CB4);
    // 0x8001C0A0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001C0A4: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001C0A8: beq         $t0, $zero, L_8001C0B8
    if (ctx->r8 == 0) {
        // 0x8001C0AC: lui         $t2, 0x800B
        ctx->r10 = S32(0X800B << 16);
            goto L_8001C0B8;
    }
    // 0x8001C0AC: lui         $t2, 0x800B
    ctx->r10 = S32(0X800B << 16);
    // 0x8001C0B0: lw          $t1, 0x1C($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X1C);
    // 0x8001C0B4: sh          $t1, 0x286C($v0)
    MEM_H(0X286C, ctx->r2) = ctx->r9;
L_8001C0B8:
    // 0x8001C0B8: lw          $t2, 0x7CB8($t2)
    ctx->r10 = MEM_W(ctx->r10, 0X7CB8);
    // 0x8001C0BC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001C0C0: lw          $t3, 0x18($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X18);
    // 0x8001C0C4: beq         $t2, $zero, L_8001C0D0
    if (ctx->r10 == 0) {
        // 0x8001C0C8: addiu       $t9, $t9, 0x66F8
        ctx->r25 = ADD32(ctx->r25, 0X66F8);
            goto L_8001C0D0;
    }
    // 0x8001C0C8: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x8001C0CC: sh          $t3, 0x286E($v0)
    MEM_H(0X286E, ctx->r2) = ctx->r11;
L_8001C0D0:
    // 0x8001C0D0: lh          $t4, 0x286E($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X286E);
    // 0x8001C0D4: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8001C0D8: bne         $t4, $at, L_8001C0F8
    if (ctx->r12 != ctx->r1) {
        // 0x8001C0DC: nop
    
            goto L_8001C0F8;
    }
    // 0x8001C0DC: nop

    // 0x8001C0E0: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001C0E4: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x8001C0E8: jalr        $t9
    // 0x8001C0EC: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001C0EC: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    after_1:
    // 0x8001C0F0: b           L_8001C104
    // 0x8001C0F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_8001C104;
    // 0x8001C0F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8001C0F8:
    // 0x8001C0F8: jalr        $t9
    // 0x8001C0FC: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001C0FC: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    after_2:
    // 0x8001C100: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8001C104:
    // 0x8001C104: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001C108: jr          $ra
    // 0x8001C10C: nop

    return;
    // 0x8001C10C: nop

;}
RECOMP_FUNC void func_8001C110(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C110: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8001C114: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x8001C118: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C11C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001C120: jal         0x8001C29C
    // 0x8001C124: addiu       $a0, $a0, 0x1640
    ctx->r4 = ADD32(ctx->r4, 0X1640);
    func_8001C29C(rdram, ctx);
        goto after_0;
    // 0x8001C124: addiu       $a0, $a0, 0x1640
    ctx->r4 = ADD32(ctx->r4, 0X1640);
    after_0:
    // 0x8001C128: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001C12C: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x8001C130: jal         0x8001C2E0
    // 0x8001C134: addiu       $a0, $a0, 0x1640
    ctx->r4 = ADD32(ctx->r4, 0X1640);
    func_8001C2E0(rdram, ctx);
        goto after_1;
    // 0x8001C134: addiu       $a0, $a0, 0x1640
    ctx->r4 = ADD32(ctx->r4, 0X1640);
    after_1:
    // 0x8001C138: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8001C13C: addiu       $v1, $v1, 0x1640
    ctx->r3 = ADD32(ctx->r3, 0X1640);
    // 0x8001C140: lw          $t7, 0x2C8($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X2C8);
    // 0x8001C144: lw          $t6, 0x1C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X1C);
    // 0x8001C148: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C14C: bnel        $t6, $t7, L_8001C16C
    if (ctx->r14 != ctx->r15) {
        // 0x8001C150: lui         $v0, 0x8
        ctx->r2 = S32(0X8 << 16);
            goto L_8001C16C;
    }
    goto skip_0;
    // 0x8001C150: lui         $v0, 0x8
    ctx->r2 = S32(0X8 << 16);
    skip_0:
    // 0x8001C154: lw          $t8, 0x2CC($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X2CC);
    // 0x8001C158: bnel        $v0, $t8, L_8001C16C
    if (ctx->r2 != ctx->r24) {
        // 0x8001C15C: lui         $v0, 0x8
        ctx->r2 = S32(0X8 << 16);
            goto L_8001C16C;
    }
    goto skip_1;
    // 0x8001C15C: lui         $v0, 0x8
    ctx->r2 = S32(0X8 << 16);
    skip_1:
    // 0x8001C160: b           L_8001C16C
    // 0x8001C164: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8001C16C;
    // 0x8001C164: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001C168: lui         $v0, 0x8
    ctx->r2 = S32(0X8 << 16);
L_8001C16C:
    // 0x8001C16C: jr          $ra
    // 0x8001C170: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x8001C170: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_8001C174(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C174: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8001C178: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x8001C17C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001C180: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001C184: addiu       $a0, $a0, 0x1640
    ctx->r4 = ADD32(ctx->r4, 0X1640);
    // 0x8001C188: jal         0x80000F30
    // 0x8001C18C: addiu       $a1, $zero, 0x400
    ctx->r5 = ADD32(0, 0X400);
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8001C18C: addiu       $a1, $zero, 0x400
    ctx->r5 = ADD32(0, 0X400);
    after_0:
    // 0x8001C190: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001C194: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001C198: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001C19C: jal         0x8001D024
    // 0x8001C1A0: addiu       $a2, $sp, 0x24
    ctx->r6 = ADD32(ctx->r29, 0X24);
    func_8001D024(rdram, ctx);
        goto after_1;
    // 0x8001C1A0: addiu       $a2, $sp, 0x24
    ctx->r6 = ADD32(ctx->r29, 0X24);
    after_1:
    // 0x8001C1A4: beq         $v0, $zero, L_8001C1BC
    if (ctx->r2 == 0) {
        // 0x8001C1A8: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001C1BC;
    }
    // 0x8001C1A8: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001C1AC: jal         0x8001C8B0
    // 0x8001C1B0: nop

    func_8001C8B0(rdram, ctx);
        goto after_2;
    // 0x8001C1B0: nop

    after_2:
    // 0x8001C1B4: b           L_8001C1EC
    // 0x8001C1B8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001C1EC;
    // 0x8001C1B8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C1BC:
    // 0x8001C1BC: lbu         $a2, 0x33($sp)
    ctx->r6 = MEM_BU(ctx->r29, 0X33);
    // 0x8001C1C0: lui         $t6, 0x800F
    ctx->r14 = S32(0X800F << 16);
    // 0x8001C1C4: addiu       $t6, $t6, 0x1640
    ctx->r14 = ADD32(ctx->r14, 0X1640);
    // 0x8001C1C8: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x8001C1CC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001C1D0: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x8001C1D4: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    // 0x8001C1D8: jal         0x8001D288
    // 0x8001C1DC: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    func_8001D288(rdram, ctx);
        goto after_3;
    // 0x8001C1DC: sll         $a2, $a2, 10
    ctx->r6 = S32(ctx->r6 << 10);
    after_3:
    // 0x8001C1E0: jal         0x8001C8B0
    // 0x8001C1E4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    func_8001C8B0(rdram, ctx);
        goto after_4;
    // 0x8001C1E4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    after_4:
    // 0x8001C1E8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C1EC:
    // 0x8001C1EC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x8001C1F0: jr          $ra
    // 0x8001C1F4: nop

    return;
    // 0x8001C1F4: nop

;}
RECOMP_FUNC void func_8001C1F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C1F8: addiu       $sp, $sp, -0x90
    ctx->r29 = ADD32(ctx->r29, -0X90);
    // 0x8001C1FC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C200: sw          $a0, 0x90($sp)
    MEM_W(0X90, ctx->r29) = ctx->r4;
    // 0x8001C204: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x8001C208: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001C20C: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001C210: andi        $a0, $a3, 0xFF
    ctx->r4 = ctx->r7 & 0XFF;
    // 0x8001C214: jal         0x8001D024
    // 0x8001C218: addiu       $a2, $sp, 0x88
    ctx->r6 = ADD32(ctx->r29, 0X88);
    func_8001D024(rdram, ctx);
        goto after_0;
    // 0x8001C218: addiu       $a2, $sp, 0x88
    ctx->r6 = ADD32(ctx->r29, 0X88);
    after_0:
    // 0x8001C21C: bne         $v0, $zero, L_8001C22C
    if (ctx->r2 != 0) {
        // 0x8001C220: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001C22C;
    }
    // 0x8001C220: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001C224: b           L_8001C234
    // 0x8001C228: lw          $v0, 0x88($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X88);
        goto L_8001C234;
    // 0x8001C228: lw          $v0, 0x88($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X88);
L_8001C22C:
    // 0x8001C22C: jal         0x8001C8B0
    // 0x8001C230: nop

    func_8001C8B0(rdram, ctx);
        goto after_1;
    // 0x8001C230: nop

    after_1:
L_8001C234:
    // 0x8001C234: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C238: addiu       $sp, $sp, 0x90
    ctx->r29 = ADD32(ctx->r29, 0X90);
    // 0x8001C23C: jr          $ra
    // 0x8001C240: nop

    return;
    // 0x8001C240: nop

;}
RECOMP_FUNC void func_8001C244(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C244: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001C248: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C24C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001C250: jal         0x8001CCC0
    // 0x8001C254: addiu       $a0, $a0, -0x470
    ctx->r4 = ADD32(ctx->r4, -0X470);
    func_8001CCC0(rdram, ctx);
        goto after_0;
    // 0x8001C254: addiu       $a0, $a0, -0x470
    ctx->r4 = ADD32(ctx->r4, -0X470);
    after_0:
    // 0x8001C258: lui         $t6, 0x800F
    ctx->r14 = S32(0X800F << 16);
    // 0x8001C25C: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8001C260: lbu         $t6, -0x469($t6)
    ctx->r14 = MEM_BU(ctx->r14, -0X469);
    // 0x8001C264: lbu         $v1, -0x46D($v1)
    ctx->r3 = MEM_BU(ctx->r3, -0X46D);
    // 0x8001C268: lui         $t8, 0x800F
    ctx->r24 = S32(0X800F << 16);
    // 0x8001C26C: lbu         $t8, -0x465($t8)
    ctx->r24 = MEM_BU(ctx->r24, -0X465);
    // 0x8001C270: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C274: lui         $t0, 0x800F
    ctx->r8 = S32(0X800F << 16);
    // 0x8001C278: sll         $t7, $t6, 16
    ctx->r15 = S32(ctx->r14 << 16);
    // 0x8001C27C: sll         $v1, $v1, 24
    ctx->r3 = S32(ctx->r3 << 24);
    // 0x8001C280: lbu         $t0, -0x461($t0)
    ctx->r8 = MEM_BU(ctx->r8, -0X461);
    // 0x8001C284: or          $v1, $v1, $t7
    ctx->r3 = ctx->r3 | ctx->r15;
    // 0x8001C288: sll         $t9, $t8, 8
    ctx->r25 = S32(ctx->r24 << 8);
    // 0x8001C28C: or          $v1, $v1, $t9
    ctx->r3 = ctx->r3 | ctx->r25;
    // 0x8001C290: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001C294: jr          $ra
    // 0x8001C298: or          $v0, $v1, $t0
    ctx->r2 = ctx->r3 | ctx->r8;
    return;
    // 0x8001C298: or          $v0, $v1, $t0
    ctx->r2 = ctx->r3 | ctx->r8;
;}
RECOMP_FUNC void func_8001C29C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C29C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8001C2A0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001C2A4: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8001C2A8: addiu       $a2, $zero, 0x164
    ctx->r6 = ADD32(0, 0X164);
L_8001C2AC:
    // 0x8001C2AC: lbu         $t6, 0x0($a1)
    ctx->r14 = MEM_BU(ctx->r5, 0X0);
    // 0x8001C2B0: lbu         $t7, 0x1($a1)
    ctx->r15 = MEM_BU(ctx->r5, 0X1);
    // 0x8001C2B4: lbu         $t8, 0x2($a1)
    ctx->r24 = MEM_BU(ctx->r5, 0X2);
    // 0x8001C2B8: addu        $v1, $t6, $v1
    ctx->r3 = ADD32(ctx->r14, ctx->r3);
    // 0x8001C2BC: lbu         $t9, 0x3($a1)
    ctx->r25 = MEM_BU(ctx->r5, 0X3);
    // 0x8001C2C0: addu        $v1, $t7, $v1
    ctx->r3 = ADD32(ctx->r15, ctx->r3);
    // 0x8001C2C4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8001C2C8: addu        $v1, $t8, $v1
    ctx->r3 = ADD32(ctx->r24, ctx->r3);
    // 0x8001C2CC: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8001C2D0: bne         $v0, $a2, L_8001C2AC
    if (ctx->r2 != ctx->r6) {
        // 0x8001C2D4: addu        $v1, $t9, $v1
        ctx->r3 = ADD32(ctx->r25, ctx->r3);
            goto L_8001C2AC;
    }
    // 0x8001C2D4: addu        $v1, $t9, $v1
    ctx->r3 = ADD32(ctx->r25, ctx->r3);
    // 0x8001C2D8: jr          $ra
    // 0x8001C2DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x8001C2DC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_8001C2E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C2E0: lw          $v1, 0x0($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X0);
    // 0x8001C2E4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8001C2E8: addiu       $a1, $a0, 0x4
    ctx->r5 = ADD32(ctx->r4, 0X4);
    // 0x8001C2EC: addiu       $a2, $zero, 0x59
    ctx->r6 = ADD32(0, 0X59);
L_8001C2F0:
    // 0x8001C2F0: lw          $t6, 0x0($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X0);
    // 0x8001C2F4: lw          $t7, 0x4($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X4);
    // 0x8001C2F8: lw          $t8, 0x8($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X8);
    // 0x8001C2FC: xor         $v1, $t6, $v1
    ctx->r3 = ctx->r14 ^ ctx->r3;
    // 0x8001C300: lw          $t9, 0xC($a1)
    ctx->r25 = MEM_W(ctx->r5, 0XC);
    // 0x8001C304: xor         $v1, $t7, $v1
    ctx->r3 = ctx->r15 ^ ctx->r3;
    // 0x8001C308: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8001C30C: xor         $v1, $t8, $v1
    ctx->r3 = ctx->r24 ^ ctx->r3;
    // 0x8001C310: addiu       $a1, $a1, 0x10
    ctx->r5 = ADD32(ctx->r5, 0X10);
    // 0x8001C314: bne         $v0, $a2, L_8001C2F0
    if (ctx->r2 != ctx->r6) {
        // 0x8001C318: xor         $v1, $t9, $v1
        ctx->r3 = ctx->r25 ^ ctx->r3;
            goto L_8001C2F0;
    }
    // 0x8001C318: xor         $v1, $t9, $v1
    ctx->r3 = ctx->r25 ^ ctx->r3;
    // 0x8001C31C: jr          $ra
    // 0x8001C320: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x8001C320: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_8001C324(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C324: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001C328: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C32C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001C330: jal         0x8001C29C
    // 0x8001C334: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    func_8001C29C(rdram, ctx);
        goto after_0;
    // 0x8001C334: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_0:
    // 0x8001C338: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8001C33C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001C340: sw          $v0, 0x1908($at)
    MEM_W(0X1908, ctx->r1) = ctx->r2;
    // 0x8001C344: jal         0x8001C2E0
    // 0x8001C348: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    func_8001C2E0(rdram, ctx);
        goto after_1;
    // 0x8001C348: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    after_1:
    // 0x8001C34C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C350: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8001C354: sw          $v0, 0x190C($at)
    MEM_W(0X190C, ctx->r1) = ctx->r2;
    // 0x8001C358: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001C35C: jr          $ra
    // 0x8001C360: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x8001C360: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8001C364(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C364: addiu       $sp, $sp, -0x10
    ctx->r29 = ADD32(ctx->r29, -0X10);
    // 0x8001C368: sw          $a0, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r4;
    // 0x8001C36C: andi        $a0, $a0, 0xFF
    ctx->r4 = ctx->r4 & 0XFF;
    // 0x8001C370: beq         $a0, $zero, L_8001C3A8
    if (ctx->r4 == 0) {
        // 0x8001C374: addiu       $a1, $zero, 0x164
        ctx->r5 = ADD32(0, 0X164);
            goto L_8001C3A8;
    }
    // 0x8001C374: addiu       $a1, $zero, 0x164
    ctx->r5 = ADD32(0, 0X164);
    // 0x8001C378: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x8001C37C: beq         $a0, $at, L_8001C3BC
    if (ctx->r4 == ctx->r1) {
        // 0x8001C380: lui         $v0, 0x800F
        ctx->r2 = S32(0X800F << 16);
            goto L_8001C3BC;
    }
    // 0x8001C380: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x8001C384: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8001C388: beq         $a0, $at, L_8001C3CC
    if (ctx->r4 == ctx->r1) {
        // 0x8001C38C: lui         $v0, 0x800F
        ctx->r2 = S32(0X800F << 16);
            goto L_8001C3CC;
    }
    // 0x8001C38C: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x8001C390: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x8001C394: beq         $a0, $at, L_8001C3DC
    if (ctx->r4 == ctx->r1) {
        // 0x8001C398: lui         $v0, 0x800F
        ctx->r2 = S32(0X800F << 16);
            goto L_8001C3DC;
    }
    // 0x8001C398: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x8001C39C: lw          $v1, 0x4($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X4);
    // 0x8001C3A0: b           L_8001C3E8
    // 0x8001C3A4: lw          $v0, 0x8($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X8);
        goto L_8001C3E8;
    // 0x8001C3A4: lw          $v0, 0x8($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X8);
L_8001C3A8:
    // 0x8001C3A8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001C3AC: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8001C3B0: addiu       $v0, $v0, -0x55A0
    ctx->r2 = ADD32(ctx->r2, -0X55A0);
    // 0x8001C3B4: b           L_8001C3E8
    // 0x8001C3B8: addiu       $v1, $v1, 0x1640
    ctx->r3 = ADD32(ctx->r3, 0X1640);
        goto L_8001C3E8;
    // 0x8001C3B8: addiu       $v1, $v1, 0x1640
    ctx->r3 = ADD32(ctx->r3, 0X1640);
L_8001C3BC:
    // 0x8001C3BC: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8001C3C0: addiu       $v0, $v0, 0x1640
    ctx->r2 = ADD32(ctx->r2, 0X1640);
    // 0x8001C3C4: b           L_8001C3E8
    // 0x8001C3C8: addiu       $v1, $v1, -0x55A0
    ctx->r3 = ADD32(ctx->r3, -0X55A0);
        goto L_8001C3E8;
    // 0x8001C3C8: addiu       $v1, $v1, -0x55A0
    ctx->r3 = ADD32(ctx->r3, -0X55A0);
L_8001C3CC:
    // 0x8001C3CC: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8001C3D0: addiu       $v0, $v0, 0x1640
    ctx->r2 = ADD32(ctx->r2, 0X1640);
    // 0x8001C3D4: b           L_8001C3E8
    // 0x8001C3D8: addiu       $v1, $v1, 0x1A40
    ctx->r3 = ADD32(ctx->r3, 0X1A40);
        goto L_8001C3E8;
    // 0x8001C3D8: addiu       $v1, $v1, 0x1A40
    ctx->r3 = ADD32(ctx->r3, 0X1A40);
L_8001C3DC:
    // 0x8001C3DC: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x8001C3E0: addiu       $v0, $v0, 0x1A40
    ctx->r2 = ADD32(ctx->r2, 0X1A40);
    // 0x8001C3E4: addiu       $v1, $v1, 0x1640
    ctx->r3 = ADD32(ctx->r3, 0X1640);
L_8001C3E8:
    // 0x8001C3E8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
L_8001C3EC:
    // 0x8001C3EC: lbu         $t6, 0x0($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X0);
    // 0x8001C3F0: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
    // 0x8001C3F4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x8001C3F8: sb          $t6, -0x4($v1)
    MEM_B(-0X4, ctx->r3) = ctx->r14;
    // 0x8001C3FC: lbu         $t7, 0x1($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X1);
    // 0x8001C400: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8001C404: sb          $t7, -0x3($v1)
    MEM_B(-0X3, ctx->r3) = ctx->r15;
    // 0x8001C408: lbu         $t8, -0x2($v0)
    ctx->r24 = MEM_BU(ctx->r2, -0X2);
    // 0x8001C40C: sb          $t8, -0x2($v1)
    MEM_B(-0X2, ctx->r3) = ctx->r24;
    // 0x8001C410: lbu         $t9, -0x1($v0)
    ctx->r25 = MEM_BU(ctx->r2, -0X1);
    // 0x8001C414: bne         $a0, $a1, L_8001C3EC
    if (ctx->r4 != ctx->r5) {
        // 0x8001C418: sb          $t9, -0x1($v1)
        MEM_B(-0X1, ctx->r3) = ctx->r25;
            goto L_8001C3EC;
    }
    // 0x8001C418: sb          $t9, -0x1($v1)
    MEM_B(-0X1, ctx->r3) = ctx->r25;
    // 0x8001C41C: jr          $ra
    // 0x8001C420: addiu       $sp, $sp, 0x10
    ctx->r29 = ADD32(ctx->r29, 0X10);
    return;
    // 0x8001C420: addiu       $sp, $sp, 0x10
    ctx->r29 = ADD32(ctx->r29, 0X10);
;}
RECOMP_FUNC void func_8001C424(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C424: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8001C428: lhu         $t6, 0x0($a0)
    ctx->r14 = MEM_HU(ctx->r4, 0X0);
    // 0x8001C42C: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x8001C430: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x8001C434: beq         $t6, $zero, L_8001C450
    if (ctx->r14 == 0) {
        // 0x8001C438: nop
    
            goto L_8001C450;
    }
    // 0x8001C438: nop

    // 0x8001C43C: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
L_8001C440:
    // 0x8001C440: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x8001C444: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8001C448: bnel        $t7, $zero, L_8001C440
    if (ctx->r15 != 0) {
        // 0x8001C44C: lhu         $t7, 0x2($v0)
        ctx->r15 = MEM_HU(ctx->r2, 0X2);
            goto L_8001C440;
    }
    goto skip_0;
    // 0x8001C44C: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
    skip_0:
L_8001C450:
    // 0x8001C450: jr          $ra
    // 0x8001C454: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x8001C454: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_8001C458(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C458: lhu         $v0, 0x0($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X0);
    // 0x8001C45C: ori         $a1, $zero, 0xB600
    ctx->r5 = 0 | 0XB600;
    // 0x8001C460: addiu       $v1, $zero, 0xA
    ctx->r3 = ADD32(0, 0XA);
    // 0x8001C464: beq         $v0, $zero, L_8001C4B4
    if (ctx->r2 == 0) {
        // 0x8001C468: slti        $at, $v0, 0x100
        ctx->r1 = SIGNED(ctx->r2) < 0X100 ? 1 : 0;
            goto L_8001C4B4;
    }
    // 0x8001C468: slti        $at, $v0, 0x100
    ctx->r1 = SIGNED(ctx->r2) < 0X100 ? 1 : 0;
L_8001C46C:
    // 0x8001C46C: bnel        $at, $zero, L_8001C480
    if (ctx->r1 != 0) {
        // 0x8001C470: slti        $at, $v0, 0x20
        ctx->r1 = SIGNED(ctx->r2) < 0X20 ? 1 : 0;
            goto L_8001C480;
    }
    goto skip_0;
    // 0x8001C470: slti        $at, $v0, 0x20
    ctx->r1 = SIGNED(ctx->r2) < 0X20 ? 1 : 0;
    skip_0:
    // 0x8001C474: b           L_8001C4A4
    // 0x8001C478: sh          $v0, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r2;
        goto L_8001C4A4;
    // 0x8001C478: sh          $v0, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r2;
    // 0x8001C47C: slti        $at, $v0, 0x20
    ctx->r1 = SIGNED(ctx->r2) < 0X20 ? 1 : 0;
L_8001C480:
    // 0x8001C480: bne         $at, $zero, L_8001C490
    if (ctx->r1 != 0) {
        // 0x8001C484: addiu       $t6, $v0, -0x20
        ctx->r14 = ADD32(ctx->r2, -0X20);
            goto L_8001C490;
    }
    // 0x8001C484: addiu       $t6, $v0, -0x20
    ctx->r14 = ADD32(ctx->r2, -0X20);
    // 0x8001C488: b           L_8001C4A4
    // 0x8001C48C: sh          $t6, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r14;
        goto L_8001C4A4;
    // 0x8001C48C: sh          $t6, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r14;
L_8001C490:
    // 0x8001C490: bnel        $v0, $v1, L_8001C4A4
    if (ctx->r2 != ctx->r3) {
        // 0x8001C494: sh          $v0, 0x0($a0)
        MEM_H(0X0, ctx->r4) = ctx->r2;
            goto L_8001C4A4;
    }
    goto skip_1;
    // 0x8001C494: sh          $v0, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r2;
    skip_1:
    // 0x8001C498: b           L_8001C4A4
    // 0x8001C49C: sh          $a1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r5;
        goto L_8001C4A4;
    // 0x8001C49C: sh          $a1, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r5;
    // 0x8001C4A0: sh          $v0, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r2;
L_8001C4A4:
    // 0x8001C4A4: lhu         $v0, 0x2($a0)
    ctx->r2 = MEM_HU(ctx->r4, 0X2);
    // 0x8001C4A8: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x8001C4AC: bnel        $v0, $zero, L_8001C46C
    if (ctx->r2 != 0) {
        // 0x8001C4B0: slti        $at, $v0, 0x100
        ctx->r1 = SIGNED(ctx->r2) < 0X100 ? 1 : 0;
            goto L_8001C46C;
    }
    goto skip_2;
    // 0x8001C4B0: slti        $at, $v0, 0x100
    ctx->r1 = SIGNED(ctx->r2) < 0X100 ? 1 : 0;
    skip_2:
L_8001C4B4:
    // 0x8001C4B4: jr          $ra
    // 0x8001C4B8: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x8001C4B8: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
;}
RECOMP_FUNC void func_8001C4BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C4BC: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8001C4C0: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8001C4C4: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x8001C4C8: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x8001C4CC: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x8001C4D0: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x8001C4D4: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8001C4D8: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8001C4DC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8001C4E0: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8001C4E4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001C4E8: jal         0x8001C1F8
    // 0x8001C4EC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001C1F8(rdram, ctx);
        goto after_0;
    // 0x8001C4EC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8001C4F0: srl         $t6, $v0, 9
    ctx->r14 = S32(U32(ctx->r2) >> 9);
    // 0x8001C4F4: beq         $t6, $zero, L_8001C504
    if (ctx->r14 == 0) {
        // 0x8001C4F8: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_8001C504;
    }
    // 0x8001C4F8: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x8001C4FC: b           L_8001C60C
    // 0x8001C500: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
        goto L_8001C60C;
    // 0x8001C500: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_8001C504:
    // 0x8001C504: lui         $s1, 0x800F
    ctx->r17 = S32(0X800F << 16);
    // 0x8001C508: addiu       $s1, $s1, 0x1640
    ctx->r17 = ADD32(ctx->r17, 0X1640);
    // 0x8001C50C: addiu       $fp, $zero, 0x4
    ctx->r30 = ADD32(0, 0X4);
    // 0x8001C510: addiu       $s7, $zero, 0x2
    ctx->r23 = ADD32(0, 0X2);
    // 0x8001C514: lui         $s6, 0x80
    ctx->r22 = S32(0X80 << 16);
    // 0x8001C518: lui         $s5, 0x40
    ctx->r21 = S32(0X40 << 16);
    // 0x8001C51C: lui         $s4, 0x10
    ctx->r20 = S32(0X10 << 16);
    // 0x8001C520: lui         $s3, 0x8
    ctx->r19 = S32(0X8 << 16);
    // 0x8001C524: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
L_8001C528:
    // 0x8001C528: jal         0x8001BF64
    // 0x8001C52C: andi        $a0, $s2, 0xFF
    ctx->r4 = ctx->r18 & 0XFF;
    func_8001BF64(rdram, ctx);
        goto after_1;
    // 0x8001C52C: andi        $a0, $s2, 0xFF
    ctx->r4 = ctx->r18 & 0XFF;
    after_1:
    // 0x8001C530: beq         $v0, $zero, L_8001C584
    if (ctx->r2 == 0) {
        // 0x8001C534: sll         $t7, $s2, 5
        ctx->r15 = S32(ctx->r18 << 5);
            goto L_8001C584;
    }
    // 0x8001C534: sll         $t7, $s2, 5
    ctx->r15 = S32(ctx->r18 << 5);
    // 0x8001C538: beq         $v0, $zero, L_8001C580
    if (ctx->r2 == 0) {
        // 0x8001C53C: nop
    
            goto L_8001C580;
    }
    // 0x8001C53C: nop

    // 0x8001C540: beq         $v0, $s3, L_8001C578
    if (ctx->r2 == ctx->r19) {
        // 0x8001C544: nop
    
            goto L_8001C578;
    }
    // 0x8001C544: nop

    // 0x8001C548: beq         $v0, $s4, L_8001C570
    if (ctx->r2 == ctx->r20) {
        // 0x8001C54C: nop
    
            goto L_8001C570;
    }
    // 0x8001C54C: nop

    // 0x8001C550: beq         $v0, $s5, L_8001C568
    if (ctx->r2 == ctx->r21) {
        // 0x8001C554: nop
    
            goto L_8001C568;
    }
    // 0x8001C554: nop

    // 0x8001C558: beq         $v0, $s6, L_8001C580
    if (ctx->r2 == ctx->r22) {
        // 0x8001C55C: addiu       $s0, $zero, 0x1
        ctx->r16 = ADD32(0, 0X1);
            goto L_8001C580;
    }
    // 0x8001C55C: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
    // 0x8001C560: b           L_8001C580
    // 0x8001C564: or          $s0, $s7, $zero
    ctx->r16 = ctx->r23 | 0;
        goto L_8001C580;
    // 0x8001C564: or          $s0, $s7, $zero
    ctx->r16 = ctx->r23 | 0;
L_8001C568:
    // 0x8001C568: b           L_8001C580
    // 0x8001C56C: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
        goto L_8001C580;
    // 0x8001C56C: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
L_8001C570:
    // 0x8001C570: b           L_8001C580
    // 0x8001C574: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
        goto L_8001C580;
    // 0x8001C574: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
L_8001C578:
    // 0x8001C578: b           L_8001C580
    // 0x8001C57C: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
        goto L_8001C580;
    // 0x8001C57C: addiu       $s0, $zero, 0x1
    ctx->r16 = ADD32(0, 0X1);
L_8001C580:
    // 0x8001C580: beq         $s0, $s7, L_8001C604
    if (ctx->r16 == ctx->r23) {
        // 0x8001C584: lui         $t8, 0x800F
        ctx->r24 = S32(0X800F << 16);
            goto L_8001C604;
    }
L_8001C584:
    // 0x8001C584: lui         $t8, 0x800F
    ctx->r24 = S32(0X800F << 16);
    // 0x8001C588: addiu       $t8, $t8, 0x1E40
    ctx->r24 = ADD32(ctx->r24, 0X1E40);
    // 0x8001C58C: bne         $s0, $zero, L_8001C5D8
    if (ctx->r16 != 0) {
        // 0x8001C590: addu        $v0, $t7, $t8
        ctx->r2 = ADD32(ctx->r15, ctx->r24);
            goto L_8001C5D8;
    }
    // 0x8001C590: addu        $v0, $t7, $t8
    ctx->r2 = ADD32(ctx->r15, ctx->r24);
    // 0x8001C594: lh          $t9, 0xD4($s1)
    ctx->r25 = MEM_H(ctx->r17, 0XD4);
    // 0x8001C598: lw          $t0, 0xE0($s1)
    ctx->r8 = MEM_W(ctx->r17, 0XE0);
    // 0x8001C59C: lh          $t1, 0x130($s1)
    ctx->r9 = MEM_H(ctx->r17, 0X130);
    // 0x8001C5A0: lh          $t2, 0x132($s1)
    ctx->r10 = MEM_H(ctx->r17, 0X132);
    // 0x8001C5A4: lw          $t3, 0x2CC($s1)
    ctx->r11 = MEM_W(ctx->r17, 0X2CC);
    // 0x8001C5A8: lw          $t4, 0x2C8($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X2C8);
    // 0x8001C5AC: lh          $t5, 0x134($s1)
    ctx->r13 = MEM_H(ctx->r17, 0X134);
    // 0x8001C5B0: lw          $t6, 0xB8($s1)
    ctx->r14 = MEM_W(ctx->r17, 0XB8);
    // 0x8001C5B4: sh          $t9, 0x4($v0)
    MEM_H(0X4, ctx->r2) = ctx->r25;
    // 0x8001C5B8: sw          $t0, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r8;
    // 0x8001C5BC: sh          $t1, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r9;
    // 0x8001C5C0: sh          $t2, 0xE($v0)
    MEM_H(0XE, ctx->r2) = ctx->r10;
    // 0x8001C5C4: sw          $t3, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->r11;
    // 0x8001C5C8: sw          $t4, 0x18($v0)
    MEM_W(0X18, ctx->r2) = ctx->r12;
    // 0x8001C5CC: sh          $t5, 0x10($v0)
    MEM_H(0X10, ctx->r2) = ctx->r13;
    // 0x8001C5D0: b           L_8001C5F8
    // 0x8001C5D4: sw          $t6, 0x14($v0)
    MEM_W(0X14, ctx->r2) = ctx->r14;
        goto L_8001C5F8;
    // 0x8001C5D4: sw          $t6, 0x14($v0)
    MEM_W(0X14, ctx->r2) = ctx->r14;
L_8001C5D8:
    // 0x8001C5D8: sh          $zero, 0x4($v0)
    MEM_H(0X4, ctx->r2) = 0;
    // 0x8001C5DC: sw          $zero, 0x8($v0)
    MEM_W(0X8, ctx->r2) = 0;
    // 0x8001C5E0: sh          $zero, 0xC($v0)
    MEM_H(0XC, ctx->r2) = 0;
    // 0x8001C5E4: sh          $zero, 0xE($v0)
    MEM_H(0XE, ctx->r2) = 0;
    // 0x8001C5E8: sw          $zero, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = 0;
    // 0x8001C5EC: sw          $zero, 0x18($v0)
    MEM_W(0X18, ctx->r2) = 0;
    // 0x8001C5F0: sh          $zero, 0x10($v0)
    MEM_H(0X10, ctx->r2) = 0;
    // 0x8001C5F4: sw          $zero, 0x14($v0)
    MEM_W(0X14, ctx->r2) = 0;
L_8001C5F8:
    // 0x8001C5F8: addiu       $s2, $s2, 0x1
    ctx->r18 = ADD32(ctx->r18, 0X1);
    // 0x8001C5FC: bnel        $s2, $fp, L_8001C528
    if (ctx->r18 != ctx->r30) {
        // 0x8001C600: or          $s0, $zero, $zero
        ctx->r16 = 0 | 0;
            goto L_8001C528;
    }
    goto skip_0;
    // 0x8001C600: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    skip_0:
L_8001C604:
    // 0x8001C604: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001C608: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_8001C60C:
    // 0x8001C60C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001C610: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8001C614: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8001C618: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8001C61C: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8001C620: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x8001C624: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x8001C628: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x8001C62C: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x8001C630: jr          $ra
    // 0x8001C634: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x8001C634: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_8001C638(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C638: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8001C63C: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8001C640: lbu         $v1, 0x43($sp)
    ctx->r3 = MEM_BU(ctx->r29, 0X43);
    // 0x8001C644: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001C648: addiu       $a0, $sp, 0x2F
    ctx->r4 = ADD32(ctx->r29, 0X2F);
    // 0x8001C64C: slti        $at, $v1, 0x4
    ctx->r1 = SIGNED(ctx->r3) < 0X4 ? 1 : 0;
    // 0x8001C650: beql        $at, $zero, L_8001C710
    if (ctx->r1 == 0) {
        // 0x8001C654: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8001C710;
    }
    goto skip_0;
    // 0x8001C654: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8001C658: jal         0x8001D1DC
    // 0x8001C65C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    func_8001D1DC(rdram, ctx);
        goto after_0;
    // 0x8001C65C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x8001C660: beq         $v0, $zero, L_8001C670
    if (ctx->r2 == 0) {
        // 0x8001C664: lw          $v1, 0x24($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X24);
            goto L_8001C670;
    }
    // 0x8001C664: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x8001C668: b           L_8001C70C
    // 0x8001C66C: lui         $v0, 0x4000
    ctx->r2 = S32(0X4000 << 16);
        goto L_8001C70C;
    // 0x8001C66C: lui         $v0, 0x4000
    ctx->r2 = S32(0X4000 << 16);
L_8001C670:
    // 0x8001C670: lbu         $t6, 0x2F($sp)
    ctx->r14 = MEM_BU(ctx->r29, 0X2F);
    // 0x8001C674: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001C678: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001C67C: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x8001C680: bne         $t7, $zero, L_8001C690
    if (ctx->r15 != 0) {
        // 0x8001C684: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_8001C690;
    }
    // 0x8001C684: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001C688: b           L_8001C70C
    // 0x8001C68C: lui         $v0, 0x2000
    ctx->r2 = S32(0X2000 << 16);
        goto L_8001C70C;
    // 0x8001C68C: lui         $v0, 0x2000
    ctx->r2 = S32(0X2000 << 16);
L_8001C690:
    // 0x8001C690: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    // 0x8001C694: jal         0x8001D024
    // 0x8001C698: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    func_8001D024(rdram, ctx);
        goto after_1;
    // 0x8001C698: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_1:
    // 0x8001C69C: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x8001C6A0: beq         $v0, $zero, L_8001C6B8
    if (ctx->r2 == 0) {
        // 0x8001C6A4: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001C6B8;
    }
    // 0x8001C6A4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001C6A8: jal         0x8001C8B0
    // 0x8001C6AC: nop

    func_8001C8B0(rdram, ctx);
        goto after_2;
    // 0x8001C6AC: nop

    after_2:
    // 0x8001C6B0: b           L_8001C710
    // 0x8001C6B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001C710;
    // 0x8001C6B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C6B8:
    // 0x8001C6B8: lui         $t8, 0x800F
    ctx->r24 = S32(0X800F << 16);
    // 0x8001C6BC: addiu       $t8, $t8, 0x1640
    ctx->r24 = ADD32(ctx->r24, 0X1640);
    // 0x8001C6C0: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    // 0x8001C6C4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001C6C8: lw          $a1, 0x28($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X28);
    // 0x8001C6CC: sll         $a2, $v1, 10
    ctx->r6 = S32(ctx->r3 << 10);
    // 0x8001C6D0: jal         0x8001D204
    // 0x8001C6D4: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    func_8001D204(rdram, ctx);
        goto after_3;
    // 0x8001C6D4: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    after_3:
    // 0x8001C6D8: beq         $v0, $zero, L_8001C6F0
    if (ctx->r2 == 0) {
        // 0x8001C6DC: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001C6F0;
    }
    // 0x8001C6DC: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001C6E0: jal         0x8001C8B0
    // 0x8001C6E4: nop

    func_8001C8B0(rdram, ctx);
        goto after_4;
    // 0x8001C6E4: nop

    after_4:
    // 0x8001C6E8: b           L_8001C710
    // 0x8001C6EC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001C710;
    // 0x8001C6EC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C6F0:
    // 0x8001C6F0: jal         0x8001C110
    // 0x8001C6F4: lbu         $a0, 0x43($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X43);
    func_8001C110(rdram, ctx);
        goto after_5;
    // 0x8001C6F4: lbu         $a0, 0x43($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X43);
    after_5:
    // 0x8001C6F8: beql        $v0, $zero, L_8001C70C
    if (ctx->r2 == 0) {
        // 0x8001C6FC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8001C70C;
    }
    goto skip_1;
    // 0x8001C6FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x8001C700: b           L_8001C70C
    // 0x8001C704: lui         $v0, 0x8
    ctx->r2 = S32(0X8 << 16);
        goto L_8001C70C;
    // 0x8001C704: lui         $v0, 0x8
    ctx->r2 = S32(0X8 << 16);
    // 0x8001C708: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8001C70C:
    // 0x8001C70C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C710:
    // 0x8001C710: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8001C714: jr          $ra
    // 0x8001C718: nop

    return;
    // 0x8001C718: nop

;}
RECOMP_FUNC void func_8001C71C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C71C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001C720: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C724: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001C728: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8001C72C: addiu       $a1, $a1, 0x17A4
    ctx->r5 = ADD32(ctx->r5, 0X17A4);
    // 0x8001C730: addiu       $a0, $a0, -0x55A0
    ctx->r4 = ADD32(ctx->r4, -0X55A0);
    // 0x8001C734: jal         0x80001090
    // 0x8001C738: addiu       $a2, $zero, 0x164
    ctx->r6 = ADD32(0, 0X164);
    func_80001090(rdram, ctx);
        goto after_0;
    // 0x8001C738: addiu       $a2, $zero, 0x164
    ctx->r6 = ADD32(0, 0X164);
    after_0:
    // 0x8001C73C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C740: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001C744: jr          $ra
    // 0x8001C748: nop

    return;
    // 0x8001C748: nop

;}
RECOMP_FUNC void func_8001C74C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C74C: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8001C750: lw          $t6, -0x54E8($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X54E8);
    // 0x8001C754: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001C758: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C75C: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x8001C760: beq         $t7, $zero, L_8001C76C
    if (ctx->r15 == 0) {
        // 0x8001C764: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8001C76C;
    }
    // 0x8001C764: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001C768: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8001C76C:
    // 0x8001C76C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001C770: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8001C774: addiu       $a1, $a1, 0x1640
    ctx->r5 = ADD32(ctx->r5, 0X1640);
    // 0x8001C778: addiu       $a0, $a0, 0x17A4
    ctx->r4 = ADD32(ctx->r4, 0X17A4);
    // 0x8001C77C: addiu       $a2, $zero, 0x164
    ctx->r6 = ADD32(0, 0X164);
    // 0x8001C780: jal         0x80001090
    // 0x8001C784: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_80001090(rdram, ctx);
        goto after_0;
    // 0x8001C784: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x8001C788: jal         0x8001C04C
    // 0x8001C78C: nop

    func_8001C04C(rdram, ctx);
        goto after_1;
    // 0x8001C78C: nop

    after_1:
    // 0x8001C790: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x8001C794: beq         $v0, $zero, L_8001C7AC
    if (ctx->r2 == 0) {
        // 0x8001C798: lui         $v0, 0x801D
        ctx->r2 = S32(0X801D << 16);
            goto L_8001C7AC;
    }
    // 0x8001C798: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001C79C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001C7A0: lw          $t8, 0x2858($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X2858);
    // 0x8001C7A4: ori         $t9, $t8, 0x1
    ctx->r25 = ctx->r24 | 0X1;
    // 0x8001C7A8: sw          $t9, 0x2858($v0)
    MEM_W(0X2858, ctx->r2) = ctx->r25;
L_8001C7AC:
    // 0x8001C7AC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C7B0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001C7B4: jr          $ra
    // 0x8001C7B8: nop

    return;
    // 0x8001C7B8: nop

;}
RECOMP_FUNC void func_8001C7BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C7BC: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x8001C7C0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001C7C4: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x8001C7C8: jal         0x8001C364
    // 0x8001C7CC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001C364(rdram, ctx);
        goto after_0;
    // 0x8001C7CC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8001C7D0: jal         0x8001C324
    // 0x8001C7D4: nop

    func_8001C324(rdram, ctx);
        goto after_1;
    // 0x8001C7D4: nop

    after_1:
    // 0x8001C7D8: lbu         $v1, 0x43($sp)
    ctx->r3 = MEM_BU(ctx->r29, 0X43);
    // 0x8001C7DC: addiu       $a0, $sp, 0x2F
    ctx->r4 = ADD32(ctx->r29, 0X2F);
    // 0x8001C7E0: slti        $at, $v1, 0x4
    ctx->r1 = SIGNED(ctx->r3) < 0X4 ? 1 : 0;
    // 0x8001C7E4: beql        $at, $zero, L_8001C8A4
    if (ctx->r1 == 0) {
        // 0x8001C7E8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_8001C8A4;
    }
    goto skip_0;
    // 0x8001C7E8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x8001C7EC: jal         0x8001D1DC
    // 0x8001C7F0: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    func_8001D1DC(rdram, ctx);
        goto after_2;
    // 0x8001C7F0: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_2:
    // 0x8001C7F4: beq         $v0, $zero, L_8001C804
    if (ctx->r2 == 0) {
        // 0x8001C7F8: lw          $v1, 0x24($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X24);
            goto L_8001C804;
    }
    // 0x8001C7F8: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x8001C7FC: b           L_8001C8A0
    // 0x8001C800: lui         $v0, 0x4000
    ctx->r2 = S32(0X4000 << 16);
        goto L_8001C8A0;
    // 0x8001C800: lui         $v0, 0x4000
    ctx->r2 = S32(0X4000 << 16);
L_8001C804:
    // 0x8001C804: lbu         $t6, 0x2F($sp)
    ctx->r14 = MEM_BU(ctx->r29, 0X2F);
    // 0x8001C808: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001C80C: addiu       $a1, $a1, 0x3864
    ctx->r5 = ADD32(ctx->r5, 0X3864);
    // 0x8001C810: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x8001C814: bne         $t7, $zero, L_8001C824
    if (ctx->r15 != 0) {
        // 0x8001C818: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_8001C824;
    }
    // 0x8001C818: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001C81C: b           L_8001C8A0
    // 0x8001C820: lui         $v0, 0x2000
    ctx->r2 = S32(0X2000 << 16);
        goto L_8001C8A0;
    // 0x8001C820: lui         $v0, 0x2000
    ctx->r2 = S32(0X2000 << 16);
L_8001C824:
    // 0x8001C824: addiu       $a2, $sp, 0x28
    ctx->r6 = ADD32(ctx->r29, 0X28);
    // 0x8001C828: jal         0x8001D024
    // 0x8001C82C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    func_8001D024(rdram, ctx);
        goto after_3;
    // 0x8001C82C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_3:
    // 0x8001C830: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x8001C834: beq         $v0, $zero, L_8001C84C
    if (ctx->r2 == 0) {
        // 0x8001C838: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001C84C;
    }
    // 0x8001C838: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001C83C: jal         0x8001C8B0
    // 0x8001C840: nop

    func_8001C8B0(rdram, ctx);
        goto after_4;
    // 0x8001C840: nop

    after_4:
    // 0x8001C844: b           L_8001C8A4
    // 0x8001C848: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001C8A4;
    // 0x8001C848: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C84C:
    // 0x8001C84C: lui         $t8, 0x800F
    ctx->r24 = S32(0X800F << 16);
    // 0x8001C850: addiu       $t8, $t8, 0x1640
    ctx->r24 = ADD32(ctx->r24, 0X1640);
    // 0x8001C854: sw          $t8, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r24;
    // 0x8001C858: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x8001C85C: lw          $a1, 0x28($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X28);
    // 0x8001C860: sll         $a2, $v1, 10
    ctx->r6 = S32(ctx->r3 << 10);
    // 0x8001C864: jal         0x8001D288
    // 0x8001C868: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    func_8001D288(rdram, ctx);
        goto after_5;
    // 0x8001C868: addiu       $a3, $zero, 0x400
    ctx->r7 = ADD32(0, 0X400);
    after_5:
    // 0x8001C86C: beq         $v0, $zero, L_8001C884
    if (ctx->r2 == 0) {
        // 0x8001C870: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001C884;
    }
    // 0x8001C870: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001C874: jal         0x8001C8B0
    // 0x8001C878: nop

    func_8001C8B0(rdram, ctx);
        goto after_6;
    // 0x8001C878: nop

    after_6:
    // 0x8001C87C: b           L_8001C8A4
    // 0x8001C880: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8001C8A4;
    // 0x8001C880: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C884:
    // 0x8001C884: jal         0x8001BE68
    // 0x8001C888: lbu         $a0, 0x43($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X43);
    func_8001BE68(rdram, ctx);
        goto after_7;
    // 0x8001C888: lbu         $a0, 0x43($sp)
    ctx->r4 = MEM_BU(ctx->r29, 0X43);
    after_7:
    // 0x8001C88C: beql        $v0, $zero, L_8001C8A0
    if (ctx->r2 == 0) {
        // 0x8001C890: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_8001C8A0;
    }
    goto skip_1;
    // 0x8001C890: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x8001C894: b           L_8001C8A0
    // 0x8001C898: lui         $v0, 0x10
    ctx->r2 = S32(0X10 << 16);
        goto L_8001C8A0;
    // 0x8001C898: lui         $v0, 0x10
    ctx->r2 = S32(0X10 << 16);
    // 0x8001C89C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8001C8A0:
    // 0x8001C8A0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8001C8A4:
    // 0x8001C8A4: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x8001C8A8: jr          $ra
    // 0x8001C8AC: nop

    return;
    // 0x8001C8AC: nop

;}
RECOMP_FUNC void func_8001C8B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001C8B0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001C8B4: addiu       $at, $zero, 0xA
    ctx->r1 = ADD32(0, 0XA);
    // 0x8001C8B8: bne         $a0, $at, L_8001C8D4
    if (ctx->r4 != ctx->r1) {
        // 0x8001C8BC: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_8001C8D4;
    }
    // 0x8001C8BC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001C8C0: jal         0x8001CE78
    // 0x8001C8C4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001CE78(rdram, ctx);
        goto after_0;
    // 0x8001C8C4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x8001C8C8: jal         0x8001D30C
    // 0x8001C8CC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    func_8001D30C(rdram, ctx);
        goto after_1;
    // 0x8001C8CC: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_1:
    // 0x8001C8D0: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
L_8001C8D4:
    // 0x8001C8D4: addiu       $t6, $a0, -0x1
    ctx->r14 = ADD32(ctx->r4, -0X1);
    // 0x8001C8D8: sltiu       $at, $t6, 0xB
    ctx->r1 = ctx->r14 < 0XB ? 1 : 0;
    // 0x8001C8DC: beq         $at, $zero, L_8001C92C
    if (ctx->r1 == 0) {
        // 0x8001C8E0: sll         $t6, $t6, 2
        ctx->r14 = S32(ctx->r14 << 2);
            goto L_8001C92C;
    }
    // 0x8001C8E0: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x8001C8E4: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x8001C8E8: addu        $at, $at, $t6
    gpr jr_addend_8001C8F0 = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x8001C8EC: lw          $t6, -0x2C6C($at)
    ctx->r14 = ADD32(ctx->r1, -0X2C6C);
    // 0x8001C8F0: jr          $t6
    // 0x8001C8F4: nop

    switch (jr_addend_8001C8F0 >> 2) {
        case 0: goto L_8001C8F8; break;
        case 1: goto L_8001C900; break;
        case 2: goto L_8001C928; break;
        case 3: goto L_8001C918; break;
        case 4: goto L_8001C910; break;
        case 5: goto L_8001C92C; break;
        case 6: goto L_8001C92C; break;
        case 7: goto L_8001C92C; break;
        case 8: goto L_8001C92C; break;
        case 9: goto L_8001C920; break;
        case 10: goto L_8001C908; break;
        default: switch_error(__func__, 0x8001C8F0, 0x800BD394);
    }
    // 0x8001C8F4: nop

L_8001C8F8:
    // 0x8001C8F8: b           L_8001C92C
    // 0x8001C8FC: lui         $v0, 0x2000
    ctx->r2 = S32(0X2000 << 16);
        goto L_8001C92C;
    // 0x8001C8FC: lui         $v0, 0x2000
    ctx->r2 = S32(0X2000 << 16);
L_8001C900:
    // 0x8001C900: b           L_8001C92C
    // 0x8001C904: lui         $v0, 0x1000
    ctx->r2 = S32(0X1000 << 16);
        goto L_8001C92C;
    // 0x8001C904: lui         $v0, 0x1000
    ctx->r2 = S32(0X1000 << 16);
L_8001C908:
    // 0x8001C908: b           L_8001C92C
    // 0x8001C90C: lui         $v0, 0x400
    ctx->r2 = S32(0X400 << 16);
        goto L_8001C92C;
    // 0x8001C90C: lui         $v0, 0x400
    ctx->r2 = S32(0X400 << 16);
L_8001C910:
    // 0x8001C910: b           L_8001C92C
    // 0x8001C914: lui         $v0, 0x4
    ctx->r2 = S32(0X4 << 16);
        goto L_8001C92C;
    // 0x8001C914: lui         $v0, 0x4
    ctx->r2 = S32(0X4 << 16);
L_8001C918:
    // 0x8001C918: b           L_8001C92C
    // 0x8001C91C: lui         $v0, 0x800
    ctx->r2 = S32(0X800 << 16);
        goto L_8001C92C;
    // 0x8001C91C: lui         $v0, 0x800
    ctx->r2 = S32(0X800 << 16);
L_8001C920:
    // 0x8001C920: b           L_8001C92C
    // 0x8001C924: lui         $v0, 0x200
    ctx->r2 = S32(0X200 << 16);
        goto L_8001C92C;
    // 0x8001C924: lui         $v0, 0x200
    ctx->r2 = S32(0X200 << 16);
L_8001C928:
    // 0x8001C928: lui         $v0, 0x100
    ctx->r2 = S32(0X100 << 16);
L_8001C92C:
    // 0x8001C92C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001C930: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001C934: jr          $ra
    // 0x8001C938: nop

    return;
    // 0x8001C938: nop

;}
