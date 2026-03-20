#include "recomp.h"
#include "funcs.h"

RECOMP_FUNC void func_80016B48(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016B48: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80016B4C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016B50: jal         0x80016D38
    // 0x80016B54: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    func_80016D38(rdram, ctx);
        goto after_0;
    // 0x80016B54: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    after_0:
    // 0x80016B58: beq         $v0, $zero, L_80016B74
    if (ctx->r2 == 0) {
        // 0x80016B5C: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_80016B74;
    }
    // 0x80016B5C: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80016B60: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80016B64: jal         0x8001674C
    // 0x80016B68: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_8001674C(rdram, ctx);
        goto after_1;
    // 0x80016B68: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_1:
    // 0x80016B6C: b           L_80016BB0
    // 0x80016B70: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80016BB0;
    // 0x80016B70: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80016B74:
    // 0x80016B74: lhu         $a0, 0xC($a2)
    ctx->r4 = MEM_HU(ctx->r6, 0XC);
    // 0x80016B78: jal         0x80015170
    // 0x80016B7C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_80015170(rdram, ctx);
        goto after_2;
    // 0x80016B7C: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_2:
    // 0x80016B80: beq         $v0, $zero, L_80016BA0
    if (ctx->r2 == 0) {
        // 0x80016B84: lw          $a2, 0x18($sp)
        ctx->r6 = MEM_W(ctx->r29, 0X18);
            goto L_80016BA0;
    }
    // 0x80016B84: lw          $a2, 0x18($sp)
    ctx->r6 = MEM_W(ctx->r29, 0X18);
    // 0x80016B88: lhu         $a0, 0xC($a2)
    ctx->r4 = MEM_HU(ctx->r6, 0XC);
    // 0x80016B8C: ori         $a0, $a0, 0x8000
    ctx->r4 = ctx->r4 | 0X8000;
    // 0x80016B90: jal         0x800161D4
    // 0x80016B94: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    func_800161D4(rdram, ctx);
        goto after_3;
    // 0x80016B94: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    after_3:
    // 0x80016B98: b           L_80016BB0
    // 0x80016B9C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80016BB0;
    // 0x80016B9C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80016BA0:
    // 0x80016BA0: or          $a0, $a2, $zero
    ctx->r4 = ctx->r6 | 0;
    // 0x80016BA4: jal         0x8001674C
    // 0x80016BA8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_8001674C(rdram, ctx);
        goto after_4;
    // 0x80016BA8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_4:
    // 0x80016BAC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80016BB0:
    // 0x80016BB0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80016BB4: jr          $ra
    // 0x80016BB8: nop

    return;
    // 0x80016BB8: nop

;}
RECOMP_FUNC void func_80016BBC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016BBC: jr          $ra
    // 0x80016BC0: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
    return;
    // 0x80016BC0: sw          $zero, 0x0($a0)
    MEM_W(0X0, ctx->r4) = 0;
;}
RECOMP_FUNC void func_80016BC4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016BC4: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80016BC8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016BCC: jal         0x80016C28
    // 0x80016BD0: nop

    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x80016BD0: nop

    after_0:
    // 0x80016BD4: beql        $v0, $zero, L_80016C18
    if (ctx->r2 == 0) {
        // 0x80016BD8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80016C18;
    }
    goto skip_0;
    // 0x80016BD8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80016BDC: jal         0x80016C38
    // 0x80016BE0: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    func_80016C38(rdram, ctx);
        goto after_1;
    // 0x80016BE0: sw          $v0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r2;
    after_1:
    // 0x80016BE4: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80016BE8: sltu        $at, $v1, $v0
    ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
    // 0x80016BEC: bnel        $at, $zero, L_80016C18
    if (ctx->r1 != 0) {
        // 0x80016BF0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80016C18;
    }
    goto skip_1;
    // 0x80016BF0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x80016BF4: jal         0x80016C44
    // 0x80016BF8: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    func_80016C44(rdram, ctx);
        goto after_2;
    // 0x80016BF8: sw          $v1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r3;
    after_2:
    // 0x80016BFC: lw          $v1, 0x1C($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X1C);
    // 0x80016C00: sltu        $at, $v1, $v0
    ctx->r1 = ctx->r3 < ctx->r2 ? 1 : 0;
    // 0x80016C04: beql        $at, $zero, L_80016C18
    if (ctx->r1 == 0) {
        // 0x80016C08: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80016C18;
    }
    goto skip_2;
    // 0x80016C08: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_2:
    // 0x80016C0C: b           L_80016C18
    // 0x80016C10: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016C18;
    // 0x80016C10: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80016C14: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80016C18:
    // 0x80016C18: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80016C1C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80016C20: jr          $ra
    // 0x80016C24: nop

    return;
    // 0x80016C24: nop

;}
RECOMP_FUNC void func_80016C28(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016C28: jr          $ra
    // 0x80016C2C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x80016C2C: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
;}
RECOMP_FUNC void func_80016C30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016C30: jr          $ra
    // 0x80016C34: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
    return;
    // 0x80016C34: or          $v0, $a0, $zero
    ctx->r2 = ctx->r4 | 0;
;}
RECOMP_FUNC void func_80016C38(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016C38: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x80016C3C: jr          $ra
    // 0x80016C40: addiu       $v0, $v0, -0x69C
    ctx->r2 = ADD32(ctx->r2, -0X69C);
    return;
    // 0x80016C40: addiu       $v0, $v0, -0x69C
    ctx->r2 = ADD32(ctx->r2, -0X69C);
;}
RECOMP_FUNC void func_80016C44(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016C44: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x80016C48: jr          $ra
    // 0x80016C4C: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
    return;
    // 0x80016C4C: addiu       $v0, $v0, 0x64
    ctx->r2 = ADD32(ctx->r2, 0X64);
;}
RECOMP_FUNC void func_80016C50(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016C50: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80016C54: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80016C58: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80016C5C: jal         0x80016C38
    // 0x80016C60: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    func_80016C38(rdram, ctx);
        goto after_0;
    // 0x80016C60: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    after_0:
    // 0x80016C64: jal         0x80016C44
    // 0x80016C68: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    func_80016C44(rdram, ctx);
        goto after_1;
    // 0x80016C68: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_1:
    // 0x80016C6C: beq         $s0, $v0, L_80016C8C
    if (ctx->r16 == ctx->r2) {
        // 0x80016C70: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_80016C8C;
    }
    // 0x80016C70: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x80016C74: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
L_80016C78:
    // 0x80016C78: jal         0x8001674C
    // 0x80016C7C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    func_8001674C(rdram, ctx);
        goto after_2;
    // 0x80016C7C: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_2:
    // 0x80016C80: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    // 0x80016C84: bnel        $s0, $s1, L_80016C78
    if (ctx->r16 != ctx->r17) {
        // 0x80016C88: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80016C78;
    }
    goto skip_0;
    // 0x80016C88: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    skip_0:
L_80016C8C:
    // 0x80016C8C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80016C90: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80016C94: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80016C98: jr          $ra
    // 0x80016C9C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x80016C9C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80016CA0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016CA0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80016CA4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80016CA8: sw          $s1, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r17;
    // 0x80016CAC: sw          $s0, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r16;
    // 0x80016CB0: jal         0x80016C38
    // 0x80016CB4: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    func_80016C38(rdram, ctx);
        goto after_0;
    // 0x80016CB4: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    after_0:
    // 0x80016CB8: jal         0x80016C44
    // 0x80016CBC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    func_80016C44(rdram, ctx);
        goto after_1;
    // 0x80016CBC: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_1:
    // 0x80016CC0: beq         $s0, $v0, L_80016D10
    if (ctx->r16 == ctx->r2) {
        // 0x80016CC4: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_80016D10;
    }
    // 0x80016CC4: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_80016CC8:
    // 0x80016CC8: jal         0x8001673C
    // 0x80016CCC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8001673C(rdram, ctx);
        goto after_2;
    // 0x80016CCC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x80016CD0: bnel        $v0, $zero, L_80016D08
    if (ctx->r2 != 0) {
        // 0x80016CD4: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_80016D08;
    }
    goto skip_0;
    // 0x80016CD4: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_0:
    // 0x80016CD8: jal         0x8001671C
    // 0x80016CDC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8001671C(rdram, ctx);
        goto after_3;
    // 0x80016CDC: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x80016CE0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80016CE4: jal         0x8001674C
    // 0x80016CE8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_8001674C(rdram, ctx);
        goto after_4;
    // 0x80016CE8: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_4:
    // 0x80016CEC: jal         0x80016C30
    // 0x80016CF0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_80016C30(rdram, ctx);
        goto after_5;
    // 0x80016CF0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_5:
    // 0x80016CF4: lw          $t6, 0x20($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X20);
    // 0x80016CF8: sw          $v0, 0x0($t6)
    MEM_W(0X0, ctx->r14) = ctx->r2;
    // 0x80016CFC: b           L_80016D1C
    // 0x80016D00: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016D1C;
    // 0x80016D00: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80016D04: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
L_80016D08:
    // 0x80016D08: bne         $s0, $s1, L_80016CC8
    if (ctx->r16 != ctx->r17) {
        // 0x80016D0C: nop
    
            goto L_80016CC8;
    }
    // 0x80016D0C: nop

L_80016D10:
    // 0x80016D10: jal         0x80016BBC
    // 0x80016D14: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    func_80016BBC(rdram, ctx);
        goto after_6;
    // 0x80016D14: lw          $a0, 0x20($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X20);
    after_6:
    // 0x80016D18: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80016D1C:
    // 0x80016D1C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80016D20: lw          $s0, 0x14($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X14);
    // 0x80016D24: lw          $s1, 0x18($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X18);
    // 0x80016D28: jr          $ra
    // 0x80016D2C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    return;
    // 0x80016D2C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
;}
RECOMP_FUNC void func_80016D30(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016D30: jr          $ra
    // 0x80016D34: nop

    return;
    // 0x80016D34: nop

;}
RECOMP_FUNC void func_80016D38(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016D38: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80016D3C: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80016D40: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80016D44: or          $s1, $a0, $zero
    ctx->r17 = ctx->r4 | 0;
    // 0x80016D48: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80016D4C: jal         0x80016C38
    // 0x80016D50: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    func_80016C38(rdram, ctx);
        goto after_0;
    // 0x80016D50: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    after_0:
    // 0x80016D54: jal         0x80016C44
    // 0x80016D58: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    func_80016C44(rdram, ctx);
        goto after_1;
    // 0x80016D58: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_1:
    // 0x80016D5C: beq         $s0, $v0, L_80016DBC
    if (ctx->r16 == ctx->r2) {
        // 0x80016D60: or          $s2, $v0, $zero
        ctx->r18 = ctx->r2 | 0;
            goto L_80016DBC;
    }
    // 0x80016D60: or          $s2, $v0, $zero
    ctx->r18 = ctx->r2 | 0;
L_80016D64:
    // 0x80016D64: jal         0x8001673C
    // 0x80016D68: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8001673C(rdram, ctx);
        goto after_2;
    // 0x80016D68: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x80016D6C: beql        $v0, $zero, L_80016DB4
    if (ctx->r2 == 0) {
        // 0x80016D70: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_80016DB4;
    }
    goto skip_0;
    // 0x80016D70: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_0:
    // 0x80016D74: lw          $t6, 0x4($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X4);
    // 0x80016D78: andi        $t7, $t6, 0x2
    ctx->r15 = ctx->r14 & 0X2;
    // 0x80016D7C: bnel        $t7, $zero, L_80016DB4
    if (ctx->r15 != 0) {
        // 0x80016D80: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_80016DB4;
    }
    goto skip_1;
    // 0x80016D80: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_1:
    // 0x80016D84: beql        $s0, $s1, L_80016DB4
    if (ctx->r16 == ctx->r17) {
        // 0x80016D88: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_80016DB4;
    }
    goto skip_2;
    // 0x80016D88: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_2:
    // 0x80016D8C: lw          $t8, 0x8($s0)
    ctx->r24 = MEM_W(ctx->r16, 0X8);
    // 0x80016D90: beql        $t8, $zero, L_80016DB4
    if (ctx->r24 == 0) {
        // 0x80016D94: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_80016DB4;
    }
    goto skip_3;
    // 0x80016D94: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_3:
    // 0x80016D98: lhu         $t9, 0xC($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0XC);
    // 0x80016D9C: lhu         $t0, 0xC($s0)
    ctx->r8 = MEM_HU(ctx->r16, 0XC);
    // 0x80016DA0: bnel        $t9, $t0, L_80016DB4
    if (ctx->r25 != ctx->r8) {
        // 0x80016DA4: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_80016DB4;
    }
    goto skip_4;
    // 0x80016DA4: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_4:
    // 0x80016DA8: b           L_80016DC0
    // 0x80016DAC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016DC0;
    // 0x80016DAC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80016DB0: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
L_80016DB4:
    // 0x80016DB4: bne         $s0, $s2, L_80016D64
    if (ctx->r16 != ctx->r18) {
        // 0x80016DB8: nop
    
            goto L_80016D64;
    }
    // 0x80016DB8: nop

L_80016DBC:
    // 0x80016DBC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80016DC0:
    // 0x80016DC0: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80016DC4: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80016DC8: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80016DCC: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80016DD0: jr          $ra
    // 0x80016DD4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80016DD4: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80016DD8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016DD8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80016DDC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016DE0: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80016DE4: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80016DE8: jal         0x80016CA0
    // 0x80016DEC: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    func_80016CA0(rdram, ctx);
        goto after_0;
    // 0x80016DEC: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    after_0:
    // 0x80016DF0: beql        $v0, $zero, L_80016E38
    if (ctx->r2 == 0) {
        // 0x80016DF4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80016E38;
    }
    goto skip_0;
    // 0x80016DF4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80016DF8: jal         0x80016C28
    // 0x80016DFC: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    func_80016C28(rdram, ctx);
        goto after_1;
    // 0x80016DFC: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    after_1:
    // 0x80016E00: beql        $v0, $zero, L_80016E38
    if (ctx->r2 == 0) {
        // 0x80016E04: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80016E38;
    }
    goto skip_1;
    // 0x80016E04: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80016E08: sb          $zero, 0x0($v0)
    MEM_B(0X0, ctx->r2) = 0;
    // 0x80016E0C: lhu         $t6, 0x22($sp)
    ctx->r14 = MEM_HU(ctx->r29, 0X22);
    // 0x80016E10: addiu       $a0, $v0, 0x10
    ctx->r4 = ADD32(ctx->r2, 0X10);
    // 0x80016E14: sh          $t6, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r14;
    // 0x80016E18: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    // 0x80016E1C: jal         0x80013CF8
    // 0x80016E20: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    func_80013CF8(rdram, ctx);
        goto after_2;
    // 0x80016E20: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    after_2:
    // 0x80016E24: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80016E28: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    // 0x80016E2C: jal         0x80013CF8
    // 0x80016E30: addiu       $a0, $v1, 0x1C
    ctx->r4 = ADD32(ctx->r3, 0X1C);
    func_80013CF8(rdram, ctx);
        goto after_3;
    // 0x80016E30: addiu       $a0, $v1, 0x1C
    ctx->r4 = ADD32(ctx->r3, 0X1C);
    after_3:
    // 0x80016E34: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80016E38:
    // 0x80016E38: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80016E3C: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80016E40: jr          $ra
    // 0x80016E44: nop

    return;
    // 0x80016E44: nop

;}
RECOMP_FUNC void func_80016E48(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016E48: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80016E4C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016E50: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80016E54: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    // 0x80016E58: sw          $a2, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r6;
    // 0x80016E5C: jal         0x80016CA0
    // 0x80016E60: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    func_80016CA0(rdram, ctx);
        goto after_0;
    // 0x80016E60: addiu       $a0, $sp, 0x1C
    ctx->r4 = ADD32(ctx->r29, 0X1C);
    after_0:
    // 0x80016E64: beql        $v0, $zero, L_80016EAC
    if (ctx->r2 == 0) {
        // 0x80016E68: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80016EAC;
    }
    goto skip_0;
    // 0x80016E68: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80016E6C: jal         0x80016C28
    // 0x80016E70: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    func_80016C28(rdram, ctx);
        goto after_1;
    // 0x80016E70: lw          $a0, 0x1C($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X1C);
    after_1:
    // 0x80016E74: beq         $v0, $zero, L_80016EA8
    if (ctx->r2 == 0) {
        // 0x80016E78: addiu       $t6, $zero, 0x1
        ctx->r14 = ADD32(0, 0X1);
            goto L_80016EA8;
    }
    // 0x80016E78: addiu       $t6, $zero, 0x1
    ctx->r14 = ADD32(0, 0X1);
    // 0x80016E7C: sb          $t6, 0x0($v0)
    MEM_B(0X0, ctx->r2) = ctx->r14;
    // 0x80016E80: lhu         $t7, 0x22($sp)
    ctx->r15 = MEM_HU(ctx->r29, 0X22);
    // 0x80016E84: addiu       $a0, $v0, 0x10
    ctx->r4 = ADD32(ctx->r2, 0X10);
    // 0x80016E88: sh          $t7, 0xC($v0)
    MEM_H(0XC, ctx->r2) = ctx->r15;
    // 0x80016E8C: sw          $v0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r2;
    // 0x80016E90: jal         0x80013CF8
    // 0x80016E94: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    func_80013CF8(rdram, ctx);
        goto after_2;
    // 0x80016E94: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    after_2:
    // 0x80016E98: lw          $v1, 0x18($sp)
    ctx->r3 = MEM_W(ctx->r29, 0X18);
    // 0x80016E9C: lw          $a1, 0x28($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X28);
    // 0x80016EA0: jal         0x80013CF8
    // 0x80016EA4: addiu       $a0, $v1, 0x1C
    ctx->r4 = ADD32(ctx->r3, 0X1C);
    func_80013CF8(rdram, ctx);
        goto after_3;
    // 0x80016EA4: addiu       $a0, $v1, 0x1C
    ctx->r4 = ADD32(ctx->r3, 0X1C);
    after_3:
L_80016EA8:
    // 0x80016EA8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80016EAC:
    // 0x80016EAC: lw          $v0, 0x1C($sp)
    ctx->r2 = MEM_W(ctx->r29, 0X1C);
    // 0x80016EB0: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80016EB4: jr          $ra
    // 0x80016EB8: nop

    return;
    // 0x80016EB8: nop

;}
RECOMP_FUNC void func_80016EBC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016EBC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80016EC0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016EC4: jal         0x80016C28
    // 0x80016EC8: nop

    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x80016EC8: nop

    after_0:
    // 0x80016ECC: beq         $v0, $zero, L_80016EE4
    if (ctx->r2 == 0) {
        // 0x80016ED0: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_80016EE4;
    }
    // 0x80016ED0: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x80016ED4: jal         0x80016B38
    // 0x80016ED8: nop

    func_80016B38(rdram, ctx);
        goto after_1;
    // 0x80016ED8: nop

    after_1:
    // 0x80016EDC: b           L_80016EE8
    // 0x80016EE0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016EE8;
    // 0x80016EE0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80016EE4:
    // 0x80016EE4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80016EE8:
    // 0x80016EE8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80016EEC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80016EF0: jr          $ra
    // 0x80016EF4: nop

    return;
    // 0x80016EF4: nop

;}
RECOMP_FUNC void func_80016EF8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016EF8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80016EFC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016F00: jal         0x80016C28
    // 0x80016F04: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x80016F04: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x80016F08: beq         $v0, $zero, L_80016F44
    if (ctx->r2 == 0) {
        // 0x80016F0C: lw          $t6, 0x1C($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X1C);
            goto L_80016F44;
    }
    // 0x80016F0C: lw          $t6, 0x1C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X1C);
    // 0x80016F10: beql        $t6, $zero, L_80016F34
    if (ctx->r14 == 0) {
        // 0x80016F14: lw          $t9, 0x4($v0)
        ctx->r25 = MEM_W(ctx->r2, 0X4);
            goto L_80016F34;
    }
    goto skip_0;
    // 0x80016F14: lw          $t9, 0x4($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X4);
    skip_0:
    // 0x80016F18: lw          $t7, 0x4($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X4);
    // 0x80016F1C: addiu       $at, $zero, -0x81
    ctx->r1 = ADD32(0, -0X81);
    // 0x80016F20: and         $t8, $t7, $at
    ctx->r24 = ctx->r15 & ctx->r1;
    // 0x80016F24: sw          $t8, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r24;
    // 0x80016F28: b           L_80016F48
    // 0x80016F2C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016F48;
    // 0x80016F2C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80016F30: lw          $t9, 0x4($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X4);
L_80016F34:
    // 0x80016F34: ori         $t0, $t9, 0x80
    ctx->r8 = ctx->r25 | 0X80;
    // 0x80016F38: sw          $t0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r8;
    // 0x80016F3C: b           L_80016F48
    // 0x80016F40: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016F48;
    // 0x80016F40: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80016F44:
    // 0x80016F44: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80016F48:
    // 0x80016F48: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80016F4C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80016F50: jr          $ra
    // 0x80016F54: nop

    return;
    // 0x80016F54: nop

;}
RECOMP_FUNC void func_80016F58(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016F58: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80016F5C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016F60: jal         0x80016C28
    // 0x80016F64: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x80016F64: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x80016F68: beq         $v0, $zero, L_80016FA4
    if (ctx->r2 == 0) {
        // 0x80016F6C: lw          $t6, 0x1C($sp)
        ctx->r14 = MEM_W(ctx->r29, 0X1C);
            goto L_80016FA4;
    }
    // 0x80016F6C: lw          $t6, 0x1C($sp)
    ctx->r14 = MEM_W(ctx->r29, 0X1C);
    // 0x80016F70: beql        $t6, $zero, L_80016F90
    if (ctx->r14 == 0) {
        // 0x80016F74: lw          $t9, 0x4($v0)
        ctx->r25 = MEM_W(ctx->r2, 0X4);
            goto L_80016F90;
    }
    goto skip_0;
    // 0x80016F74: lw          $t9, 0x4($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X4);
    skip_0:
    // 0x80016F78: lw          $t7, 0x4($v0)
    ctx->r15 = MEM_W(ctx->r2, 0X4);
    // 0x80016F7C: ori         $t8, $t7, 0x4
    ctx->r24 = ctx->r15 | 0X4;
    // 0x80016F80: sw          $t8, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r24;
    // 0x80016F84: b           L_80016FA8
    // 0x80016F88: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016FA8;
    // 0x80016F88: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80016F8C: lw          $t9, 0x4($v0)
    ctx->r25 = MEM_W(ctx->r2, 0X4);
L_80016F90:
    // 0x80016F90: addiu       $at, $zero, -0x5
    ctx->r1 = ADD32(0, -0X5);
    // 0x80016F94: and         $t0, $t9, $at
    ctx->r8 = ctx->r25 & ctx->r1;
    // 0x80016F98: sw          $t0, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r8;
    // 0x80016F9C: b           L_80016FA8
    // 0x80016FA0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016FA8;
    // 0x80016FA0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80016FA4:
    // 0x80016FA4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80016FA8:
    // 0x80016FA8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80016FAC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80016FB0: jr          $ra
    // 0x80016FB4: nop

    return;
    // 0x80016FB4: nop

;}
RECOMP_FUNC void func_80016FB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80016FB8: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80016FBC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80016FC0: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80016FC4: jal         0x80016C28
    // 0x80016FC8: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x80016FC8: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_0:
    // 0x80016FCC: beql        $v0, $zero, L_80016FFC
    if (ctx->r2 == 0) {
        // 0x80016FD0: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80016FFC;
    }
    goto skip_0;
    // 0x80016FD0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80016FD4: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    // 0x80016FD8: ori         $t7, $t6, 0x8
    ctx->r15 = ctx->r14 | 0X8;
    // 0x80016FDC: sw          $t7, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r15;
    // 0x80016FE0: lwc1        $f4, 0x1C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x80016FE4: swc1        $f4, 0x28($v0)
    MEM_W(0X28, ctx->r2) = ctx->f4.u32l;
    // 0x80016FE8: lwc1        $f6, 0x20($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X20);
    // 0x80016FEC: swc1        $f6, 0x2C($v0)
    MEM_W(0X2C, ctx->r2) = ctx->f6.u32l;
    // 0x80016FF0: b           L_80016FFC
    // 0x80016FF4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80016FFC;
    // 0x80016FF4: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80016FF8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80016FFC:
    // 0x80016FFC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80017000: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80017004: jr          $ra
    // 0x80017008: nop

    return;
    // 0x80017008: nop

;}
RECOMP_FUNC void func_8001700C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001700C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017010: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80017014: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    // 0x80017018: jal         0x80016C28
    // 0x8001701C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x8001701C: sw          $a2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r6;
    after_0:
    // 0x80017020: beql        $v0, $zero, L_80017050
    if (ctx->r2 == 0) {
        // 0x80017024: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80017050;
    }
    goto skip_0;
    // 0x80017024: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80017028: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    // 0x8001702C: ori         $t7, $t6, 0x10
    ctx->r15 = ctx->r14 | 0X10;
    // 0x80017030: sw          $t7, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r15;
    // 0x80017034: lwc1        $f4, 0x1C($sp)
    ctx->f4.u32l = MEM_W(ctx->r29, 0X1C);
    // 0x80017038: swc1        $f4, 0x30($v0)
    MEM_W(0X30, ctx->r2) = ctx->f4.u32l;
    // 0x8001703C: lwc1        $f6, 0x20($sp)
    ctx->f6.u32l = MEM_W(ctx->r29, 0X20);
    // 0x80017040: swc1        $f6, 0x34($v0)
    MEM_W(0X34, ctx->r2) = ctx->f6.u32l;
    // 0x80017044: b           L_80017050
    // 0x80017048: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80017050;
    // 0x80017048: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x8001704C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80017050:
    // 0x80017050: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80017054: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80017058: jr          $ra
    // 0x8001705C: nop

    return;
    // 0x8001705C: nop

;}
RECOMP_FUNC void func_80017060(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017060: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017064: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80017068: jal         0x80016C28
    // 0x8001706C: nop

    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x8001706C: nop

    after_0:
    // 0x80017070: beql        $v0, $zero, L_80017094
    if (ctx->r2 == 0) {
        // 0x80017074: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80017094;
    }
    goto skip_0;
    // 0x80017074: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80017078: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    // 0x8001707C: addiu       $at, $zero, -0x9
    ctx->r1 = ADD32(0, -0X9);
    // 0x80017080: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x80017084: sw          $t7, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r15;
    // 0x80017088: b           L_80017094
    // 0x8001708C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80017094;
    // 0x8001708C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80017090: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80017094:
    // 0x80017094: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80017098: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x8001709C: jr          $ra
    // 0x800170A0: nop

    return;
    // 0x800170A0: nop

;}
RECOMP_FUNC void func_800170A4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800170A4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800170A8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800170AC: jal         0x80016C28
    // 0x800170B0: nop

    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x800170B0: nop

    after_0:
    // 0x800170B4: beql        $v0, $zero, L_800170D8
    if (ctx->r2 == 0) {
        // 0x800170B8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_800170D8;
    }
    goto skip_0;
    // 0x800170B8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x800170BC: lw          $t6, 0x4($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X4);
    // 0x800170C0: addiu       $at, $zero, -0x11
    ctx->r1 = ADD32(0, -0X11);
    // 0x800170C4: and         $t7, $t6, $at
    ctx->r15 = ctx->r14 & ctx->r1;
    // 0x800170C8: sw          $t7, 0x4($v0)
    MEM_W(0X4, ctx->r2) = ctx->r15;
    // 0x800170CC: b           L_800170D8
    // 0x800170D0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_800170D8;
    // 0x800170D0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800170D4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800170D8:
    // 0x800170D8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800170DC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800170E0: jr          $ra
    // 0x800170E4: nop

    return;
    // 0x800170E4: nop

;}
RECOMP_FUNC void func_800170E8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800170E8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800170EC: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800170F0: jal         0x80016C28
    // 0x800170F4: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x800170F4: sw          $a1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r5;
    after_0:
    // 0x800170F8: beql        $v0, $zero, L_80017134
    if (ctx->r2 == 0) {
        // 0x800170FC: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80017134;
    }
    goto skip_0;
    // 0x800170FC: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80017100: lbu         $t6, 0x0($v0)
    ctx->r14 = MEM_BU(ctx->r2, 0X0);
    // 0x80017104: addiu       $a2, $v0, 0x10
    ctx->r6 = ADD32(ctx->r2, 0X10);
    // 0x80017108: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x8001710C: bne         $t6, $zero, L_80017130
    if (ctx->r14 != 0) {
        // 0x80017110: addiu       $a0, $v0, 0x1C
        ctx->r4 = ADD32(ctx->r2, 0X1C);
            goto L_80017130;
    }
    // 0x80017110: addiu       $a0, $v0, 0x1C
    ctx->r4 = ADD32(ctx->r2, 0X1C);
    // 0x80017114: jal         0x80013CF8
    // 0x80017118: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    func_80013CF8(rdram, ctx);
        goto after_1;
    // 0x80017118: sw          $a2, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r6;
    after_1:
    // 0x8001711C: lw          $a0, 0x18($sp)
    ctx->r4 = MEM_W(ctx->r29, 0X18);
    // 0x80017120: jal         0x80013CF8
    // 0x80017124: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    func_80013CF8(rdram, ctx);
        goto after_2;
    // 0x80017124: lw          $a1, 0x24($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X24);
    after_2:
    // 0x80017128: b           L_80017134
    // 0x8001712C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80017134;
    // 0x8001712C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80017130:
    // 0x80017130: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80017134:
    // 0x80017134: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80017138: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001713C: jr          $ra
    // 0x80017140: nop

    return;
    // 0x80017140: nop

;}
RECOMP_FUNC void func_80017144(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017144: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017148: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001714C: jal         0x80016C28
    // 0x80017150: nop

    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x80017150: nop

    after_0:
    // 0x80017154: beq         $v0, $zero, L_80017170
    if (ctx->r2 == 0) {
        // 0x80017158: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80017170;
    }
    // 0x80017158: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001715C: lw          $t6, 0x8($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X8);
    // 0x80017160: beql        $t6, $zero, L_80017174
    if (ctx->r14 == 0) {
        // 0x80017164: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_80017174;
    }
    goto skip_0;
    // 0x80017164: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
    // 0x80017168: b           L_80017174
    // 0x8001716C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_80017174;
    // 0x8001716C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80017170:
    // 0x80017170: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80017174:
    // 0x80017174: jr          $ra
    // 0x80017178: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80017178: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_8001717C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001717C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017180: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80017184: jal         0x80016BC4
    // 0x80017188: nop

    func_80016BC4(rdram, ctx);
        goto after_0;
    // 0x80017188: nop

    after_0:
    // 0x8001718C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80017190: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80017194: jr          $ra
    // 0x80017198: nop

    return;
    // 0x80017198: nop

;}
RECOMP_FUNC void func_8001719C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001719C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800171A0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800171A4: jal         0x80016C28
    // 0x800171A8: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x800171A8: sw          $a1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r5;
    after_0:
    // 0x800171AC: beq         $v0, $zero, L_800171BC
    if (ctx->r2 == 0) {
        // 0x800171B0: or          $a0, $v0, $zero
        ctx->r4 = ctx->r2 | 0;
            goto L_800171BC;
    }
    // 0x800171B0: or          $a0, $v0, $zero
    ctx->r4 = ctx->r2 | 0;
    // 0x800171B4: jal         0x800168AC
    // 0x800171B8: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    func_800168AC(rdram, ctx);
        goto after_1;
    // 0x800171B8: lw          $a1, 0x1C($sp)
    ctx->r5 = MEM_W(ctx->r29, 0X1C);
    after_1:
L_800171BC:
    // 0x800171BC: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800171C0: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800171C4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x800171C8: jr          $ra
    // 0x800171CC: nop

    return;
    // 0x800171CC: nop

;}
RECOMP_FUNC void func_800171D0(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800171D0: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800171D4: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800171D8: jal         0x80016C28
    // 0x800171DC: nop

    func_80016C28(rdram, ctx);
        goto after_0;
    // 0x800171DC: nop

    after_0:
    // 0x800171E0: beq         $v0, $zero, L_800171F8
    if (ctx->r2 == 0) {
        // 0x800171E4: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_800171F8;
    }
    // 0x800171E4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800171E8: lw          $v0, 0x4($v0)
    ctx->r2 = MEM_W(ctx->r2, 0X4);
    // 0x800171EC: andi        $v0, $v0, 0x40
    ctx->r2 = ctx->r2 & 0X40;
    // 0x800171F0: b           L_800171FC
    // 0x800171F4: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
        goto L_800171FC;
    // 0x800171F4: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
L_800171F8:
    // 0x800171F8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800171FC:
    // 0x800171FC: jr          $ra
    // 0x80017200: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    return;
    // 0x80017200: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
;}
RECOMP_FUNC void func_80017204(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017204: jr          $ra
    // 0x80017208: nop

    return;
    // 0x80017208: nop

;}
RECOMP_FUNC void func_8001720C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001720C: addiu       $sp, $sp, -0x28
    ctx->r29 = ADD32(ctx->r29, -0X28);
    // 0x80017210: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x80017214: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80017218: or          $s2, $a0, $zero
    ctx->r18 = ctx->r4 | 0;
    // 0x8001721C: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80017220: jal         0x80016C38
    // 0x80017224: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    func_80016C38(rdram, ctx);
        goto after_0;
    // 0x80017224: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    after_0:
    // 0x80017228: jal         0x80016C44
    // 0x8001722C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    func_80016C44(rdram, ctx);
        goto after_1;
    // 0x8001722C: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_1:
    // 0x80017230: beq         $s0, $v0, L_8001725C
    if (ctx->r16 == ctx->r2) {
        // 0x80017234: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_8001725C;
    }
    // 0x80017234: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
L_80017238:
    // 0x80017238: jal         0x8001673C
    // 0x8001723C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8001673C(rdram, ctx);
        goto after_2;
    // 0x8001723C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_2:
    // 0x80017240: beq         $v0, $zero, L_80017250
    if (ctx->r2 == 0) {
        // 0x80017244: or          $a0, $s0, $zero
        ctx->r4 = ctx->r16 | 0;
            goto L_80017250;
    }
    // 0x80017244: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80017248: jal         0x800168AC
    // 0x8001724C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    func_800168AC(rdram, ctx);
        goto after_3;
    // 0x8001724C: or          $a1, $s2, $zero
    ctx->r5 = ctx->r18 | 0;
    after_3:
L_80017250:
    // 0x80017250: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    // 0x80017254: bne         $s0, $s1, L_80017238
    if (ctx->r16 != ctx->r17) {
        // 0x80017258: nop
    
            goto L_80017238;
    }
    // 0x80017258: nop

L_8001725C:
    // 0x8001725C: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80017260: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80017264: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80017268: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x8001726C: jr          $ra
    // 0x80017270: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
    return;
    // 0x80017270: addiu       $sp, $sp, 0x28
    ctx->r29 = ADD32(ctx->r29, 0X28);
;}
RECOMP_FUNC void func_80017274(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017274: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017278: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x8001727C: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80017280: jal         0x8001738C
    // 0x80017284: lhu         $a0, 0x1A($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1A);
    func_8001738C(rdram, ctx);
        goto after_0;
    // 0x80017284: lhu         $a0, 0x1A($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1A);
    after_0:
    // 0x80017288: bne         $v0, $zero, L_80017298
    if (ctx->r2 != 0) {
        // 0x8001728C: lhu         $t6, 0x1A($sp)
        ctx->r14 = MEM_HU(ctx->r29, 0X1A);
            goto L_80017298;
    }
    // 0x8001728C: lhu         $t6, 0x1A($sp)
    ctx->r14 = MEM_HU(ctx->r29, 0X1A);
    // 0x80017290: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x80017294: sh          $t6, -0x9DC($at)
    MEM_H(-0X9DC, ctx->r1) = ctx->r14;
L_80017298:
    // 0x80017298: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x8001729C: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800172A0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800172A4: jr          $ra
    // 0x800172A8: nop

    return;
    // 0x800172A8: nop

;}
RECOMP_FUNC void func_800172AC(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800172AC: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800172B0: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800172B4: jal         0x8001759C
    // 0x800172B8: nop

    func_8001759C(rdram, ctx);
        goto after_0;
    // 0x800172B8: nop

    after_0:
    // 0x800172BC: beq         $v0, $zero, L_800172D0
    if (ctx->r2 == 0) {
        // 0x800172C0: lui         $at, 0x800B
        ctx->r1 = S32(0X800B << 16);
            goto L_800172D0;
    }
    // 0x800172C0: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x800172C4: sh          $zero, -0x9DC($at)
    MEM_H(-0X9DC, ctx->r1) = 0;
    // 0x800172C8: b           L_800172D4
    // 0x800172CC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_800172D4;
    // 0x800172CC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800172D0:
    // 0x800172D0: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800172D4:
    // 0x800172D4: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800172D8: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800172DC: jr          $ra
    // 0x800172E0: nop

    return;
    // 0x800172E0: nop

;}
RECOMP_FUNC void func_800172E4(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800172E4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800172E8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800172EC: jal         0x80017600
    // 0x800172F0: nop

    func_80017600(rdram, ctx);
        goto after_0;
    // 0x800172F0: nop

    after_0:
    // 0x800172F4: beq         $v0, $zero, L_80017308
    if (ctx->r2 == 0) {
        // 0x800172F8: lui         $at, 0x800B
        ctx->r1 = S32(0X800B << 16);
            goto L_80017308;
    }
    // 0x800172F8: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x800172FC: sh          $zero, -0x9DC($at)
    MEM_H(-0X9DC, ctx->r1) = 0;
    // 0x80017300: b           L_8001730C
    // 0x80017304: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_8001730C;
    // 0x80017304: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_80017308:
    // 0x80017308: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8001730C:
    // 0x8001730C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80017310: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80017314: jr          $ra
    // 0x80017318: nop

    return;
    // 0x80017318: nop

;}
RECOMP_FUNC void func_8001731C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001731C: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017320: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80017324: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x80017328: jal         0x8001738C
    // 0x8001732C: lhu         $a0, 0x1A($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1A);
    func_8001738C(rdram, ctx);
        goto after_0;
    // 0x8001732C: lhu         $a0, 0x1A($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1A);
    after_0:
    // 0x80017330: bnel        $v0, $zero, L_8001736C
    if (ctx->r2 != 0) {
        // 0x80017334: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_8001736C;
    }
    goto skip_0;
    // 0x80017334: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_0:
    // 0x80017338: jal         0x800172E4
    // 0x8001733C: nop

    func_800172E4(rdram, ctx);
        goto after_1;
    // 0x8001733C: nop

    after_1:
    // 0x80017340: bne         $v0, $zero, L_80017350
    if (ctx->r2 != 0) {
        // 0x80017344: nop
    
            goto L_80017350;
    }
    // 0x80017344: nop

    // 0x80017348: b           L_8001736C
    // 0x8001734C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8001736C;
    // 0x8001734C: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_80017350:
    // 0x80017350: jal         0x80017274
    // 0x80017354: lhu         $a0, 0x1A($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1A);
    func_80017274(rdram, ctx);
        goto after_2;
    // 0x80017354: lhu         $a0, 0x1A($sp)
    ctx->r4 = MEM_HU(ctx->r29, 0X1A);
    after_2:
    // 0x80017358: bnel        $v0, $zero, L_8001736C
    if (ctx->r2 != 0) {
        // 0x8001735C: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_8001736C;
    }
    goto skip_1;
    // 0x8001735C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_1:
    // 0x80017360: b           L_8001736C
    // 0x80017364: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_8001736C;
    // 0x80017364: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    // 0x80017368: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_8001736C:
    // 0x8001736C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x80017370: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80017374: jr          $ra
    // 0x80017378: nop

    return;
    // 0x80017378: nop

;}
RECOMP_FUNC void func_8001737C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001737C: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x80017380: lhu         $v0, -0x9E0($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X9E0);
    // 0x80017384: jr          $ra
    // 0x80017388: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
    return;
    // 0x80017388: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
;}
RECOMP_FUNC void func_8001738C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001738C: lui         $a2, 0x800B
    ctx->r6 = S32(0X800B << 16);
    // 0x80017390: lhu         $a2, -0x9E0($a2)
    ctx->r6 = MEM_HU(ctx->r6, -0X9E0);
    // 0x80017394: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017398: sw          $a0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r4;
    // 0x8001739C: xori        $v0, $a2, 0x18
    ctx->r2 = ctx->r6 ^ 0X18;
    // 0x800173A0: sltiu       $v1, $v0, 0x1
    ctx->r3 = ctx->r2 < 0X1 ? 1 : 0;
    // 0x800173A4: andi        $a0, $a0, 0xFFFF
    ctx->r4 = ctx->r4 & 0XFFFF;
    // 0x800173A8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800173AC: beq         $v1, $zero, L_800173BC
    if (ctx->r3 == 0) {
        // 0x800173B0: or          $a1, $a2, $zero
        ctx->r5 = ctx->r6 | 0;
            goto L_800173BC;
    }
    // 0x800173B0: or          $a1, $a2, $zero
    ctx->r5 = ctx->r6 | 0;
    // 0x800173B4: xori        $v1, $a0, 0x17
    ctx->r3 = ctx->r4 ^ 0X17;
    // 0x800173B8: sltiu       $v1, $v1, 0x1
    ctx->r3 = ctx->r3 < 0X1 ? 1 : 0;
L_800173BC:
    // 0x800173BC: beq         $a0, $a1, L_800173CC
    if (ctx->r4 == ctx->r5) {
        // 0x800173C0: nop
    
            goto L_800173CC;
    }
    // 0x800173C0: nop

    // 0x800173C4: beql        $v1, $zero, L_800173E8
    if (ctx->r3 == 0) {
        // 0x800173C8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_800173E8;
    }
    goto skip_0;
    // 0x800173C8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_0:
L_800173CC:
    // 0x800173CC: jal         0x80015170
    // 0x800173D0: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    func_80015170(rdram, ctx);
        goto after_0;
    // 0x800173D0: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    after_0:
    // 0x800173D4: beql        $v0, $zero, L_800173E8
    if (ctx->r2 == 0) {
        // 0x800173D8: or          $v0, $zero, $zero
        ctx->r2 = 0 | 0;
            goto L_800173E8;
    }
    goto skip_1;
    // 0x800173D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
    skip_1:
    // 0x800173DC: b           L_800173E8
    // 0x800173E0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
        goto L_800173E8;
    // 0x800173E0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x800173E4: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800173E8:
    // 0x800173E8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800173EC: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800173F0: jr          $ra
    // 0x800173F4: nop

    return;
    // 0x800173F4: nop

;}
RECOMP_FUNC void func_800173F8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800173F8: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x800173FC: lhu         $v0, -0x9DE($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X9DE);
    // 0x80017400: jr          $ra
    // 0x80017404: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
    return;
    // 0x80017404: sltu        $v0, $zero, $v0
    ctx->r2 = 0 < ctx->r2 ? 1 : 0;
;}
RECOMP_FUNC void func_80017408(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017408: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x8001740C: lui         $a2, 0x800B
    ctx->r6 = S32(0X800B << 16);
    // 0x80017410: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80017414: beq         $a0, $zero, L_80017454
    if (ctx->r4 == 0) {
        // 0x80017418: lhu         $a2, -0x9E0($a2)
        ctx->r6 = MEM_HU(ctx->r6, -0X9E0);
            goto L_80017454;
    }
    // 0x80017418: lhu         $a2, -0x9E0($a2)
    ctx->r6 = MEM_HU(ctx->r6, -0X9E0);
    // 0x8001741C: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x80017420: jal         0x80015198
    // 0x80017424: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    func_80015198(rdram, ctx);
        goto after_0;
    // 0x80017424: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    after_0:
    // 0x80017428: beq         $v0, $zero, L_80017440
    if (ctx->r2 == 0) {
        // 0x8001742C: lhu         $a2, 0x1E($sp)
        ctx->r6 = MEM_HU(ctx->r29, 0X1E);
            goto L_80017440;
    }
    // 0x8001742C: lhu         $a2, 0x1E($sp)
    ctx->r6 = MEM_HU(ctx->r29, 0X1E);
    // 0x80017430: jal         0x800161D4
    // 0x80017434: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    func_800161D4(rdram, ctx);
        goto after_1;
    // 0x80017434: addiu       $a0, $zero, 0x3
    ctx->r4 = ADD32(0, 0X3);
    after_1:
    // 0x80017438: b           L_80017488
    // 0x8001743C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80017488;
    // 0x8001743C: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017440:
    // 0x80017440: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x80017444: jal         0x80016658
    // 0x80017448: addiu       $a1, $zero, 0x0
    ctx->r5 = ADD32(0, 0X0);
    func_80016658(rdram, ctx);
        goto after_2;
    // 0x80017448: addiu       $a1, $zero, 0x0
    ctx->r5 = ADD32(0, 0X0);
    after_2:
    // 0x8001744C: b           L_80017488
    // 0x80017450: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80017488;
    // 0x80017450: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017454:
    // 0x80017454: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x80017458: jal         0x80015198
    // 0x8001745C: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    func_80015198(rdram, ctx);
        goto after_3;
    // 0x8001745C: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    after_3:
    // 0x80017460: beq         $v0, $zero, L_80017478
    if (ctx->r2 == 0) {
        // 0x80017464: lhu         $a2, 0x1E($sp)
        ctx->r6 = MEM_HU(ctx->r29, 0X1E);
            goto L_80017478;
    }
    // 0x80017464: lhu         $a2, 0x1E($sp)
    ctx->r6 = MEM_HU(ctx->r29, 0X1E);
    // 0x80017468: jal         0x800161D4
    // 0x8001746C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    func_800161D4(rdram, ctx);
        goto after_4;
    // 0x8001746C: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_4:
    // 0x80017470: b           L_80017488
    // 0x80017474: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80017488;
    // 0x80017474: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017478:
    // 0x80017478: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x8001747C: jal         0x80016658
    // 0x80017480: lui         $a1, 0x3F80
    ctx->r5 = S32(0X3F80 << 16);
    func_80016658(rdram, ctx);
        goto after_5;
    // 0x80017480: lui         $a1, 0x3F80
    ctx->r5 = S32(0X3F80 << 16);
    after_5:
    // 0x80017484: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017488:
    // 0x80017488: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001748C: jr          $ra
    // 0x80017490: nop

    return;
    // 0x80017490: nop

;}
RECOMP_FUNC void func_80017494(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017494: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80017498: lui         $a2, 0x800B
    ctx->r6 = S32(0X800B << 16);
    // 0x8001749C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800174A0: beq         $a0, $zero, L_800174E4
    if (ctx->r4 == 0) {
        // 0x800174A4: lhu         $a2, -0x9E0($a2)
        ctx->r6 = MEM_HU(ctx->r6, -0X9E0);
            goto L_800174E4;
    }
    // 0x800174A4: lhu         $a2, -0x9E0($a2)
    ctx->r6 = MEM_HU(ctx->r6, -0X9E0);
    // 0x800174A8: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x800174AC: jal         0x80015198
    // 0x800174B0: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    func_80015198(rdram, ctx);
        goto after_0;
    // 0x800174B0: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    after_0:
    // 0x800174B4: beq         $v0, $zero, L_800174CC
    if (ctx->r2 == 0) {
        // 0x800174B8: lhu         $a2, 0x1E($sp)
        ctx->r6 = MEM_HU(ctx->r29, 0X1E);
            goto L_800174CC;
    }
    // 0x800174B8: lhu         $a2, 0x1E($sp)
    ctx->r6 = MEM_HU(ctx->r29, 0X1E);
    // 0x800174BC: jal         0x800161D4
    // 0x800174C0: addiu       $a0, $zero, 0xD
    ctx->r4 = ADD32(0, 0XD);
    func_800161D4(rdram, ctx);
        goto after_1;
    // 0x800174C0: addiu       $a0, $zero, 0xD
    ctx->r4 = ADD32(0, 0XD);
    after_1:
    // 0x800174C4: b           L_80017518
    // 0x800174C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80017518;
    // 0x800174C8: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_800174CC:
    // 0x800174CC: lui         $a1, 0x3F19
    ctx->r5 = S32(0X3F19 << 16);
    // 0x800174D0: ori         $a1, $a1, 0x999A
    ctx->r5 = ctx->r5 | 0X999A;
    // 0x800174D4: jal         0x80016658
    // 0x800174D8: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    func_80016658(rdram, ctx);
        goto after_2;
    // 0x800174D8: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    after_2:
    // 0x800174DC: b           L_80017518
    // 0x800174E0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80017518;
    // 0x800174E0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_800174E4:
    // 0x800174E4: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x800174E8: jal         0x80015198
    // 0x800174EC: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    func_80015198(rdram, ctx);
        goto after_3;
    // 0x800174EC: sh          $a2, 0x1E($sp)
    MEM_H(0X1E, ctx->r29) = ctx->r6;
    after_3:
    // 0x800174F0: beq         $v0, $zero, L_80017508
    if (ctx->r2 == 0) {
        // 0x800174F4: lhu         $a2, 0x1E($sp)
        ctx->r6 = MEM_HU(ctx->r29, 0X1E);
            goto L_80017508;
    }
    // 0x800174F4: lhu         $a2, 0x1E($sp)
    ctx->r6 = MEM_HU(ctx->r29, 0X1E);
    // 0x800174F8: jal         0x800161D4
    // 0x800174FC: addiu       $a0, $zero, 0xE
    ctx->r4 = ADD32(0, 0XE);
    func_800161D4(rdram, ctx);
        goto after_4;
    // 0x800174FC: addiu       $a0, $zero, 0xE
    ctx->r4 = ADD32(0, 0XE);
    after_4:
    // 0x80017500: b           L_80017518
    // 0x80017504: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80017518;
    // 0x80017504: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017508:
    // 0x80017508: andi        $a0, $a2, 0xFFFF
    ctx->r4 = ctx->r6 & 0XFFFF;
    // 0x8001750C: jal         0x80016658
    // 0x80017510: lui         $a1, 0x3F80
    ctx->r5 = S32(0X3F80 << 16);
    func_80016658(rdram, ctx);
        goto after_5;
    // 0x80017510: lui         $a1, 0x3F80
    ctx->r5 = S32(0X3F80 << 16);
    after_5:
    // 0x80017514: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017518:
    // 0x80017518: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x8001751C: jr          $ra
    // 0x80017520: nop

    return;
    // 0x80017520: nop

;}
RECOMP_FUNC void func_80017524(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017524: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x80017528: lui         $a1, 0x800B
    ctx->r5 = S32(0X800B << 16);
    // 0x8001752C: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x80017530: beq         $a0, $zero, L_80017558
    if (ctx->r4 == 0) {
        // 0x80017534: lhu         $a1, -0x9E0($a1)
        ctx->r5 = MEM_HU(ctx->r5, -0X9E0);
            goto L_80017558;
    }
    // 0x80017534: lhu         $a1, -0x9E0($a1)
    ctx->r5 = MEM_HU(ctx->r5, -0X9E0);
    // 0x80017538: jal         0x80015198
    // 0x8001753C: andi        $a0, $a1, 0xFFFF
    ctx->r4 = ctx->r5 & 0XFFFF;
    func_80015198(rdram, ctx);
        goto after_0;
    // 0x8001753C: andi        $a0, $a1, 0xFFFF
    ctx->r4 = ctx->r5 & 0XFFFF;
    after_0:
    // 0x80017540: beql        $v0, $zero, L_80017574
    if (ctx->r2 == 0) {
        // 0x80017544: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80017574;
    }
    goto skip_0;
    // 0x80017544: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_0:
    // 0x80017548: jal         0x800161D4
    // 0x8001754C: addiu       $a0, $zero, 0xB
    ctx->r4 = ADD32(0, 0XB);
    func_800161D4(rdram, ctx);
        goto after_1;
    // 0x8001754C: addiu       $a0, $zero, 0xB
    ctx->r4 = ADD32(0, 0XB);
    after_1:
    // 0x80017550: b           L_80017574
    // 0x80017554: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
        goto L_80017574;
    // 0x80017554: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017558:
    // 0x80017558: jal         0x80015198
    // 0x8001755C: andi        $a0, $a1, 0xFFFF
    ctx->r4 = ctx->r5 & 0XFFFF;
    func_80015198(rdram, ctx);
        goto after_2;
    // 0x8001755C: andi        $a0, $a1, 0xFFFF
    ctx->r4 = ctx->r5 & 0XFFFF;
    after_2:
    // 0x80017560: beql        $v0, $zero, L_80017574
    if (ctx->r2 == 0) {
        // 0x80017564: lw          $ra, 0x14($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X14);
            goto L_80017574;
    }
    goto skip_1;
    // 0x80017564: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    skip_1:
    // 0x80017568: jal         0x800161D4
    // 0x8001756C: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    func_800161D4(rdram, ctx);
        goto after_3;
    // 0x8001756C: addiu       $a0, $zero, 0xC
    ctx->r4 = ADD32(0, 0XC);
    after_3:
    // 0x80017570: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
L_80017574:
    // 0x80017574: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x80017578: jr          $ra
    // 0x8001757C: nop

    return;
    // 0x8001757C: nop

;}
RECOMP_FUNC void func_80017580(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017580: lui         $v0, 0x800B
    ctx->r2 = S32(0X800B << 16);
    // 0x80017584: jr          $ra
    // 0x80017588: lhu         $v0, -0x9E0($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X9E0);
    return;
    // 0x80017588: lhu         $v0, -0x9E0($v0)
    ctx->r2 = MEM_HU(ctx->r2, -0X9E0);
;}
RECOMP_FUNC void func_8001758C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001758C: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x80017590: sw          $a0, -0x9D8($at)
    MEM_W(-0X9D8, ctx->r1) = ctx->r4;
    // 0x80017594: jr          $ra
    // 0x80017598: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    return;
    // 0x80017598: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
;}
RECOMP_FUNC void func_8001759C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001759C: lui         $a0, 0x800B
    ctx->r4 = S32(0X800B << 16);
    // 0x800175A0: lhu         $a0, -0x9E0($a0)
    ctx->r4 = MEM_HU(ctx->r4, -0X9E0);
    // 0x800175A4: addiu       $sp, $sp, -0x18
    ctx->r29 = ADD32(ctx->r29, -0X18);
    // 0x800175A8: sw          $ra, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r31;
    // 0x800175AC: beql        $a0, $zero, L_800175F0
    if (ctx->r4 == 0) {
        // 0x800175B0: addiu       $v0, $zero, 0x1
        ctx->r2 = ADD32(0, 0X1);
            goto L_800175F0;
    }
    goto skip_0;
    // 0x800175B0: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    skip_0:
    // 0x800175B4: jal         0x80015198
    // 0x800175B8: nop

    func_80015198(rdram, ctx);
        goto after_0;
    // 0x800175B8: nop

    after_0:
    // 0x800175BC: beq         $v0, $zero, L_800175E4
    if (ctx->r2 == 0) {
        // 0x800175C0: nop
    
            goto L_800175E4;
    }
    // 0x800175C0: nop

    // 0x800175C4: jal         0x800161D4
    // 0x800175C8: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    func_800161D4(rdram, ctx);
        goto after_1;
    // 0x800175C8: addiu       $a0, $zero, 0x2
    ctx->r4 = ADD32(0, 0X2);
    after_1:
    // 0x800175CC: bne         $v0, $zero, L_800175DC
    if (ctx->r2 != 0) {
        // 0x800175D0: lui         $at, 0x800B
        ctx->r1 = S32(0X800B << 16);
            goto L_800175DC;
    }
    // 0x800175D0: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x800175D4: b           L_800175F0
    // 0x800175D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
        goto L_800175F0;
    // 0x800175D8: or          $v0, $zero, $zero
    ctx->r2 = 0 | 0;
L_800175DC:
    // 0x800175DC: b           L_800175EC
    // 0x800175E0: sh          $zero, -0x9E0($at)
    MEM_H(-0X9E0, ctx->r1) = 0;
        goto L_800175EC;
    // 0x800175E0: sh          $zero, -0x9E0($at)
    MEM_H(-0X9E0, ctx->r1) = 0;
L_800175E4:
    // 0x800175E4: jal         0x80017600
    // 0x800175E8: nop

    func_80017600(rdram, ctx);
        goto after_2;
    // 0x800175E8: nop

    after_2:
L_800175EC:
    // 0x800175EC: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
L_800175F0:
    // 0x800175F0: lw          $ra, 0x14($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X14);
    // 0x800175F4: addiu       $sp, $sp, 0x18
    ctx->r29 = ADD32(ctx->r29, 0X18);
    // 0x800175F8: jr          $ra
    // 0x800175FC: nop

    return;
    // 0x800175FC: nop

;}
RECOMP_FUNC void func_80017600(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017600: lui         $v1, 0x800B
    ctx->r3 = S32(0X800B << 16);
    // 0x80017604: addiu       $v1, $v1, -0xA20
    ctx->r3 = ADD32(ctx->r3, -0XA20);
    // 0x80017608: lhu         $t6, 0x40($v1)
    ctx->r14 = MEM_HU(ctx->r3, 0X40);
    // 0x8001760C: addiu       $v0, $zero, 0x1
    ctx->r2 = ADD32(0, 0X1);
    // 0x80017610: jr          $ra
    // 0x80017614: sh          $t6, 0x42($v1)
    MEM_H(0X42, ctx->r3) = ctx->r14;
    return;
    // 0x80017614: sh          $t6, 0x42($v1)
    MEM_H(0X42, ctx->r3) = ctx->r14;
;}
RECOMP_FUNC void func_80017618(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017618: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x8001761C: lui         $a0, 0x801D
    ctx->r4 = S32(0X801D << 16);
    // 0x80017620: lw          $a0, -0x53E4($a0)
    ctx->r4 = MEM_W(ctx->r4, -0X53E4);
    // 0x80017624: sw          $ra, 0x2C($sp)
    MEM_W(0X2C, ctx->r29) = ctx->r31;
    // 0x80017628: sw          $s4, 0x28($sp)
    MEM_W(0X28, ctx->r29) = ctx->r20;
    // 0x8001762C: sw          $s3, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r19;
    // 0x80017630: sw          $s2, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r18;
    // 0x80017634: sw          $s1, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r17;
    // 0x80017638: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x8001763C: beq         $a0, $zero, L_8001766C
    if (ctx->r4 == 0) {
        // 0x80017640: sw          $zero, 0x34($sp)
        MEM_W(0X34, ctx->r29) = 0;
            goto L_8001766C;
    }
    // 0x80017640: sw          $zero, 0x34($sp)
    MEM_W(0X34, ctx->r29) = 0;
    // 0x80017644: jal         0x8017B138
    // 0x80017648: nop

    func_8017B138(rdram, ctx);
        goto after_0;
    // 0x80017648: nop

    after_0:
    // 0x8001764C: beql        $v0, $zero, L_80017670
    if (ctx->r2 == 0) {
        // 0x80017650: lw          $t9, 0x34($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X34);
            goto L_80017670;
    }
    goto skip_0;
    // 0x80017650: lw          $t9, 0x34($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X34);
    skip_0:
    // 0x80017654: lw          $t6, 0x0($v0)
    ctx->r14 = MEM_W(ctx->r2, 0X0);
    // 0x80017658: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x8001765C: sll         $t7, $t6, 11
    ctx->r15 = S32(ctx->r14 << 11);
    // 0x80017660: bgezl       $t7, L_80017670
    if (SIGNED(ctx->r15) >= 0) {
        // 0x80017664: lw          $t9, 0x34($sp)
        ctx->r25 = MEM_W(ctx->r29, 0X34);
            goto L_80017670;
    }
    goto skip_1;
    // 0x80017664: lw          $t9, 0x34($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X34);
    skip_1:
    // 0x80017668: sw          $t8, 0x34($sp)
    MEM_W(0X34, ctx->r29) = ctx->r24;
L_8001766C:
    // 0x8001766C: lw          $t9, 0x34($sp)
    ctx->r25 = MEM_W(ctx->r29, 0X34);
L_80017670:
    // 0x80017670: lui         $t0, 0x800B
    ctx->r8 = S32(0X800B << 16);
    // 0x80017674: beql        $t9, $zero, L_800176F4
    if (ctx->r25 == 0) {
        // 0x80017678: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_800176F4;
    }
    goto skip_2;
    // 0x80017678: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_2:
    // 0x8001767C: lw          $t0, 0x70($t0)
    ctx->r8 = MEM_W(ctx->r8, 0X70);
    // 0x80017680: bnel        $t0, $zero, L_800176F4
    if (ctx->r8 != 0) {
        // 0x80017684: lw          $ra, 0x2C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X2C);
            goto L_800176F4;
    }
    goto skip_3;
    // 0x80017684: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
    skip_3:
    // 0x80017688: jal         0x80016C38
    // 0x8001768C: nop

    func_80016C38(rdram, ctx);
        goto after_1;
    // 0x8001768C: nop

    after_1:
    // 0x80017690: jal         0x80016C44
    // 0x80017694: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    func_80016C44(rdram, ctx);
        goto after_2;
    // 0x80017694: or          $s0, $v0, $zero
    ctx->r16 = ctx->r2 | 0;
    after_2:
    // 0x80017698: beq         $s0, $v0, L_800176F0
    if (ctx->r16 == ctx->r2) {
        // 0x8001769C: or          $s1, $v0, $zero
        ctx->r17 = ctx->r2 | 0;
            goto L_800176F0;
    }
    // 0x8001769C: or          $s1, $v0, $zero
    ctx->r17 = ctx->r2 | 0;
    // 0x800176A0: addiu       $s4, $zero, 0x213
    ctx->r20 = ADD32(0, 0X213);
    // 0x800176A4: addiu       $s3, $zero, 0x107
    ctx->r19 = ADD32(0, 0X107);
    // 0x800176A8: addiu       $s2, $zero, 0x106
    ctx->r18 = ADD32(0, 0X106);
L_800176AC:
    // 0x800176AC: jal         0x8001673C
    // 0x800176B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    func_8001673C(rdram, ctx);
        goto after_3;
    // 0x800176B0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    after_3:
    // 0x800176B4: beql        $v0, $zero, L_800176E8
    if (ctx->r2 == 0) {
        // 0x800176B8: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_800176E8;
    }
    goto skip_4;
    // 0x800176B8: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_4:
    // 0x800176BC: lhu         $v0, 0xC($s0)
    ctx->r2 = MEM_HU(ctx->r16, 0XC);
    // 0x800176C0: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x800176C4: beq         $v0, $s2, L_800176DC
    if (ctx->r2 == ctx->r18) {
        // 0x800176C8: nop
    
            goto L_800176DC;
    }
    // 0x800176C8: nop

    // 0x800176CC: beq         $v0, $s3, L_800176DC
    if (ctx->r2 == ctx->r19) {
        // 0x800176D0: nop
    
            goto L_800176DC;
    }
    // 0x800176D0: nop

    // 0x800176D4: bnel        $v0, $s4, L_800176E8
    if (ctx->r2 != ctx->r20) {
        // 0x800176D8: addiu       $s0, $s0, 0x38
        ctx->r16 = ADD32(ctx->r16, 0X38);
            goto L_800176E8;
    }
    goto skip_5;
    // 0x800176D8: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
    skip_5:
L_800176DC:
    // 0x800176DC: jal         0x800168AC
    // 0x800176E0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    func_800168AC(rdram, ctx);
        goto after_4;
    // 0x800176E0: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    after_4:
    // 0x800176E4: addiu       $s0, $s0, 0x38
    ctx->r16 = ADD32(ctx->r16, 0X38);
L_800176E8:
    // 0x800176E8: bne         $s0, $s1, L_800176AC
    if (ctx->r16 != ctx->r17) {
        // 0x800176EC: nop
    
            goto L_800176AC;
    }
    // 0x800176EC: nop

L_800176F0:
    // 0x800176F0: lw          $ra, 0x2C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X2C);
L_800176F4:
    // 0x800176F4: lw          $t1, 0x34($sp)
    ctx->r9 = MEM_W(ctx->r29, 0X34);
    // 0x800176F8: lui         $at, 0x800B
    ctx->r1 = S32(0X800B << 16);
    // 0x800176FC: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80017700: lw          $s1, 0x1C($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X1C);
    // 0x80017704: lw          $s2, 0x20($sp)
    ctx->r18 = MEM_W(ctx->r29, 0X20);
    // 0x80017708: lw          $s3, 0x24($sp)
    ctx->r19 = MEM_W(ctx->r29, 0X24);
    // 0x8001770C: lw          $s4, 0x28($sp)
    ctx->r20 = MEM_W(ctx->r29, 0X28);
    // 0x80017710: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x80017714: jr          $ra
    // 0x80017718: sw          $t1, 0x70($at)
    MEM_W(0X70, ctx->r1) = ctx->r9;
    return;
    // 0x80017718: sw          $t1, 0x70($at)
    MEM_W(0X70, ctx->r1) = ctx->r9;
;}
RECOMP_FUNC void func_80017720(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
;}
RECOMP_FUNC void game_main(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800177E0: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x800177E4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x800177E8: jal         0x800994C0
    // 0x800177EC: nop

    osContInit_recomp(rdram, ctx);
        goto after_0;
    // 0x800177EC: nop

    after_0:
    // 0x800177F0: lui         $t6, 0x800C
    ctx->r14 = S32(0X800C << 16);
    // 0x800177F4: addiu       $t6, $t6, 0x3830
    ctx->r14 = ADD32(ctx->r14, 0X3830);
    // 0x800177F8: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800177FC: lui         $a2, 0x8001
    ctx->r6 = S32(0X8001 << 16);
    // 0x80017800: addiu       $t7, $zero, 0x9
    ctx->r15 = ADD32(0, 0X9);
    // 0x80017804: sw          $t7, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r15;
    // 0x80017808: addiu       $a2, $a2, 0x783C
    ctx->r6 = ADD32(ctx->r6, 0X783C);
    // 0x8001780C: addiu       $a0, $a0, 0x2680
    ctx->r4 = ADD32(ctx->r4, 0X2680);
    // 0x80017810: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x80017814: addiu       $a1, $zero, 0x1
    ctx->r5 = ADD32(0, 0X1);
    // 0x80017818: jal         0x80098A00
    // 0x8001781C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    osCreateThread_recomp(rdram, ctx);
        goto after_1;
    // 0x8001781C: or          $a3, $zero, $zero
    ctx->r7 = 0 | 0;
    after_1:
    // 0x80017820: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017824: jal         0x800A6E30
    // 0x80017828: addiu       $a0, $a0, 0x2680
    ctx->r4 = ADD32(ctx->r4, 0X2680);
    osStartThread_recomp(rdram, ctx);
        goto after_2;
    // 0x80017828: addiu       $a0, $a0, 0x2680
    ctx->r4 = ADD32(ctx->r4, 0X2680);
    after_2:
    // 0x8001782C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x80017830: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80017834: jr          $ra
    // 0x80017838: nop

    return;
    // 0x80017838: nop

;}
RECOMP_FUNC void func_8001783C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x8001783C: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80017840: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80017844: sw          $a0, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r4;
    // 0x80017848: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x8001784C: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x80017850: addiu       $a2, $a2, 0x59F8
    ctx->r6 = ADD32(ctx->r6, 0X59F8);
    // 0x80017854: addiu       $a1, $a1, 0x59E0
    ctx->r5 = ADD32(ctx->r5, 0X59E0);
    // 0x80017858: addiu       $a0, $zero, 0x96
    ctx->r4 = ADD32(0, 0X96);
    // 0x8001785C: jal         0x8009B2F0
    // 0x80017860: addiu       $a3, $zero, 0xC8
    ctx->r7 = ADD32(0, 0XC8);
    osCreatePiManager_recomp(rdram, ctx);
        goto after_0;
    // 0x80017860: addiu       $a3, $zero, 0xC8
    ctx->r7 = ADD32(0, 0XC8);
    after_0:
    // 0x80017864: lui         $t6, 0x800C
    ctx->r14 = S32(0X800C << 16);
    // 0x80017868: addiu       $t6, $t6, 0x59E0
    ctx->r14 = ADD32(ctx->r14, 0X59E0);
    // 0x8001786C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017870: lui         $a2, 0x8001
    ctx->r6 = S32(0X8001 << 16);
    // 0x80017874: addiu       $t7, $zero, 0xA
    ctx->r15 = ADD32(0, 0XA);
    // 0x80017878: sw          $t7, 0x14($sp)
    MEM_W(0X14, ctx->r29) = ctx->r15;
    // 0x8001787C: addiu       $a2, $a2, 0x78D0
    ctx->r6 = ADD32(ctx->r6, 0X78D0);
    // 0x80017880: addiu       $a0, $a0, 0x3830
    ctx->r4 = ADD32(ctx->r4, 0X3830);
    // 0x80017884: sw          $t6, 0x10($sp)
    MEM_W(0X10, ctx->r29) = ctx->r14;
    // 0x80017888: addiu       $a1, $zero, 0x5
    ctx->r5 = ADD32(0, 0X5);
    // 0x8001788C: jal         0x80098A00
    // 0x80017890: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    osCreateThread_recomp(rdram, ctx);
        goto after_1;
    // 0x80017890: lw          $a3, 0x20($sp)
    ctx->r7 = MEM_W(ctx->r29, 0X20);
    after_1:
    // 0x80017894: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017898: jal         0x800A6E30
    // 0x8001789C: addiu       $a0, $a0, 0x3830
    ctx->r4 = ADD32(ctx->r4, 0X3830);
    osStartThread_recomp(rdram, ctx);
        goto after_2;
    // 0x8001789C: addiu       $a0, $a0, 0x3830
    ctx->r4 = ADD32(ctx->r4, 0X3830);
    after_2:
    // 0x800178A0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x800178A4: jal         0x800A19D0
    // 0x800178A8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    osSetThreadPri_recomp(rdram, ctx);
        goto after_3;
    // 0x800178A8: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    after_3:
L_800178AC:
    // 0x800178AC: b           L_800178AC
    pause_self(rdram);
    // 0x800178B0: nop

    // 0x800178B4: nop

    // 0x800178B8: nop

    // 0x800178BC: nop

    // 0x800178C0: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    // 0x800178C4: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x800178C8: jr          $ra
    // 0x800178CC: nop

    return;
    // 0x800178CC: nop

;}
RECOMP_FUNC void graphThread_entrypoint(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x800178D0: addiu       $sp, $sp, -0x48
    ctx->r29 = ADD32(ctx->r29, -0X48);
    // 0x800178D4: sw          $ra, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r31;
    // 0x800178D8: sw          $s1, 0x20($sp)
    MEM_W(0X20, ctx->r29) = ctx->r17;
    // 0x800178DC: sw          $s0, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r16;
    // 0x800178E0: sdc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    SD(ctx->f20.u64, 0X10, ctx->r29);
    // 0x800178E4: sw          $a0, 0x48($sp)
    MEM_W(0X48, ctx->r29) = ctx->r4;
    // 0x800178E8: jal         0x8001A350
    // 0x800178EC: sw          $zero, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = 0;
    func_8001A350(rdram, ctx);
        goto after_0;
    // 0x800178EC: sw          $zero, 0x3C($sp)
    MEM_W(0X3C, ctx->r29) = 0;
    after_0:
    // 0x800178F0: jal         0x800A11D0
    // 0x800178F4: nop

    osUnmapTLBAll_recomp(rdram, ctx);
        goto after_1;
    // 0x800178F4: nop

    after_1:
    // 0x800178F8: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x800178FC: lui         $t6, 0x800D
    ctx->r14 = S32(0X800D << 16);
    // 0x80017900: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80017904: addiu       $t6, $t6, -0x1898
    ctx->r14 = ADD32(ctx->r14, -0X1898);
    // 0x80017908: sb          $zero, 0x0($t6)
    MEM_B(0X0, ctx->r14) = 0;
    // 0x8001790C: or          $a0, $s0, $zero
    ctx->r4 = ctx->r16 | 0;
    // 0x80017910: jal         0x80000F30
    // 0x80017914: addiu       $a1, $zero, 0x2BD8
    ctx->r5 = ADD32(0, 0X2BD8);
    bzero_recomp(rdram, ctx);
        goto after_2;
    // 0x80017914: addiu       $a1, $zero, 0x2BD8
    ctx->r5 = ADD32(0, 0X2BD8);
    after_2:
    // 0x80017918: sh          $zero, 0x34($s0)
    MEM_H(0X34, ctx->r16) = 0;
    // 0x8001791C: lh          $v0, 0x34($s0)
    ctx->r2 = MEM_H(ctx->r16, 0X34);
    // 0x80017920: lui         $t7, 0x801B
    ctx->r15 = S32(0X801B << 16);
    // 0x80017924: lui         $t8, 0x801C
    ctx->r24 = S32(0X801C << 16);
    // 0x80017928: lui         $t9, 0x8063
    ctx->r25 = S32(0X8063 << 16);
    // 0x8001792C: addiu       $t7, $t7, 0x42C0
    ctx->r15 = ADD32(ctx->r15, 0X42C0);
    // 0x80017930: addiu       $t8, $t8, -0x1D40
    ctx->r24 = ADD32(ctx->r24, -0X1D40);
    // 0x80017934: addiu       $t9, $t9, 0x4000
    ctx->r25 = ADD32(ctx->r25, 0X4000);
    // 0x80017938: sw          $t7, 0x14($s0)
    MEM_W(0X14, ctx->r16) = ctx->r15;
    // 0x8001793C: sw          $t8, 0x18($s0)
    MEM_W(0X18, ctx->r16) = ctx->r24;
    // 0x80017940: sw          $t9, 0x1C($s0)
    MEM_W(0X1C, ctx->r16) = ctx->r25;
    // 0x80017944: sh          $zero, 0x36($s0)
    MEM_H(0X36, ctx->r16) = 0;
    // 0x80017948: beq         $v0, $zero, L_8001795C
    if (ctx->r2 == 0) {
        // 0x8001794C: sh          $zero, 0x38($s0)
        MEM_H(0X38, ctx->r16) = 0;
            goto L_8001795C;
    }
    // 0x8001794C: sh          $zero, 0x38($s0)
    MEM_H(0X38, ctx->r16) = 0;
    // 0x80017950: addiu       $t0, $zero, 0x3
    ctx->r8 = ADD32(0, 0X3);
    // 0x80017954: b           L_80017964
    // 0x80017958: sh          $t0, 0x24($s0)
    MEM_H(0X24, ctx->r16) = ctx->r8;
        goto L_80017964;
    // 0x80017958: sh          $t0, 0x24($s0)
    MEM_H(0X24, ctx->r16) = ctx->r8;
L_8001795C:
    // 0x8001795C: addiu       $t1, $zero, 0x2
    ctx->r9 = ADD32(0, 0X2);
    // 0x80017960: sh          $t1, 0x24($s0)
    MEM_H(0X24, ctx->r16) = ctx->r9;
L_80017964:
    // 0x80017964: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x80017968: mtc1        $at, $f4
    ctx->f4.u32l = ctx->r1;
    // 0x8001796C: lui         $at, 0x41F0
    ctx->r1 = S32(0X41F0 << 16);
    // 0x80017970: beq         $v0, $zero, L_80017988
    if (ctx->r2 == 0) {
        // 0x80017974: swc1        $f4, 0x4($s0)
        MEM_W(0X4, ctx->r16) = ctx->f4.u32l;
            goto L_80017988;
    }
    // 0x80017974: swc1        $f4, 0x4($s0)
    MEM_W(0X4, ctx->r16) = ctx->f4.u32l;
    // 0x80017978: lui         $at, 0x41A0
    ctx->r1 = S32(0X41A0 << 16);
    // 0x8001797C: mtc1        $at, $f6
    ctx->f6.u32l = ctx->r1;
    // 0x80017980: b           L_80017994
    // 0x80017984: swc1        $f6, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f6.u32l;
        goto L_80017994;
    // 0x80017984: swc1        $f6, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f6.u32l;
L_80017988:
    // 0x80017988: mtc1        $at, $f8
    ctx->f8.u32l = ctx->r1;
    // 0x8001798C: nop

    // 0x80017990: swc1        $f8, 0x8($s0)
    MEM_W(0X8, ctx->r16) = ctx->f8.u32l;
L_80017994:
    // 0x80017994: lwc1        $f10, 0x8($s0)
    ctx->f10.u32l = MEM_W(ctx->r16, 0X8);
    // 0x80017998: lwc1        $f16, 0x4($s0)
    ctx->f16.u32l = MEM_W(ctx->r16, 0X4);
    // 0x8001799C: addiu       $t3, $zero, 0x2
    ctx->r11 = ADD32(0, 0X2);
    // 0x800179A0: addiu       $t2, $zero, 0xFF
    ctx->r10 = ADD32(0, 0XFF);
    // 0x800179A4: div.s       $f18, $f10, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 16);
    NAN_CHECK(ctx->f10.fl); NAN_CHECK(ctx->f16.fl); 
    ctx->f18.fl = DIV_S(ctx->f10.fl, ctx->f16.fl);
    // 0x800179A8: sh          $t3, 0x7A($s0)
    MEM_H(0X7A, ctx->r16) = ctx->r11;
    // 0x800179AC: addiu       $v1, $zero, 0x1
    ctx->r3 = ADD32(0, 0X1);
    // 0x800179B0: sb          $t2, 0x7F($s0)
    MEM_B(0X7F, ctx->r16) = ctx->r10;
    // 0x800179B4: lui         $t3, 0x8020
    ctx->r11 = S32(0X8020 << 16);
    // 0x800179B8: lui         $t4, 0x8022
    ctx->r12 = S32(0X8022 << 16);
    // 0x800179BC: lui         $t5, 0x8025
    ctx->r13 = S32(0X8025 << 16);
    // 0x800179C0: sh          $v1, 0x80($s0)
    MEM_H(0X80, ctx->r16) = ctx->r3;
    // 0x800179C4: sh          $v1, 0x82($s0)
    MEM_H(0X82, ctx->r16) = ctx->r3;
    // 0x800179C8: addiu       $t2, $zero, 0x160
    ctx->r10 = ADD32(0, 0X160);
    // 0x800179CC: addiu       $t3, $t3, 0x0
    ctx->r11 = ADD32(ctx->r11, 0X0);
    // 0x800179D0: addiu       $t4, $t4, 0x5800
    ctx->r12 = ADD32(ctx->r12, 0X5800);
    // 0x800179D4: addiu       $t5, $t5, -0x5000
    ctx->r13 = ADD32(ctx->r13, -0X5000);
    // 0x800179D8: addiu       $t6, $zero, 0x140
    ctx->r14 = ADD32(0, 0X140);
    // 0x800179DC: addiu       $t7, $zero, 0xF0
    ctx->r15 = ADD32(0, 0XF0);
    // 0x800179E0: addiu       $t8, $zero, 0xC
    ctx->r24 = ADD32(0, 0XC);
    // 0x800179E4: addiu       $t9, $zero, 0x8
    ctx->r25 = ADD32(0, 0X8);
    // 0x800179E8: addiu       $t0, $zero, 0x134
    ctx->r8 = ADD32(0, 0X134);
    // 0x800179EC: addiu       $t1, $zero, 0xE8
    ctx->r9 = ADD32(0, 0XE8);
    // 0x800179F0: beq         $v0, $zero, L_80017A4C
    if (ctx->r2 == 0) {
        // 0x800179F4: swc1        $f18, 0xC($s0)
        MEM_W(0XC, ctx->r16) = ctx->f18.u32l;
            goto L_80017A4C;
    }
    // 0x800179F4: swc1        $f18, 0xC($s0)
    MEM_W(0XC, ctx->r16) = ctx->f18.u32l;
    // 0x800179F8: lui         $t4, 0x8064
    ctx->r12 = S32(0X8064 << 16);
    // 0x800179FC: lui         $t5, 0x806D
    ctx->r13 = S32(0X806D << 16);
    // 0x80017A00: lui         $t6, 0x8077
    ctx->r14 = S32(0X8077 << 16);
    // 0x80017A04: addiu       $t4, $t4, -0x2000
    ctx->r12 = ADD32(ctx->r12, -0X2000);
    // 0x80017A08: addiu       $t5, $t5, 0x4000
    ctx->r13 = ADD32(ctx->r13, 0X4000);
    // 0x80017A0C: addiu       $t6, $t6, -0x6000
    ctx->r14 = ADD32(ctx->r14, -0X6000);
    // 0x80017A10: addiu       $t7, $zero, 0x280
    ctx->r15 = ADD32(0, 0X280);
    // 0x80017A14: addiu       $t8, $zero, 0x1E0
    ctx->r24 = ADD32(0, 0X1E0);
    // 0x80017A18: addiu       $t9, $zero, 0x18
    ctx->r25 = ADD32(0, 0X18);
    // 0x80017A1C: addiu       $t0, $zero, 0x10
    ctx->r8 = ADD32(0, 0X10);
    // 0x80017A20: addiu       $t1, $zero, 0x1D8
    ctx->r9 = ADD32(0, 0X1D8);
    // 0x80017A24: sw          $t4, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r12;
    // 0x80017A28: sw          $t5, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r13;
    // 0x80017A2C: sw          $t6, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r14;
    // 0x80017A30: sh          $t7, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r15;
    // 0x80017A34: sh          $t8, 0x4E($s0)
    MEM_H(0X4E, ctx->r16) = ctx->r24;
    // 0x80017A38: sh          $t9, 0x50($s0)
    MEM_H(0X50, ctx->r16) = ctx->r25;
    // 0x80017A3C: sh          $t0, 0x52($s0)
    MEM_H(0X52, ctx->r16) = ctx->r8;
    // 0x80017A40: sh          $t1, 0x54($s0)
    MEM_H(0X54, ctx->r16) = ctx->r9;
    // 0x80017A44: b           L_80017A70
    // 0x80017A48: sh          $t2, 0x56($s0)
    MEM_H(0X56, ctx->r16) = ctx->r10;
        goto L_80017A70;
    // 0x80017A48: sh          $t2, 0x56($s0)
    MEM_H(0X56, ctx->r16) = ctx->r10;
L_80017A4C:
    // 0x80017A4C: sw          $t3, 0x3C($s0)
    MEM_W(0X3C, ctx->r16) = ctx->r11;
    // 0x80017A50: sw          $t4, 0x40($s0)
    MEM_W(0X40, ctx->r16) = ctx->r12;
    // 0x80017A54: sw          $t5, 0x44($s0)
    MEM_W(0X44, ctx->r16) = ctx->r13;
    // 0x80017A58: sh          $t6, 0x4C($s0)
    MEM_H(0X4C, ctx->r16) = ctx->r14;
    // 0x80017A5C: sh          $t7, 0x4E($s0)
    MEM_H(0X4E, ctx->r16) = ctx->r15;
    // 0x80017A60: sh          $t8, 0x50($s0)
    MEM_H(0X50, ctx->r16) = ctx->r24;
    // 0x80017A64: sh          $t9, 0x52($s0)
    MEM_H(0X52, ctx->r16) = ctx->r25;
    // 0x80017A68: sh          $t0, 0x54($s0)
    MEM_H(0X54, ctx->r16) = ctx->r8;
    // 0x80017A6C: sh          $t1, 0x56($s0)
    MEM_H(0X56, ctx->r16) = ctx->r9;
L_80017A70:
    // 0x80017A70: lh          $t2, 0x4C($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X4C);
    // 0x80017A74: lh          $t4, 0x4E($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X4E);
    // 0x80017A78: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    // 0x80017A7C: sll         $t3, $t2, 2
    ctx->r11 = S32(ctx->r10 << 2);
    // 0x80017A80: subu        $t3, $t3, $t2
    ctx->r11 = SUB32(ctx->r11, ctx->r10);
    // 0x80017A84: sll         $t3, $t3, 1
    ctx->r11 = S32(ctx->r11 << 1);
    // 0x80017A88: multu       $t3, $t4
    result = U64(U32(ctx->r11)) * U64(U32(ctx->r12)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80017A8C: mflo        $a1
    ctx->r5 = lo;
    // 0x80017A90: jal         0x80000F30
    // 0x80017A94: nop

    bzero_recomp(rdram, ctx);
        goto after_3;
    // 0x80017A94: nop

    after_3:
    // 0x80017A98: addiu       $a0, $zero, 0x2710
    ctx->r4 = ADD32(0, 0X2710);
    // 0x80017A9C: addiu       $t9, $zero, 0xF
    ctx->r25 = ADD32(0, 0XF);
    // 0x80017AA0: addiu       $t5, $zero, 0x1
    ctx->r13 = ADD32(0, 0X1);
    // 0x80017AA4: addiu       $t6, $zero, 0x3D4
    ctx->r14 = ADD32(0, 0X3D4);
    // 0x80017AA8: addiu       $t7, $zero, 0x3E8
    ctx->r15 = ADD32(0, 0X3E8);
    // 0x80017AAC: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80017AB0: lui         $v1, 0x801D
    ctx->r3 = S32(0X801D << 16);
    // 0x80017AB4: sh          $t5, 0x8C($s0)
    MEM_H(0X8C, ctx->r16) = ctx->r13;
    // 0x80017AB8: sh          $t6, 0x98($s0)
    MEM_H(0X98, ctx->r16) = ctx->r14;
    // 0x80017ABC: sh          $t7, 0x9A($s0)
    MEM_H(0X9A, ctx->r16) = ctx->r15;
    // 0x80017AC0: sh          $zero, 0x259E($s0)
    MEM_H(0X259E, ctx->r16) = 0;
    // 0x80017AC4: sh          $t9, 0x2BC2($s0)
    MEM_H(0X2BC2, ctx->r16) = ctx->r25;
    // 0x80017AC8: sh          $t9, 0x2BC0($s0)
    MEM_H(0X2BC0, ctx->r16) = ctx->r25;
    // 0x80017ACC: sh          $a0, 0x287A($s0)
    MEM_H(0X287A, ctx->r16) = ctx->r4;
    // 0x80017AD0: sh          $a0, 0x287C($s0)
    MEM_H(0X287C, ctx->r16) = ctx->r4;
    // 0x80017AD4: sh          $zero, 0x287E($s0)
    MEM_H(0X287E, ctx->r16) = 0;
    // 0x80017AD8: sw          $zero, 0x2880($s0)
    MEM_W(0X2880, ctx->r16) = 0;
    // 0x80017ADC: addiu       $v1, $v1, -0x7D00
    ctx->r3 = ADD32(ctx->r3, -0X7D00);
    // 0x80017AE0: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
L_80017AE4:
    // 0x80017AE4: addiu       $v0, $v0, 0x4
    ctx->r2 = ADD32(ctx->r2, 0X4);
    // 0x80017AE8: sb          $zero, 0x2880($v0)
    MEM_B(0X2880, ctx->r2) = 0;
    // 0x80017AEC: sb          $zero, 0x2881($v0)
    MEM_B(0X2881, ctx->r2) = 0;
    // 0x80017AF0: sb          $zero, 0x2882($v0)
    MEM_B(0X2882, ctx->r2) = 0;
    // 0x80017AF4: bne         $v0, $v1, L_80017AE4
    if (ctx->r2 != ctx->r3) {
        // 0x80017AF8: sb          $zero, 0x2883($v0)
        MEM_B(0X2883, ctx->r2) = 0;
            goto L_80017AE4;
    }
    // 0x80017AF8: sb          $zero, 0x2883($v0)
    MEM_B(0X2883, ctx->r2) = 0;
    // 0x80017AFC: jal         0x80018EE0
    // 0x80017B00: sw          $zero, 0x28C4($s0)
    MEM_W(0X28C4, ctx->r16) = 0;
    func_80018EE0(rdram, ctx);
        goto after_4;
    // 0x80017B00: sw          $zero, 0x28C4($s0)
    MEM_W(0X28C4, ctx->r16) = 0;
    after_4:
    // 0x80017B04: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017B08: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80017B0C: addiu       $a1, $a1, 0x5D30
    ctx->r5 = ADD32(ctx->r5, 0X5D30);
    // 0x80017B10: addiu       $a0, $a0, 0x5D18
    ctx->r4 = ADD32(ctx->r4, 0X5D18);
    // 0x80017B14: jal         0x800A1460
    // 0x80017B18: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_5;
    // 0x80017B18: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_5:
    // 0x80017B1C: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017B20: lui         $a1, 0x800C
    ctx->r5 = S32(0X800C << 16);
    // 0x80017B24: addiu       $a1, $a1, 0x5D50
    ctx->r5 = ADD32(ctx->r5, 0X5D50);
    // 0x80017B28: addiu       $a0, $a0, 0x5D38
    ctx->r4 = ADD32(ctx->r4, 0X5D38);
    // 0x80017B2C: jal         0x800A1460
    // 0x80017B30: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    osCreateMesgQueue_recomp(rdram, ctx);
        goto after_6;
    // 0x80017B30: addiu       $a2, $zero, 0x8
    ctx->r6 = ADD32(0, 0X8);
    after_6:
    // 0x80017B34: lui         $v1, 0x8000
    ctx->r3 = S32(0X8000 << 16);
    // 0x80017B38: lw          $v1, 0x300($v1)
    ctx->r3 = MEM_W(ctx->r3, 0X300);
    // 0x80017B3C: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x80017B40: addiu       $a0, $s1, 0x5E80
    ctx->r4 = ADD32(ctx->r17, 0X5E80);
    // 0x80017B44: beq         $v1, $zero, L_80017BA4
    if (ctx->r3 == 0) {
        // 0x80017B48: addiu       $a1, $zero, 0x2A
        ctx->r5 = ADD32(0, 0X2A);
            goto L_80017BA4;
    }
    // 0x80017B48: addiu       $a1, $zero, 0x2A
    ctx->r5 = ADD32(0, 0X2A);
    // 0x80017B4C: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80017B50: beq         $v1, $at, L_80017B68
    if (ctx->r3 == ctx->r1) {
        // 0x80017B54: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_80017B68;
    }
    // 0x80017B54: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80017B58: beq         $v1, $at, L_80017B88
    if (ctx->r3 == ctx->r1) {
        // 0x80017B5C: lui         $s1, 0x800C
        ctx->r17 = S32(0X800C << 16);
            goto L_80017B88;
    }
    // 0x80017B5C: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x80017B60: b           L_80017BAC
    // 0x80017B64: nop

        goto L_80017BAC;
    // 0x80017B64: nop

L_80017B68:
    // 0x80017B68: lui         $s1, 0x800C
    ctx->r17 = S32(0X800C << 16);
    // 0x80017B6C: addiu       $s1, $s1, 0x5E80
    ctx->r17 = ADD32(ctx->r17, 0X5E80);
    // 0x80017B70: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80017B74: or          $a1, $zero, $zero
    ctx->r5 = 0 | 0;
    // 0x80017B78: jal         0x800184E0
    // 0x80017B7C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    scheduler_create(rdram, ctx);
        goto after_7;
    // 0x80017B7C: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_7:
    // 0x80017B80: b           L_80017BB4
    // 0x80017B84: nop

        goto L_80017BB4;
    // 0x80017B84: nop

L_80017B88:
    // 0x80017B88: addiu       $s1, $s1, 0x5E80
    ctx->r17 = ADD32(ctx->r17, 0X5E80);
    // 0x80017B8C: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80017B90: addiu       $a1, $zero, 0x1C
    ctx->r5 = ADD32(0, 0X1C);
    // 0x80017B94: jal         0x800184E0
    // 0x80017B98: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    scheduler_create(rdram, ctx);
        goto after_8;
    // 0x80017B98: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_8:
    // 0x80017B9C: b           L_80017BB4
    // 0x80017BA0: nop

        goto L_80017BB4;
    // 0x80017BA0: nop

L_80017BA4:
    // 0x80017BA4: jal         0x800184E0
    // 0x80017BA8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    scheduler_create(rdram, ctx);
        goto after_9;
    // 0x80017BA8: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_9:
L_80017BAC:
    // 0x80017BAC: b           L_80017BAC
    pause_self(rdram);
    // 0x80017BB0: nop

L_80017BB4:
    // 0x80017BB4: lui         $a2, 0x800C
    ctx->r6 = S32(0X800C << 16);
    // 0x80017BB8: addiu       $a2, $a2, 0x5D38
    ctx->r6 = ADD32(ctx->r6, 0X5D38);
    // 0x80017BBC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    // 0x80017BC0: jal         0x80018890
    // 0x80017BC4: addiu       $a1, $sp, 0x40
    ctx->r5 = ADD32(ctx->r29, 0X40);
    func_80018890(rdram, ctx);
        goto after_10;
    // 0x80017BC4: addiu       $a1, $sp, 0x40
    ctx->r5 = ADD32(ctx->r29, 0X40);
    after_10:
    // 0x80017BC8: jal         0x80018764
    // 0x80017BCC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    func_80018764(rdram, ctx);
        goto after_11;
    // 0x80017BCC: or          $a0, $s1, $zero
    ctx->r4 = ctx->r17 | 0;
    after_11:
    // 0x80017BD0: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80017BD4: sw          $v0, 0x6748($at)
    MEM_W(0X6748, ctx->r1) = ctx->r2;
    // 0x80017BD8: jal         0x800A38F0
    // 0x80017BDC: addiu       $a0, $zero, 0x4A
    ctx->r4 = ADD32(0, 0X4A);
    osViSetSpecialFeatures_recomp(rdram, ctx);
        goto after_12;
    // 0x80017BDC: addiu       $a0, $zero, 0x4A
    ctx->r4 = ADD32(0, 0X4A);
    after_12:
    // 0x80017BE0: jal         0x800195E0
    // 0x80017BE4: nop

    contpak_init(rdram, ctx);
        goto after_13;
    // 0x80017BE4: nop

    after_13:
    // 0x80017BE8: lui         $a0, 0x801A
    ctx->r4 = S32(0X801A << 16);
    // 0x80017BEC: lui         $t0, 0x801A
    ctx->r8 = S32(0X801A << 16);
    // 0x80017BF0: addiu       $a0, $a0, -0x2EB0
    ctx->r4 = ADD32(ctx->r4, -0X2EB0);
    // 0x80017BF4: addiu       $t0, $t0, -0x510
    ctx->r8 = ADD32(ctx->r8, -0X510);
    // 0x80017BF8: jal         0x80000F30
    // 0x80017BFC: subu        $a1, $t0, $a0
    ctx->r5 = SUB32(ctx->r8, ctx->r4);
    bzero_recomp(rdram, ctx);
        goto after_14;
    // 0x80017BFC: subu        $a1, $t0, $a0
    ctx->r5 = SUB32(ctx->r8, ctx->r4);
    after_14:
    // 0x80017C00: lui         $a0, 0xC
    ctx->r4 = S32(0XC << 16);
    // 0x80017C04: lui         $t1, 0x12
    ctx->r9 = S32(0X12 << 16);
    // 0x80017C08: addiu       $a0, $a0, 0x2120
    ctx->r4 = ADD32(ctx->r4, 0X2120);
    // 0x80017C0C: addiu       $t1, $t1, -0x2600
    ctx->r9 = ADD32(ctx->r9, -0X2600);
    // 0x80017C10: lui         $a1, 0x8014
    ctx->r5 = S32(0X8014 << 16);
    // 0x80017C14: addiu       $a1, $a1, 0x1870
    ctx->r5 = ADD32(ctx->r5, 0X1870);
    // 0x80017C18: jal         0x8001A42C
    // 0x80017C1C: subu        $a2, $t1, $a0
    ctx->r6 = SUB32(ctx->r9, ctx->r4);
    func_8001A42C(rdram, ctx);
        goto after_15;
    // 0x80017C1C: subu        $a2, $t1, $a0
    ctx->r6 = SUB32(ctx->r9, ctx->r4);
    after_15:
    // 0x80017C20: lui         $a0, 0x800B
    ctx->r4 = S32(0X800B << 16);
    // 0x80017C24: jal         0x8001A230
    // 0x80017C28: addiu       $a0, $a0, 0x2A78
    ctx->r4 = ADD32(ctx->r4, 0X2A78);
    func_8001A230(rdram, ctx);
        goto after_16;
    // 0x80017C28: addiu       $a0, $a0, 0x2A78
    ctx->r4 = ADD32(ctx->r4, 0X2A78);
    after_16:
    // 0x80017C2C: lui         $a0, 0x800B
    ctx->r4 = S32(0X800B << 16);
    // 0x80017C30: jal         0x8001A230
    // 0x80017C34: addiu       $a0, $a0, 0x2C28
    ctx->r4 = ADD32(ctx->r4, 0X2C28);
    func_8001A230(rdram, ctx);
        goto after_17;
    // 0x80017C34: addiu       $a0, $a0, 0x2C28
    ctx->r4 = ADD32(ctx->r4, 0X2C28);
    after_17:
    // 0x80017C38: lui         $a0, 0x12
    ctx->r4 = S32(0X12 << 16);
    // 0x80017C3C: lui         $t2, 0x13
    ctx->r10 = S32(0X13 << 16);
    // 0x80017C40: addiu       $a0, $a0, -0x2600
    ctx->r4 = ADD32(ctx->r4, -0X2600);
    // 0x80017C44: addiu       $t2, $t2, -0x5D20
    ctx->r10 = ADD32(ctx->r10, -0X5D20);
    // 0x80017C48: lui         $a1, 0x8027
    ctx->r5 = S32(0X8027 << 16);
    // 0x80017C4C: addiu       $a1, $a1, 0x800
    ctx->r5 = ADD32(ctx->r5, 0X800);
    // 0x80017C50: jal         0x8001A42C
    // 0x80017C54: subu        $a2, $t2, $a0
    ctx->r6 = SUB32(ctx->r10, ctx->r4);
    func_8001A42C(rdram, ctx);
        goto after_18;
    // 0x80017C54: subu        $a2, $t2, $a0
    ctx->r6 = SUB32(ctx->r10, ctx->r4);
    after_18:
    // 0x80017C58: jal         0x80090C00
    // 0x80017C5C: nop

    func_80090C00(rdram, ctx);
        goto after_19;
    // 0x80017C5C: nop

    after_19:
    // 0x80017C60: jal         0x80000460
    // 0x80017C64: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    func_80000460(rdram, ctx);
        goto after_20;
    // 0x80017C64: addiu       $a0, $zero, 0x4
    ctx->r4 = ADD32(0, 0X4);
    after_20:
    // 0x80017C68: lui         $at, 0x3F80
    ctx->r1 = S32(0X3F80 << 16);
    // 0x80017C6C: lui         $s1, 0x800B
    ctx->r17 = S32(0X800B << 16);
    // 0x80017C70: mtc1        $at, $f20
    ctx->f20.u32l = ctx->r1;
    // 0x80017C74: addiu       $s1, $s1, 0x88
    ctx->r17 = ADD32(ctx->r17, 0X88);
L_80017C78:
    // 0x80017C78: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017C7C: addiu       $a0, $a0, 0x5D38
    ctx->r4 = ADD32(ctx->r4, 0X5D38);
    // 0x80017C80: addiu       $a1, $sp, 0x3C
    ctx->r5 = ADD32(ctx->r29, 0X3C);
    // 0x80017C84: jal         0x80096CE0
    // 0x80017C88: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    osRecvMesg_recomp(rdram, ctx);
        goto after_21;
    // 0x80017C88: addiu       $a2, $zero, 0x1
    ctx->r6 = ADD32(0, 0X1);
    after_21:
    // 0x80017C8C: lw          $t3, 0x3C($sp)
    ctx->r11 = MEM_W(ctx->r29, 0X3C);
    // 0x80017C90: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80017C94: lh          $v1, 0x0($t3)
    ctx->r3 = MEM_H(ctx->r11, 0X0);
    // 0x80017C98: beq         $v1, $at, L_80017CB8
    if (ctx->r3 == ctx->r1) {
        // 0x80017C9C: addiu       $at, $zero, 0x2
        ctx->r1 = ADD32(0, 0X2);
            goto L_80017CB8;
    }
    // 0x80017C9C: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80017CA0: beq         $v1, $at, L_80017CFC
    if (ctx->r3 == ctx->r1) {
        // 0x80017CA4: addiu       $at, $zero, 0x3
        ctx->r1 = ADD32(0, 0X3);
            goto L_80017CFC;
    }
    // 0x80017CA4: addiu       $at, $zero, 0x3
    ctx->r1 = ADD32(0, 0X3);
    // 0x80017CA8: beql        $v1, $at, L_80017D50
    if (ctx->r3 == ctx->r1) {
        // 0x80017CAC: lhu         $t9, 0x0($s1)
        ctx->r25 = MEM_HU(ctx->r17, 0X0);
            goto L_80017D50;
    }
    goto skip_0;
    // 0x80017CAC: lhu         $t9, 0x0($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0X0);
    skip_0:
    // 0x80017CB0: b           L_80017C78
    // 0x80017CB4: nop

        goto L_80017C78;
    // 0x80017CB4: nop

L_80017CB8:
    // 0x80017CB8: lhu         $t4, 0x0($s1)
    ctx->r12 = MEM_HU(ctx->r17, 0X0);
    // 0x80017CBC: bne         $t4, $zero, L_80017CD4
    if (ctx->r12 != 0) {
        // 0x80017CC0: nop
    
            goto L_80017CD4;
    }
    // 0x80017CC0: nop

    // 0x80017CC4: jal         0x80017DB8
    // 0x80017CC8: nop

    func_80017DB8(rdram, ctx);
        goto after_22;
    // 0x80017CC8: nop

    after_22:
    // 0x80017CCC: b           L_80017C78
    // 0x80017CD0: nop

        goto L_80017C78;
    // 0x80017CD0: nop

L_80017CD4:
    // 0x80017CD4: jal         0x800A6F80
    // 0x80017CD8: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    osViSetYScale_recomp(rdram, ctx);
        goto after_23;
    // 0x80017CD8: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_23:
    // 0x80017CDC: jal         0x800A70B0
    // 0x80017CE0: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    osViSetXScale_recomp(rdram, ctx);
        goto after_24;
    // 0x80017CE0: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_24:
    // 0x80017CE4: jal         0x80099B80
    // 0x80017CE8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    osViBlack_recomp(rdram, ctx);
        goto after_25;
    // 0x80017CE8: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_25:
    // 0x80017CEC: jal         0x80017EC0
    // 0x80017CF0: nop

    graphThread_submitGFX(rdram, ctx);
        goto after_26;
    // 0x80017CF0: nop

    after_26:
    // 0x80017CF4: b           L_80017C78
    // 0x80017CF8: nop

        goto L_80017C78;
    // 0x80017CF8: nop

L_80017CFC:
    // 0x80017CFC: lw          $t5, 0x20($s0)
    ctx->r13 = MEM_W(ctx->r16, 0X20);
    // 0x80017D00: lh          $t7, 0x2A($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X2A);
    // 0x80017D04: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
    // 0x80017D08: addiu       $t6, $t5, 0x1
    ctx->r14 = ADD32(ctx->r13, 0X1);
    // 0x80017D0C: beq         $t7, $zero, L_80017C78
    if (ctx->r15 == 0) {
        // 0x80017D10: sw          $t6, 0x20($s0)
        MEM_W(0X20, ctx->r16) = ctx->r14;
            goto L_80017C78;
    }
    // 0x80017D10: sw          $t6, 0x20($s0)
    MEM_W(0X20, ctx->r16) = ctx->r14;
    // 0x80017D14: jal         0x80099B80
    // 0x80017D18: sh          $zero, 0x2A($s0)
    MEM_H(0X2A, ctx->r16) = 0;
    osViBlack_recomp(rdram, ctx);
        goto after_27;
    // 0x80017D18: sh          $zero, 0x2A($s0)
    MEM_H(0X2A, ctx->r16) = 0;
    after_27:
    // 0x80017D1C: lh          $t8, 0x34($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X34);
    // 0x80017D20: lui         $at, 0x800C
    ctx->r1 = S32(0X800C << 16);
    // 0x80017D24: beq         $t8, $zero, L_80017C78
    if (ctx->r24 == 0) {
        // 0x80017D28: nop
    
            goto L_80017C78;
    }
    // 0x80017D28: nop

    // 0x80017D2C: jal         0x800A6F80
    // 0x80017D30: lwc1        $f12, -0x2F88($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X2F88);
    osViSetYScale_recomp(rdram, ctx);
        goto after_28;
    // 0x80017D30: lwc1        $f12, -0x2F88($at)
    ctx->f12.u32l = MEM_W(ctx->r1, -0X2F88);
    after_28:
    // 0x80017D34: lui         $at, 0x3F40
    ctx->r1 = S32(0X3F40 << 16);
    // 0x80017D38: mtc1        $at, $f12
    ctx->f12.u32l = ctx->r1;
    // 0x80017D3C: jal         0x800A70B0
    // 0x80017D40: nop

    osViSetXScale_recomp(rdram, ctx);
        goto after_29;
    // 0x80017D40: nop

    after_29:
    // 0x80017D44: b           L_80017C78
    // 0x80017D48: nop

        goto L_80017C78;
    // 0x80017D48: nop

    // 0x80017D4C: lhu         $t9, 0x0($s1)
    ctx->r25 = MEM_HU(ctx->r17, 0X0);
L_80017D50:
    // 0x80017D50: bne         $t9, $zero, L_80017C78
    if (ctx->r25 != 0) {
        // 0x80017D54: nop
    
            goto L_80017C78;
    }
    // 0x80017D54: nop

    // 0x80017D58: jal         0x80091108
    // 0x80017D5C: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    video_init(rdram, ctx);
        goto after_30;
    // 0x80017D5C: addiu       $a0, $zero, 0xA
    ctx->r4 = ADD32(0, 0XA);
    after_30:
    // 0x80017D60: jal         0x800A6F80
    // 0x80017D64: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    osViSetYScale_recomp(rdram, ctx);
        goto after_31;
    // 0x80017D64: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_31:
    // 0x80017D68: jal         0x800A70B0
    // 0x80017D6C: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    osViSetXScale_recomp(rdram, ctx);
        goto after_32;
    // 0x80017D6C: mov.s       $f12, $f20
    CHECK_FR(ctx, 12);
    CHECK_FR(ctx, 20);
    ctx->f12.fl = ctx->f20.fl;
    after_32:
    // 0x80017D70: jal         0x80099B80
    // 0x80017D74: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    osViBlack_recomp(rdram, ctx);
        goto after_33;
    // 0x80017D74: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_33:
    // 0x80017D78: addiu       $t0, $zero, 0x1
    ctx->r8 = ADD32(0, 0X1);
    // 0x80017D7C: b           L_80017C78
    // 0x80017D80: sh          $t0, 0x0($s1)
    MEM_H(0X0, ctx->r17) = ctx->r8;
        goto L_80017C78;
    // 0x80017D80: sh          $t0, 0x0($s1)
    MEM_H(0X0, ctx->r17) = ctx->r8;
    // 0x80017D84: nop

    // 0x80017D88: nop

    // 0x80017D8C: nop

    // 0x80017D90: nop

    // 0x80017D94: nop

    // 0x80017D98: nop

    // 0x80017D9C: nop

    // 0x80017DA0: lw          $ra, 0x24($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X24);
    // 0x80017DA4: ldc1        $f20, 0x10($sp)
    CHECK_FR(ctx, 20);
    ctx->f20.u64 = LD(ctx->r29, 0X10);
    // 0x80017DA8: lw          $s0, 0x1C($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X1C);
    // 0x80017DAC: lw          $s1, 0x20($sp)
    ctx->r17 = MEM_W(ctx->r29, 0X20);
    // 0x80017DB0: jr          $ra
    // 0x80017DB4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
    return;
    // 0x80017DB4: addiu       $sp, $sp, 0x48
    ctx->r29 = ADD32(ctx->r29, 0X48);
;}
RECOMP_FUNC void func_80017DB8(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017DB8: addiu       $sp, $sp, -0x20
    ctx->r29 = ADD32(ctx->r29, -0X20);
    // 0x80017DBC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80017DC0: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80017DC4: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80017DC8: lw          $t6, 0x0($s0)
    ctx->r14 = MEM_W(ctx->r16, 0X0);
    // 0x80017DCC: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80017DD0: addiu       $t7, $t6, 0x1
    ctx->r15 = ADD32(ctx->r14, 0X1);
    // 0x80017DD4: jal         0x800196DC
    // 0x80017DD8: sw          $t7, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r15;
    contpak_mainLoop(rdram, ctx);
        goto after_0;
    // 0x80017DD8: sw          $t7, 0x0($s0)
    MEM_W(0X0, ctx->r16) = ctx->r15;
    after_0:
    // 0x80017DDC: lh          $t8, 0x88($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X88);
    // 0x80017DE0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017DE4: lui         $t7, 0x2000
    ctx->r15 = S32(0X2000 << 16);
    // 0x80017DE8: addiu       $t9, $t8, -0x1
    ctx->r25 = ADD32(ctx->r24, -0X1);
    // 0x80017DEC: sh          $t9, 0x88($s0)
    MEM_H(0X88, ctx->r16) = ctx->r25;
    // 0x80017DF0: lh          $t0, 0x88($s0)
    ctx->r8 = MEM_H(ctx->r16, 0X88);
    // 0x80017DF4: bgezl       $t0, L_80017E44
    if (SIGNED(ctx->r8) >= 0) {
        // 0x80017DF8: sw          $t7, 0x10($s0)
        MEM_W(0X10, ctx->r16) = ctx->r15;
            goto L_80017E44;
    }
    goto skip_0;
    // 0x80017DF8: sw          $t7, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r15;
    skip_0:
    // 0x80017DFC: jal         0x80018E28
    // 0x80017E00: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    func_80018E28(rdram, ctx);
        goto after_1;
    // 0x80017E00: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    after_1:
    // 0x80017E04: lh          $t1, 0x24($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X24);
    // 0x80017E08: lui         $t3, 0x7FC0
    ctx->r11 = S32(0X7FC0 << 16);
    // 0x80017E0C: lui         $t6, 0x2000
    ctx->r14 = S32(0X2000 << 16);
    // 0x80017E10: sltu        $at, $v0, $t1
    ctx->r1 = ctx->r2 < ctx->r9 ? 1 : 0;
    // 0x80017E14: beq         $at, $zero, L_80017E38
    if (ctx->r1 == 0) {
        // 0x80017E18: nop
    
            goto L_80017E38;
    }
    // 0x80017E18: nop

    // 0x80017E1C: lhu         $t4, 0x52C($s0)
    ctx->r12 = MEM_HU(ctx->r16, 0X52C);
    // 0x80017E20: lh          $t2, 0x86($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X86);
    // 0x80017E24: sw          $t3, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r11;
    // 0x80017E28: addiu       $t5, $t4, 0x1
    ctx->r13 = ADD32(ctx->r12, 0X1);
    // 0x80017E2C: sh          $t5, 0x52C($s0)
    MEM_H(0X52C, ctx->r16) = ctx->r13;
    // 0x80017E30: b           L_80017E44
    // 0x80017E34: sh          $t2, 0x88($s0)
    MEM_H(0X88, ctx->r16) = ctx->r10;
        goto L_80017E44;
    // 0x80017E34: sh          $t2, 0x88($s0)
    MEM_H(0X88, ctx->r16) = ctx->r10;
L_80017E38:
    // 0x80017E38: b           L_80017E44
    // 0x80017E3C: sw          $t6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r14;
        goto L_80017E44;
    // 0x80017E3C: sw          $t6, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r14;
    // 0x80017E40: sw          $t7, 0x10($s0)
    MEM_W(0X10, ctx->r16) = ctx->r15;
L_80017E44:
    // 0x80017E44: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017E48: jal         0x80002950
    // 0x80017E4C: lw          $a0, 0x1520($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1520);
    object_dispatch1(rdram, ctx);
        goto after_2;
    // 0x80017E4C: lw          $a0, 0x1520($a0)
    ctx->r4 = MEM_W(ctx->r4, 0X1520);
    after_2:
    // 0x80017E50: lui         $t9, 0x800D
    ctx->r25 = S32(0X800D << 16);
    // 0x80017E54: addiu       $t9, $t9, -0x1898
    ctx->r25 = ADD32(ctx->r25, -0X1898);
    // 0x80017E58: addiu       $t8, $zero, 0x1
    ctx->r24 = ADD32(0, 0X1);
    // 0x80017E5C: jal         0x8001A25C
    // 0x80017E60: sb          $t8, 0x0($t9)
    MEM_B(0X0, ctx->r25) = ctx->r24;
    func_8001A25C(rdram, ctx);
        goto after_3;
    // 0x80017E60: sb          $t8, 0x0($t9)
    MEM_B(0X0, ctx->r25) = ctx->r24;
    after_3:
    // 0x80017E64: lui         $t0, 0x800D
    ctx->r8 = S32(0X800D << 16);
    // 0x80017E68: addiu       $t0, $t0, -0x1898
    ctx->r8 = ADD32(ctx->r8, -0X1898);
    // 0x80017E6C: sb          $zero, 0x0($t0)
    MEM_B(0X0, ctx->r8) = 0;
    // 0x80017E70: lh          $t2, 0x88($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X88);
    // 0x80017E74: lh          $t1, 0x86($s0)
    ctx->r9 = MEM_H(ctx->r16, 0X86);
    // 0x80017E78: bnel        $t1, $t2, L_80017E8C
    if (ctx->r9 != ctx->r10) {
        // 0x80017E7C: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80017E8C;
    }
    goto skip_1;
    // 0x80017E7C: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x80017E80: jal         0x800197F8
    // 0x80017E84: nop

    func_800197F8(rdram, ctx);
        goto after_4;
    // 0x80017E84: nop

    after_4:
    // 0x80017E88: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80017E8C:
    // 0x80017E8C: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80017E90: addiu       $sp, $sp, 0x20
    ctx->r29 = ADD32(ctx->r29, 0X20);
    // 0x80017E94: jr          $ra
    // 0x80017E98: nop

    return;
    // 0x80017E98: nop

;}
RECOMP_FUNC void func_80017E9C(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017E9C: lui         $v0, 0x801D
    ctx->r2 = S32(0X801D << 16);
    // 0x80017EA0: addiu       $v0, $v0, -0x7D40
    ctx->r2 = ADD32(ctx->r2, -0X7D40);
    // 0x80017EA4: lh          $t6, 0x26($v0)
    ctx->r14 = MEM_H(ctx->r2, 0X26);
    // 0x80017EA8: lui         $at, 0x800D
    ctx->r1 = S32(0X800D << 16);
    // 0x80017EAC: sll         $t7, $t6, 2
    ctx->r15 = S32(ctx->r14 << 2);
    // 0x80017EB0: addu        $t8, $v0, $t7
    ctx->r24 = ADD32(ctx->r2, ctx->r15);
    // 0x80017EB4: lw          $t9, 0x14($t8)
    ctx->r25 = MEM_W(ctx->r24, 0X14);
    // 0x80017EB8: jr          $ra
    // 0x80017EBC: sw          $t9, -0x18A0($at)
    MEM_W(-0X18A0, ctx->r1) = ctx->r25;
    return;
    // 0x80017EBC: sw          $t9, -0x18A0($at)
    MEM_W(-0X18A0, ctx->r1) = ctx->r25;
;}
RECOMP_FUNC void graphThread_submitGFX(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80017EC0: addiu       $sp, $sp, -0x38
    ctx->r29 = ADD32(ctx->r29, -0X38);
    // 0x80017EC4: sw          $ra, 0x1C($sp)
    MEM_W(0X1C, ctx->r29) = ctx->r31;
    // 0x80017EC8: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017ECC: sw          $s0, 0x18($sp)
    MEM_W(0X18, ctx->r29) = ctx->r16;
    // 0x80017ED0: jal         0x80018E28
    // 0x80017ED4: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    func_80018E28(rdram, ctx);
        goto after_0;
    // 0x80017ED4: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    after_0:
    // 0x80017ED8: sll         $v1, $v0, 16
    ctx->r3 = S32(ctx->r2 << 16);
    // 0x80017EDC: sra         $v1, $v1, 16
    ctx->r3 = S32(SIGNED(ctx->r3) >> 16);
    // 0x80017EE0: bne         $v1, $zero, L_80017EF8
    if (ctx->r3 != 0) {
        // 0x80017EE4: lui         $t1, 0x800B
        ctx->r9 = S32(0X800B << 16);
            goto L_80017EF8;
    }
    // 0x80017EE4: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80017EE8: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x80017EEC: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
    // 0x80017EF0: beq         $v0, $zero, L_80017F1C
    if (ctx->r2 == 0) {
        // 0x80017EF4: nop
    
            goto L_80017F1C;
    }
    // 0x80017EF4: nop

L_80017EF8:
    // 0x80017EF8: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80017EFC: slti        $at, $v1, 0x2
    ctx->r1 = SIGNED(ctx->r3) < 0X2 ? 1 : 0;
    // 0x80017F00: beq         $at, $zero, L_80018360
    if (ctx->r1 == 0) {
        // 0x80017F04: addiu       $t1, $t1, 0x8C
        ctx->r9 = ADD32(ctx->r9, 0X8C);
            goto L_80018360;
    }
    // 0x80017F04: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x80017F08: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
    // 0x80017F0C: beq         $v0, $zero, L_80018360
    if (ctx->r2 == 0) {
        // 0x80017F10: slti        $at, $v0, 0x11
        ctx->r1 = SIGNED(ctx->r2) < 0X11 ? 1 : 0;
            goto L_80018360;
    }
    // 0x80017F10: slti        $at, $v0, 0x11
    ctx->r1 = SIGNED(ctx->r2) < 0X11 ? 1 : 0;
    // 0x80017F14: beql        $at, $zero, L_80018364
    if (ctx->r1 == 0) {
        // 0x80017F18: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80018364;
    }
    goto skip_0;
    // 0x80017F18: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_0:
L_80017F1C:
    // 0x80017F1C: bne         $v0, $zero, L_8001806C
    if (ctx->r2 != 0) {
        // 0x80017F20: lui         $a0, 0x800C
        ctx->r4 = S32(0X800C << 16);
            goto L_8001806C;
    }
    // 0x80017F20: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x80017F24: jal         0x8001876C
    // 0x80017F28: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    scheduler_getAudioCmdQ(rdram, ctx);
        goto after_1;
    // 0x80017F28: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    after_1:
    // 0x80017F2C: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80017F30: beq         $v0, $zero, L_80017F58
    if (ctx->r2 == 0) {
        // 0x80017F34: addiu       $t1, $t1, 0x8C
        ctx->r9 = ADD32(ctx->r9, 0X8C);
            goto L_80017F58;
    }
    // 0x80017F34: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x80017F38: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80017F3C: beq         $v0, $at, L_80017FB8
    if (ctx->r2 == ctx->r1) {
        // 0x80017F40: lui         $s0, 0x801D
        ctx->r16 = S32(0X801D << 16);
            goto L_80017FB8;
    }
    // 0x80017F40: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80017F44: addiu       $at, $zero, 0x2
    ctx->r1 = ADD32(0, 0X2);
    // 0x80017F48: beq         $v0, $at, L_80018014
    if (ctx->r2 == ctx->r1) {
        // 0x80017F4C: lui         $s0, 0x801D
        ctx->r16 = S32(0X801D << 16);
            goto L_80018014;
    }
    // 0x80017F4C: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80017F50: b           L_8001806C
    // 0x80017F54: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
        goto L_8001806C;
    // 0x80017F54: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
L_80017F58:
    // 0x80017F58: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80017F5C: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80017F60: lh          $t6, 0x4C($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X4C);
    // 0x80017F64: lh          $t7, 0x4E($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X4E);
    // 0x80017F68: lw          $a0, 0x44($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X44);
    // 0x80017F6C: lw          $a1, 0x3C($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X3C);
    // 0x80017F70: multu       $t6, $t7
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r15)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80017F74: mflo        $a2
    ctx->r6 = lo;
    // 0x80017F78: sll         $a2, $a2, 1
    ctx->r6 = S32(ctx->r6 << 1);
    // 0x80017F7C: jal         0x80001090
    // 0x80017F80: nop

    memory_copy(rdram, ctx);
        goto after_2;
    // 0x80017F80: nop

    after_2:
    // 0x80017F84: lh          $t8, 0x4C($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X4C);
    // 0x80017F88: lh          $t9, 0x4E($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X4E);
    // 0x80017F8C: lw          $a0, 0x44($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X44);
    // 0x80017F90: lw          $a1, 0x40($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X40);
    // 0x80017F94: multu       $t8, $t9
    result = U64(U32(ctx->r24)) * U64(U32(ctx->r25)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80017F98: mflo        $a2
    ctx->r6 = lo;
    // 0x80017F9C: sll         $a2, $a2, 1
    ctx->r6 = S32(ctx->r6 << 1);
    // 0x80017FA0: jal         0x80001090
    // 0x80017FA4: nop

    memory_copy(rdram, ctx);
        goto after_3;
    // 0x80017FA4: nop

    after_3:
    // 0x80017FA8: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80017FAC: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x80017FB0: b           L_8001806C
    // 0x80017FB4: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
        goto L_8001806C;
    // 0x80017FB4: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
L_80017FB8:
    // 0x80017FB8: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80017FBC: lh          $t2, 0x4C($s0)
    ctx->r10 = MEM_H(ctx->r16, 0X4C);
    // 0x80017FC0: lh          $t3, 0x4E($s0)
    ctx->r11 = MEM_H(ctx->r16, 0X4E);
    // 0x80017FC4: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    // 0x80017FC8: lw          $a1, 0x40($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X40);
    // 0x80017FCC: multu       $t2, $t3
    result = U64(U32(ctx->r10)) * U64(U32(ctx->r11)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80017FD0: mflo        $a2
    ctx->r6 = lo;
    // 0x80017FD4: sll         $a2, $a2, 1
    ctx->r6 = S32(ctx->r6 << 1);
    // 0x80017FD8: jal         0x80001090
    // 0x80017FDC: nop

    memory_copy(rdram, ctx);
        goto after_4;
    // 0x80017FDC: nop

    after_4:
    // 0x80017FE0: lh          $t4, 0x4C($s0)
    ctx->r12 = MEM_H(ctx->r16, 0X4C);
    // 0x80017FE4: lh          $t5, 0x4E($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X4E);
    // 0x80017FE8: lw          $a0, 0x3C($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X3C);
    // 0x80017FEC: lw          $a1, 0x44($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X44);
    // 0x80017FF0: multu       $t4, $t5
    result = U64(U32(ctx->r12)) * U64(U32(ctx->r13)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80017FF4: mflo        $a2
    ctx->r6 = lo;
    // 0x80017FF8: sll         $a2, $a2, 1
    ctx->r6 = S32(ctx->r6 << 1);
    // 0x80017FFC: jal         0x80001090
    // 0x80018000: nop

    memory_copy(rdram, ctx);
        goto after_5;
    // 0x80018000: nop

    after_5:
    // 0x80018004: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80018008: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x8001800C: b           L_8001806C
    // 0x80018010: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
        goto L_8001806C;
    // 0x80018010: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
L_80018014:
    // 0x80018014: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80018018: lh          $t6, 0x4C($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X4C);
    // 0x8001801C: lh          $t7, 0x4E($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X4E);
    // 0x80018020: lw          $a0, 0x40($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X40);
    // 0x80018024: lw          $a1, 0x3C($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X3C);
    // 0x80018028: multu       $t6, $t7
    result = U64(U32(ctx->r14)) * U64(U32(ctx->r15)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x8001802C: mflo        $a2
    ctx->r6 = lo;
    // 0x80018030: sll         $a2, $a2, 1
    ctx->r6 = S32(ctx->r6 << 1);
    // 0x80018034: jal         0x80001090
    // 0x80018038: nop

    memory_copy(rdram, ctx);
        goto after_6;
    // 0x80018038: nop

    after_6:
    // 0x8001803C: lh          $t8, 0x4C($s0)
    ctx->r24 = MEM_H(ctx->r16, 0X4C);
    // 0x80018040: lh          $t9, 0x4E($s0)
    ctx->r25 = MEM_H(ctx->r16, 0X4E);
    // 0x80018044: lw          $a0, 0x40($s0)
    ctx->r4 = MEM_W(ctx->r16, 0X40);
    // 0x80018048: lw          $a1, 0x44($s0)
    ctx->r5 = MEM_W(ctx->r16, 0X44);
    // 0x8001804C: multu       $t8, $t9
    result = U64(U32(ctx->r24)) * U64(U32(ctx->r25)); lo = S32(result >> 0); hi = S32(result >> 32);
    // 0x80018050: mflo        $a2
    ctx->r6 = lo;
    // 0x80018054: sll         $a2, $a2, 1
    ctx->r6 = S32(ctx->r6 << 1);
    // 0x80018058: jal         0x80001090
    // 0x8001805C: nop

    memory_copy(rdram, ctx);
        goto after_7;
    // 0x8001805C: nop

    after_7:
    // 0x80018060: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80018064: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x80018068: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
L_8001806C:
    // 0x8001806C: lui         $s0, 0x801D
    ctx->r16 = S32(0X801D << 16);
    // 0x80018070: addiu       $t2, $v0, 0x1
    ctx->r10 = ADD32(ctx->r2, 0X1);
    // 0x80018074: addiu       $s0, $s0, -0x7D40
    ctx->r16 = ADD32(ctx->r16, -0X7D40);
    // 0x80018078: jal         0x80017E9C
    // 0x8001807C: sh          $t2, 0x0($t1)
    MEM_H(0X0, ctx->r9) = ctx->r10;
    func_80017E9C(rdram, ctx);
        goto after_8;
    // 0x8001807C: sh          $t2, 0x0($t1)
    MEM_H(0X0, ctx->r9) = ctx->r10;
    after_8:
    // 0x80018080: lui         $a3, 0x800D
    ctx->r7 = S32(0X800D << 16);
    // 0x80018084: addiu       $a3, $a3, -0x18A0
    ctx->r7 = ADD32(ctx->r7, -0X18A0);
    // 0x80018088: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x8001808C: lui         $t4, 0xDB06
    ctx->r12 = S32(0XDB06 << 16);
    // 0x80018090: lui         $t6, 0xE300
    ctx->r14 = S32(0XE300 << 16);
    // 0x80018094: addiu       $t3, $v1, 0x8
    ctx->r11 = ADD32(ctx->r3, 0X8);
    // 0x80018098: sw          $t3, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r11;
    // 0x8001809C: sw          $t4, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r12;
    // 0x800180A0: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x800180A4: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x800180A8: ori         $t6, $t6, 0xA01
    ctx->r14 = ctx->r14 | 0XA01;
    // 0x800180AC: lui         $t7, 0x30
    ctx->r15 = S32(0X30 << 16);
    // 0x800180B0: addiu       $t5, $v1, 0x8
    ctx->r13 = ADD32(ctx->r3, 0X8);
    // 0x800180B4: sw          $t5, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r13;
    // 0x800180B8: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    // 0x800180BC: sw          $t7, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r15;
    // 0x800180C0: lhu         $t9, 0x7A($s0)
    ctx->r25 = MEM_HU(ctx->r16, 0X7A);
    // 0x800180C4: lh          $t5, 0x4C($s0)
    ctx->r13 = MEM_H(ctx->r16, 0X4C);
    // 0x800180C8: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x800180CC: andi        $t2, $t9, 0x3
    ctx->r10 = ctx->r25 & 0X3;
    // 0x800180D0: sll         $t3, $t2, 19
    ctx->r11 = S32(ctx->r10 << 19);
    // 0x800180D4: lui         $at, 0xFF00
    ctx->r1 = S32(0XFF00 << 16);
    // 0x800180D8: addiu       $t6, $t5, -0x1
    ctx->r14 = ADD32(ctx->r13, -0X1);
    // 0x800180DC: addiu       $t8, $v1, 0x8
    ctx->r24 = ADD32(ctx->r3, 0X8);
    // 0x800180E0: sw          $t8, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r24;
    // 0x800180E4: andi        $t7, $t6, 0xFFF
    ctx->r15 = ctx->r14 & 0XFFF;
    // 0x800180E8: or          $t4, $t3, $at
    ctx->r12 = ctx->r11 | ctx->r1;
    // 0x800180EC: or          $t8, $t4, $t7
    ctx->r24 = ctx->r12 | ctx->r15;
    // 0x800180F0: lui         $a0, 0x800C
    ctx->r4 = S32(0X800C << 16);
    // 0x800180F4: sw          $t8, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r24;
    // 0x800180F8: addiu       $a0, $a0, 0x5E80
    ctx->r4 = ADD32(ctx->r4, 0X5E80);
    // 0x800180FC: jal         0x8001876C
    // 0x80018100: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    scheduler_getAudioCmdQ(rdram, ctx);
        goto after_9;
    // 0x80018100: sw          $v1, 0x24($sp)
    MEM_W(0X24, ctx->r29) = ctx->r3;
    after_9:
    // 0x80018104: sll         $t9, $v0, 2
    ctx->r25 = S32(ctx->r2 << 2);
    // 0x80018108: addu        $t2, $s0, $t9
    ctx->r10 = ADD32(ctx->r16, ctx->r25);
    // 0x8001810C: lw          $t3, 0x3C($t2)
    ctx->r11 = MEM_W(ctx->r10, 0X3C);
    // 0x80018110: lui         $at, 0x4080
    ctx->r1 = S32(0X4080 << 16);
    // 0x80018114: mtc1        $at, $f0
    ctx->f0.u32l = ctx->r1;
    // 0x80018118: lw          $t0, 0x24($sp)
    ctx->r8 = MEM_W(ctx->r29, 0X24);
    // 0x8001811C: lui         $at, 0x8000
    ctx->r1 = S32(0X8000 << 16);
    // 0x80018120: lui         $a3, 0x800D
    ctx->r7 = S32(0X800D << 16);
    // 0x80018124: addu        $t5, $t3, $at
    ctx->r13 = ADD32(ctx->r11, ctx->r1);
    // 0x80018128: addiu       $a3, $a3, -0x18A0
    ctx->r7 = ADD32(ctx->r7, -0X18A0);
    // 0x8001812C: sw          $t5, 0x4($t0)
    MEM_W(0X4, ctx->r8) = ctx->r13;
    // 0x80018130: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x80018134: lui         $t4, 0xED00
    ctx->r12 = S32(0XED00 << 16);
    // 0x80018138: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x8001813C: addiu       $t6, $v1, 0x8
    ctx->r14 = ADD32(ctx->r3, 0X8);
    // 0x80018140: sw          $t6, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r14;
    // 0x80018144: sw          $t4, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r12;
    // 0x80018148: lh          $t7, 0x4C($s0)
    ctx->r15 = MEM_H(ctx->r16, 0X4C);
    // 0x8001814C: lh          $t6, 0x4E($s0)
    ctx->r14 = MEM_H(ctx->r16, 0X4E);
    // 0x80018150: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x80018154: addiu       $t8, $t7, -0x1
    ctx->r24 = ADD32(ctx->r15, -0X1);
    // 0x80018158: mtc1        $t8, $f4
    ctx->f4.u32l = ctx->r24;
    // 0x8001815C: addiu       $t4, $t6, -0x1
    ctx->r12 = ADD32(ctx->r14, -0X1);
    // 0x80018160: mtc1        $t4, $f16
    ctx->f16.u32l = ctx->r12;
    // 0x80018164: cvt.s.w     $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.fl = CVT_S_W(ctx->f4.u32l);
    // 0x80018168: lui         $t4, 0x1
    ctx->r12 = S32(0X1 << 16);
    // 0x8001816C: ori         $t4, $t4, 0x1
    ctx->r12 = ctx->r12 | 0X1;
    // 0x80018170: lui         $t6, 0xF700
    ctx->r14 = S32(0XF700 << 16);
    // 0x80018174: lui         $t0, 0xF600
    ctx->r8 = S32(0XF600 << 16);
    // 0x80018178: cvt.s.w     $f18, $f16
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 16);
    ctx->f18.fl = CVT_S_W(ctx->f16.u32l);
    // 0x8001817C: mul.s       $f8, $f6, $f0
    CHECK_FR(ctx, 8);
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f6.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f8.fl = MUL_S(ctx->f6.fl, ctx->f0.fl);
    // 0x80018180: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
    // 0x80018184: mul.s       $f4, $f18, $f0
    CHECK_FR(ctx, 4);
    CHECK_FR(ctx, 18);
    CHECK_FR(ctx, 0);
    NAN_CHECK(ctx->f18.fl); NAN_CHECK(ctx->f0.fl); 
    ctx->f4.fl = MUL_S(ctx->f18.fl, ctx->f0.fl);
    // 0x80018188: trunc.w.s   $f10, $f8
    CHECK_FR(ctx, 10);
    CHECK_FR(ctx, 8);
    ctx->f10.u32l = TRUNC_W_S(ctx->f8.fl);
    // 0x8001818C: trunc.w.s   $f6, $f4
    CHECK_FR(ctx, 6);
    CHECK_FR(ctx, 4);
    ctx->f6.u32l = TRUNC_W_S(ctx->f4.fl);
    // 0x80018190: mfc1        $t2, $f10
    ctx->r10 = (int32_t)ctx->f10.u32l;
    // 0x80018194: mfc1        $t8, $f6
    ctx->r24 = (int32_t)ctx->f6.u32l;
    // 0x80018198: andi        $t3, $t2, 0xFFF
    ctx->r11 = ctx->r10 & 0XFFF;
    // 0x8001819C: sll         $t5, $t3, 12
    ctx->r13 = S32(ctx->r11 << 12);
    // 0x800181A0: andi        $t9, $t8, 0xFFF
    ctx->r25 = ctx->r24 & 0XFFF;
    // 0x800181A4: or          $t2, $t5, $t9
    ctx->r10 = ctx->r13 | ctx->r25;
    // 0x800181A8: sw          $t2, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r10;
    // 0x800181AC: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x800181B0: addiu       $t3, $v1, 0x8
    ctx->r11 = ADD32(ctx->r3, 0X8);
    // 0x800181B4: sw          $t3, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r11;
    // 0x800181B8: sw          $t4, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r12;
    // 0x800181BC: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
L_800181C0:
    // 0x800181C0: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
    // 0x800181C4: bgez        $v0, L_800181D0
    if (SIGNED(ctx->r2) >= 0) {
        // 0x800181C8: addu        $at, $v0, $zero
        ctx->r1 = ADD32(ctx->r2, 0);
            goto L_800181D0;
    }
    // 0x800181C8: addu        $at, $v0, $zero
    ctx->r1 = ADD32(ctx->r2, 0);
    // 0x800181CC: addiu       $at, $v0, 0x1
    ctx->r1 = ADD32(ctx->r2, 0X1);
L_800181D0:
    // 0x800181D0: sra         $v0, $at, 1
    ctx->r2 = S32(SIGNED(ctx->r1) >> 1);
    // 0x800181D4: subu        $a0, $a1, $v0
    ctx->r4 = SUB32(ctx->r5, ctx->r2);
    // 0x800181D8: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x800181DC: addu        $a2, $a1, $v0
    ctx->r6 = ADD32(ctx->r5, ctx->r2);
    // 0x800181E0: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x800181E4: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x800181E8: bgez        $a0, L_800181F4
    if (SIGNED(ctx->r4) >= 0) {
        // 0x800181EC: sra         $a2, $a2, 16
        ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
            goto L_800181F4;
    }
    // 0x800181EC: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x800181F0: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
L_800181F4:
    // 0x800181F4: slti        $at, $a0, 0x140
    ctx->r1 = SIGNED(ctx->r4) < 0X140 ? 1 : 0;
    // 0x800181F8: bne         $at, $zero, L_80018204
    if (ctx->r1 != 0) {
        // 0x800181FC: addiu       $a1, $a1, 0x10
        ctx->r5 = ADD32(ctx->r5, 0X10);
            goto L_80018204;
    }
    // 0x800181FC: addiu       $a1, $a1, 0x10
    ctx->r5 = ADD32(ctx->r5, 0X10);
    // 0x80018200: addiu       $a0, $zero, 0x13F
    ctx->r4 = ADD32(0, 0X13F);
L_80018204:
    // 0x80018204: slti        $at, $a2, 0x140
    ctx->r1 = SIGNED(ctx->r6) < 0X140 ? 1 : 0;
    // 0x80018208: bne         $at, $zero, L_80018214
    if (ctx->r1 != 0) {
        // 0x8001820C: andi        $t3, $a0, 0x3FF
        ctx->r11 = ctx->r4 & 0X3FF;
            goto L_80018214;
    }
    // 0x8001820C: andi        $t3, $a0, 0x3FF
    ctx->r11 = ctx->r4 & 0X3FF;
    // 0x80018210: addiu       $a2, $zero, 0x13F
    ctx->r6 = ADD32(0, 0X13F);
L_80018214:
    // 0x80018214: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x80018218: andi        $t8, $a2, 0x3FF
    ctx->r24 = ctx->r6 & 0X3FF;
    // 0x8001821C: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x80018220: sll         $t5, $t8, 14
    ctx->r13 = S32(ctx->r24 << 14);
    // 0x80018224: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x80018228: or          $t9, $t5, $t0
    ctx->r25 = ctx->r13 | ctx->r8;
    // 0x8001822C: addiu       $t7, $v1, 0x8
    ctx->r15 = ADD32(ctx->r3, 0X8);
    // 0x80018230: slti        $at, $a1, 0x141
    ctx->r1 = SIGNED(ctx->r5) < 0X141 ? 1 : 0;
    // 0x80018234: sw          $t7, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r15;
    // 0x80018238: ori         $t2, $t9, 0x3BC
    ctx->r10 = ctx->r25 | 0X3BC;
    // 0x8001823C: sll         $t6, $t3, 14
    ctx->r14 = S32(ctx->r11 << 14);
    // 0x80018240: sw          $t6, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r14;
    // 0x80018244: bne         $at, $zero, L_800181C0
    if (ctx->r1 != 0) {
        // 0x80018248: sw          $t2, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r10;
            goto L_800181C0;
    }
    // 0x80018248: sw          $t2, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r10;
    // 0x8001824C: lui         $t0, 0xF64F
    ctx->r8 = S32(0XF64F << 16);
    // 0x80018250: ori         $t0, $t0, 0xC000
    ctx->r8 = ctx->r8 | 0XC000;
    // 0x80018254: addiu       $a1, $zero, 0x8
    ctx->r5 = ADD32(0, 0X8);
L_80018258:
    // 0x80018258: lhu         $v0, 0x0($t1)
    ctx->r2 = MEM_HU(ctx->r9, 0X0);
    // 0x8001825C: bgez        $v0, L_80018268
    if (SIGNED(ctx->r2) >= 0) {
        // 0x80018260: addu        $at, $v0, $zero
        ctx->r1 = ADD32(ctx->r2, 0);
            goto L_80018268;
    }
    // 0x80018260: addu        $at, $v0, $zero
    ctx->r1 = ADD32(ctx->r2, 0);
    // 0x80018264: addiu       $at, $v0, 0x1
    ctx->r1 = ADD32(ctx->r2, 0X1);
L_80018268:
    // 0x80018268: sra         $v0, $at, 1
    ctx->r2 = S32(SIGNED(ctx->r1) >> 1);
    // 0x8001826C: subu        $a0, $a1, $v0
    ctx->r4 = SUB32(ctx->r5, ctx->r2);
    // 0x80018270: sll         $a0, $a0, 16
    ctx->r4 = S32(ctx->r4 << 16);
    // 0x80018274: addu        $a2, $a1, $v0
    ctx->r6 = ADD32(ctx->r5, ctx->r2);
    // 0x80018278: sra         $a0, $a0, 16
    ctx->r4 = S32(SIGNED(ctx->r4) >> 16);
    // 0x8001827C: sll         $a2, $a2, 16
    ctx->r6 = S32(ctx->r6 << 16);
    // 0x80018280: bgez        $a0, L_8001828C
    if (SIGNED(ctx->r4) >= 0) {
        // 0x80018284: sra         $a2, $a2, 16
        ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
            goto L_8001828C;
    }
    // 0x80018284: sra         $a2, $a2, 16
    ctx->r6 = S32(SIGNED(ctx->r6) >> 16);
    // 0x80018288: or          $a0, $zero, $zero
    ctx->r4 = 0 | 0;
L_8001828C:
    // 0x8001828C: slti        $at, $a0, 0xF0
    ctx->r1 = SIGNED(ctx->r4) < 0XF0 ? 1 : 0;
    // 0x80018290: bne         $at, $zero, L_8001829C
    if (ctx->r1 != 0) {
        // 0x80018294: addiu       $a1, $a1, 0x10
        ctx->r5 = ADD32(ctx->r5, 0X10);
            goto L_8001829C;
    }
    // 0x80018294: addiu       $a1, $a1, 0x10
    ctx->r5 = ADD32(ctx->r5, 0X10);
    // 0x80018298: addiu       $a0, $zero, 0xEF
    ctx->r4 = ADD32(0, 0XEF);
L_8001829C:
    // 0x8001829C: slti        $at, $a2, 0xF0
    ctx->r1 = SIGNED(ctx->r6) < 0XF0 ? 1 : 0;
    // 0x800182A0: bne         $at, $zero, L_800182AC
    if (ctx->r1 != 0) {
        // 0x800182A4: andi        $t9, $a0, 0x3FF
        ctx->r25 = ctx->r4 & 0X3FF;
            goto L_800182AC;
    }
    // 0x800182A4: andi        $t9, $a0, 0x3FF
    ctx->r25 = ctx->r4 & 0X3FF;
    // 0x800182A8: addiu       $a2, $zero, 0xEF
    ctx->r6 = ADD32(0, 0XEF);
L_800182AC:
    // 0x800182AC: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x800182B0: sll         $a1, $a1, 16
    ctx->r5 = S32(ctx->r5 << 16);
    // 0x800182B4: andi        $t7, $a2, 0x3FF
    ctx->r15 = ctx->r6 & 0X3FF;
    // 0x800182B8: sra         $a1, $a1, 16
    ctx->r5 = S32(SIGNED(ctx->r5) >> 16);
    // 0x800182BC: sll         $t8, $t7, 2
    ctx->r24 = S32(ctx->r15 << 2);
    // 0x800182C0: addiu       $t4, $v1, 0x8
    ctx->r12 = ADD32(ctx->r3, 0X8);
    // 0x800182C4: slti        $at, $a1, 0xF1
    ctx->r1 = SIGNED(ctx->r5) < 0XF1 ? 1 : 0;
    // 0x800182C8: sw          $t4, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r12;
    // 0x800182CC: or          $t5, $t8, $t0
    ctx->r13 = ctx->r24 | ctx->r8;
    // 0x800182D0: sll         $t2, $t9, 2
    ctx->r10 = S32(ctx->r25 << 2);
    // 0x800182D4: sw          $t2, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r10;
    // 0x800182D8: bne         $at, $zero, L_80018258
    if (ctx->r1 != 0) {
        // 0x800182DC: sw          $t5, 0x0($v1)
        MEM_W(0X0, ctx->r3) = ctx->r13;
            goto L_80018258;
    }
    // 0x800182DC: sw          $t5, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r13;
    // 0x800182E0: lw          $v1, 0x0($a3)
    ctx->r3 = MEM_W(ctx->r7, 0X0);
    // 0x800182E4: lui         $t6, 0xE700
    ctx->r14 = S32(0XE700 << 16);
    // 0x800182E8: addiu       $t3, $v1, 0x8
    ctx->r11 = ADD32(ctx->r3, 0X8);
    // 0x800182EC: sw          $t3, 0x0($a3)
    MEM_W(0X0, ctx->r7) = ctx->r11;
    // 0x800182F0: sw          $zero, 0x4($v1)
    MEM_W(0X4, ctx->r3) = 0;
    // 0x800182F4: jal         0x80019034
    // 0x800182F8: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    createGraphicTasks(rdram, ctx);
        goto after_10;
    // 0x800182F8: sw          $t6, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r14;
    after_10:
    // 0x800182FC: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80018300: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
    // 0x80018304: lhu         $t4, 0x0($t1)
    ctx->r12 = MEM_HU(ctx->r9, 0X0);
    // 0x80018308: addiu       $at, $zero, 0x11
    ctx->r1 = ADD32(0, 0X11);
    // 0x8001830C: bne         $t4, $at, L_80018324
    if (ctx->r12 != ctx->r1) {
        // 0x80018310: nop
    
            goto L_80018324;
    }
    // 0x80018310: nop

    // 0x80018314: jal         0x80091108
    // 0x80018318: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    video_init(rdram, ctx);
        goto after_11;
    // 0x80018318: addiu       $a0, $zero, 0x1
    ctx->r4 = ADD32(0, 0X1);
    after_11:
    // 0x8001831C: lui         $t1, 0x800B
    ctx->r9 = S32(0X800B << 16);
    // 0x80018320: addiu       $t1, $t1, 0x8C
    ctx->r9 = ADD32(ctx->r9, 0X8C);
L_80018324:
    // 0x80018324: lui         $s0, 0x800B
    ctx->r16 = S32(0X800B << 16);
    // 0x80018328: addiu       $s0, $s0, 0x88
    ctx->r16 = ADD32(ctx->r16, 0X88);
    // 0x8001832C: lhu         $t7, 0x0($s0)
    ctx->r15 = MEM_HU(ctx->r16, 0X0);
    // 0x80018330: addiu       $at, $zero, 0x1
    ctx->r1 = ADD32(0, 0X1);
    // 0x80018334: bnel        $t7, $at, L_80018364
    if (ctx->r15 != ctx->r1) {
        // 0x80018338: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80018364;
    }
    goto skip_1;
    // 0x80018338: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_1:
    // 0x8001833C: lhu         $t8, 0x0($t1)
    ctx->r24 = MEM_HU(ctx->r9, 0X0);
    // 0x80018340: slti        $at, $t8, 0x13
    ctx->r1 = SIGNED(ctx->r24) < 0X13 ? 1 : 0;
    // 0x80018344: bnel        $at, $zero, L_80018364
    if (ctx->r1 != 0) {
        // 0x80018348: lw          $ra, 0x1C($sp)
        ctx->r31 = MEM_W(ctx->r29, 0X1C);
            goto L_80018364;
    }
    goto skip_2;
    // 0x80018348: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
    skip_2:
    // 0x8001834C: jal         0x800A01F0
    // 0x80018350: nop

    func_800A01F0(rdram, ctx);
        goto after_12;
    // 0x80018350: nop

    after_12:
    // 0x80018354: bne         $v0, $zero, L_80018360
    if (ctx->r2 != 0) {
        // 0x80018358: addiu       $t5, $zero, 0x2
        ctx->r13 = ADD32(0, 0X2);
            goto L_80018360;
    }
    // 0x80018358: addiu       $t5, $zero, 0x2
    ctx->r13 = ADD32(0, 0X2);
    // 0x8001835C: sh          $t5, 0x0($s0)
    MEM_H(0X0, ctx->r16) = ctx->r13;
L_80018360:
    // 0x80018360: lw          $ra, 0x1C($sp)
    ctx->r31 = MEM_W(ctx->r29, 0X1C);
L_80018364:
    // 0x80018364: lw          $s0, 0x18($sp)
    ctx->r16 = MEM_W(ctx->r29, 0X18);
    // 0x80018368: addiu       $sp, $sp, 0x38
    ctx->r29 = ADD32(ctx->r29, 0X38);
    // 0x8001836C: jr          $ra
    // 0x80018370: nop

    return;
    // 0x80018370: nop

;}
RECOMP_FUNC void func_80018374(uint8_t* rdram, recomp_context* ctx) {
    uint64_t hi = 0, lo = 0, result = 0;
    int c1cs = 0;
    // 0x80018374: lui         $a0, 0x800D
    ctx->r4 = S32(0X800D << 16);
    // 0x80018378: addiu       $a0, $a0, -0x18A0
    ctx->r4 = ADD32(ctx->r4, -0X18A0);
    // 0x8001837C: lw          $v1, 0x0($a0)
    ctx->r3 = MEM_W(ctx->r4, 0X0);
    // 0x80018380: lui         $t8, 0x800B
    ctx->r24 = S32(0X800B << 16);
    // 0x80018384: addiu       $t8, $t8, 0x90
    ctx->r24 = ADD32(ctx->r24, 0X90);
    // 0x80018388: addiu       $t6, $v1, 0x8
    ctx->r14 = ADD32(ctx->r3, 0X8);
    // 0x8001838C: sw          $t6, 0x0($a0)
    MEM_W(0X0, ctx->r4) = ctx->r14;
    // 0x80018390: lui         $t7, 0xDE00
    ctx->r15 = S32(0XDE00 << 16);
    // 0x80018394: sw          $t7, 0x0($v1)
    MEM_W(0X0, ctx->r3) = ctx->r15;
    // 0x80018398: jr          $ra
    // 0x8001839C: sw          $t8, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r24;
    return;
    // 0x8001839C: sw          $t8, 0x4($v1)
    MEM_W(0X4, ctx->r3) = ctx->r24;
;}
