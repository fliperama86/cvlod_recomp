#include "recomp.h"
#include "funcs.h"
#include <stdio.h>

RECOMP_FUNC void func_800183A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800183A0: sll         $t6, $a0, 1
    ctx->r14 = S32(ctx->r4 << 1);
    // 0x800183A4: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x800183A8: addiu       $t9, $t9, 0xF88
    ctx->r25 = ADD32(ctx->r25, 0XF88);
    // 0x800183AC: sll         $t8, $t6, 2
    ctx->r24 = S32(ctx->r14 << 2);
    // 0x800183B0: addu        $v0, $t8, $t9
    ctx->r2 = ADD32(ctx->r24, ctx->r25);
    // 0x800183B4: addiu       $v1, $v0, -0x8
    ctx->r3 = ADD32(ctx->r2, -0X8);
    // 0x800183B8: lw          $a3, 0x10($v1)
    ctx->r7 = MEM_W(ctx->r3, 0X10);
    // 0x800183BC: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x800183C0: addiu       $sp, $sp, -0x30
    ctx->r29 = ADD32(ctx->r29, -0X30);
    // 0x800183C4: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x800183C8: and         $t0, $a3, $at
    ctx->r8 = ctx->r7 & ctx->r1;
    // 0x800183CC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800183D0: sltiu       $at, $a0, 0x1D9
    ctx->r1 = ctx->r4 < 0X1D9 ? 1 : 0;
    // 0x800183D4: or          $s0, $a1, $zero
    ctx->r16 = ctx->r5 | 0;
    // 0x800183D8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800183DC: sw          $t0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r8;
    // 0x800183E0: bne         $at, $zero, L_800183F0
    if (ctx->r1 != 0) {
        // 0x800183E4: sw          $t6, 0x28($sp)
        MEM_W(0X28, ctx->r29) = ctx->r14;
            goto L_800183F0;
    }
    // 0x800183E4: sw          $t6, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r14;
L_800183E8:
    // 0x800183E8: b           L_800183E8
    pause_self(rdram);
    // 0x800183EC: nop

L_800183F0:
    // 0x800183F0: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x800183F4: addiu       $t1, $t1, 0xF88
    ctx->r9 = ADD32(ctx->r9, 0XF88);
    // 0x800183F8: beq         $v0, $t1, L_800184B0
    if (ctx->r2 == ctx->r9) {
        // 0x800183FC: lw          $t2, 0x2C($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X2C);
            goto L_800184B0;
    }
    // 0x800183FC: lw          $t2, 0x2C($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X2C);
    // 0x80018400: lw          $v0, 0x14($v1)
    ctx->r2 = MEM_W(ctx->r3, 0X14);
    // 0x80018404: sll         $t3, $a3, 0
    ctx->r11 = S32(ctx->r7 << 0);
    // 0x80018408: subu        $a2, $v0, $t2
    ctx->r6 = SUB32(ctx->r2, ctx->r10);
    // 0x8001840C: beql        $a2, $zero, L_80018488
    if (ctx->r6 == 0) {
        // 0x80018410: lw          $t2, 0x28($sp)
        ctx->r10 = MEM_W(ctx->r29, 0X28);
            goto L_80018488;
    }
    goto skip_0;
    // 0x80018410: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
    skip_0:
    // 0x80018414: bltz        $t3, L_80018434
    if (SIGNED(ctx->r11) < 0) {
        // 0x80018418: lw          $a0, 0x2C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X2C);
            goto L_80018434;
    }
    // 0x80018418: lw          $a0, 0x2C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X2C);
    // 0x8001841C: or          $a0, $a3, $zero
    ctx->r4 = ctx->r7 | 0;
    // 0x80018420: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    // 0x80018424: jal         0x8001A42C
    // 0x80018428: subu        $a2, $v0, $a3
    ctx->r6 = SUB32(ctx->r2, ctx->r7);
    func_8001A42C(rdram, ctx);
        goto after_0;
    // 0x80018428: subu        $a2, $v0, $a3
    ctx->r6 = SUB32(ctx->r2, ctx->r7);
    after_0:
    // 0x8001842C: b           L_80018488
    // 0x80018430: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
        goto L_80018488;
    // 0x80018430: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
L_80018434:
    // 0x80018434: jal         0x800199C0
    // 0x80018438: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_800199C0(rdram, ctx);
        goto after_1;
    // 0x80018438: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_1:
    // 0x8001843C: lw          $t4, 0x28($sp)
    ctx->r12 = MEM_W(ctx->r29, 0X28);
    // 0x80018440: lui         $t7, 0x800B
    ctx->r15 = S32(0X800B << 16);
    // 0x80018444: addiu       $t7, $t7, 0xC4
    ctx->r15 = ADD32(ctx->r15, 0XC4);
    // 0x80018448: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x8001844C: addiu       $t6, $t5, -0x8
    ctx->r14 = ADD32(ctx->r13, -0X8);
    // 0x80018450: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x80018454: lw          $t8, 0x4($v1)
    ctx->r24 = MEM_W(ctx->r3, 0X4);
    // 0x80018458: lw          $t9, 0x0($v1)
    ctx->r25 = MEM_W(ctx->r3, 0X0);
    // 0x8001845C: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    // 0x80018460: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80018464: jal         0x80099440
    // 0x80018468: subu        $a1, $t8, $t9
    ctx->r5 = SUB32(ctx->r24, ctx->r25);
    osWritebackDCache_recomp(rdram, ctx);
        goto after_2;
    // 0x80018468: subu        $a1, $t8, $t9
    ctx->r5 = SUB32(ctx->r24, ctx->r25);
    after_2:
    // 0x8001846C: lw          $v1, 0x24($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X24);
    // 0x80018470: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80018474: lw          $t0, 0x4($v1)
    ctx->r8 = MEM_W(ctx->r3, 0X4);
    // 0x80018478: lw          $t1, 0x0($v1)
    ctx->r9 = MEM_W(ctx->r3, 0X0);
    // 0x8001847C: jal         0x800A13E0
    // 0x80018480: subu        $a1, $t0, $t1
    ctx->r5 = SUB32(ctx->r8, ctx->r9);
    osInvalICache_recomp(rdram, ctx);
        goto after_3;
    // 0x80018480: subu        $a1, $t0, $t1
    ctx->r5 = SUB32(ctx->r8, ctx->r9);
    after_3:
    // 0x80018484: lw          $t2, 0x28($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X28);
L_80018488:
    // 0x80018488: lui         $t5, 0x800B
    ctx->r13 = S32(0X800B << 16);
    // 0x8001848C: addiu       $t5, $t5, 0xC4
    ctx->r13 = ADD32(ctx->r13, 0XC4);
    // 0x80018490: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80018494: addiu       $t4, $t3, -0x8
    ctx->r12 = ADD32(ctx->r11, -0X8);
    // 0x80018498: addu        $v1, $t4, $t5
    ctx->r3 = ADD32(ctx->r12, ctx->r13);
    // 0x8001849C: lw          $t6, 0x4($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X4);
    // 0x800184A0: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x800184A4: subu        $t8, $t6, $t7
    ctx->r24 = SUB32(ctx->r14, ctx->r15);
    // 0x800184A8: b           L_800184B4
    // 0x800184AC: addu        $v0, $t8, $s0
    ctx->r2 = ADD32(ctx->r24, ctx->r16);
        goto L_800184B4;
    // 0x800184AC: addu        $v0, $t8, $s0
    ctx->r2 = ADD32(ctx->r24, ctx->r16);
L_800184B0:
    // 0x800184B0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800184B4:
    // 0x800184B4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800184B8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800184BC: addiu       $sp, $sp, 0x30
    ctx->r29 = ADD32(ctx->r29, 0X30);
    // 0x800184C0: jr          $ra
    // 0x800184C4: nop

    return;
    // 0x800184C4: nop

;}
RECOMP_FUNC void func_800184E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800184E0: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x800184E4: sw          $s0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r16;
    // 0x800184E8: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x800184EC: sw          $a1, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r5;
    // 0x800184F0: sw          $a2, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r6;
    // 0x800184F4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800184F8: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x800184FC: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x80018500: addiu       $t7, $zero, 0x3
    ctx->r15 = ADD32(0, 0X3);
    // 0x80018504: addiu       $t8, $zero, 0x0
    ctx->r24 = ADD32(0, 0X0);
    // 0x80018508: addiu       $t9, $zero, 0x0
    ctx->r25 = ADD32(0, 0X0);
    // 0x8001850C: addiu       $t0, $zero, 0x0
    ctx->r8 = ADD32(0, 0X0);
    // 0x80018510: addiu       $t1, $zero, 0x0
    ctx->r9 = ADD32(0, 0X0);
    // 0x80018514: addiu       $t2, $zero, 0x0
    ctx->r10 = ADD32(0, 0X0);
    // 0x80018518: addiu       $t3, $zero, 0x0
    ctx->r11 = ADD32(0, 0X0);
    // 0x8001851C: addiu       $t4, $zero, 0x0
    ctx->r12 = ADD32(0, 0X0);
    // 0x80018520: addiu       $t5, $zero, 0x0
    ctx->r13 = ADD32(0, 0X0);
    // 0x80018524: sw          $zero, 0x88C($a0)
    MEM_W(0X88C, ctx->r4) = 0;
    // 0x80018528: sw          $zero, 0x890($a0)
    MEM_W(0X890, ctx->r4) = 0;
    // 0x8001852C: sw          $zero, 0x894($a0)
    MEM_W(0X894, ctx->r4) = 0;
    // 0x80018530: sw          $zero, 0x888($a0)
    MEM_W(0X888, ctx->r4) = 0;
    // 0x80018534: sw          $zero, 0x89C($a0)
    MEM_W(0X89C, ctx->r4) = 0;
    // 0x80018538: sw          $zero, 0x898($a0)
    MEM_W(0X898, ctx->r4) = 0;
    // 0x8001853C: sw          $v0, 0x8A0($a0)
    MEM_W(0X8A0, ctx->r4) = ctx->r2;
    // 0x80018540: sw          $v0, 0x8A4($a0)
    MEM_W(0X8A4, ctx->r4) = ctx->r2;
    // 0x80018544: sh          $t6, 0x0($a0)
    MEM_H(0X0, ctx->r4) = ctx->r14;
    // 0x80018548: sh          $t7, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r15;
    // 0x8001854C: sw          $t9, 0x8AC($a0)
    MEM_W(0X8AC, ctx->r4) = ctx->r25;
    // 0x80018550: sw          $t8, 0x8A8($a0)
    MEM_W(0X8A8, ctx->r4) = ctx->r24;
    // 0x80018554: sw          $t1, 0x8B4($a0)
    MEM_W(0X8B4, ctx->r4) = ctx->r9;
    // 0x80018558: sw          $t0, 0x8B0($a0)
    MEM_W(0X8B0, ctx->r4) = ctx->r8;
    // 0x8001855C: sw          $t3, 0x8BC($a0)
    MEM_W(0X8BC, ctx->r4) = ctx->r11;
    // 0x80018560: sw          $t2, 0x8B8($a0)
    MEM_W(0X8B8, ctx->r4) = ctx->r10;
    // 0x80018564: sw          $t5, 0x8C4($a0)
    MEM_W(0X8C4, ctx->r4) = ctx->r13;
    // 0x80018568: sw          $t4, 0x8C0($a0)
    MEM_W(0X8C0, ctx->r4) = ctx->r12;
    // 0x8001856C: addiu       $a0, $a0, 0xAC
    ctx->r4 = ADD32(ctx->r4, 0XAC);
    // 0x80018570: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x80018574: addiu       $a1, $s0, 0xC4
    ctx->r5 = ADD32(ctx->r16, 0XC4);
    // 0x80018578: jal         0x800A1460
    // 0x8001857C: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_0;
    // 0x8001857C: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_0:
    // 0x80018580: addiu       $a0, $s0, 0xE4
    ctx->r4 = ADD32(ctx->r16, 0XE4);
    // 0x80018584: sw          $a0, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r4;
    // 0x80018588: addiu       $a1, $s0, 0xFC
    ctx->r5 = ADD32(ctx->r16, 0XFC);
    // 0x8001858C: jal         0x800A1460
    // 0x80018590: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_1;
    // 0x80018590: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_1:
    // 0x80018594: addiu       $a0, $s0, 0x11C
    ctx->r4 = ADD32(ctx->r16, 0X11C);
    // 0x80018598: sw          $a0, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r4;
    // 0x8001859C: addiu       $a1, $s0, 0x134
    ctx->r5 = ADD32(ctx->r16, 0X134);
    // 0x800185A0: jal         0x800A1460
    // 0x800185A4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_2;
    // 0x800185A4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_2:
    // 0x800185A8: addiu       $a0, $s0, 0x3C
    ctx->r4 = ADD32(ctx->r16, 0X3C);
    // 0x800185AC: addiu       $a1, $s0, 0x54
    ctx->r5 = ADD32(ctx->r16, 0X54);
    // 0x800185B0: jal         0x800A1460
    // 0x800185B4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_3;
    // 0x800185B4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_3:
    // 0x800185B8: addiu       $a0, $s0, 0x74
    ctx->r4 = ADD32(ctx->r16, 0X74);
    // 0x800185BC: addiu       $a1, $s0, 0x8C
    ctx->r5 = ADD32(ctx->r16, 0X8C);
    // 0x800185C0: jal         0x800A1460
    // 0x800185C4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_4;
    // 0x800185C4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_4:
    // 0x800185C8: addiu       $a0, $s0, 0x4
    ctx->r4 = ADD32(ctx->r16, 0X4);
    // 0x800185CC: addiu       $a1, $s0, 0x1C
    ctx->r5 = ADD32(ctx->r16, 0X1C);
    // 0x800185D0: jal         0x800A1460
    // 0x800185D4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_5;
    // 0x800185D4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_5:
    // 0x800185D8: addiu       $a0, $s0, 0x154
    ctx->r4 = ADD32(ctx->r16, 0X154);
    // 0x800185DC: addiu       $a1, $s0, 0x16C
    ctx->r5 = ADD32(ctx->r16, 0X16C);
    // 0x800185E0: jal         0x800A1460
    // 0x800185E4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_6;
    // 0x800185E4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_6:
    // 0x800185E8: addiu       $a0, $s0, 0x18C
    ctx->r4 = ADD32(ctx->r16, 0X18C);
    // 0x800185EC: addiu       $a1, $s0, 0x1A4
    ctx->r5 = ADD32(ctx->r16, 0X1A4);
    // 0x800185F0: jal         0x800A1460
    // 0x800185F4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_7;
    // 0x800185F4: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_7:
    // 0x800185F8: jal         0x800A66C0
    // 0x800185FC: addiu       $a0, $zero, 0xFE
    ctx->r4 = ADD32(0, 0XFE);
    osCreateViManager_recomp(rdram, ctx);
        goto after_8;
    // 0x800185FC: addiu       $a0, $zero, 0xFE
    ctx->r4 = ADD32(0, 0XFE);
    after_8:
    // 0x80018600: lbu         $t6, 0x3F($sp)
    ctx->r14 = MEM_BU(ctx->r29, 0X3F);
    // 0x80018604: lui         $t8, 0x800C
    ctx->r24 = S32(0X800C << 16);
    // 0x80018608: addiu       $t8, $t8, -0x5C80
    ctx->r24 = ADD32(ctx->r24, -0X5C80);
    // 0x8001860C: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80018610: addu        $t7, $t7, $t6
    ctx->r15 = ADD32(ctx->r15, ctx->r14);
    // 0x80018614: sll         $t7, $t7, 4
    ctx->r15 = S32(ctx->r15 << 4);
    // 0x80018618: jal         0x8009A5A0
    // 0x8001861C: addu        $a0, $t7, $t8
    ctx->r4 = ADD32(ctx->r15, ctx->r24);
    osViSetMode_recomp(rdram, ctx);
        goto after_9;
    // 0x8001861C: addu        $a0, $t7, $t8
    ctx->r4 = ADD32(ctx->r15, ctx->r24);
    after_9:
    // 0x80018620: jal         0x80099B80
    // 0x80018624: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    osViBlack_recomp(rdram, ctx);
        goto after_10;
    // 0x80018624: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_10:
    // 0x80018628: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    // 0x8001862C: addiu       $a1, $zero, 0x29A
    ctx->r5 = ADD32(0, 0X29A);
    // 0x80018630: jal         0x800A3A80
    // 0x80018634: lbu         $a2, 0x43($sp)
    ctx->r6 = MEM_BU(ctx->r29, 0X43);
    osViSetEvent_recomp(rdram, ctx);
        goto after_11;
    // 0x80018634: lbu         $a2, 0x43($sp)
    ctx->r6 = MEM_BU(ctx->r29, 0X43);
    after_11:
    // 0x80018638: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    // 0x8001863C: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    // 0x80018640: jal         0x800A0120
    // 0x80018644: addiu       $a2, $zero, 0x29B
    ctx->r6 = ADD32(0, 0X29B);
    osSetEventMesg_recomp(rdram, ctx);
        goto after_12;
    // 0x80018644: addiu       $a2, $zero, 0x29B
    ctx->r6 = ADD32(0, 0X29B);
    after_12:
    // 0x80018648: addiu       $a0, $zero, 0x9
    ctx->r4 = ADD32(0, 0X9);
    // 0x8001864C: lw          $a1, 0x2C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X2C);
    // 0x80018650: jal         0x800A0120
    // 0x80018654: addiu       $a2, $zero, 0x29C
    ctx->r6 = ADD32(0, 0X29C);
    osSetEventMesg_recomp(rdram, ctx);
        goto after_13;
    // 0x80018654: addiu       $a2, $zero, 0x29C
    ctx->r6 = ADD32(0, 0X29C);
    after_13:
    // 0x80018658: addiu       $a0, $zero, 0xE
    ctx->r4 = ADD32(0, 0XE);
    // 0x8001865C: lw          $a1, 0x34($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X34);
    // 0x80018660: jal         0x800A0120
    // 0x80018664: addiu       $a2, $zero, 0x29D
    ctx->r6 = ADD32(0, 0X29D);
    osSetEventMesg_recomp(rdram, ctx);
        goto after_14;
    // 0x80018664: addiu       $a2, $zero, 0x29D
    ctx->r6 = ADD32(0, 0X29D);
    after_14:
    // 0x80018668: lui         $t9, 0x800D
    ctx->r25 = S32(0X800D << 16);
    // 0x8001866C: addiu       $t9, $t9, -0x78B0
    ctx->r25 = ADD32(ctx->r25, -0X78B0);
    // 0x80018670: addiu       $a0, $s0, 0x1C8
    ctx->r4 = ADD32(ctx->r16, 0X1C8);
    // 0x80018674: lui         $a2, 0x8002
    ctx->r6 = S32(0X8002 << 16);
    // 0x80018678: addiu       $t0, $zero, 0x82
    ctx->r8 = ADD32(0, 0X82);
    // 0x8001867C: sw          $t0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r8;
    // 0x80018680: addiu       $a2, $a2, -0x788C
    ctx->r6 = ADD32(ctx->r6, -0X788C);
    // 0x80018684: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x80018688: sw          $t9, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r25;
    // 0x8001868C: addiu       $a1, $zero, 0x13
    ctx->r5 = ADD32(0, 0X13);
    // 0x80018690: jal         0x80098A00
    // 0x80018694: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    osCreateThread_recomp(rdram, ctx);
        goto after_15;
    // 0x80018694: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    after_15:
    // 0x80018698: jal         0x800A6E30
    // 0x8001869C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    osStartThread_recomp(rdram, ctx);
        goto after_16;
    // 0x8001869C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    after_16:
    // 0x800186A0: lui         $t1, 0x800D
    ctx->r9 = S32(0X800D << 16);
    // 0x800186A4: addiu       $t1, $t1, -0x58B0
    ctx->r9 = ADD32(ctx->r9, -0X58B0);
    // 0x800186A8: addiu       $a0, $s0, 0x378
    ctx->r4 = ADD32(ctx->r16, 0X378);
    // 0x800186AC: lui         $a2, 0x8002
    ctx->r6 = S32(0X8002 << 16);
    // 0x800186B0: addiu       $t2, $zero, 0x78
    ctx->r10 = ADD32(0, 0X78);
    // 0x800186B4: sw          $t2, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r10;
    // 0x800186B8: addiu       $a2, $a2, -0x7648
    ctx->r6 = ADD32(ctx->r6, -0X7648);
    // 0x800186BC: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x800186C0: sw          $t1, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r9;
    // 0x800186C4: addiu       $a1, $zero, 0x12
    ctx->r5 = ADD32(0, 0X12);
    // 0x800186C8: jal         0x80098A00
    // 0x800186CC: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    osCreateThread_recomp(rdram, ctx);
        goto after_17;
    // 0x800186CC: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    after_17:
    // 0x800186D0: jal         0x800A6E30
    // 0x800186D4: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    osStartThread_recomp(rdram, ctx);
        goto after_18;
    // 0x800186D4: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    after_18:
    // 0x800186D8: lui         $t3, 0x800D
    ctx->r11 = S32(0X800D << 16);
    // 0x800186DC: addiu       $t3, $t3, -0x38B0
    ctx->r11 = ADD32(ctx->r11, -0X38B0);
    // 0x800186E0: addiu       $a0, $s0, 0x528
    ctx->r4 = ADD32(ctx->r16, 0X528);
    // 0x800186E4: lui         $a2, 0x8002
    ctx->r6 = S32(0X8002 << 16);
    // 0x800186E8: addiu       $t4, $zero, 0x64
    ctx->r12 = ADD32(0, 0X64);
    // 0x800186EC: sw          $t4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r12;
    // 0x800186F0: addiu       $a2, $a2, -0x74B0
    ctx->r6 = ADD32(ctx->r6, -0X74B0);
    // 0x800186F4: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x800186F8: sw          $t3, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r11;
    // 0x800186FC: addiu       $a1, $zero, 0x11
    ctx->r5 = ADD32(0, 0X11);
    // 0x80018700: jal         0x80098A00
    // 0x80018704: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    osCreateThread_recomp(rdram, ctx);
        goto after_19;
    // 0x80018704: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    after_19:
    // 0x80018708: jal         0x800A6E30
    // 0x8001870C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    osStartThread_recomp(rdram, ctx);
        goto after_20;
    // 0x8001870C: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    after_20:
    // 0x80018710: lui         $t5, 0x800D
    ctx->r13 = S32(0X800D << 16);
    // 0x80018714: addiu       $t5, $t5, -0x18B0
    ctx->r13 = ADD32(ctx->r13, -0X18B0);
    // 0x80018718: addiu       $a0, $s0, 0x6D8
    ctx->r4 = ADD32(ctx->r16, 0X6D8);
    // 0x8001871C: lui         $a2, 0x8002
    ctx->r6 = S32(0X8002 << 16);
    // 0x80018720: addiu       $t6, $zero, 0x6E
    ctx->r14 = ADD32(0, 0X6E);
    // 0x80018724: sw          $t6, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r14;
    // 0x80018728: addiu       $a2, $a2, -0x72D8
    ctx->r6 = ADD32(ctx->r6, -0X72D8);
    // 0x8001872C: sw          $a0, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r4;
    // 0x80018730: sw          $t5, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r13;
    // 0x80018734: addiu       $a1, $zero, 0x10
    ctx->r5 = ADD32(0, 0X10);
    // 0x80018738: jal         0x80098A00
    // 0x8001873C: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    osCreateThread_recomp(rdram, ctx);
        goto after_21;
    // 0x8001873C: or          $a3, $s0, $zero
    ctx->r7 = ctx->r16 | 0;
    after_21:
    // 0x80018740: jal         0x800A6E30
    // 0x80018744: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    osStartThread_recomp(rdram, ctx);
        goto after_22;
    // 0x80018744: lw          $a0, 0x34($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X34);
    after_22:
    // 0x80018748: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x8001874C: lw          $s0, 0x20($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X20);
    // 0x80018750: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80018754: jr          $ra
    // 0x80018758: nop

    return;
    // 0x80018758: nop

;}
RECOMP_FUNC void func_8001875C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001875C: jr          $ra
    // 0x80018760: addiu       $v0, $a0, 0x4
    ctx->r2 = ADD32(ctx->r4, 0X4);
    return;
    // 0x80018760: addiu       $v0, $a0, 0x4
    ctx->r2 = ADD32(ctx->r4, 0X4);
;}
RECOMP_FUNC void func_80018764(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018764: jr          $ra
    // 0x80018768: addiu       $v0, $a0, 0x3C
    ctx->r2 = ADD32(ctx->r4, 0X3C);
    return;
    // 0x80018768: addiu       $v0, $a0, 0x3C
    ctx->r2 = ADD32(ctx->r4, 0X3C);
;}
RECOMP_FUNC void func_8001876C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001876C: jr          $ra
    // 0x80018770: lw          $v0, 0x898($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X898);
    return;
    // 0x80018770: lw          $v0, 0x898($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X898);
;}
RECOMP_FUNC void func_80018774(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018774: addiu       $sp, $sp, -0x50
    ctx->r29 = ADD32(ctx->r29, -0X50);
    // 0x80018778: sw          $s5, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r21;
    // 0x8001877C: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x80018780: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80018784: sw          $s7, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r23;
    // 0x80018788: sw          $s6, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r22;
    // 0x8001878C: sw          $s4, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r20;
    // 0x80018790: sw          $s3, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r19;
    // 0x80018794: sw          $s2, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r18;
    // 0x80018798: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x8001879C: lui         $s5, 0x800B
    ctx->r21 = S32(0X800B << 16);
    // 0x800187A0: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x800187A4: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x800187A8: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x800187AC: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x800187B0: sw          $zero, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = 0;
    // 0x800187B4: addiu       $s5, $s5, 0x88
    ctx->r21 = ADD32(ctx->r21, 0X88);
    // 0x800187B8: addiu       $s2, $sp, 0x4C
    ctx->r18 = ADD32(ctx->r29, 0X4C);
    // 0x800187BC: addiu       $s3, $zero, 0x29A
    ctx->r19 = ADD32(0, 0X29A);
    // 0x800187C0: addiu       $s4, $zero, 0x29D
    ctx->r20 = ADD32(0, 0X29D);
    // 0x800187C4: addiu       $s6, $zero, 0x1
    ctx->r22 = ADD32(0, 0X1);
    // 0x800187C8: addiu       $s7, $a0, 0xAC
    ctx->r23 = ADD32(ctx->r4, 0XAC);
    // 0x800187CC: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
L_800187D0:
    // 0x800187D0: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x800187D4: jal         0x80096CE0
    // 0x800187D8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_0;
    // 0x800187D8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x800187DC: lw          $t6, 0x4C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X4C);
    // 0x800187E0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x800187E4: beq         $t6, $s3, L_800187FC
    if (ctx->r14 == ctx->r19) {
        // 0x800187E8: nop
    
            goto L_800187FC;
    }
    // 0x800187E8: nop

    // 0x800187EC: beq         $t6, $s4, L_8001880C
    if (ctx->r14 == ctx->r20) {
        // 0x800187F0: addiu       $a0, $zero, 0xA
        ctx->r4 = ADD32(0, 0XA);
            goto L_8001880C;
    }
    // 0x800187F0: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    // 0x800187F4: b           L_800187D0
    // 0x800187F8: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
        goto L_800187D0;
    // 0x800187F8: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
L_800187FC:
    // 0x800187FC: jal         0x80018968
    // 0x80018800: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    func_80018968(rdram, ctx);
        goto after_1;
    // 0x80018800: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    after_1:
    // 0x80018804: b           L_800187D0
    // 0x80018808: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
        goto L_800187D0;
    // 0x80018808: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
L_8001880C:
    // 0x8001880C: jal         0x80091108
    // 0x80018810: addiu       $s0, $s1, 0x2
    ctx->r16 = ADD32(ctx->r17, 0X2);
    func_80091108(rdram, ctx);
        goto after_2;
    // 0x80018810: addiu       $s0, $s1, 0x2
    ctx->r16 = ADD32(ctx->r17, 0X2);
    after_2:
    // 0x80018814: jal         0x800A6F80
    // 0x80018818: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    osViSetYScale_recomp(rdram, ctx);
        goto after_3;
    // 0x80018818: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_3:
    // 0x8001881C: jal         0x800A70B0
    // 0x80018820: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    osViSetXScale_recomp(rdram, ctx);
        goto after_4;
    // 0x80018820: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_4:
    // 0x80018824: jal         0x80099B80
    // 0x80018828: andi        $a0, $s6, 0xFF
    ctx->r4 = ctx->r22 & 0XFF;
    osViBlack_recomp(rdram, ctx);
        goto after_5;
    // 0x80018828: andi        $a0, $s6, 0xFF
    ctx->r4 = ctx->r22 & 0XFF;
    after_5:
    // 0x8001882C: sh          $s6, 0x0($s5)
    MEM_H(0X0, ctx->r21) = ctx->r22;
    // 0x80018830: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80018834: jal         0x80018968
    // 0x80018838: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    func_80018968(rdram, ctx);
        goto after_6;
    // 0x80018838: or          $a1, $s0, $zero
    ctx->r5 = ctx->r16 | 0;
    after_6:
    // 0x8001883C: b           L_800187D0
    // 0x80018840: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
        goto L_800187D0;
    // 0x80018840: or          $a0, $s7, $zero
    ctx->r4 = ctx->r23 | 0;
    // 0x80018844: nop

    // 0x80018848: nop

    // 0x8001884C: nop

    // 0x80018850: nop

    // 0x80018854: nop

    // 0x80018858: nop

    // 0x8001885C: nop

    // 0x80018860: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80018864: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80018868: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x8001886C: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x80018870: lw          $s2, 0x24($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X24);
    // 0x80018874: lw          $s3, 0x28($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X28);
    // 0x80018878: lw          $s4, 0x2C($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X2C);
    // 0x8001887C: lw          $s5, 0x30($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X30);
    // 0x80018880: lw          $s6, 0x34($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X34);
    // 0x80018884: lw          $s7, 0x38($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X38);
    // 0x80018888: jr          $ra
    // 0x8001888C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
    return;
    // 0x8001888C: addiu       $sp, $sp, 0x50
    ctx->r29 = ADD32(ctx->r29, 0X50);
;}
RECOMP_FUNC void func_80018890(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018890: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80018894: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80018898: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001889C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x800188A0: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x800188A4: jal         0x80096E20
    // 0x800188A8: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    osSetIntMask_recomp(rdram, ctx);
        goto after_0;
    // 0x800188A8: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x800188AC: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x800188B0: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    // 0x800188B4: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x800188B8: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x800188BC: sw          $t6, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r14;
    // 0x800188C0: lw          $t7, 0x888($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X888);
    // 0x800188C4: sw          $t7, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r15;
    // 0x800188C8: jal         0x80096E20
    // 0x800188CC: sw          $a1, 0x888($v1)
    MEM_W(0X888, ctx->r3) = ctx->r5;
    osSetIntMask_recomp(rdram, ctx);
        goto after_1;
    // 0x800188CC: sw          $a1, 0x888($v1)
    MEM_W(0X888, ctx->r3) = ctx->r5;
    after_1:
    // 0x800188D0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800188D4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800188D8: jr          $ra
    // 0x800188DC: nop

    return;
    // 0x800188DC: nop

;}
RECOMP_FUNC void func_800188E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800188E0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800188E4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800188E8: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x800188EC: lw          $v1, 0x888($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X888);
    // 0x800188F0: sw          $zero, 0x18($sp)
    MEM_W(0X18, ctx->r29) = 0;
    // 0x800188F4: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x800188F8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x800188FC: jal         0x80096E20
    // 0x80018900: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    osSetIntMask_recomp(rdram, ctx);
        goto after_0;
    // 0x80018900: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_0:
    // 0x80018904: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80018908: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x8001890C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80018910: beq         $v1, $zero, L_80018950
    if (ctx->r3 == 0) {
        // 0x80018914: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80018950;
    }
    // 0x80018914: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
L_80018918:
    // 0x80018918: bnel        $v1, $a1, L_80018944
    if (ctx->r3 != ctx->r5) {
        // 0x8001891C: or          $a2, $v1, $zero
        ctx->r6 = ctx->r3 | 0;
            goto L_80018944;
    }
    goto skip_0;
    // 0x8001891C: or          $a2, $v1, $zero
    ctx->r6 = ctx->r3 | 0;
    skip_0:
    // 0x80018920: beq         $a2, $zero, L_80018934
    if (ctx->r6 == 0) {
        // 0x80018924: lw          $t9, 0x20($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X20);
            goto L_80018934;
    }
    // 0x80018924: lw          $t9, 0x20($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X20);
    // 0x80018928: lw          $t7, 0x0($a1)
    ctx->r15 = MEM_W(ctx->r5, 0X0);
    // 0x8001892C: b           L_80018950
    // 0x80018930: sw          $t7, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r15;
        goto L_80018950;
    // 0x80018930: sw          $t7, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r15;
L_80018934:
    // 0x80018934: lw          $t8, 0x0($a1)
    ctx->r24 = MEM_W(ctx->r5, 0X0);
    // 0x80018938: b           L_80018950
    // 0x8001893C: sw          $t8, 0x888($t9)
    MEM_W(0X888, ctx->r25) = ctx->r24;
        goto L_80018950;
    // 0x8001893C: sw          $t8, 0x888($t9)
    MEM_W(0X888, ctx->r25) = ctx->r24;
    // 0x80018940: or          $a2, $v1, $zero
    ctx->r6 = ctx->r3 | 0;
L_80018944:
    // 0x80018944: lw          $v1, 0x0($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X0);
    // 0x80018948: bne         $v1, $zero, L_80018918
    if (ctx->r3 != 0) {
        // 0x8001894C: nop
    
            goto L_80018918;
    }
    // 0x8001894C: nop

L_80018950:
    // 0x80018950: jal         0x80096E20
    // 0x80018954: nop

    osSetIntMask_recomp(rdram, ctx);
        goto after_1;
    // 0x80018954: nop

    after_1:
    // 0x80018958: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001895C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80018960: jr          $ra
    // 0x80018964: nop

    return;
    // 0x80018964: nop

;}
RECOMP_FUNC void func_80018968(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018968: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001896C: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80018970: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80018974: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80018978: lw          $s0, 0x888($a0)
    ctx->r16 = MEM_W(ctx->r4, 0X888);
    // 0x8001897C: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x80018980: beql        $s0, $zero, L_800189A8
    if (ctx->r16 == 0) {
        // 0x80018984: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_800189A8;
    }
    goto skip_0;
    // 0x80018984: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80018988: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
L_8001898C:
    // 0x8001898C: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80018990: jal         0x80096760
    // 0x80018994: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    osJamMesg_recomp(rdram, ctx);
        goto after_0;
    // 0x80018994: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_0:
    // 0x80018998: lw          $s0, 0x0($s0)
    ctx->r16 = MEM_W(ctx->r16, 0X0);
    // 0x8001899C: bnel        $s0, $zero, L_8001898C
    if (ctx->r16 != 0) {
        // 0x800189A0: lw          $a0, 0x4($s0)
        ctx->r4 = MEM_W(ctx->r16, 0X4);
            goto L_8001898C;
    }
    goto skip_1;
    // 0x800189A0: lw          $a0, 0x4($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X4);
    skip_1:
    // 0x800189A4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_800189A8:
    // 0x800189A8: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x800189AC: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x800189B0: jr          $ra
    // 0x800189B4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x800189B4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_800189B8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800189B8: addiu       $sp, $sp, -0x68
    ctx->r29 = ADD32(ctx->r29, -0X68);
    // 0x800189BC: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x800189C0: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x800189C4: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x800189C8: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x800189CC: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x800189D0: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x800189D4: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x800189D8: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x800189DC: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x800189E0: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x800189E4: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800189E8: sw          $zero, 0x64($sp)
    MEM_W(0X64, ctx->r29) = 0;
    // 0x800189EC: sw          $zero, 0x60($sp)
    MEM_W(0X60, ctx->r29) = 0;
    // 0x800189F0: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x800189F4: addiu       $s4, $sp, 0x64
    ctx->r20 = ADD32(ctx->r29, 0X64);
    // 0x800189F8: addiu       $s5, $a0, 0x4
    ctx->r21 = ADD32(ctx->r4, 0X4);
    // 0x800189FC: addiu       $s6, $sp, 0x60
    ctx->r22 = ADD32(ctx->r29, 0X60);
    // 0x80018A00: addiu       $s7, $zero, 0x1
    ctx->r23 = ADD32(0, 0X1);
    // 0x80018A04: addiu       $fp, $zero, 0x2
    ctx->r30 = ADD32(0, 0X2);
    // 0x80018A08: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
L_80018A0C:
    // 0x80018A0C: or          $a1, $s6, $zero
    ctx->r5 = ctx->r22 | 0;
    // 0x80018A10: jal         0x80096CE0
    // 0x80018A14: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_0;
    // 0x80018A14: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x80018A18: jal         0x800A69E0
    // 0x80018A1C: nop

    osWritebackDCacheAll_recomp(rdram, ctx);
        goto after_1;
    // 0x80018A1C: nop

    after_1:
    // 0x80018A20: lw          $s3, 0x88C($s2)
    ctx->r19 = MEM_W(ctx->r18, 0X88C);
    // 0x80018A24: beql        $s3, $zero, L_80018A60
    if (ctx->r19 == 0) {
        // 0x80018A28: lw          $t6, 0x60($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X60);
            goto L_80018A60;
    }
    goto skip_0;
    // 0x80018A28: lw          $t6, 0x60($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X60);
    skip_0:
    // 0x80018A2C: jal         0x80099C90
    // 0x80018A30: addiu       $s0, $s2, 0xE4
    ctx->r16 = ADD32(ctx->r18, 0XE4);
    osSpTaskYield_recomp(rdram, ctx);
        goto after_2;
    // 0x80018A30: addiu       $s0, $s2, 0xE4
    ctx->r16 = ADD32(ctx->r18, 0XE4);
    after_2:
    // 0x80018A34: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80018A38: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    // 0x80018A3C: jal         0x80096CE0
    // 0x80018A40: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_3;
    // 0x80018A40: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_3:
    // 0x80018A44: jal         0x800A23B0
    // 0x80018A48: addiu       $a0, $s3, 0x10
    ctx->r4 = ADD32(ctx->r19, 0X10);
    osSpTaskYielded_recomp(rdram, ctx);
        goto after_4;
    // 0x80018A48: addiu       $a0, $s3, 0x10
    ctx->r4 = ADD32(ctx->r19, 0X10);
    after_4:
    // 0x80018A4C: beq         $v0, $zero, L_80018A5C
    if (ctx->r2 == 0) {
        // 0x80018A50: or          $s1, $fp, $zero
        ctx->r17 = ctx->r30 | 0;
            goto L_80018A5C;
    }
    // 0x80018A50: or          $s1, $fp, $zero
    ctx->r17 = ctx->r30 | 0;
    // 0x80018A54: b           L_80018A5C
    // 0x80018A58: or          $s1, $s7, $zero
    ctx->r17 = ctx->r23 | 0;
        goto L_80018A5C;
    // 0x80018A58: or          $s1, $s7, $zero
    ctx->r17 = ctx->r23 | 0;
L_80018A5C:
    // 0x80018A5C: lw          $t6, 0x60($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X60);
L_80018A60:
    // 0x80018A60: addiu       $s0, $s2, 0xE4
    ctx->r16 = ADD32(ctx->r18, 0XE4);
    // 0x80018A64: sw          $t6, 0x890($s2)
    MEM_W(0X890, ctx->r18) = ctx->r14;
    // 0x80018A68: lw          $a0, 0x60($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X60);
    // 0x80018A6C: jal         0x8009755C
    // 0x80018A70: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    osSpTaskLoad_recomp(rdram, ctx);
        goto after_5;
    // 0x80018A70: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    after_5:
    // 0x80018A74: lw          $a0, 0x60($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X60);
    // 0x80018A78: jal         0x800976EC
    // 0x80018A7C: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    osSpTaskStartGo_recomp(rdram, ctx);
        goto after_6;
    // 0x80018A7C: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    after_6:
    // 0x80018A80: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80018A84: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    // 0x80018A88: jal         0x80096CE0
    // 0x80018A8C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_7;
    // 0x80018A8C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_7:
    // 0x80018A90: lw          $t7, 0x894($s2)
    ctx->r15 = MEM_W(ctx->r18, 0X894);
    // 0x80018A94: sw          $zero, 0x890($s2)
    MEM_W(0X890, ctx->r18) = 0;
    // 0x80018A98: addiu       $a0, $s2, 0x154
    ctx->r4 = ADD32(ctx->r18, 0X154);
    // 0x80018A9C: beq         $t7, $zero, L_80018AAC
    if (ctx->r15 == 0) {
        // 0x80018AA0: or          $a1, $s4, $zero
        ctx->r5 = ctx->r20 | 0;
            goto L_80018AAC;
    }
    // 0x80018AA0: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    // 0x80018AA4: jal         0x80096760
    // 0x80018AA8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osJamMesg_recomp(rdram, ctx);
        goto after_8;
    // 0x80018AA8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_8:
L_80018AAC:
    // 0x80018AAC: bne         $s1, $s7, L_80018AD0
    if (ctx->r17 != ctx->r23) {
        // 0x80018AB0: nop
    
            goto L_80018AD0;
    }
    // 0x80018AB0: nop

    // 0x80018AB4: addiu       $s0, $s3, 0x10
    ctx->r16 = ADD32(ctx->r19, 0X10);
    // 0x80018AB8: jal         0x8009755C
    // 0x80018ABC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    osSpTaskLoad_recomp(rdram, ctx);
        goto after_9;
    // 0x80018ABC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_9:
    // 0x80018AC0: jal         0x800976EC
    // 0x80018AC4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    osSpTaskStartGo_recomp(rdram, ctx);
        goto after_10;
    // 0x80018AC4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_10:
    // 0x80018AC8: b           L_80018AE8
    // 0x80018ACC: lw          $t8, 0x60($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X60);
        goto L_80018AE8;
    // 0x80018ACC: lw          $t8, 0x60($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X60);
L_80018AD0:
    // 0x80018AD0: bne         $s1, $fp, L_80018AE4
    if (ctx->r17 != ctx->r30) {
        // 0x80018AD4: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80018AE4;
    }
    // 0x80018AD4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80018AD8: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    // 0x80018ADC: jal         0x80096760
    // 0x80018AE0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osJamMesg_recomp(rdram, ctx);
        goto after_11;
    // 0x80018AE0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_11:
L_80018AE4:
    // 0x80018AE4: lw          $t8, 0x60($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X60);
L_80018AE8:
    // 0x80018AE8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x80018AEC: or          $s1, $zero, $zero
    ctx->r17 = 0 | 0;
    // 0x80018AF0: lw          $a0, 0x50($t8)
    ctx->r4 = MEM_W(ctx->r24, 0X50);
    // 0x80018AF4: jal         0x80096760
    // 0x80018AF8: lw          $a1, 0x54($t8)
    ctx->r5 = MEM_W(ctx->r24, 0X54);
    osJamMesg_recomp(rdram, ctx);
        goto after_12;
    // 0x80018AF8: lw          $a1, 0x54($t8)
    ctx->r5 = MEM_W(ctx->r24, 0X54);
    after_12:
    // 0x80018AFC: b           L_80018A0C
    // 0x80018B00: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
        goto L_80018A0C;
    // 0x80018B00: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x80018B04: nop

    // 0x80018B08: nop

    // 0x80018B0C: nop

    // 0x80018B10: nop

    // 0x80018B14: nop

    // 0x80018B18: nop

    // 0x80018B1C: nop

    // 0x80018B20: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
    // 0x80018B24: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80018B28: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80018B2C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80018B30: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80018B34: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80018B38: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80018B3C: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80018B40: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80018B44: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80018B48: jr          $ra
    // 0x80018B4C: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
    return;
    // 0x80018B4C: addiu       $sp, $sp, 0x68
    ctx->r29 = ADD32(ctx->r29, 0X68);
;}
RECOMP_FUNC void func_80018B50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018B50: addiu       $sp, $sp, -0x60
    ctx->r29 = ADD32(ctx->r29, -0X60);
    // 0x80018B54: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80018B58: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80018B5C: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80018B60: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80018B64: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80018B68: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80018B6C: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80018B70: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80018B74: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80018B78: sw          $zero, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = 0;
    // 0x80018B7C: addiu       $s2, $sp, 0x50
    ctx->r18 = ADD32(ctx->r29, 0X50);
    // 0x80018B80: addiu       $s3, $sp, 0x5C
    ctx->r19 = ADD32(ctx->r29, 0X5C);
    // 0x80018B84: addiu       $s5, $sp, 0x58
    ctx->r21 = ADD32(ctx->r29, 0X58);
    // 0x80018B88: addiu       $s6, $a0, 0x3C
    ctx->r22 = ADD32(ctx->r4, 0X3C);
    // 0x80018B8C: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
L_80018B90:
    // 0x80018B90: or          $a1, $s5, $zero
    ctx->r5 = ctx->r21 | 0;
    // 0x80018B94: jal         0x80096CE0
    // 0x80018B98: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_0;
    // 0x80018B98: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x80018B9C: jal         0x800A7320
    // 0x80018BA0: nop

    osViGetCurrentFramebuffer_recomp(rdram, ctx);
        goto after_1;
    // 0x80018BA0: nop

    after_1:
    // 0x80018BA4: lw          $t6, 0x58($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X58);
    // 0x80018BA8: lw          $t7, 0xC($t6)
    ctx->r15 = MEM_W(ctx->r14, 0XC);
    // 0x80018BAC: beql        $v0, $t7, L_80018BCC
    if (ctx->r2 == ctx->r15) {
        // 0x80018BB0: addiu       $s0, $s1, 0x154
        ctx->r16 = ADD32(ctx->r17, 0X154);
            goto L_80018BCC;
    }
    goto skip_0;
    // 0x80018BB0: addiu       $s0, $s1, 0x154
    ctx->r16 = ADD32(ctx->r17, 0X154);
    skip_0:
    // 0x80018BB4: jal         0x80099970
    // 0x80018BB8: nop

    osViGetNextFramebuffer_recomp(rdram, ctx);
        goto after_2;
    // 0x80018BB8: nop

    after_2:
    // 0x80018BBC: lw          $t8, 0x58($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X58);
    // 0x80018BC0: lw          $t9, 0xC($t8)
    ctx->r25 = MEM_W(ctx->r24, 0XC);
    // 0x80018BC4: bne         $v0, $t9, L_80018C28
    if (ctx->r2 != ctx->r25) {
        // 0x80018BC8: addiu       $s0, $s1, 0x154
        ctx->r16 = ADD32(ctx->r17, 0X154);
            goto L_80018C28;
    }
    // 0x80018BC8: addiu       $s0, $s1, 0x154
    ctx->r16 = ADD32(ctx->r17, 0X154);
L_80018BCC:
    // 0x80018BCC: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    // 0x80018BD0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80018BD4: jal         0x80018890
    // 0x80018BD8: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_80018890(rdram, ctx);
        goto after_3;
    // 0x80018BD8: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_3:
    // 0x80018BDC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80018BE0: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x80018BE4: jal         0x80096CE0
    // 0x80018BE8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_4;
    // 0x80018BE8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_4:
    // 0x80018BEC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80018BF0: jal         0x800188E0
    // 0x80018BF4: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_800188E0(rdram, ctx);
        goto after_5;
    // 0x80018BF4: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_5:
    // 0x80018BF8: jal         0x800A7320
    // 0x80018BFC: nop

    osViGetCurrentFramebuffer_recomp(rdram, ctx);
        goto after_6;
    // 0x80018BFC: nop

    after_6:
    // 0x80018C00: lw          $t0, 0x58($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X58);
    // 0x80018C04: lw          $t1, 0xC($t0)
    ctx->r9 = MEM_W(ctx->r8, 0XC);
    // 0x80018C08: beql        $v0, $t1, L_80018BCC
    if (ctx->r2 == ctx->r9) {
        // 0x80018C0C: addiu       $s0, $s1, 0x154
        ctx->r16 = ADD32(ctx->r17, 0X154);
            goto L_80018BCC;
    }
    goto skip_1;
    // 0x80018C0C: addiu       $s0, $s1, 0x154
    ctx->r16 = ADD32(ctx->r17, 0X154);
    skip_1:
    // 0x80018C10: jal         0x80099970
    // 0x80018C14: nop

    osViGetNextFramebuffer_recomp(rdram, ctx);
        goto after_7;
    // 0x80018C14: nop

    after_7:
    // 0x80018C18: lw          $t2, 0x58($sp)
    ctx->r10 = MEM_W(ctx->r29, 0X58);
    // 0x80018C1C: lw          $t3, 0xC($t2)
    ctx->r11 = MEM_W(ctx->r10, 0XC);
    // 0x80018C20: beql        $v0, $t3, L_80018BCC
    if (ctx->r2 == ctx->r11) {
        // 0x80018C24: addiu       $s0, $s1, 0x154
        ctx->r16 = ADD32(ctx->r17, 0X154);
            goto L_80018BCC;
    }
    goto skip_2;
    // 0x80018C24: addiu       $s0, $s1, 0x154
    ctx->r16 = ADD32(ctx->r17, 0X154);
    skip_2:
L_80018C28:
    // 0x80018C28: lw          $t4, 0x890($s1)
    ctx->r12 = MEM_W(ctx->r17, 0X890);
    // 0x80018C2C: lw          $t5, 0x58($sp)
    ctx->r13 = MEM_W(ctx->r29, 0X58);
    // 0x80018C30: addiu       $a0, $s1, 0x154
    ctx->r4 = ADD32(ctx->r17, 0X154);
    // 0x80018C34: beq         $t4, $zero, L_80018C4C
    if (ctx->r12 == 0) {
        // 0x80018C38: or          $a1, $s3, $zero
        ctx->r5 = ctx->r19 | 0;
            goto L_80018C4C;
    }
    // 0x80018C38: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x80018C3C: sw          $t5, 0x894($s1)
    MEM_W(0X894, ctx->r17) = ctx->r13;
    // 0x80018C40: jal         0x80096CE0
    // 0x80018C44: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_8;
    // 0x80018C44: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_8:
    // 0x80018C48: sw          $zero, 0x894($s1)
    MEM_W(0X894, ctx->r17) = 0;
L_80018C4C:
    // 0x80018C4C: lw          $t6, 0x58($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X58);
    // 0x80018C50: addiu       $s0, $s1, 0xE4
    ctx->r16 = ADD32(ctx->r17, 0XE4);
    // 0x80018C54: addiu       $s4, $s1, 0x11C
    ctx->r20 = ADD32(ctx->r17, 0X11C);
    // 0x80018C58: sw          $t6, 0x88C($s1)
    MEM_W(0X88C, ctx->r17) = ctx->r14;
    // 0x80018C5C: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x80018C60: jal         0x8009755C
    // 0x80018C64: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    osSpTaskLoad_recomp(rdram, ctx);
        goto after_9;
    // 0x80018C64: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    after_9:
    // 0x80018C68: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x80018C6C: jal         0x800976EC
    // 0x80018C70: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    osSpTaskStartGo_recomp(rdram, ctx);
        goto after_10;
    // 0x80018C70: addiu       $a0, $a0, 0x10
    ctx->r4 = ADD32(ctx->r4, 0X10);
    after_10:
    // 0x80018C74: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80018C78: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x80018C7C: jal         0x80096CE0
    // 0x80018C80: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_11;
    // 0x80018C80: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_11:
    // 0x80018C84: sw          $zero, 0x88C($s1)
    MEM_W(0X88C, ctx->r17) = 0;
    // 0x80018C88: or          $a0, $s4, $zero
    ctx->r4 = ctx->r20 | 0;
    // 0x80018C8C: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x80018C90: jal         0x80096CE0
    // 0x80018C94: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_12;
    // 0x80018C94: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_12:
    // 0x80018C98: lw          $t7, 0x8A0($s1)
    ctx->r15 = MEM_W(ctx->r17, 0X8A0);
    // 0x80018C9C: beql        $t7, $zero, L_80018CB4
    if (ctx->r15 == 0) {
        // 0x80018CA0: lw          $t8, 0x58($sp)
        ctx->r24 = MEM_W(ctx->r29, 0X58);
            goto L_80018CB4;
    }
    goto skip_3;
    // 0x80018CA0: lw          $t8, 0x58($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X58);
    skip_3:
    // 0x80018CA4: jal         0x80099B80
    // 0x80018CA8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    osViBlack_recomp(rdram, ctx);
        goto after_13;
    // 0x80018CA8: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    after_13:
    // 0x80018CAC: sw          $zero, 0x8A0($s1)
    MEM_W(0X8A0, ctx->r17) = 0;
    // 0x80018CB0: lw          $t8, 0x58($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X58);
L_80018CB4:
    // 0x80018CB4: addiu       $a0, $s1, 0x74
    ctx->r4 = ADD32(ctx->r17, 0X74);
    // 0x80018CB8: lw          $t9, 0x8($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X8);
    // 0x80018CBC: or          $a1, $t8, $zero
    ctx->r5 = ctx->r24 | 0;
    // 0x80018CC0: andi        $t0, $t9, 0x40
    ctx->r8 = ctx->r25 & 0X40;
    // 0x80018CC4: beql        $t0, $zero, L_80018CE4
    if (ctx->r8 == 0) {
        // 0x80018CC8: lw          $t3, 0x58($sp)
        ctx->r11 = MEM_W(ctx->r29, 0X58);
            goto L_80018CE4;
    }
    goto skip_4;
    // 0x80018CC8: lw          $t3, 0x58($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X58);
    skip_4:
    // 0x80018CCC: jal         0x80096760
    // 0x80018CD0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osJamMesg_recomp(rdram, ctx);
        goto after_14;
    // 0x80018CD0: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_14:
    // 0x80018CD4: lw          $t1, 0x89C($s1)
    ctx->r9 = MEM_W(ctx->r17, 0X89C);
    // 0x80018CD8: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x80018CDC: sw          $t2, 0x89C($s1)
    MEM_W(0X89C, ctx->r17) = ctx->r10;
    // 0x80018CE0: lw          $t3, 0x58($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X58);
L_80018CE4:
    // 0x80018CE4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    // 0x80018CE8: lw          $a0, 0x50($t3)
    ctx->r4 = MEM_W(ctx->r11, 0X50);
    // 0x80018CEC: jal         0x80096760
    // 0x80018CF0: lw          $a1, 0x54($t3)
    ctx->r5 = MEM_W(ctx->r11, 0X54);
    osJamMesg_recomp(rdram, ctx);
        goto after_15;
    // 0x80018CF0: lw          $a1, 0x54($t3)
    ctx->r5 = MEM_W(ctx->r11, 0X54);
    after_15:
    // 0x80018CF4: b           L_80018B90
    // 0x80018CF8: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
        goto L_80018B90;
    // 0x80018CF8: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80018CFC: nop

    // 0x80018D00: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80018D04: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80018D08: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80018D0C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80018D10: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80018D14: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80018D18: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80018D1C: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80018D20: jr          $ra
    // 0x80018D24: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
    return;
    // 0x80018D24: addiu       $sp, $sp, 0x60
    ctx->r29 = ADD32(ctx->r29, 0X60);
;}
RECOMP_FUNC void func_80018D28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018D28: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x80018D2C: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80018D30: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80018D34: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80018D38: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80018D3C: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80018D40: or          $s3, $a0, $zero
    ctx->r19 = ctx->r4 | 0;
    // 0x80018D44: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x80018D48: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80018D4C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80018D50: sw          $zero, 0x54($sp)
    MEM_W(0X54, ctx->r29) = 0;
    // 0x80018D54: addiu       $s2, $sp, 0x48
    ctx->r18 = ADD32(ctx->r29, 0X48);
    // 0x80018D58: addiu       $s4, $sp, 0x54
    ctx->r20 = ADD32(ctx->r29, 0X54);
    // 0x80018D5C: addiu       $s5, $sp, 0x50
    ctx->r21 = ADD32(ctx->r29, 0X50);
    // 0x80018D60: addiu       $s6, $a0, 0x74
    ctx->r22 = ADD32(ctx->r4, 0X74);
    // 0x80018D64: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
L_80018D68:
    // 0x80018D68: or          $a1, $s5, $zero
    ctx->r5 = ctx->r21 | 0;
    // 0x80018D6C: jal         0x80096CE0
    // 0x80018D70: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_0;
    // 0x80018D70: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x80018D74: lw          $t6, 0x8A4($s3)
    ctx->r14 = MEM_W(ctx->r19, 0X8A4);
    // 0x80018D78: bnel        $t6, $zero, L_80018DE8
    if (ctx->r14 != 0) {
        // 0x80018D7C: sw          $zero, 0x8A4($s3)
        MEM_W(0X8A4, ctx->r19) = 0;
            goto L_80018DE8;
    }
    goto skip_0;
    // 0x80018D7C: sw          $zero, 0x8A4($s3)
    MEM_W(0X8A4, ctx->r19) = 0;
    skip_0:
    // 0x80018D80: jal         0x800A7320
    // 0x80018D84: nop

    osViGetCurrentFramebuffer_recomp(rdram, ctx);
        goto after_1;
    // 0x80018D84: nop

    after_1:
    // 0x80018D88: jal         0x80099970
    // 0x80018D8C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    osViGetNextFramebuffer_recomp(rdram, ctx);
        goto after_2;
    // 0x80018D8C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_2:
    // 0x80018D90: beq         $v0, $s0, L_80018DE8
    if (ctx->r2 == ctx->r16) {
        // 0x80018D94: addiu       $s1, $s3, 0x18C
        ctx->r17 = ADD32(ctx->r19, 0X18C);
            goto L_80018DE8;
    }
    // 0x80018D94: addiu       $s1, $s3, 0x18C
    ctx->r17 = ADD32(ctx->r19, 0X18C);
    // 0x80018D98: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
L_80018D9C:
    // 0x80018D9C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    // 0x80018DA0: jal         0x80018890
    // 0x80018DA4: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    func_80018890(rdram, ctx);
        goto after_3;
    // 0x80018DA4: or          $a2, $s1, $zero
    ctx->r6 = ctx->r17 | 0;
    after_3:
    // 0x80018DA8: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80018DAC: or          $a1, $s4, $zero
    ctx->r5 = ctx->r20 | 0;
    // 0x80018DB0: jal         0x80096CE0
    // 0x80018DB4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_4;
    // 0x80018DB4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_4:
    // 0x80018DB8: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80018DBC: jal         0x800188E0
    // 0x80018DC0: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_800188E0(rdram, ctx);
        goto after_5;
    // 0x80018DC0: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_5:
    // 0x80018DC4: jal         0x800A7320
    // 0x80018DC8: nop

    osViGetCurrentFramebuffer_recomp(rdram, ctx);
        goto after_6;
    // 0x80018DC8: nop

    after_6:
    // 0x80018DCC: jal         0x80099970
    // 0x80018DD0: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    osViGetNextFramebuffer_recomp(rdram, ctx);
        goto after_7;
    // 0x80018DD0: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_7:
    // 0x80018DD4: bnel        $v0, $s0, L_80018D9C
    if (ctx->r2 != ctx->r16) {
        // 0x80018DD8: or          $a0, $s3, $zero
        ctx->r4 = ctx->r19 | 0;
            goto L_80018D9C;
    }
    goto skip_1;
    // 0x80018DD8: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    skip_1:
    // 0x80018DDC: b           L_80018DEC
    // 0x80018DE0: lw          $t7, 0x50($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X50);
        goto L_80018DEC;
    // 0x80018DE0: lw          $t7, 0x50($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X50);
    // 0x80018DE4: sw          $zero, 0x8A4($s3)
    MEM_W(0X8A4, ctx->r19) = 0;
L_80018DE8:
    // 0x80018DE8: lw          $t7, 0x50($sp)
    ctx->r15 = MEM_W(ctx->r29, 0X50);
L_80018DEC:
    // 0x80018DEC: jal         0x800A1C30
    // 0x80018DF0: lw          $a0, 0xC($t7)
    ctx->r4 = MEM_W(ctx->r15, 0XC);
    osViSwapBuffer_recomp(rdram, ctx);
        goto after_8;
    // 0x80018DF0: lw          $a0, 0xC($t7)
    ctx->r4 = MEM_W(ctx->r15, 0XC);
    after_8:
    // 0x80018DF4: b           L_80018D68
    // 0x80018DF8: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
        goto L_80018D68;
    // 0x80018DF8: or          $a0, $s6, $zero
    ctx->r4 = ctx->r22 | 0;
    // 0x80018DFC: nop

    // 0x80018E00: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x80018E04: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80018E08: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80018E0C: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80018E10: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80018E14: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80018E18: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80018E1C: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80018E20: jr          $ra
    // 0x80018E24: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x80018E24: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_80018E28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018E28: jr          $ra
    // 0x80018E2C: lw          $v0, 0x89C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X89C);
    return;
    // 0x80018E2C: lw          $v0, 0x89C($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X89C);
;}
RECOMP_FUNC void func_80018E30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018E30: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80018E34: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80018E38: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80018E3C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    // 0x80018E40: lw          $t7, 0x8($a2)
    ctx->r15 = MEM_W(ctx->r6, 0X8);
    // 0x80018E44: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x80018E48: andi        $t8, $t7, 0x40
    ctx->r24 = ctx->r15 & 0X40;
    // 0x80018E4C: beql        $t8, $zero, L_80018E94
    if (ctx->r24 == 0) {
        // 0x80018E50: lw          $a0, 0x1C($sp)
        ctx->r4 = MEM_W(ctx->r29, 0X1C);
            goto L_80018E94;
    }
    goto skip_0;
    // 0x80018E50: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    skip_0:
    // 0x80018E54: lw          $t9, 0x898($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X898);
    // 0x80018E58: addiu       $t0, $t9, 0x1
    ctx->r8 = ADD32(ctx->r25, 0X1);
    // 0x80018E5C: sltiu       $at, $t0, 0x3
    ctx->r1 = ctx->r8 < 0X3 ? 1 : 0;
    // 0x80018E60: bne         $at, $zero, L_80018E6C
    if (ctx->r1 != 0) {
        // 0x80018E64: sw          $t0, 0x898($a0)
        MEM_W(0X898, ctx->r4) = ctx->r8;
            goto L_80018E6C;
    }
    // 0x80018E64: sw          $t0, 0x898($a0)
    MEM_W(0X898, ctx->r4) = ctx->r8;
    // 0x80018E68: sw          $zero, 0x898($a0)
    MEM_W(0X898, ctx->r4) = 0;
L_80018E6C:
    // 0x80018E6C: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    // 0x80018E70: jal         0x80096E20
    // 0x80018E74: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    osSetIntMask_recomp(rdram, ctx);
        goto after_0;
    // 0x80018E74: sw          $a3, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r7;
    after_0:
    // 0x80018E78: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80018E7C: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80018E80: lw          $t2, 0x89C($a3)
    ctx->r10 = MEM_W(ctx->r7, 0X89C);
    // 0x80018E84: addiu       $t3, $t2, 0x1
    ctx->r11 = ADD32(ctx->r10, 0X1);
    // 0x80018E88: jal         0x80096E20
    // 0x80018E8C: sw          $t3, 0x89C($a3)
    MEM_W(0X89C, ctx->r7) = ctx->r11;
    osSetIntMask_recomp(rdram, ctx);
        goto after_1;
    // 0x80018E8C: sw          $t3, 0x89C($a3)
    MEM_W(0X89C, ctx->r7) = ctx->r11;
    after_1:
    // 0x80018E90: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
L_80018E94:
    // 0x80018E94: lw          $a1, 0x20($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X20);
    // 0x80018E98: jal         0x80096760
    // 0x80018E9C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osJamMesg_recomp(rdram, ctx);
        goto after_2;
    // 0x80018E9C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_2:
    // 0x80018EA0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80018EA4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80018EA8: jr          $ra
    // 0x80018EAC: nop

    return;
    // 0x80018EAC: nop

;}
RECOMP_FUNC void func_80018EB0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018EB0: lw          $t6, 0x89C($a0)
    ctx->r14 = MEM_W(ctx->r4, 0X89C);
    // 0x80018EB4: addiu       $t7, $t6, 0x2
    ctx->r15 = ADD32(ctx->r14, 0X2);
    // 0x80018EB8: jr          $ra
    // 0x80018EBC: sw          $t7, 0x89C($a0)
    MEM_W(0X89C, ctx->r4) = ctx->r15;
    return;
    // 0x80018EBC: sw          $t7, 0x89C($a0)
    MEM_W(0X89C, ctx->r4) = ctx->r15;
;}
RECOMP_FUNC void func_80018EC0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018EC0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x80018EC4: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x80018EC8: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x80018ECC: jr          $ra
    // 0x80018ED0: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    return;
    // 0x80018ED0: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
;}
RECOMP_FUNC void func_80018EE0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018EE0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80018EE4: sw          $fp, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r30;
    // 0x80018EE8: sw          $s6, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r22;
    // 0x80018EEC: sw          $s5, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r21;
    // 0x80018EF0: sw          $s3, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r19;
    // 0x80018EF4: sw          $s1, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r17;
    // 0x80018EF8: sw          $s7, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r23;
    // 0x80018EFC: sw          $s4, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r20;
    // 0x80018F00: sw          $s2, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r18;
    // 0x80018F04: sw          $s0, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r16;
    // 0x80018F08: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x80018F0C: lui         $s3, 0x800F
    ctx->r19 = S32(0X800F << 16);
    // 0x80018F10: lui         $s5, 0x800D
    ctx->r21 = S32(0X800D << 16);
    // 0x80018F14: lui         $s6, 0x800D
    ctx->r22 = S32(0X800D << 16);
    // 0x80018F18: lui         $fp, 0x800C
    ctx->r30 = S32(0X800C << 16);
    // 0x80018F1C: lui         $t5, 0x800B
    ctx->r13 = S32(0X800B << 16);
    // 0x80018F20: lui         $t4, 0x800B
    ctx->r12 = S32(0X800B << 16);
    // 0x80018F24: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x80018F28: lui         $a3, 0x800C
    ctx->r7 = S32(0X800C << 16);
    // 0x80018F2C: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80018F30: addiu       $a1, $a1, 0x5D70
    ctx->r5 = ADD32(ctx->r5, 0X5D70);
    // 0x80018F34: addiu       $a3, $a3, 0x5D78
    ctx->r7 = ADD32(ctx->r7, 0X5D78);
    // 0x80018F38: addiu       $t2, $t2, -0x7D40
    ctx->r10 = ADD32(ctx->r10, -0X7D40);
    // 0x80018F3C: addiu       $t4, $t4, -0x5260
    ctx->r12 = ADD32(ctx->r12, -0X5260);
    // 0x80018F40: addiu       $t5, $t5, -0x5190
    ctx->r13 = ADD32(ctx->r13, -0X5190);
    // 0x80018F44: addiu       $fp, $fp, 0x5D38
    ctx->r30 = ADD32(ctx->r30, 0X5D38);
    // 0x80018F48: addiu       $s6, $s6, -0x1890
    ctx->r22 = ADD32(ctx->r22, -0X1890);
    // 0x80018F4C: addiu       $s5, $s5, -0x890
    ctx->r21 = ADD32(ctx->r21, -0X890);
    // 0x80018F50: addiu       $s3, $s3, -0x890
    ctx->r19 = ADD32(ctx->r19, -0X890);
    // 0x80018F54: addiu       $s1, $s1, 0xE40
    ctx->r17 = ADD32(ctx->r17, 0XE40);
    // 0x80018F58: addiu       $s0, $zero, 0x1000
    ctx->r16 = ADD32(0, 0X1000);
    // 0x80018F5C: addiu       $s2, $zero, 0x800
    ctx->r18 = ADD32(0, 0X800);
    // 0x80018F60: addiu       $s4, $zero, 0x400
    ctx->r20 = ADD32(0, 0X400);
    // 0x80018F64: addiu       $s7, $zero, 0xC00
    ctx->r23 = ADD32(0, 0XC00);
    // 0x80018F68: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80018F6C: addiu       $t3, $zero, 0x1
    ctx->r11 = ADD32(0, 0X1);
    // 0x80018F70: addiu       $t1, $zero, 0x40
    ctx->r9 = ADD32(0, 0X40);
    // 0x80018F74: addiu       $t0, $zero, 0x58
    ctx->r8 = ADD32(0, 0X58);
    // 0x80018F78: addiu       $a2, $zero, 0x2
    ctx->r6 = ADD32(0, 0X2);
L_80018F7C:
    // 0x80018F7C: multu       $v0, $t0
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80018F80: sll         $t6, $v0, 1
    ctx->r14 = S32(ctx->r2 << 1);
    // 0x80018F84: sll         $t8, $v0, 2
    ctx->r24 = S32(ctx->r2 << 2);
    // 0x80018F88: addu        $v1, $a1, $t6
    ctx->r3 = ADD32(ctx->r5, ctx->r14);
    // 0x80018F8C: addu        $t9, $t2, $t8
    ctx->r25 = ADD32(ctx->r10, ctx->r24);
    // 0x80018F90: lw          $t6, 0x3C($t9)
    ctx->r14 = MEM_W(ctx->r25, 0X3C);
    // 0x80018F94: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x80018F98: andi        $v0, $v0, 0xFF
    ctx->r2 = ctx->r2 & 0XFF;
    // 0x80018F9C: slti        $at, $v0, 0x3
    ctx->r1 = SIGNED(ctx->r2) < 0X3 ? 1 : 0;
    // 0x80018FA0: sh          $a2, 0x0($v1)
    MEM_H(0X0, ctx->r3) = ctx->r6;
    // 0x80018FA4: mflo        $t7
    ctx->r15 = lo;
    // 0x80018FA8: addu        $a0, $a3, $t7
    ctx->r4 = ADD32(ctx->r7, ctx->r15);
    // 0x80018FAC: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
    // 0x80018FB0: sw          $zero, 0x4($a0)
    MEM_W(0X4, ctx->r4) = 0;
    // 0x80018FB4: sw          $t1, 0x8($a0)
    MEM_W(0X8, ctx->r4) = ctx->r9;
    // 0x80018FB8: sw          $t3, 0x10($a0)
    MEM_W(0X10, ctx->r4) = ctx->r11;
    // 0x80018FBC: sw          $zero, 0x14($a0)
    MEM_W(0X14, ctx->r4) = 0;
    // 0x80018FC0: sw          $t4, 0x18($a0)
    MEM_W(0X18, ctx->r4) = ctx->r12;
    // 0x80018FC4: sw          $zero, 0x1C($a0)
    MEM_W(0X1C, ctx->r4) = 0;
    // 0x80018FC8: sw          $t5, 0x20($a0)
    MEM_W(0X20, ctx->r4) = ctx->r13;
    // 0x80018FCC: sw          $s0, 0x24($a0)
    MEM_W(0X24, ctx->r4) = ctx->r16;
    // 0x80018FD0: sw          $s1, 0x28($a0)
    MEM_W(0X28, ctx->r4) = ctx->r17;
    // 0x80018FD4: sw          $s2, 0x2C($a0)
    MEM_W(0X2C, ctx->r4) = ctx->r18;
    // 0x80018FD8: sw          $s3, 0x30($a0)
    MEM_W(0X30, ctx->r4) = ctx->r19;
    // 0x80018FDC: sw          $s4, 0x34($a0)
    MEM_W(0X34, ctx->r4) = ctx->r20;
    // 0x80018FE0: sw          $s5, 0x38($a0)
    MEM_W(0X38, ctx->r4) = ctx->r21;
    // 0x80018FE4: sw          $zero, 0x3C($a0)
    MEM_W(0X3C, ctx->r4) = 0;
    // 0x80018FE8: sw          $zero, 0x40($a0)
    MEM_W(0X40, ctx->r4) = 0;
    // 0x80018FEC: sw          $zero, 0x44($a0)
    MEM_W(0X44, ctx->r4) = 0;
    // 0x80018FF0: sw          $s6, 0x48($a0)
    MEM_W(0X48, ctx->r4) = ctx->r22;
    // 0x80018FF4: sw          $s7, 0x4C($a0)
    MEM_W(0X4C, ctx->r4) = ctx->r23;
    // 0x80018FF8: sw          $fp, 0x50($a0)
    MEM_W(0X50, ctx->r4) = ctx->r30;
    // 0x80018FFC: sw          $v1, 0x54($a0)
    MEM_W(0X54, ctx->r4) = ctx->r3;
    // 0x80019000: bne         $at, $zero, L_80018F7C
    if (ctx->r1 != 0) {
        // 0x80019004: sw          $t6, 0xC($a0)
        MEM_W(0XC, ctx->r4) = ctx->r14;
            goto L_80018F7C;
    }
    // 0x80019004: sw          $t6, 0xC($a0)
    MEM_W(0XC, ctx->r4) = ctx->r14;
    // 0x80019008: lw          $s0, 0x4($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X4);
    // 0x8001900C: lw          $s1, 0x8($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X8);
    // 0x80019010: lw          $s2, 0xC($sp)
    ctx->r18 = MEM_W(ctx->r29, 0XC);
    // 0x80019014: lw          $s3, 0x10($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X10);
    // 0x80019018: lw          $s4, 0x14($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X14);
    // 0x8001901C: lw          $s5, 0x18($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X18);
    // 0x80019020: lw          $s6, 0x1C($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X1C);
    // 0x80019024: lw          $s7, 0x20($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X20);
    // 0x80019028: lw          $fp, 0x24($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X24);
    // 0x8001902C: jr          $ra
    // 0x80019030: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80019030: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80019034(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80019034: lui         $t0, 0x800D
    ctx->r8 = S32(0X800D << 16);
    // 0x80019038: addiu       $t0, $t0, -0x18A0
    ctx->r8 = ADD32(ctx->r8, -0X18A0);
    // 0x8001903C: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x80019040: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80019044: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80019048: addiu       $t6, $v0, 0x8
    ctx->r14 = ADD32(ctx->r2, 0X8);
    // 0x8001904C: sw          $t6, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r14;
    // 0x80019050: lui         $t7, 0xE900
    ctx->r15 = S32(0XE900 << 16);
    // 0x80019054: sw          $t7, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r15;
    // 0x80019058: sw          $zero, 0x4($v0)
    MEM_W(0X4, ctx->r2) = 0;
    // 0x8001905C: lw          $v0, 0x0($t0)
    ctx->r2 = MEM_W(ctx->r8, 0X0);
    // 0x80019060: lui         $t9, 0xDF00
    ctx->r25 = S32(0XDF00 << 16);
    // 0x80019064: addiu       $t8, $v0, 0x8
    ctx->r24 = ADD32(ctx->r2, 0X8);
    // 0x80019068: sw          $t8, 0x0($t0)
    MEM_W(0X0, ctx->r8) = ctx->r24;
    // 0x8001906C: sw          $zero, 0x4($v0)
    MEM_W(0X4, ctx->r2) = 0;
    // 0x80019070: jal         0x800A69E0
    // 0x80019074: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
    osWritebackDCacheAll_recomp(rdram, ctx);
        goto after_0;
    // 0x80019074: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
    after_0:
    // 0x80019078: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001907C: jal         0x8001876C
    // 0x80019080: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    func_8001876C(rdram, ctx);
        goto after_1;
    // 0x80019080: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    after_1:
    // 0x80019084: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80019088: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8001908C: lh          $t4, 0x26($v1)
    ctx->r12 = MEM_H(ctx->r3, 0X26);
    // 0x80019090: sll         $t2, $v0, 2
    ctx->r10 = S32(ctx->r2 << 2);
    // 0x80019094: subu        $t2, $t2, $v0
    ctx->r10 = SUB32(ctx->r10, ctx->r2);
    // 0x80019098: sll         $t2, $t2, 2
    ctx->r10 = S32(ctx->r10 << 2);
    // 0x8001909C: subu        $t2, $t2, $v0
    ctx->r10 = SUB32(ctx->r10, ctx->r2);
    // 0x800190A0: lui         $t3, 0x800C
    ctx->r11 = S32(0X800C << 16);
    // 0x800190A4: sll         $t5, $t4, 2
    ctx->r13 = S32(ctx->r12 << 2);
    // 0x800190A8: addu        $t6, $v1, $t5
    ctx->r14 = ADD32(ctx->r3, ctx->r13);
    // 0x800190AC: addiu       $t3, $t3, 0x5D78
    ctx->r11 = ADD32(ctx->r11, 0X5D78);
    // 0x800190B0: sll         $t2, $t2, 3
    ctx->r10 = S32(ctx->r10 << 3);
    // 0x800190B4: lw          $a3, 0x14($t6)
    ctx->r7 = MEM_W(ctx->r14, 0X14);
    // 0x800190B8: lui         $t0, 0x800D
    ctx->r8 = S32(0X800D << 16);
    // 0x800190BC: addu        $a2, $t2, $t3
    ctx->r6 = ADD32(ctx->r10, ctx->r11);
    // 0x800190C0: addiu       $t0, $t0, -0x18A0
    ctx->r8 = ADD32(ctx->r8, -0X18A0);
    // 0x800190C4: lw          $t7, 0x0($t0)
    ctx->r15 = MEM_W(ctx->r8, 0X0);
    // 0x800190C8: lui         $t1, 0x800D
    ctx->r9 = S32(0X800D << 16);
    // 0x800190CC: lui         $t3, 0x800B
    ctx->r11 = S32(0X800B << 16);
    // 0x800190D0: lui         $t4, 0x800B
    ctx->r12 = S32(0X800B << 16);
    // 0x800190D4: subu        $t8, $t7, $a3
    ctx->r24 = SUB32(ctx->r15, ctx->r7);
    // 0x800190D8: addiu       $t1, $t1, -0x890
    ctx->r9 = ADD32(ctx->r9, -0X890);
    // 0x800190DC: sra         $t9, $t8, 3
    ctx->r25 = S32(SIGNED(ctx->r24) >> 3);
    // 0x800190E0: addiu       $t4, $t4, -0x5260
    ctx->r12 = ADD32(ctx->r12, -0X5260);
    // 0x800190E4: addiu       $t3, $t3, -0x5190
    ctx->r11 = ADD32(ctx->r11, -0X5190);
    // 0x800190E8: lui         $at, 0x2
    ctx->r1 = S32(0X2 << 16);
    // 0x800190EC: sll         $t2, $t9, 3
    ctx->r10 = S32(ctx->r25 << 3);
    // 0x800190F0: subu        $t5, $t3, $t4
    ctx->r13 = SUB32(ctx->r11, ctx->r12);
    // 0x800190F4: addu        $t6, $t1, $at
    ctx->r14 = ADD32(ctx->r9, ctx->r1);
    // 0x800190F8: addiu       $t7, $zero, 0x40
    ctx->r15 = ADD32(0, 0X40);
    // 0x800190FC: sw          $t2, 0x44($a2)
    MEM_W(0X44, ctx->r6) = ctx->r10;
    // 0x80019100: sw          $t5, 0x1C($a2)
    MEM_W(0X1C, ctx->r6) = ctx->r13;
    // 0x80019104: sw          $t1, 0x38($a2)
    MEM_W(0X38, ctx->r6) = ctx->r9;
    // 0x80019108: sw          $t6, 0x3C($a2)
    MEM_W(0X3C, ctx->r6) = ctx->r14;
    // 0x8001910C: sw          $t7, 0x8($a2)
    MEM_W(0X8, ctx->r6) = ctx->r15;
    // 0x80019110: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80019114: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80019118: sw          $a3, 0x40($a2)
    MEM_W(0X40, ctx->r6) = ctx->r7;
    // 0x8001911C: lw          $a1, 0x6748($a1)
    ctx->r5 = MEM_W(ctx->r5, 0X6748);
    // 0x80019120: jal         0x80018E30
    // 0x80019124: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    func_80018E30(rdram, ctx);
        goto after_2;
    // 0x80019124: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    after_2:
    // 0x80019128: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001912C: jal         0x8001876C
    // 0x80019130: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    func_8001876C(rdram, ctx);
        goto after_3;
    // 0x80019130: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    after_3:
    // 0x80019134: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80019138: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8001913C: lh          $a0, 0x2C($v1)
    ctx->r4 = MEM_H(ctx->r3, 0X2C);
    // 0x80019140: lh          $a1, 0x24($v1)
    ctx->r5 = MEM_H(ctx->r3, 0X24);
    // 0x80019144: lh          $t8, 0x2E($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X2E);
    // 0x80019148: addiu       $t9, $a0, 0x1
    ctx->r25 = ADD32(ctx->r4, 0X1);
    // 0x8001914C: sh          $t9, 0x2C($v1)
    MEM_H(0X2C, ctx->r3) = ctx->r25;
    // 0x80019150: lh          $t2, 0x2C($v1)
    ctx->r10 = MEM_H(ctx->r3, 0X2C);
    // 0x80019154: sh          $v0, 0x78($v1)
    MEM_H(0X78, ctx->r3) = ctx->r2;
    // 0x80019158: sh          $a0, 0x2E($v1)
    MEM_H(0X2E, ctx->r3) = ctx->r4;
    // 0x8001915C: slt         $at, $t2, $a1
    ctx->r1 = SIGNED(ctx->r10) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x80019160: bne         $at, $zero, L_8001916C
    if (ctx->r1 != 0) {
        // 0x80019164: sh          $t8, 0x30($v1)
        MEM_H(0X30, ctx->r3) = ctx->r24;
            goto L_8001916C;
    }
    // 0x80019164: sh          $t8, 0x30($v1)
    MEM_H(0X30, ctx->r3) = ctx->r24;
    // 0x80019168: sh          $zero, 0x2C($v1)
    MEM_H(0X2C, ctx->r3) = 0;
L_8001916C:
    // 0x8001916C: lh          $t3, 0x26($v1)
    ctx->r11 = MEM_H(ctx->r3, 0X26);
    // 0x80019170: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x80019174: sh          $t4, 0x26($v1)
    MEM_H(0X26, ctx->r3) = ctx->r12;
    // 0x80019178: lh          $t5, 0x26($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X26);
    // 0x8001917C: slt         $at, $t5, $a1
    ctx->r1 = SIGNED(ctx->r13) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x80019180: bnel        $at, $zero, L_80019190
    if (ctx->r1 != 0) {
        // 0x80019184: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80019190;
    }
    goto skip_0;
    // 0x80019184: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80019188: sh          $zero, 0x26($v1)
    MEM_H(0X26, ctx->r3) = 0;
    // 0x8001918C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80019190:
    // 0x80019190: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80019194: jr          $ra
    // 0x80019198: nop

    return;
    // 0x80019198: nop

;}
RECOMP_FUNC void func_800191A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800191A0: lui         $a2, 0x800D
    ctx->r6 = S32(0X800D << 16);
    // 0x800191A4: addiu       $a2, $a2, -0x18A0
    ctx->r6 = ADD32(ctx->r6, -0X18A0);
    // 0x800191A8: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x800191AC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800191B0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800191B4: addiu       $t6, $v1, 0x8
    ctx->r14 = ADD32(ctx->r3, 0X8);
    // 0x800191B8: sw          $t6, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r14;
    // 0x800191BC: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x800191C0: lui         $t7, 0xE700
    ctx->r15 = S32(0XE700 << 16);
    // 0x800191C4: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x800191C8: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x800191CC: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x800191D0: lhu         $t9, 0x7A($a1)
    ctx->r25 = MEM_HU(ctx->r5, 0X7A);
    // 0x800191D4: lh          $t3, 0x4C($a1)
    ctx->r11 = MEM_H(ctx->r5, 0X4C);
    // 0x800191D8: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x800191DC: andi        $t0, $t9, 0x3
    ctx->r8 = ctx->r25 & 0X3;
    // 0x800191E0: sll         $t1, $t0, 19
    ctx->r9 = S32(ctx->r8 << 19);
    // 0x800191E4: lui         $at, 0xFF00
    ctx->r1 = S32(0XFF00 << 16);
    // 0x800191E8: addiu       $t4, $t3, -0x1
    ctx->r12 = ADD32(ctx->r11, -0X1);
    // 0x800191EC: andi        $t5, $t4, 0xFFF
    ctx->r13 = ctx->r12 & 0XFFF;
    // 0x800191F0: or          $t2, $t1, $at
    ctx->r10 = ctx->r9 | ctx->r1;
    // 0x800191F4: addiu       $t8, $v1, 0x8
    ctx->r24 = ADD32(ctx->r3, 0X8);
    // 0x800191F8: sw          $t8, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r24;
    // 0x800191FC: or          $t6, $t2, $t5
    ctx->r14 = ctx->r10 | ctx->r13;
    // 0x80019200: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80019204: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x80019208: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    // 0x8001920C: jal         0x8001876C
    // 0x80019210: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    func_8001876C(rdram, ctx);
        goto after_0;
    // 0x80019210: sw          $v1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r3;
    after_0:
    // 0x80019214: lui         $a1, 0x801D
    ctx->r5 = S32(0X801D << 16);
    // 0x80019218: addiu       $a1, $a1, -0x7D40
    ctx->r5 = ADD32(ctx->r5, -0X7D40);
    // 0x8001921C: sll         $t7, $v0, 2
    ctx->r15 = S32(ctx->r2 << 2);
    // 0x80019220: addu        $t8, $a1, $t7
    ctx->r24 = ADD32(ctx->r5, ctx->r15);
    // 0x80019224: lw          $t9, 0x3C($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X3C);
    // 0x80019228: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x8001922C: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80019230: lw          $a3, 0x18($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X18);
    // 0x80019234: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80019238: addu        $t0, $t9, $at
    ctx->r8 = ADD32(ctx->r25, ctx->r1);
    // 0x8001923C: sw          $t0, 0x4($a3)
    MEM_W(0X4, ctx->r7) = ctx->r8;
    // 0x80019240: lh          $t3, 0x50($a1)
    ctx->r11 = MEM_H(ctx->r5, 0X50);
    // 0x80019244: lh          $t8, 0x52($a1)
    ctx->r24 = MEM_H(ctx->r5, 0X52);
    // 0x80019248: lui         $a2, 0x800D
    ctx->r6 = S32(0X800D << 16);
    // 0x8001924C: mtc1        $t3, $f4
    ctx->f4.u32l = ctx->r11;
    // 0x80019250: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x80019254: addiu       $a2, $a2, -0x18A0
    ctx->r6 = ADD32(ctx->r6, -0X18A0);
    // 0x80019258: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x8001925C: lw          $v1, 0x0($a2)
    ctx->r3 = MEM_W(ctx->r6, 0X0);
    // 0x80019260: lui         $at, 0xED00
    ctx->r1 = S32(0XED00 << 16);
    // 0x80019264: addiu       $t1, $v1, 0x8
    ctx->r9 = ADD32(ctx->r3, 0X8);
    // 0x80019268: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8001926C: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80019270: sw          $t1, 0x0($a2)
    MEM_W(0X0, ctx->r6) = ctx->r9;
    // 0x80019274: mul.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80019278: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8001927C: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x80019280: mfc1        $t2, $f10
    ctx->r10 = (int32_t)ctx->f10.u32l;
    // 0x80019284: mfc1        $t0, $f6
    ctx->r8 = (int32_t)ctx->f6.u32l;
    // 0x80019288: andi        $t5, $t2, 0xFFF
    ctx->r13 = ctx->r10 & 0XFFF;
    // 0x8001928C: sll         $t6, $t5, 12
    ctx->r14 = S32(ctx->r13 << 12);
    // 0x80019290: or          $t7, $t6, $at
    ctx->r15 = ctx->r14 | ctx->r1;
    // 0x80019294: andi        $t1, $t0, 0xFFF
    ctx->r9 = ctx->r8 & 0XFFF;
    // 0x80019298: or          $t3, $t7, $t1
    ctx->r11 = ctx->r15 | ctx->r9;
    // 0x8001929C: sw          $t3, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r11;
    // 0x800192A0: lh          $t4, 0x54($a1)
    ctx->r12 = MEM_H(ctx->r5, 0X54);
    // 0x800192A4: lh          $t9, 0x56($a1)
    ctx->r25 = MEM_H(ctx->r5, 0X56);
    // 0x800192A8: mtc1        $t4, $f8
    ctx->f8.u32l = ctx->r12;
    // 0x800192AC: mtc1        $t9, $f4
    ctx->f4.u32l = ctx->r25;
    // 0x800192B0: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x800192B4: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800192B8: mul.s       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x800192BC: nop

    // 0x800192C0: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x800192C4: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x800192C8: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x800192CC: mfc1        $t5, $f18
    ctx->r13 = (int32_t)ctx->f18.u32l;
    // 0x800192D0: mfc1        $t7, $f10
    ctx->r15 = (int32_t)ctx->f10.u32l;
    // 0x800192D4: andi        $t6, $t5, 0xFFF
    ctx->r14 = ctx->r13 & 0XFFF;
    // 0x800192D8: sll         $t8, $t6, 12
    ctx->r24 = S32(ctx->r14 << 12);
    // 0x800192DC: andi        $t1, $t7, 0xFFF
    ctx->r9 = ctx->r15 & 0XFFF;
    // 0x800192E0: or          $t3, $t8, $t1
    ctx->r11 = ctx->r24 | ctx->r9;
    // 0x800192E4: sw          $t3, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r11;
    // 0x800192E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800192EC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800192F0: jr          $ra
    // 0x800192F4: nop

    return;
    // 0x800192F4: nop

;}
RECOMP_FUNC void func_800192F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800192F8: lui         $t1, 0x800D
    ctx->r9 = S32(0X800D << 16);
    // 0x800192FC: addiu       $t1, $t1, -0x18A0
    ctx->r9 = ADD32(ctx->r9, -0X18A0);
    // 0x80019300: lw          $v1, 0x0($t1)
    ctx->r3 = MEM_W(ctx->r9, 0X0);
    // 0x80019304: lui         $t7, 0xE300
    ctx->r15 = S32(0XE300 << 16);
    // 0x80019308: lui         $t2, 0x801D
    ctx->r10 = S32(0X801D << 16);
    // 0x8001930C: addiu       $t6, $v1, 0x8
    ctx->r14 = ADD32(ctx->r3, 0X8);
    // 0x80019310: sw          $t6, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r14;
    // 0x80019314: ori         $t7, $t7, 0xA01
    ctx->r15 = ctx->r15 | 0XA01;
    // 0x80019318: lui         $t8, 0x30
    ctx->r24 = S32(0X30 << 16);
    // 0x8001931C: addiu       $t2, $t2, -0x7D40
    ctx->r10 = ADD32(ctx->r10, -0X7D40);
    // 0x80019320: sw          $t8, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r24;
    // 0x80019324: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x80019328: lbu         $t9, 0x7C($t2)
    ctx->r25 = MEM_BU(ctx->r10, 0X7C);
    // 0x8001932C: lbu         $t5, 0x7D($t2)
    ctx->r13 = MEM_BU(ctx->r10, 0X7D);
    // 0x80019330: lw          $v1, 0x0($t1)
    ctx->r3 = MEM_W(ctx->r9, 0X0);
    // 0x80019334: sll         $t3, $t9, 8
    ctx->r11 = S32(ctx->r25 << 8);
    // 0x80019338: sll         $t6, $t5, 3
    ctx->r14 = S32(ctx->r13 << 3);
    // 0x8001933C: lbu         $t9, 0x7E($t2)
    ctx->r25 = MEM_BU(ctx->r10, 0X7E);
    // 0x80019340: andi        $t7, $t6, 0x7C0
    ctx->r15 = ctx->r14 & 0X7C0;
    // 0x80019344: andi        $t4, $t3, 0xF800
    ctx->r12 = ctx->r11 & 0XF800;
    // 0x80019348: or          $t8, $t4, $t7
    ctx->r24 = ctx->r12 | ctx->r15;
    // 0x8001934C: lbu         $t4, 0x7F($t2)
    ctx->r12 = MEM_BU(ctx->r10, 0X7F);
    // 0x80019350: sra         $t3, $t9, 2
    ctx->r11 = S32(SIGNED(ctx->r25) >> 2);
    // 0x80019354: andi        $t5, $t3, 0x3E
    ctx->r13 = ctx->r11 & 0X3E;
    // 0x80019358: or          $t6, $t8, $t5
    ctx->r14 = ctx->r24 | ctx->r13;
    // 0x8001935C: andi        $t7, $t4, 0x1
    ctx->r15 = ctx->r12 & 0X1;
    // 0x80019360: or          $a0, $t6, $t7
    ctx->r4 = ctx->r14 | ctx->r15;
    // 0x80019364: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x80019368: addiu       $t9, $v1, 0x8
    ctx->r25 = ADD32(ctx->r3, 0X8);
    // 0x8001936C: sw          $t9, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r25;
    // 0x80019370: sll         $t8, $a0, 16
    ctx->r24 = S32(ctx->r4 << 16);
    // 0x80019374: or          $t5, $t8, $a0
    ctx->r13 = ctx->r24 | ctx->r4;
    // 0x80019378: lui         $t3, 0xF700
    ctx->r11 = S32(0XF700 << 16);
    // 0x8001937C: sw          $t3, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r11;
    // 0x80019380: sw          $t5, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r13;
    // 0x80019384: lw          $v1, 0x0($t1)
    ctx->r3 = MEM_W(ctx->r9, 0X0);
    // 0x80019388: lh          $t6, 0x54($t2)
    ctx->r14 = MEM_H(ctx->r10, 0X54);
    // 0x8001938C: lh          $t8, 0x56($t2)
    ctx->r24 = MEM_H(ctx->r10, 0X56);
    // 0x80019390: addiu       $t4, $v1, 0x8
    ctx->r12 = ADD32(ctx->r3, 0X8);
    // 0x80019394: andi        $t7, $t6, 0x3FF
    ctx->r15 = ctx->r14 & 0X3FF;
    // 0x80019398: sll         $t9, $t7, 14
    ctx->r25 = S32(ctx->r15 << 14);
    // 0x8001939C: sw          $t4, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r12;
    // 0x800193A0: lui         $at, 0xF600
    ctx->r1 = S32(0XF600 << 16);
    // 0x800193A4: andi        $t5, $t8, 0x3FF
    ctx->r13 = ctx->r24 & 0X3FF;
    // 0x800193A8: sll         $t4, $t5, 2
    ctx->r12 = S32(ctx->r13 << 2);
    // 0x800193AC: or          $t3, $t9, $at
    ctx->r11 = ctx->r25 | ctx->r1;
    // 0x800193B0: or          $t6, $t3, $t4
    ctx->r14 = ctx->r11 | ctx->r12;
    // 0x800193B4: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x800193B8: lh          $t5, 0x52($t2)
    ctx->r13 = MEM_H(ctx->r10, 0X52);
    // 0x800193BC: lh          $t7, 0x50($t2)
    ctx->r15 = MEM_H(ctx->r10, 0X50);
    // 0x800193C0: andi        $t3, $t5, 0x3FF
    ctx->r11 = ctx->r13 & 0X3FF;
    // 0x800193C4: andi        $t9, $t7, 0x3FF
    ctx->r25 = ctx->r15 & 0X3FF;
    // 0x800193C8: sll         $t8, $t9, 14
    ctx->r24 = S32(ctx->r25 << 14);
    // 0x800193CC: sll         $t4, $t3, 2
    ctx->r12 = S32(ctx->r11 << 2);
    // 0x800193D0: or          $t6, $t8, $t4
    ctx->r14 = ctx->r24 | ctx->r12;
    // 0x800193D4: sw          $t6, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r14;
    // 0x800193D8: lw          $v1, 0x0($t1)
    ctx->r3 = MEM_W(ctx->r9, 0X0);
    // 0x800193DC: lui         $t9, 0xE700
    ctx->r25 = S32(0XE700 << 16);
    // 0x800193E0: addiu       $t7, $v1, 0x8
    ctx->r15 = ADD32(ctx->r3, 0X8);
    // 0x800193E4: sw          $t7, 0x0($t1)
    MEM_W(0X0, ctx->r9) = ctx->r15;
    // 0x800193E8: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x800193EC: jr          $ra
    // 0x800193F0: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    return;
    // 0x800193F0: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
;}
RECOMP_FUNC void func_800193F4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800193F4: lui         $t4, 0x800D
    ctx->r12 = S32(0X800D << 16);
    // 0x800193F8: addiu       $t4, $t4, -0x18A0
    ctx->r12 = ADD32(ctx->r12, -0X18A0);
    // 0x800193FC: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x80019400: addiu       $sp, $sp, -0x8
    ctx->r29 = ADD32(ctx->r29, -0X8);
    // 0x80019404: sw          $s0, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r16;
    // 0x80019408: addiu       $t6, $v1, 0x8
    ctx->r14 = ADD32(ctx->r3, 0X8);
    // 0x8001940C: sw          $t6, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r14;
    // 0x80019410: lui         $t5, 0xE700
    ctx->r13 = S32(0XE700 << 16);
    // 0x80019414: sw          $t5, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r13;
    // 0x80019418: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x8001941C: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x80019420: lui         $t8, 0xE300
    ctx->r24 = S32(0XE300 << 16);
    // 0x80019424: ori         $t8, $t8, 0xA01
    ctx->r24 = ctx->r24 | 0XA01;
    // 0x80019428: addiu       $t7, $v1, 0x8
    ctx->r15 = ADD32(ctx->r3, 0X8);
    // 0x8001942C: sw          $t7, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r15;
    // 0x80019430: lui         $t9, 0x30
    ctx->r25 = S32(0X30 << 16);
    // 0x80019434: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80019438: sw          $t9, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r25;
    // 0x8001943C: sw          $t8, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r24;
    // 0x80019440: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x80019444: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80019448: lh          $t7, 0x4C($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X4C);
    // 0x8001944C: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x80019450: addiu       $t6, $v1, 0x8
    ctx->r14 = ADD32(ctx->r3, 0X8);
    // 0x80019454: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80019458: sw          $t6, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r14;
    // 0x8001945C: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x80019460: andi        $t9, $t8, 0xFFF
    ctx->r25 = ctx->r24 & 0XFFF;
    // 0x80019464: lui         $at, 0xFF10
    ctx->r1 = S32(0XFF10 << 16);
    // 0x80019468: lui         $a2, 0x1E
    ctx->r6 = S32(0X1E << 16);
    // 0x8001946C: addiu       $a2, $a2, -0x5800
    ctx->r6 = ADD32(ctx->r6, -0X5800);
    // 0x80019470: or          $t6, $t9, $at
    ctx->r14 = ctx->r25 | ctx->r1;
    // 0x80019474: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x80019478: sw          $a2, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r6;
    // 0x8001947C: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x80019480: lui         $t8, 0xFE00
    ctx->r24 = S32(0XFE00 << 16);
    // 0x80019484: lui         $at, 0xED00
    ctx->r1 = S32(0XED00 << 16);
    // 0x80019488: addiu       $t7, $v1, 0x8
    ctx->r15 = ADD32(ctx->r3, 0X8);
    // 0x8001948C: sw          $t7, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r15;
    // 0x80019490: sw          $a2, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r6;
    // 0x80019494: sw          $t8, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r24;
    // 0x80019498: lh          $t6, 0x50($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X50);
    // 0x8001949C: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x800194A0: mtc1        $t6, $f4
    ctx->f4.u32l = ctx->r14;
    // 0x800194A4: addiu       $t9, $v1, 0x8
    ctx->r25 = ADD32(ctx->r3, 0X8);
    // 0x800194A8: sw          $t9, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r25;
    // 0x800194AC: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x800194B0: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x800194B4: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x800194B8: mfc1        $t8, $f10
    ctx->r24 = (int32_t)ctx->f10.u32l;
    // 0x800194BC: nop

    // 0x800194C0: andi        $t9, $t8, 0xFFF
    ctx->r25 = ctx->r24 & 0XFFF;
    // 0x800194C4: lh          $t8, 0x52($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X52);
    // 0x800194C8: sll         $t6, $t9, 12
    ctx->r14 = S32(ctx->r25 << 12);
    // 0x800194CC: or          $t7, $t6, $at
    ctx->r15 = ctx->r14 | ctx->r1;
    // 0x800194D0: mtc1        $t8, $f16
    ctx->f16.u32l = ctx->r24;
    // 0x800194D4: lui         $at, 0xF600
    ctx->r1 = S32(0XF600 << 16);
    // 0x800194D8: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x800194DC: mul.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x800194E0: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x800194E4: mfc1        $t6, $f6
    ctx->r14 = (int32_t)ctx->f6.u32l;
    // 0x800194E8: nop

    // 0x800194EC: andi        $t8, $t6, 0xFFF
    ctx->r24 = ctx->r14 & 0XFFF;
    // 0x800194F0: or          $t9, $t7, $t8
    ctx->r25 = ctx->r15 | ctx->r24;
    // 0x800194F4: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x800194F8: lh          $t6, 0x54($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X54);
    // 0x800194FC: lh          $t7, 0x56($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X56);
    // 0x80019500: mtc1        $t6, $f8
    ctx->f8.u32l = ctx->r14;
    // 0x80019504: mtc1        $t7, $f4
    ctx->f4.u32l = ctx->r15;
    // 0x80019508: cvt.s.w     $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.fl = CVT_S_W(ctx->f8.u32l);
    // 0x8001950C: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80019510: mul.s       $f16, $f10, $f0
    CHECK_FR(ctx, 16);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f16.fl = MUL_S(ctx->f10.fl, ctx->f0.fl);
    // 0x80019514: trunc.w.s   $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.u32l = TRUNC_W_S(ctx->f16.fl);
    // 0x80019518: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x8001951C: mfc1        $t8, $f18
    ctx->r24 = (int32_t)ctx->f18.u32l;
    // 0x80019520: nop

    // 0x80019524: andi        $t9, $t8, 0xFFF
    ctx->r25 = ctx->r24 & 0XFFF;
    // 0x80019528: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8001952C: sll         $t6, $t9, 12
    ctx->r14 = S32(ctx->r25 << 12);
    // 0x80019530: mfc1        $t9, $f10
    ctx->r25 = (int32_t)ctx->f10.u32l;
    // 0x80019534: nop

    // 0x80019538: andi        $t7, $t9, 0xFFF
    ctx->r15 = ctx->r25 & 0XFFF;
    // 0x8001953C: or          $t8, $t6, $t7
    ctx->r24 = ctx->r14 | ctx->r15;
    // 0x80019540: sw          $t8, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r24;
    // 0x80019544: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x80019548: lui         $t7, 0xFFFC
    ctx->r15 = S32(0XFFFC << 16);
    // 0x8001954C: ori         $t7, $t7, 0xFFFC
    ctx->r15 = ctx->r15 | 0XFFFC;
    // 0x80019550: addiu       $t9, $v1, 0x8
    ctx->r25 = ADD32(ctx->r3, 0X8);
    // 0x80019554: sw          $t9, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r25;
    // 0x80019558: lui         $t6, 0xF700
    ctx->r14 = S32(0XF700 << 16);
    // 0x8001955C: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x80019560: sw          $t7, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r15;
    // 0x80019564: lh          $t9, 0x54($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X54);
    // 0x80019568: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x8001956C: andi        $t6, $t9, 0x3FF
    ctx->r14 = ctx->r25 & 0X3FF;
    // 0x80019570: lh          $t9, 0x56($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X56);
    // 0x80019574: sll         $t7, $t6, 14
    ctx->r15 = S32(ctx->r14 << 14);
    // 0x80019578: addiu       $t8, $v1, 0x8
    ctx->r24 = ADD32(ctx->r3, 0X8);
    // 0x8001957C: sw          $t8, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r24;
    // 0x80019580: or          $t8, $t7, $at
    ctx->r24 = ctx->r15 | ctx->r1;
    // 0x80019584: andi        $t6, $t9, 0x3FF
    ctx->r14 = ctx->r25 & 0X3FF;
    // 0x80019588: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x8001958C: or          $t9, $t8, $t7
    ctx->r25 = ctx->r24 | ctx->r15;
    // 0x80019590: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x80019594: lh          $t6, 0x50($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X50);
    // 0x80019598: lh          $t9, 0x52($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X52);
    // 0x8001959C: andi        $t8, $t6, 0x3FF
    ctx->r24 = ctx->r14 & 0X3FF;
    // 0x800195A0: sll         $t7, $t8, 14
    ctx->r15 = S32(ctx->r24 << 14);
    // 0x800195A4: andi        $t6, $t9, 0x3FF
    ctx->r14 = ctx->r25 & 0X3FF;
    // 0x800195A8: sll         $t8, $t6, 2
    ctx->r24 = S32(ctx->r14 << 2);
    // 0x800195AC: or          $t9, $t7, $t8
    ctx->r25 = ctx->r15 | ctx->r24;
    // 0x800195B0: sw          $t9, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r25;
    // 0x800195B4: lw          $v1, 0x0($t4)
    ctx->r3 = MEM_W(ctx->r12, 0X0);
    // 0x800195B8: addiu       $t6, $v1, 0x8
    ctx->r14 = ADD32(ctx->r3, 0X8);
    // 0x800195BC: sw          $t6, 0x0($t4)
    MEM_W(0X0, ctx->r12) = ctx->r14;
    // 0x800195C0: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x800195C4: sw          $t5, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r13;
    // 0x800195C8: lw          $s0, 0x4($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X4);
    // 0x800195CC: jr          $ra
    // 0x800195D0: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
    return;
    // 0x800195D0: addiu       $sp, $sp, 0x8
    ctx->r29 = ADD32(ctx->r29, 0X8);
;}
RECOMP_FUNC void func_800195E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800195E0: addiu       $sp, $sp, -0x40
    ctx->r29 = ADD32(ctx->r29, -0X40);
    // 0x800195E4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800195E8: addiu       $a0, $sp, 0x28
    ctx->r4 = ADD32(ctx->r29, 0X28);
    // 0x800195EC: addiu       $a1, $sp, 0x24
    ctx->r5 = ADD32(ctx->r29, 0X24);
    // 0x800195F0: jal         0x800A1460
    // 0x800195F4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_0;
    // 0x800195F4: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_0:
    // 0x800195F8: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    // 0x800195FC: addiu       $a1, $sp, 0x28
    ctx->r5 = ADD32(ctx->r29, 0X28);
    // 0x80019600: jal         0x800A0120
    // 0x80019604: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osSetEventMesg_recomp(rdram, ctx);
        goto after_1;
    // 0x80019604: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_1:
    // 0x80019608: lui         $a3, 0x800F
    ctx->r7 = S32(0X800F << 16);
    // 0x8001960C: addiu       $a2, $a3, -0x470
    ctx->r6 = ADD32(ctx->r7, -0X470);
    // 0x80019610: addiu       $a0, $sp, 0x28
    ctx->r4 = ADD32(ctx->r29, 0X28);
    // 0x80019614: jal         0x80098600
    // 0x80019618: addiu       $a1, $sp, 0x21
    ctx->r5 = ADD32(ctx->r29, 0X21);
    osContStartReadData_recomp(rdram, ctx);
        goto after_2;
    // 0x80019618: addiu       $a1, $sp, 0x21
    ctx->r5 = ADD32(ctx->r29, 0X21);
    after_2:
    // 0x8001961C: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80019620: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80019624: addiu       $a1, $a1, -0x478
    ctx->r5 = ADD32(ctx->r5, -0X478);
    // 0x80019628: addiu       $a0, $a0, -0x490
    ctx->r4 = ADD32(ctx->r4, -0X490);
    // 0x8001962C: jal         0x800A1460
    // 0x80019630: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_3;
    // 0x80019630: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_3:
    // 0x80019634: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80019638: addiu       $a1, $a1, -0x490
    ctx->r5 = ADD32(ctx->r5, -0X490);
    // 0x8001963C: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    // 0x80019640: jal         0x800A0120
    // 0x80019644: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    osSetEventMesg_recomp(rdram, ctx);
        goto after_4;
    // 0x80019644: or          $a2, $zero, $zero
    ctx->r6 = 0 | 0;
    after_4:
    // 0x80019648: lui         $a3, 0x800F
    ctx->r7 = S32(0X800F << 16);
    // 0x8001964C: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80019650: addiu       $a3, $a3, -0x470
    ctx->r7 = ADD32(ctx->r7, -0X470);
    // 0x80019654: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x80019658: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001965C: lbu         $a2, 0x21($sp)
    ctx->r6 = MEM_BU(ctx->r29, 0X21);
    // 0x80019660: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80019664: addiu       $a0, $zero, 0xE
    ctx->r4 = ADD32(0, 0XE);
    // 0x80019668: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
L_8001966C:
    // 0x8001966C: sllv        $t7, $t6, $v0
    ctx->r15 = S32(ctx->r14 << (ctx->r2 & 31));
    // 0x80019670: and         $t8, $a2, $t7
    ctx->r24 = ctx->r6 & ctx->r15;
    // 0x80019674: beq         $t8, $zero, L_800196A4
    if (ctx->r24 == 0) {
        // 0x80019678: sll         $t9, $v0, 2
        ctx->r25 = S32(ctx->r2 << 2);
            goto L_800196A4;
    }
    // 0x80019678: sll         $t9, $v0, 2
    ctx->r25 = S32(ctx->r2 << 2);
    // 0x8001967C: addu        $t0, $a3, $t9
    ctx->r8 = ADD32(ctx->r7, ctx->r25);
    // 0x80019680: lbu         $t1, 0x3($t0)
    ctx->r9 = MEM_BU(ctx->r8, 0X3);
    // 0x80019684: andi        $t2, $t1, 0x8
    ctx->r10 = ctx->r9 & 0X8;
    // 0x80019688: bne         $t2, $zero, L_800196A4
    if (ctx->r10 != 0) {
        // 0x8001968C: nop
    
            goto L_800196A4;
    }
    // 0x8001968C: nop

    // 0x80019690: multu       $v0, $a0
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r4)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80019694: mflo        $t3
    ctx->r11 = lo;
    // 0x80019698: addu        $t4, $v1, $t3
    ctx->r12 = ADD32(ctx->r3, ctx->r11);
    // 0x8001969C: b           L_800196B4
    // 0x800196A0: sh          $a1, 0x534($t4)
    MEM_H(0X534, ctx->r12) = ctx->r5;
        goto L_800196B4;
    // 0x800196A0: sh          $a1, 0x534($t4)
    MEM_H(0X534, ctx->r12) = ctx->r5;
L_800196A4:
    // 0x800196A4: multu       $v0, $a0
    result = U64(U32(ctx->r2)) * U64(U32(ctx->r4)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x800196A8: mflo        $t5
    ctx->r13 = lo;
    // 0x800196AC: addu        $t6, $v1, $t5
    ctx->r14 = ADD32(ctx->r3, ctx->r13);
    // 0x800196B0: sh          $zero, 0x534($t6)
    MEM_H(0X534, ctx->r14) = 0;
L_800196B4:
    // 0x800196B4: addiu       $v0, $v0, 0x1
    ctx->r2 = ADD32(ctx->r2, 0X1);
    // 0x800196B8: sll         $v0, $v0, 16
    ctx->r2 = S32(ctx->r2 << 16);
    // 0x800196BC: sra         $v0, $v0, 16
    ctx->r2 = S32(SIGNED(ctx->r2) >> 16);
    // 0x800196C0: slti        $at, $v0, 0x4
    ctx->r1 = SIGNED(ctx->r2) < 0X4 ? 1 : 0;
    // 0x800196C4: bnel        $at, $zero, L_8001966C
    if (ctx->r1 != 0) {
        // 0x800196C8: addiu       $t6, $zero, 0x1
        ctx->r14 = ADD32(0, 0X1);
            goto L_8001966C;
    }
    goto skip_0;
    // 0x800196C8: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    skip_0:
    // 0x800196CC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800196D0: addiu       $sp, $sp, 0x40
    ctx->r29 = ADD32(ctx->r29, 0X40);
    // 0x800196D4: jr          $ra
    // 0x800196D8: nop

    return;
    // 0x800196D8: nop

;}
RECOMP_FUNC void func_800196DC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800196DC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800196E0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x800196E4: lui         $s0, 0x800F
    ctx->r16 = S32(0X800F << 16);
    // 0x800196E8: addiu       $s0, $s0, -0x490
    ctx->r16 = ADD32(ctx->r16, -0X490);
    // 0x800196EC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800196F0: jal         0x80098B50
    // 0x800196F4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80098B50(rdram, ctx);
        goto after_0;
    // 0x800196F4: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_0:
    // 0x800196F8: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800196FC: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80019700: jal         0x80096CE0
    // 0x80019704: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_1;
    // 0x80019704: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_1:
    // 0x80019708: lui         $s0, 0x800F
    ctx->r16 = S32(0X800F << 16);
    // 0x8001970C: addiu       $s0, $s0, -0x460
    ctx->r16 = ADD32(ctx->r16, -0X460);
    // 0x80019710: jal         0x80098BD4
    // 0x80019714: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80098BD4(rdram, ctx);
        goto after_2;
    // 0x80019714: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x80019718: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001971C: addiu       $a0, $a0, -0x780C
    ctx->r4 = ADD32(ctx->r4, -0X780C);
    // 0x80019720: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    // 0x80019724: addiu       $t0, $zero, 0x6
    ctx->r8 = ADD32(0, 0X6);
L_80019728:
    // 0x80019728: multu       $a3, $t0
    result = U64(U32(ctx->r7)) * U64(U32(ctx->r8)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8001972C: lhu         $t8, 0x2($a0)
    ctx->r24 = MEM_HU(ctx->r4, 0X2);
    // 0x80019730: lhu         $t7, 0x4($a0)
    ctx->r15 = MEM_HU(ctx->r4, 0X4);
    // 0x80019734: addiu       $a3, $a3, 0x1
    ctx->r7 = ADD32(ctx->r7, 0X1);
    // 0x80019738: sll         $a3, $a3, 16
    ctx->r7 = S32(ctx->r7 << 16);
    // 0x8001973C: sra         $a3, $a3, 16
    ctx->r7 = S32(SIGNED(ctx->r7) >> 16);
    // 0x80019740: mflo        $t6
    ctx->r14 = lo;
    // 0x80019744: addu        $v1, $s0, $t6
    ctx->r3 = ADD32(ctx->r16, ctx->r14);
    // 0x80019748: lhu         $v0, 0x0($v1)
    ctx->r2 = MEM_HU(ctx->r3, 0X0);
    // 0x8001974C: xor         $t9, $t8, $v0
    ctx->r25 = ctx->r24 ^ ctx->r2;
    // 0x80019750: and         $t1, $t9, $v0
    ctx->r9 = ctx->r25 & ctx->r2;
    // 0x80019754: or          $t2, $t7, $t1
    ctx->r10 = ctx->r15 | ctx->r9;
    // 0x80019758: sh          $t2, 0x4($a0)
    MEM_H(0X4, ctx->r4) = ctx->r10;
    // 0x8001975C: sh          $v0, 0x2($a0)
    MEM_H(0X2, ctx->r4) = ctx->r2;
    // 0x80019760: lb          $t3, 0x2($v1)
    ctx->r11 = MEM_B(ctx->r3, 0X2);
    // 0x80019764: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80019768: sh          $t3, 0x6($a0)
    MEM_H(0X6, ctx->r4) = ctx->r11;
    // 0x8001976C: lh          $a2, 0x6($a0)
    ctx->r6 = MEM_H(ctx->r4, 0X6);
    // 0x80019770: lb          $t4, 0x3($v1)
    ctx->r12 = MEM_B(ctx->r3, 0X3);
    // 0x80019774: slti        $at, $a2, -0x28
    ctx->r1 = SIGNED(ctx->r6) < -0X28 ? 1 : 0;
    // 0x80019778: beq         $at, $zero, L_80019784
    if (ctx->r1 == 0) {
        // 0x8001977C: sh          $t4, 0x8($a0)
        MEM_H(0X8, ctx->r4) = ctx->r12;
            goto L_80019784;
    }
    // 0x8001977C: sh          $t4, 0x8($a0)
    MEM_H(0X8, ctx->r4) = ctx->r12;
    // 0x80019780: addiu       $v0, $zero, 0x200
    ctx->r2 = ADD32(0, 0X200);
L_80019784:
    // 0x80019784: slti        $at, $a2, 0x29
    ctx->r1 = SIGNED(ctx->r6) < 0X29 ? 1 : 0;
    // 0x80019788: bnel        $at, $zero, L_8001979C
    if (ctx->r1 != 0) {
        // 0x8001978C: lh          $v1, 0x8($a0)
        ctx->r3 = MEM_H(ctx->r4, 0X8);
            goto L_8001979C;
    }
    goto skip_0;
    // 0x8001978C: lh          $v1, 0x8($a0)
    ctx->r3 = MEM_H(ctx->r4, 0X8);
    skip_0:
    // 0x80019790: ori         $v0, $v0, 0x100
    ctx->r2 = ctx->r2 | 0X100;
    // 0x80019794: andi        $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 & 0XFFFF;
    // 0x80019798: lh          $v1, 0x8($a0)
    ctx->r3 = MEM_H(ctx->r4, 0X8);
L_8001979C:
    // 0x8001979C: slti        $at, $v1, 0x29
    ctx->r1 = SIGNED(ctx->r3) < 0X29 ? 1 : 0;
    // 0x800197A0: bnel        $at, $zero, L_800197B4
    if (ctx->r1 != 0) {
        // 0x800197A4: slti        $at, $v1, -0x28
        ctx->r1 = SIGNED(ctx->r3) < -0X28 ? 1 : 0;
            goto L_800197B4;
    }
    goto skip_1;
    // 0x800197A4: slti        $at, $v1, -0x28
    ctx->r1 = SIGNED(ctx->r3) < -0X28 ? 1 : 0;
    skip_1:
    // 0x800197A8: ori         $v0, $v0, 0x800
    ctx->r2 = ctx->r2 | 0X800;
    // 0x800197AC: andi        $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 & 0XFFFF;
    // 0x800197B0: slti        $at, $v1, -0x28
    ctx->r1 = SIGNED(ctx->r3) < -0X28 ? 1 : 0;
L_800197B4:
    // 0x800197B4: beql        $at, $zero, L_800197C8
    if (ctx->r1 == 0) {
        // 0x800197B8: lhu         $t5, 0xA($a0)
        ctx->r13 = MEM_HU(ctx->r4, 0XA);
            goto L_800197C8;
    }
    goto skip_2;
    // 0x800197B8: lhu         $t5, 0xA($a0)
    ctx->r13 = MEM_HU(ctx->r4, 0XA);
    skip_2:
    // 0x800197BC: ori         $v0, $v0, 0x400
    ctx->r2 = ctx->r2 | 0X400;
    // 0x800197C0: andi        $v0, $v0, 0xFFFF
    ctx->r2 = ctx->r2 & 0XFFFF;
    // 0x800197C4: lhu         $t5, 0xA($a0)
    ctx->r13 = MEM_HU(ctx->r4, 0XA);
L_800197C8:
    // 0x800197C8: slti        $at, $a3, 0x4
    ctx->r1 = SIGNED(ctx->r7) < 0X4 ? 1 : 0;
    // 0x800197CC: addiu       $a0, $a0, 0xE
    ctx->r4 = ADD32(ctx->r4, 0XE);
    // 0x800197D0: xor         $t6, $t5, $v0
    ctx->r14 = ctx->r13 ^ ctx->r2;
    // 0x800197D4: and         $t8, $t6, $v0
    ctx->r24 = ctx->r14 & ctx->r2;
    // 0x800197D8: sh          $t8, -0x2($a0)
    MEM_H(-0X2, ctx->r4) = ctx->r24;
    // 0x800197DC: bne         $at, $zero, L_80019728
    if (ctx->r1 != 0) {
        // 0x800197E0: sh          $v0, -0x4($a0)
        MEM_H(-0X4, ctx->r4) = ctx->r2;
            goto L_80019728;
    }
    // 0x800197E0: sh          $v0, -0x4($a0)
    MEM_H(-0X4, ctx->r4) = ctx->r2;
    // 0x800197E4: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800197E8: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x800197EC: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800197F0: jr          $ra
    // 0x800197F4: nop

    return;
    // 0x800197F4: nop

;}
RECOMP_FUNC void func_800197F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800197F8: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x800197FC: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80019800: addiu       $v1, $v1, -0x77D4
    ctx->r3 = ADD32(ctx->r3, -0X77D4);
    // 0x80019804: addiu       $v0, $v0, -0x780C
    ctx->r2 = ADD32(ctx->r2, -0X780C);
L_80019808:
    // 0x80019808: addiu       $v0, $v0, 0xE
    ctx->r2 = ADD32(ctx->r2, 0XE);
    // 0x8001980C: sltu        $at, $v0, $v1
    ctx->r1 = ctx->r2 < ctx->r3 ? 1 : 0;
    // 0x80019810: bne         $at, $zero, L_80019808
    if (ctx->r1 != 0) {
        // 0x80019814: sh          $zero, -0xA($v0)
        MEM_H(-0XA, ctx->r2) = 0;
            goto L_80019808;
    }
    // 0x80019814: sh          $zero, -0xA($v0)
    MEM_H(-0XA, ctx->r2) = 0;
    // 0x80019818: jr          $ra
    // 0x8001981C: nop

    return;
    // 0x8001981C: nop

;}
RECOMP_FUNC void func_80019820(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80019820: jr          $ra
    // 0x80019824: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
    return;
    // 0x80019824: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
;}
RECOMP_FUNC void func_80019828(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80019828: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x8001982C: sll         $t6, $a3, 3
    ctx->r14 = S32(ctx->r7 << 3);
    // 0x80019830: subu        $t6, $t6, $a3
    ctx->r14 = SUB32(ctx->r14, ctx->r7);
    // 0x80019834: slt         $at, $v0, $a1
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x80019838: bne         $at, $zero, L_80019848
    if (ctx->r1 != 0) {
        // 0x8001983C: sll         $t6, $t6, 1
        ctx->r14 = S32(ctx->r14 << 1);
            goto L_80019848;
    }
    // 0x8001983C: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x80019840: sw          $zero, 0x0($a2)
    MEM_W(0X0, ctx->r6) = 0;
    // 0x80019844: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80019848:
    // 0x80019848: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x8001984C: addiu       $t7, $t7, -0x7D40
    ctx->r15 = ADD32(ctx->r15, -0X7D40);
    // 0x80019850: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x80019854: lhu         $t8, 0x536($v1)
    ctx->r24 = MEM_HU(ctx->r3, 0X536);
    // 0x80019858: sll         $t9, $v0, 3
    ctx->r25 = S32(ctx->r2 << 3);
    // 0x8001985C: addu        $t0, $a0, $t9
    ctx->r8 = ADD32(ctx->r4, ctx->r25);
    // 0x80019860: sh          $t8, 0x0($t0)
    MEM_H(0X0, ctx->r8) = ctx->r24;
    // 0x80019864: lw          $t2, 0x0($a2)
    ctx->r10 = MEM_W(ctx->r6, 0X0);
    // 0x80019868: lhu         $t1, 0x538($v1)
    ctx->r9 = MEM_HU(ctx->r3, 0X538);
    // 0x8001986C: sll         $t3, $t2, 3
    ctx->r11 = S32(ctx->r10 << 3);
    // 0x80019870: addu        $t4, $a0, $t3
    ctx->r12 = ADD32(ctx->r4, ctx->r11);
    // 0x80019874: sh          $t1, 0x2($t4)
    MEM_H(0X2, ctx->r12) = ctx->r9;
    // 0x80019878: lw          $t6, 0x0($a2)
    ctx->r14 = MEM_W(ctx->r6, 0X0);
    // 0x8001987C: lh          $t5, 0x53A($v1)
    ctx->r13 = MEM_H(ctx->r3, 0X53A);
    // 0x80019880: sll         $t7, $t6, 3
    ctx->r15 = S32(ctx->r14 << 3);
    // 0x80019884: addu        $t9, $a0, $t7
    ctx->r25 = ADD32(ctx->r4, ctx->r15);
    // 0x80019888: sh          $t5, 0x4($t9)
    MEM_H(0X4, ctx->r25) = ctx->r13;
    // 0x8001988C: lw          $t0, 0x0($a2)
    ctx->r8 = MEM_W(ctx->r6, 0X0);
    // 0x80019890: lh          $t8, 0x53C($v1)
    ctx->r24 = MEM_H(ctx->r3, 0X53C);
    // 0x80019894: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80019898: sll         $t2, $t0, 3
    ctx->r10 = S32(ctx->r8 << 3);
    // 0x8001989C: addu        $t3, $a0, $t2
    ctx->r11 = ADD32(ctx->r4, ctx->r10);
    // 0x800198A0: sh          $t8, 0x6($t3)
    MEM_H(0X6, ctx->r11) = ctx->r24;
    // 0x800198A4: lw          $t1, 0x0($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X0);
    // 0x800198A8: addiu       $t4, $t1, 0x1
    ctx->r12 = ADD32(ctx->r9, 0X1);
    // 0x800198AC: bne         $a1, $t4, L_800198BC
    if (ctx->r5 != ctx->r12) {
        // 0x800198B0: nop
    
            goto L_800198BC;
    }
    // 0x800198B0: nop

    // 0x800198B4: jr          $ra
    // 0x800198B8: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    return;
    // 0x800198B8: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_800198BC:
    // 0x800198BC: jr          $ra
    // 0x800198C0: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x800198C0: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_800198C4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800198C4: lw          $v0, 0x0($a2)
    ctx->r2 = MEM_W(ctx->r6, 0X0);
    // 0x800198C8: sll         $t6, $a3, 3
    ctx->r14 = S32(ctx->r7 << 3);
    // 0x800198CC: subu        $t6, $t6, $a3
    ctx->r14 = SUB32(ctx->r14, ctx->r7);
    // 0x800198D0: slt         $at, $v0, $a1
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r5) ? 1 : 0;
    // 0x800198D4: bne         $at, $zero, L_800198E4
    if (ctx->r1 != 0) {
        // 0x800198D8: sll         $t6, $t6, 1
        ctx->r14 = S32(ctx->r14 << 1);
            goto L_800198E4;
    }
    // 0x800198D8: sll         $t6, $t6, 1
    ctx->r14 = S32(ctx->r14 << 1);
    // 0x800198DC: sw          $zero, 0x0($a2)
    MEM_W(0X0, ctx->r6) = 0;
    // 0x800198E0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800198E4:
    // 0x800198E4: sll         $t8, $v0, 3
    ctx->r24 = S32(ctx->r2 << 3);
    // 0x800198E8: addu        $t9, $a0, $t8
    ctx->r25 = ADD32(ctx->r4, ctx->r24);
    // 0x800198EC: lhu         $t0, 0x0($t9)
    ctx->r8 = MEM_HU(ctx->r25, 0X0);
    // 0x800198F0: lui         $t7, 0x801D
    ctx->r15 = S32(0X801D << 16);
    // 0x800198F4: addiu       $t7, $t7, -0x7D40
    ctx->r15 = ADD32(ctx->r15, -0X7D40);
    // 0x800198F8: addu        $v1, $t6, $t7
    ctx->r3 = ADD32(ctx->r14, ctx->r15);
    // 0x800198FC: sh          $t0, 0x536($v1)
    MEM_H(0X536, ctx->r3) = ctx->r8;
    // 0x80019900: lw          $t1, 0x0($a2)
    ctx->r9 = MEM_W(ctx->r6, 0X0);
    // 0x80019904: sll         $t2, $t1, 3
    ctx->r10 = S32(ctx->r9 << 3);
    // 0x80019908: addu        $t3, $a0, $t2
    ctx->r11 = ADD32(ctx->r4, ctx->r10);
    // 0x8001990C: lhu         $t4, 0x2($t3)
    ctx->r12 = MEM_HU(ctx->r11, 0X2);
    // 0x80019910: sh          $t4, 0x538($v1)
    MEM_H(0X538, ctx->r3) = ctx->r12;
    // 0x80019914: lw          $t5, 0x0($a2)
    ctx->r13 = MEM_W(ctx->r6, 0X0);
    // 0x80019918: sll         $t6, $t5, 3
    ctx->r14 = S32(ctx->r13 << 3);
    // 0x8001991C: addu        $t7, $a0, $t6
    ctx->r15 = ADD32(ctx->r4, ctx->r14);
    // 0x80019920: lh          $t8, 0x4($t7)
    ctx->r24 = MEM_H(ctx->r15, 0X4);
    // 0x80019924: sh          $t8, 0x53A($v1)
    MEM_H(0X53A, ctx->r3) = ctx->r24;
    // 0x80019928: lw          $t9, 0x0($a2)
    ctx->r25 = MEM_W(ctx->r6, 0X0);
    // 0x8001992C: sll         $t0, $t9, 3
    ctx->r8 = S32(ctx->r25 << 3);
    // 0x80019930: addu        $t1, $a0, $t0
    ctx->r9 = ADD32(ctx->r4, ctx->r8);
    // 0x80019934: lh          $t2, 0x6($t1)
    ctx->r10 = MEM_H(ctx->r9, 0X6);
    // 0x80019938: sh          $t2, 0x53C($v1)
    MEM_H(0X53C, ctx->r3) = ctx->r10;
    // 0x8001993C: lw          $t3, 0x0($a2)
    ctx->r11 = MEM_W(ctx->r6, 0X0);
    // 0x80019940: or          $v1, $zero, $zero
    ctx->r3 = 0 | 0;
    // 0x80019944: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x80019948: bne         $a1, $t4, L_80019958
    if (ctx->r5 != ctx->r12) {
        // 0x8001994C: nop
    
            goto L_80019958;
    }
    // 0x8001994C: nop

    // 0x80019950: jr          $ra
    // 0x80019954: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
    return;
    // 0x80019954: addiu       $v0, $zero, -0x1
    ctx->r2 = ADD32(0, -0X1);
L_80019958:
    // 0x80019958: jr          $ra
    // 0x8001995C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
    return;
    // 0x8001995C: or          $v0, $v1, $zero
    ctx->r2 = ctx->r3 | 0;
;}
RECOMP_FUNC void func_80019960(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80019960: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x80019964: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x80019968: addiu       $v0, $v0, 0x13D8
    ctx->r2 = ADD32(ctx->r2, 0X13D8);
    // 0x8001996C: addiu       $v1, $v1, -0x428
    ctx->r3 = ADD32(ctx->r3, -0X428);
L_80019970:
    // 0x80019970: addiu       $t8, $zero, 0x0
    ctx->r24 = ADD32(0, 0X0);
    // 0x80019974: addiu       $t9, $zero, 0x0
    ctx->r25 = ADD32(0, 0X0);
    // 0x80019978: addiu       $t0, $zero, 0x0
    ctx->r8 = ADD32(0, 0X0);
    // 0x8001997C: addiu       $t1, $zero, 0x0
    ctx->r9 = ADD32(0, 0X0);
    // 0x80019980: addiu       $t2, $zero, 0x0
    ctx->r10 = ADD32(0, 0X0);
    // 0x80019984: addiu       $t3, $zero, 0x0
    ctx->r11 = ADD32(0, 0X0);
    // 0x80019988: addiu       $v1, $v1, 0x20
    ctx->r3 = ADD32(ctx->r3, 0X20);
    // 0x8001998C: addiu       $t6, $zero, 0x0
    ctx->r14 = ADD32(0, 0X0);
    // 0x80019990: addiu       $t7, $zero, 0x0
    ctx->r15 = ADD32(0, 0X0);
    // 0x80019994: sw          $t3, -0x4($v1)
    MEM_W(-0X4, ctx->r3) = ctx->r11;
    // 0x80019998: sw          $t2, -0x8($v1)
    MEM_W(-0X8, ctx->r3) = ctx->r10;
    // 0x8001999C: sw          $t1, -0xC($v1)
    MEM_W(-0XC, ctx->r3) = ctx->r9;
    // 0x800199A0: sw          $t0, -0x10($v1)
    MEM_W(-0X10, ctx->r3) = ctx->r8;
    // 0x800199A4: sw          $t9, -0x14($v1)
    MEM_W(-0X14, ctx->r3) = ctx->r25;
    // 0x800199A8: sw          $t8, -0x18($v1)
    MEM_W(-0X18, ctx->r3) = ctx->r24;
    // 0x800199AC: sw          $t7, -0x1C($v1)
    MEM_W(-0X1C, ctx->r3) = ctx->r15;
    // 0x800199B0: bne         $v1, $v0, L_80019970
    if (ctx->r3 != ctx->r2) {
        // 0x800199B4: sw          $t6, -0x20($v1)
        MEM_W(-0X20, ctx->r3) = ctx->r14;
            goto L_80019970;
    }
    // 0x800199B4: sw          $t6, -0x20($v1)
    MEM_W(-0X20, ctx->r3) = ctx->r14;
    // 0x800199B8: jr          $ra
    // 0x800199BC: nop

    return;
    // 0x800199BC: nop

;}
RECOMP_FUNC void func_800199C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800199C0: addiu       $sp, $sp, -0x58
    ctx->r29 = ADD32(ctx->r29, -0X58);
    // 0x800199C4: sw          $fp, 0x38($sp)
    MEM_W(0X38, ctx->r29) = ctx->r30;
    // 0x800199C8: lui         $fp, 0x800F
    ctx->r30 = S32(0X800F << 16);
    // 0x800199CC: addiu       $fp, $fp, -0x43C
    ctx->r30 = ADD32(ctx->r30, -0X43C);
    // 0x800199D0: sw          $a0, 0x0($fp)
    MEM_W(0X0, ctx->r30) = ctx->r4;
    // 0x800199D4: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800199D8: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x800199DC: sw          $a0, -0x440($at)
    MEM_W(-0X440, ctx->r1) = ctx->r4;
    // 0x800199E0: addiu       $v1, $v1, -0x434
    ctx->r3 = ADD32(ctx->r3, -0X434);
    // 0x800199E4: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x800199E8: sw          $a2, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r6;
    // 0x800199EC: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x800199F0: sw          $ra, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r31;
    // 0x800199F4: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x800199F8: sw          $a2, -0x438($at)
    MEM_W(-0X438, ctx->r1) = ctx->r6;
    // 0x800199FC: addiu       $t9, $a2, -0x1
    ctx->r25 = ADD32(ctx->r6, -0X1);
    // 0x80019A00: lui         $s3, 0x800F
    ctx->r19 = S32(0X800F << 16);
    // 0x80019A04: or          $s1, $a1, $zero
    ctx->r17 = ctx->r5 | 0;
    // 0x80019A08: sw          $s7, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r23;
    // 0x80019A0C: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x80019A10: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x80019A14: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x80019A18: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80019A1C: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80019A20: sw          $a0, 0x58($sp)
    MEM_W(0X58, ctx->r29) = ctx->r4;
    // 0x80019A24: sw          $a2, 0x60($sp)
    MEM_W(0X60, ctx->r29) = ctx->r6;
    // 0x80019A28: sw          $t9, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r25;
    // 0x80019A2C: jal         0x80019FD0
    // 0x80019A30: addiu       $s3, $s3, -0x428
    ctx->r19 = ADD32(ctx->r19, -0X428);
    func_80019FD0(rdram, ctx);
        goto after_0;
    // 0x80019A30: addiu       $s3, $s3, -0x428
    ctx->r19 = ADD32(ctx->r19, -0X428);
    after_0:
    // 0x80019A34: lui         $t0, 0x800F
    ctx->r8 = S32(0X800F << 16);
    // 0x80019A38: lw          $t0, -0x434($t0)
    ctx->r8 = MEM_W(ctx->r8, -0X434);
    // 0x80019A3C: lui         $s5, 0x800F
    ctx->r21 = S32(0X800F << 16);
    // 0x80019A40: addiu       $s5, $s5, -0x42A
    ctx->r21 = ADD32(ctx->r21, -0X42A);
    // 0x80019A44: blezl       $t0, L_80019F28
    if (SIGNED(ctx->r8) <= 0) {
        // 0x80019A48: or          $v0, $s1, $zero
        ctx->r2 = ctx->r17 | 0;
            goto L_80019F28;
    }
    goto skip_0;
    // 0x80019A48: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
    skip_0:
    // 0x80019A4C: lbu         $s6, 0x0($s3)
    ctx->r22 = MEM_BU(ctx->r19, 0X0);
L_80019A50:
    // 0x80019A50: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80019A54: jal         0x80019F58
    // 0x80019A58: sll         $s6, $s6, 24
    ctx->r22 = S32(ctx->r22 << 24);
    func_80019F58(rdram, ctx);
        goto after_1;
    // 0x80019A58: sll         $s6, $s6, 24
    ctx->r22 = S32(ctx->r22 << 24);
    after_1:
    // 0x80019A5C: lbu         $t1, 0x0($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X0);
    // 0x80019A60: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019A64: sll         $t2, $t1, 16
    ctx->r10 = S32(ctx->r9 << 16);
    // 0x80019A68: jal         0x80019F58
    // 0x80019A6C: addu        $s6, $s6, $t2
    ctx->r22 = ADD32(ctx->r22, ctx->r10);
    func_80019F58(rdram, ctx);
        goto after_2;
    // 0x80019A6C: addu        $s6, $s6, $t2
    ctx->r22 = ADD32(ctx->r22, ctx->r10);
    after_2:
    // 0x80019A70: lbu         $t3, 0x0($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X0);
    // 0x80019A74: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019A78: sll         $t4, $t3, 8
    ctx->r12 = S32(ctx->r11 << 8);
    // 0x80019A7C: jal         0x80019F58
    // 0x80019A80: addu        $s6, $s6, $t4
    ctx->r22 = ADD32(ctx->r22, ctx->r12);
    func_80019F58(rdram, ctx);
        goto after_3;
    // 0x80019A80: addu        $s6, $s6, $t4
    ctx->r22 = ADD32(ctx->r22, ctx->r12);
    after_3:
    // 0x80019A84: lbu         $t5, 0x0($v0)
    ctx->r13 = MEM_BU(ctx->r2, 0X0);
    // 0x80019A88: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019A8C: jal         0x80019F58
    // 0x80019A90: addu        $s6, $s6, $t5
    ctx->r22 = ADD32(ctx->r22, ctx->r13);
    func_80019F58(rdram, ctx);
        goto after_4;
    // 0x80019A90: addu        $s6, $s6, $t5
    ctx->r22 = ADD32(ctx->r22, ctx->r13);
    after_4:
    // 0x80019A94: sll         $t6, $s6, 0
    ctx->r14 = S32(ctx->r22 << 0);
    // 0x80019A98: bgez        $t6, L_80019ACC
    if (SIGNED(ctx->r14) >= 0) {
        // 0x80019A9C: or          $s3, $v0, $zero
        ctx->r19 = ctx->r2 | 0;
            goto L_80019ACC;
    }
    // 0x80019A9C: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019AA0: lui         $t7, 0x800D
    ctx->r15 = S32(0X800D << 16);
    // 0x80019AA4: addiu       $t7, $t7, -0x1898
    ctx->r15 = ADD32(ctx->r15, -0X1898);
    // 0x80019AA8: lbu         $t8, 0x0($t7)
    ctx->r24 = MEM_BU(ctx->r15, 0X0);
    // 0x80019AAC: beql        $t8, $zero, L_80019AD0
    if (ctx->r24 == 0) {
        // 0x80019AB0: lw          $v1, 0x0($fp)
        ctx->r3 = MEM_W(ctx->r30, 0X0);
            goto L_80019AD0;
    }
    goto skip_1;
    // 0x80019AB0: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
    skip_1:
    // 0x80019AB4: lw          $a0, 0x58($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X58);
    // 0x80019AB8: or          $a1, $s1, $zero
    ctx->r5 = ctx->r17 | 0;
    // 0x80019ABC: jal         0x8001A05C
    // 0x80019AC0: lw          $a2, 0x60($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X60);
    func_8001A05C(rdram, ctx);
        goto after_5;
    // 0x80019AC0: lw          $a2, 0x60($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X60);
    after_5:
    // 0x80019AC4: b           L_80019F2C
    // 0x80019AC8: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
        goto L_80019F2C;
    // 0x80019AC8: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_80019ACC:
    // 0x80019ACC: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
L_80019AD0:
    // 0x80019AD0: lui         $at, 0xF000
    ctx->r1 = S32(0XF000 << 16);
    // 0x80019AD4: and         $a0, $s6, $at
    ctx->r4 = ctx->r22 & ctx->r1;
    // 0x80019AD8: addu        $s7, $v1, $s6
    ctx->r23 = ADD32(ctx->r3, ctx->r22);
    // 0x80019ADC: bne         $a0, $zero, L_80019C64
    if (ctx->r4 != 0) {
        // 0x80019AE0: addiu       $s7, $s7, -0x4
        ctx->r23 = ADD32(ctx->r23, -0X4);
            goto L_80019C64;
    }
    // 0x80019AE0: addiu       $s7, $s7, -0x4
    ctx->r23 = ADD32(ctx->r23, -0X4);
    // 0x80019AE4: sltu        $at, $v1, $s7
    ctx->r1 = ctx->r3 < ctx->r23 ? 1 : 0;
    // 0x80019AE8: beq         $at, $zero, L_80019C5C
    if (ctx->r1 == 0) {
        // 0x80019AEC: nop
    
            goto L_80019C5C;
    }
    // 0x80019AEC: nop

    // 0x80019AF0: lbu         $s0, 0x0($s3)
    ctx->r16 = MEM_BU(ctx->r19, 0X0);
L_80019AF4:
    // 0x80019AF4: jal         0x80019F58
    // 0x80019AF8: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    func_80019F58(rdram, ctx);
        goto after_6;
    // 0x80019AF8: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    after_6:
    // 0x80019AFC: slti        $at, $s0, 0x80
    ctx->r1 = SIGNED(ctx->r16) < 0X80 ? 1 : 0;
    // 0x80019B00: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019B04: beq         $at, $zero, L_80019B70
    if (ctx->r1 == 0) {
        // 0x80019B08: or          $s4, $s0, $zero
        ctx->r20 = ctx->r16 | 0;
            goto L_80019B70;
    }
    // 0x80019B08: or          $s4, $s0, $zero
    ctx->r20 = ctx->r16 | 0;
    // 0x80019B0C: lbu         $t9, 0x0($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X0);
    // 0x80019B10: sll         $t0, $s0, 8
    ctx->r8 = S32(ctx->r16 << 8);
    // 0x80019B14: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019B18: addu        $s2, $t9, $t0
    ctx->r18 = ADD32(ctx->r25, ctx->r8);
    // 0x80019B1C: andi        $s2, $s2, 0x3FF
    ctx->r18 = ctx->r18 & 0X3FF;
    // 0x80019B20: jal         0x80019F58
    // 0x80019B24: andi        $s2, $s2, 0xFFFF
    ctx->r18 = ctx->r18 & 0XFFFF;
    func_80019F58(rdram, ctx);
        goto after_7;
    // 0x80019B24: andi        $s2, $s2, 0xFFFF
    ctx->r18 = ctx->r18 & 0XFFFF;
    after_7:
    // 0x80019B28: sra         $a1, $s4, 2
    ctx->r5 = S32(SIGNED(ctx->r20) >> 2);
    // 0x80019B2C: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x80019B30: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x80019B34: addiu       $s0, $a1, -0x1
    ctx->r16 = ADD32(ctx->r5, -0X1);
    // 0x80019B38: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019B3C: subu        $a0, $s1, $s2
    ctx->r4 = SUB32(ctx->r17, ctx->r18);
    // 0x80019B40: beq         $a1, $zero, L_80019C4C
    if (ctx->r5 == 0) {
        // 0x80019B44: andi        $s0, $s0, 0xFFFF
        ctx->r16 = ctx->r16 & 0XFFFF;
            goto L_80019C4C;
    }
    // 0x80019B44: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
L_80019B48:
    // 0x80019B48: lbu         $t1, 0x0($a0)
    ctx->r9 = MEM_BU(ctx->r4, 0X0);
    // 0x80019B4C: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019B50: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019B54: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019B58: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80019B5C: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
    // 0x80019B60: bne         $v1, $zero, L_80019B48
    if (ctx->r3 != 0) {
        // 0x80019B64: sb          $t1, -0x1($s1)
        MEM_B(-0X1, ctx->r17) = ctx->r9;
            goto L_80019B48;
    }
    // 0x80019B64: sb          $t1, -0x1($s1)
    MEM_B(-0X1, ctx->r17) = ctx->r9;
    // 0x80019B68: b           L_80019C50
    // 0x80019B6C: lw          $t3, 0x0($fp)
    ctx->r11 = MEM_W(ctx->r30, 0X0);
        goto L_80019C50;
    // 0x80019B6C: lw          $t3, 0x0($fp)
    ctx->r11 = MEM_W(ctx->r30, 0X0);
L_80019B70:
    // 0x80019B70: slti        $at, $s4, 0xA0
    ctx->r1 = SIGNED(ctx->r20) < 0XA0 ? 1 : 0;
    // 0x80019B74: beq         $at, $zero, L_80019BB8
    if (ctx->r1 == 0) {
        // 0x80019B78: andi        $v0, $s4, 0x1F
        ctx->r2 = ctx->r20 & 0X1F;
            goto L_80019BB8;
    }
    // 0x80019B78: andi        $v0, $s4, 0x1F
    ctx->r2 = ctx->r20 & 0X1F;
    // 0x80019B7C: addiu       $s0, $v0, -0x1
    ctx->r16 = ADD32(ctx->r2, -0X1);
    // 0x80019B80: beq         $v0, $zero, L_80019C4C
    if (ctx->r2 == 0) {
        // 0x80019B84: andi        $s0, $s0, 0xFFFF
        ctx->r16 = ctx->r16 & 0XFFFF;
            goto L_80019C4C;
    }
    // 0x80019B84: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
L_80019B88:
    // 0x80019B88: lbu         $t2, 0x0($s3)
    ctx->r10 = MEM_BU(ctx->r19, 0X0);
    // 0x80019B8C: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
    // 0x80019B90: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80019B94: jal         0x80019F58
    // 0x80019B98: sb          $t2, -0x1($s1)
    MEM_B(-0X1, ctx->r17) = ctx->r10;
    func_80019F58(rdram, ctx);
        goto after_8;
    // 0x80019B98: sb          $t2, -0x1($s1)
    MEM_B(-0X1, ctx->r17) = ctx->r10;
    after_8:
    // 0x80019B9C: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019BA0: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019BA4: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019BA8: bne         $v1, $zero, L_80019B88
    if (ctx->r3 != 0) {
        // 0x80019BAC: or          $s3, $v0, $zero
        ctx->r19 = ctx->r2 | 0;
            goto L_80019B88;
    }
    // 0x80019BAC: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019BB0: b           L_80019C50
    // 0x80019BB4: lw          $t3, 0x0($fp)
    ctx->r11 = MEM_W(ctx->r30, 0X0);
        goto L_80019C50;
    // 0x80019BB4: lw          $t3, 0x0($fp)
    ctx->r11 = MEM_W(ctx->r30, 0X0);
L_80019BB8:
    // 0x80019BB8: slti        $at, $s4, 0xC0
    ctx->r1 = SIGNED(ctx->r20) < 0XC0 ? 1 : 0;
    // 0x80019BBC: beq         $at, $zero, L_80019BCC
    if (ctx->r1 == 0) {
        // 0x80019BC0: andi        $s0, $s4, 0x1F
        ctx->r16 = ctx->r20 & 0X1F;
            goto L_80019BCC;
    }
    // 0x80019BC0: andi        $s0, $s4, 0x1F
    ctx->r16 = ctx->r20 & 0X1F;
    // 0x80019BC4: b           L_80019F28
    // 0x80019BC8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80019F28;
    // 0x80019BC8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80019BCC:
    // 0x80019BCC: slti        $at, $s4, 0xE0
    ctx->r1 = SIGNED(ctx->r20) < 0XE0 ? 1 : 0;
    // 0x80019BD0: beq         $at, $zero, L_80019BF0
    if (ctx->r1 == 0) {
        // 0x80019BD4: addiu       $s0, $s0, 0x2
        ctx->r16 = ADD32(ctx->r16, 0X2);
            goto L_80019BF0;
    }
    // 0x80019BD4: addiu       $s0, $s0, 0x2
    ctx->r16 = ADD32(ctx->r16, 0X2);
    // 0x80019BD8: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019BDC: lbu         $s2, 0x0($s3)
    ctx->r18 = MEM_BU(ctx->r19, 0X0);
    // 0x80019BE0: jal         0x80019F58
    // 0x80019BE4: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    func_80019F58(rdram, ctx);
        goto after_9;
    // 0x80019BE4: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    after_9:
    // 0x80019BE8: b           L_80019C24
    // 0x80019BEC: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
        goto L_80019C24;
    // 0x80019BEC: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
L_80019BF0:
    // 0x80019BF0: slti        $at, $s4, 0xFF
    ctx->r1 = SIGNED(ctx->r20) < 0XFF ? 1 : 0;
    // 0x80019BF4: beq         $at, $zero, L_80019C0C
    if (ctx->r1 == 0) {
        // 0x80019BF8: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_80019C0C;
    }
    // 0x80019BF8: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80019BFC: andi        $s0, $s4, 0x1F
    ctx->r16 = ctx->r20 & 0X1F;
    // 0x80019C00: addiu       $s0, $s0, 0x2
    ctx->r16 = ADD32(ctx->r16, 0X2);
    // 0x80019C04: b           L_80019C24
    // 0x80019C08: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
        goto L_80019C24;
    // 0x80019C08: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
L_80019C0C:
    // 0x80019C0C: lbu         $s0, 0x0($s3)
    ctx->r16 = MEM_BU(ctx->r19, 0X0);
    // 0x80019C10: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80019C14: addiu       $s0, $s0, 0x2
    ctx->r16 = ADD32(ctx->r16, 0X2);
    // 0x80019C18: jal         0x80019F58
    // 0x80019C1C: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    func_80019F58(rdram, ctx);
        goto after_10;
    // 0x80019C1C: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    after_10:
    // 0x80019C20: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
L_80019C24:
    // 0x80019C24: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019C28: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019C2C: beq         $v1, $zero, L_80019C4C
    if (ctx->r3 == 0) {
        // 0x80019C30: andi        $s0, $s0, 0xFFFF
        ctx->r16 = ctx->r16 & 0XFFFF;
            goto L_80019C4C;
    }
    // 0x80019C30: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
L_80019C34:
    // 0x80019C34: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019C38: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019C3C: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019C40: sb          $s2, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r18;
    // 0x80019C44: bne         $v1, $zero, L_80019C34
    if (ctx->r3 != 0) {
        // 0x80019C48: addiu       $s1, $s1, 0x1
        ctx->r17 = ADD32(ctx->r17, 0X1);
            goto L_80019C34;
    }
    // 0x80019C48: addiu       $s1, $s1, 0x1
    ctx->r17 = ADD32(ctx->r17, 0X1);
L_80019C4C:
    // 0x80019C4C: lw          $t3, 0x0($fp)
    ctx->r11 = MEM_W(ctx->r30, 0X0);
L_80019C50:
    // 0x80019C50: sltu        $at, $t3, $s7
    ctx->r1 = ctx->r11 < ctx->r23 ? 1 : 0;
    // 0x80019C54: bnel        $at, $zero, L_80019AF4
    if (ctx->r1 != 0) {
        // 0x80019C58: lbu         $s0, 0x0($s3)
        ctx->r16 = MEM_BU(ctx->r19, 0X0);
            goto L_80019AF4;
    }
    goto skip_2;
    // 0x80019C58: lbu         $s0, 0x0($s3)
    ctx->r16 = MEM_BU(ctx->r19, 0X0);
    skip_2:
L_80019C5C:
    // 0x80019C5C: b           L_80019ED0
    // 0x80019C60: andi        $v1, $s7, 0xF
    ctx->r3 = ctx->r23 & 0XF;
        goto L_80019ED0;
    // 0x80019C60: andi        $v1, $s7, 0xF
    ctx->r3 = ctx->r23 & 0XF;
L_80019C64:
    // 0x80019C64: srl         $t5, $a0, 28
    ctx->r13 = S32(U32(ctx->r4) >> 28);
    // 0x80019C68: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x80019C6C: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80019C70: andi        $t7, $t6, 0xFFFF
    ctx->r15 = ctx->r14 & 0XFFFF;
    // 0x80019C74: andi        $v0, $zero, 0xFFFF
    ctx->r2 = 0 & 0XFFFF;
    // 0x80019C78: addiu       $a1, $a1, -0x42C
    ctx->r5 = ADD32(ctx->r5, -0X42C);
    // 0x80019C7C: slt         $at, $v0, $t7
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r15) ? 1 : 0;
    // 0x80019C80: sh          $zero, 0x0($a1)
    MEM_H(0X0, ctx->r5) = 0;
    // 0x80019C84: sh          $t6, 0x0($s5)
    MEM_H(0X0, ctx->r21) = ctx->r14;
    // 0x80019C88: beq         $at, $zero, L_80019ECC
    if (ctx->r1 == 0) {
        // 0x80019C8C: sw          $s1, 0x40($sp)
        MEM_W(0X40, ctx->r29) = ctx->r17;
            goto L_80019ECC;
    }
    // 0x80019C8C: sw          $s1, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r17;
L_80019C90:
    // 0x80019C90: beql        $v0, $zero, L_80019D00
    if (ctx->r2 == 0) {
        // 0x80019C94: lui         $at, 0xFFF
        ctx->r1 = S32(0XFFF << 16);
            goto L_80019D00;
    }
    goto skip_3;
    // 0x80019C94: lui         $at, 0xFFF
    ctx->r1 = S32(0XFFF << 16);
    skip_3:
    // 0x80019C98: lbu         $s6, 0x0($s3)
    ctx->r22 = MEM_BU(ctx->r19, 0X0);
    // 0x80019C9C: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80019CA0: jal         0x80019F58
    // 0x80019CA4: sll         $s6, $s6, 24
    ctx->r22 = S32(ctx->r22 << 24);
    func_80019F58(rdram, ctx);
        goto after_11;
    // 0x80019CA4: sll         $s6, $s6, 24
    ctx->r22 = S32(ctx->r22 << 24);
    after_11:
    // 0x80019CA8: lbu         $t8, 0x0($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X0);
    // 0x80019CAC: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019CB0: sll         $t9, $t8, 16
    ctx->r25 = S32(ctx->r24 << 16);
    // 0x80019CB4: jal         0x80019F58
    // 0x80019CB8: addu        $s6, $s6, $t9
    ctx->r22 = ADD32(ctx->r22, ctx->r25);
    func_80019F58(rdram, ctx);
        goto after_12;
    // 0x80019CB8: addu        $s6, $s6, $t9
    ctx->r22 = ADD32(ctx->r22, ctx->r25);
    after_12:
    // 0x80019CBC: lbu         $t0, 0x0($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X0);
    // 0x80019CC0: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019CC4: sll         $t1, $t0, 8
    ctx->r9 = S32(ctx->r8 << 8);
    // 0x80019CC8: jal         0x80019F58
    // 0x80019CCC: addu        $s6, $s6, $t1
    ctx->r22 = ADD32(ctx->r22, ctx->r9);
    func_80019F58(rdram, ctx);
        goto after_13;
    // 0x80019CCC: addu        $s6, $s6, $t1
    ctx->r22 = ADD32(ctx->r22, ctx->r9);
    after_13:
    // 0x80019CD0: lbu         $t2, 0x0($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X0);
    // 0x80019CD4: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019CD8: jal         0x80019F58
    // 0x80019CDC: addu        $s6, $s6, $t2
    ctx->r22 = ADD32(ctx->r22, ctx->r10);
    func_80019F58(rdram, ctx);
        goto after_14;
    // 0x80019CDC: addu        $s6, $s6, $t2
    ctx->r22 = ADD32(ctx->r22, ctx->r10);
    after_14:
    // 0x80019CE0: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
    // 0x80019CE4: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80019CE8: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019CEC: addu        $s7, $v1, $s6
    ctx->r23 = ADD32(ctx->r3, ctx->r22);
    // 0x80019CF0: addiu       $s7, $s7, -0x4
    ctx->r23 = ADD32(ctx->r23, -0X4);
    // 0x80019CF4: addiu       $a1, $a1, -0x42C
    ctx->r5 = ADD32(ctx->r5, -0X42C);
    // 0x80019CF8: lw          $s1, 0x40($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X40);
    // 0x80019CFC: lui         $at, 0xFFF
    ctx->r1 = S32(0XFFF << 16);
L_80019D00:
    // 0x80019D00: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x80019D04: and         $s6, $s6, $at
    ctx->r22 = ctx->r22 & ctx->r1;
    // 0x80019D08: sltu        $at, $v1, $s7
    ctx->r1 = ctx->r3 < ctx->r23 ? 1 : 0;
    // 0x80019D0C: beql        $at, $zero, L_80019EA8
    if (ctx->r1 == 0) {
        // 0x80019D10: lhu         $t3, 0x0($a1)
        ctx->r11 = MEM_HU(ctx->r5, 0X0);
            goto L_80019EA8;
    }
    goto skip_4;
    // 0x80019D10: lhu         $t3, 0x0($a1)
    ctx->r11 = MEM_HU(ctx->r5, 0X0);
    skip_4:
    // 0x80019D14: lbu         $s0, 0x0($s3)
    ctx->r16 = MEM_BU(ctx->r19, 0X0);
L_80019D18:
    // 0x80019D18: jal         0x80019F58
    // 0x80019D1C: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    func_80019F58(rdram, ctx);
        goto after_15;
    // 0x80019D1C: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    after_15:
    // 0x80019D20: slti        $at, $s0, 0x80
    ctx->r1 = SIGNED(ctx->r16) < 0X80 ? 1 : 0;
    // 0x80019D24: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019D28: beq         $at, $zero, L_80019DA8
    if (ctx->r1 == 0) {
        // 0x80019D2C: or          $s4, $s0, $zero
        ctx->r20 = ctx->r16 | 0;
            goto L_80019DA8;
    }
    // 0x80019D2C: or          $s4, $s0, $zero
    ctx->r20 = ctx->r16 | 0;
    // 0x80019D30: lbu         $t3, 0x0($v0)
    ctx->r11 = MEM_BU(ctx->r2, 0X0);
    // 0x80019D34: sll         $t4, $s0, 8
    ctx->r12 = S32(ctx->r16 << 8);
    // 0x80019D38: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80019D3C: addu        $s2, $t3, $t4
    ctx->r18 = ADD32(ctx->r11, ctx->r12);
    // 0x80019D40: andi        $s2, $s2, 0x3FF
    ctx->r18 = ctx->r18 & 0X3FF;
    // 0x80019D44: jal         0x80019F58
    // 0x80019D48: andi        $s2, $s2, 0xFFFF
    ctx->r18 = ctx->r18 & 0XFFFF;
    func_80019F58(rdram, ctx);
        goto after_16;
    // 0x80019D48: andi        $s2, $s2, 0xFFFF
    ctx->r18 = ctx->r18 & 0XFFFF;
    after_16:
    // 0x80019D4C: lhu         $t5, 0x0($s5)
    ctx->r13 = MEM_HU(ctx->r21, 0X0);
    // 0x80019D50: sra         $a1, $s4, 2
    ctx->r5 = S32(SIGNED(ctx->r20) >> 2);
    // 0x80019D54: addiu       $a1, $a1, 0x2
    ctx->r5 = ADD32(ctx->r5, 0X2);
    // 0x80019D58: multu       $s2, $t5
    result = U64(U32(ctx->r18)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80019D5C: andi        $a1, $a1, 0xFFFF
    ctx->r5 = ctx->r5 & 0XFFFF;
    // 0x80019D60: addiu       $s0, $a1, -0x1
    ctx->r16 = ADD32(ctx->r5, -0X1);
    // 0x80019D64: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019D68: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019D6C: mflo        $t6
    ctx->r14 = lo;
    // 0x80019D70: subu        $a0, $s1, $t6
    ctx->r4 = SUB32(ctx->r17, ctx->r14);
    // 0x80019D74: beql        $a1, $zero, L_80019E90
    if (ctx->r5 == 0) {
        // 0x80019D78: lw          $v1, 0x0($fp)
        ctx->r3 = MEM_W(ctx->r30, 0X0);
            goto L_80019E90;
    }
    goto skip_5;
    // 0x80019D78: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
    skip_5:
L_80019D7C:
    // 0x80019D7C: lbu         $t7, 0x0($a0)
    ctx->r15 = MEM_BU(ctx->r4, 0X0);
    // 0x80019D80: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019D84: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019D88: sb          $t7, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r15;
    // 0x80019D8C: lhu         $v0, 0x0($s5)
    ctx->r2 = MEM_HU(ctx->r21, 0X0);
    // 0x80019D90: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019D94: addu        $s1, $s1, $v0
    ctx->r17 = ADD32(ctx->r17, ctx->r2);
    // 0x80019D98: bne         $v1, $zero, L_80019D7C
    if (ctx->r3 != 0) {
        // 0x80019D9C: addu        $a0, $a0, $v0
        ctx->r4 = ADD32(ctx->r4, ctx->r2);
            goto L_80019D7C;
    }
    // 0x80019D9C: addu        $a0, $a0, $v0
    ctx->r4 = ADD32(ctx->r4, ctx->r2);
    // 0x80019DA0: b           L_80019E90
    // 0x80019DA4: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
        goto L_80019E90;
    // 0x80019DA4: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
L_80019DA8:
    // 0x80019DA8: slti        $at, $s4, 0xA0
    ctx->r1 = SIGNED(ctx->r20) < 0XA0 ? 1 : 0;
    // 0x80019DAC: beq         $at, $zero, L_80019DF4
    if (ctx->r1 == 0) {
        // 0x80019DB0: andi        $v0, $s4, 0x1F
        ctx->r2 = ctx->r20 & 0X1F;
            goto L_80019DF4;
    }
    // 0x80019DB0: andi        $v0, $s4, 0x1F
    ctx->r2 = ctx->r20 & 0X1F;
    // 0x80019DB4: addiu       $s0, $v0, -0x1
    ctx->r16 = ADD32(ctx->r2, -0X1);
    // 0x80019DB8: beq         $v0, $zero, L_80019E8C
    if (ctx->r2 == 0) {
        // 0x80019DBC: andi        $s0, $s0, 0xFFFF
        ctx->r16 = ctx->r16 & 0XFFFF;
            goto L_80019E8C;
    }
    // 0x80019DBC: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
L_80019DC0:
    // 0x80019DC0: lbu         $t8, 0x0($s3)
    ctx->r24 = MEM_BU(ctx->r19, 0X0);
    // 0x80019DC4: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80019DC8: sb          $t8, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r24;
    // 0x80019DCC: lhu         $t9, 0x0($s5)
    ctx->r25 = MEM_HU(ctx->r21, 0X0);
    // 0x80019DD0: jal         0x80019F58
    // 0x80019DD4: addu        $s1, $s1, $t9
    ctx->r17 = ADD32(ctx->r17, ctx->r25);
    func_80019F58(rdram, ctx);
        goto after_17;
    // 0x80019DD4: addu        $s1, $s1, $t9
    ctx->r17 = ADD32(ctx->r17, ctx->r25);
    after_17:
    // 0x80019DD8: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019DDC: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019DE0: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019DE4: bne         $v1, $zero, L_80019DC0
    if (ctx->r3 != 0) {
        // 0x80019DE8: or          $s3, $v0, $zero
        ctx->r19 = ctx->r2 | 0;
            goto L_80019DC0;
    }
    // 0x80019DE8: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
    // 0x80019DEC: b           L_80019E90
    // 0x80019DF0: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
        goto L_80019E90;
    // 0x80019DF0: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
L_80019DF4:
    // 0x80019DF4: slti        $at, $s4, 0xC0
    ctx->r1 = SIGNED(ctx->r20) < 0XC0 ? 1 : 0;
    // 0x80019DF8: beq         $at, $zero, L_80019E08
    if (ctx->r1 == 0) {
        // 0x80019DFC: andi        $s0, $s4, 0x1F
        ctx->r16 = ctx->r20 & 0X1F;
            goto L_80019E08;
    }
    // 0x80019DFC: andi        $s0, $s4, 0x1F
    ctx->r16 = ctx->r20 & 0X1F;
    // 0x80019E00: b           L_80019F28
    // 0x80019E04: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80019F28;
    // 0x80019E04: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80019E08:
    // 0x80019E08: slti        $at, $s4, 0xE0
    ctx->r1 = SIGNED(ctx->r20) < 0XE0 ? 1 : 0;
    // 0x80019E0C: beq         $at, $zero, L_80019E2C
    if (ctx->r1 == 0) {
        // 0x80019E10: addiu       $s0, $s0, 0x2
        ctx->r16 = ADD32(ctx->r16, 0X2);
            goto L_80019E2C;
    }
    // 0x80019E10: addiu       $s0, $s0, 0x2
    ctx->r16 = ADD32(ctx->r16, 0X2);
    // 0x80019E14: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019E18: lbu         $s2, 0x0($s3)
    ctx->r18 = MEM_BU(ctx->r19, 0X0);
    // 0x80019E1C: jal         0x80019F58
    // 0x80019E20: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    func_80019F58(rdram, ctx);
        goto after_18;
    // 0x80019E20: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    after_18:
    // 0x80019E24: b           L_80019E60
    // 0x80019E28: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
        goto L_80019E60;
    // 0x80019E28: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
L_80019E2C:
    // 0x80019E2C: slti        $at, $s4, 0xFF
    ctx->r1 = SIGNED(ctx->r20) < 0XFF ? 1 : 0;
    // 0x80019E30: beq         $at, $zero, L_80019E48
    if (ctx->r1 == 0) {
        // 0x80019E34: or          $s2, $zero, $zero
        ctx->r18 = 0 | 0;
            goto L_80019E48;
    }
    // 0x80019E34: or          $s2, $zero, $zero
    ctx->r18 = 0 | 0;
    // 0x80019E38: andi        $s0, $s4, 0x1F
    ctx->r16 = ctx->r20 & 0X1F;
    // 0x80019E3C: addiu       $s0, $s0, 0x2
    ctx->r16 = ADD32(ctx->r16, 0X2);
    // 0x80019E40: b           L_80019E60
    // 0x80019E44: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
        goto L_80019E60;
    // 0x80019E44: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
L_80019E48:
    // 0x80019E48: lbu         $s0, 0x0($s3)
    ctx->r16 = MEM_BU(ctx->r19, 0X0);
    // 0x80019E4C: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    // 0x80019E50: addiu       $s0, $s0, 0x2
    ctx->r16 = ADD32(ctx->r16, 0X2);
    // 0x80019E54: jal         0x80019F58
    // 0x80019E58: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    func_80019F58(rdram, ctx);
        goto after_19;
    // 0x80019E58: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    after_19:
    // 0x80019E5C: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
L_80019E60:
    // 0x80019E60: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019E64: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019E68: beq         $v1, $zero, L_80019E8C
    if (ctx->r3 == 0) {
        // 0x80019E6C: andi        $s0, $s0, 0xFFFF
        ctx->r16 = ctx->r16 & 0XFFFF;
            goto L_80019E8C;
    }
    // 0x80019E6C: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
L_80019E70:
    // 0x80019E70: sb          $s2, 0x0($s1)
    MEM_B(0X0, ctx->r17) = ctx->r18;
    // 0x80019E74: lhu         $t0, 0x0($s5)
    ctx->r8 = MEM_HU(ctx->r21, 0X0);
    // 0x80019E78: or          $v1, $s0, $zero
    ctx->r3 = ctx->r16 | 0;
    // 0x80019E7C: addiu       $s0, $s0, -0x1
    ctx->r16 = ADD32(ctx->r16, -0X1);
    // 0x80019E80: andi        $s0, $s0, 0xFFFF
    ctx->r16 = ctx->r16 & 0XFFFF;
    // 0x80019E84: bne         $v1, $zero, L_80019E70
    if (ctx->r3 != 0) {
        // 0x80019E88: addu        $s1, $s1, $t0
        ctx->r17 = ADD32(ctx->r17, ctx->r8);
            goto L_80019E70;
    }
    // 0x80019E88: addu        $s1, $s1, $t0
    ctx->r17 = ADD32(ctx->r17, ctx->r8);
L_80019E8C:
    // 0x80019E8C: lw          $v1, 0x0($fp)
    ctx->r3 = MEM_W(ctx->r30, 0X0);
L_80019E90:
    // 0x80019E90: sltu        $at, $v1, $s7
    ctx->r1 = ctx->r3 < ctx->r23 ? 1 : 0;
    // 0x80019E94: bnel        $at, $zero, L_80019D18
    if (ctx->r1 != 0) {
        // 0x80019E98: lbu         $s0, 0x0($s3)
        ctx->r16 = MEM_BU(ctx->r19, 0X0);
            goto L_80019D18;
    }
    goto skip_6;
    // 0x80019E98: lbu         $s0, 0x0($s3)
    ctx->r16 = MEM_BU(ctx->r19, 0X0);
    skip_6:
    // 0x80019E9C: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80019EA0: addiu       $a1, $a1, -0x42C
    ctx->r5 = ADD32(ctx->r5, -0X42C);
    // 0x80019EA4: lhu         $t3, 0x0($a1)
    ctx->r11 = MEM_HU(ctx->r5, 0X0);
L_80019EA8:
    // 0x80019EA8: lhu         $t5, 0x0($s5)
    ctx->r13 = MEM_HU(ctx->r21, 0X0);
    // 0x80019EAC: lw          $t1, 0x40($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X40);
    // 0x80019EB0: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x80019EB4: andi        $v0, $t4, 0xFFFF
    ctx->r2 = ctx->r12 & 0XFFFF;
    // 0x80019EB8: slt         $at, $v0, $t5
    ctx->r1 = SIGNED(ctx->r2) < SIGNED(ctx->r13) ? 1 : 0;
    // 0x80019EBC: addiu       $t2, $t1, 0x1
    ctx->r10 = ADD32(ctx->r9, 0X1);
    // 0x80019EC0: sw          $t2, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r10;
    // 0x80019EC4: bne         $at, $zero, L_80019C90
    if (ctx->r1 != 0) {
        // 0x80019EC8: sh          $t4, 0x0($a1)
        MEM_H(0X0, ctx->r5) = ctx->r12;
            goto L_80019C90;
    }
    // 0x80019EC8: sh          $t4, 0x0($a1)
    MEM_H(0X0, ctx->r5) = ctx->r12;
L_80019ECC:
    // 0x80019ECC: andi        $v1, $s7, 0xF
    ctx->r3 = ctx->r23 & 0XF;
L_80019ED0:
    // 0x80019ED0: beq         $v1, $zero, L_80019EF0
    if (ctx->r3 == 0) {
        // 0x80019ED4: addiu       $s7, $s7, 0x1
        ctx->r23 = ADD32(ctx->r23, 0X1);
            goto L_80019EF0;
    }
    // 0x80019ED4: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
L_80019ED8:
    // 0x80019ED8: jal         0x80019F58
    // 0x80019EDC: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    func_80019F58(rdram, ctx);
        goto after_20;
    // 0x80019EDC: or          $a0, $s3, $zero
    ctx->r4 = ctx->r19 | 0;
    after_20:
    // 0x80019EE0: andi        $v1, $s7, 0xF
    ctx->r3 = ctx->r23 & 0XF;
    // 0x80019EE4: addiu       $s7, $s7, 0x1
    ctx->r23 = ADD32(ctx->r23, 0X1);
    // 0x80019EE8: bne         $v1, $zero, L_80019ED8
    if (ctx->r3 != 0) {
        // 0x80019EEC: or          $s3, $v0, $zero
        ctx->r19 = ctx->r2 | 0;
            goto L_80019ED8;
    }
    // 0x80019EEC: or          $s3, $v0, $zero
    ctx->r19 = ctx->r2 | 0;
L_80019EF0:
    // 0x80019EF0: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x80019EF4: lw          $v0, -0x434($v0)
    ctx->r2 = MEM_W(ctx->r2, -0X434);
    // 0x80019EF8: addiu       $at, $zero, -0x1
    ctx->r1 = ADD32(0, -0X1);
    // 0x80019EFC: lui         $t6, 0x800D
    ctx->r14 = S32(0X800D << 16);
    // 0x80019F00: beq         $v0, $at, L_80019F1C
    if (ctx->r2 == ctx->r1) {
        // 0x80019F04: addiu       $t6, $t6, -0x1898
        ctx->r14 = ADD32(ctx->r14, -0X1898);
            goto L_80019F1C;
    }
    // 0x80019F04: addiu       $t6, $t6, -0x1898
    ctx->r14 = ADD32(ctx->r14, -0X1898);
    // 0x80019F08: lbu         $t7, 0x0($t6)
    ctx->r15 = MEM_BU(ctx->r14, 0X0);
    // 0x80019F0C: bne         $t7, $zero, L_80019F1C
    if (ctx->r15 != 0) {
        // 0x80019F10: nop
    
            goto L_80019F1C;
    }
    // 0x80019F10: nop

    // 0x80019F14: b           L_80019F28
    // 0x80019F18: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_80019F28;
    // 0x80019F18: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80019F1C:
    // 0x80019F1C: bgtzl       $v0, L_80019A50
    if (SIGNED(ctx->r2) > 0) {
        // 0x80019F20: lbu         $s6, 0x0($s3)
        ctx->r22 = MEM_BU(ctx->r19, 0X0);
            goto L_80019A50;
    }
    goto skip_7;
    // 0x80019F20: lbu         $s6, 0x0($s3)
    ctx->r22 = MEM_BU(ctx->r19, 0X0);
    skip_7:
    // 0x80019F24: or          $v0, $s1, $zero
    ctx->r2 = ctx->r17 | 0;
L_80019F28:
    // 0x80019F28: lw          $ra, 0x3C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X3C);
L_80019F2C:
    // 0x80019F2C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80019F30: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80019F34: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80019F38: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x80019F3C: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80019F40: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x80019F44: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x80019F48: lw          $s7, 0x34($sp)
    ctx->r23 = MEM_W(ctx->r29, 0X34);
    // 0x80019F4C: lw          $fp, 0x38($sp)
    ctx->r30 = MEM_W(ctx->r29, 0X38);
    // 0x80019F50: jr          $ra
    // 0x80019F54: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
    return;
    // 0x80019F54: addiu       $sp, $sp, 0x58
    ctx->r29 = ADD32(ctx->r29, 0X58);
;}
RECOMP_FUNC void func_80019F58(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80019F58: lui         $v0, 0x800F
    ctx->r2 = S32(0X800F << 16);
    // 0x80019F5C: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x80019F60: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x80019F64: addiu       $a1, $a1, -0x434
    ctx->r5 = ADD32(ctx->r5, -0X434);
    // 0x80019F68: addiu       $v1, $v1, -0x43C
    ctx->r3 = ADD32(ctx->r3, -0X43C);
    // 0x80019F6C: addiu       $v0, $v0, -0x430
    ctx->r2 = ADD32(ctx->r2, -0X430);
    // 0x80019F70: lw          $t6, 0x0($v1)
    ctx->r14 = MEM_W(ctx->r3, 0X0);
    // 0x80019F74: lw          $t8, 0x0($v0)
    ctx->r24 = MEM_W(ctx->r2, 0X0);
    // 0x80019F78: lw          $t0, 0x0($a1)
    ctx->r8 = MEM_W(ctx->r5, 0X0);
    // 0x80019F7C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80019F80: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80019F84: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x80019F88: addiu       $t1, $t0, -0x1
    ctx->r9 = ADD32(ctx->r8, -0X1);
    // 0x80019F8C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80019F90: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x80019F94: sw          $t9, 0x0($v0)
    MEM_W(0X0, ctx->r2) = ctx->r25;
    // 0x80019F98: beq         $t9, $zero, L_80019FA8
    if (ctx->r25 == 0) {
        // 0x80019F9C: sw          $t1, 0x0($a1)
        MEM_W(0X0, ctx->r5) = ctx->r9;
            goto L_80019FA8;
    }
    // 0x80019F9C: sw          $t1, 0x0($a1)
    MEM_W(0X0, ctx->r5) = ctx->r9;
    // 0x80019FA0: b           L_80019FBC
    // 0x80019FA4: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
        goto L_80019FBC;
    // 0x80019FA4: addiu       $a0, $a0, 0x1
    ctx->r4 = ADD32(ctx->r4, 0X1);
L_80019FA8:
    // 0x80019FA8: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80019FAC: addiu       $a0, $a0, -0x428
    ctx->r4 = ADD32(ctx->r4, -0X428);
    // 0x80019FB0: jal         0x80019FD0
    // 0x80019FB4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80019FD0(rdram, ctx);
        goto after_0;
    // 0x80019FB4: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80019FB8: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
L_80019FBC:
    // 0x80019FBC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80019FC0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80019FC4: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x80019FC8: jr          $ra
    // 0x80019FCC: nop

    return;
    // 0x80019FCC: nop

;}
RECOMP_FUNC void func_80019FD0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80019FD0: lui         $v1, 0x800F
    ctx->r3 = S32(0X800F << 16);
    // 0x80019FD4: addiu       $v1, $v1, -0x438
    ctx->r3 = ADD32(ctx->r3, -0X438);
    // 0x80019FD8: lw          $t7, 0x0($v1)
    ctx->r15 = MEM_W(ctx->r3, 0X0);
    // 0x80019FDC: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x80019FE0: addiu       $t6, $zero, 0x1800
    ctx->r14 = ADD32(0, 0X1800);
    // 0x80019FE4: addiu       $a0, $a0, -0x430
    ctx->r4 = ADD32(ctx->r4, -0X430);
    // 0x80019FE8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80019FEC: subu        $t8, $t7, $t6
    ctx->r24 = SUB32(ctx->r15, ctx->r14);
    // 0x80019FF0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80019FF4: sw          $t6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r14;
    // 0x80019FF8: sw          $t8, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r24;
    // 0x80019FFC: bgez        $t8, L_8001A01C
    if (SIGNED(ctx->r24) >= 0) {
        // 0x8001A000: or          $a2, $t6, $zero
        ctx->r6 = ctx->r14 | 0;
            goto L_8001A01C;
    }
    // 0x8001A000: or          $a2, $t6, $zero
    ctx->r6 = ctx->r14 | 0;
    // 0x8001A004: addiu       $t9, $t8, 0x1
    ctx->r25 = ADD32(ctx->r24, 0X1);
    // 0x8001A008: addiu       $at, $zero, -0x2
    ctx->r1 = ADD32(0, -0X2);
    // 0x8001A00C: and         $t0, $t9, $at
    ctx->r8 = ctx->r25 & ctx->r1;
    // 0x8001A010: addu        $a2, $t6, $t0
    ctx->r6 = ADD32(ctx->r14, ctx->r8);
    // 0x8001A014: sw          $a2, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r6;
    // 0x8001A018: sw          $zero, 0x0($v1)
    MEM_W(0X0, ctx->r3) = 0;
L_8001A01C:
    // 0x8001A01C: blez        $a2, L_8001A03C
    if (SIGNED(ctx->r6) <= 0) {
        // 0x8001A020: lui         $a0, 0x800F
        ctx->r4 = S32(0X800F << 16);
            goto L_8001A03C;
    }
    // 0x8001A020: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001A024: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8001A028: addiu       $a1, $a1, -0x428
    ctx->r5 = ADD32(ctx->r5, -0X428);
    // 0x8001A02C: jal         0x8001A42C
    // 0x8001A030: lw          $a0, -0x440($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X440);
    func_8001A42C(rdram, ctx);
        goto after_0;
    // 0x8001A030: lw          $a0, -0x440($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X440);
    after_0:
    // 0x8001A034: lui         $a2, 0x800F
    ctx->r6 = S32(0X800F << 16);
    // 0x8001A038: lw          $a2, -0x430($a2)
    ctx->r6 = MEM_W(ctx->r6, -0X430);
L_8001A03C:
    // 0x8001A03C: lui         $t2, 0x800F
    ctx->r10 = S32(0X800F << 16);
    // 0x8001A040: lw          $t2, -0x440($t2)
    ctx->r10 = MEM_W(ctx->r10, -0X440);
    // 0x8001A044: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A048: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8001A04C: addu        $t3, $t2, $a2
    ctx->r11 = ADD32(ctx->r10, ctx->r6);
    // 0x8001A050: sw          $t3, -0x440($at)
    MEM_W(-0X440, ctx->r1) = ctx->r11;
    // 0x8001A054: jr          $ra
    // 0x8001A058: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x8001A058: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8001A05C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A05C: addiu       $sp, $sp, -0x80
    ctx->r29 = ADD32(ctx->r29, -0X80);
    // 0x8001A060: sw          $s3, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r19;
    // 0x8001A064: lui         $at, 0x7FFF
    ctx->r1 = S32(0X7FFF << 16);
    // 0x8001A068: lui         $s3, 0x800F
    ctx->r19 = S32(0X800F << 16);
    // 0x8001A06C: sw          $s5, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r21;
    // 0x8001A070: sw          $s2, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r18;
    // 0x8001A074: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x8001A078: ori         $at, $at, 0xFFFF
    ctx->r1 = ctx->r1 | 0XFFFF;
    // 0x8001A07C: sw          $a1, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r5;
    // 0x8001A080: addiu       $s5, $sp, 0x3C
    ctx->r21 = ADD32(ctx->r29, 0X3C);
    // 0x8001A084: addiu       $s3, $s3, -0x428
    ctx->r19 = ADD32(ctx->r19, -0X428);
    // 0x8001A088: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x8001A08C: and         $s2, $a2, $at
    ctx->r18 = ctx->r6 & ctx->r1;
    // 0x8001A090: addiu       $s1, $a0, 0x4
    ctx->r17 = ADD32(ctx->r4, 0X4);
    // 0x8001A094: addiu       $t6, $zero, -0x1
    ctx->r14 = ADD32(0, -0X1);
    // 0x8001A098: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x8001A09C: sw          $s4, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r20;
    // 0x8001A0A0: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x8001A0A4: addiu       $s2, $s2, -0x4
    ctx->r18 = ADD32(ctx->r18, -0X4);
    // 0x8001A0A8: sw          $zero, 0x5C($sp)
    MEM_W(0X5C, ctx->r29) = 0;
    // 0x8001A0AC: sw          $zero, 0x60($sp)
    MEM_W(0X60, ctx->r29) = 0;
    // 0x8001A0B0: sw          $zero, 0x64($sp)
    MEM_W(0X64, ctx->r29) = 0;
    // 0x8001A0B4: sw          $s3, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r19;
    // 0x8001A0B8: sw          $t6, 0x4C($sp)
    MEM_W(0X4C, ctx->r29) = ctx->r14;
    // 0x8001A0BC: addiu       $a1, $a1, -0x2EBC
    ctx->r5 = ADD32(ctx->r5, -0X2EBC);
    // 0x8001A0C0: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x8001A0C4: addiu       $a2, $zero, 0x38
    ctx->r6 = ADD32(0, 0X38);
    // 0x8001A0C8: jal         0x800A75F0
    // 0x8001A0CC: sw          $zero, 0x40($sp)
    MEM_W(0X40, ctx->r29) = 0;
    func_800A75F0(rdram, ctx);
        goto after_0;
    // 0x8001A0CC: sw          $zero, 0x40($sp)
    MEM_W(0X40, ctx->r29) = 0;
    after_0:
    // 0x8001A0D0: beq         $v0, $zero, L_8001A0E0
    if (ctx->r2 == 0) {
        // 0x8001A0D4: lw          $v1, 0x44($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X44);
            goto L_8001A0E0;
    }
    // 0x8001A0D4: lw          $v1, 0x44($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X44);
    // 0x8001A0D8: b           L_8001A184
    // 0x8001A0DC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8001A184;
    // 0x8001A0DC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8001A0E0:
    // 0x8001A0E0: sltu        $at, $v1, $s2
    ctx->r1 = ctx->r3 < ctx->r18 ? 1 : 0;
    // 0x8001A0E4: beq         $at, $zero, L_8001A168
    if (ctx->r1 == 0) {
        // 0x8001A0E8: addiu       $s4, $zero, 0x1
        ctx->r20 = ADD32(0, 0X1);
            goto L_8001A168;
    }
    // 0x8001A0E8: addiu       $s4, $zero, 0x1
    ctx->r20 = ADD32(0, 0X1);
    // 0x8001A0EC: lw          $v0, 0x40($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X40);
L_8001A0F0:
    // 0x8001A0F0: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x8001A0F4: or          $a1, $s3, $zero
    ctx->r5 = ctx->r19 | 0;
    // 0x8001A0F8: beql        $v0, $zero, L_8001A10C
    if (ctx->r2 == 0) {
        // 0x8001A0FC: subu        $v0, $s2, $v1
        ctx->r2 = SUB32(ctx->r18, ctx->r3);
            goto L_8001A10C;
    }
    goto skip_0;
    // 0x8001A0FC: subu        $v0, $s2, $v1
    ctx->r2 = SUB32(ctx->r18, ctx->r3);
    skip_0:
    // 0x8001A100: b           L_8001A134
    // 0x8001A104: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
        goto L_8001A134;
    // 0x8001A104: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x8001A108: subu        $v0, $s2, $v1
    ctx->r2 = SUB32(ctx->r18, ctx->r3);
L_8001A10C:
    // 0x8001A10C: sltiu       $at, $v0, 0x1800
    ctx->r1 = ctx->r2 < 0X1800 ? 1 : 0;
    // 0x8001A110: beq         $at, $zero, L_8001A128
    if (ctx->r1 == 0) {
        // 0x8001A114: addiu       $s0, $zero, 0x1800
        ctx->r16 = ADD32(0, 0X1800);
            goto L_8001A128;
    }
    // 0x8001A114: addiu       $s0, $zero, 0x1800
    ctx->r16 = ADD32(0, 0X1800);
    // 0x8001A118: bne         $v0, $zero, L_8001A128
    if (ctx->r2 != 0) {
        // 0x8001A11C: or          $s0, $v0, $zero
        ctx->r16 = ctx->r2 | 0;
            goto L_8001A128;
    }
    // 0x8001A11C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    // 0x8001A120: b           L_8001A168
    // 0x8001A124: nop

        goto L_8001A168;
    // 0x8001A124: nop

L_8001A128:
    // 0x8001A128: jal         0x8001A42C
    // 0x8001A12C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    func_8001A42C(rdram, ctx);
        goto after_1;
    // 0x8001A12C: or          $a2, $s0, $zero
    ctx->r6 = ctx->r16 | 0;
    after_1:
    // 0x8001A130: sw          $s3, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = ctx->r19;
L_8001A134:
    // 0x8001A134: addu        $s1, $s1, $s0
    ctx->r17 = ADD32(ctx->r17, ctx->r16);
    // 0x8001A138: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    // 0x8001A13C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8001A140: jal         0x800A761C
    // 0x8001A144: sw          $s0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r16;
    func_800A761C(rdram, ctx);
        goto after_2;
    // 0x8001A144: sw          $s0, 0x40($sp)
    MEM_W(0X40, ctx->r29) = ctx->r16;
    after_2:
    // 0x8001A148: beq         $v0, $s4, L_8001A168
    if (ctx->r2 == ctx->r20) {
        // 0x8001A14C: lw          $v1, 0x44($sp)
        ctx->r3 = MEM_W(ctx->r29, 0X44);
            goto L_8001A168;
    }
    // 0x8001A14C: lw          $v1, 0x44($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X44);
    // 0x8001A150: beq         $v0, $zero, L_8001A160
    if (ctx->r2 == 0) {
        // 0x8001A154: sltu        $at, $v1, $s2
        ctx->r1 = ctx->r3 < ctx->r18 ? 1 : 0;
            goto L_8001A160;
    }
    // 0x8001A154: sltu        $at, $v1, $s2
    ctx->r1 = ctx->r3 < ctx->r18 ? 1 : 0;
    // 0x8001A158: b           L_8001A184
    // 0x8001A15C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8001A184;
    // 0x8001A15C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_8001A160:
    // 0x8001A160: bnel        $at, $zero, L_8001A0F0
    if (ctx->r1 != 0) {
        // 0x8001A164: lw          $v0, 0x40($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X40);
            goto L_8001A0F0;
    }
    goto skip_1;
    // 0x8001A164: lw          $v0, 0x40($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X40);
    skip_1:
L_8001A168:
    // 0x8001A168: jal         0x800A7410
    // 0x8001A16C: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    func_800A7410(rdram, ctx);
        goto after_3;
    // 0x8001A16C: or          $a0, $s5, $zero
    ctx->r4 = ctx->r21 | 0;
    after_3:
    // 0x8001A170: beql        $v0, $zero, L_8001A184
    if (ctx->r2 == 0) {
        // 0x8001A174: lw          $v0, 0x48($sp)
        ctx->r2 = MEM_W(ctx->r29, 0X48);
            goto L_8001A184;
    }
    goto skip_2;
    // 0x8001A174: lw          $v0, 0x48($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X48);
    skip_2:
    // 0x8001A178: b           L_8001A184
    // 0x8001A17C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8001A184;
    // 0x8001A17C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x8001A180: lw          $v0, 0x48($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X48);
L_8001A184:
    // 0x8001A184: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    // 0x8001A188: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x8001A18C: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x8001A190: lw          $s2, 0x1C($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A194: lw          $s3, 0x20($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X20);
    // 0x8001A198: lw          $s4, 0x24($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X24);
    // 0x8001A19C: lw          $s5, 0x28($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X28);
    // 0x8001A1A0: jr          $ra
    // 0x8001A1A4: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
    return;
    // 0x8001A1A4: addiu       $sp, $sp, 0x80
    ctx->r29 = ADD32(ctx->r29, 0X80);
;}
RECOMP_FUNC void func_8001A1A8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A1A8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A1AC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A1B0: jal         0x80001918
    // 0x8001A1B4: nop

    func_80001918(rdram, ctx);
        goto after_0;
    // 0x8001A1B4: nop

    after_0:
    // 0x8001A1B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A1BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A1C0: jr          $ra
    // 0x8001A1C4: nop

    return;
    // 0x8001A1C4: nop

;}
RECOMP_FUNC void func_8001A1C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A1C8: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8001A1CC: multu       $a2, $a1
    result = U64(U32(ctx->r6)) * U64(U32(ctx->r5)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8001A1D0: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x8001A1D4: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001A1D8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A1DC: addiu       $t9, $t9, 0x1514
    ctx->r25 = ADD32(ctx->r25, 0X1514);
    // 0x8001A1E0: addiu       $a0, $zero, 0x5
    ctx->r4 = ADD32(0, 0X5);
    // 0x8001A1E4: mflo        $a1
    ctx->r5 = lo;
    // 0x8001A1E8: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x8001A1EC: jalr        $t9
    // 0x8001A1F0: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001A1F0: nop

    after_0:
    // 0x8001A1F4: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A1F8: beq         $v0, $zero, L_8001A210
    if (ctx->r2 == 0) {
        // 0x8001A1FC: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_8001A210;
    }
    // 0x8001A1FC: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x8001A200: jal         0x80000F30
    // 0x8001A204: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    bzero_recomp(rdram, ctx);
        goto after_1;
    // 0x8001A204: sw          $v0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r2;
    after_1:
    // 0x8001A208: b           L_8001A218
    // 0x8001A20C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
        goto L_8001A218;
    // 0x8001A20C: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
L_8001A210:
    // 0x8001A210: swl         $zero, 0x1($zero)
    do_swl(rdram, 0X1, 0, 0);
    // 0x8001A214: swr         $zero, 0x4($zero)
    do_swr(rdram, 0X4, 0, 0);
L_8001A218:
    // 0x8001A218: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A21C: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    // 0x8001A220: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    // 0x8001A224: jr          $ra
    // 0x8001A228: nop

    return;
    // 0x8001A228: nop

;}
RECOMP_FUNC void func_8001A230(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A230: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8001A234: addiu       $a3, $a3, -0x7D40
    ctx->r7 = ADD32(ctx->r7, -0X7D40);
    // 0x8001A238: lbu         $a1, 0x56C($a3)
    ctx->r5 = MEM_BU(ctx->r7, 0X56C);
    // 0x8001A23C: sll         $t6, $a1, 2
    ctx->r14 = S32(ctx->r5 << 2);
    // 0x8001A240: addiu       $t7, $a1, 0x1
    ctx->r15 = ADD32(ctx->r5, 0X1);
    // 0x8001A244: sb          $t7, 0x56C($a3)
    MEM_B(0X56C, ctx->r7) = ctx->r15;
    // 0x8001A248: addu        $v1, $a3, $t6
    ctx->r3 = ADD32(ctx->r7, ctx->r14);
    // 0x8001A24C: sw          $a0, 0xD70($v1)
    MEM_W(0XD70, ctx->r3) = ctx->r4;
    // 0x8001A250: addiu       $v1, $v1, 0xD70
    ctx->r3 = ADD32(ctx->r3, 0XD70);
    // 0x8001A254: jr          $ra
    // 0x8001A258: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
    return;
    // 0x8001A258: lw          $v0, 0x4($a0)
    ctx->r2 = MEM_W(ctx->r4, 0X4);
;}
RECOMP_FUNC void func_8001A25C(uint8_t* rdram, recomp_context* ctx) {
    // DEBUG: Check render list population
    {
        static int rlog = 0; rlog++;
        if (rlog <= 5 || rlog == 50 || (rlog % 1000 == 0)) {
            gpr sys = S32(0x801C82C0);
            fprintf(stderr, "[RENDER] #%d render slots:", rlog);
            int total = 0;
            for (int i = 0; i < 8; i++) {
                uint32_t head = (uint32_t)MEM_W(sys, 0xD70 + i * 4);
                if (head) { fprintf(stderr, " [%d]=0x%08X", i, head); total++; }
            }
            if (total == 0) fprintf(stderr, " (empty!)");
            fprintf(stderr, "\n");
        }
    }
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A25C: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8001A260: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8001A264: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x8001A268: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x8001A26C: sw          $s5, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r21;
    // 0x8001A270: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x8001A274: lui         $s1, 0xFFF
    ctx->r17 = S32(0XFFF << 16);
    // 0x8001A278: lui         $s2, 0x801D
    ctx->r18 = S32(0X801D << 16);
    // 0x8001A27C: lui         $s4, 0x801D
    ctx->r20 = S32(0X801D << 16);
    // 0x8001A280: sw          $ra, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r31;
    // 0x8001A284: sw          $s6, 0x30($sp)
    MEM_W(0X30, ctx->r29) = ctx->r22;
    // 0x8001A288: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001A28C: addiu       $s4, $s4, -0x7D40
    ctx->r20 = ADD32(ctx->r20, -0X7D40);
    // 0x8001A290: addiu       $s2, $s2, -0x7C14
    ctx->r18 = ADD32(ctx->r18, -0X7C14);
    // 0x8001A294: ori         $s1, $s1, 0xFFFF
    ctx->r17 = ctx->r17 | 0XFFFF;
    // 0x8001A298: lui         $s3, 0x4000
    ctx->r19 = S32(0X4000 << 16);
    // 0x8001A29C: or          $s5, $zero, $zero
    ctx->r21 = 0 | 0;
    // 0x8001A2A0: sll         $t6, $s5, 2
    ctx->r14 = S32(ctx->r21 << 2);
L_8001A2A4:
    // 0x8001A2A4: addu        $v0, $s4, $t6
    ctx->r2 = ADD32(ctx->r20, ctx->r14);
    // 0x8001A2A8: lw          $t7, 0xD70($v0)
    ctx->r15 = MEM_W(ctx->r2, 0XD70);
    // 0x8001A2AC: addiu       $s6, $v0, 0xD70
    ctx->r22 = ADD32(ctx->r2, 0XD70);
    // 0x8001A2B0: beql        $t7, $zero, L_8001A314
    if (ctx->r15 == 0) {
        // 0x8001A2B4: addiu       $s5, $s5, 0x1
        ctx->r21 = ADD32(ctx->r21, 0X1);
            goto L_8001A314;
    }
    goto skip_0;
    // 0x8001A2B4: addiu       $s5, $s5, 0x1
    ctx->r21 = ADD32(ctx->r21, 0X1);
    skip_0:
    // 0x8001A2B8: lw          $s0, 0x0($s6)
    ctx->r16 = MEM_W(ctx->r22, 0X0);
L_8001A2BC:
    // 0x8001A2BC: lw          $t8, 0x0($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X0);
    // 0x8001A2C0: or          $a0, $s2, $zero
    ctx->r4 = ctx->r18 | 0;
    // 0x8001A2C4: jal         0x800048C4
    // 0x8001A2C8: and         $a1, $t8, $s1
    ctx->r5 = ctx->r24 & ctx->r17;
    func_800048C4(rdram, ctx);
        goto after_0;
    // 0x8001A2C8: and         $a1, $t8, $s1
    ctx->r5 = ctx->r24 & ctx->r17;
    after_0:
    // 0x8001A2CC: lw          $t0, 0x0($s0)
    ctx->r8 = MEM_W(ctx->r16, 0X0);
    // 0x8001A2D0: lw          $t9, 0x4($s0)
    ctx->r25 = MEM_W(ctx->r16, 0X4);
    // 0x8001A2D4: and         $t1, $t0, $s1
    ctx->r9 = ctx->r8 & ctx->r17;
    // 0x8001A2D8: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x8001A2DC: addu        $t3, $s4, $t2
    ctx->r11 = ADD32(ctx->r20, ctx->r10);
    // 0x8001A2E0: sw          $t9, 0x570($t3)
    MEM_W(0X570, ctx->r11) = ctx->r25;
    // 0x8001A2E4: lw          $t4, 0x0($s0)
    ctx->r12 = MEM_W(ctx->r16, 0X0);
    // 0x8001A2E8: lw          $a1, 0x4($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X4);
    // 0x8001A2EC: jal         0x800183A0
    // 0x8001A2F0: and         $a0, $t4, $s1
    ctx->r4 = ctx->r12 & ctx->r17;
    func_800183A0(rdram, ctx);
        goto after_1;
    // 0x8001A2F0: and         $a0, $t4, $s1
    ctx->r4 = ctx->r12 & ctx->r17;
    after_1:
    // 0x8001A2F4: lw          $t5, 0x0($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X0);
    // 0x8001A2F8: and         $t6, $t5, $s3
    ctx->r14 = ctx->r13 & ctx->r19;
    // 0x8001A2FC: bnel        $t6, $zero, L_8001A310
    if (ctx->r14 != 0) {
        // 0x8001A300: sw          $zero, 0x0($s6)
        MEM_W(0X0, ctx->r22) = 0;
            goto L_8001A310;
    }
    goto skip_1;
    // 0x8001A300: sw          $zero, 0x0($s6)
    MEM_W(0X0, ctx->r22) = 0;
    skip_1:
    // 0x8001A304: b           L_8001A2BC
    // 0x8001A308: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
        goto L_8001A2BC;
    // 0x8001A308: addiu       $s0, $s0, 0x8
    ctx->r16 = ADD32(ctx->r16, 0X8);
    // 0x8001A30C: sw          $zero, 0x0($s6)
    MEM_W(0X0, ctx->r22) = 0;
L_8001A310:
    // 0x8001A310: addiu       $s5, $s5, 0x1
    ctx->r21 = ADD32(ctx->r21, 0X1);
L_8001A314:
    // 0x8001A314: andi        $s5, $s5, 0xFF
    ctx->r21 = ctx->r21 & 0XFF;
    // 0x8001A318: slti        $at, $s5, 0x8
    ctx->r1 = SIGNED(ctx->r21) < 0X8 ? 1 : 0;
    // 0x8001A31C: bnel        $at, $zero, L_8001A2A4
    if (ctx->r1 != 0) {
        // 0x8001A320: sll         $t6, $s5, 2
        ctx->r14 = S32(ctx->r21 << 2);
            goto L_8001A2A4;
    }
    goto skip_2;
    // 0x8001A320: sll         $t6, $s5, 2
    ctx->r14 = S32(ctx->r21 << 2);
    skip_2:
    // 0x8001A324: lw          $ra, 0x34($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X34);
    // 0x8001A328: sb          $zero, 0x56C($s4)
    MEM_B(0X56C, ctx->r20) = 0;
    // 0x8001A32C: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x8001A330: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001A334: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A338: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8001A33C: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8001A340: lw          $s5, 0x2C($sp)
    ctx->r21 = MEM_W(ctx->r29, 0X2C);
    // 0x8001A344: lw          $s6, 0x30($sp)
    ctx->r22 = MEM_W(ctx->r29, 0X30);
    // 0x8001A348: jr          $ra
    // 0x8001A34C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    return;
    // 0x8001A34C: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
;}
RECOMP_FUNC void func_8001A350(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A350: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A354: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A358: jal         0x800A3C70
    // 0x8001A35C: nop

    osEPiLinkHandle(rdram, ctx);
        goto after_0;
    // 0x8001A35C: nop

    after_0:
    // 0x8001A360: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A364: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8001A368: sw          $v0, 0x13F8($at)
    MEM_W(0X13F8, ctx->r1) = ctx->r2;
    // 0x8001A36C: jr          $ra
    // 0x8001A370: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x8001A370: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8001A374(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A374: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001A378: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001A37C: or          $s0, $a2, $zero
    ctx->r16 = ctx->r6 | 0;
    // 0x8001A380: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001A384: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001A388: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x8001A38C: bne         $a1, $zero, L_8001A3C0
    if (ctx->r5 != 0) {
        // 0x8001A390: sw          $a3, 0x2C($sp)
        MEM_W(0X2C, ctx->r29) = ctx->r7;
            goto L_8001A3C0;
    }
    // 0x8001A390: sw          $a3, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r7;
    // 0x8001A394: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x8001A398: jal         0x80099440
    // 0x8001A39C: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    osWritebackDCache_recomp(rdram, ctx);
        goto after_0;
    // 0x8001A39C: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    after_0:
    // 0x8001A3A0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001A3A4: jal         0x800A1490
    // 0x8001A3A8: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    osInvalDCache_recomp(rdram, ctx);
        goto after_1;
    // 0x8001A3A8: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    after_1:
    // 0x8001A3AC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001A3B0: jal         0x800A13E0
    // 0x8001A3B4: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    osInvalICache_recomp(rdram, ctx);
        goto after_2;
    // 0x8001A3B4: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    after_2:
    // 0x8001A3B8: b           L_8001A3CC
    // 0x8001A3BC: nop

        goto L_8001A3CC;
    // 0x8001A3BC: nop

L_8001A3C0:
    // 0x8001A3C0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001A3C4: jal         0x80099440
    // 0x8001A3C8: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    osWritebackDCache_recomp(rdram, ctx);
        goto after_3;
    // 0x8001A3C8: lw          $a1, 0x30($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X30);
    after_3:
L_8001A3CC:
    // 0x8001A3CC: lui         $a1, 0x800F
    ctx->r5 = S32(0X800F << 16);
    // 0x8001A3D0: addiu       $a1, $a1, 0x13E0
    ctx->r5 = ADD32(ctx->r5, 0X13E0);
    // 0x8001A3D4: lw          $t8, 0x2C($sp)
    ctx->r24 = MEM_W(ctx->r29, 0X2C);
    // 0x8001A3D8: lw          $t9, 0x30($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X30);
    // 0x8001A3DC: lui         $t7, 0x800C
    ctx->r15 = S32(0X800C << 16);
    // 0x8001A3E0: addiu       $t7, $t7, 0x5D18
    ctx->r15 = ADD32(ctx->r15, 0X5D18);
    // 0x8001A3E4: sb          $zero, 0x2($a1)
    MEM_B(0X2, ctx->r5) = 0;
    // 0x8001A3E8: sw          $t7, 0x4($a1)
    MEM_W(0X4, ctx->r5) = ctx->r15;
    // 0x8001A3EC: sw          $s0, 0x8($a1)
    MEM_W(0X8, ctx->r5) = ctx->r16;
    // 0x8001A3F0: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    // 0x8001A3F4: lw          $a2, 0x24($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X24);
    // 0x8001A3F8: sw          $t8, 0xC($a1)
    MEM_W(0XC, ctx->r5) = ctx->r24;
    // 0x8001A3FC: jal         0x800A1220
    // 0x8001A400: sw          $t9, 0x10($a1)
    MEM_W(0X10, ctx->r5) = ctx->r25;
    osEPiStartDma_recomp(rdram, ctx);
        goto after_4;
    // 0x8001A400: sw          $t9, 0x10($a1)
    MEM_W(0X10, ctx->r5) = ctx->r25;
    after_4:
    // 0x8001A404: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001A408: addiu       $a0, $a0, 0x5D18
    ctx->r4 = ADD32(ctx->r4, 0X5D18);
    // 0x8001A40C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8001A410: jal         0x80096CE0
    // 0x8001A414: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_5;
    // 0x8001A414: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_5:
    // 0x8001A418: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A41C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001A420: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001A424: jr          $ra
    // 0x8001A428: nop

    return;
    // 0x8001A428: nop

;}
RECOMP_FUNC void func_8001A42C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A42C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001A430: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x8001A434: lw          $t6, 0x28($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X28);
    // 0x8001A438: or          $a3, $a0, $zero
    ctx->r7 = ctx->r4 | 0;
    // 0x8001A43C: or          $a2, $a1, $zero
    ctx->r6 = ctx->r5 | 0;
    // 0x8001A440: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001A444: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x8001A448: lui         $a0, 0x800F
    ctx->r4 = S32(0X800F << 16);
    // 0x8001A44C: lw          $a0, 0x13F8($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X13F8);
    // 0x8001A450: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x8001A454: jal         0x8001A374
    // 0x8001A458: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    func_8001A374(rdram, ctx);
        goto after_0;
    // 0x8001A458: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    after_0:
    // 0x8001A45C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A460: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001A464: jr          $ra
    // 0x8001A468: nop

    return;
    // 0x8001A468: nop

;}
RECOMP_FUNC void func_8001A470(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A470: mtc1        $a1, $f12
    ctx->f12.u32l = ctx->r5;
    // 0x8001A474: mtc1        $a2, $f14
    ctx->f14.u32l = ctx->r6;
    // 0x8001A478: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A47C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A480: mfc1        $a1, $f12
    ctx->r5 = (int32_t)ctx->f12.u32l;
    // 0x8001A484: mfc1        $a2, $f14
    ctx->r6 = (int32_t)ctx->f14.u32l;
    // 0x8001A488: jal         0x800A6A80
    // 0x8001A48C: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    guRotateRPYF(rdram, ctx);
        goto after_0;
    // 0x8001A48C: sw          $a3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r7;
    after_0:
    // 0x8001A490: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A494: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A498: jr          $ra
    // 0x8001A49C: nop

    return;
    // 0x8001A49C: nop

;}
RECOMP_FUNC void func_8001A4A0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A4A0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A4A4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A4A8: jal         0x8009C670
    // 0x8001A4AC: nop

    func_8009C670(rdram, ctx);
        goto after_0;
    // 0x8001A4AC: nop

    after_0:
    // 0x8001A4B0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A4B4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A4B8: jr          $ra
    // 0x8001A4BC: nop

    return;
    // 0x8001A4BC: nop

;}
RECOMP_FUNC void func_8001A4C0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A4C0: sw          $a0, 0x0($sp)
    MEM_W(0X0, ctx->r29) = ctx->r4;
    // 0x8001A4C4: sw          $a1, 0x4($sp)
    MEM_W(0X4, ctx->r29) = ctx->r5;
    // 0x8001A4C8: sw          $a2, 0x8($sp)
    MEM_W(0X8, ctx->r29) = ctx->r6;
    // 0x8001A4CC: jr          $ra
    // 0x8001A4D0: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
    return;
    // 0x8001A4D0: sw          $a3, 0xC($sp)
    MEM_W(0XC, ctx->r29) = ctx->r7;
;}
RECOMP_FUNC void func_8001A4E0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A4E0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x8001A4E4: addiu       $v1, $v1, -0x7D40
    ctx->r3 = ADD32(ctx->r3, -0X7D40);
    // 0x8001A4E8: lh          $t6, 0x28D0($v1)
    ctx->r14 = MEM_H(ctx->r3, 0X28D0);
    // 0x8001A4EC: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x8001A4F0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A4F4: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x8001A4F8: addu        $v0, $v0, $t7
    ctx->r2 = ADD32(ctx->r2, ctx->r15);
    // 0x8001A4FC: lhu         $v0, 0x3760($v0)
    ctx->r2 = MEM_HU(ctx->r2, 0X3760);
    // 0x8001A500: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A504: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001A508: beq         $v0, $zero, L_8001A51C
    if (ctx->r2 == 0) {
        // 0x8001A50C: andi        $a0, $v0, 0xFFFF
        ctx->r4 = ctx->r2 & 0XFFFF;
            goto L_8001A51C;
    }
    // 0x8001A50C: andi        $a0, $v0, 0xFFFF
    ctx->r4 = ctx->r2 & 0XFFFF;
    // 0x8001A510: addiu       $t9, $t9, 0x731C
    ctx->r25 = ADD32(ctx->r25, 0X731C);
    // 0x8001A514: jalr        $t9
    // 0x8001A518: sh          $v0, 0x2904($v1)
    MEM_H(0X2904, ctx->r3) = ctx->r2;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001A518: sh          $v0, 0x2904($v1)
    MEM_H(0X2904, ctx->r3) = ctx->r2;
    after_0:
L_8001A51C:
    // 0x8001A51C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A520: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A524: jr          $ra
    // 0x8001A528: nop

    return;
    // 0x8001A528: nop

;}
RECOMP_FUNC void func_8001A52C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A52C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A530: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A534: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001A538: lui         $a2, 0x802A
    ctx->r6 = S32(0X802A << 16);
    // 0x8001A53C: lui         $a3, 0x801D
    ctx->r7 = S32(0X801D << 16);
    // 0x8001A540: addiu       $a3, $a3, -0x51F8
    ctx->r7 = ADD32(ctx->r7, -0X51F8);
    // 0x8001A544: addiu       $a2, $a2, 0x3B70
    ctx->r6 = ADD32(ctx->r6, 0X3B70);
    // 0x8001A548: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    // 0x8001A54C: jal         0x80011998
    // 0x8001A550: addiu       $a1, $zero, 0xB2
    ctx->r5 = ADD32(0, 0XB2);
    func_80011998(rdram, ctx);
        goto after_0;
    // 0x8001A550: addiu       $a1, $zero, 0xB2
    ctx->r5 = ADD32(0, 0XB2);
    after_0:
    // 0x8001A554: jal         0x80011364
    // 0x8001A558: addiu       $a0, $zero, 0xB2
    ctx->r4 = ADD32(0, 0XB2);
    func_80011364(rdram, ctx);
        goto after_1;
    // 0x8001A558: addiu       $a0, $zero, 0xB2
    ctx->r4 = ADD32(0, 0XB2);
    after_1:
    // 0x8001A55C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A560: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A564: jr          $ra
    // 0x8001A568: nop

    return;
    // 0x8001A568: nop

;}
RECOMP_FUNC void func_8001A56C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A56C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001A570: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001A574: lw          $t6, 0x2B48($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X2B48);
    // 0x8001A578: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A57C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A580: beq         $t6, $zero, L_8001A5AC
    if (ctx->r14 == 0) {
        // 0x8001A584: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_8001A5AC;
    }
    // 0x8001A584: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8001A588: addiu       $t7, $zero, 0x5
    ctx->r15 = ADD32(0, 0X5);
    // 0x8001A58C: sh          $t7, 0x80($v0)
    MEM_H(0X80, ctx->r2) = ctx->r15;
    // 0x8001A590: lh          $t8, 0x80($v0)
    ctx->r24 = MEM_H(ctx->r2, 0X80);
    // 0x8001A594: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001A598: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001A59C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x8001A5A0: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x8001A5A4: jalr        $t9
    // 0x8001A5A8: sh          $t8, 0x82($v0)
    MEM_H(0X82, ctx->r2) = ctx->r24;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001A5A8: sh          $t8, 0x82($v0)
    MEM_H(0X82, ctx->r2) = ctx->r24;
    after_0:
L_8001A5AC:
    // 0x8001A5AC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A5B0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A5B4: jr          $ra
    // 0x8001A5B8: nop

    return;
    // 0x8001A5B8: nop

;}
RECOMP_FUNC void func_8001A5BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A5BC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A5C0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A5C4: lh          $v1, 0xE($a0)
    ctx->r3 = MEM_H(ctx->r4, 0XE);
    // 0x8001A5C8: addiu       $v1, $v1, 0x1
    ctx->r3 = ADD32(ctx->r3, 0X1);
    // 0x8001A5CC: sll         $v1, $v1, 16
    ctx->r3 = S32(ctx->r3 << 16);
    // 0x8001A5D0: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x8001A5D4: sll         $t6, $v1, 1
    ctx->r14 = S32(ctx->r3 << 1);
    // 0x8001A5D8: addu        $v0, $a0, $t6
    ctx->r2 = ADD32(ctx->r4, ctx->r14);
    // 0x8001A5DC: sh          $v1, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r3;
    // 0x8001A5E0: lbu         $t7, 0x8($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X8);
    // 0x8001A5E4: addiu       $t8, $t7, 0x1
    ctx->r24 = ADD32(ctx->r15, 0X1);
    // 0x8001A5E8: sb          $t8, 0x8($v0)
    MEM_B(0X8, ctx->r2) = ctx->r24;
    // 0x8001A5EC: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001A5F0: lbu         $t9, 0x9($v0)
    ctx->r25 = MEM_BU(ctx->r2, 0X9);
    // 0x8001A5F4: sll         $t0, $t9, 2
    ctx->r8 = S32(ctx->r25 << 2);
    // 0x8001A5F8: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001A5FC: addu        $t9, $t9, $t0
    ctx->r25 = ADD32(ctx->r25, ctx->r8);
    // 0x8001A600: lw          $t9, 0x37C4($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X37C4);
    // 0x8001A604: jalr        $t9
    // 0x8001A608: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001A608: nop

    after_0:
    // 0x8001A60C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001A610: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001A614: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001A618: lh          $t1, 0xE($a0)
    ctx->r9 = MEM_H(ctx->r4, 0XE);
    // 0x8001A61C: addiu       $t2, $t1, -0x1
    ctx->r10 = ADD32(ctx->r9, -0X1);
    // 0x8001A620: sh          $t2, 0xE($a0)
    MEM_H(0XE, ctx->r4) = ctx->r10;
    // 0x8001A624: lw          $t3, 0x2B20($v0)
    ctx->r11 = MEM_W(ctx->r2, 0X2B20);
    // 0x8001A628: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A62C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A630: addiu       $t4, $t3, 0x1
    ctx->r12 = ADD32(ctx->r11, 0X1);
    // 0x8001A634: jr          $ra
    // 0x8001A638: sw          $t4, 0x2B20($v0)
    MEM_W(0X2B20, ctx->r2) = ctx->r12;
    return;
    // 0x8001A638: sw          $t4, 0x2B20($v0)
    MEM_W(0X2B20, ctx->r2) = ctx->r12;
;}
RECOMP_FUNC void func_8001A63C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A63C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001A640: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001A644: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x8001A648: lui         $t6, 0x801D
    ctx->r14 = S32(0X801D << 16);
    // 0x8001A64C: addiu       $a0, $a0, -0x5438
    ctx->r4 = ADD32(ctx->r4, -0X5438);
    // 0x8001A650: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001A654: addiu       $t6, $t6, -0x51EC
    ctx->r14 = ADD32(ctx->r14, -0X51EC);
    // 0x8001A658: jal         0x80000F30
    // 0x8001A65C: subu        $a1, $t6, $a0
    ctx->r5 = SUB32(ctx->r14, ctx->r4);
    bzero_recomp(rdram, ctx);
        goto after_0;
    // 0x8001A65C: subu        $a1, $t6, $a0
    ctx->r5 = SUB32(ctx->r14, ctx->r4);
    after_0:
    // 0x8001A660: jal         0x800404D0
    // 0x8001A664: nop

    func_800404D0(rdram, ctx);
        goto after_1;
    // 0x8001A664: nop

    after_1:
    // 0x8001A668: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x8001A66C: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x8001A670: lbu         $t7, 0x92($v0)
    ctx->r15 = MEM_BU(ctx->r2, 0X92);
    // 0x8001A674: lui         $t9, 0x8001
    ctx->r25 = S32(0X8001 << 16);
    // 0x8001A678: addiu       $t9, $t9, 0x558
    ctx->r25 = ADD32(ctx->r25, 0X558);
    // 0x8001A67C: lbu         $a2, 0x90($v0)
    ctx->r6 = MEM_BU(ctx->r2, 0X90);
    // 0x8001A680: lbu         $a3, 0x91($v0)
    ctx->r7 = MEM_BU(ctx->r2, 0X91);
    // 0x8001A684: addiu       $a0, $zero, -0x8000
    ctx->r4 = ADD32(0, -0X8000);
    // 0x8001A688: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x8001A68C: jalr        $t9
    // 0x8001A690: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001A690: sw          $t7, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r15;
    after_2:
    // 0x8001A694: lw          $v0, 0x20($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X20);
    // 0x8001A698: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001A69C: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001A6A0: addiu       $a0, $v0, 0x8
    ctx->r4 = ADD32(ctx->r2, 0X8);
    // 0x8001A6A4: jalr        $t9
    // 0x8001A6A8: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8001A6A8: addiu       $a1, $v0, 0xE
    ctx->r5 = ADD32(ctx->r2, 0XE);
    after_3:
    // 0x8001A6AC: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A6B0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001A6B4: jr          $ra
    // 0x8001A6B8: nop

    return;
    // 0x8001A6B8: nop

;}
RECOMP_FUNC void func_8001A6BC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A6BC: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001A6C0: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001A6C4: or          $s0, $a0, $zero
    ctx->r16 = ctx->r4 | 0;
    // 0x8001A6C8: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001A6CC: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x8001A6D0: lw          $a0, 0x1600($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1600);
    // 0x8001A6D4: jal         0x800116AC
    // 0x8001A6D8: lui         $a1, 0x80
    ctx->r5 = S32(0X80 << 16);
    func_800116AC(rdram, ctx);
        goto after_0;
    // 0x8001A6D8: lui         $a1, 0x80
    ctx->r5 = S32(0X80 << 16);
    after_0:
    // 0x8001A6DC: lui         $at, 0x800F
    ctx->r1 = S32(0X800F << 16);
    // 0x8001A6E0: jal         0x8001A52C
    // 0x8001A6E4: sw          $v0, 0x1480($at)
    MEM_W(0X1480, ctx->r1) = ctx->r2;
    func_8001A52C(rdram, ctx);
        goto after_1;
    // 0x8001A6E4: sw          $v0, 0x1480($at)
    MEM_W(0X1480, ctx->r1) = ctx->r2;
    after_1:
    // 0x8001A6E8: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001A6EC: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001A6F0: addiu       $a0, $s0, 0x8
    ctx->r4 = ADD32(ctx->r16, 0X8);
    // 0x8001A6F4: jalr        $t9
    // 0x8001A6F8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001A6F8: addiu       $a1, $s0, 0xE
    ctx->r5 = ADD32(ctx->r16, 0XE);
    after_2:
    // 0x8001A6FC: lh          $t6, 0xE($s0)
    ctx->r14 = MEM_H(ctx->r16, 0XE);
    // 0x8001A700: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001A704: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x8001A708: sll         $t7, $t6, 1
    ctx->r15 = S32(ctx->r14 << 1);
    // 0x8001A70C: addu        $v0, $s0, $t7
    ctx->r2 = ADD32(ctx->r16, ctx->r15);
    // 0x8001A710: lbu         $t1, 0x9($v0)
    ctx->r9 = MEM_BU(ctx->r2, 0X9);
    // 0x8001A714: lbu         $t8, 0x8($v0)
    ctx->r24 = MEM_BU(ctx->r2, 0X8);
    // 0x8001A718: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001A71C: sll         $t2, $t1, 2
    ctx->r10 = S32(ctx->r9 << 2);
    // 0x8001A720: addiu       $t0, $t8, 0x1
    ctx->r8 = ADD32(ctx->r24, 0X1);
    // 0x8001A724: sb          $t0, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r8;
    // 0x8001A728: addu        $t9, $t9, $t2
    ctx->r25 = ADD32(ctx->r25, ctx->r10);
    // 0x8001A72C: lw          $t9, 0x37C4($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X37C4);
    // 0x8001A730: jalr        $t9
    // 0x8001A734: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8001A734: nop

    after_3:
    // 0x8001A738: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A73C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001A740: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001A744: jr          $ra
    // 0x8001A748: nop

    return;
    // 0x8001A748: nop

;}
RECOMP_FUNC void func_8001A74C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A74C: lui         $t6, 0x8019
    ctx->r14 = S32(0X8019 << 16);
    // 0x8001A750: lw          $t6, 0x27A8($t6)
    ctx->r14 = MEM_W(ctx->r14, 0X27A8);
    // 0x8001A754: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x8001A758: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001A75C: beq         $t6, $zero, L_8001A7B8
    if (ctx->r14 == 0) {
        // 0x8001A760: or          $a2, $a0, $zero
        ctx->r6 = ctx->r4 | 0;
            goto L_8001A7B8;
    }
    // 0x8001A760: or          $a2, $a0, $zero
    ctx->r6 = ctx->r4 | 0;
    // 0x8001A764: lui         $t9, 0x8000
    ctx->r25 = S32(0X8000 << 16);
    // 0x8001A768: addiu       $t9, $t9, 0x1CE8
    ctx->r25 = ADD32(ctx->r25, 0X1CE8);
    // 0x8001A76C: addiu       $a0, $a0, 0x8
    ctx->r4 = ADD32(ctx->r4, 0X8);
    // 0x8001A770: addiu       $a1, $a2, 0xE
    ctx->r5 = ADD32(ctx->r6, 0XE);
    // 0x8001A774: jalr        $t9
    // 0x8001A778: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001A778: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_0:
    // 0x8001A77C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x8001A780: lui         $t9, 0x800B
    ctx->r25 = S32(0X800B << 16);
    // 0x8001A784: lh          $t7, 0xE($a0)
    ctx->r15 = MEM_H(ctx->r4, 0XE);
    // 0x8001A788: sll         $t8, $t7, 1
    ctx->r24 = S32(ctx->r15 << 1);
    // 0x8001A78C: addu        $v0, $a0, $t8
    ctx->r2 = ADD32(ctx->r4, ctx->r24);
    // 0x8001A790: lbu         $t2, 0x9($v0)
    ctx->r10 = MEM_BU(ctx->r2, 0X9);
    // 0x8001A794: lbu         $t0, 0x8($v0)
    ctx->r8 = MEM_BU(ctx->r2, 0X8);
    // 0x8001A798: addiu       $v0, $v0, 0x8
    ctx->r2 = ADD32(ctx->r2, 0X8);
    // 0x8001A79C: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x8001A7A0: addiu       $t1, $t0, 0x1
    ctx->r9 = ADD32(ctx->r8, 0X1);
    // 0x8001A7A4: sb          $t1, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r9;
    // 0x8001A7A8: addu        $t9, $t9, $t3
    ctx->r25 = ADD32(ctx->r25, ctx->r11);
    // 0x8001A7AC: lw          $t9, 0x37C4($t9)
    ctx->r25 = MEM_W(ctx->r25, 0X37C4);
    // 0x8001A7B0: jalr        $t9
    // 0x8001A7B4: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001A7B4: nop

    after_1:
L_8001A7B8:
    // 0x8001A7B8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001A7BC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001A7C0: jr          $ra
    // 0x8001A7C4: nop

    return;
    // 0x8001A7C4: nop

;}
RECOMP_FUNC void func_8001A7C8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001A7C8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001A7CC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001A7D0: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x8001A7D4: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x8001A7D8: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x8001A7DC: lw          $a0, 0x290C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X290C);
    // 0x8001A7E0: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x8001A7E4: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A7E8: jalr        $t9
    // 0x8001A7EC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_0;
    // 0x8001A7EC: nop

    after_0:
    // 0x8001A7F0: lw          $a0, 0x2914($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2914);
    // 0x8001A7F4: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A7F8: jalr        $t9
    // 0x8001A7FC: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_1;
    // 0x8001A7FC: nop

    after_1:
    // 0x8001A800: lw          $a0, 0x291C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X291C);
    // 0x8001A804: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A808: jalr        $t9
    // 0x8001A80C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_2;
    // 0x8001A80C: nop

    after_2:
    // 0x8001A810: lw          $a0, 0x2924($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2924);
    // 0x8001A814: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A818: jalr        $t9
    // 0x8001A81C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_3;
    // 0x8001A81C: nop

    after_3:
    // 0x8001A820: lw          $a0, 0x292C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X292C);
    // 0x8001A824: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A828: jalr        $t9
    // 0x8001A82C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_4;
    // 0x8001A82C: nop

    after_4:
    // 0x8001A830: lw          $a0, 0x2934($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2934);
    // 0x8001A834: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A838: jalr        $t9
    // 0x8001A83C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_5;
    // 0x8001A83C: nop

    after_5:
    // 0x8001A840: lw          $a0, 0x293C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X293C);
    // 0x8001A844: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A848: jalr        $t9
    // 0x8001A84C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_6;
    // 0x8001A84C: nop

    after_6:
    // 0x8001A850: lw          $a0, 0x2944($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2944);
    // 0x8001A854: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A858: jalr        $t9
    // 0x8001A85C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_7;
    // 0x8001A85C: nop

    after_7:
    // 0x8001A860: lw          $a0, 0x294C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X294C);
    // 0x8001A864: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A868: jalr        $t9
    // 0x8001A86C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_8;
    // 0x8001A86C: nop

    after_8:
    // 0x8001A870: lw          $a0, 0x2954($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X2954);
    // 0x8001A874: lw          $t9, 0x10($a0)
    ctx->r25 = MEM_W(ctx->r4, 0X10);
    // 0x8001A878: jalr        $t9
    // 0x8001A87C: nop

    LOOKUP_FUNC(ctx->r25)(rdram, ctx);
        goto after_9;
    // 0x8001A87C: nop

    after_9:
    // 0x8001A880: sw          $zero, 0x291C($s0)
    MEM_W(0X291C, ctx->r16) = 0;
    // 0x8001A884: sw          $zero, 0x2914($s0)
    MEM_W(0X2914, ctx->r16) = 0;
    // 0x8001A888: sw          $zero, 0x290C($s0)
    MEM_W(0X290C, ctx->r16) = 0;
    // 0x8001A88C: sw          $zero, 0x2934($s0)
    MEM_W(0X2934, ctx->r16) = 0;
    // 0x8001A890: sw          $zero, 0x292C($s0)
    MEM_W(0X292C, ctx->r16) = 0;
    // 0x8001A894: sw          $zero, 0x2924($s0)
    MEM_W(0X2924, ctx->r16) = 0;
    // 0x8001A898: sw          $zero, 0x294C($s0)
    MEM_W(0X294C, ctx->r16) = 0;
    // 0x8001A89C: sw          $zero, 0x2944($s0)
    MEM_W(0X2944, ctx->r16) = 0;
    // 0x8001A8A0: sw          $zero, 0x293C($s0)
    MEM_W(0X293C, ctx->r16) = 0;
    // 0x8001A8A4: sw          $zero, 0x2954($s0)
    MEM_W(0X2954, ctx->r16) = 0;
    // 0x8001A8A8: jal         0x80002CE0
    // 0x8001A8AC: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_80002CE0(rdram, ctx);
        goto after_10;
    // 0x8001A8AC: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_10:
    // 0x8001A8B0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x8001A8B4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x8001A8B8: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001A8BC: jr          $ra
    // 0x8001A8C0: nop

    return;
    // 0x8001A8C0: nop

;}
