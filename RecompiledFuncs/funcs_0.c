#include "recomp.h"
#include "funcs.h"
RECOMP_FUNC void recomp_entrypoint(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000400: lui         $t0, 0x800C
    ctx->r8 = S32(0X800C << 16);
    // 0x80000404: lui         $t1, 0x8
    ctx->r9 = S32(0X8 << 16);
    // 0x80000408: addiu       $t0, $t0, 0x1520
    ctx->r8 = ADD32(ctx->r8, 0X1520);
    // 0x8000040C: ori         $t1, $t1, 0x350
    ctx->r9 = ctx->r9 | 0X350;
L_80000410:
    // 0x80000410: addi        $t1, $t1, -0x8
    ctx->r9 = ADD32(ctx->r9, -0X8);
    // 0x80000414: sw          $zero, 0x0($t0)
    MEM_W(0X0, ctx->r8) = 0;
    // 0x80000418: sw          $zero, 0x4($t0)
    MEM_W(0X4, ctx->r8) = 0;
    // 0x8000041C: bne         $t1, $zero, L_80000410
    if (ctx->r9 != 0) {
        // 0x80000420: addi        $t0, $t0, 0x8
        ctx->r8 = ADD32(ctx->r8, 0X8);
            goto L_80000410;
    }
    // 0x80000420: addi        $t0, $t0, 0x8
    ctx->r8 = ADD32(ctx->r8, 0X8);
    // 0x80000424: lui         $t2, 0x8001
    ctx->r10 = S32(0X8001 << 16);
    // 0x80000428: lui         $sp, 0x800C
    ctx->r29 = S32(0X800C << 16);
    // 0x8000042C: addiu       $t2, $t2, 0x77E0
    ctx->r10 = ADD32(ctx->r10, 0X77E0);
    // 0x80000430: jr          $t2
    // 0x80000434: addiu       $sp, $sp, 0x3680
    ctx->r29 = ADD32(ctx->r29, 0X3680);
    LOOKUP_FUNC(ctx->r10)(rdram, ctx);
    return;
    // 0x80000434: addiu       $sp, $sp, 0x3680
    ctx->r29 = ADD32(ctx->r29, 0X3680);
;}
RECOMP_FUNC void func_80000460(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000460: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x80000464: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80000468: addu        $t6, $t6, $a0
    ctx->r14 = ADD32(ctx->r14, ctx->r4);
    // 0x8000046C: lui         $t7, 0x800B
    ctx->r15 = S32(0X800B << 16);
    // 0x80000470: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80000474: addiu       $t7, $t7, -0x2FE0
    ctx->r15 = ADD32(ctx->r15, -0X2FE0);
    // 0x80000478: sll         $t6, $t6, 4
    ctx->r14 = S32(ctx->r14 << 4);
    // 0x8000047C: addu        $s2, $t6, $t7
    ctx->r18 = ADD32(ctx->r14, ctx->r15);
    // 0x80000480: lw          $t9, -0x48($s2)
    ctx->r25 = MEM_W(ctx->r18, -0X48);
    // 0x80000484: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80000488: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8000048C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80000490: jalr        $t9
    // 0x80000494: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80000494: sw          $a0, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r4;
    after_0:
    // 0x80000498: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8000049C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x800004A0: lh          $t8, 0x84($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X84);
    // 0x800004A4: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x800004A8: addiu       $s1, $s1, 0x1538
    ctx->r17 = ADD32(ctx->r17, 0X1538);
    // 0x800004AC: beq         $t8, $zero, L_800004C0
    if (ctx->r24 == 0) {
        // 0x800004B0: lui         $s0, 0x800C
        ctx->r16 = S32(0X800C << 16);
            goto L_800004C0;
    }
    // 0x800004B0: lui         $s0, 0x800C
    ctx->r16 = S32(0X800C << 16);
    // 0x800004B4: lw          $t0, -0x4C($s2)
    ctx->r8 = MEM_W(ctx->r18, -0X4C);
    // 0x800004B8: b           L_800004C8
    // 0x800004BC: sh          $t0, 0x86($v1)
    MEM_H(0X86, ctx->r3) = ctx->r8;
        goto L_800004C8;
    // 0x800004BC: sh          $t0, 0x86($v1)
    MEM_H(0X86, ctx->r3) = ctx->r8;
L_800004C0:
    // 0x800004C0: lw          $t1, -0x50($s2)
    ctx->r9 = MEM_W(ctx->r18, -0X50);
    // 0x800004C4: sh          $t1, 0x86($v1)
    MEM_H(0X86, ctx->r3) = ctx->r9;
L_800004C8:
    // 0x800004C8: addiu       $s0, $s0, 0x1520
    ctx->r16 = ADD32(ctx->r16, 0X1520);
L_800004CC:
    // (alloc loop logging removed)
    // 0x800004CC: jal         0x80001F30
    // 0x800004D0: lw          $a0, 0x0($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X0);
    func_80001F30(rdram, ctx);
        goto after_1;
    // 0x800004D0: lw          $a0, 0x0($s1)
    ctx->r4 = MEM_W(ctx->r17, 0X0);
    after_1:
    // (post func_80001F30 logging removed)
    // 0x800004D4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x800004D8: jal         0x8000233C
    // 0x800004DC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_8000233C(rdram, ctx);
        goto after_2;
    // 0x800004DC: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_2:
    // (post func_8000233C logging removed)
    // 0x800004E0: beq         $v0, $zero, L_800004CC
    if (ctx->r2 == 0) {
        // 0x800004E4: sw          $v0, 0x0($s0)
        MEM_W(0X0, ctx->r16) = ctx->r2;
            goto L_800004CC;
    }
    // 0x800004E4: sw          $v0, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r2;
    // 0x800004E8: lhu         $t2, 0x2($v0)
    ctx->r10 = MEM_HU(ctx->r2, 0X2);
    // 0x800004EC: lui         $t4, 0x8000
    ctx->r12 = S32(0X8000 << 16);
    // 0x800004F0: addiu       $t4, $t4, 0x2D54
    ctx->r12 = ADD32(ctx->r12, 0X2D54);
    // 0x800004F4: ori         $t3, $t2, 0x4000
    ctx->r11 = ctx->r10 | 0X4000;
    // 0x800004F8: sh          $t3, 0x2($v0)
    MEM_H(0X2, ctx->r2) = ctx->r11;
    // 0x800004FC: lw          $t5, 0x0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X0);
    // 0x80000500: addiu       $a0, $s2, -0x40
    ctx->r4 = ADD32(ctx->r18, -0X40);
    // 0x80000504: addiu       $a2, $zero, 0x40
    ctx->r6 = ADD32(0, 0X40);
    // 0x80000508: sw          $t4, 0x10($t5)
    MEM_W(0X10, ctx->r13) = ctx->r12;
    // 0x8000050C: lw          $t7, 0x0($s0)
    ctx->r15 = MEM_W(ctx->r16, 0X0);
    // 0x80000510: lw          $t6, 0x28($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X28);
    // 0x80000514: sw          $t6, 0x24($t7)
    MEM_W(0X24, ctx->r15) = ctx->r14;
    // 0x80000518: lw          $a1, 0x0($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X0);
    // 0x8000051C: jal         0x80001090
    // 0x80000520: addiu       $a1, $a1, 0x34
    ctx->r5 = ADD32(ctx->r5, 0X34);
    func_80001090(rdram, ctx);
        goto after_3;
    // 0x80000520: addiu       $a1, $a1, 0x34
    ctx->r5 = ADD32(ctx->r5, 0X34);
    after_3:
    // 0x80000524: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80000528: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8000052C: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80000530: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80000534: jr          $ra
    // 0x80000538: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80000538: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_8000053C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000053C: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80000540: addiu       $v1, $v1, 0x1520
    ctx->r3 = ADD32(ctx->r3, 0X1520);
    // 0x80000544: lw          $v0, 0x0($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X0);
    // 0x80000548: negu        $t7, $a0
    ctx->r15 = SUB32(0, ctx->r4);
    // 0x8000054C: lw          $t6, 0x24($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X24);
    // 0x80000550: blez        $t6, L_80000568
    if (SIGNED(ctx->r14) <= 0) {
        // 0x80000554: nop
    
            goto L_80000568;
    }
    // 0x80000554: nop

    // 0x80000558: sw          $t7, 0x24($v0)
    MEM_W(0X24, ctx->r2) = ctx->r15;
    // 0x8000055C: lw          $t9, 0x0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X0);
    // 0x80000560: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x80000564: sh          $t8, 0x6($t9)
    MEM_H(0X6, ctx->r25) = ctx->r24;
L_80000568:
    // 0x80000568: jr          $ra
    // 0x8000056C: nop

    return;
    // 0x8000056C: nop

    // 0x80000570: jr          $ra
    // 0x80000574: nop

    return;
    // 0x80000574: nop

;}
RECOMP_FUNC void func_80000578(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000578: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8000057C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80000580: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80000584: lw          $v0, 0x24($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X24);
    // 0x80000588: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8000058C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000590: bgez        $v0, L_800005DC
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80000594: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_800005DC;
    }
    // 0x80000594: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80000598: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    // 0x8000059C: jal         0x80018E28
    // 0x800005A0: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    func_80018E28(rdram, ctx);
        goto after_0;
    // 0x800005A0: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_0:
    // 0x800005A4: bne         $v0, $zero, L_80000668
    if (ctx->r2 != 0) {
        // 0x800005A8: lw          $v1, 0x24($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X24);
            goto L_80000668;
    }
    // 0x800005A8: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x800005AC: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x800005B0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x800005B4: sh          $t6, -0x7D16($at)
    MEM_H(-0X7D16, ctx->r1) = ctx->r14;
    // 0x800005B8: lhu         $v0, 0x6($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0X6);
    // 0x800005BC: bne         $v0, $zero, L_800005D4
    if (ctx->r2 != 0) {
        // 0x800005C0: addiu       $t7, $v0, -0x1
        ctx->r15 = ADD32(ctx->r2, -0X1);
            goto L_800005D4;
    }
    // 0x800005C0: addiu       $t7, $v0, -0x1
    ctx->r15 = ADD32(ctx->r2, -0X1);
    // 0x800005C4: jal         0x80000460
    // 0x800005C8: negu        $a0, $v1
    ctx->r4 = SUB32(0, ctx->r3);
    func_80000460(rdram, ctx);
        goto after_1;
    // 0x800005C8: negu        $a0, $v1
    ctx->r4 = SUB32(0, ctx->r3);
    after_1:
    // 0x800005CC: b           L_8000066C
    // 0x800005D0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8000066C;
    // 0x800005D0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_800005D4:
    // 0x800005D4: b           L_80000668
    // 0x800005D8: sh          $t7, 0x6($s0)
    MEM_H(0X6, ctx->r16) = ctx->r15;
        goto L_80000668;
    // 0x800005D8: sh          $t7, 0x6($s0)
    MEM_H(0X6, ctx->r16) = ctx->r15;
L_800005DC:
    // 0x800005DC: lbu         $v1, 0x9($s0)
    ctx->r3 = MEM_BU(ctx->r16, 0X9);
    // 0x800005E0: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x800005E4: addu        $t8, $t8, $v0
    ctx->r24 = ADD32(ctx->r24, ctx->r2);
    // 0x800005E8: beq         $v1, $zero, L_80000604
    if (ctx->r3 == 0) {
        // 0x800005EC: sll         $t8, $t8, 4
        ctx->r24 = S32(ctx->r24 << 4);
            goto L_80000604;
    }
    // 0x800005EC: sll         $t8, $t8, 4
    ctx->r24 = S32(ctx->r24 << 4);
    // 0x800005F0: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x800005F4: beq         $v1, $at, L_80000644
    if (ctx->r3 == ctx->r1) {
        // 0x800005F8: nop
    
            goto L_80000644;
    }
    // 0x800005F8: nop

    // 0x800005FC: b           L_80000658
    // 0x80000600: nop

        goto L_80000658;
    // 0x80000600: nop

L_80000604:
    // 0x80000604: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x80000608: addu        $t9, $t9, $t8
    ctx->r25 = ADD32(ctx->r25, ctx->r24);
    // 0x8000060C: lw          $t9, -0x3024($t9)
    ctx->r25 = MEM_W(ctx->r25, -0X3024);
    // 0x80000610: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80000614: jalr        $t9
    // 0x80000618: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x80000618: nop

    after_2:
    // PATCH: Create NI system objects BEFORE child creation
    {
        static int ni_init = 0;
        if (!ni_init) {
            ni_init = 1;
            func_8001A8C4(rdram, ctx);
            ctx->r4 = ctx->r16 | 0; // restore a0
        }
    }
    // After func_80010B84 creates children, force the DMAMgr init.
    // The DMAMgr (object 4) registers itself at 0x800C1600 during its
    // state 0 handler (func_80011D80). This must happen BEFORE the overlay
    // loading in state 1. We dispatch DMAMgr's state 0 manually here.
    // 0x8000061C: jal         0x80010B84
    // 0x80000620: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80010B84(rdram, ctx);
        goto after_3;
    // 0x80000620: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x80000624: lbu         $t0, 0x9($s0)
    ctx->r8 = MEM_BU(ctx->r16, 0X9);
    // 0x80000628: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8000062C: lui         $a1, 0xFFC0
    ctx->r5 = S32(0XFFC0 << 16);
    // 0x80000630: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x80000634: jal         0x80010DD4
    // 0x80000638: sb          $t1, 0x9($s0)
    MEM_B(0X9, ctx->r16) = ctx->r9;
    func_80010DD4(rdram, ctx);
        goto after_4;
    // 0x80000638: sb          $t1, 0x9($s0)
    MEM_B(0X9, ctx->r16) = ctx->r9;
    after_4:
    // 0x8000063C: b           L_8000066C
    // 0x80000640: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
        goto L_8000066C;
    // 0x80000640: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80000644:
    // 0x80000644: jal         0x80010C80
    // 0x80000648: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80010C80(rdram, ctx);
        goto after_5;
    // 0x80000648: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x8000064C: lbu         $t2, 0x9($s0)
    ctx->r10 = MEM_BU(ctx->r16, 0X9);
    // 0x80000650: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80000654: sb          $t3, 0x9($s0)
    MEM_B(0X9, ctx->r16) = ctx->r11;
L_80000658:
    // 0x80000658: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x8000065C: lw          $a1, -0x7D30($a1)
    ctx->r5 = MEM_W(ctx->r5, -0X7D30);
    {
        // PATCH: Force mask to dispatch all children unconditionally.
        ctx->r5 = (gpr)(int32_t)0xFFFFFFFF;
        // Clear bit 30 from overlay object entries so DD4 dispatches them.
        // Bit 30 is set by func_80010B84 for deferred overlay creation.
        // After creation in func_80010C80, bit 30 should be cleared but isn't.
        {
            gpr obase = S32(0x8031AC78);
            for (int i = 0; i < 16; i++) {
                uint32_t spec = (uint32_t)MEM_W(obase, 0x34 + i*4);
                if (spec & 0x40000000) {
                    MEM_W(obase, 0x34 + i*4) = (int32_t)(spec & ~0x40000000);
                }
            }
        }
    }
    // 0x80000660: jal         0x80010DD4
    // 0x80000664: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80010DD4(rdram, ctx);
        goto after_6;
    // 0x80000664: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_6:
L_80000668:
    // 0x80000668: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_8000066C:
    // 0x8000066C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80000670: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x80000674: jr          $ra
    // 0x80000678: nop

    return;
    // 0x80000678: nop

;}
RECOMP_FUNC void func_8000067C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000067C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80000680: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80000684: jal         0x800014B4
    // 0x80000688: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_800014B4(rdram, ctx);
        goto after_0;
    // 0x80000688: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x8000068C: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80000690: lh          $t6, -0x7D0C($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X7D0C);
    // 0x80000694: lui         $a1, 0x802E
    ctx->r5 = S32(0X802E << 16);
    // 0x80000698: addiu       $a1, $a1, 0x3B70
    ctx->r5 = ADD32(ctx->r5, 0X3B70);
    // 0x8000069C: beq         $t6, $zero, L_800006B0
    if (ctx->r14 == 0) {
        // 0x800006A0: or          $a0, $zero, $zero
        ctx->r4 = 0 | 0;
            goto L_800006B0;
    }
    // 0x800006A0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x800006A4: lui         $v0, 0x8063
    ctx->r2 = S32(0X8063 << 16);
    // 0x800006A8: b           L_800006B4
    // 0x800006AC: addiu       $v0, $v0, 0x4000
    ctx->r2 = ADD32(ctx->r2, 0X4000);
        goto L_800006B4;
    // 0x800006AC: addiu       $v0, $v0, 0x4000
    ctx->r2 = ADD32(ctx->r2, 0X4000);
L_800006B0:
    // 0x800006B0: lui         $v0, 0x8040
    ctx->r2 = S32(0X8040 << 16);
L_800006B4:
    // 0x800006B4: subu        $a2, $v0, $a1
    ctx->r6 = SUB32(ctx->r2, ctx->r5);
    // 0x800006B8: jal         0x800013E8
    // 0x800006BC: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    func_800013E8(rdram, ctx);
        goto after_1;
    // 0x800006BC: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    after_1:
    // 0x800006C0: lui         $t7, 0x802A
    ctx->r15 = S32(0X802A << 16);
    // 0x800006C4: lui         $t8, 0x8029
    ctx->r24 = S32(0X8029 << 16);
    // 0x800006C8: addiu       $t8, $t8, 0x3B70
    ctx->r24 = ADD32(ctx->r24, 0X3B70);
    // 0x800006CC: addiu       $t7, $t7, 0x3B70
    ctx->r15 = ADD32(ctx->r15, 0X3B70);
    // 0x800006D0: lui         $a1, 0x8029
    ctx->r5 = S32(0X8029 << 16);
    // 0x800006D4: addiu       $a1, $a1, 0x3B70
    ctx->r5 = ADD32(ctx->r5, 0X3B70);
    // 0x800006D8: subu        $a2, $t7, $t8
    ctx->r6 = SUB32(ctx->r15, ctx->r24);
    // 0x800006DC: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    // 0x800006E0: jal         0x800013E8
    // 0x800006E4: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    func_800013E8(rdram, ctx);
        goto after_2;
    // 0x800006E4: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    after_2:
    // 0x800006E8: lui         $t9, 0x802E
    ctx->r25 = S32(0X802E << 16);
    // 0x800006EC: lui         $t0, 0x802A
    ctx->r8 = S32(0X802A << 16);
    // 0x800006F0: addiu       $t0, $t0, 0x3B70
    ctx->r8 = ADD32(ctx->r8, 0X3B70);
    // 0x800006F4: addiu       $t9, $t9, 0x3B70
    ctx->r25 = ADD32(ctx->r25, 0X3B70);
    // 0x800006F8: lui         $a1, 0x802A
    ctx->r5 = S32(0X802A << 16);
    // 0x800006FC: addiu       $a1, $a1, 0x3B70
    ctx->r5 = ADD32(ctx->r5, 0X3B70);
    // 0x80000700: subu        $a2, $t9, $t0
    ctx->r6 = SUB32(ctx->r25, ctx->r8);
    // 0x80000704: addiu       $a0, $zero, 0x6
    ctx->r4 = ADD32(0, 0X6);
    // 0x80000708: jal         0x800013E8
    // 0x8000070C: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    func_800013E8(rdram, ctx);
        goto after_3;
    // 0x8000070C: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    after_3:
    // 0x80000710: jal         0x80001F30
    // 0x80000714: addiu       $a0, $zero, 0x12C
    ctx->r4 = ADD32(0, 0X12C);
    func_80001F30(rdram, ctx);
        goto after_4;
    // 0x80000714: addiu       $a0, $zero, 0x12C
    ctx->r4 = ADD32(0, 0X12C);
    after_4:
    // 0x80000718: lui         $t1, 0x801D
    ctx->r9 = S32(0X801D << 16);
    // 0x8000071C: lh          $t1, -0x7D0C($t1)
    ctx->r9 = MEM_H(ctx->r9, -0X7D0C);
    // 0x80000720: mtc1        $zero, $f4
    ctx->f4.u32l = 0;
    // 0x80000724: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80000728: beql        $t1, $zero, L_80000748
    if (ctx->r9 == 0) {
        // 0x8000072C: mtc1        $at, $f1
        ctx->f_odd[(1 - 1) * 2] = ctx->r1;
            goto L_80000748;
    }
    goto skip_0;
    // 0x8000072C: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    skip_0:
    // 0x80000730: lui         $at, 0x3FF8
    ctx->r1 = S32(0X3FF8 << 16);
    // 0x80000734: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    // 0x80000738: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8000073C: b           L_80000754
    // 0x80000740: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
        goto L_80000754;
    // 0x80000740: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x80000744: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
L_80000748:
    // 0x80000748: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x8000074C: nop

    // 0x80000750: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
L_80000754:
    // 0x80000754: mtc1        $at, $f5
    ctx->f_odd[(5 - 1) * 2] = ctx->r1;
    // 0x80000758: lui         $at, 0x3FFC
    ctx->r1 = S32(0X3FFC << 16);
    // 0x8000075C: mtc1        $at, $f9
    ctx->f_odd[(9 - 1) * 2] = ctx->r1;
    // 0x80000760: mul.d       $f6, $f0, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 4);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f4.d); 
    ctx->f6.d = MUL_D(ctx->f0.d, ctx->f4.d);
    // 0x80000764: mtc1        $zero, $f8
    ctx->f8.u32l = 0;
    // 0x80000768: addiu       $a0, $zero, 0x200
    ctx->r4 = ADD32(0, 0X200);
    // 0x8000076C: addiu       $a2, $zero, 0x100
    ctx->r6 = ADD32(0, 0X100);
    // 0x80000770: mul.d       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.d); NAN_CHECK(ctx->f8.d); 
    ctx->f10.d = MUL_D(ctx->f6.d, ctx->f8.d);
    // 0x80000774: trunc.w.d   $f16, $f10
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    ctx->f16.u32l = TRUNC_W_D(ctx->f10.d);
    // 0x80000778: mfc1        $a1, $f16
    ctx->r5 = (int32_t)ctx->f16.u32l;
    // 0x8000077C: jal         0x80004CB0
    // 0x80000780: nop

    func_80004CB0(rdram, ctx);
        goto after_5;
    // 0x80000780: nop

    after_5:
    // 0x80000784: ori         $a0, $zero, 0x8000
    ctx->r4 = 0 | 0X8000;
    // 0x80000788: jal         0x8000B5A0
    // 0x8000078C: addiu       $a1, $zero, 0xD7
    ctx->r5 = ADD32(0, 0XD7);
    func_8000B5A0(rdram, ctx);
        goto after_6;
    // 0x8000078C: addiu       $a1, $zero, 0xD7
    ctx->r5 = ADD32(0, 0XD7);
    after_6:
    // 0x80000790: jal         0x80001940
    // 0x80000794: nop

    func_80001940(rdram, ctx);
        goto after_7;
    // 0x80000794: nop

    after_7:
    // 0x80000798: jal         0x80000BD0
    // 0x8000079C: nop

    func_80000BD0(rdram, ctx);
        goto after_8;
    // 0x8000079C: nop

    after_8:
    // 0x800007A0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800007A4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800007A8: jr          $ra
    // 0x800007AC: nop

    return;
    // 0x800007AC: nop

;}
RECOMP_FUNC void func_800007B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800007B0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x800007B4: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x800007B8: addiu       $t6, $zero, 0x4
    ctx->r14 = ADD32(0, 0X4);
    // 0x800007BC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800007C0: sh          $t6, 0x80($v0)
    MEM_H(0X80, ctx->r2) = ctx->r14;
    // 0x800007C4: lh          $t7, 0x80($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X80);
    // 0x800007C8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x800007CC: lui         $a0, 0x8019
    ctx->r4 = S32(0X8019 << 16);
    // 0x800007D0: lui         $v1, 0x8019
    ctx->r3 = S32(0X8019 << 16);
    // 0x800007D4: lui         $a1, 0x8019
    ctx->r5 = S32(0X8019 << 16);
    // 0x800007D8: lui         $a2, 0x8019
    ctx->r6 = S32(0X8019 << 16);
    // 0x800007DC: lui         $a3, 0x8019
    ctx->r7 = S32(0X8019 << 16);
    // 0x800007E0: lui         $t0, 0x8019
    ctx->r8 = S32(0X8019 << 16);
    // 0x800007E4: lui         $t1, 0x8019
    ctx->r9 = S32(0X8019 << 16);
    // 0x800007E8: lui         $t2, 0x8019
    ctx->r10 = S32(0X8019 << 16);
    // 0x800007EC: addiu       $t2, $t2, 0x27A4
    ctx->r10 = ADD32(ctx->r10, 0X27A4);
    // 0x800007F0: addiu       $t1, $t1, 0x27A8
    ctx->r9 = ADD32(ctx->r9, 0X27A8);
    // 0x800007F4: addiu       $t0, $t0, 0x27AC
    ctx->r8 = ADD32(ctx->r8, 0X27AC);
    // 0x800007F8: addiu       $a3, $a3, 0x27B0
    ctx->r7 = ADD32(ctx->r7, 0X27B0);
    // 0x800007FC: addiu       $a2, $a2, 0x27B4
    ctx->r6 = ADD32(ctx->r6, 0X27B4);
    // 0x80000800: addiu       $a1, $a1, 0x27B8
    ctx->r5 = ADD32(ctx->r5, 0X27B8);
    // 0x80000804: addiu       $v1, $v1, 0x27C0
    ctx->r3 = ADD32(ctx->r3, 0X27C0);
    // 0x80000808: addiu       $a0, $a0, 0x27BC
    ctx->r4 = ADD32(ctx->r4, 0X27BC);
    // 0x8000080C: sw          $zero, 0x2B70($v0)
    MEM_W(0X2B70, ctx->r2) = 0;
    // 0x80000810: sw          $zero, 0x2B6C($v0)
    MEM_W(0X2B6C, ctx->r2) = 0;
    // 0x80000814: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
    // 0x80000818: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
    // 0x8000081C: sw          $zero, 0x0($a1)
    MEM_W(0X0, ctx->r5) = 0;
    // 0x80000820: sw          $zero, 0x0($a2)
    MEM_W(0X0, ctx->r6) = 0;
    // 0x80000824: sw          $zero, 0x0($a3)
    MEM_W(0X0, ctx->r7) = 0;
    // 0x80000828: sw          $zero, 0x0($t0)
    MEM_W(0X0, ctx->r8) = 0;
    // 0x8000082C: sw          $zero, 0x0($t1)
    MEM_W(0X0, ctx->r9) = 0;
    // 0x80000830: sw          $zero, 0x0($t2)
    MEM_W(0X0, ctx->r10) = 0;
    // 0x80000834: lui         $at, 0x8019
    ctx->r1 = S32(0X8019 << 16);
    // 0x80000838: sh          $t7, 0x82($v0)
    MEM_H(0X82, ctx->r2) = ctx->r15;
    // 0x8000083C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80000840: sw          $zero, 0x27A0($at)
    MEM_W(0X27A0, ctx->r1) = 0;
    // 0x80000844: jal         0x80014C7C
    // 0x80000848: sw          $zero, 0x295C($v0)
    // Original code zeros sys+0x295C (NI system ptr) during scene reset.
    // NI init (overlay_system_func_801CB5CC) never runs, so it stays NULL.
    // This blocks the rendering path (func_8017A600 early-exits).
    MEM_W(0X295C, ctx->r2) = 0;
    func_80014C7C(rdram, ctx);
        goto after_0;
    // 0x80000848: sw          $zero, 0x295C($v0)
    MEM_W(0X295C, ctx->r2) = 0;
    after_0:
    // 0x8000084C: jal         0x800404D0
    // 0x80000850: nop

    func_800404D0(rdram, ctx);
        goto after_1;
    // 0x80000850: nop

    after_1:
    // 0x80000854: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80000858: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8000085C: jr          $ra
    // 0x80000860: nop

    return;
    // 0x80000860: nop

;}
RECOMP_FUNC void func_80000864(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000864: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80000868: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x8000086C: lh          $t6, 0x26($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X26);
    // 0x80000870: lui         $a0, 0x800D
    ctx->r4 = S32(0X800D << 16);
    // 0x80000874: addiu       $a0, $a0, -0x18A0
    ctx->r4 = ADD32(ctx->r4, -0X18A0);
    // 0x80000878: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x8000087C: addu        $t8, $t0, $t7
    ctx->r24 = ADD32(ctx->r8, ctx->r15);
    // 0x80000880: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x80000884: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80000888: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8000088C: sw          $t9, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r25;
    // 0x80000890: addiu       $t2, $t9, 0x8
    ctx->r10 = ADD32(ctx->r25, 0X8);
    // 0x80000894: sw          $t2, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r10;
    // 0x80000898: lui         $t3, 0xDB06
    ctx->r11 = S32(0XDB06 << 16);
    // 0x8000089C: sw          $t3, 0x0($t9)
    MEM_W(0X0, ctx->r25) = ctx->r11;
    // 0x800008A0: jal         0x80018374
    // 0x800008A4: sw          $zero, 0x4($t9)
    MEM_W(0X4, ctx->r25) = 0;
    func_80018374(rdram, ctx);
        goto after_0;
    // 0x800008A4: sw          $zero, 0x4($t9)
    MEM_W(0X4, ctx->r25) = 0;
    after_0:
    // 0x800008A8: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x800008AC: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x800008B0: lh          $t4, 0x82($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X82);
    // 0x800008B4: beql        $t4, $zero, L_800008D0
    if (ctx->r12 == 0) {
        // 0x800008B8: lh          $t5, 0x24($t0)
        ctx->r13 = MEM_H(ctx->r8, 0X24);
            goto L_800008D0;
    }
    goto skip_0;
    // 0x800008B8: lh          $t5, 0x24($t0)
    ctx->r13 = MEM_H(ctx->r8, 0X24);
    skip_0:
    // 0x800008BC: jal         0x800193F4
    // 0x800008C0: nop

    func_800193F4(rdram, ctx);
        goto after_1;
    // 0x800008C0: nop

    after_1:
    // 0x800008C4: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x800008C8: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x800008CC: lh          $t5, 0x24($t0)
    ctx->r13 = MEM_H(ctx->r8, 0X24);
L_800008D0:
    // 0x800008D0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x800008D4: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x800008D8: slti        $at, $t5, 0x3
    ctx->r1 = SIGNED(ctx->r13) < 0X3 ? 1 : 0;
    // 0x800008DC: bne         $at, $zero, L_800008EC
    if (ctx->r1 != 0) {
        // 0x800008E0: or          $a2, $zero, $zero
        ctx->r6 = 0 | 0;
            goto L_800008EC;
    }
    // 0x800008E0: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    // 0x800008E4: b           L_800008F0
    // 0x800008E8: lh          $t1, 0x2C($t0)
    ctx->r9 = MEM_H(ctx->r8, 0X2C);
        goto L_800008F0;
    // 0x800008E8: lh          $t1, 0x2C($t0)
    ctx->r9 = MEM_H(ctx->r8, 0X2C);
L_800008EC:
    // 0x800008EC: lh          $t1, 0x78($t0)
    ctx->r9 = MEM_H(ctx->r8, 0X78);
L_800008F0:
    // 0x800008F0: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x800008F4: addiu       $v1, $t1, 0x1
    ctx->r3 = ADD32(ctx->r9, 0X1);
L_800008F8:
    // 0x800008F8: slti        $at, $v1, 0x3
    ctx->r1 = SIGNED(ctx->r3) < 0X3 ? 1 : 0;
    // 0x800008FC: bne         $at, $zero, L_80000908
    if (ctx->r1 != 0) {
        // 0x80000900: nop
    
            goto L_80000908;
    }
    // 0x80000900: nop

    // 0x80000904: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80000908:
    // 0x80000908: beq         $v1, $t1, L_8000095C
    if (ctx->r3 == ctx->r9) {
        // 0x8000090C: sll         $t6, $v1, 1
        ctx->r14 = S32(ctx->r3 << 1);
            goto L_8000095C;
    }
    // 0x8000090C: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x80000910: addu        $v0, $t0, $t6
    ctx->r2 = ADD32(ctx->r8, ctx->r14);
    // 0x80000914: lh          $t7, 0x60($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X60);
    // 0x80000918: lh          $t8, 0x66($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X66);
    // 0x8000091C: lh          $t9, 0x6C($v0)
    ctx->r25 = MEM_H(ctx->r2, 0X6C);
    // 0x80000920: lh          $t2, 0x72($v0)
    ctx->r10 = MEM_H(ctx->r2, 0X72);
    // 0x80000924: addu        $a3, $a3, $t7
    ctx->r7 = ADD32(ctx->r7, ctx->r15);
    // 0x80000928: addu        $a2, $a2, $t8
    ctx->r6 = ADD32(ctx->r6, ctx->r24);
    // 0x8000092C: addu        $a1, $a1, $t9
    ctx->r5 = ADD32(ctx->r5, ctx->r25);
    // 0x80000930: addu        $a0, $a0, $t2
    ctx->r4 = ADD32(ctx->r4, ctx->r10);
    // 0x80000934: sll         $a3, $a3, 16
    ctx->r7 = S32(ctx->r7 << 16);
    // 0x80000938: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x8000093C: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80000940: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80000944: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80000948: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x8000094C: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80000950: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80000954: b           L_800008F8
    // 0x80000958: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
        goto L_800008F8;
    // 0x80000958: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
L_8000095C:
    // 0x8000095C: lh          $v1, 0x28($t0)
    ctx->r3 = MEM_H(ctx->r8, 0X28);
    // 0x80000960: sll         $t3, $t1, 1
    ctx->r11 = S32(ctx->r9 << 1);
    // 0x80000964: addu        $v0, $t0, $t3
    ctx->r2 = ADD32(ctx->r8, ctx->r11);
    // 0x80000968: beql        $v1, $zero, L_800009B4
    if (ctx->r3 == 0) {
        // 0x8000096C: lh          $t8, 0x50($t0)
        ctx->r24 = MEM_H(ctx->r8, 0X50);
            goto L_800009B4;
    }
    goto skip_1;
    // 0x8000096C: lh          $t8, 0x50($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X50);
    skip_1:
    // 0x80000970: lh          $t4, 0x60($v0)
    ctx->r12 = MEM_H(ctx->r2, 0X60);
    // 0x80000974: lh          $t5, 0x66($v0)
    ctx->r13 = MEM_H(ctx->r2, 0X66);
    // 0x80000978: lh          $t6, 0x6C($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X6C);
    // 0x8000097C: lh          $t7, 0x72($v0)
    ctx->r15 = MEM_H(ctx->r2, 0X72);
    // 0x80000980: addu        $a3, $a3, $t4
    ctx->r7 = ADD32(ctx->r7, ctx->r12);
    // 0x80000984: addu        $a2, $a2, $t5
    ctx->r6 = ADD32(ctx->r6, ctx->r13);
    // 0x80000988: addu        $a1, $a1, $t6
    ctx->r5 = ADD32(ctx->r5, ctx->r14);
    // 0x8000098C: addu        $a0, $a0, $t7
    ctx->r4 = ADD32(ctx->r4, ctx->r15);
    // 0x80000990: sll         $a3, $a3, 16
    ctx->r7 = S32(ctx->r7 << 16);
    // 0x80000994: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x80000998: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x8000099C: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x800009A0: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x800009A4: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x800009A8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800009AC: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x800009B0: lh          $t8, 0x50($t0)
    ctx->r24 = MEM_H(ctx->r8, 0X50);
L_800009B4:
    // 0x800009B4: lh          $t2, 0x52($t0)
    ctx->r10 = MEM_H(ctx->r8, 0X52);
    // 0x800009B8: lh          $t4, 0x54($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X54);
    // 0x800009BC: lh          $t6, 0x56($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X56);
    // 0x800009C0: subu        $t9, $t8, $a3
    ctx->r25 = SUB32(ctx->r24, ctx->r7);
    // 0x800009C4: subu        $t3, $t2, $a2
    ctx->r11 = SUB32(ctx->r10, ctx->r6);
    // 0x800009C8: subu        $t5, $t4, $a1
    ctx->r13 = SUB32(ctx->r12, ctx->r5);
    // 0x800009CC: subu        $t7, $t6, $a0
    ctx->r15 = SUB32(ctx->r14, ctx->r4);
    // 0x800009D0: sh          $t9, 0x50($t0)
    MEM_H(0X50, ctx->r8) = ctx->r25;
    // 0x800009D4: sh          $t3, 0x52($t0)
    MEM_H(0X52, ctx->r8) = ctx->r11;
    // 0x800009D8: sh          $t5, 0x54($t0)
    MEM_H(0X54, ctx->r8) = ctx->r13;
    // 0x800009DC: bne         $v1, $zero, L_800009FC
    if (ctx->r3 != 0) {
        // 0x800009E0: sh          $t7, 0x56($t0)
        MEM_H(0X56, ctx->r8) = ctx->r15;
            goto L_800009FC;
    }
    // 0x800009E0: sh          $t7, 0x56($t0)
    MEM_H(0X56, ctx->r8) = ctx->r15;
    // 0x800009E4: sll         $t8, $t1, 1
    ctx->r24 = S32(ctx->r9 << 1);
    // 0x800009E8: addu        $v0, $t0, $t8
    ctx->r2 = ADD32(ctx->r8, ctx->r24);
    // 0x800009EC: sh          $zero, 0x60($v0)
    MEM_H(0X60, ctx->r2) = 0;
    // 0x800009F0: sh          $zero, 0x66($v0)
    MEM_H(0X66, ctx->r2) = 0;
    // 0x800009F4: sh          $zero, 0x6C($v0)
    MEM_H(0X6C, ctx->r2) = 0;
    // 0x800009F8: sh          $zero, 0x72($v0)
    MEM_H(0X72, ctx->r2) = 0;
L_800009FC:
    // 0x800009FC: jal         0x800191A0
    // 0x80000A00: nop

    func_800191A0(rdram, ctx);
        goto after_2;
    // 0x80000A00: nop

    after_2:
    // 0x80000A04: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80000A08: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80000A0C: lh          $t9, 0x80($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X80);
    // 0x80000A10: beql        $t9, $zero, L_80000A24
    if (ctx->r25 == 0) {
        // 0x80000A14: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80000A24;
    }
    goto skip_2;
    // 0x80000A14: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_2:
    // 0x80000A18: jal         0x800192F8
    // 0x80000A1C: nop

    func_800192F8(rdram, ctx);
        goto after_3;
    // 0x80000A1C: nop

    after_3:
    // 0x80000A20: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80000A24:
    // 0x80000A24: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80000A28: jr          $ra
    // 0x80000A2C: nop

    return;
    // 0x80000A2C: nop

;}
RECOMP_FUNC void func_80000A30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000A30: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80000A34: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80000A38: jal         0x80010604
    // 0x80000A3C: nop

    func_80010604(rdram, ctx);
        goto after_0;
    // 0x80000A3C: nop

    after_0:
    // 0x80000A40: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80000A44: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80000A48: lh          $t6, 0x8C($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X8C);
    // 0x80000A4C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000A50: beql        $t6, $zero, L_80000A8C
    if (ctx->r14 == 0) {
        // 0x80000A54: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80000A8C;
    }
    goto skip_0;
    // 0x80000A54: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80000A58: jal         0x80018E28
    // 0x80000A5C: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    func_80018E28(rdram, ctx);
        goto after_1;
    // 0x80000A5C: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    after_1:
    // 0x80000A60: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80000A64: sltiu       $at, $v0, 0x2
    ctx->r1 = ctx->r2 < 0X2 ? 1 : 0;
    // 0x80000A68: beq         $at, $zero, L_80000A80
    if (ctx->r1 == 0) {
        // 0x80000A6C: addiu       $v1, $v1, -0x7D40
        ctx->r3 = ADD32(ctx->r3, -0X7D40);
            goto L_80000A80;
    }
    // 0x80000A6C: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80000A70: jal         0x80019034
    // 0x80000A74: sh          $zero, 0x28($v1)
    MEM_H(0X28, ctx->r3) = 0;
    func_80019034(rdram, ctx);
        goto after_2;
    // 0x80000A74: sh          $zero, 0x28($v1)
    MEM_H(0X28, ctx->r3) = 0;
    after_2:
    // 0x80000A78: b           L_80000A8C
    // 0x80000A7C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80000A8C;
    // 0x80000A7C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80000A80:
    // 0x80000A80: addiu       $t7, $zero, 0x1
    ctx->r15 = ADD32(0, 0X1);
    // 0x80000A84: sh          $t7, 0x28($v1)
    MEM_H(0X28, ctx->r3) = ctx->r15;
    // 0x80000A88: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80000A8C:
    // 0x80000A8C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80000A90: jr          $ra
    // 0x80000A94: nop

    return;
    // 0x80000A94: nop

;}
RECOMP_FUNC void func_80000A98(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000A98: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x80000A9C: lh          $t6, -0x5188($t6)
    ctx->r14 = MEM_H(ctx->r14, -0X5188);
    // 0x80000AA0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80000AA4: lui         $t8, 0x800B
    ctx->r24 = S32(0X800B << 16);
    // 0x80000AA8: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80000AAC: subu        $t7, $t7, $t6
    ctx->r15 = SUB32(ctx->r15, ctx->r14);
    // 0x80000AB0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80000AB4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80000AB8: sll         $t7, $t7, 2
    ctx->r15 = S32(ctx->r15 << 2);
    // 0x80000ABC: addiu       $t8, $t8, -0x2C20
    ctx->r24 = ADD32(ctx->r24, -0X2C20);
    // 0x80000AC0: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80000AC4: jal         0x800014B4
    // 0x80000AC8: addu        $s0, $t7, $t8
    ctx->r16 = ADD32(ctx->r15, ctx->r24);
    func_800014B4(rdram, ctx);
        goto after_0;
    // 0x80000AC8: addu        $s0, $t7, $t8
    ctx->r16 = ADD32(ctx->r15, ctx->r24);
    after_0:
    // 0x80000ACC: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80000AD0: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80000AD4: lh          $t9, 0x2BB8($v1)
    ctx->r25 = MEM_H(ctx->r3, 0X2BB8);
    // 0x80000AD8: lh          $t1, 0x34($v1)
    ctx->r9 = MEM_H(ctx->r3, 0X34);
    // 0x80000ADC: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x80000AE0: sll         $t0, $t9, 3
    ctx->r8 = S32(ctx->r25 << 3);
    // 0x80000AE4: addu        $a1, $a1, $t0
    ctx->r5 = ADD32(ctx->r5, ctx->r8);
    // 0x80000AE8: beq         $t1, $zero, L_80000AFC
    if (ctx->r9 == 0) {
        // 0x80000AEC: lw          $a1, 0x2C5C($a1)
        ctx->r5 = MEM_W(ctx->r5, 0X2C5C);
            goto L_80000AFC;
    }
    // 0x80000AEC: lw          $a1, 0x2C5C($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X2C5C);
    // 0x80000AF0: lui         $v0, 0x8063
    ctx->r2 = S32(0X8063 << 16);
    // 0x80000AF4: b           L_80000B00
    // 0x80000AF8: addiu       $v0, $v0, 0x4000
    ctx->r2 = ADD32(ctx->r2, 0X4000);
        goto L_80000B00;
    // 0x80000AF8: addiu       $v0, $v0, 0x4000
    ctx->r2 = ADD32(ctx->r2, 0X4000);
L_80000AFC:
    // 0x80000AFC: lui         $v0, 0x8040
    ctx->r2 = S32(0X8040 << 16);
L_80000B00:
    // 0x80000B00: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80000B04: subu        $a2, $v0, $a1
    ctx->r6 = SUB32(ctx->r2, ctx->r5);
    // 0x80000B08: jal         0x800013E8
    // 0x80000B0C: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    func_800013E8(rdram, ctx);
        goto after_1;
    // 0x80000B0C: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    after_1:
    // 0x80000B10: lui         $t2, 0x802A
    ctx->r10 = S32(0X802A << 16);
    // 0x80000B14: lui         $t3, 0x8029
    ctx->r11 = S32(0X8029 << 16);
    // 0x80000B18: addiu       $t3, $t3, 0x3B70
    ctx->r11 = ADD32(ctx->r11, 0X3B70);
    // 0x80000B1C: addiu       $t2, $t2, 0x3B70
    ctx->r10 = ADD32(ctx->r10, 0X3B70);
    // 0x80000B20: lui         $a1, 0x8029
    ctx->r5 = S32(0X8029 << 16);
    // 0x80000B24: addiu       $a1, $a1, 0x3B70
    ctx->r5 = ADD32(ctx->r5, 0X3B70);
    // 0x80000B28: subu        $a2, $t2, $t3
    ctx->r6 = SUB32(ctx->r10, ctx->r11);
    // 0x80000B2C: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    // 0x80000B30: jal         0x800013E8
    // 0x80000B34: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    func_800013E8(rdram, ctx);
        goto after_2;
    // 0x80000B34: addiu       $a3, $zero, 0x4000
    ctx->r7 = ADD32(0, 0X4000);
    after_2:
    // 0x80000B38: jal         0x80001F30
    // 0x80000B3C: lh          $a0, 0x0($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X0);
    func_80001F30(rdram, ctx);
        goto after_3;
    // 0x80000B3C: lh          $a0, 0x0($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X0);
    after_3:
    // 0x80000B40: lui         $t4, 0x801D
    ctx->r12 = S32(0X801D << 16);
    // 0x80000B44: lh          $t4, -0x7D0C($t4)
    ctx->r12 = MEM_H(ctx->r12, -0X7D0C);
    // 0x80000B48: lui         $at, 0x3FF0
    ctx->r1 = S32(0X3FF0 << 16);
    // 0x80000B4C: beql        $t4, $zero, L_80000B6C
    if (ctx->r12 == 0) {
        // 0x80000B50: mtc1        $at, $f1
        ctx->f_odd[(1 - 1) * 2] = ctx->r1;
            goto L_80000B6C;
    }
    goto skip_0;
    // 0x80000B50: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    skip_0:
    // 0x80000B54: lui         $at, 0x3FF8
    ctx->r1 = S32(0X3FF8 << 16);
    // 0x80000B58: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
    // 0x80000B5C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80000B60: b           L_80000B78
    // 0x80000B64: lh          $t5, 0x4($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X4);
        goto L_80000B78;
    // 0x80000B64: lh          $t5, 0x4($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X4);
    // 0x80000B68: mtc1        $at, $f1
    ctx->f_odd[(1 - 1) * 2] = ctx->r1;
L_80000B6C:
    // 0x80000B6C: mtc1        $zero, $f0
    ctx->f0.u32l = 0;
    // 0x80000B70: nop

    // 0x80000B74: lh          $t5, 0x4($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X4);
L_80000B78:
    // 0x80000B78: lh          $a0, 0x2($s0)
    ctx->r4 = MEM_H(ctx->r16, 0X2);
    // 0x80000B7C: lh          $a2, 0x6($s0)
    ctx->r6 = MEM_H(ctx->r16, 0X6);
    // 0x80000B80: mtc1        $t5, $f4
    ctx->f4.u32l = ctx->r13;
    // 0x80000B84: nop

    // 0x80000B88: cvt.d.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.d = CVT_D_W(ctx->f4.u32l);
    // 0x80000B8C: mul.d       $f8, $f0, $f6
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 0);
    CHECK_FR(ctx, 6);
    NAN_CHECK(ctx->f0.d); NAN_CHECK(ctx->f6.d); 
    ctx->f8.d = MUL_D(ctx->f0.d, ctx->f6.d);
    // 0x80000B90: trunc.w.d   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_D(ctx->f8.d);
    // 0x80000B94: mfc1        $a1, $f10
    ctx->r5 = (int32_t)ctx->f10.u32l;
    // 0x80000B98: jal         0x80004CB0
    // 0x80000B9C: nop

    func_80004CB0(rdram, ctx);
        goto after_4;
    // 0x80000B9C: nop

    after_4:
    // 0x80000BA0: ori         $a0, $zero, 0x8000
    ctx->r4 = 0 | 0X8000;
    // 0x80000BA4: jal         0x8000B5A0
    // 0x80000BA8: addiu       $a1, $zero, 0xD7
    ctx->r5 = ADD32(0, 0XD7);
    func_8000B5A0(rdram, ctx);
        goto after_5;
    // 0x80000BA8: addiu       $a1, $zero, 0xD7
    ctx->r5 = ADD32(0, 0XD7);
    after_5:
    // 0x80000BAC: jal         0x80001940
    // 0x80000BB0: nop

    func_80001940(rdram, ctx);
        goto after_6;
    // 0x80000BB0: nop

    after_6:
    // 0x80000BB4: jal         0x80000BD0
    // 0x80000BB8: nop

    func_80000BD0(rdram, ctx);
        goto after_7;
    // 0x80000BB8: nop

    after_7:
    // 0x80000BBC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80000BC0: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80000BC4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80000BC8: jr          $ra
    // 0x80000BCC: nop

    return;
    // 0x80000BCC: nop

;}
RECOMP_FUNC void func_80000BD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000BD0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80000BD4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80000BD8: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80000BDC: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80000BE0: lh          $t6, 0x36($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X36);
    // 0x80000BE4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80000BE8: beql        $t6, $zero, L_80000F1C
    if (ctx->r14 == 0) {
        // 0x80000BEC: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80000F1C;
    }
    goto skip_0;
    // 0x80000BEC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80000BF0: lh          $t7, 0x38($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X38);
    // 0x80000BF4: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x80000BF8: bnel        $t7, $zero, L_80000F1C
    if (ctx->r15 != 0) {
        // 0x80000BFC: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80000F1C;
    }
    goto skip_1;
    // 0x80000BFC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x80000C00: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x80000C04: lwc1        $f8, 0x4($s0)
    ctx->f8.u32l = MEM_W(ctx->r16, 0X4);
    // 0x80000C08: lh          $t8, 0x34($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X34);
    // 0x80000C0C: swc1        $f4, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f4.u32l;
    // 0x80000C10: lwc1        $f6, 0x8($s0)
    ctx->f6.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80000C14: lui         $t6, 0x8020
    ctx->r14 = S32(0X8020 << 16);
    // 0x80000C18: lui         $t7, 0x8022
    ctx->r15 = S32(0X8022 << 16);
    // 0x80000C1C: div.s       $f10, $f6, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 8);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f8.fl); 
    ctx->f10.fl = DIV_S(ctx->f6.fl, ctx->f8.fl);
    // 0x80000C20: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80000C24: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80000C28: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80000C2C: sh          $zero, 0x84($s0)
    MEM_H(0X84, ctx->r16) = 0;
    // 0x80000C30: addiu       $t5, $zero, 0x18
    ctx->r13 = ADD32(0, 0X18);
    // 0x80000C34: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x80000C38: addiu       $t6, $t6, 0x0
    ctx->r14 = ADD32(ctx->r14, 0X0);
    // 0x80000C3C: addiu       $t7, $t7, 0x5800
    ctx->r15 = ADD32(ctx->r15, 0X5800);
    // 0x80000C40: addiu       $t9, $zero, 0x140
    ctx->r25 = ADD32(0, 0X140);
    // 0x80000C44: addiu       $t0, $zero, 0xF0
    ctx->r8 = ADD32(0, 0XF0);
    // 0x80000C48: addiu       $t1, $zero, 0xC
    ctx->r9 = ADD32(0, 0XC);
    // 0x80000C4C: addiu       $t2, $zero, 0x8
    ctx->r10 = ADD32(0, 0X8);
    // 0x80000C50: addiu       $t3, $zero, 0x134
    ctx->r11 = ADD32(0, 0X134);
    // 0x80000C54: addiu       $t4, $zero, 0xE8
    ctx->r12 = ADD32(0, 0XE8);
    // 0x80000C58: addiu       $a0, $a0, -0x7D34
    ctx->r4 = ADD32(ctx->r4, -0X7D34);
    // 0x80000C5C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80000C60: addiu       $v1, $v1, 0x5D78
    ctx->r3 = ADD32(ctx->r3, 0X5D78);
    // 0x80000C64: beq         $t8, $zero, L_80000DCC
    if (ctx->r24 == 0) {
        // 0x80000C68: swc1        $f10, 0xC($s0)
        MEM_W(0XC, ctx->r16) = ctx->f10.u32l;
            goto L_80000DCC;
    }
    // 0x80000C68: swc1        $f10, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f10.u32l;
    // 0x80000C6C: lui         $t0, 0x8064
    ctx->r8 = S32(0X8064 << 16);
    // 0x80000C70: lui         $t1, 0x806D
    ctx->r9 = S32(0X806D << 16);
    // 0x80000C74: lui         $t2, 0x8077
    ctx->r10 = S32(0X8077 << 16);
    // 0x80000C78: addiu       $t9, $zero, 0x3
    ctx->r25 = ADD32(0, 0X3);
    // 0x80000C7C: addiu       $t0, $t0, -0x2000
    ctx->r8 = ADD32(ctx->r8, -0X2000);
    // 0x80000C80: addiu       $t1, $t1, 0x4000
    ctx->r9 = ADD32(ctx->r9, 0X4000);
    // 0x80000C84: addiu       $t2, $t2, -0x6000
    ctx->r10 = ADD32(ctx->r10, -0X6000);
    // 0x80000C88: addiu       $t3, $zero, 0x280
    ctx->r11 = ADD32(0, 0X280);
    // 0x80000C8C: addiu       $t4, $zero, 0x1E0
    ctx->r12 = ADD32(0, 0X1E0);
    // 0x80000C90: addiu       $t6, $zero, 0x10
    ctx->r14 = ADD32(0, 0X10);
    // 0x80000C94: addiu       $t7, $zero, 0x1D8
    ctx->r15 = ADD32(0, 0X1D8);
    // 0x80000C98: addiu       $t8, $zero, 0x160
    ctx->r24 = ADD32(0, 0X160);
    // 0x80000C9C: lui         $v1, 0x800C
    ctx->r3 = S32(0X800C << 16);
    // 0x80000CA0: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80000CA4: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80000CA8: sh          $t9, 0x24($s0)
    MEM_H(0X24, ctx->r16) = ctx->r25;
    // 0x80000CAC: sw          $t0, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r8;
    // 0x80000CB0: sw          $t1, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r9;
    // 0x80000CB4: sw          $t2, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r10;
    // 0x80000CB8: sh          $t3, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r11;
    // 0x80000CBC: sh          $t4, 0x4E($s0)
    MEM_H(0X4E, ctx->r16) = ctx->r12;
    // 0x80000CC0: sh          $t5, 0x50($s0)
    MEM_H(0X50, ctx->r16) = ctx->r13;
    // 0x80000CC4: sh          $t6, 0x52($s0)
    MEM_H(0X52, ctx->r16) = ctx->r14;
    // 0x80000CC8: sh          $t7, 0x54($s0)
    MEM_H(0X54, ctx->r16) = ctx->r15;
    // 0x80000CCC: sh          $t8, 0x56($s0)
    MEM_H(0X56, ctx->r16) = ctx->r24;
    // 0x80000CD0: addiu       $a0, $a0, -0x7D34
    ctx->r4 = ADD32(ctx->r4, -0X7D34);
    // 0x80000CD4: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80000CD8: addiu       $v1, $v1, 0x5D78
    ctx->r3 = ADD32(ctx->r3, 0X5D78);
L_80000CDC:
    // 0x80000CDC: lw          $t9, 0x3C($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X3C);
    // 0x80000CE0: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80000CE4: addiu       $v1, $v1, 0x58
    ctx->r3 = ADD32(ctx->r3, 0X58);
    // 0x80000CE8: bne         $v0, $a0, L_80000CDC
    if (ctx->r2 != ctx->r4) {
        // 0x80000CEC: sw          $t9, -0x4C($v1)
        MEM_W(-0X4C, ctx->r3) = ctx->r25;
            goto L_80000CDC;
    }
    // 0x80000CEC: sw          $t9, -0x4C($v1)
    MEM_W(-0X4C, ctx->r3) = ctx->r25;
    // 0x80000CF0: lui         $v0, 0x8000
    ctx->r2 = S32(0X8000 << 16);
    // 0x80000CF4: lw          $v0, 0x300($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X300);
    // 0x80000CF8: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80000CFC: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000D00: beq         $v0, $zero, L_80000D74
    if (ctx->r2 == 0) {
        // 0x80000D04: nop
    
            goto L_80000D74;
    }
    // 0x80000D04: nop

    // 0x80000D08: beq         $v0, $at, L_80000D24
    if (ctx->r2 == ctx->r1) {
        // 0x80000D0C: lui         $a0, 0x800C
        ctx->r4 = S32(0X800C << 16);
            goto L_80000D24;
    }
    // 0x80000D0C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000D10: addiu       $a1, $zero, 0x2
    ctx->r5 = ADD32(0, 0X2);
    // 0x80000D14: beq         $v0, $a1, L_80000D64
    if (ctx->r2 == ctx->r5) {
        // 0x80000D18: lui         $a0, 0x800C
        ctx->r4 = S32(0X800C << 16);
            goto L_80000D64;
    }
    // 0x80000D18: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000D1C: b           L_80000DC4
    // 0x80000D20: nop

        goto L_80000DC4;
    // 0x80000D20: nop

L_80000D24:
    // 0x80000D24: jal         0x8009A5A0
    // 0x80000D28: addiu       $a0, $a0, -0x5910
    ctx->r4 = ADD32(ctx->r4, -0X5910);
    osViSetMode_recomp(rdram, ctx);
        goto after_0;
    // 0x80000D28: addiu       $a0, $a0, -0x5910
    ctx->r4 = ADD32(ctx->r4, -0X5910);
    after_0:
L_80000D2C:
    // 0x80000D2C: jal         0x800A38F0
    // 0x80000D30: addiu       $a0, $zero, 0x4A
    ctx->r4 = ADD32(0, 0X4A);
    osViSetSpecialFeatures_recomp(rdram, ctx);
        goto after_1;
    // 0x80000D30: addiu       $a0, $zero, 0x4A
    ctx->r4 = ADD32(0, 0X4A);
    after_1:
    // 0x80000D34: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80000D38: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80000D3C: jal         0x800A6F80
    // 0x80000D40: nop

    osViSetYScale_recomp(rdram, ctx);
        goto after_2;
    // 0x80000D40: nop

    after_2:
    // 0x80000D44: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80000D48: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80000D4C: jal         0x800A70B0
    // 0x80000D50: nop

    osViSetXScale_recomp(rdram, ctx);
        goto after_3;
    // 0x80000D50: nop

    after_3:
    // 0x80000D54: jal         0x80099B80
    // 0x80000D58: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    osViBlack_recomp(rdram, ctx);
        goto after_4;
    // 0x80000D58: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_4:
    // 0x80000D5C: b           L_80000EEC
    // 0x80000D60: lh          $t2, 0x4C($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X4C);
        goto L_80000EEC;
    // 0x80000D60: lh          $t2, 0x4C($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X4C);
L_80000D64:
    // 0x80000D64: jal         0x8009A5A0
    // 0x80000D68: addiu       $a0, $a0, -0x5050
    ctx->r4 = ADD32(ctx->r4, -0X5050);
    osViSetMode_recomp(rdram, ctx);
        goto after_5;
    // 0x80000D68: addiu       $a0, $a0, -0x5050
    ctx->r4 = ADD32(ctx->r4, -0X5050);
    after_5:
    // 0x80000D6C: b           L_80000D2C
    // 0x80000D70: nop

        goto L_80000D2C;
    // 0x80000D70: nop

L_80000D74:
    // 0x80000D74: jal         0x8009A5A0
    // 0x80000D78: addiu       $a0, $a0, -0x4BF0
    ctx->r4 = ADD32(ctx->r4, -0X4BF0);
    osViSetMode_recomp(rdram, ctx);
        goto after_6;
    // 0x80000D78: addiu       $a0, $a0, -0x4BF0
    ctx->r4 = ADD32(ctx->r4, -0X4BF0);
    after_6:
    // 0x80000D7C: addiu       $t0, $zero, 0x280
    ctx->r8 = ADD32(0, 0X280);
    // 0x80000D80: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000D84: sh          $t0, -0x7CF4($at)
    MEM_H(-0X7CF4, ctx->r1) = ctx->r8;
    // 0x80000D88: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000D8C: addiu       $t1, $zero, 0x1E0
    ctx->r9 = ADD32(0, 0X1E0);
    // 0x80000D90: sh          $t1, -0x7CF2($at)
    MEM_H(-0X7CF2, ctx->r1) = ctx->r9;
    // 0x80000D94: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000D98: addiu       $t2, $zero, 0x18
    ctx->r10 = ADD32(0, 0X18);
    // 0x80000D9C: sh          $t2, -0x7CF0($at)
    MEM_H(-0X7CF0, ctx->r1) = ctx->r10;
    // 0x80000DA0: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000DA4: addiu       $t3, $zero, 0x10
    ctx->r11 = ADD32(0, 0X10);
    // 0x80000DA8: sh          $t3, -0x7CEE($at)
    MEM_H(-0X7CEE, ctx->r1) = ctx->r11;
    // 0x80000DAC: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000DB0: addiu       $t4, $zero, 0x1D8
    ctx->r12 = ADD32(0, 0X1D8);
    // 0x80000DB4: sh          $t4, -0x7CEC($at)
    MEM_H(-0X7CEC, ctx->r1) = ctx->r12;
    // 0x80000DB8: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000DBC: addiu       $t5, $zero, 0x160
    ctx->r13 = ADD32(0, 0X160);
    // 0x80000DC0: sh          $t5, -0x7CEA($at)
    MEM_H(-0X7CEA, ctx->r1) = ctx->r13;
L_80000DC4:
    // 0x80000DC4: b           L_80000DC4
    pause_self(rdram);
    // 0x80000DC8: nop

L_80000DCC:
    // 0x80000DCC: lui         $t8, 0x8025
    ctx->r24 = S32(0X8025 << 16);
    // 0x80000DD0: addiu       $t8, $t8, -0x5000
    ctx->r24 = ADD32(ctx->r24, -0X5000);
    // 0x80000DD4: sh          $a1, 0x24($s0)
    MEM_H(0X24, ctx->r16) = ctx->r5;
    // 0x80000DD8: sw          $t6, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r14;
    // 0x80000DDC: sw          $t7, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r15;
    // 0x80000DE0: sw          $t8, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r24;
    // 0x80000DE4: sh          $t9, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r25;
    // 0x80000DE8: sh          $t0, 0x4E($s0)
    MEM_H(0X4E, ctx->r16) = ctx->r8;
    // 0x80000DEC: sh          $t1, 0x50($s0)
    MEM_H(0X50, ctx->r16) = ctx->r9;
    // 0x80000DF0: sh          $t2, 0x52($s0)
    MEM_H(0X52, ctx->r16) = ctx->r10;
    // 0x80000DF4: sh          $t3, 0x54($s0)
    MEM_H(0X54, ctx->r16) = ctx->r11;
    // 0x80000DF8: sh          $t4, 0x56($s0)
    MEM_H(0X56, ctx->r16) = ctx->r12;
L_80000DFC:
    // 0x80000DFC: lw          $t5, 0x3C($v0)
    ctx->r13 = MEM_W(ctx->r2, 0X3C);
    // 0x80000E00: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80000E04: addiu       $v1, $v1, 0x58
    ctx->r3 = ADD32(ctx->r3, 0X58);
    // 0x80000E08: bne         $v0, $a0, L_80000DFC
    if (ctx->r2 != ctx->r4) {
        // 0x80000E0C: sw          $t5, -0x4C($v1)
        MEM_W(-0X4C, ctx->r3) = ctx->r13;
            goto L_80000DFC;
    }
    // 0x80000E0C: sw          $t5, -0x4C($v1)
    MEM_W(-0X4C, ctx->r3) = ctx->r13;
    // 0x80000E10: lui         $v0, 0x8000
    ctx->r2 = S32(0X8000 << 16);
    // 0x80000E14: lw          $v0, 0x300($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X300);
    // 0x80000E18: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80000E1C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000E20: beq         $v0, $zero, L_80000E60
    if (ctx->r2 == 0) {
        // 0x80000E24: nop
    
            goto L_80000E60;
    }
    // 0x80000E24: nop

    // 0x80000E28: beq         $v0, $at, L_80000E40
    if (ctx->r2 == ctx->r1) {
        // 0x80000E2C: lui         $a0, 0x800C
        ctx->r4 = S32(0X800C << 16);
            goto L_80000E40;
    }
    // 0x80000E2C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000E30: beq         $v0, $a1, L_80000E50
    if (ctx->r2 == ctx->r5) {
        // 0x80000E34: lui         $a0, 0x800C
        ctx->r4 = S32(0X800C << 16);
            goto L_80000E50;
    }
    // 0x80000E34: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80000E38: b           L_80000EB0
    // 0x80000E3C: nop

        goto L_80000EB0;
    // 0x80000E3C: nop

L_80000E40:
    // 0x80000E40: jal         0x8009A5A0
    // 0x80000E44: addiu       $a0, $a0, -0x5C80
    ctx->r4 = ADD32(ctx->r4, -0X5C80);
    osViSetMode_recomp(rdram, ctx);
        goto after_7;
    // 0x80000E44: addiu       $a0, $a0, -0x5C80
    ctx->r4 = ADD32(ctx->r4, -0X5C80);
    after_7:
    // 0x80000E48: b           L_80000EB8
    // 0x80000E4C: nop

        goto L_80000EB8;
    // 0x80000E4C: nop

L_80000E50:
    // 0x80000E50: jal         0x8009A5A0
    // 0x80000E54: addiu       $a0, $a0, -0x53C0
    ctx->r4 = ADD32(ctx->r4, -0X53C0);
    osViSetMode_recomp(rdram, ctx);
        goto after_8;
    // 0x80000E54: addiu       $a0, $a0, -0x53C0
    ctx->r4 = ADD32(ctx->r4, -0X53C0);
    after_8:
    // 0x80000E58: b           L_80000EB8
    // 0x80000E5C: nop

        goto L_80000EB8;
    // 0x80000E5C: nop

L_80000E60:
    // 0x80000E60: jal         0x8009A5A0
    // 0x80000E64: addiu       $a0, $a0, -0x4F60
    ctx->r4 = ADD32(ctx->r4, -0X4F60);
    osViSetMode_recomp(rdram, ctx);
        goto after_9;
    // 0x80000E64: addiu       $a0, $a0, -0x4F60
    ctx->r4 = ADD32(ctx->r4, -0X4F60);
    after_9:
    // 0x80000E68: addiu       $t6, $zero, 0x140
    ctx->r14 = ADD32(0, 0X140);
    // 0x80000E6C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000E70: sh          $t6, -0x7CF4($at)
    MEM_H(-0X7CF4, ctx->r1) = ctx->r14;
    // 0x80000E74: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000E78: addiu       $t7, $zero, 0xF0
    ctx->r15 = ADD32(0, 0XF0);
    // 0x80000E7C: sh          $t7, -0x7CF2($at)
    MEM_H(-0X7CF2, ctx->r1) = ctx->r15;
    // 0x80000E80: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000E84: addiu       $t8, $zero, 0xC
    ctx->r24 = ADD32(0, 0XC);
    // 0x80000E88: sh          $t8, -0x7CF0($at)
    MEM_H(-0X7CF0, ctx->r1) = ctx->r24;
    // 0x80000E8C: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000E90: addiu       $t9, $zero, 0x8
    ctx->r25 = ADD32(0, 0X8);
    // 0x80000E94: sh          $t9, -0x7CEE($at)
    MEM_H(-0X7CEE, ctx->r1) = ctx->r25;
    // 0x80000E98: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000E9C: addiu       $t0, $zero, 0x134
    ctx->r8 = ADD32(0, 0X134);
    // 0x80000EA0: sh          $t0, -0x7CEC($at)
    MEM_H(-0X7CEC, ctx->r1) = ctx->r8;
    // 0x80000EA4: lui         $at, 0x801D
    ctx->r1 = S32(0X801D << 16);
    // 0x80000EA8: addiu       $t1, $zero, 0xE8
    ctx->r9 = ADD32(0, 0XE8);
    // 0x80000EAC: sh          $t1, -0x7CEA($at)
    MEM_H(-0X7CEA, ctx->r1) = ctx->r9;
L_80000EB0:
    // 0x80000EB0: b           L_80000EB0
    pause_self(rdram);
    // 0x80000EB4: nop

L_80000EB8:
    // 0x80000EB8: jal         0x800A38F0
    // 0x80000EBC: addiu       $a0, $zero, 0x4A
    ctx->r4 = ADD32(0, 0X4A);
    osViSetSpecialFeatures_recomp(rdram, ctx);
        goto after_10;
    // 0x80000EBC: addiu       $a0, $zero, 0x4A
    ctx->r4 = ADD32(0, 0X4A);
    after_10:
    // 0x80000EC0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80000EC4: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80000EC8: jal         0x800A6F80
    // 0x80000ECC: nop

    osViSetYScale_recomp(rdram, ctx);
        goto after_11;
    // 0x80000ECC: nop

    after_11:
    // 0x80000ED0: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80000ED4: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80000ED8: jal         0x800A70B0
    // 0x80000EDC: nop

    osViSetXScale_recomp(rdram, ctx);
        goto after_12;
    // 0x80000EDC: nop

    after_12:
    // 0x80000EE0: jal         0x80099B80
    // 0x80000EE4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    osViBlack_recomp(rdram, ctx);
        goto after_13;
    // 0x80000EE4: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_13:
    // 0x80000EE8: lh          $t2, 0x4C($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X4C);
L_80000EEC:
    // 0x80000EEC: lh          $t4, 0x4E($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X4E);
    // 0x80000EF0: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    // 0x80000EF4: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80000EF8: subu        $t3, $t3, $t2
    ctx->r11 = SUB32(ctx->r11, ctx->r10);
    // 0x80000EFC: sll         $t3, $t3, 1
    ctx->r11 = S32(ctx->r11 << 1);
    // 0x80000F00: multu       $t3, $t4
    result = U64(U32(ctx->r11)) * U64(U32(ctx->r12)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80000F04: mflo        $a1
    ctx->r5 = lo;
    // 0x80000F08: jal         0x80000F30
    // 0x80000F0C: nop

    bzero_recomp(rdram, ctx);
        goto after_14;
    // 0x80000F0C: nop

    after_14:
    // 0x80000F10: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80000F14: sh          $t5, 0x38($s0)
    MEM_H(0X38, ctx->r16) = ctx->r13;
    // 0x80000F18: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80000F1C:
    // 0x80000F1C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80000F20: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80000F24: jr          $ra
    // 0x80000F28: nop

    return;
    // 0x80000F28: nop

;}
RECOMP_FUNC void bzero_recomp(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80000F30: sltiu       $at, $a1, 0x4
    ctx->r1 = ctx->r5 < 0X4 ? 1 : 0;
    // 0x80000F34: beq         $at, $zero, L_80000F8C
    if (ctx->r1 == 0) {
        // 0x80000F38: addiu       $t6, $a0, 0x4
        ctx->r14 = ADD32(ctx->r4, 0X4);
            goto L_80000F8C;
    }
    // 0x80000F38: addiu       $t6, $a0, 0x4
    ctx->r14 = ADD32(ctx->r4, 0X4);
    // 0x80000F3C: beq         $a1, $zero, L_80001088
    if (ctx->r5 == 0) {
        // 0x80000F40: or          $v0, $a0, $zero
        ctx->r2 = ctx->r4 | 0;
            goto L_80001088;
    }
    // 0x80000F40: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80000F44: andi        $a0, $a1, 0x3
    ctx->r4 = ctx->r5 & 0X3;
    // 0x80000F48: negu        $a0, $a0
    ctx->r4 = SUB32(0, ctx->r4);
    // 0x80000F4C: beq         $a0, $zero, L_80000F68
    if (ctx->r4 == 0) {
        // 0x80000F50: addu        $v1, $a0, $a1
        ctx->r3 = ADD32(ctx->r4, ctx->r5);
            goto L_80000F68;
    }
    // 0x80000F50: addu        $v1, $a0, $a1
    ctx->r3 = ADD32(ctx->r4, ctx->r5);
L_80000F54:
    // 0x80000F54: addiu       $a1, $a1, -0x1
    ctx->r5 = ADD32(ctx->r5, -0X1);
    // 0x80000F58: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80000F5C: bne         $v1, $a1, L_80000F54
    if (ctx->r3 != ctx->r5) {
        // 0x80000F60: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80000F54;
    }
    // 0x80000F60: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80000F64: beq         $a1, $zero, L_80001088
    if (ctx->r5 == 0) {
        // 0x80000F68: addiu       $a1, $a1, -0x4
        ctx->r5 = ADD32(ctx->r5, -0X4);
            goto L_80001088;
    }
L_80000F68:
    // 0x80000F68: addiu       $a1, $a1, -0x4
    ctx->r5 = ADD32(ctx->r5, -0X4);
    // 0x80000F6C: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80000F70: sb          $zero, 0x1($v0)
    MEM_B(0X1, ctx->r2) = 0;
    // 0x80000F74: sb          $zero, 0x2($v0)
    MEM_B(0X2, ctx->r2) = 0;
    // 0x80000F78: sb          $zero, 0x3($v0)
    MEM_B(0X3, ctx->r2) = 0;
    // 0x80000F7C: bne         $a1, $zero, L_80000F68
    if (ctx->r5 != 0) {
        // 0x80000F80: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_80000F68;
    }
    // 0x80000F80: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80000F84: jr          $ra
    // 0x80000F88: nop

    return;
    // 0x80000F88: nop

L_80000F8C:
    // 0x80000F8C: addiu       $at, $zero, -0x4
    ctx->r1 = ADD32(0, -0X4);
    // 0x80000F90: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x80000F94: subu        $a2, $t7, $a0
    ctx->r6 = SUB32(ctx->r15, ctx->r4);
    // 0x80000F98: or          $v1, $a2, $zero
    ctx->r3 = ctx->r6 | 0;
    // 0x80000F9C: subu        $a1, $a1, $a2
    ctx->r5 = SUB32(ctx->r5, ctx->r6);
    // 0x80000FA0: beq         $a2, $zero, L_80000FE8
    if (ctx->r6 == 0) {
        // 0x80000FA4: or          $v0, $a0, $zero
        ctx->r2 = ctx->r4 | 0;
            goto L_80000FE8;
    }
    // 0x80000FA4: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80000FA8: andi        $a3, $a2, 0x3
    ctx->r7 = ctx->r6 & 0X3;
    // 0x80000FAC: negu        $a3, $a3
    ctx->r7 = SUB32(0, ctx->r7);
    // 0x80000FB0: beq         $a3, $zero, L_80000FCC
    if (ctx->r7 == 0) {
        // 0x80000FB4: addu        $a0, $a3, $a2
        ctx->r4 = ADD32(ctx->r7, ctx->r6);
            goto L_80000FCC;
    }
    // 0x80000FB4: addu        $a0, $a3, $a2
    ctx->r4 = ADD32(ctx->r7, ctx->r6);
L_80000FB8:
    // 0x80000FB8: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x80000FBC: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80000FC0: bne         $a0, $v1, L_80000FB8
    if (ctx->r4 != ctx->r3) {
        // 0x80000FC4: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80000FB8;
    }
    // 0x80000FC4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80000FC8: beq         $v1, $zero, L_80000FE8
    if (ctx->r3 == 0) {
        // 0x80000FCC: addiu       $v1, $v1, -0x4
        ctx->r3 = ADD32(ctx->r3, -0X4);
            goto L_80000FE8;
    }
L_80000FCC:
    // 0x80000FCC: addiu       $v1, $v1, -0x4
    ctx->r3 = ADD32(ctx->r3, -0X4);
    // 0x80000FD0: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80000FD4: sb          $zero, 0x1($v0)
    MEM_B(0X1, ctx->r2) = 0;
    // 0x80000FD8: sb          $zero, 0x2($v0)
    MEM_B(0X2, ctx->r2) = 0;
    // 0x80000FDC: sb          $zero, 0x3($v0)
    MEM_B(0X3, ctx->r2) = 0;
    // 0x80000FE0: bne         $v1, $zero, L_80000FCC
    if (ctx->r3 != 0) {
        // 0x80000FE4: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_80000FCC;
    }
    // 0x80000FE4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
L_80000FE8:
    // 0x80000FE8: srl         $a0, $a1, 2
    ctx->r4 = S32(U32(ctx->r5) >> 2);
    // 0x80000FEC: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80000FF0: beq         $a0, $zero, L_80001038
    if (ctx->r4 == 0) {
        // 0x80000FF4: or          $a2, $v0, $zero
        ctx->r6 = ctx->r2 | 0;
            goto L_80001038;
    }
    // 0x80000FF4: or          $a2, $v0, $zero
    ctx->r6 = ctx->r2 | 0;
    // 0x80000FF8: andi        $a3, $a0, 0x3
    ctx->r7 = ctx->r4 & 0X3;
    // 0x80000FFC: negu        $a3, $a3
    ctx->r7 = SUB32(0, ctx->r7);
    // 0x80001000: beq         $a3, $zero, L_8000101C
    if (ctx->r7 == 0) {
        // 0x80001004: addu        $v0, $a3, $a0
        ctx->r2 = ADD32(ctx->r7, ctx->r4);
            goto L_8000101C;
    }
    // 0x80001004: addu        $v0, $a3, $a0
    ctx->r2 = ADD32(ctx->r7, ctx->r4);
L_80001008:
    // 0x80001008: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x8000100C: sw          $zero, 0x0($a2)
    MEM_W(0X0, ctx->r6) = 0;
    // 0x80001010: bne         $v0, $v1, L_80001008
    if (ctx->r2 != ctx->r3) {
        // 0x80001014: addiu       $a2, $a2, 0x4
        ctx->r6 = ADD32(ctx->r6, 0X4);
            goto L_80001008;
    }
    // 0x80001014: addiu       $a2, $a2, 0x4
    ctx->r6 = ADD32(ctx->r6, 0X4);
    // 0x80001018: beq         $v1, $zero, L_80001038
    if (ctx->r3 == 0) {
        // 0x8000101C: addiu       $v1, $v1, -0x4
        ctx->r3 = ADD32(ctx->r3, -0X4);
            goto L_80001038;
    }
L_8000101C:
    // 0x8000101C: addiu       $v1, $v1, -0x4
    ctx->r3 = ADD32(ctx->r3, -0X4);
    // 0x80001020: sw          $zero, 0x0($a2)
    MEM_W(0X0, ctx->r6) = 0;
    // 0x80001024: sw          $zero, 0x4($a2)
    MEM_W(0X4, ctx->r6) = 0;
    // 0x80001028: sw          $zero, 0x8($a2)
    MEM_W(0X8, ctx->r6) = 0;
    // 0x8000102C: sw          $zero, 0xC($a2)
    MEM_W(0XC, ctx->r6) = 0;
    // 0x80001030: bne         $v1, $zero, L_8000101C
    if (ctx->r3 != 0) {
        // 0x80001034: addiu       $a2, $a2, 0x10
        ctx->r6 = ADD32(ctx->r6, 0X10);
            goto L_8000101C;
    }
    // 0x80001034: addiu       $a2, $a2, 0x10
    ctx->r6 = ADD32(ctx->r6, 0X10);
L_80001038:
    // 0x80001038: andi        $a0, $a1, 0x3
    ctx->r4 = ctx->r5 & 0X3;
    // 0x8000103C: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80001040: beq         $a0, $zero, L_80001088
    if (ctx->r4 == 0) {
        // 0x80001044: or          $v0, $a2, $zero
        ctx->r2 = ctx->r6 | 0;
            goto L_80001088;
    }
    // 0x80001044: or          $v0, $a2, $zero
    ctx->r2 = ctx->r6 | 0;
    // 0x80001048: andi        $a2, $a0, 0x3
    ctx->r6 = ctx->r4 & 0X3;
    // 0x8000104C: negu        $a2, $a2
    ctx->r6 = SUB32(0, ctx->r6);
    // 0x80001050: beq         $a2, $zero, L_8000106C
    if (ctx->r6 == 0) {
        // 0x80001054: addu        $a1, $a2, $a0
        ctx->r5 = ADD32(ctx->r6, ctx->r4);
            goto L_8000106C;
    }
    // 0x80001054: addu        $a1, $a2, $a0
    ctx->r5 = ADD32(ctx->r6, ctx->r4);
L_80001058:
    // 0x80001058: addiu       $v1, $v1, -0x1
    ctx->r3 = ADD32(ctx->r3, -0X1);
    // 0x8000105C: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80001060: bne         $a1, $v1, L_80001058
    if (ctx->r5 != ctx->r3) {
        // 0x80001064: addiu       $v0, $v0, 0x1
        ctx->r2 = ADD32(ctx->r2, 0X1);
            goto L_80001058;
    }
    // 0x80001064: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80001068: beq         $v1, $zero, L_80001088
    if (ctx->r3 == 0) {
        // 0x8000106C: addiu       $v1, $v1, -0x4
        ctx->r3 = ADD32(ctx->r3, -0X4);
            goto L_80001088;
    }
L_8000106C:
    // 0x8000106C: addiu       $v1, $v1, -0x4
    ctx->r3 = ADD32(ctx->r3, -0X4);
    // 0x80001070: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80001074: sb          $zero, 0x1($v0)
    MEM_B(0X1, ctx->r2) = 0;
    // 0x80001078: sb          $zero, 0x2($v0)
    MEM_B(0X2, ctx->r2) = 0;
    // 0x8000107C: sb          $zero, 0x3($v0)
    MEM_B(0X3, ctx->r2) = 0;
    // 0x80001080: bne         $v1, $zero, L_8000106C
    if (ctx->r3 != 0) {
        // 0x80001084: addiu       $v0, $v0, 0x4
        ctx->r2 = ADD32(ctx->r2, 0X4);
            goto L_8000106C;
    }
    // 0x80001084: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
L_80001088:
    // 0x80001088: jr          $ra
    // 0x8000108C: nop

    return;
    // 0x8000108C: nop

;}
RECOMP_FUNC void func_80001090(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001090: sltiu       $at, $a2, 0x4
    ctx->r1 = ctx->r6 < 0X4 ? 1 : 0;
    // 0x80001094: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80001098: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8000109C: beq         $at, $zero, L_8000111C
    if (ctx->r1 == 0) {
        // 0x800010A0: or          $a3, $a1, $zero
        ctx->r7 = ctx->r5 | 0;
            goto L_8000111C;
    }
    // 0x800010A0: or          $a3, $a1, $zero
    ctx->r7 = ctx->r5 | 0;
    // 0x800010A4: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x800010A8: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x800010AC: beq         $a2, $zero, L_800013E0
    if (ctx->r6 == 0) {
        // 0x800010B0: sw          $a3, 0x4($sp)
        MEM_W(0X4, ctx->r29) = ctx->r7;
            goto L_800013E0;
    }
    // 0x800010B0: sw          $a3, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r7;
    // 0x800010B4: andi        $a0, $a2, 0x3
    ctx->r4 = ctx->r6 & 0X3;
    // 0x800010B8: negu        $a0, $a0
    ctx->r4 = SUB32(0, ctx->r4);
    // 0x800010BC: beq         $a0, $zero, L_800010E4
    if (ctx->r4 == 0) {
        // 0x800010C0: addu        $v0, $a0, $a2
        ctx->r2 = ADD32(ctx->r4, ctx->r6);
            goto L_800010E4;
    }
    // 0x800010C0: addu        $v0, $a0, $a2
    ctx->r2 = ADD32(ctx->r4, ctx->r6);
L_800010C4:
    // 0x800010C4: lbu         $t6, 0x0($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X0);
    // 0x800010C8: addiu       $a2, $a2, -0x1
    ctx->r6 = ADD32(ctx->r6, -0X1);
    // 0x800010CC: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x800010D0: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x800010D4: bne         $v0, $a2, L_800010C4
    if (ctx->r2 != ctx->r6) {
        // 0x800010D8: sb          $t6, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r14;
            goto L_800010C4;
    }
    // 0x800010D8: sb          $t6, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r14;
    // 0x800010DC: beq         $a2, $zero, L_800013E0
    if (ctx->r6 == 0) {
        // 0x800010E0: nop
    
            goto L_800013E0;
    }
    // 0x800010E0: nop

L_800010E4:
    // 0x800010E4: lbu         $t7, 0x0($v1)
    ctx->r15 = MEM_BU(ctx->r3, 0X0);
    // 0x800010E8: addiu       $a2, $a2, -0x4
    ctx->r6 = ADD32(ctx->r6, -0X4);
    // 0x800010EC: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x800010F0: sb          $t7, -0x4($a1)
    MEM_B(-0X4, ctx->r5) = ctx->r15;
    // 0x800010F4: lbu         $t8, 0x1($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0X1);
    // 0x800010F8: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x800010FC: sb          $t8, -0x3($a1)
    MEM_B(-0X3, ctx->r5) = ctx->r24;
    // 0x80001100: lbu         $t9, -0x2($v1)
    ctx->r25 = MEM_BU(ctx->r3, -0X2);
    // 0x80001104: sb          $t9, -0x2($a1)
    MEM_B(-0X2, ctx->r5) = ctx->r25;
    // 0x80001108: lbu         $t1, -0x1($v1)
    ctx->r9 = MEM_BU(ctx->r3, -0X1);
    // 0x8000110C: bne         $a2, $zero, L_800010E4
    if (ctx->r6 != 0) {
        // 0x80001110: sb          $t1, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r9;
            goto L_800010E4;
    }
    // 0x80001110: sb          $t1, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r9;
    // 0x80001114: jr          $ra
    // 0x80001118: nop

    return;
    // 0x80001118: nop

L_8000111C:
    // 0x8000111C: andi        $t2, $a3, 0x3
    ctx->r10 = ctx->r7 & 0X3;
    // 0x80001120: andi        $t3, $a0, 0x3
    ctx->r11 = ctx->r4 & 0X3;
    // 0x80001124: bne         $t2, $t3, L_800012A8
    if (ctx->r10 != ctx->r11) {
        // 0x80001128: andi        $v0, $a0, 0x1
        ctx->r2 = ctx->r4 & 0X1;
            goto L_800012A8;
    }
    // 0x80001128: andi        $v0, $a0, 0x1
    ctx->r2 = ctx->r4 & 0X1;
    // 0x8000112C: addiu       $t4, $a0, 0x4
    ctx->r12 = ADD32(ctx->r4, 0X4);
    // 0x80001130: addiu       $at, $zero, -0x4
    ctx->r1 = ADD32(0, -0X4);
    // 0x80001134: and         $t5, $t4, $at
    ctx->r13 = ctx->r12 & ctx->r1;
    // 0x80001138: subu        $t0, $t5, $a0
    ctx->r8 = SUB32(ctx->r13, ctx->r4);
    // 0x8000113C: or          $v0, $t0, $zero
    ctx->r2 = ctx->r8 | 0;
    // 0x80001140: subu        $a2, $a2, $t0
    ctx->r6 = SUB32(ctx->r6, ctx->r8);
    // 0x80001144: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80001148: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x8000114C: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80001150: beq         $t0, $zero, L_800011B8
    if (ctx->r8 == 0) {
        // 0x80001154: sw          $a3, 0x4($sp)
        MEM_W(0X4, ctx->r29) = ctx->r7;
            goto L_800011B8;
    }
    // 0x80001154: sw          $a3, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r7;
    // 0x80001158: andi        $a3, $t0, 0x3
    ctx->r7 = ctx->r8 & 0X3;
    // 0x8000115C: negu        $a3, $a3
    ctx->r7 = SUB32(0, ctx->r7);
    // 0x80001160: beq         $a3, $zero, L_80001188
    if (ctx->r7 == 0) {
        // 0x80001164: addu        $a0, $a3, $t0
        ctx->r4 = ADD32(ctx->r7, ctx->r8);
            goto L_80001188;
    }
    // 0x80001164: addu        $a0, $a3, $t0
    ctx->r4 = ADD32(ctx->r7, ctx->r8);
L_80001168:
    // 0x80001168: lbu         $t6, 0x0($v1)
    ctx->r14 = MEM_BU(ctx->r3, 0X0);
    // 0x8000116C: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80001170: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x80001174: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80001178: bne         $a0, $v0, L_80001168
    if (ctx->r4 != ctx->r2) {
        // 0x8000117C: sb          $t6, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r14;
            goto L_80001168;
    }
    // 0x8000117C: sb          $t6, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r14;
    // 0x80001180: beql        $v0, $zero, L_800011BC
    if (ctx->r2 == 0) {
        // 0x80001184: srl         $a0, $a2, 2
        ctx->r4 = S32(U32(ctx->r6) >> 2);
            goto L_800011BC;
    }
    goto skip_0;
    // 0x80001184: srl         $a0, $a2, 2
    ctx->r4 = S32(U32(ctx->r6) >> 2);
    skip_0:
L_80001188:
    // 0x80001188: lbu         $t7, 0x0($v1)
    ctx->r15 = MEM_BU(ctx->r3, 0X0);
    // 0x8000118C: addiu       $v0, $v0, -0x4
    ctx->r2 = ADD32(ctx->r2, -0X4);
    // 0x80001190: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x80001194: sb          $t7, -0x4($a1)
    MEM_B(-0X4, ctx->r5) = ctx->r15;
    // 0x80001198: lbu         $t8, 0x1($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0X1);
    // 0x8000119C: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x800011A0: sb          $t8, -0x3($a1)
    MEM_B(-0X3, ctx->r5) = ctx->r24;
    // 0x800011A4: lbu         $t9, -0x2($v1)
    ctx->r25 = MEM_BU(ctx->r3, -0X2);
    // 0x800011A8: sb          $t9, -0x2($a1)
    MEM_B(-0X2, ctx->r5) = ctx->r25;
    // 0x800011AC: lbu         $t1, -0x1($v1)
    ctx->r9 = MEM_BU(ctx->r3, -0X1);
    // 0x800011B0: bne         $v0, $zero, L_80001188
    if (ctx->r2 != 0) {
        // 0x800011B4: sb          $t1, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r9;
            goto L_80001188;
    }
    // 0x800011B4: sb          $t1, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r9;
L_800011B8:
    // 0x800011B8: srl         $a0, $a2, 2
    ctx->r4 = S32(U32(ctx->r6) >> 2);
L_800011BC:
    // 0x800011BC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x800011C0: or          $a3, $v1, $zero
    ctx->r7 = ctx->r3 | 0;
    // 0x800011C4: beq         $a0, $zero, L_8000122C
    if (ctx->r4 == 0) {
        // 0x800011C8: or          $t0, $a1, $zero
        ctx->r8 = ctx->r5 | 0;
            goto L_8000122C;
    }
    // 0x800011C8: or          $t0, $a1, $zero
    ctx->r8 = ctx->r5 | 0;
    // 0x800011CC: andi        $a1, $a0, 0x3
    ctx->r5 = ctx->r4 & 0X3;
    // 0x800011D0: negu        $a1, $a1
    ctx->r5 = SUB32(0, ctx->r5);
    // 0x800011D4: beq         $a1, $zero, L_800011FC
    if (ctx->r5 == 0) {
        // 0x800011D8: addu        $v1, $a1, $a0
        ctx->r3 = ADD32(ctx->r5, ctx->r4);
            goto L_800011FC;
    }
    // 0x800011D8: addu        $v1, $a1, $a0
    ctx->r3 = ADD32(ctx->r5, ctx->r4);
L_800011DC:
    // 0x800011DC: lw          $t2, 0x0($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X0);
    // 0x800011E0: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x800011E4: addiu       $t0, $t0, 0x4
    ctx->r8 = ADD32(ctx->r8, 0X4);
    // 0x800011E8: addiu       $a3, $a3, 0x4
    ctx->r7 = ADD32(ctx->r7, 0X4);
    // 0x800011EC: bne         $v1, $v0, L_800011DC
    if (ctx->r3 != ctx->r2) {
        // 0x800011F0: sw          $t2, -0x4($t0)
        MEM_W(-0X4, ctx->r8) = ctx->r10;
            goto L_800011DC;
    }
    // 0x800011F0: sw          $t2, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->r10;
    // 0x800011F4: beql        $v0, $zero, L_80001230
    if (ctx->r2 == 0) {
        // 0x800011F8: andi        $a0, $a2, 0x3
        ctx->r4 = ctx->r6 & 0X3;
            goto L_80001230;
    }
    goto skip_1;
    // 0x800011F8: andi        $a0, $a2, 0x3
    ctx->r4 = ctx->r6 & 0X3;
    skip_1:
L_800011FC:
    // 0x800011FC: lw          $t3, 0x0($a3)
    ctx->r11 = MEM_W(ctx->r7, 0X0);
    // 0x80001200: addiu       $v0, $v0, -0x4
    ctx->r2 = ADD32(ctx->r2, -0X4);
    // 0x80001204: addiu       $t0, $t0, 0x10
    ctx->r8 = ADD32(ctx->r8, 0X10);
    // 0x80001208: sw          $t3, -0x10($t0)
    MEM_W(-0X10, ctx->r8) = ctx->r11;
    // 0x8000120C: lw          $t4, 0x4($a3)
    ctx->r12 = MEM_W(ctx->r7, 0X4);
    // 0x80001210: addiu       $a3, $a3, 0x10
    ctx->r7 = ADD32(ctx->r7, 0X10);
    // 0x80001214: sw          $t4, -0xC($t0)
    MEM_W(-0XC, ctx->r8) = ctx->r12;
    // 0x80001218: lw          $t5, -0x8($a3)
    ctx->r13 = MEM_W(ctx->r7, -0X8);
    // 0x8000121C: sw          $t5, -0x8($t0)
    MEM_W(-0X8, ctx->r8) = ctx->r13;
    // 0x80001220: lw          $t6, -0x4($a3)
    ctx->r14 = MEM_W(ctx->r7, -0X4);
    // 0x80001224: bne         $v0, $zero, L_800011FC
    if (ctx->r2 != 0) {
        // 0x80001228: sw          $t6, -0x4($t0)
        MEM_W(-0X4, ctx->r8) = ctx->r14;
            goto L_800011FC;
    }
    // 0x80001228: sw          $t6, -0x4($t0)
    MEM_W(-0X4, ctx->r8) = ctx->r14;
L_8000122C:
    // 0x8000122C: andi        $a0, $a2, 0x3
    ctx->r4 = ctx->r6 & 0X3;
L_80001230:
    // 0x80001230: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80001234: or          $v1, $a3, $zero
    ctx->r3 = ctx->r7 | 0;
    // 0x80001238: beq         $a0, $zero, L_800013E0
    if (ctx->r4 == 0) {
        // 0x8000123C: or          $a1, $t0, $zero
        ctx->r5 = ctx->r8 | 0;
            goto L_800013E0;
    }
    // 0x8000123C: or          $a1, $t0, $zero
    ctx->r5 = ctx->r8 | 0;
    // 0x80001240: andi        $a3, $a0, 0x3
    ctx->r7 = ctx->r4 & 0X3;
    // 0x80001244: negu        $a3, $a3
    ctx->r7 = SUB32(0, ctx->r7);
    // 0x80001248: beq         $a3, $zero, L_80001270
    if (ctx->r7 == 0) {
        // 0x8000124C: addu        $a2, $a3, $a0
        ctx->r6 = ADD32(ctx->r7, ctx->r4);
            goto L_80001270;
    }
    // 0x8000124C: addu        $a2, $a3, $a0
    ctx->r6 = ADD32(ctx->r7, ctx->r4);
L_80001250:
    // 0x80001250: lbu         $t7, 0x0($v1)
    ctx->r15 = MEM_BU(ctx->r3, 0X0);
    // 0x80001254: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80001258: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x8000125C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x80001260: bne         $a2, $v0, L_80001250
    if (ctx->r6 != ctx->r2) {
        // 0x80001264: sb          $t7, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r15;
            goto L_80001250;
    }
    // 0x80001264: sb          $t7, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r15;
    // 0x80001268: beq         $v0, $zero, L_800013E0
    if (ctx->r2 == 0) {
        // 0x8000126C: nop
    
            goto L_800013E0;
    }
    // 0x8000126C: nop

L_80001270:
    // 0x80001270: lbu         $t8, 0x0($v1)
    ctx->r24 = MEM_BU(ctx->r3, 0X0);
    // 0x80001274: addiu       $v0, $v0, -0x4
    ctx->r2 = ADD32(ctx->r2, -0X4);
    // 0x80001278: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x8000127C: sb          $t8, -0x4($a1)
    MEM_B(-0X4, ctx->r5) = ctx->r24;
    // 0x80001280: lbu         $t9, 0x1($v1)
    ctx->r25 = MEM_BU(ctx->r3, 0X1);
    // 0x80001284: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x80001288: sb          $t9, -0x3($a1)
    MEM_B(-0X3, ctx->r5) = ctx->r25;
    // 0x8000128C: lbu         $t1, -0x2($v1)
    ctx->r9 = MEM_BU(ctx->r3, -0X2);
    // 0x80001290: sb          $t1, -0x2($a1)
    MEM_B(-0X2, ctx->r5) = ctx->r9;
    // 0x80001294: lbu         $t2, -0x1($v1)
    ctx->r10 = MEM_BU(ctx->r3, -0X1);
    // 0x80001298: bne         $v0, $zero, L_80001270
    if (ctx->r2 != 0) {
        // 0x8000129C: sb          $t2, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r10;
            goto L_80001270;
    }
    // 0x8000129C: sb          $t2, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r10;
    // 0x800012A0: jr          $ra
    // 0x800012A4: nop

    return;
    // 0x800012A4: nop

L_800012A8:
    // 0x800012A8: andi        $t3, $a3, 0x1
    ctx->r11 = ctx->r7 & 0X1;
    // 0x800012AC: bne         $t3, $v0, L_80001370
    if (ctx->r11 != ctx->r2) {
        // 0x800012B0: or          $v1, $a0, $zero
        ctx->r3 = ctx->r4 | 0;
            goto L_80001370;
    }
    // 0x800012B0: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x800012B4: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x800012B8: beq         $v0, $zero, L_800012E4
    if (ctx->r2 == 0) {
        // 0x800012BC: sw          $a3, 0x4($sp)
        MEM_W(0X4, ctx->r29) = ctx->r7;
            goto L_800012E4;
    }
    // 0x800012BC: sw          $a3, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r7;
    // 0x800012C0: lbu         $t5, 0x0($a0)
    ctx->r13 = MEM_BU(ctx->r4, 0X0);
    // 0x800012C4: addiu       $a2, $a2, -0x1
    ctx->r6 = ADD32(ctx->r6, -0X1);
    // 0x800012C8: sb          $t5, 0x0($a3)
    MEM_B(0X0, ctx->r7) = ctx->r13;
    // 0x800012CC: lw          $t9, 0x0($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X0);
    // 0x800012D0: lw          $t7, 0x4($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X4);
    // 0x800012D4: addiu       $t1, $t9, 0x1
    ctx->r9 = ADD32(ctx->r25, 0X1);
    // 0x800012D8: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x800012DC: sw          $t8, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r24;
    // 0x800012E0: sw          $t1, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r9;
L_800012E4:
    // 0x800012E4: srl         $v1, $a2, 1
    ctx->r3 = S32(U32(ctx->r6) >> 1);
    // 0x800012E8: lw          $a1, 0x0($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X0);
    // 0x800012EC: lw          $t0, 0x4($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X4);
    // 0x800012F0: beq         $v1, $zero, L_80001358
    if (ctx->r3 == 0) {
        // 0x800012F4: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80001358;
    }
    // 0x800012F4: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x800012F8: andi        $a3, $v1, 0x3
    ctx->r7 = ctx->r3 & 0X3;
    // 0x800012FC: negu        $a3, $a3
    ctx->r7 = SUB32(0, ctx->r7);
    // 0x80001300: beq         $a3, $zero, L_80001328
    if (ctx->r7 == 0) {
        // 0x80001304: addu        $a0, $a3, $v1
        ctx->r4 = ADD32(ctx->r7, ctx->r3);
            goto L_80001328;
    }
    // 0x80001304: addu        $a0, $a3, $v1
    ctx->r4 = ADD32(ctx->r7, ctx->r3);
L_80001308:
    // 0x80001308: lhu         $t2, 0x0($a1)
    ctx->r10 = MEM_HU(ctx->r5, 0X0);
    // 0x8000130C: addiu       $v0, $v0, -0x1
    ctx->r2 = ADD32(ctx->r2, -0X1);
    // 0x80001310: addiu       $t0, $t0, 0x2
    ctx->r8 = ADD32(ctx->r8, 0X2);
    // 0x80001314: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x80001318: bne         $a0, $v0, L_80001308
    if (ctx->r4 != ctx->r2) {
        // 0x8000131C: sh          $t2, -0x2($t0)
        MEM_H(-0X2, ctx->r8) = ctx->r10;
            goto L_80001308;
    }
    // 0x8000131C: sh          $t2, -0x2($t0)
    MEM_H(-0X2, ctx->r8) = ctx->r10;
    // 0x80001320: beql        $v0, $zero, L_8000135C
    if (ctx->r2 == 0) {
        // 0x80001324: andi        $t7, $a2, 0x1
        ctx->r15 = ctx->r6 & 0X1;
            goto L_8000135C;
    }
    goto skip_2;
    // 0x80001324: andi        $t7, $a2, 0x1
    ctx->r15 = ctx->r6 & 0X1;
    skip_2:
L_80001328:
    // 0x80001328: lhu         $t3, 0x0($a1)
    ctx->r11 = MEM_HU(ctx->r5, 0X0);
    // 0x8000132C: addiu       $v0, $v0, -0x4
    ctx->r2 = ADD32(ctx->r2, -0X4);
    // 0x80001330: addiu       $t0, $t0, 0x8
    ctx->r8 = ADD32(ctx->r8, 0X8);
    // 0x80001334: sh          $t3, -0x8($t0)
    MEM_H(-0X8, ctx->r8) = ctx->r11;
    // 0x80001338: lhu         $t4, 0x2($a1)
    ctx->r12 = MEM_HU(ctx->r5, 0X2);
    // 0x8000133C: addiu       $a1, $a1, 0x8
    ctx->r5 = ADD32(ctx->r5, 0X8);
    // 0x80001340: sh          $t4, -0x6($t0)
    MEM_H(-0X6, ctx->r8) = ctx->r12;
    // 0x80001344: lhu         $t5, -0x4($a1)
    ctx->r13 = MEM_HU(ctx->r5, -0X4);
    // 0x80001348: sh          $t5, -0x4($t0)
    MEM_H(-0X4, ctx->r8) = ctx->r13;
    // 0x8000134C: lhu         $t6, -0x2($a1)
    ctx->r14 = MEM_HU(ctx->r5, -0X2);
    // 0x80001350: bne         $v0, $zero, L_80001328
    if (ctx->r2 != 0) {
        // 0x80001354: sh          $t6, -0x2($t0)
        MEM_H(-0X2, ctx->r8) = ctx->r14;
            goto L_80001328;
    }
    // 0x80001354: sh          $t6, -0x2($t0)
    MEM_H(-0X2, ctx->r8) = ctx->r14;
L_80001358:
    // 0x80001358: andi        $t7, $a2, 0x1
    ctx->r15 = ctx->r6 & 0X1;
L_8000135C:
    // 0x8000135C: beq         $t7, $zero, L_800013E0
    if (ctx->r15 == 0) {
        // 0x80001360: nop
    
            goto L_800013E0;
    }
    // 0x80001360: nop

    // 0x80001364: lbu         $t8, 0x0($a1)
    ctx->r24 = MEM_BU(ctx->r5, 0X0);
    // 0x80001368: jr          $ra
    // 0x8000136C: sb          $t8, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r24;
    return;
    // 0x8000136C: sb          $t8, 0x0($t0)
    MEM_B(0X0, ctx->r8) = ctx->r24;
L_80001370:
    // 0x80001370: or          $a1, $a3, $zero
    ctx->r5 = ctx->r7 | 0;
    // 0x80001374: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80001378: beq         $a2, $zero, L_800013E0
    if (ctx->r6 == 0) {
        // 0x8000137C: sw          $a3, 0x4($sp)
        MEM_W(0X4, ctx->r29) = ctx->r7;
            goto L_800013E0;
    }
    // 0x8000137C: sw          $a3, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r7;
    // 0x80001380: andi        $a0, $a2, 0x3
    ctx->r4 = ctx->r6 & 0X3;
    // 0x80001384: negu        $a0, $a0
    ctx->r4 = SUB32(0, ctx->r4);
    // 0x80001388: beq         $a0, $zero, L_800013B0
    if (ctx->r4 == 0) {
        // 0x8000138C: addu        $v0, $a0, $a2
        ctx->r2 = ADD32(ctx->r4, ctx->r6);
            goto L_800013B0;
    }
    // 0x8000138C: addu        $v0, $a0, $a2
    ctx->r2 = ADD32(ctx->r4, ctx->r6);
L_80001390:
    // 0x80001390: lbu         $t9, 0x0($v1)
    ctx->r25 = MEM_BU(ctx->r3, 0X0);
    // 0x80001394: addiu       $a2, $a2, -0x1
    ctx->r6 = ADD32(ctx->r6, -0X1);
    // 0x80001398: addiu       $a1, $a1, 0x1
    ctx->r5 = ADD32(ctx->r5, 0X1);
    // 0x8000139C: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x800013A0: bne         $v0, $a2, L_80001390
    if (ctx->r2 != ctx->r6) {
        // 0x800013A4: sb          $t9, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r25;
            goto L_80001390;
    }
    // 0x800013A4: sb          $t9, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r25;
    // 0x800013A8: beq         $a2, $zero, L_800013E0
    if (ctx->r6 == 0) {
        // 0x800013AC: nop
    
            goto L_800013E0;
    }
    // 0x800013AC: nop

L_800013B0:
    // 0x800013B0: lbu         $t1, 0x0($v1)
    ctx->r9 = MEM_BU(ctx->r3, 0X0);
    // 0x800013B4: addiu       $a2, $a2, -0x4
    ctx->r6 = ADD32(ctx->r6, -0X4);
    // 0x800013B8: addiu       $a1, $a1, 0x4
    ctx->r5 = ADD32(ctx->r5, 0X4);
    // 0x800013BC: sb          $t1, -0x4($a1)
    MEM_B(-0X4, ctx->r5) = ctx->r9;
    // 0x800013C0: lbu         $t2, 0x1($v1)
    ctx->r10 = MEM_BU(ctx->r3, 0X1);
    // 0x800013C4: addiu       $v1, $v1, 0x4
    ctx->r3 = ADD32(ctx->r3, 0X4);
    // 0x800013C8: sb          $t2, -0x3($a1)
    MEM_B(-0X3, ctx->r5) = ctx->r10;
    // 0x800013CC: lbu         $t3, -0x2($v1)
    ctx->r11 = MEM_BU(ctx->r3, -0X2);
    // 0x800013D0: sb          $t3, -0x2($a1)
    MEM_B(-0X2, ctx->r5) = ctx->r11;
    // 0x800013D4: lbu         $t4, -0x1($v1)
    ctx->r12 = MEM_BU(ctx->r3, -0X1);
    // 0x800013D8: bne         $a2, $zero, L_800013B0
    if (ctx->r6 != 0) {
        // 0x800013DC: sb          $t4, -0x1($a1)
        MEM_B(-0X1, ctx->r5) = ctx->r12;
            goto L_800013B0;
    }
    // 0x800013DC: sb          $t4, -0x1($a1)
    MEM_B(-0X1, ctx->r5) = ctx->r12;
L_800013E0:
    // 0x800013E0: jr          $ra
    // 0x800013E4: nop

    return;
    // 0x800013E4: nop

;}
RECOMP_FUNC void func_800013E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800013E8: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x800013EC: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x800013F0: lui         $t7, 0x801B
    ctx->r15 = S32(0X801B << 16);
    // 0x800013F4: addiu       $t7, $t7, 0x1D00
    ctx->r15 = ADD32(ctx->r15, 0X1D00);
    // 0x800013F8: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x800013FC: addiu       $v1, $zero, -0x8
    ctx->r3 = ADD32(0, -0X8);
    // 0x80001400: addu        $v0, $t6, $t7
    ctx->r2 = ADD32(ctx->r14, ctx->r15);
    // 0x80001404: addiu       $t9, $a2, 0x7
    ctx->r25 = ADD32(ctx->r6, 0X7);
    // 0x80001408: addiu       $t1, $a1, 0x7
    ctx->r9 = ADD32(ctx->r5, 0X7);
    // 0x8000140C: ori         $t8, $a3, 0x8000
    ctx->r24 = ctx->r7 | 0X8000;
    // 0x80001410: and         $t0, $t9, $v1
    ctx->r8 = ctx->r25 & ctx->r3;
    // 0x80001414: and         $t2, $t1, $v1
    ctx->r10 = ctx->r9 & ctx->r3;
    // 0x80001418: sh          $t8, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r24;
    // 0x8000141C: sw          $t0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r8;
    // 0x80001420: sw          $t2, 0x8($v0)
    MEM_W(0X8, ctx->r2) = ctx->r10;
    // 0x80001424: sh          $zero, 0x0($a1)
    MEM_H(0X0, ctx->r5) = 0;
    // 0x80001428: lw          $t3, 0x4($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X4);
    // 0x8000142C: addiu       $t4, $t3, -0x18
    ctx->r12 = ADD32(ctx->r11, -0X18);
    // 0x80001430: jr          $ra
    // 0x80001434: sw          $t4, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r12;
    return;
    // 0x80001434: sw          $t4, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r12;
;}
RECOMP_FUNC void func_80001438(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001438: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x8000143C: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x80001440: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80001444: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x80001448: addu        $at, $at, $t6
    ctx->r1 = ADD32(ctx->r1, ctx->r14);
    // 0x8000144C: jr          $ra
    // 0x80001450: sh          $zero, 0x1D00($at)
    MEM_H(0X1D00, ctx->r1) = 0;
    return;
    // 0x80001450: sh          $zero, 0x1D00($at)
    MEM_H(0X1D00, ctx->r1) = 0;
;}
RECOMP_FUNC void func_80001454(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001454: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80001458: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8000145C: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80001460: lui         $s0, 0x801B
    ctx->r16 = S32(0X801B << 16);
    // 0x80001464: lui         $s1, 0x801B
    ctx->r17 = S32(0X801B << 16);
    // 0x80001468: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8000146C: addiu       $s1, $s1, 0x1D00
    ctx->r17 = ADD32(ctx->r17, 0X1D00);
    // 0x80001470: addiu       $s0, $s0, 0x1D54
    ctx->r16 = ADD32(ctx->r16, 0X1D54);
    // 0x80001474: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
L_80001478:
    // 0x80001478: andi        $t7, $t6, 0x4000
    ctx->r15 = ctx->r14 & 0X4000;
    // 0x8000147C: beql        $t7, $zero, L_80001494
    if (ctx->r15 == 0) {
        // 0x80001480: addiu       $s0, $s0, -0xC
        ctx->r16 = ADD32(ctx->r16, -0XC);
            goto L_80001494;
    }
    goto skip_0;
    // 0x80001480: addiu       $s0, $s0, -0xC
    ctx->r16 = ADD32(ctx->r16, -0XC);
    skip_0:
    // 0x80001484: lw          $a0, 0x8($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X8);
    // 0x80001488: jal         0x80099440
    // 0x8000148C: lw          $a1, 0x4($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X4);
    osWritebackDCache_recomp(rdram, ctx);
        goto after_0;
    // 0x8000148C: lw          $a1, 0x4($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X4);
    after_0:
    // 0x80001490: addiu       $s0, $s0, -0xC
    ctx->r16 = ADD32(ctx->r16, -0XC);
L_80001494:
    // 0x80001494: sltu        $at, $s0, $s1
    ctx->r1 = ctx->r16 < ctx->r17 ? 1 : 0;
    // 0x80001498: beql        $at, $zero, L_80001478
    if (ctx->r1 == 0) {
        // 0x8000149C: lh          $t6, 0x0($s0)
        ctx->r14 = MEM_H(ctx->r16, 0X0);
            goto L_80001478;
    }
    goto skip_1;
    // 0x8000149C: lh          $t6, 0x0($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X0);
    skip_1:
    // 0x800014A0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800014A4: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x800014A8: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x800014AC: jr          $ra
    // 0x800014B0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x800014B0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_800014B4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800014B4: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x800014B8: sh          $zero, 0x1D54($at)
    MEM_H(0X1D54, ctx->r1) = 0;
    // 0x800014BC: addiu       $v0, $zero, 0x4
    ctx->r2 = ADD32(0, 0X4);
    // 0x800014C0: sll         $t6, $v0, 2
    ctx->r14 = S32(ctx->r2 << 2);
    // 0x800014C4: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x800014C8: sh          $zero, 0x1D48($at)
    MEM_H(0X1D48, ctx->r1) = 0;
    // 0x800014CC: subu        $t6, $t6, $v0
    ctx->r14 = SUB32(ctx->r14, ctx->r2);
    // 0x800014D0: lui         $t7, 0x801B
    ctx->r15 = S32(0X801B << 16);
    // 0x800014D4: addiu       $t7, $t7, 0x1D00
    ctx->r15 = ADD32(ctx->r15, 0X1D00);
    // 0x800014D8: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x800014DC: lui         $at, 0x801B
    ctx->r1 = S32(0X801B << 16);
    // 0x800014E0: sh          $zero, 0x1D3C($at)
    MEM_H(0X1D3C, ctx->r1) = 0;
    // 0x800014E4: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x800014E8: sh          $zero, 0x0($v1)
    MEM_H(0X0, ctx->r3) = 0;
    // 0x800014EC: sh          $zero, -0xC($v1)
    MEM_H(-0XC, ctx->r3) = 0;
    // 0x800014F0: sh          $zero, -0x18($v1)
    MEM_H(-0X18, ctx->r3) = 0;
    // 0x800014F4: jr          $ra
    // 0x800014F8: sh          $zero, -0x24($v1)
    MEM_H(-0X24, ctx->r3) = 0;
    return;
    // 0x800014F8: sh          $zero, -0x24($v1)
    MEM_H(-0X24, ctx->r3) = 0;
;}
RECOMP_FUNC void func_800014FC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800014FC: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80001500: jr          $ra
    // 0x80001504: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    return;
    // 0x80001504: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
;}
RECOMP_FUNC void func_80001508(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001508: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8000150C: jr          $ra
    // 0x80001510: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80001510: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
;}
RECOMP_FUNC void func_80001514(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001514: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x80001518: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x8000151C: lui         $t7, 0x801B
    ctx->r15 = S32(0X801B << 16);
    // 0x80001520: addiu       $t7, $t7, 0x1D00
    ctx->r15 = ADD32(ctx->r15, 0X1D00);
    // 0x80001524: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80001528: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x8000152C: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x80001530: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80001534: lw          $s0, 0x8($v1)
    ctx->r16 = MEM_W(ctx->r3, 0X8);
    // 0x80001538: lw          $t8, 0x4($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X4);
    // 0x8000153C: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80001540: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80001544: addiu       $s2, $a1, 0x7
    ctx->r18 = ADD32(ctx->r5, 0X7);
    // 0x80001548: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8000154C: srl         $s2, $s2, 3
    ctx->r18 = S32(U32(ctx->r18) >> 3);
    // 0x80001550: addu        $s1, $t8, $s0
    ctx->r17 = ADD32(ctx->r24, ctx->r16);
    // 0x80001554: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80001558: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8000155C: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80001560: beq         $s1, $s0, L_80001618
    if (ctx->r17 == ctx->r16) {
        // 0x80001564: sll         $s2, $s2, 3
        ctx->r18 = S32(ctx->r18 << 3);
            goto L_80001618;
    }
    // 0x80001564: sll         $s2, $s2, 3
    ctx->r18 = S32(ctx->r18 << 3);
    // 0x80001568: lui         $s4, 0xC000
    ctx->r20 = S32(0XC000 << 16);
    // 0x8000156C: sltu        $at, $s1, $s0
    ctx->r1 = ctx->r17 < ctx->r16 ? 1 : 0;
L_80001570:
    // 0x80001570: beq         $at, $zero, L_80001580
    if (ctx->r1 == 0) {
        // 0x80001574: or          $a0, $s3, $zero
        ctx->r4 = ctx->r19 | 0;
            goto L_80001580;
    }
    // 0x80001574: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80001578: jal         0x800014FC
    // 0x8000157C: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    func_800014FC(rdram, ctx);
        goto after_0;
    // 0x8000157C: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    after_0:
L_80001580:
    // 0x80001580: lh          $t9, 0x0($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X0);
    // 0x80001584: bnel        $t9, $zero, L_80001608
    if (ctx->r25 != 0) {
        // 0x80001588: lw          $t5, 0x4($s0)
        ctx->r13 = MEM_W(ctx->r16, 0X4);
            goto L_80001608;
    }
    goto skip_0;
    // 0x80001588: lw          $t5, 0x4($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X4);
    skip_0:
L_8000158C:
    // 0x8000158C: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x80001590: sltu        $at, $v0, $s2
    ctx->r1 = ctx->r2 < ctx->r18 ? 1 : 0;
    // 0x80001594: beq         $at, $zero, L_800015D0
    if (ctx->r1 == 0) {
        // 0x80001598: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_800015D0;
    }
    // 0x80001598: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8000159C: addu        $v1, $s0, $v0
    ctx->r3 = ADD32(ctx->r16, ctx->r2);
    // 0x800015A0: addiu       $v1, $v1, 0x18
    ctx->r3 = ADD32(ctx->r3, 0X18);
    // 0x800015A4: sltu        $at, $v1, $s1
    ctx->r1 = ctx->r3 < ctx->r17 ? 1 : 0;
    // 0x800015A8: beq         $at, $zero, L_800015D4
    if (ctx->r1 == 0) {
        // 0x800015AC: nop
    
            goto L_800015D4;
    }
    // 0x800015AC: nop

    // 0x800015B0: lh          $t0, 0x0($v1)
    ctx->r8 = MEM_H(ctx->r3, 0X0);
    // 0x800015B4: bne         $t0, $zero, L_800015D4
    if (ctx->r8 != 0) {
        // 0x800015B8: nop
    
            goto L_800015D4;
    }
    // 0x800015B8: nop

    // 0x800015BC: lw          $t1, 0x4($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X4);
    // 0x800015C0: addu        $t2, $v0, $t1
    ctx->r10 = ADD32(ctx->r2, ctx->r9);
    // 0x800015C4: addiu       $t3, $t2, 0x18
    ctx->r11 = ADD32(ctx->r10, 0X18);
    // 0x800015C8: b           L_8000158C
    // 0x800015CC: sw          $t3, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r11;
        goto L_8000158C;
    // 0x800015CC: sw          $t3, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r11;
L_800015D0:
    // 0x800015D0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_800015D4:
    // 0x800015D4: bne         $v1, $zero, L_80001604
    if (ctx->r3 != 0) {
        // 0x800015D8: subu        $v1, $a0, $s2
        ctx->r3 = SUB32(ctx->r4, ctx->r18);
            goto L_80001604;
    }
    // 0x800015D8: subu        $v1, $a0, $s2
    ctx->r3 = SUB32(ctx->r4, ctx->r18);
    // 0x800015DC: addiu       $v1, $v1, -0x18
    ctx->r3 = ADD32(ctx->r3, -0X18);
    // 0x800015E0: blez        $v1, L_800015F8
    if (SIGNED(ctx->r3) <= 0) {
        // 0x800015E4: addiu       $t4, $zero, -0x8000
        ctx->r12 = ADD32(0, -0X8000);
            goto L_800015F8;
    }
    // 0x800015E4: addiu       $t4, $zero, -0x8000
    ctx->r12 = ADD32(0, -0X8000);
    // 0x800015E8: addu        $v0, $s0, $s2
    ctx->r2 = ADD32(ctx->r16, ctx->r18);
    // 0x800015EC: sh          $zero, 0x18($v0)
    MEM_H(0X18, ctx->r2) = 0;
    // 0x800015F0: sw          $v1, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->r3;
    // 0x800015F4: sw          $s2, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r18;
L_800015F8:
    // 0x800015F8: sh          $t4, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r12;
    // 0x800015FC: b           L_8000161C
    // 0x80001600: addiu       $v0, $s0, 0x18
    ctx->r2 = ADD32(ctx->r16, 0X18);
        goto L_8000161C;
    // 0x80001600: addiu       $v0, $s0, 0x18
    ctx->r2 = ADD32(ctx->r16, 0X18);
L_80001604:
    // 0x80001604: lw          $t5, 0x4($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X4);
L_80001608:
    // 0x80001608: addu        $s0, $s0, $t5
    ctx->r16 = ADD32(ctx->r16, ctx->r13);
    // 0x8000160C: addiu       $s0, $s0, 0x18
    ctx->r16 = ADD32(ctx->r16, 0X18);
    // 0x80001610: bnel        $s0, $s1, L_80001570
    if (ctx->r16 != ctx->r17) {
        // 0x80001614: sltu        $at, $s1, $s0
        ctx->r1 = ctx->r17 < ctx->r16 ? 1 : 0;
            goto L_80001570;
    }
    goto skip_1;
    // 0x80001614: sltu        $at, $s1, $s0
    ctx->r1 = ctx->r17 < ctx->r16 ? 1 : 0;
    skip_1:
L_80001618:
    // 0x80001618: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8000161C:
    // 0x8000161C: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x80001620: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80001624: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80001628: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8000162C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80001630: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80001634: jr          $ra
    // 0x80001638: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    return;
    // 0x80001638: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
;}
RECOMP_FUNC void func_8000163C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8000163C: sll         $t6, $a0, 2
    ctx->r14 = S32(ctx->r4 << 2);
    // 0x80001640: subu        $t6, $t6, $a0
    ctx->r14 = SUB32(ctx->r14, ctx->r4);
    // 0x80001644: lui         $t7, 0x801B
    ctx->r15 = S32(0X801B << 16);
    // 0x80001648: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8000164C: addiu       $t7, $t7, 0x1D00
    ctx->r15 = ADD32(ctx->r15, 0X1D00);
    // 0x80001650: sll         $t6, $t6, 2
    ctx->r14 = S32(ctx->r14 << 2);
    // 0x80001654: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x80001658: lw          $v0, 0x8($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X8);
    // 0x8000165C: lw          $t8, 0x4($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X4);
    // 0x80001660: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80001664: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80001668: addiu       $s2, $a1, 0x7
    ctx->r18 = ADD32(ctx->r5, 0X7);
    // 0x8000166C: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80001670: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80001674: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80001678: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8000167C: srl         $s2, $s2, 3
    ctx->r18 = S32(U32(ctx->r18) >> 3);
    // 0x80001680: addu        $s1, $t8, $v0
    ctx->r17 = ADD32(ctx->r24, ctx->r2);
    // 0x80001684: or          $s4, $a2, $zero
    ctx->r20 = ctx->r6 | 0;
    // 0x80001688: or          $s5, $a0, $zero
    ctx->r21 = ctx->r4 | 0;
    // 0x8000168C: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80001690: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80001694: sll         $s2, $s2, 3
    ctx->r18 = S32(ctx->r18 << 3);
    // 0x80001698: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x8000169C: beq         $s1, $v0, L_800017D8
    if (ctx->r17 == ctx->r2) {
        // 0x800016A0: or          $s3, $v0, $zero
        ctx->r19 = ctx->r2 | 0;
            goto L_800017D8;
    }
    // 0x800016A0: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x800016A4: lui         $s6, 0xC000
    ctx->r22 = S32(0XC000 << 16);
    // 0x800016A8: sltu        $at, $s1, $s0
    ctx->r1 = ctx->r17 < ctx->r16 ? 1 : 0;
L_800016AC:
    // 0x800016AC: beql        $at, $zero, L_800016C4
    if (ctx->r1 == 0) {
        // 0x800016B0: lh          $t9, 0x0($s0)
        ctx->r25 = MEM_H(ctx->r16, 0X0);
            goto L_800016C4;
    }
    goto skip_0;
    // 0x800016B0: lh          $t9, 0x0($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X0);
    skip_0:
    // 0x800016B4: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x800016B8: jal         0x800014FC
    // 0x800016BC: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    func_800014FC(rdram, ctx);
        goto after_0;
    // 0x800016BC: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    after_0:
    // 0x800016C0: lh          $t9, 0x0($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X0);
L_800016C4:
    // 0x800016C4: addiu       $t0, $s0, 0x18
    ctx->r8 = ADD32(ctx->r16, 0X18);
    // 0x800016C8: bnel        $t9, $zero, L_800017C4
    if (ctx->r25 != 0) {
        // 0x800016CC: lw          $t4, 0x4($s0)
        ctx->r12 = MEM_W(ctx->r16, 0X4);
            goto L_800017C4;
    }
    goto skip_1;
    // 0x800016CC: lw          $t4, 0x4($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X4);
    skip_1:
    // 0x800016D0: divu        $zero, $t0, $s4
    lo = S32(U32(ctx->r8) / U32(ctx->r20)); hi = S32(U32(ctx->r8) % U32(ctx->r20));
    // 0x800016D4: mfhi        $t1
    ctx->r9 = hi;
    // 0x800016D8: subu        $a0, $s4, $t1
    ctx->r4 = SUB32(ctx->r20, ctx->r9);
    // 0x800016DC: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x800016E0: bne         $s4, $zero, L_800016EC
    if (ctx->r20 != 0) {
        // 0x800016E4: nop
    
            goto L_800016EC;
    }
    // 0x800016E4: nop

    // 0x800016E8: break       7
    do_break(2147489512);
L_800016EC:
    // 0x800016EC: lw          $v0, 0x4($s0)
    ctx->r2 = MEM_W(ctx->r16, 0X4);
    // 0x800016F0: subu        $t2, $v0, $a0
    ctx->r10 = SUB32(ctx->r2, ctx->r4);
    // 0x800016F4: slt         $at, $t2, $s2
    ctx->r1 = SIGNED(ctx->r10) < SIGNED(ctx->r18) ? 1 : 0;
    // 0x800016F8: bne         $at, $zero, L_80001754
    if (ctx->r1 != 0) {
        // 0x800016FC: addu        $v1, $s0, $v0
        ctx->r3 = ADD32(ctx->r16, ctx->r2);
            goto L_80001754;
    }
    // 0x800016FC: addu        $v1, $s0, $v0
    ctx->r3 = ADD32(ctx->r16, ctx->r2);
    // 0x80001700: addiu       $a1, $a0, -0x18
    ctx->r5 = ADD32(ctx->r4, -0X18);
    // 0x80001704: addu        $v1, $s0, $a1
    ctx->r3 = ADD32(ctx->r16, ctx->r5);
    // 0x80001708: blez        $a1, L_8000172C
    if (SIGNED(ctx->r5) <= 0) {
        // 0x8000170C: addiu       $v1, $v1, 0x18
        ctx->r3 = ADD32(ctx->r3, 0X18);
            goto L_8000172C;
    }
    // 0x8000170C: addiu       $v1, $v1, 0x18
    ctx->r3 = ADD32(ctx->r3, 0X18);
    // 0x80001710: subu        $t3, $v0, $a2
    ctx->r11 = SUB32(ctx->r2, ctx->r6);
    // 0x80001714: sw          $t3, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r11;
    // 0x80001718: addiu       $t4, $a2, -0x18
    ctx->r12 = ADD32(ctx->r6, -0X18);
    // 0x8000171C: sw          $t4, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r12;
    // 0x80001720: or          $s0, $v1, $zero
    ctx->r16 = ctx->r3 | 0;
    // 0x80001724: b           L_80001784
    // 0x80001728: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
        goto L_80001784;
    // 0x80001728: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_8000172C:
    // 0x8000172C: beq         $s3, $s0, L_8000174C
    if (ctx->r19 == ctx->r16) {
        // 0x80001730: nop
    
            goto L_8000174C;
    }
    // 0x80001730: nop

    // 0x80001734: subu        $t5, $v0, $a2
    ctx->r13 = SUB32(ctx->r2, ctx->r6);
    // 0x80001738: sw          $t5, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r13;
    // 0x8000173C: lw          $t6, 0x4($s3)
    ctx->r14 = MEM_W(ctx->r19, 0X4);
    // 0x80001740: or          $s0, $v1, $zero
    ctx->r16 = ctx->r3 | 0;
    // 0x80001744: addu        $t7, $t6, $a2
    ctx->r15 = ADD32(ctx->r14, ctx->r6);
    // 0x80001748: sw          $t7, 0x4($s3)
    MEM_W(0X4, ctx->r19) = ctx->r15;
L_8000174C:
    // 0x8000174C: b           L_80001784
    // 0x80001750: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
        goto L_80001784;
    // 0x80001750: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
L_80001754:
    // 0x80001754: addiu       $v1, $v1, 0x18
    ctx->r3 = ADD32(ctx->r3, 0X18);
    // 0x80001758: sltu        $at, $v1, $s1
    ctx->r1 = ctx->r3 < ctx->r17 ? 1 : 0;
    // 0x8000175C: beq         $at, $zero, L_80001784
    if (ctx->r1 == 0) {
        // 0x80001760: nop
    
            goto L_80001784;
    }
    // 0x80001760: nop

    // 0x80001764: lh          $t8, 0x0($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X0);
    // 0x80001768: bne         $t8, $zero, L_80001784
    if (ctx->r24 != 0) {
        // 0x8000176C: nop
    
            goto L_80001784;
    }
    // 0x8000176C: nop

    // 0x80001770: lw          $t9, 0x4($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X4);
    // 0x80001774: addu        $t0, $v0, $t9
    ctx->r8 = ADD32(ctx->r2, ctx->r25);
    // 0x80001778: addiu       $t1, $t0, 0x18
    ctx->r9 = ADD32(ctx->r8, 0X18);
    // 0x8000177C: b           L_800016EC
    // 0x80001780: sw          $t1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r9;
        goto L_800016EC;
    // 0x80001780: sw          $t1, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r9;
L_80001784:
    // 0x80001784: bnel        $v1, $zero, L_800017C4
    if (ctx->r3 != 0) {
        // 0x80001788: lw          $t4, 0x4($s0)
        ctx->r12 = MEM_W(ctx->r16, 0X4);
            goto L_800017C4;
    }
    goto skip_2;
    // 0x80001788: lw          $t4, 0x4($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X4);
    skip_2:
    // 0x8000178C: lw          $t2, 0x4($s0)
    ctx->r10 = MEM_W(ctx->r16, 0X4);
    // 0x80001790: addiu       $a0, $s0, 0x18
    ctx->r4 = ADD32(ctx->r16, 0X18);
    // 0x80001794: addu        $v0, $s0, $s2
    ctx->r2 = ADD32(ctx->r16, ctx->r18);
    // 0x80001798: subu        $v1, $t2, $s2
    ctx->r3 = SUB32(ctx->r10, ctx->r18);
    // 0x8000179C: addiu       $v1, $v1, -0x18
    ctx->r3 = ADD32(ctx->r3, -0X18);
    // 0x800017A0: blez        $v1, L_800017B4
    if (SIGNED(ctx->r3) <= 0) {
        // 0x800017A4: addiu       $t3, $zero, -0x8000
        ctx->r11 = ADD32(0, -0X8000);
            goto L_800017B4;
    }
    // 0x800017A4: addiu       $t3, $zero, -0x8000
    ctx->r11 = ADD32(0, -0X8000);
    // 0x800017A8: sh          $zero, 0x18($v0)
    MEM_H(0X18, ctx->r2) = 0;
    // 0x800017AC: sw          $v1, 0x1C($v0)
    MEM_W(0X1C, ctx->r2) = ctx->r3;
    // 0x800017B0: sw          $s2, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->r18;
L_800017B4:
    // 0x800017B4: sh          $t3, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r11;
    // 0x800017B8: b           L_800017DC
    // 0x800017BC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
        goto L_800017DC;
    // 0x800017BC: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x800017C0: lw          $t4, 0x4($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X4);
L_800017C4:
    // 0x800017C4: or          $s3, $s0, $zero
    ctx->r19 = ctx->r16 | 0;
    // 0x800017C8: addu        $s0, $s0, $t4
    ctx->r16 = ADD32(ctx->r16, ctx->r12);
    // 0x800017CC: addiu       $s0, $s0, 0x18
    ctx->r16 = ADD32(ctx->r16, 0X18);
    // 0x800017D0: bnel        $s0, $s1, L_800016AC
    if (ctx->r16 != ctx->r17) {
        // 0x800017D4: sltu        $at, $s1, $s0
        ctx->r1 = ctx->r17 < ctx->r16 ? 1 : 0;
            goto L_800016AC;
    }
    goto skip_3;
    // 0x800017D4: sltu        $at, $s1, $s0
    ctx->r1 = ctx->r17 < ctx->r16 ? 1 : 0;
    skip_3:
L_800017D8:
    // 0x800017D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800017DC:
    // 0x800017DC: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x800017E0: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800017E4: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x800017E8: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x800017EC: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x800017F0: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x800017F4: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x800017F8: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x800017FC: jr          $ra
    // 0x80001800: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x80001800: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_80001804(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001804: addiu       $v0, $a0, -0x18
    ctx->r2 = ADD32(ctx->r4, -0X18);
    // 0x80001808: lw          $v1, 0x4($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X4);
    // 0x8000180C: addiu       $a1, $a1, 0x7
    ctx->r5 = ADD32(ctx->r5, 0X7);
    // 0x80001810: srl         $a1, $a1, 3
    ctx->r5 = S32(U32(ctx->r5) >> 3);
    // 0x80001814: sll         $a1, $a1, 3
    ctx->r5 = S32(ctx->r5 << 3);
    // 0x80001818: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x8000181C: beq         $at, $zero, L_8000182C
    if (ctx->r1 == 0) {
        // 0x80001820: or          $a2, $v1, $zero
        ctx->r6 = ctx->r3 | 0;
            goto L_8000182C;
    }
    // 0x80001820: or          $a2, $v1, $zero
    ctx->r6 = ctx->r3 | 0;
    // 0x80001824: jr          $ra
    // 0x80001828: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    return;
    // 0x80001828: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_8000182C:
    // 0x8000182C: subu        $a0, $a2, $a1
    ctx->r4 = SUB32(ctx->r6, ctx->r5);
    // 0x80001830: addiu       $a0, $a0, -0x18
    ctx->r4 = ADD32(ctx->r4, -0X18);
    // 0x80001834: blez        $a0, L_8000184C
    if (SIGNED(ctx->r4) <= 0) {
        // 0x80001838: addu        $a2, $v0, $a1
        ctx->r6 = ADD32(ctx->r2, ctx->r5);
            goto L_8000184C;
    }
    // 0x80001838: addu        $a2, $v0, $a1
    ctx->r6 = ADD32(ctx->r2, ctx->r5);
    // 0x8000183C: sh          $zero, 0x18($a2)
    MEM_H(0X18, ctx->r6) = 0;
    // 0x80001840: sw          $a0, 0x1C($a2)
    MEM_W(0X1C, ctx->r6) = ctx->r4;
    // 0x80001844: sw          $a1, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r5;
    // 0x80001848: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
L_8000184C:
    // 0x8000184C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80001850: jr          $ra
    // 0x80001854: nop

    return;
    // 0x80001854: nop

;}
RECOMP_FUNC void func_80001858(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001858: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x8000185C: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80001860: lh          $t6, 0x24($t0)
    ctx->r14 = MEM_H(ctx->r8, 0X24);
    // 0x80001864: addiu       $a2, $a1, 0x7
    ctx->r6 = ADD32(ctx->r5, 0X7);
    // 0x80001868: srl         $a2, $a2, 3
    ctx->r6 = S32(U32(ctx->r6) >> 3);
    // 0x8000186C: sll         $a2, $a2, 3
    ctx->r6 = S32(ctx->r6 << 3);
    // 0x80001870: multu       $t6, $a2
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r6)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80001874: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80001878: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8000187C: sw          $a2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r6;
    // 0x80001880: mflo        $a1
    ctx->r5 = lo;
    // 0x80001884: jal         0x80001514
    // 0x80001888: nop

    func_80001514(rdram, ctx);
        goto after_0;
    // 0x80001888: nop

    after_0:
    // 0x8000188C: lui         $t0, 0x801D
    ctx->r8 = S32(0X801D << 16);
    // 0x80001890: addiu       $t0, $t0, -0x7D40
    ctx->r8 = ADD32(ctx->r8, -0X7D40);
    // 0x80001894: lw          $a2, 0x1C($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X1C);
    // 0x80001898: bne         $v0, $zero, L_800018A8
    if (ctx->r2 != 0) {
        // 0x8000189C: or          $t1, $v0, $zero
        ctx->r9 = ctx->r2 | 0;
            goto L_800018A8;
    }
    // 0x8000189C: or          $t1, $v0, $zero
    ctx->r9 = ctx->r2 | 0;
    // 0x800018A0: b           L_80001908
    // 0x800018A4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80001908;
    // 0x800018A4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800018A8:
    // 0x800018A8: addiu       $a3, $v0, -0x18
    ctx->r7 = ADD32(ctx->r2, -0X18);
    // 0x800018AC: lh          $t7, 0x0($a3)
    ctx->r15 = MEM_H(ctx->r7, 0X0);
    // 0x800018B0: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x800018B4: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x800018B8: ori         $t8, $t7, 0x4000
    ctx->r24 = ctx->r15 | 0X4000;
    // 0x800018BC: sh          $t8, 0x0($a3)
    MEM_H(0X0, ctx->r7) = ctx->r24;
    // 0x800018C0: lh          $t9, 0x24($t0)
    ctx->r25 = MEM_H(ctx->r8, 0X24);
    // 0x800018C4: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x800018C8: blez        $t9, L_800018EC
    if (SIGNED(ctx->r25) <= 0) {
        // 0x800018CC: addu        $t2, $a3, $a0
        ctx->r10 = ADD32(ctx->r7, ctx->r4);
            goto L_800018EC;
    }
L_800018CC:
    // 0x800018CC: addu        $t2, $a3, $a0
    ctx->r10 = ADD32(ctx->r7, ctx->r4);
    // 0x800018D0: sw          $a1, 0xC($t2)
    MEM_W(0XC, ctx->r10) = ctx->r5;
    // 0x800018D4: lh          $t3, 0x24($t0)
    ctx->r11 = MEM_H(ctx->r8, 0X24);
    // 0x800018D8: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x800018DC: addu        $a1, $a1, $a2
    ctx->r5 = ADD32(ctx->r5, ctx->r6);
    // 0x800018E0: slt         $at, $v1, $t3
    ctx->r1 = SIGNED(ctx->r3) < SIGNED(ctx->r11) ? 1 : 0;
    // 0x800018E4: bne         $at, $zero, L_800018CC
    if (ctx->r1 != 0) {
        // 0x800018E8: addiu       $a0, $a0, 0x4
        ctx->r4 = ADD32(ctx->r4, 0X4);
            goto L_800018CC;
    }
    // 0x800018E8: addiu       $a0, $a0, 0x4
    ctx->r4 = ADD32(ctx->r4, 0X4);
L_800018EC:
    // 0x800018EC: lh          $t4, 0x2C($t0)
    ctx->r12 = MEM_H(ctx->r8, 0X2C);
    // 0x800018F0: addiu       $v1, $t1, -0x18
    ctx->r3 = ADD32(ctx->r9, -0X18);
    // 0x800018F4: addiu       $v0, $v1, 0x8
    ctx->r2 = ADD32(ctx->r3, 0X8);
    // 0x800018F8: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x800018FC: addu        $t6, $v1, $t5
    ctx->r14 = ADD32(ctx->r3, ctx->r13);
    // 0x80001900: lw          $t7, 0xC($t6)
    ctx->r15 = MEM_W(ctx->r14, 0XC);
    // 0x80001904: sw          $t7, 0x8($v1)
    MEM_W(0X8, ctx->r3) = ctx->r15;
L_80001908:
    // 0x80001908: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8000190C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80001910: jr          $ra
    // 0x80001914: nop

    return;
    // 0x80001914: nop

;}
RECOMP_FUNC void func_80001918(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001918: jr          $ra
    // 0x8000191C: sh          $zero, -0x18($a0)
    MEM_H(-0X18, ctx->r4) = 0;
    return;
    // 0x8000191C: sh          $zero, -0x18($a0)
    MEM_H(-0X18, ctx->r4) = 0;
;}
RECOMP_FUNC void func_80001920(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001920: addiu       $sp, $sp, -0x8
    ctx->r29 = ADD32(ctx->r29, -0X8);
    // 0x80001924: lw          $v0, 0x4($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X4);
    // 0x80001928: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
    // 0x8000192C: subu        $t6, $a0, $v0
    ctx->r14 = SUB32(ctx->r4, ctx->r2);
    // 0x80001930: addu        $v0, $t6, $v0
    ctx->r2 = ADD32(ctx->r14, ctx->r2);
    // 0x80001934: addiu       $v0, $v0, -0x8
    ctx->r2 = ADD32(ctx->r2, -0X8);
    // 0x80001938: jr          $ra
    // 0x8000193C: sh          $zero, 0x0($v0)
    MEM_H(0X0, ctx->r2) = 0;
    return;
    // 0x8000193C: sh          $zero, 0x0($v0)
    MEM_H(0X0, ctx->r2) = 0;
;}
RECOMP_FUNC void func_80001940(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001940: lui         $v0, 0x801B
    ctx->r2 = S32(0X801B << 16);
    // 0x80001944: lui         $v1, 0x801B
    ctx->r3 = S32(0X801B << 16);
    // 0x80001948: addiu       $v1, $v1, 0x3D60
    ctx->r3 = ADD32(ctx->r3, 0X3D60);
    // 0x8000194C: addiu       $v0, $v0, 0x1D60
    ctx->r2 = ADD32(ctx->r2, 0X1D60);
L_80001950:
    // 0x80001950: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x80001954: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80001958: bne         $at, $zero, L_80001950
    if (ctx->r1 != 0) {
        // 0x8000195C: sw          $zero, -0x4($v0)
        MEM_W(-0X4, ctx->r2) = 0;
            goto L_80001950;
    }
    // 0x8000195C: sw          $zero, -0x4($v0)
    MEM_W(-0X4, ctx->r2) = 0;
    // 0x80001960: jr          $ra
    // 0x80001964: nop

    return;
    // 0x80001964: nop

;}
RECOMP_FUNC void func_80001968(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001968: lui         $v1, 0x801B
    ctx->r3 = S32(0X801B << 16);
    // 0x8000196C: lui         $v0, 0x801B
    ctx->r2 = S32(0X801B << 16);
    // 0x80001970: addiu       $v0, $v0, 0x3D60
    ctx->r2 = ADD32(ctx->r2, 0X3D60);
    // 0x80001974: addiu       $v1, $v1, 0x1D60
    ctx->r3 = ADD32(ctx->r3, 0X1D60);
    // 0x80001978: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
L_8000197C:
    // 0x8000197C: bnel        $t6, $zero, L_80001998
    if (ctx->r14 != 0) {
        // 0x80001980: addiu       $v1, $v1, 0x8
        ctx->r3 = ADD32(ctx->r3, 0X8);
            goto L_80001998;
    }
    goto skip_0;
    // 0x80001980: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
    skip_0:
    // 0x80001984: sw          $a0, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r4;
    // 0x80001988: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
    // 0x8000198C: jr          $ra
    // 0x80001990: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x80001990: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    // 0x80001994: addiu       $v1, $v1, 0x8
    ctx->r3 = ADD32(ctx->r3, 0X8);
L_80001998:
    // 0x80001998: sltu        $at, $v1, $v0
    ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
    // 0x8000199C: bnel        $at, $zero, L_8000197C
    if (ctx->r1 != 0) {
        // 0x800019A0: lw          $t6, 0x4($v1)
        ctx->r14 = MEM_W(ctx->r3, 0X4);
            goto L_8000197C;
    }
    goto skip_1;
    // 0x800019A0: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
    skip_1:
    // 0x800019A4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x800019A8: jr          $ra
    // 0x800019AC: nop

    return;
    // 0x800019AC: nop

;}
RECOMP_FUNC void func_800019B0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800019B0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800019B4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800019B8: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x800019BC: jal         0x80001968
    // 0x800019C0: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80001968(rdram, ctx);
        goto after_0;
    // 0x800019C0: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_0:
    // 0x800019C4: beq         $v0, $zero, L_800019D8
    if (ctx->r2 == 0) {
        // 0x800019C8: lw          $v1, 0x18($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X18);
            goto L_800019D8;
    }
    // 0x800019C8: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x800019CC: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x800019D0: sw          $t6, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r14;
    // 0x800019D4: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
L_800019D8:
    // 0x800019D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800019DC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800019E0: jr          $ra
    // 0x800019E4: nop

    return;
    // 0x800019E4: nop

;}
RECOMP_FUNC void func_800019E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800019E8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800019EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800019F0: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x800019F4: jal         0x80001968
    // 0x800019F8: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    func_80001968(rdram, ctx);
        goto after_0;
    // 0x800019F8: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    after_0:
    // 0x800019FC: beq         $v0, $zero, L_80001A3C
    if (ctx->r2 == 0) {
        // 0x80001A00: lw          $v1, 0x18($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X18);
            goto L_80001A3C;
    }
    // 0x80001A00: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80001A04: lw          $a1, 0x0($v1)
    ctx->r5 = MEM_W(ctx->r3, 0X0);
    // 0x80001A08: beql        $a1, $zero, L_80001A3C
    if (ctx->r5 == 0) {
        // 0x80001A0C: sw          $v0, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r2;
            goto L_80001A3C;
    }
    goto skip_0;
    // 0x80001A0C: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    skip_0:
    // 0x80001A10: lw          $v1, 0x0($a1)
    ctx->r3 = MEM_W(ctx->r5, 0X0);
    // 0x80001A14: or          $a0, $a1, $zero
    ctx->r4 = ctx->r5 | 0;
    // 0x80001A18: beq         $v1, $zero, L_80001A30
    if (ctx->r3 == 0) {
        // 0x80001A1C: nop
    
            goto L_80001A30;
    }
    // 0x80001A1C: nop

    // 0x80001A20: or          $a0, $v1, $zero
    ctx->r4 = ctx->r3 | 0;
L_80001A24:
    // 0x80001A24: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
    // 0x80001A28: bnel        $v1, $zero, L_80001A24
    if (ctx->r3 != 0) {
        // 0x80001A2C: or          $a0, $v1, $zero
        ctx->r4 = ctx->r3 | 0;
            goto L_80001A24;
    }
    goto skip_1;
    // 0x80001A2C: or          $a0, $v1, $zero
    ctx->r4 = ctx->r3 | 0;
    skip_1:
L_80001A30:
    // 0x80001A30: b           L_80001A3C
    // 0x80001A34: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
        goto L_80001A3C;
    // 0x80001A34: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
    // 0x80001A38: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
L_80001A3C:
    // 0x80001A3C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80001A40: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80001A44: jr          $ra
    // 0x80001A48: nop

    return;
    // 0x80001A48: nop

;}
RECOMP_FUNC void func_80001A4C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001A4C: lw          $v1, 0x0($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X0);
    // 0x80001A50: bnel        $v1, $zero, L_80001A64
    if (ctx->r3 != 0) {
        // 0x80001A54: lw          $v0, 0x4($v1)
        ctx->r2 = MEM_W(ctx->r3, 0X4);
            goto L_80001A64;
    }
    goto skip_0;
    // 0x80001A54: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
    skip_0:
    // 0x80001A58: jr          $ra
    // 0x80001A5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80001A5C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80001A60: lw          $v0, 0x4($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X4);
L_80001A64:
    // 0x80001A64: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x80001A68: lw          $t6, 0x0($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X0);
    // 0x80001A6C: lw          $t7, 0x0($t6)
    ctx->r15 = MEM_W(ctx->r14, 0X0);
    // 0x80001A70: sw          $t7, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r15;
    // 0x80001A74: jr          $ra
    // 0x80001A78: nop

    return;
    // 0x80001A78: nop

;}
RECOMP_FUNC void func_80001A7C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001A7C: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x80001A80: bnel        $v0, $zero, L_80001A94
    if (ctx->r2 != 0) {
        // 0x80001A84: lw          $a2, 0x0($v0)
        ctx->r6 = MEM_W(ctx->r2, 0X0);
            goto L_80001A94;
    }
    goto skip_0;
    // 0x80001A84: lw          $a2, 0x0($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X0);
    skip_0:
    // 0x80001A88: jr          $ra
    // 0x80001A8C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80001A8C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80001A90: lw          $a2, 0x0($v0)
    ctx->r6 = MEM_W(ctx->r2, 0X0);
L_80001A94:
    // 0x80001A94: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80001A98: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80001A9C: beql        $a2, $zero, L_80001ABC
    if (ctx->r6 == 0) {
        // 0x80001AA0: sw          $zero, 0x4($a1)
        MEM_W(0X4, ctx->r5) = 0;
            goto L_80001ABC;
    }
    goto skip_1;
    // 0x80001AA0: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
    skip_1:
    // 0x80001AA4: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
L_80001AA8:
    // 0x80001AA8: or          $v1, $a2, $zero
    ctx->r3 = ctx->r6 | 0;
    // 0x80001AAC: lw          $a2, 0x0($a2)
    ctx->r6 = MEM_W(ctx->r6, 0X0);
    // 0x80001AB0: bnel        $a2, $zero, L_80001AA8
    if (ctx->r6 != 0) {
        // 0x80001AB4: or          $a1, $a2, $zero
        ctx->r5 = ctx->r6 | 0;
            goto L_80001AA8;
    }
    goto skip_2;
    // 0x80001AB4: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    skip_2:
    // 0x80001AB8: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
L_80001ABC:
    // 0x80001ABC: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
    // 0x80001AC0: jr          $ra
    // 0x80001AC4: nop

    return;
    // 0x80001AC4: nop

;}
RECOMP_FUNC void func_80001AC8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001AC8: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x80001ACC: beql        $v0, $zero, L_80001AF8
    if (ctx->r2 == 0) {
        // 0x80001AD0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80001AF8;
    }
    goto skip_0;
    // 0x80001AD0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80001AD4: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
L_80001AD8:
    // 0x80001AD8: bnel        $a1, $t6, L_80001AEC
    if (ctx->r5 != ctx->r14) {
        // 0x80001ADC: lw          $v0, 0x0($v0)
        ctx->r2 = MEM_W(ctx->r2, 0X0);
            goto L_80001AEC;
    }
    goto skip_1;
    // 0x80001ADC: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    skip_1:
    // 0x80001AE0: jr          $ra
    // 0x80001AE4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80001AE4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80001AE8: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
L_80001AEC:
    // 0x80001AEC: bnel        $v0, $zero, L_80001AD8
    if (ctx->r2 != 0) {
        // 0x80001AF0: lw          $t6, 0x4($v0)
        ctx->r14 = MEM_W(ctx->r2, 0X4);
            goto L_80001AD8;
    }
    goto skip_2;
    // 0x80001AF0: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    skip_2:
    // 0x80001AF4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80001AF8:
    // 0x80001AF8: jr          $ra
    // 0x80001AFC: nop

    return;
    // 0x80001AFC: nop

;}
RECOMP_FUNC void func_80001B00(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001B00: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    // 0x80001B04: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    // 0x80001B08: or          $v1, $a0, $zero
    ctx->r3 = ctx->r4 | 0;
    // 0x80001B0C: bne         $v0, $zero, L_80001B1C
    if (ctx->r2 != 0) {
        // 0x80001B10: nop
    
            goto L_80001B1C;
    }
    // 0x80001B10: nop

    // 0x80001B14: jr          $ra
    // 0x80001B18: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    return;
    // 0x80001B18: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80001B1C:
    // 0x80001B1C: beq         $v0, $zero, L_80001B54
    if (ctx->r2 == 0) {
        // 0x80001B20: or          $a1, $v0, $zero
        ctx->r5 = ctx->r2 | 0;
            goto L_80001B54;
    }
    // 0x80001B20: or          $a1, $v0, $zero
    ctx->r5 = ctx->r2 | 0;
    // 0x80001B24: lw          $t6, 0x4($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X4);
L_80001B28:
    // 0x80001B28: bnel        $a2, $t6, L_80001B48
    if (ctx->r6 != ctx->r14) {
        // 0x80001B2C: or          $v1, $a1, $zero
        ctx->r3 = ctx->r5 | 0;
            goto L_80001B48;
    }
    goto skip_0;
    // 0x80001B2C: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    skip_0:
    // 0x80001B30: lw          $t7, 0x0($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X0);
    // 0x80001B34: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80001B38: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x80001B3C: jr          $ra
    // 0x80001B40: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
    return;
    // 0x80001B40: sw          $zero, 0x4($a1)
    MEM_W(0X4, ctx->r5) = 0;
    // 0x80001B44: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
L_80001B48:
    // 0x80001B48: lw          $a1, 0x0($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X0);
    // 0x80001B4C: bnel        $a1, $zero, L_80001B28
    if (ctx->r5 != 0) {
        // 0x80001B50: lw          $t6, 0x4($a1)
        ctx->r14 = MEM_W(ctx->r5, 0X4);
            goto L_80001B28;
    }
    goto skip_1;
    // 0x80001B50: lw          $t6, 0x4($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X4);
    skip_1:
L_80001B54:
    // 0x80001B54: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80001B58: jr          $ra
    // 0x80001B5C: nop

    return;
    // 0x80001B5C: nop

;}
RECOMP_FUNC void func_80001B60(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001B60: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    // 0x80001B64: sw          $v0, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r2;
    // 0x80001B68: sw          $a1, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r5;
    // 0x80001B6C: sw          $a1, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r5;
    // 0x80001B70: jr          $ra
    // 0x80001B74: sw          $a0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r4;
    return;
    // 0x80001B74: sw          $a0, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r4;
;}
RECOMP_FUNC void func_80001B78(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001B78: sw          $a0, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r4;
    // 0x80001B7C: sw          $a1, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r5;
    // 0x80001B80: beq         $a1, $zero, L_80001B8C
    if (ctx->r5 == 0) {
        // 0x80001B84: sw          $a1, 0x0($a1)
        MEM_W(0X0, ctx->r5) = ctx->r5;
            goto L_80001B8C;
    }
    // 0x80001B84: sw          $a1, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r5;
    // 0x80001B88: sw          $a1, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r5;
L_80001B8C:
    // 0x80001B8C: jr          $ra
    // 0x80001B90: nop

    return;
    // 0x80001B90: nop

;}
RECOMP_FUNC void func_80001B94(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001B94: beq         $a1, $zero, L_80001BD0
    if (ctx->r5 == 0) {
        // 0x80001B98: or          $v0, $a1, $zero
        ctx->r2 = ctx->r5 | 0;
            goto L_80001BD0;
    }
    // 0x80001B98: or          $v0, $a1, $zero
    ctx->r2 = ctx->r5 | 0;
    // 0x80001B9C: lw          $t6, 0x0($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X0);
    // 0x80001BA0: beql        $t6, $zero, L_80001BC0
    if (ctx->r14 == 0) {
        // 0x80001BA4: lw          $v1, 0x4($a0)
        ctx->r3 = MEM_W(ctx->r4, 0X4);
            goto L_80001BC0;
    }
    goto skip_0;
    // 0x80001BA4: lw          $v1, 0x4($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X4);
    skip_0:
    // 0x80001BA8: lw          $v1, 0x0($v0)
    ctx->r3 = MEM_W(ctx->r2, 0X0);
    // 0x80001BAC: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
L_80001BB0:
    // 0x80001BB0: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
    // 0x80001BB4: bnel        $v1, $zero, L_80001BB0
    if (ctx->r3 != 0) {
        // 0x80001BB8: or          $v0, $v1, $zero
        ctx->r2 = ctx->r3 | 0;
            goto L_80001BB0;
    }
    goto skip_1;
    // 0x80001BB8: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    skip_1:
    // 0x80001BBC: lw          $v1, 0x4($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X4);
L_80001BC0:
    // 0x80001BC0: sw          $v1, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r3;
    // 0x80001BC4: sw          $a1, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r5;
    // 0x80001BC8: sw          $v0, 0x4($a0)
    MEM_W(0X4, ctx->r4) = ctx->r2;
    // 0x80001BCC: sw          $a0, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r4;
L_80001BD0:
    // 0x80001BD0: jr          $ra
    // 0x80001BD4: nop

    return;
    // 0x80001BD4: nop

;}
RECOMP_FUNC void func_80001BD8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001BD8: sw          $a0, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r4;
    // 0x80001BDC: beq         $a1, $zero, L_80001C18
    if (ctx->r5 == 0) {
        // 0x80001BE0: sw          $a1, 0x0($a0)
        MEM_W(0X0, ctx->r4) = ctx->r5;
            goto L_80001C18;
    }
    // 0x80001BE0: sw          $a1, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r5;
    // 0x80001BE4: beq         $a1, $zero, L_80001C18
    if (ctx->r5 == 0) {
        // 0x80001BE8: or          $v1, $a1, $zero
        ctx->r3 = ctx->r5 | 0;
            goto L_80001C18;
    }
    // 0x80001BE8: or          $v1, $a1, $zero
    ctx->r3 = ctx->r5 | 0;
    // 0x80001BEC: lw          $t6, 0x0($a1)
    ctx->r14 = MEM_W(ctx->r5, 0X0);
    // 0x80001BF0: beql        $t6, $zero, L_80001C10
    if (ctx->r14 == 0) {
        // 0x80001BF4: lw          $v0, 0x0($a0)
        ctx->r2 = MEM_W(ctx->r4, 0X0);
            goto L_80001C10;
    }
    goto skip_0;
    // 0x80001BF4: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
    skip_0:
    // 0x80001BF8: lw          $v0, 0x0($a1)
    ctx->r2 = MEM_W(ctx->r5, 0X0);
    // 0x80001BFC: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
L_80001C00:
    // 0x80001C00: lw          $v0, 0x0($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X0);
    // 0x80001C04: bnel        $v0, $zero, L_80001C00
    if (ctx->r2 != 0) {
        // 0x80001C08: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80001C00;
    }
    goto skip_1;
    // 0x80001C08: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    skip_1:
    // 0x80001C0C: lw          $v0, 0x0($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X0);
L_80001C10:
    // 0x80001C10: sw          $v0, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r2;
    // 0x80001C14: sw          $v1, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r3;
L_80001C18:
    // 0x80001C18: jr          $ra
    // 0x80001C1C: nop

    return;
    // 0x80001C1C: nop

;}
RECOMP_FUNC void func_80001C20(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001C20: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001C24: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001C28: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001C2C: lhu         $t8, 0x0($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X0);
    // 0x80001C30: addiu       $v1, $v0, 0x2
    ctx->r3 = ADD32(ctx->r2, 0X2);
    // 0x80001C34: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001C38: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x80001C3C: beq         $at, $zero, L_80001C54
    if (ctx->r1 == 0) {
        // 0x80001C40: sh          $t9, 0x0($v0)
        MEM_H(0X0, ctx->r2) = ctx->r25;
            goto L_80001C54;
    }
    // 0x80001C40: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
L_80001C44:
    // 0x80001C44: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80001C48: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001C4C: bne         $at, $zero, L_80001C44
    if (ctx->r1 != 0) {
        // 0x80001C50: sh          $zero, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = 0;
            goto L_80001C44;
    }
    // 0x80001C50: sh          $zero, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = 0;
L_80001C54:
    // 0x80001C54: jr          $ra
    // 0x80001C58: nop

    return;
    // 0x80001C58: nop

;}
RECOMP_FUNC void func_80001C5C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001C5C: lh          $v0, 0x0($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X0);
    // 0x80001C60: blez        $v0, L_80001C94
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80001C64: sll         $t6, $v0, 1
        ctx->r14 = S32(ctx->r2 << 1);
            goto L_80001C94;
    }
    // 0x80001C64: sll         $t6, $v0, 1
    ctx->r14 = S32(ctx->r2 << 1);
    // 0x80001C68: addu        $v1, $t6, $a0
    ctx->r3 = ADD32(ctx->r14, ctx->r4);
    // 0x80001C6C: lhu         $t7, -0x2($v1)
    ctx->r15 = MEM_HU(ctx->r3, -0X2);
    // 0x80001C70: addiu       $a2, $v1, 0x0
    ctx->r6 = ADD32(ctx->r3, 0X0);
    // 0x80001C74: sltu        $at, $a2, $a1
    ctx->r1 = ctx->r6 < ctx->r5 ? 1 : 0;
    // 0x80001C78: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x80001C7C: beq         $at, $zero, L_80001C94
    if (ctx->r1 == 0) {
        // 0x80001C80: sh          $t8, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = ctx->r24;
            goto L_80001C94;
    }
    // 0x80001C80: sh          $t8, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = ctx->r24;
L_80001C84:
    // 0x80001C84: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x80001C88: sltu        $at, $a2, $a1
    ctx->r1 = ctx->r6 < ctx->r5 ? 1 : 0;
    // 0x80001C8C: bne         $at, $zero, L_80001C84
    if (ctx->r1 != 0) {
        // 0x80001C90: sh          $zero, -0x2($a2)
        MEM_H(-0X2, ctx->r6) = 0;
            goto L_80001C84;
    }
    // 0x80001C90: sh          $zero, -0x2($a2)
    MEM_H(-0X2, ctx->r6) = 0;
L_80001C94:
    // 0x80001C94: jr          $ra
    // 0x80001C98: nop

    return;
    // 0x80001C98: nop

;}
RECOMP_FUNC void func_80001C9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001C9C: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001CA0: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001CA4: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001CA8: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80001CAC: sltu        $at, $v0, $a1
    ctx->r1 = ctx->r2 < ctx->r5 ? 1 : 0;
    // 0x80001CB0: beq         $at, $zero, L_80001CE0
    if (ctx->r1 == 0) {
        // 0x80001CB4: nop
    
            goto L_80001CE0;
    }
    // 0x80001CB4: nop

    // 0x80001CB8: lhu         $t8, 0x0($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X0);
    // 0x80001CBC: addiu       $a0, $v0, 0x2
    ctx->r4 = ADD32(ctx->r2, 0X2);
    // 0x80001CC0: sltu        $at, $a0, $a1
    ctx->r1 = ctx->r4 < ctx->r5 ? 1 : 0;
    // 0x80001CC4: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x80001CC8: beq         $at, $zero, L_80001CE0
    if (ctx->r1 == 0) {
        // 0x80001CCC: sh          $t9, 0x0($v0)
        MEM_H(0X0, ctx->r2) = ctx->r25;
            goto L_80001CE0;
    }
    // 0x80001CCC: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
L_80001CD0:
    // 0x80001CD0: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x80001CD4: sltu        $at, $a0, $a1
    ctx->r1 = ctx->r4 < ctx->r5 ? 1 : 0;
    // 0x80001CD8: bne         $at, $zero, L_80001CD0
    if (ctx->r1 != 0) {
        // 0x80001CDC: sh          $zero, -0x2($a0)
        MEM_H(-0X2, ctx->r4) = 0;
            goto L_80001CD0;
    }
    // 0x80001CDC: sh          $zero, -0x2($a0)
    MEM_H(-0X2, ctx->r4) = 0;
L_80001CE0:
    // 0x80001CE0: jr          $ra
    // 0x80001CE4: nop

    return;
    // 0x80001CE4: nop

;}
RECOMP_FUNC void func_80001CE8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001CE8: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001CEC: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001CF0: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001CF4: lhu         $t8, 0x0($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X0);
    // 0x80001CF8: addiu       $v1, $v0, 0x2
    ctx->r3 = ADD32(ctx->r2, 0X2);
    // 0x80001CFC: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001D00: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x80001D04: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
    // 0x80001D08: beq         $at, $zero, L_80001D20
    if (ctx->r1 == 0) {
        // 0x80001D0C: sb          $zero, 0x0($v0)
        MEM_B(0X0, ctx->r2) = 0;
            goto L_80001D20;
    }
    // 0x80001D0C: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
L_80001D10:
    // 0x80001D10: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80001D14: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001D18: bne         $at, $zero, L_80001D10
    if (ctx->r1 != 0) {
        // 0x80001D1C: sh          $zero, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = 0;
            goto L_80001D10;
    }
    // 0x80001D1C: sh          $zero, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = 0;
L_80001D20:
    // 0x80001D20: jr          $ra
    // 0x80001D24: nop

    return;
    // 0x80001D24: nop

;}
RECOMP_FUNC void func_80001D28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001D28: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001D2C: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001D30: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001D34: lhu         $t8, 0x0($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X0);
    // 0x80001D38: addiu       $v1, $v0, 0x2
    ctx->r3 = ADD32(ctx->r2, 0X2);
    // 0x80001D3C: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001D40: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x80001D44: beq         $at, $zero, L_80001D5C
    if (ctx->r1 == 0) {
        // 0x80001D48: sh          $t9, 0x0($v0)
        MEM_H(0X0, ctx->r2) = ctx->r25;
            goto L_80001D5C;
    }
    // 0x80001D48: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
L_80001D4C:
    // 0x80001D4C: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80001D50: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001D54: bne         $at, $zero, L_80001D4C
    if (ctx->r1 != 0) {
        // 0x80001D58: sh          $zero, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = 0;
            goto L_80001D4C;
    }
    // 0x80001D58: sh          $zero, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = 0;
L_80001D5C:
    // 0x80001D5C: jr          $ra
    // 0x80001D60: nop

    return;
    // 0x80001D60: nop

;}
RECOMP_FUNC void func_80001D64(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001D64: lh          $v0, 0x0($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X0);
    // 0x80001D68: blez        $v0, L_80001D9C
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80001D6C: sll         $t6, $v0, 1
        ctx->r14 = S32(ctx->r2 << 1);
            goto L_80001D9C;
    }
    // 0x80001D6C: sll         $t6, $v0, 1
    ctx->r14 = S32(ctx->r2 << 1);
    // 0x80001D70: addu        $v1, $t6, $a0
    ctx->r3 = ADD32(ctx->r14, ctx->r4);
    // 0x80001D74: lhu         $t7, -0x2($v1)
    ctx->r15 = MEM_HU(ctx->r3, -0X2);
    // 0x80001D78: addiu       $a2, $v1, 0x0
    ctx->r6 = ADD32(ctx->r3, 0X0);
    // 0x80001D7C: sltu        $at, $a2, $a1
    ctx->r1 = ctx->r6 < ctx->r5 ? 1 : 0;
    // 0x80001D80: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x80001D84: beq         $at, $zero, L_80001D9C
    if (ctx->r1 == 0) {
        // 0x80001D88: sh          $t8, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = ctx->r24;
            goto L_80001D9C;
    }
    // 0x80001D88: sh          $t8, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = ctx->r24;
L_80001D8C:
    // 0x80001D8C: addiu       $a2, $a2, 0x2
    ctx->r6 = ADD32(ctx->r6, 0X2);
    // 0x80001D90: sltu        $at, $a2, $a1
    ctx->r1 = ctx->r6 < ctx->r5 ? 1 : 0;
    // 0x80001D94: bne         $at, $zero, L_80001D8C
    if (ctx->r1 != 0) {
        // 0x80001D98: sh          $zero, -0x2($a2)
        MEM_H(-0X2, ctx->r6) = 0;
            goto L_80001D8C;
    }
    // 0x80001D98: sh          $zero, -0x2($a2)
    MEM_H(-0X2, ctx->r6) = 0;
L_80001D9C:
    // 0x80001D9C: jr          $ra
    // 0x80001DA0: nop

    return;
    // 0x80001DA0: nop

;}
RECOMP_FUNC void func_80001DA4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001DA4: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001DA8: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001DAC: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001DB0: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80001DB4: sltu        $at, $v0, $a1
    ctx->r1 = ctx->r2 < ctx->r5 ? 1 : 0;
    // 0x80001DB8: beq         $at, $zero, L_80001DE8
    if (ctx->r1 == 0) {
        // 0x80001DBC: nop
    
            goto L_80001DE8;
    }
    // 0x80001DBC: nop

    // 0x80001DC0: lhu         $t8, 0x0($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X0);
    // 0x80001DC4: addiu       $a0, $v0, 0x2
    ctx->r4 = ADD32(ctx->r2, 0X2);
    // 0x80001DC8: sltu        $at, $a0, $a1
    ctx->r1 = ctx->r4 < ctx->r5 ? 1 : 0;
    // 0x80001DCC: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x80001DD0: beq         $at, $zero, L_80001DE8
    if (ctx->r1 == 0) {
        // 0x80001DD4: sh          $t9, 0x0($v0)
        MEM_H(0X0, ctx->r2) = ctx->r25;
            goto L_80001DE8;
    }
    // 0x80001DD4: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
L_80001DD8:
    // 0x80001DD8: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x80001DDC: sltu        $at, $a0, $a1
    ctx->r1 = ctx->r4 < ctx->r5 ? 1 : 0;
    // 0x80001DE0: bne         $at, $zero, L_80001DD8
    if (ctx->r1 != 0) {
        // 0x80001DE4: sh          $zero, -0x2($a0)
        MEM_H(-0X2, ctx->r4) = 0;
            goto L_80001DD8;
    }
    // 0x80001DE4: sh          $zero, -0x2($a0)
    MEM_H(-0X2, ctx->r4) = 0;
L_80001DE8:
    // 0x80001DE8: jr          $ra
    // 0x80001DEC: nop

    return;
    // 0x80001DEC: nop

;}
RECOMP_FUNC void func_80001DF0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001DF0: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001DF4: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001DF8: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001DFC: lhu         $t8, 0x0($v0)
    ctx->r24 = MEM_HU(ctx->r2, 0X0);
    // 0x80001E00: addiu       $v1, $v0, 0x2
    ctx->r3 = ADD32(ctx->r2, 0X2);
    // 0x80001E04: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001E08: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x80001E0C: sh          $t9, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r25;
    // 0x80001E10: beq         $at, $zero, L_80001E28
    if (ctx->r1 == 0) {
        // 0x80001E14: sb          $zero, 0x0($v0)
        MEM_B(0X0, ctx->r2) = 0;
            goto L_80001E28;
    }
    // 0x80001E14: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
L_80001E18:
    // 0x80001E18: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80001E1C: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001E20: bne         $at, $zero, L_80001E18
    if (ctx->r1 != 0) {
        // 0x80001E24: sh          $zero, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = 0;
            goto L_80001E18;
    }
    // 0x80001E24: sh          $zero, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = 0;
L_80001E28:
    // 0x80001E28: jr          $ra
    // 0x80001E2C: nop

    return;
    // 0x80001E2C: nop

;}
RECOMP_FUNC void func_80001E30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001E30: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001E34: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001E38: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001E3C: addiu       $v1, $v0, 0x2
    ctx->r3 = ADD32(ctx->r2, 0X2);
    // 0x80001E40: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001E44: beq         $at, $zero, L_80001E5C
    if (ctx->r1 == 0) {
        // 0x80001E48: sh          $a2, 0x0($v0)
        MEM_H(0X0, ctx->r2) = ctx->r6;
            goto L_80001E5C;
    }
    // 0x80001E48: sh          $a2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r6;
L_80001E4C:
    // 0x80001E4C: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80001E50: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001E54: bne         $at, $zero, L_80001E4C
    if (ctx->r1 != 0) {
        // 0x80001E58: sh          $zero, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = 0;
            goto L_80001E4C;
    }
    // 0x80001E58: sh          $zero, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = 0;
L_80001E5C:
    // 0x80001E5C: jr          $ra
    // 0x80001E60: nop

    return;
    // 0x80001E60: nop

;}
RECOMP_FUNC void func_80001E64(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001E64: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80001E68: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x80001E6C: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80001E70: sll         $t6, $a2, 1
    ctx->r14 = S32(ctx->r6 << 1);
    // 0x80001E74: addu        $v0, $t6, $a0
    ctx->r2 = ADD32(ctx->r14, ctx->r4);
    // 0x80001E78: addiu       $v1, $v0, 0x2
    ctx->r3 = ADD32(ctx->r2, 0X2);
    // 0x80001E7C: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001E80: beq         $at, $zero, L_80001E98
    if (ctx->r1 == 0) {
        // 0x80001E84: sh          $a3, 0x0($v0)
        MEM_H(0X0, ctx->r2) = ctx->r7;
            goto L_80001E98;
    }
    // 0x80001E84: sh          $a3, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r7;
L_80001E88:
    // 0x80001E88: addiu       $v1, $v1, 0x2
    ctx->r3 = ADD32(ctx->r3, 0X2);
    // 0x80001E8C: sltu        $at, $v1, $a1
    ctx->r1 = ctx->r3 < ctx->r5 ? 1 : 0;
    // 0x80001E90: bne         $at, $zero, L_80001E88
    if (ctx->r1 != 0) {
        // 0x80001E94: sh          $zero, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = 0;
            goto L_80001E88;
    }
    // 0x80001E94: sh          $zero, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = 0;
L_80001E98:
    // 0x80001E98: jr          $ra
    // 0x80001E9C: nop

    return;
    // 0x80001E9C: nop

;}
RECOMP_FUNC void func_80001EA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001EA0: lh          $v0, 0x0($a1)
    ctx->r2 = MEM_H(ctx->r5, 0X0);
    // 0x80001EA4: blez        $v0, L_80001ED0
    if (SIGNED(ctx->r2) <= 0) {
        // 0x80001EA8: sll         $t6, $v0, 1
        ctx->r14 = S32(ctx->r2 << 1);
            goto L_80001ED0;
    }
    // 0x80001EA8: sll         $t6, $v0, 1
    ctx->r14 = S32(ctx->r2 << 1);
    // 0x80001EAC: addu        $v1, $t6, $a0
    ctx->r3 = ADD32(ctx->r14, ctx->r4);
    // 0x80001EB0: addiu       $a3, $v1, 0x0
    ctx->r7 = ADD32(ctx->r3, 0X0);
    // 0x80001EB4: sltu        $at, $a3, $a1
    ctx->r1 = ctx->r7 < ctx->r5 ? 1 : 0;
    // 0x80001EB8: beq         $at, $zero, L_80001ED0
    if (ctx->r1 == 0) {
        // 0x80001EBC: sh          $a2, -0x2($v1)
        MEM_H(-0X2, ctx->r3) = ctx->r6;
            goto L_80001ED0;
    }
    // 0x80001EBC: sh          $a2, -0x2($v1)
    MEM_H(-0X2, ctx->r3) = ctx->r6;
L_80001EC0:
    // 0x80001EC0: addiu       $a3, $a3, 0x2
    ctx->r7 = ADD32(ctx->r7, 0X2);
    // 0x80001EC4: sltu        $at, $a3, $a1
    ctx->r1 = ctx->r7 < ctx->r5 ? 1 : 0;
    // 0x80001EC8: bne         $at, $zero, L_80001EC0
    if (ctx->r1 != 0) {
        // 0x80001ECC: sh          $zero, -0x2($a3)
        MEM_H(-0X2, ctx->r7) = 0;
            goto L_80001EC0;
    }
    // 0x80001ECC: sh          $zero, -0x2($a3)
    MEM_H(-0X2, ctx->r7) = 0;
L_80001ED0:
    // 0x80001ED0: jr          $ra
    // 0x80001ED4: nop

    return;
    // 0x80001ED4: nop

;}
RECOMP_FUNC void func_80001ED8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001ED8: lh          $t6, 0x0($a1)
    ctx->r14 = MEM_H(ctx->r5, 0X0);
    // 0x80001EDC: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x80001EE0: addu        $v0, $t7, $a0
    ctx->r2 = ADD32(ctx->r15, ctx->r4);
    // 0x80001EE4: addiu       $v0, $v0, 0x2
    ctx->r2 = ADD32(ctx->r2, 0X2);
    // 0x80001EE8: sltu        $at, $v0, $a1
    ctx->r1 = ctx->r2 < ctx->r5 ? 1 : 0;
    // 0x80001EEC: beq         $at, $zero, L_80001F10
    if (ctx->r1 == 0) {
        // 0x80001EF0: addiu       $a0, $v0, 0x2
        ctx->r4 = ADD32(ctx->r2, 0X2);
            goto L_80001F10;
    }
    // 0x80001EF0: addiu       $a0, $v0, 0x2
    ctx->r4 = ADD32(ctx->r2, 0X2);
    // 0x80001EF4: sltu        $at, $a0, $a1
    ctx->r1 = ctx->r4 < ctx->r5 ? 1 : 0;
    // 0x80001EF8: beq         $at, $zero, L_80001F10
    if (ctx->r1 == 0) {
        // 0x80001EFC: sh          $a2, 0x0($v0)
        MEM_H(0X0, ctx->r2) = ctx->r6;
            goto L_80001F10;
    }
    // 0x80001EFC: sh          $a2, 0x0($v0)
    MEM_H(0X0, ctx->r2) = ctx->r6;
L_80001F00:
    // 0x80001F00: addiu       $a0, $a0, 0x2
    ctx->r4 = ADD32(ctx->r4, 0X2);
    // 0x80001F04: sltu        $at, $a0, $a1
    ctx->r1 = ctx->r4 < ctx->r5 ? 1 : 0;
    // 0x80001F08: bne         $at, $zero, L_80001F00
    if (ctx->r1 != 0) {
        // 0x80001F0C: sh          $zero, -0x2($a0)
        MEM_H(-0X2, ctx->r4) = 0;
            goto L_80001F00;
    }
    // 0x80001F0C: sh          $zero, -0x2($a0)
    MEM_H(-0X2, ctx->r4) = 0;
L_80001F10:
    // 0x80001F10: jr          $ra
    // 0x80001F14: nop

    return;
    // 0x80001F14: nop

;}
RECOMP_FUNC void func_80001F30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001F30: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80001F34: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80001F38: lw          $a1, 0x18($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X18);
    // 0x80001F3C: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x80001F40: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80001F44: addu        $at, $a1, $zero
    ctx->r1 = ADD32(ctx->r5, 0);
    // 0x80001F48: sll         $a1, $a1, 3
    ctx->r5 = S32(ctx->r5 << 3);
    // 0x80001F4C: subu        $a1, $a1, $at
    ctx->r5 = SUB32(ctx->r5, ctx->r1);
    // 0x80001F50: sll         $a1, $a1, 2
    ctx->r5 = S32(ctx->r5 << 2);
    // 0x80001F54: addu        $a1, $a1, $at
    ctx->r5 = ADD32(ctx->r5, ctx->r1);
    // 0x80001F58: addiu       $t9, $t9, 0x1514
    ctx->r25 = ADD32(ctx->r25, 0X1514);
    // 0x80001F5C: sll         $a1, $a1, 2
    ctx->r5 = S32(ctx->r5 << 2);
    // 0x80001F60: jalr        $t9
    // 0x80001F64: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x80001F64: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_0:
    // 0x80001F68: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80001F6C: addiu       $a0, $a0, 0x1530
    ctx->r4 = ADD32(ctx->r4, 0X1530);
    // 0x80001F70: sw          $v0, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r2;
    // 0x80001F74: bne         $v0, $zero, L_80001F84
    if (ctx->r2 != 0) {
        // 0x80001F78: or          $v1, $v0, $zero
        ctx->r3 = ctx->r2 | 0;
            goto L_80001F84;
    }
    // 0x80001F78: or          $v1, $v0, $zero
    ctx->r3 = ctx->r2 | 0;
    // 0x80001F7C: b           L_80001FC0
    // 0x80001F80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80001FC0;
    // 0x80001F80: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80001F84:
    // 0x80001F84: lw          $t6, 0x18($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X18);
    // 0x80001F88: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80001F8C: addiu       $v0, $v0, 0x1538
    ctx->r2 = ADD32(ctx->r2, 0X1538);
    // 0x80001F90: sll         $t8, $t6, 3
    ctx->r24 = S32(ctx->r14 << 3);
    // 0x80001F94: subu        $t8, $t8, $t6
    ctx->r24 = SUB32(ctx->r24, ctx->r14);
    // 0x80001F98: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80001F9C: addu        $t8, $t8, $t6
    ctx->r24 = ADD32(ctx->r24, ctx->r14);
    // 0x80001FA0: sll         $t8, $t8, 2
    ctx->r24 = S32(ctx->r24 << 2);
    // 0x80001FA4: addu        $t0, $t8, $v1
    ctx->r8 = ADD32(ctx->r24, ctx->r3);
    // 0x80001FA8: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80001FAC: sw          $t6, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r14;
    // 0x80001FB0: jal         0x800021BC
    // 0x80001FB4: sw          $t0, 0x1534($at)
    MEM_W(0X1534, ctx->r1) = ctx->r8;
    func_800021BC(rdram, ctx);
        goto after_1;
    // 0x80001FB4: sw          $t0, 0x1534($at)
    MEM_W(0X1534, ctx->r1) = ctx->r8;
    after_1:
    // 0x80001FB8: lui         $v0, 0x800C
    ctx->r2 = S32(0X800C << 16);
    // 0x80001FBC: lw          $v0, 0x1538($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X1538);
L_80001FC0:
    // 0x80001FC0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80001FC4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80001FC8: jr          $ra
    // 0x80001FCC: nop

    return;
    // 0x80001FCC: nop

;}
RECOMP_FUNC void func_80001FD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80001FD0: lui         $t6, 0x800C
    ctx->r14 = S32(0X800C << 16);
    // 0x80001FD4: lw          $t6, 0x1530($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X1530);
    // 0x80001FD8: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x80001FDC: sltu        $v0, $a0, $t6
    ctx->r2 = ctx->r4 < ctx->r14 ? 1 : 0;
    // 0x80001FE0: xori        $v0, $v0, 0x1
    ctx->r2 = ctx->r2 ^ 0X1;
    // 0x80001FE4: beq         $v0, $zero, L_80001FF4
    if (ctx->r2 == 0) {
        // 0x80001FE8: nop
    
            goto L_80001FF4;
    }
    // 0x80001FE8: nop

    // 0x80001FEC: lw          $t7, 0x1534($t7)
    ctx->r15 = MEM_W(ctx->r15, 0X1534);
    // 0x80001FF0: sltu        $v0, $a0, $t7
    ctx->r2 = ctx->r4 < ctx->r15 ? 1 : 0;
L_80001FF4:
    // 0x80001FF4: jr          $ra
    // 0x80001FF8: nop

    return;
    // 0x80001FF8: nop

;}
