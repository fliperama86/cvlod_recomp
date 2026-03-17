#include "recomp.h"
#include "funcs.h"
RECOMP_FUNC void func_80001FFC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001FFC: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80002000: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80002004: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80002008: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8000200C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80002010: or          $s2, $a3, $zero
    ctx->r18 = ctx->r7 | 0;
    // 0x80002014: or          $s3, $a2, $zero
    ctx->r19 = ctx->r6 | 0;
    // 0x80002018: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x8000201C: bne         $a1, $zero, L_8000202C
    if (ctx->r5 != 0) {
        // 0x80002020: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_8000202C;
    }
    // 0x80002020: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80002024: b           L_80002034
    // 0x80002028: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
        goto L_80002034;
    // 0x80002028: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
L_8000202C:
    // 0x8000202C: b           L_80002050
    // 0x80002030: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
        goto L_80002050;
    // 0x80002030: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
L_80002034:
    // 0x80002034: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80002038: jalr        $s3
    // 0x8000203C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_0;
    // 0x8000203C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_0:
    // 0x80002040: beql        $v0, $zero, L_80002054
    if (ctx->r2 == 0) {
        // 0x80002044: lw          $v0, 0x1C($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X1C);
            goto L_80002054;
    }
    goto skip_0;
    // 0x80002044: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    skip_0:
    // 0x80002048: b           L_800020CC
    // 0x8000204C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
        goto L_800020CC;
    // 0x8000204C: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80002050:
    // 0x80002050: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
L_80002054:
    // 0x80002054: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80002058: beq         $v0, $zero, L_80002078
    if (ctx->r2 == 0) {
        // 0x8000205C: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80002078;
    }
    // 0x8000205C: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80002060: jalr        $s3
    // 0x80002064: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    LOOKUP_FUNC(ctx->r19)(rdram, ctx);
        goto after_1;
    // 0x80002064: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_1:
    // 0x80002068: beql        $v0, $zero, L_80002054
    if (ctx->r2 == 0) {
        // 0x8000206C: lw          $v0, 0x1C($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X1C);
            goto L_80002054;
    }
    goto skip_1;
    // 0x8000206C: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    skip_1:
    // 0x80002070: b           L_800020CC
    // 0x80002074: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
        goto L_800020CC;
    // 0x80002074: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_80002078:
    // 0x80002078: bnel        $s0, $s1, L_8000208C
    if (ctx->r16 != ctx->r17) {
        // 0x8000207C: lw          $v0, 0x18($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X18);
            goto L_8000208C;
    }
    goto skip_2;
    // 0x8000207C: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    skip_2:
    // 0x80002080: b           L_800020CC
    // 0x80002084: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_800020CC;
    // 0x80002084: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80002088: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
L_8000208C:
    // 0x8000208C: bne         $v0, $zero, L_800020C4
    if (ctx->r2 != 0) {
        // 0x80002090: nop
    
            goto L_800020C4;
    }
    // 0x80002090: nop

    // 0x80002094: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
L_80002098:
    // 0x80002098: bne         $v0, $zero, L_800020A8
    if (ctx->r2 != 0) {
        // 0x8000209C: nop
    
            goto L_800020A8;
    }
    // 0x8000209C: nop

    // 0x800020A0: b           L_800020CC
    // 0x800020A4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_800020CC;
    // 0x800020A4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800020A8:
    // 0x800020A8: bne         $v0, $s1, L_800020B8
    if (ctx->r2 != ctx->r17) {
        // 0x800020AC: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_800020B8;
    }
    // 0x800020AC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x800020B0: b           L_800020CC
    // 0x800020B4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_800020CC;
    // 0x800020B4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800020B8:
    // 0x800020B8: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    // 0x800020BC: beql        $v0, $zero, L_80002098
    if (ctx->r2 == 0) {
        // 0x800020C0: lw          $v0, 0x14($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X14);
            goto L_80002098;
    }
    goto skip_3;
    // 0x800020C0: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    skip_3:
L_800020C4:
    // 0x800020C4: b           L_80002034
    // 0x800020C8: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
        goto L_80002034;
    // 0x800020C8: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_800020CC:
    // 0x800020CC: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x800020D0: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x800020D4: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x800020D8: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x800020DC: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x800020E0: jr          $ra
    // 0x800020E4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x800020E4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_800020E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800020E8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x800020EC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x800020F0: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x800020F4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800020F8: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x800020FC: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80002100: or          $s0, $zero, $zero
    ctx->r16 = 0 | 0;
    // 0x80002104: addiu       $s1, $zero, 0x1
    ctx->r17 = ADD32(0, 0X1);
L_80002108:
    // 0x80002108: lhu         $t6, 0x20($s2)
    ctx->r14 = MEM_HU(ctx->r18, 0X20);
    // 0x8000210C: sll         $t8, $s0, 2
    ctx->r24 = S32(ctx->r16 << 2);
    // 0x80002110: addu        $t9, $s2, $t8
    ctx->r25 = ADD32(ctx->r18, ctx->r24);
    // 0x80002114: and         $t7, $t6, $s1
    ctx->r15 = ctx->r14 & ctx->r17;
    // 0x80002118: beql        $t7, $zero, L_8000212C
    if (ctx->r15 == 0) {
        // 0x8000211C: lhu         $t0, 0x22($s2)
        ctx->r8 = MEM_HU(ctx->r18, 0X22);
            goto L_8000212C;
    }
    goto skip_0;
    // 0x8000211C: lhu         $t0, 0x22($s2)
    ctx->r8 = MEM_HU(ctx->r18, 0X22);
    skip_0:
    // 0x80002120: jal         0x80001918
    // 0x80002124: lw          $a0, 0x34($t9)
    ctx->r4 = MEM_W(ctx->r25, 0X34);
    func_80001918(rdram, ctx);
        goto after_0;
    // 0x80002124: lw          $a0, 0x34($t9)
    ctx->r4 = MEM_W(ctx->r25, 0X34);
    after_0:
    // 0x80002128: lhu         $t0, 0x22($s2)
    ctx->r8 = MEM_HU(ctx->r18, 0X22);
L_8000212C:
    // 0x8000212C: sll         $t2, $s0, 2
    ctx->r10 = S32(ctx->r16 << 2);
    // 0x80002130: addu        $t3, $s2, $t2
    ctx->r11 = ADD32(ctx->r18, ctx->r10);
    // 0x80002134: and         $t1, $t0, $s1
    ctx->r9 = ctx->r8 & ctx->r17;
    // 0x80002138: beql        $t1, $zero, L_8000214C
    if (ctx->r9 == 0) {
        // 0x8000213C: addiu       $s0, $s0, 0x1
        ctx->r16 = ADD32(ctx->r16, 0X1);
            goto L_8000214C;
    }
    goto skip_1;
    // 0x8000213C: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
    skip_1:
    // 0x80002140: jal         0x80001920
    // 0x80002144: lw          $a0, 0x34($t3)
    ctx->r4 = MEM_W(ctx->r11, 0X34);
    func_80001920(rdram, ctx);
        goto after_1;
    // 0x80002144: lw          $a0, 0x34($t3)
    ctx->r4 = MEM_W(ctx->r11, 0X34);
    after_1:
    // 0x80002148: addiu       $s0, $s0, 0x1
    ctx->r16 = ADD32(ctx->r16, 0X1);
L_8000214C:
    // 0x8000214C: slti        $at, $s0, 0x10
    ctx->r1 = SIGNED(ctx->r16) < 0X10 ? 1 : 0;
    // 0x80002150: bne         $at, $zero, L_80002108
    if (ctx->r1 != 0) {
        // 0x80002154: sll         $s1, $s1, 1
        ctx->r17 = S32(ctx->r17 << 1);
            goto L_80002108;
    }
    // 0x80002154: sll         $s1, $s1, 1
    ctx->r17 = S32(ctx->r17 << 1);
    // 0x80002158: addiu       $v0, $s2, 0x24
    ctx->r2 = ADD32(ctx->r18, 0X24);
    // 0x8000215C: addiu       $s0, $zero, 0x3
    ctx->r16 = ADD32(0, 0X3);
L_80002160:
    // 0x80002160: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x80002164: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80002168: beq         $v1, $zero, L_80002174
    if (ctx->r3 == 0) {
        // 0x8000216C: nop
    
            goto L_80002174;
    }
    // 0x8000216C: nop

    // 0x80002170: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
L_80002174:
    // 0x80002174: bgez        $s0, L_80002160
    if (SIGNED(ctx->r16) >= 0) {
        // 0x80002178: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_80002160;
    }
    // 0x80002178: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x8000217C: lh          $t4, 0x0($s2)
    ctx->r12 = MEM_H(ctx->r18, 0X0);
    // 0x80002180: lui         $t7, 0x801B
    ctx->r15 = S32(0X801B << 16);
    // 0x80002184: addiu       $t7, $t7, 0x3D60
    ctx->r15 = ADD32(ctx->r15, 0X3D60);
    // 0x80002188: andi        $t5, $t4, 0x7FF
    ctx->r13 = ctx->r12 & 0X7FF;
    // 0x8000218C: sll         $t6, $t5, 1
    ctx->r14 = S32(ctx->r13 << 1);
    // 0x80002190: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
    // 0x80002194: lhu         $t8, -0x2($v0)
    ctx->r24 = MEM_HU(ctx->r2, -0X2);
    // 0x80002198: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x8000219C: sh          $t9, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r25;
    // 0x800021A0: sh          $zero, 0x0($s2)
    MEM_H(0X0, ctx->r18) = 0;
    // 0x800021A4: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x800021A8: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x800021AC: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x800021B0: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800021B4: jr          $ra
    // 0x800021B8: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x800021B8: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_800021BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800021BC: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x800021C0: sh          $zero, 0x3D60($at)
    MEM_H(0X3D60, ctx->r1) = 0;
    // 0x800021C4: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x800021C8: sh          $zero, 0x3D62($at)
    MEM_H(0X3D62, ctx->r1) = 0;
    // 0x800021CC: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x800021D0: lui         $v1, 0x801B
    ctx->r3 = S32(0X801B << 16);
    // 0x800021D4: lui         $v0, 0x801B
    ctx->r2 = S32(0X801B << 16);
    // 0x800021D8: addiu       $v0, $v0, 0x42BE
    ctx->r2 = ADD32(ctx->r2, 0X42BE);
    // 0x800021DC: addiu       $v1, $v1, 0x3D66
    ctx->r3 = ADD32(ctx->r3, 0X3D66);
    // 0x800021E0: sh          $zero, 0x3D64($at)
    MEM_H(0X3D64, ctx->r1) = 0;
L_800021E4:
    // 0x800021E4: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    // 0x800021E8: sh          $zero, -0x8($v1)
    MEM_H(-0X8, ctx->r3) = 0;
    // 0x800021EC: sh          $zero, -0x6($v1)
    MEM_H(-0X6, ctx->r3) = 0;
    // 0x800021F0: sh          $zero, -0x4($v1)
    MEM_H(-0X4, ctx->r3) = 0;
    // 0x800021F4: bne         $v1, $v0, L_800021E4
    if (ctx->r3 != ctx->r2) {
        // 0x800021F8: sh          $zero, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = 0;
            goto L_800021E4;
    }
    // 0x800021F8: sh          $zero, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = 0;
    // 0x800021FC: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80002200: addiu       $a0, $a0, 0x1534
    ctx->r4 = ADD32(ctx->r4, 0X1534);
    // 0x80002204: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80002208: lw          $v1, 0x1530($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1530);
    // 0x8000220C: lw          $t6, 0x0($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X0);
    // 0x80002210: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80002214: sltu        $at, $v1, $t6
    ctx->r1 = ctx->r3 < ctx->r14 ? 1 : 0;
    // 0x80002218: beq         $at, $zero, L_80002240
    if (ctx->r1 == 0) {
        // 0x8000221C: nop
    
            goto L_80002240;
    }
    // 0x8000221C: nop

    // 0x80002220: sw          $zero, 0x0($v0)
    MEM_W(0X0, ctx->r2) = 0;
L_80002224:
    // 0x80002224: lw          $t7, 0x0($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X0);
    // 0x80002228: addiu       $v0, $v0, 0x74
    ctx->r2 = ADD32(ctx->r2, 0X74);
    // 0x8000222C: sltu        $at, $v0, $t7
    ctx->r1 = ctx->r2 < ctx->r15 ? 1 : 0;
    // 0x80002230: bnel        $at, $zero, L_80002224
    if (ctx->r1 != 0) {
        // 0x80002234: sw          $zero, 0x0($v0)
        MEM_W(0X0, ctx->r2) = 0;
            goto L_80002224;
    }
    goto skip_0;
    // 0x80002234: sw          $zero, 0x0($v0)
    MEM_W(0X0, ctx->r2) = 0;
    skip_0:
    // 0x80002238: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x8000223C: lw          $v1, 0x1530($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X1530);
L_80002240:
    // 0x80002240: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80002244: jr          $ra
    // 0x80002248: sw          $v1, 0x153C($at)
    MEM_W(0X153C, ctx->r1) = ctx->r3;
    return;
    // 0x80002248: sw          $v1, 0x153C($at)
    MEM_W(0X153C, ctx->r1) = ctx->r3;
;}
RECOMP_FUNC void func_8000224C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000224C: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x80002250: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80002254: lw          $v0, 0x1534($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1534);
    // 0x80002258: lw          $a2, 0x1530($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X1530);
    // 0x8000225C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002260: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002264: sltu        $at, $a2, $v0
    ctx->r1 = ctx->r6 < ctx->r2 ? 1 : 0;
    // 0x80002268: beq         $at, $zero, L_800022CC
    if (ctx->r1 == 0) {
        // 0x8000226C: sw          $a0, 0x20($sp)
        MEM_W(0X20, ctx->r29) = ctx->r4;
            goto L_800022CC;
    }
    // 0x8000226C: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80002270: lh          $t6, 0x0($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X0);
L_80002274:
    // 0x80002274: bne         $t6, $zero, L_800022BC
    if (ctx->r14 != 0) {
        // 0x80002278: or          $a0, $a2, $zero
        ctx->r4 = ctx->r6 | 0;
            goto L_800022BC;
    }
    // 0x80002278: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x8000227C: addiu       $a1, $zero, 0x74
    ctx->r5 = ADD32(0, 0X74);
    // 0x80002280: jal         0x80000F30
    // 0x80002284: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x80002284: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    after_0:
    // 0x80002288: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x8000228C: lw          $t7, 0x20($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X20);
    // 0x80002290: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80002294: addiu       $v0, $v0, 0x153C
    ctx->r2 = ADD32(ctx->r2, 0X153C);
    // 0x80002298: sh          $t7, 0x0($a2)
    MEM_H(0X0, ctx->r6) = ctx->r15;
    // 0x8000229C: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x800022A0: addiu       $t9, $a2, 0x74
    ctx->r25 = ADD32(ctx->r6, 0X74);
    // 0x800022A4: sltu        $at, $a2, $t8
    ctx->r1 = ctx->r6 < ctx->r24 ? 1 : 0;
    // 0x800022A8: bne         $at, $zero, L_800022B4
    if (ctx->r1 != 0) {
        // 0x800022AC: nop
    
            goto L_800022B4;
    }
    // 0x800022AC: nop

    // 0x800022B0: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
L_800022B4:
    // 0x800022B4: b           L_800022D0
    // 0x800022B8: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
        goto L_800022D0;
    // 0x800022B8: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
L_800022BC:
    // 0x800022BC: addiu       $a2, $a2, 0x74
    ctx->r6 = ADD32(ctx->r6, 0X74);
    // 0x800022C0: sltu        $at, $a2, $v0
    ctx->r1 = ctx->r6 < ctx->r2 ? 1 : 0;
    // 0x800022C4: bnel        $at, $zero, L_80002274
    if (ctx->r1 != 0) {
        // 0x800022C8: lh          $t6, 0x0($a2)
        ctx->r14 = MEM_H(ctx->r6, 0X0);
            goto L_80002274;
    }
    goto skip_0;
    // 0x800022C8: lh          $t6, 0x0($a2)
    ctx->r14 = MEM_H(ctx->r6, 0X0);
    skip_0:
L_800022CC:
    // 0x800022CC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800022D0:
    // 0x800022D0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800022D4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800022D8: jr          $ra
    // 0x800022DC: nop

    return;
    // 0x800022DC: nop

;}
RECOMP_FUNC void func_800022E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800022E0: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x800022E4: lw          $v0, 0x1534($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1534);
    // 0x800022E8: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x800022EC: lw          $a1, 0x1530($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X1530);
    // 0x800022F0: addiu       $a0, $v0, -0x74
    ctx->r4 = ADD32(ctx->r2, -0X74);
    // 0x800022F4: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x800022F8: sltu        $at, $a0, $a1
    ctx->r1 = ctx->r4 < ctx->r5 ? 1 : 0;
    // 0x800022FC: bne         $at, $zero, L_80002330
    if (ctx->r1 != 0) {
        // 0x80002300: nop
    
            goto L_80002330;
    }
    // 0x80002300: nop

    // 0x80002304: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x80002308: bne         $t6, $zero, L_80002330
    if (ctx->r14 != 0) {
        // 0x8000230C: nop
    
            goto L_80002330;
    }
    // 0x8000230C: nop

    // 0x80002310: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80002314:
    // 0x80002314: addiu       $v1, $v1, -0x74
    ctx->r3 = ADD32(ctx->r3, -0X74);
    // 0x80002318: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x8000231C: bne         $at, $zero, L_80002330
    if (ctx->r1 != 0) {
        // 0x80002320: nop
    
            goto L_80002330;
    }
    // 0x80002320: nop

    // 0x80002324: lh          $t7, 0x0($v1)
    ctx->r15 = MEM_H(ctx->r3, 0X0);
    // 0x80002328: beql        $t7, $zero, L_80002314
    if (ctx->r15 == 0) {
        // 0x8000232C: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80002314;
    }
    goto skip_0;
    // 0x8000232C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_0:
L_80002330:
    // 0x80002330: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80002334: jr          $ra
    // 0x80002338: sw          $v0, 0x153C($at)
    MEM_W(0X153C, ctx->r1) = ctx->r2;
    return;
    // 0x80002338: sw          $v0, 0x153C($at)
    MEM_W(0X153C, ctx->r1) = ctx->r2;
;}
RECOMP_FUNC void func_8000233C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000233C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002340: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002344: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80002348: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    // 0x8000234C: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80002350: jal         0x8000224C
    // 0x80002354: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_8000224C(rdram, ctx);
        goto after_0;
    // 0x80002354: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_0:
    // 0x80002358: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x8000235C: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x80002360: beq         $v0, $zero, L_800023FC
    if (ctx->r2 == 0) {
        // 0x80002364: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_800023FC;
    }
    // 0x80002364: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80002368: andi        $a2, $a2, 0x7FF
    ctx->r6 = ctx->r6 & 0X7FF;
    // 0x8000236C: sll         $t6, $a2, 2
    ctx->r14 = S32(ctx->r6 << 2);
    // 0x80002370: lui         $t7, 0x800B
    ctx->r15 = S32(0X800B << 16);
    // 0x80002374: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80002378: lw          $t7, -0x1574($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X1574);
    // 0x8000237C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80002380: beql        $t7, $zero, L_800023BC
    if (ctx->r15 == 0) {
        // 0x80002384: addiu       $t8, $zero, -0x1
        ctx->r24 = ADD32(0, -0X1);
            goto L_800023BC;
    }
    goto skip_0;
    // 0x80002384: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    skip_0:
    // 0x80002388: lw          $a0, 0x15E0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X15E0);
    // 0x8000238C: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80002390: sw          $a2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r6;
    // 0x80002394: jal         0x80010EFC
    // 0x80002398: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    func_80010EFC(rdram, ctx);
        goto after_1;
    // 0x80002398: sw          $a3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r7;
    after_1:
    // 0x8000239C: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x800023A0: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x800023A4: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x800023A8: bne         $v0, $at, L_800023B8
    if (ctx->r2 != ctx->r1) {
        // 0x800023AC: lw          $a3, 0x20($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X20);
            goto L_800023B8;
    }
    // 0x800023AC: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    // 0x800023B0: b           L_80002400
    // 0x800023B4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80002400;
    // 0x800023B4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800023B8:
    // 0x800023B8: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
L_800023BC:
    // 0x800023BC: beq         $a3, $zero, L_800023D4
    if (ctx->r7 == 0) {
        // 0x800023C0: sh          $t8, 0xE($a1)
        MEM_H(0XE, ctx->r5) = ctx->r24;
            goto L_800023D4;
    }
    // 0x800023C0: sh          $t8, 0xE($a1)
    MEM_H(0XE, ctx->r5) = ctx->r24;
    // 0x800023C4: sw          $a3, 0x14($a1)
    MEM_W(0X14, ctx->r5) = ctx->r7;
    // 0x800023C8: lw          $t9, 0x1C($a3)
    ctx->r25 = MEM_W(ctx->r7, 0X1C);
    // 0x800023CC: sw          $t9, 0x18($a1)
    MEM_W(0X18, ctx->r5) = ctx->r25;
    // 0x800023D0: sw          $a1, 0x1C($a3)
    MEM_W(0X1C, ctx->r7) = ctx->r5;
L_800023D4:
    // 0x800023D4: lui         $t0, 0x8000
    ctx->r8 = S32(0X8000 << 16);
    // 0x800023D8: lui         $t2, 0x801B
    ctx->r10 = S32(0X801B << 16);
    // 0x800023DC: addiu       $t0, $t0, 0x2CE0
    ctx->r8 = ADD32(ctx->r8, 0X2CE0);
    // 0x800023E0: addiu       $t2, $t2, 0x3D60
    ctx->r10 = ADD32(ctx->r10, 0X3D60);
    // 0x800023E4: sll         $t1, $a2, 1
    ctx->r9 = S32(ctx->r6 << 1);
    // 0x800023E8: sw          $t0, 0x10($a1)
    MEM_W(0X10, ctx->r5) = ctx->r8;
    // 0x800023EC: addu        $v0, $t1, $t2
    ctx->r2 = ADD32(ctx->r9, ctx->r10);
    // 0x800023F0: lhu         $t3, -0x2($v0)
    ctx->r11 = MEM_HU(ctx->r2, -0X2);
    // 0x800023F4: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x800023F8: sh          $t4, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r12;
L_800023FC:
    // 0x800023FC: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
L_80002400:
    // 0x80002400: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80002404: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80002408: jr          $ra
    // 0x8000240C: nop

    return;
    // 0x8000240C: nop

;}
RECOMP_FUNC void func_80002410(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002410: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002414: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002418: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8000241C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x80002420: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80002424: jal         0x8000224C
    // 0x80002428: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_8000224C(rdram, ctx);
        goto after_0;
    // 0x80002428: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_0:
    // 0x8000242C: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x80002430: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x80002434: beq         $v0, $zero, L_800024FC
    if (ctx->r2 == 0) {
        // 0x80002438: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_800024FC;
    }
    // 0x80002438: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x8000243C: andi        $a3, $a3, 0x7FF
    ctx->r7 = ctx->r7 & 0X7FF;
    // 0x80002440: sll         $t6, $a3, 2
    ctx->r14 = S32(ctx->r7 << 2);
    // 0x80002444: lui         $t7, 0x800B
    ctx->r15 = S32(0X800B << 16);
    // 0x80002448: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x8000244C: lw          $t7, -0x1574($t7)
    ctx->r15 = MEM_W(ctx->r15, -0X1574);
    // 0x80002450: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80002454: beql        $t7, $zero, L_80002490
    if (ctx->r15 == 0) {
        // 0x80002458: addiu       $t8, $zero, -0x1
        ctx->r24 = ADD32(0, -0X1);
            goto L_80002490;
    }
    goto skip_0;
    // 0x80002458: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
    skip_0:
    // 0x8000245C: lw          $a0, 0x15E0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X15E0);
    // 0x80002460: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80002464: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x80002468: jal         0x80010EFC
    // 0x8000246C: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    func_80010EFC(rdram, ctx);
        goto after_1;
    // 0x8000246C: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    after_1:
    // 0x80002470: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80002474: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x80002478: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
    // 0x8000247C: bne         $v0, $at, L_8000248C
    if (ctx->r2 != ctx->r1) {
        // 0x80002480: lw          $a3, 0x24($sp)
        ctx->r7 = MEM_W(ctx->r29, 0X24);
            goto L_8000248C;
    }
    // 0x80002480: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x80002484: b           L_80002500
    // 0x80002488: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80002500;
    // 0x80002488: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8000248C:
    // 0x8000248C: addiu       $t8, $zero, -0x1
    ctx->r24 = ADD32(0, -0X1);
L_80002490:
    // 0x80002490: beq         $a2, $zero, L_800024D4
    if (ctx->r6 == 0) {
        // 0x80002494: sh          $t8, 0xE($a1)
        MEM_H(0XE, ctx->r5) = ctx->r24;
            goto L_800024D4;
    }
    // 0x80002494: sh          $t8, 0xE($a1)
    MEM_H(0XE, ctx->r5) = ctx->r24;
    // 0x80002498: sw          $a2, 0x14($a1)
    MEM_W(0X14, ctx->r5) = ctx->r6;
    // 0x8000249C: lw          $a0, 0x1C($a2)
    ctx->r4 = MEM_W(ctx->r6, 0X1C);
    // 0x800024A0: beql        $a0, $zero, L_800024D4
    if (ctx->r4 == 0) {
        // 0x800024A4: sw          $a1, 0x1C($a2)
        MEM_W(0X1C, ctx->r6) = ctx->r5;
            goto L_800024D4;
    }
    goto skip_1;
    // 0x800024A4: sw          $a1, 0x1C($a2)
    MEM_W(0X1C, ctx->r6) = ctx->r5;
    skip_1:
    // 0x800024A8: lw          $v0, 0x18($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X18);
    // 0x800024AC: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x800024B0: beq         $v0, $zero, L_800024C8
    if (ctx->r2 == 0) {
        // 0x800024B4: nop
    
            goto L_800024C8;
    }
    // 0x800024B4: nop

    // 0x800024B8: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_800024BC:
    // 0x800024BC: lw          $v0, 0x18($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X18);
    // 0x800024C0: bnel        $v0, $zero, L_800024BC
    if (ctx->r2 != 0) {
        // 0x800024C4: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_800024BC;
    }
    goto skip_2;
    // 0x800024C4: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    skip_2:
L_800024C8:
    // 0x800024C8: b           L_800024D4
    // 0x800024CC: sw          $a1, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r5;
        goto L_800024D4;
    // 0x800024CC: sw          $a1, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r5;
    // 0x800024D0: sw          $a1, 0x1C($a2)
    MEM_W(0X1C, ctx->r6) = ctx->r5;
L_800024D4:
    // 0x800024D4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x800024D8: lui         $t1, 0x801B
    ctx->r9 = S32(0X801B << 16);
    // 0x800024DC: addiu       $t9, $t9, 0x2CE0
    ctx->r25 = ADD32(ctx->r25, 0X2CE0);
    // 0x800024E0: addiu       $t1, $t1, 0x3D60
    ctx->r9 = ADD32(ctx->r9, 0X3D60);
    // 0x800024E4: sll         $t0, $a3, 1
    ctx->r8 = S32(ctx->r7 << 1);
    // 0x800024E8: sw          $t9, 0x10($a1)
    MEM_W(0X10, ctx->r5) = ctx->r25;
    // 0x800024EC: addu        $v0, $t0, $t1
    ctx->r2 = ADD32(ctx->r8, ctx->r9);
    // 0x800024F0: lhu         $t2, -0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, -0X2);
    // 0x800024F4: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x800024F8: sh          $t3, -0x2($v0)
    MEM_H(-0X2, ctx->r2) = ctx->r11;
L_800024FC:
    // 0x800024FC: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
L_80002500:
    // 0x80002500: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80002504: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80002508: jr          $ra
    // 0x8000250C: nop

    return;
    // 0x8000250C: nop

;}
RECOMP_FUNC void func_80002510(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002510: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80002514: lw          $v0, 0x153C($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X153C);
    // 0x80002518: addiu       $a1, $a1, 0x74
    ctx->r5 = ADD32(ctx->r5, 0X74);
    // 0x8000251C: andi        $a0, $a0, 0x7FF
    ctx->r4 = ctx->r4 & 0X7FF;
    // 0x80002520: sltu        $at, $a1, $v0
    ctx->r1 = ctx->r5 < ctx->r2 ? 1 : 0;
    // 0x80002524: beql        $at, $zero, L_80002558
    if (ctx->r1 == 0) {
        // 0x80002528: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80002558;
    }
    goto skip_0;
    // 0x80002528: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x8000252C: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
L_80002530:
    // 0x80002530: andi        $t7, $t6, 0x7FF
    ctx->r15 = ctx->r14 & 0X7FF;
    // 0x80002534: bnel        $a0, $t7, L_80002548
    if (ctx->r4 != ctx->r15) {
        // 0x80002538: addiu       $a1, $a1, 0x74
        ctx->r5 = ADD32(ctx->r5, 0X74);
            goto L_80002548;
    }
    goto skip_1;
    // 0x80002538: addiu       $a1, $a1, 0x74
    ctx->r5 = ADD32(ctx->r5, 0X74);
    skip_1:
    // 0x8000253C: jr          $ra
    // 0x80002540: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    return;
    // 0x80002540: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80002544: addiu       $a1, $a1, 0x74
    ctx->r5 = ADD32(ctx->r5, 0X74);
L_80002548:
    // 0x80002548: sltu        $at, $a1, $v0
    ctx->r1 = ctx->r5 < ctx->r2 ? 1 : 0;
    // 0x8000254C: bnel        $at, $zero, L_80002530
    if (ctx->r1 != 0) {
        // 0x80002550: lh          $t6, 0x0($a1)
        ctx->r14 = MEM_H(ctx->r5, 0X0);
            goto L_80002530;
    }
    goto skip_2;
    // 0x80002550: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    skip_2:
    // 0x80002554: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80002558:
    // 0x80002558: jr          $ra
    // 0x8000255C: nop

    return;
    // 0x8000255C: nop

;}
RECOMP_FUNC void func_80002560(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002560: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80002564: lw          $a1, 0x1530($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X1530);
    // 0x80002568: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8000256C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002570: jal         0x80002510
    // 0x80002574: addiu       $a1, $a1, -0x74
    ctx->r5 = ADD32(ctx->r5, -0X74);
    func_80002510(rdram, ctx);
        goto after_0;
    // 0x80002574: addiu       $a1, $a1, -0x74
    ctx->r5 = ADD32(ctx->r5, -0X74);
    after_0:
    // 0x80002578: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8000257C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80002580: jr          $ra
    // 0x80002584: nop

    return;
    // 0x80002584: nop

;}
RECOMP_FUNC void func_80002588(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002588: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x8000258C: lw          $v0, 0x153C($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X153C);
    // 0x80002590: addiu       $a2, $a2, 0x74
    ctx->r6 = ADD32(ctx->r6, 0X74);
    // 0x80002594: andi        $a0, $a0, 0x7FF
    ctx->r4 = ctx->r4 & 0X7FF;
    // 0x80002598: sltu        $at, $a2, $v0
    ctx->r1 = ctx->r6 < ctx->r2 ? 1 : 0;
    // 0x8000259C: beq         $at, $zero, L_800025D8
    if (ctx->r1 == 0) {
        // 0x800025A0: andi        $a1, $a1, 0x7FF
        ctx->r5 = ctx->r5 & 0X7FF;
            goto L_800025D8;
    }
    // 0x800025A0: andi        $a1, $a1, 0x7FF
    ctx->r5 = ctx->r5 & 0X7FF;
    // 0x800025A4: lh          $v1, 0x0($a2)
    ctx->r3 = MEM_H(ctx->r6, 0X0);
L_800025A8:
    // 0x800025A8: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x800025AC: slt         $at, $v1, $a0
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r4) ? 1 : 0;
    // 0x800025B0: bne         $at, $zero, L_800025C8
    if (ctx->r1 != 0) {
        // 0x800025B4: slt         $at, $a1, $v1
        ctx->r1 = SIGNED(ctx->r5) < SIGNED(ctx->r3) ? 1 : 0;
            goto L_800025C8;
    }
    // 0x800025B4: slt         $at, $a1, $v1
    ctx->r1 = SIGNED(ctx->r5) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x800025B8: bnel        $at, $zero, L_800025CC
    if (ctx->r1 != 0) {
        // 0x800025BC: addiu       $a2, $a2, 0x74
        ctx->r6 = ADD32(ctx->r6, 0X74);
            goto L_800025CC;
    }
    goto skip_0;
    // 0x800025BC: addiu       $a2, $a2, 0x74
    ctx->r6 = ADD32(ctx->r6, 0X74);
    skip_0:
    // 0x800025C0: jr          $ra
    // 0x800025C4: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    return;
    // 0x800025C4: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
L_800025C8:
    // 0x800025C8: addiu       $a2, $a2, 0x74
    ctx->r6 = ADD32(ctx->r6, 0X74);
L_800025CC:
    // 0x800025CC: sltu        $at, $a2, $v0
    ctx->r1 = ctx->r6 < ctx->r2 ? 1 : 0;
    // 0x800025D0: bnel        $at, $zero, L_800025A8
    if (ctx->r1 != 0) {
        // 0x800025D4: lh          $v1, 0x0($a2)
        ctx->r3 = MEM_H(ctx->r6, 0X0);
            goto L_800025A8;
    }
    goto skip_1;
    // 0x800025D4: lh          $v1, 0x0($a2)
    ctx->r3 = MEM_H(ctx->r6, 0X0);
    skip_1:
L_800025D8:
    // 0x800025D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x800025DC: jr          $ra
    // 0x800025E0: nop

    return;
    // 0x800025E0: nop

;}
RECOMP_FUNC void func_800025E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800025E4: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x800025E8: lw          $a2, 0x1530($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X1530);
    // 0x800025EC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800025F0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800025F4: jal         0x80002588
    // 0x800025F8: addiu       $a2, $a2, -0x74
    ctx->r6 = ADD32(ctx->r6, -0X74);
    func_80002588(rdram, ctx);
        goto after_0;
    // 0x800025F8: addiu       $a2, $a2, -0x74
    ctx->r6 = ADD32(ctx->r6, -0X74);
    after_0:
    // 0x800025FC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80002600: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80002604: jr          $ra
    // 0x80002608: nop

    return;
    // 0x80002608: nop

;}
RECOMP_FUNC void func_8000260C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000260C: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80002610: lw          $v0, 0x153C($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X153C);
    // 0x80002614: addiu       $a1, $a1, 0x74
    ctx->r5 = ADD32(ctx->r5, 0X74);
    // 0x80002618: sltu        $at, $a1, $v0
    ctx->r1 = ctx->r5 < ctx->r2 ? 1 : 0;
    // 0x8000261C: beql        $at, $zero, L_80002650
    if (ctx->r1 == 0) {
        // 0x80002620: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80002650;
    }
    goto skip_0;
    // 0x80002620: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80002624: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
L_80002628:
    // 0x80002628: and         $t7, $t6, $a0
    ctx->r15 = ctx->r14 & ctx->r4;
    // 0x8000262C: beql        $t7, $zero, L_80002640
    if (ctx->r15 == 0) {
        // 0x80002630: addiu       $a1, $a1, 0x74
        ctx->r5 = ADD32(ctx->r5, 0X74);
            goto L_80002640;
    }
    goto skip_1;
    // 0x80002630: addiu       $a1, $a1, 0x74
    ctx->r5 = ADD32(ctx->r5, 0X74);
    skip_1:
    // 0x80002634: jr          $ra
    // 0x80002638: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    return;
    // 0x80002638: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x8000263C: addiu       $a1, $a1, 0x74
    ctx->r5 = ADD32(ctx->r5, 0X74);
L_80002640:
    // 0x80002640: sltu        $at, $a1, $v0
    ctx->r1 = ctx->r5 < ctx->r2 ? 1 : 0;
    // 0x80002644: bnel        $at, $zero, L_80002628
    if (ctx->r1 != 0) {
        // 0x80002648: lh          $t6, 0x0($a1)
        ctx->r14 = MEM_H(ctx->r5, 0X0);
            goto L_80002628;
    }
    goto skip_2;
    // 0x80002648: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    skip_2:
    // 0x8000264C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80002650:
    // 0x80002650: jr          $ra
    // 0x80002654: nop

    return;
    // 0x80002654: nop

;}
RECOMP_FUNC void func_80002658(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002658: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x8000265C: lw          $a1, 0x1530($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X1530);
    // 0x80002660: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80002664: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002668: jal         0x8000260C
    // 0x8000266C: addiu       $a1, $a1, -0x74
    ctx->r5 = ADD32(ctx->r5, -0X74);
    func_8000260C(rdram, ctx);
        goto after_0;
    // 0x8000266C: addiu       $a1, $a1, -0x74
    ctx->r5 = ADD32(ctx->r5, -0X74);
    after_0:
    // 0x80002670: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80002674: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80002678: jr          $ra
    // 0x8000267C: nop

    return;
    // 0x8000267C: nop

    // 0x80002680: jr          $ra
    // 0x80002684: nop

    return;
    // 0x80002684: nop

;}
RECOMP_FUNC void func_80002688(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002688: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8000268C: lh          $v1, 0x0($a0)
    ctx->r3 = MEM_H(ctx->r4, 0X0);
    // 0x80002690: lh          $t6, 0x4($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X4);
    // 0x80002694: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80002698: andi        $v1, $v1, 0x7FF
    ctx->r3 = ctx->r3 & 0X7FF;
    // 0x8000269C: slt         $v0, $v1, $t6
    ctx->r2 = SIGNED(ctx->r3) < SIGNED(ctx->r14) ? 1 : 0;
    // 0x800026A0: xori        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 ^ 0X1;
    // 0x800026A4: beq         $v0, $zero, L_800026B4
    if (ctx->r2 == 0) {
        // 0x800026A8: sra         $a1, $a1, 16
        ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
            goto L_800026B4;
    }
    // 0x800026A8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800026AC: slt         $v0, $a1, $v1
    ctx->r2 = SIGNED(ctx->r5) < SIGNED(ctx->r3) ? 1 : 0;
    // 0x800026B0: xori        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 ^ 0X1;
L_800026B4:
    // 0x800026B4: jr          $ra
    // 0x800026B8: nop

    return;
    // 0x800026B8: nop

;}
RECOMP_FUNC void func_800026BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800026BC: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x800026C0: jr          $ra
    // 0x800026C4: and         $v0, $t6, $a1
    ctx->r2 = ctx->r14 & ctx->r5;
    return;
    // 0x800026C4: and         $v0, $t6, $a1
    ctx->r2 = ctx->r14 & ctx->r5;
;}
RECOMP_FUNC void func_800026C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800026C8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800026CC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800026D0: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x800026D4: lui         $a2, 0x8000
    ctx->r6 = S32(0X8000 << 16);
    // 0x800026D8: addiu       $a2, $a2, 0x26BC
    ctx->r6 = ADD32(ctx->r6, 0X26BC);
    // 0x800026DC: lw          $a0, -0x542C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X542C);
    // 0x800026E0: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x800026E4: jal         0x80001FFC
    // 0x800026E8: addiu       $a3, $zero, 0x1000
    ctx->r7 = ADD32(0, 0X1000);
    func_80001FFC(rdram, ctx);
        goto after_0;
    // 0x800026E8: addiu       $a3, $zero, 0x1000
    ctx->r7 = ADD32(0, 0X1000);
    after_0:
    // 0x800026EC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800026F0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800026F4: jr          $ra
    // 0x800026F8: nop

    return;
    // 0x800026F8: nop

;}
RECOMP_FUNC void func_800026FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800026FC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80002700: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80002704: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002708: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8000270C: lui         $a2, 0x8000
    ctx->r6 = S32(0X8000 << 16);
    // 0x80002710: addiu       $a2, $a2, 0x26BC
    ctx->r6 = ADD32(ctx->r6, 0X26BC);
    // 0x80002714: lw          $a0, -0x542C($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X542C);
    // 0x80002718: jal         0x80001FFC
    // 0x8000271C: addiu       $a3, $zero, 0x1000
    ctx->r7 = ADD32(0, 0X1000);
    func_80001FFC(rdram, ctx);
        goto after_0;
    // 0x8000271C: addiu       $a3, $zero, 0x1000
    ctx->r7 = ADD32(0, 0X1000);
    after_0:
    // 0x80002720: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80002724: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80002728: jr          $ra
    // 0x8000272C: nop

    return;
    // 0x8000272C: nop

;}
RECOMP_FUNC void func_80002730(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002730: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002734: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80002738: andi        $t6, $a0, 0x2000
    ctx->r14 = ctx->r4 & 0X2000;
    // 0x8000273C: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80002740: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80002744: beq         $t6, $zero, L_80002794
    if (ctx->r14 == 0) {
        // 0x80002748: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_80002794;
    }
    // 0x80002748: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8000274C: andi        $s1, $a0, 0x7FF
    ctx->r17 = ctx->r4 & 0X7FF;
    // 0x80002750: jal         0x80002560
    // 0x80002754: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80002560(rdram, ctx);
        goto after_0;
    // 0x80002754: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_0:
    // 0x80002758: beq         $v0, $zero, L_8000278C
    if (ctx->r2 == 0) {
        // 0x8000275C: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8000278C;
    }
    // 0x8000275C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80002760:
    // 0x80002760: lhu         $t7, 0x2($v0)
    ctx->r15 = MEM_HU(ctx->r2, 0X2);
    // 0x80002764: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80002768: andi        $t8, $t7, 0x2000
    ctx->r24 = ctx->r15 & 0X2000;
    // 0x8000276C: bne         $t8, $zero, L_8000277C
    if (ctx->r24 != 0) {
        // 0x80002770: nop
    
            goto L_8000277C;
    }
    // 0x80002770: nop

    // 0x80002774: b           L_8000279C
    // 0x80002778: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
        goto L_8000279C;
    // 0x80002778: or          $v0, $s0, $zero
    ctx->r2 = ctx->r16 | 0;
L_8000277C:
    // 0x8000277C: jal         0x80002510
    // 0x80002780: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80002510(rdram, ctx);
        goto after_1;
    // 0x80002780: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x80002784: bne         $v0, $zero, L_80002760
    if (ctx->r2 != 0) {
        // 0x80002788: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_80002760;
    }
    // 0x80002788: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_8000278C:
    // 0x8000278C: b           L_8000279C
    // 0x80002790: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8000279C;
    // 0x80002790: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80002794:
    // 0x80002794: jal         0x80002560
    // 0x80002798: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80002560(rdram, ctx);
        goto after_2;
    // 0x80002798: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_2:
L_8000279C:
    // 0x8000279C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800027A0: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x800027A4: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x800027A8: jr          $ra
    // 0x800027AC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x800027AC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_800027B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800027B0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800027B4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800027B8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x800027BC: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x800027C0: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    // 0x800027C4: lhu         $t7, 0x20($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X20);
    // 0x800027C8: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x800027CC: sllv        $t0, $t9, $a3
    ctx->r8 = S32(ctx->r25 << (ctx->r7 & 31));
    // 0x800027D0: or          $t1, $t7, $t0
    ctx->r9 = ctx->r15 | ctx->r8;
    // 0x800027D4: sh          $t1, 0x20($a0)
    MEM_H(0X20, ctx->r4) = ctx->r9;
    // 0x800027D8: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x800027DC: jal         0x80001514
    // 0x800027E0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    func_80001514(rdram, ctx);
        goto after_0;
    // 0x800027E0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    after_0:
    // 0x800027E4: lw          $t3, 0x24($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X24);
    // 0x800027E8: lw          $t2, 0x18($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X18);
    // 0x800027EC: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x800027F0: addu        $t5, $t2, $t4
    ctx->r13 = ADD32(ctx->r10, ctx->r12);
    // 0x800027F4: sw          $v0, 0x34($t5)
    MEM_W(0X34, ctx->r13) = ctx->r2;
    // 0x800027F8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800027FC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80002800: jr          $ra
    // 0x80002804: nop

    return;
    // 0x80002804: nop

;}
RECOMP_FUNC void func_80002808(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002808: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8000280C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002810: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80002814: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80002818: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x8000281C: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    // 0x80002820: lhu         $t7, 0x20($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X20);
    // 0x80002824: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x80002828: sllv        $t0, $t9, $a3
    ctx->r8 = S32(ctx->r25 << (ctx->r7 & 31));
    // 0x8000282C: or          $t1, $t7, $t0
    ctx->r9 = ctx->r15 | ctx->r8;
    // 0x80002830: sh          $t1, 0x20($a0)
    MEM_H(0X20, ctx->r4) = ctx->r9;
    // 0x80002834: lw          $a1, 0x28($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X28);
    // 0x80002838: jal         0x80001514
    // 0x8000283C: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    func_80001514(rdram, ctx);
        goto after_0;
    // 0x8000283C: lw          $a0, 0x24($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X24);
    after_0:
    // 0x80002840: lw          $t3, 0x2C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X2C);
    // 0x80002844: lw          $t2, 0x20($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X20);
    // 0x80002848: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8000284C: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x80002850: addu        $v1, $t2, $t4
    ctx->r3 = ADD32(ctx->r10, ctx->r12);
    // 0x80002854: beq         $v0, $zero, L_8000286C
    if (ctx->r2 == 0) {
        // 0x80002858: sw          $v0, 0x34($v1)
        MEM_W(0X34, ctx->r3) = ctx->r2;
            goto L_8000286C;
    }
    // 0x80002858: sw          $v0, 0x34($v1)
    MEM_W(0X34, ctx->r3) = ctx->r2;
    // 0x8000285C: lw          $a1, 0x28($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X28);
    // 0x80002860: jal         0x80000F30
    // 0x80002864: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    bzero_recomp(rdram, ctx);
        goto after_1;
    // 0x80002864: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_1:
    // 0x80002868: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
L_8000286C:
    // 0x8000286C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80002870: lw          $v0, 0x34($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X34);
    // 0x80002874: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80002878: jr          $ra
    // 0x8000287C: nop

    return;
    // 0x8000287C: nop

;}
RECOMP_FUNC void func_80002880(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002880: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80002884: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002888: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8000288C: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80002890: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    // 0x80002894: lhu         $t7, 0x22($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X22);
    // 0x80002898: addiu       $t9, $zero, 0x1
    ctx->r25 = ADD32(0, 0X1);
    // 0x8000289C: sllv        $t0, $t9, $a3
    ctx->r8 = S32(ctx->r25 << (ctx->r7 & 31));
    // 0x800028A0: or          $t1, $t7, $t0
    ctx->r9 = ctx->r15 | ctx->r8;
    // 0x800028A4: sh          $t1, 0x22($a0)
    MEM_H(0X22, ctx->r4) = ctx->r9;
    // 0x800028A8: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    // 0x800028AC: jal         0x80001858
    // 0x800028B0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    func_80001858(rdram, ctx);
        goto after_0;
    // 0x800028B0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    after_0:
    // 0x800028B4: lw          $t3, 0x24($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X24);
    // 0x800028B8: lw          $t2, 0x18($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X18);
    // 0x800028BC: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x800028C0: addu        $t5, $t2, $t4
    ctx->r13 = ADD32(ctx->r10, ctx->r12);
    // 0x800028C4: sw          $v0, 0x34($t5)
    MEM_W(0X34, ctx->r13) = ctx->r2;
    // 0x800028C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800028CC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800028D0: jr          $ra
    // 0x800028D4: nop

    return;
    // 0x800028D4: nop

;}
RECOMP_FUNC void func_800028D8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800028D8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800028DC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800028E0: lhu         $t7, 0x20($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X20);
    // 0x800028E4: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x800028E8: sllv        $v0, $t6, $a1
    ctx->r2 = S32(ctx->r14 << (ctx->r5 & 31));
    // 0x800028EC: and         $t8, $t7, $v0
    ctx->r24 = ctx->r15 & ctx->r2;
    // 0x800028F0: beq         $t8, $zero, L_80002920
    if (ctx->r24 == 0) {
        // 0x800028F4: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_80002920;
    }
    // 0x800028F4: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x800028F8: sll         $t9, $a1, 2
    ctx->r25 = S32(ctx->r5 << 2);
    // 0x800028FC: addu        $t0, $a0, $t9
    ctx->r8 = ADD32(ctx->r4, ctx->r25);
    // 0x80002900: lw          $a0, 0x34($t0)
    ctx->r4 = MEM_W(ctx->r8, 0X34);
    // 0x80002904: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x80002908: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x8000290C: jal         0x80001918
    // 0x80002910: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_80001918(rdram, ctx);
        goto after_0;
    // 0x80002910: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x80002914: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80002918: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x8000291C: lw          $a2, 0x20($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X20);
L_80002920:
    // 0x80002920: lhu         $t1, 0x22($a2)
    ctx->r9 = MEM_HU(ctx->r6, 0X22);
    // 0x80002924: sll         $t3, $a1, 2
    ctx->r11 = S32(ctx->r5 << 2);
    // 0x80002928: addu        $t4, $a2, $t3
    ctx->r12 = ADD32(ctx->r6, ctx->r11);
    // 0x8000292C: and         $t2, $t1, $v0
    ctx->r10 = ctx->r9 & ctx->r2;
    // 0x80002930: beql        $t2, $zero, L_80002944
    if (ctx->r10 == 0) {
        // 0x80002934: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80002944;
    }
    goto skip_0;
    // 0x80002934: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80002938: jal         0x80001920
    // 0x8000293C: lw          $a0, 0x34($t4)
    ctx->r4 = MEM_W(ctx->r12, 0X34);
    func_80001920(rdram, ctx);
        goto after_1;
    // 0x8000293C: lw          $a0, 0x34($t4)
    ctx->r4 = MEM_W(ctx->r12, 0X34);
    after_1:
    // 0x80002940: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80002944:
    // 0x80002944: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80002948: jr          $ra
    // 0x8000294C: nop

    return;
    // 0x8000294C: nop

;}
RECOMP_FUNC void func_80002950(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002950: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80002954: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002958: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x8000295C: andi        $t6, $v0, 0x2000
    ctx->r14 = ctx->r2 & 0X2000;
    // 0x80002960: beql        $t6, $zero, L_800029B8
    if (ctx->r14 == 0) {
        // 0x80002964: lhu         $t2, 0x2($a0)
        ctx->r10 = MEM_HU(ctx->r4, 0X2);
            goto L_800029B8;
    }
    goto skip_0;
    // 0x80002964: lhu         $t2, 0x2($a0)
    ctx->r10 = MEM_HU(ctx->r4, 0X2);
    skip_0:
    // 0x80002968: lhu         $t7, 0x2($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X2);
    // 0x8000296C: andi        $t8, $t7, 0x2000
    ctx->r24 = ctx->r15 & 0X2000;
    // 0x80002970: bnel        $t8, $zero, L_800029E0
    if (ctx->r24 != 0) {
        // 0x80002974: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_800029E0;
    }
    goto skip_1;
    // 0x80002974: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80002978: jal         0x80011060
    // 0x8000297C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80011060(rdram, ctx);
        goto after_0;
    // 0x8000297C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80002980: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80002984: lh          $t9, 0x0($a0)
    ctx->r25 = MEM_H(ctx->r4, 0X0);
    // 0x80002988: andi        $t0, $t9, 0x7FF
    ctx->r8 = ctx->r25 & 0X7FF;
    // 0x8000298C: sll         $t1, $t0, 2
    ctx->r9 = S32(ctx->r8 << 2);
    // 0x80002990: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80002994: addu        $t9, $t9, $t1
    ctx->r25 = ADD32(ctx->r25, ctx->r9);
    // 0x80002998: lw          $t9, -0x29C0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X29C0);
    // 0x8000299C: jalr        $t9
    // 0x800029A0: nop

    // Translate N64 segment addresses to overlay vram.
    // Dynamic overlay sections 4-50 all share vram 0x802E3B70.
    // overlay_system (section 3) is at 0x801CAEA0.
    // The overlay slot table determines which is active, but since our bypass
    // doesn't update it, use the dynamic overlay vram as default.
    {
        uint32_t orig = (uint32_t)ctx->r25;
        if ((orig >> 24) == 0x0F) {
            uint32_t offset = orig & 0x00FFFFFF;
            // Read current overlay vram from slot 0 at 0x800C15E8
            uint32_t ovl_vram = (uint32_t)MEM_W(S32(0x800C15E8), 0);
            if (ovl_vram == 0 || ovl_vram < 0x80000000) {
                // Use dynamic overlay vram (all sections 4-50 load here)
                ovl_vram = 0x802E3B70;
            }
            ctx->r25 = (gpr)(int32_t)(ovl_vram + offset);
        }
    }
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x800029A0: nop

    after_1:
    // 0x800029A4: jal         0x8001123C
    // 0x800029A8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    func_8001123C(rdram, ctx);
        goto after_2;
    // 0x800029A8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    after_2:
    // 0x800029AC: b           L_800029E0
    // 0x800029B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_800029E0;
    // 0x800029B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800029B4: lhu         $t2, 0x2($a0)
    ctx->r10 = MEM_HU(ctx->r4, 0X2);
L_800029B8:
    // 0x800029B8: andi        $t4, $v0, 0x7FF
    ctx->r12 = ctx->r2 & 0X7FF;
    // 0x800029BC: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x800029C0: andi        $t3, $t2, 0x2000
    ctx->r11 = ctx->r10 & 0X2000;
    // 0x800029C4: bne         $t3, $zero, L_800029DC
    if (ctx->r11 != 0) {
        // 0x800029C8: lui         $t9, 0x800B
        ctx->r25 = S32(0X800B << 16);
            goto L_800029DC;
    }
    // 0x800029C8: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x800029CC: addu        $t9, $t9, $t5
    ctx->r25 = ADD32(ctx->r25, ctx->r13);
    // 0x800029D0: lw          $t9, -0x29C0($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X29C0);
    // 0x800029D4: jalr        $t9
    // 0x800029D8: nop

    // Translate segment addresses for path B too
    if (((uint32_t)ctx->r25 >> 24) == 0x0F) {
        uint32_t offset = (uint32_t)ctx->r25 & 0x00FFFFFF;
        uint32_t ovl_vram = (uint32_t)MEM_W(S32(0x800C15E8), 0);
        if (ovl_vram == 0 || ovl_vram < 0x80000000) ovl_vram = 0x802E3B70;
        ctx->r25 = (gpr)(int32_t)(ovl_vram + offset);
    }
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x800029D8: nop

    after_3:
    // PATCH: Call func_8001123C (child linked list traversal) after path B too.
    // On real N64, overlay children in the linked list are traversed here.
    // Without this, overlay objects (like the Konami logo renderer) never execute.
    ctx->r4 = MEM_W(ctx->r29, 0X18);  // restore obj ptr from stack
    func_8001123C(rdram, ctx);
L_800029DC:
    // 0x800029DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_800029E0:
    // 0x800029E0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800029E4: jr          $ra
    // 0x800029E8: nop

    return;
    // 0x800029E8: nop

;}
RECOMP_FUNC void func_800029EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800029EC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800029F0: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x800029F4: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x800029F8: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x800029FC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80002A00: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
L_80002A04:
    // 0x80002A04: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    // 0x80002A08: blezl       $t6, L_80002A1C
    if (SIGNED(ctx->r14) <= 0) {
        // 0x80002A0C: lw          $v0, 0x1C($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X1C);
            goto L_80002A1C;
    }
    goto skip_0;
    // 0x80002A0C: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    skip_0:
    // 0x80002A10: jal         0x80002950
    // 0x80002A14: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80002950(rdram, ctx);
        goto after_0;
    // 0x80002A14: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
    // 0x80002A18: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
L_80002A1C:
    // 0x80002A1C: beq         $v0, $zero, L_80002A44
    if (ctx->r2 == 0) {
        // 0x80002A20: nop
    
            goto L_80002A44;
    }
    // 0x80002A20: nop

    // 0x80002A24: lh          $t7, 0x0($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X0);
    // 0x80002A28: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x80002A2C: blezl       $t7, L_80002A1C
    if (SIGNED(ctx->r15) <= 0) {
        // 0x80002A30: lw          $v0, 0x1C($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X1C);
            goto L_80002A1C;
    }
    goto skip_1;
    // 0x80002A30: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
    skip_1:
    // 0x80002A34: jal         0x80002950
    // 0x80002A38: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    func_80002950(rdram, ctx);
        goto after_1;
    // 0x80002A38: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    after_1:
    // 0x80002A3C: b           L_80002A1C
    // 0x80002A40: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
        goto L_80002A1C;
    // 0x80002A40: lw          $v0, 0x1C($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X1C);
L_80002A44:
    // 0x80002A44: beql        $s0, $s1, L_80002A84
    if (ctx->r16 == ctx->r17) {
        // 0x80002A48: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80002A84;
    }
    goto skip_2;
    // 0x80002A48: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x80002A4C: lw          $v0, 0x18($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X18);
    // 0x80002A50: bne         $v0, $zero, L_80002A78
    if (ctx->r2 != 0) {
        // 0x80002A54: nop
    
            goto L_80002A78;
    }
    // 0x80002A54: nop

    // 0x80002A58: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
L_80002A5C:
    // 0x80002A5C: beql        $v0, $zero, L_80002A84
    if (ctx->r2 == 0) {
        // 0x80002A60: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80002A84;
    }
    goto skip_3;
    // 0x80002A60: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_3:
    // 0x80002A64: beq         $v0, $s1, L_80002A80
    if (ctx->r2 == ctx->r17) {
        // 0x80002A68: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_80002A80;
    }
    // 0x80002A68: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x80002A6C: lw          $v0, 0x18($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X18);
    // 0x80002A70: beql        $v0, $zero, L_80002A5C
    if (ctx->r2 == 0) {
        // 0x80002A74: lw          $v0, 0x14($s0)
        ctx->r2 = MEM_W(ctx->r16, 0X14);
            goto L_80002A5C;
    }
    goto skip_4;
    // 0x80002A74: lw          $v0, 0x14($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X14);
    skip_4:
L_80002A78:
    // 0x80002A78: b           L_80002A04
    // 0x80002A7C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
        goto L_80002A04;
    // 0x80002A7C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
L_80002A80:
    // 0x80002A80: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80002A84:
    // 0x80002A84: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80002A88: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80002A8C: jr          $ra
    // 0x80002A90: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x80002A90: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80002A94(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002A94: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002A98: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002A9C: jal         0x800A2550
    // 0x80002AA0: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    osGetTime_recomp(rdram, ctx);
        goto after_0;
    // 0x80002AA0: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    after_0:
    // 0x80002AA4: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    // 0x80002AA8: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    // 0x80002AAC: jal         0x800029EC
    // 0x80002AB0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_800029EC(rdram, ctx);
        goto after_1;
    // 0x80002AB0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x80002AB4: jal         0x800A2550
    // 0x80002AB8: nop

    osGetTime_recomp(rdram, ctx);
        goto after_2;
    // 0x80002AB8: nop

    after_2:
    // 0x80002ABC: lw          $t6, 0x18($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X18);
    // 0x80002AC0: lw          $t7, 0x1C($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X1C);
    // 0x80002AC4: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x80002AC8: subu        $a0, $v0, $t6
    ctx->r4 = SUB32(ctx->r2, ctx->r14);
    // 0x80002ACC: sltu        $at, $v1, $t7
    ctx->r1 = ctx->r3 < ctx->r15 ? 1 : 0;
    // 0x80002AD0: subu        $a0, $a0, $at
    ctx->r4 = SUB32(ctx->r4, ctx->r1);
    // 0x80002AD4: addiu       $a3, $zero, 0x40
    ctx->r7 = ADD32(0, 0X40);
    // 0x80002AD8: jal         0x80097958
    // 0x80002ADC: subu        $a1, $v1, $t7
    ctx->r5 = SUB32(ctx->r3, ctx->r15);
    func_80097958(rdram, ctx);
        goto after_3;
    // 0x80002ADC: subu        $a1, $v1, $t7
    ctx->r5 = SUB32(ctx->r3, ctx->r15);
    after_3:
    // 0x80002AE0: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80002AE4: or          $a1, $v1, $zero
    ctx->r5 = ctx->r3 | 0;
    // 0x80002AE8: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x80002AEC: jal         0x80097858
    // 0x80002AF0: addiu       $a3, $zero, 0xBB8
    ctx->r7 = ADD32(0, 0XBB8);
    __ull_div_recomp(rdram, ctx);
        goto after_4;
    // 0x80002AF0: addiu       $a3, $zero, 0xBB8
    ctx->r7 = ADD32(0, 0XBB8);
    after_4:
    // 0x80002AF4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80002AF8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80002AFC: jr          $ra
    // 0x80002B00: nop

    return;
    // 0x80002B00: nop

;}
RECOMP_FUNC void func_80002B10(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002B10: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80002B14: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80002B18: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80002B1C: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80002B20: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80002B24: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80002B28: lh          $t6, 0x0($a0)
    ctx->r14 = MEM_H(ctx->r4, 0X0);
    // 0x80002B2C: lhu         $t8, 0x2($a0)
    ctx->r24 = MEM_HU(ctx->r4, 0X2);
    // 0x80002B30: lw          $a1, 0x14($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X14);
    // 0x80002B34: ori         $t7, $t6, 0x8000
    ctx->r15 = ctx->r14 | 0X8000;
    // 0x80002B38: ori         $t9, $t8, 0x8000
    ctx->r25 = ctx->r24 | 0X8000;
    // 0x80002B3C: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80002B40: sh          $t7, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r15;
    // 0x80002B44: beq         $a1, $zero, L_80002BB4
    if (ctx->r5 == 0) {
        // 0x80002B48: sh          $t9, 0x2($a0)
        MEM_H(0X2, ctx->r4) = ctx->r25;
            goto L_80002BB4;
    }
    // 0x80002B48: sh          $t9, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r25;
    // 0x80002B4C: lw          $a0, 0x1C($a1)
    ctx->r4 = MEM_W(ctx->r5, 0X1C);
    // 0x80002B50: bnel        $s3, $a0, L_80002B7C
    if (ctx->r19 != ctx->r4) {
        // 0x80002B54: lw          $v0, 0x18($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X18);
            goto L_80002B7C;
    }
    goto skip_0;
    // 0x80002B54: lw          $v0, 0x18($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X18);
    skip_0:
    // 0x80002B58: lw          $a0, 0x1C($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X1C);
    // 0x80002B5C: beql        $a0, $zero, L_80002B70
    if (ctx->r4 == 0) {
        // 0x80002B60: lw          $t0, 0x18($s3)
        ctx->r8 = MEM_W(ctx->r19, 0X18);
            goto L_80002B70;
    }
    goto skip_1;
    // 0x80002B60: lw          $t0, 0x18($s3)
    ctx->r8 = MEM_W(ctx->r19, 0X18);
    skip_1:
    // 0x80002B64: b           L_80002BB4
    // 0x80002B68: sw          $a0, 0x1C($a1)
    MEM_W(0X1C, ctx->r5) = ctx->r4;
        goto L_80002BB4;
    // 0x80002B68: sw          $a0, 0x1C($a1)
    MEM_W(0X1C, ctx->r5) = ctx->r4;
    // 0x80002B6C: lw          $t0, 0x18($s3)
    ctx->r8 = MEM_W(ctx->r19, 0X18);
L_80002B70:
    // 0x80002B70: b           L_80002BB4
    // 0x80002B74: sw          $t0, 0x1C($a1)
    MEM_W(0X1C, ctx->r5) = ctx->r8;
        goto L_80002BB4;
    // 0x80002B74: sw          $t0, 0x1C($a1)
    MEM_W(0X1C, ctx->r5) = ctx->r8;
    // 0x80002B78: lw          $v0, 0x18($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X18);
L_80002B7C:
    // 0x80002B7C: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80002B80: beql        $s3, $v0, L_80002B9C
    if (ctx->r19 == ctx->r2) {
        // 0x80002B84: lw          $a0, 0x1C($s3)
        ctx->r4 = MEM_W(ctx->r19, 0X1C);
            goto L_80002B9C;
    }
    goto skip_2;
    // 0x80002B84: lw          $a0, 0x1C($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X1C);
    skip_2:
    // 0x80002B88: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80002B8C:
    // 0x80002B8C: lw          $v0, 0x18($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X18);
    // 0x80002B90: bnel        $s3, $v0, L_80002B8C
    if (ctx->r19 != ctx->r2) {
        // 0x80002B94: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80002B8C;
    }
    goto skip_3;
    // 0x80002B94: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    skip_3:
    // 0x80002B98: lw          $a0, 0x1C($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X1C);
L_80002B9C:
    // 0x80002B9C: beql        $a0, $zero, L_80002BB0
    if (ctx->r4 == 0) {
        // 0x80002BA0: lw          $t1, 0x18($s3)
        ctx->r9 = MEM_W(ctx->r19, 0X18);
            goto L_80002BB0;
    }
    goto skip_4;
    // 0x80002BA0: lw          $t1, 0x18($s3)
    ctx->r9 = MEM_W(ctx->r19, 0X18);
    skip_4:
    // 0x80002BA4: b           L_80002BB4
    // 0x80002BA8: sw          $a0, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r4;
        goto L_80002BB4;
    // 0x80002BA8: sw          $a0, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r4;
    // 0x80002BAC: lw          $t1, 0x18($s3)
    ctx->r9 = MEM_W(ctx->r19, 0X18);
L_80002BB0:
    // 0x80002BB0: sw          $t1, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r9;
L_80002BB4:
    // 0x80002BB4: lw          $a0, 0x1C($s3)
    ctx->r4 = MEM_W(ctx->r19, 0X1C);
    // 0x80002BB8: addiu       $s0, $s3, 0x24
    ctx->r16 = ADD32(ctx->r19, 0X24);
    // 0x80002BBC: addiu       $s1, $zero, 0x3
    ctx->r17 = ADD32(0, 0X3);
    // 0x80002BC0: beq         $a0, $zero, L_80002C04
    if (ctx->r4 == 0) {
        // 0x80002BC4: addiu       $s2, $zero, -0x8000
        ctx->r18 = ADD32(0, -0X8000);
            goto L_80002C04;
    }
    // 0x80002BC4: addiu       $s2, $zero, -0x8000
    ctx->r18 = ADD32(0, -0X8000);
    // 0x80002BC8: lw          $v0, 0x18($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X18);
    // 0x80002BCC: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80002BD0: beql        $v0, $zero, L_80002C00
    if (ctx->r2 == 0) {
        // 0x80002BD4: lw          $t3, 0x18($s3)
        ctx->r11 = MEM_W(ctx->r19, 0X18);
            goto L_80002C00;
    }
    goto skip_5;
    // 0x80002BD4: lw          $t3, 0x18($s3)
    ctx->r11 = MEM_W(ctx->r19, 0X18);
    skip_5:
    // 0x80002BD8: lw          $t2, 0x14($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X14);
L_80002BDC:
    // 0x80002BDC: beql        $t2, $zero, L_80002BF0
    if (ctx->r10 == 0) {
        // 0x80002BE0: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80002BF0;
    }
    goto skip_6;
    // 0x80002BE0: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    skip_6:
    // 0x80002BE4: sw          $a1, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->r5;
    // 0x80002BE8: lw          $v0, 0x18($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X18);
    // 0x80002BEC: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80002BF0:
    // 0x80002BF0: lw          $v0, 0x18($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X18);
    // 0x80002BF4: bnel        $v0, $zero, L_80002BDC
    if (ctx->r2 != 0) {
        // 0x80002BF8: lw          $t2, 0x14($v1)
        ctx->r10 = MEM_W(ctx->r3, 0X14);
            goto L_80002BDC;
    }
    goto skip_7;
    // 0x80002BF8: lw          $t2, 0x14($v1)
    ctx->r10 = MEM_W(ctx->r3, 0X14);
    skip_7:
    // 0x80002BFC: lw          $t3, 0x18($s3)
    ctx->r11 = MEM_W(ctx->r19, 0X18);
L_80002C00:
    // 0x80002C00: sw          $t3, 0x18($v1)
    MEM_W(0X18, ctx->r3) = ctx->r11;
L_80002C04:
    // 0x80002C04: jal         0x80004E58
    // 0x80002C08: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    func_80004E58(rdram, ctx);
        goto after_0;
    // 0x80002C08: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    after_0:
    // 0x80002C0C: beql        $v0, $zero, L_80002C3C
    if (ctx->r2 == 0) {
        // 0x80002C10: addiu       $s1, $s1, -0x1
        ctx->r17 = ADD32(ctx->r17, -0X1);
            goto L_80002C3C;
    }
    goto skip_8;
    // 0x80002C10: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
    skip_8:
    // 0x80002C14: lw          $a0, 0x0($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X0);
    // 0x80002C18: lh          $v0, 0x0($a0)
    ctx->r2 = MEM_H(ctx->r4, 0X0);
    // 0x80002C1C: beql        $s2, $v0, L_80002C3C
    if (ctx->r18 == ctx->r2) {
        // 0x80002C20: addiu       $s1, $s1, -0x1
        ctx->r17 = ADD32(ctx->r17, -0X1);
            goto L_80002C3C;
    }
    goto skip_9;
    // 0x80002C20: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
    skip_9:
    // 0x80002C24: beql        $v0, $zero, L_80002C38
    if (ctx->r2 == 0) {
        // 0x80002C28: sw          $zero, 0x0($s0)
        MEM_W(0X0, ctx->r16) = 0;
            goto L_80002C38;
    }
    goto skip_10;
    // 0x80002C28: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
    skip_10:
    // 0x80002C2C: jal         0x8000516C
    // 0x80002C30: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_8000516C(rdram, ctx);
        goto after_1;
    // 0x80002C30: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_1:
    // 0x80002C34: sw          $zero, 0x0($s0)
    MEM_W(0X0, ctx->r16) = 0;
L_80002C38:
    // 0x80002C38: addiu       $s1, $s1, -0x1
    ctx->r17 = ADD32(ctx->r17, -0X1);
L_80002C3C:
    // 0x80002C3C: bgez        $s1, L_80002C04
    if (SIGNED(ctx->r17) >= 0) {
        // 0x80002C40: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80002C04;
    }
    // 0x80002C40: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80002C44: lui         $a2, 0x801D
    ctx->r6 = S32(0X801D << 16);
    // 0x80002C48: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80002C4C: lh          $a2, -0x7D1C($a2)
    ctx->r6 = MEM_H(ctx->r6, -0X7D1C);
    // 0x80002C50: lw          $a0, 0x1540($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1540);
    // 0x80002C54: jal         0x80002D5C
    // 0x80002C58: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    func_80002D5C(rdram, ctx);
        goto after_2;
    // 0x80002C58: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    after_2:
    // 0x80002C5C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80002C60: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80002C64: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80002C68: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80002C6C: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80002C70: jr          $ra
    // 0x80002C74: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80002C74: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80002C78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002C78: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002C7C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80002C80: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80002C84: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80002C88: lw          $a1, 0x1C($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X1C);
    // 0x80002C8C: beq         $a1, $zero, L_80002CCC
    if (ctx->r5 == 0) {
        // 0x80002C90: or          $s0, $a1, $zero
        ctx->r16 = ctx->r5 | 0;
            goto L_80002CCC;
    }
    // 0x80002C90: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80002C94: lw          $t9, 0x10($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X10);
    // 0x80002C98: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80002C9C: jalr        $t9
    // 0x80002CA0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80002CA0: nop

    after_0:
    // 0x80002CA4: lw          $s0, 0x18($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X18);
    // 0x80002CA8: beql        $s0, $zero, L_80002CD0
    if (ctx->r16 == 0) {
        // 0x80002CAC: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80002CD0;
    }
    goto skip_0;
    // 0x80002CAC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
L_80002CB0:
    // 0x80002CB0: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80002CB4: lw          $s1, 0x18($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X18);
    // 0x80002CB8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80002CBC: jalr        $t9
    // 0x80002CC0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80002CC0: nop

    after_1:
    // 0x80002CC4: bne         $s1, $zero, L_80002CB0
    if (ctx->r17 != 0) {
        // 0x80002CC8: or          $s0, $s1, $zero
        ctx->r16 = ctx->r17 | 0;
            goto L_80002CB0;
    }
    // 0x80002CC8: or          $s0, $s1, $zero
    ctx->r16 = ctx->r17 | 0;
L_80002CCC:
    // 0x80002CCC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80002CD0:
    // 0x80002CD0: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80002CD4: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80002CD8: jr          $ra
    // 0x80002CDC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x80002CDC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80002CE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002CE0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002CE4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80002CE8: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80002CEC: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80002CF0: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80002CF4: lw          $a1, 0x1C($a0)
    ctx->r5 = MEM_W(ctx->r4, 0X1C);
    // 0x80002CF8: beq         $a1, $zero, L_80002D38
    if (ctx->r5 == 0) {
        // 0x80002CFC: or          $s0, $a1, $zero
        ctx->r16 = ctx->r5 | 0;
            goto L_80002D38;
    }
    // 0x80002CFC: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x80002D00: lw          $t9, 0x10($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X10);
    // 0x80002D04: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80002D08: jalr        $t9
    // 0x80002D0C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80002D0C: nop

    after_0:
    // 0x80002D10: lw          $s0, 0x18($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X18);
    // 0x80002D14: beq         $s0, $zero, L_80002D38
    if (ctx->r16 == 0) {
        // 0x80002D18: nop
    
            goto L_80002D38;
    }
    // 0x80002D18: nop

L_80002D1C:
    // 0x80002D1C: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80002D20: lw          $s1, 0x18($s0)
    ctx->r17 = MEM_W(ctx->r16, 0X18);
    // 0x80002D24: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80002D28: jalr        $t9
    // 0x80002D2C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80002D2C: nop

    after_1:
    // 0x80002D30: bne         $s1, $zero, L_80002D1C
    if (ctx->r17 != 0) {
        // 0x80002D34: or          $s0, $s1, $zero
        ctx->r16 = ctx->r17 | 0;
            goto L_80002D1C;
    }
    // 0x80002D34: or          $s0, $s1, $zero
    ctx->r16 = ctx->r17 | 0;
L_80002D38:
    // 0x80002D38: jal         0x80002B10
    // 0x80002D3C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_80002B10(rdram, ctx);
        goto after_2;
    // 0x80002D3C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_2:
    // 0x80002D40: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80002D44: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80002D48: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80002D4C: jr          $ra
    // 0x80002D50: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x80002D50: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80002D54(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002D54: jr          $ra
    // 0x80002D58: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    return;
    // 0x80002D58: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
;}
RECOMP_FUNC void func_80002D5C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002D5C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80002D60: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80002D64: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80002D68: lw          $t7, 0x34($a0)
    ctx->r15 = MEM_W(ctx->r4, 0X34);
    // 0x80002D6C: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80002D70: beq         $t7, $zero, L_80002D9C
    if (ctx->r15 == 0) {
        // 0x80002D74: sw          $t7, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r15;
            goto L_80002D9C;
    }
    // 0x80002D74: sw          $t7, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r15;
L_80002D78:
    // 0x80002D78: lw          $t8, 0x18($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X18);
    // 0x80002D7C: lw          $t1, 0x18($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X18);
    // 0x80002D80: lw          $t9, 0x4($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X4);
    // 0x80002D84: lw          $t0, 0x4($t9)
    ctx->r8 = MEM_W(ctx->r25, 0X4);
    // 0x80002D88: beql        $a2, $t0, L_80002DA0
    if (ctx->r6 == ctx->r8) {
        // 0x80002D8C: sw          $a1, 0x24($sp)
        MEM_W(0X24, ctx->r29) = ctx->r5;
            goto L_80002DA0;
    }
    goto skip_0;
    // 0x80002D8C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    skip_0:
    // 0x80002D90: lw          $t2, 0x0($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X0);
    // 0x80002D94: bne         $t2, $zero, L_80002D78
    if (ctx->r10 != 0) {
        // 0x80002D98: sw          $t2, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r10;
            goto L_80002D78;
    }
    // 0x80002D98: sw          $t2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r10;
L_80002D9C:
    // 0x80002D9C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
L_80002DA0:
    // 0x80002DA0: jal         0x80001FD0
    // 0x80002DA4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    func_80001FD0(rdram, ctx);
        goto after_0;
    // 0x80002DA4: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_0:
    // 0x80002DA8: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80002DAC: beq         $v0, $zero, L_80002DBC
    if (ctx->r2 == 0) {
        // 0x80002DB0: lw          $a2, 0x28($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X28);
            goto L_80002DBC;
    }
    // 0x80002DB0: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80002DB4: lui         $at, 0x4000
    ctx->r1 = S32(0X4000 << 16);
    // 0x80002DB8: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
L_80002DBC:
    // 0x80002DBC: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80002DC0: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80002DC4: jal         0x80005484
    // 0x80002DC8: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    func_80005484(rdram, ctx);
        goto after_1;
    // 0x80002DC8: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_1:
    // 0x80002DCC: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80002DD0: beq         $v0, $zero, L_80002DE0
    if (ctx->r2 == 0) {
        // 0x80002DD4: lw          $a2, 0x28($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X28);
            goto L_80002DE0;
    }
    // 0x80002DD4: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x80002DD8: lui         $at, 0x2000
    ctx->r1 = S32(0X2000 << 16);
    // 0x80002DDC: or          $a1, $a1, $at
    ctx->r5 = ctx->r5 | ctx->r1;
L_80002DE0:
    // 0x80002DE0: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80002DE4: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80002DE8: beq         $a3, $zero, L_80002E04
    if (ctx->r7 == 0) {
        // 0x80002DEC: nop
    
            goto L_80002E04;
    }
    // 0x80002DEC: nop

    // 0x80002DF0: addiu       $a0, $a3, 0x4
    ctx->r4 = ADD32(ctx->r7, 0X4);
    // 0x80002DF4: jal         0x800019E8
    // 0x80002DF8: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    func_800019E8(rdram, ctx);
        goto after_2;
    // 0x80002DF8: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_2:
    // 0x80002DFC: b           L_80002E30
    // 0x80002E00: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80002E30;
    // 0x80002E00: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80002E04:
    // 0x80002E04: jal         0x80001968
    // 0x80002E08: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    func_80001968(rdram, ctx);
        goto after_3;
    // 0x80002E08: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_3:
    // 0x80002E0C: lw          $a1, 0x28($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X28);
    // 0x80002E10: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x80002E14: jal         0x800019B0
    // 0x80002E18: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    func_800019B0(rdram, ctx);
        goto after_4;
    // 0x80002E18: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    after_4:
    // 0x80002E1C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80002E20: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x80002E24: jal         0x800019B0
    // 0x80002E28: addiu       $a0, $a0, 0x34
    ctx->r4 = ADD32(ctx->r4, 0X34);
    func_800019B0(rdram, ctx);
        goto after_5;
    // 0x80002E28: addiu       $a0, $a0, 0x34
    ctx->r4 = ADD32(ctx->r4, 0X34);
    after_5:
    // 0x80002E2C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80002E30:
    // 0x80002E30: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80002E34: jr          $ra
    // 0x80002E38: nop

    return;
    // 0x80002E38: nop

;}
RECOMP_FUNC void func_80002E3C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002E3C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80002E40: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80002E44: lh          $t6, -0x7D18($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X7D18);
    // 0x80002E48: sw          $s7, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r23;
    // 0x80002E4C: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80002E50: or          $s7, $a0, $zero
    ctx->r23 = ctx->r4 | 0;
    // 0x80002E54: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80002E58: sw          $s6, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r22;
    // 0x80002E5C: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x80002E60: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x80002E64: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x80002E68: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x80002E6C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80002E70: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80002E74: bne         $t6, $zero, L_80002F2C
    if (ctx->r14 != 0) {
        // 0x80002E78: sw          $a0, 0x1540($at)
        MEM_W(0X1540, ctx->r1) = ctx->r4;
            goto L_80002F2C;
    }
    // 0x80002E78: sw          $a0, 0x1540($at)
    MEM_W(0X1540, ctx->r1) = ctx->r4;
    // 0x80002E7C: lw          $s6, 0x34($a0)
    ctx->r22 = MEM_W(ctx->r4, 0X34);
    // 0x80002E80: lui         $s5, 0xDFFF
    ctx->r21 = S32(0XDFFF << 16);
    // 0x80002E84: ori         $s5, $s5, 0xFFFF
    ctx->r21 = ctx->r21 | 0XFFFF;
    // 0x80002E88: beq         $s6, $zero, L_80002F24
    if (ctx->r22 == 0) {
        // 0x80002E8C: lui         $s4, 0x2000
        ctx->r20 = S32(0X2000 << 16);
            goto L_80002F24;
    }
    // 0x80002E8C: lui         $s4, 0x2000
    ctx->r20 = S32(0X2000 << 16);
    // 0x80002E90: lui         $s3, 0xBFFF
    ctx->r19 = S32(0XBFFF << 16);
    // 0x80002E94: ori         $s3, $s3, 0xFFFF
    ctx->r19 = ctx->r19 | 0XFFFF;
    // 0x80002E98: lui         $s2, 0x4000
    ctx->r18 = S32(0X4000 << 16);
    // 0x80002E9C: lw          $s1, 0x4($s6)
    ctx->r17 = MEM_W(ctx->r22, 0X4);
L_80002EA0:
    // 0x80002EA0: addiu       $a0, $s7, 0x34
    ctx->r4 = ADD32(ctx->r23, 0X34);
    // 0x80002EA4: lw          $t7, 0x4($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X4);
    // 0x80002EA8: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x80002EAC: bne         $t8, $zero, L_80002F18
    if (ctx->r24 != 0) {
        // 0x80002EB0: sw          $t8, 0x4($s1)
        MEM_W(0X4, ctx->r17) = ctx->r24;
            goto L_80002F18;
    }
    // 0x80002EB0: sw          $t8, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r24;
    // 0x80002EB4: jal         0x80001B00
    // 0x80002EB8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80001B00(rdram, ctx);
        goto after_0;
    // 0x80002EB8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_0:
    // 0x80002EBC: lw          $s0, 0x0($s1)
    ctx->r16 = MEM_W(ctx->r17, 0X0);
    // 0x80002EC0: beql        $s0, $zero, L_80002F1C
    if (ctx->r16 == 0) {
        // 0x80002EC4: lw          $s6, 0x0($s6)
        ctx->r22 = MEM_W(ctx->r22, 0X0);
            goto L_80002F1C;
    }
    goto skip_0;
    // 0x80002EC4: lw          $s6, 0x0($s6)
    ctx->r22 = MEM_W(ctx->r22, 0X0);
    skip_0:
    // 0x80002EC8: lw          $a1, 0x4($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X4);
L_80002ECC:
    // 0x80002ECC: and         $t0, $a1, $s2
    ctx->r8 = ctx->r5 & ctx->r18;
    // 0x80002ED0: beq         $t0, $zero, L_80002EE8
    if (ctx->r8 == 0) {
        // 0x80002ED4: and         $t1, $a1, $s4
        ctx->r9 = ctx->r5 & ctx->r20;
            goto L_80002EE8;
    }
    // 0x80002ED4: and         $t1, $a1, $s4
    ctx->r9 = ctx->r5 & ctx->r20;
    // 0x80002ED8: jal         0x800020E8
    // 0x80002EDC: and         $a0, $a1, $s3
    ctx->r4 = ctx->r5 & ctx->r19;
    func_800020E8(rdram, ctx);
        goto after_1;
    // 0x80002EDC: and         $a0, $a1, $s3
    ctx->r4 = ctx->r5 & ctx->r19;
    after_1:
    // 0x80002EE0: b           L_80002F0C
    // 0x80002EE4: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
        goto L_80002F0C;
    // 0x80002EE4: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
L_80002EE8:
    // 0x80002EE8: beq         $t1, $zero, L_80002F00
    if (ctx->r9 == 0) {
        // 0x80002EEC: nop
    
            goto L_80002F00;
    }
    // 0x80002EEC: nop

    // 0x80002EF0: jal         0x80005530
    // 0x80002EF4: and         $a0, $a1, $s5
    ctx->r4 = ctx->r5 & ctx->r21;
    func_80005530(rdram, ctx);
        goto after_2;
    // 0x80002EF4: and         $a0, $a1, $s5
    ctx->r4 = ctx->r5 & ctx->r21;
    after_2:
    // 0x80002EF8: b           L_80002F0C
    // 0x80002EFC: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
        goto L_80002F0C;
    // 0x80002EFC: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
L_80002F00:
    // 0x80002F00: jal         0x80001920
    // 0x80002F04: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80001920(rdram, ctx);
        goto after_3;
    // 0x80002F04: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_3:
    // 0x80002F08: sw          $zero, 0x4($s0)
    MEM_W(0X4, ctx->r16) = 0;
L_80002F0C:
    // 0x80002F0C: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x80002F10: bnel        $s0, $zero, L_80002ECC
    if (ctx->r16 != 0) {
        // 0x80002F14: lw          $a1, 0x4($s0)
        ctx->r5 = MEM_W(ctx->r16, 0X4);
            goto L_80002ECC;
    }
    goto skip_1;
    // 0x80002F14: lw          $a1, 0x4($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X4);
    skip_1:
L_80002F18:
    // 0x80002F18: lw          $s6, 0x0($s6)
    ctx->r22 = MEM_W(ctx->r22, 0X0);
L_80002F1C:
    // 0x80002F1C: bnel        $s6, $zero, L_80002EA0
    if (ctx->r22 != 0) {
        // 0x80002F20: lw          $s1, 0x4($s6)
        ctx->r17 = MEM_W(ctx->r22, 0X4);
            goto L_80002EA0;
    }
    goto skip_2;
    // 0x80002F20: lw          $s1, 0x4($s6)
    ctx->r17 = MEM_W(ctx->r22, 0X4);
    skip_2:
L_80002F24:
    // 0x80002F24: jal         0x800022E0
    // 0x80002F28: nop

    func_800022E0(rdram, ctx);
        goto after_4;
    // 0x80002F28: nop

    after_4:
L_80002F2C:
    // 0x80002F2C: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80002F30: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80002F34: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80002F38: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x80002F3C: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x80002F40: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x80002F44: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x80002F48: lw          $s6, 0x2C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X2C);
    // 0x80002F4C: lw          $s7, 0x30($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X30);
    // 0x80002F50: jr          $ra
    // 0x80002F54: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x80002F54: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_80002F60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002F60: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80002F64: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80002F68: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80002F6C: lwc1        $f16, 0x10($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X10);
    // 0x80002F70: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80002F74: lwc1        $f6, 0x20($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X20);
    // 0x80002F78: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80002F7C: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80002F80: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80002F84: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80002F88: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x80002F8C: add.s       $f8, $f16, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = ctx->f16.fl + ctx->f4.fl;
    // 0x80002F90: swc1        $f8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->f8.u32l;
    // 0x80002F94: lwc1        $f18, 0x0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80002F98: lwc1        $f6, 0x4($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X4);
    // 0x80002F9C: lwc1        $f16, 0x4($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80002FA0: lwc1        $f4, 0x14($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X14);
    // 0x80002FA4: mul.s       $f10, $f18, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f6.fl);
    // 0x80002FA8: lwc1        $f6, 0x24($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X24);
    // 0x80002FAC: mul.s       $f8, $f16, $f4
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f8.fl = MUL_S(ctx->f16.fl, ctx->f4.fl);
    // 0x80002FB0: lwc1        $f16, 0x8($a0)
    ctx->f16.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80002FB4: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80002FB8: add.s       $f18, $f10, $f8
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f18.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80002FBC: add.s       $f10, $f4, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f18.fl;
    // 0x80002FC0: swc1        $f10, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f10.u32l;
    // 0x80002FC4: lwc1        $f6, 0x8($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80002FC8: lwc1        $f8, 0x0($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80002FCC: lwc1        $f18, 0x18($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X18);
    // 0x80002FD0: lwc1        $f4, 0x4($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80002FD4: mul.s       $f16, $f8, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f6.fl);
    // 0x80002FD8: lwc1        $f6, 0x28($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X28);
    // 0x80002FDC: mul.s       $f10, $f4, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x80002FE0: lwc1        $f4, 0x8($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80002FE4: mul.s       $f18, $f6, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f18.fl = MUL_S(ctx->f6.fl, ctx->f4.fl);
    // 0x80002FE8: add.s       $f8, $f16, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f16.fl + ctx->f10.fl;
    // 0x80002FEC: add.s       $f16, $f18, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = ctx->f18.fl + ctx->f8.fl;
    // 0x80002FF0: jr          $ra
    // 0x80002FF4: swc1        $f16, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f16.u32l;
    return;
    // 0x80002FF4: swc1        $f16, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f16.u32l;
;}
RECOMP_FUNC void func_80002FF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80002FF8: lwc1        $f4, 0x0($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80002FFC: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x80003000: lwc1        $f10, 0x4($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80003004: lwc1        $f16, 0x10($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X10);
    // 0x80003008: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x8000300C: lwc1        $f6, 0x8($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80003010: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80003014: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80003018: lwc1        $f10, 0x20($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X20);
    // 0x8000301C: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80003020: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x80003024: lwc1        $f18, 0x30($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X30);
    // 0x80003028: add.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x8000302C: add.s       $f6, $f18, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f8.fl;
    // 0x80003030: swc1        $f6, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->f6.u32l;
    // 0x80003034: lwc1        $f10, 0x0($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80003038: lwc1        $f4, 0x4($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X4);
    // 0x8000303C: lwc1        $f18, 0x4($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X4);
    // 0x80003040: lwc1        $f8, 0x14($a1)
    ctx->f8.u32l = MEM_W(ctx->r5, 0X14);
    // 0x80003044: mul.s       $f16, $f10, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x80003048: lwc1        $f4, 0x8($a0)
    ctx->f4.u32l = MEM_W(ctx->r4, 0X8);
    // 0x8000304C: mul.s       $f6, $f18, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f8.fl);
    // 0x80003050: lwc1        $f18, 0x24($a1)
    ctx->f18.u32l = MEM_W(ctx->r5, 0X24);
    // 0x80003054: mul.s       $f8, $f4, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x80003058: add.s       $f10, $f16, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f16.fl + ctx->f6.fl;
    // 0x8000305C: lwc1        $f6, 0x34($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X34);
    // 0x80003060: add.s       $f16, $f10, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x80003064: add.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f16.fl;
    // 0x80003068: swc1        $f4, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f4.u32l;
    // 0x8000306C: lwc1        $f10, 0x8($a1)
    ctx->f10.u32l = MEM_W(ctx->r5, 0X8);
    // 0x80003070: lwc1        $f18, 0x0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80003074: lwc1        $f16, 0x18($a1)
    ctx->f16.u32l = MEM_W(ctx->r5, 0X18);
    // 0x80003078: lwc1        $f6, 0x4($a0)
    ctx->f6.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8000307C: mul.s       $f8, $f18, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f10.fl);
    // 0x80003080: lwc1        $f10, 0x8($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80003084: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80003088: lwc1        $f6, 0x28($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X28);
    // 0x8000308C: mul.s       $f16, $f10, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x80003090: add.s       $f18, $f8, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f18.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80003094: lwc1        $f4, 0x38($a1)
    ctx->f4.u32l = MEM_W(ctx->r5, 0X38);
    // 0x80003098: add.s       $f8, $f18, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f18.fl + ctx->f16.fl;
    // 0x8000309C: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x800030A0: jr          $ra
    // 0x800030A4: swc1        $f10, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f10.u32l;
    return;
    // 0x800030A4: swc1        $f10, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f10.u32l;
;}
RECOMP_FUNC void func_800030A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800030A8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800030AC: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x800030B0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x800030B4: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x800030B8: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x800030BC: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x800030C0: or          $s1, $a2, $zero
    ctx->r17 = ctx->r6 | 0;
    // 0x800030C4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800030C8: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x800030CC: addiu       $a2, $zero, 0x30
    ctx->r6 = ADD32(0, 0X30);
    // 0x800030D0: jal         0x80001090
    // 0x800030D4: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    func_80001090(rdram, ctx);
        goto after_0;
    // 0x800030D4: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    after_0:
    // 0x800030D8: lw          $a3, 0x24($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X24);
    // 0x800030DC: lwc1        $f4, 0x0($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800030E0: lwc1        $f10, 0x4($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X4);
    // 0x800030E4: lwc1        $f6, 0x0($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X0);
    // 0x800030E8: lwc1        $f16, 0x10($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X10);
    // 0x800030EC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x800030F0: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x800030F4: lwc1        $f6, 0x8($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X8);
    // 0x800030F8: addiu       $v0, $s1, 0x30
    ctx->r2 = ADD32(ctx->r17, 0X30);
    // 0x800030FC: mul.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f10.fl, ctx->f16.fl);
    // 0x80003100: lwc1        $f10, 0x20($a3)
    ctx->f10.u32l = MEM_W(ctx->r7, 0X20);
    // 0x80003104: mul.s       $f16, $f6, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80003108: add.s       $f4, $f8, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f18.fl;
    // 0x8000310C: lwc1        $f18, 0x30($a3)
    ctx->f18.u32l = MEM_W(ctx->r7, 0X30);
    // 0x80003110: add.s       $f8, $f4, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f16.fl;
    // 0x80003114: add.s       $f6, $f18, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = ctx->f18.fl + ctx->f8.fl;
    // 0x80003118: swc1        $f6, 0x30($s1)
    MEM_W(0X30, ctx->r17) = ctx->f6.u32l;
    // 0x8000311C: lwc1        $f10, 0x0($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80003120: lwc1        $f4, 0x4($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X4);
    // 0x80003124: lwc1        $f18, 0x4($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80003128: lwc1        $f8, 0x14($a3)
    ctx->f8.u32l = MEM_W(ctx->r7, 0X14);
    // 0x8000312C: mul.s       $f16, $f10, $f4
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x80003130: lwc1        $f4, 0x8($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80003134: mul.s       $f6, $f18, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f8.fl);
    // 0x80003138: lwc1        $f18, 0x24($a3)
    ctx->f18.u32l = MEM_W(ctx->r7, 0X24);
    // 0x8000313C: mul.s       $f8, $f4, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f18.fl);
    // 0x80003140: add.s       $f10, $f16, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = ctx->f16.fl + ctx->f6.fl;
    // 0x80003144: lwc1        $f6, 0x34($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X34);
    // 0x80003148: add.s       $f16, $f10, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = ctx->f10.fl + ctx->f8.fl;
    // 0x8000314C: add.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = ctx->f6.fl + ctx->f16.fl;
    // 0x80003150: swc1        $f4, 0x34($s1)
    MEM_W(0X34, ctx->r17) = ctx->f4.u32l;
    // 0x80003154: lwc1        $f18, 0x0($s0)
    ctx->f18.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80003158: lwc1        $f10, 0x8($a3)
    ctx->f10.u32l = MEM_W(ctx->r7, 0X8);
    // 0x8000315C: lwc1        $f6, 0x4($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80003160: lwc1        $f16, 0x18($a3)
    ctx->f16.u32l = MEM_W(ctx->r7, 0X18);
    // 0x80003164: mul.s       $f8, $f18, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f10.fl);
    // 0x80003168: lwc1        $f10, 0x8($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X8);
    // 0x8000316C: mul.s       $f4, $f6, $f16
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f16.fl);
    // 0x80003170: lwc1        $f6, 0x28($a3)
    ctx->f6.u32l = MEM_W(ctx->r7, 0X28);
    // 0x80003174: mul.s       $f16, $f10, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x80003178: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8000317C: add.s       $f18, $f8, $f4
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f18.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80003180: lwc1        $f4, 0x38($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X38);
    // 0x80003184: swc1        $f6, 0x3C($s1)
    MEM_W(0X3C, ctx->r17) = ctx->f6.u32l;
    // 0x80003188: add.s       $f8, $f18, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = ctx->f18.fl + ctx->f16.fl;
    // 0x8000318C: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80003190: swc1        $f10, 0x38($s1)
    MEM_W(0X38, ctx->r17) = ctx->f10.u32l;
    // 0x80003194: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80003198: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8000319C: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x800031A0: jr          $ra
    // 0x800031A4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x800031A4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_800031A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800031A8: addiu       $sp, $sp, -0x8
    ctx->r29 = ADD32(ctx->r29, -0X8);
    // 0x800031AC: sw          $s0, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r16;
    // 0x800031B0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x800031B4: addiu       $v1, $zero, 0x8
    ctx->r3 = ADD32(0, 0X8);
    // 0x800031B8: addiu       $a3, $a1, 0x8
    ctx->r7 = ADD32(ctx->r5, 0X8);
L_800031BC:
    // 0x800031BC: addu        $a0, $a2, $v1
    ctx->r4 = ADD32(ctx->r6, ctx->r3);
    // 0x800031C0: addu        $a1, $s0, $v1
    ctx->r5 = ADD32(ctx->r16, ctx->r3);
    // 0x800031C4: addiu       $a1, $a1, 0x30
    ctx->r5 = ADD32(ctx->r5, 0X30);
    // 0x800031C8: addiu       $a0, $a0, 0x30
    ctx->r4 = ADD32(ctx->r4, 0X30);
    // 0x800031CC: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
L_800031D0:
    // 0x800031D0: lwc1        $f4, 0x0($a3)
    ctx->f4.u32l = MEM_W(ctx->r7, 0X0);
    // 0x800031D4: lwc1        $f6, 0x0($a1)
    ctx->f6.u32l = MEM_W(ctx->r5, 0X0);
    // 0x800031D8: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x800031DC: addiu       $a0, $a0, -0x10
    ctx->r4 = ADD32(ctx->r4, -0X10);
    // 0x800031E0: mul.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x800031E4: addiu       $a1, $a1, -0x10
    ctx->r5 = ADD32(ctx->r5, -0X10);
    // 0x800031E8: bgez        $v0, L_800031D0
    if (SIGNED(ctx->r2) >= 0) {
        // 0x800031EC: swc1        $f8, 0x10($a0)
        MEM_W(0X10, ctx->r4) = ctx->f8.u32l;
            goto L_800031D0;
    }
    // 0x800031EC: swc1        $f8, 0x10($a0)
    MEM_W(0X10, ctx->r4) = ctx->f8.u32l;
    // 0x800031F0: addiu       $v1, $v1, -0x4
    ctx->r3 = ADD32(ctx->r3, -0X4);
    // 0x800031F4: bgez        $v1, L_800031BC
    if (SIGNED(ctx->r3) >= 0) {
        // 0x800031F8: addiu       $a3, $a3, -0x4
        ctx->r7 = ADD32(ctx->r7, -0X4);
            goto L_800031BC;
    }
    // 0x800031F8: addiu       $a3, $a3, -0x4
    ctx->r7 = ADD32(ctx->r7, -0X4);
    // 0x800031FC: addiu       $v0, $zero, 0x3
    ctx->r2 = ADD32(0, 0X3);
    // 0x80003200: addiu       $v1, $a2, 0x30
    ctx->r3 = ADD32(ctx->r6, 0X30);
    // 0x80003204: addiu       $a0, $s0, 0x30
    ctx->r4 = ADD32(ctx->r16, 0X30);
L_80003208:
    // 0x80003208: lwc1        $f10, 0xC($a0)
    ctx->f10.u32l = MEM_W(ctx->r4, 0XC);
    // 0x8000320C: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80003210: addiu       $v1, $v1, -0x10
    ctx->r3 = ADD32(ctx->r3, -0X10);
    // 0x80003214: addiu       $a0, $a0, -0x10
    ctx->r4 = ADD32(ctx->r4, -0X10);
    // 0x80003218: bgez        $v0, L_80003208
    if (SIGNED(ctx->r2) >= 0) {
        // 0x8000321C: swc1        $f10, 0x10($v1)
        MEM_W(0X10, ctx->r3) = ctx->f10.u32l;
            goto L_80003208;
    }
    // 0x8000321C: swc1        $f10, 0x10($v1)
    MEM_W(0X10, ctx->r3) = ctx->f10.u32l;
    // 0x80003220: lw          $s0, 0x4($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X4);
    // 0x80003224: jr          $ra
    // 0x80003228: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
    return;
    // 0x80003228: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
;}
RECOMP_FUNC void func_8000322C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000322C: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80003230: sw          $s0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r16;
    // 0x80003234: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80003238: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8000323C: sw          $s1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r17;
    // 0x80003240: sdc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X20, ctx->r29);
    // 0x80003244: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x80003248: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x8000324C: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x80003250: jal         0x800A3A50
    // 0x80003254: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    func_800A3A50(rdram, ctx);
        goto after_0;
    // 0x80003254: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    after_0:
    // 0x80003258: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x8000325C: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80003260: lwc1        $f24, -0x32B0($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X32B0);
    // 0x80003264: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80003268: lhu         $a0, 0x0($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X0);
    // 0x8000326C: jal         0x80097330
    // 0x80003270: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    func_80097330(rdram, ctx);
        goto after_1;
    // 0x80003270: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    after_1:
    // 0x80003274: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x80003278: lhu         $a0, 0x2($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X2);
    // 0x8000327C: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80003280: jal         0x800A3A50
    // 0x80003284: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    func_800A3A50(rdram, ctx);
        goto after_2;
    // 0x80003284: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    after_2:
    // 0x80003288: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x8000328C: lhu         $a0, 0x2($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X2);
    // 0x80003290: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80003294: div.s       $f14, $f6, $f24
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f14.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x80003298: jal         0x80097330
    // 0x8000329C: swc1        $f14, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f14.u32l;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x8000329C: swc1        $f14, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f14.u32l;
    after_3:
    // 0x800032A0: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x800032A4: lhu         $a0, 0x4($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X4);
    // 0x800032A8: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x800032AC: div.s       $f18, $f10, $f24
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f18.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    // 0x800032B0: jal         0x800A3A50
    // 0x800032B4: swc1        $f18, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f18.u32l;
    func_800A3A50(rdram, ctx);
        goto after_4;
    // 0x800032B4: swc1        $f18, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f18.u32l;
    after_4:
    // 0x800032B8: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x800032BC: lhu         $a0, 0x4($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X4);
    // 0x800032C0: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800032C4: div.s       $f2, $f6, $f24
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x800032C8: jal         0x80097330
    // 0x800032CC: swc1        $f2, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f2.u32l;
    func_80097330(rdram, ctx);
        goto after_5;
    // 0x800032CC: swc1        $f2, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f2.u32l;
    after_5:
    // 0x800032D0: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x800032D4: lwc1        $f2, 0x44($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X44);
    // 0x800032D8: lwc1        $f14, 0x48($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X48);
    // 0x800032DC: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x800032E0: lwc1        $f18, 0x3C($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x800032E4: mul.s       $f8, $f14, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x800032E8: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x800032EC: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x800032F0: swc1        $f16, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f16.u32l;
    // 0x800032F4: swc1        $f16, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f16.u32l;
    // 0x800032F8: div.s       $f0, $f4, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = DIV_S(ctx->f4.fl, ctx->f24.fl);
    // 0x800032FC: swc1        $f8, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f8.u32l;
    // 0x80003300: swc1        $f16, 0x2C($s0)
    MEM_W(0X2C, ctx->r16) = ctx->f16.u32l;
    // 0x80003304: swc1        $f16, 0x30($s0)
    MEM_W(0X30, ctx->r16) = ctx->f16.u32l;
    // 0x80003308: neg.s       $f8, $f18
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f8.fl = -ctx->f18.fl;
    // 0x8000330C: swc1        $f16, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->f16.u32l;
    // 0x80003310: swc1        $f8, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f8.u32l;
    // 0x80003314: swc1        $f16, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->f16.u32l;
    // 0x80003318: mul.s       $f6, $f14, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x8000331C: nop

    // 0x80003320: mul.s       $f12, $f22, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f18.fl);
    // 0x80003324: swc1        $f6, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f6.u32l;
    // 0x80003328: mul.s       $f10, $f12, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x8000332C: nop

    // 0x80003330: mul.s       $f4, $f20, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f0.fl);
    // 0x80003334: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x80003338: mul.s       $f8, $f12, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x8000333C: nop

    // 0x80003340: mul.s       $f10, $f20, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x80003344: swc1        $f6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f6.u32l;
    // 0x80003348: mul.s       $f6, $f22, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f22.fl, ctx->f14.fl);
    // 0x8000334C: nop

    // 0x80003350: mul.s       $f12, $f20, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f18.fl);
    // 0x80003354: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80003358: swc1        $f6, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f6.u32l;
    // 0x8000335C: swc1        $f4, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f4.u32l;
    // 0x80003360: mul.s       $f8, $f12, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x80003364: nop

    // 0x80003368: mul.s       $f10, $f22, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f22.fl, ctx->f0.fl);
    // 0x8000336C: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80003370: mul.s       $f6, $f12, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x80003374: nop

    // 0x80003378: mul.s       $f8, $f22, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x8000337C: swc1        $f4, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f4.u32l;
    // 0x80003380: mul.s       $f4, $f20, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f14.fl);
    // 0x80003384: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80003388: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x8000338C: swc1        $f4, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f4.u32l;
    // 0x80003390: swc1        $f10, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f10.u32l;
    // 0x80003394: swc1        $f6, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f6.u32l;
    // 0x80003398: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x8000339C: lw          $s1, 0x30($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X30);
    // 0x800033A0: lw          $s0, 0x2C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X2C);
    // 0x800033A4: ldc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X20);
    // 0x800033A8: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x800033AC: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x800033B0: jr          $ra
    // 0x800033B4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x800033B4: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_800033B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800033B8: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x800033BC: sw          $s0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r16;
    // 0x800033C0: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x800033C4: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x800033C8: sw          $s1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r17;
    // 0x800033CC: sdc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X20, ctx->r29);
    // 0x800033D0: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x800033D4: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x800033D8: sw          $a2, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r6;
    // 0x800033DC: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x800033E0: jal         0x800A3A50
    // 0x800033E4: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    func_800A3A50(rdram, ctx);
        goto after_0;
    // 0x800033E4: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    after_0:
    // 0x800033E8: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x800033EC: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800033F0: lwc1        $f24, -0x32AC($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X32AC);
    // 0x800033F4: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800033F8: lhu         $a0, 0x0($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X0);
    // 0x800033FC: jal         0x80097330
    // 0x80003400: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    func_80097330(rdram, ctx);
        goto after_1;
    // 0x80003400: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    after_1:
    // 0x80003404: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x80003408: lhu         $a0, 0x2($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X2);
    // 0x8000340C: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x80003410: jal         0x800A3A50
    // 0x80003414: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    func_800A3A50(rdram, ctx);
        goto after_2;
    // 0x80003414: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    after_2:
    // 0x80003418: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x8000341C: lhu         $a0, 0x2($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X2);
    // 0x80003420: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80003424: div.s       $f14, $f6, $f24
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f14.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x80003428: jal         0x80097330
    // 0x8000342C: swc1        $f14, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f14.u32l;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x8000342C: swc1        $f14, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->f14.u32l;
    after_3:
    // 0x80003430: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x80003434: lhu         $a0, 0x4($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X4);
    // 0x80003438: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8000343C: div.s       $f16, $f10, $f24
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    // 0x80003440: jal         0x800A3A50
    // 0x80003444: swc1        $f16, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f16.u32l;
    func_800A3A50(rdram, ctx);
        goto after_4;
    // 0x80003444: swc1        $f16, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->f16.u32l;
    after_4:
    // 0x80003448: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x8000344C: lhu         $a0, 0x4($s1)
    ctx->r4 = MEM_HU(ctx->r17, 0X4);
    // 0x80003450: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80003454: div.s       $f2, $f6, $f24
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x80003458: jal         0x80097330
    // 0x8000345C: swc1        $f2, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f2.u32l;
    func_80097330(rdram, ctx);
        goto after_5;
    // 0x8000345C: swc1        $f2, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f2.u32l;
    after_5:
    // 0x80003460: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x80003464: lwc1        $f2, 0x44($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80003468: lwc1        $f14, 0x48($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X48);
    // 0x8000346C: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80003470: lwc1        $f16, 0x3C($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x80003474: mul.s       $f8, $f14, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x80003478: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x8000347C: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80003480: swc1        $f18, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f18.u32l;
    // 0x80003484: div.s       $f0, $f4, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = DIV_S(ctx->f4.fl, ctx->f24.fl);
    // 0x80003488: swc1        $f8, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f8.u32l;
    // 0x8000348C: neg.s       $f8, $f16
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f8.fl = -ctx->f16.fl;
    // 0x80003490: swc1        $f8, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f8.u32l;
    // 0x80003494: mul.s       $f6, $f14, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x80003498: nop

    // 0x8000349C: mul.s       $f12, $f22, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f16.fl);
    // 0x800034A0: swc1        $f6, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f6.u32l;
    // 0x800034A4: mul.s       $f10, $f12, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x800034A8: nop

    // 0x800034AC: mul.s       $f4, $f20, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f0.fl);
    // 0x800034B0: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x800034B4: mul.s       $f8, $f12, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x800034B8: nop

    // 0x800034BC: mul.s       $f10, $f20, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x800034C0: swc1        $f6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f6.u32l;
    // 0x800034C4: mul.s       $f6, $f22, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f22.fl, ctx->f14.fl);
    // 0x800034C8: nop

    // 0x800034CC: mul.s       $f12, $f20, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f16.fl);
    // 0x800034D0: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x800034D4: swc1        $f6, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f6.u32l;
    // 0x800034D8: swc1        $f4, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f4.u32l;
    // 0x800034DC: mul.s       $f8, $f12, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x800034E0: lw          $v0, 0x58($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X58);
    // 0x800034E4: swc1        $f18, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f18.u32l;
    // 0x800034E8: mul.s       $f10, $f22, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f22.fl, ctx->f0.fl);
    // 0x800034EC: swc1        $f18, 0x2C($s0)
    MEM_W(0X2C, ctx->r16) = ctx->f18.u32l;
    // 0x800034F0: mul.s       $f6, $f12, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x800034F4: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x800034F8: mul.s       $f8, $f22, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800034FC: swc1        $f4, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f4.u32l;
    // 0x80003500: mul.s       $f4, $f20, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f14.fl);
    // 0x80003504: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80003508: swc1        $f4, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f4.u32l;
    // 0x8000350C: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80003510: swc1        $f10, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f10.u32l;
    // 0x80003514: lwc1        $f6, 0x0($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80003518: swc1        $f6, 0x30($s0)
    MEM_W(0X30, ctx->r16) = ctx->f6.u32l;
    // 0x8000351C: lwc1        $f8, 0x4($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80003520: swc1        $f8, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->f8.u32l;
    // 0x80003524: lwc1        $f10, 0x8($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80003528: swc1        $f4, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f4.u32l;
    // 0x8000352C: swc1        $f10, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->f10.u32l;
    // 0x80003530: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80003534: lw          $s1, 0x30($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X30);
    // 0x80003538: lw          $s0, 0x2C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X2C);
    // 0x8000353C: ldc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X20);
    // 0x80003540: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x80003544: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80003548: jr          $ra
    // 0x8000354C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x8000354C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_80003550(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80003550: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x80003554: sw          $s0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r16;
    // 0x80003558: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8000355C: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80003560: sw          $s2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r18;
    // 0x80003564: sw          $s1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r17;
    // 0x80003568: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x8000356C: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80003570: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80003574: or          $s1, $a2, $zero
    ctx->r17 = ctx->r6 | 0;
    // 0x80003578: or          $s2, $a1, $zero
    ctx->r18 = ctx->r5 | 0;
    // 0x8000357C: jal         0x800A3A50
    // 0x80003580: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    func_800A3A50(rdram, ctx);
        goto after_0;
    // 0x80003580: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    after_0:
    // 0x80003584: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80003588: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x8000358C: lwc1        $f24, -0x32A8($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X32A8);
    // 0x80003590: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80003594: lhu         $a0, 0x0($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X0);
    // 0x80003598: jal         0x80097330
    // 0x8000359C: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    func_80097330(rdram, ctx);
        goto after_1;
    // 0x8000359C: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    after_1:
    // 0x800035A0: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x800035A4: lhu         $a0, 0x2($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X2);
    // 0x800035A8: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x800035AC: jal         0x800A3A50
    // 0x800035B0: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    func_800A3A50(rdram, ctx);
        goto after_2;
    // 0x800035B0: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    after_2:
    // 0x800035B4: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x800035B8: lhu         $a0, 0x2($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X2);
    // 0x800035BC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800035C0: div.s       $f14, $f6, $f24
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f14.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x800035C4: jal         0x80097330
    // 0x800035C8: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x800035C8: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    after_3:
    // 0x800035CC: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x800035D0: lhu         $a0, 0x4($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X4);
    // 0x800035D4: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x800035D8: div.s       $f18, $f10, $f24
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f18.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    // 0x800035DC: jal         0x800A3A50
    // 0x800035E0: swc1        $f18, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f18.u32l;
    func_800A3A50(rdram, ctx);
        goto after_4;
    // 0x800035E0: swc1        $f18, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f18.u32l;
    after_4:
    // 0x800035E4: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x800035E8: lhu         $a0, 0x4($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X4);
    // 0x800035EC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800035F0: div.s       $f2, $f6, $f24
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x800035F4: jal         0x80097330
    // 0x800035F8: swc1        $f2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f2.u32l;
    func_80097330(rdram, ctx);
        goto after_5;
    // 0x800035F8: swc1        $f2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f2.u32l;
    after_5:
    // 0x800035FC: lwc1        $f2, 0x4C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80003600: lwc1        $f14, 0x50($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X50);
    // 0x80003604: lwc1        $f8, 0x0($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X0);
    // 0x80003608: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x8000360C: mul.s       $f10, $f14, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x80003610: lwc1        $f18, 0x44($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80003614: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80003618: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8000361C: mul.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80003620: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80003624: swc1        $f4, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f4.u32l;
    // 0x80003628: lwc1        $f10, 0x0($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X0);
    // 0x8000362C: div.s       $f0, $f8, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f24.fl);
    // 0x80003630: mul.s       $f4, $f14, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x80003634: nop

    // 0x80003638: mul.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8000363C: neg.s       $f10, $f18
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f10.fl = -ctx->f18.fl;
    // 0x80003640: swc1        $f6, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f6.u32l;
    // 0x80003644: lwc1        $f8, 0x0($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X0);
    // 0x80003648: swc1        $f16, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f16.u32l;
    // 0x8000364C: mul.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80003650: nop

    // 0x80003654: mul.s       $f12, $f22, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f18.fl);
    // 0x80003658: swc1        $f4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f4.u32l;
    // 0x8000365C: lwc1        $f4, 0x4($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80003660: mul.s       $f6, $f12, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x80003664: nop

    // 0x80003668: mul.s       $f8, $f20, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f20.fl, ctx->f0.fl);
    // 0x8000366C: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80003670: mul.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f10.fl);
    // 0x80003674: nop

    // 0x80003678: mul.s       $f8, $f12, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x8000367C: nop

    // 0x80003680: mul.s       $f4, $f20, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x80003684: swc1        $f6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f6.u32l;
    // 0x80003688: lwc1        $f6, 0x4($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X4);
    // 0x8000368C: add.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80003690: mul.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80003694: swc1        $f8, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f8.u32l;
    // 0x80003698: mul.s       $f6, $f22, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f22.fl, ctx->f14.fl);
    // 0x8000369C: lwc1        $f4, 0x4($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800036A0: swc1        $f16, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f16.u32l;
    // 0x800036A4: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x800036A8: nop

    // 0x800036AC: mul.s       $f12, $f20, $f18
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f18.fl);
    // 0x800036B0: swc1        $f10, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f10.u32l;
    // 0x800036B4: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800036B8: mul.s       $f8, $f12, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x800036BC: nop

    // 0x800036C0: mul.s       $f4, $f22, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f22.fl, ctx->f0.fl);
    // 0x800036C4: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x800036C8: mul.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x800036CC: nop

    // 0x800036D0: mul.s       $f4, $f12, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x800036D4: nop

    // 0x800036D8: mul.s       $f10, $f22, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800036DC: swc1        $f8, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f8.u32l;
    // 0x800036E0: lwc1        $f8, 0x8($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800036E4: sub.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f10.fl;
    // 0x800036E8: mul.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f6.fl);
    // 0x800036EC: swc1        $f4, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f4.u32l;
    // 0x800036F0: mul.s       $f8, $f20, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f20.fl, ctx->f14.fl);
    // 0x800036F4: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800036F8: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x800036FC: swc1        $f16, 0x2C($s0)
    MEM_W(0X2C, ctx->r16) = ctx->f16.u32l;
    // 0x80003700: swc1        $f16, 0x30($s0)
    MEM_W(0X30, ctx->r16) = ctx->f16.u32l;
    // 0x80003704: swc1        $f16, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->f16.u32l;
    // 0x80003708: swc1        $f16, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->f16.u32l;
    // 0x8000370C: mul.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f8.fl);
    // 0x80003710: swc1        $f4, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f4.u32l;
    // 0x80003714: swc1        $f6, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f6.u32l;
    // 0x80003718: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x8000371C: lw          $s2, 0x38($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X38);
    // 0x80003720: lw          $s1, 0x34($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X34);
    // 0x80003724: lw          $s0, 0x30($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X30);
    // 0x80003728: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x8000372C: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x80003730: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80003734: jr          $ra
    // 0x80003738: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x80003738: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_8000373C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000373C: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x80003740: sw          $s0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r16;
    // 0x80003744: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80003748: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8000374C: sw          $s2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r18;
    // 0x80003750: sw          $s1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r17;
    // 0x80003754: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x80003758: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x8000375C: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80003760: sw          $a2, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r6;
    // 0x80003764: or          $s1, $a3, $zero
    ctx->r17 = ctx->r7 | 0;
    // 0x80003768: or          $s2, $a1, $zero
    ctx->r18 = ctx->r5 | 0;
    // 0x8000376C: jal         0x800A3A50
    // 0x80003770: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    func_800A3A50(rdram, ctx);
        goto after_0;
    // 0x80003770: lhu         $a0, 0x0($a1)
    ctx->r4 = MEM_HU(ctx->r5, 0X0);
    after_0:
    // 0x80003774: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80003778: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x8000377C: lwc1        $f24, -0x32A4($at)
    ctx->f24.u32l = MEM_W(ctx->r1, -0X32A4);
    // 0x80003780: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80003784: lhu         $a0, 0x0($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X0);
    // 0x80003788: jal         0x80097330
    // 0x8000378C: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    func_80097330(rdram, ctx);
        goto after_1;
    // 0x8000378C: div.s       $f20, $f6, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f20.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    after_1:
    // 0x80003790: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x80003794: lhu         $a0, 0x2($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X2);
    // 0x80003798: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8000379C: jal         0x800A3A50
    // 0x800037A0: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    func_800A3A50(rdram, ctx);
        goto after_2;
    // 0x800037A0: div.s       $f22, $f10, $f24
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f22.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    after_2:
    // 0x800037A4: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x800037A8: lhu         $a0, 0x2($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X2);
    // 0x800037AC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800037B0: div.s       $f14, $f6, $f24
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f14.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x800037B4: jal         0x80097330
    // 0x800037B8: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x800037B8: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    after_3:
    // 0x800037BC: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x800037C0: lhu         $a0, 0x4($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X4);
    // 0x800037C4: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x800037C8: div.s       $f16, $f10, $f24
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f16.fl = DIV_S(ctx->f10.fl, ctx->f24.fl);
    // 0x800037CC: jal         0x800A3A50
    // 0x800037D0: swc1        $f16, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f16.u32l;
    func_800A3A50(rdram, ctx);
        goto after_4;
    // 0x800037D0: swc1        $f16, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f16.u32l;
    after_4:
    // 0x800037D4: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x800037D8: lhu         $a0, 0x4($s2)
    ctx->r4 = MEM_HU(ctx->r18, 0X4);
    // 0x800037DC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800037E0: div.s       $f2, $f6, $f24
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f2.fl = DIV_S(ctx->f6.fl, ctx->f24.fl);
    // 0x800037E4: jal         0x80097330
    // 0x800037E8: swc1        $f2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f2.u32l;
    func_80097330(rdram, ctx);
        goto after_5;
    // 0x800037E8: swc1        $f2, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f2.u32l;
    after_5:
    // 0x800037EC: lwc1        $f2, 0x4C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x800037F0: lwc1        $f14, 0x50($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X50);
    // 0x800037F4: lwc1        $f8, 0x0($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800037F8: mtc1        $v0, $f6
    ctx->f6.u32l = ctx->r2;
    // 0x800037FC: mul.s       $f10, $f14, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x80003800: lwc1        $f16, 0x44($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80003804: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80003808: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8000380C: mul.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80003810: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80003814: swc1        $f4, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f4.u32l;
    // 0x80003818: lwc1        $f10, 0x0($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X0);
    // 0x8000381C: div.s       $f0, $f8, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f0.fl = DIV_S(ctx->f8.fl, ctx->f24.fl);
    // 0x80003820: mul.s       $f4, $f14, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f0.fl);
    // 0x80003824: nop

    // 0x80003828: mul.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f4.fl);
    // 0x8000382C: neg.s       $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = -ctx->f16.fl;
    // 0x80003830: swc1        $f6, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f6.u32l;
    // 0x80003834: lwc1        $f8, 0x0($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X0);
    // 0x80003838: swc1        $f18, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f18.u32l;
    // 0x8000383C: mul.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80003840: nop

    // 0x80003844: mul.s       $f12, $f22, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f12.fl = MUL_S(ctx->f22.fl, ctx->f16.fl);
    // 0x80003848: swc1        $f4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f4.u32l;
    // 0x8000384C: lwc1        $f4, 0x4($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80003850: mul.s       $f6, $f12, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x80003854: nop

    // 0x80003858: mul.s       $f8, $f20, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f20.fl, ctx->f0.fl);
    // 0x8000385C: sub.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x80003860: mul.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f10.fl);
    // 0x80003864: nop

    // 0x80003868: mul.s       $f8, $f12, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x8000386C: nop

    // 0x80003870: mul.s       $f4, $f20, $f2
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x80003874: swc1        $f6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f6.u32l;
    // 0x80003878: lwc1        $f6, 0x4($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X4);
    // 0x8000387C: add.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x80003880: mul.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x80003884: swc1        $f8, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->f8.u32l;
    // 0x80003888: mul.s       $f6, $f22, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f22.fl, ctx->f14.fl);
    // 0x8000388C: lwc1        $f4, 0x4($s1)
    ctx->f4.u32l = MEM_W(ctx->r17, 0X4);
    // 0x80003890: swc1        $f18, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->f18.u32l;
    // 0x80003894: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80003898: nop

    // 0x8000389C: mul.s       $f12, $f20, $f16
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f12.fl = MUL_S(ctx->f20.fl, ctx->f16.fl);
    // 0x800038A0: swc1        $f10, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->f10.u32l;
    // 0x800038A4: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800038A8: mul.s       $f8, $f12, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f2.fl);
    // 0x800038AC: nop

    // 0x800038B0: mul.s       $f4, $f22, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f22.fl, ctx->f0.fl);
    // 0x800038B4: add.s       $f6, $f8, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f8.fl + ctx->f4.fl;
    // 0x800038B8: mul.s       $f8, $f10, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f6.fl);
    // 0x800038BC: nop

    // 0x800038C0: mul.s       $f4, $f12, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f0.fl);
    // 0x800038C4: nop

    // 0x800038C8: mul.s       $f10, $f22, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f22.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f22.fl, ctx->f2.fl);
    // 0x800038CC: swc1        $f8, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f8.u32l;
    // 0x800038D0: lwc1        $f8, 0x8($s1)
    ctx->f8.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800038D4: sub.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = ctx->f4.fl - ctx->f10.fl;
    // 0x800038D8: mul.s       $f4, $f8, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f6.fl);
    // 0x800038DC: swc1        $f4, 0x24($s0)
    MEM_W(0X24, ctx->r16) = ctx->f4.u32l;
    // 0x800038E0: mul.s       $f8, $f20, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f20.fl, ctx->f14.fl);
    // 0x800038E4: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800038E8: mul.s       $f6, $f10, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f6.fl = MUL_S(ctx->f10.fl, ctx->f8.fl);
    // 0x800038EC: swc1        $f6, 0x28($s0)
    MEM_W(0X28, ctx->r16) = ctx->f6.u32l;
    // 0x800038F0: lw          $v0, 0x60($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X60);
    // 0x800038F4: swc1        $f18, 0x2C($s0)
    MEM_W(0X2C, ctx->r16) = ctx->f18.u32l;
    // 0x800038F8: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x800038FC: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80003900: swc1        $f4, 0x30($s0)
    MEM_W(0X30, ctx->r16) = ctx->f4.u32l;
    // 0x80003904: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80003908: swc1        $f10, 0x34($s0)
    MEM_W(0X34, ctx->r16) = ctx->f10.u32l;
    // 0x8000390C: lwc1        $f8, 0x8($v0)
    ctx->f8.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80003910: swc1        $f6, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->f6.u32l;
    // 0x80003914: swc1        $f8, 0x38($s0)
    MEM_W(0X38, ctx->r16) = ctx->f8.u32l;
    // 0x80003918: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x8000391C: lw          $s2, 0x38($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X38);
    // 0x80003920: lw          $s1, 0x34($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X34);
    // 0x80003924: lw          $s0, 0x30($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X30);
    // 0x80003928: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x8000392C: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x80003930: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80003934: jr          $ra
    // 0x80003938: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x80003938: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_8000393C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000393C: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x80003940: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80003944: sw          $a1, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = ctx->r5;
    // 0x80003948: sw          $a0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r4;
    // 0x8000394C: jal         0x8000322C
    // 0x80003950: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    func_8000322C(rdram, ctx);
        goto after_0;
    // 0x80003950: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    after_0:
    // 0x80003954: lw          $v0, 0x5C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X5C);
    // 0x80003958: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x8000395C: lw          $a1, 0x0($v0)
    ctx->r5 = MEM_W(ctx->r2, 0X0);
    // 0x80003960: lw          $a2, 0x4($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X4);
    // 0x80003964: jal         0x800A17E0
    // 0x80003968: lw          $a3, 0x8($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X8);
    guTranslateF(rdram, ctx);
        goto after_1;
    // 0x80003968: lw          $a3, 0x8($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X8);
    after_1:
    // 0x8000396C: lw          $a1, 0x58($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X58);
    // 0x80003970: addiu       $a0, $sp, 0x18
    ctx->r4 = ADD32(ctx->r29, 0X18);
    // 0x80003974: jal         0x8000CC18
    // 0x80003978: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    func_8000CC18(rdram, ctx);
        goto after_2;
    // 0x80003978: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    after_2:
    // 0x8000397C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80003980: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    // 0x80003984: jr          $ra
    // 0x80003988: nop

    return;
    // 0x80003988: nop

;}
RECOMP_FUNC void func_8000398C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000398C: addiu       $sp, $sp, -0x98
    ctx->r29 = ADD32(ctx->r29, -0X98);
    // 0x80003990: sw          $s3, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r19;
    // 0x80003994: or          $s3, $a2, $zero
    ctx->r19 = ctx->r6 | 0;
    // 0x80003998: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x8000399C: sw          $s2, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r18;
    // 0x800039A0: sw          $a1, 0x9C($sp)
    MEM_W(0X9C, ctx->r29) = ctx->r5;
    // 0x800039A4: or          $s2, $a3, $zero
    ctx->r18 = ctx->r7 | 0;
    // 0x800039A8: sw          $s1, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r17;
    // 0x800039AC: sw          $s0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r16;
    // 0x800039B0: sdc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X20, ctx->r29);
    // 0x800039B4: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x800039B8: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x800039BC: addiu       $a1, $sp, 0x58
    ctx->r5 = ADD32(ctx->r29, 0X58);
    // 0x800039C0: jal         0x80001090
    // 0x800039C4: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    func_80001090(rdram, ctx);
        goto after_0;
    // 0x800039C4: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_0:
    // 0x800039C8: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x800039CC: or          $s1, $s2, $zero
    ctx->r17 = ctx->r18 | 0;
    // 0x800039D0: mtc1        $at, $f23
    ctx->f_odd[(23 - 1) * 2] = ctx->r1;
    // 0x800039D4: mtc1        $zero, $f24
    ctx->f24.u32l = 0;
    // 0x800039D8: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x800039DC: addiu       $s2, $sp, 0x64
    ctx->r18 = ADD32(ctx->r29, 0X64);
    // 0x800039E0: addiu       $s0, $sp, 0x58
    ctx->r16 = ADD32(ctx->r29, 0X58);
L_800039E4:
    // 0x800039E4: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x800039E8: mul.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x800039EC: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x800039F0: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x800039F4: add.s       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f0.fl;
    // 0x800039F8: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x800039FC: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003A00: jal         0x800A01E0
    // 0x80003A04: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80003A04: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    after_1:
    // 0x80003A08: c.eq.s      $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    c1cs = ctx->f0.fl == ctx->f24.fl;
    // 0x80003A0C: swc1        $f0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->f0.u32l;
    // 0x80003A10: bc1tl       L_80003A54
    if (c1cs) {
        // 0x80003A14: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80003A54;
    }
    goto skip_0;
    // 0x80003A14: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    skip_0:
    // 0x80003A18: cvt.d.s     $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.d = CVT_D_S(ctx->f0.fl);
    // 0x80003A1C: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80003A20: div.d       $f6, $f22, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f22.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = DIV_D(ctx->f22.d, ctx->f4.d);
    // 0x80003A24: lwc1        $f16, 0x10($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X10);
    // 0x80003A28: lwc1        $f4, 0x20($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X20);
    // 0x80003A2C: cvt.s.d     $f20, $f6
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f20.fl = CVT_S_D(ctx->f6.d);
    // 0x80003A30: mul.s       $f10, $f8, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f20.fl);
    // 0x80003A34: nop

    // 0x80003A38: mul.s       $f18, $f16, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x80003A3C: nop

    // 0x80003A40: mul.s       $f6, $f4, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f20.fl);
    // 0x80003A44: swc1        $f10, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f10.u32l;
    // 0x80003A48: swc1        $f18, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f18.u32l;
    // 0x80003A4C: swc1        $f6, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f6.u32l;
    // 0x80003A50: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80003A54:
    // 0x80003A54: bne         $s0, $s2, L_800039E4
    if (ctx->r16 != ctx->r18) {
        // 0x80003A58: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_800039E4;
    }
    // 0x80003A58: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x80003A5C: lwc1        $f0, 0x58($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80003A60: mul.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003A64: lwc1        $f0, 0x5C($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80003A68: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003A6C: jal         0x800A01E0
    // 0x80003A70: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_2;
    // 0x80003A70: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    after_2:
    // 0x80003A74: lwc1        $f12, 0x60($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X60);
    // 0x80003A78: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
    // 0x80003A7C: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80003A80: jal         0x80004C68
    // 0x80003A84: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_80004C68(rdram, ctx);
        goto after_3;
    // 0x80003A84: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_3:
    // 0x80003A88: c.eq.s      $f20, $f24
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 24);
    c1cs = ctx->f20.fl == ctx->f24.fl;
    // 0x80003A8C: sh          $v0, 0x2($s3)
    MEM_H(0X2, ctx->r19) = ctx->r2;
    // 0x80003A90: bc1tl       L_80003AEC
    if (c1cs) {
        // 0x80003A94: sh          $zero, 0x0($s3)
        MEM_H(0X0, ctx->r19) = 0;
            goto L_80003AEC;
    }
    goto skip_1;
    // 0x80003A94: sh          $zero, 0x0($s3)
    MEM_H(0X0, ctx->r19) = 0;
    skip_1:
    // 0x80003A98: cvt.d.s     $f8, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f8.d = CVT_D_S(ctx->f20.fl);
    // 0x80003A9C: lwc1        $f16, 0x70($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X70);
    // 0x80003AA0: div.d       $f10, $f22, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f22.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = DIV_D(ctx->f22.d, ctx->f8.d);
    // 0x80003AA4: lwc1        $f18, 0x80($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X80);
    // 0x80003AA8: cvt.s.d     $f20, $f10
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f20.fl = CVT_S_D(ctx->f10.d);
    // 0x80003AAC: mul.s       $f12, $f16, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x80003AB0: nop

    // 0x80003AB4: mul.s       $f14, $f18, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f14.fl = MUL_S(ctx->f18.fl, ctx->f20.fl);
    // 0x80003AB8: jal         0x80004C68
    // 0x80003ABC: nop

    func_80004C68(rdram, ctx);
        goto after_4;
    // 0x80003ABC: nop

    after_4:
    // 0x80003AC0: sh          $v0, 0x0($s3)
    MEM_H(0X0, ctx->r19) = ctx->r2;
    // 0x80003AC4: lwc1        $f4, 0x5C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X5C);
    // 0x80003AC8: lwc1        $f6, 0x58($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80003ACC: mul.s       $f12, $f4, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f12.fl = MUL_S(ctx->f4.fl, ctx->f20.fl);
    // 0x80003AD0: nop

    // 0x80003AD4: mul.s       $f14, $f6, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f14.fl = MUL_S(ctx->f6.fl, ctx->f20.fl);
    // 0x80003AD8: jal         0x80004C68
    // 0x80003ADC: nop

    func_80004C68(rdram, ctx);
        goto after_5;
    // 0x80003ADC: nop

    after_5:
    // 0x80003AE0: b           L_80003B00
    // 0x80003AE4: sh          $v0, 0x4($s3)
    MEM_H(0X4, ctx->r19) = ctx->r2;
        goto L_80003B00;
    // 0x80003AE4: sh          $v0, 0x4($s3)
    MEM_H(0X4, ctx->r19) = ctx->r2;
    // 0x80003AE8: sh          $zero, 0x0($s3)
    MEM_H(0X0, ctx->r19) = 0;
L_80003AEC:
    // 0x80003AEC: lwc1        $f12, 0x68($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80003AF0: lwc1        $f14, 0x6C($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X6C);
    // 0x80003AF4: jal         0x80004C68
    // 0x80003AF8: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_80004C68(rdram, ctx);
        goto after_6;
    // 0x80003AF8: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_6:
    // 0x80003AFC: sh          $v0, 0x4($s3)
    MEM_H(0X4, ctx->r19) = ctx->r2;
L_80003B00:
    // 0x80003B00: lw          $v0, 0x9C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X9C);
    // 0x80003B04: lwc1        $f8, 0x88($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X88);
    // 0x80003B08: swc1        $f8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f8.u32l;
    // 0x80003B0C: lwc1        $f10, 0x8C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X8C);
    // 0x80003B10: swc1        $f10, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f10.u32l;
    // 0x80003B14: lwc1        $f16, 0x90($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X90);
    // 0x80003B18: swc1        $f16, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f16.u32l;
    // 0x80003B1C: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80003B20: lw          $s3, 0x38($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X38);
    // 0x80003B24: lw          $s2, 0x34($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X34);
    // 0x80003B28: lw          $s1, 0x30($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X30);
    // 0x80003B2C: lw          $s0, 0x2C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X2C);
    // 0x80003B30: ldc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X20);
    // 0x80003B34: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x80003B38: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80003B3C: jr          $ra
    // 0x80003B40: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
    return;
    // 0x80003B40: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
;}
RECOMP_FUNC void func_80003B44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80003B44: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x80003B48: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80003B4C: sw          $a1, 0x84($sp)
    MEM_W(0X84, ctx->r29) = ctx->r5;
    // 0x80003B50: sw          $a2, 0x88($sp)
    MEM_W(0X88, ctx->r29) = ctx->r6;
    // 0x80003B54: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80003B58: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    // 0x80003B5C: jal         0x80001090
    // 0x80003B60: addiu       $a1, $sp, 0x40
    ctx->r5 = ADD32(ctx->r29, 0X40);
    func_80001090(rdram, ctx);
        goto after_0;
    // 0x80003B60: addiu       $a1, $sp, 0x40
    ctx->r5 = ADD32(ctx->r29, 0X40);
    after_0:
    // 0x80003B64: lwc1        $f0, 0x40($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80003B68: mul.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003B6C: lwc1        $f0, 0x44($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80003B70: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003B74: jal         0x800A01E0
    // 0x80003B78: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80003B78: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    after_1:
    // 0x80003B7C: mov.s       $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    ctx->f20.fl = ctx->f0.fl;
    // 0x80003B80: lwc1        $f12, 0x48($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80003B84: jal         0x80004C68
    // 0x80003B88: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
    func_80004C68(rdram, ctx);
        goto after_2;
    // 0x80003B88: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
    after_2:
    // 0x80003B8C: negu        $v1, $v0
    ctx->r3 = SUB32(0, ctx->r2);
    // 0x80003B90: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80003B94: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80003B98: slti        $at, $v1, -0x4000
    ctx->r1 = SIGNED(ctx->r3) < -0X4000 ? 1 : 0;
    // 0x80003B9C: bne         $at, $zero, L_80003BB0
    if (ctx->r1 != 0) {
        // 0x80003BA0: lwc1        $f12, 0x48($sp)
        ctx->f12.u32l = MEM_W(ctx->r29, 0X48);
            goto L_80003BB0;
    }
    // 0x80003BA0: lwc1        $f12, 0x48($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X48);
    // 0x80003BA4: slti        $at, $v1, 0x4000
    ctx->r1 = SIGNED(ctx->r3) < 0X4000 ? 1 : 0;
    // 0x80003BA8: bne         $at, $zero, L_80003BBC
    if (ctx->r1 != 0) {
        // 0x80003BAC: or          $v1, $zero, $zero
        ctx->r3 = 0 | 0;
            goto L_80003BBC;
    }
    // 0x80003BAC: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80003BB0:
    // 0x80003BB0: neg.s       $f20, $f20
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); 
    ctx->f20.fl = -ctx->f20.fl;
    // 0x80003BB4: b           L_80003BBC
    // 0x80003BB8: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
        goto L_80003BBC;
    // 0x80003BB8: addiu       $v1, $zero, -0x1
    ctx->r3 = ADD32(0, -0X1);
L_80003BBC:
    // 0x80003BBC: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    // 0x80003BC0: mov.s       $f14, $f20
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 20);
    ctx->f14.fl = ctx->f20.fl;
    // 0x80003BC4: jal         0x80004C68
    // 0x80003BC8: sh          $v1, 0x30($sp)
    MEM_H(0X30, ctx->r29) = ctx->r3;
    func_80004C68(rdram, ctx);
        goto after_3;
    // 0x80003BC8: sh          $v1, 0x30($sp)
    MEM_H(0X30, ctx->r29) = ctx->r3;
    after_3:
    // 0x80003BCC: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80003BD0: lw          $t6, 0x88($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X88);
    // 0x80003BD4: lh          $v1, 0x30($sp)
    ctx->r3 = MEM_H(ctx->r29, 0X30);
    // 0x80003BD8: c.eq.s      $f20, $f4
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f20.fl == ctx->f4.fl;
    // 0x80003BDC: sh          $v0, 0x2($t6)
    MEM_H(0X2, ctx->r14) = ctx->r2;
    // 0x80003BE0: lwc1        $f14, 0x54($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X54);
    // 0x80003BE4: lwc1        $f12, 0x64($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X64);
    // 0x80003BE8: bc1t        L_80003C68
    if (c1cs) {
        // 0x80003BEC: nop
    
            goto L_80003C68;
    }
    // 0x80003BEC: nop

    // 0x80003BF0: beq         $v1, $zero, L_80003C04
    if (ctx->r3 == 0) {
        // 0x80003BF4: lwc1        $f12, 0x58($sp)
        ctx->f12.u32l = MEM_W(ctx->r29, 0X58);
            goto L_80003C04;
    }
    // 0x80003BF4: lwc1        $f12, 0x58($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80003BF8: lwc1        $f12, 0x58($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80003BFC: b           L_80003C04
    // 0x80003C00: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
        goto L_80003C04;
    // 0x80003C00: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
L_80003C04:
    // 0x80003C04: beq         $v1, $zero, L_80003C18
    if (ctx->r3 == 0) {
        // 0x80003C08: lwc1        $f14, 0x68($sp)
        ctx->f14.u32l = MEM_W(ctx->r29, 0X68);
            goto L_80003C18;
    }
    // 0x80003C08: lwc1        $f14, 0x68($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80003C0C: lwc1        $f14, 0x68($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X68);
    // 0x80003C10: b           L_80003C18
    // 0x80003C14: neg.s       $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = -ctx->f14.fl;
        goto L_80003C18;
    // 0x80003C14: neg.s       $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = -ctx->f14.fl;
L_80003C18:
    // 0x80003C18: jal         0x80004C68
    // 0x80003C1C: sh          $v1, 0x30($sp)
    MEM_H(0X30, ctx->r29) = ctx->r3;
    func_80004C68(rdram, ctx);
        goto after_4;
    // 0x80003C1C: sh          $v1, 0x30($sp)
    MEM_H(0X30, ctx->r29) = ctx->r3;
    after_4:
    // 0x80003C20: lh          $v1, 0x30($sp)
    ctx->r3 = MEM_H(ctx->r29, 0X30);
    // 0x80003C24: lw          $t7, 0x88($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X88);
    // 0x80003C28: beq         $v1, $zero, L_80003C3C
    if (ctx->r3 == 0) {
        // 0x80003C2C: sh          $v0, 0x0($t7)
        MEM_H(0X0, ctx->r15) = ctx->r2;
            goto L_80003C3C;
    }
    // 0x80003C2C: sh          $v0, 0x0($t7)
    MEM_H(0X0, ctx->r15) = ctx->r2;
    // 0x80003C30: lwc1        $f12, 0x44($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X44);
    // 0x80003C34: b           L_80003C40
    // 0x80003C38: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
        goto L_80003C40;
    // 0x80003C38: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
L_80003C3C:
    // 0x80003C3C: lwc1        $f12, 0x44($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X44);
L_80003C40:
    // 0x80003C40: beq         $v1, $zero, L_80003C54
    if (ctx->r3 == 0) {
        // 0x80003C44: lwc1        $f14, 0x40($sp)
        ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
            goto L_80003C54;
    }
    // 0x80003C44: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80003C48: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80003C4C: b           L_80003C54
    // 0x80003C50: neg.s       $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = -ctx->f14.fl;
        goto L_80003C54;
    // 0x80003C50: neg.s       $f14, $f14
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f14.fl = -ctx->f14.fl;
L_80003C54:
    // 0x80003C54: jal         0x80004C68
    // 0x80003C58: nop

    func_80004C68(rdram, ctx);
        goto after_5;
    // 0x80003C58: nop

    after_5:
    // 0x80003C5C: lw          $t8, 0x88($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X88);
    // 0x80003C60: b           L_80003C80
    // 0x80003C64: sh          $v0, 0x4($t8)
    MEM_H(0X4, ctx->r24) = ctx->r2;
        goto L_80003C80;
    // 0x80003C64: sh          $v0, 0x4($t8)
    MEM_H(0X4, ctx->r24) = ctx->r2;
L_80003C68:
    // 0x80003C68: jal         0x80004C68
    // 0x80003C6C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    func_80004C68(rdram, ctx);
        goto after_6;
    // 0x80003C6C: neg.s       $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = -ctx->f12.fl;
    after_6:
    // 0x80003C70: lw          $t9, 0x88($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X88);
    // 0x80003C74: sh          $v0, 0x0($t9)
    MEM_H(0X0, ctx->r25) = ctx->r2;
    // 0x80003C78: lw          $t0, 0x88($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X88);
    // 0x80003C7C: sh          $zero, 0x4($t0)
    MEM_H(0X4, ctx->r8) = 0;
L_80003C80:
    // 0x80003C80: lw          $v0, 0x84($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X84);
    // 0x80003C84: lwc1        $f6, 0x70($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X70);
    // 0x80003C88: swc1        $f6, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f6.u32l;
    // 0x80003C8C: lwc1        $f8, 0x74($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X74);
    // 0x80003C90: swc1        $f8, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f8.u32l;
    // 0x80003C94: lwc1        $f10, 0x78($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X78);
    // 0x80003C98: swc1        $f10, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f10.u32l;
    // 0x80003C9C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80003CA0: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80003CA4: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    // 0x80003CA8: jr          $ra
    // 0x80003CAC: nop

    return;
    // 0x80003CAC: nop

;}
RECOMP_FUNC void func_80003CB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80003CB0: addiu       $sp, $sp, -0x98
    ctx->r29 = ADD32(ctx->r29, -0X98);
    // 0x80003CB4: sw          $s2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r18;
    // 0x80003CB8: or          $s2, $a2, $zero
    ctx->r18 = ctx->r6 | 0;
    // 0x80003CBC: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x80003CC0: sw          $a1, 0x9C($sp)
    MEM_W(0X9C, ctx->r29) = ctx->r5;
    // 0x80003CC4: sw          $s1, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r17;
    // 0x80003CC8: sw          $s0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r16;
    // 0x80003CCC: sdc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X28, ctx->r29);
    // 0x80003CD0: sdc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X20, ctx->r29);
    // 0x80003CD4: sdc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X18, ctx->r29);
    // 0x80003CD8: addiu       $a1, $sp, 0x58
    ctx->r5 = ADD32(ctx->r29, 0X58);
    // 0x80003CDC: jal         0x80001090
    // 0x80003CE0: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    func_80001090(rdram, ctx);
        goto after_0;
    // 0x80003CE0: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_0:
    // 0x80003CE4: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80003CE8: or          $s1, $s2, $zero
    ctx->r17 = ctx->r18 | 0;
    // 0x80003CEC: mtc1        $at, $f23
    ctx->f_odd[(23 - 1) * 2] = ctx->r1;
    // 0x80003CF0: mtc1        $zero, $f24
    ctx->f24.u32l = 0;
    // 0x80003CF4: mtc1        $zero, $f22
    ctx->f22.u32l = 0;
    // 0x80003CF8: addiu       $s2, $sp, 0x64
    ctx->r18 = ADD32(ctx->r29, 0X64);
    // 0x80003CFC: addiu       $s0, $sp, 0x58
    ctx->r16 = ADD32(ctx->r29, 0X58);
L_80003D00:
    // 0x80003D00: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80003D04: mul.s       $f20, $f0, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003D08: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x80003D0C: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003D10: add.s       $f20, $f20, $f0
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f20.fl = ctx->f20.fl + ctx->f0.fl;
    // 0x80003D14: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x80003D18: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003D1C: jal         0x800A01E0
    // 0x80003D20: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80003D20: add.s       $f12, $f20, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f20.fl + ctx->f0.fl;
    after_1:
    // 0x80003D24: c.eq.s      $f0, $f24
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    c1cs = ctx->f0.fl == ctx->f24.fl;
    // 0x80003D28: swc1        $f0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->f0.u32l;
    // 0x80003D2C: bc1tl       L_80003D70
    if (c1cs) {
        // 0x80003D30: addiu       $s0, $s0, 0x4
        ctx->r16 = ADD32(ctx->r16, 0X4);
            goto L_80003D70;
    }
    goto skip_0;
    // 0x80003D30: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    skip_0:
    // 0x80003D34: cvt.d.s     $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.d = CVT_D_S(ctx->f0.fl);
    // 0x80003D38: lwc1        $f8, 0x0($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80003D3C: div.d       $f6, $f22, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f22.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = DIV_D(ctx->f22.d, ctx->f4.d);
    // 0x80003D40: lwc1        $f16, 0x10($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X10);
    // 0x80003D44: lwc1        $f4, 0x20($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X20);
    // 0x80003D48: cvt.s.d     $f20, $f6
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f20.fl = CVT_S_D(ctx->f6.d);
    // 0x80003D4C: mul.s       $f10, $f8, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f20.fl);
    // 0x80003D50: nop

    // 0x80003D54: mul.s       $f18, $f16, $f20
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x80003D58: nop

    // 0x80003D5C: mul.s       $f6, $f4, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f20.fl);
    // 0x80003D60: swc1        $f10, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->f10.u32l;
    // 0x80003D64: swc1        $f18, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->f18.u32l;
    // 0x80003D68: swc1        $f6, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->f6.u32l;
    // 0x80003D6C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
L_80003D70:
    // 0x80003D70: bne         $s0, $s2, L_80003D00
    if (ctx->r16 != ctx->r18) {
        // 0x80003D74: addiu       $s1, $s1, 0x4
        ctx->r17 = ADD32(ctx->r17, 0X4);
            goto L_80003D00;
    }
    // 0x80003D74: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x80003D78: lw          $v0, 0x9C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X9C);
    // 0x80003D7C: lwc1        $f8, 0x88($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X88);
    // 0x80003D80: swc1        $f8, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->f8.u32l;
    // 0x80003D84: lwc1        $f10, 0x8C($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X8C);
    // 0x80003D88: swc1        $f10, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->f10.u32l;
    // 0x80003D8C: lwc1        $f16, 0x90($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X90);
    // 0x80003D90: swc1        $f16, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->f16.u32l;
    // 0x80003D94: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80003D98: lw          $s2, 0x38($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X38);
    // 0x80003D9C: lw          $s1, 0x34($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X34);
    // 0x80003DA0: lw          $s0, 0x30($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X30);
    // 0x80003DA4: ldc1        $f24, 0x28($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X28);
    // 0x80003DA8: ldc1        $f22, 0x20($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X20);
    // 0x80003DAC: ldc1        $f20, 0x18($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X18);
    // 0x80003DB0: jr          $ra
    // 0x80003DB4: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
    return;
    // 0x80003DB4: addiu       $sp, $sp, 0x98
    ctx->r29 = ADD32(ctx->r29, 0X98);
;}
RECOMP_FUNC void func_80003DB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80003DB8: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x80003DBC: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80003DC0: or          $s2, $a1, $zero
    ctx->r18 = ctx->r5 | 0;
    // 0x80003DC4: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80003DC8: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80003DCC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80003DD0: addiu       $a1, $sp, 0x40
    ctx->r5 = ADD32(ctx->r29, 0X40);
    // 0x80003DD4: jal         0x80001090
    // 0x80003DD8: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    func_80001090(rdram, ctx);
        goto after_0;
    // 0x80003DD8: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    after_0:
    // 0x80003DDC: or          $s1, $s2, $zero
    ctx->r17 = ctx->r18 | 0;
    // 0x80003DE0: addiu       $s2, $sp, 0x4C
    ctx->r18 = ADD32(ctx->r29, 0X4C);
    // 0x80003DE4: addiu       $s0, $sp, 0x40
    ctx->r16 = ADD32(ctx->r29, 0X40);
L_80003DE8:
    // 0x80003DE8: lwc1        $f0, 0x0($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X0);
    // 0x80003DEC: mul.s       $f12, $f0, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003DF0: lwc1        $f0, 0x10($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X10);
    // 0x80003DF4: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003DF8: add.s       $f12, $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f12.fl + ctx->f0.fl;
    // 0x80003DFC: lwc1        $f0, 0x20($s0)
    ctx->f0.u32l = MEM_W(ctx->r16, 0X20);
    // 0x80003E00: mul.s       $f0, $f0, $f0
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80003E04: jal         0x800A01E0
    // 0x80003E08: add.s       $f12, $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f12.fl + ctx->f0.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80003E08: add.s       $f12, $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f12.fl = ctx->f12.fl + ctx->f0.fl;
    after_1:
    // 0x80003E0C: addiu       $s0, $s0, 0x4
    ctx->r16 = ADD32(ctx->r16, 0X4);
    // 0x80003E10: addiu       $s1, $s1, 0x4
    ctx->r17 = ADD32(ctx->r17, 0X4);
    // 0x80003E14: bne         $s0, $s2, L_80003DE8
    if (ctx->r16 != ctx->r18) {
        // 0x80003E18: swc1        $f0, -0x4($s1)
        MEM_W(-0X4, ctx->r17) = ctx->f0.u32l;
            goto L_80003DE8;
    }
    // 0x80003E18: swc1        $f0, -0x4($s1)
    MEM_W(-0X4, ctx->r17) = ctx->f0.u32l;
    // 0x80003E1C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80003E20: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80003E24: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80003E28: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80003E2C: jr          $ra
    // 0x80003E30: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    return;
    // 0x80003E30: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
;}
RECOMP_FUNC void func_80003E34(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80003E34: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x80003E38: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80003E3C: sdc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X20, ctx->r29);
    // 0x80003E40: mul.s       $f4, $f14, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80003E44: mtc1        $a3, $f24
    ctx->f24.u32l = ctx->r7;
    // 0x80003E48: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80003E4C: lwc1        $f20, 0x60($sp)
    ctx->f20.u32l = MEM_W(ctx->r29, 0X60);
    // 0x80003E50: mul.s       $f6, $f24, $f24
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f6.fl = MUL_S(ctx->f24.fl, ctx->f24.fl);
    // 0x80003E54: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80003E58: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80003E5C: mul.s       $f10, $f20, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = MUL_S(ctx->f20.fl, ctx->f20.fl);
    // 0x80003E60: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x80003E64: sw          $a0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r4;
    // 0x80003E68: sw          $a1, 0x54($sp)
    MEM_W(0X54, ctx->r29) = ctx->r5;
    // 0x80003E6C: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80003E70: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80003E74: c.eq.s      $f18, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f18.fl == ctx->f12.fl;
    // 0x80003E78: nop

    // 0x80003E7C: bc1tl       L_80004064
    if (c1cs) {
        // 0x80003E80: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80004064;
    }
    goto skip_0;
    // 0x80003E80: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_0:
    // 0x80003E84: sw          $a0, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r4;
    // 0x80003E88: jal         0x800A01E0
    // 0x80003E8C: swc1        $f14, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f14.u32l;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x80003E8C: swc1        $f14, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f14.u32l;
    after_0:
    // 0x80003E90: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80003E94: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x80003E98: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80003E9C: cvt.d.s     $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.d = CVT_D_S(ctx->f0.fl);
    // 0x80003EA0: lwc1        $f14, 0x58($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80003EA4: div.d       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = DIV_D(ctx->f16.d, ctx->f4.d);
    // 0x80003EA8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80003EAC: mtc1        $zero, $f5
    ctx->f_odd[(5 - 1) * 2] = 0;
    // 0x80003EB0: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80003EB4: lw          $a2, 0x50($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X50);
    // 0x80003EB8: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80003EBC: cvt.s.d     $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f2.fl = CVT_S_D(ctx->f6.d);
    // 0x80003EC0: mul.s       $f14, $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x80003EC4: nop

    // 0x80003EC8: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x80003ECC: nop

    // 0x80003ED0: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x80003ED4: nop

    // 0x80003ED8: mul.s       $f8, $f14, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80003EDC: nop

    // 0x80003EE0: mul.s       $f10, $f20, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = MUL_S(ctx->f20.fl, ctx->f20.fl);
    // 0x80003EE4: add.s       $f22, $f8, $f10
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f22.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80003EE8: cvt.d.s     $f6, $f22
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f22.fl); 
    ctx->f6.d = CVT_D_S(ctx->f22.fl);
    // 0x80003EEC: c.eq.d      $f4, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f4.d == ctx->f6.d;
    // 0x80003EF0: nop

    // 0x80003EF4: bc1f        L_80003F00
    if (!c1cs) {
        // 0x80003EF8: nop
    
            goto L_80003F00;
    }
    // 0x80003EF8: nop

    // 0x80003EFC: lwc1        $f22, -0x32A0($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0X32A0);
L_80003F00:
    // 0x80003F00: c.eq.s      $f22, $f18
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 18);
    c1cs = ctx->f22.fl == ctx->f18.fl;
    // 0x80003F04: nop

    // 0x80003F08: bc1tl       L_80004064
    if (c1cs) {
        // 0x80003F0C: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80004064;
    }
    goto skip_1;
    // 0x80003F0C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_1:
    // 0x80003F10: mov.s       $f12, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    ctx->f12.fl = ctx->f22.fl;
    // 0x80003F14: sw          $a2, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->r6;
    // 0x80003F18: jal         0x800A01E0
    // 0x80003F1C: swc1        $f14, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f14.u32l;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80003F1C: swc1        $f14, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->f14.u32l;
    after_1:
    // 0x80003F20: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80003F24: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x80003F28: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80003F2C: cvt.d.s     $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f8.d = CVT_D_S(ctx->f0.fl);
    // 0x80003F30: lhu         $a0, 0x56($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X56);
    // 0x80003F34: div.d       $f10, $f16, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = DIV_D(ctx->f16.d, ctx->f8.d);
    // 0x80003F38: mov.s       $f22, $f0
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    ctx->f22.fl = ctx->f0.fl;
    // 0x80003F3C: sw          $a0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r4;
    // 0x80003F40: cvt.s.d     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f2.fl = CVT_S_D(ctx->f10.d);
    // 0x80003F44: jal         0x800A3A50
    // 0x80003F48: swc1        $f2, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f2.u32l;
    func_800A3A50(rdram, ctx);
        goto after_2;
    // 0x80003F48: swc1        $f2, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->f2.u32l;
    after_2:
    // 0x80003F4C: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80003F50: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80003F54: lwc1        $f8, -0x329C($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X329C);
    // 0x80003F58: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80003F5C: lw          $a0, 0x38($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X38);
    // 0x80003F60: div.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80003F64: jal         0x80097330
    // 0x80003F68: swc1        $f12, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f12.u32l;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x80003F68: swc1        $f12, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f12.u32l;
    after_3:
    // 0x80003F6C: lw          $a2, 0x50($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X50);
    // 0x80003F70: mtc1        $v0, $f10
    ctx->f10.u32l = ctx->r2;
    // 0x80003F74: lwc1        $f2, 0x40($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X40);
    // 0x80003F78: lwc1        $f12, 0x4C($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80003F7C: lwc1        $f14, 0x58($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80003F80: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80003F84: cvt.s.w     $f4, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    ctx->f4.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80003F88: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80003F8C: swc1        $f24, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f24.u32l;
    // 0x80003F90: swc1        $f20, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f20.u32l;
    // 0x80003F94: swc1        $f14, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->f14.u32l;
    // 0x80003F98: swc1        $f18, 0xC($a2)
    MEM_W(0XC, ctx->r6) = ctx->f18.u32l;
    // 0x80003F9C: lwc1        $f6, -0x3298($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X3298);
    // 0x80003FA0: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
    // 0x80003FA4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80003FA8: div.s       $f16, $f4, $f6
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f16.fl = DIV_S(ctx->f4.fl, ctx->f6.fl);
    // 0x80003FAC: mul.s       $f8, $f0, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f14.fl);
    // 0x80003FB0: nop

    // 0x80003FB4: mul.s       $f10, $f8, $f24
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f24.fl);
    // 0x80003FB8: nop

    // 0x80003FBC: mul.s       $f4, $f16, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x80003FC0: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x80003FC4: mul.s       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x80003FC8: nop

    // 0x80003FCC: mul.s       $f10, $f12, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f22.fl);
    // 0x80003FD0: nop

    // 0x80003FD4: mul.s       $f4, $f16, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f14.fl);
    // 0x80003FD8: swc1        $f8, 0x10($a2)
    MEM_W(0X10, ctx->r6) = ctx->f8.u32l;
    // 0x80003FDC: mul.s       $f6, $f0, $f24
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x80003FE0: swc1        $f10, 0x14($a2)
    MEM_W(0X14, ctx->r6) = ctx->f10.u32l;
    // 0x80003FE4: swc1        $f4, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f4.u32l;
    // 0x80003FE8: lwc1        $f10, 0x34($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80003FEC: swc1        $f18, 0x1C($a2)
    MEM_W(0X1C, ctx->r6) = ctx->f18.u32l;
    // 0x80003FF0: mul.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f20.fl);
    // 0x80003FF4: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80003FF8: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80003FFC: swc1        $f6, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->f6.u32l;
    // 0x80004000: lwc1        $f8, 0x34($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80004004: swc1        $f18, 0x2C($a2)
    MEM_W(0X2C, ctx->r6) = ctx->f18.u32l;
    // 0x80004008: swc1        $f18, 0x30($a2)
    MEM_W(0X30, ctx->r6) = ctx->f18.u32l;
    // 0x8000400C: mul.s       $f10, $f8, $f24
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f24.fl);
    // 0x80004010: swc1        $f18, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f18.u32l;
    // 0x80004014: swc1        $f18, 0x38($a2)
    MEM_W(0X38, ctx->r6) = ctx->f18.u32l;
    // 0x80004018: mul.s       $f4, $f12, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f20.fl);
    // 0x8000401C: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x80004020: neg.s       $f10, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = -ctx->f16.fl;
    // 0x80004024: mul.s       $f8, $f6, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f2.fl);
    // 0x80004028: nop

    // 0x8000402C: mul.s       $f4, $f10, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f22.fl);
    // 0x80004030: nop

    // 0x80004034: mul.s       $f6, $f16, $f24
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f24.fl);
    // 0x80004038: swc1        $f8, 0x20($a2)
    MEM_W(0X20, ctx->r6) = ctx->f8.u32l;
    // 0x8000403C: swc1        $f4, 0x24($a2)
    MEM_W(0X24, ctx->r6) = ctx->f4.u32l;
    // 0x80004040: mul.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f20.fl);
    // 0x80004044: nop

    // 0x80004048: mul.s       $f10, $f12, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f14.fl);
    // 0x8000404C: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x80004050: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x80004054: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80004058: swc1        $f8, 0x3C($a2)
    MEM_W(0X3C, ctx->r6) = ctx->f8.u32l;
    // 0x8000405C: swc1        $f6, 0x28($a2)
    MEM_W(0X28, ctx->r6) = ctx->f6.u32l;
    // 0x80004060: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80004064:
    // 0x80004064: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80004068: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x8000406C: ldc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X20);
    // 0x80004070: jr          $ra
    // 0x80004074: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x80004074: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_80004078(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004078: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x8000407C: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x80004080: sdc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    SD(ctx->f24.u64, 0X20, ctx->r29);
    // 0x80004084: mul.s       $f4, $f14, $f14
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x80004088: mtc1        $a3, $f24
    ctx->f24.u32l = ctx->r7;
    // 0x8000408C: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80004090: lwc1        $f20, 0x58($sp)
    ctx->f20.u32l = MEM_W(ctx->r29, 0X58);
    // 0x80004094: mul.s       $f6, $f24, $f24
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f6.fl = MUL_S(ctx->f24.fl, ctx->f24.fl);
    // 0x80004098: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8000409C: sdc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    SD(ctx->f22.u64, 0X18, ctx->r29);
    // 0x800040A0: mul.s       $f10, $f20, $f20
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f10.fl = MUL_S(ctx->f20.fl, ctx->f20.fl);
    // 0x800040A4: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x800040A8: sw          $a1, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r5;
    // 0x800040AC: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x800040B0: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x800040B4: add.s       $f12, $f8, $f10
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f12.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x800040B8: c.eq.s      $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f4.fl == ctx->f12.fl;
    // 0x800040BC: nop

    // 0x800040C0: bc1tl       L_80004310
    if (c1cs) {
        // 0x800040C4: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80004310;
    }
    goto skip_0;
    // 0x800040C4: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_0:
    // 0x800040C8: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x800040CC: jal         0x800A01E0
    // 0x800040D0: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x800040D0: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    after_0:
    // 0x800040D4: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x800040D8: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x800040DC: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x800040E0: cvt.d.s     $f6, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f6.d = CVT_D_S(ctx->f0.fl);
    // 0x800040E4: lwc1        $f14, 0x50($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X50);
    // 0x800040E8: div.d       $f8, $f16, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = DIV_D(ctx->f16.d, ctx->f6.d);
    // 0x800040EC: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x800040F0: mtc1        $zero, $f7
    ctx->f_odd[(7 - 1) * 2] = 0;
    // 0x800040F4: lw          $a2, 0x48($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X48);
    // 0x800040F8: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800040FC: cvt.s.d     $f2, $f8
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.d); 
    ctx->f2.fl = CVT_S_D(ctx->f8.d);
    // 0x80004100: mul.s       $f14, $f14, $f2
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f14.fl = MUL_S(ctx->f14.fl, ctx->f2.fl);
    // 0x80004104: nop

    // 0x80004108: mul.s       $f24, $f24, $f2
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 24);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f24.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f24.fl = MUL_S(ctx->f24.fl, ctx->f2.fl);
    // 0x8000410C: nop

    // 0x80004110: mul.s       $f20, $f20, $f2
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f20.fl = MUL_S(ctx->f20.fl, ctx->f2.fl);
    // 0x80004114: nop

    // 0x80004118: mul.s       $f10, $f14, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8000411C: nop

    // 0x80004120: mul.s       $f4, $f20, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 20);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f20.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f20.fl, ctx->f20.fl);
    // 0x80004124: add.s       $f22, $f10, $f4
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f22.fl = ctx->f10.fl + ctx->f4.fl;
    // 0x80004128: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x8000412C: cvt.d.s     $f8, $f22
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f22.fl); 
    ctx->f8.d = CVT_D_S(ctx->f22.fl);
    // 0x80004130: c.eq.d      $f6, $f8
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f6.d == ctx->f8.d;
    // 0x80004134: nop

    // 0x80004138: bc1f        L_80004144
    if (!c1cs) {
        // 0x8000413C: nop
    
            goto L_80004144;
    }
    // 0x8000413C: nop

    // 0x80004140: lwc1        $f22, -0x3294($at)
    ctx->f22.u32l = MEM_W(ctx->r1, -0X3294);
L_80004144:
    // 0x80004144: c.eq.s      $f22, $f10
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f22.fl == ctx->f10.fl;
    // 0x80004148: nop

    // 0x8000414C: bc1tl       L_80004310
    if (c1cs) {
        // 0x80004150: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_80004310;
    }
    goto skip_1;
    // 0x80004150: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_1:
    // 0x80004154: mov.s       $f12, $f22
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    ctx->f12.fl = ctx->f22.fl;
    // 0x80004158: sw          $a2, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r6;
    // 0x8000415C: jal         0x800A01E0
    // 0x80004160: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    sqrtf_recomp(rdram, ctx);
        goto after_1;
    // 0x80004160: swc1        $f14, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f14.u32l;
    after_1:
    // 0x80004164: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80004168: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x8000416C: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80004170: cvt.d.s     $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f4.d = CVT_D_S(ctx->f0.fl);
    // 0x80004174: lhu         $a0, 0x4E($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X4E);
    // 0x80004178: div.d       $f6, $f16, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = DIV_D(ctx->f16.d, ctx->f4.d);
    // 0x8000417C: mov.s       $f22, $f0
    CHECK_FR(ctx, 22);
    CHECK_FR(ctx, 0);
    ctx->f22.fl = ctx->f0.fl;
    // 0x80004180: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80004184: cvt.s.d     $f2, $f6
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.d); 
    ctx->f2.fl = CVT_S_D(ctx->f6.d);
    // 0x80004188: jal         0x800A3A50
    // 0x8000418C: swc1        $f2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f2.u32l;
    func_800A3A50(rdram, ctx);
        goto after_2;
    // 0x8000418C: swc1        $f2, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f2.u32l;
    after_2:
    // 0x80004190: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x80004194: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80004198: lwc1        $f4, -0x3290($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X3290);
    // 0x8000419C: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x800041A0: lw          $a0, 0x30($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X30);
    // 0x800041A4: div.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = DIV_S(ctx->f10.fl, ctx->f4.fl);
    // 0x800041A8: jal         0x80097330
    // 0x800041AC: swc1        $f12, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f12.u32l;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x800041AC: swc1        $f12, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f12.u32l;
    after_3:
    // 0x800041B0: lw          $v1, 0x60($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X60);
    // 0x800041B4: lwc1        $f14, 0x50($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X50);
    // 0x800041B8: lw          $a2, 0x48($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X48);
    // 0x800041BC: lwc1        $f6, 0x0($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X0);
    // 0x800041C0: lwc1        $f2, 0x38($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X38);
    // 0x800041C4: lwc1        $f12, 0x44($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X44);
    // 0x800041C8: mul.s       $f8, $f6, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f14.fl);
    // 0x800041CC: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800041D0: neg.s       $f18, $f12
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f18.fl = -ctx->f12.fl;
    // 0x800041D4: swc1        $f8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->f8.u32l;
    // 0x800041D8: lwc1        $f10, 0x0($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X0);
    // 0x800041DC: mul.s       $f4, $f10, $f24
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f24.fl);
    // 0x800041E0: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x800041E4: swc1        $f4, 0x4($a2)
    MEM_W(0X4, ctx->r6) = ctx->f4.u32l;
    // 0x800041E8: lwc1        $f6, 0x0($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X0);
    // 0x800041EC: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x800041F0: swc1        $f10, 0xC($a2)
    MEM_W(0XC, ctx->r6) = ctx->f10.u32l;
    // 0x800041F4: mul.s       $f8, $f6, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f20.fl);
    // 0x800041F8: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800041FC: swc1        $f8, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->f8.u32l;
    // 0x80004200: lwc1        $f8, -0x328C($at)
    ctx->f8.u32l = MEM_W(ctx->r1, -0X328C);
    // 0x80004204: mul.s       $f10, $f18, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f18.fl, ctx->f14.fl);
    // 0x80004208: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x8000420C: div.s       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80004210: mul.s       $f4, $f10, $f24
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f4.fl = MUL_S(ctx->f10.fl, ctx->f24.fl);
    // 0x80004214: nop

    // 0x80004218: mul.s       $f6, $f16, $f20
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f20.fl);
    // 0x8000421C: sub.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80004220: lwc1        $f4, 0x4($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X4);
    // 0x80004224: mul.s       $f10, $f8, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x80004228: nop

    // 0x8000422C: mul.s       $f6, $f4, $f10
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f10.fl);
    // 0x80004230: swc1        $f6, 0x10($a2)
    MEM_W(0X10, ctx->r6) = ctx->f6.u32l;
    // 0x80004234: mul.s       $f4, $f12, $f22
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f4.fl = MUL_S(ctx->f12.fl, ctx->f22.fl);
    // 0x80004238: lwc1        $f8, 0x4($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X4);
    // 0x8000423C: mul.s       $f10, $f8, $f4
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f4.fl);
    // 0x80004240: nop

    // 0x80004244: mul.s       $f0, $f16, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = MUL_S(ctx->f16.fl, ctx->f14.fl);
    // 0x80004248: nop

    // 0x8000424C: mul.s       $f8, $f18, $f24
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f8.fl = MUL_S(ctx->f18.fl, ctx->f24.fl);
    // 0x80004250: swc1        $f10, 0x14($a2)
    MEM_W(0X14, ctx->r6) = ctx->f10.u32l;
    // 0x80004254: lwc1        $f6, 0x4($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X4);
    // 0x80004258: mul.s       $f4, $f8, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f8.fl, ctx->f20.fl);
    // 0x8000425C: add.s       $f10, $f4, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f0.fl;
    // 0x80004260: mul.s       $f8, $f10, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f10.fl, ctx->f2.fl);
    // 0x80004264: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80004268: nop

    // 0x8000426C: swc1        $f10, 0x1C($a2)
    MEM_W(0X1C, ctx->r6) = ctx->f10.u32l;
    // 0x80004270: mul.s       $f4, $f6, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80004274: nop

    // 0x80004278: mul.s       $f6, $f0, $f24
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f24.fl);
    // 0x8000427C: nop

    // 0x80004280: mul.s       $f8, $f12, $f20
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f8.fl = MUL_S(ctx->f12.fl, ctx->f20.fl);
    // 0x80004284: swc1        $f4, 0x18($a2)
    MEM_W(0X18, ctx->r6) = ctx->f4.u32l;
    // 0x80004288: sub.s       $f4, $f6, $f8
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f4.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x8000428C: lwc1        $f6, 0x8($v1)
    ctx->f6.u32l = MEM_W(ctx->r3, 0X8);
    // 0x80004290: mul.s       $f10, $f4, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80004294: nop

    // 0x80004298: mul.s       $f8, $f6, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f10.fl);
    // 0x8000429C: neg.s       $f6, $f16
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f16.fl); 
    ctx->f6.fl = -ctx->f16.fl;
    // 0x800042A0: mul.s       $f10, $f6, $f22
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 22);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f22.fl); 
    ctx->f10.fl = MUL_S(ctx->f6.fl, ctx->f22.fl);
    // 0x800042A4: swc1        $f8, 0x20($a2)
    MEM_W(0X20, ctx->r6) = ctx->f8.u32l;
    // 0x800042A8: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x800042AC: mul.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f4.fl, ctx->f10.fl);
    // 0x800042B0: nop

    // 0x800042B4: mul.s       $f6, $f16, $f24
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 24);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f24.fl); 
    ctx->f6.fl = MUL_S(ctx->f16.fl, ctx->f24.fl);
    // 0x800042B8: swc1        $f8, 0x24($a2)
    MEM_W(0X24, ctx->r6) = ctx->f8.u32l;
    // 0x800042BC: lw          $v0, 0x5C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X5C);
    // 0x800042C0: mul.s       $f4, $f6, $f20
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 20);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f20.fl); 
    ctx->f4.fl = MUL_S(ctx->f6.fl, ctx->f20.fl);
    // 0x800042C4: nop

    // 0x800042C8: mul.s       $f10, $f12, $f14
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f10.fl = MUL_S(ctx->f12.fl, ctx->f14.fl);
    // 0x800042CC: add.s       $f8, $f4, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f10.fl;
    // 0x800042D0: lwc1        $f4, 0x8($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X8);
    // 0x800042D4: mul.s       $f6, $f8, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x800042D8: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x800042DC: nop

    // 0x800042E0: swc1        $f8, 0x2C($a2)
    MEM_W(0X2C, ctx->r6) = ctx->f8.u32l;
    // 0x800042E4: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x800042E8: mul.s       $f10, $f4, $f6
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f10.fl = MUL_S(ctx->f4.fl, ctx->f6.fl);
    // 0x800042EC: swc1        $f10, 0x28($a2)
    MEM_W(0X28, ctx->r6) = ctx->f10.u32l;
    // 0x800042F0: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x800042F4: swc1        $f4, 0x30($a2)
    MEM_W(0X30, ctx->r6) = ctx->f4.u32l;
    // 0x800042F8: lwc1        $f6, 0x4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X4);
    // 0x800042FC: swc1        $f6, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->f6.u32l;
    // 0x80004300: lwc1        $f10, 0x8($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80004304: swc1        $f8, 0x3C($a2)
    MEM_W(0X3C, ctx->r6) = ctx->f8.u32l;
    // 0x80004308: swc1        $f10, 0x38($a2)
    MEM_W(0X38, ctx->r6) = ctx->f10.u32l;
    // 0x8000430C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_80004310:
    // 0x80004310: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80004314: ldc1        $f22, 0x18($sp)
    CHECK_FR(ctx, 22);
    ctx->f22.u64 = LD(ctx->r29, 0X18);
    // 0x80004318: ldc1        $f24, 0x20($sp)
    CHECK_FR(ctx, 24);
    ctx->f24.u64 = LD(ctx->r29, 0X20);
    // 0x8000431C: jr          $ra
    // 0x80004320: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x80004320: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_80004324(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004324: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x80004328: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x8000432C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80004330: cvt.d.s     $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f4.d = CVT_D_S(ctx->f12.fl);
    // 0x80004334: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80004338: c.eq.d      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.d == ctx->f4.d;
    // 0x8000433C: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80004340: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x80004344: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80004348: bc1f        L_80004370
    if (!c1cs) {
        // 0x8000434C: lh          $t6, 0x26($sp)
        ctx->r14 = MEM_H(ctx->r29, 0X26);
            goto L_80004370;
    }
    // 0x8000434C: lh          $t6, 0x26($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X26);
    // 0x80004350: lwc1        $f6, 0x30($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80004354: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80004358: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x8000435C: c.eq.d      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.d == ctx->f8.d;
    // 0x80004360: nop

    // 0x80004364: bc1fl       L_80004374
    if (!c1cs) {
        // 0x80004368: mtc1        $t6, $f10
        ctx->f10.u32l = ctx->r14;
            goto L_80004374;
    }
    goto skip_0;
    // 0x80004368: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    skip_0:
    // 0x8000436C: lwc1        $f12, -0x3288($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X3288);
L_80004370:
    // 0x80004370: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
L_80004374:
    // 0x80004374: lui         $at, 0x3BB4
    ctx->r1 = S32(0X3BB4 << 16);
    // 0x80004378: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x8000437C: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80004380: lwc1        $f10, 0x30($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80004384: neg.s       $f6, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = -ctx->f12.fl;
    // 0x80004388: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x8000438C: neg.s       $f8, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = -ctx->f14.fl;
    // 0x80004390: neg.s       $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = -ctx->f10.fl;
    // 0x80004394: mfc1        $a3, $f8
    ctx->r7 = (int32_t)ctx->f8.u32l;
    // 0x80004398: mfc1        $a2, $f6
    ctx->r6 = (int32_t)ctx->f6.u32l;
    // 0x8000439C: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    // 0x800043A0: mfc1        $a1, $f4
    ctx->r5 = (int32_t)ctx->f4.u32l;
    // 0x800043A4: jal         0x80098D30
    // 0x800043A8: nop

    guLookAtF(rdram, ctx);
        goto after_0;
    // 0x800043A8: nop

    after_0:
    // 0x800043AC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800043B0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800043B4: jr          $ra
    // 0x800043B8: nop

    return;
    // 0x800043B8: nop

;}
RECOMP_FUNC void func_800043BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800043BC: mtc1        $a2, $f12
    ctx->f12.u32l = ctx->r6;
    // 0x800043C0: mtc1        $zero, $f1
    ctx->f_odd[(1 - 1) * 2] = 0;
    // 0x800043C4: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x800043C8: cvt.d.s     $f4, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f4.d = CVT_D_S(ctx->f12.fl);
    // 0x800043CC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800043D0: c.eq.d      $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    c1cs = ctx->f0.d == ctx->f4.d;
    // 0x800043D4: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x800043D8: mtc1        $a3, $f14
    ctx->f14.u32l = ctx->r7;
    // 0x800043DC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800043E0: bc1f        L_80004408
    if (!c1cs) {
        // 0x800043E4: lh          $t6, 0x26($sp)
        ctx->r14 = MEM_H(ctx->r29, 0X26);
            goto L_80004408;
    }
    // 0x800043E4: lh          $t6, 0x26($sp)
    ctx->r14 = MEM_H(ctx->r29, 0X26);
    // 0x800043E8: lwc1        $f6, 0x30($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X30);
    // 0x800043EC: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x800043F0: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x800043F4: c.eq.d      $f0, $f8
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    c1cs = ctx->f0.d == ctx->f8.d;
    // 0x800043F8: nop

    // 0x800043FC: bc1fl       L_8000440C
    if (!c1cs) {
        // 0x80004400: mtc1        $t6, $f10
        ctx->f10.u32l = ctx->r14;
            goto L_8000440C;
    }
    goto skip_0;
    // 0x80004400: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
    skip_0:
    // 0x80004404: lwc1        $f12, -0x3284($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X3284);
L_80004408:
    // 0x80004408: mtc1        $t6, $f10
    ctx->f10.u32l = ctx->r14;
L_8000440C:
    // 0x8000440C: lui         $at, 0x3BB4
    ctx->r1 = S32(0X3BB4 << 16);
    // 0x80004410: mtc1        $at, $f18
    ctx->f18.u32l = ctx->r1;
    // 0x80004414: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80004418: lwc1        $f10, 0x30($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X30);
    // 0x8000441C: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80004420: neg.s       $f6, $f12
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f6.fl = -ctx->f12.fl;
    // 0x80004424: mul.s       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x80004428: neg.s       $f8, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = -ctx->f14.fl;
    // 0x8000442C: neg.s       $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = -ctx->f10.fl;
    // 0x80004430: mfc1        $a3, $f8
    ctx->r7 = (int32_t)ctx->f8.u32l;
    // 0x80004434: mfc1        $a2, $f6
    ctx->r6 = (int32_t)ctx->f6.u32l;
    // 0x80004438: swc1        $f16, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->f16.u32l;
    // 0x8000443C: mfc1        $a1, $f4
    ctx->r5 = (int32_t)ctx->f4.u32l;
    // 0x80004440: jal         0x80098D30
    // 0x80004444: nop

    guLookAtF(rdram, ctx);
        goto after_0;
    // 0x80004444: nop

    after_0:
    // 0x80004448: lw          $v0, 0x38($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X38);
    // 0x8000444C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80004450: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x80004454: lwc1        $f18, 0x0($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X0);
    // 0x80004458: lwc1        $f8, 0x4($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X4);
    // 0x8000445C: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x80004460: lwc1        $f18, 0x8($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X8);
    // 0x80004464: swc1        $f6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->f6.u32l;
    // 0x80004468: lwc1        $f10, 0x0($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8000446C: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80004470: lwc1        $f8, 0x10($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X10);
    // 0x80004474: swc1        $f16, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->f16.u32l;
    // 0x80004478: lwc1        $f4, 0x0($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X0);
    // 0x8000447C: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x80004480: lwc1        $f18, 0x14($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X14);
    // 0x80004484: swc1        $f6, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->f6.u32l;
    // 0x80004488: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x8000448C: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80004490: lwc1        $f8, 0x18($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X18);
    // 0x80004494: swc1        $f16, 0x10($a0)
    MEM_W(0X10, ctx->r4) = ctx->f16.u32l;
    // 0x80004498: lwc1        $f4, 0x4($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X4);
    // 0x8000449C: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x800044A0: lwc1        $f18, 0x20($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X20);
    // 0x800044A4: swc1        $f6, 0x14($a0)
    MEM_W(0X14, ctx->r4) = ctx->f6.u32l;
    // 0x800044A8: lwc1        $f10, 0x4($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X4);
    // 0x800044AC: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x800044B0: lwc1        $f8, 0x24($a0)
    ctx->f8.u32l = MEM_W(ctx->r4, 0X24);
    // 0x800044B4: swc1        $f16, 0x18($a0)
    MEM_W(0X18, ctx->r4) = ctx->f16.u32l;
    // 0x800044B8: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x800044BC: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x800044C0: lwc1        $f18, 0x28($a0)
    ctx->f18.u32l = MEM_W(ctx->r4, 0X28);
    // 0x800044C4: swc1        $f6, 0x20($a0)
    MEM_W(0X20, ctx->r4) = ctx->f6.u32l;
    // 0x800044C8: lwc1        $f10, 0x8($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X8);
    // 0x800044CC: lw          $v1, 0x34($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X34);
    // 0x800044D0: mul.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x800044D4: swc1        $f16, 0x24($a0)
    MEM_W(0X24, ctx->r4) = ctx->f16.u32l;
    // 0x800044D8: lwc1        $f4, 0x8($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X8);
    // 0x800044DC: mul.s       $f6, $f18, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f18.fl, ctx->f4.fl);
    // 0x800044E0: swc1        $f6, 0x28($a0)
    MEM_W(0X28, ctx->r4) = ctx->f6.u32l;
    // 0x800044E4: lwc1        $f8, 0x0($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X0);
    // 0x800044E8: swc1        $f8, 0x30($a0)
    MEM_W(0X30, ctx->r4) = ctx->f8.u32l;
    // 0x800044EC: lwc1        $f10, 0x4($v1)
    ctx->f10.u32l = MEM_W(ctx->r3, 0X4);
    // 0x800044F0: swc1        $f10, 0x34($a0)
    MEM_W(0X34, ctx->r4) = ctx->f10.u32l;
    // 0x800044F4: lwc1        $f16, 0x8($v1)
    ctx->f16.u32l = MEM_W(ctx->r3, 0X8);
    // 0x800044F8: swc1        $f16, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->f16.u32l;
    // 0x800044FC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80004500: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80004504: jr          $ra
    // 0x80004508: nop

    return;
    // 0x80004508: nop

;}
RECOMP_FUNC void func_8000450C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000450C: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x80004510: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80004514: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80004518: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8000451C: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x80004520: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80004524: bne         $a2, $zero, L_8000454C
    if (ctx->r6 != 0) {
        // 0x80004528: sw          $a2, 0x70($sp)
        MEM_W(0X70, ctx->r29) = ctx->r6;
            goto L_8000454C;
    }
    // 0x80004528: sw          $a2, 0x70($sp)
    MEM_W(0X70, ctx->r29) = ctx->r6;
    // 0x8000452C: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80004530: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80004534: jal         0x80002F60
    // 0x80004538: addiu       $a2, $sp, 0x38
    ctx->r6 = ADD32(ctx->r29, 0X38);
    func_80002F60(rdram, ctx);
        goto after_0;
    // 0x80004538: addiu       $a2, $sp, 0x38
    ctx->r6 = ADD32(ctx->r29, 0X38);
    after_0:
    // 0x8000453C: lwc1        $f0, 0x38($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80004540: lwc1        $f2, 0x3C($sp)
    ctx->f2.u32l = MEM_W(ctx->r29, 0X3C);
    // 0x80004544: b           L_80004594
    // 0x80004548: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
        goto L_80004594;
    // 0x80004548: lwc1        $f14, 0x40($sp)
    ctx->f14.u32l = MEM_W(ctx->r29, 0X40);
L_8000454C:
    // 0x8000454C: lw          $a0, 0x70($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X70);
    // 0x80004550: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80004554: addiu       $a2, $sp, 0x2C
    ctx->r6 = ADD32(ctx->r29, 0X2C);
    // 0x80004558: jal         0x80002F60
    // 0x8000455C: sw          $a3, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r7;
    func_80002F60(rdram, ctx);
        goto after_1;
    // 0x8000455C: sw          $a3, 0x74($sp)
    MEM_W(0X74, ctx->r29) = ctx->r7;
    after_1:
    // 0x80004560: lw          $a0, 0x74($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X74);
    // 0x80004564: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80004568: jal         0x80002F60
    // 0x8000456C: addiu       $a2, $sp, 0x20
    ctx->r6 = ADD32(ctx->r29, 0X20);
    func_80002F60(rdram, ctx);
        goto after_2;
    // 0x8000456C: addiu       $a2, $sp, 0x20
    ctx->r6 = ADD32(ctx->r29, 0X20);
    after_2:
    // 0x80004570: lwc1        $f4, 0x20($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X20);
    // 0x80004574: lwc1        $f6, 0x2C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X2C);
    // 0x80004578: lwc1        $f8, 0x24($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X24);
    // 0x8000457C: lwc1        $f10, 0x30($sp)
    ctx->f10.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80004580: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x80004584: lwc1        $f6, 0x34($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80004588: lwc1        $f4, 0x28($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X28);
    // 0x8000458C: sub.s       $f2, $f8, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f2.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80004590: sub.s       $f14, $f4, $f6
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f14.fl = ctx->f4.fl - ctx->f6.fl;
L_80004594:
    // 0x80004594: mul.s       $f8, $f0, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x80004598: lui         $at, 0xBF80
    ctx->r1 = S32(0XBF80 << 16);
    // 0x8000459C: mul.s       $f10, $f2, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x800045A0: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x800045A4: mul.s       $f6, $f14, $f14
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f6.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x800045A8: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x800045AC: add.s       $f12, $f4, $f6
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x800045B0: c.eq.s      $f8, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f8.fl == ctx->f12.fl;
    // 0x800045B4: swc1        $f12, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->f12.u32l;
    // 0x800045B8: bc1tl       L_80004664
    if (c1cs) {
        // 0x800045BC: mtc1        $at, $f0
        ctx->f0.u32l = ctx->r1;
            goto L_80004664;
    }
    goto skip_0;
    // 0x800045BC: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    skip_0:
    // 0x800045C0: lwc1        $f10, 0x0($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X0);
    // 0x800045C4: lwc1        $f4, 0x30($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X30);
    // 0x800045C8: lwc1        $f6, 0x4($s1)
    ctx->f6.u32l = MEM_W(ctx->r17, 0X4);
    // 0x800045CC: lwc1        $f8, 0x34($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X34);
    // 0x800045D0: sub.s       $f12, $f10, $f4
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f12.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x800045D4: lwc1        $f4, 0x38($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X38);
    // 0x800045D8: lwc1        $f10, 0x8($s1)
    ctx->f10.u32l = MEM_W(ctx->r17, 0X8);
    // 0x800045DC: sub.s       $f16, $f6, $f8
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f16.fl = ctx->f6.fl - ctx->f8.fl;
    // 0x800045E0: mul.s       $f8, $f0, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f12.fl);
    // 0x800045E4: sub.s       $f6, $f10, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = ctx->f10.fl - ctx->f4.fl;
    // 0x800045E8: mul.s       $f10, $f2, $f16
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f16.fl);
    // 0x800045EC: swc1        $f6, 0x50($sp)
    MEM_W(0X50, ctx->r29) = ctx->f6.u32l;
    // 0x800045F0: lwc1        $f6, 0x50($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X50);
    // 0x800045F4: add.s       $f4, $f8, $f10
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f4.fl = ctx->f8.fl + ctx->f10.fl;
    // 0x800045F8: mul.s       $f8, $f14, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x800045FC: lwc1        $f6, 0x4C($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X4C);
    // 0x80004600: add.s       $f10, $f4, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = ctx->f4.fl + ctx->f8.fl;
    // 0x80004604: div.s       $f18, $f10, $f6
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f18.fl = DIV_S(ctx->f10.fl, ctx->f6.fl);
    // 0x80004608: mul.s       $f4, $f0, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f4.fl = MUL_S(ctx->f0.fl, ctx->f18.fl);
    // 0x8000460C: swc1        $f18, 0x44($sp)
    MEM_W(0X44, ctx->r29) = ctx->f18.u32l;
    // 0x80004610: sub.s       $f0, $f4, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f12.fl;
    // 0x80004614: swc1        $f0, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->f0.u32l;
    // 0x80004618: lwc1        $f8, 0x44($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8000461C: mul.s       $f10, $f2, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = MUL_S(ctx->f2.fl, ctx->f8.fl);
    // 0x80004620: sub.s       $f2, $f10, $f16
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f2.fl = ctx->f10.fl - ctx->f16.fl;
    // 0x80004624: swc1        $f2, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->f2.u32l;
    // 0x80004628: lwc1        $f6, 0x44($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X44);
    // 0x8000462C: lwc1        $f8, 0x50($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X50);
    // 0x80004630: mul.s       $f4, $f14, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = MUL_S(ctx->f14.fl, ctx->f6.fl);
    // 0x80004634: sub.s       $f14, $f4, $f8
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f14.fl = ctx->f4.fl - ctx->f8.fl;
    // 0x80004638: mul.s       $f10, $f0, $f0
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f10.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x8000463C: nop

    // 0x80004640: mul.s       $f6, $f2, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80004644: swc1        $f14, 0x8($s1)
    MEM_W(0X8, ctx->r17) = ctx->f14.u32l;
    // 0x80004648: mul.s       $f8, $f14, $f14
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f14.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f8.fl = MUL_S(ctx->f14.fl, ctx->f14.fl);
    // 0x8000464C: add.s       $f4, $f10, $f6
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f4.fl = ctx->f10.fl + ctx->f6.fl;
    // 0x80004650: jal         0x800A01E0
    // 0x80004654: add.s       $f12, $f4, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_3;
    // 0x80004654: add.s       $f12, $f4, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f4.fl + ctx->f8.fl;
    after_3:
    // 0x80004658: b           L_8000466C
    // 0x8000465C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8000466C;
    // 0x8000465C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80004660: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
L_80004664:
    // 0x80004664: nop

    // 0x80004668: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8000466C:
    // 0x8000466C: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80004670: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80004674: jr          $ra
    // 0x80004678: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x80004678: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void func_80004680(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80004680: bgez        $a0, L_80004690
    if (SIGNED(ctx->r4) >= 0) {
        // 0x80004684: or          $v1, $a0, $zero
        ctx->r3 = ctx->r4 | 0;
            goto L_80004690;
    }
    // 0x80004684: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80004688: jr          $ra
    // 0x8000468C: negu        $v0, $a0
    ctx->r2 = SUB32(0, ctx->r4);
    return;
    // 0x8000468C: negu        $v0, $a0
    ctx->r2 = SUB32(0, ctx->r4);
L_80004690:
    // 0x80004690: jr          $ra
    // 0x80004694: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80004694: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
