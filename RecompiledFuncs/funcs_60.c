#include "recomp.h"
#include "funcs.h"
#include "lod_symbols.h"

RECOMP_FUNC void func_80176020(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176020: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80176024: lh          $v0, -0x5470($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X5470);
    // 0x80176028: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8017602C: addiu       $at, $zero, 0x7
    ctx->r1 = ADD32(0, 0X7);
    // 0x80176030: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176034: beq         $v0, $at, L_80176068
    if (ctx->r2 == ctx->r1) {
        // 0x80176038: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_80176068;
    }
    // 0x80176038: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8017603C: addiu       $at, $zero, 0x8
    ctx->r1 = ADD32(0, 0X8);
    // 0x80176040: beq         $v0, $at, L_80176078
    if (ctx->r2 == ctx->r1) {
        // 0x80176044: lui         $t7, 0x8019
        ctx->r15 = S32(0X8019 << 16);
            goto L_80176078;
    }
    // 0x80176044: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80176048: addiu       $at, $zero, 0x19
    ctx->r1 = ADD32(0, 0X19);
    // 0x8017604C: beq         $v0, $at, L_80176090
    if (ctx->r2 == ctx->r1) {
        // 0x80176050: lui         $t9, 0x8019
        ctx->r25 = S32(0X8019 << 16);
            goto L_80176090;
    }
    // 0x80176050: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176054: addiu       $at, $zero, 0x2A
    ctx->r1 = ADD32(0, 0X2A);
    // 0x80176058: beq         $v0, $at, L_80176084
    if (ctx->r2 == ctx->r1) {
        // 0x8017605C: lui         $t8, 0x8019
        ctx->r24 = S32(0X8019 << 16);
            goto L_80176084;
    }
    // 0x8017605C: lui         $t8, 0x8019
    ctx->r24 = S32(0X8019 << 16);
    // 0x80176060: b           L_8017609C
    // 0x80176064: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
        goto L_8017609C;
    // 0x80176064: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
L_80176068:
    // 0x80176068: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x8017606C: addiu       $t6, $t6, 0x2700
    ctx->r14 = ADD32(ctx->r14, 0X2700);
    // 0x80176070: b           L_80176098
    // 0x80176074: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
        goto L_80176098;
    // 0x80176074: sw          $t6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r14;
L_80176078:
    // 0x80176078: addiu       $t7, $t7, 0x2704
    ctx->r15 = ADD32(ctx->r15, 0X2704);
    // 0x8017607C: b           L_80176098
    // 0x80176080: sw          $t7, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r15;
        goto L_80176098;
    // 0x80176080: sw          $t7, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r15;
L_80176084:
    // 0x80176084: addiu       $t8, $t8, 0x2708
    ctx->r24 = ADD32(ctx->r24, 0X2708);
    // 0x80176088: b           L_80176098
    // 0x8017608C: sw          $t8, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r24;
        goto L_80176098;
    // 0x8017608C: sw          $t8, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r24;
L_80176090:
    // 0x80176090: addiu       $t9, $t9, 0x270C
    ctx->r25 = ADD32(ctx->r25, 0X270C);
    // 0x80176094: sw          $t9, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r25;
L_80176098:
    // 0x80176098: lw          $t0, 0x1C($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X1C);
L_8017609C:
    // 0x8017609C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801760A0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801760A4: sw          $t0, 0x34($a2)
    MEM_W(0X34, ctx->r6) = ctx->r8;
    // 0x801760A8: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x801760AC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801760B0: jalr        $t9
    // 0x801760B4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801760B4: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
    // 0x801760B8: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x801760BC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801760C0: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801760C4: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x801760C8: addu        $v0, $a0, $t2
    ctx->r2 = ADD32(ctx->r4, ctx->r10);
    // 0x801760CC: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x801760D0: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x801760D4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801760D8: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x801760DC: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x801760E0: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x801760E4: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x801760E8: lw          $t9, 0x2710($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2710);
    // 0x801760EC: jalr        $t9
    // 0x801760F0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x801760F0: nop

    after_1:
    // 0x801760F4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801760F8: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x801760FC: jr          $ra
    // 0x80176100: nop

    return;
    // 0x80176100: nop

;}
RECOMP_FUNC void func_80176104(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
;}
RECOMP_FUNC void func_801761E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801761E4: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801761E8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801761EC: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801761F0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801761F4: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x801761F8: lw          $s1, 0x24($a0)
    ctx->r17 = MEM_W(ctx->r4, 0X24);
    // 0x801761FC: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80176200: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    // 0x80176204: jal         0x8014314C
    // 0x80176208: addiu       $a1, $v0, 0x4
    ctx->r5 = ADD32(ctx->r2, 0X4);
    func_8014314C(rdram, ctx);
        goto after_0;
    // 0x80176208: addiu       $a1, $v0, 0x4
    ctx->r5 = ADD32(ctx->r2, 0X4);
    after_0:
    // 0x8017620C: beql        $v0, $zero, L_80176230
    if (ctx->r2 == 0) {
        // 0x80176210: lh          $t6, 0x5E($s1)
        ctx->r14 = MEM_H(ctx->r17, 0X5E);
            goto L_80176230;
    }
    goto skip_0;
    // 0x80176210: lh          $t6, 0x5E($s1)
    ctx->r14 = MEM_H(ctx->r17, 0X5E);
    skip_0:
    // 0x80176214: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80176218: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8017621C: jalr        $t9
    // 0x80176220: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176220: nop

    after_1:
    // 0x80176224: b           L_80176340
    // 0x80176228: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80176340;
    // 0x80176228: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8017622C: lh          $t6, 0x5E($s1)
    ctx->r14 = MEM_H(ctx->r17, 0X5E);
L_80176230:
    // 0x80176230: lui         $t9, 0x8009
    ctx->r25 = S32(0X8009 << 16);
    // 0x80176234: addiu       $t9, $t9, 0x7330
    ctx->r25 = ADD32(ctx->r25, 0X7330);
    // 0x80176238: addiu       $t7, $t6, 0x444
    ctx->r15 = ADD32(ctx->r14, 0X444);
    // 0x8017623C: sh          $t7, 0x5E($s1)
    MEM_H(0X5E, ctx->r17) = ctx->r15;
    // 0x80176240: lh          $a0, 0x5E($s1)
    ctx->r4 = MEM_H(ctx->r17, 0X5E);
    // 0x80176244: sll         $a0, $a0, 1
    ctx->r4 = S32(ctx->r4 << 1);
    // 0x80176248: jalr        $t9
    // 0x8017624C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8017624C: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_2:
    // 0x80176250: mtc1        $v0, $f4
    ctx->f4.u32l = ctx->r2;
    // 0x80176254: lui         $at, 0x40E0
    ctx->r1 = S32(0X40E0 << 16);
    // 0x80176258: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x8017625C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80176260: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x80176264: lui         $at, 0x4050
    ctx->r1 = S32(0X4050 << 16);
    // 0x80176268: mtc1        $at, $f19
    ctx->f_odd[(19 - 1) * 2] = ctx->r1;
    // 0x8017626C: mtc1        $zero, $f18
    ctx->f18.u32l = 0;
    // 0x80176270: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80176274: cvt.d.s     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f6.fl); 
    ctx->f8.d = CVT_D_S(ctx->f6.fl);
    // 0x80176278: div.d       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.d); NAN_CHECK(ctx->f10.d); 
    ctx->f16.d = DIV_D(ctx->f8.d, ctx->f10.d);
    // 0x8017627C: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x80176280: trunc.w.d   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_D(ctx->f4.d);
    // 0x80176284: mfc1        $t0, $f6
    ctx->r8 = (int32_t)ctx->f6.u32l;
    // 0x80176288: nop

    // 0x8017628C: addiu       $t1, $t0, 0xBF
    ctx->r9 = ADD32(ctx->r8, 0XBF);
    // 0x80176290: sb          $t1, 0x1B($s1)
    MEM_B(0X1B, ctx->r17) = ctx->r9;
    // 0x80176294: lhu         $a0, -0x7814($a0)
    ctx->r4 = MEM_HU(ctx->r4, -0X7814);
    // 0x80176298: sll         $a0, $a0, 9
    ctx->r4 = S32(ctx->r4 << 9);
    // 0x8017629C: jal         0x80097330
    // 0x801762A0: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_80097330(rdram, ctx);
        goto after_3;
    // 0x801762A0: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_3:
    // 0x801762A4: mtc1        $v0, $f8
    ctx->f8.u32l = ctx->r2;
    // 0x801762A8: lui         $at, 0x4700
    ctx->r1 = S32(0X4700 << 16);
    // 0x801762AC: mtc1        $at, $f16
    ctx->f16.u32l = ctx->r1;
    // 0x801762B0: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x801762B4: lw          $t2, 0x20($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X20);
    // 0x801762B8: lui         $at, 0x4018
    ctx->r1 = S32(0X4018 << 16);
    // 0x801762BC: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x801762C0: lwc1        $f4, 0x8($t2)
    ctx->f4.u32l = MEM_W(ctx->r10, 0X8);
    // 0x801762C4: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x801762C8: div.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = DIV_S(ctx->f10.fl, ctx->f16.fl);
    // 0x801762CC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801762D0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801762D4: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x801762D8: add.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = ctx->f6.d + ctx->f8.d;
    // 0x801762DC: cvt.d.s     $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f0.d = CVT_D_S(ctx->f18.fl);
    // 0x801762E0: add.d       $f16, $f0, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f0.d); 
    ctx->f16.d = ctx->f0.d + ctx->f0.d;
    // 0x801762E4: add.d       $f18, $f16, $f10
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f10.d); 
    ctx->f18.d = ctx->f16.d + ctx->f10.d;
    // 0x801762E8: cvt.s.d     $f4, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f4.fl = CVT_S_D(ctx->f18.d);
    // 0x801762EC: swc1        $f4, 0x54($s1)
    MEM_W(0X54, ctx->r17) = ctx->f4.u32l;
    // 0x801762F0: lh          $a1, 0xE($s0)
    ctx->r5 = MEM_H(ctx->r16, 0XE);
    // 0x801762F4: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x801762F8: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x801762FC: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80176300: sll         $t3, $a1, 1
    ctx->r11 = S32(ctx->r5 << 1);
    // 0x80176304: addu        $v1, $s0, $t3
    ctx->r3 = ADD32(ctx->r16, ctx->r11);
    // 0x80176308: sh          $a1, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r5;
    // 0x8017630C: lbu         $t6, 0x9($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X9);
    // 0x80176310: lbu         $t4, 0x8($v1)
    ctx->r12 = MEM_BU(ctx->r3, 0X8);
    // 0x80176314: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80176318: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x8017631C: sb          $t5, 0x8($v1)
    MEM_B(0X8, ctx->r3) = ctx->r13;
    // 0x80176320: addu        $t9, $t9, $t7
    ctx->r25 = ADD32(ctx->r25, ctx->r15);
    // 0x80176324: lw          $t9, 0x271C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X271C);
    // 0x80176328: jalr        $t9
    // 0x8017632C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x8017632C: nop

    after_4:
    // 0x80176330: lh          $t8, 0xE($s0)
    ctx->r24 = MEM_H(ctx->r16, 0XE);
    // 0x80176334: addiu       $t0, $t8, -0x1
    ctx->r8 = ADD32(ctx->r24, -0X1);
    // 0x80176338: sh          $t0, 0xE($s0)
    MEM_H(0XE, ctx->r16) = ctx->r8;
    // 0x8017633C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80176340:
    // 0x80176340: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80176344: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80176348: jr          $ra
    // 0x8017634C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x8017634C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80176350(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176350: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80176354: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176358: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8017635C: lw          $v0, 0x70($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X70);
    // 0x80176360: lw          $v1, -0x53E0($v1)
    ctx->r3 = MEM_W(ctx->r3, -0X53E0);
    // 0x80176364: lwc1        $f10, 0x8($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X8);
    // 0x80176368: lwc1        $f8, 0x54($v1)
    ctx->f8.u32l = MEM_W(ctx->r3, 0X54);
    // 0x8017636C: lwc1        $f6, 0x4($v0)
    ctx->f6.u32l = MEM_W(ctx->r2, 0X4);
    // 0x80176370: lwc1        $f4, 0x50($v1)
    ctx->f4.u32l = MEM_W(ctx->r3, 0X50);
    // 0x80176374: sub.s       $f16, $f8, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f16.fl = ctx->f8.fl - ctx->f10.fl;
    // 0x80176378: sub.s       $f0, $f4, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = ctx->f4.fl - ctx->f6.fl;
    // 0x8017637C: swc1        $f16, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->f16.u32l;
    // 0x80176380: lwc1        $f4, 0xC($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0XC);
    // 0x80176384: lwc1        $f18, 0x58($v1)
    ctx->f18.u32l = MEM_W(ctx->r3, 0X58);
    // 0x80176388: mul.s       $f6, $f0, $f0
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f0.fl);
    // 0x8017638C: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    // 0x80176390: sub.s       $f2, $f18, $f4
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f2.fl = ctx->f18.fl - ctx->f4.fl;
    // 0x80176394: mul.s       $f8, $f2, $f2
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f2.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f8.fl = MUL_S(ctx->f2.fl, ctx->f2.fl);
    // 0x80176398: jal         0x800A01E0
    // 0x8017639C: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
    sqrtf_recomp(rdram, ctx);
        goto after_0;
    // 0x8017639C: add.s       $f12, $f6, $f8
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f12.fl = ctx->f6.fl + ctx->f8.fl;
    after_0:
    // 0x801763A0: lui         $at, 0x4024
    ctx->r1 = S32(0X4024 << 16);
    // 0x801763A4: mtc1        $at, $f11
    ctx->f_odd[(11 - 1) * 2] = ctx->r1;
    // 0x801763A8: mtc1        $zero, $f10
    ctx->f10.u32l = 0;
    // 0x801763AC: cvt.d.s     $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f16.d = CVT_D_S(ctx->f0.fl);
    // 0x801763B0: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    // 0x801763B4: c.le.d      $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    c1cs = ctx->f16.d <= ctx->f10.d;
    // 0x801763B8: lwc1        $f18, 0x18($sp)
    ctx->f18.u32l = MEM_W(ctx->r29, 0X18);
    // 0x801763BC: bc1fl       L_80176458
    if (!c1cs) {
        // 0x801763C0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80176458;
    }
    goto skip_0;
    // 0x801763C0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x801763C4: mtc1        $zero, $f5
    ctx->f_odd[(5 - 1) * 2] = 0;
    // 0x801763C8: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x801763CC: cvt.d.s     $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.fl); 
    ctx->f0.d = CVT_D_S(ctx->f18.fl);
    // 0x801763D0: lui         $at, 0x4044
    ctx->r1 = S32(0X4044 << 16);
    // 0x801763D4: c.le.d      $f4, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f4.d <= ctx->f0.d;
    // 0x801763D8: nop

    // 0x801763DC: bc1fl       L_80176458
    if (!c1cs) {
        // 0x801763E0: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80176458;
    }
    goto skip_1;
    // 0x801763E0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x801763E4: mtc1        $at, $f7
    ctx->f_odd[(7 - 1) * 2] = ctx->r1;
    // 0x801763E8: mtc1        $zero, $f6
    ctx->f6.u32l = 0;
    // 0x801763EC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801763F0: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x801763F4: c.lt.d      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.d < ctx->f6.d;
    // 0x801763F8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801763FC: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x80176400: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80176404: bc1fl       L_80176458
    if (!c1cs) {
        // 0x80176408: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80176458;
    }
    goto skip_2;
    // 0x80176408: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x8017640C: sb          $t6, 0x40($a2)
    MEM_B(0X40, ctx->r6) = ctx->r14;
    // 0x80176410: jalr        $t9
    // 0x80176414: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176414: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    after_1:
    // 0x80176418: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x8017641C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176420: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x80176424: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x80176428: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x8017642C: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x80176430: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x80176434: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80176438: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x8017643C: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80176440: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x80176444: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x80176448: lw          $t9, 0x271C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X271C);
    // 0x8017644C: jalr        $t9
    // 0x80176450: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80176450: nop

    after_2:
    // 0x80176454: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80176458:
    // 0x80176458: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8017645C: jr          $ra
    // 0x80176460: nop

    return;
    // 0x80176460: nop

;}
RECOMP_FUNC void func_80176464(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176464: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80176468: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8017646C: lw          $t6, 0x2908($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2908);
    // 0x80176470: lw          $a3, 0x2B6C($v0)
    ctx->r7 = MEM_W(ctx->r2, 0X2B6C);
    // 0x80176474: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176478: ori         $t7, $t6, 0x40
    ctx->r15 = ctx->r14 | 0X40;
    // 0x8017647C: ori         $t9, $t7, 0x8
    ctx->r25 = ctx->r15 | 0X8;
    // 0x80176480: sw          $t7, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r15;
    // 0x80176484: sw          $t9, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r25;
    // 0x80176488: ori         $t1, $t9, 0x80
    ctx->r9 = ctx->r25 | 0X80;
    // 0x8017648C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176490: sw          $t1, 0x2908($v0)
    MEM_W(0X2908, ctx->r2) = ctx->r9;
    // 0x80176494: lw          $t2, 0x34($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X34);
    // 0x80176498: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8017649C: addiu       $at, $zero, 0x33
    ctx->r1 = ADD32(0, 0X33);
    // 0x801764A0: lhu         $v1, 0x6($t2)
    ctx->r3 = MEM_HU(ctx->r10, 0X6);
    // 0x801764A4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801764A8: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x801764AC: beq         $v1, $at, L_801764D0
    if (ctx->r3 == ctx->r1) {
        // 0x801764B0: nop
    
            goto L_801764D0;
    }
    // 0x801764B0: nop

    // 0x801764B4: jal         0x80179720
    // 0x801764B8: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_80179720(rdram, ctx);
        goto after_0;
    // 0x801764B8: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x801764BC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801764C0: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x801764C4: jal         0x80179D38
    // 0x801764C8: lw          $a0, 0x2B6C($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X2B6C);
    func_80179D38(rdram, ctx);
        goto after_1;
    // 0x801764C8: lw          $a0, 0x2B6C($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X2B6C);
    after_1:
    // 0x801764CC: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
L_801764D0:
    // 0x801764D0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801764D4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801764D8: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801764DC: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x801764E0: jalr        $t9
    // 0x801764E4: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801764E4: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_2:
    // 0x801764E8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801764EC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801764F0: lh          $t3, 0xE($a0)
    ctx->r11 = MEM_H(ctx->r4, 0XE);
    // 0x801764F4: sll         $t4, $t3, 1
    ctx->r12 = S32(ctx->r11 << 1);
    // 0x801764F8: addu        $v0, $a0, $t4
    ctx->r2 = ADD32(ctx->r4, ctx->r12);
    // 0x801764FC: lbu         $t7, 0x9($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X9);
    // 0x80176500: lbu         $t5, 0x8($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X8);
    // 0x80176504: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80176508: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x8017650C: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x80176510: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x80176514: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x80176518: lw          $t9, 0x271C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X271C);
    // 0x8017651C: jalr        $t9
    // 0x80176520: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80176520: nop

    after_3:
    // 0x80176524: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176528: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017652C: jr          $ra
    // 0x80176530: nop

    return;
    // 0x80176530: nop

;}
RECOMP_FUNC void func_80176534(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176534: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80176538: lh          $v0, -0x5470($v0)
    ctx->r2 = MEM_H(ctx->r2, -0X5470);
    // 0x8017653C: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x80176540: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80176544: addiu       $at, $zero, 0x19
    ctx->r1 = ADD32(0, 0X19);
    // 0x80176548: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8017654C: beq         $v0, $at, L_8017656C
    if (ctx->r2 == ctx->r1) {
        // 0x80176550: sw          $ra, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r31;
            goto L_8017656C;
    }
    // 0x80176550: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80176554: addiu       $at, $zero, 0x2A
    ctx->r1 = ADD32(0, 0X2A);
    // 0x80176558: beq         $v0, $at, L_80176588
    if (ctx->r2 == ctx->r1) {
        // 0x8017655C: ori         $a0, $zero, 0x81BF
        ctx->r4 = 0 | 0X81BF;
            goto L_80176588;
    }
    // 0x8017655C: ori         $a0, $zero, 0x81BF
    ctx->r4 = 0 | 0X81BF;
    // 0x80176560: lui         $v0, 0x8001
    ctx->r2 = S32(0X8001 << 16);
    // 0x80176564: b           L_8017659C
    // 0x80176568: addiu       $v0, $v0, 0x66F8
    ctx->r2 = ADD32(ctx->r2, 0X66F8);
        goto L_8017659C;
    // 0x80176568: addiu       $v0, $v0, 0x66F8
    ctx->r2 = ADD32(ctx->r2, 0X66F8);
L_8017656C:
    // 0x8017656C: lui         $v0, 0x8001
    ctx->r2 = S32(0X8001 << 16);
    // 0x80176570: addiu       $v0, $v0, 0x66F8
    ctx->r2 = ADD32(ctx->r2, 0X66F8);
    // 0x80176574: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    // 0x80176578: jalr        $v0
    // 0x8017657C: ori         $a0, $zero, 0x81ED
    ctx->r4 = 0 | 0X81ED;
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_0;
    // 0x8017657C: ori         $a0, $zero, 0x81ED
    ctx->r4 = 0 | 0X81ED;
    after_0:
    // 0x80176580: b           L_8017659C
    // 0x80176584: lw          $v0, 0x24($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X24);
        goto L_8017659C;
    // 0x80176584: lw          $v0, 0x24($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X24);
L_80176588:
    // 0x80176588: lui         $v0, 0x8001
    ctx->r2 = S32(0X8001 << 16);
    // 0x8017658C: addiu       $v0, $v0, 0x66F8
    ctx->r2 = ADD32(ctx->r2, 0X66F8);
    // 0x80176590: jalr        $v0
    // 0x80176594: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_1;
    // 0x80176594: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    after_1:
    // 0x80176598: lw          $v0, 0x24($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X24);
L_8017659C:
    // 0x8017659C: jalr        $v0
    // 0x801765A0: addiu       $a0, $zero, 0x19B
    ctx->r4 = ADD32(0, 0X19B);
    LOOKUP_FUNC(ctx->r2)(rdram, ctx);
        goto after_2;
    // 0x801765A0: addiu       $a0, $zero, 0x19B
    ctx->r4 = ADD32(0, 0X19B);
    after_2:
    // 0x801765A4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801765A8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801765AC: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801765B0: jalr        $t9
    // 0x801765B4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x801765B4: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_3:
    // 0x801765B8: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x801765BC: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801765C0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x801765C4: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x801765C8: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x801765CC: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x801765D0: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x801765D4: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x801765D8: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x801765DC: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x801765E0: sb          $t0, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r8;
    // 0x801765E4: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
    // 0x801765E8: lw          $t9, 0x271C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X271C);
    // 0x801765EC: jalr        $t9
    // 0x801765F0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801765F0: nop

    after_4:
    // 0x801765F4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x801765F8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x801765FC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x80176600: jr          $ra
    // 0x80176604: nop

    return;
    // 0x80176604: nop

;}
RECOMP_FUNC void func_80176608(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176608: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017660C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176610: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80176614: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80176618: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8017661C: lbu         $a3, 0x0($v1)
    ctx->r7 = MEM_BU(ctx->r3, 0X0);
    // 0x80176620: lbu         $t0, 0x1($v1)
    ctx->r8 = MEM_BU(ctx->r3, 0X1);
    // 0x80176624: addiu       $t1, $zero, 0xFF
    ctx->r9 = ADD32(0, 0XFF);
    // 0x80176628: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8017662C: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80176630: addiu       $t6, $zero, 0x2D
    ctx->r14 = ADD32(0, 0X2D);
    // 0x80176634: addiu       $t7, $zero, 0x12
    ctx->r15 = ADD32(0, 0X12);
    // 0x80176638: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8017663C: sh          $t7, 0x2BC2($v0)
    MEM_H(0X2BC2, ctx->r2) = ctx->r15;
    // 0x80176640: sh          $t6, 0x2BC0($v0)
    MEM_H(0X2BC0, ctx->r2) = ctx->r14;
    // 0x80176644: sb          $t1, 0x2BBC($v0)
    MEM_B(0X2BBC, ctx->r2) = ctx->r9;
    // 0x80176648: sb          $t1, 0x2BBD($v0)
    MEM_B(0X2BBD, ctx->r2) = ctx->r9;
    // 0x8017664C: sb          $t1, 0x2BBE($v0)
    MEM_B(0X2BBE, ctx->r2) = ctx->r9;
    // 0x80176650: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x80176654: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80176658: sh          $a3, 0x2BB8($v0)
    MEM_H(0X2BB8, ctx->r2) = ctx->r7;
    // 0x8017665C: jalr        $t9
    // 0x80176660: sh          $t0, 0x2BBA($v0)
    MEM_H(0X2BBA, ctx->r2) = ctx->r8;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176660: sh          $t0, 0x2BBA($v0)
    MEM_H(0X2BBA, ctx->r2) = ctx->r8;
    after_0:
    // 0x80176664: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176668: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017666C: jr          $ra
    // 0x80176670: nop

    return;
    // 0x80176670: nop

;}
RECOMP_FUNC void func_80176674(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176674: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176678: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8017667C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176680: addiu       $t9, $t9, 0x5BC
    ctx->r25 = ADD32(ctx->r25, 0X5BC);
    // 0x80176684: jalr        $t9
    // 0x80176688: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176688: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x8017668C: beq         $v0, $zero, L_801766B8
    if (ctx->r2 == 0) {
        // 0x80176690: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_801766B8;
    }
    // 0x80176690: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176694: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176698: jalr        $t9
    // 0x8017669C: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8017669C: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    after_1:
    // 0x801766A0: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x801766A4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801766A8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801766AC: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x801766B0: jalr        $t9
    // 0x801766B4: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x801766B4: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    after_2:
L_801766B8:
    // 0x801766B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801766BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801766C0: jr          $ra
    // 0x801766C4: nop

    return;
    // 0x801766C4: nop

;}
RECOMP_FUNC void func_801766C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801766C8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801766CC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801766D0: lw          $t6, 0x38($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X38);
    // 0x801766D4: lui         $t9, 0x8006
    ctx->r25 = S32(0X8006 << 16);
    // 0x801766D8: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x801766DC: beq         $t6, $zero, L_801766F4
    if (ctx->r14 == 0) {
        // 0x801766E0: addiu       $t9, $t9, -0x2EC4
        ctx->r25 = ADD32(ctx->r25, -0X2EC4);
            goto L_801766F4;
    }
    // 0x801766E0: addiu       $t9, $t9, -0x2EC4
    ctx->r25 = ADD32(ctx->r25, -0X2EC4);
    // 0x801766E4: lw          $a0, 0x38($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X38);
    // 0x801766E8: jalr        $t9
    // 0x801766EC: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801766EC: sw          $a1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r5;
    after_0:
    // 0x801766F0: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
L_801766F4:
    // 0x801766F4: lw          $t9, 0x3C($a1)
    ctx->r25 = MEM_W(ctx->r5, 0X3C);
    // 0x801766F8: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x801766FC: jalr        $t9
    // 0x80176700: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176700: nop

    after_1:
    // 0x80176704: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176708: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8017670C: jr          $ra
    // 0x80176710: nop

    return;
    // 0x80176710: nop

;}
RECOMP_FUNC void func_80176720(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176720: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176724: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176728: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8017672C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176730: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176734: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176738: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8017673C: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176740: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80176744: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80176748: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8017674C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80176750: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176754: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80176758: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8017675C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176760: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80176764: lw          $t9, 0x2744($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2744);
    // 0x80176768: jalr        $t9
    // 0x8017676C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8017676C: nop

    after_0:
    // 0x80176770: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176774: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176778: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8017677C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80176780: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176784: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176788: jr          $ra
    // 0x8017678C: nop

    return;
    // 0x8017678C: nop

;}
RECOMP_FUNC void func_80176790(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176790: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80176794: lh          $t6, -0x5470($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5470);
    // 0x80176798: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017679C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801767A0: addiu       $t7, $t6, -0x10
    ctx->r15 = ADD32(ctx->r14, -0X10);
    // 0x801767A4: sltiu       $at, $t7, 0x1B
    ctx->r1 = ctx->r15 < 0X1B ? 1 : 0;
    // 0x801767A8: beq         $at, $zero, L_801768CC
    if (ctx->r1 == 0) {
        // 0x801767AC: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_801768CC;
    }
    // 0x801767AC: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x801767B0: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x801767B4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x801767B8: addu        $at, $at, $t7
    gpr jr_addend_801767C0 = ctx->r15;
    ctx->r1 = ADD32(ctx->r1, ctx->r15);
    // 0x801767BC: lw          $t7, -0x43CC($at)
    ctx->r15 = ADD32(ctx->r1, -0X43CC);
    // 0x801767C0: jr          $t7
    // 0x801767C4: nop

    switch (jr_addend_801767C0 >> 2) {
        case 0: goto L_801767C8; break;
        case 1: goto L_801767DC; break;
        case 2: goto L_801768CC; break;
        case 3: goto L_801768CC; break;
        case 4: goto L_801767F0; break;
        case 5: goto L_801768CC; break;
        case 6: goto L_801768CC; break;
        case 7: goto L_80176804; break;
        case 8: goto L_801768CC; break;
        case 9: goto L_80176818; break;
        case 10: goto L_801768CC; break;
        case 11: goto L_801768CC; break;
        case 12: goto L_801768CC; break;
        case 13: goto L_801768CC; break;
        case 14: goto L_8017682C; break;
        case 15: goto L_80176840; break;
        case 16: goto L_80176854; break;
        case 17: goto L_80176868; break;
        case 18: goto L_8017687C; break;
        case 19: goto L_801768CC; break;
        case 20: goto L_801768CC; break;
        case 21: goto L_801768CC; break;
        case 22: goto L_801768CC; break;
        case 23: goto L_801768CC; break;
        case 24: goto L_80176890; break;
        case 25: goto L_801768A4; break;
        case 26: goto L_801768B8; break;
        default: switch_error(__func__, 0x801767C0, 0x8019BC34);
    }
    // 0x801767C4: nop

L_801767C8:
    // 0x801767C8: lui         $t8, 0x8017
    ctx->r24 = S32(0X8017 << 16);
    // 0x801767CC: addiu       $t8, $t8, 0x6978
    ctx->r24 = ADD32(ctx->r24, 0X6978);
    // 0x801767D0: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801767D4: b           L_801768EC
    // 0x801767D8: sw          $t8, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r24;
        goto L_801768EC;
    // 0x801767D8: sw          $t8, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r24;
L_801767DC:
    // 0x801767DC: lui         $t9, 0x8017
    ctx->r25 = S32(0X8017 << 16);
    // 0x801767E0: addiu       $t9, $t9, 0x6A2C
    ctx->r25 = ADD32(ctx->r25, 0X6A2C);
    // 0x801767E4: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801767E8: b           L_801768EC
    // 0x801767EC: sw          $t9, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r25;
        goto L_801768EC;
    // 0x801767EC: sw          $t9, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r25;
L_801767F0:
    // 0x801767F0: lui         $t0, 0x802E
    ctx->r8 = S32(0X802E << 16);
    // 0x801767F4: addiu       $t0, $t0, 0x4B70
    ctx->r8 = ADD32(ctx->r8, 0X4B70);
    // 0x801767F8: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801767FC: b           L_801768EC
    // 0x80176800: sw          $t0, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r8;
        goto L_801768EC;
    // 0x80176800: sw          $t0, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r8;
L_80176804:
    // 0x80176804: lui         $t1, 0x802E
    ctx->r9 = S32(0X802E << 16);
    // 0x80176808: addiu       $t1, $t1, 0x47B0
    ctx->r9 = ADD32(ctx->r9, 0X47B0);
    // 0x8017680C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80176810: b           L_801768EC
    // 0x80176814: sw          $t1, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r9;
        goto L_801768EC;
    // 0x80176814: sw          $t1, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r9;
L_80176818:
    // 0x80176818: lui         $t2, 0x8017
    ctx->r10 = S32(0X8017 << 16);
    // 0x8017681C: addiu       $t2, $t2, 0x6AE0
    ctx->r10 = ADD32(ctx->r10, 0X6AE0);
    // 0x80176820: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80176824: b           L_801768EC
    // 0x80176828: sw          $t2, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r10;
        goto L_801768EC;
    // 0x80176828: sw          $t2, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r10;
L_8017682C:
    // 0x8017682C: lui         $t3, 0x8017
    ctx->r11 = S32(0X8017 << 16);
    // 0x80176830: addiu       $t3, $t3, 0x6BD0
    ctx->r11 = ADD32(ctx->r11, 0X6BD0);
    // 0x80176834: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80176838: b           L_801768EC
    // 0x8017683C: sw          $t3, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r11;
        goto L_801768EC;
    // 0x8017683C: sw          $t3, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r11;
L_80176840:
    // 0x80176840: lui         $t4, 0x8017
    ctx->r12 = S32(0X8017 << 16);
    // 0x80176844: addiu       $t4, $t4, 0x6BD0
    ctx->r12 = ADD32(ctx->r12, 0X6BD0);
    // 0x80176848: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017684C: b           L_801768EC
    // 0x80176850: sw          $t4, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r12;
        goto L_801768EC;
    // 0x80176850: sw          $t4, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r12;
L_80176854:
    // 0x80176854: lui         $t5, 0x8017
    ctx->r13 = S32(0X8017 << 16);
    // 0x80176858: addiu       $t5, $t5, 0x6BD0
    ctx->r13 = ADD32(ctx->r13, 0X6BD0);
    // 0x8017685C: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80176860: b           L_801768EC
    // 0x80176864: sw          $t5, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r13;
        goto L_801768EC;
    // 0x80176864: sw          $t5, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r13;
L_80176868:
    // 0x80176868: lui         $t6, 0x8017
    ctx->r14 = S32(0X8017 << 16);
    // 0x8017686C: addiu       $t6, $t6, 0x6CAC
    ctx->r14 = ADD32(ctx->r14, 0X6CAC);
    // 0x80176870: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80176874: b           L_801768EC
    // 0x80176878: sw          $t6, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r14;
        goto L_801768EC;
    // 0x80176878: sw          $t6, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r14;
L_8017687C:
    // 0x8017687C: lui         $t7, 0x8017
    ctx->r15 = S32(0X8017 << 16);
    // 0x80176880: addiu       $t7, $t7, 0x6D88
    ctx->r15 = ADD32(ctx->r15, 0X6D88);
    // 0x80176884: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80176888: b           L_801768EC
    // 0x8017688C: sw          $t7, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r15;
        goto L_801768EC;
    // 0x8017688C: sw          $t7, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r15;
L_80176890:
    // 0x80176890: lui         $t8, 0x8017
    ctx->r24 = S32(0X8017 << 16);
    // 0x80176894: addiu       $t8, $t8, 0x6F04
    ctx->r24 = ADD32(ctx->r24, 0X6F04);
    // 0x80176898: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017689C: b           L_801768EC
    // 0x801768A0: sw          $t8, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r24;
        goto L_801768EC;
    // 0x801768A0: sw          $t8, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r24;
L_801768A4:
    // 0x801768A4: lui         $t9, 0x802E
    ctx->r25 = S32(0X802E << 16);
    // 0x801768A8: addiu       $t9, $t9, 0x5430
    ctx->r25 = ADD32(ctx->r25, 0X5430);
    // 0x801768AC: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801768B0: b           L_801768EC
    // 0x801768B4: sw          $t9, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r25;
        goto L_801768EC;
    // 0x801768B4: sw          $t9, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r25;
L_801768B8:
    // 0x801768B8: lui         $t0, 0x802E
    ctx->r8 = S32(0X802E << 16);
    // 0x801768BC: addiu       $t0, $t0, 0x7A60
    ctx->r8 = ADD32(ctx->r8, 0X7A60);
    // 0x801768C0: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801768C4: b           L_801768EC
    // 0x801768C8: sw          $t0, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r8;
        goto L_801768EC;
    // 0x801768C8: sw          $t0, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = ctx->r8;
L_801768CC:
    // 0x801768CC: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801768D0: sw          $zero, 0x2740($at)
    MEM_W(0X2740, ctx->r1) = 0;
    // 0x801768D4: lw          $t9, 0x10($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X10);
    // 0x801768D8: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x801768DC: jalr        $t9
    // 0x801768E0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801768E0: nop

    after_0:
    // 0x801768E4: b           L_80176944
    // 0x801768E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80176944;
    // 0x801768E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801768EC:
    // 0x801768EC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801768F0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801768F4: addiu       $a0, $a2, 0x8
    ctx->r4 = ADD32(ctx->r6, 0X8);
    // 0x801768F8: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x801768FC: jalr        $t9
    // 0x80176900: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176900: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_1:
    // 0x80176904: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176908: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x8017690C: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176910: sll         $t2, $t1, 1
    ctx->r10 = S32(ctx->r9 << 1);
    // 0x80176914: addu        $v0, $a0, $t2
    ctx->r2 = ADD32(ctx->r4, ctx->r10);
    // 0x80176918: lbu         $t5, 0x9($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X9);
    // 0x8017691C: lbu         $t3, 0x8($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X8);
    // 0x80176920: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80176924: sll         $t6, $t5, 2
    ctx->r14 = S32(ctx->r13 << 2);
    // 0x80176928: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x8017692C: sb          $t4, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r12;
    // 0x80176930: addu        $t9, $t9, $t6
    ctx->r25 = ADD32(ctx->r25, ctx->r14);
    // 0x80176934: lw          $t9, 0x2744($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2744);
    // 0x80176938: jalr        $t9
    // 0x8017693C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8017693C: nop

    after_2:
    // 0x80176940: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80176944:
    // 0x80176944: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176948: jr          $ra
    // 0x8017694C: nop

    return;
    // 0x8017694C: nop

;}
RECOMP_FUNC void func_80176950(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176950: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176954: lw          $t9, 0x2740($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2740);
    // 0x80176958: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017695C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176960: jalr        $t9
    // 0x80176964: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176964: nop

    after_0:
    // 0x80176968: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017696C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176970: jr          $ra
    // 0x80176974: nop

    return;
    // 0x80176974: nop

;}
RECOMP_FUNC void func_80176978(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176978: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017697C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176980: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176984: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176988: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8017698C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176990: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176994: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176998: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8017699C: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801769A0: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801769A4: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801769A8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801769AC: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801769B0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801769B4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801769B8: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801769BC: lw          $t9, 0x274C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X274C);
    // 0x801769C0: jalr        $t9
    // 0x801769C4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801769C4: nop

    after_0:
    // 0x801769C8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801769CC: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801769D0: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801769D4: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801769D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801769DC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801769E0: jr          $ra
    // 0x801769E4: nop

    return;
    // 0x801769E4: nop

;}
RECOMP_FUNC void func_801769E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801769E8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801769EC: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x801769F0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801769F4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801769F8: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x801769FC: jalr        $t9
    // 0x80176A00: addiu       $a0, $zero, 0x407
    ctx->r4 = ADD32(0, 0X407);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176A00: addiu       $a0, $zero, 0x407
    ctx->r4 = ADD32(0, 0X407);
    after_0:
    // 0x80176A04: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80176A08: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80176A0C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80176A10: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80176A14: jalr        $t9
    // 0x80176A18: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176A18: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x80176A1C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176A20: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176A24: jr          $ra
    // 0x80176A28: nop

    return;
    // 0x80176A28: nop

;}
RECOMP_FUNC void func_80176A2C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176A2C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176A30: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176A34: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176A38: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176A3C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176A40: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176A44: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176A48: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176A4C: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80176A50: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80176A54: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80176A58: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80176A5C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176A60: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80176A64: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80176A68: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176A6C: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80176A70: lw          $t9, 0x2754($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2754);
    // 0x80176A74: jalr        $t9
    // 0x80176A78: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176A78: nop

    after_0:
    // 0x80176A7C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176A80: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176A84: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80176A88: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80176A8C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176A90: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176A94: jr          $ra
    // 0x80176A98: nop

    return;
    // 0x80176A98: nop

;}
RECOMP_FUNC void func_80176A9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176A9C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176AA0: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176AA4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176AA8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176AAC: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176AB0: jalr        $t9
    // 0x80176AB4: addiu       $a0, $zero, 0x408
    ctx->r4 = ADD32(0, 0X408);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176AB4: addiu       $a0, $zero, 0x408
    ctx->r4 = ADD32(0, 0X408);
    after_0:
    // 0x80176AB8: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80176ABC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80176AC0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80176AC4: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80176AC8: jalr        $t9
    // 0x80176ACC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176ACC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x80176AD0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176AD4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176AD8: jr          $ra
    // 0x80176ADC: nop

    return;
    // 0x80176ADC: nop

;}
RECOMP_FUNC void func_80176AE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176AE0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176AE4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176AE8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176AEC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176AF0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176AF4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176AF8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176AFC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176B00: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80176B04: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80176B08: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80176B0C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80176B10: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176B14: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80176B18: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80176B1C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176B20: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80176B24: lw          $t9, 0x275C($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X275C);
    // 0x80176B28: jalr        $t9
    // 0x80176B2C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176B2C: nop

    after_0:
    // 0x80176B30: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176B34: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176B38: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80176B3C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80176B40: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176B44: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176B48: jr          $ra
    // 0x80176B4C: nop

    return;
    // 0x80176B4C: nop

;}
RECOMP_FUNC void func_80176B50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176B50: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80176B54: lw          $t6, -0x5178($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5178);
    // 0x80176B58: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176B5C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176B60: bne         $t6, $zero, L_80176B7C
    if (ctx->r14 != 0) {
        // 0x80176B64: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_80176B7C;
    }
    // 0x80176B64: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x80176B68: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80176B6C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80176B70: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x80176B74: jalr        $t9
    // 0x80176B78: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176B78: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    after_0:
L_80176B7C:
    // 0x80176B7C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176B80: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176B84: jr          $ra
    // 0x80176B88: nop

    return;
    // 0x80176B88: nop

;}
RECOMP_FUNC void func_80176B8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176B8C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176B90: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176B94: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176B98: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176B9C: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176BA0: jalr        $t9
    // 0x80176BA4: addiu       $a0, $zero, 0x1ED
    ctx->r4 = ADD32(0, 0X1ED);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176BA4: addiu       $a0, $zero, 0x1ED
    ctx->r4 = ADD32(0, 0X1ED);
    after_0:
    // 0x80176BA8: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80176BAC: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80176BB0: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80176BB4: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80176BB8: jalr        $t9
    // 0x80176BBC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176BBC: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_1:
    // 0x80176BC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176BC4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176BC8: jr          $ra
    // 0x80176BCC: nop

    return;
    // 0x80176BCC: nop

;}
RECOMP_FUNC void func_80176BD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176BD0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176BD4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176BD8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176BDC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176BE0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176BE4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176BE8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176BEC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176BF0: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80176BF4: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80176BF8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80176BFC: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80176C00: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176C04: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80176C08: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80176C0C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176C10: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80176C14: lw          $t9, 0x2768($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2768);
    // 0x80176C18: jalr        $t9
    // 0x80176C1C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176C1C: nop

    after_0:
    // 0x80176C20: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176C24: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176C28: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80176C2C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80176C30: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176C34: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176C38: jr          $ra
    // 0x80176C3C: nop

    return;
    // 0x80176C3C: nop

;}
RECOMP_FUNC void func_80176C40(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176C40: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176C44: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176C48: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176C4C: jal         0x80015170
    // 0x80176C50: addiu       $a0, $zero, 0x1BB
    ctx->r4 = ADD32(0, 0X1BB);
    func_80015170(rdram, ctx);
        goto after_0;
    // 0x80176C50: addiu       $a0, $zero, 0x1BB
    ctx->r4 = ADD32(0, 0X1BB);
    after_0:
    // 0x80176C54: bne         $v0, $zero, L_80176C68
    if (ctx->r2 != 0) {
        // 0x80176C58: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176C68;
    }
    // 0x80176C58: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176C5C: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176C60: jalr        $t9
    // 0x80176C64: addiu       $a0, $zero, 0x1BB
    ctx->r4 = ADD32(0, 0X1BB);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176C64: addiu       $a0, $zero, 0x1BB
    ctx->r4 = ADD32(0, 0X1BB);
    after_1:
L_80176C68:
    // 0x80176C68: jal         0x80015170
    // 0x80176C6C: addiu       $a0, $zero, 0x369
    ctx->r4 = ADD32(0, 0X369);
    func_80015170(rdram, ctx);
        goto after_2;
    // 0x80176C6C: addiu       $a0, $zero, 0x369
    ctx->r4 = ADD32(0, 0X369);
    after_2:
    // 0x80176C70: bne         $v0, $zero, L_80176C84
    if (ctx->r2 != 0) {
        // 0x80176C74: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176C84;
    }
    // 0x80176C74: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176C78: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176C7C: jalr        $t9
    // 0x80176C80: addiu       $a0, $zero, 0x369
    ctx->r4 = ADD32(0, 0X369);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80176C80: addiu       $a0, $zero, 0x369
    ctx->r4 = ADD32(0, 0X369);
    after_3:
L_80176C84:
    // 0x80176C84: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80176C88: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80176C8C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80176C90: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80176C94: jalr        $t9
    // 0x80176C98: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80176C98: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_4:
    // 0x80176C9C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176CA0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176CA4: jr          $ra
    // 0x80176CA8: nop

    return;
    // 0x80176CA8: nop

;}
RECOMP_FUNC void func_80176CAC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176CAC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176CB0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176CB4: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176CB8: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176CBC: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176CC0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176CC4: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176CC8: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176CCC: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80176CD0: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80176CD4: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80176CD8: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80176CDC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176CE0: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80176CE4: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80176CE8: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176CEC: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80176CF0: lw          $t9, 0x2770($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2770);
    // 0x80176CF4: jalr        $t9
    // 0x80176CF8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176CF8: nop

    after_0:
    // 0x80176CFC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176D00: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176D04: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80176D08: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80176D0C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176D10: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176D14: jr          $ra
    // 0x80176D18: nop

    return;
    // 0x80176D18: nop

;}
RECOMP_FUNC void func_80176D1C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176D1C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176D20: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176D24: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176D28: jal         0x80015170
    // 0x80176D2C: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    func_80015170(rdram, ctx);
        goto after_0;
    // 0x80176D2C: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    after_0:
    // 0x80176D30: bne         $v0, $zero, L_80176D44
    if (ctx->r2 != 0) {
        // 0x80176D34: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176D44;
    }
    // 0x80176D34: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176D38: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176D3C: jalr        $t9
    // 0x80176D40: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176D40: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    after_1:
L_80176D44:
    // 0x80176D44: jal         0x80015170
    // 0x80176D48: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    func_80015170(rdram, ctx);
        goto after_2;
    // 0x80176D48: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    after_2:
    // 0x80176D4C: bne         $v0, $zero, L_80176D60
    if (ctx->r2 != 0) {
        // 0x80176D50: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176D60;
    }
    // 0x80176D50: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176D54: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176D58: jalr        $t9
    // 0x80176D5C: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80176D5C: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    after_3:
L_80176D60:
    // 0x80176D60: lw          $v0, 0x18($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X18);
    // 0x80176D64: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80176D68: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80176D6C: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x80176D70: jalr        $t9
    // 0x80176D74: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x80176D74: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_4:
    // 0x80176D78: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176D7C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176D80: jr          $ra
    // 0x80176D84: nop

    return;
    // 0x80176D84: nop

;}
RECOMP_FUNC void func_80176D88(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176D88: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176D8C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176D90: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176D94: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176D98: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176D9C: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176DA0: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176DA4: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176DA8: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80176DAC: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80176DB0: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80176DB4: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80176DB8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176DBC: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80176DC0: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80176DC4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176DC8: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80176DCC: lw          $t9, 0x2778($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2778);
    // 0x80176DD0: jalr        $t9
    // 0x80176DD4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176DD4: nop

    after_0:
    // 0x80176DD8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176DDC: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176DE0: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80176DE4: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80176DE8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176DEC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176DF0: jr          $ra
    // 0x80176DF4: nop

    return;
    // 0x80176DF4: nop

;}
RECOMP_FUNC void func_80176DF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176DF8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80176DFC: lw          $v0, -0x53E0($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X53E0);
    // 0x80176E00: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176E04: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176E08: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176E0C: lwc1        $f4, 0x50($v0)
    ctx->f4.u32l = MEM_W(ctx->r2, 0X50);
    // 0x80176E10: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80176E14: ldc1        $f6, -0x4360($at)
    CHECK_FR(ctx, 6);
    ctx->f6.u64 = LD(ctx->r1, -0X4360);
    // 0x80176E18: cvt.d.s     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f0.d = CVT_D_S(ctx->f4.fl);
    // 0x80176E1C: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80176E20: c.lt.d      $f0, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    c1cs = ctx->f0.d < ctx->f6.d;
    // 0x80176E24: nop

    // 0x80176E28: bc1f        L_80176EBC
    if (!c1cs) {
        // 0x80176E2C: nop
    
            goto L_80176EBC;
    }
    // 0x80176E2C: nop

    // 0x80176E30: ldc1        $f8, -0x4358($at)
    CHECK_FR(ctx, 8);
    ctx->f8.u64 = LD(ctx->r1, -0X4358);
    // 0x80176E34: c.lt.d      $f8, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f8.d < ctx->f0.d;
    // 0x80176E38: nop

    // 0x80176E3C: bc1f        L_80176EBC
    if (!c1cs) {
        // 0x80176E40: nop
    
            goto L_80176EBC;
    }
    // 0x80176E40: nop

    // 0x80176E44: lwc1        $f10, 0x58($v0)
    ctx->f10.u32l = MEM_W(ctx->r2, 0X58);
    // 0x80176E48: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80176E4C: ldc1        $f16, -0x4350($at)
    CHECK_FR(ctx, 16);
    ctx->f16.u64 = LD(ctx->r1, -0X4350);
    // 0x80176E50: cvt.d.s     $f0, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.fl); 
    ctx->f0.d = CVT_D_S(ctx->f10.fl);
    // 0x80176E54: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80176E58: c.lt.d      $f0, $f16
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    c1cs = ctx->f0.d < ctx->f16.d;
    // 0x80176E5C: nop

    // 0x80176E60: bc1f        L_80176EBC
    if (!c1cs) {
        // 0x80176E64: nop
    
            goto L_80176EBC;
    }
    // 0x80176E64: nop

    // 0x80176E68: ldc1        $f18, -0x4348($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, -0X4348);
    // 0x80176E6C: c.lt.d      $f18, $f0
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f18.d < ctx->f0.d;
    // 0x80176E70: nop

    // 0x80176E74: bc1f        L_80176EBC
    if (!c1cs) {
        // 0x80176E78: nop
    
            goto L_80176EBC;
    }
    // 0x80176E78: nop

    // 0x80176E7C: jal         0x80015170
    // 0x80176E80: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    func_80015170(rdram, ctx);
        goto after_0;
    // 0x80176E80: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    after_0:
    // 0x80176E84: bne         $v0, $zero, L_80176E98
    if (ctx->r2 != 0) {
        // 0x80176E88: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176E98;
    }
    // 0x80176E88: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176E8C: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176E90: jalr        $t9
    // 0x80176E94: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176E94: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    after_1:
L_80176E98:
    // 0x80176E98: jal         0x80015170
    // 0x80176E9C: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    func_80015170(rdram, ctx);
        goto after_2;
    // 0x80176E9C: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    after_2:
    // 0x80176EA0: bne         $v0, $zero, L_80176EF4
    if (ctx->r2 != 0) {
        // 0x80176EA4: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176EF4;
    }
    // 0x80176EA4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176EA8: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176EAC: jalr        $t9
    // 0x80176EB0: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80176EB0: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    after_3:
    // 0x80176EB4: b           L_80176EF8
    // 0x80176EB8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80176EF8;
    // 0x80176EB8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80176EBC:
    // 0x80176EBC: jal         0x80015170
    // 0x80176EC0: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    func_80015170(rdram, ctx);
        goto after_4;
    // 0x80176EC0: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    after_4:
    // 0x80176EC4: beq         $v0, $zero, L_80176ED8
    if (ctx->r2 == 0) {
        // 0x80176EC8: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176ED8;
    }
    // 0x80176EC8: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176ECC: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176ED0: jalr        $t9
    // 0x80176ED4: ori         $a0, $zero, 0x8188
    ctx->r4 = 0 | 0X8188;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x80176ED4: ori         $a0, $zero, 0x8188
    ctx->r4 = 0 | 0X8188;
    after_5:
L_80176ED8:
    // 0x80176ED8: jal         0x80015170
    // 0x80176EDC: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    func_80015170(rdram, ctx);
        goto after_6;
    // 0x80176EDC: addiu       $a0, $zero, 0x368
    ctx->r4 = ADD32(0, 0X368);
    after_6:
    // 0x80176EE0: beq         $v0, $zero, L_80176EF4
    if (ctx->r2 == 0) {
        // 0x80176EE4: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176EF4;
    }
    // 0x80176EE4: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176EE8: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176EEC: jalr        $t9
    // 0x80176EF0: ori         $a0, $zero, 0x8368
    ctx->r4 = 0 | 0X8368;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80176EF0: ori         $a0, $zero, 0x8368
    ctx->r4 = 0 | 0X8368;
    after_7:
L_80176EF4:
    // 0x80176EF4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80176EF8:
    // 0x80176EF8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176EFC: jr          $ra
    // 0x80176F00: nop

    return;
    // 0x80176F00: nop

;}
RECOMP_FUNC void func_80176F04(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176F04: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176F08: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176F0C: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176F10: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176F14: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176F18: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176F1C: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176F20: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80176F24: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80176F28: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80176F2C: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80176F30: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80176F34: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176F38: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80176F3C: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x80176F40: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80176F44: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80176F48: lw          $t9, 0x2780($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2780);
    // 0x80176F4C: jalr        $t9
    // 0x80176F50: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80176F50: nop

    after_0:
    // 0x80176F54: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80176F58: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80176F5C: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80176F60: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80176F64: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176F68: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176F6C: jr          $ra
    // 0x80176F70: nop

    return;
    // 0x80176F70: nop

;}
RECOMP_FUNC void func_80176F74(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176F74: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176F78: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176F7C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80176F80: jal         0x80015170
    // 0x80176F84: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    func_80015170(rdram, ctx);
        goto after_0;
    // 0x80176F84: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    after_0:
    // 0x80176F88: bne         $v0, $zero, L_80176F9C
    if (ctx->r2 != 0) {
        // 0x80176F8C: lui         $t9, 0x8001
        ctx->r25 = S32(0X8001 << 16);
            goto L_80176F9C;
    }
    // 0x80176F8C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176F90: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176F94: jalr        $t9
    // 0x80176F98: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x80176F98: addiu       $a0, $zero, 0x188
    ctx->r4 = ADD32(0, 0X188);
    after_1:
L_80176F9C:
    // 0x80176F9C: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x80176FA0: addiu       $t9, $t9, 0x66F8
    ctx->r25 = ADD32(ctx->r25, 0X66F8);
    // 0x80176FA4: jalr        $t9
    // 0x80176FA8: addiu       $a0, $zero, 0x3EC
    ctx->r4 = ADD32(0, 0X3EC);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80176FA8: addiu       $a0, $zero, 0x3EC
    ctx->r4 = ADD32(0, 0X3EC);
    after_2:
    // 0x80176FAC: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80176FB0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80176FB4: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80176FB8: addiu       $a0, $v1, 0x8
    ctx->r4 = ADD32(ctx->r3, 0X8);
    // 0x80176FBC: jalr        $t9
    // 0x80176FC0: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x80176FC0: addiu       $a1, $v1, 0xE
    ctx->r5 = ADD32(ctx->r3, 0XE);
    after_3:
    // 0x80176FC4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80176FC8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80176FCC: jr          $ra
    // 0x80176FD0: nop

    return;
    // 0x80176FD0: nop

;}
RECOMP_FUNC void func_80176FE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80176FE0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80176FE4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80176FE8: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80176FEC: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80176FF0: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80176FF4: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80176FF8: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80176FFC: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x80177000: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x80177004: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x80177008: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8017700C: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x80177010: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80177014: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x80177018: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8017701C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80177020: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x80177024: lw          $t9, 0x2794($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2794);
    // 0x80177028: jalr        $t9
    // 0x8017702C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8017702C: nop

    after_0:
    // 0x80177030: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80177034: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x80177038: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8017703C: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x80177040: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80177044: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80177048: jr          $ra
    // 0x8017704C: nop

    return;
    // 0x8017704C: nop

;}
RECOMP_FUNC void func_80177050(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177050: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80177054: lhu         $t6, -0x5470($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X5470);
    // 0x80177058: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8017705C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80177060: sltiu       $at, $t6, 0x7
    ctx->r1 = ctx->r14 < 0X7 ? 1 : 0;
    // 0x80177064: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80177068: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017706C: beq         $at, $zero, L_801770F0
    if (ctx->r1 == 0) {
        // 0x80177070: sw          $s0, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r16;
            goto L_801770F0;
    }
    // 0x80177070: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80177074: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80177078: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017707C: addu        $at, $at, $t6
    gpr jr_addend_80177084 = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x80177080: lw          $t6, -0x42FC($at)
    ctx->r14 = ADD32(ctx->r1, -0X42FC);
    // 0x80177084: jr          $t6
    // 0x80177088: nop

    switch (jr_addend_80177084 >> 2) {
        case 0: goto L_8017708C; break;
        case 1: goto L_801770F0; break;
        case 2: goto L_801770F0; break;
        case 3: goto L_801770A0; break;
        case 4: goto L_801770B4; break;
        case 5: goto L_801770C8; break;
        case 6: goto L_801770DC; break;
        default: switch_error(__func__, 0x80177084, 0x8019BD04);
    }
    // 0x80177088: nop

L_8017708C:
    // 0x8017708C: lui         $t7, 0x802F
    ctx->r15 = S32(0X802F << 16);
    // 0x80177090: addiu       $t7, $t7, -0x5B18
    ctx->r15 = ADD32(ctx->r15, -0X5B18);
    // 0x80177094: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80177098: b           L_80177150
    // 0x8017709C: sw          $t7, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r15;
        goto L_80177150;
    // 0x8017709C: sw          $t7, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r15;
L_801770A0:
    // 0x801770A0: lui         $t8, 0x802E
    ctx->r24 = S32(0X802E << 16);
    // 0x801770A4: addiu       $t8, $t8, 0x4528
    ctx->r24 = ADD32(ctx->r24, 0X4528);
    // 0x801770A8: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801770AC: b           L_80177150
    // 0x801770B0: sw          $t8, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r24;
        goto L_80177150;
    // 0x801770B0: sw          $t8, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r24;
L_801770B4:
    // 0x801770B4: lui         $t9, 0x802E
    ctx->r25 = S32(0X802E << 16);
    // 0x801770B8: addiu       $t9, $t9, 0x4EDC
    ctx->r25 = ADD32(ctx->r25, 0X4EDC);
    // 0x801770BC: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801770C0: b           L_80177150
    // 0x801770C4: sw          $t9, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r25;
        goto L_80177150;
    // 0x801770C4: sw          $t9, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r25;
L_801770C8:
    // 0x801770C8: lui         $t0, 0x802E
    ctx->r8 = S32(0X802E << 16);
    // 0x801770CC: addiu       $t0, $t0, 0x5BA4
    ctx->r8 = ADD32(ctx->r8, 0X5BA4);
    // 0x801770D0: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801770D4: b           L_80177150
    // 0x801770D8: sw          $t0, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r8;
        goto L_80177150;
    // 0x801770D8: sw          $t0, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r8;
L_801770DC:
    // 0x801770DC: lui         $t1, 0x802E
    ctx->r9 = S32(0X802E << 16);
    // 0x801770E0: addiu       $t1, $t1, 0x4010
    ctx->r9 = ADD32(ctx->r9, 0X4010);
    // 0x801770E4: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801770E8: b           L_80177150
    // 0x801770EC: sw          $t1, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r9;
        goto L_80177150;
    // 0x801770EC: sw          $t1, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = ctx->r9;
L_801770F0:
    // 0x801770F0: lui         $s0, 0x8001
    ctx->r16 = S32(0X8001 << 16);
    // 0x801770F4: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801770F8: addiu       $s0, $s0, 0x66F8
    ctx->r16 = ADD32(ctx->r16, 0X66F8);
    // 0x801770FC: sw          $zero, 0x2790($at)
    MEM_W(0X2790, ctx->r1) = 0;
    // 0x80177100: jalr        $s0
    // 0x80177104: ori         $a0, $zero, 0x8133
    ctx->r4 = 0 | 0X8133;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_0;
    // 0x80177104: ori         $a0, $zero, 0x8133
    ctx->r4 = 0 | 0X8133;
    after_0:
    // 0x80177108: jalr        $s0
    // 0x8017710C: ori         $a0, $zero, 0x8134
    ctx->r4 = 0 | 0X8134;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_1;
    // 0x8017710C: ori         $a0, $zero, 0x8134
    ctx->r4 = 0 | 0X8134;
    after_1:
    // 0x80177110: jalr        $s0
    // 0x80177114: ori         $a0, $zero, 0x8132
    ctx->r4 = 0 | 0X8132;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_2;
    // 0x80177114: ori         $a0, $zero, 0x8132
    ctx->r4 = 0 | 0X8132;
    after_2:
    // 0x80177118: jalr        $s0
    // 0x8017711C: ori         $a0, $zero, 0x83E9
    ctx->r4 = 0 | 0X83E9;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_3;
    // 0x8017711C: ori         $a0, $zero, 0x83E9
    ctx->r4 = 0 | 0X83E9;
    after_3:
    // 0x80177120: jalr        $s0
    // 0x80177124: ori         $a0, $zero, 0x83EA
    ctx->r4 = 0 | 0X83EA;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_4;
    // 0x80177124: ori         $a0, $zero, 0x83EA
    ctx->r4 = 0 | 0X83EA;
    after_4:
    // 0x80177128: jalr        $s0
    // 0x8017712C: ori         $a0, $zero, 0x83E6
    ctx->r4 = 0 | 0X83E6;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_5;
    // 0x8017712C: ori         $a0, $zero, 0x83E6
    ctx->r4 = 0 | 0X83E6;
    after_5:
    // 0x80177130: jalr        $s0
    // 0x80177134: ori         $a0, $zero, 0x83E7
    ctx->r4 = 0 | 0X83E7;
    LOOKUP_FUNC(ctx->r16)(rdram, ctx);
        goto after_6;
    // 0x80177134: ori         $a0, $zero, 0x83E7
    ctx->r4 = 0 | 0X83E7;
    after_6:
    // 0x80177138: lw          $t9, 0x10($s1)
    ctx->r25 = MEM_W(ctx->r17, 0X10);
    // 0x8017713C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177140: jalr        $t9
    // 0x80177144: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x80177144: nop

    after_7:
    // 0x80177148: b           L_801771A4
    // 0x8017714C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_801771A4;
    // 0x8017714C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80177150:
    // 0x80177150: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80177154: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x80177158: addiu       $a0, $s1, 0x8
    ctx->r4 = ADD32(ctx->r17, 0X8);
    // 0x8017715C: jalr        $t9
    // 0x80177160: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x80177160: addiu       $a1, $s1, 0xE
    ctx->r5 = ADD32(ctx->r17, 0XE);
    after_8:
    // 0x80177164: lh          $t2, 0xE($s1)
    ctx->r10 = MEM_H(ctx->r17, 0XE);
    // 0x80177168: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x8017716C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177170: sll         $t3, $t2, 1
    ctx->r11 = S32(ctx->r10 << 1);
    // 0x80177174: addu        $v0, $s1, $t3
    ctx->r2 = ADD32(ctx->r17, ctx->r11);
    // 0x80177178: lbu         $t6, 0x9($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X9);
    // 0x8017717C: lbu         $t4, 0x8($v0)
    ctx->r12 = MEM_BU(ctx->r2, 0X8);
    // 0x80177180: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80177184: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80177188: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x8017718C: sb          $t5, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r13;
    // 0x80177190: addu        $t9, $t9, $t7
    ctx->r25 = ADD32(ctx->r25, ctx->r15);
    // 0x80177194: lw          $t9, 0x2794($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2794);
    // 0x80177198: jalr        $t9
    // 0x8017719C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x8017719C: nop

    after_9:
    // 0x801771A0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_801771A4:
    // 0x801771A4: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x801771A8: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801771AC: jr          $ra
    // 0x801771B0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x801771B0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_801771B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801771B4: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801771B8: lw          $t9, 0x2790($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X2790);
    // 0x801771BC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x801771C0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801771C4: jalr        $t9
    // 0x801771C8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801771C8: nop

    after_0:
    // 0x801771CC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801771D0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801771D4: jr          $ra
    // 0x801771D8: nop

    return;
    // 0x801771D8: nop

;}
RECOMP_FUNC void func_801771E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801771E0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801771E4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801771E8: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x801771EC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x801771F0: addiu       $a0, $v0, 0x24
    ctx->r4 = ADD32(ctx->r2, 0X24);
    // 0x801771F4: jal         0x80013CF8
    // 0x801771F8: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x801771F8: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x801771FC: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80177200: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80177204: jal         0x8017DF5C
    // 0x80177208: lw          $a0, 0x10($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X10);
    func_8017DF5C(rdram, ctx);
        goto after_1;
    // 0x80177208: lw          $a0, 0x10($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X10);
    after_1:
    // 0x8017720C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80177210: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80177214: jr          $ra
    // 0x80177218: nop

    return;
    // 0x80177218: nop

;}
RECOMP_FUNC void func_8017721C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017721C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80177220: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80177224: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80177228: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x8017722C: addiu       $a0, $v0, 0x24
    ctx->r4 = ADD32(ctx->r2, 0X24);
    // 0x80177230: jal         0x80013CF8
    // 0x80177234: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x80177234: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x80177238: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x8017723C: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80177240: jal         0x8017DFE8
    // 0x80177244: lw          $a0, 0x10($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X10);
    func_8017DFE8(rdram, ctx);
        goto after_1;
    // 0x80177244: lw          $a0, 0x10($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X10);
    after_1:
    // 0x80177248: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017724C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80177250: jr          $ra
    // 0x80177254: nop

    return;
    // 0x80177254: nop

;}
RECOMP_FUNC void func_80177258(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177258: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017725C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80177260: sltiu       $at, $a1, 0x34
    ctx->r1 = ctx->r5 < 0X34 ? 1 : 0;
    // 0x80177264: beq         $at, $zero, L_8017744C
    if (ctx->r1 == 0) {
        // 0x80177268: lw          $v0, 0x34($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X34);
            goto L_8017744C;
    }
    // 0x80177268: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x8017726C: sll         $t6, $a1, 2
    ctx->r14 = S32(ctx->r5 << 2);
    // 0x80177270: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80177274: addu        $at, $at, $t6
    gpr jr_addend_8017727C = ctx->r14;
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x80177278: lw          $t6, -0x42D4($at)
    ctx->r14 = ADD32(ctx->r1, -0X42D4);
    // 0x8017727C: jr          $t6
    // 0x80177280: nop

    switch (jr_addend_8017727C >> 2) {
        case 0: goto L_80177454; break;
        case 1: goto L_80177454; break;
        case 2: goto L_80177454; break;
        case 3: goto L_80177454; break;
        case 4: goto L_8017744C; break;
        case 5: goto L_8017744C; break;
        case 6: goto L_8017744C; break;
        case 7: goto L_8017744C; break;
        case 8: goto L_801772CC; break;
        case 9: goto L_801772DC; break;
        case 10: goto L_801772EC; break;
        case 11: goto L_801772FC; break;
        case 12: goto L_8017730C; break;
        case 13: goto L_8017731C; break;
        case 14: goto L_8017744C; break;
        case 15: goto L_8017744C; break;
        case 16: goto L_8017732C; break;
        case 17: goto L_8017733C; break;
        case 18: goto L_8017734C; break;
        case 19: goto L_8017735C; break;
        case 20: goto L_8017736C; break;
        case 21: goto L_8017737C; break;
        case 22: goto L_8017738C; break;
        case 23: goto L_8017739C; break;
        case 24: goto L_801773AC; break;
        case 25: goto L_801773BC; break;
        case 26: goto L_801773CC; break;
        case 27: goto L_801773DC; break;
        case 28: goto L_801773EC; break;
        case 29: goto L_801773FC; break;
        case 30: goto L_8017740C; break;
        case 31: goto L_8017741C; break;
        case 32: goto L_8017742C; break;
        case 33: goto L_8017743C; break;
        case 34: goto L_80177454; break;
        case 35: goto L_8017744C; break;
        case 36: goto L_8017744C; break;
        case 37: goto L_8017744C; break;
        case 38: goto L_8017744C; break;
        case 39: goto L_8017744C; break;
        case 40: goto L_8017744C; break;
        case 41: goto L_8017744C; break;
        case 42: goto L_8017744C; break;
        case 43: goto L_8017744C; break;
        case 44: goto L_8017744C; break;
        case 45: goto L_8017744C; break;
        case 46: goto L_8017744C; break;
        case 47: goto L_8017744C; break;
        case 48: goto L_80177284; break;
        case 49: goto L_801772A0; break;
        case 50: goto L_801772BC; break;
        case 51: goto L_80177454; break;
        default: switch_error(__func__, 0x8017727C, 0x8019BD2C);
    }
    // 0x80177280: nop

L_80177284:
    // 0x80177284: lw          $t8, 0x10($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X10);
    // 0x80177288: addiu       $t7, $zero, 0x4
    ctx->r15 = ADD32(0, 0X4);
    // 0x8017728C: lw          $t9, 0x34($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X34);
    // 0x80177290: jal         0x8017BE80
    // 0x80177294: sw          $t7, 0x0($t9)
    MEM_W(0X0, ctx->r25) = ctx->r15;
    func_8017BE80(rdram, ctx);
        goto after_0;
    // 0x80177294: sw          $t7, 0x0($t9)
    MEM_W(0X0, ctx->r25) = ctx->r15;
    after_0:
    // 0x80177298: b           L_80177458
    // 0x8017729C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x8017729C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801772A0:
    // 0x801772A0: lw          $t1, 0x10($v0)
    ctx->r9 = MEM_W(ctx->r2, 0X10);
    // 0x801772A4: addiu       $t0, $zero, 0x3
    ctx->r8 = ADD32(0, 0X3);
    // 0x801772A8: lw          $t2, 0x34($t1)
    ctx->r10 = MEM_W(ctx->r9, 0X34);
    // 0x801772AC: jal         0x8017BFFC
    // 0x801772B0: sw          $t0, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->r8;
    func_8017BFFC(rdram, ctx);
        goto after_1;
    // 0x801772B0: sw          $t0, 0x0($t2)
    MEM_W(0X0, ctx->r10) = ctx->r8;
    after_1:
    // 0x801772B4: b           L_80177458
    // 0x801772B8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801772B8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801772BC:
    // 0x801772BC: jal         0x8017C9B8
    // 0x801772C0: nop

    func_8017C9B8(rdram, ctx);
        goto after_2;
    // 0x801772C0: nop

    after_2:
    // 0x801772C4: b           L_80177458
    // 0x801772C8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801772C8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801772CC:
    // 0x801772CC: jal         0x802E4CF8
    // 0x801772D0: nop

    LOOKUP_FUNC(0x802E4CF8)(rdram, ctx);
        goto after_3;
    // 0x801772D0: nop

    after_3:
    // 0x801772D4: b           L_80177458
    // 0x801772D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801772D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801772DC:
    // 0x801772DC: jal         0x802E54DC
    // 0x801772E0: nop

    LOOKUP_FUNC(0x802E54DC)(rdram, ctx);
        goto after_4;
    // 0x801772E0: nop

    after_4:
    // 0x801772E4: b           L_80177458
    // 0x801772E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801772E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801772EC:
    // 0x801772EC: jal         0x802E59CC
    // 0x801772F0: nop

    map_ovl_09_func_802E59CC(rdram, ctx);
        goto after_5;
    // 0x801772F0: nop

    after_5:
    // 0x801772F4: b           L_80177458
    // 0x801772F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801772F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801772FC:
    // 0x801772FC: jal         0x802E59F0
    // 0x80177300: nop

    map_ovl_20_func_802E59F0(rdram, ctx);
        goto after_6;
    // 0x80177300: nop

    after_6:
    // 0x80177304: b           L_80177458
    // 0x80177308: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177308: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017730C:
    // 0x8017730C: jal         0x802E5FAC
    // 0x80177310: nop

    LOOKUP_FUNC(0x802E5FAC)(rdram, ctx);
        goto after_7;
    // 0x80177310: nop

    after_7:
    // 0x80177314: b           L_80177458
    // 0x80177318: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177318: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017731C:
    // 0x8017731C: jal         0x802E3BDC
    // 0x80177320: nop

    map_ovl_27_func_802E3BDC(rdram, ctx);
        goto after_8;
    // 0x80177320: nop

    after_8:
    // 0x80177324: b           L_80177458
    // 0x80177328: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177328: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017732C:
    // 0x8017732C: jal         0x802E3E9C
    // 0x80177330: nop

    map_ovl_24_func_802E3E9C(rdram, ctx);
        goto after_9;
    // 0x80177330: nop

    after_9:
    // 0x80177334: b           L_80177458
    // 0x80177338: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177338: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017733C:
    // 0x8017733C: jal         0x802EAE50
    // 0x80177340: nop

    map_ovl_00_func_802EAE50(rdram, ctx);
        goto after_10;
    // 0x80177340: nop

    after_10:
    // 0x80177344: b           L_80177458
    // 0x80177348: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177348: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017734C:
    // 0x8017734C: jal         0x802E5B80
    // 0x80177350: nop

    map_ovl_02_func_802E5B80(rdram, ctx);
        goto after_11;
    // 0x80177350: nop

    after_11:
    // 0x80177354: b           L_80177458
    // 0x80177358: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177358: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017735C:
    // 0x8017735C: jal         0x802E4AA0
    // 0x80177360: nop

    LOOKUP_FUNC(0x802E4AA0)(rdram, ctx);
        goto after_12;
    // 0x80177360: nop

    after_12:
    // 0x80177364: b           L_80177458
    // 0x80177368: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177368: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017736C:
    // 0x8017736C: jal         0x802E3EA4
    // 0x80177370: nop

    LOOKUP_FUNC(0x802E3EA4)(rdram, ctx);
        goto after_13;
    // 0x80177370: nop

    after_13:
    // 0x80177374: b           L_80177458
    // 0x80177378: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177378: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017737C:
    // 0x8017737C: jal         0x802EAD24
    // 0x80177380: nop

    map_ovl_39_func_802EAD24(rdram, ctx);
        goto after_14;
    // 0x80177380: nop

    after_14:
    // 0x80177384: b           L_80177458
    // 0x80177388: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177388: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017738C:
    // 0x8017738C: jal         0x802E5A80
    // 0x80177390: nop

    LOOKUP_FUNC(0x802E5A80)(rdram, ctx);
        goto after_15;
    // 0x80177390: nop

    after_15:
    // 0x80177394: b           L_80177458
    // 0x80177398: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177398: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017739C:
    // 0x8017739C: jal         0x802E7F78
    // 0x801773A0: nop

    map_ovl_42_func_802E7F78(rdram, ctx);
        goto after_16;
    // 0x801773A0: nop

    after_16:
    // 0x801773A4: b           L_80177458
    // 0x801773A8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801773A8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801773AC:
    // 0x801773AC: jal         0x802E6B50
    // 0x801773B0: nop

    map_ovl_06_func_802E6B50(rdram, ctx);
        goto after_17;
    // 0x801773B0: nop

    after_17:
    // 0x801773B4: b           L_80177458
    // 0x801773B8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801773B8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801773BC:
    // 0x801773BC: jal         0x802E66C8
    // 0x801773C0: nop

    map_ovl_20_func_802E66C8(rdram, ctx);
        goto after_18;
    // 0x801773C0: nop

    after_18:
    // 0x801773C4: b           L_80177458
    // 0x801773C8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801773C8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801773CC:
    // 0x801773CC: jal         0x802E3BF0
    // 0x801773D0: nop

    map_ovl_46_func_802E3BF0(rdram, ctx);
        goto after_19;
    // 0x801773D0: nop

    after_19:
    // 0x801773D4: b           L_80177458
    // 0x801773D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801773D8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801773DC:
    // 0x801773DC: jal         0x802E4C80
    // 0x801773E0: nop

    map_ovl_19_func_802E4C80(rdram, ctx);
        goto after_20;
    // 0x801773E0: nop

    after_20:
    // 0x801773E4: b           L_80177458
    // 0x801773E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801773E8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801773EC:
    // 0x801773EC: jal         0x802EB368
    // 0x801773F0: nop

    map_ovl_39_func_802EB368(rdram, ctx);
        goto after_21;
    // 0x801773F0: nop

    after_21:
    // 0x801773F4: b           L_80177458
    // 0x801773F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x801773F8: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_801773FC:
    // 0x801773FC: jal         0x802F6134
    // 0x80177400: nop

    map_ovl_34_func_802F6134(rdram, ctx);
        goto after_22;
    // 0x80177400: nop

    after_22:
    // 0x80177404: b           L_80177458
    // 0x80177408: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177408: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017740C:
    // 0x8017740C: jal         0x802EB8F8
    // 0x80177410: nop

    map_ovl_39_func_802EB8F8(rdram, ctx);
        goto after_23;
    // 0x80177410: nop

    after_23:
    // 0x80177414: b           L_80177458
    // 0x80177418: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177418: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017741C:
    // 0x8017741C: jal         0x802EBEB8
    // 0x80177420: nop

    map_ovl_39_func_802EBEB8(rdram, ctx);
        goto after_24;
    // 0x80177420: nop

    after_24:
    // 0x80177424: b           L_80177458
    // 0x80177428: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177428: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017742C:
    // 0x8017742C: jal         0x802E7364
    // 0x80177430: nop

    map_ovl_20_func_802E7364(rdram, ctx);
        goto after_25;
    // 0x80177430: nop

    after_25:
    // 0x80177434: b           L_80177458
    // 0x80177438: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177438: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017743C:
    // 0x8017743C: jal         0x802EB4E4
    // 0x80177440: nop

    map_ovl_00_func_802EB4E4(rdram, ctx);
        goto after_26;
    // 0x80177440: nop

    after_26:
    // 0x80177444: b           L_80177458
    // 0x80177448: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80177458;
    // 0x80177448: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8017744C:
    // 0x8017744C: b           L_80177458
    // 0x80177450: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80177458;
    // 0x80177450: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80177454:
    // 0x80177454: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80177458:
    // 0x80177458: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017745C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80177460: jr          $ra
    // 0x80177464: nop

    return;
    // 0x80177464: nop

;}
RECOMP_FUNC void func_80177468(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177468: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x8017746C: lw          $v0, 0x98($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X98);
    // 0x80177470: sll         $v0, $v0, 3
    ctx->r2 = S32(ctx->r2 << 3);
    // 0x80177474: jr          $ra
    // 0x80177478: srl         $v0, $v0, 31
    ctx->r2 = S32(U32(ctx->r2) >> 31);
    return;
    // 0x80177478: srl         $v0, $v0, 31
    ctx->r2 = S32(U32(ctx->r2) >> 31);
;}
RECOMP_FUNC void func_8017747C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8017747C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80177480: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80177484: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x80177488: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8017748C: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x80177490: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80177494: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80177498: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8017749C: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x801774A0: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x801774A4: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x801774A8: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x801774AC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x801774B0: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x801774B4: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x801774B8: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x801774BC: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x801774C0: lw          $t9, 0x27C8($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X27C8);
    // 0x801774C4: jalr        $t9
    // 0x801774C8: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x801774C8: nop

    after_0:
    // 0x801774CC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x801774D0: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x801774D4: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x801774D8: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x801774DC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801774E0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801774E4: jr          $ra
    // 0x801774E8: nop

    return;
    // 0x801774E8: nop

;}
RECOMP_FUNC void func_801774EC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801774EC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x801774F0: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x801774F4: lw          $t6, 0x2B6C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2B6C);
    // 0x801774F8: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x801774FC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80177500: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x80177504: beq         $t6, $zero, L_80177520
    if (ctx->r14 == 0) {
        // 0x80177508: sw          $ra, 0x1C($sp)
        MEM_W(0X1C, ctx->r29) = ctx->r31;
            goto L_80177520;
    }
    // 0x80177508: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017750C: lw          $t9, 0x10($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X10);
    // 0x80177510: jalr        $t9
    // 0x80177514: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80177514: nop

    after_0:
    // 0x80177518: b           L_80177604
    // 0x8017751C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_80177604;
    // 0x8017751C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80177520:
    // 0x80177520: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80177524: addiu       $t9, $t9, 0x2808
    ctx->r25 = ADD32(ctx->r25, 0X2808);
    // 0x80177528: sw          $s0, 0x2B6C($v0)
    MEM_W(0X2B6C, ctx->r2) = ctx->r16;
    // 0x8017752C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80177530: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80177534: addiu       $a2, $zero, 0x588
    ctx->r6 = ADD32(0, 0X588);
    // 0x80177538: jalr        $t9
    // 0x8017753C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8017753C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_1:
    // 0x80177540: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80177544: jal         0x80177614
    // 0x80177548: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    func_80177614(rdram, ctx);
        goto after_2;
    // 0x80177548: sw          $v0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r2;
    after_2:
    // 0x8017754C: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x80177550: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x80177554: lw          $t7, 0x27A8($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X27A8);
    // 0x80177558: lui         $at, 0x4110
    ctx->r1 = S32(0X4110 << 16);
    // 0x8017755C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80177560: lui         $at, 0x4270
    ctx->r1 = S32(0X4270 << 16);
    // 0x80177564: mtc1        $at, $f2
    ctx->f2.u32l = ctx->r1;
    // 0x80177568: lbu         $t0, 0x98($v1)
    ctx->r8 = MEM_BU(ctx->r3, 0X98);
    // 0x8017756C: lui         $at, 0x4282
    ctx->r1 = S32(0X4282 << 16);
    // 0x80177570: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80177574: lui         $at, 0x42C8
    ctx->r1 = S32(0X42C8 << 16);
    // 0x80177578: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8017757C: andi        $t3, $t0, 0xFF7F
    ctx->r11 = ctx->r8 & 0XFF7F;
    // 0x80177580: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80177584: ori         $t5, $t3, 0x40
    ctx->r13 = ctx->r11 | 0X40;
    // 0x80177588: sw          $t7, 0xC($v1)
    MEM_W(0XC, ctx->r3) = ctx->r15;
    // 0x8017758C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80177590: addiu       $t8, $zero, 0x31
    ctx->r24 = ADD32(0, 0X31);
    // 0x80177594: andi        $t7, $t5, 0xDF
    ctx->r15 = ctx->r13 & 0XDF;
    // 0x80177598: sb          $t3, 0x98($v1)
    MEM_B(0X98, ctx->r3) = ctx->r11;
    // 0x8017759C: sw          $t8, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r24;
    // 0x801775A0: sb          $t5, 0x98($v1)
    MEM_B(0X98, ctx->r3) = ctx->r13;
    // 0x801775A4: ori         $t8, $t7, 0x10
    ctx->r24 = ctx->r15 | 0X10;
    // 0x801775A8: sb          $t7, 0x98($v1)
    MEM_B(0X98, ctx->r3) = ctx->r15;
    // 0x801775AC: sw          $zero, 0x8($v1)
    MEM_W(0X8, ctx->r3) = 0;
    // 0x801775B0: sb          $t8, 0x98($v1)
    MEM_B(0X98, ctx->r3) = ctx->r24;
    // 0x801775B4: swc1        $f0, 0x3C($v1)
    MEM_W(0X3C, ctx->r3) = ctx->f0.u32l;
    // 0x801775B8: swc1        $f0, 0x30($v1)
    MEM_W(0X30, ctx->r3) = ctx->f0.u32l;
    // 0x801775BC: swc1        $f2, 0x40($v1)
    MEM_W(0X40, ctx->r3) = ctx->f2.u32l;
    // 0x801775C0: swc1        $f2, 0x34($v1)
    MEM_W(0X34, ctx->r3) = ctx->f2.u32l;
    // 0x801775C4: swc1        $f12, 0x44($v1)
    MEM_W(0X44, ctx->r3) = ctx->f12.u32l;
    // 0x801775C8: swc1        $f12, 0x38($v1)
    MEM_W(0X38, ctx->r3) = ctx->f12.u32l;
    // 0x801775CC: swc1        $f4, 0x20($v1)
    MEM_W(0X20, ctx->r3) = ctx->f4.u32l;
    // 0x801775D0: swc1        $f6, 0x54($v1)
    MEM_W(0X54, ctx->r3) = ctx->f6.u32l;
    // 0x801775D4: jal         0x8017BCE4
    // 0x801775D8: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    func_8017BCE4(rdram, ctx);
        goto after_3;
    // 0x801775D8: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_3:
    // 0x801775DC: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x801775E0: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x801775E4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x801775E8: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x801775EC: sw          $v0, 0x14($v1)
    MEM_W(0X14, ctx->r3) = ctx->r2;
    // 0x801775F0: sh          $zero, -0x525C($at)
    MEM_H(-0X525C, ctx->r1) = 0;
    // 0x801775F4: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x801775F8: jalr        $t9
    // 0x801775FC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x801775FC: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_4:
    // 0x80177600: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80177604:
    // 0x80177604: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80177608: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8017760C: jr          $ra
    // 0x80177610: nop

    return;
    // 0x80177610: nop

;}
RECOMP_FUNC void func_80177614(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177614: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80177618: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017761C: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80177620: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80177624: addiu       $a0, $zero, 0x40
    ctx->r4 = ADD32(0, 0X40);
    // 0x80177628: jal         0x800057B4
    // 0x8017762C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_800057B4(rdram, ctx);
        goto after_0;
    // 0x8017762C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_0:
    // 0x80177630: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80177634: sw          $v0, 0x27A0($at)
    MEM_W(0X27A0, ctx->r1) = ctx->r2;
    // 0x80177638: addiu       $a0, $zero, 0x40
    ctx->r4 = ADD32(0, 0X40);
    // 0x8017763C: jal         0x800057B4
    // 0x80177640: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_800057B4(rdram, ctx);
        goto after_1;
    // 0x80177640: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_1:
    // 0x80177644: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80177648: sw          $v0, 0x27B0($at)
    MEM_W(0X27B0, ctx->r1) = ctx->r2;
    // 0x8017764C: addiu       $a0, $zero, 0x40
    ctx->r4 = ADD32(0, 0X40);
    // 0x80177650: jal         0x800057B4
    // 0x80177654: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_800057B4(rdram, ctx);
        goto after_2;
    // 0x80177654: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_2:
    // 0x80177658: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x8017765C: sw          $v0, 0x27C0($at)
    MEM_W(0X27C0, ctx->r1) = ctx->r2;
    // 0x80177660: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    // 0x80177664: jal         0x800057B4
    // 0x80177668: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_800057B4(rdram, ctx);
        goto after_3;
    // 0x80177668: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_3:
    // 0x8017766C: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80177670: addiu       $a2, $a2, 0x27A8
    ctx->r6 = ADD32(ctx->r6, 0X27A8);
    // 0x80177674: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x80177678: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x8017767C: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    // 0x80177680: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80177684: ori         $t7, $t6, 0x60
    ctx->r15 = ctx->r14 | 0X60;
    // 0x80177688: jal         0x800057B4
    // 0x8017768C: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    func_800057B4(rdram, ctx);
        goto after_4;
    // 0x8017768C: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    after_4:
    // 0x80177690: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80177694: addiu       $a2, $a2, 0x27A4
    ctx->r6 = ADD32(ctx->r6, 0X27A4);
    // 0x80177698: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x8017769C: lhu         $t8, 0x2($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X2);
    // 0x801776A0: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    // 0x801776A4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801776A8: ori         $t9, $t8, 0x60
    ctx->r25 = ctx->r24 | 0X60;
    // 0x801776AC: jal         0x800057B4
    // 0x801776B0: sh          $t9, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r25;
    func_800057B4(rdram, ctx);
        goto after_5;
    // 0x801776B0: sh          $t9, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r25;
    after_5:
    // 0x801776B4: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x801776B8: addiu       $a2, $a2, 0x27AC
    ctx->r6 = ADD32(ctx->r6, 0X27AC);
    // 0x801776BC: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x801776C0: lhu         $t0, 0x2($v0)
    ctx->r8 = MEM_HU(ctx->r2, 0X2);
    // 0x801776C4: addiu       $a0, $zero, 0x40
    ctx->r4 = ADD32(0, 0X40);
    // 0x801776C8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801776CC: ori         $t1, $t0, 0x60
    ctx->r9 = ctx->r8 | 0X60;
    // 0x801776D0: jal         0x800057B4
    // 0x801776D4: sh          $t1, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r9;
    func_800057B4(rdram, ctx);
        goto after_6;
    // 0x801776D4: sh          $t1, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r9;
    after_6:
    // 0x801776D8: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x801776DC: addiu       $a2, $a2, 0x27B4
    ctx->r6 = ADD32(ctx->r6, 0X27B4);
    // 0x801776E0: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x801776E4: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x801776E8: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    // 0x801776EC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x801776F0: ori         $t3, $t2, 0x60
    ctx->r11 = ctx->r10 | 0X60;
    // 0x801776F4: jal         0x800057B4
    // 0x801776F8: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    func_800057B4(rdram, ctx);
        goto after_7;
    // 0x801776F8: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    after_7:
    // 0x801776FC: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80177700: addiu       $a2, $a2, 0x27B8
    ctx->r6 = ADD32(ctx->r6, 0X27B8);
    // 0x80177704: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x80177708: lhu         $t4, 0x2($v0)
    ctx->r12 = MEM_HU(ctx->r2, 0X2);
    // 0x8017770C: addiu       $a0, $zero, 0x80
    ctx->r4 = ADD32(0, 0X80);
    // 0x80177710: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80177714: ori         $t5, $t4, 0x60
    ctx->r13 = ctx->r12 | 0X60;
    // 0x80177718: jal         0x800057B4
    // 0x8017771C: sh          $t5, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r13;
    func_800057B4(rdram, ctx);
        goto after_8;
    // 0x8017771C: sh          $t5, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r13;
    after_8:
    // 0x80177720: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80177724: addiu       $a2, $a2, 0x27BC
    ctx->r6 = ADD32(ctx->r6, 0X27BC);
    // 0x80177728: sw          $v0, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r2;
    // 0x8017772C: lhu         $t6, 0x2($v0)
    ctx->r14 = MEM_HU(ctx->r2, 0X2);
    // 0x80177730: lui         $a3, 0x8019
    ctx->r7 = S32(0X8019 << 16);
    // 0x80177734: addiu       $a3, $a3, 0x27A0
    ctx->r7 = ADD32(ctx->r7, 0X27A0);
    // 0x80177738: ori         $t7, $t6, 0x60
    ctx->r15 = ctx->r14 | 0X60;
    // 0x8017773C: lui         $at, 0x447A
    ctx->r1 = S32(0X447A << 16);
    // 0x80177740: sh          $t7, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r15;
    // 0x80177744: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80177748: lw          $t8, 0x0($a3)
    ctx->r24 = MEM_W(ctx->r7, 0X0);
    // 0x8017774C: lui         $t9, 0x8019
    ctx->r25 = S32(0X8019 << 16);
    // 0x80177750: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x80177754: swc1        $f0, 0x58($t8)
    MEM_W(0X58, ctx->r24) = ctx->f0.u32l;
    // 0x80177758: lw          $t9, 0x27A4($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X27A4);
    // 0x8017775C: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x80177760: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x80177764: swc1        $f0, 0x58($t9)
    MEM_W(0X58, ctx->r25) = ctx->f0.u32l;
    // 0x80177768: lw          $t0, 0x27A8($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X27A8);
    // 0x8017776C: lui         $t3, 0x8019
    ctx->r11 = S32(0X8019 << 16);
    // 0x80177770: lui         $t4, 0x8019
    ctx->r12 = S32(0X8019 << 16);
    // 0x80177774: swc1        $f0, 0x58($t0)
    MEM_W(0X58, ctx->r8) = ctx->f0.u32l;
    // 0x80177778: lw          $t1, 0x27AC($t1)
    ctx->r9 = MEM_W(ctx->r9, 0X27AC);
    // 0x8017777C: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x80177780: lui         $s0, 0x800C
    ctx->r16 = S32(0X800C << 16);
    // 0x80177784: swc1        $f0, 0x58($t1)
    MEM_W(0X58, ctx->r9) = ctx->f0.u32l;
    // 0x80177788: lw          $t2, 0x27B0($t2)
    ctx->r10 = MEM_W(ctx->r10, 0X27B0);
    // 0x8017778C: addiu       $s0, $s0, 0x1650
    ctx->r16 = ADD32(ctx->r16, 0X1650);
    // 0x80177790: swc1        $f0, 0x58($t2)
    MEM_W(0X58, ctx->r10) = ctx->f0.u32l;
    // 0x80177794: lw          $t3, 0x27B4($t3)
    ctx->r11 = MEM_W(ctx->r11, 0X27B4);
    // 0x80177798: swc1        $f0, 0x58($t3)
    MEM_W(0X58, ctx->r11) = ctx->f0.u32l;
    // 0x8017779C: lw          $t4, 0x27B8($t4)
    ctx->r12 = MEM_W(ctx->r12, 0X27B8);
    // 0x801777A0: swc1        $f0, 0x58($t4)
    MEM_W(0X58, ctx->r12) = ctx->f0.u32l;
    // 0x801777A4: lw          $t5, 0x0($a2)
    ctx->r13 = MEM_W(ctx->r6, 0X0);
    // 0x801777A8: swc1        $f0, 0x58($t5)
    MEM_W(0X58, ctx->r13) = ctx->f0.u32l;
    // 0x801777AC: lw          $t6, 0x27C0($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X27C0);
    // 0x801777B0: swc1        $f0, 0x58($t6)
    MEM_W(0X58, ctx->r14) = ctx->f0.u32l;
    // 0x801777B4: lw          $t7, 0x0($a3)
    ctx->r15 = MEM_W(ctx->r7, 0X0);
    // 0x801777B8: lw          $t8, 0x20($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X20);
    // 0x801777BC: sw          $t7, 0x24($t8)
    MEM_W(0X24, ctx->r24) = ctx->r15;
    // 0x801777C0: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x801777C4: jal         0x800058A4
    // 0x801777C8: lw          $a0, 0x0($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X0);
    func_800058A4(rdram, ctx);
        goto after_9;
    // 0x801777C8: lw          $a0, 0x0($a3)
    ctx->r4 = MEM_W(ctx->r7, 0X0);
    after_9:
    // 0x801777CC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x801777D0: lw          $a0, 0x27A4($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A4);
    // 0x801777D4: jal         0x800058A4
    // 0x801777D8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_10;
    // 0x801777D8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_10:
    // 0x801777DC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x801777E0: lw          $a0, 0x27A8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A8);
    // 0x801777E4: jal         0x800058A4
    // 0x801777E8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_11;
    // 0x801777E8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_11:
    // 0x801777EC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x801777F0: lw          $a0, 0x27AC($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27AC);
    // 0x801777F4: jal         0x800058A4
    // 0x801777F8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_12;
    // 0x801777F8: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_12:
    // 0x801777FC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177800: lw          $a0, 0x27B0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27B0);
    // 0x80177804: jal         0x800058A4
    // 0x80177808: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_13;
    // 0x80177808: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_13:
    // 0x8017780C: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177810: lw          $a0, 0x27B4($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27B4);
    // 0x80177814: jal         0x800058A4
    // 0x80177818: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_14;
    // 0x80177818: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_14:
    // 0x8017781C: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177820: lw          $a0, 0x27B8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27B8);
    // 0x80177824: jal         0x800058A4
    // 0x80177828: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_15;
    // 0x80177828: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_15:
    // 0x8017782C: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177830: lw          $a0, 0x27BC($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27BC);
    // 0x80177834: jal         0x800058A4
    // 0x80177838: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_16;
    // 0x80177838: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_16:
    // 0x8017783C: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177840: lw          $a0, 0x27C0($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27C0);
    // 0x80177844: jal         0x800058A4
    // 0x80177848: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    func_800058A4(rdram, ctx);
        goto after_17;
    // 0x80177848: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    after_17:
    // 0x8017784C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80177850: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80177854: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80177858: jr          $ra
    // 0x8017785C: nop

    return;
    // 0x8017785C: nop

;}
RECOMP_FUNC void bgHandler_dispatch(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177860: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80177864: lw          $t6, -0x5438($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X5438);
    // 0x80177868: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8017786C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80177870: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x80177874: bgez        $t7, L_80177938
    if (SIGNED(ctx->r15) >= 0) {
        // 0x80177878: sw          $a0, 0x18($sp)
        MEM_W(0X18, ctx->r29) = ctx->r4;
            goto L_80177938;
    }
    // 0x80177878: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8017787C: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x80177880: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80177884: jal         0x8017A600
    // 0x80177888: sw          $t8, 0x24A8($at)
    MEM_W(0X24A8, ctx->r1) = ctx->r24;
    func_8017A600(rdram, ctx);
        goto after_0;
    // 0x80177888: sw          $t8, 0x24A8($at)
    MEM_W(0X24A8, ctx->r1) = ctx->r24;
    after_0:
    // 0x8017788C: lui         $t9, 0x801D
    ctx->r25 = S32(0X801D << 16);
    // 0x80177890: lw          $t9, -0x5170($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X5170);
    // 0x80177894: andi        $t0, $t9, 0x1
    ctx->r8 = ctx->r25 & 0X1;
    // 0x80177898: beq         $t0, $zero, L_801778B0
    if (ctx->r8 == 0) {
        // 0x8017789C: nop
    
            goto L_801778B0;
    }
    // 0x8017789C: nop

    // 0x801778A0: jal         0x80177B4C
    // 0x801778A4: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    bgHandler_inactive(rdram, ctx);
        goto after_1;
    // 0x801778A4: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    after_1:
    // 0x801778A8: b           L_801778B8
    // 0x801778AC: nop

        goto L_801778B8;
    // 0x801778AC: nop

L_801778B0:
    // 0x801778B0: jal         0x80177948
    // 0x801778B4: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    bgHandler_active(rdram, ctx);
        goto after_2;
    // 0x801778B4: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    after_2:
L_801778B8:
    // 0x801778B8: jal         0x801780C4
    // 0x801778BC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    func_801780C4(rdram, ctx);
        goto after_3;
    // 0x801778BC: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    after_3:
    // 0x801778C0: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x801778C4: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x801778C8: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x801778CC: lw          $a0, 0x27A4($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A4);
    // 0x801778D0: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x801778D4: sw          $zero, 0x24A8($at)
    MEM_W(0X24A8, ctx->r1) = 0;
    // 0x801778D8: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    // 0x801778DC: jal         0x80013CF8
    // 0x801778E0: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_4;
    // 0x801778E0: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    after_4:
    // 0x801778E4: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x801778E8: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x801778EC: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x801778F0: lw          $a0, 0x27A4($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A4);
    // 0x801778F4: addiu       $a1, $a1, 0x68
    ctx->r5 = ADD32(ctx->r5, 0X68);
    // 0x801778F8: jal         0x80013CF8
    // 0x801778FC: addiu       $a0, $a0, 0x68
    ctx->r4 = ADD32(ctx->r4, 0X68);
    func_80013CF8(rdram, ctx);
        goto after_5;
    // 0x801778FC: addiu       $a0, $a0, 0x68
    ctx->r4 = ADD32(ctx->r4, 0X68);
    after_5:
    // 0x80177900: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177904: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80177908: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x8017790C: lw          $a0, 0x27AC($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27AC);
    // 0x80177910: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    // 0x80177914: jal         0x80013CF8
    // 0x80177918: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_6;
    // 0x80177918: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    after_6:
    // 0x8017791C: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177920: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80177924: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x80177928: lw          $a0, 0x27AC($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27AC);
    // 0x8017792C: addiu       $a1, $a1, 0x68
    ctx->r5 = ADD32(ctx->r5, 0X68);
    // 0x80177930: jal         0x80013CF8
    // 0x80177934: addiu       $a0, $a0, 0x68
    ctx->r4 = ADD32(ctx->r4, 0X68);
    func_80013CF8(rdram, ctx);
        goto after_7;
    // 0x80177934: addiu       $a0, $a0, 0x68
    ctx->r4 = ADD32(ctx->r4, 0X68);
    after_7:
L_80177938:
    // 0x80177938: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8017793C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80177940: jr          $ra
    // 0x80177944: nop

    return;
    // 0x80177944: nop

;}
RECOMP_FUNC void bgHandler_active(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177948: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8017794C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80177950: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80177954: lw          $t6, 0x295C($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X295C);
    // 0x80177958: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8017795C: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80177960: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80177964: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80177968: beq         $t6, $zero, L_80177B38
    if (ctx->r14 == 0) {
        // 0x8017796C: lw          $s0, 0x34($a0)
        ctx->r16 = MEM_W(ctx->r4, 0X34);
            goto L_80177B38;
    }
    // 0x8017796C: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x80177970: lw          $t7, 0x2B70($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X2B70);
    // 0x80177974: beq         $t7, $zero, L_80177B38
    if (ctx->r15 == 0) {
        // 0x80177978: sw          $t7, 0x10($s0)
        MEM_W(0X10, ctx->r16) = ctx->r15;
            goto L_80177B38;
    }
    // 0x80177978: sw          $t7, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r15;
    // 0x8017797C: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x80177980: lw          $t9, 0x8($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X8);
    // 0x80177984: lbu         $t4, 0x98($s0)
    ctx->r12 = MEM_BU(ctx->r16, 0X98);
    // 0x80177988: sw          $v0, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->r2;
    // 0x8017798C: xor         $t0, $t9, $v0
    ctx->r8 = ctx->r25 ^ ctx->r2;
    // 0x80177990: sltu        $t1, $zero, $t0
    ctx->r9 = 0 < ctx->r8 ? 1 : 0;
    // 0x80177994: sll         $t2, $t1, 4
    ctx->r10 = S32(ctx->r9 << 4);
    // 0x80177998: andi        $t3, $t2, 0x10
    ctx->r11 = ctx->r10 & 0X10;
    // 0x8017799C: andi        $t5, $t4, 0xFFEF
    ctx->r13 = ctx->r12 & 0XFFEF;
    // 0x801779A0: or          $t6, $t3, $t5
    ctx->r14 = ctx->r11 | ctx->r13;
    // 0x801779A4: jal         0x801782A8
    // 0x801779A8: sb          $t6, 0x98($s0)
    MEM_B(0X98, ctx->r16) = ctx->r14;
    func_801782A8(rdram, ctx);
        goto after_0;
    // 0x801779A8: sb          $t6, 0x98($s0)
    MEM_B(0X98, ctx->r16) = ctx->r14;
    after_0:
    // 0x801779AC: jal         0x801783FC
    // 0x801779B0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_801783FC(rdram, ctx);
        goto after_1;
    // 0x801779B0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_1:
    // 0x801779B4: lw          $a0, 0x10($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X10);
    // 0x801779B8: addiu       $a1, $s0, 0x24
    ctx->r5 = ADD32(ctx->r16, 0X24);
    // 0x801779BC: jal         0x8017DFBC
    // 0x801779C0: sw          $a1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r5;
    func_8017DFBC(rdram, ctx);
        goto after_2;
    // 0x801779C0: sw          $a1, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r5;
    after_2:
    // 0x801779C4: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801779C8: jal         0x8018166C
    // 0x801779CC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_8018166C(rdram, ctx);
        goto after_3;
    // 0x801779CC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_3:
    // 0x801779D0: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x801779D4: beql        $t7, $zero, L_80177A0C
    if (ctx->r15 == 0) {
        // 0x801779D8: lbu         $t8, 0x581($s0)
        ctx->r24 = MEM_BU(ctx->r16, 0X581);
            goto L_80177A0C;
    }
    goto skip_0;
    // 0x801779D8: lbu         $t8, 0x581($s0)
    ctx->r24 = MEM_BU(ctx->r16, 0X581);
    skip_0:
    // 0x801779DC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x801779E0: addiu       $a0, $s0, 0x48
    ctx->r4 = ADD32(ctx->r16, 0X48);
    // 0x801779E4: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x801779E8: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x801779EC: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x801779F0: jal         0x80013F30
    // 0x801779F4: nop

    func_80013F30(rdram, ctx);
        goto after_4;
    // 0x801779F4: nop

    after_4:
    // 0x801779F8: jal         0x80177BC8
    // 0x801779FC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80177BC8(rdram, ctx);
        goto after_5;
    // 0x801779FC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_5:
    // 0x80177A00: b           L_80177A40
    // 0x80177A04: addiu       $t9, $s0, 0x48
    ctx->r25 = ADD32(ctx->r16, 0X48);
        goto L_80177A40;
    // 0x80177A04: addiu       $t9, $s0, 0x48
    ctx->r25 = ADD32(ctx->r16, 0X48);
    // 0x80177A08: lbu         $t8, 0x581($s0)
    ctx->r24 = MEM_BU(ctx->r16, 0X581);
L_80177A0C:
    // 0x80177A0C: bnel        $t8, $zero, L_80177A40
    if (ctx->r24 != 0) {
        // 0x80177A10: addiu       $t9, $s0, 0x48
        ctx->r25 = ADD32(ctx->r16, 0X48);
            goto L_80177A40;
    }
    goto skip_1;
    // 0x80177A10: addiu       $t9, $s0, 0x48
    ctx->r25 = ADD32(ctx->r16, 0X48);
    skip_1:
    // 0x80177A14: jal         0x80177CC0
    // 0x80177A18: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80177CC0(rdram, ctx);
        goto after_6;
    // 0x80177A18: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_6:
    // 0x80177A1C: jal         0x80178500
    // 0x80177A20: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80178500(rdram, ctx);
        goto after_7;
    // 0x80177A20: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_7:
    // 0x80177A24: jal         0x80177C74
    // 0x80177A28: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80177C74(rdram, ctx);
        goto after_8;
    // 0x80177A28: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_8:
    // 0x80177A2C: addiu       $a0, $s0, 0x48
    ctx->r4 = ADD32(ctx->r16, 0X48);
    // 0x80177A30: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80177A34: jal         0x80013D58
    // 0x80177A38: lw          $a2, 0x54($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X54);
    func_80013D58(rdram, ctx);
        goto after_9;
    // 0x80177A38: lw          $a2, 0x54($s0)
    ctx->r6 = MEM_W(ctx->r16, 0X54);
    after_9:
    // 0x80177A3C: addiu       $t9, $s0, 0x48
    ctx->r25 = ADD32(ctx->r16, 0X48);
L_80177A40:
    // 0x80177A40: sw          $t9, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r25;
    // 0x80177A44: jal         0x80185390
    // 0x80177A48: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80185390(rdram, ctx);
        goto after_10;
    // 0x80177A48: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_10:
    // 0x80177A4C: addiu       $a0, $s0, 0x18
    ctx->r4 = ADD32(ctx->r16, 0X18);
    // 0x80177A50: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80177A54: sw          $a0, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r4;
    // 0x80177A58: jal         0x80013C90
    // 0x80177A5C: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    func_80013C90(rdram, ctx);
        goto after_11;
    // 0x80177A5C: lw          $a2, 0x28($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X28);
    after_11:
    // 0x80177A60: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80177A64: lw          $a0, 0x28($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X28);
    // 0x80177A68: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x80177A6C: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80177A70: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x80177A74: jal         0x80013F30
    // 0x80177A78: nop

    func_80013F30(rdram, ctx);
        goto after_12;
    // 0x80177A78: nop

    after_12:
    // 0x80177A7C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177A80: jal         0x8018166C
    // 0x80177A84: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_8018166C(rdram, ctx);
        goto after_13;
    // 0x80177A84: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_13:
    // 0x80177A88: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80177A8C: lw          $a2, 0x27A8($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X27A8);
    // 0x80177A90: addiu       $a0, $sp, 0x30
    ctx->r4 = ADD32(ctx->r29, 0X30);
    // 0x80177A94: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80177A98: jal         0x80013CC4
    // 0x80177A9C: addiu       $a2, $a2, 0x50
    ctx->r6 = ADD32(ctx->r6, 0X50);
    func_80013CC4(rdram, ctx);
        goto after_14;
    // 0x80177A9C: addiu       $a2, $a2, 0x50
    ctx->r6 = ADD32(ctx->r6, 0X50);
    after_14:
    // 0x80177AA0: lbu         $t1, 0x581($s0)
    ctx->r9 = MEM_BU(ctx->r16, 0X581);
    // 0x80177AA4: addiu       $t0, $sp, 0x30
    ctx->r8 = ADD32(ctx->r29, 0X30);
    // 0x80177AA8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80177AAC: beq         $t1, $at, L_80177ACC
    if (ctx->r9 == ctx->r1) {
        // 0x80177AB0: or          $s1, $t0, $zero
        ctx->r17 = ctx->r8 | 0;
            goto L_80177ACC;
    }
    // 0x80177AB0: or          $s1, $t0, $zero
    ctx->r17 = ctx->r8 | 0;
    // 0x80177AB4: lui         $a2, 0x3F0C
    ctx->r6 = S32(0X3F0C << 16);
    // 0x80177AB8: ori         $a2, $a2, 0xCCCD
    ctx->r6 = ctx->r6 | 0XCCCD;
    // 0x80177ABC: or          $a0, $t0, $zero
    ctx->r4 = ctx->r8 | 0;
    // 0x80177AC0: jal         0x80013D58
    // 0x80177AC4: or          $a1, $t0, $zero
    ctx->r5 = ctx->r8 | 0;
    func_80013D58(rdram, ctx);
        goto after_15;
    // 0x80177AC4: or          $a1, $t0, $zero
    ctx->r5 = ctx->r8 | 0;
    after_15:
    // 0x80177AC8: addiu       $s1, $sp, 0x30
    ctx->r17 = ADD32(ctx->r29, 0X30);
L_80177ACC:
    // 0x80177ACC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177AD0: lw          $a0, 0x27A8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A8);
    // 0x80177AD4: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80177AD8: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    // 0x80177ADC: jal         0x80013C90
    // 0x80177AE0: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013C90(rdram, ctx);
        goto after_16;
    // 0x80177AE0: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_16:
    // 0x80177AE4: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x80177AE8: lw          $a2, 0x27A8($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X27A8);
    // 0x80177AEC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177AF0: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    // 0x80177AF4: jal         0x80013CC4
    // 0x80177AF8: addiu       $a2, $a2, 0x68
    ctx->r6 = ADD32(ctx->r6, 0X68);
    func_80013CC4(rdram, ctx);
        goto after_17;
    // 0x80177AF8: addiu       $a2, $a2, 0x68
    ctx->r6 = ADD32(ctx->r6, 0X68);
    after_17:
    // 0x80177AFC: lbu         $t2, 0x581($s0)
    ctx->r10 = MEM_BU(ctx->r16, 0X581);
    // 0x80177B00: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80177B04: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177B08: beq         $t2, $at, L_80177B1C
    if (ctx->r10 == ctx->r1) {
        // 0x80177B0C: or          $a1, $s1, $zero
        ctx->r5 = ctx->r17 | 0;
            goto L_80177B1C;
    }
    // 0x80177B0C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80177B10: lui         $a2, 0x3F0C
    ctx->r6 = S32(0X3F0C << 16);
    // 0x80177B14: jal         0x80013D58
    // 0x80177B18: ori         $a2, $a2, 0xCCCD
    ctx->r6 = ctx->r6 | 0XCCCD;
    func_80013D58(rdram, ctx);
        goto after_18;
    // 0x80177B18: ori         $a2, $a2, 0xCCCD
    ctx->r6 = ctx->r6 | 0XCCCD;
    after_18:
L_80177B1C:
    // 0x80177B1C: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80177B20: lw          $a0, 0x27A8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A8);
    // 0x80177B24: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80177B28: addiu       $a0, $a0, 0x68
    ctx->r4 = ADD32(ctx->r4, 0X68);
    // 0x80177B2C: jal         0x80013C90
    // 0x80177B30: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013C90(rdram, ctx);
        goto after_19;
    // 0x80177B30: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_19:
    // 0x80177B34: sb          $zero, 0x581($s0)
    MEM_B(0X581, ctx->r16) = 0;
L_80177B38:
    // 0x80177B38: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80177B3C: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80177B40: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80177B44: jr          $ra
    // 0x80177B48: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x80177B48: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void bgHandler_inactive(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177B4C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80177B50: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80177B54: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80177B58: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80177B5C: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x80177B60: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x80177B64: addiu       $a0, $s0, 0x18
    ctx->r4 = ADD32(ctx->r16, 0X18);
    // 0x80177B68: jal         0x80013CF8
    // 0x80177B6C: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x80177B6C: addiu       $a1, $a1, 0x50
    ctx->r5 = ADD32(ctx->r5, 0X50);
    after_0:
    // 0x80177B70: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x80177B74: lw          $a1, 0x27A8($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X27A8);
    // 0x80177B78: addiu       $a0, $s0, 0x24
    ctx->r4 = ADD32(ctx->r16, 0X24);
    // 0x80177B7C: jal         0x80013CF8
    // 0x80177B80: addiu       $a1, $a1, 0x68
    ctx->r5 = ADD32(ctx->r5, 0X68);
    func_80013CF8(rdram, ctx);
        goto after_1;
    // 0x80177B80: addiu       $a1, $a1, 0x68
    ctx->r5 = ADD32(ctx->r5, 0X68);
    after_1:
    // 0x80177B84: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80177B88: addiu       $a0, $s0, 0x48
    ctx->r4 = ADD32(ctx->r16, 0X48);
    // 0x80177B8C: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x80177B90: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80177B94: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x80177B98: jal         0x80013F30
    // 0x80177B9C: nop

    func_80013F30(rdram, ctx);
        goto after_2;
    // 0x80177B9C: nop

    after_2:
    // 0x80177BA0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80177BA4: lw          $a0, 0x14($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X14);
    // 0x80177BA8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80177BAC: jal         0x8017BD78
    // 0x80177BB0: swc1        $f0, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->f0.u32l;
    func_8017BD78(rdram, ctx);
        goto after_3;
    // 0x80177BB0: swc1        $f0, 0x54($s0)
    MEM_W(0X54, ctx->r16) = ctx->f0.u32l;
    after_3:
    // 0x80177BB4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80177BB8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80177BBC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80177BC0: jr          $ra
    // 0x80177BC4: nop

    return;
    // 0x80177BC4: nop

;}
RECOMP_FUNC void func_80177BC8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177BC8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80177BCC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80177BD0: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80177BD4: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80177BD8: addiu       $t7, $t6, -0x2
    ctx->r15 = ADD32(ctx->r14, -0X2);
    // 0x80177BDC: sltiu       $at, $t7, 0x9
    ctx->r1 = ctx->r15 < 0X9 ? 1 : 0;
    // 0x80177BE0: beq         $at, $zero, L_80177C64
    if (ctx->r1 == 0) {
        // 0x80177BE4: sll         $t7, $t7, 2
        ctx->r15 = S32(ctx->r15 << 2);
            goto L_80177C64;
    }
    // 0x80177BE4: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80177BE8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80177BEC: addu        $at, $at, $t7
    gpr jr_addend_80177BF4 = ctx->r15;
    ctx->r1 = ADD32(ctx->r1, ctx->r15);
    // 0x80177BF0: lw          $t7, -0x4204($at)
    ctx->r15 = ADD32(ctx->r1, -0X4204);
    // 0x80177BF4: jr          $t7
    // 0x80177BF8: nop

    switch (jr_addend_80177BF4 >> 2) {
        case 0: goto L_80177BFC; break;
        case 1: goto L_80177C0C; break;
        case 2: goto L_80177C64; break;
        case 3: goto L_80177C64; break;
        case 4: goto L_80177C1C; break;
        case 5: goto L_80177C2C; break;
        case 6: goto L_80177C3C; break;
        case 7: goto L_80177C4C; break;
        case 8: goto L_80177C5C; break;
        default: switch_error(__func__, 0x80177BF4, 0x8019BDFC);
    }
    // 0x80177BF8: nop

L_80177BFC:
    // 0x80177BFC: jal         0x801789B0
    // 0x80177C00: nop

    func_801789B0(rdram, ctx);
        goto after_0;
    // 0x80177C00: nop

    after_0:
    // 0x80177C04: b           L_80177C68
    // 0x80177C08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80177C68;
    // 0x80177C08: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80177C0C:
    // 0x80177C0C: jal         0x80179010
    // 0x80177C10: nop

    func_80179010(rdram, ctx);
        goto after_1;
    // 0x80177C10: nop

    after_1:
    // 0x80177C14: b           L_80177C68
    // 0x80177C18: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80177C68;
    // 0x80177C18: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80177C1C:
    // 0x80177C1C: jal         0x8017924C
    // 0x80177C20: nop

    func_8017924C(rdram, ctx);
        goto after_2;
    // 0x80177C20: nop

    after_2:
    // 0x80177C24: b           L_80177C68
    // 0x80177C28: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80177C68;
    // 0x80177C28: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80177C2C:
    // 0x80177C2C: jal         0x801794CC
    // 0x80177C30: nop

    func_801794CC(rdram, ctx);
        goto after_3;
    // 0x80177C30: nop

    after_3:
    // 0x80177C34: b           L_80177C68
    // 0x80177C38: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80177C68;
    // 0x80177C38: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80177C3C:
    // 0x80177C3C: jal         0x801828B4
    // 0x80177C40: nop

    func_801828B4(rdram, ctx);
        goto after_4;
    // 0x80177C40: nop

    after_4:
    // 0x80177C44: b           L_80177C68
    // 0x80177C48: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80177C68;
    // 0x80177C48: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80177C4C:
    // 0x80177C4C: jal         0x80179530
    // 0x80177C50: nop

    func_80179530(rdram, ctx);
        goto after_5;
    // 0x80177C50: nop

    after_5:
    // 0x80177C54: b           L_80177C68
    // 0x80177C58: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80177C68;
    // 0x80177C58: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80177C5C:
    // 0x80177C5C: jal         0x8017965C
    // 0x80177C60: nop

    func_8017965C(rdram, ctx);
        goto after_6;
    // 0x80177C60: nop

    after_6:
L_80177C64:
    // 0x80177C64: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80177C68:
    // 0x80177C68: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80177C6C: jr          $ra
    // 0x80177C70: nop

    return;
    // 0x80177C70: nop

;}
RECOMP_FUNC void func_80177C74(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177C74: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80177C78: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80177C7C: lw          $v1, 0x34($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X34);
    // 0x80177C80: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80177C84: jal         0x80177468
    // 0x80177C88: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    func_80177468(rdram, ctx);
        goto after_0;
    // 0x80177C88: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_0:
    // 0x80177C8C: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80177C90: beq         $v0, $zero, L_80177CA8
    if (ctx->r2 == 0) {
        // 0x80177C94: lw          $a0, 0x20($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X20);
            goto L_80177CA8;
    }
    // 0x80177C94: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80177C98: lw          $t7, 0x10($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X10);
    // 0x80177C9C: addiu       $t6, $zero, 0x3
    ctx->r14 = ADD32(0, 0X3);
    // 0x80177CA0: lw          $t8, 0x34($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X34);
    // 0x80177CA4: sw          $t6, 0x0($t8)
    MEM_W(0X0, ctx->r24) = ctx->r14;
L_80177CA8:
    // 0x80177CA8: jal         0x80177258
    // 0x80177CAC: lw          $a1, 0x4($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X4);
    func_80177258(rdram, ctx);
        goto after_1;
    // 0x80177CAC: lw          $a1, 0x4($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X4);
    after_1:
    // 0x80177CB0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80177CB4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80177CB8: jr          $ra
    // 0x80177CBC: nop

    return;
    // 0x80177CBC: nop

;}
RECOMP_FUNC void func_80177CC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177CC0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80177CC4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80177CC8: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80177CCC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x80177CD0: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80177CD4: lw          $t7, 0x4($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X4);
    // 0x80177CD8: addiu       $t8, $t7, -0x8
    ctx->r24 = ADD32(ctx->r15, -0X8);
    // 0x80177CDC: sltiu       $at, $t8, 0x1A
    ctx->r1 = ctx->r24 < 0X1A ? 1 : 0;
    // 0x80177CE0: beq         $at, $zero, L_80177D1C
    if (ctx->r1 == 0) {
        // 0x80177CE4: sll         $t8, $t8, 2
        ctx->r24 = S32(ctx->r24 << 2);
            goto L_80177D1C;
    }
    // 0x80177CE4: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80177CE8: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80177CEC: addu        $at, $at, $t8
    gpr jr_addend_80177CF4 = ctx->r24;
    ctx->r1 = ADD32(ctx->r1, ctx->r24);
    // 0x80177CF0: lw          $t8, -0x41E0($at)
    ctx->r24 = ADD32(ctx->r1, -0X41E0);
    // 0x80177CF4: jr          $t8
    // 0x80177CF8: nop

    switch (jr_addend_80177CF4 >> 2) {
        case 0: goto L_80177CFC; break;
        case 1: goto L_80177CFC; break;
        case 2: goto L_80177CFC; break;
        case 3: goto L_80177CFC; break;
        case 4: goto L_80177CFC; break;
        case 5: goto L_80177CFC; break;
        case 6: goto L_80177CFC; break;
        case 7: goto L_80177CFC; break;
        case 8: goto L_80177CFC; break;
        case 9: goto L_80177CFC; break;
        case 10: goto L_80177CFC; break;
        case 11: goto L_80177CFC; break;
        case 12: goto L_80177CFC; break;
        case 13: goto L_80177CFC; break;
        case 14: goto L_80177CFC; break;
        case 15: goto L_80177CFC; break;
        case 16: goto L_80177CFC; break;
        case 17: goto L_80177CFC; break;
        case 18: goto L_80177CFC; break;
        case 19: goto L_80177CFC; break;
        case 20: goto L_80177CFC; break;
        case 21: goto L_80177CFC; break;
        case 22: goto L_80177CFC; break;
        case 23: goto L_80177CFC; break;
        case 24: goto L_80177CFC; break;
        case 25: goto L_80177CFC; break;
        default: switch_error(__func__, 0x80177CF4, 0x8019BE20);
    }
    // 0x80177CF8: nop

L_80177CFC:
    // 0x80177CFC: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80177D00: addiu       $a0, $v0, 0x48
    ctx->r4 = ADD32(ctx->r2, 0X48);
    // 0x80177D04: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    // 0x80177D08: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x80177D0C: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80177D10: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x80177D14: jal         0x80013F30
    // 0x80177D18: nop

    func_80013F30(rdram, ctx);
        goto after_0;
    // 0x80177D18: nop

    after_0:
L_80177D1C:
    // 0x80177D1C: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80177D20: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x80177D24: addiu       $t9, $v0, 0x48
    ctx->r25 = ADD32(ctx->r2, 0X48);
    // 0x80177D28: jal         0x80179790
    // 0x80177D2C: sw          $t9, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r25;
    func_80179790(rdram, ctx);
        goto after_1;
    // 0x80177D2C: sw          $t9, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r25;
    after_1:
    // 0x80177D30: beql        $v0, $zero, L_80177D58
    if (ctx->r2 == 0) {
        // 0x80177D34: lw          $a0, 0x1C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X1C);
            goto L_80177D58;
    }
    goto skip_0;
    // 0x80177D34: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80177D38: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80177D3C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80177D40: mfc1        $a1, $f0
    ctx->r5 = (int32_t)ctx->f0.u32l;
    // 0x80177D44: mfc1        $a2, $f0
    ctx->r6 = (int32_t)ctx->f0.u32l;
    // 0x80177D48: mfc1        $a3, $f0
    ctx->r7 = (int32_t)ctx->f0.u32l;
    // 0x80177D4C: jal         0x80013F30
    // 0x80177D50: nop

    func_80013F30(rdram, ctx);
        goto after_2;
    // 0x80177D50: nop

    after_2:
    // 0x80177D54: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_80177D58:
    // 0x80177D58: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80177D5C: addiu       $a0, $a0, 0x18
    ctx->r4 = ADD32(ctx->r4, 0X18);
    // 0x80177D60: jal         0x80013C90
    // 0x80177D64: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013C90(rdram, ctx);
        goto after_3;
    // 0x80177D64: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_3:
    // 0x80177D68: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80177D6C: addiu       $a1, $zero, 0x0
    ctx->r5 = ADD32(0, 0X0);
    // 0x80177D70: addiu       $a2, $zero, 0x0
    ctx->r6 = ADD32(0, 0X0);
    // 0x80177D74: jal         0x80013F30
    // 0x80177D78: addiu       $a3, $zero, 0x0
    ctx->r7 = ADD32(0, 0X0);
    func_80013F30(rdram, ctx);
        goto after_4;
    // 0x80177D78: addiu       $a3, $zero, 0x0
    ctx->r7 = ADD32(0, 0X0);
    after_4:
    // 0x80177D7C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80177D80: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80177D84: jr          $ra
    // 0x80177D88: nop

    return;
    // 0x80177D88: nop

;}
RECOMP_FUNC void func_80177D8C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177D8C: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x80177D90: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80177D94: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80177D98: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80177D9C: sw          $a0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r4;
    // 0x80177DA0: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x80177DA4: addiu       $s1, $sp, 0x30
    ctx->r17 = ADD32(ctx->r29, 0X30);
    // 0x80177DA8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177DAC: jal         0x80013CF8
    // 0x80177DB0: addiu       $a1, $s0, 0x24
    ctx->r5 = ADD32(ctx->r16, 0X24);
    func_80013CF8(rdram, ctx);
        goto after_0;
    // 0x80177DB0: addiu       $a1, $s0, 0x24
    ctx->r5 = ADD32(ctx->r16, 0X24);
    after_0:
    // 0x80177DB4: lwc1        $f4, 0x34($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80177DB8: lwc1        $f6, 0x30($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X30);
    // 0x80177DBC: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x80177DC0: lh          $t7, -0x5470($t7)
    ctx->r15 = MEM_H(ctx->r15, -0X5470);
    // 0x80177DC4: add.s       $f8, $f4, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f8.fl = ctx->f4.fl + ctx->f6.fl;
    // 0x80177DC8: addiu       $at, $zero, 0x1D
    ctx->r1 = ADD32(0, 0X1D);
    // 0x80177DCC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177DD0: bne         $t7, $at, L_80177DF4
    if (ctx->r15 != ctx->r1) {
        // 0x80177DD4: swc1        $f8, 0x34($sp)
        MEM_W(0X34, ctx->r29) = ctx->f8.u32l;
            goto L_80177DF4;
    }
    // 0x80177DD4: swc1        $f8, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f8.u32l;
    // 0x80177DD8: lui         $at, 0x402A
    ctx->r1 = S32(0X402A << 16);
    // 0x80177DDC: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x80177DE0: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80177DE4: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80177DE8: add.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = ctx->f10.d + ctx->f16.d;
    // 0x80177DEC: cvt.s.d     $f0, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f0.fl = CVT_S_D(ctx->f18.d);
    // 0x80177DF0: swc1        $f0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f0.u32l;
L_80177DF4:
    // 0x80177DF4: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80177DF8: jal         0x80013CC4
    // 0x80177DFC: addiu       $a2, $s0, 0x18
    ctx->r6 = ADD32(ctx->r16, 0X18);
    func_80013CC4(rdram, ctx);
        goto after_1;
    // 0x80177DFC: addiu       $a2, $s0, 0x18
    ctx->r6 = ADD32(ctx->r16, 0X18);
    after_1:
    // 0x80177E00: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177E04: jal         0x80013DE4
    // 0x80177E08: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80013DE4(rdram, ctx);
        goto after_2;
    // 0x80177E08: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_2:
    // 0x80177E0C: lwc1        $f12, 0x34($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X34);
    // 0x80177E10: mov.s       $f14, $f0
    CHECK_FR(ctx, 14);
    CHECK_FR(ctx, 0);
    ctx->f14.fl = ctx->f0.fl;
    // 0x80177E14: c.lt.s      $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    c1cs = ctx->f0.fl < ctx->f12.fl;
    // 0x80177E18: nop

    // 0x80177E1C: bc1fl       L_80177E64
    if (!c1cs) {
        // 0x80177E20: lwc1        $f12, 0x38($s0)
        ctx->f12.u32l = MEM_W(ctx->r16, 0X38);
            goto L_80177E64;
    }
    goto skip_0;
    // 0x80177E20: lwc1        $f12, 0x38($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X38);
    skip_0:
    // 0x80177E24: sub.s       $f4, $f0, $f12
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f4.fl = ctx->f0.fl - ctx->f12.fl;
    // 0x80177E28: lui         $at, 0x3FC0
    ctx->r1 = S32(0X3FC0 << 16);
    // 0x80177E2C: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x80177E30: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x80177E34: cvt.d.s     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.fl); 
    ctx->f6.d = CVT_D_S(ctx->f4.fl);
    // 0x80177E38: lwc1        $f16, 0x30($sp)
    ctx->f16.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80177E3C: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x80177E40: lwc1        $f4, 0x38($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80177E44: cvt.s.d     $f2, $f10
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f10.d); 
    ctx->f2.fl = CVT_S_D(ctx->f10.d);
    // 0x80177E48: mul.s       $f18, $f16, $f2
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f18.fl = MUL_S(ctx->f16.fl, ctx->f2.fl);
    // 0x80177E4C: nop

    // 0x80177E50: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80177E54: swc1        $f18, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f18.u32l;
    // 0x80177E58: b           L_80177EC0
    // 0x80177E5C: swc1        $f6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f6.u32l;
        goto L_80177EC0;
    // 0x80177E5C: swc1        $f6, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f6.u32l;
    // 0x80177E60: lwc1        $f12, 0x38($s0)
    ctx->f12.u32l = MEM_W(ctx->r16, 0X38);
L_80177E64:
    // 0x80177E64: c.lt.s      $f12, $f0
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 0);
    c1cs = ctx->f12.fl < ctx->f0.fl;
    // 0x80177E68: nop

    // 0x80177E6C: bc1fl       L_80177EB4
    if (!c1cs) {
        // 0x80177E70: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_80177EB4;
    }
    goto skip_1;
    // 0x80177E70: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_1:
    // 0x80177E74: sub.s       $f8, $f0, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = ctx->f0.fl - ctx->f12.fl;
    // 0x80177E78: lui         $at, 0x3FC0
    ctx->r1 = S32(0X3FC0 << 16);
    // 0x80177E7C: mtc1        $at, $f17
    ctx->f_odd[(17 - 1) * 2] = ctx->r1;
    // 0x80177E80: mtc1        $zero, $f16
    ctx->f16.u32l = 0;
    // 0x80177E84: cvt.d.s     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f8.fl); 
    ctx->f10.d = CVT_D_S(ctx->f8.fl);
    // 0x80177E88: lwc1        $f4, 0x30($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80177E8C: mul.d       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.d); NAN_CHECK(ctx->f16.d); 
    ctx->f18.d = MUL_D(ctx->f10.d, ctx->f16.d);
    // 0x80177E90: lwc1        $f8, 0x38($sp)
    ctx->f8.u32l = MEM_W(ctx->r29, 0X38);
    // 0x80177E94: cvt.s.d     $f2, $f18
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f18.d); 
    ctx->f2.fl = CVT_S_D(ctx->f18.d);
    // 0x80177E98: mul.s       $f6, $f4, $f2
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f4.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f6.fl = MUL_S(ctx->f4.fl, ctx->f2.fl);
    // 0x80177E9C: nop

    // 0x80177EA0: mul.s       $f10, $f8, $f2
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 2);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f2.fl); 
    ctx->f10.fl = MUL_S(ctx->f8.fl, ctx->f2.fl);
    // 0x80177EA4: swc1        $f6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f6.u32l;
    // 0x80177EA8: b           L_80177EC0
    // 0x80177EAC: swc1        $f10, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f10.u32l;
        goto L_80177EC0;
    // 0x80177EAC: swc1        $f10, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f10.u32l;
    // 0x80177EB0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
L_80177EB4:
    // 0x80177EB4: nop

    // 0x80177EB8: swc1        $f0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f0.u32l;
    // 0x80177EBC: swc1        $f0, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->f0.u32l;
L_80177EC0:
    // 0x80177EC0: lwc1        $f0, 0x34($sp)
    ctx->f0.u32l = MEM_W(ctx->r29, 0X34);
    // 0x80177EC4: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80177EC8: ldc1        $f18, -0x4178($at)
    CHECK_FR(ctx, 18);
    ctx->f18.u64 = LD(ctx->r1, -0X4178);
    // 0x80177ECC: mul.s       $f0, $f0, $f14
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 14);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f14.fl); 
    ctx->f0.fl = MUL_S(ctx->f0.fl, ctx->f14.fl);
    // 0x80177ED0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80177ED4: cvt.d.s     $f16, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f0.fl); 
    ctx->f16.d = CVT_D_S(ctx->f0.fl);
    // 0x80177ED8: mul.d       $f4, $f16, $f18
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.d); NAN_CHECK(ctx->f18.d); 
    ctx->f4.d = MUL_D(ctx->f16.d, ctx->f18.d);
    // 0x80177EDC: cvt.s.d     $f0, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f4.d); 
    ctx->f0.fl = CVT_S_D(ctx->f4.d);
    // 0x80177EE0: jal         0x80013F50
    // 0x80177EE4: swc1        $f0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f0.u32l;
    vec3f_multiplyByOne(rdram, ctx);
        goto after_3;
    // 0x80177EE4: swc1        $f0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->f0.u32l;
    after_3:
    // 0x80177EE8: lw          $a0, 0x40($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X40);
    // 0x80177EEC: jal         0x80179988
    // 0x80177EF0: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80179988(rdram, ctx);
        goto after_4;
    // 0x80177EF0: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_4:
    // 0x80177EF4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80177EF8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80177EFC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80177F00: jr          $ra
    // 0x80177F04: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    return;
    // 0x80177F04: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
;}
RECOMP_FUNC void func_80177F08(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177F08: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80177F0C: lhu         $t6, -0x7814($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X7814);
    // 0x80177F10: mul.s       $f12, $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80177F14: andi        $t7, $t6, 0x1
    ctx->r15 = ctx->r14 & 0X1;
    // 0x80177F18: beql        $t7, $zero, L_80177F30
    if (ctx->r15 == 0) {
        // 0x80177F1C: neg.s       $f2, $f12
        CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = -ctx->f12.fl;
            goto L_80177F30;
    }
    goto skip_0;
    // 0x80177F1C: neg.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = -ctx->f12.fl;
    skip_0:
    // 0x80177F20: mov.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    ctx->f2.fl = ctx->f12.fl;
    // 0x80177F24: jr          $ra
    // 0x80177F28: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    return;
    // 0x80177F28: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    // 0x80177F2C: neg.s       $f2, $f12
    CHECK_FR(ctx, 2);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f2.fl = -ctx->f12.fl;
L_80177F30:
    // 0x80177F30: jr          $ra
    // 0x80177F34: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
    return;
    // 0x80177F34: mov.s       $f0, $f2
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 2);
    ctx->f0.fl = ctx->f2.fl;
;}
RECOMP_FUNC void func_80177F38(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177F38: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80177F3C: lhu         $v0, -0x7814($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X7814);
    // 0x80177F40: mul.s       $f12, $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80177F44: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80177F48: bgez        $v0, L_80177F5C
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80177F4C: andi        $v0, $v0, 0x3
        ctx->r2 = ctx->r2 & 0X3;
            goto L_80177F5C;
    }
    // 0x80177F4C: andi        $v0, $v0, 0x3
    ctx->r2 = ctx->r2 & 0X3;
    // 0x80177F50: beq         $v0, $zero, L_80177F5C
    if (ctx->r2 == 0) {
        // 0x80177F54: nop
    
            goto L_80177F5C;
    }
    // 0x80177F54: nop

    // 0x80177F58: addiu       $v0, $v0, -0x4
    ctx->r2 = ADD32(ctx->r2, -0X4);
L_80177F5C:
    // 0x80177F5C: beql        $v0, $zero, L_80177F88
    if (ctx->r2 == 0) {
        // 0x80177F60: mtc1        $zero, $f0
        ctx->f0.u32l = 0;
            goto L_80177F88;
    }
    goto skip_0;
    // 0x80177F60: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    skip_0:
    // 0x80177F64: beq         $v0, $at, L_80177F90
    if (ctx->r2 == ctx->r1) {
        // 0x80177F68: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_80177F90;
    }
    // 0x80177F68: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80177F6C: beq         $v0, $at, L_80177F98
    if (ctx->r2 == ctx->r1) {
        // 0x80177F70: addiu       $at, $zero, 0x3
        ctx->r1 = ADD32(0, 0X3);
            goto L_80177F98;
    }
    // 0x80177F70: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x80177F74: beql        $v0, $at, L_80177FA8
    if (ctx->r2 == ctx->r1) {
        // 0x80177F78: neg.s       $f0, $f12
        CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
            goto L_80177FA8;
    }
    goto skip_1;
    // 0x80177F78: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
    skip_1:
    // 0x80177F7C: jr          $ra
    // 0x80177F80: nop

    return;
    // 0x80177F80: nop

    // 0x80177F84: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
L_80177F88:
    // 0x80177F88: jr          $ra
    // 0x80177F8C: nop

    return;
    // 0x80177F8C: nop

L_80177F90:
    // 0x80177F90: jr          $ra
    // 0x80177F94: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    return;
    // 0x80177F94: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_80177F98:
    // 0x80177F98: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80177F9C: jr          $ra
    // 0x80177FA0: nop

    return;
    // 0x80177FA0: nop

    // 0x80177FA4: neg.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f0.fl = -ctx->f12.fl;
L_80177FA8:
    // 0x80177FA8: jr          $ra
    // 0x80177FAC: nop

    return;
    // 0x80177FAC: nop

;}
RECOMP_FUNC void func_80177FB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80177FB0: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80177FB4: lhu         $t6, -0x7814($t6)
    ctx->r14 = MEM_HU(ctx->r14, -0X7814);
    // 0x80177FB8: mul.s       $f12, $f12, $f12
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f12.fl); 
    ctx->f12.fl = MUL_S(ctx->f12.fl, ctx->f12.fl);
    // 0x80177FBC: andi        $t7, $t6, 0x7
    ctx->r15 = ctx->r14 & 0X7;
    // 0x80177FC0: sltiu       $at, $t7, 0x8
    ctx->r1 = ctx->r15 < 0X8 ? 1 : 0;
    // 0x80177FC4: beq         $at, $zero, L_8017805C
    if (ctx->r1 == 0) {
        // 0x80177FC8: sll         $t7, $t7, 2
        ctx->r15 = S32(ctx->r15 << 2);
            goto L_8017805C;
    }
    // 0x80177FC8: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80177FCC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80177FD0: addu        $at, $at, $t7
    gpr jr_addend_80177FD8 = ctx->r15;
    ctx->r1 = ADD32(ctx->r1, ctx->r15);
    // 0x80177FD4: lw          $t7, -0x4170($at)
    ctx->r15 = ADD32(ctx->r1, -0X4170);
    // 0x80177FD8: jr          $t7
    // 0x80177FDC: nop

    switch (jr_addend_80177FD8 >> 2) {
        case 0: goto L_80177FE0; break;
        case 1: goto L_80177FEC; break;
        case 2: goto L_80178000; break;
        case 3: goto L_80178008; break;
        case 4: goto L_8017801C; break;
        case 5: goto L_80178028; break;
        case 6: goto L_80178040; break;
        case 7: goto L_80178048; break;
        default: switch_error(__func__, 0x80177FD8, 0x8019BE90);
    }
    // 0x80177FDC: nop

L_80177FE0:
    // 0x80177FE0: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80177FE4: jr          $ra
    // 0x80177FE8: nop

    return;
    // 0x80177FE8: nop

L_80177FEC:
    // 0x80177FEC: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x80177FF0: lwc1        $f4, -0x4150($at)
    ctx->f4.u32l = MEM_W(ctx->r1, -0X4150);
    // 0x80177FF4: mul.s       $f0, $f12, $f4
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f0.fl = MUL_S(ctx->f12.fl, ctx->f4.fl);
    // 0x80177FF8: jr          $ra
    // 0x80177FFC: nop

    return;
    // 0x80177FFC: nop

L_80178000:
    // 0x80178000: jr          $ra
    // 0x80178004: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    return;
    // 0x80178004: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_80178008:
    // 0x80178008: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017800C: lwc1        $f6, -0x414C($at)
    ctx->f6.u32l = MEM_W(ctx->r1, -0X414C);
    // 0x80178010: mul.s       $f0, $f12, $f6
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f12.fl); NAN_CHECK(ctx->f6.fl); 
    ctx->f0.fl = MUL_S(ctx->f12.fl, ctx->f6.fl);
    // 0x80178014: jr          $ra
    // 0x80178018: nop

    return;
    // 0x80178018: nop

L_8017801C:
    // 0x8017801C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80178020: jr          $ra
    // 0x80178024: nop

    return;
    // 0x80178024: nop

L_80178028:
    // 0x80178028: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017802C: lwc1        $f10, -0x4148($at)
    ctx->f10.u32l = MEM_W(ctx->r1, -0X4148);
    // 0x80178030: neg.s       $f8, $f12
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f8.fl = -ctx->f12.fl;
    // 0x80178034: mul.s       $f0, $f8, $f10
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f0.fl = MUL_S(ctx->f8.fl, ctx->f10.fl);
    // 0x80178038: jr          $ra
    // 0x8017803C: nop

    return;
    // 0x8017803C: nop

L_80178040:
    // 0x80178040: jr          $ra
    // 0x80178044: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
    return;
    // 0x80178044: mov.s       $f0, $f12
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 12);
    ctx->f0.fl = ctx->f12.fl;
L_80178048:
    // 0x80178048: lui         $at, 0x801A
    ctx->r1 = S32(0X801A << 16);
    // 0x8017804C: lwc1        $f18, -0x4144($at)
    ctx->f18.u32l = MEM_W(ctx->r1, -0X4144);
    // 0x80178050: neg.s       $f16, $f12
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 12);
    NAN_CHECK(ctx->f12.fl); 
    ctx->f16.fl = -ctx->f12.fl;
    // 0x80178054: mul.s       $f0, $f16, $f18
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 18);
    NAN_CHECK(ctx->f16.fl); NAN_CHECK(ctx->f18.fl); 
    ctx->f0.fl = MUL_S(ctx->f16.fl, ctx->f18.fl);
    // 0x80178058: nop

L_8017805C:
    // 0x8017805C: jr          $ra
    // 0x80178060: nop

    return;
    // 0x80178060: nop

;}
RECOMP_FUNC void func_80178064(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80178064: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80178068: beq         $a1, $zero, L_8017808C
    if (ctx->r5 == 0) {
        // 0x8017806C: sw          $ra, 0x14($sp)
        MEM_W(0X14, ctx->r29) = ctx->r31;
            goto L_8017808C;
    }
    // 0x8017806C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80178070: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80178074: beq         $a1, $at, L_8017809C
    if (ctx->r5 == ctx->r1) {
        // 0x80178078: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_8017809C;
    }
    // 0x80178078: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x8017807C: beq         $a1, $at, L_801780AC
    if (ctx->r5 == ctx->r1) {
        // 0x80178080: nop
    
            goto L_801780AC;
    }
    // 0x80178080: nop

    // 0x80178084: b           L_801780AC
    // 0x80178088: nop

        goto L_801780AC;
    // 0x80178088: nop

L_8017808C:
    // 0x8017808C: jal         0x80177F08
    // 0x80178090: nop

    func_80177F08(rdram, ctx);
        goto after_0;
    // 0x80178090: nop

    after_0:
    // 0x80178094: b           L_801780B8
    // 0x80178098: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801780B8;
    // 0x80178098: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_8017809C:
    // 0x8017809C: jal         0x80177F38
    // 0x801780A0: nop

    func_80177F38(rdram, ctx);
        goto after_1;
    // 0x801780A0: nop

    after_1:
    // 0x801780A4: b           L_801780B8
    // 0x801780A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_801780B8;
    // 0x801780A8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801780AC:
    // 0x801780AC: jal         0x80177FB0
    // 0x801780B0: nop

    func_80177FB0(rdram, ctx);
        goto after_2;
    // 0x801780B0: nop

    after_2:
    // 0x801780B4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_801780B8:
    // 0x801780B8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x801780BC: jr          $ra
    // 0x801780C0: nop

    return;
    // 0x801780C0: nop

;}
RECOMP_FUNC void func_801780C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801780C4: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x801780C8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x801780CC: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x801780D0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x801780D4: lw          $s0, 0x34($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X34);
    // 0x801780D8: lui         $t7, 0x8019
    ctx->r15 = S32(0X8019 << 16);
    // 0x801780DC: addiu       $t7, $t7, 0x27D0
    ctx->r15 = ADD32(ctx->r15, 0X27D0);
    // 0x801780E0: lw          $t6, 0x64($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X64);
    // 0x801780E4: blezl       $t6, L_80178298
    if (SIGNED(ctx->r14) <= 0) {
        // 0x801780E8: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80178298;
    }
    goto skip_0;
    // 0x801780E8: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x801780EC: lw          $t9, 0x0($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X0);
    // 0x801780F0: addiu       $s1, $sp, 0x58
    ctx->r17 = ADD32(ctx->r29, 0X58);
    // 0x801780F4: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x801780F8: sw          $t9, 0x0($s1)
    MEM_W(0X0, ctx->r17) = ctx->r25;
    // 0x801780FC: lw          $t8, 0x4($t7)
    ctx->r24 = MEM_W(ctx->r15, 0X4);
    // 0x80178100: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80178104: addiu       $a0, $sp, 0x4C
    ctx->r4 = ADD32(ctx->r29, 0X4C);
    // 0x80178108: sw          $t8, 0x4($s1)
    MEM_W(0X4, ctx->r17) = ctx->r24;
    // 0x8017810C: lw          $t9, 0x8($t7)
    ctx->r25 = MEM_W(ctx->r15, 0X8);
    // 0x80178110: addiu       $a1, $s0, 0x24
    ctx->r5 = ADD32(ctx->r16, 0X24);
    // 0x80178114: addiu       $a2, $s0, 0x18
    ctx->r6 = ADD32(ctx->r16, 0X18);
    // 0x80178118: sw          $t9, 0x8($s1)
    MEM_W(0X8, ctx->r17) = ctx->r25;
    // 0x8017811C: sw          $zero, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = 0;
    // 0x80178120: jal         0x80013CC4
    // 0x80178124: swc1        $f4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f4.u32l;
    func_80013CC4(rdram, ctx);
        goto after_0;
    // 0x80178124: swc1        $f4, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f4.u32l;
    after_0:
    // 0x80178128: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x8017812C: addiu       $a2, $a2, -0x3068
    ctx->r6 = ADD32(ctx->r6, -0X3068);
    // 0x80178130: addiu       $a0, $sp, 0x40
    ctx->r4 = ADD32(ctx->r29, 0X40);
    // 0x80178134: jal         0x80013E6C
    // 0x80178138: addiu       $a1, $sp, 0x4C
    ctx->r5 = ADD32(ctx->r29, 0X4C);
    vec3f_crossProduct(rdram, ctx);
        goto after_1;
    // 0x80178138: addiu       $a1, $sp, 0x4C
    ctx->r5 = ADD32(ctx->r29, 0X4C);
    after_1:
    // 0x8017813C: addiu       $a0, $sp, 0x34
    ctx->r4 = ADD32(ctx->r29, 0X34);
    // 0x80178140: addiu       $a1, $sp, 0x40
    ctx->r5 = ADD32(ctx->r29, 0X40);
    // 0x80178144: jal         0x80013E6C
    // 0x80178148: addiu       $a2, $sp, 0x4C
    ctx->r6 = ADD32(ctx->r29, 0X4C);
    vec3f_crossProduct(rdram, ctx);
        goto after_2;
    // 0x80178148: addiu       $a2, $sp, 0x4C
    ctx->r6 = ADD32(ctx->r29, 0X4C);
    after_2:
    // 0x8017814C: lbu         $v0, 0x60($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X60);
    // 0x80178150: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80178154: andi        $t0, $v0, 0x8
    ctx->r8 = ctx->r2 & 0X8;
    // 0x80178158: beq         $t0, $zero, L_80178180
    if (ctx->r8 == 0) {
        // 0x8017815C: andi        $t3, $v0, 0x1
        ctx->r11 = ctx->r2 & 0X1;
            goto L_80178180;
    }
    // 0x8017815C: andi        $t3, $v0, 0x1
    ctx->r11 = ctx->r2 & 0X1;
    // 0x80178160: lw          $t1, 0x64($s0)
    ctx->r9 = MEM_W(ctx->r16, 0X64);
    // 0x80178164: lw          $t2, 0x5C($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X5C);
    // 0x80178168: mtc1        $t1, $f6
    ctx->f6.u32l = ctx->r9;
    // 0x8017816C: mtc1        $t2, $f10
    ctx->f10.u32l = ctx->r10;
    // 0x80178170: cvt.s.w     $f8, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    ctx->f8.fl = CVT_S_W(ctx->f6.u32l);
    // 0x80178174: cvt.s.w     $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.fl = CVT_S_W(ctx->f10.u32l);
    // 0x80178178: div.s       $f18, $f8, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f8.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = DIV_S(ctx->f8.fl, ctx->f16.fl);
    // 0x8017817C: swc1        $f18, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->f18.u32l;
L_80178180:
    // 0x80178180: beql        $t3, $zero, L_801781CC
    if (ctx->r11 == 0) {
        // 0x80178184: andi        $t5, $v0, 0x2
        ctx->r13 = ctx->r2 & 0X2;
            goto L_801781CC;
    }
    goto skip_1;
    // 0x80178184: andi        $t5, $v0, 0x2
    ctx->r13 = ctx->r2 & 0X2;
    skip_1:
    // 0x80178188: jal         0x80178064
    // 0x8017818C: lwc1        $f12, 0x30($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X30);
    func_80178064(rdram, ctx);
        goto after_3;
    // 0x8017818C: lwc1        $f12, 0x30($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X30);
    after_3:
    // 0x80178190: lwc1        $f4, 0x58($s0)
    ctx->f4.u32l = MEM_W(ctx->r16, 0X58);
    // 0x80178194: addiu       $a0, $sp, 0x40
    ctx->r4 = ADD32(ctx->r29, 0X40);
    // 0x80178198: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x8017819C: mul.s       $f6, $f0, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f4.fl); 
    ctx->f6.fl = MUL_S(ctx->f0.fl, ctx->f4.fl);
    // 0x801781A0: mfc1        $a2, $f6
    ctx->r6 = (int32_t)ctx->f6.u32l;
    // 0x801781A4: jal         0x80013C10
    // 0x801781A8: nop

    func_80013C10(rdram, ctx);
        goto after_4;
    // 0x801781A8: nop

    after_4:
    // 0x801781AC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801781B0: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x801781B4: jal         0x80013C90
    // 0x801781B8: addiu       $a2, $sp, 0x40
    ctx->r6 = ADD32(ctx->r29, 0X40);
    func_80013C90(rdram, ctx);
        goto after_5;
    // 0x801781B8: addiu       $a2, $sp, 0x40
    ctx->r6 = ADD32(ctx->r29, 0X40);
    after_5:
    // 0x801781BC: addiu       $t4, $zero, 0x1
    ctx->r12 = ADD32(0, 0X1);
    // 0x801781C0: sw          $t4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r12;
    // 0x801781C4: lbu         $v0, 0x60($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X60);
    // 0x801781C8: andi        $t5, $v0, 0x2
    ctx->r13 = ctx->r2 & 0X2;
L_801781CC:
    // 0x801781CC: beq         $t5, $zero, L_80178218
    if (ctx->r13 == 0) {
        // 0x801781D0: lwc1        $f12, 0x30($sp)
        ctx->f12.u32l = MEM_W(ctx->r29, 0X30);
            goto L_80178218;
    }
    // 0x801781D0: lwc1        $f12, 0x30($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X30);
    // 0x801781D4: jal         0x80178064
    // 0x801781D8: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    func_80178064(rdram, ctx);
        goto after_6;
    // 0x801781D8: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    after_6:
    // 0x801781DC: lwc1        $f10, 0x58($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X58);
    // 0x801781E0: addiu       $a0, $sp, 0x34
    ctx->r4 = ADD32(ctx->r29, 0X34);
    // 0x801781E4: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x801781E8: mul.s       $f8, $f0, $f10
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 10);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f10.fl); 
    ctx->f8.fl = MUL_S(ctx->f0.fl, ctx->f10.fl);
    // 0x801781EC: mfc1        $a2, $f8
    ctx->r6 = (int32_t)ctx->f8.u32l;
    // 0x801781F0: jal         0x80013C10
    // 0x801781F4: nop

    func_80013C10(rdram, ctx);
        goto after_7;
    // 0x801781F4: nop

    after_7:
    // 0x801781F8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x801781FC: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80178200: jal         0x80013C90
    // 0x80178204: addiu       $a2, $sp, 0x34
    ctx->r6 = ADD32(ctx->r29, 0X34);
    func_80013C90(rdram, ctx);
        goto after_8;
    // 0x80178204: addiu       $a2, $sp, 0x34
    ctx->r6 = ADD32(ctx->r29, 0X34);
    after_8:
    // 0x80178208: lw          $t6, 0x2C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X2C);
    // 0x8017820C: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80178210: sw          $t7, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r15;
    // 0x80178214: lbu         $v0, 0x60($s0)
    ctx->r2 = MEM_BU(ctx->r16, 0X60);
L_80178218:
    // 0x80178218: andi        $t8, $v0, 0x4
    ctx->r24 = ctx->r2 & 0X4;
    // 0x8017821C: beq         $t8, $zero, L_80178258
    if (ctx->r24 == 0) {
        // 0x80178220: lwc1        $f12, 0x30($sp)
        ctx->f12.u32l = MEM_W(ctx->r29, 0X30);
            goto L_80178258;
    }
    // 0x80178220: lwc1        $f12, 0x30($sp)
    ctx->f12.u32l = MEM_W(ctx->r29, 0X30);
    // 0x80178224: jal         0x80178064
    // 0x80178228: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    func_80178064(rdram, ctx);
        goto after_9;
    // 0x80178228: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    after_9:
    // 0x8017822C: lwc1        $f16, 0x58($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X58);
    // 0x80178230: addiu       $a0, $sp, 0x4C
    ctx->r4 = ADD32(ctx->r29, 0X4C);
    // 0x80178234: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    // 0x80178238: mul.s       $f18, $f0, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f0.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = MUL_S(ctx->f0.fl, ctx->f16.fl);
    // 0x8017823C: mfc1        $a2, $f18
    ctx->r6 = (int32_t)ctx->f18.u32l;
    // 0x80178240: jal         0x80013C10
    // 0x80178244: nop

    func_80013C10(rdram, ctx);
        goto after_10;
    // 0x80178244: nop

    after_10:
    // 0x80178248: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x8017824C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80178250: jal         0x80013C90
    // 0x80178254: addiu       $a2, $sp, 0x4C
    ctx->r6 = ADD32(ctx->r29, 0X4C);
    func_80013C90(rdram, ctx);
        goto after_11;
    // 0x80178254: addiu       $a2, $sp, 0x4C
    ctx->r6 = ADD32(ctx->r29, 0X4C);
    after_11:
L_80178258:
    // 0x80178258: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x8017825C: lw          $a0, 0x27A8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A8);
    // 0x80178260: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x80178264: addiu       $a0, $a0, 0x50
    ctx->r4 = ADD32(ctx->r4, 0X50);
    // 0x80178268: jal         0x80013C90
    // 0x8017826C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013C90(rdram, ctx);
        goto after_12;
    // 0x8017826C: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_12:
    // 0x80178270: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x80178274: lw          $a0, 0x27A8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X27A8);
    // 0x80178278: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    // 0x8017827C: addiu       $a0, $a0, 0x68
    ctx->r4 = ADD32(ctx->r4, 0X68);
    // 0x80178280: jal         0x80013C90
    // 0x80178284: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    func_80013C90(rdram, ctx);
        goto after_13;
    // 0x80178284: or          $a1, $a0, $zero
    ctx->r5 = ctx->r4 | 0;
    after_13:
    // 0x80178288: lw          $t9, 0x64($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X64);
    // 0x8017828C: addiu       $t0, $t9, -0x1
    ctx->r8 = ADD32(ctx->r25, -0X1);
    // 0x80178290: sw          $t0, 0x64($s0)
    MEM_W(0X64, ctx->r16) = ctx->r8;
    // 0x80178294: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80178298:
    // 0x80178298: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8017829C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x801782A0: jr          $ra
    // 0x801782A4: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x801782A4: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void func_801782A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x801782A8: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x801782AC: lw          $t6, -0x53E4($t6)
    ctx->r14 = MEM_W(ctx->r14, -0X53E4);
    // 0x801782B0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x801782B4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x801782B8: beq         $t6, $zero, L_801782D4
    if (ctx->r14 == 0) {
        // 0x801782BC: lw          $v0, 0x34($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X34);
            goto L_801782D4;
    }
    // 0x801782BC: lw          $v0, 0x34($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X34);
    // 0x801782C0: jal         0x801782E4
    // 0x801782C4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_801782E4(rdram, ctx);
        goto after_0;
    // 0x801782C4: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_0:
    // 0x801782C8: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x801782CC: jal         0x8017BAB0
    // 0x801782D0: lw          $a0, 0x14($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X14);
    func_8017BAB0(rdram, ctx);
        goto after_1;
    // 0x801782D0: lw          $a0, 0x14($v0)
    ctx->r4 = MEM_W(ctx->r2, 0X14);
    after_1:
L_801782D4:
    // 0x801782D4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x801782D8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x801782DC: jr          $ra
    // 0x801782E0: nop

    return;
    // 0x801782E0: nop

;}
